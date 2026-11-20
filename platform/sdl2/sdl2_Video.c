/**
 * sdl2_Video.c — Platform palette and video update (from MOM_PFL.c).
 *
 * Implements Platform_Palette_Update() and Platform_Video_Update().
 */

#include "../../platform/include/Platform.h"
#include "../include/Platform_Capture.h"
#include "../include/Platform_Input_Metrics.h"  /* CLAUDE: Platform-Input Layer 1 metrics */
#include "../include/Platform_Perf.h"           /* CLAUDE: live frame-time readout (FR9/FR11) */

#include "sdl2_PFL.h"
#include "../../STU/src/STU_LOG.h"
#include <SDL.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>  /* getenv: REMOM_SHARP_BILINEAR_FAIL */



/* Where the game image is shown. Starts as the default 1280x800 window at 4x so mouse mapping is valid before
   Startup_Platform() refreshes it from the real window. */
struct s_VIEWPORT sdl2_viewport = {
    PLATFORM_SCREEN_WIDTH * 4, PLATFORM_SCREEN_HEIGHT * 4,  /* window */
    PLATFORM_SCREEN_WIDTH * 4, PLATFORM_SCREEN_HEIGHT * 4,  /* drawable */
    PLATFORM_SCREEN_WIDTH, PLATFORM_SCREEN_HEIGHT,          /* game */
    VIEWPORT_SCALE_MODE_INTEGER, VIEWPORT_PIXEL_ASPECT_SQUARE,
    4,                                                      /* scale */
    0, 0, PLATFORM_SCREEN_WIDTH * 4, PLATFORM_SCREEN_HEIGHT * 4  /* rect */
};

int sdl2_Viewport_Refresh(void)
{
    struct s_VIEWPORT viewport;
    int window_width = 0;
    int window_height = 0;
    int drawable_width = 0;
    int drawable_height = 0;

    if((sdl2_window == NULL) || (sdl2_renderer == NULL))
    {
        return 0;
    }

    SDL_GetWindowSize(sdl2_window, &window_width, &window_height);
    if((SDL_GetRendererOutputSize(sdl2_renderer, &drawable_width, &drawable_height) != 0) || (drawable_width <= 0) || (drawable_height <= 0))
    {
        drawable_width = window_width;
        drawable_height = window_height;
    }
    if((window_width <= 0) || (window_height <= 0))
    {
        return 0;  /* minimised: keep the last viewport */
    }

    Viewport_Compute(&viewport, window_width, window_height, drawable_width, drawable_height, PLATFORM_SCREEN_WIDTH, PLATFORM_SCREEN_HEIGHT, Platform_Video_Scale_Mode(), Platform_Video_Pixel_Aspect());  /* [Video] scale_mode, aspect_ratio */
    if(Viewport_Equal(&viewport, &sdl2_viewport))
    {
        return 0;
    }
    sdl2_viewport = viewport;
    sdl2_Mouse_Confine_Update();  /* a captured mouse follows the image's new rect */
    sdl2_Viewport_Log();
    return 1;
}

/*
 * fit_sharp_bilinear (PLAN-Fullscreen.md phase 4): the frame is first drawn nearest-neighbour at the viewport's whole-number
 * scale into this intermediate render target, then filtered linearly into the rect. Rebuilt only when that scale changes.
 * If the renderer cannot make or use a render target the mode falls back to fit_nearest for the rest of the run, logged once.
 * REMOM_SHARP_BILINEAR_FAIL=1 forces that fallback, to test it.
 */
static int sdl2_upscaled_scale = 0;                    /* the scale sdl2_texture_upscaled was built at; 0 = none */
static int sdl2_sharp_bilinear_unavailable = 0;        /* fell back to fit_nearest */

static void sdl2_Sharp_Bilinear_Give_Up(const char * reason)
{
    LOG_ERROR(LOG_CAT_SDL2_PFL, "graphics: fit_sharp_bilinear unavailable (%s) -- using fit_nearest", reason);
    sdl2_sharp_bilinear_unavailable = 1;
    if(sdl2_texture_upscaled != NULL)
    {
        SDL_DestroyTexture(sdl2_texture_upscaled);
        sdl2_texture_upscaled = NULL;
    }
    sdl2_upscaled_scale = 0;
}

/* The intermediate target at the viewport's current scale. Returns 0 if sharp bilinear cannot be used. */
static int sdl2_Sharp_Bilinear_Ready(void)
{
    int scale = sdl2_viewport.scale;

    if(sdl2_sharp_bilinear_unavailable != 0)
    {
        return 0;
    }
    if((sdl2_texture_upscaled != NULL) && (sdl2_upscaled_scale == scale))
    {
        return 1;
    }
    if(sdl2_texture_upscaled != NULL)
    {
        SDL_DestroyTexture(sdl2_texture_upscaled);
        sdl2_texture_upscaled = NULL;
        sdl2_upscaled_scale = 0;
    }
    {
        const char * fail_env = getenv("REMOM_SHARP_BILINEAR_FAIL");
        if((fail_env != NULL) && (strcmp(fail_env, "1") == 0))
        {
            sdl2_Sharp_Bilinear_Give_Up("forced by REMOM_SHARP_BILINEAR_FAIL=1");
            return 0;
        }
    }
    if(SDL_RenderTargetSupported(sdl2_renderer) == SDL_FALSE)
    {
        sdl2_Sharp_Bilinear_Give_Up("the renderer has no render targets");
        return 0;
    }
    sdl2_texture_upscaled = SDL_CreateTexture(sdl2_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, PLATFORM_SCREEN_WIDTH * scale, PLATFORM_SCREEN_HEIGHT * scale);
    if(sdl2_texture_upscaled == NULL)
    {
        sdl2_Sharp_Bilinear_Give_Up(SDL_GetError());
        return 0;
    }
    SDL_SetTextureScaleMode(sdl2_texture_upscaled, SDL_ScaleModeLinear);
    sdl2_upscaled_scale = scale;
    LOG_INFO(LOG_CAT_SDL2_PFL, "graphics: fit_sharp_bilinear intermediate %dx%d (x%d), filtered to %dx%d", PLATFORM_SCREEN_WIDTH * scale, PLATFORM_SCREEN_HEIGHT * scale, scale, sdl2_viewport.rect_width, sdl2_viewport.rect_height);
    return 1;
}

/* Draw the frame (already in sdl2_texture) into the viewport's rect, with black around it. */
void sdl2_Present_Image(void)
{
    SDL_Rect image_rect = { sdl2_viewport.rect_x, sdl2_viewport.rect_y, sdl2_viewport.rect_width, sdl2_viewport.rect_height };

    if((sdl2_viewport.scale_mode == VIEWPORT_SCALE_MODE_FIT_SHARP_BILINEAR) && (sdl2_Sharp_Bilinear_Ready() != 0))
    {
        if(SDL_SetRenderTarget(sdl2_renderer, sdl2_texture_upscaled) == 0)
        {
            SDL_RenderCopy(sdl2_renderer, sdl2_texture, NULL, NULL);  /* nearest-neighbour: sdl2_texture's scale mode */
            SDL_SetRenderTarget(sdl2_renderer, NULL);
            SDL_SetRenderDrawColor(sdl2_renderer, 0, 0, 0, 255);
            SDL_RenderClear(sdl2_renderer);
            SDL_RenderCopy(sdl2_renderer, sdl2_texture_upscaled, NULL, &image_rect);  /* linear */
            return;
        }
        sdl2_Sharp_Bilinear_Give_Up(SDL_GetError());
    }

    SDL_SetRenderDrawColor(sdl2_renderer, 0, 0, 0, 255);
    SDL_RenderClear(sdl2_renderer);
    SDL_RenderCopy(sdl2_renderer, sdl2_texture, NULL, &image_rect);
}

void sdl2_Viewport_Log(void)
{
    LOG_INFO(LOG_CAT_SDL2_PFL, "graphics: viewport -- window %dx%d, drawable %dx%d pixels, %s, %s, whole-number scale %d, image at %d,%d size %dx%d", sdl2_viewport.window_width, sdl2_viewport.window_height, sdl2_viewport.drawable_width, sdl2_viewport.drawable_height, Viewport_Scale_Mode_Name(sdl2_viewport.scale_mode), Viewport_Aspect_Ratio_Name(sdl2_viewport.pixel_aspect), sdl2_viewport.scale, sdl2_viewport.rect_x, sdl2_viewport.rect_y, sdl2_viewport.rect_width, sdl2_viewport.rect_height);
    sdl2_Viewport_Note_Metrics();
}

void sdl2_Viewport_Note_Metrics(void)
{
    Input_Metrics_Note_Viewport(sdl2_viewport.window_width, sdl2_viewport.window_height, sdl2_viewport.drawable_width, sdl2_viewport.drawable_height, Viewport_Scale_Mode_Name(sdl2_viewport.scale_mode), Viewport_Aspect_Ratio_Name(sdl2_viewport.pixel_aspect), sdl2_viewport.scale, sdl2_viewport.rect_x, sdl2_viewport.rect_y, sdl2_viewport.rect_width, sdl2_viewport.rect_height);
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
    SDL_Color color = {0};
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = 255;

    platform_palette_buffer[index].r = r;
    platform_palette_buffer[index].g = g;
    platform_palette_buffer[index].b = b;
    platform_palette_buffer[index].a = 255;

    if(sdl2_surface_RGB666->format->palette != NULL)
    {
        SDL_SetPaletteColors(sdl2_surface_RGB666->format->palette, &color, index, 1);
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

    assert(sdl2_surface_RGB666 != NULL);
    assert(sdl2_surface_ARGB8888 != NULL);
    assert(sdl2_texture != NULL);
    assert(sdl2_renderer != NULL);
    assert(video_page_buffer[draw_page_num] != NULL);

    /* PFL_Color is layout-compatible with SDL_Color */
    {
        SDL_Palette *pal = sdl2_surface_RGB666->format->palette;
        assert(pal != NULL && "8-bit surface must have a palette");
        SDL_SetPaletteColors(pal, (SDL_Color *)platform_palette_buffer, 0, 256);
    }

    SDL_LockSurface(sdl2_surface_RGB666);

    pitch = sdl2_surface_RGB666->pitch;
    dst = (uint8_t *)sdl2_surface_RGB666->pixels;
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

    SDL_UnlockSurface(sdl2_surface_RGB666);

    SDL_LockTexture(sdl2_texture, &sdl2_blit_rect, &sdl2_surface_ARGB8888->pixels, &sdl2_surface_ARGB8888->pitch);
    assert(sdl2_surface_ARGB8888->pixels != NULL);

    sdl2_surface_ARGB8888->w = sdl2_blit_rect.w;
    sdl2_surface_ARGB8888->h = sdl2_blit_rect.h;

    {
        SDL_Rect dst_rect = { 0, 0, sdl2_blit_rect.w, sdl2_blit_rect.h };
        SDL_LowerBlit(sdl2_surface_RGB666, &sdl2_blit_rect, sdl2_surface_ARGB8888, &dst_rect);
    }

    SDL_UnlockTexture(sdl2_texture);

    /* The image in the viewport's rect, black bars around it. Clear every frame so the bars never keep stale pixels after a
       resize. */
    sdl2_Viewport_Refresh();
    sdl2_Present_Image();

    /* CLAUDE: metrics -- bracket the present so block time (vsync stall) and inter-present interval
       are both measured.  Off path is one branch; the two SDL_GetTicks() only run when enabled. */
    if(Input_Metrics_Active())
    {
        uint64_t im_t0 = (uint64_t)SDL_GetTicks();
        SDL_RenderPresent(sdl2_renderer);
        Input_Metrics_Record_Present(im_t0, (uint64_t)SDL_GetTicks());
    }
    else
    {
        SDL_RenderPresent(sdl2_renderer);
    }

    /* CLAUDE: live frame-time readout -- one subtraction per present, one OS call per second. */
    Perf_Live_Note_Present();

    /* CLAUDE */  DBG_Frame_Reset();

}
