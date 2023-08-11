
#include "MoX_TYPE.H"
#include "MoX_DEF.H"

#include "Fonts.H"

extern uint8_t g_Palette[];
extern uint8_t g_Palette_XBGR[];

#include "Allocate.H"
#include "LBX_Load.H"
#include "Video.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif

#include <stdio.h>      /* FILE; fclose(), fopen(), fread(), fseek(); */
#include <stdlib.h>     /* itoa() */
// #include <string.h>     /* strcat(), strcpy() */
#include <string.h>         /* strcpy() */

#include <assert.h>



// WZD dseg:783C
uint16_t outline_color = 0;
// WZD dseg:783E
int16_t draw_alias_flag;


// WZD dseg:E7E8
// current_palette
// WZD dseg:E7EA
char font_name[16];                             // set in Load_Font_File
// WZD dseg:E7FA
// Intensity_Scale_Tbl@ dw 0               ; 60h LBX_Alloc_Space paragraphs (should be only 10h)
// WZD dseg:E7FC
SAMB_ptr palette_data;
// WZD dseg:E7FE0                               ; 15Ch paragraphs
SAMB_ptr palette_block;

// WZD dseg:E800 00 00 00 00 00 00 00 00+VGA_AAColor_Array db 10h dup(0)
// WZD dseg:E810 00                      VGA_Current_AA_Color db 0
// WZD dseg:E811 00                      VGA_AA_Color_1 db 0
// WZD dseg:E812 00                      VGA_AA_Color_2 db 0
// WZD dseg:E813 00                      VGA_AA_Color_3 db 0
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
byte_ptr p_PaletteFlags;                        // MGC dseg:A7D6    alloc in Load_Font_File()
// DELETE  byte_ptr p_Palette_XBGR;                        // STU/Win32



void Load_Font_File(char * font_file)
{
    int itr;

    strcpy(font_name, font_file);

    font_style_data = LBX_Load(font_file, 0);
    border_style_data = LBX_Load(font_file, 1);

    font_header = (struct s_FONT_HEADER *)font_style_data;

    palette_block          = Allocate_Space(348);    // 348 paragraphs = 386 * 16 bytes = 5568 bytes
    p_Palette              = Allocate_Space(64);     //  64 paragraphs =  64 * 16 bytes = 1024 bytes
    p_PaletteFlags         = p_Palette + (48 * 16);  // ~== p_PaletteFlags = &p_Palette[768];
// DELETE      p_Palette_XBGR         = Allocate_Space(64);     // STU/Win32  4 bytes per pixel * 256 colors / 16 bytes per paragraph

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
// ? MoO2 Set_Font_Highlight_Colors ?

// WZD s17p11
// ? MoO2 Set_Font_Special_Colors ?
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
int16_t Print_Centered(int16_t x, int16_t y, char * string)
{
    int16_t next_x;
    int16_t string_len;

    string_len = Get_String_Width(string);

    next_x = Print((x - (string_len/2)), y, string);

    return next_x;
}


// WZD s17p25
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


// WZD s17p35
int16_t Print(int16_t x, int16_t y, char * string)
{
    int16_t next_x;

    next_x = Print_Display(x, y, string, ST_FALSE);

    return next_x;
}


// WZD s17p36
int16_t Print_Display(int16_t x, int16_t y, char * string, int16_t full_flag)
{
    int16_t itr;
    int16_t next_x;
    uint16_t outline_style;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Print_Display(x = %d, y = %d, string = %s, full_flag = %d)\n", __FILE__, __LINE__, x, y, string, full_flag);
#endif

    // // outline_style = farpeekb(font_style_data, FONT_HEADER_SHADOW_FLAG);
    // outline_style = 0;  // ~None
    outline_style = GET_1B_OFS(font_style_data, FONT_HDR_POS_SHADOW_FLAG);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: outline_style: %d\n", __FILE__, __LINE__, outline_style);
#endif

    if(outline_style != 0)  /* ¿ ST_NONE ? */
    {
        DLOG("(outline_style != 0)");
        for(itr = 0; itr < 16; itr++)
        {
            // farpokeb(font_style_data, itr, outline_color);
            SET_1B_OFS(font_style_data, FONT_HDR_POS_CURRENT_COLORS + itr, outline_color);
        }
        draw_alias_flag = ST_TRUE;
        
        if(outline_style != 2) /* Shadow_TopLeft */
        {
            DLOG("(outline_style != 2)");
            Print_String(x + 1, y + 1, string, ST_FALSE, full_flag);  // overdraw right + botton
            Print_String(x    , y + 1, string, ST_FALSE, full_flag);  // overdraw bottom
            Print_String(x + 1, y    , string, ST_FALSE, full_flag);  // overdraw right
        }
        if( outline_style != 1 && outline_style != 3)  /* Shadow_BtmRight || Shadow_BtmRight_2px */
        {
            DLOG("( outline_style != 1 && outline_style != 3)");
            Print_String(x - 1, y    , string, ST_FALSE, full_flag);  // overdraw left
            Print_String(x - 1, y - 1, string, ST_FALSE, full_flag);  // overdraw left + top
            Print_String(x    , y - 1, string, ST_FALSE, full_flag);  // overdraw top
        }
        if(outline_style == 3 || outline_style == 5)  /* Shadow_BtmRight_2px || Outline_Plus_BR2px */
        {
            DLOG("(outline_style == 3 || outline_style == 5)");
            Print_String(x + 2, y + 2, string, ST_FALSE, full_flag);
            Print_String(x + 1, y + 2, string, ST_FALSE, full_flag);
            Print_String(x + 2, y + 1, string, ST_FALSE, full_flag);
        }
        if(outline_style > 3)  /* Shadow_BtmRight_2px */
        {
            DLOG("(outline_style > 3)");
            Print_String(x + 1, y - 1, string, ST_FALSE, full_flag);  // overdraw right + top
            Print_String(x - 1, y + 1, string, ST_FALSE, full_flag);  // overdraw left + bottom
        }
        if(outline_style == 5)  /* Outline_Plus_BR2px */
        {
            DLOG("(outline_style == 5)");
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Print_Display(x = %d, y = %d, string = %s, full_flag = %d)\n", __FILE__, __LINE__, x, y, string, full_flag);
#endif

    return next_x;
}


// WZD s17p37
int16_t Print_String(int16_t x, int16_t y, char * string, int16_t change_color_ok_flag, int16_t full_flag)
{
    int16_t next_x;
    // char character_;
    // Char_Index
    int16_t space_add;
    int16_t space_remainder;
    int16_t current_space;
    int16_t space_count;
    char character;
    uint16_t ptr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Print_String(x = %d, y = %d, string = %s, change_color_ok_flag = %d, full_flag = %d)\n", __FILE__, __LINE__, x, y, string, change_color_ok_flag, full_flag);
#endif

    ptr = 0;

    print_xpos = x;
    print_ypos = y;

    if(full_flag != ST_FALSE)
    {
        DLOG("(full_flag != ST_FALSE)");
        current_space = 0;
        space_remainder = 0;
        // Char_Index = 0;
        space_count = 0;


        // ...
        space_add = 0;
        // ...


        if(space_count == 0)
        {
            full_flag = ST_FALSE;
        }
    }



    // while(character = string[ptr++] != '\0')
    while(string[ptr] != '\0')
    {
        character = string[ptr];
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: string: %s\n", __FILE__, __LINE__, string);
    dbg_prn("DEBUG: [%s, %d]: string[%d]: %d\n", __FILE__, __LINE__, ptr, string[ptr]);
    dbg_prn("DEBUG: [%s, %d]: character: %d\n", __FILE__, __LINE__, character);
#endif

        switch(character)
        {
            case 1:
            {
                DLOG("case 1:");
                if(change_color_ok_flag != ST_FALSE)
                {
                    DLOG("case 1: && change_color_ok_flag != ST_FALSE)");
                    Set_Normal_Colors_On();
                }
            } break;
            case 2:
            {
                DLOG("case 2:");
                if(change_color_ok_flag != ST_FALSE)
                {
                    DLOG("case 2: && change_color_ok_flag != ST_FALSE)");
                    Set_Highlight_Colors_On();
                }
            } break;
            case 3:
            {
                DLOG("case 3:");
                if(change_color_ok_flag != ST_FALSE)
                {
                    DLOG("case 3: && change_color_ok_flag != ST_FALSE)");
                    Set_Special_Colors_On();
                }
            } break;
            case 4:
            {
                DLOG("case 4:");
                if(change_color_ok_flag != ST_FALSE)
                {
                    DLOG("case 4: && change_color_ok_flag != ST_FALSE)");
                    Set_Highlight_Colors_On();
                }
            } break;
            case 13:
                DLOG("case 13:");
            case 20:
            {
                DLOG("case 20:");
                print_ypos += GET_2B_OFS(font_style_data,FONT_HDR_POS_CURRENT_BASE_HEIGHT);
                print_xpos += x;
            } break;
            case 21:
            {
                DLOG("case 21:");
            } break;
            case 25:
                DLOG("case 25:");
            case 29:
            {
                DLOG("case 29:");
                print_xpos += x + string[ptr++];
            } break;

        }


        if(draw_alias_flag != ST_FALSE)
        {
            DLOG("(draw_alias_flag != ST_FALSE)");
            print_xpos = Print_Character_No_Alias(print_xpos, print_ypos, character);
        }
        else
        {
            DLOG("(draw_alias_flag == ST_FALSE)");
            print_xpos = Print_Character(print_xpos, print_ypos, character);
        }

        if(full_flag != ST_FALSE)
        {
            DLOG("(full_flag != ST_FALSE)");
            if(character == 32)  /* ASCII SPACE 0x20 ' ' */
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Print_String(x = %d, y = %d, string = %s, change_color_ok_flag = %d, full_flag = %d)\n", __FILE__, __LINE__, x, y, string, change_color_ok_flag, full_flag);
#endif

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
void Set_Font(int16_t font_idx, int16_t color1, int16_t color2, int16_t color3)
{
    int16_t itr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Set_Font(font_idx = %d, color1 = %d, color2 = %d, color3 = %d)\n", __FILE__, __LINE__, font_idx, color1, color2, color3);
#endif

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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Set_Font(font_idx = %d, color1 = %d, color2 = %d, color3 = %d)\n", __FILE__, __LINE__, font_idx, color1, color2, color3);
#endif

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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Print_Character(x = %d, y = %d, char_num = %d)\n", __FILE__, __LINE__, x, y, char_num);
#endif
#ifdef STU_DEBUG
    // if(x == 271 && y == 101 && char_num == 50)
    if(
        (x == 271 && y == 101 && char_num == 50) || 
        (x == 271 && y == 133 && char_num == 53)
    )
    {
        DLOG("DBG_Print_Character");
        DLOG("DBG_Print_Character_ASM");
        DBG_Print_Character = 1;
        DBG_Print_Character_ASM = 1;
    }
#endif

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
#ifdef STU_DEBUG
    // if(x == 271 && y == 101 && char_num == 50)
    if(
        (x == 271 && y == 101 && char_num == 50) || 
        (x == 271 && y == 133 && char_num == 53)
    )
    {
        DBG_Print_Character = 0;
        DBG_Print_Character_ASM = 0;
    }
#endif
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Print_Character(x = %d, y = %d, char_num = %d)\n", __FILE__, __LINE__, x, y, char_num);
#endif

    return next_x;
}


// WZD s18p03
int16_t Print_Character_No_Alias(int16_t x, int16_t y, int16_t char_num)
{
    int16_t next_x;
    int16_t width;
    byte_ptr font_data_offset;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Print_Character_No_Alias(x = %d, y = %d, char_num = %d)\n", __FILE__, __LINE__, x, y, char_num);
#endif
// #ifdef STU_DEBUG
//     if(x == 272 && y == 100 && char_num == 50)
//     {
//         DLOG("DBG_Print_Character_No_Alias");
//         DBG_Print_Character_No_Alias = 1;
//         DBG_Print_Character_No_Alias_ASM = 1;
//     }
// #endif

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
// #ifdef STU_DEBUG
//     if(x == 272 && y == 100 && char_num == 50)
//     {
//         DBG_Print_Character_No_Alias = 0;
//         DBG_Print_Character_No_Alias_ASM = 0;
//     }
// #endif
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Print_Character_No_Alias(x = %d, y = %d, char_num = %d)\n", __FILE__, __LINE__, x, y, char_num);
#endif
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Print_Character_ASM(x_start = %d, y_start = %d, width = %d, font_data_offset = %p)\n", __FILE__, __LINE__, x_start, y_start, width, font_data_offset);
#endif
#ifdef STU_DEBUG
    for(color_index = 0; color_index < 16; color_index++)
    {
        palette_index = GET_1B_OFS(font_style_data,FONT_HDR_POS_CURRENT_COLORS + color_index);
        dbg_prn("DEBUG: [%s, %d]: current_colors[%d]: 0x%02X\n", __FILE__, __LINE__, color_index, palette_index);
    }
#endif
    screen_start = current_video_page + ((y_start * SCREEN_WIDTH) + x_start);

    screen_pos = screen_start;
    while(width)
    {

        font_data_byte = *font_data_offset++;
#ifdef STU_DEBUG
    if(DBG_Print_Character_ASM == 1)
    {
        dbg_prn("DEBUG: [%s, %d]: font_data_byte: 0x%02X\n", __FILE__, __LINE__, font_data_byte);
    }
#endif

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
#ifdef STU_DEBUG
    if(DBG_Print_Character_ASM == 1)
    {
        dbg_prn("DEBUG: [%s, %d]: skip_count: %d\n", __FILE__, __LINE__, skip_count);
    }
#endif

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
#ifdef STU_DEBUG
    if(DBG_Print_Character_ASM == 1)
    {
        dbg_prn("DEBUG: [%s, %d]: color_index: 0x%02X\n", __FILE__, __LINE__, color_index);
    }
#endif

                // *screen_pos = font_header->current_colors[color_index];
                // FTW palette_index = font_header->current_colors[color_index];
                palette_index = GET_1B_OFS(font_style_data,FONT_HDR_POS_CURRENT_COLORS + color_index);
#ifdef STU_DEBUG
    if(DBG_Print_Character_ASM == 1)
    {
        dbg_prn("DEBUG: [%s, %d]: palette_index: 0x%02X\n", __FILE__, __LINE__, palette_index);
    }
#endif
                *screen_pos = palette_index;

                screen_pos += SCREEN_WIDTH;
            }
        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Print_Character_ASM(x_start = %d, y_start = %d, width = %d, font_data_offset = %p)\n", __FILE__, __LINE__, x_start, y_start, width, font_data_offset);
#endif

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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Print_Character_No_Alias_ASM(x_start = %d, y_start = %d, width = %d, font_data_offset = %p)\n", __FILE__, __LINE__, x_start, y_start, width, font_data_offset);
#endif

#ifdef STU_DEBUG
    if(DBG_Print_Character_No_Alias == 1)
    {

    }
#endif

    screen_start = current_video_page + ((y_start * SCREEN_WIDTH) + x_start);

    screen_pos = screen_start;
    while(width)
    {
        font_data_byte = *font_data_offset++;
#ifdef STU_DEBUG
    if(DBG_Print_Character_No_Alias == 1)
    {
        dbg_prn("DEBUG: [%s, %d]: font_data_byte: 0x%02X\n", __FILE__, __LINE__, font_data_byte);
    }
#endif

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
#ifdef STU_DEBUG
    if(DBG_Print_Character_No_Alias == 1)
    {
        dbg_prn("DEBUG: [%s, %d]: skip_count: %d\n", __FILE__, __LINE__, skip_count);
    }
#endif
            while(skip_count--)
            {
                screen_pos += SCREEN_WIDTH;
            }
        }

        // @@Nay_Negative
        if((font_data_byte & 0x80) == 0)
        {
            repeat_count = ((font_data_byte & 0xF0) >> 4);
#ifdef STU_DEBUG
    if(DBG_Print_Character_No_Alias == 1)
    {
        dbg_prn("DEBUG: [%s, %d]: repeat_count: %d\n", __FILE__, __LINE__, repeat_count);
    }
#endif
            if((font_data_byte & 0x0F) == 0)
            {
#ifdef STU_DEBUG
    if(DBG_Print_Character_No_Alias == 1)
    {
        DLOG("(font_data_byte == 0)");
    }
#endif
                screen_pos += SCREEN_WIDTH;
            }
            else
            {
#ifdef STU_DEBUG
    if(DBG_Print_Character_No_Alias == 1)
    {
        DLOG("(font_data_byte != 0)");
    }
#endif
                while(repeat_count--)
                {
                    color_index = (font_data_byte & 0x0F);
#ifdef STU_DEBUG
    if(DBG_Print_Character_No_Alias == 1)
    {
        dbg_prn("DEBUG: [%s, %d]: color_index: 0x%02X\n", __FILE__, __LINE__, color_index);
    }
#endif
                    // *screen_pos = font_header->current_colors[color_index];
                    palette_index = font_header->current_colors[color_index];
#ifdef STU_DEBUG
    if(DBG_Print_Character_No_Alias == 1)
    {
        dbg_prn("DEBUG: [%s, %d]: palette_index: 0x%02X\n", __FILE__, __LINE__, palette_index);
    }
#endif
                    *screen_pos = palette_index;
                    screen_pos += SCREEN_WIDTH;
                }
            }

        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Print_Character_No_Alias_ASM(x_start = %d, y_start = %d, width = %d, font_data_offset = %p)\n", __FILE__, __LINE__, x_start, y_start, width, font_data_offset);
#endif

}



// WZD s18p07
int16_t Get_String_Width(char * string)
{
    int16_t pos;
    int16_t width;
    int16_t char_num;
    int16_t horizontal_spacing;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Get_String_Width(string = %s)\n", __FILE__, __LINE__, string);
#endif

    pos = 0;
    width = 0;

    // MoM - ASM: horizontal_spacing
    // MoO2: horizontal_spacing = font_header.current_horizontal_spacing;
    // horizontal_spacing = font_style_data[72];
    horizontal_spacing = GET_2B_OFS(font_style_data,FONT_HDR_POS_CURRENT_HORIZONTAL_SPACING);
    
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: horizontal_spacing: %d\n", __FILE__, __LINE__, horizontal_spacing);
#endif


Next_Char:
    char_num = string[pos++];  // ~== LODSB
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: char_num: %d\n", __FILE__, __LINE__, char_num);
#endif
    char_num -= 32;

    // Non-Printable Character
    if(char_num < 0)
    {
        char_num += 32;
        if(char_num == 0)
        {
            goto Done;
        }
        else if(char_num == 13)
        {
            goto Done;
        }
        else if(char_num == 20)
        {
            goto Done;
        }
        else if(char_num == 21)
        {
            goto Done;
        }
        else if(char_num == 25)
        {
            goto Done;
        }
        else if(char_num == 29)
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: GET_1B_OFS(font_style_data,FONT_HDR_POS_CURRENT_FONT_WIDTHS + %d): %d\n", __FILE__, __LINE__, char_num, GET_1B_OFS(font_style_data,FONT_HDR_POS_CURRENT_FONT_WIDTHS + char_num));
#endif

    width = width + GET_1B_OFS(font_style_data,FONT_HDR_POS_CURRENT_FONT_WIDTHS + char_num);
    width = width + horizontal_spacing;  // MoO2:    ax, font_header.current_horizontal_spacing

goto Next_Char;


Done:
    width = width - horizontal_spacing;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Get_String_Width(string = %s) { width = %d }\n", __FILE__, __LINE__, string, width);
#endif

    return width;
}




/*
    WIZARDS.EXE  seg020
*/

// WZD s20p01
void Load_Palette(int entry, int start_color, int end_color)
{
    int color_start;
    int color_count;
    int itr;
    uint8_t font_color_block;
    uint8_t color_index;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Load_Palette(entry = %d, start_color = %d, end_color = %d)\n", __FILE__, __LINE__, entry, start_color, end_color);
#endif

    palette_data = LBX_Reload(font_name, entry+2, palette_block);
    
    // font_colors = (palette_data + (16 * (48)));  // 768
    font_colors = &palette_data[768];
#ifdef STU_DEBUG
    for(font_color_block = 0; font_color_block < 16; font_color_block++)
    {
        for(color_index = 0; color_index < 16; color_index++)
        {
            dbg_prn("DEBUG: [%s, %d]: font_colors[%d][%d]: 0x%02X\n", __FILE__, __LINE__, font_color_block, color_index, font_colors[(font_color_block * 16) + color_index]);
        }

    }
#endif

    // UU_gsa_Palette_Data = (palette_data + (16 * (48 + 16)));  // 400h

    mouse_palette = (palette_data + (16 * (48 + 16 + 16)));  // 1280

    // array of 24 color fractions (B-G-R-Percent)
    // used to set shaded replacement colors for each color index
    // gsa_ShadingColors = (palette_data + (16 * (48 + 16 + 16 + 256)));  // 5376


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

// WZD s20p05
void Set_Palette_Changes(int start_color, int end_color)
{
    int itr;

//     for(itr = start_color; itr < end_color; itr++)
//     {
//         current_palette[itr * 4] = ST_TRUE;
//     }

    for(itr = start_color; itr < end_color; itr++)
    {
        *(p_PaletteFlags + itr) = ST_TRUE;
    }

}

/*
    WIZARDS.EXE  seg021
*/

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
