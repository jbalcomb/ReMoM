#ifndef SEG020_H
#define SEG020_H


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

/*
    seg020
*/

// s20p01
void VGA_LoadPalette(int Palette_Index, int First_Color, int Last_Color);

// s20p05
void VGA_SetDACChanged(int First_Color, int Last_Color);


#ifdef __cplusplus
}
#endif


#endif /* SEG020_H */
