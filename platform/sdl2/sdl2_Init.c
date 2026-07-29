
#include "Platform.h"
#include "Platform_Perf.h"  /* CLAUDE: live frame-time readout base title */
#include "Platform_Input_Metrics.h"  /* CLAUDE: Platform-Input Layer 1 metrics */

#include "sdl2_Audio.h"

#include "../../STU/src/STU_DBG.h"
#include "../../STU/src/STU_LOG.h"

#include "sdl2_KD.h"
#include "sdl2_MOM.h"
#include "sdl2_PFL.h"

#include <SDL.h>
#ifndef NO_SOUND_LIBRARY
#include <SDL_mixer.h>
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>  /* random(), getenv() */
#include <string.h>  /* CLAUDE: strcmp() for the metrics gate */

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

int16_t platform_start_id;

// make Valgrind says something else, maybe
void Platform_Report_Startup_Platform(void)
{
#ifdef _WIN32
    platform_start_id = rand();
#else
    platform_start_id = random();
#endif
    LOG_INFO(LOG_CAT_SDL2_INIT, "ReMoM: Starting up platform layer...  (%d)", platform_start_id);
}


void Startup_Platform(void)
{
    int w = 0;
    int h = 0;
    Uint32 sdl2_window_flags = 0;

    Platform_Report_Startup_Platform();

#ifndef NO_SOUND_LIBRARY
    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER);
#else
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER);
#endif

    sdl2_ticks_startup = (uint64_t)SDL_GetTicks();  // the number of milliseconds since SDL library initialization
#ifdef STU_DEBUG
    LOG_DEBUG(LOG_CAT_PFL, "sdl2_ticks_startup: %llu", sdl2_ticks_startup);
#endif

    sdl2_performance_counter = SDL_GetPerformanceCounter();
#ifdef STU_DEBUG
    LOG_DEBUG(LOG_CAT_PFL, "sdl2_performance_counter: %llu", sdl2_performance_counter);
#endif

    w = sdl2_window_width;
    h = sdl2_window_height;
    assert(w >= PLATFORM_SCREEN_WIDTH && "window width must be >= PLATFORM_SCREEN_WIDTH");
    assert(h >= PLATFORM_SCREEN_HEIGHT && "window height must be >= PLATFORM_SCREEN_HEIGHT");

    sdl2_window_flags = SDL_WINDOW_RESIZABLE;

    sdl2_window = SDL_CreateWindow(
        sdl2_window_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, sdl2_window_flags
    );
    assert(sdl2_window != NULL);

    /* CLAUDE: register the real title so the live readout appends to it, not to a default. */
    Perf_Live_Set_Base_Title(sdl2_window_title);
    {
        int actual_w, actual_h;
        SDL_GetWindowSize(sdl2_window, &actual_w, &actual_h);
        LOG_INFO(LOG_CAT_SDL2_INIT, "CLAUDE: requested window %dx%d, got %dx%d", w, h, actual_w, actual_h);
        assert(actual_w == w && "SDL created window with wrong width");
        assert(actual_h == h && "SDL created window with wrong height");
    }

    sdl2_renderer = SDL_CreateRenderer(sdl2_window, -1, SDL_RENDERER_PRESENTVSYNC);
    assert(sdl2_renderer != NULL);

    // Create the 8-bit paletted and the 32-bit RGBA screenbuffer surfaces.

    sdl2_surface_RGB666 = SDL_CreateRGBSurface(0, PLATFORM_SCREEN_WIDTH, PLATFORM_SCREEN_HEIGHT, 8, 0, 0, 0, 0);
    assert(sdl2_surface_RGB666 != NULL);
    /* SDL2 auto-creates a palette for 8-bit surfaces — no manual creation needed. */
    assert(sdl2_surface_RGB666->format->palette != NULL && "SDL2 must auto-create palette for 8-bit surface");

    SDL_FillRect(sdl2_surface_RGB666, NULL, 0);

    sdl2_surface_ARGB8888 = SDL_CreateRGBSurfaceWithFormat(0, PLATFORM_SCREEN_WIDTH, PLATFORM_SCREEN_HEIGHT, 32, SDL_PIXELFORMAT_ARGB8888);
    assert(sdl2_surface_ARGB8888 != NULL);

    sdl2_texture = SDL_CreateTexture(sdl2_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, PLATFORM_SCREEN_WIDTH, PLATFORM_SCREEN_HEIGHT);
    assert(sdl2_texture != NULL);

    /* CLAUDE: HW-cursor prototype shows the OS cursor; the default path hides it and software-draws. */
    if(Platform_HW_Cursor_Active())
    {
        SDL_ShowCursor(SDL_ENABLE);
    }
    else
    {
        SDL_ShowCursor(SDL_DISABLE);
    }

    // SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);

    Build_Key_Xlat();

#ifndef NO_SOUND_LIBRARY
    sdl2_Audio_Init();
#endif

    /* CLAUDE: Platform-Input Layer 1 -- runtime-gated input responsiveness metrics, OFF by default.
       REMOM_INPUT_METRICS=1 writes ./remom_input_metrics.fwv; =PATH writes PATH; unset/0 stays off.
       See doc/#AI_Plans/{BRA,PRD,PLAN}-Platform-Input.md. */
    {
        const char * im_env = getenv("REMOM_INPUT_METRICS");
        if(im_env != NULL && im_env[0] != '\0' && strcmp(im_env, "0") != 0)
        {
            const char * im_path = (strcmp(im_env, "1") == 0) ? "remom_input_metrics.fwv" : im_env;
            int im_scale = sdl2_window_width / PLATFORM_SCREEN_WIDTH;
            int im_refresh = 0;
            SDL_DisplayMode im_mode;
            if(im_scale < 1) { im_scale = 1; }
            if(SDL_GetWindowDisplayMode(sdl2_window, &im_mode) == 0)
            {
                im_refresh = im_mode.refresh_rate;
            }
            Input_Metrics_Init(im_path, "SDL2", SDL_GetCurrentVideoDriver(), im_scale, im_refresh);
            LOG_INFO(LOG_CAT_SDL2_INIT, "CLAUDE: input metrics ENABLED -> %s", im_path);
        }
    }

}

void Shutdown_Platform(void)
{
    Input_Metrics_Shutdown();  /* CLAUDE: flush the input-metrics .fwv (also runs via atexit) */

    SDL_ShowCursor(SDL_ENABLE);

#ifndef NO_SOUND_LIBRARY
    sdl2_Audio_Deinit();
#endif

    SDL_DestroyRenderer(sdl2_renderer);

    SDL_DestroyWindow(sdl2_window);

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
