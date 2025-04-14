




Interpret_Mouse_Input()
    character = 0;
    field_num = 0;
    down_mouse_button = ST_UNDEFINED;
    MD_ButtonStatus = 0;
    mouse_field = 0;
    mx = Pointer_X();
    my = Pointer_Y();
    pointer_offset = Get_Pointer_Offset();

    if(Keyboard_Status() == ST_TRUE)
    {
        // ...
    }
    else

```c

Mouse_Movement_Handler();
if(Mouse_Button() != ST_FALSE)
{
    mouse_button = Mouse_Button();
    if(mouse_button != ST_RIGHTBUTTON)
    {
        while(Mouse_Button != ST_FALSE)
        {

        }
    }

}
```


# TARGET:  Mouse Button Left Click
Nay MD, Yay KD
Mouse_Button() || Mouse_Buffer()
¿ mouse_button != ST_RIGHTBUTTON || (() && ())
// IDA:  @@Loop_MouseButton  bright green #11
```c
    mouse_x = Pointer_X();
    mouse_y = Pointer_Y();
    pointer_offset = Get_Pointer_Offset();
    field_num = 0;
    // Unused_Local == ST_UNDEFINED
    character = 0;
    field_num = Scan_Field();
    if(field_num != 0)
    {

    }
    /*
        else ... field_num == 0
        ~ Click Off Field
    */
```

1oom has `if(field_num == 0) { ...; break; } instead of if(field_num != 0) { ... } else { ... }.
Any clues has to which it may have actually been?
Any reason to choose one approach over the other?



if(field_num == 0) means you clicked where there is no field, so you just need to handle deselecting an active field.



...
¿¿¿
if(mouse_auto_exit != ST_FALSE)
{
    break;
}
if(Mouse_Button != ST_FALSE)
{
    Call_Auto_Function();
}
???
