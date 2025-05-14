
#include "sdl2_Audio.H"

#include "MOX_BITS.H"
#include "MOX_DEF.H"
#include "SOUND.H"

#include "STU/STU_DBG.H"
#include "STU/STU_SND.H"

// #include "SDL_mixer.h"
#include "C:\devellib\SDL2_mixer-2.8.1\include\SDL_mixer.h"



// BLK_SIZE        equ 32768               ;max. block size for INT 66 calls
#define BLK_SIZE 32768
// 32768 * 2 bytes * 2 channels = 131072  128KB
#define WAV_BUFFER_SIZE (BLK_SIZE * 2 * 2)
// uint8_t wav_buffer[WAV_BUFFER_SIZE];

#define LEN_VOC_HDR     26
#define LEN_WAV_HDR     44

const uint8_t cl_voc_id[20] = { 'C','r','e','a','t','i','v','e',' ','V','o','i','c','e',' ','F','i','l','e','\x1A' };
// #define HDR_VOC         ((const uint8_t *)"Creative Voice File\x1A")
#define HDR_VOC_LEN     20
#define HDR_VOC_LENGTH     0x001A  // 26 bytes
#define HDR_VOC_VERSION    0x010A  // v1.10
#define HDR_VOC_CHECK      0x1129

#define LEN_LBX_SND_HDR 16
#define LBX_ENTRY_TYPE_SOUND        0xDEAF
// #define LBX_ENTRY_TYPE_SOUND_XMI    0xDEAF0001
// #define LBX_ENTRY_TYPE_SOUND_VOC    0xDEAF0002
#define LBX_ENTRY_TYPE_SOUND_XMI    0x0001
#define LBX_ENTRY_TYPE_SOUND_VOC    0x0002

// C:\STU\devel\SoundBlaster\Sound Blaster Developer Kit 2.00 (1991) (5.25-1.2mb)\disk01\MSC\INCLUDE\SBCVOICE.H
//      /*   creative voice file header    */
// 
//      typedef   struct
//      {
//       char         id[20] ;
//       unsigned     voice_offset ;
//       unsigned     version ;
//       unsigned     check_code ;
//      } VOCHDR ;
#pragma pack(push)
#pragma pack(2)
typedef struct
{
    char     id[20];
    uint16_t voice_offset;
    uint16_t version;
    uint16_t check_code;
} VOCHDR;
#pragma pack(pop)



#define MAX_WAV_BUFFER_IDX 5
uint8_t * wav_buffers[MAX_WAV_BUFFER_IDX] = { NULL, NULL, NULL, NULL, NULL };
int16_t wav_buffer_idx = 0;



void sdl2_Audio_Init(void)
{

    // // // // // Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);  // Expands to: AUDIO_S16SYS
    // // // // Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048);
    // // // Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 1024);
    // // Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 512);
    // // Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 8192);  // workie?
    // Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 1024);
    if(Mix_Init((MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID | MIX_INIT_OPUS | MIX_INIT_WAVPACK)) == 0)
    {
        dbg_prn("ERROR:  Mix_Init():  %s\n", Mix_GetError());
    }
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
    {
        dbg_prn("ERROR:  Mix_OpenAudio():  %s\n", Mix_GetError());
    }

}

void sdl2_Audio_Deinit(void)
{
    int itr = 0;

    Mix_CloseAudio();

    Mix_Quit();

    for (itr = 0; itr < MAX_WAV_BUFFER_IDX; itr++)
    {
        if(wav_buffers[itr] != NULL)
        {
            free(wav_buffers[itr]);
        }
    }

}

/*

// DOMSDOS  Play_Sound__WIP(digi_sound_buffer3);
sdl2_Play_Sound(digi_sound_buffer3, sound_buffer_size3);
digi_sound_buffer3 = LBX_Reload_Next(soundfx_lbx_file__MGC_ovr058, SFX_ATK_LightningBolt, _screen_seg);
sound_buffer_size3 = lbxload_entry_length;
Convert_VOC_To_WAV(digi_sound_buffer3, sound_buffer_size3, &wav_sound_buffer3, &wav_sound_buffer_size3);
wav_sound_chunk3 = Mix_LoadWAV_RW(SDL_RWFromMem(wav_sound_buffer3, wav_sound_buffer_size3), 0);
Mix_PlayChannel(0, wav_sound_chunk3, 0);

*/
int16_t sdl2_Play_Sound(void* sound_buffer, uint32_t sound_buffer_size)
{
    int16_t lbx_sound_type = 0;
    // char error[4096] = { 0 };
    uint8_t* wav_sound_buffer;
    uint32_t wav_sound_buffer_size;
    SDL_RWops * sdl2_rw_ops = NULL;
    Mix_Chunk * sdl2_audio_data_chunk = NULL;
    int sdl2_play_channel_result = 0;

    if(GET_2B_OFS(sound_buffer, 0) != SNDMAGSIG)
    {
        return 1;
    }

    // printf("GET_2B_OFS(sound_buffer, 0): %04X\n", GET_2B_OFS(sound_buffer, 0));  // DEAF
    // printf("GET_2B_OFS(sound_buffer, 2): %04X\n", GET_2B_OFS(sound_buffer, 2));  // 0002
    // printf("GET_2B_OFS(sound_buffer, 4): %04X\n", GET_2B_OFS(sound_buffer, 4));  // 0000
    // printf("GET_2B_OFS(sound_buffer, 6): %04X\n", GET_2B_OFS(sound_buffer, 6));  // 0000

    lbx_sound_type = GET_2B_OFS(sound_buffer, 2);
    dbg_prn("lbx_sound_type: %d\n", lbx_sound_type);

    switch(lbx_sound_type)
    {
        case 1:  /* XMI file */
        {
            dbg_prn("LBX Sound Entry is XMI\n");

        }
        case 2:  /* VOC file */
        {
            dbg_prn("LBX Sound Entry is XMI\n");
            Convert_VOC_To_WAV(sound_buffer, sound_buffer_size, &wav_sound_buffer, &wav_sound_buffer_size);
            sdl2_rw_ops = SDL_RWFromMem(wav_sound_buffer, wav_sound_buffer_size);
            if(sdl2_rw_ops == NULL)
            {
                dbg_prn("ERROR:  SDL_RWFromMem()  %s\n", SDL_GetError());
            }
            sdl2_audio_data_chunk = Mix_LoadWAV_RW(sdl2_rw_ops, 0);
            if(sdl2_audio_data_chunk == NULL)
            {
                dbg_prn("ERROR:  Mix_LoadWAV_RW()  %s\n", SDL_GetError());
            }
            sdl2_play_channel_result = Mix_PlayChannel(0, sdl2_audio_data_chunk, 0);
            if(sdl2_play_channel_result == -1)
            {
                dbg_prn("ERROR:  Mix_PlayChannel()  %s\n", SDL_GetError());
            }
        } break;
        default:
        {
            return 1;
        } break;
    }

}

void sdl2_Play_Sound_WAV(Mix_Chunk * wav_sound_chunk)
{
    int result;

    result = Mix_PlayChannel(0, wav_sound_chunk, 0);

    if(result == -1)
    {
        dbg_prn("ERROR:  SOUND:  Mix_PlayChannel()  %s\n", SDL_GetError());
    }

}


/*
1oom:
    fmt_sfx.c
        fmt_sfx_convert()
        fmt_sfx_detect()
        fmt_sfx_convert_voc()

*/

/*

voc_buf
    VOC LBX Entry, via 'LBX Load'
voc_len
    VOC LBX Entry Size, from lbxload_entry_length, after 'LBX Load'
out_wav_buf
    , for Mix_LoadWAV_RW(SDL_RWFromMem())
out_wav_len
    , for Mix_LoadWAV_RW(SDL_RWFromMem())

NOTE: initial values for wav_header are hard-coded for SOUNDFX.LBX, entry 0  (SLX19.VOC)
*/
int Convert_VOC_To_WAV(const uint8_t* voc_buf, uint32_t voc_len, int16_t** out_wav_buf, uint32_t* out_wav_len)
{
    /* WAV */
    // out_wav_buf
    // out_wav_len
    uint8_t* wav_rvr = NULL;
    int16_t sample_16bit_signed = 0;
    uint8_t wav_header[LEN_WAV_HDR] = { 'R', 'I', 'F', 'F', 0, 0, 0, 0, 'W', 'A', 'V', 'E', 'f', 'm', 't', ' ', 16, 0, 0, 0, 1, 0, 1, 0, 0x94, 0x15, 0, 0, 0x94, 0x15, 0, 0, 1, 0, 8, 0, 'd', 'a', 't', 'a', 0, 0, 0, 0 };
    uint32_t wav_samples_per_second = 0;  // frequency, in hertz
    uint32_t wav_bytes_per_second = 0;  // samples per second * bytes per sample * channel count
    uint32_t wav_sample_count = 0;
    uint16_t wav_sample_size = 0;
    uint16_t wav_channel_count = 0;
    uint16_t wav_sample_frame_size = 0;
    uint32_t wav_data_size = 0;
    /* VOC */
    // voc_buf
    // voc_len
    const uint8_t* voc_rvr = NULL;
    uint8_t sample_8bit_unsigned = 0;
    uint8_t voc_block_type;
    uint8_t voc_block_sample_rate = 0;
    uint8_t voc_block_codec_id = 0;
    uint32_t voc_block_size = 0;
    uint32_t voc_block_frequency = 0;
    // Chunk Body Size             4       32-bit unsigned integer
    // Format Tag                  2       16-bit unsigned integer
    // Number of Channels          2       16-bit unsigned integer     (1: mono, 2: stereo, ..., 6: 5.1 surround sound)
    // Samples per second          4       32-bit unsigned integer     (sample rate – number of sample frames that occur each second)
    // Average Bytes per Second    4       32-bit unsigned integer     (number of bytes required for one second of audio data - sample rate * sample frame size)
    // Block align                 2       16-bit unsigned integer     (sample frame size - size in bytes of a unit of sample data) (e.g., 16-bit, 2-channel (16÷8)×2=4)
    // Bits per sample             2       16-bit unsigned integer     (the size in bits of a single sample (not a sample frame, but a single sample))
    uint8_t* wav_buffer = NULL;

    if(wav_buffer_idx == MAX_WAV_BUFFER_IDX)
    {
        dbg_prn("(wav_buffer_idx == MAX_WAV_BUFFER_IDX)\n");
        // __debugbreak();
        wav_buffer_idx = 0;
    }
    if(wav_buffers[wav_buffer_idx] != NULL)
    {
        dbg_prn("(wav_buffers[wav_buffer_idx] != NULL)\n");
        free(wav_buffers[wav_buffer_idx]);
    }
    wav_buffers[wav_buffer_idx] = malloc(WAV_BUFFER_SIZE);
    if(wav_buffers[wav_buffer_idx] == NULL)
    {
        dbg_prn("FATAL: malloc()\n");
        __debugbreak();
    }
    wav_buffer = wav_buffers[wav_buffer_idx];
    wav_buffer_idx += 1;

    memset(wav_buffer, 0, WAV_BUFFER_SIZE);

    memcpy(wav_buffer, wav_header, sizeof(wav_header));


    voc_rvr = voc_buf;
    wav_rvr = &wav_buffer[LEN_WAV_HDR];

    voc_rvr += LEN_LBX_SND_HDR;
    voc_len -= LEN_LBX_SND_HDR;

    voc_rvr += HDR_VOC_LENGTH;
    voc_len -= HDR_VOC_LENGTH;

    wav_sample_count = 0;

    wav_sample_size = 1;  // 1 byte per sample - 8-bit, mono
    wav_channel_count = 1;  // 1 channel AKA mono
    wav_sample_frame_size = wav_sample_size * wav_channel_count;  // 1 channel * 1 byte per sample

    // voc_len = 856-16-26;
    // voc_rvr = &voc_buf[16+26];
    while (voc_len)
    {
        printf("voc_len: %u\n", voc_len);

        voc_block_type = voc_rvr[0];
        printf("voc_block_type: %u\n", voc_block_type);

        switch (voc_block_type)
        {
        case 0:  /* VOC_TYPE_EOF */
        {
            printf("VOC_TYPE_EOF\n");
            voc_len -= 1;  // -1 for block type
        } break;
        case 1:  /* VOC_TYPE_VOICE_DATA */
        {
            printf("VOC_TYPE_VOICE_DATA\n");
            voc_block_sample_rate = voc_rvr[4];
            printf("voc_block_sample_rate: %u\n", voc_block_sample_rate);
            voc_block_codec_id = voc_rvr[5];
            printf("voc_block_codec_id: %u\n", voc_block_codec_id);
            voc_block_size = voc_rvr[3] << 16;
            voc_block_size |= voc_rvr[2] << 8;
            voc_block_size |= voc_rvr[1];
            voc_block_size -= 2;  // -2 for sample rate and codec id
            printf("voc_block_size: %u\n", voc_block_size);
            voc_len -= 6;  // -6 for block type, block size, sample rate, and codec id
            voc_rvr += 6;  // +6 for block type, block size, sample rate, and codec id
            voc_block_frequency = 1000000 / (256 - voc_block_sample_rate);  // sample rate in hertz
            printf("voc_block_frequency: %u\n", voc_block_frequency);
            // wav_frequency = voc_block_frequency;
            if (voc_block_codec_id != 0)
            {
                __debugbreak();
            }
            wav_sample_count += voc_block_size;
            voc_len -= voc_block_size;
            while (voc_block_size--)
            {
                sample_8bit_unsigned = *voc_rvr++;
                *wav_rvr++ = sample_8bit_unsigned;
            }
        } break;
        default:
        {
            __debugbreak();
        } break;
        }
    }

    printf("wav_sample_count: %u\n", wav_sample_count);

    wav_data_size = wav_sample_count * wav_sample_frame_size;

    printf("wav_data_size: %u\n", wav_data_size);

    wav_samples_per_second = 1000000 / (256 - voc_block_sample_rate);  // sample rate (frequency, in hertz)

    printf("wav_samples_per_second: %u\n", wav_samples_per_second);

    wav_bytes_per_second = (wav_samples_per_second * wav_sample_frame_size * wav_channel_count);

    printf("wav_bytes_per_second: %u\n", wav_bytes_per_second);

    // // wav_header[0x04] = ((44 - 8) + wav_data_size);  /* File size (integer)  (total file size - 8 bytes), in bytes (32-bit integer) */
    // wav_header[0x04] = ((((44 - 8) + wav_data_size) >>  0) & 0x000000FF);
    // wav_header[0x05] = ((((44 - 8) + wav_data_size) >>  8) & 0x000000FF);
    // wav_header[0x06] = ((((44 - 8) + wav_data_size) >> 16) & 0x000000FF);
    // wav_header[0x07] = ((((44 - 8) + wav_data_size) >> 24) & 0x000000FF);
    // // wav_header[0x18] = audiorate);                            /* Sample Rate - 32 bit integer. Common values are 44100 (CD), 48000 (DAT). Sample Rate = Number of Samples per second, or Hertz */
    // // wav_header[0x1C] = ((audiorate * 16 * 2) / 8));           /* (Sample Rate * BitsPerSample * Channels) / 8 */
    // // wav_header[0x28] = wav_data_size;                        /* File size (data)  Size of the data section */
    // wav_header[0x28] = ((wav_data_size >>  0) & 0x000000FF);
    // wav_header[0x29] = ((wav_data_size >>  8) & 0x000000FF);
    // wav_header[0x2A] = ((wav_data_size >> 16) & 0x000000FF);
    // wav_header[0x2B] = ((wav_data_size >> 24) & 0x000000FF);

    wav_buffer[0x04] = ((((LEN_WAV_HDR - 8) + wav_data_size) >> 0) & 0x000000FF);
    wav_buffer[0x05] = ((((LEN_WAV_HDR - 8) + wav_data_size) >> 8) & 0x000000FF);
    wav_buffer[0x06] = ((((LEN_WAV_HDR - 8) + wav_data_size) >> 16) & 0x000000FF);
    wav_buffer[0x07] = ((((LEN_WAV_HDR - 8) + wav_data_size) >> 24) & 0x000000FF);
    // Format Chunk :: Samples per second
    wav_buffer[0x18] = ((wav_samples_per_second >> 0) & 0x000000FF);
    wav_buffer[0x19] = ((wav_samples_per_second >> 8) & 0x000000FF);
    wav_buffer[0x1A] = ((wav_samples_per_second >> 16) & 0x000000FF);
    wav_buffer[0x1B] = ((wav_samples_per_second >> 24) & 0x000000FF);
    // Format Chunk :: Average Bytes per Second
    wav_buffer[0x1C] = ((wav_bytes_per_second >> 0) & 0x000000FF);
    wav_buffer[0x1D] = ((wav_bytes_per_second >> 8) & 0x000000FF);
    wav_buffer[0x1E] = ((wav_bytes_per_second >> 16) & 0x000000FF);
    wav_buffer[0x1F] = ((wav_bytes_per_second >> 24) & 0x000000FF);
    // Data Chunk :: Chunk Body Size
    wav_buffer[0x28] = ((wav_data_size >> 0) & 0x000000FF);
    wav_buffer[0x29] = ((wav_data_size >> 8) & 0x000000FF);
    wav_buffer[0x2A] = ((wav_data_size >> 16) & 0x000000FF);
    wav_buffer[0x2B] = ((wav_data_size >> 24) & 0x000000FF);

    // memcpy(wav_buffer[sizeof(wav_header)], wav_data, wav_data_size);

    *out_wav_buf = &wav_buffer[0];
    *out_wav_len = (LEN_WAV_HDR + wav_data_size);

    return 0;
}
