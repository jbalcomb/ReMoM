// MGC  seg014  WZD  seg014
// MOM_DEF.H

#include "seg014.H"

#include "ST_HEAD.H"

#include "ST_LBX.H"
#include "ST_SA.H"
#include "ST_VGA.H"


// s14p01
void Hardware_Init(int argInputType, int argSoundChannels, char *argFontFileName, int M_Drv, int M_IO, int M_IRQ, int M_DMA, int D_Drv, int D_IO, int D_IRQ, int D_DMA)
{
    int tmpInputType;

//    dlvfprintf("DEBUG: %s %d BEGIN: Hardware_Init()\n", __FILE__, __LINE__);

    // CRP_Empty_Exit_Fn2()

    EMM_Startup();

    VGA_SetModeY();

    if (argInputType == -1)
    {
        VGA_DAC_Init(DEFAULT_FONTS_FILE);
        //SND_Init(0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1);
        tmpInputType = 1;
    }
    else
    {
        VGA_DAC_Init(argFontFileName);
        //SND_Init(M_Drv, argSoundChannels, M_IO, M_IRQ, M_DMA, D_Drv, D_IO, D_IRQ, D_DMA);
        tmpInputType = argInputType;
    }

    IN_Init(tmpInputType);  // _s34p65

    RNG_TimerSeed();

    VGA_Set_DSP_Addr();

//    dlvfprintf("DEBUG: %s %d END: Hardware_Init()\n", __FILE__, __LINE__);
}

// s14p03
// TODO(JimBalcomb): move these defs elsewhere - ST_HEAD, MOM_DEF, etc.
//#define SZ_348PR_5568B 348
//#define SZ_64PR_1024B 64
//#define SZ_48PR_768B 48
//#define SZ_2PR_32B 2
/*
Loads FONTS.LBX records 1 & 2
Allocates memory from the far heap...
    5568 bytes for a LBX Palette Record
    1024 bytes for a 256 color RGB palette and 256 color change flags

*/
//F:\Development_OPC\momrtgt-code\MoMModel\MoMTemplate.h
//J:\STU\DBWD\developc\1oom-master\src\lbxfont.c
/*
    Initialize Font, Palette, Text
*/
void VGA_DAC_Init(char *PaletteLbxFileName)
{
    unsigned int itrPalette;
    unsigned int itrPaletteFlags;
    void * pPaletteLbxEntry;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: VGA_DAC_Init(PaletteLbxFileName = %s)\n", __FILE__, __LINE__, PaletteLbxFileName);
#endif

    strcpy(g_PaletteLbxFileName, PaletteLbxFileName);

    gsa_FontStyleData = LBXE_LoadSingle(PaletteLbxFileName, 0);

    gsa_BorderStyleData = LBXE_LoadSingle(PaletteLbxFileName, 1);

    // gsa_PaletteLbxEntry = FP_SEG(SA_Allocate_Space(348));       // 348 paragraphs = 386 * 16 bytes = 5,568 bytes
    pPaletteLbxEntry = SA_Allocate_Space(348);
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] pPaletteLbxEntry: %p\n", __FILE__, __LINE__, pPaletteLbxEntry);
#endif
    gsa_PaletteLbxEntry = FP_SEG(pPaletteLbxEntry);
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] gsa_PaletteLbxEntry: 0x%04X\n", __FILE__, __LINE__, gsa_PaletteLbxEntry);
#endif

    gsa_Palette = FP_SEG(SA_Allocate_Space(64));                // 64 paragraphcs = 64 * 16 bytes = 1024 bytes
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] gsa_Palette: 0x%04X\n", __FILE__, __LINE__, gsa_Palette);
#endif
    gsa_PaletteFlags = gsa_Palette + 48;                        // 48 paragaphs = 48 * 16 = 768 bytes
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] gsa_PaletteFlags: 0x%04X\n", __FILE__, __LINE__, gsa_PaletteFlags);
#endif

    gsa_PaletteSaved = FP_SEG(SA_Allocate_Space(48));           // 48 paragraphcs = 48 * 16 bytes = 768 bytes

    gsa_ReplacementColors = FP_SEG(SA_Allocate_Space(384));     // 348 paragraphcs = 384 * 16 bytes = 6144 bytes / 256 = 24 & 256 - 24 - 232 ? shading colors in _R functions ?

    gsa_VGAFILEH_Header = FP_SEG(SA_Allocate_Space(2));         // 2 paragraphs = 2 * 16 bytes = 32 bytes

    gsa_IntensityScaleTable = FP_SEG(SA_Allocate_Space(2));     // 96 paragraphs = 96 * 16 = 1536 bytes / 256 = 6

    VGA_TextDraw_Init();

    for ( itrPalette = 0; itrPalette < 768; itrPalette++)
    {
        farpokeb(gsa_Palette, itrPalette, 0);
    }

    for ( itrPaletteFlags = 0; itrPaletteFlags < 256; itrPaletteFlags++)
    {
        farpokeb(gsa_PaletteFlags, itrPaletteFlags, 1);
    }

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: VGA_DAC_Init(PaletteLbxFileName = %s)\n", __FILE__, __LINE__, PaletteLbxFileName);
#endif
}

// s14p04
void VGA_SetDrawWindow(int Min_X, int Min_Y, int Max_X, int Max_Y)
{
    int tmp_Min_X;
    int tmp_Min_Y;
    int tmp_Max_X;
    int tmp_Max_Y;
    int tmp;

    if ( tmp_Min_X < 0 )
    {
        tmp_Min_X = 0;
    }
    if ( tmp_Min_Y < 0 )
    {
        tmp_Min_Y = 0;
    }
    if ( tmp_Max_X > 319 )
    {
        tmp_Min_X = 319;
    }
    if ( tmp_Max_Y > 199 )
    {
        tmp_Min_Y = 199;
    }
    if ( tmp_Min_X > tmp_Max_X )
    {
        tmp = tmp_Max_X;
        tmp_Max_X = tmp_Min_X;
        tmp_Min_X = tmp;
    }
    if ( tmp_Min_Y > tmp_Max_Y )
    {
        tmp = tmp_Max_Y;
        tmp_Max_Y = tmp_Min_Y;
        tmp_Min_Y = tmp;
    }

    g_VGA_Min_X = tmp_Min_X;
    g_VGA_Max_X = tmp_Max_X;
    g_VGA_Min_Y = tmp_Min_Y;
    g_VGA_Max_Y = tmp_Max_Y;

}

// s14p05
void VGA_ResetDrawWindow(void)
{
    g_VGA_Min_X = 0;
    g_VGA_Max_X = 319;
    g_VGA_Min_Y = 0;
    g_VGA_Max_Y = 199;
}
