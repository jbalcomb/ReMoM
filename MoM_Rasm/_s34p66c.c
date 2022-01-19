// _s34p66c.c GUI_GetInput()
// ST_GUI.H

#include "ST_GUI.H"

#include "STU_DBG.H"

/*

Return Value:
    ? Signed / Unsigned ?
        ...depends on GUI_ProcessInput()
e.g.,
    SCREEN_Menu() expects the return value to match the control index of one of the controls that it created
    So, one function creates controls to be selected, either for the mouse or the keyboard, with the mouse x, y, and button or the keyboard key character.
    Then, this function return the index of the control that was found to fit the actual user input.

*/

int GUI_GetInput(void)
{
    int input;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: GUI_GetInput()\n", __FILE__, __LINE__);
#endif

    if ( g_GUI_Delay > 0 )
    {
        HERE("if ( g_GUI_Delay > 0 )");
        g_GUI_Delay -= 1;
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: GUI_GetInput(void) { input=0 }\n", __FILE__, __LINE__);
#endif
        return 0;
    }

    if ( g_GUI_Delay < 0 )
    {
        HERE("if ( g_GUI_Delay < 0 )");
        g_GUI_Delay = 0;
    }

    if ( g_GUI_Control_Count <= 1 )
    {
        HERE("if ( g_GUI_Control_Count <= 1 )");
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: GUI_GetInput(void) { input=0 }\n", __FILE__, __LINE__);
#endif
        return 0;
    }

    // TODO(JimBalcomb): Deprecated - Keyboard Only
    // if ( g_MOUSE_Hardware == 0 )
    // {
    //     input = CRP_GUI_KeyInputOnly();
    // }
    // else
    // {
    //     input = GUI_ProcessInput();
    // }

    input = GUI_ProcessInput();  // _s34p01c.c
//    input = 5;  // DEBUG: return GUI_Quit_Lbl_Index: 5
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] input: %d\n", __FILE__, __LINE__, input);
#endif

    VGA_SetDrawFrame();

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: GUI_GetInput() { input=%d }\n", __FILE__, __LINE__, input);
#endif
    return input;
}
