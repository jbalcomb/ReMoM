
#include "MoX_TYPE.H"
#include "MoX_DEF.H"

#include "Fields.H"
#include "Input.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif



// TODO  // MGC s33p14
int16_t Keyboard_Status(void)
{
    int16_t keyboard_status;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Keyboard_Status()\n", __FILE__, __LINE__);
#endif

    // TODO  MD_Draw_Disable()

    if(g_Key_Pressed == ST_TRUE)
    {
        keyboard_status = ST_TRUE;
    }
    else{
        keyboard_status = ST_FALSE;
    }

    // TODO  MD_Draw_Restore()

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Keyboard_Status() { keyboard_status = %d }\n", __FILE__, __LINE__, keyboard_status);
#endif

    return keyboard_status;
}


// TODO  // MGC s33p15
int16_t Read_Key(void)
{
    int16_t key_pressed;
    int16_t return_key;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Read_Key()\n", __FILE__, __LINE__);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: g_Last_Key_Pressed: %d\n", __FILE__, __LINE__, g_Last_Key_Pressed);
#endif

    key_pressed = g_Last_Key_Pressed;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: key_pressed: %d\n", __FILE__, __LINE__, key_pressed);
#endif

    switch(key_pressed)
    {
        case 0x011B:
        {
            return_key = 27;
        } break;

        default:
        {
            return_key = key_pressed;
        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: return_key: %d\n", __FILE__, __LINE__, return_key);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Read_Key() { return_key = %d }\n", __FILE__, __LINE__, return_key);
#endif

    return return_key;
}

// MGC s34p22
int16_t Interpret_Keyboard_Input(int16_t * field_num)
{
    int16_t character;
    int16_t itr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Interpret_Keyboard_Input()\n", __FILE__, __LINE__);
#endif

    character = Read_Key();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: character: %d\n", __FILE__, __LINE__, character);
#endif

    for(itr = 0; itr < fields_count; itr++)
    {
        if(character == p_fields[itr].hotkey)
        {
            *field_num = itr;
        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: *field_num: %d\n", __FILE__, __LINE__, *field_num);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Interpret_Keyboard_Input() { character = %d }\n", __FILE__, __LINE__, character);
#endif

    return character;
}
