/*
    WIZARDS.EXE
        ovr059
*/

#include "CITYCALC.h"
#include "Explore.h"
#include "Lair.h"
#include "MOX/MOX_DAT.h"  /* _players[] */
#include "MOX/MOX_BASE.h"
#include "MOX/MOX_DEF.h"
#include "MOX/MOX_TYPE.h"
#include "MOX/GENDRAW.h"

#include "MOM_DEF.h"
#include "MainScr.h"
#include "MainScr_Maps.h"  /* TILE_Explore(); */
#include "Outpost.h"
#include "Terrain.h"
#include "UNITTYPE.h"
#include "UnitMove.h"

#include "WZD_o059.h"


/*
    WIZARDS.EXE  ovr059
*/

// WZD o59p01
int16_t Player_Fortress_City(int16_t player_idx)
{
    int16_t city_idx = 0;
    int16_t itr_cities = 0;  // _SI_

    if(player_idx == NEUTRAL_PLAYER_IDX)
    {
        return ST_UNDEFINED;
    }

    city_idx = ST_UNDEFINED;

    for(itr_cities = 0; ((itr_cities < _cities) && (city_idx == ST_UNDEFINED)); itr_cities++)
    {
        if(
            (_FORTRESSES[player_idx].active == ST_TRUE)
            &&
            (_FORTRESSES[player_idx].wp == _CITIES[itr_cities].wp)
            &&
            (_FORTRESSES[player_idx].wx == _CITIES[itr_cities].wx)
            &&
            (_FORTRESSES[player_idx].wy == _CITIES[itr_cities].wy)
        )
        {
            city_idx = itr_cities;
        }
    }

    return city_idx;
}


// WZD o59p02
int16_t Player_Summon_City(int16_t player_idx)
{
    int16_t city_idx = 0;  // _DI_
    int16_t itr_cities = 0;  // _CX_

    if(player_idx == NEUTRAL_PLAYER_IDX)
    {
        return ST_UNDEFINED;
    }

    city_idx = ST_UNDEFINED;

    for(itr_cities = 0; ((itr_cities < _cities) && (city_idx == ST_UNDEFINED)); itr_cities++)
    {
        if(
            (_CITIES[itr_cities].wp == _players[player_idx].summon_wp) &&
            (_CITIES[itr_cities].wx == _players[player_idx].summon_wx) &&
            (_CITIES[itr_cities].wy == _players[player_idx].summon_wy)
        )
        {
            city_idx = itr_cities;
        }
    }

    return city_idx;
}


// WZD o59p03
/*

in-outs count and indices of units in city

*/
void Army_At_City(int16_t city_idx, int16_t * troop_count, int16_t troops[])
{
    int16_t itr = 0;

    *troop_count = 0;

    for(itr = 0; ((itr < _units) && (*troop_count < MAX_STACK)); itr++)
    {
        if(
            (_CITIES[city_idx].wp == _UNITS[itr].wp) &&
            (_CITIES[city_idx].wx == _UNITS[itr].wx) &&
            (_CITIES[city_idx].wy == _UNITS[itr].wy) &&
            (_CITIES[city_idx].owner_idx == _UNITS[itr].owner_idx)
        )
        {
            troops[*troop_count] = itr;
            *troop_count += 1;
        }
    }

}


// WZD o59p04
void Player_Army_At_Square(int16_t wx, int16_t wy, int16_t wp, int16_t player_idx, int16_t * troop_count, int16_t troops[])
{
    int16_t itr = 0;

    *troop_count = 0;

    for(itr = 0; itr < _units; itr++)
    {
        if(
            ((_UNITS[itr].wp == wp) || (_UNITS[itr].in_tower == ST_TRUE)) &&
            (_UNITS[itr].wx == wx) &&
            (_UNITS[itr].wy == wy) &&
            (_UNITS[itr].owner_idx == player_idx)
        )
        {
            troops[*troop_count] = itr;
            *troop_count += 1;
        }
    }

}


// WZD o59p05
void Enemy_Army_At_Square(int16_t wx, int16_t wy, int16_t wp, int16_t player_idx, int16_t * troop_count, int16_t troops[])
{
    int16_t itr = 0;

    *troop_count = 0;

    for(itr = 0; itr < _units; itr++)
    {
        if(
            ((_UNITS[itr].wp == wp) || (_UNITS[itr].in_tower == ST_TRUE)) &&
            (_UNITS[itr].wx == wx) &&
            (_UNITS[itr].wy == wy) &&
            (_UNITS[itr].owner_idx != player_idx) &&
            (_UNITS[itr].owner_idx != ST_UNDEFINED)
        )
        {
            troops[*troop_count] = itr;
            *troop_count += 1;
        }
    }

}


// WZD o59p06
// drake178: TILE_GetUnits_2()
void Army_At_Square_2(int16_t wx, int16_t wy, int16_t wp, int16_t * troop_count, int16_t troops[])
{
    int16_t itr = 0;

    *troop_count = 0;

    for(itr = 0; ((itr < _units) && (*troop_count < MAX_STACK)); itr++)
    {
        if(
            (
                (_UNITS[itr].wp == wp)
                ||
                (_UNITS[itr].in_tower == ST_TRUE)
            )
            &&
            (_UNITS[itr].wx == wx)
            &&
            (_UNITS[itr].wy == wy)
            &&
            (_UNITS[itr].owner_idx != ST_UNDEFINED)
        )
        {

            troops[*troop_count] = itr;

            *troop_count += 1;

        }

    }

}


// WZD o59p07
// drake178: CRP_TILE_GetUnits_2()
/*
returns a list of the units occupying the selected
tile, excluding any that are marked as owned by
player index -1

(slightly better than TILE_GetUnits)
*/
/*

vs Army_At_Square_2()
    doesn't check tower
    doesn't limit MAX_STACK

*/
void Army_At_Square_3(int16_t wx, int16_t wy, int16_t wp, int16_t * troop_count, int16_t troops[])
{
    int16_t itr_units = 0;  // _DX_

    *troop_count = 0;

    for(itr_units = 0; itr_units < _units; itr_units++)
    {

        if(
            (_UNITS[itr_units].wp == wp)
            &&
            (_UNITS[itr_units].wx == wx)
            &&
            (_UNITS[itr_units].wy == wy)
            &&
            (_UNITS[itr_units].owner_idx != ST_UNDEFINED)
        )
        {

            troops[*troop_count] = itr_units;

            *troop_count += 1;

        }

    }

}


// WZD o59p08
/*
*/
/*

vs Army_At_Square_2()
    doesn't limit MAX_STACK

vs Army_At_Square_3()
    does check tower

*/
void UU_Army_At_Square_4(int16_t wx, int16_t wy, int16_t wp, int16_t * troop_count, int16_t troops[])
{

    int16_t itr_units = 0;  // _DX_

    *troop_count = 0;

    for(itr_units = 0; itr_units < _units; itr_units++)
    {

        if(
            (
                (_UNITS[itr_units].wp == wp)
                ||
                (_UNITS[itr_units].in_tower == ST_TRUE)
            )
            &&
            (_UNITS[itr_units].wx == wx)
            &&
            (_UNITS[itr_units].wy == wy)
            &&
            (_UNITS[itr_units].owner_idx != ST_UNDEFINED)
        )
        {

            troops[*troop_count] = itr_units;

            *troop_count += 1;

        }

    }

}


// WZD o59p09
// drake178: STK_SettlingPossible()
/*
*/
/*

*/
int16_t Unit_Action_Special_Settle(int16_t troop_count, int16_t troops[])
{
    int16_t unit_type = 0;
    int16_t have_settlers = 0;
    int16_t itr_troops = 0;  // _DI_
    int16_t unit_idx = 0;  // _SI_

    have_settlers = ST_FALSE;

    for(itr_troops = 0; ((itr_troops < troop_count) && (have_settlers == ST_FALSE)); itr_troops++)
    {
        unit_idx = troops[itr_troops];
        unit_type = _UNITS[unit_idx].type;
        if(
            ((_unit_type_table[unit_type].Abilities & UA_CREATEOUTPOST) != 0)
            &&
            ((_UNITS[unit_idx].mutations & UM_UNDEAD) == 0)
        )
        {
            have_settlers = ST_TRUE;
        }
    }

    unit_idx = troops[0];

    if(have_settlers == ST_TRUE)
    {
        if(Map_Square_Survey(_UNITS[unit_idx].wx, _UNITS[unit_idx].wy, _UNITS[unit_idx].wp) == 0)
        {
            special_action_flag = 1;
            return ST_TRUE;
        }
    }

    return ST_FALSE;
}


// WZD o59p10
void o59p10_empty_function(void)
{
// proc o59p10_empty_function far
// push    bp
// mov     bp, sp
// pop     bp
// retf
// endp o59p10_empty_function

    return;
    
}


// WZD o59p11
// sub_520DD()


// WZD o59p12
/*
; returns the index (into Node_Table@) of the node if
; the tile has one, or -1 if it doesn't
*/
/* 

*/
int16_t Get_Map_Square_Magic_Node(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t node_idx = 0;  // _SI_
    int16_t itr = 0;  // _CX_

    node_idx = ST_UNDEFINED;

    for(itr = 0; ((itr < NUM_NODES) && (node_idx == ST_UNDEFINED)); itr++)
    {
        if(
            (_NODES[itr].wp == wp)
            &&
            (_NODES[itr].wx == wx)
            &&
            (_NODES[itr].wy == wy)
        )
        {
            node_idx = itr;
        }
    }

    return node_idx;
}


// WZD o59p13
// TODO  rename; "Units_In_Tower" implies a return of a {F,T} status
/*
checks if stack is in a tower
sets unit's in_tower to {F,T}
updates explored
*/
void Units_In_Tower(int16_t troop_count, int16_t troops[], int16_t map_p)
{
    int16_t unit_wy = 0;
    int16_t unit_wx = 0;
    int16_t tower_idx = 0;
    int16_t units_in_tower = 0;
    int16_t unit_idx = 0;
    int16_t itr_towers = 0;
    int16_t itr_units = 0;

    if(troop_count < 1)
    {
        return;
    }

    unit_wx = _UNITS[troops[0]].wx;
    unit_wy = _UNITS[troops[0]].wy;

    units_in_tower = ST_FALSE;

    for(itr_towers = 0; (itr_towers < NUM_TOWERS) && (units_in_tower == ST_FALSE); itr_towers++)
    {
        if((_TOWERS[itr_towers].wx == unit_wx) && (_TOWERS[itr_towers].wy == unit_wy))
        {
            units_in_tower = ST_TRUE;
            tower_idx = itr_towers;
        }
    }

    if((units_in_tower == ST_TRUE) && (Check_Planar_Seal() == ST_TRUE))
    {
        units_in_tower = ST_FALSE;
    }

    if(units_in_tower == ST_TRUE)
    {
        for(itr_units = 0; itr_units < troop_count; itr_units++)
        {
            unit_idx = troops[itr_units];
            _UNITS[unit_idx].in_tower = ST_TRUE;
        }
        unit_idx = troops[0];
        _TOWERS[tower_idx].owner_idx = _UNITS[unit_idx].owner_idx;
        if(_UNITS[unit_idx].owner_idx == _human_player_idx)
        {
            Set_Map_Square_Explored_Flags_XYP(_TOWERS[tower_idx].wx, _TOWERS[tower_idx].wy, 0);
            Set_Map_Square_Explored_Flags_XYP(_TOWERS[tower_idx].wx, _TOWERS[tower_idx].wy, 1);
        }
    }
    else
    {
        for(itr_units = 0; itr_units < troop_count; itr_units++)
        {
            unit_idx = troops[itr_units];
            if(_UNITS[unit_idx].in_tower == ST_TRUE)
            {
                _UNITS[unit_idx].in_tower = ST_FALSE;
            }
        }
    }

}


// WZD o59p14
// drake178: GAME_InTowerRefresh()
/*
    updates all _UNITS[].in_tower
        including 'Planar Seal'
*/
void All_Units_In_Towers(void)
{
    int16_t have_planar_seal = 0;
    int16_t itr_units = 0; // _SI_
    int16_t itr_towers = 0; // _DI_

    have_planar_seal = Check_Planar_Seal();

    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        _UNITS[itr_units].in_tower = ST_FALSE;

        for(itr_towers = 0; itr_towers < NUM_TOWERS; itr_towers++)
        {
            if(
                (_UNITS[itr_units].wx == _TOWERS[itr_towers].wx)
                &&
                (_UNITS[itr_units].wy == _TOWERS[itr_towers].wy)
                &&
                (have_planar_seal == ST_FALSE)
            )
            {
                _UNITS[itr_units].in_tower = ST_TRUE;
            }
            else
            {
                _UNITS[itr_units].in_tower = ST_FALSE;
            }
        }
    }

}


// WZD o59p15
// MoO2  Module: OFFICER  Dismiss_Officer_()
/*
    calls Kill_Unit() with 1 - 'dismiss'
*/
void Dismiss_Unit(int16_t unit_idx)
{
    Kill_Unit(unit_idx, 1);
}


// WZD o59p16
void Cheat_Reveal(void)
{
    int16_t itr_y = 0;  // _DI_
    int16_t itr_x = 0;  // _SI_
    int16_t itr_players1 = 0;  // _SI_
    int16_t itr_players2 = 0;  // _DI_

    for(itr_y = 0; itr_y < WORLD_HEIGHT; itr_y++)
    {
        for(itr_x = 0; itr_x <WORLD_WIDTH; itr_x++)
        {
            SET_SQUARE_EXPLORED(itr_x, itr_y, 0, 0xF);
            SET_SQUARE_EXPLORED(itr_x, itr_y, 1, 0xF);
        }
    }

    for(itr_players1 = 0; itr_players1 < _num_players; itr_players1++)
    {
        for(itr_players2 = 0; itr_players2 < _num_players; itr_players2++)
        {
            _players[itr_players2].Dipl.Contacted[itr_players1] = ST_TRUE;
            _players[itr_players1].Dipl.Contacted[itr_players2] = ST_TRUE;
        }
    }

    _players[_human_player_idx].Globals[NATURES_AWARENESS] = ST_TRUE;

    Update_Scouted_And_Contacted__WIP();

    Reset_Map_Draw();

}

// WZD o59p17
// drake178: sub_5245A()
// sub_5245A()


// WZD o59p18
// drake178: ¿ ?
/*

Set_Map_Square_Explored_Flags_XYP_Range()
AKA TILE_ExploreRadius__WIP()
*/
void Set_Map_Square_Explored_Flags_XYP_Range(int16_t wx, int16_t wy, int16_t wp, int16_t scout_range)
{
    int16_t y_start = 0;
    int16_t x_start = 0;
    int16_t world_x = 0;
    int16_t itr_world_y = 0;
    int16_t itr_world_x = 0;  // _DI_

    if(scout_range == 0)
    {
        return;
    }

    if(scout_range == 1)
    {
        Set_Map_Square_Explored_Flags_XYP(wx, wy, wp);
    }

    scout_range--;

    y_start = wy - scout_range;

    if(y_start < 0)
    {
        y_start = 0;
    }

    x_start = wx - scout_range;

    if(x_start < 0)
    {
        x_start += WORLD_WIDTH;
    }

    scout_range = (scout_range * 2) + 1;

    for(itr_world_y = y_start; ((y_start + scout_range) > itr_world_y) && (itr_world_y < WORLD_HEIGHT); itr_world_y++)
    {
        for(itr_world_x = x_start; (x_start + scout_range) > itr_world_x; itr_world_x++)
        {
            if(itr_world_x < WORLD_WIDTH)
                world_x = itr_world_x;
            else
                world_x = itr_world_x - WORLD_WIDTH;

            Set_Map_Square_Explored_Flags_XYP(world_x, itr_world_y, wp);
        }
    }

}

// WZD o59p19
void Do_Plane_Button__WIP(int16_t player_idx, int16_t * map_x, int16_t * map_y, int16_t * map_plane)
{
    int16_t stack_is_on_tower = 0;
    int16_t stack_plane_shift = 0;
// Radius= word ptr -0Eh
    int16_t unit_world_x = 0;
    int16_t unit_world_y = 0;
    int16_t stack_planar_travel = 0;
    int16_t unit_stack_unit_idx = 0;
    int16_t itr_towers = 0;
    int16_t orig_map_plane = 0;
    // int16_t curr_map_plane;  // _DI_

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Do_Plane_Button__WIP(player_idx = %d, *map_x = %d, *map_y = %d, *map_plane = %d)\n", __FILE__, __LINE__, player_idx, *map_x, *map_y, *map_plane);
#endif

    // curr_map_plane = *map_plane;
    _active_stack_has_path = 0;
    // orig_map_plane = curr_map_plane;
    orig_map_plane = *map_plane;
    // curr_map_plane = ((curr_map_plane + 1) % PLANE_COUNT_MAX);
    *map_plane = ((*map_plane + 1) % NUM_PLANES);
    stack_planar_travel = ST_FALSE;

#ifdef STU_DEBUG
    // dbg_prn("DEBUG: [%s, %d]: curr_map_plane: %d\n", __FILE__, __LINE__, curr_map_plane);
    dbg_prn("DEBUG: [%s, %d]: *map_plane: %d\n", __FILE__, __LINE__, *map_plane);
#endif

    /*
        Active Stack && Can Travel && Sealed
    */
    if(all_units_moved == ST_FALSE)
    {
        if(_unit_stack_count > 0)
        {
            stack_planar_travel = Stack_Has_PlanarTravel();
            if(stack_planar_travel == ST_TRUE)
            {
                if(Check_Planar_Seal() == ST_TRUE)
                {
                    Warn0(_msg_planar_seal_prevents);  // "Planar Seal prevents your units from changing planes."
                    // curr_map_plane = orig_map_plane;
                    *map_plane = orig_map_plane;
                    stack_planar_travel = 99;  // ¿ `99` means 'Yay Planar Trav, Yay Planar Seal' ?
                }
                else
                {
                    // TODO  DLOG("(Check_Planar_Seal() != ST_TRUE)");
                }
            }
            else
            {
                // TODO  DLOG("(stack_planar_travel != ST_TRUE)");
            }
        }
        else
        {
            // TODO  DLOG("(_unit_stack_count <= 0)");
        }
    }
    else
    {
        // TODO  DLOG("(all_units_moved != ST_FALSE)");
    }
    /*
        HERE: stack_planar_travel = {-1, 0, 1, 99}
        -1: default/undefined
         0: Stack_Has_PlanarTravel() == ST_FALSE && _unit_stack_count > 0 && all_units_moved == ST_FALSE
         1: Stack_Has_PlanarTravel() == ST_TRUE && Check_Planar_Seal() == ST_FALSE && _unit_stack_count > 0 && all_units_moved == ST_FALSE
        99: Stack_Has_PlanarTravel() == ST_TRUE && Check_Planar_Seal() == ST_TRUE && _unit_stack_count > 0 && all_units_moved == ST_FALSE
    */
#ifdef STU_DEBUG
    // dbg_prn("DEBUG: [%s, %d]: curr_map_plane: %d\n", __FILE__, __LINE__, curr_map_plane);
    dbg_prn("DEBUG: [%s, %d]: *map_plane: %d\n", __FILE__, __LINE__, *map_plane);
#endif

    /*
        BEGIN: Yay PlanarTravel, Nay PlanarSeal
    */
    if(stack_planar_travel == ST_TRUE)
    {
        // TODO  DLOG("(stack_planar_travel == ST_TRUE)");
        // stack_plane_shift = Check_Stack_Plane_Shift(_unit_stack[0].unit_idx, curr_map_plane);
        stack_plane_shift = Check_Stack_Plane_Shift(_unit_stack[0].unit_idx, *map_plane);

        if(stack_plane_shift == -2)
        {
            // TODO  DLOG("(stack_plane_shift == -2)");
            // curr_map_plane = orig_map_plane;
            *map_plane = orig_map_plane;
            Warn0(_msg_units_cant_planar_travel); // "The selected units cannot Planar Travel at this location."
        }
        else
        {
            // TODO  DLOG("(stack_plane_shift != -2)");
        }
        if(stack_plane_shift == -1)
        {
            // TODO  DLOG("(stack_plane_shift == -1)");

        }
        else
        {
            // TODO  DLOG("(stack_plane_shift != -1)");
        }

        Reset_Map_Draw();
    }
    /*
        END: Yay PlanarTravel, Nay PlanarSeal
    */
    /*
        BEGIN: Nay Active Stack OR Nay PlanarTravel OR Yay PlanarTravel, Yay PlanarSeal
    */
    else
    {
        // TODO  DLOG("(stack_planar_travel != ST_TRUE)");
        // HERE: Yay PlanarTravel, Yay PlanarSeal falls through to '@@Done'
        if(stack_planar_travel != 99)
        {
            // HERE: Nay Active Stack OR Nay PlanarTravel NOT Yay PlanarTravel, Yay PlanarSeal
            // if(curr_map_plane == orig_map_plane)  /* ¿ ~== stack_planar_travel = 99 ? ¿ couln't possibly, but would only happens if Check_Stack_Plane_Shift() returned -2 ? */
            if(*map_plane == orig_map_plane)
            {
                // curr_map_plane = ((curr_map_plane + 1) % NUM_PLANES);
                *map_plane = ((*map_plane + 1) % NUM_PLANES);
                _unit_stack_count = 0;
            }

            if(_unit_stack_count > 0)
            {
                // HERE: Yay Active Stack, Nay Astral Gate, Nay All Unit Have Planar Travel
                // THEN: Reset Plane, Red Notify
                if(Check_Planar_Seal() == ST_TRUE)
                {
                    stack_is_on_tower = ST_FALSE;
                    unit_stack_unit_idx = _unit_stack[0].unit_idx;
                    unit_world_x = _UNITS[unit_stack_unit_idx].wx;
                    unit_world_y = _UNITS[unit_stack_unit_idx].wy;
                    for(itr_towers = 0; itr_towers < NUM_TOWERS && stack_is_on_tower == ST_FALSE; itr_towers++)
                    {
                        if(_TOWERS[itr_towers].wx == unit_world_x && _TOWERS[itr_towers].wy == unit_world_y)
                        {
                            stack_is_on_tower = ST_TRUE;
                        }
                    }
                    // HERE: Yay/Nay Active Stack is on/inside a Tower of Wizardy
                    if(stack_is_on_tower == ST_TRUE)
                    {
                        // curr_map_plane = orig_map_plane;
                        *map_plane = orig_map_plane;
                        Warn0(_msg_planar_seal_prevents);  // "Planar Seal prevents your units from changing planes."
                    }
                }
            }
        }
    }
    /*
        END: Nay PlanarTravel OR Yay PlanarTravel, Yay PlanarSeal
    */


// @@Done:
    Set_Unit_Draw_Priority();
    Reset_Stack_Draw_Priority();
    // Set_Entities_On_Map_Window(*map_x, *map_y, curr_map_plane);
    Set_Entities_On_Map_Window(*map_x, *map_y, *map_plane);


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Do_Plane_Button__WIP(player_idx = %d, *map_x = %d, *map_y = %d, *map_plane = %d)\n", __FILE__, __LINE__, player_idx, *map_x, *map_y, *map_plane);
#endif

}

// WZD o59p20
// AKA IDK_PlanarTravel_TestWaterCity_s52774()
/*
    returns {-2, -1, ¿ ... ?}
    -2: terrain is ocean||shore||river && 
*/
int16_t Check_Stack_Plane_Shift(int16_t unit_stack_unit_idx, int16_t map_plane)
{
    int16_t movement_mode_flags[6] = { 0xBB,0xBB,0xBB,0xBB,0xBB,0xBB };
    int16_t stack_array[MAX_STACK] = { 0xBB,0xBB,0xBB,0xBB,0xBB,0xBB,0xBB,0xBB,0xBB };
    int16_t city_has_spellward = 0;
    // int16_t laird_idx__city_idx;
    int16_t lair_idx = 0;
    int16_t city_idx = 0;
    int16_t unit_world_x = 0;
    int16_t unit_world_y = 0;
    int16_t stack_size = 0;

    int16_t unit_idx = 0;
    int16_t return_value = 0;


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Check_Stack_Plane_Shift(unit_stack_unit_idx = %d, map_plane = %d)\n", __FILE__, __LINE__, unit_stack_unit_idx, map_plane);
#endif

    unit_idx = unit_stack_unit_idx;

    return_value = -1;  // ¿ ST_SUCCESS ?

    Active_Stack_Movement_Modes(&movement_mode_flags[0]);

    if(Square_Is_Sailable(_UNITS[unit_idx].wx, _UNITS[unit_idx].wy, map_plane) == ST_TRUE)
    {
        return_value = -2;  // ¿ -2 ~== Non-Walking Terrain ?
        if(movement_mode_flags[MM_SWIMMING] == ST_TRUE || movement_mode_flags[MM_SAILING] == ST_TRUE || movement_mode_flags[MM_FLYING] == ST_TRUE)
        {
            return_value = -1;  // ¿ ST_SUCCESS ?
        }
    }

    if(return_value == -1)
    {
        unit_world_x = _UNITS[unit_stack_unit_idx].wx;
        unit_world_y = _UNITS[unit_stack_unit_idx].wy;
        lair_idx = Square_Has_Lair(unit_world_x, unit_world_y, map_plane);
        if(lair_idx == -1)
        {
            city_idx = Square_Has_City(unit_world_x, unit_world_y, map_plane);
            if(city_idx == -1)
            {
                stack_size = 0;
                // drake178: fills the return variables with the count and indices of all units on the selected tile that do not belong to the specified player
                // TODO  TILE_GetEnemyStack(unit_world_x, unit_world_y, map_plane, _UNITS[unit_idx].owner_idx, &stack_size, &stack_array[0]);
                if(stack_size > 0)
                {
                    return_value = -2;
                }
                else
                {
                    stack_size = 0;
                    // drake178: fills the return variables with the count and indices of all units on the selected tile that belong to the specified player
                    // TODO  TILE_GetUnitStack(_UNITS[unit_idx].wx, _UNITS[unit_idx].wy, map_plane, _UNITS[unit_idx].owner_idx, &stack_size, &stack_array[0]);
                    stack_size = stack_size + _unit_stack_count;
                    if(stack_size > MAX_STACK)
                    {
                        return_value = -2;
                    }
                }
            }
            else
            {
                city_has_spellward = RP_CTY_CheckSpellWard__STUB(city_idx, &stack_size, &stack_array[0]);
                if(city_has_spellward == ST_TRUE)
                {
                    return_value = -2;
                }
                else
                {
                    if(_CITIES[city_idx].owner_idx != _UNITS[unit_idx].owner_idx)
                    {
                        return_value = -2;
                    }
                    else
                    {
                        // drake178: fills the return variables with the count and indices of all units on the selected tile that belong to the specified player
                        // TODO  TILE_GetUnitStack(unit_world_x, unit_world_y, map_plane, _UNITS[unit_idx].owner_idx, &stack_size, &stack_array[0]);
                        stack_size = stack_size + _unit_stack_count;

                        if(stack_size > MAX_STACK)
                        {
                            return_value = -2;
                        }
                    }
                }
            }
        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Check_Stack_Plane_Shift(unit_stack_unit_idx = %d, map_plane = %d)\n", __FILE__, __LINE__, unit_stack_unit_idx, map_plane);
#endif

    return return_value;
}
