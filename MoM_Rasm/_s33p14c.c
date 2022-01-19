// _s33p14c.c MOUSE_CDraw_Restore
// ST_GUI.H

#include "ST_GUI.H"


void MOUSE_CDraw_Restore(void)
{
    g_MOUSE_CursorDraw = g_MOUSE_CDraw_Save;
}
