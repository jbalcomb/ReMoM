
// Screen_Action
// case 0:  // "Continue"
// case 1:  // "Load Game"
// case 2:  // "New Game"
// case 3:  // "Quit To DOS"
// case 4:  // "Hall Of Fame"
/*
    enum Screens
    MGC has no screen number for the main menu screen
    ? make MGC 1n and WZD 2n, with menu as 0 ?

    MGC & WZD share Load, Settings, and Quit
    what happens in WZD when you click quit on the load screen?
    why the lag in WZD coming back from the load screen?
    ...no such lag in MGC
    ...but, such a lag in MGC coming back from HoF

    MGC & WZD have no screen number for the settings screen


*/

#include "ST_DEF.H"

#include "MAINMENU.H"


/*
    Initialized Data
*/

// MGC  dseg:2880
char quit_message[] = "Thank you for playing Master of Magic!";



// MGC s01p03
// AKA GAME_MainMenu()
// AKA Main_Menu_Screen_Control()
void MGC_Screen_Control(void)
{
    // char Read_Buffer[30];
    int Screen_Action;
    // FILE *fp;
    int flag_quit;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: MGC_Screen_Control()\n", __FILE__, __LINE__);
#endif

    // Load_CONFIG_MOM();

    screen_fade = ST_TRUE;
    main_menu_jump = ST_UNDEFINED;
    IN_SetEscOverride();  // Set_Global_Esc();

    flag_quit = ST_FALSE;
    while(flag_quit == ST_FALSE)
    {
        VGA_SetOutlineColor(0);  // Set_Outline_Color(0);
        HLP_ClearHelp();  // Deactivate_Help_List();
        Screen_Action = Main_Menu_Screen();  // MGC s01p05
        HLP_ClearHelp();  // Deactivate_Help_List();
        screen_fade = ST_FALSE;
        switch(Screen_Action)
        {
            case 0:  // "Continue"
                // SND_Stop_Music()
                // j_GAME_WizardsLaunch(8)  // e_SAVE9GAM
                GUI_LoadSave_State = ST_UNDEFINED;
                break;
            case 1:  // "Load Game"
                // j_GAME_LoadSaveScreen
                break;
            case 2:  // "New Game"
                // j_GAME_New_Create
                GUI_LoadSave_State = ST_UNDEFINED;
                break;
            case 3:  // "Quit To DOS"
                flag_quit = ST_TRUE;
                GUI_LoadSave_State = ST_UNDEFINED;
                break;
            case 4:  // "Hall Of Fame"
                // j_GAME_Hall_of_Fame
                // j_GAME_PrepareCredits
                // jmp  short $+2 ... somehow this jumps back to @@LoopWhileZero
                break;
        }
        
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: MGC_Screen_Control()\n", __FILE__, __LINE__);
#endif

    Exit(quit_message);  // Exit_With_Message(quit_message);
}
