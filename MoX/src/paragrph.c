/*
    WIZARDS.EXE
        seg019

    MoO2
        paragrph
*/

#include "MOX_BASE.h"
#include "MOX_BITS.h"
#include "MOX_TYPE.h"



#include "Fields.h"  /* Get_Field_Counter() */
#include "Fonts.h"
#include "paragrph.h"



// dseg:783C                                                 seg017  MoO2 Module: fonts
// dseg:783C 00 00                                           outline_color dw 0                      ; DATA XREF: Set_Outline_Color+6^w ...
// dseg:783E 00 00                                           draw_alias_flag dw 0                    ; DATA XREF: Print_Display+4C^w ...
// dseg:7840 00 00                                           UU_VGA_TextDraw_Enabled dw 0            ; DATA XREF: VGA_TextDraw_Init+4^w

// dseg:7842
// drake178: VGA_FltBlock_Count
int16_t exclusion_count = 0;

// dseg:7844                                                 seg020  MoO2 Module: palette
// dseg:7844 FF FF                                           cycle_direction_flag dw 0FFFFh          ; DATA XREF: Reset_Cycle_Palette_Color+3^w ...



// WZD dseg:E82A                                                 BEGIN: paragrph

// WZD dseg:E82A
// drake178:  points to an array of starting string indexes for drawing lines of text (128 bytes, LBX_Alloc_Space)
// drake178: VGA_TextLine_Starts
int16_t paragraph_line_offset[64];

// WZD dseg:E82E
// drake178: points to an array of starting Y positions for drawing lines of text (128 bytes, LBX_Alloc_Space)
// drake178: VGA_TextLine_Tops
int16_t paragraph_line_y_start[64];

// WZD dseg:E832
// drake178: points to an array of ending X positions for drawing lines of text (128 bytes, LBX_Alloc_Space)
// drake178: VGA_TextLine_Rights
int16_t paragraph_line_x_end[64];

// WZD dseg:E836
// drake178: VGA_TextLine_Lefts
// drake178: points to an array of starting X positions for drawing lines of text (128 bytes, LBX_Alloc_Space)
int16_t paragraph_line_x_start[64];

// WZD dseg:E83A
// drake178: VGA_TextDraw_Lines;
// drake178: the amount of elements in the VGA_TextLine_ arrays
int16_t paragraph_max_lines;

// WZD dseg:E83C
// drake178: VGA_FltBlock_Bottoms
// drake178: up to 4 bottom row positions used to reserve space where floating text won't be drawn
int16_t exclusion_area_y2[4];

// WZD dseg:E844
// drake178: VGA_FltBlock_Rights
// drake178: up to 4 right column positions used to reserve space where floating text won't be drawn
int16_t exclusion_area_x2[4];

// WZD dseg:E84C
// drake178: VGA_FltBlock_Tops
// drake178: up to 4 top row positions used to reserve space where floating text won't be drawn
int16_t exclusion_area_y1[4];

// WZD dseg:E854 00 00 00 00 00 00 00 00                         
// drake178: VGA_FltBlock_Lefts
// drake178: up to 4 left column positions used to reserve space where floating text won't be drawn
int16_t exclusion_area_x1[4];


/*
MoO2


VGA_TextDraw_Lines  ~==  paragraph_max_lines


dseg02:0017C9EC word_17C9EC         dw 0                                                                            ; DATA XREF: Set_Paragraph_Exclusion_Area+21^r ...
dseg02:0017C9EE exclusion_count     dw 0                                                                            ; DATA XREF: Set_Line_Limits_:loc_106B54^r ...

dseg02:001B8AF4 exclusion_area_x1   dw 4 dup(     ?)                                                                ; 0
dseg02:001B8AF4                                                                                                     ; DATA XREF: Set_Line_Limits_+C2^r ...
dseg02:001B8AFC exclusion_area_x2   dw 4 dup(     ?)                                                                ; 0
dseg02:001B8AFC                                                                                                     ; DATA XREF: Set_Line_Limits_+C9^r ...
dseg02:001B8B04 exclusion_area_y1   dw 4 dup(     ?)                                                                ; 0
dseg02:001B8B04                                                                                                     ; DATA XREF: Set_Line_Limits_+A1^r ...
dseg02:001B8B0C exclusion_area_y2   dw 4 dup(     ?)                                                                ; 0
dseg02:001B8B0C                                                                                                     ; DATA XREF: Set_Line_Limits_+B5^r ...
dseg02:001B8B14 paragraph_line_y_start dw 64h dup(     ?)                                                              ; 0
dseg02:001B8B14                                                                                                     ; DATA XREF: Mark_Paragraph+2B0^w ...
dseg02:001B8BDC paragraph_line_x_start dw 64h dup(     ?)                                                              ; 0
dseg02:001B8BDC                                                                                                     ; DATA XREF: Mark_Paragraph+286^w ...
dseg02:001B8CA4 paragraph_line_offset dw 64h dup(     ?)                                                              ; 0
dseg02:001B8CA4                                                                                                     ; DATA XREF: Mark_Paragraph+2C5^w ...
dseg02:001B8D6C paragraph_line_x_end dw 64h dup(     ?)                                                              ; 0
dseg02:001B8D6C                                                                                                     ; DATA XREF: Mark_Paragraph+29B^w ...
dseg02:001B8E34 paragraph_max_lines dw ?                                                                            ; DATA XREF: Print_Paragraph+3F^r ...
dseg02:001B8E36                     align 4
*/



/*
    WIZARDS.EXE  seg019
*/

// WZD s19p01
// drake178: VGA_DrawText()
/*

    print_type
        0: Print()
        1: Print_Right()
        2: Print_Centered()
        3: Print_Full()
*/
void Print_Paragraph(int16_t x, int16_t y, int16_t max_width, char * string, int16_t print_type)
{
    int16_t itr;

    Mark_Paragraph(x, y, max_width, string);

    for(itr = 0; itr < paragraph_max_lines; itr++)
    {

        switch(print_type)
        {

            case 0:  // ¿ Align-Left ?
            {
                Print(paragraph_line_x_start[itr], paragraph_line_y_start[itr], &string[paragraph_line_offset[itr]]);
            } break;

            case 1:  // ¿ Align-Right ?
            {
                // Print_Right(paragraph_line_x_start[itr], paragraph_line_y_start[itr], &string[paragraph_line_offset[itr]], max_width);
                Print_Centered(paragraph_line_x_start[itr], paragraph_line_y_start[itr], &string[paragraph_line_offset[itr]]);
            } break;

            case 2:  // ¿ Center ?
            {
                Print_Centered(((paragraph_line_x_start[itr] + paragraph_line_x_end[itr]) / 2), paragraph_line_y_start[itr], &string[paragraph_line_offset[itr]]
                );
            } break;
            
            case 3:  // ¿ Justify ?
            {

                if((paragraph_max_lines - 1) == itr)  /* last line */
                {

                    Print(
                        paragraph_line_x_start[itr],
                        paragraph_line_y_start[itr],
                        &string[paragraph_line_offset[itr]]
                    );

                }
                else
                {

                    Print_Full(
                        paragraph_line_x_start[itr],
                        paragraph_line_y_start[itr],
                        &string[paragraph_line_offset[itr]],
                        (paragraph_line_x_end[itr] - paragraph_line_x_start[itr])
                    );

                }

            } break;

        }

    }

    Remove_Paragraph_Marks(string);

    exclusion_count = 0;

}

// WZD s19p02
// drake178: UU_VGA_WndDrawText()
/*
NIU  nothing in WIZARDS.EXE calls it
Mark_Paragraph() string into width at (x, y), then prints every line left-aligned (align 0), right-aligned (1) or centered (2)
*/
void Print_Paragraph_Aligned__NIU(int16_t x, int16_t y, int16_t width, char * string, int16_t align)
{
    int16_t itr = 0;  // _SI_

    Mark_Paragraph(x, y, width, string);

    for(itr = 0; itr < paragraph_max_lines; itr++)
    {

        switch(align)
        {

            case 0:
            {

                Clipped_Print(paragraph_line_x_start[itr], paragraph_line_y_start[itr], &string[paragraph_line_offset[itr]]);

            } break;

            case 1:
            {

                Clipped_Print_Right(paragraph_line_x_end[itr], paragraph_line_y_start[itr], &string[paragraph_line_offset[itr]]);  /* NOTE  the asm also pushes width (add sp, 8); Clipped_Print_Right() reads only x, y, string */

            } break;

            case 2:
            {

                Clipped_Print_Centered(((paragraph_line_x_start[itr] + paragraph_line_x_end[itr]) / 2), paragraph_line_y_start[itr], &string[paragraph_line_offset[itr]]);  /* NOTE  the asm also pushes width (add sp, 8); Clipped_Print_Centered() reads only x, y, string */

            } break;

        }

    }

    Remove_Paragraph_Marks(string);

    exclusion_count = 0;

}


// WZD s19p03
// drake178: UU_GUI_CreateHelpLinks()
/*
NIU  nothing in WIZARDS.EXE calls it
Mark_Paragraph() string into width at (x, y) and adds a field for each link in it - the text between a pair of char 4 marks - that sets *variable to the link's number (1, 2, ...)
    align 0 left, 1 right, 2 centered, 3 justified (the last line left-aligned): the link x positions follow the same layout
*/
void Add_Paragraph_Link_Fields__NIU(int16_t x, int16_t y, int16_t width, char * string, int16_t align, int16_t * variable, char * hotkey, int16_t help)
{
    int16_t link_x2[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t link_x1[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    uint8_t * font_widths = 0;
    int16_t line_string_width = 0;
    int16_t space_extension = 0;
    int16_t line_space_count = 0;
    int16_t spread_pixels = 0;
    int16_t spaces_drawn = 0;
    int16_t link_on = 0;
    int16_t link_count = 0;
    int16_t link_number = 0;
    int16_t first_field_idx = 0;
    int16_t line_char = 0;
    int16_t justify_width = 0;
    int16_t line_char_idx = 0;
    int16_t line_x = 0;
    int16_t font_spacing = 0;
    int16_t font_height = 0;
    int16_t itr_chars = 0;  // _SI_
    int16_t itr_lines = 0;  // _DI_

    font_widths = (uint8_t *)(font_style_data + FONT_HDR_POS_CURRENT_FONT_WIDTHS);

    font_height = GET_1B_OFS(font_style_data, FONT_HDR_POS_HEIGHT);  /* asm: farpeekb */

    font_spacing = GET_1B_OFS(font_style_data, FONT_HDR_POS_CURRENT_HORIZONTAL_SPACING);  /* asm: farpeekb */

    first_field_idx = Get_Field_Counter();

    link_number = 1;

    Mark_Paragraph(x, y, width, string);

    justify_width = 0;

    for(itr_lines = 0; itr_lines < paragraph_max_lines; itr_lines++)
    {

        line_string_width = Get_String_Width(&string[paragraph_line_offset[itr_lines]]);

        if(
            ((paragraph_max_lines - 1) == itr_lines)
            &&
            (align == 3)
        )
        {

            align = 0;

        }

        switch(align)
        {

            case 0:
            {

                line_x = paragraph_line_x_start[itr_lines];

            } break;

            case 1:
            {

                line_x = (paragraph_line_x_end[itr_lines] - line_string_width);

            } break;

            case 2:
            {

                line_x = (((paragraph_line_x_start[itr_lines] + paragraph_line_x_end[itr_lines]) / 2) - (line_string_width / 2));

            } break;

            case 3:
            {

                line_x = paragraph_line_x_start[itr_lines];

                justify_width = (paragraph_line_x_end[itr_lines] - line_x);

                if(justify_width < 0)
                {

                    justify_width = 0;

                }

                if(justify_width != 0)
                {

                    spaces_drawn = 0;

                    spread_pixels = 0;

                    itr_chars = paragraph_line_offset[itr_lines];

                    line_space_count = 0;

                    /* OGBUG  the last test compares the INDEX with 29 (asm: cmp si, 1Dh), not string[itr_chars] - every other end-of-line code is tested on the character */
                    while(
                        (string[itr_chars] != 0)
                        &&
                        (string[itr_chars] != 13)
                        &&
                        (string[itr_chars] != 20)
                        &&
                        (string[itr_chars] != 25)
                        &&
                        (string[itr_chars] != 21)
                        &&
                        (itr_chars != 29)
                    )
                    {

                        if(string[itr_chars] == ' ')
                        {

                            line_space_count++;

                        }

                        itr_chars++;

                    }

                    if(line_space_count == 0)
                    {

                        justify_width = 0;

                    }

                    justify_width -= line_string_width;

                    if(justify_width <= 0)
                    {

                        justify_width = 0;

                    }
                    else
                    {

                        spread_pixels = (justify_width % line_space_count);

                        space_extension = (justify_width / line_space_count);

                    }

                }

            } break;

        }

        line_char_idx = paragraph_line_offset[itr_lines];

        link_count = 0;

        link_on = ST_FALSE;

        while(1)
        {

            line_char = string[line_char_idx];  /* asm: mov al, [bx] / cbw - a signed char */

            /* end-of-line codes: 0, 21, 13, 20, 25, 29 */
            if(
                (line_char == 0)
                ||
                (line_char == 21)
                ||
                (line_char == 13)
                ||
                (line_char == 20)
                ||
                (line_char == 25)
                ||
                (line_char == 29)
            )
            {

                break;

            }

            if(line_char == 4)
            {

                if(link_on == ST_FALSE)
                {

                    link_on = ST_TRUE;

                    link_x1[link_count] = line_x;

                }
                else
                {

                    link_on = ST_FALSE;

                    link_x2[link_count] = line_x;

                    link_count++;

                }

            }

            if(line_char >= ' ')
            {

                line_x += (font_widths[(line_char - ' ')] + font_spacing);

                if(
                    (justify_width != 0)
                    &&
                    (line_char == ' ')
                )
                {

                    line_x += space_extension;

                    if(spaces_drawn < spread_pixels)
                    {

                        line_x++;

                    }

                    spaces_drawn++;

                }

            }

            line_char_idx++;

        }

        for(itr_chars = 0; itr_chars < link_count; itr_chars++)
        {

            Add_Click_Link_Field__NIU(first_field_idx, variable, link_number, (link_x1[itr_chars] - 1), (paragraph_line_y_start[itr_lines] - 1), link_x2[itr_chars], (paragraph_line_y_start[itr_lines] + font_height), hotkey, help);

            link_number++;

        }

    }

    Remove_Paragraph_Marks(string);

}


// WZD s19p04
// drake178: VGA_TextSetup()
/*


    initialized and never changed:
        x
        y
        full_width
        string
        font_height
        spacing
        line_feed

    string_widths is just a pointer to the current font widths

*/
void Mark_Paragraph(int16_t x, int16_t y, int16_t full_width, char * string)
{
    uint8_t * string_widths;
    int16_t x_override;
    int16_t Line_Width;
    int16_t DrawPos_Override;
    char character;
    int16_t blank_pos;
    int16_t skip_flag;
    int16_t xmax;
    int16_t ypos;
    int16_t xmin;
    int16_t tab;
    int16_t line_feed;
    int16_t spacing;
    int16_t font_height;
    int16_t str_ptr;
    int16_t itr_exclusion_count;
    int16_t temp_ptr;

    str_ptr = 0;
    paragraph_max_lines = 0; // ; the amount of elements in the VGA_TextLine_ arrays
    x_override = -1;
    string_widths = (uint8_t *)(font_style_data + FONT_HDR_POS_CURRENT_FONT_WIDTHS);
    // font_height = GET_2B_OFS(font_style_data, FONT_HDR_POS_HEIGHT);
    // spacing = GET_2B_OFS(font_style_data, FONT_HDR_POS_CURRENT_HORIZONTAL_SPACING);
    // line_feed = GET_2B_OFS(font_style_data, FONT_HDR_POS_CURRENT_BASE_HEIGHT);  // MoO2  font_header.current_line_feed ¿ added after MoM ?
    font_height = GET_1B_OFS(font_style_data, FONT_HDR_POS_HEIGHT);  /* CLAUDE  asm seg019 Mark_Paragraph reads all three with farpeekb (byte, zero-extended) */
    spacing = GET_1B_OFS(font_style_data, FONT_HDR_POS_CURRENT_HORIZONTAL_SPACING);  /* CLAUDE */
    line_feed = GET_1B_OFS(font_style_data, FONT_HDR_POS_CURRENT_BASE_HEIGHT);  /* CLAUDE */  // MoO2  font_header.current_line_feed ¿ added after MoM ?
    ypos = y;

    xmin = 0;  // DNE in Dasm
    xmax = 0;  // DNE in Dasm
    Line_Width = 0;  // DNE in Dasm



    while(string[str_ptr] != '\0')
    {

        skip_flag = 0;

        // if(x_override != -1)
        // {
        //     xmin = x_override;
        //     xmax = x + full_width - 1;
        //     x_override = -1;
        // }
        // else
        // {
        //     xmin = x;
        //     xmax = x + full_width;
        // }
        /* CLAUDE  asm: `cmp [bp+x_override], -1; jnz` - the -1 branch comes first, and its xmax is `x + full_width` then `dec ax`, too */
        if(x_override == -1)
        {
            xmin = x;
            xmax = x + full_width - 1;
        }
        else
        {
            xmin = x_override;
            xmax = x + full_width - 1;
            x_override = -1;
        }



        /*
            BEGIN:  Line Limits / Exclusion Areas
        */
        {
            for(itr_exclusion_count = 0; (itr_exclusion_count < exclusion_count) && (skip_flag == ST_FALSE); itr_exclusion_count++)
            {
                if(
                    (
                        (exclusion_area_y1[itr_exclusion_count] <= ypos)
                        &&
                        (exclusion_area_y2[itr_exclusion_count] >= ypos)
                    )
                    ||
                    (
                        (exclusion_area_y1[itr_exclusion_count] <= (ypos + line_feed))
                        &&
                        (exclusion_area_y2[itr_exclusion_count] >= (ypos + font_height))
                    )
                )
                {
                    if(
                        (exclusion_area_x2[itr_exclusion_count] > xmin)
                        &&
                        (exclusion_area_x1[itr_exclusion_count] < xmax)
                    )
                    {
                        if(exclusion_area_x1[itr_exclusion_count] > xmin)
                        {
                            xmax = (exclusion_area_x1[itr_exclusion_count] - 1);
                        }
                        else
                        {
                            xmin = (exclusion_area_x2[itr_exclusion_count] + 1);
                        }
                        // if(xmin > xmax)
                        // {
                        //     skip_flag = ST_TRUE;
                        // }
                    }
                    /* CLAUDE  asm @@CheckXminGteXmax: reached by every exclusion whose y range matches, x overlap or not, and `cmp xmin, xmax; jl` skips only while xmin < xmax */
                    if(xmin >= xmax)
                    {
                        skip_flag = ST_TRUE;
                    }
                }
            }
        }
        /*
            END:  Line Limits / Exclusion Areas
        */



        if(skip_flag == ST_FALSE)
        {

            if((ypos + font_height) >= SCREEN_HEIGHT)
            {

                goto Error;

            }

            Line_Width = xmax - xmin + 1;  // we just didn't skip, so (xmin <= xmax)

            blank_pos = -1;

            tab = -1;

            temp_ptr = str_ptr;

            while( -(spacing) <= Line_Width )
            {

                character = string[temp_ptr];

                // if(character < 0x20)  /* LT ASCII SPACE ∴ Non-Printable ∴ Control Character */
                // {
                //
                //     if(
                //         character == 0x14   /*  ASCII  14h  20d  DC4 (device control 4)      */
                //         ||
                //         character == 0x0D   /*  ASCII  0Dh  13d  CR  (carriage return)       */
                //         ||
                //         character == 0x00   /*  ASCII  00h  00d  NUL (null)                  */
                //         ||
                //         character == 0x19   /*  ASCII  15h  21d  NAK (negative acknowledge)  */
                //         ||
                //         character == 0x1D   /*  ASCII  1Dh  29d  GS  (group separator)       */
                //     )
                //     {
                //         Line_Width = (-(spacing) - 1);  // ¿ this makes it take the other branch at the top of the loop ?
                //         tab = character;
                //     }
                // }
                // else
                // {
                //
                //     Line_Width -= string_widths[(character - 32)] + spacing;
                //
                //     if(character == 0x20)
                //     {
                //         blank_pos = temp_ptr;
                //     }
                // }
                /* CLAUDE  asm: `cmp [bp+character], 20h; jl` - the printable branch comes first */
                if(character >= 0x20)
                {

                    Line_Width -= string_widths[(character - 32)] + spacing;

                    if(character == 0x20)
                    {
                        blank_pos = temp_ptr;
                    }
                }
                else  /* LT ASCII SPACE ∴ Non-Printable ∴ Control Character */
                {

                    if(
                        character == 0x14   /*  ASCII  14h  20d  DC4 (device control 4)      */
                        ||
                        character == 0x0D   /*  ASCII  0Dh  13d  CR  (carriage return)       */
                        ||
                        character == 0x00   /*  ASCII  00h  00d  NUL (null)                  */
                        ||
                        character == 0x19   /*  ASCII  19h  25d  EM  (end of medium)         */
                        ||
                        character == 0x1D   /*  ASCII  1Dh  29d  GS  (group separator)       */
                    )
                    {
                        Line_Width = (-(spacing) - 1);  // ¿ this makes it take the other branch at the top of the loop ?
                        tab = character;
                    }
                }
                
                temp_ptr++;
            }  /* while( -(spacing) <= Line_Width ) */



            if(tab == 0)
            {
                temp_ptr -= 1;
            }
        
            if(tab != -1)  /* the current character in the string IS NOT a control-character */
            {
                paragraph_line_x_start[paragraph_max_lines] = xmin;
                paragraph_line_x_end[paragraph_max_lines] = xmax;
                paragraph_line_y_start[paragraph_max_lines] = ypos;
                paragraph_line_offset[paragraph_max_lines] = str_ptr;
                paragraph_max_lines++;

                if(tab == 0x19)  /*  ASCII  19h  25d  EM (end of medium)          */
                {
                    // HERE: the *character* in the string is the byte after the control-character that we just previously matched and set up for process
                    // So, WTF with all this math?
                    // {0, ..., 255} - 48 = {-48, ..., 207}
                    // {-48, ..., 207} * 11 = {-528, ..., 2277}
                    // 48 - 48 = 0 * 11 = 0
                    // 49 - 48 = 1 * 11 = 11
                    // 55 - 48 = 7 * 11 = 77
                    // 71 - 48 = 23 * 11 = 253
                    // 72 - 48 = 24 * 11 = 264
                    // ...
                    // IDGI
                    /* OGBUG  both digits are read from string[temp_ptr]: the first digit is used twice ("42" gives 44, always 11 x the first digit), the second digit is never read */
                    DrawPos_Override =  ((string[temp_ptr] + -48) * 10);
                    DrawPos_Override +=  (string[temp_ptr] + -48);

                    // drake178: ; entirely independent of the current draw locations!
                    // drake178: ; negate the new line that follows
                    ypos = DrawPos_Override - line_feed;

                    temp_ptr += 2;
                }
                if(tab == 0x1D)  /*  ASCII  1Dh  29d  GS (group separator)        */
                {
                    /* OGBUG  same as the 19h code: the first digit is read twice */
                    DrawPos_Override =  ((string[temp_ptr] + -48) * 10);
                    DrawPos_Override +=  (string[temp_ptr] + -48);

                    ypos -= line_feed; // drake178: ; negate the new line that follows

                    x_override = DrawPos_Override;  // drake178: ; entirely independent of the current draw locations!

                    temp_ptr += 2;
                }

                str_ptr = temp_ptr;
            }
            else  /* if(tab != -1) */
            {
                paragraph_line_x_start[paragraph_max_lines] = xmin;
                paragraph_line_x_end[paragraph_max_lines] = xmax;
                paragraph_line_y_start[paragraph_max_lines] = ypos;
                paragraph_line_offset[paragraph_max_lines] = str_ptr;
                paragraph_max_lines++;

                // BTB, Remove_Paragraph_Marks() goes back through the string and replaces these with  ASCII SPACE  0x20  32
                if(blank_pos != -1)
                {
                    string[blank_pos] = 0x15;  /*  ASCII  19h  25d  EM (end of medium)          */
                    str_ptr = blank_pos + 1;
                }

            }

        }  /* if(skip_flag == ST_FALSE) */

        // @@StringLoopTail
        if( (x + full_width - 1) <= xmax)
        {
            ypos += line_feed;
        }
        else
        {
            x_override = (xmax + 1);
        }

    }  /* while(string[str_ptr] != '\0') */


goto Done;


Error:
    paragraph_max_lines = 0;
    goto Done;


Done:

    return;  // DNE in Dasm - VSBS
}

// WZD s19p05
// MoO2  Module: paragrph  Get_Paragraph_Max_Height()
int16_t Get_Paragraph_Max_Height(int16_t max_width, char * string)
{
    int16_t ymax;
    int16_t font_height;
    font_height = GET_1B_OFS(font_style_data, FONT_HDR_POS_HEIGHT);
    Mark_Paragraph(0, 0, max_width, string);
    if(paragraph_max_lines >= 1)
    {
        ymax = (paragraph_line_y_start[(paragraph_max_lines - 1)] + font_height);
        Remove_Paragraph_Marks(string);
    }
    else
    {
        ymax = 0;
    }
    return ymax;
}


// WZD s19p06
// drake178: VGA_GetTextWidth
// EXACT  MoO2  Module: paragrph  Get_Paragraph_Max_Width()
int16_t Get_Paragraph_Max_Width(int16_t max_width, char * string, int16_t print_type)
{
    int16_t x2max = 0;
    int16_t x1min = 0;
    int16_t width = 0;
    int16_t xc = 0;
    int16_t x2 = 0;
    int16_t xmax = 0;
    int16_t itr = 0;  // _SI_
    int16_t x1 = 0;  // _DI_

    Mark_Paragraph(0, 0, max_width, string);

    x1min = 1000;
    x2max = 0;

    for(itr = 0; itr < paragraph_max_lines; itr++)
    {
        width = Get_String_Width(&string[paragraph_line_offset[itr]]);

        switch(print_type)
        {
            case 0:  // ¿ Left-Align ?
            {
                x1 = paragraph_line_x_start[itr];
                x2 = (x1 + width);
            } break;
            case 1:  // ¿ Right-Align ?
            {
                x2 = paragraph_line_x_end[itr];
                x1 = (x2 - width);
            } break;
            case 2:
            {
                xc = ((paragraph_line_x_end[itr] - paragraph_line_x_start[itr]) / 2);
                x1 = (xc - (width / 2));
                x2 = (xc + (width / 2));
            } break;
            case 3:
            {
                x1 = paragraph_line_x_start[itr];
                x2 = paragraph_line_x_end[itr];
            } break;
        }

        if(x1 < x1min)
        {
            x1min = x1;
        }
        if(x2 > x2max)
        {
            x2max = x2;
        }

    }

    Remove_Paragraph_Marks(string);

    xmax = ((x2max - x1min) + 1);

    return xmax;
}


// WZD s19p07
// drake178: VGA_ResetFltBlocks()
// drake178: resets the floating block allocation, text will now be drawn without this limitation
void Reset_Paragraph_Exclusions(void)
{
    exclusion_count = 0;
}


// WZD s19p08
// drake178: VGA_AddFloatingBlock()
// MoO2: Set_Paragraph_Exclusion_Area()
void Set_Paragraph_Exclusion_Area(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
    if(exclusion_count < 4)
    {
        exclusion_area_x1[exclusion_count] = x1;
        exclusion_area_y1[exclusion_count] = y1;
        exclusion_area_x2[exclusion_count] = x2;
        exclusion_area_y2[exclusion_count] = y2;
        
        exclusion_count++;
    }
}

// WZD s19p09
// drake178: UU_VGA_AddFltIMGSpace()
// MoO2: Set_Paragraph_Exclusion_Picture()

// WZD s19p10
// drake178: STR_RemoveLBSPs
void Remove_Paragraph_Marks(char * string)
{
    int16_t itr;

    itr = 0;
    while(string[itr] != '\0')
    {
        if(string[itr] == 0x15)  /*  ASCII  19h  25d  EM (end of medium)          */
        {
            string[itr] = 0x20;  /*  ASCII  20h  32d  SPACE                       */
        }
        itr++;
    }

}


// WZD s19p11
// Get_Font_Height()

// WZD s19p12
// UU_VGA_WidestGlyph()

// WZD s19p13
// drake178: LBX_DrawText()
/*
; draws a paragraph of text into an LBX image
; allocation, then clears any rectangles reserved for
; text to float around
; Align can be one of the following:
;   0 - left aligned (default)
;   1 - right aligned (BUGGED)
;   2 - centered (BUGGED)
;   3 - justified (last line is left-aligned)
;
; WARNING: no text will be drawn if the last line
; would go below the bottom of the screen
*/
void Print_Paragraph_To_Bitmap(int16_t x, int16_t y, int16_t max_width, char * string, int16_t print_type, SAMB_ptr bitmap)
{
    int16_t itr;  // _SI_

    Mark_Paragraph(x, y, max_width, string);

    for(itr = 0; itr < paragraph_max_lines; itr++)
    {

        switch(print_type)
        {

            case 0:  // ¿ Align-Left ?
            {
                Print_To_Bitmap(paragraph_line_x_start[itr], paragraph_line_y_start[itr], &string[paragraph_line_offset[itr]], bitmap);
            } break;

            case 1:  // ¿ Align-Right ?
            {
                // DEDU  Print_Right_To_Bitmap(paragraph_line_x_start[itr], paragraph_line_y_start[itr], &string[paragraph_line_offset[itr]], max_width, bitmap);
                // Print_Right_To_Bitmap(paragraph_line_x_start[itr], paragraph_line_y_start[itr], &string[paragraph_line_offset[itr]], bitmap);
                /* OGBUG  the asm pushes five words (x, y, text, max_width, bitmap; `add sp, 0Ah`) to the four-argument Print_Right_To_Bitmap(), which takes max_width as its bitmap segment
                   and writes the glyphs into that low-memory segment; never reached in the game (both callers pass print_type 0) */
                Print_Right_To_Bitmap(paragraph_line_x_end[itr], paragraph_line_y_start[itr], &string[paragraph_line_offset[itr]], bitmap);  /* CLAUDE  asm: x is paragraph_line_x_end[itr] */  /* HACK  passes the real bitmap; the OG's bitmap segment is max_width */
            } break;

            case 2:  // ¿ Center ?
            {
                // DEDU  Print_Centered_To_Bitmap(paragraph_line_x_start[itr], paragraph_line_y_start[itr], &string[paragraph_line_offset[itr]], max_width, bitmap);
                // Print_Centered_To_Bitmap(paragraph_line_x_start[itr], paragraph_line_y_start[itr], &string[paragraph_line_offset[itr]], bitmap);
                /* OGBUG  same as print_type 1: five words pushed to the four-argument Print_Centered_To_Bitmap(), so max_width is used as the bitmap segment */
                Print_Centered_To_Bitmap(((paragraph_line_x_start[itr] + paragraph_line_x_end[itr]) / 2), paragraph_line_y_start[itr], &string[paragraph_line_offset[itr]], bitmap);  /* CLAUDE  asm: x is (x_start + x_end) / 2 */  /* HACK  passes the real bitmap; the OG's bitmap segment is max_width */
            } break;
            
            case 3:  // ¿ Justify ?
            {
                // if((paragraph_max_lines - 1) == itr)  /* last line */
                // {
                //     Print_To_Bitmap(paragraph_line_x_start[itr], paragraph_line_y_start[itr], &string[paragraph_line_offset[itr]], bitmap);
                // }
                // else
                // {
                //     Print_Full_To_Bitmap(paragraph_line_x_start[itr], paragraph_line_y_start[itr], &string[paragraph_line_offset[itr]], (paragraph_line_x_end[itr] - paragraph_line_x_start[itr]), bitmap);
                // }
                /* CLAUDE  asm: `cmp ax, si; jz` to the last-line Print_To_Bitmap - the Print_Full_To_Bitmap branch comes first */
                if((paragraph_max_lines - 1) != itr)
                {
                    Print_Full_To_Bitmap(paragraph_line_x_start[itr], paragraph_line_y_start[itr], &string[paragraph_line_offset[itr]], (paragraph_line_x_end[itr] - paragraph_line_x_start[itr]), bitmap);
                }
                else  /* last line */
                {
                    Print_To_Bitmap(paragraph_line_x_start[itr], paragraph_line_y_start[itr], &string[paragraph_line_offset[itr]], bitmap);
                }

            } break;

        }

    }

    Remove_Paragraph_Marks(string);

    exclusion_count = 0;

}

// WZD s19p14
// VGA_TextDraw_Init()
