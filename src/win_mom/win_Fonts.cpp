
#include "win_TYPE.hpp"    /* SAMB_ptr */
#include "win_DEF.hpp"     /* ST_TRUE, ST_UNDEFINED */

#include "win_Fonts.hpp"

#include "win_LBX.hpp"     /* LBX_Load(), LBX_Reload() */
#include "win_SA.hpp"      /* Allocate_Space() */

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif

#include <string.h>         /* strcpy() */



char font_name[16];                             // MGC dseg:A7E0    set in Load_Font_File
SAMB_ptr font_style_data;                       // MGC dseg:A816    alloc & load in Load_Font_File
SAMB_ptr border_style_data;                     // MGC dseg:A818    alloc & load in Load_Font_File

SAMB_ptr palette_block;                         // ? seg020  // dseg:A7F4 ?
SAMB_ptr palette_data;                          // ? seg020  // dseg:A7F2 ?

byte_ptr p_Palette;                             // MGC dseg:A7DE    alloc in Load_Font_File()
byte_ptr p_PaletteFlags;                        // MGC dseg:A7D6    alloc in Load_Font_File()
byte_ptr p_Palette_XBGR;                        // STU/Win32

// SAMB_ptr UU_p_PaletteSaved;                     // MGC dseg:A7DC    alloc in Load_Font_File()
// SAMB_ptr p_ReplacementColors;                   // MGC dseg:A7DA    alloc in Load_Font_File()
// SAMB_ptr p_VGAFILEH_Header;                     // MGC dseg:A7D4    alloc in Load_Font_File()
// SAMB_ptr p_IntensityScaleTable;                 // MGC dseg:A7F0    alloc in Load_Font_File()

// byte_ptr p_ShadingColors;                       // dseg:A7D8    5,376 bytes (1500h) into the palette entry
// byte_ptr p_Cursor_Array;                        // dseg:A81A  ; 500h into the palette entry
// byte_ptr UU_p_Palette_Data;                     // dseg:A81C  ; 400h into the palette entry
// byte_ptr p_Palette_Font_Colors;                 // dseg:A81E  ; 300h into the palette entry, 16 arrays of 16 colors


void Load_Font_File(char * font_file);
void Load_Palette(int entry, int start_color, int end_color);
void Set_Palette_Changes(int start_color, int end_color);


void Load_Font_File(char * font_file)
{
    int itr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Load_Font_File(font_file = %s)\n", __FILE__, __LINE__, font_file);
#endif

    strcpy(font_name, font_file);

    font_style_data = LBX_Load(font_file, 0);
    border_style_data = LBX_Load(font_file, 1);

    /* MoO2 palette_block */
    palette_block          = Allocate_Space(348);    // 348 paragraphs = 386 * 16 bytes = 5568 bytes
    p_Palette              = Allocate_Space(64);     //  64 paragraphs =  64 * 16 bytes = 1024 bytes
    p_PaletteFlags         = p_Palette + (48 * 16);  // ~== p_PaletteFlags = &p_Palette[768];
    p_Palette_XBGR         = Allocate_Space(64);     // STU/Win32
    // UU_p_PaletteSaved      = Allocate_Space(48);     //  48 paragraphs =  48 * 16 bytes =  768 bytes
    // p_ReplacementColors    = Allocate_Space(384);    // 348 paragraphs = 384 * 16 bytes = 6144 bytes / 256 = 24 & 256 - 24 - 232 ? shading colors in _R functions ?
    // p_VGAFILEH_Header      = Allocate_Space(2);      //   2 paragraphs =   2 * 16 bytes =   32 bytes
    // p_IntensityScaleTable  = Allocate_Space(96);     //  96 paragraphs =  96 * 16 bytes = 1536 bytes / 256 = 6
    
    // TextLine_Init();

    for(itr = 0; itr < 768; itr++)
    {
        *(p_Palette + itr) = 0;
    }
    for(itr = 0; itr < 256; itr++)
    {
        *(p_PaletteFlags + itr) = 1;
    }
    for(itr = 0; itr < 1024; itr++)
    {
        *(p_Palette_XBGR + itr) = 0;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Load_Font_File(font_file = %s)\n", __FILE__, __LINE__, font_file);
#endif

}



/*
    MAGIC.EXE  seg020
*/

// MGC s20p01
void Load_Palette(int entry, int start_color, int end_color)
{
    int color_start;
    int color_count;
    int itr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Load_Palette(entry = %d, start_color = %d, end_color = %d)\n", __FILE__, __LINE__, entry, start_color, end_color);
#endif

    palette_data = LBX_Reload(font_name, entry+2, palette_block);

    // p_Palette_Font_Colors = palette_data + ( (12 * 1) + (16 * (48           - 1)) );
    // UU_p_Palette_Data     = palette_data + ( (12 * 1) + (16 * (48+16        - 1)) );
    // p_Cursor_Array        = palette_data + ( (12 * 1) + (16 * (48+16+16     - 1)) );
    // p_ShadingColors       = palette_data + ( (12 * 1) + (16 * (48+16+16+256 - 1)) );

    if(start_color == -1)
    {
        color_start = 0;
        color_count = 256;
    }
    else
    {
        color_start = start_color;
        color_count = (end_color - start_color) + 1;
    }

    for(itr = 0; itr < (color_count * 3); itr++)
    {
        *(p_Palette + (color_start * 3) + itr) = *(palette_data + (color_start * 3) + itr);
    }

    for(itr = 0; itr < color_count; itr++)
    {
        *(p_Palette_XBGR + (color_start * 4) + (itr * 4) + 3) = 0x00;
        *(p_Palette_XBGR + (color_start * 4) + (itr * 4) + 2) = (*(palette_data + (color_start * 3) + (itr * 3) + 0) << 2);
        *(p_Palette_XBGR + (color_start * 4) + (itr * 4) + 1) = (*(palette_data + (color_start * 3) + (itr * 3) + 1) << 2);
        *(p_Palette_XBGR + (color_start * 4) + (itr * 4) + 0) = (*(palette_data + (color_start * 3) + (itr * 3) + 2) << 2);
    }

    // Set_Font(0, 0, 0);

    if(start_color == ST_UNDEFINED)
    {
        Set_Palette_Changes(0, 255);
    }
    else
    {
        Set_Palette_Changes(start_color, end_color);
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Load_Palette(entry = %d, start_color = %d, end_color = %d)\n", __FILE__, __LINE__, entry, start_color, end_color);
#endif
}

// MGC s20p05
void Set_Palette_Changes(int start_color, int end_color)
{
    int itr;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Set_Palette_Changes(start_color = %d, end_color = %d)\n", __FILE__, __LINE__, start_color, end_color);
#endif

    for(itr = start_color; itr < end_color; itr++)
    {
        *(p_PaletteFlags + itr) = ST_TRUE;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Set_Palette_Changes(start_color = %d, end_color = %d)\n", __FILE__, __LINE__, start_color, end_color);
#endif
}
