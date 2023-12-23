/*
    WIZARDS.EXE
        ovr059
*/

#ifndef WZD_O059_H
#define WZD_O059_H

#include "MoM.H"


#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr059
*/

// WZD o59p01
// drake178: WIZ_GetCapital()
// WIZ_GetCapital()

// WZD o59p02
// drake178: WIZ_GetSummonTo()
// WIZ_GetSummonTo()

// WZD o59p03
// drake178: CTY_GetGarrison()
void Get_Units_City(int16_t city_idx, int16_t * unit_count, int16_t unit_array[]);

// WZD o59p04
// drake178: TILE_GetUnitStack()
// TILE_GetUnitStack()

// WZD o59p05
// drake178: TILE_GetEnemyStack()
void Get_Units_Other(int16_t wx, int16_t wy, int16_t wp, int16_t player_idx, int16_t * unit_count, int16_t unit_array[]);

// WZD o59p06
// drake178: TILE_GetUnits_2()
// TILE_GetUnits_2()

// WZD o59p07
// drake178: CRP_TILE_GetUnits_2()
// CRP_TILE_GetUnits_2()

// WZD o59p08
// drake178: sub_51F82()
// sub_51F82()

// WZD o59p09
// drake178: STK_SettlingPossible()
// STK_SettlingPossible()

// WZD o59p10
// drake178: fxnptr_o59p10()
// fxnptr_o59p10()

// WZD o59p11
// drake178: sub_520DD()
// sub_520DD()

// WZD o59p12
// drake178: TILE_HasNode()
// TILE_HasNode()

// WZD o59p13
// drake178: STK_CheckTower()
// ¿PRIVATE?  void Units_In_Tower(int16_t unit_array_count, int16_t unit_array[], int16_t map_p);

// WZD o59p14
// drake178: GAME_InTowerRefresh()
// GAME_InTowerRefresh()

// WZD o59p15
// drake178: UNIT_Dismiss()
// UNIT_Dismiss()

// WZD o59p16
// drake178: IDK_MagicScreen_RVL()
// IDK_MagicScreen_RVL()

// WZD o59p17
// drake178: sub_5245A()
// sub_5245A()

// WZD o59p18
// drake178: TILE_ExploreRadius()
// TILE_ExploreRadius()

// WZD o59p19
// AKA IDK_MaybeSwitchStackPlane_s52514()
void Do_Plane_Button(int16_t player_idx, int16_t * map_x, int16_t * map_y, int16_t * map_plane);

// WZD o59p20
// AKA IDK_PlanarTravel_TestWaterCity_s52774()
int16_t Check_Stack_Plane_Shift(int16_t unit_idx, int16_t map_plane);



#ifdef __cplusplus
}
#endif



#endif /* WZD_O059_H */
