// _s33p01c.c GUI_SetWindows
// in ST_GUI.H

#include "ST_GUI.H"

void GUI_SetWindows(int argWindowCount, struct GUI_WINDOW *argWindowArray)
{
    int tmpWindowCount;
    struct GUI_WINDOW *tmpGuiWindow;
    *tmpGuiWindow = argWindowArray;

    tmpWindowCount = argWindowCount;
    g_GUI_WindowCount = tmpWindowCount;
    g_GUI_Windows = argWindowArray;
    tmpWindowCount--;

    // tmpGuiWindow = tmpGuiWindow + (tmpWindowCount * sizeof(GUI_WINDOW))
    // tmpGuiWindow = argWindowArray[tmpWindowCount * sizeof(GUI_WINDOW]
    // tmpGuiWindow = argWindowArray[tmpWindowCount]
    if ( tmpWindowCount != 0 )
    {
        tmpGuiWindow = argWindowArray[tmpWindowCount];
        do
        {
            if ( (tmpGuiWindow.Left == 0) && (tmpGuiWindow.Top == 0)
            {
                break;
            }

            tmpGuiWindow = argWindowArray[tmpWindowCount]
            tmpGuiWindow--;
            tmpGuiWindowCount--;
        } while ( tmpGuiWindowCount != 0 )
    }

mov  ax, [bx+GUI_WINDOW.Cursor_Offset]
mov  [GUI_CursorOffset], ax
mov  ax, [bx+GUI_WINDOW.Cursor_Index]
mov  [GUI_CurrentCursor], ax

}
