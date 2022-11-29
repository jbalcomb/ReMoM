#ifndef WIN32_WINWND_H
#define WIN32_WINWND_H


#include <windows.h>
#include <stdint.h>

#define internal        static 
#define local_persist   static 
#define global_variable static 

struct s_win32_offscreen_buffer
{
    // NOTE(casey): Pixels are always 32-bits wide, 
    // Memory Order  0x BB GG RR xx
    // Little Endian 0x xx RR GG BB
    BITMAPINFO Info;
    void *Memory;
    int Width;
    int Height;
    int Pitch;
    int BytesPerPixel;
};
typedef struct s_win32_offscreen_buffer win32_offscreen_buffer;

struct s_win32_window_dimension
{
    int Width;
    int Height;
};
typedef struct s_win32_window_dimension win32_window_dimension;

extern HINSTANCE g_hInstance;
extern HWND g_hWnd;
extern int GlobalRunning;
extern win32_offscreen_buffer GlobalBackbuffer;
extern int g_KbHit;

extern const char g_lpszClassName[];
extern const char g_lpszWindowName[];

win32_window_dimension Win32GetWindowDimension(HWND Window);
void Win32ResizeDIBSection(win32_offscreen_buffer * Buffer, int Width, int Height);
void Win32DisplayBufferInWindow(win32_offscreen_buffer * Buffer, HDC DeviceContext, int WindowWidth, int WindowHeight);

int WndInit(void);

LRESULT CALLBACK WndProc(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam);

void RenderWeirdGradient(win32_offscreen_buffer * Buffer, int XOffset, int YOffset);


#endif  /* WIN32_WINWND_H */
