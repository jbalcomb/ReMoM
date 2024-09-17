/*
    WIZARDS.EXE
        seg017
        seg018
        seg019
        seg020  MoO2 Module: palette
        seg021
*/
#ifndef FONTS_H
#define FONTS_H

#include "MoX_TYPE.H"




#define SZ_FONT_HDR                             1946

#define COLOR_SET_COUNT                           16
#define COLOR_SET_HIGHLIGHT                        1
#define COLOR_SET_SPECIAL                          2

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
    e_Font_Shadow_Down = 1,          // bottom right
    e_Font_Shadow_Up = 2,            // top left
    e_Font_Shadow_Heavy = 3,         // 2x bottom right
    e_Font_Shadow_Outline = 4,       // full
    e_Font_Shadow_Outline_Heavy = 5  // full + bottom right
};

// 96 characters; ASCII: {32, ..., 126}

struct s_FONT_HEADER
{
    /*  00 */ uint8_t  current_colors[COLOR_SET_COUNT];
    /*  10 */ uint16_t height;
    /*  12 */ uint8_t  shadow_flag;  /* ¿ Set_Font_Style() does STOSW here ? */
    /*  13 */ uint8_t  current_color_set;  /* {0: normal, 1: highlight, 2: special} */
    /*  14 */ uint8_t  normal_colors[COLOR_SET_COUNT];
    /*  24 */ uint8_t  highlight_colors[COLOR_SET_COUNT];
    /*  34 */ uint8_t  special_colors[COLOR_SET_COUNT];
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



// WZD dseg:783C                                                 seg017  MoO2 Module: fonts

// WZD dseg:783C
// extern int16_t outline_color = 0;
// uint16_t outline_color = 0;

// WZD dseg:783E
// int16_t draw_alias_flag;

// WZD dseg:7840 00 00                                           UU_VGA_TextDraw_Enabled dw 0            ; DATA XREF: VGA_TextDraw_Init+4w
// WZD dseg:7842 00 00                                           exclusion_count dw 0                    ; DATA XREF: Print_Paragraph+181w ...

// WZD dseg:7844                                                 seg020  MoO2 Module: palette
// WZD dseg:7844
// PRIVATE  int16_t cycle_direction_flag;

// WZD dseg:7846 68 35 68 35                                     random_seed dd 35683568h                ; DATA XREF: Set_Random_Seed+6w ...





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

// WZD dseg:E7FA
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
void Set_Font_Style(int16_t font_idx, int16_t color1, int16_t color2, int16_t color3);


/*
    WIZARDS.EXE  seg017
*/

// WZD s17p01
void Set_Font_Style_Shadow_Down(int16_t style_num, int16_t Color_1, int16_t Color_2, int16_t Color_3);
// WZD s17p02
void Set_Font_Style_Shadow_Up(int16_t style_num, int16_t Color_1, int16_t Color_2, int16_t Color_3);
// WZD s17p03
void Set_Font_Style_Shadow_Heavy(int16_t style_num, int16_t Color_1, int16_t Color_2, int16_t Color_3);
// WZD s17p04
void Set_Font_Style_Outline(int16_t style_num, int16_t Color_1, int16_t Color_2, int16_t Color_3);
// WZD s17p05
void Set_Font_Style_Outline_Heavy(int16_t style_num, int16_t Color_1, int16_t Color_2, int16_t Color_3);
// WZD s17p06
void Set_Outline_Color(int16_t color_num);
// WZD s17p07
void Set_Font_LF(int16_t spacing);
// WZD s17p08
void Set_Font_Spacing_Width(int16_t spacing);
// WZD s17p09
void Set_Font_Spacing(int16_t spacing);

// WZD s17p10
// drake178: ?
// Set_Font_Highlight_Colors()

// WZD s17p11
// drake178: ?
void Set_Font_Colors_15(int16_t font_idx, uint8_t * colors);

// WZD s17p12
// drake178: ?
void Set_Alias_Color(int16_t color);

// WZD s17p13
// drake178: ?
void Save_Alias_Colors(void);

// WZD s17p14
// drake178: ?
void Restore_Alias_Colors(void);

// WZD s17p15
// drake178: VGA_DrawFarString()
// PLATFORM  MSDOS  int16_t Print_Far(int16_t x, int16_t y, char * string);
// Print_Far()

// WZD s17p16
// VGA_DrawCenteredFar()
// WZD s17p17
// UU_VGA_DrawRtAlgFar()
// WZD s17p18
// VGA_WndDrawFarString()
// WZD s17p19
// UU_VGA_WndDrawCntrdFar()
// WZD s17p20
// UU_VGA_WndDrawRtAlgFar()

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

// WZD s17p27
// drake178: UU_Print_Long_Right()
// WZD s17p28
// drake178: VGA_WndDrawNumber()
// WZD s17p29
// drake178: UU_VGA_WndDrawLongN()
// WZD s17p30
// drake178: UU_VGA_WndDrawRtAligned()

// WZD s17p31
// drake178: VGA_WndDrawCentered()
int16_t Clipped_Print_Centered(int16_t x, int16_t y, char * string);

// WZD s17p32
// drake178: UU_VGA_WndDrawRtAlgNum()
// WZD s17p33
// drake178: UU_VGA_WndDrawRtAlgLong()

// Clipped_Print_String()
// Print_Clipped_Character()
// Print_Clipped_Letter()

// WZD s17p34
int16_t Print_Full(int16_t x, int16_t y, char * string, int16_t right_side);

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

// WZD s17p43
// RP_DBG_TblDrawValue()
// WZD s17p44
// UU_DBG_TblCellWrapper()
// WZD s17p45
// UU_DBG_TblDrawString()
// WZD s17p46
// UU_DBG_TblDrawS16()
// WZD s17p47
// UU_DBG_TblDrawS32()
// WZD s17p48
// UU_DBG_TblDrawU32()
// WZD s17p49
// DBG_DrawTableCell()

// WZD s17p50
// drake178: VGA_WndDrawSString()
// DNE in MoO2
int16_t Clipped_Print(int16_t x, int16_t y, char * string);

// WZD s17p51
int16_t Clipped_Print_String(int16_t x, int16_t y, char * string, int16_t change_color_ok_flag);

// WZD s17p52
// drake178: VGA_WndDrawChar()
/*

*/
int16_t Clipped_Print_Character(int16_t x, int16_t y, char character);

// WZD s17p53
// MoO2  Get_Font_Style_()
int16_t Get_Current_Font_Style(void);
// WZD s17p54
int16_t Get_Current_Normal_Color(void);
// WZD s17p55
int16_t Get_Current_Highlight_Color(void);
// WZD s17p56
int16_t Get_Current_Special_Color(void);

// WZD s17p57
// UU_STR_CopyToNearLBX()
// WZD s17p58
// VGA_GetVertSpacing()

// WZD s17p59
void Trim(char * string);

// WZD s17p60
int16_t Print_To_Bitmap(int16_t x, int16_t y, char * string, SAMB_ptr bitmap);

// WZD s17p61
int16_t Print_Display_To_Bitmap(int16_t x, int16_t y, char * string, int16_t full_flag, SAMB_ptr bitmap);

// WZD s17p62
int16_t Print_String_To_Bitmap(int16_t x, int16_t y, char * string, int16_t change_color_ok_flag, int16_t full_flag, SAMB_ptr bitmap);

// WZD s17p63
int16_t Print_Right_To_Bitmap(int16_t x, int16_t y, char * string, SAMB_ptr bitmap);

// MoO2
// Print_Centered_To_Bitmap()
// Print_Full_To_Bitmap()
// Print_To_Bitmap()
// Print_Display_To_Bitmap()
// Print_String_To_Bitmap()
// Print_Character_To_Bitmap()
// Print_Clipped_Letter_To_Bitmap()
// Set_PSTR_Style()

// WZD s17p64
// Print_Centered_To_Bitmap()

// WZD s17p65
// Print_Full_To_Bitmap()

// WZD s17p66
int16_t Print_To_Bitmap_Far(int16_t x, int16_t y, char * string, SAMB_ptr bitmap);

// WZD s17p67
// UU_LBX_DrawRtAlgFar()
// WZD s17p68
// UU_LBX_DrawCenteredFar()
// WZD s17p69
// UU_LBX_DrawJustifiedFar()



/*
    WIZARDS.EXE  seg018
*/
// WZD s18p01
void Set_Font_Style(int16_t font_idx, int16_t color1, int16_t color2, int16_t color3);

// WZD s18p02
int16_t Print_Character(int16_t x, int16_t y, int16_t char_num);

// WZD s18p03
int16_t Print_Character_No_Alias(int16_t x, int16_t y, int16_t char_num);

// WZD s18p04
int16_t Print_Character_To_Bitmap(int16_t x, int16_t y, uint8_t char_num, SAMB_ptr bitmap);

// WZD s18p05
void Print_Character_ASM(int16_t x_start, int16_t y_start, int16_t width, byte_ptr font_data_offset);

// WZD s18p06
void Print_Character_No_Alias_ASM(int16_t x_start, int16_t y_start, int16_t width, byte_ptr font_data_offset);

// WZD s18p07
int16_t Get_String_Width(char * string);

// WZD s18p08
// drake178: VGA_DrawPartialChar()
// ¿ MoO2  Module: fonts  Print_Clipped_Character() ?
void Print_Clipped_Character(int16_t x, int16_t y, char char_num, int16_t skip_x, int16_t width, int16_t skip_y, int16_t max_y);

// WZD s18p09
// drake178: VGA_DrawPartialGlyph()
// ¿ MoO2  Module: fonts  Print_Clipped_Letter() ?
void Print_Clipped_Letter(int16_t x_start, byte_ptr font_data_offset);



/*
    WIZARDS.EXE  seg019
*/

// WZD s19p01
// Print_Paragraph()

// WZD s19p02
// WZD s19p03
// WZD s19p04
// WZD s19p05
// WZD s19p06
// WZD s19p07
// WZD s19p08
// WZD s19p09

// WZD s19p10
int16_t Get_Font_Height(void);

// WZD s19p11
// WZD s19p12
// WZD s19p13
// WZD s19p14


/*
    WIZARDS.EXE  seg020
*/

// WZD s20p01
void Load_Palette(int entry, int start_color, int end_color);

// WZD s20p02
// drake178: VGA_ResetShade0()
// MoO2: ~ Calculate_Remap_Gray_Palettes_()
void Update_Remap_Gray_Palette(void);

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
void Update_Remap_Color_Range(int16_t first_set, int16_t last_set);

// WZD s20p05
void Set_Palette_Changes(int start_color, int end_color);

// WZD s20p06
void Clear_Palette_Changes(int start_color, int end_color);

// WZD s20p07
// WZD s20p08
// WZD s20p09
// WZD s20p10

// WZD s20p11
void Reset_Cycle_Palette_Color(void);

// WZD s20p12
void Cycle_Palette_Color__STUB(int16_t color_num, int16_t red_min, int16_t green_min, int16_t blue_min, int16_t red_max, int16_t green_max, int16_t blue_max, int16_t step_value);

// WZD s20p13
// Update_Cycle(int *color_min, int *color_max)

// WZD s20p14
// UU_VGA_CreateHues(int First_DAC_Reg, int Steps, int B_Red, int B_Grn, int B_Blu, int T_Red, int T_Grn, int T_Blu)



/*
    WIZARDS.EXE  seg021
    (assembly code)
*/

// WZD s21p01
// PLATFORM
void Apply_Palette(void);

// WZD s21p02
// drake178: VGA_DAC_DimWrite()
// ¿ MoO2  Module: palstore  Cycle_Palette_() Darken_Palette_() Tint_Palette_() ?
void Cycle_Palette(int16_t percent);


// WZD s21p03
// drake178: VGA_ShadeScreen()

// WZD s21p04
// drake178: VGA_SlideColors()
// ¿ MoO2: Cycle_Palette_Color() ... Update_Cycle()
// ¿ 1oom ?
void VGA_SlideColors__STUB(int16_t First_Color, int16_t Count, int16_t ShiftBy);

// WZD s21p05
// drake178: UU_VGA_ColorWave()


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

// WZD s21p07
// FLIC_Load_Palette()


#ifdef __cplusplus
}
#endif


#endif /* FONTS_H */
