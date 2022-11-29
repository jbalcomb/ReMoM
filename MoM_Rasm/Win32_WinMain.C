
/*
    WinMain()
        |-> WndInit()
        |-> WndProc() <-| WndMsg()
    
*/

#include <windows.h>
#include <stdint.h>

#include "Win32_WinWnd.H"

#include "Win32_WndInit.C"
#include "Win32_WndProc.C"


HINSTANCE g_hInstance;
HWND g_hWnd;
int GlobalRunning;
win32_offscreen_buffer GlobalBackbuffer;
int g_KbHit;

const char g_lpszClassName[] = "Win32 ClassName";
const char g_lpszWindowName[] = "Win32 WindowName";


int CALLBACK WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CommandeLine, int ShowCode)
{
    OutputDebugStringA("DEBUG: BEGIN: WinMain()\n");

    g_hInstance = Instance;

    WndInit();



    Win32ResizeDIBSection(&GlobalBackbuffer, 1280, 720);
    HDC DeviceContext = GetDC(g_hWnd);
    int XOffset = 0;
    int YOffset = 0;
    GlobalRunning = TRUE;

    while (GlobalRunning)
    {
        MSG Message;
        while(PeekMessageA(&Message, 0, 0, 0, PM_REMOVE))
        {
            if (Message.message == WM_QUIT)
            {
                GlobalRunning = FALSE;
            }
            TranslateMessage(&Message);
            DispatchMessageA(&Message);
        }
        // We dealt with our messages, now do the rest of our game loop here.

        RenderWeirdGradient(&GlobalBackbuffer, XOffset, YOffset);
        ++XOffset;

        win32_window_dimension Dimension = Win32GetWindowDimension(g_hWnd);
        Win32DisplayBufferInWindow(&GlobalBackbuffer, DeviceContext, Dimension.Width, Dimension.Height);

    }

    OutputDebugStringA("DEBUG: END: WinMain()\n");

    return 0;
}


win32_window_dimension Win32GetWindowDimension(HWND Window)
{
    win32_window_dimension Result; 
    
    RECT ClientRect;
    GetClientRect(Window, &ClientRect);    
    Result.Width = ClientRect.right - ClientRect.left;
    Result.Height = ClientRect.bottom - ClientRect.top;
    
    return(Result);
}


void Win32ResizeDIBSection(win32_offscreen_buffer * Buffer, int Width, int Height)
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

void Win32DisplayBufferInWindow(win32_offscreen_buffer * Buffer, HDC DeviceContext, int WindowWidth, int WindowHeight)
{
    // TODO(casey): Aspect ratio correction

    StretchDIBits(DeviceContext, 
                  0, 0, WindowWidth, WindowHeight,      /* destination rectangle (window)   */
                  0, 0, Buffer->Width, Buffer->Height,  /* source rectangle (bitmap buffer) */
                  Buffer->Memory,
                  &Buffer->Info,
                  DIB_RGB_COLORS, SRCCOPY);
    
}


void RenderWeirdGradient(win32_offscreen_buffer * Buffer, int XOffset, int YOffset)
{
    uint8_t * Row = (uint8_t *) Buffer->Memory;
    for (int Y = 0; Y < Buffer->Height; ++Y)
    {
        uint32_t * Pixel = (uint32_t *)Row;

        for(int X = 0; X < Buffer->Width; ++X)
        {
        uint8_t Red   = (uint8_t) 0;
        uint8_t Green = (uint8_t) (Y + YOffset);
        uint8_t Blue  = (uint8_t) (X + XOffset);
        *Pixel++ = (uint32_t) Red << 16 | Green << 8 | Blue; // << 0
        }
        Row += Buffer->Pitch;
    }
}
