
#include "MoX.H"



// WZD dseg:76EE
int16_t draw_page_num = 0;

uint8_t * current_video_page;
uint8_t * video_page_buffer[2];
uint8_t * draw_page;



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
// PLATFORM  void Page_Flip(void)



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
