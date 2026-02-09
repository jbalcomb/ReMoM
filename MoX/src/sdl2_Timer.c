#include "../../MoX/src/Mouse.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_TYPE.h"
#include "../../MoX/src/Timer.h"
#include "../../MoX/src/Video.h"

#include "MOM_PFL.h"

#include "sdl2_PFL.h"

#include <SDL.h>



void Release_Time_Seconds(uint32_t delay);

// uint32_t tick_count;

// ~== INT 1A,0 - Read System Clock Timer
// uint32_t Get_System_Clock_Counter(void)

// WZD s02p01
// void Mark_Time(void)

// WZD s02p02
// uint32_t Time_Passed(void)

// WZD s02p03
// void Release_Time(int ticks)

/* -------------------------------------------------------------------------- */

// MS-DOS  INT 1A,0 - Read System Clock Counter
// MWA  GetTickCount64()
// SDL2  SDL_GetTicks64()
// 1oom  hw_get_time_us()
/*
    the number of seconds since the SDL library initialized
*/
uint64_t Read_System_Clock_Timer(void)
{
    return SDL_GetTicks64() * 1000;  // the number of milliseconds since the SDL library initialized
}

/* -------------------------------------------------------------------------- */

#define DELAY_EVENT_HANDLE_LIMIT    12500
#define DELAY_MOUSE_UPDATE_LIMIT    20000

// static uint32_t delay_start;
static uint64_t delay_start;

/* -------------------------------------------------------------------------- */


// WZD s02p01
// ui_delay_prepare()
void Mark_Time(void)
{
    sdl2_ticks_mark_time = SDL_GetTicks64();  // the number of milliseconds since SDL library initialization
    // DELETEME  dbg_prn("sdl2_ticks_mark_time: %llu\n", sdl2_ticks_mark_time);
    // DELETEME  dbg_prn("(sdl2_ticks_mark_time - sdl2_ticks_startup): %llu\n", (sdl2_ticks_mark_time - sdl2_ticks_startup));

    delay_start = Read_System_Clock_Timer();
}

// WZD s02p02
// uint32_t Time_Passed(void);

// WZD s02p03
// ui_delay_ticks_or_click()
// 110 calls-sites, return-value never used?  bool Release_Time(int ticks)
void Release_Time(int ticks)
{
    // // return Release_Time_Seconds(MOO_TICKS_TO_US(ticks));
    // // #define MOO_TICKS_TO_US(_t_) (((_t_) * 5_000_000) / 91)
    // Release_Time_Seconds(MOO_TICKS_TO_US(ticks));

    // double elapsed_seconds = (double)(end - start) / SDL_GetPerformanceFrequency();
    // double elapsed_microseconds = elapsed_seconds * 1000000;

//     uint64_t now;
//     int32_t diff;
//     now = Read_System_Clock_Timer();
//     diff = now - delay_start;
//     uint32_t current_tick_count = 0;
//     do
//     {
//         current_tick_count = Read_System_Clock_Timer();
//     } while(current_tick_count < tick_count + ticks);

    uint64_t sdl2_tick_count = 0;
    uint64_t sdl2_tick_end = 0;

    sdl2_ticks_release_time = Platform_Get_Millies();  // the number of milliseconds since SDL library initialization
    // DELETEME  dbg_prn("sdl2_ticks_release_time: %llu\n", sdl2_ticks_release_time);
    // DELETEME  dbg_prn("(sdl2_ticks_release_time - sdl2_ticks_startup): %llu\n", (sdl2_ticks_release_time - sdl2_ticks_startup));

    sdl2_tick_end = (sdl2_ticks_mark_time + (ticks * MILLISECONDS_PER_FRAME));  // ~ IBM-PC - 55 ms per BIOS timer tick

    do
    {
        sdl2_tick_count = Platform_Get_Millies();
    }
    while(sdl2_tick_count < (sdl2_ticks_mark_time + (ticks * MILLISECONDS_PER_FRAME)));

    sdl2_frame_count += 1;
    // DELETEME  dbg_prn("sdl2_frame_count: %llu\n", sdl2_frame_count);

    // cap the frame rate by *sleeping* for the remaining frame time

// This is where we do the actual frame rate capping.
// When we started the frame, we started a timer to keep track of how much time it took to output this frame. In order for this program not to run too fast, each frame must take a certain amount of time. Since 20 frames are being shown per second, each frame must take no less than 1/20th of a second. If the frame rate is at 60 FPS, each frame must take no less than 1/60th of a second. Since this demo is running at 20 FPS, that means we should spend 50 milliseconds (1000 milliseconds / 20 frames) per frame.
// To regulate the frame rate, first we check if the frame timer is less than the time allowed per frame. If it's more, it means we're either on time or behind schedule so we don't have time to wait. If it is less, then we use SDL_Delay() to sleep the rest of the frame time.
// So if the frame timer in this program is at 20 milliseconds we sleep for 30 milliseconds. If the frame timer is at 40 milliseconds, we sleep for 10 milliseconds, etc, etc.

    int itr_ticks;
    uint64_t current_millies;
    for(itr_ticks = ticks; itr_ticks > 0; itr_ticks--)
    {

        do
        {
            
            current_millies = Platform_Get_Millies();

            // (MAYBE) MOVE MOUSE
            Platform_Maybe_Move_Mouse();

            // CAP FRAME-RATE
            if((Platform_Get_Millies() - sdl2_ticks_mark_time) < 1000 / FRAMES_PER_SECOND)
            {
                Platform_Sleep_Millies((1000 / FRAMES_PER_SECOND) - (Platform_Get_Millies() - sdl2_ticks_mark_time));
            }

        }
        while(sdl2_tick_count < (sdl2_ticks_mark_time + (itr_ticks * MILLISECONDS_PER_FRAME)));

        sdl2_frame_count += 1;
        // DELETEME  dbg_prn("sdl2_frame_count: %llu\n", sdl2_frame_count);
    
    }
}

// ui_delay_us_or_click()
void Release_Time_Seconds(uint32_t delay)
{
    bool pressed;
    bool handled;
    int mx;
    int my;
    uint32_t mouse_time;
    int32_t diff;
    uint32_t now;

    pressed = false;
    handled = false;
    mx = pointer_x;
    my = pointer_y;

    mouse_time = (uint32_t)Read_System_Clock_Timer();
    
    while(1)
    {

        now = (uint32_t)Read_System_Clock_Timer();

        diff = (int32_t)(now - delay_start);  // TODO  DEDU  why/how diff data type of mouse_time, now, diff, delay, Read_System_Clock_Timer() 

        if(
            (diff < 0)
            ||
#pragma warning(suppress : 4018)  // TODO  signed/unsigned mismatch
            (diff >= delay)
        )
        {

            if(!handled)
            {
                Platform_Event_Handler();
            }

            return;

        }

        if(diff < DELAY_EVENT_HANDLE_LIMIT)
        {
            continue;
        }

        Platform_Event_Handler();
        handled = true;

        if(!pressed)
        {
            // ITRY  if(platform_mouse_button_status)
            if(Mouse_Button())
            {
                pressed = true;
            }
        }
        else
        {
            // ITRY  if(!platform_mouse_button_status)
            if(Mouse_Button())
            {
                return;
            }
        }

        if(
            ((mx != pointer_x) || (my != pointer_y))  /* ¿ mouse moved ? ¿ pointer_x,y got updated in Platform_Event_Handler() ?*/
            &&
            ((now - mouse_time) > DELAY_MOUSE_UPDATE_LIMIT)  /* ¿ 20,000 whats ?*/
            )
        {

            mouse_time = now;

            Toggle_Pages_No_Draw_Fields();

        }

    }

}
