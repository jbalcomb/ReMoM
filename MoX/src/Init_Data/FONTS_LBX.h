#ifndef FONTS_LBX_H
#define FONTS_LBX_H

#include "../Fonts.h"
#include "../MOX_TYPE.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif



/*
    FONTS.LBX Entry [0] — Font Style Data
    The first 1946 bytes are the s_FONT_HEADER (metadata for 8 font styles, 96 chars each).
    The remaining bytes are RLE-encoded glyph bitmap data, addressed via data_offsets[]
    as byte offsets from the start of the entire entry.
    Header and glyph data must remain contiguous.
*/
#define FONTS_LBX_GLYPH_DATA_SIZE  15066  /* was 15834 before SZ_FONT_HDR fix (1946 -> 2714); total entry size 17780 unchanged (header 2714 + glyph_data 15066 = 17780) */

struct s_FONTS_LBX_ENTRY_0
{
    struct s_FONT_HEADER header;                            /* 2714 bytes at offset 0x000 */
    uint8_t              glyph_data[FONTS_LBX_GLYPH_DATA_SIZE];  /* RLE bitmaps at offset 0xA9A */
};

/* CLAUDE: Validate the embedded font data has no compiler-inserted padding.
 * The runtime treats the entire entry as a flat byte array and indexes glyph
 * bitmaps via header.data_offsets[] as byte offsets from the start of the
 * entry.  Any padding in s_FONT_HEADER would shift glyph_data and corrupt
 * every glyph lookup — the most likely cause of "garbage rendering" or a
 * crash inside the font printing code.  These typedef'd negative-size arrays
 * fail to compile if the sizes are wrong (works in any C mode). */
typedef char FONTS_LBX_assert_header_size[(sizeof(struct s_FONT_HEADER) == 2714) ? 1 : -1];
typedef char FONTS_LBX_assert_entry0_size[(sizeof(struct s_FONTS_LBX_ENTRY_0) == 2714 + FONTS_LBX_GLYPH_DATA_SIZE) ? 1 : -1];



/*
    FONTS.LBX Entry [2] — Palette Block
    Loaded by Load_Palette() into palette_block via LBX_Reload().
    Contains the VGA palette, font colour sets, mouse cursor images, and
    colour remap parameters.
*/
struct s_FONTS_LBX_PALETTE
{
    uint8_t rgb_palette[768];           /* 0x000: 256 colours x 3 bytes (R,G,B)    */
    uint8_t font_colors[256];           /* 0x300: 16 colour sets x 16 colours      */
    uint8_t palette_data_0x400[256];    /* 0x400: unknown / unused                 */
    uint8_t mouse_palette[4096];        /* 0x500: 16 cursor images x 256 bytes     */
    uint8_t remap_colors[96];           /* 0x1500: 24 blocks x 4 bytes (B,G,R,%)  */
};



/*
    Entry [0]: font glyph / style data
*/
extern const struct s_FONTS_LBX_ENTRY_0  FONTS_LBX_ENTRY_0;
extern const uint32_t                    FONTS_LBX_ENTRY_0_SIZE;

/*
    Entry [1]: border / outline data (structure unknown, raw bytes)
*/
extern const uint8_t                     FONTS_LBX_ENTRY_1[];
extern const uint32_t                    FONTS_LBX_ENTRY_1_SIZE;

/*
    Entry [2]: palette + font_colors + mouse_palette + remap_colors
*/
extern const struct s_FONTS_LBX_PALETTE  FONTS_LBX_ENTRY_2;
extern const uint32_t                    FONTS_LBX_ENTRY_2_SIZE;



#ifdef __cplusplus
}
#endif

#endif /* FONTS_LBX_H */
