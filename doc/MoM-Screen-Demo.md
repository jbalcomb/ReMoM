// current_screen = scr_Advisors_Screen;
// current_screen = scr_Cities_Screen;
// current_screen = scr_Magic_Screen;
// current_screen = scr_Armies_Screen;



Screen_Control()
    switch(current_screen)
        case Some_Screen:
            Some_Screen();



Pre-Draw
Post-Draw

Graphics Assets

Input Fields


Clear_Fields();
Some_Screen_Add_Fields();


Deactivate_Auto_Function();
Deactivate_Help_List();
Reset_Window();
Clear_Fields();



```c
void Some_Screen(void)
{
    int16_t leave_screen_flag;

    // ...
    // ...
    // ...

    while(leave_screen_flag == ST_FALSE)
    {

        input_field_idx = Get_Input();

        //

        // if leave_screen_flag == ST_FALSE
        // first_draw-done
        // screen_change

    }

    // ...
    // ...
    // ...

}
```
