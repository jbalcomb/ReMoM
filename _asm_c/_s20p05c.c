// _s20p05c.c VGA_SetDACChanged
// ST_VGA.H

#include "ST_VGA.H"

void VGA_SetDACChanged(int First_Color, int Last_Color)
{
    int itr_colors;

    for ( itr_colors = First_Color; itr_colors < Last_Color; itr_colors++)
    {
        farpokeb(gsa_PaletteFlags, itr_colors, 1);
    }
}
