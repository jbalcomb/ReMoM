
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Keyboard_Status()\n", __FILE__, __LINE__);
#endif

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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Keyboard_Status() { keyboard_status = %d }\n", __FILE__, __LINE__, keyboard_status);
#endif

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
uint8_t Read_Key(void)
{
    int8_t return_key;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Read_Key()\n", __FILE__, __LINE__);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: scan_code_char_code: %d\n", __FILE__, __LINE__, scan_code_char_code);
#endif

    switch(scan_code_char_code)
    {
        case SCCC_ESC: { return_key = ST_KEY_ESCAPE; } break;

        case SCCC_A: { return_key = 'a'; } break;
        
        case SCCC_C:
        {
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: case SCCC_C:\n", __FILE__, __LINE__);
#endif
            return_key = 'c';
        } break;

        case SCCC_Q: { return_key = 'q'; } break;

        default: { return_key = ST_NULL; }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: return_key: %d\n", __FILE__, __LINE__, return_key);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Read_Key() { return_key = %d }\n", __FILE__, __LINE__, return_key);
#endif

    return return_key;
}
