
/*
    WIZARDS.EXE
        ovr061

MoO2
    Module: SHIPSTK
    Module: SHIPMOVE

*/

#include "MoM.H"



/*
    WIZARDS.EXE  ovr061
*/

// WZD o61p01
// AKA IDK_ActiveUnitStack_MovesOrPath_s53150()
/*
    Handles a left-click on a Unit in the Unit Window
    
    Mostly Just:
        _unit_stack[stack_idx].active = ST_FALSE || ST_TRUE

    May
        _active_stack_has_path = ST_FALSE;
        _UNITS[stack_unit_idx].Status = us_Ready;
        _UNITS[stack_unit_idx].Finished = ST_FALSE;
        _UNITS[stack_unit_idx].Rd_Constr_Left = ST_UNDEFINED;
        _UNITS[stack_unit_idx].Finished = ST_TRUE;
        all_units_moved = ST_FALSE;
*/
void Update_Stack_Active(int16_t stack_idx)
{
    int16_t DEPR_Local_Flag;
    int16_t itr_unit_stack_count;
    int16_t active_unit_stack_count;
// stack_idx= word ptr  6

    int16_t stack_unit_idx;
    int16_t itr_stack;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Update_Stack_Active()\n", __FILE__, __LINE__);
#endif

    stack_unit_idx = _unit_stack[stack_idx].unit_idx;

    if(
        (_UNITS[stack_unit_idx].Status != us_ReachedDest)  /* "DONE" */ &&
        (_UNITS[stack_unit_idx].moves2 >= 1)
    )
    {

        if(
            (reset_active_stack == ST_TRUE) &&
            (_unit_stack_count > 1) &&
            (_UNITS[stack_unit_idx].Status != us_Casting)
        )
        {
            active_unit_stack_count = 0;
            for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
            {
                if(_unit_stack[itr_stack].active == ST_TRUE)
                {
                    active_unit_stack_count++;
                }
            }

            if(active_unit_stack_count > 1)
            {
                for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
                {
                    _unit_stack[itr_stack].active = ST_FALSE;
                }
            }


        }

        reset_active_stack = ST_FALSE;
        stack_unit_idx = _unit_stack[stack_idx].unit_idx;

        if(_unit_stack[stack_idx].active == ST_FALSE)
        {
            if(_UNITS[stack_unit_idx].Status > 0 /* us_Ready */)
            {
                if(_UNITS[stack_unit_idx].moves2 > 0)
                {
                    DEPR_Local_Flag = ST_TRUE;

                    if(DEPR_Local_Flag == ST_TRUE)
                    {
                        _unit_stack[stack_idx].active = ST_TRUE;

                        if(_UNITS[stack_unit_idx].Status == us_GOTO)
                        {
                            _active_stack_has_path = ST_FALSE;
                        }

                        _UNITS[stack_unit_idx].Status = us_Ready;
                        _UNITS[stack_unit_idx].Finished = ST_FALSE;
                        _UNITS[stack_unit_idx].Rd_Constr_Left = ST_UNDEFINED;

                        if(_UNITS[stack_unit_idx].moves2 >= 1)
                        {
                            all_units_moved = ST_FALSE;
                            Reset_Draw_Active_Stack(); 
                        }
                        else
                        {
                            _unit_stack[stack_idx].active = ST_FALSE;
                            _UNITS[stack_unit_idx].Finished = ST_TRUE;
                        }

                    }
                }
            }
            else
            {
                if(_UNITS[stack_unit_idx].moves2 <= 0)
                {
                    _unit_stack[stack_idx].active = ST_FALSE;
                }
                else
                {
                    _unit_stack[stack_idx].active = ST_TRUE;
                }
            }
        }
        else
        {
            _unit_stack[stack_idx].active = ST_FALSE;
        }



        if(_UNITS[stack_unit_idx].Finished == ST_TRUE)
        {
            _unit_stack[stack_idx].active = ST_FALSE;
        }

        Stack_Moves_Active();
        Set_Unit_Action_Special();
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Update_Stack_Active()\n", __FILE__, __LINE__);
#endif
}



// WZD o61p02
// AKA Do_Move_Stack()
int16_t Move_Stack(int16_t move_x, int16_t move_y, int16_t player_idx, int16_t * map_x, int16_t * map_y, int16_t * map_p)
{
    int16_t unit_array[9];
    int16_t unit_array_count;
    int16_t unit_p;
    int16_t unit_y;
    int16_t unit_x;
    int16_t movement_points_available;
    int16_t move_type;
    int16_t unit_idx;

    int16_t itr_units;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Move_Stack(move_x = %d, move_y = %d, player_idx = %d, *map_x = %d, *map_y = %d, *map_p = %d)\n", __FILE__, __LINE__, move_x, move_y, player_idx, *map_x, *map_y, *map_p);
#endif

    move_type = 0;

    if(move_y != WORLD_Y_MIN && move_y != WORLD_Y_MAX)
    {
        Build_Moveable_Stack(&unit_array_count, &unit_array[0]);

        _unit = unit_array[0];

// DONT          if(OVL_MapVar3 == 1)
// DONT          {
// DONT              OVL_MapVar3 = 0;
// DONT              CRP_OVL_Obstacle_Var1 = 0;
// DONT          }
// DONT          if(CRP_OVL_Obstacle_Var1 == 0)
// DONT          {
// DONT              o58p03_Empty_pFxn();
// DONT          }


        Move_Units(player_idx, move_x, move_y, move_type, map_x, map_y, *map_p, unit_array_count, &unit_array[0]);


        unit_idx = _unit;
        unit_x = _UNITS[unit_idx].wx;
        unit_y = _UNITS[unit_idx].wy;
        unit_p = _UNITS[unit_idx].wp;


        // DONT  o62p01_Empty_pFxn(player_idx);
        movement_points_available = Stack_Moves();


        // DONT  o62p01_Empty_pFxn(player_idx);
        Select_Unit_Stack(player_idx, map_x, map_y, *map_p, unit_x, unit_y);
        movement_points_available = Stack_Moves();


        if(movement_points_available < 1)
        {
            DLOG("(movement_points_available < 1)");
            for(itr_units = 0; itr_units < unit_array_count; itr_units++)
            {
                if(_UNITS[unit_array[itr_units]].Status == us_Ready)
                {
                    _UNITS[unit_array[itr_units]].Finished = ST_TRUE;
                    _UNITS[unit_array[itr_units]].Status = us_ReachedDest;
                }
            }

            WIZ_NextIdleStack(player_idx, map_x, map_y, map_p);
        }
        else
        {
            DLOG("(movement_points_available >= 1)");
        }

        Set_Unit_Draw_Priority();
        Reset_Stack_Draw_Priority();
        Set_Entities_On_Map_Window(*map_x, *map_y, *map_p);
        // DONT  fxnptr_o59p();
        Reset_Draw_Active_Stack();
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Move_Stack(move_x = %d, move_y = %d, player_idx = %d, *map_x = %d, *map_y = %d, *map_p = %d)\n", __FILE__, __LINE__, move_x, move_y, player_idx, *map_x, *map_y, *map_p);
#endif

    return ST_TRUE;
}

// WZD o61p03
int16_t RdBd_UNIT_MoveStack__WIP(int16_t player_idx, int16_t unit_idx, int16_t dst_wx, int16_t dst_wy, int16_t * map_x, int16_t * map_y, int16_t map_p)
{
    int16_t troops[MAX_STACK];
    int16_t roadbuilder_count;
    int16_t unit_wy;
    int16_t unit_wx;
    int16_t did_move_stack;
    int16_t troop_count;
    int16_t Special_Move;
    int16_t troop_unit_idx;
    int16_t itr_units;  // _SI_
    int16_t itr_troops;  // _SI_


    Build_RoadBuilder_Stack(&troop_count, &troops[0], dst_wx, dst_wy, player_idx, unit_idx);


    if(troop_count < 1)
    {
        return ST_FALSE;
    }


    roadbuilder_count = 0;

    Special_Move = 0;

    // yellow
    for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
    {
        troop_unit_idx = troops[itr_troops];
        if(_UNITS[troop_unit_idx].Rd_Constr_Left != ST_UNDEFINED)
        {
            roadbuilder_count++;
        }
    }


    if(roadbuilder_count == troop_count)
    {
        Special_Move = 2;  /* ¿ IIF 'Build Road' ? */
    }


    unit_wx = _UNITS[unit_idx].wx;
    unit_wy = _UNITS[unit_idx].wy;


    // HERE: as-is, except Special_Move is '2' if all 'road builder', rather than *normal* '0'
    did_move_stack = Move_Units(player_idx, dst_wx, dst_wy, Special_Move, map_x, map_y, map_p, troop_count, &troops[0]);


    // purple
    for(itr_units = 0; itr_units < troop_count; itr_units++)
    {
        if(
            (_UNITS[troops[itr_units]].wx == unit_wx)
            &&
            (_UNITS[troops[itr_units]].wy == unit_wy)
            &&
            (_UNITS[troops[itr_units]].Rd_Constr_Left == ST_UNDEFINED)
            &&
            ((_UNITS[troops[itr_units]].Status == us_GOTO) || (_UNITS[troops[itr_units]].Status == us_Move))
            &&
            (_UNITS[troops[itr_units]].Finished == ST_FALSE)
        )
        {
            _UNITS[troops[itr_units]].Move_Failed = ST_TRUE;
            _UNITS[troops[itr_units]].Status = us_Ready;
            _UNITS[troops[itr_units]].dst_wx = 0;
            _UNITS[troops[itr_units]].dst_wy = 0;
        }
    }


    // dark gold
    for(itr_units = 0; itr_units < troop_count; itr_units++)
    {
        if(
            (_UNITS[troops[itr_units]].owner_idx != ST_UNDEFINED)
            &&
            (_UNITS[troops[itr_units]].Rd_Constr_Left == -1)
        )
        {
            if(_UNITS[troops[itr_units]].Status == us_Ready)
            {
                // HERE: unit status is NOT "NO ORDERS"  us_Ready
                // TODO  EMM_Map_DataH();                   ; maps the EMM Data block into the page frame
                // TODO  OVL_ClearUnitPath();               ; clears the long path referenced by UNIT_OverlandPath, provided that it is in the range of the corresponding table
            }
            else
            {
                // TODO  EMM_Map_DataH();
                // TODO  OVL_StoreLongPath(player_idx, _UNITS[unit_array[itr_units]].wx, _UNITS[unit_array[itr_units]].wy, _UNITS[unit_array[itr_units]].dst_wx, _UNITS[unit_array[itr_units]].dst_wy, map_p, &MovePath_X, &MovePath_Y, &OVL_Path_Costs);
                    // ; attempts to store a multi-turn path into EMS,
                    // ; provided that both the source and destination
                    // ; locations are on it, and there is space left in the
                    // ; corresponding table
                    // ; BUG: ignores the plane when looking for a match
                    // ; WARNING: the state of the map can change by the time
                    // ;  the path is retrieved later (concept flaw)
            }
        }

    }



    if(player_idx == _human_player_idx)
    {
        // DONT  o62p01_Empty_pFxn(player_idx);
        Set_Unit_Draw_Priority();
        Reset_Stack_Draw_Priority();
        Set_Entities_On_Map_Window(*map_x, *map_y, map_p);
        Reset_Draw_Active_Stack();
    }

    return did_move_stack;
}


// WZD o61p04
/*
updates _unit

*/
void WIZ_NextIdleStack(int16_t player_idx, int16_t * map_x, int16_t * map_y, int16_t * map_p)
{
    // DONT  int16_t UU_var8;
    int16_t next_unit_dst_wy;
    int16_t next_unit_dst_wx;
    int16_t next_unit_wy;
    int16_t next_unit_wx;
    int16_t no_units_available;
    int16_t done;
    int16_t next_unit_idx;

    // DONT  UU_var8 = 0;

    no_units_available = ST_FALSE;

    done = ST_FALSE;

    Reset_Map_Draw();

    // TODO  o62p01_Empty_pFxn(player_idx);

    while(done == ST_FALSE)
    {
        // TODO  CRP_OverlandVar_3 = ST_FALSE;

        no_units_available = Next_Unit_Nearest_Available(player_idx, map_p);  // updates `_unit`

        if(_unit == 502)
        {
            // __debugbreak();
        }

        if(no_units_available == ST_TRUE)
        {
            all_units_moved = ST_TRUE;
            _active_stack_has_path = ST_FALSE;
            done = ST_TRUE;
        }
        else
        {
            next_unit_idx = _unit;  // just updated by Next_Unit_Nearest_Available()
            next_unit_wx = _UNITS[next_unit_idx].wx;
            next_unit_wy = _UNITS[next_unit_idx].wy;
            Select_Unit_Stack(player_idx, map_x, map_y, *map_p, next_unit_wx, next_unit_wy);  // calls Build_Unit_Stack() & Sort_Unit_Stack(); Sort_Unit_Stack() updates `_unit`;
        }

        // HERE: Found a Unit;  `_unit` has been updated;  and ?
        if(done == ST_FALSE)
        {
            done = ST_TRUE;

            // HERE:  Nay no_units_available / Yay GotNextUnit;  To be sure, assume we're good, but undo if this Unit is *busy* with a 'GOTO'
            // if it does have a 'GOTO' and that 'GOTO' is actually for 'Build Road', do some 'Build Road' process (which appears to update the map views)
            // or, not quite? cause the conditions are ORs?
            // @@Next_Unit_Goto
            if(_UNITS[_unit].Status == us_GOTO)
            {
                next_unit_dst_wx = _UNITS[_unit].dst_wx;
                next_unit_dst_wy = _UNITS[_unit].dst_wy;

                // TODO  CRP_OverlandVar_3 = ST_TRUE;

                // handling the end of the 'Go To' & 'Build Road'
                if(
                    (_UNITS[_unit].wx == next_unit_dst_wx)
                    &&
                    (_UNITS[_unit].wy == next_unit_dst_wy)
                    &&
                    (_UNITS[_unit].Rd_Constr_Left == ST_UNDEFINED)
                )
                {
                    _UNITS[_unit].Status = us_Ready;
                }
                else
                {
                    Allocate_Reduced_Map();
                    RdBd_UNIT_MoveStack__WIP(player_idx, _unit, next_unit_dst_wx, next_unit_dst_wy, map_x, map_y, *map_p);
                    Allocate_Reduced_Map();
                }

                done = ST_FALSE;
            }
        }

    }  /* while(done == ST_FALSE) */


    if(all_units_moved == ST_FALSE)
    {
        next_unit_idx = _unit;
        next_unit_wx = _UNITS[next_unit_idx].wx;
        next_unit_wy = _UNITS[next_unit_idx].wy;
        Select_Unit_Stack(player_idx, map_x, map_y, *map_p, next_unit_wx, next_unit_wy);  // ...calls Build_Unit_Stack() & Sort_Unit_Stack(); Sort_Unit_Stack() updates `_unit`;
    }
    else
    {
        _unit_stack_count = 0;
        Set_Draw_Active_Stack_Always();
        Set_Entities_On_Map_Window(*map_x, *map_y, *map_p);
    }

    // TODO  OVL_MapVar3 = ST_TRUE;

}


// WZD o61p05
/*
OON XREF:  WIZ_NextIdleStack()

    returns {F,T} 

    next nearest available unit
        from _active_world_x, _active_world_y
    sets _unit, _active_world_x, _active_world_y
    may set _map_plane

    ~== *available* / *selectable*
        Finished != ST_TRUE

How to not keep selecting the same waiting stack/unit?

*/
int16_t Next_Unit_Nearest_Available(int16_t player_idx, int16_t * map_plane)
{
    int16_t UU_var11;
    int16_t tried_other_plane;
    int16_t itr_wait_units;
    int16_t Closest_Active_Unit;
    int16_t Closest_Active_Dist;
    int16_t Closest_Waiting_Unit;
    int16_t Closest_Waiting_Dist;
    int16_t itr_units;
    int16_t delta;
    int16_t Return_Value;
    int16_t done;
    int16_t current_world_plane;

    current_world_plane = *map_plane;

    Closest_Waiting_Dist = 1000;
    Closest_Waiting_Unit = ST_UNDEFINED;
    Closest_Active_Dist = 1000;
    Closest_Active_Unit = ST_UNDEFINED;

    tried_other_plane = ST_FALSE;

    UU_var11 = ST_UNDEFINED;

    Return_Value = ST_FALSE;

    done = ST_FALSE;
    itr_units = 0;
    while(done == ST_FALSE)
    {
        assert(itr_units < MAX_UNIT_COUNT);
        assert(_units != 0);

        if(
            (_UNITS[itr_units].owner_idx == player_idx)
            &&
            (
                (_UNITS[itr_units].wp == current_world_plane)
                ||
                (_UNITS[itr_units].in_tower == ST_TRUE)
            )
            &&
            (_UNITS[itr_units].owner_idx != ST_UNDEFINED)  /* ¿ not dead / disbanded / banished / unsummoned ? */
            &&
            (_UNITS[itr_units].Finished == ST_FALSE)
        )
        {
            // TODO  figure out how to deal with delta being 0 for the previously select stack that set _active_world_x,y
            delta = Delta_XY_With_Wrap(_active_world_x, _active_world_y, _UNITS[itr_units].wx, _UNITS[itr_units].wy, WORLD_WIDTH);

            if(_UNITS[itr_units].Status == us_Wait)
            {
                // BUGBUG  reselects the same Unit  ... add logic/flag to allow currently selected unit to not be the next selected unit  ¿ or, just `itr_units != _unit` ?
                if(Closest_Waiting_Dist > delta)
                {
                    Closest_Waiting_Dist = delta;
                    Closest_Waiting_Unit = itr_units;
                }
            }
            else
            {
                if((_UNITS[itr_units].Status != us_Purify) && (_UNITS[itr_units].Status != us_Unknown_100))
                {
                    if(Closest_Active_Dist > delta)
                    {
                        Closest_Active_Dist = delta;
                        Closest_Active_Unit = itr_units;
                    }
                }
            }
        }

        itr_units++;
        // HERE: we've iterated through all of the units
        if(itr_units == _units)
        {
            if(Closest_Active_Unit != ST_UNDEFINED)
            {
                done = ST_TRUE;
                
                if(current_world_plane == 2)
                {
                    current_world_plane = 0;
                }

                *map_plane = current_world_plane;

                _unit = Closest_Active_Unit;

                _active_world_x = _UNITS[_unit].wx;
                _active_world_y = _UNITS[_unit].wy;
            }
            else if(Closest_Waiting_Unit != ST_UNDEFINED)
            {
                if(current_world_plane == 2)
                {
                    current_world_plane = 0;
                }

                *map_plane = current_world_plane;

                _unit = Closest_Waiting_Unit;

                done = ST_TRUE;
                
                _active_world_x = _UNITS[_unit].wx;
                _active_world_y = _UNITS[_unit].wy;

                for(itr_wait_units = 0; itr_wait_units < _units; itr_wait_units++)
                {
                    if(_UNITS[itr_wait_units].owner_idx == player_idx)
                    {
                        if(_UNITS[itr_wait_units].Status == us_Wait)
                        {
                            _UNITS[itr_wait_units].Status = us_Ready;
                        }
                    }
                }
            }
            else if(tried_other_plane != ST_TRUE)
            {
                tried_other_plane = ST_TRUE;
                current_world_plane = ((current_world_plane + 1) % 2);
                Closest_Waiting_Dist = 1000;
                Closest_Waiting_Unit = ST_UNDEFINED;
                Closest_Active_Dist = 1000;
                Closest_Active_Unit = ST_UNDEFINED;
                UU_var11 = ST_UNDEFINED;
                itr_units = 0;
            }
            else
            {
                done = ST_TRUE;
                Return_Value = ST_TRUE;  // FAILED!!  tried both planes, neither Closest_Active_Unit nor Closest_Waiting_Unit
            }
        }
    }

    return Return_Value;
}

// WZD o61p06
/*
    sets special_action_flag to {build,settle,purify,merge}
*/
void Set_Unit_Action_Special(void)
{
    int16_t troops[MAX_STACK];
    int16_t troop_count;

    special_action_flag = ST_UNDEFINED;


    Active_Unit_Stack(&troop_count, &troops[0]);

    // sets special_action_flag to 0
    Unit_Action_Special_Build(troop_count, &troops[0]);

    // sets special_action_flag to 1
    Unit_Action_Special_Settle(troop_count, &troops[0]);

    // sets special_action_flag to 2
    Unit_Action_Special_Purify(troop_count, &troops[0]);

    // sets special_action_flag to 9
    Unit_Action_Special_Meld(troop_count, &troops[0]);

}

// WZD o61p07
/*
    count of Units in Stack with 'active'
    inout of count of units
    inout of array of unit indices
*/
void Active_Unit_Stack(int16_t * troop_count, int16_t troops[])
{
    int16_t itr_stack;

    *troop_count = 0;

    for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
    {
        if(_unit_stack[itr_stack].active == ST_TRUE)
        {
            troops[*troop_count] = _unit_stack[itr_stack].unit_idx;
            *troop_count += 1;
        }
    }

}

// WZD o61p08
/*

*/
void Build_Moveable_Stack(int16_t * troop_count, int16_t troops[])
{
    int16_t moveable_troops[MAX_STACK];
    int16_t boatriders[MAX_STACK];
    int16_t transport_load;
    int16_t boatriders_idx;
    int16_t transport_capacity;
    int16_t boatrider_count;
    int16_t stack_has_transport;
    int16_t moveable_troop_count;
    int16_t l_troop_count;

    int16_t itr_stack;  // _DI_
    int16_t itr_boatriders;  // _DI_

    stack_has_transport = ST_FALSE;
    transport_capacity = 0;

    for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
    {
        if(
            (_unit_stack[itr_stack].active == ST_TRUE)
            &&
            (_UNITS[_unit_stack[itr_stack].unit_idx].Status != us_GOTO)
            &&
            (_unit_type_table[_UNITS[_unit_stack[itr_stack].unit_idx].type].Transport > 0)
        )
        {
            stack_has_transport = ST_TRUE;
            transport_capacity += _unit_type_table[_UNITS[_unit_stack[itr_stack].unit_idx].type].Transport;
        }
    }

    if(stack_has_transport == ST_TRUE)
    {
        l_troop_count = 0;
        moveable_troop_count = 0;

        for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
        {
            if(
                (_unit_stack[itr_stack].active == ST_TRUE)
                &&
                (_UNITS[_unit_stack[itr_stack].unit_idx].Status != us_GOTO)
                &&
                (_unit_type_table[_UNITS[_unit_stack[itr_stack].unit_idx].type].Transport > 0)
            )
            {
                troops[moveable_troop_count] = _unit_stack[itr_stack].unit_idx;  // just boats, active, non-goto
                moveable_troop_count++;
            }
            moveable_troops[l_troop_count] = _unit_stack[itr_stack].unit_idx;  // all, active, non-goto
            l_troop_count++;
        }

        l_troop_count = moveable_troop_count;
        
        if(Square_Has_City(_UNITS[_unit_stack[0].unit_idx].wx, _UNITS[_unit_stack[0].unit_idx].wy, _UNITS[_unit_stack[0].unit_idx].wp) == ST_UNDEFINED)
        {
            for(itr_boatriders = 0; itr_boatriders < MAX_STACK; itr_boatriders++)
            {
                boatriders[itr_boatriders] = ST_UNDEFINED;
            }

            boatrider_count = Army_Boatriders(_unit_stack_count, &moveable_troops[0], &boatriders[0]);

            if(boatrider_count > 0)
            {
                l_troop_count = moveable_troop_count;
                boatriders_idx = 0;
                transport_load = 0;

                for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
                {
                    if(boatriders[boatriders_idx] == _unit_stack[itr_stack].unit_idx)
                    {
                        if(
                            (transport_load < transport_capacity) ||
                            (_UNITS[_unit_stack[itr_stack].unit_idx].type <= ut_Chosen)  /* Hero Unit */
                        )
                        {
                            troops[l_troop_count] = _unit_stack[itr_stack].unit_idx;
                            l_troop_count++;
                            boatriders_idx++;
                            if( _UNITS[_unit_stack[itr_stack].unit_idx].type > ut_Chosen)  /* Hero Unit */
                            {
                                transport_load++;
                            }
                        }
                    }
                }

                for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
                {
                    if(
                        (Unit_Has_AirTravel_Item(_unit_stack[itr_stack].unit_idx) == ST_FALSE) &&
                        (Unit_Has_WaterTravel_Item(_unit_stack[itr_stack].unit_idx) == ST_FALSE) &&
                        (Unit_Has_Swimming(_unit_stack[itr_stack].unit_idx) == ST_FALSE) &&
                        (Unit_Has_WindWalking(_unit_stack[itr_stack].unit_idx) == ST_FALSE) &&
                        (Unit_Has_AirTravel(_unit_stack[itr_stack].unit_idx) == ST_FALSE)
                    )
                    {
                        if(_UNITS[_unit_stack[itr_stack].unit_idx].Status != us_GOTO)
                        {
                            troops[l_troop_count] = _unit_stack[itr_stack].unit_idx;
                            l_troop_count++;
                        }
                    }
                }

                goto Done;

            }
            else  /* !(boatrider_count > 0) */
            {
                // HERE: Yay TransPort, Nay City, Nay LandLubbers
                if(l_troop_count < _unit_stack_count)
                {
                    for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
                    {
                        if((_UNITS[itr_stack].Status != us_GOTO) && (_unit_type_table[_UNITS[itr_stack].type].Transport < 1))
                        {
                            troops[l_troop_count] = _unit_stack[itr_stack].unit_idx;
                            l_troop_count++;
                        }
                    }
                }
                else
                {
                    // HERE: ¿ we're on the ocean and all stack units are transports ?
                    goto Done;
                }


            }

        }
        else  /* Square_Has_City() != ST_UNDEFINED */
        {
            goto Done;
        }

    }
    else  /* (stack_has_transport != ST_TRUE) */
    {
        // HERE:  no boats means we're on land
        l_troop_count = 0;

        for(itr_stack = 0; itr_stack < _unit_stack_count; itr_stack++)
        {
            if((_unit_stack[itr_stack].active == ST_TRUE) && (_UNITS[_unit_stack[itr_stack].unit_idx].Status != us_GOTO))
            {
                troops[l_troop_count] = _unit_stack[itr_stack].unit_idx;
                l_troop_count++;
            }
        }

    }

Done:
    *troop_count = l_troop_count;

}



// WZD o61p09
/*
OON XREF:  RdBd_UNIT_MoveStack_WIP()

; select the stack of units from a tile that match the
; destination and road building left of the specified
; unit, and fill out the return values accordingly;
; accounting for transport units if the tile has any,
; and only returning these if the tile has a city on it
; BUG: considers Non_Corporeal units to be landlubbers
; that require transport to cross oceans

*/
void Build_RoadBuilder_Stack(int16_t * troop_count, int16_t troops[], int16_t dst_wx, int16_t dst_wy, int16_t player_idx, int16_t unit_idx)
{
    int16_t entire_stack_copy[MAX_STACK];
    int16_t boatriders[MAX_STACK];
    int16_t entire_stack[MAX_STACK];
    int16_t unit_roadbuild_count;
    int16_t Unused_Local;
    int16_t passenger_count;
    int16_t boatrider_idx;
    int16_t transport_capacity;
    int16_t boatrider_count;
    int16_t stack_has_transport;
    int16_t unit_wp;
    int16_t unit_wy;
    int16_t unit_wx;
    int16_t entire_stack_count;
    int16_t boat_count;
    int16_t l_troop_count;
    int16_t itr_units;  // _SI_
    int16_t itr;  // _SI_

    unit_wx = _UNITS[unit_idx].wx;
    unit_wy = _UNITS[unit_idx].wy;
    unit_wp = _UNITS[unit_idx].wp;

    unit_roadbuild_count = _UNITS[unit_idx].Rd_Constr_Left;

    entire_stack_count = 0;
    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if((_UNITS[itr_units].wp == unit_wp) && (_UNITS[itr_units].wx == unit_wx) && (_UNITS[itr_units].wy == unit_wy) && (_UNITS[itr_units].owner_idx == player_idx))
        {
            entire_stack[entire_stack_count] = itr_units;
            entire_stack_count++;
        }
    }

    l_troop_count = 0;
    stack_has_transport = ST_FALSE;
    transport_capacity = 0;
    for(itr = 0; itr < entire_stack_count; itr++)
    {
        unit_idx = entire_stack[itr];
        if(((_UNITS[unit_idx].Status == us_GOTO) || (_UNITS[unit_idx].Status == us_Move)) && (_UNITS[unit_idx].dst_wx == dst_wx) && (_UNITS[unit_idx].dst_wy == dst_wy) && (_unit_type_table[_UNITS[unit_idx].type].Transport > 0))
        {
            stack_has_transport = ST_TRUE;
            transport_capacity += _unit_type_table[_UNITS[unit_idx].type].Transport;
        }
    }

    if(stack_has_transport == ST_TRUE)
    {
        l_troop_count = 0;
        boat_count = 0;
        for(itr = 0; itr < entire_stack_count; itr++)
        {
            unit_idx = entire_stack[itr];
            if(((_UNITS[unit_idx].Status == us_GOTO) || (_UNITS[unit_idx].Status == us_Move)) && (_UNITS[unit_idx].dst_wx == dst_wx) && (_UNITS[unit_idx].dst_wy == dst_wy) && (_unit_type_table[_UNITS[unit_idx].type].Transport > 0))
            {
                troops[boat_count] = unit_idx;
                boat_count++;
            }
            entire_stack_copy[l_troop_count] = unit_idx;  // just a copy of the initial, entire stack
            l_troop_count++;
        }
        l_troop_count = boat_count;

        if(Square_Has_City(unit_wx, unit_wy, unit_wp) == ST_UNDEFINED)
        {
            for(itr = 0; itr < MAX_STACK; itr++)
            {
                boatriders[itr] = ST_UNDEFINED;
            }
            boatrider_count = Army_Boatriders(entire_stack_count, &entire_stack_copy[0], &boatriders[0]);
            Unused_Local = 0;
            if(boatrider_count > 0)
            {
                l_troop_count = boat_count;
                boatrider_idx = 0;
                passenger_count = 0;
                for(itr = 0; itr < entire_stack_count; itr++)
                {
                    unit_idx = entire_stack[itr];
                    if((boatriders[boatrider_idx] != unit_idx) && (passenger_count < transport_capacity))
                    {
                        troops[l_troop_count] = unit_idx;
                        l_troop_count++;
                        boatrider_idx++;
                        // drake178: ; BUG: incorrect order of operations - heroes should be checked for before evaluating the passenger count
                        if(_UNITS[unit_idx].type > ut_Chosen)
                        {
                            passenger_count++;
                        }
                    }
                }
                for(itr = 0; itr < entire_stack_count; itr++)
                {
                    // TODO  macro - does or does not need a seat on a boat
                    if(
                        (Unit_Has_AirTravel_Item(entire_stack[itr]) == ST_TRUE)
                        ||
                        (Unit_Has_WaterTravel_Item(entire_stack[itr]) == ST_TRUE)
                        ||
                        (Unit_Has_Swimming(entire_stack[itr]) == ST_TRUE)
                        ||
                        (Unit_Has_WindWalking(entire_stack[itr]) == ST_TRUE)
                        ||
                        (Unit_Has_AirTravel(entire_stack[itr]) == ST_TRUE)
                    )
                    {
                        troops[l_troop_count] = entire_stack[itr];
                        l_troop_count++;
                    }
                }
            }  /* if(boatrider_count > 0) */
            else
            {
                if(l_troop_count < entire_stack_count)
                {
                    for(itr = 0; itr < entire_stack_count; itr++)
                    {
                        if(_unit_type_table[_UNITS[entire_stack[itr]].type].Transport < 1)
                        {
                            troops[l_troop_count] = entire_stack[itr];
                            l_troop_count++;
                        }
                    }
                }
            }  /* if(boatrider_count > 0) { ... } else */

        }  /* if(Square_Has_City(unit_wx, unit_wy, unit_wp) == ST_UNDEFINED) */

    }  /* if(stack_has_transport == ST_TRUE) */
    else
    {
        l_troop_count = 0;
        for(itr = 0; itr < entire_stack_count; itr++)
        {
            unit_idx = entire_stack[itr];
            if(((_UNITS[unit_idx].Status == us_GOTO) || (_UNITS[unit_idx].Status == us_Move)) && (_UNITS[unit_idx].dst_wx == dst_wx) && (_UNITS[unit_idx].dst_wy == dst_wy) && (_UNITS[unit_idx].moves2 > 0) && (_UNITS[unit_idx].Rd_Constr_Left == unit_roadbuild_count))
            {
                troops[l_troop_count] = unit_idx;
                l_troop_count++;
            }
        }
    }  /* if(stack_has_transport == ST_TRUE) { ... } else */

    *troop_count = l_troop_count;
}


// WZD o61p10
// drake178:  STK_BuildingPossible()
/*
; returns 1 if the stack is on a non-sailable tile and
; has at least one unit with the construction ability;
; or 0 otherwise
; sets GUI_ExtraUnitAction to 0 if returning 1
*/
int16_t Unit_Action_Special_Build(int16_t troop_count, int16_t troops[])
{
    int16_t have_road_builders;
    int16_t unit_type;
    int16_t unit_idx;
    int16_t itr_troops;  // _DI_

    if(Terrain_Is_Sailable(_UNITS[troops[0]].wx, _UNITS[troops[0]].wy, _UNITS[troops[0]].wp) == ST_TRUE)
    {
        return ST_FALSE;
    }

    have_road_builders = ST_FALSE;

    for(itr_troops = 0; ((itr_troops < troop_count) && (have_road_builders == ST_FALSE)); itr_troops++)
    {
        unit_idx = troops[itr_troops];
        unit_type = _UNITS[unit_idx].type;
        if(_unit_type_table[unit_type].Construction > 0)
        {
            have_road_builders = ST_TRUE;
        }

    }

    if(have_road_builders != ST_TRUE)
    {
        return ST_FALSE;
    }
    else
    {
        special_action_flag = 0;
        return ST_TRUE;
    }

}


// WZD o61p11
/*
    sets special_action_flag
    returns {F,T} could 'Purify' 'Unit Action - Special
*/
int16_t Unit_Action_Special_Purify(int16_t troop_count, int16_t troops[])
{
    int16_t unit_wy;
    int16_t unit_wx;
    int16_t have_purifier;
    int16_t unit_type;
    int16_t unit_idx;
    int16_t itr_troops;  // _DI_

    have_purifier = ST_FALSE;

    for(itr_troops = 0; ((itr_troops < troop_count) && (have_purifier == ST_FALSE)); itr_troops++)
    {
        unit_idx = troops[itr_troops];
        unit_type = _UNITS[unit_idx].type;
        if((_unit_type_table[unit_type].Abilities & UA_PURIFY) != 0)
        {
            have_purifier = ST_TRUE;
        }
    }

    if(have_purifier != ST_TRUE)
    {
        return ST_FALSE;
    }

    unit_wx = _UNITS[troops[0]].wx;
    unit_wy = _UNITS[troops[0]].wy;

    if((TBL_Terrain_Flags[((_map_plane * WORLD_SIZE) + (unit_wy * WORLD_WIDTH) + unit_wx)] & TF_Corruption) == TF_Corruption)
    {
        special_action_flag = 2;
        return ST_TRUE;
    }

}


// WZD o61p12
int16_t Unit_Action_Special_Meld(int16_t troop_count, int16_t troops[])
{
    int16_t Guardian;
    int16_t node_idx;
    int16_t Spirit;
    int16_t unit_owner;
    int16_t unit_type;
    int16_t unit_idx;
    int16_t itr_troops;  // _DI_

    node_idx = TILE_HasNode(_UNITS[troops[0]].wx, _UNITS[troops[0]].wy, _UNITS[troops[0]].wp);

    unit_owner = _UNITS[troops[0]].owner_idx;

    Spirit = ST_FALSE;
    Guardian = ST_FALSE;

    if(node_idx == ST_UNDEFINED)
    {
        return ST_FALSE;
    }

    for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
    {
        unit_idx = troops[itr_troops];
        unit_type = _UNITS[unit_idx].type;
        if((_unit_type_table[unit_type].Abilities & UA_MELD) != 0)
        {
            // cmp     ax, es:[bx+(Guardian_Spirit*24h)+s_SPELL_DATA.Param0]
            // TODO  if(_UNITS[unit_idx].type == spell_data_table[GUARDIAN_SPIRIT].Param0)
            // TODO  Guardian = ST_TRUE;
            // TODO  Spirit = ST_TRUE;
            // TODO  break;
            // TODO  else
            // TODO  Spirit = ST_TRUE;
            Guardian = ST_TRUE;
            Spirit = ST_TRUE;
        }

        if(Spirit != ST_TRUE)
        {
            return ST_FALSE;
        }

        if(_NODES[node_idx].owner_idx != unit_owner)
        {
            if((_NODES[node_idx].Meld_Flags & M_Warped) != 0)
            {
                return ST_FALSE;
            }
            else
            {
                special_action_flag = 9;
                return ST_TRUE;
            }
        }
        else
        {
            if((_NODES[node_idx].Meld_Flags & M_Guardian) != 0)
            {
                return ST_FALSE;
            }
            else
            {
                if(Guardian != ST_TRUE)
                {
                    return ST_FALSE;
                }
                else
                {
                    special_action_flag = 9;
                    return ST_TRUE;
                }
            }
        }
    }

}

// WZD o61p13
void STK_MeldWithNode(void)
{
    int16_t troops[MAX_STACK];
    int16_t troop_count;

    Active_Unit_Stack(&troop_count, &troops[0]);

    STK_DoMeldWithNode(troop_count, &troops[0]);

}


// WZD o61p14
/*
// ; attempts to meld the first spirit in the stack with
// ; the node on their tile, if there is one, handling
// ; all related mechanics (warp, guardian, fail prompt)
*/
void STK_DoMeldWithNode(int16_t troop_count, int16_t troops[])
{
    int16_t Random_Result;
    int16_t Can_Meld;
    int16_t node_owner;
    int16_t unit_owner;
    int16_t Guardian;
    int16_t Spirit_Unit_Index;
    int16_t itr_troops;
    int16_t unit_idx;  // _SI_
    int16_t node_idx;  // _DI_

    if(troop_count < 1)
    {
        return;
    }

    unit_idx = troops[0];

    node_idx = TILE_HasNode(_UNITS[unit_idx].wx, _UNITS[unit_idx].wy, _UNITS[unit_idx].wp);

    if((_NODES[node_idx].Meld_Flags & M_Warped) != 0)
    {
        return;
    }

    unit_owner = _UNITS[unit_idx].owner_idx;

    node_owner = _NODES[node_idx].owner_idx;

    Guardian = ST_FALSE;

    Spirit_Unit_Index = ST_UNDEFINED;

    for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
    {
        if((_unit_type_table[_UNITS[troops[itr_troops]].type].Abilities & UA_MELD) != 0)
        {
            // cmp     ax, es:[bx+(Guardian_Spirit*24h)+s_SPELL_DATA.Param0]
            // TODO  if(_UNITS[unit_idx].type == spell_data_table[GUARDIAN_SPIRIT].Param0)
            // TODO  Spirit_Unit_Index = unit_idx;
            // TODO  Guardian = ST_TRUE;
            // TODO  break;
            // else
            Spirit_Unit_Index = unit_idx;
        }
    }

    if(Spirit_Unit_Index == ST_UNDEFINED)
    {
        return;
    }

    Can_Meld = ST_FALSE;

    if(
        (node_owner == ST_UNDEFINED)
        ||
        (node_owner == unit_owner)
    )
    {
        Can_Meld = ST_TRUE;
    }
    else
    {
        if((_NODES[node_idx].Meld_Flags & M_Guardian) == 0)
        {
            Can_Meld = ST_TRUE;
        }
        else
        {
            Random_Result = Random(100);
            if(Random_Result <= 25)
            {
                Can_Meld = ST_TRUE;
                _NODES[node_idx].Meld_Flags ^= M_Guardian;
            }
            else
            {
                Warn0(_msg_guardian_kills_spirit);  // "The Guardian Spirit occupying this node kills your spirit."
            }
        }
    }

    if(Can_Meld == ST_TRUE)
    {
        _NODES[node_idx].owner_idx = unit_owner;

        if(Guardian == ST_TRUE)
        {
            _NODES[node_idx].Meld_Flags |= M_Guardian;
        }
    }

    UNIT_MarkRemoved(Spirit_Unit_Index, 1);

}


// WZD o61p15
/*
; progress the purification counters of all purifying units, cleansing any tiles where the treshold has been reached
; WARNING: the Target_X field may not be the best place to hold this information?
*/
void Next_Turn_Process_Purify(void)
{
    int16_t troops[MAX_STACK];
    int16_t unit_owner;
    int16_t Purify_Progress;
    int16_t troop_count;
    int16_t unit_wp;
    int16_t unit_wy;
    int16_t unit_wx;
    int16_t itr_units;  // _SI_
    int16_t time_stop_flag;  // _DI_
    int16_t itr_troops;  // _DI_
    int16_t unit_idx;

    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(_UNITS[itr_units].owner_idx != ST_UNDEFINED)
        {
            time_stop_flag = ST_FALSE;
            if(
                (g_TimeStop_PlayerNum > 0)
                &&
                ((_UNITS[itr_units].owner_idx + 1) != g_TimeStop_PlayerNum)
            )
            {
                time_stop_flag = ST_TRUE;
            }

            if(time_stop_flag != ST_TRUE)
            {
                if(_UNITS[itr_units].Status == us_Purify)
                {
                    if((_unit_type_table[_UNITS[itr_units].type].Abilities & UA_PURIFY) != 0)
                    {
                        _UNITS[itr_units].Status = us_PurifyDone;
                    }
                    else
                    {
                        _UNITS[itr_units].Status = us_Ready;
                    }
                }
            }
        }
    }


    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(_UNITS[itr_units].owner_idx != ST_UNDEFINED)
        {
            if(_UNITS[itr_units].Status == us_PurifyDone)
            {
                unit_wx = _UNITS[itr_units].wx;
                unit_wy = _UNITS[itr_units].wy;
                unit_wp = _UNITS[itr_units].wp;
                unit_owner = _UNITS[itr_units].owner_idx;

                Player_Army_At_Square(unit_wx, unit_wy, unit_wp, unit_owner, &troop_count, &troops[0]);

                Purify_Progress = 0;

                for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
                {
                    if(_UNITS[troops[itr_troops]].Status == us_PurifyDone)
                    {
                        Purify_Progress += _UNITS[troops[itr_troops]].dst_wx;
                        _UNITS[troops[itr_troops]].dst_wx += 1;
                        _UNITS[troops[itr_troops]].Status = us_Purify;
                    }
                }

                if(Purify_Progress >= 4)
                {

                    Map_Square_Clear_Corruption(_UNITS[troops[0]].wx, _UNITS[troops[0]].wy, _UNITS[troops[0]].wp);
                    // unit_idx = troops[(itr_troops - 1)];
                    // unit_wx = _UNITS[unit_idx].wx;
                    // unit_wy = _UNITS[unit_idx].wy;
                    // unit_wp = _UNITS[unit_idx].wp;
                    // Map_Square_Clear_Corruption(unit_wx, unit_wy, unit_wp);

                    for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
                    {
                        if(_UNITS[troops[itr_troops]].Status == us_Purify)
                        {
                            _UNITS[troops[itr_troops]].dst_wx = 0;
                            _UNITS[troops[itr_troops]].Status = us_Ready;
                            _UNITS[troops[itr_troops]].Finished = ST_TRUE;
                        }
                    }
                }
            }
        }
    }


    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(_UNITS[itr_units].owner_idx != ST_UNDEFINED)
        {
            if(_UNITS[itr_units].Status == us_PurifyDone)
            {
                _UNITS[itr_units].Status = us_Purify;
            }
        }
    }

}



// WZD o61p16
/*
    counts units that are not busy {Patrol,Goto,Purify}
    returns ST_TRUE if any of the player's units are not *busy*
    logic feels inverted for the way it is tested
*/
int16_t Any_Units_Not_Busy()
{
    int16_t unit_count;
    int16_t itr_units;
    int16_t return_value;

    unit_count = 0;

    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(_UNITS[itr_units].owner_idx == _human_player_idx)
        {
            if(
                (_UNITS[itr_units].Status != us_Patrol)
                &&
                (_UNITS[itr_units].Status != us_GOTO)
                &&
                (_UNITS[itr_units].Status != us_Purify)
            )
            {
                unit_count++;
            }
        }
    }

    if(unit_count > 0)
    {
        return ST_TRUE;
    }
    else
    {
        return ST_FALSE;
    }

}
