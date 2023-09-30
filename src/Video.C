
#include "MoX.H"



// WZD dseg:76EE
int16_t draw_page_num = 0;

uint8_t * current_video_page;
uint8_t * video_page_buffer[3];  // On, Off, Back, ¿ Back2 ?
uint8_t * draw_page;



// WZD s28p01
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


// WZD s28p02
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
// PLATFORM  void Page_Flip(void)



// WZD s28p05
// MoO2  Copy_Off_To_On_Page()
void Copy_On_To_Off_Page(void)
{

    if(Get_Pointer_Image_Number() != crsr_None)
    {
        Copy_On_To_Off_YM();
    }
    else
    {
        Copy_On_To_Off_NM();
    }

}

// WZD s28p06
/*
    ¿¿¿
        copy 4000 bytes at time, so 1/4 memory plane per itr?
        ¿ related to width of cursor image ?


    ???
*/
void Copy_On_To_Off_YM(void)
{
    int16_t mouse_x;
    int16_t mouse_y;
    uint8_t * src;
    uint8_t * dst;
    int16_t itr;
    int16_t itr_rep_movsb;
    int16_t tmp_mouse_x;
    int16_t tmp_mouse_y;

    Save_Mouse_State();

    mouse_x = Pointer_X();
    mouse_y = Pointer_Y();


    src = (uint8_t *)video_page_buffer[draw_page_num];
    dst = (uint8_t *)video_page_buffer[1 - draw_page_num];


    itr = 0;
    // while(itr++ < 16000)
    while(itr++ < (screen_pixel_size / (screen_pixel_size / 4)))
    {
        itr_rep_movsb = 0;
        while(itr_rep_movsb++ < (screen_pixel_size / 4))
        {
            *dst++ = *src++;
        }

        Restore_Mouse_Off_Page();

        tmp_mouse_x = Pointer_X();
        tmp_mouse_y = Pointer_Y();
        if( (tmp_mouse_x != mouse_x) || (tmp_mouse_y != mouse_y) )
        {
            Check_Mouse_Shape(tmp_mouse_x, tmp_mouse_y);
            Restore_Mouse_On_Page();
            Save_Mouse_On_Page(tmp_mouse_x, tmp_mouse_y);
            Check_Mouse_Shape(tmp_mouse_x, tmp_mouse_y);
            Draw_Mouse_On_Page(tmp_mouse_x, tmp_mouse_y);
            Set_Pointer_Position(tmp_mouse_x, tmp_mouse_y);
        }

    }

    Restore_Mouse_Off_Page();

    Restore_Mouse_State();

}

// WZD s28p07
// TODO  this should probably just be Copy_On_To_Off(), since it's just like the other Copy_On/Offs
void Copy_On_To_Off_NM(void)
{
    uint8_t * src;
    uint8_t * dst;
    int16_t itr;

    Save_Mouse_State();

    src = (uint8_t *)video_page_buffer[draw_page_num];
    dst = (uint8_t *)video_page_buffer[1 - draw_page_num];

    itr = 0;
    // while(itr++ < 16000)
    while(itr++ < screen_pixel_size)
    {
        *dst++ = *src++;
    }

    Restore_Mouse_State();
}

// WZD s28p08
// copy VGA RAM 0xA000 to VGA RAM 0xA800 - frame-buffer 0 to frame-buffer 2
void Copy_Off_To_Back(void)
{
    uint16_t * src;
    uint16_t * dst;
    int16_t itr;

    src = (uint16_t *)video_page_buffer[1 - draw_page_num];
    dst = (uint16_t *)video_page_buffer[2];

    itr = 0;
    // while(itr++ < 16000)
    while(itr++ < (screen_pixel_size / 2))
    {
        *dst++ = *src++;
    }
}

// WZD s28p09
// copy VGA RAM 0xA800 to VGA RAM 0xA000 - frame-buffer 2 to frame-buffer 0
void Copy_Back_To_Off(void)
{
    uint16_t * src;
    uint16_t * dst;
    int16_t itr;

    src = (uint16_t *)video_page_buffer[2];
    dst = (uint16_t *)video_page_buffer[1 - draw_page_num];

    itr = 0;
    // while(itr++ < 16000)
    while(itr++ < (screen_pixel_size / 2))
    {
        *dst++ = *src++;
    }
}

// WZD s28p10
// drake178: VGA_SaveDraw_Frame4()
// copy VGA RAM 0xA000 to VGA RAM 0xAC00 - frame-buffer 0 to frame-buffer 3
void VGA_SaveDraw_Frame4(void)
{
    uint16_t * src;
    uint16_t * dst;
    int16_t itr;

    src = (uint16_t *)video_page_buffer[1 - draw_page_num];
    dst = (uint16_t *)video_page_buffer[2];

    itr = 0;
    // while(itr++ < 16000)
    while(itr++ < (screen_pixel_size / 2))
    {
        *dst++ = *src++;
    }

}

// WZD s28p11
// drake178: VGA_LoadDraw_Frame4()
// copy VGA RAM 0xAC00 to VGA RAM 0xA000 - frame-buffer 3 to frame-buffer 0
void VGA_LoadDraw_Frame4(void)
{
    uint16_t * src;
    uint16_t * dst;
    int16_t itr;

    src = (uint16_t *)video_page_buffer[2];
    dst = (uint16_t *)video_page_buffer[1 - draw_page_num];

    itr = 0;
    // while(itr++ < 16000)
    while(itr++ < (screen_pixel_size / 2))
    {
        *dst++ = *src++;
    }
}
