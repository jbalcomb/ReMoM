/*
    WIZARDS.EXE
        ovr100

    Moo2
        Module: COLONIZE

*/

#include "MoM.H"

/*
o100p04
IDK_Settle_s82061()
o100p05
STK_SettleTile()
o77p01
Create_Outpost()


Module: COLONIZE
    Make_New_Colony_Or_Outpost_
        Address: 01:000E5EB3
    Make_New_Colony_
        Address: 01:000E6071
    Make_New_Outpost_
        Address: 01:000E607F
    Allocate_New_Colony_Slots_
        Address: 01:000E64F4

Module: MAINPUPS
    New_Colony_Confirmation_Box_
        Address: 01:000C9FC4

*/
/*
Program-Flow: Special Unit Action - Settle 

    IDK_Settle_s82061()
        |-> STK_GetMovableUnits()
        |-> STK_SettleTile()
            |-> Create_Outpost(unit_x, unit_y, unit_p, unit_race, unit_owner_idx, unit_idx)
                |-> TILE_CanBeSettled()
                |-> CTY_CreateEmpty()

Warn0(cnst_MaxCitiesError);  / * "No more outposts may be built." * /


Has Unit Type Ability 'Create Outpost' and NOT Unit Mutation - Undead

Manual:
**Build** outposts: settlers can build outposts (click on *build button*)
on map squares that are at least three map squares from any
other city.
...select the build option...
...build an outpost...

Game-Data:
"No more outposts may be built."
"New Outpost Founded"
"Outpost Of "
"New Outpost"
*/




// WZD o100p01
// drake178: AI_MoveUnits()
// AI_MoveUnits()

// WZD o100p02
// drake178: AI_UNIT_Meld()
// AI_UNIT_Meld()

// WZD o100p03
// drake178: AI_UNIT_Settle()
// AI_UNIT_Settle()

// WZD o100p04
int16_t Do_Build_Outpost(void)
{
    int16_t troops[MAX_STACK];
    int16_t troop_count;

    Active_Unit_Stack(&troop_count, &troops[0]);

    return STK_SettleTile(troop_count, &troops[0]);

}

// WZD o100p05
// drake178: STK_SettleTile()
int16_t STK_SettleTile(int16_t troop_count, int16_t troops[])
{
    int16_t unit_owner;
    int16_t unit_race;
    int16_t unit_wp;
    int16_t unit_wy;
    int16_t unit_wx;
    int16_t stack_has_settler;
    int16_t unit_type;
    int16_t itr_troops;  // _DI_
    int16_t unit_idx;  // _SI_

    stack_has_settler = ST_FALSE;

    for(itr_troops = 0; ((itr_troops < troop_count) && (stack_has_settler == ST_FALSE)); itr_troops++)
    {
        unit_idx = troops[itr_troops];
        unit_type = _UNITS[unit_idx].type;
        if(
            ((_unit_type_table[unit_type].Abilities & UA_CREATEOUTPOST) != 0)
            &&
            ((_UNITS[unit_idx].mutations & UM_UNDEAD) == 0)
        )
        {
            stack_has_settler = ST_TRUE;
        }
    }

    if(stack_has_settler != ST_TRUE)
    {
        return ST_FALSE;
    }

    unit_wx = _UNITS[unit_idx].wx;
    unit_wy = _UNITS[unit_idx].wy;
    unit_wp = _UNITS[unit_idx].wp;

    unit_owner = _UNITS[unit_idx].owner_idx;

    unit_race = _unit_type_table[unit_type].Race;

    if(Create_Outpost(unit_wx, unit_wy, unit_wp, unit_race, unit_owner, unit_idx) == ST_TRUE)
    {
        return ST_TRUE;
    }
    else
    {
        return ST_FALSE;
    }

}


// WZD o100p06
// drake178: AI_UNIT_Move()
// AI_UNIT_Move()

// WZD o100p07
// drake178: sub_8227A()
// sub_8227A()

// WZD o100p08
// drake178: sub_82377()
// sub_82377()

// WZD o100p09
// drake178: UNIT_PushOffTile()
void UNIT_PushOffTile(int16_t unit_idx)
{
    int16_t troops[2];
    int16_t Target_Y;
    int16_t Target_X;
    int16_t unit_owner_idx;
    int16_t unit_wp;
    int16_t unit_wy;
    int16_t unit_wx;
    int16_t unit_spaces;
    int16_t Found_Tile;
    int16_t Checked_X;
    int16_t Diameter;
    int16_t Min_X;
    int16_t Min_Y;
    int16_t itr_x;
    int16_t itr_y;  // _DI_

    unit_wx = _UNITS[unit_idx].wx;
    unit_wy = _UNITS[unit_idx].wy;
    unit_wp = _UNITS[unit_idx].wp;
    unit_owner_idx = _UNITS[unit_idx].owner_idx;

    Min_Y = (unit_wy - 1);

    if(Min_Y < 0)
    {
        Min_Y = 0;  // BUGBUG:  drake178: BUG: range checking must be performed on the test coordinates, doing it here shifts the rectangle
    }

    Min_X = (unit_wx - 1);

    if(Min_X < 0)
    {
        Min_X += WORLD_WIDTH;
    }

    Diameter = 3;
    Found_Tile = ST_FALSE;
    unit_spaces = 0;

    for(itr_y = Min_Y; ((itr_y <= (Min_Y + Diameter)) && (Found_Tile == ST_FALSE)); itr_y++)
    {
        if(itr_y < WORLD_HEIGHT)
        {
            for(itr_x = Min_X; itr_x <= (Min_X + Diameter); itr_x++)
            {
                if(Found_Tile == ST_FALSE)
                {
                    if(itr_x < WORLD_WIDTH)
                    {
                        Checked_X = itr_x;
                    }
                    else
                    {
                        Checked_X = (itr_x - WORLD_WIDTH);
                    }

                    if(
                        (Checked_X != OVL_Action_XPos) &&
                        (itr_y != OVL_Action_YPos)
                    )
                    {
                        if(
                            (Square_Has_Lair(Checked_X, itr_y, unit_wp) == -1) &&
                            (Player_City_At_Square(Checked_X, itr_y, unit_wp, unit_owner_idx) == -1)
                        )
                        {
                            unit_spaces = Unit_Space_At_Square(Checked_X, itr_y, unit_wp, unit_owner_idx, unit_idx);

                            if(unit_spaces > 0)
                            {
                                Found_Tile = ST_TRUE;
                                Target_X = Checked_X;
                                Target_Y = itr_y;
                            }
                        }
                    }
                }
            }
        }
    }

    if(unit_spaces > 0)
    {
        _UNITS[unit_idx].wx = Target_X;
        _UNITS[unit_idx].wy = Target_Y;

        troops[0] = unit_idx;

        Units_In_Tower(1, &troops[0], _UNITS[unit_idx].wp);  // BUGBUG  drake178: Towers of Wizardry are not valid push destinations
    }
    else
    {
        UNIT_MarkRemoved(unit_idx, 1);
    }

}


// WZD o100p10
// drake178: TILE_UNIT_CanMoveTo()
/*

    Return:
        0, if Unit's movement mode does not support the terrain type or there are already 9 units on the square
        {1, ..., 9}, count of Units that could move to the target map square
        ~==
        == 0, False
        >= 1, True
*/
int16_t Unit_Space_At_Square(int16_t wx, int16_t wy, int16_t wp, int16_t player_idx, int16_t unit_idx)
{
    int16_t troops[9];
    int16_t trooper_idx;
    int16_t have_windwalker;
    int16_t unit_spaces;
    int16_t troop_count;
    int16_t itr;  // _SI_
    int16_t is_legal_move;  // _DI_

    Army_At_Square_2(wx, wy, wp, &troop_count, &troops[0]);

    if(
        (troop_count > 0) &&
        (
            (_UNITS[troops[0]].owner_idx != player_idx) ||
            (troop_count >= MAX_STACK)
        )
    )
    {
        goto Done_Return_Zero;
    }

    have_windwalker = ST_FALSE;

    for(itr = 0; ((itr < NUM_TOWERS) && (have_windwalker == ST_FALSE)); itr++)
    {
        if(
            (_TOWERS[itr].wx == wx) &&
            (_TOWERS[itr].wy == wy)
        )
        {
            goto Done_Return_Zero;
        }
    }

    is_legal_move = ST_FALSE;

    unit_spaces = 0;

    if(Terrain_Is_Sailable(wx, wy, wp) == ST_TRUE)
    {
        have_windwalker = ST_FALSE;
        if(Unit_Has_WindWalking(unit_idx) == ST_TRUE)
        {
            is_legal_move = ST_TRUE;
            have_windwalker = ST_TRUE;
        }

        if(have_windwalker == ST_FALSE)
        {
            for(itr = 0; ((itr < troop_count) && (have_windwalker == ST_FALSE)); itr++)
            {
                trooper_idx = troops[itr];

                if(trooper_idx != -1)
                {
                    if(Unit_Has_WindWalking(trooper_idx) == ST_TRUE)
                    {
                        is_legal_move = ST_TRUE;
                        have_windwalker = ST_TRUE;
                    }
                }
            }
        }

        if(is_legal_move == ST_FALSE)
        {
            if(
                (Unit_Has_AirTravel(unit_idx) == ST_TRUE) ||
                (Unit_Has_WaterTravel(unit_idx) == ST_TRUE)
            )
            {
                is_legal_move = ST_TRUE;
            }
        }
    }
    else
    {
        if((_unit_type_table[_UNITS[unit_idx].type].Move_Flags & MV_SAILING) != 0)
        {
            if(Unit_Has_AirTravel(unit_idx) != ST_TRUE)
            {
                is_legal_move = ST_FALSE;
            }
            else
            {
                is_legal_move = ST_TRUE;
            }
        }
        else
        {
            is_legal_move = ST_TRUE;
        }
    }

    if(is_legal_move == ST_TRUE)
    {
        unit_spaces = (MAX_STACK - troop_count);
    }

    goto Done;


Done_Return_Zero:
    unit_spaces = 0;
    goto Done;


Done:

    return unit_spaces;
}


// WZD o100p11
// drake178: AI_UNIT_SeekTransprt()
// AI_UNIT_SeekTransprt()

// WZD o100p12
// drake178: AI_UNIT_BuildRoad()
// AI_UNIT_BuildRoad()

// WZD o100p13
// drake178: TILE_CanShiftPlanes()
// TILE_CanShiftPlanes()
