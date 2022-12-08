
#include "MoX_TYPE.H"
#include "MoX_DEF.H"

#include "Fonts.H"

#include "LBX_Load.H"
#include "Allocate.H"

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



void Load_Font_File(char * font_file)
{
    int itr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Load_Font_File(font_file = %s)\n", __FILE__, __LINE__, font_file);
#endif

    strcpy(font_name, font_file);

    font_style_data = LBX_Load(font_file, 0);
    border_style_data = LBX_Load(font_file, 1);

    palette_block          = Allocate_Space(348);    // 348 paragraphs = 386 * 16 bytes = 5568 bytes
    p_Palette              = Allocate_Space(64);     //  64 paragraphs =  64 * 16 bytes = 1024 bytes
    p_PaletteFlags         = p_Palette + (48 * 16);  // ~== p_PaletteFlags = &p_Palette[768];
    p_Palette_XBGR         = Allocate_Space(64);     // STU/Win32

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
