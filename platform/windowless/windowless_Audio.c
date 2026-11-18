/**
 * windowless_Audio.c — Audio stub (windowless).
 *
 * No audio output in windowless mode.
 */

#include "../../platform/include/Platform.h"



int16_t Platform_Audio_Play_Sound(void *sound_buffer, uint32_t sound_buffer_size)
{
    (void)sound_buffer;
    (void)sound_buffer_size;
    return -1;
}
