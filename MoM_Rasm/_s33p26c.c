// _s33p26c.c CRP_MOUSE_GetClickRec2
// ST_GUI.H

#include "ST_GUI.H"


int CRP_MOUSE_GetClickRec2(void)
{
    int tmp_MOUSE_ClickRec2;

    tmp_MOUSE_ClickRec2 = g_MOUSE_ClickRec2;

    g_MOUSE_ClickRec2 = 0;

    return tmp_MOUSE_ClickRec2;
}
