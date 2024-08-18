// #pragma runtime_checks( "", off )
/*
    Terrain
    Terrain Types
    Terrain Type Attributes


    WIZARDS.EXE
        ovr161
    
*/

#include "MoX.H"

#include "TerrType.H"



// TerType_Count 0x2FA 762



// WZD s161p01
void Map_Square_Clear_Corruption(int16_t wx, int16_t wy, int16_t wp)
{
    _map_square_flags[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] &= 0xDF;  // not TF_Corruption
}


// WZD s161p02
// TILE_IsNode          

// WZD s161p03
// drake178: TILE_GetFood()
/*

    drake178:  BUG: swamps do not yield the documented 1/2 food
*/
int16_t Map_Square_Food2(int16_t wx, int16_t wy, int16_t wp)
{
// uint8_t * world_data_ptr;
// 
// terrain_001_index = GET_2B_OFS(_world_maps, ((world_plane * WORLD_SIZE * 2) + (itr_world_y * WORLD_WIDTH * 2) + (curr_world_x * 2)));
// 
// world_maps_offset = ((world_plane * WORLD_SIZE * 2) + (itr_world_y * WORLD_WIDTH * 2) + (curr_world_x * 2));
// terrain_001_index = GET_2B_OFS(_world_maps,world_maps_offset);
// 
// world_data_ptr = (_world_maps + (world_plane * 4800));
// terrain_type_idx = GET_2B_OFS(world_data_ptr, ((minimap_square_y * 120) + (minimap_square_x * 2)));

    uint8_t * world_map_ptr;
    int16_t terrain_type_idx;
    int16_t terrain_type;
    int16_t food_units;
    // IDGI  int16_t terrain_type_switch_value;

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: BEGIN: Map_Square_Food2()\n", __FILE__, __LINE__);
// #endif

    world_map_ptr = (_world_maps + (wp * WORLD_SIZE * 2) + (wy * WORLD_WIDTH * 2) + (wx * 2));
    terrain_type_idx = GET_2B_OFS(world_map_ptr, 0);
    terrain_type = terrain_type_idx % NUM_TERRAIN_TYPES;

    if(terrain_type != 0)
    {
        // HERE:  >= 1 && <= ??
        if(terrain_type < TT_Ocean2)
        {
            // HERE:  >= 1 && < 'Tundra Range'
            if(terrain_type != TT_BugGrass)
            {
                // HERE:  >= 2 && < 'Tundra Range'
                if(terrain_type >= _Grasslands1)  /* enum OVL_Tiles_Extended */
                {
                    // HERE:  > 'Shore1 Range' && < 'Tundra Range'
                    if(terrain_type <= _River1111_5)
                    {
                        // HERE:  > 'Shore1 Range' && < 'Shore3 Range < 'Tundra Range'
                        if(terrain_type <= _Shore111R1110)
                        {
                            // HERE:  > 'Shore1 Range' && < '4WRiver Range' < 'Shore3 Range < 'Tundra Range'
                            if(terrain_type <= _Desert10101111)
                            {
                                // HERE:  > 'Shore1 Range' && < 'Shore2 Range'  < '4WRiver Range' < 'Shore3 Range < 'Tundra Range'
                                if(terrain_type <= _1Hills2)
                                {
                                    // HERE:  > 'Shore1 Range' && < 'Desert Range' < 'Shore2 Range'  < '4WRiver Range' < 'Shore3 Range < 'Tundra Range'
                                    if(terrain_type <= _Mount1001)
                                    {
                                        // HERE:  > 'Shore1 Range' && < 'Hills Range' < 'Desert Range' < 'Shore2 Range'  < '4WRiver Range' < 'Shore3 Range < 'Tundra Range'
                                        if(terrain_type <= TT_Rivers_end)
                                        {
                                            // HERE:  > 'Shore1 Range' && < 'Mountain Range' < 'Hills Range' < 'Desert Range' < 'Shore2 Range'  < '4WRiver Range' < 'Shore3 Range < 'Tundra Range'
                                            if(terrain_type <= TT_Forest3)
                                            {
                                                // HERE:  > 'Shore1 Range' && 'RiverM, Lake, Shore2, Rivers Range' < 'Mountain Range' < 'Hills Range' < 'Desert Range' < 'Shore2 Range'  < '4WRiver Range' < 'Shore3 Range < 'Tundra Range'
                                                // HERE:  ¿ 'RiverM, Lake, Shore2, Rivers Range' ?

                                                // IDGI  terrain_type_switch_value = terrain_type - e_TT_Grass1;  /* enum TER_TYPES */
//  ; DATA XREF: TILE_GetFood+B5
//  ; jump table for switch statement
// ovr161:01AC
// sw_terrtype_based_grass
// sw_terrtype_based_grass[ 0] = 5B 01 = sw_terrtype_based_grass_00_10_11_18    TT_Grass1     = 0A2h
// sw_terrtype_based_grass[ 1] = 60 01 = sw_terrtype_based_grass_01_21_22       TT_Forest1    = 0A3h
// sw_terrtype_based_grass[ 2] = 62 01 = sw_terrtype_based_grass_02             TT_Mountain1  = 0A4h
// sw_terrtype_based_grass[ 3] = 64 01 = sw_terrtype_based_grass_03_12_13_14    TT_Desert1    = 0A5h
// sw_terrtype_based_grass[ 4] = 66 01 = sw_terrtype_based_grass_04_15_16       TT_Swamp1     = 0A6h
// sw_terrtype_based_grass[ 5] = 68 01 = sw_terrtype_based_grass_05_19_20       TT_Tundra1    = 0A7h
// sw_terrtype_based_grass[ 6] = 6A 01 = sw_terrtype_based_grass_06             TT_SorcNode   = 0A8h
// sw_terrtype_based_grass[ 7] = 6F 01 = sw_terrtype_based_grass_07             TT_NatNode    = 0A9h
// sw_terrtype_based_grass[ 8] = 74 01 = sw_terrtype_based_grass_08             TT_ChaosNode  = 0AAh
// sw_terrtype_based_grass[ 9] = 7A 01 = sw_terrtype_based_grass_09             TT_Hills1     = 0ABh
// sw_terrtype_based_grass[10] = 5B 01 = sw_terrtype_based_grass_00_10_11_18    TT_Grass2     = 0ACh
// sw_terrtype_based_grass[11] = 5B 01 = sw_terrtype_based_grass_00_10_11_18    TT_Grass3     = 0ADh
// sw_terrtype_based_grass[12] = 64 01 = sw_terrtype_based_grass_03_12_13_14    TT_Desert2    = 0AEh
// sw_terrtype_based_grass[13] = 64 01 = sw_terrtype_based_grass_03_12_13_14    TT_Desert3    = 0AFh
// sw_terrtype_based_grass[14] = 64 01 = sw_terrtype_based_grass_03_12_13_14    TT_Desert4    = 0B0h
// sw_terrtype_based_grass[15] = 66 01 = sw_terrtype_based_grass_04_15_16       TT_Swamp2     = 0B1h
// sw_terrtype_based_grass[16] = 66 01 = sw_terrtype_based_grass_04_15_16       TT_Swamp3     = 0B2h
// sw_terrtype_based_grass[17] = 76 01 = sw_terrtype_based_grass_17             TT_Volcano    = 0B3h
// sw_terrtype_based_grass[18] = 5B 01 = sw_terrtype_based_grass_00_10_11_18    TT_Grass4     = 0B4h
// sw_terrtype_based_grass[19] = 68 01 = sw_terrtype_based_grass_05_19_20       TT_Tundra2    = 0B5h
// sw_terrtype_based_grass[20] = 68 01 = sw_terrtype_based_grass_05_19_20       TT_Tundra3    = 0B6h
// sw_terrtype_based_grass[21] = 60 01 = sw_terrtype_based_grass_01_21_22       TT_Forest2    = 0B7h
// sw_terrtype_based_grass[22] = 60 01 = sw_terrtype_based_grass_01_21_22       TT_Forest3    = 0B8h

// sw_terrtype_based_grass[ 0] = 5B 01 = sw_terrtype_based_grass_00_10_11_18    TT_Grass1     = 0A2h
// sw_terrtype_based_grass[10] = 5B 01 = sw_terrtype_based_grass_00_10_11_18    TT_Grass2     = 0ACh
// sw_terrtype_based_grass[11] = 5B 01 = sw_terrtype_based_grass_00_10_11_18    TT_Grass3     = 0ADh
// sw_terrtype_based_grass[18] = 5B 01 = sw_terrtype_based_grass_00_10_11_18    TT_Grass4     = 0B4h

// sw_terrtype_based_grass[ 1] = 60 01 = sw_terrtype_based_grass_01_21_22       TT_Forest1    = 0A3h
// sw_terrtype_based_grass[21] = 60 01 = sw_terrtype_based_grass_01_21_22       TT_Forest2    = 0B7h
// sw_terrtype_based_grass[22] = 60 01 = sw_terrtype_based_grass_01_21_22       TT_Forest3    = 0B8h

// sw_terrtype_based_grass[ 2] = 62 01 = sw_terrtype_based_grass_02             TT_Mountain1  = 0A4h

// sw_terrtype_based_grass[ 3] = 64 01 = sw_terrtype_based_grass_03_12_13_14    TT_Desert1    = 0A5h
// sw_terrtype_based_grass[12] = 64 01 = sw_terrtype_based_grass_03_12_13_14    TT_Desert2    = 0AEh
// sw_terrtype_based_grass[13] = 64 01 = sw_terrtype_based_grass_03_12_13_14    TT_Desert3    = 0AFh
// sw_terrtype_based_grass[14] = 64 01 = sw_terrtype_based_grass_03_12_13_14    TT_Desert4    = 0B0h

// sw_terrtype_based_grass[ 4] = 66 01 = sw_terrtype_based_grass_04_15_16       TT_Swamp1     = 0A6h
// sw_terrtype_based_grass[15] = 66 01 = sw_terrtype_based_grass_04_15_16       TT_Swamp2     = 0B1h
// sw_terrtype_based_grass[16] = 66 01 = sw_terrtype_based_grass_04_15_16       TT_Swamp3     = 0B2h

// sw_terrtype_based_grass[ 5] = 68 01 = sw_terrtype_based_grass_05_19_20       TT_Tundra1    = 0A7h
// sw_terrtype_based_grass[19] = 68 01 = sw_terrtype_based_grass_05_19_20       TT_Tundra2    = 0B5h
// sw_terrtype_based_grass[20] = 68 01 = sw_terrtype_based_grass_05_19_20       TT_Tundra3    = 0B6h

// sw_terrtype_based_grass[ 6] = 6A 01 = sw_terrtype_based_grass_06             TT_SorcNode   = 0A8h

// sw_terrtype_based_grass[ 7] = 6F 01 = sw_terrtype_based_grass_07             TT_NatNode    = 0A9h

// sw_terrtype_based_grass[ 8] = 74 01 = sw_terrtype_based_grass_08             TT_ChaosNode  = 0AAh

// sw_terrtype_based_grass[ 9] = 7A 01 = sw_terrtype_based_grass_09             TT_Hills1     = 0ABh

// sw_terrtype_based_grass[17] = 76 01 = sw_terrtype_based_grass_17             TT_Volcano    = 0B3h

// ovr161:015B                                                 sw_terrtype_based_grass_00_10_11_18:    ; DATA XREF: ovr161:sw_terrtype_based_grasso

// TT_Grass1     = 0A2h
// TT_Forest1    = 0A3h
// TT_Mountain1  = 0A4h
// TT_Desert1    = 0A5h
// TT_Swamp1     = 0A6h
// TT_Tundra1    = 0A7h
// TT_SorcNode   = 0A8h
// TT_NatNode    = 0A9h
// TT_ChaosNode  = 0AAh
// TT_Hills1     = 0ABh
// TT_Grass2     = 0ACh
// TT_Grass3     = 0ADh
// TT_Desert2    = 0AEh
// TT_Desert3    = 0AFh
// TT_Desert4    = 0B0h
// TT_Swamp2     = 0B1h
// TT_Swamp3     = 0B2h
// TT_Volcano    = 0B3h
// TT_Grass4     = 0B4h
// TT_Tundra2    = 0B5h
// TT_Tundra3    = 0B6h
// TT_Forest2    = 0B7h
// TT_Forest3    = 0B8h
                                                // {162, ..., 184}
                                                // IDGI  switch(terrain_type_switch_value)
                                                switch(terrain_type)
                                                {
                                                    // terrain_type - e_TT_Grass1 = 0; sw_terrtype_based_grass[0] = sw_terrtype_based_grass_00_10_11_18 = ovr161:015B
                                                    case TT_Grass1:
                                                    case TT_Grass2:
                                                    case TT_Grass3:
                                                    case TT_Grass4:
                                                    {
                                                        food_units = 3;
                                                    } break;
                                                    case TT_Forest1:
                                                    case TT_Forest2:
                                                    case TT_Forest3:
                                                    {
                                                        food_units = 1;
                                                    } break;
                                                    case TT_Mountain1:
                                                    {
                                                        food_units = 0;
                                                    } break;
                                                    case TT_Desert1:
                                                    case TT_Desert2:
                                                    case TT_Desert3:
                                                    case TT_Desert4:
                                                    {
                                                        food_units = 0;
                                                    } break;
                                                    case TT_Swamp1:
                                                    case TT_Swamp2:
                                                    case TT_Swamp3:
                                                    {
                                                        food_units = 0;
                                                    } break;
                                                    case TT_Tundra1:
                                                    case TT_Tundra2:
                                                    case TT_Tundra3:
                                                    {
                                                        food_units = 0;
                                                    } break;
                                                    case TT_SorcNode:
                                                    {
                                                        food_units = 4;
                                                    } break;
                                                    case TT_NatNode:
                                                    {
                                                        food_units = 5;
                                                    } break;
                                                    case TT_ChaosNode:
                                                    {
                                                        food_units = 0;
                                                    } break;
                                                    case TT_Hills1:
                                                    {
                                                        food_units = 1;
                                                    } break;
                                                    case TT_Volcano:
                                                    {
                                                        food_units = 0;
                                                    } break;

                                                }
                                            }
                                            else
                                            {
                                                // HERE:  ¿ 'RiverM, Lake, Shore2, Rivers Range' ?
                                                food_units = 4;
                                            }
                                        }
                                        else
                                        {
                                            // HERE:  'Mountain Range'
                                            food_units = 0;  // DNE in Dasm - magically falls-through / jumps to 'return 0'
                                        }
                                    }
                                    else
                                    {
                                        // HERE:  'Hills Range'
                                        food_units = 1;
                                    }
                                }
                                else
                                {
                                    // HERE:  'Desert Range'
                                    food_units = 0;  // DNE in Dasm - magically falls-through / jumps to 'return 0'
                                }
                            }
                            else
                            {
                                // HERE:  'Shore2 Range'
                                food_units = 1;
                            }
                        }
                        else
                        {
                            // HERE:  '4WRiver Range'
                            food_units = 4;
                        }
                    }
                    else
                    {
                        // HERE:  'Shore3 Range'
                        food_units = 1;

                    }
                }
                else
                {
                    // HERE:  'Shore1 Range'
                    food_units = 1;
                }
            }
            else
            {
                // 0x0001  _Land  TT_BugGrass
                food_units = 3;
            }

            food_units *= 2;

            if(City_Area_Square_Is_Shared(wx, wy, wp) == ST_TRUE)
            {
                food_units /= 2;
            }                               

        }
        else
        {
            // 0x0259  _AnimOcean  TT_Ocean2  && 'Tundra Range'
            food_units = 0;
        }
    }
    else
    {
        // 0x0000  _Ocean  TT_Ocean1
        food_units = 0;
    }

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: END: Map_Square_Food2()\n", __FILE__, __LINE__);
// #endif

    return food_units;
}

// WZD s161p04
// drake178: TILE_GetProd()
/*
    Chaos Node ~== Mountain
    ¿ Volcano ?
*/
int16_t Map_Square_Production_Bonus(int16_t wx, int16_t wy, int16_t wp, int16_t have_gaias_blessing)
{
    uint16_t terrain_type;  // _SI_
    int16_t production_bonus;  // _DI_

    int16_t world_maps_offset;

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: BEGIN: Map_Square_Production_Bonus(wx = %d, wy = %d, wp = %d, have_gaias_blessing = %d)\n", __FILE__, __LINE__, wx, wy, wp, have_gaias_blessing);
// #endif

    // terrain_type = (*( (uint16_t *)(_world_maps + ( (wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + (wx) )) ) % NUM_TERRAIN_TYPES);
    // terrain_type = GET_2B_OFS(_world_maps, ((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + (wx * 2)));
    world_maps_offset = ((wp * WORLD_SIZE * 2) + (wy * WORLD_WIDTH * 2) + (wx * 2));
    terrain_type = GET_2B_OFS(_world_maps,world_maps_offset);

    if(terrain_type <= TT_Desert_end)
    {
        if(terrain_type <= TT_Mntns_end)
        {
            if(terrain_type <= TT_Rivers_end)
            {
                if ( (terrain_type == TT_Mountain1) || (terrain_type == TT_ChaosNode) )
                {
                    // HERE:  base/super-type 'Moutain' or 'Chaos Node'
                    production_bonus = 5;
                }
                else
                {
                    if
                    (
                        (terrain_type == TT_Forest1) ||
                        (terrain_type == TT_Forest2) ||
                        (terrain_type == TT_Forest3) ||
                        (terrain_type == TT_NatNode)
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
                            (terrain_type == TT_Hills1) ||
                            (terrain_type == TT_Desert2) ||
                            (terrain_type == TT_Desert3) ||
                            (terrain_type == TT_Desert4)
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

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: END: Map_Square_Production_Bonus(wx = %d, wy = %d, wp = %d, have_gaias_blessing = %d) { production_bonus = %d }\n", __FILE__, __LINE__, wx, wy, wp, have_gaias_blessing, production_bonus);
// #endif

    return production_bonus;
}

// WZD s161p05
// drake178: TILE_GetWaterGold()
int16_t Map_Square_Gold_Bonus(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t on_coast;
    int16_t on_river;
    int16_t gold_bonus;

    int16_t itr_wy;  // _SI_
    int16_t itr_wx;  // _DI_

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: BEGIN: Map_Square_Gold_Bonus()\n", __FILE__, __LINE__);
// #endif

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
                    if(Terrain_Is_Sailable((wx + itr_wx), (wy + itr_wy), wp) != ST_FALSE)
                    {
                        on_coast = ST_TRUE;
                    }
                }
            }
            else
            {
                if(Terrain_Is_River(wx, wy, wp) != ST_FALSE)
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

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: END: Map_Square_Gold_Bonus()\n", __FILE__, __LINE__);
// #endif

    return gold_bonus;
}


// WZD s161p06
// drake178: CTY_GetRoadGold()
int16_t City_Road_Trade_Bonus(int16_t city_idx)
{
    int16_t bit_index;
    int16_t itr_bit_index;
    int16_t itr_roadconn;

    int16_t trade_bonus;  // _SI_

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: BEGIN: City_Road_Trade_Bonus(city_idx = %d)\n", __FILE__, __LINE__, city_idx);
// #endif

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

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: END: City_Road_Trade_Bonus(city_idx = %d) { trade_bonus = %d }\n", __FILE__, __LINE__, city_idx, trade_bonus);
// #endif

    return trade_bonus;
}


// WZD s161p07
// drake178: TILE_IsRiver()
/*
used to decide on river for cityscape  (not ocean, not *water*)
*/
int16_t Terrain_Is_River(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type;  // _SI_
    int16_t is_river;

    is_river = ST_FALSE;  // DNE in Dasm

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
        terrain_type = (*( (uint16_t *)(_world_maps + ( (wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + (wx) )) ) % NUM_TERRAIN_TYPES);

        if(terrain_type <= TT_4WRiver5)
        {
            if(terrain_type <= TT_Shore2_end)
            {
                if(terrain_type <= TT_Rivers_end)
                {
                    if(terrain_type <= TT_Shore2F_end)
                    {
                        if(terrain_type <= TT_RiverM_end)
                        {
                            if(terrain_type <= TT_Forest3)
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
// drake178: TILE_GetSpecialGold()
int16_t Map_Square_Gold_Income(int16_t wx, int16_t wy, int16_t wp, int16_t have_miners_guild, int16_t are_dwarf)
{
    int16_t terrain_special;  // _DI_
    int16_t gold_units;  // _SI_

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: BEGIN: Map_Square_Gold_Income(wx = %d, wy = %d, wp = %d, have_miners_guild = %d, are_dwarf = %d)\n", __FILE__, __LINE__, wx, wy, wp, have_miners_guild, are_dwarf);
// #endif

    terrain_special = *(TBL_Terr_Specials + (wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx);

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: terrain_special: %04X\n", __FILE__, __LINE__, terrain_special);
// #endif

    gold_units = 0;

    if(terrain_special == 0x03 /* TS_SilverOre */)
    {
        gold_units = 2;
    }

    if(terrain_special == 0x04 /* TS_GoldOre */)
    {
        gold_units = 3;
    }

    if(terrain_special == 0x05 /* TS_Gems */)
    {
        gold_units = 5;
    }

    if(are_dwarf != ST_FALSE)
    {
        gold_units = (gold_units * 2);
    }

    if(have_miners_guild != ST_FALSE)
    {
        gold_units = ((gold_units * 3) / 2);
    }

    if(City_Area_Square_Is_Shared(wx, wy, wp) != ST_FALSE)
    {
        gold_units = (gold_units / 2);
    }

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: END: Map_Square_Gold_Income(wx = %d, wy = %d, wp = %d, have_miners_guild = %d, are_dwarf = %d) { gold_units = %d }\n", __FILE__, __LINE__, wx, wy, wp, have_miners_guild, are_dwarf, gold_units);
// #endif

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
int16_t Map_Square_Magic_Power(int16_t wx, int16_t wy, int16_t wp, int16_t have_miners_guild, int16_t are_dwarf)
{
    int16_t terrain_special;  // _DI_
    int16_t mana_units;  // _SI_

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: BEGIN: Map_Square_Magic_Power()\n", __FILE__, __LINE__);
// #endif

    terrain_special = *(TBL_Terr_Specials + (wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx);

    mana_units = 0;

    if(terrain_special == 0x06 /* TS_MithrilOre */)
    {
        mana_units = 1;
    }

    if(terrain_special == 0x07 /* TS_AdamantiumOre */)
    {
        mana_units = 2;
    }

    if(terrain_special == 0x08 /* TS_QuorkCrystals */)
    {
        mana_units = 3;
    }

    if(terrain_special == 0x09 /* TS_CrysxCrystals */)
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

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: END: Map_Square_Magic_Power()\n", __FILE__, __LINE__);
// #endif

    return mana_units;
}

// WZD s161p12
// TILE_GetMithrilPower 

// WZD s161p13
// TILE_GetAdamntmPower 

// WZD s161p14
// TILE_GetQuorkPower   

// WZD s161p15
// TILE_GetCrysxPower   

// WZD s161p16
// TILE_HasMithril      

// WZD s161p17
// TILE_HasAdamantium   


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
int16_t Terrain_Unit_Cost_Reduction(int16_t wx, int16_t wy, int16_t wp, int16_t have_minersguild, int16_t are_dwarves)
{
    int16_t terrain_special;

    int16_t cost_reduction;  // _SI_

    cost_reduction = 0;

    terrain_special = *(TBL_Terr_Specials + (wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx);

    if(terrain_special == 0x01 /* TS_IronOre */)
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
    
    if(terrain_special == 0x02 /* TS_Coal */)
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
    int16_t wy_array[CITY_AREA_SIZE];
    int16_t wx_array[CITY_AREA_SIZE];
    int16_t weapon_quality;
    int16_t city_wp;
    int16_t useable_map_squares;
    int16_t terrain_special;
    int16_t itr;  // _DI_

    weapon_quality = wq_Normal;

    if(
        (_CITIES[city_idx].bldg_status[bt_AlchemistsGuild] == bs_Built) ||
        (_CITIES[city_idx].bldg_status[bt_AlchemistsGuild] == bs_Replaced)
    )
    {
        weapon_quality = wq_Magic;

        city_wp = _CITIES[city_idx].wp;

        useable_map_squares = Get_Useable_City_Area(CITYX(), CITYY(), city_wp, &wx_array[0], &wy_array[0]);

        for(itr = 0; itr < useable_map_squares; itr++)
        {
            terrain_special = *(TBL_Terr_Specials + (city_wp * WORLD_SIZE) + (wy_array[itr] * WORLD_WIDTH) + wx_array[itr]);

            if(terrain_special == 7)  /* TS_AdamantiumOre */
            {
                weapon_quality = wq_Adamantium;
            }

            if(
                (terrain_special == 6) &&  /* TS_MithrilOre */
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
// drake178: TILE_GetRoadBldTime()
/*
; returns the amount of time, in turns, that it would
; take to construct a road on the tile, -1 if one may
; not be built, or 0 if there already is one
*/
/*
TABLE F
ROAD BUILDING
TERRAIN TYPE OR SPECIAL
TURNS TO BUILD
*/
/*
no idea how to make this look sensible
maybe not actually a switch

*/
int16_t Turns_To_Build_Road(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type;  // _CX_
    int16_t world_maps_offset;  // DNE in Dasm

    if((_map_square_flags[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] & TF_Road) != 0)
    {
        return 0;
    }

    // terrain_type = (*( (uint16_t *)(_world_maps + ( (wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + (wx) )) ) % NUM_TERRAIN_TYPES);
    world_maps_offset = ((wp * WORLD_SIZE * 2) + (wy * WORLD_WIDTH * 2) + (wx * 2));
    terrain_type = GET_2B_OFS(_world_maps,world_maps_offset);

    if(terrain_type >= TT_Tundra_1st)
    {
        return 6;
    }

    if(terrain_type == TT_BugGrass)
    {
        return 2;
    }

    // TT_Ocean1, TT_BugGrass, TT_Shore1_1st, TT_Lake, TT_Shore1_end
    if(terrain_type < TT_Grass1)
    {
        return ST_UNDEFINED;
    }

    if(terrain_type > TT_4WRiver5)
    {
        return ST_UNDEFINED;
    }

    if(terrain_type > TT_Shore2_end)
    {
        return 5;
    }

    if(terrain_type > TT_Desert_end)
    {
        return ST_UNDEFINED;
    }

    if(terrain_type > TT_Hills_end)
    {
        return 4;
    }

    if(terrain_type > TT_Mntns_end)
    {
        return 6;
    }

    if(terrain_type > TT_Rivers_end)
    {
        return 8;
    }

    if(terrain_type > TT_Shore2F_end)
    {
        return 5;
    }

    if(terrain_type > TT_RiverM_end)
    {
        return ST_UNDEFINED;
    }

    if(terrain_type > TT_Forest3)
    {
        return 5;
    }

    switch(terrain_type)
    {
        case TT_Grass1:     /* 0x0A2 */
        {
            return 3;
        } break;
        case TT_Forest1:    /* 0x0A3 */
        {
            return 6;
        } break;
        case TT_Mountain1:  /* 0x0A4 */
        {
            return 8;
        } break;
        case TT_Desert1:    /* 0x0A5 */
        {
            return 4;
        } break;
        case TT_Swamp1:     /* 0x0A6 */
        {
            return 8;
        } break;
        case TT_Tundra1:    /* 0x0A7 */
        {
            return 6;
        } break;
        case TT_SorcNode:   /* 0x0A8 */
        {
            return 4;
        } break;
        case TT_NatNode:    /* 0x0A9 */
        {
            return 5;
        } break;
        case TT_ChaosNode:  /* 0x0AA */
        {
            return 5;
        } break;
        case TT_Hills1:     /* 0x0AB */
        {
            return 6;
        } break;
        case TT_Grass2:     /* 0x0AC */
        {
            return 3;
        } break;
        case TT_Grass3:     /* 0x0AD */
        {
            return 3;
        } break;
        case TT_Desert2:    /* 0x0AE */
        {
            return 4;
        } break;
        case TT_Desert3:    /* 0x0AF */
        {
            return 4;
        } break;
        case TT_Desert4:    /* 0x0B0 */
        {
            return 4;
        } break;
        case TT_Swamp2:     /* 0x0B1 */
        {
            return 8;
        } break;
        case TT_Swamp3:     /* 0x0B2 */
        {
            return 8;
        } break;
        case TT_Volcano:    /* 0x0B3 */
        {
            return 8;
        } break;
        case TT_Grass4:     /* 0x0B4 */
        {
            return 3;
        } break;
        case TT_Tundra2:    /* 0x0B5 */
        {
            return 6;
        } break;
        case TT_Tundra3:    /* 0x0B6 */
        {
            return 6;
        } break;
        case TT_Forest2:    /* 0x0B7 */
        {
            return 6;
        } break;
        case TT_Forest3:    /* 0x0B8 */
        {
            return 6;
        } break;

    }

    return ST_UNDEFINED;
}


// WZD s161p21
// UU_TILE_GetUnsdMPCost

// WZD s161p22
// TILE_IsAISailable    
/*
WZD s161p22
TILE_IsAISailable()
WZD s161p24
TILE_IsSailable()
WZD s161p39
Terrain_Is_Ocean()
*/
/*
WZD s161p22
TILE_IsAISailable()
; returns 1 if the tile is a shore, ocean, or lake, or 0 otherwise
; INCONSISTENT: returns 0 for the single tile no-river
; lake, which means it should also return 0 for its
; river outlet versions (0xC5 - 0xC8)

; drake178: TILE_IsSailable()
; returns 1 for ocean/shore/lake tiles, or 0 otherwise

*/

// WZD s161p23
// drake178: TILE_IsVisibleForest()
/*
returns 1 if the tile is explored by the human
player, and is a forest tile, or 0 otherwise

INCONSISTENT: unlike all other tile type check
functions, this only returns 1 for tiles visible to
the human player (explored)
*/
int16_t Terrain_Is_Explored_Forest(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_is_explored_forest;
    uint8_t * world_map_ptr;
    int16_t terrain_type_idx;
    int16_t terrain_type;
    int16_t square_is_explored;

    world_map_ptr = (_world_maps + (wp * WORLD_SIZE * 2) + (wy * WORLD_WIDTH * 2) + (wx * 2));
    terrain_type_idx = GET_2B_OFS(world_map_ptr, 0);
    terrain_type = terrain_type_idx % NUM_TERRAIN_TYPES;

    square_is_explored = TBL_Scouting[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)];

    if(
        (square_is_explored != ST_FALSE)
        &&
        (
            (terrain_type == TT_Forest1)
            ||
            (terrain_type == TT_Forest2)
            ||
            (terrain_type == TT_Forest3)
            ||
            (terrain_type == TT_NatNode)
        )
    )
    {
        terrain_is_explored_forest = ST_TRUE;
    }
    else
    {
        terrain_is_explored_forest = ST_FALSE;
    }

    return terrain_is_explored_forest;
}


// WZD s161p24
// drake178: TILE_IsSailable()
/*
    regularly used as 'Square is Ocean-Terrain'
    
WZD s161p22
TILE_IsAISailable()
WZD s161p24
TILE_IsSailable()
WZD s161p39
Terrain_Is_Ocean()
*/
int16_t Terrain_Is_Sailable(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t return_value;
    uint8_t * src_sgmt;
    uint16_t src_ofst;
    uint16_t world_map_value;
    uint16_t terrain_type;

    // les  bx, [_world_maps]
    // ~== ES = (&_world_maps[0] / 16); BX = (&_world_maps[0] % 16);
    src_sgmt = _world_maps;
    src_ofst = 0;

    src_ofst = (wp * 4800) + (wy * 120) + (wx * 2);
    world_map_value = GET_2B_OFS(src_sgmt, src_ofst);

    terrain_type = world_map_value % NUM_TERRAIN_TYPES;

    if(terrain_type > 0x25A)  /* _Tundra00001000 */
    {
        goto Return_False;
    }
    else
    {
        if(terrain_type > 0x1D8)  /* _River1111_5 */
        {
            goto Return_True;
        }
        else
        {
            if(terrain_type > 0x1D3)  /* _Shore111R1110 */
            {
                goto Return_False;
            }
            else
            {
                if(terrain_type > 0x1C3)  /* _Desert10101111 */
                {
                    goto Return_True;
                }
                else
                {
                    if(terrain_type > 0xE8)  /* _Shore000R0000 */
                    {
                        goto Return_False;
                    }
                    else
                    {
                        if(terrain_type > 0xC4)  /* _River1001_2 */
                        {
                            goto Return_True;
                        }
                        else
                        {
                            if(terrain_type > 0xA1)  /* _Shore10101111 */
                            {
                                goto Return_False;
                            }
                            else
                            {
                                if(terrain_type == 0x1)  /* TT_BugGrass */
                                {
                                    goto Return_False;
                                }
                                else
                                {
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
// TILE_IsDeepOcean     

// WZD s161p26
// G_TILE_IsAIEmbarkable

// WZD s161p27
// TILE_BuildingReqType 

// WZD s161p28
// drake178: TILE_IsHills()
int16_t Terrain_Is_Hills(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type;  // _CX_
    int16_t is_hills;  // DNE in Dasm

    is_hills = ST_FALSE;  // DNE in Dasm

    terrain_type = (*( (uint16_t *)(_world_maps + ( (wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + (wx) )) ) % NUM_TERRAIN_TYPES);

    if(
        (terrain_type == TT_Hills1) ||
        (terrain_type > TT_Mntns_end) && (terrain_type < TT_Desert_1st)
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
// drake178: TILE_IsMountains()
int16_t Terrain_Is_Mountain(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type;  // _CX_
    int16_t is_mountain;  // DNE in Dasm

    is_mountain = ST_FALSE;  // DNE in Dasm

    terrain_type = (*( (uint16_t *)(_world_maps + ( (wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + (wx) )) ) % NUM_TERRAIN_TYPES);

    if(
        (terrain_type == TT_Mountain1) ||
        (terrain_type == TT_ChaosNode) ||
        (terrain_type == TT_Volcano) ||
        ((terrain_type > TT_Rivers_end) && (terrain_type < TT_Hills_1st))
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
// TILE_IsDesert        

// WZD s161p31
// TILE_IsTundra        

// WZD s161p32
// TILE_IsSwamp         

// WZD s161p33
// TILE_IsGrasslands    

// WZD s161p34
// TILE_IsVolcano       

// WZD s161p35
// TILE_IsCorrupted     
// WZD s161p36
// UU_TILE_IsRes_40h    

// WZD s161p37
// CTY_CountNightshades 

// WZD s161p38
// TILE_IsLand          

// WZD s161p39
// drake178: TILE_IsAISailable2()
/*
WZD s161p22
TILE_IsAISailable()

WZD s161p24
TILE_IsSailable()

WZD s161p39
Terrain_Is_Ocean()
*/
/*
used to decide on ocean for cityscape  (not river, not *water*)

; returns 1 if the tile is a shore, ocean, or lake, or 0 otherwise
; differs from TILE_IsAISailable in that it checks for
; invalid tile indices (not Plane though), for which
; it returns 0 instead
; INCONSISTENT: returns 0 for the single tile no-river
; lake, which means it should also return 0 for its
; river outlet versions (0xC5 - 0xC8)

*/
int16_t Terrain_Is_Ocean(int16_t wx, int16_t wy, int16_t wp)
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
        terrain_type = (*( (uint16_t *)(_world_maps + ( (wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + (wx) )) ) % NUM_TERRAIN_TYPES);

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
                                    if(terrain_type != TT_BugGrass)
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
// UU_TILE_BotchedResFn 
