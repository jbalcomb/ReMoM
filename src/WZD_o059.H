/*
    WIZARDS.EXE
        ovr059
*/

#ifndef WZD_O059_H
#define WZD_O059_H

#include "MOM.H"


#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr059
*/

// WZD o59p01
int16_t Player_Fortress_City(int16_t player_idx);

// WZD o59p02
int16_t Player_Summon_City(int16_t player_idx);

// WZD o59p03
void Army_At_City(int16_t city_idx, int16_t * troop_count, int16_t troops[]);

// WZD o59p04
void Player_Army_At_Square(int16_t wx, int16_t wy, int16_t wp, int16_t player_idx, int16_t * troop_count, int16_t troops[]);

// WZD o59p05
void Enemy_Army_At_Square(int16_t wx, int16_t wy, int16_t wp, int16_t player_idx, int16_t * troop_count, int16_t troops[]);

// WZD o59p06
// drake178: TILE_GetUnits_2()
void Army_At_Square_2(int16_t wx, int16_t wy, int16_t wp, int16_t * troop_count, int16_t troops[]);

// WZD o59p07
void Army_At_Square_3(int16_t wx, int16_t wy, int16_t wp, int16_t * troop_count, int16_t troops[]);

// WZD o59p08
void UU_Army_At_Square_4(int16_t wx, int16_t wy, int16_t wp, int16_t * troop_count, int16_t troops[]);

// WZD o59p09
int16_t Unit_Action_Special_Settle(int16_t troop_count, int16_t troops[]);

// WZD o59p10
void o59p10_empty_function(void);

// WZD o59p11
// sub_520DD()

// WZD o59p12
int16_t TILE_HasNode(int16_t wx, int16_t wy, int16_t wp);

// WZD o59p13
void Units_In_Tower(int16_t troop_count, int16_t troops[], int16_t map_p);

// WZD o59p14
void All_Units_In_Towers(void);

// WZD o59p15
// drake178: UNIT_Dismiss()
void Dismiss_Unit(int16_t unit_idx);

// WZD o59p16
void Cheat_Reveal(void);

// WZD o59p17
// sub_5245A()

// WZD o59p18
void TILE_ExploreRadius__WIP(int16_t wx, int16_t wy, int16_t wp, int16_t scout_range);

// WZD o59p19
void Do_Plane_Button__WIP(int16_t player_idx, int16_t * map_x, int16_t * map_y, int16_t * map_plane);

// WZD o59p20
int16_t Check_Stack_Plane_Shift(int16_t unit_idx, int16_t map_plane);



#ifdef __cplusplus
}
#endif



#endif /* WZD_O059_H */
