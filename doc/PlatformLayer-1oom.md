

main()  hw/sdl/2/hwsdl2.c
    |-> main_1oom()  main.c
        |-> main_init()  main.c
            |-> hw_init()  hw/sdl/2/hwsdl2.c
                |-> hw_audio_init()  hw/sdl/hwsdl_audio.c
                |-> build_key_xlat()  hw/sdl/2/hwsdl2.c
        |-> main_do()  game/game.c



hw_event_handle()
    |-> kbd_set_pressed()

hw_event_handle()
    |-> hw_mouse_button()
        |-> mouse_set_buttons_from_hw()


ui_starmap_handle_scrollkeys()
    |-> kbd_is_pressed()


Keyboard_Status()

Read_Key()



### platform_mouse_button_status
~== Mouse_Button()  INT 33h, 03h
ReMoM:  WndEvnt()          |-> User_Mouse_Handler()                            platform_mouse_button_status = buttons;
1oom:   hw_event_handle()  |-> User_Mouse_Handler()  |-> Check_Mouse_Buffer()  mouse_buttons = buttons;
OON XREF: User_Mouse_Handler() |-> Check_Mouse_Buffer()
so, ...
    change his code, because it can't affect anything and it is more logical for the code-flow?
    means I can replace his User_Mouse_Handler() with mine?
    Does it matter that Kilgore did not set g_mouse_x,y in User_Mouse_Handler()? It's in the Dasm.
        Where does he set them?
            mouse_set_xy_from_hw()
            ...hw_mouse_move() |-> mouse_set_xy_from_hw()
            ...hw_event_handle() |-> hw_mouse_move()
            OON XREF hw_event_handle() |-> hw_mouse_move() |-> mouse_set_xy_from_hw()

void mouse_set_xy_from_hw(int mx, int my)
{
    g_mouse_x = mx;
    g_mouse_y = my;
}

void Set_Pointer_Position(int mx, int my)
{
    g_mouse_x = mx;
    g_mouse_y = my;
}
MoO2 sets old_mouse_x = mouse_x in Toggle_Pages(), which is used by Save, Restore, Draw_Mouse_To/From_Off_Page()

```c
if (hw_mouse_enabled) {
    int x, y;
    SDL_GetRelativeMouseState(&x, &y);
    if ((x != 0) || (y != 0)) {
        hw_mouse_move(x, y);
    }
}
// ...
    g_mouse_x = mx;
    g_mouse_y = my;
```
~==
    ReMoM
        WndEvnt()
```c
case WM_MOUSEMOVE:
{
    OutputDebugStringA("WM_MOUSEMOVE\n");
    POINT ptMouse;
    GetCursorPos(&ptMouse);
    ScreenToClient(g_Window, &ptMouse);
    // DELETE  Update_Mouse_Position((int16_t)ptMouse.x, (int16_t)ptMouse.y);
    User_Mouse_Handler(0 /*0b00000000*/, (int16_t)ptMouse.x, (int16_t)ptMouse.y);
} break;
// ...
    g_mouse_x = l_mx / 2;
    g_mouse_y = l_my / 2;
```






## Keyboard

void kbd_clear(void)
void kbd_add_keypress(mookey_t key, uint32_t mod, char c)
bool Keyboard_Status(void)
uint32_t Read_Key(void)
void kbd_set_pressed(mookey_t key, uint32_t mod, bool pressed)
bool kbd_is_pressed(mookey_t key, uint32_t modon, uint32_t modoff)

Keyboard_Status() is the same for MoO1 and MoM
Read_Key() is the same for MoO1 and MoM
Read_Key() in MoO2 uses last_key_pressed buffer/queue, with key_read and key_write as the head and tail indices
    key_pressed = (key_read != key_write)

Kilgore's approach is effectively the same as Simtex moved to in MoO2
Kilgore used a "queue", Simtex used a "circular buffer"

WWKD
    `if(Keyboard_Status()) { int key = Read_Key(); }`

Why did he used `uint32_t` for the return value of Read_Key()?
typedef enum {} mookey_t;
...enum is `int` by default...
...he returns `key` which is declared `mookey_t key`...
...`key = (mookey_t)kbd.buf[kbd.tail]`...
...`buf` is defined as `uint32_t buf[KBD_BUFSIZE]`...


// WZD s35p14
int16_t Keyboard_Status(void);
// WZD s35p15
uint8_t Read_Key(void);







## Hack It Up, and Drag It Over!!

...collapse main.c, hwsdl.c, hwsdl2.c
...treat it all as part of sdl2_PFL.C

...start from hw_event_handle()
...not called in the screen-loops?  ...just calls Get_Input()
...not called in Get_Input()?  ...just calls Interpret_Mouse_Input()
...not called in Interpret_Mouse_Input()

./src/boolvec.h
./src/comp.h
./src/hw.h
./src/kdb.h
./src/kbd.c
./src/mouse.h
./src/mouse.c
./src/hw/sdl/hwsdl_audio.c
./src/hw/sdl/hwsdl_audio.h
./src/hw/sdl/hwsdl_mouse.c
./src/hw/sdl/hwsdl_mouse.h
./src/hw/sdl/hwsdl_opt.c
./src/hw/sdl/hwsdl_opt.h
./src/hw/sdl/hwsdl_video.c
./src/hw/sdl/hwsdl_video.h
./src/hw/sdl/hwsdl.c
./src/hw/sdl/2/hwsdl2_audio.c
./src/hw/sdl/2/hwsdl2_mouse.c
./src/hw/sdl/2/hwsdl2_opt.c
./src/hw/sdl/2/hwsdl2_video.c
./src/hw/sdl/2/hwsdl2_video.h
./src/hw/sdl/2/hwsdl2.c

hw_event_handle()
    hw_kbd_check_hotkey()
    mookey_t
    SDLK_TBLI_FROM_SCAN()
    key_xlat_scan()
    key_xlat_key[]
    MOO_KEY_z
    MOO_KEY_SPACE
    hw_textinput_active
    MOO_KEY_LAST
    MOO_KEY_UNKNOWN
    mod_xlat()
    kbd_add_keypress()
    kbd_set_pressed()
    hw_mouse_button()
    // hw_mouse_scroll()
    // hw_audio_shutdown_pre()
    hw_video_get_window_id()
    hw_event_handle_window()
    hw_mouse_enabled
    hw_mouse_move()
    // hw_audio_check_process()


void mouse_set_xy_from_hw(int mx, int my)
~== win_PFL.cpp WndEvnt() case WM_MOUSEMOVE: User_Mouse_Handler(0 /*0b00000000*/, (int16_t)ptMouse.x, (int16_t)ptMouse.y);
win_MD
// WZD s35p11
void User_Mouse_Handler(int16_t buttons, int16_t l_mx, int16_t l_my)
    g_mouse_x = l_mx / 2;
    g_mouse_y = l_my / 2;
MOX/Mouse.C
// WZD dseg:78C2
int16_t g_mouse_x = 158;
// WZD dseg:78C4
int16_t g_mouse_y = 100;
Eh?
    hw_event_handle()
        if (hw_mouse_enabled) {
            int x, y;
            SDL_GetRelativeMouseState(&x, &y);
            if ((x != 0) || (y != 0)) {
                hw_mouse_move(x, y);
            }
        }


mouse_buttons
    he used this to replace Mouse_Button()?
mouse_click_buttons
    set by mouse_set_buttons_from_hw()
¿ win_MD.C  int16_t platform_mouse_button_status ?
in Interpret_Mouse_Input()
    if (mouse_buttons == 0)
    vs.
    if(Mouse_Button() != ST_FALSE)
So, ...
    What is mouse_buttons at that point?
    set in mouse_set_buttons_from_hw(int buttons)
        buttons &= (MOUSE_BUTTON_MASK_LEFT | MOUSE_BUTTON_MASK_RIGHT);
        mouse_buttons = buttons;
        if (buttons) {
            mouse_click_buttons = buttons;
So, ...
    What does mouse_click_buttons do then?
    Is it coupled with ...?
        mouse_click_x = moouse_x;
        mouse_click_y = moouse_y;
        mouse_have_click_hw = true;
        mouse_have_click_sw = true;
int mouse_click_x = 0;  // he used this to reaplce Pointer_X()?
int mouse_click_y = 0;  // he used this to reaplce Pointer_Y()?

mouse_have_click_hw
mouse_have_click_sw
bool mouse_getclear_click_hw(void)
    bool r = mouse_have_click_hw;
    mouse_have_click_hw = false;
    return r;
bool mouse_getclear_click_sw(void)
    bool r = mouse_have_click_sw;
    mouse_have_click_sw = false;
    return r;

used by uiobj_input_wait()
    while (!got_any) {
        if (mouse_buttons || mouse_getclear_click_hw()) {
            got_any = true;
            got_mb = true;
        }
    if (got_mb) {
        while (mouse_buttons) {
            uiobj_finish_callback_delay_stored();

Wait_For_Input()
    while(keyboard_flag == ST_FALSE)
    {
            Mouse_Movement_Handler();
            if((Mouse_Button() != 0) || (Mouse_Buffer() != 0))
            {
                mouse_button_flag = ST_TRUE;
                keyboard_flag = ST_TRUE;
            }
    if(mouse_button_flag != ST_FALSE)
    {
        Mouse_Movement_Handler();
        while(Mouse_Button() != 0)
        {
            Call_Auto_Function();
        }
    }


Can I reaplce his Check_Mouse_Buffer() with mine?
Not quite.
    That's where he sets mouse_buttons, which he uses in place of Mouse_Button()





hwsdlmouse.c
hwsdlmouse.h

hwsdl.c
hwsdl2.c

kbd.c
kbd.h
mouse.c
mouse.h


## kbd

mookey_t
MOO_MOD_SHIFT, MOO_MOD_ALT, MOO_MOD_CTRL, MOO_MOD_META, MOO_MOD_ALL
KBD_GET_KEY(_k_), KBD_GET_CHAR(_k_), KBD_GET_MOD(_k_), KBD_GET_KEYMOD(_k_), KBD_MOD_ONLY_SHIFT(_k_), KBD_MOD_ONLY_CTRL(_k_), KBD_MOD_ONLY_ALT(_k_)
void kbd_clear(void);
void kbd_add_keypress(mookey_t key, uint32_t mod, char c);
bool Keyboard_Status(void);
uint32_t Read_Key(void);
void kbd_set_pressed(mookey_t key, uint32_t mod, bool pressed);
bool kbd_is_pressed(mookey_t key, uint32_t modon, uint32_t modoff);

## mouse

MOUSE_BUTTON_MASK_LEFT, MOUSE_BUTTON_MASK_RIGHT
moouse_x;
moouse_y;
mouse_buttons;
mouse_scroll;
mouse_stored_x;
mouse_stored_y;
mouse_click_x;
mouse_click_y;
mouse_click_buttons;
void mouse_set_xy_from_hw(int mx, int my);
void mouse_set_buttons_from_hw(int buttons);
void mouse_set_scroll_from_hw(int scroll);
void mouse_set_click_xy(int mx, int my);
void Set_Pointer_Position(int mx, int my);
bool mouse_getclear_click_hw(void);
bool mouse_getclear_click_sw(void);
