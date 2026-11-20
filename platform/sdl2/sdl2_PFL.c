
#include "sdl2_PFL.h"

#include "../../platform/include/Platform_Replay.h"
#include "../../platform/include/Platform_Input_Metrics.h"  /* CLAUDE: Platform-Input Layer 1 metrics */

#include "sdl2_MOM.h"

#include "Platform.h"

#include <SDL.h>

#include <stdbool.h>
#include <stdio.h>   /* CLAUDE: debug */
#include "../../STU/src/STU_DBG.h"
#include "../../STU/src/STU_LOG.h"
#include <stdlib.h>
#include <inttypes.h>

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


void Platform_Show_Error(const char * title, const char * message)
{
    /* stderr first so the message survives even when no display is available
       (e.g. a --offscreen run); then the GUI box for interactive players. */
    fprintf(stderr, "%s: %s\n", title, message);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, message, NULL);
}


// static SDL_Window * sdl2_window;                    // static SDL_Window *screen;
SDL_Window * sdl2_window;
SDL_Renderer * sdl2_renderer;                // static SDL_Renderer *renderer;
SDL_Surface * sdl2_surface_RGB666 = NULL;    // static SDL_Surface *screenbuffer = NULL;
SDL_Surface * sdl2_surface_ARGB8888 = NULL;  // static SDL_Surface *argbbuffer = NULL;
SDL_Texture * sdl2_texture = NULL;           // static SDL_Texture *texture = NULL;
SDL_Texture * sdl2_texture_upscaled = NULL;  //

int platform_screen_scale = 4;
int sdl2_window_width  = PLATFORM_SCREEN_WIDTH * 4;
int sdl2_window_height = PLATFORM_SCREEN_HEIGHT * 4;

char sdl2_window_title[] = "(MoM-Rasm) Master of Magic v1.31 - Reassembly (v" REMOM_VERSION_STRING ")";  // static const char *window_title = "";

SDL_Rect sdl2_blit_rect = {0, 0, PLATFORM_SCREEN_WIDTH, PLATFORM_SCREEN_HEIGHT};  // static SDL_Rect blit_rect = { 0, 0, SCREENWIDTH, SCREENHEIGHT};

uint64_t sdl2_ticks_startup;
uint64_t sdl2_frame_count;
uint64_t sdl2_ticks_mark_time;
uint64_t sdl2_ticks_release_time;

uint64_t sdl2_performance_counter;

// MOM_PFL.C  // YNM  int16_t platform_mouse_click_x;
// MOM_PFL.C  // YNM  int16_t platform_mouse_click_y;
// MOM_PFL.C  int platform_mouse_button_status;
// MOM_PFL.C  // left, right, middle, x1, x2; wheel;
// MOM_PFL.C  // MOM_PFL.C  int16_t lock_mouse_button_status_flag = ST_FALSE;

/* Per-frame mouse click flag: set when BUTTON_DOWN occurs this frame, cleared at
   the start of each Platform_Event_Handler() call.  Unlike mouse_buffer_button
   (which is sticky and never clears), this is 1 only on the frame where the
   click actually happened.  Used by Replay_Capture_Frame() for recording. */
int16_t platform_frame_mouse_buttons = 0;

/* Optional per-frame callback for synthetic input injection (DeMoM). */
static void (*platform_frame_callback)(void) = NULL;

void Platform_Register_Frame_Callback(void (*callback)(void))
{
    platform_frame_callback = callback;
}


int Platform_Get_Scale(void)
{
    /* The viewport's whole-number scale (game pixel -> drawable pixels). Was the fixed platform_screen_scale (4). */
    return sdl2_viewport.scale;
}

uint64_t Platform_Get_Millies(void)
{
    return (uint64_t)SDL_GetTicks();
}

/* Microsecond clock for perf instrumentation -- see Platform.h.  SDL_GetTicks() is already
 * QPC-backed on Windows, but it returns milliseconds, which cannot resolve a sub-frame zone. */
uint64_t Platform_Get_Micros(void)
{
    static uint64_t sdl2_micros_startup = 0;
    uint64_t freq = (uint64_t)SDL_GetPerformanceFrequency();
    uint64_t now;

    if(freq == 0) { return (uint64_t)SDL_GetTicks() * 1000u; }  /* no perf counter: fall back, coarse but monotonic */

    now = (uint64_t)SDL_GetPerformanceCounter();
    /* Split the division so a long session cannot overflow the numerator. */
    now = (now / freq) * 1000000u + ((now % freq) * 1000000u) / freq;

    if(sdl2_micros_startup == 0)
    {
        sdl2_micros_startup = now;
    }
    return now - sdl2_micros_startup;
}

/* CLAUDE: runtime window title -- see Platform.h.  No-ops before the window exists. */
void Platform_Set_Window_Title(const char * title)
{
    if(sdl2_window == NULL || title == NULL) { return; }
    SDL_SetWindowTitle(sdl2_window, title);
}

void Platform_Sleep_Millies(uint64_t millies)
{
    SDL_Delay((uint32_t)millies);
}

void Platform_Get_Mouse_Position_XY(int * mx, int * my)
{
    int ix, iy;
    SDL_GetMouseState(&ix, &iy);
    *mx = ix;
    *my = iy;
}

/* CLAUDE: Warp guard — after Platform_Warp_Mouse() calls SDL_WarpMouseInWindow(), SDL_GetMouseState()
   may briefly return the OLD position before the warp takes effect.  We skip OS polling for a short
   window after a warp to prevent the cursor from bouncing back to its pre-warp position. */
static uint64_t pfl_warp_guard_until = 0;

void Platform_Set_Warp_Guard(void)
{
    pfl_warp_guard_until = (uint64_t)SDL_GetTicks() + 100;
}

/* CLAUDE: Cursor refresh — decoupled from the game loop.
 *
 * Previously this function read the stale pointer_x/pointer_y (set only when Platform_Event_Handler
 * ran) and unconditionally did save/draw/present/restore every call — even when the cursor hadn't
 * moved.  This meant the cursor was frozen for 55–125ms+ between game frames.
 *
 * Now it polls SDL_GetMouseState() for the real OS cursor position, converts to game coordinates,
 * and only redraws + presents when the cursor has actually moved.  This matches Win32's pattern
 * (which already polled GetCursorPos directly) and lets the cursor track smoothly even during
 * long game logic frames, as long as Platform_Pump_Events() is called periodically. */
void Platform_Maybe_Move_Mouse(void)
{
    int wx, wy;
    int gx, gy;

    if(Get_Pointer_Image_Number() == 0)  /* crsr_None — no cursor to draw */
    {
        return;
    }

    /* Poll the OS for the current mouse position unless we just warped. */
    if((uint64_t)SDL_GetTicks() < pfl_warp_guard_until)
    {
        /* Warp guard active — use the engine's position (set by the warp caller). */
        gx = pointer_x;
        gy = pointer_y;
    }
    else
    {
        int ix, iy;
        SDL_GetMouseState(&ix, &iy);
        int16_t game_x = 0;
        int16_t game_y = 0;
        wx = ix;
        wy = iy;
        /* Through the viewport (letterbox, high-DPI); off the image the pointer stays on the nearest edge. */
        Platform_Window_To_Game(wx, wy, &game_x, &game_y);
        sdl2_Mouse_Keep_On_Image(wx, wy);
        gx = game_x;
        gy = game_y;
    }

    /* CLAUDE: metrics -- note every sample (moved or not) so net motion between presents is correct. */
    if(Input_Metrics_Active()) { Input_Metrics_Note_Poll(gx, gy); }

    /* CLAUDE: HW-cursor prototype -- the OS cursor tracks natively, so skip the software redraw/present
       entirely; just keep the engine's pointer position and cursor shape current for hit-testing. */
    if(Platform_HW_Cursor_Active())
    {
        pointer_x = gx;
        pointer_y = gy;
        if(current_mouse_list_count >= 2) { Check_Mouse_Shape(gx, gy); }
        Platform_HW_Cursor_Refresh();
        return;
    }

    /* Only redraw if the cursor has actually moved. */
    if(gx == pointer_x && gy == pointer_y)
    {
        return;
    }

#ifdef MOUSE_DEBUG
    MOUSE_LOG("MOUSEt=%llu MAYBE_MOVE ptr=%d,%d -> %d,%d img=%d\n", (unsigned long long)SDL_GetTicks(), pointer_x, pointer_y, gx, gy, Get_Pointer_Image_Number());
#endif

    if(mouse_enabled == ST_TRUE)
    {
        Restore_Mouse_On_Page();
    }
    pointer_x = gx;
    pointer_y = gy;
    if(mouse_enabled == ST_TRUE)
    {
        if(current_mouse_list_count >= 2)
        {
            Check_Mouse_Shape(gx, gy);
        }
        Save_Mouse_On_Page(gx, gy);
        Draw_Mouse_On_Page(gx, gy);
        if(Input_Metrics_Active()) { Input_Metrics_Set_Reason("cursor"); }  /* CLAUDE: tag this present as cursor-driven */
        Platform_Video_Update();
        Restore_Mouse_On_Page();
    }
}




// ./src/hw/sdl/hwsdl_opt.c
int pfl_option_mouse_speed = 100;

// ./src/hw/sdl/2/hwsdl2_video.c
static int pfl_video_get_window_id(void)
{

    return SDL_GetWindowID(sdl2_window);

}

// not a MoO1/MoM function
/*
    ...happens in User_Mouse_Handler()
    ...happens in Set_Pointer_Position()
    ¿ ~== Set_Pointer_Position(), w/o Set_Mouse_Position() ?
    ...as used, is effectively equivalent?
*/
static void pfl_mouse_set_xy_from_hw(int mx, int my)
{
    pointer_x = mx;
    pointer_y = my;
}

static int pfl_mouse_game_w = 320;
static int pfl_mouse_game_h = 200;

static int pfl_mouse_dx_acc = 0;  // accelerated mouse x
static int pfl_mouse_dy_acc = 0;  // accelerated mouse y
static int pfl_mouse_sx = 1;  // scaled mouse x
static int pfl_mouse_sy = 1;  // scaled mouse y

/* -------------------------------------------------------------------------- */

bool pfl_mouse_grabbed = false;

/* -------------------------------------------------------------------------- */

bool pfl_option_relative_mouse = true;

/* -------------------------------------------------------------------------- */

// hwsdl2_video2.c

// not a MoO1/MoM function
static void pfl_video_input_grab(bool grabbed)
{

    SDL_SetWindowGrab(sdl2_window, grabbed ? SDL_TRUE : SDL_FALSE);

    /* The hardware cursor tracks the real OS cursor, which relative mode would hide and detach -- grab and confine only. */
    if(pfl_option_relative_mouse && !Platform_HW_Cursor_Active())
    {

        SDL_SetRelativeMouseMode(grabbed ? SDL_TRUE : SDL_FALSE);

    }

    sdl2_Mouse_Confine_Update();  /* PLAN-Fullscreen.md phase 2: captured -> confined to the image, released -> free */

}

/* While the mouse is captured, confine it to the game image (not just the window), so it can never sit in a black bar where
   the drawn cursor would stick to the edge and clicks would be ignored. Released, the confinement is removed: SDL applies a
   mouse rect whenever the window has mouse focus, so leaving it set would trap the cursor in a windowed game. Called when the
   capture changes and whenever the viewport changes. */
void sdl2_Mouse_Confine_Update(void)
{
    SDL_Rect image_rect = { 0, 0, 0, 0 };

    if(sdl2_window == NULL)
    {
        return;
    }
    if(pfl_mouse_grabbed == false)
    {
        SDL_SetWindowMouseRect(sdl2_window, NULL);
        return;
    }
    Viewport_Image_Rect_In_Window(&sdl2_viewport, &image_rect.x, &image_rect.y, &image_rect.w, &image_rect.h);
    if(SDL_SetWindowMouseRect(sdl2_window, &image_rect) != 0)
    {
        LOG_WARN(LOG_CAT_SDL2_PFL, "graphics: could not confine the mouse to the image %d,%d %dx%d: %s", image_rect.x, image_rect.y, image_rect.w, image_rect.h, SDL_GetError());
    }
}

/* -------------------------------------------------------------------------- */

// hwsdl_mouse.c

// not a MoO1/MoM function
static void pfl_mouse_grab(void)
{
    /* CLAUDE: the hardware cursor (the default) keeps the OS cursor visible and never uses relative mode -- both would break
       native cursor tracking. Windowed it is never captured (click-to-capture is the software cursor's behaviour); fullscreen
       captures it with a window grab and the image-rect confinement only (PLAN-Fullscreen.md phase 2). */
    if(Platform_HW_Cursor_Active() && (sdl2_Is_Fullscreen() == 0)) { return; }
    if(!pfl_mouse_grabbed)
    {
        pfl_mouse_grabbed = true;
        if(!Platform_HW_Cursor_Active())
        {
            SDL_ShowCursor(SDL_DISABLE);
        }
        pfl_video_input_grab(true);
    }
}

// not a MoO1/MoM function
static void pfl_mouse_ungrab(void)
{
    /* CLAUDE: the hardware cursor is captured only in fullscreen, and its OS cursor was never hidden. */
    if(pfl_mouse_grabbed)
    {
        pfl_mouse_grabbed = false;
        if(!Platform_HW_Cursor_Active())
        {
            SDL_ShowCursor(SDL_ENABLE);
        }
        pfl_video_input_grab(false);
    }
}

/* PLAN-Fullscreen.md phases 2 and 6: the window states -- windowed and fullscreen -- and the switch between them.
   Fullscreen is a borderless window covering the whole display the window is on (SDL's desktop fullscreen): no display-mode or
   refresh-rate change. ReMoM has no exclusive fullscreen. Fullscreen captures the mouse; the window releases it. Leaving fullscreen,
   SDL puts the window back where it was and the size it was. The SIZE_CHANGED event that follows redraws; the viewport is refreshed
   here too so the mouse confinement is right immediately. */
static int sdl2_window_state = VIEWPORT_WINDOW_STATE_WINDOWED;

void sdl2_Set_Window_State(int window_state)
{
    int from = sdl2_window_state;

    window_state = Viewport_Window_State_Normalize(window_state);
    if((sdl2_window == NULL) || (window_state == from))
    {
        return;
    }

    if(window_state == VIEWPORT_WINDOW_STATE_FULLSCREEN)
    {
        if(SDL_SetWindowFullscreen(sdl2_window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
        {
            LOG_ERROR(LOG_CAT_SDL2_PFL, "graphics: entering fullscreen failed: %s", SDL_GetError());
            return;
        }
    }
    else
    {
        if(SDL_SetWindowFullscreen(sdl2_window, 0) != 0)
        {
            LOG_ERROR(LOG_CAT_SDL2_PFL, "graphics: leaving fullscreen failed: %s", SDL_GetError());
            return;
        }
    }

    sdl2_window_state = window_state;
    LOG_INFO(LOG_CAT_SDL2_PFL, "graphics: %s (display %d)", Viewport_Window_State_Name(window_state), SDL_GetWindowDisplayIndex(sdl2_window));

    sdl2_Viewport_Refresh();
    if(window_state == VIEWPORT_WINDOW_STATE_FULLSCREEN)
    {
        pfl_mouse_grab();
    }
    else
    {
        pfl_mouse_ungrab();
    }
    sdl2_Mouse_Confine_Update();
}

/* Alt+Enter: fullscreen, and back to the window. */
void sdl2_Toggle_Fullscreen(void)
{
    if(sdl2_window_state == VIEWPORT_WINDOW_STATE_FULLSCREEN)
    {
        sdl2_Set_Window_State(VIEWPORT_WINDOW_STATE_WINDOWED);
    }
    else
    {
        sdl2_Set_Window_State(VIEWPORT_WINDOW_STATE_FULLSCREEN);
    }
}

int sdl2_Is_Fullscreen(void)
{
    return ((sdl2_window != NULL) && ((SDL_GetWindowFlags(sdl2_window) & SDL_WINDOW_FULLSCREEN) != 0));
}

/* While captured, never let the OS mouse position sit off the game image: warp it to the nearest point on the image. SDL's
   mouse rect (sdl2_Mouse_Confine_Update) is not enough on its own -- with relative mouse mode on, the OS position still ran
   into the black bars (seen on Windows, SDL 2.32), leaving the drawn cursor stuck on the edge until the mouse came back across
   the bar. Called by both mouse polls with the position they just read. */
void sdl2_Mouse_Keep_On_Image(int window_x, int window_y)
{
    int image_x = 0;
    int image_y = 0;
    int image_width = 0;
    int image_height = 0;
    int target_x = window_x;
    int target_y = window_y;

    if((pfl_mouse_grabbed == false) || (sdl2_window == NULL))
    {
        return;
    }
    Viewport_Image_Rect_In_Window(&sdl2_viewport, &image_x, &image_y, &image_width, &image_height);
    SETRANGE(target_x, image_x, image_x + image_width - 1);
    SETRANGE(target_y, image_y, image_y + image_height - 1);
    if((target_x != window_x) || (target_y != window_y))
    {
        SDL_WarpMouseInWindow(sdl2_window, target_x, target_y);
    }
}

// not a MoO1/MoM function
static void pfl_mouse_toggle_grab(void)
{
    if(pfl_mouse_grabbed)
    {
        pfl_mouse_ungrab();
    }
    else
    {
        pfl_mouse_grab();
    }
}

static void pfl_mouse_set_limits(int w, int h)
{
    pfl_mouse_game_w = w;
    pfl_mouse_game_h = h;
}

static void pfl_mouse_set_scale(int w, int h)
{
    int v;
    v = w / pfl_mouse_game_w;
    SETMAX(v, 1);
    v *= 100;
    pfl_mouse_sx = v;
    v = h / pfl_mouse_game_h;
    SETMAX(v, 1);
    v *= 100;
    pfl_mouse_sy = v;
}

static void pfl_mouse_move(int dx, int dy)
{
    int x, y;
    {
        pfl_mouse_dx_acc += dx * pfl_option_mouse_speed;
        dx = pfl_mouse_dx_acc / pfl_mouse_sx;
        pfl_mouse_dx_acc = pfl_mouse_dx_acc % pfl_mouse_sx;
    }
    {
        pfl_mouse_dy_acc += dy * pfl_option_mouse_speed;
        dy = pfl_mouse_dy_acc / pfl_mouse_sy;
        pfl_mouse_dy_acc = pfl_mouse_dy_acc % pfl_mouse_sy;
    }
    if((dx == 0) && (dy == 0))
    {
        return;
    }
    x = pointer_x + dx;
    SETRANGE(x, 0, pfl_mouse_game_w - 1);
    y = pointer_y + dy;
    SETRANGE(y, 0, pfl_mouse_game_h - 1);
    pfl_mouse_set_xy_from_hw(x, y);
}

/* CLAUDE: replaced hardcoded /2 with dynamic scale from window/screen dimensions */
static void Platform_Update_Mouse_Position(int l_mx, int l_my)
{
    /* OG: int screen_scale = 2; */
    int16_t game_x = 0;
    int16_t game_y = 0;
    int gx = 0;
    int gy = 0;

    /* Through the viewport (letterbox, high-DPI). Off the image -- a black bar -- the pointer stops on the nearest edge. */
    Platform_Window_To_Game(l_mx, l_my, &game_x, &game_y);
    sdl2_Mouse_Keep_On_Image(l_mx, l_my);
    gx = game_x;
    gy = game_y;
    pointer_x = gx;
    pointer_y = gy;
    if(Input_Metrics_Active()) { Input_Metrics_Note_Poll(gx, gy); }  /* CLAUDE: metrics poll */
    // ITRY  platform_mouse_button_status = buttons;
    if(mouse_interrupt_active == ST_FALSE)
    {
        mouse_interrupt_active = ST_TRUE;
        // Check_Mouse_Buffer(gx, gy, buttons);
        if(mouse_enabled == ST_TRUE)
        {
            mouse_enabled = ST_FALSE;
            if(current_mouse_list_count >= 2)
            {
                Check_Mouse_Shape(gx, gy);
            }
            Restore_Mouse_On_Page();                     // mouse_background_buffer           ->  video_page_buffer[draw_page_num]
            Save_Mouse_On_Page(gx, gy);                  // video_page_buffer[draw_page_num]  ->  mouse_background_buffer
            Draw_Mouse_On_Page(gx, gy);                  // mouse_palette                     ->  video_page_buffer[draw_page_num]
            mouse_enabled = ST_TRUE;
        }
        mouse_interrupt_active = ST_FALSE;
    }
    /* CLAUDE: HW-cursor prototype -- reflect any shape change from Check_Mouse_Shape above into the OS
       cursor.  Self-gates (no-op when inactive).  The software Draw above is suppressed in Mouse.c. */
    Platform_HW_Cursor_Refresh();
}

void Platform_Window_Event(SDL_WindowEvent *sdl2_window_event)
{

    switch(sdl2_window_event->event)
    {

        case SDL_WINDOWEVENT_RESIZED:
        {
            // hw_video_resize(0, 0);
        } break;

        /* New window size, or moved to a display with a different scale: re-fit the image and redraw the bars. */
        case SDL_WINDOWEVENT_SIZE_CHANGED:
        case SDL_WINDOWEVENT_DISPLAY_CHANGED:
        {
            Platform_Video_Update();
        } break;

        case SDL_WINDOWEVENT_EXPOSED:
        {
            Platform_Video_Update();
        } break;

        case SDL_WINDOWEVENT_FOCUS_GAINED:
        {
            Platform_Mouse_Input_Enable();
            /* Back from Alt+Tab: fullscreen recaptures the mouse straight away (windowed waits for a click, as before). */
            if(sdl2_Is_Fullscreen() != 0)
            {
                pfl_mouse_grab();
                sdl2_Mouse_Confine_Update();
            }
        } break;

        case SDL_WINDOWEVENT_FOCUS_LOST:
        {
            Platform_Mouse_Input_Disable();
        } break;

        default:
        {
        } break;
    }

}

/* CLAUDE: debug — frame timing and event/call counters */
static uint64_t dbg_frame_start_ticks = 0;
static uint32_t dbg_handler_calls = 0;
static uint32_t dbg_events_key = 0;
static uint32_t dbg_events_mousedown = 0;
static uint32_t dbg_events_mouseup = 0;
static uint32_t dbg_events_mousemotion = 0;
static uint32_t dbg_events_window = 0;
static uint32_t dbg_events_other = 0;
static uint32_t dbg_mouse_updates = 0;
static uint32_t dbg_frame_number = 0;
static uint64_t dbg_prev_handler_ticks = 0;
static uint64_t dbg_max_handler_delta = 0;
static uint64_t dbg_total_handler_delta = 0;

void DBG_Frame_Reset(void)
{
    uint64_t now = (uint64_t)SDL_GetTicks();
    uint64_t frametime = now - dbg_frame_start_ticks;
    if(dbg_frame_start_ticks != 0 && (dbg_frame_number % 60) == 0)
    {
        uint64_t avg_delta = (dbg_handler_calls > 0) ? (dbg_total_handler_delta / dbg_handler_calls) : 0;
#ifdef STU_DEBUG
        LOG_TRACE(LOG_CAT_PFL, "DBG frame=%u  frametime=%llu ms  handler_calls=%u  max_delta=%llu ms  avg_delta=%llu ms  events: key=%u mdown=%u mup=%u mmove=%u win=%u other=%u  mouse_updates=%u", dbg_frame_number, (unsigned long long)frametime, dbg_handler_calls, (unsigned long long)dbg_max_handler_delta, (unsigned long long)avg_delta, dbg_events_key, dbg_events_mousedown, dbg_events_mouseup, dbg_events_mousemotion, dbg_events_window, dbg_events_other, dbg_mouse_updates);
#endif
    }
    dbg_frame_start_ticks = now;
    dbg_handler_calls = 0;
    dbg_max_handler_delta = 0;
    dbg_total_handler_delta = 0;
    dbg_events_key = 0;
    dbg_events_mousedown = 0;
    dbg_events_mouseup = 0;
    dbg_events_mousemotion = 0;
    dbg_events_window = 0;
    dbg_events_other = 0;
    dbg_mouse_updates = 0;
    dbg_frame_number++;
}
/* CLAUDE: end debug */

/* CLAUDE: Pump events AND refresh cursor.  Any busy-wait that calls this (Release_Time, animation
   loops, etc.) now gets automatic cursor tracking without needing a separate Platform_Maybe_Move_Mouse()
   call.  The cursor refresh polls the OS position and only redraws when moved, so the cost is negligible
   when the mouse is stationary. */
void Platform_Pump_Events(void)
{
    if(Input_Metrics_Active()) { Input_Metrics_Note_Tick((uint64_t)SDL_GetTicks()); }  /* CLAUDE: metrics tick */
    SDL_PumpEvents();
    Platform_Maybe_Move_Mouse();
}

void Platform_Event_Handler(void)
{
    SDL_Event sdl2_event;

    /* CLAUDE */  {
        uint64_t dbg_now = (uint64_t)SDL_GetTicks();
        if(dbg_prev_handler_ticks != 0)
        {
            uint64_t delta = dbg_now - dbg_prev_handler_ticks;
            dbg_total_handler_delta += delta;
            if(delta > dbg_max_handler_delta) { dbg_max_handler_delta = delta; }
        }
        dbg_prev_handler_ticks = dbg_now;
        dbg_handler_calls++;
        if(Input_Metrics_Active()) { Input_Metrics_Note_Tick(dbg_now); }  /* CLAUDE: metrics tick */
#ifdef MOUSE_DEBUG
        MOUSE_LOG("MOUSEt=%llu HANDLER_START ptr=%d,%d\n", (unsigned long long)dbg_now, pointer_x, pointer_y);
#endif
    }

    /* CLAUDE: do NOT reset platform_frame_mouse_buttons here.  Clicks that arrive during Release_Time's Platform_Pump_Events spin-wait are OR'd into this flag between frames.  Resetting at the top would clobber them before Replay_Capture_Frame could read them.  Reset is now done at the END of this function, after capture. */

    /* CLAUDE: Replay — if replaying, inject recorded frame instead of polling OS events. */
    /* CLAUDE: If the user presses a key or clicks the mouse during replay, cancel it. */
    if(Platform_Replay_Active())
    {
        SDL_Event peek_event;
        SDL_PumpEvents();
        while(SDL_PeepEvents(&peek_event, 1, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT) > 0)
        {
            if(peek_event.type == SDL_KEYDOWN || peek_event.type == SDL_MOUSEBUTTONDOWN)
            {
                LOG_INFO(LOG_CAT_SDL2_PFL, "REPLAY: cancelled by user input");
                Platform_Replay_Stop();
                /* Flush the event that cancelled so it doesn't trigger a game action. */
                SDL_PollEvent(&peek_event);
                return;
            }
            else if(peek_event.type == SDL_QUIT)
            {
                /* Let the quit event through normally. */
                Platform_Replay_Stop();
                break;
            }
            else
            {
                /* Discard non-input events (window focus, mouse motion, etc.) during replay. */
                SDL_PollEvent(&peek_event);
            }
        }

        if(Platform_Replay_Active())
        {
            if(!Replay_Inject_Frame())
            {
                /* Replay ended — fall through to live input. */
            }
            else
            {
                return;
            }
        }
    }

    SDL_PumpEvents();

    while(SDL_PollEvent(&sdl2_event))
    {

        switch(sdl2_event.type)
        {

            case SDL_KEYDOWN:
            case SDL_KEYUP:
            case SDL_TEXTINPUT:
            {
                /* CLAUDE */  dbg_events_key++;
                Platform_Keyboard_Event(&sdl2_event);
            } break;

            case SDL_MOUSEBUTTONDOWN:
            {
                /* CLAUDE */  dbg_events_mousedown++;
#ifdef MOUSE_DEBUG
                MOUSE_LOG("MOUSEt=%llu BTN_DOWN btn=%d wx=%d wy=%d\n", (unsigned long long)SDL_GetTicks(), (int)sdl2_event.button.button, (int)sdl2_event.button.x, (int)sdl2_event.button.y);
#endif
                if(sdl2_event.button.state == SDL_PRESSED)
                {
                    if(platform_mouse_input_enabled)
                    {
                        pfl_mouse_grab();
                    }
                }
                // if(SDL_BUTTON(e.button.button) == SDL_BUTTON_LEFT)
                if(sdl2_event.button.button == SDL_BUTTON_LEFT)
                {
                    platform_frame_mouse_buttons |= 1;
                    User_Mouse_Handler(1 /*0b00000001*/, sdl2_event.button.x, sdl2_event.button.y);
                }
                // if(SDL_BUTTON(e.button.button) == SDL_BUTTON_RIGHT)
                if(sdl2_event.button.button == SDL_BUTTON_RIGHT)
                {
                    platform_frame_mouse_buttons |= 2;
                    User_Mouse_Handler(2 /*0b00000010*/, sdl2_event.button.x, sdl2_event.button.y);
                }
                if(sdl2_event.button.button == SDL_BUTTON_MIDDLE)
                {
                    pfl_mouse_ungrab();
                }
            } break;

            case SDL_MOUSEBUTTONUP:
            {
                /* CLAUDE */  dbg_events_mouseup++;
                // platform_mouse_button_status = 0;
                /* NOTE: don't clear platform_frame_mouse_buttons here.
                   It's a per-frame "was clicked" flag, cleared at the
                   start of each Platform_Event_Handler() call.  BUTTON_UP
                   in the same frame as BUTTON_DOWN should still record btn=1. */
            } break;

            case SDL_MOUSEMOTION:
            {
                /* CLAUDE */  dbg_events_mousemotion++;
            } break;

            case SDL_QUIT:
            {
                // hw_audio_shutdown_pre();
                exit(EXIT_SUCCESS);
            } break;

            case SDL_WINDOWEVENT:
            {
                /* CLAUDE */  dbg_events_window++;
                if(sdl2_event.window.windowID == pfl_video_get_window_id())
                {
                    Platform_Window_Event(&sdl2_event.window);
                }
            } break;

            default:
            {
                /* CLAUDE */  dbg_events_other++;
            } break;

        }
    }

    // ¿ ~== MWA WM_MOUSEMOVE ?
    if(platform_mouse_input_enabled)
    {
        int x;
        int y;
        SDL_GetMouseState(&x, &y);
        if((x != 0) || (y != 0))
        {
            // // // // // hw_mouse_move(x, y);
            // // // // pfl_mouse_move(x, y);
            // // // User_Mouse_Handler(0, x, y);
            // // Platform_Update_Mouse_Position(x, y);
            // // User_Mouse_Handler(platform_mouse_button_status, (pointer_x + x), (pointer_y + y));
            // // pfl_mouse_move(x, y);
            // User_Mouse_Handler(platform_mouse_button_status, x, y);
            Platform_Update_Mouse_Position(x, y);
            /* CLAUDE */  dbg_mouse_updates++;
#ifdef MOUSE_DEBUG
            MOUSE_LOG("MOUSEt=%llu POLL wx=%d wy=%d gx=%d gy=%d\n", (unsigned long long)SDL_GetTicks(), x, y, pointer_x, pointer_y);
#endif
        }
    }

    // if(hw_audio_check_process()) {
    //     exit(EXIT_FAILURE);
    // }

    /* CLAUDE */  /* Dasm: SDL_Delay(10) removed; vsync (SDL_RENDERER_PRESENTVSYNC) already provides frame pacing via SDL_RenderPresent(); the delay here stacked on every call to Platform_Event_Handler() (multiple times per frame from Get_Input and Mouse_Button), adding 30-50ms of cumulative lag per frame */

    /* Artificial human player: inject scripted input. */
    if(platform_frame_callback != NULL)
    {
        platform_frame_callback();
    }

    STU_Log_Pump();

    /* CLAUDE: Record — capture input state after processing OS events. */
    if(Platform_Record_Active())
    {
        Replay_Capture_Frame();
    }

    /* CLAUDE: reset per-frame click edge AFTER capture so clicks arriving during the next spin-wait accumulate into the following frame's edge. */
    platform_frame_mouse_buttons = 0;
}
