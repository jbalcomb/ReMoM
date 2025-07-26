
OVL_Action_Type  ==>  _combat_environ
OVL_Action_Structure  ==> _combat_environ_idx
CMB_combat_structure  ==>  _combat_structure
OVL_Action_Plane  ==>  _combat_wp
OVL_Action_YPos  ==>  _combat_wy
OVL_Action_XPos  ==>  _combat_wx
BU_GetEffectiveDEF()  ==>  Battle_Unit_Defense_Special()



Wall Crushers
    Catapult
    Engineers
    War Mammoths
    Colossus
    Earth Elemental
    Fire Giant
    Stone Giant
    Storm Giants



## _combat_environ


## _combat_environ_idx


## _combat_structure
    set by Combat_Structure(wx,wy,wp)
    only called once each by tactical combat and strategic combat
    looks like what was described in the manual and OSG





¿ "entity" vs. unit/object/combat_grid_datum ?
unit level
figure level
unit vs. rock vs. tree vs. wall vs. structure



Per BU_IsInCityProper()
city area on combat grid
cgx >=  5 && cgx <=  8
cgy >= 10 && cgy <= 13
cgx: {  5,  6,  7,  8 }
cgy: { 10, 11, 12, 13 }
{
    { 5, 10 }, { 5, 11 }, { 5, 12 }, { 5, 13 },
    { 6, 10 }, { 6, 11 }, { 6, 12 }, { 6, 13 },
    { 7, 10 }, { 7, 11 }, { 7, 12 }, { 7, 13 },
    { 8, 10 }, { 8, 11 }, { 8, 12 }, { 8, 13 }
}



CMB_SelectedUnit ==> _active_battle_unit
G_BU_SelectUnit__WIP() ==> Switch_Active_Battle_Unit()
G_CMB_Auto_OFF__WIP() ==> Turn_Off_Auto_Combat()   ~ MoO2 Toggle_Auto_Combat_Flags_()
WIZ_BU_SelectClosest__WIP() ==> Next_Battle_Unit_Nearest_Available()
WIZ_BU_SelectNext__WIP() ==> Next_Battle_Unit()
CMB_GetPath() ==> CMB_GetPath__WIP()
CMB_GetPath__WIP() ==> Combat_Move_Path_Find()
CMB_FillReachMap() ==> CMB_FillReachMap__WIP()
CMB_FillReachMap__WIP() ==> Combat_Move_Path_Valid()
Adj_Tile_Cost ==> adjacent_path_cost
Cost_As_Origin ==> potential_path_cost
Current_Origin ==> old_next_cell_index
CMB_BU_Figure_GFX  ==>  battle_unit_picts_seg
ptr_figure_pointer_seg  ==>  figure_pict_seg
figure_pointer_seg  ==>  figure_pict_seg
CMB_EntitiesReset()  ==>  Clear_Combat_Grid_Entities()
CMB_GetFigDrawPos__SEGRAX()  ==>  Battle_Unit_Figure_Position()
combat_entity_draw_order_array  ==>  combat_grid_entities_draw_order
BU_GetMoveMap__WIP() ==> Set_Movement_Cost_Map()

battle_unit_figure_idx  ==>  bufpi
bufpi  Battle Unit Figure Picture Index
TODO  rename bufpi to pict_idx or some such, after your're done feeling confused about all the the mix-ups you made



## Active Battle Unit


set in 8 places
    CMB_VortexPlayerMove+63       mov     [_active_battle_unit], -1                  
    End_Of_Combat__WIP:loc_A5B34  mov     [_active_battle_unit], 667                 
    Next_Battle_Unit+AA           mov     [_active_battle_unit], _SI_itr_battle_units
    STK_CaptureCity+372           mov     [_active_battle_unit], ax                  
    CMB_PrepareTurn__WIP+8E       mov     [_active_battle_unit], ax                  
    Switch_Active_Battle_Unit+1A  mov     [_active_battle_unit], ax                  
    End_Of_Combat__WIP+B8F        mov     [_active_battle_unit], ax                  
    AI_BU_ProcessAction+64        mov     [_active_battle_unit], si                  

[x] Switch_Active_Battle_Unit()
[x] Next_Battle_Unit()



Tactical_Combat__WIP()
    if(_combat_attacker_player == _human_player_idx)
        Switch_Active_Battle_Unit(0);
    else
        Switch_Active_Battle_Unit((_combat_total_unit_count - Defending_Unit_Count));
NOTE: there's a call to CMB_PrepareTurn__WIP() immediately preceding


Tactical_Combat__WIP+304
Tactical_Combat__WIP+70A
Tactical_Combat__WIP+775
Tactical_Combat__WIP+EB1
Tactical_Combat__WIP+F5D
Tactical_Combat__WIP+1189
Turn_Off_Auto_Combat+D
    |-> j_Next_Battle_Unit()
        |-> Next_Battle_Unit()


Tactical_Combat__WIP()
    ***cancel auto combat***
        j_CMB_ProgressTurnFlow__WIP()
        j_Turn_Off_Auto_Combat()
            Turn_Off_Auto_Combat()
                j_Next_Battle_Unit(_human_player_idx)
                    Next_Battle_Unit(_human_player_idx)


Next_Battle_Unit()

XREF:
    j_Next_Battle_Unit()
        Next_Battle_Unit()
XREF:
j_Next_Battle_Unit
    Tactical_Combat__WIP+304  call    j_Next_Battle_Unit
    Tactical_Combat__WIP+70A  call    j_Next_Battle_Unit
    Tactical_Combat__WIP+775  call    j_Next_Battle_Unit
    Tactical_Combat__WIP+EB1  call    j_Next_Battle_Unit
    Tactical_Combat__WIP+F5D  call    j_Next_Battle_Unit
    Tactical_Combat__WIP+1189 call    j_Next_Battle_Unit
    Turn_Off_Auto_Combat+D    call    j_Next_Battle_Unit

sets _active_battle_unit, via Next_Battle_Unit_Nearest_Available(player_idx)
but, ...
    if(battle_units[_active_battle_unit].controller_idx != combat_human_player)
        for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
            if(battle_units[itr_battle_units].controller_idx == _combat_attacker_player)
                _active_battle_unit = itr_battle_units;

called for Turn_Off_Auto_Combat(), so doesn't *feel* like "Next"
other six?
Tactical_Combat__WIP()
    Pre Screen Loop
        if(_combat_attacker_player == _human_player_idx)
            j_Next_Battle_Unit(_human_player_idx)
    Screen / Input Loop
        Eh? _active_battle_unit has become unavailable?
            j_Next_Battle_Unit(_human_player_idx)
        _active_battle_unit is *confused*
            j_Next_Battle_Unit(_human_player_idx)
        input_field_idx == wait_button_field
            j_Next_Battle_Unit(_human_player_idx)
        input_field_idx == done_button_field
            j_Next_Battle_Unit(_human_player_idx)
    Closing The Loop
        if leave_screen == ST_FALSE && CMB_HumanUnitsDone == ST_TRUE && CMB_ImmobileCanAct == ST_FALSE
            CMB_HumanUnitsDone = ST_FALSE
            j_CMB_ProgressTurnFlow__WIP()
            j_Next_Battle_Unit(_human_player_idx)





## Next_Battle_Unit_Nearest_Available()

Next_Battle_Unit_Nearest_Available()
OON XREF:  Next_Battle_Unit()

UNITSTK.C Next_Unit_Nearest_Available()
OON XREF:  WIZ_NextIdleStack()

CMB_HumanUnitsDone
CMB_ImmobileCanAct
in Next_Battle_Unit()
    if(all_done_none_available == ST_TRUE)
        CMB_HumanUnitsDone = ST_TRUE;
    else
        CMB_ImmobileCanAct = ST_FALSE;




Move_Units()
    Eval_Move_Path__WIP(..., &attack_flag, ...)
    if((attack_flag == ST_TRUE) || (UU_flag_FALSE == ST_TRUE))
        goto Combat_Handlers;
Combat_Handlers:
    if(OVL_Action_Type == 5)
    else
    combat_result = Combat__WIP(player_idx, defender_idx, troop_count, &troops[0]);
    if(combat_result == 1)
        Player_Army_At_Square(_combat_wx, _combat_wy, _combat_wp, player_idx, &troop_count, troops);



combat_result = Combat__WIP(player_idx, defender_idx, troop_count, &troops[0]);
    if(((combat_attacker_player_idx == _human_player_idx) || (defender_idx == _human_player_idx)) && (magic_set.strategic_combat_only == ST_FALSE))
        Battle_Outcome = Tactical_Combat__WIP(combat_attacker_player_idx, defender_idx, troops, troop_count, _combat_wx, _combat_wy, _combat_wp, &Item_Count, &Item_List[0]);



int16_t Tactical_Combat__WIP(int16_t combat_attacker_player_idx, int16_t combat_defender_player_idx, int16_t troops[], int16_t troop_count, int16_t wx, int16_t wy, int16_t wp, int16_t * item_count, int16_t item_list[])



MoO2
_combat_total_ship_count gets incremented in Tactical_Combat_()
while calling Load_Combat_Ship_(_combat_total_ship_count)
¿ something should be setting data that looks like `battle_units[]` ?
