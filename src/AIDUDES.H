/*
    AI - Data

    WIZARDS.EXE
        ¿ ovr145 ?
*/

#ifndef AIDUDES_H
#define AIDUDES_H

#include "MOM.H"

#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr145
*/

// WZD o145p01
int16_t AI_Accept_Mercenaries(int16_t player_idx, int16_t cost);

// WZD o145p02
void AI_Next_Turn__WIP(void);

// WZD o145p03
void AI_Continent_Eval__WIP(int16_t player_idx);

// WZD o145p04
void AI_Update_Magic_Power(int16_t player);

// WZD o145p05
int16_t Player_Knows_Spell(int16_t player_idx, int16_t spell_idx);

// WZD o145p06
int16_t Player_Known_Spell_Count(int16_t player_idx);

// WZD o145p07
void AI_Player_Calculate_Target_Values(int16_t player_idx);

// WZD o145p08
void Player_Hostile_Opponents(int16_t player_idx);

// WZD o145p09
// sub_D405F()

// WZD o145p10
int16_t Map_Square_Unit_Count(int16_t wx, int16_t wy, int16_t wp);

// WZD o145p11
void AI_Update_Gold_And_Mana_Reserves(int16_t player_idx);

// WZD o145p12
void AI_Update_Gold_Income_And_Food_Income(int16_t player_idx);

// WZD o145p13
// AI_SetNeutralFarmers()

// WZD o145p14
void AI_Kill_Excess_Settlers_And_Engineers(int16_t player_idx);

// WZD o145p15
// AI_StasisDisband()

// WZD o145p16
void EMM_Map_CONTXXX__WIP(void);

// WZD o145p17
void CONTX_CreateChains__WIP(void);

// WZD o145p18
void CONTX_CreateLChains__WIP(void);



#ifdef __cplusplus
}
#endif

#endif  /* AIDUDES_H */
