// _s14p05c.c VGA_ResetDrawWindow
// MOM_DEF.H

#include "ST_VGA.H"

void VGA_ResetDrawWindow(void)
{
    g_VGA_Min_X = 0;
    g_VGA_Max_X = 319;
    g_VGA_Min_Y = 0;
    g_VGA_Max_Y = 199;
}
