#include "STU_LOG.h"

#include "../../ext/stu_compat.h"
#include "STU_UTIL.h"

#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>

#if defined(_MSC_VER)
#define LOG_VSNPRINTF _vsnprintf
#else
#define LOG_VSNPRINTF vsnprintf
#endif

#define LOG_RING_SIZE   ((size_t)(2 * 1024 * 1024))  /* 2 MB */
#define LOG_FMT_BUF_LEN ((size_t)1024)
#define LOG_OUTPUT_FILE "remom_log.txt"

static char   log_ring[LOG_RING_SIZE];
static size_t log_head = 0;
static size_t log_tail = 0;
static FILE * log_file = NULL;

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

void log_init(void)
{
    log_head = 0;
    log_tail = 0;
    log_file = fopen(LOG_OUTPUT_FILE, "w");
    if (log_file == NULL)
    {
        fprintf(stderr, "STU_LOG: failed to open '%s' for writing\n", LOG_OUTPUT_FILE);
    }
}

void log_shutdown(void)
{
    log_pump();
    if (log_file != NULL)
    {
        fflush(log_file);
        fclose(log_file);
        log_file = NULL;
    }
}

void log_pump(void)
{
    size_t used;
    size_t first_chunk;

    if (log_file == NULL)
    {
        log_tail = log_head;
        return;
    }

    used = log_ring_used();
    if (used == 0)
    {
        return;
    }

    if (log_tail + used <= LOG_RING_SIZE)
    {
        fwrite(&log_ring[log_tail], 1, used, log_file);
    }
    else
    {
        first_chunk = LOG_RING_SIZE - log_tail;
        fwrite(&log_ring[log_tail], 1, first_chunk, log_file);
        fwrite(&log_ring[0], 1, used - first_chunk, log_file);
    }
    fflush(log_file);
    log_tail = log_head;
}

void log_write_v(enum log_severity sev, enum log_category cat, const char * fmt, va_list args)
{
    char   stack_buf[LOG_FMT_BUF_LEN];
    char   datetime[32];
    int    header_len;
    int    body_len;
    size_t total_len;

    (void)sev;
    (void)cat;

    get_datetime(datetime);

    header_len = snprintf(stack_buf, LOG_FMT_BUF_LEN, "[%s] ", datetime);
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
        return;
    }

    log_ring_write_bytes(stack_buf, total_len);
}

void log_write(enum log_severity sev, enum log_category cat, const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    log_write_v(sev, cat, fmt, args);
    va_end(args);
}
