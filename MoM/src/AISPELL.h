/*
    AI Spell

    WIZARDS.EXE
    ovr156

MoO2
Module: AITECH

*/

#ifndef AISPELL_H
#define AISPELL_H

#include "../../MoX/src/MOX_TYPE.h"



#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE ovr156
*/

// WZD o156p01
void AI_Research_Picker__STUB(int16_t player_idx);

// WZD o156p02
void AI_Spell_Select__STUB(int16_t player_idx);

// WZD o156p03
// AI_OVL_SplCat_Picker()

// WZD o156p04
void AI_Compute_Spells_Info(int16_t player_idx);

// WZD o156p05
// AI_OVL_PickSummon()

// WZD o156p06
// AI_OVL_PickUnitBuff()

// WZD o156p07
// AI_OVL_PickRealmSupr()

// WZD o156p08
// AI_OVL_PickGlobal()

// WZD o156p09
// AI_OVL_PickCurse()

// WZD o156p10
// AI_OVL_PickCityBuff()

// WZD o156p11
// SPL_IsLifeSupressed()

// WZD o156p12
// SPL_IsChsSuppressed()

// WZD o156p13
// SPL_IsDthSuppressed()

// WZD o156p14
// CRP_SPL_IsNatSuppressed()

// WZD o156p15
// AI_OVL_PickDise()

// WZD o156p16
// AI_OVL_PickDisj()

// WZD o156p17
// AITP_WallofStone()

// WZD o156p18
// AITP_Transmute()

// WZD o156p19
int16_t AITP_ChangeTerrain__WIP(int16_t player_idx, int16_t * wx, int16_t * wy, int16_t * wp);

// WZD o156p20
// AITP_MoveFortress()

// WZD o156p21
// AITP_FlyingFortress()

// WZD o156p22
// AITP_EarthGate()

// WZD o156p23
// AITP_GaiasBlessing()

// WZD o156p24
// AITP_WallofFire()

// WZD o156p25
// AITP_WallofDarkness()

// WZD o156p26
// AITP_HeavenlyLight()

// WZD o156p27
// AITP_AltarofBattle()

// WZD o156p28
// AITP_StreamofLife()

// WZD o156p29
// AITP_Inspirations()

// WZD o156p30
// AITP_Prosperity()

// WZD o156p31
// AITP_AstralGate()

// WZD o156p32
// AITP_DarkRituals()

// WZD o156p33
// AITP_CloudofShadow()

// WZD o156p34
int16_t AITP_SpellWard__STUB(int16_t player_idx, int16_t * city_idx, int16_t * magic_realm);

// WZD o156p35
// AITP_Consecration()

// WZD o156p36
// UU_DBG_GetKnownSpells()

// WZD o156p37
void Cast_Spell_Target_Error(int16_t spell_idx);

// WZD o156p38
void AI_Sanity_Check_Overland_Enchantments(int16_t player_idx);

// WZD o156p39
int16_t Pick_Target_For_City_Enchantment__WIP(int16_t spell_target_type, int16_t * city_idx, int16_t spell_idx, int16_t player_idx);

// WZD o156p40
// AI_CanTargetCities()

// WZD o156p41
int16_t IDK_Pick_Target_For_Unit_Enchantment__STUB(int16_t spell_target_type, int16_t * unit_idx, int16_t spell_idx, int16_t player_idx);

// WZD o156p42
// sub_E8448()

// WZD o156p43
// sub_E882B()

// WZD o156p44
int16_t IDK_AITP_Disjunction__STUB(int16_t * wx, int16_t * wy, int16_t spell_idx, int16_t player_idx);

// WZD o156p45
int16_t Get_Map_Square_Target_For_Spell(int16_t spell_arget_type, int16_t * wx, int16_t * wy, int16_t * wp, int16_t spell_idx, int16_t player_idx);

// WZD o156p46
// AITP_OVL_HarmStack()

// WZD o156p47
int16_t Pick_Target_For_Floating_Island(int16_t player_idx, int16_t * wx, int16_t * wy, int16_t * wp);

// WZD o156p48
// AITP_EnchantRoad()

// WZD o156p49
// AITP_Disenchant()

// WZD o156p50
// AITP_HarmTerrain()

// WZD o156p51
// sub_E9FA9()

// WZD o156p52
// AITP_PlaneShift()

// WZD o156p53
// AITP_NaturesCures()

// WZD o156p54
// sub_EA43C()
int16_t IDK_AITP_Target_Wizard__STUB(int16_t * spell_target_idx, int16_t spell_idx, int16_t player_idx);

// WZD o156p55
int16_t AITP_SpellWard_Wrapper__STUB(int16_t * city_idx, int16_t * magic_realm, int16_t player_idx);

// WZD o156p56
// AI_MyrrorAdvantage()

// WZD o156p57
int16_t Square_Is_Legal_For_Floating_Island(int16_t wx, int16_t wy, int16_t wp);

// WZD o156p58
// AITP_Summoning_Circle()

// WZD o156p59
// WIZ_SummonInFortress()



#ifdef __cplusplus
}
#endif


#endif  /* AISPELL_H */
