/*
    WIZARDS.EXE
    

*/

#include "MoX_TYPE.H"     /* byte_ptr, SAMB_ptr */
#include "MoX_DEF.H"      /* DLOG() */
#include "MoM_DEF.H"

#include "MoX_Data.H"

#include "FLIC_Draw.H"

#include "Fonts.H"    /* p_Palette() */
#include "Video.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif


extern uint8_t g_Palette[];
extern uint8_t g_Palette_XBGR[];



/*
    WZD  seg021
*/

// WZD s21p07
void FLIC_Load_Palette(SAMB_ptr p_FLIC_Header, int16_t frame_index)
{
    SAMB_ptr p_FLIC_File;
    int16_t start;
    int16_t count;
    byte_ptr flic_palette_data;
    int16_t itr;

    p_FLIC_File = (p_FLIC_Header + 0);  // ~== p_FLIC_File = &p_FLIC_Header[0]

    if((frame_index == 0) || (FLIC_GET_FRAME_PALETTES(p_FLIC_File) == 0))
    {
        start = FLIC_GET_PALETTE_COLOR_START(p_FLIC_File);
        count = FLIC_GET_PALETTE_COLOR_COUNT(p_FLIC_File);
        flic_palette_data = (p_FLIC_File + FLIC_GET_PALETTE_DATA_OFFSET(p_FLIC_File));
    }
    else
    {
        flic_palette_data = (p_FLIC_File + FLIC_GET_FRAME_PALETTE_DATA_OFFSET(p_FLIC_File,frame_index));
        start = FLIC_GET_FRAME_PALETTE_COLOR_INDEX(p_FLIC_File,frame_index);
        count = FLIC_GET_FRAME_PALETTE_COLOR_COUNT(p_FLIC_File,frame_index);
    }

    for(itr = start; itr < count; itr++)
    {
        *(p_Palette + itr) = *(flic_palette_data + itr);
        *(p_Palette + 768 + itr) = 1;
    }

    for(itr = start; itr < count; itr++)
    {
        *(p_Palette_XBGR + (start * 4) + (itr * 4) + 3) = 0x00;
        *(p_Palette_XBGR + (start * 4) + (itr * 4) + 2) = (*(flic_palette_data + (start * 3) + (itr * 3) + 0) << 2);
        *(p_Palette_XBGR + (start * 4) + (itr * 4) + 1) = (*(flic_palette_data + (start * 3) + (itr * 3) + 1) << 2);
        *(p_Palette_XBGR + (start * 4) + (itr * 4) + 0) = (*(flic_palette_data + (start * 3) + (itr * 3) + 2) << 2);
    }

}



/*
    WZD seg024
*/
// WZD s24p08
// drake178: VGA_SaveDrawSection
/*
    calls Set_Page_Off(), so ~ Copy Off To Picture
    MoO2: Screen_Bitmap_Capture() |-> Create_Bitmap_Header(); Capture_Screen_Block_();
*/
void Screen_Picture_Capture(int16_t x1, int16_t y1, int16_t x2, int16_t y2, SAMB_ptr pict_seg)
{
    int16_t width;
    int16_t height;

    Set_Page_Off();

    width = x2 - x1 + 1;
    height = y2 - y1 + 1;

    if(width < 0)
    {
        width = width * -1;
    }
    if(height < 0)
    {
        height = height * -1;
    }

    Create_Picture(width, height, pict_seg);

    Capture_Screen_Block((pict_seg + 16), x1, y1, x2, y2);

}


// WZD s24p09
/*
    x1,y1,x2,y2 of screen
*/
void Capture_Screen_Block(byte_ptr frame_data, int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
    byte_ptr buffer;
    byte_ptr screen_page;
    int16_t width;
    int16_t height;
    uint16_t screen_page_offset;
    uint16_t itr_width;
    uint16_t itr_height;
    
    width = x2 - x1 + 1;
    height = y2 - y1 + 1;

    screen_page_offset = ((y1 * SCREEN_WIDTH) + x1);

    buffer = frame_data;

    screen_page = current_video_page + screen_page_offset;

    itr_height = 0;
    while(itr_height++ < height)
    {
        itr_width = 0;
        while(itr_width++ < width)
        {
            *buffer++ = *(screen_page + (itr_height * SCREEN_WIDTH) + itr_width);
        }
    }

}


/*
    WZD  seg029
*/

// WZD s29p01
void FLIC_Draw_Frame(int16_t x_start, int16_t y_start, int16_t width, byte_ptr frame_data)
{
    unsigned char * bbuff_pos;
    unsigned char * bbuff;
    unsigned char data_byte;

    unsigned char packet_op;
    unsigned char packet_byte_count;
    unsigned char sequence_byte_count;
    unsigned char delta_byte_count;
    unsigned char itr_op_repeat;

    bbuff_pos = current_video_page + ((y_start * SCREEN_WIDTH) + x_start);

    while (width--)
    {
        bbuff = bbuff_pos++;
        packet_op = *frame_data++;
        if(packet_op == 0xFF)  /* Type: skip */
        {
            continue;
        }
        packet_byte_count = *frame_data++;
        if(packet_op == 0x80)  /* Type: decode */
        {
            do {
                sequence_byte_count = *frame_data++;
                delta_byte_count = *frame_data++;
                bbuff += (delta_byte_count * SCREEN_WIDTH);
                packet_byte_count -= sequence_byte_count + 2;
                while(sequence_byte_count--)
                {
                    data_byte = *frame_data++;  // this unsigned char is the op-repeat or just the unsigned char to copy
                    if(data_byte >= 224)  /* op: repeat */
                    {
                        itr_op_repeat = (data_byte - 224) + 1;
                        sequence_byte_count--;
                        data_byte = *frame_data++;
                        while(itr_op_repeat--)
                        {
                            *bbuff = data_byte;
                            bbuff += SCREEN_WIDTH;
                        }
                    }
                    else  /* op: copy */
                    {
                        *bbuff = data_byte;
                        bbuff += SCREEN_WIDTH;
                    }
                }
            } while(packet_byte_count >= 1);
        }
        if(packet_op == 0x00)  /* Type: copy */
        {
            do {
                sequence_byte_count = *frame_data++;
                delta_byte_count = *frame_data++;
                bbuff += (delta_byte_count * SCREEN_WIDTH);
                packet_byte_count -= sequence_byte_count + 2;
                while(sequence_byte_count--)
                {
                    *bbuff = *frame_data++;
                    bbuff += SCREEN_WIDTH;
                }
            } while(packet_byte_count >= 1);
        }
    }

}



/*
    WZD seg030
*/


// WZD s30p02
/*
    ¿ FLIC_Prepare() vs. Create_Blank_Picture() ?
    same header values, just default transparent vs. specified color
*/
void Create_Picture(int16_t width, int16_t height, byte_ptr pict_seg)
{
    int16_t length;
    int16_t itr_length;

    // TODO(JimBalcomb,20230701): fix up the Macros
    // FLIC_SET_WIDTH(pict_seg, width);
    // FLIC_SET_HEIGHT(pict_seg, height);
    // FLIC_SET_CURRENT_FRAME(pict_seg,0xDE0A);  // LBX_DecodedIMG
    // FLIC_SET_FRAME_COUNT(pict_seg, 0);
    // FLIC_SET_LOOP_FRAME(pict_seg,0);

    SET_2B_OFS(pict_seg,0,width);
    SET_2B_OFS(pict_seg,2,height);
    SET_2B_OFS(pict_seg,4,0xDE0A);  /* e_FLIC_Decoded */
    SET_2B_OFS(pict_seg,6,0);
    SET_2B_OFS(pict_seg,8,0);

    length = width * height;
    
    itr_length = 0;
    while(itr_length++ < length)
    {
        *(pict_seg + 16 + itr_length) = ST_TRANSPARENT;  /* Color-Map Index 0 */
    }

}

// WZD s30p06
/*
    ¿ FLIC_Prepare() vs. Create_Blank_Picture() ?
*/
void Create_Blank_Picture(int16_t width, int16_t height, byte_ptr pict_seg, uint8_t color)
{
    int16_t length;
    int16_t itr_length;

    // TODO(JimBalcomb,20230701): fix up the Macros
    // FLIC_SET_WIDTH(pict_seg, width);
    // FLIC_SET_HEIGHT(pict_seg, height);
    // FLIC_SET_CURRENT_FRAME(pict_seg,0xDE0A);  // LBX_DecodedIMG
    // FLIC_SET_FRAME_COUNT(pict_seg, 0);
    // FLIC_SET_LOOP_FRAME(pict_seg,0);

    SET_2B_OFS(pict_seg,0,width);
    SET_2B_OFS(pict_seg,2,height);
    SET_2B_OFS(pict_seg,4,0xDE0A);  /* e_FLIC_Decoded */
    SET_2B_OFS(pict_seg,6,0);
    SET_2B_OFS(pict_seg,8,0);

    length = width * height;
    
    itr_length = 0;
    while(itr_length++ < length)
    {
        *(pict_seg + 16 + itr_length) = color;
    }

}


// WZD s30p11
void FLIC_Draw(int16_t x_start, int16_t y_start, SAMB_ptr p_FLIC_File)
{
    int16_t current_frame_index;
    int16_t next_frame_index;
    unsigned int flic_frame_offset;
    unsigned short flic_frame_offset_sgmt;
    unsigned short flic_frame_offset_ofst;
    byte_ptr p_FLIC_Frame;

    // ? MEM_Copy_Far(&flic_hdr, 0, 0, p_FLIC_File, 16) ?

    current_frame_index = FLIC_GET_CURRENT_FRAME(p_FLIC_File);
    next_frame_index = FLIC_GET_CURRENT_FRAME(p_FLIC_File) + 1;
    if(next_frame_index < FLIC_GET_FRAME_COUNT(p_FLIC_File))
    {
        FLIC_SET_CURRENT_FRAME(p_FLIC_File, next_frame_index);
    }
    else
    {
        FLIC_SET_CURRENT_FRAME(p_FLIC_File, FLIC_GET_LOOP_FRAME(p_FLIC_File));
    }

    if((FLIC_GET_PALETTE_HEADER_OFFSET(p_FLIC_File) != 0))
    {
        FLIC_Load_Palette(p_FLIC_File, current_frame_index);
    }

    flic_frame_offset = FLIC_GET_FRAME_OFFSET(p_FLIC_File, current_frame_index);
    p_FLIC_Frame = p_FLIC_File + (flic_frame_offset) + 1;

    FLIC_Draw_Frame(x_start, y_start, FLIC_GET_WIDTH(p_FLIC_File), p_FLIC_Frame);

}


// WZD s30p13
// AKA LBX_IMG_BuildFrame()
// NOTE: Draw_Picture_To_Bitmap(SAMB_ptr src_pict_seg) ~== FLIC_Draw(SAMB_ptr p_FLIC_File)
void Draw_Picture_To_Bitmap(SAMB_ptr src_pict_seg, SAMB_ptr dst_pict_seg)
{
    int16_t current_frame_index;
    int16_t next_frame_index;
    int8_t full_store_flag;
    int16_t frame_start;
    int16_t frame_idx;
    int16_t itr_frames;
    uint32_t flic_frame_offset;
    byte_ptr p_FLIC_Frame;

    /*
        BEGIN: same as FLIC_Draw()
    */
    // MEM_Copy_Far(src sgmt ofst dst sgmt ofst)
    // ? MEM_Copy_Far(&flic_hdr, 0, 0, p_FLIC_File, 16) ?

    current_frame_index = FLIC_GET_CURRENT_FRAME(src_pict_seg);
    next_frame_index = FLIC_GET_CURRENT_FRAME(src_pict_seg) + 1;
    if(next_frame_index < FLIC_GET_FRAME_COUNT(src_pict_seg))
    {
        FLIC_SET_CURRENT_FRAME(src_pict_seg, next_frame_index);
    }
    else
    {
        FLIC_SET_CURRENT_FRAME(src_pict_seg, FLIC_GET_LOOP_FRAME(src_pict_seg));
    }

    if((FLIC_GET_PALETTE_HEADER_OFFSET(src_pict_seg) != 0))
    {
        FLIC_Load_Palette(src_pict_seg, current_frame_index);
    }
    /*
        END: same as FLIC_Draw()
    */

   Create_Multi_Frame_Blank_Bitmap(src_pict_seg, dst_pict_seg, current_frame_index);

   /*
        if header.ehn != 0
            ~ Add_Picture_To_Bitmap_EMM
   */

    /*
        "Animation File"
            "Get_Full_Store_Flag()"
        ...
        farpeekb(src_pict_seg, 16);

        if full store, just draw current frame
        else, build up to current frame
    */
    full_store_flag = GET_1B_OFS(src_pict_seg, 16);

    if(full_store_flag != ST_FALSE)
    {
        frame_start = current_frame_index;
    }
    else
    {
        frame_start = 0;
    }

    frame_idx = current_frame_index;

    for(itr_frames = frame_start; itr_frames <= frame_idx; itr_frames++)
    {
        /*
            BEGIN: same as FLIC_Draw()
        */
        flic_frame_offset = FLIC_GET_FRAME_OFFSET(src_pict_seg, current_frame_index);
        p_FLIC_Frame = (src_pict_seg + (flic_frame_offset + 1));
        /*
            END: same as FLIC_Draw()
        */

        Add_Picture_To_Bitmap(p_FLIC_Frame, dst_pict_seg);

    }

}


// WZD s30p14
void FLIC_Set_CurrentFrame(SAMB_ptr p_FLIC_Header, int16_t frame_index)
{
    int16_t loop_length;
    int16_t loop_frame;
    int16_t frame_count;

    frame_index = (frame_index & 0x7fff);
    frame_count = FLIC_GET_FRAME_COUNT(p_FLIC_Header);
    loop_frame = FLIC_GET_LOOP_FRAME(p_FLIC_Header);
    loop_length = frame_count - loop_frame;

    if( !(frame_index < frame_count) )
    {
        frame_index = loop_frame + ((frame_index - frame_count) % loop_length);
    }

    FLIC_SET_CURRENT_FRAME(p_FLIC_Header, frame_index);

}


// WZD s30p15
void FLIC_Reset_CurrentFrame(SAMB_ptr p_FLIC_Header)
{

    FLIC_SET_CURRENT_FRAME(p_FLIC_Header, 0);

}


// WZD s30p16
int16_t FLIC_Get_CurrentFrame(SAMB_ptr p_FLIC_Header)
{
    int16_t current_frame_index;

    current_frame_index = FLIC_GET_CURRENT_FRAME(p_FLIC_Header);

    return current_frame_index;
}


// WZD s30p17
// ?NIU? int16_t FLIC_Get_FrameCount()


// WZD s30p18
int16_t FLIC_Get_Width(SAMB_ptr p_FLIC_Header)
{
    int16_t flic_width;

    flic_width = FLIC_GET_WIDTH(p_FLIC_Header);

    return flic_width;
}


// WZD s30p19
int16_t FLIC_Get_Height(SAMB_ptr p_FLIC_Header)
{
    int16_t flic_height;

    flic_height = FLIC_GET_HEIGHT(p_FLIC_Header);

    return flic_height;
}


// WZD s30p24
void Draw_Picture(int16_t x, int16_t y, byte_ptr pict_seg)
{
    int16_t height;
    int16_t width;

    width = GET_2B_OFS(pict_seg, 0);
    height = GET_2B_OFS(pict_seg, 2);

    Draw_Picture_ASM(x, y, SZ_FLIC_HDR, pict_seg, width, height, 0);

}


// WZD s30p25
// MoO2: ~ Clipped_Draw_Picture()
void Draw_Picture_Windowed(int16_t x1, int16_t y1, byte_ptr pict_seg)
{
    int16_t height;
    int16_t width;
    int16_t x2;
    int16_t y2;
    int16_t skip_x;
    int16_t start_x;
    int16_t skip_y;
    int16_t start_y;
    int16_t actual_width;
    int16_t actual_height;
    int16_t buffer_add;
    int16_t skip_add;

    // Test - completely off on the right
    if( !(x1 > screen_window_x2) )
    {
        // Test - completely off on the bottom
        if( !(y1 > screen_window_y2) )
        {
            width = GET_2B_OFS(pict_seg, 0);
            height = GET_2B_OFS(pict_seg, 2);
            x2 = x1 + width - 1;
            // Test - completely off on the left
            if( !(x2 < screen_window_x1) )
            {
                y2 = y1 + height - 1;
                // Test - completely off on the top
                if( !(y2 < screen_window_y1) )
                {
                    // Test - partially off on the left
                    if(x1 >= screen_window_x1)
                    {
                        skip_x = 0;
                        start_x = x1;
                    }
                    else
                    {
                        skip_x = screen_window_x1 - x1;
                        start_x = screen_window_x1;
                    }
                    // Test - partially off on the top
                    if(y1 >= screen_window_y1)
                    {
                        skip_y = 0;
                        start_y = y1;
                    }
                    else
                    {
                        skip_y = screen_window_y1 - y1;
                        start_y = screen_window_y1;
                    }
                    // Test - partially off on the right
                    if(x2 < screen_window_x2)
                    {
                        actual_width = x2 - start_x + 1;
                    }
                    else
                    {
                        actual_width = screen_window_x2 - start_x + 1;
                    }
                    // Test - partially off on the bottom
                    if(y2 < screen_window_y2)
                    {
                        actual_height = y2 - start_y + 1;
                    }
                    else
                    {
                        actual_height = screen_window_y2 - start_y + 1;
                    }

                    buffer_add = SZ_FLIC_HDR + skip_y + (skip_x * height);
                    skip_add = height - actual_height;

                    Draw_Picture_ASM(start_x, start_y, buffer_add, pict_seg, actual_width, actual_height, skip_add);

                }
            }
        }
    }

}


// WZD s30p40
int16_t Get_Full_Store_Flag(SAMB_ptr p_FLIC_Header)
{
    int16_t full_store_flag;

    full_store_flag = FLIC_GET_FRAME_TYPE(p_FLIC_Header);

    return full_store_flag;
}



// WZD s30p41
// drake178: LBX_IMG_SetLoop1
void FLIC_Set_LoopFrame_1(SAMB_ptr p_FLIC_Header)
{
    FLIC_SET_LOOP_FRAME(p_FLIC_Header, 1);
}



/*
    WIZARDS.EXE  seg033

    ? MoO2: Module: bitmap
*/

// WZD s33p01
/*
    copy width and height
    set type, count, loop
    ? frame index is 
*/
// ? MoO2: Create_Multi_Frame_Bitmap_Header() ?
// AKA LBX_IMG_DecPrepare()
void Create_Multi_Frame_Blank_Bitmap(SAMB_ptr src_pict_seg, SAMB_ptr dst_pict_seg, int16_t frame_idx)
{
    int16_t src_width;
    int16_t src_height;
    int16_t dst_width;
    int16_t dst_height;
    int16_t dst_current;
    int16_t dst_count;
    int16_t dst_loop;
    int16_t length;
    int16_t word_length;
    uint16_t * word_ptr;

    src_width = GET_2B_OFS(src_pict_seg, 0);
    src_height = GET_2B_OFS(src_pict_seg, 2);

    /*
        animation_header
            width, height, current, count, loop, ehn, elpn, elpo, pal_hdr_ofst
    */
    dst_width = src_width;
    dst_height = src_height;
    dst_current = 0xDE0A;  // ?!? CurrentFrame = enum ? e_FLIC_Decoded
    dst_count = frame_idx;
    dst_loop = 0;
    // DNE dst ehn
    // DNE dst elpn
    // DNE dst elpo
    // DNE dst pal_hdr_ofst

    SET_2B_OFS(dst_pict_seg, 0, dst_width);
    SET_2B_OFS(dst_pict_seg, 2, dst_height);
    SET_2B_OFS(dst_pict_seg, 4, dst_current);
    SET_2B_OFS(dst_pict_seg, 6, dst_count);
    SET_2B_OFS(dst_pict_seg, 8, dst_loop);
    // SET_2B_OFS(dst_pict_seg, 10, 0);
    // SET_2B_OFS(dst_pict_seg, 12, 0);
    // SET_2B_OFS(dst_pict_seg, 14, 0);

    length = src_width * src_height;
    word_length = ((length + 1) / 2);
    word_ptr = (uint16_t *)(dst_pict_seg + SZ_FLIC_HDR);

    while(word_length)
    {
        *word_ptr++ = 0;
        word_length--;
    }

}


// WZD s33p02
// AKA LBX_IMB_LoadFrame()
void Add_Picture_To_Bitmap(byte_ptr source_picture, byte_ptr destination_bitmap)
{
    int16_t width;
    int16_t height;
    byte_ptr src;
    byte_ptr dst;
    byte_ptr dst_pos;
    byte packet_op;
    byte packet_byte_count;
    byte sequence_byte_count;
    byte delta_byte_count;
    byte data_byte;
    byte itr_op_repeat;
    byte_ptr dst_base;
    byte_ptr dst_curr;
    uint16_t dst_diff;
    int16_t dst_w;
    int16_t dst_h;
    int16_t dst_x;
    int16_t dst_y;

    width = FLIC_GET_WIDTH(destination_bitmap);
    height = FLIC_GET_HEIGHT(destination_bitmap);

    src = source_picture;
    dst = (destination_bitmap + 16);
    dst_pos = (destination_bitmap + 16);

    dst_base = dst;
    dst_curr = dst;
    dst_diff = dst_curr - dst_base;
    dst_w = width;
    dst_h = height;
    // ? x = diff / height ?
    dst_x = dst_diff / dst_h;
    // ? diff - (x * height) ?
    dst_y = dst_diff - (dst_x * dst_h);

    while(width--)
    {
        dst = dst_pos;
        dst_pos += height;
        packet_op = *src++;
        if(packet_op == 0xFF)
        {
            dst += height;
            continue;
        }
        packet_byte_count = *src++;
        if(packet_op == 0x80)  /* Type: decode */
        {
            do {
                sequence_byte_count = *src++;
                delta_byte_count = *src++;
                dst += delta_byte_count;
                packet_byte_count -= sequence_byte_count + 2;
                while(sequence_byte_count--)
                {
                    data_byte = *src++;
                    if(data_byte >= 224)  /* op: repeat */
                    {
                        itr_op_repeat = (data_byte - 224) + 1;
                        sequence_byte_count--;
                        data_byte = *src++;
                        while(itr_op_repeat--)
                        {
                            *dst = data_byte;
                            dst++;
                        }
                    }
                    else  /* op: copy */
                    {
                        *dst = data_byte;
                        dst++;
                    }
                }
            } while(packet_byte_count >= 1);
        }
        if(packet_op == 0x00)  /* Type: copy */
        {
            do {
                sequence_byte_count = *src++;
                delta_byte_count = *src++;
                dst += delta_byte_count;
                packet_byte_count -= sequence_byte_count + 2;
                while(sequence_byte_count--)
                {
                    *dst = *src;
                    src++;
                    dst++;
                }
            } while(packet_byte_count >= 1);
        }
    }

}


// WZD s33p05
void Draw_Picture_ASM(int16_t x_start, int16_t y_start, int16_t ofst, byte_ptr pict_seg, int16_t width, int16_t height, int16_t skip_x)
{
    byte_ptr screen_start;
    byte_ptr screen_pos;
    byte_ptr data;
    uint8_t data_byte;
    int16_t itr_width;
    int16_t itr_height;
    
    screen_start = current_video_page + (y_start * SCREEN_WIDTH) + x_start;
    data = pict_seg + ofst;
    screen_pos = screen_start;
    itr_width = width;
    while(itr_width)
    {
        screen_pos = screen_start++;
        itr_height = height;
        while(itr_height)
        {
            data_byte = *data++;
            if(data_byte != 0x00)  /* color index 0 TRANSPARENT */
            {
                *screen_pos = data_byte;
            }
            screen_pos += SCREEN_WIDTH;  // add screen line stride - same column, one row down

            itr_height--;
        }
        data += skip_x;

        itr_width--;
    }

}


// WZD s33p09
// drake178: LBX_IMG_ColorReplace
void FLIC_Remap_Color(SAMB_ptr pict_seg, uint8_t color, uint8_t remap_color)
{
    int16_t loop_counter;
    uint8_t * src;
    uint8_t * dst;
    uint8_t pixel;

    loop_counter = FLIC_GET_WIDTH(pict_seg) * FLIC_GET_HEIGHT(pict_seg);

    src = pict_seg + 16;
    dst = pict_seg + 16;

    while(loop_counter--)
    {
        pixel = *src++;
        dst++;
        if(pixel == color)
        {
            dst--;
            *dst++ = remap_color;
        }
    }

}

