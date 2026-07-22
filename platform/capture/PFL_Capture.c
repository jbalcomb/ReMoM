/**
 * PFL_Capture.c — in-engine A/V capture (see Platform_Capture.h and
 * doc/#AI_Plans/PRD-Scripted-Demo-Capture.md).
 *
 * Backend-agnostic: each windowed backend's Platform_Video_Update() feeds video here, and the SDL
 * backends' post-mix hook feeds audio.  Nothing in this file touches SDL or Win32.
 */

#include "../include/Platform_Capture.h"

#include "../../ext/stu_compat.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)
#include <direct.h>
#define CAPTURE_MKDIR(p)  _mkdir(p)
#else
#include <sys/stat.h>
#include <sys/types.h>
#define CAPTURE_MKDIR(p)  mkdir((p), 0755)
#endif



#define CAPTURE_DEFAULT_FPS   60
#define CAPTURE_MAX_PATH      512

/* WAV header is 44 bytes for PCM; sizes are patched in at Stop(). */
#define CAPTURE_WAV_HEADER_BYTES  44



static int        m_active = 0;
static int        m_fps = CAPTURE_DEFAULT_FPS;
static char       m_out_dir[CAPTURE_MAX_PATH];

static FILE *     m_video_file = NULL;
static uint8_t *  m_last_frame_rgb = NULL;     /* what was on screen during the interval being backfilled */
static int        m_frame_width = 0;
static int        m_frame_height = 0;
static uint64_t   m_frames_written = 0;
static uint64_t   m_start_millies = 0;
static int        m_have_first_frame = 0;

static FILE *     m_audio_file = NULL;
static int        m_audio_frequency = 0;
static int        m_audio_channels = 0;
static int        m_audio_bits = 0;
static int        m_audio_is_float = 0;
static uint64_t   m_audio_bytes = 0;



/* ========================================================================= */
/*  Helpers                                                                  */
/* ========================================================================= */

/* Defined below; called from Platform_Capture_Start(), which appears above the definition. */
static void Capture_Open_Wav_If_Ready(void);

static void Capture_Join_Path(char * dst, size_t dst_size, const char * dir, const char * name)
{
    size_t len;

    dst[0] = '\0';
    stu_strncat(dst, dir, dst_size - 1);
    len = stu_strlen(dst);
    if((len > 0) && (dst[len - 1] != '/') && (dst[len - 1] != '\\') && (len + 1 < dst_size))
    {
        dst[len] = '/';
        dst[len + 1] = '\0';
    }
    stu_strncat(dst, name, dst_size - stu_strlen(dst) - 1);
}


/* Create out_dir, including any missing parents.  Existing directories are not an error. */
static int Capture_Make_Dirs(const char * path)
{
    char   work[CAPTURE_MAX_PATH];
    size_t i;

    work[0] = '\0';
    stu_strncat(work, path, sizeof(work) - 1);

    for(i = 0; work[i] != '\0'; i++)
    {
        if(((work[i] == '/') || (work[i] == '\\')) && (i > 0))
        {
            char saved = work[i];
            work[i] = '\0';
            CAPTURE_MKDIR(work);   /* errors ignored: already-exists is the common case */
            work[i] = saved;
        }
    }
    CAPTURE_MKDIR(work);

    return 0;
}


static void Capture_Write_LE32(FILE * f, uint32_t value)
{
    uint8_t bytes[4];

    bytes[0] = (uint8_t)(value & 0xFF);
    bytes[1] = (uint8_t)((value >> 8) & 0xFF);
    bytes[2] = (uint8_t)((value >> 16) & 0xFF);
    bytes[3] = (uint8_t)((value >> 24) & 0xFF);
    fwrite(bytes, 1, 4, f);
}


static void Capture_Write_LE16(FILE * f, uint16_t value)
{
    uint8_t bytes[2];

    bytes[0] = (uint8_t)(value & 0xFF);
    bytes[1] = (uint8_t)((value >> 8) & 0xFF);
    fwrite(bytes, 1, 2, f);
}


/* Write a placeholder WAV header; the two size fields are patched in Capture_Finalize_Wav(). */
static void Capture_Write_Wav_Header(FILE * f)
{
    uint16_t format_tag = (uint16_t)((m_audio_is_float != 0) ? 3 : 1);   /* 3 = IEEE_FLOAT, 1 = PCM */
    uint16_t block_align = (uint16_t)(m_audio_channels * (m_audio_bits / 8));
    uint32_t byte_rate = (uint32_t)(m_audio_frequency * block_align);

    fwrite("RIFF", 1, 4, f);
    Capture_Write_LE32(f, 0);            /* patched: 36 + data size */
    fwrite("WAVE", 1, 4, f);

    fwrite("fmt ", 1, 4, f);
    Capture_Write_LE32(f, 16);
    Capture_Write_LE16(f, format_tag);
    Capture_Write_LE16(f, (uint16_t)m_audio_channels);
    Capture_Write_LE32(f, (uint32_t)m_audio_frequency);
    Capture_Write_LE32(f, byte_rate);
    Capture_Write_LE16(f, block_align);
    Capture_Write_LE16(f, (uint16_t)m_audio_bits);

    fwrite("data", 1, 4, f);
    Capture_Write_LE32(f, 0);            /* patched: data size */
}


static void Capture_Finalize_Wav(void)
{
    if(m_audio_file == NULL)
    {
        return;
    }

    fseek(m_audio_file, 4, SEEK_SET);
    Capture_Write_LE32(m_audio_file, (uint32_t)(36 + m_audio_bytes));
    fseek(m_audio_file, 40, SEEK_SET);
    Capture_Write_LE32(m_audio_file, (uint32_t)m_audio_bytes);

    stu_fclose(m_audio_file);
    m_audio_file = NULL;
}


static void Capture_Write_Info(void)
{
    char   path[CAPTURE_MAX_PATH];
    FILE * f;

    Capture_Join_Path(path, sizeof(path), m_out_dir, "capture_info.txt");
    f = stu_fopen_ci(path, "wb");
    if(f == NULL)
    {
        return;
    }

    fprintf(f, "video_file=video_rgb24.raw\n");
    fprintf(f, "pixel_format=rgb24\n");
    fprintf(f, "width=%d\n", m_frame_width);
    fprintf(f, "height=%d\n", m_frame_height);
    fprintf(f, "fps=%d\n", m_fps);
    fprintf(f, "frame_count=%llu\n", (unsigned long long)m_frames_written);
    if(m_audio_bytes > 0)
    {
        fprintf(f, "audio_file=audio.wav\n");
        fprintf(f, "audio_frequency=%d\n", m_audio_frequency);
        fprintf(f, "audio_channels=%d\n", m_audio_channels);
        fprintf(f, "audio_bits=%d\n", m_audio_bits);
        fprintf(f, "audio_is_float=%d\n", m_audio_is_float);
        fprintf(f, "audio_bytes=%llu\n", (unsigned long long)m_audio_bytes);
    }
    else
    {
        fprintf(f, "audio_file=\n");   /* silent capture (e.g. the win32 backend) */
    }

    stu_fclose(f);
}



/* ========================================================================= */
/*  Public API                                                               */
/* ========================================================================= */

int Platform_Capture_Start(const char * out_dir, int fps)
{
    char path[CAPTURE_MAX_PATH];

    if(m_active != 0)
    {
        fprintf(stderr, "[capture] already active; ignoring Platform_Capture_Start(\"%s\")\n", (out_dir != NULL) ? out_dir : "");
        return 1;
    }
    if((out_dir == NULL) || (out_dir[0] == '\0'))
    {
        fprintf(stderr, "[capture] no output directory given\n");
        return 1;
    }

    m_out_dir[0] = '\0';
    stu_strncat(m_out_dir, out_dir, sizeof(m_out_dir) - 1);
    Capture_Make_Dirs(m_out_dir);

    m_fps = (fps > 0) ? fps : CAPTURE_DEFAULT_FPS;

    Capture_Join_Path(path, sizeof(path), m_out_dir, "video_rgb24.raw");
    m_video_file = stu_fopen_ci(path, "wb");
    if(m_video_file == NULL)
    {
        fprintf(stderr, "[capture] cannot open '%s' for writing\n", path);
        return 1;
    }

    m_last_frame_rgb = NULL;
    /* Provisional geometry for the up-front info file; the first submitted frame is authoritative. */
    m_frame_width = screen_pixel_width;
    m_frame_height = screen_pixel_height;
    m_frames_written = 0;
    m_have_first_frame = 0;
    m_audio_bytes = 0;
    m_audio_file = NULL;

    m_start_millies = Platform_Get_Millies();
    m_active = 1;

    /* The audio backend normally declared its format during Startup_Platform(), before we existed. */
    Capture_Open_Wav_If_Ready();

    /* Write capture_info.txt up front as well as at Stop().  A render that is killed rather than
       exiting cleanly still leaves an encodable capture -- frame_count is recoverable from the raw
       file size, and everything else in the info file is known at start. */
    Capture_Write_Info();

    fprintf(stderr, "[capture] started: dir='%s' fps=%d\n", m_out_dir, m_fps);
    return 0;
}


int Platform_Capture_Active(void)
{
    return m_active;
}


/*
    Open the WAV once BOTH facts are known: that a capture is running, and what the audio device
    format is.  These arrive in either order and, in practice, the unhelpful one:
    Startup_Platform() initialises audio (and declares the format) BEFORE the CLI is parsed and
    Platform_Capture_Start() runs.  An earlier version bailed out of Set_Audio_Format when capture
    was not yet active, which silently produced video-only captures.  Hence this is called from both
    Start() and Set_Audio_Format(), and is idempotent.
*/
static void Capture_Open_Wav_If_Ready(void)
{
    char path[CAPTURE_MAX_PATH];

    if((m_active == 0) || (m_audio_file != NULL))
    {
        return;
    }
    if((m_audio_frequency <= 0) || (m_audio_channels <= 0) || (m_audio_bits <= 0))
    {
        return;   /* format not declared yet -- the audio backend will call us back */
    }

    Capture_Join_Path(path, sizeof(path), m_out_dir, "audio.wav");
    m_audio_file = stu_fopen_ci(path, "wb");
    if(m_audio_file == NULL)
    {
        fprintf(stderr, "[capture] cannot open '%s' for writing; audio not captured\n", path);
        return;
    }

    Capture_Write_Wav_Header(m_audio_file);
    fprintf(stderr, "[capture] audio: %d Hz, %d ch, %d bits%s\n", m_audio_frequency, m_audio_channels, m_audio_bits, (m_audio_is_float != 0) ? " (float)" : "");
}


void Platform_Capture_Set_Audio_Format(int frequency, int channels, int bits_per_sample, int is_float)
{
    if(m_audio_file != NULL)
    {
        return;   /* already writing; a mid-capture device change is not supported */
    }

    /* Recorded unconditionally -- this usually arrives BEFORE the capture is started. */
    m_audio_frequency = frequency;
    m_audio_channels = channels;
    m_audio_bits = bits_per_sample;
    m_audio_is_float = is_float;

    if((frequency <= 0) || (channels <= 0) || (bits_per_sample <= 0))
    {
        fprintf(stderr, "[capture] bad audio format (%d Hz, %d ch, %d bits); audio not captured\n", frequency, channels, bits_per_sample);
        return;
    }

    Capture_Open_Wav_If_Ready();
}


void Platform_Capture_Audio(const uint8_t * stream, int len)
{
    if((m_active == 0) || (m_audio_file == NULL) || (stream == NULL) || (len <= 0))
    {
        return;
    }

    fwrite(stream, 1, (size_t)len, m_audio_file);
    m_audio_bytes += (uint64_t)len;
}


void Platform_Capture_Video_Frame(const uint8_t * indexed, int w, int h, const PFL_Color * palette)
{
    uint64_t now;
    uint64_t target_frames;
    size_t   rgb_bytes;
    int      i;

    if((m_active == 0) || (indexed == NULL) || (palette == NULL) || (w <= 0) || (h <= 0))
    {
        return;
    }

    /* First frame establishes the geometry; a mid-capture resolution change would corrupt the raw
       stream, so refuse it rather than write frames of two different sizes. */
    if(m_have_first_frame == 0)
    {
        m_frame_width = w;
        m_frame_height = h;
        m_last_frame_rgb = (uint8_t *)calloc((size_t)w * (size_t)h, 3);
        if(m_last_frame_rgb == NULL)
        {
            fprintf(stderr, "[capture] out of memory for the frame buffer; capture stopped\n");
            m_active = 0;
            return;
        }
        m_have_first_frame = 1;
    }
    else if((w != m_frame_width) || (h != m_frame_height))
    {
        fprintf(stderr, "[capture] frame size changed %dx%d -> %dx%d mid-capture; frame dropped\n", m_frame_width, m_frame_height, w, h);
        return;
    }

    rgb_bytes = (size_t)m_frame_width * (size_t)m_frame_height * 3;

    /* Emit however many constant-rate frames have come due since the last emit, filling them with
       the frame that was actually on screen during that interval (the PREVIOUS one) -- then adopt
       the new frame as "what is on screen now". */
    now = Platform_Get_Millies();
    target_frames = ((now - m_start_millies) * (uint64_t)m_fps) / 1000ull;
    while(m_frames_written < target_frames)
    {
        fwrite(m_last_frame_rgb, 1, rgb_bytes, m_video_file);
        m_frames_written++;
    }

    for(i = 0; i < (m_frame_width * m_frame_height); i++)
    {
        const PFL_Color * color = &palette[indexed[i]];
        m_last_frame_rgb[(i * 3) + 0] = color->r;
        m_last_frame_rgb[(i * 3) + 1] = color->g;
        m_last_frame_rgb[(i * 3) + 2] = color->b;
    }
}


void Platform_Capture_Stop(void)
{
    if(m_active == 0)
    {
        return;
    }

    /* Flush the trailing frame so the last thing shown is not lost. */
    if((m_video_file != NULL) && (m_last_frame_rgb != NULL))
    {
        uint64_t now = Platform_Get_Millies();
        uint64_t target_frames = ((now - m_start_millies) * (uint64_t)m_fps) / 1000ull;
        size_t   rgb_bytes = (size_t)m_frame_width * (size_t)m_frame_height * 3;

        if(target_frames <= m_frames_written)
        {
            target_frames = m_frames_written + 1;
        }
        while(m_frames_written < target_frames)
        {
            fwrite(m_last_frame_rgb, 1, rgb_bytes, m_video_file);
            m_frames_written++;
        }
    }

    if(m_video_file != NULL)
    {
        stu_fclose(m_video_file);
        m_video_file = NULL;
    }

    Capture_Finalize_Wav();
    Capture_Write_Info();

    fprintf(stderr, "[capture] stopped: %llu frames at %d fps (%.1fs), %llu audio bytes -> '%s'\n",
            (unsigned long long)m_frames_written, m_fps,
            (double)m_frames_written / (double)((m_fps > 0) ? m_fps : 1),
            (unsigned long long)m_audio_bytes, m_out_dir);

    free(m_last_frame_rgb);
    m_last_frame_rgb = NULL;
    m_have_first_frame = 0;
    m_active = 0;
}
