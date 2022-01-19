// _s34p66c.c GUI_SetDelay
// ST_GUI.H

#include "ST_GUI.H"


void GUI_SetDelay(int Input_Delay)
{
    g_GUI_Delay = Input_Delay;

    /* clear Click-Record 1 and Click-Record 2 */
    MOUSE_GetClickRec1();
    CRP_MOUSE_GetClickRec2();
}
