/**
 * headless_Timer.c — Headless timer / clock functions.
 *
 * Implements Platform_Get_Millies(), Platform_Sleep_Millies(), and
 * Read_System_Clock_Timer() using portable C / OS APIs.
 */

#include "../../platform/include/Platform.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <time.h>
#include <unistd.h>
#endif

static uint64_t headless_ticks_startup = 0;



#ifdef _WIN32

static uint64_t Headless_Get_Ticks_Ms(void)
{
    return (uint64_t)GetTickCount64();
}

void Platform_Sleep_Millies(uint64_t ms)
{
    Sleep((DWORD)ms);
}

#else  /* POSIX */

static uint64_t Headless_Get_Ticks_Ms(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000 + (uint64_t)ts.tv_nsec / 1000000;
}

void Platform_Sleep_Millies(uint64_t ms)
{
    struct timespec ts;
    ts.tv_sec = (time_t)(ms / 1000);
    ts.tv_nsec = (long)((ms % 1000) * 1000000);
    nanosleep(&ts, NULL);
}

#endif



uint64_t Platform_Get_Millies(void)
{
    if (headless_ticks_startup == 0)
    {
        headless_ticks_startup = Headless_Get_Ticks_Ms();
    }
    return Headless_Get_Ticks_Ms() - headless_ticks_startup;
}

/*
    ~== INT 1A,0 -- Read System Clock Timer
    Returns a BIOS-style tick count (~18.2 Hz) derived from wall-clock milliseconds.
*/
uint64_t Read_System_Clock_Timer(void)
{
    uint64_t ms = Headless_Get_Ticks_Ms();
    return ms / 55;  /* ~54.925 ms per BIOS timer tick */
}
