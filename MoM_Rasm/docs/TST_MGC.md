
# Test - MAGIC.EXE


Address / Pointer is not Zero / Null

? Address / Pointer testable as offset from other Address / Pointer ?
e.g., if ( gsa_PaletteFlags != (gsa_Palette + 48) )

? Address / Pointer testable with hard-coded value ?
e.g.,
    gsa_Palette[0,...,767] == 1
    gsa_PaletteFlags[0,...,255] == 1



MAGIC.EXE, main()
MGC_Main()

Hardware_Init()
|-> EMM_Startup()
|-> VGA_SetModeY()
|-> VGA_DAC_Init() |-> ... LBXE_LoadSingle(), SA_Allocate_Space(), VGA_TextDraw_Init()
|-> SND_Init()
|-> IN_Init() |-> MD_Init()
|-> RNG_TimerSeed()
|-> VGA_Set_DSP_Addr()

...
    VGA_LoadPalette(0, -1, 0);
    VGA_DAC_Write();
...
    VGA_DrawFilledRect(0, 0, 319, 199, 0);  // ~= Clear Screen, on Draw Screen-Page
    VGA_SetDirectDraw();
    VGA_DrawFilledRect(0, 0, 319, 199, 0);  // ~= Clear Screen, on Render Screen-Page
    VGA_Set_DSP_Addr();
...
    VGA_LoadPalette(0, -1, 0);  // EMPERATO
    VGA_DAC_Write();
...

    MoM_Tables_Init(6100);

    GAME_LoadMainImages();
    GAME_Load_TERRSTAT_0();
    GAME_Load_SPELLDAT_0();

## Addresses / Pointers

_s14p03c.c  VGA_DAC_Init()       void VGA_DAC_Init(char *PaletteLbxFileName)
_s19p14c.c  VGA_TextDraw_Init()  void VGA_TextDraw_Init(void)

VGA_DAC_Init()
    g_PaletteLbxFileName
    gsa_FontStyleData
    gsa_BorderStyleData
    gsa_PaletteLbxEntry
    gsa_Palette
    gsa_PaletteFlags
    gsa_PaletteSaved
    gsa_ReplacementColors
    gsa_VGAFILEH_Header
    gsa_IntensityScaleTable

VGA_TextDraw_Init()
    UU_g_VGA_TextDraw_Initd
    gfp_VGA_TextLine_Lefts
    gfp_VGA_TextLine_Rights
    gfp_VGA_TextLine_Tops
    gfp_VGA_TextLine_Starts

VGA_DAC_Init()
    strcpy(g_PaletteLbxFileName, PaletteLbxFileName);
    gsa_FontStyleData = LBXE_LoadSingle(PaletteLbxFileName, 0);
    gsa_BorderStyleData = LBXE_LoadSingle(PaletteLbxFileName, 1);
    gsa_PaletteLbxEntry = FP_SEG(SA_Allocate_Space(348));       // 348 paragraphs = 386 * 16 bytes = 5,568 bytes
    gsa_Palette = FP_SEG(SA_Allocate_Space(64));                // 64 paragraphcs = 64 * 16 bytes = 1024 bytes
    gsa_PaletteFlags = gsa_Palette + 48;                        // 48 paragaphs = 48 * 16 = 768 bytes
    gsa_PaletteSaved = FP_SEG(SA_Allocate_Space(48));           // 48 paragraphcs = 48 * 16 bytes = 768 bytes
    gsa_ReplacementColors = FP_SEG(SA_Allocate_Space(384));     // 348 paragraphcs = 384 * 16 bytes = 6144 bytes / 256 = 24 & 256 - 24 - 232 ? shading colors in _R functions ?
    gsa_VGAFILEH_Header = FP_SEG(SA_Allocate_Space(2));         // 2 paragraphs = 2 * 16 bytes = 32 bytes
    gsa_IntensityScaleTable = FP_SEG(SA_Allocate_Space(2));     // 96 paragraphs = 96 * 16 = 1536 bytes / 256 = 6

VGA_TextDraw_Init()
    UU_g_VGA_TextDraw_Initd = 1;
    gfp_VGA_TextLine_Lefts = SA_Allocate_Space(8);   // 8 paragraphs = 8 * 16 bytes = 128 bytes
    gfp_VGA_TextLine_Rights = SA_Allocate_Space(8);  // 8 paragraphs = 8 * 16 bytes = 128 bytes
    gfp_VGA_TextLine_Tops = SA_Allocate_Space(8);    // 8 paragraphs = 8 * 16 bytes = 128 bytes
    gfp_VGA_TextLine_Starts = SA_Allocate_Space(8);  // 8 paragraphs = 8 * 16 bytes = 128 bytes



## VGA_DAC_Init()
MOM_DEF.H
_s14p03c.c VGA_DAC_Init


## VGA_LoadPalette()
ST_VGA.H
_s20o01c.c  VGA_LoadPalette()  void VGA_LoadPalette(int Palette_Index, int First_Color, int Last_Color)

    From VGA_DAC_Init()
        gsa_PaletteLbxEntry

VGA_LoadPalette()
    gsa_gsa_PaletteLbxEntry
    gsa_Palette_Font_Colors
    UU_gsa_Palette_Data
    gsa_Cursor_Array
    gsa_ShadingColors

    gsa_gsa_PaletteLbxEntry = LBXE_LoadReplace(g_PaletteLbxFileName, Palette_Index+2, gsa_PaletteLbxEntry);
                             // |-> LBX_EntryLoader(g_PaletteLbxFileName, Palette_Index+2, gsa_PaletteLbxEntry, 1, 0);
                                                              //                0x0000  3*256 palette / color-map
    gsa_Palette_Font_Colors = gsa_gsa_PaletteLbxEntry +  48;  //  48 pr  768 b  0x0300  16 arrays of 16 colors
    UU_gsa_Palette_Data     = gsa_Palette_Font_Colors +  16;  //  16 pr  256 b  0x0400  ? UnUsed ? DNE in MoO1 ?
    gsa_Cursor_Array        = UU_gsa_Palette_Data     +  16;  //  16 pr  256 b  0x0500  16 16x16 cursor bitmap images
    gsa_ShadingColors       = gsa_Cursor_Array        + 256;  // 256 pr 4096 b  0x1500  5,376 bytes array of 24 color fractions (B-G-R-Percent)




## VGA_DAC_Write()
_s21p01c.c      VGA_DAC_Write               void VGA_DAC_Write(void)


## VGA_SetDACChanged()
// _s20p05c.c VGA_SetDACChanged
// ST_VGA.H

#include "ST_VGA.H"

void VGA_SetDACChanged(int First_Color, int Last_Color)
{
    int itr_colors;

    for ( itr_colors = First_Color; itr_colors < Last_Color; itr_colors++)
    {
        farpokeb(gsa_PaletteFlags, itr_colors, 1);
    }
}


## VGA_Set_DSP_Addr()
ST_VGA.H
_s26p02c.c  VGA_Set_DSP_Addr()  void VGA_Set_DSP_Addr(void)

VRAM_BASE
g_RSP_Idx
gsa_DSP_Addr

assert VRAM_BASE == 0xA000
assert g_RSP_Idx == 0 || 1
assert gsa_DSP_Addr = 0xA000 || 0xA4000

```c
void VGA_Set_DSP_Addr(void)
{
    gsa_DSP_Addr = VRAM_BASE + ( (1 - g_RSP_Idx ) << 2 );
}
```
