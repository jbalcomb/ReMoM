/**
 * sdl3_Video.c — Platform palette and video update (from MOM_PFL.c).
 *
 * Implements Platform_Palette_Update() and Platform_Video_Update().
 */

#include "../../platform/include/Platform.h"

#include "../../MoX/src/Fonts.h"
#include "../../MoX/src/MOX_BASE.h"
#include "../../MoX/src/MOX_TYPE.h"
#include "../../MoX/src/Video.h"
#include "../../MoX/src/Video2.h"

#include "../../MoX/src/sdl2_PFL.h"
#include <SDL3/SDL.h>


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

    palette = SDL_GetSurfacePalette(sdl2_surface_RGB666);
    if (palette != NULL)
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



    /* PFL_Color is layout-compatible with SDL_Color */
    SDL_SetPaletteColors(SDL_GetSurfacePalette(sdl2_surface_RGB666), (SDL_Color *)platform_palette_buffer, 0, 256);



    SDL_LockSurface(sdl2_surface_RGB666);

    pitch = sdl2_surface_RGB666->pitch;
    dst = (uint8_t *)sdl2_surface_RGB666->pixels;
    src = video_page_buffer[draw_page_num];
    for(int y = 0; y < screen_pixel_height; ++y)
    {
        memcpy(dst, src, screen_pixel_width);
        dst += pitch;
        src += screen_pixel_width;
    }

    SDL_UnlockSurface(sdl2_surface_RGB666);



    SDL_LockTexture(sdl2_texture, &sdl2_blit_rect, &sdl2_surface_ARGB8888->pixels, &sdl2_surface_ARGB8888->pitch);

    SDL_BlitSurfaceUnchecked(sdl2_surface_RGB666, &sdl2_blit_rect, sdl2_surface_ARGB8888, &sdl2_blit_rect);

    SDL_UnlockTexture(sdl2_texture);

    SDL_RenderTexture(sdl2_renderer, sdl2_texture, NULL, NULL);

    SDL_RenderPresent(sdl2_renderer);

    /* CLAUDE */  DBG_Frame_Reset();

}
