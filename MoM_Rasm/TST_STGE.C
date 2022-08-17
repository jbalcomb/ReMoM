
#include "MGC_DEF.H"  // gsa_MAINSCRN_0_AnimatedLogo
#include "MOM_DEF.H"  // Quit()
#include "ST_EMM.H"  // EMM_Startup
#include "ST_FLIC.H"  // FLIC_Draw_XY()
//#include "ST_LBX.H"
//#include "ST_SA.H"
#include "ST_VGA.H"  // DEFAULT_FONTS_FILE; VGA_SetModeY()

#include "STU_DBG.H"


void MGC_StartUp(void)
{
    g_EMM_Pages_Reserved = EMM_PAGES_REQUIRED;
    EMM_SetMinKB(EMM_MIN_KB);
    RAM_SetMinKB(RAM_MIN_KB);
    Hardware_Init(1, 2, GAME_FONT_FILE, 0, 0, 0, 0, 0, 0, 0, 0);  // Defaults for 'No Sound'
        // |-> EMM_Startup();  VGA_SetModeY();  VGA_DAC_Init();  SND_Init();  GUI_Init();  RNG_TimerSeed();  VGA_SetDrawFrame();
    DBG_Disable();
    LBX_Tables_Init(6100);
    IN_SetEscOverride();
    VGA_LoadPalette(0, -1, 0);
    VGA_DAC_Write();
    // if not "JENNY", then Play_Intro()
    // GAME_LoadMainImages();
    // GAME_Load_TERRSTAT_0();
    // GAME_Load_SPELLDAT_0();
    // GAME_PrepareCredits();
    // VGA_DrawFilledRect(0, 0, 319, 199, 0);  // ~= Clear Screen, on Draw Screen-Page
    // VGA_SetDirectDraw();
    // VGA_DrawFilledRect(0, 0, 319, 199, 0);  // ~= Clear Screen, on Render Screen-Page
    // VGA_SetDrawFrame();
    // SND_Stop_Music(); MainMenu_Music_Seg = LBXE_LoadSingle("music", 104); if (Settings.BG_Music == 1) { SND_PlayFile(MainMenu_Music_Seg); }
    // VGA_LoadPalette(0, -1, 0);  // EMPERATO
    // VGA_DAC_Write();
    // GAME_MainMenu();  // MGC_DEF.H  _s01p03c.c
    // SAVEGAM_Save(8)
    // Quit("Thank you for playing Master of Magic!$");
}

// void Hardware_Init(int argInputType, int argSoundChannels, char *argFontFileName, int M_Drv, int M_IO, int M_IRQ, int M_DMA, int D_Drv, int D_IO, int D_IRQ, int D_DMA)
// //Settings.InputType = 1;
// Hardware_Init(1, 2, GAME_FONT_FILE, 0, 0, 0, 0, 0, 0, 0, 0);  // Defaults for 'No Sound'
void MGC_HardwareInit(void)
{
    g_EMM_Pages_Reserved = EMM_PAGES_REQUIRED;
    EMM_SetMinKB(EMM_MIN_KB);
    RAM_SetMinKB(RAM_MIN_KB);
    // ...
    EMM_Startup();
    VGA_SetModeY();
    // ? // VGA_DAC_Init(DEFAULT_FONTS_FILE);
    VGA_DAC_Init("FONTS.LBX");
    // ? // SND_Init(0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1);
    // SND_Init(M_Drv, argSoundChannels, M_IO, M_IRQ, M_DMA, D_Drv, D_IO, D_IRQ, D_DMA);
    IN_Init(1);
    RNG_TimerSeed();
    VGA_Set_DSP_Addr();
}

void STGE_Startup(void)
{

    dlvfprintf("DEBUG: [%s, %d] g_EMM_Pages_Reserved = EMM_PAGES_REQUIRED<%d>\n", __FILE__, __LINE__, EMM_PAGES_REQUIRED);
    g_EMM_Pages_Reserved = EMM_PAGES_REQUIRED;

    dlvfprintf("DEBUG: [%s, %d] EMM_SetMinKB(EMM_MIN_KB<%d>)\n", __FILE__, __LINE__, EMM_MIN_KB);
    EMM_SetMinKB(EMM_MIN_KB);

    dlvfprintf("DEBUG: [%s, %d] RAM_SetMinKB(RAM_MIN_KB<%d>)\n", __FILE__, __LINE__, RAM_MIN_KB);
    RAM_SetMinKB(RAM_MIN_KB);

    // Hardware_Init(1, 2, GAME_FONT_FILE, 0, 0, 0, 0, 0, 0, 0, 0);  // Defaults for 'No Sound'
        // |-> EMM_Startup();  VGA_SetModeY();  VGA_DAC_Init();  SND_Init();  GUI_Init();  RNG_TimerSeed();  VGA_SetDrawFrame();
    HERE("CALL: EMM_Startup()");
    EMM_Startup();
    HERE("CALL: VGA_SetModeY()");
    VGA_SetModeY();
    HERE("CALL: VGA_DAC_Init(DEFAULT_FONTS_FILE)");
    VGA_DAC_Init(DEFAULT_FONTS_FILE);
    HERE("CALL: GUI_Init(1)");
    IN_Init(1);
    // HERE("RNG_TimerSeed()");
    RNG_TimerSeed();
    HERE("VGA_Set_DSP_Addr()");
    VGA_Set_DSP_Addr();

    DBG_Disable();

    LBX_Tables_Init(6100);

    IN_SetEscOverride();

    VGA_LoadPalette(0, -1, 0);
    VGA_DAC_Write();

    // if not "JENNY", then Play_Intro()

    GAME_LoadMainImages();
    GAME_Load_TERRSTAT_0();
    GAME_Load_SPELLDAT_0();

    // GAME_PrepareCredits();

    VGA_DrawFilledRect(0, 0, 319, 199, 0);  // ~= Clear Screen, on Draw Screen-Page
    VGA_SetDirectDraw();
    VGA_DrawFilledRect(0, 0, 319, 199, 0);  // ~= Clear Screen, on Render Screen-Page
    VGA_Set_DSP_Addr();

    // SND_Stop_Music(); MainMenu_Music_Seg = LBXE_LoadSingle("music", 104); if (Settings.BG_Music == 1) { SND_PlayFile(MainMenu_Music_Seg); }
    
    VGA_LoadPalette(0, -1, 0);  // EMPERATO
    VGA_DAC_Write();
    
    // GAME_MainMenu();  // MGC_DEF.H  _s01p03c.c
    
    // SAVEGAM_Save(8)
    // Quit("Thank you for playing Master of Magic!$");

}

void STGE_Init_Draw(void)
{

    dlvfprintf("DEBUG: [%s, %d] g_EMM_Pages_Reserved = EMM_PAGES_REQUIRED<%d>\n", __FILE__, __LINE__, EMM_PAGES_REQUIRED);
    g_EMM_Pages_Reserved = EMM_PAGES_REQUIRED;

    dlvfprintf("DEBUG: [%s, %d] EMM_SetMinKB(EMM_MIN_KB<%d>)\n", __FILE__, __LINE__, EMM_MIN_KB);
    EMM_SetMinKB(EMM_MIN_KB);

    dlvfprintf("DEBUG: [%s, %d] RAM_SetMinKB(RAM_MIN_KB<%d>)\n", __FILE__, __LINE__, RAM_MIN_KB);
    RAM_SetMinKB(RAM_MIN_KB);

    // Hardware_Init(1, 2, GAME_FONT_FILE, 0, 0, 0, 0, 0, 0, 0, 0);  // Defaults for 'No Sound'
        // |-> EMM_Startup();  VGA_SetModeY();  VGA_DAC_Init();  SND_Init();  GUI_Init();  RNG_TimerSeed();  VGA_SetDrawFrame();
    HERE("CALL: EMM_Startup()");
    EMM_Startup();
    HERE("CALL: VGA_SetModeY()");
    VGA_SetModeY();
    HERE("CALL: VGA_DAC_Init(DEFAULT_FONTS_FILE)");
    VGA_DAC_Init(DEFAULT_FONTS_FILE);
    HERE("CALL: GUI_Init(1)");
    IN_Init(1);
    // HERE("RNG_TimerSeed()");
    // RNG_TimerSeed();
    HERE("VGA_SetDrawFrame()");
    VGA_Set_DSP_Addr();

    // DBG_Disable();

    // LBX_Tables_Init(6100);

    // GUI_SetEscOverride();

    // VGA_LoadPalette(0, -1, 0);
    // VGA_DAC_Write();

    // if not "JENNY", then Play_Intro()

    // GAME_LoadMainImages();
    // GAME_Load_TERRSTAT_0();
    // GAME_Load_SPELLDAT_0();

    // GAME_PrepareCredits();

    // VGA_DrawFilledRect(0, 0, 319, 199, 0);  // ~= Clear Screen, on Draw Screen-Page
    // VGA_SetDirectDraw();
    // VGA_DrawFilledRect(0, 0, 319, 199, 0);  // ~= Clear Screen, on Render Screen-Page
    // VGA_SetDrawFrame();

    // SND_Stop_Music(); MainMenu_Music_Seg = LBXE_LoadSingle("music", 104); if (Settings.BG_Music == 1) { SND_PlayFile(MainMenu_Music_Seg); }
    
    // VGA_LoadPalette(0, -1, 0);  // EMPERATO
    // VGA_DAC_Write();
    
    // GAME_MainMenu();  // MGC_DEF.H  _s01p03c.c
    
    // SAVEGAM_Save(8)
    // Quit("Thank you for playing Master of Magic!$");

}
