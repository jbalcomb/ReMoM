// _s34p66c.c GUI_GetInput()
// ST_GUI.H

#include "ST_GUI.H"

#include "STU_DBG.H"

/*

Return Value:
    ? Signed / Unsigned ?
        ...depends on IN_ProcessInput() s34p01
e.g.,
    SCREEN_Menu() expects the return value to match the control index of one of the controls that it created
    So, one function creates controls to be selected, either for the mouse or the keyboard, with the mouse x, y, and button or the keyboard key character.
    Then, this function returns the index of the control that was found to fit the actual user input.

*/

int IN_GetInput(void)
{
    int input;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: GUI_GetInput()\n", __FILE__, __LINE__);
#endif

    if ( g_IN_Skip > 0 )
    {
        HERE("if ( g_GUI_Delay > 0 )");
        g_IN_Skip -= 1;
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: GUI_GetInput(void) { input=0 }\n", __FILE__, __LINE__);
#endif
        return 0;
    }

    if ( g_IN_Skip < 0 )
    {
        HERE("if ( g_GUI_Delay < 0 )");
        g_IN_Skip = 0;
    }

    // TODO(JimBalcomb,20220727): figure out the logic here of skipping input on CtrlCnt<=1
    if ( g_CTRL_Control_Count <= 1 )
    {
        HERE("if ( g_CTRL_Control_Count <= 1 )");
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

    input = IN_ProcessInput();  // _s34p01c.c
//    input = 5;  // DEBUG: return GUI_Quit_Lbl_Index: 5
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] input: %d\n", __FILE__, __LINE__, input);
#endif

    VGA_Set_DSP_Addr();

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: GUI_GetInput() { input=%d }\n", __FILE__, __LINE__, input);
#endif
    return input;
}
