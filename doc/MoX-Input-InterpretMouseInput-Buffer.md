


20250403:
Mouse_Button_Handler() && User_Mouse_Handler()
sets g_mouse_x, g_mouse_y
Check_Mouse_Buffer()
    mouse_buffer_button = buttons
    mouse_buffer_flag = ST_TRUE
    mouse_buffer_flag2 = ST_TRUE
    mouse_buffer_x = mx
    mouse_buffer_y = my
...
Interpret_Mouse_Input()
    ...
    ...
    Mouse_Buffer() ... returns and clears mouse_buffer_flag



IDA light purple (color #22)

Interpret_Mouse_Input()

ProgramPath:
    Keyboard_Status() == 0
@@DoTheMouse:
    Mouse_Movement_Handler()
    Mouse_Button()
or      ax, ax
jz      short @@Nay_KD_Nay_MD
No current/new mouse button, check previous/old...




Nay KD
Nay MD

@@Nay_KD_Nay_MD:
Mouse_Buffer() != 0


mouse_button == ST_RIGHT_BUTTON
    help_list_active != ST_FALSE
    Check_Help_List__STUB() == 0
        Mouse_Buffer()
        Mouse_Buffer2()
        return 0;
    mouse_cancel_disabled == ST_FALSE
        


