/*
    STU_GD -- Game-Data capture: "[GD]" state dumps for OG-vs-ReMoM matchup.

    Emits the game's state arrays in the SAME text format as the OG-side STU-DOSBox probe, driven by the SAME generated field maps
    (gd_*_fields.h, from gen-fieldmap.py), so the two logs diff field-for-field (compare-gd).  This is reverse-engineering
    evaluation code: it has no counterpart in the original game.

    The output format is a contract with the OG-side tooling.  Do not change a byte of it:
        [GD] <point> <label>[<idx>].<field> = <value>        records  (STU_GD_Dump_Record / STU_GD_Dump_Records)
        [GD] <point> <label>[<plane>].y<NN> = <csv>           per-square grids  (STU_GD_Dump_Grid)
        [GD] <point> <label>[<idx>]<member>@<base> = <csv>    <=100-value chunks  (STU_GD_Dump_Chunked)

    Gating
        compile time:  STU_GD_CAPTURE  (defaults on when STU_DEBUG is defined; off in Release)
        run time:      REMOM_GD_CAPTURE environment variable  (set, non-empty, and not "0")
    With either gate off, the STU_GD_ONCE / STU_GD_DO / STU_GD_TRACE call-site macros do nothing, so no formatting cost is paid.
*/

#ifndef STU_GD_H
#define STU_GD_H

#include <stddef.h>
#include <stdint.h>

#include "STU_LOG.h"

#if defined(STU_DEBUG) && !defined(STU_GD_CAPTURE)
#define STU_GD_CAPTURE 1
#endif

/* Every [GD] line goes through STU_GD_WRITE, not LOG_TRACE: it writes at TRACE severity directly, so the Release STU_LOG_MIN_SEVERITY floor does not
   strip it.  CMake -DREMOM_GD_CAPTURE=ON builds the capture into every config.  The run-time gates still apply: REMOM_GD_CAPTURE set, and
   severity_threshold = TRACE in the [Logging] INI. */
#if defined(STU_GD_CAPTURE)
#define STU_GD_WRITE(...)  STU_Log_Write_At(LOG_SEV_TRACE, LOG_CAT_GENERAL, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define STU_GD_WRITE(...)  LOG__NOOP(LOG_CAT_GENERAL, __VA_ARGS__)
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Field kinds and field descriptor.  These names are used by the generated gd_*_fields.h headers -- keep them as they are. */
enum { GD_U8 = 0, GD_I8, GD_U16, GD_I16, GD_U32, GD_I32, GD_STR, GD_BYTES };
typedef struct { uint16_t off; uint8_t kind; uint16_t count; const char* name; } gd_field_t;

/* Getter for one world-map square; returns the value to print. */
typedef int (*STU_GD_Square_Fn)(int wx, int wy, int wp);

/* 1 when both gates are on.  The run-time gate is read once and cached. */
int STU_GD_Is_Enabled(void);

/* 1 the first time it is called with a given point string (compared by content), 0 after that.  Backs STU_GD_ONCE. */
int STU_GD_First_Time(const char * point);

/* Format one field of a record into val (always NUL-terminated).  Decode matches the OG-side gd_decode_record. */
void STU_GD_Format_Field(char * val, int val_size, const uint8_t * field_ptr, const gd_field_t * field);

/* Format array[first..end) as "v,v,v" into buf.  kind is GD_U8 / GD_I8 / GD_U16 / GD_I16 / GD_U32 / GD_I32.  Returns the length written. */
int STU_GD_Format_Csv(char * buf, int buf_size, const void * array, int kind, int first, int end);

/* array[0..count) as one line: "[GD] <point> <name> = v,v,v".  No flush. */
void STU_GD_Dump_Csv(const char * point, const char * name, const void * array, int kind, int count);

/* One record: one [GD] line per field, then a log flush (each dump is a burst with no main-loop pump to drain the async ring). */
void STU_GD_Dump_Record(const char * point, const char * label, int idx, const void * record, const gd_field_t * fields, int field_count);

/* count contiguous records of stride bytes, each through STU_GD_Dump_Record. */
void STU_GD_Dump_Records(const char * point, const char * label, const void * base, size_t stride, int count, const gd_field_t * fields, int field_count);

/* A planes x height x width per-square grid: one [GD] line per (plane, row), a log flush per plane. */
void STU_GD_Dump_Grid(const char * point, const char * label, STU_GD_Square_Fn square_fn, int width, int height, int planes);

/* array[0..count) in chunks of <=100 values, one [GD] line per chunk tagged with its start index (keeps each line under the logger's LOG_FMT_BUF_LEN).
   member is appended after "[idx]" -- pass ".heads" or "" -- so the line reads "<label>[<idx>]<member>@<base>".  No flush; the caller flushes. */
void STU_GD_Dump_Chunked(const char * point, const char * label, int idx, const char * member, const void * array, int kind, int count);

#ifdef __cplusplus
}
#endif

/* Call-site macros.  Every probe in game code goes through one of these so it compiles out with STU_GD_CAPTURE and costs nothing when REMOM_GD_CAPTURE is unset. */
#if defined(STU_GD_CAPTURE)
/* run the statements the first time this point is reached (keyed by point string, so two sites sharing a point fire once between them) */
#define STU_GD_ONCE(point_, ...)  do { if(STU_GD_Is_Enabled() && STU_GD_First_Time(point_)) { __VA_ARGS__; } } while(0)
/* run the statements every time */
#define STU_GD_DO(...)            do { if(STU_GD_Is_Enabled()) { __VA_ARGS__; } } while(0)
/* a one-off [GD] scalar line; the format string carries its own "[GD] " prefix */
#define STU_GD_TRACE(...)         do { if(STU_GD_Is_Enabled()) { STU_GD_WRITE(__VA_ARGS__); } } while(0)
#else
#define STU_GD_ONCE(point_, ...)  do { } while(0)
#define STU_GD_DO(...)            do { } while(0)
#define STU_GD_TRACE(...)         do { } while(0)
#endif

#endif /* STU_GD_H */
