
MoM
seg030
seg025

MoO2  Module: file_ani
MoO2  Module: animate

¿ 1oom ?

SEEALSO: EMM ... IO Buffer ...


1oom just does a LBX_Load() and FLIC_Draw()
MoO2 uses io_buffer, instead of _VGAFILEH_seg and file_animation_header


The business with file_animation_header and _VGAFILEH_seg *feels* like a special use-case of the EMM Header Only *caching*.
The approach of just doing a LBX_Load() and FLIC_Draw() seems to work just fine.
But, *need* something more consistent with the existing code-base and Dasm.
Can we just allocated a large enough buffer for _VGAFILEH_seg and then point file_animation_header at the SAMB_data?
Or, just allocated the buffer for file_animation_header directly and just use LBX_Reload()?

Get_File_Animation_Frame() uses file_animation_header
Set_File_Animation_Frame() uses file_animation_header
Load_File_Animation_Frame() reads the data into _VGAFILEH_seg










Module: init
    unsigned integer (4 bytes) io_buffer_seg
    Address: 02:001A86F8

    pointer (4 bytes) io_buffer
    Address: 02:001A86FC

    signed integer (2 bytes) io_buffer_selector
    Address: 02:001A8700

    function (0 bytes) Init_Drivers
    Address: 01:00113D47
        Num params: 2
        Return type: void (1 bytes) 
        signed integer (4 bytes) 
        pointer (4 bytes) 
        Locals:
            signed integer (4 bytes) video_mode
            pointer (4 bytes) font_file

    function (0 bytes) Reset_System
    Address: 01:00113DBD
        Num params: 0
        Return type: void (1 bytes) 
        Locals:
            signed integer (4 bytes) re_entrant_reset_system_flag

    function (0 bytes) Create_IO_Buffer
    Address: 01:00113E08
        Num params: 0
        Return type: void (1 bytes) 
        Locals:
            signed integer (4 bytes) seg_start


Module: allocate

    function (0 bytes) Get_Max_Block_Space
    Address: 01:00110C29
        Num params: 1
        Return type: signed integer (4 bytes) 
        pointer (4 bytes) 
        Locals:
            pointer (4 bytes) block
            pointer (4 bytes) header

    function (0 bytes) Allocate_Dos_Space
    Address: 01:00110C62
        Num params: 1
        Return type: pointer (4 bytes) 
        signed integer (4 bytes) 
        Locals:
            signed integer (4 bytes) size
            pointer (4 bytes) name
            pointer (4 bytes) header
            signed integer (4 bytes) lsize






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





Module: animate
    pointer (4 bytes) palette_header
    Address: 02:001B42A4

    pointer (4 bytes) animation_header
    Address: 02:001B42A8

    signed integer (2 bytes) dot_stencil_flag
    Address: 02:0017C5DA

    signed integer (2 bytes) clipping_flag
    Address: 02:0017C5D8

    function (0 bytes) Draw_No_Glass
    Address: 01:00129FF9
        Num params: 3
        Return type: void (1 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        pointer (4 bytes) 
        Locals:
            signed integer (2 bytes) x
            signed integer (2 bytes) y
            pointer (4 bytes) picture
            signed integer (4 bytes) i
            signed integer (2 bytes) x2
            signed integer (2 bytes) y2
            signed integer (4 bytes) clip
            signed integer (4 bytes) frame_list
            pointer (4 bytes) frame_offset
            pointer (4 bytes) frame_offset_table

    function (0 bytes) Draw
    Address: 01:0012A478
        Num params: 3
        Return type: void (1 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        pointer (4 bytes) 
        Locals:
            signed integer (2 bytes) x
            signed integer (2 bytes) y
            pointer (4 bytes) picture
            signed integer (4 bytes) i
            signed integer (2 bytes) x2
            signed integer (2 bytes) y2
            signed integer (4 bytes) clip
            signed integer (4 bytes) frame_list
            pointer (4 bytes) frame_offset
            pointer (4 bytes) frame_offset_table

    function (0 bytes) Draw_Animated_Sprite_Back_Diff
    Address: 01:0012A914
        Num params: 3
        Return type: void (1 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        pointer (4 bytes) 
        Locals:
            signed integer (2 bytes) x_start
            signed integer (2 bytes) y_start
            pointer (4 bytes) frame_data
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
            unsigned integer (4 bytes) copy_size
            pointer (4 bytes) frame_data_word
            signed integer (4 bytes) square_x
            signed integer (4 bytes) square_y
            signed integer (4 bytes) square_x2

    function (0 bytes) Draw_No_Glassed_Animated_Sprite
    Address: 01:0012AAA1
        Num params: 3
        Return type: void (1 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        pointer (4 bytes) 
        Locals:
            signed integer (2 bytes) x_start
            signed integer (2 bytes) y_start
            pointer (4 bytes) frame_data
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
            unsigned integer (1 bytes) data_byte
            unsigned integer (1 bytes) screen_byte

    function (0 bytes) Draw_No_Glassed_Bitmap_Sprite
    Address: 01:0012ABCC
        Num params: 3
        Return type: void (1 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        pointer (4 bytes) 
        Locals:
            signed integer (2 bytes) x_start
            signed integer (2 bytes) y_start
            pointer (4 bytes) frame_data
            signed integer (4 bytes) bitmap_size
            signed integer (4 bytes) pos
            signed integer (4 bytes) screen_pos
            signed integer (4 bytes) line_add
            signed integer (4 bytes) x
            signed integer (4 bytes) y
            signed integer (4 bytes) i
            signed integer (2 bytes) height
            signed integer (2 bytes) width
            unsigned integer (1 bytes) data
            unsigned integer (1 bytes) screen_data

    function (0 bytes) Draw_Glassed_Animated_Sprite
    Address: 01:0012ACA4
        Num params: 3
        Return type: void (1 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        pointer (4 bytes) 
        Locals:
            signed integer (2 bytes) x_start
            signed integer (2 bytes) y_start
            pointer (4 bytes) frame_data
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
            unsigned integer (1 bytes) data_byte
            unsigned integer (1 bytes) screen_byte

    function (0 bytes) Set_Animation_Palette
    Address: 01:0012AE00
        Num params: 0
        Return type: void (1 bytes) 
        Locals:
            signed integer (4 bytes) palette_num
            signed integer (4 bytes) start
            signed integer (4 bytes) count
            signed integer (4 bytes) palette_pos
            pointer (4 bytes) current_palette_SDWORD
            signed integer (4 bytes) palette_data

    function (0 bytes) Draw_Glassed_Bitmap_Sprite
    Address: 01:0012AEC0
        Num params: 3
        Return type: void (1 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        pointer (4 bytes) 
        Locals:
            signed integer (2 bytes) x_start
            signed integer (2 bytes) y_start
            pointer (4 bytes) frame_data
            signed integer (4 bytes) bitmap_size
            signed integer (4 bytes) pos
            signed integer (4 bytes) screen_pos
            signed integer (4 bytes) line_add
            signed integer (4 bytes) x
            signed integer (4 bytes) y
            signed integer (4 bytes) i
            signed integer (2 bytes) height
            signed integer (2 bytes) width
            unsigned integer (1 bytes) data
            unsigned integer (1 bytes) screen_data

    function (0 bytes) Draw_Screen_Background_Off
    Address: 01:0012AFCC
        Num params: 1
        Return type: void (1 bytes) 
        pointer (4 bytes) 
        Locals:
            pointer (4 bytes) frame_offset
            signed integer (4 bytes) bank_num
            signed integer (4 bytes) buffer_pos
            signed integer (4 bytes) screen_pos
            signed integer (4 bytes) i
            signed integer (4 bytes) buffer_end
            signed integer (4 bytes) data
            signed integer (4 bytes) skip_count
            signed integer (4 bytes) data_count
            pointer (4 bytes) video_memory_SDWORD

    function (0 bytes) Draw_Animated_Sprite_On_Page
    Address: 01:0012B0B7
        Num params: 3
        Return type: void (1 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        pointer (4 bytes) 
        Locals:
            signed integer (2 bytes) x_start
            signed integer (2 bytes) y_start
            pointer (4 bytes) frame_data
            signed integer (4 bytes) bank_pos
            signed integer (4 bytes) bank_num
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
            pointer (4 bytes) video_memory_SDWORD
            signed integer (4 bytes) square_x
            signed integer (4 bytes) square_y
            signed integer (4 bytes) square_x2
            signed integer (4 bytes) copy_size

    function (0 bytes) Draw_Screen_Background_On
    Address: 01:0012B2D9
        Num params: 1
        Return type: void (1 bytes) 
        pointer (4 bytes) 
        Locals:
            pointer (4 bytes) frame_offset
            signed integer (4 bytes) bank_num
            signed integer (4 bytes) buffer_pos
            signed integer (4 bytes) screen_pos
            signed integer (4 bytes) i
            signed integer (4 bytes) buffer_end
            signed integer (4 bytes) data
            signed integer (4 bytes) skip_count
            signed integer (4 bytes) data_count
            pointer (4 bytes) video_memory_SDWORD

    function (0 bytes) Draw_Glassed_Screen_Background_Off
    Address: 01:0012B3E0
        Num params: 1
        Return type: void (1 bytes) 
        pointer (4 bytes) 
        Locals:
            pointer (4 bytes) frame_offset
            signed integer (4 bytes) bank_num
            signed integer (4 bytes) buffer_pos
            signed integer (4 bytes) screen_pos
            signed integer (4 bytes) i
            signed integer (4 bytes) buffer_end
            signed integer (4 bytes) data
            signed integer (4 bytes) skip_count
            signed integer (4 bytes) data_count
            unsigned integer (1 bytes) screen_byte
            unsigned integer (1 bytes) data_byte

    function (0 bytes) Draw_Glassed_Screen_Background_On
    Address: 01:0012B501
        Num params: 1
        Return type: void (1 bytes) 
        pointer (4 bytes) 
        Locals:
            pointer (4 bytes) frame_offset
            signed integer (4 bytes) bank_num
            signed integer (4 bytes) buffer_pos
            signed integer (4 bytes) screen_pos
            signed integer (4 bytes) i
            signed integer (4 bytes) buffer_end
            signed integer (4 bytes) data
            signed integer (4 bytes) skip_count
            signed integer (4 bytes) data_count
            signed integer (4 bytes) data_byte_SDWORD
            signed integer (4 bytes) screen_data_SDWORD
            unsigned integer (1 bytes) screen_byte
            unsigned integer (1 bytes) data_byte

    function (0 bytes) Clip_On
    Address: 01:0012B634
        Num params: 0
        Return type: void (1 bytes) 
        Locals:

    function (0 bytes) Clip_Off
    Address: 01:0012B65C
        Num params: 0
        Return type: void (1 bytes) 
        Locals:

    function (0 bytes) Dot_Stencil_On
    Address: 01:0012B684
        Num params: 0
        Return type: void (1 bytes) 
        Locals:

    function (0 bytes) Dot_Stencil_Off
    Address: 01:0012B6AC
        Num params: 0
        Return type: void (1 bytes) 
        Locals:

    function (0 bytes) Get_Full_Store_Flag
    Address: 01:0012B6D4
        Num params: 1
        Return type: signed integer (2 bytes) 
        pointer (4 bytes) 
        Locals:
            pointer (4 bytes) picture

    function (0 bytes) Reset_Animation_Frame
    Address: 01:0012B726
        Num params: 1
        Return type: void (1 bytes) 
        pointer (4 bytes) 
        Locals:
            pointer (4 bytes) picture

    function (0 bytes) Set_Animation_Frame
    Address: 01:0012B753
        Num params: 2
        Return type: void (1 bytes) 
        pointer (4 bytes) 
        signed integer (4 bytes) 
        Locals:
            pointer (4 bytes) picture
            signed integer (4 bytes) frame_num

    function (0 bytes) Draw_Palette
    Address: 01:0012B79D
        Num params: 1
        Return type: void (1 bytes) 
        pointer (4 bytes) 
        Locals:
            pointer (4 bytes) picture

    function (0 bytes) Remap_Draw
    Address: 01:0012B7E1
        Num params: 3
        Return type: void (1 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        pointer (4 bytes) 
        Locals:
            signed integer (2 bytes) x
            signed integer (2 bytes) y
            pointer (4 bytes) picture
            signed integer (4 bytes) i
            signed integer (2 bytes) x2
            signed integer (2 bytes) y2
            signed integer (2 bytes) clip
            signed integer (4 bytes) frame_list
            pointer (4 bytes) frame_offset

    function (0 bytes) Remap_Draw_Animated_Sprite
    Address: 01:0012BAEB
        Num params: 3
        Return type: void (1 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        pointer (4 bytes) 
        Locals:
            signed integer (2 bytes) x_start
            signed integer (2 bytes) y_start
            pointer (4 bytes) frame_data
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

    function (0 bytes) Remap_Clipped_Draw_Animated_Sprite
    Address: 01:0012BC0B
        Num params: 3
        Return type: void (1 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        pointer (4 bytes) 
        Locals:
            signed integer (2 bytes) x1
            signed integer (2 bytes) y1
            pointer (4 bytes) frame_data
            signed integer (4 bytes) screen_pos
            signed integer (4 bytes) y
            signed integer (4 bytes) screen_start
            signed integer (4 bytes) buffer_pos
            signed integer (4 bytes) buffer_pos_word
            signed integer (4 bytes) packet_end
            signed integer (2 bytes) height
            signed integer (2 bytes) data_count
            signed integer (2 bytes) skip_count
            signed integer (2 bytes) store_type
            signed integer (2 bytes) width
            signed integer (2 bytes) x2
            signed integer (2 bytes) y2
            signed integer (2 bytes) skip_x
            signed integer (2 bytes) start_x
            signed integer (2 bytes) skip_y
            signed integer (2 bytes) start_y
            signed integer (2 bytes) actual_width
            signed integer (2 bytes) actual_height
            signed integer (2 bytes) skip_add
            signed integer (2 bytes) clipped_count
            pointer (4 bytes) frame_data_word
            signed integer (4 bytes) buffer_add
            signed integer (4 bytes) screen_line_add
            signed integer (4 bytes) screen_line_start
            signed integer (4 bytes) screen_line_end

    function (0 bytes) Draw_Gray_Scaled
    Address: 01:0012BF3A
        Num params: 4
        Return type: void (1 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        pointer (4 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        Locals:
            signed integer (2 bytes) x
            signed integer (2 bytes) y
            pointer (4 bytes) picture
            signed integer (2 bytes) start_color
            signed integer (2 bytes) color_count
            signed integer (2 bytes) inverse_flag
            signed integer (4 bytes) i
            signed integer (2 bytes) x2
            signed integer (2 bytes) y2
            signed integer (2 bytes) clip
            signed integer (4 bytes) frame_list
            pointer (4 bytes) frame_offset
            pointer (4 bytes) frame_offset_table

    function (0 bytes) Draw_Screen_Background_Off_Gray_Scale
    Address: 01:0012C117
        Num params: 4
        Return type: void (1 bytes) 
        pointer (4 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        Locals:
            pointer (4 bytes) frame_offset
            signed integer (2 bytes) start_color
            signed integer (2 bytes) color_count
            signed integer (2 bytes) inverse_flag
            signed integer (4 bytes) bank_num
            signed integer (4 bytes) buffer_pos
            signed integer (4 bytes) screen_pos
            signed integer (4 bytes) i
            signed integer (4 bytes) buffer_end
            signed integer (4 bytes) buffer_pos_UBYTE
            signed integer (4 bytes) data
            signed integer (4 bytes) skip_count
            signed integer (4 bytes) data_count
            pointer (4 bytes) video_memory_SDWORD
            pointer (4 bytes) video_memory_UBYTE
            pointer (4 bytes) frame_offset_UBYTE
