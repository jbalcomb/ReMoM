
#include "Platform.h"

#include "sdl2_PFL.h"

#include <SDL.h>

#include <stdlib.h>  /* CLAUDE: getenv() for the HW-cursor gate */
#include <string.h>  /* CLAUDE: strcmp() */



/* CLAUDE: synthetic (injected) mouse button for HMS/replay click playback — see Platform.h. */
static int16_t sdl2_synthetic_mouse_button = 0;
static int16_t sdl2_synthetic_mouse_button_hold = 0;

void Platform_Set_Synthetic_Mouse_Button(int16_t buttons)
{
    sdl2_synthetic_mouse_button = buttons;
    sdl2_synthetic_mouse_button_hold = (buttons != 0) ? PLATFORM_SYNTHETIC_MOUSE_HOLD : 0;
}

int16_t Platform_Get_Mouse_Button_State(void)
{
    Uint32 sdl2_mouse_state = 0;
    int16_t l_mouse_button = 0;

    if(!platform_mouse_input_enabled)
    {
        return 0;
    }

    sdl2_mouse_state = SDL_GetMouseState(NULL, NULL);

    if(sdl2_mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT))
    {
        l_mouse_button = ST_LEFT_BUTTON;
    }
    if(sdl2_mouse_state & SDL_BUTTON(SDL_BUTTON_RIGHT))
    {
        l_mouse_button = ST_RIGHT_BUTTON;
    }

    /* CLAUDE: OR in the synthetic press, returning it BEFORE decrementing the hold so
       this read still sees it; auto-release when the hold expires so the caller's
       release spin-wait exits. */
    if(sdl2_synthetic_mouse_button != 0)
    {
        l_mouse_button |= sdl2_synthetic_mouse_button;
        if(sdl2_synthetic_mouse_button_hold > 0)
        {
            sdl2_synthetic_mouse_button_hold--;
            if(sdl2_synthetic_mouse_button_hold == 0)
            {
                sdl2_synthetic_mouse_button = 0;
            }
        }
    }

    return l_mouse_button;
}


void Platform_Warp_Mouse(int16_t game_x, int16_t game_y)
{
    float screen_scale = (float)sdl2_window_width / (float)PLATFORM_SCREEN_WIDTH;
    /* CLAUDE */  Platform_Set_Warp_Guard();  /* prevent Platform_Maybe_Move_Mouse() from reading stale OS position */
    SDL_WarpMouseInWindow(sdl2_window, (int)(game_x * screen_scale), (int)(game_y * screen_scale));
}


int Platform_Get_Window_Width(void)
{
    return sdl2_window_width;
}


/* Mouse_Button(), Mouse_Movement_Handler(), Mouse_Button_Handler(), User_Mouse_Handler(), Set_Mouse_Position() moved to MoX/src/Mouse.c */


/* ========================================================================= */
/*  CLAUDE: Hardware-cursor prototype (Platform-Input follow-on).            */
/*                                                                           */
/*  Baseline (doc/PlatformLayer/input-metrics/baseline_2026-07-24_x11_scale4 */
/*  .fwv) showed the software cursor only reaches the screen at the ~18 fps   */
/*  present cadence.  Here we build a real SDL cursor from the current game   */
/*  cursor sprite so the compositor tracks it at native rate, decoupled from  */
/*  the framebuffer present -- the modern equivalent of the DOS hardware      */
/*  sprite.  Opt-in via REMOM_HW_CURSOR=1.                                    */
/* ========================================================================= */

/* Sprite geometry for the shipped 320x200 build.  These mirror MOM_DEF.h CURSOR_WIDTH/HEIGHT and
   MOX_DEF.h ST_TRANSPARENT; a production version should thread those macros through the interface
   rather than restating them.  Both cursor dimensions are 16 and the transparent index is 0. */
#define HWCUR_DIM          16
#define HWCUR_TRANSPARENT  0

static SDL_Cursor * hwcur_current = NULL;
static int          hwcur_gate = -1;   /* -1 = unresolved, else 0/1 from the env var */
static int          hwcur_last_image = -1;

int Platform_HW_Cursor_Active(void)
{
    if(hwcur_gate < 0)
    {
        const char * env = getenv("REMOM_HW_CURSOR");
        /* CLAUDE: default ON -- the hardware cursor is the shipped behaviour; only REMOM_HW_CURSOR=0 forces the software cursor. */
        hwcur_gate = (env != NULL && strcmp(env, "0") == 0) ? 0 : 1;
    }
    return hwcur_gate;
}

void Platform_HW_Cursor_Refresh(void)
{
    int image;
    int scale;
    int px, py, sx, sy;
    const uint8_t * sprite;
    SDL_Surface * surf;
    SDL_Cursor * newcur;
    uint32_t * dst;
    int pitch_px;

    if(!Platform_HW_Cursor_Active()) { return; }

    image = Get_Pointer_Image_Number();
    if(image == hwcur_last_image) { return; }   /* no shape change since the last build */
    hwcur_last_image = image;

    if(image == 0)   /* crsr_None */
    {
        SDL_ShowCursor(SDL_DISABLE);
        return;
    }
    if(mouse_palette == NULL)   /* sprite bank not loaded yet (pre-Load_Palette) -- retry next frame */
    {
        hwcur_last_image = -1;
        return;
    }

    scale = sdl2_window_width / PLATFORM_SCREEN_WIDTH;
    if(scale < 1) { scale = 1; }

    surf = SDL_CreateRGBSurfaceWithFormat(0, HWCUR_DIM * scale, HWCUR_DIM * scale, 32, SDL_PIXELFORMAT_ARGB8888);
    if(surf == NULL) { return; }

    sprite = mouse_palette + ((image - 1) * (HWCUR_DIM * HWCUR_DIM));
    dst = (uint32_t *)surf->pixels;
    pitch_px = surf->pitch / 4;

    for(py = 0; py < HWCUR_DIM; py++)
    {
        for(px = 0; px < HWCUR_DIM; px++)
        {
            uint8_t idx = sprite[px * HWCUR_DIM + py];   /* column-major, matching Draw_Mouse_On_Page_ */
            uint32_t argb;
            if(idx == HWCUR_TRANSPARENT)
            {
                argb = 0x00000000u;
            }
            else
            {
                PFL_Color c = platform_palette_buffer[idx];
                argb = ((uint32_t)0xFFu << 24) | ((uint32_t)c.r << 16) | ((uint32_t)c.g << 8) | (uint32_t)c.b;
            }
            /* nearest-neighbour upscale by the window scale so the OS cursor matches the drawn size */
            for(sy = 0; sy < scale; sy++)
            {
                for(sx = 0; sx < scale; sx++)
                {
                    dst[(py * scale + sy) * pitch_px + (px * scale + sx)] = argb;
                }
            }
        }
    }

    /* Hotspot (0,0): the game treats the pointer position as the sprite's top-left for hit-testing
       (Draw_Mouse_On_Page_ draws at x,y with no offset), so a top-left hotspot lands clicks identically. */
    newcur = SDL_CreateColorCursor(surf, 0, 0);
    SDL_FreeSurface(surf);
    if(newcur != NULL)
    {
        SDL_ShowCursor(SDL_ENABLE);
        SDL_SetCursor(newcur);
        if(hwcur_current != NULL) { SDL_FreeCursor(hwcur_current); }
        hwcur_current = newcur;
    }
}
