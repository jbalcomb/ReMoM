#ifndef MOX_FONTS_H
#define MOX_FONTS_H

#include "MoX_TYPE.H"       /* byte_ptr, SAMB_ptr */


extern SAMB_ptr p_Palette;
extern byte_ptr p_PaletteFlags;


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

/*
    seg020
*/

// MGC  s20p01
// MoO2  Load_Palette()
// void PAL_Load_Palette(int entry, int start_color, int end_color);
// void Load_Palette(int entry, int start_color, int end_color);

// MGC  s20p05
// MoO2  Set_Palette_Changes()
// void PAL_Set_PaletteFlags(int start_color, int end_color);
// void Set_Palette_Changes(int start_color, int end_color);


#ifdef __cplusplus
}
#endif



#endif MOX_FONTS_H
