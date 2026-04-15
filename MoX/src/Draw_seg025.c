/*
    MAGIC.EXE
        seg025
    WIZARDS.EXE
        seg025
            VGA File Animation Drawing

*/
#include "../../ext/stu_compat.h"

#include "file_ani.h"
#include "FLIC_Draw.h"
#include "MOX_TYPE.h"
#include "EMS/EMS.h"

#include "Draw_seg025.h"



static int16_t CS_file_animation_width;
static int16_t CS_file_animation_emm_logical_page;
static EMM_HANDLE CS_file_animation_emm_handle;



/*
    WZD  seg025
*/

// WZD s25p01
// AKA Draw_File_Animation_Frame_ASM()
void Draw_File_Animation_Frame(int16_t x, int16_t y, SAMB_ptr file_animation, int16_t frame_num)
{
    byte_ptr frame_data = 0;
    byte_ptr frame_data_base = 0;
    byte_ptr bbuff_pos = 0;
    byte_ptr bbuff = 0;
    uint32_t frame_offset = 0;
    uint16_t emm_logical_page_offset = 0;
    int16_t emm_logical_page_number = 0;
    EMM_HANDLE emm_handle = 0;
    int16_t width = 0;
    int16_t remap_flag = 0;

    uint8_t packet_op = 0;
    uint8_t packet_byte_count = 0;
    uint8_t sequence_byte_count = 0;
    uint8_t delta_byte_count = 0;
    uint8_t itr_op_repeat = 0;
    uint8_t data_byte = 0;

    width = FLIC_GET_WIDTH(file_animation);
    CS_file_animation_width = width;

    emm_handle = FLIC_GET_EMM_HANDLE_NUMBER(file_animation);
    emm_logical_page_number = (int16_t)FLIC_GET_EMM_LOGICAL_PAGE_NUMBER(file_animation);
    emm_logical_page_offset = FLIC_GET_EMM_LOGICAL_PAGE_OFFSET(file_animation);

    EMM_MapFourPages(emm_handle, emm_logical_page_number);

    frame_offset = GET_4B_OFS(EMS_PFBA, emm_logical_page_offset + 18 + (frame_num * 4));
    frame_offset++;

    CS_file_animation_emm_logical_page = emm_logical_page_number + (int16_t)(frame_offset / SZ_EMM_LOGICAL_PAGE);
    emm_logical_page_offset = (uint16_t)(frame_offset % SZ_EMM_LOGICAL_PAGE);

    emm_logical_page_offset = (uint16_t)(emm_logical_page_offset + FLIC_GET_EMM_LOGICAL_PAGE_OFFSET(file_animation));
    if (emm_logical_page_offset >= SZ_EMM_LOGICAL_PAGE)
    {
        emm_logical_page_offset &= 0x3FFF;
        CS_file_animation_emm_logical_page++;
    }

    CS_file_animation_emm_handle = emm_handle;

    remap_flag = GET_1B_OFS(file_animation, FLIC_HDR_POS_REMAP_FLAG);
    if (remap_flag != ST_FALSE)
    {
        Remap_Draw_File_Animation_Frame(x, y, file_animation);
        return;
    }

    EMM_MapFourPages(CS_file_animation_emm_handle, CS_file_animation_emm_logical_page);

    frame_data_base = EMS_PFBA;
    frame_data = frame_data_base + emm_logical_page_offset;

    bbuff_pos = current_video_page + ((y * SCREEN_WIDTH) + x);

    while (width--)
    {
        bbuff = bbuff_pos++;

        if ((size_t)(frame_data - frame_data_base) >= 31744U)  /* 32 KB - 1 KB = 31 KB*/
        {
            Map_File_Animation_Frame_ASM();  /* Restock the data store */
            frame_data -= SZ_EMM_LOGICAL_PAGE;
            frame_data_base = EMS_PFBA;
        }

        packet_op = *frame_data++;

        if (packet_op == 0xFF)
        {
            continue;
        }

        packet_byte_count = *frame_data++;

        if ((packet_op & 0x80) != 0)  /* Type: decode / run-count path */
        {
            do
            {
                sequence_byte_count = *frame_data++;
                delta_byte_count = *frame_data++;
                bbuff += (delta_byte_count * SCREEN_WIDTH);
                packet_byte_count -= sequence_byte_count + 2;

                while (sequence_byte_count--)
                {
                    data_byte = *frame_data++;

                    if (data_byte >= 224)
                    {
                        itr_op_repeat = (data_byte - 224) + 1;
                        sequence_byte_count--;
                        data_byte = *frame_data++;

                        while (itr_op_repeat--)
                        {
                            *bbuff = data_byte;
                            bbuff += SCREEN_WIDTH;
                        }
                    }
                    else
                    {
                        *bbuff = data_byte;
                        bbuff += SCREEN_WIDTH;
                    }
                }
            } while (packet_byte_count >= 1);
        }
        else  /* Type: copy */
        {
            do
            {
                sequence_byte_count = *frame_data++;
                delta_byte_count = *frame_data++;
                bbuff += (delta_byte_count * SCREEN_WIDTH);
                packet_byte_count -= sequence_byte_count + 2;

                while (sequence_byte_count--)
                {
                    *bbuff = *frame_data++;
                    bbuff += SCREEN_WIDTH;
                }
            } while (packet_byte_count >= 1);
        }
    }
}

// WZD s25p02
// AKA Map_File_Animation_Frame_ASM()
/*
"next", because it incrememnts the logical page
*/
// Map_File_Animation_Frame()
static void Map_File_Animation_Frame(void)
{
    CS_file_animation_emm_logical_page++;
    EMM_MapFourPages(CS_file_animation_emm_handle, CS_file_animation_emm_logical_page);
}

// WZD s25p03
// AKA Remap_Draw_File_Animation_Frame_ASM()
// Remap_Draw_File_Animation_Frame()
void Remap_Draw_File_Animation_Frame(int16_t x, int16_t y, SAMB_ptr file_animation, int16_t frame_num)
{
    byte_ptr frame_data = 0;
    byte_ptr frame_data_base = 0;
    byte_ptr bbuff_pos = 0;
    byte_ptr bbuff = 0;
    uint32_t frame_offset = 0;
    uint16_t emm_logical_page_offset = 0;
    int16_t emm_logical_page_number = 0;
    EMM_HANDLE emm_handle = 0;
    int16_t width = 0;

    uint8_t packet_op = 0;
    uint8_t packet_byte_count = 0;
    uint8_t sequence_byte_count = 0;
    uint8_t delta_byte_count = 0;
    uint8_t itr_op_repeat = 0;
    uint8_t data_byte = 0;

    uint8_t remap_block = 0;
    uint8_t remap_block_index = 0;
    uint8_t remap_color = 0;

    width = FLIC_GET_WIDTH(file_animation);
    CS_file_animation_width = width;

    emm_handle = FLIC_GET_EMM_HANDLE_NUMBER(file_animation);
    emm_logical_page_number = (int16_t)FLIC_GET_EMM_LOGICAL_PAGE_NUMBER(file_animation);
    emm_logical_page_offset = FLIC_GET_EMM_LOGICAL_PAGE_OFFSET(file_animation);

    EMM_MapFourPages(emm_handle, emm_logical_page_number);

    frame_offset = GET_4B_OFS(EMS_PFBA, emm_logical_page_offset + 18 + (frame_num * 4));
    frame_offset++;

    CS_file_animation_emm_logical_page = emm_logical_page_number + (int16_t)(frame_offset / SZ_EMM_LOGICAL_PAGE);
    emm_logical_page_offset = (uint16_t)(frame_offset % SZ_EMM_LOGICAL_PAGE);

    emm_logical_page_offset = (uint16_t)(emm_logical_page_offset + FLIC_GET_EMM_LOGICAL_PAGE_OFFSET(file_animation));
    if (emm_logical_page_offset >= SZ_EMM_LOGICAL_PAGE)
    {
        emm_logical_page_offset &= 0x3FFF;
        CS_file_animation_emm_logical_page++;
    }

    CS_file_animation_emm_handle = emm_handle;

    EMM_MapFourPages(CS_file_animation_emm_handle, CS_file_animation_emm_logical_page);

    frame_data_base = EMS_PFBA;
    frame_data = frame_data_base + emm_logical_page_offset;

    bbuff_pos = current_video_page + ((y * SCREEN_WIDTH) + x);

    while (width--)
    {
        bbuff = bbuff_pos++;

        if ((size_t)(frame_data - frame_data_base) >= 31744U)
        {
            Map_File_Animation_Frame();
            frame_data -= SZ_EMM_LOGICAL_PAGE;
            frame_data_base = EMS_PFBA;
        }

        packet_op = *frame_data++;

        if (packet_op == 0xFF)
        {
            continue;
        }

        packet_byte_count = *frame_data++;

        if (packet_op == 0x80)
        {
            do
            {
                sequence_byte_count = *frame_data++;
                delta_byte_count = *frame_data++;
                bbuff += (delta_byte_count * SCREEN_WIDTH);
                packet_byte_count -= sequence_byte_count + 2;

                while (sequence_byte_count--)
                {
                    data_byte = *frame_data++;

                    if (data_byte >= 224)
                    {
                        itr_op_repeat = (data_byte - 224) + 1;
                        sequence_byte_count--;
                        data_byte = *frame_data++;

                        if (data_byte >= 232)
                        {
                            remap_block = data_byte - 232;
                            while (itr_op_repeat--)
                            {
                                remap_block_index = *bbuff;
                                remap_color = *(remap_color_palettes + (remap_block * (16 * 16)) + remap_block_index);
                                *bbuff = remap_color;
                                bbuff += SCREEN_WIDTH;
                            }
                        }
                        else
                        {
                            while (itr_op_repeat--)
                            {
                                *bbuff = data_byte;
                                bbuff += SCREEN_WIDTH;
                            }
                        }
                    }
                    else
                    {
                        *bbuff = data_byte;
                        bbuff += SCREEN_WIDTH;
                    }
                }
            } while (packet_byte_count >= 1);
        }

        if (packet_op == 0x00)  /* Type: copy */
        {
            do {
                sequence_byte_count = *frame_data++;
                delta_byte_count = *frame_data++;
                bbuff += (delta_byte_count * SCREEN_WIDTH);
                packet_byte_count -= sequence_byte_count + 2;
                while (sequence_byte_count--)
                {
                    data_byte = *frame_data++;
                    if (data_byte >= 232)
                    {
                        remap_block = data_byte - 232;
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
            } while (packet_byte_count >= 1);
        }
    }
}
