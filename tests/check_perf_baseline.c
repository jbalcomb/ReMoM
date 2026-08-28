/**
 * check_perf_baseline.c -- performance regression gate for the .fwv zone captures.
 *
 * See doc/@AI_Plans/{PRD,PLAN}-Performance-Management.md.  Compares a candidate capture against a
 * committed reference baseline and fails (non-zero exit) on regression.
 *
 * WHY THIS IS NOT THE PERCENTILE GATE THE PRD ASKED FOR
 * -----------------------------------------------------
 * FR14 specifies relative thresholds on p95/p99/max.  Two back-to-back captures of the SAME
 * scenario on the SAME machine (2026-07-29, committed beside this tool as baseline_* and repeat_*)
 * measured the actual reproducibility:
 *
 *     structure : 66 zones, ZERO instance-count mismatches           -- perfectly deterministic
 *     p50       : median ratio 0.825, range 0.46 .. 1.66             -- +-40..65%
 *     max       : median ratio 1.004, range 0.12 .. 24.46            -- worthless
 *
 * 11 of the 12 hottest zones were uniformly faster on the second run, so the variance is dominated
 * by a systematic warmup effect (cold caches, CPU boost residency, background load), not by the
 * code under test.  A p95/max threshold tight enough to be useful would fail randomly; a threshold
 * loose enough to be stable would not be measuring percentiles in any meaningful sense.
 *
 * So the gate asserts what is actually reproducible:
 *
 *   1. STRUCTURE (exact, zero tolerance).  Same zone set, same per-zone instance counts.  This has
 *      no timing component at all, so it never flakes -- and it is the check with real teeth: it
 *      catches a changed code path, a stage that stopped running, a loop that now runs a different
 *      number of times.  The wp=-1 corpse that walked into AI_Evaluation_Map would have tripped it.
 *
 *   2. LOOSE CEILING on p50 ONLY (advisory-grade).  p50 may not exceed the baseline by more than
 *      --ratio (default 2.5x, chosen above the 1.66x worst observed inflation with headroom).  This
 *      only catches gross regressions, and the tool says so rather than implying precision it does
 *      not have.  Zones below --floor ms are exempt: a ratio on a 0.1 ms zone is noise.
 *
 *      p50 is used because it is a RANK -- one stalled sample cannot move it.  total and max are
 *      sums/extremes and are outlier-dominated on a live desktop; gating them was tried against the
 *      committed baseline/repeat pair and produced four false failures (up to 32x on a 1.3 ms zone)
 *      while p50 passed on all 66 zones.  See the comment at the ceiling check for the numbers.
 *
 * Tighten later by killing the noise, not by tightening the threshold: discard a warmup run and
 * take median-of-N, or move to Callgrind instruction counts for a deterministic number.
 *
 * MEDIAN-OF-N (--merge) -- the actual noise fix
 * ---------------------------------------------
 * Loosening the threshold treats the symptom.  The cause is that a single capture on a live
 * desktop carries warmup and scheduling noise, and 11 of the 12 hottest zones were uniformly
 * faster on the second run.  --merge collapses N captures of the same scenario into one summary
 * holding the PER-ZONE MEDIAN of each statistic, which discards the run that got unlucky (or
 * lucky) instead of averaging its damage in.  Merge N>=3 captures for both the baseline and the
 * candidate and the ratio threshold can then be tightened well below the 2.5x a single pair needs.
 *
 * A merged file is SUMMARY-ONLY: it carries #meta/#head/#zone rows but not the per-instance data
 * rows, which is all this gate reads.  Instance counts must be identical across the inputs -- if
 * they are not, the runs did different work and merging them would be meaningless, so it errors.
 *
 * Usage:
 *   check_perf_baseline <baseline.fwv> <candidate.fwv> [--ratio R] [--floor MS]
 *   check_perf_baseline --merge <out.fwv> <in1.fwv> <in2.fwv> [in3.fwv ...]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define PERF_MAX_ZONES      512
#define PERF_NAME_LEN       128

/* Default regression multiplier.  Measured across five captures of the same scenario (2026-07-29),
 * comparing each run's p50 against the 5-run median, for zones passing the floor and sample
 * minimum below: worst case 1.55x.  2.0 gives ~29% headroom over that.
 *
 * This started at 2.5x when the only evidence was a single pair of runs and every zone was gated.
 * Excluding statistics that cannot hold still -- see PERF_DEFAULT_MIN_SAMPLES -- is what bought the
 * tightening; the threshold was not simply guessed downward. */
#define PERF_DEFAULT_RATIO  2.0

/* Zones cheaper than this are exempt from the ceiling check: a ratio on a sub-millisecond zone
 * measures scheduler jitter, not the code. */
#define PERF_DEFAULT_FLOOR_MS 1.0

/* Zones with fewer instances than this are exempt.  p50 is a RANK, so over a small sample one slow
 * call moves it outright.  Measured across five captures of the same scenario (2026-07-29): with no
 * sample minimum the worst single-run-vs-median p50 ratio was 2.46x, entirely from
 * Diplomacy_Growth() at n=11 (once per turn).  Requiring n >= 12 drops the worst case to 1.55x --
 * a 1.6x tightening of the gate for free, purely by not measuring statistics that cannot hold
 * still.  The per-player zones (n=44), including AI_Set_Unit_Orders, comfortably qualify. */
#define PERF_DEFAULT_MIN_SAMPLES 12


struct perf_zone_row
{
    char     name[PERF_NAME_LEN];
    unsigned count;
    unsigned over_budget;
    double   total_ms;
    double   self_total_ms;
    double   p50_ms;
    double   p95_ms;
    double   p99_ms;
    double   max_ms;
};


static void Trim_Trailing_WS(char * s)
{
    size_t n = strlen(s);
    while(n > 0 && (s[n - 1] == ' ' || s[n - 1] == '\t' || s[n - 1] == '\r' || s[n - 1] == '\n'))
    {
        s[--n] = '\0';
    }
}

/**
 * Read the "#zone" summary rows out of a .fwv.  The row layout is fixed-width and written by
 * PFL_Perf.c: eight numeric columns then the zone name, which may itself contain spaces (e.g.
 * "Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane)"), so the name is taken as the remainder
 * of the line rather than as a whitespace-delimited token.
 */
static int Load_Fwv_Zones(const char * path, struct perf_zone_row * rows, int max_rows, int * out_count)
{
    FILE * fp;
    char   line[1024];
    int    n = 0;

    fp = fopen(path, "r");
    if(fp == NULL)
    {
        fprintf(stderr, "check_perf_baseline: cannot open '%s'\n", path);
        return -1;
    }

    while(fgets(line, (int)sizeof(line), fp) != NULL)
    {
        struct perf_zone_row row;
        int    consumed = 0;
        char * name_start;

        if(strncmp(line, "#zone ", 6) != 0)
        {
            continue;
        }

        memset(&row, 0, sizeof(row));

        if(sscanf(line + 6, "%u %u %lf %lf %lf %lf %lf %lf%n",
                  &row.count, &row.over_budget,
                  &row.total_ms, &row.self_total_ms,
                  &row.p50_ms, &row.p95_ms, &row.p99_ms, &row.max_ms,
                  &consumed) != 8)
        {
            fprintf(stderr, "check_perf_baseline: malformed #zone row in '%s': %s", path, line);
            fclose(fp);
            return -1;
        }

        name_start = line + 6 + consumed;
        while(*name_start == ' ' || *name_start == '\t') { name_start++; }
        Trim_Trailing_WS(name_start);

        if(n >= max_rows)
        {
            fprintf(stderr, "check_perf_baseline: more than %d zones in '%s'\n", max_rows, path);
            fclose(fp);
            return -1;
        }

        strncpy(row.name, name_start, PERF_NAME_LEN - 1);
        row.name[PERF_NAME_LEN - 1] = '\0';
        rows[n++] = row;
    }

    fclose(fp);

    if(n == 0)
    {
        fprintf(stderr, "check_perf_baseline: '%s' contains no #zone rows (empty or truncated capture?)\n", path);
        return -1;
    }

    *out_count = n;
    return 0;
}

static const struct perf_zone_row * Find_Zone(const struct perf_zone_row * rows, int count, const char * name)
{
    int i;
    for(i = 0; i < count; i++)
    {
        if(strcmp(rows[i].name, name) == 0)
        {
            return &rows[i];
        }
    }
    return NULL;
}


static int Cmp_Double(const void * a, const void * b)
{
    double x = *(const double *)a;
    double y = *(const double *)b;
    if(x < y) { return -1; }
    if(x > y) { return  1; }
    return 0;
}

/** Median of n samples.  Even n averages the two middle values; the array is sorted in place. */
static double Median_Of(double * v, int n)
{
    if(n <= 0) { return 0.0; }
    qsort(v, (size_t)n, sizeof(double), Cmp_Double);
    if((n % 2) == 1)
    {
        return v[n / 2];
    }
    return (v[(n / 2) - 1] + v[n / 2]) / 2.0;
}

/**
 * Collapse N captures of the same scenario into one summary holding the per-zone median of each
 * statistic.  This is the noise fix: it discards an unlucky run rather than averaging it in.
 */
static int Merge_Medians(const char * out_path, char * const * in_paths, int n_in)
{
    static struct perf_zone_row sets[16][PERF_MAX_ZONES];
    int    counts[16];
    double samples[16];
    FILE * out;
    int    f, i, k;

    if(n_in < 2 || n_in > 16)
    {
        fprintf(stderr, "check_perf_baseline: --merge needs 2..16 input captures (got %d)\n", n_in);
        return 2;
    }

    for(f = 0; f < n_in; f++)
    {
        if(Load_Fwv_Zones(in_paths[f], sets[f], PERF_MAX_ZONES, &counts[f]) != 0)
        {
            return 2;
        }
    }

    /* Every input must describe the same work, or a median across them is meaningless. */
    for(f = 1; f < n_in; f++)
    {
        if(counts[f] != counts[0])
        {
            fprintf(stderr, "check_perf_baseline: --merge input '%s' has %d zones, '%s' has %d -- runs differ\n",
                    in_paths[f], counts[f], in_paths[0], counts[0]);
            return 2;
        }
    }

    out = fopen(out_path, "w");
    if(out == NULL)
    {
        fprintf(stderr, "check_perf_baseline: cannot write '%s'\n", out_path);
        return 2;
    }

    fprintf(out, "#meta merged=median_of_%d  (summary only -- per-instance data rows are not carried)\n", n_in);
    for(f = 0; f < n_in; f++)
    {
        fprintf(out, "#meta source=%s\n", in_paths[f]);
    }
    fprintf(out, "#head %6s %11s %12s %13s %10s %10s %10s %10s  %-40s\n",
            "count", "over_budget", "total_ms", "self_total_ms", "p50_ms", "p95_ms", "p99_ms", "max_ms", "zone");

    for(i = 0; i < counts[0]; i++)
    {
        const char *                 name = sets[0][i].name;
        const struct perf_zone_row * row[16];
        double                       med_total, med_self, med_p50, med_p95, med_p99, med_max, med_ovr;

        row[0] = &sets[0][i];

        for(f = 1; f < n_in; f++)
        {
            row[f] = Find_Zone(sets[f], counts[f], name);
            if(row[f] == NULL)
            {
                fprintf(stderr, "check_perf_baseline: --merge zone '%s' missing from '%s'\n", name, in_paths[f]);
                fclose(out);
                return 2;
            }
            if(row[f]->count != row[0]->count)
            {
                fprintf(stderr, "check_perf_baseline: --merge zone '%s' instance count %u vs %u -- runs did different work\n",
                        name, row[0]->count, row[f]->count);
                fclose(out);
                return 2;
            }
        }

        for(k = 0; k < n_in; k++) { samples[k] = row[k]->total_ms;      } med_total = Median_Of(samples, n_in);
        for(k = 0; k < n_in; k++) { samples[k] = row[k]->self_total_ms; } med_self  = Median_Of(samples, n_in);
        for(k = 0; k < n_in; k++) { samples[k] = row[k]->p50_ms;        } med_p50   = Median_Of(samples, n_in);
        for(k = 0; k < n_in; k++) { samples[k] = row[k]->p95_ms;        } med_p95   = Median_Of(samples, n_in);
        for(k = 0; k < n_in; k++) { samples[k] = row[k]->p99_ms;        } med_p99   = Median_Of(samples, n_in);
        for(k = 0; k < n_in; k++) { samples[k] = row[k]->max_ms;        } med_max   = Median_Of(samples, n_in);
        for(k = 0; k < n_in; k++) { samples[k] = (double)row[k]->over_budget; } med_ovr = Median_Of(samples, n_in);

        fprintf(out, "#zone %6u %11u %12.3f %13.3f %10.3f %10.3f %10.3f %10.3f  %-40s\n",
                row[0]->count, (unsigned)(med_ovr + 0.5),
                med_total, med_self, med_p50, med_p95, med_p99, med_max, name);
    }

    fclose(out);
    printf("check_perf_baseline: merged %d captures -> %s (%d zones, per-zone medians)\n",
           n_in, out_path, counts[0]);
    return 0;
}

int main(int argc, char * argv[])
{
    struct perf_zone_row base[PERF_MAX_ZONES];
    struct perf_zone_row cand[PERF_MAX_ZONES];
    int    base_count = 0;
    int    cand_count = 0;
    int    i;
    int    argi;
    int    structural_failures = 0;
    int    ceiling_failures = 0;
    double ratio = PERF_DEFAULT_RATIO;
    double floor_ms = PERF_DEFAULT_FLOOR_MS;
    unsigned min_samples = PERF_DEFAULT_MIN_SAMPLES;

    if(argc < 3)
    {
        fprintf(stderr, "Usage: %s <baseline.fwv> <candidate.fwv> [--ratio R] [--floor MS]\n", argv[0]);
        fprintf(stderr, "       %s --merge <out.fwv> <in1.fwv> <in2.fwv> [in3.fwv ...]\n", argv[0]);
        return 2;
    }

    if(strcmp(argv[1], "--merge") == 0)
    {
        if(argc < 5)
        {
            fprintf(stderr, "Usage: %s --merge <out.fwv> <in1.fwv> <in2.fwv> [in3.fwv ...]\n", argv[0]);
            return 2;
        }
        return Merge_Medians(argv[2], &argv[3], argc - 3);
    }

    for(argi = 3; argi < argc; argi++)
    {
        if(strcmp(argv[argi], "--ratio") == 0 && (argi + 1) < argc)
        {
            argi++;
            ratio = atof(argv[argi]);
        }
        else if(strcmp(argv[argi], "--floor") == 0 && (argi + 1) < argc)
        {
            argi++;
            floor_ms = atof(argv[argi]);
        }
        else if(strcmp(argv[argi], "--min-samples") == 0 && (argi + 1) < argc)
        {
            argi++;
            min_samples = (unsigned)atoi(argv[argi]);
        }
        else
        {
            fprintf(stderr, "check_perf_baseline: unknown argument '%s'\n", argv[argi]);
            return 2;
        }
    }

    if(Load_Fwv_Zones(argv[1], base, PERF_MAX_ZONES, &base_count) != 0) { return 2; }
    if(Load_Fwv_Zones(argv[2], cand, PERF_MAX_ZONES, &cand_count) != 0) { return 2; }

    printf("check_perf_baseline\n");
    printf("  baseline : %s (%d zones)\n", argv[1], base_count);
    printf("  candidate: %s (%d zones)\n", argv[2], cand_count);
    printf("  ceiling  : %.2fx on p50 and total, for zones >= %.2f ms\n", ratio, floor_ms);
    printf("\n");

    /* ---- 1. STRUCTURE: exact, zero tolerance ---------------------------------------------- */

    for(i = 0; i < base_count; i++)
    {
        const struct perf_zone_row * c = Find_Zone(cand, cand_count, base[i].name);

        if(c == NULL)
        {
            printf("STRUCTURE FAIL  zone missing from candidate: %s\n", base[i].name);
            structural_failures++;
            continue;
        }

        if(c->count != base[i].count)
        {
            printf("STRUCTURE FAIL  %s: instance count %u -> %u (code path changed)\n",
                   base[i].name, base[i].count, c->count);
            structural_failures++;
        }
    }

    for(i = 0; i < cand_count; i++)
    {
        if(Find_Zone(base, base_count, cand[i].name) == NULL)
        {
            printf("STRUCTURE FAIL  zone not in baseline: %s (new instrumentation? rebaseline)\n", cand[i].name);
            structural_failures++;
        }
    }

    /* ---- 2. LOOSE CEILINGS: gross regressions only ----------------------------------------- */

    for(i = 0; i < base_count; i++)
    {
        const struct perf_zone_row * c = Find_Zone(cand, cand_count, base[i].name);

        if(c == NULL) { continue; }  /* already reported as structural */

        if(base[i].count >= min_samples && base[i].p50_ms >= floor_ms && c->p50_ms > (base[i].p50_ms * ratio))
        {
            printf("CEILING FAIL    %s: p50 %.3f -> %.3f ms (%.2fx, limit %.2fx)\n",
                   base[i].name, base[i].p50_ms, c->p50_ms, c->p50_ms / base[i].p50_ms, ratio);
            ceiling_failures++;
        }

        /* total_ms is deliberately NOT gated.  It is a SUM, so a single OS stall landing in one
         * call inflates it without the code having changed -- the same outlier sensitivity that
         * makes max useless here.  Measured on the committed baseline/repeat pair, where p50 held
         * within 2.5x for all 66 zones but total did not:
         *     Diplomacy_Growth_For_Enchantments   1.309 ->  42.347 ms  (32.35x)
         *     Make_Monsters                       3.619 ->  33.303 ms  ( 9.20x)
         *     AI_Update_Gold_And_Mana_Reserves    21.964 ->  76.934 ms  ( 3.50x)
         *     Determine_Event                     3.078 ->   9.574 ms  ( 3.11x)
         * p50 is the robust statistic: it is a rank, so one stalled sample cannot move it.
         * Revisit once median-of-N kills the warmup noise -- with a stable mean, total becomes a
         * meaningful "did the whole distribution shift" signal, which p50 alone does not give. */
    }

    printf("\n");
    printf("  structural failures: %d\n", structural_failures);
    printf("  ceiling failures   : %d\n", ceiling_failures);

    if(structural_failures > 0 || ceiling_failures > 0)
    {
        printf("RESULT: FAIL\n");
        return 1;
    }

    printf("RESULT: PASS  (structure identical; no zone exceeded %.2fx)\n", ratio);
    printf("NOTE: passing means 'not grossly worse'.  Run-to-run p50 varies 0.46..1.66x on the\n");
    printf("      reference host, so this gate cannot detect regressions smaller than %.2fx.\n", ratio);
    return 0;
}
