


https://github.com/FFmpeg/FFmpeg/blob/master/libavformat/voc.c

const AVCodecTag ff_voc_codec_tags[] = {
    {AV_CODEC_ID_PCM_U8,        0x00},
    {AV_CODEC_ID_ADPCM_SBPRO_4, 0x01},
    {AV_CODEC_ID_ADPCM_SBPRO_3, 0x02},
    {AV_CODEC_ID_ADPCM_SBPRO_2, 0x03},
    {AV_CODEC_ID_PCM_S16LE,     0x04},
    {AV_CODEC_ID_PCM_ALAW,      0x06},
    {AV_CODEC_ID_PCM_MULAW,     0x07},
    {AV_CODEC_ID_ADPCM_CT,    0x0200},
    {AV_CODEC_ID_NONE,             0},
};

#if CONFIG_VOC_DEMUXER || CONFIG_VOC_MUXER
const unsigned char ff_voc_magic[21] = "Creative Voice File\x1A";
const AVCodecTag *const ff_voc_codec_tags_list[] = { ff_voc_codec_tags, NULL };
#endif

https://github.com/FFmpeg/FFmpeg/blob/master/libavformat/voc.h

#ifndef AVFORMAT_VOC_H
#define AVFORMAT_VOC_H

#include "avformat.h"
#include "internal.h"

typedef struct voc_dec_context {
    int64_t remaining_size;
    int64_t pts;
} VocDecContext;

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

extern const unsigned char ff_voc_magic[21];
extern const AVCodecTag ff_voc_codec_tags[];
extern const AVCodecTag *const ff_voc_codec_tags_list[];

int ff_voc_get_packet(AVFormatContext *s, AVPacket *pkt, AVStream *st, int max_size);

#endif /* AVFORMAT_VOC_H */


https://github.com/FFmpeg/FFmpeg/blob/master/libavformat/vocdec.c



https://github.com/FFmpeg/FFmpeg/blob/master/libavformat/vocenc.c
