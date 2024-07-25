




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

IDA purple #15
¿ BEGIN BLOCK:  Exit, Enter, or Exit & Enter an ~Edit-State ?

missing something here... truth table... 2^2 states... current vs. previous && is/isn't vs was/wasn't
    Dasm is 'current isn't', so probably code was 'current is', then previous is/isn't
    but, also, is + was and same vs. not same
    inherently exlucded?
        current isn't and previous wasn't

looks like...
if(p_fields[field_num].type != ft_ContinuousStringInput)
    if(input_field_active == ST_FALSE)
        END BLOCK:  Exit, Enter, or Exit & Enter an ~Edit-State
    if(input_field_active != ST_FALSE)
        itr_continuous_string = 0
        while( (continuous_string[itr_continuous_string] != '\0') && (p_fields[active_input_field_number].Param5 < itr_continuous_string) )
            ...assumes input_field_active and continuous_string and active_input_field_number
            ...find the null-terminator...
            if(continuous_string[(itr_continuous_string - 1)] == '_') ... replaced the UI "_" with a null-terminator
            copy the existing string to the *text input field*
            unset input_field_active and active_input_field_number
...
if(p_fields[field_num].type == ft_ContinuousStringInput)
    if(input_field_active != ST_FALSE)
        if(active_input_field_number == field_num)

feels like...
    ~ if previously active field was a *(text) input field* and currently active field is not a *(text) input field*
        then *deactivate* the previously active *(text) input field*




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
