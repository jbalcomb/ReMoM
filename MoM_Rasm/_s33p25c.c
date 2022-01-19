// _s33p25c.c MOUSE_GetClickRec1
// ST_GUI.H

#include "ST_GUI.H"


int MOUSE_GetClickRec1(void)
{
    int tmp_MOUSE_ClickRec1;

    tmp_MOUSE_ClickRec1 = g_MOUSE_ClickRec1;

    g_MOUSE_ClickRec1 = 0;

    return tmp_MOUSE_ClickRec1;
}
