#ifndef ST_TXT_H
#define ST_TXT_H


enum Outline_Style
{
    Shadow_BtmRight     = 1,
    Shadow_TopLeft      = 2,
    Shadow_BtmRight_2px = 3,
    Outline_Full        = 4,
    Outline_Plus_BR2px  = 5
};


struct s_FONT_HEADER //; (sizeof=0x16A, standard type)
{
    unsigned char Current_Colors[16];
    unsigned int Font_Height;
    unsigned char Outline_Style;
    unsigned char Color_Index;              // ? Currect ColorSet Selected ?
    unsigned char Colors_1[16];
    unsigned char Colors_2[16];
    unsigned char Colors_3[16];
    unsigned int Line_Height;
    unsigned int Vert_Spacing;
    unsigned int Horz_Spacing;
    unsigned char Glyph_Widths[96];
    unsigned int Glyph_Offsets[96];
};

struct s_FONT_DATA //; (sizeof=0x4574)
{
    struct s_FONT_HEADER Hdr_Space;
    unsigned int Font_Heights[8];
    unsigned int Horz_Spacings[8];
    unsigned int Vert_Spacings[8];
    unsigned int Glyph_Widths[768];
    unsigned int Glyph_Offsets[768];
    unsigned char Glyph_Data[15066];
};


/*
    Initialized Data
*/

extern int g_VGA_Outline_Color;             // dseg:4314
extern int g_VGA_Skip_AA_Pixels;            // dseg:4316
// dseg:4318 UU_VGA_TextDraw_Enabled dw 0
// dseg:431A VGA_FltBlock_Count dw 0
// dseg:431C VGA_BlinkState dw 0FFFFh                ; 0 - normal, 1 - reverse, -1 - disabled

/*
    Uninitialized Data
*/

// dseg:A7F6 VGA_AAColor_Array db 16 dup(  0)
// dseg:A806 VGA_Current_AA_Color db 0
// dseg:A807 VGA_AA_Color_1 db 0
// dseg:A808 VGA_AA_Color_2 db 0
// dseg:A809 VGA_AA_Color_3 db 0
extern int g_VGA_Font_ColorIndex3; // dseg:A80A
extern int g_VGA_Font_ColorIndex2; // dseg:A80C
extern int g_VGA_Font_ColorIndex1; // dseg:A80E
extern int g_VGA_Font_Index; // dseg:A810
extern int g_VGA_TextDraw_Y; // dseg:A812
extern int g_VGA_TextDraw_X; // dseg:A814
// dseg:A816 gsa_FontStyleData dw 0                  ; single-loaded LBX entry set up in VGA_DAC_Init
// dseg:A818 gsa_BorderStyleData dw 0
// dseg:A81A gsa_Palette_Cursor_Data dw 0            ; 500h into the palette entry
// dseg:A81C UU_gsa_Palette_Data dw 0                ; 400h into the palette entry
// dseg:A81E gsa_Palette_Font_Colors dw 0            ; 300h into the palette entry, 16 arrays of 16 colors
// dseg:A820 VGA_TextLine_Starts dd 0                ; points to an array of starting string indexes for
// dseg:A824 VGA_TextLine_Tops dd 0                  ; points to an array of starting Y positions for
// dseg:A828 VGA_TextLine_Rights dd 0                ; points to an array of ending X positions for
// dseg:A82C VGA_TextLine_Lefts dd 0                 ; points to an array of starting X positions for
// dseg:A830 VGA_TextDraw_Lines dw 0                 ; the amount of elements in the VGA_TextLine_ arrays
// dseg:A832 VGA_FltBlock_Bottoms dw 4 dup(     0)   ; up to 4 bottom row positions used to reserve space
// dseg:A83A VGA_FltBlock_Rights dw 4 dup(     0)    ; up to 4 right column positions used to reserve space
// dseg:A842 VGA_FltBlock_Tops dw 4 dup(     0)      ; up to 4 top row positions used to reserve space
// dseg:A84A VGA_FltBlock_Lefts dw 4 dup(     0)     ; up to 4 left column positions used to reserve space
// dseg:A852 VGA_Blink_Speed dw 0
// dseg:A854 VGA_BlinkHue_C1 dw 0                    ; the component of C1 that holds the defining hue


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

/*
    seg017
*/
// _s17p06c.c
void VGA_SetOutlineColor(int Color_Index);
// _s17p35c.c
void VGA_DrawTextLine(int Left, int Top, char *String);
// _s17p36.asm
void VGA_DrawStyledString(int Left, int Top, char *String, int JstfyTo);
// _s17p37.asm
void VGA_DrawString(int Left, int Top, char *String, int Colors, int JstfyTo);

// _s17p39.asm
void VGA_UseFontColor1(void);
// _s17p40.asm
void VGA_FontColor2Toggle(void);
// _s17p41.asm
void VGA_FontColor3Toggle(void);

// _s17p42.asm
void VGA_FontColorSelect(int ColorSet_Index);


/*
    seg018
*/
// _s18p01
void VGA_SetFont(int Font_Index, int Color_1, int Color_2, int Color_3);

// _s18p02
void VGA_DrawChar(int Left, int Top, int Char);
// _s18p03
void VGA_DrawChar_NoAA(int Left, int Top, int Char);

// _s18p04
//void LBX_DrawGlyph(int Left, int Top, int Char, unsigned int IMG_Seg);
// _s18p05
void VGA_DrawGlyph(void);
// _s18p06
void VGA_DrawGlyph_NoAA(void);

// _s18p07
int VGA_GetStringWidth(char *String);


#ifdef __cplusplus
}
#endif

#endif /* ST_TXT_H */
