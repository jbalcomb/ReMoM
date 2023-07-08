
/*
SEEALSO: C:\devel\STU-MoM_Rasm\doc\MoM-Init.md
SEEALSO: C:\devel\STU-MoM_Rasm\doc\MoM-Init-MGC.md
SEEALSO: C:\devel\STU-MoM_Rasm\doc\MoM-Init-WZD.md
SEEALSO: C:\STU-DASM\drake178\__MGC_main.asm
SEEALSO: C:\STU-DASM\drake178\__WZD_main.asm
*/

/*
    WIZARDS.EXE seg001
*/
// WZD s01p01
int WZD_main(int argc, char *argv[])
{
    char Temp_String[30];
    char File_Name[40];
    FILE * fileptr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: WZD_main(argc = %d, argv[0] = %s)\n", __FILE__, __LINE__, argc, argv[0]);
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

    DBG_Disable();

    IN_SetEscOverride();

    // WZD_Tables_Init(4600);

    // Load_SAVE_GAM(8);

    // LBX_Tables_LoadMain()

    /*
        EMM_Unallocated_Pages
        ehn_OVERXYZ
        __OvrInitEms()
    */

    // LoadPalette(0, -1, 0);
    // VGA_SetShades_Grey0();
    // GUI_SetBtnClickDir(1, 1);
    // VGA_SetBlinkColor(198, 40, 0, 0, 63, 0, 0, 1);
    // VGA_DAC_Write();
    // VGA_Fade_In();

    // _current_screen == e_Main_Screen;

    // TBL_Wizards.Banner+17E8h = BNR_Brown;

    // Clear_Fields();

    // GAME_Startup()

    // GAME_SoM_Cast_By = -1;



    WZD_Screen_Control();



    // RP_Empty_Exit_Fn1()
    // Quit_With_Size();


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: WZD_main(argc = %d, argv[0] = %s)\n", __FILE__, __LINE__, argc, argv[0]);
#endif
    return 0;
}
