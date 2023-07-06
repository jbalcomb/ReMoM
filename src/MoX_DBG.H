#ifndef MOX_DBG_H
#define MOX_DBG_H

#include "MoX_TYPE.H"

extern uint8_t DBG_debug_flag;



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

/*
    WZD  seg024
    MGC  seg024
*/



void Main_Screen_Draw_Debug_Information(void);



// WZD s24p05
void Release_Version(void);

// WZD s24p06
int Check_Release_Version(void);


#ifdef __cplusplus
}
#endif

#endif /* MOX_DBG_H */
