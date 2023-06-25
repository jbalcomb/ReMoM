#ifndef UNITMOVE_H
#define UNITMOVE_H

/*
    WIZARDS.EXE
    ovr071
*/

#include "MoX_TYPE.H"



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


/*
    WIZARDS.EXE  ovr071
*/

// WZD o71p01 OVL_GetMoveTypes   ();
// WZD o71p02 STK_GetMoveTypes   ();

// WZD o71p03
// int16_t UNIT_IsFlying(int16_t unit_idx);

// WZD o71p04
// AKA UNIT_IsWindWalker()
int16_t UNIT_HasWindWalking(int16_t unit_idx);

// WZD o71p05
// int16_t UNIT_IsOceanFaring(int16_t unit_idx);
// WZD o71p06
// int16_t UNIT_IsSailing(int16_t unit_idx);
// WZD o71p07
// int16_t UNIT_IsSwimming(int16_t unit_idx);
// WZD o71p08
// int16_t UNIT_ReturnZero(int16_t unit_idx);
// WZD o71p09
// int16_t UNIT_HasItemSwim(int16_t unit_idx);
// WZD o71p010
// int16_t UNIT_HasItemFlight(int16_t unit_idx);

// WZD o71p011
int16_t UNIT_HasInvisibility(int16_t unit_idx);

// WZD o71p012
// int16_t UNIT_HasEnduranceUE(int16_t unit_idx);
// WZD o71p013
// int16_t UNIT_HasPlanarItem (int16_t unit_idx);
// WZD o71p014
// int16_t UNIT_IsNonCorporeal(int16_t unit_idx);



#ifdef __cplusplus
}
#endif


#endif  /* UNITMOVE_H */
