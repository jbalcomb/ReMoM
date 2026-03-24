/**
 * win_CLK.c — Win32 timer / clock functions.
 *
 * Implements Platform_Get_Millies(), Platform_Sleep_Millies(), and
 * Read_System_Clock_Timer() using the Win32 API.
 *
 * Game-level Mark_Time(), Release_Time(), Release_Time_Seconds() live
 * in MoX/src/Timer.c and call these Platform_* functions.
 */

#include "../../platform/include/Platform.h"
#include "win_PFL.h"

#include <Windows.h>



uint64_t Platform_Get_Millies(void)
{
    return (uint64_t)GetTickCount64() - win_ticks_startup;
}

void Platform_Sleep_Millies(uint64_t ms)
{
    Sleep((DWORD)ms);
}

/*
    ~== INT 1A,0 — Read System Clock Timer
    Returns a BIOS-style tick count (~18.2 Hz) derived from wall-clock milliseconds.

    ~DOS — System Clock Counter — incremented approximately 18.206 times per second
    resolution of the standard DOS clock, approximately 54.925 milliseconds.
*/
uint64_t Read_System_Clock_Timer(void)
{
    uint64_t ms = (uint64_t)GetTickCount64();
    return ms / 55;  /* ~54.925 ms per BIOS timer tick, truncate to integer division */
}
