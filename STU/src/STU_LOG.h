#ifndef STU_LOG_H
#define STU_LOG_H

#include <stdarg.h>

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
    LOG_CAT_WIN_PFL      = 16
};

/* ini_path: path to an INI file with a [Logging] section, or NULL for built-in defaults. Missing file or missing section is non-fatal — defaults apply. */
void log_init(const char * ini_path);
void log_shutdown(void);
void log_pump(void);
void log_flush_all(void);

void log_write_at(int sev, enum log_category cat, const char * file, int line, const char * func, const char * fmt, ...);
void log_write_at_v(int sev, enum log_category cat, const char * file, int line, const char * func, const char * fmt, va_list args);

#ifndef STU_LOG_MIN_SEVERITY
#define STU_LOG_MIN_SEVERITY LOG_SEV_TRACE
#endif

/* sizeof-noop: argument expressions are syntactically validated but never evaluated; produces no code in optimized builds and prevents side effects from stripped-severity call-sites. */
#define LOG__NOOP(cat, ...) ((void)sizeof((cat), __VA_ARGS__, 0))

#if STU_LOG_MIN_SEVERITY <= LOG_SEV_TRACE
#define LOG_TRACE(cat, ...) log_write_at(LOG_SEV_TRACE, (cat), __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define LOG_TRACE(cat, ...) LOG__NOOP((cat), __VA_ARGS__)
#endif

#if STU_LOG_MIN_SEVERITY <= LOG_SEV_DEBUG
#define LOG_DEBUG(cat, ...) log_write_at(LOG_SEV_DEBUG, (cat), __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define LOG_DEBUG(cat, ...) LOG__NOOP((cat), __VA_ARGS__)
#endif

#if STU_LOG_MIN_SEVERITY <= LOG_SEV_INFO
#define LOG_INFO(cat, ...) log_write_at(LOG_SEV_INFO, (cat), __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define LOG_INFO(cat, ...) LOG__NOOP((cat), __VA_ARGS__)
#endif

#if STU_LOG_MIN_SEVERITY <= LOG_SEV_WARN
#define LOG_WARN(cat, ...) log_write_at(LOG_SEV_WARN, (cat), __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define LOG_WARN(cat, ...) LOG__NOOP((cat), __VA_ARGS__)
#endif

#if STU_LOG_MIN_SEVERITY <= LOG_SEV_ERROR
#define LOG_ERROR(cat, ...) log_write_at(LOG_SEV_ERROR, (cat), __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define LOG_ERROR(cat, ...) LOG__NOOP((cat), __VA_ARGS__)
#endif

#define LOG_FATAL(cat, ...) do { log_write_at(LOG_SEV_FATAL, (cat), __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); log_flush_all(); } while(0)

#ifdef __cplusplus
}
#endif

#endif /* STU_LOG_H */
