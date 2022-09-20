// MGC seg020
// ST_VGA.H

#include "seg020.H"

#include "ST_LBX.H"
#include "ST_TXT.H"
#include "ST_VGA.H"

#include "STU_DBG.H"


// s20p01
// 1oom :: lbxpal.c :: void lbxpal_select(int pal_index, int first/*or -1*/, int last)
void VGA_LoadPalette(int Palette_Index, int First_Color, int Last_Color)
{
    int Color_Index;
    int Color_Count;
    // int itr_Color_Count;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: VGA_LoadPalette(Palette_Index = %d, First_Color = %d, Last_Color = %d)\n", __FILE__, __LINE__, Palette_Index, First_Color, Last_Color);
#endif

    // ? passes in a SAMB_head, gets back a SAMB_data ?
    gsa_gsa_PaletteLbxEntry = LBXE_LoadReplace(g_PaletteLbxFileName, Palette_Index+2, gsa_PaletteLbxEntry);
                             // |-> LBX_EntryLoader(g_PaletteLbxFileName, Palette_Index+2, gsa_PaletteLbxEntry, 1, 0);

//    dlvfprintf("DEBUG: [%s, %d]: gsa_gsa_PaletteLbxEntry = 0x%04X\n", __FILE__, __LINE__, gsa_gsa_PaletteLbxEntry);

                                                              //                0x0000  3*256 palette / color-map
    gsa_Palette_Font_Colors = gsa_gsa_PaletteLbxEntry +  48;  //  48 pr  768 b  0x0300  16 arrays of 16 colors
    UU_gsa_Palette_Data     = gsa_Palette_Font_Colors +  16;  //  16 pr  256 b  0x0400  ? UnUsed ? DNE in MoO1 ?
    gsa_Cursor_Array        = UU_gsa_Palette_Data     +  16;  //  16 pr  256 b  0x0500  16 16x16 cursor bitmap images
    gsa_ShadingColors       = gsa_Cursor_Array        + 256;  // 256 pr 4096 b  0x1500  5,376 bytes array of 24 color fractions (B-G-R-Percent)


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

asm push si
asm push di
asm push es
asm push ds
asm mov di, Color_Index
asm mov bx, di
asm add bx, 0x300                           //; 768
asm mov ax, di
asm shl di, 1
asm add di, ax
asm mov si, di
asm mov cx, Color_Count
asm mov ax, gsa_gsa_PaletteLbxEntry
asm mov dx, gsa_Palette
asm mov ds, ax  // SrcSgmt: _DS = gsa_gsa_PaletteLbxEntry
asm mov es, dx  // DstSgmt: _ES = gsa_Palette
Copy_Red:
asm lodsb
asm cmp al, [es:di]
asm jz short Copy_Green
asm mov byte ptr [es:bx], 1
Copy_Green:
asm stosb
asm lodsb
asm cmp al, [es:di]
asm jz short Copy_Blue
asm mov byte ptr [es:bx], 1
Copy_Blue:
asm stosb
asm lodsb
asm cmp al, [es:di]
asm jz short LoopColorCount
asm mov byte ptr [es:bx], 1
LoopColorCount:
asm stosb
asm inc bx
asm loop Copy_Red
asm pop ds
asm pop es
asm pop di
asm pop si


//    dlvfprintf("DEBUG: [%s, %d] Call: VGA_SetFont(0, 0, 0, 0)\n", __FILE__, __LINE__);
    //VGA_SetFont(0, 0);  // argument missing in dasm
    //HERE("VGA_SetFont(0, 0, 0, 0);");
    //VGA_SetFont(0, 0, 0, 0);

    if ( First_Color == -1 )
    {
        HERE("VGA_SetDACChanged(0, 255);");
        VGA_SetDACChanged(0, 255);
    }
    else
    {
        HERE("VGA_SetDACChanged(First_Color, Last_Color);");
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
