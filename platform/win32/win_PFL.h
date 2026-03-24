#ifndef WIN_PFL_H
#define WIN_PFL_H

#include <stdint.h>
#include <Windows.h>

#ifdef __cplusplus
extern "C" {
#endif



/* ========================================================================= */
/*  Win32 Backend State                                                      */
/* ========================================================================= */

extern HINSTANCE win_instance;
extern HWND win_window;
extern HDC win_device_context;

extern int win_window_width;
extern int win_window_height;

extern uint64_t win_ticks_startup;



/* ========================================================================= */
/*  Win32 Backend Internal Types                                             */
/* ========================================================================= */

struct win32_offscreen_buffer
{
    BITMAPINFO Info;
    void *Memory;
    int Width;
    int Height;
    int Pitch;
    int BytesPerPixel;
};

extern struct win32_offscreen_buffer win_video_back_buffer;



/* ========================================================================= */
/*  Win32 Backend Internal Functions                                         */
/* ========================================================================= */

/** VK (virtual key code) to MOX_KEY translation table. */
extern int VK_to_MOX_KEY[];

void Build_Key_Xlat(void);

void Win_Pump_Messages(void);



#ifdef __cplusplus
}
#endif

#endif /* WIN_PFL_H */
