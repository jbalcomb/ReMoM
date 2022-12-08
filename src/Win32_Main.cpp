
#include "MoX_TYPE.H"

#include "MoM_main.H"

#include "Win32.hpp"

#include <windows.h>
#include <stdint.h>


#define STU_DEBUG 1
#ifdef STU_DEBUG
// #include "J:\STU\devel\STU-MoM_Rasm\MoM_Rasm\STU\STU_DBG.H"
// // .\win_mom\src
// #include "..\src\STU\STU_DBG.C"
// #include "..\src\STU\STU_UTIL.C"   /* get_datetime() */
#include "STU_DBG.H"
#endif

// global_variable win32_offscreen_buffer GlobalBackbuffer;
win32_offscreen_buffer GlobalBackbuffer;



void Render_VBB(win32_offscreen_buffer * Buffer)
{
    uint8_t * bbuf;
    bbuf = (uint8_t *)g_Video_Back_Buffer;
    struct tagRGBQUAD XBGR;

    uint32_t * Pixel = (uint32_t*)Buffer->Memory;
    int itr;
    unsigned char vbb_byte;
    unsigned int * p_XBGR;
    unsigned int long_XBGR;
    p_XBGR = (uint32_t *)g_Palette_XBGR;
    for(itr = 0; itr < 64000; itr++)
    {
        vbb_byte = *(g_Video_Back_Buffer + itr);
        long_XBGR = *(p_XBGR + vbb_byte);
        *Pixel++ = long_XBGR;
    }

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


void Main_Game_Loop(void)
{
    while(g_State_Run)
    {
        Poll_Messages();

        // Screen_Control();

        // Copy Back-Buffer to Front-Buffer
        Render_VBB(&GlobalBackbuffer);

        win32_window_dimension Dimension = Win32GetWindowDimension(g_Window);
        Win32DisplayBufferInWindow(&GlobalBackbuffer, g_DeviceContext, Dimension.Width, Dimension.Height);
    }
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
#ifdef STU_DEBUG
    Debug_Log_Startup();
#endif

    // Initialize the "Windows Desktop Application"
    // ~== 'Application-Type' of 'Game'
    Init_WDA_Game(hInstance, hPrevInstance, lpCmdLine, nShowCmd);

    Win32ResizeDIBSection(&GlobalBackbuffer, 320, 200);

    /*
        BEGIN: Initialize Game State
    */

    g_State_Run = 1;  // ST_TRUE
    g_Current_Screen = scr_Main_Menu;


    MoM_main();

    /*
        END: Initialize Game State
    */

    Main_Game_Loop();


#ifdef STU_DEBUG
    Debug_Log_Shutdown();
#endif

    return 0;
}
