



Interpret_Mouse_Input()
if(Keyboard_Status() == ST_TRUE)
character = Interpret_Keyboard_Input(&field_num);
ST_KEY_F11, ST_KEY_F12, ST_KEY_ESCAPE, '`', '~'
BEGIN:  Keyboard Hot-Key
ST_KEY_ENTER
...ft_ContinuousStringInput...
...
...ft_Scroll...'+','-'
...input_field_active...ST_KEY_BACKSPACE... ¿ ?

¿ arrow key field navigation ?
¿ mouse emulation ?




e.g.,
Magic Screen |-> Alchemy Popup
Armies Screen |-> Items Screen |-> Alchemy Popup

ovr073
Alchemy_Popup()
Alchemy_Popup_Draw()

Alchemy_Popup_Add_Fields()
Add_Scroll_Field() ... hotkey = 'A'

Alchemy_Popup() DOES NOT test input for the scroll field!

press 'A' for the alchemy button
move the pointer to the alchemy button
press 'A' for the alchemy scroll bar
move the pointer to the alchemy scroll bar

        input_field_idx = Get_Input();
        // ...
        // ...
        // ...

        /*
            BEGIN:  Alchemy Button  (Alchemy Popup)
        */
        if(input_field_idx == m_item_screen_alchemy_button_field)
        {
            // TODO  SND_LeftClickSound();
            Alchemy_Popup((_item_window_start_x + 63), (_item_window_start_y + 58));
            // ...
            // ...
            // ...
        }
        /*
            BEGIN:  Alchemy Button  (Alchemy Popup)
        */

So, ...
    'A' matches the field for the alchemy button
    but, before that, something else moves the pointer to the alchemy button field
    Set_Pointer_Position() { mouse_x = x; mouse_y = y; if(mouse_driver_installed) { Set_Mouse_Position(); } }
    ¿ Interpret_Keyboard_Input() ... |-> ?
    ¿ Push_Field_Down() ?
    



inc/dec scroll bar requires `alt_field_num = Scan_Field()`
so, the "pointer" must be on the scroll field

¿ click and hold ?
