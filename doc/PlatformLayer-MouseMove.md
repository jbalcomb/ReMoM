


hardware interrupt
OS event



For IBM-PC/MS-DOS, the hardware mouse interrupt handler calls the mouse movement code.

For SDL2, the event handler calls the mouse movement code.

In OG-MoM, as coded, the mouse movement handler has to be in the screen-loop.

```c
while(!leave_screen_flag)
{
    Mark_Time();
    input_field_idxGet_Input();
    // ...
    // ...
    // ...
    Release_Time(2);
}
```

e.g., ...
Fade_In() / Fade_Out()
    Mark_Time()
    Set_Palette_Changes()
    Cycle_Palette()
    Release_Time(2)
...Cycle_Palette() should call whatever platform function updates what gets displayed in the window
...Release_Time() should make it sit on that until what is the end of a "frame"

Where to handle mouse movement?

SDL_RenderPresent()


How to handle mouse movement?
    ¿ ***NOT*** SDL_MOUSEMOTION ?
    SDL_GetMouseState(&x, &y);
    SDL_GetRelativeMouseState(&x, &y);
    if(platform_mouse_input_enabled)
...
Platform_Update_Mouse_Position(x, y);
...



while( quit == false )
    {
        //Start the frame timer
        fps.start();
        // ...
        // ...
        // ...
        //Cap the frame rate
        if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
        {
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
        }
    }



if (event.type == SDL_MOUSEMOTION)

