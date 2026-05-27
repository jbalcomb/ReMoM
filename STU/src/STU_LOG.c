#include "STU_LOG.h"

#include "../../ext/stu_compat.h"
#include "STU_UTIL.h"

#include <ctype.h>
#include <signal.h>
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)
#include <windows.h>
#endif

#if defined(_MSC_VER)
#define LOG_VSNPRINTF _vsnprintf
#else
#define LOG_VSNPRINTF vsnprintf
#endif

#define LOG_RING_SIZE        ((size_t)(2 * 1024 * 1024))  /* 2 MB */
#define LOG_FMT_BUF_LEN      ((size_t)1024)
#define LOG_PUMP_MAX_BYTES   ((size_t)4096)
#define LOG_FILE_NEW         "remom_log_new.txt"
#define LOG_FILE_CURRENT     "remom_log_current.txt"
#define LOG_FILE_PREVIOUS    "remom_log_previous.txt"
#define LOG_N_CATEGORIES     8

static char   log_ring[LOG_RING_SIZE];
static size_t log_head = 0;
static size_t log_tail = 0;
static FILE * log_file = NULL;
static size_t log_dropped_since_last_pump = 0;

struct log_config
{
    int sev_threshold;
    int cat_enabled[LOG_N_CATEGORIES];
};

static struct log_config log_cfg;

static const char * const log_sev_str[] = {
    "TRACE",
    "DEBUG",
    "INFO ",
    "WARN ",
    "ERROR",
    "FATAL"
};

static const char * const log_cat_str[] = {
    "GENERAL ",
    "AIMOVE  ",
    "COMBAT  ",
    "SAVE    ",
    "PFL     ",
    "IKI     ",
    "LOADSAVE",
    "AIDUDES "
};

static const char * const log_cat_ini_key[] = {
    "GENERAL",
    "AIMOVE",
    "COMBAT",
    "SAVE",
    "PFL",
    "IKI",
    "LOADSAVE",
    "AIDUDES"
};

static void log_config_set_defaults(void)
{
    int i;
    log_cfg.sev_threshold = LOG_SEV_TRACE;
    for (i = 0; i < LOG_N_CATEGORIES; ++i)
    {
        log_cfg.cat_enabled[i] = 1;
    }
}

static int log_ci_eq(const char * a, const char * b)
{
    while (*a != '\0' && *b != '\0')
    {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
        {
            return 0;
        }
        ++a;
        ++b;
    }
    return *a == '\0' && *b == '\0';
}

static char * log_trim_ws(char * s)
{
    char * end;
    while (*s == ' ' || *s == '\t' || *s == '\r' || *s == '\n')
    {
        ++s;
    }
    end = s + strlen(s);
    while (end > s && (end[-1] == ' ' || end[-1] == '\t' || end[-1] == '\r' || end[-1] == '\n'))
    {
        --end;
    }
    *end = '\0';
    return s;
}

static int log_parse_severity(const char * value, int * out)
{
    if      (log_ci_eq(value, "TRACE")) { *out = LOG_SEV_TRACE; return 1; }
    else if (log_ci_eq(value, "DEBUG")) { *out = LOG_SEV_DEBUG; return 1; }
    else if (log_ci_eq(value, "INFO"))  { *out = LOG_SEV_INFO;  return 1; }
    else if (log_ci_eq(value, "WARN"))  { *out = LOG_SEV_WARN;  return 1; }
    else if (log_ci_eq(value, "ERROR")) { *out = LOG_SEV_ERROR; return 1; }
    else if (log_ci_eq(value, "FATAL")) { *out = LOG_SEV_FATAL; return 1; }
    return 0;
}

static int log_parse_bool(const char * value, int * out)
{
    if (log_ci_eq(value, "true") || log_ci_eq(value, "yes") || log_ci_eq(value, "on")  || log_ci_eq(value, "1"))
    {
        *out = 1;
        return 1;
    }
    if (log_ci_eq(value, "false") || log_ci_eq(value, "no") || log_ci_eq(value, "off") || log_ci_eq(value, "0"))
    {
        *out = 0;
        return 1;
    }
    return 0;
}

static void log_config_load_ini(const char * path)
{
    FILE * fp;
    char   line[512];
    char * key;
    char * value;
    char * eq;
    int    in_logging_section = 0;
    int    parsed_int;
    int    i;

    if (path == NULL)
    {
        return;
    }
    fp = fopen(path, "r");
    if (fp == NULL)
    {
        return;
    }

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        key = log_trim_ws(line);
        if (*key == '\0' || *key == '#' || *key == ';')
        {
            continue;
        }
        if (*key == '[')
        {
            in_logging_section = (strncmp(key, "[Logging]", 9) == 0);
            continue;
        }
        if (!in_logging_section)
        {
            continue;
        }
        eq = strchr(key, '=');
        if (eq == NULL)
        {
            continue;
        }
        *eq = '\0';
        value = log_trim_ws(eq + 1);
        key   = log_trim_ws(key);

        if (log_ci_eq(key, "severity_threshold"))
        {
            if (log_parse_severity(value, &parsed_int))
            {
                log_cfg.sev_threshold = parsed_int;
            }
            continue;
        }
        for (i = 0; i < LOG_N_CATEGORIES; ++i)
        {
            if (log_ci_eq(key, log_cat_ini_key[i]))
            {
                if (log_parse_bool(value, &parsed_int))
                {
                    log_cfg.cat_enabled[i] = parsed_int;
                }
                break;
            }
        }
    }
    fclose(fp);
}

static const char * log_basename(const char * path)
{
    const char * last_slash;
    const char * last_bslash;
    const char * last;

    if (path == NULL)
    {
        return "";
    }
    last_slash  = strrchr(path, '/');
    last_bslash = strrchr(path, '\\');
    last = (last_slash > last_bslash) ? last_slash : last_bslash;
    return (last != NULL) ? (last + 1) : path;
}

static size_t log_ring_used(void)
{
    if (log_head >= log_tail)
    {
        return log_head - log_tail;
    }
    return LOG_RING_SIZE - (log_tail - log_head);
}

static size_t log_ring_free(void)
{
    return LOG_RING_SIZE - log_ring_used() - 1;
}

static void log_ring_write_bytes(const char * src, size_t n)
{
    size_t first_chunk;

    if (n == 0)
    {
        return;
    }

    if (log_head + n <= LOG_RING_SIZE)
    {
        memcpy(&log_ring[log_head], src, n);
        log_head = (log_head + n) % LOG_RING_SIZE;
        return;
    }

    first_chunk = LOG_RING_SIZE - log_head;
    memcpy(&log_ring[log_head], src, first_chunk);
    memcpy(&log_ring[0], src + first_chunk, n - first_chunk);
    log_head = n - first_chunk;
}

static size_t log_drain_up_to(size_t cap)
{
    size_t used;
    size_t to_drain;
    size_t first_chunk;

    if (log_file == NULL)
    {
        log_tail = log_head;
        return 0;
    }

    used = log_ring_used();
    if (used == 0)
    {
        return 0;
    }
    to_drain = (used < cap) ? used : cap;

    if (log_tail + to_drain <= LOG_RING_SIZE)
    {
        fwrite(&log_ring[log_tail], 1, to_drain, log_file);
        log_tail = (log_tail + to_drain) % LOG_RING_SIZE;
    }
    else
    {
        first_chunk = LOG_RING_SIZE - log_tail;
        if (first_chunk >= to_drain)
        {
            fwrite(&log_ring[log_tail], 1, to_drain, log_file);
            log_tail = (log_tail + to_drain) % LOG_RING_SIZE;
        }
        else
        {
            fwrite(&log_ring[log_tail], 1, first_chunk, log_file);
            fwrite(&log_ring[0], 1, to_drain - first_chunk, log_file);
            log_tail = to_drain - first_chunk;
        }
    }
    return to_drain;
}

/* The synthetic dropped-message line is written directly to the file (bypassing the ring) so it cannot itself be dropped or split across a pump boundary. */
static void log_emit_drop_marker(void)
{
    char datetime[32];

    if (log_dropped_since_last_pump == 0 || log_file == NULL)
    {
        return;
    }
    get_datetime(datetime);
    fprintf(log_file, "[%s] [LOGGER] %lu messages dropped since last drain\n",
            datetime, (unsigned long)log_dropped_since_last_pump);
    log_dropped_since_last_pump = 0;
}

static void log_rotate_files(void)
{
    /* remove(<missing>) returns -1 and rename(<missing>, ...) does the same; both are safe to ignore. */
    remove(LOG_FILE_PREVIOUS);
    rename(LOG_FILE_CURRENT, LOG_FILE_PREVIOUS);
    rename(LOG_FILE_NEW,     LOG_FILE_CURRENT);
}

static void log_emit_crash_marker(const char * signal_name)
{
    char datetime[32];

    if (log_file == NULL)
    {
        return;
    }
    get_datetime(datetime);
    fprintf(log_file, "[%s] [CRASH] %s\n", datetime, signal_name);
}

static void log_atexit_handler(void)
{
    log_shutdown();
}

#if defined(_WIN32)

static LPTOP_LEVEL_EXCEPTION_FILTER log_previous_seh_filter = NULL;

static const char * log_seh_code_name(DWORD code)
{
    switch (code)
    {
        case EXCEPTION_ACCESS_VIOLATION:        return "EXCEPTION_ACCESS_VIOLATION";
        case EXCEPTION_ILLEGAL_INSTRUCTION:     return "EXCEPTION_ILLEGAL_INSTRUCTION";
        case EXCEPTION_INT_DIVIDE_BY_ZERO:      return "EXCEPTION_INT_DIVIDE_BY_ZERO";
        case EXCEPTION_STACK_OVERFLOW:          return "EXCEPTION_STACK_OVERFLOW";
        case EXCEPTION_PRIV_INSTRUCTION:        return "EXCEPTION_PRIV_INSTRUCTION";
        case EXCEPTION_DATATYPE_MISALIGNMENT:   return "EXCEPTION_DATATYPE_MISALIGNMENT";
        default:                                return "EXCEPTION_UNKNOWN";
    }
}

static LONG WINAPI log_seh_filter(EXCEPTION_POINTERS * ep)
{
    log_emit_crash_marker(log_seh_code_name(ep->ExceptionRecord->ExceptionCode));
    log_flush_all();
    if (log_file != NULL)
    {
        fflush(log_file);
        fclose(log_file);
        log_file = NULL;
    }
    /* EXCEPTION_EXECUTE_HANDLER terminates the process without invoking the JIT debugger / WER dialog, which makes crash tests run cleanly. */
    return EXCEPTION_EXECUTE_HANDLER;
}

static void log_install_crash_handlers(void)
{
    log_previous_seh_filter = SetUnhandledExceptionFilter(log_seh_filter);
}

#else  /* POSIX */

static const char * log_signal_name(int sig)
{
    switch (sig)
    {
        case SIGSEGV:  return "SIGSEGV";
        case SIGABRT:  return "SIGABRT";
        case SIGFPE:   return "SIGFPE";
        case SIGILL:   return "SIGILL";
        default:       return "SIGNAL_UNKNOWN";
    }
}

/* Signal handlers calling stdio is technically UB on POSIX, but consistent with this project's single-threaded posture and acceptable for a crash-time-only path. */
static void log_signal_handler(int sig)
{
    log_emit_crash_marker(log_signal_name(sig));
    log_flush_all();
    if (log_file != NULL)
    {
        fflush(log_file);
        fclose(log_file);
        log_file = NULL;
    }
    signal(sig, SIG_DFL);
    raise(sig);
}

static void log_install_crash_handlers(void)
{
    signal(SIGSEGV, log_signal_handler);
    signal(SIGABRT, log_signal_handler);
    signal(SIGFPE,  log_signal_handler);
    signal(SIGILL,  log_signal_handler);
}

#endif

void log_init(const char * ini_path)
{
    static int atexit_registered = 0;
    static int crash_handlers_installed = 0;

    if (log_file != NULL)
    {
        fflush(log_file);
        fclose(log_file);
        log_file = NULL;
    }

    log_head = 0;
    log_tail = 0;
    log_dropped_since_last_pump = 0;
    log_config_set_defaults();
    log_config_load_ini(ini_path);

    log_rotate_files();

    log_file = fopen(LOG_FILE_NEW, "w");
    if (log_file == NULL)
    {
        fprintf(stderr, "STU_LOG: failed to open '%s' for writing\n", LOG_FILE_NEW);
    }

    if (!atexit_registered)
    {
        atexit(log_atexit_handler);
        atexit_registered = 1;
    }
    if (!crash_handlers_installed)
    {
        log_install_crash_handlers();
        crash_handlers_installed = 1;
    }
}

void log_shutdown(void)
{
    if (log_file != NULL)
    {
        log_emit_drop_marker();
        while (log_drain_up_to(LOG_RING_SIZE) > 0)
        {
            /* keep draining until empty */
        }
        fflush(log_file);
        fclose(log_file);
        log_file = NULL;
    }
}

void log_pump(void)
{
    log_emit_drop_marker();
    log_drain_up_to(LOG_PUMP_MAX_BYTES);
    if (log_file != NULL)
    {
        fflush(log_file);
    }
}

void log_flush_all(void)
{
    if (log_file == NULL)
    {
        return;
    }
    log_emit_drop_marker();
    while (log_drain_up_to(LOG_RING_SIZE) > 0)
    {
        /* keep draining until empty */
    }
    fflush(log_file);
}

void log_write_at_v(int sev, enum log_category cat, const char * file, int line, const char * func, const char * fmt, va_list args)
{
    char   stack_buf[LOG_FMT_BUF_LEN];
    char   datetime[32];
    int    header_len;
    int    body_len;
    size_t total_len;
    int    sev_idx;
    int    cat_idx;

    sev_idx = sev;
    cat_idx = (int)cat;
    if (sev_idx < 0 || sev_idx > LOG_SEV_FATAL)
    {
        sev_idx = LOG_SEV_INFO;
    }
    if (cat_idx < 0 || cat_idx >= LOG_N_CATEGORIES)
    {
        cat_idx = LOG_CAT_GENERAL;
    }

    if (sev_idx < log_cfg.sev_threshold)
    {
        return;
    }
    if (!log_cfg.cat_enabled[cat_idx])
    {
        return;
    }

    get_datetime(datetime);

    header_len = snprintf(stack_buf, LOG_FMT_BUF_LEN, "[%s] [%s] [%s] %s:%d %s: ",
                          datetime,
                          log_sev_str[sev_idx],
                          log_cat_str[cat_idx],
                          log_basename(file),
                          line,
                          (func != NULL) ? func : "");
    if (header_len < 0 || (size_t)header_len >= LOG_FMT_BUF_LEN)
    {
        return;
    }

    body_len = LOG_VSNPRINTF(stack_buf + header_len, LOG_FMT_BUF_LEN - (size_t)header_len, fmt, args);
    if (body_len < 0)
    {
        return;
    }

    total_len = (size_t)header_len + (size_t)body_len;
    if (total_len >= LOG_FMT_BUF_LEN - 1)
    {
        total_len = LOG_FMT_BUF_LEN - 2;
    }
    stack_buf[total_len] = '\n';
    total_len += 1;

    if (total_len > log_ring_free())
    {
        ++log_dropped_since_last_pump;
        return;
    }

    log_ring_write_bytes(stack_buf, total_len);
}

void log_write_at(int sev, enum log_category cat, const char * file, int line, const char * func, const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    log_write_at_v(sev, cat, file, line, func, fmt, args);
    va_end(args);
}
