
#include "sdl2_Audio.H"

#include "MOX_BITS.H"
#include "MOX_DEF.H"
#include "SOUND.H"

#include "STU/STU_DBG.H"
#include "STU/STU_SND.H"

// #include "SDL_mixer.h"
#include "C:\devellib\SDL2_mixer-2.8.1\include\SDL_mixer.h"

#include <stdbool.h>

// SOUND.C
// WZD dseg:82AC
extern int16_t audio_initialized;



// C:\STU\developp\1oom\src\ui\classic\uidefs.h
#define NUM_MUSICS  0x28

// struct ui_data_s { ... }; extern struct ui_data_s ui_data;
uint8_t * ui_data_mus;
int ui_data_music_i;


// C:\STU\developp\1oom\src\fmt_mus.h
typedef enum {
    MUS_TYPE_UNKNOWN = 0,
    MUS_TYPE_LBXXMID,
    MUS_TYPE_MIDI,
    MUS_TYPE_WAV,
    MUS_TYPE_OGG,
    MUS_TYPE_FLAC
} mus_type_t;

// C:\STU\developp\1oom\src\options.c
bool opt_music_enabled = true;
int opt_music_volume = 64;

// C:\STU\developp\1oom\src\hw\sdl\hwsdl_audio.c
struct mus_s {
    mus_type_t type;
    Mix_Music *music;
    Mix_MusicType sdlmtype;  // SDL Mixer enum Mix_MusicType { MUS_MID = 4 }
    uint8_t *buf;   /* WAV music files need the data to be kept */
    bool loops;
};
static int mus_num = 0;
// static struct mus_s * mustbl[1] = { NULL };
// static struct mus_s * mustbl[1] = { NULL };
static struct mus_s mustbl;
// static int mus_playing;

// C:\STU\developp\1oom\src\hw\sdl\hwsdl_audio.c
void hw_audio_music_release(int mus_index);
void hw_audio_music_play(int mus_index);
void hw_audio_music_fadeout(void);
void hw_audio_music_stop(void);
bool hw_audio_music_volume(int volume);



// C:\STU\developp\1oom\src\fmt_mus.c
/* -------------------------------------------------------------------------- */

/* #define DEBUGLEVEL_FMTMUS   4 */

#define HDR_MIDI_LEN    0x16

typedef struct noteoff_s {
    uint32_t t;     /* time of event */
    int16_t next;    /* next event, sorted by time */
    uint8_t ch;     /* 0 if unused, otherwise 0x8Z whe Z is channel */
    uint8_t note;
} noteoff_t;

/* MOO1 has max. 15 pending noteoffs */
#define NOTEOFFBUFSIZE  32

struct noteoffs_s {
    noteoff_t tbl[NOTEOFFBUFSIZE];  /* noteoff events */
    int16_t top;    /* noteoff with nearest time */
    int16_t pos;    /* position of (likely) free tbl entry */
    int8_t num; /* number of pending events */
    int8_t max; /* max. number of pending events */
};

#define XMID_TICKSPERQ  60
/*#define XMID_USE_BANKS*/

/* -------------------------------------------------------------------------- */

bool fmt_mus_convert_xmid(const uint8_t *data_in, uint32_t len_in, uint8_t **data_out_ptr, uint32_t *len_out_ptr, bool *tune_loops);





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

    // Set the volume of the music to 50% (half the MAX_VOLUME)
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    // Later, elsewhere, ...
    // // Set the volume of the sound effect to 75%
    // Mix_VolumeChunk(sdl2_mix_chunk, MIX_MAX_VOLUME * 0.75);

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



// AKA Play_Sound__STUB()
/*

// DOMSDOS  Play_Sound__WIP(digi_sound_buffer3);
sdl2_Play_Sound(digi_sound_buffer3, sound_buffer_size3);
digi_sound_buffer3 = LBX_Reload_Next(soundfx_lbx_file__MGC_ovr058, SFX_ATK_LightningBolt, _screen_seg);
sound_buffer_size3 = lbxload_entry_length;
Convert_VOC_To_WAV(digi_sound_buffer3, sound_buffer_size3, &wav_sound_buffer3, &wav_sound_buffer_size3);
wav_sound_chunk3 = Mix_LoadWAV_RW(SDL_RWFromMem(wav_sound_buffer3, wav_sound_buffer_size3), 0);
Mix_PlayChannel(0, wav_sound_chunk3, 0);

*/
int16_t sdl2_Play_Sound__WIP(void* sound_buffer, uint32_t sound_buffer_size)
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

    printf("GET_2B_OFS(sound_buffer, 0): %04X\n", GET_2B_OFS(sound_buffer, 0));  // DEAF
    printf("GET_2B_OFS(sound_buffer, 2): %04X\n", GET_2B_OFS(sound_buffer, 2));  // 0001 || 0002
    printf("GET_2B_OFS(sound_buffer, 4): %04X\n", GET_2B_OFS(sound_buffer, 4));  // 0000
    printf("GET_2B_OFS(sound_buffer, 6): %04X\n", GET_2B_OFS(sound_buffer, 6));  // 0000

    lbx_sound_type = GET_2B_OFS(sound_buffer, 2);
    dbg_prn("lbx_sound_type: %d\n", lbx_sound_type);

    switch(lbx_sound_type)
    {
        case 1:  /* XMI file */
        {
            dbg_prn("LBX Sound Entry is XMI\n");
            hw_audio_music_stop();
            // hw_audio_music_init(0, sound_buffer, sound_buffer_size);
            const uint8_t* data = NULL;
            uint8_t* xmi_sound_buffer = NULL;
            uint32_t xmi_sound_buffer_size = 0;
            struct mus_s* m;
            // m = &mustbl[0];
            m = &mustbl;
            fmt_mus_convert_xmid(sound_buffer, sound_buffer_size, &xmi_sound_buffer, &xmi_sound_buffer_size, &m->loops);
            SDL_RWops* sdl2_rw_ops = SDL_RWFromConstMem(xmi_sound_buffer, xmi_sound_buffer_size);
            m->music = Mix_LoadMUSType_RW(sdl2_rw_ops, m->sdlmtype, 0);
            SDL_RWclose(sdl2_rw_ops);
            free(xmi_sound_buffer);
            // ...
            ui_data_mus = sound_buffer;
            ui_data_music_i = 0;
            hw_audio_music_play(0);

        } break;
        case 2:  /* VOC file */
        {
            dbg_prn("LBX Sound Entry is VOC\n");
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
            Mix_VolumeChunk(sdl2_audio_data_chunk, MIX_MAX_VOLUME * 0.75);
            sdl2_play_channel_result = Mix_PlayChannel(0, sdl2_audio_data_chunk, 0);
            if(sdl2_play_channel_result == -1)
            {
                dbg_prn("ERROR:  Mix_PlayChannel()  %s\n", SDL_GetError());
            }
        } break;
        default:
        {
            __debugbreak();
            Audio_Error__STUB(SND_Invalid_File);
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
int Convert_VOC_To_WAV(const uint8_t * voc_buf, uint32_t voc_len, uint8_t ** out_wav_buf, uint32_t * out_wav_len)
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

    // digi_sound_buffer1 = LBX_Reload_Next(soundfx_lbx_file__MGC_ovr058, SFX_IntroT3, _screen_seg);
    // // TORAN3M1    intro speech
    // voc_len: 80602
    if(voc_len >= (128 * 1024))
    {
        printf("voc_len: %u\n", voc_len);
        dbg_prn("voc_len: %u\n", voc_len);
        __debugbreak();
        // Pssst... is your size var for lbxload_entry_length a uint32_t?
    }

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
    while(voc_len)
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

// C:\STU\developp\1oom\src\ui\classic\uisound.c
// ...moved into sdl2_Play_Sound()
// void ui_sound_play_music(int musici)
// {
//     if ((musici < 0) || (musici >= NUM_MUSICS)) {
//         // log_error("uisound: invalid music num %i\n", musici);
//         dbg_prn("uisound: invalid music num %i\n", musici);
//         return;
//     }
//     hw_audio_music_stop();
//     if (ui_data_music_i != musici) {
//         uint8_t *n;
//         uint32_t len;
//         n = lbxfile_item_get_with_len(LBXFILE_MUSIC, musici, &len);
//         if (ui_data_mus) {
//             hw_audio_music_release(0);
//             lbxfile_item_release(LBXFILE_MUSIC, ui_data_mus);
//         }
//         hw_audio_music_init(0, n, len);
//         ui_data_mus = n;
//         ui_data_music_i = musici;
//     }
//     hw_audio_music_play(0);
// }

// C:\STU\developp\1oom\src\ui\classic\uisound.c
void ui_sound_stop_music(void)
{
    hw_audio_music_stop();
}



// C:\STU\developp\1oom\src\hw\sdl\hwsdl_audio.c
// ...moved into sdl2_Play_Sound()
// int hw_audio_music_init(int mus_index, const uint8_t *data_in, uint32_t len_in)
// {
//     const uint8_t *data = NULL;
//     uint8_t *buf = NULL;
//     uint32_t len = 0;
//     struct mus_s *m;
// 
//     if (!audio_initialized) {
//         return 0;
//     }
// 
//     if (mus_index >= mus_num) {
//         int old_mus_num = mus_num;
//         mus_num = (mus_index + 1);
//         // mustbl = lib_realloc(mustbl, mus_num * sizeof(struct mus_s));
//         mustbl = realloc(mustbl, mus_num * sizeof(struct mus_s));
//         for (int i = old_mus_num; i < mus_num; ++i) {
//             mustbl[i].type = MUS_TYPE_UNKNOWN;
//             mustbl[i].music = NULL;
//             mustbl[i].buf = NULL;
//         }
//     }
// 
//     m = &mustbl[mus_index];
// 
//     if (m->type != MUS_TYPE_UNKNOWN) {
//         hw_audio_music_release(mus_index);
//     }
// 
//     dbg_prn("GET_2B_OFS(data, 0): %04X\n", GET_2B_OFS(data, 0));
//     dbg_prn("GET_2B_OFS(data, 2): %04X\n", GET_2B_OFS(data, 2));
//     if(
//         (GET_2B_OFS(data, 0) != LBX_ENTRY_TYPE_SOUND)
//         ||
//         (GET_2B_OFS(data, 2) != LBX_ENTRY_TYPE_SOUND_XMI)
//     )
//     {
//         __debugbreak();
//     }
//     
//     fmt_mus_convert_xmid(data_in, len_in, &buf, &len, &m->loops);
//     data = buf;
// 
//     // m->sdlmtype = mus_type_to_sdlm(m->type);
//     m->sdlmtype = MUS_MID;
// 
//     if (m->type == MUS_TYPE_UNKNOWN) {
//         // log_error("SDLA: failed to init music %i\n", mus_index);
//         return -1;
//     }
// 
//     {
//         SDL_RWops *rw = SDL_RWFromConstMem(data, len);
//         // m->music = Mix_LoadMUSType_RW(rw, m->sdlmtype, 0);
//         m->music = Mix_LoadMUSType_RW(rw, MUS_MID, 0);  // NOTE:  need to call Mix_FreeMusic(m->music) later (probably, call  Mix_HaltMusic() first)
//         // if(m->music == NULL) { ... }
//         SDL_RWclose(rw);
//     }
// 
//     lib_free(buf);
// 
//     if(!m->music)
//     {
//         // log_error("SDLA: Mix_LoadMUSType_RW failed on music %i (type %i): %s\n", mus_index, m->type, Mix_GetError());
//         m->type = MUS_TYPE_UNKNOWN;
//         m->sdlmtype = MUS_NONE;
//         return -1;
//     }
// 
//     return 0;
// }

// C:\STU\developp\1oom\src\hw\sdl\hwsdl_audio.c
void hw_audio_music_release(int mus_index)
{
    hw_audio_music_stop();
    if(mustbl.music != NULL)
    {
        Mix_FreeMusic(mustbl.music);
        mustbl.music = NULL;
    }
    mustbl.type = MUS_TYPE_UNKNOWN;
    mustbl.sdlmtype = MUS_NONE;
}

void hw_audio_music_play(int mus_index)
{
    // if (audio_initialized && opt_music_enabled && (mus_index < mus_num)) {
        if(Mix_PlayingMusic())
        {
            Mix_HaltMusic();
        }
        // 1oom  Mix_VolumeMusic(opt_music_volume);
        Mix_VolumeMusic(32);  // 128 / 4
        Mix_PlayMusic(mustbl.music, mustbl.loops ? -1 : 0);
        // mus_playing = mus_index;
    // }
}

void hw_audio_music_fadeout(void)
{
    if (audio_initialized && opt_music_enabled && Mix_PlayingMusic()) {
        Mix_FadeOutMusic(1000);
    }
}

void hw_audio_music_stop(void)
{
    // if (audio_initialized && opt_music_enabled) {
        Mix_HaltMusic();
        // mus_playing = -1;
    // }
}

bool hw_audio_music_volume(int volume)
{
    if (volume < 0) {
        volume = 0;
    }
    if (volume > 128) {
        volume = 128;
    }
    if (audio_initialized && opt_music_enabled) {
        Mix_VolumeMusic(volume);
    }
    if (opt_music_volume != volume) {
        // log_message("SDLA: music volume %i\n", volume);
        opt_music_volume = volume;
    }
    return true;
}




// C:\STU\developp\1oom\src\fmt_mus.c

static int8_t xmid_find_free_noteoff(struct noteoffs_s *s)
{
    int i = s->pos;
    int num = NOTEOFFBUFSIZE;
    while (num) {
        if (++i == NOTEOFFBUFSIZE) {
            i = 0;
        }
        if (s->tbl[i].ch == 0) {
            s->pos = i;
            return i;
        }
        --num;
    }
    return -1;
}

static bool xmid_add_pending_noteoff(struct noteoffs_s *s, const uint8_t *data, uint32_t t_now, uint32_t duration)
{
    uint32_t t = t_now + duration;
    int8_t i = xmid_find_free_noteoff(s);
    noteoff_t *n;
    if (i < 0) {
        /* log_error("XMID: BUG noteoff tbl full!\n"); */
        return false;
    }
    n = &(s->tbl[i]);
    n->next = -1;
    n->t = t;
    n->ch = data[0] & 0x8f; /* 9x -> 8x */
    n->note = data[1];

    if (s->top < 0) {
        s->top = i;
    } else {
        int j, k;
        j = s->top;
        k = -1;
        while ((j >= 0) && (t >= s->tbl[j].t)) {
            k = j;
            j = s->tbl[j].next;
        }
        if (k < 0) {
            n->next = s->top;
            s->top = i;
        } else {
            s->tbl[k].next = i;
            s->tbl[i].next = j;
        }
    }
    if (++s->num > s->max) {
        s->max = s->num;
    }
    return true;
}

static uint32_t xmid_encode_delta_time(uint8_t *buf, uint32_t delta_time)
{
    uint32_t len_event = 0, v = delta_time & 0x7f;
    while ((delta_time >>= 7) != 0) {
        v <<= 8;
        v |= (delta_time & 0x7f) | 0x80;
    }
    while (1) {
        buf[len_event++] = (uint8_t)(v & 0xff);
        if (v & 0x80) {
            v >>= 8;
        } else {
            return len_event;
        }
    }
}

static int xmid_convert_evnt(const uint8_t *data_in, uint32_t len_in, const uint8_t *timbre_tbl, uint16_t timbre_num, uint8_t *p, bool *tune_loops)
{
    struct noteoffs_s s[1];
    int rc = -1, noteons = 0, looppoint = -1;
    uint32_t len_out = 0, t_now = 0, delta_time = 0;
    bool end_found = false;
    *tune_loops = false;
    memset(s, 0, sizeof(s[0]));
    s->top = -1;

    while ((len_in > 0) && (!end_found)) {
        uint32_t len_event, add_extra_bytes, skip_extra_bytes;
        uint8_t buf_extra[4];
        bool add_event;

        add_event = true;
        add_extra_bytes = 0;
        skip_extra_bytes = 0;

        switch (*data_in & 0xf0) {
            case 0x90:
                {
                    uint32_t dt_off;
                    uint8_t b;
                    dt_off = 0;
                    skip_extra_bytes = 1;
                    while (((b = data_in[2 + skip_extra_bytes]) & 0x80) != 0) {
                        dt_off |= b & 0x7f;
                        dt_off <<= 7;
                        ++skip_extra_bytes;
                    }
                    dt_off |= b;
                    if (!xmid_add_pending_noteoff(s, data_in, t_now + delta_time, dt_off)) {
                        goto fail;
                    }
                }
                ++noteons;
                len_event = 3;
                break;

            case 0xb0:
                len_event = 3;
                {
                    uint8_t c;
                    c = data_in[1];
                    if (0
                      || ((c >= 0x20) && (c <= 0x2e))
                      || ((c >= 0x3a) && (c <= 0x3f))
                      || ((c >= 0x6e) && (c <= 0x78))
                    ) {
                        /* LOG_DEBUG((DEBUGLEVEL_FMTMUS, "XMID: dropping unhandled AIL CC event %02x %02x %02x, notes %i\n", *data_in, c, data_in[2], noteons)); */
                        add_event = false;
                        len_event = 0;
                        skip_extra_bytes = 3;
                    }
                    switch (c) {
                        case 0x74:  /* AIL loop: FOR loop = 1 to n */
                            if (looppoint >= 0) {
                                /* log_warning("XMID: nth FOR loop unimpl\n"); */
                            } else {
                                looppoint = noteons;
                                if (looppoint > 0) {
                                    /* log_warning("XMID: FOR loop after %i notes unimpl\n", looppoint); */
                                }
                            }
                            break;
                        case 0x75:  /* AIL loop: NEXT/BREAK */
                            /* LOG_DEBUG((DEBUGLEVEL_FMTMUS, "XMID: NEXT/BREAK at %i after %i notes, forcing end\n", looppoint, noteons)); */
                            if (looppoint >= 0) {
                                *tune_loops = true;
                            } else {
                                /* log_warning("XMID: NEXT/BREAK without FOR\n"); */
                            }
                            len_event = 0;
                            end_found = true;
                            add_event = true;
                            buf_extra[0] = 0xff;
                            buf_extra[1] = 0x2f;
                            buf_extra[2] = 0x00;
                            add_extra_bytes = 3;
                            break;
                        default:
                            break;
                    }
                }
                break;

            case 0xa0:
            case 0xe0:
                len_event = 3;
                break;
            case 0x80:
                /* log_error("XMID: unexpected noteoff\n"); */
                goto fail;
            case 0xc0:
                len_event = 2;
#ifdef XMID_USE_BANKS
                {
                    int ti;
                    uint8_t patch;
                    uint8_t bank = 0;
                    patch = data_in[1];
                    for (ti = 0; ti < timbre_num; ++ti) {
                        if (timbre_tbl[ti * 2] == patch) {
                            bank = timbre_tbl[ti * 2 + 1];
                            break;
                        }
                    }
                    if (ti < timbre_num) {
                        /* LOG_DEBUG((DEBUGLEVEL_FMTMUS, "XMID: TIMB found bank 0x%02x for patch 0x%02x\n", bank, patch)); */
                        buf_extra[0] = 0xb0 | (data_in[1] & 0xf);
                        buf_extra[1] = 0;
                        buf_extra[2] = bank;
                        buf_extra[3] = 0;
                        add_extra_bytes = 4;
                    } else {
                        /* LOG_DEBUG((DEBUGLEVEL_FMTMUS, "XMID: TIMB no bank for patch 0x%02x\n", patch)); */
                    }
                }
#endif /*XMID_USE_BANKS*/
                break;
            case 0xd0:
                len_event = 2;
                break;
            case 0xf0:
                switch (*data_in & 0xf) {
                    case 0x08:
                    case 0x0a:
                    case 0x0b:
                    case 0x0c:
                        len_event = 1;
                        break;
                    case 0x0f:
                        len_event = 3 + data_in[2];
                        if (data_in[1] == 0x2f) {
                            end_found = true;
                        } else if (data_in[1] == 0x51) {
                            /* MOO1 seems to ignore the set tempo events as not dropping them results in wrong tempo in f.ex tune 0xA */
                            /*& LOG_DEBUG((DEBUGLEVEL_FMTMUS, "XMID: dropping tempo %u event after %i notes\n", GET_BE_24(&data_in[3]), noteons)); */
                            add_event = false;
                            skip_extra_bytes = len_event;
                        }
                        break;
                    default:
                        /* log_error("XMID: unhandled event 0x%02x\n", *data_in); */
                        goto fail;
                }
                break;
            default:    /* 0x00..0x7f */
                add_event = false;
                while(!(*data_in & 0x80))
                {
                    delta_time += *data_in++;
                    --len_in;
                }
                break;
        }

        if (add_event) {
            uint32_t len_delta_time;
            uint8_t buf_delta_time[4];

            while((s->top >= 0) && ((t_now + delta_time) >= s->tbl[s->top].t))
            {
                noteoff_t *n = &(s->tbl[s->top]);
                uint32_t delay_noff = n->t - t_now;
                len_delta_time = xmid_encode_delta_time(buf_delta_time, delay_noff);
                for (int i = 0; i < len_delta_time; ++i) {
                    *p++ = buf_delta_time[i];
                }
                len_out += len_delta_time;
                *p++ = n->ch;
                *p++ = n->note;
                *p++ = 0x00;
                len_out += 3;
                delta_time -= delay_noff;
                t_now += delay_noff;
                n->ch = 0;
                s->top = n->next;
                --s->num;
            }
            t_now += delta_time;
            len_delta_time = xmid_encode_delta_time(buf_delta_time, delta_time);
            delta_time = 0;

            for(int i = 0; i < len_delta_time; ++i)
            {
                *p++ = buf_delta_time[i];
            }

            if(end_found)
            {
                /* last event, add remaining noteoffs */
                /* LOG_DEBUG((DEBUGLEVEL_FMTMUS, "XMID: %i noteoffs at end, max %i noteoffs, total %i noteons\n", s->num, s->max, noteons)); */
                while (s->top >= 0) {
                    noteoff_t *n = &(s->tbl[s->top]);
                    *p++ = n->ch;
                    *p++ = n->note;
                    *p++ = 0x00;
                    *p++ = 0;
                    len_out += 4;
                    n->ch = 0;
                    s->top = n->next;
                }
                s->num = 0;
            }

            for(int i = 0; i < add_extra_bytes; ++i)
            {
                *p++ = buf_extra[i];
            }

            for(int i = 0; i < len_event; ++i)
            {
                uint8_t c;
                c = *data_in++;
                --len_in;
                *p++ = c;
            }

            len_out += len_event + len_delta_time + add_extra_bytes;
        }

        if(skip_extra_bytes)
        {
            data_in += skip_extra_bytes;
            len_in -= skip_extra_bytes;
        }
    }

    rc = len_out;
fail:
    return rc;
}

/* -------------------------------------------------------------------------- */

// mus_type_t fmt_mus_detect(const uint8_t *data, uint32_t len)
// {
//     uint32_t hdrid;
//     if (len < 32) {
//         return MUS_TYPE_UNKNOWN;
//     }
//     hdrid = GET_BE_32(data);
//     if (hdrid == HDRID_LBXXMID) {
//         return MUS_TYPE_LBXXMID;
//     } else if (hdrid == HDRID_MIDI) {
//         return MUS_TYPE_MIDI;
//     } else if (hdrid == HDRID_WAV) {
//         return MUS_TYPE_WAV;
//     } else if (hdrid == HDRID_OGG) {
//         return MUS_TYPE_OGG;
//     } else if (hdrid == HDRID_FLAC) {
//         return MUS_TYPE_FLAC;
//     }
//     return MUS_TYPE_UNKNOWN;
// }

bool fmt_mus_convert_xmid(const uint8_t *data_in, uint32_t len_in, uint8_t **data_out_ptr, uint32_t *len_out_ptr, bool *tune_loops)
{
    uint8_t *data = NULL;
    const uint8_t *timbre_tbl;
    uint32_t len_timbre, len_evnt;
    uint16_t timbre_num;
    int len = 0;

    if (0
      /* || (fmt_mus_detect(data_in, len_in) != MUS_TYPE_LBXXMID) */
      || (len_in < 0x4e)
      || (memcmp(&data_in[0x10], (const uint8_t *)"FORM", 4) != 0)
      || (memcmp(&data_in[0x3e], (const uint8_t *)"TIMB", 4) != 0)
      || ((len_timbre = GET_BE_32(&data_in[0x42])), (len_in < (0x4e + len_timbre)))
      || (memcmp(&data_in[0x46 + len_timbre], (const uint8_t *)"EVNT", 4) != 0)
      || ((len_evnt = GET_BE_32(&data_in[0x4a + len_timbre])), (len_in < (0x4e + len_timbre + len_evnt)))
    ) {
        /* log_error("XMID: invalid header\n"); */
        goto fail;
    }
    /* LOG_DEBUG((DEBUGLEVEL_FMTMUS, "XMID: lent %i lene %i\n", len_timbre, len_evnt)); */

    // 1oom  timbre_num = GET_LE_16(&data_in[0x46]);
    timbre_num = GET_2B_OFS(&data_in[0x46], 0);
    timbre_tbl = &data_in[0x48];

    data_in += 0x4e + len_timbre;

    /* max. len_out/len_in ratio for MOO1 data is about 1.8 */
    // 1oom  data = lib_malloc(HDR_MIDI_LEN + len_evnt * 2);
    data = malloc(HDR_MIDI_LEN + len_evnt * 2);
    {
        uint8_t hdr[] = {
            /*00*/ 'M', 'T', 'h', 'd',
            /*04*/ 0, 0, 0, 6,
            /*08*/ 0, 0, 0, 1,
            /*0c*/ (XMID_TICKSPERQ >> 8) & 0xFF, XMID_TICKSPERQ & 0xFF,
            /*0e*/ 'M', 'T', 'r', 'k'
            /*12*/ /* length, big endian */
        };
        memcpy(data, hdr, sizeof(hdr));
    }

    len = xmid_convert_evnt(data_in, len_evnt, timbre_tbl, timbre_num, &data[HDR_MIDI_LEN], tune_loops);
    /* LOG_DEBUG((DEBUGLEVEL_FMTMUS, "XMID: lene %i len %i (%f) %s\n", len_evnt, len, (double)len / (double)len_evnt, *tune_loops ? "loop" : "once")); */
    if(len < 0)
    {
        goto fail;
    }
    SET_BE_32(&data[0x12], len);
    len += HDR_MIDI_LEN;
    // 1oom  data = lib_realloc(data, len);
    data = realloc(data, len);

    if (data_out_ptr) {
        *data_out_ptr = data;
    }
    if (len_out_ptr) {
        *len_out_ptr = len;
    }
    return true;

fail:
    // 1oom  lib_free(data);
    free(data);
    if(data_out_ptr)
    {
        *data_out_ptr = NULL;
    }
    if(len_out_ptr)
    {
        *len_out_ptr = 0;
    }
    return false;
}
