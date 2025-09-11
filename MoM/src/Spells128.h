/*
    WIZARDS.EXE
        ovr128
*/

#ifndef SPELLS128_H
#define SPELLS128_H

#include "../../MoX/src/MOX_TYPE.h"



#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr128
*/

// WZD o128p01
int16_t Player_Research_Spells(int16_t player_idx);

// WZD o128p02
void Build_Research_List(int16_t player_idx, int16_t research_list[]);

// WZD o128p03
int16_t WIZ_RollSpellReward(int16_t player_idx, int16_t rarity);

// WZD o128p04
int16_t Calc_Spell_Value(int16_t player_idx, int16_t spell_idx);

// WZD o128p05
int16_t Get_Differential_Spell_List(int16_t player1, int16_t player2, int16_t min_value, uint8_t spell_list[]);

// WZD o128p06
void Change_Relations_For_Bad_City_Spell(int16_t player_idx, int16_t spell_idx, int16_t city_idx);

// WZD o128p07
void Change_Relations_For_Enchantments(int16_t player_idx, int16_t spell_idx, int16_t divisor);

// WZD o128p08
void Sort_Research_List(int16_t player_idx, int16_t count);

// WZD o128p09
void WIZ_AddSpellRank__WIP(int16_t player_idx, int16_t magic_realm);

// WZD o128p10
// sub_ACACC()

// WZD o128p11
// WIZ_ConquestSpells()



#ifdef __cplusplus
}
#endif



#endif /* SPELLS128_H */
