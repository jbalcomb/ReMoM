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

// 00000001 ; enum MOVEFLAGS (bitfield)
// 00000001 M_Cavalry  = 1
// 00000002 M_Sailing  = 2
// 00000004 M_Swimming  = 4
// 00000008 M_Flying  = 8
// 00000010 M_Teleport  = 10h
// 00000020 M_Forester  = 20h
// 00000040 M_Mntnr  = 40h
// 00000080 M_Merging  = 80h


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
/*
    Unit Type - Movement Type - Flying
    Unit Enchantment - WindWalking
    Unit Enchantment - Flying
    NOT Unit Ability - WindWalking
    Unit Mutation - Chaos Channels - Demon Wings
    returns ST_FALSE/ST_TRUE
*/
int16_t UNIT_HasAirTravel(int16_t unit_idx);

// WZD o71p04
// "HasWindWalking", because the manual says that is the name of the "Movement Type"
int16_t UNIT_HasWindWalking(int16_t unit_idx);

// WZD o71p05
/*
    Unit Type - Movement Type - Sailing
    Unit Type - Movement Type - Swimming
    Unit Enchantment - WaterWalking
    returns ST_FALSE/ST_TRUE
*/
int16_t UNIT_HasWaterTravel(int16_t unit_idx);

// WZD o71p06
// int16_t UNIT_IsSailing(int16_t unit_idx);
// WZD o71p07
// int16_t UNIT_IsSwimming(int16_t unit_idx);
// WZD o71p08
// int16_t UNIT_ReturnZero(int16_t unit_idx);

// WZD o71p09
int16_t UNIT_HasWaterTravelItem(int16_t unit_idx);

// WZD o71p010
int16_t UNIT_HasAirTravelItem(int16_t unit_idx);

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
