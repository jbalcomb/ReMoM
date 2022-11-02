
#include "MoX_TYPE.H"       /* byte_ptr, SAMB_ptr */
#include "MoX_DEF.H"        /* PTR_ADD_PARAGRAPH() */

#include "MoX_Fonts.H"
#include "MoX_Palette.H"    /* Set_Palette_Changes() */
#include "MoX_LBX.H"        /* Farload() */
#include "MoX_SA.H"         /* Allocate_Space() */

#include <stdio.h>          /* printf() */
#include <string.h>         /* strcpy() */

char font_name[16];
SAMB_ptr font_ptr;
SAMB_ptr border_ptr;
SAMB_ptr palette_block;
SAMB_ptr p_Palette;
byte_ptr p_PaletteFlags;

SAMB_ptr palette_data;
// byte_ptr p_Palette_Font_Colors;
// byte_ptr UU_p_Palette_Data;
// byte_ptr p_Cursor_Array;
// byte_ptr p_ShadingColors;


// s14p03
/*
    Initialize Font, Palette, Text
*/
void Load_Font_File(char * font_file)
{
    int i;

    printf("BEGIN: Load_Font_File()\n");

    strcpy(font_name, font_file);
    font_ptr = Farload(font_file, 0);
    border_ptr = Farload(font_file, 1);
    palette_block = Allocate_Space(348);                // 348 paragraphs = 386 * 16 bytes = 5568 bytes
    p_Palette = Allocate_Space(64);                     //  64 paragraphs =  64 * 16 bytes = 1024 bytes
    p_PaletteFlags = PTR_ADD_PARAGRAPH(p_Palette, 48);  // ~== p_PaletteFlags = &p_Palette[768];
    // UU_p_PaletteSaved      = Allocate_Space(48);             //  48 paragraphs =  48 * 16 bytes =  768 bytes
    // p_ReplacementColors    = Allocate_Space(384);            // 348 paragraphs = 384 * 16 bytes = 6144 bytes / 256 = 24 & 256 - 24 - 232 ? shading colors in _R functions ?
    // p_VGAFILEH_Header      = Allocate_Space(2);              //   2 paragraphs =   2 * 16 bytes =   32 bytes
    // p_IntensityScaleTable  = Allocate_Space(96);             //  96 paragraphs =  96 * 16 bytes = 1536 bytes / 256 = 6
    // TextLine_Init();
    for ( i = 0; i < 768; i++)
    {
        *(p_Palette + i) = 0;
    }
    for ( i = 0; i < 256; i++)
    {
        *(p_PaletteFlags + i) = 0;
    }

    printf("END: Load_Font_File()\n");
}

// MGC  s20p01   VGA_LoadPalette
// 1oom :: lbxpal.c :: void lbxpal_select(int pal_index, int first/*or -1*/, int last)
// MoO2 Module: fonts Load_Palette
void Load_Palette(int entry, int start_color, int end_color)
{
    int Color_Index;
    int Color_Count;
    byte_ptr p_Dst;
    byte_ptr p_Src;
    int i;

    printf("BEGIN: Load_Palette()\n");

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Load_Palette(entry = %d, start_color = %d, end_color = %d)\n", __FILE__, __LINE__, entry, start_color, end_color);
#endif

    // palette_data = Far_Reload(file_name = font_name, entry_num = entry+1, base_seg = palette_block)
    // palette_data  = (SAMB_ptr)MK_FP(LBXE_LoadReplace(font_name, entry+2,(SAMB_addr)FP_SEG(palette_block)),0);
    palette_data  = Far_Reload(font_name, entry+2, palette_block);
    //                                                                          //  48 pr  768 b  0x0000 [  0] 3*256 palette / color-map
    // p_Palette_Font_Colors = PTR_ADD_PARAGRAPH(palette_data, 48);                //  48 pr  768 b  0x0300 [768] 16 arrays of 16 colors
    // UU_p_Palette_Data = PTR_ADD_PARAGRAPH(palette_data, (48 + 16));             //  16 pr  256 b  0x0400 [1024] ? UnUsed ? DNE in MoO1 ?
    // p_Cursor_Array = PTR_ADD_PARAGRAPH(palette_data, (48 + 16 + 16));           //  16 pr  256 b  0x0500 [1280] 16 16x16 cursor bitmap images
    // p_ShadingColors = PTR_ADD_PARAGRAPH(palette_data, (48 + 16 + 16 + 256));    // 256 pr 4096 b  0x1500  5,376 byte array of 24 color fractions (B-G-R-Percent)

    if ( start_color == -1 )
    {
        Color_Index = 0;
        Color_Count = 256;
    }
    else
    {
        Color_Index = start_color;
        Color_Count = (end_color - start_color) + 1;
    }

    // p_Dst = &p_Palette[(Color_Index * 3)];
    // p_Src = &palette_data[(Color_Index * 3)];  // Warning: Nonportable pointer conversion
    for(i = 0; i < (Color_Count * 3); i++)
    {
        // *p_Dst++ = *p_Src++;
        *(p_Palette + (Color_Index * 3) + i) = *(palette_data + (Color_Index * 3) + i);
    }

    // VGA_SetFont(0, 0, 0, 0);

    if ( start_color == -1 )
    {
        Set_Palette_Changes(0, 255);
    }
    else
    {
        Set_Palette_Changes(start_color, end_color);
    }

    printf("END: Load_Palette()\n");

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Load_Palette(entry = %d, start_color = %d, end_color = %d)\n", __FILE__, __LINE__, entry, start_color, end_color);
#endif

}
