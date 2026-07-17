/*
    STU_LOG - High-throughput ring-buffered structured logger.

    Single-threaded; no mutexes or atomics. 2 MB static ring buffer drained
    once per frame via the public pump call, capped at 4 KB per pump to keep
    I/O off the hot path. Messages exceeding ring free space are silently
    dropped; the per-pump drop count is reported on disk as a synthetic
    [LOGGER] line, so a gap in the log is distinguishable from a quiet
    period.

    3-file rotation at startup: previous is deleted, current is renamed to
    previous, new is renamed to current, fresh new is opened for writing.

    atexit + signal/SEH crash handlers flush the entire ring synchronously
    before terminating, so messages buffered between pumps survive a crash.
*/

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

/* CLAUDE 2026-06-14: ring sized to absorb worst-case bursty workloads
   (worldgen, init-time CALL_TRACE flood, long combat turns) without
   exceeding the per-frame pump drain rate.  Old values (2 MB ring,
   4 KB/pump) were dropping thousands of messages: init-time burst hit
   ~8000 drops before the SDL main loop came online to pump; worldgen
   added more.  At ~150 B per LOG_INFO line, 16 MB ≈ 110k lines of
   headroom — plenty for worst-case worldgen.  Pump cap bumped to 64 KB
   so the ring also drains 16x faster once pumping is online; covers the
   sync between worldgen finishing and the main loop pumping. */
#define LOG_RING_SIZE        ((size_t)(16 * 1024 * 1024))  /* 16 MB */
#define LOG_FMT_BUF_LEN      ((size_t)1024)
#define LOG_PUMP_MAX_BYTES   ((size_t)(64 * 1024))         /* 64 KB */
#define LOG_FILE_NEW         "remom_log_new.txt"
#define LOG_FILE_CURRENT     "remom_log_current.txt"
#define LOG_FILE_PREVIOUS    "remom_log_previous.txt"
#define LOG_N_CATEGORIES     47

/* CALL_TRACE rng-call ordinal.  Definition lives here in libSTU so the
   stu_compat.h CALL_TRACE wrappers (part of libSTU via stu_compat.c) link
   cleanly even in libSTU-only targets like test_STU.  Random_at() in
   MoX/src/random.c increments it via the extern declaration in random.h. */
uint64_t g_random_call_count = 0;

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

/* Width sized to the longest per-file category name across the codebase
 * (currently ARTIFICIAL_HUMAN_PLAYER at 23 chars). Fixed-width keeps the
 * log column predictable for awk-by-position. */
static const char * const log_cat_str[] = {
    "GENERAL                ",
    "AIMOVE                 ",
    "COMBAT                 ",
    "SAVE                   ",
    "PFL                    ",
    "IKI                    ",
    "LOADSAVE               ",
    "AIDUDES                ",
    "HEADLESS_PFL           ",
    "REPLAY                 ",
    "SDL2_AUDIO             ",
    "SDL2_INIT              ",
    "SDL2_PFL               ",
    "SDL3_AUDIO             ",
    "SDL3_INIT              ",
    "SDL3_PFL               ",
    "WIN_PFL                ",
    "HEMOM                  ",
    "REMOM                  ",
    "ARTIFICIAL_HUMAN_PLAYER",
    "GAME_SAVE_DUMP         ",
    "STU_WRLD               ",
    "STU_VLD                ",
    "STU_SND                ",
    "STU_INIT               ",
    "STU_XTRC               ",
    "STU_FON                ",
    "STU_PRN                ",
    "STU_FIO                ",
    "STU_DBG                ",
    "STU_LOG                ",
    "NEXTTURN               ",
    "MOM_SCR                ",
    "LOADSCR                ",
    "LOADER                 ",
    "NEWGAME                ",
    "SPELLBOOK              ",
    "SPELLS128              ",
    "SBOOKSCR               ",
    "MAINMENU               ",
    "MAINSCR                ",
    "INPUT                  ",
    "RANDOM                 ",
    "EXIT                   ",
    "FONTS                  ",
    "MOX2                   ",
    "CALL_TRACE             "
};

static const char * const log_cat_ini_key[] = {
    "GENERAL",
    "AIMOVE",
    "COMBAT",
    "SAVE",
    "PFL",
    "IKI",
    "LOADSAVE",
    "AIDUDES",
    "HEADLESS_PFL",
    "REPLAY",
    "SDL2_AUDIO",
    "SDL2_INIT",
    "SDL2_PFL",
    "SDL3_AUDIO",
    "SDL3_INIT",
    "SDL3_PFL",
    "WIN_PFL",
    "HEMOM",
    "REMOM",
    "ARTIFICIAL_HUMAN_PLAYER",
    "GAME_SAVE_DUMP",
    "STU_WRLD",
    "STU_VLD",
    "STU_SND",
    "STU_INIT",
    "STU_XTRC",
    "STU_FON",
    "STU_PRN",
    "STU_FIO",
    "STU_DBG",
    "STU_LOG",
    "NEXTTURN",
    "MOM_SCR",
    "LOADSCR",
    "LOADER",
    "NEWGAME",
    "SPELLBOOK",
    "SPELLS128",
    "SBOOKSCR",
    "MAINMENU",
    "MAINSCR",
    "INPUT",
    "RANDOM",
    "EXIT",
    "FONTS",
    "MOX2",
    "CALL_TRACE"
};

static void STU_Log_Config_Set_Defaults(void)
{
    int i;
    log_cfg.sev_threshold = LOG_SEV_TRACE;
    for (i = 0; i < LOG_N_CATEGORIES; ++i)
    {
        log_cfg.cat_enabled[i] = 1;
    }
}

static int STU_Log_CI_Eq(const char * a, const char * b)
{
    while(*a != '\0' && *b != '\0')
    {
        if(tolower((unsigned char)*a) != tolower((unsigned char)*b))
        {
            return 0;
        }
        ++a;
        ++b;
    }
    return *a == '\0' && *b == '\0';
}

static char * STU_Log_Trim_WS(char * s)
{
    char * end;
    while(*s == ' ' || *s == '\t' || *s == '\r' || *s == '\n')
    {
        ++s;
    }
    end = s + strlen(s);
    while(end > s && (end[-1] == ' ' || end[-1] == '\t' || end[-1] == '\r' || end[-1] == '\n'))
    {
        --end;
    }
    *end = '\0';
    return s;
}

static int STU_Log_Parse_Severity(const char * value, int * out)
{
    if(STU_Log_CI_Eq(value, "TRACE"))
    {
        *out = LOG_SEV_TRACE;
        return 1;
    }
    else if(STU_Log_CI_Eq(value, "DEBUG"))
    {
        *out = LOG_SEV_DEBUG;
        return 1;
    }
    else if(STU_Log_CI_Eq(value, "INFO"))
    {
        *out = LOG_SEV_INFO;
        return 1;
    }
    else if(STU_Log_CI_Eq(value, "WARN"))
    {
        *out = LOG_SEV_WARN;
        return 1;
    }
    else if(STU_Log_CI_Eq(value, "ERROR"))
    {
        *out = LOG_SEV_ERROR;
        return 1;
    }
    else if(STU_Log_CI_Eq(value, "FATAL"))
    {
        *out = LOG_SEV_FATAL;
        return 1;
    }
    return 0;
}

static int STU_Log_Parse_Bool(const char * value, int * out)
{
    if(STU_Log_CI_Eq(value, "true") || STU_Log_CI_Eq(value, "yes") || STU_Log_CI_Eq(value, "on")  || STU_Log_CI_Eq(value, "1"))
    {
        *out = 1;
        return 1;
    }
    if(STU_Log_CI_Eq(value, "false") || STU_Log_CI_Eq(value, "no") || STU_Log_CI_Eq(value, "off") || STU_Log_CI_Eq(value, "0"))
    {
        *out = 0;
        return 1;
    }
    return 0;
}

static void STU_Log_Config_Load_INI(const char * path)
{
    FILE * fp;
    char   line[512];
    char * key;
    char * value;
    char * eq;
    int    in_logging_section = 0;
    int    parsed_int;
    int    i;

    if(path == NULL)
    {
        return;
    }
    fp = fopen(path, "r");
    if(fp == NULL)
    {
        return;
    }

    while(fgets(line, sizeof(line), fp) != NULL)
    {
        key = STU_Log_Trim_WS(line);
        if(*key == '\0' || *key == '#' || *key == ';')
        {
            continue;
        }
        if(*key == '[')
        {
            in_logging_section = (strncmp(key, "[Logging]", 9) == 0);
            continue;
        }
        if(!in_logging_section)
        {
            continue;
        }
        eq = strchr(key, '=');
        if(eq == NULL)
        {
            continue;
        }
        *eq = '\0';
        value = STU_Log_Trim_WS(eq + 1);
        key   = STU_Log_Trim_WS(key);

        if(STU_Log_CI_Eq(key, "severity_threshold"))
        {
            if(STU_Log_Parse_Severity(value, &parsed_int))
            {
                log_cfg.sev_threshold = parsed_int;
            }
            continue;
        }
        for (i = 0; i < LOG_N_CATEGORIES; ++i)
        {
            if(STU_Log_CI_Eq(key, log_cat_ini_key[i]))
            {
                if(STU_Log_Parse_Bool(value, &parsed_int))
                {
                    log_cfg.cat_enabled[i] = parsed_int;
                }
                break;
            }
        }
    }
    fclose(fp);
}

static const char * STU_Log_Basename(const char * path)
{
    const char * last_slash;
    const char * last_bslash;
    const char * last;

    if(path == NULL)
    {
        return "";
    }
    last_slash  = strrchr(path, '/');
    last_bslash = strrchr(path, '\\');
    last = (last_slash > last_bslash) ? last_slash : last_bslash;
    return (last != NULL) ? (last + 1) : path;
}

static size_t STU_Log_Ring_Used(void)
{
    if(log_head >= log_tail)
    {
        return log_head - log_tail;
    }
    return LOG_RING_SIZE - (log_tail - log_head);
}

static size_t STU_Log_Ring_Free(void)
{
    return LOG_RING_SIZE - STU_Log_Ring_Used() - 1;
}

static void STU_Log_Ring_Write_Bytes(const char * src, size_t n)
{
    size_t first_chunk;

    if(n == 0)
    {
        return;
    }

    if(log_head + n <= LOG_RING_SIZE)
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

static size_t STU_Log_Drain_Up_To(size_t cap)
{
    size_t used;
    size_t to_drain;
    size_t first_chunk;

    if(log_file == NULL)
    {
        log_tail = log_head;
        return 0;
    }

    used = STU_Log_Ring_Used();
    if(used == 0)
    {
        return 0;
    }
    to_drain = (used < cap) ? used : cap;

    if(log_tail + to_drain <= LOG_RING_SIZE)
    {
        fwrite(&log_ring[log_tail], 1, to_drain, log_file);
        log_tail = (log_tail + to_drain) % LOG_RING_SIZE;
    }
    else
    {
        first_chunk = LOG_RING_SIZE - log_tail;
        if(first_chunk >= to_drain)
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
static void STU_Log_Emit_Drop_Marker(void)
{
    char datetime[32];

    if(log_dropped_since_last_pump == 0 || log_file == NULL)
    {
        return;
    }
    get_datetime(datetime);
    fprintf(log_file, "[%s] [LOGGER] %lu messages dropped since last drain\n",
            datetime, (unsigned long)log_dropped_since_last_pump);
    log_dropped_since_last_pump = 0;
}

static void STU_Log_Rotate_Files(void)
{
    /* remove(<missing>) returns -1 and rename(<missing>, ...) does the same; both are safe to ignore. */
    remove(LOG_FILE_PREVIOUS);
    rename(LOG_FILE_CURRENT, LOG_FILE_PREVIOUS);
    rename(LOG_FILE_NEW,     LOG_FILE_CURRENT);
}

static void STU_Log_Emit_Crash_Marker(const char * signal_name)
{
    char datetime[32];

    if(log_file == NULL)
    {
        return;
    }
    get_datetime(datetime);
    fprintf(log_file, "[%s] [CRASH] %s\n", datetime, signal_name);
}

static void STU_Log_Atexit_Handler(void)
{
    STU_Log_Shutdown();
}

#if defined(_WIN32)

static LPTOP_LEVEL_EXCEPTION_FILTER log_previous_seh_filter = NULL;

static const char * STU_Log_SEH_Code_Name(DWORD code)
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

static LONG WINAPI STU_Log_SEH_Filter(EXCEPTION_POINTERS * ep)
{
    STU_Log_Emit_Crash_Marker(STU_Log_SEH_Code_Name(ep->ExceptionRecord->ExceptionCode));
    if(log_file != NULL)
    {
        char dt[32];
        void * base = (void *)GetModuleHandleA(NULL);
        void * addr = (void *)ep->ExceptionRecord->ExceptionAddress;
        get_datetime(dt);
        fprintf(log_file, "[%s] [CRASH] code=0x%08lX addr=%p module_base=%p rva=0x%llX\n",
                dt,
                (unsigned long)ep->ExceptionRecord->ExceptionCode,
                addr, base,
                (unsigned long long)((char *)addr - (char *)base));
    }
    STU_Log_Flush_All();
    if(log_file != NULL)
    {
        fflush(log_file);
        fclose(log_file);
        log_file = NULL;
    }
    /* EXCEPTION_EXECUTE_HANDLER terminates the process without invoking the JIT debugger / WER dialog, which makes crash tests run cleanly. */
    return EXCEPTION_EXECUTE_HANDLER;
}

static void STU_Log_Install_Crash_Handlers(void)
{
    log_previous_seh_filter = SetUnhandledExceptionFilter(STU_Log_SEH_Filter);
}

#else  /* POSIX */

static const char * STU_Log_Signal_Name(int sig)
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
static void STU_Log_Signal_Handler(int sig)
{
    STU_Log_Emit_Crash_Marker(STU_Log_Signal_Name(sig));
    STU_Log_Flush_All();
    if(log_file != NULL)
    {
        fflush(log_file);
        fclose(log_file);
        log_file = NULL;
    }
    signal(sig, SIG_DFL);
    raise(sig);
}

static void STU_Log_Install_Crash_Handlers(void)
{
    signal(SIGSEGV, STU_Log_Signal_Handler);
    signal(SIGABRT, STU_Log_Signal_Handler);
    signal(SIGFPE,  STU_Log_Signal_Handler);
    signal(SIGILL,  STU_Log_Signal_Handler);
}

#endif

void STU_Log_Startup(const char * ini_path)
{
    static int atexit_registered = 0;
    static int crash_handlers_installed = 0;

    if(log_file != NULL)
    {
        fflush(log_file);
        fclose(log_file);
        log_file = NULL;
    }

    log_head = 0;
    log_tail = 0;
    log_dropped_since_last_pump = 0;
    STU_Log_Config_Set_Defaults();
    STU_Log_Config_Load_INI(ini_path);

    STU_Log_Rotate_Files();

    log_file = fopen(LOG_FILE_NEW, "w");
    if(log_file == NULL)
    {
        fprintf(stderr, "STU_LOG: failed to open '%s' for writing\n", LOG_FILE_NEW);
    }

    if(!atexit_registered)
    {
        atexit(STU_Log_Atexit_Handler);
        atexit_registered = 1;
    }
    if(!crash_handlers_installed)
    {
        STU_Log_Install_Crash_Handlers();
        crash_handlers_installed = 1;
    }
}

void STU_Log_Shutdown(void)
{
    if(log_file != NULL)
    {
        STU_Log_Emit_Drop_Marker();
        while(STU_Log_Drain_Up_To(LOG_RING_SIZE) > 0)
        {
            /* keep draining until empty */
        }
        fflush(log_file);
        fclose(log_file);
        log_file = NULL;
    }
}

void STU_Log_Pump(void)
{
    STU_Log_Emit_Drop_Marker();
    STU_Log_Drain_Up_To(LOG_PUMP_MAX_BYTES);
    if(log_file != NULL)
    {
        fflush(log_file);
    }
}

void STU_Log_Flush_All(void)
{
    if(log_file == NULL)
    {
        return;
    }
    STU_Log_Emit_Drop_Marker();
    while(STU_Log_Drain_Up_To(LOG_RING_SIZE) > 0)
    {
        /* keep draining until empty */
    }
    fflush(log_file);
}

void STU_Log_Write_At_V(int sev, enum log_category cat, const char * file, int line, const char * func, const char * fmt, va_list args)
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
    if(sev_idx < 0 || sev_idx > LOG_SEV_FATAL)
    {
        sev_idx = LOG_SEV_INFO;
    }
    if(cat_idx < 0 || cat_idx >= LOG_N_CATEGORIES)
    {
        cat_idx = LOG_CAT_GENERAL;
    }

    if(sev_idx < log_cfg.sev_threshold)
    {
        return;
    }
    if(!log_cfg.cat_enabled[cat_idx])
    {
        return;
    }

    get_datetime(datetime);

    header_len = snprintf(stack_buf, LOG_FMT_BUF_LEN, "[%s] [%s] [%s] %s:%d %s: ",
                          datetime,
                          log_sev_str[sev_idx],
                          log_cat_str[cat_idx],
                          STU_Log_Basename(file),
                          line,
                          (func != NULL) ? func : "");
    if(header_len < 0 || (size_t)header_len >= LOG_FMT_BUF_LEN)
    {
        return;
    }

    body_len = LOG_VSNPRINTF(stack_buf + header_len, LOG_FMT_BUF_LEN - (size_t)header_len, fmt, args);
    if(body_len < 0)
    {
        return;
    }

    total_len = (size_t)header_len + (size_t)body_len;
    if(total_len >= LOG_FMT_BUF_LEN - 1)
    {
        total_len = LOG_FMT_BUF_LEN - 2;
    }
    stack_buf[total_len] = '\n';
    total_len += 1;

    if(total_len > STU_Log_Ring_Free())
    {
        ++log_dropped_since_last_pump;
        return;
    }

    STU_Log_Ring_Write_Bytes(stack_buf, total_len);
}

void STU_Log_Write_At(int sev, enum log_category cat, const char * file, int line, const char * func, const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    STU_Log_Write_At_V(sev, cat, file, line, func, fmt, args);
    va_end(args);
}
