#ifndef INPUT_H
#define INPUT_H

#include "MoX_TYPE.H"


enum Key_Press
{
    KP_NumMult         = 0x00,
    KP_Left            = 0x01,
    KP_Right           = 0x02,
    KP_Up              = 0x03,
    KP_Down            = 0x04,
    KP_RightUp         = 0x05,
    KP_RightDown       = 0x06,
    KP_LeftUp          = 0x07,
    KP_LeftDown        = 0x08,
    KP_Insert          = 0x09,
    KP_Delete          = 0x0A,
    KP_Backspace       = 0x0B,
    KP_Enter           = 0x0C,
    KP_Tab             = 0x0D,
    KP_F1              = 0x0E,
    KP_F2              = 0x0F,
    KP_F3              = 0x10,
    KP_F4              = 0x11,
    KP_F5              = 0x12,
    KP_F6              = 0x13,
    KP_F7              = 0x14,
    KP_F8              = 0x15,
    KP_F9              = 0x16,
    KP_F10             = 0x17,
    KP_PgUp            = 0x19,
    KP_Home            = 0x1A,
    KP_Esc             = 0x1B,
    KP_End             = 0x1C,
    KP_PgDn            = 0x1D,
    KP_Center          = 0x1E,
    KP_NumPlus         = 0x2B,
    KP_NumMinus        = 0x2D,
    KP_Ctrl_Right      = 0x80,
    KP_Ctrl_Left       = 0x81,
    KP_Ctrl_Up         = 0x82,
    KP_Ctrl_Down       = 0x83,
    KP_Ctrl_LeftUp     = 0x84,
    KP_Ctrl_RightUp    = 0x85,
    KP_Ctrl_LeftDown   = 0x86,
    KP_Ctrl_RightDown  = 0x87,
    KP_Ctrl_s          = 0x90,
    KP_F11             = 0x91,
    KP_F12             = 0x92,
    KP_Alt_a           = 0xA0,
    KP_Alt_b           = 0xA1,
    KP_Alt_c           = 0xA2,
    KP_Alt_d           = 0xA3,
    KP_Alt_e           = 0xA4,
    KP_Alt_f           = 0xA5,
    KP_Alt_g           = 0xA6,
    KP_Alt_h           = 0xA7,
    KP_Alt_i           = 0xA8,
    KP_Alt_j           = 0xA9,
    KP_Alt_k           = 0xAA,
    KP_Alt_l           = 0xAB,
    KP_Alt_m           = 0xAC,
    KP_Alt_n           = 0xAD,
    KP_Alt_o           = 0xAE,
    KP_Alt_p           = 0xAF,
    KP_Alt_q           = 0xB0,
    KP_Alt_r           = 0xB1,
    KP_Alt_s           = 0xB2,
    KP_Alt_t           = 0xB3,
    KP_Alt_u           = 0xB4,
    KP_Alt_v           = 0xB5,
    KP_Alt_w           = 0xB6,
    KP_Alt_x           = 0xB7,
    KP_Alt_y           = 0xB8,
    KP_Alt_z           = 0xB9,
};

#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */



extern uint8_t g_Key_Pressed;
extern uint16_t g_Last_Key_Pressed;
extern int16_t g_Mouse_X;
extern int16_t g_Mouse_Y;




// WZD s36p01
// MGC s34p01
int16_t Interpret_Mouse_Input(void);

// WZD s36p65  AKA GUI_Init()
// MGC s34p65  AKA IN_Init()
void Init_Mouse_Keyboard(int16_t input_type);

// WZD s36p66
// MGC s34p66
int16_t Get_Input(void);

// MGC s34p67
// Set_Input_Delay()





#ifdef __cplusplus
}
#endif

#endif  /* INPUT_H */
