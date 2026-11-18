/*
    WIZARDS.EXE
        ovr132
*/

#ifndef SPELLS132_H
#define SPELLS132_H

#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_TYPE.h"



#ifdef __cplusplus
extern "C" {
#endif




/*
    WIZARDS.EXE  ovr132
*/

// WZD o132p01
void Cast_Summon_Hero(int16_t player_idx, int16_t type);

// WZD o132p02
void Cast_Incarnation(int16_t player_idx);

// WZD o132p03
void Apply_Chaos_Rift(int16_t city_idx);

// WZD o132p04
void Apply_Meteor_Storm(int16_t player_idx);

// WZD o132p05
void Apply_Stream_Of_Life(int16_t city_idx);

// WZD o132p06
int16_t Hire_Hero(int16_t player_idx, int16_t unit_type_idx, int16_t hero_slot_idx, int16_t saved_flag);

// WZD o132p07
void Cast_Resurrection(int16_t player_idx);

// WZD o132p08
int16_t Cast_Plane_Shift_Check(int16_t wx, int16_t wy, int16_t wp);

// WZD o132p09
void Cast_Plane_Shift_Draw(int16_t wx, int16_t wy, int16_t wp);

// WZD o132p10
int16_t Cast_Plane_Shift(int16_t player_idx);

// WZD o132p11
void Apply_Great_Wasting(int16_t player_idx);

// WZD o132p12
void Apply_Gaias_Blessing(int16_t city_idx);

// WZD o132p13
void Apply_Armageddon(int16_t player_idx);

// WZD o132p14
void Cast_Floating_Island(int16_t player_idx);

// WZD o132p15
void Apply_Nightshade_Dispel(int16_t city_idx);

// WZD o132p16
int16_t Cast_Lycantrophy(int16_t player_idx);

// WZD o132p17
int16_t Cast_Wall_Of_Stone(int16_t player_idx);

// WZD o132p18
int16_t Cast_Spell_Of_Return(int16_t player_idx);



#ifdef __cplusplus
}
#endif


#endif  /* SPELLS132_H */
