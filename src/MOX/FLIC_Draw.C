/*
    WIZARDS.EXE
        seg030
        seg031
        seg032
        seg033
        seg034

seg030    
MoO2 Module: bitmap

seg033
MoO2 Module: bitmap
MoO2 Module: shear

*/

#include "FLIC_Draw.H"

#include "Fonts.H"
#include "Graphics.H"
#include "MOX_TYPE.H"

#include "malloc.h"



/*
    Video
*/
// WZD dseg:76EC
extern uint8_t* current_video_page;
// WZD s28p02
void Set_Page_Off(void);



// WZD dseg:E860                                                 BEGIN: seg030

// WZD dseg:E860
char file_animation_file_name[16];

// WZD dseg:E870
int16_t file_animation_entry_num;

// WZD dseg:E870                                                 END: seg030



/*
    WZD  seg021
*/

// WZD s21p07
// ¿ MoO2  Module: animate  Set_Animation_Palette() ?
// Load_Animation_Palette()
// ¿ vs. MoO2  Draw_Palette() ?
void FLIC_Load_Palette(SAMB_ptr p_FLIC_Header, int16_t frame_index)
{
    SAMB_ptr p_FLIC_File;
    int16_t start;
    int16_t count;
    byte_ptr flic_palette_data;
    int16_t itr;
    uint16_t DBG_flic_palette_header_frame_palettes;
    uint16_t DBG_frame_palette_data_offset;
    uint16_t DBG_flic_palette_data_offset;

    p_FLIC_File = (p_FLIC_Header + 0);  // ~== p_FLIC_File = &p_FLIC_Header[0]

    DBG_flic_palette_header_frame_palettes = FLIC_GET_FRAME_PALETTES(p_FLIC_File);
    if((frame_index == 0) || (FLIC_GET_FRAME_PALETTES(p_FLIC_File) == 0))
    {
        start = FLIC_GET_PALETTE_COLOR_START(p_FLIC_File);
        count = FLIC_GET_PALETTE_COLOR_COUNT(p_FLIC_File);
        flic_palette_data = (p_FLIC_File + FLIC_GET_PALETTE_DATA_OFFSET(p_FLIC_File));
    }
    else
    {
        DBG_frame_palette_data_offset = FLIC_GET_FRAME_PALETTE_DATA_OFFSET(p_FLIC_File, frame_index);
        DBG_flic_palette_data_offset = FLIC_GET_PALETTE_DATA_OFFSET(p_FLIC_File);
        flic_palette_data = (p_FLIC_File + FLIC_GET_FRAME_PALETTE_DATA_OFFSET(p_FLIC_File,frame_index));
        start = FLIC_GET_FRAME_PALETTE_COLOR_INDEX(p_FLIC_File,frame_index);
        count = FLIC_GET_FRAME_PALETTE_COLOR_COUNT(p_FLIC_File,frame_index);
        flic_palette_data = (p_FLIC_File + FLIC_GET_PALETTE_DATA_OFFSET(p_FLIC_File));
        start = FLIC_GET_PALETTE_COLOR_START(p_FLIC_File);
        count = FLIC_GET_PALETTE_COLOR_COUNT(p_FLIC_File);
    }

    // // for(itr = start; itr < count; itr++)
    // for(itr = start; itr < (start + count); itr++)
    // {
    //     *(current_palette + itr) = *(flic_palette_data + itr);
    //     *(current_palette + 768 + itr) = 1;
    // }
    for(itr = 0; itr < count; itr++)
    {

        // DASM: MOVSW; MOVSB;
        // ¿ ~== ? SET_2B, SET_1B ... rvr++;
        *(current_palette + (start * 3) + (itr * 3) + 0) = *(flic_palette_data + (itr * 3) + 0);
        *(current_palette + (start * 3) + (itr * 3) + 1) = *(flic_palette_data + (itr * 3) + 1);
        *(current_palette + (start * 3) + (itr * 3) + 2) = *(flic_palette_data + (itr * 3) + 2);

        *(current_palette + 768 + start + itr) = ST_TRUE;  // TODO  review, remaster, add manifest-constant for palette change flags offset
    }

}



/*
    WZD seg024
*/

// WZD s24p08
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

    Capture_Screen_Block((pict_seg + SZ_FLIC_HDR), x1, y1, x2, y2);

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
                while(sequence_byte_count--)
                {
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
        DBG_frame_data_pos = (frame_data - DBG_frame_data);
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



/*
    WZD seg030
*/


// WZD s30p01
// drake178: ¿UU_LBX_Image_Copy() or LBX_Image_Copy() ?
// MoO2  Module: bitmap  Copy_Bitmap_To_Bitmap()
// 1oom 
/*
; copies an LBX composed or decoded image to an
; arbitrary far memory segment address (including the
; header)
*/
/*

*/
void Copy_Bitmap_To_Bitmap(SAMB_ptr target_bitmap, SAMB_ptr source_bitmap)
{
    int16_t width = 0;
    int16_t height = 0;
    int16_t length = 0;

    // DOMSDOS  width = farpeekw(source_bitmap, FLIC_HDR_POS_WIDTH);
    width = GET_2B_OFS(source_bitmap, FLIC_HDR_POS_WIDTH);

    // height = farpeekw(source_bitmap, FLIC_HDR_POS_HEIGHT);
    height = GET_2B_OFS(source_bitmap, FLIC_HDR_POS_HEIGHT);

    length = (SZ_FLIC_HDR + (width * height));

    memcpy(target_bitmap, source_bitmap, length);

}


// WZD s30p02
// AKA  FLIC_Prepare()
/*
creates a decoded image header into the specified
segment, and zeroes out the bytes required to hold it
ST_FLIC.H
*/
/*
    ¿ Create_Picture() vs. Create_Blank_Picture() ?
    same header values, just default transparent vs. specified color
*/
void Create_Picture(int16_t width, int16_t height, byte_ptr pict_seg)
{
    uint16_t length = 0;
    uint16_t counter = 0;
    byte_ptr dst_sgmt;
    uint16_t dst_ofst;

    // TODO(JimBalcomb,20230701): fix up the Macros
    // FLIC_SET_WIDTH(pict_seg, width);
    // FLIC_SET_HEIGHT(pict_seg, height);
    // FLIC_SET_CURRENT_FRAME(pict_seg,0xDE0A);  // LBX_DecodedIMG
    // FLIC_SET_FRAME_COUNT(pict_seg, 0);
    // FLIC_SET_LOOP_FRAME(pict_seg,0);

    SET_2B_OFS(pict_seg, 0, 0xBBBB);  // DNE in Dasm
    SET_2B_OFS(pict_seg, 2, 0xBBBB);  // DNE in Dasm
    SET_2B_OFS(pict_seg, 4, 0xBBBB);  // DNE in Dasm
    SET_2B_OFS(pict_seg, 6, 0xBBBB);  // DNE in Dasm
    SET_2B_OFS(pict_seg, 8, 0xBBBB);  // DNE in Dasm
    SET_2B_OFS(pict_seg, 10, 0xBBBB);  // DNE in Dasm
    SET_2B_OFS(pict_seg, 12, 0xBBBB);  // DNE in Dasm
    SET_2B_OFS(pict_seg, 14, 0xBBBB);  // DNE in Dasm

    SET_2B_OFS(pict_seg, 0, width);
    SET_2B_OFS(pict_seg, 2, height);
    SET_2B_OFS(pict_seg, 4, 0xDE0A);  /* e_FLIC_Decoded */
    SET_2B_OFS(pict_seg, 6, 0);
    SET_2B_OFS(pict_seg, 8, 0);

    length = width * height;

    // rep stosb
    dst_sgmt = pict_seg;
    dst_ofst = SZ_FLIC_HDR;
    counter = length;
    while(counter > 0)
    {
        *(dst_sgmt + dst_ofst) = ST_TRANSPARENT;  /* Color-Map Index 0 */
        dst_ofst++;
        counter--;
    }

}

// WZD s30p06
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
        *(pict_seg + SZ_FLIC_HDR + itr_length) = color;
    }

}



// WZD s30p09
// MoO2  Module: replace  Replace_Color_All()
/*
    replaces all colors with replacement color, excluding transparent
*/
void Replace_Color_All(SAMB_ptr pict_seg, uint8_t replacement_color)
{
    int16_t width;
    int16_t height;
    int16_t pict_size;
    uint8_t * src_ptr;
    uint8_t * dst_ptr;
    uint8_t pixel;

    width = FLIC_GET_WIDTH(pict_seg);
    height = FLIC_GET_HEIGHT(pict_seg);
    pict_size = width * height;

    src_ptr = (uint8_t *)(pict_seg + SZ_FLIC_HDR);
    dst_ptr = (uint8_t *)(pict_seg + SZ_FLIC_HDR);

    while(pict_size--)
    {
        pixel = *src_ptr++;
        dst_ptr++;
        if(pixel != ST_TRANSPARENT)
        {
            dst_ptr--;
            *dst_ptr++ = replacement_color;
        }

    }

}


// WZD s30p10
// ¿ MoO2  Module: animate  Draw_Palette() ?
/*
    tests 'frames have palettes'
    loads palette from frame 0
*/
void Load_Palette_From_Animation(SAMB_ptr picture)
{
    struct s_FLIC_HDR animation_header;

    memcpy(&animation_header, picture, sizeof(struct s_FLIC_HDR));

    if(animation_header.palette_header_offset != 0)
    {
        FLIC_Load_Palette(picture, 0);
    }

}


// WZD s30p11
// MoO2  Module: animate  Draw(); |-> Remap_Draw(); Draw_No_Glass(); ... Remap_Draw_Animated_Sprite();
/*

picture
    pointer to buffer of FLIC animation file

*/
void FLIC_Draw(int16_t x_start, int16_t y_start, SAMB_ptr picture)
{
    struct s_FLIC_HDR animation_header;
    int16_t current_frame;
    int16_t next_frame;
    unsigned int frame_offset;
    unsigned short flic_frame_offset_sgmt;
    unsigned short flic_frame_offset_ofst;
    byte_ptr source_start;
    uint8_t remap_flag;
    int16_t DBG_width;
    int16_t DBG_height;

    assert(picture != NULL);
    assert(x_start >= SCREEN_XMIN);
    assert(x_start <= SCREEN_XMAX);
    assert(y_start >= SCREEN_YMIN);
    assert(y_start <= SCREEN_YMAX);
    DBG_width = FLIC_GET_WIDTH(picture);
    DBG_height = FLIC_GET_HEIGHT(picture);
    assert(DBG_width <= SCREEN_WIDTH);
    assert(DBG_height <= SCREEN_HEIGHT);
    assert((x_start + (DBG_width - 1)) <= SCREEN_XMAX);
    assert((y_start + (DBG_height - 1)) <= SCREEN_YMAX);

    memcpy(&animation_header, picture, sizeof(struct s_FLIC_HDR));

    current_frame = animation_header.current_frame;
    animation_header.current_frame++;
    if(animation_header.current_frame >= animation_header.frame_count)
    {
        // DOMSDOS  farpokew(picture, FLIC_HDR_POS_CURRENT_FRAME, animation_header.loop_frame);
        FLIC_SET_CURRENT_FRAME(picture, animation_header.loop_frame);
    }
    else
    {
        // DOMSDOS  farpokew(picture, FLIC_HDR_POS_CURRENT_FRAME, animation_header.current_frame);
        FLIC_SET_CURRENT_FRAME(picture, animation_header.current_frame);
    }

    if(animation_header.palette_header_offset != 0)
    {
        FLIC_Load_Palette(picture, current_frame);
    }

    // DOMSDOS  if(animation_header.emm_handle_number != 0)
    // DOMSDOS  {
    // DOMSDOS      FLIC_Draw_Frame_EMM()
    // DOMSDOS  }

    frame_offset = FLIC_GET_FRAME_OFFSET(picture, current_frame);
    source_start = picture + (frame_offset + 1);

    remap_flag = FLIC_GET_REMAP_FLAG(picture);

    if(remap_flag == ST_FALSE)
    {
        FLIC_Draw_Frame(x_start, y_start, animation_header.width, source_start, DBG_height);
    }
    else
    {
        // MoO2  Module: animate  Remap_Draw_Animated_Sprite(x_start, y_start, frame_data)
        FLIC_Remap_Draw_Frame(x_start, y_start, animation_header.width, source_start, DBG_height);
    }

}

// WZD s30p12
// MoO2  
void Clipped_Draw(int16_t x, int16_t y, SAMB_ptr picture)
{
    struct s_FLIC_HDR l_animation_header;
// var_16= word ptr -16h
// frame_offset_sgmt= word ptr -14h
// frame_offset_ofst= word ptr -12h
    int16_t skip_y;
    int16_t skip_x;
    int16_t actual_height;
    int16_t actual_width;
    int16_t y2;
    int16_t x2;
    int16_t current_frame;
    int16_t remap_flag;

    int16_t start_x;
    int16_t start_y;

    uint16_t frame_offset;
    byte_ptr frame_data;

    if((x > screen_window_x2) || (y > screen_window_y2))
    {
        return;
    }
    // // _fmemcpy(animation_header, 0, 0, picture, 16)
    // memcpy(&animation_header, picture, 16);
    // x2 = x + animation_header.width - 1;
    // if (x2 < screen_window_x1)
    // {
    //     return;
    // }
    // y2 = y + animation_header.height - 1;
    // if(y2 < screen_window_y1)
    // {
    //    return;
    // }


    // _fmemcpy(animation_header, 0, 0, picture, 16)
    memcpy(&l_animation_header, picture, 16);

    x2 = x + l_animation_header.width - 1;

    if(x2 < screen_window_x1)
    {
        return;
    }

    y2 = y + l_animation_header.height - 1;

    if(y2 < screen_window_y1)
    {
        return;
    }

    if(x < screen_window_x1)
    {
        skip_x = screen_window_x1 - x;
        start_x = screen_window_x1;
    }
    else
    {
        skip_x = 0;
        start_x = x;
    }

    if(y < screen_window_y1)
    {
        skip_y = screen_window_y1 - y;
        start_y = screen_window_y1;
    }
    else
    {
        skip_y = 0;
        start_y = y;
    }

    if(x2 >= screen_window_x2)
    {
        actual_width = screen_window_x2 - start_x + 1;
    }
    else
    {
        actual_width = x2 - start_x + 1;
    }

    if(y2 >= screen_window_y2)
    {
        actual_height = screen_window_y2 - start_y + 1;
    }
    else
    {
        actual_height = y2 - start_y + 1;
    }

    current_frame = l_animation_header.current_frame;
    l_animation_header.current_frame += 1;

    if(l_animation_header.current_frame < l_animation_header.frame_count)
    {
        FLIC_SET_CURRENT_FRAME(picture, l_animation_header.current_frame);
    }
    else
    {
        FLIC_SET_CURRENT_FRAME(picture, l_animation_header.loop_frame);
    }


    // if((FLIC_GET_PALETTE_HEADER_OFFSET(picture) != 0))
    if(l_animation_header.palette_header_offset != 0)
    {
        FLIC_Load_Palette(picture, current_frame);
    }



    /*
        Test EMM_Handle_Number
            VGA_DrawPartEmsImg()
    */



    frame_offset = FLIC_GET_FRAME_OFFSET(picture, current_frame);
    frame_data = picture + frame_offset + 1;

    remap_flag = FLIC_GET_REMAP_FLAG(picture);

    if(remap_flag == ST_FALSE)
    {
        Clipped_Draw_Frame(start_x, start_y, actual_width, actual_height, skip_x, skip_y, frame_data);
    }
    else
    {
        // MoO2  Module: animate  Remap_Draw_Animated_Sprite(x_start, y_start, frame_data)
        // TODO  VGA_DrawPartImage_R(start_x, start_y, FLIC_GET_WIDTH(picture), p_FLIC_Frame);
        // TODO  Clipped_Remap_Draw_Frame__NOP(start_x, start_y, actual_width, actual_height, skip_x, skip_y, frame_data);
        Clipped_Draw_Frame(start_x, start_y, actual_width, actual_height, skip_x, skip_y, frame_data);
    }

}


// WZD s30p13
// NOTE: Draw_Picture_To_Bitmap(SAMB_ptr src_pict_seg) ~== FLIC_Draw(SAMB_ptr p_FLIC_File)
void Draw_Picture_To_Bitmap(SAMB_ptr source_picture, SAMB_ptr destination_bitmap)
{
    struct s_FLIC_HDR animation_header;
    int16_t current_frame;
    int8_t full_store_flag;
    int16_t frame_start;
    int16_t frame_idx;
    int16_t itr_frames;
    uint32_t frame_offset;
    byte_ptr source_start;
    int16_t loop_frame_index;  // DNE in Dasm
    int16_t frame_count;  // DNE in Dasm

    /*
        BEGIN: same as FLIC_Draw()
    */
    memcpy(&animation_header, source_picture, sizeof(struct s_FLIC_HDR));

    current_frame = animation_header.current_frame;
    animation_header.current_frame++;
    if(animation_header.current_frame >= animation_header.frame_count)
    {
        // DOMSDOS  farpokew(picture, FLIC_HDR_POS_CURRENT_FRAME, animation_header.loop_frame);
        FLIC_SET_CURRENT_FRAME(source_picture, animation_header.loop_frame);
    }
    else
    {
        // DOMSDOS  farpokew(picture, FLIC_HDR_POS_CURRENT_FRAME, animation_header.current_frame);
        FLIC_SET_CURRENT_FRAME(source_picture, animation_header.current_frame);
    }

    if(animation_header.palette_header_offset != 0)
    {
        FLIC_Load_Palette(source_picture, current_frame);
    }
    /*
        END: same as FLIC_Draw()
    */

   Create_Multi_Frame_Blank_Bitmap(source_picture, destination_bitmap, current_frame);

    // DOMSDOS  if(animation_header.emm_handle_number != 0)
    // DOMSDOS  {
    // DOMSDOS      Add_Picture_To_Bitmap_EMM
    // DOMSDOS  }
    
    // DOMSDOS  full_store_flag = farpeekb(source_picture, sizeof(struct s_FLIC_HDR));
    full_store_flag = GET_1B_OFS(source_picture, sizeof(struct s_FLIC_HDR));

    if(full_store_flag != ST_FALSE)
    {
        frame_start = current_frame;
    }
    else
    {
        frame_start = 0;
    }

    frame_idx = current_frame;

    for(itr_frames = frame_start; itr_frames <= frame_idx; itr_frames++)
    {
        /*
            BEGIN: same as FLIC_Draw()
        */
        frame_offset = FLIC_GET_FRAME_OFFSET(source_picture, itr_frames);
        source_start = (source_picture + (frame_offset + 1));
        /*
            END: same as FLIC_Draw()
        */

        Add_Picture_To_Bitmap(source_start, destination_bitmap);

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
    int16_t current_frame;
    current_frame = FLIC_GET_CURRENT_FRAME(p_FLIC_Header);
    return current_frame;
}


// WZD s30p17
int16_t FLIC_Get_FrameCount(SAMB_ptr p_FLIC_Header)
{
    int16_t frame_count;
    frame_count = FLIC_GET_FRAME_COUNT(p_FLIC_Header);
    return frame_count;
}


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


// WZD s30p20
// VGA_WndDrawRotateImg()

// WZD s30p21
// VGA_WndDrawImageRect()

// WZD s30p22
// UU_VGA_WndDrawTransform()

// WZD s30p23
// VGA_RotateRect()


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

    // Test - completely off on the right or completely off on the bottom
    if((x1 > screen_window_x2) || (y1 > screen_window_y2))
    {
        return;
    }

    width = GET_2B_OFS(pict_seg, 0);
    height = GET_2B_OFS(pict_seg, 2);

    x2 = x1 + width - 1;
    // Test - completely off on the left
    if(x2 < screen_window_x1)
    {
        return;
    }
    
    y2 = y1 + height - 1;
    // Test - completely off on the top
    if(y2 < screen_window_y1)
    {
        return;
    }

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


// WZD s30p26
// MoO2  Module: bitmap  Mask_Out_Bitmap_With_Bitmap() |-> Mask_Bitmap_Pixels_()
/*
    no RLE
        so, ~"bitmap"
    is "Clipped"
    accomodates transparent pixels
        so, "sprite"?

MoO2  Module: bitmap   Copy_Bitmap_To_Bitmap()
MoO2  Module: draw     Color_Stream_Copy_()
MoO2  Module: draw     Draw_Bitmap_Sprite_()
MoO2  Module: clipped  Clipped_Draw_Bitmap_Sprite()
Draw()
    |-> Clipped_Draw_Bitmap_Sprite()

So, ...
    ¿ ~ Clipped Copy Bitmap Sprite ?

Draw_Item_With_Name()
    |-> Draw_Item_Icon_With_Enchantment_Outline(item_idx, m_item_icon_workarea)
Draw_Item_Icon_With_Enchantment_Outline(int16_t item_idx, SAMB_ptr item_icon_pict_seg)
    |-> Draw_Picture_To_Bitmap(item_icons_seg[_ITEMS[item_idx].icon_idx], GfxBuf_2400B);
    |-> Create_Picture(19, 19, item_icon_pict_seg);
    |-> Clipped_Copy_Bitmap(2, 2, item_icon_pict_seg, GfxBuf_2400B);

*/
/*
doesn't copy transparent pixels from the src
so, the dst keeps those pixels of its own

Mask_Out_Bitmap_With_Bitmap() vs. Mask_Out_Bitmap_With_Bitmap_Inverse()
...inverse only copies the transparent pixels

probably, definitely should be Mask_Bitmap_Pixels() and Mask_Bitmap_Pixels_ASM()
*/
void Clipped_Copy_Bitmap(int16_t x, int16_t y, byte_ptr dst_pict_seg, byte_ptr src_pict_seg)
{
    int16_t dst_ofst;
    int16_t src_ofst;
    int16_t cwidth;
    int16_t cy1;
    int16_t cx1;
    int16_t skip_y;
    int16_t skip_x;
    int16_t dst_width;
    int16_t dst_skip_y;
    int16_t src_skip_y;
    int16_t y2;
    int16_t x2;
    int16_t dst_height;  // _SI_
    int16_t cheight;  // _DI_
    byte_ptr dst;  // DNE in Dasm
    byte_ptr src;  // DNE in Dasm

    dst_width = FLIC_GET_WIDTH(dst_pict_seg);
    dst_height = FLIC_GET_HEIGHT(dst_pict_seg);

    if(((dst_width - 1) < x) || ((dst_height - 1) < y))
    {
        return;
    }

    x2 = (x + FLIC_GET_WIDTH(src_pict_seg) - 1);

    if(x2 < 0)
    {
        return;
    }

    y2 = (y + FLIC_GET_HEIGHT(src_pict_seg) - 1);

    if(y2 < 0)
    {
        return;
    }

    if(x < 0)
    {
        skip_x = (-x);
        cx1 = 0;
    }
    else
    {
        skip_x = 0;
        cx1 = x;
    }

    if(y < 0)
    {
        skip_y = (-y);
        cy1 = 0;
    }
    else
    {
        skip_y = 0;
        cy1 = y;
    }

    if((dst_width - 1) > x2)
    {
        cwidth = (x2 - cx1 + 1);
    }
    else
    {
        cwidth = (dst_width - cx1);
    }

    if(cwidth > dst_width)
    {
        cwidth = dst_width;
    }

    if((dst_height - 1) > y2)
    {
        cheight = (y2 - cy1 + 1);
    }
    else
    {
        cheight = (dst_height - cy1);
    }

    if(cheight > dst_height)
    {
        cheight = dst_height;
    }

    dst_ofst = (sizeof(struct s_FLIC_HDR) + ((cx1 * dst_height) + cy1));
    dst_skip_y = (dst_height - cheight);

    src_ofst = (sizeof(struct s_FLIC_HDR) + (skip_x * FLIC_GET_HEIGHT(src_pict_seg)) + skip_y);
    src_skip_y = (FLIC_GET_HEIGHT(src_pict_seg) - cheight);

    dst = (dst_pict_seg + dst_ofst);
    src = (src_pict_seg + src_ofst);

    Mask_Bitmap_Pixels(dst, src, dst_skip_y, src_skip_y, cwidth, cheight);

}


// WZD s30p27
// drake178: LBX_IMG_Overlay()
// MoO2  Module: bitmap  Mask_Out_Bitmap_With_Bitmap() |-> Mask_Bitmap_Pixels_Inverse_()
/*
; overlays one LBX image onto another, trimming the
; source if necessary; Left and Top are the coordinates
; in the target image, the source is always wholly
; copied if possible
*/
/*
Usage:
Warped Node
    Draw_Map_Nodes()

Overland Spell Animation
    OVL_LoadGlobalAnim()
        // SPECFX.LBX, 054  "MASK1"     ""
        GAME_MP_SpellVar_2 = LBX_Reload_Next(specfx_lbx_file__ovr137, 54, _screen_seg);
        OR
        // SPECFX.LBX, 055  "MASK2"     ""
        GAME_MP_SpellVar_2 = LBX_Reload_Next(specfx_lbx_file__ovr137, 55, _screen_seg);
    OVL_DrawGlobalAnim()
        Draw_Picture_To_Bitmap(ge_anim_moodwiz_seg, IMG_SBK_PageText);
        Clipped_Copy_Mask(0, 0, IMG_SBK_PageText, IMG_SBK_Anims);
        Draw_Picture((start_x + 12), (start_y + 12), IMG_SBK_PageText);

*/
/*
    IDGI

XREF:
    OVL_DrawGlobalAnim()
    Draw_Map_Nodes()

probably, definitely should be Mask_Bitmap_Pixels_Inverse() and Mask_Bitmap_Pixels_Inverse_ASM()

*/
void Clipped_Copy_Mask(int16_t x, int16_t y, SAMB_ptr target_bitmap, SAMB_ptr mask_bitmap)
{

    int16_t dst_ofst;
    int16_t src_ofst;
    int16_t cwidth;
    int16_t cy1;
    int16_t cx1;
    int16_t skip_y;
    int16_t skip_x;
    int16_t dst_width;
    int16_t dst_skip_y;
    int16_t src_skip_y;
    int16_t y2;
    int16_t x2;
    int16_t dst_height;  // _SI_
    int16_t cheight;  // _DI_
    byte_ptr dst;  // DNE in Dasm
    byte_ptr src;  // DNE in Dasm

    dst_width = FLIC_GET_WIDTH(mask_bitmap);
    dst_height = FLIC_GET_HEIGHT(mask_bitmap);

    if(((dst_width - 1) < x) || ((dst_height - 1) < y))
    {
        return;
    }

    x2 = (x + FLIC_GET_WIDTH(target_bitmap) - 1);

    if(x2 < 0)
    {
        return;
    }

    y2 = (y + FLIC_GET_HEIGHT(target_bitmap) - 1);

    if(y2 < 0)
    {
        return;
    }

    if(x < 0)
    {
        skip_x = (-x);
        cx1 = 0;
    }
    else
    {
        skip_x = 0;
        cx1 = x;
    }

    if(y < 0)
    {
        skip_y = (-y);
        cy1 = 0;
    }
    else
    {
        skip_y = 0;
        cy1 = y;
    }

    if((dst_width - 1) > x2)
    {
        cwidth = (x2 - cx1 + 1);
    }
    else
    {
        cwidth = (dst_width - cx1);
    }

    if(cwidth > dst_width)
    {
        cwidth = dst_width;
    }

    if((dst_height - 1) > y2)
    {
        cheight = (y2 - cy1 + 1);
    }
    else
    {
        cheight = (dst_height - cy1);
    }

    if(cheight > dst_height)
    {
        cheight = dst_height;
    }

    dst_ofst = (sizeof(struct s_FLIC_HDR) + ((cx1 * dst_height) + cy1));
    dst_skip_y = (dst_height - cheight);

    src_ofst = (sizeof(struct s_FLIC_HDR) + (skip_x * FLIC_GET_HEIGHT(target_bitmap)) + skip_y);
    src_skip_y = (FLIC_GET_HEIGHT(target_bitmap) - cheight);

    dst = (mask_bitmap + dst_ofst);
    src = (target_bitmap + src_ofst);

    Mask_Bitmap_Pixels_Inverse(dst, src, dst_skip_y, src_skip_y, cwidth, cheight);

}


// WZD s30p28
// drake178: LBX_IMG_StripColors()
// ¿ MoO2  Module: bitmap  Mask_Out_Colors() ?
// ¿ Replace_Color_Range() ?
/*
; replaces any colors between the specified two
; (inclusive) with the overlay color (index #00) in an
; LBX image
*/
/*

*/
void Transparent_Color_Range(SAMB_ptr bitmap, int16_t color_start, int16_t color_end)
{
    int16_t height = 0;
    int16_t width = 0;
    int16_t bitmap_size = 0;
    byte_ptr src_sgmt;  // _DS_
    byte_ptr dst_sgmt;  // _ES_
    uint16_t src_ofst;  // _SI_
    uint16_t dst_ofst;  // _DI_
    uint8_t pixel;

    // TODO  width = farpeekw(bitmap, FLIC_HDR_POS_WIDTH);
    // TODO  height = farpeekw(bitmap, FLIC_HDR_POS_HEIGHT);
    width = GET_2B_OFS(bitmap, FLIC_HDR_POS_WIDTH);
    height = GET_2B_OFS(bitmap, FLIC_HDR_POS_HEIGHT);

    bitmap_size = (height * width);

// push    ds
// push    es
// push    si
// push    di
// mov     si, 16
// mov     di, 16
// mov     cx, [bp+bitmap_size]
// mov     bx, [bp+color_start]
// mov     dx, [bp+color_end]
// mov     ax, [bp+bitmap]
// mov     es, ax
// mov     ds, ax

    src_ofst = SZ_FLIC_HDR;
    dst_ofst = SZ_FLIC_HDR;
    dst_sgmt = (uint8_t *)(bitmap);
    src_sgmt = (uint8_t *)(bitmap);

    while(bitmap_size--)
    {

        pixel = *(src_sgmt + src_ofst++);  // `LODSB`  ; AX = DS:SI++

        if(pixel >= color_start && pixel <= color_end)
        {
            pixel = ST_TRANSPARENT;
            *(dst_sgmt + dst_ofst) = pixel;  // `STOSB`  ; ES:DI++ = AX
            dst_ofst -= 1;
        }

        dst_ofst += 1;

    }


}


// WZD s30p29
// UU_LBX_IMG_CropRect()


// WZD s30p30
// MoO2  ¿ Fill_Bitmap_();  Fill_Bitmap_Unbounded_();  Fill();  Fill_Region_(); ?
void Clear_Bitmap_Region(int16_t x1, int16_t y1, int16_t x2, int16_t y2, SAMB_ptr bitm)
{
    int16_t Draw_Height;
    int16_t Draw_Width;
    int16_t Draw_Start;
    int16_t Skip_Height;
    int16_t height;
    int16_t width;
    int16_t itr_draw_height;  // _CX_
    int16_t itr_draw_width;  // _BX_
    byte_ptr rvr_bitm;  // _ES_DI_

    // width  = farpeekw(bitm, 0);
    // height = farpeekw(bitm, 2);

    width  = FLIC_GET_WIDTH(bitm);
    height = FLIC_GET_HEIGHT(bitm);

    if((x1 < width) && (y1 < height) && (x2 > 0) && (y2 > 0))
    {
        if(x1 < 0) { x1 = 0; }
        if(y1 < 0) { y1 = 0; }
        if((width  - 1) < x2) { x2 = (width  - 1); }
        if((height - 1) < y2) { y2 = (height - 1); }
        Draw_Height = (y2 - y1 + 1);
        Skip_Height = (height - Draw_Height);
        Draw_Start = ((x1 * height) + y1);
        Draw_Width = (x2 - x1 + 1);

        rvr_bitm = (byte_ptr)(bitm + sizeof(struct s_FLIC_HDR) + Draw_Start);

        itr_draw_width = Draw_Width;
        while(itr_draw_width != 0)
        {
            itr_draw_height = Draw_Height;
            while(itr_draw_height != 0)
            {
                *rvr_bitm++ = ST_TRANSPARENT;
                itr_draw_height--;
            }
            rvr_bitm += Skip_Height;
            itr_draw_width--;
        }
    }
    else
    {
        // TODO  RP_MEM_Clear_Far2(sizeof(struct FLIC_HDR), bitm, (height * width));
    }

}


// WZD s30p31
// UU_LBX_IMG_FullGScale()

// WZD s30p32
// UU_LBX_IMG_ExtGScaleEC()

// WZD s30p33
// UU_LBX_IMG_ExtGrayScale()

// WZD s30p34
// drake178: LBX_IMG_Resize()
// STARMAP.EXE  s39p22  gfx_aux_scale()
// 1oom: C:\STU\developp\1oom\src\gfxaux.c  void gfx_aux_scale(struct gfx_aux_s *aux, int xscale, int yscale)
// MoO2  Module: bitmap  Scale_Bitmap()
/*
MoO2  Module: bitmap
    function (0 bytes) Scale_Bitmap
    Address: 01:0012E374
        Num params: 4
        Return type: void (1 bytes) 
        pointer (4 bytes) 
        pointer (4 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        Locals:
            pointer (4 bytes) target_bitmap
            pointer (4 bytes) source_bitmap
            signed integer (2 bytes) scale_x
            signed integer (2 bytes) scale_y
            signed integer (4 bytes) initial_x_scale
            signed integer (4 bytes) initial_y_scale
            signed integer (4 bytes) pict_x
            signed integer (4 bytes) pict_y
            signed integer (4 bytes) new_x
            signed integer (4 bytes) new_y
            signed integer (2 bytes) width
            signed integer (2 bytes) height
            pointer (4 bytes) frame_offset
            pointer (4 bytes) frame_offset_table
            pointer (4 bytes) source_start
            pointer (4 bytes) target_start
*/
/*

*/
void Scale_Bitmap(SAMB_ptr bitmap, int16_t scale_x, int16_t scale_y)
{
    int16_t height;
    int16_t width;
    int16_t initial_y_scale;
    int16_t initial_x_scale;

    width = FLIC_GET_WIDTH(bitmap);
    height = FLIC_GET_HEIGHT(bitmap);

    if(
        (scale_x <= 0) ||
        (scale_y <= 0) ||
        (((width  * scale_x) / 100) < 1) ||
        (((height * scale_y) / 100) < 1)
    )
    {
        Create_Picture(width, height, bitmap);
    }
    else
    {

        initial_x_scale = scale_x;
        initial_y_scale = scale_y;

        if(
            (scale_x < 100) &&
            (scale_y > 100)
        )
        {
            scale_x = 100;
        }

        if(
            (scale_x > 100) &&
            (scale_y < 100)
        )
        {
            scale_y = 100;
        }

        if(
            (scale_x > 100) ||
            (scale_y > 100)
        )
        {
            Enlarge_Bitmap(bitmap, scale_x, scale_y);
        }

        if(initial_x_scale > 100)
        {
            initial_x_scale = 100;
        }

        if(initial_y_scale > 100)
        {
            initial_y_scale = 100;
        }

        if(
            (scale_x < 100) ||
            (scale_y < 100)
        )
        {
            Reduce_Bitmap(bitmap, scale_x, scale_y);
        }

    }

}


// WZD s30p35
// VGA_FILEH_LoadFirst()
// MoO2  Module: file_ani  Open_File_Animation()
/*
MoO2
Module: file_ani
    function (0 bytes) Open_File_Animation
    Address: 01:0012C607
        Num params: 2
        Return type: void (1 bytes) 
        pointer (4 bytes) 
        signed integer (2 bytes) 
        Locals:
            pointer (4 bytes) file_name
            signed integer (2 bytes) entry_num
            signed integer (4 bytes) i
            signed integer (2 bytes) x2
            signed integer (2 bytes) y2
            signed integer (2 bytes) clip
            signed integer (4 bytes) frame_list
            pointer (4 bytes) frame_offset
            pointer (4 bytes) frame_offset_table
            signed integer (4 bytes) file_index
            signed integer (4 bytes) space_remaining
            signed integer (4 bytes) current_extended_flag
            unsigned integer (4 bytes) num_blocks
            unsigned integer (4 bytes) current_seg
            unsigned integer (4 bytes) read_size
            signed integer (4 bytes) foffset
            signed integer (4 bytes) entry_start
            signed integer (4 bytes) entry_end
            signed integer (4 bytes) entry_length
            array (60 bytes) full_file_path
            Num elements:   60, Type:                unsigned integer (4 bytes) 
*/
/*
; loads up to 16300 bytes from the beginning of an LBX
; file entry into the first (index 0) logical page of
; the VGAFILEH EMM handle, and copies the image header
; into the VGA_FILE_H_Hdr allocation, filling out the
; handle and data offset fields
*/
void Open_File_Animation__HACK(char * file_name, int16_t entry_num)
{

    strcpy(file_animation_file_name, file_name);

    file_animation_entry_num = entry_num;

    // file_animation_header = (struct s_FLIC_HDR *)LBX_Reload(file_animation_file_name, file_animation_entry_num, _VGAFILEH_seg);
    file_animation_header = LBX_Load(file_animation_file_name, file_animation_entry_num);

}
// void Open_File_Animation__STUB(char * file_name, int16_t entry_num)
// {
// 
//     strcpy(file_animation_file_name, file_name);
// 
//     file_animation_entry_num = entry_num;
// 
// 
// // xor     ax, ax
// // push    ax                              ; logical_page
// // xor     ax, ax
// // mov     dx, 16300
// // push    ax
// // push    dx                              ; size
// // xor     ax, ax
// // xor     dx, dx
// // push    ax
// // push    dx                              ; offset
// // nop
// // push    cs
// // Load_File_Animation_Frame__STUB();
// // // Open_File_Animation__STUB() pushes are 32-bit value
//     Load_File_Animation_Frame__STUB(0, 16300, 0, 0);
// 
// 
// // push    [EmmHndlNbr_VGAFILEH]           ; EMM_Handle
// // mov     ax, 16
// // push    ax                              ; Amt_Bytes
// // xor     ax, ax
// // xor     dx, dx
// // push    ax
// // push    dx                              ; Data_Offset
// // push    [file_animation_header]         ; Target_Seg
// // xor     ax, ax
// // push    ax                              ; Target_Off
// // EMM_MapnRead();
// // // Open_File_Animation__STUB() pushes are 32-bit value
// 
// 
//     // farpokew(file_animation_header, FLIC_HDR_POS_EMM_HANDLE_NUMBER, EmmHndlNbr_VGAFILEH);
//     // farpokew(file_animation_header, FLIC_HDR_POS_EMM_LOGICAL_PAGE_OFFSET, 0);
// 
// 
// }


// WZD s30p36
// drake178: VGA_FILEH_DrawFrame()
/*
; loads an image frame from the current VGA_Source_File
; into the VGAFILEH EMM handle starting at page index
; 1, and then draws it into the current draw frame
*/
// Draw_File_Animation()
void Draw_File_Animation__HACK(void)
{

    FLIC_Draw(0, 0, file_animation_header);

}


// WZD s30p37
// drake178: VGA_FILEH_Loader()
// Load_File_Animation_Frame()
/*
; loads data from a specific entry of an LBX file to
; the specified logical page in the VGAFILEH EMM handle
; the file name is set through VGA_Source_File, while
; the entry index is specified in VGA_Source_Entry
; passing a large enough byte count will load from the
; offset to the end of the entry, never beyond
; has a minor bug about error reporting
*/
/*

XREF:
    Open_File_Animation__WIP()
    Draw_File_Animation__WIP()

*/
// void Load_File_Animation_Frame__STUB(int32_t offset, int32_t size, int16_t logical_page)
// {
//     char file_name[LEN_FILE_NAME] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
//     int32_t entry_start = 0;
//     int32_t entry_length = 0;
//     int32_t Total_Bytes_Left = 0;
//     int16_t read_size = 0;
//     int16_t Page_Frame_Segment = 0;
//     int16_t Page_Count = 0;
//     int16_t current_page = 0;  // _SI_
//     int16_t lbx_fptr = 0;  // _DI_
//     // ; BUG: should not allocate a local string
// 
//     // if(_VGAFILEH_seg == ST_NULL)
//     {
//         // Error_Handler(file_animation_file_name, le_low_EMS, 0);
//         Error_Handler(file_animation_file_name, le_low_EMS, 0, ST_NULL);
//     }
// 
//     // ; finds and opens the LBX file if it wasn't already,
//     // ; loads its header data, and fills out the 32bit
//     // ; returns values pointed to by the arguments
//     // ; returns the file handle (unclosed), quits on errors
//     lbx_fptr = LBX_GetEntryData__WIP(file_animation_file_name, file_animation_entry_num, &entry_start, &entry_length, 0);
// 
//     entry_start += offset;
// 
//     entry_length -= offset;
// 
//     if(entry_length <= 0)
//     {
//         // ; BUG: should use the global variable
//         // Error_Handler(file_name, le_corrupted, file_animation_entry_num);
//         Error_Handler(file_name, le_corrupted, file_animation_entry_num, ST_NULL);
//     }
// 
// 
// 
// 
// 
// 
// 
// }


// WZD s30p38
// drake178: VGA_FILEH_GetFrame()
/*
; returns the index of the next frame that will be
; displayed by a VGA_FILEH_DrawFrame call
; Attributes: bp-based frame
*/
/*
proc Get_File_Animation_Frame far
push    bp
mov     bp, sp
mov     ax, s_FLIC_HDR.current_frame
push    ax                              ; Offset
push    [file_animation_header]         ; Segment
call    farpeekw
pop     cx
pop     cx
jmp     short $+2
pop     bp
retf
endp Get_File_Animation_Frame
*/
int16_t Get_File_Animation_Frame(void)
{
    int16_t file_animation_frame = 0;  // DNE in Dasm

    file_animation_frame = GET_2B_OFS(file_animation_header, FLIC_HDR_POS_CURRENT_FRAME);

    return file_animation_frame;
}


// WZD s30p39
// drake178: VGA_FILEH_SetFrame()
/*
proc Set_File_Animation_Frame far
frame_num= word ptr  6
push    bp
mov     bp, sp
push    [bp+frame_num]                  ; value
mov     ax, s_FLIC_HDR.current_frame
push    ax                              ; ofst
push    [file_animation_header]         ; sgmt
call    farpokew
add     sp, 6
pop     bp
retf
endp Set_File_Animation_Frame
*/
void Set_File_Animation_Frame(int16_t frame_num)
{

    SET_2B_OFS(file_animation_header, FLIC_HDR_POS_CURRENT_FRAME, frame_num);

}


// WZD s30p40
int16_t Get_Full_Store_Flag(SAMB_ptr p_FLIC_Header)
{
    int16_t full_store_flag;
    full_store_flag = FLIC_GET_FRAME_TYPE(p_FLIC_Header);
    return full_store_flag;
}


// WZD s30p41
void FLIC_Set_LoopFrame_1(SAMB_ptr p_FLIC_Header)
{
    FLIC_SET_LOOP_FRAME(p_FLIC_Header, 1);
}


// WZD s30p42
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

        if(pixel == ST_TRANSPARENT || pixel == outline_color)  /* outside pixel */
        {
            if(inside_state != ST_FALSE)  /* inside pixel & inside state */
            {
                *(src_sgmt + (src_ofst - 1)) = outline_color;
            }
            inside_state = ST_FALSE;
            outside_state = ST_TRUE;
        }
        else  /* inside pixel */
        {
            if(outside_state != ST_FALSE)
            {
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

        if(pixel == ST_TRANSPARENT || pixel == outline_color)  /* outside pixel */
        {
            if(inside_state != ST_FALSE)  /* inside pixel & inside state */
            {
                *(src_sgmt + (src_ofst - 1)) = outline_color;
            }
            inside_state = ST_FALSE;
            outside_state = ST_TRUE;
        }
        else  /* inside pixel */
        {
            if(outside_state != ST_FALSE)
            {
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

}


// WZD s30p43
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
    
    width = FLIC_GET_WIDTH(pict_seg);
    height = FLIC_GET_HEIGHT(pict_seg);
    pict_size = width * height;

    src_sgmt = (uint8_t *)(pict_seg + SZ_FLIC_HDR);

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

        if(pixel == ST_TRANSPARENT || pixel == outline_color || pixel >= 224)  /* outside pixel */
        {
            if(inside_state != ST_FALSE)  /* inside pixel & inside state */
            {
                *(src_sgmt + (src_ofst - 1)) = outline_color;
            }
            inside_state = ST_FALSE;
            outside_state = ST_TRUE;
        }
        else  /* inside pixel */
        {
            // Struggle-Mode  // TODO  DLOG("(pixel != ST_TRANSPARENT && pixel != outline_color && pixel < 224)");
            if(outside_state != ST_FALSE)
            {
                // Struggle-Mode  // TODO  DLOG("transition: inside pixel, outside state");
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

        if(pixel == ST_TRANSPARENT || pixel == outline_color || pixel >= 224)  /* outside pixel */
        {
            if(inside_state != ST_FALSE)  /* inside pixel & inside state */
            {
                *(src_sgmt + (src_ofst - 1)) = outline_color;
            }
            inside_state = ST_FALSE;
            outside_state = ST_TRUE;
        }
        else  /* inside pixel */
        {
            if(outside_state != ST_FALSE)
            {
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

}


// WZD s30p44
// MoO2: Bitmap_Aura() |-> Bitmap_Aura_Pixels_()
// replaces a selected color in an image with one of the eight indexes in the passed color array, constantly changing the next one to be written using a 16 color long repeat sequence
void Bitmap_Aura_Pixels(SAMB_ptr pict_seg, uint8_t aura_color, uint8_t * color_list)
{
    int16_t width;
    int16_t height;
    int16_t pict_size;
    uint8_t * pict_sgmt;
    uint16_t pict_ofst;
    uint8_t color_list_idx;
    uint8_t color_list_ctr;

    width = FLIC_GET_WIDTH(pict_seg);
    height = FLIC_GET_HEIGHT(pict_seg);
    pict_size = width * height;

    pict_sgmt = (uint8_t *)(pict_seg + SZ_FLIC_HDR);
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

}


// WZD s30p45
// UU_LBX_IMG_DrawRect()


// WZD s30p46
// EXACT  MoO2  Module: bitmap  Get_Bitmap_Actual_Size()
/*
sets x, y, w, h based on the first and last non-transparent pixel

    No RLE, therefore "Bitmap", per MoO2
*/
void Get_Bitmap_Actual_Size(SAMB_ptr bitmap_addr, int16_t * x1, int16_t * y1, int16_t * width, int16_t * height)
{
    uint8_t * bitmap;
    int16_t offset;
    int16_t itr_height;
    int16_t itr_width;
    int16_t length;
    int16_t x2;
    int16_t y2;
    int16_t flic_width;
    int16_t flic_height;

    // flic_width  = farpeekw(pict_seg, s_FLIC_HDR.Width);
    // flic_height = farpeekw(pict_seg, s_FLIC_HDR.Height);
    flic_width  = FLIC_GET_WIDTH(bitmap_addr);
    flic_height = FLIC_GET_HEIGHT(bitmap_addr);

    length = flic_height * flic_width;

    bitmap = (uint8_t *)bitmap_addr;

    *x1 = 1000;
    *y1 = 1000;

    x2 = 0;
    y2 = 0;

    offset = SZ_FLIC_HDR;

    for(itr_width = 0; itr_width < flic_width; itr_width++)
    {
        for(itr_height = 0; itr_height < flic_height; itr_height++)
        {
            if(bitmap[offset] != ST_TRANSPARENT)
            {
                if(*x1 > itr_width)
                {
                    *x1 = itr_width;
                }
                if(*y1 > itr_height)
                {
                    *y1 = itr_height;
                }
                if(x2 < itr_width)
                {
                    x2 = itr_width;
                }
                if(y2 < itr_height)
                {
                    y2 = itr_height;
                }

            }
            offset++;
        }
    }

    *height = (y2 - *y1 + 1);
    *width  = (x2 - *x1 + 1);

}



/*
    WIZARDS.EXE  seg031
*/

// WZD seg031:000A
int16_t CS031_skip_y;
// WZD seg031:000C
// CS031_UU_skip_x dw 0
// WZD seg031:000E
int16_t CS031_height;
// WZD seg031:0010
int16_t CS031_width;
// WZD seg031:0012

// WZD s31p01
// MoO2  Module: animate  Clipped_Draw_Animated_Sprite()
// 1oom  lbxgfx.c  lbxgfx_draw_pixels_offs_fmt0()
void Clipped_Draw_Frame(int16_t x1, int16_t y1, int16_t width, int16_t height, int16_t skip_x, int16_t skip_y, SAMB_ptr frame_data)
{
    unsigned char * bbuff_pos;
    unsigned char * bbuff;
    unsigned char data_byte;

    unsigned char packet_op;
    unsigned char packet_byte_count;

    unsigned char data_count;  // sequence_byte_count
    unsigned char skip_count;  // delta_byte_count
    unsigned char itr_op_repeat;

    int16_t line_count;  // height
    int16_t line_skip_count;  // skip_y ... skip_count

    CS031_skip_y = skip_y;
    CS031_width  = width;   // actual_width;
    CS031_height = height;  // actual_height;


    while(skip_x != 0)
    {
        packet_op = *frame_data++;
        packet_byte_count = *frame_data++;
        if(packet_op == 0xFF)  /* Type: skip */
        {
            frame_data--;
        }
        else
        {
            frame_data += packet_byte_count;
        }
        skip_x--;
    }

    bbuff_pos = current_video_page + ((y1 * SCREEN_WIDTH) + x1);

    while (width--)
    {
        // line_count = CS031_height;
        bbuff = bbuff_pos++;

        packet_op = *frame_data++;

        if(packet_op == 0xFF)  /* Type: skip */
        {
            continue;
        }

        packet_byte_count = *frame_data++;

        if(packet_op == 0x80)  /* Type: decode */
        {
            uint8_t len_compr;
            /* 10e97 */
            line_skip_count = skip_y;
        loc_10ea2:
            if(line_skip_count != 0)
            {
                skip_count = *(frame_data + 1);
                line_skip_count -= skip_count;
                if (line_skip_count > 0)
                {
                    data_count = *frame_data++;
                    frame_data++;
                    packet_byte_count -= data_count + 2;
                    /* loc_10eb6: */
                    do {
                        data_byte = *frame_data++;
                        if (data_byte <= 223)
                        {
                            if (--line_skip_count >= 0)
                            {
                                /* continue; */
                            }
                            else
                            {
                                /* goto 10ec7 */
                                data_count += line_skip_count;  // HERE: line_skip_count is negative
                                line_count = height - 1;
                                ++data_count;
                                goto loc_10f38;
                            }
                        }
                        else
                        {
                            /* 10ed3 */
                            frame_data++;
                            line_skip_count -= (data_byte - 223);
                            if (line_skip_count >= 0)
                            {
                                data_count--;
                                /* continue; */
                            }
                            else {
                                /* goto loc_10ef6; */
                                frame_data--;
                                data_byte = *frame_data++;
                                data_count--;
                                len_compr = -(line_skip_count);
                                line_count = height;
                                goto loc_10f51;
                            }
                        }
                    } while (--data_count);
                    if (packet_byte_count >= 1)
                    {
                        goto loc_10ea2;
                    }
                    else {
                        goto next_column;
                    }
                }
                else
                {
                    /* goto loc_10eea; */
                    data_count = -(line_skip_count);
                    line_count = height;
                    goto loc_10f0f;
                }
            }
            /* loc_10f05: */
            line_count = height;
            do {
                /* loc_10f0a: */
                skip_count = *(frame_data + 1);
            loc_10f0f:
                line_count -= skip_count;
                bbuff += skip_count * SCREEN_WIDTH;
                data_count = *frame_data++;
                frame_data++;
                packet_byte_count -= data_count + 2;
                /* loc_10f30: */
                do {
                    if (--line_count >= 0)
                    {
                        data_byte = *frame_data++;
                        if (data_byte <= 223)
                        {
                        loc_10f38:
                            *bbuff = data_byte;
                            bbuff += SCREEN_WIDTH;
                        }
                        else
                        {
                            len_compr = data_byte - 223;
                            data_byte = *frame_data++;
                            data_count--;
                            line_count++;
                        loc_10f51:
                            do {
                                if (--line_count >= 0)
                                {
                                    *bbuff = data_byte;
                                    bbuff += SCREEN_WIDTH;
                                }
                                else
                                {
                                    data_count--;
                                    goto loc_10f7e;
                                }
                            } while (--len_compr);
                        }
                    }
                    else
                    {
                    loc_10f7e:
                        frame_data += data_count;
                        break;
                    }
                } while (--data_count);
            } while (packet_byte_count >= 1);
        }

/*
    BEGIN: Copy - Packet Operation
*/
        if(packet_op == 0x00)  /* Type: copy */
        {
            line_skip_count = CS031_skip_y;

        copy_consume_skip_y:
            if(line_skip_count != 0)
            {
                skip_count = *(frame_data + 1);
                line_skip_count -= skip_count;

                if(line_skip_count > 0)
                {
                    data_count = *frame_data++;
                    line_skip_count -= data_count;

                    if (line_skip_count < 0)
                    {
                        /* copy_sequence_pre3: */
                        frame_data++;
                        packet_byte_count -= data_count + 2;
                        frame_data += line_skip_count;  // negative
                        frame_data += data_count;
                        data_count = -(line_skip_count);
                        line_count = CS031_height;
                        goto copy_loop_height;
                    }
                    else
                    {
                        frame_data++;
                        frame_data += data_count;
                        packet_byte_count -= data_count + 2;
                        if (packet_byte_count != 0) { goto copy_consume_skip_y; }
                        goto next_column;
                    }
                }
                else
                {
                    /* copy_sequence_pre2: */
                    line_skip_count = -(line_skip_count);
                    line_count = CS031_height;
                    goto copy_sequence_start_loop;
                }

            }

            /* copy_sequence_pre1: */
            line_count = CS031_height;
            /* copy_sequence_start: */
            do {
                line_skip_count = *(frame_data + 1);
                copy_sequence_start_loop:
                line_count -= line_skip_count;
                bbuff += (line_skip_count * SCREEN_WIDTH);
                data_count = *frame_data++;
                frame_data++;
                packet_byte_count -= data_count + 2;
                copy_loop_height:
                for (;;)
                {
                    line_count--;
                    if (line_count < 0) { frame_data += data_count; break; }
                    *bbuff = *frame_data++;
                    bbuff += SCREEN_WIDTH;
                    data_count--;
                    if (data_count == 0) { break; }
                }
                // if (!((packet_byte_count - 1) < 0)) { goto copy_sequence_start; }
                // goto next_column;  /* copy_end_loop_height: */
            } while (packet_byte_count >= 1);
        }  /* if (packet_op == 0x00)  / * Type: copy * / */

/*
    END: Copy - Packet Operation
*/

    next_column:
        ;

    }  /* while (width--) */

}

// static void lbxgfx_draw_pixels_offs_fmt0(int x0, int y0, int w, int h, int xskip, int yskip, uint8_t* data, uint16_t pitch)
static void lbxgfx_draw_pixels_offs_fmt0(int x0, int y0, int width, int h, int skip_x, int skip_y, uint8_t* data)
{
    uint16_t pitch = 320;
    uint8_t* p = current_video_page + y0 * pitch + x0;
    /* FIXME this an unreadable goto mess */
    // uint8_t* p = hw_video_get_buf() + y0 * pitch + x0;
    uint8_t* q;
    uint8_t b, packet_op, packet_byte_count, data_count;
    int ylen;

    /* skip columns */
    while (skip_x--) {
        b = *data++;
        if (b != 0xff) {
            b = *data++;
            data += b;
        }
    }

    while (width--)
    {
        q = p++;
        b = *data++;
        if (b == 0xff) { /* skip column */
            continue;
        }

        packet_op = b;
        packet_byte_count = *data++;
        if ((packet_op & 0x80) == 0) /* regular data  unsigned/non-negative*/
        {
            ylen = skip_y;  // line_skip_count = skip_y
        copy_consume_skip_y:
            if(ylen != 0)
            {
                ylen -= data[1];
                if(ylen > 0)
                {
                    data_count = *data++;
                    ylen -= data_count;
                    if(ylen >= 0)
                    {
                        data += data_count + 1;
                        packet_byte_count -= data_count + 2;
                        if (packet_byte_count != 0)
                        {
                            goto copy_consume_skip_y;
                        }
                        else
                        {
                            /*10e18*/
                            continue; /* goto next_column */
                        }
                    }  /* if(ylen >= 0) */
                    else
                    {
                        /*goto loc_10e83;*/
                        ++data;
                        packet_byte_count -= data_count + 2;
                        data += ylen + data_count;
                        data_count = -ylen;
                        ylen = h;
                        goto copy_loop_height;
                    }
                }  /* if(ylen > 0) */
                else
                {
                    /*goto loc_10e78;*/
                    data_count = -ylen;
                    ylen = h;
                    goto copy_sequence_start_loop;
                }
            }  /* if(ylen != 0) */

            /* copy_sequence_pre1 */
            ylen = h;
            do {
                /* copy_sequence_start */
                data_count = data[1]; /*really skip pixels...*/
            copy_sequence_start_loop:
                ylen -= data_count;
                q += data_count * pitch;
                data_count = *data++;
                ++data;
                packet_byte_count -= data_count + 2;
            copy_loop_height:
                do {
                    if (--ylen >= 0) {
                        *q = *data++;
                        q += pitch;
                    }
                    else {
                        data += data_count;
                        break;
                    }
                } while (--data_count);
            } while (packet_byte_count >= 1);
        }
        else {    /* compressed data */
            uint8_t len_compr;
            /*10e97*/
            ylen = skip_y;
        loc_10ea2:
            if (ylen != 0) {
                ylen -= data[1];
                if (ylen > 0) {
                    data_count = *data++;
                    ++data;
                    packet_byte_count -= data_count + 2;
                    /*loc_10eb6:*/
                    do {
                        b = *data++;
                        if (b <= 0xdf) {
                            if (--ylen >= 0) {
                                /*continue;*/
                            }
                            else {
                                /*goto 10ec7*/
                                data_count += ylen;
                                ylen = h - 1;
                                ++data_count;
                                goto loc_10f38;
                            }
                        }
                        else {
                            /*10ed3*/
                            ++data;
                            ylen -= (b - 0xdf);
                            if (ylen >= 0) {
                                --data_count;
                                /*continue;*/
                            }
                            else {
                                /*goto loc_10ef6;*/
                                --data;
                                b = *data++;
                                --data_count;
                                len_compr = -ylen;
                                ylen = h;
                                goto loc_10f51;
                            }
                        }
                    } while (--data_count);
                    if (packet_byte_count >= 1) {
                        goto loc_10ea2;
                    }
                    else {
                        goto next_column;
                    }
                }
                else {
                    /*goto loc_10eea;*/
                    data_count = -ylen;
                    ylen = h;
                    goto loc_10f0f;
                }
            }
            /*loc_10f05:*/
            ylen = h;
            do {
                /*loc_10f0a:*/
                data_count = data[1]; /*really skip pixels...*/
            loc_10f0f:
                ylen -= data_count;
                q += data_count * pitch;
                data_count = *data++;
                ++data;
                packet_byte_count -= data_count + 2;
                /*loc_10f30:*/
                do {
                    if (--ylen >= 0) {
                        b = *data++;
                        if (b <= 0xdf) {
                        loc_10f38:
                            *q = b;
                            q += pitch;
                        }
                        else {
                            len_compr = b - 0xdf;
                            b = *data++;
                            --data_count;
                            ++ylen;
                        loc_10f51:
                            do {
                                if (--ylen >= 0) {
                                    *q = b;
                                    q += pitch;
                                }
                                else {
                                    --data_count;
                                    goto loc_10f7e;
                                }
                            } while (--len_compr);
                        }
                    }
                    else {
                    loc_10f7e:
                        data += data_count;
                        break;
                    }
                } while (--data_count);
            } while (packet_byte_count >= 1);
        }
    next_column:
        ;
    }  /* while (width--) */
}


// WZD s31p02
// MoO2  Module:   Remap_Clipped_Draw_Animated_Sprite()
void Clipped_Remap_Draw_Frame__NOP(int16_t x1, int16_t y1, int16_t width, int16_t height, int16_t skip_x, int16_t skip_y, SAMB_ptr frame_data)
{

    assert(0);

}



/*
    WIZARDS.EXE  seg032
*/

// WZD s32p01
// VGA_DrawPartEmsImg()

// WZD s32p02
// EMM_MapNextIMGPages()

// WZD s32p03
// VGA_DrawPartEmsImg_R()



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
    dst = (destination_bitmap + SZ_FLIC_HDR);
    dst_pos = (destination_bitmap + SZ_FLIC_HDR);

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


// WZD s33p03
// Add_Picture_To_Bitmap_EMM()

// WZD s33p04
// EMM_MapNextIMGPages2()


// WZD seg033:024D
int16_t CS_height;
// WZD seg033:024F
int16_t CS_width;
// WZD seg033:0251
int16_t CS_skip_add;


// WZD s33p05
// MoO2 Draw_Bitmap_Sprite_
/*
    ¿ vs. Draw() ?  AKA FLIC_Draw() ...just assumes animation frames?
*/
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

    if(FLIC_GET_LOOP_FRAME(pict_seg) != 0)
    {
        Remap_Draw_Picture_ASM(x_start, y_start, ofst, pict_seg, width, height, skip_x);
    }
    else
    {
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

}

// WZD s33p07
// drake178: 
// MoO2  Module: shear  Mask_Bitmap_Pixels_()
// 1oom  gfxaux.c  static void gfx_aux_overlay_do_normal(uint8_t *dest, const uint8_t *src, int w, int h, int destskipw, int srcskipw)
/*
doesn't copy transparent pixels
Mask_Bitmap_Pixels() vs. Mask_Bitmap_Pixels_Inverse()
...inverse only copies transparent pixels

*/
void Mask_Bitmap_Pixels(byte_ptr dst, byte_ptr src, int16_t dst_skip_y, int16_t src_skip_y, int16_t width, int16_t height)
{
    int16_t itr_height;  // _CX_
    uint8_t pixel;  // _AL_

    CS_height = height;
    CS_width = width;

    while(CS_width)
    {
        itr_height = CS_height;
        while(itr_height)
        {
            pixel = *src++;
            dst++;
            if(pixel != ST_TRANSPARENT)
            {
                dst--;
                *dst++ = pixel;
            }
            itr_height--;
        }
        dst += dst_skip_y;
        src += src_skip_y;
        CS_width--;
    }

}


// WZD s33p08
// 1oom  gfxaux.c  gfx_aux_flipx()
/*

*/
void Flip_Bitmap(SAMB_ptr pict_seg)
{
    int16_t w;
    int16_t h;
    uint8_t * p;
    int16_t y;
    int16_t x;
    uint8_t t;
    w = FLIC_GET_WIDTH(pict_seg);
    h = FLIC_GET_HEIGHT(pict_seg);
    p = (pict_seg + SZ_FLIC_HDR);
    for (y = 0; y < h; ++y) {
        for (x = 0; x < (w / 2); ++x) {
            t = p[x];
            p[x] = p[w - 1 - x];
            p[w - 1 - x] = t;
        }
        p += w;
    }
}

// WZD s33p09
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

    src = pict_seg + SZ_FLIC_HDR;
    dst = pict_seg + SZ_FLIC_HDR;

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


// WZD s33p10
// UU_LBX_IMG_GetIntensity()

// WZD s33p11
// drake178: LBX_IMG_PartialCopy()
/*
; overlays part of an LBX image on another
*/
/*
...copies transparent from the src to the dst...

JZ vs. JNZ is same in MoO2 Mask_Bitmap_Pixels() vs. Mask_Bitmap_Pixels_Inverse()
¿ ~ Copy_Bitmap_Pixels() ?

¿ exact same as Color_Stream_Copy_() ?

// WZD s33p07
// MoO2  Module: draw  Color_Stream_Copy_()
// 1oom  gfxaux.c  static void gfx_aux_overlay_do_normal(uint8_t *dest, const uint8_t *src, int w, int h, int destskipw, int srcskipw)
void Mask_Bitmap_Pixels(byte_ptr dst, byte_ptr src, int16_t dst_skip_y, int16_t src_skip_y, int16_t width, int16_t height)

*/
void Mask_Bitmap_Pixels_Inverse(byte_ptr dst, byte_ptr src, int16_t dst_skip_y, int16_t src_skip_y, int16_t width, int16_t height)
{
    int16_t itr_height;  // _CX_
    uint8_t pixel;  // _AL_

    CS_height = height;
    CS_width = width;

    while(CS_width)
    {
        itr_height = CS_height;
        while(itr_height)
        {
            pixel = *src++;
            dst++;
            if(pixel == ST_TRANSPARENT)
            {
                dst--;
                *dst++ = pixel;
            }
            itr_height--;
        }
        dst += dst_skip_y;
        src += src_skip_y;
        CS_width--;
    }

}


// WZD s33p12
void Clear_Memory_Far(byte_ptr dst, int16_t n)
{
    byte_ptr rvr_dst;
    rvr_dst = dst;
    while(n != 0)
    {
        *rvr_dst++ = 0;
        n--;
    }
}


// WZD seg033:056B
int16_t CS_Horz_Remainder;
// WZD seg033:056D
int16_t CS_Horz_Ratio;
// WZD seg033:056F
int16_t CS_Orig_Height;

// WZD s33p13
// drake178: LBX_IMG_Shrink()
void Reduce_Bitmap(SAMB_ptr bitmap, int16_t scale_x, int16_t scale_y)
{
    int16_t Vert_Ratio;  // _DX_
    int16_t Vert_Remainder;  // _BX_
    uint8_t * src_sgmt;  // _DS_
    uint16_t src_ofst;  // _SI_
    byte_ptr src;
    uint8_t * dst_sgmt;  // _ES_
    uint16_t dst_ofst;  // _DI_
    byte_ptr dst;
    int16_t itr_height;  // _CX_
    uint16_t src_ofst_start;  // _SI_

    src_sgmt = (uint8_t *)bitmap;
    dst_sgmt = (uint8_t *)bitmap;
    FLIC_SET_WIDTH(bitmap, ((FLIC_GET_WIDTH(bitmap) * scale_x) / 100));
    CS_Horz_Ratio = (25600 / scale_x);
    CS_Orig_Height = FLIC_GET_HEIGHT(bitmap);
    FLIC_SET_HEIGHT(bitmap, ((FLIC_GET_HEIGHT(bitmap) * scale_y) / 100));
    Vert_Ratio = (25600 / scale_y);
    src_ofst = sizeof(struct s_FLIC_HDR);
    dst_ofst = sizeof(struct s_FLIC_HDR);
    CS_width =  FLIC_GET_WIDTH(bitmap);
    CS_Horz_Remainder = 0;
    dst = (dst_sgmt + dst_ofst);
    src = (src_sgmt + src_ofst);

    while(CS_width)
    {
        src_ofst_start = src_ofst;
        itr_height = FLIC_GET_HEIGHT(bitmap);
        Vert_Remainder = 0;
        while(itr_height)
        {
            *dst++ = *src++;

            src--;
            Vert_Remainder += Vert_Ratio;
            src += (Vert_Remainder >> 8);
            itr_height--;
        }
        src_ofst = src_ofst_start;
        src_ofst += (CS_Orig_Height * ((CS_Horz_Remainder + CS_Horz_Ratio) / 256));
        src = (src_sgmt + src_ofst);
        CS_Horz_Remainder = ((CS_Horz_Remainder + CS_Horz_Ratio) % 256);
        CS_width--;
    }

}

// WZD s33p14
// drake178: LBX_IMG_Stretch()
void Enlarge_Bitmap(SAMB_ptr bitmap, int16_t scale_x, int16_t scale_y)
{
    int16_t Vert_Ratio;  // _DX_
    int16_t Vert_Remainder;  // _BX_
    uint8_t * src_sgmt;  // _DS_
    uint16_t src_ofst;  // _SI_
    byte_ptr src;
    uint8_t * dst_sgmt;  // _ES_
    uint16_t dst_ofst;  // _DI_
    byte_ptr dst;
    int16_t itr_height;  // _CX_
    uint16_t src_ofst_start;  // _SI_

    src_sgmt = (uint8_t *)bitmap;
    dst_sgmt = (uint8_t *)bitmap;

    src_ofst = (15 + FLIC_GET_WIDTH(bitmap) * FLIC_GET_HEIGHT(bitmap));

    FLIC_SET_WIDTH(bitmap, ((FLIC_GET_WIDTH(bitmap) * scale_x) / 100));

    CS_Horz_Ratio = (25600 / scale_x);

    CS_Orig_Height = FLIC_GET_HEIGHT(bitmap);

    FLIC_SET_HEIGHT(bitmap, ((FLIC_GET_HEIGHT(bitmap) * scale_y) / 100));

    Vert_Ratio = (25600 / scale_y);

    dst_ofst = (15 + FLIC_GET_WIDTH(bitmap) * FLIC_GET_HEIGHT(bitmap));

    CS_width =  FLIC_GET_WIDTH(bitmap);

    CS_Horz_Remainder = 0;

    dst = (dst_sgmt + dst_ofst);
    src = (src_sgmt + src_ofst);

    while(CS_width)
    {
        src_ofst_start = src_ofst;
        itr_height = FLIC_GET_WIDTH(bitmap);
        Vert_Remainder = 0;
        while(itr_height)
        {
            *dst++ = *src++;
            dst--;
            dst--;
            src--;
            Vert_Remainder += Vert_Ratio;
            src -= (Vert_Remainder >> 8);
            itr_height--;
        }
        src_ofst = src_ofst_start;
        if((CS_Horz_Remainder + CS_Horz_Ratio) >= 256)
        {
            src_ofst -= CS_Orig_Height;
        }
        src = (src_sgmt + src_ofst);

        CS_Horz_Remainder = ((CS_Horz_Remainder + CS_Horz_Ratio) % 256);

        CS_width--;
    }

}



// WZD s33p15
// MoO2  Module: bitmap  Vanish_Bitmap()
// MoO2  Moudle: shear   Vanish_Bitmap_Pixels_()
/*
; deletes an amount of random pixels from an LBX image
; roughly corresponding to the specified percent (they
; are replaced with transparent pixels - color #00)
; uses three local RNGs, not the global one
*/
/*
push    [Percent]                       ; percent
push    [GfxBuf_2400B]                  ; bitmap
call    Jumble_Bitmap  

bitmap is a segment address

*/
// WZD seg033:06E3 00 00                                           _CS_GFX_RNG_TempSeed dw 0
// WZD seg033:06E5 2A 44 23 01 46 19 4F 3B 3F 41 06 2E 52 1C 3E 5C+_CS_GFX_RND100_Lookup db 42,68,35,1,70,25,79,59,63,65,6,46,82,28,62,92,96,43,28,37,92,5,3,54,93,83,22,17,19,96,48,27,72,39,70,13,68,100,36,95,4,12,23,34,74,65,42,12,54,69,48,45,63,58,38,60,24,42,30,79,17,36,91,43,89,7,41,43,65,49,47,6,91,30,71,51,7,2,94,49,30,24,85,55,57,41,67
// WZD seg033:06E5 60 2B 1C 25 5C 05 03 36 5D 53 16 11 13 60 30 1B+
// WZD seg033:06E5 48 27 46 0D 44 64 24 5F 04 0C 17 22 4A 41 2A 0C+db 77,32,9,45,40,27,24,38,39,19,83,30,42,34,16,40,59,5,31,78,7,74,87,22,46,25,73,71,30,78,74,98,13,87,91,62,37,56,68,56,75,32,53,51,51,42,25,67,31,8,92,8,38,58,88,54,84,46,10,10,59,22,89,23,47,7,31,14,69,1,92,63,56,11,60,25,38,49,84,96,42,3,51,92,37,75,21
// WZD seg033:06E5 36 45 30 2D 3F 3A 26 3C 18 2A 1E 4F 11 24 5B 2B+db 97,22,49,100,69,85,82,35,54,100,19,39,1,89,28,68,29,94,49,84,8,22,11,18,14,15,10,17,36,52,1,50,20,57,99,4,25,9,45,10,90,3,96,86,94,44,24,88,15,4,49,1,59,19,81,97,99,82,90,99,10,58,73,23,39,93,39,80,91,58,59,92,16,89,57,12,3,35,73,56,29,47
static uint16_t _CS_GFX_RNG_TempSeed = 0;
uint8_t _CS_GFX_RND100_Lookup[] = {42,68,35,1,70,25,79,59,63,65,6,46,82,28,62,92,96,43,28,37,92,5,3,54,93,83,22,17,19,96,48,27,72,39,70,13,68,100,36,95,4,12,23,34,74,65,42,12,54,69,48,45,63,58,38,60,24,42,30,79,17,36,91,43,89,7,41,43,65,49,47,6,91,30,71,51,7,2,94,49,30,24,85,55,57,41,67,77,32,9,45,40,27,24,38,39,19,83,30,42,34,16,40,59,5,31,78,7,74,87,22,46,25,73,71,30,78,74,98,13,87,91,62,37,56,68,56,75,32,53,51,51,42,25,67,31,8,92,8,38,58,88,54,84,46,10,10,59,22,89,23,47,7,31,14,69,1,92,63,56,11,60,25,38,49,84,96,42,3,51,92,37,75,21,97,22,49,100,69,85,82,35,54,100,19,39,1,89,28,68,29,94,49,84,8,22,11,18,14,15,10,17,36,52,1,50,20,57,99,4,25,9,45,10,90,3,96,86,94,44,24,88,15,4,49,1,59,19,81,97,99,82,90,99,10,58,73,23,39,93,39,80,91,58,59,92,16,89,57,12,3,35,73,56,29,47};
static uint16_t _CS_width = 0;
void Vanish_Bitmap__WIP(SAMB_ptr bitmap, int16_t percent)
{
    int16_t offset = 0;  // _DI_
    int16_t _AX_ = 0;
    int16_t _BX_ = 0;
    int16_t _CX_ = 0;
    int16_t _DX_ = 0;
    void * _DS_ = NULL;
    void * _SI_ = NULL;
    void * _ES_ = NULL;
    void * _DI_ = NULL;

    _CS_GFX_RNG_TempSeed = RNG_GFX_Random__WIP(1000);

    // mov     ax, [bp+bitmap]
    // mov     es, ax

    offset = SZ_FLIC_HDR;

    // _CS_width = *((struct s_FLIC_Header *)bitmap)->width;
    _CX_ = GET_2B_OFS(bitmap, FLIC_HDR_POS_WIDTH);

    // mov     dx, [bp+percent]
    _DX_ = percent;

    // mov     ax, cs
    // mov     ds, ax

    do {

        _AX_ = rnd_bitfiddle__1oom(_CS_GFX_RNG_TempSeed);
        _CS_GFX_RNG_TempSeed = _AX_;

        // mov     al, ah
        // xor     ah, ah
        _AX_ >>= 8;
        _AX_ &= 0x0F;

        // mov     bx, ax
        _BX_ = _AX_;

        // mov     si, bx
        // add     si, offset GFX_RND100_Lookup
        // _SI_ = _BX_;
        // _SI_ += _CS_GFX_RND100_Lookup;
        // ~ &_CS_GFX_RND100_Lookup[_BX_]
        _SI_ = &_CS_GFX_RND100_Lookup[_BX_];

        // mov     cx, [es:s_FLIC_HDR.height]
        _CX_ = GET_2B_OFS(bitmap, FLIC_HDR_POS_HEIGHT);

        while(--_CX_)
        {
            
            // ; AL = byte ptr [DS:SI]
            // lodsb
            _AX_ = *(uint8_t *)_SI_;
            // inc     _DI_offset
            offset++;

            // cmp     al, dl
            // jz      short loc_24DCD
            if(
                (_AX_ & 0x0F) != (_DX_ * 0x0F)
                &&
                (_AX_ & 0x0F) >= (_DX_ * 0x0F)
            )
            {
                // xor     al, al
                _AX_ &= 0xF0; 
                // dec     _DI_offset
                offset--;
                // stosb 
                // ES:DI = AL
                bitmap[offset] = ST_TRANSPARENT;

                // inc     bx
                _BX_++;
                // xor     bh, bh
                _BX_ &= 0x0F;
                // mov     si, bx
                // add     si, offset _CS_GFX_RND100_Lookup
                _SI_ = &_CS_GFX_RND100_Lookup[_BX_];

                // loop    loc_24DC1

            }

        }

    } while (--_CS_width != 0);

}


// WZD s33p16
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

    // DS:SI  src  current_palette[0]
    // ES:DI  dst  Intensity_Scale_Tbl@[0]
    src_sgmt = current_palette;
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

    src_ofst = SZ_FLIC_HDR;
    dst_ofst = SZ_FLIC_HDR;

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

}

// WZD s33p17
// drake178: LBX_IMG_RevGrayscale()
/*
; similar to LBX_IMG_Grayscale, this calculates the
; intensities of each DAC color and condenses them to a
; nibble before replacing the original non-overlay
; colors of the image with them, except this negates
; them before adding the provided color index, creating
; an inverse scale, but also taking the last color of
; the scale as the parameter instead of the first one
*/
/*

*/
void LBX_IMG_RevGrayscale__STUB(byte_ptr bitmap, int16_t color_index)
{



}


// WZD s33p18
// UU_DUP_RevGrayscale()


// WZD s33p19
/*
drake178:
    a secondary linear feedback shift register
    using the same feedback polynomial as RNG_Random,
    also shifted 9 states per call,
    but actually returning the lowest order 10 bits of the resulting state (1-1023)
    rather than the shifted out return value
*/
// WZD seg033:09E3 93 28                                           _CS_Local_LFSR_LO dw 2893h                  ; DATA XREF: RNG_GFX_Random__NOP+1Ar ...
// WZD seg033:09E5 0A 00                                           _CS_Local_LFSR_HI dw 0Ah                    ; DATA XREF: RNG_GFX_Random__NOP+1Fr ...
static uint16_t _CS_Local_LFSR_LO = 0x2893;
static uint16_t _CS_Local_LFSR_HI = 0x0A;
int16_t RNG_GFX_Random__WIP(int16_t max)
{
    int16_t UU_result = 0;
    int16_t _AX_ = 0;
    int16_t _BX_ = 0;
    int16_t _CX_ = 0;
    int16_t _DX_ = 0;
    // void * _SI_ = NULL;
    // void * _DI_ = NULL;
    int16_t _SI_ = 0;
    int16_t _DI_ = 0;
    int16_t _CF_ = 0;

    if(max == 0)
    {
        max = 1;
    }

    UU_result = 0;

    // mov     si, [cs:_CS_Local_LFSR_LO]
    // _SI_ = &_CS_Local_LFSR_LO[0];
    _SI_ = _CS_Local_LFSR_LO;

    // mov     di, [cs:_CS_Local_LFSR_HI]
    // _DI_ = &_CS_Local_LFSR_HI[0];
    _DI_ = _CS_Local_LFSR_HI;

    // mov     cx, 9
    _CX_ = 9;

// loc_24F9E:
do {
_AX_ = _SI_;    // mov     ax, si
_BX_ = _SI_;    // mov     bx, si
_DX_ = _DI_;    // mov     dx, di
_DX_ >>= 1;     // shr     dx, 1

// rcr     bx, 1
    if(_BX_ & 0x0001) { _CF_ = 0x0001; }
    _BX_ >>= _BX_;
    _BX_ &= _CF_;

_AX_ ^= _BX_;   // xor     ax, bx
_DX_ >>= 1;     // shr     dx, 1

// rcr     bx, 1
    if(_BX_ & 0x0001) { _CF_ = 0x0001; }
    _BX_ >>= _BX_;
    _BX_ &= _CF_;

_AX_ ^= _BX_;   // xor     ax, bx
_DX_ >>= 1;     // shr     dx, 1

// rcr     bx, 1
    if(_BX_ & 0x0001) { _CF_ = 0x0001; }
    _BX_ >>= _BX_;
    _BX_ &= _CF_;

_DX_ >>= 1;     // shr     dx, 1

// rcr     bx, 1
    if(_BX_ & 0x0001) { _CF_ = 0x0001; }
    _BX_ >>= _BX_;
    _BX_ &= _CF_;

_AX_ ^= _BX_;   // xor     ax, bx
_DX_ >>= 1;     // shr     dx, 1

// rcr     bx, 1
    if(_BX_ & 0x0001) { _CF_ = 0x0001; }
    _BX_ >>= _BX_;
    _BX_ &= _CF_;

_DX_ >>= 1;     // shr     dx, 1

// rcr     bx, 1
    if(_BX_ & 0x0001) { _CF_ = 0x0001; }
    _BX_ >>= _BX_;
    _BX_ &= _CF_;

_AX_ ^= _BX_;   // xor     ax, bx
_DX_ >>= 1;     // shr     dx, 1
_AX_ = (_AX_ & 0x0F) ^ (_DX_ & 0xF0);   // xor     al, dh
_DX_ = _AX_;    // mov     dx, ax
_DX_ >>= 1;     // shr     dx, 1

// rcl     [bp+UU_result], 1
    if(UU_result & 0x8000) {_CF_ = 0x0001; }
    UU_result >>= UU_result;
    UU_result &= _CF_;

_AX_ >>= 1;     // shr     ax, 1

// rcr     di, 1
    if(_DI_ & 0x0001) { _CF_ = 0x0001; }
    _DI_ >>= _DI_;
    _DI_ &= _CF_;

// rcr     si, 1
    if(_SI_ & 0x0001) { _CF_ = 0x0001; }
    _SI_ >>= _SI_;
    _SI_ &= _CF_;

} while (--_CX_ != 0);
// loop    loc_24F9E


// cmp     si, 0
// jnz     short loc_24FE4
// cmp     di, 0
// jnz     short loc_24FE4
// mov     si, 12478
if(
    (_SI_ = 0)
    &&
    (_DI_ == 0)
)
{
    _SI_ = 0x30BE;
}

// loc_24FE4:
// mov     [cs:_CS_Local_LFSR_LO], si
// mov     [cs:_CS_Local_LFSR_HI], di
    _CS_Local_LFSR_LO = _SI_;
    _CS_Local_LFSR_HI = _DI_;
// mov     ax, si
// and     ax, 1023
    _AX_ = _SI_;
    _AX_ &= 0x03FF;
    
// loc_24FF3:
// cmp     ax, [bp+max]
// jb      short @@Done
// sub     ax, [bp+max]
// jmp     short loc_24FF3
    while(!(_AX_ < max))
    {
        _AX_ -= max;
    }

    return _AX_;

}
