
#include <Windows.h>

#include "MOX/MOX_TYPE.h"
#include "MOX/Mouse.h"
#include "MOX/SOUND.h"

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
    Stop_Music__STUB();

    Reset_System_Mouse();

    Audio_Uninit__STUB();

    // TODO  EMM_ReleaseAll();

    // TODO  DBG_Close_ERROR_LOG();

    // TODO  VGA_SetTextMode();
    
    // TODO  Quit_With_Message(string);

    MessageBoxA(g_Window, string, "Allocation_Error", MB_OKCANCEL | WS_POPUP);
    PostMessage(g_Window, WM_CLOSE, 0, 0);

}
