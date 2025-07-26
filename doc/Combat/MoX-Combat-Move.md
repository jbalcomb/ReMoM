


## CMB_ActiveMoveMap



Teleporting . . . . . . . . Unit can move to any free square on the combat map for a cost of two movement points.
Teleport under Movement refers to the ability to teleport during combat, a special ability (see Special Unit Abilities).





Left-Click Combat Grid







¿ Combat_Move_Path_Valid() is MoO2  Module: COMBAT1  Set_Legal_Moves_() ?
calls Move_Cost_(), checks against Range_To_XY_()



CMB_FillTargetMaps__WIP()
...is always/only gets called after actions, changes in state, etc.
¿ Therefore, it is MoO2  Module: COMBAT1  Assign_Combat_Grids_(). ?
CMB_FillTargetMaps__WIP() ==> Assign_Combat_Grids()




    for(itr = 0; itr < _combat_total_unit_count; itr++)
        if((battle_units[itr].Status == bus_Active) && (battle_units[itr].Image_Effect != 5))
            CMB_TargetRows[battle_units[itr].cgy][battle_units[itr].cgx] = itr;  // batle_unit_idx


CMB_SetMoveMaps()
    les     bx, [battlefield]
    mov     [es:bx+(s_BATTLEFIELD.MoveCost_Ground+0EDh)], -1 ; tile [6,11] - central structure


Q: Who gets the memcpy() of the movement cost map?
A: CMB_ActiveMoveMap[], in BU_GetMoveMap__WIP()

...same indexing for CMB_TargetRows[] and CMB_NearBuffer_3[]...
...CMB_NearBuffer_3[((cgy * 21) + cgx)]
...CMB_TargetRows[cgy][cgx]


Assign_Combat_Grids()
Combat_Move_Path_Valid()

CMB_Units_Init__WIP()
Deploy_Battle_Units()

Tactical_Combat__WIP()
|-> CMB_Units_Init__WIP()
|-> Deploy_Battle_Units()

Tactical_Combat__WIP()
    |-> CMB_Units_Init__WIP()
        |-> Deploy_Battle_Units()

starting_cgx[ 0] = 14;
starting_cgy[ 0] = 12;

Melee_Units[] is 0

        battle_units[Melee_Units[itr]].cgx = starting_cgx[ctr];
        battle_units[Melee_Units[itr]].cgy = starting_cgy[ctr];

Hrrrmm... neeed a way to debug Deploy_Battle_Units() while knowing which side is being deployed...
...`player_idx == HUMAN_PLAYER_IDX`?

In the call to Combat_Move_Path_Valid(), battle_units[_active_battle_unit].cgx,cgy should (still) be 14,12.

dig in to Combat_Move_Path_Valid()...
...break on cgx,cgy
...at if(moves2 > Tile_GetTo_Cost)

    Combat_Move_Path_Valid(battle_units[_active_battle_unit].cgx, battle_units[_active_battle_unit].cgy, useable_moves2);

    if(
        (battle_units[_active_battle_unit].cgx == 14)
        &&
        (battle_units[_active_battle_unit].cgy == 12)
    )







combat_grid_field = Add_Grid_Field(0, 0, 1, 1, 319, 164, &Grid_X, &Grid_Y, ST_UNDEFINED);
...
if(input_field_idx == combat_grid_field)
    CMB_TargetFrame_X = Get_Combat_Grid_Cell_X((Grid_X + 4), (Grid_Y + 4));
    CMB_TargetFrame_Y = Get_Combat_Grid_Cell_Y((Grid_X + 4), (Grid_Y + 4));
    BU_CombatAction__WIP(_active_battle_unit, CMB_TargetFrame_X, CMB_TargetFrame_Y);
    for(itr = 0; itr < _combat_total_unit_count; itr++)
        BU_SetVisibility__WIP(itr);
    Assign_Combat_Grids();
    Combat_Winner = CMB_WinLoseFlee__WIP();
    if(Combat_Winner != ST_UNDEFINED)  /* invalid / no winner / none / neither */
        leave_screen = ST_UNDEFINED;
...
everything that matters here takes place under BU_CombatAction__WIP()
no idea why drake178 named those "TargetFrame"
    combat_grid_datum = CMB_TargetRows[y][x];
they are used to index CMB_TargetRows[], which is the array populated with *what* is in/on the target combat grid cell

There's a function that checks if a combat grid cell has a city wall. Sets 99 in CMB_TargetRows[].

treats 99 (City Wall) as impassible
>= 0 means battle_unit_idx






Tactical_Combat__WIP()
    |-> CMB_BaseAllocs__WIP()
    |-> CMB_LoadResources__WIP()
        |-> CMB_SetNearAllocs__WIP()

CMB_BaseAllocs__WIP()
    CMB_ActiveMoveMap = Near_Allocate_First(504);
    CMB_Path_Costs = Near_Allocate_Next(504);
    CMB_NearBuffer_3 = Near_Allocate_Next(1008);
    CMB_Path_Xs = Near_Allocate_Next(504);
    CMB_Path_Ys = Near_Allocate_Next(504);

CMB_SetNearAllocs__WIP()
    CMB_ActiveMoveMap = Near_Allocate_First(504);
    CMB_Path_Costs = Near_Allocate_Next(504);
    CMB_NearBuffer_3 = Near_Allocate_Next(1008);
    CMB_Path_Xs = Near_Allocate_Next(60);
    CMB_Path_Ys = Near_Allocate_Next(60);
    for(itr = 0; itr < 22; itr++)
        CMB_TargetRows[itr] = (int8_t *)Near_Allocate_Next(21);





Tactical_Combat__WIP()
    CMB_TargetFrame_X = Get_Combat_Grid_Cell_X((Grid_X + 4), (Grid_Y + 4));
    CMB_TargetFrame_Y = Get_Combat_Grid_Cell_Y((Grid_X + 4), (Grid_Y + 4));
    |-> BU_CombatAction__WIP(_active_battle_unit, CMB_TargetFrame_X, CMB_TargetFrame_Y);
        |-> BU_Move__WIP()




movement_mode_cost_maps[2400][6]

Move_Stack_DirKey()    |-> Move_Stack()    |-> Move_Units() |-> Move_Units_Draw()

Set_Active_Stack_Movement_Path()
    sets OVL_Path_Length from STK_GetPath()

movepath_cost_map[]
    Overland_Pathfinder()
    updated by Update_MovePathMap()  ¿ identifies and marks *impassible* ?

Init_MovePathMap()
    copies from movement_mode_cost_maps[] to movepath_cost_map

// WZD o95p01
Move_Stack(int16_t player_idx, int16_t destination_x, int16_t destination_y, int16 Spec, int16_t map_x, int16_t map_y, int16_t map_p, int16_t unit_array_count, int16_t unit_array[])
(only) bails on 0 movement points or 0 path length

Move_Stack()
    passes the offset of OVL_Path_Costs to STK_GetPath()
    passes the offset of OVL_Path_Costs to STK_EvaluatePath()

Set_Active_Stack_Movement_Path
    passes the offset of OVL_Path_Costs to STK_GetPath()

UNIT_MoveStack
    passes the offset of OVL_Path_Costs to OVL_StoreLongPath()

Move_Stack()
    has two blocks assigning values to OVL_Path_Costs
        both are iterationg and setting its value to TILE_GetRoadBldTime()
    ¿ these are in a separate major branch from passing the offset to STK_GetPath() ? ¿ and STK_EvaluatePath() ?


¿ OVL_Path_Costs is 119 and  IDK_MovePath_DestinationX,Y are 1 + 119 ?

STK_GetPath()
    passed offset of OVL_Path_Costs RCs
        memcpy() from TBL_OvlMovePathsEMS@  Move_Path.Costs
        ¿ OR ?
        RCs[itr_path_length] = movepath_cost_map[((y * WORLD_WIDTH) + x)]

movepath_cost_map[] is created by Overland_Pathfinder()

movepath_cost_map[] is updated by Update_MovePathMap()  ¿ identifies and marks *impassible* ?

Init_MovePathMap()
    copies from movement_mode_cost_maps[] to movepath_cost_map









void CMB_GetPath__WIP(int16_t source_cgx, int16_t source_cgy, int16_t target_cgx, int16_t target_cgy)

    CMB_Path_Length = 0;

    if(CMB_ActiveMoveMap[((target_cgy * 21) + target_cgx)] == -1)  /* impassible */
        return;

    for(itr = 0; itr < 462; itr++)
        CMB_NearBuffer_3[itr] = itr;

    for(itr = 0; itr < 462; itr++)
        CMB_Path_Costs[itr] = -1;



Unit_Moce.C  movepath_cost_map->moves2
~== 
CMB_Path_Costs




## BU_GetMoveMap__WIP()
~== UnitMove.C  Init_MovePathMap()



## Combat_Move_Path_Valid

populates CMB_Path_Costs[]

What is in CMB_ActiveMoveMap[] at this point?
...BU_GetMoveMap__WIP() set CMB_ActiveMoveMap[] to the movement costs per the movement mode(s)
...then, it got manually updated for other battle units, magic vortices, and combat structure


sets CMB_NearBuffer_3[itr] = itr
uses it to stash old_next_cell_index
checks old_next_cell_index to see if CMB_NearBuffer_3[ctr] was changed to adjacent_idx

sets CMB_NearBuffer_3[] to 1 for every cell that can be reached
...first sets them all to 0
so, ...
    CMB_NearBuffer_3[] is {F,T} - unreachable, reachable
AKA 


¿ sames as movepath_cost_map->Reach_From[itr] = itr; in MovePath.C  Move_Path_Find() ?
CMB_NearBuffer_3[itr] = itr;
¿ sames as movepath_cost_map->Reach_Costs[itr] = 255; in MovePath.C  Move_Path_Find() ?
CMB_Path_Costs[itr] = 255;

void Combat_Move_Path_Valid(int16_t cgx, int16_t cgy, int16_t moves2)

void Move_Path_Find(int16_t arg_wx, int16_t arg_wy, struct s_MOVE_PATH * arg_movepath_cost_map)
    movepath_cost_map = arg_movepath_cost_map;
    for(itr = 0; itr < WORLD_SIZE; itr++)
        movepath_cost_map->Reach_Costs[itr] = 255;
    for(itr = 0; itr < WORLD_SIZE; itr++)
        movepath_cost_map->Reach_From[itr] = itr;
    movepath_cost_map->Reach_Costs[((wy * WORLD_WIDTH) + wx)] = 0;



## CMB_ActiveMoveMap

~== Overland movepath_cost_map->moves2[]

"Terrain Movement Points Per Square"



504 byte array of ...
-1 means impassible  (same as in Overland's ¿...?)

allocated in CMB_BaseAllocs__WIP()
immediately reallocated in CMB_SetNearAllocs__WIP()

Assign_Combat_Grids()
    BU_GetMoveMap__WIP(_active_battle_unit);
    for(itr = 0; itr < _combat_total_unit_count; itr++)
        if(battle_units[itr].Status == bus_Active)
            CMB_ActiveMoveMap[((battle_units[itr].cgy * 21) + battle_units[itr].cgx)] = -1;
    for(itr= 0; itr < CMB_Vortex_Count; itr++)
        CMB_ActiveMoveMap[((CMB_Vortex_Array[itr].cgy * 21) + CMB_Vortex_Array[itr].cgx)] = -1;
    if(battlefield->Central_Structure!= CS_None)
        CMB_ActiveMoveMap[((11 * 21) + 6)] = -1;

...after BU_GetMoveMap__WIP(), is populated with movement costs
...also updates CMB_TargetRows[]
...uses CMB_NearBuffer_3[]


Combat_Move_Path_Valid()



BU_GetMoveMap__WIP()
    |-> Battle_Unit_Movement_Icon()
    |-> BU_GetInstaMoveType__WIP()
...
    BU_Move__WIP()
        |-> BU_GetMoveMap__WIP()
    Assign_Combat_Grids()
        |-> BU_GetMoveMap__WIP()

~ Combat Movement Modes
0
1   Default, Forester, Mountaineer  ¿ ∴ Ground / Walking ?
2   Flying
3
4   Sailing
5   Swimming
6   Teleporting
7   Merging

        case 0:
        case 1:
            memcpy(CMB_ActiveMoveMap, &battlefield->MoveCost_Ground[1], 462);
        case 2:
            memcpy(CMB_ActiveMoveMap, &battlefield->MoveCost_Teleport[1], 462);
        case 3:
            memcpy(CMB_ActiveMoveMap, &battlefield->MoveCost_Ground2[1], 462);
        case 4:
            memcpy(CMB_ActiveMoveMap, &battlefield->MoveCost_Sailing[1], 462);
        case 5:
            for(itr_y = 0; itr_y < 22; itr_y++)
                for(itr_x = 0; itr_x < 21; itr_x++)
                    if(battlefield->MoveCost_Sailing[((itr_y * 21) + itr_x)] > battlefield->MoveCost_Ground[((itr_y * 21) + itr_x)])
                        CMB_ActiveMoveMap[((itr_y * 21) + itr_x)] = battlefield->MoveCost_Ground[((itr_y * 21) + itr_x)];
                    else
                        CMB_ActiveMoveMap[((itr_y * 21) + itr_x)] = battlefield->MoveCost_Sailing[((itr_y * 21) + itr_x)];
        case 6:
        case 7:
            memcpy(CMB_ActiveMoveMap, &battlefield->MoveCost_Teleport[1], 462);


BU_Move__WIP()
AI_GetCombatRallyPt()   goes through the move-path process to get the next cgx,cgy on its way to its target
Do_Auto_Unit_Turn()     sets other units and vortexes as impassible, checks if target is impassible
G_AI_BU_Move()
BU_SetCityMovement()    sets city area as impassible
AI_RestrictToCity()     sets city area perimeter as impassible
CMB_GetPath__WIP()


XREF:  (50)
CMB_BaseAllocs__WIP+81       mov     [CMB_ActiveMoveMap], ax
CMB_SetNearAllocs__WIP+E     mov     [CMB_ActiveMoveMap], ax

BU_GetMoveMap__WIP+48        push    [CMB_ActiveMoveMap]
BU_GetMoveMap__WIP+DA        add     ax, [CMB_ActiveMoveMap]
BU_Move__WIP+66              add     ax, [CMB_ActiveMoveMap]
BU_Move__WIP+A2              add     ax, [CMB_ActiveMoveMap]
BU_Move__WIP+BE              mov     bx, [CMB_ActiveMoveMap]
Assign_Combat_Grids+58   add     ax, [CMB_ActiveMoveMap]
Assign_Combat_Grids+9E   add     ax, [CMB_ActiveMoveMap]
Assign_Combat_Grids+BF   mov     bx, [CMB_ActiveMoveMap]
AI_GetCombatRallyPt+AC       add     ax, [CMB_ActiveMoveMap]
AI_GetCombatRallyPt+F4       add     ax, [CMB_ActiveMoveMap]
AI_GetCombatRallyPt+133      add     ax, [CMB_ActiveMoveMap]
Do_Auto_Unit_Turn+58     add     ax, [CMB_ActiveMoveMap]
Do_Auto_Unit_Turn+E3     add     ax, [CMB_ActiveMoveMap]
Do_Auto_Unit_Turn+134    add     ax, [CMB_ActiveMoveMap]
G_AI_BU_Move+74              add     ax, [CMB_ActiveMoveMap]
G_AI_BU_Move+109             add     ax, [CMB_ActiveMoveMap]
BU_SetCityMovement+1D        mov     bx, [CMB_ActiveMoveMap]
BU_SetCityMovement+32        mov     bx, [CMB_ActiveMoveMap]
BU_SetCityMovement+47        mov     bx, [CMB_ActiveMoveMap]
BU_SetCityMovement+5C        mov     bx, [CMB_ActiveMoveMap]
BU_SetCityMovement+71        mov     bx, [CMB_ActiveMoveMap]
BU_SetCityMovement+160       add     ax, [CMB_ActiveMoveMap]
BU_SetCityMovement+16E       mov     bx, [CMB_ActiveMoveMap]
BU_SetCityMovement+17C       mov     bx, [CMB_ActiveMoveMap]
BU_SetCityMovement:loc_A789D mov     bx, [CMB_ActiveMoveMap]
BU_SetCityMovement+1A1       add     ax, [CMB_ActiveMoveMap]
BU_SetCityMovement+1AF       mov     bx, [CMB_ActiveMoveMap]
BU_SetCityMovement+1BD       mov     bx, [CMB_ActiveMoveMap]
BU_SetCityMovement+1CB       mov     bx, [CMB_ActiveMoveMap]
BU_SetCityMovement+23A       add     ax, [CMB_ActiveMoveMap]
BU_SetCityMovement+2BD       add     ax, [CMB_ActiveMoveMap]
AI_RestrictToCity+1C         add     ax, [CMB_ActiveMoveMap]
AI_RestrictToCity+2A         mov     bx, [CMB_ActiveMoveMap]
AI_RestrictToCity+38         mov     bx, [CMB_ActiveMoveMap]
AI_RestrictToCity+45         mov     bx, [CMB_ActiveMoveMap]
AI_RestrictToCity+5D         add     ax, [CMB_ActiveMoveMap]
AI_RestrictToCity+6B         mov     bx, [CMB_ActiveMoveMap]
AI_RestrictToCity+79         mov     bx, [CMB_ActiveMoveMap]
AI_RestrictToCity+87         mov     bx, [CMB_ActiveMoveMap]
CMB_GetPath__WIP+19          add     ax, [CMB_ActiveMoveMap]
CMB_GetPath__WIP:loc_E2A6A   mov     bx, [CMB_ActiveMoveMap]
CMB_GetPath__WIP:loc_E2B87   mov     bx, [CMB_ActiveMoveMap]
CMB_GetPath__WIP:loc_E2CAA   mov     bx, [CMB_ActiveMoveMap]
CMB_GetPath__WIP:loc_E2DD6   mov     ax, [CMB_ActiveMoveMap]
CMB_FillReachMap:loc_E2EFE   mov     bx, [CMB_ActiveMoveMap]
CMB_FillReachMap:loc_E2FA1   mov     bx, [CMB_ActiveMoveMap]
CMB_FillReachMap:loc_E304A   mov     bx, [CMB_ActiveMoveMap]
CMB_FillReachMap+2B1         mov     bx, [CMB_ActiveMoveMap]
