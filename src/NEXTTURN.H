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

// WZD o121p02
// AKAK Calc_Nominal_Skill()
int16_t Player_Base_Casting_Skill(int16_t player_idx);

// WZD o121p03
// WZD o121p04
// WZD o121p05
// WZD o121p06
// WZD o121p07
// WZD o121p08
int16_t UNIT_GetHalfMoves_WIP(int16_t unit_idx);

// WZD o121p09

// WZD o121p10
// WZD o121p11
// WZD o121p12
// WZD o121p13
// WZD o121p14



/*
    WIZARDS.EXE  ovr140
*/

// WZD s140p01
// drake178: CTY_RecalculateAll()
void All_City_Calculations(void);

// WZD s140p02
// WZD s140p03

// WZD s140p04
// drake178: WIZ_GoldIncomes()
void Update_Players_Gold_Reserve(void);

// WZD o140p05
// drake178: WIZ_PowerIncomes()
// OON XREF Next_Turn_Calc()
void Players_Apply_Magic_Power(void);

// WZD s140p06
// WZD s140p07
// WZD s140p08
// WZD s140p09
// WZD s140p10
// WZD s140p11
// WZD s140p12
// WZD s140p13
// WZD s140p14
// WZD s140p15
// WZD s140p16

// WZD o140p17
// drake178: WIZ_SkillFromHeroes()
int16_t Player_Hero_Casting_Skill(int16_t player_idx);

// WZD s140p18
// WZD s140p19
// WZD s140p20
// WZD s140p21
// WZD s140p22
// WZD s140p23
// WZD s140p24
// WZD s140p25
// WZD s140p26
// WZD s140p27
// WZD s140p28
// Cool_Off_Volcanoes



#ifdef __cplusplus
}
#endif

#endif  /* NEXTTURN_H */
