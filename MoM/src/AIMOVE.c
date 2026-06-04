/*
    AI - Move

    WIZARDS.EXE
        ovr158
        ovr162
*/

#include "../../STU/src/STU_DBG.h"
#include "../../STU/src/STU_LOG.h"

#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOX_DAT.h"  /* _players[] */
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_TYPE.h"
#include "../../MoX/src/random.h"
#include "../../MoX/src/special.h"
#include "../../MoX/src/Util.h"
#include "../../MoX/src/EMS/EMS.h"

#include "Outpost.h"
#include "SETTLE.h"
#include "AIDATA.h"
#include "AIDUDES.h"
#include "CITYCALC.h"
#include "Combat.h"
#include "DIPLOMAC.h"
#include "NEXTTURN.h"
#include "RACETYPE.h"
#include "Spellbook.h"
#include "Terrain.h"
#include "UNITTYPE.h"
#include "UnitMove.h"
#include "DIPLODEF.h"

#include <assert.h>
#include <stdlib.h>

#include "AIMOVE.h"

static int16_t g_ai_set_target_caller = 0;



// WZD dseg:7116                                                 BEGIN:  ovr158 - Initialized Data  (AIMOVE)

int16_t ai_human_hostility = ST_FALSE;  /* OON XREF: AI_Set_Unit_Orders() */

// WZD dseg:7116                                                 END:  ovr158 - Initialized Data  (AIMOVE)



// WZD dseg:D3EC                                                 BEGIN:  ovr158 - Uninitialized Data  (AIMOVE)

// WZD dseg:D3EC
/*

*/
uint8_t * cp_landmass_type_array;
uint8_t * cp_landmass_wy_array;
uint8_t * cp_landmass_wx_array;

// WZD dseg:D3F2
/*
cleared in AI_Stacks_Survey_Expedition_Forces()
populated in AI_Stacks_Survey_Expedition_Forces_Stack()

AI_Set_Unit_Orders
    |-> AI_Stacks_Survey_Expedition_Forces()
        |-> AI_Stacks_Survey_Expedition_Forces_Stack()
    |-> AI_Stacks_Stage_Expedition_Forces()

G_Pushout_Count  ==>  cp_drafted_unit_count

G_Seafaring_Lowest_Value
G_Seafaring_Count
XRFEF:
    AI_Stacks_Survey_Expedition_Forces()
    AI_Stacks_Survey_Expedition_Forces_Stack()
G_Seafaring_Values[]
G_Pushout_Values[]
G_Seafaring_CX_IDs[]
G_Pushout_CX_IDs[]
G_Seafaring_UL_Indices[]
G_Seafaring_Unit_Indices[]
XREF:
    AI_Stacks_Survey_Expedition_Forces_Stack()

all of "G_Seafaring_..." is, effectively, Not-In-Use (NIU)

*/
int16_t G_Seafaring_Lowest_Value;
int16_t G_Pushout_Lowest_Value;
int16_t G_Seafaring_Count;
int16_t cp_drafted_unit_count;
int16_t G_Seafaring_Values[MAX_STACK];
int16_t G_Pushout_Values[MAX_STACK];  /* holds Effective_Unit_Type_Strength() */
int16_t G_Seafaring_CX_IDs[MAX_STACK];
int16_t G_Pushout_CX_IDs[MAX_STACK];
int16_t G_Seafaring_UL_Indices[MAX_STACK];
int16_t G_Pushout_UL_Indices[MAX_STACK];
int16_t G_Seafaring_Unit_Indices[MAX_STACK];
int16_t G_Pushout_Unit_Indices[MAX_STACK];
/*
Sizes match exactly — 4 fields × 2 bytes × 9 entries + 2 scalars × 2 bytes = 76 bytes per pool, 152 bytes total. Same as the current 12-global layout.
*/
/*
Producer access pattern (the only place these are written)
Before (AIMOVE.c:4014-4017):
    G_Pushout_UL_Indices[cp_drafted_unit_count] = stack_ul_idx;
    G_Pushout_CX_IDs[cp_drafted_unit_count] = stack_idx;
    G_Pushout_Values[cp_drafted_unit_count] = unit_value;
    G_Pushout_Unit_Indices[cp_drafted_unit_count] = unit_idx;
    cp_drafted_unit_count++;
After:
    _ai_pushout_pool.entries[_ai_pushout_pool.count].list_unit_idx = stack_ul_idx;
    _ai_pushout_pool.entries[_ai_pushout_pool.count].stack_idx     = stack_idx;
    _ai_pushout_pool.entries[_ai_pushout_pool.count].value         = unit_value;
    _ai_pushout_pool.entries[_ai_pushout_pool.count].unit_idx      = unit_idx;
    _ai_pushout_pool.count++;
Verbose at the call site, but the cp_drafted_unit_count / G_Seafaring_Count naming mismatch goes away — both pools use .count.
*/
/*
Bubble-sort swap becomes one struct copy
Before (AIMOVE.c:4122-4125):
    Swap_Short(&G_Seafaring_UL_Indices[itr2], &G_Seafaring_UL_Indices[itr2+1]);
    Swap_Short(&G_Seafaring_CX_IDs[itr2],     &G_Seafaring_CX_IDs[itr2+1]);
    Swap_Short(&G_Seafaring_Values[itr2],     &G_Seafaring_Values[itr2+1]);
    Swap_Short(&G_Seafaring_Unit_Indices[itr2], &G_Seafaring_Unit_Indices[itr2+1]);
After:
    struct s_AI_Excess_Entry tmp = _ai_seafaring_pool.entries[itr2];
    _ai_seafaring_pool.entries[itr2]     = _ai_seafaring_pool.entries[itr2+1];
    _ai_seafaring_pool.entries[itr2+1]   = tmp;
One value-copy chain instead of four function calls. Cleaner — and arguably more SimTex-style than the OG, since SimTex did love their record-copies.
*/
/* Per-entry: a single ranked unit in an excess pool */
struct s_AI_Excess_Entry
{
    int16_t value;            /* unit's effective strength score */
    int16_t stack_idx;        /* index into _ai_own_stack_* (was CX_ID) */
    int16_t list_unit_idx;    /* per-stack slot index   (was UL_Index) */
    int16_t unit_idx;         /* index into _UNITS[]    (was unit_idx) */
};

/* Pool: top-N priority queue keyed on value, with cached lowest for fast displacement */
struct s_AI_Excess_Pool
{
    int16_t count;
    int16_t lowest_value;
    struct s_AI_Excess_Entry entries[MAX_STACK];
};

/* The two instances — replaces the 12 parallel globals */
struct s_AI_Excess_Pool _ai_pushout_pool;
struct s_AI_Excess_Pool _ai_seafaring_pool;


// WZD dseg:D48A
/*
cleared and calculated in AI_Stacks_Init_Build_Target_Order()
used by AI_Stacks_Stage_Expedition_Forces()
*/
int16_t cp_staged_unit_count;
int16_t cp_enroute_unit_count;

// WZD dseg:D48E
/*
calcuated in AI_Set_Unit_Orders()
used by AI_Stacks_Stage_Expedition_Forces()
*/
int16_t _ai_expedition_size_threshold;

// WZD dseg:D490
int16_t niu_unknown_var;

// WZD dseg:D492 00 00                                           dw 0

// WZD dseg:D492                                                 END:  ovr158 - Uninitialized Data  (AIMOVE)



/*
    WIZARDS.EXE  ovr158
*/

// WZD o158p01
/**
 * @brief Execute the AI movement-order pass for one player for the current turn.
 *
 * This routine is the top-level dispatcher for the land and ocean movement AI.
 * It determines whether the AI is currently hostile toward the human player,
 * switches EMS mapping to the CONTXXX working data, resets ferry staging state,
 * computes the minimum desired expedition stack size, and performs a set of
 * once-per-turn global adjustments before processing each world plane.
 *
 * For every plane, the function binds the active landmass classification arrays
 * for `player_idx` and then iterates each non-ocean landmass, rebuilding local
 * stack state and running the major AI movement phases in order: non-military
 * garrison cleanup, expedition-force survey, meld/settle/purify/road actions,
 * target-list construction, roamer assignment or redeployment, reinforcement
 * toward the main war landmass, expedition staging, and site garrisoning. After
 * all landmasses on the plane have been processed, it runs the ocean-unit
 * wartime movement and ocean-landmass order passes.
 *
 * @param player_idx Index of the AI player whose units, stacks, and continent
 *                   state should be evaluated and assigned orders.
 *
 * @return This function does not return a value. It updates multiple global AI
 *         work arrays, may change unit orders across all planes, and restores
 *         EMS mapping to the default data block before returning.
 *
 * @note This function initializes `_ai_expedition_size_threshold`, `_ai_ferry_count`,
 *       `cp_landmass_wx_array`, `cp_landmass_wy_array`, and
 *       `cp_landmass_type_array` for downstream helpers during the current AI
 *       pass.
 *
 * @note The current implementation preserves a known original-game quirk where
 *       `AI_Find_Opportunity_City_Target()` is called with `wp` before the
 *       plane loop assigns it a defined plane index.
 */
void AI_Set_Unit_Orders(int16_t player_idx)
{
    int16_t wp = 0;
    int16_t landmass_idx = 0;


    /* Phase 1: Init */

    /* Initialization and Hostility Check */
    ai_human_hostility = ST_FALSE;
    if(
        (
            (_players[player_idx].Hostility[HUMAN_PLAYER_IDX] >= 3)
            ||
            (_players[player_idx].Dipl.Dipl_Status[HUMAN_PLAYER_IDX] >= DIPL_War)
        )
        &&
        (_players[player_idx].peace_duration[HUMAN_PLAYER_IDX] == 0)
    )
    {
        ai_human_hostility = ST_TRUE;
    }

    EMM_Map_CONTXXX__WIP();

    _ai_ferry_count = 0;

    /* Compute the minimum attack-stack threshold: grows 1 per 30 turns starting from 2, capped at MAX_STACK (9 by turn 240). */
    /* NOTE(JimBalcomb,20260525): only used by AI_Stacks_Stage_Expedition_Forces() */
    _ai_expedition_size_threshold = (2 + (_turn / 30));
    SETMAX(_ai_expedition_size_threshold,MAX_STACK);


    /* Phase 2 */
    /* Global AI adjustments */
    AI_Disband_To_Balance_Budget(player_idx);
    AI_Shift_Off_Home_Plane(player_idx);
    AI_Move_Out_Boats();
    /* OGBUG: wp is used here before being initialized */
    AI_Find_Opportunity_City_Target(wp, player_idx);
    

    /* Phase 3 */
    /* Iterate through all planes */
    for(wp = 0; wp < NUM_PLANES; wp++)
    {

        /* convenience pointers */
        cp_landmass_wx_array = &_ai_continents.plane[wp].player[player_idx].wx_array[0];
        cp_landmass_wy_array = &_ai_continents.plane[wp].player[player_idx].wy_array[0];
        cp_landmass_type_array = &_ai_continents.plane[wp].player[player_idx].type_array[0];

        /* Iterate through all landmasses */
        for(landmass_idx = 1; landmass_idx < NUM_LANDMASSES; landmass_idx++)
        {

            AI_Stacks_Init_Build_Target_Order(player_idx, landmass_idx, wp);
            AI_Stacks_Move_Out_NonMilitary_Garrisoned(wp);
            AI_Stacks_Survey_Expedition_Forces();

            AI_Stacks_Do_Meld(player_idx);
            AI_Stacks_Do_Settle(player_idx, landmass_idx);
            AI_Do_Purify(landmass_idx, wp);
            AI_Do_RoadBuild(landmass_idx);

            AI_Build_Target_List(player_idx, landmass_idx, wp);
            AI_Stacks_Roamers_Target_Or_Deploy(landmass_idx, wp, player_idx);

            /* reallocate stacks where we feel safe */
            if(
                (cp_landmass_type_array[landmass_idx] >= lmt_Leaveable)  /* {5:lmt_Leaveable,6:lmt_NoTargets} */
                ||
                (cp_landmass_type_array[landmass_idx] == lmt_Own)
                ||
                (cp_landmass_type_array[landmass_idx] == lmt_NoOwnCityAndAllyHasCity)
                ||
                (cp_landmass_type_array[landmass_idx] == lmt_NoOwnCity)
            )
            {
                AI_Stacks_Order_To_War_Landmass(player_idx, wp);
            }

            /* move non-garrisoned stacks off of landmasses whose type/status would suggested we can spare them */
            if(
                (cp_landmass_type_array[landmass_idx] >= lmt_Leaveable)  /* {5:lmt_Leaveable,6:lmt_NoTargets} */
                ||
                (cp_landmass_type_array[landmass_idx] == lmt_Own)
            )
            {
                AI_Stacks_Relocate_Roamers(landmass_idx, wp, player_idx);
            }

            /* could be super-duper weird this doesn't have a landmass type/status filer prerequisite */
            AI_Stacks_Stage_Expedition_Forces(landmass_idx, wp, player_idx);

            /* Eh? Incongruent landmass filter? dominant, nervous, sure we can withdaraw, no reason not to withdraw? */
            if(
                (cp_landmass_type_array[landmass_idx] == lmt_Own)
                ||
                (cp_landmass_type_array[landmass_idx] == lmt_Contested)
                ||
                (cp_landmass_type_array[landmass_idx] >= lmt_Leaveable)  /* {5:lmt_Leaveable,6:lmt_NoTargets} */
            )
            {
                AI_Stacks_Garrison_Sites(player_idx, wp, landmass_idx);
            }
        }

        /* Process Ocean landmass based Units */
        AI_Stacks_Wartime_Ocean_Movement_And_Cleanup(player_idx, wp);  /* ¿ only for war landmass ? */
        AI_Stacks_Ocean_Landmass_Orders(player_idx, wp);
        
    }

    
    /* Phase 4 */
    /* Restore EMM mapping to default Data block */
    EMMDATAH_Map();

}


// WZD o158p02
/**
 * @brief Send ranked expedition candidates toward a landmass stage point.
 *
 * Consumes the global expedition pool previously built by
 * AI_Stacks_Survey_Expedition_Forces(), checks whether the selected landmass
 * still needs additional staged or en-route units, and then issues movement
 * orders for the highest-ranked drafted units toward that landmass's current
 * stage coordinates.
 *
 * The function first performs a throttled continent re-evaluation when the
 * staged stack is already full. It then stops early if the minimum desired
 * attack-stack size has already been met, or if the landmass state and total
 * available expedition strength indicate that staging should not proceed. When
 * staging does proceed, units are taken from the front of the global pushout
 * arrays and ordered to the stage point via
 * AI_Stacks_Order_Attack_Target_Or_Goto_Destination().
 *
 * @param landmass_idx Index of the landmass whose stage point should receive
 *                     expedition units.
 * @param wp World plane containing the stage point and selected landmass.
 * @param player_idx Index of the AI player whose expedition forces are being
 *                   staged.
 *
 * @return This function does not return a value. It may trigger
 *         AI_Reevaluate_Continent() and may assign move orders to units stored
 *         in the global pushout expedition pool.
 *
 * @note This routine relies on the current values of `cp_staged_unit_count`,
 *       `cp_enroute_unit_count`, `cp_drafted_unit_count`, and
 *       `_ai_expedition_size_threshold`, all of which must already have been prepared by
 *       earlier AI passes in the same turn.
 */
void AI_Stacks_Stage_Expedition_Forces(int16_t landmass_idx, int16_t wp, int16_t player_idx)
{
    int16_t unit_list_idx = 0;
    int16_t unit_idx = 0;
    int16_t list_unit_idx = 0;
    int16_t gap_count = 0;
    int16_t itr = 0;

    /* Phase 1: Staged Stack is Full -  */
    if(cp_staged_unit_count == MAX_STACK)
    {
        /* Throttled re-evaluation of the continent strategy */
        if(Random(20) == 1)  /* 1:20  5% chance */
        {
            AI_Reevaluate_Continent(player_idx, landmass_idx, wp);
        }
    }

    /* Phase 2: Sanity Checks */
    /* Check if we've already met our quota */
    if((cp_staged_unit_count + cp_enroute_unit_count) >= _ai_expedition_size_threshold)
    {
        return;
    }
    /* Not enough available, don't bother */
    /* {lmt_Unevaluated, lmt_Own, lmt_Contested, lmt_NoOwnCityAndAllyHasCity} */
    /* Alt. if {lmt_NoOwnCity, lmt_Leaveable, lmt_NoTargets}, assemble what you can?*/
    if(
        (_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] < lmt_Leaveable)
        &&
        (_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] != lmt_NoOwnCity)
        &&
        ((cp_staged_unit_count + cp_enroute_unit_count + cp_drafted_unit_count) < _ai_expedition_size_threshold)
    )
    {
        return;
    }

    /* Phase 3 */
    /* Determine how many slots are available in the virtual "stack" being built */
    gap_count = (MAX_STACK - cp_enroute_unit_count);
    for(itr = 0; itr < gap_count; itr++)
    {
        /* If we run out of drafted units to send, stop */
        if(itr >= cp_drafted_unit_count)
        {
            break;
        }
        unit_list_idx = G_Pushout_CX_IDs[itr];
        list_unit_idx = G_Pushout_UL_Indices[itr];
        unit_idx = G_Pushout_Unit_Indices[itr];
        /* Call movement logic to set the unit's target to this landmass's stage point */
#ifdef STU_DEBUG
//      dbg_prn("AI_ORDERS: [GarrPush] unit %d -> continent %d coords (%d,%d)\n", unit_idx, landmass_idx, AI_Continent_X_Ptr[landmass_idx], AI_Continent_Y_Ptr[landmass_idx]);
        LOG_DEBUG(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: %s: -> AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx=%d, target_wx=%d, target_wy=%d, stack_idx=%d, list_unit_idx=%d)", __FILE__, __LINE__, __FUNCTION__, unit_idx, cp_landmass_wx_array[landmass_idx], cp_landmass_wy_array[landmass_idx], unit_list_idx, list_unit_idx);
#endif
        g_ai_set_target_caller = 1;
        AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, cp_landmass_wx_array[landmass_idx], cp_landmass_wy_array[landmass_idx], unit_list_idx, list_unit_idx);
        /* BUGBUG(JimBalcomb,20260525): Claude on iMustAi just said this is showing always 0,0 - must be bad expedition arrays */
    }
    
}


// WZD o158p03
/**
 * @brief Redirect available stacks to cities and nodes that still need garrison units.
 *
 * Surveys the specified landmass for player-owned cities and AI-targetable
 * nodes, computes how many defenders each site still needs after accounting for
 * units already present in AI stacks, and then walks the non-garrison stacks on
 * that landmass to assign reinforcements to the best remaining city or node.
 *
 * Site demand is influenced by the current landmass threat classification and,
 * for cities, by whether the city race tends to field stronger defenders. Each
 * eligible source stack chooses the site with the best distance-minus-shortfall
 * heuristic, then sends up to the requested number of military units while
 * skipping engineers, settlers, and melders.
 *
 * @param player_idx Index of the AI player whose sites are being reinforced.
 * @param wp World plane containing the landmass, cities, and nodes under
 *           consideration.
 * @param landmass_idx Index of the landmass whose cities and nodes should be
 *                     surveyed for garrison shortfalls.
 *
 * @return This function does not return a value. It may assign movement orders
 *         to units in `_ai_own_stack_unit_list` and updates the local
 *         shortfall-tracking arrays while processing.
 *
 * @note The current implementation preserves several original-game quirks,
 *       including asymmetrical node shortfall formulas, a duplicated
 *       `list_unit_count` assignment, and the loop condition that limits units
 *       sent by processed slot index rather than by successfully dispatched
 *       defenders.
 */
void AI_Stacks_Garrison_Sites(int16_t player_idx, int16_t wp, int16_t landmass_idx)
{
    int8_t node_garrison_shortfall[NUM_NODES] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int8_t node_list[NUM_NODES] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int8_t city_garrison_shortfall[NUM_CITIES] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int8_t city_list[NUM_CITIES] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t extra_strong_race = 0;
    int16_t fortress_city_idx = 0;
    int16_t low_concern_landmass = 0;
    int16_t site_added = 0;
    int16_t unit_idx = 0;
    int16_t target_type = 0;
    int16_t units_to_send = 0;
    int16_t stack_wy = 0;
    int16_t stack_wx = 0;
    int16_t target_wy = 0;
    int16_t target_wx = 0;
    int16_t target_value = 0;
    int16_t best_target_value = 0;
    int16_t node_count = 0;
    int16_t city_count = 0;
    int16_t node_wy = 0;
    int16_t node_wx = 0;
    int16_t city_wy = 0;
    int16_t city_wx = 0;
    int16_t itr = 0;
    int16_t unit_type = 0;
    int16_t list_unit_count = 0;
    int16_t itr_list_units = 0;
    int16_t itr_stacks = 0;
    int16_t target_site_idx = 0;


    /* Phase 1 */
    /* Check landmass threat level for player on this continent - threat assessment shifts garrison-size formulas */
    /* same check as in AI_Stacks_Stage_Expedition_Forces() */
    if(
        (_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] < lmt_Leaveable)
        &&
        (_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] != lmt_Own)
    )
    {
        /* {lmt_Unevaluated, lmt_Contested, lmt_NoOwnCity, lmt_NoOwnCityAndAllyHasCity} */
        low_concern_landmass = ST_FALSE;
    }
    else
    {
        /* {lmt_Own, lmt_Leaveable, lmt_NoTargets} */
        low_concern_landmass = ST_TRUE;
    }


    /* Phase 2 */
    /* Locate Fortress City for the player */
    /* player_idx, here, because fortress cities are created during world-gen in player order */
    /* OGBUG  should early-exit with `break;`*/
    fortress_city_idx = player_idx;
    for(itr = 0; itr < _cities; itr++)
    {
        if(
            (_CITIES[itr].wx == _FORTRESSES[player_idx].wx)
            &&
            (_CITIES[itr].wy == _FORTRESSES[player_idx].wy)
            &&
            (_CITIES[itr].wp == _FORTRESSES[player_idx].wp)
        )
        {
            fortress_city_idx = itr;
        }
    }


    /* Phase 3: Survey Cities for garrison shortfalls */
    city_count = 0;
    for(itr = 0; itr < _cities; itr++)
    {
        if(_CITIES[itr].owner_idx != player_idx) { continue; }
        if(_CITIES[itr].wp != wp) { continue; }

        city_wx = _CITIES[itr].wx;
        city_wy = _CITIES[itr].wy;

        /* Dwarf/Troll/Draconian units are high value, influencing garrison size logic */
        extra_strong_race = ST_FALSE;
        if(_CITIES[itr].race == rt_Dwarf || _CITIES[itr].race == rt_Troll || _CITIES[itr].race == rt_Draconian)
        {
            extra_strong_race = ST_TRUE;
        }
    
        /* Ensure City is on the active landmass being processed */
        if(_landmasses[((wp * WORLD_SIZE) + (city_wy * WORLD_WIDTH) + city_wx)] != landmass_idx)
        {
            continue;
        }
    
        site_added = ST_FALSE;
        if(itr == fortress_city_idx)
        {
            city_garrison_shortfall[city_count] = MAX_STACK;
        }
        else
        {
            if(
                (low_concern_landmass == ST_FALSE)
                &&
                (extra_strong_race == ST_FALSE)
            )
            {
                /* Low threat or high-quality defenders: 2 + pop/3 */
                city_garrison_shortfall[city_count] = (2 + (_CITIES[itr].population / 3));
            }
            else
            {
                /* Standard threat: 2+ pop/4 */
                city_garrison_shortfall[city_count] = (2 + (_CITIES[itr].population / 4));
            }
        }
        SETMAX(city_garrison_shortfall[city_count],MAX_STACK);

        /* Back out units already present in AI stacks at this city */
        for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
        {
            if(site_added != ST_FALSE)
            {
                break;
            }
            if(
                (_ai_own_stack_wx[itr_stacks] == city_wx)
                &&
                (_ai_own_stack_wy[itr_stacks] == city_wy)
            )
            {
                city_garrison_shortfall[city_count] -= _ai_own_stack_unit_count[itr_stacks];
                city_list[city_count] = (int8_t)itr;
                if(city_garrison_shortfall[city_count] > 0)
                {
                    city_count++;
                }
                site_added = ST_TRUE;
            }
        }

        /* If no stack was found at city, add it as a target if it needs units */
        if(site_added == ST_FALSE)
        {
            city_list[city_count] = (int8_t)itr;
            city_count++;
        }
    }


    /* Phase 4: Survey Nodes for garrison shortfalls */
    node_count = 0;
    for(itr = 0; itr < NUM_NODES; itr++)
    {
        if(_NODES[itr].wp != wp)
        {
            continue;
        }
        node_wx = _NODES[itr].wx;
        node_wy = _NODES[itr].wy;
        /* Only process nodes marked as valid AI target sites on the evaluation map */
        if(g_ai_evaluation_map[wp][((node_wy * WORLD_WIDTH) + node_wx)] != AI_TARGET_SITE)
        {
            continue;
        }
        /* OGBUG  redundant code */
        node_wx = _NODES[itr].wx;
        node_wy = _NODES[itr].wy;
        /* Ensure Node is on the active landmass being processed */
        if(_landmasses[((wp * WORLD_SIZE) + (node_wy * WORLD_WIDTH) + node_wx)] != landmass_idx)
        {
            continue;
        }
        site_added = ST_FALSE;
        for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
        {
            if(site_added != ST_FALSE)
            {
                break;
            }
            if(
                (_ai_own_stack_wx[itr_stacks] == node_wx)
                &&
                (_ai_own_stack_wy[itr_stacks] == node_wy)
            )
            {
                /* OGBUG  should be if 4 else 8, concern/threat is backwards */
                if(low_concern_landmass == ST_TRUE)
                {
                    node_garrison_shortfall[node_count] = (8 - _ai_own_stack_unit_count[itr_stacks]);
                }
                else
                {
                    node_garrison_shortfall[node_count] = (4 - _ai_own_stack_unit_count[itr_stacks]);
                }
                node_list[node_count] = (int8_t)itr;
                if(node_garrison_shortfall[node_count] > 0)
                {
                    node_count++;
                }
                site_added = ST_TRUE;
            }
        }
        if(site_added == ST_FALSE)
        {
            node_garrison_shortfall[node_count] = 8;  /* OGBUG  should if 4 else 8, as above */
            node_list[node_count] = (int8_t)itr;
            node_count++;
        }
    }


    /* Phase 5: Garrison for each Stack */
    for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
    {
        /* Phase 5a: Sanity Checks */
        /* Filter out stacks already garrisoned at a site {AISTK_Garrison,AISTK_FortressGarrison} */
        if(
            (_ai_own_stack_type[itr_stacks] != AISTK_Unknown)
            &&
            (_ai_own_stack_type[itr_stacks] != AISTK_Roamer)
        )
        {
            continue;
        }
        /* Filter out stacks already at the stage point — those were just handled by AI_Stacks_Stage_Expedition_Forces() */
        if(
            (_ai_own_stack_wx[itr_stacks] == cp_landmass_wx_array[landmass_idx])
            &&
            (_ai_own_stack_wy[itr_stacks] == cp_landmass_wy_array[landmass_idx])
        )
        {
            continue;
        }
    
        /* Phase 5b: Init */
        list_unit_count = _ai_own_stack_unit_count[itr_stacks];
        stack_wx = _ai_own_stack_wx[itr_stacks];
        stack_wy = _ai_own_stack_wy[itr_stacks];
        list_unit_count = _ai_own_stack_unit_count[itr_stacks];  /* OGBUG  diplicate line, see three above */
        best_target_value = 1000;

        /* Phase 5c: Choose City */
        for(itr = 0; itr < city_count; itr++)
        {
            if(city_garrison_shortfall[itr] > 0)
            {
                /* Heuristic: value = distance - units_needed. Lower is better (closer + more-needed wins) */
                target_value = (
                    Delta_XY_With_Wrap(stack_wx, stack_wy, _CITIES[city_list[itr]].wx, _CITIES[city_list[itr]].wy, WORLD_WIDTH)
                    -
                    city_garrison_shortfall[itr]
                );
                if(target_value < best_target_value)
                {
                    best_target_value = target_value;
                    target_wx = _CITIES[city_list[itr]].wx;
                    target_wy = _CITIES[city_list[itr]].wy;
                    units_to_send = city_garrison_shortfall[itr];
                    target_type = 0;
                    target_site_idx = itr;
                }
            }
        }

        /* Phase 5d: Choose Node */
        /* only tries Nodes if it didn't find a City */
        if(best_target_value == 1000)
        {
            for(itr = 0; node_count > itr; itr++)
            {
                if(node_garrison_shortfall[itr] > 0)
                {
                    /* Heuristic: value = distance - units_needed. Lower is better (closer + more-needed wins) */
                    target_value = (
                        Delta_XY_With_Wrap(stack_wx, stack_wy, _NODES[node_list[itr]].wx, _NODES[node_list[itr]].wy, WORLD_WIDTH)
                        -
                        node_garrison_shortfall[itr]
                    );
                    if(target_value < best_target_value)
                    {
                        best_target_value = target_value;
                        target_wx = _NODES[node_list[itr]].wx;
                        target_wy = _NODES[node_list[itr]].wy;
                        units_to_send = node_garrison_shortfall[itr];
                        target_type = 1;
                        target_site_idx = itr;
                    }
                }
            }
        }

        if(best_target_value < 1000)
        {
            for(itr_list_units = 0; itr_list_units < list_unit_count; itr_list_units++)
            {
                /* OGBUG  should track units sent, not processed - will exit early */
                if(itr_list_units >= units_to_send)
                {
                    break;
                }
                unit_idx = _ai_own_stack_unit_list[itr_stacks][itr_list_units];
                if(unit_idx == ST_UNDEFINED)
                {
                    continue;
                }
                unit_type = _UNITS[unit_idx].type;
                /* Skip Engineers, Settlers, Melders */
                if(
                    ((_unit_type_table[unit_type].Construction != 0))
                    ||
                    ((_unit_type_table[unit_type].Abilities & (UA_CREATEOUTPOST | UA_MELD)) != 0)
                )
                {
                    continue;
                }
#ifdef STU_DEBUG
                LOG_DEBUG(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: %s: -> AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx=%d, target_wx=%d, target_wy=%d, stack_idx=%d, list_unit_idx=%d)", __FILE__, __LINE__, __FUNCTION__, unit_idx, target_wx, target_wy, itr_stacks, itr_list_units);
#endif
                g_ai_set_target_caller = 2;
                AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, target_wx, target_wy, itr_stacks, itr_list_units);
                if(target_type == 0)
                {
                    city_garrison_shortfall[target_site_idx] -= 1;
                }
                else
                {
                    node_garrison_shortfall[target_site_idx] -= 1;
                }
            }
        }

    }

}


// WZD o158p04
/**
 * @brief Route ocean units toward the war stage point and remove stranded ocean stacks.
 *
 * This helper runs once per plane after landmass-specific AI processing. If the
 * player has no main war landmass on `wp`, it returns immediately. Otherwise it
 * determines a stage point for the current war effort, first from the stored
 * war-target landmass coordinates and, if those coordinates are unset, by
 * selecting the nearest contested or no-own-city landmass relative to the
 * player's fortress.
 *
 * When a stage point is available, the function scans all player-owned ocean
 * units on the plane and sends ready, individually mobile ocean-capable units
 * there by assigning `us_GOTO` with the stage coordinates. It skips units that
 * are not on ocean tiles, are already busy, are melders, or are transports.
 * Ocean-capable movement is recognized through air travel, water travel, or
 * non-corporeal status.
 *
 * The routine then rebuilds AI ocean stack state by calling
 * AI_Stacks_Init_Build_Target_Order(player_idx, 0, wp). Using that ocean stack
 * snapshot, it identifies stacks whose tracked entries contain no busy units
 * and no sea-capable units. Every unit owned by `player_idx` on the matching
 * ocean square is then killed through Kill_Unit(), preserving the original
 * stranded-stack cleanup behavior.
 *
 * @param player_idx Index of the AI player whose ocean movement is being
 *                   processed.
 * @param wp World plane whose ocean units and war stage point are examined.
 *
 * @return This function does not return a value. It may assign `us_GOTO`
 *         orders to individual units and may kill units on invalid ocean
 *         stacks.
 *
 * @note The cleanup pass relies on `AI_Stacks_Init_Build_Target_Order()` being
 *       able to represent ocean stacks with `landmass_idx == 0`, and preserves
 *       the original behavior where a stack is considered invalid when its
 *       tracked entries show neither busy units nor ocean-capable movement.
 */
void AI_Stacks_Wartime_Ocean_Movement_And_Cleanup(int16_t player_idx, int16_t wp)
{
    int16_t stack_wy = 0;
    int16_t stack_wx = 0;
    int16_t list_unit_count = 0;
    int16_t itr_list_units = 0;
    int16_t itr_stacks = 0;
    int16_t stack_has_seafaring_unit = 0;
    int16_t stack_has_busy_unit = 0;
    int16_t fortress_wy = 0;
    int16_t fortress_wx = 0;
    int16_t min_delta_distance = 0;
    int16_t delta_distance = 0;
    int16_t stage_wy = 0;
    int16_t stage_wx = 0;
    int16_t unit_idx = 0;
    int16_t itr = 0;


    /* Phase 1:  No War, bail. */
    if(_ai_landmass_war_targets[wp][player_idx] == 0)
    {
        return;
    }


    /* Phase 2 */

    /* ¿ war landmass geographic centroid ? */
    stage_wx = _ai_continents.plane[wp].player[player_idx].wx_array[_ai_landmass_war_targets[wp][player_idx]];
    stage_wy = _ai_continents.plane[wp].player[player_idx].wy_array[_ai_landmass_war_targets[wp][player_idx]];

    fortress_wx = _FORTRESSES[player_idx].wx;
    fortress_wy = _FORTRESSES[player_idx].wy;

    /* ¿ doesn't make sense, cause must be no war ? */
    if((stage_wx == 0) && (stage_wy == 0))
    {
        min_delta_distance = 1000;
        for(itr = 0; itr < NUM_LANDMASSES; itr++)
        {
            if(
                (_ai_continents.plane[wp].player[player_idx].type_array[itr] == lmt_Contested)
                ||
                (_ai_continents.plane[wp].player[player_idx].type_array[itr] == lmt_NoOwnCity)
            )
            {
                delta_distance = Delta_XY_With_Wrap(fortress_wx, fortress_wy, _ai_continents.plane[wp].player[player_idx].wx_array[itr], _ai_continents.plane[wp].player[player_idx].wy_array[itr], WORLD_WIDTH);
                if(delta_distance < min_delta_distance)
                {
                    min_delta_distance = delta_distance;
                    stage_wx = _ai_continents.plane[wp].player[player_idx].wx_array[itr];
                    stage_wy = _ai_continents.plane[wp].player[player_idx].wy_array[itr];
                }
            }
        }
    }


    /* Phase 3 */
    /* If a stage point was found, assign all idle individual seafarers to go there */
    if((stage_wx != 0) || (stage_wy != 0))
    {
        for(itr = 0; itr < _units; itr++)
        {
            if(_UNITS[itr].owner_idx != player_idx)
            {
                continue;
            }
            if(_UNITS[itr].wp != wp)
            {
                continue;
            }
            /* Ignore Units that are not on the Ocean */
            if(_landmasses[((wp * WORLD_SIZE) + (_UNITS[itr].wy * WORLD_WIDTH) + _UNITS[itr].wx)] != 0)  /* 0 is Ocean landmass */
            {
                continue;
            }
            /* DEDU  How's come? Maybe must mean they did get ordered? */
            if(_UNITS[itr].Status != us_Ready)
            {
                continue;
            }
            /* Ignore Melders, because they are all Non-Corporeal/Seafaring */
            if((_unit_type_table[_UNITS[itr].type].Abilities & UA_MELD) != 0)
            {
                continue;
            }
            /* Ignore Melders, because they all have Sailing/Seafaring */
            if(_unit_type_table[_UNITS[itr].type].Transport != 0)
            {
                continue;
            }
            /* If the Unit can move on the Ocean, send it on it's way */
            if(
                (Unit_Has_AirTravel(itr) != ST_FALSE)
                ||
                (Unit_Has_WaterTravel(itr) != ST_FALSE)
                ||
                (Unit_Has_NonCorporeal(itr) != ST_FALSE)
            )
            {
                _UNITS[itr].Status = us_GOTO;
                _UNITS[itr].dst_wx = (int8_t)stage_wx;
                _UNITS[itr].dst_wy = (int8_t)stage_wy;
            }
        }
    }


    /* Phase 4: make AI stack data for the Ocean */
    /* Re-evaluate stacks and process moves */
    AI_Stacks_Init_Build_Target_Order(player_idx, 0, wp);


    /* Phase 5: Cleanup the Ocean */
    for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
    {

        /* Phase 5a: (Re-)Init */
        stack_has_busy_unit = ST_FALSE;
        stack_has_seafaring_unit = ST_FALSE;
        list_unit_count = _ai_own_stack_unit_count[itr_stacks];

        /* Phase 5b: Collect our Decision Criteria */
        for(itr_list_units = 0; itr_list_units < list_unit_count; itr_list_units++)
        {
            /* OGBUG  should evaluate the rest of the units in the stack - misses any more invalid units */
            if(stack_has_busy_unit != ST_FALSE)
            {
                break;
            }
            unit_idx = _ai_own_stack_unit_list[itr_stacks][itr_list_units];
            if(unit_idx == ST_UNDEFINED)
            {
                stack_has_busy_unit = ST_TRUE;
            }
            else
            {
                if(
                    (Unit_Has_AirTravel(unit_idx) != ST_FALSE)
                    ||
                    (Unit_Has_WaterTravel(unit_idx) != ST_FALSE)
                    ||
                    (Unit_Has_NonCorporeal(unit_idx) != ST_FALSE)
                )
                {
                    stack_has_seafaring_unit = ST_TRUE;
                }
            }
        }

        /* Phase 5c: Invalid State, Destory the Unit */
        /* Claude says "stranded" - "stack has no busy units AND no swimmers" */
        if(
            (stack_has_busy_unit == ST_FALSE)
            &&
            (stack_has_seafaring_unit == ST_FALSE)
        )
        {
#ifdef STU_DEBUG
    LOG_DEBUG(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: %s: -> AI Stack Stranded", __FILE__, __LINE__, __FUNCTION__);
    STU_DEBUG_BREAK();
#endif
            /* ¿ OGBUG  Claude is quite concerned this is killing Seafaring Units that might still be on this square ? */
            stack_wx = _ai_own_stack_wx[itr_stacks];
            stack_wy = _ai_own_stack_wy[itr_stacks];
            for(itr = 0; itr < _units; itr++)
            {
                if(
                    (_UNITS[itr].wx == stack_wx)
                    &&
                    (_UNITS[itr].wy == stack_wy)
                    &&
                    (_UNITS[itr].wp == wp)
                    &&
                    (_UNITS[itr].owner_idx == player_idx)
                )
                {
                    Kill_Unit(itr, kt_Normal);
                }
            }
        }
        
    }

}


// WZD o158p05
void AI_Stacks_Ocean_Landmass_Orders(int16_t player_idx, int16_t wp)
{
    int16_t Transport_Spaces[MAX_AI_FERRIES] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t landmass_has_own_city = 0;
    int16_t landmass_has_ally_city = 0;
    int16_t stack_has_goto = 0;
    int16_t adjacent_landmass_idx = 0;
    int16_t Transport_Stack_Room = 0;
    int16_t Target_Queue_Index = 0;
    int16_t unit_idx = 0;
    int16_t itr_list_units = 0;
    int16_t adjacent_landmass_wy = 0;
    int16_t adjacent_landmass_wx = 0;
    int16_t stack_wy = 0;
    int16_t stack_wx = 0;
    int16_t Landing_Allowed = 0;
    int16_t unit_type = 0;
    int16_t stack_has_settler = 0;
    int16_t stack_has_only_boats = 0;
    int16_t stack_has_boat = 0;
    int16_t itr2 = 0;
    int16_t list_unit_count = 0;
    int16_t landmass_node_index = 0;
    int16_t wy_offset = 0;
    int16_t wx_offset = 0;
    int16_t delta_distance = 0;
    int16_t min_delta_distance = 0;
    int16_t target_wy = 0;
    int16_t target_wx = 0;
    int16_t itr_stacks = 0;
    int16_t itr1 = 0;


    /* Phase 1: Init */
    stack_has_goto = ST_FALSE;


    /* Phase 2: Transport_Spaces */
    /* Phase 2a: Per-ferry-queue-entry capacity counter */
    for(itr1 = 0; itr1 < MAX_AI_FERRIES; itr1++)
    {
        Transport_Spaces[itr1] = MAX_STACK;
    }
    /* Phase 2b */
    for(itr2 = 0; itr2 < _units; itr2++)
    {
        if(_UNITS[itr2].Status != us_GOTO)
        {
            continue;
        }
        if(_UNITS[itr2].owner_idx != player_idx)
        {
            continue;
        }
        if(_UNITS[itr2].wp != wp)
        {
            continue;
        }
        /* unit's destination is a ferry stage-point */
        for(itr1 = 0; itr1 < _ai_ferry_count; itr1++)
        {
            if(
                (_UNITS[itr2].dst_wx == _ai_ferry_wx_array[itr1])
                &&
                (_UNITS[itr2].dst_wy == _ai_ferry_wy_array[itr1])
                &&
                (_ai_ferry_wp_array[itr1] == wp)
            )
            {
                /* OGBUG  for unit_idx, should use itr2, instead of itr1 */
                /* available stack slots at destination (+1 to account for the boat's place in the stack) */
                Transport_Spaces[itr1] -= (_unit_type_table[_UNITS[itr1].type].Transport + 1);
            }
        }
    }

    /* Phase 2c: invalidate ferry entries */
    for(itr1 = 0; itr1 < _ai_ferry_count; itr1++)
    {
        /* flag any ferry entries that are full or overfull */
        if(Transport_Spaces[itr1] <= 0)
        {
            _ai_ferry_wp_array[itr1] = ST_UNDEFINED;
        }
    }


    /* Phase 3: make AI stack data for the Ocean */
    /* Re-evaluate stacks and process moves */
    AI_Stacks_Init_Build_Target_Order(player_idx, 0, wp);


    /* Phase 4 */
    AI_Stacks_Do_Meld(player_idx);


    /* Phase 5: Per-ocean-stack dispatch */
    for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
    {


        stack_has_boat = ST_FALSE;
        stack_has_only_boats = ST_TRUE;
        stack_has_settler = ST_FALSE;
        stack_has_goto = ST_FALSE;
        list_unit_count = _ai_own_stack_unit_count[itr_stacks];
        stack_wx = _ai_own_stack_wx[itr_stacks];
        stack_wy = _ai_own_stack_wy[itr_stacks];


/* Phase 5a: classify — stack_has_boat, stack_has_only_boats, stack_has_settler, stack_has_goto */
        for(unit_idx = 0; unit_idx < _units; unit_idx++)
        {
            if(
                (_UNITS[unit_idx].wx == stack_wx)
                &&
                (_UNITS[unit_idx].wy == stack_wy)
                &&
                (_UNITS[unit_idx].wp == wp)
            )
            {
                unit_type = _UNITS[unit_idx].type;
                if(_unit_type_table[unit_type].Transport > 0)
                {
                    stack_has_boat = ST_TRUE;
                }
                else
                {
                    stack_has_only_boats = ST_FALSE;
                    if(_unit_type_table[unit_type].Abilities & UA_CREATEOUTPOST)
                    {
                        stack_has_settler = ST_TRUE;
                    }
                }
                if(_UNITS[unit_idx].Status == us_GOTO)
                {
                    stack_has_goto = ST_TRUE;
                }
            }
        }


        /* No boat, nothing to do */
        if(stack_has_boat != ST_TRUE)
        {
            continue;
        }


        /* 5c	1261ST_UNDEFINED305	All-transport → closest ferry queue [caller=3] */
        /* Phase 5c: all-transport stack → route to closest ferry queue */
        if(stack_has_only_boats != ST_FALSE)
        {

            min_delta_distance = 10000;  /* OGBUG  should be 1000, instead of 10000 */
            for(itr1 = 0; itr1 < _ai_ferry_count; itr1++)
            {
                if(_ai_ferry_wp_array[itr1] == wp)
                {
                    delta_distance = Delta_XY_With_Wrap(stack_wx, stack_wy, _ai_ferry_wx_array[itr1], _ai_ferry_wy_array[itr1], WORLD_WIDTH);
                    if(delta_distance < min_delta_distance)
                    {
                        min_delta_distance = delta_distance;
                        target_wx = _ai_ferry_wx_array[itr1];
                        target_wy = _ai_ferry_wy_array[itr1];
                        Target_Queue_Index = itr1;
                    }
                }
            }

            if(min_delta_distance < 10000)
            {
                Transport_Stack_Room = Transport_Spaces[Target_Queue_Index];
                for(itr_list_units = 0; itr_list_units < list_unit_count; itr_list_units++)
                {
                    if(Transport_Stack_Room > 0)
                    {
                        unit_idx = _ai_own_stack_unit_list[itr_stacks][itr_list_units];
                        Transport_Stack_Room -= (_unit_type_table[_UNITS[unit_idx].type].Transport + 1);
#ifdef STU_DEBUG
                        LOG_DEBUG(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: %s: -> AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx=%d, target_wx=%d, target_wy=%d, stack_idx=%d, list_unit_idx=%d)", __FILE__, __LINE__, __FUNCTION__, unit_idx, target_wx, target_wy, itr_stacks, itr_list_units);
#endif
                        g_ai_set_target_caller = 3;
                        AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, target_wx, target_wy, itr_stacks, itr_list_units);
                    }
                }
                Transport_Spaces[Target_Queue_Index] = Transport_Stack_Room;
                if(Transport_Stack_Room <= 0)
                {
                    _ai_ferry_wp_array[Target_Queue_Index] = ST_UNDEFINED;
                }
            }

            continue;
        }
        

        /* 5d	1306ST_UNDEFINED309	Mixed-stack outer gate: skip if already-going */
        if(stack_has_goto != ST_FALSE)
        {
            continue;
        }


        /* 5e	1313ST_UNDEFINED335	3x3 adjacent ocean-square scan */
        /* OGBUG  no range checks */
        adjacent_landmass_wx = 0;
        adjacent_landmass_wy = 0;
        Landing_Allowed = ST_FALSE;
        for(wy_offset = ST_UNDEFINED; wy_offset < 2; wy_offset++)
        {
            for(wx_offset = ST_UNDEFINED; wx_offset < 2; wx_offset++)
            {
                /* ¿ OGBUG bad indexing, incorrect order of operation ? */
                /* adjacent square is a land square and occupieable */
                if(
                    (_landmasses[((wp * WORLD_SIZE) + ((stack_wy + wy_offset) * WORLD_WIDTH) + (stack_wx + wx_offset))] != 0)
                    &&
                    (g_ai_evaluation_map[wp][(((stack_wy + wy_offset) * WORLD_WIDTH) + (stack_wx + wx_offset))] == 0)
                )
                {
                    adjacent_landmass_idx = _landmasses[((wp * WORLD_SIZE) + ((stack_wy + wy_offset) * WORLD_WIDTH) + (stack_wx + wx_offset))];
                    adjacent_landmass_wx = (stack_wx + wx_offset);
                    adjacent_landmass_wy = (stack_wy + wy_offset);
                    Landing_Allowed = ST_TRUE;
                }
            }
        }


        /* Phase 5f:  Non-settler landing eligibility (war + city checks) */
        if(Landing_Allowed == ST_TRUE)
        {

            if(stack_has_settler != ST_TRUE)
            {

                /* IDA grey-purple */

                /* OGBUG  no range checking */
                Landing_Allowed = ST_FALSE;
                for(wy_offset = ST_UNDEFINED; wy_offset < 2; wy_offset++)
                {
                    for(wx_offset = ST_UNDEFINED; wx_offset < 2; wx_offset++)
                    {
                        /* Is the adjacent landmass the war landmass? */
                        if(_landmasses[((wp * WORLD_SIZE) + ((stack_wy + wy_offset) * WORLD_WIDTH) + (stack_wx + wx_offset))] == _ai_landmass_war_targets[wp][player_idx])
                        {
                            adjacent_landmass_idx = _landmasses[((wp * WORLD_SIZE) + ((stack_wy + wy_offset) * WORLD_WIDTH) + (stack_wx + wx_offset))];
                            adjacent_landmass_wx = (stack_wx + wx_offset);
                            adjacent_landmass_wy = (stack_wy + wy_offset);
                            Landing_Allowed = ST_TRUE;
                        }
                    }
                }

                /* maybe, override the unload flag */
                /* HERE: Landing_Allowed was ST_TRUE, but now ST_FALSE. Because? Adjacent landmass was not war landmass? */
                if(Landing_Allowed == ST_FALSE)
                {
                    landmass_has_own_city = ST_FALSE;
                    landmass_has_ally_city = ST_FALSE;
                    for(itr1 = 0; itr1 < _cities; itr1++)
                    {
                        if(
                            (_CITIES[itr1].wp == wp)
                            &&
                            (_landmasses[((wp * WORLD_SIZE) + (_CITIES[itr1].wy * WORLD_WIDTH) + _CITIES[itr1].wx)] == adjacent_landmass_idx)
                        )
                        {
                            if(_CITIES[itr1].owner_idx == player_idx)
                            {
                                landmass_has_own_city = ST_TRUE;
                            }
                            else
                            {
                                if(_players[player_idx].Dipl.Dipl_Status[_CITIES[itr1].owner_idx] == DIPL_Alliance)
                                {
                                    landmass_has_ally_city = ST_TRUE;
                                }
                            }
                        }
                    }
                }
                if(
                    (landmass_has_own_city == ST_TRUE)
                    ||
                    (landmass_has_ally_city == ST_TRUE)
                )
                {
                    Landing_Allowed = ST_FALSE;
                }
                else
                {
                    Landing_Allowed = ST_TRUE;
                }
            }
            else  /* if(stack_has_settler != ST_TRUE) */
            {
                /* IDA light-purple */

                /* 5g	1397ST_UNDEFINED435	Settler landing eligibility (target + enemy-proximity) */
                /* OGBUG  no range checking */
                for(wy_offset = ST_UNDEFINED; wy_offset < 2; wy_offset++)
                {
                    for(wx_offset = ST_UNDEFINED; wx_offset < 2; wx_offset++)
                    {
                        if(
                            (_landmasses[((wp * WORLD_SIZE) + ((stack_wy + wy_offset) * WORLD_WIDTH) + (stack_wx + wx_offset))] == _ai_landmass_settler_targets[wp][player_idx])
                            &&
                            _ai_landmass_settler_targets[wp][player_idx] != 0
                        )
                        {
                            adjacent_landmass_idx = _landmasses[((wp * WORLD_SIZE) + ((stack_wy + wy_offset) * WORLD_WIDTH) + (stack_wx + wx_offset))];
                            adjacent_landmass_wx = (stack_wx + wx_offset);
                            adjacent_landmass_wy = (stack_wy + wy_offset);
                        }
                    }
                }
                /* override the unload flag */
                if(
                    (
                        (AI_Enemy_Unit_In_Range(stack_wx, stack_wy, wp, 5, player_idx, adjacent_landmass_idx) != ST_FALSE)
                        ||
                        (_ai_continents.plane[wp].player[player_idx].type_array[adjacent_landmass_idx] != lmt_NoOwnCity)
                    )
                    &&
                    (_ai_landmass_settler_targets[wp][player_idx] != adjacent_landmass_idx)
                )
                {
                    Landing_Allowed = ST_FALSE;
                }
                else
                {
                    Landing_Allowed = ST_TRUE;
                }
            }  /* END:  if(stack_has_settler != ST_TRUE) */

        }

/*
    BEGIN:  Assign Orders
*/
        /* HERE:
            join war or settle continent
            - all just updated:
            Landing_Allowed, adjacent_landmass_idx, adjacent_landmass_wx, adjacent_landmass_wy
        */
        /* 5h	1439ST_UNDEFINED462	LAND DISPATCH [caller=4] */
        if(Landing_Allowed == ST_TRUE)
        {
            for(itr_list_units = 0; itr_list_units < list_unit_count; itr_list_units++)
            {
                /* OGBUG  MUST test unit_idx is not ST_UNDEFINED */
                unit_idx = _ai_own_stack_unit_list[itr_stacks][itr_list_units];
                unit_type = _UNITS[unit_idx].type;
                if(_unit_type_table[unit_type].Transport > 0)
                {
                    AI_Stacks_Order_Ferry(unit_idx, itr_stacks, itr_list_units);
                }
                else
                {
#ifdef STU_DEBUG
                    LOG_DEBUG(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: %s: -> AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx=%d, target_wx=%d, target_wy=%d, stack_idx=%d, list_unit_idx=%d)", __FILE__, __LINE__, __FUNCTION__, unit_idx, adjacent_landmass_wx, adjacent_landmass_wy, itr_stacks, itr_list_units);
#endif
                    g_ai_set_target_caller = 4;
                    AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, adjacent_landmass_wx, adjacent_landmass_wy, itr_stacks, itr_list_units);
                }
            }

            continue;

        }
/*
    END:  Assign Orders
*/


/*
BEGIN:  fixup bad orders, for valid colony or military stack
*/
    
        /* IDA Orange */

        /* 5i	1465ST_UNDEFINED496	Settler-fallback dispatch [caller=5] */
        /*
            WTF?
            maybe, colonization stack bumped into some landmass
            but, that landmass was not the colonization landmass
            so, just in case, but for no good reason
            double-check the colonization landmass coordinates
            and reset all the unit's orders
            (did we already handle 'colonization landmass was unset'?)
        */
        if((stack_has_settler == ST_TRUE) && (_ai_landmass_settler_targets[wp][player_idx] != 0))
        {
// ; if the new colony's stage point has an adjacent ocean square, set all units in the stack to move there
// ; BUG: no range checking
            adjacent_landmass_wx = 0;
            adjacent_landmass_wy = 0;
            for(wy_offset = ST_UNDEFINED; wy_offset < 2; wy_offset++)
            {
                for(wx_offset = ST_UNDEFINED; wx_offset < 2; wx_offset++)
                {
                    /* if colonization landmass has valid stage-point coordinates... */
                    if(_landmasses[((wp * WORLD_SIZE) + ((wy_offset + _ai_landmass_settler_targets_wy_array[wp][player_idx]) * WORLD_WIDTH) + (wx_offset + _ai_landmass_settler_targets_wx_array[wp][player_idx]))] == 0)
                    {
                        adjacent_landmass_wx = (wx_offset + _ai_landmass_settler_targets_wx_array[wp][player_idx]);
                        adjacent_landmass_wy = (wy_offset + _ai_landmass_settler_targets_wy_array[wp][player_idx]);
                        Landing_Allowed = ST_TRUE;  /* OGBUG  bogus line of code; c&p error? */
                    }
                }
            }
            for(itr_list_units = 0; itr_list_units < list_unit_count; itr_list_units++)
            {
                unit_idx = _ai_own_stack_unit_list[itr_stacks][itr_list_units];
#ifdef STU_DEBUG
                LOG_DEBUG(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: %s: -> AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx=%d, target_wx=%d, target_wy=%d, stack_idx=%d, list_unit_idx=%d)", __FILE__, __LINE__, __FUNCTION__, unit_idx, adjacent_landmass_wx, adjacent_landmass_wy, itr_stacks, itr_list_units);
#endif
                g_ai_set_target_caller = 5;
                AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, adjacent_landmass_wx, adjacent_landmass_wy, itr_stacks, itr_list_units);
            }

            continue;

        }
    
        /* IDA burgundy */

        /* 5j	1500ST_UNDEFINED551	War-landing dock dispatch [caller=6] */
        /* Do we have a war landmass? */
        /* if, if, if, reset unit's orders to nearest shoreline on war continent */
        if(_ai_landmass_war_targets[wp][player_idx] != 0)
        {
            adjacent_landmass_wx = stack_wx;
            adjacent_landmass_wy = stack_wy;
            landmass_node_index = _ai_landmass_dock_squares_heads[wp][_ai_landmass_war_targets[wp][player_idx]];
            min_delta_distance = 1000;  /* NOTE(JimBalcomb,20260525): this line prove that landmass_node_index was not cleverly set in the while statement */
            while(landmass_node_index != ST_UNDEFINED)
            {
                delta_distance = Delta_XY_With_Wrap(_ai_landmass_dock_squares_wx_array[wp][landmass_node_index], _ai_landmass_dock_squares_wy_array[wp][landmass_node_index], adjacent_landmass_wx, adjacent_landmass_wy, WORLD_WIDTH);
                if(delta_distance < min_delta_distance)
                {
                    /* landmass_node_wx, landmass_node_wy */
                    wx_offset = _ai_landmass_dock_squares_wx_array[wp][landmass_node_index];
                    wy_offset = _ai_landmass_dock_squares_wy_array[wp][landmass_node_index];
                    /* occupieable - no site or enemy stack */
                    if(g_ai_evaluation_map[wp][((wy_offset * WORLD_WIDTH) + wx_offset)] == 0)
                    {
                        min_delta_distance = delta_distance;
                        target_wx = wx_offset;
                        target_wy = wy_offset;
                    }
                }
                landmass_node_index = _ai_landmass_dock_squares_lists[wp][landmass_node_index];
            }
            if(min_delta_distance < 1000)
            {
                for(wy_offset = ST_UNDEFINED; wy_offset < 2; wy_offset++)
                {
                    for(wx_offset = ST_UNDEFINED; wx_offset < 2; wx_offset++)
                    {
                        if(_landmasses[((wp * WORLD_SIZE) + ((target_wy + wy_offset) * WORLD_WIDTH) + (target_wx + wx_offset))] == 0)
                        {
                            adjacent_landmass_wx = (target_wx + wx_offset);
                            adjacent_landmass_wy = (target_wy + wy_offset);
                        }
                    }
                }
                for(itr_list_units = 0; itr_list_units < list_unit_count; itr_list_units++)
                {
                    unit_idx = _ai_own_stack_unit_list[itr_stacks][itr_list_units];
#ifdef STU_DEBUG
                    LOG_DEBUG(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: %s: -> AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx=%d, target_wx=%d, target_wy=%d, stack_idx=%d, list_unit_idx=%d)", __FILE__, __LINE__, __FUNCTION__, unit_idx, adjacent_landmass_wx, adjacent_landmass_wy, itr_stacks, itr_list_units);
#endif
                    g_ai_set_target_caller = 6;
                    AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, adjacent_landmass_wx, adjacent_landmass_wy, itr_stacks, itr_list_units);
                }
            }

            continue;

        }  /* END:  if(_ai_landmass_war_targets[wp][player_idx] != 0) */

/*
END:  fixup bad orders, for valid colony or military stack
*/

    }  /* END:  for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++) */

}


// WZD o158p06
/**
 * @brief Assign targets to roaming stacks on a landmass or prepare them to redeploy.
 *
 * Iterates over AI stacks marked as `AISTK_Roamer` on the specified landmass,
 * checking whether each stack is fully populated with valid unit entries and,
 * when possible, requesting a destination from AI_Stacks_Assign_Target(). If a
 * target is found, every unit in that stack receives orders toward the chosen
 * target square.
 *
 * If no roaming stack on the landmass can be assigned a target, but at least
 * one large roaming stack exists and the player has a different main-war
 * landmass, the function searches for the closest empty dock square to use as a
 * staging point for leaving this landmass. In that case it updates the
 * landmass state in `_ai_continents` to `lmt_Leaveable` and stores the chosen
 * embark square coordinates.
 *
 * @param landmass_idx Index of the landmass being processed.
 * @param wp World plane containing the landmass.
 * @param player_idx Index of the AI player whose roaming stacks are being
 * processed.
 *
 * @note This is the only code path in the current code base that sets a
 * landmass type to `lmt_Leaveable`.
 */
void AI_Stacks_Roamers_Target_Or_Deploy(int16_t landmass_idx, int16_t wp, int16_t player_idx)
{
    int16_t have_large_stack = 0;
    int16_t landmass_node_count = 0;
    int16_t min_delta_distance = 0;
    int16_t target_square_wy = 0;
    int16_t no_target_found_for_any_stack = 0;
    int16_t landmass_node_wy = 0;
    int16_t landmass_node_wx = 0;
    int16_t target_square_wx = 0;
    int16_t landmass_node_index = 0;
    int16_t landmass_node_centroid_wy = 0;
    int16_t landmass_node_centroid_wx = 0;
    int16_t hit_end_of_stack = 0;
    int16_t unit_idx = 0;
    int16_t list_unit_count = 0;
    int16_t niu_have_boat_riders = 0;
    int16_t itr_list_units = 0;
    int16_t target_wy = 0;
    int16_t target_wx = 0;
    int16_t stack_wy = 0;
    int16_t stack_wx = 0;
    int16_t itr_stacks = 0;
    int16_t delta_distance = 0;  // DNE in Dasm, reuses landmass_node_count

    /* Phase 1 */
    have_large_stack = ST_FALSE;

    no_target_found_for_any_stack = ST_TRUE;


    /* Phase 2 */
    for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
    {
        if(_ai_own_stack_type[itr_stacks] != AISTK_Roamer)
        {
            continue;
        }
        list_unit_count = _ai_own_stack_unit_count[itr_stacks];
        stack_wx = _ai_own_stack_wx[itr_stacks];
        stack_wy = _ai_own_stack_wy[itr_stacks];
        if(list_unit_count >= 8)  /* ¿ (MAX_STACK - 1) ?*/
        {
            have_large_stack = ST_TRUE;
        }

        niu_have_boat_riders = ST_FALSE;
        hit_end_of_stack = ST_FALSE;

        for(itr_list_units = 0; itr_list_units < list_unit_count; itr_list_units++)
        {
            unit_idx = _ai_own_stack_unit_list[itr_stacks][itr_list_units];
            if(unit_idx == ST_UNDEFINED)
            {
                hit_end_of_stack = ST_TRUE;
                break;
            }

            if(
                (Unit_Has_AirTravel(unit_idx) == ST_FALSE)
                &&
                (Unit_Has_WaterTravel(unit_idx) == ST_FALSE)
            )
            {
                niu_have_boat_riders = ST_TRUE;
            }
        }

        if(hit_end_of_stack == ST_FALSE)
        {
            if(AI_Stacks_Assign_Target(stack_wx, stack_wy, &target_wx, &target_wy, itr_stacks, itr_list_units) == ST_TRUE)
            {
                no_target_found_for_any_stack = ST_FALSE;
                for(itr_list_units = 0; itr_list_units < list_unit_count; itr_list_units++)
                {
                    unit_idx = _ai_own_stack_unit_list[itr_stacks][itr_list_units];
#ifdef STU_DEBUG
                    LOG_DEBUG(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: %s: -> AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx=%d, target_wx=%d, target_wy=%d, stack_idx=%d, list_unit_idx=%d)", __FILE__, __LINE__, __FUNCTION__, unit_idx, target_wx, target_wy, itr_stacks, itr_list_units);
#endif
                    g_ai_set_target_caller = 7;
                    AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, target_wx, target_wy, itr_stacks, itr_list_units);
                }
            }
        }
        niu_unknown_var = 0;
    }

/*
¿ prepare large stack to embark and ferry off ?
*/
    if(have_large_stack != ST_TRUE)
    {
        return;
    }
    if(no_target_found_for_any_stack != ST_TRUE)
    {
        return;
    }
    if(_ai_landmass_war_targets[wp][player_idx] == 0)
    {
        return;
    }
    if(_ai_landmass_war_targets[wp][player_idx] == landmass_idx)
    {
        return;
    }


/* ¿ same block of code as AI_Reevaluate_Continent() ? */

    landmass_node_centroid_wx = 0;
    landmass_node_centroid_wy = 0;
    landmass_node_count = 0;
    landmass_node_index = _ai_landmass_dock_squares_heads[wp][_ai_landmass_war_targets[wp][player_idx]];
    while(landmass_node_index != ST_UNDEFINED)
    {
        landmass_node_centroid_wx += _ai_landmass_dock_squares_wx_array[wp][landmass_node_index];
        landmass_node_centroid_wy += _ai_landmass_dock_squares_wy_array[wp][landmass_node_index];
        landmass_node_count++;
        landmass_node_index = _ai_landmass_dock_squares_lists[wp][landmass_node_index];
    }
    /* HACK */  /* OGBUG  divide by zero, should `if(landmass_node_count > 0)` */
    /* HACK */  if (landmass_node_count == 0)
    /* HACK */  {
    /* HACK */      landmass_node_centroid_wx = 0;
    /* HACK */      landmass_node_centroid_wy = 0;
    /* HACK */  }
    /* HACK */  else
    /* HACK */  {
    /* HACK */      landmass_node_centroid_wx /= landmass_node_count;
    /* HACK */      landmass_node_centroid_wy /= landmass_node_count;
    /* HACK */  }
    min_delta_distance = 1000;
    landmass_node_index = _ai_landmass_dock_squares_heads[wp][landmass_idx];
    while(landmass_node_index != ST_UNDEFINED)
    {
        landmass_node_wx = _ai_landmass_dock_squares_wx_array[wp][landmass_node_index];
        landmass_node_wy = _ai_landmass_dock_squares_wy_array[wp][landmass_node_index];
        if(g_ai_evaluation_map[wp][((landmass_node_wy * WORLD_WIDTH) + landmass_node_wx)] == 0)
        {
            delta_distance = Delta_XY_With_Wrap(
                landmass_node_centroid_wx,
                landmass_node_centroid_wy,
                landmass_node_wx,
                landmass_node_wy,
                WORLD_WIDTH
            );
            if(delta_distance < min_delta_distance)
            {
                min_delta_distance = delta_distance;
                target_square_wx = landmass_node_wx;
                target_square_wy = landmass_node_wy;
            }
        }
        landmass_node_index = _ai_landmass_dock_squares_lists[wp][landmass_node_index];
    }

    /* NOTE(JimBalcomb,20260521): this is the only place in the whole code-base that sets lmt_Leaveable */
    if(min_delta_distance != 1000)
    {
        _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] = lmt_Leaveable;
        _ai_continents.plane[wp].player[player_idx].wx_array[landmass_idx] = (uint8_t)target_square_wx;
        _ai_continents.plane[wp].player[player_idx].wy_array[landmass_idx] = (uint8_t)target_square_wy;
    }

}


// WZD o158p07
/**
 * @brief Reposition roaming stacks toward a landmass stage point and prepare ferry handling.
 *
 * Reads the current stage-point coordinates stored for the specified landmass,
 * estimates how many unit slots remain open at that square, and then iterates
 * over all AI-owned stacks marked as `AISTK_Roamer`. Each non-empty roamer
 * stack is handed off to AI_Stacks_Setup_Ferry(), which may move the stack to
 * the stage point, queue ferry pickup coordinates, or issue ferry-related
 * orders for the stack's units.
 *
 * After each eligible stack is processed, this helper reduces the projected
 * stage-point capacity by that stack's unit count so later stacks are routed
 * using the remaining assumed space.
 *
 * @param landmass_idx Index of the landmass whose stage point is being filled.
 * @param wp World plane containing the landmass and its stage square.
 * @param player_idx Index of the AI player whose roaming stacks are being
 *                   relocated.
 *
 * @return This function does not return a value. It may indirectly update unit
 *         orders and ferry queues through AI_Stacks_Setup_Ferry().
 *
 * @note The available-space calculation reflects only the last stack already
 *       found on the exact stage square, then projects remaining capacity by
 *       subtracting each processed roamer stack in sequence.
 */
void AI_Stacks_Relocate_Roamers(int16_t landmass_idx, int16_t wp, int16_t player_idx)
{
    int16_t stage_wy = 0;
    int16_t stage_wx = 0;
    int16_t stack_wy = 0;
    int16_t stack_wx = 0;
    int16_t itr_stacks = 0;
    int16_t stage_count_slot_count = 0;

    /* Phase 1: Init */
    stage_wx = _ai_continents.plane[wp].player[player_idx].wx_array[landmass_idx];
    stage_wy = _ai_continents.plane[wp].player[player_idx].wy_array[landmass_idx];

    /* Phase 2: Count of Open Stack Slots at Landmass Stage-Point */
    stage_count_slot_count = 0;
    for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
    {
        stack_wx = _ai_own_stack_wx[itr_stacks];
        stack_wy = _ai_own_stack_wy[itr_stacks];
        if(
            (stack_wx == stage_wx)
            &&
            (stack_wy == stage_wy)
        )
        {
            stage_count_slot_count = (MAX_STACK - _ai_own_stack_unit_count[itr_stacks]);
        }
    }

    /* Phase 3: Setup Ferry for Valid, Roamer Stacks */ 
    for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
    {
        if(_ai_own_stack_type[itr_stacks] != AISTK_Roamer)
        {
            continue;
        }
        if(_ai_own_stack_unit_list[itr_stacks][0] == ST_UNDEFINED)
        {
            continue;
        }
        /* NOTE: OON XREF of AI_Stacks_Setup_Ferry() ~ per stack portion of parent function */
        AI_Stacks_Setup_Ferry(itr_stacks, landmass_idx, wp, stage_wx, stage_wy, stage_count_slot_count, player_idx);
        /* Deduct the units in this stack from the destination's projected available space */
        stage_count_slot_count -= _ai_own_stack_unit_count[itr_stacks];
    }

}


// WZD o158p08
void AI_Stacks_Order_To_War_Landmass(int16_t player_idx, int16_t wp)
{
    int16_t stack_wy = 0;
    int16_t stack_wx = 0;
    int16_t list_unit_count = 0;
    int16_t unit_idx = 0;
    int16_t itr_list_units = 0;
    int16_t itr_stacks = 0;
    int16_t war_landmass_idx = 0; // DNE in Dasm
    int16_t current_landmass_idx = 0; // DNE in Dasm
    int16_t war_landmass_stage_point_wx = 0;  // DNE in Dasm
    int16_t war_landmass_stage_point_wy = 0;  // DNE in Dasm

    war_landmass_idx = _ai_landmass_war_targets[wp][player_idx];
    if(war_landmass_idx == 0)
    {
        return;
    }
    for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
    {
        /* Filter stack types (only Non-Garrison and Unknown) */
        if(
            (_ai_own_stack_type[itr_stacks] != AISTK_Roamer)
            &&
            (_ai_own_stack_type[itr_stacks] != AISTK_Unknown)
        )
        {
            continue;
        }
        list_unit_count = _ai_own_stack_unit_count[itr_stacks];
        stack_wx = _ai_own_stack_wx[itr_stacks];
        stack_wy = _ai_own_stack_wy[itr_stacks];
        current_landmass_idx = _landmasses[((wp * WORLD_SIZE) + (stack_wy * WORLD_WIDTH) + stack_wx)];
        if(current_landmass_idx == war_landmass_idx)
        {
            continue;
        }
        for(itr_list_units = 0; itr_list_units < list_unit_count; itr_list_units++)
        {
            unit_idx = _ai_own_stack_unit_list[itr_stacks][itr_list_units];
            /* ¿ OGBUG  should check wind-walk and items ? */
            if(
                (Unit_Has_AirTravel(unit_idx) != ST_TRUE)
                &&
                (Unit_Has_WaterTravel(unit_idx) != ST_TRUE)
            )
            {
                continue;
            }
            if((_unit_type_table[_UNITS[unit_idx].type].Abilities & UA_MELD) != 0)
            {
                continue;
            }
            war_landmass_stage_point_wx = _ai_continents.plane[wp].player[player_idx].wx_array[war_landmass_idx];
            war_landmass_stage_point_wy = _ai_continents.plane[wp].player[player_idx].wy_array[war_landmass_idx];
#ifdef STU_DEBUG
//                        dbg_prn("AI_ORDERS: [PullMainWar] unit %d -> MainWarCont[%d][%d]=%d coords (%d,%d)\n", unit_idx, wp, player_idx, _ai_landmass_war_targets[wp][player_idx], war_landmass_stage_point_wx, war_landmass_stage_point_wy);
            LOG_DEBUG(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: %s: -> AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx=%d, target_wx=%d, target_wy=%d, stack_idx=%d, list_unit_idx=%d)", __FILE__, __LINE__, __FUNCTION__, unit_idx, war_landmass_stage_point_wx, war_landmass_stage_point_wy, itr_stacks, itr_list_units);
#endif
            g_ai_set_target_caller = 8;
            AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, war_landmass_stage_point_wx, war_landmass_stage_point_wy, itr_stacks, itr_list_units);
        }
    }
}


// WZD o158p09
/**
 * @brief Move one roaming stack toward a landmass stage point and prepare ferry support.
 *
 * This helper is called from AI_Stacks_Relocate_Roamers() for roaming stacks on
 * landmasses the AI wants to reinforce. It first determines an ocean square
 * adjacent to the destination stage point and registers that square in the AI
 * ferry-request queue through AI_Stacks_Ferry_Add_Location().
 *
 * If the stack is not already standing on the stage point, the function orders
 * as many units as there is projected room for toward the stage coordinates.
 * It then checks the stack's neighborhood for allied units already on ocean
 * squares, estimating available transport capacity and free stack slots there.
 * When such a nearby ocean group exists, eligible units are redirected to join
 * that square immediately; otherwise the function leaves a ferry request in
 * place and marks units to wait for ferry processing.
 *
 * @param stack_idx Index of the AI-owned stack being processed.
 * @param landmass_idx Index of the landmass currently being rallied. Preserved
 *                     locally for legacy logic, but not otherwise used here.
 * @param wp World plane containing the stack, stage point, and ferry request.
 * @param stage_wx World x-coordinate of the stage point for this landmass.
 * @param stage_wy World y-coordinate of the stage point for this landmass.
 * @param stage_count_slot_count Projected number of open unit slots available
 *                               at the stage point before this stack is applied.
 * @param player_idx Index of the AI player that owns the stack and nearby
 *                   transport candidates.
 *
 * @return This function does not return a value. It may enqueue ferry pickup
 *         coordinates in the `_ai_ferry_*` arrays and may assign movement or
 *         ferry-wait orders to units in `_ai_own_stack_unit_list[stack_idx]`.
 *
 * @note Several original-game quirks are intentionally preserved, including
 *       lack of bounds validation around adjacent-square scans and the fallback
 *       ferry request that can reuse the stack's current land square when no
 *       adjacent ocean square is found.
 */
void AI_Stacks_Setup_Ferry(int16_t stack_idx, int16_t landmass_idx, int16_t wp, int16_t stage_wx, int16_t stage_wy, int16_t stage_count_slot_count, int16_t player_idx)
{
    int16_t unit_type = 0;
    int16_t max_wy = 0;
    int16_t min_wy = 0;
    int16_t max_wx = 0;
    int16_t min_wx = 0;
    int16_t ocean_wy = 0;
    int16_t ocean_wx = 0;
    int16_t unit_idx = 0;
    int16_t list_unit_count = 0;
    int16_t stack_wy = 0;
    int16_t stack_wx = 0;
    int16_t free_stack_slots_on_square = 0;
    int16_t total_boat_capacity = 0;
    int16_t niu_embarkable_wy = 0;
    int16_t niu_embarkable_wx = 0;
    int16_t niu_Landmass_idx = 0;
    int16_t adjacent_unit_on_ocean_wy = 0;  // last-seen ocean unit's square
    int16_t adjacent_unit_on_ocean_wx = 0;  // last-seen ocean unit's square
    int16_t Adjacent_X = 0;
    int16_t wy_offset = 0;
    int16_t itr_units = 0;
    int16_t found_adjacent_unit_on_ocean = 0;
    int16_t niu_embarkable_found = 0;
    int16_t itr_list_units = 0;
    int16_t wx_offset = 0;
    int16_t unit_wx = 0;  // DNE in Dasm, reuses wx_offset
    int16_t unit_wy = 0;  // DNE in Dasm, reuses wy_offset
    int16_t itr_stack_wx = 0;  // DNE in Dasm, reuses Adjacent_X
    int16_t itr_stack_wy = 0;  // DNE in Dasm, reuses wy_offset


    /* Phase 1: Init*/
    niu_Landmass_idx = landmass_idx;
    stack_wx = _ai_own_stack_wx[stack_idx];
    stack_wy = _ai_own_stack_wy[stack_idx];
    ocean_wx = stack_wx;
    ocean_wy = stack_wy;


    /* Phase 2: Find adjacent Ocean square */
    /* OGBUG  should handle no ocean found */
    /* OGBUG  should validate coordinates */
    /* OGBUG  should early-exit on Ocean found */
    for(wy_offset = ST_UNDEFINED; wy_offset < 2; wy_offset++)
    {
        for(wx_offset = ST_UNDEFINED; wx_offset < 2; wx_offset++)
        {
            if(_landmasses[((wp * WORLD_SIZE) + ((stage_wy + wy_offset) * WORLD_WIDTH) + (stage_wx + wx_offset))] == 0)  /* Square is in Ocean landmass */
            {
                ocean_wx = (stage_wx + wx_offset);
                ocean_wy = (stage_wy + wy_offset);
            }
        }
    }


    /* Phase 3 */
    /* OGBUG  will add the original land square if there was no ocean around the stack */
    AI_Stacks_Ferry_Add_Location(ocean_wx, ocean_wy, wp);


    /* Phase 4 */
    /* If the stack is not at the staging point, move units to it */
    if(
        (stack_wx != stage_wx)
        ||
        (stack_wy != stage_wy)
    )
    {
        list_unit_count = _ai_own_stack_unit_count[stack_idx];
        for(itr_list_units = 0; itr_list_units < list_unit_count; itr_list_units++)
        {
            if(stage_count_slot_count <= 0)
            {
                break;
            }
            unit_idx = _ai_own_stack_unit_list[stack_idx][itr_list_units];
#ifdef STU_DEBUG
            LOG_DEBUG(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: %s: -> AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx=%d, target_wx=%d, target_wy=%d, stack_idx=%d, list_unit_idx=%d)", __FILE__, __LINE__, __FUNCTION__, unit_idx, stage_wx, stage_wy, stack_idx, itr_list_units);
#endif
            g_ai_set_target_caller = 9;
            AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, stage_wx, stage_wy, stack_idx, itr_list_units);
            stage_count_slot_count--;
        }
    }


    /* Phase 5 */
    /* DEDU  For what might this have been meant to be used? */
    /* NOTE(JimBalcomb,20260523): this block is in AI_Stacks_Reorder_Settle_Elsewhere(), used properly there */
    for(itr_stack_wy = (stack_wy - 1); (stack_wy + 2) > itr_stack_wy; itr_stack_wy++)
    {
        if(
            (itr_stack_wy > 0)
            &&
            (itr_stack_wy < WORLD_HEIGHT)
        )
        {
            for(itr_stack_wx = (stack_wx - 1); (stack_wx + 2) > itr_stack_wx; itr_stack_wx++)
            {
                wx_offset = itr_stack_wx;
                if(wx_offset < 0)
                {
                    wx_offset += WORLD_WIDTH;
                }
                if(wx_offset >= WORLD_WIDTH)
                {
                    wx_offset -= WORLD_WIDTH;
                }
                if(Map_Square_Is_Embarkable(wx_offset, itr_stack_wy, wp) != ST_FALSE)
                {
                    niu_embarkable_found = ST_TRUE;
                    niu_embarkable_wx = itr_stack_wx;
                    niu_embarkable_wy = itr_stack_wy;
                }
            }
        }
    }


    /* Phase 6 */
    /* Search all units for a nearby transport on the ocean */
    /* OGBUG  should skip center square */
    /* OGBUG  should track coordinates and capacity for where flag was set */
    found_adjacent_unit_on_ocean = ST_FALSE;
    min_wx = (stack_wx - 1);
    min_wy = (stack_wy - 1);
    max_wx = (stack_wx + 1);
    max_wy = (stack_wy + 1);
    total_boat_capacity = 0;
    free_stack_slots_on_square = MAX_STACK;
    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(_UNITS[itr_units].owner_idx != player_idx)
        {
            continue;
        }
        unit_wx = _UNITS[itr_units].wx;
        unit_wy = _UNITS[itr_units].wy;
        if(
            (unit_wx < min_wx)
            ||
            (unit_wy < min_wy)
            ||
            (unit_wx > max_wx)
            ||
            (unit_wy > max_wy)
        )
        {
            continue;
        }
        /* OGBUG  should use unit's wp */
        if(_landmasses[((wp * WORLD_SIZE) + (unit_wy * WORLD_WIDTH) + unit_wx)] != 0)
        {
            continue;
        }
        /* found any adjacent unit on an ocean square */
        found_adjacent_unit_on_ocean = ST_TRUE;
        adjacent_unit_on_ocean_wx = _UNITS[itr_units].wx;  // last-seen ocean unit's square
        adjacent_unit_on_ocean_wy = _UNITS[itr_units].wy;  // last-seen ocean unit's square
        unit_type = _UNITS[itr_units].type;
        /* Boat or Boatrider? */
        if(_unit_type_table[unit_type].Transport > 0)
        {
            _UNITS[itr_units].dst_wx = _UNITS[itr_units].wx;
            _UNITS[itr_units].dst_wy = _UNITS[itr_units].wy;
            _UNITS[itr_units].Status = us_GOTO;
            total_boat_capacity += _unit_type_table[unit_type].Transport;  // transport capacity for all nine squares?
        }
        else
        {
            if(
                (Unit_Has_AirTravel(itr_units) == ST_FALSE)
                &&
                (Unit_Has_WaterTravel(itr_units) == ST_FALSE)
            )
            {
                total_boat_capacity--;  // ← passenger needing ferry: deduct one
            }
        }
        free_stack_slots_on_square--;
    }


    /* Phase 7: Optimization - join neighbor or actively seek transport */
    if(found_adjacent_unit_on_ocean == ST_TRUE)
    {
        /* Phase 7a: override the ferry request */
        for(itr_list_units = 0; _ai_own_stack_unit_count[stack_idx] > itr_list_units; itr_list_units++)
        {
            if(itr_list_units < free_stack_slots_on_square)
            {
                if(total_boat_capacity > 0)
                {
                    unit_idx = _ai_own_stack_unit_list[stack_idx][itr_list_units];
#ifdef STU_DEBUG
                    LOG_DEBUG(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: %s: -> AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx=%d, target_wx=%d, target_wy=%d, stack_idx=%d, list_unit_idx=%d)", __FILE__, __LINE__, __FUNCTION__, unit_idx, adjacent_unit_on_ocean_wx, adjacent_unit_on_ocean_wy, stack_idx, itr_list_units);
#endif
                    g_ai_set_target_caller = 10;
                    AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, adjacent_unit_on_ocean_wx, adjacent_unit_on_ocean_wy, stack_idx, itr_list_units);
                    if(
                        (Unit_Has_AirTravel(unit_idx) == ST_FALSE)
                        ||
                        (Unit_Has_WaterTravel(unit_idx) == ST_FALSE)
                    )
                    {
                        total_boat_capacity--;
                    }
                }
            }
        }
    }
    else  /* (found_adjacent_unit_on_ocean == ST_FALSE) */
    {
        /* Phase 7b: set ferry request */
        /* would have been an optimization, because us_Ferry? otherwise, Phase 3 and 4 still set up the stage move and ferry dispatch */
        for(itr_list_units = 0; _ai_own_stack_unit_count[stack_idx] > itr_list_units; itr_list_units++)
        {
            if(itr_list_units < free_stack_slots_on_square)
            {
                /* unreachable - total_boat_capacity is always zero if there was no adjacent ocean unit found */
                if(total_boat_capacity > 0)
                {
                    unit_idx = _ai_own_stack_unit_list[stack_idx][itr_list_units];
                    AI_Stacks_Order_Ferry(unit_idx, stack_idx, itr_list_units);
                }
            }
        }
    }

}


// WZD o158p10
/**
 * @brief Rebuild the candidate target list for one player on one landmass.
 *
 * Clears the current `_ai_targets_*` arrays by resetting `_ai_targets_count`,
 * then scans the specified landmass on the given plane for strategic targets.
 * The function evaluates hostile fortresses and cities, hostile roaming stacks,
 * magic nodes, lairs, and towers, and appends each worthwhile target through
 * AI_Add_Target().
 *
 * The resulting parallel arrays are consumed later by
 * AI_Stacks_Assign_Target() when roaming stacks pick destinations.
 *
 * @param player_idx Index of the AI player building the target list.
 * @param landmass_idx Index of the landmass whose targets should be evaluated.
 * @param wp World plane containing the landmass.
 *
 * @note If no targets are added, this function marks the landmass as
 * `lmt_NoTargets` in `_ai_continents`.
 */
void AI_Build_Target_List(int16_t player_idx, int16_t landmass_idx, int16_t wp)
{
    int16_t target_strength = 0;
    int16_t target_value_base = 0;
    int16_t lair_wy = 0;
    int16_t lair_wx = 0;
    int16_t city_wy = 0;
    int16_t city_wx = 0;
    int16_t tower_wy = 0;
    int16_t tower_wx = 0;
    int16_t node_wy = 0;
    int16_t node_wx = 0;
    int16_t landmass_node_wy = 0;
    int16_t landmass_node_wx = 0;
    int16_t target_square_evaluation_value = 0;
    int16_t landmass_node_index = 0;
    int16_t target_owner_idx = 0;
    int16_t fortress_wy = 0;
    int16_t fortress_wx = 0;
    int16_t itr_players = 0;
    int16_t itr_cities = 0;
    int16_t itr_nodes = 0;
    int16_t itr_lairs = 0;
    int16_t itr_towers = 0;
    int16_t neutral_city_value = 0;  // DNE in Dasm


    /* Phase 1 */
    _ai_targets_count = 0;  /* start of "ai_targets" */


    /* Phase 2 */
    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        /* ¿ OGBUG  own fortress is not excluded - caught by _players[player_idx].Hostility[] check ? */
        if(_FORTRESSES[itr_players].wp != wp)
        {
            continue;
        }
        /* OGBUG  should be >= for {DIPL_War, DIPL_Crusade} */
        if(_players[player_idx].Dipl.Dipl_Status[itr_players] == DIPL_War)
        {
            target_value_base = 10;
        }
        else
        {
            target_value_base = 1;
        }
        /* Human Player Fortress is 25% more difficult/tougher */
        if(itr_players == HUMAN_PLAYER_IDX)
        {
            /* OGBUG  unset fortress_wx, fortress_wy */
            target_strength = ((g_ai_evaluation_map[wp][((fortress_wy * WORLD_WIDTH) + fortress_wx)] & AI_TARGET_STRENGTH_MASK) * 5 / 4);
        }
        else
        {
            /* OGBUG  unset fortress_wx, fortress_wy */
            target_strength = (g_ai_evaluation_map[wp][((fortress_wy * WORLD_WIDTH) + fortress_wx)] & AI_TARGET_STRENGTH_MASK);
        }
        fortress_wx = _FORTRESSES[itr_players].wx;
        fortress_wy = _FORTRESSES[itr_players].wy;
        if(
            (_landmasses[((wp * WORLD_SIZE) + (fortress_wy * WORLD_WIDTH) + fortress_wx)] == landmass_idx)
            &&
            (_players[player_idx].Hostility[itr_players] >= 3)  /* >= I might kill you or I WILLL DESTROY YOU! */
        )
        {
            AI_Add_Target(fortress_wx, fortress_wy, target_strength, (target_value_base * 500));
        }
    }

    
    /* Phase 3 */
/*
    BEGIN:  Enemy Cities
*/
    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(_CITIES[itr_cities].wp != wp)
        {
            continue;
        }
        target_owner_idx = _CITIES[itr_cities].owner_idx;
        city_wx = _CITIES[itr_cities].wx;
        city_wy = _CITIES[itr_cities].wy;
        /* OGBUG  should be >= for {DIPL_War, DIPL_Crusade} */
        if(
            (target_owner_idx != NEUTRAL_PLAYER_IDX)
            &&
            (_players[player_idx].Dipl.Dipl_Status[target_owner_idx] == DIPL_War)
        )
        {
            target_value_base = 10;
        }
        else
        {
            target_value_base = 1;
        }
        /* OGBUG  should be target_owner_idx, not itr; c&p error from above? */
        /* Human Player Fortress is 25% more difficult/tougher */
        if(itr_cities == HUMAN_PLAYER_IDX)
        {
            target_strength = ((g_ai_evaluation_map[wp][((city_wy * WORLD_WIDTH) + city_wx)] & AI_TARGET_STRENGTH_MASK) * 5 / 4);
        }
        else
        {
            target_strength = (g_ai_evaluation_map[wp][((city_wy * WORLD_WIDTH) + city_wx)] & AI_TARGET_STRENGTH_MASK);
        }
        if(
            (target_owner_idx == NEUTRAL_PLAYER_IDX)
            &&
            (_landmasses[((wp * WORLD_SIZE) + (city_wy * WORLD_WIDTH) + city_wx)] == landmass_idx)
        )
        {
            neutral_city_value = ((((_CITIES[itr_cities].population + _CITIES[itr_cities].bldg_cnt) * 10) + 100) * target_value_base);
            AI_Add_Target(city_wx, city_wy, target_strength, neutral_city_value);
        }
        else
        {
            if(target_owner_idx == player_idx)
            {
                continue;
            }
            if(_players[player_idx].Hostility[target_owner_idx] < 3)
            {
                continue;
            }
            /* OGBUG  duplicate city_wx,city_wy */
            city_wx = _CITIES[itr_cities].wx;
            city_wy = _CITIES[itr_cities].wy;
            if(_landmasses[((wp * WORLD_SIZE) + (city_wy * WORLD_WIDTH) + city_wx)] != landmass_idx)
            {
                continue;
            }
            if(_players[player_idx].Hostility[target_owner_idx] >= 3)
            {
                AI_Add_Target(city_wx, city_wy, target_strength, ((((_CITIES[itr_cities].population + _CITIES[itr_cities].bldg_cnt) * 10) + 100) * target_value_base));
            }
/* DEAD */  else if(_players[player_idx].Hostility[target_owner_idx] >= 2)
/* DEAD */  {
/* DEAD */      if(target_owner_idx != HUMAN_PLAYER_IDX)
/* DEAD */      {
/* DEAD */          AI_Add_Target(city_wx, city_wy, target_strength, (((_CITIES[itr_cities].population + _CITIES[itr_cities].bldg_cnt) * 5) + 50));
/* DEAD */      }
/* DEAD */      else
/* DEAD */      {
/* DEAD */          AI_Add_Target(city_wx, city_wy, target_strength, (((_CITIES[itr_cities].population + _CITIES[itr_cities].bldg_cnt) * 10) + 100));
/* DEAD */      }
/* DEAD */  }
        }
    }
/*
    END:  Enemy Cities
*/


    /* Phase 4 */
/*
    BEGIN:  Enemy Stacks
*/
    landmass_node_index = _ai_landmass_land_squares_heads[wp][landmass_idx];
    while(landmass_node_index != ST_UNDEFINED)
    {
        landmass_node_wx = _ai_landmass_land_squares_wx_array[wp][landmass_node_index];
        landmass_node_wy = _ai_landmass_land_squares_wy_array[wp][landmass_node_index];
        target_square_evaluation_value = g_ai_evaluation_map[wp][((landmass_node_wy * WORLD_WIDTH) + landmass_node_wx)];
        if(target_square_evaluation_value != 0)
        {
            if(
                ((target_square_evaluation_value & AI_TARGET_NONHOSTILE) == 0)
                &&
                ((target_square_evaluation_value & AI_TARGET_SITE) == 0)
                &&
                ((target_square_evaluation_value & AI_TARGET_STRENGTH_MASK) != 0)  // just getting the value
            )
            {
                if(
                    (_FORTRESSES[player_idx].wp == wp)
                    &&
                    (_landmasses[((wp * WORLD_SIZE) + (_FORTRESSES[player_idx].wy * WORLD_WIDTH) + _FORTRESSES[player_idx].wx)] == landmass_idx)
                )
                {
                    /* defensive priority - value is full strength if our fortress is on this landmass */
                    AI_Add_Target(landmass_node_wx, landmass_node_wy, (target_square_evaluation_value & AI_TARGET_STRENGTH_MASK), (target_square_evaluation_value & AI_TARGET_STRENGTH_MASK));
                }
                else
                {
                    AI_Add_Target(landmass_node_wx, landmass_node_wy, (target_square_evaluation_value & AI_TARGET_STRENGTH_MASK), ((target_square_evaluation_value & AI_TARGET_STRENGTH_MASK) / 3));
                }
            }
        }
        landmass_node_index = _ai_landmass_land_squares_lists[wp][landmass_node_index];
    }
/*
    END:  Enemy Stacks
*/


    /* Phase 5 */
/*
    BEGIN:  Nodes
*/
    for(itr_nodes = 0; itr_nodes < NUM_NODES; itr_nodes++)
    {
        if(_NODES[itr_nodes].wp != wp)
        {
            continue;
        }
        target_owner_idx = _NODES[itr_nodes].owner_idx;
        if(target_owner_idx == player_idx)
        {
            continue;
        }
        /* OGBUG  WTF? (target_owner_idx != ST_UNDEFINED) accessing arrays with ST_UNDEFINED index? */
        if(
            (_players[player_idx].Hostility[target_owner_idx] < 2)
            &&
            (target_owner_idx != ST_UNDEFINED)
        )
        {
            continue;
        }
        node_wx = _NODES[itr_nodes].wx;
        node_wy = _NODES[itr_nodes].wy;
        if(_landmasses[((wp * WORLD_SIZE) + (node_wy * WORLD_WIDTH) + node_wx)] != landmass_idx)
        {
            continue;
        }
        /* Node Value - Owned vs. Unowned & Occupied */
        if(target_owner_idx != ST_UNDEFINED)
        {
            AI_Add_Target(node_wx, node_wy, (g_ai_evaluation_map[wp][((node_wy * WORLD_WIDTH) + node_wx)] & AI_TARGET_STRENGTH_MASK), ((_NODES[itr_nodes].power * 10) + 50));
        }
        else
        {
            if((g_ai_evaluation_map[wp][((node_wy * WORLD_WIDTH) + node_wx)] & AI_TARGET_STRENGTH_MASK) != 0)
            {
                AI_Add_Target(node_wx, node_wy, (g_ai_evaluation_map[wp][((node_wy * WORLD_WIDTH) + node_wx)] & AI_TARGET_STRENGTH_MASK), ((_NODES[itr_nodes].power * 10) + 25));
            }
        }
    }
/*
    END:  Nodes
*/


    /* Phase 6 */
/*
    BEGIN:  Lairs
*/
    for(itr_lairs = 0; itr_lairs < NUM_LAIRS; itr_lairs++)
    {
        if(_LAIRS[itr_lairs].wp != wp)
        {
            continue;
        }
        if(_LAIRS[itr_lairs].intact != ST_TRUE)
        {
            continue;
        }
        lair_wx = _LAIRS[itr_lairs].wx;
        lair_wy = _LAIRS[itr_lairs].wy;
        if(_landmasses[((wp * WORLD_SIZE) + (lair_wy * WORLD_WIDTH) + lair_wx)] != landmass_idx)
        {
            continue;
        }
        AI_Add_Target(lair_wx, lair_wy, (g_ai_evaluation_map[wp][((lair_wy * WORLD_WIDTH) + lair_wx)] & AI_TARGET_STRENGTH_MASK), 50);
    }
/*
    END:  Lairs
*/


    /* Phase 7 */
/*
    BEGIN:  Towers
*/
    if(_FORTRESSES[player_idx].wp == wp)
    {
        for(itr_towers = 0; itr_towers < NUM_TOWERS; itr_towers++)
        {
            target_owner_idx = _TOWERS[itr_towers].owner_idx;
            tower_wx = _TOWERS[itr_towers].wx;
            tower_wy = _TOWERS[itr_towers].wy;
            if(_landmasses[((wp * WORLD_SIZE) + (tower_wy * WORLD_WIDTH) + tower_wx)] != landmass_idx)
            {
                continue;
            }
            if(AI_Tower_Target_Worthwhile(player_idx, tower_wx, tower_wy, wp) == ST_FALSE)
            {
                continue;
            }
            if(
                (target_owner_idx == player_idx)
                &&
                (_landmasses[((wp * WORLD_SIZE) + (tower_wy * WORLD_WIDTH) + tower_wx)] == landmass_idx)
            )
            {
                AI_Add_Target(tower_wx, tower_wy, 0, 1);
            }
            else
            {
                /* OGBUG  WTF? (target_owner_idx != ST_UNDEFINED) accessing arrays with ST_UNDEFINED index? */
                if(
                    (_players[player_idx].Hostility[target_owner_idx] < 2)
                    &&
                    (target_owner_idx != ST_UNDEFINED)
                )
                {
                    continue;
                }
                {
                    /* Tower Value - Owned & Hostile vs. Unowned */
                    if(
                        (target_owner_idx > ST_UNDEFINED)
                        &&
                        (_players[player_idx].Hostility[target_owner_idx] >= 3)
                    )
                    {
                        AI_Add_Target(tower_wx, tower_wy, (g_ai_evaluation_map[wp][((tower_wy * WORLD_WIDTH) + tower_wx)] & AI_TARGET_STRENGTH_MASK), 10);
                    }
                    else
                    {
                        if(target_owner_idx == ST_UNDEFINED)
                        {
                            AI_Add_Target(tower_wx, tower_wy, (g_ai_evaluation_map[wp][((tower_wy * WORLD_WIDTH) + tower_wx)] & AI_TARGET_STRENGTH_MASK), 150);
                        }
                    }
                }
            }
        }
    }
/*
    END:  Towers
*/


    /* Phase 7 */
    /* NOTE(JimBalcomb,20260521): this is the only place lmt_NoTargets is set in the whole code-base */
    if(_ai_targets_count == 0)
    {
        _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] = lmt_NoTargets;
    }

}


// WZD o158p11
/**
 * @brief Append a candidate target to the current AI target list.
 *
 * Stores the target coordinates, estimated defending strength, and strategic
 * value at the next free slot in the parallel `_ai_targets_*` arrays.
 *
 * @param wx World x-coordinate of the target square.
 * @param wy World y-coordinate of the target square.
 * @param strength Estimated combat strength associated with the target.
 * @param value Strategic value used later when rating candidate targets.
 *
 * @note If `_ai_targets_count` has already reached `MAX_AI_TARGETS`, this
 * function leaves the target list unchanged.
 */
void AI_Add_Target(int16_t wx, int16_t wy, int16_t strength, int16_t value)
{
    if(_ai_targets_count < MAX_AI_TARGETS)
    {
        _ai_targets_wx[_ai_targets_count] = wx;
        _ai_targets_wy[_ai_targets_count] = wy;
        _ai_targets_strength[_ai_targets_count] = strength;
        _ai_targets_value[_ai_targets_count] = value;
        _ai_targets_count++;
    }
}


// WZD o158p12
/**
 * @brief Choose the best available target for one AI stack.
 *
 * Computes the stack's effective strength from the units listed in
 * `_ai_own_stack_unit_list[unit_list_idx]`, then scans the current
 * `_ai_targets_*` arrays for the highest-valued reachable target that the
 * stack is strong enough to attack. Target scoring is based on target value
 * reduced by distance, and when the stack's last move failed the function also
 * requires `Make_Move_Path()` to find a valid path.
 *
 * On success, the chosen target coordinates are written through the output
 * pointers and the selected target is consumed by clearing its entry in
 * `_ai_targets_value[]`.
 *
 * @param stack_wx World x-coordinate of the stack being assigned.
 * @param stack_wy World y-coordinate of the stack being assigned.
 * @param target_wx Output pointer that receives the chosen target x-coordinate.
 * @param target_wy Output pointer that receives the chosen target y-coordinate.
 * @param unit_list_idx Index of the AI stack within `_ai_own_stack_*` arrays.
 * @param niu_list_unit_idx Unused legacy parameter preserved to match the
 * existing call signature.
 *
 * @return `ST_TRUE` if a target was selected and written to `target_wx` and
 * `target_wy`; otherwise `ST_FALSE`.
 */
int16_t AI_Stacks_Assign_Target(int16_t stack_wx, int16_t stack_wy, int16_t * target_wx, int16_t * target_wy, int16_t unit_list_idx, int16_t niu_list_unit_idx)
{
    int16_t last_move_failed = 0;
    int16_t player_idx = 0;
    int16_t wp = 0;
    int16_t effective_unit_strength = 0;
    int16_t delta_distance = 0;
    int16_t target_weighted_value = 0;
    int16_t targets_idx = 0;
    int16_t highest_target_value = 0;
    int16_t effective_stack_strength = 0;
    int16_t unit_idx = 0;
    int16_t list_unit_count = 0;
    int16_t itr_list_units = 0;
    int16_t itr_targets = 0;
    int16_t return_value = 0;  // DNE in Dasm


    /* Phase 1 */
/* DEAD */  itr_list_units = niu_list_unit_idx;
    list_unit_count = _ai_own_stack_unit_count[unit_list_idx];


    /* Phase 2 */
    /* Calculate the total effective strength of the stack */
    effective_stack_strength = 0;
    for(itr_list_units = 0; itr_list_units < list_unit_count; itr_list_units++)
    {
        unit_idx = _ai_own_stack_unit_list[unit_list_idx][itr_list_units];
        if(unit_idx != ST_UNDEFINED)
        {
            wp = _UNITS[unit_idx].wp;
            player_idx = _UNITS[unit_idx].owner_idx;
            /* ¿ OGBUG  should be once true, always true (ever false), for the whole stack ? */
            last_move_failed = _UNITS[unit_idx].Move_Failed;
            effective_unit_strength = (Effective_Unit_Strength(unit_idx) / 10);
            effective_stack_strength += effective_unit_strength;
        }
    }


    /* Phase 3 */
    highest_target_value = 0;
    targets_idx = ST_UNDEFINED;
    for(itr_targets = 0; itr_targets < _ai_targets_count; itr_targets++)
    {
        if (_ai_targets_value[itr_targets] <= 0)
        {
            continue;
        }
        /* Strength Check: Stack must be stronger than 75% of target strength */
        /* Calculation: (target_strength * 3) / 4 */
        if((_ai_targets_strength[itr_targets] * 3 / 4) >= effective_stack_strength)
        {
            continue;
        }
        {
            delta_distance = Delta_XY_With_Wrap(_ai_targets_wx[itr_targets], _ai_targets_wy[itr_targets], stack_wx, stack_wy, WORLD_WIDTH);
            SETMIN(delta_distance,1);
            target_weighted_value = (_ai_targets_value[itr_targets] / (delta_distance * 4));
            SETMIN(target_weighted_value,1);
            if(target_weighted_value > highest_target_value)
            {
                if(
                    (last_move_failed == ST_FALSE)
                    ||
                    (
                        Make_Move_Path(
                            0, 
                            0, 
                            0, 
                            0, 
                            0, 
                            0, 
                            stack_wx, 
                            stack_wy, 
                            _ai_targets_wx[itr_targets], 
                            _ai_targets_wy[itr_targets], 
                            wp, 
                            &movepath_x_array[2], 
                            &movepath_y_array[2], 
                            &movepath_cost_array[0], 
                            1, 
                            4, 
                            list_unit_count, 
                            list_unit_count, 
                            player_idx
                        ) > 0
                    )
                )
                {
                    highest_target_value = target_weighted_value;
                    targets_idx = itr_targets;
                }
            }
        }
    }


    /* Phase 4 */
    if(targets_idx == ST_UNDEFINED)
    {
        return_value = ST_FALSE;
    }
    else
    {
        *target_wx = _ai_targets_wx[targets_idx];
        *target_wy = _ai_targets_wy[targets_idx];
        _ai_targets_value[targets_idx] = 0;  /* *consume* the chosen target */
        return_value = ST_TRUE;
    }
    return return_value;

}


// WZD o158p13
/**
 * @brief Rebuild AI-owned stack state for one landmass and issue immediate roamer orders.
 *
 * Clears the `_ai_own_stack_*` working arrays, resets the expedition staging
 * counters, then scans all units belonging to `player_idx` on the selected
 * plane. Units standing on `landmass_idx` are grouped into stack records by
 * shared coordinates and plane, with busy units stored as `ST_UNDEFINED` in
 * the per-stack unit list while still contributing to stack counts.
 *
 * During this rebuild the function also counts units already standing on the
 * current stage point in `cp_staged_unit_count` and units already travelling
 * there in `cp_enroute_unit_count`. Newly created stacks are classified as
 * fortress garrisons, ordinary garrisons, or roamers based on the evaluation
 * map and fortress coordinates.
 *
 * After stack construction, each roamer stack is checked for a ready lead unit.
 * If one exists and AI_Stacks_Target_Nearest_Hostile_Stack() finds a nearby
 * hostile roaming stack, all units in the roamer are ordered toward that
 * target. Otherwise, small inactive roamers are downgraded to `AISTK_Unknown`
 * and failed target searches reset their units to `us_Ready`.
 *
 * @param player_idx Index of the AI player whose units are being grouped.
 * @param landmass_idx Index of the landmass whose units and stage point are
 *                     being processed.
 * @param wp World plane containing the landmass and candidate units.
 *
 * @return This function does not return a value. It updates the global
 *         `_ai_own_stack_*` working state and may issue unit orders.
 */
void AI_Stacks_Init_Build_Target_Order(int16_t player_idx, int16_t landmass_idx, int16_t wp)
{
    int16_t first_unit_idx = 0;
    int16_t target_wy = 0;
    int16_t target_wx = 0;
    int16_t stack_idx = 0;
    int16_t unit_wp = 0;
    int16_t unit_wy = 0;
    int16_t unit_wx = 0;
    int16_t itr_stacks1 = 0;
    int16_t itr_units1 = 0;
    int16_t itr_stacks2 = 0;
    int16_t itr_units2 = 0;  // DNE in Dasm, reuses itr_stacks2

#ifdef STU_DEBUG
    LOG_INFO(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: BEGIN: AI_Stacks_Init_Build_Target_Order()", __FILE__, __LINE__);
    LOG_DEBUG(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: BEGIN: AI_Stacks_Init_Build_Target_Order()", __FILE__, __LINE__);
    LOG_TRACE(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: BEGIN: AI_Stacks_Init_Build_Target_Order()", __FILE__, __LINE__);
#endif


    /* Phase 1: Init */
    _ai_own_stack_count = 0;
    cp_enroute_unit_count = 0;  /* only used by AI_Stacks_Stage_Expedition_Forces() */
    cp_staged_unit_count = 0;   /* only used by AI_Stacks_Stage_Expedition_Forces() */
    for(itr_stacks1 = 0; itr_stacks1 < MAX_AI_STACKS; itr_stacks1++)
    {
        _ai_own_stack_wx[itr_stacks1] = ST_UNDEFINED;
        _ai_own_stack_wy[itr_stacks1] = ST_UNDEFINED;
        _ai_own_stack_wp[itr_stacks1] = ST_UNDEFINED;
        _ai_own_stack_unit_count[itr_stacks1] = 0;
    }


    /* Phase 2 */
    /* Iterate through all units to group them into stacks by location */
    for(itr_units1 = 0; itr_units1 < _units; itr_units1++)
    {
        if
        (
            /* ¿ OGBUG  should break on >= MAX_AI_STACKS ? */
            (_ai_own_stack_count >= MAX_AI_STACKS)
            ||
            (_UNITS[itr_units1].owner_idx != player_idx)
        )
        {
            continue;
        }
        unit_wp = _UNITS[itr_units1].wp;
        if(unit_wp != wp)
        {
            continue;
        }
        unit_wx = _UNITS[itr_units1].wx;
        unit_wy = _UNITS[itr_units1].wy;
        /* Check if unit is at the staging landmass coordinates */
        if(
            (cp_landmass_wx_array[landmass_idx] == unit_wx)
            &&
            (cp_landmass_wy_array[landmass_idx] == unit_wy)
        )
        {
            cp_staged_unit_count++;
        }
        if(_landmasses[((unit_wp * WORLD_SIZE) + (unit_wy * WORLD_WIDTH) + unit_wx)] != landmass_idx)
        {
            continue;
        }
        /* Check if an AI stack already exists at this location */
        stack_idx = ST_UNDEFINED;
        for(itr_stacks2 = 0; itr_stacks2 < _ai_own_stack_count; itr_stacks2++)
        {
            if(
                (_ai_own_stack_wx[itr_stacks2] == unit_wx)
                &&
                (_ai_own_stack_wy[itr_stacks2] == unit_wy)
                &&
                (_ai_own_stack_wp[itr_stacks2] == unit_wp)
            )
            {
                stack_idx = itr_stacks2;
                /* ¿ OGBUG  should break here ? */
            }
        }

/*
    BEGIN:  Existing or New
*/
        if(stack_idx != ST_UNDEFINED)
        {
            if(
                (_UNITS[itr_units1].Status != us_GOTO)
                &&
                (_UNITS[itr_units1].Status != us_Purify)
                &&
                (_UNITS[itr_units1].Status != us_BuildRoad)
            )
            {
                // *free*
                _ai_own_stack_unit_list[stack_idx][_ai_own_stack_unit_count[stack_idx]] = itr_units1;
            }
            else
            {
                // *busy*
                _ai_own_stack_unit_list[stack_idx][_ai_own_stack_unit_count[stack_idx]] = ST_UNDEFINED;
                if(
                    (_UNITS[itr_units1].dst_wx == cp_landmass_wx_array[landmass_idx])
                    &&
                    (_UNITS[itr_units1].dst_wy == cp_landmass_wy_array[landmass_idx])
                )
                {
                    cp_enroute_unit_count++;
                }
            }
            _ai_own_stack_unit_count[stack_idx]++;
        }
        else
        {
            /* Create a new AI stack entry */
            _ai_own_stack_wx[_ai_own_stack_count] = (unsigned char)unit_wx;
            _ai_own_stack_wy[_ai_own_stack_count] = (unsigned char)unit_wy;
            _ai_own_stack_wp[_ai_own_stack_count] = (unsigned char)unit_wp;
            _ai_own_stack_unit_count[_ai_own_stack_count] = 1;
            if(
                (_UNITS[itr_units1].Status != us_GOTO)
                &&
                (_UNITS[itr_units1].Status != us_Purify)
                &&
                (_UNITS[itr_units1].Status != us_BuildRoad)
            )
            {
                // *free*
                _ai_own_stack_unit_list[_ai_own_stack_count][0] = itr_units1;
            }
            else
            {
                // *busy*
                _ai_own_stack_unit_list[_ai_own_stack_count][0] = ST_UNDEFINED;
                if(
                    (_UNITS[itr_units1].dst_wx == cp_landmass_wx_array[landmass_idx])
                    &&
                    (_UNITS[itr_units1].dst_wy == cp_landmass_wy_array[landmass_idx])
                )
                {
                    cp_enroute_unit_count++;
                }
            }
            /* Classify the new stack */
            /* weird but clever way to check if a stack is effectively a garrison - it's on a City, Node, or Tower (or, but probably not, on a Lair or Enemy Stack) */
            if(g_ai_evaluation_map[unit_wp][((unit_wy * WORLD_WIDTH) + unit_wx)] != 0)
            {
                /* Is the *site* our Fortress City? */
                if(
                    (_FORTRESSES[player_idx].wx == unit_wx)
                    &&
                    (_FORTRESSES[player_idx].wy == unit_wy)
                    &&
                    (_FORTRESSES[player_idx].wp == unit_wp)
                )
                {
                    _ai_own_stack_type[_ai_own_stack_count] = AISTK_FortressGarrison;
                }
                else
                {
                    _ai_own_stack_type[_ai_own_stack_count] = AISTK_Garrison;
                }
            }
            else
            {
                _ai_own_stack_type[_ai_own_stack_count] = AISTK_Roamer;
            }
            _ai_own_stack_count++;
        }
/*
    END:  Existing or New
*/
    }

    /* Phase 3 */
    /* IDA: Ocean Blue  color#14 */
    /* Process the built stacks and assign orders, non-garrison only */
    for(itr_stacks1 = 0; itr_stacks1 < _ai_own_stack_count; itr_stacks1++)
    {
        if (_ai_own_stack_type[itr_stacks1] != AISTK_Roamer)
        {
            continue;
        }
        /* If the stack contains a valid primary unit ready to move */
        first_unit_idx = _ai_own_stack_unit_list[itr_stacks1][0];
        if(
            (first_unit_idx == ST_UNDEFINED)
            ||
            (_UNITS[first_unit_idx].Status != us_Move)
        )
        {
            if((_ai_own_stack_unit_count[itr_stacks1] + 1) < 3)  /* NOTE(JimBalcomb,20260522): double-checked, this really does weirdly test +1<3 */
            {
                _ai_own_stack_type[itr_stacks1] = AISTK_Unknown;
            }
        }
        else  /* (first_unit_idx != ST_UNDEFINED) && (_UNITS[first_unit_idx].Status == us_Move) */
        {
            /* Redirect to attack nearest hostile free-roaming stack */
            if(AI_Stacks_Target_Nearest_Hostile_Stack(itr_stacks1, landmass_idx, &target_wx, &target_wy, wp) == ST_TRUE)
            {
                /* Assign move orders to all units in the stack */
                for(itr_units2 = 0; _ai_own_stack_unit_count[itr_stacks1] > itr_units2; itr_units2++)
                {
#ifdef STU_DEBUG
                    LOG_DEBUG(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: %s: -> AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx=%d, target_wx=%d, target_wy=%d, stack_idx=%d, list_unit_idx=%d)", __FILE__, __LINE__, __FUNCTION__, _ai_own_stack_unit_list[itr_stacks1][itr_units2], target_wx, target_wy, itr_stacks1, itr_units2);
#endif
                    g_ai_set_target_caller = 11;
                    AI_Stacks_Order_Attack_Target_Or_Goto_Destination(_ai_own_stack_unit_list[itr_stacks1][itr_units2], target_wx, target_wy, itr_stacks1, itr_units2);
                }
            }
            else
            {
                /* (first_unit_idx != ST_UNDEFINED) && (_UNITS[first_unit_idx].Status == us_Move) */
                /* (AI_Stacks_Target_Nearest_Hostile_Stack(itr_stacks1, landmass_idx, &target_wx, &target_wy, wp) == ST_FALSE) */
                for(itr_units2 = 0; _ai_own_stack_unit_count[itr_stacks1] > itr_units2; itr_units2++)
                {
                    _UNITS[_ai_own_stack_unit_list[itr_stacks1][itr_units2]].Status = us_Ready;
                }
                _ai_own_stack_type[itr_stacks1] = AISTK_Unknown;
            }
        }
    }

#ifdef STU_DEBUG
    LOG_INFO(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: END: AI_Stacks_Init_Build_Target_Order()", __FILE__, __LINE__);
    LOG_DEBUG(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: END: AI_Stacks_Init_Build_Target_Order()", __FILE__, __LINE__);
    LOG_TRACE(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: END: AI_Stacks_Init_Build_Target_Order()", __FILE__, __LINE__);
#endif

}


// WZD o158p14
/**
 * @brief Find the nearest hostile non-site stack for one AI stack to attack.
 *
 * Scans the land-square list for the specified landmass and looks for map
 * squares whose evaluation value indicates hostile unit strength, while
 * excluding squares marked as non-hostile or as sites. Among those candidates,
 * it chooses the one with the smallest wrapped distance from the requesting AI
 * stack.
 *
 * @param stack_idx Index of the AI-owned stack whose current position is used
 * as the search origin.
 * @param landmass_idx Index of the landmass to search.
 * @param target_wx Output pointer that receives the chosen target x-coordinate
 * when a hostile stack is found.
 * @param target_wy Output pointer that receives the chosen target y-coordinate
 * when a hostile stack is found.
 * @param wp World plane containing both the stack and the searched landmass.
 *
 * @return `ST_TRUE` if a hostile non-site stack was found and written to the
 * output pointers; otherwise `ST_FALSE`.
 */
int16_t AI_Stacks_Target_Nearest_Hostile_Stack(int16_t stack_idx, int16_t landmass_idx, int16_t * target_wx, int16_t * target_wy, int16_t wp)
{
    int16_t landmass_node_wy = 0;
    int16_t landmass_node_wx = 0;
    int16_t stack_wy = 0;
    int16_t stack_wx = 0;
    int16_t l_target_wy = 0;
    int16_t l_target_wx = 0;
    int16_t min_delta_distance = 0;
    int16_t delta_distance = 0;
    int16_t target_square_evaluation_value = 0;
    int16_t landmass_node_index = 0;


    /* Phase 1 */
    min_delta_distance = 1000;

    stack_wx = _ai_own_stack_wx[stack_idx];
    stack_wy = _ai_own_stack_wy[stack_idx];


    /* Phase 2 */
    landmass_node_index = _ai_landmass_land_squares_heads[wp][landmass_idx];
    while(landmass_node_index != ST_UNDEFINED)
    {
        landmass_node_wx = _ai_landmass_land_squares_wx_array[wp][landmass_node_index];
        landmass_node_wy = _ai_landmass_land_squares_wy_array[wp][landmass_node_index];
        /* map square has some enemy unit(s) on it and not a Site */
        target_square_evaluation_value = g_ai_evaluation_map[wp][((landmass_node_wy * WORLD_WIDTH) + landmass_node_wx)];
        if(
            ((target_square_evaluation_value & AI_TARGET_STRENGTH_MASK) != 0)
            &&
            ((target_square_evaluation_value & AI_TARGET_NONHOSTILE) == 0)
            &&
            ((target_square_evaluation_value & AI_TARGET_SITE) == 0)
        )
        {
            delta_distance = Delta_XY_With_Wrap(stack_wx, stack_wy, landmass_node_wx, landmass_node_wy, WORLD_WIDTH);
            if(delta_distance < min_delta_distance)
            {
                min_delta_distance = delta_distance;
                l_target_wx = landmass_node_wx;
                l_target_wy = landmass_node_wy;
            }
        }
        landmass_node_index = _ai_landmass_land_squares_lists[wp][landmass_node_index];
    }


    /* Phase 3 */
    if(min_delta_distance < 1000)
    {
        *target_wx = l_target_wx;
        *target_wy = l_target_wy;
        return ST_TRUE;
    }
    else
    {
        return ST_FALSE;
    }

}


// WZD o158p15
/*
vs. AI_Stacks_Target_Nearest_Hostile_Stack()?
*/
void AI_Find_Opportunity_City_Target(int16_t wp, int16_t player_idx)
{
    int16_t defender_count = 0;
    int16_t city_wp = 0;
    int16_t city_wy = 0;
    int16_t city_wx = 0;
    int16_t max_wy = 0;
    int16_t max_wx = 0;
    int16_t min_wy = 0;
    int16_t min_wx = 0;
    int16_t found_target = 0;
    int16_t stack_wp = 0;
    int16_t stack_wy = 0;
    int16_t stack_wx = 0;
    int16_t itr_units = 0;
    int16_t itr_stacks = 0;
    int16_t itr_cities = 0;
    
    for(itr_stacks = 0; itr_stacks < _ai_all_own_stack_count; itr_stacks++)
    {

        if(_ai_all_own_stacks[itr_stacks].unit_status != us_GOTO)
        {
            continue;
        }

        stack_wx = _ai_all_own_stacks[itr_stacks].wx;
        stack_wy = _ai_all_own_stacks[itr_stacks].wy;
        stack_wp = _ai_all_own_stacks[itr_stacks].wp;
        min_wx = (stack_wx - 1);
        min_wy = (stack_wy - 1);
        max_wx = (stack_wx + 1);
        max_wy = (stack_wy + 1);
        found_target = ST_FALSE;

        for(itr_cities = 0; ((itr_cities < _cities) && (found_target == ST_FALSE)); itr_cities++)
        {
            /* Search for an adjacent enemy city that is weakly defended */
            if (_CITIES[itr_cities].wx < min_wx) { continue; }
            if (_CITIES[itr_cities].wx > max_wx) { continue; }
            if (_CITIES[itr_cities].wy < min_wy) { continue; }
            if (_CITIES[itr_cities].wy > max_wy) { continue; }
            if (_CITIES[itr_cities].wp != wp) { continue; }
            if (_CITIES[itr_cities].owner_idx == player_idx) { continue; }

            /* Enemy city found in range, count its defenders */
            defender_count = 0;
            city_wx = _CITIES[itr_cities].wx;
            city_wy = _CITIES[itr_cities].wy;
            city_wp = _CITIES[itr_cities].wp;
            for(itr_units = 0; itr_units < _units; itr_units++)
            {
                if(
                    (_UNITS[itr_units].wx == city_wx)
                    &&
                    (_UNITS[itr_units].wy == city_wy)
                    &&
                    (_UNITS[itr_units].wp == city_wp)
                )
                {
                    defender_count++;
                }
            }
            /* If the city has fewer than 4 defenders, it's an "opportunity" */
            if(defender_count < 4)
            {
                found_target = ST_TRUE;
            }
        }

        /* If an opportunity was found, cancel the GOTO status for this stack and its units */
        if(found_target == ST_TRUE)
        {
            for(itr_units = 0; itr_units < _units; itr_units++)
            {
                if(
                    (_UNITS[itr_units].wx == stack_wx)
                    &&
                    (_UNITS[itr_units].wy == stack_wy)
                    &&
                    (_UNITS[itr_units].wp == stack_wp)
                )
                {
                    _UNITS[itr_units].Status = us_Ready;
                }
            }
            _ai_all_own_stacks[itr_stacks].unit_status = us_Ready;
        }

    }  /* END: for(itr_stacks = 0; itr_stacks < _ai_all_own_stack_count; itr_stacks++) */

}


// WZD o158p16
// drake178: AI_Balance_Upkeep()
/*
if food, gold, or mana income are negative, attempts
to find and disband the lowest value units until the
incomes reach at least zero

BUG: parameter mismatch when trying to determine
 the value of fantastic units
BUG? resets the units to plane 0 after disbanding
*/
/*

kill units until gold,food,mana income == upkeep

uses _ai_landmass_strength_ratios[][]

AI_Set_Unit_Orders(itr_players)
    |-> AI_Balance_Upkeep(itr_players)

*/
void AI_Disband_To_Balance_Budget(int16_t player_idx)
{
    int16_t niu_var_1C = 0;
    int16_t lowest_value_unit_upkeep = 0;
    int16_t unit_landmass_idx = 0;
    int16_t unit_wp = 0;
    int16_t mana_deficit = 0;
    int16_t gold_deficit = 0;
    int16_t food_deficit = 0;
    int16_t lowest_value_unit_idx = 0;
    int16_t unit_value = 0;
    int16_t lowest_unit_value = 0;
    int16_t tries = 0;
    int16_t mana_income = 0;
    int16_t food_income = 0;
    int16_t gold_income = 0;
    int16_t itr_units = 0;  // _DI_

    Player_Resource_Income_Total(player_idx, &gold_income, &food_income, &mana_income);

    mana_deficit = 0;
    food_deficit = 0;
    gold_deficit = 0;

    niu_var_1C = 0;  // was total?  MoO1 artifact?  myserious fourth resource?  wasn't there something like this elsewhere?

    if(food_income < 0)
    {

        food_deficit += -(food_income);

    }

    if(gold_income < 0)
    {

        gold_deficit += -(gold_income);

    }

    if(mana_income < 0)
    {

        mana_deficit += -(mana_income);

    }


    for(tries = 0; ((tries < 200) && ((food_deficit > 0) || (gold_deficit > 0))); tries++)
    {

        lowest_unit_value = 10000;

        lowest_value_unit_idx = ST_UNDEFINED;

        for(itr_units = 0; itr_units < _units; itr_units++)
        {

            if(
                (_UNITS[itr_units].type < ut_Magic_Spirit)
                &&
                (_UNITS[itr_units].owner_idx == player_idx)
            )
            {

                unit_wp = _UNITS[itr_units].wp;

                unit_landmass_idx = _landmasses[((unit_wp * WORLD_SIZE) + (_UNITS[itr_units].wy * WORLD_WIDTH) + _UNITS[itr_units].wx)];

                if(unit_landmass_idx > 0)
                {

                    unit_value = (Effective_Unit_Strength(itr_units) / 10);

                    if(_ai_landmass_strength_ratios[unit_wp][unit_landmass_idx] == 0)
                    {

                        unit_value *= 2;

                    }

                    if(unit_value < lowest_unit_value)
                    {

                        lowest_unit_value = unit_value;

                        lowest_value_unit_idx = itr_units;

                    }

                }

            }

        }

        if(lowest_value_unit_idx != ST_UNDEFINED)
        {

            food_deficit--;

            lowest_value_unit_upkeep = ((_unit_type_table[_UNITS[lowest_value_unit_idx].type].Upkeep * difficulty_modifiers_table[_difficulty].maintenance) / 100);

            if(lowest_value_unit_upkeep == 0)
            {

                lowest_value_unit_upkeep = 1;

            }

            gold_deficit -= lowest_value_unit_upkeep;

            Kill_Unit(lowest_value_unit_idx, kt_Normal);
// TODO  
            // ; BUG? resets the unit's plane to 0 afterward
            _UNITS[lowest_value_unit_idx].wp = 0;  // BUGBUG?  ...because, Kill_Unit() sets ST_UNDEFINED  ...but, why?
// TODO  
        }

    }


    for(tries = 0; ((tries < 200) && (mana_deficit > 0)); tries++)
    {

        // ; BUG: uses unit type instead of unit index when trying
        // ;  to determine threat value

        lowest_unit_value = 10000;

        lowest_value_unit_idx = ST_UNDEFINED;

        for(itr_units = 0; itr_units < _units; itr_units++)
        {

            if(
                (_UNITS[itr_units].type >= ut_Magic_Spirit)
                &&
                (_UNITS[itr_units].owner_idx == player_idx)
            )
            {

                unit_wp = _UNITS[itr_units].wp;

                unit_landmass_idx = _landmasses[((unit_wp * WORLD_SIZE) + (_UNITS[itr_units].wy * WORLD_WIDTH) + _UNITS[itr_units].wx)];

                if(unit_landmass_idx > 0)
                {

                    /* OGBUG ¿ should use Effective_Unit_Type_Strength() or pass itr_units ? */
                    unit_value = (Effective_Unit_Strength(_UNITS[itr_units].type) / 10);

                    if(_ai_landmass_strength_ratios[unit_wp][unit_landmass_idx] == 0)
                    {

                        unit_value *= 2;

                    }

                    if(unit_value < lowest_unit_value)
                    {

                        lowest_unit_value = unit_value;

                        lowest_value_unit_idx = itr_units;

                    }

                }

            }

        }

        if(lowest_value_unit_idx != ST_UNDEFINED)
        {

            lowest_value_unit_upkeep = ((_unit_type_table[_UNITS[lowest_value_unit_idx].type].Upkeep * difficulty_modifiers_table[_difficulty].maintenance) / 100);

            if(lowest_value_unit_upkeep == 0)
            {

                lowest_value_unit_upkeep = 1;

            }

            mana_deficit -= lowest_value_unit_upkeep;

            Kill_Unit(lowest_value_unit_idx, kt_Normal);
// TODO  
            // ; BUG? resets the unit's plane to 0 afterward
            _UNITS[lowest_value_unit_idx].wp = 0;  // BUGBUG?  ...because, Kill_Unit() sets ST_UNDEFINED  ...but, why?
// TODO  
        }

    }

}


// WZD o158p17
void AI_Shift_Off_Home_Plane(int16_t player_idx)
{
    int16_t itr_cities = 0;
    int16_t stack_wp = 0;
    int16_t stack_wy = 0;
    int16_t stack_wx = 0;
    int16_t other_wp = 0;
    int16_t itr_towers = 0;
    int16_t itr = 0;
    int16_t itr2 = 0;
    for(itr = 0; itr < _num_players; itr++)
    {
        if(_players[itr].Globals[PLANAR_SEAL] != 0)
        {
            return;
        }
    }
    other_wp = (1 - _FORTRESSES[player_idx].wp);
    for(itr2 = 0; itr2 < _ai_all_own_stack_count; itr2++)
    {
        stack_wp = _ai_all_own_stacks[itr2].wp;
        if(stack_wp != other_wp)
        {
            stack_wx = _ai_all_own_stacks[itr2].wx;
            stack_wy = _ai_all_own_stacks[itr2].wy;
            if(Map_Square_Troops_Can_Plane_Shift(stack_wx, stack_wy, stack_wp) == ST_TRUE)
            {
                for(itr_towers = 0; itr_towers < NUM_TOWERS; itr_towers++)
                {
                    if(
                        (_TOWERS[itr_towers].wx == stack_wx)
                        &&
                        (_TOWERS[itr_towers].wy == stack_wy)
                    )
                    {
                        for(itr = 0; itr < _units; itr++)
                        {
                            _ai_all_own_stacks[itr2].wp = (uint8_t)other_wp;
                        }
                    }
                }
                for(itr_cities = 0; itr_cities < _cities; itr_cities++)
                {
                    if(
                        (_CITIES[itr_cities].owner_idx == player_idx)
                        &&
                        (_CITIES[itr_cities].enchantments[ASTRAL_GATE] != 0)
                    )
                    {
                        if(
                            (_CITIES[itr_cities].wx == stack_wx)
                            &&
                            (_CITIES[itr_cities].wy == stack_wy)
                        )
                        {
                            for(itr = 0; itr < _units; itr++)
                            {
                                if(
                                    (_UNITS[itr].wx == (int8_t)stack_wx)
                                    &&
                                    (_UNITS[itr].wy == (int8_t)stack_wy)
                                )
                                {
                                    _UNITS[itr].wp = (int8_t)other_wp;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


// WZD o158p18
void AI_Move_Out_Boats(void)
{
    int16_t do_move_out = 0;
    int16_t adjacent_wy = 0;
    int16_t adjacent_wx = 0;
    int16_t itr_wy = 0;
    int16_t landmass_idx = 0;
    int16_t unit_wp = 0;
    int16_t unit_wy = 0;
    int16_t unit_wx = 0;
    int16_t itr_stack = 0;
    int16_t itr_wx = 0;

    for(itr_stack = 0; itr_stack < _ai_all_own_stack_count; itr_stack++)
    {
        if(
            (_ai_all_own_stacks[itr_stack].value != ST_UNDEFINED)
            &&
            ((_ai_all_own_stacks[itr_stack].abilities & AICAP_Transport) != 0)
        )
        {
            unit_wx = _ai_all_own_stacks[itr_stack].wx;
            unit_wy = _ai_all_own_stacks[itr_stack].wy;
            unit_wp = _ai_all_own_stacks[itr_stack].wp;
            landmass_idx = _landmasses[((unit_wp * WORLD_SIZE) + (unit_wy * WORLD_WIDTH) + unit_wx)];
            adjacent_wx = 0;
            adjacent_wy = 0;
            do_move_out = ST_FALSE;
            if(landmass_idx > 0)
            {
                for(itr_wy = ST_UNDEFINED; itr_wy < 2; itr_wy++)
                {
                    for(itr_wx = ST_UNDEFINED; itr_wx < 2; itr_wx++)
                    {
                        if(
                            (itr_wy != 0)
                            &&
                            (itr_wx != 0)
                            &&
                            ((itr_wy + unit_wy) < WORLD_HEIGHT)
                            &&
                            ((itr_wx + unit_wx) < WORLD_WIDTH)
                        )
                        {
                            if(Square_Is_OceanLike((itr_wx + unit_wx), (itr_wy + unit_wy), unit_wp) == ST_TRUE)
                            {
                                adjacent_wx = (unit_wx + itr_wx);
                                adjacent_wy = (unit_wy + itr_wy);
                                do_move_out = ST_TRUE;
                            }
                        }
                    }
                }
                if(do_move_out == ST_TRUE)
                {
                    AI_Stack_Set_Boats_Goto(itr_stack, adjacent_wx, adjacent_wy);
                }
            }
        }
    }
}


// WZD o158p19
/**
 * @brief Push non-military units out of garrison stacks.
 *
 * Scans AI stacks whose type is at least `AISTK_Garrison` and looks for units
 * that should not remain parked in a garrison square. Settlers and Engineers are
 * moved to an adjacent free square immediately. Melders are also pushed out
 * unless they are already standing on a magic node square on the specified
 * plane.
 *
 * When a valid adjacent square is found, the function issues a move/goto order
 * for the individual unit via AI_Stacks_Order_Attack_Target_Or_Goto_Destination().
 *
 * @param wp World plane used when checking whether a melder is already on a
 * magic node square.
 */
void AI_Stacks_Move_Out_NonMilitary_Garrisoned(int16_t wp)
{
    int16_t unit_is_on_node_square = 0;
    int16_t itr_nodes = 0;
    int16_t unit_wy = 0;
    int16_t unit_wx = 0;
    int16_t unit_idx = 0;
    int16_t unit_type = 0;
    int16_t stack_unit_count = 0;
    int16_t adjacent_wy = 0;
    int16_t adjacent_wx = 0;
    int16_t itr_stacks = 0;
    int16_t itr_stack_units = 0;


    /* Phase 1 */
    for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
    {
        if(_ai_own_stack_type[itr_stacks] < AISTK_Garrison)  /* Site Garrison or Fortress City Garrison */
        {
            continue;
        }
        stack_unit_count = _ai_own_stack_unit_count[itr_stacks];
        for(itr_stack_units = 0; itr_stack_units < stack_unit_count; itr_stack_units++)
        {
            unit_idx = _ai_own_stack_unit_list[itr_stacks][itr_stack_units];
            if(unit_idx == ST_UNDEFINED)
            {
                continue;
            }
            unit_type = _UNITS[unit_idx].type;
            /* Settlers & Engineers vs. Melders */
            if(
                ((_unit_type_table[unit_type].Abilities & UA_CREATEOUTPOST) != 0)
                ||
                (_unit_type_table[unit_type].Construction > 0)
            )
            {
                if(Adjacent_Free_Square(_ai_own_stack_wx[itr_stacks], _ai_own_stack_wy[itr_stacks], _ai_own_stack_wp[itr_stacks], &adjacent_wx, &adjacent_wy) == ST_TRUE)
                {
#ifdef STU_DEBUG
                    LOG_DEBUG(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: %s: -> AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx=%d, target_wx=%d, target_wy=%d, stack_idx=%d, list_unit_idx=%d)", __FILE__, __LINE__, __FUNCTION__, unit_idx, adjacent_wx, adjacent_wy, itr_stacks, itr_stack_units);
#endif
                    g_ai_set_target_caller = 12;
                    AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, adjacent_wx, adjacent_wy, itr_stacks, itr_stack_units);
                }
            }
            else if((_unit_type_table[unit_type].Abilities & UA_MELD) != 0)
            {
                unit_wx = _UNITS[unit_idx].wx;
                unit_wy = _UNITS[unit_idx].wy;
                unit_is_on_node_square = ST_FALSE;
                for(itr_nodes = 0; itr_nodes < NUM_NODES; itr_nodes++)
                {
                    /* ¿ OGBUG  should check if Nodes is owned ? */
                    if(
                        (_NODES[itr_nodes].wx == unit_wx)
                        &&
                        (_NODES[itr_nodes].wy == unit_wy)
                        &&
                        (_NODES[itr_nodes].wp == wp)
                    )
                    {
                        unit_is_on_node_square = ST_TRUE;
                        break;
                    }
                }

                /* If Melder is in a Garrison but NOT on a Node, push it out to find one */
                if(unit_is_on_node_square == ST_FALSE)
                {
                    if(Adjacent_Free_Square(_ai_own_stack_wx[itr_stacks], _ai_own_stack_wy[itr_stacks], _ai_own_stack_wp[itr_stacks], &adjacent_wx, &adjacent_wy) == ST_TRUE)
                    {
#ifdef STU_DEBUG
                        LOG_DEBUG(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: %s: -> AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx=%d, target_wx=%d, target_wy=%d, stack_idx=%d, list_unit_idx=%d)", __FILE__, __LINE__, __FUNCTION__, unit_idx, adjacent_wx, adjacent_wy, itr_stacks, itr_stack_units);
#endif
                        g_ai_set_target_caller = 13;
                        AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, adjacent_wx, adjacent_wy, itr_stacks, itr_stack_units);
                    }
                }
            }
        }
    }
}


// WZD o158p20
/**
 * @brief Rebuild the global expedition candidate pools from the current AI stacks.
 *
 * Clears the global pushout and seafaring expedition bookkeeping, then scans
 * every stack in `_ai_own_stack_*` and submits qualifying stacks to
 * AI_Stacks_Survey_Expedition_Forces_Stack(). Roaming and unknown stacks can
 * contribute all of their military units, while garrison-style stacks only
 * contribute units beyond the five-unit reserve threshold used here.
 *
 * The resulting global arrays are later consumed by stage and war-staging logic
 * that pulls expedition-capable units away from lower-priority duties.
 *
 * @return This function does not return a value. It resets and repopulates the
 *         global `G_Pushout_*`, `G_Seafaring_*`, `cp_drafted_unit_count`, and
 *         related lowest-value trackers.
 *
 * @note Fortress garrisons only contribute excess units during the early game
 *       (`_turn < 100`), preserving the current heuristic that avoids stripping
 *       the fortress city too aggressively in early turns.
 */
void AI_Stacks_Survey_Expedition_Forces(void)
{
    int16_t itr_stacks = 0;
    int16_t stack_unit_count = 0;

    /* Phase 1: Init */
    cp_drafted_unit_count = 0;
    G_Seafaring_Count = 0;
    G_Pushout_Lowest_Value = 10000;
    G_Seafaring_Lowest_Value = 10000;

    /* Phase 2: Submit qualifying stacks and counts */
    for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
    {
        stack_unit_count = _ai_own_stack_unit_count[itr_stacks];
        switch(_ai_own_stack_type[itr_stacks])
        {
            case AISTK_Unknown:
            {
                AI_Stacks_Survey_Expedition_Forces_Stack(itr_stacks, stack_unit_count, stack_unit_count);
            } break;
            case AISTK_Roamer:
            {
                if(_ai_own_stack_unit_list[itr_stacks][0] != ST_UNDEFINED)
                {
                    AI_Stacks_Survey_Expedition_Forces_Stack(itr_stacks, stack_unit_count, stack_unit_count);
                }
            } break;
            case AISTK_DNE:
            {
                // DNE
            } break;
            case AISTK_Garrison:
            {
                if(stack_unit_count > 5)
                {
                    AI_Stacks_Survey_Expedition_Forces_Stack(itr_stacks, stack_unit_count, (stack_unit_count - 5));
                }
            } break;
            case AISTK_FortressGarrison:
            {
                /* ¿ hack to avoid over-garrisoning of Fortress City in the early-game ? */
                if((_turn < 100) && (stack_unit_count > 5))
                {
                    AI_Stacks_Survey_Expedition_Forces_Stack(itr_stacks, stack_unit_count, (stack_unit_count - 5));
                }
            } break;
            default:
            {
                /* others are skipped */
            } break;
        }
    }

}


// WZD o158p21
/**
 * @brief Score one AI stack's excess military units for expedition reassignment.
 *
 * Filters the specified stack down to military units that can be pulled out for
 * offensive use, skipping engineers, settlers, melders, and transport hulls.
 * The surviving candidates are assigned per-unit values, sorted in descending
 * order, truncated to the requested excess count, and then merged into the two
 * global ranked expedition pools: the general pushout list and the seafaring
 * pushout list.
 *
 * Candidates added here are stored in the global `G_Pushout_*` and
 * `G_Seafaring_*` arrays along with their source stack slot indices so later AI
 * passes can reassign those units toward stage or war objectives.
 *
 * @param stack_idx Index of the AI-owned stack being surveyed.
 * @param unit_count Number of entries from `_ai_own_stack_unit_list[stack_idx]`
 *                   to inspect.
 * @param excess_count Maximum number of filtered military units from this stack
 *                     that may be contributed to the global expedition pools.
 *
 * @return This function does not return a value. It may update the global
 *         expedition candidate arrays, their counts, and their tracked lowest
 *         values.
 *
 * @note The preserved original-game behavior intentionally uses a bogus random
 *       value in place of a true strength score and truncates by the filtered
 *       list length after sorting, matching the current reverse-engineered
 *       implementation rather than an idealized ranking pass.
 */
void AI_Stacks_Survey_Expedition_Forces_Stack(int16_t stack_idx, int16_t unit_count, int16_t excess_count)
{
    int16_t military_unit_indices[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t military_unit_values[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };  /* holds Effective_Unit_Type_Strength() */
    int16_t military_unit_ul_indices[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t military_unit_count = 0;
    int16_t unit_value = 0;
    int16_t local_itr = 0;
    int16_t lowest_value_index = 0;
    int16_t is_seafaring = 0;
    int16_t unit_type = 0;
    int16_t stack_ul_idx = 0;
    int16_t unit_idx = 0;
    int16_t itr2 = 0;
    int16_t itr = 0;
    int16_t ogbug_value = 0;  // DNE in Dasm

    /* Phase 1: Filter units in the stack for candidates to push out */
    military_unit_count = 0;
    for(stack_ul_idx = 0; stack_ul_idx < unit_count; stack_ul_idx++)
    {

        unit_idx = _ai_own_stack_unit_list[stack_idx][stack_ul_idx];
        
        if(unit_idx == ST_UNDEFINED)
        {
            continue;
        }

        unit_type = _UNITS[unit_idx].type;

        /* Skip non-military units: Engineers, Settlers, Melders, Boats */
        if(_unit_type_table[unit_type].Construction != 0)             { continue; }
        if(_unit_type_table[unit_type].Abilities & UA_CREATEOUTPOST)  { continue; }
        if(_unit_type_table[unit_type].Abilities & UA_MELD)           { continue; }
        if(_unit_type_table[unit_type].Transport != 0)                { continue; }

        military_unit_ul_indices[military_unit_count] = stack_ul_idx;
        military_unit_indices[military_unit_count] = unit_idx;

        /* Calculate unit strength value. */
        // OGBUG  should pass unit_idx or use Effective_Unit_Type_Strength()
        /* military_unit_values[military_unit_count] = Effective_Unit_Strength(unit_type) / 10; */
        /* HACK  OGBUG would have result in bogus data, so we try to replicate that */
        ogbug_value = ((Random(256) << 8) | Random(256));
#ifdef STU_DEBUG
    LOG_TRACE(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: AI_Stacks_Survey_Expedition_Forces_Stack(): ogbug_value: %d", __FILE__, __LINE__, ogbug_value);
#endif
        military_unit_values[military_unit_count] = ogbug_value;

        military_unit_count++;
    }

    /* Phase 2: Bubble sort candidates by value descending */
    for (itr = 0; itr < (military_unit_count - 1); itr++)
    {
        for (itr2 = 0; itr2 < (military_unit_count - 1); itr2++)
        {
            if(military_unit_values[itr2] < military_unit_values[itr2 + 1])
            {
                Swap_Short(&military_unit_values[itr2], &military_unit_values[itr2 + 1]);
                Swap_Short(&military_unit_ul_indices[itr2], &military_unit_ul_indices[itr2 + 1]);
                Swap_Short(&military_unit_indices[itr2], &military_unit_indices[itr2 + 1]);
            }
        }
    }

    /* Phase 4 */
    /* Limit processing to the requested number of excess units */
    // OGBUG  This truncates by list order, not by score — so for a type-3/4 garrison, it sends the first N combat units in list order, not the N weakest.
    if(excess_count < military_unit_count)
    {
        military_unit_count = excess_count;
    }

    /* Phase 5: Update global Offensive lists with these candidates */
    for(local_itr = 0; local_itr < military_unit_count; local_itr++)
    {
        stack_ul_idx = military_unit_ul_indices[local_itr];
        unit_idx = military_unit_indices[local_itr];
        unit_value = military_unit_values[local_itr];
        if(
            (Unit_Has_AirTravel(unit_idx) == ST_TRUE)
            ||
            (Unit_Has_WaterTravel(unit_idx) == ST_TRUE)
        )
        {
            is_seafaring = ST_TRUE;
        }
        else
        {
            is_seafaring = ST_FALSE;
        }

        /* Update standard Pushout list (Global Offensive Stack) */
        if(cp_drafted_unit_count < MAX_STACK)
        {
            /* Add new unit to the global list */
            G_Pushout_UL_Indices[cp_drafted_unit_count] = stack_ul_idx;
            G_Pushout_CX_IDs[cp_drafted_unit_count] = stack_idx;
            G_Pushout_Values[cp_drafted_unit_count] = unit_value;
            G_Pushout_Unit_Indices[cp_drafted_unit_count] = unit_idx;
            cp_drafted_unit_count++;
            if(unit_value < G_Pushout_Lowest_Value)
            {
                G_Pushout_Lowest_Value = unit_value;
            }
        }
        else
        {
            /* Replace the weakest unit in the global list */
            if(unit_value > G_Pushout_Lowest_Value)
            {

                lowest_value_index = 0;
                for(itr = 0; itr < MAX_STACK; itr++)
                {
                    if(G_Pushout_Values[itr] == G_Pushout_Lowest_Value)
                    {
                        lowest_value_index = itr;
                    }
                }

                G_Pushout_UL_Indices[lowest_value_index] = stack_ul_idx;
                G_Pushout_CX_IDs[lowest_value_index] = stack_idx;
                G_Pushout_Values[lowest_value_index] = unit_value;
                G_Pushout_Unit_Indices[lowest_value_index] = unit_idx;

                /* Recalculate new lowest value in list */
                G_Pushout_Lowest_Value = 10000;
                for(itr = 0; itr < MAX_STACK; itr++)
                {
                    if(G_Pushout_Values[itr] < G_Pushout_Lowest_Value)
                    {
                        G_Pushout_Lowest_Value = G_Pushout_Values[itr];
                    }
                }
            }
        }

        /* Update Seafaring Pushout list */
        if(is_seafaring == ST_TRUE)
        {
            if(G_Seafaring_Count < MAX_STACK)
            {
                G_Seafaring_UL_Indices[G_Seafaring_Count] = stack_ul_idx;
                G_Seafaring_CX_IDs[G_Seafaring_Count] = stack_idx;
                G_Seafaring_Values[G_Seafaring_Count] = unit_value;
                G_Seafaring_Unit_Indices[G_Seafaring_Count] = unit_idx;
                G_Seafaring_Count++;
                if(unit_value < G_Seafaring_Lowest_Value)
                {
                    G_Seafaring_Lowest_Value = (Effective_Unit_Strength(unit_idx) / 10);
                }
            }
            else
            {
                if(unit_value > G_Seafaring_Lowest_Value)
                {
                    lowest_value_index = 0;
                    for(itr = 0; itr < MAX_STACK; itr++)
                    {
                        if(G_Seafaring_Values[itr] == G_Seafaring_Lowest_Value)
                        {
                            lowest_value_index = itr;
                        }
                    }
                    G_Seafaring_UL_Indices[lowest_value_index] = stack_ul_idx;
                    G_Seafaring_CX_IDs[lowest_value_index] = stack_idx;
                    G_Seafaring_Values[lowest_value_index] = unit_value;
                    G_Seafaring_Unit_Indices[lowest_value_index] = unit_idx;
                    G_Seafaring_Lowest_Value = 10000;
                    for(itr = 0; itr < MAX_STACK; itr++)
                    {
                        if(G_Seafaring_Values[itr] < G_Seafaring_Lowest_Value)
                        {
                            G_Seafaring_Lowest_Value = G_Seafaring_Values[itr];
                        }
                    }
                }
            }
        }
    }

    /* Phase 6: Final sort of Global Pushout list descending */
    for(itr = 0; (cp_drafted_unit_count - 1) > itr; itr++)
    {
        for(itr2 = 0; (cp_drafted_unit_count - 1) > itr2; itr2++)
        {
            if(G_Pushout_Values[itr2] < G_Pushout_Values[(itr2 + 1)])
            {
                Swap_Short(&G_Pushout_UL_Indices[itr2], &G_Pushout_UL_Indices[(itr2 + 1)]);
                Swap_Short(&G_Pushout_CX_IDs[itr2], &G_Pushout_CX_IDs[(itr2 + 1)]);
                Swap_Short(&G_Pushout_Values[itr2], &G_Pushout_Values[(itr2 + 1)]);
                Swap_Short(&G_Pushout_Unit_Indices[itr2], &G_Pushout_Unit_Indices[(itr2 + 1)]);
            }
        }
    }

    /* Phase 7: Final sort of Global Seafaring list descending */
    for(itr = 0; (G_Seafaring_Count - 1) > itr; itr++)
    {
        for(itr2 = 0; (G_Seafaring_Count - 1) > itr2; itr2++)
        {
            if(G_Seafaring_Values[itr2] < G_Seafaring_Values[(itr2 + 1)])
            {
                Swap_Short(&G_Seafaring_UL_Indices[itr2], &G_Seafaring_UL_Indices[(itr2 + 1)]);
                Swap_Short(&G_Seafaring_CX_IDs[itr2], &G_Seafaring_CX_IDs[(itr2 + 1)]);
                Swap_Short(&G_Seafaring_Values[itr2], &G_Seafaring_Values[(itr2 + 1)]);
                Swap_Short(&G_Seafaring_Unit_Indices[itr2], &G_Seafaring_Unit_Indices[(itr2 + 1)]);
            }
        }
    }

}


// WZD o158p22
/**
 * @brief Orders AI meld-capable units to claim nearby eligible magic nodes.
 *
 * Iterates every unit slot in the current `_ai_own_stack_*` working stacks,
 * filters for units with `UA_MELD`, and searches all nodes on the same plane
 * that are not currently owned by `player_idx`. Candidate nodes are further
 * constrained to squares flagged as `AI_TARGET_SITE` and to landmasses the AI
 * currently treats as own/leaveable, or nodes already garrisoned by the AI.
 *
 * For each eligible unit, the closest qualifying node (wrapped map distance)
 * is selected. If the unit is already on that node, the function issues
 * AI_Stacks_Order_Meld(); otherwise it assigns a movement/attack-goto order via
 * AI_Stacks_Order_Attack_Target_Or_Goto_Destination() toward that node.
 *
 * @param player_idx Index of the AI player issuing meld-related orders.
 *
 * @return This function does not return a value. It may update unit status and
 *         destination fields indirectly through AI_Stacks_Order_Meld() and
 *         AI_Stacks_Order_Attack_Target_Or_Goto_Destination().
 *
 * @note The current implementation preserves original behavior where the
 *       `UA_MELD` ability check occurs before the `ST_UNDEFINED` unit-index
 *       guard.
 */
void AI_Stacks_Do_Meld(int16_t player_idx)
{
    int16_t node_landmass_idx = 0;
    int16_t unit_wp = 0;
    int16_t node_is_garrisoned = 0;
    int16_t delta_distance = 0;
    int16_t node_wy = 0;
    int16_t node_wx = 0;
    int16_t unit_wy = 0;
    int16_t unit_wx = 0;
    int16_t itr_units = 0;
    int16_t unit_idx = 0;
    int16_t list_unit_count = 0;
    int16_t list_unit_idx = 0;
    int16_t min_delta_distance = 0;
    int16_t target_node_idx = 0;
    int16_t itr = 0;
    int16_t itr_nodes = 0;

    for(itr = 0; itr < _ai_own_stack_count; itr++)
    {

        list_unit_count = _ai_own_stack_unit_count[itr];

        for(list_unit_idx = 0; list_unit_idx < list_unit_count; list_unit_idx++)
        {

            unit_idx = _ai_own_stack_unit_list[itr][list_unit_idx];

            /* OGBUG  MUST check for ST_UNDEFINED first */
            if((_unit_type_table[_UNITS[unit_idx].type].Abilities & UA_MELD) == 0)
            {
                continue;
            }
            if(unit_idx == ST_UNDEFINED)
            {
                continue;
            }

            unit_wx = _UNITS[unit_idx].wx;
            unit_wy = _UNITS[unit_idx].wy;
            unit_wp = _UNITS[unit_idx].wp;

            target_node_idx = ST_UNDEFINED;

            min_delta_distance = 1000;

            for(itr_nodes = 0; itr_nodes < NUM_NODES; itr_nodes++)
            {

                if(
                    (_NODES[itr_nodes].wp == unit_wp)
                    &&
                    (_NODES[itr_nodes].owner_idx != player_idx)
                )
                {

                    node_wx = _NODES[itr_nodes].wx;
                    node_wy = _NODES[itr_nodes].wy;

                    if(g_ai_evaluation_map[unit_wp][((node_wy * WORLD_WIDTH) + node_wx)] == AI_TARGET_SITE)
                    {

                        node_landmass_idx = _landmasses[((unit_wp * WORLD_SIZE) + (node_wy * WORLD_WIDTH) + node_wx)];
                        node_is_garrisoned = ST_FALSE;

                        for(itr_units = 0; itr_units < _units; itr_units++)
                        {
                            if(
                                (_UNITS[itr_units].owner_idx == player_idx)
                                &&
                                (_UNITS[itr_units].wx == node_wx)
                                &&
                                (_UNITS[itr_units].wy == node_wy)
                                &&
                                (_UNITS[itr_units].wp == unit_wp)
                            )
                            {
                                node_is_garrisoned = ST_TRUE;
                            }
                        }

                        if(
                            (_ai_continents.plane[unit_wp].player[player_idx].type_array[node_landmass_idx] == lmt_Own)
                            ||
                            (_ai_continents.plane[unit_wp].player[player_idx].type_array[node_landmass_idx] >= lmt_Leaveable)  // {..., 5: lmt_Leaveable, 6: lmt_NoTargets}
                            ||
                            (node_is_garrisoned == ST_TRUE)
                        )
                        {
                            delta_distance = Delta_XY_With_Wrap(node_wx, node_wy, unit_wx, unit_wy, WORLD_WIDTH);
                            if(delta_distance < min_delta_distance)
                            {
                                min_delta_distance = delta_distance;
                                target_node_idx = itr_nodes;
                            }
                        }

                    }

                }

            }

            if(target_node_idx != ST_UNDEFINED)
            {
                if(min_delta_distance == 0)
                {
                    /* Unit is at the node; order it to meld */
                    AI_Stacks_Order_Meld(unit_idx, itr, list_unit_idx);
                }
                else
                {
                    /* Move toward the closest target node */
                    node_wx = _NODES[target_node_idx].wx;
                    node_wy = _NODES[target_node_idx].wy;
#ifdef STU_DEBUG
                    LOG_DEBUG(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: %s: -> AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx=%d, target_wx=%d, target_wy=%d, stack_idx=%d, list_unit_idx=%d)", __FILE__, __LINE__, __FUNCTION__, unit_idx, node_wx, node_wy, itr, list_unit_idx);
#endif
                    g_ai_set_target_caller = 14;
                    AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, node_wx, node_wy, itr, list_unit_idx);
                }
            }

        }

    }

}


// WZD o158p23
void AI_Stacks_Do_Settle(int16_t player_idx, int16_t landmass_idx)
{
    int16_t landmass_node_index = 0;
    int16_t unit_wp = 0;
    int16_t highest_value_wy = 0;
    int16_t highest_value_wx = 0;
    int16_t have_adamantium = 0;
    int16_t have_mithril = 0;
    int16_t map_square_value = 0;
    int16_t have_nightshade = 0;
    int16_t magic_units = 0;
    int16_t gold_units = 0;
    int16_t unit_cost_reduction = 0;
    int16_t gold_bonus = 0;
    int16_t production_bonus = 0;
    int16_t maximum_population = 0;
    int16_t wx = 0;
    int16_t wy = 0;
    int16_t highest_map_square_value = 0;
    int16_t have_empty_garrison = 0;
    int16_t Fast_Expand_Chance = 0;
    int16_t Unexplored = 0;
    int16_t have_shore = 0;
    int16_t distance = 0;
    int16_t unit_wy = 0;
    int16_t unit_wx = 0;
    int16_t list_unit_count = 0;
    int16_t itr_list_units = 0;
    int16_t tower_wy = 0;
    int16_t tower_wx = 0;
    int16_t wp = 0;
    int16_t itr_stacks = 0;
    int16_t unit_idx = 0;


    /* Phase 1 */
    /* Determine expansion chance based on Wizard Objective */
    Fast_Expand_Chance = 1;
    switch (_players[player_idx].Objective)
    {
        case OBJ_Expansionist:  { Fast_Expand_Chance = 4; } break;
        case OBJ_Militarist:    { Fast_Expand_Chance = 2; } break;
        case OBJ_Pragmatist:    { Fast_Expand_Chance = 2; } break;
        case OBJ_Perfectionist: { Fast_Expand_Chance = 1; } break;
        case OBJ_Theurgist:     { Fast_Expand_Chance = 3; } break;
    }


    /* Phase 2: Abort if we under defended */
    have_empty_garrison = ST_FALSE;
    for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
    {
        if(
            (_ai_own_stack_type[itr_stacks] == AISTK_Garrison)
            &&
            (_ai_own_stack_unit_count[itr_stacks] < 1)  /* OGBUG  stacks can't have zero or negatives */
        )
        {
            have_empty_garrison = ST_TRUE;
        }
    }
    if(have_empty_garrison == ST_TRUE)
    {
        return;
    }


    /* Phase 3 */
    for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
    {

        list_unit_count = _ai_own_stack_unit_count[itr_stacks];

        for(itr_list_units = 0; itr_list_units < list_unit_count; itr_list_units++)
        {

            unit_idx = _ai_own_stack_unit_list[itr_stacks][itr_list_units];

            if(unit_idx == ST_UNDEFINED)
            {
                continue;
            }
            if((_unit_type_table[_UNITS[unit_idx].type].Abilities & UA_CREATEOUTPOST) != 0)
            {
                continue;
            }

            unit_wx = _UNITS[unit_idx].wx;
            unit_wy = _UNITS[unit_idx].wy;
            unit_wp = _UNITS[unit_idx].wp;

            if(
                (
                    (Map_Square_Area_Has_Opponent(unit_wx, unit_wy, unit_wp, 3, player_idx) == ST_FALSE)
                    ||
                    (_turn < 100)
                    ||
                    (Random(5) == 1)
                )
                &&
                (
                    (Map_Square_Survey(unit_wx, unit_wy, unit_wp) == ST_FALSE)
                    &&
                    (Random(5) <= Fast_Expand_Chance)
                )
            )
            {
                AI_Order_Settle(unit_idx, itr_stacks, itr_list_units);
                continue;
            }

            if(AI_Find_Tower_To_Settle_Elsewhere(player_idx, unit_idx, &tower_wx, &tower_wy, unit_wp) == ST_TRUE)
            {

#ifdef STU_DEBUG
                LOG_DEBUG(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: %s: -> AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx=%d, target_wx=%d, target_wy=%d, stack_idx=%d, list_unit_idx=%d)", __FILE__, __LINE__, __FUNCTION__, unit_idx, tower_wx, tower_wy, itr_stacks, itr_list_units);
#endif

                g_ai_set_target_caller = 15;
                AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, tower_wx, tower_wy, itr_stacks, itr_list_units);

            }
            else
            {
                highest_map_square_value = ST_UNDEFINED;
                landmass_node_index = _ai_landmass_land_squares_heads[unit_wp][landmass_idx];
                while(landmass_node_index != ST_UNDEFINED)
                {
                    wx = _ai_landmass_land_squares_wx_array[unit_wp][landmass_node_index];
                    wy = _ai_landmass_land_squares_wy_array[unit_wp][landmass_node_index];
                    if(Map_Square_Survey(wx, wy, unit_wp) == 0)
                    {
                        if((g_ai_evaluation_map[unit_wp][((wy * WORLD_WIDTH) + wx)] & AI_TARGET_STRENGTH_MASK) == 0)
                        {
                            distance = Range(unit_wx, unit_wy, wx, wy);
                            if(distance != 0)
                            {
                                Compute_Base_Values_For_Map_Square(wx, wy, wp, &maximum_population, &production_bonus, &gold_bonus, &unit_cost_reduction, &gold_units, &magic_units, &have_nightshade, &have_mithril, &have_adamantium, &have_shore, &Unexplored);
                                map_square_value = 
                                    (
                                        (
                                            (
                                                (maximum_population * 10)
                                                +
                                                production_bonus
                                                +
                                                gold_bonus
                                                +
                                                unit_cost_reduction
                                            )
                                            +
                                            (gold_units * 3)
                                            +
                                            (magic_units * 5)
                                            +
                                            (have_nightshade * 10)
                                            +
                                            (have_mithril * 20)
                                            +
                                            (have_adamantium * 50)
                                            +
                                            (have_shore * 50)
                                        )
                                        /
                                        distance
                                    );

                                EMM_Map_CONTXXX__WIP();

                                if(map_square_value > highest_map_square_value)
                                {
                                    highest_map_square_value = map_square_value;
                                    highest_value_wx = wx;
                                    highest_value_wy = wy;
                                }
                            }
                        }
                    }
                    landmass_node_index = _ai_landmass_land_squares_lists[wp][landmass_node_index];
                }

                if(highest_map_square_value == ST_UNDEFINED)
                {

                    /* No good square on this landmass, send settler to colonize elsewhere */
                    AI_Stacks_Reorder_Settle_Elsewhere(unit_idx, unit_wx, unit_wy, unit_wp, player_idx, itr_stacks, itr_list_units);

                }
                else
                {
#ifdef STU_DEBUG
                    LOG_DEBUG(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: %s: -> AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx=%d, target_wx=%d, target_wy=%d, stack_idx=%d, list_unit_idx=%d)", __FILE__, __LINE__, __FUNCTION__, unit_idx, highest_value_wx, highest_value_wy, itr_stacks, itr_list_units);
#endif
                    g_ai_set_target_caller = 16;
                    AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, highest_value_wx, highest_value_wy, itr_stacks, itr_list_units);

                }

            }

        }

    }

}


// WZD o158p24
// drake178: AI_ProcessPurifiers()
/*
; processes any unassigned purifiers on the continent,
; either by giving a purify or a move order - provided
; that there are any harmful corrupted tiles to purify
*/
/*

*/
void AI_Do_Purify(int16_t landmass_idx, int16_t wp)
{
    int16_t unit_idx = 0;
    int16_t list_unit_idx = 0;
    int16_t list_unit_count = 0;
    int16_t landmass_node_wy = 0;
    int16_t landmass_node_wx = 0;
    int16_t Tile_Y_Offset = 0;
    int16_t Tile_X_Offset = 0;
    int16_t target_wy = 0;
    int16_t target_wx = 0;
    int16_t landmass_node_index = 0;
    int16_t InRange_Corruption = 0;
    int16_t unit_wy = 0;
    int16_t unit_wx = 0;

    int16_t itr_stacks;  // _DI_

// ; check if there are any corrupted tiles on the
// ; continent near own assets and not under enemy control
// ; 
// ; BUG: should really only check city catchments

    InRange_Corruption = ST_FALSE;

    landmass_node_index = _ai_landmass_land_squares_heads[wp][landmass_idx];

    // while(landmass_node_index = _ai_landmass_land_squares_lists[wp][landmass_node_index] != ST_UNDEFINED)
    while(landmass_node_index != ST_UNDEFINED)
    {

        if(InRange_Corruption != ST_FALSE)
        {

            break;

        }

        landmass_node_wx = _ai_landmass_land_squares_wx_array[wp][landmass_node_index];
        
        landmass_node_wy = _ai_landmass_land_squares_wy_array[wp][landmass_node_index];

        if(
            ((_map_square_flags[((wp * WORLD_SIZE) + (landmass_node_wy * WORLD_WIDTH) + landmass_node_wx)] & MSF_CORRUPTION) != 0)
            &&
            ((g_ai_evaluation_map[wp][((landmass_node_wy * WORLD_WIDTH) + landmass_node_wx)] & AI_TARGET_STRENGTH_MASK) == 0)
        )
        {

// ; check the 5 by 5 area centered around the square to see
// ; if there are any cities or nodes, and set
// ; InRange_Corruption along with the first found
// ; coordinates if there are
// ; 
// ; BUG: should really only go for city catchments

            for(Tile_Y_Offset = -2; Tile_Y_Offset < 3; Tile_Y_Offset++)
            {

                for(Tile_X_Offset = -2; Tile_X_Offset < 3; Tile_X_Offset++)
                {

                    if(g_ai_evaluation_map[wp][(((Tile_Y_Offset + landmass_node_wy) * WORLD_WIDTH) + (Tile_X_Offset + landmass_node_wx))] == AI_TARGET_SITE)
                    {

                        InRange_Corruption = ST_TRUE;

                        target_wx = landmass_node_wx;

                        target_wy = landmass_node_wy;

                    }


                }

            }

        }

        landmass_node_index = _ai_landmass_land_squares_lists[wp][landmass_node_index];

    }

    if(InRange_Corruption != ST_FALSE)
    {

        for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
        {

            if(_ai_own_stack_type[itr_stacks] == AISTK_Unknown)
            {

                list_unit_count = _ai_own_stack_unit_count[itr_stacks];

                for(list_unit_idx = 0; list_unit_idx < list_unit_count; list_unit_idx++)
                {

                    unit_idx = _ai_own_stack_unit_list[itr_stacks][list_unit_idx];

                    if(unit_idx == ST_UNDEFINED)
                    {

                        if(_unit_type_table[_UNITS[unit_idx].type].Abilities & UA_PURIFY)
                        {

                            unit_wx = _UNITS[unit_idx].wx;
                            
                            unit_wy = _UNITS[unit_idx].wy;

                            if((_map_square_flags[((wp * WORLD_SIZE) + (unit_wy * WORLD_WIDTH) + unit_wx)] & MSF_CORRUPTION) != 0)
                            {

                                AI_Order_Purify(unit_idx, itr_stacks, list_unit_idx);

                            }
                            else
                            {

#ifdef STU_DEBUG
                                LOG_DEBUG(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: %s: -> AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx=%d, target_wx=%d, target_wy=%d, stack_idx=%d, list_unit_idx=%d)", __FILE__, __LINE__, __FUNCTION__, unit_idx, target_wx, target_wy, itr_stacks, list_unit_idx);
#endif
                                g_ai_set_target_caller = 17;
                                AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, target_wx, target_wy, itr_stacks, list_unit_idx);

                            }

                        }

                    }

                }

            }

        }

    }

}


// WZD o158p25
// drake178: AI_ProcessRoadBuilds()
/*
*/
/*

*/
void AI_Do_RoadBuild(int16_t landmass_idx)
{
    int16_t UU_Local_2 = 0;
    int16_t UU_Local_1 = 0;
    int16_t unit_wp = 0;
    int16_t unit_idx = 0;
    int16_t itr_list_units = 0;
    int16_t list_unit_count = 0;
    int16_t RoadTo_City = 0;
    int16_t nearest_city_idx = 0;
    int16_t delta_distance = 0;
    int16_t city_landmass_idx = 0;
    int16_t min_delta_distance = 0;
    int16_t Landmass_Dup = 0;
    int16_t unit_wy = 0;
    int16_t unit_wx = 0;
    int16_t itr_stacks = 0;
    int16_t city_idx = 0;  // _DI_


    /*
        DEDU  could this be meaningful?
        ovr158:4C30 C7 46 E4 EA D4                                  mov     [bp+UU_Local_1], 0D4EAh
        ovr158:4C35 C7 46 E2 26 D5                                  mov     [bp+UU_Local_2], 0D526h
    */
    UU_Local_1 = 0xD4EA;
    UU_Local_2 = 0xD526;


    for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
    {

        list_unit_count = _ai_own_stack_unit_count[itr_stacks];

        for(itr_list_units = 0; itr_list_units < list_unit_count; itr_list_units++)
        {

            unit_idx = _ai_own_stack_unit_list[itr_stacks][itr_list_units];

            if(
                (unit_idx != ST_UNDEFINED)
                &&
                (_unit_type_table[_UNITS[unit_idx].type].Construction > 0)
            )
            {

                unit_wx = _UNITS[unit_idx].wx;
                
                unit_wy = _UNITS[unit_idx].wy;
                
                unit_wp = _UNITS[unit_idx].wp;
                
                Landmass_Dup = landmass_idx;

                nearest_city_idx = ST_UNDEFINED;

                min_delta_distance = 1000;

                for(city_idx = 0; city_idx < _cities; city_idx++)
                {

// ; BUG: parameter mismatch (and redundant branching, as
// ; the next comparison would be enough by itself)
                    if(
                        (_CITIES[city_idx].wp == unit_wp)
                        &&
                        (
                            ((_ai_all_own_stacks[itr_stacks].abilities & AICAP_LandOnly) != 0)
                            ||
                            ((unit_wp * WORLD_SIZE) + (_CITIES[city_idx].wy * WORLD_WIDTH) + _CITIES[city_idx].wx)  == Landmass_Dup
                        )
                    )
                    {

                        delta_distance = Delta_XY_With_Wrap(unit_wx, unit_wy, _CITIES[city_idx].wx, _CITIES[city_idx].wy, WORLD_WIDTH);

                        if(delta_distance < min_delta_distance)
                        {
                            
                            min_delta_distance = delta_distance;

                            nearest_city_idx = city_idx;

                        }

                    }

                }

                if(nearest_city_idx != ST_UNDEFINED)
                {

                    if(min_delta_distance > 1)
                    {

#ifdef STU_DEBUG
                        LOG_DEBUG(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: %s: -> AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx=%d, target_wx=%d, target_wy=%d, stack_idx=%d, list_unit_idx=%d)", __FILE__, __LINE__, __FUNCTION__, unit_idx, _CITIES[nearest_city_idx].wx, _CITIES[nearest_city_idx].wy, itr_stacks, itr_list_units);
#endif
                        g_ai_set_target_caller = 18;
                        AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, _CITIES[nearest_city_idx].wx, _CITIES[nearest_city_idx].wy, itr_stacks, itr_list_units);

                    }
                    else
                    {

                        RoadTo_City = ST_UNDEFINED;

                        min_delta_distance = 1000;

                        city_landmass_idx = _landmasses[((unit_wp * WORLD_SIZE) + (_CITIES[nearest_city_idx].wy * WORLD_WIDTH) + _CITIES[nearest_city_idx].wx)];

                        for(city_idx = 0; city_idx < _cities; city_idx++)
                        {

                            if(city_idx == nearest_city_idx)
                            {

                                if(
                                    (Test_Bit_Field(city_idx, (uint8_t *)&_CITIES[nearest_city_idx].road_connections[0]) == 0)
                                    &&
                                    (_landmasses[((unit_wp * WORLD_SIZE) + (_CITIES[city_idx].wy * WORLD_WIDTH) + _CITIES[city_idx].wx)] == city_landmass_idx)
                                    )
                                {

                                    delta_distance = Delta_XY_With_Wrap(_CITIES[nearest_city_idx].wx, _CITIES[nearest_city_idx].wy, _CITIES[city_idx].wx, _CITIES[city_idx].wy, WORLD_WIDTH);

                                    if(delta_distance < min_delta_distance)
                                    {
                                        
                                        min_delta_distance = delta_distance;

                                        RoadTo_City = city_idx;

                                    }


                                }

                            }

                        }

                        if(RoadTo_City != ST_UNDEFINED)
                        {

                            AI_Order_RoadBuild(unit_idx, _CITIES[RoadTo_City].wx, _CITIES[RoadTo_City].wy, itr_stacks, itr_list_units);

                        }

                    }

                }

            }

        }

    }

}


// WZD o158p26
/**
 * @brief Assign a unit either an attack-move or plain goto order toward a target square.
 *
 * Validates the supplied unit index, looks up the AI evaluation value for the
 * destination square on the unit's current plane, and chooses between
 * `us_Move` and `us_GOTO` based on whether the target square currently holds a
 * hostile stack or a site marker. The destination coordinates are then written
 * into the unit record and the unit is removed from the originating
 * `_ai_own_stack_unit_list` slot so later passes do not treat it as still
 * unassigned within that AI stack.
 *
 * @param unit_idx Global unit index receiving the order.
 * @param target_wx World x-coordinate of the destination square.
 * @param target_wy World y-coordinate of the destination square.
 * @param stack_idx Index of the AI-owned stack entry that currently contains
 *                  the unit.
 * @param list_unit_idx Slot of the unit inside
 *                      `_ai_own_stack_unit_list[stack_idx]`.
 *
 * @return This function does not return a value. It may update the unit's
 *         status and destination fields and clears the corresponding entry in
 *         `_ai_own_stack_unit_list[stack_idx][list_unit_idx]`.
 *
 * @note If @p unit_idx is outside the accepted unit range, the function exits
 *       immediately without modifying unit or stack state.
 */
void AI_Stacks_Order_Attack_Target_Or_Goto_Destination(int16_t unit_idx, int16_t target_wx, int16_t target_wy, int16_t stack_idx, int16_t list_unit_idx)
{
    int16_t wp = 0;
    int16_t target_value = 0;
#ifdef STU_DEBUG
    LOG_INFO(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: BEGIN: AI_Stacks_Order_Attack_Target_Or_Goto_Destination()", __FILE__, __LINE__);
    LOG_DEBUG(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: BEGIN: AI_Stacks_Order_Attack_Target_Or_Goto_Destination()", __FILE__, __LINE__);
    LOG_TRACE(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: BEGIN: AI_Stacks_Order_Attack_Target_Or_Goto_Destination()", __FILE__, __LINE__);
#endif
    if((unit_idx < 0) || (unit_idx >= MAX_UNIT_COUNT)) { return; }
    wp = _UNITS[unit_idx].wp;
    target_value = g_ai_evaluation_map[wp][((target_wy * WORLD_WIDTH) + target_wx)];
#ifdef STU_DEBUG
    LOG_DEBUG(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: unit_idx: %d, target_wx: %d, target_wy: %d, target_value: %d", __FILE__, __LINE__, unit_idx, target_wx, target_wy, target_value);
#endif
    if(
        ((target_value & AI_TARGET_SITE) != 0)
        ||
        ((target_value & AI_TARGET_STRENGTH_MASK) != 0)
    )
    {
        _UNITS[unit_idx].Status = us_Move;  /* Move to Attack */
    }
    else
    {
        _UNITS[unit_idx].Status = us_GOTO;  /* Just Move */
    }
    _UNITS[unit_idx].dst_wx = (int8_t)target_wx;
    _UNITS[unit_idx].dst_wy = (int8_t)target_wy;

#ifdef STU_DEBUG
        LOG_DEBUG(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: unit_idx: %d, status: %d, dst_wx: %d, dst_wy: %d", __FILE__, __LINE__, unit_idx, _UNITS[unit_idx].Status, _UNITS[unit_idx].dst_wx, _UNITS[unit_idx].dst_wy);
#endif

    _ai_own_stack_unit_list[stack_idx][list_unit_idx] = ST_UNDEFINED;

#ifdef STU_DEBUG
    if (target_wx == 0 && target_wy == 0)
    {
        LOG_DEBUG(LOG_CAT_AIMOVE, "AI_ORDERS: WARNING unit %d (owner %d type %d) at (%d,%d) assigned dst (0,0) — status=%d stack_idx=%d list_unit=%d wp=%d caller=%d", unit_idx, _UNITS[unit_idx].owner_idx, _UNITS[unit_idx].type, _UNITS[unit_idx].wx, _UNITS[unit_idx].wy, _UNITS[unit_idx].Status, stack_idx, list_unit_idx, _UNITS[unit_idx].wp, g_ai_set_target_caller);
    }
#endif

#ifdef STU_DEBUG
    LOG_INFO(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: END: AI_Stacks_Order_Attack_Target_Or_Goto_Destination()", __FILE__, __LINE__);
    LOG_DEBUG(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: END: AI_Stacks_Order_Attack_Target_Or_Goto_Destination()", __FILE__, __LINE__);
    LOG_TRACE(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: END: AI_Stacks_Order_Attack_Target_Or_Goto_Destination()", __FILE__, __LINE__);
#endif

}


// WZD o158p27
void AI_Order_Settle(int16_t unit_idx, int16_t unit_list_idx, int16_t list_unit_idx)
{
    if((unit_idx < 0) || (unit_idx >= MAX_UNIT_COUNT)) { return; }
    _UNITS[unit_idx].Status = us_Settle;
    _ai_own_stack_unit_list[unit_list_idx][list_unit_idx] = ST_UNDEFINED;
}


// WZD o158p28
void AI_Order_RoadBuild(int16_t unit_idx, int16_t wx, int16_t wy, int16_t unit_list_idx, int16_t list_unit_idx)
{
    if((unit_idx < 0) || (unit_idx >= MAX_UNIT_COUNT)) { return; }
    _UNITS[unit_idx].Rd_From_X = _UNITS[unit_idx].wx;
    _UNITS[unit_idx].Rd_From_Y = _UNITS[unit_idx].wy;
    _UNITS[unit_idx].Status = us_BuildRoad;
    _UNITS[unit_idx].dst_wx = (int8_t)wx;
    _UNITS[unit_idx].dst_wy = (int8_t)wy;
    _ai_own_stack_unit_list[unit_list_idx][list_unit_idx] = ST_UNDEFINED;
}


// WZD o158p29
/**
 * @brief Marks one AI-controlled unit as waiting for ferry transport.
 *
 * Validates the supplied unit index, then switches the unit's status to
 * @c us_Ferry so later AI transport handling can pick it up. The routine also
 * stores the original game's ferry wait counter value in @c dst_wx and removes
 * the unit from the current `_ai_own_stack_unit_list` entry so the stack logic
 * no longer treats it as unassigned this pass.
 *
 * @param unit_idx Global unit index to place into ferry-wait status.
 * @param unit_list_idx Index of the owning AI stack within
 *                      `_ai_own_stack_unit_list`.
 * @param list_unit_idx Slot of the unit inside that AI stack list.
 *
 * @return This function does not return a value. It may update
 *         @c _UNITS[unit_idx].Status, @c _UNITS[unit_idx].dst_wx, and
 *         @c _ai_own_stack_unit_list[unit_list_idx][list_unit_idx].
 *
 * @note If @p unit_idx falls outside the valid unit array range, the function
 *       exits without modifying unit or stack state.
 */
void AI_Stacks_Order_Ferry(int16_t unit_idx, int16_t unit_list_idx, int16_t list_unit_idx)
{
    if((unit_idx < 0) || (unit_idx >= MAX_UNIT_COUNT)) { return; }
    _UNITS[unit_idx].Status = us_Ferry;
    _UNITS[unit_idx].dst_wx = 10;  /* DEDU  drake178: "sets ... a timer of 10" */
    _ai_own_stack_unit_list[unit_list_idx][list_unit_idx] = ST_UNDEFINED;
}


// WZD o158p30
/**
 * @brief Marks one AI-controlled unit to perform a meld action.
 *
 * Validates the supplied unit index, then switches the unit status to
 * @c us_Meld so the unit executes melding logic on its next AI processing
 * pass. The function also removes the unit from the current
 * `_ai_own_stack_unit_list` slot so subsequent stack-ordering steps do not
 * treat it as still unassigned.
 *
 * @param unit_idx Global unit index to assign the meld order to.
 * @param unit_list_idx Index of the owning AI stack within
 *                      `_ai_own_stack_unit_list`.
 * @param list_unit_idx Slot of the unit inside that AI stack list.
 *
 * @return This function does not return a value. It may update
 *         @c _UNITS[unit_idx].Status and
 *         @c _ai_own_stack_unit_list[unit_list_idx][list_unit_idx].
 *
 * @note If @p unit_idx falls outside the valid unit array range, the function
 *       exits without modifying unit or stack state.
 */
void AI_Stacks_Order_Meld(int16_t unit_idx, int16_t unit_list_idx, int16_t list_unit_idx)
{
    if((unit_idx < 0) || (unit_idx >= MAX_UNIT_COUNT)) { return; }
    _UNITS[unit_idx].Status = us_Meld;
    _ai_own_stack_unit_list[unit_list_idx][list_unit_idx] = ST_UNDEFINED;
}


// WZD o158p31
void AI_Order_Purify(int16_t unit_idx, int16_t unit_list_idx, int16_t list_unit_idx)
{
    if((unit_idx < 0) || (unit_idx >= MAX_UNIT_COUNT)) { return; }
    _UNITS[unit_idx].Status = us_Purify;
    _UNITS[unit_idx].dst_wx = 20;  /* DEDU  timer, like SeekTransport? */
    _ai_own_stack_unit_list[unit_list_idx][list_unit_idx] = ST_UNDEFINED;
}


// WZD o158p32
/*
OON XREF: AI_Stacks_Do_Settle()
~== AI_Enemy_Unit_In_Range(), but Landmass agnostic
*/
int16_t Map_Square_Area_Has_Opponent(int16_t wx, int16_t wy, int16_t wp, int16_t range, int16_t player_idx)
{
    int16_t max_wy = 0;
    int16_t min_wy = 0;
    int16_t max_wx = 0;
    int16_t min_wx = 0;
    int16_t itr_units = 0;
    min_wx = (wx - range);
    min_wy = (wy - range);
    max_wx = (wx + range);
    max_wy = (wy + range);
    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(
            (_UNITS[itr_units].wx < min_wx)
            &&
            (_UNITS[itr_units].wy < min_wy)
            &&
            (_UNITS[itr_units].wx > max_wx)
            &&
            (_UNITS[itr_units].wy > max_wy)
        )
        {
            continue;
        }
        if(_UNITS[itr_units].wp != wp)
        {
            continue;
        }
        if(_UNITS[itr_units].owner_idx != player_idx)
        {
            continue;
        }
        return ST_TRUE;
    }
    return ST_FALSE;
}


// WZD o158p33
/*
OON XREF: AI_Stacks_Ocean_Landmass_Orders()
~== Map_Square_Area_Has_Opponent(), but Landmass aware
*/
int16_t AI_Enemy_Unit_In_Range(int16_t wx, int16_t wy, int16_t wp, int16_t range, int16_t player_idx, int16_t landmass_idx)
{
    int16_t max_wy = 0;
    int16_t min_wy = 0;
    int16_t max_wx = 0;
    int16_t min_wx = 0;
    int16_t itr_units = 0;
    min_wx = (wx - range);
    min_wy = (wy - range);
    max_wx = (wx + range);
    max_wy = (wy + range);
    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(
            (_UNITS[itr_units].wx < min_wx)
            &&
            (_UNITS[itr_units].wy < min_wy)
            &&
            (_UNITS[itr_units].wx > max_wx)
            &&
            (_UNITS[itr_units].wy > max_wy)
        )
        {
            continue;
        }
        if(_UNITS[itr_units].wp != wp)
        {
            continue;
        }
        if(_UNITS[itr_units].owner_idx != player_idx)
        {
            continue;
        }
        if(_landmasses[((wp * WORLD_SIZE) + (_UNITS[itr_units].wy * WORLD_WIDTH) + _UNITS[itr_units].wx)] != landmass_idx)
        {
            continue;
        }
        return ST_TRUE;
    }
    return ST_FALSE;
}


// WZD o158p34
/* COPILOT */
/**
 * @brief Finds a friendly tower route for sending a settler to the opposite plane.
 *
 * This helper evaluates whether a settler-like unit should be redirected through
 * a tower to settle on the other plane. It first aborts when any wizard has
 * Planar Seal active, then rejects calls already on the destination plane. For
 * eligible units, it selects the nearest tower owned by @p player_idx on the
 * unit's current landmass and evaluates the opposite-plane landmass reached at
 * that tower coordinate.
 *
 * The destination is accepted only when the target landmass classification is
 * neither contested nor ally-occupied-without-own-city.
 *
 * @param player_idx AI player requesting the off-plane settlement redirect.
 * @param unit_idx Index of the unit being evaluated for tower transfer.
 * @param new_target_wx Output pointer receiving the selected tower x coordinate.
 * @param new_target_wy Output pointer receiving the selected tower y coordinate.
 * @param wp Caller-provided current plane context used for early rejection.
 *
 * @return ST_TRUE when a valid tower destination is found and written to output
 *         pointers; otherwise ST_FALSE.
 *
 * @note The routine assumes @p unit_idx is valid for _UNITS[] and does not
 *       perform its own bounds check.
 */
int16_t AI_Find_Tower_To_Settle_Elsewhere(int16_t player_idx, int16_t unit_idx, int16_t * new_target_wx, int16_t * new_target_wy, int16_t wp)
{
    int16_t landmass_idx = 0;
    int16_t uu_var_1a = 0;
    int16_t delta_distance = 0;
    int16_t unit_landmass_idx = 0;
    int16_t unit_is_on_tower = 0;
    int16_t landmass_wp = 0;
    int16_t min_delta_distance = 0;
    int16_t tower_wy = 0;
    int16_t tower_wx = 0;
    int16_t landmass_wy = 0;
    int16_t landmass_wx = 0;
    int16_t unit_wp = 0;
    int16_t unit_wy = 0;
    int16_t unit_wx = 0;
    int16_t itr_players = 0;
    int16_t itr_towers = 0;

    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        if(_players[itr_players].Globals[PLANAR_SEAL] != 0)
        {
            return ST_FALSE;
        }
    }

    landmass_wp = (1 - _FORTRESSES[player_idx].wp);
    
    if(wp == landmass_wp)
    {
        return ST_FALSE;
    }

    unit_wx = _UNITS[unit_idx].wx;
    unit_wy = _UNITS[unit_idx].wy;
    unit_wp = _UNITS[unit_idx].wp;
    unit_is_on_tower = 0; /* e_ST_FALSE */

    for(itr_towers = 0; itr_towers < NUM_TOWERS; itr_towers++)
    {
        if(_TOWERS[itr_towers].wx == unit_wx && _TOWERS[itr_towers].wy == unit_wy)
        {
            unit_is_on_tower = 1;
        }
    }

    if (unit_is_on_tower == 1) {
        return 0;
    }

    unit_landmass_idx = _landmasses[(unit_wp * WORLD_SIZE) + (unit_wy * WORLD_WIDTH) + unit_wx];
    min_delta_distance = 1000;

    for (itr_towers = 0; itr_towers < NUM_TOWERS; itr_towers++) {
        if (_TOWERS[itr_towers].owner_idx == player_idx) {
            if (_landmasses[(unit_wp * WORLD_SIZE) + (_TOWERS[itr_towers].wy * WORLD_WIDTH) + _TOWERS[itr_towers].wx] == unit_landmass_idx) {
                delta_distance = Delta_XY_With_Wrap(unit_wx, unit_wy, _TOWERS[itr_towers].wx, _TOWERS[itr_towers].wy, WORLD_WIDTH);
                if (delta_distance < min_delta_distance) {
                    min_delta_distance = delta_distance;
                    tower_wx = _TOWERS[itr_towers].wx;
                    tower_wy = _TOWERS[itr_towers].wy;
                }
            }
        }
    }

    if (min_delta_distance == 1000) {
        return 0;
    }

    landmass_wx = tower_wx;
    landmass_wy = tower_wy;
    landmass_idx = _landmasses[(landmass_wp * WORLD_SIZE) + (landmass_wy * WORLD_WIDTH) + landmass_wx];
    uu_var_1a = 0;

    if(_ai_continents.plane[landmass_wp].player[player_idx].type_array[landmass_idx] == lmt_Contested)
    {
        return ST_FALSE;
    }

    if(_ai_continents.plane[landmass_wp].player[player_idx].type_array[landmass_idx] == lmt_NoOwnCityAndAllyHasCity)
    {
        return ST_FALSE;
    }

    *new_target_wx = landmass_wx;
    *new_target_wy = landmass_wy;

    return ST_TRUE;

}


// WZD o158p35
void AI_Stacks_Reorder_Settle_Elsewhere(int16_t unit_idx, int16_t wx, int16_t wy, int16_t wp, int16_t player_idx, int16_t unit_list_idx, int16_t list_unit_idx)
{
    int16_t target_wy = 0;
    int16_t target_wx = 0;
    int16_t is_seafaring = 0;
    int16_t niu_stack_capacity_free = 0;
    int16_t niu_stack_capacity_total = 0;
    int16_t new_target_wy = 0;
    int16_t new_target_wx = 0;
    int16_t uu_landmass_idx = 0;
    int16_t destination_wy = 0;
    int16_t destination_wx = 0;
    int16_t itr_wx = 0;
    int16_t itr_stacks = 0;
    int16_t found_transport = 0;
    int16_t square_is_embarkable = 0;
    int16_t unit_wy = 0;
    int16_t unit_wx = 0;
    

    /* Phase 1: Init */
    uu_landmass_idx = _landmasses[((WORLD_SIZE * wp) + (wy * WORLD_WIDTH) + wx)];
    unit_wx = _UNITS[unit_idx].wx;
    unit_wy = _UNITS[unit_idx].wx;  /* OGBUG  should be _UNITS[].wy, not _UNITS[].wx */
    wp = _UNITS[unit_idx].wp;  /* OGBUG  overwrites WP */


    /* Phase 2: Determine if the unit is seafaring */
    is_seafaring = ST_FALSE;
    if(
        (Unit_Has_AirTravel(unit_idx) != ST_FALSE)
        ||
        (Unit_Has_WaterTravel(unit_idx) != ST_FALSE)
    )
    {
        is_seafaring = ST_TRUE;
    }


    /* Phase 3: Sanity Check */
    if(_ai_landmass_settler_targets[wp][player_idx] == 0)
    {
        return;
    }


    /* Phase 4 */
    destination_wx = _ai_landmass_settler_targets_wx_array[wp][player_idx];
    destination_wy = _ai_landmass_settler_targets_wy_array[wp][player_idx];
    if(is_seafaring == ST_TRUE)
    {
        /* Phase 4a: send it directly */
#ifdef STU_DEBUG
        LOG_DEBUG(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: %s: -> AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx=%d, target_wx=%d, target_wy=%d, stack_idx=%d, list_unit_idx=%d)", __FILE__, __LINE__, __FUNCTION__, unit_idx, destination_wx, destination_wy, unit_list_idx, list_unit_idx);
#endif
        g_ai_set_target_caller = 19;
        AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, destination_wx, destination_wy, unit_list_idx, list_unit_idx);
    }
    else
    {
        /* Phase 4b: Ferry */
        square_is_embarkable = ST_FALSE;
        for(unit_wy = (wy - 1); (wy + 2) > unit_wy; unit_wy++)
        {
            if(
                (unit_wy <= 0)
                ||
                (unit_wy >= WORLD_HEIGHT)
            )
            {
                continue;
            }
            for(itr_wx = (wx - 1); (wx + 2) > itr_wx; itr_wx++)
            {
                unit_wx = itr_wx;
                if(unit_wx < 0)
                {
                    unit_wx += WORLD_WIDTH;
                }
                if(unit_wx >= WORLD_WIDTH)
                {
                    unit_wx -= WORLD_WIDTH;
                }
                if(Map_Square_Is_Embarkable(unit_wx, unit_wy, wp) != ST_FALSE)
                {
                    square_is_embarkable = ST_TRUE;
                    new_target_wx = itr_wx;  /* OGBUG  this should be unit_wx, not itr_wx */
                    new_target_wy = unit_wy;
                }
            }
        }

        /* Phase 4c: */
        if(square_is_embarkable == ST_FALSE)
        {
            /* Phase 4c1: */
            /* Find a suitable landing site or transport load point */
            Next_Nearest_Ferry_Square(wx, wy, wp, &new_target_wx, &new_target_wy);
            if(Adjacent_Land_Square(new_target_wx, new_target_wy, wp, &new_target_wx, &new_target_wy) == ST_TRUE)
            {
#ifdef STU_DEBUG
                LOG_DEBUG(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: %s: -> AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx=%d, target_wx=%d, target_wy=%d, stack_idx=%d, list_unit_idx=%d)", __FILE__, __LINE__, __FUNCTION__, unit_idx, new_target_wx, new_target_wy, unit_list_idx, list_unit_idx);
#endif
                g_ai_set_target_caller = 20;
                AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, new_target_wx, new_target_wy, unit_list_idx, list_unit_idx);
            }
        }
        else
        {
            /* Phase 4c2: */
            /* Unit is near water: check if a transport is already nearby */
            found_transport = ST_FALSE;
            for(itr_stacks = 0; ((itr_stacks < _ai_all_own_stack_count) && (found_transport == ST_FALSE)); itr_stacks++)
            {
                if(
                    (_ai_all_own_stacks[itr_stacks].wp == wp)
                    &&
                    ((_ai_all_own_stacks[itr_stacks].abilities & AICAP_Transport) != 0)
                )
                {
                    unit_wx = _ai_all_own_stacks[itr_stacks].wx;
                    unit_wy = _ai_all_own_stacks[itr_stacks].wy;
                    if(
                        (abs(wx - unit_wx) < 2)
                        &&
                        (abs(wy - unit_wy) < 2)
                    )
                    {
                        found_transport = ST_TRUE;
                        destination_wx = unit_wx;
                        destination_wy = unit_wy;
                        niu_stack_capacity_total = _ai_all_own_stacks[itr_stacks].transport_capacity;
                        niu_stack_capacity_free = (MAX_STACK - _ai_all_own_stacks[itr_stacks].unit_count);
                    }
                }
            }
            if(found_transport == ST_TRUE)
            {
#ifdef STU_DEBUG
                LOG_DEBUG(LOG_CAT_AIMOVE, "DEBUG: [%s, %d]: %s: -> AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx=%d, target_wx=%d, target_wy=%d, stack_idx=%d, list_unit_idx=%d)", __FILE__, __LINE__, __FUNCTION__, unit_idx, destination_wx, destination_wy, unit_list_idx, list_unit_idx);
#endif
                g_ai_set_target_caller = 21;
                AI_Stacks_Order_Attack_Target_Or_Goto_Destination(unit_idx, destination_wx, destination_wy, unit_list_idx, list_unit_idx);
            }
            else
            {
                if(_ai_ferry_count < MAX_AI_FERRIES)
                {
                    AI_Stacks_Order_Ferry(unit_idx, unit_list_idx, list_unit_idx);
                    _ai_ferry_wx_array[_ai_ferry_count] = new_target_wx;
                    _ai_ferry_wy_array[_ai_ferry_count] = new_target_wy;
                    _ai_ferry_wp_array[_ai_ferry_count] = wp;
                    _ai_ferry_count++;
                }
            }
        }
    }
}


// WZD o158p36
/**
 * @brief Registers a ferry-request location if it is not already queued.
 *
 * Scans the current AI ferry request arrays for an existing entry that matches
 * the supplied world coordinates and plane. If a match is already present, the
 * function leaves the queue unchanged. Otherwise, it appends a new request at
 * the end of the arrays, provided the queue has not reached
 * @c MAX_AI_FERRIES entries.
 *
 * @param wx World x coordinate of the requested ferry pickup square.
 * @param wy World y coordinate of the requested ferry pickup square.
 * @param wp World plane containing the requested ferry pickup square.
 *
 * @return This function does not return a value. It may update
 *         @c _ai_ferry_wx_array, @c _ai_ferry_wy_array,
 *         @c _ai_ferry_wp_array, and @c _ai_ferry_count.
 */
void AI_Stacks_Ferry_Add_Location(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t itr = 0;
    /* Search for existing entry */
    for(itr = 0; itr < _ai_ferry_count; itr++)
    {
        if(
            (_ai_ferry_wx_array[itr] == wx)
            &&
            (_ai_ferry_wy_array[itr] == wy)
            &&
            (_ai_ferry_wp_array[itr] == wp)
        )
        {
            /* Found: do nothing and return. */
            return;
        }
    }
    if(_ai_ferry_count >= MAX_AI_FERRIES)
    {
        return;
    }
    /* If not found and the list has capacity (max 15), add new entry */
    _ai_ferry_wx_array[_ai_ferry_count] = wx;
    _ai_ferry_wy_array[_ai_ferry_count] = wy;
    _ai_ferry_wp_array[_ai_ferry_count] = wp;
    _ai_ferry_count++;
}


// WZD o158p37
/*

*/
int16_t AI_Tower_Target_Worthwhile(int16_t player_idx, int16_t wx, int16_t wy, int16_t wp)
{
    int16_t landmass_node_index = 0;
    int16_t city_owner_idx = 0;
    int16_t Opposite_Plane = 0;
    int16_t Opposite_Landmass = 0;
    int16_t Have_City_Target = 0;
    int16_t itr_cities = 0;
    Have_City_Target = ST_FALSE;
    Opposite_Plane = (1 - wp);
    Opposite_Landmass = _landmasses[((Opposite_Plane * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)];
    if(
        (_ai_continents.plane[Opposite_Plane].player[player_idx].type_array[Opposite_Landmass] != lmt_Own)
        &&
        (_ai_continents.plane[Opposite_Plane].player[player_idx].type_array[Opposite_Landmass] != lmt_NoOwnCityAndAllyHasCity)
    )
    {
        for(itr_cities = 0; ((itr_cities < _cities) && (Have_City_Target == ST_FALSE)); itr_cities++)
        {
            if(_landmasses[((_CITIES[itr_cities].wp * WORLD_SIZE) + (_CITIES[itr_cities].wy * WORLD_WIDTH) + _CITIES[itr_cities].wx)] == Opposite_Landmass)
            {
                city_owner_idx = _CITIES[itr_cities].owner_idx;
                if(
                    (city_owner_idx == NEUTRAL_PLAYER_IDX)
                    ||
                    (_players[player_idx].Hostility[city_owner_idx] >= 2)
                )
                {
                    Have_City_Target = ST_TRUE;
                }
            }
        }
    }
    /*
        Landmass has no 'Own City'.
        So, ...
    */
    if(_ai_continents.plane[Opposite_Plane].player[player_idx].type_array[Opposite_Landmass] != lmt_NoOwnCity)
    {
        itr_cities = 0;
        landmass_node_index = _ai_landmass_land_squares_heads[Opposite_Plane][Opposite_Landmass];
        while(landmass_node_index != ST_UNDEFINED)
        {
            if(Have_City_Target != ST_FALSE)
            {
                break;
            }
            itr_cities++;
            if(itr_cities > 10)
            {
                Have_City_Target = ST_TRUE;
            }
            landmass_node_index = _ai_landmass_land_squares_lists[Opposite_Plane][landmass_node_index];
        }
    }
    return Have_City_Target;
}


// WZD o158p38
/**
 * @brief Re-evaluates one player's strategic state for a single landmass on one plane.
 *
 * This routine rebuilds the continent record used by the AI stage and main-war
 * logic for the selected player, landmass, and plane. It temporarily maps the
 * CONTXXX work area, clears non-engineer orders for the player's units on the
 * selected landmass, recomputes the player's city centroid, classifies the
 * landmass state, and then chooses an updated stage square for mustering or
 * transport loading.
 *
 * The classification pass distinguishes between landmasses with own cities,
 * contested landmasses, and landmasses that only contain allied cities. When a
 * valid stage point can be found, the routine stores it in the per-player
 * continent arrays; when none can be found for an otherwise relevant landmass,
 * it downgrades the type to @c lmt_NoOwnCityAndAllyHasCity. For landmasses the
 * AI considers fully owned, it also retargets the stage square toward the dock
 * square nearest the current war-target landmass so troops can be pulled toward
 * transport loading points.
 *
 * @param player_idx Index of the AI-controlled player whose continent data is
 *                   being rebuilt.
 * @param landmass_idx Index of the landmass to re-evaluate on the selected plane.
 * @param wp World plane containing the landmass and all rewritten continent data.
 *
 * @return This function does not return a value. It updates
 *         @c _ai_continents.plane[wp].player[player_idx], clears matching unit
 *         orders by setting unit status to @c us_Ready, and consults the
 *         landmass-linked and dock-linked list work arrays.
 *
 * @note Several original-game bugs and reverse-engineering concerns are
 *       preserved in place, including mixed reuse of city-coordinate accumulators
 *       as force totals, landmass tests that do not consistently gate on plane,
 *       and the missing @c EMMDATAH_Map() restore noted at the end of the function.
 */
/*
OON XREF: AI_Set_Unit_Orders() |-> AI_Stacks_Stage_Expedition_Forces() |-> AI_Reevaluate_Continent()
Per Player, Per Landmass+Plane
(no plane and landmass loops, so can not use continue style logic, like AI_Evaluate_Continents())
*/
void AI_Reevaluate_Continent(int16_t player_idx, int16_t landmass_idx, int16_t wp)
{
    int16_t delta_distance = 0;
    int16_t unit_type = 0;
    int16_t sum_own_city_wy__enemy_units_cost = 0;
    int16_t sum_own_city_wx__own_units_cost = 0;
    int16_t target_square_wy = 0;
    int16_t target_square_wx = 0;
    int16_t landmass_node_wy = 0;
    int16_t landmass_node_wx = 0;
    int16_t square_occupation_value = 0;
    int16_t landmass_node_centroid_wy = 0;
    int16_t landmass_node_centroid_wx = 0;
    int16_t min_delta_distance = 0;
    int16_t landmass_node_index = 0;
    int16_t territory_centroid_wy = 0;
    int16_t territory_centroid_wx = 0;
    int16_t own_city_count = 0;
    // int16_t itr = 0;
    int16_t itr_units = 0;  // DNE in Dasm, reuses itr
    int16_t itr_cities = 0;  // DNE in Dasm, reuses itr
    int16_t landmass_node_count = 0;  // DNE in Dasm, reuses landmass_node_wx


    /* Phase 1: */
    EMM_Map_CONTXXX__WIP();


    /* Phase 2: */
    // ; cancel the orders of every unit on the continent except for engineers
    /* ¿ OGBUG  should clear all non-military units ? */
    /* OGBUG  should condition on same plane */
    /* (landmass indices are per-plane and the value space overlaps) */
    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(
            (_UNITS[itr_units].owner_idx == player_idx)
            &&
            (_unit_type_table[_UNITS[itr_units].type].Construction == 0)
            &&
            (_landmasses[((_UNITS[itr_units].wp * WORLD_SIZE) + (_UNITS[itr_units].wy * WORLD_WIDTH) + _UNITS[itr_units].wx)] == landmass_idx)
        )
        {
            _UNITS[itr_units].Status = us_Ready;
        }
    }


    /* Phase 3: */
/*
    BEGIN:  geographic centroid
*/
    _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] = 0;
    sum_own_city_wx__own_units_cost = 0;
    sum_own_city_wy__enemy_units_cost = 0;
    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(
            (_CITIES[itr_cities].owner_idx == player_idx)
            &&
            (_landmasses[((_CITIES[itr_cities].wp * WORLD_SIZE) + (_CITIES[itr_cities].wy * WORLD_WIDTH) + _CITIES[itr_cities].wx)] == landmass_idx)
        )
        {
            sum_own_city_wx__own_units_cost += _CITIES[itr_cities].wx;
            sum_own_city_wy__enemy_units_cost += _CITIES[itr_cities].wy;
            _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] += 1;
        }
    }
    own_city_count = _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx];
    if(own_city_count > 0)
    {
        _ai_continents.plane[wp].player[player_idx].wx_array[landmass_idx] = (sum_own_city_wx__own_units_cost / own_city_count);
        _ai_continents.plane[wp].player[player_idx].wy_array[landmass_idx] = (sum_own_city_wy__enemy_units_cost / own_city_count);
    }
/*
    END:  geographic centroid
*/


    /* Phase 5: */
/*
    BEGIN:  Classify the landmass  {lmt_NoOwnCity, lmt_Own, lmt_Contested, lmt_NoOwnCityAndAllyHasCity}
*/

    /* OGBUG  should use separate vars/clear vars beforehand - vars still contain city coordinate sums */
    for (itr_units = 0; itr_units < _units; itr_units++)
    {
        unit_type = _UNITS[itr_units].type;
        /* ¿ OGBUG  should skip all non-military units ? */
        /* Skip Settlers */
        if ((_unit_type_table[unit_type].Abilities & UA_CREATEOUTPOST) != 0)
        {
            continue;
        }
        /* OGBUG  should condition on same plane */
        if(_landmasses[((_UNITS[itr_units].wp * WORLD_SIZE) + (_UNITS[itr_units].wy * WORLD_WIDTH) + _UNITS[itr_units].wx)] == landmass_idx)
        {
            if (_UNITS[itr_units].owner_idx == player_idx)
            {
                sum_own_city_wx__own_units_cost += _unit_type_table[unit_type].cost;
            }
            else
            {
                sum_own_city_wy__enemy_units_cost += _unit_type_table[unit_type].cost;
            }
        }
    }

    /* Determine continent status based on force presence */
    /* if landmass has any cities for this player, replace city count with landmass type/status */
    if(_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] > 0)  /* HERE: stills 'own city count' */
    {
        /* If enemy forces are less than 10% of ours, we own it */
        if((sum_own_city_wy__enemy_units_cost * 10) < sum_own_city_wx__own_units_cost)
        {
            _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] = lmt_Own;
        }
        else
        {
            _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] = lmt_Contested;
        }
    }
    else  /* landmass has no player's cities */
    {
        _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] = lmt_NoOwnCity;
    }
/* maybe, change lmt_NoOwnCity to lmt_NoOwnCityAndAllyHasCity */
    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(
            (_CITIES[itr_cities].owner_idx != player_idx)
            &&
            (_CITIES[itr_cities].owner_idx != NEUTRAL_PLAYER_IDX)
            &&
            (_landmasses[((_CITIES[itr_cities].wp * WORLD_SIZE) + (_CITIES[itr_cities].wy * WORLD_WIDTH) + _CITIES[itr_cities].wx)] == landmass_idx)
        )
        {
            if(
                (_players[player_idx].Dipl.Dipl_Status[_CITIES[itr_cities].owner_idx] == DIPL_Alliance)
                &&
                (_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] == lmt_NoOwnCity)
            )
            {
                _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] = lmt_NoOwnCityAndAllyHasCity;
            }
        }
    }

    /* Landmass 0 is Ocean / 'Landmass of No-LandMass' */
    _ai_continents.plane[0].player[player_idx].type_array[0] = lmt_NoOwnCityAndAllyHasCity;
    _ai_continents.plane[1].player[player_idx].type_array[0] = lmt_NoOwnCityAndAllyHasCity;
    /* NOTE(JimBalcomb,20260517): this is weird, but just means ignore/exclude - OON effective usage - AI_Tower_Target_Worthwhile(), at line 5851, excludes lmt_NoOwnCityAndAllyHasCity */

/*
    END:  Classify the landmass  {lmt_NoOwnCity, lmt_Own, lmt_Contested, lmt_NoOwnCityAndAllyHasCity}
*/


    /* Phase 7: */
    // ¿ Refine stage point for Own/Contested/AllyLand	 ?
    /* DEDU  Is this just finding the nearest square to the territory centroid, because the territy centroid square has become occupid? */
    /* Phase 5: */
/*
    BEGIN:  Phase 5: Determine Stage Point for the Continent
            (which may include marking the landmass as not having one)
*/
    if(
        (_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] == lmt_Contested)
        ||
        (_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] == lmt_Own)
        ||
        (_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] == lmt_NoOwnCityAndAllyHasCity)
    )
    {
        // DEDU  What's in wx_array,wy_array at this point?
        // Would be from AI_Evaluate_Continents(), Phase 7 - nearest square to territory centroid?
        territory_centroid_wx = _ai_continents.plane[wp].player[player_idx].wx_array[landmass_idx];
        territory_centroid_wy = _ai_continents.plane[wp].player[player_idx].wy_array[landmass_idx];
        min_delta_distance = 1000;
        landmass_node_index = _ai_landmass_land_squares_heads[wp][landmass_idx];
        while(landmass_node_index != ST_UNDEFINED)
        {
// ...jitter (0-4) ... mostly affects ties: two tiles at exact taxicab distance ... and which one wins depends on the rolls
// ...isn't strictly the closest, occupieable square — it's probabilistically near-closest, with some randomization to break tie clusters.
            delta_distance = (
                Delta_XY_With_Wrap(_ai_landmass_land_squares_wx_array[wp][landmass_node_index], _ai_landmass_land_squares_wy_array[wp][landmass_node_index], territory_centroid_wx, territory_centroid_wy, WORLD_WIDTH)
                +
                Random(5)
            );
            if(delta_distance < min_delta_distance)
            {
                landmass_node_wx = _ai_landmass_land_squares_wx_array[wp][landmass_node_index];
                landmass_node_wy = _ai_landmass_land_squares_wy_array[wp][landmass_node_index];
                /* occupieable - no site or enemy stack */
                square_occupation_value = g_ai_evaluation_map[wp][((landmass_node_wy * WORLD_WIDTH) + landmass_node_wx)];
                if(square_occupation_value == 0)
                {
                    min_delta_distance = delta_distance;
                    target_square_wx = _ai_landmass_land_squares_wx_array[wp][landmass_node_index];
                    target_square_wy = _ai_landmass_land_squares_wy_array[wp][landmass_node_index];
                }
            }
            landmass_node_index = _ai_landmass_land_squares_lists[wp][landmass_node_index];
        }
        /*
            Update stage-point
        */
        if(min_delta_distance < 1000)
        {
            _ai_continents.plane[wp].player[player_idx].wx_array[landmass_idx] = (uint8_t)target_square_wx;
            _ai_continents.plane[wp].player[player_idx].wy_array[landmass_idx] = (uint8_t)target_square_wy;
        }
        else
        {
            _ai_continents.plane[wp].player[player_idx].wx_array[landmass_idx] = 0;
            _ai_continents.plane[wp].player[player_idx].wy_array[landmass_idx] = 0;
            _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] = lmt_NoOwnCityAndAllyHasCity;
        }
    }


    /* Phase 9: set stage-square for landmasses we can afford to pull troops from */
/*
if we are doing a war, can we move units off the current landmass?
if we can move units off the current landmass, make sure the stage-square is the dock square nearest to the war landmass
*/
    /* Phase 6: Prepare stage-square for requisition of troops */
    /* Phase 6: Own Continent - Calculate Transport Loading Center */
    if(_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] == lmt_Own)
    {
        landmass_node_centroid_wx = 0;
        landmass_node_centroid_wy = 0;
        landmass_node_count = 0;
        /* Calculate average position of all possible coastal loading points */
        /* ¿ OG BUG  _ai_landmass_dock_squares_heads[][0] ST_UNDEFINED on purpose, because _ai_landmass_war_targets[wp][player_idx] could be 0 as in NONE ? */
        landmass_node_index = _ai_landmass_dock_squares_heads[wp][_ai_landmass_war_targets[wp][player_idx]];
        while(landmass_node_index != ST_UNDEFINED)
        {
            landmass_node_centroid_wx += _ai_landmass_dock_squares_wx_array[wp][landmass_node_index];
            landmass_node_centroid_wy += _ai_landmass_dock_squares_wy_array[wp][landmass_node_index];
            landmass_node_count++;
            landmass_node_index = _ai_landmass_dock_squares_lists[wp][landmass_node_index];
        }
        /* OGBUG  possible division by 0, should `if(landmass_node_count > 0)` */
        landmass_node_centroid_wx /= landmass_node_count;
        landmass_node_centroid_wy /= landmass_node_count;
        min_delta_distance = 1000;
        landmass_node_index = _ai_landmass_dock_squares_heads[wp][landmass_idx];
        while(landmass_node_index != ST_UNDEFINED)
        {
            delta_distance = (
                Delta_XY_With_Wrap(
                    _ai_landmass_dock_squares_wx_array[wp][landmass_node_index],
                    _ai_landmass_dock_squares_wy_array[wp][landmass_node_index],
                    landmass_node_centroid_wx,
                    landmass_node_centroid_wy,
                    WORLD_WIDTH
                )
                +
                Random(5)
            );
            if(delta_distance < min_delta_distance)
            {
                landmass_node_wx = _ai_landmass_dock_squares_wx_array[wp][landmass_node_index];
                landmass_node_wy = _ai_landmass_dock_squares_wy_array[wp][landmass_node_index];
                if(g_ai_evaluation_map[wp][((landmass_node_wy * WORLD_WIDTH) + landmass_node_wx)] == 0)
                {
                    min_delta_distance = delta_distance;
                    target_square_wx = landmass_node_wx;
                    target_square_wy = landmass_node_wy;
                }
            }
            landmass_node_index = _ai_landmass_dock_squares_lists[wp][landmass_node_index];
        }
        if(min_delta_distance < 1000)
        {
            _ai_continents.plane[wp].player[player_idx].wx_array[landmass_idx] = (uint8_t)target_square_wx;
            _ai_continents.plane[wp].player[player_idx].wy_array[landmass_idx] = (uint8_t)target_square_wy;
        }
    }

    /* ¿ OGBUG  no EMMDATAH_Map() ? */

}


/*
    WIZARDS.EXE  ovr162
*/

// WZD o162p01
// drake178: sub_F48F0()

// WZD o162p02
// drake178: sub_F49E3()

// WZD o162p03
// drake178: sub_F4A9E()

// WZD o162p04
// drake178: sub_F4B96()

// WZD o162p05
// drake178: sub_F4C89()

// WZD o162p06
// drake178: sub_F4D32()

// WZD o162p07
// drake178: sub_F4E28()

// WZD o162p08
// drake178: sub_F5025()

// WZD o162p09
// drake178: sub_F514D()

// WZD o162p10
// drake178: sub_F528F()

// WZD o162p11
// drake178: sub_F5432()

// WZD o162p12
/*
OON XREF:
    NPC_Destinations()
        both calls are for city targets
        both calls are for NEUTRAL_PLAYER_IDX
¿ feels like MoO2 Ships_Try_To_Move_To_() ? 
*/
/* GEMINI */
void AI_Stack_Set_Destination(int16_t stack_idx, int16_t wx, int16_t wy, int16_t player_idx)
{
    int16_t occupant_owner_idx = 0;
    int16_t occupied = 0;
    int16_t stack_wp = 0;
    int16_t stack_wy = 0;
    int16_t stack_wx = 0;
    int16_t itr = 0;
    struct s_AI_STACK_DATA * stack_ptr = NULL;

    /* Retrieve current stack location and plane */
    stack_ptr = &_ai_all_own_stacks[stack_idx];
    stack_wx = (int8_t)stack_ptr->wx;
    stack_wy = (int8_t)stack_ptr->wy;
    stack_wp = (int8_t)stack_ptr->wp;

    /* Phase 1: Check for defenders at the target location */
    occupied = ST_FALSE;
    for (itr = 0; itr < _units; itr++)
    {
        if (((int8_t)_UNITS[itr].wx) == wx &&
            ((int8_t)_UNITS[itr].wy) == wy &&
            ((int8_t)_UNITS[itr].wp) == stack_wp)
        {
            occupied = ST_TRUE;
            occupant_owner_idx = ((int8_t)_UNITS[itr].owner_idx);
            break;
        }
    }

    /* Phase 2: Diplomatic restrictions */
    if (occupied == ST_TRUE)
    {
        /* If owner is not self and not Neutral Player */
        if (occupant_owner_idx != player_idx && occupant_owner_idx != NEUTRAL_PLAYER_IDX)
        {

            if (
                _players[player_idx].peace_duration[occupant_owner_idx] != 0
                ||
                _players[player_idx].Dipl.Dipl_Status[occupant_owner_idx] == DIPL_WizardPact
                ||
                _players[player_idx].Dipl.Dipl_Status[occupant_owner_idx] == DIPL_Alliance
            )
            {
                return; /* Cannot target due to treaty */
            }
        }
    }

    /* Phase 3: Check if the stack is strategically allowed to leave current square (garrison check) */
    if (!AI_Stack_Can_Mobilize(stack_idx, stack_wx, stack_wy, stack_wp))
    {
        return;
    }

    /* Phase 4: Assign GOTO status to all units in the stack at current location */
    for (itr = 0; itr < _units; itr++)
    {
        if (((int8_t)_UNITS[itr].wx) == stack_wx &&
            ((int8_t)_UNITS[itr].wy) == stack_wy &&
            ((int8_t)_UNITS[itr].wp) == stack_wp)
        {
            /* Do not move units that are intended for outpost creation (Settlers/Engineers) */
            if (!(_unit_type_table[_UNITS[itr].type].Abilities & UA_CREATEOUTPOST))
            {
                _UNITS[itr].Status = us_GOTO;
                _UNITS[itr].dst_wx = (int8_t)wx;
                _UNITS[itr].dst_wy = (int8_t)wy;
            }
        }
    }

    /* Mark stack as having a target/processed */
    _ai_all_own_stacks[stack_idx].value = ST_UNDEFINED;

}


// WZD o162p13
// drake178: sub_F57AF()

// WZD o162p14
// drake178: sub_F58D2()

// WZD o162p15
// drake178: sub_F59DA()

// WZD o162p16
// drake178: sub_F5B56()

// WZD o162p17
// drake178: sub_F5D73()

// WZD o162p18
// drake178: sub_F5EBF()


// WZD o162p19
void AI_Stack_Set_Boats_Goto(int16_t ai_stack_idx, int16_t wx, int16_t wy)
{
    int16_t stack_wp = 0;
    int16_t stack_wy = 0;
    int16_t stack_wx = 0;
    int16_t itr_units = 0;
    stack_wx = _ai_all_own_stacks[ai_stack_idx].wx;
    stack_wy = _ai_all_own_stacks[ai_stack_idx].wy;
    stack_wp = _ai_all_own_stacks[ai_stack_idx].wp;
    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(
            (_UNITS[itr_units].wx == stack_wx)
            &&
            (_UNITS[itr_units].wy == stack_wy)
            &&
            (_UNITS[itr_units].wp == stack_wp)
            &&
            (_unit_type_table[_UNITS[itr_units].type].Transport > 0)
        )
        {
            _UNITS[itr_units].Status = us_GOTO;
            _UNITS[itr_units].dst_wx = (int8_t)wx;
            _UNITS[itr_units].dst_wy = (int8_t)wy;
        }
    }
    _ai_all_own_stacks[ai_stack_idx].value = ST_UNDEFINED;
}


// WZD o162p20
/**
 * @brief Finds the nearest eligible dock square for ferry movement from a source square.
 *
 * Scans the linked list of dock squares for the source landmass on the specified plane,
 * filters candidates to those that have at least one adjacent occupiable land square
 * (non-zero landmass and zero in g_ai_evaluation_map), and selects the candidate with
 * the smallest wrapped delta distance to the input coordinates.
 *
 * If no eligible dock square is found, the source coordinates are returned unchanged.
 *
 * @param wx Source world x coordinate.
 * @param wy Source world y coordinate.
 * @param wp Source world plane index.
 * @param target_wx Output pointer receiving the selected dock-square x coordinate.
 * @param target_wy Output pointer receiving the selected dock-square y coordinate.
 * @return This function does not return a value; result coordinates are written via
 *         @p target_wx and @p target_wy.
 *
 * @note This routine expects AI landmass dock-square lists and evaluation-map state to
 *       be initialized for the specified plane before it is called.
 */
void Next_Nearest_Ferry_Square(int16_t wx, int16_t wy, int16_t wp, int16_t * target_wx, int16_t * target_wy)
{
    int16_t adjacent_square_is_occupieable_land = 0;
    int16_t landmass_idx = 0;
    int16_t landmass_node_index = 0;
    int16_t min_delta_distance = 0;
    int16_t delta_distance = 0;
    int16_t landmass_node_wy = 0;
    int16_t landmass_node_wx = 0;
    int16_t Return_Y = 0;
    int16_t Return_X = 0;
    int16_t wy_offset = 0;
    int16_t wx_offset = 0;

    landmass_idx = _landmasses[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)];
    min_delta_distance = 2000;
    
    landmass_node_index = _ai_landmass_dock_squares_heads[wp][landmass_idx];
    while(landmass_node_index != ST_UNDEFINED)
    {
        landmass_node_wx = _ai_landmass_dock_squares_wx_array[wp][landmass_node_index];
        landmass_node_wy = _ai_landmass_dock_squares_wy_array[wp][landmass_node_index];
        adjacent_square_is_occupieable_land = ST_FALSE;
        for(wy_offset = ST_UNDEFINED; ((wy_offset < 2) && (adjacent_square_is_occupieable_land == ST_FALSE)); wy_offset++)
        {
            for(wx_offset = ST_UNDEFINED; ((wx_offset < 2) && (adjacent_square_is_occupieable_land == ST_FALSE)); wx_offset++)
            {
                if(
                    (_landmasses[((wp * WORLD_SIZE) + ((wy_offset + landmass_node_wy) * WORLD_WIDTH) + (wx_offset + landmass_node_wx))] != 0)
                    &&
                    (g_ai_evaluation_map[wp][((wy_offset + landmass_node_wy) * WORLD_WIDTH) + (wx_offset + landmass_node_wx)] == 0)
                )
                {
                    adjacent_square_is_occupieable_land = ST_TRUE;
                }
            }
        }
        if(adjacent_square_is_occupieable_land == ST_TRUE)
        {
            delta_distance = Delta_XY_With_Wrap(landmass_node_wx, landmass_node_wy, wx, wy, WORLD_WIDTH);
            if(delta_distance < min_delta_distance)
            {
                min_delta_distance = delta_distance;
                Return_X = landmass_node_wx;
                Return_Y = landmass_node_wy;
            }
        }
        landmass_node_index = _ai_landmass_dock_squares_lists[wp][landmass_node_index];
    }

    if(min_delta_distance != 2000)
    {
        *target_wx = Return_X;
        *target_wy = Return_Y;
    }
    else
    {
        *target_wx = wx;
        *target_wy = wy;
    }
}


// WZD o162p22
// drake178: sub_F6316()


// WZD o162p23
/* COPILOT */
/**
 * @brief Finds the first adjacent land square that is a safe landing/staging candidate for the current AI pass.
 *
 * Scans the 3x3 neighborhood centered on the input square (including the center tile),
 * applies horizontal world-wrap for X coordinates, and checks each candidate for two
 * conditions: terrain is land (Square_Is_Land() == ST_TRUE) and the corresponding
 * g_ai_evaluation_map entry is exactly zero (no strength bits and no site/nonhostile flags).
 *
 * A zero map entry means no AI_TARGET_STRENGTH_MASK bits and no AI_TARGET_SITE or
 * AI_TARGET_NONHOSTILE flags are set for that square on the evaluated plane.
 *
 * On the first match in scan order, writes coordinates to the output pointers and
 * returns success.
 *
 * @param wx Source world X coordinate.
 * @param wy Source world Y coordinate.
 * @param wp World plane index.
 * @param target_wx Output pointer receiving the selected adjacent land-square X.
 * @param target_wy Output pointer receiving the selected adjacent land-square Y.
 * @return ST_TRUE if a valid square is found; otherwise ST_FALSE.
 *
 * @note This routine wraps X across world boundaries but only evaluates Y values where
 *       `curr_wy > 0` and `curr_wy < WORLD_HEIGHT`.
 */
int16_t Adjacent_Land_Square(int16_t wx, int16_t wy, int16_t wp, int16_t * target_wx, int16_t * target_wy)
{
    int16_t itr_wx = 0;
    int16_t curr_wy = 0;
    int16_t curr_wx = 0;
    for(curr_wy = (wy - 1); (wy + 2) > curr_wy; curr_wy++)
    {
        if(
            (curr_wy > 0)
            &&
            (curr_wy < WORLD_HEIGHT)
        )
        {
            for(itr_wx = (wx - 1); (wx + 2) > itr_wx; itr_wx++)
            {
                curr_wx = itr_wx;
                if(curr_wx < 0)
                {
                    curr_wx += WORLD_WIDTH;
                }
                if(curr_wx >= WORLD_WIDTH)
                {
                    curr_wx -= WORLD_WIDTH;
                }
                if(
                    (Square_Is_Land(curr_wx, curr_wy, wp) == ST_TRUE)
                    &&
                    (g_ai_evaluation_map[wp][((curr_wy * WORLD_WIDTH) + curr_wx)] == 0)
                )
                {
                    *target_wx = curr_wx;
                    *target_wy = curr_wy;
                    return ST_TRUE;
                }
            }
        }
    }
    return ST_FALSE;
}


// WZD o162p24
// drake178: sub_F64C2()

// WZD o162p25
// drake178: sub_F671D()

// WZD o162p26
// drake178: sub_F689A()

// WZD o162p27
// drake178: sub_F68C9()

// WZD o162p28
// drake178: sub_F69A6()

// WZD o162p29
// drake178: sub_F6CAB()

// WZD o162p30
// drake178: sub_F6E1A()

// WZD o162p31
// drake178: sub_F6EBF()

// WZD o162p32
/*
returns {F,T} 'found target'
in-outs wx,wy
¿ weird indexing of the local array ?
*/
/**
 * @brief Finds an adjacent unoccupied land square around the given square.
 *
 * Builds a 3x3 availability map centered on the source square, first marking
 * which neighboring tiles are land, then clearing any entries occupied by a
 * unit, an intact lair, or a city on the same plane. The center square is always
 * treated as unavailable.
 *
 * The search returns the first remaining free square in row-major order,
 * scanning from the northwest neighbor to the southeast neighbor. When such a
 * square is found, the chosen coordinates are written through the output
 * pointers.
 *
 * @param wx World x coordinate of the center square.
 * @param wy World y coordinate of the center square.
 * @param wp World plane containing the search area.
 * @param adjacent_wx Output pointer that receives the selected adjacent x coordinate.
 * @param adjacent_wy Output pointer that receives the selected adjacent y coordinate.
 *
 * @return ST_TRUE if at least one adjacent land square remains free after
 *         occupancy checks, or ST_FALSE if no qualifying square exists.
 *
 * @note Occupancy checks consider global unit, lair, and city state from
 *       _UNITS, _LAIRS, and _CITIES on the requested plane.
 */
int16_t Adjacent_Free_Square(int16_t wx, int16_t wy, int16_t wp, int16_t * adjacent_wx, int16_t * adjacent_wy)
{
    int16_t map_square_area[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t entity_wy = 0;
    int16_t entity_wx = 0;
    int16_t itr1 = 0;
    int16_t itr2 = 0;
    for(itr1 = 0; itr1 < 3; itr1++)
    {
        for(itr2 = 0; itr2 < 3; itr2++)
        {
            if(Square_Is_Land((wx + itr2 - 1), (wy + itr1 - 1), wp) == ST_FALSE)
            {
                map_square_area[((itr1 * 3) + itr2)] = ST_FALSE;
            }
            else
            {
                map_square_area[((itr1 * 3) + itr2)] = ST_TRUE;
            }
        }
    }
    map_square_area[4] = ST_FALSE;
    for(itr1 = 0; itr1 < _units; itr1++)
    {
        if(_UNITS[itr1].wp == wp)
        {
            entity_wx = _UNITS[itr1].wx;
            entity_wy = _UNITS[itr1].wy;
            if(
                (abs((entity_wx - wx)) < 2)
                &&
                (abs((entity_wy - wy)) < 2)
            )
            {
                map_square_area[(4 + (((entity_wy - wy) * 3) + (entity_wx - wx)))] = ST_FALSE;
            }
        }
    }
    for(itr1 = 0; itr1 < NUM_LAIRS; itr1++)
    {
        if((_LAIRS[itr1].intact == ST_TRUE) && (_LAIRS[itr1].wp == wp))
        {
            entity_wx = _LAIRS[itr1].wx;
            entity_wy = _LAIRS[itr1].wy;
            if(
                (abs((entity_wx - wx)) < 2)
                &&
                (abs((entity_wy - wy)) < 2)
            )
            {
                map_square_area[(4 + ((entity_wy - wy) * 3) + (entity_wx - wx))] = ST_FALSE;
            }
        }
    }
    for(itr1 = 0; itr1 < _cities; itr1++)
    {
        if(_CITIES[itr1].wp == wp)
        {
            entity_wx = _CITIES[itr1].wx;
            entity_wy = _CITIES[itr1].wy;
            if(
                (abs((entity_wx - wx)) < 2)
                &&
                (abs((entity_wy - wy)) < 2)
            )
            {
                map_square_area[(4 + ((entity_wy - wy) * 3) + (entity_wx - wx))] = ST_FALSE;
            }
        }
    }
    for(itr1 = 0; itr1 < 3; itr1++)
    {
        for(itr2 = 0; itr2 < 3; itr2++)
        {
            if(map_square_area[((itr1 * 3) + itr2)] == ST_TRUE)
            {
                *adjacent_wx = (wx + itr2 - 1);
                *adjacent_wy = (wy + itr1 - 1);
                return ST_TRUE;
            }
        }
    }
    return ST_FALSE;
}


// WZD o162p33
int16_t Make_Monster_List(int16_t budget, int16_t lair_race, int16_t * unit_type_array)
{
    int16_t rolled_unit_type = 0;
    int16_t tries = 0;
    int16_t count = 0;
    int16_t current_budget = 0;

    current_budget = budget;

    if (current_budget < 0)
    {
        current_budget = 0;
    }

    count = 0;
    tries = 0;

    /* Loop continues while budget is at least 25 and we haven't exceeded 2000 attempts */
    while (current_budget >= 25 && tries < 2000)
    {
        /* Generate a random unit index from the pool of rampage monsters */
        /* OGBUG  should be Random(44) */
        /* BUG: should be 153 (base index) */
        /* ¿ is meant to be inclusively between { ut_Magic_Spirit  = 154, ..., ut_Nagas  = 197 } ? */
        rolled_unit_type = (ut_TrollShaman + Random(47));

        if(
            (_unit_type_table[rolled_unit_type].race_type == lair_race)
            &&
            (_unit_type_table[rolled_unit_type].cost <= current_budget)
            &&
            (_unit_type_table[rolled_unit_type].Transport == 0)
            &&
            (count < MAX_STACK)
        )
        {
            /* Deduct cost and add to list */
            current_budget -= _unit_type_table[rolled_unit_type].cost;
            unit_type_array[count] = rolled_unit_type;
            count++;
        }

        tries++;
    }

    return count;
}


// WZD o162p34
/*
called by 11 other functions that are all Not-In-Use (NIU)
OON XREF:
    AI_Stack_Set_Destination()
*/
/* GEMINI */
int16_t AI_Stack_Can_Mobilize(int16_t stack_idx, int16_t wx, int16_t wy, int16_t wp)
{
    int16_t unit_owner_idx = 0;
    int16_t unit_type = 0;
    int16_t square_has_city = 0;
    int16_t itr = 0;
    int16_t garrison = 0;

    square_has_city = ST_FALSE;

    for (itr = 0; itr < _cities; itr++)
    {
        if (_CITIES[itr].wx == wx && 
            _CITIES[itr].wy == wy && 
            _CITIES[itr].wp == wp)
        {
            square_has_city = ST_TRUE;
            break;
        }
    }

    if (square_has_city == ST_FALSE)
    {
        return ST_TRUE;
    }

    /* Set minimum required defenders based on difficulty level */
    if (_difficulty < god_Normal)
    {
        garrison = 1;
    }
    else if (_difficulty == god_Normal)
    {
        garrison = 2;
    }
    else  /* (_difficulty > god_Normal) */
    {
        garrison = 3;
    }

    /* Count combat-capable units at the location */
    for (itr = 0; itr < _units; itr++)
    {
        if (garrison <= 0)
        {
            break;
        }

        if (_UNITS[itr].wx == wx && 
            _UNITS[itr].wy == wy && 
            _UNITS[itr].wp == wp)
        {
            unit_owner_idx = _UNITS[itr].owner_idx;
            unit_type = _UNITS[itr].type;

            /* Check if unit is a military unit (not Settler,Engineer,Boat) */
            if (!(_unit_type_table[unit_type].Abilities & UA_CREATEOUTPOST) &&
                _unit_type_table[unit_type].Construction == 0 &&
                _unit_type_table[unit_type].Transport == 0)
            {
                garrison--;
            }
        }
    }

    /* Fortress defense check */
    // DEDU  What is the intent of the 120 turns? Is it backwards? Supports early game exploration?
    if (unit_owner_idx != NEUTRAL_PLAYER_IDX)
    {
        if (_FORTRESSES[unit_owner_idx].wp == wp &&
            _FORTRESSES[unit_owner_idx].wx == wx &&
            _FORTRESSES[unit_owner_idx].wy == wy)
        {
            if (_turn > 120)
            {
                return ST_FALSE;
            }
        }
    }

    /* If defense requirement met, square can be left */
    if (garrison <= 0)
    {
        return ST_TRUE;
    }

    return ST_FALSE;
}


// WZD o162p35
/**
 * @brief Builds per-plane continent classifications, stage points, and
 *        settler targets for one AI player.
 *
 * This routine is the full continent-evaluation pass that feeds later stage,
 * colonization, and war-mobilization decisions. After mapping the CONTXXX work
 * data, it first checks whether any previously selected no-city continent has
 * become blocked and uses that result, together with a per-player countdown, to
 * decide whether reevaluation should run this turn. When reevaluation proceeds,
 * it clears stale non-engineer orders that no longer point at a live target,
 * recomputes geographic centroids from the player's cities, and classifies each
 * landmass as owned, contested, lacking an own city, or lacking an own city
 * while containing an allied city.
 *
 * The pass then selects new colonization targets for empty landmasses, refines
 * stage squares for owned, contested, allied, and no-city landmasses using the
 * prebuilt land-square and dock-square linked lists, and for secure owned
 * continents may shift the stage square toward the dock closest to the current
 * war-target landmass. Results are written back into the per-player continent
 * arrays and settler-target globals used by subsequent AI movement logic.
 *
 * @param player_idx Index of the AI-controlled player whose continent state is
 *                   being reevaluated.
 *
 * @return This function does not return a value. It updates
 *         @c _ai_continents, @c _ai_landmass_settler_targets,
 *         @c _ai_landmass_settler_targets_wx_array,
 *         @c _ai_landmass_settler_targets_wy_array, and
 *         @c _ai_reevaluate_continents_countdown, and may reset matching unit
 *         orders to @c us_Ready.
 *
 * @note The implementation preserves several reverse-engineered original-game
 *       quirks documented inline, including mixed military-value heuristics,
 *       possible divide-by-zero cases when no dock squares exist, and stage or
 *       settler target selection that depends on occupiable squares from
 *       @c g_ai_evaluation_map.
 * @note The routine temporarily maps the CONTXXX EMS data via
 *       @c EMM_Map_CONTXXX__WIP() and restores the default data mapping with
 *       @c EMMDATAH_Map() before returning.
 */
void AI_Evaluate_Continents(int16_t player_idx)
{
    int16_t sum_own_city_wy__enemy_units_cost[NUM_PLANES][NUM_LANDMASSES] = {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    };
    int16_t sum_own_city_wx__own_units_cost[NUM_PLANES][NUM_LANDMASSES] = {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    };
    int16_t world_offset = 0;
    int16_t force_reevaluation = 0;
    int16_t territory_centroid_wy = 0;  /* per landmass, geographic centroid of own cities */
    int16_t target_landmass_idx = 0;
    int16_t delta_distance = 0;
    int16_t target_square_wy = 0;
    int16_t target_square_wx = 0;
    int16_t territory_centroid_wx = 0;  /* per landmass, geographic centroid of own cities */
    int16_t min_delta_distance = 0;
    int16_t unit_type = 0;
    int16_t landmass_node_centroid_wy = 0;
    int16_t landmass_node_centroid_wx = 0;
    int16_t landmass_node_index = 0;
    int16_t city_count = 0;
    int16_t landmass_node_wy = 0;
    int16_t landmass_node_wx = 0;
    // int16_t itr = 0;
    int16_t itr_units = 0;  // DNE in Dasm, resuses itr
    int16_t itr_cities = 0;  // DNE in Dasm, resuses itr
    int16_t itr_landmasses = 0;  // DNE in Dasm, resuses itr
    int16_t landmass_idx = 0;  // DNE in Dasm, resuses itr
    int16_t wp = 0;  // _DI_
    int16_t square_occupation_value = 0;  // DNE in Dasm, resuses found_targe
    int16_t landmass_node_count = 0;  // DNE in Dasm, reuses world_offset


    /* Phase 1: */
    EMM_Map_CONTXXX__WIP();  /* needs all the landmass data */


    /* Phase 2: */
/*
    BEGIN:  Reevaluation Check
*/
/*
HERE:
    lmt_NoOwnCity is 'landmass previously targeted for colonization'
    So, here we are checking if the square has become unoccupieable.
*/
    force_reevaluation = ST_FALSE;
    for(wp = 0; wp < NUM_PLANES; wp++)
    {
        for(landmass_idx = 0; ((landmass_idx < NUM_LANDMASSES) && (force_reevaluation == ST_FALSE)); landmass_idx++)
        {
            if(_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] == lmt_NoOwnCity)
            {
                /* unoccupieable - site or enemy stack */
                if(g_ai_evaluation_map[wp][((_ai_continents.plane[wp].player[player_idx].wy_array[landmass_idx] * WORLD_WIDTH) + _ai_continents.plane[wp].player[player_idx].wx_array[landmass_idx])] != 0)
                {
                    force_reevaluation = ST_TRUE;
                }
            }
        }
    }
    /* if settler landing bad, force reevaluation */
    if(force_reevaluation == ST_TRUE)
    {
        _ai_reevaluate_continents_countdown[player_idx] = 0;
    }
    _ai_reevaluate_continents_countdown[player_idx] -= 1;
    if(_ai_reevaluate_continents_countdown[player_idx] > 0)
    {
        return ;
    }
    _ai_reevaluate_continents_countdown[player_idx] = (25 + Random(10) + Random(10));
/*
    END:  Reevaluation Check
*/


    /* Phase 3: */
// ; cancel the orders of all non-engineer units moving to a square with no enemy presence
/* OGBUG  should check unit wp */
/* OGBUG  settlers and transports (non-military) are not excluded ? */
/* DEDU  ¿ clearning orders for destination without a site or enemy stack means the orders are considered invalid because they don't have a target for an attack ? */
/* OGBUG  wp leftover from previously loop */
    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(
            (_UNITS[itr_units].owner_idx == player_idx)
            &&
            (_unit_type_table[_UNITS[itr_units].type].Construction == 0)
            &&
            (g_ai_evaluation_map[wp][((_UNITS[itr_units].dst_wy * WORLD_WIDTH) + _UNITS[itr_units].dst_wx)] == 0)
        )
        {
            _UNITS[itr_units].Status = us_Ready;  // 'MoM Demo': NO ORDERS
        }
    }


    /* Phase 4: */
/*
    BEGIN:  Center of Territory, per Landmass?  (geographic centroid)
*/
    for(wp = 0; wp < NUM_PLANES; wp++)
    {
        for(landmass_idx = 0; landmass_idx < NUM_LANDMASSES; landmass_idx++)
        {
            _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] = 0;  /* NOTE: actually just 0, because it's about to be used for city count */
            sum_own_city_wx__own_units_cost[wp][landmass_idx] = 0;
            sum_own_city_wy__enemy_units_cost[wp][landmass_idx] = 0;
        }
    }
    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(_CITIES[itr_cities].owner_idx == player_idx)
        {
            wp = _CITIES[itr_cities].wp;
            landmass_idx = _landmasses[((wp * WORLD_SIZE) + (_CITIES[itr_cities].wy * WORLD_WIDTH) + _CITIES[itr_cities].wx)];
            if(landmass_idx < NUM_LANDMASSES)
            {
                sum_own_city_wx__own_units_cost[wp][landmass_idx] += _CITIES[itr_cities].wx;
                sum_own_city_wy__enemy_units_cost[wp][landmass_idx] += _CITIES[itr_cities].wy;
                _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] += 1;
            }
        }
    }
    for(wp = 0; wp < NUM_PLANES; wp++)
    {
        for(landmass_idx = 0; landmass_idx < NUM_LANDMASSES; landmass_idx++)
        {
            city_count = _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx];
            if(city_count > 0)
            {
                _ai_continents.plane[wp].player[player_idx].wx_array[landmass_idx] = (sum_own_city_wx__own_units_cost[wp][landmass_idx] / city_count);
                _ai_continents.plane[wp].player[player_idx].wy_array[landmass_idx] = (sum_own_city_wy__enemy_units_cost[wp][landmass_idx] / city_count);
            }
        }
    }
/*
    END:  Center of Territory, per Landmass?  (geographic centroid)
*/


    /* Phase 5: */
/*
    BEGIN:  Classify the landmass  {lmt_NoOwnCity, lmt_Own, lmt_Contested, lmt_NoOwnCityAndAllyHasCity}
*/
    for(itr_landmasses = 0; itr_landmasses < NUM_LANDMASSES; itr_landmasses++)
    {
        sum_own_city_wx__own_units_cost[0][itr_landmasses] = 0;
        sum_own_city_wx__own_units_cost[1][itr_landmasses] = 0;
        sum_own_city_wy__enemy_units_cost[0][itr_landmasses] = 0;
        sum_own_city_wy__enemy_units_cost[1][itr_landmasses] = 0;
    }
/*  ¿ OGBUG  should exclude non-military, rather than just Settlers (or, why exclude anything?) ? */
    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        unit_type = _UNITS[itr_units].type;
        if((_unit_type_table[unit_type].Abilities & UA_CREATEOUTPOST) != 0)
        {
            continue;
        }
        wp = _UNITS[itr_units].wp;
        landmass_idx = _landmasses[((wp * WORLD_SIZE) + (_UNITS[itr_units].wy * WORLD_WIDTH) + _UNITS[itr_units].wx)];
        if(landmass_idx < NUM_LANDMASSES)
        {
            if(_UNITS[itr_units].owner_idx == player_idx)
            {
                sum_own_city_wx__own_units_cost[wp][landmass_idx] += _unit_type_table[unit_type].cost;
            }
            else
            {
                sum_own_city_wy__enemy_units_cost[wp][landmass_idx] += _unit_type_table[unit_type].cost;
            }
        }
    }
/* {1:lmt_Own, 2:lmt_Contested, 3:lmt_NoOwnCity} */
    for(wp = 0; wp < NUM_PLANES; wp++)
    {
        for(landmass_idx = 0; landmass_idx < NUM_LANDMASSES; landmass_idx++)
        {
            /* if already lmt_NoTargets and no enemy units, then still lmt_NoTargets */
            /* OGBUG  checking for landmass status/type, but actually just city count */
            if(
                (_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] == lmt_NoTargets)
                &&
                (sum_own_city_wy__enemy_units_cost[wp][landmass_idx] == 0)
            )
            {
                continue;
            }
            if(_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] > 0)
            {
                /* Classify lmt_Own when our unit-cost on this landmass is more than 10× the enemy's unit-cost (equivalently: enemy unit-cost is < 10% of ours). */
                if(
                    sum_own_city_wx__own_units_cost[wp][landmass_idx]
                    >
                    (sum_own_city_wy__enemy_units_cost[wp][landmass_idx] * 10)  /* ¿ OGBUG  could overflow ? */
                )
                {
                    _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] = lmt_Own;
                }
                else
                {
                    _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] = lmt_Contested;
                }
            }
            else
            {
                _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] = lmt_NoOwnCity;
            }
        }
    }
/* maybe, change lmt_NoOwnCity to lmt_NoOwnCityAndAllyHasCity */
    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(_CITIES[itr_cities].owner_idx == player_idx)
        {
            continue;
        }
        if(_CITIES[itr_cities].owner_idx == NEUTRAL_PLAYER_IDX)
        {
            continue;
        }
        if(_players[player_idx].Dipl.Dipl_Status[_CITIES[itr_cities].owner_idx] != DIPL_Alliance)
        {
            continue;
        }
        wp = _CITIES[itr_cities].wp;
        landmass_idx = _landmasses[((wp * WORLD_SIZE) + (_CITIES[itr_cities].wy * WORLD_WIDTH) + _CITIES[itr_cities].wx)];
        if(_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] == lmt_NoOwnCity)
        {
            _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] = lmt_NoOwnCityAndAllyHasCity;
        }
    }

    _ai_continents.plane[0].player[player_idx].type_array[0] = lmt_NoOwnCityAndAllyHasCity;
    _ai_continents.plane[1].player[player_idx].type_array[0] = lmt_NoOwnCityAndAllyHasCity;

/*
    END:  Classify the landmass  {lmt_NoOwnCity, lmt_Own, lmt_Contested, lmt_NoOwnCityAndAllyHasCity}
*/


    /* Phase 6: Choose Unoccupied Landmass for Settler Target */
    for(wp = 0; wp < NUM_PLANES; wp++)
    {
        min_delta_distance = 1000;
        for(landmass_idx = 1; landmass_idx < NUM_LANDMASSES; landmass_idx++)
        {
            if(_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] != lmt_NoOwnCity)
            {
                continue;
            }
            if(sum_own_city_wy__enemy_units_cost[wp][landmass_idx] != 0)
            {
                continue;
            }
            landmass_node_index = _ai_landmass_land_squares_heads[wp][landmass_idx];
            while(
                (landmass_node_index != ST_UNDEFINED)
                &&
                (square_occupation_value == ST_FALSE)
            )
            {
                landmass_node_wx = _ai_landmass_land_squares_wx_array[wp][landmass_node_index];
                landmass_node_wy = _ai_landmass_land_squares_wy_array[wp][landmass_node_index];
                world_offset = ((landmass_node_wy * WORLD_WIDTH) + landmass_node_wx);
                /* occupieable - no site or enemy stack */
                if(g_ai_evaluation_map[wp][world_offset] == 0)
                {
                    /* OGBUG  landmass_node_centroid_wx and landmass_node_centroid_wy are uninitialized */
                    delta_distance = (
                        Random(20)
                        +
                        Delta_XY_With_Wrap(landmass_node_wx, landmass_node_wy, landmass_node_centroid_wx, landmass_node_centroid_wy, WORLD_WIDTH)
                    );
                    if(delta_distance < min_delta_distance)
                    {
                        min_delta_distance = delta_distance;
                        target_square_wx = _ai_landmass_land_squares_wx_array[wp][landmass_node_index];
                        target_square_wy = _ai_landmass_land_squares_wy_array[wp][landmass_node_index];
                        target_landmass_idx = landmass_idx;
                    }
                }
                landmass_node_index = _ai_landmass_land_squares_lists[wp][landmass_node_index];
            }
        }
        if(min_delta_distance < 1000)
        {
            _ai_landmass_settler_targets[wp][player_idx] = (uint8_t)target_landmass_idx;
            _ai_landmass_settler_targets_wx_array[wp][player_idx] = (uint8_t)target_square_wx;
            _ai_landmass_settler_targets_wy_array[wp][player_idx] = (uint8_t)target_square_wy;
        }
        else
        {
            _ai_landmass_settler_targets[wp][player_idx] = 0;  /* no unoccupied landmass available */
        }
    }


    /* Phase 7: */
    // ¿ Refine stage point for Own/Contested/AllyLand	 ?
    /* DEDU  Is this just finding the nearest square to the territory centroid, because the territy centroid square has become occupid? */
    for(wp = 0; wp < NUM_PLANES; wp++)
    {
        for(landmass_idx = 1; landmass_idx < NUM_LANDMASSES; landmass_idx++)
        {
            if(
                (_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] == lmt_Contested)
                ||
                (_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] == lmt_Own)
                ||
                (_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] == lmt_NoOwnCityAndAllyHasCity)
            )
            {
                territory_centroid_wx = _ai_continents.plane[wp].player[player_idx].wx_array[landmass_idx];
                territory_centroid_wy = _ai_continents.plane[wp].player[player_idx].wy_array[landmass_idx];
                /* DEDU  What does this test amount to? reasons to update the stage-square or even ignore the landmass? */
                // CLAUDE: Skip if current centroid is already a valid unoccupied square on this landmass
                if(
                    (g_ai_evaluation_map[wp][((territory_centroid_wy * WORLD_WIDTH) + territory_centroid_wx)] == 0)
                    &&
                    (_landmasses[((wp * WORLD_SIZE) + (territory_centroid_wy * WORLD_WIDTH) + territory_centroid_wx)] == landmass_idx)
                )
                {
                    continue;
                }
                min_delta_distance = 1000;
                landmass_node_index = _ai_landmass_land_squares_heads[wp][landmass_idx];
                while(landmass_node_index != ST_UNDEFINED)
                {
                    /* no jitter, wants exact */
                    delta_distance = Delta_XY_With_Wrap(
                        _ai_landmass_land_squares_wx_array[wp][landmass_node_index],
                        _ai_landmass_land_squares_wy_array[wp][landmass_node_index],
                        territory_centroid_wx,
                        territory_centroid_wy,
                        WORLD_WIDTH
                    );
                    if(delta_distance < min_delta_distance)
                    {
                        landmass_node_wx = _ai_landmass_land_squares_wx_array[wp][landmass_node_index];
                        landmass_node_wy = _ai_landmass_land_squares_wy_array[wp][landmass_node_index];
                        /* occupieable - no site or enemy stack */
                        square_occupation_value = g_ai_evaluation_map[wp][((landmass_node_wy * WORLD_WIDTH) + landmass_node_wx)];
                        if(square_occupation_value == 0)
                        {
                            min_delta_distance = delta_distance;
                            target_square_wx = _ai_landmass_land_squares_wx_array[wp][landmass_node_index];
                            target_square_wy = _ai_landmass_land_squares_wy_array[wp][landmass_node_index];
                            target_landmass_idx = landmass_idx;  /* OGBUG  c&p error? target_landmass_idx is not used in this phase */
                        }
                    }
                    landmass_node_index = _ai_landmass_land_squares_lists[wp][landmass_node_index];
                }
                if(min_delta_distance < 1000)
                {
                    _ai_continents.plane[wp].player[player_idx].wx_array[landmass_idx] = (uint8_t)target_square_wx;
                    _ai_continents.plane[wp].player[player_idx].wy_array[landmass_idx] = (uint8_t)target_square_wy;
                }
                else
                {
                    _ai_continents.plane[wp].player[player_idx].wx_array[landmass_idx] = 0;
                    _ai_continents.plane[wp].player[player_idx].wy_array[landmass_idx] = 0;
                    _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] = lmt_NoOwnCityAndAllyHasCity;
                }
            }
        }
    }


    /* Phase 8: set stage-square to nearest occupieable square, for unsettled landmasses */
/*
HERE:
    lmt_NoOwnCity means we don't have a city on the landmass
    also, we didn't change it to lmt_NoOwnCityAndAllyHasCity, so no ally has a city either
*/
    for(wp = 0; wp < NUM_PLANES; wp++)
    {
        for(landmass_idx = 1; landmass_idx < NUM_LANDMASSES; landmass_idx++)
        {
            if(_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] != lmt_NoOwnCity)
            {
                continue;
            }
            territory_centroid_wx = _ai_continents.plane[wp].player[player_idx].wx_array[landmass_idx];
            territory_centroid_wy = _ai_continents.plane[wp].player[player_idx].wy_array[landmass_idx];
            min_delta_distance = 1000;
            landmass_node_index = _ai_landmass_dock_squares_heads[wp][landmass_idx];
            while(landmass_node_index != ST_UNDEFINED)
            {
                landmass_node_wx = _ai_landmass_dock_squares_wx_array[wp][landmass_node_index];
                landmass_node_wy = _ai_landmass_dock_squares_wy_array[wp][landmass_node_index];
                /* occupieable - no site or enemy stack */
                if(g_ai_evaluation_map[wp][((landmass_node_wy * WORLD_WIDTH) + landmass_node_wx)] == 0)
                {
                    /* ¿ OGBUG  no jitter ? */
                    delta_distance = Delta_XY_With_Wrap(landmass_node_wx, landmass_node_wy, territory_centroid_wx, territory_centroid_wy, WORLD_WIDTH);
                    if(delta_distance < min_delta_distance)
                    {
                        min_delta_distance = delta_distance;
                        target_square_wx = _ai_landmass_dock_squares_wx_array[wp][landmass_node_index];
                        target_square_wy = _ai_landmass_dock_squares_wy_array[wp][landmass_node_index];
                        target_landmass_idx = landmass_idx;  /* OGBUG  c&p error? target_landmass_idx is not used in this phase */
                    }
                }
                landmass_node_index = _ai_landmass_dock_squares_lists[wp][landmass_node_index];
            }
            if(min_delta_distance < 1000)
            {
                _ai_continents.plane[wp].player[player_idx].wx_array[landmass_idx] = (uint8_t)target_square_wx;
                _ai_continents.plane[wp].player[player_idx].wy_array[landmass_idx] = (uint8_t)target_square_wy;
            }
            else
            {
                _ai_continents.plane[wp].player[player_idx].wx_array[landmass_idx] = 0;
                _ai_continents.plane[wp].player[player_idx].wy_array[landmass_idx] = 0;
                _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] = lmt_NoOwnCityAndAllyHasCity;
            }
        }
    }


    /* Phase 9: set stage-square for landmasses we can afford to pull troops from */
/*
if we are doing a war, can we move units off the current landmass?
if we can move units off the current landmass, make sure the stage-square is the dock square nearest to the war landmass
*/
    for(wp = 0; wp < NUM_PLANES; wp++)
    {
        /* Any war effort on this plane? */
        if(_ai_landmass_war_targets[wp][player_idx] != 0)
        {
            for(landmass_idx = 1; landmass_idx < NUM_LANDMASSES; landmass_idx++)
            {
                /* Are we secure enough to relocate troops from this landmass? */
                if(_ai_continents.plane[wp].player[player_idx].type_array[landmass_idx] != lmt_Own)
                {
                    continue;
                }
                landmass_node_centroid_wx = 0;
                landmass_node_centroid_wy = 0;
                landmass_node_count = 0;
                landmass_node_index = _ai_landmass_dock_squares_heads[wp][_ai_landmass_war_targets[wp][player_idx]];
                while(landmass_node_index != ST_UNDEFINED)
                {
                    landmass_node_centroid_wx += _ai_landmass_dock_squares_wx_array[wp][landmass_node_index];
                    landmass_node_centroid_wy += _ai_landmass_dock_squares_wy_array[wp][landmass_node_index];
                    landmass_node_count++;
                    landmass_node_index = _ai_landmass_dock_squares_lists[wp][landmass_node_index];
                }
                /* OGBUG  divide by zero, should `if(landmass_node_count > 0)` */
                landmass_node_centroid_wx /= landmass_node_count;
                landmass_node_centroid_wy /= landmass_node_count;
                min_delta_distance = 1000;
                landmass_node_index = _ai_landmass_dock_squares_heads[wp][landmass_idx];
                while(landmass_node_index != ST_UNDEFINED)
                {
                    /* ¿ OGBUG  no jitter ? */
                    delta_distance = Delta_XY_With_Wrap(
                        _ai_landmass_dock_squares_wx_array[wp][landmass_node_index],
                        _ai_landmass_dock_squares_wy_array[wp][landmass_node_index],
                        landmass_node_centroid_wx,
                        landmass_node_centroid_wy,
                        WORLD_WIDTH
                    );
                    if(delta_distance < min_delta_distance)
                    {
                        landmass_node_wx = _ai_landmass_dock_squares_wx_array[wp][landmass_node_index];
                        landmass_node_wy = _ai_landmass_dock_squares_wy_array[wp][landmass_node_index];
                        if(g_ai_evaluation_map[wp][((landmass_node_wy * WORLD_WIDTH) + landmass_node_wx)] == 0)
                        {
                            min_delta_distance = delta_distance;
                            target_square_wx = landmass_node_wx;
                            target_square_wy = landmass_node_wy;
                        }
                    }
                    landmass_node_index = _ai_landmass_dock_squares_lists[wp][landmass_node_index];
                }
                if(min_delta_distance < 1000)
                {
                    _ai_continents.plane[wp].player[player_idx].wx_array[landmass_idx] = (uint8_t)target_square_wx;
                    _ai_continents.plane[wp].player[player_idx].wy_array[landmass_idx] = (uint8_t)target_square_wy;
                }
            }
        }
    }


    /* Phase 10:  */
    /* Restore EMM mapping to default Data block */
/* Safely remap the standard Data Handle before exiting */
    EMMDATAH_Map();  /* ¿ set EMM back to the default handle - EMMDATAH ? */

}


// WZD o162p36
/**
 * @brief Builds the per-plane AI evaluation map for one wizard's turn.
 *
 * @details Clears @c g_ai_evaluation_map for both planes, then repopulates each
 * square with packed evaluation data used by later AI targeting and movement
 * logic. For units not owned by @p player_idx, one tenth of each unit's
 * effective combat strength is added to the square value. Squares occupied by
 * wizards considered non-hostile to @p player_idx are additionally flagged
 * with @c AI_TARGET_NONHOSTILE. Intact lairs contribute guardian strength, and
 * city, lair, and node squares are marked with @c AI_TARGET_SITE. Non-owned
 * cities also add a small extra value to make them eligible as AI targets.
 *
 * @param player_idx Index of the wizard whose diplomatic state is used to
 * classify other players as hostile or non-hostile while building the map.
 *
 * @note This routine mutates the global @c g_ai_evaluation_map data for both
 * planes. The low 14 bits store the accumulated evaluation value, while the
 * upper bits store flags such as @c AI_TARGET_NONHOSTILE and
 * @c AI_TARGET_SITE. The routine also depends on the continent-mapping state
 * prepared by @c EMM_Map_CONTXXX__WIP().
 *
 * @warning The implementation is still under reverse-engineering and contains
 * defensive range checks and naming that do not yet fully describe the final
 * intent of the original routine.
 */
void AI_Evaluation_Map(int16_t player_idx)
{
    int16_t nonhostiles[NUM_PLAYERS] = { 0, 0, 0, 0, 0, 0 };
    int16_t xy_ofst = 0;
    int16_t unit_owner_idx = 0;
    int16_t map_square_count = 0;
    int16_t itr = 0;
    int16_t itr_units = 0;
    int16_t wp = 0;
    int16_t strength = 0;
    struct s_UNIT * p_unit = NULL;
    struct s_LAIR * p_lair = NULL;
    struct s_CITY * p_city = NULL;
    struct s_NODE * p_node = NULL;

    EMM_Map_CONTXXX__WIP();

    /* Identify non-hostile players relative to the current player */
    for(itr = 0; itr < NUM_PLAYERS; itr++)
    {
        /* Check for Wizard Pact, Alliance, Peace Treaty, or low Hostility */
        if(
            (_players[player_idx].Dipl.Dipl_Status[itr] == DIPL_WizardPact)
            ||
            (_players[player_idx].Dipl.Dipl_Status[itr] == DIPL_Alliance)
            ||
            (_players[player_idx].peace_duration[itr] > 0)
            ||
            (_players[player_idx].Hostility[itr] < 2)
        )
        {
            nonhostiles[itr] = ST_TRUE;
        }
        else
        {
            nonhostiles[itr] = ST_FALSE;
        }
    }

    /* Clear the strength maps for both planes (Arcanus and Myrror) */
    map_square_count = WORLD_SIZE;
    for(wp = 0; wp < NUM_PLANES; wp++)
    {
        for(itr = 0; itr < map_square_count; itr++)
        {
            g_ai_evaluation_map[wp][itr] = 0;
        }
    }

    /* Iterate through all units and add their strength to the map */
    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        p_unit = &_UNITS[itr_units];

        // OGBUG  by (bad) design  assert(_UNITS[itr_units].owner_idx != ST_UNDEFINED);
        /* HACK */  if(p_unit->owner_idx == ST_UNDEFINED)
        /* HACK */  {
        /* HACK */      continue;
        /* HACK */  }
        assert(p_unit->wp != ST_UNDEFINED);
        /* HACK */  if(p_unit->wp == ST_UNDEFINED)
        /* HACK */  {
        /* HACK */      continue;
        /* HACK */  }

        unit_owner_idx = p_unit->owner_idx;

        /* Skip units belonging to the player being evaluated */
        if(
            (unit_owner_idx == player_idx)
            ||
            (p_unit->owner_idx == player_idx)  // ; conflicting condition - will always jump
        )
        {
            continue;
        }

        wp = p_unit->wp;
        xy_ofst = ((p_unit->wy * WORLD_WIDTH) + p_unit->wx);

        /* Add 1/10th of the unit's effective strength to the map square */
        strength = Effective_Unit_Strength(itr) / 10;
        g_ai_evaluation_map[wp][xy_ofst] += strength;

        /* If the unit owner is non-hostile, mark the square with a flag */
        // OGBUG  will index nonhostiles[] with unit_owner_idx = ST_UNDEFINED, because it *sanitizes* owner_idx to ST_UNDEFINED at start of turn
        if(nonhostiles[unit_owner_idx] == ST_TRUE)
        {
            g_ai_evaluation_map[wp][xy_ofst] |= AI_TARGET_NONHOSTILE;
        }

    }

    /* Iterate through all lairs and add guardian strength */
    for(itr = 0; itr < NUM_LAIRS; itr++)
    {

        if(
            (_LAIRS[itr].intact == ST_TRUE)
            &&
            (_LAIRS[itr].guard1_count > 0)
        )
        {
            assert((_LAIRS[itr].wp >= WORLD_PMIN) && (_LAIRS[itr].wp <= WORLD_PMAX));
            assert((_LAIRS[itr].wx >= WORLD_XMIN) && (_LAIRS[itr].wx <= WORLD_XMAX));
            assert((_LAIRS[itr].wy >= WORLD_YMIN) && (_LAIRS[itr].wy <= WORLD_YMAX));
            /* HACK */  if(
            /* HACK */      (_LAIRS[itr].wp < WORLD_PMIN) || (_LAIRS[itr].wp >= WORLD_PMAX)
            /* HACK */      ||
            /* HACK */      (_LAIRS[itr].wx < WORLD_XMIN) || (_LAIRS[itr].wx >= WORLD_XMAX)
            /* HACK */      ||
            /* HACK */      (_LAIRS[itr].wy < WORLD_YMIN) || (_LAIRS[itr].wy >= WORLD_YMAX)
            /* HACK */  )
            /* HACK */  {
            /* HACK */      continue;
            /* HACK */  }

            wp = _LAIRS[itr].wp;
            xy_ofst = ((_LAIRS[itr].wy * WORLD_WIDTH) + _LAIRS[itr].wx);

            /* Add Guardian 1 strength */
            strength = (Effective_Unit_Type_Strength(_LAIRS[itr].guard1_unit_type) / 10) * (_LAIRS[itr].guard1_count & 0x0F);
            g_ai_evaluation_map[wp][xy_ofst] += strength;

            /* Add Guardian 2 strength */
            strength = (Effective_Unit_Type_Strength(_LAIRS[itr].guard2_unit_type) / 10) * (_LAIRS[itr].guard2_count & 0x0F);
            g_ai_evaluation_map[wp][xy_ofst] += strength;

        }

    }

    /* Iterate through all cities and flag them on the map */
    for(itr = 0; itr < _cities; itr++)
    {
        p_city = &_CITIES[itr];
        assert((p_city->wp >= WORLD_PMIN) && (p_city->wp <= WORLD_PMAX));
        assert((p_city->wx >= WORLD_XMIN) && (p_city->wx <= WORLD_XMAX));
        assert((p_city->wy >= WORLD_YMIN) && (p_city->wy <= WORLD_YMAX));
        /* HACK */  if(
        /* HACK */      (p_city->wp < WORLD_PMIN) || (p_city->wp >= WORLD_PMAX)
        /* HACK */      ||
        /* HACK */      (p_city->wx < WORLD_XMIN) || (p_city->wx >= WORLD_XMAX)
        /* HACK */      ||
        /* HACK */      (p_city->wy < WORLD_YMIN) || (p_city->wy >= WORLD_YMAX)
        /* HACK */  )
        /* HACK */  {
        /* HACK */      continue;
        /* HACK */  }

        wp = p_city->wp;
        xy_ofst = ((p_city->wy * WORLD_WIDTH) + p_city->wx);

        g_ai_evaluation_map[wp][xy_ofst] |= AI_TARGET_SITE;

        /* If the city is not owned by the current player, increment value for AI targeting */
        if(p_city->owner_idx != player_idx)
        {
            g_ai_evaluation_map[wp][xy_ofst] += 1;
        }
        
    }

    /* Re-iterate through all lairs to flag intact lairs */
    for(itr = 0; itr < NUM_LAIRS; itr++)
    {
        p_lair = &_LAIRS[itr];

        if(p_lair->intact != ST_FALSE)  /* CAUTION: 0xC0 != ST_FALSE */
        {

            assert((p_lair->wp >= WORLD_PMIN) && (p_lair->wp <= WORLD_PMAX));
            assert((p_lair->wx >= WORLD_XMIN) && (p_lair->wx <= WORLD_XMAX));
            assert((p_lair->wy >= WORLD_YMIN) && (p_lair->wy <= WORLD_YMAX));
            /* HACK */  if(
            /* HACK */      (p_lair->wp < WORLD_PMIN) || (p_lair->wp >= WORLD_PMAX)
            /* HACK */      ||
            /* HACK */      (p_lair->wx < WORLD_XMIN) || (p_lair->wx >= WORLD_XMAX)
            /* HACK */      ||
            /* HACK */      (p_lair->wy < WORLD_YMIN) || (p_lair->wy >= WORLD_YMAX)
            /* HACK */  )
            /* HACK */  {
            /* HACK */      continue;
            /* HACK */  }

            wp = p_lair->wp;
            xy_ofst = ((p_lair->wy * WORLD_WIDTH) + p_lair->wx);

            g_ai_evaluation_map[wp][xy_ofst] |= AI_TARGET_SITE;

        }

    }

    /* Iterate through all magic nodes and flag them */
    for(itr = 0; itr < NUM_NODES; itr++)
    {
        p_node = &_NODES[itr];
        
        assert((p_node->wp >= WORLD_PMIN) && (p_node->wp <= WORLD_PMAX));
        assert((p_node->wx >= WORLD_XMIN) && (p_node->wx <= WORLD_XMAX));
        assert((p_node->wy >= WORLD_YMIN) && (p_node->wy <= WORLD_YMAX));
        /* HACK */  if(
        /* HACK */      (p_node->wp < WORLD_PMIN) || (p_node->wp >= WORLD_PMAX)
        /* HACK */      ||
        /* HACK */      (p_node->wx < WORLD_XMIN) || (p_node->wx >= WORLD_XMAX)
        /* HACK */      ||
        /* HACK */      (p_node->wy < WORLD_YMIN) || (p_node->wy >= WORLD_YMAX)
        /* HACK */  )
        /* HACK */  {
        /* HACK */      continue;
        /* HACK */  }

        wp = p_node->wp;
        xy_ofst = ((p_node->wy * WORLD_WIDTH) + p_node->wx);

        g_ai_evaluation_map[wp][xy_ofst] |= AI_TARGET_SITE;

    }

}


// WZD o162p37
void AI_Choose_War_Landmass(int16_t player_idx)
{
    int16_t final_landmass_weights[NUM_LANDMASSES] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Continent_Values[NUM_LANDMASSES] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t city_owner_idx = 0;
    int16_t first_hostile_player_idx = 0;
    int16_t reevaluate_the_current_plane = 0;
    int16_t landmass_idx = 0;
    int16_t own_city_count = 0;
    int16_t delta_distance = 0;  // DNE in Dasm; reuses own_city_count
    int16_t dock_square_wy = 0;
    int16_t dock_square_wx = 0;
    int16_t Value_Sum = 0;
    int16_t Landmass_Loopvar = 0;
    int16_t min_delta_distance = 0;
    int16_t wp = 0;
    int16_t empire_wy = 0;
    int16_t empire_wx = 0;
    int16_t itr_players = 0;  // _DI_
    int16_t itr_cities = 0;  // _DI_
    int16_t itr_landmasses = 0;  // _DI_
    uint8_t continent_status;  // DNE in Dasm
    int16_t landmass_weight_check = 0;  // DNE in Dasm, reuses Value_Sum
    int16_t landmass_node_index = 0;  // DNE in Dasm, reuses Value_Sum

    first_hostile_player_idx = ST_UNDEFINED;

    EMM_Map_CONTXXX__WIP();

    /* OGBUG  should check spl_Spell_Of_Mastery, not spl_Fire_Elemental */
    /* OGBUG  should exclude self - `itr_players != player_idx` */
    for(itr_players = 0; ((itr_players < _num_players) & (first_hostile_player_idx == ST_UNDEFINED)); itr_players++)
    {
        if(
            (_players[player_idx].Dipl.Dipl_Status[itr_players] >= DIPL_War)
            ||
            (_players[itr_players].casting_spell_idx == spl_Fire_Elemental)
        )
        {
            first_hostile_player_idx = itr_players;
        }
    }


    for(wp = 0; wp < NUM_PLANES; wp++)
    {

        reevaluate_the_current_plane = ST_FALSE;

        landmass_idx = _ai_landmass_war_targets[wp][player_idx];

        if(landmass_idx == 0)  /* this player has no war/jihad landmass on this plane */
        {

            reevaluate_the_current_plane = ST_TRUE;

        }
        else
        {

            continent_status = _ai_continents.plane[wp].player[player_idx].type_array[landmass_idx];
            // (type - 1), so {1,2,3,4,5}
            // Dasm and Gemini don't feel right
            switch(continent_status)
            {

                case lmt_Unevaluated:
                {
                    // DNE
                } break;

                case lmt_Own:
                {
                    reevaluate_the_current_plane = ST_FALSE;
                } break;

                case lmt_Contested:
                {
                    reevaluate_the_current_plane = ST_FALSE;
                } break;

                case lmt_NoOwnCity:
                case lmt_NoOwnCityAndAllyHasCity:
                {
                    for(itr_players = 0; itr_players < NUM_PLAYERS; itr_players++)
                    {
                        Continent_Values[itr_players] = 0;
                    }
                    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
                    {
                        // ; BUG: what if the city is on the other plane?
                        if(_landmasses[((wp * WORLD_SIZE) + (_CITIES[itr_cities].wy * WORLD_WIDTH) + _CITIES[itr_cities].wx)] == landmass_idx)
                        {
                            if(_CITIES[itr_cities].owner_idx != player_idx)
                            {
                                city_owner_idx = _CITIES[itr_cities].owner_idx;
                                Continent_Values[city_owner_idx] += 1;
                            }
                        }
                    }
                    if(first_hostile_player_idx > ST_UNDEFINED)
                    {
                        if(Continent_Values[first_hostile_player_idx] == 0)
                        {
                            reevaluate_the_current_plane = ST_TRUE;
                        }
                        else
                        {
                            reevaluate_the_current_plane = ST_FALSE;
                        }
                    }
                    else
                    {
                        reevaluate_the_current_plane = ST_TRUE;
                        if(Continent_Values[NEUTRAL_PLAYER_IDX] > 0)
                        {
                            reevaluate_the_current_plane = ST_FALSE;
                        }
                        else
                        {
                            for(itr_players = 0; itr_players < _num_players; itr_players++)
                            {
                                if(reevaluate_the_current_plane == ST_TRUE)
                                {
                                    if(_players[player_idx].Hostility[itr_players] >= 2)
                                    {
                                        if(Continent_Values[itr_players] > 0)
                                        {
                                            reevaluate_the_current_plane = ST_FALSE;
                                        }
                                    }
                                }
                            }
                        }
                    }
                } break;

                case lmt_Leaveable:
                {
                    reevaluate_the_current_plane = ST_TRUE;
                } break;

                default:
                {
                    // DNE
                } break;

            }

        }

/*
    Per-Plane, reevaluate_the_current_plane All Landmasses
*/
        if(reevaluate_the_current_plane == ST_TRUE)
        {

            for(itr_landmasses = 0; itr_landmasses < NUM_LANDMASSES; itr_landmasses++)
            {
                final_landmass_weights[itr_landmasses] = 0;
                Continent_Values[itr_landmasses] = 0;
            }

/*
    BEGIN:  Center of Empire?  (geographic centroid)
*/
            
            empire_wx = 0;
            empire_wy = 0;
            own_city_count = 0;

            for(itr_cities = 0; itr_cities < _cities; itr_cities++)
            {
                if(_CITIES[itr_cities].wp == wp)
                {
                    city_owner_idx = _CITIES[itr_cities].owner_idx;
                    if(city_owner_idx == player_idx)
                    {
                        // OWN city: accumulate centroid coordinates
                        empire_wx += _CITIES[itr_cities].wx;
                        empire_wy += _CITIES[itr_cities].wy;
                        own_city_count++;
                    }
                    else
                    {
                        if(first_hostile_player_idx <= ST_UNDEFINED)
                        {
                            // AT PEACE: count only neutrals, weight = 1
                            if(city_owner_idx == NEUTRAL_PLAYER_IDX)
                            {
                                Continent_Values[_landmasses[((wp * WORLD_SIZE) + (_CITIES[itr_cities].wy * WORLD_WIDTH) + _CITIES[itr_cities].wx)]] += 1;
                            }
                        }
                        else
                        {
                            // AT WAR: 
                            if(city_owner_idx == first_hostile_player_idx)
                            {
                                // enemy city weight = 10
                                Continent_Values[_landmasses[((wp * WORLD_SIZE) + (_CITIES[itr_cities].wy * WORLD_WIDTH) + _CITIES[itr_cities].wx)]] += 10;
                            }
                            else
                            {
                                if(city_owner_idx == NEUTRAL_PLAYER_IDX)
                                {
                                    // neutral city weight = 1 even in wartime
                                    Continent_Values[_landmasses[((wp * WORLD_SIZE) + (_CITIES[itr_cities].wy * WORLD_WIDTH) + _CITIES[itr_cities].wx)]] += 1;
                                }
                            }
                            // Other (non-enemy, non-neutral, non-self) players' cities: NOT added (weight 0)
                        }
                    }
                }
            }  /* for(itr_cities = 0; itr_cities < _cities; itr_cities++) */

            if(own_city_count == 0)
            {
                empire_wx = _FORTRESSES[player_idx].wx;
                empire_wy = _FORTRESSES[player_idx].wy;
            }
            else
            {
                empire_wx /= own_city_count;
                empire_wy /= own_city_count;
            }
/*
    END:  Center of Empire?  (geographic centroid)
*/


            for(itr_landmasses = 1; itr_landmasses < NUM_LANDMASSES; itr_landmasses++)
            {
                landmass_node_index = _ai_landmass_dock_squares_heads[wp][itr_landmasses];
                min_delta_distance = 1000;
                while(landmass_node_index != ST_UNDEFINED)
                {
                    dock_square_wx = _ai_landmass_dock_squares_wx_array[wp][Value_Sum];
                    dock_square_wy = _ai_landmass_dock_squares_wy_array[wp][Value_Sum];
                    if(g_ai_evaluation_map[wp][((dock_square_wy * WORLD_WIDTH) + dock_square_wx)] == 0)
                    {
                        delta_distance = Delta_XY_With_Wrap(dock_square_wx, dock_square_wy, empire_wx, empire_wy, WORLD_WIDTH);
                    }
                    /* OGBUG  this nearest test should be inside the occupieable test block */
                    if(delta_distance < min_delta_distance)
                    {
                        min_delta_distance = delta_distance;
                    }
                    landmass_node_index = _ai_landmass_dock_squares_lists[wp][landmass_node_index];
                }
                /* to avoid divide by zero */
                if(min_delta_distance == 0) { min_delta_distance = 1; }

                if(first_hostile_player_idx <= ST_UNDEFINED)
                {
                    /* ¿ weighted by distance ? */
                    final_landmass_weights[itr_landmasses] = (
                        (Continent_Values[itr_landmasses] * 1000)
                        /
                        min_delta_distance
                    );
                    if(
                        (Continent_Values[itr_landmasses] > 0)
                        &&
                        (final_landmass_weights[itr_landmasses] < 10)
                    )
                    {
                        final_landmass_weights[itr_landmasses] = 10;
                    }
                }
                else  /* (first_hostile_player_idx > ST_UNDEFINED) */
                {
                    if(Continent_Values[itr_landmasses] > 0)
                    {
                        /* DEDU  Inverse something? Should direct choice towards first hostile player... */
                        final_landmass_weights[itr_landmasses] = (
                            (2000 - (Continent_Values[itr_landmasses] * 10))
                            /
                            min_delta_distance
                        );
                        /* DEDU  Is this >=10 creating a separation with the map square count weight below? */
                        if(final_landmass_weights[itr_landmasses] < 10)
                        {
                            final_landmass_weights[itr_landmasses] = 10;
                        }
                    }
                    /* HUGE bonus if it's the home of the dude we really want to destroy */
                    if(
                        (_players[player_idx].Hostility[first_hostile_player_idx] == 4)  /* "Jihad" / MAX_HOSTILITY */
                        &&
                        (_FORTRESSES[first_hostile_player_idx].wp == wp)
                    )
                    {
                        if(_landmasses[((wp * WORLD_SIZE) + (_FORTRESSES[first_hostile_player_idx].wy * WORLD_WIDTH) + _FORTRESSES[first_hostile_player_idx].wx)] == itr_landmasses)
                        {
                            final_landmass_weights[itr_landmasses] += 1000;
                        }
                    }
                }
            }

/*
    First Double-Check
*/
            landmass_weight_check = 0;
            for(itr_landmasses = 0; itr_landmasses < NUM_LANDMASSES; itr_landmasses++)
            {
                landmass_weight_check += final_landmass_weights[itr_landmasses];
            }
            /*
                not own or empty landmass
                player's fortress is inactive or it's the player's home continent
                ...
                change landmass weight value to be count of map squares
            */
            if(landmass_weight_check == 0)
            {
                for(itr_landmasses = 0; itr_landmasses < NUM_LANDMASSES; itr_landmasses++)
                {
                    if(
                        (_ai_continents.plane[wp].player[player_idx].type_array[itr_landmasses] != lmt_Own)
                        &&
                        (_ai_continents.plane[wp].player[player_idx].type_array[itr_landmasses] != lmt_NoTargets)
                    )
                    {
                        if(
                            (_FORTRESSES[player_idx].active == ST_FALSE)
                            ||
                            /* OGBUG  should check fortress plane */
                            (itr_landmasses == _landmasses[((wp * WORLD_SIZE) + (_FORTRESSES[player_idx].wy * WORLD_WIDTH) + _FORTRESSES[player_idx].wx)])
                        )
                        {
                            landmass_node_index = _ai_landmass_land_squares_heads[wp][itr_landmasses];
                            while(landmass_node_index != ST_UNDEFINED)
                            {
                                final_landmass_weights[itr_landmasses] += 1;
                                landmass_node_index = _ai_landmass_land_squares_lists[wp][landmass_node_index];
                            }
                            if (final_landmass_weights[itr_landmasses] < 10)
                            {
                                final_landmass_weights[itr_landmasses] = 0;
                            }
                        }
                    }
                }
            }

/*
    Second Double-Check
*/
            landmass_weight_check = 0;
            for(itr_landmasses = 0; itr_landmasses < NUM_LANDMASSES; itr_landmasses++)
            {
                landmass_weight_check += final_landmass_weights[itr_landmasses];
            }
            if(landmass_weight_check == 0)
            {
                _ai_landmass_war_targets[wp][player_idx] = 0;
            }
            else
            {
                _ai_landmass_war_targets[wp][player_idx] = Get_Weighted_Choice(&final_landmass_weights[0], NUM_LANDMASSES);
            }

        }  /* if(reevaluate_the_current_plane == ST_TRUE) */

    }  /* for(wp = 0; wp < NUM_PLANES; wp++) */

}


// WZD o162p38
/*
DEDU  doesn't include Transports and Settlers because they have their ordering functions?
*/
void Player_Clear_All_Unit_Orders(int16_t player_idx)
{
    int16_t itr = 0;
    if(player_idx == HUMAN_PLAYER_IDX)
    {
        return;
    }
    for(itr = 0; itr < _units; itr++)
    {
        if(
            (_UNITS[itr].owner_idx == player_idx)
            &&
            (_unit_type_table[_UNITS[itr].type].Construction == 0)
        )
        {
            _UNITS[itr].Status = us_Ready;
        }
    }
}
