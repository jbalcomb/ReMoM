
#include "MoX.H"

// WZD s36p66
// MGC s34p66
/*
    returns field_idx/num
*/
int16_t Get_Input(void)
{

    int16_t field_index;

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: BEGIN: Get_Input()\n", __FILE__, __LINE__);
// #endif


    // Pump_Events();


    // TODO if(input_delay > 0)
    // TODO if(input_delay < 0)
    // TODO if(fields_count <= 1)

    // DONT if(mouse_installed == ST_FALSE) { field_index = CRP_GUI_KeyInputOnly(); }

    field_index = Interpret_Mouse_Input();  // WZD s36p01

    Set_Page_Off();

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: END: Get_Input() { field_index = %d }\n", __FILE__, __LINE__, field_index);
// #endif

    return field_index;
}
