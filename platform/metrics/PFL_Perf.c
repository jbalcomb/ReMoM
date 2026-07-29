/**
 * PFL_Perf.c -- zone recorder and percentile reporter for the performance-measurement layer.
 *
 * See Platform_Perf.h for the API contract and doc/#AI_Plans/{BRA,PRD,PLAN}-Performance-Management.md
 * for why the layer exists.  Backend-agnostic: nothing here touches SDL, Win32, or engine globals.
 *
 * OBSERVER EFFECT IS THE DESIGN CONSTRAINT
 * ----------------------------------------
 * The whole module exists to measure time, so anything it does on the hot path is a lie added to
 * the measurement.  Therefore:
 *   - A zone body is one clock read plus a handful of stores into a PREALLOCATED array.  No
 *     formatting, no allocation, no I/O, no logging -- the three things the old PHASE macro did
 *     wrong (it called LOG_INFO inside the timed region).
 *   - The buffer is allocated once at Perf_Init() and never grows.
 *   - Every row and every percentile is produced in Perf_Shutdown(), after all measurement is over.
 *
 * The deliberate consequence: a capture that dies before Perf_Shutdown() (SIGSEGV, kill) is LOST.
 * That is the opposite trade from PFL_Input_Metrics, which flushes every ~0.5 s because its job is
 * diagnosing sessions that crash.  A zone capture's job is profiling a DETERMINISTIC scenario
 * (HeMoM headless with a fixed seed and a committed save) -- if it crashes you rerun it and get the
 * same run back, so buying crash-safety with a periodic fflush inside somebody's timed region
 * would be paying the observer effect for nothing.  If the record buffer does fill mid-run, the
 * rows are drained to disk at that moment and the file records a #note so the perturbed region is
 * identifiable rather than silently believed.
 */

#include "Platform_Perf.h"

#include "Platform.h"
#include "../../STU/src/STU_LOG.h"
#include "../../ext/stu_compat.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Ring capacity.  32 bytes/record; 262144 records = 8 MB, allocated only when perf is active.
 * A Mode A turn-pipeline capture produces tens of records per turn, so this holds thousands of
 * turns; a per-frame capture at ~18 fps holds hours.  Overflow drains and notes itself. */
#define PERF_MAX_RECORDS   262144u

/* Deepest zone nesting.  The stage tree is coarse by design (PRD: keep zone density low); 64 is
 * far past any sane depth and bounds the stack array. */
#define PERF_MAX_DEPTH     64u

/* Distinct zone names the summary can aggregate.  Names are string literals at call-sites. */
#define PERF_MAX_NAMES     256u

/* The 55 ms DOS BIOS tick -- the frame budget every report is measured against. */
#define PERF_BUDGET_US     (PLATFORM_MILLISECONDS_PER_FRAME * 1000u)


struct perf_record
{
    const char * name;        /* static storage duration; never copied */
    uint64_t     t_start_us;  /* Platform_Get_Micros() at zone open */
    uint64_t     dur_us;      /* inclusive: this zone and everything nested in it */
    uint64_t     child_us;    /* time spent in DIRECT children; dur_us - child_us = self time */
    uint32_t     depth;       /* 0 = outermost */
};

struct perf_stack_entry
{
    const char * name;
    uint64_t     t_start_us;
    uint64_t     child_us;    /* accumulated as each direct child closes */
};


static int      m_active = 0;
static int      m_shutdown_done = 0;

static FILE *                  m_file = NULL;
static struct perf_record *    m_records = NULL;
static uint32_t                m_record_count = 0;
static uint64_t                m_records_dropped = 0;   /* only if a drain could not be performed */
static uint32_t                m_drain_count = 0;       /* mid-run drains; each perturbs one region */

static struct perf_stack_entry m_stack[PERF_MAX_DEPTH];
static uint32_t                m_depth = 0;
static uint64_t                m_stack_overflows = 0;   /* total Begins refused beyond PERF_MAX_DEPTH, for the footer */
static uint32_t                m_overflow_pending = 0;  /* refused Begins whose matching End must be swallowed */
static uint64_t                m_unbalanced_ends = 0;   /* End with an empty stack */

static char     m_backend[32] = "?";
static char     m_build[16]   = "?";



/* ========================================================================= */
/*  Small helpers                                                            */
/* ========================================================================= */

static void perf_copy_str(char * dst, size_t dst_size, const char * src)
{
    size_t i;
    if(src == NULL) { src = "?"; }
    for(i = 0; (i + 1) < dst_size && src[i] != '\0'; ++i)
    {
        dst[i] = src[i];
    }
    dst[i] = '\0';
}

static int perf_env_enabled(const char * name, const char ** value_out)
{
    const char * v = getenv(name);
    if(v == NULL || v[0] == '\0') { return 0; }
    if(v[0] == '0' && v[1] == '\0') { return 0; }
    if(value_out != NULL) { *value_out = v; }
    return 1;
}

static int perf_cmp_u64(const void * a, const void * b)
{
    uint64_t x = *(const uint64_t *)a;
    uint64_t y = *(const uint64_t *)b;
    if(x < y) { return -1; }
    if(x > y) { return  1; }
    return 0;
}

/* Nearest-rank percentile over an ascending-sorted array: the smallest value at or above which
 * at least p of the samples fall.  No interpolation -- every reported number is a real observed
 * duration, which is what you want when chasing a specific hitch. */
static uint64_t perf_percentile(const uint64_t * sorted, uint32_t n, double p)
{
    uint32_t rank;

    if(n == 0) { return 0; }

    rank = (uint32_t)(p * (double)n);
    if((double)rank < (p * (double)n)) { rank++; }   /* ceil */
    if(rank < 1) { rank = 1; }
    if(rank > n) { rank = n; }

    return sorted[rank - 1];
}



/* ========================================================================= */
/*  Row output                                                               */
/* ========================================================================= */

/**
 * Write the buffered records out as .fwv rows and reset the buffer.  Called from Perf_Shutdown()
 * and, if the buffer fills, from Perf_Zone_End() -- in the latter case it is I/O inside whatever
 * zone is still open, so it is announced with a #note.
 */
static void perf_drain_records(void)
{
    uint32_t i;

    if(m_file == NULL) { return; }

    for(i = 0; i < m_record_count; ++i)
    {
        fprintf(m_file, "%10llu %12llu %10llu %10llu %5u  %-40s\n",
                (unsigned long long)(i + 1),
                (unsigned long long)m_records[i].t_start_us,
                (unsigned long long)m_records[i].dur_us,
                (unsigned long long)(m_records[i].dur_us - m_records[i].child_us),
                (unsigned)m_records[i].depth,
                (m_records[i].name != NULL) ? m_records[i].name : "?");
    }

    m_record_count = 0;
}



/* ========================================================================= */
/*  Lifecycle                                                                */
/* ========================================================================= */

int Perf_Init(const char * backend, const char * build)
{
    const char * gate = NULL;
    const char * out_path = "remom_perf.fwv";
    int          sev;

    if(m_active)
    {
        return 0;  /* already recording */
    }

    /* REMOM_PERF is the unified gate; REMOM_INPUT_METRICS is honoured as an alias so the existing
     * capture instructions in the GUIDE keep working (PRD FR3). */
    if(!perf_env_enabled("REMOM_PERF", &gate) && !perf_env_enabled("REMOM_INPUT_METRICS", &gate))
    {
        return -1;  /* gate unset: stay inactive, no allocation, no file */
    }

    /* A gate value that is not a bare "1" is taken as the output path, matching REMOM_INPUT_METRICS. */
    if(gate != NULL && !(gate[0] == '1' && gate[1] == '\0'))
    {
        out_path = gate;
    }

    m_records = (struct perf_record *)calloc(PERF_MAX_RECORDS, sizeof(struct perf_record));
    if(m_records == NULL)
    {
        return -1;
    }

    m_file = stu_fopen(out_path, "w");
    if(m_file == NULL)
    {
        free(m_records);
        m_records = NULL;
        return -1;  /* stay inactive if the path is not writable */
    }
    setvbuf(m_file, NULL, _IOFBF, 1 << 16);

    perf_copy_str(m_backend, sizeof(m_backend), backend);
    perf_copy_str(m_build,   sizeof(m_build),   build);

    sev = STU_Log_Get_Severity_Threshold();

    /* The header has to answer "is this capture trustworthy?" without anyone remembering how it was
     * taken.  log_severity / call_trace are the DZ-3 trap: a run with TRACE or CALL_TRACE enabled
     * is measuring the logger, not the game.  build says Debug or Release, because Debug numbers do
     * not transfer.  clock names the counter, since the ms clock is ~15.6 ms granular on Windows. */
    fprintf(m_file, "#meta backend=%s build=%s perf_instrument=%s clock=Platform_Get_Micros budget_ms=%d\n",
            m_backend, m_build,
#ifdef PERF_INSTRUMENT
            "on",
#else
            "off",
#endif
            PLATFORM_MILLISECONDS_PER_FRAME);
    fprintf(m_file, "#meta log_severity=%s call_trace=%s  (a capture taken with TRACE or CALL_TRACE on measures the logger, not the game)\n",
            STU_Log_Severity_Name(sev),
            ((sev <= LOG_SEV_TRACE) && STU_Log_Category_Enabled((int)LOG_CAT_CALL_TRACE)) ? "ON" : "off");
    fprintf(m_file, "#units t_start_us,dur_us,self_us=microseconds  depth=nesting_level  dur=inclusive  self=dur_minus_direct_children\n");
    /* Fixed-width columns (FWV): read by column position, not by delimiter. */
    fprintf(m_file, "%10s %12s %10s %10s %5s  %-40s\n",
            "idx", "t_start_us", "dur_us", "self_us", "depth", "zone");
    fflush(m_file);

    m_record_count    = 0;
    m_records_dropped = 0;
    m_drain_count     = 0;
    m_depth           = 0;
    m_stack_overflows = 0;
    m_unbalanced_ends = 0;

    m_shutdown_done = 0;
    m_active = 1;

    atexit(Perf_Shutdown);

#ifndef PERF_INSTRUMENT
    /* Recording is live but no zone will ever fire -- say so plainly rather than emitting an empty
     * file that reads like "the game spent no time anywhere." */
    LOG_WARN(LOG_CAT_PFL, "[PERF] capture enabled but this build has PERF_INSTRUMENT off -- no zones will be recorded (configure with -DPERF_INSTRUMENT=ON)");
#endif

    return 0;
}

int Perf_Active(void)
{
    return m_active;
}



/* ========================================================================= */
/*  Live readouts (FR9/FR10/FR11) -- always compiled, independent of zones   */
/* ========================================================================= */

/* Ring of recent inter-present intervals, for the on-screen percentile readout.  256 frames is
 * ~14 s at the 18.2 fps design rate -- long enough to be stable, short enough to still be "live". */
#define PERF_LIVE_RING       256u
#define PERF_LIVE_UPDATE_MS  1000u

static int      m_live_checked = 0;      /* env gate resolved once */
static int      m_live_enabled = 0;
static char     m_live_base_title[128] = "ReMoM";

static uint64_t m_live_prev_ms = 0;
static uint64_t m_live_window_start_ms = 0;
static uint32_t m_live_window_frames = 0;
static uint32_t m_live_window_worst_ms = 0;

static uint32_t m_live_ring[PERF_LIVE_RING];
static uint32_t m_live_ring_n = 0;       /* saturates at PERF_LIVE_RING */
static uint32_t m_live_ring_pos = 0;

/* Pre-formatted display lines for the on-screen readout (FR10).  Built once per second on the
 * rollover below, NOT per frame -- see the contract in Platform_Perf.h. */
static char m_live_line1[32] = "";
static char m_live_line2[32] = "";
static char m_live_line3[32] = "";
static int  m_live_lines_ready = 0;

int Perf_Live_Get_Display_Lines(const char ** line1, const char ** line2, const char ** line3)
{
    if(!m_live_lines_ready) { return 0; }
    if(line1 != NULL) { *line1 = m_live_line1; }
    if(line2 != NULL) { *line2 = m_live_line2; }
    if(line3 != NULL) { *line3 = m_live_line3; }
    return 1;
}

void Perf_Live_Set_Base_Title(const char * base_title)
{
    perf_copy_str(m_live_base_title, sizeof(m_live_base_title), base_title);
}

int Perf_Live_Get_Stats(double * fps, unsigned * p50_ms, unsigned * p95_ms, unsigned * p99_ms,
                        unsigned * max_ms, unsigned * over_budget)
{
    uint64_t sorted[PERF_LIVE_RING];
    uint32_t i;
    uint32_t n = m_live_ring_n;
    uint32_t ovr = 0;
    uint64_t sum = 0;

    if(n == 0) { return 0; }

    for(i = 0; i < n; i++)
    {
        sorted[i] = (uint64_t)m_live_ring[i];
        sum += sorted[i];
        if(m_live_ring[i] > PLATFORM_MILLISECONDS_PER_FRAME) { ovr++; }
    }
    qsort(sorted, (size_t)n, sizeof(uint64_t), perf_cmp_u64);

    if(fps != NULL)         { *fps = (sum > 0) ? (1000.0 * (double)n / (double)sum) : 0.0; }
    if(p50_ms != NULL)      { *p50_ms = (unsigned)perf_percentile(sorted, n, 0.50); }
    if(p95_ms != NULL)      { *p95_ms = (unsigned)perf_percentile(sorted, n, 0.95); }
    if(p99_ms != NULL)      { *p99_ms = (unsigned)perf_percentile(sorted, n, 0.99); }
    if(max_ms != NULL)      { *max_ms = (unsigned)sorted[n - 1]; }
    if(over_budget != NULL) { *over_budget = ovr; }

    return 1;
}

void Perf_Live_Note_Present(void)
{
    uint64_t now_ms;
    uint32_t dt_ms;

    if(!m_live_checked)
    {
        const char * v = getenv("REMOM_FPS_TITLE");
        m_live_checked = 1;
        /* On by default -- it is a title string, and immediate feedback is the whole point.
         * REMOM_FPS_TITLE=0 turns it off for anyone who wants the plain title back. */
        m_live_enabled = !(v != NULL && v[0] == '0' && v[1] == '\0');
    }
    if(!m_live_enabled) { return; }

    now_ms = Platform_Get_Millies();

    if(m_live_prev_ms == 0)
    {
        m_live_prev_ms = now_ms;
        m_live_window_start_ms = now_ms;
        return;
    }

    dt_ms = (uint32_t)(now_ms - m_live_prev_ms);
    m_live_prev_ms = now_ms;

    m_live_ring[m_live_ring_pos] = dt_ms;
    m_live_ring_pos = (m_live_ring_pos + 1u) % PERF_LIVE_RING;
    if(m_live_ring_n < PERF_LIVE_RING) { m_live_ring_n++; }

    m_live_window_frames++;
    if(dt_ms > m_live_window_worst_ms) { m_live_window_worst_ms = dt_ms; }

    if((now_ms - m_live_window_start_ms) >= PERF_LIVE_UPDATE_MS)
    {
        char     title[192];
        uint64_t elapsed = now_ms - m_live_window_start_ms;
        double   fps = (elapsed > 0) ? (1000.0 * (double)m_live_window_frames / (double)elapsed) : 0.0;
        unsigned p50 = 0, p95 = 0, p99 = 0, mx = 0, ovr = 0;

        /* Rolling rate AND worst frame: the average alone hides the hitches this exists to expose. */
        snprintf(title, sizeof(title), "%s - %.1f fps (worst %u ms)",
                 m_live_base_title, fps, (unsigned)m_live_window_worst_ms);
        Platform_Set_Window_Title(title);

        /* FR11: the same window as a percentile line, so scrollback carries a timeline that can be
         * diffed across runs.  Once a second, outside any timed zone. */
        if(Perf_Live_Get_Stats(NULL, &p50, &p95, &p99, &mx, &ovr))
        {
            LOG_INFO(LOG_CAT_PFL, "[PERF-LIVE] %.1f fps  frame_ms p50=%u p95=%u p99=%u max=%u  over_budget=%u/%u (budget %d ms)",
                     fps, p50, p95, p99, mx, ovr, (unsigned)m_live_ring_n, PLATFORM_MILLISECONDS_PER_FRAME);

            /* FR10: build the on-screen lines here, on the same once-a-second tick, so the draw
             * path never formats.  Kept short -- the Main Screen debug column is ~100 px wide at
             * ~5 px/char, so ~20 characters. */
            snprintf(m_live_line1, sizeof(m_live_line1), "FPS %.1f", fps);
            snprintf(m_live_line2, sizeof(m_live_line2), "MS %u/%u/%u", p50, p95, p99);
            snprintf(m_live_line3, sizeof(m_live_line3), "MAX %u  OVR %u", mx, ovr);
            m_live_lines_ready = 1;
        }

        m_live_window_start_ms = now_ms;
        m_live_window_frames = 0;
        m_live_window_worst_ms = 0;
    }
}


/* ========================================================================= */
/*  Zone hooks -- hot path, timestamp arithmetic only                        */
/* ========================================================================= */

void Perf_Zone_Begin(const char * name)
{
    struct perf_stack_entry * e;

    if(!m_active) { return; }

    if(m_depth >= PERF_MAX_DEPTH)
    {
        /* Refuse the push, and remember that this zone's End must be swallowed rather than popping
         * somebody else's frame.  A refused zone is always innermost, so LIFO order holds. */
        m_stack_overflows++;
        m_overflow_pending++;
        return;
    }

    e = &m_stack[m_depth];
    e->name       = name;
    e->child_us   = 0;
    e->t_start_us = Platform_Get_Micros();   /* read the clock LAST so setup is outside the zone */

    m_depth++;
}

void Perf_Zone_End(void)
{
    uint64_t                  now_us;
    struct perf_stack_entry * e;
    struct perf_record *      r;
    uint64_t                  dur_us;

    if(!m_active) { return; }

    now_us = Platform_Get_Micros();          /* read the clock FIRST so teardown is outside the zone */

    /* A Begin refused at the depth cap never pushed; swallow its End before touching the stack. */
    if(m_overflow_pending > 0)
    {
        m_overflow_pending--;
        return;
    }

    if(m_depth == 0)
    {
        m_unbalanced_ends++;
        return;
    }

    m_depth--;
    e = &m_stack[m_depth];
    dur_us = now_us - e->t_start_us;

    /* Charge this zone to its parent's child total, so the parent's self time excludes it. */
    if(m_depth > 0)
    {
        m_stack[m_depth - 1].child_us += dur_us;
    }

    if(m_record_count >= PERF_MAX_RECORDS)
    {
        /* Buffer full.  Draining here is I/O inside the still-open parent zones -- it inflates
         * them.  Note it so the perturbed region is visible instead of quietly trusted. */
        if(m_file != NULL)
        {
            fprintf(m_file, "#note buffer_full drain=%u at_t_us=%llu (rows around this point include the cost of writing them)\n",
                    m_drain_count + 1, (unsigned long long)now_us);
            perf_drain_records();
            m_drain_count++;
        }
        else
        {
            m_records_dropped++;
            return;
        }
    }

    r = &m_records[m_record_count];
    r->name       = e->name;
    r->t_start_us = e->t_start_us;
    r->dur_us     = dur_us;
    r->child_us   = e->child_us;
    r->depth      = m_depth;

    m_record_count++;
}



/* ========================================================================= */
/*  Percentile reporting                                                     */
/* ========================================================================= */

struct perf_summary
{
    const char * name;
    uint32_t     count;
    uint64_t     total_us;
    uint64_t     self_total_us;
    uint64_t     p50_us;
    uint64_t     p95_us;
    uint64_t     p99_us;
    uint64_t     max_us;
    uint32_t     over_budget;
};

/**
 * Aggregate every recorded instance of every distinct zone name into a percentile summary.
 *
 * Runs at shutdown only, so it is free to allocate and sort.  Aggregation is over the records
 * still in memory: if the run drained mid-way (buffer full) those earlier instances are already on
 * disk as rows and are NOT in the summary -- the footer reports the drain count so a partial
 * summary is never mistaken for a whole one.
 */
static void perf_write_summary(void)
{
    const char * names[PERF_MAX_NAMES];
    uint32_t     name_count = 0;
    uint64_t *   durs;
    uint32_t     i;
    uint32_t     n;

    if(m_file == NULL || m_record_count == 0) { return; }

    /* Collect distinct names, preserving first-seen order (which is close-order, i.e. the order the
     * stages actually finished -- more readable than alphabetical for a pipeline). */
    for(i = 0; i < m_record_count; ++i)
    {
        const char * nm = (m_records[i].name != NULL) ? m_records[i].name : "?";
        uint32_t     j;
        int          found = 0;

        for(j = 0; j < name_count; ++j)
        {
            if(names[j] == nm || strcmp(names[j], nm) == 0) { found = 1; break; }
        }
        if(!found && name_count < PERF_MAX_NAMES)
        {
            names[name_count++] = nm;
        }
    }

    durs = (uint64_t *)malloc((size_t)m_record_count * sizeof(uint64_t));
    if(durs == NULL) { return; }

    fprintf(m_file, "#\n");
    fprintf(m_file, "#summary  per-zone inclusive duration distribution.  p50/p95/p99/max are nearest-rank over\n");
    fprintf(m_file, "#summary  every recorded instance -- NEVER a mean, because the fat tail is the thing players feel.\n");
    fprintf(m_file, "#summary  over_budget = instances whose INCLUSIVE time exceeded the %d ms tick budget.\n",
            PLATFORM_MILLISECONDS_PER_FRAME);
    /* The '#head '/'#zone ' prefixes are the same width so the columns line up for FWV reading. */
    fprintf(m_file, "#head %6s %11s %12s %13s %10s %10s %10s %10s  %-40s\n",
            "count", "over_budget", "total_ms", "self_total_ms", "p50_ms", "p95_ms", "p99_ms", "max_ms", "zone");

    for(i = 0; i < name_count; ++i)
    {
        struct perf_summary s;
        uint32_t            k;

        memset(&s, 0, sizeof(s));
        s.name = names[i];
        n = 0;

        for(k = 0; k < m_record_count; ++k)
        {
            const char * nm = (m_records[k].name != NULL) ? m_records[k].name : "?";
            if(nm == s.name || strcmp(nm, s.name) == 0)
            {
                durs[n++] = m_records[k].dur_us;
                s.total_us      += m_records[k].dur_us;
                s.self_total_us += (m_records[k].dur_us - m_records[k].child_us);
                if(m_records[k].dur_us > PERF_BUDGET_US) { s.over_budget++; }
            }
        }

        qsort(durs, n, sizeof(uint64_t), perf_cmp_u64);

        s.count  = n;
        s.p50_us = perf_percentile(durs, n, 0.50);
        s.p95_us = perf_percentile(durs, n, 0.95);
        s.p99_us = perf_percentile(durs, n, 0.99);
        s.max_us = (n > 0) ? durs[n - 1] : 0;

        fprintf(m_file, "#zone %6u %11u %12.3f %13.3f %10.3f %10.3f %10.3f %10.3f  %-40s\n",
                s.count,
                s.over_budget,
                (double)s.total_us / 1000.0,
                (double)s.self_total_us / 1000.0,
                (double)s.p50_us / 1000.0,
                (double)s.p95_us / 1000.0,
                (double)s.p99_us / 1000.0,
                (double)s.max_us / 1000.0,
                s.name);

        /* Same numbers into the log, so a capture is readable without opening the .fwv.  This is
         * shutdown-time reporting -- deliberately NOT inside any timed region. */
        LOG_INFO(LOG_CAT_PFL, "[PERF] %-44s n=%-5u over_budget=%-4u total=%.1fms self=%.1fms p50=%.3f p95=%.3f p99=%.3f max=%.3f ms",
                 s.name, s.count, s.over_budget,
                 (double)s.total_us / 1000.0,
                 (double)s.self_total_us / 1000.0,
                 (double)s.p50_us / 1000.0,
                 (double)s.p95_us / 1000.0,
                 (double)s.p99_us / 1000.0,
                 (double)s.max_us / 1000.0);
    }

    if(name_count >= PERF_MAX_NAMES)
    {
        fprintf(m_file, "#note distinct_zone_names hit the PERF_MAX_NAMES cap (%u) -- summary is INCOMPLETE\n", PERF_MAX_NAMES);
    }

    free(durs);
}

void Perf_Shutdown(void)
{
    if(!m_active || m_shutdown_done)
    {
        return;
    }
    m_shutdown_done = 1;

    /* Close anything still open (a return/goto out of a PERF_ZONE block, or a missing PERF_END) so
     * its time is recorded rather than silently vanishing.  Do this while still active.  Clearing
     * the refused-Begin count first is what keeps this loop finite: a pending swallow makes
     * Perf_Zone_End() return without popping, which would spin here forever. */
    m_overflow_pending = 0;
    while(m_depth > 0)
    {
        m_unbalanced_ends++;
        Perf_Zone_End();
    }

    m_active = 0;  /* stop any further recording during reporting */

    if(m_file != NULL)
    {
        perf_write_summary();
        perf_drain_records();

        fprintf(m_file, "#end drains=%u dropped=%llu stack_overflows=%llu unbalanced=%llu\n",
                m_drain_count,
                (unsigned long long)m_records_dropped,
                (unsigned long long)m_stack_overflows,
                (unsigned long long)m_unbalanced_ends);
        fflush(m_file);
        stu_fclose(m_file);
        m_file = NULL;
    }

    if(m_records != NULL)
    {
        free(m_records);
        m_records = NULL;
    }
}
