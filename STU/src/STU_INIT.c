/*
    STU Init
    This module contains functions for extracting, inspecting, and validating the game data
    that the start-up code (Init_Drivers, Load_Font_File, Load_Palette, etc.) expects to find
    in the original game files.

    The start-up sequence loads from several LBX archives.  The primary ones are:
        FONTS.LBX   - font glyph data, border/outline data, palette + color tables
        (others TBD as they are catalogued)

    This module is a companion to STU_XTRC (general LBX extraction) and STU_FON (font-specific
    extraction and TTF conversion).  STU_INIT focuses on the dependency graph: what data must be
    present and in what order for the game to initialise correctly.

    See also:
        MoM/src/Init.c          - the actual start-up code
        MoX/src/Fonts.c         - Load_Font_File(), Load_Palette()
        doc/STU-MoM-FON_PAL.md  - notes on font/palette data layout
*/

#include "STU_INIT.h"
#include "STU_DBG.h"

#include <stdio.h>



/*
    FONTS.LBX sub-file inventory
    Sub-file 0: font glyph / style data   (font_style_data, loaded by Load_Font_File)
    Sub-file 1: border / outline data      (border_style_data, loaded by Load_Font_File)
    Sub-file 2+: palette entries           (palette_data, loaded by Load_Palette with entry+2)
        Palette data layout at offset 0x000: 768 bytes  RGB palette (256 * 3)
        Palette data layout at offset 0x300: 256 bytes  font_colors
        Palette data layout at offset 0x400: 256 bytes  (unused?)
        Palette data layout at offset 0x500: 4096 bytes mouse_palette
        Palette data layout at offset 0x1500: remap_colors (shading color parameters)
*/
static const LBX_SUBFILE_INFO fonts_lbx_subfiles[] =
{
    { 0, 0, "font glyph / style data (font_style_data)" },
    { 1, 0, "border / outline data (border_style_data)" },
    { 2, 0, "palette entry 0 (default palette + font_colors + mouse_palette + remap_colors)" },
};

static const LBX_ARCHIVE_INFO init_archives[] =
{
    { "FONTS.LBX", 3, fonts_lbx_subfiles },
};

#define INIT_ARCHIVE_COUNT  (sizeof(init_archives) / sizeof(init_archives[0]))



void Dump_Init_Data_Inventory(void)
{
    int i;
    int j;

    fprintf(stdout, "=== Init Data Inventory ===\n");
    for(i = 0; i < (int)INIT_ARCHIVE_COUNT; i++)
    {
        const LBX_ARCHIVE_INFO * archive = &init_archives[i];
        fprintf(stdout, "\n%s  (%d sub-files catalogued)\n", archive->filename, archive->subfile_count);
        for(j = 0; j < archive->subfile_count; j++)
        {
            const LBX_SUBFILE_INFO * sf = &archive->subfiles[j];
            if(sf->size > 0)
            {
                fprintf(stdout, "  [%d]  %d bytes  %s\n", sf->index, sf->size, sf->label);
            }
            else
            {
                fprintf(stdout, "  [%d]  (size TBD)  %s\n", sf->index, sf->label);
            }
        }
    }
    fprintf(stdout, "\n=== End Init Data Inventory ===\n");
}



int Validate_Init_Data(const char * game_data_path)
{
    /* TODO  check that each catalogued LBX file exists at game_data_path */
    /* TODO  open each LBX, verify sub-file count >= expected */
    /* TODO  spot-check known offsets / magic values */

    (void)game_data_path;

    fprintf(stdout, "Validate_Init_Data: not yet implemented\n");
    return -1;
}
