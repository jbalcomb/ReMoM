#include "sdl2_KD.h"

#include "../../ext/stu_compat.h"

#include "../../MoX/src/Keyboard.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_KEYS.h"
#include "../../MoX/src/MOX_TYPE.h"

#include "MOM_PFL.h"

#include "sdl2_PFL.h"

#include <SDL.h>



char * sdl2_get_key_code_name(int32_t virtual_key_code);
uint32_t mod_xlat(uint16_t smod);
void kbd_set_pressed(int mox_key, uint32_t mox_mod, int pressed);
int kbd_is_pressed(int mox_key, uint32_t modon, uint32_t modoff);



static char sdl2_key_code_name[4096];



/* SDL2: dual key translation tables — ASCII-indexed + scancode-indexed */
static int key_xlat_key[128];  // 0x80
static int key_xlat_scan[SDL_NUM_SCANCODES];  // SDL_NUM_SCANCODES = 512

static int hw_textinput_active = false;

#define SDLK_TABLE_INDEX_FROM_SCANCODE(_sdl2_scancode_) ((_sdl2_scancode_) & (~SDLK_SCANCODE_MASK))



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



/*
    Handle/Process Any/All SDL2 Keyboard Events

translate SDL2 key code to MOX_KEY
SDL2: event.key.keysym.sym (SDLK_*), event.key.keysym.scancode, event.key.keysym.mod
In SDL2, ASCII keys have SDLK values equal to their ASCII code.
Non-ASCII keys (F-keys, arrows, etc.) have the SDLK_SCANCODE_MASK bit set.

*/
void Platform_Keyboard_Event(SDL_Event * sdl2_event)
{
    SDL_Scancode sdl2_scan_code = 0;
    int32_t sdl2_key_code = 0;
    uint16_t sdl2_key_modifiers = 0;
    int mox_key = 0;
    uint32_t mox_mod = 0;
    char mox_character = 0;
    const char * sdl2_scan_code_name = 0;
    const char * sdl2_key_code_name = 0;

    sdl2_scan_code = sdl2_event->key.keysym.scancode;
    sdl2_key_code = sdl2_event->key.keysym.sym;
    sdl2_key_modifiers = sdl2_event->key.keysym.mod;

    sdl2_scan_code_name = SDL_GetScancodeName(sdl2_scan_code);
    sdl2_key_code_name = SDL_GetKeyName(sdl2_key_code);

    switch (sdl2_event->type)
    {
        case SDL_KEYDOWN:
        {
            if((sdl2_key_modifiers & (KMOD_SHIFT | KMOD_CTRL | KMOD_ALT)) && (sdl2_key_code == SDLK_q))
            {
                SDL_Event sdl2_push_event = { 0 };
                sdl2_push_event.type = SDL_QUIT;
                SDL_PushEvent(&sdl2_push_event);
            }

            // 1oom  if(!(Platform_Hotkey(sym, smod, c)))
            {
                if(sdl2_key_code & SDLK_SCANCODE_MASK)
                {
                    mox_key = key_xlat_scan[SDLK_TABLE_INDEX_FROM_SCANCODE(sdl2_key_code)];
                    mox_character = 0;
                }
                else
                {
                    mox_key = key_xlat_key[sdl2_key_code];
                    mox_character = (char)sdl2_key_code; /* SDL2: SDLK values for ASCII keys equal their ASCII code */
                    /* ignore ASCII range when expecting SDL_TEXTINPUT */
                    if((mox_character < MOX_KEY_SPACE) || (mox_character > MOX_KEY_z))
                    {
                        mox_character = MOX_KEY_UNKNOWN;
                    }
                    if((hw_textinput_active) && (mox_key >= MOX_KEY_SPACE) && (mox_key <= MOX_KEY_z))
                    {
                        mox_key = MOX_KEY_OVERRUN;
                    }
                }

                mox_mod = mod_xlat(sdl2_key_modifiers);
                if((mox_key != MOX_KEY_UNKNOWN) && (mox_key < MOX_KEY_OVERRUN))
                {
                    Platform_Keyboard_Buffer_Add_Key_Press(mox_key, mox_mod, mox_character);
                }
                kbd_set_pressed(mox_key, mox_mod, ST_TRUE);
            }
        } break;
        case SDL_KEYUP:
        {
            sdl2_key_code = sdl2_event->key.keysym.sym;
            sdl2_key_modifiers = sdl2_event->key.keysym.mod;
            if(sdl2_key_code & SDLK_SCANCODE_MASK)
            {
                mox_key = key_xlat_scan[SDLK_TABLE_INDEX_FROM_SCANCODE(sdl2_key_code)];
            }
            else
            {
                mox_key = key_xlat_key[sdl2_key_code];
            }
            kbd_set_pressed(mox_key, mod_xlat(sdl2_key_modifiers), false);
        } break;
        case SDL_TEXTINPUT:
        if(
            (hw_textinput_active)
            &&
            (sdl2_event->text.text[0] != 0)
        )
        {
            char mox_character = sdl2_event->text.text[0];
            SDL_StopTextInput();
            SDL_StartTextInput();
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

int Platform_Keyboard_Buffer_Pending_Count(void)
{
    return (platform_keyboard_buffer.key_write - platform_keyboard_buffer.key_read + PLATFORM_KEYBOARD_BUFFER_LENGTH) % PLATFORM_KEYBOARD_BUFFER_LENGTH;
}

uint32_t Platform_Keyboard_Buffer_Peek_Latest(void)
{
    if(platform_keyboard_buffer.key_write == platform_keyboard_buffer.key_read)
    {
        return 0;
    }
    return platform_keyboard_buffer.kilgore_key[(platform_keyboard_buffer.key_write - 1 + PLATFORM_KEYBOARD_BUFFER_LENGTH) % PLATFORM_KEYBOARD_BUFFER_LENGTH];
}

char * sdl2_get_key_code_name(int32_t sdl2_key_code)
{

    memset(sdl2_key_code_name, 0, 4096);

    strcpy(sdl2_key_code_name, SDL_GetKeyName(sdl2_key_code));

    return sdl2_key_code_name;
}


void hw_textinput_start(void)
{
    SDL_StartTextInput();
    hw_textinput_active = true;
}

void hw_textinput_stop(void)
{
    SDL_StopTextInput();
    hw_textinput_active = false;
}

/*
    SDL2: dual key translation tables.
    key_xlat_key[] is indexed by SDLK values for ASCII keys (< 128).
    key_xlat_scan[] is indexed by scancode for non-ASCII keys (F-keys, arrows, etc.)
    which have SDLK_SCANCODE_MASK set.
*/
void build_key_xlat(void)
{
    memset(key_xlat_key, 0, sizeof(key_xlat_key));
    memset(key_xlat_scan, 0, sizeof(key_xlat_scan));

    /* ASCII-range keys (indexed by SDLK which equals ASCII value in SDL2) */
    key_xlat_key[SDLK_BACKSPACE] = MOX_KEY_BACKSPACE;
    key_xlat_key[SDLK_TAB] = MOX_KEY_TAB;
    key_xlat_key[SDLK_RETURN] = MOX_KEY_ENTER;
    key_xlat_key[SDLK_ESCAPE] = MOX_KEY_ESCAPE;
    key_xlat_key[SDLK_a] = MOX_KEY_a;
    key_xlat_key[SDLK_b] = MOX_KEY_b;
    key_xlat_key[SDLK_c] = MOX_KEY_c;
    key_xlat_key[SDLK_d] = MOX_KEY_d;
    key_xlat_key[SDLK_e] = MOX_KEY_e;
    key_xlat_key[SDLK_f] = MOX_KEY_f;
    key_xlat_key[SDLK_g] = MOX_KEY_g;
    key_xlat_key[SDLK_h] = MOX_KEY_h;
    key_xlat_key[SDLK_i] = MOX_KEY_i;
    key_xlat_key[SDLK_j] = MOX_KEY_j;
    key_xlat_key[SDLK_k] = MOX_KEY_k;
    key_xlat_key[SDLK_l] = MOX_KEY_l;
    key_xlat_key[SDLK_m] = MOX_KEY_m;
    key_xlat_key[SDLK_n] = MOX_KEY_n;
    key_xlat_key[SDLK_o] = MOX_KEY_o;
    key_xlat_key[SDLK_p] = MOX_KEY_p;
    key_xlat_key[SDLK_q] = MOX_KEY_q;
    key_xlat_key[SDLK_r] = MOX_KEY_r;
    key_xlat_key[SDLK_s] = MOX_KEY_s;
    key_xlat_key[SDLK_t] = MOX_KEY_t;
    key_xlat_key[SDLK_u] = MOX_KEY_u;
    key_xlat_key[SDLK_v] = MOX_KEY_v;
    key_xlat_key[SDLK_w] = MOX_KEY_w;
    key_xlat_key[SDLK_x] = MOX_KEY_x;
    key_xlat_key[SDLK_y] = MOX_KEY_y;
    key_xlat_key[SDLK_z] = MOX_KEY_z;

    /* Non-ASCII keys (scancode-indexed, SDLK_SCANCODE_MASK stripped via macro) */
    key_xlat_scan[SDLK_TABLE_INDEX_FROM_SCANCODE(SDLK_F1)] = MOX_KEY_F1;
    key_xlat_scan[SDLK_TABLE_INDEX_FROM_SCANCODE(SDLK_F2)] = MOX_KEY_F2;
    key_xlat_scan[SDLK_TABLE_INDEX_FROM_SCANCODE(SDLK_F3)] = MOX_KEY_F3;
    key_xlat_scan[SDLK_TABLE_INDEX_FROM_SCANCODE(SDLK_F4)] = MOX_KEY_F4;
    key_xlat_scan[SDLK_TABLE_INDEX_FROM_SCANCODE(SDLK_F5)] = MOX_KEY_F5;
    key_xlat_scan[SDLK_TABLE_INDEX_FROM_SCANCODE(SDLK_F6)] = MOX_KEY_F6;
    key_xlat_scan[SDLK_TABLE_INDEX_FROM_SCANCODE(SDLK_F7)] = MOX_KEY_F7;
    key_xlat_scan[SDLK_TABLE_INDEX_FROM_SCANCODE(SDLK_F8)] = MOX_KEY_F8;
    key_xlat_scan[SDLK_TABLE_INDEX_FROM_SCANCODE(SDLK_F9)] = MOX_KEY_F9;
    key_xlat_scan[SDLK_TABLE_INDEX_FROM_SCANCODE(SDLK_F10)] = MOX_KEY_F10;
    key_xlat_scan[SDLK_TABLE_INDEX_FROM_SCANCODE(SDLK_F11)] = MOX_KEY_F11;
    key_xlat_scan[SDLK_TABLE_INDEX_FROM_SCANCODE(SDLK_F12)] = MOX_KEY_F12;

    /* Navigation keys */
    key_xlat_scan[SDLK_TABLE_INDEX_FROM_SCANCODE(SDLK_INSERT)] = MOX_KEY_INSERT;
    key_xlat_scan[SDLK_TABLE_INDEX_FROM_SCANCODE(SDLK_HOME)] = MOX_KEY_HOME;
    key_xlat_scan[SDLK_TABLE_INDEX_FROM_SCANCODE(SDLK_DELETE)] = MOX_KEY_DELETE;
    key_xlat_scan[SDLK_TABLE_INDEX_FROM_SCANCODE(SDLK_END)] = MOX_KEY_END;
    key_xlat_scan[SDLK_TABLE_INDEX_FROM_SCANCODE(SDLK_RIGHT)] = MOX_KEY_RIGHT;
    key_xlat_scan[SDLK_TABLE_INDEX_FROM_SCANCODE(SDLK_LEFT)] = MOX_KEY_LEFT;
    key_xlat_scan[SDLK_TABLE_INDEX_FROM_SCANCODE(SDLK_DOWN)] = MOX_KEY_DOWN;
    key_xlat_scan[SDLK_TABLE_INDEX_FROM_SCANCODE(SDLK_UP)] = MOX_KEY_UP;

    /* Keypad — directional mapping for MoM */
    key_xlat_scan[SDLK_TABLE_INDEX_FROM_SCANCODE(SDLK_KP_1)] = MOX_KEY_LEFTDOWN;
    key_xlat_scan[SDLK_TABLE_INDEX_FROM_SCANCODE(SDLK_KP_2)] = MOX_KEY_DOWN;
    key_xlat_scan[SDLK_TABLE_INDEX_FROM_SCANCODE(SDLK_KP_3)] = MOX_KEY_RIGHTDOWN;
    key_xlat_scan[SDLK_TABLE_INDEX_FROM_SCANCODE(SDLK_KP_4)] = MOX_KEY_LEFT;
    key_xlat_scan[SDLK_TABLE_INDEX_FROM_SCANCODE(SDLK_KP_6)] = MOX_KEY_RIGHT;
    key_xlat_scan[SDLK_TABLE_INDEX_FROM_SCANCODE(SDLK_KP_7)] = MOX_KEY_LEFTUP;
    key_xlat_scan[SDLK_TABLE_INDEX_FROM_SCANCODE(SDLK_KP_8)] = MOX_KEY_UP;
    key_xlat_scan[SDLK_TABLE_INDEX_FROM_SCANCODE(SDLK_KP_9)] = MOX_KEY_RIGHTUP;
}

uint32_t mod_xlat(uint16_t sdl2_key_modifiers)
{
    uint32_t mox_mod = 0;
    if (sdl2_key_modifiers & KMOD_SHIFT ) { mox_mod |= MOX_MOD_SHIFT; }
    if (sdl2_key_modifiers & KMOD_ALT   ) { mox_mod |= MOX_MOD_ALT;   }
    if (sdl2_key_modifiers & KMOD_CTRL  ) { mox_mod |= MOX_MOD_CTRL;  }
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
