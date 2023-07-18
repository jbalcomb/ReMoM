#include "MoX_TYPE.H"

#include "windows.h"

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef s32 b32;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef float f32;
typedef double f64;
#define internal static 
#define local_persist static 
#define global_variable static 



extern uint8_t g_KbHit;

#define getch() { while(!g_KbHit) {} }



struct win32_offscreen_buffer
{
    BITMAPINFO Info;
    void * Memory;
    int Width;
    int Height;
    int Pitch;
    int BytesPerPixel;
};

struct win32_window_dimension
{
    int Width;
    int Height;
};



extern HINSTANCE g_Instance;
extern "C" { extern HWND g_Window; }
extern HDC g_DeviceContext;

extern const char lpszClassName[];
extern const char lpszWindowName[];

extern win32_offscreen_buffer GlobalBackbuffer;

// extern global_variable s64 GlobalPerfCountFrequency;  // HMH Day 18



// Win32_Init.cpp
BOOL Init_WDA_Game(HINSTANCE hInstance, int nShowCmd);
BOOL InitApplication(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
// void ErrorExit(LPTSTR lpszFunction);
// void ErrorExit(const wchar_t * lpszFunction);
void ErrorExit(LPCTSTR lpszFunction);

// Win32_Evnt.cpp
void Poll_Messages(void);
LRESULT CALLBACK MainWindowCallback(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam);

// Win32_Main.cpp
win32_window_dimension Win32GetWindowDimension(HWND Window);
void Win32ResizeDIBSection(win32_offscreen_buffer * Buffer, int Width, int Height);
void Win32DisplayBufferInWindow(win32_offscreen_buffer * Buffer, HDC DeviceContext, int WindowWidth, int WindowHeight);
