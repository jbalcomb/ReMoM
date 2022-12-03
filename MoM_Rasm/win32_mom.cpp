
#include <windows.h>
#include <stdint.h>

/*
https://yakvi.github.io/handmade-hero-notes/html/day11.html
You might also notice that we decided to #include a .cpp file instead of building it separately. 
This is because, as our building philosophy, we use the so-called Unity Builds,
i.e. building everything in one go. You can read more in the subsection 6.1.
|-> https://yakvi.github.io/handmade-hero-notes/html/day11.html#toc6.1
*/
#include "win32_init.cpp"


static bool g_KbHit;

#if !defined(__DOS16__)
//#define getch() (kbhit())
#define getch() { while(!g_KbHit) {} }
#endif


// unsigned integers
typedef uint8_t u8;     // 1-byte long unsigned integer
typedef uint16_t u16;   // 2-byte long unsigned integer
typedef uint32_t u32;   // 4-byte long unsigned integer
typedef uint64_t u64;   // 8-byte long unsigned integer
// signed integers
typedef int8_t s8;      // 1-byte long signed integer
typedef int16_t s16;    // 2-byte long signed integer
typedef int32_t s32;    // 4-byte long signed integer
typedef int64_t s64;    // 8-byte long signed integer

#define internal static 
#define local_persist static 
#define global_variable static 

struct win32_offscreen_buffer
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

struct win32_window_dimension
{
    int Width;
    int Height;
};

global_variable bool g_State_Run;
global_variable win32_offscreen_buffer GlobalBackbuffer;
global_variable win32_offscreen_buffer g_Video_Back_Buffer;

uint8_t video_back_buffer[64000];

// uint8_t * p_Palette_XBGR;
uint8_t p_Palette_XBGR[1024];


/*
    MGC
        {0,1,2,3,4} == {Continue,Load_Game,New_Game,Quit_To_DOS,Hall_Of_Fame}
*/
enum e_SCREENS
{
    scr_Main_Menu = 3,
    scr_Exit = 5
};

int16_t current_screen;



void
Render_VBB(win32_offscreen_buffer * Buffer)
{
    uint8_t * bbuf;
    bbuf = (uint8_t *)video_back_buffer;
    struct tagRGBQUAD XBGR;

    uint32_t * Pixel = (uint32_t*)Buffer->Memory;
    int itr;
    unsigned char vbb_byte;
    unsigned int * p_XBGR;
    unsigned int long_XBGR;
    p_XBGR = (uint32_t *)p_Palette_XBGR;
    for(itr = 0; itr < 64000; itr++)
    {
        vbb_byte = *(video_back_buffer + itr);
        long_XBGR = *(p_XBGR + vbb_byte);
        *Pixel++ = long_XBGR;
    }

}

internal win32_window_dimension
Win32GetWindowDimension(HWND Window)
{
    win32_window_dimension Result; 
    
    RECT ClientRect;
    GetClientRect(Window, &ClientRect);
    Result.Width = ClientRect.right - ClientRect.left;
    Result.Height = ClientRect.bottom - ClientRect.top;
    
    return(Result);
}

internal void
Win32ResizeDIBSection(win32_offscreen_buffer * Buffer, int Width, int Height)
{
    if(Buffer->Memory) // ~== (BitmapMemory != 0) ~== (BitmapMemory != NULL)
    {
        VirtualFree(Buffer->Memory, 0, MEM_RELEASE);
        // Optionally, you can check if the result of VirtualFree is not zero.
        // Print out an error message if it is.
    }

    Buffer->Width = Width;
    Buffer->Height = Height;
    Buffer->BytesPerPixel = 4;
    Buffer->Pitch = Buffer->Width * Buffer->BytesPerPixel;

    Buffer->Info.bmiHeader.biSize = sizeof(Buffer->Info.bmiHeader);
    Buffer->Info.bmiHeader.biWidth = Buffer->Width;
    Buffer->Info.bmiHeader.biHeight = -Buffer->Height; // negative value: top-down
    Buffer->Info.bmiHeader.biPlanes = 1;
    Buffer->Info.bmiHeader.biBitCount = 32;
    Buffer->Info.bmiHeader.biCompression = BI_RGB;
    
    int BitmapMemorySize = Buffer->BytesPerPixel * (Buffer->Width * Buffer->Height);
    
    Buffer->Memory = VirtualAlloc(0, BitmapMemorySize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

internal void
Win32DisplayBufferInWindow(win32_offscreen_buffer * Buffer, 
                           HDC DeviceContext, int WindowWidth, int WindowHeight)
{
    // TODO(casey): Aspect ratio correction

    StretchDIBits(DeviceContext, 
                  0, 0, WindowWidth, WindowHeight,      /* destination rectangle (window)   */
                  0, 0, Buffer->Width, Buffer->Height,  /* source rectangle (bitmap buffer) */
                  Buffer->Memory,
                  &Buffer->Info,
                  DIB_RGB_COLORS, SRCCOPY);
    
}

void Poll_Messages(void)
{
        MSG Message;
        while(PeekMessageA(&Message, 0, 0, 0, PM_REMOVE))
        {
            if (Message.message == WM_QUIT)
            {
                g_State_Run = false;
            }
            TranslateMessage(&Message);
            DispatchMessageA(&Message);
        }
}

void Screen_Control(void)
{
    switch(current_screen)
    {
        case scr_Main_Menu:
        {
            // Main_Menu_Screen();
        } break;
    }
}

void Main_Game_Loop(void)
{
    while(g_State_Run)
    {
        Poll_Messages();

        Screen_Control();

        // Copy Back-Buffer to Front-Buffer
        Render_VBB(&GlobalBackbuffer);

        win32_window_dimension Dimension = Win32GetWindowDimension(g_Window);
        Win32DisplayBufferInWindow(&GlobalBackbuffer, g_DeviceContext, Dimension.Width, Dimension.Height);
    }
}

LRESULT CALLBACK MainWindowCallback(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
    LRESULT Result = 0;

    switch (Message)
    {
        case WM_CLOSE:
        {
            OutputDebugStringA("WM_CLOSE\n");
            g_State_Run = false;

        } break;
    
        case WM_DESTROY:
        {
            OutputDebugStringA("WM_DESTROY\n");
            g_State_Run = false;
        } break;

        // https://yakvi.github.io/handmade-hero-notes/html/day6.html#keyboardinput
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            u32 VKCode = WParam;  // the virtual-key code of the key

            if (VKCode == VK_SPACE)
            {
                g_KbHit = TRUE;
                OutputDebugStringA("SPACEBAR\n");
            } 
            if (VKCode == 'W')
            {
                OutputDebugStringA("W\n");
            } 
            else if (VKCode == 'A')
            {
                OutputDebugStringA("A\n");
            } 
            else if (VKCode == 'S')
            {
                OutputDebugStringA("S\n");
            } 
            else if (VKCode == 'D')
            {
                OutputDebugStringA("D\n");
            } 
        } break;

        case WM_PAINT:
        {
            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(Window, &Paint);
            
            win32_window_dimension Dimension = Win32GetWindowDimension(Window);
            Win32DisplayBufferInWindow(&GlobalBackbuffer, DeviceContext, Dimension.Width, Dimension.Height);
            EndPaint(Window, &Paint);
        } break;

        case WM_SIZE:
        {
            OutputDebugStringA("WM_SIZE\n");
        } break;

        case WM_ACTIVATEAPP:
        {
            OutputDebugStringA("WM_ACTIVATEAPP\n");
        } break;

        default:
        {
            Result = DefWindowProc(Window, Message, WParam, LParam);
        } break;
    }

    return Result;
}

int CALLBACK
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
#ifdef STU_DEBUG
    Debug_Log_Startup();
#endif

    // Initialize the "Windows Desktop Application"
    // ~== 'Application-Type' of 'Game'
    Init_WDA_Game(hInstance, hPrevInstance, lpCmdLine, nShowCmd);


    /*
        BEGIN: Initialize Application State
    */
    
    // Init_STGE()
    // Init_MGC()
    // Init_WZD()
    // Init_MoM()
    g_State_Run = true;
    current_screen = scr_Main_Menu;

    /*
        END: Initialize Application State
    */



    Win32ResizeDIBSection(&GlobalBackbuffer, 1280, 720);

    Main_Game_Loop();

#ifdef STU_DEBUG
    Debug_Log_Shutdown();
#endif

    return 0;
}
