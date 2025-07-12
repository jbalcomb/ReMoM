
#include "sdl2_PFL.h"

#include "MOX/MOX_BASE.h"
#include "MOX/MOX_DEF.h"
#include "MOX/MOX_KEYS.h"
#include "MOX/MOX_TYPE.h"
#include "MOX/Mouse.h"
#include "MOX/Video.h"

#include "sdl2_MOM.h"
#include "sdl2_SCCC.h"

#include "MOM_PFL.h"

// #include "SDL.h"
#include "C:\devellib\SDL2-2.32.2\include\SDL.h"

#include <stdbool.h>
#include <stdlib.h>
#include <inttypes.h>



// static SDL_Window * sdl2_window;                    // static SDL_Window *screen;
SDL_Window * sdl2_window;
SDL_Renderer * sdl2_renderer;                // static SDL_Renderer *renderer;
SDL_Surface * sdl2_surface_RGB666 = NULL;    // static SDL_Surface *screenbuffer = NULL;
SDL_Surface * sdl2_surface_ARGB8888 = NULL;  // static SDL_Surface *argbbuffer = NULL;
SDL_Texture * sdl2_texture = NULL;           // static SDL_Texture *texture = NULL;
SDL_Texture * sdl2_texture_upscaled = NULL;  // 

int sdl2_window_width  = 640;
int sdl2_window_height = 400;

char sdl2_window_title[] = "(MoM-Rasm) Master of Magic v1.31 - Reassembly";  // static const char *window_title = "";

SDL_Rect sdl2_blit_rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};  // static SDL_Rect blit_rect = { 0, 0, SCREENWIDTH, SCREENHEIGHT};

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


uint64_t Platform_Get_Millies(void)
{
    return SDL_GetTicks64();
}

void Platform_Sleep_Millies(uint64_t millies)
{
    SDL_Delay(millies);
}

void Platform_Get_Mouse_Position_XY(int * mx, int * my)
{
    SDL_GetMouseState(mx, my);
}

// ~== 1oom uidelay |-> ui_cursor_refresh(mx, my);
void Platform_Maybe_Move_Mouse(void)
{
    int mx;
    int my;

    if(Get_Pointer_Image_Number() == crsr_None)
    {
        return;
    }

    // platform mouse movement update timer

    // if(((mx != pointer_x) || (my != pointer_y)) && ((now - mouse_time) > DELAY_MOUSE_UPDATE_LIMIT))
        
    Platform_Get_Mouse_Position_XY(&mx, &my);
    // pointer_x = mx;
    // pointer_y = my;
    Save_Mouse_On_Page((mx / 2), (my / 2));
    Draw_Mouse_On_Page((mx / 2), (my / 2));
    // hw_video_redraw_front();  // doesn't touch any game/MoX/MoO1 code, just updates what is displayed on the platform window
    // Dont?  Platform_Palette_Update();
    Platform_Video_Update();
    Restore_Mouse_On_Page();

//     if(current_mouse_list_count >= 2)
//     {
//         Check_Mouse_Shape((l_mx / 2), (l_my/ 2));
//     }
//     Restore_Mouse_On_Page();                     // mouse_background_buffer           ->  video_page_buffer[draw_page_num]
//     Save_Mouse_On_Page((l_mx / 2), (l_my / 2));  // video_page_buffer[draw_page_num]  ->  mouse_background_buffer
//     Draw_Mouse_On_Page((l_mx / 2), (l_my / 2));  // mouse_palette                     ->  video_page_buffer[draw_page_num]

}




// ./src/hw/sdl/hwsdl_opt.c
int kilgore_option_mouse_speed = 100;

// ./src/hw/sdl/2/hwsdl2_video.c
int kilgore_video_get_window_id(void)
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
void kilgore_mouse_set_xy_from_hw(int mx, int my)
{
    pointer_x = mx;
    pointer_y = my;
}

static int kilgore_mouse_game_w = 320;
static int kilgore_mouse_game_h = 200;

static int kilgore_mouse_dx_acc = 0;  // accelerated mouse x
static int kilgore_mouse_dy_acc = 0;  // accelerated mouse y
static int kilgore_mouse_sx = 1;  // scaled mouse x
static int kilgore_mouse_sy = 1;  // scaled mouse y

/* -------------------------------------------------------------------------- */

bool kilgore_mouse_grabbed = false;

/* -------------------------------------------------------------------------- */

bool kilgore_option_relative_mouse = true;

/* -------------------------------------------------------------------------- */

// hwsdl2_video2.c

// not a MoO1/MoM function
void kilgore_video_input_grab(bool grabbed)
{

    SDL_SetWindowGrab(sdl2_window, (SDL_bool)grabbed);

    if(kilgore_option_relative_mouse)
    {

        SDL_SetRelativeMouseMode((SDL_bool)grabbed);

    }

}

/* -------------------------------------------------------------------------- */

// hwsdl_mouse.c

// not a MoO1/MoM function
void kilgore_mouse_grab(void)
{
    if(!kilgore_mouse_grabbed)
    {
        kilgore_mouse_grabbed = true;
        SDL_ShowCursor(SDL_DISABLE);
        kilgore_video_input_grab(true);
    }
}

// not a MoO1/MoM function
void kilgore_mouse_ungrab(void)
{
    if(kilgore_mouse_grabbed)
    {
        kilgore_mouse_grabbed = false;
        SDL_ShowCursor(SDL_ENABLE);
        kilgore_video_input_grab(false);
    }
}

// not a MoO1/MoM function
void kilgore_mouse_toggle_grab(void)
{
    if(kilgore_mouse_grabbed)
    {
        kilgore_mouse_ungrab();
    }
    else
    {
        kilgore_mouse_grab();
    }
}

void kilgore_mouse_set_limits(int w, int h)
{
    kilgore_mouse_game_w = w;
    kilgore_mouse_game_h = h;
}

void kilgore_mouse_set_scale(int w, int h)
{
    int v;
    v = w / kilgore_mouse_game_w;
    SETMAX(v, 1);
    v *= 100;
    kilgore_mouse_sx = v;
    v = h / kilgore_mouse_game_h;
    SETMAX(v, 1);
    v *= 100;
    kilgore_mouse_sy = v;
}

void kilgore_mouse_move(int dx, int dy)
{
    int x, y;
    {
        kilgore_mouse_dx_acc += dx * kilgore_option_mouse_speed;
        dx = kilgore_mouse_dx_acc / kilgore_mouse_sx;
        kilgore_mouse_dx_acc = kilgore_mouse_dx_acc % kilgore_mouse_sx;
    }
    {
        kilgore_mouse_dy_acc += dy * kilgore_option_mouse_speed;
        dy = kilgore_mouse_dy_acc / kilgore_mouse_sy;
        kilgore_mouse_dy_acc = kilgore_mouse_dy_acc % kilgore_mouse_sy;
    }
    if ((dx == 0) && (dy == 0))
    {
        return;
    }
    x = pointer_x + dx;
    SETRANGE(x, 0, kilgore_mouse_game_w - 1);
    y = pointer_y + dy;
    SETRANGE(y, 0, kilgore_mouse_game_h - 1);
    kilgore_mouse_set_xy_from_hw(x, y);
}

void Platform_Update_Mouse_Position(int l_mx, int l_my)
{
    if(l_mx < SCREEN_XMIN || l_my < SCREEN_YMIN || (l_mx / 2) > SCREEN_XMAX || (l_my / 2) > SCREEN_YMAX)
    {
        return;
    }
    pointer_x = l_mx / 2;
    pointer_y = l_my / 2;
    // ITRY  platform_mouse_button_status = buttons;
    if(mouse_interrupt_active == ST_FALSE)
    {
        mouse_interrupt_active = ST_TRUE;
        // Check_Mouse_Buffer((l_mx / 2), (l_my / 2), buttons);
        if(mouse_enabled == ST_TRUE)
        {
            mouse_enabled = ST_FALSE;
            if(current_mouse_list_count >= 2)
            {
                Check_Mouse_Shape((l_mx / 2), (l_my/ 2));
            }
            Restore_Mouse_On_Page();                     // mouse_background_buffer           ->  video_page_buffer[draw_page_num]
            Save_Mouse_On_Page((l_mx / 2), (l_my / 2));  // video_page_buffer[draw_page_num]  ->  mouse_background_buffer
            Draw_Mouse_On_Page((l_mx / 2), (l_my / 2));  // mouse_palette                     ->  video_page_buffer[draw_page_num]
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

void Platform_Event_Handler(void)
{
    SDL_Event sdl2_event;

    SDL_PumpEvents();

    while(SDL_PollEvent(&sdl2_event))
    {

        switch(sdl2_event.type)
        {

            case SDL_KEYDOWN:
            case SDL_KEYUP:
            case SDL_TEXTINPUT:
            {
                Platform_Keyboard_Event(&sdl2_event);
            } break;

            case SDL_MOUSEBUTTONDOWN:
            {
                if(sdl2_event.button.state == SDL_PRESSED)
                {
                    if(platform_mouse_input_enabled)
                    {
                        kilgore_mouse_grab();
                    }
                }
                // if(SDL_BUTTON(e.button.button) == SDL_BUTTON_LEFT)
                if(sdl2_event.button.button == SDL_BUTTON_LEFT)
                {
                    User_Mouse_Handler(1 /*0b00000001*/, sdl2_event.button.x, sdl2_event.button.y);
                }
                // if(SDL_BUTTON(e.button.button) == SDL_BUTTON_RIGHT)
                if(sdl2_event.button.button == SDL_BUTTON_RIGHT)
                {
                    User_Mouse_Handler(2 /*0b00000010*/, sdl2_event.button.x, sdl2_event.button.y);
                }
                if(sdl2_event.button.button == SDL_BUTTON_MIDDLE)
                {
                    kilgore_mouse_ungrab();
                }
            } break;

            case SDL_MOUSEBUTTONUP:
            {
                // platform_mouse_button_status = 0;
            } break;

            case SDL_QUIT:
            {
                // hw_audio_shutdown_pre();
                exit(EXIT_SUCCESS);
            } break;

            case SDL_WINDOWEVENT:
            {
                if(sdl2_event.window.windowID == kilgore_video_get_window_id())
                {
                    Platform_Window_Event(&sdl2_event.window);
                }
            } break;

            default:
            {

            } break;

        }
    }

    // ¿ ~== MWA WM_MOUSEMOVE ?
    if(platform_mouse_input_enabled)
    {
        int x;
        int y;
        // SDL_GetRelativeMouseState(&x, &y);  // ...x and y are set to the mouse deltas since the last call to SDL_GetRelativeMouseState() or since event initialization
        // SDL_GetMouseState(&x, &y);
        // SDL_GetRelativeMouseState(&x, &y);
        SDL_GetMouseState(&x, &y);
        if((x != 0) || (y != 0))
        {
            // // // // // hw_mouse_move(x, y);
            // // // // kilgore_mouse_move(x, y);
            // // // User_Mouse_Handler(0, x, y);
            // // Platform_Update_Mouse_Position(x, y);
            // // User_Mouse_Handler(platform_mouse_button_status, (pointer_x + x), (pointer_y + y));
            // // kilgore_mouse_move(x, y);
            // User_Mouse_Handler(platform_mouse_button_status, x, y);
            Platform_Update_Mouse_Position(x, y);
        }
    }

    // if (hw_audio_check_process()) {
    //     exit(EXIT_FAILURE);
    // }

    SDL_Delay(10);

}
