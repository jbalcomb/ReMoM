/**
 * lbx_image_export.c — Export Master of Magic LBX picture entries (FLIC animations) to PNG, decoded by the engine's own FLIC_Draw().
 *
 * Phase 1a tracer bullet of doc/@AI_Plans/PLAN-Graphics-Upscale.md.
 *
 * Usage:
 *   lbx_image_export [-d DATA_DIR] [-o OUT_DIR] [-p PALETTE] [-P ARCHIVE:ENTRY] ARCHIVE ENTRY [ENTRY ...]
 *
 * Example:
 *   lbx_image_export -d assets -o out LOAD 0     -> out/LOAD_000_indexed.png, out/LOAD_000_rgba.png
 *
 * Why this links the engine instead of parsing FLIC itself:
 *   The game puts these entries on screen with FLIC_Draw() (MoX/src/FLIC_Draw.c) -- e.g. MoM/src/LoadScr.c draws LOAD.LBX entry 0 with FLIC_Draw(0, 0, loadsave_background).  Calling that
 *   same function makes the export the game's decode by construction, so the project carries one authoritative FLIC reader, not two that can drift apart.  The LBX container is read by the
 *   engine's LBX_Load() for the same reason (tools/lbx_extract.c is a standalone container reader for sound entries and is deliberately left engine-free).
 *
 * How transparency is recovered:
 *   FLIC frames encode transparency as pixels the decoder never writes (0xFF column skips, row deltas).  Each entry is drawn twice, onto a page pre-filled with 0x00 and then with 0xFF.  A pixel that
 *   reads the same both times was written (opaque); one that differs was skipped (transparent).  Any difference outside the declared width x height box is a decode/data error and is reported.
 *
 * Palette:
 *   The base palette is FONTS.LBX entry (PALETTE + 2), first 768 bytes -- the same entry and bytes Load_Palette() (MoX/src/Fonts.c) copies into current_palette.  FLIC_Draw() then overlays the
 *   entry's own palette range, when it has one, through FLIC_Load_Palette().  6-bit VGA components expand to 8-bit with << 2, exactly as Platform_Palette_Update() does
 *   (platform/sdl2/sdl2_Video.c, platform/win32/win_PFL.cpp), so the RGBA output is byte-for-byte what the game presents.
 *
 * Outputs per entry (stem and index from the archive, e.g. LOAD_000):
 *   <STEM>_<NNN>_indexed.png   8-bit indexed PNG: the decoded palette indices plus the 256-colour palette -- the index-preserving export.  Transparent pixels are written as ST_TRANSPARENT (0) and
 *                              marked transparent with a tRNS chunk, unless index 0 is also drawn opaque somewhere in the entry; that conflict is reported and the indexed file then has no tRNS.
 *   <STEM>_<NNN>_rgba.png      32-bit RGBA PNG under that palette; transparent pixels have alpha 0.
 *
 * Tracer-bullet limits (deliberate; see PLAN Phase 1b): frame 0 only; remap-flagged entries are refused (FLIC_Remap_Draw_Frame() reads the background, so they are not plain sprites); entries larger
 * than 320x200 are refused (FLIC_Draw() asserts on them).  A missing archive or entry exits through the engine's LBX error handler.
 *
 * PNG encoding is dependency-free: a zlib stream of "stored" (uncompressed) deflate blocks.  Files are larger than they need to be; compression is a Phase 1e concern.
 */

#define _CRT_SECURE_NO_WARNINGS

#include "../MoX/src/MOX_TYPE.h"
#include "../MoX/src/MOX_BITS.h"
#include "../MoX/src/MOX_DEF.h"
#include "../MoX/src/FLIC_Draw.h"
#include "../MoX/src/Fonts.h"
#include "../MoX/src/LBX_Load.h"
#include "../MoX/src/Video.h"
#include "../STU/src/STU_GRAF.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/* ========================================================================= */
/*  Constants                                                                */
/* ========================================================================= */

#define PALETTE_COLOR_COUNT          256
#define PALETTE_BYTES                768     /* 256 colours x 3 components, 6-bit VGA */
#define PALETTE_BLOCK_BYTES          1024    /* current_palette layout: 768 palette bytes + 256 change flags (Fonts.c allocates 64 paragraphs) */
#define FONTS_PALETTE_ENTRY_BASE     2       /* Load_Palette(entry) reads FONTS.LBX entry (entry + 2), MoX/src/Fonts.c */
#define VGA_TO_8BIT_SHIFT            2       /* Platform_Palette_Update(): component << 2 */
#define PAGE_FILL_A                  0x00
#define PAGE_FILL_B                  0xFF
#define PAGE_PIXEL_COUNT             (SCREEN_WIDTH * SCREEN_HEIGHT)
#define MAX_PATH_LEN                 1024
#define MAX_STEM_LEN                 64

#define PNG_COLOR_TYPE_INDEXED       3
#define PNG_COLOR_TYPE_RGBA          6
#define ZLIB_STORED_BLOCK_MAX        65535
#define ADLER32_MODULUS              65521u



/* ========================================================================= */
/*  PNG writer (stored deflate, no dependencies)                             */
/* ========================================================================= */

static uint32_t crc32_table[256];
static int crc32_table_ready = 0;

static void Build_Crc32_Table(void)
{
    uint32_t value;
    int table_index;
    int bit_index;

    for(table_index = 0; table_index < 256; table_index++)
    {
        value = (uint32_t)table_index;
        for(bit_index = 0; bit_index < 8; bit_index++)
        {
            if((value & 1u) != 0)
            {
                value = 0xEDB88320u ^ (value >> 1);
            }
            else
            {
                value = value >> 1;
            }
        }
        crc32_table[table_index] = value;
    }
    crc32_table_ready = 1;
}

/* Running CRC-32 over a byte range; start with 0xFFFFFFFF and invert the final value, per the PNG spec. */
static uint32_t Crc32_Update(uint32_t crc, const uint8_t * data, size_t length)
{
    size_t byte_index;

    if(crc32_table_ready == 0)
    {
        Build_Crc32_Table();
    }
    for(byte_index = 0; byte_index < length; byte_index++)
    {
        crc = crc32_table[(crc ^ data[byte_index]) & 0xFFu] ^ (crc >> 8);
    }
    return crc;
}

static uint32_t Adler32(const uint8_t * data, size_t length)
{
    uint32_t sum_a = 1;
    uint32_t sum_b = 0;
    size_t byte_index;

    for(byte_index = 0; byte_index < length; byte_index++)
    {
        sum_a = (sum_a + data[byte_index]) % ADLER32_MODULUS;
        sum_b = (sum_b + sum_a) % ADLER32_MODULUS;
    }
    return (sum_b << 16) | sum_a;
}

static void Put_U32_BE(uint8_t * buffer, uint32_t value)
{
    buffer[0] = (uint8_t)(value >> 24);
    buffer[1] = (uint8_t)(value >> 16);
    buffer[2] = (uint8_t)(value >> 8);
    buffer[3] = (uint8_t)(value);
}

static int Write_Png_Chunk(FILE * file_ptr, const char * chunk_type, const uint8_t * data, uint32_t length)
{
    uint8_t length_bytes[4];
    uint8_t crc_bytes[4];
    uint32_t crc;

    Put_U32_BE(length_bytes, length);
    crc = Crc32_Update(0xFFFFFFFFu, (const uint8_t *)chunk_type, 4);
    if(length > 0)
    {
        crc = Crc32_Update(crc, data, (size_t)length);
    }
    Put_U32_BE(crc_bytes, crc ^ 0xFFFFFFFFu);

    if(fwrite(length_bytes, 1, 4, file_ptr) != 4) { return 0; }
    if(fwrite(chunk_type, 1, 4, file_ptr) != 4) { return 0; }
    if((length > 0) && (fwrite(data, 1, (size_t)length, file_ptr) != (size_t)length)) { return 0; }
    if(fwrite(crc_bytes, 1, 4, file_ptr) != 4) { return 0; }
    return 1;
}

/* Wraps raw bytes in a zlib stream of stored deflate blocks.  Caller frees the result. */
static uint8_t * Build_Zlib_Stored(const uint8_t * raw, size_t raw_length, size_t * out_length)
{
    size_t block_count;
    size_t zlib_length;
    uint8_t * zlib;
    size_t write_pos;
    size_t read_pos;
    size_t block_length;
    uint32_t adler;

    block_count = (raw_length + ZLIB_STORED_BLOCK_MAX - 1) / ZLIB_STORED_BLOCK_MAX;
    if(block_count == 0)
    {
        block_count = 1;
    }
    zlib_length = 2 + (block_count * 5) + raw_length + 4;
    zlib = (uint8_t *)malloc(zlib_length);
    if(zlib == NULL)
    {
        return NULL;
    }

    zlib[0] = 0x78;  /* CMF: deflate, 32K window */
    zlib[1] = 0x01;  /* FLG: no dictionary, check bits make (CMF * 256 + FLG) % 31 == 0 */
    write_pos = 2;
    read_pos = 0;
    do
    {
        block_length = raw_length - read_pos;
        if(block_length > ZLIB_STORED_BLOCK_MAX)
        {
            block_length = ZLIB_STORED_BLOCK_MAX;
        }
        zlib[write_pos++] = (uint8_t)(((read_pos + block_length) >= raw_length) ? 0x01 : 0x00);  /* BFINAL, BTYPE = 00 stored */
        zlib[write_pos++] = (uint8_t)(block_length & 0xFFu);
        zlib[write_pos++] = (uint8_t)((block_length >> 8) & 0xFFu);
        zlib[write_pos++] = (uint8_t)(~block_length & 0xFFu);
        zlib[write_pos++] = (uint8_t)((~block_length >> 8) & 0xFFu);
        memcpy(zlib + write_pos, raw + read_pos, block_length);
        write_pos += block_length;
        read_pos += block_length;
    } while(read_pos < raw_length);

    adler = Adler32(raw, raw_length);
    Put_U32_BE(zlib + write_pos, adler);
    write_pos += 4;

    *out_length = write_pos;
    return zlib;
}

/*
    Writes an 8-bit PNG.  color_type PNG_COLOR_TYPE_INDEXED: pixels are 1 byte (palette index), plte_rgb is 256 x RGB, trns/trns_count optional.  color_type PNG_COLOR_TYPE_RGBA: pixels are 4 bytes.
*/
static int Write_Png(const char * path, int width, int height, int color_type, const uint8_t * pixels, const uint8_t * plte_rgb, const uint8_t * trns, int trns_count)
{
    static const uint8_t png_signature[8] = { 0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A };
    FILE * file_ptr;
    uint8_t ihdr[13];
    int bytes_per_pixel;
    size_t row_bytes;
    size_t raw_length;
    uint8_t * raw;
    uint8_t * zlib;
    size_t zlib_length;
    int row;
    int ok;

    bytes_per_pixel = (color_type == PNG_COLOR_TYPE_RGBA) ? 4 : 1;
    row_bytes = (size_t)width * (size_t)bytes_per_pixel;
    raw_length = (size_t)height * (1 + row_bytes);
    raw = (uint8_t *)malloc(raw_length);
    if(raw == NULL)
    {
        fprintf(stderr, "lbx_image_export: out of memory building scanlines for '%s' (%lu bytes)\n", path, (unsigned long)raw_length);
        return 0;
    }
    for(row = 0; row < height; row++)
    {
        raw[(size_t)row * (1 + row_bytes)] = 0;  /* filter type 0: None */
        memcpy(raw + ((size_t)row * (1 + row_bytes)) + 1, pixels + ((size_t)row * row_bytes), row_bytes);
    }

    zlib = Build_Zlib_Stored(raw, raw_length, &zlib_length);
    free(raw);
    if(zlib == NULL)
    {
        fprintf(stderr, "lbx_image_export: out of memory building the zlib stream for '%s'\n", path);
        return 0;
    }

    Put_U32_BE(ihdr + 0, (uint32_t)width);
    Put_U32_BE(ihdr + 4, (uint32_t)height);
    ihdr[8] = 8;                     /* bit depth */
    ihdr[9] = (uint8_t)color_type;
    ihdr[10] = 0;                    /* compression: deflate */
    ihdr[11] = 0;                    /* filter method 0 */
    ihdr[12] = 0;                    /* no interlace */

    file_ptr = fopen(path, "wb");
    if(file_ptr == NULL)
    {
        fprintf(stderr, "lbx_image_export: cannot open '%s' for writing (does the output directory exist?)\n", path);
        free(zlib);
        return 0;
    }

    ok = (fwrite(png_signature, 1, sizeof(png_signature), file_ptr) == sizeof(png_signature));
    ok = ok && Write_Png_Chunk(file_ptr, "IHDR", ihdr, (uint32_t)sizeof(ihdr));
    if(color_type == PNG_COLOR_TYPE_INDEXED)
    {
        ok = ok && Write_Png_Chunk(file_ptr, "PLTE", plte_rgb, (uint32_t)PALETTE_BYTES);
        if((trns != NULL) && (trns_count > 0))
        {
            ok = ok && Write_Png_Chunk(file_ptr, "tRNS", trns, (uint32_t)trns_count);
        }
    }
    ok = ok && Write_Png_Chunk(file_ptr, "IDAT", zlib, (uint32_t)zlib_length);
    ok = ok && Write_Png_Chunk(file_ptr, "IEND", NULL, 0);
    free(zlib);

    if(fclose(file_ptr) != 0)
    {
        ok = 0;
    }
    if(ok == 0)
    {
        fprintf(stderr, "lbx_image_export: write error on '%s'\n", path);
    }
    return ok;
}



/* ========================================================================= */
/*  Entry export                                                             */
/* ========================================================================= */

/* "assets/LOAD.LBX" -> "LOAD"; "LOAD" -> "LOAD".  The stem is what LBX_Load() takes and what output files are named after. */
static void Archive_Stem(const char * path, char * out, size_t out_size)
{
    const char * base;
    const char * cursor;
    const char * dot;
    size_t length;

    base = path;
    for(cursor = path; *cursor != '\0'; cursor++)
    {
        if((*cursor == '/') || (*cursor == '\\'))
        {
            base = cursor + 1;
        }
    }
    dot = strrchr(base, '.');
    length = (dot != NULL) ? (size_t)(dot - base) : strlen(base);
    if(length >= out_size)
    {
        length = out_size - 1;
    }
    memcpy(out, base, length);
    out[length] = '\0';
}

static void Join_Output_Path(char * out, size_t out_size, const char * out_dir, const char * stem, int entry_num, const char * kind)
{
    if((out_dir != NULL) && (out_dir[0] != '\0'))
    {
        snprintf(out, out_size, "%s/%s_%03d_%s.png", out_dir, stem, entry_num, kind);
    }
    else
    {
        snprintf(out, out_size, "%s_%03d_%s.png", stem, entry_num, kind);
    }
}

/* Draws frame 0 of the picture onto a page pre-filled with fill_value, using the engine's own FLIC_Draw(). */
static void Draw_Frame_Zero_Onto(uint8_t * page, uint8_t fill_value, SAMB_ptr picture, const uint8_t * base_palette)
{
    memset(page, fill_value, PAGE_PIXEL_COUNT);
    memcpy(current_palette, base_palette, PALETTE_BYTES);
    current_video_page = page;
    FLIC_SET_CURRENT_FRAME(picture, 0);
    FLIC_Draw(0, 0, picture);
    FLIC_SET_CURRENT_FRAME(picture, 0);
}

static int Export_Entry(const char * stem, int entry_num, const uint8_t * base_palette, const char * out_dir)
{
    SAMB_ptr picture;
    uint32_t entry_length;
    int16_t width;
    int16_t height;
    int16_t frame_count;
    int16_t palette_header_offset;
    uint8_t remap_flag;
    uint32_t frame_zero_offset;
    uint8_t * page_a;
    uint8_t * page_b;
    uint8_t * indexed_pixels;
    uint8_t * rgba_pixels;
    uint8_t plte_rgb[PALETTE_BYTES];
    uint8_t trns_index_zero[1];
    int trns_count;
    long opaque_count;
    long transparent_count;
    long opaque_index_zero_count;
    long out_of_box_count;
    int x;
    int y;
    int color_index;
    char indexed_path[MAX_PATH_LEN];
    char rgba_path[MAX_PATH_LEN];
    char own_palette_text[32];
    int ok;

    picture = LBX_Load((char *)stem, (int16_t)entry_num);
    entry_length = lbxload_entry_length;
    if((picture == NULL) || (entry_length < (uint32_t)(FLIC_HDR_POS_FRAME_OFFSET_TABLE + 4)))
    {
        fprintf(stderr, "lbx_image_export: %s entry %d is %lu bytes, too short to hold a FLIC header plus one frame offset\n", stem, entry_num, (unsigned long)entry_length);
        return 0;
    }

    width = FLIC_GET_WIDTH(picture);
    height = FLIC_GET_HEIGHT(picture);
    frame_count = FLIC_GET_FRAME_COUNT(picture);
    palette_header_offset = FLIC_GET_PALETTE_HEADER_OFFSET(picture);
    remap_flag = FLIC_GET_REMAP_FLAG(picture);
    frame_zero_offset = FLIC_GET_FRAME_OFFSET(picture, 0);

    if((width < 1) || (width > SCREEN_WIDTH) || (height < 1) || (height > SCREEN_HEIGHT))
    {
        fprintf(stderr, "lbx_image_export: %s entry %d is %dx%d; only 1x1..%dx%d is supported (FLIC_Draw() asserts outside the screen)\n", stem, entry_num, width, height, SCREEN_WIDTH, SCREEN_HEIGHT);
        return 0;
    }
    if(frame_count < 1)
    {
        fprintf(stderr, "lbx_image_export: %s entry %d declares frame_count %d; not a picture entry\n", stem, entry_num, frame_count);
        return 0;
    }
    if(remap_flag != 0)
    {
        fprintf(stderr, "lbx_image_export: %s entry %d has the remap flag set (0x%02X); remap entries draw through FLIC_Remap_Draw_Frame() and are deferred to Phase 1b\n", stem, entry_num, remap_flag);
        return 0;
    }
    if((frame_zero_offset + 1) >= entry_length)
    {
        fprintf(stderr, "lbx_image_export: %s entry %d frame 0 offset 0x%lX lies outside the %lu-byte entry\n", stem, entry_num, (unsigned long)frame_zero_offset, (unsigned long)entry_length);
        return 0;
    }

    page_a = (uint8_t *)malloc(PAGE_PIXEL_COUNT);
    page_b = (uint8_t *)malloc(PAGE_PIXEL_COUNT);
    indexed_pixels = (uint8_t *)malloc((size_t)width * (size_t)height);
    rgba_pixels = (uint8_t *)malloc((size_t)width * (size_t)height * 4);
    if((page_a == NULL) || (page_b == NULL) || (indexed_pixels == NULL) || (rgba_pixels == NULL))
    {
        fprintf(stderr, "lbx_image_export: out of memory exporting %s entry %d (%dx%d)\n", stem, entry_num, width, height);
        free(page_a);
        free(page_b);
        free(indexed_pixels);
        free(rgba_pixels);
        return 0;
    }

    Draw_Frame_Zero_Onto(page_a, PAGE_FILL_A, picture, base_palette);
    Draw_Frame_Zero_Onto(page_b, PAGE_FILL_B, picture, base_palette);

    /* current_palette now holds the base palette with the entry's own range overlaid -- the palette this picture is shown under */
    for(color_index = 0; color_index < PALETTE_COLOR_COUNT; color_index++)
    {
        plte_rgb[(color_index * 3) + 0] = (uint8_t)(current_palette[(color_index * 3) + 0] << VGA_TO_8BIT_SHIFT);
        plte_rgb[(color_index * 3) + 1] = (uint8_t)(current_palette[(color_index * 3) + 1] << VGA_TO_8BIT_SHIFT);
        plte_rgb[(color_index * 3) + 2] = (uint8_t)(current_palette[(color_index * 3) + 2] << VGA_TO_8BIT_SHIFT);
    }

    opaque_count = 0;
    transparent_count = 0;
    opaque_index_zero_count = 0;
    out_of_box_count = 0;
    for(y = 0; y < SCREEN_HEIGHT; y++)
    {
        for(x = 0; x < SCREEN_WIDTH; x++)
        {
            int page_pos = (y * SCREEN_WIDTH) + x;
            int inside_box = ((x < width) && (y < height));

            if(inside_box == 0)
            {
                if((page_a[page_pos] != PAGE_FILL_A) || (page_b[page_pos] != PAGE_FILL_B))
                {
                    out_of_box_count++;
                }
                continue;
            }

            {
                int image_pos = (y * width) + x;
                uint8_t * rgba = rgba_pixels + ((size_t)image_pos * 4);

                if(page_a[page_pos] == page_b[page_pos])
                {
                    color_index = page_a[page_pos];
                    indexed_pixels[image_pos] = (uint8_t)color_index;
                    rgba[0] = plte_rgb[(color_index * 3) + 0];
                    rgba[1] = plte_rgb[(color_index * 3) + 1];
                    rgba[2] = plte_rgb[(color_index * 3) + 2];
                    rgba[3] = 0xFF;
                    opaque_count++;
                    if(color_index == ST_TRANSPARENT)
                    {
                        opaque_index_zero_count++;
                    }
                }
                else
                {
                    indexed_pixels[image_pos] = ST_TRANSPARENT;
                    rgba[0] = 0;
                    rgba[1] = 0;
                    rgba[2] = 0;
                    rgba[3] = 0;
                    transparent_count++;
                }
            }
        }
    }

    trns_count = 0;
    if(transparent_count > 0)
    {
        if(opaque_index_zero_count == 0)
        {
            trns_index_zero[0] = 0;  /* index ST_TRANSPARENT fully transparent; indices 1..255 default to opaque */
            trns_count = 1;
        }
        else
        {
            fprintf(stderr, "lbx_image_export: %s entry %d: transparency not representable in the indexed PNG -- index %d is also drawn opaque %ld time(s); the _rgba.png carries the alpha\n", stem, entry_num, ST_TRANSPARENT, opaque_index_zero_count);
        }
    }

    Join_Output_Path(indexed_path, sizeof(indexed_path), out_dir, stem, entry_num, "indexed");
    Join_Output_Path(rgba_path, sizeof(rgba_path), out_dir, stem, entry_num, "rgba");
    ok = Write_Png(indexed_path, width, height, PNG_COLOR_TYPE_INDEXED, indexed_pixels, plte_rgb, trns_index_zero, trns_count);
    ok = Write_Png(rgba_path, width, height, PNG_COLOR_TYPE_RGBA, rgba_pixels, NULL, NULL, 0) && ok;

    if(palette_header_offset != 0)
    {
        int palette_start = FLIC_GET_PALETTE_COLOR_START(picture);
        int palette_count = FLIC_GET_PALETTE_COLOR_COUNT(picture);
        snprintf(own_palette_text, sizeof(own_palette_text), "%d..%d", palette_start, (palette_start + palette_count) - 1);
    }
    else
    {
        snprintf(own_palette_text, sizeof(own_palette_text), "none");
    }

    printf("%s %d: %dx%d frames=%d own_palette=%s opaque=%ld transparent=%ld opaque_index0=%ld out_of_box=%ld%s -> %s, %s\n", stem, entry_num, width, height, frame_count, own_palette_text, opaque_count, transparent_count, opaque_index_zero_count, out_of_box_count, (frame_count > 1) ? " (frame 0 only)" : "", indexed_path, rgba_path);

    if(out_of_box_count > 0)
    {
        fprintf(stderr, "lbx_image_export: %s entry %d: FLIC_Draw() wrote %ld pixel(s) outside the declared %dx%d box -- decode/data error\n", stem, entry_num, out_of_box_count, width, height);
        ok = 0;
    }

    free(page_a);
    free(page_b);
    free(indexed_pixels);
    free(rgba_pixels);
    return ok;
}



/* ========================================================================= */
/*  main                                                                     */
/* ========================================================================= */

static void Print_Usage(const char * argv0)
{
    fprintf(stderr, "usage: %s [-d DATA_DIR] [-o OUT_DIR] [-p PALETTE] ARCHIVE ENTRY [ENTRY ...]\n", argv0);
    fprintf(stderr, "\n");
    fprintf(stderr, "  ARCHIVE         archive stem or file name, e.g. LOAD or LOAD.LBX, looked up in DATA_DIR\n");
    fprintf(stderr, "  ENTRY           entry index (frame 0 is exported)\n");
    fprintf(stderr, "  -d, --data-dir  directory holding the .LBX files (default: current directory)\n");
    fprintf(stderr, "  -o, --outdir    write PNGs here (default: current directory; must already exist)\n");
    fprintf(stderr, "  -p, --palette   base palette N = FONTS.LBX entry N+2, as Load_Palette() reads it (default 0, the main game palette);\n");
    fprintf(stderr, "                  an entry's own palette range is overlaid on top, as FLIC_Draw() does\n");
    fprintf(stderr, "  -P, --palette-from ARCHIVE:ENTRY\n");
    fprintf(stderr, "                  also overlay another entry's own palette onto the base first, for entries the game only draws\n");
    fprintf(stderr, "                  after a palette-carrying sibling (e.g. MAINSCRN 5 after MAINSCRN 0: -P MAINSCRN:0)\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "  Writes <STEM>_<NNN>_indexed.png (palette indices + palette) and <STEM>_<NNN>_rgba.png per entry.\n");
}

int main(int argc, char ** argv)
{
    const char * data_dir = NULL;
    const char * out_dir = NULL;
    const char * archive_arg = NULL;
    const char * palette_from_arg = NULL;
    int base_palette_num = 0;
    int first_entry_arg = 0;
    int arg_index;
    char stem[MAX_STEM_LEN];
    SAMB_ptr palette_entry;
    uint8_t base_palette[PALETTE_BYTES];
    int failures = 0;

    for(arg_index = 1; arg_index < argc; arg_index++)
    {
        const char * arg = argv[arg_index];

        if(((strcmp(arg, "-d") == 0) || (strcmp(arg, "--data-dir") == 0)) && ((arg_index + 1) < argc))
        {
            data_dir = argv[++arg_index];
        }
        else if(((strcmp(arg, "-o") == 0) || (strcmp(arg, "--outdir") == 0)) && ((arg_index + 1) < argc))
        {
            out_dir = argv[++arg_index];
        }
        else if(((strcmp(arg, "-p") == 0) || (strcmp(arg, "--palette") == 0)) && ((arg_index + 1) < argc))
        {
            base_palette_num = atoi(argv[++arg_index]);
        }
        else if(((strcmp(arg, "-P") == 0) || (strcmp(arg, "--palette-from") == 0)) && ((arg_index + 1) < argc))
        {
            palette_from_arg = argv[++arg_index];
        }
        else if((arg[0] == '-') && (arg[1] != '\0'))
        {
            fprintf(stderr, "lbx_image_export: unknown option or missing value: '%s'\n", arg);
            Print_Usage(argv[0]);
            return 2;
        }
        else if(archive_arg == NULL)
        {
            archive_arg = arg;
        }
        else
        {
            first_entry_arg = arg_index;
            break;
        }
    }

    if((archive_arg == NULL) || (first_entry_arg == 0))
    {
        Print_Usage(argv[0]);
        return 2;
    }
    if(base_palette_num < 0)
    {
        fprintf(stderr, "lbx_image_export: palette number must be >= 0 (got %d)\n", base_palette_num);
        return 2;
    }

    if(data_dir != NULL)
    {
        STU_GRAF_Reset();
        STU_GRAF_Add_Search_Dir(data_dir);
    }

    /* FLIC_Load_Palette() writes current_palette[0..767] and the change flags at [768..1023]; the game allocates this block in Fonts.c, so a standalone tool provides its own. */
    current_palette = (uint8_t *)calloc(1, PALETTE_BLOCK_BYTES);
    if(current_palette == NULL)
    {
        fprintf(stderr, "lbx_image_export: out of memory for the palette block\n");
        return 1;
    }

    palette_entry = LBX_Load("FONTS", (int16_t)(base_palette_num + FONTS_PALETTE_ENTRY_BASE));
    if((palette_entry == NULL) || (lbxload_entry_length < PALETTE_BYTES))
    {
        fprintf(stderr, "lbx_image_export: FONTS.LBX entry %d (palette %d) is %lu bytes, smaller than a %d-byte palette\n", base_palette_num + FONTS_PALETTE_ENTRY_BASE, base_palette_num, (unsigned long)lbxload_entry_length, PALETTE_BYTES);
        return 1;
    }
    memcpy(base_palette, palette_entry, PALETTE_BYTES);

    /* Some entries carry no palette and are only ever drawn after a sibling that does (MAINSCRN 5 follows MAINSCRN 0 on the main menu); overlay that sibling's palette the way FLIC_Draw() would. */
    if(palette_from_arg != NULL)
    {
        char palette_from_archive[MAX_PATH_LEN];
        char palette_from_stem[MAX_STEM_LEN];
        const char * colon;
        long palette_from_entry;
        char * end_ptr = NULL;
        SAMB_ptr palette_picture;

        colon = strrchr(palette_from_arg, ':');
        if((colon == NULL) || (colon == palette_from_arg) || ((size_t)(colon - palette_from_arg) >= sizeof(palette_from_archive)))
        {
            fprintf(stderr, "lbx_image_export: --palette-from expects ARCHIVE:ENTRY, got '%s'\n", palette_from_arg);
            return 2;
        }
        memcpy(palette_from_archive, palette_from_arg, (size_t)(colon - palette_from_arg));
        palette_from_archive[colon - palette_from_arg] = '\0';
        Archive_Stem(palette_from_archive, palette_from_stem, sizeof(palette_from_stem));
        palette_from_entry = strtol(colon + 1, &end_ptr, 10);
        if((end_ptr == (colon + 1)) || (*end_ptr != '\0') || (palette_from_entry < 0) || (palette_from_entry > 32767))
        {
            fprintf(stderr, "lbx_image_export: --palette-from entry '%s' is not an entry number\n", colon + 1);
            return 2;
        }
        palette_picture = LBX_Load(palette_from_stem, (int16_t)palette_from_entry);
        if((palette_picture == NULL) || (lbxload_entry_length < (uint32_t)SZ_FLIC_HDR) || (FLIC_GET_PALETTE_HEADER_OFFSET(palette_picture) == 0))
        {
            fprintf(stderr, "lbx_image_export: --palette-from %s entry %ld carries no palette (%lu bytes, palette_header_offset %d)\n", palette_from_stem, palette_from_entry, (unsigned long)lbxload_entry_length, (palette_picture != NULL) ? FLIC_GET_PALETTE_HEADER_OFFSET(palette_picture) : 0);
            return 1;
        }
        memcpy(current_palette, base_palette, PALETTE_BYTES);
        FLIC_Load_Palette(palette_picture, 0);
        memcpy(base_palette, current_palette, PALETTE_BYTES);
    }

    Archive_Stem(archive_arg, stem, sizeof(stem));

    for(arg_index = first_entry_arg; arg_index < argc; arg_index++)
    {
        char * end_ptr = NULL;
        long entry_num;

        entry_num = strtol(argv[arg_index], &end_ptr, 10);
        if((end_ptr == argv[arg_index]) || (*end_ptr != '\0') || (entry_num < 0) || (entry_num > 32767))
        {
            fprintf(stderr, "lbx_image_export: '%s' is not an entry number\n", argv[arg_index]);
            failures++;
            continue;
        }
        if(Export_Entry(stem, (int)entry_num, base_palette, out_dir) == 0)
        {
            failures++;
        }
    }

    if(failures > 0)
    {
        fprintf(stderr, "lbx_image_export: %d entry/entries failed\n", failures);
        return 1;
    }
    return 0;
}
