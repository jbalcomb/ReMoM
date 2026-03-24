#ifndef SDL3_AUDIO_H
#define SDL3_AUDIO_H

#include <stdint.h>

#ifndef NO_SOUND_LIBRARY
#include <SDL3_mixer/SDL_mixer.h>
#endif



#ifdef __cplusplus
extern "C" {
#endif


void sdl3_Audio_Init(void);
void sdl3_Audio_Deinit(void);

// int Convert_VOC_To_WAV(const uint8_t* voc_buf, uint32_t voc_len, int16_t** out_wav_buf, uint32_t* out_wav_len);
// void sdl3_Audio_Init(void);
// void sdl3_Audio_Deinit(void);
// int16_t Play_Sound(void* sound_buffer, uint32_t sound_buffer_size);
// void sdl3_Play_Sound_WAV(Mix_Chunk * wav_sound_chunk);



#ifdef __cplusplus
}
#endif

#endif  /* SDL3_AUDIO_H */
