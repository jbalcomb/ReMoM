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

void Platform_Event_Handler(void)
{
    platform_frame_mouse_buttons = 0;

    /* During replay, the replay system in Replay.c handles injecting input. */
    /* In non-replay headless mode, there is simply no input. */
}

void Platform_Pump_Events(void)
{
    /* No OS event queue to pump. */
}

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
    return PLATFORM_WINDOW_WIDTH;
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
