/**
 * PFL_Input_Metrics.c — Layer 1 of the Platform-Input responsiveness toolkit (see the header and
 * doc/#AI_Plans/{BRA,PRD,PLAN}-Platform-Input.md).
 *
 * Backend-agnostic: nothing here touches SDL or engine globals.  The windowed backend feeds it three
 * ways -- Note_Tick() per event-handler tick, Note_Poll() per cursor sample, Record_Present() around
 * the present call -- and it emits one fixed-width row per present.
 *
 * Crash-safe persistence.  The tool exists to diagnose misbehaving sessions, which crash, hang, and
 * get force-quit -- so it CANNOT rely on a single flush at exit (a SIGSEGV or kill never reaches
 * atexit).  Instead each row is written to an open, fully-buffered stream and the stream is fflush()ed
 * every IM_FLUSH_INTERVAL_MS.  On any death the file already holds everything up to the last flush;
 * at most the final ~0.5 s is lost.  The per-row fprintf is a userspace memcpy into the stdio buffer
 * (no syscall), so it does not perturb the present timing it measures; the once-per-interval fflush
 * is the only syscall and shows, at worst, as one slightly inflated dt_ms per interval.
 *
 * No custom signal handler is installed, so the existing STU_BRAK crash reporter keeps its handlers.
 */

#include "Platform_Input_Metrics.h"

#include "../../ext/stu_compat.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/* Flush cadence: bounds crash data loss to this window without per-row syscalls. */
#define IM_FLUSH_INTERVAL_MS   500u

/* Disk cap: a runaway multi-hour session stops writing here rather than filling the disk.  At ~80
 * bytes/row this is ~160 MB / many hours of presents; a lag reproduction is minutes. */
#define IM_MAX_ROWS            2000000u

#define IM_REASON_LEN          12



static int      m_active = 0;
static int      m_shutdown_done = 0;
static int      m_capped = 0;

static FILE *   m_file = NULL;
static uint64_t m_idx = 0;            /* rows written so far / monotonic present counter */
static uint64_t m_last_flush_ms = 0;

/* Accumulators between presents. */
static uint64_t m_prev_present_ms = 0;
static uint32_t m_ticks_since = 0;
static uint32_t m_max_gap_since = 0;
static uint64_t m_prev_tick_ms = 0;
static int      m_cur_x = 0;
static int      m_cur_y = 0;
static int      m_prev_present_x = 0;
static int      m_prev_present_y = 0;
static char     m_pending_reason[IM_REASON_LEN] = "frame";



static void im_copy_reason(char * dst, const char * src)
{
    size_t i;
    if(src == NULL) { src = "frame"; }
    for(i = 0; i + 1 < IM_REASON_LEN && src[i] != '\0'; ++i)
    {
        dst[i] = src[i];
    }
    dst[i] = '\0';
}

static int im_clamp_i16(int v)
{
    if(v > 32767)  { return 32767; }
    if(v < -32768) { return -32768; }
    return v;
}



int Input_Metrics_Init(const char * out_path, const char * backend, const char * video_driver, int scale, int refresh_hz)
{
    if(m_active)
    {
        return 0;  /* already recording */
    }

    m_file = stu_fopen((out_path != NULL) ? out_path : "remom_input_metrics.fwv", "w");
    if(m_file == NULL)
    {
        return -1;  /* stay inactive if the path is not writable */
    }
    /* Fully buffered: per-row fprintf stays in userspace; only the periodic fflush hits disk. */
    setvbuf(m_file, NULL, _IOFBF, 1 << 16);

    im_copy_reason(m_pending_reason, "frame");
    if(scale < 1) { scale = 1; }
    if(refresh_hz < 0) { refresh_hz = 0; }

    fprintf(m_file, "#meta backend=%s video_driver=%s scale=%d refresh=%d flush_ms=%u\n",
            (backend != NULL) ? backend : "?",
            (video_driver != NULL && video_driver[0] != '\0') ? video_driver : "?",
            scale, refresh_hz, IM_FLUSH_INTERVAL_MS);
    fprintf(m_file, "#units t_ms,dt_ms,block_ms,max_gap_ms=milliseconds  dx,dy,ptr=game_px  reason=present_cause\n");
    /* Fixed-width columns (FWV): read by column position, not by delimiter. */
    fprintf(m_file, "%10s %10s %7s %8s %6s %8s %5s %5s %6s %6s  %-11s\n",
            "idx", "t_ms", "dt_ms", "block_ms", "ticks", "max_gap", "dx", "dy", "ptr_x", "ptr_y", "reason");
    fflush(m_file);

    m_idx = 0;
    m_last_flush_ms = 0;
    m_capped = 0;
    m_prev_present_ms = 0;
    m_ticks_since = 0;
    m_max_gap_since = 0;
    m_prev_tick_ms = 0;
    m_cur_x = 0;
    m_cur_y = 0;
    m_prev_present_x = 0;
    m_prev_present_y = 0;

    m_shutdown_done = 0;
    m_active = 1;

    atexit(Input_Metrics_Shutdown);  /* covers clean exits; the periodic fflush covers crashes/kills */

    return 0;
}

int Input_Metrics_Active(void)
{
    return m_active;
}

void Input_Metrics_Note_Tick(uint64_t now_ms)
{
    uint64_t gap;

    if(!m_active) { return; }

    if(m_prev_tick_ms != 0)
    {
        gap = now_ms - m_prev_tick_ms;
        if(gap > m_max_gap_since)
        {
            m_max_gap_since = (uint32_t)gap;
        }
    }
    m_prev_tick_ms = now_ms;
    m_ticks_since++;
}

void Input_Metrics_Note_Poll(int game_x, int game_y)
{
    if(!m_active) { return; }
    m_cur_x = game_x;
    m_cur_y = game_y;
}

void Input_Metrics_Set_Reason(const char * reason)
{
    if(!m_active) { return; }
    im_copy_reason(m_pending_reason, reason);
}

void Input_Metrics_Record_Present(uint64_t t_before_ms, uint64_t t_after_ms)
{
    uint32_t dt_ms;
    uint32_t block_ms;

    if(!m_active) { return; }

    if(!m_capped && m_idx >= IM_MAX_ROWS)
    {
        fprintf(m_file, "#end capped=1 rows=%llu (IM_MAX_ROWS reached; recording stopped)\n",
                (unsigned long long)m_idx);
        fflush(m_file);
        m_capped = 1;
    }

    if(!m_capped)
    {
        dt_ms    = (m_prev_present_ms != 0) ? (uint32_t)(t_before_ms - m_prev_present_ms) : 0;
        block_ms = (t_after_ms >= t_before_ms) ? (uint32_t)(t_after_ms - t_before_ms) : 0;

        fprintf(m_file, "%10llu %10llu %7u %8u %6u %8u %5d %5d %6d %6d  %-11s\n",
                (unsigned long long)(m_idx + 1),
                (unsigned long long)t_before_ms,
                dt_ms,
                block_ms,
                m_ticks_since,
                m_max_gap_since,
                im_clamp_i16(m_cur_x - m_prev_present_x),
                im_clamp_i16(m_cur_y - m_prev_present_y),
                im_clamp_i16(m_cur_x),
                im_clamp_i16(m_cur_y),
                m_pending_reason);

        m_idx++;

        /* Periodic flush: bound crash loss to IM_FLUSH_INTERVAL_MS without a syscall per row. */
        if(t_before_ms - m_last_flush_ms >= IM_FLUSH_INTERVAL_MS)
        {
            fflush(m_file);
            m_last_flush_ms = t_before_ms;
        }
    }

    /* Reset the between-present accumulators.  m_prev_tick_ms is cleared too so max_gap_ms measures
       only gaps between consecutive handler ticks WITHIN the next window -- otherwise the first tick
       after each present would always register a gap that includes this present's block time, putting
       a ~16 ms vsync floor on max_gap and masking the abnormal freezes it exists to surface.  A freeze
       that spans a present boundary still shows plainly as a large dt_ms with a low tick count. */
    m_prev_present_ms = t_before_ms;
    m_ticks_since = 0;
    m_max_gap_since = 0;
    m_prev_tick_ms = 0;
    m_prev_present_x = m_cur_x;
    m_prev_present_y = m_cur_y;
    im_copy_reason(m_pending_reason, "frame");
}

void Input_Metrics_Shutdown(void)
{
    if(!m_active || m_shutdown_done)
    {
        return;
    }
    m_shutdown_done = 1;
    m_active = 0;  /* stop any further recording during flush */

    if(m_file != NULL)
    {
        if(!m_capped)
        {
            fprintf(m_file, "#end rows=%llu\n", (unsigned long long)m_idx);
        }
        fflush(m_file);
        stu_fclose(m_file);
        m_file = NULL;
    }
}
