/*
    WIZARDS.EXE
        seg023

    Moo2
        Module: special
            Interpolate_Line()
            Absolute_Interpolate_Line()
            Range()
            Delta_XY_With_Wrap()
            Path()
            Path_Wrap()
            isqrt()

*/

#include "MoX.H"



/*
    WIZARDS.EXE seg023
*/

// WZD s23p01
// UU_VGA_GetPointInDir()

// WZD s23p02
// TILE_GetDistance()

// WZD s23p03
/*
    calculate the distance between {x1,y1} and {x2,y2}

*/
int16_t Delta_XY_With_Wrap(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t wrap_x)
{
    int16_t delta_x1;
    int16_t delta_x2;
    int16_t delta_y;
    int16_t range;

    delta_x1 = x2 - x1;
    delta_y = y2 - y1;

    // TODO  AbsVal(delta_x1);
    if(delta_x1 < 0)
    {
        delta_x1 *= -1;
    }

    if(delta_y < 0)
    {
        delta_y *= -1;
    }

    if(x2 < x1)
    {
        delta_x2 = x2 + wrap_x - x1;
    }
    else
    {
        delta_x2 = x2 - wrap_x - x1;
    }

    if(delta_x2 < 0)
    {
        delta_x2 *= -1;
    }

    if(delta_x1 > delta_x2)
    {
        delta_x1 = delta_x2;
    }
    
    if(delta_x1 > delta_y)
    {
        range = delta_x1;
    }
    else
    {
        range = delta_y;
    }

    return range;
}


// WZD s23p04
// UU_Tile_PlotLine()

// WZD s23p05
// TILE_PlotMapLine()
