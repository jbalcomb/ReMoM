// _s20o01c.c VGA_LoadPalette

#include "ST_TXT.H"
#include "ST_VGA.H"


void VGA_LoadPalette(int Palette_Index, int First_Color, int Last_Color)
{
    unsigned int tmp_SgmtAddr;
    int First_Index;
    int Color_Count;

    gsa_gsa_PaletteLbxEntry = LBXE_LoadReplace(g_PaletteLbxFileName, Palette_Index+2, gsa_PaletteLbxEntry);
                             // |-> LBX_EntryLoader(g_PaletteLbxFileName, Palette_Index+2, gsa_PaletteLbxEntry, 1, 0);

    printf("DEBUG:  gsa_gsa_PaletteLbxEntry 0x%04X  gsa_PaletteLbxEntry 0x%04X  \n");

    tmp_SgmtAddr = gsa_gsa_PaletteLbxEntry;

    tmp_SgmtAddr = tmp_SgmtAddr + 48;
    gsa_Palette_Font_Colors = tmp_SgmtAddr;

    tmp_SgmtAddr = tmp_SgmtAddr + 16;
    UU_gsa_Palette_Data = tmp_SgmtAddr;

    tmp_SgmtAddr = tmp_SgmtAddr + 16;
    gsa_Palette_Cursor_Data = tmp_SgmtAddr;

    tmp_SgmtAddr = tmp_SgmtAddr + 256;
    gsa_ShadingColors = tmp_SgmtAddr;


    if ( First_Color == -1 )
    {
        First_Index = 0;
        Color_Count = 256;
    }
    else
    {
        First_Index = First_Color;
        Color_Count = (Last_Color - First_Color) + 1;
    }


asm push si
asm push di
asm push es
asm push ds
asm mov di, First_Index
asm mov bx, di
asm add bx, 0x300                           ; 768
asm mov ax, di
asm shl di, 1
asm add di, ax
asm mov si, di
asm mov cx, Color_Count
asm mov ax, gsa_gsa_PaletteLbxEntry
asm mov dx, gsa_VGA_DAC_Segment
asm mov ds, ax
asm mov es, dx
Copy_Red:
asm lodsb
asm cmp al, [es:di]
asm jz short Copy_Blue
asm mov [byte ptr es:bx], 1
Copy_Blue:
asm stosb
asm lodsb
asm cmp al, [es:di]
asm jz short Copy_Blue
asm mov [byte ptr es:bx], 1
Copy_Blue:
asm stosb
asm lodsb
asm cmp al, [es:di]
asm jz short LoopColorCount
asm mov [byte ptr es:bx], 1
LoopColorCount:
asm stosb
asm inc bx
asm loop Copy_Red
asm pop ds
asm pop es
asm pop di
asm pop si


    VGA_SetFont(0, 0);


    if ( First_Color == -1 )
    {
        VGA_SetDACChanged(0, 255);
    }
    else
    {
        VGA_SetDACChanged(First_Color, Last_Color);
    }

}
