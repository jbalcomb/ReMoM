
#include "MoM.H"
#include "MoX_DEF.H"

#include "Keyboard.H"
#include "Input.H"


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

    // TODO  MD_Draw_Disable()

    if(g_Key_Pressed == ST_TRUE)
    {
        keyboard_status = ST_TRUE;
    }
    else
    {
        keyboard_status = ST_FALSE;
    }

    // TODO  MD_Draw_Restore()

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
    g_Key_Pressed = ST_FALSE;

    // treat 1-byte scan code and 1-byte character code as a 2-byte value
    // short int / int16_t
    // ? array of 65536 values ?
    // ? loop thorugh array of stuct ?
    // ? procedurally generate a switch statement code block ?
    switch(scan_code_char_code)
    {
        case SCCC_ESC: { return_key = ST_KEY_ESCAPE; } break;

        case SCCC_A: { return_key = 'a'; } break;
        
        case SCCC_C: { return_key = 'c'; } break;

        case SCCC_Q: { return_key = 'q'; } break;

        default: { return_key = ST_KEY_OVERRUN; }
    }

    return return_key;
}
