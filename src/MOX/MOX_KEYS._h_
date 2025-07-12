#ifndef MOX_KEYS_H
#define MOX_KEYS_H

#include "MOX_TYPE.H"

/**
 * Enumeration of valid key mods (possibly OR'd together).
 */
enum s_ST_KEY_MOD
{
    ST_KEY_MOD_NONE     = 0x0000,
    ST_KEY_MOD_LSHIFT   = 0x0001,
    ST_KEY_MOD_RSHIFT   = 0x0002,
    ST_KEY_MOD_LCTRL    = 0x0040,
    ST_KEY_MOD_RCTRL    = 0x0080,
    ST_KEY_MOD_LALT     = 0x0100,
    ST_KEY_MOD_RALT     = 0x0200,
    ST_KEY_MOD_LGUI     = 0x0400,
    ST_KEY_MOD_RGUI     = 0x0800,
    ST_KEY_MOD_NUM      = 0x1000,
    ST_KEY_MOD_CAPS     = 0x2000,
    ST_KEY_MOD_MODE     = 0x4000,
    ST_KEY_MOD_SCROLL   = 0x8000,

    ST_KEY_MOD_CTRL  = ST_KEY_MOD_LCTRL  | ST_KEY_MOD_RCTRL,
    ST_KEY_MOD_SHIFT = ST_KEY_MOD_LSHIFT | ST_KEY_MOD_RSHIFT,
    ST_KEY_MOD_ALT   = ST_KEY_MOD_LALT   | ST_KEY_MOD_RALT,
    ST_KEY_MOD_GUI   = ST_KEY_MOD_LGUI   | ST_KEY_MOD_RGUI,

    ST_KEY_MOD_OVERRUN
};
// typedef enum s_ST_KEY_MOD;

/*

https://www.stanislavs.org/helppc/scan_codes.html
INT 16 - Keyboard Scan Codes
    on return:
    ZF = 0 if key pressed (data waiting)
    AX = 0 if no scan code is available
    AH = scan code
    AL = ASCII character or zero if special function key

    ? need 4 arrays ?
    normal, shift, control, alt
    ~ [4][128]
    enum Keystroke_Modifier { none, shift, control, alt }, provided by Platform
    
    NOTE: for SCCC, "a" is normal, "A" is shifted

    Keypad  NumPad
    ¿ Num Lock ?
    ¿ Arrow Keys vs. NumPad ?


*/
enum ScanCodeCharCode
{
/*
Key     Normal      Shifted     w/Ctrl      w/Alt
A       1E61        1E41        1E01        1E00
B      3062         3042        3002        3000
C      2E63         2E42        2E03        2E00
D      2064         2044        2004        2000
E      1265         1245        1205        1200
F      2166         2146        2106        2100
G      2267         2247        2207        2200
H      2368         2348        2308        2300
I      1769         1749        1709        1700
J      246A         244A        240A        2400
K      256B         254B        250B        2500
L      266C         264C        260C        2600
M      326D         324D        320D        3200
N      316E         314E        310E        3100
O      186F         184F        180F        1800
P      1970         1950        1910        1900
Q      1071         1051        1011        1000
R      1372         1352        1312        1300
S      1F73         1F53        1F13        1F00
T      1474         1454        1414        1400
U      1675         1655        1615        1600
V      2F76         2F56        2F16        2F00
W      1177         1157        1117        1100
X      2D78         2D58        2D18        2D00
Y      1579         1559        1519        1500
Z      2C7A         2C5A        2C1A        2C00
*/
    SCCC_A = 0x1E61,        // "A" key
    SCCC_B = 0x3062,        // "B" key
    SCCC_C = 0x2E63,        // "C" key
    SCCC_D = 0x2064,        // "D" key
    SCCC_E = 0x1265,        // "E" key
    SCCC_F = 0x2166,        // "F" key
    SCCC_G = 0x2267,        // "G" key
    SCCC_H = 0x2368,        // "H" key
    SCCC_I = 0x1769,        // "I" key
    SCCC_J = 0x246A,        // "J" key
    SCCC_K = 0x256B,        // "K" key
    SCCC_L = 0x266C,        // "L" key
    SCCC_M = 0x326D,        // "M" key
    SCCC_N = 0x316E,        // "N" key
    SCCC_O = 0x186F,        // "O" key
    SCCC_P = 0x1970,        // "P" key
    SCCC_Q = 0x1071,        // "Q" key
    SCCC_R = 0x1372,        // "R" key
    SCCC_S = 0x1F73,        // "S" key
    SCCC_T = 0x1474,        // "T" key
    SCCC_U = 0x1675,        // "U" key
    SCCC_V = 0x2F76,        // "V" key
    SCCC_W = 0x1177,        // "W" key
    SCCC_X = 0x2D78,        // "X" key
    SCCC_Y = 0x1579,        // "Y" key
    SCCC_Z = 0x2C7A,        // "Z" key

    SCCC_Shift_A = 0x1E41,  // "Shift" key + "A" key
    SCCC_Shift_B = 0x3042,  // "Shift" key + "B" key
    SCCC_Shift_C = 0x2E42,  // "Shift" key + "C" key
    SCCC_Shift_D = 0x2044,  // "Shift" key + "D" key
    SCCC_Shift_E = 0x1245,  // "Shift" key + "E" key
    SCCC_Shift_F = 0x2146,  // "Shift" key + "F" key
    SCCC_Shift_G = 0x2247,  // "Shift" key + "G" key
    SCCC_Shift_H = 0x2348,  // "Shift" key + "H" key
    SCCC_Shift_I = 0x1749,  // "Shift" key + "I" key
    SCCC_Shift_J = 0x244A,  // "Shift" key + "J" key
    SCCC_Shift_K = 0x254B,  // "Shift" key + "K" key
    SCCC_Shift_L = 0x264C,  // "Shift" key + "L" key
    SCCC_Shift_M = 0x324D,  // "Shift" key + "M" key
    SCCC_Shift_N = 0x314E,  // "Shift" key + "N" key
    SCCC_Shift_O = 0x184F,  // "Shift" key + "O" key
    SCCC_Shift_P = 0x1950,  // "Shift" key + "P" key
    SCCC_Shift_Q = 0x1051,  // "Shift" key + "Q" key
    SCCC_Shift_R = 0x1352,  // "Shift" key + "R" key
    SCCC_Shift_S = 0x1F53,  // "Shift" key + "S" key
    SCCC_Shift_T = 0x1454,  // "Shift" key + "T" key
    SCCC_Shift_U = 0x1655,  // "Shift" key + "U" key
    SCCC_Shift_V = 0x2F56,  // "Shift" key + "V" key
    SCCC_Shift_W = 0x1157,  // "Shift" key + "W" key
    SCCC_Shift_X = 0x2D58,  // "Shift" key + "X" key
    SCCC_Shift_Y = 0x1559,  // "Shift" key + "Y" key
    SCCC_Shift_Z = 0x2C5A,  // "Shift" key + "Z" key

    SCCC_Ctrl_A = 0x1E01,   // "Control" key + "A" key
    SCCC_Ctrl_B = 0x3002,   // "Control" key + "B" key
    SCCC_Ctrl_C = 0x2E03,   // "Control" key + "C" key
    SCCC_Ctrl_D = 0x2004,   // "Control" key + "D" key
    SCCC_Ctrl_E = 0x1205,   // "Control" key + "E" key
    SCCC_Ctrl_F = 0x2106,   // "Control" key + "F" key
    SCCC_Ctrl_G = 0x2207,   // "Control" key + "G" key
    SCCC_Ctrl_H = 0x2308,   // "Control" key + "H" key
    SCCC_Ctrl_I = 0x1709,   // "Control" key + "I" key
    SCCC_Ctrl_J = 0x240A,   // "Control" key + "J" key
    SCCC_Ctrl_K = 0x250B,   // "Control" key + "K" key
    SCCC_Ctrl_L = 0x260C,   // "Control" key + "L" key
    SCCC_Ctrl_M = 0x320D,   // "Control" key + "M" key
    SCCC_Ctrl_N = 0x310E,   // "Control" key + "N" key
    SCCC_Ctrl_O = 0x180F,   // "Control" key + "O" key
    SCCC_Ctrl_P = 0x1910,   // "Control" key + "P" key
    SCCC_Ctrl_Q = 0x1011,   // "Control" key + "Q" key
    SCCC_Ctrl_R = 0x1312,   // "Control" key + "R" key
    SCCC_Ctrl_S = 0x1F13,   // "Control" key + "S" key
    SCCC_Ctrl_T = 0x1414,   // "Control" key + "T" key
    SCCC_Ctrl_U = 0x1615,   // "Control" key + "U" key
    SCCC_Ctrl_V = 0x2F16,   // "Control" key + "V" key
    SCCC_Ctrl_W = 0x1117,   // "Control" key + "W" key
    SCCC_Ctrl_X = 0x2D18,   // "Control" key + "X" key
    SCCC_Ctrl_Y = 0x1519,   // "Control" key + "Y" key
    SCCC_Ctrl_Z = 0x2C1A,   // "Control" key + "Z" key

    SCCC_Alt_A = 0x1E00,    // "Alt" key + "A" key
    SCCC_Alt_B = 0x3000,    // "Alt" key + "B" key
    SCCC_Alt_C = 0x2E00,    // "Alt" key + "C" key
    SCCC_Alt_D = 0x2000,    // "Alt" key + "D" key
    SCCC_Alt_E = 0x1200,    // "Alt" key + "E" key
    SCCC_Alt_F = 0x2100,    // "Alt" key + "F" key
    SCCC_Alt_G = 0x2200,    // "Alt" key + "G" key
    SCCC_Alt_H = 0x2300,    // "Alt" key + "H" key
    SCCC_Alt_I = 0x1700,    // "Alt" key + "I" key
    SCCC_Alt_J = 0x2400,    // "Alt" key + "J" key
    SCCC_Alt_K = 0x2500,    // "Alt" key + "K" key
    SCCC_Alt_L = 0x2600,    // "Alt" key + "L" key
    SCCC_Alt_M = 0x3200,    // "Alt" key + "M" key
    SCCC_Alt_N = 0x3100,    // "Alt" key + "N" key
    SCCC_Alt_O = 0x1800,    // "Alt" key + "O" key
    SCCC_Alt_P = 0x1900,    // "Alt" key + "P" key
    SCCC_Alt_Q = 0x1000,    // "Alt" key + "Q" key
    SCCC_Alt_R = 0x1300,    // "Alt" key + "R" key
    SCCC_Alt_S = 0x1F00,    // "Alt" key + "S" key
    SCCC_Alt_T = 0x1400,    // "Alt" key + "T" key
    SCCC_Alt_U = 0x1600,    // "Alt" key + "U" key
    SCCC_Alt_V = 0x2F00,    // "Alt" key + "V" key
    SCCC_Alt_W = 0x1100,    // "Alt" key + "W" key
    SCCC_Alt_X = 0x2D00,    // "Alt" key + "X" key
    SCCC_Alt_Y = 0x1500,    // "Alt" key + "Y" key
    SCCC_Alt_Z = 0x2C00,    // "Alt" key + "Z" key
/*
Key     Normal      Shifted     w/Ctrl      w/Alt
1       0231        0221        7800
2       0332        0340        0300        7900
3       0433        0423        7A00
4       0534        0524        7B00
5       0635        0625        7C00
6       0736        075E        071E        7D00
7       0837        0826        7E00
8       0938        092A        7F00
9       0A39        0A28        8000
0       0B30        0B29        8100
*/
    SCCC_1 = 0x0231,      // '1' "one" key
    SCCC_2 = 0x0332,      // '2' "two" key
    SCCC_3 = 0x0433,      // '3' "three" key
    SCCC_4 = 0x0534,      // '4' "four" key
    SCCC_5 = 0x0635,      // '5' "five" key
    SCCC_6 = 0x0736,      // '6' "six" key
    SCCC_7 = 0x0837,      // '7' "seven" key
    SCCC_8 = 0x0938,      // '8' "eight" key
    SCCC_9 = 0x0A39,      // '9' "nine" key
    SCCC_0 = 0x0B30,      // '0' "zero" key
/*
Key     Normal    Shifted   w/Ctrl    w/Alt
-      0C2D        0C5F      0C1F    8200
=      0D3D        0D2B        8300
[      1A5B        1A7B      1A1B    1A00
]      1B5D        1B7D      1B1D    1B00
;      273B        273A        2700
'      2827        2822
`      2960        297E
\      2B5C        2B7C      2B1C    2600 (same as Alt L)
,      332C        333C
.      342E        343E
/      352F        353F
*/
SCCC_MINUS = 0x0C2D,      // '-' "minus" key
SCCC_EQUALS = 0x0D3D,      // '=' "equals" key

SCCC_PLUS = 0x0D2B,      // '+' "plus" key

/*
Key     Normal      Shifted   w/Ctrl  w/Alt
F1      3B00        5400      5E00    6800
F2      3C00        5500      5F00    6900
F3      3D00        5600      6000    6A00
F4      3E00        5700      6100    6B00
F5      3F00        5800      6200    6C00
F6      4000        5900      6300    6D00
F7      4100        5A00      6400    6E00
F8      4200        5B00      6500    6F00
F9      4300        5C00      6600    7000
F10     4400        5D00      6700    7100
F11     8500        8700      8900    8B00
F12     8600        8800      8A00    8C00
*/
    SCCC_F10 = 0x4400,      // "F10" key

    SCCC_BACKSPACE = 0x0E08,    // BackSpace
    SCCC_DELETE = 0x5300,       // Delete
    SCCC_KP_2       = 0x5000,   // Numeric Keypad 2 (Down)
    SCCC_DOWN       = 0x50E0,   // Down Arrow  ||  NumLock + Numeric Keypad 2 (Down)
    SCCC_KP_1       = 0x4F00,   // Numeric Keypad 1 (End)
    SCCC_DOWNLEFT   = 0x4FE0,   // NumLock + Numeric Keypad 1 (DownLeft)
    SCCC_ENTER      = 0x1C0D,   // Enter
    SCCC_ESCAPE     = 0x011B,   // Escape
    SCCC_KP_7       = 0x4700,   // Numeric Keypad 7 (Home)
    SCCC_UPLEFT     = 0x47E0,   // NumLock + Numeric Keypad 7 (UpLeft)
    // Ins         5200
    // Keypad 5        
    // Keypad *    372A
    // Keypad -    4A2D
    // Keypad +    4E2B
    // Keypad /    352F
    SCCC_KP_4       = 0x4B00,   // Numeric Keypad 4 (Left)
    SCCC_LEFT       = 0x4BE0,   // Left Arrow  ||  NumLock + Numeric Keypad 4 (Left)
    SCCC_KP_3       = 0x5100,   // Numeric Keypad 3 (PgDn)
    SCCC_DOWNRIGHT  = 0x51E0,   // NumLock + Numeric Keypad 3 (DownRight)
    SCCC_KP_9       = 0x4900,   // Numeric Keypad 9 (PgUp)
    SCCC_UPRIGHT    = 0x49E0,   // NumLock + Numeric Keypad 9 (UpRight)
    // PrtSc           
    SCCC_KP_6       = 0x4D00,   // Numeric Keypad 6 (Right)
    SCCC_RIGHT      = 0x4DE0,   // Right Arrow  ||  NumLock + Numeric Keypad 6 (Right)
    SCCC_SPACE      = 0x3920,   // Space-Bar
    // Tab         0F09
    SCCC_KP_8       = 0x4800,   // Numeric Keypad 8 (Up)
    SCCC_UP         = 0x48E0,   // Up Arrow  ||  NumLock + Numeric Keypad 8 (Up)

/*
Key         Normal  Shifted     w/Ctrl  w/Alt
BackSpace   0E08    0E08        0E7F    0E00
Del         5300    532E        9300    A300
Down Arrow  5000    5032        9100    A000
End         4F00    4F31        7500    9F00
Enter       1C0D    1C0D        1C0A    A600
Esc         011B    011B        011B    0100
Home        4700    4737        7700    9700
Ins         5200    5230        9200    A200
Keypad 5            4C35        8F00
Keypad *    372A                9600    3700
Keypad -    4A2D    4A2D        8E00    4A00
Keypad +    4E2B    4E2B                4E00
Keypad /    352F    352F        9500    A400
Left Arrow  4B00    4B34        7300    9B00
PgDn        5100    5133        7600    A100
PgUp        4900    4939        8400    9900
PrtSc                           7200
Right Arrow 4D00    4D36        7400    9D00
SpaceBar    3920    3920        3920    3920
Tab         0F09    0F00        9400    A500
Up Arrow    4800    4838        8D00    9800
*/

    SCCC_OVERRUN
};
//     /*  32 0x20 */  0x3920,  /* VK_SPACE */
//     /*  48 0x30 */  0x0B30,  /* VK_0 */
//     /*  49 0x31 */  0x0231,  /* VK_1 */
//     /*  50 0x32 */  0x0332,  /* VK_2 */
//     /*  51 0x33 */  0x0433,  /* VK_3 */
//     /*  52 0x34 */  0x0534,  /* VK_4 */
//     /*  53 0x35 */  0x0635,  /* VK_5 */
//     /*  54 0x36 */  0x0736,  /* VK_6 */
//     /*  55 0x37 */  0x0837,  /* VK_7 */
//     /*  56 0x38 */  0x0938,  /* VK_8 */
//     /*  57 0x39 */  0x0A39,  /* VK_9 */


//     /*  66 0x42 */  0x3062,  /* VK_B */
//     /*  67 0x43 */  0x2E63,  /* VK_C */
//     /*  68 0x44 */  0x2064,  /* VK_D */
//     /*  69 0x45 */  0x1265,  /* VK_E */
//     /*  70 0x46 */  0x2166,  /* VK_F */
//     /*  71 0x47 */  0x2267,  /* VK_G */
//     /*  72 0x48 */  0x2368,  /* VK_H */
//     /*  73 0x49 */  0x1769,  /* VK_I */
//     /*  74 0x4A */  0x246A,  /* VK_J */
//     /*  75 0x4B */  0x256B,  /* VK_K */
//     /*  76 0x4C */  0x266C,  /* VK_L */
//     /*  77 0x4D */  0x326D,  /* VK_M */
//     /*  78 0x4E */  0x316E,  /* VK_N */
//     /*  79 0x4F */  0x186F,  /* VK_O */
//     /*  80 0x50 */  0x1970,  /* VK_P */

//     /*  82 0x52 */  0x1372,  /* VK_R */
//     /*  83 0x53 */  0x1F73,  /* VK_S */
//     /*  84 0x54 */  0x1474,  /* VK_T */
//     /*  85 0x55 */  0x1675,  /* VK_U */
//     /*  86 0x56 */  0x2F76,  /* VK_V */
//     /*  87 0x57 */  0x1177,  /* VK_W */
//     /*  88 0x58 */  0x2D78,  /* VK_X */
//     /*  89 0x59 */  0x1579,  /* VK_Y */
//     /*  90 0x5A */  0x2C7A,  /* VK_Z */

/*

    the manual says "by hitting the keys on the numeric keypad"
    {1,2,3,4,5,6,7,8}
    {KP_Left, KP_Right, KP_Up, KP_Down, KP_RightUp, KP_RightDown, KP_LeftUp, KP_LeftDown}

KP_Left  = 1
KP_Right  = 2
KP_Up  = 3
KP_Down  = 4
KP_RightUp  = 5
KP_RightDown  = 6
KP_LeftUp  = 7
KP_LeftDown  = 8

The regular Home, End, PgUp, PgDn keys do not do anything.
Not sure what 'Enter' is doing with Unit Movement, or why it is even matching an input field.


SCANCODES FOR SCANCODE SET 1 (XT)
US 104-key keyboard, set 1 scancodes
When the kayboard's internal NumLock is active:
    two-byte repeat code  = E0nn

4 (L)  4B
(L) E04B

So, ...
Numeric Keypad 4 is 0x4B
Left is 0xE04B
Anywhoodle, ...
    just map Numeric Keypard 4 and Left to SimTex Left

*/

enum e_SimTex_Key_Codes
{
//     KP_NumMult         = 0x00,
    ST_KEY_LEFT             = 0x01,
    ST_KEY_RIGHT            = 0x02,
    ST_KEY_UP               = 0x03,
    ST_KEY_DOWN             = 0x04,
    ST_KEY_RIGHTUP          = 0x05,
    ST_KEY_RIGHTDOWN        = 0x06,
    ST_KEY_LEFTUP           = 0x07,
    ST_KEY_LEFTDOWN         = 0x08,
//     KP_Insert          = 0x09,
    ST_KEY_DELETE           = 0x0A,
    ST_KEY_BACKSPACE        = 0x0B,
    ST_KEY_ENTER            = 0x0C,
    ST_KEY_TAB              = 0x0D,
    ST_KEY_F1               = 0x0E,
    ST_KEY_F2               = 0x0F,
    ST_KEY_F3               = 0x10,
    ST_KEY_F4               = 0x11,
    ST_KEY_F5               = 0x12,
    ST_KEY_F6               = 0x13,
    ST_KEY_F7               = 0x14,
    ST_KEY_F8               = 0x15,
    ST_KEY_F9               = 0x16,
    ST_KEY_F10              = 0x17,
    // IDK ST_KEY_KP_9             = 0x19,  /* PAGEUP */
    // IDK ST_KEY_KP_7             = 0x1A,  /* HOME */
    ST_KEY_ESCAPE           = 0x1B,
//     KP_End             = 0x1C,
    // IDK ST_KEY_KP_3             = 0x1D,  /* PAGEDOWN */
//     KP_Center          = 0x1E,
//     KP_NumPlus         = 0x2B,
//     KP_NumMinus        = 0x2D,
//     KP_Ctrl_Right      = 0x80,
//     KP_Ctrl_Left       = 0x81,
//     KP_Ctrl_Up         = 0x82,
//     KP_Ctrl_Down       = 0x83,
//     KP_Ctrl_LeftUp     = 0x84,
//     KP_Ctrl_RightUp    = 0x85,
//     KP_Ctrl_LeftDown   = 0x86,
//     KP_Ctrl_RightDown  = 0x87,
//     KP_Ctrl_s          = 0x90,
    ST_KEY_F11              = 0x91,
    ST_KEY_F12              = 0x92,
/* MoO2  Get_Alt_Alpha_Key()  character => alpha_key */
    ST_KEY_ALT_A            = 0xA0,  /* 160d  160 - 95 = 65 41h 'A' */
    ST_KEY_ALT_B            = 0xA1,
    ST_KEY_ALT_C            = 0xA2,
    ST_KEY_ALT_D            = 0xA3,
    ST_KEY_ALT_E            = 0xA4,
    ST_KEY_ALT_F            = 0xA5,
    ST_KEY_ALT_G            = 0xA6,
    ST_KEY_ALT_H            = 0xA7,
    ST_KEY_ALT_I            = 0xA8,
    ST_KEY_ALT_J            = 0xA9,
    ST_KEY_ALT_K            = 0xAA,
    ST_KEY_ALT_L            = 0xAB,
    ST_KEY_ALT_M            = 0xAC,
    ST_KEY_ALT_N            = 0xAD,
    ST_KEY_ALT_O            = 0xAE,
    ST_KEY_ALT_P            = 0xAF,
    ST_KEY_ALT_Q            = 0xB0,
    ST_KEY_ALT_R            = 0xB1,
    ST_KEY_ALT_S            = 0xB2,
    ST_KEY_ALT_T            = 0xB3,
    ST_KEY_ALT_U            = 0xB4,
    ST_KEY_ALT_V            = 0xB5,
    ST_KEY_ALT_W            = 0xB6,
    ST_KEY_ALT_X            = 0xB7,
    ST_KEY_ALT_Y            = 0xB8,
    ST_KEY_ALT_Z            = 0xB9,

    ST_KEY_OVERRUN
};



extern uint16_t Alt_Keycodes[26][2];

/*
enum EKEYCODES
EKey_Num7_Home  = 47h
EKey_Num8_Up  = 48h
EKey_Num9_PgUp  = 49h
EKey_NumMinus  = 4Ah
EKey_Num4_Left  = 4Bh
EKey_Num5  = 4Ch
EKey_Num6_Right  = 4Dh
EKey_NumPlus  = 4Eh
EKey_Num1_End  = 4Fh
EKey_Num2_Down  = 50h
EKey_Num3_PgDn  = 51h
EKey_Esc  = 11Bh
EKey_NumMult  = 92Ah
EKey_Backspace  = 0E08h
EKey_Tab  = 0F09h
EKey_Alt_q  = 1000h
EKey_Alt_w  = 1100h
EKey_Alt_e  = 1200h
EKey_Alt_r  = 1300h
EKey_Alt_t  = 1400h
EKey_Alt_y  = 1500h
EKey_Alt_u  = 1600h
EKey_Alt_i  = 1700h
EKey_Alt_o  = 1800h
EKey_Alt_p  = 1900h
EKey_Enter  = 1C0Dh
EKey_Alt_a  = 1E00h
EKey_Alt_s  = 1F00h
EKey_Ctrl_s  = 1F13h
EKey_Alt_d  = 2000h
EKey_Alt_f  = 2100h
EKey_Alt_g  = 2200h
EKey_Alt_h  = 2300h
EKey_Alt_j  = 2400h
EKey_Alt_k  = 2500h
EKey_Alt_l  = 2600h
EKey_Alt_z  = 2C00h
EKey_Alt_x  = 2D00h
EKey_Alt_c  = 2E00h
EKey_Alt_v  = 2F00h
EKey_Alt_b  = 3000h
EKey_Alt_n  = 3100h
EKey_Alt_m  = 3200h
EKey_F1  = 3B00h
EKey_F2  = 3C00h
EKey_F3  = 3D00h
EKey_F4  = 3E00h
EKey_F5  = 3F00h
EKey_F6  = 4000h
EKey_F7  = 4100h
EKey_F8  = 4200h
EKey_F9  = 4300h
EKey_F10  = 4400h
EKey_Home  = 47E0h
EKey_Up  = 48E0h
EKey_PgUp  = 49E0h
EKey_Left  = 4BE0h
EKey_Right  = 4DE0h
EKey_End  = 4FE0h
EKey_Down  = 50E0h
EKey_PgDn  = 51E0h
EKey_Ins  = 52E0h
EKey_Del  = 53E0h
EKey_Ctrl_Num4  = 7300h
EKey_Ctrl_Left  = 73E0h
EKey_Ctrl_Num6  = 7400h
EKey_Ctrl_Right  = 74E0h
EKey_Ctrl_Num1  = 7500h
EKey_Ctrl_Num3  = 7600h
EKey_Ctrl_Num7  = 7700h
EKey_Ctrl_Num9  = 8400h
EKey_F11  = 8500h
EKey_F12  = 8600h
EKey_Ctrl_Num8  = 8D00h
EKey_Ctrl_Up  = 8DE0h
EKey_Ctrl_Num2  = 9100h
EKey_Ctrl_Down  = 91E0h
EKey_NumEnter  = 0E00Dh
*/

#endif  /* MOX_KEYS_H */
