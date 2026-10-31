/*
    WIZARDS.EXE
        seg025  ASM
        seg030

MoO2
    Module: file_ani

*/

#include "MOX_TYPE.h"

#include "EMS/EMS.h"
#include "file_ani.h"
#include "FLIC_Draw.h"
#include "Fonts.h"  /* remap_color_palettes */
#include "MOX_BASE.h"
#include "MOX_BITS.h"
#include "Video.h"  /* current_video_page */



// WZD dseg:E7DE                                                 BEGIN:  Fonts

// WZD dseg:E7DE 00 00                                           gsa_VGAFILEH_Header dw 0                ; DATA XREF: Load_Font_File+78w ...
// WZD dseg:E7DE
// struct s_animation_header file_animation_header = 0;
// struct s_FLIC_HDR * file_animation_header;
SAMB_ptr file_animation_header;



/*
Module: file_ani
    signed integer (4 bytes) file_animation_length
    Address: 02:001B42E8

    array (36 bytes) multi_file_animation_names
    Address: 02:001B42EC
    Num elements:    3, Type:        unsigned integer (4 bytes) 

    array (12 bytes) file_animation_fptr_multi
    Address: 02:001B4310
    Num elements:    3, Type:        unsigned integer (4 bytes) 

    pointer (4 bytes) file_lbx_header
    Address: 02:001B431C

    pointer (4 bytes) file_animation_header
    Address: 02:001B4320

    pointer (4 bytes) file_animation_buffer
    Address: 02:001B4324

    signed integer (4 bytes) file_animation_start
    Address: 02:001B4328

    array (12 bytes) file_animation_end_multi
    Address: 02:001B432C
    Num elements:    3, Type:        unsigned integer (4 bytes) 

    pointer (4 bytes) file_animation_fptr
    Address: 02:001B4338

    array (12 bytes) file_animation_length_multi
    Address: 02:001B433C
    Num elements:    3, Type:        unsigned integer (4 bytes) 

    array (12 bytes) file_animation_start_multi
    Address: 02:001B4348
    Num elements:    3, Type:        unsigned integer (4 bytes) 

    signed integer (4 bytes) file_animation_end
    Address: 02:001B4354

    array (6 bytes) file_animation_opened_flag_multi
    Address: 02:0017C5E2
    Num elements:    3, Type:        unsigned integer (4 bytes) 

    signed integer (2 bytes) file_animation_opened_flag
    Address: 02:0017C5E0

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

    function (0 bytes) Draw_File_Animation
    Address: 01:0012C7CC
        Num params: 0
        Return type: void (1 bytes) 
        Locals:
            signed integer (4 bytes) i
            signed integer (4 bytes) x
            signed integer (4 bytes) y
            signed integer (2 bytes) x2
            signed integer (2 bytes) y2
            signed integer (2 bytes) clip
            signed integer (4 bytes) frame_list
            signed integer (4 bytes) frame_length
            pointer (4 bytes) frame_offset
            pointer (4 bytes) frame_offset_table
            signed integer (4 bytes) file_offset

    function (0 bytes) Draw_Multi_File_Animation_Stencil
    Address: 01:0012CAD6
        Num params: 1
        Return type: void (1 bytes) 
        signed integer (2 bytes) 
        Locals:
            signed integer (2 bytes) file_num
            signed integer (4 bytes) i
            signed integer (4 bytes) x
            signed integer (4 bytes) y
            signed integer (2 bytes) x2
            signed integer (2 bytes) y2
            signed integer (2 bytes) clip
            signed integer (4 bytes) frame_list
            signed integer (4 bytes) frame_length
            pointer (4 bytes) frame_offset
            pointer (4 bytes) frame_offset_table
            signed integer (4 bytes) file_offset

    function (0 bytes) Draw_File_Animation_Stencil
    Address: 01:0012CD2D
        Num params: 0
        Return type: void (1 bytes) 
        Locals:
            signed integer (4 bytes) i
            signed integer (4 bytes) x
            signed integer (4 bytes) y
            signed integer (2 bytes) x2
            signed integer (2 bytes) y2
            signed integer (2 bytes) clip
            signed integer (4 bytes) frame_list
            signed integer (4 bytes) frame_length
            pointer (4 bytes) frame_offset
            pointer (4 bytes) frame_offset_table
            signed integer (4 bytes) file_offset
            signed integer (4 bytes) saved_refresh

    function (0 bytes) Draw_File_Animation_On
    Address: 01:0012CF6D
        Num params: 0
        Return type: void (1 bytes) 
        Locals:
            signed integer (4 bytes) i
            signed integer (4 bytes) x
            signed integer (4 bytes) y
            signed integer (2 bytes) x2
            signed integer (2 bytes) y2
            signed integer (2 bytes) clip
            signed integer (4 bytes) frame_list
            signed integer (4 bytes) frame_length
            pointer (4 bytes) frame_offset
            pointer (4 bytes) frame_offset_table
            signed integer (4 bytes) file_offset
            signed integer (4 bytes) saved_refresh

    function (0 bytes) Open_Multi_File_Animation
    Address: 01:0012D1C5
        Num params: 3
        Return type: void (1 bytes) 
        pointer (4 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        Locals:
            pointer (4 bytes) file_name
            signed integer (2 bytes) entry_num
            signed integer (2 bytes) file_num
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

    function (0 bytes) Draw_Multi_File_Animation
    Address: 01:0012D408
        Num params: 1
        Return type: void (1 bytes) 
        signed integer (2 bytes) 
        Locals:
            signed integer (2 bytes) file_num
            signed integer (4 bytes) i
            signed integer (4 bytes) x
            signed integer (4 bytes) y
            signed integer (2 bytes) x2
            signed integer (2 bytes) y2
            signed integer (2 bytes) clip
            signed integer (4 bytes) frame_list
            signed integer (4 bytes) frame_length
            pointer (4 bytes) frame_offset
            pointer (4 bytes) frame_offset_table
            signed integer (4 bytes) file_offset

    function (0 bytes) Set_File_Animation_Frame
    Address: 01:0012D6B2
        Num params: 1
        Return type: void (1 bytes) 
        signed integer (2 bytes) 
        Locals:
            signed integer (2 bytes) frame_num

    function (0 bytes) Get_File_Animation_Frame
    Address: 01:0012D70B
        Num params: 0
        Return type: signed integer (2 bytes) 
        void (1 bytes) 
        Locals:

    function (0 bytes) Get_Multi_File_Animation_Frame
    Address: 01:0012D745
        Num params: 1
        Return type: signed integer (2 bytes) 
        signed integer (2 bytes) 
        Locals:
            signed integer (2 bytes) file_num

    function (0 bytes) Reset_File_Animation_Buffer
    Address: 01:0012D78E
        Num params: 0
        Return type: void (1 bytes) 
        Locals:

    function (0 bytes) Set_File_Animation_Buffer
    Address: 01:0012D7B7
        Num params: 1
        Return type: void (1 bytes) 
        pointer (4 bytes) 
        Locals:
            pointer (4 bytes) buffer

    function (0 bytes) Draw_Multi_File_Animation_Palette
    Address: 01:0012D7E3
        Num params: 1
        Return type: void (1 bytes) 
        signed integer (4 bytes) 
        Locals:
            signed integer (4 bytes) file_num

    function (0 bytes) Draw_File_Animation_Palette
    Address: 01:0012D84E
        Num params: 0
        Return type: void (1 bytes) 
        Locals:

    function (0 bytes) Set_Multi_File_Animation_Frame
    Address: 01:0012D88F
        Num params: 2
        Return type: void (1 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        Locals:
            signed integer (2 bytes) file_num
            signed integer (2 bytes) frame_num
*/



/*
    WIZARDS.EXE  seg025
*/

// WZD seg025  CS_file_animation_emm_logical_page dw 0
static uint16_t file_animation_emm_logical_page = 0;
// WZD seg025  CS_file_animation_emm_handle dw 0
static uint16_t file_animation_emm_handle = 0;
// WZD seg025  CS_file_animation_width dw 0
static uint16_t file_animation_width = 0;


// WZD seg025
// Map_File_Animation_Frame_ASM()
/*
    maps the next logical page of the file animation into physical pages 0 and 1, and moves the frame data offset back by one page
    the asm takes and returns the frame data offset in SI
*/
static void Map_File_Animation_Frame(uint16_t * frame_data_offset)
{
    file_animation_emm_logical_page++;
    // INT 67h  AX = 4400h  BX = file_animation_emm_logical_page        DX = file_animation_emm_handle
    // INT 67h  AX = 4401h  BX = (file_animation_emm_logical_page + 1)  DX = file_animation_emm_handle
    EMM_MapFourPages(file_animation_emm_logical_page, _VGAFILEH_seg);  /* CLAUDE  the header's 1-byte EMM handle can't hold a ReMoM handle; VGAFILEH is the file animation handle */
    *frame_data_offset -= SZ_EMM_LOGICAL_PAGE;
}


// WZD seg025
/*
    draws a frame of the file animation from its EMM handle
    finds the frame through the frame offset table in the EMM copy of the animation header
    hands off to Remap_Draw_File_Animation_Frame_ASM() if the remap flag in the EMM copy of the animation header is set
*/
void Draw_File_Animation_Frame_ASM(int16_t x, int16_t y, SAMB_ptr file_animation, int16_t frame_num)
{
    byte_ptr page_frame = 0;
    byte_ptr screen_start = 0;
    byte_ptr screen_pos = 0;
    uint32_t frame_offset = 0;
    uint16_t header_offset = 0;
    uint16_t frame_data_offset = 0;
    uint16_t logical_page = 0;
    uint16_t packet_byte_count = 0;
    uint16_t sequence_byte_count = 0;
    uint16_t itr_op_repeat = 0;
    uint16_t itr_width = 0;
    uint8_t packet_op = 0;
    uint8_t data_byte = 0;
    uint8_t remap_flag = 0;

    file_animation_width = FLIC_GET_WIDTH(file_animation);

    file_animation_emm_handle = GET_1B_OFS(file_animation, FLIC_HDR_POS_EMM_HANDLE_NUMBER);
    logical_page = GET_1B_OFS(file_animation, FLIC_HDR_POS_EMM_LOGICAL_PAGE_NUMBER);
    header_offset = GET_2B_OFS(file_animation, FLIC_HDR_POS_EMM_LOGICAL_PAGE_OFFSET);

    // INT 67h  AX = 4400h / 4401h  BX = logical_page / (logical_page + 1)  DX = file_animation_emm_handle
    EMM_MapFourPages(logical_page, _VGAFILEH_seg);  /* CLAUDE */
    page_frame = EMM_GetPageFrame();

    frame_offset = GET_4B_OFS(page_frame, (header_offset + (frame_num * 4) + 18));
    frame_offset++;
    logical_page = (uint16_t)(frame_offset >> 14);
    frame_data_offset = (uint16_t)(frame_offset & 0x3FFF);
    logical_page += GET_1B_OFS(file_animation, FLIC_HDR_POS_EMM_LOGICAL_PAGE_NUMBER);
    frame_data_offset += GET_2B_OFS(file_animation, FLIC_HDR_POS_EMM_LOGICAL_PAGE_OFFSET);
    if(frame_data_offset >= SZ_EMM_LOGICAL_PAGE)
    {
        frame_data_offset &= 0x3FFF;
        logical_page++;
    }
    file_animation_emm_logical_page = logical_page;

    remap_flag = GET_1B_OFS(page_frame, (header_offset + FLIC_HDR_POS_REMAP_FLAG));

    // INT 67h  AX = 4400h / 4401h  BX = file_animation_emm_logical_page / (file_animation_emm_logical_page + 1)  DX = file_animation_emm_handle
    EMM_MapFourPages(file_animation_emm_logical_page, _VGAFILEH_seg);  /* CLAUDE */
    page_frame = EMM_GetPageFrame();

    if(remap_flag != 0)
    {
        /* CLAUDE  the asm passes the frame data offset in SI */
        Remap_Draw_File_Animation_Frame_ASM(x, y, file_animation, frame_data_offset);
        return;
    }

    screen_start = current_video_page + ((y * SCREEN_WIDTH) + x);

    itr_width = file_animation_width;

    do
    {
        screen_pos = screen_start;

        if(frame_data_offset >= 31744)
        {
            Map_File_Animation_Frame(&frame_data_offset);
            page_frame = EMM_GetPageFrame();
        }

        packet_op = page_frame[frame_data_offset++];

        if(packet_op != 0xFF)
        {
            packet_byte_count = ((packet_op << 8) | page_frame[frame_data_offset++]);

            if((int8_t)packet_op >= 0)  /* Type: copy */
            {
                do
                {
                    screen_pos += (page_frame[(frame_data_offset + 1)] * SCREEN_WIDTH);
                    sequence_byte_count = page_frame[frame_data_offset++];
                    frame_data_offset++;
                    packet_byte_count -= 2;
                    packet_byte_count -= sequence_byte_count;
                    do
                    {
                        *screen_pos = page_frame[frame_data_offset++];
                        screen_pos += SCREEN_WIDTH;
                    } while(--sequence_byte_count != 0);
                } while((int16_t)(packet_byte_count - 1) >= 0);  /* cmp dx, 1 ; jns */
            }
            else  /* Type: decode */
            {
                packet_byte_count &= 0x7FFF;
                do
                {
                    screen_pos += (page_frame[(frame_data_offset + 1)] * SCREEN_WIDTH);
                    sequence_byte_count = page_frame[frame_data_offset++];
                    frame_data_offset++;
                    packet_byte_count -= 2;
                    packet_byte_count -= sequence_byte_count;
                    do
                    {
                        data_byte = page_frame[frame_data_offset++];
                        if(data_byte >= 224)  /* op: repeat */
                        {
                            itr_op_repeat = ((data_byte & 0x1F) + 1);
                            data_byte = page_frame[frame_data_offset++];
                            do
                            {
                                *screen_pos = data_byte;
                                screen_pos += SCREEN_WIDTH;
                            } while(--itr_op_repeat != 0);
                            sequence_byte_count--;
                        }
                        else  /* op: copy */
                        {
                            *screen_pos = data_byte;
                            screen_pos += SCREEN_WIDTH;
                        }
                    } while(--sequence_byte_count != 0);
                } while((int16_t)(packet_byte_count - 1) >= 0);  /* cmp dx, 1 ; jns */
            }
        }

        screen_start++;

    } while(--itr_width != 0);

}


// WZD s25p02
// Map_File_Animation_Frame_ASM()
/*
proc Map_File_Animation_Frame_ASM near
push bx
mov cx, ax
inc [cs:CS_file_animation_emm_logical_page]
mov ax, 4400h
mov bx, [cs:CS_file_animation_emm_logical_page]
mov dx, [cs:CS_file_animation_emm_handle]
int 67h                             ;  - LIM EMS - MAP MEMORY
                                    ; AL = physical page number (0-3)
                                    ; BX = logical page number, DX = handle
                                    ; Return: AH = status
mov ax, 4401h
mov bx, [cs:CS_file_animation_emm_logical_page]
inc bx
mov dx, [cs:CS_file_animation_emm_handle]
int 67h                             ;  - LIM EMS - MAP MEMORY
                                    ; AL = physical page number (0-3)
                                    ; BX = logical page number, DX = handle
                                    ; Return: AH = status
sub si, 16384
mov ax, cx
pop bx
retn
endp Map_File_Animation_Frame_ASM
*/

// WZD s25p03
/*
    draws a frame of the file animation from its EMM handle, using the remap colors
    called from Draw_File_Animation_Frame() with the frame's pages already mapped
    the asm takes the frame data offset in SI
    file_animation is passed, but not used
*/
void Remap_Draw_File_Animation_Frame_ASM(int16_t x, int16_t y, SAMB_ptr file_animation, uint16_t frame_data_offset)
{
    byte_ptr page_frame = 0;
    byte_ptr screen_start = 0;
    byte_ptr screen_pos = 0;
    uint16_t packet_byte_count = 0;
    uint16_t sequence_byte_count = 0;
    uint16_t itr_op_repeat = 0;
    uint16_t itr_width = 0;
    uint8_t packet_op = 0;
    uint8_t data_byte = 0;

    screen_start = current_video_page + ((y * SCREEN_WIDTH) + x);

    page_frame = EMM_GetPageFrame();

    itr_width = file_animation_width;

    do
    {
        screen_pos = screen_start;

        if(frame_data_offset >= 31744)
        {
            Map_File_Animation_Frame(&frame_data_offset);
            page_frame = EMM_GetPageFrame();
        }

        packet_op = page_frame[frame_data_offset++];

        if(packet_op != 0xFF)
        {
            packet_byte_count = ((packet_op << 8) | page_frame[frame_data_offset++]);

            if((int8_t)packet_op >= 0)  /* Type: copy */
            {
                do
                {
                    screen_pos += (page_frame[(frame_data_offset + 1)] * SCREEN_WIDTH);
                    sequence_byte_count = page_frame[frame_data_offset++];
                    frame_data_offset++;
                    packet_byte_count -= 2;
                    packet_byte_count -= sequence_byte_count;
                    do
                    {
                        data_byte = page_frame[frame_data_offset++];
                        if(data_byte >= 232)  /* op: remap */
                        {
                            *screen_pos = *(remap_color_palettes + ((data_byte - 232) * (16 * 16)) + *screen_pos);
                        }
                        else
                        {
                            *screen_pos = data_byte;
                        }
                        screen_pos += SCREEN_WIDTH;
                    } while(--sequence_byte_count != 0);
                } while((int16_t)(packet_byte_count - 1) >= 0);  /* cmp dx, 1 ; jns */
            }
            else  /* Type: decode */
            {
                packet_byte_count &= 0x7FFF;
                do
                {
                    screen_pos += (page_frame[(frame_data_offset + 1)] * SCREEN_WIDTH);
                    sequence_byte_count = page_frame[frame_data_offset++];
                    frame_data_offset++;
                    packet_byte_count -= 2;
                    packet_byte_count -= sequence_byte_count;
                    do
                    {
                        data_byte = page_frame[frame_data_offset++];
                        if(data_byte >= 224)  /* op: repeat */
                        {
                            itr_op_repeat = ((data_byte & 0x1F) + 1);
                            data_byte = page_frame[frame_data_offset++];
                            if(data_byte >= 232)  /* op: remap */
                            {
                                do
                                {
                                    *screen_pos = *(remap_color_palettes + ((data_byte - 232) * (16 * 16)) + *screen_pos);
                                    screen_pos += SCREEN_WIDTH;
                                } while(--itr_op_repeat != 0);
                            }
                            else
                            {
                                do
                                {
                                    *screen_pos = data_byte;
                                    screen_pos += SCREEN_WIDTH;
                                } while(--itr_op_repeat != 0);
                            }
                            sequence_byte_count--;
                        }
                        else if(data_byte >= 232)  /* op: remap  ; unreachable, 232 and up already took the repeat branch */
                        {
                            *screen_pos = *(remap_color_palettes + ((data_byte - 232) * (16 * 16)) + *screen_pos);
                            screen_pos += SCREEN_WIDTH;
                        }
                        else  /* op: copy */
                        {
                            *screen_pos = data_byte;
                            screen_pos += SCREEN_WIDTH;
                        }
                    } while(--sequence_byte_count != 0);
                } while((int16_t)(packet_byte_count - 1) >= 0);  /* cmp dx, 1 ; jns */
            }
        }

        screen_start++;

    } while(--itr_width != 0);

}