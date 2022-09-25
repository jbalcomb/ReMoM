// MGC seg020
// ST_VGA.H

#include "ST_TYPE.H"
#include "ST_DEF.H"
#include "seg020.H"

#include "ST_LBX.H"
#include "ST_TXT.H"
#include "ST_VGA.H"

// #include "STU_DBG.H"


// s20p01
// 1oom :: lbxpal.c :: void lbxpal_select(int pal_index, int first/*or -1*/, int last)
void VGA_LoadPalette(int Palette_Index, int First_Color, int Last_Color)
{
    int Color_Index;
    int Color_Count;
    // int itr_Color_Count;
    unsigned int DstSgmt;
    unsigned int DstOfst;
    unsigned int SrcSgmt;
    unsigned int SrcOfst;
    unsigned char *fptr_DstByte;
    unsigned char *fptr_SrcByte;
    int itr_Color_Count;

// #ifdef DEBUG
//     dlvfprintf("DEBUG: [%s, %d] BEGIN: VGA_LoadPalette(Palette_Index = %d, First_Color = %d, Last_Color = %d)\n", __FILE__, __LINE__, Palette_Index, First_Color, Last_Color);
// #endif

    // ? passes in a SAMB_head, gets back a SAMB_data ?
    // gsa_PaletteLbxEntry allocated in VGA_DAC_Init() _s14p03
    gsa_gsa_PaletteLbxEntry = LBXE_LoadReplace(g_PaletteLbxFileName, Palette_Index+2, gsa_PaletteLbxEntry);
                             // |-> LBX_EntryLoader(g_PaletteLbxFileName, Palette_Index+2, gsa_PaletteLbxEntry, 1, 0);

//    dlvfprintf("DEBUG: [%s, %d]: gsa_gsa_PaletteLbxEntry = 0x%04X\n", __FILE__, __LINE__, gsa_gsa_PaletteLbxEntry);

                                                              //                0x0000  3*256 palette / color-map
    gsa_Palette_Font_Colors = gsa_gsa_PaletteLbxEntry +  48;  //  48 pr  768 b  0x0300  16 arrays of 16 colors
    UU_gsa_Palette_Data     = gsa_Palette_Font_Colors +  16;  //  16 pr  256 b  0x0400  ? UnUsed ? DNE in MoO1 ?
    gsa_Cursor_Array        = UU_gsa_Palette_Data     +  16;  //  16 pr  256 b  0x0500  16 16x16 cursor bitmap images
    gsa_ShadingColors       = gsa_Cursor_Array        + 256;  // 256 pr 4096 b  0x1500  5,376 byte array of 24 color fractions (B-G-R-Percent)
    

    if ( First_Color == -1 )
    {
        Color_Index = 0;
        Color_Count = 256;
    }
    else
    {
        Color_Index = First_Color;
        Color_Count = (Last_Color - First_Color) + 1;
    }

    // for ( itr_Color_Count = Color_Index; itr_Color_Count < Color_Count; itr_Color_Count++ )
    // {
    //     gsa_Palette[Color_Index + itr_Color_Count] = gsa_gsa_PaletteLbxEntrygsa_Palette[Color_Index + itr_Color_Count];
    //     gsa_Palette[Color_Index + itr_Color_Count] = 1;
    // }

// asm push si
// asm push di
// asm push es
// asm push ds
// asm mov di, Color_Index
// asm mov bx, di
// asm add bx, 0x300                           //; 768
// asm mov ax, di
// asm shl di, 1
// asm add di, ax
// asm mov si, di  // SrcOfst: DstOfst: ((Color_Index * 2) + Color_Index) = (Color_Index * 3)
// asm mov cx, Color_Count
// asm mov ax, gsa_gsa_PaletteLbxEntry
// asm mov ds, ax  // SrcSgmt: _DS = gsa_gsa_PaletteLbxEntry
// asm mov dx, gsa_Palette
// asm mov es, dx  // DstSgmt: _ES = gsa_Palette
// Copy_Red:
// asm lodsb
// asm cmp al, [es:di]  // Is the /new/ color the same as the /old/ color?
// asm jz short Copy_Green
// asm mov byte ptr [es:bx], 1  // set PaletteFlags
// Copy_Green:
// asm stosb
// asm lodsb
// asm cmp al, [es:di]
// asm jz short Copy_Blue
// asm mov byte ptr [es:bx], 1  // set PaletteFlags
// Copy_Blue:
// asm stosb
// asm lodsb
// asm cmp al, [es:di]
// asm jz short LoopColorCount
// asm mov byte ptr [es:bx], 1  // set PaletteFlags
// LoopColorCount:
// asm stosb
// asm inc bx
// asm loop Copy_Red
// asm pop ds
// asm pop es
// asm pop di
// asm pop si

/*
    copy gsa_gsa_PaletteLbxEntry to gsa_Palette, while setting gsa_PaletteFlags
    ? same logic/approach as s14p03 VGA_DAC_Init() ?
    ? same logic/approach as s21p07 FLIC_LoadPalette() ?
*/
    DstSgmt = gsa_Palette;
    DstOfst = (Color_Index * 3);
    SrcSgmt = gsa_gsa_PaletteLbxEntry;  // SAMB_data Type 1, returned by LBXE_LoadReplace()
    SrcOfst = (Color_Index * 3);
    
    // fptr_DstByte = (unsigned char *)MK_FP(DstSgmt, 0);
    // fptr_SrcByte = (unsigned char *)MK_FP(SrcSgmt, 0);
    fptr_DstByte = (unsigned char *)MK_FP(DstSgmt, DstOfst);
    fptr_SrcByte = (unsigned char *)MK_FP(SrcSgmt, SrcOfst);

    for(itr_Color_Count = 0; itr_Color_Count < (Color_Count * 3); itr_Color_Count++)
    {
        // dlvfprintf("DEBUG: [%s, %d] fptr_SrcByte[%u]: %02X %u)\n", __FILE__, __LINE__, itr_Color_Count, fptr_SrcByte[(SrcOfst + itr_Color_Count)], fptr_SrcByte[(SrcOfst + itr_Color_Count)]);
        // fptr_DstByte[(DstOfst + itr_Color_Count)] = fptr_SrcByte[(SrcOfst + itr_Color_Count)];
        *fptr_DstByte++ = *fptr_SrcByte++;
    }


//    dlvfprintf("DEBUG: [%s, %d] Call: VGA_SetFont(0, 0, 0, 0)\n", __FILE__, __LINE__);
    //VGA_SetFont(0, 0);  // argument missing in dasm
    //HERE("VGA_SetFont(0, 0, 0, 0);");
    //VGA_SetFont(0, 0, 0, 0);

    if ( First_Color == -1 )
    {
        // HERE("VGA_SetDACChanged(0, 255);");
        VGA_SetDACChanged(0, 255);
    }
    else
    {
        // HERE("VGA_SetDACChanged(First_Color, Last_Color);");
        VGA_SetDACChanged(First_Color, Last_Color);
    }

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: VGA_LoadPalette()\n", __FILE__, __LINE__);
#endif
}

// s20p05
void VGA_SetDACChanged(int First_Color, int Last_Color)
{
    int itr_colors;

    for ( itr_colors = First_Color; itr_colors < Last_Color; itr_colors++)
    {
        farpokeb(gsa_PaletteFlags, itr_colors, 1);
    }
}
