/*
    AI - Move

    WIZARDS.EXE
        ovr158
        ovr162
*/

#include "AIMOVE.H"

#include "MOX/random.H"
#include "STU/STU_CHK.H"

#include "MOX/MOM_Data.H"
#include "MOX/MOX_DAT.H"  /* _players[] */
#include "MOX/MOX_DEF.H"
#include "MOX/MOX_TYPE.H"
#include "MOX/Util.H"

#include "AIDATA.H"
#include "AIDUDES.H"
#include "CITYCALC.H"
#include "Combat.H"
#include "DIPLOMAC.H"
#include "NEXTTURN.H"
#include "RACETYPE.H"
#include "Spellbook.H"
#include "UNITTYPE.H"
#include "UnitMove.H"
#include "special.H"
#include "DIPLODEF.H"



// WZD dseg:D3EC                                                 BEGIN:  ovr158 - Uninitialized Data

// WZD dseg:D3EC
uint8_t * AI_ContinentType_Ptr;
// WZD dseg:D3EE
uint8_t * AI_Continent_Y_Ptr;
// WZD dseg:D3F0
uint8_t * AI_Continent_X_Ptr;

// WZD dseg:D3F2
int16_t G_Seafaring_Lowest_Value;
// WZD dseg:D3F4
int16_t G_Pushout_Lowest_Value;
// WZD dseg:D3F6
int16_t G_Seafaring_Count;
// WZD dseg:D3F8
int16_t G_Pushout_Units_Count;
// WZD dseg:D3FA
int16_t G_Seafaring_Values[9];
// WZD dseg:D40C
int16_t G_Pushout_Values[9];
// WZD dseg:D41E
int16_t G_Seafaring_CX_IDs[9];
// WZD dseg:D430
int16_t G_Pushout_CX_IDs[9];
// WZD dseg:D442
int16_t G_Seafaring_UL_Indices[9];
// WZD dseg:D454
int16_t G_Pushout_UL_Indices[9];
// WZD dseg:D466
int16_t G_Seafaring_Unit_Indices[9];
// WZD dseg:D478
int16_t G_Pushout_Unit_Indices[9];
// WZD dseg:D48A
int16_t AI_OnRallyPt_Count;
// WZD dseg:D48C
int16_t AI_RallyEnRouteCount;

// WZD dseg:D48E
int16_t g_ai_minattackstack;

// WZD dseg:D490
int16_t UU_AI_TargetingVar;

// WZD dseg:D492 00 00                                           dw 0

// WZD dseg:D492                                                 END:  ovr158 - Uninitialized Data




/*
    WIZARDS.EXE  ovr162
*/

// WZD o158p01
// drake178: AI_SetUnitOrders()
/*
processes continent reevaluation and gives orders to
all available units, including disbanding some if
necessary

many many BUGs
*/
/*



*/
void AI_SetUnitOrders__WIP(int16_t player_idx)
{
    int16_t wp = 0;
    int16_t landmass_idx = 0;  // _SI_


    ai_human_hostility = ST_FALSE;


    if(
        (
            (_players[player_idx].Hostility[HUMAN_PLAYER_IDX] < 3)
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


    ai_seektransport_cnt = 0;


    g_ai_minattackstack = (2 + (_turn / 30));


    if(g_ai_minattackstack > MAX_STACK)
    {

        g_ai_minattackstack = MAX_STACK;

    }


Check_Cities_Data();
    AI_Disband_To_Balance_Budget(player_idx);
Check_Cities_Data();


Check_Cities_Data();
    AI_Shift_Off_Home_Plane(player_idx);
Check_Cities_Data();


Check_Cities_Data();
    AI_Move_Out_Boats();
Check_Cities_Data();


Check_Cities_Data();
    AI_Find_Opportunity_City_Target(wp, player_idx);
Check_Cities_Data();


    for(wp = 0; wp < NUM_PLANES; wp++)
    {

        AI_Continent_X_Ptr = &_ai_continents.plane[wp].player[player_idx].X_Coords[0];

        AI_Continent_Y_Ptr = &_ai_continents.plane[wp].player[player_idx].Y_Coords[0];

        AI_ContinentType_Ptr = &_ai_continents.plane[wp].player[player_idx].Cont_Types[0];

        for(landmass_idx = 1; landmass_idx < NUM_LANDMASSES; landmass_idx++)
        {

Check_Cities_Data();
            AI_Build_Stacks_Find_Targets_Order_Moves(player_idx, landmass_idx, wp);
Check_Cities_Data();


Check_Cities_Data();
            AI_GarrBuilderPush__WIP(wp);
Check_Cities_Data();


Check_Cities_Data();
            AI_sEFBD6__WIP();
Check_Cities_Data();


Check_Cities_Data();
            AI_Do_Meld(player_idx);
Check_Cities_Data();


Check_Cities_Data();
            AI_Do_Settle(player_idx, landmass_idx);
Check_Cities_Data();


Check_Cities_Data();
            AI_Do_Purify(landmass_idx, wp);
Check_Cities_Data();


Check_Cities_Data();
            AI_Do_RoadBuild(landmass_idx);
Check_Cities_Data();


Check_Cities_Data();
            AI_Build_Target_List(player_idx, landmass_idx, wp);
Check_Cities_Data();


Check_Cities_Data();
            AI_ProcessRoamers__WIP(landmass_idx, wp, player_idx);
Check_Cities_Data();


            // ; should just test for 2...
            if(
                (AI_ContinentType_Ptr[landmass_idx] >= CONT_Abandon)     /* {..., CONT_Abandon: 5, CONT_NoTargets: 6 } */
                ||
                (AI_ContinentType_Ptr[landmass_idx] == CONT_Own)         /* 1 */
                ||
                (AI_ContinentType_Ptr[landmass_idx] == CONT_NoLanding)   /* 4 */
                ||
                (AI_ContinentType_Ptr[landmass_idx] == CONT_NoPresence)  /* 3 */
            )
            {

Check_Cities_Data();
                AI_PullForMainWar__WIP(player_idx, wp);
Check_Cities_Data();

            }


            if(
                (AI_ContinentType_Ptr[landmass_idx] >= CONT_Abandon)  /* {..., CONT_Abandon: 5, CONT_NoTargets: 6 } */
                ||
                (AI_ContinentType_Ptr[landmass_idx] == CONT_Own)
            )
            {

Check_Cities_Data();
                G_AI_HomeRallyFill__WIP(landmass_idx, wp, player_idx);
Check_Cities_Data();

            }


Check_Cities_Data();
            G_AI_RallyFill__WIP(landmass_idx, wp, player_idx);
Check_Cities_Data();


            if(
                (AI_ContinentType_Ptr[landmass_idx] == CONT_Own)
                ||
                (AI_ContinentType_Ptr[landmass_idx] == CONT_Contested)
                ||
                (AI_ContinentType_Ptr[landmass_idx] >= CONT_Abandon)
            )
            {

Check_Cities_Data();
                AI_FillGarrisons__WIP(player_idx, wp, landmass_idx);
Check_Cities_Data();

            }


        }


Check_Cities_Data();
        AI_ProcessOcean__WIP(player_idx, wp);
Check_Cities_Data();


Check_Cities_Data();
        G_AI_ProcessTransports__WIP(player_idx, wp);
Check_Cities_Data();


    }


    // DONT  EMM_Map_DataH()


}


// WZD o158p02
// drake178: G_AI_RallyFill()
/*
attempts to move enough units to the rally point to
create an offensive stack ... I think anyway
*/
/*

*/
void G_AI_RallyFill__WIP(int16_t landmass_idx, int16_t wp, int16_t player_idx)
{
    int16_t unit_list_idx = 0;
    int16_t unit_idx = 0;
    int16_t list_unit_idx = 0;
    int16_t gap_count = 0;
    int16_t itr = 0;  // _SI_

    if(AI_OnRallyPt_Count == MAX_STACK)
    {

        if(Random(20) == 1)
        {

            AI_SingleCont_Reeval__WIP(player_idx, landmass_idx, wp);

        }

    }

    if((AI_OnRallyPt_Count + AI_RallyEnRouteCount) < g_ai_minattackstack)  // ; turn / 30 + 2, max 9
    {

        if(
            (_ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] >= CONT_Abandon)
            ||
            (_ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] == CONT_NoPresence)
            ||
            ((AI_OnRallyPt_Count + AI_RallyEnRouteCount + G_Pushout_Units_Count) > g_ai_minattackstack)
        )
        {

            gap_count = (MAX_STACK - AI_RallyEnRouteCount);

            for(itr = 0; ((itr < gap_count) && (itr < G_Pushout_Units_Count)); itr++)
            {

                unit_list_idx = G_Pushout_CX_IDs[itr];

                list_unit_idx = G_Pushout_UL_Indices[itr];

                unit_idx = G_Pushout_Unit_Indices[itr];

                AI_Set_Move_Or_Goto_Target(unit_idx, AI_Continent_X_Ptr[landmass_idx], AI_Continent_Y_Ptr[landmass_idx], unit_list_idx, list_unit_idx);

            }

        }

    }

}


// WZD o158p03
// drake178: AI_FillGarrisons()
/*
; tries to fill all garrisons on the continent to their
; predefined minimum unit count using any roaming or
; building stack units (but not the actual builders)
*/
/*

*/
void AI_FillGarrisons__WIP(int16_t player_idx, int16_t wp, int16_t landmass_idx)
{
    int8_t Node_Units_Needed[NUM_NODES] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int8_t Node_Indices[NUM_NODES] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int8_t Garrison_Units_Needed[NUM_CITIES] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int8_t City_Indices[NUM_CITIES] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t TrollDwarfDraconian = 0;
    int16_t Fortress_City_Index = 0;
    int16_t G_Low_Threat = 0;
    int16_t site_added = 0;
    int16_t unit_idx = 0;
    int16_t target_type = 0;
    int16_t Units_To_Send = 0;
    int16_t stack_wy = 0;
    int16_t stack_wx = 0;
    int16_t target_wy = 0;
    int16_t target_wx = 0;
    int16_t target_value = 0;
    int16_t best_target_value = 0;
    int16_t Node_Arrays_Count = 0;
    int16_t City_Arrays_Count = 0;
    int16_t Asset_Y = 0;
    int16_t Asset_X = 0;
    int16_t Own_City_Y = 0;
    int16_t Own_City_X = 0;
    int16_t itr = 0;
    int16_t unit_type = 0;
    int16_t list_unit_count = 0;
    int16_t itr_list_units = 0;
    int16_t itr_stacks = 0;  // _SI_
    int16_t target_site_idx = 0;  // _DI_

    if(
        (_ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] < CONT_Abandon)
        &&
        (_ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] != CONT_Own)
    )
    {

        // CONT_Invalid, CONT_Contested, CONT_NoPresence, CONT_NoLanding
        G_Low_Threat = ST_FALSE;

    }
    else
    {
        // CONT_Own, CONT_Abandon, CONT_NoTargets
        G_Low_Threat = ST_TRUE;

    }

    Fortress_City_Index = player_idx;  // ¿ BUGBUG  useless? meaningless? invalid?

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

            Fortress_City_Index = itr;

        }

    }

    City_Arrays_Count = 0;

    for(itr = 0; itr < _cities; itr++)
    {

        if(
            (_CITIES[itr].owner_idx == player_idx)
            &&
            (_CITIES[itr].wp == wp)
        )
        {

            Own_City_X = _CITIES[itr].wx;

            Own_City_Y = _CITIES[itr].wy;

            TrollDwarfDraconian = ST_FALSE;

            if(
                (_CITIES[itr].race == rt_Dwarf)
                ||
                (_CITIES[itr].race == rt_Troll)
                ||
                (_CITIES[itr].race == rt_Draconian)
            )
            {

                TrollDwarfDraconian = ST_TRUE;

            }

            if(_landmasses[((wp * WORLD_SIZE) + (Own_City_Y * WORLD_WIDTH) + Own_City_X)] == landmass_idx)
            {

                site_added = ST_FALSE;

                if(itr == Fortress_City_Index)
                {

                    Garrison_Units_Needed[City_Arrays_Count] = MAX_STACK;

                }
                else
                {

                    if(
                        (G_Low_Threat == ST_FALSE)
                        &&
                        (TrollDwarfDraconian == ST_FALSE)
                    )
                    {

                        Garrison_Units_Needed[City_Arrays_Count] = (2 + (_CITIES[itr].population / 3));

                    }
                    else
                    {

                        Garrison_Units_Needed[City_Arrays_Count] = (2 + (_CITIES[itr].population / 4));

                    }

                }

                if(Garrison_Units_Needed[City_Arrays_Count] > MAX_STACK)
                {

                    Garrison_Units_Needed[City_Arrays_Count] = MAX_STACK;

                }

                for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
                {

                    if(site_added != ST_FALSE)
                    {

                        break;

                    }

                    if(
                        (_ai_own_stack_wx[itr_stacks] == Own_City_X)
                        &&
                        (_ai_own_stack_wy[itr_stacks] == Own_City_Y)
                    )
                    {

                        Garrison_Units_Needed[City_Arrays_Count] -= _ai_own_stack_unit_count[itr_stacks];

                        City_Indices[City_Arrays_Count] = itr;

                        if(Garrison_Units_Needed[City_Arrays_Count] > 0)
                        {

                            City_Arrays_Count++;

                        }

                        site_added = ST_TRUE;

                    }

                }

                if(site_added == ST_FALSE)
                {

                    City_Indices[City_Arrays_Count] = itr;

                    City_Arrays_Count++;

                }

            }

        }

    }


    Node_Arrays_Count = 0;

    for(itr = 0; itr < NUM_NODES; itr++)
    {

        if(_NODES[itr].wp == wp)
        {

            Asset_X = _NODES[itr].wx;

            Asset_Y = _NODES[itr].wx;

            if(CONTX_NmeStrMap[wp][((Asset_Y * WORLD_WIDTH) + Asset_X)] == AI_TARGET_SITE)
            {

                Asset_X = _NODES[itr].wx;

                Asset_Y = _NODES[itr].wx;

                if(_landmasses[((wp * WORLD_SIZE) + (Asset_Y * WORLD_WIDTH) + Asset_X)] == landmass_idx)
                {

                    site_added = ST_FALSE;

                    for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
                    {

                        if(site_added != ST_FALSE)
                        {

                            break;

                        }

                        if(
                            (_ai_own_stack_wx[itr_stacks] == Asset_X)
                            &&
                            (_ai_own_stack_wy[itr_stacks] == Asset_Y)
                        )
                        {

                            if(G_Low_Threat == ST_TRUE)
                            {

                                Node_Units_Needed[Node_Arrays_Count] = (8 - _ai_own_stack_unit_count[itr_stacks]);

                            }
                            else
                            {

                                Node_Units_Needed[Node_Arrays_Count] = (4 - _ai_own_stack_unit_count[itr_stacks]);

                            }

                            Node_Indices[Node_Arrays_Count] = itr;

                            if(Node_Units_Needed[Node_Arrays_Count] > 0)
                            {

                                Node_Arrays_Count++;

                            }

                            site_added = ST_TRUE;

                        }

                    }

                    if(site_added == ST_FALSE)
                    {

                        Node_Units_Needed[Node_Arrays_Count] = 8;  // ¿ BUGBUG  should if 4 else 8, as above ?

                        Node_Indices[Node_Arrays_Count] = itr;

                        Node_Arrays_Count++;

                    }

                }

            }

        }

    }


    for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
    {
        if(
            (_ai_own_stack_type[itr_stacks] == 0)
            ||
            (_ai_own_stack_type[itr_stacks] == 1)
        )
        {

            if(
                (_ai_own_stack_wx[itr_stacks] != AI_Continent_X_Ptr[landmass_idx])
                ||
                (_ai_own_stack_wy[itr_stacks] != AI_Continent_Y_Ptr[landmass_idx])
            )
            {

                list_unit_count = _ai_own_stack_unit_count[itr_stacks];

                stack_wx = _ai_own_stack_wx[itr_stacks];
                
                stack_wy = _ai_own_stack_wy[itr_stacks];
                
                // ; just did this above
                list_unit_count = _ai_own_stack_unit_count[itr_stacks];

                best_target_value = 1000;

                for(itr = 0; City_Arrays_Count > itr; itr++)
                {

                    if(Garrison_Units_Needed[itr] > 0)
                    {

                        target_value = (Delta_XY_With_Wrap(stack_wx, stack_wy, _CITIES[City_Indices[itr]].wx, _CITIES[City_Indices[itr]].wy, WORLD_WIDTH) - Garrison_Units_Needed[itr]);

                        if(target_value < best_target_value)
                        {

                            best_target_value = target_value;

                            target_wx = _CITIES[City_Indices[itr]].wx;

                            target_wy = _CITIES[City_Indices[itr]].wy;

                            Units_To_Send = Garrison_Units_Needed[itr];

                            target_type = 0;

                            target_site_idx = itr;

                        }

                    }

                }

                if(best_target_value == 1000)  // didn't find a city
                {

                    for(itr = 0; Node_Arrays_Count > itr; itr++)
                    {

                        if(Node_Units_Needed[itr] > 0)
                        {

                            target_value = (Delta_XY_With_Wrap(stack_wx, stack_wy, _NODES[City_Indices[itr]].wx, _NODES[City_Indices[itr]].wy, WORLD_WIDTH) - Node_Units_Needed[itr]);

                            if(target_value < best_target_value)
                            {

                                best_target_value = target_value;

                                target_wx = _NODES[City_Indices[itr]].wx;

                                target_wy = _NODES[City_Indices[itr]].wy;

                                Units_To_Send = Garrison_Units_Needed[itr];

                                target_type = 1;

                                target_site_idx = itr;

                            }

                        }

                    }

                    if(best_target_value < 1000)
                    {

                        for(itr_list_units = 0; itr_list_units < list_unit_count; itr_list_units++)
                        {

                            // ; already processed units also increase the index
                            if(itr_list_units >= Units_To_Send)
                            {

                                break;

                            }

                            unit_idx = _ai_own_stack_unit_list[itr_stacks][itr_list_units];

                            if(unit_idx != ST_UNDEFINED)
                            {

                                unit_type = _UNITS[unit_idx].type;

                                if(
                                    ((_unit_type_table[unit_type].Construction == 0))
                                    &&
                                    ((_unit_type_table[unit_type].Abilities & (UA_CREATEOUTPOST | UA_MELD)) == 0)
                                )
                                {

                                    AI_Set_Move_Or_Goto_Target(unit_idx, target_wx, target_wy, itr_stacks, itr_list_units);

                                    if(target_type == 0)
                                    {

                                        Garrison_Units_Needed[target_site_idx] -= 1;

                                    }
                                    else
                                    {

                                        Node_Units_Needed[target_site_idx] -= 1;

                                    }

                                }

                            }

                        }
                        
                    }

                }

            }

        }

    }

}


// WZD o158p04
// drake178: AI_ProcessOcean()
/*
sends all seafaring units out on the ocean towards
the main war or another target, and disbands any idle
stacks (no orders and no seafaring unit)
*/
/*

*/
void AI_ProcessOcean__WIP(int16_t player_idx, int16_t wp)
{
    int16_t stack_wy = 0;
    int16_t stack_wx = 0;
    int16_t list_unit_count = 0;
    int16_t itr_list_units = 0;
    int16_t itr_stacks = 0;
    int16_t Have_Seafarer = 0;
    int16_t Have_Assigned_Unit = 0;
    int16_t fortress_wy = 0;
    int16_t fortress_wx = 0;
    int16_t Lowest_Fortress_Distance = 0;
    int16_t Fortress_Distance = 0;
    int16_t MainWar_Rally_Y = 0;
    int16_t MainWar_Rally_X = 0;
    int16_t unit_idx = 0;
    int16_t itr = 0;  // _DI_

    if(AI_MainWarConts[wp][player_idx] == 0)
    {

        return;

    }

    MainWar_Rally_X = _ai_continents.plane[wp].player[player_idx].X_Coords[AI_MainWarConts[wp][player_idx]];

    MainWar_Rally_Y = _ai_continents.plane[wp].player[player_idx].Y_Coords[AI_MainWarConts[wp][player_idx]];

    fortress_wx = _FORTRESSES[player_idx].wx;

    fortress_wy = _FORTRESSES[player_idx].wy;

    if(
        (MainWar_Rally_X == 0)
        &&
        (MainWar_Rally_Y == 0)
    )
    {

        Lowest_Fortress_Distance = 1000;

        for(itr = 0; itr < NUM_LANDMASSES; itr++)
        {

            if(
                (_ai_continents.plane[wp].player[player_idx].Cont_Types[itr] == CONT_Contested)
                ||
                (_ai_continents.plane[wp].player[player_idx].Cont_Types[itr] == CONT_NoPresence)
            )
            {

                Fortress_Distance = Delta_XY_With_Wrap(fortress_wx, fortress_wy, _ai_continents.plane[wp].player[player_idx].X_Coords[itr], _ai_continents.plane[wp].player[player_idx].Y_Coords[itr], WORLD_WIDTH);

                if(Fortress_Distance < Lowest_Fortress_Distance)
                {
                    
                    Lowest_Fortress_Distance = Fortress_Distance;

                    MainWar_Rally_X = _ai_continents.plane[wp].player[player_idx].X_Coords[itr];

                    MainWar_Rally_Y = _ai_continents.plane[wp].player[player_idx].Y_Coords[itr];

                }

            }

        }


    }


    if(
        (MainWar_Rally_X != 0)
        &&
        (MainWar_Rally_Y != 0)
    )
    {

        for(itr = 0; itr < _units; itr++)
        {

            if(
                (_UNITS[itr].owner_idx == player_idx)
                &&
                (_UNITS[itr].wp == wp)
                &&
                (_landmasses[((wp * WORLD_SIZE) + (_UNITS[itr].wy * WORLD_WIDTH) + _UNITS[itr].wx)])
            )
            {

                if(
                    (_UNITS[itr].Status == us_Ready)
                    &&
                    ((_unit_type_table[_UNITS[itr].type].Abilities & UA_MELD) == 0)
                    &&
                    (_unit_type_table[_UNITS[itr].type].Transport == 0)
                )
                {

                    if(
                        (Unit_Has_AirTravel(itr) != ST_FALSE)
                        ||
                        (Unit_Has_WaterTravel(itr) != ST_FALSE)
                        ||
                        (Unit_Has_NonCorporeal(itr) != ST_FALSE)
                    )
                    {

                        _UNITS[itr].Status = us_GOTO;

                        _UNITS[itr].dst_wx = MainWar_Rally_X;
                        
                        _UNITS[itr].dst_wy = MainWar_Rally_Y;

                    }
                    
                }

            }

        }

    }


    AI_Build_Stacks_Find_Targets_Order_Moves(player_idx, 0, wp);


    for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
    {

        Have_Assigned_Unit = ST_FALSE;

        Have_Seafarer = ST_FALSE;

        list_unit_count = _ai_own_stack_unit_count[itr_stacks];

        for(itr_list_units = 0; itr_list_units < list_unit_count; itr_list_units++)
        {
            
            if(Have_Assigned_Unit != ST_FALSE)
            {
                
                break;

            }

            unit_idx = _ai_own_stack_unit_list[itr_stacks][itr_list_units];

            if(unit_idx == ST_UNDEFINED)
            {

                Have_Assigned_Unit = ST_TRUE;

            }
            else
            {

                if(
                    (Unit_Has_AirTravel(itr) != ST_FALSE)
                    ||
                    (Unit_Has_WaterTravel(itr) != ST_FALSE)
                    ||
                    (Unit_Has_NonCorporeal(itr) != ST_FALSE)
                )
                {
                    
                    Have_Seafarer = ST_TRUE;

                }

            }

        }

        if(
            (Have_Assigned_Unit == ST_FALSE)
            &&
            (Have_Seafarer == ST_FALSE)
        )
        {

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

                    Kill_Unit(itr, 0);

                }

            }

        }

    }

}


// WZD o158p05
// drake178: G_AI_ProcessTransports()
void G_AI_ProcessTransports__WIP(int16_t player_idx, int16_t wp)
{
    int16_t Transport_Spaces[15] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Have_Own_City = 0;
    int16_t Have_Allied_City = 0;
    int16_t GoingTo_Order = 0;
    int16_t Adjacent_Landmass = 0;
    int16_t Transport_Stack_Room = 0;
    int16_t Target_Queue_Index = 0;
    int16_t itr_units = 0;
    int16_t itr_list_units = 0;
    int16_t Adj_NonOcean_Y = 0;
    int16_t Adj_NonOcean_X = 0;
    int16_t stack_wy = 0;
    int16_t stack_wx = 0;
    int16_t Landing_Allowed = 0;
    int16_t unit_type = 0;
    int16_t Settler_In_Stack = 0;
    int16_t All_Tansport_Stack = 0;
    int16_t Transport_In_Stack = 0;
    int16_t itr2 = 0;
    int16_t list_unit_count = 0;
    int16_t Tile_Index = 0;
    int16_t Y_Offset = 0;
    int16_t X_Offset = 0;
    int16_t target_distance = 0;
    int16_t closest_target_distance = 0;
    int16_t target_wy = 0;
    int16_t target_wx = 0;
    int16_t itr_stacks = 0;
    int16_t itr1 = 0;  // _DI_


    GoingTo_Order = ST_FALSE;


    for(itr1 = 0; itr1 < 15; itr1++)
    {

        Transport_Spaces[itr1] = MAX_STACK;

    }


// ; if the unit is going to a set of transport queue
// ; coordinates and is a transport, reduce the
// ; corresponding array element by its capacity plus 1
// ; 
// ; BUG: uses the wrong unit index for the transport
// ;  check
// ; BUG: should check transport first rather than last
// ; BUG? how does this 9-x calculation work?

    for(itr2 = 0; itr2 < _units; itr2++)
    {

        if(
            (_UNITS[itr2].Status == us_GOTO)
            &&
            (_UNITS[itr2].owner_idx == player_idx)
            &&
            (_UNITS[itr2].wp == wp)
        )
        {

            for(itr1 = 0; itr1 < ai_seektransport_cnt; itr1++)
            {

                if(
                    (_UNITS[itr2].dst_wx == AI_SeekTransport_Xs[itr1])
                    &&
                    (_UNITS[itr2].dst_wy == AI_SeekTransport_Ys[itr1])
                    &&
                    (AI_SeekTransport_Ps[itr1] == wp)
                )
                {

                    // ; BUG: this is not the unit index
                    Transport_Spaces[itr1] -= (_unit_type_table[_UNITS[itr1].type].Transport + 1);

                }

            }

        }

    }


    for(itr1 = 0; itr1 < ai_seektransport_cnt; itr1++)
    {

        if(Transport_Spaces[itr1] <= 0)
        {

            AI_SeekTransport_Ps[itr1] = ST_UNDEFINED;

        }
        
    }


    // ; just did this in the previous function
    AI_Build_Stacks_Find_Targets_Order_Moves(player_idx, 0, wp);


    AI_Do_Meld(player_idx);


    for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
    {

        Transport_In_Stack = ST_FALSE;

        All_Tansport_Stack = ST_TRUE;

        Settler_In_Stack = ST_FALSE;

        GoingTo_Order = ST_FALSE;

        list_unit_count = _ai_own_stack_unit_count[itr_stacks];

        stack_wx = _ai_own_stack_wx[itr_stacks];

        stack_wy = _ai_own_stack_wy[itr_stacks];


// ; evaluate some characteristics of the units in the
// ; stack

        for(itr_units = 0; itr_units < _units; itr_units++)
        {

            if(
                (_UNITS[itr_units].wx == stack_wx)
                &&
                (_UNITS[itr_units].wy == stack_wy)
                &&
                (_UNITS[itr_units].wp == wp)
            )
            {

                unit_type = _UNITS[itr_units].type;

                if(_unit_type_table[unit_type].Transport > 0)
                {

                    Transport_In_Stack = ST_TRUE;

                }
                else
                {

                    All_Tansport_Stack = ST_FALSE;

                    if(_unit_type_table[unit_type].Abilities & UA_CREATEOUTPOST)
                    {

                        Settler_In_Stack = ST_TRUE;

                    }

                }

                if(_UNITS[itr_units].Status == us_GOTO)
                {

                    GoingTo_Order = ST_TRUE;

                }

            }


        }


// NOTE: the code-flow of the conditions/branching here *feel* *odd*...

        if(Transport_In_Stack == ST_TRUE)
        {


            if(All_Tansport_Stack != ST_FALSE)
            {

// ; find the closest transport request queue tile

                closest_target_distance = 10000;

                for(itr1 = 0; itr1 < ai_seektransport_cnt; itr1++)
                {

                    if(AI_SeekTransport_Ps[itr1] == wp)
                    {

                        target_distance = Delta_XY_With_Wrap(stack_wx, stack_wy, AI_SeekTransport_Xs[itr1], AI_SeekTransport_Ys[itr1], WORLD_WIDTH);

                        if(target_distance < closest_target_distance)
                        {
                            closest_target_distance = target_distance;

                            target_wx = AI_SeekTransport_Xs[itr1];
                            
                            target_wy = AI_SeekTransport_Ys[itr1];
                            
                            Target_Queue_Index = itr1;

                        }

                    }

                }

                if(closest_target_distance < 10000)
                {

// send transports from the stack to the queue location
// until either it's full (in which case set its plane
// to -1) or the transport stack is all assigned

                    Transport_Stack_Room = Transport_Spaces[Target_Queue_Index];

                    for(itr_list_units = 0; itr_list_units < list_unit_count; itr_list_units++)
                    {

                        if(Transport_Stack_Room > 0)
                        {

                            itr_units = _ai_own_stack_unit_list[itr_stacks][itr_list_units];

                            Transport_Stack_Room -= (_unit_type_table[_UNITS[itr_units].type].Transport + 1);

                            AI_Set_Move_Or_Goto_Target(itr_units, target_wx, target_wy, itr_stacks, itr_list_units);

                        }

                    }

                    Transport_Spaces[Target_Queue_Index] = Transport_Stack_Room;

                    if(Transport_Stack_Room <= 0)
                    {

                        AI_SeekTransport_Ps[Target_Queue_Index] = ST_UNDEFINED;

                    }

                }

            }
            else
            {

                if(GoingTo_Order == ST_FALSE)
                {

// ; check whether there is an adjacent land tile, and if
// ; so, store the coordinates of the last one
// ; 
// ; BUG: the coordinates are not range checked

                    Adj_NonOcean_X = 0;

                    Adj_NonOcean_Y = 0;

                    Landing_Allowed = ST_FALSE;

                    for(Y_Offset = -1; Y_Offset < 2; Y_Offset++)
                    {

                        for(X_Offset = -1; X_Offset < 2; X_Offset++)
                        {
                            
                            // BUGBUG bad indexing, incorrect order of operation
                            // (CONTX_NmeStrMap[wp][(((stack_wy * WORLD_WIDTH) + Y_Offset) + (X_Offset + stack_wx))] == 0)
                            // (CONTX_NmeStrMap[wp][(((Y_Offset + stack_wy) * WORLD_WIDTH) + (X_Offset + stack_wx))] == 0)
                            if(
                                (_landmasses[((wp * WORLD_SIZE) + ((Y_Offset + stack_wy) * WORLD_WIDTH) + (X_Offset + stack_wx))] == 0)  // ; BUG: no range checks
                                &&
                                (CONTX_NmeStrMap[wp][(((stack_wy * WORLD_WIDTH) + Y_Offset) + (X_Offset + stack_wx))] == 0)  // ; BUG: no range checks
                            )
                            {

                                Adjacent_Landmass = _landmasses[((wp * WORLD_SIZE) + ((Y_Offset + stack_wy) * WORLD_WIDTH) + (X_Offset + stack_wx))];

                                Adj_NonOcean_X = (X_Offset + stack_wx);

                                Adj_NonOcean_Y = (Y_Offset + stack_wy);

                                Landing_Allowed = ST_TRUE;

                            }

                        }

                    }


                    if(Landing_Allowed == ST_TRUE)
                    {

                        if(Settler_In_Stack != ST_TRUE)
                        {

// ; check whether one of the adjacent tiles belongs to
// ; the main war continent
// ; 
// ; BUG: no range checking

                            Landing_Allowed = ST_FALSE;

                            for(Y_Offset = -1; Y_Offset < 2; Y_Offset++)
                            {

                                for(X_Offset = -1; X_Offset < 2; X_Offset++)
                                {

                                    if(_landmasses[((wp * WORLD_SIZE) + ((Y_Offset + stack_wy) * WORLD_WIDTH) + (X_Offset + stack_wx))] == AI_MainWarConts[wp][player_idx])
                                    {

                                        Adjacent_Landmass = _landmasses[((wp * WORLD_SIZE) + ((Y_Offset + stack_wy) * WORLD_WIDTH) + (X_Offset + stack_wx))];

                                        Adj_NonOcean_X = (X_Offset + stack_wx);

                                        Adj_NonOcean_Y = (Y_Offset + stack_wy);

                                        Landing_Allowed = ST_TRUE;

                                    }

                                }

                            }


                            if(Landing_Allowed == ST_FALSE)
                            {

// ; check whether the stored adjacent continent has an
// ; own or allied city
// ; 
// ; BUG: only looks at the last stored continent

                                Have_Own_City = ST_FALSE;

                                Have_Allied_City = ST_FALSE;

                                for(itr1 = 0; itr1 < _cities; itr1++)
                                {

                                    if(
                                        (_CITIES[itr1].wp == wp)
                                        &&
                                        (_landmasses[((wp * WORLD_SIZE) + (_CITIES[itr1].wy * WORLD_WIDTH) + _CITIES[itr1].wx)] == Adjacent_Landmass)
                                    )
                                    {

                                        if(_CITIES[itr1].owner_idx == player_idx)
                                        {

                                            Have_Own_City = ST_TRUE;

                                        }
                                        else
                                        {

                                            if(_players[player_idx].Dipl.Dipl_Status[_CITIES[itr1].owner_idx] == DIPL_Alliance)
                                            {

                                                Have_Allied_City = ST_TRUE;

                                            }

                                        }

                                    }

                                }

                            }

                            if(
                                (Have_Own_City == ST_TRUE)
                                ||
                                (Have_Allied_City == ST_TRUE)
                            )
                            {

                                Landing_Allowed = ST_FALSE;

                            }

                        }
                        else
                        {

// ; check whether one of the adjacent tiles belongs to
// ; the next colony continent
// ; 
// ; BUG: no range checking

                            for(Y_Offset = -1; Y_Offset < 2; Y_Offset++)
                            {

                                for(X_Offset = -1; X_Offset < 2; X_Offset++)
                                {

                                    if(
                                        (_landmasses[((wp * WORLD_SIZE) + ((Y_Offset + stack_wy) * WORLD_WIDTH) + (X_Offset + stack_wx))] == AI_MainWarConts[wp][player_idx])
                                        &&
                                        AI_NewColConts[wp][player_idx] != 0
                                    )
                                    {

                                        Adjacent_Landmass = _landmasses[((wp * WORLD_SIZE) + ((Y_Offset + stack_wy) * WORLD_WIDTH) + (X_Offset + stack_wx))];

                                        Adj_NonOcean_X = (X_Offset + stack_wx);

                                        Adj_NonOcean_Y = (Y_Offset + stack_wy);

                                    }

                                }

                            }

                            if(
                                (
                                    (AI_Enemy_Unit_In_Range(stack_wx, stack_wy, wp, 5, player_idx, Adjacent_Landmass) != ST_FALSE)
                                    ||
                                    (_ai_continents.plane[wp].player[player_idx].Cont_Types[Adjacent_Landmass] != CONT_NoPresence)
                                )
                                &&
                                (AI_NewColConts[wp][player_idx] != Adjacent_Landmass)
                            )
                            {

                                Landing_Allowed = ST_FALSE;

                            }
                            else
                            {

                                Landing_Allowed = ST_TRUE;

                            }

                        }

                    }


                    if(Landing_Allowed == ST_TRUE)
                    {

// ; set all transports in the stack to seek transport,
// ; and all other units to move to the stored adjacent
// ; tile
// ; 
// ; BUG: fails to check for already assigned units
// ; BUG? setting ships to seek transport?

                        for(itr_list_units = 0; itr_list_units < list_unit_count; itr_list_units++)
                        {

                            itr_units = _ai_own_stack_unit_list[itr_stacks][itr_list_units];

// ; BUG: need to check for -1 first

                            unit_type = _UNITS[itr_units].type;

                            if(_unit_type_table[unit_type].Transport > 0)
                            {

                                AI_Order_SeekTransport(itr_units, itr_stacks, itr_list_units);

                            }
                            else
                            {

                                AI_Set_Move_Or_Goto_Target(itr_units, Adj_NonOcean_X, Adj_NonOcean_Y, itr_stacks, itr_list_units);

                            }

                        }

                    }
                    else
                    {

                        if(
                            (Settler_In_Stack == ST_TRUE)
                            &&
                            (AI_NewColConts[wp][player_idx] != 0)
                        )
                        {

// ; if the new colony's rally point has an adjacent ocean
// ; tile, set all units in the stack to move there
// ; 
// ; BUG: no range checking

                            Adj_NonOcean_X = 0;

                            Adj_NonOcean_Y = 0;

                            for(Y_Offset = -1; Y_Offset < 2; Y_Offset++)
                            {

                                for(X_Offset = -1; X_Offset < 2; X_Offset++)
                                {

                                    if(_landmasses[((wp * WORLD_SIZE) + ((Y_Offset + AI_NewColTgtYs[wp][player_idx]) * WORLD_WIDTH) + (X_Offset + AI_NewColTgtXs[wp][player_idx]))] != 0)
                                    {

                                        Adj_NonOcean_X = (X_Offset + AI_NewColTgtXs[wp][player_idx]);

                                        Adj_NonOcean_Y = (Y_Offset + AI_NewColTgtYs[wp][player_idx]);

                                        Landing_Allowed = ST_TRUE;

                                    }

                                }

                            }

                            for(itr_list_units = 0; itr_list_units < list_unit_count; itr_list_units++)
                            {

                                itr_units = _ai_own_stack_unit_list[itr_stacks][itr_list_units];

                                AI_Set_Move_Or_Goto_Target(itr_units, Adj_NonOcean_X, Adj_NonOcean_Y, itr_stacks, itr_list_units);

                            }



                        }
                        else
                        {

                            if(AI_MainWarConts[wp][player_idx] != 0)
                            {

// ; find the closest empty unload tile on the main war
// ; continent, if any

                                Adj_NonOcean_X = stack_wx;

                                Adj_NonOcean_Y = stack_wy;

                                Tile_Index = CONTX_1stLoadTs[wp][AI_MainWarConts[wp][player_idx]];

                                closest_target_distance = 1000;

                                if(Tile_Index != ST_UNDEFINED)
                                {

                                    target_distance = Delta_XY_With_Wrap(CONTX_LoadTileXs[wp][Tile_Index], CONTX_LoadTileYs[wp][Tile_Index], Adj_NonOcean_X, Adj_NonOcean_Y, WORLD_WIDTH);

                                    if(target_distance < closest_target_distance)
                                    {

                                        X_Offset = CONTX_LoadTileXs[wp][Tile_Index];

                                        Y_Offset = CONTX_LoadTileYs[wp][Tile_Index];

                                        if(CONTX_NmeStrMap[wp][((Y_Offset * WORLD_WIDTH) + X_Offset)] == 0)  // empty map square
                                        {

                                            closest_target_distance = target_distance;

                                            target_wx = X_Offset;

                                            target_wy = Y_Offset;

                                        }

                                    }

                                        Tile_Index = CONTX_LoadTChain[wp][Tile_Index];

                                }


                                if(closest_target_distance < 1000)
                                {

// ; set all units in the stack to move to either an ocean
// ; tile adjacent to the target tile if one exists, or
// ; to their current location if there aren't any
// ; 
// ; BUG? possible move order to current position

                                    for(Y_Offset = -1; Y_Offset < 2; Y_Offset++)
                                    {

                                        for(X_Offset = -1; X_Offset < 2; X_Offset++)
                                        {

                                            if(_landmasses[((wp * WORLD_SIZE) + ((target_wy + Y_Offset) * WORLD_WIDTH) + (target_wx + X_Offset))] != 0)
                                            {

                                                Adj_NonOcean_X = (target_wx + X_Offset);

                                                Adj_NonOcean_Y = (target_wy + Y_Offset);

                                            }

                                        }

                                    }

                                    for(itr_list_units = 0; itr_list_units < list_unit_count; itr_list_units++)
                                    {

                                        itr_units = _ai_own_stack_unit_list[itr_stacks][itr_list_units];

                                        AI_Set_Move_Or_Goto_Target(itr_units, Adj_NonOcean_X, Adj_NonOcean_Y, itr_stacks, itr_list_units);

                                    }

                                }

                            }

                        }

                    }

                }

            }

        }

    }

}


// WZD o158p06
// drake178: AI_ProcessRoamers()
/*
searches for and assigns targets and corresponding
move orders to all roamer stack on the continent or,
if there are no targets but at least one roaming
stack with at least 7 units, changes the type and
rally point of the continent to prepare for troops to
move to the action continent instead

WARNING: when setting the leave continent type, the
rally point chosen ignores own units, and can't be
a tile with a city, tower, or node either
*/
/*

*/
void AI_ProcessRoamers__WIP(int16_t landmass_idx, int16_t wp, int16_t player_idx)
{
    int16_t Have_Large_Stack = 0;
    int16_t Tile_Count = 0;
    int16_t Closest_Landing_Distance = 0;
    int16_t Best_LoadTile_Y = 0;
    int16_t No_Targets = 0;
    int16_t LoadTile_Y = 0;
    int16_t LoadTile_X = 0;
    int16_t Best_LoadTile_X = 0;
    int16_t Next_Tile_ChainIndex = 0;
    int16_t Midpoint_Y = 0;
    int16_t Midpoint_X = 0;
    int16_t Have_Processed_Unit = 0;
    int16_t unit_idx = 0;
    int16_t list_unit_count = 0;
    int16_t UU_Have_Landlubbers = 0;
    int16_t itr_list_units = 0;
    int16_t target_wy = 0;
    int16_t target_wx = 0;
    int16_t stack_wy = 0;
    int16_t stack_wx = 0;
    int16_t itr_stacks = 0;  // _DI_

    Have_Large_Stack = ST_FALSE;

    No_Targets = ST_TRUE;

// ; if possible, assign targets to all roaming stacks
// ; that do not have a unit that is already given a task,
// ; while also looking for stacks of more than 7 units

    for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
    {

        if(_ai_own_stack_type[itr_stacks] == 1)
        {

            list_unit_count = _ai_own_stack_unit_count[itr_stacks];

            stack_wx = _ai_own_stack_wx[itr_stacks];

            stack_wy = _ai_own_stack_wy[itr_stacks];

            if(list_unit_count >= 8)
            {
                Have_Large_Stack = ST_TRUE;
            }

// ; check for processed units and land only units
// ; 
// ; WARNING: the latter is not evaluated properly by the
// ; called functions (but is not used anyway)

            if(Have_Processed_Unit == ST_FALSE)
            {

                if(AI_AssignStackTarget__WIP(stack_wx, stack_wy, &target_wx, &target_wy, itr_stacks, itr_list_units) == ST_TRUE)
                {

// ; clear the no targets indicator, and assign every unit
// ; in the stack a move order to the target coordinates

                    No_Targets = ST_FALSE;

                    for(itr_list_units = 0; itr_list_units < list_unit_count; itr_list_units++)
                    {

                        unit_idx = _ai_own_stack_unit_list[itr_stacks][itr_list_units];

                        AI_Set_Move_Or_Goto_Target(unit_idx, target_wx, target_wy, itr_stacks, itr_list_units);

                    }

                }

            }

            UU_AI_TargetingVar = 0;

        }

    }


    if(
        (Have_Large_Stack == ST_TRUE)
        &&
        (No_Targets == ST_TRUE)
        &&
        (AI_MainWarConts[wp][player_idx] != 0)
        &&
        (AI_MainWarConts[wp][player_idx] != landmass_idx)
    )
    {

// ; find the nearest empty transport loading tile to the
// ; center of the action continent, if any
// ; 
// ; WARNING: ignores own units on the tile

        Midpoint_X = 0;

        Midpoint_Y = 0;

        Tile_Count = 0;

        Next_Tile_ChainIndex = CONTX_1stLoadTs[wp][AI_MainWarConts[wp][player_idx]];

        // while(Next_Tile_ChainIndex = CONTX_LoadTChain[wp][Next_Tile_ChainIndex] != ST_UNDEFINED)
        while(Next_Tile_ChainIndex != ST_UNDEFINED)
        {

            Midpoint_X += CONTX_LoadTileXs[wp][Next_Tile_ChainIndex];

            Midpoint_X += CONTX_LoadTileYs[wp][Next_Tile_ChainIndex];

            Tile_Count++;

            Next_Tile_ChainIndex = CONTX_LoadTChain[wp][Next_Tile_ChainIndex];

        }

            Midpoint_X /= Tile_Count;

            Midpoint_Y /= Tile_Count;

            Closest_Landing_Distance = 1000;

            Next_Tile_ChainIndex = CONTX_1stLoadTs[wp][landmass_idx];

        // while(Next_Tile_ChainIndex = CONTX_LoadTChain[wp][Next_Tile_ChainIndex] != ST_UNDEFINED)
        while(Next_Tile_ChainIndex != ST_UNDEFINED)
        {

            LoadTile_X = CONTX_LoadTileXs[wp][Next_Tile_ChainIndex];

            LoadTile_X = CONTX_LoadTileYs[wp][Next_Tile_ChainIndex];

            if(CONTX_NmeStrMap[wp][((LoadTile_Y * WORLD_WIDTH) + LoadTile_X)] == 0)
            {

                Tile_Count = Delta_XY_With_Wrap(Midpoint_X, Midpoint_Y, LoadTile_X, LoadTile_Y, WORLD_WIDTH);

                if(Tile_Count < Closest_Landing_Distance)
                {

                    Closest_Landing_Distance = Tile_Count;

                    Best_LoadTile_X = LoadTile_X;

                    Best_LoadTile_Y = LoadTile_Y;

                }

            }

            Next_Tile_ChainIndex = CONTX_LoadTChain[wp][Next_Tile_ChainIndex];

        }
        
    }


    if(Closest_Landing_Distance != 1000)
    {

        _ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] = CONT_Abandon;

        _ai_continents.plane[wp].player[player_idx].X_Coords[landmass_idx] = Best_LoadTile_X;

        _ai_continents.plane[wp].player[player_idx].Y_Coords[landmass_idx] = Best_LoadTile_Y;

    }

}


// WZD o158p07
// drake178: G_AI_HomeRallyFill()
/*
tries to fill the stack at the rally point and load
all excess units onto ships afterward

full of BUGs, it's a complete mess
*/
/*

*/
void G_AI_HomeRallyFill__WIP(int16_t landmass_idx, int16_t wp, int16_t player_idx)
{
    int16_t stage_wy = 0;
    int16_t stage_wx = 0;
    int16_t stack_wy = 0;
    int16_t stack_wx = 0;
    int16_t itr_stacks = 0;
    int16_t IDK_count = 0;


    stage_wx = _ai_continents.plane[wp].player[player_idx].X_Coords[landmass_idx];

    stage_wy = _ai_continents.plane[wp].player[player_idx].Y_Coords[landmass_idx];

    IDK_count = 0;

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

            IDK_count = (MAX_STACK - _ai_own_stack_unit_count[itr_stacks]);

        }

    }


    for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
    {

        if(
            (_ai_own_stack_type[itr_stacks] == 1)
            &&
            (_ai_own_stack_unit_list[itr_stacks][0] != ST_UNDEFINED)
        )
        {

            G_AI_RallyOrFerry__WIP(itr_stacks, landmass_idx, wp, stage_wx, stage_wy, IDK_count, player_idx);

            IDK_count -= _ai_own_stack_unit_count[itr_stacks];

        }

    }

}


// WZD o158p08
// drake178: AI_PullForMainWar()
/*
; send all unprocessed flying and seafaring units that
; are not in a garrison to the main war continent, with
; the exception of spirits
;
; BUG: the functions used do not cover all cases
*/
/*

*/
void AI_PullForMainWar__WIP(int16_t player_idx, int16_t wp)
{
    int16_t stack_wy = 0;
    int16_t stack_wx = 0;
    int16_t list_unit_count = 0;
    int16_t unit_idx = 0;
    int16_t itr_list_units = 0;
    int16_t itr_stacks = 0;  // _SI_


    if(AI_MainWarConts[wp][player_idx] == 0)
    {

        return;

    }


    for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
    {

        if(
            (_ai_own_stack_type[itr_stacks] == 1)
            ||
            (_ai_own_stack_type[itr_stacks] == 0)
        )
        {


            list_unit_count = _ai_own_stack_unit_count[itr_stacks];

            stack_wx = _ai_own_stack_wx[itr_stacks];

            stack_wy = _ai_own_stack_wy[itr_stacks];

            if(_landmasses[((wp * WORLD_SIZE) + (stack_wy * WORLD_WIDTH) + stack_wx)] == AI_MainWarConts[wp][player_idx])
            {

// ; send all unassigned flying and seafaring units in the
// ; stack to the main war continent (give move orders),
// ; except those that can Meld
// ; 
// ; BUG: the functions used don't cover all cases

                for(itr_list_units = 0; itr_list_units < list_unit_count; itr_list_units++)
                {

                    unit_idx = _ai_own_stack_unit_list[itr_stacks][itr_list_units];

                    if(
                        (
                            (Unit_Has_AirTravel(unit_idx) == ST_TRUE)
                            ||
                            (Unit_Has_WaterTravel(unit_idx) == ST_TRUE)
                        )
                        &&
                        ((_unit_type_table[_UNITS[unit_idx].type].Abilities & UA_MELD) == 0)
                    )
                    {

                        AI_Set_Move_Or_Goto_Target(unit_idx, _ai_continents.plane[wp].player[player_idx].X_Coords[AI_MainWarConts[wp][player_idx]], _ai_continents.plane[wp].player[player_idx].Y_Coords[AI_MainWarConts[wp][player_idx]], itr_stacks, itr_list_units);

                    }

                }

            }

        }

    }

}


// WZD o158p09
// drake178: G_AI_RallyOrFerry()
/*
gives any unassigned units move orders to the passed
tile, gives any adjacent ships a halt order (move to
their current location), and tries to move any
remaining units in the stack to a ship if there is
one adjacent

full of BUGs and inconsistencies
*/
/*

*/
void G_AI_RallyOrFerry__WIP(int16_t stack_idx, int16_t landmass_idx, int16_t wp, int16_t stage_wx, int16_t stage_wy, int16_t IDK_count, int16_t player_idx)
{
    int16_t unit_type = 0;
    int16_t Max_Y = 0;
    int16_t Min_Y = 0;
    int16_t Max_X = 0;
    int16_t Min_X = 0;
    int16_t Ocean_Y = 0;
    int16_t Ocean_X = 0;
    int16_t unit_idx = 0;
    int16_t list_unit_count = 0;
    int16_t stack_wy = 0;
    int16_t stack_wx = 0;
    int16_t Space_On_Tile = 0;
    int16_t Total_Transport = 0;
    int16_t UU_Embarkable_Y = 0;
    int16_t UU_Embarkable_X = 0;
    int16_t UU_Landmass = 0;
    int16_t Ocean_Unit_Y = 0;
    int16_t Ocean_Unit_X = 0;
    int16_t Adjacent_X = 0;
    int16_t Y_Offset = 0;
    int16_t itr_units = 0;
    int16_t Nearby_Ocean_Stack = 0;
    int16_t UU_OceanFound = 0;
    int16_t itr_list_units = 0;  // _SI_
    int16_t X_Offset = 0;  // _DI_

    UU_Landmass = landmass_idx;

    stack_wx = _ai_own_stack_wx[stack_idx];

    stack_wy = _ai_own_stack_wy[stack_idx];

    Ocean_X = stack_wx;

    Ocean_Y = stack_wy;

// ; find the last ocean tile in the 3 by 3 area centered
// ; around the stack
// ; 
// ; BUG: there is no guarantee that there will be one
// ; BUG: doesn't validate the coordinates

    for(Y_Offset = -1; Y_Offset < 2; Y_Offset++)
    {

        for(Y_Offset = -1; Y_Offset < 2; Y_Offset++)
        {

            if(_landmasses[((wp * WORLD_SIZE) + ((Y_Offset + stage_wy) * WORLD_WIDTH) + (X_Offset + stage_wx))] == 0)
            {

                Ocean_X = (X_Offset + stage_wx);

                Ocean_Y = (Y_Offset + stage_wy);

            }

        }

    }

// ; BUG: will add the original land tile if there was no
// ; ocean around the stack

    AI_SeekTransportFrom__WIP(Ocean_X, Ocean_Y, wp);


// ; keep giving move orders to the target destination to
// ; any unassigned units in the stack until they are
// ; either all assigned, or the required count is filled
// ; 
// ; BUG: never checks if the units can actually go there

    if(
        (stack_wx != stage_wx)
        ||
        (stack_wy != stage_wy)
    )
    {

        list_unit_count = _ai_own_stack_unit_count[stack_idx];

        for(itr_list_units = 0; ((itr_list_units < list_unit_count) && (IDK_count > 0)); itr_list_units++)
        {

            unit_idx = _ai_own_stack_unit_list[stack_idx][itr_list_units];

            AI_Set_Move_Or_Goto_Target(unit_idx, stage_wx, stage_wy, stack_idx, itr_list_units);

            IDK_count--;

        }

    }


// ; find the last embarkable tile in the 3 by 3 area
// ; centered around the stack
// ; 
// ; BUG: no guarantee that there is a valid tile
// ; BUG: the result variables are uninitialized
// ; BUG: the function used to validate tiles does not
// ; return true for all embarkable ones

    for(Y_Offset = (stack_wy - 1); (stack_wy + 2) > Y_Offset; Y_Offset++)
    {

        if(
            (Y_Offset > 0)
            &&
            (Y_Offset < WORLD_HEIGHT)
        )
        {

            for(Adjacent_X = (stack_wx - 1); (stack_wx + 2) > Adjacent_X; Adjacent_X++)
            {

                X_Offset = Adjacent_X;

                if(X_Offset < 0)
                {

                    X_Offset += WORLD_WIDTH;

                }

                if(X_Offset >= WORLD_WIDTH)
                {

                    X_Offset -= WORLD_WIDTH;

                }

                if(Map_Square_Is_Embarkable(X_Offset, Y_Offset, wp) != ST_FALSE)
                {

                    UU_OceanFound = ST_TRUE;

                    UU_Embarkable_X = Adjacent_X;

                    UU_Embarkable_Y = Y_Offset;

                }

            }

        }

    }

// ; check the 3 by 3 area around the stack to determine
// ; whether there are any ocean stacks there, and count
// ; the units that need transportation as well as the
// ; capacity of the transports
// ; 
// ; BUG: fails to check the plane the units are on
// ; BUG: the check for seafaring units is inadequate
// ; BUG? assigns any transports found a GoingTo order to
// ;  the same tile that they are already on
// ; BUG? counts the units on all 9 tiles together

    Nearby_Ocean_Stack = ST_FALSE;

    Min_X = (stack_wx - 1);

    Min_Y = (stack_wx - 1);

    Max_X = (stack_wx + 1);

    Max_Y = (stack_wx + 1);

    Total_Transport = 0;

    Space_On_Tile = MAX_STACK;

    for(itr_units = 0; itr_units < _units; itr_units++)
    {

        if(_UNITS[itr_units].owner_idx == player_idx)
        {

            X_Offset = _UNITS[itr_units].wx;

            Y_Offset = _UNITS[itr_units].wy;

            if(
                (X_Offset >= Min_X)
                &&
                (Y_Offset >= Min_Y)
                &&
                (X_Offset <= Max_X)
                &&
                (Y_Offset <= Max_Y)
            )
            {

                if(_landmasses[((wp * WORLD_SIZE) + (Y_Offset * WORLD_WIDTH) + X_Offset)] == 0)
                {

                    Nearby_Ocean_Stack = ST_TRUE;

                    Ocean_Unit_X = _UNITS[itr_units].wx;
                    
                    Ocean_Unit_Y = _UNITS[itr_units].wy;

                    unit_type = _UNITS[itr_units].type;

                    if(_unit_type_table[unit_type].Transport > 0)
                    {

                        _UNITS[itr_units].dst_wx = _UNITS[itr_units].wx;
                        
                        _UNITS[itr_units].dst_wy = _UNITS[itr_units].wy;

                        _UNITS[itr_units].Status = us_GOTO;

                        Total_Transport += _unit_type_table[unit_type].Transport;

                    }
                    else
                    {

                        if(
                            (Unit_Has_AirTravel(itr_units) == ST_FALSE)
                            &&
                            (Unit_Has_WaterTravel(itr_units) == ST_FALSE)
                        )
                        {

                            Total_Transport--;

                        }

                    }

                }

            }    

        }

    }


    if(Nearby_Ocean_Stack == ST_TRUE)
    {

        for(itr_list_units = 0; _ai_own_stack_unit_count[stack_idx] > itr_list_units; itr_list_units++)
        {

            if(itr_list_units < Space_On_Tile)
            {

                if(Total_Transport > 0)
                {

                    unit_idx = _ai_own_stack_unit_list[stack_idx][itr_list_units];

                    AI_Set_Move_Or_Goto_Target(unit_idx, Ocean_Unit_X, Ocean_Unit_Y, stack_idx, itr_list_units);

                    if(
                        (Unit_Has_AirTravel(unit_idx) == ST_FALSE)
                        ||
                        (Unit_Has_WaterTravel(unit_idx) == ST_FALSE)
                    )
                    {

                        Total_Transport--;

                    }

                }

            }

        }

    }
    else
    {

        for(itr_list_units = 0; _ai_own_stack_unit_count[stack_idx] > itr_list_units; itr_list_units++)
        {

            if(itr_list_units < Space_On_Tile)
            {

                // ; conflicting condition - will never jump
                if(Total_Transport > 0)
                {

//  ; code path can't be reached - transport count is
// ; always zero if there was no ocean stack

                    unit_idx = _ai_own_stack_unit_list[stack_idx][itr_list_units];

                    AI_Order_SeekTransport(unit_idx, stack_idx, itr_list_units);

                }

            }

        }

    }


}


// WZD o158p10
// drake178: AI_CreateTargetList()
// ¿ MoO2  Module: COMBFIND  Collect_Targets_For_Combat_() ?
/*
creates a target list for the selected continent,
setting its type to 6 if none were found

contains several BUGs, and can only handle 25 targets
*/
/*

adds targets of fortress cities, cities, nodes, lairs, towers

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
    int16_t Next_Tile_Y = 0;
    int16_t Next_Tile_X = 0;
    int16_t Tile_Strength_Value = 0;
    int16_t Next_Tile_ChainIndex = 0;
    int16_t Asset_Owner = 0;
    int16_t fortress_wy = 0;
    int16_t fortress_wx = 0;
    int16_t itr_players = 0;  // _DI_
    int16_t itr_cities = 0;  // _DI_
    int16_t itr_nodes = 0;  // _DI_
    int16_t itr_lairs = 0;  // _DI_
    int16_t itr_towers = 0;  // _DI_


    _ai_targets_count = 0;


// ; add any player fortresses on the continent to the
// ; target list
// ; 
// ; BUG: own fortress is not excluded
// ; BUG: enemy strength calculation is botched

    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {

        if(_FORTRESSES[itr_players].wp == wp)
        {

            if(_players[player_idx].Dipl.Dipl_Status[itr_players] == DIPL_War)
            {
                
                target_value_base = 10;

            }
            else
            {

                target_value_base = 1;

            }

            if(itr_players == HUMAN_PLAYER_IDX)
            {

// ; BUG: coordinates not set up yet (will use those of
// ; the previous wizard, if any)

                target_strength = ((CONTX_NmeStrMap[wp][((fortress_wy * WORLD_WIDTH) + fortress_wx)] & AI_TARGET_FORTRESS) * 5 / 4);

            }
            else
            {

// ; BUG: coordinates not set up yet (will use those of
// ; the previous wizard, if any)

                target_strength = (CONTX_NmeStrMap[wp][((fortress_wy * WORLD_WIDTH) + fortress_wx)] & AI_TARGET_FORTRESS);

            }

            fortress_wx = _FORTRESSES[itr_players].wx;

            fortress_wy = _FORTRESSES[itr_players].wy;

            if(
                (_landmasses[((wp * WORLD_SIZE) + (fortress_wy * WORLD_WIDTH) + fortress_wx)] == landmass_idx)
                &&
                (_players[player_idx].Hostility[itr_players] >= 3)
            )
            {

                AI_Add_Target(fortress_wx, fortress_wy, target_strength, (target_value_base * 500));

            }

        }

    }


// ; add any non-own cities on the continent to the
// ; target list
// ; 
// ; BUG: enemy strength calculation is botched
// ; BUG: hostility rating 2 is botched

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {

        if(_CITIES[itr_players].wp == wp)
        {

            Asset_Owner = _CITIES[itr_players].owner_idx;

            city_wx = _CITIES[itr_players].wx;

            city_wy = _CITIES[itr_players].wy;

            if(
                (Asset_Owner != NEUTRAL_PLAYER_IDX)
                &&
                (_players[player_idx].Dipl.Dipl_Status[itr_players] == DIPL_War)
            )
            {
                
                target_value_base = 10;

            }
            else
            {

                target_value_base = 1;

            }

// ; BUG: this is not a player index here
            if(itr_cities == HUMAN_PLAYER_IDX)
            {

                target_strength = ((CONTX_NmeStrMap[wp][((fortress_wy * WORLD_WIDTH) + fortress_wx)] & AI_TARGET_FORTRESS) * 5 / 4);

            }
            else
            {

                target_strength = (CONTX_NmeStrMap[wp][((fortress_wy * WORLD_WIDTH) + fortress_wx)] & AI_TARGET_FORTRESS);

            }


            if(
                (Asset_Owner == NEUTRAL_PLAYER_IDX)
                &&
                (_landmasses[((wp * WORLD_SIZE) + (city_wy * WORLD_WIDTH) + city_wx)] == landmass_idx)
            )
            {

                AI_Add_Target(city_wx, city_wy, target_strength, ((((_CITIES[itr_cities].population + _CITIES[itr_cities].bldg_cnt) * 10) + 100) * target_value_base));

            }
            else if(
                (Asset_Owner != player_idx)
                &&
                (_players[player_idx].Hostility[itr_players] >= 3)
            )
            {

// ; already have these

                city_wx = _CITIES[itr_players].wx;

                city_wy = _CITIES[itr_players].wy;

                if(_landmasses[((wp * WORLD_SIZE) + (fortress_wy * WORLD_WIDTH) + fortress_wx)] == landmass_idx)
                {

                    if(_players[player_idx].Hostility[itr_players] >= 3)
                    {

                        AI_Add_Target(city_wx, city_wy, target_strength, ((((_CITIES[itr_cities].population + _CITIES[itr_cities].bldg_cnt) * 10) + 100) * target_value_base));

                    }
                    else
                    {

// ; this code never executes

                        if(_players[player_idx].Hostility[itr_players] >= 2)
                        {

                            if(Asset_Owner != HUMAN_PLAYER_IDX)
                            {

                                AI_Add_Target(city_wx, city_wy, target_strength, ((_CITIES[itr_cities].population + _CITIES[itr_cities].bldg_cnt) * 50));

                            }
                            else
                            {

                                AI_Add_Target(city_wx, city_wy, target_strength, ((_CITIES[itr_cities].population + _CITIES[itr_cities].bldg_cnt) * 100));

                            }

                        }

                    }
                 
                }

            }

        }

    }


// ; add any free-roaming enemy stacks on the continent to
// ; the target list

    Next_Tile_ChainIndex = CONTX_FirstTiles[wp][landmass_idx];

    // while(Next_Tile_ChainIndex = CONTX_TileChain[wp][Next_Tile_ChainIndex] != ST_UNDEFINED)
    while(Next_Tile_ChainIndex != ST_UNDEFINED)
    {

        Next_Tile_X = CONTX_TileXs[wp][Next_Tile_ChainIndex];

        Next_Tile_Y = CONTX_TileYs[wp][Next_Tile_ChainIndex];

        Tile_Strength_Value = CONTX_NmeStrMap[wp][((Next_Tile_Y * WORLD_WIDTH) + Next_Tile_X)];

        if(
            (Tile_Strength_Value > 0)
            &&
            ((Tile_Strength_Value & AI_TARGET_NONHOSTILE) == 0)
            &&
            ((Tile_Strength_Value & AI_TARGET_SITE) == 0)
            &&
            ((Tile_Strength_Value & AI_TARGET_FORTRESS) != 0)  // just getting the value
        )
        {

            if(
                (_FORTRESSES[player_idx].wp == wp)
                &&
                (_landmasses[((wp * WORLD_SIZE) + (_FORTRESSES[player_idx].wy * WORLD_WIDTH) + _FORTRESSES[player_idx].wx)] == landmass_idx)
            )
            {

                AI_Add_Target(Next_Tile_X, Next_Tile_Y, (Tile_Strength_Value & AI_TARGET_FORTRESS), (Tile_Strength_Value & AI_TARGET_FORTRESS));

            }
            else
            {

                AI_Add_Target(Next_Tile_X, Next_Tile_Y, (Tile_Strength_Value & AI_TARGET_FORTRESS), ((Tile_Strength_Value & AI_TARGET_FORTRESS) / 3));

            }

        }

        Next_Tile_ChainIndex = CONTX_TileChain[wp][Next_Tile_ChainIndex];

    }


// ; add any nodes on the continent to the target list
// ; unless they are owned by a player with less than 2
// ; hostility rating


    for(itr_nodes = 0; itr_nodes < NUM_NODES; itr_nodes++)
    {

        if(_NODES[itr_players].wp == wp)
        {

            Asset_Owner = _NODES[itr_players].owner_idx;

            if(
                (Asset_Owner != player_idx)
                &&
                (
                    (_players[player_idx].Hostility[Asset_Owner] >= 2)
                    ||
                    (Asset_Owner == ST_UNDEFINED)
                )
            )
            {

                node_wx = _NODES[itr_players].wx;

                node_wy = _NODES[itr_players].wy;

                if(_landmasses[((wp * WORLD_SIZE) + (node_wy * WORLD_WIDTH) + node_wx)] == landmass_idx)
                {
                    
                    if(Asset_Owner != ST_UNDEFINED)
                    {

                        AI_Add_Target(node_wx, node_wy, (CONTX_NmeStrMap[wp][((node_wy * WORLD_WIDTH) + node_wx)] & AI_TARGET_FORTRESS), ((_NODES[itr_nodes].power * 10) + 50));

                    }
                    else
                    {

                        // ¿ checking if there are any defenders for attacking ?
                        if((CONTX_NmeStrMap[wp][((node_wy * WORLD_WIDTH) + node_wx)] & AI_TARGET_FORTRESS) != 0)
                        {

                            AI_Add_Target(node_wx, node_wy, (CONTX_NmeStrMap[wp][((node_wy * WORLD_WIDTH) + node_wx)] & AI_TARGET_FORTRESS), ((_NODES[itr_nodes].power * 10) + 25));

                        }

                    }

                }

            }

        }

    }



// ; add any intact encounter zones on the continent to
// ; the target list

    for(itr_lairs = 0; itr_lairs < NUM_LAIRS; itr_lairs++)
    {

        if(
            (_LAIRS[itr_players].wp == wp)
            &&
            (_LAIRS[itr_players].Intact == ST_TRUE)
        )
        {

            lair_wx = _NODES[itr_players].wx;

            lair_wy = _NODES[itr_players].wy;

            if(_landmasses[((wp * WORLD_SIZE) + (lair_wy * WORLD_WIDTH) + lair_wx)] == landmass_idx)
            {

                AI_Add_Target(lair_wx, lair_wy, (CONTX_NmeStrMap[wp][((lair_wy * WORLD_WIDTH) + lair_wx)] & AI_TARGET_FORTRESS), 50);

            }
            
        }

    }


// ; add any Towers of Wizardry on the continent to the
// ; target list if they lead to an interesting
// ; destination

    if(_FORTRESSES[player_idx].wp == wp)
    {

        for(itr_towers = 0; itr_towers < NUM_TOWERS; itr_towers++)
        {

            Asset_Owner = _TOWERS[itr_towers].wx;

            tower_wx = _TOWERS[itr_towers].wx;

            tower_wy = _TOWERS[itr_towers].wy;

            if(_landmasses[((wp * WORLD_SIZE) + (tower_wy * WORLD_WIDTH) + tower_wx)] == landmass_idx)
            {

                if(AI_Tower_Target_Worthwhile(player_idx, tower_wx, tower_wy, wp) != ST_FALSE)
                {

                    if(Asset_Owner == player_idx)
                    {

                        // ; conflicting condition - will never jump

                        if(_landmasses[((wp * WORLD_SIZE) + (tower_wy * WORLD_WIDTH) + tower_wx)] == landmass_idx)
                        {

                            AI_Add_Target(tower_wx, tower_wy, 0, 1);

                        }

                    }
                    {

                        if(
                            (_players[player_idx].Hostility[Asset_Owner] >= 2)
                            ||
                            (Asset_Owner == ST_UNDEFINED)
                        )
                        {

                            if(
                                (Asset_Owner > ST_UNDEFINED)
                                &&
                                (_players[player_idx].Hostility[Asset_Owner] >= 3)
                            )
                            {

                                AI_Add_Target(tower_wx, tower_wy, (CONTX_NmeStrMap[wp][((tower_wy * WORLD_WIDTH) + tower_wx)] & AI_TARGET_FORTRESS), 10);

                            }
                            else
                            {
                                
                                if(Asset_Owner == ST_UNDEFINED)
                                {

                                    AI_Add_Target(tower_wx, tower_wy, (CONTX_NmeStrMap[wp][((tower_wy * WORLD_WIDTH) + tower_wx)] & AI_TARGET_FORTRESS), 150);

                                }

                            }

                        }

                    }

                }

            }

        }

    }


    if(_ai_targets_count == 0)
    {

        _ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] = CONT_NoTargets;

    }

}


// WZD o158p11
// drake178: AI_AddTarget()
/*
adds the specified target to the AI target list,
unless there are already 25
*/
/*

*/
void AI_Add_Target(int16_t wx, int16_t wy, int16_t strength, int16_t value)
{

    if(_ai_targets_count < 25)
    {

        _ai_targets_wx[_ai_targets_count] = wx;
        
        _ai_targets_wy[_ai_targets_count] = wy;
        
        _ai_targets_strength[_ai_targets_count] = strength;

        _ai_targets_value[_ai_targets_count] = value;

        _ai_targets_count++;

    }

}


// WZD o158p12
// drake178: AI_AssignStackTarget()
/*
; selects a target for the unassigned units in the
; specified stack, clearing its value and returning 1
; if a valid one was found, or returning 0 otherwise
*/
/*

*/
int16_t AI_AssignStackTarget__WIP(int16_t stack_wx, int16_t stack_wy, int16_t * target_wx, int16_t * target_wy, int16_t unit_list_idx, int16_t list_unit_idx)
{
    int16_t Need_Path_To_Target = 0;
    int16_t player_idx = 0;
    int16_t wp = 0;
    int16_t effective_unit_strength = 0;
    int16_t distance = 0;
    int16_t Target_Rating = 0;
    int16_t targets_idx = 0;
    int16_t Highest_Target_Rating = 0;
    int16_t effective_stack_strength = 0;
    int16_t unit_idx = 0;
    int16_t list_unit_count = 0;
    int16_t itr_list_units = 0;  // _DI_
    int16_t itr_targets = 0;  // _SI_
    int16_t return_value = 0;  // DNE in Dasm

    itr_list_units = list_unit_idx;

    list_unit_count = _ai_own_stack_unit_count[unit_list_idx];

    effective_stack_strength = 0;

// ; sum up the threat vealue of all units in the stack
// ; that have not yet been assigned an order, and store
// ; whether the last unit is marked as needing a valid
// ; path for targeting

    for(itr_list_units = 0; itr_list_units < list_unit_count; itr_list_units++)
    {

        unit_idx = _ai_own_stack_unit_list[unit_list_idx][itr_list_units];

        if(unit_idx != ST_UNDEFINED)
        {

            wp = _UNITS[unit_idx].wp;

            player_idx = _UNITS[unit_idx].owner_idx;

            Need_Path_To_Target = _UNITS[unit_idx].Move_Failed;

            effective_unit_strength = (Effective_Unit_Strength(unit_idx) / 10);

            effective_stack_strength += effective_unit_strength;

        }

    }

    Highest_Target_Rating = 0;

    targets_idx = ST_UNDEFINED;

    for(itr_targets = 0; itr_targets < _ai_targets_count; itr_targets++)
    {

        if(
            (_ai_targets_value[itr_targets] > 0)
            &&
            ((_ai_targets_strength[itr_targets] * 3 / 4) < effective_stack_strength)
        )
        {

            distance = Delta_XY_With_Wrap(_ai_targets_wx[itr_targets], _ai_targets_wy[itr_targets], stack_wx, stack_wy, WORLD_WIDTH);

            if(distance < 1)
            {
                distance = 1;
            }

            Target_Rating = (_ai_targets_value[itr_targets] / (distance * 4));

            if(Target_Rating < 1)
            {

                Target_Rating = 1;

            }

            if(Target_Rating > Highest_Target_Rating)
            {

                if(
                    (Need_Path_To_Target != ST_FALSE)
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
// UnitMove.C
// int16_t Make_Move_Path(int16_t MvMd_0, int16_t MvMd_1, int16_t MvMd_2, int16_t MvMd_3, int16_t MvMd_4, int16_t MvMd_5, 
//                        int16_t src_wx, int16_t src_wy, int16_t dst_wx, int16_t dst_wy, int16_t wp, 
//                        int8_t mvpth_x[], int8_t mvpth_y[], int8_t mvpth_c[], 
//                        int16_t UU_flag, int16_t UU_moves2, int16_t boatrider_count, int16_t troop_count, int16_t player_idx)
// MainScr.C
// _active_stack_path_length = Make_Move_Path(
//     movement_mode_flags[0],
//     movement_mode_flags[1],
//     movement_mode_flags[2],
//     movement_mode_flags[3],
//     movement_mode_flags[4],
//     movement_mode_flags[5],
//     unit_wx,
//     unit_wy,
//     unit_goto_wx,
//     unit_goto_wy,
//     _map_plane,
//     &movepath_x_array[2],
//     &movepath_y_array[2],
//     &movepath_cost_array[0],
//     1,
//     30,
//     boatrider_count,
//     troop_count,
//     player_idx
// );

                    Highest_Target_Rating = Target_Rating;

                    targets_idx = itr_targets;

                }

            }

        }

    }

    if(targets_idx != ST_UNDEFINED)
    {

        *target_wx = _ai_targets_wx[targets_idx];
        
        *target_wy = _ai_targets_wy[targets_idx];
        
        _ai_targets_value[targets_idx] = 0;

        return_value = ST_TRUE;

    }
    else
    {
        return_value = ST_FALSE;
    }

    return return_value;

}


// WZD o158p13
// drake178: AI_CONTX_Regroup()
/*
form up to 80 stacks of own units on the selected
continent, mark their types, count the units at or
en route to the rally point, and try to move roaming
stacks to attack any nearby roaming enemy stacks
*/
/*

builds _ai_own_stack_unit_list[]
finds nearest target unit
sets move|goto status

What's with AI_RallyEnRouteCount and AI_OnRallyPt_Count?

*/
void AI_Build_Stacks_Find_Targets_Order_Moves(int16_t player_idx, int16_t landmass_idx, int16_t wp)
{
    int16_t First_Unit_Index = 0;
    int16_t target_wy = 0;
    int16_t target_wx = 0;
    int16_t stack_idx = 0;
    int16_t unit_wp = 0;
    int16_t unit_wy = 0;
    int16_t unit_wx = 0;
    int16_t itr_stacks1 = 0;  // _SI_
    int16_t itr_units = 0;  // _SI_
    int16_t itr_stacks2 = 0;  // _DI_

    _ai_own_stack_count = 0;

    AI_RallyEnRouteCount = 0;

    AI_OnRallyPt_Count = 0;

    for(itr_stacks1 = 0; itr_stacks1 < 80; itr_stacks1++)
    {

        _ai_own_stack_wx[itr_stacks1] = ST_UNDEFINED;

        _ai_own_stack_wy[itr_stacks1] = ST_UNDEFINED;

        _ai_own_stack_wp[itr_stacks1] = ST_UNDEFINED;

        _ai_own_stack_unit_count[itr_stacks1] = 0;

    }


    for(itr_units = 0; itr_units < _units; itr_units++)
    {

// ; BUG: fails to sort any more units if there are
// ; already 80 stacks

        if
        (
            (_ai_own_stack_count >= 80)
            ||
            (_UNITS[itr_units].owner_idx != player_idx)
        )
        {

            continue;

        }

        unit_wp = _UNITS[itr_units].wp;

        if(unit_wp != wp)
        {

            continue;

        }

        unit_wx = _UNITS[itr_units].wx;

        unit_wy = _UNITS[itr_units].wy;


        if(
            (AI_Continent_X_Ptr[landmass_idx] == unit_wx)
            &&
            (AI_Continent_Y_Ptr[landmass_idx] == unit_wy)
        )
        {
            
            AI_OnRallyPt_Count++;
            
        }

        if(_landmasses[((unit_wp * WORLD_SIZE) + (unit_wy * WORLD_WIDTH) + unit_wx)] != landmass_idx)
        {

            continue;

        }

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

            }

        }

        if(stack_idx != ST_UNDEFINED)
        {

            if(
                (_UNITS[itr_units].Status != us_GOTO)
                &&
                (_UNITS[itr_units].Status != us_Purify)
                &&
                (_UNITS[itr_units].Status != us_BuildRoad)
            )
            {

                // *free*
                _ai_own_stack_unit_list[stack_idx][_ai_own_stack_unit_count[stack_idx]] = itr_units;

            }
            else
            {

                // *busy*
                _ai_own_stack_unit_list[stack_idx][_ai_own_stack_unit_count[stack_idx]] = ST_UNDEFINED;

                if(
                    (_UNITS[itr_units].dst_wx == AI_Continent_X_Ptr[landmass_idx])
                    &&
                    (_UNITS[itr_units].dst_wy == AI_Continent_Y_Ptr[landmass_idx])
                )
                {

                    AI_RallyEnRouteCount++;

                }

            }

        }
        else
        {

            _ai_own_stack_wx[_ai_own_stack_count] = unit_wx;

            _ai_own_stack_wy[_ai_own_stack_count] = unit_wy;

            _ai_own_stack_wp[_ai_own_stack_count] = unit_wp;

            _ai_own_stack_unit_count[_ai_own_stack_count] = 1;

            if(
                (_UNITS[itr_units].Status != us_GOTO)
                &&
                (_UNITS[itr_units].Status != us_Purify)
                &&
                (_UNITS[itr_units].Status != us_BuildRoad)
            )
            {

                // *free*
                _ai_own_stack_unit_list[_ai_own_stack_count][0] = itr_units;

            }
            else
            {

                // *busy*
                _ai_own_stack_unit_list[_ai_own_stack_count][0] = ST_UNDEFINED;


                if(
                    (_UNITS[itr_units].dst_wx == AI_Continent_X_Ptr[landmass_idx])
                    &&
                    (_UNITS[itr_units].dst_wy == AI_Continent_Y_Ptr[landmass_idx])
                )
                {

                    AI_RallyEnRouteCount++;

                }

            }

            // DEDU  what is it actually check here?  What's in CONTX_NmeStrMap[] for the AI's own units?
            if(CONTX_NmeStrMap[unit_wp][((unit_wy * WORLD_WIDTH) + unit_wx)] != 0)
            {

                if(
                    (_FORTRESSES[player_idx].wx == unit_wx)
                    &&
                    (_FORTRESSES[player_idx].wy == unit_wy)
                    &&
                    (_FORTRESSES[player_idx].wp == unit_wp)
                )
                {

                    _ai_own_stack_type[_ai_own_stack_count] = 4;

                }
                else
                {

                    _ai_own_stack_type[_ai_own_stack_count] = 3;

                }

            }
            else
            {

                _ai_own_stack_type[_ai_own_stack_count] = 1;

            }

            _ai_own_stack_count++;

        }

    }

    for(itr_stacks1 = 0; itr_stacks1 < _ai_own_stack_count; itr_stacks1++)
    {

        if(_ai_own_stack_type[_ai_own_stack_count] == 1)
        {

            First_Unit_Index = _ai_own_stack_unit_list[itr_stacks1][0];

            if(
                (First_Unit_Index == ST_UNDEFINED)
                ||
                (_UNITS[First_Unit_Index].Status != us_Move)
            )
            {

                if((_ai_own_stack_unit_count[itr_stacks1] + 1) < 3)
                {

                    _ai_own_stack_type[itr_stacks1] = 0;

                }

            }
            else
            {

                if(AI_Find_Nearest_Target_Unit(itr_stacks1, landmass_idx, &target_wx, &target_wy, wp) == ST_TRUE)
                {

                    for(itr_stacks2 = 0; _ai_own_stack_unit_count[itr_stacks1] > itr_stacks2; itr_stacks2++)
                    {

                        AI_Set_Move_Or_Goto_Target(_ai_own_stack_unit_list[itr_stacks1][itr_stacks2], target_wx, target_wy, itr_stacks1, itr_stacks2);

                    }

                }
                else
                {

                    for(itr_stacks2 = 0; _ai_own_stack_unit_count[itr_stacks1] > itr_stacks2; itr_stacks2++)
                    {

                        _UNITS[_ai_own_stack_unit_list[itr_stacks1][itr_stacks2]].Status = us_Ready;

                    }

                    _ai_own_stack_type[itr_stacks1] = 0;

                }

            }

        }

    }

}


// WZD o158p14
// drake178: AI_FindClosestEnemy()
/*
; searches for the closest tile on the continent that
; has enemy units on it but no structure
; returns 1 with the coordinates filled if an enemy was
; found; or 0 if there aren't any
*/
/*

returns {F,T} 'found target'
in-outs target_wx,wy

*/
int16_t AI_Find_Nearest_Target_Unit(int16_t stack_idx, int16_t landmass_idx, int16_t * target_wx, int16_t * target_wy, int16_t wp)
{
    int16_t Next_Tile_Y = 0;
    int16_t Next_Tile_X = 0;
    int16_t stack_wy = 0;
    int16_t stack_wx = 0;
    int16_t l_target_wy = 0;
    int16_t l_target_wx = 0;
    int16_t closest_distance = 0;
    int16_t distance = 0;
    int16_t Tile_Strength_Value = 0;
    int16_t Next_Tile_ChainIndex;  // _DI_

    closest_distance = 1000;

    stack_wx = _ai_own_stack_wx[stack_idx];

    stack_wy = _ai_own_stack_wy[stack_idx];

    Next_Tile_ChainIndex = CONTX_FirstTiles[wp][landmass_idx];

    // while(Next_Tile_ChainIndex = CONTX_TileChain[wp][Next_Tile_ChainIndex] != ST_UNDEFINED)
    while(Next_Tile_ChainIndex != ST_UNDEFINED)
    {

        Next_Tile_X = CONTX_TileXs[wp][Next_Tile_ChainIndex];

        Next_Tile_Y = CONTX_TileYs[wp][Next_Tile_ChainIndex];

        Tile_Strength_Value = CONTX_NmeStrMap[wp][((Next_Tile_Y * WORLD_WIDTH) + Next_Tile_X)];

        if(
            ((Tile_Strength_Value & AI_TARGET_FORTRESS) != 0)
            &&
            ((Tile_Strength_Value & AI_TARGET_NONHOSTILE) != 0)
            &&
            ((Tile_Strength_Value & AI_TARGET_SITE) != 0)
        )
        {

            distance = Delta_XY_With_Wrap(stack_wx, stack_wy, Next_Tile_X, Next_Tile_Y, WORLD_WIDTH);

            if(distance < closest_distance)
            {

                closest_distance = distance;

                l_target_wx = Next_Tile_X;

                l_target_wy = Next_Tile_Y;

            }

        }

        Next_Tile_ChainIndex = CONTX_TileChain[wp][Next_Tile_ChainIndex];

    }

    if(closest_distance < 1000)
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
// drake178: AI_OppMoveOverride()
/*
cancels the orders of any stack currently set to move
(status 3) if they are adjacent to a non-own city
with less than 4 defending units

could do with optimizing, RE-EXPLORE
*/
/*

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
    int16_t itr_stacks = 0;  // _DI_
    int16_t itr_cities = 0;  // _SI_

    for(itr_stacks = 0; itr_stacks < AI_Own_Stack_Count; itr_stacks++)
    {

        if(AI_Own_Stacks[itr_stacks].unit_status != us_GOTO)
        {

            continue;

        }

        stack_wx = AI_Own_Stacks[itr_stacks].wx;

        stack_wy = AI_Own_Stacks[itr_stacks].wy;

        stack_wp = AI_Own_Stacks[itr_stacks].wp;

        min_wx = (stack_wx - 1);
        min_wy = (stack_wy - 1);
        max_wx = (stack_wx + 1);
        max_wy = (stack_wy + 1);

        found_target = ST_FALSE;


// ; check if there's a lightly defended enemy city on the
// ; tiles adjacent to the stack

        for(itr_cities = 0; ((itr_cities < _cities) && (found_target == ST_FALSE)); itr_cities++)
        {

            if(
                (_CITIES[itr_cities].wx < min_wx)
                ||
                (_CITIES[itr_cities].wx > max_wx)
                ||
                (_CITIES[itr_cities].wy < min_wy)
                ||
                (_CITIES[itr_cities].wy > min_wy)
                ||
                (_CITIES[itr_cities].wp != wp)
                ||
                (_CITIES[itr_cities].owner_idx != player_idx)
            )
            {

                continue;

            }

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

            if(defender_count < 4)
            {

                found_target = ST_TRUE;

            }

        }

// ; cancel the orders of both the stack and the
// ; individual units in it

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

        }

    }

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

uses AI_ContBalances[][]

AI_SetUnitOrders__WIP(itr_players)
    |-> AI_Balance_Upkeep(itr_players)

*/
void AI_Disband_To_Balance_Budget(int16_t player_idx)
{
    int16_t UU_Local_0 = 0;
    int16_t Upkeep = 0;
    int16_t Unit_Landmass = 0;
    int16_t Unit_Plane = 0;
    int16_t Mana_Needed = 0;
    int16_t Gold_Needed = 0;
    int16_t Food_Needed = 0;
    int16_t Lowest_Value_Index = 0;
    int16_t Unit_Value = 0;
    int16_t Lowest_Value = 0;
    int16_t Tries = 0;
    int16_t Mana_Income = 0;
    int16_t Food_Income = 0;
    int16_t Gold_Income = 0;
    int16_t itr_units = 0;  // _DI_


    Player_Resource_Income_Total(player_idx, &Gold_Income, &Food_Income, &Mana_Income);


    Mana_Needed = 0;
    Food_Needed = 0;
    Gold_Needed = 0;


    UU_Local_0 = 0;


    if(Food_Income < 0)
    {

        Food_Needed += -(Food_Income);

    }

    if(Gold_Income < 0)
    {

        Gold_Needed += -(Gold_Income);

    }

    if(Mana_Income < 0)
    {

        Mana_Needed += -(Mana_Income);

    }


    for(Tries = 0; ((Tries < 200) && ((Food_Needed > 0) || (Gold_Needed > 0))); Tries++)
    {

        // ; find and remove the lowest value non-fantastic unit
        // ; from play
        // ; 
        // ; BUG? resets the unit's plane to 0 afterward

        Lowest_Value = 10000;

        Lowest_Value_Index = ST_UNDEFINED;

        for(itr_units = 0; itr_units < _units; itr_units++)
        {

            /* HACK */  if(
            /* HACK */      (_UNITS[itr_units].owner_idx == ST_UNDEFINED)
            /* HACK */      ||
            /* HACK */      (_UNITS[itr_units].wp == ST_UNDEFINED)
            /* HACK */  )
            /* HACK */  {
            /* HACK */      continue;
            /* HACK */  }

            if(
                (_UNITS[itr_units].type < ut_Magic_Spirit)
                ||
                (_UNITS[itr_units].owner_idx == player_idx)
            )
            {

                Unit_Plane = _UNITS[itr_units].wp;

                Unit_Landmass = _landmasses[((Unit_Plane * WORLD_SIZE) + (_UNITS[itr_units].wy * WORLD_WIDTH) + _UNITS[itr_units].wx)];

                if(Unit_Landmass > 0)
                {

                    Unit_Value = (Effective_Unit_Strength(itr_units) / 10);

                    if(AI_ContBalances[Unit_Plane][Unit_Landmass] == 0)
                    {

                        Unit_Value *= 2;

                    }

                    if(Unit_Value < Lowest_Value)
                    {

                        Lowest_Value = Unit_Value;

                        Lowest_Value_Index = itr_units;

                    }

                }

            }

        }

        if(Lowest_Value_Index != ST_UNDEFINED)
        {

            Food_Needed--;

            Upkeep = ((_unit_type_table[_UNITS[Lowest_Value_Index].type].Upkeep * difficulty_modifiers_table[_difficulty].maintenance) / 100);

            if(Upkeep == 0)
            {

                Upkeep = 1;

            }

            Gold_Needed -= Upkeep;

            Kill_Unit(Lowest_Value_Index, 0);

            _UNITS[Lowest_Value_Index].wp = 0;

        }

    }


    for(Tries = 0; ((Tries < 200) && (Mana_Needed > 0)); Tries++)
    {

        // ; find and remove the lowest value fantastic unit
        // ; from play
        // ; 
        // ; BUG: uses unit type instead of unit index when trying
        // ;  to determine threat value
        // ; BUG? resets the unit's plane to 0 afterward

        Lowest_Value = 10000;

        Lowest_Value_Index = ST_UNDEFINED;

        for(itr_units = 0; itr_units < _units; itr_units++)
        {
            
            /* HACK */  if(
            /* HACK */      (_UNITS[itr_units].owner_idx == ST_UNDEFINED)
            /* HACK */      ||
            /* HACK */      (_UNITS[itr_units].wp == ST_UNDEFINED)
            /* HACK */  )
            /* HACK */  {
            /* HACK */      continue;
            /* HACK */  }

            if(
                (_UNITS[itr_units].type >= ut_Magic_Spirit)
                ||
                (_UNITS[itr_units].owner_idx == player_idx)
            )
            {

                Unit_Plane = _UNITS[itr_units].wp;

                Unit_Landmass = _landmasses[((Unit_Plane * WORLD_SIZE) + (_UNITS[itr_units].wy * WORLD_WIDTH) + _UNITS[itr_units].wx)];

                if(Unit_Landmass > 0)
                {

                    // ; BUG: parameter mismatch
                    // BUGBUG ¿ should use Effective_Unit_Type_Strength() or pass itr_units ?
                    Unit_Value = (Effective_Unit_Strength(_UNITS[itr_units].type) / 10);

                    if(AI_ContBalances[Unit_Plane][Unit_Landmass] == 0)
                    {

                        Unit_Value *= 2;

                    }

                    if(Unit_Value < Lowest_Value)
                    {

                        Lowest_Value = Unit_Value;

                        Lowest_Value_Index = itr_units;

                    }

                }

            }

        }

        if(Lowest_Value_Index != ST_UNDEFINED)
        {

            Upkeep = ((_unit_type_table[_UNITS[Lowest_Value_Index].type].Upkeep * difficulty_modifiers_table[_difficulty].maintenance) / 100);

            if(Upkeep == 0)
            {

                Upkeep = 1;

            }

            Mana_Needed -= Upkeep;

            Kill_Unit(Lowest_Value_Index, 0);

            _UNITS[Lowest_Value_Index].wp = 0;

        }

    }

}


// WZD o158p17
// drake178: AI_ShiftOffHomePlane()
/*
shifts all stacks that are on a Tower of Wizardry or
Astral Gate tile off the fortress plane if
possible

contains multiple BUGs in its subfunctions,
RE-EXPLORE!
*/
/*

shift all units on a tower or astral gate to the plane opposite the players fortress

*/
void AI_Shift_Off_Home_Plane(int16_t player_idx)
{
    int16_t itr_cities = 0;
    int16_t Stack_Plane = 0;
    int16_t Stack_Y = 0;
    int16_t Stack_X = 0;
    int16_t Opposite_Plane = 0;
    int16_t itr_towers = 0;
    int16_t itr = 0;  // _SI_
    int16_t itr2 = 0;  // _DI_


    for(itr = 0; itr < _num_players; itr++)
    {

        if(_players[itr].Globals[PLANAR_SEAL] != 0)
        {

            return;

        }

    }

    Opposite_Plane = (1 - _FORTRESSES[player_idx].wp);

    for(itr2 = 0; itr2 < AI_Own_Stack_Count; itr2++)
    {

        Stack_Plane = AI_Own_Stacks[itr2].wp;

        if(Stack_Plane != Opposite_Plane)
        {

            Stack_X = AI_Own_Stacks[itr2].wx;

            Stack_Y = AI_Own_Stacks[itr2].wy;


            if(Map_Square_Troops_Can_Plane_Shift(Stack_X, Stack_Y, Stack_Plane) == ST_TRUE)
            {


// ; if there is a Tower of Wizardry on the tile, shift
// ; all of the units to the other plane

                for(itr_towers = 0; itr_towers < NUM_TOWERS; itr_towers++)
                {

                    if(
                        (_TOWERS[itr_towers].wx == Stack_X)
                        &&
                        (_TOWERS[itr_towers].wy == Stack_Y)
                    )
                    {

                        for(itr = 0; itr < _units; itr++)
                        {

                            AI_Own_Stacks[itr2].wp = Opposite_Plane;

                        }

                    }

                }

// ; if there's an own city with Astral Gate on the tile,
// ; shift all of the units to the other plane

                for(itr_cities = 0; itr_cities < _cities; itr_cities++)
                {

                    if(
                        (_CITIES[itr_cities].owner_idx == player_idx)
                        &&
                        (_CITIES[itr_cities].enchantments[ASTRAL_GATE] != 0)
                    )
                    {

                        if(
                            (_CITIES[itr_cities].wx == Stack_X)
                            &&
                            (_CITIES[itr_cities].wy == Stack_Y)
                        )
                        {

                            for(itr = 0; itr < _units; itr++)
                            {
                                if(
                                    (_UNITS[itr].wx = Stack_X)
                                    &&
                                    (_UNITS[itr].wy = Stack_Y)
                                )
                                {

                                    _UNITS[itr].wp = Opposite_Plane;

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
// drake178: AI_Transport_Sailoff()
/*
; sets all transports that are not on a sailable tile
; to move to one if there's one adjacent, reducing
; their stack's value to -1
;
; BUG: flying or non-corporeal transports should
; definitely no do this
*/
/*

being in AI_Own_Stacks[] and being next to Square_Is_OceanLike()
means the boats is on land/in a city/dry dock?

*/
void AI_Move_Out_Boats(void)
{
    int16_t Can_Sail_Off = 0;
    int16_t Sailable_Tile_Y = 0;
    int16_t Sailable_Tile_X = 0;
    int16_t itr_wy = 0;
    int16_t landmass_idx = 0;
    int16_t wp = 0;
    int16_t wy = 0;
    int16_t wx = 0;
    int16_t itr_stack = 0;  // _SI_
    int16_t itr_wx = 0;  // _DI_

    for(itr_stack = 0; itr_stack < AI_Own_Stack_Count; itr_stack++)
    {

        if(
            (AI_Own_Stacks[itr_stack].value != ST_UNDEFINED)
            &&
            ((AI_Own_Stacks[itr_stack].abilities & AICAP_Transport) != 0)
        )
        {

            wx = AI_Own_Stacks[itr_stack].wx;

            wy = AI_Own_Stacks[itr_stack].wy;

            wp = AI_Own_Stacks[itr_stack].wp;

            landmass_idx = _landmasses[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)];

            Sailable_Tile_X = 0;

            Sailable_Tile_Y = 0;

            Can_Sail_Off = ST_FALSE;


            if(landmass_idx > 0)
            {

                // ; check if there's an adjacent sailable tile

                for(itr_wy = -1; itr_wy < 2; itr_wy++)
                {

                    for(itr_wx = -1; itr_wx < 2; itr_wx++)
                    {

                        if(
                            (itr_wy != 0)
                            &&
                            (itr_wx != 0)
                            &&
                            ((itr_wy + wy) < WORLD_HEIGHT)
                            &&
                            ((itr_wx + wx) < WORLD_WIDTH)
                        )
                        {

                            if(Square_Is_OceanLike((itr_wx + wx), (itr_wy + wy), wp) == ST_TRUE)
                            {

                                Sailable_Tile_X = (wx + itr_wx);

                                Sailable_Tile_Y = (wy + itr_wy);

                                Can_Sail_Off = ST_TRUE;

                            }

                        }

                    }


                }

                if(Can_Sail_Off == ST_TRUE)
                {

                    AI_Stack_Set_Boats_Goto(itr_stack, Sailable_Tile_X, Sailable_Tile_Y);

                }

            }

        }




    }

}


// WZD o158p19
// drake178: AI_GarrBuilderPush()
/*
; pushes settlers, engineers and spirits out of all
; garrison stacks to an adjacent free tile if possible
; by giving them a move order
*/
/*

*/
void AI_GarrBuilderPush__WIP(int16_t wp)
{
    int16_t Node_On_Tile = 0;
    int16_t itr_nodes = 0;
    int16_t Spirit_Y = 0;
    int16_t Spirit_X = 0;
    int16_t unit_idx = 0;
    int16_t unit_type = 0;
    int16_t Garrison_Unit_Count = 0;
    int16_t RetY = 0;
    int16_t RetX = 0;
    int16_t itr_stacks = 0;  // _SI_
    int16_t itr_garrison = 0;  // _DI_

    for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
    {

        if(_ai_own_stack_type[itr_stacks] < 3)
        {

            continue;

        }

        Garrison_Unit_Count = _ai_own_stack_unit_count[itr_stacks];

        for(itr_garrison = 0; itr_garrison < Garrison_Unit_Count; itr_garrison++)
        {

            unit_idx = _ai_own_stack_unit_list[itr_stacks][itr_garrison];

            if(unit_idx == ST_UNDEFINED)
            {
                continue;
            }

            unit_type = _UNITS[unit_idx].type;

            if(
                ((_unit_type_table[unit_type].Abilities & UA_CREATEOUTPOST) != 0)
                ||
                (_unit_type_table[unit_type].Construction > 0)
            )
            {

                if(TILE_NextFreeLand__WIP(_ai_own_stack_wx[itr_stacks], _ai_own_stack_wy[itr_stacks], _ai_own_stack_wp[itr_stacks], &RetX, &RetY) == ST_TRUE)
                {

                    AI_Set_Move_Or_Goto_Target(unit_idx, RetX, RetY, itr_stacks, itr_garrison);

                }

            }
            else if((_unit_type_table[unit_type].Abilities & UA_MELD) != 0)
            {

                Spirit_X = _UNITS[unit_idx].wx;
                
                Spirit_Y = _UNITS[unit_idx].wy;

                Node_On_Tile = ST_FALSE;

                for(itr_nodes = 0; ((itr_nodes < NUM_NODES) && (Node_On_Tile == ST_FALSE)); itr_nodes++)
                {

                    if(
                        (_NODES[itr_nodes].wx == Spirit_X)
                        &&
                        (_NODES[itr_nodes].wy == Spirit_Y)
                        &&
                        (_NODES[itr_nodes].wp == wp)
                    )
                    {

                        Node_On_Tile = ST_TRUE;

                    }

                }

                if(Node_On_Tile == ST_FALSE)
                {

                    if(TILE_NextFreeLand__WIP(_ai_own_stack_wx[itr_stacks], _ai_own_stack_wy[itr_stacks], _ai_own_stack_wp[itr_stacks], &RetX, &RetY) == ST_TRUE)
                    {

                        AI_Set_Move_Or_Goto_Target(unit_idx, RetX, RetY, itr_stacks, itr_garrison);

                    }

                }

            }

        }

    }

}


// WZD o158p20
// drake178: sub_EFBD6()
/*
evaluates excess units for inclusion into the two
global offensive? stacks instead if they are strong
enough
*/
/*

*/
void AI_sEFBD6__WIP(void)
{
    int16_t itr_stacks = 0;  // _SI_
    int16_t stack_unit_count = 0;  // _DI_

    G_Pushout_Units_Count = 0;

    G_Seafaring_Count = 0;

    G_Pushout_Lowest_Value = 10000;

    G_Seafaring_Lowest_Value = 10000;

    for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
    {

        stack_unit_count = _ai_own_stack_unit_count[itr_stacks];

        if(_ai_own_stack_type[itr_stacks] == 4)
        {

            if(
                (_turn < 100)
                &&
                (stack_unit_count > 5)
            )
            {

                AI_sEFC92__WIP(itr_stacks, stack_unit_count, (stack_unit_count - 5));

            }

        }
        else if (_ai_own_stack_type[itr_stacks] == 3)
        {

            if(stack_unit_count > 5)
            {

                AI_sEFC92__WIP(itr_stacks, stack_unit_count, (stack_unit_count - 5));

            }

        }
        else
        {

            if(
                (_ai_own_stack_type[itr_stacks] == 0)
                ||
                (
                    (_ai_own_stack_type[itr_stacks] == 1)
                    &&
                    (_ai_own_stack_unit_list[itr_stacks][0] != ST_UNDEFINED)
                )
            )
            {

                AI_sEFC92__WIP(itr_stacks, stack_unit_count, stack_unit_count);

            }

        }

    }

}


// WZD o158p21
// drake178: sub_EFC92()
/*
evaluates the excess units in the stack, and adds
them to the two global offensive? stacks if they are
stronger than the current selections
*/
/*

*/
void AI_sEFC92__WIP(int16_t CX_ID, int16_t Count, int16_t Excess)
{
    int16_t Combat_Unit_Indices[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Combat_Unit_Values[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Combat_Unit_UL_Indices[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Combat_Unit_Count = 0;
    int16_t Unit_Value = 0;
    int16_t Local_Array_Index = 0;
    int16_t Lowest_Value_Index = 0;
    int16_t Seafaring_Unit = 0;
    int16_t unit_type = 0;
    int16_t CX_UL_Index = 0;
    int16_t unit_idx = 0;
    int16_t itr2 = 0;  // _SI_
    int16_t itr = 0;  // _DI_

    Combat_Unit_Count = 0;

// ; create a list of the combat units (not builders or
// ; transports) in the stack, complete with indices and
// ; threat values
// ; 
// ; BUG: parameter mismatch in the threat function call
// ; BUG? transports could fight over water

    for(CX_UL_Index = 0; CX_UL_Index < Count; CX_UL_Index++)
    {

        unit_idx = _ai_own_stack_unit_list[CX_ID][CX_UL_Index];

        if(unit_idx != ST_UNDEFINED)
        {

            unit_type = _UNITS[unit_idx].type;

            if(
                (_unit_type_table[unit_type].Construction == 0)
                &&
                ((_unit_type_table[unit_type].Abilities & UA_CREATEOUTPOST) == 0)
                &&
                ((_unit_type_table[unit_type].Abilities & UA_MELD) == 0)
                &&
                (_unit_type_table[unit_type].Transport == 0)
            )
            {

                Combat_Unit_UL_Indices[Combat_Unit_Count] = CX_UL_Index;

                Combat_Unit_Indices[Combat_Unit_Count] = unit_idx;

                // ; BUG: parameter mismatch
                // BUGBUG  should use unit_idx or Effective_Unit_Type_Strength()
                Combat_Unit_Values[Combat_Unit_Count] = (Effective_Unit_Strength(unit_type) / 10);

                Combat_Unit_Count++;

            }

        }

    }


// ; sort the arrays into descending value order

    if(Excess < Combat_Unit_Count)
    {

        Combat_Unit_Count = Excess;

    }

    for(Local_Array_Index = 0; Local_Array_Index < Combat_Unit_Count; Local_Array_Index++)
    {

        CX_UL_Index = Combat_Unit_UL_Indices[Local_Array_Index];

        unit_idx = Combat_Unit_Indices[Local_Array_Index];

        Unit_Value = Combat_Unit_Values[Local_Array_Index];

        if(
            (Unit_Has_AirTravel(unit_idx) == ST_TRUE)
            ||
            (Unit_Has_WaterTravel(unit_idx) == ST_TRUE)
        )
        {

            Seafaring_Unit = ST_TRUE;

        }
        else
        {

            Seafaring_Unit = ST_FALSE;

        }

        if(G_Pushout_Units_Count < 9)
        {

            G_Pushout_UL_Indices[G_Pushout_Units_Count] = CX_UL_Index;

            G_Pushout_CX_IDs[G_Pushout_Units_Count] = CX_ID;

            G_Pushout_Values[G_Pushout_Units_Count] = Unit_Value;

            G_Pushout_Unit_Indices[G_Pushout_Units_Count] = unit_idx;

            G_Pushout_Units_Count++;

            if(Unit_Value < G_Pushout_Lowest_Value)
            {

                G_Pushout_Lowest_Value = Unit_Value;

            }

        }
        else
        {

            if(Unit_Value > G_Pushout_Lowest_Value)
            {

// ; find the local array index that corresponds to the
// ; lowest value

                Lowest_Value_Index = 0;

                for(itr = 0; itr < 9; itr++)
                {

                    if(G_Pushout_Values[itr] == G_Pushout_Lowest_Value)
                    {

                        Lowest_Value_Index = itr;

                    }

                }

// ; replace and recalculate the lowest value

                G_Pushout_UL_Indices[Lowest_Value_Index] = CX_UL_Index;

                G_Pushout_CX_IDs[Lowest_Value_Index] = CX_ID;

                G_Pushout_Values[Lowest_Value_Index] = Unit_Value;

                G_Pushout_Unit_Indices[Lowest_Value_Index] = unit_idx;

                G_Pushout_Lowest_Value = 10000;

                for(itr = 0; itr < 9; itr++)
                {

                    if(G_Pushout_Values[itr] < G_Pushout_Lowest_Value)
                    {

                        G_Pushout_Lowest_Value = G_Pushout_Values[itr];

                    }

                }

            }

        }

        if(Seafaring_Unit == ST_TRUE)
        {

            if(G_Seafaring_Count < 9)
            {

                G_Seafaring_UL_Indices[G_Seafaring_Count] = CX_UL_Index;

                G_Seafaring_CX_IDs[G_Seafaring_Count] = CX_ID;

                G_Seafaring_Values[G_Seafaring_Count] = Unit_Value;

                G_Seafaring_Unit_Indices[G_Seafaring_Count] = unit_idx;

                G_Seafaring_Count++;

                if(Unit_Value < G_Seafaring_Lowest_Value)
                {

                    G_Seafaring_Lowest_Value = (Effective_Unit_Strength(unit_idx) / 10);

                }

            }
            else
            {

// ; replace the current lowest value unit with the new
// ; one

                if(Unit_Value > G_Seafaring_Lowest_Value)
                {

                    Lowest_Value_Index = 0;

                    for(itr = 0; itr < 9; itr++)
                    {

                        if(G_Seafaring_Values[itr] == G_Seafaring_Lowest_Value)
                        {

                            Lowest_Value_Index = itr;

                        }

                    }

                    G_Seafaring_UL_Indices[Lowest_Value_Index] = CX_UL_Index;

                    G_Seafaring_CX_IDs[Lowest_Value_Index] = CX_ID;

                    G_Seafaring_Values[Lowest_Value_Index] = Unit_Value;

                    G_Seafaring_Unit_Indices[Lowest_Value_Index] = unit_idx;

                    G_Seafaring_Lowest_Value = 10000;

                    for(itr = 0; itr < 9; itr++)
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



// ; rearrange the regular table into descending order

    for(itr = 0; (G_Pushout_Units_Count - 1) > itr; itr++)
    {

        for(itr = 0; (G_Pushout_Units_Count - 1) > itr; itr++)
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


// ; rearrange the seafaring table into descending order

    for(itr = 0; (G_Seafaring_Count - 1) > itr; itr++)
    {

        for(itr = 0; (G_Seafaring_Count - 1) > itr; itr++)
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
// drake178: AI_ProcessMelders()
/*
processes all units with the Meld ability in the
current stacks - if there is a meldable node they
will start going there, or if already there, they
will meld into it
*/
/*

*/
void AI_Do_Meld(int16_t player_idx)
{
    int16_t node_landmass_idx = 0;
    int16_t unit_wp = 0;
    int16_t node_is_garrisoned = 0;
    int16_t distance = 0;
    int16_t node_wy = 0;
    int16_t node_wx = 0;
    int16_t unit_wy = 0;
    int16_t unit_wx = 0;
    int16_t itr_units = 0;
    int16_t unit_idx = 0;
    int16_t list_unit_count = 0;
    int16_t list_unit_idx = 0;
    int16_t closest_distance = 0;
    int16_t target_node_idx = 0;
    int16_t itr = 0;  // _DI_
    int16_t itr_nodes = 0;  // _SI_

    for(itr = 0; itr < _ai_own_stack_count; itr++)
    {

        list_unit_count = _ai_own_stack_unit_count[itr];

        for(list_unit_idx = 0; list_unit_idx < list_unit_count; list_unit_idx++)
        {

            unit_idx = _ai_own_stack_unit_list[itr][list_unit_idx];

// ; BUG: the check for -1 needs to come first

            if(
                ((_unit_type_table[_UNITS[unit_idx].type].Abilities & UA_MELD) != 0)
                &&
                (unit_idx != ST_UNDEFINED)
            )
            {

// ; find the closest non-owned node that either has own
// ; units on it, or is on an own, abandon, or no targets
// ; continent

                unit_wx = _UNITS[unit_idx].wx;

                unit_wy = _UNITS[unit_idx].wy;

                unit_wp = _UNITS[unit_idx].wp;

                target_node_idx = ST_UNDEFINED;

                closest_distance = 1000;

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

                        if(CONTX_NmeStrMap[unit_wp][((node_wy * WORLD_WIDTH) + node_wx)] == AI_TARGET_SITE)
                        {

                            node_landmass_idx = _landmasses[((unit_wp * WORLD_SIZE) + (node_wy * WORLD_WIDTH) + node_wx)];

                            node_is_garrisoned = ST_FALSE;

// ; check if the node has any own units on it

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
                                (_ai_continents.plane[unit_wp].player[player_idx].Cont_Types[node_landmass_idx] == CONT_Own)
                                ||
                                (_ai_continents.plane[unit_wp].player[player_idx].Cont_Types[node_landmass_idx] >= CONT_Abandon)  // {..., 5: CONT_Abandon, 6: CONT_NoTargets}
                                ||
                                (node_is_garrisoned == ST_TRUE)
                            )
                            {

                                distance = Delta_XY_With_Wrap(node_wx, node_wy, unit_wx, unit_wy, WORLD_WIDTH);

                                if(distance < closest_distance)
                                {
                                    
                                    closest_distance = distance;

                                    target_node_idx = itr_nodes;

                                }

                            }

                        }

                    }

                }

                if(target_node_idx != ST_UNDEFINED)
                {

                    if(closest_distance == 0)
                    {

                        AI_Order_Meld(unit_idx, itr, list_unit_idx);

                    }
                    else
                    {

                        node_wx = _NODES[target_node_idx].wx;

                        node_wy = _NODES[target_node_idx].wy;

                        AI_Set_Move_Or_Goto_Target(unit_idx, node_wx, node_wy, itr, list_unit_idx);

                    }

                }

            }

        }

    }

}


// WZD o158p23
// drake178: AI_ProcessSettlers()
/*
processes any unassigned settlers, giving them either
move or settle orders based on the circumstances

has multiple BUGs, including one that allows settling
while the AI has an empty garrison somewhere
*/
/*

*/
void AI_Do_Settle(int16_t player_idx, int16_t landmass_idx)
{
    int16_t Next_Tile_ChainIndex = 0;
    int16_t unit_wp = 0;
    int16_t Best_Tile_Y = 0;
    int16_t Best_Tile_X = 0;
    int16_t have_adamantium = 0;
    int16_t have_mithril = 0;
    int16_t Tile_Settling_Value = 0;
    int16_t have_nightshade = 0;
    int16_t magic_units = 0;
    int16_t gold_units = 0;
    int16_t unit_cost_reduction = 0;
    int16_t gold_bonus = 0;
    int16_t production_bonus = 0;
    int16_t maximum_population = 0;
    int16_t wx = 0;
    int16_t wy = 0;
    int16_t Best_Tile_Value = 0;
    int16_t Empty_Garrison = 0;
    int16_t Fast_Expand_Chance = 0;
    int16_t Unexplored = 0;
    int16_t have_shore = 0;
    int16_t Tile_Distance = 0;
    int16_t unit_wy = 0;
    int16_t unit_wx = 0;
    int16_t list_unit_count = 0;
    int16_t itr_list_units = 0;
    int16_t Tower_Y = 0;
    int16_t Tower_X = 0;
    int16_t wp = 0;
    int16_t itr_stacks = 0;  // _SI_
    int16_t unit_idx = 0;  // _DI_

    Fast_Expand_Chance = 1;

    // DEDU  proabably switch{}?
    if(_players[player_idx].Objective == OBJ_Expansionist)
    {

        Fast_Expand_Chance = 4;

    }
    else if(_players[player_idx].Objective == OBJ_Militarist)
    {

        Fast_Expand_Chance = 2;

    }
    else if(_players[player_idx].Objective == OBJ_Pragmatist)
    {

        Fast_Expand_Chance = 2;

    }
    else if(_players[player_idx].Objective == OBJ_Perfectionist)
    {

        Fast_Expand_Chance = 1;

    }
    else if(_players[player_idx].Objective == OBJ_Theurgist)
    {

        Fast_Expand_Chance = 3;

    }

    Empty_Garrison = ST_FALSE;

    for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
    {

        if(
            (_ai_own_stack_type[itr_stacks] == 3)  // DEDU  What is stack type 3 getting us here?
            &&
            (_ai_own_stack_unit_count[itr_stacks] < 1)  // ¿ ; conflicting condition - will always jump ?
        )
        {

            Empty_Garrison = ST_TRUE;

        }

    }

    if(Empty_Garrison != ST_TRUE)
    {

        for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
        {

            list_unit_count = _ai_own_stack_unit_count[itr_stacks];

            for(itr_list_units = 0; itr_list_units < list_unit_count; itr_list_units++)
            {

                unit_idx = _ai_own_stack_unit_list[itr_stacks][itr_list_units];


                if(
                    (unit_idx != ST_UNDEFINED)
                    &&
                    ((_unit_type_table[_UNITS[unit_idx].type].Abilities & UA_CREATEOUTPOST) != 0)
                )
                {

                    unit_wx = _UNITS[unit_idx].wx;

                    unit_wy = _UNITS[unit_idx].wy;

                    unit_wp = _UNITS[unit_idx].wp;

                    if(
                        (Map_Square_Area_Has_Opponent(unit_wx, unit_wy, unit_wp, 3, player_idx) != ST_FALSE)
                        &&
                        (_turn >= 100)
                        &&
                        (Random(5) != 1)
                        &&
                        (
                            (Map_Square_Survey(unit_wx, unit_wy, unit_wp) != ST_FALSE)
                            ||
                            (Random(5) > Fast_Expand_Chance)
                        )
                    )
                    {

                        if(AI_CanSettleOffPlane__WIP(player_idx, unit_idx, &Tower_X, &Tower_Y, unit_wp) == ST_TRUE)
                        {

                            AI_Set_Move_Or_Goto_Target(unit_idx, Tower_X, Tower_Y, itr_stacks, itr_list_units);

                        }
                        else
                        {

                            Best_Tile_Value = ST_UNDEFINED;

                            Next_Tile_ChainIndex = CONTX_FirstTiles[unit_wp][landmass_idx];

                            // while(Next_Tile_ChainIndex = CONTX_TileChain[wp][Next_Tile_ChainIndex] != ST_UNDEFINED)
                            while(Next_Tile_ChainIndex != ST_UNDEFINED)
                            {

                                wx = CONTX_TileXs[unit_wp][Next_Tile_ChainIndex];
                                
                                wy = CONTX_TileYs[unit_wp][Next_Tile_ChainIndex];

                                if(Map_Square_Survey(wx, wy, unit_wp) == 0)
                                {

                                    if((CONTX_NmeStrMap[unit_wp][((wy * WORLD_WIDTH) + wx)] & AI_TARGET_FORTRESS) == 0)
                                    {
                                        
                                        Tile_Distance = Range(unit_wx, unit_wy, wx, wy);

                                        if(Tile_Distance == 0)
                                        {

                                            Compute_Base_Values_For_Map_Square(wx, wy, wp, &maximum_population, &production_bonus, &gold_bonus, &unit_cost_reduction, &gold_units, &magic_units, &have_nightshade, &have_mithril, &have_adamantium, &have_shore, &Unexplored);

                                            Tile_Settling_Value = 
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
                                                    Tile_Distance
                                                );

                                            EMM_Map_CONTXXX__WIP();


                                            if(Tile_Settling_Value > Best_Tile_Value)
                                            {

                                                Best_Tile_Value = Tile_Settling_Value;
                                                
                                                Best_Tile_X = wx;

                                                Best_Tile_Y = wy;

                                            }

                                        }

                                    }
                                    
                                }

                                Next_Tile_ChainIndex = CONTX_TileChain[wp][Next_Tile_ChainIndex];

                            }

                            if(Best_Tile_Value == ST_UNDEFINED)
                            {

                                AI_SendToColonize__WIP(unit_idx, unit_wx, unit_wy, unit_wp, player_idx, itr_stacks, itr_list_units);

                            }
                            else
                            {

                                AI_Set_Move_Or_Goto_Target(unit_idx, Best_Tile_X, Best_Tile_Y, itr_stacks, itr_list_units);

                            }

                        }

                    }
                    else
                    {
                        
                        AI_Order_Settle(unit_idx, itr_stacks, itr_list_units);

                    }

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
    int16_t Tile_Y = 0;
    int16_t Tile_X = 0;
    int16_t Tile_Y_Offset = 0;
    int16_t Tile_X_Offset = 0;
    int16_t target_wy = 0;
    int16_t target_wx = 0;
    int16_t Next_Tile_ChainIndex = 0;
    int16_t InRange_Corruption = 0;
    int16_t Unit_Y = 0;
    int16_t Unit_X = 0;

    int16_t itr_stacks;  // _DI_

// ; check if there are any corrupted tiles on the
// ; continent near own assets and not under enemy control
// ; 
// ; BUG: should really only check city catchments

    InRange_Corruption = ST_FALSE;

    Next_Tile_ChainIndex = CONTX_FirstTiles[wp][landmass_idx];

    // while(Next_Tile_ChainIndex = CONTX_TileChain[wp][Next_Tile_ChainIndex] != ST_UNDEFINED)
    while(Next_Tile_ChainIndex != ST_UNDEFINED)
    {

        if(InRange_Corruption != ST_FALSE)
        {

            break;

        }

        Tile_X = CONTX_TileXs[wp][Next_Tile_ChainIndex];
        
        Tile_Y = CONTX_TileYs[wp][Next_Tile_ChainIndex];

        if(
            ((_map_square_flags[((wp * WORLD_SIZE) + (Tile_Y * WORLD_WIDTH) + Tile_X)] & MSF_CORRUPTION) != 0)
            &&
            ((CONTX_NmeStrMap[wp][((Tile_Y * WORLD_WIDTH) + Tile_X)] & AI_TARGET_FORTRESS) == 0)
        )
        {

// ; check the 5 by 5 area centered around the tile to see
// ; if there are any cities or nodes, and set
// ; InRange_Corruption along with the first found
// ; coordinates if there are
// ; 
// ; BUG: should really only go for city catchments

            for(Tile_Y_Offset = -2; Tile_Y_Offset < 3; Tile_Y_Offset++)
            {

                for(Tile_X_Offset = -2; Tile_X_Offset < 3; Tile_X_Offset++)
                {

                    if(CONTX_NmeStrMap[wp][(((Tile_Y_Offset + Tile_Y) * WORLD_WIDTH) + (Tile_X_Offset + Tile_X))] == AI_TARGET_SITE)
                    {

                        InRange_Corruption = ST_TRUE;

                        target_wx = Tile_X;

                        target_wy = Tile_Y;

                    }


                }

            }

        }

        Next_Tile_ChainIndex = CONTX_TileChain[wp][Next_Tile_ChainIndex];

    }

    if(InRange_Corruption != ST_FALSE)
    {

        for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
        {

            if(_ai_own_stack_type[itr_stacks] == 0)
            {

                list_unit_count = _ai_own_stack_unit_count[itr_stacks];

                for(list_unit_idx = 0; list_unit_idx < list_unit_count; list_unit_idx++)
                {

                    unit_idx = _ai_own_stack_unit_list[itr_stacks][list_unit_idx];

                    if(unit_idx == ST_UNDEFINED)
                    {

                        if(_unit_type_table[_UNITS[unit_idx].type].Abilities & UA_PURIFY)
                        {

                            Unit_X = _UNITS[unit_idx].wx;
                            
                            Unit_Y = _UNITS[unit_idx].wy;

                            if((_map_square_flags[((wp * WORLD_SIZE) + (Unit_Y * WORLD_WIDTH) + Unit_X)] & MSF_CORRUPTION) != 0)
                            {

                                AI_Order_Purify(unit_idx, itr_stacks, list_unit_idx);

                            }
                            else
                            {

                                AI_Set_Move_Or_Goto_Target(unit_idx, target_wx, target_wy, itr_stacks, list_unit_idx);

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
    int16_t distance = 0;
    int16_t city_landmass_idx = 0;
    int16_t closest_distance = 0;
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

                closest_distance = 1000;

                for(city_idx = 0; city_idx < _cities; city_idx++)
                {

// ; BUG: parameter mismatch (and redundant branching, as
// ; the next comparison would be enough by itself)
                    if(
                        (_CITIES[city_idx].wp == unit_wp)
                        &&
                        (
                            ((AI_Own_Stacks[itr_stacks].abilities & AICAP_LandOnly) != 0)
                            ||
                            ((unit_wp * WORLD_SIZE) + (_CITIES[city_idx].wy * WORLD_WIDTH) + _CITIES[city_idx].wx)  == Landmass_Dup
                        )
                    )
                    {

                        distance = Delta_XY_With_Wrap(unit_wx, unit_wy, _CITIES[city_idx].wx, _CITIES[city_idx].wy, WORLD_WIDTH);

                        if(distance < closest_distance)
                        {
                            
                            closest_distance = distance;

                            nearest_city_idx = city_idx;

                        }

                    }

                }

                if(nearest_city_idx != ST_UNDEFINED)
                {

                    if(closest_distance > 1)
                    {

                        AI_Set_Move_Or_Goto_Target(unit_idx, _CITIES[nearest_city_idx].wx, _CITIES[nearest_city_idx].wy, itr_stacks, itr_list_units);

                    }
                    else
                    {

                        RoadTo_City = ST_UNDEFINED;

                        closest_distance = 1000;

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

                                    distance = Delta_XY_With_Wrap(_CITIES[nearest_city_idx].wx, _CITIES[nearest_city_idx].wy, _CITIES[city_idx].wx, _CITIES[city_idx].wy, WORLD_WIDTH);

                                    if(distance < closest_distance)
                                    {
                                        
                                        closest_distance = distance;

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
// drake178: AI_UNIT_SetMoveOrder()
/*
; sets the move order for the unit, either US_Move if
; the target tile contains enemy units and nothing
; else, or US_GoingTo otherwise; and replaces the
; unit's index in the stack with -1, marking it
; processed
*/
/*

*/
void AI_Set_Move_Or_Goto_Target(int16_t unit_idx, int16_t target_wx, int16_t target_wy, int16_t CX_ID, int16_t list_unit_idx)
{
    int16_t wp = 0;
    int16_t target_value = 0;  // _DI_

    if(
        (unit_idx < 0)
        ||
        (unit_idx > MAX_UNIT_COUNT)
    )
    {

        return;

    }

    wp = _UNITS[unit_idx].wp;

    target_value = CONTX_NmeStrMap[wp][((target_wy * WORLD_WIDTH) + target_wx)];

    if(
        ((target_value & AI_TARGET_SITE) != 0)
        ||
        ((target_value & AI_TARGET_FORTRESS) != 0)
    )
    {

        _UNITS[unit_idx].Status = us_Move;  // DEDU  What does this status mean?

    }
    else
    {

        _UNITS[unit_idx].Status = us_GOTO;

    }

        _UNITS[unit_idx].dst_wx = target_wx;

        _UNITS[unit_idx].dst_wy = target_wy;

        _ai_own_stack_unit_list[CX_ID][list_unit_idx] = ST_UNDEFINED;

}


// WZD o158p27
// drake178: AI_UNIT_SettleOrder()
/*
; sets the unit to US_Settle status, and replaces its
; index in the stack with -1, marking it processed
*/
/*

*/
void AI_Order_Settle(int16_t unit_idx, int16_t unit_list_idx, int16_t list_unit_idx)
{


    if(
        (unit_idx < 0)
        ||
        (unit_idx >= MAX_UNIT_COUNT)
    )
    {

        return;

    }

    _UNITS[unit_idx].Status = us_Settle;

    _ai_own_stack_unit_list[unit_list_idx][list_unit_idx] = ST_UNDEFINED;


}


// WZD o158p28
// drake178: AI_UNIT_RoadBldOrder()
/*
sets the unit to US_BuildRoad status, filling out its
from and to road coordinates, and replacing its index
in the stack with -1, marking it processed
*/
/*

*/
void AI_Order_RoadBuild(int16_t unit_idx, int16_t wx, int16_t wy, int16_t unit_list_idx, int16_t list_unit_idx)
{

    if(
        (unit_idx < 0)
        ||
        (unit_idx >= MAX_UNIT_COUNT)
    )
    {

        return;

    }

    _UNITS[unit_idx].Rd_From_X = _UNITS[unit_idx].wx;

    _UNITS[unit_idx].Rd_From_Y = _UNITS[unit_idx].wy;

    _UNITS[unit_idx].Status = us_BuildRoad;

    _UNITS[unit_idx].dst_wx = wx;

    _UNITS[unit_idx].dst_wy = wy;

    _ai_own_stack_unit_list[unit_list_idx][list_unit_idx] = ST_UNDEFINED;

}


// WZD o158p29
// drake178: AI_UNIT_SeekTransport()
/*
sets the unit to US_SeekTransport status with a timer
of 10 (stored in the Target_X field), and replaces
its index in the stack with -1, marking it processed
*/
/*

SUL *aware*

*/
void AI_Order_SeekTransport(int16_t unit_idx, int16_t unit_list_idx, int16_t list_unit_idx)
{

    if(
        (unit_idx < 0)
        ||
        (unit_idx >= UNIT_COUNT_MAX)
    )
    {

        return;

    }

    _UNITS[unit_idx].Status = us_SeekTransport;

    _UNITS[unit_idx].dst_wx = 10;

    _ai_own_stack_unit_list[unit_list_idx][list_unit_idx] = ST_UNDEFINED;

}


// WZD o158p30
// drake178: AI_UNIT_SetMeldOrder()
/*
; sets the unit to US_Meld status, and replaces its
; index in the stack with -1, marking it processed
*/
/*

*/
void AI_Order_Meld(int16_t unit_idx, int16_t unit_list_idx, int16_t list_unit_idx)
{

    if(
        (unit_idx < 0)
        ||
        (unit_idx > MAX_UNIT_COUNT)
    )
    {

        return;

    }

    _UNITS[unit_idx].Status = us_Meld;

    _ai_own_stack_unit_list[unit_list_idx][list_unit_idx] = ST_UNDEFINED;

}


// WZD o158p31
// drake178: AI_UNIT_PurifyOrder()
/*
sets the unit to US_Purify status, and replaces its
index in the stack with -1, marking it processed
*/
/*

*/
void AI_Order_Purify(int16_t unit_idx, int16_t unit_list_idx, int16_t list_unit_idx)
{

    if(
        (unit_idx < 0)
        &&
        (unit_idx >= UNIT_COUNT_MAX)
    )
    {
        return;
    }

        _UNITS[unit_idx].Status = us_Purify;

        _UNITS[unit_idx].dst_wx = 20;

        _ai_own_stack_unit_list[unit_list_idx][list_unit_idx] = ST_UNDEFINED;

}


// WZD o158p32
// drake178: TILE_CheckEnemyUnits()
/*
; checks if there are any non-own units in the radius
; around the specified tile
; returns 1 if there are, 0 if there aren't
*/
/*

~== AI_Enemy_Unit_In_Range()

*/
int16_t Map_Square_Area_Has_Opponent(int16_t wx, int16_t wy, int16_t wp, int16_t range, int16_t player_idx)
{
    int16_t max_wy = 0;
    int16_t min_wy = 0;
    int16_t max_wx = 0;
    int16_t min_wx = 0;
    int16_t itr_units = 0;  // _DX_

    min_wx = (wx - range);
    
    min_wy = (wy - range);

    max_wx = (wx + range);
    
    max_wy = (wy + range);

    for(itr_units = 0; itr_units < _units; itr_units++)
    {

        if(
            (_UNITS[itr_units].wx >= min_wx)
            &&
            (_UNITS[itr_units].wx <= max_wx)
            &&
            (_UNITS[itr_units].wy >= min_wy)
            &&
            (_UNITS[itr_units].wy <= max_wy)
            &&
            (_UNITS[itr_units].wp == wp)
            &&
            (_UNITS[itr_units].owner_idx != player_idx)
        )
        {

            return ST_TRUE;

        }

    }

    return ST_FALSE;

}


// WZD o158p33
// drake178: AI_TILE_CheckEnemies()
/*
returns 1 if there is at least one non-own unit
within the specified range of the selected tile; or
0 otherwise
*/
/*

~== Map_Square_Area_Has_Opponent()

*/
int16_t AI_Enemy_Unit_In_Range(int16_t wx, int16_t wy, int16_t wp, int16_t range, int16_t player_idx, int16_t landmass_idx)
{
    int16_t Max_Y = 0;
    int16_t Min_Y = 0;
    int16_t Max_X = 0;
    int16_t Min_X = 0;
    int16_t itr_units = 0;  // _DI_

    Min_X = (wx - range);

    Min_Y = (wy - range);

    Max_X = (wx + range);

    Max_Y = (wy + range);

    for(itr_units = 0; itr_units < _units; itr_units++)
    {

        if(
            (_UNITS[itr_units].wx >= Min_X)
            &&
            (_UNITS[itr_units].wy >= Min_Y)
            &&
            (_UNITS[itr_units].wx <= Max_X)
            &&
            (_UNITS[itr_units].wy <= Max_Y)
            &&
            (_UNITS[itr_units].wp == wp)
            &&
            (_UNITS[itr_units].owner_idx == player_idx)
            &&
            (_landmasses[((wp * WORLD_SIZE) + (_UNITS[itr_units].wy * WORLD_WIDTH) + _UNITS[itr_units].wx)] == landmass_idx)
        )
        {

            return ST_TRUE;

        }

    }

    return ST_FALSE;
    
}


// WZD o158p34
// drake178: AI_CanSettleOffPlane()
/*
; finds the nearest controlled Tower of Wizardry that
; is on the same continent as the unit, and its non-
; home plane side leads to a continent that is landable
; and not contested (not type 2 or 4)
; returns 1 along with the coordinates if successful
; returns 0 otherwise, if Planar Seal is in effect, if
; the selected Plane is not the home Plane, or if the
; unit is already on a tower's tile
*/
/*

*/
int16_t AI_CanSettleOffPlane__WIP(int16_t player_idx, int16_t unit_idx, int16_t * Tower_X, int16_t * Tower_Y, int16_t unit_wp)
{

    return ST_FALSE;

}


// WZD o158p35
// drake178: AI_SendToColonize()
/*
if there is a new colony continent set for the plane,
set the unit to go there - on its own if it is
seafaring, on land if it can get there, or by
requesting transport otherwise
*/
/*

*/
void AI_SendToColonize__WIP(int16_t unit_idx, int16_t wx, int16_t wy, int16_t wp, int16_t player_idx, int16_t unit_list_idx, int16_t list_unit_idx)
{
    int16_t target_wy = 0;
    int16_t target_wx = 0;
    int16_t Seafaring_Unit = 0;
    int16_t Transport_StackSize = 0;
    int16_t transport_capacity = 0;
    int16_t Adjacent_Ocean_Y = 0;
    int16_t Adjacent_Ocean_X = 0;
    int16_t uu_landmass_idx = 0;
    int16_t transport_wy = 0;
    int16_t transport_wx = 0;
    int16_t Unwrapped_X = 0;
    int16_t Own_Stack_Index = 0;
    int16_t found_transport = 0;
    int16_t Embarkable_Tile = 0;
    int16_t unit_wy = 0;  // _DI_
    int16_t unit_wx = 0;  // _SI_

    uu_landmass_idx = _landmasses[((WORLD_SIZE * wp) + (wy * WORLD_WIDTH) + wx)];

    unit_wx = _UNITS[unit_idx].wx;

    unit_wy = _UNITS[unit_idx].wx;  // BUGBUG  should be _UNITS[].wy, not _UNITS[].wx

    wp = _UNITS[unit_idx].wp;

    Seafaring_Unit = ST_FALSE;

    if(
        (Unit_Has_AirTravel(unit_idx) != ST_FALSE)
        ||
        (Unit_Has_WaterTravel(unit_idx) != ST_FALSE)
    )
    {

        Seafaring_Unit = ST_TRUE;

    }


    if(AI_NewColConts[wp][player_idx] == 0)
    {

        return;

    }

    transport_wx = AI_NewColTgtXs[wp][player_idx];

    transport_wy = AI_NewColTgtYs[wp][player_idx];

    if(Seafaring_Unit == ST_TRUE)
    {

        AI_Set_Move_Or_Goto_Target(unit_idx, transport_wx, transport_wy, unit_list_idx, list_unit_idx);

    }
    else
    {

// ; check if any of the adjacent tiles can be embarked
// ; onto
// ; 
// ; BUG: not all valid tiles are ambarkable
// ; BUG: there's already a CONTX allocation for listing
// ;  tiles that can be embarked from

        Embarkable_Tile = ST_FALSE;

        for(unit_wy = (wy - 1); (wy + 2) > unit_wy; unit_wy++)
        {

            if(
                (unit_wy > 0)
                &&
                (unit_wy < WORLD_HEIGHT)
            )
            {

                for(Unwrapped_X = (wx - 1); (wx + 2) > Unwrapped_X; Unwrapped_X++)
                {

                    unit_wx = Unwrapped_X;

                    if(unit_wx > 0)
                    {

                        unit_wx += WORLD_WIDTH;

                    }

                    if(unit_wx >= WORLD_WIDTH)
                    {

                        unit_wx -= WORLD_WIDTH;
                    }

                    if(Map_Square_Is_Embarkable(unit_wx, unit_wy, wp) != ST_FALSE)
                    {

                        Embarkable_Tile = ST_TRUE;

                        Adjacent_Ocean_X = Unwrapped_X;  // BUGBUG  ; BUG: this should be si  [NOTE: "si" as in unit_wx]

                        Adjacent_Ocean_Y = unit_wy;
                        
                    }

                }

            }

        }


        if(Embarkable_Tile == ST_FALSE)
        {

            TILE_AI_FindLoadTile__WIP(wx, wy, wp, &Adjacent_Ocean_X, &Adjacent_Ocean_Y);

            if(TILE_AI_FindEmptyLnd__WIP(Adjacent_Ocean_X, Adjacent_Ocean_Y, wp, &Adjacent_Ocean_X, &Adjacent_Ocean_Y) == ST_TRUE)
            {

                AI_Set_Move_Or_Goto_Target(unit_idx, Adjacent_Ocean_X, Adjacent_Ocean_Y, unit_list_idx, list_unit_idx);

            }

        }
        else
        {

// ; check if there's an adjacent transport stack

            found_transport = ST_FALSE;

            for(Own_Stack_Index = 0; ((Own_Stack_Index < AI_Own_Stack_Count) && (found_transport == ST_FALSE)); Own_Stack_Index++)
            {

                if(
                    (AI_Own_Stacks[Own_Stack_Index].wp == wp)
                    &&
                    ((AI_Own_Stacks[Own_Stack_Index].abilities & AICAP_Transport) != 0)
                )
                {

                    unit_wx = AI_Own_Stacks[Own_Stack_Index].wx;

                    unit_wy = AI_Own_Stacks[Own_Stack_Index].wy;

                    if(
                        (abs(wx - unit_wx) < 2)
                        &&
                        (abs(wy - unit_wy) < 2)
                    )
                    {

                        found_transport = ST_TRUE;

                        transport_wx = unit_wx;

                        transport_wy = unit_wy;

                        transport_capacity = AI_Own_Stacks[Own_Stack_Index].transport_capacity;

                        Transport_StackSize = (MAX_STACK - AI_Own_Stacks[Own_Stack_Index].unit_count);

                    }

                }

            }

            if(found_transport == ST_TRUE)
            {

                AI_Set_Move_Or_Goto_Target(unit_idx, transport_wx, transport_wy, unit_list_idx, list_unit_idx);

            }
            else
            {

                if(ai_seektransport_cnt < 15)
                {

                    AI_Order_SeekTransport(unit_idx, unit_list_idx, list_unit_idx);

                    AI_SeekTransport_Xs[ai_seektransport_cnt] = Adjacent_Ocean_X;

                    AI_SeekTransport_Ys[ai_seektransport_cnt] = Adjacent_Ocean_Y;

                    AI_SeekTransport_Ps[ai_seektransport_cnt] = wp;

                    ai_seektransport_cnt++;

                }

            }

        }

    }




}


// WZD o158p36
// drake178: AI_SeekTransportFrom()
/*
; adds the tile to the transport request array, unless
; it is already on it, or the array is full
*/
/*

*/
void AI_SeekTransportFrom__WIP(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t itr_seektransports = 0;  // _DX_

    for(itr_seektransports = 0; itr_seektransports < ai_seektransport_cnt; itr_seektransports++)
    {
        
        if(
            (AI_SeekTransport_Xs[itr_seektransports] == wx)
            &&
            (AI_SeekTransport_Ys[itr_seektransports] == wy)
            &&
            (AI_SeekTransport_Ps[itr_seektransports] == wp)
        )
        {
            return;
        }

    }

    AI_SeekTransport_Xs[ai_seektransport_cnt] = wx;

    AI_SeekTransport_Ys[ai_seektransport_cnt] = wy;

    AI_SeekTransport_Ps[ai_seektransport_cnt] = wp;

    ai_seektransport_cnt++;

}


// WZD o158p37
// drake178: AI_CheckOtherPlane()
/*
; returns 1 if the continent on the other plane is at
; least 11 tiles large if it is owned or not landable;
; if it has a city that can be attacked if it is a no
; presence one; or if either of these two criteria are
; true if the continent is any other type
; returns 0 if none of the requirements are met
*/
/*



*/
int16_t AI_Tower_Target_Worthwhile(int16_t player_idx, int16_t wx, int16_t wy, int16_t wp)
{
    int16_t Next_Tile_ChainIndex = 0;
    int16_t city_owner_idx = 0;
    int16_t Opposite_Plane = 0;
    int16_t Opposite_Landmass = 0;
    int16_t Have_City_Target = 0;
    int16_t itr_cities = 0;  // _DI_

    Have_City_Target = ST_FALSE;

    Opposite_Plane = (1 - wp);

    Opposite_Landmass = _landmasses[((Opposite_Plane * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)];

    if(
        (_ai_continents.plane[Opposite_Plane].player[player_idx].Cont_Types[Opposite_Landmass] != CONT_Own)
        &&
        (_ai_continents.plane[Opposite_Plane].player[player_idx].Cont_Types[Opposite_Landmass] != CONT_NoLanding)
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

    if(_ai_continents.plane[Opposite_Plane].player[player_idx].Cont_Types[Opposite_Landmass] != CONT_NoPresence)
    {

        itr_cities = 0;

        Next_Tile_ChainIndex = CONTX_FirstTiles[Opposite_Plane][Opposite_Landmass];

        // while(Next_Tile_ChainIndex = CONTX_TileChain[Opposite_Plane][Next_Tile_ChainIndex] != ST_UNDEFINED)
        while(Next_Tile_ChainIndex != ST_UNDEFINED)
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

            Next_Tile_ChainIndex = CONTX_TileChain[Opposite_Plane][Next_Tile_ChainIndex];

        }

    }

    return Have_City_Target;

}


// WZD o158p38
// drake178: AI_SingleCont_Reeval()
/*
; cancels the orders of all non-engineer units on the
; selected continent, and reevaluates it much the same
; way as AI_Continent_Reeval, recalculating its type
; and rally point accordingly
;
; contains multiple BUGs, including a possible division
; by zero crash
*/
/*

*/
void AI_SingleCont_Reeval__WIP(int16_t player_idx, int16_t landmass_idx, int16_t wp)
{
    int16_t Tile_Weight = 0;
    int16_t unit_type = 0;
    int16_t Sum_City_Y_NonOwn_UnitCost = 0;
    int16_t Sum_City_X_Own_UnitCost = 0;
    int16_t Rally_Y = 0;
    int16_t Rally_X = 0;
    int16_t Tile_Y = 0;
    int16_t Tile_X = 0;
    int16_t Opposition = 0;
    int16_t LoadTile_Y = 0;
    int16_t LoadTile_X = 0;
    int16_t Best_Tile_Weight = 0;
    int16_t Next_Tile_ChainIndex = 0;
    int16_t Center_Y = 0;
    int16_t Center_X = 0;
    int16_t City_Count = 0;
    int16_t itr = 0;


    EMM_Map_CONTXXX__WIP();


// ; cancel the orders of every unit on the continent
// ; except for engineers

    for(itr = 0; itr < _units; itr++)
    {

        if(
            (_UNITS[itr].owner_idx == player_idx)
            &&
            (_unit_type_table[_UNITS[itr].type].Construction == 0)
            &&
            (_landmasses[((_UNITS[itr].wp * WORLD_SIZE) + (_UNITS[itr].wp * WORLD_WIDTH) + _UNITS[itr].wp)] == landmass_idx)
        )
        {

            _UNITS[itr].Status = us_Ready;

        }

    }


// ; count the amount of own cities on the continent into
// ; the type global, and sum their coordinates into the
// ; local variables

    Sum_City_X_Own_UnitCost = 0;

    Sum_City_Y_NonOwn_UnitCost = 0;

    for(itr = 0; itr < _cities; itr++)
    {

        if(
            (_CITIES[itr].owner_idx == player_idx)
            &&
            (_landmasses[((_CITIES[itr].wp * WORLD_SIZE) + (_CITIES[itr].wp * WORLD_WIDTH) + _CITIES[itr].wp)] == landmass_idx)
        )
        {

            Sum_City_X_Own_UnitCost += _CITIES[itr].wx;

            Sum_City_Y_NonOwn_UnitCost += _CITIES[itr].wy;

            _ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] += 1;

        }

    }

    City_Count = _ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx];

    if(City_Count > 0)
    {

        _ai_continents.plane[wp].player[player_idx].X_Coords[landmass_idx] = (Sum_City_X_Own_UnitCost / City_Count);

        _ai_continents.plane[wp].player[player_idx].Y_Coords[landmass_idx] = (Sum_City_Y_NonOwn_UnitCost / City_Count);

    }


// ; sum the costs of own and not own units on the
// ; continent with the exception of settlers
// ; 
// ; BUG: does not clear the variables beforehand, so
// ; they still contain the coordinate sums too

    if(_ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] > 0)
    {

        if((Sum_City_Y_NonOwn_UnitCost * 10) < Sum_City_X_Own_UnitCost)
        {

            _ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] = CONT_Own;

        }
        else
        {
            
            _ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] = CONT_Contested;

        }

    }
    else
    {

        _ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] = CONT_NoPresence;

    }


// ; if the continent has no own city, but has an allied
// ; one, mark it for no landing instead

    for(itr = 0; itr < _cities; itr++)
    {

        if(
            (_CITIES[itr].owner_idx != player_idx)
            &&
            (_CITIES[itr].owner_idx != NEUTRAL_PLAYER_IDX)
            &&
            (_landmasses[((_CITIES[itr].wp * WORLD_SIZE) + (_CITIES[itr].wp * WORLD_WIDTH) + _CITIES[itr].wp)] == landmass_idx)
        )
        {

            if(
                (_players[player_idx].Dipl.Dipl_Status[_CITIES[itr].owner_idx] == DIPL_Alliance)
                &&
                (_ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] == CONT_NoPresence)
            )
            {

                _ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] = CONT_NoLanding;

            }

        }

    }

    _ai_continents.plane[0].player[player_idx].Cont_Types[0] = CONT_NoLanding;

    _ai_continents.plane[1].player[player_idx].Cont_Types[0] = CONT_NoLanding;

    if(
        (_ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] == CONT_Contested)
        ||
        (_ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] == CONT_Own)
        ||
        (_ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] == CONT_NoLanding)
    )
    {

        Center_X = _ai_continents.plane[wp].player[player_idx].X_Coords[landmass_idx];

        Center_Y = _ai_continents.plane[wp].player[player_idx].Y_Coords[landmass_idx];

        Best_Tile_Weight = 1000;

// ; find a semi-random empty tile on the continent, if
// ; any (weighted by distance + a random value)

        Next_Tile_ChainIndex = CONTX_1stLoadTs[wp][landmass_idx];

        // while(Next_Tile_ChainIndex = CONTX_LoadTChain[wp][Next_Tile_ChainIndex] != ST_UNDEFINED)
        while(Next_Tile_ChainIndex != ST_UNDEFINED)
        {

            Tile_Weight = Delta_XY_With_Wrap(CONTX_TileYs[wp][Next_Tile_ChainIndex], CONTX_TileYs[wp][Next_Tile_ChainIndex], Center_X, Center_Y, WORLD_WIDTH) + Random(5);

            if(Tile_Weight < Best_Tile_Weight)
            {

                Tile_X = CONTX_TileXs[wp][Next_Tile_ChainIndex];

                Tile_Y = CONTX_TileYs[wp][Next_Tile_ChainIndex];

                Opposition = CONTX_NmeStrMap[wp][((Tile_Y * WORLD_WIDTH) + Tile_X)];
                
                if(Opposition == 0)
                {

                    Best_Tile_Weight = Tile_Weight;
                    
                    // ¿ ; already in local vars ?

                    Rally_X = CONTX_TileXs[wp][Next_Tile_ChainIndex];
                    
                    Rally_Y = CONTX_TileYs[wp][Next_Tile_ChainIndex];

                }

            }

            Next_Tile_ChainIndex = CONTX_LoadTChain[wp][Next_Tile_ChainIndex];

        }

        if(Best_Tile_Weight < 1000)
        {

            _ai_continents.plane[wp].player[player_idx].X_Coords[landmass_idx] = Rally_X;

            _ai_continents.plane[wp].player[player_idx].Y_Coords[landmass_idx] = Rally_Y;

        }
        else
        {

            _ai_continents.plane[wp].player[player_idx].X_Coords[landmass_idx] = 0;

            _ai_continents.plane[wp].player[player_idx].Y_Coords[landmass_idx] = 0;

            _ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] = CONT_NoLanding;

        }

    }


// ; if the continent has no presence, ensure that the
// ; rally point is a valid disembarking tile, or if that
// ; is not possible, re-mark the continent as not
// ; landable with a rally point of [0,0]

    if(_ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] == CONT_Own)
    {

        LoadTile_X = 0;

        LoadTile_Y = 0;

        Tile_X = 0;

// ; ensure that the rally point of the continent is a
// ; valid embarking tile
// ; 
// ; BUG: coming from above, if there is no main war
// ; continent, this code will crash with a division by 0

        Next_Tile_ChainIndex = CONTX_1stLoadTs[wp][AI_MainWarConts[wp][player_idx]];

        // while(Next_Tile_ChainIndex = CONTX_LoadTChain[wp][Next_Tile_ChainIndex] != ST_UNDEFINED)
        while(Next_Tile_ChainIndex != ST_UNDEFINED)
        {

            LoadTile_X += CONTX_LoadTileXs[wp][Next_Tile_ChainIndex];

            LoadTile_Y += CONTX_LoadTileYs[wp][Next_Tile_ChainIndex];

            Tile_X++;

            Next_Tile_ChainIndex = CONTX_LoadTChain[wp][Next_Tile_ChainIndex];

        }

        // ; BUG: possible division by 0
        LoadTile_X /= Tile_X;

        // ; BUG: possible division by 0
        LoadTile_Y /= Tile_X;

        Best_Tile_Weight = 1000;

        Next_Tile_ChainIndex = CONTX_1stLoadTs[wp][landmass_idx];

        // while(Next_Tile_ChainIndex = CONTX_LoadTChain[wp][Next_Tile_ChainIndex] != ST_UNDEFINED)
        while(Next_Tile_ChainIndex != ST_UNDEFINED)
        {

            Tile_Weight = (Delta_XY_With_Wrap(CONTX_LoadTileXs[wp][Next_Tile_ChainIndex], CONTX_LoadTileYs[wp][Next_Tile_ChainIndex], LoadTile_X, LoadTile_Y, WORLD_WIDTH) + Random(5));

            if(Tile_Weight < Best_Tile_Weight)
            {

                Tile_X = CONTX_LoadTileXs[wp][Next_Tile_ChainIndex];

                Tile_Y = CONTX_LoadTileYs[wp][Next_Tile_ChainIndex];

                if(CONTX_NmeStrMap[wp][((Tile_Y * WORLD_WIDTH) + Tile_X)] == 0)
                {

                    Best_Tile_Weight = Tile_Weight;

                    Rally_X = Tile_X;

                    Rally_Y = Tile_Y;

                }

            }

            Next_Tile_ChainIndex = CONTX_LoadTChain[wp][Next_Tile_ChainIndex];

        }

    }

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
// drake178: AI_STK_SetTarget()

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
// drake178: AI_STK_MoveTransports()
/*
; give all transport units in the selected stack a move
; order to the specified destination, changing the
; stack's value to -1 in the process
*/
/*

sets every boat in the stack to GOTO with wx,wy as their dst

*/
void AI_Stack_Set_Boats_Goto(int16_t ai_stack_idx, int16_t wx, int16_t wy)
{
    int16_t stack_wp = 0;
    int16_t stack_wy = 0;
    int16_t stack_wx = 0;
    int16_t itr_units = 0;  // _SI_

    stack_wx = AI_Own_Stacks[ai_stack_idx].wx;

    stack_wy = AI_Own_Stacks[ai_stack_idx].wy;

    stack_wp = AI_Own_Stacks[ai_stack_idx].wp;

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

            _UNITS[itr_units].dst_wx = wx;

            _UNITS[itr_units].dst_wy = wy;

        }

    }

    AI_Own_Stacks[ai_stack_idx].value = ST_UNDEFINED;

}


// WZD o162p20
// drake178: sub_F60F4()

// WZD o162p21
// drake178: TILE_AI_FindLoadTile()
/*
finds the closest land tile from which units can be
loaded onto a transport - or passes back the original
coordinates if none are found

WARNING: requires the CONTXXX EMS mapping
*/
/*

*/
void TILE_AI_FindLoadTile__WIP(int16_t wx, int16_t wy, int16_t wp, int16_t * RetX, int16_t * RetY)
{
    int16_t Adj_Empty_Land = 0;
    int16_t landmass_idx = 0;
    int16_t Next_Tile_ChainIndex = 0;
    int16_t closest_distance = 0;
    int16_t distance = 0;
    int16_t LoadTile_Y = 0;
    int16_t LoadTile_X = 0;
    int16_t Return_Y = 0;
    int16_t Return_X = 0;
    int16_t some_wy = 0;
    int16_t some_wx = 0;  // _DI_

    landmass_idx = _landmasses[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)];

    closest_distance = 2000;

    Next_Tile_ChainIndex = CONTX_1stLoadTs[wp][landmass_idx];

    // while(Next_Tile_ChainIndex = CONTX_LoadTChain[wp][Next_Tile_ChainIndex] != ST_UNDEFINED)
    while(Next_Tile_ChainIndex != ST_UNDEFINED)
    {

        LoadTile_Y = CONTX_LoadTileYs[wp][Next_Tile_ChainIndex];

        Adj_Empty_Land = ST_FALSE;

        for(some_wy = -1; ((some_wy < 2) && (Adj_Empty_Land == ST_FALSE)); some_wy++)
        {

            for(some_wx = -1; ((some_wx < 2) && (Adj_Empty_Land == ST_FALSE)); some_wx++)
            {

                if(
                    (_landmasses[((wp * WORLD_SIZE) + ((some_wy + LoadTile_Y) * WORLD_WIDTH) + (some_wx + LoadTile_X))] != 0)
                    &&
                    (CONTX_NmeStrMap[wp][((some_wy + LoadTile_Y) * WORLD_WIDTH) + (some_wx + LoadTile_X)] == 0)
                )
                {

                    Adj_Empty_Land = ST_TRUE;

                }

            }

        }

        if(Adj_Empty_Land == ST_TRUE)
        {

            distance = Delta_XY_With_Wrap(LoadTile_X, LoadTile_Y, wx, wy, WORLD_WIDTH);

            if(distance < closest_distance)
            {

                closest_distance = distance;

                Return_X = LoadTile_X;

                Return_Y = LoadTile_Y;

            }

        }

        Next_Tile_ChainIndex = CONTX_LoadTChain[wp][Next_Tile_ChainIndex];

    }

    if(closest_distance != 2000)
    {

        *RetX = Return_X;
        
        *RetY = Return_Y;
        
    }
    else
    {

        *RetX = wx;
        
        *RetY = wy;

    }

}


// WZD o162p22
// drake178: sub_F6316()

// WZD o162p23
// drake178: TILE_AI_FindEmptyLnd()
/*
; checks whether there is an empty land tile adjacent
; to the specified square
; returns 1 and the coordinates if successful, or 0 if
; there are no adjacent empty land tiles
;
; WARNING: requires the CONTXXX EMS mapping
*/
/*

returns {F,T}
maybe, in-outs new wx,wy of unoccupied land square

*/
int16_t TILE_AI_FindEmptyLnd__WIP(int16_t wx, int16_t wy, int16_t wp, int16_t * RetX, int16_t * RetY)
{
    int16_t Unwrapped_X = 0;
    int16_t some_wy = 0;  // _DI_
    int16_t some_wx = 0;  // _SI_

    for(some_wy = (wy - 1); (wy + 2) > some_wy; some_wy++)
    {

        if(
            (some_wy > 0)
            &&
            (some_wy < WORLD_HEIGHT)
        )
        {

            for(Unwrapped_X = (wy - 1); (wy + 2) > Unwrapped_X; Unwrapped_X++)
            {

                some_wx = Unwrapped_X;

                if(some_wx < 0)
                {

                    some_wx += WORLD_WIDTH;

                }

                if(some_wx >= WORLD_WIDTH)
                {

                    some_wx -= WORLD_WIDTH;

                }

                if(
                    (Square_Is_Land(some_wx, some_wy, wp) == ST_TRUE)
                    &&
                    CONTX_NmeStrMap[wp][((some_wy * WORLD_WIDTH) + some_wx)]
                )
                {

                    *RetX = some_wx;
                    
                    *RetY = some_wy;

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
// drake178: TILE_NextFreeLand()
/*
; looks for an adjacent empty land tile
; returns 1 along with the coordinates; or 0 if there
; aren't any
*/
/*

returns {F,T} 'found target'
in-outs wx,wy

¿ weird indexing of the local array ?

*/
int16_t TILE_NextFreeLand__WIP(int16_t wx, int16_t wy, int16_t wp, int16_t * RetX, int16_t * RetY)
{
    int16_t map_square_area[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Checked_Y = 0;
    int16_t Checked_X = 0;
    int16_t itr1 = 0;  // _SI_
    int16_t itr2 = 0;  // _DI_

// ; set up the adjacent tile array - walkable tiles are
// ; 1s, oceans and shores are 0s

    for(itr1 = 0; itr1 < 3; itr1++)
    {

        for(itr1 = 0; itr1 < 3; itr1++)
        {

            if(Square_Is_Land((wx + itr2 - 1), (wy + itr1 - 1), wp) == ST_FALSE)
            {

                map_square_area[((itr1 * 6) + itr2)] = ST_FALSE;

            }
            else
            {

                map_square_area[((itr1 * 6) + itr2)] = ST_TRUE;

            }

        }

    }

// ; clear the value of any tile that has a unit on it

    map_square_area[4] = 0;

    for(itr1 = 0; itr1 < _units; itr1++)
    {

        if(_UNITS[itr1].wp == wp)
        {

            Checked_X = _UNITS[itr1].wx;

            Checked_Y = _UNITS[itr1].wy;

            if(
                (abs((Checked_X - wx)) < 2)
                &&
                (abs((Checked_Y - wy)) < 2)
            )
            {

                map_square_area[(4 + (((Checked_Y - wy) * 6) + (Checked_X - wx)))] = 0;

            }

        }

    }

// clear the value of any tile with an intact encounter

    for(itr1 = 0; itr1 < NUM_LAIRS; itr1++)
    {

        if(
            (_LAIRS[itr1].Intact == ST_TRUE)
            &&
            (_LAIRS[itr1].wp == wp)
        )
        {

            Checked_X = _LAIRS[itr1].wx;

            Checked_Y = _LAIRS[itr1].wy;

            if(
                (abs((Checked_X - wx)) < 2)
                &&
                (abs((Checked_Y - wy)) < 2)
            )
            {

                map_square_area[(4 + ((Checked_Y - wy) * 6) + (Checked_X - wx))] = 0;

            }

        }

    }

// ; clear the value of any tile with a city on it

    for(itr1 = 0; itr1 < _cities; itr1++)
    {

        if(_CITIES[itr1].wp == wp)
        {

            Checked_X = _CITIES[itr1].wx;

            Checked_Y = _CITIES[itr1].wy;

            if(
                (abs((Checked_X - wx)) < 2)
                &&
                (abs((Checked_Y - wy)) < 2)
            )
            {

                map_square_area[(4 + ((Checked_Y - wy) * 6) + (Checked_X - wx))] = 0;

            }


        }

    }

    for(itr1 = 0; itr1 < 3; itr1++)
    {

        for(itr1 = 0; itr1 < 3; itr1++)
        {

            if(map_square_area[((itr1 * 6) + itr2)] == ST_TRUE)
            {

                *RetX = (wx + itr2 - 1);

                *RetY = (wy + itr1 - 1);

                return ST_TRUE;

            }

        }

    }

    return ST_FALSE;

}


// WZD o162p33
// drake178: EVNT_MakeRampageList()

// WZD o162p34
// drake178: AI_TILE_CanLeave()

// WZD o162p35
// drake178: AI_Continent_Reeval()
/*
*/
/*



*/
void AI_Continent_Reeval__WIP(int16_t player_idx)
{
    // ; not own unit strength (value total)
    int16_t Sum_City_Y_NonOwn_UnitCost[2][60] = {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    };
    // ; own unit strength (value total)
    int16_t Sum_City_X_Own_UnitCost[2][60] = {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    };
    int16_t Tile_Linear = 0;
    int16_t found_target = 0;
    int16_t Center_Y = 0;
    int16_t Target_Tile_Landmass = 0;
    int16_t Tile_Weight = 0;
    int16_t Target_Tile_Y = 0;
    int16_t Target_Tile_X = 0;
    int16_t Center_X = 0;
    int16_t Best_Tile_Weight = 0;
    int16_t unit_type = 0;
    int16_t Y_2 = 0;
    int16_t X_2 = 0;
    int16_t Next_Tile_ChainIndex = 0;
    int16_t city_count = 0;
    int16_t Tile_Y = 0;
    int16_t Tile_X = 0;
    int16_t itr = 0;
    int16_t landmass_idx = 0;
    int16_t wp = 0;  // _DI_


    EMM_Map_CONTXXX__WIP();


    found_target = ST_FALSE;


// ; check if there's at least one type 3 continent that
// ; has a non-owned unit or structure at its saved tile

    for(wp = 0; wp < NUM_PLANES; wp++)
    {
        
        for(landmass_idx = 0; ((landmass_idx < NUM_PLANES) && (found_target == ST_FALSE)); landmass_idx++)
        {

            if(_ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] == CONT_NoPresence)
            {

                if(CONTX_NmeStrMap[wp][((_ai_continents.plane[wp].player[player_idx].Y_Coords[landmass_idx] * WORLD_WIDTH) + _ai_continents.plane[wp].player[player_idx].X_Coords[landmass_idx])] != 0)
                {

                    found_target = ST_TRUE;
                    
                }

            }

        }

    }


    _ai_reevaluate_continents_countdown[player_idx] -= 1;

    if(_ai_reevaluate_continents_countdown[player_idx] > 0)
    {

        return ;

    }

    _ai_reevaluate_continents_countdown[(25 + Random(10) + Random(10))] = 0;


// ; cancel the orders of all non-engineer units moving
// ; to a tile with no enemy presence
// ; 
// ; BUG: fails to check the plane of movement
// ; BUG? settlers and transports are not excluded

    for(itr = 0; itr < _units; itr++)
    {

        if(
            (_UNITS[itr].owner_idx == player_idx)
            &&
            (_unit_type_table[_UNITS[itr].type].Construction == 0)
            &&
            (CONTX_NmeStrMap[wp][((_UNITS[itr].dst_wy * WORLD_WIDTH) + _UNITS[itr].dst_wx)] == 0)
        )
        {

            _UNITS[itr].Status = us_Ready;  // 'MoM Demo': NO ORDERS

        }

    }


// ; clear the player's continent type global array, as
// ; well as the local coordinate arrays

    for(wp = 0; wp < NUM_PLANES; wp++)
    {
        
        for(landmass_idx = 0; ((landmass_idx < NUM_PLANES) && (found_target == ST_FALSE)); landmass_idx++)
        {

            _ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] = CONT_Invalid;  // actually just 0, cause its about to just be a city count

            // ; own unit strength (value total)
            Sum_City_X_Own_UnitCost[wp][landmass_idx] = 0;

            // ; not own unit strength (value total)
            Sum_City_Y_NonOwn_UnitCost[wp][landmass_idx] = 0;

        }

    }


// ; add the X/Y coordinates of owned cities into the
// ; local arrays, and count them into the global one
// ; 
// ; WARNING: limited to 60 continents

    for(itr = 0; itr < _cities; itr++)
    {

        landmass_idx = _landmasses[((_CITIES[itr].wp * WORLD_SIZE) + (_CITIES[itr].wy * WORLD_WIDTH) + _CITIES[itr].wx)];

        if(landmass_idx < NUM_LANDMASSES)
        {

            Sum_City_X_Own_UnitCost[wp][landmass_idx] += _CITIES[itr].wx;

            Sum_City_Y_NonOwn_UnitCost[wp][landmass_idx] += _CITIES[itr].wy;

            _ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] += 1;

        }
        
    }


// ; divide the local coordinates by the global count to
// ; get center points, and store these into the global
// ; arrays

    for(wp = 0; wp < NUM_PLANES; wp++)
    {
        
        for(landmass_idx = 0; ((landmass_idx < NUM_PLANES) && (found_target == ST_FALSE)); landmass_idx++)
        {

            city_count = _ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx];

            if(city_count > 0)
            {

                _ai_continents.plane[wp].player[player_idx].X_Coords[landmass_idx] = (Sum_City_X_Own_UnitCost[wp][landmass_idx] / city_count);

                _ai_continents.plane[wp].player[player_idx].Y_Coords[landmass_idx] = (Sum_City_Y_NonOwn_UnitCost[wp][landmass_idx] / city_count);

            }

        }

    }


// ; re-clear the local coordinate array

    for(itr = 0; itr < NUM_LANDMASSES; itr++)
    {

        Sum_City_X_Own_UnitCost[0][itr] = 0;

        Sum_City_X_Own_UnitCost[1][itr] = 0;

        Sum_City_Y_NonOwn_UnitCost[0][itr] = 0;

        Sum_City_Y_NonOwn_UnitCost[1][itr] = 0;

    }


// ; add the total cost of owned units on each landmass
// ; into the X coordinate array, and other units into the
// ; Y one, ignoring settlers in both cases

    for(itr = 0; itr < _units; itr++)
    {

        unit_type = _UNITS[itr].type;

        if((_unit_type_table[unit_type].Abilities & UA_CREATEOUTPOST) == 0)
        {

            landmass_idx = _landmasses[((_UNITS[itr].wp * WORLD_SIZE) + (_UNITS[itr].wy * WORLD_WIDTH) + _UNITS[itr].wx)];

            if(landmass_idx < NUM_LANDMASSES)
            {

                if(_UNITS[itr].owner_idx == player_idx)
                {

                    Sum_City_X_Own_UnitCost[wp][landmass_idx] += _unit_type_table[unit_type].cost;

                }
                else
                {

                    Sum_City_Y_NonOwn_UnitCost[wp][landmass_idx] += _unit_type_table[unit_type].cost;

                }

            }

        }

    }


// ; mark every continent as own, contested, or no city
// ; presence
// ; 
// ; BUG: ignores continents with exactly 6 own cities
// ; and no enemy presence - they remain the no targets
// ; type (6)

    for(wp = 0; wp < NUM_PLANES; wp++)
    {
        
        // ¿ BUGBUG  checking for landmass status/type, but actually just city count ?
        for(landmass_idx = 0; ((landmass_idx < NUM_PLANES) && (found_target == ST_FALSE)); landmass_idx++)
        {

            if(
                (_ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] != CONT_NoTargets)  // HERE: still just city count?
                ||
                (Sum_City_Y_NonOwn_UnitCost[wp][itr] != 0)
            )
            {

                if(_ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] > CONT_Invalid)  // HERE: still just city count?
                {

                    // ; BUG: this can easily overflow on a large continent,
                    // ;  even unsigned

                    // ; BUG: unsigned comparison after signed multiplication

                    if(Sum_City_X_Own_UnitCost[wp][itr] > (Sum_City_Y_NonOwn_UnitCost[wp][itr] / 10))
                    {

                        _ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] = CONT_Own;

                    }
                    else
                    {

                        _ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] = CONT_Contested;

                    }

                }
                else
                {

                    _ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] = CONT_NoPresence;

                }

            }

        }

    }


// ; if the continent has no own city, but has an allied
// ; one, mark it for no landing instead

    for(itr = 0; itr < _cities; itr++)
    {

        if(
            (_CITIES[itr].owner_idx != player_idx)
            &&
            (_CITIES[itr].owner_idx == NEUTRAL_PLAYER_IDX)
            &&
            (_players[_CITIES[itr].owner_idx].Dipl.Dipl_Status[player_idx] == DIPL_Alliance)
        )
        {

            landmass_idx = _landmasses[((_CITIES[itr].wp * WORLD_SIZE) + (_CITIES[itr].wy * WORLD_WIDTH) + _CITIES[itr].wx)];

            if(_ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] == CONT_NoPresence)
            {

                _ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] = CONT_NoLanding;

            }

        }

    }


    _ai_continents.plane[0].player[player_idx].Cont_Types[landmass_idx] = CONT_NoLanding;

    _ai_continents.plane[1].player[player_idx].Cont_Types[landmass_idx] = CONT_NoLanding;


// ; choose the next colony continent from among those
// ; with no presence, or clear it if no such ones exist

    for(itr = 0; itr < NUM_PLANES; itr++)
    {

        // ; select an empty target tile on the plane - skip this
        // ; step on forced recalculation (type 3 no longer empty)

        Best_Tile_Weight = 1000;

        for(landmass_idx = 1; landmass_idx < NUM_LANDMASSES; landmass_idx++)
        {

            if(
                (_ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] == CONT_NoPresence)
                &&
                (Sum_City_Y_NonOwn_UnitCost[wp][itr] == 0)
            )
            {

                Next_Tile_ChainIndex = CONTX_FirstTiles[wp][landmass_idx];

                while(((Next_Tile_ChainIndex = CONTX_TileChain[wp][Next_Tile_ChainIndex] != ST_UNDEFINED) && (found_target != ST_FALSE)))
                {

                    Tile_X = CONTX_TileXs[wp][Next_Tile_ChainIndex];

                    Tile_Y = CONTX_TileYs[wp][Next_Tile_ChainIndex];

                    Tile_Linear = ((Tile_Y * WORLD_WIDTH) + Tile_X);

                    if(CONTX_NmeStrMap[wp][Tile_Linear] == 0)
                    {

                        // ; BUG: uninitialized variables
                        Tile_Weight = (Delta_XY_With_Wrap(Tile_X, Tile_Y, X_2, Y_2, WORLD_WIDTH) + Random(20));

                        if(Tile_Weight < Best_Tile_Weight)
                        {

                            Best_Tile_Weight = Tile_Weight;

                            Target_Tile_X = CONTX_TileXs[wp][Next_Tile_ChainIndex];

                            Target_Tile_Y = CONTX_TileXs[wp][Next_Tile_ChainIndex];

                            Target_Tile_Landmass = landmass_idx;

                        }

                    }

                }

            }

        }

        if(Best_Tile_Weight < 1000)
        {

            AI_NewColConts[wp][player_idx] = Target_Tile_Landmass;

            AI_NewColTgtXs[wp][player_idx] = Target_Tile_X;

            AI_NewColTgtYs[wp][player_idx] = Target_Tile_Y;

        }
        else
        {

            AI_NewColConts[wp][player_idx] = 0;

        }

    }


// ; ensure that the rally points of own, contested, and
// ; no landing continents are empty land tiles on them,
// ; or otherwise set them to [0,0] re-marking the
// ; continent as not landable
// ; 
// ; WARNING: excludes tiles with structures

    for(itr = 0; itr < NUM_PLANES; itr++)
    {

        for(landmass_idx = 1; landmass_idx < NUM_LANDMASSES; landmass_idx++)
        {

            if(
                (_ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] == CONT_Contested)
                &&
                (_ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] == CONT_Own)
                &&
                (_ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] == CONT_NoLanding)
            )
            {

                Center_X = _ai_continents.plane[wp].player[player_idx].X_Coords[landmass_idx];

                Center_Y = _ai_continents.plane[wp].player[player_idx].Y_Coords[landmass_idx];

                if(
                    (CONTX_NmeStrMap[wp][Tile_Linear] == 0)
                    &&
                    (_landmasses[((wp * WORLD_SIZE) + (Center_Y * WORLD_WIDTH) + Center_X)] == landmass_idx)
                )
                {

                    // ;find the closest empty tile on the same continent, if any

                    Best_Tile_Weight = 1000;

                    Next_Tile_ChainIndex = CONTX_FirstTiles[wp][landmass_idx];

                    // while(Next_Tile_ChainIndex = CONTX_TileChain[wp][Next_Tile_ChainIndex] != ST_UNDEFINED)
                    while(Next_Tile_ChainIndex != ST_UNDEFINED)
                    {

                        Tile_Weight = Delta_XY_With_Wrap(CONTX_TileXs[wp][Next_Tile_ChainIndex], CONTX_TileYs[wp][Next_Tile_ChainIndex], Center_X, Center_Y, WORLD_WIDTH);

                        if(Tile_Weight < Best_Tile_Weight)
                        {

                            Tile_X = CONTX_TileXs[wp][Next_Tile_ChainIndex];

                            Tile_Y = CONTX_TileYs[wp][Next_Tile_ChainIndex];

                            found_target = CONTX_NmeStrMap[wp][((Tile_Y * WORLD_WIDTH) + Tile_X)];

                            if(found_target == ST_FALSE)
                            {

                                Best_Tile_Weight = Tile_Weight;

                                Target_Tile_X = CONTX_TileXs[wp][Next_Tile_ChainIndex];

                                Target_Tile_Y = CONTX_TileXs[wp][Next_Tile_ChainIndex];

                                Target_Tile_Landmass = landmass_idx;

                            }

                        }

                        Next_Tile_ChainIndex = CONTX_TileChain[wp][Next_Tile_ChainIndex];

                    }

                    if(Best_Tile_Weight < 1000)
                    {

                        _ai_continents.plane[wp].player[player_idx].X_Coords[landmass_idx] = Target_Tile_X;

                        _ai_continents.plane[wp].player[player_idx].Y_Coords[landmass_idx] = Target_Tile_Y;

                    }
                    else
                    {

                        _ai_continents.plane[wp].player[player_idx].X_Coords[landmass_idx] = 0;

                        _ai_continents.plane[wp].player[player_idx].Y_Coords[landmass_idx] = 0;

                        _ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] = CONT_NoLanding;

                    }

                }

            }

        }

    }


// ; for continents with no presence, ensure that the
// ; rally point is a valid disembarking tile, or if that
// ; is not possible, re-mark the continent as not
// ; landable with a rally point of [0,0]


    for(itr = 0; itr < NUM_PLANES; itr++)
    {

        for(landmass_idx = 1; landmass_idx < NUM_LANDMASSES; landmass_idx++)
        {

            if(_ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] == CONT_NoPresence)
            {

                Center_X = _ai_continents.plane[wp].player[player_idx].X_Coords[landmass_idx];

                Center_Y = _ai_continents.plane[wp].player[player_idx].Y_Coords[landmass_idx];

                Best_Tile_Weight = 1000;

                Next_Tile_ChainIndex = CONTX_1stLoadTs[wp][landmass_idx];

                // while(Next_Tile_ChainIndex = CONTX_LoadTChain[wp][Next_Tile_ChainIndex] != ST_UNDEFINED)
                while(Next_Tile_ChainIndex != ST_UNDEFINED)
                {

                    Tile_X = CONTX_LoadTileXs[wp][Next_Tile_ChainIndex];

                    Tile_Y = CONTX_LoadTileYs[wp][Next_Tile_ChainIndex];

                    if(CONTX_NmeStrMap[wp][((Tile_Y * WORLD_WIDTH) + Tile_X)] == 0)
                    {

                        Tile_Weight = Delta_XY_With_Wrap(Tile_X, Tile_Y, Center_X, Center_Y, WORLD_WIDTH);

                        if(Tile_Weight < Best_Tile_Weight)
                        {

                            Best_Tile_Weight = Tile_Weight;

                            Target_Tile_X = CONTX_LoadTileXs[wp][Next_Tile_ChainIndex];

                            Target_Tile_Y = CONTX_LoadTileYs[wp][Next_Tile_ChainIndex];

                            Target_Tile_Landmass = landmass_idx;

                        }

                    }

                    Next_Tile_ChainIndex = CONTX_LoadTChain[wp][Next_Tile_ChainIndex];

                }

                if(Best_Tile_Weight < 1000)
                {

                    _ai_continents.plane[wp].player[player_idx].X_Coords[landmass_idx] = Target_Tile_X;

                    _ai_continents.plane[wp].player[player_idx].Y_Coords[landmass_idx] = Target_Tile_Y;

                }
                else
                {

                    _ai_continents.plane[wp].player[player_idx].X_Coords[landmass_idx] = 0;

                    _ai_continents.plane[wp].player[player_idx].Y_Coords[landmass_idx] = 0;

                    _ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] = CONT_NoLanding;

                }

            }

        }

    }


// ; if there is an ongoing war (main war continent is
// ; set on the plane), ensure that the rally point of all
// ; own continents is a valid embarking tile

    for(itr = 0; itr < NUM_PLANES; itr++)
    {

        if(AI_MainWarConts[wp][player_idx] != 0)
        {

            for(landmass_idx = 1; landmass_idx < NUM_LANDMASSES; landmass_idx++)
            {

                if(_ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx] == CONT_Own)
                {

                    X_2 = 0;

                    Y_2 = 0;

                    Tile_Linear = 0;

                    Next_Tile_ChainIndex = CONTX_1stLoadTs[wp][AI_MainWarConts[wp][player_idx]];

                    // while(Next_Tile_ChainIndex = CONTX_LoadTChain[wp][Next_Tile_ChainIndex] != ST_UNDEFINED)
                    while(Next_Tile_ChainIndex != ST_UNDEFINED)
                    {

                        X_2 = CONTX_LoadTileXs[wp][Next_Tile_ChainIndex];

                        Y_2 = CONTX_LoadTileYs[wp][Next_Tile_ChainIndex];

                        Tile_Linear++;

                        Next_Tile_ChainIndex = CONTX_LoadTChain[wp][Next_Tile_ChainIndex];

                    }

                    X_2 /= Tile_Linear;

                    Y_2 /= Tile_Linear;
                    
                    Best_Tile_Weight = 1000;

                    Next_Tile_ChainIndex = CONTX_1stLoadTs[wp][landmass_idx];

                    // while(Next_Tile_ChainIndex = CONTX_LoadTChain[wp][Next_Tile_ChainIndex] != ST_UNDEFINED)
                    while(Next_Tile_ChainIndex != ST_UNDEFINED)
                    {

                        Tile_Weight = Delta_XY_With_Wrap(CONTX_LoadTileXs[wp][Next_Tile_ChainIndex], CONTX_LoadTileYs[wp][Next_Tile_ChainIndex], X_2, Y_2, WORLD_WIDTH);

                        if(Tile_Weight < Best_Tile_Weight)
                        {

                            Tile_X = CONTX_LoadTileXs[wp][Next_Tile_ChainIndex];

                            Tile_Y = CONTX_LoadTileYs[wp][Next_Tile_ChainIndex];

                            if(CONTX_NmeStrMap[wp][((Tile_Y * WORLD_WIDTH) + Tile_X)] == 0)
                            {

                                Best_Tile_Weight = Tile_Weight;

                                Target_Tile_X = Tile_X;

                                Target_Tile_Y = Tile_Y;

                            }

                        }

                        Next_Tile_ChainIndex = CONTX_LoadTChain[wp][Next_Tile_ChainIndex];

                    }

                    if(Best_Tile_Weight < 1000)
                    {

                        _ai_continents.plane[wp].player[player_idx].X_Coords[landmass_idx] = Target_Tile_X;

                        _ai_continents.plane[wp].player[player_idx].Y_Coords[landmass_idx] = Target_Tile_Y;

                    }

                }
        
            }

        }

    }


    // DONT  EMM_Map_DataH();

}


// WZD o162p36
// drake178: AI_SetEnemyStrMaps()
/*
; maps in and fills out the CONTXXX enemy strength
; arrays
;
; BUG? the maps are only 4800 bytes despite their
; allocated 9600
*/
/*



*/
void AI_SetEnemyStrMaps(int16_t player_idx)
{
    int16_t nonhostiles[NUM_PLAYERS] = { 0, 0, 0, 0, 0, 0 };
    int16_t xy_ofst = 0;
    int16_t unit_owner_idx = 0;
    int16_t map_square_count = 0;
    int16_t itr = 0;  // _SI_
    int16_t itr_units = 0;  // _SI_
    int16_t wp = 0;  // _DI_

    EMM_Map_CONTXXX__WIP();

    for(itr = 0; itr < NUM_PLAYERS; itr++)
    {

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

    }

    map_square_count = WORLD_SIZE;

    for(wp = 0; wp < NUM_PLANES; wp++)
    {

        for(itr = 0; itr < map_square_count; itr++)
        {

            CONTX_NmeStrMap[wp][itr] = 0;

        }

    }

    for(itr_units = 0; itr_units < _units; itr_units++)
    {

        /* HACK */  if(
        /* HACK */      (_UNITS[itr_units].owner_idx == ST_UNDEFINED)
        /* HACK */      ||
        /* HACK */      (_UNITS[itr_units].wp == ST_UNDEFINED)
        /* HACK */  )
        /* HACK */  {
        /* HACK */      continue;
        /* HACK */  }

        unit_owner_idx = _UNITS[itr_units].owner_idx;

        if(
            (unit_owner_idx != player_idx)
            &&
            (_UNITS[itr_units].owner_idx != player_idx)  // ; conflicting condition - will always jump
        )
        {

            wp = _UNITS[itr_units].wp;

            xy_ofst = ((_UNITS[itr_units].wy * WORLD_WIDTH) + _UNITS[itr_units].wx);

            CONTX_NmeStrMap[wp][xy_ofst] += (Effective_Unit_Strength(itr_units) / 10);

            if(nonhostiles[unit_owner_idx] == ST_TRUE)
            {
                CONTX_NmeStrMap[wp][xy_ofst] |= AI_TARGET_NONHOSTILE;
            }

        }

    }

    for(itr = 0; itr < NUM_LAIRS; itr++)
    {

        if(
            (_LAIRS[itr].Intact == ST_TRUE)
            &&
            (_LAIRS[itr].guard1_count > 0)
        )
        {

            wp = _LAIRS[itr].wp;

            xy_ofst = ((_LAIRS[itr].wy * WORLD_WIDTH) + _LAIRS[itr].wx);

            CONTX_NmeStrMap[wp][xy_ofst] += ((Effective_Unit_Type_Strength((_LAIRS[itr].guard1_unit_type & 0x0F)) / 10) * _LAIRS[itr].guard1_count);

            CONTX_NmeStrMap[wp][xy_ofst] += ((Effective_Unit_Type_Strength((_LAIRS[itr].guard2_unit_type & 0x0F)) / 10) * _LAIRS[itr].guard2_count);

        }

    }

    for(itr = 0; itr < _cities; itr++)
    {

        wp = _CITIES[itr].wp;

        xy_ofst = ((_CITIES[itr].wy * WORLD_WIDTH) + _CITIES[itr].wx);

        CONTX_NmeStrMap[wp][xy_ofst] |= AI_TARGET_SITE;

        if(_CITIES[itr].owner_idx != player_idx)
        {

            CONTX_NmeStrMap[wp][xy_ofst] += 1;

        }

    }

    for(itr = 0; itr < NUM_LAIRS; itr++)
    {

        if(_LAIRS[itr].Intact != ST_FALSE)
        {

            wp = _LAIRS[itr].wp;

            xy_ofst = ((_LAIRS[itr].wy * WORLD_WIDTH) + _LAIRS[itr].wx);

            CONTX_NmeStrMap[wp][xy_ofst] |= AI_TARGET_SITE;

        }

    }

    for(itr = 0; itr < NUM_NODES; itr++)
    {

        wp = _NODES[itr].wp;

        xy_ofst = ((_NODES[itr].wy * WORLD_WIDTH) + _NODES[itr].wx);

        CONTX_NmeStrMap[wp][xy_ofst] |= AI_TARGET_SITE;

    }

}


// WZD o162p37
// drake178: AI_Pick_Action_Conts()
/*
; selects the main action continents for both Arcanus
; and Myrror... I think anyway
;
; contains a host of BUGs, need to re-examine later
; Attributes: bp-based frame
*/
/*



*/
void AI_Pick_Action_Conts__WIP(int16_t player_idx)
{
    int16_t Continent_Values_2[60] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Continent_Values[60] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t city_owner_idx = 0;
    int16_t First_Hostile_Player = 0;
    int16_t Reevaluate = 0;
    int16_t landmass_idx = 0;
    int16_t Own_City_Count = 0;
    int16_t distance = 0;  // DNE in Dasm; reuses Own_City_Count
    int16_t Transport_Tile_Y = 0;
    int16_t Transport_Tile_X = 0;
    int16_t Value_Sum = 0;
    int16_t Landmass_Loopvar = 0;
    int16_t Closest_LoadTile_Dist = 0;
    int16_t wp = 0;
    int16_t Territory_Center_Y = 0;
    int16_t Territory_Center_X = 0;
    int16_t itr_players = 0;  // _DI_
    int16_t itr_cities = 0;  // _DI_
    int16_t itr_landmasses = 0;  // _DI_
    uint8_t continent_status;  // DNE in Dasm


    First_Hostile_Player = ST_UNDEFINED;


    EMM_Map_CONTXXX__WIP();

    // ; check if there are any hostile players (at war with
    // ; or casting SoM)
    // ; 
    // ; BUG: checks Fire Elemental instead of SoM
    // ; BUG: does not exclude self

    for(itr_players = 0; ((itr_players < _num_players) & (First_Hostile_Player == ST_UNDEFINED)); itr_players++)
    {

        if(
            (_players[player_idx].Dipl.Dipl_Status[itr_players] < 3)
            ||
            (_players[itr_players].casting_spell_idx == spl_Fire_Elemental)
        )
        {

            First_Hostile_Player = itr_players;

        }

    }


    for(wp = 0; wp < 2; wp++)
    {

        Reevaluate = ST_FALSE;

        landmass_idx = AI_MainWarConts[wp][player_idx];

        if(landmass_idx == 0)
        {

            Reevaluate = ST_TRUE;

        }
        else
        {

            continent_status = _ai_continents.plane[wp].player[player_idx].Cont_Types[landmass_idx];
            // -1, so {1,2,3,4,5}
            switch(continent_status)
            {

                case 0:
                case 3:
                case 4:
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

                    if(First_Hostile_Player > ST_UNDEFINED)
                    {

                        if(Continent_Values[First_Hostile_Player] == 0)
                        {

                            Reevaluate = ST_TRUE;

                        }
                        else
                        {

                            Reevaluate = ST_FALSE;

                        }

                    }
                    else
                    {
                        
                        Reevaluate = ST_TRUE;

                        if(Continent_Values[NEUTRAL_PLAYER_IDX] > 0)
                        {

                            Reevaluate = ST_FALSE;

                        }
                        else
                        {

                            for(itr_players = 0; itr_players < _num_players; itr_players++)
                            {

                                if(Reevaluate == ST_TRUE)
                                {

                                    if(_players[player_idx].Hostility[itr_players] >= 2)
                                    {

                                        if(Continent_Values[itr_players] > 0)
                                        {

                                            Reevaluate = ST_FALSE;

                                        }

                                    }

                                }

                            }

                        }

                    }


                } break;

                case 1:
                case 2:
                {
                    
                    Reevaluate = ST_FALSE;

                } break;

                case 5:
                {
                    
                    Reevaluate = ST_TRUE;

                } break;
                
            }

        }


        if(Reevaluate == ST_TRUE)
        {

            for(itr_landmasses = 0; itr_landmasses < NUM_LANDMASSES; itr_landmasses++)
            {

                Continent_Values_2[itr_landmasses] = 0;

                Continent_Values[itr_landmasses] = 0;

            }

            Territory_Center_X = 0;

            Territory_Center_Y = 0;

            Own_City_Count = 0;

            for(itr_cities = 0; itr_cities < _cities; itr_cities++)
            {

                if(_CITIES[itr_cities].wp == wp)
                {

                    city_owner_idx = _CITIES[itr_cities].owner_idx;

                    if(city_owner_idx == player_idx)
                    {

                        Territory_Center_X += _CITIES[itr_cities].wx;

                        Territory_Center_Y += _CITIES[itr_cities].wy;

                        Own_City_Count++;
                                            
                    }
                    else
                    {

                        if(First_Hostile_Player <= ST_UNDEFINED)
                        {

                            if(city_owner_idx == NEUTRAL_PLAYER_IDX)
                            {

                                Continent_Values[_landmasses[((wp * WORLD_SIZE) + (_CITIES[itr_cities].wy * WORLD_WIDTH) + _CITIES[itr_cities].wx)]] += 1;

                            }

                        }
                        else
                        {

                            if(city_owner_idx == First_Hostile_Player)
                            {

                                Continent_Values[_landmasses[((wp * WORLD_SIZE) + (_CITIES[itr_cities].wy * WORLD_WIDTH) + _CITIES[itr_cities].wx)]] += 10;

                            }
                            else
                            {

                                if(city_owner_idx == NEUTRAL_PLAYER_IDX)
                                {

                                    Continent_Values[_landmasses[((wp * WORLD_SIZE) + (_CITIES[itr_cities].wy * WORLD_WIDTH) + _CITIES[itr_cities].wx)]] += 1;

                                }

                            }

                        }

                    }

                }

            }

            if(Own_City_Count == 0)
            {

                Territory_Center_X = _FORTRESSES[player_idx].wx;

                Territory_Center_Y = _FORTRESSES[player_idx].wy;

            }
            else
            {

                Territory_Center_X /= Own_City_Count;

                Territory_Center_Y /= Own_City_Count;

            }

            // ; create a second array of continent values that is
            // ; modified by the distance from the continent center to
            // ; the closest transport loading tile - adding 1k to the
            // ; value of the fortress continent of the first hostile
            // ; player if hostility is 4
            // ; 
            // ; BUG? uses an array that holds previous data at this
            // ;  point in time?
            // ; BUG: only processes 59 continents
            // ; BUG: scaling everything by the same value does not
            // ;  change the weights

            for(itr_landmasses = 1; itr_landmasses < NUM_LANDMASSES; itr_landmasses++)
            {

                Value_Sum = CONTX_1stLoadTs[wp][itr_landmasses];

                Closest_LoadTile_Dist = 1000;

                // ; calculate the distance to the closest transport load
                // ; tile from the continent center, if any
                // ; 
                // ; BUG? uses an array that holds previous data at this
                // ; point in time?

                // while(Value_Sum = CONTX_LoadTChain[wp][Value_Sum] != ST_UNDEFINED)
                while(Value_Sum != ST_UNDEFINED)
                {

                    Transport_Tile_X = CONTX_LoadTileXs[wp][Value_Sum];

                    Transport_Tile_Y = CONTX_LoadTileYs[wp][Value_Sum];

                    // ; isn't this the previous data? the table is not
                    // ; recalculated before this point
                    CONTX_NmeStrMap[wp][((Transport_Tile_Y * WORLD_WIDTH) + Transport_Tile_X)] = 0;

                    // Own_City_Count = Delta_XY_With_Wrap(Transport_Tile_X, Transport_Tile_Y, Territory_Center_X, Territory_Center_Y, WORLD_WIDTH);
                    distance = Delta_XY_With_Wrap(Transport_Tile_X, Transport_Tile_Y, Territory_Center_X, Territory_Center_Y, WORLD_WIDTH);

                    if(distance < Closest_LoadTile_Dist)
                    {
                        Closest_LoadTile_Dist = distance;
                    }

                    Value_Sum = CONTX_LoadTChain[wp][Value_Sum];

                }

                if(Closest_LoadTile_Dist == 0)
                {

                    Closest_LoadTile_Dist = 1;

                }

                if(First_Hostile_Player <= ST_UNDEFINED)
                {

                    Continent_Values_2[itr_landmasses] = Continent_Values[itr_landmasses] * 1000 / Closest_LoadTile_Dist;

                    if(Continent_Values[itr_landmasses] > 0)
                    {

                        if(Continent_Values_2[itr_landmasses] < 10)
                        {

                            Continent_Values_2[itr_landmasses] = 10;

                        }

                    }

                }
                else
                {

                    if(Continent_Values[itr_landmasses] > 0)
                    {

                        Continent_Values_2[itr_landmasses] = ((2000 - (Continent_Values[itr_landmasses] * 10)) / Closest_LoadTile_Dist);

                        if(Continent_Values_2[itr_landmasses] < 10)
                        {

                            Continent_Values_2[itr_landmasses] = 10;

                        }

                    }

                    if(
                        (_players[player_idx].Hostility[First_Hostile_Player] == 4)
                        &&
                        (_FORTRESSES[First_Hostile_Player].wp == wp)
                    )
                    {

                        if(_landmasses[((wp * WORLD_SIZE) + (_FORTRESSES[First_Hostile_Player].wy * WORLD_WIDTH) + _FORTRESSES[First_Hostile_Player].wx)] == itr_landmasses)
                        {

                            Continent_Values_2[itr_landmasses] += 1000;

                        }

                    }

                }

            }

            for(itr_landmasses = 0; itr_landmasses < NUM_LANDMASSES; itr_landmasses++)
            {

                Value_Sum += Continent_Values_2[itr_landmasses];

            }

            // ; replace the modified value of all continents with
            // ; their tile count instead, with the exception of own
            // ; or no targets continents, and the fortress continent
            // ; 
            // ; BUG: fortress exclusion also counts the other plane

            if(Value_Sum == 0)
            {

                for(itr_landmasses = 0; itr_landmasses < NUM_LANDMASSES; itr_landmasses++)
                {

                    if(
                        (_ai_continents.plane[wp].player[player_idx].Cont_Types[itr_landmasses] != CONT_Own)
                        &&
                        (_ai_continents.plane[wp].player[player_idx].Cont_Types[itr_landmasses] != CONT_NoTargets)
                    )
                    {

                        if(
                            (_FORTRESSES[player_idx].active == ST_FALSE)
                            ||
                            (itr_landmasses == _landmasses[((wp * WORLD_SIZE) + (_FORTRESSES[player_idx].wy * WORLD_WIDTH) + _FORTRESSES[player_idx].wx)])
                        )
                        {

                            Value_Sum = CONTX_FirstTiles[wp][itr_landmasses];

                            // while(Value_Sum = CONTX_TileChain[wp][Value_Sum] != ST_UNDEFINED)
                            while(Value_Sum != ST_UNDEFINED)
                            {

                                Continent_Values_2[itr_landmasses] += 1;

                                Value_Sum = CONTX_TileChain[wp][Value_Sum];

                            }

                        }

                    }

                }

            }


            // ; sum up the values in the modified array
            // ; 
            // ; BUG: can only handle 60 continents

            Value_Sum = 0;

            for(itr_landmasses = 0; itr_landmasses < NUM_LANDMASSES; itr_landmasses++)
            {

                Continent_Values_2[itr_landmasses] += Value_Sum;

            }


            if(Value_Sum == 0)
            {

                AI_MainWarConts[wp][player_idx] = 0;

            }
            else
            {

                AI_MainWarConts[wp][player_idx] = Get_Weighted_Choice(&Continent_Values_2[0], NUM_LANDMASSES);

            }

        }

    }

}


// WZD o162p38
void Player_Clear_All_Unit_Orders(int16_t player_idx)
{
    int16_t itr = 0;  // _SI_

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
