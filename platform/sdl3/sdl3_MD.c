
#include "Platform.h"

#include "sdl3_PFL.h"

#include <SDL3/SDL.h>

#include "../../STU/src/STU_LOG.h"  /* CLAUDE: pointer-move diagnosis */

#include <stdlib.h>  /* CLAUDE: getenv() for the HW-cursor gate */
#include <string.h>  /* CLAUDE: strcmp() */



/* CLAUDE: synthetic (injected) mouse button for HMS/replay click playback — see Platform.h. */
static int16_t sdl3_synthetic_mouse_button = 0;
static int16_t sdl3_synthetic_mouse_button_hold = 0;

void Platform_Set_Synthetic_Mouse_Button(int16_t buttons)
{
    sdl3_synthetic_mouse_button = buttons;
    sdl3_synthetic_mouse_button_hold = (buttons != 0) ? PLATFORM_SYNTHETIC_MOUSE_HOLD : 0;
}

int16_t Platform_Get_Mouse_Button_State(void)
{
    SDL_MouseButtonFlags sdl3_mouse_state = 0;
    int16_t l_mouse_button = 0;

    if(!platform_mouse_input_enabled)
    {
        return 0;
    }

    sdl3_mouse_state = SDL_GetMouseState(NULL, NULL);

    if(sdl3_mouse_state & SDL_BUTTON_LMASK)
    {
        l_mouse_button = ST_LEFT_BUTTON;
    }
    if(sdl3_mouse_state & SDL_BUTTON_RMASK)
    {
        l_mouse_button = ST_RIGHT_BUTTON;
    }

    /* CLAUDE: OR in the synthetic press, returning it BEFORE decrementing the hold so
       this read still sees it; auto-release when the hold expires so the caller's
       release spin-wait exits. */
    if(sdl3_synthetic_mouse_button != 0)
    {
        l_mouse_button |= sdl3_synthetic_mouse_button;
        if(sdl3_synthetic_mouse_button_hold > 0)
        {
            sdl3_synthetic_mouse_button_hold--;
            if(sdl3_synthetic_mouse_button_hold == 0)
            {
                sdl3_synthetic_mouse_button = 0;
            }
        }
    }

    return l_mouse_button;
}


void Platform_Warp_Mouse(int16_t game_x, int16_t game_y)
{
    int window_x = 0;
    int window_y = 0;
    /* CLAUDE */  Platform_Set_Warp_Guard();  /* prevent Platform_Maybe_Move_Mouse() from reading stale OS position */
    Platform_Game_To_Window(game_x, game_y, &window_x, &window_y);  /* through the viewport (letterbox, high-DPI) */
    if(sdl3_Mouse_Is_Relative() != 0)
    {
        sdl3_Mouse_Relative_Set(game_x, game_y);  /* CLAUDE: captured -- the game cursor is tracked here, not from the OS pointer */
    }
    /* CLAUDE: MUTTER-49.0-CRASH -- WAYLAND + CAPTURED: DO NOT WARP THE OS POINTER. Do not remove this check.
       On GNOME 49 / mutter 49.0 (Ubuntu 25.10), SDL3 3.2's warp of a captured (relative-mode) pointer at startup crashed GNOME Shell,
       and with it the player's whole desktop session: mutter logged "Invalid window geometry for xdg_surface" and died with SIGFPE
       in meta_window_update_visibility (a divide by the window's size). Proven on 2026-11-26 in a throwaway login session: the same
       binary crashed the shell with this warp and ran clean without it (and the SDL3 backend without the startup warp,
       Init_Mouse_Driver in MoX/src/Mouse.c, never crashed). Captured, the warp is not needed: the real pointer is locked and hidden,
       and the game cursor is the position set just above. (SDL2 2.32 on the same desktop made the same startup warp without a
       crash.) */
    if((sdl3_Mouse_Is_Relative() != 0) && (SDL_GetCurrentVideoDriver() != NULL) && (SDL_strcmp(SDL_GetCurrentVideoDriver(), "wayland") == 0))
    {
        LOG_DEBUG(LOG_CAT_PFL, "mouse: warp game %d,%d -- captured on Wayland: the game cursor moved, the OS pointer left alone (MUTTER-49.0-CRASH)", (int)game_x, (int)game_y);
        return;
    }
    SDL_WarpMouseInWindow(sdl3_window, (float)window_x, (float)window_y);
    /* CLAUDE: pointer-move diagnosis (Linux: the start position and arrow keys) -- where it was asked to go, and where SDL says it is. */
    {
        float now_x = 0.0f;
        float now_y = 0.0f;
        SDL_GetMouseState(&now_x, &now_y);
        LOG_DEBUG(LOG_CAT_PFL, "mouse: warp game %d,%d -> window %d,%d; SDL reports %d,%d (relative mode %d, mouse focus %s)", (int)game_x, (int)game_y, window_x, window_y, (int)now_x, (int)now_y, sdl3_Mouse_Is_Relative(), (SDL_GetMouseFocus() == sdl3_window) ? "this window" : "elsewhere");
    }
}


int Platform_Get_Window_Width(void)
{
    return sdl3_window_width;
}

/* CLAUDE: window <-> game through the shared Viewport (letterbox, fit modes, high-DPI). PLAN-Fullscreen.md phase 7. */
int16_t Platform_Window_To_Game(int window_x, int window_y, int16_t * game_x, int16_t * game_y)
{
    int gx = 0;
    int gy = 0;
    int on_image = Viewport_Window_To_Game(&sdl3_viewport, window_x, window_y, &gx, &gy);
    *game_x = (int16_t)gx;
    *game_y = (int16_t)gy;
    return (on_image != 0) ? ST_TRUE : ST_FALSE;
}

void Platform_Game_To_Window(int16_t game_x, int16_t game_y, int * window_x, int * window_y)
{
    Viewport_Game_To_Window(&sdl3_viewport, game_x, game_y, window_x, window_y);
}


/* Mouse_Button(), Mouse_Movement_Handler(), Mouse_Button_Handler(), User_Mouse_Handler(), Set_Mouse_Position() moved to MoX/src/Mouse.c */


/* ========================================================================= */
/*  CLAUDE: Hardware-cursor path (Platform-Input follow-on) -- SDL3 port of   */
/*  the SDL2 implementation in sdl2_MD.c.  See doc/@AI_Plans/PLAN-Platform-   */
/*  Input.md.  Builds a real SDL cursor from the current game cursor sprite   */
/*  so the compositor tracks it at native rate, decoupled from the ~18 fps    */
/*  framebuffer present.  Opt-in via REMOM_HW_CURSOR=1.                        */
/* ========================================================================= */

/* Sprite geometry for the shipped 320x200 build; mirrors MOM_DEF.h CURSOR_WIDTH/HEIGHT and
   MOX_DEF.h ST_TRANSPARENT (both 16, transparent index 0). */
#define HWCUR_DIM          16
#define HWCUR_TRANSPARENT  0

static SDL_Cursor * hwcur_current = NULL;
static int          hwcur_gate = -1;   /* -1 = unresolved, else 0/1 from the env var */
static int          hwcur_last_image = -1;
static int          hwcur_last_width = 0;   /* size the current OS cursor was built at, in drawable pixels */
static int          hwcur_last_height = 0;
static int          hwcur_overlay_gate = 0;            /* CLAUDE: draw the cursor over the picture while captured (Linux default) */
static SDL_Texture * hwcur_overlay_texture = NULL;     /* CLAUDE: the same image as the OS cursor, for the overlay */

int Platform_HW_Cursor_Active(void)
{
    if(hwcur_gate < 0)
    {
        const char * env = getenv("REMOM_HW_CURSOR");
        /* CLAUDE: default ON -- the hardware cursor is the shipped behaviour; only REMOM_HW_CURSOR=0 forces the software cursor. */
        hwcur_gate = (env != NULL && strcmp(env, "0") == 0) ? 0 : 1;
#if defined(__linux__)
        /* CLAUDE: Linux, by default, as in sdl2_MD.c: the OS cursor in a window, and while captured in fullscreen the cursor drawn
           over the picture by ReMoM (sdl3_Cursor_Overlay_*), with the mouse in relative mode. With the OS cursor captured, GNOME's
           XWayland neither enforces the confinement nor moves the pointer for the game, and the game's own software cursor, drawn
           into the page, only moves at the game's 18 frames a second. The overlay is redrawn whenever the mouse moves.
           REMOM_HW_CURSOR=1 forces the plain OS cursor, =0 the game's software cursor. */
        hwcur_overlay_gate = ((hwcur_gate == 1) && ((env == NULL) || (strcmp(env, "1") != 0))) ? 1 : 0;
#endif
    }
    return hwcur_gate;
}

int sdl3_Cursor_Overlay_Active(void)
{
    (void)Platform_HW_Cursor_Active();  /* resolves the gates */
    return hwcur_overlay_gate;
}

/* CLAUDE: the cursor, drawn over the frame already rendered, at the game pointer position -- only while captured (relative mode),
   where the OS cursor is hidden. Hotspot top-left, as for the OS cursor. */
void sdl3_Cursor_Overlay_Draw(void)
{
    SDL_FRect cursor_rect = { 0.0f, 0.0f, 0.0f, 0.0f };

    if((sdl3_Cursor_Overlay_Active() == 0) || (sdl3_Mouse_Is_Relative() == 0) || (hwcur_overlay_texture == NULL) || (Get_Pointer_Image_Number() == 0))
    {
        return;
    }
    cursor_rect.x = (float)(sdl3_viewport.rect_x + (int)(((int64_t)pointer_x * sdl3_viewport.rect_width) / PLATFORM_SCREEN_WIDTH));
    cursor_rect.y = (float)(sdl3_viewport.rect_y + (int)(((int64_t)pointer_y * sdl3_viewport.rect_height) / PLATFORM_SCREEN_HEIGHT));
    cursor_rect.w = (float)hwcur_last_width;
    cursor_rect.h = (float)hwcur_last_height;
    SDL_RenderTexture(sdl3_renderer, hwcur_overlay_texture, NULL, &cursor_rect);
}

void Platform_HW_Cursor_Refresh(void)
{
    int image;
    int cursor_width;
    int cursor_height;
    int px, py;
    const uint8_t * sprite;
    SDL_Surface * surf;
    SDL_Cursor * newcur;
    uint32_t * dst;
    int pitch_px;

    if(!Platform_HW_Cursor_Active()) { return; }

    image = Get_Pointer_Image_Number();
    /* The OS cursor is as big as 16 game pixels are on screen: the viewport rect over the game size, so it matches the drawn
       image in every scale mode (PLAN-Fullscreen.md phases 5 and 7).  Rounded to the nearest pixel. */
    Viewport_Game_Size_In_Drawable(&sdl3_viewport, HWCUR_DIM, HWCUR_DIM, &cursor_width, &cursor_height);
    if((image == hwcur_last_image) && (cursor_width == hwcur_last_width) && (cursor_height == hwcur_last_height)) { return; }   /* no shape or size change since the last build */
    hwcur_last_image = image;
    hwcur_last_width = cursor_width;
    hwcur_last_height = cursor_height;

    if(image == 0)   /* crsr_None */
    {
        SDL_HideCursor();
        return;
    }
    if(mouse_palette == NULL)   /* sprite bank not loaded yet (pre-Load_Palette) -- retry next frame */
    {
        hwcur_last_image = -1;
        return;
    }

    surf = SDL_CreateSurface(cursor_width, cursor_height, SDL_PIXELFORMAT_ARGB8888);
    if(surf == NULL) { return; }

    sprite = mouse_palette + ((image - 1) * (HWCUR_DIM * HWCUR_DIM));
    dst = (uint32_t *)surf->pixels;
    pitch_px = surf->pitch / 4;

    /* Nearest-neighbour: each cursor pixel takes the sprite pixel it falls in, so the OS cursor matches the drawn size. */
    for(py = 0; py < cursor_height; py++)
    {
        int sprite_y = (py * HWCUR_DIM) / cursor_height;
        for(px = 0; px < cursor_width; px++)
        {
            int sprite_x = (px * HWCUR_DIM) / cursor_width;
            uint8_t idx = sprite[sprite_x * HWCUR_DIM + sprite_y];   /* column-major, matching Draw_Mouse_On_Page_ */
            uint32_t argb;
            if(idx == HWCUR_TRANSPARENT)
            {
                argb = 0x00000000u;
            }
            else
            {
                PFL_Color c = platform_palette_buffer[idx];
                argb = ((uint32_t)0xFFu << 24) | ((uint32_t)c.r << 16) | ((uint32_t)c.g << 8) | (uint32_t)c.b;
            }
            dst[py * pitch_px + px] = argb;
        }
    }

    /* Hotspot (0,0): the game treats the pointer position as the sprite's top-left for hit-testing. */
    newcur = SDL_CreateColorCursor(surf, 0, 0);
    if(hwcur_overlay_gate != 0)
    {
        /* CLAUDE: the overlay draws this same image, already at its on-screen size */
        if(hwcur_overlay_texture != NULL) { SDL_DestroyTexture(hwcur_overlay_texture); }
        hwcur_overlay_texture = SDL_CreateTextureFromSurface(sdl3_renderer, surf);
        if(hwcur_overlay_texture != NULL)
        {
            SDL_SetTextureBlendMode(hwcur_overlay_texture, SDL_BLENDMODE_BLEND);
            SDL_SetTextureScaleMode(hwcur_overlay_texture, SDL_SCALEMODE_NEAREST);
        }
    }
    SDL_DestroySurface(surf);
    if(newcur != NULL)
    {
        SDL_ShowCursor();
        SDL_SetCursor(newcur);
        if(hwcur_current != NULL) { SDL_DestroyCursor(hwcur_current); }
        hwcur_current = newcur;
    }
}
