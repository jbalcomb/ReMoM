#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "MoX_TYPE.H"

/*
    MoM uses INT 16,10, which returns AH = scan code, AL = ASCII character or 0
    But, it just them as a 16-bit value in AX. (which is what most everyone actually means by "scancode")



    ? complex, but not complicated ?
    e.g.,
        the asterisk ("*") is ASCII code 0x2A
        there is no asterisk key on the keyboard
        but, you can produce one with the 8 key shifted or on the keypad (number pad)
        the 8 key normally produces the scan code 0x0938, but shifted it produces 0x092A
        the keypad * key produces the scan code 0x372A
        in both cases, if you logically AND the scan code with 0x00FF
          you will be left with 0x002A which is the ASCII code for the asterisk

*/


enum EKEYCODES
{
    EKey_Num7_Home  = 0x0047,
    EKey_Num8_Up    = 0x0048,
    EKey_Num9_PgUp  = 0x0049,
    EKey_NumMinus   = 0x004A,
    EKey_Num4_Left  = 0x004B,
    EKey_Num5       = 0x004C,
    EKey_Num6_Right = 0x004D,
    EKey_NumPlus    = 0x004E,
    EKey_Num1_End   = 0x004F,
    EKey_Num2_Down  = 0x0050,
    EKey_Num3_PgDn  = 0x0051,
    EKey_Esc        = 0x011B,
    EKey_NumMult    = 0x092A,
    EKey_Backspace  = 0x0E08,
    EKey_Tab        = 0x0F09,
    EKey_Alt_q      = 0x1000,
    EKey_Alt_w      = 0x1100,
    EKey_Alt_e      = 0x1200,
    EKey_Alt_r      = 0x1300,
    EKey_Alt_t      = 0x1400,
    EKey_Alt_y      = 0x1500,
    EKey_Alt_u      = 0x1600,
    EKey_Alt_i      = 0x1700,
    EKey_Alt_o      = 0x1800,
    EKey_Alt_p      = 0x1900,
    EKey_Enter      = 0x1C0D,
    EKey_Alt_a      = 0x1E00,
    EKey_Alt_s      = 0x1F00,
    EKey_Ctrl_s     = 0x1F13,
    EKey_Alt_d      = 0x2000,
    EKey_Alt_f      = 0x2100,
    EKey_Alt_g      = 0x2200,
    EKey_Alt_h      = 0x2300,
    EKey_Alt_j      = 0x2400,
    EKey_Alt_k      = 0x2500,
    EKey_Alt_l      = 0x2600,
    EKey_Alt_z      = 0x2C00,
    EKey_Alt_x      = 0x2D00,
    EKey_Alt_c      = 0x2E00,
    EKey_Alt_v      = 0x2F00,
    EKey_Alt_b      = 0x3000,
    EKey_Alt_n      = 0x3100,
    EKey_Alt_m      = 0x3200,
    EKey_F1         = 0x3B00,
    EKey_F2         = 0x3C00,
    EKey_F3         = 0x3D00,
    EKey_F4         = 0x3E00,
    EKey_F5         = 0x3F00,
    EKey_F6         = 0x4000,
    EKey_F7         = 0x4100,
    EKey_F8         = 0x4200,
    EKey_F9         = 0x4300,
    EKey_F10        = 0x4400,
    EKey_Home       = 0x47E0,
    EKey_Up         = 0x48E0,
    EKey_PgUp       = 0x49E0,
    EKey_Left       = 0x4BE0,
    EKey_Right      = 0x4DE0,
    EKey_End        = 0x4FE0,
    EKey_Down       = 0x50E0,
    EKey_PgDn       = 0x51E0,
    EKey_Ins        = 0x52E0,
    EKey_Del        = 0x53E0,
    EKey_Ctrl_Num4  = 0x7300,
    EKey_Ctrl_Left  = 0x73E0,
    EKey_Ctrl_Num6  = 0x7400,
    EKey_Ctrl_Right = 0x74E0,
    EKey_Ctrl_Num1  = 0x7500,
    EKey_Ctrl_Num3  = 0x7600,
    EKey_Ctrl_Num7  = 0x7700,
    EKey_Ctrl_Num9  = 0x8400,
    EKey_F11        = 0x8500,
    EKey_F12        = 0x8600,
    EKey_Ctrl_Num8  = 0x8D00,
    EKey_Ctrl_Up    = 0x8DE0,
    EKey_Ctrl_Num2  = 0x9100,
    EKey_Ctrl_Down  = 0x91E0,
    EKey_NumEnter   = 0xE00D
};



// struct Key_Translator
// {
//     uint16_t KBD_Read_Value;    // ; enum EKEYCODES
//     uint16_t Input_Value;       // ; enum Key_Press
// };
// struct Key_Translator Alt_Keycodes[26] = {
//     {0x001E, 0xA000},
//     ...
// };

/*
Read_Key()

@@AltXlat
mov     bx, offset Alt_Keycodes
mov     dx, 104                             ; 26 * 4
add     dx, 4                               ; sizeof(struct Key_Translator)
@@CheckKeyCode:
mov     cx, [cs:bx+Key_Translator.KBD_Read_Value]
cmp     ax, cx
jz      short @@Translate
add     bx, 4                               ; sizeof(struct Key_Translator)
cmp     bx, dx
jz      short @@Done
jmp     short @@CheckKeyCode
@@Translate:
inc     bx
inc     bx
mov     ax, [cs:bx]
*/

// DATA XREF: Read_Key
// seg035:0004
// Alt_Keycodes
// Key_Translator <EKey_Alt_a, KP_Alt_a>                           ;  0
// Key_Translator <EKey_Alt_b, KP_Alt_b>                           ;  1
// Key_Translator <EKey_Alt_c, KP_Alt_c>                           ;  2
// Key_Translator <EKey_Alt_d, KP_Alt_d>                           ;  3
// Key_Translator <EKey_Alt_e, KP_Alt_e>                           ;  4
// Key_Translator <EKey_Alt_f, KP_Alt_f>                           ;  5
// Key_Translator <EKey_Alt_g, KP_Alt_g>                           ;  6
// Key_Translator <EKey_Alt_h, KP_Alt_h>                           ;  7
// Key_Translator <EKey_Alt_i, KP_Alt_i>                           ;  8
// Key_Translator <EKey_Alt_j, KP_Alt_j>                           ;  9
// Key_Translator <EKey_Alt_k, KP_Alt_k>                           ; 10
// Key_Translator <EKey_Alt_l, KP_Alt_l>                           ; 11
// Key_Translator <EKey_Alt_m, KP_Alt_m>                           ; 12
// Key_Translator <EKey_Alt_n, KP_Alt_n>                           ; 13
// Key_Translator <EKey_Alt_o, KP_Alt_o>                           ; 14
// Key_Translator <EKey_Alt_p, KP_Alt_p>                           ; 15
// Key_Translator <EKey_Alt_q, KP_Alt_q>                           ; 16
// Key_Translator <EKey_Alt_r, KP_Alt_r>                           ; 17
// Key_Translator <EKey_Alt_s, KP_Alt_s>                           ; 18
// Key_Translator <EKey_Alt_t, KP_Alt_t>                           ; 19
// Key_Translator <EKey_Alt_u, KP_Alt_u>                           ; 20
// Key_Translator <EKey_Alt_v, KP_Alt_v>                           ; 21
// Key_Translator <EKey_Alt_w, KP_Alt_w>                           ; 22
// Key_Translator <EKey_Alt_x, KP_Alt_x>                           ; 23
// Key_Translator <EKey_Alt_y, KP_Alt_y>                           ; 24
// Key_Translator <EKey_Alt_z, KP_Alt_z>                           ; 25

// 00 1E A0 00 00 30 A1 00 00 2E A2 00 00 20 A3 00 00 12 A4 00 00 21 A5 00 00 22 A6 00 00 23 A7 00 00 17 A8 00 00 24 A9 00 00 25 AA 00 00 26 AB 00 00 32 AC 00 00 31 AD 00 00 18 AE 00 00 19 AF 00 00 10 B0 00 00 13 B1 00 00 1F B2 00 00 14 B3 00 00 16 B4 00 00 2F B5 00 00 11 B6 00 00 2D B7 00 00 15 B8 00 00 2C B9 00

// 00 1E A0 00 
// 00 30 A1 00 
// 00 2E A2 00 
// 00 20 A3 00 
// 00 12 A4 00 
// 00 21 A5 00 
// 00 22 A6 00 
// 00 23 A7 00 
// 00 17 A8 00 
// 00 24 A9 00 
// 00 25 AA 00 
// 00 26 AB 00 
// 00 32 AC 00 
// 00 31 AD 00 
// 00 18 AE 00 
// 00 19 AF 00 
// 00 10 B0 00 
// 00 13 B1 00 
// 00 1F B2 00 
// 00 14 B3 00 
// 00 16 B4 00 
// 00 2F B5 00 
// 00 11 B6 00 
// 00 2D B7 00 
// 00 15 B8 00 
// 00 2C B9 00



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


/*
    WIZARDS.EXE  seg035
    MAGIC.EXE  seg033
*/

// WZD s35p14
// MGC s33p14
int16_t Keyboard_Status(void);

// WZD s35p15
// MGC s33p15
uint8_t Read_Key(void);


/*
    WIZARDS.EXE  seg036
    MAGIC.EXE  seg034
*/

// WZD s36p22
// MGC s34p22
uint8_t Interpret_Keyboard_Input(int16_t * field_num);


#ifdef __cplusplus
}
#endif

#endif /* KEYBOARD_H */
