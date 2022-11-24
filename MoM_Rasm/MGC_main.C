
#include "MGC_main.H"

#include "MAINMENU.H"   /* Main_Menu_Screen_Control() */

#include "MOM_DEF.H"    /* EMM_PAGES_REQUIRED, EMM_MIN_KB, RAM_MIN_KB, GAME_FONT_FILE; */

#include "ST_EMM.H"     /* EMM_Pages_Reserved */

#include <STDIO.H>      /* FILE */

/*
    MAGIC.EXE seg001
*/
// MGC s01p01
int MGC_main(int argc, char *argv[])
{
    char Temp_String[30];
    char File_Name[40];
    FILE * fileptr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: MGC_main()\n", __FILE__, __LINE__);
#endif
    
    EMM_Pages_Reserved = EMM_PAGES_REQUIRED;

    // Load_CONFIG_MOM();
    // Load_MAGIC_SET();

    /*
        MIDI Driver
        DSP Driver
    */

    /*
        Saved Game Files
    */

    EMM_SetMinKB(EMM_MIN_KB);
    RAM_SetMinKB(RAM_MIN_KB);

    Hardware_Init(1, GAME_FONT_FILE);
    /*
        |-> EMM_Startup()
        |-> VGA_SetModeY()
        |-> VGA_DAC_Init() |-> ... LBXE_LoadSingle(), SA_Allocate_MemBlk(); ... VGA_TextDraw_Init()
        |-> SND_Init()
        |-> IN_Init() |-> MD_Init()
        |-> RNG_TimerSeed()
        |-> VGA_Set_DSP_Addr()
    */

    DBG_Disable();

    MoM_Tables_Init(6100);
    
    IN_SetEscOverride();

    PAL_Load_Palette(0, -1, 0);
    VGA_DAC_Write();

    /*
        if not "JENNY", then Play_Intro()
    */

    GAME_LoadMainImages();
    GAME_Load_TERRSTAT_0();
    GAME_Load_SPELLDAT_0();

    // Load_Credits();

    VGA_DrawFilledRect(0, 0, 319, 199, 0);  // ~= Clear Screen, on Draw Screen-Page
    VGA_SetDirectDraw();
    VGA_DrawFilledRect(0, 0, 319, 199, 0);  // ~= Clear Screen, on Render Screen-Page
    VGA_Set_DSP_Addr();

    /*
        Play Main Menu Music
    */

    PAL_Load_Palette(0, -1, 0);  // EMPERATO
    VGA_DAC_Write();



    // GAME_MainMenu();  // MGC s01p03
    Main_Menu_Screen_Control();



    // Save_SAVE_GAM(8);
    // Exit_With_Size();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: MGC_main()\n", __FILE__, __LINE__);
#endif
    return 0;
}
