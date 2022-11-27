// MGC seg020
// ST_VGA.H
// MoO2 Module: fonts
// MoO2 Module: palette
// MoO2 Module: palstore
// MoO2 Module: remap

#include "MoX_TYPE.H"
#include "ST_DEF.H"
#include "seg020.H"

#include "ST_LBX.H"
#include "ST_TXT.H"
#include "ST_VGA.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif


SAMB_ptr palette_block;
SAMB_ptr palette_data;


// s20p01   VGA_LoadPalette
// 1oom :: lbxpal.c :: void lbxpal_select(int pal_index, int first/*or -1*/, int last)
// MoO2 Module: fonts Load_Palette
void PAL_Load_Palette(int entry, int start_color, int end_color)
{
    int Color_Index;
    int Color_Count;
    // byte_ptr p_Dst;
    // byte_ptr p_Src;
    int itr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: PAL_Load_Palette(entry = %d, start_color = %d, end_color = %d)\n", __FILE__, __LINE__, entry, start_color, end_color);
#endif

    // palette_data = Far_Reload(file_name = font_name, entry_num = entry+1, base_seg = palette_block)
    // palette_data  = (SAMB_ptr)MK_FP(LBXE_LoadReplace(font_name, entry+2,(SAMB_addr)FP_SEG(palette_block)),0);
    palette_data  = LBXE_LoadReplace_LM(font_name, entry+2, palette_block);

    //                                                                          //  48 pr  768 b  0x0000 [  0] 3*256 palette / color-map
    
    p_Palette_Font_Colors = PTR_ADD_PARAGRAPH(palette_data, 48);                //  48 pr  768 b  0x0300 [768] 16 arrays of 16 colors
    // SM2LM  
    sa_Palette_Font_Colors = FP_SEG(p_Palette_Font_Colors);

    // MoO2  mov eax, [palette_data];  add eax, 1024;  mov mouse_palette, eax
    UU_p_Palette_Data = PTR_ADD_PARAGRAPH(palette_data, (48 + 16));             //  16 pr  256 b  0x0400 [1024] ? UnUsed ? DNE in MoO1 ?
    // SM2LM  
    UU_sa_Palette_Data = FP_SEG(UU_p_Palette_Data);
    
    p_Cursor_Array = PTR_ADD_PARAGRAPH(palette_data, (48 + 16 + 16));           //  16 pr  256 b  0x0500 [1280] 16 16x16 cursor bitmap images
    // SM2LM  
    sa_Cursor_Array = FP_SEG(p_Cursor_Array);
    
    p_ShadingColors = PTR_ADD_PARAGRAPH(palette_data, (48 + 16 + 16 + 256));    // 256 pr 4096 b  0x1500  5,376 byte array of 24 color fractions (B-G-R-Percent)
    // SM2LM  
    sa_ShadingColors = FP_SEG(p_ShadingColors);

    if(start_color == -1)
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
        // *p_Dst++ = *p_Src++;
    for(itr = 0; itr < (Color_Count * 3); itr++)
    {
        *(p_Palette + (Color_Index * 3) + itr) = *(palette_data + (Color_Index * 3) + itr);
    }

    // map 768 to 1024
    // map [256][3] to [256][4]
    // map [256][{R,G,B] to [256][{X,B,G,R}]
    // map 3,R to 3,R ... 3*4,+3 = 3*3,+0
    for(itr = 0; itr < Color_Count; itr++)
    {
        // *(p_Palette_XBGR + (Color_Index * 4) + (itr * 4) + 3) = (*(palette_data + (Color_Index * 3) + (itr * 3) + 0) << 2);
        // *(p_Palette_XBGR + (Color_Index * 4) + (itr * 4) + 2) = (*(palette_data + (Color_Index * 3) + (itr * 3) + 1) << 2);
        // *(p_Palette_XBGR + (Color_Index * 4) + (itr * 4) + 1) = (*(palette_data + (Color_Index * 3) + (itr * 3) + 2) << 2);
        // *(p_Palette_XBGR + (Color_Index * 4) + (itr * 4) + 0) = 0x00;
        *(p_Palette_XBGR + (Color_Index * 4) + (itr * 4) + 3) = 0x00;
        *(p_Palette_XBGR + (Color_Index * 4) + (itr * 4) + 2) = (*(palette_data + (Color_Index * 3) + (itr * 3) + 0) << 2);
        *(p_Palette_XBGR + (Color_Index * 4) + (itr * 4) + 1) = (*(palette_data + (Color_Index * 3) + (itr * 3) + 1) << 2);
        *(p_Palette_XBGR + (Color_Index * 4) + (itr * 4) + 0) = (*(palette_data + (Color_Index * 3) + (itr * 3) + 2) << 2);
    }

    // VGA_SetFont(0, 0, 0, 0);

    if ( start_color == -1 )
    {
        PAL_Set_PaletteFlags(0, 255);
    }
    else
    {
        PAL_Set_PaletteFlags(start_color, end_color);
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: PAL_Load_Palette(entry = %d, start_color = %d, end_color = %d)\n", __FILE__, __LINE__, entry, start_color, end_color);
#endif

}

// s20p05   VGA_SetDACChanged()
// MoO2 Module: palette Set_Palette_Changes()
void PAL_Set_PaletteFlags(int start_color, int end_color)
{
    int i;
    for ( i = start_color; i < end_color; i++)
    {
        *(p_PaletteFlags + i) = 1;
    }
}
