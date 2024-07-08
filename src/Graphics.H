/*
    WIZARDS.EXE
        seg014
        seg015
        seg016

MoO2:
Module: init
Module: graphcis
Module: regions
Module: shear
*/
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "MoX_TYPE.H"



#ifdef __cplusplus
extern "C" {
#endif



// WZD dseg:76DA                                                 BEGIN : ovr014
// WZD dseg:76DA                                                 BEGIN : graphics

// WZD dseg:76DA
extern int16_t screen_window_x1;
// WZD dseg:76DC
extern int16_t screen_window_y1;
// WZD dseg:76DE
extern int16_t screen_window_x2;
// WZD dseg:76E0
extern int16_t screen_window_y2;

// WZD dseg:76E0                                                 END : graphics
// WZD dseg:76E2                                                 � Init ?





// WZD s14p01
// MoO2  Module: init  void Init_Drivers()
/*  Init.C */  // void Init_Drivers(int input_type, char * font_file);

// WZD s14p02
// MoO2  DNE
/* Init.C */  // void UU_Legacy_Startup(int input_type, int midi_driver, int sound_channels, char * font_file)

// WZD s14p03
// drake178: 
// MoO2  
// Load_Font_File()

// WZD s14p04
// drake178: 
// MoO2  Module: graphics  Set_Window(signed integer (2 bytes) x1, signed integer (2 bytes) y1, signed integer (2 bytes) x2, signed integer (2 bytes) y2)
void Set_Window(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

// WZD s14p05
// drake178: 
// MoO2  Module: graphics  Reset_Window()
void Reset_Window(void);

// WZD s14p06
// drake178: VGA_WndDrawFilldRect
void Clipped_Fill(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int8_t color);

// WZD s14p07
// drake178: UU_VGA_WndPutPixel()
// MoO2  Module: fields  Clipped_Dot()
void Clipped_Dot(int16_t x, int16_t y, int16_t color);

// WZD s14p08
// drake178: 
// MoO2  
// VGA_WndDrawLine()

// WZD s14p09
// drake178: 
// MoO2  
// VGA_WndDrawPtrnLine()

// WZD s14p10
// drake178: 
// MoO2  
// VGA_WndDrawLineBase()

// WZD s14p11
// drake178: UU_VGA_DrawBiColorRect()
// MoO2  Module: graphics  Interlaced_Fill()
// UU_Interlaced_Fill()

// WZD s14p12
// drake178: 
// MoO2  
// UU_VGA_DrawRect()

// WZD s14p13
// drake178: 
// MoO2  
// UU_VGA_WndDrawRect()

// WZD s14p14
// drake178: 
// MoO2  
// UU_VGA_DrawDblRect()

// WZD s14p15
// drake178: VGA_RectangleFX()
// MoO2  Module: graphics  Gradient_Fill()
void Gradient_Fill(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t fill_type, int16_t remap_block, int16_t Slope, int16_t Scale, int16_t Seed);

// WZD s14p16
// drake178: DOS_PrintString()
// MoO2  
// 1oom 
// DOS_PrintString



/*
    WIZARDS.EXE
    seg016
*/

// WZD s16p01
void Fill(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color);

// WZD s16p02
// MoO2  Module: graphics  Dot()
// 1oom  uidraw.c  ui_draw_pixel()
void Dot(int16_t x, int16_t y, uint8_t color);

// WZD s16p03
// drake178: VGA_DrawLine()
// AKA VGA_Draw_Line()
// MoO2  Module: line  Line()
// 1oom  uidraw.c  // ui_draw_line1()
void Line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color);

// WZD s16p04
// drake178: VGA_DrawPatternLine90
// MoO2  
// VGA_DrawPatternLine

// WZD s16p05
// drake178: UU_VGA_CreateColorWave()
// MoO2  
// UU_VGA_CreateColorWave

// WZD s16p06
// drake178: UU_VGA_DiagColumns()
// MoO2  
// UU_VGA_DiagColumns

// WZD s16p07
// drake178: 
// MoO2  
// RNG_Direct_LFSR

// WZD s16p08
// drake178: VGA_Shade_Rect()
// MoO2  Module: graphics  Tint_Fill()
void Tint_Fill(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t remap_block);

// WZD s16p09
// drake178: VGA_Grayscale_Rect()
// MoO2  Module: graphics  Gray_Scale_Fill()
// VGA_Grayscale_Rect

// WZD s16p10
// drake178: UU_VGA_Columns()
// MoO2  
// UU_VGA_Columns

// WZD s16p11
// drake178: UU_VGA_ScrambleRect()
// MoO2  
// UU_VGA_ScrambleRect



#ifdef __cplusplus
}
#endif

#endif  /* GRAPHICS_H */
