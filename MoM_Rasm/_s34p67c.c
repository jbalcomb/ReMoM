// _s34p66c.c GUI_SetDelay
// ST_GUI.H

#include "ST_GUI.H"


void IN_Set_Skip(int Input_Skip)
{
    g_IN_Skip = Input_Skip;

    /* clear Click-Record 1 and Click-Record 2 */
    MD_GetClickRec1();
    CRP_MD_GetClickRec2();
}
