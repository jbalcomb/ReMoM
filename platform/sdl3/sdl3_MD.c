
#include "Platform.h"

#include "sdl3_PFL.h"

#include <SDL3/SDL.h>



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

    return l_mouse_button;
}


void Platform_Warp_Mouse(int16_t game_x, int16_t game_y)
{
    float screen_scale = (float)sdl3_window_width / (float)PLATFORM_SCREEN_WIDTH;
    /* CLAUDE */  Platform_Set_Warp_Guard();  /* prevent Platform_Maybe_Move_Mouse() from reading stale OS position */
    SDL_WarpMouseInWindow(sdl3_window, game_x * screen_scale, game_y * screen_scale);
}


int Platform_Get_Window_Width(void)
{
    return sdl3_window_width;
}


/* Mouse_Button(), Mouse_Movement_Handler(), Mouse_Button_Handler(), User_Mouse_Handler(), Set_Mouse_Position() moved to MoX/src/Mouse.c */
