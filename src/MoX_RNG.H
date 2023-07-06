/*
    WIZARDS.EXE
    seg022
*/
#ifndef MOX_RNG_H
#define MOX_RNG_H

#include "MoX_TYPE.H"


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


/*
    WZD  seg022
*/

// WZD s22p05
void Set_Random_Seed(uint32_t n);

// WZD s22p06
uint32_t Get_Random_Seed(void);

// WZD s22p07
void Randomize(void);

// WZD s22p08
int16_t Random(int16_t max);


#ifdef __cplusplus
}
#endif



#endif /* MOX_RNG_H */
