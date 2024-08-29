
#include "MoX.H"



/*
typedef __int64 LONG_PTR, *PLONG_PTR;
typedef unsigned __int64 UINT_PTR, *PUINT_PTR;
typedef UINT_PTR            WPARAM;
typedef LONG_PTR            LPARAM;
WPARAM  wParam
LPARAM  lParam
~ uint64_t wParam
~ int64_t  lParam

WPARAM  wParam
LPARAM  lParam

Keyboard Input Notification
WM_KEYDOWN
Parameters
wParam
The virtual-key code of the nonsystem key. See Virtual-Key Codes.
lParam
The repeat count, scan code, extended-key flag, context code, previous key-state flag, and transition-state flag, as shown following.

uint32_t VKCode = wParam;  // the virtual-key code of the key

repeat count
scan code
extended-key flag
context code
previous key-state flag
transition-state flag

The previous key state (bit 30) can be used to determine whether the WM_KEYDOWN message indicates the first down transition or a repeated down transition.


When you receive a WM_KEYDOWN, WM_KEYUP, WM_SYSKEYDOWN, WM_SYSKEYUP message
    the lParam parameter contains the scancode in the 16 - 23 bits
    and the 24th bit indicates if the scancode is 1 or 2 bytes (extended).
    If the scancode is extended, you just need to add 0xE000 or 0xE100.



    WORD vkCode = LOWORD(wParam);                                 // virtual-key code
    WORD keyFlags = HIWORD(lParam);
    WORD scanCode = LOBYTE(keyFlags);                             // scan code




            int16_t key_shift = GetKeyState(VK_SHIFT);   // VK_SHIFT    0x10  SHIFT key
            int16_t key_ctrl = GetKeyState(VK_CONTROL);  // VK_CONTROL  0x11  CTRL key
            // "(The virtual-key code for the ALT key is named VK_MENU for historical reasons.)"
            int16_t key_alt = GetKeyState(VK_MENU);      // VK_MENU     0x12  ALT key
            if (GetKeyState(VK_LMENU) & 0x8000)
            {
                    // Left ALT key is down.
            }
            if (GetKeyState(VK_RMENU) & 0x8000)
            {
                    // Right ALT key is down.
            }


*/

// WZD s35p14
// keystroke available: {0,1} {ST_FALSE,ST_TRUE}
int16_t Keyboard_Status(void)
{
    int16_t keyboard_status;

    Save_Mouse_State();

    Pump_Events();

    if(key_pressed == ST_TRUE)
    {
        keyboard_status = ST_TRUE;
    }
    else
    {
        keyboard_status = ST_FALSE;
    }

    Restore_Mouse_State();

    return keyboard_status;
}


// WZD s35p15
// gets keystroke, clears status and keystroke
/*
    MoM gets scan code + character and translates
    0x01 + 0x1B => 0x001B
    0x4B + 0xE0 => 0x0001
    ...


*/
/*
    To add/edit a key;
        the SCCC and ST_KEY enums are in Input.H
        the VK_to_SCCS array is in Win32_Evnt.cpp
    
    * Reminder: Key Labels are (generally) capitalized, so (safely) assume "C" is 'c', "Shift-C" is 'C'
*/
uint8_t Read_Key(void)
{
    int8_t return_key;

    // Clear the Keyboard Status ~== INT 16,10
    key_pressed = ST_FALSE;

    // treat 1-byte scan code and 1-byte character code as a 2-byte value
    // short int / int16_t
    // ¿ array of 65536 values ?
    // ¿ loop thorugh array of stuct ?
    // ¿ procedurally generate a switch statement code block ?
    switch(scan_code_char_code)
    {

        case SCCC_F10: { return_key = ST_KEY_F10; } break;

        case SCCC_A: { return_key = 'a'; } break;
        case SCCC_B: { return_key = 'b'; } break;
        case SCCC_C: { return_key = 'c'; } break;
        case SCCC_D: { return_key = 'd'; } break;
        case SCCC_E: { return_key = 'e'; } break;
        case SCCC_F: { return_key = 'f'; } break;
        case SCCC_G: { return_key = 'g'; } break;
        case SCCC_H: { return_key = 'h'; } break;
        case SCCC_I: { return_key = 'i'; } break;
        case SCCC_J: { return_key = 'j'; } break;
        case SCCC_K: { return_key = 'k'; } break;
        case SCCC_L: { return_key = 'l'; } break;
        case SCCC_M: { return_key = 'm'; } break;
        case SCCC_N: { return_key = 'n'; } break;
        case SCCC_O: { return_key = 'o'; } break;
        case SCCC_P: { return_key = 'p'; } break;
        case SCCC_Q: { return_key = 'q'; } break;
        case SCCC_R: { return_key = 'r'; } break;
        case SCCC_S: { return_key = 's'; } break;
        case SCCC_T: { return_key = 't'; } break;
        case SCCC_U: { return_key = 'u'; } break;
        case SCCC_V: { return_key = 'v'; } break;
        case SCCC_W: { return_key = 'w'; } break;
        case SCCC_X: { return_key = 'x'; } break;
        case SCCC_Y: { return_key = 'y'; } break;
        case SCCC_Z: { return_key = 'z'; } break;

        case SCCC_Shift_A: { return_key = 'A'; } break;
        case SCCC_Shift_B: { return_key = 'B'; } break;
        case SCCC_Shift_C: { return_key = 'C'; } break;
        case SCCC_Shift_D: { return_key = 'D'; } break;
        case SCCC_Shift_E: { return_key = 'E'; } break;
        case SCCC_Shift_F: { return_key = 'F'; } break;
        case SCCC_Shift_G: { return_key = 'G'; } break;
        case SCCC_Shift_H: { return_key = 'H'; } break;
        case SCCC_Shift_I: { return_key = 'I'; } break;
        case SCCC_Shift_J: { return_key = 'J'; } break;
        case SCCC_Shift_K: { return_key = 'K'; } break;
        case SCCC_Shift_L: { return_key = 'L'; } break;
        case SCCC_Shift_M: { return_key = 'M'; } break;
        case SCCC_Shift_N: { return_key = 'N'; } break;
        case SCCC_Shift_O: { return_key = 'O'; } break;
        case SCCC_Shift_P: { return_key = 'P'; } break;
        case SCCC_Shift_Q: { return_key = 'Q'; } break;
        case SCCC_Shift_R: { return_key = 'R'; } break;
        case SCCC_Shift_S: { return_key = 'S'; } break;
        case SCCC_Shift_T: { return_key = 'T'; } break;
        case SCCC_Shift_U: { return_key = 'U'; } break;
        case SCCC_Shift_V: { return_key = 'V'; } break;
        case SCCC_Shift_W: { return_key = 'W'; } break;
        case SCCC_Shift_X: { return_key = 'X'; } break;
        case SCCC_Shift_Y: { return_key = 'Y'; } break;
        case SCCC_Shift_Z: { return_key = 'Z'; } break;

        case SCCC_1: { return_key = '1'; } break;
        case SCCC_2: { return_key = '2'; } break;
        case SCCC_3: { return_key = '3'; } break;
        case SCCC_4: { return_key = '4'; } break;
        case SCCC_5: { return_key = '5'; } break;
        case SCCC_6: { return_key = '6'; } break;
        case SCCC_7: { return_key = '7'; } break;
        case SCCC_8: { return_key = '8'; } break;
        case SCCC_9: { return_key = '9'; } break;
        case SCCC_0: { return_key = '0'; } break;

        case SCCC_MINUS:  { return_key = '-'; } break;
        case SCCC_EQUALS: { return_key = '='; } break;
        case SCCC_PLUS:   { return_key = '+'; } break;

        case SCCC_BACKSPACE: { return_key = ST_KEY_BACKSPACE; } break;
        case SCCC_DELETE:    { return_key = ST_KEY_DELETE;    } break;
        
        case SCCC_KP_2:     { return_key = ST_KEY_DOWN;       } break;
        case SCCC_DOWN:     { return_key = ST_KEY_DOWN;       } break;
        case SCCC_KP_1:     { return_key = ST_KEY_LEFTDOWN;   } break;
        case SCCC_DOWNLEFT: { return_key = ST_KEY_LEFTDOWN;   } break;

        case SCCC_ENTER:    { return_key = ST_KEY_ENTER;      } break;
        case SCCC_ESCAPE:   { return_key = ST_KEY_ESCAPE;     } break;

        case SCCC_KP_7:     { return_key = ST_KEY_LEFTUP; } break;
        case SCCC_UPLEFT:   { return_key = ST_KEY_LEFTUP; } break;
        // Ins         5200
        // Keypad 5        
        // Keypad *    372A
        // Keypad -    4A2D
        // Keypad +    4E2B
        // Keypad /    352F
        case SCCC_KP_4:         { return_key = ST_KEY_LEFT; } break;
        case SCCC_LEFT:         { return_key = ST_KEY_LEFT; } break;
        case SCCC_KP_3:         { return_key = ST_KEY_RIGHTDOWN; } break;
        case SCCC_DOWNRIGHT:    { return_key = ST_KEY_RIGHTDOWN; } break;
        case SCCC_KP_9:         { return_key = ST_KEY_RIGHTUP; } break;
        case SCCC_UPRIGHT:      { return_key = ST_KEY_RIGHTUP; } break;
        // PrtSc           
        case SCCC_KP_6:         { return_key = ST_KEY_RIGHT; } break;
        case SCCC_RIGHT:        { return_key = ST_KEY_RIGHT; } break;
        // SpaceBar    3920
        // Tab         0F09
        case SCCC_KP_8:         { return_key = ST_KEY_UP; } break;
        case SCCC_UP:           { return_key = ST_KEY_UP; } break;

        default: { return_key = ST_KEY_OVERRUN; }
    }

    return return_key;
}
