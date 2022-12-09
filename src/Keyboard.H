#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "MoX_TYPE.H"



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


int16_t Keyboard_Status(void);

int16_t Interpret_Keyboard_Input(field_num);



#ifdef __cplusplus
}
#endif

#endif /* KEYBOARD_H */
