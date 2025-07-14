
#include "Fields.h"
#include "MOX_DEF.H"
#include "MOX_TYPE.h"

#include <string.h>

#include "Keyboard.h"


// WZD dseg:8240                                                 ? BEGIN: Mouse Buffer - Initialized Data ?
// WZD dseg:824C  MGC dseg:4D1E
int16_t KD_prev_field_idx = 0;
// WZD dseg:829A                                                 ? BEGIN: Mouse Buffer ?
// WZD dseg:829A  MGC dseg:4D6C
int16_t multi_hotkey_active_field = 0;


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
// 1oom  uiobj.c  uiobj_handle_kbd()
// MoO2  Module: fields  Interpret_Keyboard_Input()
/*
function (0 bytes) Interpret_Keyboard_Input
Address: 01:0011CACE
    Num params: 1
    Return type: signed integer (2 bytes) 
    pointer (4 bytes) 
    Locals:
        pointer (4 bytes) field_num
        signed integer (2 bytes) count
        signed integer (2 bytes) key
        signed integer (2 bytes) mouse_x
        signed integer (2 bytes) mouse_y
        signed integer (2 bytes) current_field
        signed integer (2 bytes) next_field
        signed integer (2 bytes) i
        signed integer (2 bytes) character
        signed integer (2 bytes) clear_hotkeys
        signed integer (2 bytes) original_character
        signed integer (2 bytes) fkey_hit
        array (40 bytes) temp_string
        Num elements:   40, Type:                unsigned integer (4 bytes) 
*/
uint8_t Interpret_Keyboard_Input(int16_t * field_num)
{
    char temp_string[30];
    uint16_t dirkey_character;
    uint8_t original_character;
    uint8_t character;
    int16_t clear_hotkeys;
    int16_t itr_fields_count;
    int16_t dirkey_field_idx;
    int16_t _SI_field_idx;
    int16_t itr;

    character = Read_Key();

    if(KD_prev_field_idx >= fields_count)
    {
        KD_prev_field_idx = 0;
    }

    _SI_field_idx = (KD_prev_field_idx + 1);

    original_character = character;

    UPPERCASE(character);


    /*
        Active, check MultiHotKey Field
    */
    {
        if(multi_hotkey_active_field == 0)
        {
            _SI_field_idx = fields_count;
        }
        else
        {
            _SI_field_idx = multi_hotkey_active_field;
            character -= 95;  // e.g., convert Alt-A to A
            if(character == p_fields[_SI_field_idx].hotkey)
            {
                character += 95;
            }
            else
            {
                character += 95;
                _SI_field_idx = fields_count;
            }
        }
    }

    /*
        From Next, check MultiHotKey Fields
    */
    {
        if(_SI_field_idx == fields_count)
        {
            multi_hotkey_active_field = 0;
            _SI_field_idx = KD_prev_field_idx + 1;
            while(((p_fields[_SI_field_idx].hotkey != character) || (p_fields[_SI_field_idx].type == ft_MultiHotKey)) && (_SI_field_idx != fields_count))
            {
                if(p_fields[_SI_field_idx].type == ft_MultiHotKey)
                {
                    character -= 95;  // e.g., convert Alt-A to A
                    if(character == p_fields[_SI_field_idx].hotkey)
                    {
                        character += 95;
                        break;
                    }
                    else
                    {
                        character += 95;
                    }
                }
                _SI_field_idx++;
            }
        }
    }

    /*
        From First, check MultiHotKey Fields
    */
    {
        if(_SI_field_idx == fields_count)
        {
            _SI_field_idx = 1;
            while(((p_fields[_SI_field_idx].hotkey != character) || (p_fields[_SI_field_idx].type == ft_MultiHotKey)) && (_SI_field_idx != fields_count))
            {
                if(p_fields[_SI_field_idx].type == ft_MultiHotKey)
                {
                    character -= 95;  // e.g., convert Alt-A to A
                    if(character == p_fields[_SI_field_idx].hotkey)
                    {
                        character += 95;
                        break;
                    }
                    else
                    {
                        character += 95;
                    }
                }
                _SI_field_idx++;
            }
        }
    }


    KD_prev_field_idx = _SI_field_idx;

    clear_hotkeys = ST_TRUE;


    // HERE:  
    if(_SI_field_idx < fields_count)
    {
        *field_num = _SI_field_idx;
        if((p_fields[_SI_field_idx].x1 < SCREEN_WIDTH) && (p_fields[_SI_field_idx].y1 < SCREEN_HEIGHT)
        )
        {
            // ~ UU_GUI_MouseEMUMoveTo()
            Save_Mouse_State();
            MOUSE_Emu_X = (p_fields[_SI_field_idx].x1 + ((p_fields[_SI_field_idx].x2 - p_fields[_SI_field_idx].x1) / 2));
            MOUSE_Emu_Y = (p_fields[_SI_field_idx].y1 + ((p_fields[_SI_field_idx].y2 - p_fields[_SI_field_idx].y1) / 2));
            if((MOUSE_Emu_X < SCREEN_WIDTH) && (MOUSE_Emu_Y < SCREEN_HEIGHT))
            {
                Check_Mouse_Shape(MOUSE_Emu_X, MOUSE_Emu_Y);
                pointer_offset = Get_Pointer_Offset();
                MOUSE_Emu_X = (MOUSE_Emu_X - pointer_offset);
                MOUSE_Emu_Y = (MOUSE_Emu_Y - pointer_offset);
                Set_Pointer_Position(MOUSE_Emu_X, MOUSE_Emu_Y);
                Restore_Mouse_On_Page();
                Save_Mouse_On_Page(MOUSE_Emu_X, MOUSE_Emu_Y);
                Draw_Mouse_On_Page(MOUSE_Emu_X, MOUSE_Emu_Y);
            }
            Restore_Mouse_State();
        }

        if(p_fields[_SI_field_idx].type == ft_MultiHotKey)
        {
            strcpy(temp_string, p_fields[_SI_field_idx].string);
            p_fields[_SI_field_idx].string_pos++;
            if(p_fields[_SI_field_idx].string_pos < p_fields[_SI_field_idx].string_len)
            {
                p_fields[_SI_field_idx].hotkey = temp_string[p_fields[_SI_field_idx].string_pos];
                multi_hotkey_active_field = _SI_field_idx;
                *field_num = 0;
                character = 0;
            }
            else
            {
                p_fields[_SI_field_idx].string_pos = 0;
                p_fields[_SI_field_idx].hotkey = temp_string[0];
                multi_hotkey_active_field = 0;
            }
        }
    }
    else  /* if(_SI_field_idx < fields_count) */
    {
        dirkey_field_idx = *field_num;
        dirkey_character = character;
        switch(dirkey_character)
        {
            // TODO  case KP_Left:  { dirkey_field_idx = Process_Direction_Key__STUB('L'); } break;
            // TODO  case KP_Right: { dirkey_field_idx = Process_Direction_Key__STUB('R'); } break;
            // TODO  case KP_Up:    { dirkey_field_idx = Process_Direction_Key__STUB('U'); } break;
            // TODO  case KP_Down:  { dirkey_field_idx = Process_Direction_Key__STUB('D'); } break;
            // TODO  case KP_PgUp:  { dirkey_field_idx = Process_Direction_Key__STUB('W'); } break;
            // TODO  case KP_Home:  { dirkey_field_idx = Process_Direction_Key__STUB('X'); } break;
            // TODO  case KP_End:   { dirkey_field_idx = Process_Direction_Key__STUB('Y'); } break;
            // TODO  case KP_PgDn:  { dirkey_field_idx = Process_Direction_Key__STUB('Z'); } break;
        }
        *field_num = dirkey_field_idx;
    }


    if(clear_hotkeys != ST_FALSE)
    {
        for(itr_fields_count = 0; itr_fields_count < fields_count; itr_fields_count++)
        {
            if(p_fields[itr_fields_count].type == ft_MultiHotKey)
            {
                strcpy(temp_string, p_fields[itr_fields_count].string);
                p_fields[itr_fields_count].string_pos = 0;
                p_fields[itr_fields_count].hotkey = temp_string[0];
            }
        }
    }


    if(p_fields[*field_num].hotkey != character)
    {
        character = original_character;
    }

    return character;
}
