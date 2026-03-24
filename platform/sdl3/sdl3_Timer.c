
#include "Platform.h"

#include <SDL3/SDL.h>



// MS-DOS  INT 1A,0 - Read System Clock Counter
// MWA  GetTickCount64()
// SDL3  SDL_GetTicks()
// 1oom  hw_get_time_us()
/*
    the number of seconds since the SDL library initialized
*/
uint64_t Read_System_Clock_Timer(void)
{
    return SDL_GetTicks() * 1000;  // the number of milliseconds since the SDL library initialized
}


/* Mark_Time(), Release_Time(), Release_Time_Seconds() moved to MoX/src/Timer.c */
