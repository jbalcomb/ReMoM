/*
    WIZARDS.EXE
        seg017
        seg018
        seg019
        seg020  MoO2 Module: palette
        seg021
*/

#include "Allocate.H"
#include "FLIC_Draw.H"
#include "Fonts.H"
#include "Graphics.H"
#include "LBX_Load.H"
#include "MOX_BASE.H"
#include "MOX_BITS.H"
#include "MOX_DEF.H"
#include "MOX_T4.H"  // ~ MOX & MOX2 //MoO2
#include "MOX_TYPE.H"
#include "Util.H"
#include "Video.H"

#include "../MOM_PFL.H"



// WZD dseg:783C                                                 seg017  MoO2 Module: fonts

// WZD dseg:783C
uint16_t outline_color = 0;

// WZD dseg:783E
int16_t draw_alias_flag = 0;

// WZD dseg:7840 00 00                                           UU_VGA_TextDraw_Enabled dw 0            ; DATA XREF: VGA_TextDraw_Init+4w
// WZD dseg:7842 00 00                                           exclusion_count dw 0                    ; DATA XREF: Print_Paragraph+181w ...

// WZD dseg:7844                                                 seg020  MoO2 Module: palette
// WZD dseg:7844
int16_t cycle_direction_flag = ST_UNDEFINED;

// WZD dseg:7846 68 35 68 35                                     random_seed dd 35683568h                ; DATA XREF: Set_Random_Seed+6w ...





/*
    BEGIN:  Fonts
*/
// WZD  dseg:E7DE 00 00                                           gsa_VGAFILEH_Header dw 0 
// WZD  dseg:E7E0 00 00                                           palette_flags dw 0      

// WZD  dseg:E7E2 00 00                                           
// AKA gsa_ShadingColors
// drake178: Shading_Colors@  ; 1500h into the palette entry, an array of 24 color fractions (B-G-R-Percent) used to set shaded replacement colors for each color index
// MoO2: ~ glass_colors
// byte_ptr gsa_ShadingColors;
byte_ptr remap_colors;

// WZD  dseg:E7E4
// AKA Replacement_Colors
// drake178: Replacement_Colors@
// MoO2: ~ glass_remap_colors[16][256]
// 1oom: lbxpal_colortable[24][256]
// SAMB_ptr Replacement_Colors;  // picture_remap_color_list[24][256]
byte_ptr remap_color_palettes;

// WZD  dseg:E7E6 00 00                                           UU_DAC_Save_Seg dw 0     

// WZD dseg:E7E8
// current_palette

// WZD dseg:E7EA
char font_name[16];                             // set in Load_Font_File

// WZD  dseg:E7FA
// Intensity_Scale_Tbl@ dw 0               ; 60h LBX_Alloc_Space paragraphs (should be only 10h)
SAMB_ptr Intensity_Scale_Tbl;

// WZD dseg:E7FC
SAMB_ptr palette_data;
// WZD dseg:E7FE0
SAMB_ptr palette_block;

// WZD dseg:E800
// drake178: VGA_AAColor_Array
uint8_t old_alias_colors[16];

// WZD dseg:E810 00                      
uint8_t VGA_Current_AA_Color;

// WZD dseg:E811 00                      
uint8_t VGA_AA_Color_1;

// WZD dseg:E812 00                      
uint8_t VGA_AA_Color_2;

// WZD dseg:E813 00                      
uint8_t VGA_AA_Color_3;

// WZD dseg:E814
int16_t m_current_special_color;
// WZD dseg:E816
int16_t m_current_highlight_color;
// WZD dseg:E818
int16_t m_current_normal_color;
// WZD dseg:E81A
int16_t m_current_font_style;

// WZD dseg:E81C
int16_t print_ypos;
// WZD dseg:E81E
int16_t print_xpos;
// WZD dseg:E820
SAMB_ptr font_style_data;                       // MGC dseg:A816    alloc & load in Load_Font_File
// font_data dw 0
// MoO2: font_ptr
struct s_FONT_HEADER * font_header;
SAMB_ptr border_style_data;                     // MGC dseg:A818    alloc & load in Load_Font_File
byte_ptr mouse_palette;                         // MGC dseg:A81A    set in Load_Palette()  16 (16*16) cursor images
// WZD dseg:E822
// border_data dw 0
// WZD dseg:E824
// mouse_palette dw 0                      ; 500h into the palette entry
// WZD dseg:E826
// UU_gsa_Palette_Data dw 0                ; 400h into the palette entry
// WZD dseg:E828 00 00                   font_colors dw 0                        ; 300h into the palette entry, 16 arrays of 16 colors

// WZD dseg:E82A 00 00 00 00             VGA_TextLine_Starts dd 0                ; points to an array of starting string indexes for
// WZD dseg:E82A                                                                 ; drawing lines of text (128 bytes, LBX_Alloc_Space)
// WZD dseg:E82E 00 00 00 00             VGA_TextLine_Tops dd 0                  ; points to an array of starting Y positions for
// WZD dseg:E82E                                                                 ; drawing lines of text (128 bytes, LBX_Alloc_Space)
// WZD dseg:E832 00 00 00 00             VGA_TextLine_Rights dd 0                ; points to an array of ending X positions for
// WZD dseg:E832                                                                 ; drawing lines of text (128 bytes, LBX_Alloc_Space)
// WZD dseg:E836 00 00 00 00             VGA_TextLine_Lefts dd 0                 ; points to an array of starting X positions for
// WZD dseg:E836                                                                 ; drawing lines of text (128 bytes, LBX_Alloc_Space)
// WZD dseg:E83A 00 00                   VGA_TextDraw_Lines dw 0                 ; the amount of elements in the VGA_TextLine_ arrays
// WZD dseg:E83C 00 00 00 00 00 00 00 00 VGA_FltBlock_Bottoms dw 4 dup(0)        ; up to 4 bottom row positions used to reserve space
// WZD dseg:E83C                                                                 ; where floating text won't be drawn
// WZD dseg:E844 00 00 00 00 00 00 00 00 VGA_FltBlock_Rights dw 4 dup(0)         ; up to 4 right column positions used to reserve space
// WZD dseg:E844                                                                 ; where floating text won't be drawn
// WZD dseg:E84C 00 00 00 00 00 00 00 00 VGA_FltBlock_Tops dw 4 dup(0)           ; up to 4 top row positions used to reserve space
// WZD dseg:E84C                                                                 ; where floating text won't be drawn
// WZD dseg:E854 00 00 00 00 00 00 00 00 VGA_FltBlock_Lefts dw 4 dup(0)          ; up to 4 left column positions used to reserve space
// WZD dseg:E854                                                                 ; where floating text won't be drawn


// WZD dseg:A81E
// AKA gsa_Palette_Font_Colors
byte_ptr font_colors;  // 300h into the palette entry, 16 arrays of 16 colors


// WZD dseg:E7E8
// MoO2  current_palette
// MGC dseg:A7DE    alloc in Load_Font_File()
// AKA p_Palette;                             
uint8_t * current_palette;

// WZD dseg:E7E0
// AKA p_Palette_XBGR
// AKA p_PaletteFlags
uint8_t * palette_flags;


// WZD  s14o03
/*
    MoO2  Module  init
*/
void Load_Font_File(char * font_file)
{
    int itr;

    strcpy(font_name, font_file);

    font_style_data = LBX_Load(font_file, 0);
    border_style_data = LBX_Load(font_file, 1);

    font_header = (struct s_FONT_HEADER *)font_style_data;

    palette_block = Allocate_Space(348);    // 348 PR, 5568 B

    current_palette = Allocate_Space(64);     //  64 PR, 1024 B

    palette_flags = current_palette + (48 * SZ_PARAGRAPH_B);  // ~== palette_flags = &current_palette[768];

    // TODO  UU_DAC_Save_Seg = Allocate_Space(48);  // in MoO1, also unused, maybe debug code

    remap_color_palettes = (uint8_t *)Allocate_Space(384);  // 384 PR, 6144 B  (24 * 256  ~'remap color tables')

    // TODO  file_animation_header = (struct s_FLIC_HDR *)Allocate_Space(2);

    Intensity_Scale_Tbl = Allocate_Space(96);  // 96 PR, 1536 B

    // TODO  VGA_TextDraw_Init();

    for(itr = 0; itr < 768; itr++)
    {
        *(current_palette + itr) = ST_TRANSPARENT;
    }
    for(itr = 0; itr < 256; itr++)
    {
        *(palette_flags + itr) = ST_TRUE;
    }

}


/*
    WIZARDS.EXE  seg017
*/

// WZD s17p01
// AKA Set_Font_Style1()
// MoO2  Module: fonts  Set_Font_Style_Shadow_Down()
/*

Settings_Screen_Draw()
    Set_Font_Style_Shadow_Down(2, 2, 45, ST_NULL);
    ~== Set_Font_Style_Shadow_Down(2, 2, 45, 0);
*/
void Set_Font_Style_Shadow_Down(int16_t style_num, int16_t Color_1, int16_t Color_2, int16_t Color_3)
{
    Set_Font_Style(style_num, Color_1, Color_2, Color_3);
    font_header->shadow_flag = e_Font_Shadow_Down;
}
// WZD s17p02
// AKA Set_Font_Style2()
// MoO2  Module: fonts  Set_Font_Style_Shadow_Up()
void Set_Font_Style_Shadow_Up(int16_t style_num, int16_t Color_1, int16_t Color_2, int16_t Color_3)
{
    Set_Font_Style(style_num, Color_1, Color_2, Color_3);
    font_header->shadow_flag = e_Font_Shadow_Up;
}
// WZD s17p03
// AKA Set_Font_Style3()
// MoO2  Module: fonts  Set_Font_Style_Shadow_Heavy()
void Set_Font_Style_Shadow_Heavy(int16_t style_num, int16_t Color_1, int16_t Color_2, int16_t Color_3)
{
    Set_Font_Style(style_num, Color_1, Color_2, Color_3);
    font_header->shadow_flag = e_Font_Shadow_Heavy;
}
// WZD s17p04
// AKA Set_Font_Style4()
// MoO2  Module: fonts  Set_Font_Style_Outline()
void Set_Font_Style_Outline(int16_t style_num, int16_t Color_1, int16_t Color_2, int16_t Color_3)
{
    Set_Font_Style(style_num, Color_1, Color_2, Color_3);
    font_header->shadow_flag = e_Font_Shadow_Outline;
}
// WZD s17p05
// AKA Set_Font_Style5()
// MoO2  Module: fonts  Set_Font_Style_Outline_Heavy()
void Set_Font_Style_Outline_Heavy(int16_t style_num, int16_t Color_1, int16_t Color_2, int16_t Color_3)
{
    Set_Font_Style(style_num, Color_1, Color_2, Color_3);
    font_header->shadow_flag = e_Font_Shadow_Outline_Heavy;
}


// WZD s17p06
void Set_Outline_Color(int16_t color_num)
{
    outline_color = color_num;
}

// WZD s17p07
// ? MoO2 Set_Font_LF ?
void Set_Font_LF(int16_t spacing)
{
    font_header->current_base_height = font_header->height + spacing;
}

// WZD s17p08
// overrides the default width of the SPACE character
void Set_Font_Spacing_Width(int16_t spacing)
{
    font_header->current_font_widths[0] = spacing;
}

// WZD s17p09
void Set_Font_Spacing(int16_t spacing)
{
    font_header->current_horizontal_spacing = spacing;
}


// WZD s17p10
// drake178: ?
// MoO2  Set_Font_Highlight_Colors()


// WZD s17p11
// ¿ MoO2:  Set_Remap_Font_Style ?
// TODO  Set_Remap_Font_Style...(style_num, colors)
// Set_Alias_Color(int16_t palette_num) ... int16_t i;  int16_t start_offset
/*
    sets 16 colors, starting at font colors index 240
    ¿ font colors block 15 is font remap colors ?

98:  162,162,186
*/
void Set_Font_Colors_15(int16_t font_idx, uint8_t * colors)
{
    int16_t itr;
    int16_t color_start;

    color_start = 240;  // ¿ 15 * 16 ?

    for(itr = 0; itr < 16; itr++)
    {
        // farpokeb(font_colors, color_start + itr, colors[itr]);
        font_colors[color_start + itr] = colors[itr];
    }

    // sets current_colors and normal_colors to font color block 15
    Set_Font_Style(font_idx, 15, 0, 0);

}


// WZD s17p12
// drake178: ?
/*
    sets font_colors[][0] to alias color
    sets current[0],normal[0],highlight[0],special[0] to alias color
*/
void Set_Alias_Color(int16_t color)
{
    int16_t itr;

    for(itr = 0; itr < 16; itr++)
    {
        font_colors[itr * 16] = color;
        // i.e., font_colors[0][0]; font_colors[1][0]; ...
    }

    SET_1B_OFS(font_style_data, FONT_HDR_POS_CURRENT_COLORS, color);
    SET_1B_OFS(font_style_data, FONT_HDR_POS_NORMAL_COLORS, color);
    SET_1B_OFS(font_style_data, FONT_HDR_POS_HIGHLIGHT_COLORS, color);
    SET_1B_OFS(font_style_data, FONT_HDR_POS_SPECIAL_COLORS, color);

}

// WZD s17p13
// drake178: ¿ ?
/*
; saves the anti-aliasing colors of each font color
; set to static variables starting at dseg:e800 (16
; color blocks + current colors + 3 default sets)
*/
/*

*/
void Save_Alias_Colors(void)
{
    int16_t itr_color_count;

    for(itr_color_count = 0; itr_color_count < 16; itr_color_count++)
    {
        old_alias_colors[itr_color_count] = *(font_colors + (itr_color_count * 16) );
    }

    VGA_Current_AA_Color = GET_1B_OFS(font_style_data, FONT_HDR_POS_CURRENT_COLORS);
    VGA_AA_Color_1 = GET_1B_OFS(font_style_data, FONT_HDR_POS_NORMAL_COLORS);
    VGA_AA_Color_2 = GET_1B_OFS(font_style_data, FONT_HDR_POS_HIGHLIGHT_COLORS);
    VGA_AA_Color_3 = GET_1B_OFS(font_style_data, FONT_HDR_POS_SPECIAL_COLORS);

}

// WZD s17p14
// drake178: ?
void Restore_Alias_Colors(void)
{
    int16_t itr_color_count;

    for(itr_color_count = 0; itr_color_count < 16; itr_color_count++)
    {
        *(font_colors + (itr_color_count * 16) ) = old_alias_colors[itr_color_count];
    }

    SET_1B_OFS(font_style_data, FONT_HDR_POS_CURRENT_COLORS, VGA_Current_AA_Color);
    SET_1B_OFS(font_style_data, FONT_HDR_POS_NORMAL_COLORS, VGA_AA_Color_1);
    SET_1B_OFS(font_style_data, FONT_HDR_POS_HIGHLIGHT_COLORS, VGA_AA_Color_2);
    SET_1B_OFS(font_style_data, FONT_HDR_POS_SPECIAL_COLORS, VGA_AA_Color_3);

}

// PLATFORM  MSDOS  // WZD s17p15
// PLATFORM  MSDOS  // drake178: VGA_DrawFarString()
// PLATFORM  MSDOS  // PLATFORM
// PLATFORM  MSDOS  int16_t Print_Far(int16_t x, int16_t y, unsigned short int src_ofst, unsigned short int src_sgmt)
// PLATFORM  MSDOS  {
// PLATFORM  MSDOS      int16_t next_x;
// PLATFORM  MSDOS  
// PLATFORM  MSDOS      String_Copy_Far(near_buffer, 0, src_ofst, src_sgmt);
// PLATFORM  MSDOS  
// PLATFORM  MSDOS      next_x = Print(x, y, near_buffer);
// PLATFORM  MSDOS  
// PLATFORM  MSDOS      return next_x;
// PLATFORM  MSDOS  }
// int16_t Print_Far(int16_t x, int16_t y, unsigned short int src_ofst, unsigned short int src_sgmt)
int16_t Print_Far(int16_t x, int16_t y, char * src)
{
    int16_t next_x;

    // String_Copy_Far(near_buffer, 0, src_ofst, src_sgmt);
    String_Copy_Far(near_buffer, src);

    next_x = Print(x, y, near_buffer);

    return next_x;
}


// WZD s17p16
// drake178: VGA_DrawCenteredFar()
// int16_t Print_Centered_Far(int16_t x, int16_t y, unsigned short int src_ofst, unsigned short int src_sgmt)
int16_t Print_Centered_Far(int16_t x, int16_t y, char * src)
{
    int16_t next_x;

    // String_Copy_Far(near_buffer, 0, src_ofst, src_sgmt);
    String_Copy_Far(near_buffer, src);

    next_x = Print(x, y, near_buffer);

    return next_x;
}

// WZD s17p17
// drake178: UU_VGA_DrawRtAlgFar()
// int16_t Print_Right_Far(int16_t x, int16_t y, unsigned short int src_ofst, unsigned short int src_sgmt)
int16_t Print_Right_Far(int16_t x, int16_t y, char * src)
{
    int16_t next_x;

    // String_Copy_Far(near_buffer, 0, src_ofst, src_sgmt);
    String_Copy_Far(near_buffer, src);

    next_x = Print(x, y, near_buffer);

    return next_x;
}

// WZD s17p18
// drake178: VGA_WndDrawFarString()
// int16_t Clipped_Print_Far(int16_t x, int16_t y, unsigned short int src_ofst, unsigned short int src_sgmt)
int16_t Clipped_Print_Far(int16_t x, int16_t y, char * src)
{
    int16_t next_x;

    // String_Copy_Far(near_buffer, 0, src_ofst, src_sgmt);
    String_Copy_Far(near_buffer, src);

    next_x = Clipped_Print(x, y, near_buffer);

    return next_x;
}

// WZD s17p19
// drake178: UU_VGA_WndDrawCntrdFar()
// int16_t Clipped_Print_Centered_Far(int16_t x, int16_t y, unsigned short int src_ofst, unsigned short int src_sgmt)
int16_t Clipped_Print_Centered_Far(int16_t x, int16_t y, char * src)
{
    int16_t next_x;

    // String_Copy_Far(near_buffer, 0, src_ofst, src_sgmt);
    String_Copy_Far(near_buffer, src);

    next_x = Clipped_Print_Centered(x, y, near_buffer);

    return next_x;
}

// WZD s17p20
// drake178: UU_VGA_WndDrawRtAlgFar()
// int16_t Clipped_Print_Right_Far(int16_t x, int16_t y, unsigned short int src_ofst, unsigned short int src_sgmt)
int16_t Clipped_Print_Right_Far(int16_t x, int16_t y, char * src)
{
    int16_t next_x;

    // String_Copy_Far(near_buffer, 0, src_ofst, src_sgmt);
    String_Copy_Far(near_buffer, src);

    next_x = Clipped_Print_Right(x, y, near_buffer);

    return next_x;
}


// WZD s17p21
int16_t Print_Integer(int16_t x, int16_t y, int16_t val)
{
    int16_t next_x;
    char buffer[10];

#pragma warning(suppress : 4996)
    itoa(val, buffer, 10);

    next_x = Print(x, y, buffer);

    return next_x;
}


// WZD s17p22
// int16_t UU_Print_Long(int16_t x, int16_t y, int32_t val);
int16_t Print_Long(int16_t x, int16_t y, int32_t val)
{
    int16_t next_x;
    char buffer[10];

#pragma warning(suppress : 4996)
    ltoa(val, buffer, 10);

    next_x = Print(x, y, buffer);

    return next_x;
}


// WZD s17p23
// drake178: VGA_DrawRightAligned()
int16_t Print_Right(int16_t x, int16_t y, char * string)
{
    int16_t next_x;
    int16_t string_len;

    string_len = Get_String_Width(string) - 1;

    next_x = Print((x - string_len), y, string);

    return next_x;
}


// WZD s17p24
// drake178: VGA_DrawCentered()
int16_t Print_Centered(int16_t x, int16_t y, char * string)
{
    int16_t next_x;
    int16_t string_len;

    string_len = Get_String_Width(string);

    next_x = Print((x - (string_len/2)), y, string);

    return next_x;
}


// WZD s17p25
// drake178: VGA_DrawRtAlgNumber()
int16_t Print_Integer_Right(int16_t x, int16_t y, int16_t val)
{
    int16_t next_x;
    char buffer[LEN_TEMP_BUFFER];

    itoa(val, buffer, 10);

    next_x = Print_Right(x, y, buffer);

    return next_x;
}


// WZD s17p26
int16_t Print_Integer_Centered(int16_t x, int16_t y, int16_t val)
{
    int16_t next_x;
    char buffer[LEN_TEMP_BUFFER];

    itoa(val, buffer, 10);

    next_x = Print_Centered(x, y, buffer);

    return next_x;
}


// WZD s17p27
// drake178: UU_Print_Long_Right()
int16_t Print_Long_Right(int16_t x, int16_t y, int32_t val)
{
    int16_t next_x;
    char buffer[LEN_TEMP_BUFFER];

    itoa(val, buffer, 10);

    next_x = Print_Right(x, y, buffer);

    return next_x;
}

// WZD s17p28
// drake178: VGA_WndDrawNumber()
int16_t Clipped_Print_Integer(int16_t x, int16_t y, int16_t val)
{
    int16_t next_x;
    char buffer[LEN_TEMP_BUFFER];

    itoa(val, buffer, 10);

    next_x = Clipped_Print(x, y, buffer);

    return next_x;
}

// WZD s17p29
// drake178: UU_VGA_WndDrawLongN()
int16_t Clipped_Print_Long(int16_t x, int16_t y, int32_t val)
{
    int16_t next_x;
    char buffer[LEN_TEMP_BUFFER];

    ltoa(val, buffer, 10);

    next_x = Clipped_Print(x, y, buffer);

    return next_x;
}

// WZD s17p30
// drake178: UU_VGA_WndDrawRtAligned()
int16_t Clipped_Print_Right(int16_t x, int16_t y, char * string)
{
    int16_t next_x;
    int16_t string_len;

    string_len = Get_String_Width(string) - 1;

    next_x = Clipped_Print((x - string_len), y, string);

    return next_x;
}

// WZD s17p31
// drake178: VGA_WndDrawCentered()
// DNE in MoO2
// 1oom  src/lbxfont  lbxfont_print_str_center_limit()
/*
int lbxfont_print_str_center_limit(int x, int y, const char *str, int lx0, int ly0, int lx1, int ly1, uint16_t pitch, int scale)
    int w = lbxfont_calc_str_width(str);
    return lbxfont_print_str_normal_limit(x - w / 2, y, str, lx0, ly0, lx1, ly1, pitch, scale);
*/
/*

Print_Centered()
    |-> Print()
        |-> Print_Display()

*/
int16_t Clipped_Print_Centered(int16_t x, int16_t y, char * string)
{
    int16_t next_x;
    int16_t string_len;

    string_len = Get_String_Width(string);

    next_x = Clipped_Print((x - (string_len / 2)), y, string);

    return next_x;
}


// WZD s17p32
// drake178: UU_VGA_WndDrawRtAlgNum()
int16_t Clipped_Print_Integer_Right(int16_t x, int16_t y, int16_t val)
{
    int16_t next_x;
    char buffer[10];

    itoa(val, buffer, 10);

    next_x = Clipped_Print_Right(x, y, buffer);

    return next_x;
}


// WZD s17p33
// drake178: UU_VGA_WndDrawRtAlgLong()
int16_t Clipped_Print_Long_Right(int16_t x, int16_t y, int32_t val)
{
    int16_t next_x;
    char buffer[LEN_TEMP_BUFFER];

    ltoa(val, buffer, 10);

    next_x = Clipped_Print_Right(x, y, buffer);

    return next_x;
}


// WZD s17p34
int16_t Print_Full(int16_t x, int16_t y, char * string, int16_t right_side)
{
    int16_t next_x;

    if(right_side < 0)
    {
        right_side = 0;
    }

    next_x = Print_Display(x, y, string, right_side);

    return next_x;

}


// WZD s17p35
// drake178: VGA_DrawTextLine()
int16_t Print(int16_t x, int16_t y, char * string)
{
    int16_t next_x;

    next_x = Print_Display(x, y, string, ST_FALSE);

    return next_x;
}


// WZD s17p36
// drake178: VGA_DrawStyledString()
int16_t Print_Display(int16_t x, int16_t y, char * string, int16_t full_flag)
{
    int16_t next_x;  // DNE in Dasm
    int16_t itr;
    uint16_t outline_style;

    outline_style = FONT_GET_SHADOW_FLAG(font_style_data);

    if(outline_style != 0)
    {
        for(itr = 0; itr < COLOR_SET_COUNT; itr++)
        {
            SET_1B_OFS(font_style_data, FONT_HDR_POS_CURRENT_COLORS + itr, outline_color);
        }

        draw_alias_flag = ST_TRUE;
        
        if(outline_style != e_Font_Shadow_Up)
        {
            Print_String(x + 1, y + 1, string, ST_FALSE, full_flag);  // overdraw right + botton
            Print_String(x    , y + 1, string, ST_FALSE, full_flag);  // overdraw bottom
            Print_String(x + 1, y    , string, ST_FALSE, full_flag);  // overdraw right
        }

        if( outline_style != e_Font_Shadow_Down && outline_style != e_Font_Shadow_Heavy)
        {
            Print_String(x - 1, y    , string, ST_FALSE, full_flag);  // overdraw left
            Print_String(x - 1, y - 1, string, ST_FALSE, full_flag);  // overdraw left + top
            Print_String(x    , y - 1, string, ST_FALSE, full_flag);  // overdraw top
        }

        if(outline_style == e_Font_Shadow_Heavy || outline_style == e_Font_Shadow_Outline_Heavy)
        {
            Print_String(x + 2, y + 2, string, ST_FALSE, full_flag);
            Print_String(x + 1, y + 2, string, ST_FALSE, full_flag);
            Print_String(x + 2, y + 1, string, ST_FALSE, full_flag);
        }

        if(outline_style > e_Font_Shadow_Heavy)
        {
            Print_String(x + 1, y - 1, string, ST_FALSE, full_flag);  // overdraw right + top
            Print_String(x - 1, y + 1, string, ST_FALSE, full_flag);  // overdraw left + bottom
        }

        if(outline_style == e_Font_Shadow_Outline_Heavy)
        {
            Print_String(x + 2, y    , string, ST_FALSE, full_flag);
            Print_String(x    , y + 2, string, ST_FALSE, full_flag);
        }

        Set_Color_Set(GET_1B_OFS(font_style_data,FONT_HDR_POS_CURRENT_COLOR_SET));
    }

    draw_alias_flag = ST_FALSE;

    next_x = Print_String(x, y, string, ST_TRUE, full_flag);

    return next_x;
}


// WZD s17p37
/*
    handles text drawing control codes
    if the string contains non-printable ASCII values, other than '\0', behavior changes
        '\x01'  Set_Normal_Colors_On();
        '\x02'  Set_Highlight_Colors_On();
        '\x03'  Set_Special_Colors_On();
        '\x04'  Set_Highlight_Colors_On();

*/
int16_t Print_String(int16_t x, int16_t y, char * string, int16_t change_color_ok_flag, int16_t full_flag)
{
    int16_t next_x;  // DNE in Dasm
    // char character_;
    int16_t str_idx;
    int16_t space_add;
    int16_t space_remainder;
    int16_t current_space;
    int16_t space_count;
    char character;
    uint16_t ptr;

    ptr = 0;

    print_xpos = x;
    print_ypos = y;

    if(full_flag != ST_FALSE)
    {
        current_space = 0;
        space_remainder = 0;
        str_idx = 0;
        space_count = 0;

        while(str_idx++)
        {
            if(string[str_idx] != '\x00') { break; }            /*  ASCII  00h  00d  NUL (null)                  */
            if(string[str_idx] != '\x0D') { break; }            /*  ASCII  0Dh  13d  CR (carriage return)        */
            if(string[str_idx] != '\x14') { break; }            /*  ASCII  14h  20d  DC4 (device control 4)      */
            if(string[str_idx] != '\x19') { break; }            /*  ASCII  15h  21d  NAK (negative acknowledge)  */
            if(string[str_idx] != '\x15') { break; }            /*  ASCII  19h  25d  EM (end of medium)          */
            if(string[str_idx] != '\x1D') { break; }            /*  ASCII  1Dh  29d  GS (group separator)        */
            if(string[str_idx] == '\x20') { space_count++; }    /*  ASCII  20h  32d  SPACE                       */
        }

        if(space_count == 0)
        {
            full_flag = ST_FALSE;
        }

        full_flag -= Get_String_Width(string);

        if(full_flag > 0)
        {
            space_remainder = full_flag % space_count;
            space_add = full_flag / space_count;
        }
        else
        {
            full_flag = ST_FALSE;
        }
    }

    while(string[ptr] != '\0')
    {
        character = string[ptr];

        switch(character)
        {
            case 1:  /* character == '\x01' */
            {
                if(change_color_ok_flag != ST_FALSE)
                {
                    Set_Normal_Colors_On();
                }
            } break;
            case 2:  /* character == '\x02' */
            {
                if(change_color_ok_flag != ST_FALSE)
                {
                    Set_Highlight_Colors_On();
                }
            } break;
            case 3:  /* character == '\x03' */
            {
                if(change_color_ok_flag != ST_FALSE)
                {
                    Set_Special_Colors_On();
                }
            } break;
            case 4:  /* character == '\x04' */
            {
                if(change_color_ok_flag != ST_FALSE)
                {
                    Set_Highlight_Colors_On();
                }
            } break;
            case 13:  /*  ASCII  0Dh  13d  CR (carriage return)        */
            case 20:  /*  ASCII  14h  20d  DC4 (device control 4)      */
            {
                print_ypos += GET_2B_OFS(font_style_data,FONT_HDR_POS_CURRENT_BASE_HEIGHT);
                print_xpos += x;
            } break;
            case 21:  /*  ASCII  15h  21d  NAK (negative acknowledge)  */
            {
                goto Done;
            } break;
            case 25:  /*  ASCII  19h  25d  EM (end of medium)          */
            case 29:  /*  ASCII  1Dh  29d  GS (group separator)        */
            {
                print_xpos = x + string[ptr+1];
                ptr++;
            } break;

        }

        if(draw_alias_flag != ST_FALSE)
        {
            print_xpos = Print_Character_No_Alias(print_xpos, print_ypos, character);
        }
        else
        {
            print_xpos = Print_Character(print_xpos, print_ypos, character);
        }

        if(full_flag != ST_FALSE)
        {
            if(character == 0x20)  /* ASCII SPACE 0x20 ' ' */
            {
                print_xpos += space_add;

                if(current_space < space_remainder)
                {
                    print_xpos++;
                }
                current_space++;
            }
        }

        ptr++;
    }


Done:

    next_x = print_xpos;

    return next_x;
}


// WZD s17p38
// drake178: UU_VGA_DisableAAPixels
// MoO2  No_Print_Alias()
void No_Print_Alias(void)
{
    draw_alias_flag = ST_TRUE;
}


// WZD s17p39
// drake178: VGA_UseFontColor1
// MoO2: Set_Normal_Colors_On
// MoO2: copies normal_colors into current_colors - 8 words
void Set_Normal_Colors_On(void)
{
    Set_Color_Set(0);
}

// WZD s17p40
// drake178: VGA_FontColor2Toggle
// MoO2: Set_Highlight_Colors_On
// MoO2: copies highlight_colors into current_colors - 8 words
/*
    IDGI:  Dasm definitely looks like a *toggle*, but 'Name Starting City' doesn't work with it like that.
    MoO2 does not look like a *toggle*.
    Something must unset it somewhere else?
        ...in Print(), change_colors_ok_flag?
*/
void Set_Highlight_Colors_On(void)
{
    uint8_t current_color_set;
    current_color_set = GET_1B_OFS(font_style_data,FONT_HDR_POS_CURRENT_COLOR_SET);
    if(current_color_set != 1)
    {
        Set_Color_Set(1);
    }
    else
    {
        Set_Color_Set(0);
    }
    // Set_Color_Set(1);
}

// WZD s17p41
// drake178: VGA_FontColor3Toggle
// MoO2: Set_Special_Colors_On
// MoO2: copies special_colors into current_colors - 8 words
void Set_Special_Colors_On(void)
{
    uint8_t current_color_set;
    current_color_set = GET_1B_OFS(font_style_data,FONT_HDR_POS_CURRENT_COLOR_SET);
    // if(current_color_set != 2)
    // {
    //     Set_Color_Set(2);
    // }
    // else
    // {
    //     Set_Color_Set(0);
    // }
    Set_Color_Set(2);
}


// WZD s17p42
// drake178: VGA_FontColorSelect()
// MoO2: Set_Normal_Colors_On(); Set_Highlight_Colors_On(); Set_Special_Colors_On();
// copies current color set colors into current_colors
void Set_Color_Set(int16_t color_set_idx)
{
    uint16_t color_set_offset;
    int16_t itr;
    uint8_t color;

    color_set_offset = FONT_HDR_POS_COLOR_SETS_OFST + (color_set_idx * COLOR_SET_COUNT);

    for(itr = 0; itr < COLOR_SET_COUNT; itr++)
    {
        // farpokew(font_style_data, itr, farpeekw(font_style_data, color_set_offset + itr) )
        color = GET_1B_OFS(font_style_data, color_set_offset + itr);
        SET_1B_OFS(font_style_data, itr, color);
    }

    // farpokeb(font_style_data, 0x13, color_set_idx);
    SET_1B_OFS(font_style_data, FONT_HDR_POS_CURRENT_COLOR_SET, color_set_idx);

}


// TODO  matches these up with MoO2's Disp_PSTR(), PSTRM(), PSTRS(), PSTR(), PSTRL(), PSTRU(), ...
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
/*
~ No Alias

; draws a string line into the draw segment, obeying
; any window limits set via VGA_SetDrawWindow, painting
; partial glyphs if necessary, using any font color
; selector bytes encountered in the text, and drawing
; outlines preset in the font header
; returns the next X coordinate to draw to
;
; unlike the other styled string functions, this one
; does not disable antialiasing for the outlines

looks like Print_Display()

Print()
    |-> Print_Display()
        |-> Print_String
            |-> Print_Character()
            |-> Print_Character_No_Alias()

So, ...
    not equivalent
    ¿ Print() vs. Print_Display() ?
        OON Print() hard codes full_flag to ST_FALSE
Here, ...
    Print() / Print_Display()
    includes hard-coded full_flag = ST_FALSE on every call to Clipped_Print_String()
    neither equivalent tp Print() or Print_Display()
    everywhere else calls Print(), so just Clipped_Print()

*/
int16_t Clipped_Print(int16_t x, int16_t y, char * string)
{
    int16_t next_x;  // DNE in Dasm
    int16_t itr;
    uint16_t outline_style;

    outline_style = FONT_GET_SHADOW_FLAG(font_style_data);

    if(outline_style != 0)
    {
        for(itr = 0; itr < 16; itr++)
        {
            SET_1B_OFS(font_style_data, FONT_HDR_POS_CURRENT_COLORS + itr, outline_color);
        }
        
        if(outline_style != 2) /* Shadow_TopLeft */
        {
            Clipped_Print_String(x + 1, y + 1, string, ST_FALSE);  // overdraw right + botton
            Clipped_Print_String(x    , y + 1, string, ST_FALSE);  // overdraw bottom
            Clipped_Print_String(x + 1, y    , string, ST_FALSE);  // overdraw right
        }

        if( outline_style != 1 && outline_style != 3)  /* Shadow_BtmRight || Shadow_BtmRight_2px */
        {
            Clipped_Print_String(x - 1, y    , string, ST_FALSE);  // overdraw left
            Clipped_Print_String(x - 1, y - 1, string, ST_FALSE);  // overdraw left + top
            Clipped_Print_String(x    , y - 1, string, ST_FALSE);  // overdraw top
        }

        if(outline_style == 3 || outline_style == 5)  /* Shadow_BtmRight_2px || Outline_Plus_BR2px */
        {
            Clipped_Print_String(x + 2, y + 2, string, ST_FALSE);
            Clipped_Print_String(x + 1, y + 2, string, ST_FALSE);
            Clipped_Print_String(x + 2, y + 1, string, ST_FALSE);
        }

        if(outline_style > 3)  /* Shadow_BtmRight_2px */
        {
            Clipped_Print_String(x + 1, y - 1, string, ST_FALSE);  // overdraw right + top
            Clipped_Print_String(x - 1, y + 1, string, ST_FALSE);  // overdraw left + bottom
        }

        if(outline_style == 5)  /* Outline_Plus_BR2px */
        {
            Clipped_Print_String(x + 2, y    , string, ST_FALSE);
            Clipped_Print_String(x    , y + 2, string, ST_FALSE);
        }

        Set_Color_Set(GET_1B_OFS(font_style_data,FONT_HDR_POS_CURRENT_COLOR_SET));
    }

    next_x = Clipped_Print_String(x, y, string, ST_TRUE);

    return next_x;
}


// WZD s17p51
// drake178: VGA_WndDrawString()
// ¿ MoO2  Module: fonts  Clipped_Print_String() ?
/*

    function (0 bytes) Clipped_Print_String
    Address: 01:00122309
        Num params: 4
        Return type: signed integer (2 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        pointer (4 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        Locals:
            signed integer (2 bytes) x
            signed integer (2 bytes) y
            pointer (4 bytes) string
            signed integer (2 bytes) change_color_ok_flag
            signed integer (2 bytes) full_flag
            signed integer (4 bytes) ptr
            signed integer (4 bytes) button_flag
            signed integer (4 bytes) tab
            signed integer (4 bytes) height
            signed integer (4 bytes) character
            signed integer (4 bytes) space_count
            signed integer (4 bytes) current_space
            signed integer (4 bytes) space_remainder
            signed integer (4 bytes) space_add
            signed integer (4 bytes) i
            array (4 bytes) tab_string
            Num elements:    4, Type:                unsigned integer (4 bytes) 

*/
/*
; draws a string line into the draw segment, obeying
; any window limits set via VGA_SetDrawWindow, painting
; partial glyphs if necessary, and using any font color
; selector bytes encountered in the text
; returns the next X coordinate to draw to

*/
int16_t Clipped_Print_String(int16_t x, int16_t y, char * string, int16_t change_color_ok_flag)
{
    int16_t next_x = 0;  // DNE in Dasm
    int16_t Switch_Char = 0;
    char character = 0;
    int16_t width = 0;
    int16_t height = 0;
    uint16_t ptr = 0;

    ptr = 0;

    print_xpos = x;
    print_ypos = y;

    height = FONT_GET_HEIGHT(font_style_data);

    if((y > screen_window_y2) || ((y + height) < screen_window_y1))
    {
        return 0;
    }

    if (x > screen_window_x2)
    {
        return x;
    }

    width = Get_String_Width(string);

    if ((x + width) < screen_window_x1)
    {
        return (x + width);
    }

    // while ((character = *str++) != 0)
    while(string[ptr] != '\0')
    {
        character = string[ptr];

        switch(character)
        {
            case 1:  /* character == '\x01' */  /* ASCII   1h  1d  SOH (start of heading)  */  /* sw_character_values[0] == character */
            {
                if(change_color_ok_flag != ST_FALSE)
                {
                    Set_Normal_Colors_On();
                }
            } break;
            case 2:
            {
                if(change_color_ok_flag != ST_FALSE)
                {
                    Set_Highlight_Colors_On();
                }
            } break;
            case 3:
            {
                if(change_color_ok_flag != ST_FALSE)
                {
                    Set_Special_Colors_On();
                }
            } break;
            case 4:
            {
                if(change_color_ok_flag != ST_FALSE)
                {
                    Set_Highlight_Colors_On();
                }
            } break;
            case 13:  /*  ASCII  0Dh  13d  CR (carriage return)        */
            {
                return print_xpos;
            } break;
            default:
            {
                // lbxfont_temp_x = lbxfont_print_char_ret_x_limit(lbxfont_temp_x, lbxfont_temp_y, c, lx0, ly0, lx1, ly1, pitch, scale);
                print_xpos = Clipped_Print_Character(print_xpos, print_ypos, character);
            } break;
        }

        ptr++;
    }

    return print_xpos;
}

// WZD s17p52
// drake178: VGA_WndDrawChar()

/*
; draws a single font character into the draw segment,
; obeying any window limits set via VGA_SetDrawWindow,
; painting partial glyphs if necessary
; returns the next X coordinate to draw to

*/
int16_t Clipped_Print_Character(int16_t x, int16_t y, char character)
{
    int16_t max_y;
    int16_t skip_y;
    char char_num;
    int16_t spacing;
    int16_t width;
    int16_t height;
    int16_t skip_x;
    int16_t next_x;

    char_num = (character - 32);

    if((char_num < 0) || (char_num > 94))
    {
        return x;
    }

    height = font_header->height;

    spacing = font_header->current_horizontal_spacing;

    width = font_header->current_font_widths[char_num];


    if((x < screen_window_x1) || ((x + width) > screen_window_x2) || (y < screen_window_y1) || ((y + height) > screen_window_y2))
    {
        next_x = x + width + spacing;

        if(x < screen_window_x1)
        {
            skip_x = screen_window_x1 - x;
            if (skip_x >= width)
            {
                return next_x;
            }
            x = screen_window_x1;
            width -= skip_x;
        }
        else
        {
            skip_x = 0;
        }

        if((x + width) > screen_window_x2)
        {
            width = (screen_window_x2 + 1) - x;
            if(width < 1)
            {
                return next_x;
            }
        }

        if(y < screen_window_y1)
        {
            skip_y = screen_window_y1 - y;
            y = screen_window_y1;
        }
        else
        {
            skip_y = 0;
        }

        if
        ((y + height) > screen_window_y2)
        {
            max_y = screen_window_y2 + 1 - y;
        }
        else
        {
            max_y = height;
        }

        // lbxfont_plotchar_limit(x, y, c, xskip, char_w, yskip, h, pitch, scale);
        Print_Clipped_Character(x, y, char_num, skip_x, width, skip_y, max_y);

    }
    else
    {
        next_x = Print_Character(x, y, character);
    }

    return next_x;
}


/*
MoO2
Pick_Font_Number_()
Pick_Font_Color_()
Get_Font_Style_()
Get_Current_Color_Set_()
Get_Current_Font_Color()
Get_Current_Highlight_Color()
Get_Current_Special_Color()
*/
// WZD s17p53
int16_t Get_Current_Font_Style(void)
{
    return m_current_font_style;
}
// WZD s17p54
int16_t Get_Current_Normal_Color(void)
{
    return m_current_normal_color;
}
// WZD s17p55
int16_t Get_Current_Highlight_Color(void)
{
    return m_current_highlight_color;
}
// WZD s17p56
int16_t Get_Current_Special_Color(void)
{
    return m_current_special_color;
}


// WZD s17p57
// drake178: UU_STR_CopyToNearLBX()
// void String_Copy_Far_To_Near(int offset, char *src_ofst, unsigned int src_sgmt)
void String_Copy_Far_To_Near(int offset, char * src)
{

    // String_Copy_Far(near_buffer[offset], 0, src_ofst, src_sgmt);
    String_Copy_Far((char *)&near_buffer[offset], src);

}


// WZD s17p58
// drake178: VGA_GetVertSpacing()
int16_t Get_Font_Vertical_Spacing(void)
{
    int16_t return_value;

    // return_value = (farpeekw(font_style_data, FONT_HDR_POS_HEIGHT) - farpeekb(font_style_data, FONT_HDR_POS_CURRENT_BASE_HEIGHT))
    return_value = (FONT_GET_HEIGHT(font_style_data) - FONT_GET_CURRENT_BASE_HEIGHT(font_style_data));

    return return_value;

}

// WZD s17p59
/*
MoO2
Module: strings
    function (0 bytes) Trim
    Address: 01:0013303E
        Num params: 1
        Return type: void (1 bytes) 
        pointer (4 bytes) 
        Locals:
            pointer (4 bytes) name
            signed integer (4 bytes) i
            signed integer (4 bytes) start_pos
            signed integer (4 bytes) end_pos
*/
/*
; trims white space from the beginning and end of the passed string
; BUGGED: will only remove one trailing space if there are multiple
*/
void Trim(char * string)
{
    int16_t Trailing_Space;
    int16_t itr1;
    int16_t itr2;  // _DI_

    itr1 = 0;
    while(string[itr1] == ' ') { itr1++; }

    itr2 = 0;
    Trailing_Space = ST_UNDEFINED;

    while(string[itr2] != '\0')
    {
        if(string[itr2] == ' ')
        {
            Trailing_Space = itr2;  //  ; BUG: will only register the final trailing space if there are multiple
        }
        else
        {
            Trailing_Space = ST_UNDEFINED;
        }
        itr2++;
    }

    if(Trailing_Space != ST_UNDEFINED)
    {
        string[Trailing_Space] = '\0';
    }

}


// WZD s17p60
// MoO2  Module: fonts  Print_To_Bitmap()
/*
    function (0 bytes) Print_To_Bitmap
    Address: 01:00122A6E
        Num params: 4
        Return type: void (1 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        pointer (4 bytes) 
        pointer (4 bytes) 
        Locals:
            signed integer (2 bytes) x
            signed integer (2 bytes) y
            pointer (4 bytes) string
            pointer (4 bytes) bitmap
*/
int16_t Print_To_Bitmap(int16_t x, int16_t y, char * string, SAMB_ptr bitmap)
{
    int16_t next_x;  // DNE in Dasm

    next_x = Print_Display_To_Bitmap(x, y, string, 0, bitmap);

    return next_x;
}


// WZD s17p61
// 1oom:  lbxfont.c  lbxfont_print_str_normal_do()
int16_t Print_Display_To_Bitmap(int16_t x, int16_t y, char * string, int16_t full_flag, SAMB_ptr bitmap)
{
    int16_t next_x;  // DNE in Dasm
    int16_t itr;
    int16_t outline_style;

    outline_style = FONT_GET_SHADOW_FLAG(font_style_data);

    if(outline_style != 0)
    {
        for(itr = 0; itr < 16; itr++)
        {
            SET_1B_OFS(font_style_data, FONT_HDR_POS_CURRENT_COLORS + itr, outline_color);
        }
        draw_alias_flag = ST_TRUE;
        
        if(outline_style != 2) /* Shadow_TopLeft */
        {
            Print_String_To_Bitmap(x + 1, y + 1, string, ST_FALSE, full_flag, bitmap);  // overdraw right + botton
            Print_String_To_Bitmap(x    , y + 1, string, ST_FALSE, full_flag, bitmap);  // overdraw bottom
            Print_String_To_Bitmap(x + 1, y    , string, ST_FALSE, full_flag, bitmap);  // overdraw right
        }

        if( outline_style != 1 && outline_style != 3)  /* Shadow_BtmRight || Shadow_BtmRight_2px */
        {
            Print_String_To_Bitmap(x - 1, y    , string, ST_FALSE, full_flag, bitmap);  // overdraw left
            Print_String_To_Bitmap(x - 1, y - 1, string, ST_FALSE, full_flag, bitmap);  // overdraw left + top
            Print_String_To_Bitmap(x    , y - 1, string, ST_FALSE, full_flag, bitmap);  // overdraw top
        }

        if(outline_style == 3 || outline_style == 5)  /* Shadow_BtmRight_2px || Outline_Plus_BR2px */
        {
            Print_String_To_Bitmap(x + 2, y + 2, string, ST_FALSE, full_flag, bitmap);
            Print_String_To_Bitmap(x + 1, y + 2, string, ST_FALSE, full_flag, bitmap);
            Print_String_To_Bitmap(x + 2, y + 1, string, ST_FALSE, full_flag, bitmap);
        }

        if(outline_style > 3)  /* Shadow_BtmRight_2px */
        {
            Print_String_To_Bitmap(x + 1, y - 1, string, ST_FALSE, full_flag, bitmap);  // overdraw right + top
            Print_String_To_Bitmap(x - 1, y + 1, string, ST_FALSE, full_flag, bitmap);  // overdraw left + bottom
        }

        if(outline_style == 5)  /* Outline_Plus_BR2px */
        {
            Print_String_To_Bitmap(x + 2, y    , string, ST_FALSE, full_flag, bitmap);
            Print_String_To_Bitmap(x    , y + 2, string, ST_FALSE, full_flag, bitmap);
        }

        Set_Color_Set(GET_1B_OFS(font_style_data,FONT_HDR_POS_CURRENT_COLOR_SET));
    }

    next_x = Print_String_To_Bitmap(x, y, string, ST_TRUE, full_flag, bitmap);

    return next_x;
}


// WZD s17p62
int16_t Print_String_To_Bitmap(int16_t x, int16_t y, char * string, int16_t change_color_ok_flag, int16_t full_flag, SAMB_ptr bitmap)
{
    int16_t next_x;  // DNE in Dasm
    // char character_;
    int16_t str_idx;
    int16_t space_add;
    int16_t space_remainder;
    int16_t current_space;
    int16_t space_count;
    char character;
    uint16_t ptr;

    ptr = 0;

    print_xpos = x;
    print_ypos = y;

    if(full_flag != ST_FALSE)
    {
        current_space = 0;
        space_remainder = 0;
        str_idx = 0;
        space_count = 0;

        while(str_idx++)
        {
            if(string[str_idx] != '\x00') { break; }            /*  ASCII  00h  00d  NUL (null)                  */
            if(string[str_idx] != '\x0D') { break; }            /*  ASCII  0Dh  13d  CR (carriage return)        */
            if(string[str_idx] != '\x14') { break; }            /*  ASCII  14h  20d  DC4 (device control 4)      */
            if(string[str_idx] != '\x19') { break; }            /*  ASCII  15h  21d  NAK (negative acknowledge)  */
            if(string[str_idx] != '\x15') { break; }            /*  ASCII  19h  25d  EM (end of medium)          */
            if(string[str_idx] != '\x1D') { break; }            /*  ASCII  1Dh  29d  GS (group separator)        */
            if(string[str_idx] == '\x20') { space_count++; }    /*  ASCII  20h  32d  SPACE                       */
        }

        if(space_count == 0)
        {
            full_flag = ST_FALSE;
        }

        full_flag -= Get_String_Width(string);

        if(full_flag > 0)
        {
            space_remainder = full_flag % space_count;
            space_add = full_flag / space_count;
        }
        else
        {
            full_flag = ST_FALSE;
        }
    }

    while(string[ptr] != '\0')
    {
        character = string[ptr];

        switch(character)
        {
            case 1:  /* character == '\x01' */  /* ASCII   1h  1d  SOH (start of heading)  */  /* sw_character_values[0] == character */
            {
                if(change_color_ok_flag != ST_FALSE)
                {
                    Set_Normal_Colors_On();
                }
            } break;
            case 2:
            {
                if(change_color_ok_flag != ST_FALSE)
                {
                    Set_Highlight_Colors_On();
                }
            } break;
            case 3:
            {
                if(change_color_ok_flag != ST_FALSE)
                {
                    Set_Special_Colors_On();
                }
            } break;
            case 4:
            {
                if(change_color_ok_flag != ST_FALSE)
                {
                    Set_Highlight_Colors_On();
                }
            } break;
            case 13:  /*  ASCII  0Dh  13d  CR (carriage return)        */
            case 20:  /*  ASCII  14h  20d  DC4 (device control 4)      */
            {
                print_ypos += GET_2B_OFS(font_style_data,FONT_HDR_POS_CURRENT_BASE_HEIGHT);
                print_xpos += x;
            } break;
            case 21:  /*  ASCII  15h  21d  NAK (negative acknowledge)  */
            {
                goto Done;
            } break;
            case 25:  /*  ASCII  19h  25d  EM (end of medium)          */
            case 29:  /*  ASCII  1Dh  29d  GS (group separator)        */
            {
                print_xpos = x + string[ptr+1];
                ptr++;
            } break;

        }

        print_xpos = Print_Character_To_Bitmap(print_xpos, print_ypos, character, bitmap);

        if(full_flag != ST_FALSE)
        {
            if(character == 0x20)  /* ASCII SPACE 0x20 ' ' */
            {
                print_xpos += space_add;

                if(current_space < space_remainder)
                {
                    print_xpos++;
                }
                current_space++;
            }
        }

        ptr++;
    }


Done:

    next_x = print_xpos;

    return next_x;
}


// WZD s17p63
int16_t Print_Right_To_Bitmap(int16_t x, int16_t y, char * string, SAMB_ptr bitmap)
{
    int16_t next_x = 0;
    int16_t string_len = 0;

    string_len = Get_String_Width(string);

    next_x = Print_To_Bitmap((x - string_len), y, string, bitmap);

    return next_x;
}

// WZD s17p64
int16_t Print_Centered_To_Bitmap(int16_t x, int16_t y, char * string, SAMB_ptr bitmap)
{
    int16_t next_x = 0;
    int16_t string_len = 0;

    string_len = Get_String_Width(string);

    next_x = Print_To_Bitmap((x - (string_len / 2)), y, string, bitmap);

    return next_x;
}


// WZD s17p65
int16_t Print_Full_To_Bitmap(int16_t x, int16_t y, char * string, int16_t right_side, SAMB_ptr bitmap)
{
    int16_t next_x;  // DNE in Dasm

    SETMIN(right_side, 0);

    next_x = Print_Display_To_Bitmap(x, y, string, right_side, bitmap);

    return next_x;
}


// WZD s17p66
// MoO2  Module: fonts  Print_To_Bitmap()
/*
    function (0 bytes) Print_To_Bitmap
    Address: 01:00122A6E
        Num params: 4
        Return type: void (1 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        pointer (4 bytes) 
        pointer (4 bytes) 
        Locals:
            signed integer (2 bytes) x
            signed integer (2 bytes) y
            pointer (4 bytes) string
            pointer (4 bytes) bitmap
*/
/*
; a double wrapper for LBX_DrawStyledString that first
; copies the far string into Temp_String in the data
; segment, then draws it left-aligned into an existing
; LBX image allocation
; returns the next X coordinate to draw to
*/
// int16_t LBX_DrawFarString(int16_t x, int16_t y, ofst_t src_ofst, sgmt_t src_sgmt, SAMB_ptr bitmap)
int16_t Print_To_Bitmap_Far(int16_t x, int16_t y, char * string, SAMB_ptr bitmap)
{
    int16_t next_x;  // DNE in Dasm
    // TODO  String_Copy_Far(near_buffer, 0, src_ofst, src_sgmt);
    strcpy(near_buffer, string);
    next_x = Print_To_Bitmap(x, y, near_buffer, bitmap);
    return next_x;
}

// WZD s17p67
// UU_LBX_DrawRtAlgFar()
// WZD s17p68
// UU_LBX_DrawCenteredFar()
// WZD s17p69
// UU_LBX_DrawJustifiedFar()



/*
    WIZARDS.EXE  seg018
*/
/*
    Assembly Code
        Register Variables
            DS  font_style_data:
            SI  
            ES  Screen Page Scan-Line Offset
            BX  start x
            DX  character width
        Code Segment Variables
            next_x
            skip_x
            width

            Print_Character
                mov ax, [bp+x]
                mov [cs:_CS_skip_x], ax
                ...
                mov     bx, [bp+char_num]
                sub     bx, 32                          ; map char from 32-126 to 0-96
                mov     dl, [bx+s_FONT_HEADER.current_font_widths]
                xor     dh, dh
                mov     ax, [ds:s_FONT_HEADER.current_horizontal_spacing]
                add     ax, dx
                add     ax, [cs:_CS_skip_x]
                mov     [cs:_CS_next_x], ax

            Print_Character_ASM_
                mov [cs:_CS_width], dx


*/
// WZD seg018:0000  Font_ColorIndex_2 dw 0            
// WZD seg018:0002  Font_ColorIndex_3 dw 0            
// WZD seg018:0004  current_font_syle dw 0            
// WZD seg018:0006  _CS_skip_x dw 0                   
// WZD seg018:0008  _CS_height dw 0                   
// WZD seg018:000A  _CS_next_x dw 0                   
// WZD seg018:000C  _CS_bitmap_start dw 0             
// WZD seg018:000E  _CS_width dw 0                    
// WZD seg018:0010  VGA_GlyphDraw_VTrim dw 0          
// WZD seg018:0012  VGA_GlyphDraw_FHgt dw 0           
// WZD seg018:0014  VGA_GlyphDraw_DHgt dw 0           
// WZD seg018:0016  VGA_GlyphDraw_Top dw 0            
// WZD seg018:0018  VGA_GlyphDraw_Buffer db 50h dup(0)
int16_t _CS_skip_x;
int16_t _CS_height;
int16_t _CS_next_x;
byte_ptr _CS_bitmap_start;
int16_t _CS_width;

int16_t _CS_skip_y;
int16_t _CS_max_y;
int16_t _CS_draw_height;
byte_ptr _CS_screen_start;
uint8_t _CS_glyph_buffer[80];

// WZD s18p01
// AKA Set_Font()
// MoO2  Module: fonts  Set_Font_Style(style_num, colors)
/*
    sets font style
    sets color1 as current & normal colors
    sets color2 as highlight colors
    sets color3 as special colors
16 color blocks  {0,...,15}

Settings_Screen_Draw()
    Set_Font_Style_Shadow_Down(2, 2, 45, ST_NULL);
    ~== Set_Font_Style_Shadow_Down(2, 2, 45, 0);
    ...
    Set_Font_Colors_15(3, &colors[0]);
    Set_Font_Style_Shadow_Down(3, 15, 45, ST_NULL);

*/
void Set_Font_Style(int16_t font_idx, int16_t color1, int16_t color2, int16_t color3)
{
    int16_t itr;

    color1 = (color1 < COLOR_SET_COUNT) ? color1 : 0;
    color2 = (color2 < COLOR_SET_COUNT) ? color2 : 0;
    color3 = (color3 < COLOR_SET_COUNT) ? color3 : 0;

    m_current_font_style = font_idx;
    m_current_normal_color = color1;
    m_current_highlight_color = color2;
    m_current_special_color = color3;

    for(itr = 0; itr < COLOR_SET_COUNT; itr++)
    {
        font_style_data[(FONT_HDR_POS_CURRENT_COLORS + itr)] = font_colors[((color1 * COLOR_SET_COUNT) + itr)];  // /*  00 */ uint8_t  current_colors[16];
    }

    for(itr = 0; itr < COLOR_SET_COUNT; itr++)
    {
        font_style_data[(FONT_HDR_POS_NORMAL_COLORS + itr)] = font_colors[((color1 * COLOR_SET_COUNT) + itr)];   // /*  14 */ uint8_t  normal_colors[16];
    }

    for(itr = 0; itr < COLOR_SET_COUNT; itr++)
    {
        font_style_data[(FONT_HDR_POS_HIGHLIGHT_COLORS + itr)] = font_colors[((color2 * COLOR_SET_COUNT) + itr)];
    }

    for(itr = 0; itr < COLOR_SET_COUNT; itr++)
    {
        font_style_data[(FONT_HDR_POS_SPECIAL_COLORS + itr)] = font_colors[((color3 * COLOR_SET_COUNT) + itr)];
    }

    font_header->height = font_header->base_height[font_idx];
    font_header->current_vertical_spacing = font_header->vertical_spacing[font_idx];
    font_header->current_base_height = font_header->height;
    font_header->current_horizontal_spacing = font_header->horizontal_spacing[font_idx];
    font_header->shadow_flag = 0;

    for(itr = 0; itr < 96; itr++)
    {
        font_header->current_font_widths[itr] = font_header->font_widths[((font_idx * 96) + itr)];
    }

    for(itr = 0; itr < 96; itr++)
    {
        font_header->current_data_offsets[itr] = font_header->data_offsets[((font_idx * 96) + itr)];
    }

}


// WZD s18p02
/*
MoO2
int16_t next_x Print_Character(int16_t x, int16_t y, uint8_t char_num)
    Address: 01:00121DEB
Locals:
signed integer (4 bytes) screen_pos
signed integer (4 bytes) screen_start
signed integer (4 bytes) line_num
signed integer (4 bytes) i
signed integer (2 bytes) height
signed integer (2 bytes) pos
signed integer (2 bytes) new_x
unsigned integer (1 bytes) data
unsigned integer (1 bytes) skip_count
signed integer (4 bytes) data_offset
*/
int16_t Print_Character(int16_t x, int16_t y, int16_t char_num)
{
    int16_t next_x;
    int16_t width;
    byte_ptr font_data_offset;

    _CS_skip_x = x;

    if(char_num >= 32 && char_num <= 126)
    {
        char_num = char_num - 32;

        _CS_next_x = _CS_skip_x;
        _CS_next_x += font_header->current_horizontal_spacing;
        _CS_next_x += font_header->current_font_widths[char_num];

        width = font_header->current_font_widths[char_num];
        font_data_offset = (font_style_data + font_header->current_data_offsets[char_num]);

        Print_Character_ASM(x, y, width, font_data_offset);

        next_x = _CS_next_x;
    }
    else
    {
        next_x = _CS_skip_x;
    }

    return next_x;
}


// WZD s18p03
int16_t Print_Character_No_Alias(int16_t x, int16_t y, int16_t char_num)
{
    int16_t next_x;  // DNE in Dasm
    int16_t width;  // _DX_
    byte_ptr font_data_offset;

    _CS_skip_x = x;

    if(char_num >= 32 && char_num <= 126)
    {
        char_num = char_num - 32;

        _CS_next_x = _CS_skip_x;
        _CS_next_x += font_header->current_horizontal_spacing;
        _CS_next_x += font_header->current_font_widths[char_num];

        width = font_header->current_font_widths[char_num];
        font_data_offset = (font_style_data + font_header->current_data_offsets[char_num]);

        Print_Character_No_Alias_ASM(x, y, width, font_data_offset);

        next_x = _CS_next_x;
    }
    else
    {
        next_x = _CS_skip_x;
    }

    return next_x;
}


// WZD s18p04
int16_t Print_Character_To_Bitmap(int16_t x, int16_t y, uint8_t char_num, SAMB_ptr bitmap)
{
    int16_t next_x;  // DNE in Dasm
    int16_t width;  // _DX_
    byte_ptr font_data_offset;
    byte_ptr bitmap_pos;  // _DI_
    uint8_t font_data_byte;
    uint8_t skip_count;
    uint8_t repeat_count;
    uint8_t color_index;
    uint8_t palette_index;
    int16_t DBG_width;
    byte_ptr DBG_bitmap_pos;

    next_x = 0;  // DNE in Dasm

    _CS_next_x = x;

    _CS_height = FLIC_GET_HEIGHT(bitmap);

    if(
        (char_num >= 32)
        &&
        (char_num <= 126)
    )
    {

        bitmap_pos = ((bitmap + SZ_PARAGRAPH_B) + ((x * _CS_height) + y));
        DBG_bitmap_pos = bitmap_pos;

        _CS_skip_x = x;

        char_num = char_num - 32;

        width = font_header->current_font_widths[char_num];
        DBG_width = width;

        _CS_next_x = _CS_skip_x;
        _CS_next_x += width;
        _CS_next_x += font_header->current_horizontal_spacing;

        font_data_offset = (font_style_data + font_header->current_data_offsets[char_num]);

// BEGIN:  ~== Print_Character_ASM()
        _CS_bitmap_start = bitmap_pos;
        while(width)
        {
            font_data_byte = *font_data_offset++;
            if((font_data_byte & 0x80) == 0)
            {
                repeat_count = ((font_data_byte & 0xF0) >> 4);
                while(repeat_count--)
                {
                    color_index = (font_data_byte & 0x0F);  // _BX_ in Dasm
                    palette_index = font_header->current_colors[color_index];
                    *bitmap_pos++ = palette_index;
                }
            }
            /* Type: skip */
            else if((font_data_byte & 0x7F) != 0)
            {
                skip_count = (font_data_byte & 0x7F);
                bitmap_pos += skip_count;
            }
            /* Type: end/skip column */
            else
            {
                bitmap_pos = _CS_bitmap_start;
                bitmap_pos += _CS_height;
                _CS_bitmap_start = bitmap_pos;
                width--;
                assert(_CS_bitmap_start == (DBG_bitmap_pos + ((DBG_width - width) * _CS_height)));
            }

        }
// END:  ~== Print_Character_ASM()

        next_x = _CS_next_x;

    }

    return next_x;
}


// WZD s18p05
void Print_Character_ASM(int16_t x_start, int16_t y_start, int16_t width, byte_ptr font_data_offset)
{
    byte_ptr screen_start;
    byte_ptr screen_pos;
    uint8_t font_data_byte;
    uint8_t skip_count;
    uint8_t repeat_count;
    uint8_t color_index;
    uint8_t palette_index;

    screen_start = current_video_page + ((y_start * SCREEN_WIDTH) + x_start);

    screen_pos = screen_start;
    while(width)
    {

        font_data_byte = *font_data_offset++;

        /* Type: end/skip column */
        if(font_data_byte == 0x80)
        {
            width--;
            screen_start++;
            screen_pos = screen_start;
            continue;
        }

        /* Type: skip */
        if(
            (font_data_byte & 0x80) != 0
            &&
            (font_data_byte & 0x7F) != 0
        )
        {
            skip_count = (font_data_byte & 0x7F);

            while(skip_count--)
            {
                screen_pos += SCREEN_WIDTH;
            }
        }

        // @@Nay_Negative
        if((font_data_byte & 0x80) == 0)
        {
            repeat_count = ((font_data_byte & 0xF0) >> 4);

            while(repeat_count--)
            {
                color_index = (font_data_byte & 0x0F);

                // *screen_pos = font_header->current_colors[color_index];
                // FTW palette_index = font_header->current_colors[color_index];
                palette_index = GET_1B_OFS(font_style_data,FONT_HDR_POS_CURRENT_COLORS + color_index);

                *screen_pos = palette_index;

                screen_pos += SCREEN_WIDTH;
            }
        }
    }

}


// WZD s18p06
/*

*/
void Print_Character_No_Alias_ASM(int16_t x_start, int16_t y_start, int16_t width, byte_ptr font_data_offset)
{
    byte_ptr screen_start;
    byte_ptr screen_pos;
    uint8_t font_data_byte;
    uint8_t skip_count;
    uint8_t repeat_count;
    uint8_t color_index;
    uint8_t palette_index;

    screen_start = current_video_page + ((y_start * SCREEN_WIDTH) + x_start);

    screen_pos = screen_start;
    while(width)
    {
        font_data_byte = *font_data_offset++;

        /* Type: end/skip column */
        if(font_data_byte == 0x80)
        {
            width--;
            screen_start++;
            screen_pos = screen_start;
            continue;
        }

        /* Type: skip */
        if(
            (font_data_byte & 0x80) != 0
            &&
            (font_data_byte & 0x7F) != 0
        )
        {
            skip_count = (font_data_byte & 0x7F);

            while(skip_count--)
            {
                screen_pos += SCREEN_WIDTH;
            }
        }

        // @@Nay_Negative
        if((font_data_byte & 0x80) == 0)
        {
            repeat_count = ((font_data_byte & 0xF0) >> 4);

            if((font_data_byte & 0x0F) == 0)
            {
                screen_pos += SCREEN_WIDTH;
            }
            else
            {
                while(repeat_count--)
                {
                    color_index = (font_data_byte & 0x0F);  // _BX_ in Dasm

                    palette_index = font_header->current_colors[color_index];

                    *screen_pos = palette_index;
                    screen_pos += SCREEN_WIDTH;
                }
            }
        }
    }

}


// WZD s18p07
int16_t Get_String_Width(char * string)
{
    int16_t pos;
    int16_t width;
    int16_t char_num = 0;
    int16_t horizontal_spacing;

    pos = 0;
    width = 0;

    // MoM - ASM: horizontal_spacing
    // MoO2: horizontal_spacing = font_header.current_horizontal_spacing;
    // horizontal_spacing = font_style_data[72];
    horizontal_spacing = GET_2B_OFS(font_style_data,FONT_HDR_POS_CURRENT_HORIZONTAL_SPACING);


Next_Char:
    char_num = string[pos++];  // ~== LODSB

    char_num -= 32;  // subtract the offset to the first printable character to get the index into the font data tables

    // Non-Printable Character
    if(char_num < 0)
    {
        char_num += 32;
        if(char_num == 0)        /* ASCII  00h  00d  NUL (null) */
        {
            goto Done;
        }
        else if(char_num == 13)  /* ASCII  0Dh  13d  CR (carriage return) */
        {
            goto Done;
        }
        else if(char_num == 20)  /* ASCII  14h  20d  DC4 (device control 4) */
        {
            goto Done;
        }
        else if(char_num == 21)  /* ASCII  15h  21d  NAK (negative acknowledge) */
        {
            goto Done;
        }
        else if(char_num == 25)  /* ASCII  19h  25d  EM (end of medium) */
        {
            goto Done;
        }
        else if(char_num == 29)  /* ASCII  1Dh  29d  GS (group separator) */
        {
            goto Done;
        }
        else
        {
            goto Next_Char;
        }
    }

    if(char_num - 94 > 0)
    {
        goto Next_Char;
    }

    // MoO2: font_header.current_font_widths[char_num]
    // width = width + font_style_data[74 + char_num];

    // width += GET_1B_OFS(font_style_data,FONT_HDR_POS_CURRENT_FONT_WIDTHS + char_num);
    width += font_header->current_font_widths[char_num];
    width += horizontal_spacing;  // MoO2:    ax, font_header.current_horizontal_spacing

goto Next_Char;


Done:
    width -= horizontal_spacing;  // remove the extra one that was added to the last character

    return width;
}



// WZD s18p08
// drake178: VGA_DrawPartialChar()
// ¿ MoO2  Module: fonts  Print_Clipped_Character() ?
// static void lbxfont_plotchar_limit(int x, int y, char c, int xskip, int char_w, int yskip, int char_h, uint16_t pitch, int scale)
/*

    function (0 bytes) Print_Clipped_Character
    Address: 01:0012260F
        Num params: 3
        Return type: signed integer (2 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        unsigned integer (1 bytes) 
        Locals:
            signed integer (2 bytes) x
            signed integer (2 bytes) y
            unsigned integer (1 bytes) character
            signed integer (2 bytes) skip_x
            signed integer (2 bytes) height
            signed integer (2 bytes) width
            signed integer (2 bytes) spacing
            signed integer (2 bytes) cpos
            signed integer (2 bytes) skip_y
            signed integer (2 bytes) max_y
            signed integer (2 bytes) next_x
            array (2 bytes) stringer
            Num elements:    2, Type:                unsigned integer (4 bytes) 

*/

/*
; draws a single font character into the draw segment
; by setting up and calling VGA_DrawPartialGlyph
; unlike most similar functions, this one DOES NOT
; return the next X coordinate to draw to

*/
void Print_Clipped_Character(int16_t x, int16_t y, char char_num, int16_t skip_x, int16_t width, int16_t skip_y, int16_t max_y)
{
    // uint16_t si = GET_LE_16(&lbxfontdata[0xaa + char_num * 2]);
    // uint8_t * p = &lbxfontdata[si];
    byte_ptr font_data_offset;

    font_data_offset = (font_style_data + font_header->current_data_offsets[char_num]);

    while(skip_x)
    {
        if(*font_data_offset++ == 0x80)
        {
            skip_x--;
        }
    }

// mov     ax, [bp+y]
// mov     cx, ax
// shl     ax, 1
// shl     ax, 1
// add     ax, cx   ; * 5 as a segment address = * 80 total bytes which, since each byte is 4 pixels, equals the draw row
// add     ax, [current_video_page]
// mov     es, ax
// mov     dx, [bp+width]
// mov     bx, [bp+x]
// mov     ax, [bp+skip_y]
// mov     cx, [bp+max_y]
// mov     di, [font_style_data]
// mov     ds, di


    _CS_skip_y = skip_y;
    _CS_max_y = max_y;
    _CS_draw_height = (max_y - skip_y);
    _CS_width = width;
    _CS_screen_start = current_video_page + (y * SCREEN_WIDTH);

    Print_Clipped_Letter(x, font_data_offset);

// ; draws a partial font glyph into the draw segment
// ; requires the following setup beforehand:
// ;   ds:si = character glyph data pointer
// ;   es = segment of the top scan line
// ;   ax = top lines to trim
// ;   bx = X pixel position
// ;   cx = original font height
// ;   dx = character width
// ;
// ; font format - glyph_width columns, for each column
// ;   sign bit: skip if set, dots if not
// ;     can skip up to 127 pixels, 0 ($80) is next col
// ;   otherwise bits 0-3: font color, 4-6: repeat count
// ;     can repeat up to 7 pixels in up to 16 colors

// mov     [cs:VGA_GlyphDraw_VTrim], ax
// mov     [cs:VGA_GlyphDraw_FHgt], cx
// sub     cx, ax
// mov     [cs:VGA_GlyphDraw_DHgt], cx
// mov     [cs:_CS_width], dx
// mov     ax, es
// mov     [cs:VGA_GlyphDraw_Top], ax

// VGA_GlyphDraw_VTrim = skip_y
// VGA_GlyphDraw_FHgt = max_y
// VGA_GlyphDraw_DHgt = (max_y - skip_y)
// _CS_width = width
// VGA_GlyphDraw_Top = current_video_page + (y * SCREEN_WIDTH)

}


// WZD s18p09
// drake178: VGA_DrawPartialGlyph()
// ¿ MoO2  Module: fonts  Print_Clipped_Letter() ?
/*

    function (0 bytes) Print_Clipped_Letter
    Address: 01:001227EC
        Num params: 4
        Return type: void (1 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        unsigned integer (1 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        Locals:
            signed integer (2 bytes) x
            signed integer (2 bytes) y
            unsigned integer (1 bytes) char_num
            signed integer (2 bytes) skip_x
            signed integer (2 bytes) width
            signed integer (2 bytes) skip_y
            signed integer (2 bytes) max_y
            signed integer (4 bytes) screen_pos
            signed integer (4 bytes) screen_start
            signed integer (4 bytes) line_num
            signed integer (4 bytes) i
            signed integer (4 bytes) screen_pos_start
            signed integer (4 bytes) screen_pos_end
            signed integer (2 bytes) data_offset
            signed integer (2 bytes) height
            signed integer (2 bytes) pos
            signed integer (2 bytes) new_x
            unsigned integer (1 bytes) data
            unsigned integer (1 bytes) skip_count

*/
/*
draws a partial font glyph into the draw segment
requires the following setup beforehand:
  ds:si = character glyph data pointer
  es = segment of the top scan line
  ax = top lines to trim
  bx = X pixel position
  cx = original font height
  dx = character width

font format - glyph_width columns, for each column
  sign bit: skip if set, dots if not
    can skip up to 127 pixels, 0 ($80) is next col
  otherwise bits 0-3: font color, 4-6: repeat count
    can repeat up to 7 pixels in up to 16 colors
*/
// void Print_Clipped_Letter(int16_t x, int16_t y, char char_num, int16_t skip_x, int16_t width, int16_t skip_y, int16_t max_y)
void Print_Clipped_Letter(int16_t x_start, byte_ptr font_data_offset)
{
    int16_t  itr = 0;
    uint8_t * ptr_glyhp_buffer = 0;
    uint8_t font_data_byte = 0;
    uint16_t repeat_count = 0;
    uint8_t color_index = 0;
    uint8_t palette_index = 0;
    uint8_t skip_count = 0;
    int16_t draw_height = 0;
    byte_ptr screen_start = 0;
    byte_ptr screen_pos = 0;
    uint8_t glyph_buffer_byte = 0;

    while(_CS_width)
    {

        // DEDU  Is there a reason why this is done on every iteration?
        for(itr = 0; itr < _CS_max_y; itr++)
        {
            _CS_glyph_buffer[itr] = 0xFF;
        }

        ptr_glyhp_buffer = &_CS_glyph_buffer[0];

        font_data_byte = *font_data_offset++;

        if(!(font_data_byte & 0x80))
        {
            repeat_count = (font_data_byte >> 4);
            color_index = (font_data_byte & 0x0F);
            palette_index = font_style_data[color_index];
            while(repeat_count--)
            {
                // ES:DI++ = palette_index;
                // ES = screen_start = current_video_page + (y_start * SCREEN_WIDTH)
                // DS = ptr_glyhp_buffer = &_CS_glyph_buffer[0]
                *ptr_glyhp_buffer++ = palette_index;
            }

        }
        else if(font_data_byte & 0x7f)
        {
            skip_count = (font_data_byte & 0x7F);
            ptr_glyhp_buffer += skip_count;
        }
        else
        {
            break;
        }


        /*
            BEGIN:  copy glyph buffer to screen
        */
        // screen_start = current_video_page + ((y_start * SCREEN_WIDTH) + x_start);
        // screen_pos = screen_start;
        screen_pos = _CS_screen_start + x_start;
        ptr_glyhp_buffer = &_CS_glyph_buffer[0];
        draw_height = _CS_draw_height;
        while(draw_height--)
        {
            glyph_buffer_byte = *ptr_glyhp_buffer++;
            screen_pos++;
            if(glyph_buffer_byte != 0xFF)
            {
                screen_pos--;
                *screen_pos++ = glyph_buffer_byte;
            }
            screen_pos += SCREEN_WIDTH;  // same column, one pixel down
        }
        /*
            END:  copy glyph buffer to screen
        */

        _CS_width--;
    }

}



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
int16_t Get_Font_Height(void)
{
    int16_t font_height;

    // DOS  farpeekw(font_style_data, 0x10); // s_FONT_HEADER.height

    font_height = GET_2B_OFS(font_style_data, FONT_HDR_POS_HEIGHT);

    return font_height;
}

// WZD s19p11
// WZD s19p12
// WZD s19p13
// WZD s19p14



/*
    WIZARDS.EXE  seg020
*/

// WZD s20p01
/*

updates current_palette from palette_data, the palette entry loaded from FONTS.LBX

*/
void Load_Palette(int entry, int start_color, int end_color)
{
    int color_start;
    int color_count;
    int itr;

    palette_data = LBX_Reload(font_name, entry+2, palette_block);
    // 1oom: lbxpal_palette_inlbx = palette_data;

    // font_colors = (palette_data + (16 * (48)));  // 768
    font_colors = &palette_data[768];

    // UU_gsa_Palette_Data = (palette_data + (16 * (48 + 16)));  // 400h

    mouse_palette = (palette_data + (16 * (48 + 16 + 16)));  // 1280
    // 5376 - 1280 = 4096
    // CURSOR_WIDTH == 16; CURSOR_HEIGHT == 16
    // 16 * 16 = 256
    // 4096 / 256 = 16

    
    // array of 24 color fractions (B-G-R-Percent)
    // used to set shaded replacement colors for each color index
    // gsa_ShadingColors = (palette_data + (16 * (48 + 16 + 16 + 256)));  // 5376  (24 * 4 = 96) 0x60  6 PR
    remap_colors = (palette_data + (16 * (48 + 16 + 16 + 256)));  // FONTS.LBX, 2; @0x1500  5376
    // 1oom
    // lbxpal_ctableparam = Pal + 0x1500;
    // 1oom: lbxpal_ctableparam = palette_data + 0x1500;


    if(start_color == ST_UNDEFINED)
    {
        color_start = 0;
        color_count = 256;
    }
    else
    {
        color_start = start_color;
        color_count = (end_color - start_color) + 1;
    }

    // TODO  make this look more like the Dasm - see 1oom
    for(itr = 0; itr < (color_count * 3); itr++)
    {
        *(current_palette + (color_start * 3) + itr) = *(palette_data + (color_start * 3) + itr);
    }

    Set_Font_Style(0, 0, 0, 0);

    // HERE: not in MoO1
    if(start_color == ST_UNDEFINED)
    {
        Set_Palette_Changes(0, 255);
    }
    else
    {
        Set_Palette_Changes(start_color, end_color);
    }

}


// WZD s20p02
// drake178: VGA_ResetShade0()
void Update_Remap_Gray_Palette(void)
{
    Create_Remap_Palette(0, 0, 0, 0, 50);
}


// WZD s20p03
// drake178: VGA_SetShades_Grey0()
void Calculate_Remap_Colors(void)
{
    int16_t itr_blocks;
    int16_t block;
    uint16_t block_ofst;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t percent;

    Create_Remap_Palette(0, 0, 0, 0, 50);

    for(itr_blocks = 1; itr_blocks < 24; itr_blocks++)
    {
        block = itr_blocks;
        block_ofst = block * 4;  // sizeof: uint8_t r,g,b,%
        red     = GET_1B_OFS(remap_colors, block_ofst + 0);
        green   = GET_1B_OFS(remap_colors, block_ofst + 1);
        blue    = GET_1B_OFS(remap_colors, block_ofst + 2);
        percent = GET_1B_OFS(remap_colors, block_ofst + 3);
        Create_Remap_Palette(block, red, green, blue, percent);
    }

}


// WZD s20p04
// drake178: VGA_SetShades_Range()
void Update_Remap_Color_Range(int16_t first_set, int16_t last_set)
{
    int16_t itr_blocks;
    int16_t block;
    uint16_t block_ofst;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t percent;

    // assert(first_set >= last_set);

    if(last_set < first_set)
    {
        Swap_Short(&first_set, &last_set);
    }

    if(first_set == 0)
    {
        Create_Remap_Palette(0, 0, 0, 0, 50);  // same as default in Calculate_Remap_Colors()

        first_set = 1;
    }

    // NOTE: LTE, therefore "inclusive"
    for(itr_blocks = first_set; itr_blocks <= last_set; itr_blocks++)
    {
        block = itr_blocks;
        block_ofst = block * 4;  // sizeof: uint8_t r,g,b,%
        red     = GET_1B_OFS(remap_colors, block_ofst + 0);
        green   = GET_1B_OFS(remap_colors, block_ofst + 1);
        blue    = GET_1B_OFS(remap_colors, block_ofst + 2);
        percent = GET_1B_OFS(remap_colors, block_ofst + 3);
        Create_Remap_Palette(block, red, green, blue, percent);
    }

}


// WZD s20p05
void Set_Palette_Changes(int start_color, int end_color)
{
    int itr;

    for(itr = start_color; itr < end_color; itr++)
    {
        *(palette_flags + itr) = ST_TRUE;
    }

}

// WZD s20p06
void Clear_Palette_Changes(int start_color, int end_color)
{
    int itr;

    for(itr = start_color; itr < end_color; itr++)
    {
        *(palette_flags + itr) = ST_FALSE;
    }

}



// WZD s20p07
// WZD s20p08
// WZD s20p09
// WZD s20p10

// WZD s20p11
/*
    0: normal, 1: reverse, -1: disabled
*/
void Reset_Cycle_Palette_Color(void)
{
    cycle_direction_flag = -1;
}

// WZD s20p12
// drake178: ¿ ?
/*
MoO2
Module: palette
    function (0 bytes) Cycle_Palette_Color
    Address: 01:00131B5D
        Num params: 4
        Return type: void (1 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        Locals:
            signed integer (2 bytes) color_num
            signed integer (2 bytes) red_min
            signed integer (2 bytes) green_min
            signed integer (2 bytes) blue_min
            signed integer (2 bytes) red_max
            signed integer (2 bytes) green_max
            signed integer (2 bytes) blue_max
            signed integer (2 bytes) step_value
            signed integer (4 bytes) delta_red
            signed integer (4 bytes) delta_green
            signed integer (4 bytes) delta_blue
            signed integer (4 bytes) primary_color
            signed integer (4 bytes) store_red
            signed integer (4 bytes) store_blue
            signed integer (4 bytes) store_green
*/
/*

*/
void Cycle_Palette_Color__STUB(int16_t color_num, int16_t red_min, int16_t green_min, int16_t blue_min, int16_t red_max, int16_t green_max, int16_t blue_max, int16_t step_value)
{





}


// WZD s20p13
// drake178: ¿ ?
/*
MoO2
Module: palette
    function (0 bytes) Update_Cycle
    Address: 01:00131E6B
        Num params: 2
        Return type: void (1 bytes) 
        pointer (4 bytes) 
        pointer (4 bytes) 
        Locals:
            pointer (4 bytes) color_min
            pointer (4 bytes) color_max
*/
// Update_Cycle(int *color_min, int *color_max)

// WZD s20p14
// drake178: UU_VGA_CreateHues()
// NX_VGA_CreateHues(int First_DAC_Reg, int Steps, int B_Red, int B_Grn, int B_Blu, int T_Red, int T_Grn, int T_Blu)



// /*
//     WIZARDS.EXE  seg021
//     (assembly code)
// */
// 
// // WZD seg021:000C                                                 ; Segment type: Pure code
// // WZD seg021:000C                                                 segment seg021 byte public 'CODE' use16
// // WZD seg021:000C                                                 assume cs:seg021
// // WZD seg021:000C                                                 ;org 0Ch
// // WZD seg021:000C                                                 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing
// // WZD seg021:000C 00 00                                           
// uint16_t CS_red1;
// // WZD seg021:000E 00 00                                           
// uint16_t CS_blue1;
// // WZD seg021:0010 00 00                                           
// uint16_t CS_green1;
// // WZD seg021:0012 00 00                                           Pass1_DstColor_1 dw 0                   ; DATA XREF: VGA_SlideColors+18w ...
// // WZD seg021:0014 00 00                                           Pass1_Count dw 0                        ; DATA XREF: VGA_SlideColors+23w ...
// // WZD seg021:0016 00 00                                           Pass1_SrcColor1 dw 0                    ; DATA XREF: VGA_SlideColors+2Aw ...
// // WZD seg021:0018 00 00                                           Pass2_DstColor_1 dw 0                   ; DATA XREF: VGA_SlideColors+31w ...
// // WZD seg021:001A 00 00                                           Pass2_Count dw 0                        ; DATA XREF: VGA_SlideColors+38w ...
// // WZD seg021:001C 00 00                                           Pass2_SrcColor_1 dw 0                   ; DATA XREF: VGA_SlideColors+45w ...
// // WZD seg021:001E 00 00                                           Color_Wave_Start dw 0                   ; DATA XREF: UU_VGA_ColorWave+1Fw ...
// // WZD seg021:0020 00 00                                           Color_Wave_HalfSize dw 0                ; DATA XREF: UU_VGA_ColorWave+27w ...
// // WZD seg021:0022 00 00                                           Color_Wave_Step dw 0                    ; DATA XREF: UU_VGA_ColorWave+17w ...
// // WZD seg021:0024 00 00                                           
// uint16_t CS_closest;
// // WZD seg021:0026 00 00                                           
// uint16_t CS_closest_dif;
// // WZD seg021:0028 00                                              
// uint16_t CS_Total_Target_Red;
// // WZD seg021:0029                                                 algn_1CE09:
// // WZD seg021:0029 00                                              align 2
// // WZD seg021:002A 00                                              
// uint16_t CS_Total_Target_Green;
// // WZD seg021:002B                                                 algn_1CE0B:
// // WZD seg021:002B 00                                              align 2
// // WZD seg021:002C 00                                              
// uint16_t CS_Total_Target_Blue;
// // WZD seg021:002D                                                 algn_1CE0D:
// // WZD seg021:002D 00                                              align 2
// // WZD seg021:002E 00 00                                           
// uint16_t CS_Orig_Multiplier;
// // WZD seg021:002E                                                                                         ; original color percent * 256 / 100
// // WZD seg021:0030 00 00                                           
// uint16_t itr_remap_palette;
// // WZD seg021:0032 00                                              
// uint16_t CS_Perc_Target_Red;
// // WZD seg021:0033                                                 algn_1CE13:
// // WZD seg021:0033 00                                              align 2
// // WZD seg021:0034 00                                              
// uint16_t CS_Perc_Target_Green;
// // WZD seg021:0035                                                 algn_1CE15:
// // WZD seg021:0035 00                                              align 2
// // WZD seg021:0036 00                                              
// uint16_t CS_Perc_Target_Blue;
// // WZD seg021:0037                                                 algn_1CE17:
// // WZD seg021:0037 00                                              align 2
// // WZD seg021:0038


// WZD s21p01
// MoO2: Refresh_Palette |-> Store_Palette_Block_
// 1oom :: uipal.c :: void ui_palette_set_n(void)
// AKA VGA_DAC_Write()
// IBM-PC, VGA, MS-DOS: write to the VGA-DAC
// MS-Windows: write to PFL-provded palette buffer
// PLATFORM  void Apply_Palette(void)
// PLATFORM  {
// PLATFORM  
// PLATFORM      int16_t itr;
// PLATFORM  
// PLATFORM      for(itr = 0; itr < 256; itr++)
// PLATFORM      {
// PLATFORM          if(*(current_palette + 768 + itr) == 1)
// PLATFORM          {
// PLATFORM              *(g_Palette + itr) = *(current_palette + itr);
// PLATFORM  
// PLATFORM              *(g_Palette_XBGR + (itr * 4) + 3) = 0x00;
// PLATFORM              *(g_Palette_XBGR + (itr * 4) + 2) = (*(current_palette + (itr * 3) + 0) << 2);
// PLATFORM              *(g_Palette_XBGR + (itr * 4) + 1) = (*(current_palette + (itr * 3) + 1) << 2);
// PLATFORM              *(g_Palette_XBGR + (itr * 4) + 0) = (*(current_palette + (itr * 3) + 2) << 2);
// PLATFORM          }
// PLATFORM  
// PLATFORM      }
// PLATFORM  
// PLATFORM  }
#ifdef _STU_WIN

#include "MOM.H"

// TODO  turn this into MoX64 or somesuch; not PFL, just not OG/DOS/16-bit/6-bpp (VGA)

/*
    WIZARDS.EXE  seg021
*/

// WZD s21p01
// MoO2: Refresh_Palette |-> Store_Palette_Block_
// 1oom :: uipal.c :: void ui_palette_set_n(void)
// AKA VGA_DAC_Write()
// IBM-PC, VGA, MS-DOS: write to the VGA-DAC
// MS-Windows: write to PFL-provded palette buffer
/*
    copy current_palette to PFL_palette

*/
void Apply_Palette(void)
{

    int16_t itr;

    for (itr = 0; itr < 256; itr++)  // TODO  ~ #define Color Count
    {
        if (*(current_palette + 768 + itr) == 1)  // TODO  ~ #define Palette Flags Offset
        {
            // ¿ XBGR pixel format ?
            *(platform_palette_buffer + (itr * 4) + 3) = 0x00;
            *(platform_palette_buffer + (itr * 4) + 2) = (*(current_palette + (itr * 3) + 0) << 2);
            *(platform_palette_buffer + (itr * 4) + 1) = (*(current_palette + (itr * 3) + 1) << 2);
            *(platform_palette_buffer + (itr * 4) + 0) = (*(current_palette + (itr * 3) + 2) << 2);
        }
    }

    // Here, in MoO1 and MoM, it clears the palette change flags - REP STOSW of 0 128 times

}

#endif
#ifdef _STU_SDL2
// WZD s21p01
void Apply_Palette(void)
{
    // uint8_t * pal;
    int itr;

    // pal = (uint8_t*)current_palette;
    // 
    // for(itr = 0; itr < 256; itr++)
    // {
    // 
    //     if(*(current_palette + 768 + itr) == ST_TRUE)
    //     {
    // 
    //         platform_palette_buffer[itr].r = *pal++ << 2;
    //         platform_palette_buffer[itr].g = *pal++ << 2;
    //         platform_palette_buffer[itr].b = *pal++ << 2;
    //         platform_palette_buffer[itr].a = 0xFF;
    // 
    //     }
    // 
    // }

    for(itr = 0; itr < 256; itr++)  // TODO  ~ #define Color Count
    {

        if(*(current_palette + 768 + itr) == ST_TRUE)  // TODO  ~ #define Palette Flags Offset
        {

            platform_palette_buffer[itr].r = (*(current_palette + (itr * 3) + 0) << 2);
            platform_palette_buffer[itr].g = (*(current_palette + (itr * 3) + 1) << 2);
            platform_palette_buffer[itr].b = (*(current_palette + (itr * 3) + 2) << 2);
            platform_palette_buffer[itr].a = 0xFF;

        }

    }

    // HERE: in MoO1 and MoM, it clears the palette change flags - REP STOSW of 0 128 times

}
#endif

// WZD s21p02
// drake178: VGA_DAC_DimWrite()
// ¿ MoO2  Module: palstore  Cycle_Palette_() Darken_Palette_() Tint_Palette_() ?
/*
    percent <= 0:       just Apply_Palette()
    percent >= 100:     set VGA DAC to all zeroes, if palette change flag is true
    1 <= percent <= 99: 
*/
void Cycle_Palette(int16_t percent)
{
    int16_t vpercent;
    uint16_t color_multiplier;
    int16_t itr;
    uint8_t palette_change_flag;
    uint8_t * tmpcurrpal;
    uint16_t ofst;
    uint8_t color_red;
    uint8_t color_grn;
    uint8_t color_blu;

    vpercent = 100 - percent;
    
    if(vpercent > 0)                    /* percent < 100 */
    {
        if(vpercent < 100)              /* (percent < 100) && (percent > 0) */
        {
            color_multiplier = ((percent << 8) / 100) & 0x00FF;  // ¿ Fixed_Point Math ~== << 8 ?
            tmpcurrpal = (uint8_t *)(current_palette);
            for(itr = 0; itr < 256; itr++)
            {
                palette_change_flag = PALETTE_FLAG(itr);
                if(palette_change_flag != ST_FALSE)
                {
                    ofst = itr * 3;

#ifdef _STU_SDL2
                    platform_palette_buffer[itr].a = 0xFF;
                    color_red = *(tmpcurrpal + ofst++);
                    platform_palette_buffer[itr].b = (((color_red * color_multiplier) >> 8) << 2);
                    color_grn = *(tmpcurrpal + ofst++);
                    platform_palette_buffer[itr].g = (((color_grn * color_multiplier) >> 8) << 2);
                    color_blu = *(tmpcurrpal + ofst++);
                    platform_palette_buffer[itr].r = (((color_blu * color_multiplier) >> 8) << 2);
#endif
#ifdef _STU_WIN
                    *(platform_palette_buffer + (itr * 4) + 3) = 0x00;
                    color_red = *(tmpcurrpal + ofst++);
                    *(platform_palette_buffer + (itr * 4) + 2) = (((color_red * color_multiplier) >> 8) << 2);
                    color_grn = *(tmpcurrpal + ofst++);
                    *(platform_palette_buffer + (itr * 4) + 1) = (((color_grn * color_multiplier) >> 8) << 2);
                    color_blu = *(tmpcurrpal + ofst++);
                    *(platform_palette_buffer + (itr * 4) + 0) = (((color_blu * color_multiplier) >> 8) << 2);
#endif

                }
            }
        }
        else                            /* (percent < 100) && (percent < 0) */
        {
            Apply_Palette();
            goto Done;
        }
    }
    else
    {
        for(itr = 0; itr < 256; itr++)
        {
            palette_change_flag = PALETTE_FLAG(itr);
            if(palette_change_flag != ST_FALSE)
            {
                // set hardware palette color to {0,0,0} black
                // ¿ ~== set platform_palette_buffer color to {0,0,0} black ?
#ifdef _STU_SDL2
                platform_palette_buffer[itr].a = 0xFF;
                platform_palette_buffer[itr].b = 0x00;
                platform_palette_buffer[itr].g = 0x00;
                platform_palette_buffer[itr].r = 0x00;
#endif
#ifdef _STU_WIN

                *(platform_palette_buffer + (itr * 4) + 3) = 0x00;
                *(platform_palette_buffer + (itr * 4) + 2) = 0x00;
                *(platform_palette_buffer + (itr * 4) + 1) = 0x00;
                *(platform_palette_buffer + (itr * 4) + 0) = 0x00;
#endif
            }
        }

    }
    ;
    // ~== REP STOSW
    memset((uint8_t *)(current_palette + 768), 0x00, 256);  // ~== `REP STOSB`

Done:
    return;  // DNE in Dasm - VSBS

}



// WZD s21p03
// drake178: VGA_ShadeScreen()

// WZD s21p04
// drake178: VGA_SlideColors()
/*
    called from Lair_Confirm_Draw()
    VGA_SlideColors(247, 8, notify_color_slide_cycle);

*/
void VGA_SlideColors__STUB(int16_t First_Color, int16_t Count, int16_t ShiftBy)
{

/*
mov     dx, 0
mov     ax, [bp+ShiftBy]
mov     bx, [bp+Count]
div     bx
mov     [bp+ShiftBy], dx


add     dx, [bp+First_Color]
mov     [cs:Pass1_DstColor_1], dx


mov     ax, [bp+Count]
sub     ax, [bp+ShiftBy]
mov     [cs:Pass1_Count], ax


mov     ax, [bp+First_Color]
mov     [cs:Pass1_SrcColor1], ax


mov     ax, [bp+First_Color]
mov     [cs:Pass2_DstColor_1], ax


mov     ax, [bp+ShiftBy]
mov     [cs:Pass2_Count], ax


mov     ax, [bp+Count]
sub     ax, [bp+ShiftBy]
add     ax, [bp+First_Color]
mov     [cs:Pass2_SrcColor_1], ax


mov     ax, [current_palette]
mov     ds, ax
mov     si, [cs:Pass1_SrcColor1]
mov     ax, si
shl     si, 1
add     si, ax
mov     bx, [cs:Pass1_DstColor_1]
mov     cx, [cs:Pass1_Count]
*/


// Wait for Vsync
// Wait for Vsync


/*
loc_1D105:
cli
mov     al, bl
out     dx, al
inc     dx
lodsb
out     dx, al
lodsb
out     dx, al
lodsb
out     dx, al
sti
dec     dx
inc     bx
loop    loc_1D105
*/

/*
mov     si, [cs:Pass2_SrcColor_1]
mov     ax, si
shl     si, 1
add     si, ax
mov     bx, [cs:Pass2_DstColor_1]
mov     cx, [cs:Pass2_Count]
cmp     cx, 0
jz      short loc_1D14F
*/



// Wait for Vsync
// Wait for Vsync



/*
loc_1D13F:
cli
mov     al, bl
out     dx, al
inc     dx
lodsb
out     dx, al
lodsb
out     dx, al
lodsb
out     dx, al
sti
dec     dx
inc     bx
loop    loc_1D13F
*/



/*
// clear all palette change flags
loc_1D14F:
mov     ax, ds
mov     es, ax
assume es:dseg
mov     di, 768
mov     cx, 128
mov     ax, 0
rep stosw
*/

}


// WZD s21p05
// drake178: UU_VGA_ColorWave()



// WZD s21p06
// drake178: VGA_ShadeColorMatch
// MoO2: 
// 1oom: lbxpal_build_colortable()
// ANI: make_cfit_table()
/*
    dst red = (src1 red * vpercent) + (src2 red * percent)
    e.g.,
        25% of src1 and 75% of src2
        25% of red parameter and 75% of screen red
        ¿ 25% of full saturation ~== "brighten" ?

    passed in color is color2 / 2nd portion of mix/blend

    calculates color3 from color1 and color2
    finds closest color in current palette
*/

/*
    Create_Remap_Palette()
        Create_Remap_Palette_0()
        Create_Remap_Palette_()
            Find_Closest_Color()
        Create_Remap_Palette_1()
*/
void Create_Remap_Palette(int16_t block, uint8_t red, uint8_t green, uint8_t blue, uint8_t percent)
{
    uint8_t vpercent;

    vpercent = (100 - percent);

    if(vpercent <= 0)               /* (percent >= 100) */
    {
        Create_Remap_Palette_1(block, red, green, blue);
    }
    else if(vpercent >= 100)         /* (percent <= 0) */
    {
        Create_Remap_Palette_0(block);
    }
    else                            /* (percent > 0) && (percent < 100) */
    {
        Create_Remap_Palette_(block, red, green, blue, percent);
    }

//     if(vpercent > 0)                    /* percent < 100 */
//     {
//         if(vpercent < 100)              /* (percent < 100) && (percent > 0) */
//         {
//             Create_Remap_Palette_(block, red, green, blue, percent);
//         }
//         else                            /* (percent < 100) && (percent < 0) */
//         {
//             Create_Remap_Palette_0(block);
//         }
//     }
//     else                                /* (percent >= 100) */
//     {
//         Create_Remap_Palette_1(block, red, green, blue);
//     }

}

void Create_Remap_Palette_0(int16_t block)
{
    uint8_t * remap_palette;
    uint8_t colormap_idx;
    uint16_t counter;

    remap_palette = (uint8_t *)(remap_color_palettes + (block * 256));

    colormap_idx = 0;
    counter = 256;
    while(counter--)
    {
        *remap_palette++ = colormap_idx++;
    }

}

/*
    takes red, green, blue from the glass_colors{} parameters
    does not use the percent
    sets the entire remap palette to the closest color in the current palette
*/
void Create_Remap_Palette_1(int16_t block, uint8_t red, uint8_t green, uint8_t blue)
{
    uint8_t * remap_palette;
    uint8_t closest;

    remap_palette   = (uint8_t *)(remap_color_palettes + (block * 256));
    closest = Find_Closest_Color(red, green, blue);
    memset(remap_palette, closest, 256);  // ~== `REP STOSB`

}

/*
    builds a color-map index table of indicies that remaps existing color to other existing colors that match based on the parameters

    blend/mix two colors
        ~ alpha-blending
    dst color = src1 color * (100 - percent) + src2 color * percent
    color2 passed in
    color1 from current palette

    e.g., {63,63,63,25} ... add 25% of full saturation, for each of R, G, B

    NOTE: The Assembly code has a lot of instances of only using the hi- or lo-byte.

*/
void Create_Remap_Palette_(int16_t block, uint8_t red, uint8_t green, uint8_t blue, uint8_t percent)
{
    int16_t itr;
    uint8_t palette_change_flag;
    uint8_t * remap_palette;
    uint8_t * tmpcurrpal;
    uint16_t ofst;
    uint8_t vpercent;
    uint16_t color2_multiplier;
    uint16_t color1_multiplier;
    uint8_t color2_red;
    uint8_t color2_grn;
    uint8_t color2_blu;
    uint8_t color1_red;
    uint8_t color1_grn;
    uint8_t color1_blu;
    uint8_t color2_red_portion;
    uint8_t color2_grn_portion;
    uint8_t color2_blu_portion;
    uint8_t color1_red_portion;
    uint8_t color1_grn_portion;
    uint8_t color1_blu_portion;
    uint8_t color3_red;
    uint8_t color3_grn;
    uint8_t color3_blu;
    uint8_t closest;
    
    color2_red = red;
    color2_grn = green;
    color2_blu = blue;

    vpercent = (100 - percent);

    color2_multiplier = (( percent << 8) / 100) & 0x00FF;  // ¿ Fixed_Point Math ~== << 8 ?
    color1_multiplier = ((vpercent << 8) / 100) & 0x00FF;  // ¿ Fixed_Point Math ~== << 8 ?

    // MoO2: tint_red = glass_colors[].red * glass_colors[].percent
    color2_red_portion = ((color2_red * color2_multiplier) >> 8);
    color2_grn_portion = ((color2_grn * color2_multiplier) >> 8);
    color2_blu_portion = ((color2_blu * color2_multiplier) >> 8);

    remap_palette   = (uint8_t *)(remap_color_palettes + (block * 256));
    tmpcurrpal = (uint8_t *)(current_palette);

    // iterate through the current build, make the new mixed color, and find the closest existing color
    for(itr = 0; itr < 256; itr++)
    {
        palette_change_flag = PALETTE_FLAG(itr);
        if(palette_change_flag != ST_FALSE)
        {
            ofst = itr * 3;

            color1_red = *(tmpcurrpal + ofst++);
            color1_grn = *(tmpcurrpal + ofst++);
            color1_blu = *(tmpcurrpal + ofst++);

            color1_red_portion = ((color1_red * color1_multiplier) >> 8);
            color1_grn_portion = ((color1_grn * color1_multiplier) >> 8);
            color1_blu_portion = ((color1_blu * color1_multiplier) >> 8);

            color3_red = color1_red_portion + color2_red_portion;
            color3_grn = color1_grn_portion + color2_grn_portion;
            color3_blu = color1_blu_portion + color2_blu_portion;

            closest = 0;
            closest = Find_Closest_Color(color3_red, color3_grn, color3_blu);
        }

        *(remap_palette + itr) = closest;
    }

}

/*
    Animator
        "match-color red", "...green", "...blue"
        "get [red,green,blue] from current table slot"

*/
uint8_t Find_Closest_Color(uint8_t red, uint8_t green, uint8_t blue)
{
    uint8_t * tmpcurrpal = 0;
    uint8_t * remap_palette = 0;
    uint16_t ofst = 0;
    // uint8_t colormap_idx;
    int16_t colormap_idx = 0;
    uint8_t palette_red = 0;
    uint8_t palette_green = 0;
    uint8_t palette_blue = 0;
    int16_t dif_red = 0;
    int16_t dif_green = 0;
    int16_t dif_blue = 0;
    // Meh.  uint16_t counter;
    int16_t current_dif = 0;
    int16_t closest_dif = 0;
    uint8_t closest = 0;

    uint8_t found_color = 0;

    found_color = 0;

    tmpcurrpal = (uint8_t *)(current_palette);

    closest = 0;
    closest_dif = 10000;
    // Meh.  counter = 256;
    // Meh.  ofst = 0;
    // colormap_idx = 0;
    // Meh.  while(counter--)
    for(colormap_idx = 0; colormap_idx < 256; colormap_idx++)
    {
        ofst = colormap_idx * 3;

        // diff color = current color - glass color
        palette_red = *(tmpcurrpal + ofst++);
        dif_red = AbsVal(palette_red - red);

        ofst += 2;
        if(dif_red >= REMAP_THRESHOLD) { continue; }
        ofst -= 2;

        palette_green = *(tmpcurrpal + ofst++);
        dif_green = AbsVal(palette_green - green);

        ofst += 1;
        if(dif_green >= REMAP_THRESHOLD) { continue; }
        ofst -= 1;

        palette_blue = *(tmpcurrpal + ofst++);
        dif_blue = AbsVal(palette_blue - blue);

        if(dif_blue >= REMAP_THRESHOLD) { continue; }

        // current_dif = dif_red * dif_red;
        // current_dif += dif_green * dif_green;
        // current_dif += dif_blue * dif_blue;
        current_dif  = dif_blue * dif_blue;
        current_dif += dif_red * dif_red;
        current_dif += dif_green * dif_green;

        if(current_dif < closest_dif)
        {
            closest_dif = current_dif;
            closest = colormap_idx;
        }
        // Meh. colormap_idx++;
        // assert(ofst == ((colormap_idx * 3) + 3));
    }

    found_color = closest;
    
    return found_color;
}


// WZD s21p07
// FLIC_Load_Palette()
