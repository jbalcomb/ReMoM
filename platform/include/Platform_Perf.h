/**
 * Platform_Perf.h -- the performance-measurement layer's zone primitive and frame-time sink.
 *
 * See doc/#AI_Plans/{BRA,PRD,PLAN}-Performance-Management.md.  This is the graduated form of the
 * hand-toggled PHASE macro that used to live in MoM/src/NEXTTURN.c (PRD FR6/FR7, PLAN step 4).
 *
 * WHAT IT REPLACES AND WHY
 * ------------------------
 * The old PHASE macro had three defects this module exists to fix:
 *   1. It was switched by hand-commenting one of two #define lines in a single .c file -- manual,
 *      per-file, and unrelated to the build type, so a wrong-state commit could ship timing code
 *      into Release invisibly.  Here the switch is ONE build option, PERF_INSTRUMENT, and when it
 *      is off the compiler strips every zone: the macros expand to the bare call / to nothing.
 *   2. It called LOG_INFO *inside* the timed region, so it inflated the very stage times it
 *      reported.  Here a zone body is timestamp-arithmetic into a preallocated buffer -- no
 *      formatting, no allocation, no I/O.  All reporting is deferred to Perf_Shutdown().
 *   3. It reported a single number per stage to a log line.  Here every zone instance is retained,
 *      so the reporter emits p50/p95/p99/max and a count-over-budget -- a distribution, never a
 *      mean (the fat tail is the whole point).
 *
 * TWO SWITCHES, DELIBERATELY
 * --------------------------
 *   PERF_INSTRUMENT (build) -- compiled in or not.  Default OFF, never on in a release preset.
 *                              Off => zero code, zero symbols, nothing to strip later.
 *   REMOM_PERF      (run)   -- env var, checked once at Perf_Init().  On an instrumented build
 *                              with the env unset, each zone hook is one predictable branch on a
 *                              global and no I/O.  REMOM_INPUT_METRICS is honoured as an alias so
 *                              existing capture instructions keep working (PRD FR3).
 *
 * TIME BASE
 * ---------
 * Zones timestamp with Platform_Get_Micros(), NOT Platform_Get_Millies().  The headless and Win32
 * backends implement the millisecond clock with GetTickCount64(), whose ~15.6 ms granularity
 * cannot resolve a sub-frame stage.  Rows are emitted in microseconds; the reporter also prints
 * milliseconds against the 55 ms budget, which is the unit every perf conversation uses.
 *
 * OUTPUT
 * ------
 * One fixed-width values file (.fwv -- project convention, NOT csv): a self-describing header
 * (backend, build, instrumentation state, budget, and the ACTIVE LOG SEVERITY -- a capture taken
 * with TRACE/CALL_TRACE on is fiction, so the file records what it was taken under), one row per
 * zone instance, then a per-zone percentile summary block.
 */

#ifndef PLATFORM_PERF_H
#define PLATFORM_PERF_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/* ========================================================================= */
/*  Lifecycle                                                                */
/* ========================================================================= */

/**
 * Check the REMOM_PERF / REMOM_INPUT_METRICS gate and, if set, open the sink and begin recording.
 * Safe and near-free to call unconditionally -- when the gate is unset it returns immediately and
 * leaves the layer inactive.  Call once, early, after STU_Log_Startup() so the header can record
 * the active log severity.
 *
 * @param backend  "SDL2" / "SDL3" / "win32" / "headless" -- recorded in the header.
 * @param build    "Debug" / "Release" -- recorded in the header.  Numbers from a Debug build do
 *                 not transfer to a Release build and the header must say which one this was.
 * @return 0 if recording is now active, non-zero if it is not (gate unset, or the path failed).
 */
int Perf_Init(const char * backend, const char * build);

/**
 * Non-zero while recording.  The zone hooks test this first; it is also the honest answer to
 * "did this run actually capture anything."
 */
int Perf_Active(void);

/**
 * Close any still-open zones, write the rows and the percentile summary, and close the file.
 * Idempotent; also registered via atexit() by Perf_Init().  Safe to call when inactive.
 */
void Perf_Shutdown(void);


/* ========================================================================= */
/*  Zone hooks -- call these through the PERF_* macros, not directly         */
/* ========================================================================= */

/**
 * Open a zone.  @p name must have static storage duration (a string literal or equivalent): the
 * pointer is retained until Perf_Shutdown() and the string is never copied.
 *
 * Timestamp-only: this takes one clock read and one push.  It must never format or allocate.
 */
void Perf_Zone_Begin(const char * name);

/**
 * Close the innermost open zone and record its duration.  Unbalanced with respect to
 * Perf_Zone_Begin() is a bug in the caller; it is counted and reported in the file footer rather
 * than crashing a capture run.
 */
void Perf_Zone_End(void);


/* ========================================================================= */
/*  Macros -- the only thing call-sites should use                           */
/* ========================================================================= */

#ifdef PERF_INSTRUMENT

/**
 * PERF_CALL(CALL) -- time one call.  The direct replacement for the old PHASE(CALL), kept so an
 * existing per-call stage breakdown migrates 1:1 without restructuring the function.
 *
 * Prefer PERF_ZONE for NEW instrumentation: one wrapper per line is the clutter the PRD calls out
 * (a function's call sequence becomes a column of PERF_CALL).  This exists for the migration and
 * for the genuinely one-call-per-stage case.
 */
#define PERF_CALL(CALL)   do { Perf_Zone_Begin(#CALL); CALL; Perf_Zone_End(); } while(0)

/**
 * PERF_ZONE("name") { ... } -- time a block.  Scope-based: brackets a region once instead of
 * wrapping every call in it.  Implemented as a one-iteration for-loop so the close cannot be
 * forgotten and no matching PERF_END is needed.
 *
 * CAVEAT: because it is a loop, `break` and `continue` inside the block bind to the PERF_ZONE, and
 * `return`/`goto` out of the block skips the close (the zone stays open until its parent closes,
 * which the footer reports as an imbalance).  Use PERF_BEGIN/PERF_END explicitly for regions with
 * a mid-block exit.
 */
#define PERF_ZONE(name)   PERF_ZONE_LINE_(name, __LINE__)
#define PERF_ZONE_LINE_(name, ln)   PERF_ZONE_JOIN_(name, ln)
#define PERF_ZONE_JOIN_(name, ln)   for(int perf_open_##ln = (Perf_Zone_Begin(name), 1); perf_open_##ln; perf_open_##ln = (Perf_Zone_End(), 0))

/** Explicit open/close, for regions PERF_ZONE's loop scoping cannot express. Must be balanced. */
#define PERF_BEGIN(name)  Perf_Zone_Begin(name)
#define PERF_END()        Perf_Zone_End()

#else  /* !PERF_INSTRUMENT -- every zone compiles away to nothing */

#define PERF_CALL(CALL)   CALL
#define PERF_ZONE(name)
#define PERF_BEGIN(name)  ((void)0)
#define PERF_END()        ((void)0)

#endif /* PERF_INSTRUMENT */


#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_PERF_H */
