
#include "MoX_Fonts.H"  /* p_PaletteFlags */

// s20p05   VGA_SetDACChanged()
// MoO2 Module: palette Set_Palette_Changes()
void Set_Palette_Changes(int start_color, int end_color)
{
    int i;

    printf("BEGIN: Set_Palette_Changes()\n");

    for ( i = start_color; i < end_color; i++)
    {
        *(p_PaletteFlags + i) = 1;
    }

    printf("END: Set_Palette_Changes()\n");

}
