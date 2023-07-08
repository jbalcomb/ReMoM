
/*
SEEALSO: C:\devel\STU-MoM_Rasm\doc\MoM-Init.md
SEEALSO: C:\devel\STU-MoM_Rasm\doc\MoM-Init-MGC.md
SEEALSO: C:\devel\STU-MoM_Rasm\doc\MoM-Init-WZD.md
SEEALSO: C:\STU-DASM\drake178\__MGC_main.asm
SEEALSO: C:\STU-DASM\drake178\__WZD_main.asm
*/

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
    dbg_prn("DEBUG: [%s, %d] BEGIN: MGC_main(argc = %d, argv[0] = %s)\n", __FILE__, __LINE__, argc, argv[0]);
#endif
    
    EMM_Pages_Reserved = EMM_PAGES_REQUIRED;

    // Load_CONFIG_MOM();
    // Load_MAGIC_SET();

    /*
        MIDI Driver
        DSP Driver
    */

    /*
        Check Saved Game Files
            magic_set.Using_Saves
            magic_set.Save_Names
    */

    EMM_SetMinKB(EMM_MIN_KB);
    // RAM_SetMinKB(RAM_MIN_KB);
    MoX_RAM_SetMinKB(RAM_MIN_KB);

    Hardware_Init(1, GAME_FONT_FILE);
    /*
        |-> EMM_Startup()
        |-> VGA_SetModeY()
        |-> Load_Font_File() |-> ... LBX_Load(), Allocate_Space_No_Header(); ... TextLine_Init()
        |-> SND_Init()
        |-> IN_Init() |-> MD_Init()
        |-> RND_TimerSeed()
        |-> VGA_Set_DSP_Addr()
    */

    // DBG_Disable();
    // Set_Release_Version();

    MoM_Tables_Init(6100);
    
    IN_SetEscOverride();

    PAL_Load_Palette(0, -1, 0);
    VGA_DAC_Write();

    /*
        if not "JENNY", then Play_Intro()
    */

    // MAINMENU  GAME_LoadMainImages();  // MGC s01p04
    Main_Menu_Load_Pictures();
    // ST_NEWG  GAME_Load_TERRSTAT_0();
    // ST_NEWG  GAME_Load_SPELLDAT_0();

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



    MGC_Screen_Control();



    // Save_SAVE_GAM(8);
    // Exit_With_Size();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: MGC_main(argc = %d, argv[0] = %s)\n", __FILE__, __LINE__, argc, argv[0]);
#endif
    return 0;
}
