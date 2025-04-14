/*
    AI - Build / "Grand Vizier"

    WIZARDS.EXE
        ovr157
*/

#include "MOX/MOX_DAT.H"  /* _players[] */

#include "MOM.H"

#ifdef STU_DEBUG
#include "STU/STU_DBG.H"    /* DLOG() */
#endif



/*
    WIZARDS.EXE  ovr157
*/

// WZD o157p01
// drake178: AI_SetProduction()
/*
; chooses and assigns the next production projects to all of the specified player's cities
; WARNING: can't produce settlers or engineers in more than one city simultaneously
*/
/*

XREF:
    j_AI_SetProduction()
        AI_Overland_Turn()

AI_Overland_Turn()
    |-> j_AI_SetProduction(_SI_itr_players)

*/
void Player_All_Colony_Autobuild(int16_t player_idx)
{
    int16_t assign_new_building = 0;
    int16_t itr_cities = 0;  // _SI_

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

Check_Cities_Data();

        if(assign_new_building == ST_TRUE)
        {

Check_Cities_Data();

            if(player_idx == NEUTRAL_PLAYER_IDX)
            {

Check_Cities_Data();
                Player_Colony_Autobuild_NP(itr_cities);
Check_Cities_Data();

            }
            else
            {

Check_Cities_Data();
                Player_Colony_Autobuild_CP(player_idx, itr_cities);
Check_Cities_Data();

            }

Check_Cities_Data();

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

Check_Cities_Data();
                AI_Player_City_Buy_Production(player_idx, itr_cities);
Check_Cities_Data();

            }

        }

    }

}

// WZD o157p02
// drake178: AI_CTY_SetProduction()
/*
; selects and assigns the next production project to the selected city
; WARNING: can't produce settlers or engineers in more than one city simultaneously
*/
// MoO2  Module: AIBUILD  Player_Colony_Autobuild_()  |->  Assign_Colony_New_Building_()
/*

XREF:
    Player_All_Colony_Autobuild()
    NX_j_AI_CTY_SetProduction()

¿ HP vs. CP vs. NP ?

*/
void Player_Colony_Autobuild_CP(int16_t player_idx, int16_t city_idx)
{
    int16_t Weights[(NUM_BUILDINGS + 14)] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t product_array[(NUM_BUILDINGS + 14)] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t fortress_landmass = 0;
    int16_t Trading_Cities = 0;
    int16_t Total_Cities = 0;
    int16_t city_wy = 0;
    int16_t city_wx = 0;
    int16_t city_wp = 0;
    int16_t Trade_Chance = 0;
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Player_Colony_Autobuild_CP()\n", __FILE__, __LINE__);
#endif

Check_Cities_Data();

    assert((player_idx >= 1) && (player_idx <= 4));

    Trading_Cities = 0;

    Total_Cities = 0;

    for(itr = 0; itr < _cities; itr++)
    {

        if(_CITIES[itr].owner_idx == player_idx)
        {

            if(_CITIES[itr].construction == bt_TradeGoods)
            {

                Trading_Cities++;

            }

            Total_Cities++;

        }

    }

    Trade_Chance = (40 + (((Total_Cities / 8) - Trading_Cities) * 10));

    if(player_idx == HUMAN_PLAYER_IDX)
    {

        Trade_Chance = 0;

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

Check_Cities_Data();
    Calculate_Product_Array(city_idx, &product_count, &uu_depr_count, &uu_unit_count, &product_array[0]);
Check_Cities_Data();

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
if(_CITIES[itr_cities].construction < -4)
{
#ifdef WIN32
    STU_DEBUG_BREAK();
#endif
}
if(_CITIES[itr_cities].construction > 298)
{
#ifdef WIN32
    STU_DEBUG_BREAK();
#endif
}

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

Check_Cities_Data();
    city_unit_count = Map_Square_Unit_Count(city_wx, city_wy, city_wp);
Check_Cities_Data();

    if(city_landmass == fortress_landmass)
    {

        if(Random(30) > city_unit_count)
        {

            need_units = ST_TRUE;

        }
        else
        {

            need_units = ST_TRUE;

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

            need_units = ST_TRUE;

        }

        /* DNE */ /* if((_turn < 20) && (city_unit_count > 1) ) { need_units = ST_FALSE; } */

Check_Cities_Data();

        if(
            (city_unit_count > 0)
            &&
            (_CITIES[city_idx].bldg_status[bt_Barracks] == bs_NotBuilt)
        )
        {

Check_Cities_Data();
            _CITIES[city_idx].construction = bt_Barracks;
// Check_Cities_Data();
Capture_Cities_Data();

            return;

        }
        if(
            (city_unit_count > 1)
            &&
            (_CITIES[city_idx].bldg_status[bt_BuildersHall] == bs_NotBuilt)
        )
        {

Check_Cities_Data();
            _CITIES[city_idx].construction = bt_BuildersHall;
// Check_Cities_Data();
Capture_Cities_Data();

            return;

        }
        if(
            (city_unit_count > 2)
            &&
            (_CITIES[city_idx].bldg_status[bt_Shrine] == bs_NotBuilt)
        )
        {

Check_Cities_Data();
            _CITIES[city_idx].construction = bt_Shrine;
// Check_Cities_Data();
Capture_Cities_Data();

            return;

        }
        if(
            (city_unit_count > 3)
            &&
            (_CITIES[city_idx].bldg_status[bt_Smithy] == bs_NotBuilt)
        )
        {

Check_Cities_Data();
            _CITIES[city_idx].construction = bt_Smithy;
// Check_Cities_Data();
Capture_Cities_Data();

            return;

        }
        if(
            (city_unit_count > 4)
            &&
            (_CITIES[city_idx].bldg_status[bt_Granary] == bs_NotBuilt)
        )
        {

Check_Cities_Data();
            _CITIES[city_idx].construction = bt_Granary;
// Check_Cities_Data();
Capture_Cities_Data();

            return;

        }

Check_Cities_Data();

    }

    if(Random(100) <= Trade_Chance)
    {

        _CITIES[city_idx].construction = bt_TradeGoods;
// Check_Cities_Data();
Capture_Cities_Data();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Player_Colony_Autobuild_CP()\n", __FILE__, __LINE__);
#endif

        return;

    }

    for(itr = 0; itr < product_count; itr++)
    {

        if(product_array[itr] < 100)
        {

            Weights[itr] = (TBL_AI_BLD_BaseWgts[bldg_data_table[product_array[itr]].Category] + TBL_AI_BLD_OBJWgts[bldg_data_table[product_array[itr]].Category][_players[player_idx].Objective]);

        }
        else
        {

            Weights[itr] = (_unit_type_table[product_array[itr]].cost / 10);

        }
    }

    // ¿ BUGBUG  should use city_idx here, not itr ?
    if(_CITIES[itr].population < 5)
    {

        for(itr = 0; itr < product_count; itr++)
        {

            if(product_array[itr] == bt_Housing)
            {

                Weights[itr] *= 2;

            }

        }

    }

    for(itr = 0; itr < product_count; itr++)
    {

        if(product_array[itr] < 100)
        {

            if(need_units == ST_TRUE)
            {

                Weights[itr] = 0;

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

                        Weights[itr] = ((Weights[itr] * 3) / 2);

                    }

                    Weights[itr] = ((Weights[itr] * (TBL_AI_BLD_OBJWgts[9][_players[player_idx].Objective] + 10)) / 10);
                }
                else
                {

                    Weights[itr] = 0;

                }

            }
            else if(_unit_type_table[unit_type].Construction > 0)
            {

                if(need_engineer == ST_TRUE)
                {

                    if(_turn < 50)
                    {

                        Weights[itr] = ((Weights[itr] * 3) / 2);

                    }

                    Weights[itr] = ((Weights[itr] * (TBL_AI_BLD_OBJWgts[9][_players[player_idx].Objective] + 10)) / 10);

                }
                else
                {

                    Weights[itr] = 0;

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

                    Weights[itr] = 0;

                }
                else
                {

                    /* HP  Weights[itr] = (Weights[itr] * ((_turn / 30) - ai_transport_count)); */
                    Weights[itr] = (Weights[itr] * ((_turn / 40) - ai_transport_count));
                    
                }
            }
            else
            {

                if(need_units != ST_FALSE)
                {

                    Weights[itr] = 0;

                }
                else
                {

                    Weights[itr] = ((Weights[itr] * (TBL_AI_BLD_OBJWgts[8][_players[player_idx].Objective] + 10)) / 10);

                }

            }

        }

    }

Check_Cities_Data();
    product_choice = Get_Weighted_Choice(&Weights[0], product_count);
Check_Cities_Data();

Check_Cities_Data();
    _CITIES[city_idx].construction = product_array[product_choice];
// Check_Cities_Data();
Capture_Cities_Data();
if(_CITIES[city_idx].construction < -4)
{
#ifdef WIN32
    STU_DEBUG_BREAK();
#endif
}
if(_CITIES[city_idx].construction > 298)
{
#ifdef WIN32
    STU_DEBUG_BREAK();
#endif
}
Check_Cities_Data();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Player_Colony_Autobuild_CP()\n", __FILE__, __LINE__);
#endif

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
    int16_t Weights[(NUM_BUILDINGS + 14)] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t product_array[(NUM_BUILDINGS + 14)] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t uu_city_landmass = 0;
    int16_t need_units = 0;
    int16_t product_choice = 0;
    int16_t product_count = 0;
    int16_t uu_unit_count = 0;
    int16_t uu_depr_count = 0;
    int16_t itr = 0;  // _DI_

Check_Cities_Data();
    Calculate_Product_Array(city_idx, &product_count, &uu_depr_count, &uu_unit_count, &product_array[0]);
Check_Cities_Data();

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

            Weights[itr] = (_unit_type_table[product_array[itr]].cost / 10);

        }
        else
        {

            Weights[itr] = TBL_AI_BLD_BaseWgts[bldg_data_table[product_array[itr]].Category];

        }

    }

    for(itr = 0; itr < product_count; itr++)
    {

        if(product_array[itr] < 100)
        {

            if(need_units == ST_TRUE)
            {

                Weights[itr] = 0;

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

                Weights[itr] = 0;

            }
            else
            {

                if(need_units == ST_FALSE)
                {

                    Weights[itr] = 0;

                }

            }

        }

    }

    for(itr = 0; itr < product_count; itr++)
    {

        if(product_array[itr] == bt_ShipWrightsGuild)
        {

            Weights[itr] = 0;

        }

        if(product_array[itr] == bt_MaritimeGuild)
        {

            Weights[itr] = 0;

        }

    }

    if(_CITIES[city_idx].bldg_status[bt_Barracks] == bs_NotBuilt)  /* {-1:NotBuilt,0:Replaced,1:Built,2:Removed} */
    {

        _CITIES[city_idx].construction = bt_Barracks;

    }
    else
    {

Check_Cities_Data();
        product_choice = Get_Weighted_Choice(&Weights[0], product_count);
Check_Cities_Data();

Check_Cities_Data();
        _CITIES[city_idx].construction = product_choice;
// Check_Cities_Data();
Capture_Cities_Data();

if(_CITIES[city_idx].construction < -4)
{
#ifdef WIN32
    STU_DEBUG_BREAK();
#endif
}
if(_CITIES[city_idx].construction > 298)
{
#ifdef WIN32
    STU_DEBUG_BREAK();
#endif
}

    }

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

Check_Cities_Data();
        cost_to_buy_product = ((Random(4) + 1) * City_Cost_To_Buy_Product(city_idx));
Check_Cities_Data();

    }
    else
    {

        if((_unit_type_table[(_CITIES[city_idx].construction - 100)].Abilities & UA_CREATEOUTPOST) != 0)  // Settler
        {

Check_Cities_Data();
            cost_to_buy_product = (Random(4) * City_Cost_To_Buy_Product(city_idx));
Check_Cities_Data();

        }
        else
        {

Check_Cities_Data();
            cost_to_buy_product = ((Random(5) + 3) * City_Cost_To_Buy_Product(city_idx));
Check_Cities_Data();

        }
    }

    if(_players[player_idx].gold_reserve > cost_to_buy_product)
    {

Check_Cities_Data();
        Player_City_Buy_Production(player_idx, city_idx);
Check_Cities_Data();

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
    int16_t Weights[(NUM_BUILDINGS + 14)] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
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

Check_Cities_Data();
    Calculate_Product_Array(city_idx, &product_count, &uu_depr_count, &uu_unit_count, &product_array[0]);
Check_Cities_Data();

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
            Weights[itr] = (TBL_AI_BLD_BaseWgts[bldg_data_table[product_array[itr]].Category] + TBL_AI_BLD_OBJWgts[bldg_data_table[product_array[itr]].Category][_players[player_idx].Objective]);
        }
        else
        {
            Weights[itr] = (_unit_type_table[product_array[itr]].cost / 10);
        }
    }

    // ¿ BUGBUG  should use city_idx here, not itr ?
    if(_CITIES[itr].population < 5)
    {
        for(itr = 0; itr < product_count; itr++)
        {
            if(product_array[itr] == bt_Housing)
            {
                Weights[itr] *= 2;
            }
        }
    }

    for(itr = 0; itr < product_count; itr++)
    {
        if(product_array[itr] < 100)
        {
            if(need_units == ST_TRUE)
            {
                Weights[itr] = 0;
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
                        Weights[itr] = ((Weights[itr] * 3) / 2);
                    }

                    Weights[itr] = ((Weights[itr] * (TBL_AI_BLD_OBJWgts[9][_players[player_idx].Objective] + 10)) / 10);
                }
                else
                {
                    Weights[itr] = 0;
                }
            }
            else if(_unit_type_table[unit_type].Construction > 0)
            {
                if(need_engineer == ST_TRUE)
                {
                    if(_turn < 50)
                    {
                        Weights[itr] = ((Weights[itr] * 3) / 2);
                    }

                    Weights[itr] = ((Weights[itr] * (TBL_AI_BLD_OBJWgts[9][_players[player_idx].Objective] + 10)) / 10);
                }
                else
                {
                    Weights[itr] = 0;
                }
            }
            else if(_unit_type_table[unit_type].Transport > 0)
            {
                if(
                    /* CP  (_turn / 40) < ai_transport_count || (ai_transport_count > 8) */
                    (_turn / 30) < ai_transport_count || (ai_transport_count > 10)
                )
                {
                    Weights[itr] = 0;
                }
                else
                {
                    /* CP  Weights[itr] = (Weights[itr] * ((_turn / 40) - ai_transport_count)); */
                    Weights[itr] = (Weights[itr] * ((_turn / 30) - ai_transport_count));
                }
            }
            else
            {
                if(need_units != ST_FALSE)
                {
                    Weights[itr] = 0;
                }
                else
                {
                    Weights[itr] = ((Weights[itr] * (TBL_AI_BLD_OBJWgts[8][_players[player_idx].Objective] + 10)) / 10);
                }
            }
        }
    }

    product_choice = Get_Weighted_Choice(&Weights[0], product_count);

    _CITIES[city_idx].construction = product_array[product_choice];

if(_CITIES[city_idx].construction < -4)
{
#ifdef WIN32
    STU_DEBUG_BREAK();
#endif
}
if(_CITIES[city_idx].construction > 298)
{
#ifdef WIN32
    STU_DEBUG_BREAK();
#endif
}

}
