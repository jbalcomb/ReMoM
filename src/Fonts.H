#ifndef FONTS_H
#define FONTS_H

#include "MoX_TYPE.H"



// 96 characters; ASCII: {32, ..., 126}
struct s_FONT_HEADER
{
    /*  00 */ uint8_t  current_colors[16];
    /*  10 */ uint16_t height;
    /*  12 */ uint8_t  shadow_flag;
    /*  14 */ uint8_t  normal_colors[16];
    /*  24 */ uint8_t  highlight_colors[16];
    /*  34 */ uint8_t  special_colors[16];
    /*  44 */ uint16_t current_base_height;
    /*  46 */ uint16_t current_vertical_spacing;
    /*  48 */ uint16_t current_horizontal_spacing;
    /*  4A */ uint8_t  current_font_widths[96];
    /*  AA */ uint16_t current_data_offsets[96];

    /* 16A */ uint16_t base_height[8];
    /* 17A */ uint16_t horizontal_spacing[8];
    /* 18A */ uint16_t vertical_spacing[8];
    /* 19A */ uint8_t  font_widths[768];    /* 8 * 96 * sizeof 1 */
    /* 49A */ uint16_t data_offsets[768];  /* 8 * 96 * sizeof 2 */
};



// WZD dseg:783C
// extern int16_toutline_color = 0;


extern byte_ptr mouse_palette;                         // MGC dseg:A81A    set in Load_Palette()  16 (16*16) cursor images

extern byte_ptr p_Palette;                             // MGC dseg:A7DE    alloc in Load_Font_File()
extern byte_ptr p_PaletteFlags;                        // MGC dseg:A7D6    alloc in Load_Font_File()
extern byte_ptr p_Palette_XBGR;                        // STU/Win32



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


void Load_Font_File(char * font_file);
void Load_Palette(int entry, int start_color, int end_color);
void Set_Palette_Changes(int start_color, int end_color);


/*
    WIZARDS.EXE  seg018
*/
// WZD s18p01
void Set_Font(int16_t font_idx, int16_t color1, int16_t color2, int16_t color3);


/*
    WIZARDS.EXE  seg017
*/

// WZD s17p01
void Set_Font_Style1(int16_t Font_Index, int16_t Color_1, int16_t Color_2, int16_t Color_3);
// WZD s17p02
void Set_Font_Style2(int16_t Font_Index, int16_t Color_1, int16_t Color_2, int16_t Color_3);
// WZD s17p03
void Set_Font_Style3(int16_t Font_Index, int16_t Color_1, int16_t Color_2, int16_t Color_3);
// WZD s17p04
void Set_Font_Style4(int16_t Font_Index, int16_t Color_1, int16_t Color_2, int16_t Color_3);
// WZD s17p05
void Set_Font_Style5(int16_t Font_Index, int16_t Color_1, int16_t Color_2, int16_t Color_3);

// WZD s17p06
void Set_Outline_Color(int16_t color_num);

// WZD s17p11
void Set_Font_Colors_15(int16_t font_idx, uint8_t * colors);

// WZD s17p12
void Set_Alias_Color(int16_t color);
// WZD s17p23
int16_t Print_Right(int16_t x, int16_t y, char * string);
// WZD s17p25
int16_t Print_Integer_Right(int16_t x, int16_t y, int16_t val);
// WZD s17p35
int16_t Print(int16_t x, int16_t y, char * string);
// WZD s17p36
int16_t Print_Display(int16_t x, int16_t y, char * string, int16_t full_flag);
// WZD s17p37
int16_t Print_String(int16_t x, int16_t y, char * string, int16_t change_color_ok_flag, int16_t full_flag);

/*
    WIZARDS.EXE  seg018
*/
// WZD s18p01
void Set_Font(int16_t font_idx, int16_t color1, int16_t color2, int16_t color3);
// WZD s18p02
int16_t Print_Character(int16_t x, int16_t y, int16_t char_num);
// WZD s18p03
int16_t Print_Character_No_Alias(int16_t x, int16_t y, int16_t char_num);
// WZD s18p04
// LBX_DrawGlyph ? Print_Character_To_Bitmap() ?
// WZD s18p05
void Print_Character_ASM(int16_t x_start, int16_t y_start, int16_t width, byte_ptr font_data_offset);
// WZD s18p06
void Print_Character_No_Alias_ASM(int16_t x_start, int16_t y_start, int16_t width, byte_ptr font_data_offset);
// WZD s18p07
int16_t Get_String_Width(char * string);


#ifdef __cplusplus
}
#endif


#endif /* FONTS_H */
