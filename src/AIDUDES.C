/*
    AI - Data

    WIZARDS.EXE
        ¿ ovr145 ?
*/

#include "MoM.H"



/*
    WIZARDS.EXE  ovr145
*/

// WZD o145p01
// drake178: RP_WIZ_AllowMercs()
/*
; returns 1 if it is at least turn 50, and the player
; can afford to spend the specified amount of gold
;
; This function should not exist - both of these
; factors are already checked in the main mercenary
; generator function, turn also has lower limit
*/
/*
    decides if the CP/NP should accept the offer of mercenaries
*/
int16_t AI_Accept_Mercenaries(int16_t player_idx, int16_t cost)
{

    if(
        (_turn < 50)
        ||
        ((_players[player_idx].gold_reserve / 2) < cost)
    )
    {
        return ST_FALSE;
    }
    else
    {
        return ST_TRUE;
    }

}


// WZD o145p02
// drake178: AI_Overland_Turn()
/*
; process all aspects of the AI turns except diplomacy,
; including the neutral player
;
; contains a hoard of BUGs
; plans first for all players before processing for all
; players
*/
/*

*/
// void AI_Overland_Turn__WIP(void)
// {
// 
// 
// 
// }


// WZD o145p03
// AI_Continent_Eval()

// WZD o145p04
// AI_Power_Distrib()

// WZD o145p05
// WIZ_IsSpellKnown()

// WZD o145p06
// WIZ_KnownSpellCount()

// WZD o145p07
// AI_OVL_Eval_Assets()

// WZD o145p08
// AI_Count_Active_Wars()

// WZD o145p09
// sub_D405F()

// WZD o145p10
// drake178: TILE_CountUnits()
int16_t Map_Square_Unit_Count(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t unit_count;  // _SI_
    int16_t itr_units;  // _DI_

    unit_count = 0;

    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(
            (_UNITS[itr_units].wx == wx)
            &&
            (_UNITS[itr_units].wy == wy)
            &&
            (_UNITS[itr_units].wp == wp)
        )
        {
            unit_count += 1;
        }
    }

    return unit_count;
}

// WZD o145p11
// AI_Alchemy()

// WZD o145p12
// AI_Tax_And_Farmers()

// WZD o145p13
// AI_SetNeutralFarmers()

// WZD o145p14
// AI_ExcessBuilders()

// WZD o145p15
// AI_StasisDisband()

// WZD o145p16
// EMM_Map_CONTXXX()

// WZD o145p17
// CONTX_CreateChains()

// WZD o145p18
// CONTX_CreateLChains()

