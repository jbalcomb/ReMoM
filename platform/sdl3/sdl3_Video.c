/**
 * sdl3_Video.c — Platform palette and video update (from MOM_PFL.c).
 *
 * Implements Platform_Palette_Update() and Platform_Video_Update().
 */

#include "../../platform/include/Platform.h"
#include "../include/Platform_Perf.h"  /* CLAUDE: live frame-time readout (FR9/FR11) */
#include "../include/Platform_Capture.h"

#include "sdl3_PFL.h"
#include "../../STU/src/STU_LOG.h"
#include <SDL3/SDL.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>  /* getenv: REMOM_SHARP_BILINEAR_FAIL */



/* Where the game image is shown. Starts as the default 1280x800 window at 4x so mouse mapping is valid before
   Startup_Platform() refreshes it from the real window. PLAN-Fullscreen.md phase 7 (the SDL2 backend's phases 1-6). */
struct s_VIEWPORT sdl3_viewport = {
    PLATFORM_SCREEN_WIDTH * 4, PLATFORM_SCREEN_HEIGHT * 4,  /* window */
    PLATFORM_SCREEN_WIDTH * 4, PLATFORM_SCREEN_HEIGHT * 4,  /* drawable */
    PLATFORM_SCREEN_WIDTH, PLATFORM_SCREEN_HEIGHT,          /* game */
    VIEWPORT_SCALE_MODE_INTEGER, VIEWPORT_PIXEL_ASPECT_SQUARE,
    4,                                                      /* scale */
    0, 0, PLATFORM_SCREEN_WIDTH * 4, PLATFORM_SCREEN_HEIGHT * 4  /* rect */
};

int sdl3_Viewport_Refresh(void)
{
    struct s_VIEWPORT viewport;
    int window_width = 0;
    int window_height = 0;
    int drawable_width = 0;
    int drawable_height = 0;

    if((sdl3_window == NULL) || (sdl3_renderer == NULL))
    {
        return 0;
    }

    SDL_GetWindowSize(sdl3_window, &window_width, &window_height);
    if((SDL_GetCurrentRenderOutputSize(sdl3_renderer, &drawable_width, &drawable_height) == false) || (drawable_width <= 0) || (drawable_height <= 0))
    {
        drawable_width = window_width;
        drawable_height = window_height;
    }
    if((window_width <= 0) || (window_height <= 0))
    {
        return 0;  /* minimised: keep the last viewport */
    }

    Viewport_Compute(&viewport, window_width, window_height, drawable_width, drawable_height, PLATFORM_SCREEN_WIDTH, PLATFORM_SCREEN_HEIGHT, Platform_Video_Scale_Mode(), Platform_Video_Pixel_Aspect());  /* [Video] scale_mode, aspect_ratio */
    if(Viewport_Equal(&viewport, &sdl3_viewport))
    {
        return 0;
    }
    sdl3_viewport = viewport;
    sdl3_Mouse_Confine_Update();  /* a captured mouse follows the image's new rect */
    sdl3_Viewport_Log();
    return 1;
}

void sdl3_Viewport_Log(void)
{
    LOG_INFO(LOG_CAT_SDL3_PFL, "graphics: viewport -- window %dx%d, drawable %dx%d pixels, %s, %s, whole-number scale %d, image at %d,%d size %dx%d", sdl3_viewport.window_width, sdl3_viewport.window_height, sdl3_viewport.drawable_width, sdl3_viewport.drawable_height, Viewport_Scale_Mode_Name(sdl3_viewport.scale_mode), Viewport_Aspect_Ratio_Name(sdl3_viewport.pixel_aspect), sdl3_viewport.scale, sdl3_viewport.rect_x, sdl3_viewport.rect_y, sdl3_viewport.rect_width, sdl3_viewport.rect_height);
}

/*
 * fit_sharp_bilinear (PLAN-Fullscreen.md phase 4): the frame is first drawn nearest-neighbour at the viewport's whole-number
 * scale into this intermediate render target, then filtered linearly into the rect. Rebuilt only when that scale changes.
 * If the renderer cannot make or use a render target the mode falls back to fit_nearest for the rest of the run, logged once.
 * REMOM_SHARP_BILINEAR_FAIL=1 forces that fallback, to test it.
 */
static int sdl3_upscaled_scale = 0;               /* the scale sdl3_texture_upscaled was built at; 0 = none */
static int sdl3_sharp_bilinear_unavailable = 0;   /* fell back to fit_nearest */

static void sdl3_Sharp_Bilinear_Give_Up(const char * reason)
{
    LOG_ERROR(LOG_CAT_SDL3_PFL, "graphics: fit_sharp_bilinear unavailable (%s) -- using fit_nearest", reason);
    sdl3_sharp_bilinear_unavailable = 1;
    if(sdl3_texture_upscaled != NULL)
    {
        SDL_DestroyTexture(sdl3_texture_upscaled);
        sdl3_texture_upscaled = NULL;
    }
    sdl3_upscaled_scale = 0;
}

/* The intermediate target at the viewport's current scale. Returns 0 if sharp bilinear cannot be used. SDL3 has no
   SDL_RenderTargetSupported(): a renderer without targets fails the texture creation instead. */
static int sdl3_Sharp_Bilinear_Ready(void)
{
    int scale = sdl3_viewport.scale;

    if(sdl3_sharp_bilinear_unavailable != 0)
    {
        return 0;
    }
    if((sdl3_texture_upscaled != NULL) && (sdl3_upscaled_scale == scale))
    {
        return 1;
    }
    if(sdl3_texture_upscaled != NULL)
    {
        SDL_DestroyTexture(sdl3_texture_upscaled);
        sdl3_texture_upscaled = NULL;
        sdl3_upscaled_scale = 0;
    }
    {
        const char * fail_env = getenv("REMOM_SHARP_BILINEAR_FAIL");
        if((fail_env != NULL) && (strcmp(fail_env, "1") == 0))
        {
            sdl3_Sharp_Bilinear_Give_Up("forced by REMOM_SHARP_BILINEAR_FAIL=1");
            return 0;
        }
    }
    sdl3_texture_upscaled = SDL_CreateTexture(sdl3_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, PLATFORM_SCREEN_WIDTH * scale, PLATFORM_SCREEN_HEIGHT * scale);
    if(sdl3_texture_upscaled == NULL)
    {
        sdl3_Sharp_Bilinear_Give_Up(SDL_GetError());
        return 0;
    }
    SDL_SetTextureScaleMode(sdl3_texture_upscaled, SDL_SCALEMODE_LINEAR);
    sdl3_upscaled_scale = scale;
    LOG_INFO(LOG_CAT_SDL3_PFL, "graphics: fit_sharp_bilinear intermediate %dx%d (x%d), filtered to %dx%d", PLATFORM_SCREEN_WIDTH * scale, PLATFORM_SCREEN_HEIGHT * scale, scale, sdl3_viewport.rect_width, sdl3_viewport.rect_height);
    return 1;
}

/* Draw the frame (already in sdl3_texture) into the viewport's rect, with black around it. */
void sdl3_Present_Image(void)
{
    SDL_FRect image_rect = { (float)sdl3_viewport.rect_x, (float)sdl3_viewport.rect_y, (float)sdl3_viewport.rect_width, (float)sdl3_viewport.rect_height };

    if((sdl3_viewport.scale_mode == VIEWPORT_SCALE_MODE_FIT_SHARP_BILINEAR) && (sdl3_Sharp_Bilinear_Ready() != 0))
    {
        if(SDL_SetRenderTarget(sdl3_renderer, sdl3_texture_upscaled))
        {
            SDL_RenderTexture(sdl3_renderer, sdl3_texture, NULL, NULL);  /* nearest-neighbour: sdl3_texture's scale mode */
            SDL_SetRenderTarget(sdl3_renderer, NULL);
            SDL_SetRenderDrawColor(sdl3_renderer, 0, 0, 0, 255);
            SDL_RenderClear(sdl3_renderer);
            SDL_RenderTexture(sdl3_renderer, sdl3_texture_upscaled, NULL, &image_rect);  /* linear */
            return;
        }
        sdl3_Sharp_Bilinear_Give_Up(SDL_GetError());
    }

    SDL_SetRenderDrawColor(sdl3_renderer, 0, 0, 0, 255);
    SDL_RenderClear(sdl3_renderer);
    SDL_RenderTexture(sdl3_renderer, sdl3_texture, NULL, &image_rect);
}


/*
 * CLAUDE: the SDL calls in Platform_Video_Update() used to sit inside assert(), so under NDEBUG (every Release build) they were
 * never made and an SDL3 Release build presented nothing. Each call now always runs; its result goes through here. A failure is
 * logged once per call site (a persistent failure would otherwise log every frame), and Debug still stops on it as before.
 */
#define SDL3_VIDEO_CALL_SET_PALETTE_COLORS  0
#define SDL3_VIDEO_CALL_LOCK_SURFACE        1
#define SDL3_VIDEO_CALL_LOCK_TEXTURE        2
#define SDL3_VIDEO_CALL_BLIT_SURFACE        3
#define SDL3_VIDEO_CALL_RENDER_PRESENT      4
#define SDL3_VIDEO_CALL_COUNT               5

static bool sdl3_Video_Check(bool succeeded, int call)
{
    static const char * const call_names[SDL3_VIDEO_CALL_COUNT] = { "SDL_SetPaletteColors", "SDL_LockSurface", "SDL_LockTexture", "SDL_BlitSurface", "SDL_RenderPresent" };
    static bool call_failure_logged[SDL3_VIDEO_CALL_COUNT] = { false, false, false, false, false };

    if((succeeded == false) && (call >= 0) && (call < SDL3_VIDEO_CALL_COUNT) && (call_failure_logged[call] == false))
    {
        LOG_ERROR(LOG_CAT_SDL3_PFL, "graphics: %s failed: %s (logged once)", call_names[call], SDL_GetError());
        call_failure_logged[call] = true;
    }
    assert(succeeded);
    return succeeded;
}


/*
    copy MoX software palette to platform *hardware* palette

    MoO2  Refresh_Palett() |-> Store_Palette_Block_()
*/
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

/*
    Update a single palette entry in the platform surface.
    Used by Cycle_Palette_Color() for real-time palette animation.
*/
void Platform_Set_Palette_Color(uint8_t index, uint8_t r, uint8_t g, uint8_t b)
{
    SDL_Color color;
    SDL_Palette *palette;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = 255;

    platform_palette_buffer[index].r = r;
    platform_palette_buffer[index].g = g;
    platform_palette_buffer[index].b = b;
    platform_palette_buffer[index].a = 255;

    palette = SDL_GetSurfacePalette(sdl3_surface_RGB666);
    if(palette != NULL)
    {
        SDL_SetPaletteColors(palette, &color, index, 1);
    }
}


/*
...just update what the user sees on the screen

*/
void Platform_Video_Update(void)
{
    uint8_t * dst;
    uint8_t * src;
    int pitch;



    /* CLAUDE: demo capture -- submit the finished frame before presenting it. */
    if(Platform_Capture_Active())
    {
        Platform_Capture_Video_Frame(video_page_buffer[draw_page_num], screen_pixel_width, screen_pixel_height, platform_palette_buffer);
    }

    assert(sdl3_surface_RGB666 != NULL);
    assert(sdl3_surface_ARGB8888 != NULL);
    assert(sdl3_texture != NULL);
    assert(sdl3_renderer != NULL);
    assert(video_page_buffer[draw_page_num] != NULL);

    /* PFL_Color is layout-compatible with SDL_Color */
    {
        SDL_Palette *pal = SDL_GetSurfacePalette(sdl3_surface_RGB666);
        assert(pal != NULL && "8-bit surface must have a palette");
        // assert(SDL_SetPaletteColors(pal, (SDL_Color *)platform_palette_buffer, 0, 256));
        /* CLAUDE */ sdl3_Video_Check(SDL_SetPaletteColors(pal, (SDL_Color *)platform_palette_buffer, 0, 256), SDL3_VIDEO_CALL_SET_PALETTE_COLORS);
    }

    // assert(SDL_LockSurface(sdl3_surface_RGB666));
    /* BEGIN:   CLAUDE */
    if(sdl3_Video_Check(SDL_LockSurface(sdl3_surface_RGB666), SDL3_VIDEO_CALL_LOCK_SURFACE) == false)
    {
        return;  /* no pixels to copy into: drop this frame */
    }
    /* END:     CLAUDE */

    pitch = sdl3_surface_RGB666->pitch;
    dst = (uint8_t *)sdl3_surface_RGB666->pixels;
    src = video_page_buffer[draw_page_num];
    assert(dst != NULL);
    assert(src != NULL);
    assert(screen_pixel_width <= pitch);
    for(int y = 0; y < screen_pixel_height; ++y)
    {
        memcpy(dst, src, screen_pixel_width);
        dst += pitch;
        src += screen_pixel_width;
    }

    SDL_UnlockSurface(sdl3_surface_RGB666);

    // assert(SDL_LockTexture(sdl3_texture, &sdl3_blit_rect, &sdl3_surface_ARGB8888->pixels, &sdl3_surface_ARGB8888->pitch));
    /* BEGIN:   CLAUDE */
    if(sdl3_Video_Check(SDL_LockTexture(sdl3_texture, &sdl3_blit_rect, &sdl3_surface_ARGB8888->pixels, &sdl3_surface_ARGB8888->pitch), SDL3_VIDEO_CALL_LOCK_TEXTURE) == false)
    {
        return;  /* the texture is not open for writing: drop this frame rather than blit through a stale pointer */
    }
    /* END:     CLAUDE */
    assert(sdl3_surface_ARGB8888->pixels != NULL);

    sdl3_surface_ARGB8888->w = sdl3_blit_rect.w;
    sdl3_surface_ARGB8888->h = sdl3_blit_rect.h;

    // assert(SDL_BlitSurface(sdl3_surface_RGB666, &sdl3_blit_rect, sdl3_surface_ARGB8888, NULL));
    /* CLAUDE */ sdl3_Video_Check(SDL_BlitSurface(sdl3_surface_RGB666, &sdl3_blit_rect, sdl3_surface_ARGB8888, NULL), SDL3_VIDEO_CALL_BLIT_SURFACE);

    SDL_UnlockTexture(sdl3_texture);

    // assert(SDL_RenderTexture(sdl3_renderer, sdl3_texture, NULL, NULL));
    /* PLAN-Fullscreen.md phase 7: the image in the viewport's rect, black bars around it. Cleared every frame so the bars never keep
       stale pixels after a resize. */
    sdl3_Viewport_Refresh();
    sdl3_Present_Image();

    // assert(SDL_RenderPresent(sdl3_renderer));
    /* CLAUDE */ sdl3_Video_Check(SDL_RenderPresent(sdl3_renderer), SDL3_VIDEO_CALL_RENDER_PRESENT);

    /* CLAUDE: live frame-time readout -- see Platform_Perf.h. */
    Perf_Live_Note_Present();

    /* CLAUDE */  DBG_Frame_Reset();

}
