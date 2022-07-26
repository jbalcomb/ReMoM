// _s01p03c.c GAME_MainMenu()
// MGC_DEF.H

#include <stdio.h>

#include "MOM_DEF.H"
#include "MGC_DEF.H"

#include "ST_TXT.H"

#include "STU_DBG.H"


void GAME_MainMenu(void)
{
    char Read_Buffer[30];
    int Screen_Action;
    FILE *fp;
    int flag_quit;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: BEGIN: GAME_MainMenu()\n", __FILE__, __LINE__);
#endif

    if ( DISK_FileFind(cnst_ConfigFile, Read_Buffer) == 0 )
    {
        Quit(cnst_ConfigErr);
    }
// #ifdef DEBUG
//     dlvfprintf("DEBUG: [%s, %d] Read_Buffer: %s\n", __FILE__, __LINE__, Read_Buffer);
// #endif

   fp = fopen(cnst_ConfigFile, cnst_RB);
   fread(Config_Data, 18, 1, fp);
   fclose(fp);


   g_ScreenChangeFade = 1;  // ST_TRUE
   g_GUI_MainScreenJump = -1;  // ST_UNDEFINED
   GUI_SetEscOverride();


    // Loop While ? == 0
    // first run through always calls SCREEN_Menu()
    flag_quit = ST_FALSE;

    while ( flag_quit == ST_FALSE )
    {

        VGA_SetOutlineColor(0);
        GUI_ClearHelp();

        HERE("Screen_Action = SCREEN_Menu();");
        Screen_Action = SCREEN_Menu();  // MGC_DEF.H  _s01p05c.c
        // DEBUG(JimBalcomb): here, the screen is rendered correctly - ? happens after input ?
        
#ifdef DEBUG
        //Screen_Action = 3;  // DEBUG: default to `Quit To DOS`
        dlvfprintf("DEBUG: [%s, %d] Screen_Action: %d\n", __FILE__, __LINE__, Screen_Action);
#endif

        GUI_ClearHelp();
        g_ScreenChangeFade = ST_FALSE;

        switch (Screen_Action)
        {
            case 0:  // "Continue"
                dlvfprintf("DEBUG: [%s, %d] Menu Event 0\n", __FILE__, __LINE__);
                // SND_Stop_Music()
                // j_GAME_WizardsLaunch(8)  // e_SAVE9GAM
                GUI_LoadSave_State = ST_UNDEFINED;
                break;
            case 1:  // "Load Game"
                dlvfprintf("DEBUG: [%s, %d] Menu Event 1\n", __FILE__, __LINE__);
                // j_GAME_LoadSaveScreen
                break;
            case 2:  // "New Game"
                dlvfprintf("DEBUG: [%s, %d] Menu Event 2\n", __FILE__, __LINE__);
                // j_GAME_New_Create
                GUI_LoadSave_State = ST_UNDEFINED;
                break;
            case 3:  // "Quit To DOS"
                dlvfprintf("DEBUG: [%s, %d] Menu Event 3\n", __FILE__, __LINE__);
                flag_quit = ST_TRUE;
                GUI_LoadSave_State = ST_UNDEFINED;
                break;
            case 4:  // "Hall Of Fame"
                dlvfprintf("DEBUG: [%s, %d] Menu Event 4\n", __FILE__, __LINE__);
                // j_GAME_Hall_of_Fame
                // j_GAME_PrepareCredits
                // jmp  short $+2 ... somehow this jumps back to @@LoopWhileZero
                break;
        }
        
    }

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: END: GAME_MainMenu()\n", __FILE__, __LINE__);
#endif
    Quit(cnst_QUIT_Message);
}
