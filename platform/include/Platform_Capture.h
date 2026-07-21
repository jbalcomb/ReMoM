/**
 * Platform_Capture.h — in-engine A/V capture for rendering scripted demo videos.
 *
 * The game records its own frames and audio while it plays a `.hms` scenario, so a demo video is
 * produced by a build step rather than by screen-recording a live session.  See
 * doc/#AI_Plans/PRD-Scripted-Demo-Capture.md.
 *
 * Video is written as a CONSTANT-rate raw RGB24 stream, not one frame per present.  The game does
 * not present at a fixed rate (the DOS-derived timer, Release_Time() waits and per-screen loops all
 * vary), so Platform_Capture_Video_Frame() samples against a wall clock started at
 * Platform_Capture_Start() and duplicates the previously-presented frame to fill any gap.  The
 * result lines up with the audio track by construction -- one shared t = 0, no timestamp manifest,
 * no variable-frame-rate muxing, no drift.
 *
 * Output files in <out_dir>:
 *   video_rgb24.raw    constant-rate RGB24 frames, screen_pixel_width x screen_pixel_height
 *   audio.wav          post-mix PCM (SDL backends only; the win32 backend has no audio)
 *   capture_info.txt   fps / dimensions / audio format / frame count -- feeds the ffmpeg step
 */

#ifndef PLATFORM_CAPTURE_H
#define PLATFORM_CAPTURE_H

#include <stdint.h>

#include "Platform.h"   /* PFL_Color */



#ifdef __cplusplus
extern "C" {
#endif



/**
 * Begin capturing to out_dir (created if it does not exist).
 * @param out_dir  Directory to write video_rgb24.raw / audio.wav / capture_info.txt into.
 * @param fps      Constant output frame rate.  <= 0 selects the default (60).
 * @return 0 on success, non-zero on failure (capture stays inactive).
 */
int Platform_Capture_Start(const char * out_dir, int fps);

/**
 * Non-zero while a capture is running.  Backends test this before doing any per-frame work.
 */
int Platform_Capture_Active(void);

/**
 * Submit the frame that is about to be presented.  Call from Platform_Video_Update() immediately
 * before presenting, with the finished indexed framebuffer and the live palette.
 * @param indexed  w*h 8-bit palette indices (video_page_buffer[draw_page_num]).
 * @param w,h      Framebuffer dimensions.
 * @param palette  256 entries, already 0-255 per channel (platform_palette_buffer).
 */
void Platform_Capture_Video_Frame(const uint8_t * indexed, int w, int h, const PFL_Color * palette);

/**
 * Submit mixed audio.  Call from the SDL_mixer post-mix hook (Mix_SetPostMix) so the captured
 * stream is exactly what the player hears.
 * @param stream   Interleaved PCM in the device format.
 * @param len      Length of stream in bytes.
 */
void Platform_Capture_Audio(const uint8_t * stream, int len);

/**
 * Declare the audio device format, so the WAV header can be written correctly.  Call once, before
 * the first Platform_Capture_Audio(); without it no WAV is produced.
 * @param frequency      Sample rate in Hz.
 * @param channels       Channel count.
 * @param bits_per_sample  8/16/32.
 * @param is_float       Non-zero for IEEE-float samples (WAVE_FORMAT_IEEE_FLOAT).
 */
void Platform_Capture_Set_Audio_Format(int frequency, int channels, int bits_per_sample, int is_float);

/**
 * Flush the trailing frame, finalize the WAV header and capture_info.txt, and close everything.
 * Safe to call when no capture is active.
 */
void Platform_Capture_Stop(void);



#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_CAPTURE_H */
