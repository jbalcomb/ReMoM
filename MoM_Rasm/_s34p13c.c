// _s34p13c.c GUI_ClearHelp()
// ST_GUI.H

#include "ST_GUI.H"


void HLP_ClearHelp(void)
{
    g_GUI_Help_Available = 0;  // ST_FALSE
    g_HLP_Entry_Count = 0;
    g_HLP_Entry_Table = 0;
}
