


## Keyboard Hot-Key
    Return_Type_Z01  (ft_Scroll)
    Return_Type_Z10  (ft_LockedButton)
    Return_Type_6    (ft_ClickLink)
    Return_Type_3    (~normal)


## ENTER
character == ST_KEY_ENTER
    input_field_active
        ~ ((character == ST_KEY_ENTER) && (input_field_active != ST_FALSE))
    OR
    Scan_Field()
        ~ ((character == ST_KEY_ENTER) && (input_field_active == ST_FALSE) && (alt_field_num = Scan_Field() != 0))

    OR
    nothing
        ~ ((character == ST_KEY_ENTER) && (input_field_active == ST_FALSE) && (alt_field_num = Scan_Field() == 0))
        return 0;
        ¿ same as 'no input' Nay KD,MD,MB ?


@@KD_ENTER_NayTextInput_JG
IDGI
just checked alt_field_num != 0
now checks alt_field_num > 0
<= looks to just be a fall-through, no else {}
then, type == ft_ContinuousStringInput
and, input_field_active != ST_FALSE
the else branch for type != ft_ContinuousStringInput looks bogus - unreachable code
    can't tell if compiler just got confused

Presumably, the origin of the effort here was activating fields using the keyboard keys - arrows then enter.
Then, they added the text input.
So, now we need to know if enter is exiting, entering, or switching between text input fields.
But, we already handled ENTER and input_field_active - save input string and return active_input_field_number.
...which also means there is no switching from one to another.
So, we are just handling entering a new one.
Maybe, it was coding up with the option to switch and then the precluding code was added?





## ENTER as *click*
scanned_field
    by way of arrow keys or mouse pointer
    e.g.,
        'Advisors Popup'
            arrow down to 'Mirro', hit ENTER, 'Mirror Screen' happens




Keyboard_Status() != ST_FALSE

character = Interpret_Keyboard_Input(&field_num)

if(character != 0)

if(character == KP_F11)         ... Exit_With_Size()
if(character == KP_F12)         ... Screen_Flic_Capture()
MoO1: if(character == KP_F1)    ... help  (not in MoM or MoO2)
if(character == KP_ESC)         ... return -1
if(character == KP_BACKTICK)    ... field_box_mode
if(character == KP_TILDE)       ... help_box_mode

if(p_fields[field_num].type == ft_MultiHotKey) { Mouse_Button_Handler(); return field_num; }

if((field_num != 0) && (p_fields[field_num].hotkey == character)) ...
    (field_num == 0 || p_fields[field_num].hotkey != character)
    OR
    !(field_num != 0 && p_fields[field_num].hotkey == character)
Where do they come back together?
Seems they wouldn't/shouldn't?
if the character matches a hotkey it is consumed
otherwise, it's a hard-coded input control or it's meant for a text input?
Maybe just feels off due to the special handling for the scroll field?
    but that's just because a scroll field hotkey is only used to *activate*/*select* the field?

¿ dark green #35  Keyboard Hot-Keys ?


inside 'Keyboard Hot-Key'
    only justification for checking field_num == 0 would be if the previous check was the alt combo that doesn't actually guarantee field_num != 0
    ¿ then ?
    probably just if(field_num != 0)
    but, is the *else* branch just a fall-through?

@@QuickCall_ClearDown_Return_FieldNum:
    Quick_Call_Auto_Function();
    down_mouse_button = ST_UNDEFINED;
    jmp     @@Return_FieldNum
        return field_num;




### ESC && mouse_cancel_disabled

MoO1:
    if(character == KP_ESCAPE) { return_value = -1; }
MoM:
    if(character == KP_ESCAPE && mouse_cancel_disabled == ST_FALSE) { return_value = -1; }
MoO2:
    if(character == KP_ESCAPE && mouse_cancel_disabled == ST_FALSE) { return_value = -1; clear_multi_hotkey_fields = ST_TRUE; }


## mouse_cancel_disabled

XREF:
    Interpret_Mouse_Input+C3        cmp     [mouse_cancel_disabled], e_ST_FALSE
    Interpret_Mouse_Input:loc_26DD1 cmp     [mouse_cancel_disabled], e_ST_FALSE
    Interpret_Mouse_Input+A5D       cmp     [mouse_cancel_disabled], e_ST_FALSE
    Interpret_Mouse_Input:loc_27192 cmp     [mouse_cancel_disabled], e_ST_FALSE
    Interpret_Mouse_Input+E9A       cmp     [mouse_cancel_disabled], e_ST_FALSE
    Enable_Cancel+3                 mov     [mouse_cancel_disabled], e_ST_TRUE 
    UU_Disable_Cancel+3             mov     [mouse_cancel_disabled], e_ST_FALSE
    Init_Mouse_Keyboard+BD          mov     [mouse_cancel_disabled], e_ST_FALSE

Enable_Cancel() is called in main(), right after Release_Version()
