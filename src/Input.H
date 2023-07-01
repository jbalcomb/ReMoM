#ifndef INPUT_H
#define INPUT_H

#include "MoX_TYPE.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
extern int16_t DBG_In_MouseButtonLeft;
extern int16_t DBG_In_MouseButtonRight;
extern int16_t DBG_movement_map_grid_field_idx;
extern int16_t DBG_reduced_map_grid_field_idx;

//char field_names[][];  // ¿ ~== char ** field_names; ?
#define FIELD_COUNT_MAX 128
// char field_names[][];
char * field_names[];
#endif


/*! @defgroup keys Keyboard keys
 *  @brief Keyboard key IDs.
 *
 *  See [key input](@ref input_key) for how these are used.
 *
 *  These key codes are inspired by the _USB HID Usage Tables v1.12_ (p. 53-60),
 *  but re-arranged to map to 7-bit ASCII for printable keys (function keys are
 *  put in the 256+ range).
 *
 *  The naming of the key codes follow these rules:
 *   - The US keyboard layout is used
 *   - Names of printable alphanumeric characters are used (e.g. "A", "R",
 *     "3", etc.)
 *   - For non-alphanumeric characters, Unicode:ish names are used (e.g.
 *     "COMMA", "LEFT_SQUARE_BRACKET", etc.). Note that some names do not
 *     correspond to the Unicode standard (usually for brevity)
 *   - Keys that lack a clear US mapping are named "WORLD_x"
 *   - For non-printable keys, custom names are used (e.g. "F4",
 *     "BACKSPACE", etc.)
 *
 *  @ingroup input
 *  @{
 */

/* The unknown key */
#define GLFW_KEY_UNKNOWN            -1

/* Printable keys */
#define GLFW_KEY_SPACE              32
#define GLFW_KEY_APOSTROPHE         39  /* ' */
#define GLFW_KEY_COMMA              44  /* , */
#define GLFW_KEY_MINUS              45  /* - */
#define GLFW_KEY_PERIOD             46  /* . */
#define GLFW_KEY_SLASH              47  /* / */
#define GLFW_KEY_0                  48
#define GLFW_KEY_1                  49
#define GLFW_KEY_2                  50
#define GLFW_KEY_3                  51
#define GLFW_KEY_4                  52
#define GLFW_KEY_5                  53
#define GLFW_KEY_6                  54
#define GLFW_KEY_7                  55
#define GLFW_KEY_8                  56
#define GLFW_KEY_9                  57
#define GLFW_KEY_SEMICOLON          59  /* ; */
#define GLFW_KEY_EQUAL              61  /* = */
#define GLFW_KEY_A                  65
#define GLFW_KEY_B                  66
#define GLFW_KEY_C                  67
#define GLFW_KEY_D                  68
#define GLFW_KEY_E                  69
#define GLFW_KEY_F                  70
#define GLFW_KEY_G                  71
#define GLFW_KEY_H                  72
#define GLFW_KEY_I                  73
#define GLFW_KEY_J                  74
#define GLFW_KEY_K                  75
#define GLFW_KEY_L                  76
#define GLFW_KEY_M                  77
#define GLFW_KEY_N                  78
#define GLFW_KEY_O                  79
#define GLFW_KEY_P                  80
#define GLFW_KEY_Q                  81
#define GLFW_KEY_R                  82
#define GLFW_KEY_S                  83
#define GLFW_KEY_T                  84
#define GLFW_KEY_U                  85
#define GLFW_KEY_V                  86
#define GLFW_KEY_W                  87
#define GLFW_KEY_X                  88
#define GLFW_KEY_Y                  89
#define GLFW_KEY_Z                  90
#define GLFW_KEY_LEFT_BRACKET       91  /* [ */
#define GLFW_KEY_BACKSLASH          92  /* \ */
#define GLFW_KEY_RIGHT_BRACKET      93  /* ] */
#define GLFW_KEY_GRAVE_ACCENT       96  /* ` */
#define GLFW_KEY_WORLD_1            161 /* non-US #1 */
#define GLFW_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define GLFW_KEY_ESCAPE             256
#define GLFW_KEY_ENTER              257
#define GLFW_KEY_TAB                258
#define GLFW_KEY_BACKSPACE          259
#define GLFW_KEY_INSERT             260
#define GLFW_KEY_DELETE             261
#define GLFW_KEY_RIGHT              262
#define GLFW_KEY_LEFT               263
#define GLFW_KEY_DOWN               264
#define GLFW_KEY_UP                 265
#define GLFW_KEY_PAGE_UP            266
#define GLFW_KEY_PAGE_DOWN          267
#define GLFW_KEY_HOME               268
#define GLFW_KEY_END                269
#define GLFW_KEY_CAPS_LOCK          280
#define GLFW_KEY_SCROLL_LOCK        281
#define GLFW_KEY_NUM_LOCK           282
#define GLFW_KEY_PRINT_SCREEN       283
#define GLFW_KEY_PAUSE              284
#define GLFW_KEY_F1                 290
#define GLFW_KEY_F2                 291
#define GLFW_KEY_F3                 292
#define GLFW_KEY_F4                 293
#define GLFW_KEY_F5                 294
#define GLFW_KEY_F6                 295
#define GLFW_KEY_F7                 296
#define GLFW_KEY_F8                 297
#define GLFW_KEY_F9                 298
#define GLFW_KEY_F10                299
#define GLFW_KEY_F11                300
#define GLFW_KEY_F12                301
#define GLFW_KEY_F13                302
#define GLFW_KEY_F14                303
#define GLFW_KEY_F15                304
#define GLFW_KEY_F16                305
#define GLFW_KEY_F17                306
#define GLFW_KEY_F18                307
#define GLFW_KEY_F19                308
#define GLFW_KEY_F20                309
#define GLFW_KEY_F21                310
#define GLFW_KEY_F22                311
#define GLFW_KEY_F23                312
#define GLFW_KEY_F24                313
#define GLFW_KEY_F25                314
#define GLFW_KEY_KP_0               320
#define GLFW_KEY_KP_1               321
#define GLFW_KEY_KP_2               322
#define GLFW_KEY_KP_3               323
#define GLFW_KEY_KP_4               324
#define GLFW_KEY_KP_5               325
#define GLFW_KEY_KP_6               326
#define GLFW_KEY_KP_7               327
#define GLFW_KEY_KP_8               328
#define GLFW_KEY_KP_9               329
#define GLFW_KEY_KP_DECIMAL         330
#define GLFW_KEY_KP_DIVIDE          331
#define GLFW_KEY_KP_MULTIPLY        332
#define GLFW_KEY_KP_SUBTRACT        333
#define GLFW_KEY_KP_ADD             334
#define GLFW_KEY_KP_ENTER           335
#define GLFW_KEY_KP_EQUAL           336
#define GLFW_KEY_LEFT_SHIFT         340
#define GLFW_KEY_LEFT_CONTROL       341
#define GLFW_KEY_LEFT_ALT           342
#define GLFW_KEY_LEFT_SUPER         343
#define GLFW_KEY_RIGHT_SHIFT        344
#define GLFW_KEY_RIGHT_CONTROL      345
#define GLFW_KEY_RIGHT_ALT          346
#define GLFW_KEY_RIGHT_SUPER        347
#define GLFW_KEY_MENU               348

#define GLFW_KEY_LAST               GLFW_KEY_MENU


/*
    ? need 4 arrays ?
    normal, shift, control, alt
    ~ [4][128]
    enum Keystroke_Modifier { none, shift, control, alt }, provided by Platform
    NOTE: for SCCC, "a" is normal, "A" is shifted
*/
enum ScanCodeCharCode
{
    SCCC_ESC = 0x011B,      // "Esc" key

    SCCC_A = 0x1E61,        // "A" key
    SCCC_B = 0x3062,        // "B" key
    SCCC_C = 0x2E63,        // "C" key
    SCCC_D = 0x2064,        // "D" key

    SCCC_Q = 0x1071,        // "Q" key

    SCCC_T = 0x1474,        // "T" key


    SCCC_Shift_A = 0x1E41,  // "Shift" key + "A" key


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

enum Simtex_Key_Codes
{
//     KP_NumMult         = 0x00,
    ST_KEY_KP_4             = 0x01,  /* LEFT */
    ST_KEY_KP_6             = 0x02,  /* RIGHT */
    STU_KEY_KP_8            = 0x03,  /* UP */
    STU_KEY_KP_2            = 0x04,  /* DOWN */
//     KP_RightUp         = 0x05,
//     KP_RightDown       = 0x06,
//     KP_LeftUp          = 0x07,
//     KP_LeftDown        = 0x08,
//     KP_Insert          = 0x09,
//     KP_Delete          = 0x0A,
//     KP_Backspace       = 0x0B,
//     KP_Enter           = 0x0C,
//     KP_Tab             = 0x0D,
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
    ST_KEY_KP_9             = 0x19,  /* PAGEUP */
    ST_KEY_KP_7             = 0x1A,  /* HOME */
    ST_KEY_ESCAPE           = 0x1B,
//     KP_End             = 0x1C,
    ST_KEY_KP_3             = 0x1D,  /* PAGEDOWN */
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
//     KP_F11             = 0x91,
//     KP_F12             = 0x92,
//     KP_Alt_a           = 0xA0,
//     KP_Alt_b           = 0xA1,
//     KP_Alt_c           = 0xA2,
//     KP_Alt_d           = 0xA3,
//     KP_Alt_e           = 0xA4,
//     KP_Alt_f           = 0xA5,
//     KP_Alt_g           = 0xA6,
//     KP_Alt_h           = 0xA7,
//     KP_Alt_i           = 0xA8,
//     KP_Alt_j           = 0xA9,
//     KP_Alt_k           = 0xAA,
//     KP_Alt_l           = 0xAB,
//     KP_Alt_m           = 0xAC,
//     KP_Alt_n           = 0xAD,
//     KP_Alt_o           = 0xAE,
//     KP_Alt_p           = 0xAF,
//     KP_Alt_q           = 0xB0,
//     KP_Alt_r           = 0xB1,
//     KP_Alt_s           = 0xB2,
//     KP_Alt_t           = 0xB3,
//     KP_Alt_u           = 0xB4,
//     KP_Alt_v           = 0xB5,
//     KP_Alt_w           = 0xB6,
//     KP_Alt_x           = 0xB7,
//     KP_Alt_y           = 0xB8,
//     KP_Alt_z           = 0xB9,
    ST_KEY_OVERRUN
};



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


/*
    WIZARDS.EXE seg036
    MAGIC.EXE seg034
*/

// WZD s36p01
int16_t Interpret_Mouse_Input(void);


// WZD s36p08
void Set_Global_ESC(void);


// WZD s36p25
int16_t Scan_Field(void);

// WZD s36p26
int16_t Scan_Input(void);


// WZD s36p65  AKA GUI_Init()  AKA IN_Init()
void Init_Mouse_Keyboard(int16_t input_type);

// WZD s36p66
/*
    returns field_idx/num
*/
int16_t Get_Input(void);

// WZD s36p67
void Set_Input_Delay(int16_t delay);

// WZD s36p68
int16_t Get_Input_Delay(void);


// WZD s36p85
void Toggle_Pages(void);



#ifdef __cplusplus
}
#endif

#endif  /* INPUT_H */
