
OVL_Action_Type  ==>  _combat_environ
OVL_Action_Structure  ==> _combat_environ_idx
CMB_combat_structure  ==>  _combat_structure
OVL_Action_Plane  ==>  _combat_wp
OVL_Action_YPos  ==>  _combat_wy
OVL_Action_XPos  ==>  _combat_wx
BU_GetEffectiveDEF()  ==>  Battle_Unit_Defense_Special()
CMB_HumanUnitsDone  ==>  _human_out_of_moves
CMB_ImmobileCanAct  ==>  _human_handle_immobile



Variables?
Raison d'etre?
CMB_HumanTurn
CMB_AIGoesFirst
_human_handle_immobile
CRP_CMB_NeverChecked1

## CMB_AIGoesFirst
In Combat_Screen__WIP(), ...
    CMB_AIGoesFirst = ST_FALSE;
    if(_combat_defender_player == combat_computer_player)
        AI_CMB_PlayTurn__WIP(_combat_defender_player);
        CMB_PrepareTurn__WIP();
        CMB_AIGoesFirst = ST_TRUE;
...
    ...nothing in AI_CMB_PlayTurn__WIP()
    ...nothing in AI_CMB_PlayTurn__WIP() |-> CMB_CE_Refresh__WIP()
    ...nothing in AI_CMB_PlayTurn__WIP() |-> AI_MoveBattleUnits__WIP()
    CMB_PrepareTurn__WIP()
        CMB_AIGoesFirst = ST_FALSE;

initialized to false in Combat_Screen__WIP()
...promptly set to false in CMB_PrepareTurn__WIP() and set to true right after
controls if CMB_ProgressTurnFlow__WIP() calls AI_CMB_PlayTurn__WIP(combat_computer_player);
...then same logic of 'if computer is defender' to call AI_CMB_PlayTurn__WIP(_combat_defender_player) and set CMB_AIGoesFirst to true



MoO2
_combat_turn is incremented at the end of the screen-loop in Tactical_Combat_(), right be calling End_Of_Turn_Bookeeping_()

End_Of_Turn_Bookeeping_()
...Recharge_Shields_(), Repair_All_Combat_Ships_(), if not retreat, 
...
vs. CMB_PrepareTurn__WIP()
...some parts feel like 'begin', some parts feel like 'end'






## CMB_HumanTurn

initialized to true in Combat_Screen__WIP()

sets to false in CMB_ProgressTurnFlow__WIP()
...then, set to true after the computer player turn

...controls if "All units are immobilized. Select an action." is printed

¿ could be side 0 or side 1 ?  ...left/right?  ...attacker/defender?



## _human_out_of_moves && _human_handle_immobile

Something like...
    _human_out_of_moves means no units left to move/attack
    _human_handle_immobile means no units immobilized
    ((_human_out_of_moves == ST_TRUE) && (_human_handle_immobile == ST_FALSE)) means the human player's turn is definitely over
if _human_out_of_moves isn't true, then _human_handle_immobile shouldn't matter?
    ...as evidence by rge either or in Next_Battle_Unit()


    Combat_Screen__WIP()
        if(battle_units[_active_battle_unit].controller_idx != combat_human_player)
            _human_out_of_moves = ST_TRUE;  // human turn is over
            _human_handle_immobile = ST_FALSE;  // don't draw target frames or all immobilized message

        if(
            (leave_screen == ST_FALSE)
            &&
            (_human_out_of_moves == ST_TRUE)  // human player's turn is over
            &&
            (_human_handle_immobile == ST_FALSE)  // 
        )
        {
            _human_out_of_moves = ST_FALSE;  // Where does this get used after this?
            CMB_ProgressTurnFlow__WIP();
            Next_Battle_Unit(_human_player_idx);    // maybe, sets _human_out_of_moves = ST_TRUE and/or _human_handle_immobile = ST_FALSE
            Assign_Combat_Grids();
            ...
        }



## _human_handle_immobile

¿ 'End Of Turn' ... ~ MoO2  _side_end_of_turn ?
    Next_Battle_Unit() sets false if any units are available for move/action
    So, ...
        means NOT end of turn?
    But, ...
        set back to true in CMB_PrepareTurn__WIP()



defaults to false, but set to true in CMB_PrepareTurn__WIP()

if _human_out_of_moves is false, then _human_handle_immobile is false
_human_handle_immobile can only be true, if _human_out_of_moves is true


Next_Battle_Unit()
...this gets set to ST_FALSE if another unit was found, instead of setting _human_out_of_moves to ST_TRUE to make the end of turn

...gets used to override _human_out_of_moves, in the Non-Auto Combat screen-loop block

...controls whether the grid cell box/"frame" is turned off, even when there's an active battle unit

...controls "All units are immobilized. Select an action."
...if true, not auto, active unit, is human unit and human turn

Assign_Mouse_Images()
    turn off active unit combat grid highlight frame
        ...if active battle unit, but immobile flag, not auto-combat, and human controller
    mouse image(s) when it's not the human player's turn?


...sets to ST_TRUE
    CMB_PrepareTurn__WIP+7       mov     [_human_handle_immobile], e_ST_TRUE 

...sets to ST_FALSE
    Combat_Screen__WIP:loc_76080 mov     [_human_handle_immobile], e_ST_FALSE
        ...initialization
    Combat_Screen__WIP+27E       mov     [_human_handle_immobile], e_ST_FALSE
        ...reinitialized after call to j_CMB_PrepareTurn__WIP()
    Combat_Screen__WIP+3C9       mov     [_human_handle_immobile], e_ST_FALSE
        ...reinitialized after cancel auto-combat
    Combat_Screen__WIP:loc_766E9 mov     [_human_handle_immobile], e_ST_FALSE
        ...IDGI - on left-click flee button
    Combat_Screen__WIP:loc_767AA mov     [_human_handle_immobile], e_ST_FALSE
        ...IDGI - on left-click combat grid
    Combat_Screen__WIP+E22       mov     [_human_handle_immobile], e_ST_FALSE
        ...IDGI - after spell button, but only for cast_status == 2
    Combat_Screen__WIP+E8E       mov     [_human_handle_immobile], e_ST_FALSE
        ...IDGI - on left-click wait button
    Combat_Screen__WIP+ECA       mov     [_human_handle_immobile], e_ST_FALSE
    Combat_Screen__WIP:loc_76DD7 mov     [_human_handle_immobile], e_ST_FALSE
        ...IDGI - on left-click info button
    Combat_Screen__WIP+1160      mov     [_human_handle_immobile], e_ST_FALSE
        ...IDGI - on left-click done button / space hotkey
    Next_Battle_Unit:loc_84BA5   mov     [_human_handle_immobile], e_ST_FALSE
        ...if all units are 'done'
    Next_Battle_Unit+6D          mov     [_human_handle_immobile], e_ST_FALSE
        bogus?

XREF:
    Combat_Screen__WIP:loc_76080 mov     [_human_handle_immobile], e_ST_FALSE
    Combat_Screen__WIP+27E       mov     [_human_handle_immobile], e_ST_FALSE
    Combat_Screen__WIP+3C9       mov     [_human_handle_immobile], e_ST_FALSE
    Combat_Screen__WIP:loc_766E9 mov     [_human_handle_immobile], e_ST_FALSE
    Combat_Screen__WIP:loc_767AA mov     [_human_handle_immobile], e_ST_FALSE
    Combat_Screen__WIP+E22       mov     [_human_handle_immobile], e_ST_FALSE
    Combat_Screen__WIP+E8E       mov     [_human_handle_immobile], e_ST_FALSE
    Combat_Screen__WIP+ECA       mov     [_human_handle_immobile], e_ST_FALSE
    Combat_Screen__WIP:loc_76DD7 mov     [_human_handle_immobile], e_ST_FALSE
    Combat_Screen__WIP+1160      mov     [_human_handle_immobile], e_ST_FALSE
    Combat_Screen__WIP+1173      cmp     [_human_handle_immobile], e_ST_FALSE
    CMB_PrepareTurn__WIP+7       mov     [_human_handle_immobile], e_ST_TRUE 
    Assign_Mouse_Images+4B       cmp     [_human_handle_immobile], e_ST_TRUE 
    Assign_Mouse_Images+216      cmp     [_human_handle_immobile], e_ST_TRUE 
    Combat_Screen_Draw+4BB       cmp     [_human_handle_immobile], e_ST_TRUE 
    Next_Battle_Unit:loc_84BA5   mov     [_human_handle_immobile], e_ST_FALSE
    Next_Battle_Unit+6D          mov     [_human_handle_immobile], e_ST_FALSE




## _human_out_of_moves


¿ ***when _human_out_of_moves is TRUE, the human player's turn is over*** ?
...not quite, but definitely means all battle units have moved or attacked
...what's left? webbed but can cast spell?
...something about disenchanting wind walking during combat over water
...so, more like 'active(current/last?) unit can not move'?


Is there something here with the current/human player?
Does it matter that it is the 'Human Player'?
combat_human_player = _human_player_idx = HUMAN_PLAYER_IDX = 0

What's the relationship with _human_handle_immobile?


Where is the first place it gets used?  (starts from Combat_Screen__WIP())


...sets to ST_TRUE
    Combat_Screen__WIP+115A      mov     [_human_out_of_moves], e_ST_TRUE                                                  
    Next_Battle_Unit+23          mov     [_human_out_of_moves], e_ST_TRUE                                                  
...
    Combat_Screen__WIP()
        if(battle_units[_active_battle_unit].controller_idx != combat_human_player)
            _human_out_of_moves = ST_TRUE;
            _human_handle_immobile = ST_FALSE;
    ...but, I have a debug-break in there and it has never been hit
    Next_Battle_Unit()
        all_done_none_available = Next_Battle_Unit_Nearest_Available(player_idx);
        if(all_done_none_available == ST_TRUE)
            _human_out_of_moves = ST_TRUE;
        else
            _human_handle_immobile = ST_FALSE;

...sets to ST_FALSE
    Combat_Screen__WIP+12C       mov     [_human_out_of_moves], e_ST_FALSE
        ...just initializing it to FALSE  (right before _combat_turn is initialized to zero)
    Combat_Screen__WIP:loc_761A1 mov     [_human_out_of_moves], e_ST_FALSE; BUG: second time clearing this without using it
        ...just initializing it to FALSE  (right before setting _active_battle_unit)
    Combat_Screen__WIP+5EB       mov     [_human_out_of_moves], e_ST_FALSE
        ...in 'Cancel Auto Combat', before calling AI_CMB_PlayTurn__WIP(combat_human_player)
    Combat_Screen__WIP+117A      mov     [_human_out_of_moves], e_ST_FALSE
        ...in the block for Non-Auto Combat
        ...subsequent usage?
        ...Eh? ...only happens if it's TRUE, so how/why would it be true here/there?
        ...backwards? ...when we get here, it's only true if it's been set by Next_Battle_Unit()

XREF:
    Combat_Screen__WIP+12C       mov     [_human_out_of_moves], e_ST_FALSE                                                 
    Combat_Screen__WIP:loc_761A1 mov     [_human_out_of_moves], e_ST_FALSE; BUG: second time clearing this without using it
    Combat_Screen__WIP+5EB       mov     [_human_out_of_moves], e_ST_FALSE                                                 
    Combat_Screen__WIP+F68       cmp     [_human_out_of_moves], e_ST_TRUE                                                  
    Combat_Screen__WIP+115A      mov     [_human_out_of_moves], e_ST_TRUE                                                  
    Combat_Screen__WIP+116C      cmp     [_human_out_of_moves], e_ST_TRUE                                                  
    Combat_Screen__WIP+117A      mov     [_human_out_of_moves], e_ST_FALSE                                                 
    Next_Battle_Unit+23          mov     [_human_out_of_moves], e_ST_TRUE                                                  


## _combat_turn

incremented in CMB_PrepareTurn__WIP()
...which gets called early in Combat_Screen__WIP()
...and a second time if defender is computer



In MoO2, ...
    _combat_turn++;
    End_Of_Turn_Bookeeping_();

XREF:
    Auto_Move_Ship_:loc_2AA10    cmp _combat_turn, 1
    Check_For_Winner_+68         cmp _combat_turn, 50
    Combat_Ship_Class_:loc_2A633 cmp _combat_turn, 1
    End_Of_Turn_Bookeeping_+17   movsx eax, _combat_turn
    Strategic_Combat_+312        cmp _combat_turn, 50
    Strategic_Combat_+320        inc _combat_turn
    Strategic_Combat_+39C        cmp _combat_turn, 4
    Strategic_Combat_+3F5        cmp _combat_turn, 2
    Strategic_Combat_+417        cmp _combat_turn, 2
    Strategic_Combat_+73         mov _combat_turn, 0
    Strategic_Combat_:loc_40579  cmp _combat_turn, 3
    Strategic_Combat_:loc_40612  cmp _combat_turn, 50
    Tactical_Combat_+55          mov _combat_turn, 0
    Tactical_Combat_:loc_48D11   inc _combat_turn

Auto_Move_Ship_()

Check_For_Winner_()
    if(_combat_turn > 50)

Combat_Ship_Class_()

End_Of_Turn_Bookeeping_()

Strategic_Combat_()

Tactical_Combat_()
    initializes _combat_turn to 0
    increments _combat_turn










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
        if leave_screen == ST_FALSE && _human_out_of_moves == ST_TRUE && _human_handle_immobile == ST_FALSE
            _human_out_of_moves = ST_FALSE
            j_CMB_ProgressTurnFlow__WIP()
            j_Next_Battle_Unit(_human_player_idx)





## Next_Battle_Unit_Nearest_Available()

Next_Battle_Unit_Nearest_Available()
OON XREF:  Next_Battle_Unit()

UNITSTK.C Next_Unit_Nearest_Available()
OON XREF:  WIZ_NextIdleStack()

_human_out_of_moves
_human_handle_immobile
in Next_Battle_Unit()
    if(all_done_none_available == ST_TRUE)
        _human_out_of_moves = ST_TRUE;
    else
        _human_handle_immobile = ST_FALSE;




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
