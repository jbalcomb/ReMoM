
#define STU_DEBUG 1

#include <windows.h>
#include <stdint.h>

/*
https://yakvi.github.io/handmade-hero-notes/html/day11.html
You might also notice that we decided to #include a .cpp file instead of building it separately. 
This is because, as our building philosophy, we use the so-called Unity Builds,
i.e. building everything in one go. You can read more in the subsection 6.1.
|-> https://yakvi.github.io/handmade-hero-notes/html/day11.html#toc6.1
*/
#include "win_DEF.hpp"

#include "win_Fonts.cpp"
#include "win_FLIC.cpp"
#include "win_LBX.cpp"
#include "win_SA.cpp"
#include "win_MainMenu.cpp"

#include "win_BMP.cpp"

#ifdef STU_DEBUG
#include "J:\STU\devel\STU-MoM_Rasm\MoM_Rasm\STU\STU_DBG.H"
// .\win_mom\src
#include "..\..\MoM_Rasm\STU\STU_DBG.C"
#include "..\..\MoM_Rasm\STU\STU_UTIL.C"   /* get_datetime() */
#endif


static bool g_KbHit;



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

unsigned char video_back_buffer[64000];


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
Render_VBB(win32_offscreen_buffer *Buffer)
{
    u8 * bbuf;
    bbuf = (u8 *)video_back_buffer;
    struct tagRGBQUAD XBGR;
// typedef struct tagRGBQUAD {
//         BYTE    rgbBlue;
//         BYTE    rgbGreen;
//         BYTE    rgbRed;
//         BYTE    rgbReserved;
// } RGBQUAD;

//     u8 *Row = (u8 *)Buffer->Memory;
//     for (int Y = 0; Y < Buffer->Height; ++Y)
//     {
//         u32* Pixel = (u32*)Row;
//         for(int X = 0; X < Buffer->Width; ++X)
//         {
//             // BLACK  *Pixel++ = *bbuf++;
// 
//             // CRASH  u8 Red = *bbuf++;
//             // CRASH  u8 Green = *bbuf++;
//             // CRASH  u8 Blue = *bbuf++;
//             // CRASH  u8 Reserved = *bbuf++;
//             // CRASH  *Pixel++ = Red << 16 | Green << 8 | Blue; // << 0
//         }
//         Row += Buffer->Pitch;
//     }

    u32* Pixel = (u32*)Buffer->Memory;
    int itr;
    unsigned char vbb_byte;
    unsigned int * p_XBGR;
    unsigned int long_XBGR;
    p_XBGR = (unsigned int *)p_Palette_XBGR;
    for(itr = 0; itr < 64000; itr++)
    {
        vbb_byte = *(video_back_buffer + itr);
        long_XBGR = *(p_XBGR + vbb_byte);
        // fwrite(&long_XBGR, sizeof(long_XBGR), 1, fileptr);
        *Pixel++ = long_XBGR;
    }

}

internal void
Win32ResizeDIBSection(win32_offscreen_buffer *Buffer, int Width, int Height)
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
Win32DisplayBufferInWindow(win32_offscreen_buffer *Buffer, 
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

//    HWND Window = CreateWindowExA(0, WindowClass.lpszClassName, "MoM Reassembly",
//                                  WS_OVERLAPPEDWINDOW | WS_VISIBLE,
//                                  CW_USEDEFAULT, CW_USEDEFAULT,
//                                  CW_USEDEFAULT, CW_USEDEFAULT,
//                                  0, 0, Instance, 0);
    HWND Window = CreateWindowExA(0, WindowClass.lpszClassName, "MoM Reassembly",
                                  WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                  0, 0,
                                  320, 200,
                                  0, 0, Instance, 0);



    /*
        BEGIN: Pre-MainLoop
    */

    // Set Screen Page Index
    // Set Screen Page Address
    // Clear Screen - Back-Buffer
    // Clear Screen - Front-Buffer
    // Load Font File
    // Load Palette
    // Load 

#ifdef STU_DEBUG
    Debug_Log_Startup();
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] video_back_buffer: %p\n", __FILE__, __LINE__, video_back_buffer);
#endif

    char font_file[] = GAME_FONT_FILE;
    Load_Font_File(font_file);
    Load_Palette(0, -1, 0);
    Main_Menu_Load_Pictures();
    // FLIC_Draw(int x_start, int y_start, SAMB_ptr p_FLIC_File)
    FLIC_Draw(0, 0, mainmenu_top);
    FLIC_Draw(0, 41, mainmenu_bot);
    int menu_x_start = 123;
    int menu_y_start = 141;
    int menu_shift = 0;
    FLIC_Draw(menu_x_start, (menu_y_start + 12), mainmenu_l);
    FLIC_Draw(menu_x_start, (menu_y_start + (12 * menu_shift)), mainmenu_c);
    FLIC_Draw(menu_x_start, (menu_y_start + 24), mainmenu_n);
    FLIC_Draw(menu_x_start, (menu_y_start + 36), mainmenu_h);
    FLIC_Draw(menu_x_start, (menu_y_start + 48), mainmenu_q);

    STU_Export_VBB_To_BMP32();

#ifdef STU_DEBUG
    Debug_Log_Shutdown();
#endif

    /*
        END: Pre-MainLoop
    */



    // Win32ResizeDIBSection(&GlobalBackbuffer, 1280, 720);
    Win32ResizeDIBSection(&GlobalBackbuffer, 320, 200);

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

        // RenderWeirdGradient(&GlobalBackbuffer, XOffset, YOffset);
        // ++XOffset;
        Render_VBB(&GlobalBackbuffer);

        win32_window_dimension Dimension = Win32GetWindowDimension(Window);
        Win32DisplayBufferInWindow(&GlobalBackbuffer, DeviceContext, Dimension.Width, Dimension.Height);

        // MainLoop();

    }
        
    return 0;
}
