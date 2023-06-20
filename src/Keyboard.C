
#include <string.h>

#include "MoX_TYPE.H"
#include "MoX_DEF.H"

#include "Keyboard.H"

#include "Fields.H"
#include "Input.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif


//  WZD dseg:829A  MGC dseg:4D6C
int16_t KD_ActiveStringTrig;
//  WZD dseg:824C  MGC dseg:4D1E
int16_t KD_prev_field_idx;


// Platform:  // WZD s35p14
// Platform:  int16_t Keyboard_Status(void)

// Platform:  // WZD s35p15
// Platform:  int16_t Read_Key(void)

enum Key_Press
{
    KP_Left  = 1,
    KP_Right = 2,
    KP_Up    = 3,
    KP_Down  = 4,

    KP_PgUp = 0x19,
    KP_Home = 0x1A,
    KP_End  = 0x1C,
    KP_PgDn = 0x1D
};


// WZD s34p22
// MGC s34p22
int16_t Interpret_Keyboard_Input(int16_t * field_num)
{
    char Local_String[30];
    uint16_t Input_Key_Extended;
    uint8_t Original_Key;
    uint8_t return_key;
    int16_t Reset_String_Triggers;
    int16_t Loop_Var;
    int16_t Control_Index;
    int16_t _SI_field_idx;
    int16_t itr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Interpret_Keyboard_Input(*field_num = %d)\n", __FILE__, __LINE__, *field_num);
#endif

    return_key = Read_Key();

    if(KD_prev_field_idx >= fields_count)
    {
        KD_prev_field_idx = ST_NULL;
    }

    _SI_field_idx = KD_prev_field_idx;

    Original_Key = return_key;

    if ((return_key > 96) && (return_key < 123) )
    {
        return_key -= 32;
    }

    if(KD_ActiveStringTrig != ST_NULL)
    {
        _SI_field_idx = KD_ActiveStringTrig;
        return_key -= 95;  // treat Alt-A as A
        if(return_key == p_fields[_SI_field_idx].hotkey)
        {
            return_key += 95;
        }
        else
        {
            return_key += 95;
            _SI_field_idx = fields_count;
        }
    }
    else
    {
        _SI_field_idx = fields_count;
    }

    if(_SI_field_idx == fields_count)
    {
        KD_ActiveStringTrig = ST_NULL;  // clear it, because we just failed to match it

        /*
            From previous, Multi Character HotKey Fields
        */
        _SI_field_idx = KD_prev_field_idx + 1;
        while(_SI_field_idx < fields_count)
        {
            if( (return_key == p_fields[_SI_field_idx].hotkey) && (p_fields[_SI_field_idx].type != ft_MultiHotKey) )
            {
                break;
            }

            if(p_fields[_SI_field_idx].type == ft_MultiHotKey)
            {
                return_key -= 95;  // treat Alt-A as A
                if(return_key == p_fields[_SI_field_idx].hotkey)
                {
                    return_key += 95;
                    break;
                }
                else
                {
                    return_key += 95;
                }
            }

            _SI_field_idx++;
        }
    }

    if(_SI_field_idx == fields_count)
    {
        /*
            From first, Multi Character HotKey Fields
        */
        _SI_field_idx = 1;
        while(_SI_field_idx < fields_count)
        {
            if( (return_key == p_fields[_SI_field_idx].hotkey) && (p_fields[_SI_field_idx].type != ft_MultiHotKey) )
            {
                break;
            }

            if(p_fields[_SI_field_idx].type == ft_MultiHotKey)
            {
                return_key -= 95;  // treat Alt-A as A
                if(return_key == p_fields[_SI_field_idx].hotkey)
                {
                    return_key += 95;
                    break;
                }
                else
                {
                    return_key += 95;
                }
            }

            _SI_field_idx++;
        }

    }

    KD_prev_field_idx = _SI_field_idx;
    Reset_String_Triggers = ST_TRUE;

    if(_SI_field_idx < fields_count)
    {
        *field_num = _SI_field_idx;

//         /*
//             Field is On-Screen
//         */
//         if( (p_fields[_SI_field_idx].x1 < 320) && (p_fields[_SI_field_idx].y1 < 200) )
//         {
//             Save_Mouse_State();
//             MOUSE_Emu_X = p_fields[_SI_field_idx].x1 + (ABS((p_fields[_SI_field_idx].x2 - p_fields[_SI_field_idx].x1)) / 2);
//             MOUSE_Emu_Y = p_fields[_SI_field_idx].y1 + (ABS((p_fields[_SI_field_idx].y2 - p_fields[_SI_field_idx].y1)) / 2);
//             if( (MOUSE_Emu_X < 320) && (MOUSE_Emu_Y < 200) )
//             {
//                 Check_Mouse_Shape(MOUSE_Emu_X, MOUSE_Emu_Y);
//                 _cursor_offset = Get_Pointer_Offset();
//                 MOUSE_Emu_X = MOUSE_Emu_X - _cursor_offset;
//                 MOUSE_Emu_Y = MOUSE_Emu_Y - _cursor_offset;
//                 MD_MoveCursor(MOUSE_Emu_X, MOUSE_Emu_Y);
//                 CRL_Restore_RSP();
//                 CRL_Save_RSP(MOUSE_Emu_X, MOUSE_Emu_Y);
//                 CRH_Draw_RSP(MOUSE_Emu_X, MOUSE_Emu_Y);
//             }
//             Restore_Mouse_State();
//         }

//         /*
//             Update the members for the Multi Character HotKey Field
//         */
//         if(p_fields[_SI_field_idx].type == ft_MultiHotKey)
//         {
//             strcpy(Local_String, p_fields[_SI_field_idx].Param0);
//             p_fields[_SI_field_idx].Param1++;
//             if(p_fields[_SI_field_idx].Param1 < p_fields[_SI_field_idx].Param2)
//             {
//                 p_fields[_SI_field_idx].hotkey = Local_String[p_fields[_SI_field_idx].Param1];
//                 KD_ActiveStringTrig = _SI_field_idx;
//                 *field_num = 0;
//                 return_key = 0;
//             }
//             else
//             {
//                 p_fields[_SI_field_idx].Param1 = 0;
//                 p_fields[_SI_field_idx].hotkey = Local_String[0];
//                 KD_ActiveStringTrig == 0;
//             }
//         }

    }
    else  /* if(_SI_field_idx < fields_count) */
    {
//         Control_Index = *field_num;
//         Input_Key_Extended = return_key;
//         switch(Input_Key_Extended)
//         {
//             case KP_Left:  { Control_Index = GUI_ProcessDirKey('L'); } break;
//             case KP_Right: { Control_Index = GUI_ProcessDirKey('R'); } break;
//             case KP_Up:    { Control_Index = GUI_ProcessDirKey('U'); } break;
//             case KP_Down:  { Control_Index = GUI_ProcessDirKey('D'); } break;
//             case KP_PgUp:  { Control_Index = GUI_ProcessDirKey('W'); } break;
//             case KP_Home:  { Control_Index = GUI_ProcessDirKey('X'); } break;
//             case KP_End:   { Control_Index = GUI_ProcessDirKey('Y'); } break;
//             case KP_PgDn:  { Control_Index = GUI_ProcessDirKey('Z'); } break;
//         }
//         *field_num = Control_Index;
    }


//     if(Reset_String_Triggers != ST_FALSE)
//     {
//         Loop_Var = 0;
//         while(Loop_Var < fields_count)
//         {
//             if(p_fields[Loop_Var].type == ft_MultiHotKey)
//             {
//                 strcpy(Local_String, p_fields[Loop_Var].Param0);
//                 p_fields[Loop_Var].Param1 = 0;
//                 p_fields[Loop_Var].hotkey = Local_String[0];
//             }
//             Loop_Var++;
//         }
//     }

    if(return_key != p_fields[*field_num].hotkey)
    {
        return_key = Original_Key;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: return_key: %d\n", __FILE__, __LINE__, return_key);
#endif

//     for(itr = 0; itr < fields_count; itr++)
//     {
//         if(return_key == p_fields[itr].hotkey)
//         {
//             *field_num = itr;
//         }
//     }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: *field_num: %d\n", __FILE__, __LINE__, *field_num);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Interpret_Keyboard_Input(*field_num = %d) { return_key = %d }\n", __FILE__, __LINE__, *field_num, return_key);
#endif

    return return_key;
}
