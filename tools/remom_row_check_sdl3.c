/*
    remom_row_check_sdl3 -- the row check (remom_row_check.h) for SDL3's render drivers. PLAN-Aspect-Ratio.md phase 13.

    Draws the coordinate-encoding test image with NEAREST scaling into each test rect, on a render target in a hidden window,
    with the same SDL_RenderTexture call ReMoM's SDL3 present uses (an SDL_FRect made from the Viewport's whole-pixel rect), reads
    it back, and reports which game row and column each drawable row and column shows. Nothing is shown on screen.

    Usage:
        remom_row_check_sdl3          # every render driver SDL3 offers
        remom_row_check_sdl3 RD       # just render driver RD (e.g. direct3d11, opengl, software)

    Dev/QA tool, SDL3 builds only (USE_SDL3=ON). The SDL2 equivalent is remom_video_probe --row-check.
*/

#define SDL_MAIN_HANDLED

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "remom_row_check.h"

static int row_check_Driver(const char * render_driver)
{
    SDL_Window * window = NULL;
    SDL_Renderer * renderer = NULL;
    SDL_Texture * game = NULL;
    SDL_Texture * target = NULL;
    uint32_t * texels = NULL;
    int case_index = 0;
    int result = 1;

    window = SDL_CreateWindow("remom_row_check_sdl3", 64, 64, SDL_WINDOW_HIDDEN);
    if(window == NULL) { printf("  %-11s window failed: %s\n", render_driver, SDL_GetError()); return 1; }
    renderer = SDL_CreateRenderer(window, render_driver);
    if(renderer == NULL) { printf("  %-11s not available: %s\n", render_driver, SDL_GetError()); SDL_DestroyWindow(window); return 1; }
    printf("  render driver %s (asked for %s)\n", SDL_GetRendererName(renderer), render_driver);

    game = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, ROW_CHECK_GAME_WIDTH, ROW_CHECK_GAME_HEIGHT);
    target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, ROW_CHECK_TARGET_WIDTH, ROW_CHECK_TARGET_HEIGHT);
    texels = (uint32_t *)malloc(sizeof(uint32_t) * ROW_CHECK_GAME_WIDTH * ROW_CHECK_GAME_HEIGHT);
    if(game == NULL || target == NULL || texels == NULL) { printf("    setup failed: %s\n", SDL_GetError()); goto done; }

    {
        int x, y;
        for(y = 0; y < ROW_CHECK_GAME_HEIGHT; y++)
        {
            for(x = 0; x < ROW_CHECK_GAME_WIDTH; x++)
            {
                texels[y * ROW_CHECK_GAME_WIDTH + x] = Row_Check_Texel(x, y);
            }
        }
    }
    SDL_UpdateTexture(game, NULL, texels, ROW_CHECK_GAME_WIDTH * (int)sizeof(uint32_t));
    SDL_SetTextureScaleMode(game, SDL_SCALEMODE_NEAREST);  /* as ReMoM's game texture (sdl3_Init.c) */
    SDL_SetTextureBlendMode(game, SDL_BLENDMODE_NONE);

    result = 0;
    for(case_index = 0; case_index < ROW_CHECK_CASE_COUNT; case_index++)
    {
        const struct row_check_case * c = &ROW_CHECK_CASES[case_index];
        SDL_FRect rect = { (float)c->x, (float)c->y, (float)c->w, (float)c->h };
        SDL_Surface * read = NULL;
        SDL_Surface * argb = NULL;

        SDL_SetRenderTarget(renderer, target);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, game, NULL, &rect);  /* the same call shape ReMoM's SDL3 present uses */
        read = SDL_RenderReadPixels(renderer, NULL);
        if(read != NULL)
        {
            argb = SDL_ConvertSurface(read, SDL_PIXELFORMAT_ARGB8888);
        }
        if(argb == NULL || argb->w < ROW_CHECK_TARGET_WIDTH || argb->h < ROW_CHECK_TARGET_HEIGHT)
        {
            printf("    %s: read back failed: %s\n", c->name, SDL_GetError());
            result = 1;
        }
        else if(Row_Check_Report(c, (const uint32_t *)argb->pixels, argb->pitch / (int)sizeof(uint32_t)) == 0)
        {
            result = 1;
        }
        SDL_DestroySurface(argb);
        SDL_DestroySurface(read);
    }

done:
    free(texels);
    if(target != NULL) { SDL_DestroyTexture(target); }
    if(game != NULL) { SDL_DestroyTexture(game); }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return result;
}

int main(int argc, char ** argv)
{
    int failures = 0;
    int index = 0;

    SDL_SetMainReady();
    setvbuf(stdout, NULL, _IONBF, 0);
    if(SDL_Init(SDL_INIT_VIDEO) == false) { printf("SDL_Init failed: %s\n", SDL_GetError()); return 1; }
    printf("row check (SDL3 %d.%d.%d): does NEAREST scaling put each drawable row/column in the game pixel its centre falls in?\n", SDL_VERSIONNUM_MAJOR(SDL_GetVersion()), SDL_VERSIONNUM_MINOR(SDL_GetVersion()), SDL_VERSIONNUM_MICRO(SDL_GetVersion()));
    if(argc >= 2)
    {
        failures += row_check_Driver(argv[1]);
    }
    else
    {
        for(index = 0; index < SDL_GetNumRenderDrivers(); index++)
        {
            failures += row_check_Driver(SDL_GetRenderDriver(index));
        }
    }
    SDL_Quit();
    printf("result: %s\n", (failures == 0) ? "every driver follows the centre rule" : "at least one driver did not follow the centre rule (or was not available)");
    return 0;
}
