/*
    WIZARDS.EXE
        seg019

    MoO2
        paragrph
*/

#include "MoX.H"



// dseg:783C                                                 seg017  MoO2 Module: fonts
// dseg:783C 00 00                                           outline_color dw 0                      ; DATA XREF: Set_Outline_Color+6w ...
// dseg:783E 00 00                                           draw_alias_flag dw 0                    ; DATA XREF: Print_Display+4Cw ...
// dseg:7840 00 00                                           UU_VGA_TextDraw_Enabled dw 0            ; DATA XREF: VGA_TextDraw_Init+4w

// dseg:7842
// drake178: VGA_FltBlock_Count
int16_t exclusion_count = 0;

// dseg:7844                                                 seg020  MoO2 Module: palette
// dseg:7844 FF FF                                           cycle_direction_flag dw 0FFFFh          ; DATA XREF: Reset_Cycle_Palette_Color+3w ...



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


dseg02:0017C9EC word_17C9EC         dw 0                                                                            ; DATA XREF: Set_Paragraph_Exclusion_Area+21r ...
dseg02:0017C9EE exclusion_count     dw 0                                                                            ; DATA XREF: Set_Line_Limits_:loc_106B54r ...

dseg02:001B8AF4 exclusion_area_x1   dw 4 dup(     ?)                                                                ; 0
dseg02:001B8AF4                                                                                                     ; DATA XREF: Set_Line_Limits_+C2r ...
dseg02:001B8AFC exclusion_area_x2   dw 4 dup(     ?)                                                                ; 0
dseg02:001B8AFC                                                                                                     ; DATA XREF: Set_Line_Limits_+C9r ...
dseg02:001B8B04 exclusion_area_y1   dw 4 dup(     ?)                                                                ; 0
dseg02:001B8B04                                                                                                     ; DATA XREF: Set_Line_Limits_+A1r ...
dseg02:001B8B0C exclusion_area_y2   dw 4 dup(     ?)                                                                ; 0
dseg02:001B8B0C                                                                                                     ; DATA XREF: Set_Line_Limits_+B5r ...
dseg02:001B8B14 paragraph_line_y_start dw 64h dup(     ?)                                                              ; 0
dseg02:001B8B14                                                                                                     ; DATA XREF: Mark_Paragraph+2B0w ...
dseg02:001B8BDC paragraph_line_x_start dw 64h dup(     ?)                                                              ; 0
dseg02:001B8BDC                                                                                                     ; DATA XREF: Mark_Paragraph+286w ...
dseg02:001B8CA4 paragraph_line_offset dw 64h dup(     ?)                                                              ; 0
dseg02:001B8CA4                                                                                                     ; DATA XREF: Mark_Paragraph+2C5w ...
dseg02:001B8D6C paragraph_line_x_end dw 64h dup(     ?)                                                              ; 0
dseg02:001B8D6C                                                                                                     ; DATA XREF: Mark_Paragraph+29Bw ...
dseg02:001B8E34 paragraph_max_lines dw ?                                                                            ; DATA XREF: Print_Paragraph+3Fr ...
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Print_Paragraph(x = %d, y = %d, max_width = %d, string = %s, print_type = %d)\n", __FILE__, __LINE__, x, y, max_width, string, print_type);
#endif

    Mark_Paragraph(x, y, max_width, string);

    for(itr = 0; itr < paragraph_max_lines; itr++)
    {
        switch(print_type)
        {
            case 0:
            {
                DLOG("switch(print_type)  case 0:");
                Print(paragraph_line_x_start[itr], paragraph_line_y_start[itr], &string[paragraph_line_offset[itr]]);
            } break;
            case 1:
            {
                DLOG("switch(print_type)  case 1:");
                // Print_Right(paragraph_line_x_start[itr], paragraph_line_y_start[itr], &string[paragraph_line_offset[itr]], max_width);
                Print_Centered(paragraph_line_x_start[itr], paragraph_line_y_start[itr], &string[paragraph_line_offset[itr]]);
            } break;
            case 2:
            {
                DLOG("switch(print_type)  case 2:");
                // Print_Right(paragraph_line_x_start[itr], paragraph_line_y_start[itr], &string[paragraph_line_offset[itr]], max_width);
                Print_Centered(paragraph_line_x_start[itr], paragraph_line_y_start[itr], &string[paragraph_line_offset[itr]]);
            } break;
            case 3:
            {
                DLOG("switch(print_type)  case 3:");
                if((paragraph_max_lines - 1) == itr)
                {
                    Print(paragraph_line_x_start[itr], paragraph_line_y_start[itr], &string[paragraph_line_offset[itr]]);
                }
                else
                {
                    Print_Full(paragraph_line_x_start[itr], paragraph_line_y_start[itr], &string[paragraph_line_offset[itr]], (paragraph_line_x_end[itr] - paragraph_line_x_start[itr]));
                }

            } break;

        }
    }

    Remove_Paragraph_Marks(string);

    exclusion_count = 0;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Print_Paragraph(x = %d, y = %d, max_width = %d, string = %s, print_type = %d)\n", __FILE__, __LINE__, x, y, max_width, string, print_type);
#endif

}

// WZD s19p02
// UU_VGA_WndDrawText()

// WZD s19p03
// UU_GUI_CreateHelpLinks()

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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Mark_Paragraph(x = %d, y = %d, full_width = %d, string = %s)\n", __FILE__, __LINE__, x, y, full_width, string);
#endif

    str_ptr = 0;
    paragraph_max_lines = 0; // ; the amount of elements in the VGA_TextLine_ arrays
    x_override = -1;
    string_widths = (uint8_t *)(font_style_data + FONT_HDR_POS_CURRENT_FONT_WIDTHS);
    font_height = GET_2B_OFS(font_style_data, FONT_HDR_POS_HEIGHT);
    spacing = GET_2B_OFS(font_style_data, FONT_HDR_POS_CURRENT_HORIZONTAL_SPACING);
    line_feed = GET_2B_OFS(font_style_data, FONT_HDR_POS_CURRENT_BASE_HEIGHT);  // MoO2  font_header.current_line_feed ¿ added after MoM ?
    ypos = y;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: font_height: %d\n", __FILE__, __LINE__, font_height);
    dbg_prn("DEBUG: [%s, %d]: spacing: %d\n", __FILE__, __LINE__, spacing);
    dbg_prn("DEBUG: [%s, %d]: line_feed: %d\n", __FILE__, __LINE__, line_feed);
#endif

    xmin = 0;  // DNE in Dasm
    xmax = 0;  // DNE in Dasm
    Line_Width = 0;  // DNE in Dasm



    while(string[str_ptr] != '\0')
    {
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: string[%d]: %c\n", __FILE__, __LINE__, str_ptr, string[str_ptr]);
#endif
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: ypos: %d\n", __FILE__, __LINE__, ypos);
#endif

        skip_flag = 0;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: x_override: %d\n", __FILE__, __LINE__, x_override);
    dbg_prn("DEBUG: [%s, %d]: xmax: %d\n", __FILE__, __LINE__, xmax);
    dbg_prn("DEBUG: [%s, %d]: xmin: %d\n", __FILE__, __LINE__, xmin);
#endif
        if(x_override != -1)
        {
            xmin = x_override;
            xmax = x + full_width - 1;
            x_override = -1;
        }
        else
        {
            xmin = x;
            xmax = x + full_width;
        }
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: x_override: %d\n", __FILE__, __LINE__, x_override);
    dbg_prn("DEBUG: [%s, %d]: xmax: %d\n", __FILE__, __LINE__, xmax);
    dbg_prn("DEBUG: [%s, %d]: xmin: %d\n", __FILE__, __LINE__, xmin);
#endif



        /*
            BEGIN:  Line Limits / Exclusion Areas
        */
        {
            for(itr_exclusion_count = 0; (itr_exclusion_count < exclusion_count) && (skip_flag == ST_FALSE); itr_exclusion_count++)
            {
                if(
                    (
                        (exclusion_area_y1[itr_exclusion_count] > ypos)
                        &&
                        (exclusion_area_y2[itr_exclusion_count] < ypos)
                    )
                    ||
                    (
                        (exclusion_area_y1[itr_exclusion_count] <= (ypos + line_feed))
                        &&
                        (exclusion_area_y2[itr_exclusion_count] >= (ypos + font_height))
                    )
                )
                {
                    if( (exclusion_area_x1[itr_exclusion_count] > xmin) && (exclusion_area_x2[itr_exclusion_count] < xmax) )
                    {
                        if(exclusion_area_x2[itr_exclusion_count] <= xmin)
                        {
                            xmin = (exclusion_area_x1[itr_exclusion_count] + 1);
                        }
                        else
                        {
                            xmax = (exclusion_area_x2[itr_exclusion_count] - 1);
                        }
                        if(xmin > xmax)
                        {
                            skip_flag = ST_TRUE;
                        }
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: Line_Width: %d\n", __FILE__, __LINE__, Line_Width);
#endif
            Line_Width = xmax - xmin + 1;  // we just didn't skip, so (xmin <= xmax)
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: Line_Width: %d\n", __FILE__, __LINE__, Line_Width);
#endif
            blank_pos = -1;
            tab = -1;



            temp_ptr = str_ptr;

            while( -(spacing) <= Line_Width )
            {
                character = string[temp_ptr];
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: character: %02X  '%c'\n", __FILE__, __LINE__, character, character);
#endif
                if(character < 0x20)  /* LT ASCII SPACE ∴ Non-Printable ∴ Control Character */
                {
                    DLOG("(character < 0x20)");
                    if(
                        character == 0x14   /*  ASCII  14h  20d  DC4 (device control 4)      */
                        ||
                        character == 0x0D   /*  ASCII  0Dh  13d  CR  (carriage return)       */
                        ||
                        character == 0x00   /*  ASCII  00h  00d  NUL (null)                  */
                        ||
                        character == 0x19   /*  ASCII  15h  21d  NAK (negative acknowledge)  */
                        ||
                        character == 0x1D   /*  ASCII  1Dh  29d  GS  (group separator)       */
                    )
                    {
                        Line_Width = (-(spacing) - 1);  // ¿ this makes it take the other branch at the top of the loop ?
                        tab = character;
                    }
                }
                else
                {

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: Line_Width: %d\n", __FILE__, __LINE__, Line_Width);
#endif
                    Line_Width -= string_widths[(character - 32)] + spacing;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: Line_Width: %d\n", __FILE__, __LINE__, Line_Width);
#endif

                    if(character == 0x20)
                    {
                        blank_pos = temp_ptr;
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
                    DrawPos_Override =  ((string[temp_ptr] + -48) * 10);
                    DrawPos_Override +=  (string[temp_ptr] + -48);

                    // drake178: ; entirely independent of the current draw locations!
                    // drake178: ; negate the new line that follows
                    ypos = DrawPos_Override - line_feed;

                    temp_ptr += 2;
                }
                if(tab == 0x1D)  /*  ASCII  1Dh  29d  GS (group separator)        */
                {
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: (x + full_width - 1): %d\n", __FILE__, __LINE__, (x + full_width - 1));
    dbg_prn("DEBUG: [%s, %d]: xmax: %d\n", __FILE__, __LINE__, xmax);
    dbg_prn("DEBUG: [%s, %d]: ypos: %d\n", __FILE__, __LINE__, ypos);
    dbg_prn("DEBUG: [%s, %d]: x_override: %d\n", __FILE__, __LINE__, x_override);
#endif
        // @@StringLoopTail
        if( (x + full_width - 1) <= xmax)
        {
            ypos += line_feed;
        }
        else
        {
            x_override = (xmax + 1);
        }
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: ypos: %d\n", __FILE__, __LINE__, ypos);
    dbg_prn("DEBUG: [%s, %d]: x_override: %d\n", __FILE__, __LINE__, x_override);
#endif

    }  /* while(string[str_ptr] != '\0') */


goto Done;


Error:
    paragraph_max_lines = 0;
    goto Done;


Done:

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: paragraph_max_lines: %d\n", __FILE__, __LINE__, paragraph_max_lines);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Mark_Paragraph(x = %d, y = %d, full_width = %d, string = %s)\n", __FILE__, __LINE__, x, y, full_width, string);
#endif

	return;
}

// WZD s19p05
// drake178: VGA_GetTextHeight
int16_t Get_Paragraph_Max_Height(int16_t max_width, char * string)
{
    int16_t paragraph_max_height;
    int16_t font_height;
    //int16_t text_height;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Get_Paragraph_Max_Height(max_width = %d, string = %s)\n", __FILE__, __LINE__, max_width, string);
#endif

    font_height = GET_1B_OFS(font_style_data, FONT_HDR_POS_HEIGHT);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: font_height: %d\n", __FILE__, __LINE__, font_height);
#endif

    Mark_Paragraph(0, 0, max_width, string);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: paragraph_max_lines: %d\n", __FILE__, __LINE__, paragraph_max_lines);
#endif

    if(paragraph_max_lines >= 1)  /* ; the amount of elements in the VGA_TextLine_ arrays */
    {

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: paragraph_max_lines: %d\n", __FILE__, __LINE__, paragraph_max_lines);
#endif

        paragraph_max_height = (paragraph_line_y_start[(paragraph_max_lines - 1)] + font_height);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: paragraph_max_height: %d\n", __FILE__, __LINE__, paragraph_max_height);
#endif

        // drake178: removes the line-breaking spaces inserted into a string by VGA_TextSetup to mark line breaks
        Remove_Paragraph_Marks(string);

    }
    else
    {
        paragraph_max_height = 0;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Get_Paragraph_Max_Height(max_width = %d, string = %s) { paragraph_max_height = %d }\n", __FILE__, __LINE__, max_width, string, paragraph_max_height);
#endif

    return paragraph_max_height;
}


// WZD s19p06
// drake178: VGA_GetTextWidth
// VGA_GetTextWidth

// WZD s19p07
// drake178: VGA_ResetFltBlocks()
// drake178: resets the floating block allocation, text will now be drawn without this limitation
void Reset_Paragraph_Exclusions(void)
{
    exclusion_count = 0;
}


// WZD s19p08
// VGA_AddFloatingBlock()

// WZD s19p09
// UU_VGA_AddFltIMGSpace()


// WZD s19p10
// drake178: STR_RemoveLBSPs
void Remove_Paragraph_Marks(char * string)
{
    int16_t itr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Remove_Paragraph_Marks(string = %s)\n", __FILE__, __LINE__, string);
#endif

    itr = 0;
    while(string[itr] != '\0')
    {
        if(string[itr] == 0x15)  /*  ASCII  19h  25d  EM (end of medium)          */
        {
            string[itr] = 0x20;  /*  ASCII  20h  32d  SPACE                       */
        }
        itr++;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Remove_Paragraph_Marks(string = %s)\n", __FILE__, __LINE__, string);
#endif

}


// WZD s19p11
// WZD s19p12
// WZD s19p13
// WZD s19p14
