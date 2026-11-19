
#include "../../STU/src/STU_DBG.h"

#include "../../ext/stu_compat.h"

#include "Fields.h"
#include "Input.h"
#include "MOX_DEF.h"
#include "MOX_KEYS.h"
#include "MOX_TYPE.h"
#include "capture.h"

#include <string.h>

#include "Keyboard.h"

#include "../../platform/include/Platform.h"
#include "../../platform/include/Platform_Keys.h"
#include "../../STU/src/STU_LOG.h"



// WZD dseg:8240                                                 ? BEGIN: Mouse Buffer - Initialized Data ?
// WZD dseg:824C  MGC dseg:4D1E
int16_t KD_prev_field_idx = 0;
// WZD dseg:829A                                                 ? BEGIN: Mouse Buffer ?
// WZD dseg:829A  MGC dseg:4D6C
int16_t multi_hotkey_active_field = 0;



// WZD s35p16
int16_t Keyboard_Status(void)
{
    if(key_pressed == ST_TRUE)
    {
        return ST_TRUE;
    }
    else
    {
        return ST_FALSE;
    }
}


// WZD s35p17
/*
    1oom/Kilgore
        ...
        keyp = Read_Key();
        key = KBD_GET_KEY(keyp);
        character = KBD_GET_CHAR(keyp);
        ...

*/
uint8_t Read_Key(void)
{
    // uint16_t key_num;
    //
    // if(Keyboard_Status())
    // {
    //     key_num = platform_keyboard_buffer.key_num[platform_keyboard_buffer.key_read];
    //
    //     platform_keyboard_buffer.key_read = ((platform_keyboard_buffer.key_read + 1) % PLATFORM_KEYBOARD_BUFFER_LENGTH);
    //
    //     if(platform_keyboard_buffer.key_read == platform_keyboard_buffer.key_write)
    //     {
    //         key_pressed = ST_FALSE;
    //     }
    //     else
    //     {
    //         key_pressed = ST_TRUE;
    //     }
    // }

    uint32_t packed_key_value;
    uint16_t packed_key_code;
    char packed_character;
    uint16_t key_num;

    if(Keyboard_Status())
    {
        packed_key_value = platform_keyboard_buffer.packed_key[platform_keyboard_buffer.key_read];

        /* CLAUDE: diagnostic — trace every consumption of the keyboard buffer.  If this fires BEFORE the corresponding REC_CAP trace for the same frame, the capture race from Input.c's comment is back. */
        LOG_TRACE(LOG_CAT_GENERAL, "KBD_READ t=%llu packed=0x%08X key_read=%d->%d key_write=%d", (unsigned long long)Platform_Get_Millies(), (unsigned)packed_key_value, platform_keyboard_buffer.key_read, (platform_keyboard_buffer.key_read + 1) % PLATFORM_KEYBOARD_BUFFER_LENGTH, platform_keyboard_buffer.key_write);

        platform_keyboard_buffer.key_read = ((platform_keyboard_buffer.key_read + 1) % PLATFORM_KEYBOARD_BUFFER_LENGTH);

        if(platform_keyboard_buffer.key_read == platform_keyboard_buffer.key_write)
        {
            key_pressed = ST_FALSE;
        }
        else
        {
            key_pressed = ST_TRUE;
        }
    }

    packed_key_code = KBD_GET_KEY(packed_key_value);
    packed_character = KBD_GET_CHAR(packed_key_value);

    if(packed_character)
    {
        key_num = packed_character;
    }
    else
    {
        key_num = packed_key_code;
    }

    /* CLAUDE */  /* DOS Alt+key scancodes are UPPERCASE(character) + 95; match that for multi-hotkey support (e.g., Alt+R,V,L cheat codes) */
    /* CLAUDE */  if((packed_key_value & MOX_MOD_ALT) && packed_character >= 'A' && packed_character <= 'z')
    /* CLAUDE */  {
    /* CLAUDE */      char alt_char = packed_character;
    /* CLAUDE */      if(alt_char >= 'a' && alt_char <= 'z') { alt_char -= 32; }
    /* CLAUDE */      key_num = (uint16_t)alt_char + 95;
    /* CLAUDE */  }

    return (uint8_t)key_num;  // TODO  why/how key_num is diff from return or packed_key or KBD_GET
}

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


// WZD s36p21
// MGC s34p21
// drake178: RP_GUI_KeyInputOnly()
/*
    The no-mouse counterpart of Interpret_Mouse_Input(): Get_Input() calls it instead when mouse_installed is ST_FALSE.
    Drives the emulated pointer (emulated_pointer_x, emulated_pointer_y) from the keyboard: a field's hotkey centers the emulated pointer on that field and returns it; ENTER acts on the
    field under the emulated pointer (string-input focus or commit, buttons, the selected list field); '+' / '-' step the scroll bar under it; other keys edit the active string-input field.
    Returns the field number, a ClickLink field's parent, ST_UNDEFINED for ESC, otherwise 0.
    ReMoM never reaches it: Init_Mouse_Keyboard() always sets mouse_installed = ST_TRUE (HACK).
*/
// #ifdef 0
/* CLAUDE */  /* the paste's `#ifdef 0` (C1016, broke the MoX build) is gone: the function is enabled; Get_Input() calls it as the asm does */
// int /* far */ RP_GUI_KeyInputOnly(void)
int16_t Interpret_Keyboard_Only_Input(void)
{
    uint8_t niu_var_c = 0;  /* IDA's var_C, bp-0Ch: no instruction reads or writes it (MAGIC.EXE's database defines no var_C for the same frame) */
    // char Input_Key;
    /* CLAUDE */  /* asm: byte; the range tests use jle / jl (signed), but `== ST_KEY_F11` (91h) / `== ST_KEY_F12` (92h) and the hotkey test are byte compares, which a signed char breaks in C; every range bound is below 80h, so unsigned gives the same results */
    // uint8_t input_key = 0;
    /* CLAUDE */  /* REBUG */  /* asm: Input_Key= byte ptr -0Bh, signed (cmp [bp+Input_Key], 40h / jle) */
    int8_t input_key = 0;
    int16_t valid_character = 0;
    int16_t keyed_field = 0;
    int16_t pointer_y = 0;
    int16_t pointer_x = 0;
    int16_t scanned_field = 0;
    int16_t field_num = 0;  // _SI_  also the string position in the string-input loops
    int16_t itr_continuous_string = 0;  // _DI_

    (void)niu_var_c;  /* CLAUDE */

    pointer_x = emulated_pointer_x;
    pointer_y = emulated_pointer_y;

    Restore_Mouse_On_Page();
    Save_Mouse_On_Page(emulated_pointer_x, emulated_pointer_y);
    Draw_Mouse_On_Page(emulated_pointer_x, emulated_pointer_y);
    // Get_Pointer_Image_Number(pointer_x, pointer_y);
    /* CLAUDE */  /* asm: push [bp+Pointer_Y] / push [bp+Pointer_X] / call Get_Pointer_Image_Number / pop cx / pop cx - the OG passes two arguments to a function that takes none and drops its result */
    Get_Pointer_Image_Number();
    pointer_offset = Get_Pointer_Offset();

    /* OGBUG  dead store: scanned_field is written twice and never read */
    scanned_field = 0;
    scanned_field = Scan_Field();

    keyed_field = 0;
    input_key = 0;

    if(Keyboard_Status() != ST_FALSE)
    {
        input_key = Interpret_Keyboard_Input(&keyed_field);

        // if(input_key == ST_KEY_F11)
        /* CLAUDE */  /* REBUG */  /* asm: cmp [bp+Input_Key], KP_F11 - a byte compare */
        if(input_key == (int8_t)ST_KEY_F11)
        {
            F11_Key();
        }

        // if(input_key == ST_KEY_F12)
        /* CLAUDE */  /* REBUG */  /* asm: cmp [bp+Input_Key], KP_F12 - a byte compare */
        if(input_key == (int8_t)ST_KEY_F12)
        {
            Restore_Mouse_On_Page();
            Screen_Flic_Capture();
            Save_Mouse_On_Page(Pointer_X(), Pointer_Y());
            Draw_Mouse_On_Page(Pointer_X(), Pointer_Y());
            Set_Pointer_Position(Pointer_X(), Pointer_Y());
        }

        if(input_key == ST_KEY_ESCAPE)
        {
            return ST_UNDEFINED;
        }

        // if(p_fields[keyed_field].hotkey == input_key)
        /* CLAUDE */  /* REBUG */  /* asm: mov al, [byte ptr es:bx+s_Field.hotkey] / cmp al, [bp+Input_Key] - a byte compare */
        if((int8_t)p_fields[keyed_field].hotkey == input_key)
        {
            if(input_key == 0)
            {
                return 0;
            }

            if(keyed_field == 0)
            {
                down_mouse_button = ST_UNDEFINED;
                return keyed_field;
            }

            pointer_x = emulated_pointer_x;
            pointer_y = emulated_pointer_y;

            emulated_pointer_x = (p_fields[keyed_field].x1 + ((p_fields[keyed_field].x2 - p_fields[keyed_field].x1) / 2));
            emulated_pointer_y = (p_fields[keyed_field].y1 + ((p_fields[keyed_field].y2 - p_fields[keyed_field].y1) / 2));

            if((emulated_pointer_x < 0) || (emulated_pointer_x > (SCREEN_WIDTH - 1)))
            {
                emulated_pointer_x = pointer_x;
            }

            if((emulated_pointer_y < 0) || (emulated_pointer_y > (SCREEN_HEIGHT - 1)))
            {
                emulated_pointer_y = pointer_y;
            }

            Push_Field_Down(keyed_field, emulated_pointer_x, emulated_pointer_y);

            switch(p_fields[keyed_field].type)
            {
                case ft_LockedButton:
                {
                    if(*p_fields[keyed_field].state == 0)
                    {
                        *p_fields[keyed_field].state = 1;
                    }
                    else
                    {
                        down_mouse_button = ST_UNDEFINED;
                        return 0;
                    }
                } break;
                case ft_ClickLink:
                {
                    return p_fields[keyed_field].parent;
                } break;
                case ft_RadioButton:
                {
                    if(*p_fields[keyed_field].state == 0)
                    {
                        *p_fields[keyed_field].state = 1;
                    }
                    else
                    {
                        *p_fields[keyed_field].state = 0;
                    }
                } break;
                default:
                {

                } break;
            }

            down_mouse_button = ST_UNDEFINED;
            return keyed_field;
        }

        if(input_key == ST_KEY_ENTER)
        {
            /* OGBUG  dead store: no path from here reads pointer_x / pointer_y again */
            pointer_x = emulated_pointer_x;
            pointer_y = emulated_pointer_y;

            field_num = Scan_Field();

            if(field_num > 0)
            {
                if(p_fields[field_num].type == ft_ContinuousStringInput)
                {
                    if(input_field_active == ST_FALSE)
                    {
                        stu_strcpy(continuous_string, p_fields[field_num].string);
                        GUI_EditAnimStage = 0;
                        GUI_EditCursorOn = ST_FALSE;
                        input_field_active = ST_TRUE;
                        active_input_field_number = field_num;
                    }
                    else if(active_input_field_number != field_num)
                    {
                        stu_strcpy(continuous_string, p_fields[field_num].string);
                        GUI_EditAnimStage = 0;
                        GUI_EditCursorOn = ST_FALSE;
                        active_input_field_number = field_num;
                    }
                }
                else
                {
                    if(
                        (input_field_active != ST_FALSE)
                        &&
                        (p_fields[field_num].type != ft_ContinuousStringInput)
                    )
                    {
                        itr_continuous_string = 0;
                        // /* Note: assembly indexes p_fields by di and reads Param0 (labeled s_SPELL_DATA.Param0 in IDA) */
                        // while (continuous_string[di] != 0 && p_fields[di].Param0 > di)
                        /* CLAUDE */  /* REBUG */  /* asm: cmp [word ptr es:bx+s_SPELL_DATA.Param0], di / jg loc_27C3B - IDA's struct label is wrong: its offset is 20h, s_Field.Param5 (max_characters), not s_Field.Param0 (16h) */
                        /* OGBUG  p_fields is indexed by the string position (di), not active_input_field_number (Interpret_Mouse_Input()'s commit uses p_fields[active_input_field_number].max_characters);
                                  di starts at 0 and p_fields[0].max_characters is 0 (Clear_Fields()), so the loop never advances and the commit below stores an empty string */
                        while((continuous_string[itr_continuous_string] != '\0') && (p_fields[itr_continuous_string].max_characters > itr_continuous_string))
                        {
                            itr_continuous_string++;
                        }

                        // if (continuous_string[di - 1] == '_')
                        /* OGBUG  with itr_continuous_string 0 the test reads continuous_string[-1], the high byte of last_button_x (0 or 1, never '_') */
                        /* HACK */  if((itr_continuous_string > 0) && (continuous_string[(itr_continuous_string - 1)] == '_'))
                        {
                            itr_continuous_string--;
                        }

                        continuous_string[itr_continuous_string] = '\0';
                        stu_strcpy(p_fields[active_input_field_number].string, continuous_string);
                        GUI_EditAnimStage = 0;
                        input_field_active = ST_FALSE;
                        active_input_field_number = ST_UNDEFINED;
                    }
                }
            }

            if(field_num != 0)
            {
                if(
                    (p_fields[field_num].type != ft_Scroll)
                    &&
                    (p_fields[field_num].type != ft_ContinuousStringInput)
                )
                {
                    Push_Field_Down(field_num, emulated_pointer_x, emulated_pointer_y);
                }

                switch(p_fields[field_num].type)
                {
                    case ft_LockedButton:
                    {
                        if(*p_fields[field_num].state == 0)
                        {
                            *p_fields[field_num].state = 1;
                        }
                        else
                        {
                            down_mouse_button = ST_UNDEFINED;
                            return 0;
                        }
                    } break;
                    case ft_ClickLink:
                    {
                        return p_fields[field_num].parent;
                    } break;
                    case ft_RadioButton:
                    {
                        /* Note: assembly checks p_fields[Keyed_Control].Param2 here, then writes to p_fields[si].Param2 */
                        /* OGBUG  tests keyed_field's state but sets field_num's: on this path keyed_field is 0 (ENTER matched no hotkey, so Interpret_Keyboard_Input() left it as set),
                                  p_fields[0].Param2 is 0 (Clear_Fields()), and the OG reads the word at DS:0000 (Borland's null-check bytes, 0), so ENTER only ever sets a radio button, never clears it */
                        // if(*p_fields[keyed_field].state == 0)
                        /* HACK */  if((keyed_field == 0) || (*p_fields[keyed_field].state == 0))
                        {
                            *p_fields[field_num].state = 1;
                        }
                        else
                        {
                            *p_fields[field_num].state = 0;
                        }
                    } break;
                    default:
                    {

                    } break;
                }

                if(mouse_auto_exit == ST_FALSE)
                {
                    Quick_Call_Auto_Function();
                }

                return field_num;
            }
            else
            {
                if(list_field_on != ST_FALSE)
                {
                    for(field_num = 1; field_num < fields_count; field_num++)
                    {
                        if(
                            (p_fields[field_num].type == ft_StringList)
                            &&
                            (*p_fields[field_num].variable1 == p_fields[field_num].set_value)
                            &&
                            (p_fields[field_num].active_flag != ST_FALSE)
                        )
                        {
                            down_mouse_button = ST_UNDEFINED;
                            return field_num;
                        }
                    }
                }
            }
        }

        if((input_key == '+') || (input_key == '-'))
        {
            field_num = Scan_Field();

            if(field_num != 0)
            {
                if(p_fields[field_num].type == ft_Scroll)
                {
                    if(input_key == '+')
                    {
                        Increment_Scroll_Bar(field_num);
                    }

                    if(input_key == '-')
                    {
                        Decrement_Scroll_Bar(field_num);
                    }

                    return field_num;
                }
                else
                {
                    return 0;
                }
            }
        }

        if(input_field_active > 0)
        {
            if(input_key == ST_KEY_BACKSPACE)
            {
                field_num = 0;
                while(
                    (continuous_string[field_num] != '\0')
                    &&
                    (continuous_string[field_num] != '_')
                    &&
                    ((p_fields[active_input_field_number].max_characters - 1) > field_num)
                )
                {
                    field_num++;
                }

                if(field_num > 0)
                {
                    continuous_string[(field_num - 1)] = '\0';
                    GUI_EditAnimStage = 0;
                }
            }
            else
            {
                valid_character = ST_FALSE;

                if(
                    ((input_key > '@') && (input_key < ']'))
                    ||
                    ((input_key > '-') && (input_key < ';'))
                    ||
                    (input_key == ' ')
                    ||
                    (input_key == '-')
                )
                {
                    valid_character = ST_TRUE;
                }
                else if((input_key > '`') && (input_key < '{'))
                {
                    valid_character = ST_TRUE;
                }

                if(valid_character == ST_TRUE)
                {
                    field_num = 0;
                    while(
                        (continuous_string[field_num] != '_')
                        &&
                        (continuous_string[field_num] != '\0')
                        &&
                        ((p_fields[active_input_field_number].max_characters - 1) > field_num)
                    )
                    {
                        field_num++;
                    }

                    if((p_fields[active_input_field_number].max_characters - 1) > field_num)
                    {
                        continuous_string[field_num] = input_key;
                        continuous_string[(field_num + 1)] = '\0';
                    }
                    else
                    {
                        continuous_string[field_num] = '\0';
                    }

                    GUI_EditCursorOn = ST_FALSE;
                    GUI_EditAnimStage = 0;
                }
            }
        }
    }

    emulated_pointer_x = Pointer_X();
    emulated_pointer_y = Pointer_Y();

    return 0;
}
// #endif


// WZD s36p22
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
// uint8_t Interpret_Keyboard_Input(int16_t * field_num)
/* CLAUDE */  /* REBUG */  /* asm @@Done: mov al, [bp+character] / cbw - the result is the sign-extended character */
int16_t Interpret_Keyboard_Input(int16_t * field_num)
{
    char temp_string[30] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t dirkey_character = 0;
    int8_t original_character = 0;
    int8_t character = 0;
    int16_t clear_hotkeys = 0;
    int16_t itr_fields = 0;
    int16_t dirkey_field_idx = 0;
    int16_t field_idx = 0;  // _SI_

    character = Read_Key();

    if(KD_prev_field_idx >= fields_count)
    {
        KD_prev_field_idx = 0;
    }

    field_idx = (KD_prev_field_idx + 1);

    original_character = character;

    UPPERCASE(character);

    /* CLAUDE */  LOG_DEBUG(LOG_CAT_GENERAL, "IKI: raw=%d(0x%02X '%c') upper=%d(0x%02X '%c') multi_active=%d",
        (int)original_character,
        (int)original_character,
        (original_character >= 32 && original_character < 127) ? original_character : '.', (int)character, (int)character, (character >= 32 && character < 127) ? character : '.',
        multi_hotkey_active_field);

    /*
        Active, check MultiHotKey Field
    */
    {
        if(multi_hotkey_active_field == 0)
        {
            field_idx = fields_count;
        }
        else
        {
            field_idx = multi_hotkey_active_field;
            /* CLAUDE */  LOG_DEBUG(LOG_CAT_GENERAL, "IKI: Active multi-hotkey field=%d, hotkey=%d(0x%02X '%c'), character=%d(0x%02X), char-95=%d(0x%02X)", field_idx, (int)p_fields[field_idx].hotkey, (int)p_fields[field_idx].hotkey, (p_fields[field_idx].hotkey >= 32 && p_fields[field_idx].hotkey < 127) ? (char)p_fields[field_idx].hotkey : '.', (int)character, (int)character, (int)(uint8_t)(character - 95), (int)(uint8_t)(character - 95));
            character -= 95;  // e.g., convert Alt-A to A
            if(character == p_fields[field_idx].hotkey)
            {
                /* CLAUDE */  LOG_DEBUG(LOG_CAT_GENERAL, "IKI: Active multi-hotkey MATCHED (char-95=%d == hotkey=%d)", (int)character, (int)p_fields[field_idx].hotkey);
                character += 95;
            }
            else
            {
                /* CLAUDE */  LOG_DEBUG(LOG_CAT_GENERAL, "IKI: Active multi-hotkey NO MATCH (char-95=%d != hotkey=%d)", (int)character, (int)p_fields[field_idx].hotkey);
                character += 95;
                field_idx = fields_count;
            }
        }
    }

    /*
        From Next, check MultiHotKey Fields
    */
    {
        if(field_idx == fields_count)
        {
            multi_hotkey_active_field = 0;
            field_idx = KD_prev_field_idx + 1;
            while(
                (
                    (p_fields[field_idx].hotkey != character)
                    ||
                    (p_fields[field_idx].type == ft_MultiHotKey)
                )
                &&
                (field_idx != fields_count)
            )
            {

                if(p_fields[field_idx].type == ft_MultiHotKey)
                {
                    /* CLAUDE */  LOG_DEBUG(LOG_CAT_GENERAL, "IKI: Scan field=%d type=MultiHotKey hotkey=%d(0x%02X '%c') char=%d(0x%02X) char-95=%d(0x%02X)",
                        field_idx,
                        (int)p_fields[field_idx].hotkey,
                        (int)p_fields[field_idx].hotkey,
                        (p_fields[field_idx].hotkey >= 32 && p_fields[field_idx].hotkey < 127) ? (char)p_fields[field_idx].hotkey : '.',
                        (int)character,
                        (int)character,
                        (int)(uint8_t)(character - 95),
                        (int)(uint8_t)(character - 95));
                    character -= 95;  // e.g., convert Alt-A to A
                    if(character == p_fields[field_idx].hotkey)
                    {
                        /* CLAUDE */  LOG_DEBUG(LOG_CAT_GENERAL, "IKI: Scan MATCHED field=%d", field_idx);
                        character += 95;
                        break;
                    }
                    else
                    {
                        character += 95;
                        /* CLAUDE */  /* REBUG */  /* asm loc_28035: add al, 95 / mov [bp+character], al / inc si / jmp loc_28041 - each branch has its own inc si */
                        field_idx++;
                    }
                }
                // field_idx++;
                /* CLAUDE */  /* REBUG */  /* asm loc_28040: inc si - the else of the ft_MultiHotKey test */
                else
                {
                    field_idx++;
                }
            }
        }
    }

    /*
        From First, check MultiHotKey Fields
    */
    {
        if(field_idx == fields_count)
        {
            field_idx = 1;
            while(((p_fields[field_idx].hotkey != character) || (p_fields[field_idx].type == ft_MultiHotKey)) && (field_idx != fields_count))
            {
                if(p_fields[field_idx].type == ft_MultiHotKey)
                {
                    character -= 95;  // e.g., convert Alt-A to A
                    if(character == p_fields[field_idx].hotkey)
                    {
                        character += 95;
                        break;
                    }
                    else
                    {
                        character += 95;
                        /* CLAUDE */  /* REBUG */  /* asm loc_280BD: add al, 95 / mov [bp+character], al / inc si / jmp loc_280C9 - each branch has its own inc si */
                        field_idx++;
                    }
                }
                // field_idx++;
                /* CLAUDE */  /* REBUG */  /* asm loc_280C8: inc si - the else of the ft_MultiHotKey test */
                else
                {
                    field_idx++;
                }
            }
        }
    }


    KD_prev_field_idx = field_idx;

    clear_hotkeys = ST_TRUE;


    // HERE:  
    if(field_idx < fields_count)
    {
        *field_num = field_idx;
        if((p_fields[field_idx].x1 < SCREEN_WIDTH) && (p_fields[field_idx].y1 < SCREEN_HEIGHT)
        )
        {
            // ~ Center_Emulated_Pointer_On_Field()
            Save_Mouse_State();
            emulated_pointer_x = (p_fields[field_idx].x1 + ((p_fields[field_idx].x2 - p_fields[field_idx].x1) / 2));
            emulated_pointer_y = (p_fields[field_idx].y1 + ((p_fields[field_idx].y2 - p_fields[field_idx].y1) / 2));
            if((emulated_pointer_x < SCREEN_WIDTH) && (emulated_pointer_y < SCREEN_HEIGHT))
            {
                Check_Mouse_Shape(emulated_pointer_x, emulated_pointer_y);
                pointer_offset = Get_Pointer_Offset();
                emulated_pointer_x = (emulated_pointer_x - pointer_offset);
                emulated_pointer_y = (emulated_pointer_y - pointer_offset);
                Set_Pointer_Position(emulated_pointer_x, emulated_pointer_y);
                Restore_Mouse_On_Page();
                Save_Mouse_On_Page(emulated_pointer_x, emulated_pointer_y);
                Draw_Mouse_On_Page(emulated_pointer_x, emulated_pointer_y);
            }
            Restore_Mouse_State();
        }

        if(p_fields[field_idx].type == ft_MultiHotKey)
        {
            stu_strcpy(temp_string, p_fields[field_idx].string);
            p_fields[field_idx].string_pos++;
            /* CLAUDE */  LOG_DEBUG(LOG_CAT_GENERAL, "IKI: MultiHotKey field=%d string=\"%s\" string_pos=%d/%d", field_idx, temp_string, p_fields[field_idx].string_pos, p_fields[field_idx].string_len);
            // if(p_fields[field_idx].string_pos < p_fields[field_idx].string_len)
            // {
            //     p_fields[field_idx].hotkey = temp_string[p_fields[field_idx].string_pos];
            //     multi_hotkey_active_field = field_idx;
            //     *field_num = 0;
            //     character = 0;
            //     /* CLAUDE */  clear_hotkeys = ST_FALSE;  /* Dasm: missing from reconstruction; don't reset multi-hotkey fields mid-sequence */
            // }
            // else
            // {
            //     p_fields[field_idx].string_pos = 0;
            //     p_fields[field_idx].hotkey = temp_string[0];
            //     multi_hotkey_active_field = 0;
            // }
            /* CLAUDE */  /* REBUG */  /* asm: cmp ax, [es:bx+s_Field.Param2] / jl loc_282CC - the fall-through (then) block is the full match, loc_282CC (else) is the partial match;
                                          both end at loc_28303: mov [bp+clear_hotkeys], e_ST_FALSE, so the full match also skips the reset loop below */
            if(p_fields[field_idx].string_pos >= p_fields[field_idx].string_len)
            {
                /* CLAUDE */  LOG_DEBUG(LOG_CAT_GENERAL, "IKI: FULL MATCH! Returning field=%d for \"%s\"", field_idx, temp_string);
                p_fields[field_idx].string_pos = 0;
                p_fields[field_idx].hotkey = temp_string[0];
                multi_hotkey_active_field = 0;
            }
            else
            {
                p_fields[field_idx].hotkey = temp_string[p_fields[field_idx].string_pos];
                multi_hotkey_active_field = field_idx;
                /* CLAUDE */  LOG_DEBUG(LOG_CAT_GENERAL, "IKI: Partial match, next hotkey='%c'(0x%02X), active_field=%d", (char)p_fields[field_idx].hotkey, (int)p_fields[field_idx].hotkey, multi_hotkey_active_field);
                *field_num = 0;
                character = 0;
            }
            clear_hotkeys = ST_FALSE;
        }
    }
    else  /* if(field_idx < fields_count) */
    {
        dirkey_field_idx = *field_num;
        dirkey_character = character;
        switch(dirkey_character)
        {
            // TODO  case KP_Left:  { dirkey_field_idx = Process_Direction_Key('L'); } break;
            // TODO  case KP_Right: { dirkey_field_idx = Process_Direction_Key('R'); } break;
            // TODO  case KP_Up:    { dirkey_field_idx = Process_Direction_Key('U'); } break;
            // TODO  case KP_Down:  { dirkey_field_idx = Process_Direction_Key('D'); } break;
            // TODO  case KP_PgUp:  { dirkey_field_idx = Process_Direction_Key('W'); } break;
            // TODO  case KP_Home:  { dirkey_field_idx = Process_Direction_Key('X'); } break;
            /* CLAUDE */  /* asm vt_DirKeys / jt_DirKeys: KP_PgUp -> dk_PgUp (mov ax, 'X'), KP_Home -> dk_Home (mov ax, 'W') - the two lines above have 'W' / 'X' swapped */
            // TODO  case KP_End:   { dirkey_field_idx = Process_Direction_Key('Y'); } break;
            // TODO  case KP_PgDn:  { dirkey_field_idx = Process_Direction_Key('Z'); } break;
            /* CLAUDE */  /* REBUG */  /* asm: mov cx, 8 / vt_DirKeys / jmp [cs:bx+10h] - bodies dk_Left, dk_Right, dk_Up, dk_Down, dk_Home, dk_PgUp, dk_End, dk_PgDn, each push 'x' / call Process_Direction_Key / mov [bp+dirkey_field_idx], ax */
            case KP_Left:  { dirkey_field_idx = Process_Direction_Key('L'); } break;
            case KP_Right: { dirkey_field_idx = Process_Direction_Key('R'); } break;
            case KP_Up:    { dirkey_field_idx = Process_Direction_Key('U'); } break;
            case KP_Down:  { dirkey_field_idx = Process_Direction_Key('D'); } break;
            case KP_Home:  { dirkey_field_idx = Process_Direction_Key('W'); } break;
            case KP_PgUp:  { dirkey_field_idx = Process_Direction_Key('X'); } break;
            case KP_End:   { dirkey_field_idx = Process_Direction_Key('Y'); } break;
            case KP_PgDn:  { dirkey_field_idx = Process_Direction_Key('Z'); } break;
        }
        *field_num = dirkey_field_idx;
    }


    if(clear_hotkeys != ST_FALSE)
    {
        for(itr_fields = 0; itr_fields < fields_count; itr_fields++)
        {
            if(p_fields[itr_fields].type == ft_MultiHotKey)
            {
                stu_strcpy(temp_string, p_fields[itr_fields].string);
                p_fields[itr_fields].string_pos = 0;
                p_fields[itr_fields].hotkey = temp_string[0];
            }
        }
    }


    if(p_fields[*field_num].hotkey != character)
    {
        character = original_character;
    }

    return character;
}
