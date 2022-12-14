
#include "MoX_TYPE.H"

#include "windows.h"



// unsigned integers
typedef uint8_t u8;     // 1-byte long unsigned integer
typedef uint16_t u16;   // 2-byte long unsigned integer
typedef uint32_t u32;   // 4-byte long unsigned integer
// NIU/DNE  typedef uint64_t u64;   // 8-byte long unsigned integer
// signed integers
typedef int8_t s8;      // 1-byte long signed integer
typedef int16_t s16;    // 2-byte long signed integer
typedef int32_t s32;    // 4-byte long signed integer
// NIU/DNE  typedef int64_t s64;    // 8-byte long signed integer


#define internal static 
#define local_persist static 
#define global_variable static 



extern uint8_t g_KbHit;

#define getch() { while(!g_KbHit) {} }


// Win32_Init.cpp

extern HINSTANCE g_Instance;
extern HWND g_Window;
extern HDC g_DeviceContext;


// Win32_Main.cpp

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

extern win32_offscreen_buffer GlobalBackbuffer;



// Win32_Init.cpp
BOOL Init_WDA_Game(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);
BOOL InitApplication(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);


// Win32_Evnt.cpp
void Poll_Messages(void);
LRESULT CALLBACK MainWindowCallback(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam);

// Win32_Main.cpp
void Render_VBB(win32_offscreen_buffer * Buffer);
win32_window_dimension Win32GetWindowDimension(HWND Window);
void Win32ResizeDIBSection(win32_offscreen_buffer * Buffer, int Width, int Height);
void Win32DisplayBufferInWindow(win32_offscreen_buffer * Buffer, HDC DeviceContext, int WindowWidth, int WindowHeight);
void Main_Game_Loop(void);
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);

// Win32_Render.cpp
void Render(void);
