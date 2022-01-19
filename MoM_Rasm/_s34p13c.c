// _s34p13c.c GUI_ClearHelp()
// ST_GUI.H

#include "ST_GUI.H"


void GUI_ClearHelp(void)
{
    g_GUI_Help_Available = 0;
    g_HLP_Entry_Count = 0;
    g_HLP_Entry_Table = 0;
}
