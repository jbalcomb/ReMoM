// MGC  seg014  WZD  seg014
// MOM_DEF.H
// MoO2 Module: graphics

#include "seg014.H"
#include "seg020.H"     /* palette_block */


// #include "ST_HEAD.H"
// #include "MoX_TYPE.H"
// #include "ST_DEF.H"  /* SWAP(); */

#include "ST_LBX.H"
#include "ST_SA.H"
#include "ST_VGA.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif


/*
DEFAULT_FONTS_FILE

g_PaletteLbxFileName
gsa_FontStyleData
gsa_BorderStyleData
gsa_PaletteLbxEntry
p_Palette
p_PaletteFlags
gsa_PaletteSaved
gsa_ReplacementColors
gsa_VGAFILEH_Header
gsa_IntensityScaleTable

EMM_Startup()
VGA_SetModeY()
Load_Font_File()
SND_Init()
IN_Init()
RNG_TimerSeed()
VGA_Set_DSP_Addr()
strcpy()
LBXE_LoadSingle()
FP_SEG()
SA_Allocate_Space()
VGA_TextDraw_Init();
farpokeb()
*/

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif

// s14p01
// void Hardware_Init(int argInputType, int argSoundChannels, char *argFontFileName, int M_Drv, int M_IO, int M_IRQ, int M_DMA, int D_Drv, int D_IO, int D_IRQ, int D_DMA)
void Hardware_Init(int argInputType, char * font_file)
{
    int tmpInputType;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Hardware_Init()\n", __FILE__, __LINE__);
#endif

    EMM_Startup();
    VGA_SetModeY();
    if (argInputType == -1)
    {
        Load_Font_File(DEFAULT_FONTS_FILE);
        //SND_Init(0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1);
        tmpInputType = 1;
    }
    else
    {
        Load_Font_File(font_file);
        //SND_Init(M_Drv, argSoundChannels, M_IO, M_IRQ, M_DMA, D_Drv, D_IO, D_IRQ, D_DMA);
        tmpInputType = argInputType;
    }
    IN_Init(tmpInputType);  // MGC s34p65  ST_GUI.H
    RNG_TimerSeed();
    VGA_Set_DSP_Addr();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Hardware_Init()\n", __FILE__, __LINE__);
#endif

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
// MoO2 Load_Font_File()
void Load_Font_File(char * font_file)
{
    int itr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Load_Font_File(font_file = %s)\n", __FILE__, __LINE__, font_file);
#endif

    strcpy(font_name, font_file);

    /* MoO2 font_ptr */
    // SM2LM  sa_FontStyleData = LBXE_LoadSingle(font_file, 0);     // ∵ Load Type 0 ∴ SA_Allocate_MemBlk() { SAMB Data Type 0 }
    p_FontStyleData = LBXE_LoadSingle_LM(font_file, 0);
    // SM2LM  sa_FontStyleData = FP_SEG(p_FontStyleData);

    /* MoO2 DNE */
    // SM2LM  gsa_BorderStyleData = LBXE_LoadSingle(font_file, 1);     // ∵ Load Type 0 ∴ SA_Allocate_MemBlk() { SAMB Data Type 0 }
    p_BorderStyleData = LBXE_LoadSingle_LM(font_file, 1);  // ∵ Load Type 0 ∴ SA_Allocate_MemBlk() { SAMB Data Type 0 }
    // SM2LM  sa_BorderStyleData = FP_SEG(p_BorderStyleData);

    /* MoO2 palette_block */
    palette_block          = SA_Allocate_Space(348);            // 348 paragraphs = 386 * 16 bytes = 5568 bytes
    p_Palette              = SA_Allocate_Space(64);             //  64 paragraphs =  64 * 16 bytes = 1024 bytes
    p_PaletteFlags         = p_Palette + (48 * 16);             // ~== p_PaletteFlags = &p_Palette[768];
    // p_PaletteFlags         = PTR_ADD_PARAGRAPH(p_Palette, 48);
    p_Palette_XBGR         = SA_Allocate_Space(64);             //  64 paragraphs =  64 * 16 bytes = 1024 bytes

    UU_p_PaletteSaved      = SA_Allocate_Space(48);             //  48 paragraphs =  48 * 16 bytes =  768 bytes
    UU_sa_PaletteSaved     = FP_SEG(UU_p_PaletteSaved);

    p_ReplacementColors    = SA_Allocate_Space(384);            // 348 paragraphs = 384 * 16 bytes = 6144 bytes / 256 = 24 & 256 - 24 - 232 ? shading colors in _R functions ?
    sa_ReplacementColors   = FP_SEG(p_ReplacementColors);

    p_VGAFILEH_Header      = SA_Allocate_Space(2);              //   2 paragraphs =   2 * 16 bytes =   32 bytes
    sa_VGAFILEH_Header     = FP_SEG(p_VGAFILEH_Header);
    
    p_IntensityScaleTable  = SA_Allocate_Space(96);             //  96 paragraphs =  96 * 16 bytes = 1536 bytes / 256 = 6
    sa_IntensityScaleTable = FP_SEG(p_IntensityScaleTable);
    
    TextLine_Init();

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

// s14p04
// MoO2  Module: graphics  Set_Window(signed integer (2 bytes) x1, signed integer (2 bytes) y1, signed integer (2 bytes) x2, signed integer (2 bytes) y2)
void VGA_SetDrawWindow(int Min_X, int Min_Y, int Max_X, int Max_Y)
{
    if ( Min_X < 0 ) { Min_X = 0; }
    if ( Min_Y < 0 ) { Min_Y = 0; }
    // if (x2 > screen_pixel_width) { x2 = screen_pixel_width - 1;}
    // if (y2 > screen_pixel_height) { y2 = screen_pixel_height - 1;}
    if ( Max_X > 319 ) { Min_X = 319; }
    if ( Max_Y > 199 ) { Min_Y = 199; }
    if ( Min_X > Max_X ) { SWAP(Max_X,Min_X); }
    if ( Min_Y > Max_Y ) { SWAP(Max_Y,Min_Y); }

    g_VGA_Min_X = Min_X;  // screen_window_x1
    g_VGA_Max_X = Max_X;  // screen_window_x2
    g_VGA_Min_Y = Min_Y;  // screen_window_y1
    g_VGA_Max_Y = Max_Y;  // screen_window_y2
}

// s14p05
// MoO2  Module: graphics  Reset_Window()
void VGA_ResetDrawWindow(void)
{
    // screen_window_x1 = 0;
    // screen_window_y1 = 0;
    // screen_window_x2 = screen_pixel_width - 1;
    // screen_window_y2 = screen_pixel_height - 1;
    g_VGA_Min_X = 0;
    g_VGA_Max_X = 319;
    g_VGA_Min_Y = 0;
    g_VGA_Max_Y = 199;
}
