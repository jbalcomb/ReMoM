/*
    Next Turn Procedure

    WIZARDS.EXE
        ovr060
        ovr119
        ovr121
        ovr140
        ovr141
*/

#ifndef NEXTTURN_H
#define NEXTTURN_H

#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr060
*/

// WZD o60p01
void Delete_Dead_Units(void);

// WZD o60p02
int16_t Player_Units_Max_Range(int16_t player_idx, int16_t wx, int16_t wy, int16_t wp);

// WZD o60p03
void empty_function__o060p03(void);

// WZD o60p04
void Next_Turn_Proc(void);

// WZD o60p05
// AI_ResetUnitMoves()

// WZD o60p06
void Update_Units_MvsSts(void);

// WZD o60p07
void TILE_VisibilityUpdt(void);



/*
    WIZARDS.EXE  ovr119
*/

// WZD o119p01
// AKA IDK_EoT_s9EC10()
void Next_Turn_Calc(void);

// WZD o119p02
void All_Colony_Calculations(void);

// WZD o119p03
// sub_9EF06()



/*
    WIZARDS.EXE  ovr121
*/

// WZD o121p01
int16_t Create_Unit__WIP(int16_t unit_type, int16_t owner_idx, int16_t wx, int16_t wy, int16_t wp, int16_t R_Param);

// WZD o121p02
int16_t Player_Base_Casting_Skill(int16_t player_idx);

// WZD o121p03
int16_t Casting_Cost(int16_t player_idx, int16_t spell_idx, int16_t combat_flag);

// WZD o121p04
int16_t Casting_Cost_Reduction(int16_t player_idx, int16_t spell_idx);

// WZD o121p05
int16_t Player_Hero_Count(int16_t player_idx);

// WZD o121p06
// Player_Dead_Hero_Count()

// WZD o121p07
int16_t Pick_Random_Hero(int16_t player_idx, int16_t zero_cost, int16_t hero_type_class);

// WZD o121p08
int16_t Unit_Moves2(int16_t unit_idx);

// WZD o121p09
// UNIT_IsNormalUnit()

// WZD o121p10
void Cheat_Power(void);

// WZD o121p11
// AI_GetThreat_UType()

// WZD o121p12
// AI_GetThreat_UNIT()

// WZD o121p13
void Patch_Units_Upkeep_And_Sound(void);

// WZD o121p14
void City_Delete_Building_Complete_Messages(int16_t city_idx);



/*
    WIZARDS.EXE  ovr140
*/

// WZD o140p01
void All_City_Calculations(void);

// WZD o140p02
void Initialize_Reports(void);

// WZD o140p03
void City_Apply_Production(int16_t city_idx);

// WZD o140p04
void Update_Players_Gold_Reserve(void);

// WZD o140p05
void Players_Apply_Magic_Power(void);

// WZD o140p06
void Players_Apply_Upkeeps__WIP(void);

// WZD o140p07
void WIZ_MatchFoodUpkeep(int16_t player_idx, int16_t food_excess, int16_t food_upkeep);

// WZD o140p08
int16_t WIZ_MatchGoldUpkeep(int16_t player_idx, int16_t gold_upkeep);

// WZD o140p09
int16_t WIZ_MatchManaUpkeep__WIP(int16_t player_idx, int16_t mana_upkeep);

// WZD o140p10
int16_t WIZ_DisbandSummons(int16_t player_idx, int16_t mana_upkeep);

// WZD o140p11
// WIZ_RemoveCEs()

// WZD o140p12
// WIZ_RemoveGEs()

// WZD o140p13
// WIZ_RemoveUEs()

// WZD o140p14
// sub_C538E()

// WZD o140p15
// sub_C53FA()

// WZD o140p16
void Apply_City_Changes(void);

// WZD o140p17
int16_t Player_Hero_Casting_Skill(int16_t player_idx);

// WZD o140p18
void All_Players_Apply_Spell_Casting(void);

// WZD o140p19
void Determine_Offer(void);

// WZD o140p20
void Players_Check_Spell_Research(void);

// WZD o140p21
void WIZ_LearnSpell__WIP(int16_t player_idx, int16_t spell_idx, int16_t New_Research);

// WZD o140p22
void Heal_Unit(int16_t unit_idx, int16_t fraction, int16_t flag);

// WZD o140p23
void Heal_All_Units(void);

// WZD o140p24
void Diplomacy_Growth_For_Enchantments__WIP(void);

// WZD o140p25
void Do_All_Units_XP_Check_(void);

// WZD o140p26
void Cool_Off_Volcanoes(void);

// WZD o140p27
void AI_Kill_Lame_Units(void);

// WZD o140p28
void AI_Calculate_Average_Unit_Cost(void);



/*
    WIZARDS.EXE  ovr141
*/

// WZD o141p01
void Do_Autosave(void);



#ifdef __cplusplus
}
#endif

#endif  /* NEXTTURN_H */
