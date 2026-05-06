/*
    STU Font
    This module extracts and works with the bitmap font data stored in FONTS.LBX.

    FONTS.LBX layout (sub-file 0 = font_style_data, cast to s_FONT_HEADER):
        The header contains data for 8 font styles, each with 96 characters (ASCII 32..127).
        Per-style metadata:
            base_height[8]          - baseline offset for each style
            horizontal_spacing[8]   - inter-character pixel spacing
            vertical_spacing[8]     - inter-line pixel spacing
            font_widths[8][96]      - per-character pixel widths
            data_offsets[8][96]     - byte offsets into the glyph bitmap data

        The glyph bitmap data follows the header.  Each glyph is stored as a column-major(?)
        stream of colour-map indices.  A zero byte means transparent.

    Sub-file 1 = border_style_data (outline / shadow pixel patterns).

    Sub-file 2+ = palette entries (see STU_INIT for layout).

    The long-term goal for this module is:
        1. Extract glyph bitmaps  (Dump / Export functions)
        2. Export as BMP for inspection and community use
        3. Eventually generate TrueType (.ttf) fonts from the glyph bitmaps

    See also:
        MoX/src/Fonts.h         - s_FONT_HEADER definition
        MoX/src/Fonts.c         - Load_Font_File(), glyph rendering code
        doc/MoX-Fonts.md        - extensive notes on font colours, aliasing, outlines
        doc/STU-MoM-FON_PAL.md  - font / palette data layout notes
*/

#include "STU_FON.h"
#include "STU_DBG.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*
    Map a colour-map index to a printable character for text-art dumps.
    0 = transparent (space), non-zero values map to density characters.
*/
static char Pixel_To_Char(uint8_t pixel)
{
    if(pixel == 0)
    {
        return '.';
    }
    if(pixel < 4)
    {
        return '+';
    }
    if(pixel < 8)
    {
        return '#';
    }
    return '@';
}



void Dump_Font_Style_Text(int style_index)
{
    /* TODO  requires loading FONTS.LBX sub-file 0 and parsing s_FONT_HEADER */
    /* TODO  iterate the 96 characters, decode glyph data, print text-art */

    fprintf(stdout, "=== Font Style %d ===\n", style_index);
    fprintf(stdout, "(not yet implemented - needs FONTS.LBX loaded)\n");
    fprintf(stdout, "=== End Font Style %d ===\n\n", style_index);
}



void Dump_All_Font_Styles_Text(void)
{
    int i;

    for(i = 0; i < FON_STYLE_COUNT; i++)
    {
        Dump_Font_Style_Text(i);
    }
}



int Export_Font_Style_BMP(int style_index, const char * out_dir)
{
    /* TODO  load FONTS.LBX, extract glyphs, write 8-bit indexed BMP files */
    /* TODO  include palette from sub-file 2 so BMPs display with correct colours */

    (void)style_index;
    (void)out_dir;

    fprintf(stdout, "Export_Font_Style_BMP: not yet implemented\n");
    return -1;
}



int Export_All_Font_Styles_BMP(const char * out_dir)
{
    int i;
    int result;

    for(i = 0; i < FON_STYLE_COUNT; i++)
    {
        result = Export_Font_Style_BMP(i, out_dir);
        if(result != 0)
        {
            return result;
        }
    }
    return 0;
}
