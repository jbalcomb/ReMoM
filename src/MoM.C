
#include "MoM.H"

#include "MoM_main.H"

#include "Mouse.H"  /* mouse_x, mouse_y */
#include "Video.H"

/*

screen_pixel_width1   =      0
screen_pixel_width2   =    640
screen_pixel_height1  =      0
screen_pixel_height2  =    480
screen_pixel_size     = 307200 ; (640 * 480)

draw_page_num

Set_Page_Off()
    current_page_flag = 1
    current_video_page = __off_page_buffer
    copy_to_on_page_flag = 0

Set_Page_Back()
    current_page_flag = 2
    current_video_page = __back_page_buffer
    copy_to_on_page_flag = 0

Set_Page_On()
    current_page_flag = 0
    current_video_page = __off_page_buffer
    copy_to_on_page_flag = 1

*/


uint16_t screen_pixel_width   =  320;
uint16_t screen_pixel_height  =  200;
// error C2099: initializer is not a constant
// uint16_t screen_pixel_size    =  (screen_pixel_width * screen_pixel_height);
uint16_t screen_pixel_size    =  64000;


void Render_VBB(struct game_offscreen_buffer * Buffer)
{

    uint32_t * Pixel = (uint32_t*)Buffer->Memory;
    int itr;
    unsigned char vbb_byte;
    unsigned int * p_XBGR;
    unsigned int long_XBGR;
    p_XBGR = (uint32_t *)g_Palette_XBGR;
    for(itr = 0; itr < 64000; itr++)
    {
        // vbb_byte = *(g_Video_Back_Buffer + itr);
        vbb_byte = *(current_video_page + itr);
        long_XBGR = *(p_XBGR + vbb_byte);
        *Pixel++ = long_XBGR;
    }

}

void GameUpdateAndRender(struct game_offscreen_buffer * Buffer)
{
    Render_VBB(Buffer);
}

void Update_Mouse_Position(int16_t platform_mouse_x, int16_t platform_mouse_y)
{
    mouse_x = platform_mouse_x / 2;
    mouse_y = platform_mouse_y / 2;
}
