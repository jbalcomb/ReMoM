/*
    AI - Data

    WIZARDS.EXE
        ¿ ovr145 ?
*/

#include "../../MoX/src/EMS/EMS.h"
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOX_BASE.h"
#include "../../MoX/src/MOX_DAT.h"  /* _players[] */
#include "../../MoX/src/MOX_TYPE.h"
#include "../../MoX/src/random.h"

#include "AIBUILD.h"
#include "AIDATA.h"
#include "AIMOVE.h"
#include "AISPELL.h"
#include "CITYCALC.h"
#include "NEXTTURN.h"
#include "RACETYPE.h"
#include "SETTLE.h"
#include "Spellbook.h"
#include "Terrain.h"
#include "UNITTYPE.h"
#include "UnitMove.h"

#include "../../STU/src/STU_DBG.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>     /* memcpy() memset(), stu_strcat(), stu_strcpy(), stricmp() */

#include "AIDUDES.h"



// WZD dseg:6F60                                                 BEGIN:  ovr145 - Initialized Data

// WZD dseg:6F60
int16_t CRP_AI_Turn_Var = 0;

// WZD dseg:6F62 02 03 04 05 06 07 08 00                         UU_IDK_OVL_AI_7x1 db  2, 3, 4, 5, 6, 7, 8, 0
// WZD dseg:6F6A 00 00                                           dw 0
// WZD dseg:6F6C 00 00                                           dw 0
// WZD dseg:6F6E 0A 00                                           dw 10
// WZD dseg:6F70 0A 00                                           dw 10
// WZD dseg:6F72 32 00                                           dw 50
// WZD dseg:6F74 32 00                                           dw 50

// WZD dseg:6F74                                                 END:  ovr145 - Initialized Data



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


static void AI_Log_Metrics(void)
{
    int16_t player_idx = 0;
    int16_t itr = 0;
    int16_t unit_counts[NUM_PLAYERS];
    int16_t city_counts[NUM_PLAYERS];

    for (player_idx = 0; player_idx < NUM_PLAYERS; player_idx++)
    {
        unit_counts[player_idx] = 0;
        city_counts[player_idx] = 0;
    }

    for (itr = 0; itr < _units; itr++)
    {
        if (_UNITS[itr].owner_idx >= 0 && _UNITS[itr].owner_idx < NUM_PLAYERS)
        {
            unit_counts[_UNITS[itr].owner_idx]++;
        }
    }

    for (itr = 0; itr < _cities; itr++)
    {
        if (_CITIES[itr].owner_idx >= 0 && _CITIES[itr].owner_idx < NUM_PLAYERS)
        {
            city_counts[_CITIES[itr].owner_idx]++;
        }
    }

    dbg_prn("AI_METRICS: Turn %d  Difficulty %d  Players %d\n", _turn, _difficulty, _num_players);

    dbg_prn("AI_METRICS: P0 %-20s units=%-4d cities=%-3d gold=%-5d mana=%-5d (Human)\n",
        _players[HUMAN_PLAYER_IDX].name,
        unit_counts[HUMAN_PLAYER_IDX],
        city_counts[HUMAN_PLAYER_IDX],
        _players[HUMAN_PLAYER_IDX].gold_reserve,
        _players[HUMAN_PLAYER_IDX].mana_reserve);

    for (player_idx = 1; player_idx < _num_players; player_idx++)
    {
        dbg_prn("AI_METRICS: P%d %-20s units=%-4d cities=%-3d gold=%-5d mana=%-5d active=%d\n",
            player_idx,
            _players[player_idx].name,
            unit_counts[player_idx],
            city_counts[player_idx],
            _players[player_idx].gold_reserve,
            _players[player_idx].mana_reserve,
            _FORTRESSES[player_idx].active);
    }

    dbg_prn("AI_METRICS: NPC %-19s units=%-4d cities=%-3d raider_acc=%-3d monster_acc=%-3d\n",
        "(Neutral)",
        unit_counts[NEUTRAL_PLAYER_IDX],
        city_counts[NEUTRAL_PLAYER_IDX],
        _players[NEUTRAL_PLAYER_IDX].casting_cost_original,
        _players[NEUTRAL_PLAYER_IDX].average_unit_cost);
}


// WZD o145p02
// ¿ MoO2  Module: AIDATA  Compute_AI_Data_() ?
void AI_Next_Turn(void)
{
    int i;                  /* _DI_itr_units__peace */
    int player_idx;         /* _SI_itr_players */
    int other_player_idx;   /* _DI_itr_units__peace (re-used) */
    // int16_t itr_players = 0;  // _SI_
    // int16_t itr_units = 0;  // _DI_
    // int16_t itr_players2 = 0;  // _DI_


    /* Unit sanity/bounds check loop */
    for (i = 0; i < _units; i++)
    {
        /* Sanity check Plane (wp) */
        if (_UNITS[i].wp < 0 || _UNITS[i].wp > 1)
        {
            _UNITS[i].wp = 0;
            _UNITS[i].owner_idx = ST_UNDEFINED;
        }

        /* Sanity check World X (wx) */
        if (_UNITS[i].wx < 0 || _UNITS[i].wx >= WORLD_WIDTH)
        {
            _UNITS[i].wx = 0;
            _UNITS[i].owner_idx = ST_UNDEFINED;
        }

        /* Sanity check World Y (wy) */
        if (_UNITS[i].wy < 0 || _UNITS[i].wy >= WORLD_HEIGHT)
        {
            _UNITS[i].wy = 0;
            _UNITS[i].owner_idx = ST_UNDEFINED;
        }
    }

    dbg_prn("AI_TURN: === BEGIN Turn %d ===\n", _turn);

    EMM_Map_DataH();
    Allocate_AI_Data();

    /* Main AI Player processing loop (Skip Human Player 0) */
    for (player_idx = 1; player_idx < _num_players; player_idx++)
    {
        /* Check for Time Stop effect */
        if (g_timestop_player_num != 0)
        {
            if ((g_timestop_player_num - 1) != player_idx)
            {
                dbg_prn("AI_TURN: Player %d (%s) SKIPPED (Time Stop)\n", player_idx, _players[player_idx].name);
                continue;
            }
        }

        /* Check if Wizard is active or returning */
        /* MoO2 ~ `s_PLAYER.eliminated == ST_FALSE` */
        if (_FORTRESSES[player_idx].active != ST_TRUE && _players[player_idx].casting_spell_idx != spl_Spell_Of_Return)
        {
            dbg_prn("AI_TURN: Player %d (%s) SKIPPED (inactive)\n", player_idx, _players[player_idx].name);
            continue;
        }

        /* Update Summoning Circle re-evaluation counter */
        AI_SCircle_Reevals[player_idx]--;

        dbg_prn("AI_TURN: --- Player %d (%s) BEGIN ---\n", player_idx, _players[player_idx].name);

        AI_Evaluate_Hostility(player_idx);

        AI_Magic_Strategy__WIP(player_idx);

        CRP_AI_Turn_Var = 0;

        Player_Hostile_Opponents(player_idx);

        AI_Player_Calculate_Target_Values(player_idx);

        AI_Continent_Eval__WIP(player_idx);

        AI_Pick_Action_Conts__WIP(player_idx);

        /* Handle Overland Casting Completion */
        if (_players[player_idx].casting_cost_remaining <= 0 && _players[player_idx].casting_spell_idx != spl_NONE)
        {
            // Cast_Spell_Overland__WIP(player_idx);
            EMM_Map_DataH();
            _players[player_idx].casting_spell_idx = spl_NONE;
            if (AI_Eval_After_Spell == ST_TRUE)
            {
                Allocate_AI_Data();
                Player_Hostile_Opponents(player_idx);
                AI_Player_Calculate_Target_Values(player_idx);
            }
        }

        /* Handle New Spell Selection if not casting */
        if (_players[player_idx].casting_spell_idx == spl_NONE)
        {
            AI_Spell_Select__STUB(player_idx);
        }

        EMM_Map_DataH();
        AI_Update_Magic_Power(player_idx);
        AI_Sanity_Check_Overland_Enchantments(player_idx);
        AI_Update_Gold_And_Mana_Reserves(player_idx);
        AI_Update_Gold_Income_And_Food_Income(player_idx);

        Some_AI_Turn_Var_2 = 0;
        Some_AI_Turn_Var_3 = 0;

        /* Decrement peace treaties durations */
        for (other_player_idx = 0; other_player_idx < _num_players; other_player_idx++)
        {
            if (_players[player_idx].peace_duration[other_player_idx] != 0)
            {
                _players[player_idx].peace_duration[other_player_idx]--;
            }
        }

        Player_All_Colony_Autobuild(player_idx);
        
        AI_Evaluation_Map(player_idx);
        AI_Continent_Reeval__WIP(player_idx);
        AI_SetUnitOrders__WIP(player_idx);
        
        EMM_Map_DataH();
        AI_Kill_Excess_Settlers_And_Engineers(player_idx);

        dbg_prn("AI_TURN: --- Player %d (%s) END ---\n", player_idx, _players[player_idx].name);
    }

    EMM_Map_DataH();

    /* Reset Move_Failed flag for all non-human units */
    for (i = 0; i < _units; i++)
    {
        if (_UNITS[i].owner_idx != HUMAN_PLAYER_IDX)
        {
            _UNITS[i].Move_Failed = ST_FALSE;
        }
    }

    /* AI Unit Movement Phase */
    dbg_prn("AI_TURN: === Movement Phase ===\n");
    for (player_idx = 1; player_idx < _num_players; player_idx++)
    {
        /* Check for Time Stop effect */
        if (g_timestop_player_num != 0)
        {
            if ((g_timestop_player_num - 1) != player_idx)
            {
                continue;
            }
        }

        /* Check if Wizard is active or returning */
        if (_FORTRESSES[player_idx].active == ST_TRUE || _players[player_idx].casting_spell_idx == spl_Spell_Of_Return)
        {
            dbg_prn("AI_TURN: Moving units for Player %d (%s)\n", player_idx, _players[player_idx].name);
            AI_MoveUnits(player_idx);
        }
    }

    EMM_Map_DataH();

    /* Neutral Player Turn Processing */
    dbg_prn("AI_TURN: === Neutral Player Phase ===\n");
    Player_All_Colony_Autobuild(NEUTRAL_PLAYER_IDX);
    NPC_Farmers();
    dbg_prn("AI_TURN: NPC_Farmers done\n");
    NPC_Destinations();
    dbg_prn("AI_TURN: NPC_Destinations done\n");
    AI_MoveUnits(NEUTRAL_PLAYER_IDX);
    dbg_prn("AI_TURN: NPC movement done\n");

    /* Event Generation */
    Make_Raiders();
    dbg_prn("AI_TURN: Make_Raiders done\n");
    Make_Monsters();
    dbg_prn("AI_TURN: Make_Monsters done\n");

    /* Cleanup and Stasis */
    NPC_Excess_Garrison();
    AI_Hopeless_Stasis();

    AI_Log_Metrics();
    dbg_prn("AI_TURN: === END Turn %d ===\n", _turn);
}


// WZD o145p03
// drake178: AI_Continent_Eval()
/*
evaluates own and enemy strength and target values on
the first 60 continents

BUG: there can be more than 60 continents
*/
/*



*/
void AI_Continent_Eval__WIP(int16_t player_idx)
{
    int16_t Continent_Balance = 0;
    int16_t stack_wp = 0;
    int16_t landmass_idx = 0;
    int16_t itr = 0;  // _DI_
    uint8_t DBG_guard1_count = 0;
    uint8_t DBG_guard2_count = 0;
    uint8_t DBG_guard1_unit_type = 0;
    uint8_t DBG_guard2_unit_type = 0;
    int16_t DBG_guard1_unit_type_cost = 0;
    int16_t DBG_guard2_unit_type_cost = 0;
    int16_t DBG_CRP_AI_Cont_Nme_Str = 0;

    // ; clear the value arrays
    // ; BUG: only supports 60 continents total

    for(itr = 0; itr < NUM_LANDMASSES; itr++)
    {

        AI_Cont_Own_Str[0][itr] = 0;
        AI_Cont_Own_Str[1][itr] = 0;

        CRP_AI_Cont_Nme_Str[0][itr] = 0;
        CRP_AI_Cont_Nme_Str[1][itr] = 0;

        AI_Cont_Own_Val[0][itr] = 0;
        AI_Cont_Own_Val[1][itr] = 0;

        AI_Cont_Nme_Val[0][itr] = 0;
        AI_Cont_Nme_Val[1][itr] = 0;

    }


// ; add the value of owned stacks to their corresponding
// ; continents
// ; BUG: only evaluates the first 60 continents
// ; BUG: the continent IDs are already stored in the
// ;  AI stack data, there's no need to recalculate

    for(itr = 0; itr < AI_Own_Stack_Count; itr++)
    {

        stack_wp = AI_Own_Stacks[itr].wp;

        landmass_idx = _landmasses[((stack_wp * WORLD_SIZE) + (AI_Own_Stacks[itr].wy * WORLD_WIDTH) + AI_Own_Stacks[itr].wx)];

        if(landmass_idx < NUM_LANDMASSES)
        {

            AI_Cont_Own_Str[stack_wp][landmass_idx] += (AI_Own_Stacks[itr].value / 10);

        }

    }


// ; add the value of enemy stacks to their corresponding
// ; continents
// ; 
// ; BUG: only evaluates the first 60 continents

    for(itr = 0; itr < AI_Enemy_Stack_Count; itr++)
    {

        stack_wp = AI_Enemy_Stacks[itr].wp;

        landmass_idx = _landmasses[((stack_wp * WORLD_SIZE) + (AI_Enemy_Stacks[itr].wy * WORLD_WIDTH) + AI_Enemy_Stacks[itr].wx)];

        if(landmass_idx < NUM_LANDMASSES)
        {

            CRP_AI_Cont_Nme_Str[stack_wp][landmass_idx] += (AI_Enemy_Stacks[itr].value / 10);

        }

    }


// ; add the value of encounters (as enemy stacks) to
// ; their corresponding continents
// ; 
// ; BUG: only evaluates the first 60 continents

    for(itr = 0; itr < NUM_LAIRS; itr++)
    {

        if(_LAIRS[itr].intact != ST_TRUE)
        {
            continue;
        }

        stack_wp = _LAIRS[itr].wp;

        landmass_idx = _landmasses[((stack_wp * WORLD_SIZE) + (_LAIRS[itr].wy * WORLD_WIDTH) + _LAIRS[itr].wx)];

        if(landmass_idx < NUM_LANDMASSES)
        {

            // CRP_AI_Cont_Nme_Str[stack_wp][landmass_idx] += ((_LAIRS[itr].guard1_count & 0x0F) * (_unit_type_table[_LAIRS[itr].guard1_unit_type].cost / 10));

            // CRP_AI_Cont_Nme_Str[stack_wp][landmass_idx] += ((_LAIRS[itr].guard2_count & 0x0F) * (_unit_type_table[_LAIRS[itr].guard2_unit_type].cost / 10));

            DBG_guard1_count = _LAIRS[itr].guard1_count;
            DBG_guard2_count = _LAIRS[itr].guard2_count;
            DBG_guard1_unit_type = _LAIRS[itr].guard1_unit_type;
            DBG_guard2_unit_type = _LAIRS[itr].guard2_unit_type;
            DBG_guard1_count = (DBG_guard1_count & 0x0F);
            DBG_guard2_count = (DBG_guard2_count & 0x0F);
            DBG_guard1_unit_type_cost = _unit_type_table[DBG_guard1_unit_type].cost;
            DBG_guard2_unit_type_cost = _unit_type_table[DBG_guard2_unit_type].cost;

            DBG_CRP_AI_Cont_Nme_Str += (DBG_guard1_count * (DBG_guard1_unit_type_cost / 10));
            DBG_CRP_AI_Cont_Nme_Str += (DBG_guard2_count * (DBG_guard2_unit_type_cost / 10));

            // CRP_AI_Cont_Nme_Str[stack_wp][landmass_idx] += DBG_CRP_AI_Cont_Nme_Str;
            DBG_CRP_AI_Cont_Nme_Str = CRP_AI_Cont_Nme_Str[stack_wp][landmass_idx];
            DBG_CRP_AI_Cont_Nme_Str += (DBG_guard1_count * (DBG_guard1_unit_type_cost / 10));
            DBG_CRP_AI_Cont_Nme_Str += (DBG_guard2_count * (DBG_guard2_unit_type_cost / 10));
            CRP_AI_Cont_Nme_Str[stack_wp][landmass_idx] = DBG_CRP_AI_Cont_Nme_Str;

        }

    }


// ; add the value of cities to their corresponding
// ; continent tables
// ; 
// ; BUG: only evaluates the first 60 continents

    for(itr = 0; itr < _cities; itr++)
    {

        stack_wp = _CITIES[itr].wp;

        landmass_idx = _landmasses[((stack_wp * WORLD_SIZE) + (_CITIES[itr].wy * WORLD_WIDTH) + _CITIES[itr].wx)];

        if(landmass_idx < NUM_LANDMASSES)
        {

            AI_Cont_Own_Val[stack_wp][landmass_idx] = AI_Own_City_Values[itr];

            AI_Cont_Nme_Val[stack_wp][landmass_idx] = AI_Enemy_City_Values[itr];

        }

    }


// ; add the value of nodes to their corresponding
// ; continent tables
// ; 
// ; BUG: only evaluates the first 60 continents

    for(itr = 0; itr < NUM_NODES; itr++)
    {

        stack_wp = _NODES[itr].wp;

        landmass_idx = _landmasses[((stack_wp * WORLD_SIZE) + (_NODES[itr].wy * WORLD_WIDTH) + _NODES[itr].wx)];

        if(landmass_idx < NUM_LANDMASSES)
        {

            if(_NODES[itr].owner_idx == player_idx)
            {

                AI_Cont_Own_Val[stack_wp][landmass_idx] = (_NODES[itr].power * _magic);

            }
            else
            {

                AI_Cont_Nme_Val[stack_wp][landmass_idx] = (_NODES[itr].power * _magic);

            }

        }

    }


// ; replaces the enemy strength values for each continent
// ; with a balance value: 
// ; own str - enemy str - (own value / 10); -25 if above
// ; 0, but set to 0 if this would reduce it below
// ; (negatives are stored as is)
// ; 
// ; BUG: only evaluates the first 60 continents

    for(stack_wp = 0; stack_wp < 2; stack_wp++)
    {

        for(itr = 0; itr < NUM_LANDMASSES; itr++)
        {

            Continent_Balance = (AI_Cont_Own_Str[stack_wp][landmass_idx] - (AI_Cont_Own_Val[stack_wp][landmass_idx] / 10) - CRP_AI_Cont_Nme_Str[stack_wp][landmass_idx]);

            if(Continent_Balance > 0)
            {

                Continent_Balance -= 25;

                if(Continent_Balance < 0)
                {

                    Continent_Balance = 0;

                }

            }

            // ; own str - enemy str - own value
            AI_ContBalances[stack_wp][landmass_idx] = Continent_Balance;

        }

    }


}


// WZD o145p04
// drake178: AI_Power_Distrib()
/*
sets the AI's power distribution percentages based
on objective, magic strategy, current skill & mana,
spells left to research, and spell being cast

contains multiple BUGs that prohibit this function
from working properly once the AI has ran out of
spells to research
*/
/*

~ Players_Update_Magic_Power()

"...available magic power is divided between researching, reserves and spell casting skill..."
"...spell casting skill..." ... "...total current spell casting skill..."
"...adjust how much of your total magic power you allocate to your mana reserves, researching efforts and spell casting skill..."

*/
void AI_Update_Magic_Power(int16_t player_idx)
{
    int16_t Mana_Total = 0;
    int16_t Nominal_Skill = 0;
    int16_t itr = 0;
    int16_t Unresearched_Spells = 0;
    int16_t itr_spells = 0;
    int16_t itr_realms = 0;
    int16_t IDK = 0;  // _DI_

    Nominal_Skill = Player_Base_Casting_Skill(player_idx);

    Mana_Total = _players[player_idx].mana_reserve;

    _players[player_idx].reevaluate_magic_power_countdown -= 1;

    if(_players[player_idx].reevaluate_magic_power_countdown)
    {
        return;
    }

    _players[player_idx].reevaluate_magic_power_countdown = (15 + Random(10));

    // ; check the wizard's spell library to see if there are
    // ; any spells still marked as "knowable"
    // ; 
    // ; BUG: spells that are already research
    // ; candidates are ignored by this loop

    Unresearched_Spells = 0;

    for(itr_realms = 0; ((itr_realms < NUM_MAGIC_REALMS) & (Unresearched_Spells == 0)); itr_realms++)
    {

        for(itr_spells = 0; ((itr_spells < NUM_SPELLS_PER_MAGIC_REALM) & (Unresearched_Spells == 0)); itr_spells++)
        {

            if(_players[player_idx].spells_list[((itr_realms * NUM_SPELLS_PER_MAGIC_REALM) + itr_spells)] == 2 /* S_Knowable */)
            {

                Unresearched_Spells++;

            }

        }

    }

    // ; BUG: branching this here removes the AI's ability
    // ; to adjust power distribution entirely when all
    // ; spells have been researched, including the intended
    // ; modifiers for researching and casting the SoM

    if(Unresearched_Spells == 0)
    {

        // ; BUG: should be added, not replacing

        _players[player_idx].skill_ratio = (_players[player_idx].research_ratio / 2);

        _players[player_idx].research_ratio = 0;

    }
    else
    {

        _players[player_idx].research_ratio = 10;

        _players[player_idx].skill_ratio = 10;

        _players[player_idx].mana_ratio = 30;

        for(itr = 0; itr < 5; itr++)
        {

            IDK = (Random(3) - 1);

            if(
                (_players[player_idx].Objective == OBJ_Theurgist)
                &&
                (Random(3) == 1)
            )
            {
                IDK = 2;
            }

            if(
                (_players[player_idx].Objective == OBJ_Militarist)
                &&
                (Random(3) == 1)
            )
            {
                IDK = 1;
            }

            if(
                (_players[player_idx].Objective == OBJ_Perfectionist)
                &&
                (Random(3) == 1)
            )
            {
                IDK = 0;
            }

            if(
                (Player_Base_Casting_Skill(player_idx) > 100)
                &&
                (Random(3) == 1)
            )
            {

                IDK = 2;  // same as OBJ_Theurgist

            }

            if(
                (_players[player_idx].alchemy != 0)
                &&
                (Random(3) == 1)
            )
            {

                IDK = 0;  // same as OBJ_Perfectionist

            }

            if(
                (_players[player_idx].Magic_Strategy == 6)
                &&
                (Random(3) == 1)
            )
            {

                IDK = 0;  // same as OBJ_Perfectionist

            }

            if(
                (_players[player_idx].Magic_Strategy == 1)
                &&
                (Random(3) == 1)
            )
            {

                IDK = 2;  // same as OBJ_Theurgist

            }

            if(
                (_players[player_idx].Magic_Strategy == 2)
                &&
                (Random(3) == 1)
            )
            {

                IDK = 1;  // same as OBJ_Militarist

            }

            if(
                (_players[player_idx].mana_reserve < (_turn * 3))
                &&
                (Random(2) == 1)
            )
            {

                IDK = 0;  // same as OBJ_Perfectionist

            }

            if(_players[player_idx].mana_reserve > 1000)
            {

                if(Random(2) == 1)
                {

                    IDK = 2;  // same as OBJ_Theurgist

                }
                else
                {

                    IDK = 1;  // same as OBJ_Militarist

                }

            }

            if(
                (IDK == 1)
                &&
                ((Mana_Total * 4) < Nominal_Skill)
            )
            {

                IDK = 0;  // same as OBJ_Perfectionist

            }

            switch(IDK)
            {

                case 0:
                {

                    _players[player_idx].mana_ratio += 10;

                } break;

                case 1:
                {

                    _players[player_idx].skill_ratio += 10;

                } break;

                case 2:
                {

                    _players[player_idx].research_ratio += 10;

                } break;
                
            }

        }

        // ; BUG: should be Spell_Of_Mastery
        if(_players[player_idx].researching_spell_idx == spl_Fire_Elemental)
        {

            _players[player_idx].research_ratio = 70;

            _players[player_idx].skill_ratio = 10;

            _players[player_idx].mana_ratio = 20;

        }

        if(_turn < 30)
        {

            _players[player_idx].research_ratio = 35;

            _players[player_idx].skill_ratio = 25;

            _players[player_idx].mana_ratio = 40;

        }

        _players[player_idx].mana_ratio = ((100 - _players[player_idx].skill_ratio) - _players[player_idx].research_ratio);

    }

}


// WZD o145p05
// drake178: WIZ_IsSpellKnown()
/*
; returns 1 if the spell is known, or 0 otherwise
*/
/*



*/
int16_t Player_Knows_Spell(int16_t player_idx, int16_t spell_idx)
{

    if(
        (spell_idx == 0)
        ||
        (_players[player_idx].spells_list[ ( (((spell_idx - 1) / NUM_SPELLS_PER_MAGIC_REALM) * NUM_SPELLS_PER_MAGIC_REALM) + ((spell_idx - 1) % NUM_SPELLS_PER_MAGIC_REALM) ) ] == 2 /* S_Known */)
    )
    {

        return ST_FALSE;

    }
    else
    {

        return ST_TRUE;

    }

}


// WZD o145p06
// drake178: WIZ_KnownSpellCount()
/*
; returns the amount of spells known by the wizard
*/
/*



*/
int16_t Player_Known_Spell_Count(int16_t player_idx)
{
    int16_t itr_realms = 0;  // _CX_
    int16_t known_spell_count = 0;  // _DI_
    int16_t itr_spells = 0;  // _SI_

    known_spell_count = 0;

    for(itr_realms = 0; itr_realms < NUM_MAGIC_REALMS; itr_realms++)
    {

        for(itr_spells = 0; itr_spells < NUM_SPELLS_PER_MAGIC_REALM; itr_spells++)
        {

            if(_players[player_idx].spells_list[((itr_realms * NUM_SPELLS_PER_MAGIC_REALM) + itr_spells)] == 2 /* S_Known */)
            {

                known_spell_count++;

            }

        }

    }

    return known_spell_count;

}


// WZD o145p07
// drake178: AI_OVL_Eval_Assets()
/*
evaluates all overland cities and units, creating
tables for own stack capabilities, own and enemy
stack and garrison values, and city values

contains a host of BUGs, needs re-evaluating later!
*/
/*

sets values for own and enemy cities, stacks, garrisons
¿ used to calculate landmass values ?
¿ used to choose targets for positive and negative city (area) spells ?


*/
void AI_Player_Calculate_Target_Values(int16_t player_idx)
{
    int16_t City_Loop_Var = 0;
    int16_t unit_has_watertravel = 0;
    int16_t unit_has_melding = 0;
    int16_t unit_is_hero = 0;
    int16_t unit_has_windwalking = 0;
    int16_t unit_has_airtravel = 0;
    int16_t effective_unit_strength = 0;
    int16_t Processed_Unit = 0;
    int16_t Loop_Var = 0;
    int16_t city_owner_idx = 0;
    int16_t itr_cities = 0;
    int16_t itr_units = 0;  // _DI_

    // ; assign target values to all owned and hostile cities
    // ; based on population, building count, and fortress
    // ; presence
    // ; BUG: considers the generic race (0x0E) instead of
    // ;  trolls (0x0D) for doubling the population value
    // ; BUG: randomly doubles the value of all enemy cities
    // ;  instead of those of the human player

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {

        AI_NME_Garrison_Strs[itr_cities] = 0;

        AI_Own_Garr_Strs[itr_cities] = 0;

        AI_Enemy_City_Values[itr_cities] = 0;

        AI_Own_City_Values[itr_cities] = 0;

        city_owner_idx = _CITIES[itr_cities].owner_idx;

        if(city_owner_idx != player_idx)
        {

            if(
                (
                    (_players[player_idx].Hostility[city_owner_idx] >= 2)  // attitude is hostile
                    &&
                    (_players[player_idx].peace_duration[city_owner_idx] == 0)  // reguardless of any treaty
                )
                ||
                (city_owner_idx == NEUTRAL_PLAYER_IDX)
            )
            {

                AI_Enemy_City_Values[itr_cities] = _CITIES[itr_cities].population;

                if(
                    (_CITIES[itr_cities].race == rt_Beastmen)
                    ||
                    (_CITIES[itr_cities].race == rt_Dark_Elf)
                    ||
                    (_CITIES[itr_cities].race == rt_Draconian)
                    ||
                    (_CITIES[itr_cities].race == rt_Dwarf)
                    ||
                    (_CITIES[itr_cities].race != rt_Standard)  /* BUGBUG  should be rt_Trolls */
                )
                {

                    AI_Enemy_City_Values[itr_cities] *= 2;

                }

                AI_Enemy_City_Values[itr_cities] += _CITIES[itr_cities].bldg_cnt;

                if(
                    (_FORTRESSES[city_owner_idx].wx == _CITIES[itr_cities].wx)
                    &&
                    (_FORTRESSES[city_owner_idx].wy == _CITIES[itr_cities].wy)
                    &&
                    (_FORTRESSES[city_owner_idx].wp == _CITIES[itr_cities].wp)
                    &&
                    (city_owner_idx != NEUTRAL_PLAYER_IDX)
                )
                {

                    AI_Enemy_City_Values[itr_cities] += 200;

                }
        
                if(_CITIES[itr_units].owner_idx == HUMAN_PLAYER_IDX)
                {

                    AI_Enemy_City_Values[itr_cities] *= 2;

                }

            }

        }
        else
        {

            AI_Own_City_Values[itr_cities] = _CITIES[itr_cities].population;

            if(
                (_CITIES[itr_cities].race == rt_Beastmen)
                ||
                (_CITIES[itr_cities].race == rt_Dark_Elf)
                ||
                (_CITIES[itr_cities].race == rt_Draconian)
                ||
                (_CITIES[itr_cities].race == rt_Dwarf)
                ||
                (_CITIES[itr_cities].race != rt_Standard)  /* BUGBUG  should be rt_Trolls */
            )
            {

                AI_Enemy_City_Values[itr_cities] *= 2;

            }

            AI_Enemy_City_Values[itr_cities] += _CITIES[itr_cities].bldg_cnt;

            if(
                (_FORTRESSES[player_idx].wx == _CITIES[itr_cities].wx)
                &&
                (_FORTRESSES[player_idx].wy == _CITIES[itr_cities].wy)
                &&
                (_FORTRESSES[player_idx].wp == _CITIES[itr_cities].wp)
            )
            {

                AI_Enemy_City_Values[itr_cities] += _turn;

            }

        }

    }


    AI_Own_Stack_Count = 0;

    AI_Enemy_Stack_Count = 0;

    for(itr_units = 0; itr_units < _units; itr_units++)
    {

        /* HACK */  if(
        /* HACK */      (_UNITS[itr_units].owner_idx == ST_UNDEFINED)
        /* HACK */      ||
        /* HACK */      (_UNITS[itr_units].wp == ST_UNDEFINED)
        /* HACK */  )
        /* HACK */  {
        /* HACK */      continue;
        /* HACK */  }

        effective_unit_strength = Effective_Unit_Strength(itr_units);

        unit_has_airtravel = Unit_Has_AirTravel(itr_units);

        unit_has_windwalking = Unit_Has_WindWalking(itr_units);

        unit_has_watertravel = Unit_Has_WaterTravel(itr_units);

        if(_UNITS[itr_units].type < ut_BarbSwordsmen)  /* ¿ hero or boat ? */ /* ; BUG? ignores Barbarian Spearmen */
        {

            unit_is_hero = ST_TRUE;

        }
        else
        {

            unit_is_hero = ST_FALSE;

        }

        if((_unit_type_table[_UNITS[itr_units].type].Abilities & UA_MELD) != 0)
        {

            unit_has_melding = ST_TRUE;

        }
        else
        {

            unit_has_melding = ST_FALSE;

        }

        if(_UNITS[itr_units].owner_idx != player_idx)
        {

            if(AI_Enemy_Stack_Count < 240)
            {

                Processed_Unit = ST_FALSE;

                for(Loop_Var = 0; ((Loop_Var < _cities) & (Processed_Unit == ST_FALSE)); Loop_Var++)
                {

                    if(
                        (_UNITS[itr_units].wx == _CITIES[Loop_Var].wx)
                        &&
                        (_UNITS[itr_units].wy == _CITIES[Loop_Var].wy)
                        &&
                        (_UNITS[itr_units].wp == _CITIES[Loop_Var].wp)
                    )
                    {

                        Processed_Unit = ST_TRUE;

                    }

                }


                for(Loop_Var = 0; ((Loop_Var < AI_Enemy_Stack_Count) & (Processed_Unit != ST_FALSE)); Loop_Var++)
                {

                    if(
                        (_UNITS[itr_units].wx == AI_Enemy_Stacks[Loop_Var].wx)
                        &&
                        (_UNITS[itr_units].wy == AI_Enemy_Stacks[Loop_Var].wy)
                        &&
                        (_UNITS[itr_units].wp == AI_Enemy_Stacks[Loop_Var].wp)
                    )
                    {

                        if(_unit_type_table[_UNITS[itr_units].type].Transport > 0)
                        {

                            effective_unit_strength = 0;

                        }

                        if((_unit_type_table[_UNITS[itr_units].type].Abilities & UA_CREATEOUTPOST) != 0)
                        {

                            effective_unit_strength = 0;

                        }

                        if(_unit_type_table[_UNITS[itr_units].type].Construction > 0)
                        {

                            effective_unit_strength = 0;

                        }

                        AI_Enemy_Stacks[Loop_Var].value = effective_unit_strength;

                        Processed_Unit = ST_TRUE;

                    }


                }


                if(Processed_Unit == ST_FALSE)
                {

                    AI_Enemy_Stacks[AI_Enemy_Stack_Count].wx = _UNITS[itr_units].wx;

                    AI_Enemy_Stacks[AI_Enemy_Stack_Count].wy = _UNITS[itr_units].wy;

                    AI_Enemy_Stacks[AI_Enemy_Stack_Count].wp = _UNITS[itr_units].wp;

                    if(_unit_type_table[_UNITS[itr_units].type].Transport > 0)
                    {

                        effective_unit_strength = 0;

                    }

                    if((_unit_type_table[_UNITS[itr_units].type].Abilities & UA_CREATEOUTPOST) != 0)
                    {

                        effective_unit_strength = 0;

                    }

                    if(_unit_type_table[_UNITS[itr_units].type].Construction > 0)
                    {

                        effective_unit_strength = 0;

                    }

                    AI_Enemy_Stacks[AI_Enemy_Stack_Count].value = effective_unit_strength;

                    AI_Enemy_Stack_Count++;

                }

                for(Loop_Var = 0; Loop_Var < _cities; Loop_Var++)
                {

                    if(_UNITS[itr_units].wp == _CITIES[Loop_Var].wp)
                    {
                        if(
                            (_UNITS[itr_units].wx == _CITIES[Loop_Var].wx)
                            &&
                            (_UNITS[itr_units].wy == _CITIES[Loop_Var].wy)
                        )
                        {

                            AI_NME_Garrison_Strs[Loop_Var] += effective_unit_strength;

                        }
                        else
                        {

                            if(
                                (abs(_UNITS[itr_units].wx - _CITIES[Loop_Var].wx) < 3)
                                &&
                                (abs(_UNITS[itr_units].wy - _CITIES[Loop_Var].wy) < 3)
                            )
                            {

                                AI_NME_Garrison_Strs[Loop_Var] += (effective_unit_strength / 2);

                            }

                        }

                    }

                }

            }

        }
        else
        {

            if(AI_Own_Stack_Count < 80)
            {

                Processed_Unit = ST_FALSE;

                for(Loop_Var = 0; ((Loop_Var < AI_Own_Stack_Count) & (Processed_Unit == ST_FALSE)); Loop_Var++)
                {


                    if(
                        (_UNITS[itr_units].wx == AI_Own_Stacks[Loop_Var].wx)
                        &&
                        (_UNITS[itr_units].wy == AI_Own_Stacks[Loop_Var].wy)
                        &&
                        (_UNITS[itr_units].wp == AI_Own_Stacks[Loop_Var].wp)
                    )
                    {

                        AI_Own_Stacks[Loop_Var].unit_count += 1;

                        if(_unit_type_table[_UNITS[itr_units].type].Transport > 0)
                        {

                            AI_Own_Stacks[Loop_Var].abilities |= AICAP_Transport;

                            AI_Own_Stacks[Loop_Var].transport_capacity += _unit_type_table[_UNITS[itr_units].type].Transport;

                            effective_unit_strength = 0;

                        }
                        else
                        {

                            // subtract boatrider from capacity
                            if(
                                (unit_has_watertravel == ST_FALSE)
                                &&
                                (unit_has_airtravel == ST_FALSE)
                            )
                            {

                                AI_Own_Stacks[Loop_Var].transport_capacity -= 1;

                            }

                        }

                        if(_unit_type_table[_UNITS[itr_units].type].Construction > 0)
                        {

                            AI_Own_Stacks[Loop_Var].abilities |= AICAP_RoadBuild;

                            effective_unit_strength = 0;

                        }

                        if((_unit_type_table[_UNITS[itr_units].type].Abilities & UA_PURIFY) != 0)
                        {

                            AI_Own_Stacks[Loop_Var].abilities |= AICAP_Purify;

                        }

                        if((_unit_type_table[_UNITS[itr_units].type].Abilities & UA_CREATEOUTPOST) != 0)
                        {

                            AI_Own_Stacks[Loop_Var].abilities |= AICAP_Settler;

                            effective_unit_strength = 0;

                        }

                        if(
                            (player_idx == NEUTRAL_PLAYER_IDX)
                            &&
                            ((_unit_type_table[_UNITS[itr_units].type].Abilities & UA_FANTASTIC) != 0)
                        )
                        {

                            AI_Own_Stacks[Loop_Var].abilities |= AICAP_Settler;

                            effective_unit_strength = 0;

                        }

                        // BUGBUG double checking transport capacity
                        if(_unit_type_table[_UNITS[itr_units].type].Transport > 0)
                        {

                            AI_Own_Stacks[Loop_Var].abilities |= AICAP_Transport;

                            AI_Own_Stacks[Loop_Var].transport_capacity += _unit_type_table[_UNITS[itr_units].type].Transport;

                            effective_unit_strength = 0;

                        }

                        if(
                            (unit_has_airtravel == ST_FALSE)
                            &&
                            (unit_has_watertravel == ST_FALSE)
                        )
                        {

                            AI_Own_Stacks[Loop_Var].abilities ^= AICAP_LandOnly;

                        }

                        if(unit_has_windwalking == ST_TRUE)
                        {

                            AI_Own_Stacks[Loop_Var].abilities |= AICAP_WindWalk;

                            AI_Own_Stacks[Loop_Var].transport_capacity = 8;

                        }

                        if(unit_is_hero == ST_TRUE)
                        {

                            AI_Own_Stacks[Loop_Var].abilities |= AICAP_Hero;

                            effective_unit_strength *= 2;

                        }

                        if(unit_has_melding == ST_TRUE)
                        {

                            AI_Own_Stacks[Loop_Var].abilities |= AICAP_Melding;

                        }

                        // BUGBUG double checking construction
                        if(_unit_type_table[_UNITS[itr_units].type].Construction > 0)
                        {

                            AI_Own_Stacks[Loop_Var].abilities |= AICAP_RoadBuild;

                            effective_unit_strength = 0;

                        }

                        AI_Own_Stacks[Loop_Var].value += effective_unit_strength;

                        if(_UNITS[itr_units].Status == us_BuildRoad)
                        {

                            AI_Own_Stacks[Loop_Var].unit_status = us_BuildRoad;

                        }
                        else
                        {

                            if(
                                (AI_Own_Stacks[Loop_Var].unit_status != us_BuildRoad)
                                &&
                                (_UNITS[itr_units].Status == us_GOTO)
                            )
                            {

                                AI_Own_Stacks[Loop_Var].unit_status = us_GOTO;

                            }

                        }
    
                        Processed_Unit = ST_TRUE;

                    }

                }

                if(Processed_Unit == ST_FALSE)
                {

                    AI_Own_Stacks[AI_Own_Stack_Count].wx = _UNITS[itr_units].wx;

                    AI_Own_Stacks[AI_Own_Stack_Count].wy = _UNITS[itr_units].wy;

                    AI_Own_Stacks[AI_Own_Stack_Count].wp = _UNITS[itr_units].wp;

                    AI_Own_Stacks[AI_Own_Stack_Count].abilities = AICAP_LandOnly;

                    AI_Own_Stacks[AI_Own_Stack_Count].value = 0;

                    AI_Own_Stacks[AI_Own_Stack_Count].transport_capacity = 0;

                    AI_Own_Stacks[AI_Own_Stack_Count].unit_count = 1;

                    AI_Own_Stacks[AI_Own_Stack_Count].landmass_idx = 0;

                    AI_Own_Stacks[AI_Own_Stack_Count].landmass_idx = _landmasses[((_UNITS[itr_units].wp * WORLD_SIZE) + (_UNITS[itr_units].wy * WORLD_WIDTH) + _UNITS[itr_units].wx)];

                    if(_unit_type_table[_UNITS[itr_units].type].Transport > 0)
                    {

                        AI_Own_Stacks[AI_Own_Stack_Count].abilities |= AICAP_Transport;

                        AI_Own_Stacks[AI_Own_Stack_Count].transport_capacity += _unit_type_table[_UNITS[itr_units].type].Transport;

                        effective_unit_strength = 0;

                    }
                    else
                    {

                        if(
                            (unit_has_watertravel == ST_FALSE)
                            &&
                            (unit_has_airtravel == ST_FALSE)
                        )
                        {

                            AI_Own_Stacks[AI_Own_Stack_Count].transport_capacity -= 1;

                        }

                    }

                    if(_unit_type_table[_UNITS[itr_units].type].Construction > 0)
                    {

                        AI_Own_Stacks[AI_Own_Stack_Count].abilities |= AICAP_RoadBuild;

                        effective_unit_strength = 0;

                    }

                    if((_unit_type_table[_UNITS[itr_units].type].Abilities & UA_PURIFY) != 0)
                    {

                        AI_Own_Stacks[AI_Own_Stack_Count].abilities |= AICAP_Purify;

                    }

                    if((_unit_type_table[_UNITS[itr_units].type].Abilities & UA_CREATEOUTPOST) != 0)
                    {

                        AI_Own_Stacks[AI_Own_Stack_Count].abilities |= AICAP_Settler;

                        effective_unit_strength = 0;

                    }

                    if(
                        (unit_has_airtravel == ST_FALSE)
                        &&
                        (unit_has_watertravel == ST_FALSE)
                    )
                    {

                        AI_Own_Stacks[AI_Own_Stack_Count].abilities ^= AICAP_LandOnly;

                    }

                    if(unit_has_windwalking == ST_TRUE)
                    {

                        AI_Own_Stacks[AI_Own_Stack_Count].abilities |= AICAP_WindWalk;

                        AI_Own_Stacks[AI_Own_Stack_Count].transport_capacity = 8;

                    }

                    if(unit_is_hero == ST_TRUE)
                    {

                        AI_Own_Stacks[AI_Own_Stack_Count].abilities |= AICAP_Hero;

                        /* ¿ BUGBUG does not do `effective_unit_strength *= 2;` ? */

                    }

                    if(unit_has_melding == ST_TRUE)
                    {

                        AI_Own_Stacks[AI_Own_Stack_Count].abilities |= AICAP_Melding;

                    }

                    AI_Own_Stacks[AI_Own_Stack_Count].value += effective_unit_strength;

                    AI_Own_Stacks[AI_Own_Stack_Count].unit_status = _UNITS[itr_units].Status;

                    if(_UNITS[itr_units].Status == us_BuildRoad)
                    {

                        AI_Own_Stacks[AI_Own_Stack_Count].unit_status = us_BuildRoad;

                    }
                    else
                    {

                        if(
                            (AI_Own_Stacks[AI_Own_Stack_Count].unit_status != us_BuildRoad)
                            &&
                            (_UNITS[itr_units].Status == us_GOTO)
                        )
                        {

                            AI_Own_Stacks[AI_Own_Stack_Count].unit_status = us_GOTO;

                        }

                    }

                    if(player_idx != NEUTRAL_PLAYER_IDX)
                    {

                        AI_Own_Stack_Count++;

                    }
                    else
                    {

                        Processed_Unit = ST_FALSE;

                        if((AI_Own_Stacks[AI_Own_Stack_Count].abilities & AICAP_Settler) == 0)
                        {

                            Processed_Unit = ST_TRUE;
                            
                        }

                        for(City_Loop_Var = 0; ((City_Loop_Var < _cities) && (Processed_Unit == ST_FALSE)); City_Loop_Var++)
                        {

                            if(
                            (_CITIES[City_Loop_Var].wx == AI_Own_Stacks[AI_Own_Stack_Count].wx)
                            &&
                            (_CITIES[City_Loop_Var].wy == AI_Own_Stacks[AI_Own_Stack_Count].wy)
                            &&
                            (_CITIES[City_Loop_Var].wp == AI_Own_Stacks[AI_Own_Stack_Count].wp)
                            )
                            {

                                Processed_Unit = ST_TRUE;

                            }

                        }

                        if(Processed_Unit == ST_FALSE)
                        {

                            AI_Own_Stack_Count++;

                        }

                    }

                    for(Loop_Var = 0; Loop_Var < _cities; Loop_Var++)
                    {

                        if(
                            (_UNITS[itr_units].wx == _CITIES[Loop_Var].wx)
                            &&
                            (_UNITS[itr_units].wy == _CITIES[Loop_Var].wy)
                            &&
                            (_UNITS[itr_units].wp == _CITIES[Loop_Var].wp)
                        )
                        {

                            AI_Own_Garr_Strs[Loop_Var] += effective_unit_strength;

                        }

                    }

                }

            }

        }

    }


    // ; perform a hostility-based doubling of enemy garrison
    // ; values... or that's what should happen I think
    // ; BUG: uses 3 different sets of variables that have no
    // ;  connection to each other whatsoever
    // ¿ BUGBUG  checks hostile, not hostile or wose ?
    for(itr_units = 0; itr_units < AI_Enemy_Stack_Count; itr_units++)
    {

        if(_players[player_idx].Hostility[city_owner_idx] == 2)  /* ; BUG: variable not set up */
        {

            AI_NME_Garrison_Strs[itr_units] *= 2;

        }

    }


    // ; remove AICAP_LandOnly from any owned stacks that also
    // ; have AICAP_WindWalk
    for(itr_units = 0; itr_units < AI_Own_Stack_Count; itr_units++)
    {

        if((AI_Own_Stacks[itr_units].abilities & AICAP_WindWalk) != 0)
        {

            AI_Own_Stacks[Loop_Var].abilities |= AICAP_LandOnly;

        }

    }


    // ; count the transport units into a global variable
    ai_transport_count = 0;

    for(itr_units = 0; itr_units < _units; itr_units++)
    {

        if(
            (_UNITS[itr_units].owner_idx == player_idx)
            &&
            (_unit_type_table[_UNITS[itr_units].type].Transport > 0)
        )
        {

            ai_transport_count++;

        }

    }

}


// WZD o145p08
// drake178: AI_Count_Active_Wars()
/*
; counts the wars the player is currently in, storing
; both the amount and the target players into their
; corresponding global variables
*/
/*



*/
void Player_Hostile_Opponents(int16_t player_idx)
{
    int16_t itr_players = 0;  // _CX_

    _cp_hostile_opponent_count = 0;

    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {

        if(
            (_players[player_idx].Hostility[itr_players] >= 2)
            &&
            (_players[player_idx].peace_duration[itr_players] == 0)  /* DEDU  Q: Why is it checking peace_duration for non-peace statuses? ¿ A: because this is *just* 'attackable wizards' ? */
            &&
            (player_idx != itr_players)
        )
        {

            _cp_hostile_opponents[_cp_hostile_opponent_count] = itr_players;

            _cp_hostile_opponent_count++;

        }

    }

}


// WZD o145p09
// drake178: sub_D405F()

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
// drake178: AI_Alchemy()
/*
converts the player's gold into mana or vice versa
based on a set of criteria

BUG: when banished, converted gold replaces existing
mana instead of being added to it
*/
/*

rearranges gold and mana

*/
void AI_Update_Gold_And_Mana_Reserves(int16_t player_idx)
{
    int16_t Desired_Mana = 0;
    int16_t Desired_Gold = 0;
    int16_t Resource_Total = 0;

    if(_players[player_idx].casting_spell_idx == spl_Spell_Of_Return)
    {

        // ; convert all gold into mana
        // ; 
        // ; BUG: assumes no mana to begin with, overwriting it
        // ; instead of adding to it

        if(_players[player_idx].alchemy == 1)
        {

            _players[player_idx].mana_reserve = _players[player_idx].gold_reserve;  // ; BUG: assumes that the wizard had no mana originally

        }
        else
        {
            
            _players[player_idx].mana_reserve = (_players[player_idx].gold_reserve / 2);  // ; BUG: assumes that the wizard had no mana originally

        }

        _players[player_idx].gold_reserve = 0;

    }
    else
    {

        if(_players[player_idx].alchemy != 1)
        {

            if(_players[player_idx].mana_reserve == 0)
            {

                _players[player_idx].mana_reserve = (_players[player_idx].gold_reserve / 4);

                _players[player_idx].gold_reserve = (_players[player_idx].gold_reserve / 2);

            }

            if(Random(10) == 1)
            {
    
                if(_players[player_idx].mana_reserve >= (_players[player_idx].gold_reserve / 5))
                {

                    if(_players[player_idx].gold_reserve >= (_players[player_idx].mana_reserve / 4))
                    {

                        if(
                            (_turn < 100)
                            &&
                            _players[player_idx].mana_reserve > 50
                        )
                        {

                            Desired_Gold = _players[player_idx].gold_reserve;

                            Desired_Mana = _players[player_idx].mana_reserve;

                            Desired_Gold = (Desired_Mana / 4);

                            Desired_Mana = (Desired_Mana - (Desired_Mana / 2));

                            SETMAX(Desired_Mana, MAX_MANA_RESERVE);

                            SETMAX(Desired_Gold, MAX_GOLD_RESERVE);

                            _players[player_idx].gold_reserve = Desired_Gold;

                            _players[player_idx].mana_reserve = Desired_Mana;

                        }

                    }

                }
                else
                {

                    if(_players[player_idx].gold_reserve > 100)
                    {

                        Desired_Gold = _players[player_idx].gold_reserve;

                        Desired_Mana = _players[player_idx].mana_reserve;

                        Desired_Mana = (Desired_Gold / 4);

                        Desired_Gold = (Desired_Gold - (Desired_Gold / 2));

                        SETMAX(Desired_Mana, MAX_MANA_RESERVE);

                        SETMAX(Desired_Gold, MAX_GOLD_RESERVE);

                        _players[player_idx].gold_reserve = Desired_Gold;

                        _players[player_idx].mana_reserve = Desired_Mana;

                    }

                }

            }

        }
        else
        {

            // want more gold or more mana
            if(_turn >= 150)
            {

                Resource_Total = (_players[player_idx].gold_reserve + _players[player_idx].mana_reserve);

                Desired_Mana = (Resource_Total * 7 / 10);  // mana = total * .7

                SETMAX(Desired_Mana, MAX_MANA_RESERVE);

                Desired_Gold = (Resource_Total - Desired_Mana);

                SETMAX(Desired_Gold, MAX_GOLD_RESERVE);

            }
            else
            {

                Resource_Total = (_players[player_idx].gold_reserve + _players[player_idx].mana_reserve);

                Desired_Gold = (Resource_Total * 9 / 10);  // gold = total * .9

                SETMAX(Desired_Gold, MAX_GOLD_RESERVE);

                Desired_Mana = (Resource_Total - Desired_Gold);


                SETMAX(Desired_Mana, MAX_MANA_RESERVE);

            }

            _players[player_idx].gold_reserve = Desired_Gold;

            _players[player_idx].mana_reserve = Desired_Mana;

        }

    }

}


// WZD o145p12
// drake178: AI_Tax_And_Farmers()
/*
sets the tax rate for the player and, if food
production is below 0, makes 50 attempts at
converting a random worker into a farmer

BUG: treats all farmers as generating 2 food
*/
/*

updates tax_rate (∴ gold_income)
updates food_income

*/
void AI_Update_Gold_Income_And_Food_Income(int16_t player_idx)
{
    int16_t food_income = 0;
    int16_t mana_income = 0;
    int16_t gold_income = 0;
    int16_t Tries = 0;
    int16_t UU_Local_0_3 = 0;
    int16_t UU_Local_0_2 = 0;
    int16_t UU_Local_0_1 = 0;
    int16_t city_idx = 0;  // _DI_

    UU_Local_0_1 = 0;
    UU_Local_0_3 = 0;
    UU_Local_0_2 = 0;

    Player_Resource_Income_Total(player_idx, &gold_income, &food_income, &mana_income);

    if(
        (((_turn / 50) * 75) > gold_income)
        &&
        (_players[player_idx].tax_rate < 6)
    )
    {

        _players[player_idx].tax_rate += 1;

    }

    Player_Resource_Income_Total(player_idx, &gold_income, &food_income, &mana_income);

    if(
        (gold_income > 200)
        &&
        (_players[player_idx].tax_rate > 1)
    )
    {

        _players[player_idx].tax_rate -= 1;

    }


    if(_players[player_idx].tax_rate > 4)
    {

        _players[player_idx].tax_rate = 4;

    }


    Tries = 0;

    food_income = -(food_income);

    while(((Tries < 50) && (food_income > 0)))
    {

        city_idx = (Random(_cities) - 1);

        if(_CITIES[city_idx].owner_idx == player_idx)
        {

            if(_CITIES[city_idx].farmer_count < _CITIES[city_idx].population)
            {

                CITIES_FARMER_COUNT(city_idx, (_CITIES[city_idx].farmer_count + 1));

                // ; BUG: not all farmers generate 2 food
                food_income -= 2;

            }

        }

        Tries++;

    }

}


// WZD o145p13
/**
 * @brief Recomputes farmer allocation for all neutral-owned cities.
 *
 * Iterates over every city owned by the neutral player and sets its
 * farmer_count high enough to satisfy the city's minimum food requirement plus
 * the upkeep burden from units currently standing on that city's square.
 *
 * The routine applies a special divisor of 3 food per farmer for Halfling
 * cities and cities with a built granary, and otherwise uses the standard
 * divisor of 2. Each result is rounded up so partial farmer demand becomes a
 * full assigned farmer, then clamped so the final farmer count never exceeds
 * the city's total population.
 *
 * @note This function preserves the original game behavior noted in the local
 *       comments, including the Halfling/granary overland food calculation.
 * @note Reads global city and unit state from _CITIES, _cities, and the unit
 *       list queried via Map_Square_Unit_Count(), and writes updated
 *       farmer_count values back into neutral cities.
 */
/* GEMINI */
void NPC_Farmers(void)
{
    int16_t city_idx = 0;
    int16_t min_farmers = 0;
    int16_t unit_count = 0;
    int16_t temp_val = 0;
    for (city_idx = 0; city_idx < _cities; city_idx++)
    {
        if (_CITIES[city_idx].owner_idx == NEUTRAL_PLAYER_IDX)
        {
            /* Calculate base farmers needed for population */
            min_farmers = City_Minimum_Farmers(city_idx);
            /* Count units currently in the city square to calculate food maintenance */
            unit_count = Map_Square_Unit_Count(_CITIES[city_idx].wx, _CITIES[city_idx].wy, _CITIES[city_idx].wp);
            /* Check for production bonus (Halflings or Granary). */
            /* OGBUG:  wrong calculation */
            if (_CITIES[city_idx].race == rt_Halfling || _CITIES[city_idx].bldg_status[GRANARY] == bs_Built)
            {
                /* Halfling/Granary logic (3 food per farmer) */
                temp_val = min_farmers + unit_count;
                _CITIES[city_idx].farmer_count = (temp_val / 3);
                /* Round up if there is a remainder */
                if ((temp_val % 3) != 0)
                {
                    _CITIES[city_idx].farmer_count++;
                }
            }
            else
            {
                /* Standard logic (2 food per farmer) */
                temp_val = min_farmers + unit_count;
                _CITIES[city_idx].farmer_count = (temp_val / 2);
                /* Round up if there is a remainder */
                if ((temp_val % 2) != 0)
                {
                    _CITIES[city_idx].farmer_count++;
                }
            }
            /* Ensure farmer count does not exceed total population */
            if (_CITIES[city_idx].farmer_count > _CITIES[city_idx].population)
            {
                _CITIES[city_idx].farmer_count = _CITIES[city_idx].population;
            }
        }
    }
}


// WZD o145p14
// drake178: AI_ExcessBuilders()
/*
ensure that there is at most one settler and, after
turn 200, at most one engineer on any continent
*/
/*

inherently keeps the oldest
kills off 2+ settlers, per continent
after turn 200, kills off 2+ engineers, per continent

*/
void AI_Kill_Excess_Settlers_And_Engineers(int16_t player_idx)
{
    int16_t engineer_count[NUM_LANDMASSES] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t settler_count[NUM_LANDMASSES] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t landmass_idx = 0;
    int16_t itr = 0;  // _DI_

    for(itr = 0; itr <NUM_LANDMASSES; itr++)
    {

        settler_count[itr] = 0;

        engineer_count[itr] = 0;

    }

    for(itr = 0; itr < _units; itr++)
    {

        if(_UNITS[itr].owner_idx == player_idx)
        {

            if((_unit_type_table[_UNITS[itr].type].Abilities & UA_CREATEUNDEAD) != 0)
            {

                landmass_idx = _landmasses[((_UNITS[itr].wp * WORLD_SIZE) + (_UNITS[itr].wy * WORLD_WIDTH) + _UNITS[itr].wx)];

                if(
                    (landmass_idx != 0)
                    &&
                    (landmass_idx < NUM_LANDMASSES)
                )
                {

                    settler_count[landmass_idx] += 1;

                    if(settler_count[landmass_idx] > 1)
                    {

                        Kill_Unit(itr, 0);

                    }

                }

            }

            if(
                (_unit_type_table[_UNITS[itr].type].Construction > 0)
                &&
                (_turn > 200)
            )
            {

                landmass_idx = _landmasses[((_UNITS[itr].wp * WORLD_SIZE) + (_UNITS[itr].wy * WORLD_WIDTH) + _UNITS[itr].wx)];

                if(
                    (landmass_idx != 0)
                    &&
                    (landmass_idx < NUM_LANDMASSES)
                )
                {

                    engineer_count[landmass_idx] += 1;

                    if(engineer_count[landmass_idx] > 1)
                    {

                        Kill_Unit(itr, 0);

                    }

                }

            }

        }

    }

}


// WZD o145p15
void AI_Hopeless_Stasis(void)
{
    int16_t unit_idx = 0;
    for (unit_idx = 0; unit_idx < _units; unit_idx++)
    {
        if (_UNITS[unit_idx].owner_idx == HUMAN_PLAYER_IDX)
        {
            continue;
        }
        if (!(_UNITS[unit_idx].mutations & (C_STASISINIT | C_STASISLINGER)))
        {
            continue;
        }
        if (_unit_type_table[_UNITS[unit_idx].type].Resist < 7)
        {
            Kill_Unit(unit_idx, kt_Normal);
        }
    }
}


// WZD o145p16
// drake178: EMM_Map_CONTXXX()
/*
maps in the EMM_ContXXX_H handle (all 4 pages), and
resets its corresponding global pointers
*/
/*
EmmHndl_CONTXXX is allocated in ALLOC.C Allocate_Data_Space()

¿ kinda like ?
m_terrain_lbx_000
LOADER.C
Terrain_Init()
// ehn_terrain_lbx = EMM_Load_LBX_File("TERRAIN.LBX", 1);
// HACK:  no EMM, so just load entry and monkey with offset adjustments
// TODO  make this an allocate and reload and set another pointer to the offset, so it looks like it did and dont need the math elsewhere - e.g., Draw_Map_Terrain()
m_terrain_lbx_000 = LBX_Load(terrain_lbx_file, 0);  // 676416 B

*/
void EMM_Map_CONTXXX__WIP(void)
{
    int16_t itr = 0;  // _SI_

    // TODO  EMM_Map4Pages(0, EmmHndl_CONTXXX);
    // ...
    // TODO  CONTX_Arc_TileXs = SA_MK_FP0(EMS_PFBA);
    // ...
    // ...
    // ...

    // EMS_PFBA = Allocate_Space(((4 * 16384) / 16));
    EMS_PFBA = EmmHndl_CONTXXX;

    CONTX_TileXs[0]           =  (int8_t *)(&EMS_PFBA[0] + (   0 * SZ_PARAGRAPH_B));  //  100 -    0 = 100 * 1 PR = 1600 B
    CONTX_TileXs[1]           =  (int8_t *)(&EMS_PFBA[0] + ( 100 * SZ_PARAGRAPH_B));  //  200 -  100 = 100 * 1 PR = 1600 B
    CONTX_TileYs[0]           =  (int8_t *)(&EMS_PFBA[0] + ( 200 * SZ_PARAGRAPH_B));  //  300 -  200 = 100 * 1 PR = 1600 B
    CONTX_TileYs[1]           =  (int8_t *)(&EMS_PFBA[0] + ( 300 * SZ_PARAGRAPH_B));  //  400 -  300 = 100 * 1 PR = 1600 B
    CONTX_TileChain[0]        = (int16_t *)(&EMS_PFBA[0] + ( 400 * SZ_PARAGRAPH_B));  //  600 -  400 = 200 * 1 PR = 3200 B
    CONTX_TileChain[1]        = (int16_t *)(&EMS_PFBA[0] + ( 600 * SZ_PARAGRAPH_B));  //  800 -  600 = 200 * 1 PR = 3200 B
    CONTX_FirstTiles[0]       = (int16_t *)(&EMS_PFBA[0] + ( 800 * SZ_PARAGRAPH_B));  //  825 -  800 =  25 * 1 PR =  400 B
    CONTX_FirstTiles[1]       = (int16_t *)(&EMS_PFBA[0] + ( 825 * SZ_PARAGRAPH_B));  //  850 -  825 =  25 * 1 PR =  400 B
    _ai_own_stack_wx          = (&EMS_PFBA[0] + ( 850 * SZ_PARAGRAPH_B));  //  860 -  850 =  10 * 1 PR =  160 B  80 2-byte values
    _ai_own_stack_wy          = (&EMS_PFBA[0] + ( 860 * SZ_PARAGRAPH_B));  //  870 -  860 =  10 * 1 PR =  160 B  80 2-byte values
    _ai_own_stack_wp          = (&EMS_PFBA[0] + ( 870 * SZ_PARAGRAPH_B));  //  880 -  870 =  10 * 1 PR =  160 B  80 2-byte values
    _ai_own_stack_type        = (&EMS_PFBA[0] + ( 880 * SZ_PARAGRAPH_B));  //  890 -  880 =  10 * 1 PR =  160 B  80 2-byte values
    _ai_own_stack_unit_count  = (&EMS_PFBA[0] + ( 890 * SZ_PARAGRAPH_B));  //  900 -  890 =  10 * 1 PR =  160 B  80 2-byte values
    g_ai_evaluation_map[0]        = (uint16_t *)(&EMS_PFBA[0] + ( 900 * SZ_PARAGRAPH_B));  // 1500 -  900 = 600 * 1 PR = 9600 B
    g_ai_evaluation_map[1]        = (uint16_t *)(&EMS_PFBA[0] + (1500 * SZ_PARAGRAPH_B));  // 2100 - 1500 = 600 * 1 PR = 9600 B
    CONTX_LoadTileXs[0]       =  (int8_t *)(&EMS_PFBA[0] + (2100 * SZ_PARAGRAPH_B));  // 2200 - 2100 = 100 * 1 PR = 1600 B
    CONTX_LoadTileXs[1]       =  (int8_t *)(&EMS_PFBA[0] + (2200 * SZ_PARAGRAPH_B));  // 2300 - 2200 = 100 * 1 PR = 1600 B
    CONTX_LoadTileYs[0]       =  (int8_t *)(&EMS_PFBA[0] + (2300 * SZ_PARAGRAPH_B));  // 2400 - 2300 = 100 * 1 PR = 1600 B
    CONTX_LoadTileYs[1]       =  (int8_t *)(&EMS_PFBA[0] + (2400 * SZ_PARAGRAPH_B));  // 2500 - 2400 = 100 * 1 PR = 1600 B
    CONTX_LoadTChain[0]       = (int16_t *)(&EMS_PFBA[0] + (2500 * SZ_PARAGRAPH_B));  // 2700 - 2500 = 200 * 1 PR = 3200 B
    CONTX_LoadTChain[1]       = (int16_t *)(&EMS_PFBA[0] + (2700 * SZ_PARAGRAPH_B));  // 2900 - 2700 = 200 * 1 PR = 3200 B
    CONTX_1stLoadTs[0]        = (int16_t *)(&EMS_PFBA[0] + (2900 * SZ_PARAGRAPH_B));  // 3000 - 2900 = 100 * 1 PR = 1600 B
    CONTX_1stLoadTs[1]        = (int16_t *)(&EMS_PFBA[0] + (3000 * SZ_PARAGRAPH_B));  // 3200 - 3000 = 200 * 1 PR = 3200 B

    /* HACK */  memset(g_ai_evaluation_map[0], 0, 9600);
    /* HACK */  memset(g_ai_evaluation_map[1], 0, 9600);

// mov     ax, _SI_itr
// shl     ax, 1
// mov     dx, [EMS_PFBA]             ; contains the segment address of the EMS page frame
// add     dx, ax
// add     dx, 3200
// push    dx                              ; addr
// call    SA_MK_FP0
// pop     cx
// mov     bx, _SI_itr
// mov     cl, 2
// shl     bx, cl
// mov     [word ptr (_ai_own_stack_unit_list+2)+bx], dx ; 2 segment increments each (32 bytes data space)
//                                         ; each holds a list of unit indices or -1s for units
//                                         ; that have already been assigned orders
// mov     [word ptr _ai_own_stack_unit_list+bx], ax ; 2 segment increments each (32 bytes data space)
//                                         ; each holds a list of unit indices or -1s for units
//                                         ; that have already been assigned orders
// inc     _SI_itr

    for(itr = 0; itr < 80; itr++)
    {

        // ¿ get a pointer at every 32 bytes, starting at 51200 bytes from the EMM base ?
        _ai_own_stack_unit_list[itr] = (int16_t *)(&EMS_PFBA[0] + (3200 * SZ_PARAGRAPH_B) + ((itr * 2) * SZ_PARAGRAPH_B));
        
    }

}

// WZD o145p17
// drake178: CONTX_CreateChains()
/*
maps in the CONTXXX handle into the EMS page frame
and (re)creates the continent tile chains

BUG: only evaluates the first 60 continents
*/
/*

*/
void CONTX_CreateChains__WIP(void)
{
    int16_t Current_Tile = 0;
    int16_t landmass_idx = 0;
    int16_t count[2] = { 0, 0 };
    int16_t itr = 0;
    int16_t wy = 0;
    int16_t wx = 0;
    int16_t wp = 0;  // _DI_
    int16_t * array_ptr;  // DNE in Dasm  ~ [ES:BX]

    EMM_Map_CONTXXX__WIP();

    for(itr = 0; itr < NUM_LANDMASSES; itr++)
    {

        CONTX_FirstTiles[0][itr] = ST_UNDEFINED;

        CONTX_FirstTiles[1][itr] = ST_UNDEFINED;

    }

    for(itr = 0; itr < 1600; itr++)
    {

        CONTX_TileChain[0][itr] = ST_UNDEFINED;

        CONTX_TileChain[1][itr] = ST_UNDEFINED;

    }

    count[0] = 0;

    count[1] = 0;

    for(wp = 0; wp < NUM_PLANES; wp++)
    {

        for(wy = 0; wy < WORLD_HEIGHT; wy++)
        {
            
            for(wx = 0; wx < WORLD_WIDTH; wx++)
            {

                landmass_idx = _landmasses[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)];
                assert(landmass_idx < NUM_LANDMASSES);

                if(landmass_idx != 0)
                {

                    if(CONTX_FirstTiles[wp][landmass_idx] == ST_UNDEFINED)
                    {

                        array_ptr = &CONTX_FirstTiles[wp][landmass_idx];

                    }
                    else
                    {

                        itr = CONTX_FirstTiles[wp][landmass_idx];

                        Current_Tile = itr;

                        while(itr != ST_UNDEFINED)
                        {

                            Current_Tile = itr;

                            itr = CONTX_TileChain[wp][itr];

                        }

                        array_ptr = &CONTX_TileChain[wp][Current_Tile];

                    }

                    *array_ptr = count[wp];

                    CONTX_TileXs[wp][count[wp]] = (int8_t)wx;

                    CONTX_TileYs[wp][count[wp]] = (int8_t)wy;

                    CONTX_TileChain[wp][count[wp]] = ST_UNDEFINED;

                    count[wp] += 1;

                }

            }

        }

    }

    // DONT  EMM_Map_DataH();

}


// WZD o145p18
// drake178: CONTX_CreateLChains()
/*
maps in the CONTXXX handle into the EMS page frame
and (re)creates the transport loading tile chains

BUG: only evaluates the first 60 continents
*/
/*

*/
void CONTX_CreateLChains__WIP(void)
{
    int16_t Transport_Tile = 0;
    int16_t wy_ofst = 0;
    int16_t wx_ofst = 0;
    int16_t Current_Tile = 0;
    int16_t landmass_idx = 0;
    int16_t count[2] = { 0, 0 };
    int16_t itr = 0;
    int16_t wy = 0;
    int16_t wx = 0;
    int16_t wp = 0;  // _DI_
    int16_t * array_ptr;  // DNE in Dasm  ~ [ES:BX]

    EMM_Map_CONTXXX__WIP();

    for(itr = 0; itr < NUM_LANDMASSES; itr++)
    {

        CONTX_1stLoadTs[0][itr] = ST_UNDEFINED;

        CONTX_1stLoadTs[1][itr] = ST_UNDEFINED;

    }

    count[0] = 0;

    count[1] = 0;

    for(wp = 0; wp < NUM_PLANES; wp++)
    {

        for(wy = 0; wy < WORLD_HEIGHT; wy++)
        {
            
            for(wx = 0; wx < WORLD_WIDTH; wx++)
            {

                landmass_idx = _landmasses[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)];
                assert(landmass_idx < NUM_LANDMASSES);

                if(landmass_idx != 0)
                {

                    Transport_Tile = ST_FALSE;

                    for(wx_ofst = -1; ((wx_ofst < 2) && (Transport_Tile == ST_FALSE)); wx_ofst++)
                    {

                        for(wy_ofst = -1; ((wy_ofst < 2) && (Transport_Tile == ST_FALSE)); wy_ofst++)
                        {

                            if(Square_Is_Shoreline((wx + wx_ofst), (wy + wy_ofst), wp) == ST_TRUE)
                            {

                                Transport_Tile = ST_TRUE;

                            }

                        }

                    }

                    if(Transport_Tile == ST_TRUE)
                    {

                        if(CONTX_1stLoadTs[wp][landmass_idx] == ST_UNDEFINED)
                        {

                            array_ptr = &CONTX_1stLoadTs[wp][landmass_idx];

                        }
                        else
                        {

                            itr = CONTX_1stLoadTs[wp][landmass_idx];

                            Current_Tile = itr;

                            while(itr != ST_UNDEFINED)
                            {

                                Current_Tile = itr;

                                itr = CONTX_LoadTChain[wp][itr];

                            }

                            array_ptr = &CONTX_LoadTChain[wp][Current_Tile];

                        }

                        *array_ptr = count[wp];

                        CONTX_LoadTileXs[wp][count[wp]] = (int8_t)wx;

                        CONTX_LoadTileYs[wp][count[wp]] = (int8_t)wy;

                        CONTX_LoadTChain[wp][count[wp]] = ST_UNDEFINED;

                        count[wp] += 1;

                    }

                }

            }

        }

    }

    // DONT  EMM_Map_DataH();

}
