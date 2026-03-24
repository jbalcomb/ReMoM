
#include "Mouse.h"
#include "MOX_DEF.h"
#include "MOX_TYPE.h"
#include "Timer.h"
#include "Video.h"

#include "../../platform/include/Platform.h"

#include <stdbool.h>



void Release_Time_Seconds(uint32_t delay);


/* -------------------------------------------------------------------------- */

#define DELAY_EVENT_HANDLE_LIMIT    12500
#define DELAY_MOUSE_UPDATE_LIMIT    20000

static uint64_t delay_start;

static uint64_t timer_ticks_mark_time;
static uint64_t timer_frame_count;

/* -------------------------------------------------------------------------- */


// WZD s02p01
// ui_delay_prepare()
void Mark_Time(void)
{
    timer_ticks_mark_time = Platform_Get_Millies();

    delay_start = Read_System_Clock_Timer();
}

// WZD s02p02
// uint32_t Time_Passed(void);
// ; Unused in MoM
// ; returns the amount of counter increments since
// ; the last call to CLK_SaveCounter
// ; int __far Time_Passed_(void)
// proc Time_Passed_ far
// push    es
// mov     ax, 0
// mov     es, ax
// assume es:nothing
// mov     ax, [es:46Ch]
// sub     ax, [word ptr CLK_BIOS_Counter]
// pop     es
// assume es:nothing
// retf
// endp Time_Passed_
// MoO2
// Time_Passed_ proc near
// push    esi
// mov     esi, 46Ch
// lodsd
// sub     eax, timerticks_dw17C5DC        ; SAR: file_animation_opened_flag
// pop     esi
// retn
// Time_Passed_ endp


// WZD s02p03
// ui_delay_ticks_or_click()
// 110 calls-sites, return-value never used?  bool Release_Time(int ticks)
void Release_Time(int ticks)
{
    /* CLAUDE */  /* Original had two loops: a bare busy-wait (no event processing, ~55ms dead time) followed by a frame-cap loop that only ran once. */
    /* CLAUDE */  /* Replaced with a single loop that processes events and updates the cursor during the wait, so the mouse stays responsive. */

    uint64_t tick_end;

    tick_end = timer_ticks_mark_time + (ticks * PLATFORM_MILLISECONDS_PER_FRAME);  /* ~ IBM-PC - 55 ms per BIOS timer tick */

    while(Platform_Get_Millies() < tick_end)
    {
        /* CLAUDE */  Platform_Pump_Events();  /* keep event queue alive without processing (Platform_Event_Handler draws cursor into buffer, conflicting with Platform_Maybe_Move_Mouse's save/draw/present/restore cycle) */
        Platform_Maybe_Move_Mouse();
        Platform_Sleep_Millies(1);
    }

    timer_frame_count += ticks;
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
            ((mx != pointer_x) || (my != pointer_y))  /* mouse moved? pointer_x,y got updated in Platform_Event_Handler() */
            &&
            ((now - mouse_time) > DELAY_MOUSE_UPDATE_LIMIT)  /* 20,000 whats? */
            )
        {

            mouse_time = now;

            Toggle_Pages_No_Draw_Fields();

        }

    }

}
