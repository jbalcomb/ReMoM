#ifndef SEG020_H
#define SEG020_H

#include "ST_TYPE.H"    /* SAMB_ptr */


extern SAMB_ptr palette_block;


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

/*
    seg020
*/

// s20p01
// MoO2  Load_Palette()
void PAL_Load_Palette(int entry, int start_color, int end_color);

// s20p05
// MoO2  Set_Palette_Changes()
void PAL_Set_PaletteFlags(int start_color, int end_color);


#ifdef __cplusplus
}
#endif


#endif /* SEG020_H */
