
#include "Mox.H"

#include "Windows.h"

/*
https://learn.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-gettickcount

The return value is the number of milliseconds that have elapsed since the system was started.

The resolution of the GetTickCount function is limited to the resolution of the system timer, which is typically in the range of 10 milliseconds to 16 milliseconds.

The elapsed time is stored as a DWORD value. Therefore, the time will wrap around to zero if the system is run continuously for 49.7 days. To avoid this problem, use the GetTickCount64 function. Otherwise, check for an overflow condition when comparing times.

If you need a higher resolution timer, use a multimedia timer or a high-resolution timer.


1/70 = 0.01428571428571428571428571428571

~DOS - System Clock Counter - incremented approximately 18.206 times per second
resolution of the standard DOS clock, approximately 54.925 milliseconds.


// https://www.freebasic.net/forum/viewtopic.php?t=12985
The BIOS timer tick normally counts 65536 cycles of the 1193182 Hz PIT clock.
65535/60/60 times every second, or about 18.204167 times
1193182 / 65536 = 18.206512451171875


https://github.com/ya-mouse/dos-utils/blob/master/watt32/src/timer.c
lots of cleverness...
hi = PEEKL (0, BIOS_CLK);


https://randomascii.wordpress.com/2013/05/09/timegettime-versus-gettickcount/


https://stackoverflow.com/questions/76149260/synchronizing-a-game-loop-to-a-desired-number-of-ticks-per-second-with-the-win32

*/

uint32_t tick_count;

// ~== INT 1A,0 - Read System Clock Timer
uint32_t Get_System_Clock_Counter(void)
{
    uint32_t dos_tick_count;
    DWORD win_tick_count;
#pragma warning(suppress : 28159)
    win_tick_count = GetTickCount();  // TODO  "Consider using GetTickCount64() ..."
    // dos_tick_count = win_tick_count / (uint32_t)54.92540;  // TODO(JimBalcomb,20231115): maybe identify castings that are purposeful and/or reasonable... macro? IMA_UINT32
    // dos_tick_count = win_tick_count / (uint32_t)(54.92540 * 2.0);
    dos_tick_count = win_tick_count / (uint32_t)(54.92540 * 1.5);
    return dos_tick_count;
}

void Release_Time(int ticks)
{
    uint32_t current_tick_count;
    do
    {
        current_tick_count = Get_System_Clock_Counter();
    } while(current_tick_count < tick_count + ticks);
    
// TODO  // HMH Day 10
// TODO  // HMH Day 10          LARGE_INTEGER EndCounter;
// TODO  // HMH Day 10          QueryPerformanceCounter(&EndCounter);
// TODO  // HMH Day 10          uint64_t EndCycleCount = __rdtsc();
// TODO  // HMH Day 10          // Calculating the time difference between the end of the previous frame, and the end of the current frame.
// TODO  // HMH Day 10          int64_t CounterElapsed = EndCounter.QuadPart - LastCounter.QuadPart;
// TODO  // HMH Day 10          int64_t CyclesElapsed = EndCycleCount - LastCycleCount;
// TODO  // HMH Day 10          // Calculating milliseconds per frame.
// TODO  // HMH Day 10          // int32_t MSPerFrame = (s32)((1000*CounterElapsed) / PerfCountFrequency);
// TODO  // HMH Day 10          // int32_t FPS = (s32)(PerfCountFrequency / CounterElapsed);
// TODO  // HMH Day 10          // int32_t MegaCyclesPerFrame = CyclesElapsed / (1000 * 1000);
// TODO  // HMH Day 10          f32 MSPerFrame = 1000.0f*(f32)CounterElapsed / (f32)PerfCountFrequency;
// TODO  // HMH Day 10          f32 FPS = (f32)PerfCountFrequency / (f32)CounterElapsed;
// TODO  // HMH Day 10          f32 MegaCyclesPerFrame = (f32)CyclesElapsed / (1000.0f * 1000.0f);
// TODO  // HMH Day 10  #if 0
// TODO  // HMH Day 10          char Buffer[256];
// TODO  // HMH Day 10          // wsprintfA(Buffer, "ms/frame: %dms\n", MSPerFrame);
// TODO  // HMH Day 10          // wsprintfA(Buffer, "ms/frame: %dms / %dFPS\n", MSPerFrame, FPS);
// TODO  // HMH Day 10          // wsprintfA(Buffer, "%dms/f, %df/s, %dMc/f \n", MSPerFrame, FPS, MegaCyclesPerFrame);
// TODO  // HMH Day 10          // sprintf(Buffer, "%dms/f, %df/s, %dMc/f \n", MSPerFrame, FPS, MegaCyclesPerFrame);
// TODO  // HMH Day 10          // sprintf(Buffer, "%fms/f, %ff/s, %fMc/f\n", MSPerFrame, FPS, MegaCyclesPerFrame);
// TODO  // HMH Day 10          sprintf(Buffer, "%.02fms/f, %.02ff/s, %.02fMc/f\n", MSPerFrame, FPS, MegaCyclesPerFrame);
// TODO  // HMH Day 10          OutputDebugStringA(Buffer);
// TODO  // HMH Day 10  #endif
// TODO  // HMH Day 10          LastCounter = EndCounter;
// TODO  // HMH Day 10          LastCycleCount = EndCycleCount;
// TODO  
// TODO  // HMH Day 18
// TODO  
// TODO          // LARGE_INTEGER EndCounter;
// TODO          // QueryPerformanceCounter(&EndCounter);
// TODO  
// TODO          // s64 CounterElapsed = EndCounter.QuadPart - LastCounter.QuadPart;
// TODO          // f32 SecondsElapsedForWork = (f32)CounterElapsed / (f32)PerfCountFrequency;
// TODO          LARGE_INTEGER WorkCounter = Win32GetWallClock();
// TODO          f32 WorkSecondsElapsed = Win32GetSecondsElapsed(LastCounter, WorkCounter);
// TODO          // ...wait until we're over our target...
// TODO          // the simplest, CPU-melting, solution - simply sit in a while loop
// TODO          // f32 SecondsElapsedForFrame = SecondsElapsedForWork;
// TODO          f32 SecondsElapsedForFrame = WorkSecondsElapsed;
// TODO          if (SecondsElapsedForFrame < TargetSecondsPerFrame)
// TODO          {
// TODO              while (SecondsElapsedForFrame < TargetSecondsPerFrame)
// TODO              {
// TODO                  // QueryPerformanceCounter(&EndCounter);
// TODO                  // CounterElapsed = EndCounter.QuadPart - LastCounter.QuadPart;
// TODO                  // SecondsElapsedForFrame = (f32)CounterElapsed / (f32)PerfCountFrequency;
// TODO                  DWORD SleepMS = (DWORD)(1000.0f * (TargetSecondsPerFrame - SecondsElapsedForFrame));
// TODO                  if (SleepMS > 0)
// TODO                  {
// TODO                      Sleep(SleepMS);
// TODO                  }
// TODO                  SecondsElapsedForFrame = Win32GetSecondsElapsed(LastCounter, Win32GetWallClock());
// TODO              }
// TODO          }
// TODO          else
// TODO          {
// TODO              // TODO(casey): MISSED FRAME RATE!
// TODO              // TODO(casey): Logging
// TODO          }
// TODO  
// TODO          #if 0
// TODO          // debug timing output
// TODO  
// TODO          f32 MSPerFrame = 1000.0f * (f32)CounterElapsed / (f32)PerfCountFrequency;
// TODO          f32 FPS = (f32)PerfCountFrequency / (f32)CounterElapsed;
// TODO          f32 MegaCyclesPerFrame = (f32)CyclesElapsed / (1000.0f * 1000.0f);
// TODO  
// TODO          char Buffer[256];
// TODO          sprintf(Buffer, "%.02fms/f, %.02ff/s, %.02fMc/f\n", MSPerFrame, FPS, MegaCyclesPerFrame);
// TODO          OutputDebugStringA(Buffer);
// TODO          #endif
// TODO  
// TODO          // ? HMH Day 10 ?  LastCounter = EndCounter;
// TODO          // ? HMH Day 10 ?  LastCycleCount = EndCycleCount;
// TODO  
// TODO          // ?  game_input *Temp = NewInput;
// TODO          // ?  NewInput = OldInput;
// TODO          // ?  OldInput = Temp;
// TODO  
// TODO          LARGE_INTEGER EndCounter = Win32GetWallClock();
// TODO          f32 MSPerFrame = 1000.0f * Win32GetSecondsElapsed(LastCounter, EndCounter);
// TODO          LastCounter = EndCounter;
// TODO  
// TODO          u64 EndCycleCount = __rdtsc();
// TODO          s64 CyclesElapsed = EndCycleCount - LastCycleCount;
// TODO          LastCycleCount = EndCycleCount;
// TODO  
// TODO  #if 1
// TODO          // debug timing output
// TODO          f32 FPS = 0.0f; // To be fixed later
// TODO          f32 MegaCyclesPerFrame = (f32)CyclesElapsed / (1000.0f * 1000.0f);
// TODO  
// TODO          char FPSBuffer[256];
// TODO          sprintf_s(FPSBuffer, sizeof(FPSBuffer), "%.02fms/f, %.02ff/s, %.02fMc/f\n", MSPerFrame, FPS, MegaCyclesPerFrame);
// TODO          OutputDebugStringA(FPSBuffer);
// TODO  #endif

}

void Mark_Time(void)
{
    tick_count = Get_System_Clock_Counter();
}

uint32_t Time_Passed(void)
{
    return Get_System_Clock_Counter() - tick_count;
}

uint32_t Timer_Value(void)
{
    return Get_System_Clock_Counter();
}
