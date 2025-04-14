



## Keyboard Buffer

¿ ~== IBM-PC/MS-DOS ?
    16 2-byte values

¿ record/replay ?
    ¿ need frame/tick ?

hw_event_handle()
    SDL_PumpEvents()
    while(SDL_PollEvent(&sdl2_event))
        switch(sdl2_event.type)
            case SDL_KEYDOWN:
                Platform_Keyboard_Event(&sdl2_event)
            case SDL_KEYUP:
                Platform_Keyboard_Event(&sdl2_event)


Platform_Keyboard_Buffer_Add_Key_Press()
...store SCCC into keyboard key press buffer
¿ retrieve in Read_Key() ?






void Platform_Keyboard_Event(SDL_Event * sdl2_event)

...translate SDL2 key codes to MoX 'Scan Code Char Code'

    switch (sdlevent->type)
        case SDL_KEYDOWN:
            event.data1 = TranslateKey(&sdlevent->key.keysym);
            event.data2 = GetLocalizedKey(&sdlevent->key.keysym);
            event.data3 = GetTypedChar(&sdlevent->key.keysym);
            if (event.data1 != 0)
                D_PostEvent(&event);
            break;
        case SDL_KEYUP:
            event.type = ev_keyup;
            event.data1 = TranslateKey(&sdlevent->key.keysym);
            event.data2 = 0;
            event.data3 = 0;
            if (event.data1 != 0)
                D_PostEvent(&event);
            break;
        default:
            break;





chocolate-doom just passes the event in I_GetEvent() to I_HandleKeyboardEvent()

On Windows, you get virtual-key code but have to check for the control, alternate, and/or shift keys.
...modifier keys...


Is my VK_to_SCCS like Kilgore's key_xlat_key?
Nope.
    /*  46 0x2E */  SCCC_DELETE,    /* VK_DELETE */
    MOO_KEY_PERIOD      = 46,


SDL2
...SDL_KeyboardEvent and SDL_TextInputEvent...


FUUU!!
sometimes a SDL_Keycode is a SDL_Scancode


https://wiki.libsdl.org/SDL2/SDL_Keycode


https://github.com/libsdl-org/SDL/blob/SDL2/include/SDL_keycode.h
https://github.com/libsdl-org/SDL/blob/SDL2/include/SDL_scancode.h



1oom


uses key.keysym.sym (key code), tests SDLK_SCANCODE_MASK
    if
        key = key_xlat_scan[SDLK_TBLI_FROM_SCAN(sym)];
    else
        key = key_xlat_key[sym];



static mookey_t key_xlat_key[0x80];
static mookey_t key_xlat_scan[SDL_NUM_SCANCODES];
#define SDLK_TBLI_FROM_SCAN(i) ((i) & (~SDLK_SCANCODE_MASK))
build_key_xlat()
    memset(key_xlat_key, 0, sizeof(key_xlat_key));
    memset(key_xlat_scan, 0, sizeof(key_xlat_scan));
    key_xlat_key[SDLK_BACKSPACE] = MOO_KEY_BACKSPACE;
    ...
    key_xlat_scan[SDLK_TBLI_FROM_SCAN(SDLK_CAPSLOCK)] = MOO_KEY_CAPSLOCK;
    ...
    key_xlat_scan[SDLK_TBLI_FROM_SCAN(SDLK_UNDO)] = MOO_KEY_UNDO;

static inline uint32_t mod_xlat(SDL_Keymod smod)
    uint32_t mod = 0;
    if (smod & KMOD_SHIFT) { mod |= MOO_MOD_SHIFT; }
    if (smod & KMOD_ALT) { mod |= MOO_MOD_ALT; }
    if (smod & KMOD_CTRL) { mod |= MOO_MOD_CTRL; }
    return mod;

static bool hw_textinput_active = false;

...later, elsewhere...
    #define KBD_GET_KEY(_k_)    ((mookey_t)((_k_) & 0xff))
    #define KBD_GET_CHAR(_k_)   ((char)(((_k_) >> 8) & 0xff))
    ...
        keyp = Read_Key();
        key = KBD_GET_KEY(keyp);
        c = KBD_GET_CHAR(keyp);
    ...
    Just getting the two values back out of the OR'd and SHIFT'd combined value?



chocolate-doom

I_GetEvent()
    while (SDL_PollEvent(&sdlevent))
        switch (sdlevent.type)
            case SDL_KEYDOWN:
                if (ToggleFullScreenKeyShortcut(&sdlevent.key.keysym))
                    I_ToggleFullScreen();
                    break;
                // deliberate fall-though
            case SDL_KEYUP:
		I_HandleKeyboardEvent(&sdlevent);
                break;



doomkeys.h
// DOOM keyboard definition.
// This is the stuff configured by Setup.Exe.
// Most key data are simple ascii (uppercased).
#define KEY_RIGHTARROW	0xae
...
#define KEY_BACKSPACE	0x7f
...
#define KEY_ESCAPE	27
#define KEY_ENTER	13
...
#define KEY_HOME        (0x80+0x47)
...
#define KEYP_ENTER      KEY_ENTER

#define SCANCODE_TO_KEYS_ARRAY { }

// Default names for keys, to use in English or as fallback.
#define KEY_NAMES_ARRAY { }

i_input.c
// Translates the SDL key to a value of the type found in doomkeys.h
static int TranslateKey(SDL_Keysym *sym)
    int scancode = sym->scancode;
    switch (scancode)
        case SDL_SCANCODE_LCTRL:
        case SDL_SCANCODE_RCTRL:
            return KEY_RCTRL;
        case SDL_SCANCODE_LSHIFT:
        case SDL_SCANCODE_RSHIFT:
            return KEY_RSHIFT;
        case SDL_SCANCODE_LALT:
            return KEY_LALT;
        case SDL_SCANCODE_RALT:
            return KEY_RALT;
        default:
            if (scancode >= 0 && scancode < arrlen(scancode_translate_table))
                return scancode_translate_table[scancode];
            else
                return 0;

void I_HandleKeyboardEvent(SDL_Event *sdlevent)
