#include "sdl2_KD.h"

#include "../../MoX/src/Keyboard.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_KEYS.h"
#include "../../MoX/src/MOX_TYPE.h"

#include "MOM_PFL.h"

#include "sdl2_PFL.h"

#include <SDL3/SDL.h>



char * sdl2_get_key_code_name(int32_t virtual_key_code);
uint32_t mod_xlat(SDL_Keymod smod);
void kbd_set_pressed(int mox_key, uint32_t mox_mod, int pressed);
int kbd_is_pressed(int mox_key, uint32_t modon, uint32_t modoff);



static char sdl2_key_code_name[4096];



/* SDL3: unified scancode-indexed key translation table (replaces key_xlat_key[] + key_xlat_scan[]) */
static int key_xlat[SDL_SCANCODE_COUNT];

static int hw_textinput_active = false;



struct s_KEYBOARD_BUFFER platform_keyboard_buffer;



int16_t Keyboard_Status(void)
{
    if(key_pressed == ST_TRUE)
    {
        return ST_TRUE;
    }
    else
    {
        return ST_FALSE;
    }
}


/*
    1oom/Kilgore
        ...
        keyp = Read_Key();
        key = KBD_GET_KEY(keyp);
        character = KBD_GET_CHAR(keyp);
        ...

*/
uint8_t Read_Key(void)
{
    // uint16_t key_num;
    //
    // if(Keyboard_Status())
    // {
    //     key_num = platform_keyboard_buffer.key_num[platform_keyboard_buffer.key_read];
    //
    //     platform_keyboard_buffer.key_read = ((platform_keyboard_buffer.key_read + 1) % PLATFORM_KEYBOARD_BUFFER_LENGTH);
    //
    //     if(platform_keyboard_buffer.key_read == platform_keyboard_buffer.key_write)
    //     {
    //         key_pressed = ST_FALSE;
    //     }
    //     else
    //     {
    //         key_pressed = ST_TRUE;
    //     }
    // }

    uint32_t kilgore_key_value;
    uint16_t kilgore_key_code;
    char kilgore_character;
    uint16_t key_num;

    if(Keyboard_Status())
    {
        kilgore_key_value = platform_keyboard_buffer.kilgore_key[platform_keyboard_buffer.key_read];

        platform_keyboard_buffer.key_read = ((platform_keyboard_buffer.key_read + 1) % PLATFORM_KEYBOARD_BUFFER_LENGTH);

        if(platform_keyboard_buffer.key_read == platform_keyboard_buffer.key_write)
        {
            key_pressed = ST_FALSE;
        }
        else
        {
            key_pressed = ST_TRUE;
        }
    }

    kilgore_key_code = KBD_GET_KEY(kilgore_key_value);
    kilgore_character = KBD_GET_CHAR(kilgore_key_value);

    if (kilgore_character)
    {
        key_num = kilgore_character;
    }
    else
    {
        key_num = kilgore_key_code;
    }

    /* CLAUDE */  /* DOS Alt+key scancodes are UPPERCASE(character) + 95; match that for multi-hotkey support (e.g., Alt+R,V,L cheat codes) */
    /* CLAUDE */  if ((kilgore_key_value & MOX_MOD_ALT) && kilgore_character >= 'A' && kilgore_character <= 'z')
    /* CLAUDE */  {
    /* CLAUDE */      char alt_char = kilgore_character;
    /* CLAUDE */      if (alt_char >= 'a' && alt_char <= 'z') { alt_char -= 32; }
    /* CLAUDE */      key_num = (uint16_t)alt_char + 95;
    /* CLAUDE */  }

    return (uint8_t)key_num;  // TODO  why/how key_num is diff from return or kilgore or KBD_GET
}



// /*
//
// e.g.,
//     if(sdl2_keysym->sym == SDLK_f && (sdl2_keysym->mod & KMOD_CTRL) != 0)  printf("CTRL-f is pressed!\n");
//
// */
// int Platform_Translate_SDL2_To_SCCC(SDL_Keysym * sdl2_keysym)
// {
//     uint16_t sccc;
//     int32_t virtual_key_code;
//     uint16_t sccc_mod;
//     uint16_t key_modifiers;
//
//     sccc = 0;
//
//     virtual_key_code = sdl2_keysym->sym;
//     key_modifiers = sdl2_keysym->mod;
//
//     sccc_mod = 0;  // no key modifier
//     // KMOD_NONE   = 0x0000,
//     // KMOD_LSHIFT = 0x0001,
//     // KMOD_RSHIFT = 0x0002,
//     // KMOD_LCTRL  = 0x0040,
//     // KMOD_RCTRL  = 0x0080,
//     // KMOD_LALT   = 0x0100,
//     // KMOD_RALT   = 0x0200,
//     // KMOD_LGUI   = 0x0400,
//     // KMOD_RGUI   = 0x0800,
//     // KMOD_NUM    = 0x1000,
//     // KMOD_CAPS   = 0x2000,
//     // KMOD_MODE   = 0x4000,
//     // KMOD_SCROLL = 0x8000,
//     if((key_modifiers = KMOD_LSHIFT) || (key_modifiers = KMOD_RSHIFT)) { sccc_mod = 1; }
//     if((key_modifiers = KMOD_LCTRL ) || (key_modifiers = KMOD_RCTRL )) { sccc_mod = 2; }
//     if((key_modifiers = KMOD_LALT  ) || (key_modifiers = KMOD_RALT  )) { sccc_mod = 3; }
//
//     switch(sccc_mod)
//     {
//         case 0:
//         {
//             sccc = SDLK_To_SCCC[virtual_key_code];
//         } break;
//         case 1:
//         {
//             // SDLK_to_SCCC_SHIFT[virtual_key_code];
//             switch(virtual_key_code)
//             {
//                 case SDLK_a: { sccc = 'A'; break; }
//             }
//         } break;
//         case 2:
//         {
//             // SDLK_to_SCCC_CTRL[virtual_key_code];
//
//         } break;
//         case 3:
//         {
//             for(int itr = 0; itr < 26; itr++)
//             {
//                 if(Alt_Keycodes[itr][0] == virtual_key_code)
//                 {
//                     sccc = Alt_Keycodes[itr][1];
//                 }
//             }
//
//             // SDLK_to_SCCC_ALT[virtual_key_code];
//             switch(virtual_key_code)
//             {
//                 case 'A': { sccc = SCCC_Alt_A; } break;
//                 case 'B': { sccc = SCCC_Alt_B; } break;
//                 case 'C': { sccc = SCCC_Alt_C; } break;
//                 case 'D': { sccc = SCCC_Alt_D; } break;
//                 case 'E': { sccc = SCCC_Alt_E; } break;
//                 case 'F': { sccc = SCCC_Alt_F; } break;
//                 case 'G': { sccc = SCCC_Alt_G; } break;
//                 case 'H': { sccc = SCCC_Alt_H; } break;
//                 case 'I': { sccc = SCCC_Alt_I; } break;
//                 case 'J': { sccc = SCCC_Alt_J; } break;
//                 case 'K': { sccc = SCCC_Alt_K; } break;
//                 case 'L': { sccc = SCCC_Alt_L; } break;
//                 case 'M': { sccc = SCCC_Alt_M; } break;
//                 case 'N': { sccc = SCCC_Alt_N; } break;
//                 case 'O': { sccc = SCCC_Alt_O; } break;
//                 case 'P': { sccc = SCCC_Alt_P; } break;
//                 case 'Q': { sccc = SCCC_Alt_Q; } break;
//                 case 'R': { sccc = SCCC_Alt_R; } break;
//                 case 'S': { sccc = SCCC_Alt_S; } break;
//                 case 'T': { sccc = SCCC_Alt_T; } break;
//                 case 'U': { sccc = SCCC_Alt_U; } break;
//                 case 'V': { sccc = SCCC_Alt_V; } break;
//                 case 'W': { sccc = SCCC_Alt_W; } break;
//                 case 'X': { sccc = SCCC_Alt_X; } break;
//                 case 'Y': { sccc = SCCC_Alt_Y; } break;
//                 case 'Z': { sccc = SCCC_Alt_Z; } break;
//             }
//         } break;
//     }
//
//     return sccc;
// }

// /*
//
//     e.g.
//         translate SCCC_F10 (0x4400) to MOX_KEY_F10 (0x17)
// */
// int Platform_Translate_SCCC_To_MOX_KEY(uint16_t sccc)
// {
//     uint8_t return_key;
//
//     switch(sccc)
//     {
//
//         case SCCC_F10: { return_key = ST_KEY_F10; } break;
//
//         case SCCC_A: { return_key = 'a'; } break;
//         case SCCC_B: { return_key = 'b'; } break;
//         case SCCC_C: { return_key = 'c'; } break;
//         case SCCC_D: { return_key = 'd'; } break;
//         case SCCC_E: { return_key = 'e'; } break;
//         case SCCC_F: { return_key = 'f'; } break;
//         case SCCC_G: { return_key = 'g'; } break;
//         case SCCC_H: { return_key = 'h'; } break;
//         case SCCC_I: { return_key = 'i'; } break;
//         case SCCC_J: { return_key = 'j'; } break;
//         case SCCC_K: { return_key = 'k'; } break;
//         case SCCC_L: { return_key = 'l'; } break;
//         case SCCC_M: { return_key = 'm'; } break;
//         case SCCC_N: { return_key = 'n'; } break;
//         case SCCC_O: { return_key = 'o'; } break;
//         case SCCC_P: { return_key = 'p'; } break;
//         case SCCC_Q: { return_key = 'q'; } break;
//         case SCCC_R: { return_key = 'r'; } break;
//         case SCCC_S: { return_key = 's'; } break;
//         case SCCC_T: { return_key = 't'; } break;
//         case SCCC_U: { return_key = 'u'; } break;
//         case SCCC_V: { return_key = 'v'; } break;
//         case SCCC_W: { return_key = 'w'; } break;
//         case SCCC_X: { return_key = 'x'; } break;
//         case SCCC_Y: { return_key = 'y'; } break;
//         case SCCC_Z: { return_key = 'z'; } break;
//
//         case SCCC_Shift_A: { return_key = 'A'; } break;
//         case SCCC_Shift_B: { return_key = 'B'; } break;
//         case SCCC_Shift_C: { return_key = 'C'; } break;
//         case SCCC_Shift_D: { return_key = 'D'; } break;
//         case SCCC_Shift_E: { return_key = 'E'; } break;
//         case SCCC_Shift_F: { return_key = 'F'; } break;
//         case SCCC_Shift_G: { return_key = 'G'; } break;
//         case SCCC_Shift_H: { return_key = 'H'; } break;
//         case SCCC_Shift_I: { return_key = 'I'; } break;
//         case SCCC_Shift_J: { return_key = 'J'; } break;
//         case SCCC_Shift_K: { return_key = 'K'; } break;
//         case SCCC_Shift_L: { return_key = 'L'; } break;
//         case SCCC_Shift_M: { return_key = 'M'; } break;
//         case SCCC_Shift_N: { return_key = 'N'; } break;
//         case SCCC_Shift_O: { return_key = 'O'; } break;
//         case SCCC_Shift_P: { return_key = 'P'; } break;
//         case SCCC_Shift_Q: { return_key = 'Q'; } break;
//         case SCCC_Shift_R: { return_key = 'R'; } break;
//         case SCCC_Shift_S: { return_key = 'S'; } break;
//         case SCCC_Shift_T: { return_key = 'T'; } break;
//         case SCCC_Shift_U: { return_key = 'U'; } break;
//         case SCCC_Shift_V: { return_key = 'V'; } break;
//         case SCCC_Shift_W: { return_key = 'W'; } break;
//         case SCCC_Shift_X: { return_key = 'X'; } break;
//         case SCCC_Shift_Y: { return_key = 'Y'; } break;
//         case SCCC_Shift_Z: { return_key = 'Z'; } break;
//
//         case SCCC_1: { return_key = '1'; } break;
//         case SCCC_2: { return_key = '2'; } break;
//         case SCCC_3: { return_key = '3'; } break;
//         case SCCC_4: { return_key = '4'; } break;
//         case SCCC_5: { return_key = '5'; } break;
//         case SCCC_6: { return_key = '6'; } break;
//         case SCCC_7: { return_key = '7'; } break;
//         case SCCC_8: { return_key = '8'; } break;
//         case SCCC_9: { return_key = '9'; } break;
//         case SCCC_0: { return_key = '0'; } break;
//
//         case SCCC_MINUS:  { return_key = '-'; } break;
//         case SCCC_EQUALS: { return_key = '='; } break;
//         case SCCC_PLUS:   { return_key = '+'; } break;
//
//         case SCCC_BACKSPACE: { return_key = ST_KEY_BACKSPACE; } break;
//         case SCCC_DELETE:    { return_key = ST_KEY_DELETE;    } break;
//
//         case SCCC_KP_2:     { return_key = ST_KEY_DOWN;       } break;
//         case SCCC_DOWN:     { return_key = ST_KEY_DOWN;       } break;
//         case SCCC_KP_1:     { return_key = ST_KEY_LEFTDOWN;   } break;
//         case SCCC_DOWNLEFT: { return_key = ST_KEY_LEFTDOWN;   } break;
//
//         case SCCC_ENTER:    { return_key = ST_KEY_ENTER;      } break;
//         case SCCC_ESCAPE:   { return_key = ST_KEY_ESCAPE;     } break;
//
//         case SCCC_KP_7:     { return_key = ST_KEY_LEFTUP; } break;
//         case SCCC_UPLEFT:   { return_key = ST_KEY_LEFTUP; } break;
//         // Ins         5200
//         // Keypad 5
//         // Keypad *    372A
//         // Keypad -    4A2D
//         // Keypad +    4E2B
//         // Keypad /    352F
//         case SCCC_KP_4:         { return_key = ST_KEY_LEFT; } break;
//         case SCCC_LEFT:         { return_key = ST_KEY_LEFT; } break;
//         case SCCC_KP_3:         { return_key = ST_KEY_RIGHTDOWN; } break;
//         case SCCC_DOWNRIGHT:    { return_key = ST_KEY_RIGHTDOWN; } break;
//         case SCCC_KP_9:         { return_key = ST_KEY_RIGHTUP; } break;
//         case SCCC_UPRIGHT:      { return_key = ST_KEY_RIGHTUP; } break;
//         // PrtSc
//         case SCCC_KP_6:         { return_key = ST_KEY_RIGHT; } break;
//         case SCCC_RIGHT:        { return_key = ST_KEY_RIGHT; } break;
//         // SpaceBar    3920
//         // Tab         0F09
//         case SCCC_KP_8:         { return_key = ST_KEY_UP; } break;
//         case SCCC_UP:           { return_key = ST_KEY_UP; } break;
//
//         default: { return_key = 0; }
//     }
//
//     return return_key;
// }

uint16_t Platform_Translate_Virtual_Key_Code_To_MOX_Key_Num(int32_t virtual_key_code, uint16_t key_modifiers)
{
    uint16_t key_num;
    uint16_t key_modifier_type;

    key_num = 0;

    key_modifier_type = 0;  // no key modifier
    if((key_modifiers = SDL_KMOD_LSHIFT) | (key_modifiers = SDL_KMOD_RSHIFT)) { key_modifier_type = 1; }
    if((key_modifiers = SDL_KMOD_LCTRL ) | (key_modifiers = SDL_KMOD_RCTRL )) { key_modifier_type = 2; }
    if((key_modifiers = SDL_KMOD_LALT  ) | (key_modifiers = SDL_KMOD_RALT  )) { key_modifier_type = 3; }

    switch(key_modifier_type)
    {
        case 0:  /* NONE */
        {
            key_num = virtual_key_code;
        } break;
        case 1:  /* SHIFT */
        {
            switch(virtual_key_code)
            {
                default:
                {
                    key_num = virtual_key_code;
                } break;
            }
        } break;
        case 2:  /* CTRL */
        {
            switch(virtual_key_code)
            {
                default:
                {
                    key_num = virtual_key_code;
                } break;
            }
        } break;
        case 3:  /* ALT */
        {
            switch(virtual_key_code)
            {
                case SDLK_A: { key_num = ST_KEY_ALT_A; }
                case SDLK_B: { key_num = ST_KEY_ALT_B; }
                case SDLK_C: { key_num = ST_KEY_ALT_C; }
                case SDLK_D: { key_num = ST_KEY_ALT_D; }
                case SDLK_E: { key_num = ST_KEY_ALT_E; }
                case SDLK_F: { key_num = ST_KEY_ALT_F; }
                case SDLK_G: { key_num = ST_KEY_ALT_G; }
                case SDLK_H: { key_num = ST_KEY_ALT_H; }
                case SDLK_I: { key_num = ST_KEY_ALT_I; }
                case SDLK_J: { key_num = ST_KEY_ALT_J; }
                case SDLK_K: { key_num = ST_KEY_ALT_K; }
                case SDLK_L: { key_num = ST_KEY_ALT_L; }
                case SDLK_M: { key_num = ST_KEY_ALT_M; }
                case SDLK_N: { key_num = ST_KEY_ALT_N; }
                case SDLK_O: { key_num = ST_KEY_ALT_O; }
                case SDLK_P: { key_num = ST_KEY_ALT_P; }
                case SDLK_Q: { key_num = ST_KEY_ALT_Q; }
                case SDLK_R: { key_num = ST_KEY_ALT_R; }
                case SDLK_S: { key_num = ST_KEY_ALT_S; }
                case SDLK_T: { key_num = ST_KEY_ALT_T; }
                case SDLK_U: { key_num = ST_KEY_ALT_U; }
                case SDLK_V: { key_num = ST_KEY_ALT_V; }
                case SDLK_W: { key_num = ST_KEY_ALT_W; }
                case SDLK_X: { key_num = ST_KEY_ALT_X; }
                case SDLK_Y: { key_num = ST_KEY_ALT_Y; }
                case SDLK_Z: { key_num = ST_KEY_ALT_Z; }
                default:
                {
                    key_num = virtual_key_code;
                } break;
            }
        } break;
        default:
        {
            key_num = virtual_key_code;
        } break;
    }

    return key_num;
}

static int Platform_Translated_Key(SDL_Scancode sdl3_scancode)
{
    return 0;
}


static int Platform_Localized_Key(SDL_Scancode sdl3_scancode)
{
    return 0;
}


static int Platform_Typed_mox_character(SDL_Scancode sdl3_scancode)
{
    return 0;
}


/*
    Handle/Process Any/All SDL3 Keyboard Events

translate SDL3 scancode to MOX_KEY
SDL3: event.key.key (SDLK_*), event.key.scancode, event.key.mod
All SDLK_* values changed in SDL3 — no longer ASCII-mapped.
We use scancode exclusively for key_xlat lookup.

*/
void Platform_Keyboard_Event(SDL_Event * sdl2_event)
{
    SDL_Scancode sdl3_scancode = 0;
    SDL_Keycode sdl3_key_code = 0;
    SDL_Keymod sdl3_key_modifiers = 0;
    int mox_key = 0;
    uint32_t mox_mod = 0;
    char mox_character = 0;
    const char * sdl3_scancode_name = 0;
    const char * sdl3_key_code_name = 0;

    sdl3_scancode = sdl2_event->key.scancode;
    sdl3_key_code = sdl2_event->key.key;
    sdl3_key_modifiers = sdl2_event->key.mod;

    sdl3_scancode_name = SDL_GetScancodeName(sdl3_scancode);
    sdl3_key_code_name = SDL_GetKeyName(sdl3_key_code);

    switch (sdl2_event->type)
    {
        case SDL_EVENT_KEY_DOWN:
        {
            if((sdl3_key_modifiers & (SDL_KMOD_SHIFT | SDL_KMOD_CTRL | SDL_KMOD_ALT)) && (sdl3_key_code == SDLK_Q))
            {
                SDL_Event sdl2_push_event = { 0 };
                sdl2_push_event.type = SDL_EVENT_QUIT;
                SDL_PushEvent(&sdl2_push_event);
            }

            // 1oom  if(!(Platform_Hotkey(sym, smod, c)))
            {
                /* SDL3: all keys use scancode-based lookup */
                if(sdl3_scancode < SDL_SCANCODE_COUNT)
                {
                    mox_key = key_xlat[sdl3_scancode];
                }

                /* SDL3: SDLK values are NOT ASCII — don't use them as character codes.
                   Set mox_character = 0 here; printable characters come from SDL_EVENT_TEXT_INPUT. */
                mox_character = 0;

                /* For non-printable keys that still need a character code (backspace, enter, escape, tab) */
                switch(sdl3_key_code)
                {
                    case SDLK_BACKSPACE: mox_character = '\b'; break;
                    case SDLK_RETURN:    mox_character = '\r'; break;
                    case SDLK_ESCAPE:    mox_character = 0x1B; break;
                    case SDLK_TAB:       mox_character = '\t'; break;
                    default: break;
                }

                /* ignore printable ASCII range when expecting SDL_EVENT_TEXT_INPUT */
                if(hw_textinput_active && mox_key >= MOX_KEY_SPACE && mox_key <= MOX_KEY_z)
                {
                    mox_key = MOX_KEY_OVERRUN;
                }

                mox_mod = mod_xlat(sdl3_key_modifiers);
                if(mox_key != MOX_KEY_UNKNOWN && mox_key < MOX_KEY_OVERRUN)
                {
                    Platform_Keyboard_Buffer_Add_Key_Press(mox_key, mox_mod, mox_character);
                }
                kbd_set_pressed(mox_key, mox_mod, ST_TRUE);
            }
        } break;
        case SDL_EVENT_KEY_UP:
        {
            sdl3_key_code = sdl2_event->key.key;
            sdl3_scancode = sdl2_event->key.scancode;
            sdl3_key_modifiers = sdl2_event->key.mod;
            if(sdl3_scancode < SDL_SCANCODE_COUNT)
            {
                mox_key = key_xlat[sdl3_scancode];
            }
            kbd_set_pressed(mox_key, mod_xlat(sdl3_key_modifiers), false);
        } break;
        case SDL_EVENT_TEXT_INPUT:
        if(
            (hw_textinput_active)
            &&
            (sdl2_event->text.text[0] != 0)
        )
        {
            char mox_character = sdl2_event->text.text[0];
            SDL_StopTextInput(sdl2_window);
            SDL_StartTextInput(sdl2_window);
            Platform_Keyboard_Buffer_Add_Key_Press(MOX_KEY_UNKNOWN, MOX_MOD_NONE, mox_character);
        }
        break;
        default:
        {
            /* No-Op */
        } break;
    }

}


void Platform_Keyboard_Buffer_Clear(void)
{
    platform_keyboard_buffer.key_write = 0;
    platform_keyboard_buffer.key_read = 0;
}

// void Platform_Keyboard_Buffer_Add_Key_Press(SDL_Keysym * sdl2_keysym)
void Platform_Keyboard_Buffer_Add_Key_Press(int mox_key, uint32_t mox_mod, char mox_character)
{
    uint16_t mox_key_num = 0;
    int32_t virtual_key_code = 0;
    uint16_t key_modifiers = 0;
    uint32_t kilgore_key = 0;

    kilgore_key = ((uint32_t)mox_key) | mox_mod | (((uint32_t)mox_character) << 8);

    if(mox_key == MOX_KEY_OVERRUN)
    {
        return;
    }

    key_pressed = ST_TRUE;

    platform_keyboard_buffer.kilgore_key[platform_keyboard_buffer.key_write] = kilgore_key;

    platform_keyboard_buffer.key_write = ((platform_keyboard_buffer.key_write + 1) % PLATFORM_KEYBOARD_BUFFER_LENGTH);

}

char * sdl2_get_key_code_name(int32_t sdl2_key_code)
{

    memset(sdl2_key_code_name, 0, 4096);

    strcpy(sdl2_key_code_name, SDL_GetKeyName(sdl2_key_code));

    return sdl2_key_code_name;
}


void hw_textinput_start(void)
{
    SDL_StartTextInput(sdl2_window);
    hw_textinput_active = true;
}

void hw_textinput_stop(void)
{
    SDL_StopTextInput(sdl2_window);
    hw_textinput_active = false;
}

/*
    SDL3: single key_xlat[] array indexed by SDL_Scancode.
    Replaces the old key_xlat_key[] (ASCII-indexed) + key_xlat_scan[] (scancode-indexed) split.
    In SDL3 ALL keys use scancodes — SDLK values are no longer usable as array indices.
*/
void build_key_xlat(void)
{
    memset(key_xlat, 0, sizeof(key_xlat));

    /* ASCII-range keys (previously in key_xlat_key[], indexed by SDLK which equaled ASCII) */
    key_xlat[SDL_SCANCODE_BACKSPACE] = MOX_KEY_BACKSPACE;
    key_xlat[SDL_SCANCODE_TAB] = MOX_KEY_TAB;
    key_xlat[SDL_SCANCODE_RETURN] = MOX_KEY_ENTER;
    key_xlat[SDL_SCANCODE_ESCAPE] = MOX_KEY_ESCAPE;
    key_xlat[SDL_SCANCODE_A] = MOX_KEY_a;
    key_xlat[SDL_SCANCODE_B] = MOX_KEY_b;
    key_xlat[SDL_SCANCODE_C] = MOX_KEY_c;
    key_xlat[SDL_SCANCODE_D] = MOX_KEY_d;
    key_xlat[SDL_SCANCODE_E] = MOX_KEY_e;
    key_xlat[SDL_SCANCODE_F] = MOX_KEY_f;
    key_xlat[SDL_SCANCODE_G] = MOX_KEY_g;
    key_xlat[SDL_SCANCODE_H] = MOX_KEY_h;
    key_xlat[SDL_SCANCODE_I] = MOX_KEY_i;
    key_xlat[SDL_SCANCODE_J] = MOX_KEY_j;
    key_xlat[SDL_SCANCODE_K] = MOX_KEY_k;
    key_xlat[SDL_SCANCODE_L] = MOX_KEY_l;
    key_xlat[SDL_SCANCODE_M] = MOX_KEY_m;
    key_xlat[SDL_SCANCODE_N] = MOX_KEY_n;
    key_xlat[SDL_SCANCODE_O] = MOX_KEY_o;
    key_xlat[SDL_SCANCODE_P] = MOX_KEY_p;
    key_xlat[SDL_SCANCODE_Q] = MOX_KEY_q;
    key_xlat[SDL_SCANCODE_R] = MOX_KEY_r;
    key_xlat[SDL_SCANCODE_S] = MOX_KEY_s;
    key_xlat[SDL_SCANCODE_T] = MOX_KEY_t;
    key_xlat[SDL_SCANCODE_U] = MOX_KEY_u;
    key_xlat[SDL_SCANCODE_V] = MOX_KEY_v;
    key_xlat[SDL_SCANCODE_W] = MOX_KEY_w;
    key_xlat[SDL_SCANCODE_X] = MOX_KEY_x;
    key_xlat[SDL_SCANCODE_Y] = MOX_KEY_y;
    key_xlat[SDL_SCANCODE_Z] = MOX_KEY_z;

    /* Function keys (previously in key_xlat_scan[]) */
    key_xlat[SDL_SCANCODE_F1] = MOX_KEY_F1;
    key_xlat[SDL_SCANCODE_F2] = MOX_KEY_F2;
    key_xlat[SDL_SCANCODE_F3] = MOX_KEY_F3;
    key_xlat[SDL_SCANCODE_F4] = MOX_KEY_F4;
    key_xlat[SDL_SCANCODE_F5] = MOX_KEY_F5;
    key_xlat[SDL_SCANCODE_F6] = MOX_KEY_F6;
    key_xlat[SDL_SCANCODE_F7] = MOX_KEY_F7;
    key_xlat[SDL_SCANCODE_F8] = MOX_KEY_F8;
    key_xlat[SDL_SCANCODE_F9] = MOX_KEY_F9;
    key_xlat[SDL_SCANCODE_F10] = MOX_KEY_F10;
    key_xlat[SDL_SCANCODE_F11] = MOX_KEY_F11;
    key_xlat[SDL_SCANCODE_F12] = MOX_KEY_F12;

    /* Navigation keys */
    key_xlat[SDL_SCANCODE_INSERT] = MOX_KEY_INSERT;
    key_xlat[SDL_SCANCODE_HOME] = MOX_KEY_HOME;
    key_xlat[SDL_SCANCODE_DELETE] = MOX_KEY_DELETE;
    key_xlat[SDL_SCANCODE_END] = MOX_KEY_END;
    key_xlat[SDL_SCANCODE_RIGHT] = MOX_KEY_RIGHT;
    key_xlat[SDL_SCANCODE_LEFT] = MOX_KEY_LEFT;
    key_xlat[SDL_SCANCODE_DOWN] = MOX_KEY_DOWN;
    key_xlat[SDL_SCANCODE_UP] = MOX_KEY_UP;

    /* Keypad — directional mapping for MoM */
    key_xlat[SDL_SCANCODE_KP_1] = MOX_KEY_LEFTDOWN;
    key_xlat[SDL_SCANCODE_KP_2] = MOX_KEY_DOWN;
    key_xlat[SDL_SCANCODE_KP_3] = MOX_KEY_RIGHTDOWN;
    key_xlat[SDL_SCANCODE_KP_4] = MOX_KEY_LEFT;
    key_xlat[SDL_SCANCODE_KP_6] = MOX_KEY_RIGHT;
    key_xlat[SDL_SCANCODE_KP_7] = MOX_KEY_LEFTUP;
    key_xlat[SDL_SCANCODE_KP_8] = MOX_KEY_UP;
    key_xlat[SDL_SCANCODE_KP_9] = MOX_KEY_RIGHTUP;
}

uint32_t mod_xlat(SDL_Keymod sdl3_key_modifiers)
{
    uint32_t mox_mod = 0;
    if (sdl3_key_modifiers & SDL_KMOD_SHIFT ) { mox_mod |= MOX_MOD_SHIFT; }
    if (sdl3_key_modifiers & SDL_KMOD_ALT   ) { mox_mod |= MOX_MOD_ALT;   }
    if (sdl3_key_modifiers & SDL_KMOD_CTRL  ) { mox_mod |= MOX_MOD_CTRL;  }
    return mox_mod;
}

void kbd_set_pressed(int mox_key, uint32_t mox_mod, int pressed)
{

    platform_keyboard_buffer.mox_mod = mox_mod;

    if(
        (mox_key != MOX_KEY_UNKNOWN)
        &&
        (mox_key < MOX_KEY_OVERRUN)
    )
    {
        BOOLVEC_SET(platform_keyboard_buffer.pressed, mox_key, pressed);
    }

}

int kbd_is_pressed(int mox_key, uint32_t modon, uint32_t modoff)
{

    return (((platform_keyboard_buffer.mox_mod & modon) == modon) && ((platform_keyboard_buffer.mox_mod & modoff) == 0) && BOOLVEC_IS1(platform_keyboard_buffer.pressed, mox_key));

}
