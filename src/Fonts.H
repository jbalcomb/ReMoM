#ifndef FONTS_H
#define FONTS_H

#include "MoX_TYPE.H"



#define PALETTE_FLAG(_palette_index_)                  ( GET_1B_OFS( (p_Palette), (768 + (_palette_index_)) ) )



/*
    MoO2
    Module: fontdraw
    Print_Character_No_Alias_ASM_
    Address: 01:00144A06
    Print_Character_ASM_
    Address: 01:001449CC
*/


enum e_Font_Style
{
    e_Font_Style_Default = 0,
    e_Font_Style_1 = 1,
    e_Font_Style_2 = 2,
    e_Font_Style_3 = 3,
    e_Font_Style_4 = 4,
    e_Font_Style_5 = 5,
    e_Font_Style_6 = 6,
    e_Font_Style_7 = 7,
};


enum e_Font_Shadow
{
    e_Font_Shadow_None = 0,
    e_Font_Shadow_Down = 1,
    e_Font_Shadow_Up = 2,
    e_Font_Shadow_Heavy = 3,
    e_Font_Shadow_Outline = 4,
    e_Font_Shadow_Outline_Heavy = 5    
};

// 96 characters; ASCII: {32, ..., 126}

struct s_FONT_HEADER
{
    /*  00 */ uint8_t  current_colors[16];
    /*  10 */ uint16_t height;
    /*  12 */ uint8_t  shadow_flag;  /* ¿ Set_Font_Style() does STOSW here ? */
    /*  13 */ uint8_t  current_color_set;
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
    /* 49A */ uint16_t data_offsets[768];   /* 8 * 96 * sizeof 2 */
    /* 79A */
};

#define SZ_FONT_HDR                             1946

#define COLOR_SET_COUNT                           16

#define FONT_HDR_POS_CURRENT_COLORS             0x00
#define FONT_HDR_POS_HEIGHT                     0x10
#define FONT_HDR_POS_SHADOW_FLAG                0x12
#define FONT_HDR_POS_CURRENT_COLOR_SET          0x13

#define FONT_HDR_POS_COLOR_SETS_OFST            0x14
#define FONT_HDR_POS_NORMAL_COLORS              0x14
#define FONT_HDR_POS_HIGHLIGHT_COLORS           0x24
#define FONT_HDR_POS_SPECIAL_COLORS             0x34

#define FONT_HDR_POS_CURRENT_BASE_HEIGHT        0x44
#define FONT_HDR_POS_CURRENT_VERTICAL_SPACING   0x46
#define FONT_HDR_POS_CURRENT_HORIZONTAL_SPACING 0x48

#define FONT_HDR_POS_CURRENT_FONT_WIDTHS        0x4A



#define FONT_GET_HEIGHT(_ptr_)                  ( GET_2B_OFS( (_ptr_), FONT_HDR_POS_HEIGHT            )             )
#define FONT_GET_SHADOW_FLAG(_ptr_)             ( GET_1B_OFS( (_ptr_), FONT_HDR_POS_SHADOW_FLAG       )             )
#define FONT_GET_CURRENT_COLOR_SET(_ptr_)       ( GET_1B_OFS( (_ptr_), FONT_HDR_POS_CURRENT_COLOR_SET )             )

#define FONT_GET_CURRENT_BASE_HEIGHT            ( GET_2B_OFS( (_ptr_), FONT_HDR_POS_CURRENT_BASE_HEIGHT        )    )
#define FONT_GET_CURRENT_VERTICAL_SPACING       ( GET_2B_OFS( (_ptr_), FONT_HDR_POS_CURRENT_VERTICAL_SPACING   )    )
#define FONT_GET_CURRENT_HORIZONTAL_SPACING     ( GET_2B_OFS( (_ptr_), FONT_HDR_POS_CURRENT_HORIZONTAL_SPACING )    )

// #define FONT_GET_CURRENT_FONT_WIDTH()           ( GET_1B_OFS( (_ptr_), FONT_HDR_POS_CURRENT_FONT_WIDTHS)       )

#define FONT_SET_CURRENT_COLOR(_ptr_,_color_)   ( SET_1B_OFS( (_ptr_), FONT_HDR_POS_CURRENT_COLORS, (_color_) )     )




#ifdef __cplusplus
extern "C" {
#endif



// WZD dseg:783C
// extern int16_t outline_color = 0;

// WZD dseg:A81E
// AKA gsa_Palette_Font_Colors
extern byte_ptr font_colors;  // 300h into the palette entry, 16 arrays of 16 colors



/*
    BEGIN:  Fonts
*/
// WZD  dseg:E7DE 00 00                                           gsa_VGAFILEH_Header dw 0 
// WZD  dseg:E7E0 00 00                                           p_PaletteFlags dw 0      

// WZD  dseg:E7E2 00 00                                           
// drake178: 1500h into the palette entry, an array of 24 color fractions (B-G-R-Percent) used to set shaded replacement colors for each color index
// extern byte_ptr gsa_ShadingColors;
extern byte_ptr remap_colors;

// WZD  dseg:E7E4
// extern SAMB_ptr Replacement_Colors;  // Replacement_Colors[24][256] ¿ or 224 ?
// 384 paragraphs = 384 * 16 = 6,144 bytes  (24 * 256  ~'remap color tables')
extern byte_ptr remap_color_palettes;   // alloc in Load_Font_File()  set in Create_Remap_Palette()

// WZD  dseg:E7E6 00 00                                           UU_DAC_Save_Seg dw 0     

// WZD dseg:E7E8
// current_palette

// WZD dseg:E7EA
// char font_name[16];                             // set in Load_Font_File()

// WZD  dseg:E7FA
// 96 paragraphs = 96 * 16 = 1,536 bytes  (1536 / 256 = 6)
extern SAMB_ptr Intensity_Scale_Tbl;  // alloc in Load_Font_File()  used in Gray_Scale_Bitmap()
// ¿ MoO2 ~ remap_gray_scale_palette ?


// WZD dseg:E820
extern SAMB_ptr font_style_data;                       // alloc & load in Load_Font_File()

extern byte_ptr mouse_palette;                         // MGC dseg:A81A    set in Load_Palette()  16 (16*16) cursor images

extern byte_ptr p_Palette;                             // MGC dseg:A7DE    alloc in Load_Font_File()
extern byte_ptr current_palette;

extern byte_ptr p_PaletteFlags;                        // MGC dseg:A7D6    alloc in Load_Font_File()
// DELETE  extern byte_ptr p_Palette_XBGR;                        // STU/Win32
extern byte_ptr palette_flags;





void Load_Font_File(char * font_file);

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
// AKA Set_Font_Style4()
// MoO2: Set_Font_Style_Outline()
void Set_Font_Style_Outline(int16_t style_num, int16_t Color_1, int16_t Color_2, int16_t Color_3);
// WZD s17p05
void Set_Font_Style5(int16_t Font_Index, int16_t Color_1, int16_t Color_2, int16_t Color_3);
// WZD s17p06
void Set_Outline_Color(int16_t color_num);

// WZD s17p08
void Set_Font_Spacing_Width(int16_t spacing);
// WZD s17p09
void Set_Font_Spacing(int16_t spacing);

// WZD s17p11
void Set_Font_Colors_15(int16_t font_idx, uint8_t * colors);

// WZD s17p12
void Set_Alias_Color(int16_t color);


// WZD s17p21
int16_t Print_Integer(int16_t x, int16_t y, int16_t val);

// WZD s17p22
// int16_t UU_Print_Long(int16_t x, int16_t y, int32_t val);

// WZD s17p23
int16_t Print_Right(int16_t x, int16_t y, char * string);
// WZD s17p24
int16_t Print_Centered(int16_t x, int16_t y, char * string);
// WZD s17p25
int16_t Print_Integer_Right(int16_t x, int16_t y, int16_t val);
// WZD s17p26
int16_t Print_Integer_Centered(int16_t x, int16_t y, int16_t val);


// WZD s17p35
int16_t Print(int16_t x, int16_t y, char * string);
// WZD s17p36
int16_t Print_Display(int16_t x, int16_t y, char * string, int16_t full_flag);
// WZD s17p37
int16_t Print_String(int16_t x, int16_t y, char * string, int16_t change_color_ok_flag, int16_t full_flag);

// WZD s17p38
// drake178: UU_VGA_DisableAAPixels

// WZD s17p39
// drake178: VGA_UseFontColor1
// MoO2: Set_Normal_Colors_On
void Set_Normal_Colors_On(void);

// WZD s17p40
// drake178: VGA_FontColor2Toggle
// MoO2: Set_Highlight_Colors_On
void Set_Highlight_Colors_On(void);


// WZD s17p41
// drake178: VGA_FontColor3Toggle
// MoO2: Set_Special_Colors_On
void Set_Special_Colors_On(void);


// WZD s17p42
// drake178: VGA_FontColorSelect()
// MoO2: Set_Normal_Colors_On(); Set_Highlight_Colors_On(); Set_Special_Colors_On();
void Set_Color_Set(int16_t color_set_idx);

// WZD s17p53
int16_t Get_Current_Font_Index(void);
// WZD s17p54
// Get_Font_Color1
int16_t Get_Current_Font_Color(void);
// WZD s17p55
// Get_Font_Color2
// WZD s17p56
// Get_Font_Color3


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


/*
    WIZARDS.EXE  seg020
*/

// WZD s20p01
void Load_Palette(int entry, int start_color, int end_color);

// WZD s20p02
// drake178: VGA_ResetShade0()
// MoO2: ~ Calculate_Remap_Gray_Palettes_()
// fills out the first record of the Replacement_Colors@ table using 50% black (the original), but will only affect colors marked as changed in the DAC
// void Update_Remap_Gray_Palette(void);

// WZD s20p02
// drake178: VGA_ResetShade0()
// MoO2: ~ Calculate_Remap_Gray_Palettes_()
// fills out the first record of the Replacement_Colors@ table using 50% black (the original), but will only affect colors marked as changed in the DAC
// void Update_Remap_Gray_Palette(void);

// WZD s20p03
// drake178: VGA_SetShades_Grey0()
// MoO2: ~ Update_Glass_Remap_Colors()
// fills out the Replacement_Colors@ table by combining Shading_Colors@ with any changed palette colors table 0 is set to shades of 50% black instead of whatever the palette originally had (the same)
void Calculate_Remap_Colors(void);
void Bogus_Calculate_Remap_Colors(void);

// WZD s20p04
// drake178: VGA_SetShades_Range()
// MoO2: ~ Update_Glass_Remap_Color_Range()
// fills out the Replacement_Colors@ table by combining Shading_Colors@ with any changed palette colors, but only for the color sets indicated by the confines table 0 is set to shades of 50% black instead of whatever the palette originally had, if included
// Update_Remap_Color_Range


/*
    WIZARDS.EXE  seg021
    (assembly code)
*/

// WZD s21p01
// PLATFORM
void Apply_Palette(void);

// WZD s21p06
// drake178: VGA_ShadeColorMatch
// void VGA_ShadeColorMatch(int16_t Tbl_Index, int16_t Red, int16_t Green, int16_t Blue, int16_t Percent);
// void Create_Remap_Palette(int16_t block, uint8_t red, uint8_t green, uint8_t blue, uint8_t percent);
// WZD s21p06
// drake178: VGA_ShadeColorMatch
// MoO2: 
// 1oom: lbxpal_build_colortable()
// ANI: make_cfit_table()
void Create_Remap_Palette(int16_t block, uint8_t red, uint8_t green, uint8_t blue, uint8_t percent);
void Create_Remap_Palette_0(int16_t block);
void Create_Remap_Palette_1(int16_t block, uint8_t red, uint8_t green, uint8_t blue);
void Create_Remap_Palette_(int16_t block, uint8_t red, uint8_t green, uint8_t blue, uint8_t percent);
uint8_t Find_Closest_Color(uint8_t red, uint8_t green, uint8_t blue);
// DELETE  void Create_Remap_Palette_ASM(int16_t block, uint8_t red, uint8_t green, uint8_t blue, uint8_t percent);


// void lbxpal_build_colortables(void);
// void lbxpal_build_colortable(int ctablei, uint8_t pr, uint8_t pg, uint8_t pb, uint8_t percent);
// uint8_t lbxpal_find_closest(uint8_t r, uint8_t g, uint8_t b);
// uint8_t Find_Closest_Color(uint8_t r, uint8_t g, uint8_t b);



#ifdef __cplusplus
}
#endif


#endif /* FONTS_H */
