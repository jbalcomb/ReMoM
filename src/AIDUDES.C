/*
    AI - Data

    WIZARDS.EXE
        ¿ ovr145 ?
*/

#include "MoM.H"



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


// WZD o145p02
// drake178: AI_Overland_Turn()
// ¿ MoO2  Module: AIDATA  Compute_AI_Data_() ?
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
void AI_Next_Turn__STUB(void)
{
    int16_t itr_players = 0;  // _SI_
    int16_t itr_units = 0;  // _DI_
    int16_t itr_players2 = 0;  // _DI_

    // perform a sanity check on all unit coordinates,
    // marking any that had an invalid one as dead by
    // setting their owner to -1
    for(itr_units = 0; itr_units < _units; itr_units++)
    {

        if(
            (_UNITS[itr_units].wp < 0)
            ||
            (_UNITS[itr_units].wp > 1)
        )
        {

            _UNITS[itr_units].wp = 0;

            _UNITS[itr_units].owner_idx = ST_UNDEFINED;

        }

        if(
            (_UNITS[itr_units].wx < 0)
            ||
            (_UNITS[itr_units].wx >= WORLD_WIDTH)
        )
        {

            _UNITS[itr_units].wx = 0;

            _UNITS[itr_units].owner_idx = ST_UNDEFINED;

        }

        if(
            (_UNITS[itr_units].wy < 0)
            ||
            (_UNITS[itr_units].wy >= WORLD_HEIGHT)
        )
        {

            _UNITS[itr_units].wy = 0;

            _UNITS[itr_units].owner_idx = ST_UNDEFINED;

        }

    }

    // DONT  EMM_Map_DataH();

    Allocate_AI_Data();


// process a planning phase for each AI in turn,
// evaluating the situation, casting a spell, setting
// production and strategy, and giving orders to all
// available units
// 
// many many many BUGs in here
    for(itr_players = 1; itr_players < _num_players; itr_players++)
    {

        if(
            (g_TimeStop_PlayerNum == 0)
            ||
            ((g_TimeStop_PlayerNum - 1) == itr_players)
        )
        {

            if(
                (_FORTRESSES[itr_players].active == ST_TRUE)
                ||
                (_players[itr_players].casting_spell_idx == spl_Spell_Of_Return)
            )
            {

                AI_SCircle_Reevals[itr_players] -= 1;
                
                // ; decrease the war reevaluation counter, and if 0 or
                // ; less reset to Rnd(10)+15 and perform a war
                // ; evaluation, resetting Hostility with all other
                // ; players, then deciding whether to set it to 2, 3, or
                // ; 4 instead
                // ;
                // ; BUG: considers the casting of Fire Elemental as an
                // ; immediate call for war instead of the Spell of
                // ; Mastery in the first instance
                // TODO  AI_Evaluate_War(itr_players, (AI_SCircle_Reevals[itr_players] - 1));

                // ; evaluates conditions and sets the magic strategy
                // ; that may influence the AI's power distribution
                // ;
                // ; has one certain, and multiple possible bugs, as it
                // ; sets unused strategies, possibly overwriting others
                // TODO  AI_Magic_Strategy(itr_players);

                CRP_AI_Turn_Var = 0;

                // ; counts the wars the player is currently in, storing
                // ; both the amount and the target players into their
                // ; corresponding global variables
                // TODO  AI_Count_Active_Wars(itr_players);

                // ; evaluates all overland cities and units, creating
                // ; tables for own stack capabilities, own and enemy
                // ; stack and garrison values, and city values
                // ;
                // ; contains a host of BUGs, needs re-evaluating later!
                // TODO  AI_OVL_Eval_Assets(itr_players0);

                // ; evaluates own and enemy strength and target values on
                // ; the first 60 continents
                // ;
                // ; BUG: there can be more than 60 continents
                // TODO  AI_Continent_Eval(itr_players);

                // ; selects the main action continents for both Arcanus
                // ; and Myrror... I think anyway
                // ;
                // ; contains a host of BUGs, need to re-examine later
                // TODO  AI_Pick_Action_Conts(itr_players);

                if(
                    (_players[itr_players].casting_cost_remaining <= 0)
                    &&
                    (_players[itr_players].casting_spell_idx != spl_NONE)
                )
                {

                    Cast_Spell_Overland__WIP(itr_players);

                    // DONT  EMM_Map_DataH();                   ; maps the EMM Data block into the page frame

                    _players[itr_players].casting_spell_idx = spl_NONE;

                    if(AI_Eval_After_Spell == ST_TRUE)
                    {

                        Allocate_AI_Data();

                        // ; counts the wars the player is currently in, storing
                        // ; both the amount and the target players into their
                        // ; corresponding global variables
                        // TODO  AI_Count_Active_Wars(itr_players);

                        //  ; evaluates all overland cities and units, creating
                        //  ; tables for own stack capabilities, own and enemy
                        //  ; stack and garrison values, and city values
                        //  ;
                        //  ; contains a host of BUGs, needs re-evaluating later!
                        // TODO  AI_OVL_Eval_Assets(itr_players0;

                    }

                    if(_players[itr_players].casting_spell_idx == spl_NONE)
                    {

                        // ; selects the next overland spell to cast for the
                        // ; specified wizard
                        // ;
                        // ; multiple BUGs and unexplored sub-functions,
                        // ; RE-EXPLORE!
                        // TODO  AI_OVL_SpellPicker(itr_players);

                    }

                }

                if(_players[itr_players].casting_spell_idx == spl_NONE)
                {

                    // ; selects the next overland spell to cast for the
                    // ; specified wizard
                    // ;
                    // ; multiple BUGs and unexplored sub-functions,
                    // ; RE-EXPLORE!
                    // TODO  AI_OVL_SpellPicker(itr_players);

                }

                // DONT  EMM_Map_DataH();

                // ; sets the AI's power distribution percentages based
                // ; on objective, magic strategy, current skill & mana,
                // ; spells left to research, and spell being cast
                // ;
                // ; contains multiple BUGs that prohibit this function
                // ; from working properly once the AI has ran out of
                // ; spells to research
                // TODO  AI_Power_Distrib(itr_players);

                // ; releases unwanted global enchantments or, if at zero
                // ; mana, all global enchantments
                // ;
                // ; WARNING: this may not be the best thing to do...
                // TODO  AI_ReleaseGlobals(itr_players);

                // ; converts the player's gold into mana or vice versa
                // ; based on a set of criteria
                // ;
                // ; BUG: when banished, converted gold replaces existing
                // ; mana instead of being added to it
                // TODO  AI_Alchemy(itr_players);

                // ; sets the tax rate for the player and, if food
                // ; production is below 0, makes 50 attempts at
                // ; converting a random worker into a farmer
                // ;
                // ; BUG: treats all farmers as generating 2 food
                // TODO  AI_Tax_And_Farmers(itr_players);

                Some_AI_Turn_Var_2 = 0;

                Some_AI_Turn_Var_3 = 0;


                for(itr_players2 = 0; itr_players2 < _num_players; itr_players2++)
                {

                    if(_players[itr_players].peace_duration[itr_players2] != 0)
                    {

                        _players[itr_players].peace_duration[itr_players2] -= 1;

                    }

                }

                // TODO  All_Player_Colony_Autobuild(itr_players);

                // ; maps in and fills out the CONTXXX enemy strength
                // ; arrays
                // ;
                // ; BUG? the maps are only 4800 bytes despite their
                // ; allocated 9600
                // TODO  AI_SetEnemyStrMaps(itr_players);

                // ; decreases the continent reevaluation counter, and if
                // ; zero, resets it and rechecks the status of all
                // ; continents
                // ;
                // ; multitude of BUGs, slow, RE-EXAMINE, RE-DESIGN!
                // TODO  AI_Continent_Reeval(itr_players);

                // ; processes continent reevaluation and gives orders to
                // ; all available units, including disbanding some if
                // ; necessary
                // ;
                // ; many many BUGs
                // TODO  AI_SetUnitOrders(itr_players);

                // DONT  EMM_Map_DataH();                   ; just called this at the end of the previous function

                // ; ensure that there is at most one settler and, after
                // ; turn 200, at most one engineer on any continent
                // TODO  AI_ExcessBuilders(itr_players);

            }

        }

    }


    // ; clear the movement failed flag from all units that
    // ; don't belong to the human player

    // DONT  EMM_Map_DataH();

    for(itr_units = 0; itr_units < _units; itr_units++)
    {

        if(_UNITS[itr_units].owner_idx != HUMAN_PLAYER_IDX)
        {

            _UNITS[itr_units].Move_Failed = ST_FALSE;

        }

    }


    // ; attempt to execute the orders of all units for each
    // ; remaining active AI player
    // ; 
    // ; many BUGs in here too, RE-EXPLORE!
    for(itr_players = 1; itr_players < _num_players; itr_players++)
    {

        if(
            (g_TimeStop_PlayerNum == 0)
            ||
            ((g_TimeStop_PlayerNum - 1) == itr_players)
        )
        {

            if(
                (_FORTRESSES[itr_players].active == ST_TRUE)
                ||
                (_players[itr_players].casting_spell_idx == spl_Spell_Of_Return)
            )
            {

                // ; attempts to execute the current orders for all owned
                // ; units
                // ;
                // ; unclear, BUGgy, RE-EXPLORE!
                // TODO  AI_MoveUnits(itr_players);

            }

        }

    }

    // DONT  EMM_Map_DataH();                   ; maps the EMM Data block into the page frame

    All_Player_Colony_Autobuild(NEUTRAL_PLAYER_IDX);

    // ; sets the amount of farmers in all neutral cities
    // ;
    // ; BUG: miscalculates the amount of farmers needed to
    // ; support the city garrison
    // TODO  AI_SetNeutralFarmers();

    // ; enumerate and assign targets to non-city neutral
    // ; stacks, disbanding them if there is no non-neutral
    // ; city that they could target
    // TODO  AI_SetNeutralTargets();

    // ; attempts to execute the current orders for all owned
    // ; units
    // ;
    // ; unclear, BUGgy, RE-EXPLORE!
    // TODO  AI_MoveUnits(NEUTRAL_PLAYER_IDX);


    // ; advances the raiders accumulator and makes 1000
    // ; attempts at generating raiders if the treshold is
    // ; reached, advancing the rampaging monster accumulator
    // ; by 15 instead if there's at least one neutral city
    // ; on the map but raider generation failed
    // ;
    // ; BUG: fails to check the plane when looking for AI
    // ;  fortresses and non-neutral cities
    // ; BUG: can't generate barbarian spearmen or swordsmen
    // TODO  EVNT_GenerateRaiders();

    // ; advances the rampaging monster accumulator, and if
    // ; the treshold is reached, attempts to create a stack
    // ; of monsters from a random zone that is on the same
    // ; continent as a non-neutral city
    // ;
    // ; BUG: fails to check for the correct plane when making
    // ; budget adjustments based on human and AI fortress
    // ; locations
    // TODO  EVNT_RampageMonsters();

    // ; removes the cheapest unit from every neutral city
    // ; that has more units than its population + 2 for
    // ; Granary + 2 for Farmers' Market
    // TODO  AI_ExcessNeutrals();

    // ; removes all units not owned by the human player that
    // ; are under the effect of the Stasis spell and have
    // ; less than 7 resistance
    // TODO  AI_StasisDisband();


}


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

