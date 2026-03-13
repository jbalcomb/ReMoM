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
    /* CLAUDE */  /* Original had two loops: a bare busy-wait (no event processing, ~55ms dead time) followed by a frame-cap loop that only ran once. */
    /* CLAUDE */  /* Replaced with a single loop that processes events and updates the cursor during the wait, so the mouse stays responsive. */

    uint64_t sdl2_tick_end;

    sdl2_ticks_release_time = Platform_Get_Millies();
    sdl2_tick_end = sdl2_ticks_mark_time + (ticks * MILLISECONDS_PER_FRAME);  /* ~ IBM-PC - 55 ms per BIOS timer tick */

    while(Platform_Get_Millies() < sdl2_tick_end)
    {
        /* CLAUDE */  SDL_PumpEvents();  /* keep SDL event queue alive without processing (Platform_Event_Handler draws cursor into buffer, conflicting with Platform_Maybe_Move_Mouse's save/draw/present/restore cycle) */
        Platform_Maybe_Move_Mouse();
        Platform_Sleep_Millies(1);
    }

    sdl2_frame_count += ticks;
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
