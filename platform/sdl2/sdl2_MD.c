
#include "Platform.h"

#include "sdl2_PFL.h"

#include <SDL.h>



int16_t Platform_Get_Mouse_Button_State(void)
{
    Uint32 sdl2_mouse_state = 0;
    int16_t l_mouse_button = 0;

    if(!platform_mouse_input_enabled)
    {
        return 0;
    }

    sdl2_mouse_state = SDL_GetMouseState(NULL, NULL);

    if(sdl2_mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT))
    {
        l_mouse_button = ST_LEFT_BUTTON;
    }
    if(sdl2_mouse_state & SDL_BUTTON(SDL_BUTTON_RIGHT))
    {
        l_mouse_button = ST_RIGHT_BUTTON;
    }

    return l_mouse_button;
}


void Platform_Warp_Mouse(int16_t game_x, int16_t game_y)
{
    float screen_scale = (float)sdl2_window_width / (float)PLATFORM_SCREEN_WIDTH;
    SDL_WarpMouseInWindow(sdl2_window, (int)(game_x * screen_scale), (int)(game_y * screen_scale));
}


int Platform_Get_Window_Width(void)
{
    return sdl2_window_width;
}


/* Mouse_Button(), Mouse_Movement_Handler(), Mouse_Button_Handler(), User_Mouse_Handler(), Set_Mouse_Position() moved to MoX/src/Mouse.c */
