// _s33p13c.c MOUSE_Disable_CDraw
// ST_GUI.H

#include "ST_GUI.H"


void MOUSE_Disable_CDraw(void)
{
    g_MOUSE_CDraw_Save = g_MOUSE_CursorDraw;
    g_MOUSE_CursorDraw = 0;
}
