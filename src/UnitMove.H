/*
    WIZARDS.EXE
    ovr071
*/

#ifndef UNITMOVE_H
#define UNITMOVE_H

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
int16_t Unit_Has_AirTravel(int16_t unit_idx);

// WZD o71p04
// "HasWindWalking", because the manual says that is the name of the "Movement Type"
int16_t Unit_Has_WindWalking(int16_t unit_idx);

// WZD o71p05
/*
    Unit Type - Movement Type - Sailing
    Unit Type - Movement Type - Swimming
    Unit Enchantment - WaterWalking
    returns ST_FALSE/ST_TRUE
*/
int16_t Unit_Has_WaterTravel(int16_t unit_idx);

// WZD o71p06
// int16_t UNIT_IsSailing(int16_t unit_idx);

// WZD o71p07
int16_t Unit_Has_Swimming(int16_t unit_idx);

// WZD o71p08
// int16_t UNIT_ReturnZero(int16_t unit_idx);

// WZD o71p09
int16_t Unit_Has_WaterTravel_Item(int16_t unit_idx);

// WZD o71p010
int16_t Unit_Has_AirTravel_Item(int16_t unit_idx);

// WZD o71p011
int16_t Unit_Has_Invisibility(int16_t unit_idx);

// WZD o71p012
// int16_t UNIT_HasEnduranceUE(int16_t unit_idx);

// WZD o71p013
// drake178: UNIT_HasPlanarItem()
int16_t Unit_Has_PlanarTravel_Item(int16_t unit_idx);

// WZD o71p014
// int16_t UNIT_IsNonCorporeal(int16_t unit_idx);



/*
    WIZARDS.EXE  ovr148
*/

// WZD o148p01
// UU_OVL_Return_2()
// WZD o148p02
// UU_OVL_GetLinePath()

// WZD o148p03
// drake178: STK_GetPath()
int16_t STK_GetPath(int16_t MvMd_00, int16_t MvMd_02, int16_t MvMd_04, int16_t MvMd_06, int16_t MvMd_08, int16_t MvMd_0A, int16_t SrcX, int16_t SrcY, int16_t TgtX, int16_t TgtY, int16_t Plane, uint8_t * RXs, uint8_t * RYs, uint8_t RCs, int16_t UU_bFlag_1, int16_t UU_vFlag_2, int16_t LLCnt, int16_t UCnt, int16_t player_idx);

// WZD o148p04
// TILE_ExtendRange()
// WZD o148p05
// sub_D601B()
// WZD o148p06
// STK_SetOvlMoveMap()
// WZD o148p07
// OVL_ClearUnitPath()
// WZD o148p08
// OVL_StoreLongPath()



#ifdef __cplusplus
}
#endif

#endif  /* UNITMOVE_H */
