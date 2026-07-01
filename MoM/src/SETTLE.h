/*
    WIZARDS.EXE
        ovr100

    Moo2
        Module: COLONIZE

*/

#ifndef SETTLE_H
#define SETTLE_H

#include "../../MoX/src/MOX_TYPE.h"



#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE ovr100
*/

// WZD o100p01
void AI_Execute_Orders(int16_t player_idx);

// WZD o100p02
void AI_Unit_Army_Do_Meld(int16_t unit_idx);

// WZD o100p03
void AI_Unit_Army_Do_Settle(int16_t unit_idx);

// WZD o100p04
int16_t Active_Army_Do_Settle(void);

// WZD o100p05
int16_t Army_Do_Settle(int16_t troop_count, int16_t troops[]);

// WZD o100p06
int16_t AI_Unit_Army_Do_Move(int16_t unit_idx);

// WZD o100p07
// sub_8227A()

// WZD o100p08
// sub_82377()

// WZD o100p09
void Evict_Unit(int16_t unit_idx);

// WZD o100p10
int16_t Unit_Space_At_Square(int16_t wx, int16_t wy, int16_t wp, int16_t player_idx, int16_t unit_idx);

// WZD o100p11
void AI_Unit_Army_Do_Ferry(uint16_t unit_idx);

// WZD o100p12
void AI_Unit_Army_Do_Road(int16_t unit_idx);

// WZD o100p13
int16_t Map_Square_Troops_Can_Plane_Shift(int16_t wx, int16_t wy, int16_t wp);



#ifdef __cplusplus
}
#endif

#endif  /* SETTLE_H */
