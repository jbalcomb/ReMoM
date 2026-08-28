
#include "Mouse.h"
#include "MOX_DEF.h"
#include "MOX_TYPE.h"
#include "Timer.h"
#include "Video.h"
#include "random.h"  /* g_random_call_count for CALL_TRACE */

#include "../../platform/include/Platform.h"
#include "../../platform/include/Platform_Perf.h"  /* CLAUDE: FR5a tick-aware accounting hook */
#include "../../STU/src/STU_LOG.h"  /* CALL_TRACE */

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
    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);
    timer_ticks_mark_time = Platform_Get_Millies();

    delay_start = Read_System_Clock_Timer();
    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);
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

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    tick_end = timer_ticks_mark_time + (ticks * PLATFORM_MILLISECONDS_PER_FRAME);  /* ~ IBM-PC - 55 ms per BIOS timer tick */

/* CLAUDE */  { uint64_t perf_wait_begin_ms = Platform_Get_Millies();

    while(Platform_Get_Millies() < tick_end)
    {
        /* CLAUDE */  Platform_Pump_Events();  /* pumps events AND refreshes cursor (polls OS position, redraws only if moved) */
        Platform_Sleep_Millies(1);
    }

    /* CLAUDE: FR5a tick-aware accounting (doc/@AI_Plans/PRD-Performance-Management.md).
     *
     * MEASUREMENT ONLY -- this reports, it does not change pacing.  The wait above is untouched:
     * same condition, same loop body, same exit.  Two clock reads and one call are added around it.
     *
     * Why it has to be here: Release_Time holds the image for N ticks WITHOUT presenting, so from
     * the outside a 3-tick hold is indistinguishable from one frame that overran by 110 ms.  Only
     * this function knows the difference, so only this function can report it.
     *
     * Both figures are reported.  `ticks` is the nominal advance.  The measured elapsed time is
     * what gets subtracted, because the loop above is `while(now < tick_end)` -- when logic has
     * already overrun past tick_end the body never runs and nothing is waited, while `ticks` is
     * unchanged.  Subtracting a nominal ticks * 55 ms there would invent a hold that never
     * happened and relabel a real overrun as idle.  Measured elapsed is ~0 in that case, which is
     * the truth. */
/* CLAUDE */  Perf_Note_Release_Time(ticks, Platform_Get_Millies() - perf_wait_begin_ms); }

    timer_frame_count += ticks;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);
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
