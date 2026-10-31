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

#include "Init.h"

#include "../../MoX/src/Fonts.h"
#include "../../MoX/src/Input.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/SOUND.h"
#include "../../MoX/src/random.h"
#include "../../MoX/src/MOX2.h"
#include "../../STU/src/STU_LOG.h"  /* CALL_TRACE */
#include "../../platform/include/Platform.h"
#include "../../MoX/src/Video2.h"

#include "../../ext/stu_compat.h"



char DEFAULT_FONT_FILE[] = "FONTS.LBX";
// TODO  #define DEFAULT_INPUT_TYPE  1  // Keyboard & Mouse
// TODO  #define DEFAULT_VIDEO_MODE  3  // vm_Mode_Y


// WZD s014p01
// MoO2  Module: init  void Init_Drivers()
void Init_Drivers(int input_type, int sound_channels, char * font_file, int midi_driver, int MIDI_IO, int MIDI_IRQ, int MIDI_DMA, int digi_driver, int Digi_IO, int Digi_IRQ, int Digi_DMA)
{
    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    /* NEWCODE */  // DBG_Open_ERROR_LOG();  // from MoO1

    EMS_Startup();

    // TODO  Create_IO_Buffer();  /* MoO2 */

    // VGA_SetModeY()  // MoO2  Init_Video_Drivers() ... |-> Set_Video_Mode()
    Init_Video_Drivers(VIDEO_MODE);

    if(input_type == ST_UNDEFINED)
    {

        // MoO2  Load_System_Font()
        Load_Font_File(DEFAULT_FONT_FILE);

        // Audio_Init__STUB(0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1);  // IDGI two parameters too many
        // TODO  Audio_Init__WIP(0, 0, -1, -1, -1, -1, -1, -1, -1);

        Init_Mouse_Keyboard(1);  // ¿ 1 is what now ?

    }
    else
    {

        Load_Font_File(font_file);

        // TODO  Audio_Init__WIP(midi_driver, sound_channels, MIDI_IO, MIDI_IRQ, MIDI_DMA, digi_driver, Digi_IO, Digi_IRQ, Digi_DMA);

        Init_Mouse_Keyboard(input_type);

    }

    /* set the random seed for the new game path, with support for deterministic behavior */
    if(_cmd_line_seed != 0)
    {
        Set_Random_Seed((uint32_t)_cmd_line_seed);
    }
    else
    {
        Randomize();
    }

    Set_Page_Off();  // initializes `current_video_page`

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);
}

// WZD s014p02
/*
also in MoO1, but also unused
seems like maybe a debug init, without video or audio ... headless?
*/
void UU_Legacy_Startup(int input_type, int midi_driver, int sound_channels, char * font_file)
{

    /* NEWCODE */  // DBG_Open_ERROR_LOG();  // from MoO1

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

    /* set the random seed for the new game path, with support for deterministic behavior */
    if(_cmd_line_seed != 0)
    {
        Set_Random_Seed((uint32_t)_cmd_line_seed);
    }
    else
    {
        Randomize();
    }

}
