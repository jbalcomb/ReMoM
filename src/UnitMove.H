#ifndef UNITMOVE_H
#define UNITMOVE_H

/*
    WIZARDS.EXE
    ovr071
*/

#include "MoX.H"


// ... in MoM_DEF.H  :shrug:
// 00000000 struc s_Movement_Modes ; (sizeof=0x12)
// 00000000 mm_UU_Cavalry dw ?
// 00000002 mm_Forester dw ?
// 00000004 mm_Mountaineer dw ?
// 00000006 mm_Swimming dw ?
// 00000008 mm_Sailing dw ?
// 0000000A mm_Flying dw ?
// 0000000C mm_PlanarTravel dw ?
// 0000000E mm_WindWalking dw ?
// 00000010 mm_Walking dw ?
// 00000012 ends s_Movement_Modes



#ifdef __cplusplus
extern "C" {
#endif


/*
    WIZARDS.EXE  ovr071
*/


// WZD o71p01
// drake178: OVL_GetMoveTypes()
// void Active_Stack_Movement_Modes(struct s_Movement_Modes * movement_mode_flags);
void Active_Stack_Movement_Modes(int16_t movement_mode_flags[]);

// WZD o71p02
// drake178: STK_GetMoveTypes()
// void Stack_Movement_Modes(struct s_Movement_Modes * movement_mode_flags, int16_t * stack_array, int16_t stack_size);
void Stack_Movement_Modes(int16_t movement_mode_flags[], int16_t * stack_array, int16_t stack_size);

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
// drake178: UNIT_HasPlanarItem()
int16_t Unit_Has_Planar_Travel_Item(int16_t unit_idx);

// WZD o71p014
// int16_t UNIT_IsNonCorporeal(int16_t unit_idx);



#ifdef __cplusplus
}
#endif


#endif  /* UNITMOVE_H */
