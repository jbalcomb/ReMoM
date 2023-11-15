/*
    WIZARDS.EXE
        seg017
        seg018
        seg019
*/
#include "MoX.H"



// WZD dseg:783C
uint16_t outline_color = 0;
// WZD dseg:783E
int16_t draw_alias_flag;


/*
    BEGIN:  Fonts
*/
// WZD  dseg:E7DE 00 00                                           gsa_VGAFILEH_Header dw 0 
// WZD  dseg:E7E0 00 00                                           p_PaletteFlags dw 0      

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
// WZD dseg:E7FE0                               ; 15Ch paragraphs
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
int16_t Font_ColorIndex3;
// WZD dseg:E816
int16_t Font_ColorIndex2;
// WZD dseg:E818
int16_t Font_ColorIndex1;
// WZD dseg:E81A
int16_t Font_Index;

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


byte_ptr p_Palette;                             // MGC dseg:A7DE    alloc in Load_Font_File()
byte_ptr current_palette;

byte_ptr p_PaletteFlags;                        // MGC dseg:A7D6    alloc in Load_Font_File()
// DELETE  byte_ptr p_Palette_XBGR;                        // STU/Win32
byte_ptr palette_flags;


void Load_Font_File(char * font_file)
{
    int itr;

    strcpy(font_name, font_file);

    font_style_data = LBX_Load(font_file, 0);
    border_style_data = LBX_Load(font_file, 1);

    font_header = (struct s_FONT_HEADER *)font_style_data;

    palette_block          = Allocate_Space(348);    // 348 paragraphs = 386 * 16 bytes = 5568 bytes

    // MoO2  current_palette
    p_Palette              = Allocate_Space(64);     //  64 paragraphs =  64 * 16 bytes = 1024 bytes
    // 1oom: lbxpal_palette = p_Palette;
    current_palette = p_Palette;

    p_PaletteFlags         = p_Palette + (48 * 16);  // ~== p_PaletteFlags = &p_Palette[768];
// DELETE      p_Palette_XBGR         = Allocate_Space(64);     // STU/Win32  4 bytes per pixel * 256 colors / 16 bytes per paragraph
    palette_flags = p_PaletteFlags;

    // TODO  UU_DAC_Save_Seg = Allocate_Space(48);

    // Replacement_Colors = Allocate_Space(384);  // 384 paragraphs = 384 * 16 = 6,144 bytes  (24 * 256  ~'remap color tables')
    remap_color_palettes = (uint8_t *)Allocate_Space(384);

    // TODO  gsa_VGAFILEH_Header = Allocate_Space(2);

    Intensity_Scale_Tbl = Allocate_Space(96);  // 96 paragraphs = 96 * 16 = 1,536 bytes  

    // TODO  VGA_TextDraw_Init();
 



    for(itr = 0; itr < 768; itr++)
    {
        *(p_Palette + itr) = 0;
    }
    for(itr = 0; itr < 256; itr++)
    {
        *(p_PaletteFlags + itr) = 1;
    }
// DELETE       for(itr = 0; itr < 1024; itr++)
// DELETE       {
// DELETE           *(p_Palette_XBGR + itr) = 0;
// DELETE       }

}


/*
    WIZARDS.EXE  seg017
*/

// WZD s17p01
// MoO2: Set_Font_Style_Shadow_Down()
// ¿ Color1 == 15 ~== MoO2 Set_Remap_Font_Style_Shadow_Down() ?
void Set_Font_Style1(int16_t Font_Index, int16_t Color_1, int16_t Color_2, int16_t Color_3)
{
    Set_Font(Font_Index, Color_1, Color_2, Color_3);
    font_header->shadow_flag = 1;  // bottom right
}
// WZD s17p02
void Set_Font_Style2(int16_t Font_Index, int16_t Color_1, int16_t Color_2, int16_t Color_3)
{
    Set_Font(Font_Index, Color_1, Color_2, Color_3);
    font_header->shadow_flag = 2;  // top left
}
// WZD s17p03
void Set_Font_Style3(int16_t Font_Index, int16_t Color_1, int16_t Color_2, int16_t Color_3)
{
    Set_Font(Font_Index, Color_1, Color_2, Color_3);
    font_header->shadow_flag = 3;  // 2x bottom right
}
// WZD s17p04
// AKA Set_Font_Style4()
// MoO2:  Set_Font_Style_Outline(); Set_Remap_Font_Style_Outline();
// MoO2 font_header.shadow_flag, 4
void Set_Font_Style_Outline(int16_t style_num, int16_t Color_1, int16_t Color_2, int16_t Color_3)
{
    Set_Font(style_num, Color_1, Color_2, Color_3);
    font_header->shadow_flag = 4;  // enum e_Font_Shadow { e_Font_Shadow_Outline }
}
// WZD s17p05
// MoO2: Set_Font_Style_Outline_Heavy(); Set_Remap_Font_Style_Outline_Heavy();
void Set_Font_Style5(int16_t Font_Index, int16_t Color_1, int16_t Color_2, int16_t Color_3)
{
    Set_Font(Font_Index, Color_1, Color_2, Color_3);
    font_header->shadow_flag = 5;  // full + bottom right
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
*/
void Set_Font_Colors_15(int16_t font_idx, uint8_t * colors)
{
    int16_t itr;
    int16_t color_start;

    color_start = 240;  // ? 15 * 16 ?

    for(itr = 0; itr < 16; itr++)
    {
        // farpokeb(font_colors, color_start + itr, colors[itr]);
        font_colors[color_start + itr] = colors[itr];
    }

    // sets current_colors and normal_colors to font color block 15
    Set_Font(font_idx, 15, 0, 0);

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
// drake178: ?
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

    GET_1B_OFS(font_style_data, FONT_HDR_POS_CURRENT_COLORS) = VGA_Current_AA_Color;
    GET_1B_OFS(font_style_data, FONT_HDR_POS_NORMAL_COLORS) = VGA_AA_Color_1;
    GET_1B_OFS(font_style_data, FONT_HDR_POS_HIGHLIGHT_COLORS) = VGA_AA_Color_2;
    GET_1B_OFS(font_style_data, FONT_HDR_POS_SPECIAL_COLORS) = VGA_AA_Color_3;

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


// WZD s17p23
// drake178: VGA_DrawRightAligned()
int16_t Print_Right(int16_t x, int16_t y, char * string)
{
    int16_t next_x;
    int16_t string_len;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] Print_Right(): string: %s\n", __FILE__, __LINE__, string);
#endif

    string_len = Get_String_Width(string) - 1;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] Print_Right(): string_len: %d\n", __FILE__, __LINE__, string_len);
#endif

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
    char buffer[10];

#pragma warning(suppress : 4996)
    itoa(val, buffer, 10);

    next_x = Print_Right(x, y, buffer);

    return next_x;
}


// WZD s17p26
int16_t Print_Integer_Centered(int16_t x, int16_t y, int16_t val)
{
    int16_t next_x;
    char buffer[10];

#pragma warning(suppress : 4996)
    itoa(val, buffer, 10);

    next_x = Print_Centered(x, y, buffer);

    return next_x;
}



// WZD s17p26
// WZD s17p27
// WZD s17p28
// WZD s17p29
// WZD s17p30
// WZD s17p31
// WZD s17p32
// WZD s17p33



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
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: Print() string: %s\n", __FILE__, __LINE__, string);
#endif

    next_x = Print_Display(x, y, string, ST_FALSE);

    return next_x;
}


// WZD s17p36
// drake178: VGA_DrawStyledString()
int16_t Print_Display(int16_t x, int16_t y, char * string, int16_t full_flag)
{
    int16_t itr;
    int16_t next_x;
    uint16_t outline_style;

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: BEGIN: Print_Display(x = %d, y = %d, string = %s, full_flag = %d)\n", __FILE__, __LINE__, x, y, string, full_flag);
// #endif

    // // outline_style = farpeekb(font_style_data, FONT_HEADER_SHADOW_FLAG);
    // outline_style = 0;  // ~None
    outline_style = GET_1B_OFS(font_style_data, FONT_HDR_POS_SHADOW_FLAG);
// DELETE  #ifdef STU_DEBUG
// DELETE      dbg_prn("DEBUG: [%s, %d]: outline_style: %d\n", __FILE__, __LINE__, outline_style);
// DELETE  #endif

    if(outline_style != 0)  /* ¿ ST_NONE ? */
    {
        // DLOG("(outline_style != 0)");
        for(itr = 0; itr < 16; itr++)
        {
            // farpokeb(font_style_data, itr, outline_color);
            SET_1B_OFS(font_style_data, FONT_HDR_POS_CURRENT_COLORS + itr, outline_color);
        }
        draw_alias_flag = ST_TRUE;
        
        if(outline_style != 2) /* Shadow_TopLeft */
        {
            // DLOG("(outline_style != 2)");
            Print_String(x + 1, y + 1, string, ST_FALSE, full_flag);  // overdraw right + botton
            Print_String(x    , y + 1, string, ST_FALSE, full_flag);  // overdraw bottom
            Print_String(x + 1, y    , string, ST_FALSE, full_flag);  // overdraw right
        }
        if( outline_style != 1 && outline_style != 3)  /* Shadow_BtmRight || Shadow_BtmRight_2px */
        {
            // DLOG("( outline_style != 1 && outline_style != 3)");
            Print_String(x - 1, y    , string, ST_FALSE, full_flag);  // overdraw left
            Print_String(x - 1, y - 1, string, ST_FALSE, full_flag);  // overdraw left + top
            Print_String(x    , y - 1, string, ST_FALSE, full_flag);  // overdraw top
        }
        if(outline_style == 3 || outline_style == 5)  /* Shadow_BtmRight_2px || Outline_Plus_BR2px */
        {
            // DLOG("(outline_style == 3 || outline_style == 5)");
            Print_String(x + 2, y + 2, string, ST_FALSE, full_flag);
            Print_String(x + 1, y + 2, string, ST_FALSE, full_flag);
            Print_String(x + 2, y + 1, string, ST_FALSE, full_flag);
        }
        if(outline_style > 3)  /* Shadow_BtmRight_2px */
        {
            // DLOG("(outline_style > 3)");
            Print_String(x + 1, y - 1, string, ST_FALSE, full_flag);  // overdraw right + top
            Print_String(x - 1, y + 1, string, ST_FALSE, full_flag);  // overdraw left + bottom
        }
        if(outline_style == 5)  /* Outline_Plus_BR2px */
        {
            // DLOG("(outline_style == 5)");
            Print_String(x + 2, y    , string, ST_FALSE, full_flag);
            Print_String(x    , y + 2, string, ST_FALSE, full_flag);
        }

        // ; selects one of the 3 stored font color sets, and
        // VGA_FontColorSelect(farpeekb(font_style_data,0x13));
        // MoO2: copies normal_colors into current_colors - compiler in-lined?
        Set_Color_Set(GET_1B_OFS(font_style_data,FONT_HDR_POS_CURRENT_COLOR_SET));
    }

        draw_alias_flag = ST_FALSE;

        next_x = Print_String(x, y, string, ST_TRUE, full_flag);

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: END: Print_Display(x = %d, y = %d, string = %s, full_flag = %d)\n", __FILE__, __LINE__, x, y, string, full_flag);
// #endif

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
    int16_t next_x = 0;
    // char character_;
    int16_t str_idx;
    int16_t space_add;
    int16_t space_remainder;
    int16_t current_space;
    int16_t space_count;
    char character;
    uint16_t ptr;

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: BEGIN: Print_String(x = %d, y = %d, string = %s, change_color_ok_flag = %d, full_flag = %d)\n", __FILE__, __LINE__, x, y, string, change_color_ok_flag, full_flag);
// #endif

    ptr = 0;

    print_xpos = x;
    print_ypos = y;

    if(full_flag != ST_FALSE)
    {
        // DLOG("(full_flag != ST_FALSE)");
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



    // while(character = string[ptr++] != '\0')
    while(string[ptr] != '\0')
    {
        character = string[ptr];

// sw_character_values[9] = {1, 2, 3, 4, 13, 20, 21, 25, 29}
// seg017:0AD1
// 01 00 02 00 03 00 04 00 0D 00 14 00 15 00 19 00 1D 00
// seg017:0AE3
// switch_character
// offset character_01
// offset character_02
// offset character_03
// offset character_04
// offset character_13_20
// offset character_13_20
// offset character_21
// offset character_25_29
// offset character_25_29

        switch(character)
        {
            case 1:  /* character == '\x01' */  /* ASCII   1h  1d  SOH (start of heading)  */  /* sw_character_values[0] == character */
            {
                // DLOG("case 1:");
                if(change_color_ok_flag != ST_FALSE)
                {
                    // DLOG("case 1: && change_color_ok_flag != ST_FALSE)");
                    Set_Normal_Colors_On();
                }
            } break;
            case 2:
            {
                // DLOG("case 2:");
                if(change_color_ok_flag != ST_FALSE)
                {
                    // DLOG("case 2: && change_color_ok_flag != ST_FALSE)");
                    Set_Highlight_Colors_On();
                }
            } break;
            case 3:
            {
                // DLOG("case 3:");
                if(change_color_ok_flag != ST_FALSE)
                {
                    // DLOG("case 3: && change_color_ok_flag != ST_FALSE)");
                    Set_Special_Colors_On();
                }
            } break;
            case 4:
            {
                // DLOG("case 4:");
                if(change_color_ok_flag != ST_FALSE)
                {
                    // DLOG("case 4: && change_color_ok_flag != ST_FALSE)");
                    Set_Highlight_Colors_On();
                }
            } break;
            case 13:  /*  ASCII  0Dh  13d  CR (carriage return)        */
                // DLOG("case 13:");
            case 20:  /*  ASCII  14h  20d  DC4 (device control 4)      */
            {
                // DLOG("case 20:");
                print_ypos += GET_2B_OFS(font_style_data,FONT_HDR_POS_CURRENT_BASE_HEIGHT);
                print_xpos += x;
            } break;
            case 21:  /*  ASCII  15h  21d  NAK (negative acknowledge)  */
            {
                // DLOG("case 21:");
                goto Done;
            } break;
            case 25:  /*  ASCII  19h  25d  EM (end of medium)          */
                // DLOG("case 25:");
            case 29:  /*  ASCII  1Dh  29d  GS (group separator)        */
            {
                // DLOG("case 29:");
                print_xpos = x + string[ptr+1];
                ptr++;
            } break;

        }


        if(draw_alias_flag != ST_FALSE)
        {
            // DLOG("(draw_alias_flag != ST_FALSE)");
            print_xpos = Print_Character_No_Alias(print_xpos, print_ypos, character);
        }
        else
        {
            // DLOG("(draw_alias_flag == ST_FALSE)");
            print_xpos = Print_Character(print_xpos, print_ypos, character);
        }

        if(full_flag != ST_FALSE)
        {
            // DLOG("(full_flag != ST_FALSE)");
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


    next_x = print_xpos;

Done:

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: END: Print_String(x = %d, y = %d, string = %s, change_color_ok_flag = %d, full_flag = %d)\n", __FILE__, __LINE__, x, y, string, change_color_ok_flag, full_flag);
// #endif

    return next_x;
}


// WZD s17p38
// drake178: UU_VGA_DisableAAPixels


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
}

// WZD s17p41
// drake178: VGA_FontColor3Toggle
// MoO2: Set_Special_Colors_On
// MoO2: copies special_colors into current_colors - 8 words
void Set_Special_Colors_On(void)
{
    uint8_t current_color_set;
    current_color_set = GET_1B_OFS(font_style_data,FONT_HDR_POS_CURRENT_COLOR_SET);
    if(current_color_set != 2)
    {
        Set_Color_Set(2);
    }
    else
    {
        Set_Color_Set(0);
    }
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


// WZD s17p53
// MoO2  Get_Current_Font_Style() ...happenstance, has the same odd extra copy to 'return vlaue' ? dereferencing a pointer ? exported by ASM ?
int16_t Get_Current_Font_Index(void)
{
    int16_t current_font_index;

    current_font_index = Font_Index;

    return current_font_index;
}


// WZD s17p54
// AKA Get_Font_Color1
int16_t Get_Current_Font_Color(void)
{
    int16_t current_font_color;

    current_font_color = Font_ColorIndex1;

    return current_font_color;
}

// WZD s17p55
// Get_Font_Color2
// WZD s17p56
// Get_Font_Color3



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
int16_t _CS_skip_x;
int16_t _CS_next_x;
int16_t _CS_width;

// WZD s18p01
// MoO2: Set_Font_Style(style_num, colors)
/*
    sets font style
    sets color1 as current & normal colors
    sets color2 as highlight colors
    sets color3 as special colors
*/
void Set_Font(int16_t font_idx, int16_t color1, int16_t color2, int16_t color3)
{
    int16_t itr;

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: BEGIN: Set_Font(font_idx = %d, color1 = %d, color2 = %d, color3 = %d)\n", __FILE__, __LINE__, font_idx, color1, color2, color3);
// #endif

    color1 = (color1 < 16) ? color1 : 0;
    color2 = (color2 < 16) ? color2 : 0;
    color3 = (color3 < 16) ? color3 : 0;

    Font_Index = font_idx;
    Font_ColorIndex1 = color1;
    Font_ColorIndex2 = color2;
    Font_ColorIndex3 = color3;

    for(itr = 0; itr < 16; itr++)
    {
        // current_colors
        font_style_data[(0 + itr)] = font_colors[((color1 * 16) + itr)];  // /*  00 */ uint8_t  current_colors[16];
        // font_style_data[(FONT_HDR_POS_CURRENT_COLORS + itr)] = font_colors[((color1 * 16) + itr)];
    }

    for(itr = 0; itr < 16; itr++)
    {
        // normal_colors
        font_style_data[(20 + itr)] = font_colors[((color1 * 16) + itr)];  // /*  14 */ uint8_t  normal_colors[16];
        // font_style_data[(FONT_HDR_POS_NORMAL_COLORS + itr)] = font_colors[((color1 * 16) + itr)];
    }

    for(itr = 0; itr < 16; itr++)
    {
        // highlight_colors
        font_style_data[(FONT_HDR_POS_HIGHLIGHT_COLORS + itr)] = font_colors[((color2 * 16) + itr)];
    }

    for(itr = 0; itr < 16; itr++)
    {
        // special_colors
        font_style_data[(FONT_HDR_POS_SPECIAL_COLORS + itr)] = font_colors[((color3 * 16) + itr)];
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

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: END: Set_Font(font_idx = %d, color1 = %d, color2 = %d, color3 = %d)\n", __FILE__, __LINE__, font_idx, color1, color2, color3);
// #endif

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

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: BEGIN: Print_Character(x = %d, y = %d, char_num = %d)\n", __FILE__, __LINE__, x, y, char_num);
// #endif

// DELETE  #ifdef STU_DEBUG
// DELETE      // if(x == 271 && y == 101 && char_num == 50)
// DELETE      if(
// DELETE          (x == 271 && y == 101 && char_num == 50) || 
// DELETE          (x == 271 && y == 133 && char_num == 53)
// DELETE      )
// DELETE      {
// DELETE          DLOG("DBG_Print_Character");
// DELETE          DLOG("DBG_Print_Character_ASM");
// DELETE          DBG_Print_Character = 1;
// DELETE          DBG_Print_Character_ASM = 1;
// DELETE      }
// DELETE  #endif

    _CS_skip_x = x;

    if(char_num < 32 || char_num > 126)
    {
        goto Done_YaySkip;
    }
    else
    {
        char_num = char_num - 32;

        _CS_next_x = _CS_skip_x;
        _CS_next_x += font_header->current_horizontal_spacing;
        _CS_next_x += font_header->current_font_widths[char_num];

        width = font_header->current_font_widths[char_num];
        font_data_offset = (font_style_data + font_header->current_data_offsets[char_num]);

        Print_Character_ASM(x, y, width, font_data_offset);

        goto Done_NaySkip;
    }

    goto Done;

Done_YaySkip:
    next_x = _CS_skip_x;
    goto Done;
Done_NaySkip:
    next_x = _CS_next_x;
    goto Done;
Done:
// DELETE  #ifdef STU_DEBUG
// DELETE      // if(x == 271 && y == 101 && char_num == 50)
// DELETE      if(
// DELETE          (x == 271 && y == 101 && char_num == 50) || 
// DELETE          (x == 271 && y == 133 && char_num == 53)
// DELETE      )
// DELETE      {
// DELETE          DBG_Print_Character = 0;
// DELETE          DBG_Print_Character_ASM = 0;
// DELETE      }
// DELETE  #endif

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: END: Print_Character(x = %d, y = %d, char_num = %d)\n", __FILE__, __LINE__, x, y, char_num);
// #endif

    return next_x;
}


// WZD s18p03
int16_t Print_Character_No_Alias(int16_t x, int16_t y, int16_t char_num)
{
    int16_t next_x;
    int16_t width;
    byte_ptr font_data_offset;

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: BEGIN: Print_Character_No_Alias(x = %d, y = %d, char_num = %d)\n", __FILE__, __LINE__, x, y, char_num);
// #endif

// DELETE  #ifdef STU_DEBUG
// DELETE      if(x == 272 && y == 100 && char_num == 50)
// DELETE      {
// DELETE          DLOG("DBG_Print_Character_No_Alias");
// DELETE          DBG_Print_Character_No_Alias = 1;
// DELETE          DBG_Print_Character_No_Alias_ASM = 1;
// DELETE      }
// DELETE  #endif

    _CS_skip_x = x;

    if(char_num < 32 || char_num > 126)
    {
        goto Done_YaySkip;
    }
    else
    {
        char_num = char_num - 32;

        _CS_next_x = _CS_skip_x;
        _CS_next_x += font_header->current_horizontal_spacing;
        _CS_next_x += font_header->current_font_widths[char_num];

        width = font_header->current_font_widths[char_num];
        font_data_offset = (font_style_data + font_header->current_data_offsets[char_num]);

        Print_Character_No_Alias_ASM(x, y, width, font_data_offset);

        goto Done_NaySkip;
    }

    goto Done;

Done_YaySkip:
    next_x = _CS_skip_x;
    goto Done;
Done_NaySkip:
    next_x = _CS_next_x;
    goto Done;
Done:
// DELETE  #ifdef STU_DEBUG
// DELETE      if(x == 272 && y == 100 && char_num == 50)
// DELETE      {
// DELETE          DBG_Print_Character_No_Alias = 0;
// DELETE          DBG_Print_Character_No_Alias_ASM = 0;
// DELETE      }
// DELETE  #endif

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: END: Print_Character_No_Alias(x = %d, y = %d, char_num = %d)\n", __FILE__, __LINE__, x, y, char_num);
// #endif
    return next_x;
}

// WZD s18p04
// LBX_DrawGlyph ? Print_Character_To_Bitmap() ?

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

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: BEGIN: Print_Character_ASM(x_start = %d, y_start = %d, width = %d, font_data_offset = %p)\n", __FILE__, __LINE__, x_start, y_start, width, font_data_offset);
// #endif

// DELETE  #ifdef STU_DEBUG
// DELETE      for(color_index = 0; color_index < 16; color_index++)
// DELETE      {
// DELETE          palette_index = GET_1B_OFS(font_style_data,FONT_HDR_POS_CURRENT_COLORS + color_index);
// DELETE          dbg_prn("DEBUG: [%s, %d]: current_colors[%d]: 0x%02X\n", __FILE__, __LINE__, color_index, palette_index);
// DELETE      }
// DELETE  #endif
    screen_start = current_video_page + ((y_start * SCREEN_WIDTH) + x_start);

    screen_pos = screen_start;
    while(width)
    {

        font_data_byte = *font_data_offset++;
// DELETE  #ifdef STU_DEBUG
// DELETE      if(DBG_Print_Character_ASM == 1)
// DELETE      {
// DELETE          dbg_prn("DEBUG: [%s, %d]: font_data_byte: 0x%02X\n", __FILE__, __LINE__, font_data_byte);
// DELETE      }
// DELETE  #endif

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
// DELETE  #ifdef STU_DEBUG
// DELETE      if(DBG_Print_Character_ASM == 1)
// DELETE      {
// DELETE          dbg_prn("DEBUG: [%s, %d]: skip_count: %d\n", __FILE__, __LINE__, skip_count);
// DELETE      }
// DELETE  #endif

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
// DELETE  #ifdef STU_DEBUG
// DELETE      if(DBG_Print_Character_ASM == 1)
// DELETE      {
// DELETE          dbg_prn("DEBUG: [%s, %d]: color_index: 0x%02X\n", __FILE__, __LINE__, color_index);
// DELETE      }
// DELETE  #endif

                // *screen_pos = font_header->current_colors[color_index];
                // FTW palette_index = font_header->current_colors[color_index];
                palette_index = GET_1B_OFS(font_style_data,FONT_HDR_POS_CURRENT_COLORS + color_index);
// DELETE  #ifdef STU_DEBUG
// DELETE      if(DBG_Print_Character_ASM == 1)
// DELETE      {
// DELETE          dbg_prn("DEBUG: [%s, %d]: palette_index: 0x%02X\n", __FILE__, __LINE__, palette_index);
// DELETE      }
// DELETE  #endif
                *screen_pos = palette_index;

                screen_pos += SCREEN_WIDTH;
            }
        }
    }

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: END: Print_Character_ASM(x_start = %d, y_start = %d, width = %d, font_data_offset = %p)\n", __FILE__, __LINE__, x_start, y_start, width, font_data_offset);
// #endif

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

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: BEGIN: Print_Character_No_Alias_ASM(x_start = %d, y_start = %d, width = %d, font_data_offset = %p)\n", __FILE__, __LINE__, x_start, y_start, width, font_data_offset);
// #endif

// DELETE  #ifdef STU_DEBUG
// DELETE      if(DBG_Print_Character_No_Alias == 1)
// DELETE      {
// DELETE  
// DELETE      }
// DELETE  #endif

    screen_start = current_video_page + ((y_start * SCREEN_WIDTH) + x_start);

    screen_pos = screen_start;
    while(width)
    {
        font_data_byte = *font_data_offset++;
// DELETE  #ifdef STU_DEBUG
// DELETE      if(DBG_Print_Character_No_Alias == 1)
// DELETE      {
// DELETE          dbg_prn("DEBUG: [%s, %d]: font_data_byte: 0x%02X\n", __FILE__, __LINE__, font_data_byte);
// DELETE      }
// DELETE  #endif

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
// DELETE  #ifdef STU_DEBUG
// DELETE      if(DBG_Print_Character_No_Alias == 1)
// DELETE      {
// DELETE          dbg_prn("DEBUG: [%s, %d]: skip_count: %d\n", __FILE__, __LINE__, skip_count);
// DELETE      }
// DELETE  #endif
            while(skip_count--)
            {
                screen_pos += SCREEN_WIDTH;
            }
        }

        // @@Nay_Negative
        if((font_data_byte & 0x80) == 0)
        {
            repeat_count = ((font_data_byte & 0xF0) >> 4);
// DELETE  #ifdef STU_DEBUG
// DELETE      if(DBG_Print_Character_No_Alias == 1)
// DELETE      {
// DELETE          dbg_prn("DEBUG: [%s, %d]: repeat_count: %d\n", __FILE__, __LINE__, repeat_count);
// DELETE      }
// DELETE  #endif
            if((font_data_byte & 0x0F) == 0)
            {
// DELETE  #ifdef STU_DEBUG
// DELETE      if(DBG_Print_Character_No_Alias == 1)
// DELETE      {
// DELETE          DLOG("(font_data_byte == 0)");
// DELETE      }
// DELETE  #endif
                screen_pos += SCREEN_WIDTH;
            }
            else
            {
// DELETE  #ifdef STU_DEBUG
// DELETE      if(DBG_Print_Character_No_Alias == 1)
// DELETE      {
// DELETE          DLOG("(font_data_byte != 0)");
// DELETE      }
// DELETE  #endif
                while(repeat_count--)
                {
                    color_index = (font_data_byte & 0x0F);
// DELETE  #ifdef STU_DEBUG
// DELETE      if(DBG_Print_Character_No_Alias == 1)
// DELETE      {
// DELETE          dbg_prn("DEBUG: [%s, %d]: color_index: 0x%02X\n", __FILE__, __LINE__, color_index);
// DELETE      }
// DELETE  #endif
                    // *screen_pos = font_header->current_colors[color_index];
                    palette_index = font_header->current_colors[color_index];
// DELETE  #ifdef STU_DEBUG
// DELETE      if(DBG_Print_Character_No_Alias == 1)
// DELETE      {
// DELETE          dbg_prn("DEBUG: [%s, %d]: palette_index: 0x%02X\n", __FILE__, __LINE__, palette_index);
// DELETE      }
// DELETE  #endif
                    *screen_pos = palette_index;
                    screen_pos += SCREEN_WIDTH;
                }
            }

        }
    }

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: END: Print_Character_No_Alias_ASM(x_start = %d, y_start = %d, width = %d, font_data_offset = %p)\n", __FILE__, __LINE__, x_start, y_start, width, font_data_offset);
// #endif

}



// WZD s18p07
int16_t Get_String_Width(char * string)
{
    int16_t pos;
    int16_t width;
    int16_t char_num;
    int16_t horizontal_spacing;

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: BEGIN: Get_String_Width(string = %s)\n", __FILE__, __LINE__, string);
// #endif

    pos = 0;
    width = 0;

    // MoM - ASM: horizontal_spacing
    // MoO2: horizontal_spacing = font_header.current_horizontal_spacing;
    // horizontal_spacing = font_style_data[72];
    horizontal_spacing = GET_2B_OFS(font_style_data,FONT_HDR_POS_CURRENT_HORIZONTAL_SPACING);
    
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: horizontal_spacing: %d\n", __FILE__, __LINE__, horizontal_spacing);
// #endif


Next_Char:
    char_num = string[pos++];  // ~== LODSB
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: char_num: %d\n", __FILE__, __LINE__, char_num);
//     dbg_prn("DEBUG: [%s, %d]: char: %c\n", __FILE__, __LINE__, char_num);
// #endif
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

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: GET_1B_OFS(font_style_data,FONT_HDR_POS_CURRENT_FONT_WIDTHS + %d): %d\n", __FILE__, __LINE__, char_num, GET_1B_OFS(font_style_data,FONT_HDR_POS_CURRENT_FONT_WIDTHS + char_num));
//     dbg_prn("DEBUG: [%s, %d]: font_header->current_font_widths[%d]: %d\n", __FILE__, __LINE__, char_num, font_header->current_font_widths[char_num]);
// #endif

    // width += GET_1B_OFS(font_style_data,FONT_HDR_POS_CURRENT_FONT_WIDTHS + char_num);
    width += font_header->current_font_widths[char_num];
    width += horizontal_spacing;  // MoO2:    ax, font_header.current_horizontal_spacing

goto Next_Char;


Done:
    width = width - horizontal_spacing;  // remove the extra one that was added to the last character

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: END: Get_String_Width(string = %s) { width = %d }\n", __FILE__, __LINE__, string, width);
// #endif

    return width;
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
void Load_Palette(int entry, int start_color, int end_color)
{
    int color_start;
    int color_count;
    int itr;
#ifdef STU_DEBUG
    int16_t palette_idx;
    uint8_t font_color_block;
    uint8_t color_index;
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Load_Palette(entry = %d, start_color = %d, end_color = %d)\n", __FILE__, __LINE__, entry, start_color, end_color);
#endif

    palette_data = LBX_Reload(font_name, entry+2, palette_block);
    // 1oom: lbxpal_palette_inlbx = palette_data;
#ifdef STU_DEBUG
//     for(palette_idx = 0; palette_idx < 256; palette_idx++)
//     {
//         dbg_prn("[%02X] %02X %02X %02X\n", palette_idx, *(palette_data + (palette_idx * 3) + 0), *(palette_data + (palette_idx * 3) + 1), *(palette_data + (palette_idx * 3) + 2) );
//     }
#endif
#ifdef STU_DEBUG
//     for(palette_idx = 0; palette_idx < 256; palette_idx++)
//     {
//         dbg_prn("[%d] %d %d %d\n", palette_idx, *(palette_data + (palette_idx * 3) + 0), *(palette_data + (palette_idx * 3) + 1), *(palette_data + (palette_idx * 3) + 2) );
//     }
#endif

    // font_colors = (palette_data + (16 * (48)));  // 768
    font_colors = &palette_data[768];
// DELETE  #ifdef STU_DEBUG
// DELETE      for(font_color_block = 0; font_color_block < 16; font_color_block++)
// DELETE      {
// DELETE          for(color_index = 0; color_index < 16; color_index++)
// DELETE          {
// DELETE              dbg_prn("DEBUG: [%s, %d]: font_colors[%d][%d]: 0x%02X\n", __FILE__, __LINE__, font_color_block, color_index, font_colors[(font_color_block * 16) + color_index]);
// DELETE          }
// DELETE  
// DELETE      }
// DELETE  #endif

    // UU_gsa_Palette_Data = (palette_data + (16 * (48 + 16)));  // 400h

    mouse_palette = (palette_data + (16 * (48 + 16 + 16)));  // 1280

    // array of 24 color fractions (B-G-R-Percent)
    // used to set shaded replacement colors for each color index
    // gsa_ShadingColors = (palette_data + (16 * (48 + 16 + 16 + 256)));  // 5376  (24 * 4 = 96) 0x60  6 PR
    remap_colors = (palette_data + (16 * (48 + 16 + 16 + 256)));  // FONTS.LBX, 2; @0x1500  5376
// DELETE  #ifdef STU_DEBUG
// DELETE      for(itr = 0; itr < 24; itr++)
// DELETE      {
// DELETE          dbg_prn("DEBUG: [%s, %d]: remap_colors[%d]: %02X %02X %02X %d\n", __FILE__, __LINE__, itr, remap_colors[itr * 4 + 0], remap_colors[itr * 4 + 1], remap_colors[itr * 4 + 2], remap_colors[itr * 4 + 3]);
// DELETE      }
// DELETE  #endif
// DELETE  #ifdef STU_DEBUG
// DELETE      for(itr = 0; itr < 24; itr++)
// DELETE      {
// DELETE          dbg_prn("DEBUG: [%s, %d]: remap_colors[%d]: %d %d %d %d\n", __FILE__, __LINE__, itr, remap_colors[itr * 4 + 0], remap_colors[itr * 4 + 1], remap_colors[itr * 4 + 2], remap_colors[itr * 4 + 3]);
// DELETE      }
// DELETE  #endif
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

    for(itr = 0; itr < (color_count * 3); itr++)
    {
        // MoO2 current_palette
        *(p_Palette + (color_start * 3) + itr) = *(palette_data + (color_start * 3) + itr);
    }

// DELETE      for(itr = 0; itr < color_count; itr++)
// DELETE      {
// DELETE          *(p_Palette_XBGR + (color_start * 4) + (itr * 4) + 3) = 0x00;
// DELETE          *(p_Palette_XBGR + (color_start * 4) + (itr * 4) + 2) = (*(palette_data + (color_start * 3) + (itr * 3) + 0) << 2);
// DELETE          *(p_Palette_XBGR + (color_start * 4) + (itr * 4) + 1) = (*(palette_data + (color_start * 3) + (itr * 3) + 1) << 2);
// DELETE          *(p_Palette_XBGR + (color_start * 4) + (itr * 4) + 0) = (*(palette_data + (color_start * 3) + (itr * 3) + 2) << 2);
// DELETE      }

    Set_Font(0, 0, 0, 0);

    if(start_color == ST_UNDEFINED)
    {
        Set_Palette_Changes(0, 255);
    }
    else
    {
        Set_Palette_Changes(start_color, end_color);
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Load_Palette(entry = %d, start_color = %d, end_color = %d)\n", __FILE__, __LINE__, entry, start_color, end_color);
#endif

}




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
// MoO2: Calculate_Remap_Gray_Palettes_
    // 1oom: lbxpal_build_colortables();
    // MoO2: Calculate_Picture_Remap_Colors() |-> Create_Picture_Remap_Palette()
// ¿ remaps colors based on the on the data in the screen buffer ? ¿ for animations, pictures, bitmaps, and sprites ?
// drake178: Replacement_Colors@ table by combining
// drake178: Shading_Colors@
// gsa_ShadingColors ~== glass_colors
// Replacement_Colors ~== glass_remap_colors[16][256]
void Calculate_Remap_Colors(void)
{
    int16_t itr_blocks;
    int16_t block;
    uint16_t block_ofst;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t percent;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Calculate_Remap_Colors()\n", __FILE__, __LINE__);
#endif

    // Create_Remap_Palette(0, 0, 0, 0, 50);
    block = 0;
    red = 0;
    green = 0;
    blue = 0;
    percent = 50;
    Create_Remap_Palette(block, red, green, blue, percent);

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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Calculate_Remap_Colors()\n", __FILE__, __LINE__);
#endif
}


// WZD s20p04
// drake178: VGA_SetShades_Range()
// MoO2: ~ Update_Glass_Remap_Color_Range()
// fills out the Replacement_Colors@ table by combining Shading_Colors@ with any changed palette colors,
//   but only for the color sets indicated by the confines table 0 is set to shades of 50% black instead of whatever the palette originally had, if included
void Update_Remap_Color_Range(int16_t first_set, int16_t last_set)
{
    int16_t itr_blocks;
    int16_t block;
    uint16_t block_ofst;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t percent;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Update_Remap_Color_Range(first_set = %d, last_set = %d)\n", __FILE__, __LINE__, first_set, last_set);
#endif

    if(last_set < first_set)
    {
        // TODO  MEM_SwapWord(first_set, last_set);
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Update_Remap_Color_Range(first_set = %d, last_set = %d)\n", __FILE__, __LINE__, first_set, last_set);
#endif
}


// WZD s20p05
void Set_Palette_Changes(int start_color, int end_color)
{
    int itr;

    for(itr = start_color; itr < end_color; itr++)
    {
        *(p_PaletteFlags + itr) = ST_TRUE;
    }

}

// WZD s20p06
void Clear_Palette_Changes(int start_color, int end_color)
{
    int itr;

    for(itr = start_color; itr < end_color; itr++)
    {
        *(p_PaletteFlags + itr) = ST_FALSE;
    }

}



// WZD s20p07
// WZD s20p08
// WZD s20p09
// WZD s20p10

// WZD s20p11
// void Reset_Cycle_Palette_Color(void);

// WZD s20p12
 // Cycle_Palette_Color(int color_num, int red_min, int green_min, int blue_min, int red_max, int green_max, int blue_max, int step_value)
/*
$ grep Cycle_Palette_Color *
LoadScr.C:        // TODO  Cycle_Palette_Color(198, 40, 0, 0, 63, 0, 0, 1);
MainScr.C:    // TODO  Reset_Cycle_Palette_Color()  AKA VGA_BlinkReset()
MainScr.C:    Reset_Cycle_Palette_Color();
MainScr.C:        // TODO  Cycle_Palette_Color(198, 40, 0, 0, 63, 0, 0, 3);
MoX.C:                // Cycle_Palette_Color(198, 40, 0, 0, 63, 0, 0, 1)
PoC_Main.cpp:    // Cycle_Palette_Color(198, 40, 0, 0, 63, 0, 0, 1);  // (color_num, red_min, green_min, blue_min, red_max, green_max, blue_max, step_value)
SBookScr.C:    // TODO  Reset_Cycle_Palette_Color();
SCORE.C:    // TODO  UU_Reset_Cycle_Palette_Color();
grep: msdos: Is a directory
grep: mswin: Is a directory
grep: stu: Is a directory
win_MoM.cpp:    // Cycle_Palette_Color(198, 40, 0, 0, 63, 0, 0, 1);  // (color_num, red_min, green_min, blue_min, red_max, green_max, blue_max, step_value)
*/

// WZD s20p13
// Update_Cycle(int *color_min, int *color_max)

// WZD s20p14
// UU_VGA_CreateHues(int First_DAC_Reg, int Steps, int B_Red, int B_Grn, int B_Blu, int T_Red, int T_Grn, int T_Blu)



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
// PLATFORM          if(*(p_Palette + 768 + itr) == 1)
// PLATFORM          {
// PLATFORM              *(g_Palette + itr) = *(p_Palette + itr);
// PLATFORM  
// PLATFORM              *(g_Palette_XBGR + (itr * 4) + 3) = 0x00;
// PLATFORM              *(g_Palette_XBGR + (itr * 4) + 2) = (*(p_Palette + (itr * 3) + 0) << 2);
// PLATFORM              *(g_Palette_XBGR + (itr * 4) + 1) = (*(p_Palette + (itr * 3) + 1) << 2);
// PLATFORM              *(g_Palette_XBGR + (itr * 4) + 0) = (*(p_Palette + (itr * 3) + 2) << 2);
// PLATFORM          }
// PLATFORM  
// PLATFORM      }
// PLATFORM  
// PLATFORM  }


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
    uint8_t * current_palette;
    uint16_t ofst;
    uint8_t color_red;
    uint8_t color_grn;
    uint8_t color_blu;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Cycle_Palette(percent = %d)\n", __FILE__, __LINE__, percent);
#endif

    vpercent = 100 - percent;

    
    if(vpercent > 0)                    /* percent < 100 */
    {
        DLOG("(vpercent > 0)  (percent < 100)");
        if(vpercent < 100)              /* (percent < 100) && (percent > 0) */
        {
            DLOG("(vpercent < 100)");
            color_multiplier = (( percent << 8) / 100) & 0x00FF;  // ¿ Fixed_Point Math ~== << 8 ?
            current_palette = (uint8_t *)(p_Palette);
            for(itr = 0; itr < 256; itr++)
            {
                palette_change_flag = PALETTE_FLAG(itr);
                if(palette_change_flag != ST_FALSE)
                {
                    ofst = itr * 3;

                    *(PFL_Palette + (itr * 4) + 3) = 0x00;

                    color_red = *(current_palette + ofst++);
                    *(PFL_Palette + (itr * 4) + 2) = (((color_red * color_multiplier) >> 8) << 2);

                    color_grn = *(current_palette + ofst++);
                    *(PFL_Palette + (itr * 4) + 1) = (((color_grn * color_multiplier) >> 8) << 2);

                    color_blu = *(current_palette + ofst++);
                    *(PFL_Palette + (itr * 4) + 0) = (((color_blu * color_multiplier) >> 8) << 2);

                }
            }


        }
        else                            /* (percent < 100) && (percent < 0) */
        {
            DLOG("(vpercent >= 100)  (percent <= 0)");
            Apply_Palette();
            goto Done;
        }
    }
    else
    {
        DLOG("(vpercent <= 0)  (percent >= 100)");        /* (percent >= 100) */
        for(itr = 0; itr < 256; itr++)
        {
            palette_change_flag = PALETTE_FLAG(itr);
            if(palette_change_flag != ST_FALSE)
            {
                // set hardware palette color to {0,0,0} black
                // ¿ ~== set PFL_Palette color to {0,0,0} black ?
                *(PFL_Palette + (itr * 4) + 3) = 0x00;
                *(PFL_Palette + (itr * 4) + 2) = 0x00;
                *(PFL_Palette + (itr * 4) + 1) = 0x00;
                *(PFL_Palette + (itr * 4) + 0) = 0x00;
            }
        }

    }

    // ~== REP STOSW
    memset((uint8_t *)(p_Palette + 768), 0x00, 256);  // ~== `REP STOSB`

Done:

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Cycle_Palette(percent = %d)\n", __FILE__, __LINE__, percent);
#endif
	return;
}


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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Create_Remap_Palette(block = %d, red = %02X, green = %02X, blue = %02X, percent = %d)\n", __FILE__, __LINE__, block, red, green, blue, percent);
#endif

    vpercent = (100 - percent);

    if(vpercent > 0)                    /* percent < 100 */
    {
        DLOG("(vpercent > 0)  (percent < 100)");
        if(vpercent < 100)              /* (percent < 100) && (percent > 0) */
        {
            DLOG("(vpercent < 100)");
            Create_Remap_Palette_(block, red, green, blue, percent);
        }
        else                            /* (percent < 100) && (percent < 0) */
        {
            DLOG("(vpercent >= 100)  (percent <= 0)");
            Create_Remap_Palette_0(block);
        }
    }
    else
    {
        DLOG("(vpercent <= 0)  (percent >= 100)");        /* (percent >= 100) */
        Create_Remap_Palette_1(block, red, green, blue);
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Create_Remap_Palette(block = %d, red = %02X, green = %02X, blue = %02X, percent = %d)\n", __FILE__, __LINE__, block, red, green, blue, percent);
#endif
}

void Create_Remap_Palette_0(int16_t block)
{
    uint8_t * remap_palette;
    uint8_t colormap_idx;
    uint16_t counter;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Create_Remap_Palette_0(block = %d)\n", __FILE__, __LINE__, block);
#endif

    remap_palette = (uint8_t *)(remap_color_palettes + (block * 256));

    colormap_idx = 0;
    counter = 256;
    while(counter--)
    {
        *remap_palette++ = colormap_idx++;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Create_Remap_Palette_0(block = %d)\n", __FILE__, __LINE__, block);
#endif
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Create_Remap_Palette_1(block = %d, red = %02X, green = %02X, blue = %02X)\n", __FILE__, __LINE__, block, red, green, blue);
#endif

    remap_palette   = (uint8_t *)(remap_color_palettes + (block * 256));
    closest = Find_Closest_Color(red, green, blue);
    memset(remap_palette, closest, 256);  // ~== `REP STOSB`

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Create_Remap_Palette_1(block = %d, red = %02X, green = %02X, blue = %02X)\n", __FILE__, __LINE__, block, red, green, blue);
#endif
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
    uint8_t * current_palette;
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
    

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Create_Remap_Palette_(block = %d, red = %02X, green = %02X, blue = %02X, percent = %d)\n", __FILE__, __LINE__, block, red, green, blue, percent);
#endif

#ifdef STU_DEBUG
    if(block == 1)  /* DEBUG.LOG: "BEGIN: Create_Remap_Palette_(block = 1, red = 3F, green = 3F, blue = 3F, percent = 25)" */
    {
        DBG_Create_Remap_Palette = 1;
        DBG_Find_Closest_Color = 1;
        dbg_prn("DEBUG: [%s, %d]: DBG_Create_Remap_Palette: %d\n", __FILE__, __LINE__, DBG_Create_Remap_Palette);
        dbg_prn("DEBUG: [%s, %d]: DBG_Find_Closest_Color: %d\n", __FILE__, __LINE__, DBG_Find_Closest_Color);
    }
#endif

    color2_red = red;
    color2_grn = green;
    color2_blu = blue;

    vpercent = (100 - percent);
#ifdef STU_DEBUG
    if(DBG_Create_Remap_Palette == 1)
    {
        dbg_prn("DEBUG: [%s, %d]: percent: %d\n", __FILE__, __LINE__, percent);
        dbg_prn("DEBUG: [%s, %d]: vpercent: %d\n", __FILE__, __LINE__, vpercent);
    }
#endif



    color2_multiplier = (( percent << 8) / 100) & 0x00FF;  // ¿ Fixed_Point Math ~== << 8 ?
    color1_multiplier = ((vpercent << 8) / 100) & 0x00FF;  // ¿ Fixed_Point Math ~== << 8 ?
#ifdef STU_DEBUG
    if(DBG_Create_Remap_Palette == 1)
    {
        dbg_prn("DEBUG: [%s, %d]: color2_multiplier: %d\n", __FILE__, __LINE__, color2_multiplier);
        dbg_prn("DEBUG: [%s, %d]: color1_multiplier: %d\n", __FILE__, __LINE__, color1_multiplier);
    }
#endif

    // MoO2: tint_red = glass_colors[].red * glass_colors[].percent
    color2_red_portion = ((color2_red * color2_multiplier) >> 8);
    color2_grn_portion = ((color2_grn * color2_multiplier) >> 8);
    color2_blu_portion = ((color2_blu * color2_multiplier) >> 8);
#ifdef STU_DEBUG
    if(DBG_Create_Remap_Palette == 1)
    {
        dbg_prn("DEBUG: [%s, %d]: color2_red_portion: %02X\n", __FILE__, __LINE__, color2_red_portion);
        dbg_prn("DEBUG: [%s, %d]: color2_grn_portion: %02X\n", __FILE__, __LINE__, color2_grn_portion);
        dbg_prn("DEBUG: [%s, %d]: color2_blu_portion: %02X\n", __FILE__, __LINE__, color2_blu_portion);
    }
#endif

    remap_palette   = (uint8_t *)(remap_color_palettes + (block * 256));
    current_palette = (uint8_t *)(p_Palette);

    // iterate through the current build, make the new mixed color, and find the closest existing color
    for(itr = 0; itr < 256; itr++)
    {
        palette_change_flag = PALETTE_FLAG(itr);
        if(palette_change_flag != ST_FALSE)
        {
            ofst = itr * 3;

            color1_red = *(current_palette + ofst++);
            color1_grn = *(current_palette + ofst++);
            color1_blu = *(current_palette + ofst++);
#ifdef STU_DEBUG
    if(DBG_Find_Closest_Color == 1 && itr == 0x74)
    {
        DLOG("(DBG_Find_Closest_Color == 1 && itr == 0x74)");
        dbg_prn("DEBUG: [%s, %d]: color1_red: %02X\n", __FILE__, __LINE__, color1_red);
        dbg_prn("DEBUG: [%s, %d]: color1_grn: %02X\n", __FILE__, __LINE__, color1_grn);
        dbg_prn("DEBUG: [%s, %d]: color1_blu: %02X\n", __FILE__, __LINE__, color1_blu);
    }
#endif

            color1_red_portion = ((color1_red * color1_multiplier) >> 8);
            color1_grn_portion = ((color1_grn * color1_multiplier) >> 8);
            color1_blu_portion = ((color1_blu * color1_multiplier) >> 8);
#ifdef STU_DEBUG
    if(DBG_Find_Closest_Color == 1 && itr == 0x74)
    {
        dbg_prn("DEBUG: [%s, %d]: color1_red_portion: %02X\n", __FILE__, __LINE__, color1_red_portion);
        dbg_prn("DEBUG: [%s, %d]: color1_grn_portion: %02X\n", __FILE__, __LINE__, color1_grn_portion);
        dbg_prn("DEBUG: [%s, %d]: color1_blu_portion: %02X\n", __FILE__, __LINE__, color1_blu_portion);
    }
#endif

            color3_red = color1_red_portion + color2_red_portion;
            color3_grn = color1_grn_portion + color2_grn_portion;
            color3_blu = color1_blu_portion + color2_blu_portion;
#ifdef STU_DEBUG
    if(DBG_Find_Closest_Color == 1 && itr == 0x74)
    {
        dbg_prn("DEBUG: [%s, %d]: color3_red: %02X\n", __FILE__, __LINE__, color3_red);
        dbg_prn("DEBUG: [%s, %d]: color3_grn: %02X\n", __FILE__, __LINE__, color3_grn);
        dbg_prn("DEBUG: [%s, %d]: color3_blu: %02X\n", __FILE__, __LINE__, color3_blu);
    }
#endif

            closest = 0;
            closest = Find_Closest_Color(color3_red, color3_grn, color3_blu);
        }
#ifdef STU_DEBUG
    if(DBG_Find_Closest_Color == 1 && itr == 0x74)
    {
        DLOG("(DBG_Find_Closest_Color == 1 && itr == 0x74)");
        dbg_prn("DEBUG: [%s, %d]: closest: %02X\n", __FILE__, __LINE__, closest);
    }
#endif
        *(remap_palette + itr) = closest;
    }

#ifdef STU_DEBUG
        DBG_Create_Remap_Palette = 0;
        DBG_Find_Closest_Color = 0;
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Create_Remap_Palette_(block = %d, red = %02X, green = %02X, blue = %02X, percent = %d)\n", __FILE__, __LINE__, block, red, green, blue, percent);
#endif
}

/*
    Animator
        "match-color red", "...green", "...blue"
        "get [red,green,blue] from current table slot"

*/
uint8_t Find_Closest_Color(uint8_t red, uint8_t green, uint8_t blue)
{
    uint8_t * current_palette;
    uint8_t * remap_palette;
    uint16_t ofst;
    // uint8_t colormap_idx;
    int16_t colormap_idx;
    uint8_t palette_red;
    uint8_t palette_green;
    uint8_t palette_blue;
    int16_t dif_red;
    int16_t dif_green;
    int16_t dif_blue;
    // Meh.  uint16_t counter;
    int16_t current_dif;
    int16_t closest_dif;
    uint8_t closest;

    uint8_t found_color;
    
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: BEGIN: Find_Closest_Color(red = %02X, green = %02X, blue = %02X)\n", __FILE__, __LINE__, red, green, blue);
// #endif
// DELETE  #ifdef STU_DEBUG
// DELETE      if(DBG_Find_Closest_Color == 1)
// DELETE      {
// DELETE          DLOG("(DBG_Find_Closest_Color == 1)");
// DELETE      }
// DELETE      if(red == 0x21 && green == 0x19 && blue == 0x29)
// DELETE      {
// DELETE          DLOG("(red == 0x21 && green == 0x19 && blue == 0x29)");  /* mix color for block 1 and color-map index 116 0x74 */
// DELETE      }
// DELETE      if( (DBG_Find_Closest_Color == 1) && (red == 0x21 && green == 0x19 && blue == 0x29) )
// DELETE      {
// DELETE          DLOG("( (DBG_Find_Closest_Color == 1) && (red == 0x21 && green == 0x19 && blue == 0x29) )");
// DELETE          // dbg_prn("DEBUG: [%s, %d]: DBG_Find_Closest_Color: %d\n", __FILE__, __LINE__, DBG_Find_Closest_Color);
// DELETE          // DBG_Find_Closest_Color = 1;
// DELETE      }
// DELETE  //     else
// DELETE  //     {
// DELETE  //         DBG_Find_Closest_Color = 0;
// DELETE  //     }
// DELETE  #endif

    found_color = 0;

    current_palette = (uint8_t *)(p_Palette);

    closest = 0;
    closest_dif = 10000;
    // Meh.  counter = 256;
    // Meh.  ofst = 0;
    // colormap_idx = 0;
    // Meh.  while(counter--)
    for(colormap_idx = 0; colormap_idx < 256; colormap_idx++)
    {
        ofst = colormap_idx * 3;

// DELETE  #ifdef STU_DEBUG
// DELETE      // if(DBG_Find_Closest_Color == 1)
// DELETE      // if( (DBG_Find_Closest_Color == 1) && (colormap_idx == 0x72) )
// DELETE      if(DBG_Find_Closest_Color == 1)
// DELETE      {
// DELETE          if(red == 0x21 && green == 0x19 && blue == 0x29)
// DELETE          {
// DELETE              if( (colormap_idx == 0x72) || (colormap_idx == 0x8C) )  /* Yay match || Nay match */
// DELETE              {
// DELETE                  dbg_prn("DEBUG: [%s, %d]: colormap_idx: %02X (%d)\n", __FILE__, __LINE__, colormap_idx, colormap_idx);
// DELETE                  dbg_prn("DEBUG: [%s, %d]: ofst: %02X (%d)\n", __FILE__, __LINE__, ofst, ofst);
// DELETE              }
// DELETE          }
// DELETE      }
// DELETE  #endif

        // diff color = current color - glass color
        palette_red = *(current_palette + ofst++);
        dif_red = AbsVal(palette_red - red);
// DELETE #ifdef STU_DEBUG
// DELETE     // if(DBG_Find_Closest_Color == 1)
// DELETE     // if( (DBG_Find_Closest_Color == 1) && (colormap_idx == 0x72) )
// DELETE     if(DBG_Find_Closest_Color == 1)
// DELETE     {
// DELETE         if(red == 0x21 && green == 0x19 && blue == 0x29)
// DELETE         {
// DELETE             if( (colormap_idx == 0x72) || (colormap_idx == 0x8C) )  /* Yay match || Nay match */
// DELETE             {
// DELETE                 dbg_prn("DEBUG: [%s, %d]: red: %02X\n", __FILE__, __LINE__, red);
// DELETE                 dbg_prn("DEBUG: [%s, %d]: palette_red: %02X\n", __FILE__, __LINE__, palette_red);
// DELETE                 dbg_prn("DEBUG: [%s, %d]: (palette_red - red): %04X\n", __FILE__, __LINE__, (palette_red - red));
// DELETE                 dbg_prn("DEBUG: [%s, %d]: AbsVal(palette_red - red): %04X\n", __FILE__, __LINE__, AbsVal(palette_red - red));
// DELETE                 dbg_prn("DEBUG: [%s, %d]: dif_red: %02X\n", __FILE__, __LINE__, dif_red);
// DELETE             }
// DELETE         }
// DELETE     }
// DELETE #endif

        ofst += 2;
        if(dif_red >= REMAP_THRESHOLD) { continue; }
        ofst -= 2;

        palette_green = *(current_palette + ofst++);
        dif_green = AbsVal(palette_green - green);
// DELETE  #ifdef STU_DEBUG
// DELETE      // if(DBG_Find_Closest_Color == 1)
// DELETE      // if( (DBG_Find_Closest_Color == 1) && (colormap_idx == 0x72) )
// DELETE      if(DBG_Find_Closest_Color == 1)
// DELETE      {
// DELETE          if(red == 0x21 && green == 0x19 && blue == 0x29)
// DELETE          {
// DELETE              if( (colormap_idx == 0x72) || (colormap_idx == 0x8C) )  /* Yay match || Nay match */
// DELETE              {
// DELETE                  dbg_prn("DEBUG: [%s, %d]: green: %02X\n", __FILE__, __LINE__, green);
// DELETE                  dbg_prn("DEBUG: [%s, %d]: palette_green: %02X\n", __FILE__, __LINE__, palette_green);
// DELETE                  dbg_prn("DEBUG: [%s, %d]: (palette_green - green): %04X\n", __FILE__, __LINE__, (palette_green - green));
// DELETE                  dbg_prn("DEBUG: [%s, %d]: AbsVal(palette_green - green): %04X\n", __FILE__, __LINE__, AbsVal(palette_green - green));
// DELETE                  dbg_prn("DEBUG: [%s, %d]: dif_green: %02X\n", __FILE__, __LINE__, dif_green);
// DELETE              }
// DELETE          }
// DELETE      }
// DELETE  #endif

        ofst += 1;
        if(dif_green >= REMAP_THRESHOLD) { continue; }
        ofst -= 1;

        palette_blue = *(current_palette + ofst++);
        dif_blue = AbsVal(palette_blue - blue);
// DELETE  #ifdef STU_DEBUG
// DELETE      // if(DBG_Find_Closest_Color == 1)
// DELETE      // if( (DBG_Find_Closest_Color == 1) && (colormap_idx == 0x72) )
// DELETE      if(DBG_Find_Closest_Color == 1)
// DELETE      {
// DELETE          if(red == 0x21 && green == 0x19 && blue == 0x29)
// DELETE          {
// DELETE              if( (colormap_idx == 0x72) || (colormap_idx == 0x8C) )  /* Yay match || Nay match */
// DELETE              {
// DELETE                  dbg_prn("DEBUG: [%s, %d]: blue: %02X\n", __FILE__, __LINE__, blue);
// DELETE                  dbg_prn("DEBUG: [%s, %d]: palette_blue: %02X\n", __FILE__, __LINE__, palette_blue);
// DELETE                  dbg_prn("DEBUG: [%s, %d]: (palette_blue - blue): %04X\n", __FILE__, __LINE__, (palette_blue - blue));
// DELETE                  dbg_prn("DEBUG: [%s, %d]: AbsVal(palette_blue - blue): %04X\n", __FILE__, __LINE__, AbsVal(palette_blue - blue));
// DELETE                  dbg_prn("DEBUG: [%s, %d]: dif_blue: %02X\n", __FILE__, __LINE__, dif_blue);
// DELETE              }
// DELETE          }
// DELETE      }
// DELETE  #endif

        if(dif_blue >= REMAP_THRESHOLD) { continue; }

        // current_dif = dif_red * dif_red;
        // current_dif += dif_green * dif_green;
        // current_dif += dif_blue * dif_blue;
        current_dif  = dif_blue * dif_blue;
        current_dif += dif_red * dif_red;
        current_dif += dif_green * dif_green;
// DELETE  #ifdef STU_DEBUG
// DELETE      // if(DBG_Find_Closest_Color == 1)
// DELETE      // if( (DBG_Find_Closest_Color == 1) && (colormap_idx == 0x72) )
// DELETE      if(DBG_Find_Closest_Color == 1)
// DELETE      {
// DELETE          if(red == 0x21 && green == 0x19 && blue == 0x29)
// DELETE          {
// DELETE              if( (colormap_idx == 0x72) || (colormap_idx == 0x8C) )  /* Yay match || Nay match */
// DELETE              {
// DELETE                  dbg_prn("DEBUG: [%s, %d]: current_dif: %d\n", __FILE__, __LINE__, current_dif);
// DELETE              }
// DELETE          }
// DELETE      }
// DELETE  #endif

        if(current_dif < closest_dif)
        {
            closest_dif = current_dif;
            closest = colormap_idx;
// DELETE  #ifdef STU_DEBUG
// DELETE      // if(DBG_Find_Closest_Color == 1)
// DELETE      // if( (DBG_Find_Closest_Color == 1) && (colormap_idx == 0x72) )
// DELETE      if(DBG_Find_Closest_Color == 1)
// DELETE      {
// DELETE          if(red == 0x21 && green == 0x19 && blue == 0x29)
// DELETE          {
// DELETE              if( (colormap_idx == 0x72) || (colormap_idx == 0x8C) )  /* Yay match || Nay match */
// DELETE              {
// DELETE                  dbg_prn("DEBUG: [%s, %d]: closest_dif: %d\n", __FILE__, __LINE__, closest_dif);
// DELETE                  dbg_prn("DEBUG: [%s, %d]: closest: %d\n", __FILE__, __LINE__, closest);
// DELETE              }
// DELETE          }
// DELETE      }
// DELETE  #endif
        }
        // Meh. colormap_idx++;
        // assert(ofst == ((colormap_idx * 3) + 3));
    }

    found_color = closest;
    
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: END: Find_Closest_Color(red = %02X, green = %02X, blue = %02X) { found_color = %02X }\n", __FILE__, __LINE__, red, green, blue, found_color);
// #endif
    return found_color;
}

// TODO  // 1oom: uint8_t lbxpal_find_closest(uint8_t r, uint8_t g, uint8_t b)
// TODO  uint8_t Find_Closest_Color(uint8_t r, uint8_t g, uint8_t b)
// TODO  {
// TODO      uint8_t min_c = 0;
// TODO      int min_dist = 10000;
// TODO      // uint8_t *p = lbxpal_palette;
// TODO      uint8_t *p = p_Palette;
// TODO      for (int i = 0; i < 256; ++i) {
// TODO          int dist;
// TODO          dist = abs(r - *p++);
// TODO          dist += abs(g - *p++);
// TODO          dist += abs(b - *p++);
// TODO          if (dist < min_dist) {
// TODO              min_dist = dist;
// TODO              min_c = i;
// TODO              if (dist == 0) {
// TODO                  break;
// TODO              }
// TODO          }
// TODO      }
// TODO      return min_c;
// TODO  }



// DELETE  void Create_Remap_Palette_ASM(int16_t block, uint8_t red, uint8_t green, uint8_t blue, uint8_t percent)
// DELETE  {
// DELETE      uint8_t vpercent;
// DELETE      uint16_t src_sgmt;
// DELETE      uint16_t src_ofst;
// DELETE      uint8_t * src_ptr;
// DELETE      uint16_t dst_sgmt;
// DELETE      uint16_t dst_ofst;
// DELETE      uint8_t * dst_ptr;
// DELETE      uint8_t colormap_idx;
// DELETE      int16_t counter;
// DELETE      int16_t dc;
// DELETE      uint8_t color;
// DELETE      uint16_t red2;
// DELETE      uint16_t green2;
// DELETE      uint16_t blue2;
// DELETE      int16_t dif_red;
// DELETE      int16_t dif_green;
// DELETE      int16_t dif_blue;
// DELETE      uint16_t current_dif;
// DELETE      uint8_t closest;
// DELETE      uint8_t palette_flag;
// DELETE  
// DELETE      uint16_t color1_multiplier;
// DELETE      uint16_t color2_multiplier;
// DELETE      uint8_t color1_red;
// DELETE      uint8_t color1_green;
// DELETE      uint8_t color1_blue;
// DELETE      uint8_t color2_red;
// DELETE      uint8_t color2_green;
// DELETE      uint8_t color2_blue;
// DELETE      uint8_t color1_red_portion;
// DELETE      uint8_t color1_green_portion;
// DELETE      uint8_t color1_blue_portion;
// DELETE      uint8_t color2_red_portion;
// DELETE      uint8_t color2_green_portion;
// DELETE      uint8_t color2_blue_portion;
// DELETE      uint8_t color3_red;
// DELETE      uint8_t color3_green;
// DELETE      uint8_t color3_blue;
// DELETE      uint8_t palette_red;
// DELETE      uint8_t palette_green;
// DELETE      uint8_t palette_blue;
// DELETE      // uint16_t CS_closest;
// DELETE      // uint16_t CS_closest_dif;
// DELETE      uint16_t closest_dif;
// DELETE      int16_t itr_remap_palette;
// DELETE  
// DELETE  #ifdef STU_DEBUG
// DELETE      dbg_prn("DEBUG: [%s, %d]: BEGIN: Create_Remap_Palette(block = %d, red = %02X, green = %02X, blue = %02X, percent = %d)\n", __FILE__, __LINE__, block, red, green, blue, percent);
// DELETE  #endif
// DELETE  
// DELETE  #ifdef STU_DEBUG
// DELETE      if(block == 1)
// DELETE      {
// DELETE          DBG_Create_Remap_Palette = 1;
// DELETE          dbg_prn("DEBUG: [%s, %d]: DBG_Create_Remap_Palette: %d\n", __FILE__, __LINE__, DBG_Create_Remap_Palette);
// DELETE      }
// DELETE  #endif
// DELETE      // CS_red1 = red;
// DELETE      // CS_green1 = green;
// DELETE      // CS_blue1 = blue;
// DELETE      color2_red = red;
// DELETE      color2_green = green;
// DELETE      color2_blue = blue;
// DELETE  
// DELETE      vpercent = (100 - percent);
// DELETE  
// DELETE      if(vpercent > 0)  /* percent < 100 */
// DELETE      {
// DELETE          DLOG("(vpercent > 0)");
// DELETE          if(vpercent < 100)
// DELETE          {
// DELETE              DLOG("(vpercent < 100)");
// DELETE              // // uint8_t temp_scale = (vpercent * 256) / 100;
// DELETE              // // uint8_t temp_mul = (percent * 256) / 100;
// DELETE              // // uint8_t r_add = (pr * temp_mul) >> 8;
// DELETE              // // uint8_t g_add = (pg * temp_mul) >> 8;
// DELETE              // // uint8_t b_add = (pb * temp_mul) >> 8;
// DELETE              // CS_Orig_Multiplier   = (vpercent * 256) / 100;  // e.g., 100 - 25 = 75 * 256 = 19200 / 100 = 192
// DELETE              // CS_Perc_Target_Red   = ((red   * ((percent * 256) / 100)) >> 8);  // e.g., 63 * (25 * 256) / 100 = 63 * 6400 / 100 = 403200 / 100 = 4032 / 256 = 15.75
// DELETE              // CS_Perc_Target_Green = ((green * ((percent * 256) / 100)) >> 8);
// DELETE              // CS_Perc_Target_Blue  = ((blue  * ((percent * 256) / 100)) >> 8);
// DELETE              // // e.g., rmap block 1: percent == 25; vpercent == 100-25=75
// DELETE              color1_multiplier = (vpercent * 256) / 100;
// DELETE              color2_multiplier = ( percent * 256) / 100;
// DELETE  
// DELETE              color2_red_portion   = ((red   * color2_multiplier) >> 8);
// DELETE              color2_green_portion = ((green * color2_multiplier) >> 8);
// DELETE              color2_blue_portion  = ((blue  * color2_multiplier) >> 8);
// DELETE  
// DELETE  
// DELETE  #ifdef STU_DEBUG
// DELETE      if(DBG_Create_Remap_Palette == 1)
// DELETE      {
// DELETE          dbg_prn("DEBUG: [%s, %d]: color1_multiplier: %d\n", __FILE__, __LINE__, color1_multiplier);
// DELETE          dbg_prn("DEBUG: [%s, %d]: color2_multiplier: %d\n", __FILE__, __LINE__, color2_multiplier);
// DELETE  
// DELETE      }
// DELETE  #endif
// DELETE  #ifdef STU_DEBUG
// DELETE      if(DBG_Create_Remap_Palette == 1)
// DELETE      {
// DELETE          dbg_prn("DEBUG: [%s, %d]: color2_red_portion: %d\n", __FILE__, __LINE__, color2_red_portion);
// DELETE          dbg_prn("DEBUG: [%s, %d]: color2_green_portion: %d\n", __FILE__, __LINE__, color2_green_portion);
// DELETE          dbg_prn("DEBUG: [%s, %d]: color2_blue_portion: %d\n", __FILE__, __LINE__, color2_blue_portion);
// DELETE  
// DELETE      }
// DELETE  #endif
// DELETE  
// DELETE              // dst_ofst = (remap_color_palettes + (block * 16));
// DELETE              // src_ofst = (p_Palette);
// DELETE              dst_ptr = (uint8_t *)( (remap_color_palettes + (block * (16 * 16))) + 0 );
// DELETE              src_ptr = (uint8_t *)( (p_Palette) + 0);
// DELETE  
// DELETE              itr_remap_palette = 0;
// DELETE              while(itr_remap_palette < 256)
// DELETE              {
// DELETE                  src_ofst = 768 + itr_remap_palette;
// DELETE                  if(*(src_ptr + src_ofst++) != 0)  /* palette change flag */
// DELETE                  {
// DELETE                      // DLOG("(*(src_ptr + src_ofst++) != 0)");
// DELETE  
// DELETE                      src_ofst = itr_remap_palette * 3;
// DELETE  
// DELETE                      // // r = (((*p++ * temp_scale) >> 8) + r_add) & 0xff;
// DELETE                      // // g = (((*p++ * temp_scale) >> 8) + g_add) & 0xff;
// DELETE                      // // b = (((*p++ * temp_scale) >> 8) + b_add) & 0xff;
// DELETE                      // CS_Total_Target_Red = (*(src_ptr + src_ofst++) * CS_Orig_Multiplier) + CS_Perc_Target_Red;
// DELETE                      // CS_Total_Target_Green = (*(src_ptr + src_ofst++) * CS_Orig_Multiplier) + CS_Perc_Target_Green;
// DELETE                      // CS_Total_Target_Blue = (*(src_ptr + src_ofst++) * CS_Orig_Multiplier) + CS_Perc_Target_Blue;
// DELETE                      color1_red = *(src_ptr + src_ofst++);
// DELETE                      color1_green = *(src_ptr + src_ofst++);
// DELETE                      color1_blue = *(src_ptr + src_ofst++);
// DELETE  #ifdef STU_DEBUG
// DELETE      if( (DBG_Create_Remap_Palette == 1) && (itr_remap_palette == 116) )
// DELETE      {
// DELETE          dbg_prn("DEBUG: [%s, %d]: color1_red: %d\n", __FILE__, __LINE__, color1_red);
// DELETE          dbg_prn("DEBUG: [%s, %d]: color1_green: %d\n", __FILE__, __LINE__, color1_green);
// DELETE          dbg_prn("DEBUG: [%s, %d]: color1_blue: %d\n", __FILE__, __LINE__, color1_blue);
// DELETE      }
// DELETE  #endif
// DELETE  
// DELETE                      color1_red_portion   = ((color1_red   * color1_multiplier) >> 8);
// DELETE                      color1_green_portion = ((color1_green * color1_multiplier) >> 8);
// DELETE                      color1_blue_portion  = ((color1_blue  * color1_multiplier) >> 8);
// DELETE  #ifdef STU_DEBUG
// DELETE      if( (DBG_Create_Remap_Palette == 1) && (itr_remap_palette == 116) )
// DELETE      {
// DELETE          dbg_prn("DEBUG: [%s, %d]: color1_red_portion: %d\n", __FILE__, __LINE__, color1_red_portion);
// DELETE          dbg_prn("DEBUG: [%s, %d]: color1_green_portion: %d\n", __FILE__, __LINE__, color1_green_portion);
// DELETE          dbg_prn("DEBUG: [%s, %d]: color1_blue_portion: %d\n", __FILE__, __LINE__, color1_blue_portion);
// DELETE      }
// DELETE  #endif
// DELETE  
// DELETE                      color3_red   = color1_red_portion   + color2_red_portion;
// DELETE                      color3_green = color1_green_portion + color2_green_portion;
// DELETE                      color3_blue  = color1_blue_portion  + color2_blue_portion;
// DELETE  
// DELETE  #ifdef STU_DEBUG
// DELETE      if(DBG_Create_Remap_Palette == 1)
// DELETE      {
// DELETE          // dbg_prn("DEBUG: [%s, %d]: CS_Total_Target_Red: %d\n", __FILE__, __LINE__, CS_Total_Target_Red);
// DELETE          // dbg_prn("DEBUG: [%s, %d]: CS_Total_Target_Green: %d\n", __FILE__, __LINE__, CS_Total_Target_Green);
// DELETE          // dbg_prn("DEBUG: [%s, %d]: CS_Total_Target_Blue: %d\n", __FILE__, __LINE__, CS_Total_Target_Blue);
// DELETE      }
// DELETE  #endif
// DELETE  #ifdef STU_DEBUG
// DELETE      if( (DBG_Create_Remap_Palette == 1) && (itr_remap_palette == 116) )
// DELETE      {
// DELETE          dbg_prn("DEBUG: [%s, %d]: color3_red: %d\n", __FILE__, __LINE__, color3_red);
// DELETE          dbg_prn("DEBUG: [%s, %d]: color3_green: %d\n", __FILE__, __LINE__, color3_green);
// DELETE          dbg_prn("DEBUG: [%s, %d]: color3_blue: %d\n", __FILE__, __LINE__, color3_blue);
// DELETE      }
// DELETE  #endif
// DELETE  
// DELETE                      /*
// DELETE                          BEGIN: Find Closest Color
// DELETE                      */
// DELETE                      closest = 0;
// DELETE                      closest_dif = 10000;
// DELETE                      counter = 256;
// DELETE                      src_ofst = 0;
// DELETE                      dst_ofst = 0;
// DELETE  
// DELETE                      while(counter--)
// DELETE                      {
// DELETE  #ifdef STU_DEBUG
// DELETE      if( (DBG_Create_Remap_Palette == 1) && (itr_remap_palette == 116) )
// DELETE      {
// DELETE          dbg_prn("DEBUG: [%s, %d]: counter: %d\n", __FILE__, __LINE__, counter);
// DELETE      }
// DELETE  #endif
// DELETE                          palette_flag = *(src_ptr + 768 + dst_ofst);
// DELETE                          src_ofst += 3;
// DELETE                          if(palette_flag != 0)  /* palette change flag */
// DELETE                          {
// DELETE                              // DLOG("(palette_flag != 0)");
// DELETE                              src_ofst -= 3;
// DELETE                              // red2 = *(src_ptr + src_ofst++);
// DELETE                              // dif_red = red2 - CS_Total_Target_Red;
// DELETE                              palette_red = *(src_ptr + src_ofst++);
// DELETE  #ifdef STU_DEBUG
// DELETE      if( (DBG_Create_Remap_Palette == 1) && (itr_remap_palette == 116) )
// DELETE      {
// DELETE          dbg_prn("DEBUG: [%s, %d]: palette_red: %d\n", __FILE__, __LINE__, palette_red);
// DELETE      }
// DELETE  #endif
// DELETE                              dif_red = palette_red - color3_red;
// DELETE  #ifdef STU_DEBUG
// DELETE      if( (DBG_Create_Remap_Palette == 1) && (itr_remap_palette == 116) )
// DELETE      {
// DELETE          dbg_prn("DEBUG: [%s, %d]: dif_red: %d\n", __FILE__, __LINE__, dif_red);
// DELETE      }
// DELETE  #endif
// DELETE                              if(dif_red < 0) { dif_red = -dif_red; }
// DELETE  #ifdef STU_DEBUG
// DELETE      if( (DBG_Create_Remap_Palette == 1) && (itr_remap_palette == 116) )
// DELETE      {
// DELETE          dbg_prn("DEBUG: [%s, %d]: dif_red: %d\n", __FILE__, __LINE__, dif_red);
// DELETE      }
// DELETE  #endif
// DELETE                              src_ofst += 2;
// DELETE                              if(dif_red >= 21) { continue; }
// DELETE                              src_ofst -= 2;
// DELETE                              // green2 = *(src_ptr + src_ofst++);
// DELETE                              // dif_green = green2 - CS_green1;
// DELETE                              palette_green = *(src_ptr + src_ofst++);
// DELETE  #ifdef STU_DEBUG
// DELETE      if( (DBG_Create_Remap_Palette == 1) && (itr_remap_palette == 116) )
// DELETE      {
// DELETE          dbg_prn("DEBUG: [%s, %d]: palette_green: %d\n", __FILE__, __LINE__, palette_green);
// DELETE      }
// DELETE  #endif
// DELETE                              dif_green = palette_green - color3_green;
// DELETE  #ifdef STU_DEBUG
// DELETE      if( (DBG_Create_Remap_Palette == 1) && (itr_remap_palette == 116) )
// DELETE      {
// DELETE          dbg_prn("DEBUG: [%s, %d]: dif_green: %d\n", __FILE__, __LINE__, dif_green);
// DELETE      }
// DELETE  #endif
// DELETE                              if(dif_green < 0) { dif_green = -dif_green; }
// DELETE  #ifdef STU_DEBUG
// DELETE      if( (DBG_Create_Remap_Palette == 1) && (itr_remap_palette == 116) )
// DELETE      {
// DELETE          dbg_prn("DEBUG: [%s, %d]: dif_green: %d\n", __FILE__, __LINE__, dif_green);
// DELETE      }
// DELETE  #endif
// DELETE                              src_ofst += 1;
// DELETE                              if(dif_green >= 21) { continue; }
// DELETE                              src_ofst -= 1;
// DELETE                              // blue2 = *(src_ptr + src_ofst++);
// DELETE                              // dif_blue = blue2 - CS_blue1;
// DELETE                              palette_blue = *(src_ptr + src_ofst++);
// DELETE  #ifdef STU_DEBUG
// DELETE      if( (DBG_Create_Remap_Palette == 1) && (itr_remap_palette == 116) )
// DELETE      {
// DELETE          dbg_prn("DEBUG: [%s, %d]: palette_blue: %d\n", __FILE__, __LINE__, palette_blue);
// DELETE      }
// DELETE  #endif
// DELETE                              dif_blue = palette_blue - color3_blue;
// DELETE  #ifdef STU_DEBUG
// DELETE      if( (DBG_Create_Remap_Palette == 1) && (itr_remap_palette == 116) )
// DELETE      {
// DELETE          dbg_prn("DEBUG: [%s, %d]: dif_blue: %d\n", __FILE__, __LINE__, dif_blue);
// DELETE      }
// DELETE  #endif
// DELETE                              if(dif_blue < 0) { dif_blue = -dif_blue; }
// DELETE  #ifdef STU_DEBUG
// DELETE      if( (DBG_Create_Remap_Palette == 1) && (itr_remap_palette == 116) )
// DELETE      {
// DELETE          dbg_prn("DEBUG: [%s, %d]: dif_blue: %d\n", __FILE__, __LINE__, dif_blue);
// DELETE      }
// DELETE  #endif
// DELETE                              if(dif_blue >= 21) { continue; }
// DELETE  
// DELETE                              current_dif = dif_red * dif_red;
// DELETE                              current_dif += dif_green * dif_green;
// DELETE                              current_dif += dif_blue * dif_blue;
// DELETE  #ifdef STU_DEBUG
// DELETE      if( (DBG_Create_Remap_Palette == 1) && (itr_remap_palette == 116) )
// DELETE      {
// DELETE          dbg_prn("DEBUG: [%s, %d]: current_dif: %d\n", __FILE__, __LINE__, current_dif);
// DELETE          dbg_prn("DEBUG: [%s, %d]: closest_dif: %d\n", __FILE__, __LINE__, closest_dif);
// DELETE      }
// DELETE  #endif
// DELETE                              if(current_dif < closest_dif)
// DELETE                              {
// DELETE                                  // DLOG("(current_dif < closest_dif)");
// DELETE                                  closest_dif = current_dif;
// DELETE                                  closest = counter;
// DELETE  #ifdef STU_DEBUG
// DELETE      if( (DBG_Create_Remap_Palette == 1) && (itr_remap_palette == 116) )
// DELETE      {
// DELETE          dbg_prn("DEBUG: [%s, %d]: closest_dif: %d\n", __FILE__, __LINE__, closest_dif);
// DELETE          dbg_prn("DEBUG: [%s, %d]: closest: %d\n", __FILE__, __LINE__, closest);
// DELETE      }
// DELETE  #endif
// DELETE                              }
// DELETE                          }
// DELETE                          dst_ofst++;
// DELETE                      }
// DELETE                      /*
// DELETE                          END: Find Closest Color
// DELETE                      */
// DELETE  
// DELETE                      // *((remap_color_palettes + (block * 16)) + itr) = closest;
// DELETE                      *(dst_ptr + itr_remap_palette) = closest;
// DELETE  #ifdef STU_DEBUG
// DELETE      if( (DBG_Create_Remap_Palette == 1) && (itr_remap_palette == 116) )
// DELETE      {
// DELETE          dbg_prn("DEBUG: [%s, %d]: remap_color_palettes[%d][%d] = p_Palette[%d]\n", __FILE__, __LINE__, block, itr_remap_palette, closest);
// DELETE      }
// DELETE  #endif
// DELETE  #ifdef STU_DEBUG
// DELETE      if( (DBG_Create_Remap_Palette == 1) && (itr_remap_palette == 116) )
// DELETE      {
// DELETE          dbg_prn("DEBUG: [%s, %d]: GET_1B_OFS(remap_color_palettes, ((block * 16) + itr_remap_palette)): %02X\n", __FILE__, __LINE__, GET_1B_OFS(remap_color_palettes, ((block * (16 * 16)) + itr_remap_palette)));
// DELETE      }
// DELETE  #endif
// DELETE                      assert( (GET_1B_OFS(remap_color_palettes, ((1 * (16 * 16)) + 116)) == 0xC0) || (GET_1B_OFS(remap_color_palettes, ((1 * (16 * 16)) + 116)) == 140) );
// DELETE  
// DELETE                  }
// DELETE                  itr_remap_palette++;
// DELETE              }
// DELETE              /*
// DELETE                  END: Remap All
// DELETE              */
// DELETE          }
// DELETE          else  /* vpercent >= 100  ~==  percent <= 0 */
// DELETE          {
// DELETE              DLOG("(vpercent >= 100)");
// DELETE              // dst_ofst = (remap_color_palettes + (block * 16));
// DELETE              // dst_ofst = 0;
// DELETE              dst_ptr = (uint8_t *)( (remap_color_palettes + (block * (16 * 16))) + 0 );
// DELETE              colormap_idx = 0;
// DELETE              counter = 256;
// DELETE              while(counter--)
// DELETE              {
// DELETE                  *dst_ptr++ = colormap_idx++;
// DELETE              }
// DELETE  
// DELETE          }
// DELETE      }
// DELETE      else  /* vpercent <= 0  ~==  percent >= 100 */
// DELETE      {
// DELETE          DLOG("(vpercent <= 0)");
// DELETE          // dst_ofst = (remap_color_palettes + (block * 16));
// DELETE          // src_ofst = (p_Palette);
// DELETE          dst_ptr = (uint8_t *)( (remap_color_palettes + (block * (16 * 16))) + 0 );
// DELETE          src_ptr = (uint8_t *)( (p_Palette) + 0);
// DELETE          closest = 0;
// DELETE          closest_dif = 10000;
// DELETE          counter = 256;
// DELETE          // src_ofst = 0;
// DELETE          // dst_ofst = 0;
// DELETE  
// DELETE          while(counter--)
// DELETE          {
// DELETE              // red2 = *src_ptr++;
// DELETE              // dif_red = red2 - CS_red1;
// DELETE              palette_red = *src_ptr++;
// DELETE              dif_red = palette_red - color2_red;
// DELETE  
// DELETE              if(dif_red < 0) { dif_red = -dif_red; }
// DELETE              src_ptr += 2;
// DELETE              if(dif_red >= 21) { continue; }
// DELETE              src_ptr -= 2;
// DELETE              // green2 = *src_ptr++;
// DELETE              // dif_green = green2 - CS_green1;
// DELETE              palette_green = *src_ptr++;
// DELETE              dif_green = palette_green - color2_green;
// DELETE  
// DELETE              if(dif_green < 0) { dif_green = -dif_green; }
// DELETE              src_ptr += 1;
// DELETE              if(dif_green >= 21) { continue; }
// DELETE              src_ptr -= 1;
// DELETE              // blue2 = *src_ptr++;
// DELETE              // dif_blue = blue2 - CS_blue1;
// DELETE              palette_blue = *src_ptr++;
// DELETE              dif_blue = palette_blue - color2_blue;
// DELETE  
// DELETE              if(dif_blue < 0) { dif_blue = -dif_blue; }
// DELETE              if(dif_blue >= 21) { continue; }
// DELETE  
// DELETE              current_dif = dif_red * dif_red;
// DELETE              current_dif += dif_green * dif_green;
// DELETE              current_dif += dif_blue * dif_blue;
// DELETE  
// DELETE              if(current_dif < closest_dif)
// DELETE              {
// DELETE                  closest_dif = current_dif;
// DELETE                  closest = counter;
// DELETE              }
// DELETE          }
// DELETE  
// DELETE      }
// DELETE  
// DELETE  #ifdef STU_DEBUG
// DELETE      DBG_Create_Remap_Palette = 0;
// DELETE  #endif
// DELETE  
// DELETE  #ifdef STU_DEBUG
// DELETE      dbg_prn("DEBUG: [%s, %d]: END: Create_Remap_Palette(block = %d, red = %02X, green = %02X, blue = %02X, percent = %d)\n", __FILE__, __LINE__, block, red, green, blue, percent);
// DELETE  #endif
// DELETE  }
