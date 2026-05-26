#ifndef STU_LOG_H
#define STU_LOG_H

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

enum log_severity
{
    LOG_SEV_TRACE = 0,
    LOG_SEV_DEBUG = 1,
    LOG_SEV_INFO  = 2,
    LOG_SEV_WARN  = 3,
    LOG_SEV_ERROR = 4,
    LOG_SEV_FATAL = 5
};

enum log_category
{
    LOG_CAT_GENERAL = 0
};

void log_init(void);
void log_shutdown(void);
void log_pump(void);

void log_write(enum log_severity sev, enum log_category cat, const char * fmt, ...);
void log_write_v(enum log_severity sev, enum log_category cat, const char * fmt, va_list args);

#define LOG_INFO(cat, ...) log_write(LOG_SEV_INFO, (cat), __VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* STU_LOG_H */
