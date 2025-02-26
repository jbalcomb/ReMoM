/*
    ¿ Overland Spell Casting ?

    WIZARDS.EXE
        ovr135
*/

#ifndef OVERSPEL_H
#define OVERSPEL_H

#include "MOM.H"

#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr135
*/

// WZD o135p01
// SPL_Get_CityE_Index()

// WZD o135p02
int16_t Calculate_Dispel_Difficulty(int16_t casting_cost, int16_t player_idx, int16_t magic_realm);

// WZD o135p03
// sub_BA3A4()

// WZD o135p04
// sub_BAB5D()

// WZD o135p05
void Cast_Spell_Overland__WIP(int16_t player_idx);

// WZD o135p06
// sub_BCB9E()


#ifdef __cplusplus
}
#endif


#endif  /* OVERSPEL_H */
