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
// WZD o71p03 UNIT_IsFlying      ();

// WZD o71p04
// AKA UNIT_IsWindWalker()
int16_t UNIT_HasWindWalking(int16_t unit_idx);

// WZD o71p05 UNIT_IsOceanFaring ();
// WZD o71p06 UNIT_IsSailing     ();
// WZD o71p07 UNIT_IsSwimming    ();
// WZD o71p08 UNIT_ReturnZero    ();
// WZD o71p09 UNIT_HasItemSwim   ();
// WZD o71p010 UNIT_HasItemFlight ();
// WZD o71p011 UNIT_IsInvisible   ();
// WZD o71p012 UNIT_HasEnduranceUE();
// WZD o71p013 UNIT_HasPlanarItem ();
// WZD o71p014 UNIT_IsNonCorporeal();



#ifdef __cplusplus
}
#endif


#endif  /* UNITMOVE_H */
