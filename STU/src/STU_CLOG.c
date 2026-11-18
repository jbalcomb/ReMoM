/*
    STU_CLOG -- Catalog logging.  See STU_CLOG.h.

    Moved from MoX/src/Fields.c (the field-origin shadow, its accessors, Dbg_Basename, and the FIELDADD trace knobs).
*/

#include "STU_CLOG.h"

#if STU_CLOG

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>   /* atexit(), getenv() */

#include "STU_LOG.h"  /* STU_Log_Path_For() */

#include <string.h>   /* strncpy() */


/* ---------------- output files: one per catalog, opened on the first row, closed at exit ---------------- */

enum { CLOG_FILE_FIELDS = 0, CLOG_FILE_DRAW = 1, CLOG_FILE_PALETTE = 2, CLOG_FILE_COUNT = 3 };
static const char * const clog_file_name[CLOG_FILE_COUNT] = { "remom_clog_fields.tsv", "remom_clog_draw.tsv", "remom_clog_palette.tsv" };
static const char * const clog_file_gate[CLOG_FILE_COUNT] = { "REMOM_CLOG_FIELDS", "REMOM_CLOG_DRAW", "REMOM_CLOG_DRAW" };
static FILE * clog_fp[CLOG_FILE_COUNT];
static int clog_enabled[CLOG_FILE_COUNT] = { -1, -1, -1 };
static int clog_open_failed[CLOG_FILE_COUNT];
static int clog_atexit_registered = 0;

static void STU_CLOG_Close_Files(void)
{
    int itr;
    for(itr = 0; itr < CLOG_FILE_COUNT; itr++)
    {
        if(clog_fp[itr] != NULL) { fclose(clog_fp[itr]); clog_fp[itr] = NULL; }
    }
}

/* run-time gate: the file's environment variable set, non-empty and not "0".  These catalogs write a lot of rows, so they are off unless asked for. */
static int STU_CLOG_File_Enabled(int which)
{
    if(clog_enabled[which] < 0)
    {
        const char * env = getenv(clog_file_gate[which]);
        clog_enabled[which] = (env != NULL && env[0] != '\0' && !(env[0] == '0' && env[1] == '\0')) ? 1 : 0;
    }
    return clog_enabled[which];
}

static void STU_CLOG_Row_V(int which, const char * fmt, va_list args)
{
    if(STU_CLOG_File_Enabled(which) == 0) { return; }
    if(clog_fp[which] == NULL)
    {
        char path[600];
        if(clog_open_failed[which]) { return; }
        STU_Log_Path_For(path, sizeof(path), clog_file_name[which]);
        clog_fp[which] = fopen(path, "w");
        if(clog_fp[which] == NULL)
        {
            clog_open_failed[which] = 1;
            LOG_ERROR(LOG_CAT_STU_LOG, "[STU_CLOG] could not open %s for writing; its catalog rows are dropped this run", path);
            return;
        }
        if(clog_atexit_registered == 0) { atexit(STU_CLOG_Close_Files); clog_atexit_registered = 1; }
    }
    vfprintf(clog_fp[which], fmt, args);
    fputc('\n', clog_fp[which]);
}

static void STU_CLOG_Row(int which, const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    STU_CLOG_Row_V(which, fmt, args);
    va_end(args);
}

void STU_CLOG_Fields_Row(const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    STU_CLOG_Row_V(CLOG_FILE_FIELDS, fmt, args);
    va_end(args);
}


/* ---------------- picture / drawing catalog ---------------- */

/* identity map: the pointer an LBX load returned -> "NAME.LBX:entry".  Open addressing on the pointer; a reload into the same buffer overwrites. */
#define CLOG_ASSET_SLOTS 8192
#define CLOG_ASSET_NAME_LEN 24
static const void * clog_asset_ptr[CLOG_ASSET_SLOTS];
static char clog_asset_name[CLOG_ASSET_SLOTS][CLOG_ASSET_NAME_LEN];
static int clog_asset_full_logged = 0;

static unsigned int STU_CLOG_Asset_Hash(const void * ptr)
{
    size_t v = (size_t)ptr;
    v ^= (v >> 16);
    v *= 0x45D9F3Bu;
    v ^= (v >> 16);
    return (unsigned int)(v & (CLOG_ASSET_SLOTS - 1));
}

void STU_CLOG_Asset_Loaded(const void * ptr, const char * lbx_name, int16_t entry)
{
    unsigned int slot;
    unsigned int probe;
    const char * base;
    char upper[16];
    int itr;
    if(ptr == NULL || lbx_name == NULL) { return; }
    base = STU_CLOG_Basename(lbx_name);
    for(itr = 0; itr < (int)sizeof(upper) - 1 && base[itr] != '\0' && base[itr] != '.'; itr++)
    {
        upper[itr] = (char)((base[itr] >= 'a' && base[itr] <= 'z') ? (base[itr] - 'a' + 'A') : base[itr]);
    }
    upper[itr] = '\0';
    slot = STU_CLOG_Asset_Hash(ptr);
    for(probe = 0; probe < CLOG_ASSET_SLOTS; probe++)
    {
        unsigned int s = (slot + probe) & (CLOG_ASSET_SLOTS - 1);
        if(clog_asset_ptr[s] == NULL || clog_asset_ptr[s] == ptr)
        {
            clog_asset_ptr[s] = ptr;
            snprintf(clog_asset_name[s], CLOG_ASSET_NAME_LEN, "%s.LBX:%d", upper, (int)entry);
            return;
        }
    }
    if(clog_asset_full_logged == 0)
    {
        clog_asset_full_logged = 1;
        LOG_WARN(LOG_CAT_STU_LOG, "[STU_CLOG] asset identity map is full (%d slots); later loads are unnamed", CLOG_ASSET_SLOTS);
    }
}

const char * STU_CLOG_Asset_Name(const void * ptr)
{
    unsigned int slot;
    unsigned int probe;
    if(ptr == NULL) { return ""; }
    slot = STU_CLOG_Asset_Hash(ptr);
    for(probe = 0; probe < CLOG_ASSET_SLOTS; probe++)
    {
        unsigned int s = (slot + probe) & (CLOG_ASSET_SLOTS - 1);
        if(clog_asset_ptr[s] == ptr) { return clog_asset_name[s]; }
        if(clog_asset_ptr[s] == NULL) { return ""; }
    }
    return "";
}

/* palette generation: bumped by every palette load; each draw records the generation it was drawn under */
static uint32_t clog_palette_generation = 0;

void STU_CLOG_Palette_Loaded(const char * source, int16_t frame, int16_t start, int16_t count)
{
    clog_palette_generation += 1;
    STU_CLOG_Row(CLOG_FILE_PALETTE, "%lu\t%s\t%d\t%d\t%d", (unsigned long)clog_palette_generation, (source != NULL && source[0] != '\0') ? source : "?", (int)frame, (int)start, (int)count);
}

/* draw ring: the most recent draws, newest last.  There is no per-frame clear: screens often draw once and then copy pages, so a sprite stays on
   screen without being redrawn.  A hit test scans newest-first, so the most recently drawn sprite at the point wins. */
#define CLOG_DRAW_RING 4096
struct s_CLOG_DRAW { int16_t x1, y1, x2, y2; uint32_t pal; char name[CLOG_ASSET_NAME_LEN]; };
static struct s_CLOG_DRAW clog_draw_ring[CLOG_DRAW_RING];
static uint32_t clog_draw_seq = 0;   /* draws so far; the newest is at (seq - 1) % CLOG_DRAW_RING */

void STU_CLOG_Draw(int16_t x, int16_t y, int16_t width, int16_t height, const void * picture)
{
    struct s_CLOG_DRAW * d = &clog_draw_ring[clog_draw_seq % CLOG_DRAW_RING];
    const char * name = STU_CLOG_Asset_Name(picture);
    d->x1 = x;
    d->y1 = y;
    d->x2 = (int16_t)(x + width - 1);
    d->y2 = (int16_t)(y + height - 1);
    d->pal = clog_palette_generation;
    strncpy(d->name, (name[0] != '\0') ? name : "?", CLOG_ASSET_NAME_LEN - 1);
    d->name[CLOG_ASSET_NAME_LEN - 1] = '\0';
    clog_draw_seq += 1;
    STU_CLOG_Row(CLOG_FILE_DRAW, "%lu\t%s\t%d\t%d\t%d\t%d\t%lu", (unsigned long)clog_draw_seq, d->name, (int)d->x1, (int)d->y1, (int)d->x2, (int)d->y2, (unsigned long)d->pal);
}

int STU_CLOG_Draw_Hit(int16_t x, int16_t y, char * out, size_t cap)
{
    uint32_t back;
    uint32_t have = (clog_draw_seq < CLOG_DRAW_RING) ? clog_draw_seq : CLOG_DRAW_RING;
    for(back = 1; back <= have; back++)
    {
        const struct s_CLOG_DRAW * d = &clog_draw_ring[(clog_draw_seq - back) % CLOG_DRAW_RING];
        if(x >= d->x1 && x <= d->x2 && y >= d->y1 && y <= d->y2)
        {
            snprintf(out, cap, "%s@(%d,%d)-(%d,%d)", d->name, (int)d->x1, (int)d->y1, (int)d->x2, (int)d->y2);
            return 1;
        }
    }
    if(cap > 0) { out[0] = '\0'; }
    return 0;
}


/* ---------------- input-field catalog ---------------- */

const char * g_clog_field_call_file = "";
int32_t g_clog_field_call_line = 0;

int16_t g_clog_fields_trace = 0;
const char * g_clog_fields_screen_tag = "";

/* per-field-index origin shadow: the Add_* call site (file + line) of each registered field.  p_fields holds ~150 entries. */
#define STU_CLOG_FIELD_ORIGIN_MAX 256
static const char * clog_field_origin_file[STU_CLOG_FIELD_ORIGIN_MAX];
static int32_t      clog_field_origin_line[STU_CLOG_FIELD_ORIGIN_MAX];


const char * STU_CLOG_Basename(const char * path)
{
    const char * slash;
    const char * p;
    if(path == NULL) { return ""; }
    slash = path;
    for(p = path; *p != '\0'; p += 1)
    {
        if(*p == '/' || *p == '\\') { slash = p + 1; }
    }
    return slash;
}

void STU_CLOG_Field_Origin_Set(int16_t idx)
{
    if(idx < 0 || idx >= STU_CLOG_FIELD_ORIGIN_MAX) { return; }
    clog_field_origin_file[idx] = g_clog_field_call_file;
    clog_field_origin_line[idx] = g_clog_field_call_line;
}

void STU_CLOG_Field_Origins_Clear(int16_t count)
{
    int16_t itr;
    for(itr = 0; itr < count && itr < STU_CLOG_FIELD_ORIGIN_MAX; itr++)
    {
        clog_field_origin_file[itr] = NULL;
        clog_field_origin_line[itr] = 0;
    }
}

const char * STU_CLOG_Field_Origin_File(int16_t idx)
{
    if(idx < 0 || idx >= STU_CLOG_FIELD_ORIGIN_MAX) { return ""; }
    return STU_CLOG_Basename(clog_field_origin_file[idx]);
}

int32_t STU_CLOG_Field_Origin_Line(int16_t idx)
{
    if(idx < 0 || idx >= STU_CLOG_FIELD_ORIGIN_MAX) { return 0; }
    return clog_field_origin_line[idx];
}

#endif  /* STU_CLOG */
