
#include "Platform.h"

#include "sdl3_Audio.h"

#include "../../STU/src/STU_DBG.h"

#include "sdl3_KD.h"
#include "sdl3_MOM.h"
#include "sdl3_PFL.h"

#include <SDL3/SDL.h>
#ifndef NO_SOUND_LIBRARY
#include <SDL3_mixer/SDL_mixer.h>
#endif

#include <assert.h>
#include <stdio.h>

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

#ifndef NO_SOUND_LIBRARY
    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);
#else
    SDL_Init(SDL_INIT_VIDEO);
#endif

    sdl3_ticks_startup = SDL_GetTicks();  // the number of milliseconds since SDL library initialization
#ifdef STU_DEBUG
    dbg_prn("sdl3_ticks_startup: %llu\n", sdl3_ticks_startup);
#endif

    sdl3_performance_counter = SDL_GetPerformanceCounter();
#ifdef STU_DEBUG
    dbg_prn("sdl3_performance_counter: %llu\n", sdl3_performance_counter);
#endif

    w = sdl3_window_width;
    h = sdl3_window_height;
    assert(w >= PLATFORM_SCREEN_WIDTH && "window width must be >= PLATFORM_SCREEN_WIDTH");
    assert(h >= PLATFORM_SCREEN_HEIGHT && "window height must be >= PLATFORM_SCREEN_HEIGHT");

    sdl3_window_flags = SDL_WINDOW_RESIZABLE;

    sdl3_window = SDL_CreateWindow(NULL, w, h, sdl3_window_flags);
    assert(sdl3_window != NULL);
    {
        int actual_w, actual_h;
        SDL_GetWindowSize(sdl3_window, &actual_w, &actual_h);
        fprintf(stderr, "CLAUDE: requested window %dx%d, got %dx%d\n", w, h, actual_w, actual_h);
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
        assert(SDL_SetSurfacePalette(sdl3_surface_RGB666, pal));
        SDL_DestroyPalette(pal);
    }
    assert(SDL_GetSurfacePalette(sdl3_surface_RGB666) != NULL && "palette must survive DestroyPalette (refcounted)");

    SDL_FillSurfaceRect(sdl3_surface_RGB666, NULL, 0);

    sdl3_surface_ARGB8888 = SDL_CreateSurface(PLATFORM_SCREEN_WIDTH, PLATFORM_SCREEN_HEIGHT, SDL_PIXELFORMAT_ARGB8888);
    assert(sdl3_surface_ARGB8888 != NULL);

    sdl3_texture = SDL_CreateTexture(sdl3_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, PLATFORM_SCREEN_WIDTH, PLATFORM_SCREEN_HEIGHT);
    assert(sdl3_texture != NULL);

    SDL_HideCursor();

    // SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);

    Build_Key_Xlat();

#ifndef NO_SOUND_LIBRARY
    sdl3_Audio_Init();
#endif

}

void Shutdown_Platform(void)
{
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
