
#include "MOX/Mouse.H"
#include "MOX/MOX_DEF.H"
#include "MOX/MOX_TYPE.H"

#include "MOM_PFL.H"

#include "sdl2_PFL.H"

// #include "SDL.h"
#include "C:\devellib\SDL2-2.32.2\include\SDL.h"



// WZD s35p05
/*
the current button state is returned as a button bitmask, 
  which can be tested using the SDL_BUTTON(X) macros 
  (where X is generally 1 for the left, 2 for middle, 3 for the right button)
x and y are set to the mouse cursor position relative to the focus window
*/
int16_t Mouse_Button(void)
{
    uint32_t sdl2_mouse_state = 0;
    int16_t l_mouse_button = 0;

    Platform_Event_Handler();

    // return platform_mouse_button_status;

    if(!platform_mouse_input_enabled)
    {
        sdl2_mouse_state = 0;
    }
    else
    {
        sdl2_mouse_state = SDL_GetMouseState(NULL, NULL);
        // sdl2_mouse_state = SDL_GetGlobalMouseState(NULL, NULL);
    }

    // Check if the left button is pressed
    if(sdl2_mouse_state & SDL_BUTTON_LMASK)
    {
        l_mouse_button = ST_LEFT_BUTTON;
    }
    // Check if the right button is pressed
    if(sdl2_mouse_state & SDL_BUTTON_RMASK)
    {
        l_mouse_button = ST_RIGHT_BUTTON;
    }

    return l_mouse_button;
}


// WZD s35p10
void Mouse_Movement_Handler(void)
{
    lock_mouse_button_status_flag = ST_TRUE;
}


// WZD s35p11
void Mouse_Button_Handler(void)
{
    // ITRY  platform_mouse_button_status = 0;
    lock_mouse_button_status_flag = ST_FALSE;
}


// WZD s35p12
void User_Mouse_Handler(int16_t buttons, int16_t l_mx, int16_t l_my)
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
        Check_Mouse_Buffer((l_mx / 2), (l_my / 2), buttons);
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

// WZD s35p21
void Set_Mouse_Position(int16_t mx, int16_t my)
{
    SDL_WarpMouseInWindow(sdl2_window, (mx * 2), (my * 2));
}
