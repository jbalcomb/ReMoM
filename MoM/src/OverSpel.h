/*
    ¿ Overland Spell Casting ?

    WIZARDS.EXE
        ovr135
*/

#ifndef OVERSPEL_H
#define OVERSPEL_H

#include "../../MoX/src/MOX_TYPE.h"



#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr135
*/

// WZD o135p01
int16_t Get_Spell_For_City_Enchantment(int16_t city_enchantment_idx);

// WZD o135p02
int16_t Calculate_Dispel_Difficulty(int16_t casting_cost, int16_t player_idx, int16_t magic_realm);

// WZD o135p03
void Cast_Disenchant(int16_t wx, int16_t wy, int16_t wp, int16_t player_idx, int16_t strength);

// WZD o135p04
int16_t Square_Has_Disenchant_Target(int16_t wx, int16_t wy, int16_t wp);

// WZD o135p05
void Cast_Spell_Overland__WIP(int16_t player_idx);

// WZD o135p06
int16_t IDK_Get_Global_Enchant_Index__STUB(int16_t value);



#ifdef __cplusplus
}
#endif


#endif  /* OVERSPEL_H */
