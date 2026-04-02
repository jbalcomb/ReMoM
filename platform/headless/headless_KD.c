/**
 * headless_KD.c — Headless keyboard handling.
 *
 * Provides the keyboard buffer functions without any OS key translation.
 */

#include "../../platform/include/Platform.h"
#include "../../platform/include/Platform_Keys.h"



/* ========================================================================= */
/*  Keyboard Buffer                                                          */
/* ========================================================================= */

struct s_KEYBOARD_BUFFER platform_keyboard_buffer;



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
