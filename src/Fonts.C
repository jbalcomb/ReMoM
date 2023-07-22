
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
byte_ptr font_colors;  // 300h into the palette entry, 16 arrays of 16 colors


byte_ptr p_Palette;                             // MGC dseg:A7DE    alloc in Load_Font_File()
byte_ptr p_PaletteFlags;                        // MGC dseg:A7D6    alloc in Load_Font_File()
byte_ptr p_Palette_XBGR;                        // STU/Win32



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
    p_Palette_XBGR         = Allocate_Space(64);     // STU/Win32  4 bytes per pixel * 256 colors / 16 bytes per paragraph

    for(itr = 0; itr < 768; itr++)
    {
        *(p_Palette + itr) = 0;
    }
    for(itr = 0; itr < 256; itr++)
    {
        *(p_PaletteFlags + itr) = 1;
    }
    for(itr = 0; itr < 1024; itr++)
    {
        *(p_Palette_XBGR + itr) = 0;
    }

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
void Set_Font_Style4(int16_t Font_Index, int16_t Color_1, int16_t Color_2, int16_t Color_3)
{
    Set_Font(Font_Index, Color_1, Color_2, Color_3);
    font_header->shadow_flag = 4;  // full outline
}
// WZD s17p05
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
        font_colors[color_start + itr] = colors[itr];
    }

    Set_Font(font_idx, 15, 0, 0);

}


// WZD s17p12
void Set_Alias_Color(int16_t color)
{
    int16_t itr;

    for(itr = 0; itr < 16; itr++)
    {
        // WZD dseg:E828
        // gsa_Palette_Font_Colors dw 0
        // 300h into the palette entry, 16 arrays of 16 colors
        font_colors[itr * 16] = color;
    }

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

    string_len = Get_String_Width(string);

    next_x = Print((x - string_len - 1), y, string);

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
    int16_t next_x;
    uint16_t outline_style;

    // outline_style = farpeekb(font_style_data, FONT_HEADER_SHADOW_FLAG);
    outline_style = 0;  // ~None

    if(outline_style == 0)
    {
        draw_alias_flag = ST_FALSE;
        next_x = Print_String(x, y, string, ST_TRUE, full_flag);
    }

    return next_x;
}


// WZD s17p37
int16_t Print_String(int16_t x, int16_t y, char * string, int16_t change_color_ok_flag, int16_t full_flag)
{
    int16_t next_x;
//     char * ptr;
    uint8_t character;
    int16_t itr;

    print_xpos = x;
    print_ypos = y;

//     ptr = string;


    if(full_flag != ST_TRUE)
    {

    }

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] string: %s\n", __FILE__, __LINE__, string);
// #endif

//     // /* If the string doesn't exist or is empty, EOF found */
//     // if (c_str && *c_str) {
//     while(character = (unsigned char)*ptr++ != 0)
//     {
// 
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] character: %u\n", __FILE__, __LINE__, character);
// #endif
// 
//         Print_Character(print_xpos, print_ypos, character);
// 
//         // ...
//         // else
//         // Print_Character_No_Alias(print_xpos, print_ypos, character);
// 
//     }

    itr = 0;
    while(string[itr] != '\0')
    {
        // if(file_name[itr] == '.') { file_name[itr] = '\0'; }

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] string[itr]: %u\n", __FILE__, __LINE__, string[itr]);
// #endif

        character = string[itr];
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] character: %u\n", __FILE__, __LINE__, character);
// #endif

        print_xpos = Print_Character(print_xpos, print_ypos, character);

        itr++;
    }


    next_x = print_xpos;

    return next_x;
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
void Set_Font(int16_t font_idx, int16_t color1, int16_t color2, int16_t color3)
{
    int16_t itr;

    color1 = (color1 < 16) ? color1 : 0;
    color2 = (color2 < 16) ? color2 : 0;
    color3 = (color3 < 16) ? color3 : 0;

    Font_Index = font_idx;
    Font_ColorIndex1 = color1;
    Font_ColorIndex2 = color2;
    Font_ColorIndex3 = color3;

    for(itr = 0; itr < 16; itr++)
    {
        font_style_data[(0 + itr)] = font_colors[((color1 * 16) + itr)];
    }

    for(itr = 0; itr < 16; itr++)
    {
        font_style_data[(20 + itr)] = font_colors[((color1 * 16) + itr)];
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

        // _ES_DstSgmt = current_video_page + (y_start * 320);
        // _DI_DstOfst = 0;
        // _DS_SrcSgmt = font_style_data;
        // _SI_SrcOfst = font_header.current_data_offsets[char_num]


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
    return next_x;
}


// WZD s18p03
int16_t Print_Character_No_Alias(int16_t x, int16_t y, int16_t char_num)
{
    int16_t next_x;

    next_x = 0;

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


    // src: _DS_SI
    // mov     ax, [font_style_data]
    // mov     ds, ax

    //  = font_style_data[(170 + (char_num ))]
    // mov     si, [bx+s_FONT_HEADER.current_data_offsets]

    // dst:  _ES_DI
    // mov     ax, [bp+y]
    // mov     cx, ax
    // shl     ax, 1
    // shl     ax, 1
    // add     ax, cx
    // add     ax, [current_video_page]
    // mov     es, ax

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] current_video_page: %p\n", __FILE__, __LINE__, current_video_page);
// #endif

    screen_start = current_video_page + ((y_start * 320) + x_start);

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] screen_start: %p\n", __FILE__, __LINE__, screen_start);
// #endif


    screen_pos = screen_start;
    while(width)
    {
        
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] screen_pos: %p\n", __FILE__, __LINE__, screen_pos);
// #endif

        font_data_byte = *font_data_offset++;

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] font_data_byte: %02X  %u\n", __FILE__, __LINE__, font_data_byte, font_data_byte);
// #endif

        /* Type: next column */
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
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] skip_count: %u\n", __FILE__, __LINE__, skip_count);
// #endif
            while(skip_count--)
            {
                screen_pos += 320;
            }
        }

        if((font_data_byte & 0x80) == 0)
        {
            repeat_count = ((font_data_byte & 0xF0) >> 4);
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] repeat_count: %u\n", __FILE__, __LINE__, repeat_count);
// #endif
            while(repeat_count--)
            {
                color_index = (font_data_byte & 0x0F);
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] color_index: %u\n", __FILE__, __LINE__, color_index);
// #endif
                *screen_pos = font_header->current_colors[color_index];
                screen_pos += 320;
            }
        }
    }

}


// WZD s18p06
void Print_Character_No_Alias_ASM(int16_t x_start, int16_t y_start, int16_t width, byte_ptr font_data_offset)
{

}



// WZD s18p07
int16_t Get_String_Width(char * string)
{
    int16_t pos;
    int16_t width;
    int16_t char_num;
    int16_t horizontal_spacing;

    pos = 0;
    width = 0;
    // horizontal_spacing = font_header.current_horizontal_spacing;
    horizontal_spacing = font_style_data[72];


Next_Char:
    char_num = string[pos];

    if(char_num > 126)
    {
        goto Next_Char;
    }
    if(char_num < 32)
    {
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
    /*
        HERE:
            32 <= char_num <= 126
    */
    // mov al, [es:bx+s_FONT_HEADER.Glyph_Widths]  ; font_header.current_font_widths[char_num]
    width = width + font_style_data[74 + char_num];
    width = width + horizontal_spacing;


Done:
    width = width - horizontal_spacing;

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

    palette_data = LBX_Reload(font_name, entry+2, palette_block);

    // gsa_Palette_Font_Colors
    // font_colors = (palette_data + (16 * (48)));  // 768
    font_colors = &palette_data[768];

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

    for(itr = 0; itr < color_count; itr++)
    {
        *(p_Palette_XBGR + (color_start * 4) + (itr * 4) + 3) = 0x00;
        *(p_Palette_XBGR + (color_start * 4) + (itr * 4) + 2) = (*(palette_data + (color_start * 3) + (itr * 3) + 0) << 2);
        *(p_Palette_XBGR + (color_start * 4) + (itr * 4) + 1) = (*(palette_data + (color_start * 3) + (itr * 3) + 1) << 2);
        *(p_Palette_XBGR + (color_start * 4) + (itr * 4) + 0) = (*(palette_data + (color_start * 3) + (itr * 3) + 2) << 2);
    }

    Set_Font(0, 0, 0, 0);

    if(start_color == ST_UNDEFINED)
    {
        Set_Palette_Changes(0, 255);
    }
    else
    {
        Set_Palette_Changes(start_color, end_color);
    }

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
void Apply_Palette(void)
{

    int16_t itr;

    for(itr = 0; itr < 256; itr++)
    {
        if(*(p_Palette + 768 + itr) == 1)
        {
            *(g_Palette + itr) = *(p_Palette + itr);

            *(g_Palette_XBGR + (itr * 4) + 3) = 0x00;
            *(g_Palette_XBGR + (itr * 4) + 2) = (*(p_Palette + (itr * 3) + 0) << 2);
            *(g_Palette_XBGR + (itr * 4) + 1) = (*(p_Palette + (itr * 3) + 1) << 2);
            *(g_Palette_XBGR + (itr * 4) + 0) = (*(p_Palette + (itr * 3) + 2) << 2);
        }

    }

}
