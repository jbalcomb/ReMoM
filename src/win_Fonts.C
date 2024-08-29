
#include "MoX.H"

// TODO  turn this into MoX64 or somesuch; not PFL, just not OG/DOS/16-bit/6-bpp (VGA)

/*
    WIZARDS.EXE  seg021
*/

// WZD s21p01
// MoO2: Refresh_Palette |-> Store_Palette_Block_
// 1oom :: uipal.c :: void ui_palette_set_n(void)
// AKA VGA_DAC_Write()
// IBM-PC, VGA, MS-DOS: write to the VGA-DAC
// MS-Windows: write to PFL-provded palette buffer
void Apply_Palette(void)
{

    int16_t itr;

    for(itr = 0; itr < 256; itr++)  // TODO  ~ #define Color Count
    {
        if( *(p_Palette + 768 + itr) == 1 )  // TODO  ~ #define Palette Flags Offset
        {
            *(PFL_Palette + (itr * 4) + 3) = 0x00;
            *(PFL_Palette + (itr * 4) + 2) = (*(p_Palette + (itr * 3) + 0) << 2);
            *(PFL_Palette + (itr * 4) + 1) = (*(p_Palette + (itr * 3) + 1) << 2);
            *(PFL_Palette + (itr * 4) + 0) = (*(p_Palette + (itr * 3) + 2) << 2);
        }
    }

}
