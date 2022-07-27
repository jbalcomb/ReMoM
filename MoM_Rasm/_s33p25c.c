// _s33p25c.c MOUSE_GetClickRec1
// ST_GUI.H

#include "ST_GUI.H"


int MD_GetClickRec1(void)
{
    int tmp_MD_ClickRec1;

    tmp_MD_ClickRec1 = g_MD_ClickRec1;

    g_MD_ClickRec1 = 0;

    return tmp_MD_ClickRec1;
}
