
#include "MoX_TYPE.H"     /* byte_ptr, SAMB_ptr */
#include "MoX_DEF.H"      /* DLOG() */

#include "FLIC_Draw.H"

#include "Fonts.H"    /* p_Palette() */
#include "Video.H"

// extern uint8_t g_Video_Back_Buffer[];
extern uint8_t g_Palette[];
extern uint8_t g_Palette_XBGR[];

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif



/*
    MGC  seg021
*/


// MGC s21p07
void FLIC_Load_Palette(SAMB_ptr p_FLIC_Header, int16_t frame_index)
{
    SAMB_ptr p_FLIC_File;
    int16_t start;
    int16_t count;
    byte_ptr flic_palette_data;
    int16_t itr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: FLIC_Load_Palette(p_FLIC_Header = %p, frame_index = %d)\n", __FILE__, __LINE__, p_FLIC_Header, frame_index);
#endif

    p_FLIC_File = (p_FLIC_Header + 0);  // ~== p_FLIC_File = &p_FLIC_Header[0]

    if((frame_index == 0) || (FLIC_GET_FRAME_PALETTES(p_FLIC_File) == 0))
    {
//         DLOG("( (frame_index == 0) || (frame_palettes == 0) )");
        start = FLIC_GET_PALETTE_COLOR_START(p_FLIC_File);
        count = FLIC_GET_PALETTE_COLOR_COUNT(p_FLIC_File);
        flic_palette_data = (p_FLIC_File + FLIC_GET_PALETTE_DATA_OFFSET(p_FLIC_File));
    }
    else
    {
//         DLOG("( (frame_index != 0) || (frame_palettes != 0) )");
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: FLIC_Load_Palette(p_FLIC_Header = %p, frame_index = %d)\n", __FILE__, __LINE__, p_FLIC_Header, frame_index);
#endif

}



/*
    MAGIC.EXE  seg027
*/

// MGC s27p01
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: FLIC_Draw_Frame(x_start = %d, y_start = %d, width = %d, frame_data = %p)\n", __FILE__, __LINE__, x_start, y_start, width, frame_data);
#endif

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] g_Video_Back_Buffer: %p\n", __FILE__, __LINE__, g_Video_Back_Buffer);
// #endif

    // bbuff_pos = g_Video_Back_Buffer + ((y_start * 320) + x_start);
    bbuff_pos = current_video_page + ((y_start * 320) + x_start);

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] bbuff_pos: %p\n", __FILE__, __LINE__, bbuff_pos);
// #endif


    while (width--)
    {
        bbuff = bbuff_pos++;
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] frame_data: %p\n", __FILE__, __LINE__, frame_data);
//     dbg_prn("DEBUG: [%s, %d] bbuff: %p\n", __FILE__, __LINE__, bbuff);
// #endif
        
        packet_op = *frame_data++;
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] packet_op: 0x%02X\n", __FILE__, __LINE__, packet_op);
// #endif

        if(packet_op == 0xFF)  /* Type: skip */
        {
            continue;
        }

        packet_byte_count = *frame_data++;

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] packet_byte_count: 0x%02X\n", __FILE__, __LINE__, packet_byte_count);
// #endif

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
            // DLOG("BEGIN: (packet_op == 0x00)");
            do {
                sequence_byte_count = *frame_data++;
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] sequence_byte_count: 0x%02X\n", __FILE__, __LINE__, sequence_byte_count);
// #endif
                delta_byte_count = *frame_data++;
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] delta_byte_count: 0x%02X\n", __FILE__, __LINE__, delta_byte_count);
// #endif
                bbuff += (320 * delta_byte_count);
                packet_byte_count -= sequence_byte_count + 2;
                while(sequence_byte_count--)
                {
                    *bbuff = *frame_data++;
                    bbuff += 320;
                }
            } while(packet_byte_count >= 1);
            // DLOG("END: (packet_op == 0x00)");
        }

    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: FLIC_Draw_Frame(x_start = %d, y_start = %d, width = %d, frame_data = %p)\n", __FILE__, __LINE__, x_start, y_start, width, frame_data);
#endif

}



/*
    MAGIC.EXE   seg028
    WIZARDS.EXE seg030
*/

// WZD s30p06
void Create_Blank_Picture(int16_t width, int16_t height, byte_ptr pict_seg, uint8_t color)
{
    int16_t length;
    int16_t itr_length;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Create_Blank_Picture(width = %d, height = %d, pict_seg = %p, color = %d)\n", __FILE__, __LINE__, width, height, pict_seg, color);
#endif

    // FLIC_SET_WIDTH(pict_seg, width);
    // FLIC_SET_HEIGHT(pict_seg, height);
    // FLIC_SET_CURRENT_FRAME(pict_seg,0xDE0A);  // LBX_DecodedIMG
    // FLIC_SET_FRAME_COUNT(pict_seg, 0);
    // FLIC_SET_LOOP_FRAME(pict_seg,0);

    SET_2B_OFS(pict_seg,0,width);
    SET_2B_OFS(pict_seg,2,height);
    SET_2B_OFS(pict_seg,4,0xDE0A);
    SET_2B_OFS(pict_seg,6,0);
    SET_2B_OFS(pict_seg,8,0);

    length = width * height;
    
    itr_length = 0;
    while(itr_length++ < length)
    {
        *(pict_seg + 16 + itr_length) = color;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Create_Blank_Picture(width = %d, height = %d, pict_seg = %p, color = %d)\n", __FILE__, __LINE__, width, height, pict_seg, color);
#endif
}

// MGC s28p11
// WZD s30p11
void FLIC_Draw(int16_t x_start, int16_t y_start, SAMB_ptr p_FLIC_File)
{
    int16_t current_frame_index;
    int16_t next_frame_index;
    unsigned int flic_frame_offset;
    unsigned short flic_frame_offset_sgmt;
    unsigned short flic_frame_offset_ofst;
    byte_ptr p_FLIC_Frame;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: FLIC_Draw(x_start = %d, y_start = %d, p_FLIC_File = %p)\n", __FILE__, __LINE__, x_start, y_start, p_FLIC_File);
#endif

    // ? MEM_Copy_Far(&flic_hdr, 0, 0, p_FLIC_File, 16) ?

    current_frame_index = FLIC_GET_CURRENT_FRAME(p_FLIC_File);
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] current_frame_index: %d\n", __FILE__, __LINE__, current_frame_index);
// #endif
    next_frame_index = FLIC_GET_CURRENT_FRAME(p_FLIC_File) + 1;
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] next_frame_index: %d\n", __FILE__, __LINE__, next_frame_index);
// #endif
    if(next_frame_index < FLIC_GET_FRAME_COUNT(p_FLIC_File))
    {
//         DLOG("(next_frame_index < FLIC_GET_FRAME_COUNT(p_FLIC_File))");
        FLIC_SET_CURRENT_FRAME(p_FLIC_File, next_frame_index);
    }
    else
    {
//         DLOG("(next_frame_index >= FLIC_GET_FRAME_COUNT(p_FLIC_File))");
        FLIC_SET_CURRENT_FRAME(p_FLIC_File, FLIC_GET_LOOP_FRAME(p_FLIC_File));
    }
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] FLIC_GET_CURRENT_FRAME(p_FLIC_File): %d\n", __FILE__, __LINE__, FLIC_GET_CURRENT_FRAME(p_FLIC_File));
// #endif

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
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] flic_frame_offset: 0x%08X\n", __FILE__, __LINE__, flic_frame_offset);
//     dbg_prn("DEBUG: [%s, %d] p_FLIC_Frame: %p\n", __FILE__, __LINE__, p_FLIC_Frame);
// #endif

    FLIC_Draw_Frame(x_start, y_start, FLIC_GET_WIDTH(p_FLIC_File), p_FLIC_Frame);


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: FLIC_Draw(x_start = %d, y_start = %d, p_FLIC_File = %p)\n", __FILE__, __LINE__, x_start, y_start, p_FLIC_File);
#endif
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Draw_Picture_To_Bitmap()\n", __FILE__, __LINE__);
#endif

    /*
        BEGIN: same as FLIC_Draw()
    */
    // MEM_Copy_Far(src sgmt ofst dst sgmt ofst)
    // ? MEM_Copy_Far(&flic_hdr, 0, 0, p_FLIC_File, 16) ?

    current_frame_index = FLIC_GET_CURRENT_FRAME(src_pict_seg);
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] current_frame_index: %d\n", __FILE__, __LINE__, current_frame_index);
// #endif
    next_frame_index = FLIC_GET_CURRENT_FRAME(src_pict_seg) + 1;
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] next_frame_index: %d\n", __FILE__, __LINE__, next_frame_index);
// #endif
    if(next_frame_index < FLIC_GET_FRAME_COUNT(src_pict_seg))
    {
//         DLOG("(next_frame_index < FLIC_GET_FRAME_COUNT(src_pict_seg))");
        FLIC_SET_CURRENT_FRAME(src_pict_seg, next_frame_index);
    }
    else
    {
//         DLOG("(next_frame_index >= FLIC_GET_FRAME_COUNT(src_pict_seg))");
        FLIC_SET_CURRENT_FRAME(src_pict_seg, FLIC_GET_LOOP_FRAME(src_pict_seg));
    }
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] FLIC_GET_CURRENT_FRAME(src_pict_seg): %d\n", __FILE__, __LINE__, FLIC_GET_CURRENT_FRAME(src_pict_seg));
// #endif

    if((FLIC_GET_PALETTE_HEADER_OFFSET(src_pict_seg) != 0))
    {
        FLIC_Load_Palette(src_pict_seg, current_frame_index);
    }
    /*
        END: same as FLIC_Draw()
    */

   // WZD s33p01
   // AKA LBX_IMG_DecPrepare()
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: dst_pict_seg: %p\n", __FILE__, __LINE__, dst_pict_seg);
// #endif
   Create_Multi_Frame_Blank_Bitmap(src_pict_seg, dst_pict_seg, current_frame_index);

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: dst_pict_seg: %p\n", __FILE__, __LINE__, dst_pict_seg);
//     dbg_prn("DEBUG: [%s, %d]: GET_2B_OFS(dst_pict_seg, 0): %d\n", __FILE__, __LINE__, GET_2B_OFS(dst_pict_seg, 0));
//     dbg_prn("DEBUG: [%s, %d]: GET_2B_OFS(dst_pict_seg, 2): %d\n", __FILE__, __LINE__, GET_2B_OFS(dst_pict_seg, 2));
//     dbg_prn("DEBUG: [%s, %d]: GET_2B_OFS(dst_pict_seg, 4): %d\n", __FILE__, __LINE__, GET_2B_OFS(dst_pict_seg, 4));
//     dbg_prn("DEBUG: [%s, %d]: GET_2B_OFS(dst_pict_seg, 6): %d\n", __FILE__, __LINE__, GET_2B_OFS(dst_pict_seg, 6));
//     dbg_prn("DEBUG: [%s, %d]: GET_2B_OFS(dst_pict_seg, 8): %d\n", __FILE__, __LINE__, GET_2B_OFS(dst_pict_seg, 7));
//     dbg_prn("DEBUG: [%s, %d]: GET_2B_OFS(dst_pict_seg, 10): %d\n", __FILE__, __LINE__, GET_2B_OFS(dst_pict_seg, 10));
//     dbg_prn("DEBUG: [%s, %d]: GET_2B_OFS(dst_pict_seg, 12): %d\n", __FILE__, __LINE__, GET_2B_OFS(dst_pict_seg, 12));
//     dbg_prn("DEBUG: [%s, %d]: GET_2B_OFS(dst_pict_seg, 14): %d\n", __FILE__, __LINE__, GET_2B_OFS(dst_pict_seg, 14));
// #endif


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
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] full_store_flag: %d\n", __FILE__, __LINE__, full_store_flag);
// #endif

    if(full_store_flag != ST_FALSE)
    {
        // _DI = _SI  (_SI = src_pict_seg->CurrentFrame)
        frame_start = current_frame_index;
    }
    else
    {
        // _DI = 0
        frame_start = 0;
    }
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] frame_start: %d\n", __FILE__, __LINE__, frame_start);
// #endif

    frame_idx = current_frame_index;
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] frame_idx: %d\n", __FILE__, __LINE__, frame_idx);
// #endif

    for(itr_frames = frame_start; itr_frames <= frame_idx; itr_frames++)
    {
        /*
            BEGIN: same as FLIC_Draw()
        */
        flic_frame_offset = FLIC_GET_FRAME_OFFSET(src_pict_seg, current_frame_index);
        // should be 0000001A for all UNITS_.LBX
        p_FLIC_Frame = (src_pict_seg + (flic_frame_offset + 1));
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] flic_frame_offset: 0x%08X\n", __FILE__, __LINE__, flic_frame_offset);
//     dbg_prn("DEBUG: [%s, %d] p_FLIC_Frame: %p\n", __FILE__, __LINE__, p_FLIC_Frame);
// #endif
        /*
            END: same as FLIC_Draw()
        */

        // src frame data
        // dst bitmap seg, with header
        Add_Picture_To_Bitmap(p_FLIC_Frame, dst_pict_seg);

    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Draw_Picture_To_Bitmap()\n", __FILE__, __LINE__);
#endif
}

// WZD s30p14
// MGC s28p14
void FLIC_Set_CurrentFrame(SAMB_ptr p_FLIC_Header, int16_t frame_index)
{
    int16_t loop_length;
    int16_t loop_frame;
    int16_t frame_count;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: FLIC_Set_CurrentFrame(p_FLIC_Header = %p, frame_index = %d)\n", __FILE__, __LINE__, p_FLIC_Header, frame_index);
#endif

    frame_index = (frame_index & 0x7fff);
    frame_count = FLIC_GET_FRAME_COUNT(p_FLIC_Header);
    loop_frame = FLIC_GET_LOOP_FRAME(p_FLIC_Header);
    loop_length = frame_count - loop_frame;
    if(!(frame_index < frame_count))
    {
        frame_index = loop_frame + ((frame_index - frame_count) % loop_length);
    }
    FLIC_SET_CURRENT_FRAME(p_FLIC_Header, frame_index);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: FLIC_Set_CurrentFrame(p_FLIC_Header = %p, frame_index = %d)\n", __FILE__, __LINE__, p_FLIC_Header, frame_index);
#endif

}

// MGC s28p15
void FLIC_Reset_CurrentFrame(SAMB_ptr p_FLIC_Header)
{

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: FLIC_Reset_CurrentFrame(p_FLIC_Header = %p)\n", __FILE__, __LINE__, p_FLIC_Header);
#endif

    FLIC_SET_CURRENT_FRAME(p_FLIC_Header, 0);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: FLIC_Reset_CurrentFrame(p_FLIC_Header = %p)\n", __FILE__, __LINE__, p_FLIC_Header);
#endif

}

// MGC s28p16
int16_t FLIC_Get_CurrentFrame(SAMB_ptr p_FLIC_Header)
{
    int16_t current_frame_index;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: FLIC_Get_CurrentFrame(p_FLIC_Header = %p)\n", __FILE__, __LINE__, p_FLIC_Header);
#endif

    current_frame_index = FLIC_GET_CURRENT_FRAME(p_FLIC_Header);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: FLIC_Get_CurrentFrame(p_FLIC_Header = %p) { current_frame_index = %d }\n", __FILE__, __LINE__, p_FLIC_Header, current_frame_index);
#endif

    return current_frame_index;
}

// WZD s30p17
// ?NIU? int16_t FLIC_Get_FrameCount()

// WZD s30p18
int16_t FLIC_Get_Width(SAMB_ptr p_FLIC_Header)
{
    int16_t flic_width;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: FLIC_Get_Width(p_FLIC_Header = %p)\n", __FILE__, __LINE__, p_FLIC_Header);
#endif

    flic_width = FLIC_GET_WIDTH(p_FLIC_Header);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: FLIC_Get_Width(p_FLIC_Header = %p) { flic_width = %d }\n", __FILE__, __LINE__, p_FLIC_Header, flic_width);
#endif

    return flic_width;
}

// WZD s30p19
int16_t FLIC_Get_Height(SAMB_ptr p_FLIC_Header)
{
    int16_t flic_height;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: FLIC_Get_Height(p_FLIC_Header = %p)\n", __FILE__, __LINE__, p_FLIC_Header);
#endif

    flic_height = FLIC_GET_HEIGHT(p_FLIC_Header);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: FLIC_Get_Height(p_FLIC_Header = %p) { flic_height = %d }\n", __FILE__, __LINE__, p_FLIC_Header, flic_height);
#endif

    return flic_height;
}


// WZD s30p24
void Draw_Picture(int16_t x, int16_t y, byte_ptr pict_seg)
{
    int16_t height;
    int16_t width;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Draw_Picture(x = %d, y = %d, pict_seg = %p)\n", __FILE__, __LINE__, x, y, pict_seg);
#endif

    width = GET_2B_OFS(pict_seg, 0);
    height = GET_2B_OFS(pict_seg, 2);

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] width: %d\n", __FILE__, __LINE__, width);
//     dbg_prn("DEBUG: [%s, %d] height: %d\n", __FILE__, __LINE__, height);
// #endif

    // void Draw_Picture_ASM(int16_t x_start, int16_t y_start, int16_t ofst, byte_ptr pict_seg, int16_t width, int16_t height, int16_t skip_x)
    Draw_Picture_ASM(x, y, 16, pict_seg, width, height, 0);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Draw_Picture(x = %d, y = %d, pict_seg = %p)\n", __FILE__, __LINE__, x, y, pict_seg);
#endif
}


// WZD s30p40
int16_t Get_Full_Store_Flag(SAMB_ptr p_FLIC_Header)
{
    int16_t full_store_flag;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Get_Full_Store_Flag(p_FLIC_Header = %p)\n", __FILE__, __LINE__, p_FLIC_Header);
#endif

    full_store_flag = FLIC_GET_FRAME_TYPE(p_FLIC_Header);

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] full_store_flag: %d\n", __FILE__, __LINE__, full_store_flag);
// #endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Get_Full_Store_Flag(p_FLIC_Header = %p) { full_store_flag = %d }\n", __FILE__, __LINE__, p_FLIC_Header, full_store_flag);
#endif

    return full_store_flag;
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Create_Multi_Frame_Blank_Bitmap()\n", __FILE__, __LINE__);
#endif

    src_width = GET_2B_OFS(src_pict_seg, 0);
    src_height = GET_2B_OFS(src_pict_seg, 2);

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: src_width: %d\n", __FILE__, __LINE__, src_width);
//     dbg_prn("DEBUG: [%s, %d]: src_height: %d\n", __FILE__, __LINE__, src_height);
// #endif


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

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: dst_pict_seg: %p\n", __FILE__, __LINE__, dst_pict_seg);
//     dbg_prn("DEBUG: [%s, %d]: GET_2B_OFS(dst_pict_seg, 0): %d\n", __FILE__, __LINE__, GET_2B_OFS(dst_pict_seg, 0));
//     dbg_prn("DEBUG: [%s, %d]: GET_2B_OFS(dst_pict_seg, 2): %d\n", __FILE__, __LINE__, GET_2B_OFS(dst_pict_seg, 2));
// #endif


    length = src_width * src_height;
    word_length = ((length + 1) / 2);
    word_ptr = (uint16_t *)(dst_pict_seg + 16);
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: length: %d\n", __FILE__, __LINE__, length);
//     dbg_prn("DEBUG: [%s, %d]: word_length: %d\n", __FILE__, __LINE__, word_length);
//     dbg_prn("DEBUG: [%s, %d]: word_ptr: %p\n", __FILE__, __LINE__, word_ptr);
// #endif
    while(word_length)
    {
        *word_ptr = 0;
        word_length--;
    }


// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: dst_pict_seg: %p\n", __FILE__, __LINE__, dst_pict_seg);
//     dbg_prn("DEBUG: [%s, %d]: GET_2B_OFS(dst_pict_seg, 0): %d\n", __FILE__, __LINE__, GET_2B_OFS(dst_pict_seg, 0));
//     dbg_prn("DEBUG: [%s, %d]: GET_2B_OFS(dst_pict_seg, 2): %d\n", __FILE__, __LINE__, GET_2B_OFS(dst_pict_seg, 2));
// #endif


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Create_Multi_Frame_Blank_Bitmap()\n", __FILE__, __LINE__);
#endif
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Add_Picture_To_Bitmap()\n", __FILE__, __LINE__);
#endif

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: source_picture: %p\n", __FILE__, __LINE__, source_picture);
//     dbg_prn("DEBUG: [%s, %d]: destination_bitmap: %p\n", __FILE__, __LINE__, destination_bitmap);
// #endif

    width = FLIC_GET_WIDTH(destination_bitmap);
    height = FLIC_GET_HEIGHT(destination_bitmap);

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: width: %d\n", __FILE__, __LINE__, width);
//     dbg_prn("DEBUG: [%s, %d]: height: %d\n", __FILE__, __LINE__, height);
// #endif

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
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: dst_base: %p\n", __FILE__, __LINE__, dst_base);
//     dbg_prn("DEBUG: [%s, %d]: dst_curr: %p\n", __FILE__, __LINE__, dst_curr);
//     dbg_prn("DEBUG: [%s, %d]: dst_diff: %u\n", __FILE__, __LINE__, dst_diff);
//     dbg_prn("DEBUG: [%s, %d]: dst_w: %d\n", __FILE__, __LINE__, dst_w);
//     dbg_prn("DEBUG: [%s, %d]: dst_h: %d\n", __FILE__, __LINE__, dst_h);
//     dbg_prn("DEBUG: [%s, %d]: dst_x: %d\n", __FILE__, __LINE__, dst_x);
//     dbg_prn("DEBUG: [%s, %d]: dst_y: %d\n", __FILE__, __LINE__, dst_y);
// #endif



    while(width--)
    {
        dst = dst_pos;
        dst_pos += height;
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: src: %p\n", __FILE__, __LINE__, src);
//     dbg_prn("DEBUG: [%s, %d]: dst: %p\n", __FILE__, __LINE__, dst);
// #endif

        packet_op = *src++;
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: packet_op: %02X\n", __FILE__, __LINE__, packet_op);
// #endif

        if(packet_op == 0xFF)
        {
            dst += height;

// #ifdef STU_DEBUG
//     dst_diff = dst - dst_base;
//     dst_x = dst_diff / dst_h;
//     dst_y = dst_diff - (dst_x * dst_h);
//     dbg_prn("DEBUG: [%s, %d]: dst_diff: %u\n", __FILE__, __LINE__, dst_diff);
//     dbg_prn("DEBUG: [%s, %d]: dst_x: %d\n", __FILE__, __LINE__, dst_x);
//     dbg_prn("DEBUG: [%s, %d]: dst_y: %d\n", __FILE__, __LINE__, dst_y);
// #endif
            // width--;
            continue;
        }

        packet_byte_count = *src++;
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: packet_byte_count: %02X\n", __FILE__, __LINE__, packet_byte_count);
// #endif

        if(packet_op == 0x80)  /* Type: decode */
        {
//             DLOG("BEGIN: (packet_op == 0x80)");

            do {
                sequence_byte_count = *src++;
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: sequence_byte_count: %02X\n", __FILE__, __LINE__, sequence_byte_count);
// #endif
                delta_byte_count = *src++;
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: delta_byte_count: %02X\n", __FILE__, __LINE__, delta_byte_count);
// #endif
                dst += delta_byte_count;
// #ifdef STU_DEBUG
//     dst_diff = dst - dst_base;
//     dbg_prn("DEBUG: [%s, %d]: dst_diff: %u\n", __FILE__, __LINE__, dst_diff);
// #endif
                packet_byte_count -= sequence_byte_count + 2;
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: packet_byte_count: %02X\n", __FILE__, __LINE__, packet_byte_count);
// #endif
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
// #ifdef STU_DEBUG
//     dst_diff = dst - dst_base;
//     dst_x = dst_diff / dst_h;
//     dst_y = dst_diff - (dst_x * dst_h);
//     dbg_prn("DEBUG: [%s, %d]: dst_diff: %u\n", __FILE__, __LINE__, dst_diff);
//     dbg_prn("DEBUG: [%s, %d]: dst_x: %d\n", __FILE__, __LINE__, dst_x);
//     dbg_prn("DEBUG: [%s, %d]: dst_y: %d\n", __FILE__, __LINE__, dst_y);
// #endif
                        }
                    }
                    else  /* op: copy */
                    {
                        *dst = data_byte;
                        dst++;
// #ifdef STU_DEBUG
//     dst_diff = dst - dst_base;
//     dst_x = dst_diff / dst_h;
//     dst_y = dst_diff - (dst_x * dst_h);
//     dbg_prn("DEBUG: [%s, %d]: dst_diff: %u\n", __FILE__, __LINE__, dst_diff);
//     dbg_prn("DEBUG: [%s, %d]: dst_x: %d\n", __FILE__, __LINE__, dst_x);
//     dbg_prn("DEBUG: [%s, %d]: dst_y: %d\n", __FILE__, __LINE__, dst_y);
// #endif
                    }

                }
            } while(packet_byte_count >= 1);

//             DLOG("END: (packet_op == 0x80)");
        }

        if(packet_op == 0x00)  /* Type: copy */
        {
//             DLOG("BEGIN: (packet_op == 0x00)");

            do {
                sequence_byte_count = *src++;
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: sequence_byte_count: %02X\n", __FILE__, __LINE__, sequence_byte_count);
// #endif
                delta_byte_count = *src++;
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: delta_byte_count: %02X\n", __FILE__, __LINE__, delta_byte_count);
// #endif
                dst += delta_byte_count;
// #ifdef STU_DEBUG
//     dst_diff = dst - dst_base;
//     dst_x = dst_diff / dst_h;
//     dst_y = dst_diff - (dst_x * dst_h);
//     dbg_prn("DEBUG: [%s, %d]: dst_diff: %u\n", __FILE__, __LINE__, dst_diff);
//     dbg_prn("DEBUG: [%s, %d]: dst_x: %d\n", __FILE__, __LINE__, dst_x);
//     dbg_prn("DEBUG: [%s, %d]: dst_y: %d\n", __FILE__, __LINE__, dst_y);
// #endif

                packet_byte_count -= sequence_byte_count + 2;
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: packet_byte_count: %02X\n", __FILE__, __LINE__, packet_byte_count);
// #endif
                while(sequence_byte_count--)
                {
                    // *dst++ = *src++;
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: *src: %02X\n", __FILE__, __LINE__, *src);
// #endif
                    *dst = *src;
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: *dst: %02X\n", __FILE__, __LINE__, *dst);
// #endif
                    src++;
                    dst++;
// #ifdef STU_DEBUG
//     dst_diff = dst - dst_base;
//     dst_x = dst_diff / dst_h;
//     dst_y = dst_diff - (dst_x * dst_h);
//     dbg_prn("DEBUG: [%s, %d]: dst_diff: %u\n", __FILE__, __LINE__, dst_diff);
//     dbg_prn("DEBUG: [%s, %d]: dst_x: %d\n", __FILE__, __LINE__, dst_x);
//     dbg_prn("DEBUG: [%s, %d]: dst_y: %d\n", __FILE__, __LINE__, dst_y);
// #endif
                }
            } while(packet_byte_count >= 1);

//             DLOG("END: (packet_op == 0x00)");
        }

    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Add_Picture_To_Bitmap()\n", __FILE__, __LINE__);
#endif
}


// WZD s33p05
void Draw_Picture_ASM(int16_t x_start, int16_t y_start, int16_t ofst, byte_ptr pict_seg, int16_t width, int16_t height, int16_t skip_x)
{

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Draw_Picture_ASM(x_start = %d, y_start = %d, ofst = %d, pict_seg = %p, width = %d, height = %d, skip_x = %d)\n", __FILE__, __LINE__, x_start, y_start, ofst, pict_seg, width, height, skip_x);
#endif

    byte_ptr screen_start;
    byte_ptr screen_pos;
    byte_ptr data;
    uint8_t data_byte;
    int16_t itr_width;
    int16_t itr_height;
    
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] current_video_page: %p\n", __FILE__, __LINE__, current_video_page);
// #endif
    screen_start = current_video_page + (y_start * 320) + x_start;
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] screen_start: %p\n", __FILE__, __LINE__, screen_start);
// #endif

    data = pict_seg + ofst;
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] data: %p\n", __FILE__, __LINE__, data);
// #endif

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
//     dbg_prn("DEBUG: [%s, %d] data_byte: %02X\n", __FILE__, __LINE__, data_byte);
// #endif

            if(data_byte != 0x00)  /* color index 0 TRANSPARENT */
            {
                *screen_pos = data_byte;
            }
            screen_pos += 320;  // add screen line stride - same column, one row down

            itr_height--;
        }
        // TODO(jimBalcomb,20230127): ? add skip_x ?
        // data_pos += skipx

        itr_width--;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Draw_Picture_ASM(x_start = %d, y_start = %d, ofst = %d, pict_seg = %p, width = %d, height = %d, skip_x = %d)\n", __FILE__, __LINE__, x_start, y_start, ofst, pict_seg, width, height, skip_x);
#endif
}
