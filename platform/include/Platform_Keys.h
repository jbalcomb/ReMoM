/**
 * Platform_Keys.h — Key codes, modifier flags, and keyboard buffer types.
 *
 * Extracted from the engine to live at the platform boundary. These types have
 * NO platform-specific dependencies (no SDL, no Win32). Both the engine and
 * the platform implementation include this header.
 */

#ifndef PLATFORM_KEYS_H
#define PLATFORM_KEYS_H

#include <stdint.h>
#include <string.h>



/* ========================================================================= */
/*  Keyboard Buffer                                                          */
/* ========================================================================= */

#define PLATFORM_KEYBOARD_BUFFER_LENGTH 256



/* ========================================================================= */
/*  Modifier Flags                                                           */
/* ========================================================================= */

#define MOX_MOD_NONE    0
#define MOX_MOD_SHIFT   (1 << 16)
#define MOX_MOD_ALT     (1 << 17)
#define MOX_MOD_CTRL    (1 << 18)
#define MOX_MOD_META    (1 << 19)
#define MOX_MOD_ALL     (MOX_MOD_SHIFT | MOX_MOD_ALT | MOX_MOD_CTRL | MOX_MOD_META)



/* ========================================================================= */
/*  Key Extraction Macros                                                    */
/* ========================================================================= */

#define KBD_GET_KEY(_k_)        ((int)((_k_) & 0xff))
#define KBD_GET_CHAR(_k_)      ((char)(((_k_) >> 8) & 0xff))
#define KBD_GET_MOD(_k_)       ((_k_) & MOX_MOD_ALL)
#define KBD_GET_KEYMOD(_k_)    ((_k_) & (MOX_MOD_ALL | 0xff))
#define KBD_MOD_ONLY_SHIFT(_k_) (KBD_GET_MOD(_k_) == MOX_MOD_SHIFT)
#define KBD_MOD_ONLY_CTRL(_k_)  (KBD_GET_MOD(_k_) == MOX_MOD_CTRL)
#define KBD_MOD_ONLY_ALT(_k_)   ((((_k_) & (MOX_MOD_ALT | MOX_MOD_META)) != 0) && (((_k_) & (MOX_MOD_SHIFT | MOX_MOD_CTRL)) == 0))



/* ========================================================================= */
/*  Boolean Vector Macros (for keyboard pressed-state bitmask)               */
/* ========================================================================= */

#define BOOLVEC_DECLARE(_name_, _fnum_)  uint8_t _name_[((_fnum_) + 7) / 8]
#define BOOLVEC_CLEAR(_name_, _fnum_)    do { if ((_fnum_) <= 8) { (_name_)[0] = 0; } else { memset(&(_name_), 0, sizeof(_name_)); } } while (0)
#define BOOLVEC_SET(_name_, _i_, _v_)    do { int ti = (_i_); uint8_t *tp = &(_name_[ti / 8]); if ((_v_)) { *tp |= (1 << (ti & 7)); } else { *tp &= ~(1 << (ti & 7)); } } while (0)
#define BOOLVEC_IS1(_name_, _i_)         ((_name_[(_i_) / 8] & (1 << ((_i_) & 7))) != 0)



/* ========================================================================= */
/*  MOX Key Codes                                                            */
/* ========================================================================= */

enum e_MOX_KEY
{
    MOX_KEY_UNKNOWN         = 0,

    MOX_KEY_NUMMULT         =  0,
    MOX_KEY_LEFT            =  1,
    MOX_KEY_RIGHT           =  2,
    MOX_KEY_UP              =  3,
    MOX_KEY_DOWN            =  4,
    MOX_KEY_RIGHTUP         =  5,
    MOX_KEY_RIGHTDOWN       =  6,
    MOX_KEY_LEFTUP          =  7,
    MOX_KEY_LEFTDOWN        =  8,
    MOX_KEY_INSERT          =  9,
    MOX_KEY_DELETE          = 10,
    MOX_KEY_BACKSPACE       = 11,
    MOX_KEY_ENTER           = 12,
    MOX_KEY_TAB             = 13,
    MOX_KEY_F1              = 14,
    MOX_KEY_F2              = 15,
    MOX_KEY_F3              = 16,
    MOX_KEY_F4              = 17,
    MOX_KEY_F5              = 18,
    MOX_KEY_F6              = 19,
    MOX_KEY_F7              = 20,
    MOX_KEY_F8              = 21,
    MOX_KEY_F9              = 22,
    MOX_KEY_F10             = 23,
    MOX_KEY_PGUP            = 25,
    MOX_KEY_HOME            = 26,
    MOX_KEY_ESCAPE          = 27,
    MOX_KEY_END             = 28,
    MOX_KEY_PGDN            = 29,
    MOX_KEY_CENTER          = 30,
    MOX_KEY_SPACE           = 32,
    MOX_KEY_NUMPLUS         = 43,
    MOX_KEY_NUMMINUS        = 45,

    MOX_KEY_a               =  97,
    MOX_KEY_b               =  98,
    MOX_KEY_c               =  99,
    MOX_KEY_d               = 100,
    MOX_KEY_e               = 101,
    MOX_KEY_f               = 102,
    MOX_KEY_g               = 103,
    MOX_KEY_h               = 104,
    MOX_KEY_i               = 105,
    MOX_KEY_j               = 106,
    MOX_KEY_k               = 107,
    MOX_KEY_l               = 108,
    MOX_KEY_m               = 109,
    MOX_KEY_n               = 110,
    MOX_KEY_o               = 111,
    MOX_KEY_p               = 112,
    MOX_KEY_q               = 113,
    MOX_KEY_r               = 114,
    MOX_KEY_s               = 115,
    MOX_KEY_t               = 116,
    MOX_KEY_u               = 117,
    MOX_KEY_v               = 118,
    MOX_KEY_w               = 119,
    MOX_KEY_x               = 120,
    MOX_KEY_y               = 121,
    MOX_KEY_z               = 122,

    MOX_KEY_CTRL_RIGHT      = 128,
    MOX_KEY_CTRL_LEFT       = 129,
    MOX_KEY_CTRL_UP         = 130,
    MOX_KEY_CTRL_DOWN       = 131,
    MOX_KEY_CTRL_LEFTUP     = 132,
    MOX_KEY_CTRL_RIGHTUP    = 133,
    MOX_KEY_CTRL_LEFTDOWN   = 134,
    MOX_KEY_CTRL_RIGHTDOWN  = 135,

    MOX_KEY_CTRL_S          = 144,

    MOX_KEY_F11             = 145,
    MOX_KEY_F12             = 146,

    MOX_KEY_ALT_A           = 160,
    MOX_KEY_ALT_B           = 161,
    MOX_KEY_ALT_C           = 162,
    MOX_KEY_ALT_D           = 163,
    MOX_KEY_ALT_E           = 164,
    MOX_KEY_ALT_F           = 165,
    MOX_KEY_ALT_G           = 166,
    MOX_KEY_ALT_H           = 167,
    MOX_KEY_ALT_I           = 168,
    MOX_KEY_ALT_J           = 169,
    MOX_KEY_ALT_K           = 170,
    MOX_KEY_ALT_L           = 171,
    MOX_KEY_ALT_M           = 172,
    MOX_KEY_ALT_N           = 173,
    MOX_KEY_ALT_O           = 174,
    MOX_KEY_ALT_P           = 175,
    MOX_KEY_ALT_Q           = 176,
    MOX_KEY_ALT_R           = 177,
    MOX_KEY_ALT_S           = 178,
    MOX_KEY_ALT_T           = 179,
    MOX_KEY_ALT_U           = 180,
    MOX_KEY_ALT_V           = 181,
    MOX_KEY_ALT_W           = 182,
    MOX_KEY_ALT_X           = 183,
    MOX_KEY_ALT_Y           = 184,
    MOX_KEY_ALT_Z           = 185,

    MOX_KEY_OVERRUN
};



/* ========================================================================= */
/*  Keyboard Buffer Structure                                                */
/* ========================================================================= */

struct s_KEYBOARD_BUFFER
{
    int key_write;
    int key_read;
    uint32_t packed_key[PLATFORM_KEYBOARD_BUFFER_LENGTH];
    uint32_t mox_mod;
    BOOLVEC_DECLARE(pressed, MOX_KEY_OVERRUN);
};

/** The global keyboard buffer — defined in platform KD, used by engine Keyboard.c */
extern struct s_KEYBOARD_BUFFER platform_keyboard_buffer;



#endif /* PLATFORM_KEYS_H */
