// _s33p01c.c GUI_SetWindows
// in ST_GUI.H

#include "ST_GUI.H"

void GUI_SetWindows(int argWindowCount, struct s_GUI_WINDOW *argWindowArray)
{
    int tmpWindowCount;
    struct s_GUI_WINDOW *tmpGuiWindow;

//    printf("DEBUG: %s %d BEGIN: GUI_SetWindows()\n", __FILE__, __LINE__);

    /*
    printf("DEBUF: argWindowCount: %d\n", argWindowCount);
    printf("DEBUF: argWindowArray: %d\n", argWindowArray);
    printf("DEBUG: argWindowArray[0].Cursor_Index: %d\n", argWindowArray[0].Cursor_Index);
    printf("DEBUG: argWindowArray[0].Cursor_Offset: %d\n", argWindowArray[0].Cursor_Offset);
    printf("DEBUG: argWindowArray[0].Left: %d\n", argWindowArray[0].Left);
    printf("DEBUG: argWindowArray[0].Top: %d\n", argWindowArray[0].Top);
    printf("DEBUG: argWindowArray[0].Right: %d\n", argWindowArray[0].Right);
    printf("DEBUG: argWindowArray[0].Bottom: %d\n", argWindowArray[0].Bottom);
    */

    //*tmpGuiWindow = argWindowArray[0];
    tmpGuiWindow = argWindowArray;

    tmpWindowCount = argWindowCount;
    g_GUI_WindowCount = tmpWindowCount;
    g_GUI_Windows = argWindowArray;
    tmpWindowCount--;

    // tmpGuiWindow = tmpGuiWindow + (tmpWindowCount * sizeof(s_GUI_WINDOW))
    // tmpGuiWindow = argWindowArray[tmpWindowCount * sizeof(s_GUI_WINDOW]
    // tmpGuiWindow = argWindowArray[tmpWindowCount]
    if ( tmpWindowCount != 0 )
    {
        *tmpGuiWindow = argWindowArray[tmpWindowCount];
        do
        {
            if ( (tmpGuiWindow->Left == 0) && (tmpGuiWindow->Top == 0) )
            {
                break;
            }

            *tmpGuiWindow = argWindowArray[tmpWindowCount];
            tmpGuiWindow--;
            tmpWindowCount--;
        } while ( tmpWindowCount != 0 );
    }

// mov  ax, [bx+GUI_WINDOW.Cursor_Offset]
// mov  [GUI_CursorOffset], ax
// mov  ax, [bx+GUI_WINDOW.Cursor_Index]
// mov  [GUI_CurrentCursor], ax
//    printf("DEBUG: %s %d END: GUI_SetWindows()\n", __FILE__, __LINE__);
}
