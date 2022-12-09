#ifndef INPUT_H
#define INPUT_H

#include "MoX_TYPE.H"



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */



extern uint8_t g_Key_Pressed;
extern uint16_t g_Last_Key_Pressed;


int16_t Get_Input(void);

int16_t Process_Input(void);




#ifdef __cplusplus
}
#endif

#endif /* INPUT_H */
