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



enum e_AI_SPELL_GROUP
{
    SGRP_Scouting  = 0,
    SGRP_Dispel  = 1,
    SGRP_ResistBuff_1  = 4,
    SGRP_CombatHarm_1  = 5,
    SGRP_MoveBuff_1  = 6,
    SGRP_AttackBuff_1  = 7,
    SGRP_WallsRituals  = 8,
    SGRP_DefenseBuff_1  = 10,
    SGRP_Disrupt  = 12,
    SGRP_OVL_CommonSummon  = 13,
    SGRP_Corruption  = 14,
    SGRP_JustCause  = 15,
    SGRP_CombatSummon_1  = 16,
    SGRP_CE_1  = 17,
    SGRP_Heals  = 18,
    SGRP_SpellLock  = 19,
    SGRP_DispelTrue  = 20,
    SGRP_Disenchant  = 21,
    SGRP_Disjunction  = 22,
    SGRP_CombatCurse  = 24,
    SGRP_MoveBuff_2  = 25,
    SGRP_AttackBuff_2  = 26,
    SGRP_OVL_Damage  = 28,
    SGRP_ImmolInvuln  = 29,
    SGRP_EnchantItem  = 30,
    SGRP_CombatMisc  = 31,
    SGRP_UC_Summon  = 32,
    SGRP_Famine  = 33,
    SGRP_SorceryGE  = 34,
    SGRP_CombatSummon_2  = 35,
    SGRP_CE_2  = 36,
    SGRP_TerrainMod  = 37,
    SGRP_HarmWizard  = 38,
    SGRP_Stasis  = 39,
    SGRP_CC_WW  = 40,
    SGRP_WarpNode  = 41,
    SGRP_WordofRecall  = 42,
    SGRP_DisjTrue  = 45,
    SGRP_ElemArmor  = 46,
    SGRP_CombatHarm_2  = 47,
    SGRP_MoveBuff_3  = 48,
    SGRP_CityBuff_1  = 51,
    SGRP_Earthquake  = 52,
    SGRP_DefenseBuff_2  = 53,
    SGRP_Artifact  = 54,
    SGRP_Haste  = 55,
    SGRP_RareSummon  = 56,
    SGRP_RaiseVolcano  = 57,
    SGRP_GreatWasting  = 58,
    SGRP_CombatSummon_3  = 59,
    SGRP_CE_3  = 60,
    SGRP_Rez  = 61,
    SGRP_PlanarSeal  = 62,
    SGRP_LifeForce  = 63,
    SGRP_Tranquility  = 64,
    SGRP_EvilOmens  = 65,
    SGRP_CityBuff_2  = 66,
    SGRP_MagicImm  = 69,
    SGRP_CombatHarm_3  = 70,
    SGRP_Gates  = 71,
    SGRP_CityBuff_3  = 73,
    SGRP_RegenMassInvis  = 75,
    SGRP_AnimateDead  = 77,
    SGRP_VRSummon  = 78,
    SGRP_DestroyCity  = 79,
    SGRP_GE  = 80,

    SGRP_IDK  = 81,  /* ¿ SGRP_CombatSummon_4 ? something combat for militarist objective */
    
    SGRP_CE_4  = 82,
    SGRP_Unsummon  = 83,
    SGRP_TimeStop  = 84,
    SGRP_Suppress  = 85,
    SGRP_DeathWish  = 86,
    SGRP_SummonHero  = 87,
    SGRP_SummonChamp  = 88,
    SGRP_SummonCircle  = 89,
    SGRP_SoM  = 90
};



#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE ovr156
*/

// WZD o156p01
int16_t AI_Spell_Research_Select(int16_t player_idx);

// WZD o156p02
void AI_Spell_Select(int16_t player_idx);

// WZD o156p03
int16_t AI_Select_Spell_Group(int16_t player_idx);

// WZD o156p04
void AI_Compute_Spells_Info(int16_t player_idx);

// WZD o156p05
int16_t AI_Select_Spell_Group_Summon(int16_t player_idx);

// WZD o156p06
int16_t AI_Select_Spell_Group_Unit_Enchantment(int16_t player_idx);

// WZD o156p07
int16_t AI_Select_Spell_Group_Suppression(int16_t player_idx);

// WZD o156p08
int16_t AI_Select_Spell_Group_Global(int16_t player_idx);

// WZD o156p09
int16_t AI_Select_Spell_Group_Attack(int16_t player_idx);

// WZD o156p10
int16_t AI_Select_Spell_Group_City_Enchantment(int16_t player_idx);

// WZD o156p11
int16_t SPL_IsLifeSupressed(void);

// WZD o156p12
int16_t SPL_IsChsSuppressed(void);

// WZD o156p13
int16_t SPL_IsDthSuppressed(void);

// WZD o156p14
int16_t CRP_SPL_IsNatSuppressed(void);

// WZD o156p15
int16_t AI_Select_Spell_Group_Disenchant(int16_t player_idx);

// WZD o156p16
int16_t AI_Select_Spell_Group_Disjunction(int16_t player_idx);

// WZD o156p17
int16_t AITP_Wall_Of_Stone(int16_t player_idx, int16_t * city_idx);

// WZD o156p18
int16_t AITP_Transmute(int16_t player_idx, int16_t * targeted_wx, int16_t * targeted_wy, int16_t * targeted_wp);

// WZD o156p19
int16_t AITP_Change_Terrain(int16_t player_idx, int16_t * wx, int16_t * wy, int16_t * wp);

// WZD o156p20
int16_t AITP_Move_Fortress(int16_t player_idx, int16_t * targeted_city_idx);

// WZD o156p21
int16_t AITP_FlyingFortress(int16_t player_idx, int16_t * city_idx);

// WZD o156p22
int16_t AITP_Earth_Gate(int16_t player_idx, int16_t * targeted_city_idx);

// WZD o156p23
int16_t AITP_Gaias_Blessing(int16_t player_idx, int16_t * city_idx);

// WZD o156p24
int16_t AITP_WallofFire(int16_t player_idx, int16_t * city_idx);

// WZD o156p25
int16_t AITP_WallofDarkness(int16_t player_idx, int16_t * city_idx);

// WZD o156p26
int16_t AITP_HeavenlyLight(int16_t player_idx, int16_t * city_idx);

// WZD o156p27
int16_t AITP_AltarofBattle(int16_t player_idx, int16_t * city_idx);

// WZD o156p28
int16_t AITP_StreamofLife(int16_t player_idx, int16_t * city_idx);

// WZD o156p29
int16_t AITP_Inspirations(int16_t player_idx, int16_t * city_idx);

// WZD o156p30
int16_t AITP_Prosperity(int16_t player_idx, int16_t * city_idx);

// WZD o156p31
int16_t AITP_AstralGate(int16_t player_idx, int16_t * city_idx);

// WZD o156p32
int16_t AITP_DarkRituals(int16_t player_idx, int16_t * city_idx);

// WZD o156p33
int16_t AITP_CloudofShadow(int16_t player_idx, int16_t * city_idx);

// WZD o156p34
int16_t AITP_SpellWard__STUB(int16_t player_idx, int16_t * city_idx, int16_t * magic_realm);

// WZD o156p35
int16_t AITP_Consecration(int16_t player_idx, int16_t * city_idx);

// WZD o156p36
// UU_DBG_GetKnownSpells()

// WZD o156p37
void Cast_Spell_Target_Error(int16_t spell_idx);

// WZD o156p38
void AI_Sanity_Check_Overland_Enchantments(int16_t player_idx);

// WZD o156p39
int16_t Pick_Target_For_City_Enchantment__WIP(int16_t spell_target_type, int16_t * city_idx, int16_t spell_idx, int16_t player_idx);

// WZD o156p40
int16_t AI_Has_Targetable_City(int16_t player_idx);

// WZD o156p41
int16_t IDK_Pick_Target_For_Unit_Enchantment__STUB(int16_t spell_target_type, int16_t * unit_idx, int16_t spell_idx, int16_t player_idx);

// WZD o156p42
// sub_E8448()

// WZD o156p43
// sub_E882B()

// WZD o156p44
int16_t AITP_Disjunction(int16_t * targeted_player_idx, int16_t * targeted_spell_idx, int16_t spell_idx, int16_t player_idx);

// WZD o156p45
int16_t Get_Map_Square_Target_For_Spell(int16_t spell_target_type, int16_t * wx, int16_t * wy, int16_t * wp, int16_t spell_idx, int16_t player_idx);

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
int16_t AI_Has_More_Myrror_Cities(int16_t player_idx);

// WZD o156p57
int16_t Square_Is_Legal_For_Floating_Island(int16_t wx, int16_t wy, int16_t wp);

// WZD o156p58
int16_t AITP_Summoning_Circle(int16_t player_idx, int16_t * city_idx);

// WZD o156p59
int16_t Player_Summon_City_Is_Fortress_City(int16_t player_idx);



#ifdef __cplusplus
}
#endif


#endif  /* AISPELL_H */
