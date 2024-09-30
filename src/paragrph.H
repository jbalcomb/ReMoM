/*
    WIZARDS.EXE
        seg019

    MoO2
        paragrph
*/

#ifndef PARAGRPH_H
#define PARAGRPH_H

#include "MoM.H"

#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  seg019
*/

// WZD s19p01
// drake178: VGA_DrawText()
void Print_Paragraph(int16_t x, int16_t y, int16_t max_width, char * string, int16_t print_type);

// WZD s19p02
// UU_VGA_WndDrawText()

// WZD s19p03
// UU_GUI_CreateHelpLinks()

// WZD s19p04
// drake178: VGA_TextSetup()
void Mark_Paragraph(int16_t x, int16_t y, int16_t text_width, char * text_string);

// WZD s19p05
// drake178: VGA_GetTextHeight
int16_t Get_Paragraph_Max_Height(int16_t max_width, char * string);

// WZD s19p06
// drake178: VGA_GetTextWidth
int16_t Get_Paragraph_Max_Width(int16_t max_width, char * string, int16_t print_type);

// WZD s19p07
// drake178: VGA_ResetFltBlocks()
void Reset_Paragraph_Exclusions(void);

// WZD s19p08
// drake178: VGA_AddFloatingBlock()
// MoO2: Set_Paragraph_Exclusion_Area()
void Set_Paragraph_Exclusion_Area(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

// WZD s19p09
// drake178: UU_VGA_AddFltIMGSpace()
// MoO2: Set_Paragraph_Exclusion_Picture()

// WZD s19p10
// drake178: STR_RemoveLBSPs
void Remove_Paragraph_Marks(char * string);

// WZD s19p11
// Get_Font_Height()

// WZD s19p12
// UU_VGA_WidestGlyph()

// WZD s19p13
void Print_Paragraph_To_Bitmap(int16_t x, int16_t y, int16_t max_width, char * string, int16_t print_type, SAMB_ptr bitmap);

// WZD s19p14
// VGA_TextDraw_Init()



#ifdef __cplusplus
}
#endif

#endif  /* PARAGRPH_H */
