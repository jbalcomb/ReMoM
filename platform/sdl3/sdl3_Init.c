
#include "../../MoX/src/Mouse.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_TYPE.h"
#include "../../MoX/src/sdl2_Audio.h"

#include "MOM_PFL.h"

#include "../../STU/src/STU_DBG.h"

#include "sdl2_KD.h"
#include "sdl2_MOM.h"
#include "sdl2_PFL.h"
#include "sdl2_SDLK.h"

#include <SDL3/SDL.h>
#ifndef NO_SOUND_LIBRARY
#include <SDL3_mixer/SDL_mixer.h>
#endif

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

    sdl2_ticks_startup = SDL_GetTicks();  // the number of milliseconds since SDL library initialization
#ifdef STU_DEBUG
    dbg_prn("sdl2_ticks_startup: %llu\n", sdl2_ticks_startup);
#endif

    sdl2_performance_counter = SDL_GetPerformanceCounter();
#ifdef STU_DEBUG
    dbg_prn("sdl2_performance_counter: %llu\n", sdl2_performance_counter);
#endif

    w = sdl2_window_width;
    h = sdl2_window_height;

    sdl3_window_flags = SDL_WINDOW_RESIZABLE;

    sdl2_window = SDL_CreateWindow(NULL, w, h, sdl3_window_flags);
    SDL_SetWindowPosition(sdl2_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    sdl2_renderer = SDL_CreateRenderer(sdl2_window, NULL);
    SDL_SetRenderVSync(sdl2_renderer, 1);

    // SDL_SetWindowTitle(sdl2_window, "Game Title - FPS: 0"); // Initial title
    SDL_SetWindowTitle(sdl2_window, sdl2_window_title);

    // Create the 8-bit paletted and the 32-bit RGBA screenbuffer surfaces.

    sdl2_surface_RGB666 = SDL_CreateSurface(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_PIXELFORMAT_INDEX8);

    SDL_FillSurfaceRect(sdl2_surface_RGB666, NULL, 0);

    sdl2_surface_ARGB8888 = SDL_CreateSurface(w, h, SDL_PIXELFORMAT_ARGB8888);

    sdl2_texture = SDL_CreateTexture(sdl2_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_HideCursor();

    // SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);

    build_key_xlat();

#ifndef NO_SOUND_LIBRARY
    sdl2_Audio_Init();
#endif

}

void Shudown_Platform(void)
{
    SDL_ShowCursor();

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
