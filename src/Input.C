
#include "MoX_TYPE.H"
#include "MoX_DEF.H"

#include "Input.H"
#include "Keyboard.H"


uint8_t g_Key_Pressed;
uint16_t g_Last_Key_Pressed;


int16_t Get_Input(void)
{

    int16_t input;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Get_Input()\n", __FILE__, __LINE__);
#endif

    // TODO if(input_delay > 0)
    // TODO if(input_delay < 0)
    // TODO if(fields_count <= 1)

    input = Process_Input();  // MGC s34p01

    // TODO Set_Page_Off();  // MGC s26p02

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Get_Input()\n", __FILE__, __LINE__);
#endif

    return input;
}

int16_t Process_Input(void)
{
    int16_t field_num;
    int16_t character;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Process_Input()\n", __FILE__, __LINE__);
#endif

    if(Keyboard_Status() == ST_TRUE)  // MGC s33p16
    {
        character = Interpret_Keyboard_Input(&field_num);  // MGC s34p22
    }

    field_num = 0;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Process_Input()\n", __FILE__, __LINE__);
#endif

    return field_num;
}