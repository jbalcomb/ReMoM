
#include "MoX.H"

#define DEFAULT_FONT_FILE "FONTS.LBX"

void Init_Drivers(int input_type, char * font_file)
{

    // CRP_Empty_Exit_Fn2()

    // EMS_Startup()

    // VGA_SetModeY()  // MoO2  Init_Video_Drivers() ... |-> Set_Video_Mode()
    Init_Video_Drivers(VIDEO_MODE);

    if(input_type == ST_UNDEFINED)
    {
        // MoO2  Load_System_Font()
        Load_Font_File(DEFAULT_FONT_FILE);
        // TODO  Audio_init(0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1);
        input_type = 1;
    }
    else
    {
        Load_Font_File(font_file);
        // TODO  Audio_Init(M_Drv, Snd_Chnls, M_IO, M_IRQ, M_DMA, D_Drv, D_IO, D_IRQ, D_DMA);
    }

    Init_Mouse_Keyboard(input_type);

    Randomize();

    Set_Page_Off();  // initializes `current_video_page`

}