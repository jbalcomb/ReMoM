/*
    WIZARDS.EXE
        ovr098
        ovr116
        ovr163
*/

#include "MoM.H"





// WZD dseg:C7AA                                                 ¿ BEGIN:  ovr103 ?

// WZD dseg:C7C2
// ; contains a Node_Type flag or -1 (unit bonus aura)
extern int16_t CMB_node_type;

// WZD dseg:C7E6                                                 ¿ END:  ovr103 ?





/*
    WIZARDS.EXE  ovr098
*/


// WZD o98p01
// drake178: CMB_CE_Refresh()
// CMB_CE_Refresh()

// WZD o98p02
// drake178: BU_HasSpellAbility()
// BU_HasSpellAbility()

// WZD o98p03
// drake178: sub_7EC81()
// sub_7EC81()

// WZD o98p04
// drake178: AI_CMB_PlayTurn()
// AI_CMB_PlayTurn()

// WZD o98p05
// drake178: CMB_ProgressTurnFlow()
// CMB_ProgressTurnFlow()

// WZD o98p06
/*
    OON XREF:  End_Of_Combat()
    sets Battle Unit to *dead* for *unsummoned* Units (_UNITS[].wp == 9)

*/
void Retreat_From_Combat(int16_t player_idx)
{
    int16_t Fleeing_Units_Lost[9];
    int16_t Wind_Walker;
    int16_t Transport_Capacity;
    int16_t Fleeing_Death_Count;
    int16_t unit_idx;
    int16_t Checked_X;  // also, boat_riders
    int16_t boat_riders;  // in Dasm, `Checked_X`
    int16_t Diameter;
    int16_t Min_Y;
    int16_t Min_X;
    int16_t Checked_Y;
    int16_t Defender_Fleeing_Count;
    int16_t Attacker_Fleeing_Count;
    int16_t itr_battle_units;  // _SI_
    int16_t fleeing_player_idx;  // _DI_

    Fleeing_Death_Count = 0;

    if(player_idx != _combat_attacker_player)
    {
        fleeing_player_idx = _combat_attacker_player;
    }
    else
    {
        fleeing_player_idx = _combat_defender_player;
    }

    /*
        BEGIN:  Fleeing Battle Unit Counts
    */
    Attacker_Fleeing_Count = 0;
    Defender_Fleeing_Count = 0;

    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
    {
        if(_UNITS[battle_units[itr_battle_units].unit_idx].wp == 9)
        {
            battle_units[itr_battle_units].Status = bus_Dead;
        }

        if(battle_units[itr_battle_units].Status == bus_Fleeing)
        {
            if(battle_units[itr_battle_units].controller_idx == _combat_attacker_player)
            {
                Attacker_Fleeing_Count++;
            }
            else if(battle_units[itr_battle_units].controller_idx == _combat_attacker_player)
            {
                Defender_Fleeing_Count++;
            }
        }

    }
    /*
        END:  Fleeing Battle Unit Counts
    */


    /*
        BEGIN:  
    */

    if(
        (Attacker_Fleeing_Count != 0)
        ||
        (Defender_Fleeing_Count != 0)
    )
    {
// mark all non-involved units as fleeing
// INCONSISTENT: these units have already been marked
// as dead in the parent function, so there will never be any
        if(battle_units[itr_battle_units].Status == bus_Uninvolved)
        {
            battle_units[itr_battle_units].Status = bus_Fleeing;
        }
    }

    /*
        END:  
    */


    /*
        BEGIN:  
    */
    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
    {
        if(battle_units[itr_battle_units].Status == bus_Fleeing)
        {
            unit_idx = battle_units[itr_battle_units].unit_idx;
            _UNITS[unit_idx].wx = OVL_Action_OriginX;
            _UNITS[unit_idx].wy = OVL_Action_OriginY;
            battle_units[itr_battle_units].Status = bus_Active;

        }

    }
    /*
        END:  
    */


    /*
        BEGIN:  
    */
// process fleeing chance - 0% if the unit is under the
// effect of Confusion, Black Sleep, or Web, 100% chance
// if its a human hero on Intro or Easy, 75% if it's a
// hero otherwise, and 50% if it's any other unit
// (success means the unit is marked as active instead)
// clear all road building progress for fleeing units
    if(fleeing_player_idx == _combat_attacker_player)
    {
        if(battle_units[itr_battle_units].Status == bus_Fleeing)
        {
            if(_UNITS[battle_units[itr_battle_units].unit_idx].Rd_Constr_Left > -1)
            {
                _UNITS[battle_units[itr_battle_units].unit_idx].Status = us_Ready;
                _UNITS[battle_units[itr_battle_units].unit_idx].Rd_Constr_Left = -1;
            }

            if(
                (_difficulty <= god_Easy)
                &&
                (fleeing_player_idx == HUMAN_PLAYER_IDX)
            )
            {
                battle_units[itr_battle_units].Status = bus_Active;
            }
            else
            {
                if(_UNITS[battle_units[itr_battle_units].unit_idx].Hero_Slot == -1)
                {
                    if(Random(2) == 1)
                    {
                        battle_units[itr_battle_units].Status = bus_Active;
                    }
                }
                else
                {
                    if(Random(4) != 1)
                    {
                        battle_units[itr_battle_units].Status = bus_Active;
                    }
                }
            }

            if(
                ((battle_units[itr_battle_units].Combat_Effects & bue_Black_Sleep) != 0)
                ||
                ((battle_units[itr_battle_units].Combat_Effects & bue_Confusion) != 0)
                ||
                (
                    ((battle_units[itr_battle_units].Combat_Effects & bue_Web) != 0)
                    &&
                    (battle_units[itr_battle_units].Web_HP > 0)
                )
            )
            {
                battle_units[itr_battle_units].Status = bus_Fleeing;
            }

        }


        /*
            BEGIN:  Ocean Combat
        */
        if(Terrain_Is_Sailable(OVL_Action_XPos, OVL_Action_YPos, OVL_Action_Plane) == ST_TRUE)
        {

    // calculate the transport capacity of the player's surviving units
            Transport_Capacity = 0;

            for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
            {
                if(
                    (battle_units[itr_battle_units].Status == bus_Active)
                    &&
                    (battle_units[itr_battle_units].controller_idx == fleeing_player_idx)
                    &&
                    (battle_units[itr_battle_units].carry_capacity > 0)
                )
                {
                    Transport_Capacity += battle_units[itr_battle_units].carry_capacity;
                }
            }


            if(Transport_Capacity == 0)
            {

    // check if there's a wind walker among the surviving units
                Wind_Walker = ST_FALSE;

                for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
                {

                    if(
                        (battle_units[itr_battle_units].Status == bus_Active)
                        &&
                        (battle_units[itr_battle_units].controller_idx == fleeing_player_idx)
                    )
                    {
                        unit_idx = battle_units[itr_battle_units].unit_idx;

                        if(Unit_Has_WindWalking(unit_idx) == ST_TRUE)
                        {
                            Wind_Walker = ST_TRUE;
                        }
                    }
                }

                if(Wind_Walker != ST_TRUE)
                {
    // mark all active units of the losing player as fleeing
    // if they don't have seafaring capability
    // BUG: ignores Non-Corporeal units
                    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
                    {
                        if(
                            (battle_units[itr_battle_units].Status == bus_Active)
                            &&
                            (battle_units[itr_battle_units].controller_idx == fleeing_player_idx)
                        )
                        {
                            unit_idx = battle_units[itr_battle_units].unit_idx;

                            if(
                                (Unit_Has_AirTravel(unit_idx) == ST_TRUE)
                                ||
                                (Unit_Has_WaterTravel(unit_idx) == ST_TRUE)  /* BUG:  ¿ should be Unit_Has_Swimming() ? SEE BELOW */
                                ||
                                (Unit_Has_AirTravel_Item(unit_idx) == ST_TRUE)
                                ||
                                (Unit_Has_WaterTravel_Item(unit_idx) == ST_TRUE)
                            )
                            {
                                battle_units[itr_battle_units].Status = bus_Active;  // BUG:  ¿ this check for filtered status is due to macro usage ?
                            }
                            else
                            {
                                battle_units[itr_battle_units].Status = bus_Fleeing;
                            }
                        }
                    }
                }

            }
            else  /* if(Transport_Capacity == 0) */
            {
    // check if there's a wind walker among the surviving units
                Wind_Walker = ST_FALSE;

                for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
                {

                    if(
                        (battle_units[itr_battle_units].Status == bus_Active)
                        &&
                        (battle_units[itr_battle_units].controller_idx == fleeing_player_idx)
                    )
                    {
                        unit_idx = battle_units[itr_battle_units].unit_idx;

                        if(Unit_Has_WindWalking(unit_idx) == ST_TRUE)
                        {
                            Wind_Walker = ST_TRUE;
                        }
                    }
                }

            }

    // process transport capacity, marking any active unit
    // that doesn't fit as fleeing again
    // BUG: the checks don't cover all seafaring ability

            if(Wind_Walker != ST_TRUE)
            {
                boat_riders = 0;

                for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
                {

                    if(
                        (battle_units[itr_battle_units].Status == bus_Active)
                        &&
                        (battle_units[itr_battle_units].controller_idx == fleeing_player_idx)
                        &&
                        (battle_units[itr_battle_units].carry_capacity < 1)
                    )
                    {
                        if(_UNITS[battle_units[itr_battle_units].unit_idx].type > ut_Chosen)
                        {
                            if(
                                (Unit_Has_AirTravel_Item(battle_units[itr_battle_units].unit_idx) != ST_TRUE)
                                &&
                                (Unit_Has_WaterTravel_Item(battle_units[itr_battle_units].unit_idx) != ST_TRUE)
                                &&
                                (Unit_Has_Swimming(battle_units[itr_battle_units].unit_idx) != ST_TRUE)  /* BUG:  ¿ should be Unit_Has_WaterTravel() ? SEE ABOVE */
                                &&
                                (Unit_Has_WindWalking(battle_units[itr_battle_units].unit_idx) != ST_TRUE)  /* BUG:  only got here because there were no WindWalkers */
                                &&
                                (Unit_Has_AirTravel(battle_units[itr_battle_units].unit_idx) != ST_TRUE)
                            )
                            {
                                boat_riders++;

                                if(boat_riders > Transport_Capacity)
                                {
                                    battle_units[itr_battle_units].Status = bus_Fleeing;
                                }
                            }
                        }
                    }
                }
            }

        }

        /*
            END:  Ocean Combat
        */


    // add all fleeing units of the losing player to the list of lost units
        for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
        {
            if(
                (battle_units[itr_battle_units].Status == bus_Active)
                &&
                (battle_units[itr_battle_units].controller_idx == fleeing_player_idx)
            )
            {
                Fleeing_Units_Lost[Fleeing_Death_Count] = _UNITS[battle_units[itr_battle_units].unit_idx].type;

                Fleeing_Death_Count++;
            }
        }

        STK_ComposeFleeLost__STUB(Fleeing_Death_Count, Fleeing_Units_Lost);

    }
    else  /* if(fleeing_player_idx == _combat_attacker_player) */
    {
// process fleeing chance - 0% if the unit is under the
// effect of Confusion, Black Sleep, or Web, 100% chance
// if its a human hero on Intro or Easy, 75% if it's a
// hero otherwise, and 50% if it's any other unit
// (failure means the unit is marked as dead instead)
// clear all road building progress for fleeing units,
// and store a list of the lost units
// BUG? has an extra block for non-human units that
// set cancels road building and marks the unit ready overland

        for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
        {

            if(battle_units[itr_battle_units].Status == bus_Fleeing)
            {

                if(fleeing_player_idx != _human_player_idx)
                {
                    _UNITS[battle_units[itr_battle_units].unit_idx].Status = us_Ready;
                    _UNITS[battle_units[itr_battle_units].unit_idx].Rd_Constr_Left = -1;
                }

                if(_UNITS[battle_units[itr_battle_units].unit_idx].Rd_Constr_Left > -1)
                {
                    _UNITS[battle_units[itr_battle_units].unit_idx].Status = us_Ready;
                    _UNITS[battle_units[itr_battle_units].unit_idx].Rd_Constr_Left = -1;
                }

                if(
                    (_difficulty > god_Easy)
                    ||
                    (fleeing_player_idx != HUMAN_PLAYER_IDX)
                )
                {
                    if(_UNITS[battle_units[itr_battle_units].unit_idx].Hero_Slot == -1)
                    {
                        if(Random(2) == 1)
                        {
                            battle_units[itr_battle_units].Status = bus_Dead;
                        }
                    }
                    else
                    {
                        if(Random(4) == 1)
                        {
                            battle_units[itr_battle_units].Status = bus_Dead;
                        }
                    }
                }

                if(
                    ((battle_units[itr_battle_units].Combat_Effects & bue_Black_Sleep) != 0)
                    ||
                    ((battle_units[itr_battle_units].Combat_Effects & bue_Confusion) != 0)
                    ||
                    (
                        ((battle_units[itr_battle_units].Combat_Effects & bue_Web) != 0)
                        &&
                        (battle_units[itr_battle_units].Web_HP > 0)
                    )
                )
                {
                    battle_units[itr_battle_units].Status = bus_Dead;
                }

                if(battle_units[itr_battle_units].Status == bus_Dead)
                {
                    Fleeing_Units_Lost[Fleeing_Death_Count] = _UNITS[battle_units[itr_battle_units].unit_idx].type;

                    Fleeing_Death_Count++;
                }

            }

        }


        /*
            BEGIN:  
        */

        // drake178:  ; BUG: this range check must be performed on the tile to be tested - here, it moves the entire area
        Min_Y = (OVL_Action_YPos - 1);

        SETMIN(Min_Y, 0);

        Min_X = (OVL_Action_XPos - 1);

        if(Min_X < 0)
        {
            Min_X += WORLD_WIDTH;
        }

// attempt to flee as many units as possible to the adjacent tiles
// BUG: the tile range checking allows 2-move flight if the origin X or Y is 0
// BUG: inherits a number of tile eligibility errors
        Diameter = 3;
        Checked_Y = Min_Y;
        while(((Min_Y + Diameter) > Checked_Y) && (Checked_Y < WORLD_HEIGHT))
        {
            itr_battle_units = Min_X;
            while((Min_X + Diameter) > itr_battle_units)
            {
                if(itr_battle_units < WORLD_WIDTH)
                {
                    Checked_X = itr_battle_units;
                }
                else
                {
                    Checked_X = (itr_battle_units - WORLD_WIDTH);
                }

                if(
                    (Checked_X != OVL_Action_XPos)
                    &&
                    (Checked_Y != OVL_Action_YPos)
                )
                {
                    if(
                        (Square_Has_Lair(Checked_X, Checked_Y, OVL_Action_Plane) == ST_UNDEFINED)
                        &&
                        (Player_City_At_Square(Checked_X, Checked_Y, OVL_Action_Plane, fleeing_player_idx) == ST_UNDEFINED)
                    )
                    {
                        Process_Retreating_Units(Checked_X, Checked_Y, OVL_Action_Plane, fleeing_player_idx);
                    }
                }

                itr_battle_units++;
            }

            Checked_Y++;
        }

        /*
            END:  
        */

// mark all fleeing units of the losing player as dead,
// and add them to the list of lost units
        for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
        {
            if(
                (battle_units[itr_battle_units].Status == bus_Fleeing)
                &&
                (battle_units[itr_battle_units].controller_idx == fleeing_player_idx)
            )
            {
                battle_units[itr_battle_units].Status = bus_Dead;

                Fleeing_Units_Lost[Fleeing_Death_Count] = _UNITS[battle_units[itr_battle_units].unit_idx].type;

                Fleeing_Death_Count++;
            }
        }

        STK_ComposeFleeLost__STUB(Fleeing_Death_Count, Fleeing_Units_Lost);

    }  /* else ... if(fleeing_player_idx == _combat_attacker_player) */
    /*
        END:  
    */


    // WTF, Mate?
    // @@JmpDone__Return_ZERO:
    // xor     ax, ax
    // jmp     @@Done
    // NOTE: jumps straight to here from way up at ```if((Attacker_Fleeing_Count != 0) || (Defender_Fleeing_Count != 0))```

}


// WZD o98p07
// drake178: TILE_HasCityOfPlayer()
int16_t Player_City_At_Square(int16_t wx, int16_t wy, int16_t wp, int16_t player_idx)
{
    int16_t city_idx;  // _SI_
    int16_t itr_cities;  // _CX_

    city_idx = -1;

    for(itr_cities = 0; ((itr_cities < _cities) && (city_idx == -1)); itr_cities++)
    {
        if(
            (_CITIES[itr_cities].wp == wp) &&
            (_CITIES[itr_cities].owner_idx == player_idx) &&
            (_CITIES[itr_cities].wy == wy) &&
            (_CITIES[itr_cities].wx == wx)
        )
        {
            city_idx = itr_cities;
        }
    }

    return city_idx;
}


// WZD o98p08
/*
    combat / battle unit
    calls Unit_Try_To_Move()
    returns F,T - successfully fleed
    OON XREF: WIZ_FleeCombat()
¿ MoO2  Module: COMBFIND  Process_Retreating_Ships_() ?
*/
int16_t Process_Retreating_Units(int16_t wx, int16_t wy, int16_t wp, int16_t player_idx)
{
    int16_t troop_list[9];
    int16_t Landlubber_Count;
    int16_t Transport_Capacity;
    int16_t Wind_Walker;
    int16_t Move_Possible;
    int16_t troop_count;
    int16_t itr_towers;  // _SI_
    int16_t itr_battle_units;  // _SI_
    int16_t itr_troops;  // _SI_
    int16_t unit_idx;  // _DI_
    int16_t return_value;  // DNE in Dasm

    Army_At_Square_2(wx, wy, wp, &troop_count, &troop_list[0]);

    if(
        (troop_count <= 0)
        ||
        (
            (_UNITS[troop_list[0]].owner_idx == player_idx)
            &&
            (troop_count < MAX_STACK)
        )
    )
    {

        Wind_Walker = ST_FALSE;

        for(itr_towers = 0; ((itr_towers < NUM_TOWERS) && (Wind_Walker == ST_FALSE)); itr_towers++)
        {
            if(
                (_TOWERS[itr_towers].wx == wx)
                &&
                (_TOWERS[itr_towers].wy == wy)
            )
            {
                goto Return_FALSE;
            }
        }

        Move_Possible = ST_FALSE;

        // Ocean Combat
        if(Terrain_Is_Sailable(wx, wy, wp) == ST_TRUE)
        {
            Wind_Walker = ST_FALSE;

            // check for Wind Walker on source square
            for(itr_battle_units = 0; ((itr_battle_units < _combat_total_unit_count) && (Wind_Walker == ST_FALSE)); itr_battle_units++)
            {
                if(
                    (battle_units[itr_battle_units].Status == bus_Fleeing)
                    &&
                    (battle_units[itr_battle_units].controller_idx == player_idx)
                )
                {
                    unit_idx = battle_units[itr_battle_units].unit_idx;

                    if(unit_idx != -1)
                    {
                        if(Unit_Has_WindWalking(unit_idx) == ST_TRUE)
                        {
                            Move_Possible = ST_TRUE;
                            Wind_Walker = ST_TRUE;
                        }
                    }
                }
            }

            // check for Wind Walker on destination square
            if(Wind_Walker == ST_FALSE)
            {
                for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
                {
                    unit_idx = troop_list[itr_troops];

                    if(unit_idx != -1)
                    {
                        if(Unit_Has_WindWalking(unit_idx) == ST_TRUE)
                        {
                            Move_Possible = ST_TRUE;
                            Wind_Walker = ST_TRUE;
                        }
                    }
                }
            }

            if(Move_Possible == ST_FALSE)
            {
                Transport_Capacity = 0;

                if(
                    (battle_units[itr_battle_units].Status == bus_Fleeing)
                    &&
                    (battle_units[itr_battle_units].controller_idx == player_idx)
                )
                {
                    unit_idx = battle_units[itr_battle_units].unit_idx;

                    if(_unit_type_table[_UNITS[unit_idx].type].Transport > 0)
                    {
                        Transport_Capacity += _unit_type_table[_UNITS[unit_idx].type].Transport;
                    }
                }

                Wind_Walker = ST_FALSE;

                Landlubber_Count = 0;

                for(itr_battle_units = 0; ((itr_battle_units < _combat_total_unit_count) && (Wind_Walker == ST_FALSE)); itr_battle_units++)
                {
                    if(
                        (battle_units[itr_battle_units].Status == bus_Fleeing)
                        &&
                        (battle_units[itr_battle_units].controller_idx == player_idx)
                    )
                    {
                        unit_idx = battle_units[itr_battle_units].unit_idx;

                        if(
                            (Unit_Has_AirTravel(unit_idx) == ST_TRUE)
                            ||
                            (Unit_Has_WaterTravel(unit_idx) == ST_TRUE)
                            ||
                            (Unit_Has_AirTravel_Item(unit_idx) == ST_TRUE)
                            ||
                            (Unit_Has_WaterTravel_Item(unit_idx) == ST_TRUE)
                        )
                        {

                            if(Unit_Try_To_Move(wx, wy, wp, unit_idx, troop_count) == ST_TRUE)
                            {
                                troop_count++;

                                battle_units[itr_battle_units].Status = bus_Active;
                            }
                        }
                        else
                        {
                            Landlubber_Count++;

                            if(Landlubber_Count < Transport_Capacity)
                            {
                                if(Unit_Try_To_Move(wx, wy, wp, unit_idx, troop_count) == ST_TRUE)
                                {
                                    troop_count++;

                                    battle_units[itr_battle_units].Status = bus_Active;
                                }   
                            }
                        }
                    }
                }
            }  /* if(Move_Possible == ST_FALSE) */

        }
        else  /* if(Terrain_Is_Sailable(wx, wy, wp) == ST_TRUE) */
        {
// abort if any of the fleeing units has sailing movement
// BUG: flying and Non-Corporeal ships can move on land

            for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
            {
                if(
                    (battle_units[itr_battle_units].Status == bus_Fleeing)
                    &&
                    (battle_units[itr_battle_units].controller_idx == player_idx)
                )
                {
                    unit_idx = battle_units[itr_battle_units].unit_idx;

                    if(Unit_Has_Sailing(unit_idx) == ST_TRUE)
                    {
                        goto Return_FALSE;
                    }
                }
            }

            Move_Possible = ST_TRUE;
        }


        if(Move_Possible == ST_TRUE)
        {
// move as many fleeing units over to the tile as possible,
// marking them as active if successful
// BUG: there is no guarantee that the wind walker is actually moved over

            for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
            {
                if(
                    (battle_units[itr_battle_units].Status == bus_Fleeing)
                    &&
                    (battle_units[itr_battle_units].controller_idx == player_idx)
                )
                {
                    unit_idx = battle_units[itr_battle_units].unit_idx;

                    if(Unit_Try_To_Move(wx, wy, wp, unit_idx, troop_count) == ST_TRUE)
                    {
                        troop_count++;

                        battle_units[itr_battle_units].Status = bus_Active;
                    }   
                }
            }
        }

        goto Return_TRUE;
    }


Return_FALSE:
    return_value = ST_FALSE;
    goto Done;

Return_TRUE:
    return_value = ST_TRUE;
    goto Done;

Done:
    return return_value;

}


// WZD o98p09
/*
    OON XREF:  TILE_FleeTo()
    moves the unit if there is space
    returns F,T - did the move
*/
int16_t Unit_Try_To_Move(int16_t wx, int16_t wy, int16_t wp, int16_t unit_idx, int16_t troop_count)
{
    int16_t troop_space;
    int16_t return_value;  // DNE in Dasm

    troop_space = (MAX_STACK - troop_count);

    if(troop_space < 1)
    {
        return_value = ST_FALSE;
    }
    else
    {
        _UNITS[unit_idx].wx = wx;
        _UNITS[unit_idx].wy = wy;
        _UNITS[unit_idx].wp = wp;

        return_value = ST_TRUE;
    }

    return return_value;
}


// WZD o98p10
// drake178: CMB_SetActionCursor()
// CMB_SetActionCursor()

// WZD o98p11
// drake178: BU_GetRangedCursor()
// BU_GetRangedCursor()

// WZD o98p12
// drake178: CMB_CreateCELabels()
// CMB_CreateCELabels()

// WZD o98p13
// drake178: BU_GetCmbtMoveType()
// BU_GetCmbtMoveType()

// WZD o98p14
// drake178: UNIT_SummonToBattle()
// UNIT_SummonToBattle()

// WZD o98p15
// drake178: BU_UnitLoadToBattle()
// BU_UnitLoadToBattle()

// WZD o98p16
// drake178: CMB_Units_Init()
// CMB_Units_Init()

// WZD o98p17
// drake178: BU_CheckFlight()
// BU_CheckFlight()

// WZD o98p18
// drake178: BU_IsFlying()
// BU_IsFlying()

// WZD o98p19
// drake178: BU_MeleeFlightCheck()
// BU_MeleeFlightCheck()

// WZD o98p20
// drake178: BU_PrepMoveSound()
// BU_PrepMoveSound()

// WZD o98p21
// drake178: G_CMB_Auto_OFF()
// G_CMB_Auto_OFF()

// WZD o98p22
/*
; composes the "While fleeing, you lost ..." string
; into the GUI_NearMsgString global based on the passed list of units
; INCONSISTENT in its use of plurals with the rest of the game
*/
void STK_ComposeFleeLost__STUB(int16_t troop_count, int16_t troop_list[])
{
    int16_t Unit_Types;
    int16_t Type_Counts;
    int16_t rest_of_the_array;
    int16_t Last_Char_Pointer;
    int16_t Conv_String;
    int16_t Record_Count;
    int16_t Added_Count;
    int16_t Record_Loopvar;
    int16_t itr;  // _DI_

    if(troop_count <= 0)
    {
        strcpy(GUI_NearMsgString, "");
        return;
    }



    strcpy(GUI_NearMsgString, "");
    return;

}



/*
    WIZARDS.EXE  ovr116
*/

// WZD o116p01
/*
returns battle_unit->item_enchantments  <-| BU_Apply_Item_Enchantments()

short-circuits if unit is not a hero unit, returns 0 as in no enchantments

¿ ~ Apply Item Powers ?
    |-> BU_Apply_Item_Enchantments()
so, ...
    handles everything except item enchantments?
but, not ip_Flaming or ip_Giant_Strength ?!?
also, ...
    |-> BU_Apply_Item_Attack_Specials()

*/
uint32_t BU_Apply_Item_Powers(int16_t unit_idx, struct s_BATTLE_UNIT * battle_unit)
{
    int16_t hero_slot_idx;
    int16_t unit_owner_idx;
    uint32_t battle_unit_item_enchantments;
    int16_t item_idx;  // _SI_
    int16_t itr_items;  // _DI_


    battle_unit->item_enchantments = 0;
    battle_unit->Melee_ATK_Flags = 0;
    battle_unit->Ranged_ATK_Flags = 0;


    unit_owner_idx = _UNITS[unit_idx].owner_idx;
    hero_slot_idx = _UNITS[unit_idx].Hero_Slot;


    if(hero_slot_idx == ST_UNDEFINED)
    {
        return 0;
    }


    battle_unit_item_enchantments = 0;

    for(itr_items = 0; itr_items < NUM_HERO_ITEMS; itr_items++)
    {

        if(_players[unit_owner_idx].Heroes[hero_slot_idx].Items[itr_items] != ST_UNDEFINED)
        {
            item_idx = _players[unit_owner_idx].Heroes[hero_slot_idx].Items[itr_items];


            BU_Apply_Item_Enchantments(item_idx, battle_unit);


            if(_ITEMS[item_idx].type == it_Shield)
            {
                battle_unit->Abilities = (battle_unit->Abilities | UA_LARGESHIELD);
            }


            if(battle_unit->mana_max > 0)
            {
                battle_unit->mana_max += _ITEMS[item_idx].spell_skill;
            }


            battle_unit->defense          += _ITEMS[item_idx].defense;
            battle_unit->Gold_Defense     += _ITEMS[item_idx].defense;
            battle_unit->movement_points  += _ITEMS[item_idx].moves2;
            battle_unit->resist           += _ITEMS[item_idx].resistance;
            battle_unit->Gold_Resist      += _ITEMS[item_idx].resistance;


            // if it_Sword, it_Mace, it_Axe, it_Misc ...
            if(
                (_ITEMS[item_idx].type <= it_Axe)
                ||
                (_ITEMS[item_idx].type == it_Misc)
            )
            {
                battle_unit->melee         += _ITEMS[item_idx].attack;
                battle_unit->Gold_Melee    += _ITEMS[item_idx].attack;
                battle_unit->Melee_To_Hit  += _ITEMS[item_idx].tohit;
                
                if(ITEM_POWER(item_idx,ip_Flaming))
                {
                    battle_unit->melee += 3;
                    battle_unit->Gold_Melee += 3;
                }

                if(ITEM_POWER(item_idx,ip_Giant_Strength))
                {
                    battle_unit->melee += 1;
                    battle_unit->Gold_Melee += 1;
                }

                // ip_Vampiric,   ip_Lightning, ip_Destruction, ip_Chaos,    ip_Death,     ip_Power_Drain, ip_Holy_Avenger, ip_Phantasmal, ip_Stoning
                // Att_LifeSteal, Att_ArmorPrc, Att_Destruct,   Att_DoomDmg, Att_DthTouch, Att_PwrDrain,   Att_DsplEvil,    Att_Illusion,  Att_StnTouch
                BU_Apply_Item_Attack_Specials(&battle_unit->Melee_ATK_Flags, item_idx);

            }


            if(
                ((_ITEMS[item_idx].type == it_Bow)   && ((battle_unit->ranged_type / 10) == rag_Missile))
                ||
                ((_ITEMS[item_idx].type == it_Staff) && ((battle_unit->ranged_type / 10) == rag_Magic))
                ||
                ((_ITEMS[item_idx].type == it_Wand)  && ((battle_unit->ranged_type / 10) == rag_Magic))
                ||
                ((_ITEMS[item_idx].type == it_Axe)   && (battle_unit->ranged_type == srat_Thrown))
                ||
                (_ITEMS[item_idx].type == it_Misc)
            )
            {
                battle_unit->ranged         += _ITEMS[item_idx].attack;
                battle_unit->Gold_Ranged    += _ITEMS[item_idx].attack;
                battle_unit->Ranged_To_Hit  += _ITEMS[item_idx].tohit;
                
                if(ITEM_POWER(item_idx,ip_Flaming))
                {
                    battle_unit->ranged += 3;
                    battle_unit->Gold_Ranged += 3;
                }

                if(ITEM_POWER(item_idx,ip_Giant_Strength) && (_ITEMS[item_idx].type == it_Axe) && (battle_unit->ranged_type == srat_Thrown))
                {
                    battle_unit->ranged += 1;
                    battle_unit->Gold_Ranged += 1;
                }

                BU_Apply_Item_Attack_Specials(&battle_unit->Ranged_ATK_Flags, item_idx);

            }

        }

    }


    battle_unit_item_enchantments |= battle_unit->item_enchantments;

    return battle_unit_item_enchantments;
}


// WZD o116p02
/*
    sets item powers in battle_unit.item_enchantments

    ¿ "Spell-Specific Artifact Enchantments" ?
*/
void BU_Apply_Item_Enchantments(int16_t item_idx, struct s_BATTLE_UNIT * battle_unit)
{
    uint32_t item_enchantments;

    item_enchantments = 0;

    if(ITEM_POWER(item_idx,ip_Cloak_Of_Fear))
    {
        item_enchantments |= UE_CLOAKOFFEAR;
    }

    if(ITEM_POWER(item_idx,ip_Wraith_Form))
    {
        item_enchantments |= UE_WRAITHFORM;
    }

    if(ITEM_POWER(item_idx,ip_Regeneration))
    {
        item_enchantments |= UE_REGENERATION;
    }

    if(ITEM_POWER(item_idx,ip_Pathfinding))
    {
        item_enchantments |= UE_PATHFINDING;
    }

    if(ITEM_POWER(item_idx,ip_Water_Walking))
    {
        item_enchantments |= UE_WATERWALKING;
    }

    if(ITEM_POWER(item_idx,ip_Resist_Elements))
    {
        item_enchantments |= UE_RESISTELEMENTS;
    }

    if(ITEM_POWER(item_idx,ip_Elemental_Armour))
    {
        item_enchantments |= UE_ELEMENTALARMOR;
    }

    if(ITEM_POWER(item_idx,ip_Endurance))
    {
        item_enchantments |= UE_ENDURANCE;
    }

    if(ITEM_POWER(item_idx,ip_Invisibility))
    {
        item_enchantments |= UE_INVISIBILITY;
    }

    if(ITEM_POWER(item_idx,ip_Flight))
    {
        item_enchantments |= UE_FLIGHT;
    }

    if(ITEM_POWER(item_idx,ip_Resist_Magic))
    {
        item_enchantments |= UE_RESISTMAGIC;
    }

    if(ITEM_POWER(item_idx,ip_Guardian_Wind))
    {
        item_enchantments |= UE_GUARDIANWIND;
    }

    if(ITEM_POWER(item_idx,ip_Magic_Immunity))
    {
        item_enchantments |= UE_MAGICIMMUNITY;
    }

    if(ITEM_POWER(item_idx,ip_True_Sight))
    {
        item_enchantments |= UE_TRUESIGHT;
    }

    if(ITEM_POWER(item_idx,ip_Bless))
    {
        item_enchantments |= UE_BLESS;
    }

    if(ITEM_POWER(item_idx,ip_Lion_Heart))
    {
        item_enchantments |= UE_LIONHEART;
    }

    if(ITEM_POWER(item_idx,ip_Planar_Travel))
    {
        item_enchantments |= UE_PLANARTRAVEL;
    }

    if(ITEM_POWER(item_idx,ip_Righteousness))
    {
        item_enchantments |= UE_RIGHTEOUSNESS;
    }

    if(ITEM_POWER(item_idx,ip_Invulnerability))
    {
        item_enchantments |= UE_INVULNERABILITY;
    }

    if(ITEM_POWER(item_idx,ip_Holy_Avenger))
    {
        item_enchantments |= UE_BLESS;
    }

    battle_unit->item_enchantments |= item_enchantments;

}


// WZD o116p03
/*
    takes a pointer to a battle unit's attack flags field
    translates item powers to attack specials
*/
void BU_Apply_Item_Attack_Specials(uint16_t * attack_flags, int16_t item_idx)
{

    if(ITEM_POWER(item_idx,ip_Vampiric))
    {
        *attack_flags |= Att_LifeSteal;
    }

    if(ITEM_POWER(item_idx,ip_Lightning))
    {
        *attack_flags |= Att_ArmorPrc;
    }

    if(ITEM_POWER(item_idx,ip_Destruction))
    {
        *attack_flags |= Att_Destruct;
    }

    if(ITEM_POWER(item_idx,ip_Chaos))
    {
        *attack_flags |= Att_DoomDmg;
    }

    if(ITEM_POWER(item_idx,ip_Death))
    {
        *attack_flags |= Att_DthTouch;
    }

    if(ITEM_POWER(item_idx,ip_Power_Drain))
    {
        *attack_flags |= Att_PwrDrain;
    }

    if(ITEM_POWER(item_idx,ip_Holy_Avenger))
    {
        *attack_flags |= Att_DsplEvil;
    }

    if(ITEM_POWER(item_idx,ip_Phantasmal))
    {
        *attack_flags |= Att_Illusion;
    }

    if(ITEM_POWER(item_idx,ip_Stoning))
    {
        *attack_flags |= Att_StnTouch;
    }

}


// WZD o116p04
int16_t Unit_Hit_Points(int16_t unit_idx)
{
    int8_t unit_level;
    int16_t Charm_of_Life_Bonus;
    int16_t hit_points;  // _DI_

    unit_level = _UNITS[unit_idx].Level;

    if(
        ((_UNITS[unit_idx].enchantments & UE_HEROISM) != 0)
        &&
        (unit_level < 3)
    )
    {
        unit_level = 3;
    }

    hit_points = _unit_type_table[_UNITS[unit_idx].type].Hits;

    if((_UNITS[unit_idx].enchantments & UE_BLACKCHANNELS) != 0)
    {
        hit_points += 1;
    }

    if((_UNITS[unit_idx].enchantments & UE_LIONHEART) != 0)
    {
        hit_points += 3;
    }

    /*
        BEGIN: Unit is Hero-Unit
    */
    if(_UNITS[unit_idx].Hero_Slot > ST_UNDEFINED)
    {
        if(unit_level > 0)
        {
            hit_points += 1;
        }

        if(HERO_CONSTITUTION(_UNITS[unit_idx].owner_idx, _UNITS[unit_idx].type))
        {
            hit_points += unit_level + 1;
        }

        if(HERO_CONSTITUTION2(_UNITS[unit_idx].owner_idx, _UNITS[unit_idx].type))
        {
            hit_points += (((unit_level + 1) * 3) / 2);
        }

        if(unit_level > 1)
        {
            hit_points += 1;
        }

        if(unit_level > 3)
        {
            hit_points += 1;
        }

    }
    /*
        END: Unit is Hero-Unit
    */

    if(unit_level > 2)
    {
        hit_points += 1;
    }

    if(unit_level > 4)
    {
        hit_points += 1;
    }

    if(unit_level > 5)
    {
        hit_points += 1;
    }

    if(unit_level > 6)
    {
        hit_points += 1;
    }

    if(unit_level > 7)
    {
        hit_points += 1;
    }

    if(_players[_UNITS[unit_idx].owner_idx].Globals[CHARM_OF_LIFE] > 0)
    {
        Charm_of_Life_Bonus = hit_points / 4;
        if(Charm_of_Life_Bonus < 1)
        {
            Charm_of_Life_Bonus = 1;
        }
        hit_points += Charm_of_Life_Bonus;
    }

    return hit_points;
}

// WZD o116p05
int16_t Battle_Unit_Hit_Points(struct s_BATTLE_UNIT * battle_unit)
{
    int16_t Charm_of_Life_Hits;
    int16_t unit_level;
    uint32_t battle_unit_enchantments;
    int16_t unit_idx;
    int16_t hit_points;  // _DI_

    unit_idx = battle_unit->unit_idx;

    unit_level = _UNITS[unit_idx].Level;

    battle_unit_enchantments = _UNITS[unit_idx].enchantments | battle_unit->enchantments | battle_unit->item_enchantments;

    if(
        ((battle_unit_enchantments & UE_HEROISM) != 0)
        &&
        (unit_level < 3)
    )
    {
        unit_level = 3;
    }

    if(
        (_players[battle_unit->controller_idx].Globals[CRUSADE] > 0)
        &&
        ((_unit_type_table[unit_idx].Abilities & UA_FANTASTIC) == 0)
    )
    {
        unit_level++;
    }

    hit_points = _unit_type_table[_UNITS[unit_idx].type].Hits;

    if((battle_unit_enchantments & UE_BLACKCHANNELS) != 0)
    {
        hit_points += 1;
        battle_unit->Gold_Hits += 1;
    }

    if((battle_unit_enchantments & UE_LIONHEART) != 0)
    {
        hit_points += 3;
        battle_unit->Gold_Hits += 3;
    }

    if(_UNITS[unit_idx].Hero_Slot != -1)
    {
        if(unit_level > 0)
        {
            hit_points += 1;
        }

        if(HERO_CONSTITUTION(_UNITS[unit_idx].owner_idx, _UNITS[unit_idx].type))
        {
            hit_points += (unit_level + 1);
        }

        if(HERO_CONSTITUTION2(_UNITS[unit_idx].owner_idx, _UNITS[unit_idx].type))
        {
            hit_points += (((unit_level + 1) * 3) / 2);
        }

        if(unit_level > 1)
        {
            hit_points += 1;
        }

        if(unit_level > 3)
        {
            hit_points += 1;
        }
    }  /* if(_UNITS[unit_idx].Hero_Slot != -1) */

    if(unit_level > 2)
    {
        hit_points += 1;
    }

    if(unit_level > 4)
    {
        hit_points += 1;
    }

    if(unit_level > 5)
    {
        hit_points += 1;
    }

    if(unit_level > 6)
    {
        hit_points += 1;
    }

    if(unit_level > 7)
    {
        hit_points += 1;
    }

    if(_players[battle_unit->controller_idx].Globals[CHARM_OF_LIFE] > 0)
    {
        Charm_of_Life_Hits = (hit_points / 4);
        
        SETMIN(Charm_of_Life_Hits, 1);

        hit_points += Charm_of_Life_Hits;
    }

    hit_points += battle_unit->Extra_Hits;

    battle_unit->Gold_Hits += battle_unit->Extra_Hits;

    return hit_points;
}


// WZD o116p06
// MoO2  Module: COMBINIT  Load_Combat_Ship_()
/*
; creates a battle unit record for the specified unit,
; calculating and setting its overland statistics, and
; saving the struct to the passed destination pointer
*/
void Load_Battle_Unit(int16_t unit_idx, struct s_BATTLE_UNIT * battle_unit)
{
    int16_t Item_Charges;
    int16_t itr;  //  _DI_

    memcpy(battle_unit, &_unit_type_table[_UNITS[unit_idx].type].Melee, sizeof(struct s_UNIT_TYPE));

    battle_unit->Combat_Effects = 0;

    battle_unit->Melee_To_Hit = 0;
    battle_unit->Ranged_To_Hit = 0;

    battle_unit->tohit = 0;  // set in BU_Init_Battle_Unit()

    battle_unit->To_Block = 0;

    battle_unit->Weapon_Plus1 = 0;
    battle_unit->Melee_ATK_Flags = 0;
    battle_unit->Ranged_ATK_Flags = 0;
    battle_unit->item_enchantments = 0;
    battle_unit->Extra_Hits = 0;
    battle_unit->unit_idx = unit_idx;
    battle_unit->Web_HP = 0;

    battle_unit->Gold_Melee = 0;
    battle_unit->Gold_Ranged = 0;
    battle_unit->Gold_Defense = 0;
    battle_unit->Gold_Resist = 0;
    battle_unit->Gold_Hits = 0;
    battle_unit->Grey_Melee = 0;
    battle_unit->Grey_Ranged = 0;
    battle_unit->Grey_Defense = 0;
    battle_unit->Grey_Resist = 0;

    battle_unit->Fig_IMG_Index = ST_UNDEFINED;

    battle_unit->Status = 0;

    battle_unit->controller_idx = _UNITS[unit_idx].owner_idx;

    for(itr = 0; itr < 3; itr++)
    {
        battle_unit->damage[itr] = 0;
    }

    battle_unit->enchantments = 0;
    battle_unit->Suppression = 0;
    battle_unit->mana_max = 0;
    battle_unit->Item_Charges = 0;

    battle_unit->Target_BU = ST_UNDEFINED;

    battle_unit->Poison_Strength = 0;

    battle_unit->upkeep = Unit_Gold_Upkeep(unit_idx);

    if((battle_unit->Attack_Flags & 0x04 /* Att_Poison */) != 0)
    {
        battle_unit->Poison_Strength = battle_unit->Spec_Att_Attrib;
    }

    if((battle_unit->ranged_type & 0x68 /* SR_MultiGaze */) != 0)
    {
        battle_unit->Spec_Att_Attrib = 0;
    }

    BU_Init_Battle_Unit(battle_unit);

    battle_unit->mana = battle_unit->mana_max;

    battle_unit->damage[0] = _UNITS[unit_idx].Damage;

    battle_unit->TopFig_Dmg = (_UNITS[unit_idx].Damage % battle_unit->hits);

    battle_unit->Cur_Figures = (battle_unit->Max_Figures - (_UNITS[unit_idx].Damage / battle_unit->hits));

    battle_unit->Atk_FigLoss = 0;
    battle_unit->Confusion_State = 0;
    battle_unit->Blood_Amount = 0;

    battle_unit->Unknown_5A = 0;  // NOTE: Per the Dasm, duplicate `battle_unit->Unknown_5A = 0;`
    battle_unit->Unknown_5A = 0;

    battle_unit->Melee_Anim = 0;
    battle_unit->UE_Color = 0;
    battle_unit->MoveStage = 0;
    battle_unit->Moving = 0;
    battle_unit->Action = 0;
    battle_unit->Always_Animate = 0;
    battle_unit->Image_Effect = 0;
    battle_unit->Move_Bob = 0;

    // ¿ BUG:  only gets item charges for 1/first item ?
    // Macro:  Unit Is Hero Unit
    // if(HEROSLOT() > ST_UNDEFINED)
    // if(YAYHERO())
    if(_UNITS[unit_idx].Hero_Slot > ST_UNDEFINED)
    {
        // Macro:  Hero Slot (Index)  _players[].Heroes[]
        // if(_players[_UNITS[unit_idx].owner_idx].Heroes[HEROSLOT()].Items[0] > ST_UNDEFINED)
        if(_players[_UNITS[unit_idx].owner_idx].Heroes[_UNITS[unit_idx].Hero_Slot].Items[0] > -1)
        {
            // Macro:  Hero Item (Index)  _ITEMS[]
            // Item_Charges = _ITEMS[_players[UNITOWNER()].Heroes[HEROSLOT()].Items[0]].embed_spell_cnt;
            Item_Charges = _ITEMS[_players[_UNITS[unit_idx].owner_idx].Heroes[_UNITS[unit_idx].Hero_Slot].Items[0]].embed_spell_cnt;
        }
        else
        {
            Item_Charges = 0;
        }

        if(Item_Charges > 0)
        {
            battle_unit->Item_Charges = Item_Charges;
        }
    }

}

// WZD o116p07
/*
    base from copy of Unit Type
    calls out for Level Stats, Hero Ability Stats, Item Stats, Enchantment Stats
    
    ...
    Chaos Surge
    ...

    ...
    movement_points
    Hits
*/
void BU_Init_Battle_Unit(struct s_BATTLE_UNIT * battle_unit)
{
    int16_t weapon_quality;
    int8_t var_8;
    int8_t unit_mutations;
    uint32_t battle_unit_enchantments;
    int16_t chaos_surge;
    int16_t unit_idx;  //  _SI_
    int16_t itr_players;  // _DI_

    unit_idx = battle_unit->unit_idx;

    unit_mutations = _UNITS[unit_idx].mutations;

    battle_unit->tohit = _unit_type_table[_UNITS[battle_unit->unit_idx].type].To_Hit;

    battle_unit->Melee_To_Hit = 0;
    battle_unit->Ranged_To_Hit = 0;
    battle_unit->To_Block = 0;

    battle_unit->Gold_Melee = 0;
    battle_unit->Gold_Ranged = 0;
    battle_unit->Gold_Defense = 0;
    battle_unit->Gold_Resist = 0;
    battle_unit->Gold_Hits = 0;
    battle_unit->Grey_Melee = 0;
    battle_unit->Grey_Ranged = 0;
    battle_unit->Grey_Defense = 0;
    battle_unit->Grey_Resist = 0;

    battle_unit->resist = _unit_type_table[_UNITS[battle_unit->unit_idx].type].Resist;

    battle_unit->defense = _unit_type_table[_UNITS[battle_unit->unit_idx].type].Defense;

    battle_unit->melee = _unit_type_table[_UNITS[battle_unit->unit_idx].type].Melee;

    battle_unit->ranged = _unit_type_table[_UNITS[battle_unit->unit_idx].type].Ranged;

    if(BU_CASTER_40())
    {
        battle_unit->mana_max = 40;
    }

    if(BU_CASTER_20())
    {
        battle_unit->mana_max = 20;
    }

    BU_Apply_Level(unit_idx, battle_unit);

    if(BU_LUCKY())
    {
        battle_unit->tohit += 1;
        battle_unit->To_Block += 1;
        battle_unit->resist += 1;
        battle_unit->Gold_Resist += 1;
    }

    /*
        Unit is a Hero Unit
    */
    if(_UNITS[unit_idx].Hero_Slot > -1)
    {
        if(battle_unit->Weapon_Plus1 == 0)
        {
            battle_unit->Weapon_Plus1 = 1;
        }

        BU_Init_Hero_Unit(unit_idx, battle_unit);

        BU_Apply_Item_Powers(unit_idx, battle_unit);
            // |-> 
            // |-> 
    }


    battle_unit_enchantments = battle_unit->item_enchantments | _UNITS[unit_idx].enchantments;


    weapon_quality = (_UNITS[unit_idx].mutations & 0x03);  // ; mask first 3 bits

    if(weapon_quality > 0)
    {
        if(battle_unit->melee > 0)
        {
            battle_unit->melee += (weapon_quality - 1);
            battle_unit->Gold_Melee += (weapon_quality - 1);
            battle_unit->Melee_To_Hit += 1;
        }

        if(
            ((battle_unit->ranged_type / 10) == rag_Missile)
            ||
            ((battle_unit->ranged_type / 10) == rag_Boulder)
            ||
            (battle_unit->ranged_type == srat_Thrown)
        )
        {
            battle_unit->ranged += (weapon_quality - 1);
            battle_unit->Gold_Ranged += (weapon_quality - 1);
            battle_unit->Ranged_To_Hit += 1;
        }

            battle_unit->defense += (weapon_quality - 1);
            battle_unit->Gold_Defense += (weapon_quality - 1);
            battle_unit->Weapon_Plus1 = (weapon_quality + 1);
    }


    // drake178:  BUG: only checks the human player's global enchantments in each iteration of the player loop
    chaos_surge = ST_FALSE;
    for(itr_players = 0; itr_players < NUM_PLAYERS; itr_players++)
    {
        if(_players[HUMAN_PLAYER_IDX].Globals[CHAOS_SURGE] > 0)
        {
            chaos_surge = ST_TRUE;
        }
    }

    if(chaos_surge == ST_TRUE)
    {
        if(battle_unit->race == rt_Chaos)
        {
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged += 2;
                battle_unit->Gold_Ranged += 2;
            }
            if(battle_unit->melee > 0)
            {
                battle_unit->melee += 2;
                battle_unit->Gold_Melee += 2;
            }
        }
    }

    if(_players[_UNITS[unit_idx].owner_idx].Globals[HOLY_ARMS] > 0)
    {
        if(
            ((battle_unit->Abilities & UA_FANTASTIC) != 0)
        )
        {
            if((unit_mutations & UM_UNDEAD) != 0)
            {
                if((battle_unit_enchantments & UE_HOLYWEAPON) != 0)
                {
                    battle_unit->enchantments |= UE_HOLYWEAPON;
                    battle_unit->Melee_To_Hit += 1;

                    if(
                        (battle_unit->ranged_type == srat_Thrown)
                        ||
                        ((battle_unit->ranged_type / 10) == rag_Boulder)
                        ||
                        ((battle_unit->ranged_type / 10) == rag_Missile)
                    )
                    {
                        battle_unit->Ranged_To_Hit += 1;
                    }

                    if(battle_unit->Weapon_Plus1 == 0)
                    {
                        battle_unit->Weapon_Plus1 = 1;
                    }
                }
            }
        }
    }

    if(
        ((battle_unit->Abilities & UA_FANTASTIC) != 0)
        ||
        (battle_unit->race > rt_Troll)
    )
    {
        if(battle_unit->Weapon_Plus1 == 0)
        {
            battle_unit->Weapon_Plus1 = 1;
        }
    }

    if((battle_unit->Abilities & UA_NONCORPOREAL) != 0)
    {
        battle_unit->Move_Flags |= MV_SWIMMING;
    }


    BU_Apply_Specials(battle_unit, battle_unit_enchantments, unit_mutations);
    

    battle_unit->movement_points = Unit_Moves2(unit_idx);

    battle_unit_enchantments = battle_unit->item_enchantments | _UNITS[unit_idx].enchantments;


    // ¿ maybe, but, also, why is this even here ?
    // drake178:  ; BUG: value not in halves, but if it was, it would cause a display bug below
    //              this entire check is completely redundant, as the above function already incorporates it,
    //              while any overland movement calculations will IGNORE this value,
    //              causing a discrepancy between the displayed and the actual movement allowance of the unit
    //              it could also only ever trigger if an opposing Wind Mastery has reduced the moves of a ship with Flight
    if(battle_unit->movement_points < 3)
    {
        if((battle_unit_enchantments & UE_FLIGHT) != 0)
        {
            // drake178:  ; BUG: could override Wind Mastery malus overland, but
            battle_unit->movement_points = 6;
        }
    }


    battle_unit->hits = Unit_Hit_Points(unit_idx);

}


// WZD o116p08
void BU_Apply_Specials(struct s_BATTLE_UNIT * battle_unit, uint32_t battle_unit_enchantments, uint8_t unit_mutations)
{

    if((battle_unit_enchantments & UE_WATERWALKING) != 0)
    {
        battle_unit->Move_Flags |= MV_SWIMMING;
    }

    if((battle_unit_enchantments & UE_TRUESIGHT) != 0)
    {
        battle_unit->Attribs_1 |= USA_IMMUNITY_ILLUSION;
    }

    if((battle_unit_enchantments & UE_INVULNERABILITY) != 0)
    {
        battle_unit->Attribs_1 |= USA_IMMUNITY_WEAPON;
    }

    if((battle_unit_enchantments & UE_FLIGHT) != 0)
    {
        battle_unit->Move_Flags |= MV_FLYING;
    }

    if((battle_unit_enchantments & UE_WRAITHFORM) != 0)
    {
        battle_unit->Abilities |= UA_NONCORPOREAL;
        battle_unit->Attribs_1 |= USA_IMMUNITY_WEAPON;
        battle_unit->Move_Flags |= MV_SWIMMING;
    }

    if((unit_mutations & UM_UNDEAD) != 0)
    {
        battle_unit->race = rt_Death;
        battle_unit->Abilities |= UA_FANTASTIC;
    }

    if((battle_unit_enchantments & UE_BLACKCHANNELS) != 0)
    {
        if(battle_unit->melee > 0)        
        {
            battle_unit->melee += 2;
            battle_unit->Gold_Melee += 2;
        }

        if(battle_unit->ranged != -1)
        {
            battle_unit->ranged += 1;
            battle_unit->Gold_Ranged += 1;
        }

        battle_unit->defense += 1;
        battle_unit->Gold_Defense += 1;
        battle_unit->resist += 1;
        battle_unit->Gold_Resist += 1;

        battle_unit->race = rt_Death;
        battle_unit->Abilities |= UA_FANTASTIC;

        battle_unit->Attribs_1 |= (USA_IMMUNITY_ILLUSION | USA_IMMUNITY_COLD | USA_IMMUNITY_POISON);

    }

    if((battle_unit_enchantments & UE_IRONSKIN) != 0)
    {
        battle_unit->defense += 5;
        battle_unit->Gold_Defense += 5;
    }
    else if((battle_unit_enchantments & UE_IRONSKIN) != 0)
    {
        battle_unit->defense += 1;
        battle_unit->Gold_Defense += 1;
    }

    if((battle_unit_enchantments & UE_GUARDIANWIND) != 0)
    {
        battle_unit->Attribs_1 |= USA_IMMUNITY_MISSILES;
    }

    if((battle_unit_enchantments & UE_MAGICIMMUNITY) != 0)
    {
        battle_unit->Attribs_1 |= USA_IMMUNITY_MAGIC;
    }

    if((battle_unit_enchantments & UE_FLAMEBLADE) != 0)
    {
        if(battle_unit->melee > 0)
        {
            battle_unit->melee += 2;
            battle_unit->Gold_Melee += 2;
        }

        if(
            (battle_unit->ranged_type == srat_Thrown)
            ||
            ((battle_unit->ranged_type / 10) == rag_Missile)
        )
        {
            battle_unit->ranged += 2;
            battle_unit->Gold_Ranged += 2;
        }
    }

    if((battle_unit_enchantments & UE_GIANTSTRENGTH) != 0)
    {
        if(battle_unit->melee > 0)
        {
            battle_unit->melee += 1;
            battle_unit->Gold_Melee += 1;
        }

        if(battle_unit->ranged_type == srat_Thrown)
        {
            battle_unit->ranged += 1;
            battle_unit->Gold_Ranged += 1;
        }
    }

    if((battle_unit_enchantments & UE_IMMOLATION) != 0)
    {
        battle_unit->Attribs_2 |= USA_IMMOLATION;
    }

    if((battle_unit_enchantments & UE_ELDRITCHWEAPON) != 0)
    {
        battle_unit->Melee_ATK_Flags |= Att_EldrWeap;

        if(
            ((battle_unit->ranged_type / 10) == rag_Missile)
            ||
            (battle_unit->ranged_type == srat_Thrown)
        )
        {
            battle_unit->Ranged_ATK_Flags |= Att_EldrWeap;
        }

        if(battle_unit->Weapon_Plus1 == 0)
        {
            battle_unit->Weapon_Plus1 = 1;
        }
    }

    if((unit_mutations & CC_ARMOR) != 0)
    {
        battle_unit->defense += 3;
        battle_unit->Gold_Defense += 3;
        battle_unit->race = rt_Chaos;
    }

    if((unit_mutations & CC_FLIGHT) != 0)
    {
        battle_unit->Move_Flags |= MV_FLYING;
        battle_unit->race = rt_Chaos;
    }

    if((unit_mutations & CC_BREATH) != 0)
    {
        battle_unit->ranged = 2;
        battle_unit->ranged_type = srat_FireBreath;
        battle_unit->race = rt_Chaos;
    }

    if((battle_unit_enchantments & UE_LIONHEART) != 0)
    {
        if(battle_unit->melee > 0)
        {
            battle_unit->melee += 3;
            battle_unit->Gold_Melee += 3;
        }

        if(
            ((battle_unit->ranged_type / 10) == rag_Missile)
            ||
            ((battle_unit->ranged_type / 10) == rag_Boulder)
            ||
            (battle_unit->ranged_type == srat_Thrown)
        )
        {
            battle_unit->ranged += 3;
            battle_unit->Gold_Ranged += 3;
        }

        battle_unit->resist += 3;
        battle_unit->Gold_Resist += 3;
    }

    if((battle_unit_enchantments & UE_HOLYARMOR) != 0)
    {
        battle_unit->defense += 2;
        battle_unit->Gold_Defense += 2;
    }

    // drake178:  ; BUG: undead are also supposed to gain Cold, Poison, and Illusions Immunities (native Death units have all of these anyway)
    if(battle_unit->race == rt_Death)
    {
        battle_unit->Attribs_1 |= USA_IMMUNITY_DEATH;
    }

    if((battle_unit_enchantments & UE_BERSERK) != 0)
    {
        battle_unit->Gold_Melee = battle_unit->melee;
        battle_unit->melee *= 2;
        battle_unit->Grey_Defense = battle_unit->defense;
        battle_unit->defense = -20;
    }

}


// WZD o116p09
void BU_Apply_Level(int16_t unit_idx, struct s_BATTLE_UNIT * battle_unit)
{
    int16_t unit_level;  // _SI_

    unit_level = _UNITS[unit_idx].Level;

    if(
        ((_UNITS[unit_idx].enchantments & UE_HEROISM) != 0)
        ||
        ((battle_unit->enchantments & UE_HEROISM) != 0)
    )
    {
        if(unit_level < 3)
        {
            unit_level = 3;
        }
    }


    if(_UNITS[unit_idx].Hero_Slot > -1)
    {
        if(unit_level > 0)
        {
            battle_unit->resist++;
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged++;
            }
            if(battle_unit->melee > 0)
            {
                battle_unit->melee++;
            }
            battle_unit->defense++;
        }

        if(unit_level > 1)
        {
            battle_unit->tohit++;
            battle_unit->resist++;
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged++;
            }
            if(battle_unit->melee > 0)
            {
                battle_unit->melee++;
            }
        }

        if(unit_level > 2)
        {
            battle_unit->resist++;
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged++;
            }
            if(battle_unit->melee > 0)
            {
                battle_unit->melee++;
            }
            battle_unit->defense++;
        }

        if(unit_level > 3)
        {
            battle_unit->resist++;
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged++;
            }
            if(battle_unit->melee > 0)
            {
                battle_unit->melee++;
            }
        }

        if(unit_level > 4)
        {
            battle_unit->tohit++;
            battle_unit->resist++;
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged++;
            }
            if(battle_unit->melee > 0)
            {
                battle_unit->melee++;
            }
            battle_unit->defense++;
        }

        if(unit_level > 5)
        {
            battle_unit->resist++;
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged++;
            }
            if(battle_unit->melee > 0)
            {
                battle_unit->melee++;
            }
        }

        if(unit_level > 6)
        {
            battle_unit->resist++;
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged++;
            }
            if(battle_unit->melee > 0)
            {
                battle_unit->melee++;
            }
            battle_unit->defense++;
        }

        if(unit_level > 7)
        {
            battle_unit->tohit++;
            battle_unit->resist++;
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged++;
            }
            if(battle_unit->melee > 0)
            {
                battle_unit->melee++;
            }
        }
    }
    else
    {
        if(unit_level > 0)
        {
            battle_unit->resist++;
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged++;
            }
            if(battle_unit->melee > 0)
            {
                battle_unit->melee++;
            }
        }

        if(unit_level > 1)
        {
            battle_unit->resist++;
            battle_unit->defense++;
        }

        if(unit_level > 2)
        {
            battle_unit->tohit++;
            battle_unit->resist++;
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged++;
            }
            if(battle_unit->melee > 0)
            {
                battle_unit->melee++;
            }
        }

        if(unit_level > 3)
        {
            battle_unit->tohit++;
            battle_unit->resist++;
            battle_unit->defense++;
        }

        if(unit_level > 4)
        {
            battle_unit->tohit++;
            battle_unit->resist++;
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged++;
            }
            if(battle_unit->melee > 0)
            {
                battle_unit->melee++;
            }
        }
    }

}


// WZD o116p10
/*
    ¿ overwrites much of what was just done ?

    upkeep

*/
void BU_Init_Hero_Unit(int16_t unit_idx, struct s_BATTLE_UNIT * battle_unit)
{
    int16_t hero_owner_idx;  // _SI_
    int16_t hero_type;  // _DI_

    hero_owner_idx = UNITOWNER();
    hero_type = UNITTYPE();
    
    if(
        (HERO_NOBLE(hero_owner_idx, hero_type))
        ||
        (_players[hero_owner_idx].famous > ST_FALSE)
    )
    {
        battle_unit->upkeep = 0;
    }


    if (HERO_AGILITY(hero_owner_idx, hero_type))
    {
        battle_unit->defense = (battle_unit->defense + (_UNITS[unit_idx].Level + 1));
    }
    
    if(HERO_AGILITY2(hero_owner_idx,hero_type))
    {
        battle_unit->defense = (battle_unit->defense + (((_UNITS[unit_idx].Level + 1) * 3) / 2));
    }

    if(HERO_BLADEMASTER(hero_owner_idx,hero_type))
    {
        battle_unit->tohit = (battle_unit->tohit + ((_UNITS[unit_idx].Level + 1) / 2));
    }
    
    if(HERO_BLADEMASTER2(hero_owner_idx,hero_type))
    {
        battle_unit->tohit = (battle_unit->tohit + (((_UNITS[unit_idx].Level + 1) * 3) / 4));
    }

    if(HERO_MIGHT(hero_owner_idx,hero_type))
    {
        battle_unit->melee = (battle_unit->melee + (_UNITS[unit_idx].Level + 1));
    }
    
    if(HERO_MIGHT2(hero_owner_idx,hero_type))
    {
        battle_unit->melee = (battle_unit->melee + (((_UNITS[unit_idx].Level + 1) * 3) / 2));
    }


    if(
        (HERO_ARCANE_POWER(hero_owner_idx,hero_type))
        &&
        ((battle_unit->ranged_type / 10) == rag_Magic)
    )
    {
        battle_unit->ranged = (battle_unit->ranged + (_UNITS[unit_idx].Level + 1));
    }
    
    if(
        (HERO_ARCANE_POWER2(hero_owner_idx,hero_type))
        &&
        ((battle_unit->ranged_type / 10) == rag_Magic)
    )
    {
        battle_unit->ranged = (battle_unit->ranged + (((_UNITS[unit_idx].Level + 1) * 3) / 2));
    }

    if(_HEROES2[hero_owner_idx]->heroes[hero_type].Casting_Skill > 0)
    {
        battle_unit->mana_max = ((((_HEROES2[hero_owner_idx]->heroes[hero_type].Casting_Skill + 1) * (_UNITS[unit_idx].Level + 1)) * 5) / 2);
    }
    else
    {
        battle_unit->mana_max = 0;
    }

    if(HERO_LUCKY(hero_owner_idx,hero_type))
    {
        battle_unit->tohit += 1;
        battle_unit->To_Block += 1;
        battle_unit->resist += 1;
        battle_unit->Gold_Resist += 1;
    }

}


// WZD o116p11
/*
    Magic Node Combat Effect



Page 98  (PDF Page 103)
STRUCTURES INFLUENCING COMBAT
Magic Nodes
Magic nodes can affect combat in one of two ways.
They may increase (by one) the attack strengths (swords and ranged weapons), defenses (shields) and resistance (crosses) of all creatures from the same magic realm as the node
  (i.e., a nature node would exert this positive effect on all creatures of nature such as sprites and basilisks)
  when these creatures are fighting in any area controlled by the node.

*/
void BU_Apply_Battlefield_Effects__WIP(struct s_BATTLE_UNIT * battle_unit)
{
    int16_t Node_Aura_Applies;
    int16_t Item_Index;
    int16_t Mutation_Flags;
    uint32_t enchantments;
    int16_t unit_idx;

    unit_idx = battle_unit->unit_idx;

    Node_Aura_Applies = ST_FALSE;

    if(
        (
            (CMB_node_type == cnt_Sorcery)
            &&
            (battle_unit->race == rt_Sorcery)
        )
        ||
        (
            (CMB_node_type == cnt_Chaos)
            &&
            (battle_unit->race == rt_Chaos)
        )
        ||
        (
            (CMB_node_type == cnt_Chaos)
            &&
            (battle_unit->race == rt_Chaos)
        )
    )
    {
        Node_Aura_Applies = ST_TRUE;
    }

    if(Node_Aura_Applies == ST_TRUE)
    {
        battle_unit->melee += 2;
        battle_unit->Gold_Melee += 2;

        if(battle_unit->ranged > 0)
        {
            battle_unit->ranged += 2;
            battle_unit->Gold_Ranged += 2;
        }

        battle_unit->resist += 2;
        battle_unit->Gold_Resist += 2;

        battle_unit->defense += 2;
        battle_unit->Gold_Defense += 2;
    }





}


// WZD o116p12
// sub_9A43E()



/*
    WIZARDS.EXE  ovr163
*/

// WZD ovr163p01
// CMB_LoadTerrainGFX()

// WZD ovr163p02
// CMB_ComposeBackgrnd()

// WZD ovr163p03
// CMB_LoadWallRiseGFX()

// WZD ovr163p04
/*

; clears the sandbox and the LBX near buffer, and
; allocates into the former the battlefield structure
; and the sprite, projectile, and vortex arrays (351h
; paragraphs in total); and the latter the pathfinding
; arrays which are entirely useless here as they will
; be redone by another function in the current code

*/
void CMB_BaseAllocs(void)
{

    battlefield = (struct s_BATTLEFIELD *)Allocate_First_Block(_screen_seg, 348);  // 348 PR, 5568 B
    CMB_EntitySprites = Allocate_Next_Block(_screen_seg, 482);
    CMB_Projectiles = Allocate_Next_Block(_screen_seg, 10);
    CMB_Projectiles = Allocate_Next_Block(_screen_seg, 10);
    CMB_Vortex_Array = Allocate_Next_Block(_screen_seg, 9);

    // ¿ drake178:  ; WARNING: these are entirely redundant and will be  ; reallocated immediately after this!
    CMB_ActiveMoveMap = Near_Allocate_First(504);
    CMB_Path_Costs = Near_Allocate_Next(504);
    CMB_NearBuffer_3 = Near_Allocate_Next(1008);
    CMB_Path_Xs = Near_Allocate_Next(504);
    CMB_Path_Ys = Near_Allocate_Next(504);

}


// WZD ovr163p05
// CMB_SetProjectiles()

// WZD ovr163p06
// EMM_LoadFigureGFX()
