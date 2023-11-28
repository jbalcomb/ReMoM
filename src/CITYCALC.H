/*
    City Calculations

    WIZARDS.EXE
        ovr120
        ovr142
*/

#ifndef CITYCALC_H
#define CITYCALC_H

#include "MoX.H"

#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr120
*/

// WZD o120p01
// drake178: CTY_Recalculate()
void Do_City_Calculations(int16_t city_idx);

// WZD o120p02
// drake178: WIZ_SetPowerBases()
void Players_Update_Magic_Power(void);

// WZD o120p03
int16_t Unit_Gold_Upkeep(int16_t unit_idx);

// WZD o120p04
int16_t Player_Armies_Gold_Upkeep(int16_t player_idx);

// WZD o120p05
int16_t Player_Armies_Food_Upkeep(int16_t player_idx);

// WZD o120p06
// drake178: WIZ_GetNUCounts()
void Players_Normal_Units(int16_t normal_units[]);

// WZD o120p07
void Player_Resource_Income_Total(int16_t player_idx, int16_t * gold, int16_t * food, int16_t * mana);

// WZD o120p08
int16_t Player_Armies_And_Enchantments_Mana_Upkeep(int16_t player_idx);

// WZD o120p09
int16_t Unit_Mana_Upkeep(int16_t unit_idx);

// WZD o120p10
int16_t Player_City_Enchantments_Upkeep(int16_t city_idx, int16_t player_idx);

// WZD o120p11
int16_t Player_Overland_Enchantments_Upkeep(int16_t player_idx);

// WZD o120p12
// WZD o120p13
// WZD o120p14
// WZD o120p15
// WZD o120p16

// WZD o120p17
void Player_Magic_Power_Distribution(int16_t * Base_Research_Income, int16_t * Base_Skill_Income, int16_t * Base_Mana_Income, int16_t player_idx);

// WZD o120p18
void Player_Magic_Power_Income_Total(int16_t * Mana, int16_t * City_Food_Surplus, int16_t * City_Gold_Balance, int16_t player_idx);

// WZD o120p19
int16_t Player_Spell_Research_Bonus(int16_t player_idx, int16_t spell_idx);

// WZD o120p20
int16_t Player_Fame(int16_t player_idx);

// WZD o120p21

// WZD o120p22
// drake178: CTY_GetGoldUpkeep()
int16_t City_Gold_Mainanence(int16_t city_idx);

// WZD o120p23
// WZD o120p24
// WZD o120p25
// WZD o120p26



/*
    WIZARDS.EXE  ovr120
*/

// WZD s142p01
// WZD s142p02
// WZD s142p03
// WZD s142p04
// WZD s142p05

// WZD s142p06
// drake178: CTY_GetTileFood()
int16_t City_Food_Terrain(int16_t city_idx);

// WZD s142p07
// drake178: CTY_GetWildGameFood()
int16_t City_Food_WildGame(int16_t city_idx);

// WZD s142p08
// drake178: CTY_GetCatchment()
int16_t Get_Useable_City_Area(int16_t city_wx, int16_t city_wy, int16_t city_wp, int16_t *wx_array, int16_t *wy_array);

// WZD s142p09
// drake178: CTY_GetFood()
int16_t City_Food_Production(int16_t city_idx);

// WZD s142p10

// WZD s142p11
// drake178: CTY_GetProd()
int16_t City_Production_Production(int16_t city_idx);

// WZD s142p12
// drake178: CTY_GetGold()
int16_t City_Gold_Production(int16_t city_idx);

// WZD s142p13
// drake178: CTY_GetResearch()
int16_t City_Research_Production(int16_t city_idx);

// WZD s142p14
// drake178: CTY_GetPower()
int16_t City_Mana_Production(int16_t city_idx);

// WZD s142p15
// WZD s142p16
// WZD s142p17
// WZD s142p18
// WZD s142p19
// WZD s142p20
// WZD s142p21
// WZD s142p22
// WZD s142p23
// WZD s142p24
// WZD s142p25
// WZD s142p26

// WZD s142p27
// drake178: CTY_GetRebelCount()
int16_t City_Rebel_Count(int16_t city_idx);

// WZD s142p28
// WZD s142p29

// WZD s142p30
// drake178: TILE_IsShared
int16_t City_Map_Square_Is_Shared(int16_t city_wx, int16_t city_wy, int16_t city_wp);

// WZD s142p31
// WZD s142p32
// WZD s142p33
// WZD s142p34
// WZD s142p35
// WZD s142p36



#ifdef __cplusplus
}
#endif

#endif  /* CITYCALC_H */
