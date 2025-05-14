
#include "MOX/Mouse.H"
#include "MOX/MOX_DEF.H"
#include "MOX/MOX_TYPE.H"
#include "MOX/sdl2_Audio.H"

#include "MOM_PFL.H"

#include "STU/STU_DBG.H"

#include "sdl2_KD.H"
#include "sdl2_MOM.H"
#include "sdl2_PFL.H"
#include "sdl2_SDLK.H"

// #include "SDL.h"
#include "C:\devellib\SDL2-2.32.2\include\SDL.h"
// #include "SDL_mixer.h"
#include "C:\devellib\SDL2_mixer-2.8.1\include\SDL_mixer.h"

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
    int x;
    int y;
    int w;
    int h;
    int sdl2_window_flags;
    int sdl2_renderer_flags;

    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_VIDEO);

    sdl2_ticks_startup = SDL_GetTicks64();  // the number of milliseconds since SDL library initialization
    dbg_prn("sdl2_ticks_startup: %llu\n", sdl2_ticks_startup);

    sdl2_performance_counter = SDL_GetPerformanceCounter();
    dbg_prn("sdl2_performance_counter: %llu\n", sdl2_performance_counter);

    x = SDL_WINDOWPOS_CENTERED;
    y = SDL_WINDOWPOS_CENTERED;

    w = sdl2_window_width;
    h = sdl2_window_height;

    sdl2_window_flags = SDL_WINDOW_RESIZABLE;

    sdl2_window = SDL_CreateWindow(NULL, x, y, w, h, sdl2_window_flags);

    sdl2_renderer_flags = SDL_RENDERER_PRESENTVSYNC;

    sdl2_renderer_flags |= SDL_RENDERER_ACCELERATED;

    sdl2_renderer = SDL_CreateRenderer(sdl2_window, -1, sdl2_renderer_flags);

    // SDL_SetWindowTitle(sdl2_window, "Game Title - FPS: 0"); // Initial title
    SDL_SetWindowTitle(sdl2_window, sdl2_window_title);

    // Create the 8-bit paletted and the 32-bit RGBA screenbuffer surfaces.

    sdl2_surface_RGB666 = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 8, 0, 0, 0, 0);

    SDL_FillRect(sdl2_surface_RGB666, NULL, 0);

    sdl2_surface_ARGB8888 = SDL_CreateRGBSurfaceWithFormatFrom(NULL, w, h, 0, 0, SDL_PIXELFORMAT_ARGB8888);

    sdl2_texture = SDL_CreateTexture(sdl2_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_ShowCursor(SDL_DISABLE);
    
    // SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);

    build_key_xlat();

    sdl2_Audio_Init();

}

void Shudown_Platform(void)
{
    SDL_ShowCursor(SDL_ENABLE);

    sdl2_Audio_Deinit();

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
