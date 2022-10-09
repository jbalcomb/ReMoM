
#include <windows.h>
#include <stdint.h>

/*
https://yakvi.github.io/handmade-hero-notes/html/day11.html
You might also notice that we decided to #include a .cpp file instead of building it separately. 
This is because, as our building philosophy, we use the so-called Unity Builds,
i.e. building everything in one go. You can read more in the subsection 6.1.
|-> https://yakvi.github.io/handmade-hero-notes/html/day11.html#toc6.1
*/
#include "mom_x64.cpp"

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

global_variable bool GlobalRunning;
global_variable win32_offscreen_buffer GlobalBackbuffer;

void *
PlatformLoadFile(char* Filename)
{
    // NOTE(casey): Implements the Win32 file loading
    // ... 
    return (0);
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
RenderWeirdGradient(win32_offscreen_buffer *Buffer, int XOffset, int YOffset)
{
    u8 *Row = (u8 *)Buffer->Memory;
    for (int Y = 0; Y < Buffer->Height; ++Y)
    {
        u32* Pixel = (u32*)Row;

        for(int X = 0; X < Buffer->Width; ++X)
        {
        u8 Red = 0;
        u8 Green = (u8)(Y + YOffset);
        u8 Blue = (u8)(X + XOffset);
        *Pixel++ = Red << 16 | Green << 8 | Blue; // << 0
        }
        Row += Buffer->Pitch;
    }
}

internal void
Win32ResizeDIBSection(win32_offscreen_buffer *Buffer, int Width, int Height)
{
     if(Buffer->Memory) // Same as writing (BitmapMemory != 0) or (BitmapMemory != NULL)
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
Win32DisplayBufferInWindow(win32_offscreen_buffer *Buffer, 
                           HDC DeviceContext, int WindowWidth, int WindowHeight)
{
    // TODO(casey): Aspect ratio correction

    StretchDIBits(DeviceContext, 
                  0, 0, WindowWidth, WindowHeight, // destination rectangle (window)
                  0, 0, Buffer->Width, Buffer->Height, // source rectangle (bitmap buffer)
                  Buffer->Memory,
                  &Buffer->Info,
                  DIB_RGB_COLORS, SRCCOPY);
    
}

LRESULT CALLBACK MainWindowCallback(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
    LRESULT Result = 0;

    switch (Message)
    {
        case WM_CLOSE:
        {
            OutputDebugStringA("WM_CLOSE\n");
            GlobalRunning = false;

        } break;
    
        case WM_DESTROY:
        {
            OutputDebugStringA("WM_DESTROY\n");
            GlobalRunning = false;
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
WinMain(HINSTANCE Instance,
        HINSTANCE PrevInstance,
        LPSTR     CommandeLine,
        int       ShowCode)
{

    // WNDCLASSEXW wcex = {0};
    // wcex.cbSize = sizeof(WNDCLASSEX);
    // wcex.style          = CS_HREDRAW | CS_VREDRAW;
    // wcex.lpfnWndProc    = MainWindowCallback;
    // wcex.hInstance      = Instance;
    // // wcex.hIcon
    // wcex.lpszClassName  = "MomRasmWindowClass";
    // // wcex.hIconSm
    // //RegisterClassExW(&wcex);

    WNDCLASSA WindowClass = {0};
    WindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpfnWndProc = MainWindowCallback;
    WindowClass.hInstance = Instance;
    // WindowClass.hIcon;  // TODO(JimBalcomb,20220821): create and add icon for MoM-Rasm app
    WindowClass.lpszClassName = "MomRasmWindowClass";

    RegisterClassA(&WindowClass);

    HWND Window = CreateWindowExA(0, WindowClass.lpszClassName, "MoM Reassembly",
                                  WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                  CW_USEDEFAULT, CW_USEDEFAULT,
                                  CW_USEDEFAULT, CW_USEDEFAULT,
                                  0, 0, Instance, 0);

    Win32ResizeDIBSection(&GlobalBackbuffer, 1280, 720);

    // NOTE(casey): Since we specified CS_OWNDC, we can just
    // get one device context and use it forever because we
    // are not sharing it with anyone.
    HDC DeviceContext = GetDC(Window);

    int XOffset = 0;
    int YOffset = 0;
    GlobalRunning = true;
    while (GlobalRunning)
    {
        MSG Message;
        while(PeekMessageA(&Message, 0, 0, 0, PM_REMOVE))
        {
            if (Message.message == WM_QUIT)
            {
                GlobalRunning = false;
            }
            TranslateMessage(&Message);
            DispatchMessageA(&Message);
        }
        // We dealt with our messages, now do the rest of our game loop here.

        RenderWeirdGradient(&GlobalBackbuffer, XOffset, YOffset);
        ++XOffset;

        win32_window_dimension Dimension = Win32GetWindowDimension(Window);
        Win32DisplayBufferInWindow(&GlobalBackbuffer, DeviceContext, Dimension.Width, Dimension.Height);

        MainLoop();

    }
        
    return 0;
}
