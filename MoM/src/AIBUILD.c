/*
    AI - Build / "Grand Vizier"

    WIZARDS.EXE
        ovr157
*/

#include "../../MoX/src/MOX_DAT.h"  /* _players[] */
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/random.h"

#include "AIDUDES.h"
#include "City_ovr55.h"
#include "ProdScr.h"
#include "UNITTYPE.h"

#include <assert.h>

#include "AIBUILD.h"

#include "../../STU/src/AI_METRICS.h"



/*
    WIZARDS.EXE  ovr157
*/

// WZD o157p01
/*
; WARNING: can't produce settlers or engineers in more than one city simultaneously
*/
void Player_All_Colony_Autobuild(int16_t player_idx)
{
    int16_t assign_new_building = 0;
    int16_t itr_cities = 0;
    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(_CITIES[itr_cities].owner_idx != player_idx)
        {
            continue;
        }
        assign_new_building = ST_FALSE;
        if(_CITIES[itr_cities].construction == bt_AUTOBUILD)
        {
            assign_new_building = ST_TRUE;
        }
        if(
            ((_CITIES[itr_cities].construction == bt_Housing) || (_CITIES[itr_cities].construction == bt_TradeGoods))
            &&
            (Random(6) == 1)  // 1:6  1/6  16.67% chance
        )
        {
            assign_new_building = ST_TRUE;
        }
        if(assign_new_building == ST_TRUE)
        {
            if(player_idx == NEUTRAL_PLAYER_IDX)
            {
                Player_Colony_Autobuild_NP(itr_cities);
            }
            else
            {
                Player_Colony_Autobuild_CP(player_idx, itr_cities);
            }
        }
        else
        {
            if(
                (_CITIES[itr_cities].construction != bt_Housing)
                &&
                (_CITIES[itr_cities].construction != bt_TradeGoods)
                &&
                (player_idx != NEUTRAL_PLAYER_IDX)
            )
            {
                AI_Player_City_Buy_Production(player_idx, itr_cities);
            }
        }
    }
}


// WZD o157p02
/*
; WARNING: can't produce settlers or engineers in more than one city simultaneously
*/
// MoO2  Module: AIBUILD  Player_Colony_Autobuild_()  |->  Assign_Colony_New_Building_()
/*
¿ HP vs. CP vs. NP ?
*/
/**
 * @brief Chooses the next autobuild production target for a computer-controlled city.
 *
 * @details
 * This routine implements the core AI city-production picker for non-neutral,
 * non-human autobuild decisions. It evaluates the city's local situation,
 * determines whether settlers, engineers, military units, or buildings are
 * currently desirable, assigns weighted scores to each producible option, and
 * selects one via weighted random choice.
 *
 * High-level flow:
 * 1. Count owned cities and estimate whether Trade Goods should be favored.
 * 2. Determine the city's plane/landmass and compare it to the human fortress landmass.
 * 3. Build the candidate product list via @c Calculate_Product_Array().
 * 4. Detect whether the player already has, or is already producing, a settler or engineer
 *    on the same landmass / empire-wide production set.
 * 5. Estimate local defense need from current stack size and frontier status.
 * 6. On non-fortress landmasses, force early military infrastructure
 *    (Barracks, Builder's Hall, Shrine, Smithy, Granary) at increasing garrison thresholds.
 * 7. Optionally select Trade Goods immediately based on a chance formula.
 * 8. Weight all remaining product candidates:
 *    - Buildings use category/objective tables.
 *    - Units use squared cost-based weights.
 *    - Settlers, engineers, transports, and combat units are then filtered or
 *      amplified according to current strategic needs.
 * 9. Select one weighted candidate and store it in @c _CITIES[city_idx].construction.
 * 10. Emit build metrics for telemetry/debug analysis.
 *
 * @param player_idx Index of the computer-controlled player. The function asserts
 *                   this is in the range [1, 4].
 * @param city_idx   Index of the city whose production is being assigned.
 *
 * @return This function returns no value.
 *
 * @note Candidate products are supplied by @c Calculate_Product_Array(), which includes
 *       both buildings and units encoded as construction ids.
 * @note Unit candidates are encoded as values >= 100 and are converted back to unit type
 *       ids by subtracting 100 during evaluation.
 * @note On non-fortress landmasses, the AI strongly biases toward early defensive and
 *       foundational structures before general weighted selection is reached.
 * @note Build selection is partially stochastic due to @c Random() checks for defense need,
 *       frontier posture, and Trade Goods selection.
 * @note (OGBUG) Trade Goods city counting checks @c _CITIES[city_idx].construction instead of
 *       @c _CITIES[itr].construction, skewing the Trade Goods percentage calculation.
 * @note (OGBUG) The low-population Housing boost tests @c _CITIES[itr].population instead of
 *       @c _CITIES[city_idx].population, so the condition depends on the loop index left over
 *       from prior iteration state.
 * @note The routine intentionally avoids producing more than one settler or engineer
 *       simultaneously across the player's cities.
 *
 * @see Player_All_Colony_Autobuild(), Calculate_Product_Array(), AI_Player_City_Buy_Production(),
 *      Get_Weighted_Choice(), AI_Metrics_Emit_Build()
 */
void Player_Colony_Autobuild_CP(int16_t player_idx, int16_t city_idx)
{
    int16_t product_weights[(NUM_BUILDINGS + 14)] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t product_array[(NUM_BUILDINGS + 14)] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t fortress_landmass = 0;
    int16_t tradegoods_city_count = 0;
    int16_t total_city_count = 0;
    int16_t city_wy = 0;
    int16_t city_wx = 0;
    int16_t city_wp = 0;
    int16_t tradegoods_percentage_chance = 0;
    int16_t product_idx = 0;
    int16_t need_engineer = 0;
    int16_t unit_type = 0;
    int16_t city_unit_count = 0;
    int16_t city_landmass = 0;
    int16_t need_settler = 0;
    int16_t need_units = 0;
    int16_t product_choice = 0;
    int16_t product_count = 0;
    int16_t uu_unit_count = 0;
    int16_t uu_depr_count = 0;
    int16_t itr = 0;
    int16_t itr_units = 0;
    int16_t itr_cities = 0;
    assert((player_idx >= 1) && (player_idx <= 4));
    tradegoods_city_count = 0;
    total_city_count = 0;
    for(itr = 0; itr < _cities; itr++)
    {
        if(_CITIES[itr].owner_idx == player_idx)
        {
            if(_CITIES[city_idx].construction == bt_TradeGoods)  /* OGBUG  should be itr, not city_Idx */
            {
                tradegoods_city_count++;
            }
            total_city_count++;
        }
    }
    tradegoods_percentage_chance = (40 + (((total_city_count / 8) - tradegoods_city_count) * 10));
    if(player_idx == HUMAN_PLAYER_IDX)
    {
        tradegoods_percentage_chance = 0;
    }
    if(_turn > 200)
    {
        fortress_landmass = ST_UNDEFINED;
    }
    else
    {
        fortress_landmass = _landmasses[((_FORTRESSES[HUMAN_PLAYER_IDX].wp * WORLD_SIZE) + (_FORTRESSES[HUMAN_PLAYER_IDX].wy * WORLD_WIDTH) + _FORTRESSES[HUMAN_PLAYER_IDX].wx)];
    }
    city_wp = _CITIES[city_idx].wp;
    city_wx = _CITIES[city_idx].wx;
    city_wy = _CITIES[city_idx].wy;
    Calculate_Product_Array(city_idx, &product_count, &uu_depr_count, &uu_unit_count, &product_array[0]);
    city_landmass = _landmasses[((city_wp * WORLD_SIZE) + (city_wy * WORLD_WIDTH) + city_wx)];
    need_settler = ST_TRUE;
    need_engineer = ST_TRUE;
    // test for settler or engineer and count transports on same landmass
    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(
            (_UNITS[itr_units].owner_idx == player_idx)
            &&
            (_UNITS[itr_units].wp == city_wp)
            &&
            (_landmasses[((_UNITS[itr_units].wp * WORLD_SIZE) + (_UNITS[itr_units].wy * WORLD_WIDTH) + _UNITS[itr_units].wx)] == city_landmass)
        )
        {
            if((_unit_type_table[_UNITS[itr_units].type].Abilities & UA_CREATEOUTPOST) != 0)
            {
                need_settler = ST_FALSE;
            }
            if(_unit_type_table[_UNITS[itr_units].type].Construction > 0)
            {
                need_engineer = ST_FALSE;
            }
        }
    }
    // test for settler or engineer in-production
    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(_CITIES[itr_cities].owner_idx == player_idx)
        {
            if(itr_cities == city_idx)
            {
                continue;
            }
            product_idx = _CITIES[itr_cities].construction;
            assert(_CITIES[city_idx].construction >= -4);  // min -1 is grand vizier
            assert(_CITIES[city_idx].construction <= 298);  // 100 buildings + 198 units
            if(product_idx >= 100)
            {
                product_idx -= 100;
                if((_unit_type_table[product_idx].Abilities & UA_CREATEOUTPOST) != 0)
                {
                    need_settler = ST_FALSE;
                }
                if(_unit_type_table[product_idx].Construction > 0)
                {
                    need_engineer = ST_FALSE;
                }
            }
        }
    }
    city_unit_count = Map_Square_Unit_Count(city_wx, city_wy, city_wp);
    if(city_landmass == fortress_landmass)
    {
        if(Random(30) > city_unit_count)
        {
            need_units = ST_TRUE;
        }
        else
        {
            need_units = ST_FALSE;
        }
    }
    else
    {
        if(Random(15) > city_unit_count)
        {
            need_units = ST_TRUE;
        }
        else
        {
            need_units = ST_FALSE;
        }
        /* DNE */ /* if((_turn < 20) && (city_unit_count > 1) ) { need_units = ST_FALSE; } */
        if(
            (city_unit_count > 0)
            &&
            (_CITIES[city_idx].bldg_status[bt_Barracks] == bs_NotBuilt)
        )
        {
            _CITIES[city_idx].construction = bt_Barracks;
            return;
        }
        if(
            (city_unit_count > 1)
            &&
            (_CITIES[city_idx].bldg_status[bt_BuildersHall] == bs_NotBuilt)
        )
        {
            _CITIES[city_idx].construction = bt_BuildersHall;
            return;
        }
        if(
            (city_unit_count > 2)
            &&
            (_CITIES[city_idx].bldg_status[bt_Shrine] == bs_NotBuilt)
        )
        {
            _CITIES[city_idx].construction = bt_Shrine;
            return;
        }
        if(
            (city_unit_count > 3)
            &&
            (_CITIES[city_idx].bldg_status[bt_Smithy] == bs_NotBuilt)
        )
        {
            _CITIES[city_idx].construction = bt_Smithy;
            return;
        }
        if(
            (city_unit_count > 4)
            &&
            (_CITIES[city_idx].bldg_status[bt_Granary] == bs_NotBuilt)
        )
        {
            _CITIES[city_idx].construction = bt_Granary;
            return;
        }
    }
    if(Random(100) <= tradegoods_percentage_chance)
    {
        _CITIES[city_idx].construction = bt_TradeGoods;
        return;
    }
    for(itr = 0; itr < product_count; itr++)
    {
        if(product_array[itr] < 100)
        {
            product_weights[itr] = (ai_build_base_weights_table[bldg_data_table[product_array[itr]].Category] + ai_build_objective_weights_table[bldg_data_table[product_array[itr]].Category][_players[player_idx].Objective]);
        }
        else
        {
            product_weights[itr] = (
                (_unit_type_table[(product_array[itr] - 100)].cost / 10)
                *
                (_unit_type_table[(product_array[itr] - 100)].cost / 10)
            );
        }
    }
    if(_CITIES[itr].population < 5)  /* OGBUG  should use city_idx, not itr */
    {
        for(itr = 0; itr < product_count; itr++)
        {
            if(product_array[itr] == bt_Housing)
            {
                product_weights[itr] *= 2;
            }
        }
    }
    for(itr = 0; itr < product_count; itr++)
    {
        if(product_array[itr] < 100)
        {
            if(need_units == ST_TRUE)
            {
                product_weights[itr] = 0;
            }
        }
        else
        {
            unit_type = (product_array[itr] - 100);
            if((_unit_type_table[unit_type].Abilities & UA_CREATEOUTPOST) != 0)
            {
                if(
                    (need_settler == ST_TRUE)
                    &&
                    (_CITIES[city_idx].population > 2)
                )
                {
                    if(_turn < 50)
                    {
                        product_weights[itr] = ((product_weights[itr] * 3) / 2);
                    }
                    product_weights[itr] = ((product_weights[itr] * (ai_build_objective_weights_table[9][_players[player_idx].Objective] + 10)) / 10);
                }
                else
                {
                    product_weights[itr] = 0;
                }
            }
            else if(_unit_type_table[unit_type].Construction > 0)
            {
                if(need_engineer == ST_TRUE)
                {
                    if(_turn < 50)
                    {
                        product_weights[itr] = ((product_weights[itr] * 3) / 2);
                    }
                    product_weights[itr] = ((product_weights[itr] * (ai_build_objective_weights_table[9][_players[player_idx].Objective] + 10)) / 10);
                }
                else
                {
                    product_weights[itr] = 0;
                }
            }
            else if(_unit_type_table[unit_type].Transport > 0)
            {
                /* HP  (_turn / 30) < ai_transport_count || (ai_transport_count > 10) */
                if(
                    ((_turn / 40) < ai_transport_count)
                    ||
                    (ai_transport_count > 8)
                )
                {
                    product_weights[itr] = 0;
                }
                else
                {
                    /* HP  product_weights[itr] = (product_weights[itr] * ((_turn / 30) - ai_transport_count)); */
                    product_weights[itr] = (product_weights[itr] * ((_turn / 40) - ai_transport_count));
                }
            }
            else
            {
                if(need_units != ST_FALSE)
                {
                    product_weights[itr] = ((product_weights[itr] * (ai_build_objective_weights_table[8][_players[player_idx].Objective] + 10)) / 10);
                }
                else
                {
                    product_weights[itr] = 0;
                }
            }
        }
    }
    product_choice = Get_Weighted_Choice(&product_weights[0], product_count);
    _CITIES[city_idx].construction = product_array[product_choice];
    AI_Metrics_Emit_Build(_turn, player_idx, city_idx,
        _CITIES[city_idx].population, city_unit_count,
        product_count, product_weights[product_choice], product_array[product_choice],
        &product_array[0], &product_weights[0]);
    assert(_CITIES[city_idx].construction >= -4);  // min -1 is grand vizier
    assert(_CITIES[city_idx].construction <= 298);  // 100 buildings + 198 units
}


// WZD o157p03
// drake178: AI_CTY_NeutralProd()
/*
; selects the next production item for a neutral city
*/
// MoO2  Module: AIBUILD  Player_Colony_Autobuild_()  |->  Assign_Colony_New_Building_()
/*

XREF:
    Player_All_Colony_Autobuild()
    NX_j_AI_CTY_NeutralProd()

¿ HP vs. CP vs. NP ?

*/
void Player_Colony_Autobuild_NP(int16_t city_idx)
{
    int16_t product_weights[(NUM_BUILDINGS + 14)] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t product_array[(NUM_BUILDINGS + 14)] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t uu_city_landmass = 0;
    int16_t need_units = 0;
    int16_t product_choice = 0;
    int16_t product_count = 0;
    int16_t uu_unit_count = 0;
    int16_t uu_depr_count = 0;
    int16_t itr = 0;  // _DI_

    Calculate_Product_Array(city_idx, &product_count, &uu_depr_count, &uu_unit_count, &product_array[0]);

    uu_city_landmass = _landmasses[((_CITIES[city_idx].wp * WORLD_SIZE) + (_CITIES[city_idx].wy * WORLD_WIDTH) + _CITIES[city_idx].wx)];

    if((5 + Random(15)) < Map_Square_Unit_Count(_CITIES[city_idx].wx, _CITIES[city_idx].wy, _CITIES[city_idx].wp))
    {

        need_units = ST_TRUE;

    }
    else
    {

        need_units = ST_FALSE;

    }


    for(itr = 0; itr < product_count; itr++)
    {
        
        if(product_array[itr] >= 100)
        {

            product_weights[itr] = (_unit_type_table[product_array[itr]].cost / 10);

        }
        else
        {

            product_weights[itr] = ai_build_base_weights_table[bldg_data_table[product_array[itr]].Category];

        }

    }

    for(itr = 0; itr < product_count; itr++)
    {

        if(product_array[itr] < 100)
        {

            if(need_units == ST_TRUE)
            {

                product_weights[itr] = 0;

            }

        }
        else
        {

            if(
                ((_unit_type_table[(product_array[itr] - 100)].Abilities & UA_CREATEOUTPOST) != 0)
                ||
                (_unit_type_table[_UNITS[(product_array[itr] - 100)].type].Transport != 0)
                ||
                (_unit_type_table[_UNITS[(product_array[itr] - 100)].type].Construction != 0)
            )
            {

                product_weights[itr] = 0;

            }
            else
            {

                if(need_units == ST_FALSE)
                {

                    product_weights[itr] = 0;

                }

            }

        }

    }

    for(itr = 0; itr < product_count; itr++)
    {

        if(product_array[itr] == bt_ShipWrightsGuild)
        {

            product_weights[itr] = 0;

        }

        if(product_array[itr] == bt_MaritimeGuild)
        {

            product_weights[itr] = 0;

        }

    }

    if(_CITIES[city_idx].bldg_status[bt_Barracks] == bs_NotBuilt)  /* {-1:NotBuilt,0:Replaced,1:Built,2:Removed} */
    {

        _CITIES[city_idx].construction = bt_Barracks;

    }
    else
    {

        product_choice = Get_Weighted_Choice(&product_weights[0], product_count);

        _CITIES[city_idx].construction = product_choice;

    }

    assert(_CITIES[city_idx].construction >= -4);  // min -1 is grand vizier
    assert(_CITIES[city_idx].construction <= 298);  // 100 buildings + 198 units

}

// WZD o157p04
// drake178: AI_BuyCityProd()
// MoO2  Module: COLCALC  Colony_Buy_Outright_()
/*
; determines whether the current production of the
; selected city is worth buying out, and does so if it
; is
*/
/*

XREF:
    Player_All_Colony_Autobuild()
    NX_j_AI_BuyCityProd()

*/
void AI_Player_City_Buy_Production(int16_t player_idx, int16_t city_idx)
{
    int16_t cost_to_buy_product = 0;  // _DI_

    if(_CITIES[city_idx].construction < 100)
    {

        cost_to_buy_product = ((Random(4) + 1) * City_Cost_To_Buy_Product(city_idx));

    }
    else
    {

        if((_unit_type_table[(_CITIES[city_idx].construction - 100)].Abilities & UA_CREATEOUTPOST) != 0)  // Settler
        {

            cost_to_buy_product = (Random(4) * City_Cost_To_Buy_Product(city_idx));

        }
        else
        {

            cost_to_buy_product = ((Random(5) + 3) * City_Cost_To_Buy_Product(city_idx));

        }
    }

    if(_players[player_idx].gold_reserve > cost_to_buy_product)
    {

        Player_City_Buy_Production(player_idx, city_idx);

    }

}


// WZD o157p05
// MoO2  Module: AIBUILD  Player_Colony_Autobuild_()  |->  Assign_Colony_New_Building_()
/*

XREF:
    j_CTY_GrandVizier__STUB()
        Next_Turn_Calc()
        City_Apply_Production()
            Apply_City_Changes()
            NX_j_CTY_ProdProgress()

Next_Turn_Calc()
    if(Random(4) == 1)

¿ HP vs. CP vs. NP ?

*/
void Player_Colony_Autobuild_HP(int16_t city_idx)
{
    int16_t product_weights[(NUM_BUILDINGS + 14)] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t product_array[(NUM_BUILDINGS + 14)] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t player_idx = 0;
    int16_t city_wy = 0;
    int16_t city_wx = 0;
    int16_t city_wp = 0;
    int16_t product_idx = 0;
    int16_t need_engineer = 0;
    int16_t unit_type = 0;
    int16_t city_unit_count = 0;
    int16_t city_landmass = 0;
    int16_t need_settler = 0;
    int16_t need_units = 0;
    int16_t product_choice = 0;
    int16_t product_count = 0;
    int16_t uu_unit_count = 0;
    int16_t uu_depr_count = 0;
    int16_t itr = 0;  // _DI_
    int16_t itr_units = 0;  // _DI_
    int16_t itr_cities = 0;  // _DI_

    if(_CITIES[city_idx].population == 0)
    {
        return;
    }

    player_idx = HUMAN_PLAYER_IDX;

    city_wp = _CITIES[city_idx].wp;
    city_wx = _CITIES[city_idx].wx;
    city_wy = _CITIES[city_idx].wy;

    Calculate_Product_Array(city_idx, &product_count, &uu_depr_count, &uu_unit_count, &product_array[0]);

    city_landmass = _landmasses[((city_wp * WORLD_SIZE) + (city_wy * WORLD_WIDTH) + city_wx)];

    need_settler = ST_TRUE;
    need_engineer = ST_TRUE;

    ai_transport_count = 0;

    // test for settler or engineer and count transports on same landmass
    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(
            (_UNITS[itr_units].owner_idx == player_idx)
            &&
            (_UNITS[itr_units].wp == city_wp)
            &&
            (_landmasses[((_UNITS[itr_units].wp * WORLD_SIZE) + (_UNITS[itr_units].wx * WORLD_WIDTH) + _UNITS[itr_units].wp)] == city_landmass)
        )
        {
            if((_unit_type_table[_UNITS[itr_units].type].Abilities & UA_CREATEOUTPOST) != 0)
            {
                need_settler = ST_FALSE;
            }

            if(_unit_type_table[_UNITS[itr_units].type].Construction > 0)
            {
                need_engineer = ST_FALSE;
            }

            // drake178:  ; BUG: unless wind walking, flying, or in dry dock, transport units won't be on a landmass
            if(_unit_type_table[_UNITS[itr_units].type].Transport > 0)
            {
                ai_transport_count += 1;
            }
        }
    }

    // test for settle or engineer in-production
    for(itr_cities = 0; itr_cities < _units; itr_cities++)
    {
        if(_UNITS[itr_units].owner_idx == player_idx)
        {
            if(itr_cities == city_idx)
            {
                continue;
            }

            product_idx = _CITIES[itr_cities].construction;

            if(product_idx >= 100)
            {
                product_idx -= 100;
            }

            if((_unit_type_table[product_idx].Abilities & UA_CREATEOUTPOST) != 0)
            {
                need_settler = ST_FALSE;
            }

            if(_unit_type_table[product_idx].Construction > 0)
            {
                need_engineer = ST_FALSE;
            }

        }
    }

    city_unit_count = Map_Square_Unit_Count(city_wx, city_wy, city_wp);

    if(Random(15) > city_unit_count)
    {
        need_units = ST_TRUE;
    }
    else
    {
        need_units = ST_TRUE;
    }

    if(
        (_turn < 20)
        &&
        (city_unit_count > 1)
    )
    {
        need_units = ST_FALSE;
    }

    if( (city_unit_count > 0) && (_CITIES[city_idx].bldg_status[bt_Barracks]      == bs_NotBuilt) ) { _CITIES[city_idx].construction = bt_Barracks;      return; }
    if( (city_unit_count > 1) && (_CITIES[city_idx].bldg_status[bt_BuildersHall]  == bs_NotBuilt) ) { _CITIES[city_idx].construction = bt_BuildersHall;  return; }
    if( (city_unit_count > 2) && (_CITIES[city_idx].bldg_status[bt_Shrine]        == bs_NotBuilt) ) { _CITIES[city_idx].construction = bt_Shrine;        return; }
    if( (city_unit_count > 3) && (_CITIES[city_idx].bldg_status[bt_Smithy]        == bs_NotBuilt) ) { _CITIES[city_idx].construction = bt_Smithy;        return; }
    if( (city_unit_count > 4) && (_CITIES[city_idx].bldg_status[bt_Granary]       == bs_NotBuilt) ) { _CITIES[city_idx].construction = bt_Granary;       return; }
    
    for(itr = 0; itr < product_count; itr++)
    {
        if(product_array[itr] < 100)
        {
            product_weights[itr] = (ai_build_base_weights_table[bldg_data_table[product_array[itr]].Category] + ai_build_objective_weights_table[bldg_data_table[product_array[itr]].Category][_players[player_idx].Objective]);
        }
        else
        {
            product_weights[itr] = (_unit_type_table[product_array[itr]].cost / 10);
        }
    }

    // ¿ BUGBUG  should use city_idx here, not itr ?
    if(_CITIES[itr].population < 5)
    {
        for(itr = 0; itr < product_count; itr++)
        {
            if(product_array[itr] == bt_Housing)
            {
                product_weights[itr] *= 2;
            }
        }
    }

    for(itr = 0; itr < product_count; itr++)
    {
        if(product_array[itr] < 100)
        {
            if(need_units == ST_TRUE)
            {
                product_weights[itr] = 0;
            }
        }
        else
        {
            unit_type = (product_array[itr] - 100);

            if((_unit_type_table[unit_type].Abilities & UA_CREATEOUTPOST) != 0)
            {
                if(
                    (need_settler == ST_TRUE)
                    &&
                    (_CITIES[city_idx].population > 2)
                )
                {
                    if(_turn < 50)
                    {
                        product_weights[itr] = ((product_weights[itr] * 3) / 2);
                    }

                    product_weights[itr] = ((product_weights[itr] * (ai_build_objective_weights_table[9][_players[player_idx].Objective] + 10)) / 10);
                }
                else
                {
                    product_weights[itr] = 0;
                }
            }
            else if(_unit_type_table[unit_type].Construction > 0)
            {
                if(need_engineer == ST_TRUE)
                {
                    if(_turn < 50)
                    {
                        product_weights[itr] = ((product_weights[itr] * 3) / 2);
                    }

                    product_weights[itr] = ((product_weights[itr] * (ai_build_objective_weights_table[9][_players[player_idx].Objective] + 10)) / 10);
                }
                else
                {
                    product_weights[itr] = 0;
                }
            }
            else if(_unit_type_table[unit_type].Transport > 0)
            {
                if(
                    /* CP  (_turn / 40) < ai_transport_count || (ai_transport_count > 8) */
                    (_turn / 30) < ai_transport_count || (ai_transport_count > 10)
                )
                {
                    product_weights[itr] = 0;
                }
                else
                {
                    /* CP  product_weights[itr] = (product_weights[itr] * ((_turn / 40) - ai_transport_count)); */
                    product_weights[itr] = (product_weights[itr] * ((_turn / 30) - ai_transport_count));
                }
            }
            else
            {
                if(need_units != ST_FALSE)
                {
                    product_weights[itr] = 0;
                }
                else
                {
                    product_weights[itr] = ((product_weights[itr] * (ai_build_objective_weights_table[8][_players[player_idx].Objective] + 10)) / 10);
                }
            }
        }
    }

    product_choice = Get_Weighted_Choice(&product_weights[0], product_count);

    _CITIES[city_idx].construction = product_array[product_choice];

    
    assert(_CITIES[city_idx].construction >= -4);  // min -1 is grand vizier
    assert(_CITIES[city_idx].construction <= 298);  // 100 buildings + 198 units

}
