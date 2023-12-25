/*
    Next Turn Procedure

    WIZARDS.EXE
        ovr060
        ovr119
        ovr121
        ovr140
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
// WZD o60p02
// WZD o60p03

// WZD o60p04
void Next_Turn_Proc(void);

// WZD o60p05

// WZD o60p06
void Update_Units_MvsSts(void);

// WZD o60p07
// void TILE_VisibilityUpdt(void);



/*
    WIZARDS.EXE  ovr119
*/

// WZD o119p01
// AKA IDK_EoT_s9EC10()
void Next_Turn_Calc(void);

// WZD o119p02
// CTY_CheckMinFarmers()

// WZD o119p03
// sub_9EF06()



/*
    WIZARDS.EXE  ovr121
*/

// WZD o121p01
// drake178: UNIT_Create()
int16_t Create_Unit__WIP(int16_t unit_type, int16_t owner_idx, int16_t wx, int16_t wy, int16_t wp, int16_t R_Param);

// WZD o121p02
// AKAK Calc_Nominal_Skill()
int16_t Player_Base_Casting_Skill(int16_t player_idx);

// WZD o121p03
// drake178: WIZ_GetCastingCost()
// WIZ_GetCastingCost()

// WZD o121p04
// drake178: WIZ_CastingCostBonus()
// WIZ_CastingCostBonus()

// WZD o121p05
// drake178: WIZ_GetHeroCount()
// WIZ_GetHeroCount()

// WZD o121p06
// drake178: WIZ_DeadHeroCount()
// WIZ_DeadHeroCount()

// WZD o121p07
// drake178: WIZ_GetRandomHero()
// WIZ_GetRandomHero()

// WZD o121p08
int16_t UNIT_GetHalfMoves_WIP(int16_t unit_idx);

// WZD o121p09
// drake178: UNIT_IsNormalUnit()
// UNIT_IsNormalUnit()

// WZD o121p10
// drake178: ¿ ?
// IDK_MagicScreen_PWR()

// WZD o121p11
// drake178: AI_GetThreat_UType()
// AI_GetThreat_UType()

// WZD o121p12
// drake178: AI_GetThreat_UNIT()
// AI_GetThreat_UNIT()

// WZD o121p13
// drake178: RP_GAME_UnitTypesUpdate()
void Patch_Units_Upkeep_And_Sound(void);

// WZD o121p14
// drake178: CTY_RemoveMessages()
// CTY_RemoveMessages()



/*
    WIZARDS.EXE  ovr140
*/

// WZD o140p01
// drake178: CTY_RecalculateAll()
void All_City_Calculations(void);

// WZD o140p02
// drake178: MSG_Clear()
void MSG_Clear(void);

// WZD o140p03
// drake178: CTY_ProdProgress()
void CTY_ProdProgress(int16_t city_idx);

// WZD o140p04
// drake178: WIZ_GoldIncomes()
void Update_Players_Gold_Reserve(void);

// WZD o140p05
// drake178: WIZ_PowerIncomes()
// OON XREF Next_Turn_Calc()
void Players_Apply_Magic_Power(void);

// WZD o140p06
// drake178: WIZ_ProcessUpkeep()
void Players_Apply_Upkeeps__WIP(void);

// WZD o140p07
// drake178: WIZ_MatchFoodUpkeep()
// WIZ_MatchFoodUpkeep()

// WZD o140p08
// drake178: WIZ_MatchGoldUpkeep()
// WIZ_MatchGoldUpkeep()

// WZD o140p09
// drake178: WIZ_MatchManaUpkeep()
// WIZ_MatchManaUpkeep()

// WZD o140p10
// drake178: WIZ_DisbandSummons()
// WIZ_DisbandSummons()

// WZD o140p11
// drake178: WIZ_RemoveCEs()
// WIZ_RemoveCEs()

// WZD o140p12
// drake178: WIZ_RemoveGEs()
// WIZ_RemoveGEs()

// WZD o140p13
// drake178: WIZ_RemoveUEs()
// WIZ_RemoveUEs()

// WZD o140p14
// drake178: N/A
// sub_C538E()

// WZD o140p15
// drake178: N/A
// sub_C53FA()

// WZD o140p16
// drake178: CTY_ProgressTurn()
void CTY_ProgressTurn(void);

// WZD o140p17
// drake178: WIZ_SkillFromHeroes()
int16_t Player_Hero_Casting_Skill(int16_t player_idx);

// WZD o140p18
// drake178: N/A
// IDK_SplCst_SplSkl_sC5AB1()

// WZD o140p19
// drake178: EVNT_RandomOffers()
// EVNT_RandomOffers()

// WZD o140p20
// drake178: WIZ_ResearchProgress()
// WIZ_ResearchProgress()

// WZD o140p21
// drake178: WIZ_LearnSpell()
// WIZ_LearnSpell()

// WZD o140p22
// drake178: G_UNIT_OvlHealing()
// G_UNIT_OvlHealing()

// WZD o140p23
// drake178: N/A
// IDK_Unit_Heal_sC6572()

// WZD o140p24
// drake178: WIZ_ProcessGlobals()
// WIZ_ProcessGlobals()

// WZD o140p25
// drake178: N/A
// IDK_Unit_XP_sC6BCF()

// WZD o140p26
// drake178: ¿ ?
// Cool_Off_Volcanoes()

// WZD o140p27
// drake178: AI_CullTheWeak()
// AI_CullTheWeak()

// WZD o140p28
// drake178: AI_GetAvgUnitCosts()
// AI_GetAvgUnitCosts()



#ifdef __cplusplus
}
#endif

#endif  /* NEXTTURN_H */
