// _s34p79c.c GUI_DisableRedraw
// ST_GUI.H

#include "ST_GUI.H"
#include "ST_SCRN.H"


void SCRN_DisableRedraw(void)
{
    g_SCRN_RedrawFn_Present = 0;
}
