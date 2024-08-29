
#include <Windows.h>

extern HWND g_Window;



/*
    Exit
*/

// WZD s05p02
/*
    MoO2
        Reset_System();
        exit(EXIT_FAILURE);
*/
void MWA_Exit_With_Message(char * string)
{
    // TODO  SND_Stop_Music();
    // TODO  Reset_System_Mouse();
    // TODO  SND_Shutdown();
    // TODO  EMM_ReleaseAll();
    // TODO  s06p06_Empty_pFxn();
    // TODO  VGA_SetTextMode();
    
    // TODO  Quit_With_Message(string);

    MessageBoxA(g_Window, string, "Allocation_Error", MB_OKCANCEL | WS_POPUP);
    PostMessage(g_Window, WM_CLOSE, 0, 0);

}
