/*
    City Calculations

    WIZARDS.EXE
        ovr120
*/

#ifndef CITYCALC_H
#define CITYCALC_H

#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr120
*/

// WZD o120p01
// CTY_Recalculate()

// WZD o120p02

// WZD o120p03
int16_t UNIT_GetGoldUpkeep(int16_t unit_idx);

// WZD o120p04
int16_t WIZ_ArmyUpkeep_Gold(int16_t player_idx);

// WZD o120p05
int16_t WIZ_ArmyUpkeep_Food(int16_t player_idx);

// WZD o120p06

// WZD o120p07
void Get_Incomes(int16_t player_idx, int16_t * gold, int16_t * food, int16_t * mana);

// WZD o120p08
int16_t WIZ_TotalUpkeep_Mana(int16_t player_idx);

// WZD o120p09
int16_t UNIT_GetManaUpkeep(int16_t unit_idx);

// WZD o120p10
int16_t CTY_EnchantsUpkeep(int16_t city_idx, int16_t player_idx);

// WZD o120p11
int16_t WIZ_GlobalsUpkeep(int16_t player_idx);

// WZD o120p12
// WZD o120p13
// WZD o120p14
// WZD o120p15
// WZD o120p16

// WZD o120p17
void Get_Power_Incomes_Base(int16_t * Base_Research_Income, int16_t * Base_Skill_Income, int16_t * Base_Mana_Income, int16_t player_idx);

// WZD o120p18
void Get_Power_Incomes(int16_t * Mana, int16_t * City_Food_Surplus, int16_t * City_Gold_Balance, int16_t player_idx);

// WZD o120p19
int16_t Get_Research_Bonus(int16_t player_idx, int16_t spell_idx);

// WZD o120p20
int16_t WIZ_GetFame(int16_t player_idx);

// WZD o120p21
// WZD o120p22
// WZD o120p23
// WZD o120p24
// WZD o120p25
// WZD o120p26



#ifdef __cplusplus
}
#endif

#endif  /* CITYCALC_H */
