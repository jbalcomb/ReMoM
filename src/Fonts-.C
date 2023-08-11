
// NOTE(JimBalcomb,20230809):
// Meh.
// just a back-up, just-in-case
// trying to refactor/remaster the logic for the Assembly code Procedures
// Driver?
//  really does look like the first level should be the whole business
//  wrapped in the first test for whether the sign bit is set  (font_data_byte & 0x80) == 0
//  also, dont know why I had thought the was a any/a separate test for (font_data_byte & 0x7F)
// Well, actually...
//  it looks like is starts with `(font_data_byte & 0x80) == 0` in the moO2 code...
//  but, maybe, in the MoM code, really does start with testing for negative?
//  and, that is the very next step in even in the moO2 code...
//  so, ...
//      if(font_data_byte < 0)



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
    int DIFF_screen_start;
    int DIFF_screen_pos;
    byte_ptr ORIG_current_video_page;
    byte_ptr ORIG_screen_start;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Print_Character_No_Alias_ASM(x_start = %d, y_start = %d, width = %d, font_data_offset = %p)\n", __FILE__, __LINE__, x_start, y_start, width, font_data_offset);
#endif

    ORIG_current_video_page = current_video_page;
    screen_start = current_video_page + ((y_start * SCREEN_WIDTH) + x_start);
    ORIG_screen_start = screen_start;

    DIFF_screen_start = screen_start - current_video_page;

    screen_pos = screen_start;
    while(width)
    {
        DIFF_screen_pos = screen_pos - screen_start;

        font_data_byte = *font_data_offset++;

        if(font_data_byte < 0)
        {

            if( (font_data_byte &= 0x7F) == 0 )  /* end column */
            {
                // @@Did_End_Column
                screen_start++;
                assert( screen_start <= (current_video_page + ((SCREEN_HEIGHT - 1) * SCREEN_WIDTH)) );
                screen_pos = screen_start;
                width--;
                assert(width >= 0);
            }
            else  /* skip pixels */
            {
                skip_count = font_data_byte;
                screen_pos += skip_count * SCREEN_WIDTH;
                assert( screen_pos <= (current_video_page + (SCREEN_HEIGHT * SCREEN_WIDTH)) );
            }

        }
        else
        {
            repeat_count = (font_data_byte >> 4);  /* upper nibble */

            if( (font_data_byte &= 0x0F) == 0 ) /* transparent/alias/skip pixel */
            {
                while(repeat_count--)
                {
                    screen_pos += SCREEN_WIDTH;
                    assert( screen_pos <= (current_video_page + (SCREEN_HEIGHT * SCREEN_WIDTH)) );
                }
            }
            else
            {
                while(repeat_count--)
                {
                    *screen_pos = font_header->current_colors[font_data_byte];
                    screen_pos += SCREEN_WIDTH;
                    assert( screen_pos <= (current_video_page + (SCREEN_HEIGHT * SCREEN_WIDTH)) );
                }
            }
        }

    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Print_Character_No_Alias_ASM(x_start = %d, y_start = %d, width = %d, font_data_offset = %p)\n", __FILE__, __LINE__, x_start, y_start, width, font_data_offset);
#endif

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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Print_Character_ASM(x_start = %d, y_start = %d, width = %d, font_data_offset = %p)\n", __FILE__, __LINE__, x_start, y_start, width, font_data_offset);
#endif

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

        font_data_byte = *font_data_offset++;

        /* Type: skip column */
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

                *screen_pos = font_header->current_colors[color_index];
                screen_pos += SCREEN_WIDTH;
            }
        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Print_Character_ASM(x_start = %d, y_start = %d, width = %d, font_data_offset = %p)\n", __FILE__, __LINE__, x_start, y_start, width, font_data_offset);
#endif

}


// WZD s18p06
void Print_Character_No_Alias_ASM(int16_t x_start, int16_t y_start, int16_t width, byte_ptr font_data_offset)
{
byte_ptr screen_start;
    byte_ptr screen_pos;
    uint8_t font_data_byte;
    uint8_t skip_count;
    uint8_t repeat_count;
    uint8_t color_index;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Print_Character_No_Alias_ASM(x_start = %d, y_start = %d, width = %d, font_data_offset = %p)\n", __FILE__, __LINE__, x_start, y_start, width, font_data_offset);
#endif

    screen_start = current_video_page + ((y_start * 320) + x_start);

    screen_pos = screen_start;
    while(width)
    {
        font_data_byte = *font_data_offset++;

        /* Type: skip column */
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

        if( (font_data_byte & 0x80) == 0 )
        {
            DLOG("( (font_data_byte & 0x80) == 0 )");
            repeat_count = ((font_data_byte & 0xF0) >> 4);

            if(font_data_byte == 0)
            {
                DLOG("(font_data_byte == 0)");
                screen_pos += SCREEN_WIDTH;
            }
            else
            {
                DLOG("(font_data_byte != 0)");
                while(repeat_count--)
                {
                    color_index = (font_data_byte & 0x0F);
                    *screen_pos = font_header->current_colors[color_index];
                    screen_pos += SCREEN_WIDTH;
                }
            }

        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Print_Character_No_Alias_ASM(x_start = %d, y_start = %d, width = %d, font_data_offset = %p)\n", __FILE__, __LINE__, x_start, y_start, width, font_data_offset);
#endif

}
