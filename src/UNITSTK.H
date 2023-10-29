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


// WZD o61p01
// IDK_ActiveUnitStack_MovesOrPath_s53150()

// WZD o61p02
// AKA Do_Move_Stack()
// ¿PRIVATE?  int16_t Move_Stack(int16_t move_x, int16_t move_y, int16_t player_idx, int16_t * map_x, int16_t * map_y, int16_t * map_p);

// WZD o61p03
// UNIT_MoveStack()
// ¿PRIVATE?  int16_t RdBd_UNIT_MoveStack_STUB(int16_t player_idx, int16_t unit_idx, int16_t destination_world_x, int16_t destination_world_y, int16_t * map_x, int16_t * map_y, int16_t map_p)

// WZD o61p04
// void WIZ_NextIdleStack(int16_t player_idx, int16_t * map_x, int16_t * map_y, int16_t * map_p);

// WZD o61p05
// ¿PRIVATE?  int16_t WIZ_NextUnit(int16_t player_idx, int16_t * map_plane);

// WZD o61p06
// ¿PRIVATE?  
// void STK_GetExtraActions(void);

// WZD o61p07
// drake178: STK_GetMovableUnits()
// ¿PRIVATE?  
// void Active_UnitGroup(int16_t * unit_array_count, int16_t unit_array[]);

// WZD o61p08
// void Build_Moveable_Stack(int16_t * unit_count, int16_t unit_array[]);

// WZD o61p09
// ¿PRIVATE?  void RdBd_UNIT_SelectStack_STUB(int16_t & unit_array_count, int16_t unit_array[], int16_t wx, int16_t wy, int16_t player_idx, int16_t unit_idx);

// WZD o61p10
// WZD o61p11
// WZD o61p12
// WZD o61p13
// WZD o61p14
// WZD o61p15

// WZD o61p16
// ¿PRIVATE?  int16_t Any_Units_Not_Busy();



#ifdef __cplusplus
}
#endif

#endif  /* UNITSTK_H */
