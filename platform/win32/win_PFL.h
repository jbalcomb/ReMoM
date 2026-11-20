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

/* Viewport: where the game image is shown in the window, and the window <-> game mouse mapping (win_PFL.cpp).
   PLAN-Fullscreen.md phase 8 -- the Win32 port of the SDL2 backend's phases 1-6. */
#include "../include/Platform_Viewport.h"
extern struct s_VIEWPORT win_viewport;
int Win_Viewport_Refresh(void);  /* re-read the client size and recompute win_viewport; 1 if it changed (and logged) */
void Win_Viewport_Log(void);     /* one INFO line describing win_viewport, and the same to the input metrics if on */

/* Window states and mouse capture (win_PFL.cpp). */
void Win_Set_Window_State(int window_state);  /* VIEWPORT_WINDOW_STATE_* */
void Win_Toggle_Fullscreen(void);  /* Alt+Enter: fullscreen <-> the window */



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

/* CLAUDE: re-assert the hardware cursor (defined in win_MD.c) from WM_SETCURSOR. */
void Win_HW_Cursor_Apply(void);



#ifdef __cplusplus
}
#endif

#endif /* WIN_PFL_H */
