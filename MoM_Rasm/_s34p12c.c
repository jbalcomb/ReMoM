// _s34p12c.c GUI_SetHelp
// ST_GUI.H

#include "ST_GUI.H"


// void GUI_SetHelp(HLP_ENTRY *Entry_Tbl, int Entry_Count)
void HLP_SetHelp(int Entry_Tbl, int Entry_Count)
{
    g_HLP_Entry_Table = Entry_Tbl;
    g_GUI_Help_Available = 1;  // ST_TRUE
    g_HLP_Entry_Count = Entry_Count;
}
