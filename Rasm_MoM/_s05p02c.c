// _s05p02.asm GAME_QuitProgram
// in MOM_DEF.H

#include "MOM_DEF.H"

// IDA: int __cdecl __far GAME_QuitProgram(char *argQuitMessage)
void GAME_QuitProgram(char *argQuitMessage)
{
    printf("DEBUG: BEGIN: GAME_QuitProgram()\n");
    
    // printf("DEBUG: argQuitMessage:  [%p]  %s\n", argQuitMessage, argQuitMessage);
    // //wait_for_any_key();

    //SND_Stop_Music
    //MOUSE_Reset
    //SND_Shutdown
    //EMM_ReleaseAll
    //CRP_Empty_Exit_Fn3
    
    // [x] VGA_SetTextMode();  // totally works, but blanks the screen, so you can see the previous printf-style debug output

    //DOS_QuitWithMsg("argQuitMessage"); // works
    //DOS_QuitWithMsg(argQuitMessage); works when called directly with a char*
    //DOS_QuitWithMsg(argQuitMessage); works when called through GAME_QuitProgram with ""
    //DOS_QuitWithMsg(argQuitMessage); // prints garbage when called through GAME_QuitProgram with char*
    //DOS_QuitWithMsg(&argQuitMessage); // prints garbage
    //DOS_QuitWithMsg(*argQuitMessage); // Error Type mismatch  
    DOS_QuitWithMsg(argQuitMessage);  // works, on account of now being in C, and using printf()

    printf("DEBUG: END: GAME_QuitProgram()\n");
}
