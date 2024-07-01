/*
    WIZARDS.EXE
        seg014

MoO2  Module: init
MoO2  Module: graphics  Set_Window(), Reset_Window(), Fill(), 
MoO2  Module: fonts     Load_Font_File()

¿ MoO2  Module: INITGAME ?
¿ MoO2  Module: INITSHIP ?
¿ MoO2  Module: initargv ?
¿ MoO2  Module: initfile ?

*/
#include "MoX.H"

char DEFAULT_FONT_FILE[] = "FONTS.LBX";
// TODO  #define DEFAULT_INPUT_TYPE  1  // Keyboard & Mouse
// TODO  #define DEFAULT_VIDEO_MODE  3  // vm_Mode_Y


// MoO2  Module: init  void Init_Drivers()
// WZD s014p01
void Init_Drivers(int input_type, char * font_file)
{

    DBG_Open_ERROR_LOG();
    
    EMS_Startup();

    // VGA_SetModeY()  // MoO2  Init_Video_Drivers() ... |-> Set_Video_Mode()
    Init_Video_Drivers(VIDEO_MODE);

    if(input_type == ST_UNDEFINED)
    {
        // MoO2  Load_System_Font()
        Load_Font_File(DEFAULT_FONT_FILE);
        // TODO  Audio_init(0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1);
        Init_Mouse_Keyboard(1);
    }
    else
    {
        Load_Font_File(font_file);
        // TODO  Audio_Init(M_Drv, Snd_Chnls, M_IO, M_IRQ, M_DMA, D_Drv, D_IO, D_IRQ, D_DMA);
        Init_Mouse_Keyboard(input_type);
    }

    Randomize();

    Set_Page_Off();  // initializes `current_video_page`

}

// WZD s014p02
/*
also in MoO1, but also unused
seems like maybe a debug init, without video or audio ... headless?

drake178: Unused in MoM  most likely a leftover from a previous game, that either didn't use AIL, or set it up differently
*/
void UU_Legacy_Startup(int input_type, int midi_driver, int sound_channels, char * font_file)
{
    DBG_Open_ERROR_LOG();
    
    EMS_Startup();

    if(input_type == ST_UNDEFINED)
    {
        Load_Font_File(DEFAULT_FONT_FILE);
        // TODO  Audio_init(2, 2);
        Init_Mouse_Keyboard(1);
    }
    else
    {
        Load_Font_File(font_file);
        // TODO  Audio_Init(sound_channels, midi_driver);
        Init_Mouse_Keyboard(input_type);
    }

    Randomize();

}
