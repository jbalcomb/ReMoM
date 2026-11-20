/**
 * win_PFL.cpp — Win32 platform implementation.
 *
 * Implements Platform.h functions using the Win32 API.
 * No engine headers are included — only Platform.h and win_PFL.h.
 */

#include "../../STU/src/STU_DBG.h"
#include "../../STU/src/STU_LOG.h"

#include <Windows.h>
#include "windowsx.h"   /* GET_X_LPARAM(), GET_Y_LPARAM() */
#include <timeapi.h>    /* timeBeginPeriod() */

#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>  /* CLAUDE: getenv() for the input-metrics gate */
#include <string.h>

#include "../../platform/include/Platform.h"
#include "../../platform/include/Platform_Keys.h"
/* CLAUDE: needed for Platform_Record_Active() / Replay_Capture_Frame() used in Platform_Event_Handler. */
#include "../../platform/include/Platform_Replay.h"
#include "../../platform/include/Platform_Capture.h"
#include "../../platform/include/Platform_Input_Metrics.h"  /* CLAUDE: Platform-Input Layer 1 metrics */
#include "../../platform/include/Platform_Perf.h"           /* CLAUDE: live frame-time readout (FR9/FR11) */
#include "win_PFL.h"

/* REMOM_VERSION_STRING comes from the CMake-generated remom_version.h (built from
 * the git tag).  The autotools build doesn't generate it, so fall back to a dev
 * placeholder when the header isn't on the include path. */
#if defined(__has_include)
#  if __has_include("remom_version.h")
#    include "remom_version.h"
#  endif
#endif
#ifndef REMOM_VERSION_STRING
#  define REMOM_VERSION_STRING "0.0.0-dev"
#endif



/* ========================================================================= */
/*  Win32 Backend State                                                      */
/* ========================================================================= */

HINSTANCE win_instance;
HWND win_window;
HDC win_device_context;

int win_window_width = PLATFORM_WINDOW_WIDTH;
int win_window_height = PLATFORM_WINDOW_HEIGHT;

uint64_t win_ticks_startup;

struct win32_offscreen_buffer win_video_back_buffer;

/* CLAUDE: PLAN-Fullscreen.md phase 8. Where the game image is shown. Starts as the default 1280x800 window at 4x so mouse mapping
   is valid before the window exists. */
struct s_VIEWPORT win_viewport = {
    PLATFORM_SCREEN_WIDTH * 4, PLATFORM_SCREEN_HEIGHT * 4,  /* window */
    PLATFORM_SCREEN_WIDTH * 4, PLATFORM_SCREEN_HEIGHT * 4,  /* drawable: no separate pixel size -- the process is not DPI-aware */
    PLATFORM_SCREEN_WIDTH, PLATFORM_SCREEN_HEIGHT,          /* game */
    VIEWPORT_SCALE_MODE_INTEGER, VIEWPORT_PIXEL_ASPECT_SQUARE,
    4,                                                      /* scale */
    0, 0, PLATFORM_SCREEN_WIDTH * 4, PLATFORM_SCREEN_HEIGHT * 4  /* rect */
};

static int win_window_state = VIEWPORT_WINDOW_STATE_WINDOWED;
static int win_window_state_before_fullscreen = VIEWPORT_WINDOW_STATE_WINDOWED;  /* where leaving fullscreen goes back to */
static WINDOWPLACEMENT win_windowed_placement = { sizeof(WINDOWPLACEMENT) };  /* the framed window, kept while borderless or fullscreen */
static int win_windowed_placement_saved = 0;
static int win_mouse_captured = 0;  /* fullscreen: the cursor is clipped to the image (ClipCursor) */
static void Win_Mouse_Confine_Update(void);

/* Optional per-frame callback for synthetic input injection (DeMoM). */
static void (*platform_frame_callback)(void) = NULL;

void Platform_Register_Frame_Callback(void (*callback)(void))
{
    platform_frame_callback = callback;
}

static const CHAR win_window_class_name[] = "MoM_Wnd_Cls";
static const CHAR win_window_title[] = "(ReMoM) Master of Magic v1.31 - Win32 (v" REMOM_VERSION_STRING ")";


void Platform_Show_Error(const char * title, const char * message)
{
    /* stderr first so the message survives even without a console/window,
       then the modal box for interactive players. */
    fprintf(stderr, "%s: %s\n", title, message);
    MessageBoxA(NULL, message, title, MB_OK | MB_ICONERROR);
}

/* CLAUDE: runtime window title -- see Platform.h.  No-ops before the window exists. */
void Platform_Set_Window_Title(const char * title)
{
    if(win_window == NULL || title == NULL) { return; }
    SetWindowTextA(win_window, title);
}



/* ========================================================================= */
/*  Forward Declarations                                                     */
/* ========================================================================= */

static void Win_Init_Back_Buffer(struct win32_offscreen_buffer *buffer, int width, int height);
static void Win_Init_Window(void);
static LRESULT CALLBACK Win_Window_Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static void Win_Convert_Engine_Pixels_To_Back_Buffer(struct win32_offscreen_buffer *buffer);
static void Win_Blit_Back_Buffer(struct win32_offscreen_buffer *buffer, HDC device_context, int window_width, int window_height);



/* ========================================================================= */
/*  Lifecycle                                                                */
/* ========================================================================= */

void Startup_Platform(void)
{
    int w = 0;
    int h = 0;

    /* CLAUDE: [Video] aspect_ratio = 4:3 (PLAN-Aspect-Ratio.md phase 14): the window opens at the image's 4:3 shape, 1280x960, so
       the image fills it at x4 with no bars. 16:10 keeps 1280x800. Before Win_Init_Window(), which sizes the window from these. */
    if(Platform_Video_Pixel_Aspect() == VIEWPORT_PIXEL_ASPECT_4_3)
    {
        win_window_height = (win_window_width * 3) / 4;
    }
    w = win_window_width;
    h = win_window_height;

#ifdef STU_DEBUG
    LOG_INFO(LOG_CAT_WIN_PFL, "DEBUG: [%s, %d]: BEGIN: Startup_Platform()", __FILE__, __LINE__);
    LOG_DEBUG(LOG_CAT_PFL, "DEBUG: [%s, %d]: BEGIN: Startup_Platform()", __FILE__, __LINE__);
#endif

    assert(w >= PLATFORM_SCREEN_WIDTH && "window width must be >= PLATFORM_SCREEN_WIDTH");
    assert(h >= PLATFORM_SCREEN_HEIGHT && "window height must be >= PLATFORM_SCREEN_HEIGHT");

    win_ticks_startup = (uint64_t)GetTickCount64();

    Win_Init_Back_Buffer(&win_video_back_buffer, PLATFORM_SCREEN_WIDTH, PLATFORM_SCREEN_HEIGHT);

    Win_Init_Window();

    /* CLAUDE: PLAN-Fullscreen.md phase 8 -- the viewport from the real window, logged once even when it equals the default; then
       [Video] startup_window = borderless | fullscreen (ReMoM.ini), exactly as if its hotkey had been pressed. */
    if(Win_Viewport_Refresh() == 0)
    {
        Win_Viewport_Log();
    }
    Win_Set_Window_State(Platform_Video_Startup_Window());

    /* CLAUDE: register the real title so the live readout appends to it, not to a default. */
    Perf_Live_Set_Base_Title(win_window_title);

    /* Set Windows scheduler granularity to 1ms for better Sleep() precision */
    timeBeginPeriod(1);

    Build_Key_Xlat();

    /* CLAUDE: Platform-Input Layer 1 -- runtime-gated input responsiveness metrics, OFF by default.
       REMOM_INPUT_METRICS=1 writes ./remom_input_metrics.fwv; =PATH writes PATH; unset/0 stays off.
       Mirrors sdl2_Init.c.  See doc/#AI_Plans/{BRA,PRD,PLAN}-Platform-Input.md. */
    {
        const char * im_env = getenv("REMOM_INPUT_METRICS");
        if(im_env != NULL && im_env[0] != '\0' && strcmp(im_env, "0") != 0)
        {
            const char * im_path = (strcmp(im_env, "1") == 0) ? "remom_input_metrics.fwv" : im_env;
            int im_scale = Platform_Get_Scale();
            if(im_scale < 1) { im_scale = 1; }
            /* Win32 has no cheap refresh-rate query here; 0 = unknown (recorded in the header). */
            Input_Metrics_Init(im_path, "Win32", "gdi", im_scale, 0);
            Input_Metrics_Note_Viewport(win_viewport.window_width, win_viewport.window_height, win_viewport.drawable_width, win_viewport.drawable_height, Viewport_Scale_Mode_Name(win_viewport.scale_mode), Viewport_Aspect_Ratio_Name(win_viewport.pixel_aspect), win_viewport.scale, win_viewport.rect_x, win_viewport.rect_y, win_viewport.rect_width, win_viewport.rect_height);  /* CLAUDE: phase 8 */
            LOG_INFO(LOG_CAT_WIN_PFL, "CLAUDE: input metrics ENABLED -> %s", im_path);
        }
    }

#ifdef STU_DEBUG
    LOG_INFO(LOG_CAT_WIN_PFL, "DEBUG: [%s, %d]: END: Startup_Platform()", __FILE__, __LINE__);
    LOG_DEBUG(LOG_CAT_PFL, "DEBUG: [%s, %d]: END: Startup_Platform()", __FILE__, __LINE__);
#endif
}

void Shutdown_Platform(void)
{
#ifdef STU_DEBUG
    LOG_INFO(LOG_CAT_WIN_PFL, "DEBUG: [%s, %d]: BEGIN: Shutdown_Platform()", __FILE__, __LINE__);
    LOG_DEBUG(LOG_CAT_PFL, "DEBUG: [%s, %d]: BEGIN: Shutdown_Platform()", __FILE__, __LINE__);
#endif
    Input_Metrics_Shutdown();  /* CLAUDE: flush the input-metrics .fwv (also runs via atexit) */
    ClipCursor(NULL);  /* CLAUDE: ClipCursor is system-wide -- never leave the cursor trapped after exit */
    ShowCursor(TRUE);
    timeEndPeriod(1);
    if(win_window)
    {
        DestroyWindow(win_window);
        win_window = NULL;
    }
#ifdef STU_DEBUG
    LOG_INFO(LOG_CAT_WIN_PFL, "DEBUG: [%s, %d]: END: Shutdown_Platform()", __FILE__, __LINE__);
    LOG_DEBUG(LOG_CAT_PFL, "DEBUG: [%s, %d]: END: Shutdown_Platform()", __FILE__, __LINE__);
#endif
}

int Platform_Get_Scale(void)
{
    // if(win_window_width <= 0)
    // {
    //     return 1;
    // }
    // return win_window_width / PLATFORM_SCREEN_WIDTH;
    return win_viewport.scale;  /* CLAUDE: the viewport's whole-number scale (PLAN-Fullscreen.md phase 8) */
}



/* ========================================================================= */
/*  CLAUDE: Viewport, window states, mouse capture -- PLAN-Fullscreen.md      */
/*  phase 8, the Win32 port of the SDL2 backend's phases 1-6.                 */
/* ========================================================================= */

int Win_Viewport_Refresh(void)
{
    struct s_VIEWPORT viewport;
    RECT client_rect;
    int client_width;
    int client_height;

    if(win_window == NULL)
    {
        return 0;
    }
    GetClientRect(win_window, &client_rect);
    client_width = client_rect.right - client_rect.left;
    client_height = client_rect.bottom - client_rect.top;
    if((client_width <= 0) || (client_height <= 0))
    {
        return 0;  /* minimised: keep the last viewport */
    }
    /* Not DPI-aware: Windows scales the window for us, so client coordinates are the pixels we draw into (window == drawable). */
    Viewport_Compute(&viewport, client_width, client_height, client_width, client_height, PLATFORM_SCREEN_WIDTH, PLATFORM_SCREEN_HEIGHT, Platform_Video_Scale_Mode(), Platform_Video_Pixel_Aspect());  /* [Video] scale_mode, aspect_ratio */
    if(Viewport_Equal(&viewport, &win_viewport))
    {
        return 0;
    }
    win_viewport = viewport;
    Win_Mouse_Confine_Update();  /* a captured mouse follows the image's new rect */
    Win_Viewport_Log();
    return 1;
}

void Win_Viewport_Log(void)
{
    LOG_INFO(LOG_CAT_WIN_PFL, "graphics: viewport -- window %dx%d, drawable %dx%d pixels, %s, %s, whole-number scale %d, image at %d,%d size %dx%d", win_viewport.window_width, win_viewport.window_height, win_viewport.drawable_width, win_viewport.drawable_height, Viewport_Scale_Mode_Name(win_viewport.scale_mode), Viewport_Aspect_Ratio_Name(win_viewport.pixel_aspect), win_viewport.scale, win_viewport.rect_x, win_viewport.rect_y, win_viewport.rect_width, win_viewport.rect_height);
    Input_Metrics_Note_Viewport(win_viewport.window_width, win_viewport.window_height, win_viewport.drawable_width, win_viewport.drawable_height, Viewport_Scale_Mode_Name(win_viewport.scale_mode), Viewport_Aspect_Ratio_Name(win_viewport.pixel_aspect), win_viewport.scale, win_viewport.rect_x, win_viewport.rect_y, win_viewport.rect_width, win_viewport.rect_height);
}

/* Fullscreen captures the mouse: the cursor is clipped to the game image (in screen coordinates), so it can never sit in a black
   bar. ClipCursor is system-wide, so it is released whenever the game is not fullscreen, loses focus, or the player middle-clicks,
   and re-applied when the image moves. */
static void Win_Mouse_Confine_Update(void)
{
    RECT clip_rect;
    POINT top_left;
    int image_x = 0;
    int image_y = 0;
    int image_width = 0;
    int image_height = 0;

    if((win_window == NULL) || (win_mouse_captured == 0))
    {
        ClipCursor(NULL);
        return;
    }
    Viewport_Image_Rect_In_Window(&win_viewport, &image_x, &image_y, &image_width, &image_height);
    top_left.x = image_x;
    top_left.y = image_y;
    ClientToScreen(win_window, &top_left);
    clip_rect.left = top_left.x;
    clip_rect.top = top_left.y;
    clip_rect.right = top_left.x + image_width;
    clip_rect.bottom = top_left.y + image_height;
    if(ClipCursor(&clip_rect) == FALSE)
    {
        LOG_WARN(LOG_CAT_WIN_PFL, "graphics: could not confine the mouse to the image %d,%d %dx%d (error %lu)", image_x, image_y, image_width, image_height, (unsigned long)GetLastError());
    }
}

static void Win_Mouse_Capture(int captured)
{
    win_mouse_captured = ((captured != 0) && (win_window_state == VIEWPORT_WINDOW_STATE_FULLSCREEN)) ? 1 : 0;
    Win_Mouse_Confine_Update();
}

/* A popup window (no title bar or frame) covering rect, on top. */
static void Win_Make_Popup(const RECT * rect)
{
    LONG_PTR style = GetWindowLongPtrA(win_window, GWL_STYLE);
    SetWindowLongPtrA(win_window, GWL_STYLE, (style & ~WS_OVERLAPPEDWINDOW) | WS_POPUP);
    SetWindowPos(win_window, HWND_TOP, rect->left, rect->top, rect->right - rect->left, rect->bottom - rect->top, SWP_FRAMECHANGED | SWP_NOOWNERZORDER | SWP_SHOWWINDOW);
}

/* The window states: windowed (WS_OVERLAPPEDWINDOW), borderless window (WS_POPUP over the monitor's work area, so the taskbar stays
   visible; not movable), fullscreen (WS_POPUP over the whole monitor -- borderless desktop fullscreen, no mode change). Going back
   to the framed window restores the placement it had before. */
void Win_Set_Window_State(int window_state)
{
    int from = win_window_state;
    MONITORINFO monitor_info;

    window_state = Viewport_Window_State_Normalize(window_state);
    if((win_window == NULL) || (window_state == from))
    {
        return;
    }

    if(from == VIEWPORT_WINDOW_STATE_WINDOWED)
    {
        win_windowed_placement.length = sizeof(WINDOWPLACEMENT);
        win_windowed_placement_saved = (GetWindowPlacement(win_window, &win_windowed_placement) != FALSE) ? 1 : 0;
    }

    if(window_state == VIEWPORT_WINDOW_STATE_WINDOWED)
    {
        LONG_PTR style = GetWindowLongPtrA(win_window, GWL_STYLE);
        SetWindowLongPtrA(win_window, GWL_STYLE, (style & ~WS_POPUP) | WS_OVERLAPPEDWINDOW);
        if(win_windowed_placement_saved != 0)
        {
            SetWindowPlacement(win_window, &win_windowed_placement);
        }
        SetWindowPos(win_window, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    }
    else
    {
        memset(&monitor_info, 0, sizeof(monitor_info));
        monitor_info.cbSize = sizeof(monitor_info);
        if(GetMonitorInfoA(MonitorFromWindow(win_window, MONITOR_DEFAULTTONEAREST), &monitor_info) == FALSE)
        {
            LOG_ERROR(LOG_CAT_WIN_PFL, "graphics: %s failed: no monitor info (error %lu)", Viewport_Window_State_Name(window_state), (unsigned long)GetLastError());
            return;
        }
        Win_Make_Popup((window_state == VIEWPORT_WINDOW_STATE_FULLSCREEN) ? &monitor_info.rcMonitor : &monitor_info.rcWork);
    }

    if(window_state == VIEWPORT_WINDOW_STATE_FULLSCREEN)
    {
        win_window_state_before_fullscreen = from;
    }
    win_window_state = window_state;
    LOG_INFO(LOG_CAT_WIN_PFL, "graphics: %s", Viewport_Window_State_Name(window_state));

    Win_Viewport_Refresh();
    Win_Mouse_Capture((window_state == VIEWPORT_WINDOW_STATE_FULLSCREEN) ? 1 : 0);
    InvalidateRect(win_window, NULL, FALSE);  /* repaint the bars at the new size */
}

void Win_Toggle_Fullscreen(void)
{
    if(win_window_state == VIEWPORT_WINDOW_STATE_FULLSCREEN)
    {
        Win_Set_Window_State(win_window_state_before_fullscreen);
    }
    else
    {
        Win_Set_Window_State(VIEWPORT_WINDOW_STATE_FULLSCREEN);
    }
}

void Win_Toggle_Borderless(void)
{
    if(win_window_state == VIEWPORT_WINDOW_STATE_BORDERLESS)
    {
        Win_Set_Window_State(VIEWPORT_WINDOW_STATE_WINDOWED);
    }
    else
    {
        Win_Set_Window_State(VIEWPORT_WINDOW_STATE_BORDERLESS);
    }
}

/* The window hotkeys, the same bindings as the SDL backends (one named table, for the planned keymap feature):
     Alt+Enter (either Enter key)  fullscreen toggle -- arrives as WM_SYSKEYDOWN
     Ctrl+L                        fullscreen toggle -- borrowed from Adobe Acrobat Reader, with Ctrl+H below
     Ctrl+H                        borderless window toggle
   The game never reads Ctrl, so Ctrl+H and Ctrl+L take nothing from it. */
#define WIN_WINDOW_HOTKEY_NONE        0
#define WIN_WINDOW_HOTKEY_FULLSCREEN  1
#define WIN_WINDOW_HOTKEY_BORDERLESS  2

static const struct
{
    int vk_code;
    int modifier_vk;  /* VK_MENU (Alt) or VK_CONTROL */
    int action;       /* WIN_WINDOW_HOTKEY_* */
} win_window_hotkeys[] = {
    { VK_RETURN, VK_MENU,    WIN_WINDOW_HOTKEY_FULLSCREEN },  /* main and keypad Enter share VK_RETURN */
    { 'L',       VK_CONTROL, WIN_WINDOW_HOTKEY_FULLSCREEN },
    { 'H',       VK_CONTROL, WIN_WINDOW_HOTKEY_BORDERLESS },
};

/* Handle a window hotkey: toggles on the first key-down (not on auto-repeat). Returns 1 if the key was a hotkey, so the caller
   stops it -- it is never translated, queued or recorded. */
static int Win_Window_Hotkey(int vk_code, LPARAM lParam)
{
    size_t hotkey_index;
    for(hotkey_index = 0; hotkey_index < (sizeof(win_window_hotkeys) / sizeof(win_window_hotkeys[0])); hotkey_index++)
    {
        if(
            (vk_code == win_window_hotkeys[hotkey_index].vk_code) &&
            ((GetKeyState(win_window_hotkeys[hotkey_index].modifier_vk) & 0x8000) != 0)
        )
        {
            if((lParam & (1L << 30)) == 0)  /* bit 30: the key was already down -- auto-repeat */
            {
                if(win_window_hotkeys[hotkey_index].action == WIN_WINDOW_HOTKEY_FULLSCREEN)
                {
                    Win_Toggle_Fullscreen();
                }
                else
                {
                    Win_Toggle_Borderless();
                }
            }
            return 1;
        }
    }
    return 0;
}



/* ========================================================================= */
/*  Video                                                                    */
/* ========================================================================= */

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

void Platform_Set_Palette_Color(uint8_t index, uint8_t r, uint8_t g, uint8_t b)
{
    platform_palette_buffer[index].r = r;
    platform_palette_buffer[index].g = g;
    platform_palette_buffer[index].b = b;
    platform_palette_buffer[index].a = 255;
}

/**
 * Convert the engine's 8-bit framebuffer to 32-bit XBGR pixels in the platform
 * back buffer, applying the current palette.  Touches engine state.
 * Called by Platform_Video_Update() — only safe after Allocate_Data_Space().
 */
static void Win_Convert_Engine_Pixels_To_Back_Buffer(struct win32_offscreen_buffer *buffer)
{
    uint8_t *source_pixels;
    uint32_t *dest_pixels;
    int width;
    int height;
    int itr_x;
    int itr_y;
    uint8_t color_index;
    uint32_t color;
    uint32_t palette_lut[256];
    int lut_i;
    
    source_pixels = video_page_buffer[draw_page_num];
    dest_pixels = (uint32_t *)buffer->Memory;
    for (lut_i = 0; lut_i < 256; lut_i++)
    {
        PFL_Color c = platform_palette_buffer[lut_i];
        palette_lut[lut_i] = ((uint32_t)c.r << 16) | ((uint32_t)c.g << 8) | (uint32_t)c.b;
    }

    width = screen_pixel_width;
    height = screen_pixel_height;

    for (itr_y = 0; itr_y < height; itr_y++)
    {
        for (itr_x = 0; itr_x < width; itr_x++)
        {
            color_index = *(source_pixels + (itr_y * width) + itr_x);
            color = palette_lut[color_index];
            *(dest_pixels + (itr_y * width) + itr_x) = color;
        }
    }
}

/**
 * Blit the platform's back buffer to the window.  Touches no engine state.
 * Safe to call any time after Win_Init_Back_Buffer() — including from WM_PAINT
 * before the engine has initialized.
 */
/*
 * CLAUDE: fit_sharp_bilinear (PLAN-Fullscreen.md phases 4 and 8). GDI has no bilinear filter, so the frame is drawn nearest-neighbour
 * at the viewport's whole-number scale into a memory bitmap, which StretchBlt's HALFTONE mode (GDI's smoothing filter, an area
 * average) then scales into the rect. The bitmap is rebuilt only when that scale changes. If it cannot be made the mode falls back
 * to fit_nearest for the rest of the run, logged once. REMOM_SHARP_BILINEAR_FAIL=1 forces that fallback, to test it.
 */
static HDC win_upscaled_dc = NULL;
static HBITMAP win_upscaled_bitmap = NULL;
static HGDIOBJ win_upscaled_previous_bitmap = NULL;
static int win_upscaled_scale = 0;
static int win_sharp_bilinear_unavailable = 0;

static void Win_Sharp_Bilinear_Release(void)
{
    if(win_upscaled_dc != NULL)
    {
        SelectObject(win_upscaled_dc, win_upscaled_previous_bitmap);
        DeleteDC(win_upscaled_dc);
        win_upscaled_dc = NULL;
    }
    if(win_upscaled_bitmap != NULL)
    {
        DeleteObject(win_upscaled_bitmap);
        win_upscaled_bitmap = NULL;
    }
    win_upscaled_scale = 0;
}

static void Win_Sharp_Bilinear_Give_Up(const char * reason)
{
    LOG_ERROR(LOG_CAT_WIN_PFL, "graphics: fit_sharp_bilinear unavailable (%s) -- using fit_nearest", reason);
    win_sharp_bilinear_unavailable = 1;
    Win_Sharp_Bilinear_Release();
}

static int Win_Sharp_Bilinear_Ready(HDC device_context)
{
    int scale = win_viewport.scale;

    if(win_sharp_bilinear_unavailable != 0)
    {
        return 0;
    }
    if((win_upscaled_dc != NULL) && (win_upscaled_scale == scale))
    {
        return 1;
    }
    Win_Sharp_Bilinear_Release();
    {
        const char * fail_env = getenv("REMOM_SHARP_BILINEAR_FAIL");
        if((fail_env != NULL) && (strcmp(fail_env, "1") == 0))
        {
            Win_Sharp_Bilinear_Give_Up("forced by REMOM_SHARP_BILINEAR_FAIL=1");
            return 0;
        }
    }
    win_upscaled_dc = CreateCompatibleDC(device_context);
    win_upscaled_bitmap = CreateCompatibleBitmap(device_context, PLATFORM_SCREEN_WIDTH * scale, PLATFORM_SCREEN_HEIGHT * scale);
    if((win_upscaled_dc == NULL) || (win_upscaled_bitmap == NULL))
    {
        Win_Sharp_Bilinear_Give_Up("could not create the intermediate bitmap");
        return 0;
    }
    win_upscaled_previous_bitmap = SelectObject(win_upscaled_dc, win_upscaled_bitmap);
    SetStretchBltMode(win_upscaled_dc, COLORONCOLOR);
    win_upscaled_scale = scale;
    LOG_INFO(LOG_CAT_WIN_PFL, "graphics: fit_sharp_bilinear intermediate %dx%d (x%d), filtered to %dx%d", PLATFORM_SCREEN_WIDTH * scale, PLATFORM_SCREEN_HEIGHT * scale, scale, win_viewport.rect_width, win_viewport.rect_height);
    return 1;
}

/* Black over the parts of the client area outside the image rect -- the bars. Drawn every present, so they never keep stale pixels. */
static void Win_Fill_Bars(HDC device_context)
{
    HBRUSH black = (HBRUSH)GetStockObject(BLACK_BRUSH);
    int client_width = win_viewport.window_width;
    int client_height = win_viewport.window_height;
    int left = win_viewport.rect_x;
    int top = win_viewport.rect_y;
    int right = win_viewport.rect_x + win_viewport.rect_width;
    int bottom = win_viewport.rect_y + win_viewport.rect_height;
    RECT bar;

    if(top > 0) { SetRect(&bar, 0, 0, client_width, top); FillRect(device_context, &bar, black); }
    if(bottom < client_height) { SetRect(&bar, 0, bottom, client_width, client_height); FillRect(device_context, &bar, black); }
    if(left > 0) { SetRect(&bar, 0, top, left, bottom); FillRect(device_context, &bar, black); }
    if(right < client_width) { SetRect(&bar, right, top, client_width, bottom); FillRect(device_context, &bar, black); }
}

static void Win_Blit_Back_Buffer(struct win32_offscreen_buffer *buffer, HDC device_context, int window_width, int window_height)
{
    // StretchDIBits(device_context, 0, 0, window_width, window_height, 0, 0, buffer->Width, buffer->Height, buffer->Memory, &buffer->Info, DIB_RGB_COLORS, SRCCOPY);
    /* BEGIN:   CLAUDE */
    /* PLAN-Fullscreen.md phase 8: the image in the viewport's rect, black bars around it, per scale mode. */
    (void)window_width;
    (void)window_height;
    Win_Viewport_Refresh();
    Win_Fill_Bars(device_context);
    if((win_viewport.scale_mode == VIEWPORT_SCALE_MODE_FIT_SHARP_BILINEAR) && (Win_Sharp_Bilinear_Ready(device_context) != 0))
    {
        StretchDIBits(win_upscaled_dc, 0, 0, PLATFORM_SCREEN_WIDTH * win_upscaled_scale, PLATFORM_SCREEN_HEIGHT * win_upscaled_scale, 0, 0, buffer->Width, buffer->Height, buffer->Memory, &buffer->Info, DIB_RGB_COLORS, SRCCOPY);
        SetStretchBltMode(device_context, HALFTONE);
        SetBrushOrgEx(device_context, 0, 0, NULL);  /* required after selecting HALFTONE */
        StretchBlt(device_context, win_viewport.rect_x, win_viewport.rect_y, win_viewport.rect_width, win_viewport.rect_height, win_upscaled_dc, 0, 0, PLATFORM_SCREEN_WIDTH * win_upscaled_scale, PLATFORM_SCREEN_HEIGHT * win_upscaled_scale, SRCCOPY);
        return;
    }
    SetStretchBltMode(device_context, COLORONCOLOR);  /* nearest-neighbour: integer and fit_nearest */
    StretchDIBits(device_context, win_viewport.rect_x, win_viewport.rect_y, win_viewport.rect_width, win_viewport.rect_height, 0, 0, buffer->Width, buffer->Height, buffer->Memory, &buffer->Info, DIB_RGB_COLORS, SRCCOPY);
    /* END:     CLAUDE */
}

void Platform_Video_Update(void)
{
    assert(video_page_buffer[draw_page_num] != NULL);
    assert(win_video_back_buffer.Memory != NULL);

    /* CLAUDE: demo capture -- submit the finished frame before presenting it. */
    if(Platform_Capture_Active())
    {
        Platform_Capture_Video_Frame(video_page_buffer[draw_page_num], screen_pixel_width, screen_pixel_height, platform_palette_buffer);
    }

    Win_Convert_Engine_Pixels_To_Back_Buffer(&win_video_back_buffer);

    /* CLAUDE: metrics -- bracket the present (StretchDIBits blit) so block time and inter-present
       interval are measured.  Off path is one branch; the two clock reads only run when enabled. */
    if(Input_Metrics_Active())
    {
        uint64_t im_t0 = Platform_Get_Millies();
        Win_Blit_Back_Buffer(&win_video_back_buffer, win_device_context, win_window_width, win_window_height);
        Input_Metrics_Record_Present(im_t0, Platform_Get_Millies());
    }
    else
    {
        Win_Blit_Back_Buffer(&win_video_back_buffer, win_device_context, win_window_width, win_window_height);
    }

    /* CLAUDE: live frame-time readout -- one subtraction per present, one OS call per second. */
    Perf_Live_Note_Present();
}



/* ========================================================================= */
/*  Events / Input                                                           */
/* ========================================================================= */

/**
 * @brief Process one platform input frame for the Win32 backend.
 *
 * Pumps pending Win32 messages, clears the per-frame mouse button state,
 * optionally invokes the registered synthetic-input callback, and then records
 * the resolved input frame when replay recording is active.
 *
 * This function is the main per-frame input entry point for the Win32 platform
 * layer. It updates global platform input state as a side effect and does not
 * return a value.
 *
 * @note The synthetic frame callback is intended for scripted input injection
 * and is executed after OS events are processed.
 *
 * @note Replay capture occurs after message pumping so the recorded frame
 * matches the final input state seen by the engine for that frame.
 */

void Platform_Event_Handler(void)
{
    if(Input_Metrics_Active()) { Input_Metrics_Note_Tick(Platform_Get_Millies()); }  /* CLAUDE: metrics tick */
#ifdef MOUSE_DEBUG
    MOUSE_LOG("MOUSEt=%llu HANDLER_START ptr=%d,%d\n", (unsigned long long)Platform_Get_Millies(), pointer_x, pointer_y);
#endif

    /* CLAUDE: do NOT reset platform_frame_mouse_buttons here.  Clicks that arrive during Release_Time's Platform_Pump_Events spin-wait are OR'd into this flag between frames.  Resetting at the top would clobber them before Replay_Capture_Frame could read them.  Reset is now done at the END of this function, after capture. */

    /* CLAUDE: Replay — if active, inject recorded frame instead of pumping OS events.  Matches sdl2/sdl3/windowless backends.  Without this, --replay on win32 opens the RMR but never injects any frames. */
    if(Platform_Replay_Active())
    {
        MSG cancel_msg;
        int cancelled = 0;
        if(PeekMessageA(&cancel_msg, 0, WM_KEYDOWN, WM_KEYDOWN, PM_NOREMOVE))
        {
            cancelled = 1;
        }
        else if(PeekMessageA(&cancel_msg, 0, WM_LBUTTONDOWN, WM_LBUTTONDOWN, PM_NOREMOVE))
        {
            cancelled = 1;
        }
        else if(PeekMessageA(&cancel_msg, 0, WM_RBUTTONDOWN, WM_RBUTTONDOWN, PM_NOREMOVE))
        {
            cancelled = 1;
        }

        if(cancelled)
        {
            LOG_INFO(LOG_CAT_WIN_PFL, "REPLAY: cancelled by user input");
            Platform_Replay_Stop();
            /* Fall through to live input. */
        }
        else
        {
            if(Replay_Inject_Frame())
            {
                /* Frame injected successfully — skip pump/capture this tick. */
                platform_frame_mouse_buttons = 0;
                return;
            }
            /* Replay ended — fall through to live input. */
        }
    }

    Win_Pump_Messages();

    /* Artificial human player: inject scripted input. */
    if(platform_frame_callback != NULL)
    {
        platform_frame_callback();
    }

    STU_Log_Pump();

    /* CLAUDE: Record — capture input state after processing OS events.  Matches sdl2/sdl3/windowless backends.  Without this, --record produces an empty RMR on win32. */
    if(Platform_Record_Active())
    {
        Replay_Capture_Frame();
    }

    /* CLAUDE: reset per-frame click edge AFTER capture so clicks arriving during the next spin-wait accumulate into the following frame's edge. */
    platform_frame_mouse_buttons = 0;
}

/* CLAUDE: Pump events AND refresh cursor, matching SDL backends. */
void Platform_Pump_Events(void)
{
    if(Input_Metrics_Active()) { Input_Metrics_Note_Tick(Platform_Get_Millies()); }  /* CLAUDE: metrics tick */
    Win_Pump_Messages();
    Platform_Maybe_Move_Mouse();
}

void Win_Pump_Messages(void)
{
    MSG message;

    while(PeekMessageA(&message, 0, 0, 0, PM_REMOVE))
    {
        if(message.message == WM_QUIT)
        {
            quit_game_flag = ST_TRUE;
        }
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }
}

void Platform_Mouse_Input_Enable(void)
{
    platform_mouse_input_enabled = ST_TRUE;
}

void Platform_Mouse_Input_Disable(void)
{
    platform_mouse_input_enabled = ST_FALSE;
}



/* ========================================================================= */
/*  Legacy Win32 API (declared in Platform.h under #ifdef _STU_WIN)          */
/* ========================================================================= */

void Pump_Events(void)
{
    Win_Pump_Messages();
}

void Pump_Paints(void)
{
    InvalidateRect(win_window, NULL, FALSE);
    MSG message;
    while(PeekMessageA(&message, 0, WM_PAINT, WM_PAINT, PM_REMOVE))
    {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }
}



/* ========================================================================= */
/*  Mouse                                                                    */
/* ========================================================================= */

/* CLAUDE: synthetic (injected) mouse button for Automation Driver / replay click playback -- see Platform.h. The Automation Driver
   has called this since 986d71c7 (2026-07-29) but the Win32 backend never had it, so Win32 did not link; same as sdl2_MD.c. */
static int16_t win_synthetic_mouse_button = 0;
static int16_t win_synthetic_mouse_button_hold = 0;

void Platform_Set_Synthetic_Mouse_Button(int16_t buttons)
{
    win_synthetic_mouse_button = buttons;
    win_synthetic_mouse_button_hold = (buttons != 0) ? PLATFORM_SYNTHETIC_MOUSE_HOLD : 0;
}

int16_t Platform_Get_Mouse_Button_State(void)
{
    int16_t buttons = 0;
    if(GetAsyncKeyState(VK_LBUTTON) & 0x8000) { buttons |= ST_LEFT_BUTTON; }
    if(GetAsyncKeyState(VK_RBUTTON) & 0x8000) { buttons |= ST_RIGHT_BUTTON; }
    /* CLAUDE: OR in the synthetic press, returning it BEFORE decrementing the hold so this read still sees it; auto-release when the
       hold expires so the caller's release spin-wait exits. */
    if(win_synthetic_mouse_button != 0)
    {
        buttons |= win_synthetic_mouse_button;
        if(win_synthetic_mouse_button_hold > 0)
        {
            win_synthetic_mouse_button_hold--;
            if(win_synthetic_mouse_button_hold == 0)
            {
                win_synthetic_mouse_button = 0;
            }
        }
    }
    return buttons;
}

void Platform_Warp_Mouse(int16_t game_x, int16_t game_y)
{
    // int scale = Platform_Get_Scale();
    int window_x = 0;  /* CLAUDE */
    int window_y = 0;  /* CLAUDE */
    POINT point;
    // point.x = (LONG)(game_x * scale);
    // point.y = (LONG)(game_y * scale);
    Platform_Game_To_Window(game_x, game_y, &window_x, &window_y);  /* CLAUDE: through the viewport (letterbox, fit modes) */
    point.x = (LONG)window_x;  /* CLAUDE */
    point.y = (LONG)window_y;  /* CLAUDE */
    ClientToScreen(win_window, &point);
    SetCursorPos(point.x, point.y);
}

void MWA_Set_Mouse_Position(int16_t x, int16_t y)
{
    Platform_Warp_Mouse(x, y);
}

int Platform_Get_Window_Width(void)
{
    RECT client_rect;
    GetClientRect(win_window, &client_rect);
    return client_rect.right - client_rect.left;
}

/* CLAUDE: window <-> game through the shared Viewport (letterbox, fit modes). PLAN-Fullscreen.md phase 8. */
int16_t Platform_Window_To_Game(int window_x, int window_y, int16_t * game_x, int16_t * game_y)
{
    int gx = 0;
    int gy = 0;
    int on_image = Viewport_Window_To_Game(&win_viewport, window_x, window_y, &gx, &gy);
    *game_x = (int16_t)gx;
    *game_y = (int16_t)gy;
    return (on_image != 0) ? ST_TRUE : ST_FALSE;
}

void Platform_Game_To_Window(int16_t game_x, int16_t game_y, int * window_x, int * window_y)
{
    Viewport_Game_To_Window(&win_viewport, game_x, game_y, window_x, window_y);
}

/* Win32: the OS cursor position in GAME coordinates (unlike the SDL backends, which return window coordinates). */
void Platform_Get_Mouse_Position_XY(int *mx, int *my)
{
    // int scale = Platform_Get_Scale();
    int16_t game_x = 0;  /* CLAUDE */
    int16_t game_y = 0;  /* CLAUDE */
    POINT cursor;
    GetCursorPos(&cursor);
    ScreenToClient(win_window, &cursor);

    // if(scale < 1) { scale = 1; }
    // *mx = (int)(cursor.x / scale);
    // *my = (int)(cursor.y / scale);
    //
    // SETRANGE(*mx, PLATFORM_SCREEN_XMIN, PLATFORM_SCREEN_XMAX);
    // SETRANGE(*my, PLATFORM_SCREEN_YMIN, PLATFORM_SCREEN_YMAX);
    Platform_Window_To_Game((int)cursor.x, (int)cursor.y, &game_x, &game_y);  /* CLAUDE: through the viewport, clamped to the image */
    *mx = game_x;  /* CLAUDE */
    *my = game_y;  /* CLAUDE */
}

void Platform_Maybe_Move_Mouse(void)
{
    int mx, my;
    int16_t smx, smy;

    if(platform_mouse_input_enabled == ST_FALSE) { return; }

    Platform_Get_Mouse_Position_XY(&mx, &my);
    smx = (int16_t)mx;
    smy = (int16_t)my;

    /* CLAUDE: metrics -- note every sample (moved or not) so net motion between presents is correct. */
    if(Input_Metrics_Active()) { Input_Metrics_Note_Poll(smx, smy); }

    /* CLAUDE: HW-cursor prototype -- the OS cursor tracks natively, so skip the software redraw/present
       entirely; just keep the engine's pointer position and cursor shape current for hit-testing.
       Mirrors sdl2_PFL.c Platform_Maybe_Move_Mouse. */
    if(Platform_HW_Cursor_Active())
    {
        pointer_x = smx;
        pointer_y = smy;
        if(current_mouse_list_count >= 2) { Check_Mouse_Shape(smx, smy); }
        Platform_HW_Cursor_Refresh();
        return;
    }

    if(smx != pointer_x || smy != pointer_y)
    {
#ifdef MOUSE_DEBUG
        MOUSE_LOG("MOUSEt=%llu MAYBE_MOVE ptr=%d,%d -> %d,%d\n", (unsigned long long)Platform_Get_Millies(), pointer_x, pointer_y, smx, smy);
#endif
        if(mouse_enabled == ST_TRUE)
        {
            Restore_Mouse_On_Page();
        }
        pointer_x = smx;
        pointer_y = smy;
        if(mouse_enabled == ST_TRUE)
        {
            Check_Mouse_Shape(smx, smy);
            Save_Mouse_On_Page(smx, smy);
            Draw_Mouse_On_Page(smx, smy);
        }
    }
}

void Hw_Textinput_Start(void)
{
    /* Win32: no IME integration needed for this game */
}

void Hw_Textinput_Stop(void)
{
    /* Win32: no IME integration needed for this game */
}



/* ========================================================================= */
/*  Audio (stub — Win32 backend does not implement audio)                    */
/* ========================================================================= */

int16_t Platform_Audio_Play_Sound(void *sound_buffer, uint32_t sound_buffer_size)
{
    (void)sound_buffer;
    (void)sound_buffer_size;
    return -1;
}



/* ========================================================================= */
/*  Window Initialization                                                    */
/* ========================================================================= */

static void Win_Init_Back_Buffer(struct win32_offscreen_buffer *buffer, int width, int height)
{
    int bitmap_memory_size;

    if(buffer->Memory != NULL)
    {
        VirtualFree(buffer->Memory, 0, MEM_RELEASE);
    }

    buffer->BytesPerPixel = 4;
    buffer->Info.bmiHeader.biSize = sizeof(buffer->Info.bmiHeader);
    buffer->Info.bmiHeader.biPlanes = 1;
    buffer->Info.bmiHeader.biBitCount = 32;
    buffer->Info.bmiHeader.biCompression = BI_RGB;
    buffer->Width = width;
    buffer->Height = height;
    buffer->Pitch = buffer->Width * buffer->BytesPerPixel;
    buffer->Info.bmiHeader.biWidth = buffer->Width;
    buffer->Info.bmiHeader.biHeight = -buffer->Height;  /* negative value: top-down */

    bitmap_memory_size = buffer->BytesPerPixel * (buffer->Width * buffer->Height);
    buffer->Memory = VirtualAlloc(NULL, bitmap_memory_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    assert(buffer->Memory != NULL && "VirtualAlloc failed for video back buffer");

    memset(buffer->Memory, 0, bitmap_memory_size);
}

static void Win_Init_Window(void)
{
    WNDCLASSEX wcex = { 0 };
    RECT rect;
    DWORD style;

    win_instance = GetModuleHandle(NULL);

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = Win_Window_Proc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = win_instance;
    wcex.hIcon = NULL;
    wcex.hCursor = NULL;
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = win_window_class_name;
    wcex.hIconSm = NULL;
    RegisterClassEx(&wcex);

    rect.left = 0;
    rect.top = 0;
    rect.right = win_window_width;
    rect.bottom = win_window_height;
    style = WS_OVERLAPPEDWINDOW;
    AdjustWindowRectEx(&rect, style, FALSE, 0);

    win_window = CreateWindowEx(0, win_window_class_name, win_window_title, style, CW_USEDEFAULT, CW_USEDEFAULT, (rect.right - rect.left), (rect.bottom - rect.top), NULL, NULL, win_instance, NULL);
    assert(win_window != NULL && "CreateWindowEx failed");

    ShowWindow(win_window, SW_SHOW);
    UpdateWindow(win_window);

    win_device_context = GetDC(win_window);  /* requires CS_OWNDC */
}



/* ========================================================================= */
/*  Window Procedure                                                         */
/* ========================================================================= */

static LRESULT CALLBACK Win_Window_Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            RECT client_rect;
            int client_width;
            int client_height;

            HDC hdc = BeginPaint(hWnd, &ps);
            GetClientRect(hWnd, &client_rect);
            client_width = client_rect.right - client_rect.left;
            client_height = client_rect.bottom - client_rect.top;
            /* Blit only the platform back buffer.  No engine state touched. */
            Win_Blit_Back_Buffer(&win_video_back_buffer, hdc, client_width, client_height);
            EndPaint(hWnd, &ps);
        } break;


        case WM_MOUSEMOVE:
        {
            if(platform_mouse_input_enabled)
            {
                /* CLAUDE: User_Mouse_Handler() expects window-space coords and divides by scale internally.  Passing pre-scaled (game-space) coords double-scales and causes the cursor to jump up-left before Platform_Maybe_Move_Mouse() corrects it. */
                int16_t win_x = (int16_t)GET_X_LPARAM(lParam);
                int16_t win_y = (int16_t)GET_Y_LPARAM(lParam);
#ifdef MOUSE_DEBUG
                int scale = Platform_Get_Scale();
                if(scale < 1) { scale = 1; }
                MOUSE_LOG("MOUSEt=%llu WM_MOUSEMOVE wx=%d wy=%d gx=%d gy=%d\n", (unsigned long long)Platform_Get_Millies(), (int)win_x, (int)win_y, (int)(win_x / scale), (int)(win_y / scale));
#endif
                User_Mouse_Handler(0, win_x, win_y);
            }
        } break;


        case WM_SYSKEYDOWN:
        {
            /* CLAUDE: PLAN-Fullscreen.md phase 8 -- Alt+Enter comes here (Alt makes it a system key). Stopped: never reaches the game. */
            if(Win_Window_Hotkey((int)wParam, lParam) != 0)
            {
                return 0;
            }
            /* CLAUDE: diagnostic — some keys (F10, Alt+key, etc.) arrive as WM_SYSKEYDOWN rather than WM_KEYDOWN.  Log them so we can see if missed keys are landing here. */
#ifdef STU_DEBUG
            LOG_TRACE(LOG_CAT_PFL, "WIN_KEY t=%llu WM_SYSKEYDOWN vk=0x%02X (%d) wParam=0x%llX lParam=0x%llX repeat=%d", (unsigned long long)Platform_Get_Millies(), (int)wParam, (int)wParam, (unsigned long long)wParam, (unsigned long long)lParam, (int)(lParam & 0xFFFF));
#endif
            return DefWindowProc(hWnd, message, wParam, lParam);
        }

        /* CLAUDE: the character for Alt+Enter -- swallowed, or DefWindowProc beeps (there is no menu for it to select). */
        case WM_SYSCHAR:
        {
            if(wParam == VK_RETURN)
            {
                return 0;
            }
            return DefWindowProc(hWnd, message, wParam, lParam);
        }

        case WM_KEYDOWN:
        {
            int vk_code = (int)wParam;
            int mox_key = 0;
            uint32_t mox_mod = 0;
            char mox_character = 0;

            /* CLAUDE: PLAN-Fullscreen.md phase 8 -- Ctrl+L / Ctrl+H. Stopped: never translated, queued or recorded. */
            if(Win_Window_Hotkey(vk_code, lParam) != 0)
            {
                break;
            }

            if(vk_code >= 0 && vk_code < 256)
            {
                mox_key = VK_to_MOX_KEY[vk_code];
            }

            if(GetKeyState(VK_SHIFT) & 0x8000) { mox_mod |= MOX_MOD_SHIFT; }
            if(GetKeyState(VK_CONTROL) & 0x8000) { mox_mod |= MOX_MOD_CTRL; }
            if(GetKeyState(VK_MENU) & 0x8000) { mox_mod |= MOX_MOD_ALT; }

            /* CLAUDE: diagnostic — trace raw WM_KEYDOWN before translation, to help find keys that are received but dropped. */
#ifdef STU_DEBUG
            LOG_TRACE(LOG_CAT_PFL, "WIN_KEY t=%llu WM_KEYDOWN vk=0x%02X (%d) mod=0x%X mox_key=0x%X repeat=%d", (unsigned long long)Platform_Get_Millies(), vk_code, vk_code, (unsigned)mox_mod, (unsigned)mox_key, (int)(lParam & 0xFFFF));
#endif

            /* Ctrl+Alt+Q to quit */
            if((mox_mod & MOX_MOD_CTRL) && (mox_mod & MOX_MOD_ALT) && vk_code == 'Q')
            {
                PostQuitMessage(0);
            }

            /* Generate character code for printable keys */
            if(vk_code >= 'A' && vk_code <= 'Z')
            {
                mox_character = (char)(vk_code + 32);  /* lowercase */
                if(mox_mod & MOX_MOD_SHIFT) { mox_character = (char)vk_code; }  /* uppercase */
            }
            else if(vk_code >= '0' && vk_code <= '9')
            {
                mox_character = (char)vk_code;
            }
            else if(vk_code == VK_OEM_MINUS || vk_code == VK_SUBTRACT)
            {
                mox_character = '-';
            }
            else if(vk_code == VK_OEM_PLUS || vk_code == VK_ADD)
            {
                mox_character = '+';
                if(vk_code == VK_OEM_PLUS && !(mox_mod & MOX_MOD_SHIFT)) { mox_character = '='; }
            }
            else if(vk_code == VK_SPACE)
            {
                mox_character = ' ';
            }
            /* CLAUDE: Backspace, Enter, and Tab leave mox_character at 0 so Read_Key()
               returns the MoX key code (e.g., ST_KEY_BACKSPACE = 0x0B), not the ASCII
               character (e.g., '\b' = 0x08). Setup_Input_Box_Popup() and other input
               loops switch on the MoX key code, not the ASCII byte.
               Escape happens to work either way: ST_KEY_ESCAPE == 0x1B == ASCII ESC. */
            else if(vk_code == VK_ESCAPE)
            {
                mox_character = 0x1B;
            }

            /* Also set legacy scan_code_char_code for backwards compatibility */
            if(vk_code >= 0 && vk_code < 256)
            {
                /* CLAUDE */  /* Legacy: populate scan_code_char_code from the old VK_to_SCCS table pattern */
                /* The packed_key / keyboard buffer system is the primary path now */
            }

            /* CLAUDE: diagnostic — log whether the key survived translation and reached the keyboard buffer, or was dropped (and why). */
            if(mox_key != MOX_KEY_UNKNOWN && mox_key < MOX_KEY_OVERRUN)
            {
#ifdef STU_DEBUG
                LOG_TRACE(LOG_CAT_PFL, "WIN_KEY t=%llu BUFFERED vk=0x%02X mox_key=0x%X mox_mod=0x%X mox_char=0x%02X ('%c')", (unsigned long long)Platform_Get_Millies(), vk_code, (unsigned)mox_key, (unsigned)mox_mod, (unsigned char)mox_character, (mox_character >= 0x20 && mox_character < 0x7F) ? mox_character : '.');
#endif
                Platform_Keyboard_Buffer_Add_Key_Press(mox_key, mox_mod, mox_character);
            }
            else
            {
#ifdef STU_DEBUG
                LOG_TRACE(LOG_CAT_PFL, "WIN_KEY t=%llu DROPPED vk=0x%02X mox_key=0x%X reason=%s", (unsigned long long)Platform_Get_Millies(), vk_code, (unsigned)mox_key, (mox_key == MOX_KEY_UNKNOWN) ? "UNKNOWN" : "OVERRUN");
#endif
            }
        } break;


        case WM_LBUTTONDOWN:
        {
            if(platform_mouse_input_enabled)
            {
                /* CLAUDE: pass window-space coords; User_Mouse_Handler divides by scale internally.  See WM_MOUSEMOVE note. */
                int16_t win_x = (int16_t)GET_X_LPARAM(lParam);
                int16_t win_y = (int16_t)GET_Y_LPARAM(lParam);
#ifdef MOUSE_DEBUG
                int scale = Platform_Get_Scale();
                if(scale < 1) { scale = 1; }
                MOUSE_LOG("MOUSEt=%llu BTN_DOWN btn=1 wx=%d wy=%d gx=%d gy=%d\n", (unsigned long long)Platform_Get_Millies(), (int)win_x, (int)win_y, (int)(win_x / scale), (int)(win_y / scale));
#endif
#ifdef STU_DEBUG
                LOG_TRACE(LOG_CAT_PFL, "WIN_BTN t=%llu WM_LBUTTONDOWN wx=%d wy=%d prev_edge=0x%X", (unsigned long long)Platform_Get_Millies(), (int)win_x, (int)win_y, (unsigned)platform_frame_mouse_buttons);
#endif
                platform_frame_mouse_buttons |= 1;
                if(win_mouse_captured == 0) { Win_Mouse_Capture(1); }  /* CLAUDE: fullscreen re-captures on a click after a middle-click release */
                User_Mouse_Handler(ST_LEFT_BUTTON, win_x, win_y);
            }
        } break;

        case WM_RBUTTONDOWN:
        {
            if(platform_mouse_input_enabled)
            {
                /* CLAUDE: pass window-space coords; User_Mouse_Handler divides by scale internally.  See WM_MOUSEMOVE note. */
                int16_t win_x = (int16_t)GET_X_LPARAM(lParam);
                int16_t win_y = (int16_t)GET_Y_LPARAM(lParam);
#ifdef MOUSE_DEBUG
                int scale = Platform_Get_Scale();
                if(scale < 1) { scale = 1; }
                MOUSE_LOG("MOUSEt=%llu BTN_DOWN btn=2 wx=%d wy=%d gx=%d gy=%d\n", (unsigned long long)Platform_Get_Millies(), (int)win_x, (int)win_y, (int)(win_x / scale), (int)(win_y / scale));
#endif
#ifdef STU_DEBUG
                LOG_TRACE(LOG_CAT_PFL, "WIN_BTN t=%llu WM_RBUTTONDOWN wx=%d wy=%d prev_edge=0x%X", (unsigned long long)Platform_Get_Millies(), (int)win_x, (int)win_y, (unsigned)platform_frame_mouse_buttons);
#endif
                platform_frame_mouse_buttons |= 2;
                if(win_mouse_captured == 0) { Win_Mouse_Capture(1); }  /* CLAUDE: as WM_LBUTTONDOWN */
                User_Mouse_Handler(ST_RIGHT_BUTTON, win_x, win_y);
            }
        } break;

        /* CLAUDE: middle-click releases the captured mouse, as in the SDL backends. */
        case WM_MBUTTONDOWN:
        {
            Win_Mouse_Capture(0);
        } break;


        case WM_SIZE:
        {
            RECT client_rect;
            GetClientRect(win_window, &client_rect);
            win_window_width = client_rect.right - client_rect.left;
            win_window_height = client_rect.bottom - client_rect.top;
            /* CLAUDE: PLAN-Fullscreen.md phase 8 -- re-fit the image and repaint the bars at the new size. */
            Win_Viewport_Refresh();
            InvalidateRect(hWnd, NULL, FALSE);
        } break;

        /* CLAUDE: moved: a captured mouse's clip rect is in screen coordinates, so it follows the window. */
        case WM_MOVE:
        {
            Win_Mouse_Confine_Update();
        } break;

        /* CLAUDE: the monitor's resolution changed under us: fullscreen / borderless re-fit to it. */
        case WM_DISPLAYCHANGE:
        {
            int current_state = win_window_state;
            if(current_state != VIEWPORT_WINDOW_STATE_WINDOWED)
            {
                win_window_state = VIEWPORT_WINDOW_STATE_WINDOWED;  /* force the re-apply below */
                Win_Set_Window_State(current_state);
            }
        } break;

        case WM_ACTIVATEAPP:
        {
            /* CLAUDE: Match SDL behavior — enable/disable mouse input on focus change.
               wParam is TRUE when the window is being activated, FALSE on deactivation. */
            if(wParam)
            {
                Platform_Mouse_Input_Enable();
                Win_Mouse_Capture(1);  /* CLAUDE: back from Alt+Tab: fullscreen recaptures straight away (no-op when windowed) */
            }
            else
            {
                Platform_Mouse_Input_Disable();
                Win_Mouse_Capture(0);  /* CLAUDE: ClipCursor is system-wide -- free the cursor for the other application */
            }
        } break;

        case WM_SETCURSOR:
        {
            /* CLAUDE: HW-cursor prototype -- Windows resets to the class cursor (NULL) on every move
               over the client area; re-assert our cursor and swallow the message so it isn't cleared.
               Inactive -> fall through to default (software cursor is hidden, class cursor NULL). */
            if(Platform_HW_Cursor_Active() && LOWORD(lParam) == HTCLIENT)
            {
                Win_HW_Cursor_Apply();
                return TRUE;
            }
            return DefWindowProc(hWnd, message, wParam, lParam);
        }

        case WM_CREATE:
        {
            /* CLAUDE: Enable mouse input on window creation so it works from the first frame
               (WM_ACTIVATEAPP also enables it on focus, but that may not fire before first input). */
            Platform_Mouse_Input_Enable();
            /* CLAUDE: HW-cursor prototype shows the OS cursor; the default path hides it and software-draws. */
            if(!Platform_HW_Cursor_Active())
            {
                ShowCursor(FALSE);
            }
        } break;

        case WM_CLOSE:
        {
            quit_game_flag = ST_TRUE;
        } break;

        case WM_DESTROY:
        {
            PostQuitMessage(0);
        } break;


        default:
            return DefWindowProc(hWnd, message, wParam, lParam);

    }

    return 0;
}
