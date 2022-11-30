#ifndef MOX_LBX_H
#define MOX_LBX_H

#include "MoX_TYPE.H"


/*
    Unitialized Data
*/
extern uint16_t MoX_RAM_Min_KB;                    // MGC dseg:A5C4  ; set to 583 in _main



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


/*
    seg010
*/

// s10p17
void MoX_RAM_SetMinKB(int16_t RAM_MinKB);


#ifdef __cplusplus
}
#endif

#endif /* MOX_LBX_H */
