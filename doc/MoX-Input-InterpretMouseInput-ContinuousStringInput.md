


multiple locations


Mouse_Button() != ST_FALSE
...
Mouse_Buffer() != ST_FALSE

if(field_num != 0)

if(p_fields[field_num].type != ft_ContinuousStringInput)

input_field_active != ST_FALSE



if(field_num == 0) means you clicked where there is no field, so you just need to handle deselecting an active field.



## String Input




```c
                        // HERE:    Nay KD, Yay MD, No Help, No Cancel, Yay Field, field_num !=/== down_mouse_button
                        // Unhandled Conditions:
                        //     field_num == down_mouse_button
                        //     (field_num != down_mouse_button) && (p_fields[field_num].type == ft_Input)
                        //     (field_num != down_mouse_button) && (p_fields[field_num].type == ft_ContinuousStringInput)
                        //     (field_num != down_mouse_button) && (down_mouse_button != ST_UNDEFINED) && ¿ ... ?

                        // Yay/Nay click on an edit field that is the active edit field
                        // NOTE: In MoM, if you click off of an active edit field, it restores the string in the previous edit field and activates the new one
                        //       the activation includes, e.g., for the save game name the arrow/marker icon and the blicking underscore cursor

                        /*
                            BEGIN BLOCK:  Exit, Enter, or Exit & Enter an ~Edit-State
                        */

                        // if((p_fields[field_num].type != ft_ContinuousStringInput) && (input_field_active == ST_FALSE)) { // DO NOTHING }
                        if(
                            (p_fields[field_num].type != ft_ContinuousStringInput)
                            &&
                            (input_field_active != ST_FALSE)
                        )
                        {
                            // `Exit Edit-State`
                            itr_continuous_string = 0;
                            while((continuous_string[itr_continuous_string] != '\0') && (p_fields[active_input_field_number].max_characters > itr_continuous_string))
                            {
                                itr_continuous_string++;
                            }
                            if(continuous_string[(itr_continuous_string - 1)] == '_')
                            {
                                itr_continuous_string--;
                            }
                            continuous_string[itr_continuous_string] = '\0';
                            strcpy((char *)p_fields[active_input_field_number].string, continuous_string);
                            input_field_active = ST_FALSE;
                            active_input_field_number = ST_UNDEFINED;
                        }
                        else if(
                                (p_fields[field_num].type == ft_ContinuousStringInput)
                                &&
                                (input_field_active == ST_FALSE)
                            )
                        {
                            strcpy(continuous_string, p_fields[field_num].string);
                            GUI_EditAnimStage = 0;
                            GUI_EditCursorOn = ST_FALSE;
                            input_field_active = ST_TRUE;
                            active_input_field_number = field_num;
                        }
                        else if(
                                (p_fields[field_num].type == ft_ContinuousStringInput)
                                &&
                                (input_field_active != ST_FALSE)
                                &&
                                (field_num != active_input_field_number)
                            )
                        {
                            // `Exit Edit-State`  ~== `Cancel Edit-State`
                            // `Enter Edit-State`
                            strcpy(continuous_string, p_fields[field_num].string);
                            GUI_EditAnimStage = 0;
                            GUI_EditCursorOn = ST_FALSE;
                            input_field_active = ST_TRUE;
                            active_input_field_number = field_num;
                        }
                        // @@After_MouseLeftButton_ContinuousStringInput
                        /*
                            END BLOCK:  Exit, Enter, or Exit & Enter an ~Edit-State
                        */
                        // @@After_MouseButtonLeft_EditState
```




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
