
#include "Platform.h"
#include "Platform_Perf.h"  /* CLAUDE: live frame-time readout base title */

#include "sdl3_Audio.h"
#include "../include/Platform_Input_Metrics.h"  /* CLAUDE: Platform-Input Layer 1 metrics */

#include "../../STU/src/STU_DBG.h"
#include "../../STU/src/STU_LOG.h"

#include "sdl3_KD.h"
#include "sdl3_MOM.h"
#include "sdl3_PFL.h"

#include <SDL3/SDL.h>
#ifndef NO_SOUND_LIBRARY
#include <SDL3_mixer/SDL_mixer.h>
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>  /* CLAUDE: getenv() */
#include <string.h>  /* CLAUDE: strcmp() */

/*
    Platform-Layer Screen Buffer / Window Surface
*/

/*
    IBM-PC VGA DAC
    required for color-cycling - change what is displayed, without changing what is drawn

*/
// MOM_PFL.C  uint8_t platform_palette_buffer[1024];  // Platform's Shadow Palette: 256 colors * bits/bytes/components per color as required by the platform  e.g., RGB, XBGR, RGBA



/*
    English     Latin
    canvas      carbasa
    paint       pingere

*/



void Startup_Platform(void)
{
    int w;
    int h;
    SDL_WindowFlags sdl3_window_flags;

#if defined(__linux__)
    /* CLAUDE: on a Wayland desktop, SDL's own Wayland driver first, X11 (XWayland) if it can't start -- as in sdl2_Init.c. Under
       XWayland SDL cannot tell where the real pointer is: the captured mouse wandered onto the other monitor and the first click
       went to another window. Wayland reports the pointer entering and leaving truthfully and locks it once it is over the game.
       SDL3 has its own default order; this makes the choice explicit. An explicit SDL_VIDEO_DRIVER (or the old SDL_VIDEODRIVER)
       still wins. */
    {
        const char * video_driver_env = getenv("SDL_VIDEO_DRIVER");
        const char * old_video_driver_env = getenv("SDL_VIDEODRIVER");
        const char * wayland_display_env = getenv("WAYLAND_DISPLAY");
        if(((video_driver_env == NULL) || (video_driver_env[0] == '\0')) && ((old_video_driver_env == NULL) || (old_video_driver_env[0] == '\0')) && (wayland_display_env != NULL) && (wayland_display_env[0] != '\0'))
        {
            SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "wayland,x11");
        }
    }
#endif

    /* CLAUDE: a captured mouse (relative mode) moves with the desktop's pointer acceleration, as the pointer does everywhere else. SDL3's
       default is raw, unaccelerated motion, which on Linux felt slow and laggy next to SDL2. */
    SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_SYSTEM_SCALE, "1");

#ifndef NO_SOUND_LIBRARY
    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);
#else
    SDL_Init(SDL_INIT_VIDEO);
#endif

    sdl3_ticks_startup = SDL_GetTicks();  // the number of milliseconds since SDL library initialization
    LOG_DEBUG(LOG_CAT_PFL, "sdl3_ticks_startup: %llu", sdl3_ticks_startup);

    sdl3_performance_counter = SDL_GetPerformanceCounter();
    LOG_DEBUG(LOG_CAT_PFL, "sdl3_performance_counter: %llu", sdl3_performance_counter);

    /* [Video] aspect_ratio = 4:3 (PLAN-Aspect-Ratio.md phase 13): the window opens at the image's 4:3 shape, 1280x960, so the
       image fills it at x4 with no bars. 16:10 keeps 1280x800. */
    if(Platform_Video_Pixel_Aspect() == VIEWPORT_PIXEL_ASPECT_4_3)
    {
        sdl3_window_height = (sdl3_window_width * 3) / 4;
    }

    w = sdl3_window_width;
    h = sdl3_window_height;
    assert(w >= PLATFORM_SCREEN_WIDTH && "window width must be >= PLATFORM_SCREEN_WIDTH");
    assert(h >= PLATFORM_SCREEN_HEIGHT && "window height must be >= PLATFORM_SCREEN_HEIGHT");

    sdl3_window_flags = SDL_WINDOW_RESIZABLE;

    sdl3_window = SDL_CreateWindow(NULL, w, h, sdl3_window_flags);
    assert(sdl3_window != NULL);

    /* CLAUDE: register the real title so the live readout appends to it, not to a default. */
    Perf_Live_Set_Base_Title(sdl3_window_title);
    {
        int actual_w, actual_h;
        SDL_GetWindowSize(sdl3_window, &actual_w, &actual_h);
        LOG_INFO(LOG_CAT_SDL3_INIT, "CLAUDE: requested window %dx%d, got %dx%d", w, h, actual_w, actual_h);
        assert(actual_w == w && "SDL created window with wrong width");
        assert(actual_h == h && "SDL created window with wrong height");
    }
    SDL_SetWindowPosition(sdl3_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    sdl3_renderer = SDL_CreateRenderer(sdl3_window, NULL);
    assert(sdl3_renderer != NULL);
    SDL_SetRenderVSync(sdl3_renderer, 1);

    // SDL_SetWindowTitle(sdl3_window, "Game Title - FPS: 0"); // Initial title
    SDL_SetWindowTitle(sdl3_window, sdl3_window_title);

    // Create the 8-bit paletted and the 32-bit RGBA screenbuffer surfaces.

    sdl3_surface_RGB666 = SDL_CreateSurface(PLATFORM_SCREEN_WIDTH, PLATFORM_SCREEN_HEIGHT, SDL_PIXELFORMAT_INDEX8);
    assert(sdl3_surface_RGB666 != NULL);

    /* CLAUDE: SDL3 does not auto-create a palette for INDEX8 surfaces (SDL2 did). */
    {
        SDL_Palette *pal = SDL_CreatePalette(256);
        assert(pal != NULL);
        // assert(SDL_SetSurfacePalette(sdl3_surface_RGB666, pal));
        /* CLAUDE: the call must run in Release too -- inside assert() it compiled away under NDEBUG, leaving the surface with no palette. */
        {
            bool palette_set = SDL_SetSurfacePalette(sdl3_surface_RGB666, pal);
            if(palette_set == false)
            {
                LOG_ERROR(LOG_CAT_SDL3_INIT, "graphics: SDL_SetSurfacePalette failed: %s", SDL_GetError());
            }
            assert(palette_set);
        }
        SDL_DestroyPalette(pal);
    }
    assert(SDL_GetSurfacePalette(sdl3_surface_RGB666) != NULL && "palette must survive DestroyPalette (refcounted)");

    SDL_FillSurfaceRect(sdl3_surface_RGB666, NULL, 0);

    sdl3_surface_ARGB8888 = SDL_CreateSurface(PLATFORM_SCREEN_WIDTH, PLATFORM_SCREEN_HEIGHT, SDL_PIXELFORMAT_ARGB8888);
    assert(sdl3_surface_ARGB8888 != NULL);

    sdl3_texture = SDL_CreateTexture(sdl3_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, PLATFORM_SCREEN_WIDTH, PLATFORM_SCREEN_HEIGHT);
    assert(sdl3_texture != NULL);
    /* CLAUDE: PLAN-Fullscreen.md phase 7 -- integer and fit_nearest both sample nearest-neighbour (SDL3 textures default to linear). */
    SDL_SetTextureScaleMode(sdl3_texture, SDL_SCALEMODE_NEAREST);

    /* CLAUDE: PLAN-Fullscreen.md phase 7 -- the viewport from the real window, logged once even when it equals the default. */
    if(sdl3_Viewport_Refresh() == 0)
    {
        sdl3_Viewport_Log();
    }

    /* CLAUDE: [Video] startup_window = fullscreen (ReMoM.ini, the default): start fullscreen, exactly as if Alt+Enter had been
       pressed. Windowed is the window just created; leaving fullscreen comes back to it. */
    sdl3_Set_Window_State(Platform_Video_Startup_Window());

    /* CLAUDE: take the focus at startup, as sdl2_Init.c does. Without it the window came up fullscreen on Linux (GNOME) with the
       keyboard still going to whatever had it before: ESC did nothing, and the game saw no input until clicked.
       SDL_HINT_FORCE_RAISEWINDOW makes the raise a real activation request, which window managers honour where a plain raise is
       treated as focus stealing. (SDL3 has no SDL_SetWindowInputFocus; SDL_RaiseWindow takes the focus.) SDL3, unlike SDL2,
       already prefers its Wayland driver on a Wayland desktop. */
#ifdef SDL_HINT_FORCE_RAISEWINDOW
    SDL_SetHint(SDL_HINT_FORCE_RAISEWINDOW, "1");
#endif
    if(SDL_RaiseWindow(sdl3_window) == false)
    {
        LOG_INFO(LOG_CAT_PFL, "graphics: could not raise the window at startup: %s", SDL_GetError());
    }

    /* CLAUDE: HW-cursor prototype shows the OS cursor; the default path hides it and software-draws. */
    if(Platform_HW_Cursor_Active())
    {
        SDL_ShowCursor();
    }
    else
    {
        SDL_HideCursor();
    }

    // SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);

    Build_Key_Xlat();

#ifndef NO_SOUND_LIBRARY
    sdl3_Audio_Init();
#endif

    /* CLAUDE: Platform-Input Layer 1 -- runtime-gated input responsiveness metrics, OFF by default, as in sdl2_Init.c.
       REMOM_INPUT_METRICS=1 writes ./remom_input_metrics.fwv; =PATH writes PATH; unset/0 stays off.
       See doc/@AI_Plans/{BRA,PRD,PLAN}-Platform-Input.md. */
    {
        const char * im_env = getenv("REMOM_INPUT_METRICS");
        if(im_env != NULL && im_env[0] != '\0' && strcmp(im_env, "0") != 0)
        {
            const char * im_path = (strcmp(im_env, "1") == 0) ? "remom_input_metrics.fwv" : im_env;
            int im_scale = Platform_Get_Scale();  /* the viewport's scale, not window width / 320 */
            int im_refresh = 0;
            const SDL_DisplayMode * im_mode = SDL_GetCurrentDisplayMode(SDL_GetDisplayForWindow(sdl3_window));
            if(im_scale < 1) { im_scale = 1; }
            if(im_mode != NULL)
            {
                im_refresh = (int)(im_mode->refresh_rate + 0.5f);
            }
            Input_Metrics_Init(im_path, "SDL3", SDL_GetCurrentVideoDriver(), im_scale, im_refresh);
            sdl3_Viewport_Note_Metrics();  /* the real scale and rect; later changes are noted as they happen */
            LOG_INFO(LOG_CAT_PFL, "CLAUDE: input metrics ENABLED -> %s", im_path);
        }
    }

}

void Shutdown_Platform(void)
{
    Input_Metrics_Shutdown();  /* CLAUDE: flush the input-metrics .fwv (also runs via atexit) */
    SDL_ShowCursor();

#ifndef NO_SOUND_LIBRARY
    sdl3_Audio_Deinit();
#endif

    SDL_DestroyRenderer(sdl3_renderer);

    SDL_DestroyWindow(sdl3_window);

    SDL_Quit();
}

void Platform_Mouse_Input_Enable(void)
{
    platform_mouse_input_enabled = ST_TRUE;
}

void Platform_Mouse_Input_Disable(void)
{
    platform_mouse_input_enabled = ST_FALSE;
}
