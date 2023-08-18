#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "MoX_TYPE.H"



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


// WZD s14p04
// MoO2  Module: graphics  Set_Window(signed integer (2 bytes) x1, signed integer (2 bytes) y1, signed integer (2 bytes) x2, signed integer (2 bytes) y2)
void Set_Window(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

// WZD s14p05
// MoO2  Module: graphics  Reset_Window()
void Reset_Window(void);

// WZD s14p06
// drake178: VGA_WndDrawFilldRect
void Clipped_Fill(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int8_t color);


/*
    WIZARDS.EXE
    seg016
*/

// WZD s16p01
void Fill(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int8_t color);

// WZD s16p02
// VGA_PutPixel

// WZD s16p03
// drake178: VGA_DrawLine()
// AKA VGA_Draw_Line()
void Line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int8_t color);

// WZD s16p04
// VGA_DrawPatternLine

// WZD s16p05
// UU_VGA_CreateColorWave

// WZD s16p06
// UU_VGA_DiagColumns

// WZD s16p07
// RNG_Direct_LFSR

// WZD s16p08
// VGA_Shade_Rect

// WZD s16p09
// VGA_Grayscale_Rect

// WZD s16p10
// UU_VGA_Columns

// WZD s16p11
// UU_VGA_ScrambleRect



#ifdef __cplusplus
}
#endif

#endif  /* GRAPHICS_H */
