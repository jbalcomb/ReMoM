/*
    STU_LOG - High-throughput ring-buffered structured logger.

    Public interface. Six severity levels (TRACE/DEBUG/INFO/WARN/ERROR/FATAL),
    runtime category filtering via the [Logging] section of ReMoM.ini, fixed
    compile-time severity floor via STU_LOG_MIN_SEVERITY (stripped severities
    expand to a sizeof-noop with no argument evaluation).

    Companion to STU_DBG (dbg_prn / trc_prn), which remains available as the
    library's synchronous-fflush-per-call ad-hoc debug print path.
*/

#ifndef STU_LOG_H
#define STU_LOG_H

#include <stdarg.h>
#include <stddef.h>   /* size_t for STU_Log_Path_For */
#include <stdint.h>   /* uint64_t for g_random_call_count */

#ifdef __cplusplus
extern "C" {
#endif

/* Severity levels are preprocessor constants so they can be compared in the #if gates below. */
#define LOG_SEV_TRACE 0
#define LOG_SEV_DEBUG 1
#define LOG_SEV_INFO  2
#define LOG_SEV_WARN  3
#define LOG_SEV_ERROR 4
#define LOG_SEV_FATAL 5

    enum log_category
    {
        LOG_CAT_GENERAL = 0,
        LOG_CAT_AIMOVE = 1,
        LOG_CAT_COMBAT = 2,
        LOG_CAT_SAVE = 3,
        LOG_CAT_PFL = 4,
        LOG_CAT_IKI = 5,
        LOG_CAT_LOADSAVE = 6,
        LOG_CAT_AIDUDES = 7,
        LOG_CAT_WINDOWLESS_PFL = 8,
        LOG_CAT_REPLAY = 9,
        LOG_CAT_SDL2_AUDIO = 10,
        LOG_CAT_SDL2_INIT = 11,
        LOG_CAT_SDL2_PFL = 12,
        LOG_CAT_SDL3_AUDIO = 13,
        LOG_CAT_SDL3_INIT = 14,
        LOG_CAT_SDL3_PFL = 15,
        LOG_CAT_WIN_PFL = 16,
        LOG_CAT_DEMOM = 17,
        LOG_CAT_REMOM = 18,
        LOG_CAT_AUTOMATION_DRIVER = 19,
        LOG_CAT_GAME_SAVE_DUMP = 20,
        LOG_CAT_STU_WRLD = 21,
        LOG_CAT_STU_VLD = 22,
        LOG_CAT_STU_SND = 23,
        LOG_CAT_STU_INIT = 24,
        LOG_CAT_STU_XTRC = 25,
        LOG_CAT_STU_FON = 26,
        LOG_CAT_STU_PRN = 27,
        LOG_CAT_STU_FIO = 28,
        LOG_CAT_STU_DBG = 29,
        LOG_CAT_STU_LOG = 30,
        LOG_CAT_NEXTTURN = 31,
        LOG_CAT_MOM_SCR = 32,
        LOG_CAT_LOADSCR = 33,
        LOG_CAT_LOADER = 34,
        LOG_CAT_NEWGAME = 35,
        LOG_CAT_SPELLBOOK = 36,
        LOG_CAT_SPELLS128 = 37,
        LOG_CAT_SBOOKSCR = 38,
        LOG_CAT_MAINMENU = 39,
        LOG_CAT_MAINSCR = 40,
        LOG_CAT_INPUT = 41,
        LOG_CAT_RANDOM = 42,
        LOG_CAT_EXIT = 43,
        LOG_CAT_FONTS = 44,
        LOG_CAT_MOX2 = 45,
        LOG_CAT_CALL_TRACE = 46,
        LOG_CAT_STU_GRAF = 47,
        LOG_CAT_OGBUG = 48,
        LOG_CAT_COMBAT_TEST = 49,  /* test-only: combat move/attack legality logging */
        LOG_CAT_INTEGRITY = 50   /* runtime integrity checkpoints: invalid game state, static-pool guard damage, rejected draws -- keep last; LOG_N_CATEGORIES derives from it */
    };

    /* CALL_TRACE rng-call ordinal -- read by the [FN-ENTER]/[FN-EXIT] log lines.
       Defined in STU_LOG.c so libSTU's CALL_TRACE wrappers (stu_compat.c) link
       without dragging libMOX in.  Random_at() in MoX/src/random.c increments it. */
    extern uint64_t g_random_call_count;

    /* Preferred log-output directory, applied by the *next* STU_Log_Startup().  Call
       before STU_Log_Startup(); pass NULL/"" to reset to the CWD.  The runtime
       precedence is REMOM_LOG_DIR (env) -> this base dir -> CWD; an unwritable dir
       falls back to the CWD.  ReMoM passes the resolved XDG state dir here;
       DeMoM / tests / matchup never call it, so they keep logging to the CWD. */
    void STU_Log_Set_Base_Dir(const char * dir);

    /* "<effective log dir>/<filename>" (the bare filename when the log dir is the CWD), for companion log files such as STU_CLOG's.  The log dir is
       resolved by STU_Log_Startup(), so call this after it. */
    void STU_Log_Path_For(char * out, size_t cap, const char * filename);

    /* ini_path: path to an INI file with a [Logging] section, or NULL for built-in defaults. Missing file or missing section is non-fatal — defaults apply. */
    void STU_Log_Startup(const char * ini_path);
    void STU_Log_Shutdown(void);

    /* Read back the effective runtime filter.  Added for the performance layer, which records the log
       level a capture was taken under: a perf number measured with TRACE / CALL_TRACE enabled is
       dominated by the logger and is fiction, so the .fwv header has to say what was on.
       STU_Log_Severity_Name() returns a static, never-NULL string ("TRACE".."FATAL", or "?"). */
    int STU_Log_Get_Severity_Threshold(void);
    const char * STU_Log_Severity_Name(int sev);
    int STU_Log_Category_Enabled(int cat);
    void STU_Log_Pump(void);
    void STU_Log_Flush_All(void);

    void STU_Log_Write_At(int sev, enum log_category cat, const char * file, int line, const char * func, const char * fmt, ...);
    void STU_Log_Write_At_V(int sev, enum log_category cat, const char * file, int line, const char * func, const char * fmt, va_list args);

#ifndef STU_LOG_MIN_SEVERITY
#define STU_LOG_MIN_SEVERITY LOG_SEV_TRACE
#endif

/* sizeof-noop: argument expressions are syntactically validated but never evaluated; produces no code in optimized builds and prevents side effects from stripped-severity call-sites. */
#define LOG__NOOP(cat, ...) ((void)sizeof((cat), __VA_ARGS__, 0))

#if STU_LOG_MIN_SEVERITY <= LOG_SEV_TRACE
#define LOG_TRACE(cat, ...) STU_Log_Write_At(LOG_SEV_TRACE, (cat), __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define LOG_TRACE(cat, ...) LOG__NOOP((cat), __VA_ARGS__)
#endif

#if STU_LOG_MIN_SEVERITY <= LOG_SEV_DEBUG
#define LOG_DEBUG(cat, ...) STU_Log_Write_At(LOG_SEV_DEBUG, (cat), __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define LOG_DEBUG(cat, ...) LOG__NOOP((cat), __VA_ARGS__)
#endif

#if STU_LOG_MIN_SEVERITY <= LOG_SEV_INFO
#define LOG_INFO(cat, ...) STU_Log_Write_At(LOG_SEV_INFO, (cat), __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define LOG_INFO(cat, ...) LOG__NOOP((cat), __VA_ARGS__)
#endif

#if STU_LOG_MIN_SEVERITY <= LOG_SEV_WARN
#define LOG_WARN(cat, ...) STU_Log_Write_At(LOG_SEV_WARN, (cat), __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define LOG_WARN(cat, ...) LOG__NOOP((cat), __VA_ARGS__)
#endif

#if STU_LOG_MIN_SEVERITY <= LOG_SEV_ERROR
#define LOG_ERROR(cat, ...) STU_Log_Write_At(LOG_SEV_ERROR, (cat), __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define LOG_ERROR(cat, ...) LOG__NOOP((cat), __VA_ARGS__)
#endif

#define LOG_FATAL(cat, ...) do { STU_Log_Write_At(LOG_SEV_FATAL, (cat), __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); STU_Log_Flush_All(); } while(0)

/*
    CALL_TRACE function-boundary events -- the [FN-ENTER] / [FN-EXIT] stream that merge-remom-trace.py / compare-call-paths.py (STU-Extras STU-DOSBox)
    align against the OG-side trace by name and rng_call.  The text is a contract with those tools (ReMoM-CallTrace-PRD.md F3):
        [FN-ENTER] name=<fn> rng_call=<N>
        [FN-EXIT]  name=<fn> rng_call=<N>          <- two spaces after "[FN-EXIT]", so the name= columns line up
    rng_call is g_random_call_count, the number of Random() calls completed when the event is logged.
    These are written at TRACE severity in the CALL_TRACE category, but they have their own compile-time switch, STU_CALL_TRACE, so they do NOT depend on
    STU_LOG_MIN_SEVERITY: a Release build can strip TRACE/DEBUG and still carry the call trace for an OG-matchup run.
        compile time:  STU_CALL_TRACE  1 = built in, 0 = sizeof-noop.  Defaults to 1 with STU_DEBUG, else 0.  CMake: -DREMOM_CALL_TRACE=ON turns it on for every config.
        run time:      the [Logging] INI -- the CALL_TRACE category enabled and severity_threshold = TRACE.
    TRACE_ENTER() / TRACE_EXIT()                      name = the enclosing function (__func__)
    TRACE_ENTER_NAMED(name) / TRACE_EXIT_NAMED(name)  name = a given string, for a wrapper that reports the function it wraps (stu_fopen -> "fopen")
    TRACE_EXIT_NOTE("note")                           __func__ name, with a string-literal note appended after rng_call (an early-out exit)
    The names are the PRD's (F2).  The PRD sketches TRACE_ENTER(name_str, fmt, ...); these take no format -- the PRD's name_str form is the _NAMED pair.
    See doc/@Devel/Logging-And-Instrumentation.md.
*/
#ifndef STU_CALL_TRACE
#ifdef STU_DEBUG
#define STU_CALL_TRACE 1
#else
#define STU_CALL_TRACE 0
#endif
#endif

#if STU_CALL_TRACE
#define STU_CALL_TRACE_WRITE(...)  STU_Log_Write_At(LOG_SEV_TRACE, LOG_CAT_CALL_TRACE, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define STU_CALL_TRACE_WRITE(...)  LOG__NOOP(LOG_CAT_CALL_TRACE, __VA_ARGS__)
#endif

#define TRACE_ENTER()             STU_CALL_TRACE_WRITE("[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count)
#define TRACE_EXIT()              STU_CALL_TRACE_WRITE("[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count)
#define TRACE_ENTER_NAMED(name_)  STU_CALL_TRACE_WRITE("[FN-ENTER] name=%s rng_call=%llu", (name_), (unsigned long long)g_random_call_count)
#define TRACE_EXIT_NAMED(name_)   STU_CALL_TRACE_WRITE("[FN-EXIT]  name=%s rng_call=%llu", (name_), (unsigned long long)g_random_call_count)
#define TRACE_EXIT_NOTE(note_)    STU_CALL_TRACE_WRITE("[FN-EXIT]  name=%s rng_call=%llu " note_, __func__, (unsigned long long)g_random_call_count)

#ifdef __cplusplus
}
#endif

#endif /* STU_LOG_H */
