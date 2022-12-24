
#include "MoX_TYPE.H"

#include "MoM_main.H"

#include "MoM.cpp"

#include "Win32.hpp"

#include <windows.h>
#include <stdint.h>
#include <stdio.h>      /* sprintf() */

#define STU_DEBUG 1
#ifdef STU_DEBUG
// #include "J:\STU\devel\STU-MoM_Rasm\MoM_Rasm\STU\STU_DBG.H"
// // .\win_mom\src
// #include "..\src\STU\STU_DBG.C"
// #include "..\src\STU\STU_UTIL.C"   /* get_datetime() */
#include "STU_DBG.H"
#endif

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


// global_variable win32_offscreen_buffer GlobalBackbuffer;
win32_offscreen_buffer GlobalBackbuffer;
global_variable s64 GlobalPerfCountFrequency;  // HMH Day 18

// uint16_t window_width = 320;
// uint16_t window_height = 200;
uint16_t window_width = 640;
uint16_t window_height = 400;
// uint16_t window_width = 1280;
// uint16_t window_height = 800;



// HMH Day 18
inline LARGE_INTEGER
Win32GetWallClock()
{
    LARGE_INTEGER Result;
    QueryPerformanceCounter(&Result);
    return (Result);
}

// HMH Day 18
inline f32
Win32GetSecondsElapsed(LARGE_INTEGER Start, LARGE_INTEGER End)
{
    f32 Result = (f32)(End.QuadPart - Start.QuadPart) /
                 (f32)GlobalPerfCountFrequency;
    
    return (Result);
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


int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
#ifdef STU_DEBUG
    Debug_Log_Startup();
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: WinMain()\n", __FILE__, __LINE__);
#endif

    // Initialize the "Windows Desktop Application"
    // ~== 'Application-Type' of 'Game'
    Init_WDA_Game(hInstance, hPrevInstance, lpCmdLine, nShowCmd);

    // Getting the performance frequency.
    LARGE_INTEGER PerfCountFrequencyResult;
    QueryPerformanceFrequency(&PerfCountFrequencyResult);
    // int64_t PerfCountFrequency = PerfCountFrequencyResult.QuadPart;
    GlobalPerfCountFrequency = PerfCountFrequencyResult.QuadPart;

    // NOTE: Set the Windows scheduler granularity to 1ms
    // so that our Sleep() can be more granular
    UINT DesiredSchedulerMS = 1;
    b32 SleepIsGranular = (timeBeginPeriod(DesiredSchedulerMS) == TIMERR_NOERROR);

    Win32ResizeDIBSection(&GlobalBackbuffer, 320, 200);
    // Win32ResizeDIBSection(&GlobalBackbuffer, 640, 400);

    // TODO(casey): How do we reliably query on this on Windows?
    int MonitorRefreshHz = 60;
    // int GameUpdateHz = MonitorRefreshHz / 2;  // 30 FPS
    // int GameUpdateHz = MonitorRefreshHz / 4;  // 15 FPS
    // int GameUpdateHz = MonitorRefreshHz / 6;  // 10 FPS
    int GameUpdateHz = MonitorRefreshHz / 12;  // 5 FPS
    f32 TargetSecondsPerFrame = 1.0f / (f32)GameUpdateHz;

    /*
        BEGIN: Initialize Game State
    */

    g_State_Run = 1;  // ST_TRUE
    g_Current_Screen = scr_Main_Menu;

    video_page_buffer[0] = (uint8_t *)VirtualAlloc(NULL, (320*200*1), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    video_page_buffer[1] = (uint8_t *)VirtualAlloc(NULL, (320*200*1), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    draw_page_num = 0;
    draw_page = video_page_buffer[draw_page_num];

    MoM_main();

    // // Copy Back-Buffer to Front-Buffer
    // Render_VBB(&GlobalBackbuffer);
    win32_window_dimension Dimension = Win32GetWindowDimension(g_Window);
    Win32DisplayBufferInWindow(&GlobalBackbuffer, g_DeviceContext, Dimension.Width, Dimension.Height);

    /*
        END: Initialize Game State
    */

    /*
        Main Game Loop
    */
    // LARGE_INTEGER LastCounter;
    // QueryPerformanceCounter(&LastCounter);
    LARGE_INTEGER LastCounter = Win32GetWallClock();
    uint64_t LastCycleCount = __rdtsc();

    while(g_State_Run)
    {
        Poll_Messages();

        Screen_Control();

        game_offscreen_buffer Buffer = {}; // clear to zero!
        Buffer.Memory = GlobalBackbuffer.Memory;
        Buffer.Width = GlobalBackbuffer.Width;
        Buffer.Height = GlobalBackbuffer.Height;
        Buffer.Pitch = GlobalBackbuffer.Pitch;
        // TODO  buffer.Pitch = buffer.width * BYTES_PER_PIXEL;

        // // Copy Back-Buffer to Front-Buffer
        // Render_VBB(&GlobalBackbuffer);
        win32_window_dimension Dimension = Win32GetWindowDimension(g_Window);
        GameUpdateAndRender(&Buffer);
        Win32DisplayBufferInWindow(&GlobalBackbuffer, g_DeviceContext, Dimension.Width, Dimension.Height);

// HMH Day 10
// HMH Day 10          LARGE_INTEGER EndCounter;
// HMH Day 10          QueryPerformanceCounter(&EndCounter);
// HMH Day 10          uint64_t EndCycleCount = __rdtsc();
// HMH Day 10          // Calculating the time difference between the end of the previous frame, and the end of the current frame.
// HMH Day 10          int64_t CounterElapsed = EndCounter.QuadPart - LastCounter.QuadPart;
// HMH Day 10          int64_t CyclesElapsed = EndCycleCount - LastCycleCount;
// HMH Day 10          // Calculating milliseconds per frame.
// HMH Day 10          // int32_t MSPerFrame = (s32)((1000*CounterElapsed) / PerfCountFrequency);
// HMH Day 10          // int32_t FPS = (s32)(PerfCountFrequency / CounterElapsed);
// HMH Day 10          // int32_t MegaCyclesPerFrame = CyclesElapsed / (1000 * 1000);
// HMH Day 10          f32 MSPerFrame = 1000.0f*(f32)CounterElapsed / (f32)PerfCountFrequency;
// HMH Day 10          f32 FPS = (f32)PerfCountFrequency / (f32)CounterElapsed;
// HMH Day 10          f32 MegaCyclesPerFrame = (f32)CyclesElapsed / (1000.0f * 1000.0f);
// HMH Day 10  #if 0
// HMH Day 10          char Buffer[256];
// HMH Day 10          // wsprintfA(Buffer, "ms/frame: %dms\n", MSPerFrame);
// HMH Day 10          // wsprintfA(Buffer, "ms/frame: %dms / %dFPS\n", MSPerFrame, FPS);
// HMH Day 10          // wsprintfA(Buffer, "%dms/f, %df/s, %dMc/f \n", MSPerFrame, FPS, MegaCyclesPerFrame);
// HMH Day 10          // sprintf(Buffer, "%dms/f, %df/s, %dMc/f \n", MSPerFrame, FPS, MegaCyclesPerFrame);
// HMH Day 10          // sprintf(Buffer, "%fms/f, %ff/s, %fMc/f\n", MSPerFrame, FPS, MegaCyclesPerFrame);
// HMH Day 10          sprintf(Buffer, "%.02fms/f, %.02ff/s, %.02fMc/f\n", MSPerFrame, FPS, MegaCyclesPerFrame);
// HMH Day 10          OutputDebugStringA(Buffer);
// HMH Day 10  #endif
// HMH Day 10          LastCounter = EndCounter;
// HMH Day 10          LastCycleCount = EndCycleCount;

// HMH Day 18

        // LARGE_INTEGER EndCounter;
        // QueryPerformanceCounter(&EndCounter);

        // s64 CounterElapsed = EndCounter.QuadPart - LastCounter.QuadPart;
        // f32 SecondsElapsedForWork = (f32)CounterElapsed / (f32)PerfCountFrequency;
        LARGE_INTEGER WorkCounter = Win32GetWallClock();
        f32 WorkSecondsElapsed = Win32GetSecondsElapsed(LastCounter, WorkCounter);
        // ...wait until we're over our target...
        // the simplest, CPU-melting, solution - simply sit in a while loop
        // f32 SecondsElapsedForFrame = SecondsElapsedForWork;
        f32 SecondsElapsedForFrame = WorkSecondsElapsed;
        if (SecondsElapsedForFrame < TargetSecondsPerFrame)
        {
            while (SecondsElapsedForFrame < TargetSecondsPerFrame)
            {
                // QueryPerformanceCounter(&EndCounter);
                // CounterElapsed = EndCounter.QuadPart - LastCounter.QuadPart;
                // SecondsElapsedForFrame = (f32)CounterElapsed / (f32)PerfCountFrequency;
                DWORD SleepMS = (DWORD)(1000.0f * (TargetSecondsPerFrame - SecondsElapsedForFrame));
                if (SleepMS > 0)
                {
                    Sleep(SleepMS);
                }
                SecondsElapsedForFrame = Win32GetSecondsElapsed(LastCounter, Win32GetWallClock());
            }
        }
        else
        {
            // TODO(casey): MISSED FRAME RATE!
            // TODO(casey): Logging
        }

        #if 0
        // debug timing output

        f32 MSPerFrame = 1000.0f * (f32)CounterElapsed / (f32)PerfCountFrequency;
        f32 FPS = (f32)PerfCountFrequency / (f32)CounterElapsed;
        f32 MegaCyclesPerFrame = (f32)CyclesElapsed / (1000.0f * 1000.0f);

        char Buffer[256];
        sprintf(Buffer, "%.02fms/f, %.02ff/s, %.02fMc/f\n", MSPerFrame, FPS, MegaCyclesPerFrame);
        OutputDebugStringA(Buffer);
        #endif

        // ? HMH Day 10 ?  LastCounter = EndCounter;
        // ? HMH Day 10 ?  LastCycleCount = EndCycleCount;

        // ?  game_input *Temp = NewInput;
        // ?  NewInput = OldInput;
        // ?  OldInput = Temp;

        LARGE_INTEGER EndCounter = Win32GetWallClock();
        f32 MSPerFrame = 1000.0f * Win32GetSecondsElapsed(LastCounter, EndCounter);
        LastCounter = EndCounter;

        u64 EndCycleCount = __rdtsc();
        s64 CyclesElapsed = EndCycleCount - LastCycleCount;
        LastCycleCount = EndCycleCount;

#if 1
        // debug timing output
        f32 FPS = 0.0f; // To be fixed later
        f32 MegaCyclesPerFrame = (f32)CyclesElapsed / (1000.0f * 1000.0f);

        char FPSBuffer[256];
        sprintf_s(FPSBuffer, sizeof(FPSBuffer), "%.02fms/f, %.02ff/s, %.02fMc/f\n", MSPerFrame, FPS, MegaCyclesPerFrame);
        OutputDebugStringA(FPSBuffer);
#endif

    }


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: WinMain()\n", __FILE__, __LINE__);
#endif

#ifdef STU_DEBUG
    Debug_Log_Shutdown();
#endif

    return 0;
}
