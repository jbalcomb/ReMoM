// _s33p14c.c MOUSE_CDraw_Restore
// ST_GUI.H

#include "ST_GUI.H"


void MD_CDraw_Restore(void)
{
    g_MD_CursorDraw = g_MD_CDraw_Save;
}
