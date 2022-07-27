// _s33p13c.c MOUSE_Disable_CDraw
// ST_GUI.H

#include "ST_GUI.H"


void MD_CDraw_Disable(void)
{
    g_MD_CDraw_Save = g_MD_CursorDraw;
    g_MD_CursorDraw = 0;
}
