/*
    WIZARDS.EXE
        ovr129
*/

#ifndef SPELLS129_H
#define SPELLS129_H

#include "MOX/MOX_TYPE.H"



#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr129
*/

// WZD o129p01
int16_t Apply_Cruel_Unminding(int16_t player_idx);

// WZD o129p02
void Cast_Attack_Spell_On_Enemy_Stack(int16_t unit_idx, int16_t spell_idx);

// WZD o129p03
void Cast_Attack_Spell_On_Enemy_Unit(int16_t unit_idx, int16_t spell_idx, int16_t * item_count, int16_t item_list[], int16_t attack_override_flag);

// WZD o129p04
// sub_AD1F0()

// WZD o129p05
int16_t Cast_Call_The_Void(int16_t player_idx);

// WZD o129p06
int16_t Apply_Call_The_Void(int16_t city_idx);

// WZD o129p07
int16_t Apply_Automatic_Spell_Counters(int16_t spell_idx, int16_t city_idx, int16_t player_idx, int16_t show_message);

// WZD o129p08
void UNIT_ChaosChannel__STUB(int16_t unit_idx);

// WZD o129p09
int16_t Cast_ChaosChannels(int16_t player_idx);

// WZD o129p10
int16_t Cast_WarpNode(int16_t player_idx);

// WZD o129p11
int16_t Cast_Stasis(int16_t player_idx);

// WZD o129p12
int16_t Cast_NaturesCures(int16_t player_idx);

// WZD o129p13
int16_t Cast_Move_Fortress(int16_t player_idx);

// WZD o129p14
int16_t Cast_Earthquake(int16_t player_idx);

// WZD o129p15
int16_t CTY_Earthquake__STUB(int16_t city_idx, int16_t * list_count, int16_t list_array[]);

// WZD o129p16
// sub_AEDB1()



#ifdef __cplusplus
}
#endif



#endif /* SPELLS129_H */
