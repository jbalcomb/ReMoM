
#include "MoX.H"
#include "Video2.H"
#include "PoC_PFL.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#include "STU_TST.H"
#endif
// #include <assert.h>


// // replicate pixels, per scale, for each x, on each y
// static inline uint8_t * gfxscale_draw_pixel(uint8_t *q, uint8_t b, uint16_t pitch, int scale)
// {
//     for (int y = 0; y < scale; ++y)
//     {
//         for (int x = 0; x < scale; ++x)
//         {
//             q[x] = b;
//         }
//         q += pitch;
//     }
//     return q;
// }


/*
    WZD  seg029
*/

// WZD s29p01
/*
    1oom:       ~==
        pixbuf     bbuff_pos
        q          bbuff
        b          data_byte
        mode       packet_op
        len_run    sequence_byte_count
        len_total  packet_byte_count
*/
void FLIC_Draw_Frame_2x(int16_t x_start, int16_t y_start, int16_t width, byte_ptr frame_data, int16_t pitch, int16_t scale)
{
    unsigned char * bbuff_pos;
    unsigned char * bbuff;
    unsigned char data_byte;

    unsigned char packet_op;
    unsigned char packet_byte_count;
    unsigned char sequence_byte_count;
    unsigned char delta_byte_count;
    unsigned char itr_op_repeat;

    byte_ptr frame_data_orig;
    unsigned char * bbuff_pos_orig;
    
    int sx;
    int sy;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: FLIC_Draw_Frame_2x()\n", __FILE__, __LINE__);
#endif


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: STU_TST: Validate_MAIN_LBX_000(): %d\n", __FILE__, __LINE__, Validate_MAIN_LBX_000_00(frame_data));
#endif


    // bbuff_pos = current_video_page + ((y_start * SCREEN_WIDTH) + x_start);
    // bbuff_pos = current_video_page + ((y_start * pitch) + x_start);
    bbuff_pos = video_page_buffer_2x[1 - draw_page_num] + ((y_start * pitch) + x_start);


    frame_data_orig = frame_data;
    bbuff_pos_orig = bbuff_pos;


    while (width--)
    {
        // bbuff = bbuff_pos++;
        bbuff = bbuff_pos;
        bbuff_pos += scale;
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
                // bbuff += (delta_byte_count * SCREEN_WIDTH);
                bbuff += (delta_byte_count * pitch) * scale;
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
                            // // *bbuff = data_byte;
                            // // bbuff += SCREEN_WIDTH;
                            // bbuff = gfxscale_draw_pixel(bbuff, data_byte, pitch, scale);
                            // // *bbuff = data_byte;
                            // // *(bbuff + 1) = data_byte;
                            // // bbuff += pitch;
                            // // *bbuff = data_byte;
                            // // *(bbuff + 1) = data_byte;
                            // // bbuff += pitch;
                            for(sy = 0; sy < scale; sy++)
                            {
                                for(sx = 0; sx < scale; sx++)
                                {
                                    *(bbuff + sx) = data_byte;
                                }
                                bbuff += pitch;
                            }

                        }
                    }
                    else  /* op: copy */
                    {
                        // // *bbuff = data_byte;
                        // // bbuff += SCREEN_WIDTH;
                        // bbuff = gfxscale_draw_pixel(bbuff, data_byte, pitch, scale);
                        for (int sy = 0; sy < scale; sy++)
                        {
                            for (sx = 0; sx < scale; sx++)
                            {
                                *(bbuff + sx) = data_byte;
                            }
                            bbuff += pitch;
                        }

                    }
                }
            } while(packet_byte_count >= 1);
        }
        if(packet_op == 0x00)  /* Type: copy */
        {
            do {
                sequence_byte_count = *frame_data++;
                delta_byte_count = *frame_data++;
                // bbuff += (delta_byte_count * SCREEN_WIDTH);
                bbuff += (delta_byte_count * pitch) * scale;
                packet_byte_count -= sequence_byte_count + 2;
                while(sequence_byte_count--)
                {
                    // // *bbuff = *frame_data++;
                    // // bbuff += SCREEN_WIDTH;
                    // data_byte = *frame_data++;
                    // bbuff = gfxscale_draw_pixel(bbuff, data_byte, pitch, scale);
                    data_byte = *frame_data++;
                    for (int sy = 0; sy < scale; sy++)
                    {
                        for (sx = 0; sx < scale; sx++)
                        {
                            *(bbuff + sx) = data_byte;
                        }
                        bbuff += pitch;
                    }

                }
            } while(packet_byte_count >= 1);
        }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: STU_TST: Validate_MAIN_LBX_000(): %d\n", __FILE__, __LINE__, Validate_MAIN_LBX_000_00(frame_data_orig));
#endif

    }  /* while (width--) */


// frame_data_orig - frame_data should be 12276
// bbuff_pos_orig - bbuff_pos should be width * scale
// bbuff should be < bbuff_pos_orig + ((640*400)*2)
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: frame_data_orig: %p\n", __FILE__, __LINE__, frame_data_orig);
    dbg_prn("DEBUG: [%s, %d]: frame_data: %p\n", __FILE__, __LINE__, frame_data);
    dbg_prn("DEBUG: [%s, %d]: (frame_data - frame_data_orig): %d\n", __FILE__, __LINE__, (frame_data - frame_data_orig));
#endif
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: bbuff_pos_orig: %p\n", __FILE__, __LINE__, bbuff_pos_orig);
    dbg_prn("DEBUG: [%s, %d]: bbuff_pos: %p\n", __FILE__, __LINE__, bbuff_pos);
    dbg_prn("DEBUG: [%s, %d]: (bbuff_pos - bbuff_pos_orig): %d\n", __FILE__, __LINE__, (bbuff_pos - bbuff_pos_orig));
#endif
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: (bbuff_pos - scale): %p\n", __FILE__, __LINE__, (bbuff_pos - scale));
    dbg_prn("DEBUG: [%s, %d]: bbuff: %p\n", __FILE__, __LINE__, bbuff);
    dbg_prn("DEBUG: [%s, %d]: ((bbuff - pitch) - (bbuff_pos - scale)): %d\n", __FILE__, __LINE__, ((bbuff - pitch) - (bbuff_pos - scale)));
#endif
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: video_page_buffer_2x[1 - draw_page_num]: %p\n", __FILE__, __LINE__, video_page_buffer_2x[1 - draw_page_num]);
    dbg_prn("DEBUG: [%s, %d]: (bbuff - picth): %p\n", __FILE__, __LINE__, (uint8_t *)(bbuff - pitch));
    dbg_prn("DEBUG: [%s, %d]: ((bbuff - pitch) - video_page_buffer_2x[1 - draw_page_num]): %d\n", __FILE__, __LINE__, ((bbuff - pitch) - video_page_buffer_2x[1 - draw_page_num]));

#endif
    // (bbuff - pitch) is that last address that was written to
    // it should be <= video_page_buffer_2x[1 - draw_page_num] + 256000

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: FLIC_Draw_Frame_2x()\n", __FILE__, __LINE__);
#endif

}



void FLIC_Draw_Frame_2x_XBGR(int16_t x_start, int16_t y_start, int16_t width, byte_ptr frame_data, int16_t pitch, int16_t scale)
{
    // unsigned char * bbuff_pos;
    // unsigned char * bbuff;
    uint32_t * bbuff_pos;
    uint32_t * bbuff;
    unsigned char data_byte;

    unsigned char packet_op;
    unsigned char packet_byte_count;
    unsigned char sequence_byte_count;
    unsigned char delta_byte_count;
    unsigned char itr_op_repeat;

    byte_ptr frame_data_orig;
    uint32_t * bbuff_pos_orig;
    
    int sx;
    int sy;

    uint32_t * p_XBGR;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: FLIC_Draw_Frame_2x_XBGR()\n", __FILE__, __LINE__);
#endif


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: STU_TST: Validate_MAIN_LBX_000(): %d\n", __FILE__, __LINE__, Validate_MAIN_LBX_000_00(frame_data));
#endif

    p_XBGR = (uint32_t*)g_Palette_XBGR;

    // bbuff_pos = current_video_page + ((y_start * SCREEN_WIDTH) + x_start);
    // bbuff_pos = current_video_page + ((y_start * pitch) + x_start);
    bbuff_pos = (uint32_t *)video_page_buffer_2x_XBGR[1 - draw_page_num] + ((y_start * pitch) + x_start);


    frame_data_orig = frame_data;
    bbuff_pos_orig = bbuff_pos;


    while (width--)
    {
        // bbuff = bbuff_pos++;
        bbuff = bbuff_pos;
        bbuff_pos += scale;
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
                // bbuff += (delta_byte_count * SCREEN_WIDTH);
                bbuff += (delta_byte_count * pitch) * scale;
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
                            // // *bbuff = data_byte;
                            // // bbuff += SCREEN_WIDTH;
                            // bbuff = gfxscale_draw_pixel(bbuff, data_byte, pitch, scale);
                            // // *bbuff = data_byte;
                            // // *(bbuff + 1) = data_byte;
                            // // bbuff += pitch;
                            // // *bbuff = data_byte;
                            // // *(bbuff + 1) = data_byte;
                            // // bbuff += pitch;
                            for(sy = 0; sy < scale; sy++)
                            {
                                for(sx = 0; sx < scale; sx++)
                                {
                                    // *(bbuff + sx) = data_byte;
                                    *(bbuff + sx) = p_XBGR[data_byte];
                                }
                                bbuff += pitch;
                            }

                        }
                    }
                    else  /* op: copy */
                    {
                        // // *bbuff = data_byte;
                        // // bbuff += SCREEN_WIDTH;
                        // bbuff = gfxscale_draw_pixel(bbuff, data_byte, pitch, scale);
                        for (int sy = 0; sy < scale; sy++)
                        {
                            for (sx = 0; sx < scale; sx++)
                            {
                                // *(bbuff + sx) = data_byte;
                                *(bbuff + sx) = p_XBGR[data_byte];
                            }
                            bbuff += pitch;
                        }

                    }
                }
            } while(packet_byte_count >= 1);
        }
        if(packet_op == 0x00)  /* Type: copy */
        {
            do {
                sequence_byte_count = *frame_data++;
                delta_byte_count = *frame_data++;
                // bbuff += (delta_byte_count * SCREEN_WIDTH);
                bbuff += (delta_byte_count * pitch) * scale;
                packet_byte_count -= sequence_byte_count + 2;
                while(sequence_byte_count--)
                {
                    // // *bbuff = *frame_data++;
                    // // bbuff += SCREEN_WIDTH;
                    // data_byte = *frame_data++;
                    // bbuff = gfxscale_draw_pixel(bbuff, data_byte, pitch, scale);
                    data_byte = *frame_data++;
                    for (int sy = 0; sy < scale; sy++)
                    {
                        for (sx = 0; sx < scale; sx++)
                        {
                            // *(bbuff + sx) = data_byte;
                            *(bbuff + sx) = p_XBGR[data_byte];
                        }
                        bbuff += pitch;
                    }

                }
            } while(packet_byte_count >= 1);
        }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: STU_TST: Validate_MAIN_LBX_000(): %d\n", __FILE__, __LINE__, Validate_MAIN_LBX_000_00(frame_data_orig));
#endif

    }  /* while (width--) */


// frame_data_orig - frame_data should be 12276
// bbuff_pos_orig - bbuff_pos should be width * scale
// bbuff should be < bbuff_pos_orig + ((640*400)*2)
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: frame_data_orig: %p\n", __FILE__, __LINE__, frame_data_orig);
    dbg_prn("DEBUG: [%s, %d]: frame_data: %p\n", __FILE__, __LINE__, frame_data);
    dbg_prn("DEBUG: [%s, %d]: (frame_data - frame_data_orig): %d\n", __FILE__, __LINE__, (frame_data - frame_data_orig));
#endif
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: bbuff_pos_orig: %p\n", __FILE__, __LINE__, bbuff_pos_orig);
    dbg_prn("DEBUG: [%s, %d]: bbuff_pos: %p\n", __FILE__, __LINE__, bbuff_pos);
    dbg_prn("DEBUG: [%s, %d]: (bbuff_pos - bbuff_pos_orig): %d\n", __FILE__, __LINE__, (bbuff_pos - bbuff_pos_orig));
#endif
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: (bbuff_pos - scale): %p\n", __FILE__, __LINE__, (bbuff_pos - scale));
    dbg_prn("DEBUG: [%s, %d]: bbuff: %p\n", __FILE__, __LINE__, bbuff);
    dbg_prn("DEBUG: [%s, %d]: ((bbuff - pitch) - (bbuff_pos - scale)): %d\n", __FILE__, __LINE__, ((bbuff - pitch) - (bbuff_pos - scale)));
#endif
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: video_page_buffer_2x_XBGR[1 - draw_page_num]: %p\n", __FILE__, __LINE__, video_page_buffer_2x_XBGR[1 - draw_page_num]);
    dbg_prn("DEBUG: [%s, %d]: (bbuff - picth): %p\n", __FILE__, __LINE__, (uint32_t *)(bbuff - pitch));
    dbg_prn("DEBUG: [%s, %d]: ((bbuff - pitch) - video_page_buffer_2x_XBGR[1 - draw_page_num]): %d\n", __FILE__, __LINE__, ((bbuff - pitch) - (uint32_t *)video_page_buffer_2x_XBGR[1 - draw_page_num]));

#endif
    // (bbuff - pitch) is that last address that was written to
    // it should be <= video_page_buffer_2x_XBGR[1 - draw_page_num] + 1000000

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: FLIC_Draw_Frame_2x_XBGR()\n", __FILE__, __LINE__);
#endif

}




/*
    WZD seg030
*/

// WZD s30p11
void Draw(int16_t x_start, int16_t y_start, SAMB_ptr p_FLIC_File, int16_t pitch, int16_t scale)
{
    int16_t current_frame_index;
    int16_t next_frame_index;
    unsigned int flic_frame_offset;
    unsigned short flic_frame_offset_sgmt;
    unsigned short flic_frame_offset_ofst;
    byte_ptr p_FLIC_Frame;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: FLIC_Draw_2x()\n", __FILE__, __LINE__);
#endif

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

    // switch (video_mode)
    // {
    //     case vm_Mode_Y:
    //     {
    //         FLIC_Draw_Frame(x_start, y_start, FLIC_GET_WIDTH(p_FLIC_File), p_FLIC_Frame);
    //     } break;
    //     case vm_Mode_Y_2x:
    //     {
    //         FLIC_Draw_Frame_2x(x_start, y_start, FLIC_GET_WIDTH(p_FLIC_File), p_FLIC_Frame, pitch, scale);
    //     } break;
    //     case vm_Mode_Y_2x_XBGR:
    //     {
    //         FLIC_Draw_Frame_2x_XBGR(x_start, y_start, FLIC_GET_WIDTH(p_FLIC_File), p_FLIC_Frame, pitch, scale);
    //     } break;
    // }
    FLIC_Draw_Frame_2x_XBGR(x_start, y_start, FLIC_GET_WIDTH(p_FLIC_File), p_FLIC_Frame, pitch, scale);


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: FLIC_Draw_2x()\n", __FILE__, __LINE__);
#endif

}
