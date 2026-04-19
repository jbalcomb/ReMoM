/**
 * headless_PFL.c — Headless platform lifecycle, video, input, and mouse.
 *
 * Implements Platform.h functions as no-ops or minimal stubs for headless
 * operation. No window, no renderer, no audio. Suitable for CI, testing,
 * and replay-driven execution.
 */

#include "../../platform/include/Platform.h"
#include "../../platform/include/Platform_Keys.h"
#include "../../platform/include/Platform_Replay.h"

#include <stdio.h>



/* ========================================================================= */
/*  Synthetic Player Callback                                                */
/* ========================================================================= */

/**
 * Optional per-frame callback for synthetic input injection.
 * Registered by HeMoM via Platform_Register_Frame_Callback().
 * Called once per Platform_Event_Handler() when no replay is active.
 */
static void (*platform_frame_callback)(void) = NULL;

void Platform_Register_Frame_Callback(void (*callback)(void))
{
    platform_frame_callback = callback;
}



/* ========================================================================= */
/*  Lifecycle                                                                */
/* ========================================================================= */

void Startup_Platform(void)
{
    fprintf(stderr, "[headless] Platform started (no display, no audio)\n");
}

void Shutdown_Platform(void)
{
    fprintf(stderr, "[headless] Platform shutdown\n");
}

int Platform_Get_Scale(void)
{
    return 1;
}



/* ========================================================================= */
/*  Video (no-ops)                                                           */
/* ========================================================================= */

void Platform_Palette_Update(void)
{
    int itr;

    for (itr = 0; itr < 256; itr++)
    {
        platform_palette_buffer[itr].r = (*(current_palette + (itr * 3) + 0) << 2);
        platform_palette_buffer[itr].g = (*(current_palette + (itr * 3) + 1) << 2);
        platform_palette_buffer[itr].b = (*(current_palette + (itr * 3) + 2) << 2);
        platform_palette_buffer[itr].a = 0xFF;
    }
}

void Platform_Get_Palette_Color(uint8_t index, uint8_t *r, uint8_t *g, uint8_t *b)
{
    *r = platform_palette_buffer[index].r;
    *g = platform_palette_buffer[index].g;
    *b = platform_palette_buffer[index].b;
}

void Platform_Set_Palette_Color(uint8_t index, uint8_t r, uint8_t g, uint8_t b)
{
    platform_palette_buffer[index].r = r;
    platform_palette_buffer[index].g = g;
    platform_palette_buffer[index].b = b;
    platform_palette_buffer[index].a = 255;
}

void Platform_Video_Update(void)
{
    /* No display — framebuffer exists in memory but is never presented. */
}



/* ========================================================================= */
/*  Events / Input                                                           */
/* ========================================================================= */

static int headless_event_handler_first_call = 1;

void Platform_Event_Handler(void)
{
    /* Log input mode on first call so we can see which path is active. */
    if (headless_event_handler_first_call)
    {
        headless_event_handler_first_call = 0;
        if (Platform_Replay_Active())
        {
            fprintf(stderr, "[headless] Platform_Event_Handler: input mode = REPLAY (.RMR)\n");
        }
        else if (platform_frame_callback != NULL)
        {
            fprintf(stderr, "[headless] Platform_Event_Handler: input mode = SCENARIO (.hms synthetic player)\n");
        }
        else
        {
            fprintf(stderr, "[headless] Platform_Event_Handler: input mode = NONE (no input source)\n");
        }
    }

    platform_frame_mouse_buttons = 0;

    /* Replay: inject recorded frame instead of polling OS events. */
    if (Platform_Replay_Active())
    {
        if (Replay_Inject_Frame())
        {
            return;
        }
        /* Replay ended — fall through (no live input in headless). */
    }

    /* Synthetic player: inject scripted input when no replay is active. */
    if (platform_frame_callback != NULL)
    {
        platform_frame_callback();
    }

    /* Record: capture input state (including synthetic player actions). */
    if (Platform_Record_Active())
    {
        Replay_Capture_Frame();
    }
}

void Platform_Pump_Events(void)
{
    /* No OS event queue to pump. */
}

/* CLAUDE: Stubs for the Win32-only pump entry points referenced from MoX
 * Input.c / Video.c under #ifdef _STU_WIN.  These are needed when MoX is
 * built with _STU_WIN defined (because the windowed backend in this build is
 * Win32) but HeMoM links against the headless platform — without these the
 * link of HeMoM fails on unresolved Pump_Events / Pump_Paints. */
void Pump_Events(void) { /* no-op */ }
void Pump_Paints(void) { /* no-op */ }

void Platform_Mouse_Input_Enable(void)
{
    platform_mouse_input_enabled = ST_TRUE;
}

void Platform_Mouse_Input_Disable(void)
{
    platform_mouse_input_enabled = ST_FALSE;
}

void Hw_Textinput_Start(void)
{
    /* No IME in headless mode. */
}

void Hw_Textinput_Stop(void)
{
    /* No IME in headless mode. */
}



/* ========================================================================= */
/*  Mouse (stubs)                                                            */
/* ========================================================================= */

int16_t Platform_Get_Mouse_Button_State(void)
{
    return 0;
}

void Platform_Warp_Mouse(int16_t game_x, int16_t game_y)
{
    pointer_x = game_x;
    pointer_y = game_y;
}

int Platform_Get_Window_Width(void)
{
    /* Headless has no window.  Return the game's native screen width so that
       User_Mouse_Handler's scale calculation (window_width / SCREEN_WIDTH) comes
       out to 1.  This matches Platform_Get_Scale() == 1 for the headless backend. */
    return PLATFORM_SCREEN_WIDTH;
}

void Platform_Get_Mouse_Position_XY(int *mx, int *my)
{
    *mx = (int)pointer_x;
    *my = (int)pointer_y;
}

void Platform_Maybe_Move_Mouse(void)
{
    /* No cursor to draw in headless mode. */
}
