/*
    AI - Data

    WIZARDS.EXE
        ¿ ovr145 ?
        ¿ ovr164 ?
*/

#ifndef AIDATA_H
#define AIDATA_H

#include "MOM_DEF.H"
#include "MOX/MOX_TYPE.H"



#ifdef __cplusplus
extern "C" {
#endif



#define AI_OBJ_PRAGMATIST       0
#define AI_OBJ_MILITARIST       1
#define AI_OBJ_THEURGIST        2
#define AI_OBJ_PERFECTIONIST    3
#define AI_OBJ_EXPANSIONIST     4

// //  ; (sizeof=0x5, standard type)
// struct s_AI_BLD_Obj_Modifier
// {
//     /* 00 */ Pragmatist db ?                         ; base 10
//     /* 01 */ Militarist db ?                         ; base 10
//     /* 02 */ Theurgist db ?                          ; base 10
//     /* 03 */ Perfectionist db ?                      ; base 10
//     /* 04 */ Expansionist db ?                       ; base 10
//     /* 05 */
// };
// 
// //  ; (sizeof=0x32)
// struct s_AI_BLD_Obj_Priorities
// {
//     /* 00 */  Projects AI_BLD_Obj_Modifier ?
//     /* 05 */  Gold AI_BLD_Obj_Modifier ?
//     /* 0A */  Religion AI_BLD_Obj_Modifier ?
//     /* 0F */  Research AI_BLD_Obj_Modifier ?
//     /* 14 */  Military AI_BLD_Obj_Modifier ?
//     /* 19 */  Production AI_BLD_Obj_Modifier ?
//     /* 1E */  Food AI_BLD_Obj_Modifier ?
//     /* 23 */  Navy AI_BLD_Obj_Modifier ?
//     /* 28 */  Combat_Unit AI_BLD_Obj_Modifier ?
//     /* 2D */  Builder AI_BLD_Obj_Modifier ?
//     /* 32 */
// };


// enum AI_STK_Capabilities
#define AICAP_Transport     0x1
#define AICAP_RoadBuild     0x2
#define AICAP_Purify        0x4
#define AICAP_LandOnly      0x8
#define AICAP_WindWalk      0x10
#define AICAP_Hero          0x20
#define AICAP_Melding       0x40
#define AICAP_Settler       0x80


/*
holds (Effective_Unit_Strength(unit_idx) / 10)
high 2 bits indicate nonhostile and site (city, intact lair, node)
bit 14  set means nonhostile, ¿ unset means hostile ?
bit 15  ¿set means {City,Lair,Node} ?, ¿ unset means Unit ?

AI_SetEnemyStrMaps() clears 2 * 2400 = 4800 words  9600 bytes

max strength value (6 bits) 0x3FFF  16383

Tile_Enemy_Value = CONTX_NmeStrMap[wp][((Next_Tile_Y * WORLD_WIDTH) + Next_Tile_X)];

((Tile_Enemy_Value & 0x3FFF) != 0)
&&
((Tile_Enemy_Value & 0x4000) != 0)
&&
((Tile_Enemy_Value & 0x8000) != 0)
*/
#define AI_TARGET_FORTRESS      0x3FFF
#define AI_TARGET_NONHOSTILE    0x4000

#define AI_TARGET_SITE          0x8000

//  ; (sizeof=0x6)
struct s_AI_TARGET
{
    /* 00 */ uint8_t wx;
    /* 01 */ uint8_t wy;
    /* 02 */ uint8_t wp;
    /* 03 */ uint8_t field_3;
    /* 04 */ int16_t value;
    /* 06 */ 
};


//  ; (sizeof=0xA)
struct s_AI_STACK_DATA
{
    /* 00 */  uint8_t wx;
    /* 01 */  uint8_t wy;
    /* 02 */  uint8_t wp;
    /* 03 */  uint8_t abilities;  /* ; enum AI_STK_Capabilities */
    /* 04 */  int16_t value;
    /* 06 */  uint8_t transport_capacity;
    /* 07 */  uint8_t unit_count;
    /* 08 */  uint8_t landmass_idx;
    /* 09 */  uint8_t unit_status;
    /* 0A */  
};


//  ; (sizeof=0xB4)
struct s_CONTS
{
    /* 00 */ uint8_t X_Coords[NUM_LANDMASSES];
    /* 3C */ uint8_t Y_Coords[NUM_LANDMASSES];
    /* 78 */ uint8_t Cont_Types[NUM_LANDMASSES];    /* ; enum CONT_Types */
    /* B4 */ 
};

//  ; (sizeof=0x384)
struct s_CONT_TBL
{
    /* 0000 */ struct s_CONTS Player_0;
    /* 00B4 */ struct s_CONTS Player_1;
    /* 0168 */ struct s_CONTS Player_2;
    /* 021C */ struct s_CONTS Player_3;
    /* 02D0 */ struct s_CONTS Player_4;
    /* 0384 */ 
};

enum e_CONT_Types
{
    CONT_Invalid    = 0,
    CONT_Own        = 1,
    CONT_Contested  = 2,
    CONT_NoPresence = 3,
    CONT_NoLanding  = 4,
    CONT_Abandon    = 5,
    CONT_NoTargets  = 6
};

struct s_AI_CONTINENTS_PLAYERS
{
    struct s_CONTS player[NUM_PLAYERS];
};

struct s_AI_CONTINENTS
{
    struct s_AI_CONTINENTS_PLAYERS plane[NUM_PLANES];
};


enum e_AI_SPELL_GROUP
{
    SGRP_Scouting           =  0,
    SGRP_Dispel             =  1,
    SGRP_ResistBuff_1       =  4,
    SGRP_CombatHarm_1       =  5,
    SGRP_MoveBuff_1         =  6,
    SGRP_AttackBuff_1       =  7,
    SGRP_WallsRituals       =  8,
    SGRP_DefenseBuff_1      = 10,
    SGRP_Disrupt            = 12,
    SGRP_OVL_CommonSummon   = 13,
    SGRP_Corruption         = 14,
    SGRP_JustCause          = 15,
    SGRP_CombatSummon_1     = 16,
    SGRP_CE_1               = 17,
    SGRP_Heals              = 18,
    SGRP_SpellLock          = 19,
    SGRP_DispelTrue         = 20,
    SGRP_Disenchant         = 21,
    SGRP_Disjunction        = 22,
    SGRP_CombatCurse        = 24,
    SGRP_MoveBuff_2         = 25,
    SGRP_AttackBuff_2       = 26,
    SGRP_OVL_Damage         = 28,
    SGRP_ImmolInvuln        = 29,
    SGRP_EnchantItem        = 30,
    SGRP_CombatMisc         = 31,
    SGRP_UC_Summon          = 32,
    SGRP_Famine             = 33,
    SGRP_SorceryGE          = 34,
    SGRP_CombatSummon_2     = 35,
    SGRP_CE_2               = 36,
    SGRP_TerrainMod         = 37,
    SGRP_HarmWizard         = 38,
    SGRP_Stasis             = 39,
    SGRP_CC_WW              = 40,
    SGRP_WarpNode           = 41,
    SGRP_WordofRecall       = 42,
    SGRP_DisjTrue           = 45,
    SGRP_ElemArmor          = 46,
    SGRP_CombatHarm_2       = 47,
    SGRP_MoveBuff_3         = 48,
    SGRP_CityBuff_1         = 51,
    SGRP_Earthquake         = 52,
    SGRP_DefenseBuff_2      = 53,
    SGRP_Artifact           = 54,
    SGRP_Haste              = 55,
    SGRP_RareSummon         = 56,
    SGRP_RaiseVolcano       = 57,
    SGRP_GreatWasting       = 58,
    SGRP_CombatSummon_3     = 59,
    SGRP_CE_3               = 60,
    SGRP_Rez                = 61,
    SGRP_PlanarSeal         = 62,
    SGRP_LifeForce          = 63,
    SGRP_Tranquility        = 64,
    SGRP_EvilOmens          = 65,
    SGRP_CityBuff_2         = 66,
    SGRP_MagicImm           = 69,
    SGRP_CombatHarm_3       = 70,
    SGRP_Gates              = 71,
    SGRP_CityBuff_3         = 73,
    SGRP_RegenMassInvis     = 75,
    SGRP_AnimateDead        = 77,
    SGRP_VRSummon           = 78,
    SGRP_DestroyCity        = 79,
    SGRP_GE                 = 80,
    SGRP_CE_4               = 82,
    SGRP_Unsummon           = 83,
    SGRP_TimeStop           = 84,
    SGRP_Suppress           = 85,
    SGRP_DeathWish          = 86,
    SGRP_SummonHero         = 87,
    SGRP_SummonChamp        = 88,
    SGRP_SummonCircle       = 89,
    SGRP_SoM                = 90,
};



/*
    WIZARDS.EXE  ovr145
*/

// WZD o145p0



/*
    WIZARDS.EXE  ovr64
*/

// WZD o164p01
// AI_ExcessNeutrals()

// WZD o164p02
// EVNT_RampageMonsters()

// WZD o164p03
// EVNT_GenerateRaiders()

// WZD o164p04
// AI_SetNeutralTargets()

// WZD o164p05
void Allocate_AI_Data(void);

// WZD o164p06
void AI_Magic_Strategy__WIP(int16_t player_idx);

// WZD o164p07
void AI_Evaluate_Hostility(int16_t player_idx);

// WZD o164p08
// AI_GetNeutralStacks()




#ifdef __cplusplus
}
#endif

#endif  /* AIDATA_H */
