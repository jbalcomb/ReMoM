/*
    STU_CLOG -- Catalog logging: data collection for the input-field and picture / drawing catalogs.

    Not debugging.  It records where things come from (the source call site of each input field; later, which LBX entry each drawn sprite is),
    so the static catalogs (tools/field_catalog, tools/asset_catalog) can be joined to what a run actually did, for HMS / rmr2hms click naming,
    graphics upscaling and screen rearranging.  See doc/@Devel/Logging-And-Instrumentation.md section 4.

    Gating
        compile time:  STU_CLOG  1 = built in, 0 = compiled out.  Defaults to 1 with STU_DEBUG, else 0.  CMake: -DREMOM_CLOG=ON turns it on for every
                       config, so a catalog run can use the faster Release build.
        run time:      the consumers gate themselves (the record log is written only with --record).

    This module holds no game types, so it links into anything.  The hooks that read game data (p_fields) stay in the game code, under #if STU_CLOG.
*/

#ifndef STU_CLOG_H
#define STU_CLOG_H

#include <stddef.h>   /* size_t */
#include <stdint.h>

#ifndef STU_CLOG
#ifdef STU_DEBUG
#define STU_CLOG 1
#else
#define STU_CLOG 0
#endif
#endif

#if STU_CLOG

#ifdef __cplusplus
extern "C" {
#endif

/*
    Input-field catalog

    Each Add_*Field call site is captured by the STU_CLOG_FIELD_CALL_SITE() wrapper macros in MoX/src/Fields.h, and each Add_* records it against the
    new field's index.  The record / replay field-hit log reads it back to stamp a click with "@basename:line" -- the join key into
    tools/fields/catalog.fwv and the HMS alias table (tools/field_catalog/resolver.py).
*/

/* the call site of the Add_*Field call in progress, set by STU_CLOG_FIELD_CALL_SITE() */
extern const char * g_clog_field_call_file;
extern int32_t g_clog_field_call_line;

#define STU_CLOG_FIELD_CALL_SITE()  (g_clog_field_call_file = __FILE__, g_clog_field_call_line = __LINE__)

/* store the captured call site against field index idx (out-of-range idx is ignored) */
void STU_CLOG_Field_Origin_Set(int16_t idx);
/* forget the origins of fields 0 .. count-1, when the field table is cleared */
void STU_CLOG_Field_Origins_Clear(int16_t count);
/* the origin of field idx: the call site's file basename ("" if none) and line (0 if none) */
const char * STU_CLOG_Field_Origin_File(int16_t idx);
int32_t STU_CLOG_Field_Origin_Line(int16_t idx);
/* the part of path after its last '/' or '\' */
const char * STU_CLOG_Basename(const char * path);

/* FIELDADD / FIELDSNAPSHOT rows: set non-zero to write them to remom_clog_fields.tsv (Clog_Field_Added / Dump_Fields_CSV in MoX/src/Fields.c) */
extern int16_t g_clog_fields_trace;
/* write one tab-separated row (no trailing newline in fmt) to remom_clog_fields.tsv in the log directory; the file is created on the first row of a
   run, overwriting the previous run's, and closed at exit.  Rows are written only when the REMOM_CLOG_FIELDS environment variable is set (non-empty,
   not "0"): Main_Screen re-adds its fields every loop pass, so this is a lot of rows. */
void STU_CLOG_Fields_Row(const char * fmt, ...);
/* short label for the current screen in those lines (e.g. "Main_Screen") */
extern const char * g_clog_fields_screen_tag;

/*
    Picture / drawing catalog  (doc/@AI_Plans/PLAN-Draw-Catalog.md, DB B)

    LBX_Load_Entry records which LBX entry each loaded buffer holds; FLIC_Draw records each draw (bbox + that name + the palette generation) in a
    ring of recent draws; the palette loaders bump the palette generation.  The record / replay field-hit log asks for the topmost recent draw under
    a click and stamps "draw=NAME.LBX:entry@(x1,y1)-(x2,y2)".  With REMOM_CLOG_DRAW set, every draw is also a row in remom_clog_draw.tsv
    (seq, name, x1, y1, x2, y2, pal) and every palette load a row in remom_clog_palette.tsv (pal, source, frame, start, count).
*/

/* the buffer at ptr now holds entry `entry` of lbx_name (called by LBX_Load_Entry) */
void STU_CLOG_Asset_Loaded(const void * ptr, const char * lbx_name, int16_t entry);
/* "NAME.LBX:entry" for a loaded buffer, or "" if it was not loaded through LBX_Load_Entry */
const char * STU_CLOG_Asset_Name(const void * ptr);
/* a picture was drawn at (x, y), width x height (called by FLIC_Draw) */
void STU_CLOG_Draw(int16_t x, int16_t y, int16_t width, int16_t height, const void * picture);
/* the most recent draw covering (x, y), as "NAME.LBX:entry@(x1,y1)-(x2,y2)"; returns 0 (and "") when none does */
int STU_CLOG_Draw_Hit(int16_t x, int16_t y, char * out, size_t cap);
/* a palette was loaded: source "FONTS.LBX:n" or a picture's "NAME.LBX:entry", its frame (-1 for a base palette), and the colors it wrote */
void STU_CLOG_Palette_Loaded(const char * source, int16_t frame, int16_t start, int16_t count);

#ifdef __cplusplus
}
#endif

#endif  /* STU_CLOG */

#endif  /* STU_CLOG_H */
