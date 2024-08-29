/*
    WIZARDS.EXE
        ovr061
*/

#ifndef UNITSTK_H
#define UNITSTK_H

#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr061
*/

// WZD o61p01
// AKA IDK_ActiveUnitStack_MovesOrPath_s53150()
void Update_Stack_Active(int16_t stack_idx);

// WZD o61p02
// AKA Do_Move_Stack()
int16_t Move_Stack(int16_t move_x, int16_t move_y, int16_t player_idx, int16_t * map_x, int16_t * map_y, int16_t * map_p);

// WZD o61p03
int16_t RdBd_UNIT_MoveStack__WIP(int16_t player_idx, int16_t unit_idx, int16_t destination_world_x, int16_t destination_world_y, int16_t * map_x, int16_t * map_y, int16_t map_p);

// WZD o61p04
void WIZ_NextIdleStack(int16_t player_idx, int16_t * map_x, int16_t * map_y, int16_t * map_p);

// WZD o61p05
int16_t Next_Unit_Nearest_Available(int16_t player_idx, int16_t * map_plane);

// WZD o61p06
// AKA STK_GetExtraActions();
void Set_Unit_Action_Special(void);

// WZD o61p07
// drake178: STK_GetMovableUnits()
void Active_Unit_Stack(int16_t * unit_array_count, int16_t unit_array[]);

// WZD o61p08
void Build_Moveable_Stack(int16_t * unit_count, int16_t unit_array[]);

// WZD o61p09
void Build_RoadBuilder_Stack(int16_t * troop_count, int16_t troops[], int16_t dst_wx, int16_t dst_wy, int16_t player_idx, int16_t unit_idx);

// WZD o61p10
int16_t Unit_Action_Special_Build(int16_t troop_count, int16_t troops[]);

// WZD o61p11
int16_t Unit_Action_Special_Purify(int16_t troop_count, int16_t troops[]);

// WZD o61p12
int16_t Unit_Action_Special_Meld(int16_t troop_count, int16_t troops[]);

// WZD o61p13
void STK_MeldWithNode(void);

// WZD o61p14
void STK_DoMeldWithNode(int16_t troop_count, int16_t troops[]);

// WZD o61p15
void Next_Turn_Process_Purify(void);

// WZD o61p16
// ¿PRIVATE?  int16_t Any_Units_Not_Busy();



#ifdef __cplusplus
}
#endif

#endif  /* UNITSTK_H */
