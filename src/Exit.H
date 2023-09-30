/*
    WIZARDS.EXE
        seg005
    
    MoO2:
        Module: exit
*/

#ifndef EXIT_H
#define EXIT_H

#include "MoX.H"

#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  seg005
*/

// WZD s05p01
void Exit_With_Size(void);

// WZD s05p02
void Exit_With_Message(char * string);

// WZD s05p03
void Exit_With_Value(int16_t value);

#ifdef __cplusplus
}
#endif

#endif  /* EXIT_H */
