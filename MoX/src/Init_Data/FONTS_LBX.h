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
#define FONTS_LBX_GLYPH_DATA_SIZE  15834

struct s_FONTS_LBX_ENTRY_0
{
    struct s_FONT_HEADER header;                            /* 1946 bytes at offset 0x000 */
    uint8_t              glyph_data[FONTS_LBX_GLYPH_DATA_SIZE];  /* RLE bitmaps at offset 0x79A */
};



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
