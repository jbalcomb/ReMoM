/*
    WIZARDS.EXE
        seg030

MoO2
    Module: file_ani

*/

#include "MOX_TYPE.h"



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