// _s34p57c.c GUI_Clear
// ST_GUI.H

#include "ST_GUI.H"


void CTRL_Clear(void)
{
    g_CTRL_Control_Count = 1;
    g_CTRL_Focused = -1;          // NONE/UNDEFINED
    g_CTRL_MouseFocusCtrl = 0;    // FALSE
    g_CTRL_InEditSelect = 0;      // FALSE
    g_CTRL_Active_EditSlct = -1;  // NONE/UNDEFINED
}
