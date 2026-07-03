/*
    WIZARDS.EXE
        ovr161

    Terrain
    Terrain Types
    Terrain Type Attributes
*/

#include "../../STU/src/STU_LOG.h"

#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOX_BITS.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_TYPE.h"
#include "../../MoX/src/random.h"
#include "../../MoX/src/Util.h"

#include "CITYCALC.h"
#include "TerrType.h"
#include "Terrain.h"



// WZD s161p01
/*
    ~ "Purify"
*/
void Square_Clear_Corruption(int16_t wx, int16_t wy, int16_t wp)
{
    _map_square_flags[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] &= ~MSF_CORRUPTION;
}


// WZD s161p02
int16_t Square_Is_Node(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t niu_variable = 0;
    int16_t terrain_type = 0;
    int16_t is_node = 0;  // DNE on Dasm
    is_node = ST_FALSE;  // DNE on Dasm
    niu_variable = 0;
    terrain_type = (p_world_map[wp][wy][wx] % NUM_TERRAIN_TYPES);
    if(
        (terrain_type != tt_SorceryNode )
        &&
        (terrain_type != tt_SorceryNode )
        &&
        (terrain_type != tt_SorceryNode )
    )
    {
        is_node = ST_FALSE;
    }
    else
    {
        is_node = ST_TRUE;
    }
    return is_node;
}


// WZD s161p03
/**
 * @brief Computes terrain food yield in doubled-food units for one map square.
 *
 * @details
 * This routine classifies the square's base terrain type and assigns a food
 * value using a mix of range checks and explicit per-terrain switch cases.
 * The computed value is then scaled to doubled units and optionally halved when
 * the square is shared by multiple city areas.
 *
 * Processing summary:
 * 1. Decode terrain via @c p_world_map[wp][wy][wx] % NUM_TERRAIN_TYPES.
 * 2. Early-return 0 for ocean/animated-ocean categories.
 * 3. Assign base food from threshold buckets and explicit terrain cases.
 * 4. Multiply by 2 to convert to the function's x2 food unit space.
 * 5. If @c City_Area_Square_Is_Shared() is true, halve the result.
 *
 * @param wx World X coordinate of the evaluated square.
 * @param wy World Y coordinate of the evaluated square.
 * @param wp World plane containing the evaluated square.
 *
 * @return Food yield in doubled-food units for the square.
 *
 * @note Return values are in x2 food units, matching call sites that aggregate
 *       fractional food using doubled arithmetic.
 * @note Several terrain classes use threshold ranges instead of only symbolic
 *       enum equality checks, preserving original binary logic.
 * @note (OGBUG) Swamp tiles return 0, despite manual expectations of 1/2 food.
 * @note (OGBUG) Nature Node returns 5 base food here, while the referenced MGC
 *       NewGame behavior notes 4.
 * @note The historical comment questioning @c food_units *= 2 is retained as-is;
 *       this documentation describes current behavior only.
 *
 * @see City_Area_Square_Is_Shared(), City_Food_Terrain(), City_Maximum_Size()
 */
int16_t Square_Food_x2(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type;
    int16_t food_units;
    terrain_type = (p_world_map[wp][wy][wx] % NUM_TERRAIN_TYPES);
    if(terrain_type == tt_Ocean)
    {
        return 0;
    }
    if(terrain_type >= tt_OceanAnim)
    {
        return 0;
    }
    if(terrain_type == tt_BugGrass)
    {
        food_units = 3;
    }
    else if(terrain_type < tt_Grasslands1)
    {
        food_units = 1;
    }
    else if(terrain_type > _River1111_5)
    {
        food_units = 1;
    }
    else if(terrain_type > _Shore111R1110)
    {
        food_units = 4;
    }
    else if(terrain_type > _Desert10101111)
    {
        food_units = 1;
    }
    else if(terrain_type > _1Hills2)
    {
        return 0;
    }
    else if(terrain_type > _Mount1001)
    {
        food_units = 1;
    }
    else if(terrain_type > tt_Rivers_end)
    {
        return 0;
    }
    else if(terrain_type > tt_Forest3)
    {
        food_units = 4;
    }
    else
    {
        switch(terrain_type)
        {
            case tt_Grasslands1:
            case tt_Grasslands2:
            case tt_Grasslands3:
            case tt_Grasslands4:
            {
                food_units = 3;
            } break;
            case tt_Forest1:
            case tt_Forest2:
            case tt_Forest3:
            {
                food_units = 1;
            } break;
            case tt_Mountain1:
            {
                food_units = 0;
            } break;
            case tt_Desert1:
            case tt_Desert2:
            case tt_Desert3:
            case tt_Desert4:
            {
                food_units = 0;
            } break;
            case tt_Swamp1:  /* OGBUG  manual says 1/2 food for Swamp */
            case tt_Swamp2:  /* OGBUG  manual says 1/2 food for Swamp */
            case tt_Swamp3:  /* OGBUG  manual says 1/2 food for Swamp */
            {
                food_units = 0;
            } break;
            case tt_Tundra1:
            case tt_Tundra2:
            case tt_Tundra3:
            {
                food_units = 0;
            } break;
            case tt_SorceryNode:
            {
                food_units = 4;
            } break;
            case tt_NatureNode:
            {
                food_units = 5;  /* OGBUG  MGC NewGame version has 4 */
            } break;
            case tt_ChaosNode:
            {
                food_units = 0;
            } break;
            case tt_Hills1:
            {
                food_units = 1;
            } break;
            case tt_Volcano:
            {
                food_units = 0;
            } break;
            default:
            {
                food_units = 0;
            } break;
        }
    }

    food_units *= 2;  /* ¿ OGBUG  should be /= 2, not *= 2 ? everywhere else is food2, not foodx2 */

    if(City_Area_Square_Is_Shared(wx, wy, wp) == ST_TRUE)
    {
        food_units /= 2;
    }

    return food_units;
}

// WZD s161p04
// drake178: TILE_GetProd()
/*
    Chaos Node ~== Mountain
    ¿ Volcano ?
*/
int16_t Square_Production_Bonus(int16_t wx, int16_t wy, int16_t wp, int16_t have_gaias_blessing)
{
    uint16_t terrain_type = 0;
    int16_t production_bonus = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    terrain_type = TERRAIN_TYPE(wx, wy, wp);

    if(terrain_type <= tt_Desert_Lst)
    {
        if(terrain_type <= tt_Mountains_Lst)
        {
            if(terrain_type <= tt_Rivers_end)
            {
                if( (terrain_type == tt_Mountain1) || (terrain_type == tt_ChaosNode) )
                {
                    // HERE:  base/super-type 'Moutain' or 'Chaos Node'
                    production_bonus = 5;
                }
                else
                {
                    if
                    (
                        (terrain_type == tt_Forest1) ||
                        (terrain_type == tt_Forest2) ||
                        (terrain_type == tt_Forest3) ||
                        (terrain_type == tt_NatureNode)
                    )
                    {
                        if(have_gaias_blessing != ST_TRUE)
                        {
                            production_bonus = 3;
                        }
                        else
                        {
                            production_bonus = 6;
                        }
                    }
                    else
                    {
                        if(
                            (terrain_type == tt_Hills1) ||
                            (terrain_type == tt_Desert2) ||
                            (terrain_type == tt_Desert3) ||
                            (terrain_type == tt_Desert4)
                        )
                        {
                            production_bonus = 3;
                        }
                        else
                        {
                            production_bonus = 0;  // DNE in Dasm - magically falls-through / jumps to 'return 0'
                        }
                    }
                }
            }
            else
            {
                // HERE:  'Mountain Range'
                production_bonus = 5;
            }
        }
        else
        {
            // HERE:  'Hills Range' and 'Desert Range'
            production_bonus = 3;
        }

        if(City_Area_Square_Is_Shared(wx, wy, wp) != ST_FALSE)
        {
            production_bonus = (production_bonus / 2);
        }
    }
    else
    {
        // HERE:  Above last range that has any production bonus
        production_bonus = 0;
    }

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

    return production_bonus;
}

// WZD s161p05
// drake178: TILE_GetWaterGold()
int16_t Square_Gold_Bonus(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t on_coast;
    int16_t on_river;
    int16_t gold_bonus;
    int16_t itr_wy;  // _SI_
    int16_t itr_wx;  // _DI_

    on_river = ST_FALSE;
    on_coast = ST_FALSE;

    // check if the map square we are on is terrain type river
    //   or if any of the adjacent map squares are shore/coast/ocean
    for(itr_wy = -1; itr_wy < 2; itr_wy++)
    {
        for(itr_wx = -1; itr_wx < 2; itr_wx++)
        {
            if( (itr_wy != 0) || (itr_wx != 0) )
            {
                if( (wy + itr_wy >= 0) && (wy + itr_wy < WORLD_HEIGHT) )
                {
                    if(Square_Is_Sailable((wx + itr_wx), (wy + itr_wy), wp) != ST_FALSE)
                    {
                        on_coast = ST_TRUE;
                    }
                }
            }
            else
            {
                if(Square_Is_River(wx, wy, wp) != ST_FALSE)
                {
                    on_river = ST_TRUE;
                }
            }
        }
    }

    gold_bonus = 0;

    if(on_river != ST_TRUE)
    {
        if(on_coast != ST_TRUE)
        {

        }
        else
        {
            // HERE:  'Shore'
            gold_bonus = 10;
        }
    }
    else
    {
        if(on_coast != ST_TRUE)
        {
            // HERE:  'River'
            gold_bonus = 20;
        }
        else
        {
            // HERE:  'River Mouth'
            gold_bonus = 30;
        }
    }

    return gold_bonus;
}


// WZD s161p06
// drake178: CTY_GetRoadGold()
int16_t City_Road_Trade_Bonus(int16_t city_idx)
{
    int16_t bit_index = 0;
    int16_t itr_bit_index = 0;
    int16_t itr_roadconn = 0;
    int16_t trade_bonus = 0;  // _SI_


    trade_bonus = 0;  // DNE in Dasm;  RTC used, but uninitialized

    for(itr_roadconn = 0; itr_roadconn < 13; itr_roadconn++)
    {
        if(_CITIES[city_idx].road_connections[itr_roadconn] != 0)
        {
            for(itr_bit_index = 0; itr_bit_index < 8; itr_bit_index++)
            {
                bit_index = ((itr_roadconn * 8) + itr_bit_index);
                if( (bit_index < _cities) && (bit_index != city_idx) )
                {
                    if(Test_Bit_Field(bit_index, (uint8_t *)&_CITIES[city_idx].road_connections[0]) != ST_FALSE)
                    {
                        if(_CITIES[city_idx].race != _CITIES[bit_index].race)
                        {
                            trade_bonus += _CITIES[bit_index].population;
                        }
                        else
                        {
                            trade_bonus += (_CITIES[bit_index].population / 2);
                        }
                    }
                }
            }
        }
    }

    // Maximum Road Connection Trade Bonus
    // Run-Time Check Failure #3 - The variable 'trade_bonus' is being used without being initialized.
    if( (_CITIES[city_idx].population * 3) < trade_bonus)
    {
        trade_bonus = (_CITIES[city_idx].population * 3);
    }

    return trade_bonus;
}


// WZD s161p07
/*
used to decide on river for cityscape  (not ocean, not *water*)
*/
int16_t Square_Is_River(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;
    int16_t is_river = 0;  // DNE in Dasm
    is_river = ST_FALSE;  // DNE in Dasm
    if(wx < 0)           { wx += WORLD_WIDTH; }
    if(wx > WORLD_WIDTH) { wx -= WORLD_WIDTH; }
    if( (wy >= 0) && (wy < WORLD_HEIGHT) )
    {
        terrain_type = (p_world_map[wp][wy][wx] % NUM_TERRAIN_TYPES);
        if(terrain_type <= tt_4WRiver5)
        {
            if(terrain_type <= tt_Shore2_end)
            {
                if(terrain_type <= tt_Rivers_end)
                {
                    if(terrain_type <= tt_Shore2F_end)
                    {
                        if(terrain_type <= tt_RiverM_end)
                        {
                            if(terrain_type <= tt_Forest3)
                            {
                                is_river = ST_FALSE;
                            }
                            else
                            {
                                is_river = ST_TRUE;
                            }
                        }
                        else
                        {
                            is_river = ST_FALSE;
                        }
                    }
                    else
                    {
                        is_river = ST_TRUE;
                    }
                }
                else
                {
                    is_river = ST_FALSE;
                }
            }
            else
            {
                is_river = ST_TRUE;
            }
        }
        else
        {
            is_river = ST_FALSE;
        }
    }

    return is_river;
}



// WZD s161p08
int16_t Square_Gold_Income(int16_t wx, int16_t wy, int16_t wp, int16_t have_miners_guild, int16_t are_dwarf)
{
    int16_t terrain_special = 0;
    int16_t gold_units = 0;
    terrain_special = GET_TERRAIN_SPECIAL(wx, wy, wp);
    gold_units = 0;
    if(terrain_special == TS_SILVER)
    {
        gold_units = 2;
    }
    if(terrain_special == TS_GOLD)
    {
        gold_units = 3;
    }
    if(terrain_special == TS_GEMS)
    {
        gold_units = 5;
    }
    if(are_dwarf != ST_FALSE)
    {
        gold_units = (gold_units * 2);  // +100%
    }
    if(have_miners_guild != ST_FALSE)
    {
        gold_units = ((gold_units * 3) / 2);  // +50%
    }
    if(City_Area_Square_Is_Shared(wx, wy, wp) != ST_FALSE)
    {
        gold_units = (gold_units / 2);
    }
    return gold_units;
}

// WZD s161p08
// TILE_GetSilverGold   

// WZD s161p09
// TILE_GetGoldOreGold  

// WZD s161p10
// TILE_GetGemGold      

// WZD s161p11
// drake178: TILE_GetSpecialPower()
int16_t Square_Magic_Power(int16_t wx, int16_t wy, int16_t wp, int16_t have_miners_guild, int16_t are_dwarf)
{
    int16_t terrain_special = 0;
    int16_t mana_units = 0;

    terrain_special = GET_TERRAIN_SPECIAL(wx, wy, wp);

    mana_units = 0;

    if(terrain_special == TS_MITHRIL)
    {
        mana_units = 1;
    }

    if(terrain_special == TS_ADAMANTIUM)
    {
        mana_units = 2;
    }

    if(terrain_special == TS_QUORK)
    {
        mana_units = 3;
    }

    if(terrain_special == TS_CRYSX)
    {
        mana_units = 5;
    }

    if(are_dwarf != ST_FALSE)
    {
        mana_units = (mana_units * 2);
    }

    if(have_miners_guild != ST_FALSE)
    {
        mana_units = ((mana_units * 3) / 2);
    }

    if(City_Area_Square_Is_Shared(wx, wy, wp) != ST_FALSE)
    {
        mana_units = (mana_units / 2);
    }

    return mana_units;
}

// WZD s161p12
// drake178: TILE_GetMithrilPower()
/*
    Terrain Special
    Magic Power
*/
int16_t Square_Mithril_Power(int16_t wx, int16_t wy, int16_t wp, int16_t have_minersguild, int16_t are_dwarves)
{
    int16_t terrain_special;
    int16_t power;  // _SI_

    terrain_special = GET_TERRAIN_SPECIAL(wx, wy, wp);

    power = 0;

    if(terrain_special == TS_MITHRIL)
    {
        power = 1;
    }

    if(are_dwarves != ST_FALSE)
    {
        power = (power * 2);
    }

    if(City_Area_Square_Is_Shared(wx, wy, wp) == ST_TRUE)
    {
        power = (power / 2);
    }

    return power;
}

// WZD s161p13
// drake178: TILE_GetAdamntmPower()
/*
    Terrain Special
    Magic Power
*/
int16_t Square_Adamantium_Power(int16_t wx, int16_t wy, int16_t wp, int16_t have_minersguild, int16_t are_dwarves)
{
    int16_t terrain_special;
    int16_t power;  // _SI_

    terrain_special = GET_TERRAIN_SPECIAL(wx, wy, wp);

    power = 0;

    if(terrain_special == TS_ADAMANTIUM)
    {
        power = 2;
    }

    if(are_dwarves != ST_FALSE)
    {
        power = (power * 2);
    }

    if(City_Area_Square_Is_Shared(wx, wy, wp) == ST_TRUE)
    {
        power = (power / 2);
    }

    return power;
}

// WZD s161p14
// drake178: TILE_GetQuorkPower   

// WZD s161p15
// drake178: TILE_GetCrysxPower   

// WZD s161p16
// drake178: TILE_HasMithril()
/*
; returns 1 if the square has Mithril Ore terrain special
; on it, or 0 otherwise
*/
/*
Compute_Base_Values_For_Map_Square()
    if(Square_Has_Mithril(curr_wx, itr_wy, wp) == ST_TRUE)
*/
int16_t Square_Has_Mithril(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_special;

    terrain_special = GET_TERRAIN_SPECIAL(wx, wy, wp);

    if(terrain_special == TS_MITHRIL)
    {
        return ST_TRUE;
    }
    else
    {
        return ST_FALSE;
    }

}


// WZD s161p17
// drake178: TILE_HasAdamantium()
/*

*/
int16_t Square_Has_Adamantium(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_special;

    terrain_special = GET_TERRAIN_SPECIAL(wx, wy, wp);

    if(terrain_special == TS_ADAMANTIUM)
    {
        return ST_TRUE;
    }
    else
    {
        return ST_FALSE;
    }

}


// WZD s161p18
// drake178: TILE_GetNUCostReduce()
/*
    
    TERRAIN SPECIAL MILITARY UNIT COSTS
    Coal            -10%
    Iron Ore        -5%
    * Applies only to normal units except for magicians, priests, shamans and warlocks.
    The bonus is cumulative for multiple iron ore and coal deposits
    up to a maximum reduction in cost of 50%. Does not affect costs of buildings.
*/
int16_t Square_Unit_Cost_Reduction(int16_t wx, int16_t wy, int16_t wp, int16_t have_minersguild, int16_t are_dwarves)
{
    int16_t terrain_special;

    int16_t cost_reduction;  // _SI_

    cost_reduction = 0;

    terrain_special = GET_TERRAIN_SPECIAL(wx, wy, wp);

    if(terrain_special == TS_IRON)
    {
        if(have_minersguild != ST_TRUE)
        {
            cost_reduction = 5;
        }
        else
        {
            cost_reduction = 10;
        }
    }
    
    if(terrain_special == TS_COAL)
    {
        if(have_minersguild != ST_TRUE)
        {
            cost_reduction = 10;
        }
        else
        {
            cost_reduction = 20;
        }
    }

    if(are_dwarves == ST_TRUE)
    {
        cost_reduction = (cost_reduction * 2);
    }

    if(City_Area_Square_Is_Shared(wx, wy, wp) != ST_FALSE)
    {
        cost_reduction = (cost_reduction / 2);
    }

    return cost_reduction;
}


// WZD s161p19
// drake178: CTY_GetWeaponQuality()
// MoO2  Module: INITSHIP  Best_..._Weapon_()
int16_t City_Best_Weapon(int16_t city_idx)
{
    int16_t wy_array[CITY_AREA_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t wx_array[CITY_AREA_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t weapon_quality = 0;
    int16_t city_wp = 0;
    int16_t useable_map_squares = 0;
    int16_t terrain_special = 0;
    int16_t itr = 0;  // _DI_

    weapon_quality = wq_Normal;

    if(
        (_CITIES[city_idx].bldg_status[bt_AlchemistsGuild] == bs_Built)
        ||
        (_CITIES[city_idx].bldg_status[bt_AlchemistsGuild] == bs_Replaced)
    )
    {
        weapon_quality = wq_Magic;

        city_wp = _CITIES[city_idx].wp;

        useable_map_squares = Get_Useable_City_Area(CITYX(), CITYY(), city_wp, &wx_array[0], &wy_array[0]);

        for(itr = 0; itr < useable_map_squares; itr++)
        {
            terrain_special = GET_TERRAIN_SPECIAL(wx_array[itr], wy_array[itr], city_wp);

            if(terrain_special == TS_ADAMANTIUM)
            {
                weapon_quality = wq_Adamantium;
            }

            if(
                (terrain_special == TS_MITHRIL)
                &&
                (weapon_quality != wq_Adamantium)
            )
            {
                weapon_quality = wq_Mithril;
            }

        }

    }

    return weapon_quality;
}


// WZD s161p20
/*
TABLE F
ROAD BUILDING
TERRAIN TYPE OR SPECIAL
TURNS TO BUILD
*/
/**
 * @brief Returns the number of turns required to build a road on a map square.
 *
 * This routine first checks whether the target square already has a road and
 * returns zero in that case. Otherwise it derives the square's base terrain
 * type from the packed world-map value and applies the original terrain-range
 * classification used by road-building logic.
 *
 * Build time depends on the square's effective terrain category:
 * grasslands are fast, deserts and sorcery nodes are moderate, forests and
 * tundra are slower, while mountains, swamps, and volcanoes take the longest.
 * Terrain outside the supported road-buildable categories returns
 * @c ST_UNDEFINED.
 *
 * @param wx World x-coordinate of the square.
 * @param wy World y-coordinate of the square.
 * @param wp World plane containing the square.
 *
 * @return 0 if the square already has a road; otherwise the number of turns
 *         required to build one on the square's terrain, or @c ST_UNDEFINED if
 *         the terrain does not support road construction.
 *
 * @note The function preserves the original threshold-based terrain checks and
 *       uses @c p_world_map[wp][wy][wx] % NUM_TERRAIN_TYPES to strip any packed
 *       non-terrain bits before classifying the square.
 */
int16_t Turns_To_Build_Road(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type;
    if((_map_square_flags[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] & MSF_ROAD) != 0) { return 0; }
    terrain_type = (p_world_map[wp][wy][wx] % NUM_TERRAIN_TYPES);
    if(terrain_type >= tt_Tundra_1st)   { return 6; }
    if(terrain_type == tt_BugGrass)     { return 2; }
    if(terrain_type < tt_Grasslands1)   { return ST_UNDEFINED; }
    if(terrain_type > tt_4WRiver5)      { return ST_UNDEFINED; }
    if(terrain_type > tt_Shore2_end)    { return 5; }
    if(terrain_type > tt_Desert_Lst)    { return ST_UNDEFINED; }
    if(terrain_type > tt_Hills_Lst)     { return 4; }
    if(terrain_type > tt_Mountains_Lst) { return 6; }
    if(terrain_type > tt_Rivers_end)    { return 8; }
    if(terrain_type > tt_Shore2F_end)   { return 5; }
    if(terrain_type > tt_RiverM_end)    { return ST_UNDEFINED; }
    if(terrain_type > tt_Forest3)       { return 5; }
    switch(terrain_type)
    {
        case tt_Grasslands1: { return 3; }
        case tt_Forest1:     { return 6; }
        case tt_Mountain1:   { return 8; }
        case tt_Desert1:     { return 4; }
        case tt_Swamp1:      { return 8; }
        case tt_Tundra1:     { return 6; }
        case tt_SorceryNode: { return 4; }
        case tt_NatureNode:  { return 5; }
        case tt_ChaosNode:   { return 5; }
        case tt_Hills1:      { return 6; }
        case tt_Grasslands2: { return 3; }
        case tt_Grasslands3: { return 3; }
        case tt_Desert2:     { return 4; }
        case tt_Desert3:     { return 4; }
        case tt_Desert4:     { return 4; }
        case tt_Swamp2:      { return 8; }
        case tt_Swamp3:      { return 8; }
        case tt_Volcano:     { return 8; }
        case tt_Grasslands4: { return 3; }
        case tt_Tundra2:     { return 6; }
        case tt_Tundra3:     { return 6; }
        case tt_Forest2:     { return 6; }
        case tt_Forest3:     { return 6; }
    }
    return ST_UNDEFINED;
}


// WZD s161p21
// UU_TILE_GetUnsdMPCost

// WZD s161p22
/*
INCONSISTENT: returns 0 for the single square no-river lake, which means it should also return 0 for its river outlet versions (0xC5 - 0xC8)
*/
/*
WZD s161p22
TILE_IsAISailable()
WZD s161p24
TILE_IsSailable()
WZD s161p39
Square_Is_OceanLike()
*/
int16_t Square_Is_Shoreline(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;
    int16_t is_shoreline = 0;
    terrain_type = (p_world_map[wp][wy][wx] % NUM_TERRAIN_TYPES);
    if(     terrain_type >= _Tundra00001000) { is_shoreline = ST_FALSE; }
    else if(terrain_type >= _Shore1100000R)  { is_shoreline = ST_TRUE;  }
    else if(terrain_type >= _River1111_1)    { is_shoreline = ST_FALSE; }
    else if(terrain_type >= _Shore00011R11)  { is_shoreline = ST_TRUE;  }
    else if(terrain_type >= _River1100_3)    { is_shoreline = ST_FALSE; }
    else if(terrain_type >= _1LakeRiv_W)     { is_shoreline = ST_TRUE;  }
    else if(terrain_type >= tte_Grasslands)  { is_shoreline = ST_FALSE; }
    else if(terrain_type == tt_BugGrass)     { is_shoreline = ST_FALSE; }
    else if(terrain_type == _1Lake)          { is_shoreline = ST_FALSE; }
    else                                     { is_shoreline = ST_TRUE;  }
    return is_shoreline;
}


// WZD s161p23
// drake178: TILE_IsVisibleForest()
// AKA Square_Is_Explored_Forest()
/*
INCONSISTENT: unlike all other square type check functions, this only returns 1 for tiles visible to the human player (explored)
*/
int16_t Square_Is_Forest(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t is_forest = 0;
    int16_t terrain_type = 0;
    terrain_type = (p_world_map[wp][wy][wx] % NUM_TERRAIN_TYPES);
    if(
        (SQUARE_EXPLORED(wx, wy, wp) == ST_TRUE)
        &&
        (
            (terrain_type == tt_Forest1)
            ||
            (terrain_type == tt_Forest2)
            ||
            (terrain_type == tt_Forest3)
            ||
            (terrain_type == tt_NatureNode)
        )
    )
    {
        is_forest = ST_TRUE;
    }
    else
    {
        is_forest = ST_FALSE;
    }
    return is_forest;
}


// WZD s161p24
// drake178: TILE_IsSailable()
/*
    regularly used as 'Square is Ocean-Terrain'
    also used as 'Square is Shore'

WZD s161p22
TILE_IsAISailable()
WZD s161p24
TILE_IsSailable()
WZD s161p39
Square_Is_OceanLike()
BUGBUG  only checks tt_AnimOcean?
TODO  DEDU  what 'water' squares not sailable?
*/
int16_t Square_Is_Sailable(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t return_value = 0;
    uint16_t terrain_type = 0;

    terrain_type = (p_world_map[wp][wy][wx] % NUM_TERRAIN_TYPES);

    // if(terrain_type > 0x25A)  /* _Tundra00001000 */
    if(terrain_type > tt_Tundra_1st)  /* _Tundra00001000 */
    {
        goto Return_False;
    }
    else
    {
        // <= tt_OceanAnim      = 0x259,
        if(terrain_type > 0x1D8)  /* _River1111_5 */
        {
            goto Return_True;
        }
        else
        {
            // <= tt_4WRiver5    = 0x1D8,
            if(terrain_type > 0x1D3)  /* _Shore111R1110 */
            {
                goto Return_False;
            }
            else
            {
                // <= tt_Shore2_end  = 0x1D3,
                if(terrain_type > 0x1C3)  /* _Desert10101111 */
                {
                    // >= tt_Shore2_1st  = 0x1C4,
                    goto Return_True;
                }
                else
                {
                    // <= tt_Desert_Lst  = 0x1C3,
                    if(terrain_type > 0xE8)  /* _Shore000R0000 */
                    {
                        // >= tt_Rivers_1st  = 0x0E9,
                        goto Return_False;
                    }
                    else
                    {
                        // <= tt_Shore2F_end = 0xE8,
                        if(terrain_type > 0xC4)  /* _River1001_2 */
                        {
                            // >= tt_Lake1       = 0x0C5,
                            goto Return_True;
                        }
                        else
                        {
                            // <= tt_RiverM_end  = 0x0C4,
                            if(terrain_type > _Shore10101111)  /* _Shore10101111 */
                            {
                                // >= tt_Grasslands1      = 0x0A2,
                                goto Return_False;
                            }
                            else
                            {
                                // <= tt_Shore1_Lst  = 0x0A1,
                                if(terrain_type == tt_BugGrass)  /* tt_BugGrass */
                                {
                                    goto Return_False;
                                }
                                else
                                {
                                    // _Ocean          = 0x0,
                                    // _Land           = 0x1,
                                    // _Shore00001000  = 0x2,
                                    // ...
                                    // _1Lake          = 0x12,
                                    // ...
                                    // _Shore10101111  = 0xA1,
                                    goto Return_True;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

Return_False:
    return_value = ST_FALSE;
    goto Done;

Return_True:
    return_value = ST_TRUE;
    goto Done;

Done:

    return return_value;
}

// WZD s161p25
/*
    DEDU  Why only tt_OceanAnim?
*/
int16_t Square_Is_Ocean(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;
    int16_t is_ocean = 0;  // DNE in Dasm
    is_ocean = ST_FALSE;  // DNE in Dasm
    terrain_type = (p_world_map[wp][wy][wx] % NUM_TERRAIN_TYPES);
    if(terrain_type == tt_OceanAnim)
    {
        is_ocean = ST_TRUE;
    }
    else
    {
        is_ocean = ST_FALSE;
    }
    return is_ocean;
}


// WZD s161p26
/*
vs. Square_Is_Sailable()?
vs. Square_Is_OceanLike()?
*/
/**
 * @brief Tests whether a map square may be used as an embarkation square.
 *
 * Reads the square terrain type and applies the original threshold-based
 * terrain checks used by AI embark/disembark logic. The function returns true
 * only for specific shoreline-like terrain intervals and false for explicit
 * exclusions such as @c tt_BugGrass and @c tt_Lake.
 *
 * @param wx World x-coordinate of the square to evaluate.
 * @param wy World y-coordinate of the square to evaluate.
 * @param wp World plane containing the evaluated square.
 *
 * @return @c ST_TRUE when the terrain falls within accepted embarkable ranges;
 *         otherwise @c ST_FALSE.
 *
 * @note This routine preserves the original game's nested comparison structure
 *       and naming, including the legacy local variable spelling
 *       @c is_emarkable.
 */
int16_t Square_Is_Embarkable(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;
    int16_t is_embarkable = 0;  // DNE in Dasm
    terrain_type = (p_world_map[wp][wy][wx] % NUM_TERRAIN_TYPES);
    if(terrain_type == tt_BugGrass)   { is_embarkable = ST_FALSE; }
    if(terrain_type == tt_Lake)       { is_embarkable = ST_FALSE; }
    if(terrain_type < _Shore11101110) { is_embarkable = ST_TRUE;  }
    if(terrain_type < _Shore10111000) { is_embarkable = ST_FALSE; }
    if(terrain_type < tte_Grasslands) { is_embarkable = ST_TRUE;  }
    if(terrain_type < _Shore00001R10) { is_embarkable = ST_FALSE; }
    if(terrain_type < _River1100_3)   { is_embarkable = ST_TRUE;  }
    if(terrain_type < _Shore1100000R) { is_embarkable = ST_FALSE; }
    if(terrain_type < _Shore1000111R) { is_embarkable = ST_TRUE;  }
    return is_embarkable;
}


// WZD s161p27
/**
 * @brief Maps a world-map terrain square to a building terrain prerequisite ID.
 *
 * This helper converts a square's raw terrain type into the prerequisite category
 * IDs used by city production/building requirement checks.
 *
 * Classification rules:
 * - Ocean-like tiles map to @c btp_BREQ_Water.
 * - Forest tiles and Nature Node map to @c btp_BREQ_Forest.
 * - Grassland tiles and BugGrass map to @c btp_BREQ_Grass.
 * - Hills-range tiles map to @c btp_BREQ_Hill.
 * - Any non-matching terrain returns its raw terrain type value unchanged.
 *
 * @param wx World-map X coordinate of the square.
 * @param wy World-map Y coordinate of the square.
 * @param wp World plane index containing the square.
 *
 * @return A prerequisite identifier in the building-terrain requirement ID space,
 *         or the raw terrain type when no category override applies.
 *
 * @note Used by city terrain prerequisite evaluation to compare a worked square's
 *       effective prerequisite category against a required terrain ID.
 */
int16_t Building_Terrain_Type_Prerequisite(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;

    terrain_type = p_world_map[wp][wy][wx];

    if(Square_Is_OceanLike(wx, wy, wp) == ST_TRUE)
    {
        return btp_BREQ_Water;
    }

    if(
        (terrain_type == tt_Forest1)
        ||
        (terrain_type == tt_Forest2)
        ||
        (terrain_type == tt_Forest3)
        ||
        (terrain_type == tt_NatureNode)
    )
    {
        return btp_BREQ_Forest;
    }

    if(
        (terrain_type == tt_Grasslands1)
        ||
        (terrain_type == tt_Grasslands2)
        ||
        (terrain_type == tt_Grasslands3)
        ||
        (terrain_type == tt_Grasslands4)
        ||
        (terrain_type == tt_BugGrass)
    )
    {
        return btp_BREQ_Grass;
    }

    if(
        (terrain_type == tt_Hills1)
        ||
        ((terrain_type > tt_Mountains_Lst) && (terrain_type < tt_Desert_Fst))
    )
    {
        return btp_BREQ_Hill;
    }

    return terrain_type;
}


// WZD s161p28
int16_t Square_Is_Hills(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;
    int16_t is_hills = 0;  // DNE in Dasm

    is_hills = ST_FALSE;  // DNE in Dasm

    terrain_type = TERRAIN_TYPE(wx, wy, wp);

    if(
        (terrain_type == tt_Hills1)
        ||
        (
            (terrain_type > tt_Mountains_Lst)   // >= tt_Hills_Fst   = 0x113
            &&
            (terrain_type < tt_Desert_Fst)  // <= tt_Hills_Lst   = 0x123
        )
    )
    {
        is_hills = ST_TRUE;
    }
    else
    {
        is_hills = ST_FALSE;
    }

    return is_hills;
}


// WZD s161p29
int16_t Square_Is_Mountain(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type;
    int16_t is_mountain;  // DNE in Dasm

    is_mountain = ST_FALSE;  // DNE in Dasm

    terrain_type = TERRAIN_TYPE(wx, wy, wp);

    if(
        (terrain_type == tt_Mountain1)
        ||
        (terrain_type == tt_ChaosNode)
        ||
        (terrain_type == tt_Volcano)
        ||
        (
            (terrain_type > tt_Rivers_end)  // >= tt_Mountains_Fst   = 0x103
            &&
            (terrain_type < tt_Hills_Fst)   // <= tt_Mountains_Lst   = 0x112
        )
    )
    {
        is_mountain = ST_TRUE;
    }
    else
    {
        is_mountain = ST_FALSE;
    }

    return is_mountain;
}


// WZD s161p30
int16_t Square_Is_Desert(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;
    int16_t is_desert = 0;  // DNE in Dasm

    is_desert = ST_FALSE;  // DNE in Dasm

    terrain_type = TERRAIN_TYPE(wx, wy, wp);

    if(
        (
            (terrain_type > tt_Hills_Lst)   // >= tt_Desert_Fst  = 0x124
            &&
            (terrain_type < tt_Shore2_1st)  // <= tt_Desert_Lst  = 0x1C3
        )
        ||
        (terrain_type == tt_Desert1)
        ||
        (terrain_type == tt_Desert2)
        ||
        (terrain_type == tt_Desert3)
        ||
        (terrain_type == tt_Desert4)
    )
    {
        is_desert = ST_TRUE;
    }
    else
    {
        is_desert = ST_FALSE;
    }

    return is_desert;
}


// WZD s161p31
int16_t Square_Is_Tundra(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;
    int16_t is_tundra = 0;  // DNE in Dasm

    is_tundra = ST_FALSE;  // DNE in Dasm

    terrain_type = TERRAIN_TYPE(wx, wy, wp);

    if(
        (terrain_type == tt_Tundra1)
        ||
        (terrain_type == tt_Tundra2)
        ||
        (terrain_type == tt_Tundra3)
        ||
        (terrain_type > tt_Tundra_1st)
    )
    {
        is_tundra = ST_TRUE;
    }
    else
    {
        is_tundra = ST_FALSE;
    }

    return is_tundra;
}


// WZD s161p32
int16_t Square_Is_Swamp(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;
    int16_t is_swamp = 0;  // DNE in Dasm

    is_swamp = ST_FALSE;  // DNE in Dasm

    terrain_type = TERRAIN_TYPE(wx, wy, wp);

    if(
        (terrain_type == tt_Swamp1)
        ||
        (terrain_type == tt_Swamp2)
        ||
        (terrain_type == tt_Swamp3)
    )
    {
        is_swamp = ST_TRUE;
    }
    else
    {
        is_swamp = ST_FALSE;
    }

    return is_swamp;
}


// WZD s161p33
int16_t Square_Is_Grasslands(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;
    int16_t is_grasslands = 0;  // DNE in Dasm

    is_grasslands = ST_FALSE;  // DNE in Dasm

    terrain_type = TERRAIN_TYPE(wx, wy, wp);

    if(
        (terrain_type == tt_Grasslands1)
        ||
        (terrain_type == tt_Grasslands2)
        ||
        (terrain_type == tt_Grasslands3)
        ||
        (terrain_type == tt_Grasslands4)
        ||
        (terrain_type == tt_SorceryNode)
    )
    {
        is_grasslands = ST_TRUE;
    }
    else
    {
        is_grasslands = ST_FALSE;
    }

    return is_grasslands;
}


// WZD s161p34
int16_t Square_Is_Volcano(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type;
    int16_t is_volcano;  // DNE in Dasm
    is_volcano = ST_FALSE;  // DNE in Dasm
    terrain_type = (p_world_map[wp][wy][wx] % NUM_TERRAIN_TYPES);
    if(
        (terrain_type == tt_ChaosNode)
        ||
        (terrain_type == tt_Volcano)
    )
    {
        is_volcano = ST_TRUE;
    }
    else
    {
        is_volcano = ST_FALSE;
    }
    return is_volcano;
}


// WZD s161p35
int16_t Square_Has_Corruption(int16_t wx, int16_t wy, int16_t wp)
{
    uint8_t map_square_flag = 0;  // DNE in Dasm
    int16_t has_corruption = 0;  // DNE in Dasm
    has_corruption = ST_FALSE;  // DNE in Dasm
    map_square_flag = GET_MAP_SQUARE_FLAG(wx, wy, wp);
    if((map_square_flag & MSF_CORRUPTION) != 0)
    {
        has_corruption = ST_TRUE;
    }
    else
    {
        has_corruption = ST_FALSE;
    }
    return has_corruption;
}


// WZD s161p36
// drake178: UU_TILE_IsRes_40h()
/*
wild game?
*/
int16_t UU_Square_Has_MSF_UNKNOWN_40H(int16_t wx, int16_t wy, int16_t wp)
{
    uint8_t map_square_flag = 0;  // DNE in Dasm
    int16_t has_unknown40h = 0;  // DNE in Dasm

    has_unknown40h = ST_FALSE;  // DNE in Dasm

    map_square_flag = GET_MAP_SQUARE_FLAG(wx, wy, wp);
    
    if((map_square_flag & MSF_UNKNOWN_40H) != 0)
    {
        has_unknown40h = ST_TRUE;
    }
    else
    {
        has_unknown40h = ST_FALSE;
    }

    return has_unknown40h;
}


// WZD s161p37
// drake178: CTY_CountNightshades()
/*
// ; counts and sets the amount of Nightshades affecting
// ; every city (returns the last count)
// ; BUG: only updates the count if a relevant building is
// ; present, meaning that it can't be cleared either if
// ; one isn't
*/
/*

XREF:
    j_CTY_CountNightshades()
        Next_Turn_Calc()

*/
void All_City_Nightshade_Count(void)
{
    int16_t wy_array[CITY_AREA_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t wx_array[CITY_AREA_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t city_wp__2 = 0;  // ; Plane would have sufficed just fine, no need to have 2 of this
    int16_t useable_map_squares = 0;
    int16_t city_wp = 0;
    int16_t city_population = 0;
    int16_t Unused_StackVar = 0;
    int16_t nightshade_count = 0;
    int16_t city_wy = 0;
    int16_t city_wx = 0;
    int16_t itr_cities = 0;  // _SI_
    int16_t itr = 0;  // _DI_

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        city_wx = _CITIES[itr_cities].wx;
        city_wy = _CITIES[itr_cities].wy;
        city_wp = _CITIES[itr_cities].wp;

        city_population = _CITIES[itr_cities].population;

        Unused_StackVar = 2;

        nightshade_count = 0;

        if(city_population == 0)
        {
            continue;
        }

        if(
            #pragma warning(suppress : 6287)  // TODO  Redundant code
            (_CITIES[itr_cities].bldg_status[ SHRINE           ] != bs_NotBuilt)
            ||
            (_CITIES[itr_cities].bldg_status[ SHRINE           ] != bs_NotBuilt)  /* NOTE: actually, definitely duplicated */
            ||
            (_CITIES[itr_cities].bldg_status[ TEMPLE           ] != bs_NotBuilt)
            ||
            (_CITIES[itr_cities].bldg_status[ PARTHENON        ] != bs_NotBuilt)
            ||
            (_CITIES[itr_cities].bldg_status[ CATHEDRAL        ] != bs_NotBuilt)
            ||
            (_CITIES[itr_cities].bldg_status[ SAGES_GUILD      ] != bs_NotBuilt)
            ||
            (_CITIES[itr_cities].bldg_status[ ORACLE           ] != bs_NotBuilt)
            ||
            (_CITIES[itr_cities].bldg_status[ ALCHEMISTS_GUILD ] != bs_NotBuilt)
            ||
            (_CITIES[itr_cities].bldg_status[ WIZARDS_GUILD    ] != bs_NotBuilt)
        )
        {

            city_wp__2 = _CITIES[itr_cities].wp;

            useable_map_squares = Get_Useable_City_Area(CITIESX(), CITIESY(), city_wp__2, &wx_array[0], &wy_array[0]);

            for(itr = 0; itr < useable_map_squares; itr++)
            {

                if((GET_TERRAIN_SPECIAL(wx_array[itr], wy_array[itr], city_wp__2) & TS_NIGHTSHADE) != 0)
                {
                    nightshade_count++;
                }

            }

            _CITIES[itr_cities].enchantments[NIGHTSHADE] = (uint8_t)nightshade_count;

        }

    }

}


// WZD s161p38
int16_t Square_Is_Land(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;
    int16_t is_land = 0;  // DNE in Dasm
    is_land = ST_FALSE;
    terrain_type = (p_world_map[wp][wy][wx] % NUM_TERRAIN_TYPES);
    if(terrain_type >= tt_Tundra_1st)
    {
        is_land = ST_TRUE;
    }
    else if(terrain_type == tt_BugGrass)
    {
        is_land = ST_TRUE;
    }
    else if(terrain_type >= tt_Rivers_1st && terrain_type <= tt_Desert_Lst)
    {
        is_land = ST_TRUE;
    }
    else if(terrain_type >= tt_4WRiver1 && terrain_type <= tt_4WRiver5)
    {
        is_land = ST_TRUE;
    }
    else if(terrain_type >= tt_Grasslands1 && terrain_type <= tt_RiverM_end)
    {
        is_land = ST_TRUE;
    }
    return is_land;
}


// WZD s161p39
/*
used to decide on ocean for cityscape  (not river, not *water*)

; returns 1 if the square is a shore, ocean, or lake, or 0 otherwise
; differs from TILE_IsAISailable in that it checks for
; invalid square indices (not Plane though), for which
; it returns 0 instead
; INCONSISTENT: returns 0 for the single square no-river
; lake, which means it should also return 0 for its
; river outlet versions (0xC5 - 0xC8)

used by AI_Move_Out_Boats(), so ~== "Sailable"
should also be equivalent to Square_Is_Shoreline(), used to build the shoreline linked-list array

WZD s161p22
    TILE_IsAISailable()
WZD s161p24
    TILE_IsSailable()
WZD s161p39
    Square_Is_OceanLike() AKA TILE_IsAISailable2()

*/
int16_t Square_Is_OceanLike(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type;  // _CX_
    int16_t is_ocean;  // DNE in Dasm

    is_ocean = ST_FALSE;

    if(wx < 0)
    {
        wx += WORLD_WIDTH;
    }

    if(wx > WORLD_WIDTH)
    {
        wx -= WORLD_WIDTH;
    }

    if( (wy >= 0) && (wy < WORLD_HEIGHT) )
    {
        
        terrain_type = TERRAIN_TYPE(wx, wy, wp);

        if(terrain_type <= _Tundra00001000)
        {
            if(terrain_type <= _River1111_5)
            {
                if(terrain_type <= _Shore111R1110)
                {
                    if(terrain_type <= _Desert10101111)
                    {
                        if(terrain_type <= _Shore000R0000)
                        {
                            if(terrain_type <= _River1001_2)
                            {
                                if(terrain_type <= _Shore10101111)
                                {
                                    if(terrain_type != tt_BugGrass)
                                    {
                                        if(terrain_type != _1Lake)
                                        {
                                            is_ocean = ST_TRUE;
                                        }
                                        else
                                        {
                                            is_ocean = ST_FALSE;
                                        }
                                    }
                                    else
                                    {
                                        is_ocean = ST_FALSE;
                                    }
                                }
                                else
                                {
                                    is_ocean = ST_FALSE;
                                }
                            }
                            else
                            {
                                is_ocean = ST_TRUE;
                            }
                        }
                        else
                        {
                            is_ocean = ST_FALSE;
                        }
                    }
                    else
                    {
                        is_ocean = ST_TRUE;
                    }
                }
                else
                {
                    is_ocean = ST_FALSE;
                }
            }
            else
            {
                is_ocean = ST_TRUE;
            }
        }
        else
        {
            is_ocean = ST_FALSE;
        }
    }

    return is_ocean;
}



// WZD s161p40
// drake178: UU_TILE_BotchedResFn()
/*
; Unused in MoM, no wonder since its totally botched
;
; would return a resource value (likely gold) based on
; square type, but in the end overwrites most of the
; values it originally wants to set due to the
; non-nesting of its "if" instructions
*/
/*
~ Square_Gold_Bonus()

looks like it's checking for shore, river, and/or river mouth?

*/
int16_t UU_Square_Provides_Some_Amount(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type;  // _DI_
    int16_t amount = 0;  // _SI_

    amount = 0;

    terrain_type = TERRAIN_TYPE(wx, wy, wp);

    if(terrain_type >= tt_Tundra_1st)  { amount =  0; }
    if(terrain_type == tt_BugGrass)    { amount =  0; }
    if(terrain_type < tt_Grasslands1)  { amount = 10; }
    if(terrain_type > tt_4WRiver5)     { amount = 10; }
    if(terrain_type > tt_Shore2_end)   { amount = 25; }
    if(terrain_type > tt_Desert_Lst)   { amount = 10; }
    if(terrain_type > tt_Hills_Lst)    { amount =  0; }
    if(terrain_type > tt_Mountains_Lst)    { amount =  0; }
    if(terrain_type > tt_Rivers_end)   { amount =  0; }
    if(terrain_type > tt_Forest3)      { amount = 25; }

    switch(terrain_type)
    {
        case tt_Grasslands1:  // 0xA2  162  tt_Grasslands1
        case tt_Forest1:      // 0xA3  163  tt_Forest1
        case tt_Mountain1:    // 0xA4  164  tt_Mountain1
        case tt_Desert1:      // 0xA5  165  tt_Desert1
        case tt_Swamp1:       // 0xA6  166  tt_Swamp1
        case tt_Tundra1:      // 0xA7  167  tt_Tundra1
        case tt_SorceryNode:  // 0xA8  168  tt_SorceryNode
        case tt_NatureNode:   // 0xA9  169  tt_NatureNode
        case tt_ChaosNode:    // 0xAA  170  tt_ChaosNode
        case tt_Hills1:       // 0xAB  171  tt_Hills1
        case tt_Volcano:      // 0xB3  179  tt_Volcano__2
        {
            amount = 0;
        }
    }

    if(City_Area_Square_Is_Shared(wx, wy, wp) != ST_FALSE)
    {
        amount /= 2;
    }

    return amount;

}