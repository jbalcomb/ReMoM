
#include "win_TYPE.hpp"     /* byte_ptr, SAMB_ptr */
#include "win_DEF.hpp"      /* DLOG() */

#include "win_FLIC.hpp"

#include "win_Fonts.hpp"    /* p_Palette() */

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif

extern unsigned char video_back_buffer[];


/*
    MAGIC.EXE  seg021
*/

// MGC s21p07
void FLIC_Load_Palette(SAMB_ptr p_FLIC_Header, short frame_index)
{
    SAMB_ptr p_FLIC_File;
    unsigned int Color_Index;
    unsigned int Color_Count;
    byte_ptr flic_palette_data;
    unsigned int itr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: FLIC_Load_Palette(p_FLIC_Header = %p, frame_index = %d)\n", __FILE__, __LINE__, p_FLIC_Header, frame_index);
#endif

    p_FLIC_File = (p_FLIC_Header + 0);  // ~== p_FLIC_File = &p_FLIC_Header[0]

    if((frame_index == 0) || (FLIC_GET_FRAME_PALETTES(p_FLIC_File) == 0))
    {
        DLOG("( (frame_index == 0) || (frame_palettes == 0) )");
        Color_Index = FLIC_GET_PALETTE_COLOR_START(p_FLIC_File);
        Color_Count = FLIC_GET_PALETTE_COLOR_COUNT(p_FLIC_File);
        flic_palette_data = (p_FLIC_File + FLIC_GET_PALETTE_DATA_OFFSET(p_FLIC_File));
    }
    else
    {
        DLOG("( (frame_index != 0) || (frame_palettes != 0) )");
        flic_palette_data = (p_FLIC_File + FLIC_GET_FRAME_PALETTE_DATA_OFFSET(p_FLIC_File,frame_index));
        Color_Index = FLIC_GET_FRAME_PALETTE_COLOR_INDEX(p_FLIC_File,frame_index);
        Color_Count = FLIC_GET_FRAME_PALETTE_COLOR_COUNT(p_FLIC_File,frame_index);
    }

    for(itr = Color_Index; itr < Color_Count; itr++)
    {
        *(p_Palette + itr) = *(flic_palette_data + itr);
        *(p_Palette + 768 + itr) = 1;
    }

    for(itr = 0; itr < Color_Count; itr++)
    {
        // *(p_Palette_XBGR + (Color_Index * 4) + (itr * 4) + 3) = (*(flic_palette_data + (Color_Index * 3) + (itr * 3) + 0) << 2);
        // *(p_Palette_XBGR + (Color_Index * 4) + (itr * 4) + 2) = (*(flic_palette_data + (Color_Index * 3) + (itr * 3) + 1) << 2);
        // *(p_Palette_XBGR + (Color_Index * 4) + (itr * 4) + 1) = (*(flic_palette_data + (Color_Index * 3) + (itr * 3) + 2) << 2);
        // *(p_Palette_XBGR + (Color_Index * 4) + (itr * 4) + 0) = 0x00;
        *(p_Palette_XBGR + (Color_Index * 4) + (itr * 4) + 3) = 0x00;
        *(p_Palette_XBGR + (Color_Index * 4) + (itr * 4) + 2) = (*(flic_palette_data + (Color_Index * 3) + (itr * 3) + 0) << 2);
        *(p_Palette_XBGR + (Color_Index * 4) + (itr * 4) + 1) = (*(flic_palette_data + (Color_Index * 3) + (itr * 3) + 1) << 2);
        *(p_Palette_XBGR + (Color_Index * 4) + (itr * 4) + 0) = (*(flic_palette_data + (Color_Index * 3) + (itr * 3) + 2) << 2);
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: FLIC_Load_Palette(p_FLIC_Header = %p, frame_index = %d)\n", __FILE__, __LINE__, p_FLIC_Header, frame_index);
#endif

}



/*
    MAGIC.EXE  seg027
*/

// MGC s27p01
void FLIC_Draw_Frame(int x_start, int y_start, int width, byte_ptr frame_data)
{
    unsigned char * bbuff_pos;
    unsigned char * bbuff;
    unsigned char data_byte;

    unsigned char packet_op;
    unsigned char packet_byte_count;
    unsigned char sequence_byte_count;
    unsigned char delta_byte_count;
    unsigned char itr_op_repeat;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: FLIC_Draw_Frame(x_start = %d, y_start = %d, width = %d, frame_data = %p)\n", __FILE__, __LINE__, x_start, y_start, width, frame_data);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] video_back_buffer: %p\n", __FILE__, __LINE__, video_back_buffer);
#endif

    bbuff_pos = video_back_buffer + ((y_start * 320) + x_start);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] bbuff_pos: %p\n", __FILE__, __LINE__, bbuff_pos);
#endif


    while (width--)
    {
        bbuff = bbuff_pos++;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] frame_data: %p\n", __FILE__, __LINE__, frame_data);
    dbg_prn("DEBUG: [%s, %d] bbuff: %p\n", __FILE__, __LINE__, bbuff);
#endif
        
        packet_op = *frame_data++;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] packet_op: 0x%02X\n", __FILE__, __LINE__, packet_op);
#endif

        if(packet_op == 0xFF)  /* Type: skip */
        {
            continue;
        }

        packet_byte_count = *frame_data++;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] packet_byte_count: 0x%02X\n", __FILE__, __LINE__, packet_byte_count);
#endif

        if(packet_op == 0x80)  /* Type: decode */
        {
            do {
                sequence_byte_count = *frame_data++;
                delta_byte_count = *frame_data++;
                bbuff += (320 * delta_byte_count);
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
                            bbuff += 320;
                        }
                    }
                    else  /* op: copy */
                    {
                        *bbuff = data_byte;
                        bbuff += 320;
                    }
                }
            } while(packet_byte_count >= 1);
        }

        if(packet_op == 0x00)  /* Type: copy */
        {
            DLOG("BEGIN: (packet_op == 0x00)");
            do {
                sequence_byte_count = *frame_data++;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] sequence_byte_count: 0x%02X\n", __FILE__, __LINE__, sequence_byte_count);
#endif
                delta_byte_count = *frame_data++;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] delta_byte_count: 0x%02X\n", __FILE__, __LINE__, delta_byte_count);
#endif
                bbuff += (320 * delta_byte_count);
                packet_byte_count -= sequence_byte_count + 2;
                while(sequence_byte_count--)
                {
                    *bbuff = *frame_data++;
                    bbuff += 320;
                }
            } while(packet_byte_count >= 1);
            DLOG("END: (packet_op == 0x00)");
        }

    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: FLIC_Draw_Frame(x_start = %d, y_start = %d, width = %d, frame_data = %p)\n", __FILE__, __LINE__, x_start, y_start, width, frame_data);
#endif

}



/*
    MAGIC.EXE  seg028
*/

// MGC s28p11
void FLIC_Draw(int x_start, int y_start, SAMB_ptr p_FLIC_File)
{
    short current_frame_index;
    short next_frame_index;
    unsigned int flic_frame_offset;
    unsigned short flic_frame_offset_sgmt;
    unsigned short flic_frame_offset_ofst;
    byte_ptr p_FLIC_Frame;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: FLIC_Draw(x_start = %d, y_start = %d, p_FLIC_File = %p)\n", __FILE__, __LINE__, x_start, y_start, p_FLIC_File);
#endif

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


//     flic_frame_offset = FLIC_GET_FRAME_OFFSET(p_FLIC_File, current_frame_index);
//     flic_frame_offset_sgmt = (flic_frame_offset >> 4);          // ~== flic_frame_offset / 16
//     flic_frame_offset_ofst = ((flic_frame_offset & 0x0F) + 1);  // ~== flic_frame_offset % 16
//     p_FLIC_Frame = p_FLIC_File + (flic_frame_offset_sgmt + flic_frame_offset_ofst);
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] flic_frame_offset: 0x%08X\n", __FILE__, __LINE__, flic_frame_offset);
//     dbg_prn("DEBUG: [%s, %d] flic_frame_offset_sgmt: 0x%04X\n", __FILE__, __LINE__, flic_frame_offset_sgmt);
//     dbg_prn("DEBUG: [%s, %d] flic_frame_offset_ofst: 0x%04X\n", __FILE__, __LINE__, flic_frame_offset_ofst);
//     dbg_prn("DEBUG: [%s, %d] p_FLIC_Frame: %p\n", __FILE__, __LINE__, p_FLIC_Frame);
// #endif
    flic_frame_offset = FLIC_GET_FRAME_OFFSET(p_FLIC_File, current_frame_index);
    p_FLIC_Frame = p_FLIC_File + (flic_frame_offset) + 1;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] flic_frame_offset: 0x%08X\n", __FILE__, __LINE__, flic_frame_offset);
    dbg_prn("DEBUG: [%s, %d] p_FLIC_Frame: %p\n", __FILE__, __LINE__, p_FLIC_Frame);
#endif

    FLIC_Draw_Frame(x_start, y_start, FLIC_GET_WIDTH(p_FLIC_File), p_FLIC_Frame);


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: FLIC_Draw(x_start = %d, y_start = %d, p_FLIC_File = %p)\n", __FILE__, __LINE__, x_start, y_start, p_FLIC_File);
#endif
}

// MGC s28p14
void FLIC_Set_CurrentFrame(SAMB_ptr p_FLIC_Header, short frame_index)
{
    short loop_length;
    short loop_frame;
    short frame_count;

    frame_index = (frame_index & 0x7fff);

    frame_count = FLIC_GET_FRAME_COUNT(p_FLIC_Header);

    loop_frame = FLIC_GET_LOOP_FRAME(p_FLIC_Header);

    loop_length = frame_count - loop_frame;

    if(!(frame_index < frame_count))
    {
        frame_index = loop_frame + ((frame_index - frame_count) % loop_length);
    }

    FLIC_SET_CURRENT_FRAME(p_FLIC_Header, frame_index);
}

// MGC s28p15
void FLIC_Reset_CurrentFrame(SAMB_ptr p_FLIC_Header)
{
    FLIC_SET_CURRENT_FRAME(p_FLIC_Header, 0);
}

// MGC s28p16
short FLIC_Get_CurrentFrame(SAMB_ptr p_FLIC_Header)
{
    short current_frame_index;

    current_frame_index = FLIC_GET_CURRENT_FRAME(p_FLIC_Header);

    return current_frame_index;
}
