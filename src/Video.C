
#include "MoX_TYPE.H"


// pointer (4 bytes) Copy_Off_To_On_Page
// Address: 02:001B3888
// pointer (4 bytes) Switch_Video_Pages
// Address: 02:001B3894



// int32_t screen_pixel_width = 640;
// int32_t screen_pixel_height = 480;
// int32_t screen_pixel_size = 307200;  // (640 * 480)
/*
    Init_Video_Drivers()
        screen_pixel_width = video mode width;
        screen_pixel_height = video mode height;
        screen_pixel_size = screen_pixel_width * screen_pixel_height;
*/


// int16_t screen_window_x1
// int16_t screen_window_x2
// int16_t screen_window_y1
// int16_t screen_window_y2


int16_t draw_page_num;
// // char current_page_flag
// // int16_t copy_to_on_page_flag
uint8_t * current_video_page;
// uint8_t off_page_buffer[64000];
// uint8_t back_page_buffer[64000];
uint8_t * video_page_buffer[2];
uint8_t * draw_page;
/*
    Create_Screen_Buffers(screen_pixel_size)
        __off_page_buffer = Allocate_Space(size + 46)
        __back_page_buffer = Allocate_Space(size + 46)
        Set_Memory_(__off_page_buffer, screen_pixel_size, 0);
        Set_Memory_(__back_page_buffer, screen_pixel_size, 0);
*/


// array (624 bytes) mouse_background_buffer
// Num elements:  624, Type:		unsigned integer (4 bytes) 
// array (624 bytes) mouse_off_page_buffer
// Num elements:  624, Type:		unsigned integer (4 bytes) 
// pointer (4 bytes) mouse_background_buffer_pointer
// pointer (4 bytes) mouse_off_page_buffer_pointer



// array (1024 bytes) current_palette
// Num elements:  256, Type:		unsigned integer (4 bytes) 
// signed integer (4 bytes) current_palette_address
// array (260 bytes) palette_list
// Num elements:   65, Type:		unsigned integer (4 bytes) 

// pointer (4 bytes) mouse_palette
// pointer (4 bytes) mouse_image_asm

// int16_t mouse_y;
// int16_t mouse_x;
// int16_t old_mouse_x;
// int16_t old_mouse_y;
// int16_t last_on_page_mouse_y;
// int16_t last_on_page_mouse_x;
// int16_t last_off_page_mouse_x;
// int16_t last_off_page_mouse_y;
// char last_on_page_saved_flag;

// signed integer (2 bytes) current_mouse_list_ptr
// signed integer (2 bytes) current_mouse_shape


// MGC s26p01
void Set_Page_On(void)
{
    /*
        ~== VRAM + ((draw_page_num) * 4)  {0xA000, 0xA400}
    */

    current_video_page = video_page_buffer[(draw_page_num)];

    // // current_page_flag = 0;
    // current_video_page = off_page_buffer;
    // // copy_to_on_page_flag = 1;

}


// MGC s26p02
void Set_Page_Off(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Set_Page_Off()\n", __FILE__, __LINE__);
#endif

    /*
        ~== VRAM + ((1 - draw_page_num) * 4)  {0xA000, 0xA400}
    */

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: current_video_page: %p\n", __FILE__, __LINE__, current_video_page);
#endif
    current_video_page = video_page_buffer[(1 - draw_page_num)];
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: current_video_page: %p\n", __FILE__, __LINE__, current_video_page);
#endif

    // // current_page_flag = 1;
    // current_video_page = off_page_buffer;
    // // copy_to_on_page_flag = 0;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Set_Page_Off()\n", __FILE__, __LINE__);
#endif
}


// MGC s26p04
void Page_Flip(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Page_Flip()\n", __FILE__, __LINE__);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: draw_page_num: %d\n", __FILE__, __LINE__, draw_page_num);
#endif
    draw_page_num = (1 - draw_page_num);  // NOTE: this is the only code that changes 'draw_page_num'
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: draw_page_num: %d\n", __FILE__, __LINE__, draw_page_num);
#endif

    // ? Meh ? current_video_page = off_page_buffer;

    // MS-DOS & IBM-PC VGA
        // CRT Controller - Address
        // VGA_WaitSync();
    // MS Windows
        // pointer to buffer to render/present
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: draw_page: %p\n", __FILE__, __LINE__, draw_page);
#endif
    draw_page = video_page_buffer[draw_page_num];
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: draw_page: %p\n", __FILE__, __LINE__, draw_page);
#endif

    // VGA_Set_DSP_Addr();  // MGC s26p02
    Set_Page_Off();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Page_Flip()\n", __FILE__, __LINE__);
#endif

}
