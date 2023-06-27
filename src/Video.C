
#include "MoX_TYPE.H"

#include "Video.H"

#include "Mouse.H"  /* e_Cursor_Image; Get_Pointer_Image_Numeber(), Restore_Mouse_State(), Save_Mouse_State(); */

#include "MoM.H"  /* Buffer; Render_VBB(); */


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




// dseg:76E2 46 4F 4E 54 53 2E 4C 42+cnst_FONT_File2 db 'FONTS.LBX',0        ; should use dseg:28f2
// dseg:76EC 00 A0                   g_VGA_DisplayBuffer_Page_SgmtAddr dw 0A000h

// WZD dseg:76EE
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

    /*
        ~== VRAM + ((1 - draw_page_num) * 4)  {0xA000, 0xA400}
    */

    current_video_page = video_page_buffer[(1 - draw_page_num)];

    // // current_page_flag = 1;
    // current_video_page = off_page_buffer;
    // // copy_to_on_page_flag = 0;

}


// WZD s28p03
void Check_Default_Video_Page(void)
{

}

// WZD s28p04
// MGC s26p04
void Page_Flip(void)
{

    // HACK: 
    Render_VBB(&Buffer);

    draw_page_num = (1 - draw_page_num);  // NOTE: this is the only code that changes 'draw_page_num'

    // ? Meh ? current_video_page = off_page_buffer;

    // MS-DOS & IBM-PC VGA
        // CRT Controller - Address
        // VGA_WaitSync();
    // MS Windows
        // pointer to buffer to render/present
    draw_page = video_page_buffer[draw_page_num];

    Set_Page_Off();

}

// WZD s28p05
// MGC s26p05
void Copy_On_to_Off_Page(void)
{

    if(Get_Pointer_Image_Number() != crsr_None)
    {
        Copy_On_to_Off_YM();
    }
    else
    {
        Copy_On_to_Off_NM();
    }

}

// WZD s28p06
// MGC s26p06
void Copy_On_to_Off_YM(void)
{

}

// WZD s28p07
// MGC s26p07
void Copy_On_to_Off_NM(void)
{
    uint16_t * src;
    uint16_t * dst;
    int16_t itr;

    Save_Mouse_State();

    src = (uint16_t *)video_page_buffer[draw_page_num];
    dst = (uint16_t *)video_page_buffer[1 - draw_page_num];

    itr = 0;
    while(itr++ < 16000)
    {
        *dst++ = *src++;
    }

    Restore_Mouse_State();
}
