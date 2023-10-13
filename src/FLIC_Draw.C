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


// DELETE  extern uint8_t g_Palette[];
// DELETE  extern uint8_t g_Palette_XBGR[];



/*
    WZD  seg021
*/

// WZD s21p07
// ¿ MoO2  Module: animate  Set_Animation_Palette() ?
// ¿ vs. MoO2  Draw_Palette() ?
void FLIC_Load_Palette(SAMB_ptr p_FLIC_Header, int16_t frame_index)
{
    SAMB_ptr p_FLIC_File;
    int16_t start;
    int16_t count;
    byte_ptr flic_palette_data;
    int16_t itr;

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: BEGIN: FLIC_Load_Palette(p_FLIC_Header = %p, frame_index = %d)\n", __FILE__, __LINE__, p_FLIC_Header, frame_index);
// #endif

    p_FLIC_File = (p_FLIC_Header + 0);  // ~== p_FLIC_File = &p_FLIC_Header[0]

    if((frame_index == 0) || (FLIC_GET_FRAME_PALETTES(p_FLIC_File) == 0))
    {
        // DLOG("((frame_index == 0) || (FLIC_GET_FRAME_PALETTES(p_FLIC_File) == 0))");
        start = FLIC_GET_PALETTE_COLOR_START(p_FLIC_File);
        count = FLIC_GET_PALETTE_COLOR_COUNT(p_FLIC_File);
        flic_palette_data = (p_FLIC_File + FLIC_GET_PALETTE_DATA_OFFSET(p_FLIC_File));
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: FLIC_GET_PALETTE_DATA_OFFSET(p_FLIC_File): %04X\n", __FILE__, __LINE__, FLIC_GET_PALETTE_DATA_OFFSET(p_FLIC_File));
// #endif
    }
    else
    {
        // DLOG("((frame_index != 0) && (FLIC_GET_FRAME_PALETTES(p_FLIC_File) != 0))");
        flic_palette_data = (p_FLIC_File + FLIC_GET_FRAME_PALETTE_DATA_OFFSET(p_FLIC_File,frame_index));
        start = FLIC_GET_FRAME_PALETTE_COLOR_INDEX(p_FLIC_File,frame_index);
        count = FLIC_GET_FRAME_PALETTE_COLOR_COUNT(p_FLIC_File,frame_index);
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: FLIC_GET_FRAME_PALETTE_DATA_OFFSET(p_FLIC_File,frame_index): %04X\n", __FILE__, __LINE__, FLIC_GET_FRAME_PALETTE_DATA_OFFSET(p_FLIC_File,frame_index));
// #endif
    }
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: start: %d\n", __FILE__, __LINE__, start);
//     dbg_prn("DEBUG: [%s, %d]: count: %d\n", __FILE__, __LINE__, count);
//     dbg_prn("DEBUG: [%s, %d]: flic_palette_data: %p\n", __FILE__, __LINE__, flic_palette_data);
// #endif

    // // for(itr = start; itr < count; itr++)
    // for(itr = start; itr < (start + count); itr++)
    // {
    //     *(p_Palette + itr) = *(flic_palette_data + itr);
    //     *(p_Palette + 768 + itr) = 1;
    // }
    for(itr = 0; itr < count; itr++)
    {
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: *(flic_palette_data + itr): %02X\n", __FILE__, __LINE__, *(flic_palette_data + itr));
// #endif
        // DASM: MOVSW; MOVSB;
        // ¿ ~== ? SET_2B, SET_1B ... rvr++;
        *(p_Palette + (start * 3) + (itr * 3) + 0) = *(flic_palette_data + (itr * 3) + 0);
        *(p_Palette + (start * 3) + (itr * 3) + 1) = *(flic_palette_data + (itr * 3) + 1);
        *(p_Palette + (start * 3) + (itr * 3) + 2) = *(flic_palette_data + (itr * 3) + 2);

        *(p_Palette + 768 + start + itr) = ST_TRUE;  // TODO  review, remaster, add manifest-constant for palette change flags offset
    }

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: END: FLIC_Load_Palette(p_FLIC_Header = %p, frame_index = %d)\n", __FILE__, __LINE__, p_FLIC_Header, frame_index);
// #endif
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

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: BEGIN: FLIC_Draw_Frame(x_start = %d, y_start = %d, width = %d, frame_data = %p)\n", __FILE__, __LINE__, x_start, y_start, width, frame_data);
// #endif

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

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: END: FLIC_Draw_Frame(x_start = %d, y_start = %d, width = %d, frame_data = %p)\n", __FILE__, __LINE__, x_start, y_start, width, frame_data);
// #endif
}


// WZD s29p02
// MoO2  Module: animate  Remap_Draw() |-> Remap_Draw_Animated_Sprite(); ... Module: remap  unsigned int picture_remap_color_list[256]  Address: 02:001B479C
/*
    MoO2
    Module: animate
    Remap_Draw_Animated_Sprite()  Address: 01:0012BAEB
        signed integer (2 bytes) x_start
        signed integer (2 bytes) y_start
        pointer (4 bytes) frame_data
            Locals:
                signed integer (4 bytes) bitmap_size
                signed integer (4 bytes) pos
                signed integer (4 bytes) screen_pos
                signed integer (4 bytes) x
                signed integer (4 bytes) y
                signed integer (4 bytes) i
                signed integer (4 bytes) screen_start
                signed integer (4 bytes) buffer_pos
                signed integer (4 bytes) buffer_pos_word
                signed integer (4 bytes) packet_end
                signed integer (2 bytes) height
                signed integer (2 bytes) data_count
                signed integer (2 bytes) skip_count
                signed integer (2 bytes) store_type
                signed integer (2 bytes) line_skip
                unsigned integer (4 bytes) data
                pointer (4 bytes) frame_data_word
    Module: remap
    unsigned int picture_remap_color_list[256]  Address: 02:001B479C

*/
/*
    draw a FLIC frame, using the remap colors
*/
void FLIC_Remap_Draw_Frame(int16_t x_start, int16_t y_start, int16_t width, byte_ptr frame_data)
{
    unsigned char * bbuff_pos;  // TODO rename all these to screen_start
    unsigned char * bbuff;  // TODO rename all these to screen_pos
    unsigned char data_byte;

    unsigned char packet_op;
    unsigned char packet_byte_count;
    unsigned char sequence_byte_count;
    unsigned char delta_byte_count;
    unsigned char itr_op_repeat;

    uint8_t remap_block;
    uint8_t remap_block_index;
    uint8_t remap_color;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: FLIC_Remap_Draw_Frame(x_start = %d, y_start = %d, width = %d, frame_data = %p)\n", __FILE__, __LINE__, x_start, y_start, width, frame_data);
#endif

    bbuff_pos = current_video_page + ((y_start * SCREEN_WIDTH) + x_start);
//     screen_start = current_video_page + (y_start * SCREEN_WIDTH) + x_start;
// 
//     data = pict_seg + ofst;
//     screen_pos = screen_start;
//     itr_width = width;

    while (width--)
    {
        bbuff = bbuff_pos++;
        packet_op = *frame_data++;  // Frame Byte #1: Op/Count
        if(packet_op == 0xFF)  /* Type: skip */
        {
            continue;
        }
        // HERE: not actually 0x80 and 0x00, just sign-bit set or unset
        // not skip, so copy or decode  (MoO2 ¿ "store_type" ?)
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
                        if(data_byte >= 232)
                        {
                            remap_block = data_byte - 232;  /* index of picture_remap_color_list[] */
                            while(itr_op_repeat--)
                            {
                                // repeatedly, get the existing palette index; remap it; replace it; move down;
                                remap_block_index = *bbuff;
                                remap_color = *(remap_color_palettes + (remap_block * (16 * 16)) + remap_block_index);
                                *bbuff = remap_color;
                                bbuff += SCREEN_WIDTH;
                            }
                        }
                        else
                        {
                            while(itr_op_repeat--)
                            {
                                *bbuff = data_byte;
                                bbuff += SCREEN_WIDTH;
                            }
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

                /* ¿  ? */

                while(sequence_byte_count--)
                {
                    // *bbuff = *frame_data++;
                    // data_byte = *data++;
                    data_byte = *frame_data++;
                    if(data_byte >= 232)
                    {
                        remap_block = data_byte - 232;  /* index of picture_remap_color_list[] */
                        remap_block_index = *bbuff;
                        remap_color = *(remap_color_palettes + (remap_block * (16 * 16)) + remap_block_index);
                        *bbuff = remap_color;
                    }
                    else
                    {
                        *bbuff = data_byte;
                    }
                    bbuff += SCREEN_WIDTH;
                }
            } while(packet_byte_count >= 1);
        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: FLIC_Remap_Draw_Frame(x_start = %d, y_start = %d, width = %d, frame_data = %p)\n", __FILE__, __LINE__, x_start, y_start, width, frame_data);
#endif
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



// WZD s30p09
// drake178: LBX_IMG_ClearGraphic()
// MoO2  Module: replace  Replace_Color_All()
void Replace_Color_All(SAMB_ptr pict_seg, uint8_t replacement_color)
{
    int16_t width;
    int16_t height;
    int16_t pict_size;
    uint8_t * src_ptr;
    uint8_t * dst_ptr;
    uint8_t pixel;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Replace_Color_All(pict_seg = %p, replacement_color = %d)\n", __FILE__, __LINE__, pict_seg, replacement_color);
#endif

    width = FLIC_GET_WIDTH(pict_seg);
    height = FLIC_GET_HEIGHT(pict_seg);
    pict_size = width * height;
#ifdef STU_DEBUG
    if(DBG_Draw_Invisibility = 1)  /* Unit Has Invisibility */
    {
        DLOG("(DBG_Draw_Invisibility = 1)");
        dbg_prn("DEBUG: [%s, %d]: width: %d\n", __FILE__, __LINE__, width);
        dbg_prn("DEBUG: [%s, %d]: height: %d\n", __FILE__, __LINE__, height);
        dbg_prn("DEBUG: [%s, %d]: pict_size: %d\n", __FILE__, __LINE__, pict_size);
    }
#endif


    src_ptr = (uint8_t *)(pict_seg + 16);
    dst_ptr = (uint8_t *)(pict_seg + 16);
// DELETE  #ifdef STU_DEBUG
// DELETE      dbg_prn("DEBUG: [%s, %d]: src_ptr: %p\n", __FILE__, __LINE__, src_ptr);
// DELETE      dbg_prn("DEBUG: [%s, %d]: dst_ptr: %p\n", __FILE__, __LINE__, dst_ptr);
// DELETE  #endif

    while(pict_size--)
    {
        pixel = *src_ptr++;
// DELETE  #ifdef STU_DEBUG
// DELETE      dbg_prn("DEBUG: [%s, %d]: pixel: 0x%02X\n", __FILE__, __LINE__, pixel);
// DELETE  #endif
        dst_ptr++;
        if(pixel != ST_TRANSPARENT)
        {
            // DELETE  DLOG("(pixel != ST_TRANSPARENT)");
            dst_ptr--;
            *dst_ptr++ = replacement_color;
        }

    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Replace_Color_All(pict_seg = %p, replacement_color = %d)\n", __FILE__, __LINE__, pict_seg, replacement_color);
#endif

}


// WZD s30p10
// drake178: LBX_IMG_LoadPalette()
// ¿ MoO2  Module: animate  Draw_Palette() ?
/*
    tests 'frames have palettes'
    loads palette from frame 0
*/
void Load_Palette_From_Animation(SAMB_ptr picture)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Load_Palette_From_Animation(picture = %p)\n", __FILE__, __LINE__, picture);
#endif

    // ¿ MEM_Copy_Far(&pict_hdr, 0, 0, picture, 16) ?

    if((FLIC_GET_PALETTE_HEADER_OFFSET(picture) != 0))
    {
        DLOG("((FLIC_GET_PALETTE_HEADER_OFFSET(picture) != 0))");
        FLIC_Load_Palette(picture, 0);
    }
    else
    {
        DLOG("((FLIC_GET_PALETTE_HEADER_OFFSET(picture) == 0))");
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Load_Palette_From_Animation(picture = %p)\n", __FILE__, __LINE__, picture);
#endif
}


// WZD s30p11
// MoO2  Module: animate  Draw(); |-> Remap_Draw(); Draw_No_Glass(); ... Remap_Draw_Animated_Sprite();
void FLIC_Draw(int16_t x_start, int16_t y_start, SAMB_ptr picture)
{
    int16_t current_frame;
    int16_t next_frame;
    unsigned int flic_frame_offset;
    unsigned short flic_frame_offset_sgmt;
    unsigned short flic_frame_offset_ofst;
    byte_ptr p_FLIC_Frame;
    uint8_t remap_flag;

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: BEGIN: FLIC_Draw(x_start = %d, y_start = %d, picture = %p)\n", __FILE__, __LINE__, x_start, y_start, picture);
// #endif

    // ¿ MEM_Copy_Far(&pict_hdr, 0, 0, picture, 16) ?

    current_frame = FLIC_GET_CURRENT_FRAME(picture);
    next_frame = FLIC_GET_CURRENT_FRAME(picture) + 1;
    if(next_frame < FLIC_GET_FRAME_COUNT(picture))
    {
        // DLOG("(next_frame < FLIC_GET_FRAME_COUNT(picture))");
        FLIC_SET_CURRENT_FRAME(picture, next_frame);
    }
    else
    {
        // DLOG("(next_frame >= FLIC_GET_FRAME_COUNT(picture))");
        FLIC_SET_CURRENT_FRAME(picture, FLIC_GET_LOOP_FRAME(picture));
    }

    if((FLIC_GET_PALETTE_HEADER_OFFSET(picture) != 0))
    {
        // DLOG("((FLIC_GET_PALETTE_HEADER_OFFSET(picture) != 0))");
        FLIC_Load_Palette(picture, current_frame);
    }
    else
    {
        // DLOG("((FLIC_GET_PALETTE_HEADER_OFFSET(picture) == 0))");
    }

    /*
        Test EMM_Handle_Number
            FLIC_Draw_Frame_EMM()
    */

    flic_frame_offset = FLIC_GET_FRAME_OFFSET(picture, current_frame);
    p_FLIC_Frame = picture + (flic_frame_offset) + 1;

    remap_flag = FLIC_GET_REMAP_FLAG(picture);

    if(remap_flag == ST_FALSE)
    {
        // DLOG("(remap_flag == ST_FALSE)");
        FLIC_Draw_Frame(x_start, y_start, FLIC_GET_WIDTH(picture), p_FLIC_Frame);
    }
    else
    {
        DLOG("(remap_flag != ST_FALSE)");
        // MoO2  Module: animate  Remap_Draw_Animated_Sprite(x_start, y_start, frame_data)
        FLIC_Remap_Draw_Frame(x_start, y_start, FLIC_GET_WIDTH(picture), p_FLIC_Frame);
    }

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: END: FLIC_Draw(x_start = %d, y_start = %d, picture = %p)\n", __FILE__, __LINE__, x_start, y_start, picture);
// #endif
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Draw_Picture(x = %d, y = %d, pict_seg = %p)\n", __FILE__, __LINE__, x, y, pict_seg);
#endif

    width = GET_2B_OFS(pict_seg, 0);
    height = GET_2B_OFS(pict_seg, 2);

    Draw_Picture_ASM(x, y, SZ_FLIC_HDR, pict_seg, width, height, 0);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Draw_Picture(x = %d, y = %d, pict_seg = %p)\n", __FILE__, __LINE__, x, y, pict_seg);
#endif

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


// WZD s30p42
// drake178: LBX_IMG_OutlineOvr()
// MoO2: Module: bitmap  Outline_Bitmap() |-> Outline_Bitmap_Pixels_()
// checks transparent, outline color  (NOT > 223)
void Outline_Bitmap_Pixels(SAMB_ptr pict_seg, uint8_t outline_color)
{
    int16_t width;
    int16_t height;
    int16_t pict_size;
    uint8_t * src_sgmt;
    uint8_t pixel;
    uint8_t inside_state;
    uint8_t outside_state;
    int16_t itr_height;
    uint16_t src_ofst;
    int16_t itr_pict_size;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Outline_Bitmap_Pixels(pict_seg = %p, outline_color = %02X)\n", __FILE__, __LINE__, pict_seg, outline_color);
#endif
    
    width = FLIC_GET_WIDTH(pict_seg);
    height = FLIC_GET_HEIGHT(pict_seg);
    pict_size = width * height;

    src_sgmt = (uint8_t *)(pict_seg + 16);

    /*
        Height-Wise / Horizontal Axis (horz)
    */
    itr_pict_size = pict_size;
    src_ofst = 0;
    inside_state = ST_FALSE;
    outside_state = ST_FALSE;
    itr_height = 0;
    while(itr_pict_size--)
    {
        pixel = *(src_sgmt + src_ofst++);  // `LODSB`  ; AX = DS:SI++
// DELETE  #ifdef STU_DEBUG
// DELETE      if(DBG_Outline_Bitmap_Pixels_No_Glass == 1)
// DELETE      {
// DELETE          dbg_prn("DEBUG: [%s, %d]: pixel: %02X\n", __FILE__, __LINE__, pixel);
// DELETE      }
// DELETE  #endif

        if(pixel == ST_TRANSPARENT || pixel == outline_color)  /* outside pixel */
        {
            // DELETE  DLOG("(pixel == ST_TRANSPARENT || pixel == outline_color)");
            if(inside_state != ST_FALSE)  /* inside pixel & inside state */
            {
                // DELETE  DLOG("transition: outside pixel, inside state");
                *(src_sgmt + (src_ofst - 1)) = outline_color;
            }
            inside_state = ST_FALSE;
            outside_state = ST_TRUE;
        }
        else  /* inside pixel */
        {
            // DELETE  DLOG("(pixel != ST_TRANSPARENT && pixel != outline_color)");
            if(outside_state != ST_FALSE)
            {
                // DELETE  DLOG("transition: inside pixel, outside state");
                *(src_sgmt + (src_ofst - 2)) = outline_color;
            }
            outside_state = ST_FALSE;
            inside_state = ST_TRUE;
        }

        // @@Increment_Horz
        itr_height++;
        if(height < itr_height)
        {
            itr_height = 0;
            outside_state = ST_FALSE;
            inside_state = ST_FALSE;
        }
    }


    /*
        Width-Wise / Vertical Axis (vert)
    */
    itr_pict_size = pict_size;
    src_ofst = 0;
    inside_state = ST_FALSE;
    outside_state = ST_FALSE;
    while(itr_pict_size--)
    {
        pixel = *(src_sgmt + src_ofst++);  // `LODSB`  ; AX = DS:SI++
// DELETE  #ifdef STU_DEBUG
// DELETE      if(DBG_Outline_Bitmap_Pixels_No_Glass == 1)
// DELETE      {
// DELETE          dbg_prn("DEBUG: [%s, %d]: pixel: %02X\n", __FILE__, __LINE__, pixel);
// DELETE      }
// DELETE  #endif

        if(pixel == ST_TRANSPARENT || pixel == outline_color)  /* outside pixel */
        {
            // Struggle-Mode  DLOG("(pixel == ST_TRANSPARENT || pixel == outline_color)");
            if(inside_state != ST_FALSE)  /* inside pixel & inside state */
            {
                // Struggle-Mode  DLOG("transition: outside pixel, inside state");
                *(src_sgmt + (src_ofst - 1)) = outline_color;
            }
            inside_state = ST_FALSE;
            outside_state = ST_TRUE;
        }
        else  /* inside pixel */
        {
            // Struggle-Mode  DLOG("(pixel != ST_TRANSPARENT && pixel != outline_color)");
            if(outside_state != ST_FALSE)
            {
                // Struggle-Mode  DLOG("transition: inside pixel, outside state");
                src_ofst -= height;  /* previous column */
                *(src_sgmt + (src_ofst - 1)) = outline_color;
                src_ofst += height;
            }
            outside_state = ST_FALSE;
            inside_state = ST_TRUE;
        }

        // @@Increment_Vert
        src_ofst--;
        src_ofst += height;
        if(src_ofst >= pict_size)
        {
            src_ofst -= pict_size;
            src_ofst++;
            outside_state = ST_FALSE;
            inside_state = ST_FALSE;
        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Outline_Bitmap_Pixels(pict_seg = %p, outline_color = %02X)\n", __FILE__, __LINE__, pict_seg, outline_color);
#endif

}


// WZD s30p43
// drake178: LBX_IMG_OutlineOvr_R()
// MoO2: Module: bitmap  Outline_Bitmap() |-> Outline_Bitmap_Pixels_No_Glass_()
// checks transparent, outline color, and > 223
void Outline_Bitmap_Pixels_No_Glass(SAMB_ptr pict_seg, uint8_t outline_color)
{
    int16_t width;
    int16_t height;
    int16_t pict_size;
    uint8_t * src_sgmt;
    uint8_t pixel;
    uint8_t inside_state;
    uint8_t outside_state;
    int16_t itr_height;
    uint16_t src_ofst;
    int16_t itr_pict_size;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Outline_Bitmap_Pixels_No_Glass(pict_seg = %p, outline_color = %02X)\n", __FILE__, __LINE__, pict_seg, outline_color);
#endif
    
    width = FLIC_GET_WIDTH(pict_seg);
    height = FLIC_GET_HEIGHT(pict_seg);
    pict_size = width * height;

    src_sgmt = (uint8_t *)(pict_seg + 16);

    /*
        Height-Wise / Horizontal Axis (horz)
    */
    itr_pict_size = pict_size;
    src_ofst = 0;
    inside_state = ST_FALSE;
    outside_state = ST_FALSE;
    itr_height = 0;
    while(itr_pict_size--)
    {
        pixel = *(src_sgmt + src_ofst++);  // `LODSB`  ; AX = DS:SI++
#ifdef STU_DEBUG
    if(DBG_Outline_Bitmap_Pixels_No_Glass == 1)
    {
        dbg_prn("DEBUG: [%s, %d]: pixel: %02X\n", __FILE__, __LINE__, pixel);
    }
#endif

        if(pixel == ST_TRANSPARENT || pixel == outline_color || pixel >= 224)  /* outside pixel */
        {
            // Struggle-Mode  DLOG("(pixel == ST_TRANSPARENT || pixel == outline_color || pixel >= 224)");
            if(inside_state != ST_FALSE)  /* inside pixel & inside state */
            {
                // Struggle-Mode  DLOG("transition: outside pixel, inside state");
                *(src_sgmt + (src_ofst - 1)) = outline_color;
            }
            inside_state = ST_FALSE;
            outside_state = ST_TRUE;
        }
        else  /* inside pixel */
        {
            // Struggle-Mode  DLOG("(pixel != ST_TRANSPARENT && pixel != outline_color && pixel < 224)");
            if(outside_state != ST_FALSE)
            {
                // Struggle-Mode  DLOG("transition: inside pixel, outside state");
                *(src_sgmt + (src_ofst - 2)) = outline_color;
            }
            outside_state = ST_FALSE;
            inside_state = ST_TRUE;
        }

        // @@Increment_Horz
        itr_height++;
        if(height < itr_height)
        {
            itr_height = 0;
            outside_state = ST_FALSE;
            inside_state = ST_FALSE;
        }
    }


    /*
        Width-Wise / Vertical Axis (vert)
    */
    itr_pict_size = pict_size;
    src_ofst = 0;
    inside_state = ST_FALSE;
    outside_state = ST_FALSE;
    while(itr_pict_size--)
    {
        pixel = *(src_sgmt + src_ofst++);  // `LODSB`  ; AX = DS:SI++
// Struggle-Mode  #ifdef STU_DEBUG
// Struggle-Mode      if(DBG_Outline_Bitmap_Pixels_No_Glass == 1)
// Struggle-Mode      {
// Struggle-Mode          dbg_prn("DEBUG: [%s, %d]: pixel: %02X\n", __FILE__, __LINE__, pixel);
// Struggle-Mode      }
// Struggle-Mode  #endif

        if(pixel == ST_TRANSPARENT || pixel == outline_color || pixel >= 224)  /* outside pixel */
        {
            // Struggle-Mode  DLOG("(pixel == ST_TRANSPARENT || pixel == outline_color || pixel >= 224)");
            if(inside_state != ST_FALSE)  /* inside pixel & inside state */
            {
                // Struggle-Mode  DLOG("transition: outside pixel, inside state");
                *(src_sgmt + (src_ofst - 1)) = outline_color;
            }
            inside_state = ST_FALSE;
            outside_state = ST_TRUE;
        }
        else  /* inside pixel */
        {
            // Struggle-Mode  DLOG("(pixel != ST_TRANSPARENT && pixel != outline_color && pixel < 224)");
            if(outside_state != ST_FALSE)
            {
                // Struggle-Mode  DLOG("transition: inside pixel, outside state");
                src_ofst -= height;  /* previous column */
                *(src_sgmt + (src_ofst - 1)) = outline_color;
                src_ofst += height;
            }
            outside_state = ST_FALSE;
            inside_state = ST_TRUE;
        }

        // @@Increment_Vert
        src_ofst--;
        src_ofst += height;
        if(src_ofst >= pict_size)
        {
            src_ofst -= pict_size;
            src_ofst++;
            outside_state = ST_FALSE;
            inside_state = ST_FALSE;
        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Outline_Bitmap_Pixels_No_Glass(pict_seg = %p, outline_color = %02X)\n", __FILE__, __LINE__, pict_seg, outline_color);
#endif
}


// WZD s30p44
// drake178: LBX_IMG_ColorScrmblr
// MoO2: Bitmap_Aura() |-> Bitmap_Aura_Pixels_()
// replaces a selected color in an LBX image with one of the eight indexes in the passed color array, constantly changing the next one to be written using a 16 color long repeat sequence
void Bitmap_Aura_Pixels(SAMB_ptr pict_seg, uint8_t aura_color, uint8_t * color_list)
{
    int16_t width;
    int16_t height;
    int16_t pict_size;
    uint8_t * pict_sgmt;
    uint16_t pict_ofst;
    uint8_t color_list_idx;
    uint8_t color_list_ctr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Bitmap_Aura_Pixels(pict_seg = %p, aura_color = %02X, color_list = %p)\n", __FILE__, __LINE__, pict_seg, aura_color, color_list);
#endif
    
    width = FLIC_GET_WIDTH(pict_seg);
    height = FLIC_GET_HEIGHT(pict_seg);
    pict_size = width * height;

    pict_sgmt = (uint8_t *)(pict_seg + 16);  // + sizeof(header)
    pict_ofst = 0;

    color_list_idx = 0;
    color_list_ctr = 0;

    while(pict_size--)
    {
        if(*(pict_sgmt + pict_ofst) == aura_color)
        {
            *(pict_sgmt + pict_ofst) = *(color_list + color_list_idx);
        }
        pict_ofst++;
        color_list_idx = ((color_list_idx + color_list_ctr) & 0x7);  // ~== mod 8
        color_list_ctr = ((color_list_ctr + 1) & 0x7);  // ~== mod 8
    }


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Bitmap_Aura_Pixels(pict_seg = %p, aura_color = %02X, color_list = %p)\n", __FILE__, __LINE__, pict_seg, aura_color, color_list);
#endif
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
// MoO2 Draw_Bitmap_Sprite_
void Draw_Picture_ASM(int16_t x_start, int16_t y_start, int16_t ofst, byte_ptr pict_seg, int16_t width, int16_t height, int16_t skip_x)
{
    byte_ptr screen_start;
    byte_ptr screen_pos;
    byte_ptr data;
    uint8_t data_byte;
    int16_t itr_width;
    int16_t itr_height;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Draw_Picture_ASM(x_start = %d,  y_start = %d, ofst = %d, pict_seg = %p, width = %d, height = %d, skip_x = %d)\n", __FILE__, __LINE__, x_start,  y_start,  ofst,  pict_seg,  width,  height,  skip_x);
#endif

    screen_start = current_video_page + (y_start * SCREEN_WIDTH) + x_start;
    data = pict_seg + ofst;
    screen_pos = screen_start;
    itr_width = width;

    if(FLIC_GET_LOOP_FRAME(pict_seg) != 0)
    {
        DLOG("(FLIC_GET_LOOP_FRAME(pict_seg) != 0)");
        Remap_Draw_Picture_ASM(x_start, y_start, ofst, pict_seg, width, height, skip_x);
    }
    else
    {
        DLOG("(FLIC_GET_LOOP_FRAME(pict_seg) == 0)");
        while(itr_width)
        {
            screen_pos = screen_start++;
            itr_height = height;
            while(itr_height)
            {
                data_byte = *data++;
                if(data_byte != ST_TRANSPARENT)
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Draw_Picture_ASM(x_start = %d,  y_start = %d, ofst = %d, pict_seg = %p, width = %d, height = %d, skip_x = %d)\n", __FILE__, __LINE__, x_start,  y_start,  ofst,  pict_seg,  width,  height,  skip_x);
#endif

}


// WZD s33p06
void Remap_Draw_Picture_ASM(int16_t x_start, int16_t y_start, int16_t ofst, byte_ptr pict_seg, int16_t width, int16_t height, int16_t skip_x)
{

    byte_ptr screen_start;
    byte_ptr screen_pos;
    byte_ptr data;
    uint8_t data_byte;
    uint8_t remap_block;
    uint8_t remap_block_index;
    uint8_t remap_color;
    int16_t itr_width;
    int16_t itr_height;
    
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Remap_Draw_Picture_ASM(x_start = %d, y_start = %d, ofst = %d, pict_seg = %p, width = %d, height = %d, skip_x = %d)\n", __FILE__, __LINE__, x_start, y_start, ofst, pict_seg, width, height, skip_x);
#endif

    // // // screen_start = current_video_page + (y_start * SCREEN_WIDTH) + x_start;
    // // screen_page_offset = ((y * SCREEN_WIDTH) + x);
    // // screen_page = video_page_buffer[1 - draw_page_num] + screen_page_offset;
    // // screen_start = video_page_buffer[1 - draw_page_num] + (y_start * SCREEN_WIDTH) + x_start;
    // screen_start = video_page_buffer[draw_page_num] + (y_start * SCREEN_WIDTH) + x_start;
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
// #ifdef STU_DEBUG
//     if(DBG_Remap_Draw_Picture_ASM == 1)
//     {
//         dbg_prn("DEBUG: [%s, %d]: data_byte: %02X\n", __FILE__, __LINE__, data_byte);
//     }
// #endif
            if(data_byte != ST_TRANSPARENT)  /* skip */
            {
                // TODO  ¿ should be `if(data_byte >= 232) ?
                if(data_byte >= 233)  /* remap colors */
                {
                    remap_block = data_byte - 232;  /* index of picture_remap_color_list[] */
                    remap_block_index = *screen_pos;  // MoO2: "screen_data"
                    remap_color = *(remap_color_palettes + (remap_block * (16 * 16)) + remap_block_index);
                    *screen_pos = remap_color;
                }
                else
                {
                    *screen_pos = data_byte;
                }
            }
            screen_pos += SCREEN_WIDTH;  // add screen line stride - same column, one row down
            itr_height--;
        }
        data += skip_x;
        itr_width--;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Remap_Draw_Picture_ASM(x_start = %d, y_start = %d, ofst = %d, pict_seg = %p, width = %d, height = %d, skip_x = %d)\n", __FILE__, __LINE__, x_start, y_start, ofst, pict_seg, width, height, skip_x);
#endif

}


// WZD s33p09
// AKA FLIC_Remap_Color()
// drake178: LBX_IMG_ColorReplace
// MoO2:  Replace_Color()
void Replace_Color(SAMB_ptr pict_seg, uint8_t color_to_replace, uint8_t replacement_color)
{
    int16_t width;
    int16_t height;
    int16_t pict_size;
    uint8_t * src;
    uint8_t * dst;
    uint8_t pixel;

    width = FLIC_GET_WIDTH(pict_seg);
    height = FLIC_GET_HEIGHT(pict_seg);

    pict_size = width * height;

    src = pict_seg + 16;
    dst = pict_seg + 16;

    while(pict_size--)
    {
        pixel = *src++;
        dst++;
        if(pixel == color_to_replace)
        {
            dst--;
            *dst++ = replacement_color;
        }
    }

}


// WZD s33p16
// drake178: LBX_IMG_Grayscale()
// MoO2: Gray_Scale_Bitmap()
/*
e.g.,
Draw_Unit_StatFig()
|-> Gray_Scale_Bitmap(UnitDraw_WorkArea, 1)
*/
void Gray_Scale_Bitmap(SAMB_ptr pict_seg, int16_t color_start)
{
    uint8_t * src_sgmt;
    uint16_t src_ofst;
    uint8_t * dst_sgmt;
    uint16_t dst_ofst;
    int16_t itr;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    int16_t value;
    int16_t width;
    int16_t height;
    int16_t pict_size;
    uint8_t data;
    uint8_t intensity_value;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Gray_Scale_Bitmap(pict_seg = %p, color_start = %d)\n", __FILE__, __LINE__, pict_seg,  color_start);
#endif

    // DS:SI  src  p_Palette[0]
    // ES:DI  dst  Intensity_Scale_Tbl@[0]
    src_sgmt = (uint8_t *)p_Palette;
    src_ofst = 0;
    dst_sgmt = (uint8_t *)Intensity_Scale_Tbl;
    dst_ofst = 0;


    // map 256 values to 16 values
    // value = (150% red, 150% green, 100% blue)
    // value >> 4
    itr = 256;
    while(itr--)
    {
        value = 0;
        red = *(src_sgmt + src_ofst++);
        value += red;
        value += red / 2;
        green = *(src_sgmt + src_ofst++);
        value += green;
        value += green / 2;
        blue = *(src_sgmt + src_ofst++);
        value += blue;
        // DNE  value += blue / 2;
        // value >>= 4
        // value /= 16
        value = value >> 4;  // ¿ reduce 2^8 to 2^4 ? map 256 to 16
        value += color_start;  // ~== where to start block in color-map?
        *(dst_sgmt + dst_ofst++) = value;
    }

    src_sgmt = (uint8_t *)Intensity_Scale_Tbl;
    dst_sgmt = (uint8_t *)pict_seg;

    width = FLIC_GET_WIDTH(pict_seg);
    height = FLIC_GET_HEIGHT(pict_seg);
    pict_size = width * height;

    src_ofst = 16;
    dst_ofst = 16;

    while(pict_size--)
    {
        data = *(dst_sgmt + src_ofst);  // get color-map index from picture
        if(data != ST_TRANSPARENT)
        {
            // *(dst_sgmt + dst_ofst++) = *(src_sgmt + data);  // replace picture's color-map index from intensity table, indexed using color-map index from picture
            intensity_value = *(src_sgmt + data);
            *(dst_sgmt + dst_ofst++) = intensity_value;
            src_ofst++;
        }
        else
        {
            src_ofst++;
            dst_ofst++;
        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Gray_Scale_Bitmap(pict_seg = %p, color_start = %d)\n", __FILE__, __LINE__, pict_seg,  color_start);
#endif

}
