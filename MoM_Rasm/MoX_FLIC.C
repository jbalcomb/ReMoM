
#include "MoX_TYPE.H"

#include "MoX_FLIC.H"
#include "MoX_Fonts.H"      /* p_Palette, p_PaletteFlags */

#include "STU_DBG.H"


void Draw(signed short int x, signed short int y, SAMB_ptr FLIC_animation);
void FLIC_Load_Palette(SAMB_ptr FLIC_animation, word frame_num);


// ~== MoO2
// dseg02:001B42A8 animation_header dd ?
// void Draw(signed short int x, signed short int y, unsigned char * picture)
void FLIC_Draw(signed short int x, signed short int y, SAMB_ptr FLIC_animation)
{
    static struct s_FLIC_HDR PS_FLIC_Header;  // persistent, local 
    word frame_num;

    signed short int x2;
    signed short int y2;

    dbg_prn("DEBUG: [%s, %d] BEGIN: Draw(x = %d, y = %d, FLIC_animation = %p)\n", __FILE__, __LINE__, x, y, FLIC_animation);


    // ST_MoveData((unsigned int)&PS_FLIC_Header, 0, 0, sa_FLIC_Header, sizeof(PS_FLIC_Header));
    // void *memcpy(void *dest, const void * src, size_t n)
    memcpy((void *)&PS_FLIC_Header, (void *)FLIC_animation, sizeof(PS_FLIC_Header));

    frame_num = PS_FLIC_Header.current_frame;
    PS_FLIC_Header.current_frame += 1;
    if ( PS_FLIC_Header.current_frame < PS_FLIC_Header.frame_count )
    {
        // farpokew(sa_FLIC_Header, 0x04, PS_FLIC_Header.current_frame);
    }
    else
    {
        // farpokew(sa_FLIC_Header, 0x04, PS_FLIC_Header.loop_frame);
    }


    if ( PS_FLIC_Header.palette_header_offset != 0 )
    {
        FLIC_Load_Palette(FLIC_animation, frame_num);
    }

    dbg_prn("DEBUG: [%s, %d] BEGIN: FLIC_Draw(x = %d, y = %d, FLIC_animation = %p)\n", __FILE__, __LINE__, x, y, FLIC_animation);

}


// s21p07
// void FLIC_LoadPalette_Redux(SAMB_addr sa_FLIC_Header, int Frame_Index)
void FLIC_Load_Palette(SAMB_ptr FLIC_animation, word frame_num)
{
    word palette_header_offset;
    byte_ptr palette_header;
    word palette_data_offset;
    byte_ptr palette_data;
    word start;
    word count;
    byte frame_palette_flag;
    word frame_palette_header_offset;
    byte_ptr frame_palette_header;
    word frame_palette_data_offset;
    byte_ptr frame_palette_data;
    int itr;

    dbg_prn("DEBUG: [%s, %d] BEGIN: FLIC_Load_Palette(FLIC_animation = %p, frame_num = %d)\n", __FILE__, __LINE__, FLIC_animation, frame_num);

    palette_header_offset = GET_2B_OFS(FLIC_animation,0x0E);
    palette_header = &FLIC_animation[palette_header_offset];
    

    frame_palette_flag = palette_header[6];

    if ( (frame_num == 0) || (frame_palette_flag == 0) )
    {
        palette_data_offset = GET_2B_OFS(palette_header,0x00);
        start = GET_2B_OFS(palette_header,0x02);
        count = GET_2B_OFS(palette_header,0x04);
        palette_data = &FLIC_animation[palette_data_offset];
    }
    else
    {
        // FLIC Segment Address + FLIC Palette Header Offset + sizeof(FLIC Palette Header) + (4 * frame_num - 1)
        // frame_palette_data
        frame_palette_header_offset = palette_header_offset + 8 + (4 * (frame_num - 1));
        frame_palette_header = &FLIC_animation[frame_palette_header_offset];
        frame_palette_data_offset = GET_2B_OFS(frame_palette_header,0x00);
        palette_data = &FLIC_animation[frame_palette_data_offset];
        start = GET_1B_OFS(frame_palette_header,0x01);
        count = GET_1B_OFS(frame_palette_header,0x02);
    }

    for(itr = 0; itr < count; itr++)
    {
        *(p_Palette + itr) = *(palette_data + itr);
        // *(p_Palette + 768 + itr) = 1;
        *(p_PaletteFlags + itr) = 1;
    }

    dbg_prn("DEBUG: [%s, %d] END: FLIC_Load_Palette(FLIC_animation = %p, frame_num = %d)\n", __FILE__, __LINE__, FLIC_animation, frame_num);

}


// void Draw_Animation(uint8_t *pixbuf, uint16_t w, uint8_t *data, uint16_t pitch)
// {
//     uint8_t *q;
//     uint8_t b, /*dh*/mode, /*dl*/len_total, len_run;
//     while (w--) {
//         q = pixbuf++;
//         b = *data++;
//         if (b == 0xff) { /* skip column */
//             continue;
//         }
//         mode = b;
//         len_total = *data++;
//         if ((mode & 0x80) == 0) { /* regular data */
//             do {
//                 len_run = *data++;
//                 q += pitch * *data++;
//                 len_total -= len_run + 2;
//                 do {
//                     *q = *data++;
//                     q += pitch;
//                 } while (--len_run);
//             } while (len_total >= 1);
//         } else {    /* compressed data */
//             do {
//                 len_run = *data++;
//                 q += pitch * *data++;
//                 len_total -= len_run + 2;
//                 do {
//                     b = *data++;
//                     if (b > 0xdf) { /* b-0xdf pixels, same color */
//                         uint8_t len_compr;
//                         len_compr = b - 0xdf;
//                         --len_run;
//                         b = *data++;
//                         while (len_compr) {
//                             *q = b;
//                             q += pitch;
//                             --len_compr;
//                         }
//                     } else {
//                         *q = b;
//                         q += pitch;
//                     }
//                 } while (--len_run);
//             } while (len_total >= 1);
//         }
//     }
// }
