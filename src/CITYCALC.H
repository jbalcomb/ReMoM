/*
    City Calculations

    WIZARDS.EXE
        ovr120
        ovr142
*/

#ifndef CITYCALC_H
#define CITYCALC_H

#include "MOM.H"

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
void Player_Resource_Income_Total(int16_t player_idx, int16_t * gold_total, int16_t * food_total, int16_t * mana_total);

// WZD o120p08
int16_t Player_Armies_And_Enchantments_Mana_Upkeep(int16_t player_idx);

// WZD o120p09
// drake178: UNIT_GetManaUpkeep()
int16_t Unit_Mana_Upkeep(int16_t unit_idx);

// WZD o120p10
// drake178: CTY_EnchantsUpkeep()
int16_t Player_City_Enchantments_Upkeep(int16_t city_idx, int16_t player_idx);

// WZD o120p11
// drake178: WIZ_GlobalsUpkeep()
int16_t Player_Overland_Enchantments_Upkeep(int16_t player_idx);

// WZD o120p12
// drake178: UU_UNIT_OnSameTile()
// UU_UNIT_OnSameTile()

// WZD o120p13
// drake178: UNIT_MarkRemoved()
void Kill_Unit(int16_t unit_idx, int16_t Rmv_Type);

// WZD o120p14
// drake178: UU_BYTE_SortDescnd()
// UU_BYTE_SortDescnd()

// WZD o120p15
// drake178: TILE_GetUnits()
void Army_At_Square_1(int16_t wx, int16_t wy, int16_t wp, int16_t * troop_count, int16_t troops[]);

// WZD o120p16
// drake178: UU_WIZ_GetReligion()
// UU_WIZ_GetReligion()

// WZD o120p17
void Player_Magic_Power_Distribution(int16_t * Base_Research_Income, int16_t * Base_Skill_Income, int16_t * Base_Mana_Income, int16_t player_idx);

// WZD o120p18
void Player_Magic_Power_Income_Total(int16_t * Mana, int16_t * City_Food_Surplus, int16_t * City_Gold_Balance, int16_t player_idx);

// WZD o120p19
int16_t Player_Spell_Research_Bonus(int16_t player_idx, int16_t spell_idx);

// WZD o120p20
int16_t Player_Fame(int16_t player_idx);

// WZD o120p21
int16_t Hero_Slot_Open(int16_t player_idx);

// WZD o120p22
// drake178: CTY_GetGoldUpkeep()
int16_t City_Gold_Mainanence(int16_t city_idx);

// WZD o120p23
// drake178: UNIT_GetLevel()
int16_t Calc_Unit_Level(int16_t unit_idx);

// WZD o120p24
// drake178: UNIT_GetBaseLevel()
int16_t Unit_Base_Level(int16_t unit_idx);

// WZD o120p25
// drake178: UNIT_LoggedPushOff()
void UNIT_LoggedPushOff(int16_t unit_idx);

// WZD o120p26
// drake178: UNIT_RemoveExcess()
void UNIT_RemoveExcess(int16_t unit_idx);



/*
    WIZARDS.EXE  ovr142
*/

// WZD o142p01
void TILE_CreateRoad(int16_t wx, int16_t wy, int16_t wp);

// WZD o142p02
void TILE_EnchantRoad(int16_t wx, int16_t wy, int16_t wp);

// WZD o142p03
void empty_fxn_o142p03(int16_t wx, int16_t wy, int16_t wp);

// WZD o142p04
void NOOP_Current_Player_All_City_Areas(void);

// WZD o142p05
int16_t City_House_Count(int16_t city_idx);

// WZD o142p06
int16_t City_Food_Terrain(int16_t city_idx);

// WZD o142p07
int16_t City_Food_WildGame(int16_t city_idx);

// WZD o142p08
int16_t Get_Useable_City_Area(int16_t city_wx, int16_t city_wy, int16_t city_wp, int16_t *wx_array, int16_t *wy_array);

// WZD o142p09
int16_t City_Food_Production(int16_t city_idx);

// WZD o142p10
int16_t City_Maximum_Size(int16_t city_idx);

// WZD o142p11
int16_t City_Production_Production(int16_t city_idx);

// WZD o142p12
int16_t City_Gold_Production(int16_t city_idx);

// WZD o142p13
int16_t City_Research_Production(int16_t city_idx);

// WZD o142p14
int16_t City_Mana_Production(int16_t city_idx);

// WZD o142p15
int16_t City_Growth_Rate(int16_t city_idx);

// WZD o142p16
int16_t City_Current_Product_Cost(int16_t city_idx);

// WZD o142p17
// UU_AI_Fort_Defense()

// WZD o142p18
void Generate_Mercenaries(int16_t player_idx, int16_t * wx, int16_t * wy, int16_t * wp, int16_t * amount, int16_t * type, int16_t * cost, int16_t * level);

// WZD o142p19
// LD_MAP_TFUnk40_Eval()

// WZD o142p20
int16_t City_Minimum_Farmers(int16_t city_idx);

// WZD o142p21
void All_Outpost_Population_Growth(void);

// WZD o142p22
void Record_History(void);

// WZD o142p23
int16_t WIZ_Get_Astr_Power__STUB(int16_t player_idx);

// WZD o142p24
int16_t WIZ_Get_Astr_ArmyStr__STUB(int16_t player_idx);

// WZD o142p25
int16_t WIZ_Get_Astr_Resrch__STUB(int16_t player_idx);

// WZD o142p26
int16_t WIZ_Get_Nation_Size__STUB(int16_t player_idx);

// WZD o142p27
int16_t City_Rebel_Count(int16_t city_idx);

// WZD o142p28
void Compute_Base_Map_Square_Values(int16_t wx, int16_t wy, int16_t wp, int16_t *MaxPop, int16_t *Prod, int16_t *TrdGld, int16_t *CostRed, int16_t *Gold, int16_t *Power, int16_t *NitShd, int16_t *Mithrl, int16_t *Admntm, int16_t *Shored, int16_t *Unexplored);

// WZD o142p29
void Reset_City_Area_Bitfields(void);

// WZD o142p30
int16_t City_Area_Square_Is_Shared(int16_t city_area_square_wx, int16_t city_area_square_wy, int16_t city_wp);

// WZD o142p31
void City_Remove_Road(int16_t wx, int16_t wy, int16_t wp);

// WZD o142p32
// CTY_Food_Breakdown()

// WZD o142p33
// CTY_Gold_Breakdown()

// WZD o142p34
// CTY_Prod_Breakdown()

// WZD o142p35
// CTY_Rsrc_Breakdown()

// WZD o142p36
// CTY_Pwr_Breakdown()



#ifdef __cplusplus
}
#endif

#endif  /* CITYCALC_H */
