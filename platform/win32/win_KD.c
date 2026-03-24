/**
 * win_KD.c — Win32 keyboard handling.
 *
 * Provides the VK-to-MOX_KEY translation table and keyboard buffer functions.
 * Game-level Keyboard_Status() and Read_Key() live in MoX/src/Keyboard.c.
 */

#include "../../platform/include/Platform.h"
#include "../../platform/include/Platform_Keys.h"
#include "win_PFL.h"

#include <string.h>



/* ========================================================================= */
/*  Keyboard Buffer                                                          */
/* ========================================================================= */

struct s_KEYBOARD_BUFFER platform_keyboard_buffer;



/* ========================================================================= */
/*  VK to MOX_KEY Translation Table (256 entries)                            */
/* ========================================================================= */

int VK_to_MOX_KEY[256];

void Build_Key_Xlat(void)
{
    memset(VK_to_MOX_KEY, 0, sizeof(VK_to_MOX_KEY));

    /* Letters */
    VK_to_MOX_KEY['A'] = MOX_KEY_a;
    VK_to_MOX_KEY['B'] = MOX_KEY_b;
    VK_to_MOX_KEY['C'] = MOX_KEY_c;
    VK_to_MOX_KEY['D'] = MOX_KEY_d;
    VK_to_MOX_KEY['E'] = MOX_KEY_e;
    VK_to_MOX_KEY['F'] = MOX_KEY_f;
    VK_to_MOX_KEY['G'] = MOX_KEY_g;
    VK_to_MOX_KEY['H'] = MOX_KEY_h;
    VK_to_MOX_KEY['I'] = MOX_KEY_i;
    VK_to_MOX_KEY['J'] = MOX_KEY_j;
    VK_to_MOX_KEY['K'] = MOX_KEY_k;
    VK_to_MOX_KEY['L'] = MOX_KEY_l;
    VK_to_MOX_KEY['M'] = MOX_KEY_m;
    VK_to_MOX_KEY['N'] = MOX_KEY_n;
    VK_to_MOX_KEY['O'] = MOX_KEY_o;
    VK_to_MOX_KEY['P'] = MOX_KEY_p;
    VK_to_MOX_KEY['Q'] = MOX_KEY_q;
    VK_to_MOX_KEY['R'] = MOX_KEY_r;
    VK_to_MOX_KEY['S'] = MOX_KEY_s;
    VK_to_MOX_KEY['T'] = MOX_KEY_t;
    VK_to_MOX_KEY['U'] = MOX_KEY_u;
    VK_to_MOX_KEY['V'] = MOX_KEY_v;
    VK_to_MOX_KEY['W'] = MOX_KEY_w;
    VK_to_MOX_KEY['X'] = MOX_KEY_x;
    VK_to_MOX_KEY['Y'] = MOX_KEY_y;
    VK_to_MOX_KEY['Z'] = MOX_KEY_z;

    /* Digits */
    /* VK_0..VK_9 are 0x30..0x39 — MOX_KEY for digits maps through character code, not MOX_KEY enum.
       We set them to MOX_KEY_SPACE as a sentinel so Platform_Keyboard_Buffer_Add_Key_Press fires.
       The actual digit character is set in WM_KEYDOWN via mox_character. */
    VK_to_MOX_KEY['0'] = MOX_KEY_SPACE;  /* mox_character='0' handles the rest */
    VK_to_MOX_KEY['1'] = MOX_KEY_SPACE;
    VK_to_MOX_KEY['2'] = MOX_KEY_SPACE;
    VK_to_MOX_KEY['3'] = MOX_KEY_SPACE;
    VK_to_MOX_KEY['4'] = MOX_KEY_SPACE;
    VK_to_MOX_KEY['5'] = MOX_KEY_SPACE;
    VK_to_MOX_KEY['6'] = MOX_KEY_SPACE;
    VK_to_MOX_KEY['7'] = MOX_KEY_SPACE;
    VK_to_MOX_KEY['8'] = MOX_KEY_SPACE;
    VK_to_MOX_KEY['9'] = MOX_KEY_SPACE;

    /* Navigation */
    VK_to_MOX_KEY[VK_BACK]   = MOX_KEY_BACKSPACE;
    VK_to_MOX_KEY[VK_TAB]    = MOX_KEY_TAB;
    VK_to_MOX_KEY[VK_RETURN] = MOX_KEY_ENTER;
    VK_to_MOX_KEY[VK_ESCAPE] = MOX_KEY_ESCAPE;
    VK_to_MOX_KEY[VK_SPACE]  = MOX_KEY_SPACE;
    VK_to_MOX_KEY[VK_DELETE] = MOX_KEY_DELETE;
    VK_to_MOX_KEY[VK_INSERT] = MOX_KEY_INSERT;
    VK_to_MOX_KEY[VK_HOME]   = MOX_KEY_HOME;
    VK_to_MOX_KEY[VK_END]    = MOX_KEY_END;
    VK_to_MOX_KEY[VK_PRIOR]  = MOX_KEY_PGUP;   /* VK_PRIOR = Page Up */
    VK_to_MOX_KEY[VK_NEXT]   = MOX_KEY_PGDN;   /* VK_NEXT = Page Down */

    /* Arrow keys */
    VK_to_MOX_KEY[VK_LEFT]   = MOX_KEY_LEFT;
    VK_to_MOX_KEY[VK_UP]     = MOX_KEY_UP;
    VK_to_MOX_KEY[VK_RIGHT]  = MOX_KEY_RIGHT;
    VK_to_MOX_KEY[VK_DOWN]   = MOX_KEY_DOWN;

    /* Numpad — directional mapping for MoM */
    VK_to_MOX_KEY[VK_NUMPAD1] = MOX_KEY_LEFTDOWN;
    VK_to_MOX_KEY[VK_NUMPAD2] = MOX_KEY_DOWN;
    VK_to_MOX_KEY[VK_NUMPAD3] = MOX_KEY_RIGHTDOWN;
    VK_to_MOX_KEY[VK_NUMPAD4] = MOX_KEY_LEFT;
    VK_to_MOX_KEY[VK_NUMPAD6] = MOX_KEY_RIGHT;
    VK_to_MOX_KEY[VK_NUMPAD7] = MOX_KEY_LEFTUP;
    VK_to_MOX_KEY[VK_NUMPAD8] = MOX_KEY_UP;
    VK_to_MOX_KEY[VK_NUMPAD9] = MOX_KEY_RIGHTUP;

    /* +/- keys */
    VK_to_MOX_KEY[VK_ADD]      = MOX_KEY_NUMPLUS;
    VK_to_MOX_KEY[VK_SUBTRACT] = MOX_KEY_NUMMINUS;
    VK_to_MOX_KEY[VK_OEM_PLUS]  = MOX_KEY_NUMPLUS;
    VK_to_MOX_KEY[VK_OEM_MINUS] = MOX_KEY_NUMMINUS;

    /* Function keys */
    VK_to_MOX_KEY[VK_F1]  = MOX_KEY_F1;
    VK_to_MOX_KEY[VK_F2]  = MOX_KEY_F2;
    VK_to_MOX_KEY[VK_F3]  = MOX_KEY_F3;
    VK_to_MOX_KEY[VK_F4]  = MOX_KEY_F4;
    VK_to_MOX_KEY[VK_F5]  = MOX_KEY_F5;
    VK_to_MOX_KEY[VK_F6]  = MOX_KEY_F6;
    VK_to_MOX_KEY[VK_F7]  = MOX_KEY_F7;
    VK_to_MOX_KEY[VK_F8]  = MOX_KEY_F8;
    VK_to_MOX_KEY[VK_F9]  = MOX_KEY_F9;
    VK_to_MOX_KEY[VK_F10] = MOX_KEY_F10;
    VK_to_MOX_KEY[VK_F11] = MOX_KEY_F11;
    VK_to_MOX_KEY[VK_F12] = MOX_KEY_F12;
}



/* ========================================================================= */
/*  Keyboard Buffer Functions                                                */
/* ========================================================================= */

void Platform_Keyboard_Buffer_Clear(void)
{
    platform_keyboard_buffer.key_write = 0;
    platform_keyboard_buffer.key_read = 0;
}

void Platform_Keyboard_Buffer_Add_Key_Press(int mox_key, uint32_t mox_mod, char mox_character)
{
    uint32_t packed_key = 0;

    packed_key = ((uint32_t)mox_key) | mox_mod | (((uint32_t)mox_character) << 8);

    if (mox_key == MOX_KEY_OVERRUN)
    {
        return;
    }

    key_pressed = ST_TRUE;

    platform_keyboard_buffer.packed_key[platform_keyboard_buffer.key_write] = packed_key;

    platform_keyboard_buffer.key_write = ((platform_keyboard_buffer.key_write + 1) % PLATFORM_KEYBOARD_BUFFER_LENGTH);
}

int Platform_Keyboard_Buffer_Pending_Count(void)
{
    return (platform_keyboard_buffer.key_write - platform_keyboard_buffer.key_read + PLATFORM_KEYBOARD_BUFFER_LENGTH) % PLATFORM_KEYBOARD_BUFFER_LENGTH;
}

uint32_t Platform_Keyboard_Buffer_Peek_Latest(void)
{
    if (platform_keyboard_buffer.key_write == platform_keyboard_buffer.key_read)
    {
        return 0;
    }
    return platform_keyboard_buffer.packed_key[(platform_keyboard_buffer.key_write - 1 + PLATFORM_KEYBOARD_BUFFER_LENGTH) % PLATFORM_KEYBOARD_BUFFER_LENGTH];
}
