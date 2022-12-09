
#include "MoX_TYPE.H"
#include "MoX_DEF.H"

#include "Input.H"


int16_t Keyboard_Status(void)
{
    int16_t keyboard_status;

    // MD_Draw_Disable()

    if(g_Key_Pressed == ST_TRUE)
    {
        keyboard_status = ST_TRUE;
    }
    else{
        keyboard_status = ST_FALSE;
    }

    // MD_Draw_Restore()

    return keyboard_status;
}



int16_t Interpret_Keyboard_Input(field_num)
{
    int16_t character;

    character = 27;  // 0x1B ESCAPE

    return character;
}