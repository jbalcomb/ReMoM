#ifndef SDL2_KD_H
#define SDL2_KD_H

#include "MOX/MOX_TYPE.h"

#include "sdl2_PFL.h"

#include <SDL.h>



#define PLATFORM_KEYBOARD_BUFFER_LENGTH 256  // MoO2  40

#define MOX_MOD_NONE    0
#define MOX_MOD_SHIFT   (1 << 16)
#define MOX_MOD_ALT     (1 << 17)
#define MOX_MOD_CTRL    (1 << 18)
#define MOX_MOD_META    (1 << 19)
#define MOX_MOD_ALL     (MOX_MOD_SHIFT | MOX_MOD_ALT | MOX_MOD_CTRL | MOX_MOD_META)

#define KBD_GET_KEY(_k_)    ((int)((_k_) & 0xff))
#define KBD_GET_CHAR(_k_)   ((char)(((_k_) >> 8) & 0xff))
#define KBD_GET_MOD(_k_)   ((_k_) & MOX_MOD_ALL)
#define KBD_GET_KEYMOD(_k_) ((_k_) & (MOX_MOD_ALL | 0xff))
#define KBD_MOD_ONLY_SHIFT(_k_) (KBD_GET_MOD(_k_) == MOX_MOD_SHIFT)
#define KBD_MOD_ONLY_CTRL(_k_)  (KBD_GET_MOD(_k_) == MOX_MOD_CTRL)
#define KBD_MOD_ONLY_ALT(_k_)   ((((_k_) & (MOX_MOD_ALT | MOX_MOD_META)) != 0) && (((_k_) & (MOX_MOD_SHIFT | MOX_MOD_CTRL)) == 0))



enum e_MOX_KEY
{
    MOX_KEY_UNKNOWN         = 0,

    MOX_KEY_NUMMULT         =  0,  //   0  NUL (null)
    MOX_KEY_LEFT            =  1,  //   1  SOH (start of heading)
    MOX_KEY_RIGHT           =  2,  //   2  STX (start of text)
    MOX_KEY_UP              =  3,  //   3  ETX (end of text)
    MOX_KEY_DOWN            =  4,  //   4  EOT (end of transmission)
    MOX_KEY_RIGHTUP         =  5,  //   5  ENQ (enquiry)
    MOX_KEY_RIGHTDOWN       =  6,  //   6  ACK (acknowledge)
    MOX_KEY_LEFTUP          =  7,  //   7  BEL (bell)
    MOX_KEY_LEFTDOWN        =  8,  //   8  BS  (backspace)
    MOX_KEY_INSERT          =  9,  //   9  TAB (horizontal tab)
    MOX_KEY_DELETE          = 10,  //  10  LF  (NL line feed, new line)
    MOX_KEY_BACKSPACE       = 11,  //  11  VT  (vertical tab)
    MOX_KEY_ENTER           = 12,  //  12  FF  (NP form feed, new page)
    MOX_KEY_TAB             = 13,  //  13  CR  (carriage return)
    MOX_KEY_F1              = 14,  //  14  SO  (shift out)
    MOX_KEY_F2              = 15,  //  15  SI  (shift in)
    MOX_KEY_F3              = 16,  //  16  DLE (data link escape)
    MOX_KEY_F4              = 17,  //  17  DC1 (device control 1)
    MOX_KEY_F5              = 18,  //  18  DC2 (device control 2)
    MOX_KEY_F6              = 19,  //  19  DC3 (device control 3)
    MOX_KEY_F7              = 20,  //  20  DC4 (device control 4)
    MOX_KEY_F8              = 21,  //  21  NAK (negative acknowledge)
    MOX_KEY_F9              = 22,  //  22  SYN (synchronous idle)
    MOX_KEY_F10             = 23,  //  23  ETB (end of trans. block)
    //  24  CAN (cancel)
    MOX_KEY_PGUP            = 25,  // 25  EM  (end of medium)
    MOX_KEY_HOME            = 26,  // 26  SUB (substitute)
    MOX_KEY_ESCAPE          = 27,  // 27  ESC (escape)
    MOX_KEY_END             = 28,  // 28  FS  (file separator)
    MOX_KEY_PGDN            = 29,  // 29  GS  (group separator)
    MOX_KEY_CENTER          = 30,  // 30  RS  (record separator)
    //  31  US  (unit separator)
    MOX_KEY_SPACE           = 32,  //  32  SPACE
    //  33  !
    //  34  "
    //  35  #
    //  36  $
    //  37  %
    //  38  &
    //  39  '
    //  40  (
    //  41  )
    //  42  *
    MOX_KEY_NUMPLUS         = 43,  // 43  +
    //  44  ,
    MOX_KEY_NUMMINUS        = 45,  // 45  -
    //  45  -
    //  46  .
    //  47  /
    //  48  0
    //  49  1
    //  50  2
    //  51  3
    //  52  4
    //  53  5
    //  54  6
    //  55  7
    //  56  8
    //  57  9
    //  58  :
    //  59  ;
    //  60  <
    //  61  =
    //  62  >
    //  63  ?
    //  64  @
    //  65  A
    //  66  B
    //  67  C
    //  68  D
    //  69  E
    //  70  F
    //  71  G
    //  72  H
    //  73  I
    //  74  J
    //  75  K
    //  76  L
    //  77  M
    //  78  N
    //  79  O
    //  80  P
    //  81  Q
    //  82  R
    //  83  S
    //  84  T
    //  85  U
    //  86  V
    //  87  W
    //  88  X
    //  89  Y
    //  90  Z
    //  91  [
    //  92  '\'
    //  93  ]
    //  94  ^
    //  95  _
    //  96  `
    MOX_KEY_a               =  97,  // 0x61   97  a
    MOX_KEY_b               =  98,  //        98  b
    MOX_KEY_c               =  99,  //        99  c
    MOX_KEY_d               = 100,  //       100  d
    MOX_KEY_e               = 101,  //       101  e
    MOX_KEY_f               = 102,  //       102  f
    MOX_KEY_g               = 103,  //       103  g
    MOX_KEY_h               = 104,  //       104  h
    MOX_KEY_i               = 105,  //       105  i
    MOX_KEY_j               = 106,  //       106  j
    MOX_KEY_k               = 107,  //       107  k
    MOX_KEY_l               = 108,  //       108  l
    MOX_KEY_m               = 109,  //       109  m
    MOX_KEY_n               = 110,  //       110  n
    MOX_KEY_o               = 111,  //       111  o
    MOX_KEY_p               = 112,  //       112  p
    MOX_KEY_q               = 113,  //       113  q
    MOX_KEY_r               = 114,  //       114  r
    MOX_KEY_s               = 115,  //       115  s
    MOX_KEY_t               = 116,  //       116  t
    MOX_KEY_u               = 117,  //       117  u
    MOX_KEY_v               = 118,  //       118  v
    MOX_KEY_w               = 119,  //       119  w
    MOX_KEY_x               = 120,  //       120  x
    MOX_KEY_y               = 121,  //       121  y
    MOX_KEY_z               = 122,  // 0x7A  122  z
    // 123  {
    // 124  |
    // 125  }
    // 126  ~
    // 127  DEL

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


struct s_KEYBOARD_BUFFER
{
    int key_write;
    int key_read;
    // uint32_t key_num[PLATFORM_KEYBOARD_BUFFER_LENGTH];
    uint32_t kilgore_key[PLATFORM_KEYBOARD_BUFFER_LENGTH];
    uint32_t mox_mod;
    BOOLVEC_DECLARE(pressed, MOX_KEY_OVERRUN);
};



#ifdef __cplusplus
extern "C" {
#endif


// static int Platform_Translated_Key(SDL_Keysym * sdl2_keysym);
// static int Platform_Localized_Key(SDL_Keysym * sdl2_keysym);
// static int Platform_Typed_Character(SDL_Keysym * sdl2_keysym);

/*

*/
void Platform_Keyboard_Event(SDL_Event * sdl2_event);

void Platform_Keyboard_Buffer_Clear(void);

// void Platform_Keyboard_Buffer_Add_Key_Press(SDL_Keysym * sdl2_keysym);
void Platform_Keyboard_Buffer_Add_Key_Press(int mox_key, uint32_t mox_mod, char mox_character);
// int Platform_Translate_SDL2_To_SCCC(SDL_Keysym * sdl2_keysym);
// int Platform_Translate_SCCC_To_MOX_KEY(uint16_t sccc);
uint16_t Platform_Translate_Virtual_Key_Code_To_MOX_Key_Num(int32_t virtual_key_code, uint16_t key_modifiers);

void build_key_xlat(void);

#ifdef __cplusplus
}
#endif

#endif  /* #define SDL2_KD_H */
