
#include "STU_SND.H"

#include "../MOX/MOX_BITS.H"
#include "../MOX/MOX_DEF.H"
#include "../MOX/LBX_Load.H"
#include "../MOX/MOX_TYPE.H"

#include "STU_DBG.H"

#include <stdlib.h>     /* abs(); itoa(); malloc(); */



// BLK_SIZE        equ 32768               ;max. block size for INT 66 calls
#define BLK_SIZE 32768
// 32768 * 2 bytes * 2 channels = 131072  128KB
#define WAV_BUFFER_SIZE (BLK_SIZE * 2 * 2)
// uint8_t wav_buffer[WAV_BUFFER_SIZE];

// C:\devellib\FFmpeg\libavformat\voc.h
typedef enum voc_type {
    VOC_TYPE_EOF              = 0x00,
    VOC_TYPE_VOICE_DATA       = 0x01,
    VOC_TYPE_VOICE_DATA_CONT  = 0x02,
    VOC_TYPE_SILENCE          = 0x03,
    VOC_TYPE_MARKER           = 0x04,
    VOC_TYPE_ASCII            = 0x05,
    VOC_TYPE_REPETITION_START = 0x06,
    VOC_TYPE_REPETITION_END   = 0x07,
    VOC_TYPE_EXTENDED         = 0x08,
    VOC_TYPE_NEW_VOICE_DATA   = 0x09,
} VocType;


#define LEN_VOC_HDR     26
#define LEN_WAV_HDR     44

// SOUNDFX.LBX, 0
// SLX19.VOC
// uint8_t wav_header[LEN_WAV_HEADER] = {
//     /* [Master RIFF chunk] */
//     /*00*/ 'R', 'I', 'F', 'F',    /* RIFF Chunk ID: "RIFF"   */
//     /*04*/ 0, 0, 0, 0,          /* RIFF Chunk Body Size    */    /* (size in bytes - 8 bytes for header) (FourCC and Chunk Size */
//     /*08*/ 'W', 'A', 'V', 'E',    /* RIFF Form Type: "WAVE"  */
//     /* [Chunk describing the data format] */
//     /*0c*/ 'f', 'm', 't', ' ',    /* Format Chunk ID: "fmt " */
//     /*10*/ 16, 0, 0, 0,         /* Format Chunk Body Size  */    /* length of fmt chunk */
//     /* Format Chunk Body */
//     /*14*/ 1, 0,                                                /* audio format (1: PCM integer, 3: PCM IEEE 754 float) */
//     /*16*/ 1, 0,                                                /* channels */
//     /*18*/ 0x94, 0x15, 0, 0,                                          /* number of sample frames that occur each second  Hz */
//     /*1c*/ 0x94, 0x15, 0, 0,                                          /* bytes required for one second of audio data  Hz * 2B/sample * stereo */
//     /*20*/ 1, 0,                                                /* block align */
//     /*22*/ 8, 0,                                                   /* bits per sample */
//     /* [Chunk containing the sampled data] */
//     /*24*/ 'd', 'a', 't', 'a',    /* Data Chunk ID: "data" */
//     /*28*/ 0, 0, 0, 0           /* Data Chunk Body Size */
//     /* Data Chunk Body */
// };

// typedef enum {
//     SFX_TYPE_UNKNOWN = 0,
//     SFX_TYPE_LBXVOC,
//     SFX_TYPE_VOC,
//     SFX_TYPE_WAV
// } sfx_type_t;

struct sfx_conv_s {
    int16_t *data;
    uint32_t num;   /* number of stereo samples */
    uint32_t samplerate;
};



const uint8_t cl_voc_id[20] = {'C','r','e','a','t','i','v','e',' ','V','o','i','c','e',' ','F','i','l','e','\x1A'};
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

/*
expects LBX Entry that is a .VOC file
    16-byte LBX Entry Header
    VOC Header
    various VOC blocks
    VOC terminator block

Exult uses buffer[2 + decomp->pos]
FFmpeg uses type = avio_r8(pb);

*/
void Parse_VOC_LBX_Entry(char * lbx_file_name, int16_t lbx_entry_num)
{
    uint16_t voc_header_length;
    uint16_t voc_header_version;
    uint16_t voc_header_check;
    uint8_t block_type;
    uint32_t voice_len;
    SAMB_ptr lbx_entry_buf;
    uint32_t lbx_entry_len;
    uint8_t * voc_buf;
    uint32_t voc_len;
    uint8_t * block_ptr;
    uint8_t block_sample_rate;
    uint8_t block_codec_id;
    uint16_t lbx_entry_type;
    uint16_t lbx_entry_subtype;

    dbg_prn("DEBUG: [%s, %d]: BEGIN: %s, %d\n", __FILE__, __LINE__, lbx_file_name, lbx_entry_num);

    lbx_entry_buf = LBX_Load(lbx_file_name, lbx_entry_num);
    lbx_entry_len = lbxload_entry_length;

    dbg_prn("lbx_entry_len: %d\n", lbx_entry_len);

    dbg_prn("lbx_entry_buf[0]: %04X\n", GET_2B_OFS(lbx_entry_buf, 0));
    dbg_prn("lbx_entry_buf[2]: %04X\n", GET_2B_OFS(lbx_entry_buf, 2));
    dbg_prn("lbx_entry_buf[4]: %04X\n", GET_2B_OFS(lbx_entry_buf, 4));
    dbg_prn("lbx_entry_buf[6]: %04X\n", GET_2B_OFS(lbx_entry_buf, 6));

    lbx_entry_type = GET_2B_OFS(lbx_entry_buf, 0);
    dbg_prn("lbx_entry_type: %04X\n", lbx_entry_type);
    if(lbx_entry_type != LBX_ENTRY_TYPE_SOUND)
    {
        dbg_prn("ERROR: LBX Entry is not a sound type  {lbx_entry_type = %04X}\n", lbx_entry_type);
        return;
    }

    lbx_entry_subtype = GET_2B_OFS(lbx_entry_buf, 2);
    dbg_prn("lbx_entry_subtype: %04X\n", lbx_entry_subtype);
    if(lbx_entry_subtype != LBX_ENTRY_TYPE_SOUND_VOC)
    {
        dbg_prn("ERROR: LBX Entry is not a VOC sound type  {lbx_entry_subtype = %04X}\n", lbx_entry_subtype);
        return;
    }

    voc_buf = (lbx_entry_buf + LEN_LBX_SND_HDR);
    voc_len = (lbx_entry_len - LEN_LBX_SND_HDR);

    dbg_prn("voc_buf[0]: %04X\n", GET_2B_OFS(voc_buf, 0));
    dbg_prn("voc_buf[2]: %04X\n", GET_2B_OFS(voc_buf, 2));
    dbg_prn("voc_buf[4]: %04X\n", GET_2B_OFS(voc_buf, 4));
    dbg_prn("voc_buf[6]: %04X\n", GET_2B_OFS(voc_buf, 6));
    // ...
    dbg_prn("voc_buf[18]: %04X\n", GET_2B_OFS(voc_buf, 18));
    dbg_prn("voc_buf[20]: %04X\n", GET_2B_OFS(voc_buf, 20));
    dbg_prn("voc_buf[22]: %04X\n", GET_2B_OFS(voc_buf, 22));
    dbg_prn("voc_buf[24]: %04X\n", GET_2B_OFS(voc_buf, 24));

    if(memcmp(voc_buf, cl_voc_id, sizeof(cl_voc_id)))
    {
        dbg_prn("BAD VOC 1!!\n");
        return;
    }
    // if((voc_len < HDR_VOC_LEN) || (memcmp(voc_buf, HDR_VOC, HDR_VOC_LEN) != 0))
    // {
    //     dbg_prn("BAD VOC 2!!\n");
    //     return;
    // }
    voc_header_length = GET_2B_OFS(voc_buf, 20);
    dbg_prn("voc_header_length: %04X\n", voc_header_length);
    if(voc_header_length != HDR_VOC_LENGTH)
    {
        dbg_prn("ERROR:  bad VOC header length");
        return;
    }

    voc_header_version = GET_2B_OFS(voc_buf, 22);
    dbg_prn("voc_header_version: %04X\n", voc_header_version);
    if(voc_header_version != HDR_VOC_VERSION)
    {
        dbg_prn("ERROR:  bad VOC header version");
        return;
    }

    voc_header_check = GET_2B_OFS(voc_buf, 24);
    dbg_prn("voc_header_check: %02X\n", voc_header_check);
    if(voc_header_check != HDR_VOC_CHECK)
    {
        dbg_prn("ERROR:  bad VOC header check");
        return;
    }

    block_ptr = voc_buf;
    // buffer += ((VOCHDR far *)buffer)->voice_offset ;
    block_ptr += ((VOCHDR *)voc_buf)->voice_offset;

    // process_block   PROC                    ;Process current block in voice data

    block_type = GET_1B_OFS(block_ptr, 0);

    switch(block_type)
    {
        case 0:  /* ;terminator? */
        {
            dbg_prn("block type: terminator");
        } break;
        case 1:  /* ;new voice block? */
        {
            block_sample_rate = GET_1B_OFS(block_ptr, 4);
            dbg_prn("block_sample_rate: %d\n", block_sample_rate);
            block_codec_id = GET_1B_OFS(block_ptr, 5);
            dbg_prn("block_codec_id: %d\n", block_codec_id);
            // CLSBDK20  long voicesize;
            //           voicesize = *(buffer+3) ;
            //           voicesize <<= 16 ;
            //           voicesize += *(unsigned far *)(buffer+1) ;
            // // voice_len = (GET_3B_OFS(block_ptr, 1) - 2);
            // voice_len = *(block_ptr+3);
            // voice_len <<= 16;
            // voice_len += *(uint16_t *)(block_ptr+1);
            voice_len = (GET_3B_OFS(block_ptr, 1) - 2);
            dbg_prn("voice_len: %d\n", voice_len);
            block_ptr += 6;

        } break;
        case 2:  /* ;continued voice block? */
        {
            voice_len = (GET_3B_OFS(block_ptr, 0) - 2);
            dbg_prn("voice_len: %d\n", voice_len);
            block_ptr += 4;

        } break;
        case 3:
        {

        } break;
        case 4:
        {

        } break;
        case 5:
        {

        } break;
        case 6:
        {

        } break;
        case 7:
        {

        } break;
        case 8:
        {

        } break;
        case 9:
        {

        } break;
        default:
        {
            dbg_prn("ERROR:  bad VOC type");
            __debugbreak();
        }

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
int STU_Convert_VOC_To_WAV(const uint8_t * voc_buf, uint32_t voc_len, uint8_t ** out_wav_buf, uint32_t * out_wav_len)
{
    /* WAV */
    // out_wav_buf
    // out_wav_len
    uint8_t * wav_rvr = NULL;
    int16_t sample_16bit_signed = 0;
    uint8_t wav_header[LEN_WAV_HDR] = {'R', 'I', 'F', 'F', 0, 0, 0, 0, 'W', 'A', 'V', 'E', 'f', 'm', 't', ' ', 16, 0, 0, 0, 1, 0, 1, 0, 0x94, 0x15, 0, 0, 0x94, 0x15, 0, 0, 1, 0, 8, 0, 'd', 'a', 't', 'a', 0, 0, 0, 0};
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
    const uint8_t * voc_rvr = NULL;
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
    uint8_t * wav_buffer = NULL;

    wav_buffer = malloc(WAV_BUFFER_SIZE);

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
        
        switch(voc_block_type)
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
                if(voc_block_codec_id != 0)
                {
                    __debugbreak();
                }
                wav_sample_count += voc_block_size;
                voc_len -= voc_block_size;
                while(voc_block_size--)
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

    wav_buffer[0x04] = ((((LEN_WAV_HDR - 8) + wav_data_size) >>  0) & 0x000000FF);
    wav_buffer[0x05] = ((((LEN_WAV_HDR - 8) + wav_data_size) >>  8) & 0x000000FF);
    wav_buffer[0x06] = ((((LEN_WAV_HDR - 8) + wav_data_size) >> 16) & 0x000000FF);
    wav_buffer[0x07] = ((((LEN_WAV_HDR - 8) + wav_data_size) >> 24) & 0x000000FF);
    // Format Chunk :: Samples per second
    wav_buffer[0x18] = ((wav_samples_per_second >>  0) & 0x000000FF);
    wav_buffer[0x19] = ((wav_samples_per_second >>  8) & 0x000000FF);
    wav_buffer[0x1A] = ((wav_samples_per_second >> 16) & 0x000000FF);
    wav_buffer[0x1B] = ((wav_samples_per_second >> 24) & 0x000000FF);
    // Format Chunk :: Average Bytes per Second
    wav_buffer[0x1C] = ((wav_bytes_per_second >>  0) & 0x000000FF);
    wav_buffer[0x1D] = ((wav_bytes_per_second >>  8) & 0x000000FF);
    wav_buffer[0x1E] = ((wav_bytes_per_second >> 16) & 0x000000FF);
    wav_buffer[0x1F] = ((wav_bytes_per_second >> 24) & 0x000000FF);
    // Data Chunk :: Chunk Body Size
    wav_buffer[0x28] = ((wav_data_size >>  0) & 0x000000FF);
    wav_buffer[0x29] = ((wav_data_size >>  8) & 0x000000FF);
    wav_buffer[0x2A] = ((wav_data_size >> 16) & 0x000000FF);
    wav_buffer[0x2B] = ((wav_data_size >> 24) & 0x000000FF);

    // memcpy(wav_buffer[sizeof(wav_header)], wav_data, wav_data_size);
    
    *out_wav_buf = &wav_buffer[0];
    *out_wav_len = (LEN_WAV_HDR + wav_data_size);

    return 0;
}

/*
in:  pointer to VOC data buffer
out: pointer to WAV data buffer

AIL214 does not check for block types 3, 5, 8, or 9

CLSBDK:  "8-bit mono PCM data" ==> "16-bit stereo PCM data"

VOC: 8-bit, unsigned PCM
WAV: 16-bit, 2-channel ... ¿ WAV format 1 ? ¿ sample-rate 48000 ?
*/
// bool fmt_sfx_convert(const uint8_t *data_in, uint32_t len_in, uint8_t **data_out_ptr, uint32_t *len_out_ptr, sfx_type_t *type_out, int audiorate, bool add_wav_header)
int FUU_Convert_VOC_To_WAV(const uint8_t * voc_buf, uint32_t voc_len, int16_t ** out_wav_buf, uint32_t * out_wav_len)
{
    int voc_block_type;
    int16_t * wav_data = NULL;
    int16_t * tmp_wav_data = NULL;
    uint32_t wav_sample_count;   /* number of stereo samples */
    uint32_t wav_frequency;
    uint32_t wav_data_size;
    int32_t audiorate = 44100;
    uint32_t curr_wav_buf_size;
    uint32_t new_wav_buf_size;
    // int16_t *data2 = NULL, 
    int16_t * wav_ptr = NULL;
    const uint8_t * voc_ptr;
    uint8_t voc_block_codec_id = 0;
    uint8_t stereo = 0;
    uint8_t voc_block_sample_rate;
    uint32_t voc_block_frequency;
    uint32_t voc_block_size;
    uint8_t sample_8bit_unsigned;
    int16_t sample_16bit_signed;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Convert_VOC_To_WAV()\n", __FILE__, __LINE__);
#endif

    if((voc_buf == NULL) || (voc_len <= 0))
    {
        goto failure;
    }

    curr_wav_buf_size = 0;
    new_wav_buf_size = 0;
    wav_sample_count = 0;

    voc_ptr = voc_buf;

    voc_ptr += 16;
    voc_len -= 16;

    voc_ptr += HDR_VOC_LEN;
    voc_len -= HDR_VOC_LEN;

    // estimated size of output wav buffer - LBX entry size, in bytes, times 2, for 8 to 16-bit, times 2, for mono to stereo
    curr_wav_buf_size = (voc_len * 2 * 2);
    
    wav_data = (void *)malloc(curr_wav_buf_size);
    if(wav_data == NULL)
    {
        __debugbreak();
    }
    memset(wav_data, 0, curr_wav_buf_size);
    wav_ptr = wav_data;

    while(voc_len)
    {

        voc_block_type = GET_1B_OFS(voc_ptr, 0);

        switch(voc_block_type)
        {
            case VOC_TYPE_EOF:
            {
                voc_len -= 1;  // -1 for block type
                *wav_ptr++ = 0;
                *wav_ptr++ = 0;
                goto success;
            } break;
            case VOC_TYPE_VOICE_DATA:
            {
                dbg_prn("BEGIN: VOC_TYPE_VOICE_DATA\n");

                voc_block_sample_rate = GET_1B_OFS(voc_ptr, 4);
                dbg_prn("voc_block_sample_rate: %d\n", voc_block_sample_rate);
                voc_block_codec_id = GET_1B_OFS(voc_ptr, 5);
                dbg_prn("voc_block_codec_id: %d\n", voc_block_codec_id);
                voc_block_size = (GET_3B_OFS(voc_ptr, 1) - 2);  // -2 for sample rate and codec id
                dbg_prn("voc_block_size: %d\n", voc_block_size);
                voc_len -= 6;  // -6 for block type, block size, sample rate, and codec id
                voc_ptr += 6;  // +6 for block type, block size, sample rate, and codec id
    
                voc_block_frequency = 1000000 / (256 - voc_block_sample_rate);  // sample rate in hertz
                dbg_prn("voc_block_frequency: %d\n", voc_block_frequency);
                // TODO  if voc_block_frequency != wav_frequency ... override / libsamplerate
                wav_frequency = voc_block_frequency;
                dbg_prn("wav_frequency: %d\n", wav_frequency);

                if(voc_block_codec_id != 0)
                {
                    __debugbreak();
                }
                
                // maybe, reallocate memory for the WAV data buffer
                // 
                if(((wav_sample_count + voc_block_size) * 2 * 2) > curr_wav_buf_size)
                {
                    new_wav_buf_size = curr_wav_buf_size + ((voc_block_size * 2 * 2) | 0xff) + 1;
                    // wav_data = realloc(wav_data, new_wav_buf_size);
                    tmp_wav_data = realloc(wav_data, new_wav_buf_size);
                    if(tmp_wav_data == NULL)
                    {
                        __debugbreak();
                    }
                    wav_data = tmp_wav_data;
                
                    curr_wav_buf_size = new_wav_buf_size;
                    wav_ptr = &wav_data[wav_sample_count * 2];  // set pointer to start-of-data
                }
                
                wav_sample_count += voc_block_size;

                voc_len -= voc_block_size;

                while(voc_block_size--)
                {
                    // convert 8-bit, unsigned to 16-bit, signed
                    sample_8bit_unsigned = *voc_ptr++;
                    sample_16bit_signed = (int16_t)(sample_8bit_unsigned << 8);
                    // Create stereo data by duplicating the 16-bit value
                    *wav_ptr++ = sample_16bit_signed;
                    *wav_ptr++ = sample_16bit_signed;
                }

                dbg_prn("END: VOC_TYPE_VOICE_DATA\n");
            } break;
            case VOC_TYPE_VOICE_DATA_CONT:
            {
                __debugbreak();
                
            } break;
            case VOC_TYPE_SILENCE:
            {
                __debugbreak();

                uint16_t len_silence;  // sample count

                len_silence = GET_2B_OFS(voc_ptr,0) + 1;  // bytes 0-1    length of the silence - 1 (unit is sample)

                voc_ptr += 2;
                voc_len -= 2;

                voc_block_sample_rate = *voc_ptr++;  // 1-byte, unsigned byte  2      frequency divisor

                voc_len--;

                voc_block_frequency = 1000000 / (256 - voc_block_sample_rate);

                if(((wav_sample_count + len_silence) * 2 * 2) > curr_wav_buf_size)
                {
                    new_wav_buf_size = curr_wav_buf_size + ((len_silence * 4) | 0xff) + 1;
                    
                    // wav_data = realloc(wav_data, new_wav_buf_size);
                    tmp_wav_data = realloc(wav_data, new_wav_buf_size);
                    if (tmp_wav_data == NULL)
                    {
                        __debugbreak();
                    }
                    wav_data = tmp_wav_data;

                    curr_wav_buf_size = new_wav_buf_size;

                    wav_ptr = &wav_data[wav_sample_count * 2];

                }

                wav_sample_count += len_silence;

                while(len_silence--)
                {
                    *wav_ptr++ = 0;
                    *wav_ptr++ = 0;
                }

            } break;

            default:
            {
                __debugbreak();
            } break;

        }  /* switch(voc_block_type) */

    }  /* while(voc_len) */

    __debugbreak();

success:
    dbg_prn("Convert_VOC_To_WAV()  success:\n");

    wav_data_size = wav_sample_count * 2 * 2;

    // https://wavefilegem.com/how_wave_files_work.html
    uint8_t wav_header[LEN_WAV_HDR] = {
        /*00*/ 'R', 'I', 'F', 'F',
        /*04*/ 0, 0, 0, 0,  /* filesize - 8 */
        /*08*/ 'W', 'A', 'V', 'E',
        /*0c*/ 'f', 'm', 't', ' ',
        /*10*/ 16, 0, 0, 0, /* length of fmt chunk */
        /*14*/ 1, 0,        /* format */
        /*16*/ 2, 0,        /* channels */
        /*18*/ 0, 0, 0, 0,  /* Hz */
        /*1c*/ 0, 0, 0, 0,  /* Hz * 2B/sample * stereo */
        /*20*/ 4, 0,        /* block align */
        /*22*/ 16, 0,       /* bits per sample */
        /*24*/ 'd', 'a', 't', 'a',
        /*28*/ 0, 0, 0, 0   /* sample data size in bytes */
    };

    SET_4B_OFS(&wav_header[0], 0x04, ((LEN_WAV_HDR + wav_data_size) - 8));  /* File size (integer)  (total file size - 8 bytes), in bytes (32-bit integer) */
    SET_4B_OFS(&wav_header[0], 0x18, audiorate);                            /* Sample Rate - 32 bit integer. Common values are 44100 (CD), 48000 (DAT). Sample Rate = Number of Samples per second, or Hertz */
    SET_4B_OFS(&wav_header[0], 0x1C, ((audiorate * 16 * 2) / 8));           /* (Sample Rate * BitsPerSample * Channels) / 8 */
    SET_4B_OFS(&wav_header[0], 0x28, wav_data_size);                        /* File size (data)  Size of the data section */
    
    // wav_data = realloc(conv_data, (LEN_WAV_HDR + wav_data_size));
    new_wav_buf_size = (LEN_WAV_HDR + wav_data_size);
    tmp_wav_data = realloc(wav_data, new_wav_buf_size);
    if (tmp_wav_data == NULL)
    {
        __debugbreak();
    }
    wav_data = tmp_wav_data;

    memmove(&wav_data[LEN_WAV_HDR], wav_data, wav_data_size);

    memcpy(&wav_data[0], wav_header, LEN_WAV_HDR);
    
    *out_wav_buf = wav_data;
    *out_wav_len = (LEN_WAV_HDR + wav_data_size);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Convert_VOC_To_WAV()\n", __FILE__, __LINE__);
#endif

    return ST_TRUE;

failure:
    dbg_prn("Convert_VOC_To_WAV()  failure:\n");
    free(wav_data);
    *out_wav_buf = NULL;
    *out_wav_len = 0;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Convert_VOC_To_WAV()\n", __FILE__, __LINE__);
#endif

    return ST_FALSE;
}
