// _s14p03c.c VGA_DAC_Init
// in MOM_DEF.H

#include "ST_HEAD.H"
#include "ST_LBX.H"
#include "ST_VGA.H"

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

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: VGA_DAC_Init(PaletteLbxFileName=%s)\n", __FILE__, __LINE__, PaletteLbxFileName);
#endif

    strcpy(g_PaletteLbxFileName, PaletteLbxFileName);

    gsa_FontStyleData = LBXE_LoadSingle(PaletteLbxFileName, 0);

    gsa_BorderStyleData = LBXE_LoadSingle(PaletteLbxFileName, 1);

    gsa_PaletteLbxEntry = SA_Allocate_Space(348);      // 348 paragraphs = 386 * 16 bytes = 5,568 bytes

    gsa_Palette = SA_Allocate_Space(64);               // 64 paragraphcs = 64 * 16 bytes = 1024 bytes

    gsa_PaletteFlags = gsa_Palette + 48;               // 48 paragaphs = 48 * 16 = 768

    gsa_PaletteSaved = SA_Allocate_Space(48);          // 48 paragraphcs = 48 * 16 bytes = 768 bytes

    gsa_ReplacementColors = SA_Allocate_Space(384);    // 348 paragraphcs = 384 * 16 bytes = 6144 bytes / 256 = 24 & 256 - 24 - 232 ? shading colors in _R functions ?

    gsa_VGAFILEH_Header = SA_Allocate_Space(2);        // 2 paragraphs = 2 * 16 bytes = 32 bytes

    gsa_IntensityScaleTable = SA_Allocate_Space(2);    // 96 paragraphs = 96 * 16 = 1536 bytes / 256 = 6

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
    dlvfprintf("DEBUG: [%s, %d] END: VGA_DAC_Init(PaletteLbxFileName=%s)\n", __FILE__, __LINE__, PaletteLbxFileName);
#endif
}
