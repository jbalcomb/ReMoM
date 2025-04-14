


20250408:
mouse grab/capture

case SDL_WINDOWEVENT:
    if(sdl2_event.window.windowID == hw_video_get_window_id())
        Platform_Window_Event()
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                Platform_Mouse_Input_Enable();
                    platform_mouse_input_enabled = ST_TRUE;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                Platform_Mouse_Input_Disable();
                    platform_mouse_input_enabled = ST_FALSE;

In Platform_Event_Handler(), ...
    if(platform_mouse_input_enabled)
        SDL_GetRelativeMouseState(&x, &y);
        if((x != 0) || (y != 0))
            User_Mouse_Handler(platform_mouse_button_status, x, y);
¿ just updating the mouse cursor image position ?
¿ done elsewhere, using Pointer_X(), Pointer_Y() ?

...click not on a field...
    does not call Set_Mouse_Position()
    ¿ must be updating pointer_x,y and redrawing the cursor ?

...click on a field...
Push_Field_Down()
    ...
    Set_Mouse_Position()
        SDL_WarpMouseInWindow()



Push_Field_Down()
    if(mouse_installed)

Setup_Input_Box_Popup()
    if(mouse_installed)
        while(Mouse_Button() != 0) { }




20250402:
FTW?!?
I click, it doesn't register; the pointer jumps, to what looks like unscaled x,y, but, then, it doesn't even register again after that...






Mouse Events
Mouse Button Click
Mouse Movement



chocolate-doom, 1oom, and SDLPoP all ignore SDL_MOUSEMOTION events.
1oom even turns thems off - `SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);`


How different is all this when using 'mouse grab'?





## Real-Time Pointer Cursor Image

MS Windows has WM_MOUSEMOVE
        case WM_MOUSEMOVE:
        {
            POINT ptMouse;
            GetCursorPos(&ptMouse);
            ScreenToClient(g_Window, &ptMouse);
            User_Mouse_Handler(0 /*0b00000000*/, (int16_t)ptMouse.x, (int16_t)ptMouse.y);
        } break;







## Mouse Grab
var on/off if platform mouse is *in* game window
no equivalent for IBM-PC/MS-DOS

...vs. capture
1oom does capture on keyboard F-10 or mouse middle button



void User_Mouse_Handler(int16_t buttons, int16_t l_mx, int16_t l_my)
    pointer_x = l_mx / 2;
    pointer_y = l_my / 2;
    platform_mouse_button_status = buttons;  // 1oom does this in Check_Mouse_Buffer()

Check_Mouse_Buffer()
    buttons &= (MOUSE_BUTTON_MASK_LEFT | MOUSE_BUTTON_MASK_RIGHT);
    mouse_buttons = buttons;

1oom
    void User_Mouse_Handler(int i, int pressed)
        b = platform_mouse_button_status
        b = AND/OR left up/down right up/down
        Check_Mouse_Buffer(b)
        void Check_Mouse_Buffer(int buttons)
            buttons &= (MOUSE_BUTTON_MASK_LEFT | MOUSE_BUTTON_MASK_RIGHT);
            platform_mouse_button_status = buttons;






https://wiki.libsdl.org/SDL2/SDL_GetMouseState
https://wiki.libsdl.org/SDL2/SDL_GetRelativeMouseState
