/**
 * win_Exit.c — Win32 exit-with-message.
 */

#include <Windows.h>

#include "../../platform/include/Platform.h"
#include "win_PFL.h"



/* WZD s05p02 */
void MWA_Exit_With_Message(char *string)
{
    MessageBoxA(win_window, string, "Allocation_Error", MB_OKCANCEL | WS_POPUP);
    PostMessage(win_window, WM_CLOSE, 0, 0);
}
