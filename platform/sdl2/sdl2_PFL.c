
#include "sdl2_PFL.h"

#include "../../platform/include/Platform_Replay.h"

#include "sdl2_MOM.h"

#include "Platform.h"

#include <SDL.h>

#include <stdbool.h>
#include <stdio.h>   /* CLAUDE: debug */
#include <stdlib.h>
#include <inttypes.h>



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

char sdl2_window_title[] = "(MoM-Rasm) Master of Magic v1.31 - Reassembly";  // static const char *window_title = "";

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

/* Optional per-frame callback for synthetic input injection (HeMoM). */
static void (*platform_frame_callback)(void) = NULL;

void Platform_Register_Frame_Callback(void (*callback)(void))
{
    platform_frame_callback = callback;
}


int Platform_Get_Scale(void)
{
    return platform_screen_scale;
}

uint64_t Platform_Get_Millies(void)
{
    return (uint64_t)SDL_GetTicks();
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
    int screen_scale;

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
        wx = ix;
        wy = iy;
        screen_scale = sdl2_window_width / PLATFORM_SCREEN_WIDTH;
        if(screen_scale < 1) { screen_scale = 1; }
        gx = wx / screen_scale;
        gy = wy / screen_scale;
        SETRANGE(gx, PLATFORM_SCREEN_XMIN, PLATFORM_SCREEN_XMAX);
        SETRANGE(gy, PLATFORM_SCREEN_YMIN, PLATFORM_SCREEN_YMAX);
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

    if(pfl_option_relative_mouse)
    {

        SDL_SetRelativeMouseMode(grabbed ? SDL_TRUE : SDL_FALSE);

    }

}

/* -------------------------------------------------------------------------- */

// hwsdl_mouse.c

// not a MoO1/MoM function
static void pfl_mouse_grab(void)
{
    if(!pfl_mouse_grabbed)
    {
        pfl_mouse_grabbed = true;
        SDL_ShowCursor(SDL_DISABLE);
        pfl_video_input_grab(true);
    }
}

// not a MoO1/MoM function
static void pfl_mouse_ungrab(void)
{
    if(pfl_mouse_grabbed)
    {
        pfl_mouse_grabbed = false;
        SDL_ShowCursor(SDL_ENABLE);
        pfl_video_input_grab(false);
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
    if ((dx == 0) && (dy == 0))
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
    int screen_scale = sdl2_window_width / PLATFORM_SCREEN_WIDTH;
    int gx = l_mx / screen_scale;
    int gy = l_my / screen_scale;

    if(l_mx < PLATFORM_SCREEN_XMIN || l_my < PLATFORM_SCREEN_YMIN || gx > PLATFORM_SCREEN_XMAX || gy > PLATFORM_SCREEN_YMAX)
    {
        return;
    }
    pointer_x = gx;
    pointer_y = gy;
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
}

void Platform_Window_Event(SDL_WindowEvent *sdl2_window_event)
{

    switch(sdl2_window_event->event)
    {

        case SDL_WINDOWEVENT_RESIZED:
        {
            // hw_video_resize(0, 0);
        } break;

        case SDL_WINDOWEVENT_EXPOSED:
        {
            Platform_Video_Update();
        } break;

        case SDL_WINDOWEVENT_FOCUS_GAINED:
        {
            Platform_Mouse_Input_Enable();
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
        fprintf(stderr, "DBG frame=%u  frametime=%llu ms  handler_calls=%u  max_delta=%llu ms  avg_delta=%llu ms  events: key=%u mdown=%u mup=%u mmove=%u win=%u other=%u  mouse_updates=%u\n", dbg_frame_number, (unsigned long long)frametime, dbg_handler_calls, (unsigned long long)dbg_max_handler_delta, (unsigned long long)avg_delta, dbg_events_key, dbg_events_mousedown, dbg_events_mouseup, dbg_events_mousemotion, dbg_events_window, dbg_events_other, dbg_mouse_updates);
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
#ifdef MOUSE_DEBUG
        MOUSE_LOG("MOUSEt=%llu HANDLER_START ptr=%d,%d\n", (unsigned long long)dbg_now, pointer_x, pointer_y);
#endif
    }

    /* Clear per-frame click flag so Replay_Capture_Frame() only sees
       BUTTON_DOWN events that occurred THIS frame. */
    platform_frame_mouse_buttons = 0;

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
                fprintf(stderr, "REPLAY: cancelled by user input\n");
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

    // if (hw_audio_check_process()) {
    //     exit(EXIT_FAILURE);
    // }

    /* CLAUDE */  /* Dasm: SDL_Delay(10) removed; vsync (SDL_RENDERER_PRESENTVSYNC) already provides frame pacing via SDL_RenderPresent(); the delay here stacked on every call to Platform_Event_Handler() (multiple times per frame from Get_Input and Mouse_Button), adding 30-50ms of cumulative lag per frame */

    /* Synthetic player: inject scripted input. */
    if (platform_frame_callback != NULL)
    {
        platform_frame_callback();
    }

    /* CLAUDE: Record — capture input state after processing OS events. */
    if(Platform_Record_Active())
    {
        Replay_Capture_Frame();
    }

}
