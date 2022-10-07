// ~== handmade.cpp

#include "mom_x64.h"
// //#include "ST_HEAD.H"
// //#include "MOM_HEAD.H"
// #include "MOM_DEF.H"  // MoX_Quit(); EMM_PAGES_REQUIRED, EMM_MIN_KB, RAM_MIN_KB
// #include "MGC_HEAD.H"
// #include "ST_EMM.H"  // g_EMM_Pages_Reserved
// #include "ST_GUI.H"
// #include "ST_VGA.H"
// #include "STU_DBG.H"
// //#include "ST_SA.C"
// #include "ST_EMM.C"  // g_EMM_Pages_Reserved



void 
MainLoop()
{
    void *FileContents = PlatformLoadFile("assets.bmp");
}

int MGC_Main(void)
{
    /*  g_EMM_Pages_Reserved = EMM_PAGES_REQUIRED;  */
    /*  CONFIG.MOM  LoadConfigMom()  */
    /*  MAGIC.SET  LoadMagicSet()  */
    /*  MIDI Driver, DSP Driver  */
    /*  SAVE.GAM  */
    /*  EMM_SetMinKB(EMM_MIN_KB);  */
    /*  RAM_SetMinKB(RAM_MIN_KB);  */



    /*
        BEGIN: Hardware_Init()
    */

    //Settings.InputType = 1;
    // _s14p01c.c
    //int __cdecl __far Hardware_Init(int Input_Type, int Snd_Chnls, char *FontFile, int M_Drv, int M_IO, int M_IRQ, int M_DMA, int D_Drv, int D_IO, int D_IRQ, int D_DMA)
    /*  Hardware_Init(1, 2, GAME_FONT_FILE, 0, 0, 0, 0, 0, 0, 0, 0);  // Defaults for 'No Sound'  */
    /*
        |-> EMM_Startup()
        |-> VGA_SetModeY()
        |-> VGA_DAC_Init() |-> ... LBXE_LoadSingle(), SA_Allocate_Space(), VGA_TextDraw_Init()
        |-> SND_Init()
        |-> IN_Init() |-> MD_Init()
        |-> RNG_TimerSeed()
        |-> VGA_Set_DSP_Addr()
    */
    //gsa_PaletteLbxEntry = SA_Allocate_Space(348);      // 348 paragraphs = 386 * 16 bytes = 5,568 bytes

    /* 
        main() |-> Hardware_Init() |-> VGA_DAC_Init() |-> VGA_TextDraw_Init()
        _s19p14c.c  void VGA_TextDraw_Init(void)
    */
    // UU_g_VGA_TextDraw_Initd = 1;
    // gfp_VGA_TextLine_Lefts = SA_Allocate_Space(8);   // 8 paragraphs = 8 * 16 bytes = 128 bytes
    // gfp_VGA_TextLine_Rights = SA_Allocate_Space(8);  // 8 paragraphs = 8 * 16 bytes = 128 bytes
    // gfp_VGA_TextLine_Tops = SA_Allocate_Space(8);    // 8 paragraphs = 8 * 16 bytes = 128 bytes
    // gfp_VGA_TextLine_Starts = SA_Allocate_Space(8);  // 8 paragraphs = 8 * 16 bytes = 128 bytes

    /* 
        main() |-> Hardware_Init() |-> IN_Init()
        _s34p65c.c IN_Init
    */
    // gfp_CTRL_Control_Table = SA_Allocate_Space(357);  // 357 paragraphs = 367 * 16 = 5712 bytes  (? 150*38=5700 ? + 12 ?)


    /*
        END: Hardware_Init()
    */



    /*  DBG_Disable();  */
    /*  MoM_Tables_Init(6100);  */  // TODO(JimBalcomb,20220818): move whatever this 6100 is to a manifest constant
    /*
        |-> SA_Allocate_Space()
        |-> SA_Alloc_First()
        |-> SA_Alloc_Next()
    */
    /*  IN_SetEscOverride();  */
    /*  VGA_LoadPalette(0, -1, 0);  VGA_DAC_Write();  */
    /*  if not "JENNY", then Play_Intro()  */
    /*  GAME_LoadMainImages();  */
    /*
        GAME_Load_TERRSTAT_0();
            int LbxEntryIndex = 0;
            int RecFirst = 0;
            int RecCount = 770;
            int RecSize = 6;
            gfp_TBL_Moves_Per_Tile = SA_MK_FP0(LBXR_LoadSingle(g_LbxNm_TERRSTAT, LbxEntryIndex, RecFirst, RecCount, RecSize));
    */
    /*  GAME_Load_SPELLDAT_0();  */
    /*  Main Menu - Credits  GAME_PrepareCredits();  */
    /*  VGA_DrawFilledRect(0, 0, 319, 199, 0);  */  // ~= Clear Screen, on Draw Screen-Page
    /*  VGA_SetDirectDraw();  */
    /*  VGA_DrawFilledRect(0, 0, 319, 199, 0);  */  // ~= Clear Screen, on Render Screen-Page
    /*  VGA_Set_DSP_Addr();  */
    /*  SND_Stop_Music()  MainMenu_Music_Seg = LBXE_LoadSingle("music", 104);  if (Settings.BG_Music == 1)  SND_PlayFile(MainMenu_Music_Seg)  */
    /*  VGA_LoadPalette(0, -1, 0);  VGA_DAC_Write();  */  // EMPERATO
    /*  GAME_MainMenu();  */  // MGC_DEF.H  _s01p03c.c
    /*  SAVEGAM_Save(8)  */
    /*  Quit()  */
    return 0;
}
