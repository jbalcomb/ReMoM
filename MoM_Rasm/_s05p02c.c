// _s05p02.asm Quit
// MOM_DEF.H

#include "MOM_DEF.H"

#include "ST_EMM.H"
#include "ST_GUI.H"

#include "STU_DBG.H"


// IDA: int __cdecl __far GAME_QuitProgram(char *argQuitMessage)
void Quit(char *argQuitMessage)
{
//    dlvfprintf("DEBUG: BEGIN: Quit()\n");
    
    // printf("DEBUG: argQuitMessage:  [%p]  %s\n", argQuitMessage, argQuitMessage);
    // //wait_for_any_key();

    //SND_Stop_Music

    MOUSE_Reset();

    //SND_Shutdown

    EMM_Shutdown();

    //CRP_Empty_Exit_Fn3

    // TODO(JimBalcomb): add DrawText("Press any key to continue...")
    getch();  // DEBUG(JimBalcomb): suspend operations, so we can have a look-see at the resultant graphics-mode screen
    HERE("VGA_SetTextMode();");
    VGA_SetTextMode();

    //DOS_QuitWithMsg("argQuitMessage"); // works
    //DOS_QuitWithMsg(argQuitMessage); works when called directly with a char*
    //DOS_QuitWithMsg(argQuitMessage); works when called through GAME_QuitProgram with ""
    //DOS_QuitWithMsg(argQuitMessage); // prints garbage when called through GAME_QuitProgram with char*
    //DOS_QuitWithMsg(&argQuitMessage); // prints garbage
    //DOS_QuitWithMsg(*argQuitMessage); // Error Type mismatch  

//    dlvfprintf("DEBUG: END: Quit()\n");
    QuitToDOS(argQuitMessage);  // works, on account of now being in C, and using printf()
}
