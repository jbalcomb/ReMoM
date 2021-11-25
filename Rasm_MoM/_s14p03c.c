// _s14p03c.c VGA_DAC_Init
// in MOM_DEF.H

#include "ST_HEAD.H"
#include "ST_LBX.H"
#include "ST_VGA.H"


#define SZ_348PR_5568B 348
#define SZ_64PR_1024B 64
#define SZ_48PR_768B 48
#define SZ_2PR_32B 2

/*
Loads FONTS.LBX records 1 & 2
Allocates memory from the far heap...
    5568 bytes for a LBX Palette Record
    1024 bytes for a 256 color RGB palette and 256 color change flags

*/

void VGA_DAC_Init(char *PaletteLbxFileName)
{
    unsigned int itrPalette;
    unsigned int itrPaletteFlags;

    printf("DEBUG: BEGIN: VGA_DAC_Init()\n");

    printf("DEBUG: PaletteLbxFileName: %s\n", PaletteLbxFileName);
    strcpy(g_PaletteLbxFileName, PaletteLbxFileName);
    printf("DEBUG: g_PaletteLbxFileName: %s\n", g_PaletteLbxFileName);

    gsa_FontStyleData = LBXE_LoadSingle(PaletteLbxFileName, 0);
    printf("DEBUG: gsa_FontStyleData: 0x%04X\n", gsa_FontStyleData);

    gsa_BorderStyleData = LBXE_LoadSingle(PaletteLbxFileName, 1);
    printf("DEBUG: gsa_BorderStyleData: 0x%04X\n", gsa_BorderStyleData);

    gsa_PaletteLbxEntry = SA_Allocate_Space(348);      // 348 paragraphs = 386 * 16 bytes = 5,568 bytes
    printf("DEBUG: gsa_PaletteLbxEntry: 0x%04X\n", gsa_PaletteLbxEntry);

    gsa_Palette = SA_Allocate_Space(64);               // 64 paragraphcs = 64 * 16 bytes = 1024 bytes
    printf("DEBUG: gsa_Palette: 0x%04X\n", gsa_Palette);

    gsa_PaletteFlags = gsa_Palette + 48;
    printf("DEBUG: gsa_PaletteFlags: 0x%04X\n", gsa_PaletteFlags);

    gsa_PaletteSaved = SA_Allocate_Space(48);          // 48 paragraphcs = 48 * 16 bytes = 768 bytes
    printf("DEBUG: gsa_PaletteSaved: 0x%04X\n", gsa_PaletteSaved);

    gsa_ReplacementColors = SA_Allocate_Space(384);    // ; 348 paragraphcs = 384 * 16 bytes = 6144 bytes
    printf("DEBUG: gsa_ReplacementColors: 0x%04X\n", gsa_ReplacementColors);

    gsa_VGAFILEH_Header = SA_Allocate_Space(2);        // 2 paragraphs = 2 * 16 bytes = 32 bytes
    printf("DEBUG: gsa_VGAFILEH_Header: 0x%04X\n", gsa_VGAFILEH_Header);

    gsa_IntensityScaleTable = SA_Allocate_Space(2);    // 96 paragraphs = 96 * 16 = 1536 bytes
    printf("DEBUG: gsa_IntensityScaleTable: 0x%04X\n", gsa_IntensityScaleTable);

    VGA_TextDraw_Init();

    for ( itrPalette = 0; itrPalette < 768; itrPalette++)
    {
        farpokeb(gsa_Palette, itrPalette, 0);
    }

    for ( itrPaletteFlags = 0; itrPaletteFlags < 256; itrPaletteFlags++)
    {
        farpokeb(gsa_Palette, itrPaletteFlags, 1);
    }

    printf("DEBUG: END: VGA_DAC_Init()\n");
}
