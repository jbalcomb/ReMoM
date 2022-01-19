// _s34p79c.c GUI_DisableRedraw
// ST_GUI.H

#include "ST_GUI.H"


void GUI_DisableRedraw(void)
{
    g_GUI_RedrawFn_Present = 0;
}
