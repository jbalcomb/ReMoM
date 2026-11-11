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
#include "../../STU/src/STU_LOG.h"  /* CLAUDE: present-stage log lines */
#include <SDL.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>  /* CLAUDE: getenv for the present-stage options */


/* CLAUDE: present stage -- aspect ratio, fit to window, full screen.
 *
 * MoM's 320x200 frame was drawn for a 4:3 screen, so each pixel is 1.2 times taller than wide. By default the frame is shown
 * at 4:3, as large as the window allows, centred, with black bars where the window's shape differs. The game keeps drawing
 * 320x200; only this last step changes, and mouse positions are mapped back through the same rectangle.
 *
 * Options (environment, read once at startup):
 *   REMOM_ASPECT     4:3 (default) | square   -- square shows the frame at 16:10 with square pixels
 *   REMOM_SCALING    sharp (default) | smooth | nearest | integer
 *                      sharp    nearest-neighbour up to the next whole multiple, then smooth down to fit: crisp pixels, even sizes
 *                      smooth   bilinear straight to fit
 *                      nearest  nearest-neighbour straight to fit (uneven pixel sizes when the scale isn't whole)
 *                      integer  the largest whole multiple that fits, nearest-neighbour, letterboxed
 *   REMOM_FULLSCREEN 1 starts full screen (desktop resolution). Alt+Enter toggles full screen at any time.
 */
int sdl2_aspect_4x3 = 1;
int sdl2_scaling = SDL2_SCALING_SHARP;
SDL_Rect sdl2_viewport = {0, 0, PLATFORM_SCREEN_WIDTH, PLATFORM_SCREEN_HEIGHT};  /* where the frame is shown, in window coordinates */
static SDL_Rect sdl2_viewport_px = {0, 0, PLATFORM_SCREEN_WIDTH, PLATFORM_SCREEN_HEIGHT};  /* the same, in renderer output pixels */
static int sdl2_upscale_factor = 0;  /* size of sdl2_texture_upscaled, as a multiple of 320x200; 0 = none */

void sdl2_Present_Options(void)
{
    static const char * scaling_names[] = {"sharp", "smooth", "nearest", "integer"};
    const char * aspect = getenv("REMOM_ASPECT");
    const char * scaling = getenv("REMOM_SCALING");

    sdl2_aspect_4x3 = !(aspect != NULL && (strcmp(aspect, "square") == 0 || strcmp(aspect, "16:10") == 0));
    if(scaling == NULL || scaling[0] == '\0' || strcmp(scaling, "sharp") == 0) { sdl2_scaling = SDL2_SCALING_SHARP; }
    else if(strcmp(scaling, "smooth") == 0) { sdl2_scaling = SDL2_SCALING_SMOOTH; }
    else if(strcmp(scaling, "nearest") == 0) { sdl2_scaling = SDL2_SCALING_NEAREST; }
    else if(strcmp(scaling, "integer") == 0) { sdl2_scaling = SDL2_SCALING_INTEGER; }
    else
    {
        LOG_ERROR(LOG_CAT_SDL2_INIT, "graphics: REMOM_SCALING=%s is not one of sharp, smooth, nearest, integer -- using sharp", scaling);
        sdl2_scaling = SDL2_SCALING_SHARP;
    }
    LOG_INFO(LOG_CAT_SDL2_INIT, "graphics: aspect %s, scaling %s", sdl2_aspect_4x3 ? "4:3" : "square pixels (16:10)", scaling_names[sdl2_scaling]);
}

/* Height of the shown frame for a given width: 4:3, or 320:200 with square pixels. */
static int sdl2_Frame_Height_For(int width)
{
    return sdl2_aspect_4x3 ? (width * 3 + 2) / 4 : (width * PLATFORM_SCREEN_HEIGHT + PLATFORM_SCREEN_WIDTH / 2) / PLATFORM_SCREEN_WIDTH;
}

/* Work out where the frame goes in the window. Returns 1 if it moved or changed size. */
int sdl2_Viewport_Update(void)
{
    int ww, wh, ow, oh;
    int w, h, k;
    SDL_Rect px, win;

    SDL_GetWindowSize(sdl2_window, &ww, &wh);
    if(SDL_GetRendererOutputSize(sdl2_renderer, &ow, &oh) != 0 || ow <= 0 || oh <= 0)
    {
        ow = ww;
        oh = wh;
    }
    if(ww <= 0 || wh <= 0) { return 0; }

    if(sdl2_scaling == SDL2_SCALING_INTEGER)
    {
        k = ow / PLATFORM_SCREEN_WIDTH;
        while(k > 1 && sdl2_Frame_Height_For(PLATFORM_SCREEN_WIDTH * k) > oh) { k--; }
        if(k < 1) { k = 1; }
        w = PLATFORM_SCREEN_WIDTH * k;
        h = sdl2_Frame_Height_For(w);
    }
    else
    {
        w = ow;
        h = sdl2_Frame_Height_For(w);
        if(h > oh)
        {
            h = oh;
            w = sdl2_aspect_4x3 ? (h * 4 + 1) / 3 : (h * PLATFORM_SCREEN_WIDTH + PLATFORM_SCREEN_HEIGHT / 2) / PLATFORM_SCREEN_HEIGHT;
        }
    }
    px.w = w;
    px.h = h;
    px.x = (ow - w) / 2;
    px.y = (oh - h) / 2;

    /* Window coordinates (what mouse events use) differ from output pixels on high-DPI displays. */
    win.x = (int)((int64_t)px.x * ww / ow);
    win.y = (int)((int64_t)px.y * wh / oh);
    win.w = (int)((int64_t)px.w * ww / ow);
    win.h = (int)((int64_t)px.h * wh / oh);
    if(win.w < 1) { win.w = 1; }
    if(win.h < 1) { win.h = 1; }

    if(memcmp(&px, &sdl2_viewport_px, sizeof(px)) == 0 && memcmp(&win, &sdl2_viewport, sizeof(win)) == 0)
    {
        return 0;
    }
    sdl2_viewport_px = px;
    sdl2_viewport = win;
    LOG_INFO(LOG_CAT_SDL2_PFL, "graphics: window %dx%d (%dx%d pixels), frame shown at %d,%d size %dx%d", ww, wh, ow, oh, px.x, px.y, px.w, px.h);
    return 1;
}

/* Window coordinates -> game coordinates (0-319, 0-199), clamped to the frame. Returns 1 if the point is on the frame. */
int sdl2_Window_To_Game(int wx, int wy, int * gx, int * gy)
{
    int inside = (wx >= sdl2_viewport.x) && (wy >= sdl2_viewport.y) && (wx < sdl2_viewport.x + sdl2_viewport.w) && (wy < sdl2_viewport.y + sdl2_viewport.h);
    int x = (int)((int64_t)(wx - sdl2_viewport.x) * PLATFORM_SCREEN_WIDTH / sdl2_viewport.w);
    int y = (int)((int64_t)(wy - sdl2_viewport.y) * PLATFORM_SCREEN_HEIGHT / sdl2_viewport.h);
    if(wx < sdl2_viewport.x) { x = 0; }
    if(wy < sdl2_viewport.y) { y = 0; }
    SETRANGE(x, PLATFORM_SCREEN_XMIN, PLATFORM_SCREEN_XMAX);
    SETRANGE(y, PLATFORM_SCREEN_YMIN, PLATFORM_SCREEN_YMAX);
    *gx = x;
    *gy = y;
    return inside;
}

/* Game coordinates -> window coordinates of the middle of that game pixel. */
void sdl2_Game_To_Window(int gx, int gy, int * wx, int * wy)
{
    *wx = sdl2_viewport.x + (int)(((int64_t)gx * 2 + 1) * sdl2_viewport.w / (PLATFORM_SCREEN_WIDTH * 2));
    *wy = sdl2_viewport.y + (int)(((int64_t)gy * 2 + 1) * sdl2_viewport.h / (PLATFORM_SCREEN_HEIGHT * 2));
}

/* Alt+Enter: full screen at the desktop's resolution, or back to the window. */
void sdl2_Toggle_Fullscreen(void)
{
    int full = (SDL_GetWindowFlags(sdl2_window) & SDL_WINDOW_FULLSCREEN) != 0;
    if(SDL_SetWindowFullscreen(sdl2_window, full ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
    {
        LOG_ERROR(LOG_CAT_SDL2_PFL, "graphics: full screen toggle failed: %s", SDL_GetError());
        return;
    }
    LOG_INFO(LOG_CAT_SDL2_PFL, "graphics: %s", full ? "windowed" : "full screen");
}

/* Copy the 320x200 texture to the viewport with the chosen filtering. Sharp: nearest-neighbour into a render target of the
   next whole multiple at least as large as the viewport, then bilinear down to it. Falls back to smooth if render targets fail. */
static void sdl2_Present_Frame(void)
{
    int k;

    SDL_SetRenderDrawColor(sdl2_renderer, 0, 0, 0, 255);
    SDL_RenderClear(sdl2_renderer);

    if(sdl2_scaling == SDL2_SCALING_SHARP)
    {
        k = (sdl2_viewport_px.w + PLATFORM_SCREEN_WIDTH - 1) / PLATFORM_SCREEN_WIDTH;
        if((sdl2_viewport_px.h + PLATFORM_SCREEN_HEIGHT - 1) / PLATFORM_SCREEN_HEIGHT > k) { k = (sdl2_viewport_px.h + PLATFORM_SCREEN_HEIGHT - 1) / PLATFORM_SCREEN_HEIGHT; }
        if(k < 1) { k = 1; }
        if(k != sdl2_upscale_factor)
        {
            if(sdl2_texture_upscaled != NULL) { SDL_DestroyTexture(sdl2_texture_upscaled); }
            sdl2_texture_upscaled = SDL_CreateTexture(sdl2_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, PLATFORM_SCREEN_WIDTH * k, PLATFORM_SCREEN_HEIGHT * k);
            if(sdl2_texture_upscaled == NULL)
            {
                LOG_ERROR(LOG_CAT_SDL2_PFL, "graphics: no render target for sharp scaling (%s) -- using smooth", SDL_GetError());
                sdl2_scaling = SDL2_SCALING_SMOOTH;
                sdl2_upscale_factor = 0;
            }
            else
            {
                SDL_SetTextureScaleMode(sdl2_texture_upscaled, SDL_ScaleModeLinear);
                sdl2_upscale_factor = k;
            }
        }
    }

    if(sdl2_scaling == SDL2_SCALING_SHARP && sdl2_texture_upscaled != NULL && SDL_SetRenderTarget(sdl2_renderer, sdl2_texture_upscaled) == 0)
    {
        SDL_SetTextureScaleMode(sdl2_texture, SDL_ScaleModeNearest);
        SDL_RenderCopy(sdl2_renderer, sdl2_texture, NULL, NULL);
        SDL_SetRenderTarget(sdl2_renderer, NULL);
        SDL_RenderCopy(sdl2_renderer, sdl2_texture_upscaled, NULL, &sdl2_viewport_px);
    }
    else
    {
        SDL_SetTextureScaleMode(sdl2_texture, (sdl2_scaling == SDL2_SCALING_NEAREST || sdl2_scaling == SDL2_SCALING_INTEGER) ? SDL_ScaleModeNearest : SDL_ScaleModeLinear);
        SDL_RenderCopy(sdl2_renderer, sdl2_texture, NULL, &sdl2_viewport_px);
    }
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

    /* CLAUDE: present stage -- the frame at 4:3 (or square pixels), fitted to the window; was a stretch to the whole window */
    // SDL_RenderCopy(sdl2_renderer, sdl2_texture, NULL, NULL);
    if(sdl2_Viewport_Update())
    {
        sdl2_HW_Cursor_Rebuild();
    }
    sdl2_Present_Frame();

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
