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
    LOG_CAT_GENERAL  = 0,
    LOG_CAT_AIMOVE   = 1,
    LOG_CAT_COMBAT   = 2,
    LOG_CAT_SAVE     = 3,
    LOG_CAT_PFL      = 4,
    LOG_CAT_IKI      = 5,
    LOG_CAT_LOADSAVE     = 6,
    LOG_CAT_AIDUDES      = 7,
    LOG_CAT_HEADLESS_PFL = 8,
    LOG_CAT_REPLAY       = 9,
    LOG_CAT_SDL2_AUDIO   = 10,
    LOG_CAT_SDL2_INIT    = 11,
    LOG_CAT_SDL2_PFL     = 12,
    LOG_CAT_SDL3_AUDIO   = 13,
    LOG_CAT_SDL3_INIT    = 14,
    LOG_CAT_SDL3_PFL     = 15,
    LOG_CAT_WIN_PFL      = 16,
    LOG_CAT_HEMOM        = 17,
    LOG_CAT_REMOM        = 18,
    LOG_CAT_ARTIFICIAL_HUMAN_PLAYER = 19,
    LOG_CAT_GAME_SAVE_DUMP = 20,
    LOG_CAT_STU_WRLD     = 21,
    LOG_CAT_STU_VLD      = 22,
    LOG_CAT_STU_SND      = 23,
    LOG_CAT_STU_INIT     = 24,
    LOG_CAT_STU_XTRC     = 25,
    LOG_CAT_STU_FON      = 26,
    LOG_CAT_STU_PRN      = 27,
    LOG_CAT_STU_FIO      = 28,
    LOG_CAT_STU_DBG      = 29,
    LOG_CAT_STU_LOG      = 30,
    LOG_CAT_NEXTTURN     = 31,
    LOG_CAT_MOM_SCR      = 32,
    LOG_CAT_LOADSCR      = 33,
    LOG_CAT_LOADER       = 34,
    LOG_CAT_NEWGAME      = 35,
    LOG_CAT_SPELLBOOK    = 36,
    LOG_CAT_SPELLS128    = 37,
    LOG_CAT_SBOOKSCR     = 38,
    LOG_CAT_MAINMENU     = 39,
    LOG_CAT_MAINSCR      = 40,
    LOG_CAT_INPUT        = 41,
    LOG_CAT_RANDOM       = 42,
    LOG_CAT_EXIT         = 43,
    LOG_CAT_FONTS        = 44,
    LOG_CAT_MOX2         = 45,
    LOG_CAT_CALL_TRACE   = 46
};

/* CALL_TRACE rng-call ordinal -- read by the [FN-ENTER]/[FN-EXIT] log lines.
   Defined in STU_LOG.c so libSTU's CALL_TRACE wrappers (stu_compat.c) link
   without dragging libMOX in.  Random_at() in MoX/src/random.c increments it. */
extern uint64_t g_random_call_count;

/* ini_path: path to an INI file with a [Logging] section, or NULL for built-in defaults. Missing file or missing section is non-fatal — defaults apply. */
void STU_Log_Startup(const char * ini_path);
void STU_Log_Shutdown(void);
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

#ifdef __cplusplus
}
#endif

#endif /* STU_LOG_H */
