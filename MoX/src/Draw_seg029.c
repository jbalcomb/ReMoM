/*
    MAGIC.EXE
        seg029
    WIZARDS.EXE
        seg029
            Picture Drawing

*/

#include "FLIC_Draw.h"
#include "MOX_TYPE.h"
#include "EMS/EMS.h"

#include "Draw_seg029.h"



/*
    WZD  seg029
*/

// WZD s29p01
// AKA Draw_Frame_ASM()
// 1oom  lbxgfx_draw_pixels_fmt0()
/*

frame_data
    pointer to buffer of FLIC animation frame

200 lines of 320 pixels  64,000 pixels at 1 byte per pixel  64,000 bytes of data
((SCREEN_YMIN * SCREEN_WIDTH) + SCREEN_XMIN)  ((  0 * 320) +   0)      0
((SCREEN_YMAX * SCREEN_WIDTH) + SCREEN_XMAX)  ((199 * 320) + 319)  63999

...on the last byte...
the address of bbuff is the address of the last pixel/byte in current_video_page
but, we still add SCREEN_WIDTH, one last time
so, testing at that point is useless

*/
void FLIC_Draw_Frame(int16_t x_start, int16_t y_start, int16_t width, byte_ptr frame_data, int16_t DBG_height)
{
    unsigned char * bbuff_pos = 0;
    unsigned char * bbuff = 0;
    unsigned char data_byte = 0;

    unsigned char packet_op = 0;
    unsigned char packet_byte_count = 0;
    unsigned char sequence_byte_count = 0;
    unsigned char delta_byte_count = 0;
    unsigned char itr_op_repeat = 0;

    byte_ptr DBG_frame_data = 0;
    // byte_ptr DBG_frame_data_end = 0;
    //uint16_t DBG_frame_data_pos = 0;
    unsigned char * DBG_bbuff_pos = 0;
    unsigned char * DBG_bbuff_pos_end = 0;
    unsigned char * DBG_bbuff = 0;
    unsigned char* DBG_bbuff_end = 0;

    assert(x_start >= SCREEN_XMIN);
    assert(x_start <= SCREEN_XMAX);
    assert(y_start >= SCREEN_YMIN);
    assert(y_start <= SCREEN_YMAX);
    assert((x_start + (width - 1)) <= SCREEN_XMAX);
    assert((y_start + (DBG_height - 1)) <= SCREEN_YMAX);

    DBG_frame_data = frame_data;
    // DBG_frame_data_end = (DBG_frame_data + (width * DBG_height));

    bbuff_pos = current_video_page + ((y_start * SCREEN_WIDTH) + x_start);
    DBG_bbuff_pos = bbuff_pos;
    DBG_bbuff_pos_end = current_video_page + (((SCREEN_YMAX * SCREEN_WIDTH) + SCREEN_XMAX) - SCREEN_HEIGHT);
    DBG_bbuff_end = current_video_page + ((SCREEN_YMAX * SCREEN_WIDTH) + SCREEN_XMAX);

    while(width--)
    {
        bbuff = bbuff_pos++;  // next column
        assert(bbuff <= DBG_bbuff_end);

        packet_op = *frame_data++;
        // DBG_frame_data_pos = (frame_data - DBG_frame_data);
        // assert(frame_data <= DBG_frame_data_end);

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
                assert(bbuff <= DBG_bbuff_end);
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
                            assert(bbuff <= DBG_bbuff_end);
                            *bbuff = data_byte;
                            bbuff += SCREEN_WIDTH;
                        }
                    }
                    else  /* op: copy */
                    {
                        assert(bbuff <= DBG_bbuff_end);
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
                assert(bbuff <= DBG_bbuff_end);
                packet_byte_count -= sequence_byte_count + 2;
                while(sequence_byte_count--) {
                    assert(bbuff <= DBG_bbuff_end);
                    *bbuff = *frame_data++;
                    bbuff += SCREEN_WIDTH;
                }
            } while(packet_byte_count >= 1);
        }
    }

}


// WZD s29p02
// MoO2  Module: animate  Remap_Draw() |-> Remap_Draw_Animated_Sprite(); ... Module: remap  unsigned int picture_remap_color_list[256]  Address: 02:001B479C
// 1oom  lbxgfx_draw_pixels_fmt1()
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
void FLIC_Remap_Draw_Frame(int16_t x_start, int16_t y_start, int16_t width, byte_ptr frame_data, int16_t DBG_height)
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

    byte_ptr DBG_frame_data;
    byte_ptr DBG_frame_data_end;
    uint16_t DBG_frame_data_pos;

    DBG_frame_data = frame_data;
    DBG_frame_data_end = (DBG_frame_data + (width * DBG_height));

    bbuff_pos = current_video_page + ((y_start * SCREEN_WIDTH) + x_start);
//     screen_start = current_video_page + (y_start * SCREEN_WIDTH) + x_start;
// 
//     data = pict_seg + ofst;
//     screen_pos = screen_start;
//     itr_width = width;

    while(width--)
    {
        bbuff = bbuff_pos++;

        packet_op = *frame_data++;  // Frame Byte #1: Op/Count
        DBG_frame_data_pos = (uint16_t)(frame_data - DBG_frame_data);  // NOTE pointers, so __int64
        // assert(frame_data <= DBG_frame_data_end);

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

}
