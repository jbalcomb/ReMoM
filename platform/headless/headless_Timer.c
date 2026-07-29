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
static uint64_t headless_micros_startup = 0;



#ifdef _WIN32

static uint64_t Headless_Get_Ticks_Ms(void)
{
    return (uint64_t)GetTickCount64();
}

/* High-resolution counter for perf instrumentation.  GetTickCount64() above quantizes to the
 * ~15.6 ms system timer interval, which cannot resolve a sub-frame zone; QPC is sub-microsecond. */
static uint64_t Headless_Get_Ticks_Us(void)
{
    static LARGE_INTEGER freq = { 0 };
    LARGE_INTEGER now;

    if(freq.QuadPart == 0)
    {
        QueryPerformanceFrequency(&freq);
        if(freq.QuadPart == 0) { return Headless_Get_Ticks_Ms() * 1000u; }  /* no QPC: fall back, coarse but monotonic */
    }
    QueryPerformanceCounter(&now);
    /* Split the division so a multi-hour session cannot overflow the numerator. */
    return ((uint64_t)now.QuadPart / (uint64_t)freq.QuadPart) * 1000000u
         + (((uint64_t)now.QuadPart % (uint64_t)freq.QuadPart) * 1000000u) / (uint64_t)freq.QuadPart;
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

static uint64_t Headless_Get_Ticks_Us(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000 + (uint64_t)ts.tv_nsec / 1000;
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
    if(headless_ticks_startup == 0)
    {
        headless_ticks_startup = Headless_Get_Ticks_Ms();
    }
    return Headless_Get_Ticks_Ms() - headless_ticks_startup;
}

uint64_t Platform_Get_Micros(void)
{
    if(headless_micros_startup == 0)
    {
        headless_micros_startup = Headless_Get_Ticks_Us();
    }
    return Headless_Get_Ticks_Us() - headless_micros_startup;
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
