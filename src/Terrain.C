// #pragma runtime_checks( "", off )
/*
    Terrain
    Terrain Types
    Terrain Type Attributes


    WIZARDS.EXE
        ovr161
    
*/

#include "MoM.H"



// TerType_Count 0x2FA 762



// WZD s161p01
void Map_Square_Clear_Corruption(int16_t wx, int16_t wy, int16_t wp)
{
    _map_square_flags[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] &= 0xDF;  // not MSF_CORRUPTION
}


// WZD s161p02
// TILE_IsNode          

// WZD s161p03
// drake178: TILE_GetFood()
/*
    drake178:  BUG: swamps do not yield the documented 1/2 food
*/
/*

*/
int16_t Square_Food2(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type;
    int16_t food_units;
    // IDGI  int16_t terrain_type_switch_value;

    terrain_type = TERRAIN_TYPE(wx, wy, wp);

    if(terrain_type != TT_Ocean1)
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
                                        if(terrain_type <= tt_Rivers_end)
                                        {
                                            // HERE:  > 'Shore1 Range' && < 'Mountain Range' < 'Hills Range' < 'Desert Range' < 'Shore2 Range'  < '4WRiver Range' < 'Shore3 Range < 'Tundra Range'
                                            if(terrain_type <= tt_Forest3)
                                            {
                                                // HERE:  > 'Shore1 Range' && 'RiverM, Lake, Shore2, Rivers Range' < 'Mountain Range' < 'Hills Range' < 'Desert Range' < 'Shore2 Range'  < '4WRiver Range' < 'Shore3 Range < 'Tundra Range'
                                                // HERE:  ¿ 'RiverM, Lake, Shore2, Rivers Range' ?

                                                // IDGI  terrain_type_switch_value = terrain_type - e_TT_Grass1;  /* enum TER_TYPES */
//  ; DATA XREF: TILE_GetFood+B5
//  ; jump table for switch statement
// ovr161:01AC
// sw_terrtype_based_grass
// sw_terrtype_based_grass[ 0] = 5B 01 = sw_terrtype_based_grass_00_10_11_18    tt_Grasslands1     = 0A2h
// sw_terrtype_based_grass[ 1] = 60 01 = sw_terrtype_based_grass_01_21_22       tt_Forest1    = 0A3h
// sw_terrtype_based_grass[ 2] = 62 01 = sw_terrtype_based_grass_02             tt_Mountain1  = 0A4h
// sw_terrtype_based_grass[ 3] = 64 01 = sw_terrtype_based_grass_03_12_13_14    tt_Desert1    = 0A5h
// sw_terrtype_based_grass[ 4] = 66 01 = sw_terrtype_based_grass_04_15_16       tt_Swamp1     = 0A6h
// sw_terrtype_based_grass[ 5] = 68 01 = sw_terrtype_based_grass_05_19_20       tt_Tundra1    = 0A7h
// sw_terrtype_based_grass[ 6] = 6A 01 = sw_terrtype_based_grass_06             tt_SorceryNode   = 0A8h
// sw_terrtype_based_grass[ 7] = 6F 01 = sw_terrtype_based_grass_07             tt_NatureNode    = 0A9h
// sw_terrtype_based_grass[ 8] = 74 01 = sw_terrtype_based_grass_08             tt_ChaosNode  = 0AAh
// sw_terrtype_based_grass[ 9] = 7A 01 = sw_terrtype_based_grass_09             tt_Hills1     = 0ABh
// sw_terrtype_based_grass[10] = 5B 01 = sw_terrtype_based_grass_00_10_11_18    tt_Grasslands2     = 0ACh
// sw_terrtype_based_grass[11] = 5B 01 = sw_terrtype_based_grass_00_10_11_18    tt_Grasslands3     = 0ADh
// sw_terrtype_based_grass[12] = 64 01 = sw_terrtype_based_grass_03_12_13_14    tt_Desert2    = 0AEh
// sw_terrtype_based_grass[13] = 64 01 = sw_terrtype_based_grass_03_12_13_14    tt_Desert3    = 0AFh
// sw_terrtype_based_grass[14] = 64 01 = sw_terrtype_based_grass_03_12_13_14    tt_Desert4    = 0B0h
// sw_terrtype_based_grass[15] = 66 01 = sw_terrtype_based_grass_04_15_16       tt_Swamp2     = 0B1h
// sw_terrtype_based_grass[16] = 66 01 = sw_terrtype_based_grass_04_15_16       tt_Swamp3     = 0B2h
// sw_terrtype_based_grass[17] = 76 01 = sw_terrtype_based_grass_17             tt_Volcano    = 0B3h
// sw_terrtype_based_grass[18] = 5B 01 = sw_terrtype_based_grass_00_10_11_18    tt_Grasslands4     = 0B4h
// sw_terrtype_based_grass[19] = 68 01 = sw_terrtype_based_grass_05_19_20       tt_Tundra2    = 0B5h
// sw_terrtype_based_grass[20] = 68 01 = sw_terrtype_based_grass_05_19_20       tt_Tundra3    = 0B6h
// sw_terrtype_based_grass[21] = 60 01 = sw_terrtype_based_grass_01_21_22       tt_Forest2    = 0B7h
// sw_terrtype_based_grass[22] = 60 01 = sw_terrtype_based_grass_01_21_22       tt_Forest3    = 0B8h

// sw_terrtype_based_grass[ 0] = 5B 01 = sw_terrtype_based_grass_00_10_11_18    tt_Grasslands1     = 0A2h
// sw_terrtype_based_grass[10] = 5B 01 = sw_terrtype_based_grass_00_10_11_18    tt_Grasslands2     = 0ACh
// sw_terrtype_based_grass[11] = 5B 01 = sw_terrtype_based_grass_00_10_11_18    tt_Grasslands3     = 0ADh
// sw_terrtype_based_grass[18] = 5B 01 = sw_terrtype_based_grass_00_10_11_18    tt_Grasslands4     = 0B4h

// sw_terrtype_based_grass[ 1] = 60 01 = sw_terrtype_based_grass_01_21_22       tt_Forest1    = 0A3h
// sw_terrtype_based_grass[21] = 60 01 = sw_terrtype_based_grass_01_21_22       tt_Forest2    = 0B7h
// sw_terrtype_based_grass[22] = 60 01 = sw_terrtype_based_grass_01_21_22       tt_Forest3    = 0B8h

// sw_terrtype_based_grass[ 2] = 62 01 = sw_terrtype_based_grass_02             tt_Mountain1  = 0A4h

// sw_terrtype_based_grass[ 3] = 64 01 = sw_terrtype_based_grass_03_12_13_14    tt_Desert1    = 0A5h
// sw_terrtype_based_grass[12] = 64 01 = sw_terrtype_based_grass_03_12_13_14    tt_Desert2    = 0AEh
// sw_terrtype_based_grass[13] = 64 01 = sw_terrtype_based_grass_03_12_13_14    tt_Desert3    = 0AFh
// sw_terrtype_based_grass[14] = 64 01 = sw_terrtype_based_grass_03_12_13_14    tt_Desert4    = 0B0h

// sw_terrtype_based_grass[ 4] = 66 01 = sw_terrtype_based_grass_04_15_16       tt_Swamp1     = 0A6h
// sw_terrtype_based_grass[15] = 66 01 = sw_terrtype_based_grass_04_15_16       tt_Swamp2     = 0B1h
// sw_terrtype_based_grass[16] = 66 01 = sw_terrtype_based_grass_04_15_16       tt_Swamp3     = 0B2h

// sw_terrtype_based_grass[ 5] = 68 01 = sw_terrtype_based_grass_05_19_20       tt_Tundra1    = 0A7h
// sw_terrtype_based_grass[19] = 68 01 = sw_terrtype_based_grass_05_19_20       tt_Tundra2    = 0B5h
// sw_terrtype_based_grass[20] = 68 01 = sw_terrtype_based_grass_05_19_20       tt_Tundra3    = 0B6h

// sw_terrtype_based_grass[ 6] = 6A 01 = sw_terrtype_based_grass_06             tt_SorceryNode   = 0A8h

// sw_terrtype_based_grass[ 7] = 6F 01 = sw_terrtype_based_grass_07             tt_NatureNode    = 0A9h

// sw_terrtype_based_grass[ 8] = 74 01 = sw_terrtype_based_grass_08             tt_ChaosNode  = 0AAh

// sw_terrtype_based_grass[ 9] = 7A 01 = sw_terrtype_based_grass_09             tt_Hills1     = 0ABh

// sw_terrtype_based_grass[17] = 76 01 = sw_terrtype_based_grass_17             tt_Volcano    = 0B3h

// ovr161:015B                                                 sw_terrtype_based_grass_00_10_11_18:    ; DATA XREF: ovr161:sw_terrtype_based_grasso

// tt_Grasslands1     = 0A2h
// tt_Forest1    = 0A3h
// tt_Mountain1  = 0A4h
// tt_Desert1    = 0A5h
// tt_Swamp1     = 0A6h
// tt_Tundra1    = 0A7h
// tt_SorceryNode   = 0A8h
// tt_NatureNode    = 0A9h
// tt_ChaosNode  = 0AAh
// tt_Hills1     = 0ABh
// tt_Grasslands2     = 0ACh
// tt_Grasslands3     = 0ADh
// tt_Desert2    = 0AEh
// tt_Desert3    = 0AFh
// tt_Desert4    = 0B0h
// tt_Swamp2     = 0B1h
// tt_Swamp3     = 0B2h
// tt_Volcano    = 0B3h
// tt_Grasslands4     = 0B4h
// tt_Tundra2    = 0B5h
// tt_Tundra3    = 0B6h
// tt_Forest2    = 0B7h
// tt_Forest3    = 0B8h
                                                // {162, ..., 184}
                                                // IDGI  switch(terrain_type_switch_value)
                                                switch(terrain_type)
                                                {
                                                    // terrain_type - e_TT_Grass1 = 0; sw_terrtype_based_grass[0] = sw_terrtype_based_grass_00_10_11_18 = ovr161:015B
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
                                                    case tt_Swamp1:
                                                    case tt_Swamp2:
                                                    case tt_Swamp3:
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
                                                        food_units = 5;
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
            // 0x0259  tt_AnimOcean  TT_Ocean2  && 'Tundra Range'
            food_units = 0;
        }
    }
    else
    {
        // 0x0000  _Ocean  TT_Ocean1
        food_units = 0;
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
    uint16_t terrain_type;  // _SI_
    int16_t production_bonus;  // _DI_

    terrain_type = TERRAIN_TYPE(wx, wy, wp);

    if(terrain_type <= TT_Desert_end)
    {
        if(terrain_type <= tt_Mntns_end)
        {
            if(terrain_type <= tt_Rivers_end)
            {
                if ( (terrain_type == tt_Mountain1) || (terrain_type == tt_ChaosNode) )
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
// drake178: TILE_IsRiver()
/*
used to decide on river for cityscape  (not ocean, not *water*)
*/
int16_t Square_Is_River(int16_t wx, int16_t wy, int16_t wp)
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
        terrain_type = TERRAIN_TYPE(wx, wy, wp);

        if(terrain_type <= TT_4WRiver5)
        {
            if(terrain_type <= TT_Shore2_end)
            {
                if(terrain_type <= tt_Rivers_end)
                {
                    if(terrain_type <= TT_Shore2F_end)
                    {
                        if(terrain_type <= TT_RiverM_end)
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
// drake178: TILE_GetSpecialGold()
/*
    returns the gold units/income for the map square
*/
int16_t Square_Gold_Income(int16_t wx, int16_t wy, int16_t wp, int16_t have_miners_guild, int16_t are_dwarf)
{
    int16_t terrain_special;  // _DI_
    int16_t gold_units;  // _SI_

    terrain_special = *(TBL_Terr_Specials + (wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx);

    gold_units = 0;

    if(terrain_special == ts_Silver)
    {
        gold_units = 2;
    }

    if(terrain_special == ts_Gold)
    {
        gold_units = 3;
    }

    if(terrain_special == ts_Gems)
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
    int16_t terrain_special;  // _DI_
    int16_t mana_units;  // _SI_

    terrain_special = *(TBL_Terr_Specials + (wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx);

    mana_units = 0;

    if(terrain_special == ts_Mithril)
    {
        mana_units = 1;
    }

    if(terrain_special == ts_Adamantium)
    {
        mana_units = 2;
    }

    if(terrain_special == ts_QuorkCrystals)
    {
        mana_units = 3;
    }

    if(terrain_special == ts_CrysxCrystals)
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

    terrain_special = *(TBL_Terr_Specials + (wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx);

    power = 0;

    if(terrain_special == ts_Mithril)
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

    terrain_special = *(TBL_Terr_Specials + (wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx);

    power = 0;

    if(terrain_special == ts_Adamantium)
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
; returns 1 if the tile has Mithril Ore terrain special
; on it, or 0 otherwise
*/
/*
Compute_Base_Map_Square_Values()
    if(Square_Has_Mithril(curr_wx, itr_wy, wp) == ST_TRUE)
*/
int16_t Square_Has_Mithril(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_special;

    terrain_special = GET_TERRAIN_SPECIAL(wx, wy, wp);

    if(terrain_special == ts_Mithril)
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

    if(terrain_special == ts_Adamantium)
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

    terrain_special = *(TBL_Terr_Specials + (wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx);

    if(terrain_special == ts_Iron)
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
    
    if(terrain_special == ts_Coal)
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
            terrain_special = *(TBL_Terr_Specials + (city_wp * WORLD_SIZE) + (wy_array[itr] * WORLD_WIDTH) + wx_array[itr]);

            if(terrain_special == ts_Adamantium)
            {
                weapon_quality = wq_Adamantium;
            }

            if(
                (terrain_special == ts_Mithril)
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

    if((_map_square_flags[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] & MSF_ROAD) != 0)
    {
        return 0;
    }

    terrain_type = TERRAIN_TYPE(wx, wy, wp);

    if(terrain_type >= tt_Tundra_1st)
    {
        return 6;
    }

    if(terrain_type == TT_BugGrass)
    {
        return 2;
    }

    // TT_Ocean1, TT_BugGrass, TT_Shore1_1st, TT_Lake, TT_Shore1_end
    if(terrain_type < tt_Grasslands1)
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

    if(terrain_type > tt_Hills_end)
    {
        return 4;
    }

    if(terrain_type > tt_Mntns_end)
    {
        return 6;
    }

    if(terrain_type > tt_Rivers_end)
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

    if(terrain_type > tt_Forest3)
    {
        return 5;
    }

    switch(terrain_type)
    {
        case tt_Grasslands1:     /* 0x0A2 */
        {
            return 3;
        } break;
        case tt_Forest1:    /* 0x0A3 */
        {
            return 6;
        } break;
        case tt_Mountain1:  /* 0x0A4 */
        {
            return 8;
        } break;
        case tt_Desert1:    /* 0x0A5 */
        {
            return 4;
        } break;
        case tt_Swamp1:     /* 0x0A6 */
        {
            return 8;
        } break;
        case tt_Tundra1:    /* 0x0A7 */
        {
            return 6;
        } break;
        case tt_SorceryNode:   /* 0x0A8 */
        {
            return 4;
        } break;
        case tt_NatureNode:    /* 0x0A9 */
        {
            return 5;
        } break;
        case tt_ChaosNode:  /* 0x0AA */
        {
            return 5;
        } break;
        case tt_Hills1:     /* 0x0AB */
        {
            return 6;
        } break;
        case tt_Grasslands2:     /* 0x0AC */
        {
            return 3;
        } break;
        case tt_Grasslands3:     /* 0x0AD */
        {
            return 3;
        } break;
        case tt_Desert2:    /* 0x0AE */
        {
            return 4;
        } break;
        case tt_Desert3:    /* 0x0AF */
        {
            return 4;
        } break;
        case tt_Desert4:    /* 0x0B0 */
        {
            return 4;
        } break;
        case tt_Swamp2:     /* 0x0B1 */
        {
            return 8;
        } break;
        case tt_Swamp3:     /* 0x0B2 */
        {
            return 8;
        } break;
        case tt_Volcano:    /* 0x0B3 */
        {
            return 8;
        } break;
        case tt_Grasslands4:     /* 0x0B4 */
        {
            return 3;
        } break;
        case tt_Tundra2:    /* 0x0B5 */
        {
            return 6;
        } break;
        case tt_Tundra3:    /* 0x0B6 */
        {
            return 6;
        } break;
        case tt_Forest2:    /* 0x0B7 */
        {
            return 6;
        } break;
        case tt_Forest3:    /* 0x0B8 */
        {
            return 6;
        } break;

    }

    return ST_UNDEFINED;
}


// WZD s161p21
// UU_TILE_GetUnsdMPCost

// WZD s161p22
// NOTE(20250216, Jim Balcomb): WTF with all these c&p notes? I'm only just now doing anything with this.
// TILE_IsAISailable    
/*
WZD s161p22
TILE_IsAISailable()
WZD s161p24
TILE_IsSailable()
WZD s161p39
Square_Is_OceanLike()
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
// *whatever*, above...
// WZD s161p22
// drake178: TILE_IsAISailable()
/*
returns 1 if the tile is a shore, ocean, or lake, or
0 otherwise

INCONSISTENT: returns 0 for the single tile no-river
lake, which means it should also return 0 for its
river outlet versions (0xC5 - 0xC8)
*/
/*

*/
int16_t Square_Is_Shoreline(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;  // _CX_
    int16_t is_shoreline = 0;

    terrain_type = TERRAIN_TYPE(wx, wy, wp);

    if(terrain_type >= _Tundra00001000)
    {

        is_shoreline = ST_TRUE;

    }
    else
    {

        if(terrain_type >= _Shore1100000R)
        {

            is_shoreline = ST_TRUE;

        }
        else
        {

            if(terrain_type >= _River1111_1)
            {

                is_shoreline = ST_FALSE;

            }
            else
            {

                if(terrain_type >= _Shore00011R11)
                {

                    is_shoreline = ST_TRUE;

                }
                else
                {

                    if(terrain_type >= _River1100_3)
                    {

                        is_shoreline = ST_FALSE;

                    }
                    else
                    {

                        if(terrain_type >= _1LakeRiv_W)
                        {

                            is_shoreline = ST_TRUE;

                        }
                        else
                        {

                            if(terrain_type >= _Grasslands1)
                            {

                                is_shoreline = ST_FALSE;

                            }
                            else
                            {

                                if(terrain_type == TT_BugGrass)
                                {

                                    is_shoreline = ST_FALSE;

                                }
                                else
                                {

                                    if(terrain_type == _1Lake)
                                    {

                                        is_shoreline = ST_FALSE;

                                    }
                                    else
                                    {

                                        is_shoreline = ST_TRUE;

                                    }

                                }

                            }
                            
                        }

                    }

                }

            }

        }

    }

    return is_shoreline;

}


// WZD s161p23
// drake178: TILE_IsVisibleForest()
// AKA Square_Is_Explored_Forest()
/*
returns 1 if the tile is explored by the human
player, and is a forest tile, or 0 otherwise

INCONSISTENT: unlike all other tile type check
functions, this only returns 1 for tiles visible to
the human player (explored)
*/
int16_t Square_Is_Forest(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t is_forest = 0;
    int16_t terrain_type = 0;

    terrain_type = TERRAIN_TYPE(wx, wy, wp);

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
    ...only checks tt_AnimOcean
*/
int16_t Square_Is_Sailable(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t return_value = 0;
    uint16_t terrain_type = 0;

    terrain_type = TERRAIN_TYPE(wx, wy, wp);

    // if(terrain_type > 0x25A)  /* _Tundra00001000 */
    if(terrain_type > tt_Tundra_1st)  /* _Tundra00001000 */
    {
        goto Return_False;
    }
    else
    {
        // <= TT_Ocean2      = 0x259,
        if(terrain_type > 0x1D8)  /* _River1111_5 */
        {
            goto Return_True;
        }
        else
        {
            // <= TT_4WRiver5    = 0x1D8,
            if(terrain_type > 0x1D3)  /* _Shore111R1110 */
            {
                goto Return_False;
            }
            else
            {
                // <= TT_Shore2_end  = 0x1D3,
                if(terrain_type > 0x1C3)  /* _Desert10101111 */
                {
                    // >= tt_Shore2_1st  = 0x1C4,
                    goto Return_True;
                }
                else
                {
                    // <= TT_Desert_end  = 0x1C3,
                    if(terrain_type > 0xE8)  /* _Shore000R0000 */
                    {
                        // >= TT_Rivers_1st  = 0x0E9,
                        goto Return_False;
                    }
                    else
                    {
                        // <= TT_Shore2F_end = 0xE8,
                        if(terrain_type > 0xC4)  /* _River1001_2 */
                        {
                            // >= TT_Lake1       = 0x0C5,
                            goto Return_True;
                        }
                        else
                        {
                            // <= TT_RiverM_end  = 0x0C4,
                            if(terrain_type > 0xA1)  /* _Shore10101111 */
                            {
                                // >= tt_Grasslands1      = 0x0A2,
                                goto Return_False;
                            }
                            else
                            {
                                // <= TT_Shore1_end  = 0x0A1,
                                if(terrain_type == e_TT_BugGrass)  /* TT_BugGrass */
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
// drake178: TILE_IsDeepOcean()
int16_t Square_Is_Ocean(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;  // _CX_
    int16_t is_ocean = 0;  // DNE in Dasm

    is_ocean = ST_FALSE;  // DNE in Dasm

    terrain_type = TERRAIN_TYPE(wx, wy, wp);

    if(
        (terrain_type == tte_AnimOcean)
    )
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
// drake178: G_TILE_IsAIEmbarkable()
/*
; returns 1 for certain ocean tiles, 0 for everything
; else
;
; BUG: should return 1 for all oceans and shores except
; single tile lakes
*/
/*

*/
int16_t Map_Square_Is_Embarkable(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;  // _CX_
    int16_t is_emarkable = 0;  // DNE in Dasm

    is_emarkable = ST_FALSE;

    terrain_type = TERRAIN_TYPE(wx, wy, wp);

    if(terrain_type == TT_BugGrass)
    {

        is_emarkable = ST_FALSE;

    }
    else
    {

        if(terrain_type == TT_Lake)
        {

            is_emarkable = ST_FALSE;

        }
        else
        {

            if(terrain_type < _Shore11101110)
            {

                is_emarkable = ST_FALSE;

            }
            else
            {

                if(terrain_type < _Shore10111000)
                {

                    is_emarkable = ST_FALSE;

                }
                else
                {

                    if(terrain_type < _Grasslands1)
                    {

                        is_emarkable = ST_TRUE;

                    }
                    else
                    {

                        if(terrain_type < _Shore00001R10)
                        {

                            is_emarkable = ST_FALSE;

                        }
                        else
                        {

                            if(terrain_type < _River1100_3)
                            {

                                is_emarkable = ST_TRUE;

                            }
                            else
                            {

                                if(terrain_type < _Shore1100000R)
                                {

                                    is_emarkable = ST_FALSE;

                                }
                                else
                                {

                                    if(terrain_type < _Shore1000111R)
                                    {

                                        is_emarkable = ST_TRUE;

                                    }
                                    else
                                    {

                                        is_emarkable = ST_FALSE;

                                    }

                                }

                            }

                        }

                    }

                }

            }

        }

    }

    return is_emarkable;

}


// WZD s161p27
// TILE_BuildingReqType 

// WZD s161p28
// drake178: TILE_IsHills()
int16_t Square_Is_Hills(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;  // _CX_
    int16_t is_hills = 0;  // DNE in Dasm

    is_hills = ST_FALSE;  // DNE in Dasm

    terrain_type = TERRAIN_TYPE(wx, wy, wp);

    if(
        (terrain_type == tt_Hills1)
        ||
        (
            (terrain_type > tt_Mntns_end)   // >= tt_Hills_1st   = 0x113
            &&
            (terrain_type < tt_Desert_1st)  // <= tt_Hills_end   = 0x123
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
// drake178: TILE_IsMountains()
int16_t Square_Is_Mountain(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type;  // _CX_
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
            (terrain_type > tt_Rivers_end)  // >= TT_Mntns_1st   = 0x103
            &&
            (terrain_type < tt_Hills_1st)   // <= tt_Mntns_end   = 0x112
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
// drake178: TILE_IsDesert()
int16_t Square_Is_Desert(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;  // _CX_
    int16_t is_desert = 0;  // DNE in Dasm

    is_desert = ST_FALSE;  // DNE in Dasm

    terrain_type = TERRAIN_TYPE(wx, wy, wp);

    if(
        (
            (terrain_type > tt_Hills_end)   // >= tt_Desert_1st  = 0x124
            &&
            (terrain_type < tt_Shore2_1st)  // <= TT_Desert_end  = 0x1C3
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
// drake178: TILE_IsTundra()
int16_t Square_Is_Tundra(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;  // _CX_
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
// drake178: TILE_IsSwamp()
int16_t Square_Is_Swamp(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;  // _CX_
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
// drake178: TILE_IsGrasslands()
int16_t Square_Is_Grasslands(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;  // _CX_
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
// drake178: TILE_IsVolcano()
int16_t Square_Is_Volcano(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type;  // _CX_
    int16_t is_volcano;  // DNE in Dasm

    is_volcano = ST_FALSE;  // DNE in Dasm

    terrain_type = TERRAIN_TYPE(wx, wy, wp);

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
// drake178: TILE_IsCorrupted()
int16_t Square_Has_Corruption(int16_t wx, int16_t wy, int16_t wp)
{
    uint8_t square_flag;  // _CX_
    int16_t has_corruption;  // DNE in Dasm

    has_corruption = ST_FALSE;  // DNE in Dasm

    square_flag = GET_MAP_SQUARE_FLAG(wx, wy, wp);
    
    if((square_flag & MSF_CORRUPTION) != 0)
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
int16_t Square_Has_Unknown40h(int16_t wx, int16_t wy, int16_t wp)
{
    uint8_t square_flag;  // _CX_
    int16_t has_unknown40h;  // DNE in Dasm

    has_unknown40h = ST_FALSE;  // DNE in Dasm

    square_flag = GET_MAP_SQUARE_FLAG(wx, wy, wp);
    
    if((square_flag & MSF_UNKNOWN_40H) != 0)
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

        if(city_population = 0)
        {
            continue;
        }

        if(
            (_CITIES[itr_cities].bldg_status[ SHRINE           ] != bs_NotBuilt)
            ||
            /* Severity	Code	Description	Project	File	Line	Suppression State                                                                   */
            /* Warning	C6287	Redundant code : the left and right sub - expressions are identical.ReMoM	C : \STU\devel\ReMoM\src\Terrain.C	1402    */
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

            _CITIES[itr_cities].enchantments[NIGHTSHADE] = nightshade_count;

        }

    }

}


// WZD s161p38
// drake178: TILE_IsLand()
/*
returns 1 for land (walkable) tiles, 0 otherwise
*/
/*

*/
int16_t Map_Square_Is_Land(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type;  // _CX_
    int16_t is_land;  // DNE in Dasm

    is_land = ST_FALSE;

    terrain_type = TERRAIN_TYPE(wx, wy, wp);

    if(terrain_type > tt_Tundra_1st)
    {
        is_land = ST_TRUE;
    }
    else
    {
        
        if(terrain_type == TT_BugGrass)
        {
            is_land = ST_TRUE;
        }
        else
        {

            if(
                (terrain_type < TT_Rivers_1st)
                ||
                (terrain_type > TT_Rivers_1st)
            )
            {
                if(
                    (terrain_type < TT_4WRiver1)
                    ||
                    (terrain_type > TT_4WRiver5)
                )
                {

                    if
                    (
                        (terrain_type < tt_Grasslands1)
                        ||
                        (terrain_type > TT_RiverM_end)
                    )
                    {

                        is_land = ST_FALSE;

                    }
                    else
                    {

                        is_land = ST_TRUE;

                    }

                }
                else
                {

                    is_land = ST_TRUE;

                }

            }
            else
            {

                is_land = ST_TRUE;

            }

        }

    }

    return is_land;
    
}


// WZD s161p39
// drake178: TILE_IsAISailable2()
/*
WZD s161p22
TILE_IsAISailable()

WZD s161p24
TILE_IsSailable()

WZD s161p39
Square_Is_OceanLike()
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
