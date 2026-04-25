


Replay_Capture_Frame()


## Visibility
    Win_Window_Proc()
    ...incoming WM event...
    ...buffer event - Platform_Keyboard_Buffer_Add_Key_Press()
        key_pressed = ST_TRUE;
        platform_keyboard_buffer.packed_key[platform_keyboard_buffer.key_write] = packed_key;
        platform_keyboard_buffer.key_write = ((platform_keyboard_buffer.key_write + 1) % PLATFORM_KEYBOARD_BUFFER_LENGTH);

    Read_Key()
        if(Keyboard_Status())
            packed_key_value = platform_keyboard_buffer.packed_key[platform_keyboard_buffer.key_read];
            platform_keyboard_buffer.key_read = ((platform_keyboard_buffer.key_read + 1) % PLATFORM_KEYBOARD_BUFFER_LENGTH);
            packed_key_code = KBD_GET_KEY(packed_key_value);
            packed_character = KBD_GET_CHAR(packed_key_value);
            ...key_num
            return key_num

## Struggle

¿ instead of calling Get_Input(), some function manually handle key presses ?


Change_Home_City_Name_Popup()
    |-> Setup_Input_Box_Popup()
        |-> Read_Key()

...
    |-> Input_Box_Popup
        |-> Read_Key()

Fix options, in order of invasiveness:
Call Platform_Event_Handler() at the top of each outer loop iteration in Setup_Input_Box_Popup (and any sibling popup loops — Setup_Editor_Field at line 1854/1901/1927 has the same pattern). This turns each popup tick into a proper frame with capture. Cleanest fit — it's what Get_Input() already does.
My recommendation: Option 1. It fits the existing model — popup loops become "mini-frames" like any other Get_Input-style loop, and keys/cursor get recorded at natural boundaries. Want me to do it for Setup_Input_Box_Popup and the two Setup_Editor_Field loops?

All three share the same defect. The minimal fix in every case: call Platform_Event_Handler() immediately before each Read_Key() in those loops — same as what Get_Input() does at its entry. That ensures Replay_Capture_Frame() peeks the buffer before Read_Key() drains it.
Want me to apply that targeted fix (just before each Read_Key() in those three functions), or would you rather put the Platform_Event_Handler() at the top of each outer-loop iteration so mouse movement/clicks inside these loops also get recorded as discrete frames?


...added "/* HACK */  Platform_Event_Handler();  /* TODO  consolidate 'record key press' effort */"
...at every call-site for Read_Key()
...except non-usages / discards / buffering clearings in Read_Character() and Reset_Wait_For_Input()



Get_Input()
    |-> Platform_Event_Handler()
Mouse_Button()
    |-> Platform_Event_Handler()
Release_Time_Seconds()
    |-> Platform_Event_Handler()

Platform_Event_Handler()
    |-> Win_Pump_Messages()
    |-> platform_frame_callback()
    |-> Replay_Capture_Frame()

Get_Input()
    |-> Platform_Event_Handler()
        |-> Win_Pump_Messages()
            |-> Win_Window_Proc()
        |-> platform_frame_callback()
        |-> Replay_Capture_Frame()
            fprintf(replay_file, ...
    |-> Interpret_Mouse_Input()
        |-> Interpret_Keyboard_Input()
            |-> Read_Key()
                platform_keyboard_buffer.key_read = ((platform_keyboard_buffer.key_read + 1) % PLATFORM_KEYBOARD_BUFFER_LENGTH);

Replay_Capture_Frame()
    key0 = (key_count > 0) ? Platform_Keyboard_Buffer_Peek_Latest() : 0;
    |-> Platform_Keyboard_Buffer_Peek_Latest()
        return platform_keyboard_buffer.packed_key[(platform_keyboard_buffer.key_write - 1 + PLATFORM_KEYBOARD_BUFFER_LENGTH) % PLATFORM_KEYBOARD_BUFFER_LENGTH];





What is the purpose of Platform_Event_Handler?

Platform_Event_Handler() is the once-per-frame input tick — the canonical point where the platform refreshes all input state before the game reads it. Every backend implements the same contract:
platform/win32/win_PFL.cpp:223
void Platform_Event_Handler(void)
{
    platform_frame_mouse_buttons = 0;   // reset per-frame edge flags
    Win_Pump_Messages();                 // drain OS events -> WM_MOUSEMOVE/WM_LBUTTON etc.
                                         //   which call User_Mouse_Handler(),
                                         //   Platform_Keyboard_Buffer_Add_Key_Press(), ...
    if (platform_frame_callback) platform_frame_callback();  // HeMoM synthetic input
    if (Platform_Record_Active()) Replay_Capture_Frame();    // .RMR recorder
}

Called from three places in MoX:
MoX/src/Input.c:3000 — top of Get_Input(), so every input poll is preceded by a fresh event drain
MoX/src/Mouse.c:234 — inside mouse-wait helper
MoX/src/Timer.c:123, :135 — inside Release_Time()'s wait loop, so timer-bounded waits stay responsive

What it does for callers:

Refreshes pointer_x/pointer_y (via User_Mouse_Handler dispatched from pumped events)
Fills the keyboard buffer
Sets platform_frame_mouse_buttons edge-flags for this frame
Feeds the HeMoM synthetic-input driver one frame of scripted input
Snapshots a frame into the replay recorder (for --record)

vs. Platform_Pump_Events() (at win_PFL.cpp:246): that's the lighter version used by the inner spin-wait of Release_Time(n) — it pumps messages and refreshes the cursor but does not reset per-frame button flags, invoke the synthetic callback, or capture replay frames. Those belong to real frame boundaries only.

So think of it as the game's "SDL_main-style frame tick" — synthetic input, recording, and per-frame input state all hinge on it firing exactly once per logical frame.
