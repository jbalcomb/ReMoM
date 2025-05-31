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

#include "MOX_Lib.H"



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
/*  Init.C */  // void Init_Drivers(int input_type, char * font_file);

// WZD s14p02
/* Init.C */  // void UU_Legacy_Startup(int input_type, int midi_driver, int sound_channels, char * font_file)

// WZD s14p03
// Load_Font_File()

// WZD s14p04
void Set_Window(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

// WZD s14p05
void Reset_Window(void);

// WZD s14p06
void Clipped_Fill(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color);

// WZD s14p07
void Clipped_Dot(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color);

// WZD s14p08
void Clipped_Line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color);

// WZD s14p09
void Clipped_Multi_Colored_Line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t colortbl[], int colornum, int colorpos);

// WZD s14p10
void Clipped_Line_Base(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t Patterned, uint8_t color, uint8_t colortbl[], int16_t colornum, int16_t colorpos);
static void ui_draw_line_limit_do(int x1, int y1, int x2, int y2, uint8_t color, const uint8_t *colortbl, int colornum, int colorpos);

// WZD s14p11
// UU_Interlaced_Fill()

// WZD s14p12
// UU_VGA_DrawRect()

// WZD s14p13
// UU_VGA_WndDrawRect()

// WZD s14p14
// UU_VGA_DrawDblRect()

// WZD s14p15
void Gradient_Fill(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t fill_type, int16_t remap_block, int16_t Slope, int16_t Scale, int16_t Seed);

// WZD s14p16
void DOS_PrintString__STUB(char * string);



/*
    WIZARDS.EXE
    seg016
*/

// WZD s16p01
void Fill(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color);

// WZD s16p02
void Dot(int16_t x, int16_t y, uint8_t color);

// WZD s16p03
void Line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color);

// WZD s16p04
void Multi_Colored_Line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t colortbl[], int16_t colornum, int16_t colorpos);
void ui_draw_line_ctbl(int x1, int y1, int x2, int y2, const uint8_t *colortbl, int colornum, int pos);

// WZD s16p05
// UU_VGA_CreateColorWave

// WZD s16p06
// UU_VGA_DiagColumns

// WZD s16p07
uint16_t rnd_bitfiddle__1oom(uint16_t ax);

// WZD s16p08
void Tint_Fill(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t remap_block);

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
