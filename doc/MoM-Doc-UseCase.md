


I'm sure I already have some notes for this sort of thing elsewhere.



## Initialize Game

## Load Saved Game



## Next Turn

## Select Stack

## Move Stack

## Next Stack

## `_units`, `_unit_stack`, `_unit_stack_count`

## `_map_x`, `_map_y`, `_map_plane`


## `_active_world_x`, `_active_world_y`

First Usage?
Next Usage?
Relationship with 'Move Stack' / 'Select Next Stack'?


Initialized in GAME_Overland_Init()
    _active_world_x = _FORTRESSES[HUMAN_PLAYER_IDX].world_x
    _active_world_y = _FORTRESSES[HUMAN_PLAYER_IDX].world_y

In Main_Screen(), 'Right-Click - Movement Map - Select Stack'
    _unit = input__entity_idx
    unit_idx = _unit
    selected_unit_x = _UNITS[unit_idx].world_x
    selected_unit_y = _UNITS[unit_idx].world_y
    _active_world_x = _UNITS[unit_idx].world_x
    _active_world_y = _UNITS[unit_idx].world_y
    Select_Unit_Stack(_human_player_idx, &_map_x, &_map_y, _map_plane, selected_unit_x, selected_unit_y)
        Build_Unit_Stack(_human_player_idx, _map_plane, selected_unit_x, selected_unit_y)
            checks HMoves & Finished to set active to ST_TRUE
                and if status is 'Wait', changes status to 'No Orders'
            then, checks if status is 'Purify' and sets active to ST_FALSE
            sets _unit_stack[]
            sets _unit_stack_count
        ...
        uses `_unit` to pass unit x,y to OVL_BringIntoView()
        passes empty unit_array to STK_GetMovableUnits()
            STK_GetMovableUnits() uses `_unit_stack` to populate the unit_array
        passes populated unit_array to Units_In_Tower()
            Units_In_Tower() sets In_Tower flag in _UNITS, only for Units in the unit_array


WIZ_NextUnit()
    passes _active_world_x, y to Delta_XY_With_Wrap()
        _active_world_x,y is x1,y1 and _UNITS[itr_units].world_x,y is x2,y2
    sets `_unit`
    sets `_active_world_x` and `_active_world_y`
    ¿ equivalent to logic at/in 'Right-Click - Movement Map - Select Stack' ?


EarthGateTeleport()
    ¿ `_unit` ?
    _active_world_x = _CITIES[IDK_dst_city_idx].world_x
    _active_world_y = _CITIES[IDK_dst_city_idx].world_y
    WIZ_NextIdleStack(Player_Index, &_map_x, &_map_y, &Plane)


ArmyList_Screen()
    ¿ same logic/code as 'Right-Click - Movement Map - Select Stack' ?
    _unit = list_armies[itr]
    _map_plane = _UNITS[_unit].world_plane
    Select_Unit_Stack(_human_player_idx, &_map_x, &_map_y, _map_plane, _UNITS[_unit].world_x, _UNITS[_unit].world_y)
    _active_world_x = _UNITS[_unit].world_x
    _active_world_y = _UNITS[_unit].world_y





explored
    GAME_Overland_Init();  Main_Screen();  WIZ_NextUnit();  EarthGateTeleport();
partial
    EarthGateTeleport();  ArmyList_Screen();  
unexplored
    Outpost_Screen();  sub_B4250();  sub_B4471();  sub_B4E00();  G_OVL_Cast();  EVNT_RandomOffers();


GAME_Overland_Init+11B mov     [_active_world_x], ax                   
Main_Screen+FB4        mov     [_active_world_x], ax                   
WIZ_NextUnit+DB        push    [_active_world_x]               ; x1    
WIZ_NextUnit+1BF       mov     [_active_world_x], ax                   
WIZ_NextUnit+251       mov     [_active_world_x], ax                   
EarthGateTeleport+2D4  mov     [_active_world_x], ax                   
ArmyList_Screen+3C2    mov     [_active_world_x], ax                   
Outpost_Screen+2FE     mov     [_active_world_x], ax                   
Outpost_Screen+31B     push    [_active_world_x]               ; Unit_X
sub_B4250+15C          mov     [_active_world_x], ax                   
sub_B4471+11D          mov     [_active_world_x], ax                   
sub_B4E00+21E          mov     [_active_world_x], ax                   
G_OVL_Cast+407         mov     [_active_world_x], ax                   
EVNT_RandomOffers+2FA  mov     [_active_world_x], ax                   
