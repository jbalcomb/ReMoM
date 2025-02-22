UnitList_Count

CRP_TILE_GetUnits_2()  ==>  Army_At_Square_3()

TILE_CanShiftPlanes__WIP()  ==>  Map_Square_Troops_Can_Plane_Shift()
AI_ShiftOffHomePlane()  ==>  AI_Shift_Off_Home_Plane()
AI_STK_MoveTransports()  ==>  AI_Stack_Set_Boats_Goto()
AI_Transport_Sailoff() ==>  AI_Move_Out_Boats()
AI_OppMoveOverride()  ==>  AI_Find_Opportunity_City_Target()
AI_FindClosestEnemy()  ==>  AI_Find_Nearest_Target_Unit()
AI_UNIT_SetMoveOrder()  ==>  AI_Set_Move_Or_Goto_Target()
AI_CONTX_Regroup()  ==>  AI_Build_Stacks_Find_Targets_Order_Moves()
TILE_IsLand()  ==>  Map_Square_Is_Land()
sub_EFBD6()  ==>  AI_sEFBD6__WIP()
sub_EFC92()  ==>  AI_sEFC92__WIP()
AI_UNIT_SetMeldOrder()  ==>  AI_Order_Meld()
AI_ProcessMelders()  ==>  AI_Do_Meld()
TILE_CheckEnemyUnits()  ==>  Map_Square_Area_Has_Opponent()
TILE_GetDistance()  ==> Range()  MoO2  Module: special
G_TILE_IsAIEmbarkable()  ==>  Map_Square_Is_Embarkable()
AI_UNIT_SeekTransport()  ==>  AI_Order_SeekTransport()
AI_UNIT_SettleOrder()  ==>  AI_Order_Settle()
AI_ProcessSettlers()  ==>  AI_Do_Settle()
AI_UNIT_PurifyOrder()  ==>  AI_Order_Purify()
AI_ProcessPurifiers()  ==>  AI_Do_Purify()
AI_UNIT_RoadBldOrder()  ==>  AI_Order_RoadBuild()
AI_ProcessRoadBuilds()  ==>  AI_Do_RoadBuild()
AI_CheckOtherPlane()  ==>  AI_Tower_Target_Worthwhile()
AI_CreateTargetList()  ==>  AI_Build_Target_List()

AI_ProcessOcean()  ==>  
G_AI_ProcessTransports()  ==>  

AI_TILE_CheckEnemies()  ==> AI_Enemy_Unit_In_Range()
AI_ExcessBuilders()  ==>  AI_Kill_Excess_Settlers_And_Engineers()




AI_Enemy_Unit_In_Range() ~== Map_Square_Area_Has_Opponent(), just adds a check for landmax_idx




unit_list_idx   index of list
list_unit_idx   index of unit in list
unit_list_idx   index of stack list
list_unit_idx   index of unit in list stack
count of stacks in list
count of units in list stack
    itr < _ai_own_stack_count                           count of stacks in list
    list_unit_count = _ai_own_stack_unit_count[itr]     count of units in list stack

for(itr_stacks = 0; itr_stacks < _ai_own_stack_count; itr_stacks++)
    UnitList_Count = _ai_own_stack_unit_count[itr_stacks];
    for(UnitList_Index = 0; UnitList_Index < UnitList_Count; UnitList_Index++)
itr_list_units
stack_idx ... _ai_own_stack_wx, _ai_own_stack_wy

CX_ID  ==>  unit_list_idx

CX_UL_ID  ==>  list_unit_idx

UnitList_Count  ==>  list_unit_count
itr_list_units

for(itr = 0; itr < _ai_own_stack_count; itr++)
itr_unit_lists

for(CX_UL_ID = 0; CX_UL_ID < list_unit_count; CX_UL_ID++)
itr_list_units

CX_ID= word ptr  8
CX_UL_ID= word ptr  0Ah

mov     ax, [bp+CONTX_Stack_Index]
cmp     ax, [_ai_own_stack_count]

cmp     _SI_itr_targets, [_ai_targets_count]
itr_targets





¿ API / Interface Design ?
Could/Should/Would embed the info in the function names?
Stack Unit List (SUL)
Stack List Unit (SLU)
Unit List (LU)
List Unit (UL)
Stack List (SL)
Stack List Unit (SLU)
...
AI_Set_Meld_List_Unit()
AI_Set_List_Unit_To_Meld()
AI_Set_SLU_To_Meld()
AI_Set_Meld_SLU()
AI_Order_Meld_SLU()



AI_SetUnitOrders()

AI_Move_Out_Boats()
    |-> AI_Stack_Set_Boats_Goto()




MoO2
Pick_Star_And_Attacker_()
    |-> Collect_Targets_For_Combat_()
Get_AI_Target_
_target[]







# AI_SetUnitOrders()

ai_human_hostility
EMM_Map_CONTXXX__WIP()
ai_seektransport_cnt
g_ai_minattackstack
AI_Disband_To_Balance_Budget()
AI_Shift_Off_Home_Plane()
AI_Move_Out_Boats()
AI_Find_Opportunity_City_Target()
AI_Build_Stacks_Find_Targets_Order_Moves()
    |-> AI_Find_Nearest_Target_Unit()

TILE_NextFreeLand__WIP()
    |-> Map_Square_Is_Land()

AI_GarrBuilderPush__WIP()
    |-> TILE_NextFreeLand__WIP()
    |-> AI_Set_Move_Or_Goto_Target()
    |-> TILE_NextFreeLand__WIP()
    |-> AI_Set_Move_Or_Goto_Target()

sub_EFBD6()
    |-> sub_EFC92()

AI_Do_Meld()
    |-> AI_Order_Meld()
    |-> AI_Set_Move_Or_Goto_Target()

AI_Do_Settle()
    |-> AI_CanSettleOffPlane()
    |-> AI_Order_Settle()
    |-> AI_SendToColonize()

AI_ProcessPurifiers()

AI_Do_RoadBuild()

AI_CreateTargetList()

AI_ProcessRoamers()

AI_PullForMainWar__WIP()


AI_ProcessOcean__WIP()




## AI_Shift_Off_Home_Plane()



## Map_Square_Troops_Can_Plane_Shift()

nay 'Planar Seal'
nay different owners
nay too many units
nay lair
nay city
if not ocean
    ¿ ?
if ocean
    any troop has windwalking
    any troop2 has windwalking
    all troops have air or water travel







## AI_Move_Out_Boats()

iters over AI_Own_Stack_Count
    AI_Own_Stacks[itr_stack].value != ST_UNDEFINED
    (AI_Own_Stacks[itr_stack].abilities & AICAP_Transport) != 0

Square_Is_OceanLike()

uses Square_Is_OceanLike(), not Square_Is_Sailable()
    Square_Is_OceanLike() ...only checks tt_AnimOcean



## AI_Stack_Set_Boats_Goto()

passed in ai_stack_idx
AI_Own_Stacks[ai_stack_idx]


What does it mean to set AI_Own_Stacks[].value to ST_UNDEFINED?



## AI_Find_Opportunity_City_Target()

set Units to us_Ready and stack value to ST_UNDEFINED
if AI Stack is us_GOTO and is next to an enemy city with three or less defenders



## AI_Build_Stacks_Find_Targets_Order_Moves()

builds _ai_own_stack_unit_list[]
finds nearest target
sets move|goto status


...
...
...
AI_Find_Nearest_Target_Unit()
...
AI_Set_Move_Or_Goto_Target()





## AI_Find_Nearest_Target_Unit()



## AI_Set_Move_Or_Goto_Target()


## TILE_NextFreeLand__WIP()

## Map_Square_Is_Land()
this should be in the terrain notes  MOVE IT!!!!!



## AI_GarrBuilderPush__WIP()
    |-> TILE_NextFreeLand__WIP()
    |-> AI_Set_Move_Or_Goto_Target()
    |-> TILE_NextFreeLand__WIP()
    |-> AI_Set_Move_Or_Goto_Target()



## sub_EFBD6()
    |-> sub_EFC92()



## AI_Do_Meld()
    |-> AI_Order_Meld()
    |-> AI_Set_Move_Or_Goto_Target()

_ai_own_stack_count
_ai_own_stack_unit_count

count of stacks in list
    count of units in stack

unit_list_idx
list_unit_idx



## AI_Do_Settle()
    |-> Map_Square_Area_Has_Opponent()
    |-> Map_Square_Survey
    ...
    |-> AI_CanSettleOffPlane()
    ...
    |-> AI_Order_Settle()
    ...
    |-> AI_SendToColonize()

Compute_Base_Map_Square_Values()

Map_Square_Is_Embarkable()

TILE_AI_FindLoadTile__WIP()



## AI_ProcessPurifiers()




## AI_Do_RoadBuild()



## AI_CreateTargetList()




## AI_ProcessRoamers()

((_ai_targets_strength[itr_targets] * 3 / 4) < effective_stack_strength)
distance = Delta_XY_With_Wrap(_ai_targets_wx[itr_targets], _ai_targets_wy[itr_targets], stack_wx, stack_wy, WORLD_WIDTH);
Target_Rating = (_ai_targets_value[itr_targets] / (distance * 4));
Need_Path_To_Target = _UNITS[unit_idx].Move_Failed;
    (Make_Move_Path(0, 0, 0, 0, 0, 0, stack_wx, stack_wy, _ai_targets_wx[itr_targets], _ai_targets_wy[itr_targets], wp, &movepath_x_array[2], &movepath_y_array[2], &movepath_cost_array[0], 1, 4, list_unit_count, list_unit_count, player_idx) > 0)



## AI_PullForMainWar__WIP()

for stack types 1 and 0
if Unit_Has_AirTravel() or Unit_Has_WaterTravel and not UA_MELD
AI_Set_Move_Or_Goto_Target()





## AI_ProcessOcean__WIP()

short-circuits
    if(AI_MainWarConts[wp][player_idx] == 0)
        return;
What does it mean for AI_MainWarConts[wp][player_idx] to be 0?

MainWar_Rally_X = _ai_continents.plane[wp].player[player_idx].X_Coords[AI_MainWarConts[wp][player_idx]];
MainWar_Rally_Y = _ai_continents.plane[wp].player[player_idx].Y_Coords[AI_MainWarConts[wp][player_idx]];
if((MainWar_Rally_X == 0) && (MainWar_Rally_X == 0))
What does it mean for the _ai_continentswx,wy to be 0?

unit_idx = _ai_own_stack_unit_list[itr_stacks][itr_list_units];
if(unit_idx == ST_UNDEFINED)
    Have_Assigned_Unit = ST_TRUE;
What does it mean for _ai_own_stack_unit_list[][] to be ST_UNDEFINED?




## G_AI_ProcessTransports__WIP()

¿ setting AI_SeekTransport_Ps[itr1] = ST_UNDEFINED means the boat stage poing is full ?









## AI_Own_Stacks[] & AI_Own_Stack_Count

struct s_AI_STACK_DATA AI_Own_Stacks[];

//  ; (sizeof=0xA)
struct s_AI_STACK_DATA
{
    /* 00 */  uint8_t wx;
    /* 01 */  uint8_t wy;
    /* 02 */  uint8_t wp;
    /* 03 */  uint8_t abilities;  /* ; enum AI_STK_Capabilities */
    /* 04 */  int16_t value;
    /* 06 */  uint8_t transport_capacity;
    /* 07 */  uint8_t unit_count;
    /* 08 */  uint8_t landmass_idx;
    /* 09 */  uint8_t unit_status;
    /* 0A */  
};



## AI_Enemy_Stacks[] & AI_Enemy_Stack_Count

struct s_AI_TARGET AI_Enemy_Stacks[];

//  ; (sizeof=0x6)
struct s_AI_TARGET
{
    /* 00 */ uint8_t wx;
    /* 01 */ uint8_t wy;
    /* 02 */ uint8_t wp;
    /* 03 */ uint8_t field_3;
    /* 04 */ int16_t value;
    /* 06 */ 
};
