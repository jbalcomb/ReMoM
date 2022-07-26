// _s34p57c.c GUI_Clear
// ST_GUI.H

#include "ST_GUI.H"


void GUI_Clear(void)
{
    g_GUI_Control_Count = 1;
    g_GUI_FocusedControl = -1;  // NONE/UNDEFINED
    g_GUI_MouseFocusCtrl = 0;  // FALSE
    g_GUI_InEditSelect = 0;  // FALSE
    g_GUI_Active_EditSlct = -1;  // NONE/UNDEFINED
}
