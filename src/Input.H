#ifndef INPUT_H
#define INPUT_H

#include "MoX_TYPE.H"



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */



extern uint8_t g_Key_Pressed;
extern uint16_t g_Last_Key_Pressed;
extern int16_t g_Mouse_X;
extern int16_t g_Mouse_Y;



int16_t Get_Input(void);

int16_t Interpret_Mouse_Input(void);


// MGC s34p65
// WZD s36p65
void Init_Mouse_Keyboard(int16_t input_type);




#ifdef __cplusplus
}
#endif

#endif  /* INPUT_H */
