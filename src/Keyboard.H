#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "MoX_TYPE.H"



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


/*
    MGC  seg034
*/

// MGC s33p14
int16_t Keyboard_Status(void);

// MGC s33p15
int16_t Read_Key(void);

// MGC s34p22
int16_t Interpret_Keyboard_Input(int16_t * field_num);


#ifdef __cplusplus
}
#endif

#endif /* KEYBOARD_H */
