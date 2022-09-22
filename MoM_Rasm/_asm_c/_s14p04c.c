// _s14p04c.c VGA_SetDrawWindow
// MOM_DEF.H

#include "ST_VGA.H"


void VGA_SetDrawWindow(int Min_X, int Min_Y, int Max_X, int Max_Y)
{
    int tmp_Min_X;
    int tmp_Min_Y;
    int tmp_Max_X;
    int tmp_Max_Y;
    int tmp;

    if ( tmp_Min_X < 0 )
    {
        tmp_Min_X = 0;
    }
    if ( tmp_Min_Y < 0 )
    {
        tmp_Min_Y = 0;
    }
    if ( tmp_Max_X > 319 )
    {
        tmp_Min_X = 319;
    }
    if ( tmp_Max_Y > 199 )
    {
        tmp_Min_Y = 199;
    }
    if ( tmp_Min_X > tmp_Max_X )
    {
        tmp = tmp_Max_X;
        tmp_Max_X = tmp_Min_X;
        tmp_Min_X = tmp;
    }
    if ( tmp_Min_Y > tmp_Max_Y )
    {
        tmp = tmp_Max_Y;
        tmp_Max_Y = tmp_Min_Y;
        tmp_Min_Y = tmp;
    }

    g_VGA_Min_X = tmp_Min_X;
    g_VGA_Max_X = tmp_Max_X;
    g_VGA_Min_Y = tmp_Min_Y;
    g_VGA_Max_Y = tmp_Max_Y;

}
