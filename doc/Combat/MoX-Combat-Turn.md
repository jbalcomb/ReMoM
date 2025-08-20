


Starts from?
Attacker is {HP,CP|NP}
Defender is {HP,CP|NP}
{HP,CP}
{CP,HP}

Human attacks Computer
    Combat_Screen__WIP()
    CMB_AIGoesFirst = ST_FALSE;
    if(_combat_defender_player == combat_computer_player)
        AI_CMB_PlayTurn__WIP(_combat_defender_player);
        CMB_PrepareTurn__WIP();
        CMB_AIGoesFirst = ST_TRUE;
    if(_combat_attacker_player == _human_player_idx)
        Next_Battle_Unit(0);  /* first attacker battle_unit_idx */

Computer attacks Human
    Combat_Screen__WIP()
    CMB_AIGoesFirst = ST_FALSE;
    NOT if(_combat_defender_player == combat_computer_player)
    _human_out_of_moves = ST_FALSE;
    NOT if(_combat_attacker_player == _human_player_idx)
        Switch_Active_Battle_Unit((_combat_total_unit_count - defender_unit_count));  /* first defender battle_unit_idx */





## CMB_ProgressTurnFlow__WIP()

called twice in 'Auto Combat'
OR
once right before screen redraw in screen-loop

    if(battle_units[_active_battle_unit].controller_idx != combat_human_player)
        _human_out_of_moves = ST_TRUE;
        _human_handle_immobile = ST_FALSE;

    if((leave_screen == ST_FALSE) && (_human_out_of_moves == ST_TRUE) && (_human_handle_immobile == ST_FALSE))
        _human_out_of_moves = ST_FALSE;  // Where does this get used after this?
        CMB_ProgressTurnFlow__WIP();
        Next_Battle_Unit(_human_player_idx);
        Assign_Combat_Grids();
        input_field_idx = ST_UNDEFINED;
        screen_changed = ST_TRUE;
        CRP_CMB_NeverChecked1 = ST_TRUE;
        winner = Check_For_Winner__WIP();
        if(Combat_Winner != ST_UNDEFINED)
            leave_screen = ST_UNDEFINED;
            input_field_idx = 0;



XREF:
    j_CMB_ProgressTurnFlow__WIP()
        Combat_Screen__WIP+3B9       call    j_CMB_ProgressTurnFlow__WIP
        Combat_Screen__WIP:loc_76281 call    j_CMB_ProgressTurnFlow__WIP
        Combat_Screen__WIP+1180      call    j_CMB_ProgressTurnFlow__WIP




## CMB_PrepareTurn__WIP()

~ 'Combat Turn Update' ?
    ...battlefield bonuses is more begin-turn?
    ...city damage is more end-turn?

encompasses MoO2's End_Of_Turn_Bookeeping_()

NOTE  doesn't touch CMB_HumanTurn

reinitializes
    _human_handle_immobile = ST_TRUE
    CMB_WizCastAvailable = ST_TRUE
    _scanned_battle_unit = ST_UNDEFINED
    CMB_AIGoesFirst = ST_FALSE
increments
    AI_ImmobileCounter
    _combat_turn











OSG
Page 252  (PDG Page 259)
Table 17.2 Bonuses to Unit Attributes and Abilities
Effects of Structures on Combat Statistics
Combat Variations: Flying, Water, Walls, and More















Move_Battle_Unit__WIP() ==> Battle_Unit_Move__WIP()
BU_CombatAction__WIP() ==> Battle_Unit_Action__WIP()
... ==> Battle_Unit_Attack__WIP()
CMB_WinLoseFlee__WIP() ==> Check_For_Winner__WIP()
BU_GetHalfMoves__WIP() == Battle_Unit_Moves2()



MoO2
mov     _ai_retreat_flag, ax            ; _ai_retreat_flag = IDK_retreat_flag; Russ_Combat() ai_can_retreat_flag
_ai_retreat_flag is {F,T} for hether the AI is allowed to retreat, checked in Retreat_Check_(), from Get_Player_Mode_()



How does a 'Combat Turn' happen?

Battle_Unit_Action__WIP()

CMB_ProgressTurnFlow__WIP() does the turn for the computer player and the auto combat for the human player, but still has nothing for an 'end of turn' indicator.
...calls AI_CMB_PlayTurn__WIP()

At the end of Tactical_Combat__WIP()
    if((leave_screen == ST_FALSE) && (_human_out_of_moves == ST_TRUE) && (_human_handle_immobile == ST_FALSE))
        CMB_ProgressTurnFlow__WIP()
            CMB_PrepareTurn__WIP()
                Battle_Unit_Moves2()


...after moving, didn't hit Next_Battle_Unit() and it still thinks the moved unit is the active unit



battle_units[itr].action = bua_Finished;
    Tactical_Combat__WIP()
        ...right after 'Auto Combat', before balance of input matching...
        if(((battle_units[_active_battle_unit].movement_points < 1) && (battle_units[_active_battle_unit].action != bua_Finished)) || (battle_units[_active_battle_unit].status > bus_Active))
    'Left-Click Done Button'
    CMB_PrepareTurn__WIP()
        resist_fails = Combat_Resistance_Check()
        if(resist_fails > 0)

...
no movement points triggers setting all_done_none_available
all_done_none_available triggers setting _human_out_of_moves
does not hit `if(battle_units[_active_battle_unit].controller_idx != combat_human_player)`
hits Next_Battle_Unit() again
Hrrrmm..._active_battle_unit didn't get its movement_points used up
...actually did though in Move_Battle_Unit__WIP()
*shrug*
did trigger on the step-through
so, ...
    if(
        (leave_screen == ST_FALSE)
        &&
        (_human_out_of_moves == ST_TRUE)
        &&
        (_human_handle_immobile == ST_FALSE)
    )
    {

        _human_out_of_moves = ST_TRUE;

        CMB_ProgressTurnFlow__WIP();
...got there
this is where the AI turn should happen
so what follows is assuming that it's the human players turn again
heh.
blows through 50 turns before I can move again
CMB_PrepareTurn__WIP() increments _combat_turn
so, it's getting called when it shouldn't?
...called by CMB_ProgressTurnFlow__WIP()



`CMB_ProgressTurnFlow__WIP()` |-> `CMB_PrepareTurn__WIP()` |-> `Battle_Unit_Moves2()`
¿ order in the code ?
    Tactical_Combat__WIP()
        ...
        _human_out_of_moves = ST_FALSE;
        ...
        _human_handle_immobile = ST_FALSE;
        G_AI_StayInTownProper = ST_TRUE;
        ...
        Switch_Active_Battle_Unit();
        ...
        CMB_HumanTurn = ST_TRUE;
        _auto_combat_flag = ST_FALSE;
        ...
        *** CMB_PrepareTurn__WIP(); ***  |-> 
        _human_handle_immobile = ST_FALSE;
        Switch_Active_Battle_Unit();
        ...
        CMB_AIGoesFirst = ST_FALSE;
        if(_combat_defender_player == combat_computer_player)
            AI_CMB_PlayTurn__WIP(_combat_defender_player);
            CMB_PrepareTurn__WIP();
            CMB_AIGoesFirst = ST_TRUE;
        Combat_Winner = Check_For_Winner__WIP();
        _human_out_of_moves = ST_FALSE;
        Next_Battle_Unit()
        ...
        while(leave_screen == ST_FALSE)

'Left-Click Done Button'
    battle_units[_active_battle_unit].action = bua_Finished;
    battle_units[_active_battle_unit].movement_points = 0;
    Next_Battle_Unit(_human_player_idx);




## 'Auto Combat'
```c
    CMB_ProgressTurnFlow__WIP();
    Combat_Winner = Check_For_Winner__WIP();
```



## _human_out_of_moves

set to ST_TRUE in Tactical_Combat__WIP()
    if(battle_units[_active_battle_unit].controller_idx != combat_human_player)
set to ST_TRUE in Next_Battle_Unit()
    all_done_none_available = Next_Battle_Unit_Nearest_Available(player_idx);
    if(all_done_none_available == ST_TRUE)


XREF:
    Tactical_Combat__WIP+12C       mov     [_human_out_of_moves], e_ST_FALSE                                                 
    Tactical_Combat__WIP:loc_761A1 mov     [_human_out_of_moves], e_ST_FALSE; BUG: second time clearing this without using it
    Tactical_Combat__WIP+5EB       mov     [_human_out_of_moves], e_ST_FALSE                                                 
    Tactical_Combat__WIP+F68       cmp     [_human_out_of_moves], e_ST_TRUE                                                  
    Tactical_Combat__WIP+115A      mov     [_human_out_of_moves], e_ST_TRUE                                                  
    Tactical_Combat__WIP+116C      cmp     [_human_out_of_moves], e_ST_TRUE                                                  
    Tactical_Combat__WIP+117A      mov     [_human_out_of_moves], e_ST_FALSE                                                 
    Next_Battle_Unit+23            mov     [_human_out_of_moves], e_ST_TRUE                                                  


## _human_handle_immobile


## CMB_HumanTurn


## CMB_AIGoesFirst





## Check_For_Winner__WIP()

defender battle unit count
attacker battle unit count
AI_FightorFlight__STUB()
NOTE: The human-player fleeing is handled by the 'Left-Click Flee Button' code.


### CMB_AI_Fled
ovr105
...odd mix of AoR's; most battle unit figures and effects

set to ST_FALSE during setup section of Tactical_Combat__WIP()
set to ST_TRUE in Check_For_Winner__WIP() if AI decides to Flee
checked after existing screen-loop in Tactical_Combat__WIP()
used to set Battle_Result for End_Of_Combat__WIP()
...in End_Of_Combat__WIP(..., MsgType)
        CMB_ScrollMsg_Type = MsgType  (only usage)
    ...in Combat_Results_Scroll() and Combat_Results_Scroll_Text()
        CMB_ScrollMsg_Type
        switch(CMB_ScrollMsg_Type)
            LBX_Load_Data_Static(message_lbx_file__ovr123)
            "Your opponent has fled"

XREF:
    Tactical_Combat__WIP:loc_75ED7 mov     [CMB_AI_Fled], e_ST_FALSE
    Tactical_Combat__WIP+123D      cmp     [CMB_AI_Fled], e_ST_TRUE
    Check_For_Winner__WIP+138       mov     [CMB_AI_Fled], e_ST_TRUE


### CMB_WizardCitySiege

set to ST_FALSE during setup section of Tactical_Combat__WIP()
immediately set to ST_TRUE if combat_defender_player_idx is NEUTRAL_PLAYER_IDX











Tactical_Combat__WIP()

CMB_PrepareTurn__WIP()

AI_CMB_PlayTurn__WIP()

CMB_ProgressTurnFlow__WIP()

Check_For_Winner__WIP()


CMB_PrepareTurn__WIP(), AI_CMB_PlayTurn__WIP(), CMB_ProgressTurnFlow__WIP(), Check_For_Winner__WIP()


...
...
...
checks auto_button_field
checks if _active_battle_unit is actually unavailable
checks if _active_battle_unit is confused
checks left-click attacker_enchantment_fields
checks left-click defender_enchantment_fields
checks 'Left-Click Flee Button'
checks left-click combat grid
checks right-click combat grid
checks 'Left-Click Spell Button'
checks 'Left-Click Wait Button'
checks 'Left-Click Info Button'
checks 'Left-Click Done Button'
checks right-click Active_Unit_Window
...



## Left-Click Combat Grid
attack or move


Tactical_Combat__WIP()
    CMB_TargetFrame_X = Get_Combat_Grid_Cell_X((Grid_X + 4), (Grid_Y + 4));
    CMB_TargetFrame_Y = Get_Combat_Grid_Cell_Y((Grid_X + 4), (Grid_Y + 4));
    |-> Battle_Unit_Action__WIP(_active_battle_unit, CMB_TargetFrame_X, CMB_TargetFrame_Y);

Battle_Unit_Action__WIP()
    |-> BU_Attack__WIP()
    |-> BU_Move__WIP()

S&S:
    if((combat_grid_datum == -1) && (battle_units[battle_unit_idx].movement_points > 0))
        BU_Move__WIP(battle_unit_idx, x, y);






## Right-Click Combat Grid
select unit
show unit view



Tactical_Combat__WIP()
    AI_ImmobileCounter = 0;
    CMB_WizardCitySiege = ST_FALSE;
    if((OVL_Action_Type == 1)  /* Stack vs. City */ && (combat_defender_player_idx != NEUTRAL_PLAYER_IDX))
        CMB_WizardCitySiege = ST_TRUE;
    CMB_AI_Fled = ST_FALSE;  // ; set to 1 if the AI decides to flee
    Player_Fled = ST_FALSE;
    if(OVL_Action_Type == 1)  /* Stack vs. City */
        String_Copy_Far(CMB_CityName, _CITIES[OVL_Action_Structure].name);
    CMB_BaseAllocs__WIP();
    CMB_LoadResources__WIP();
    _combat_wx = wx;
    _combat_wy = wy;
    _combat_wp = wp;
    _combat_attacker_player = combat_attacker_player_idx;
    _combat_defender_player = combat_defender_player_idx;
    Cache_Graphics_Combat();
    CMB_Terrain_Init__WIP(wx, wy, wp);
    Defending_Unit_Count = CMB_Units_Init__WIP(troop_count, troops);
    CMB_ATKR_First_CE = 0;
    CMB_DEFR_First_CE = 0;
    CMB_combat_structure = Combat_Structure(wx, wy, wp, 0);
    CMB_CE_Refresh__WIP();
    Combat_Node_Type();
    _human_out_of_moves = ST_FALSE;
    _combat_turn = 0;
    CMB_WizCastAvailable = ST_TRUE;
    _combat_total_battle_effect_count = Combat_Info_Effects_Count();
    Init_Battlefield_Effects(CMB_combat_structure);
    _human_handle_immobile = 0;
    G_AI_StayInTownProper = 1;
    _scanned_battle_unit = ST_UNDEFINED;
    if(_combat_attacker_player == _human_player_idx)
        Switch_Active_Battle_Unit(0);
    else
        Switch_Active_Battle_Unit((_combat_total_unit_count - Defending_Unit_Count));
    CMB_ActiveUnitFrame = 0;
    CMB_TargetFrame = 0;
    CRP_CMB_NeverChecked1 = 1;
    CMB_CityDamage = 0;
    CMB_HumanTurn = ST_TRUE;
    _auto_combat_flag = ST_FALSE;
    CMB_PrepareTurn__WIP();
    _human_handle_immobile = 0;
    if(_combat_attacker_player == _human_player_idx)
        Switch_Active_Battle_Unit(0);
    else
        Switch_Active_Battle_Unit((_combat_total_unit_count - Defending_Unit_Count));
    CMB_ActiveUnitFrame = 0;
    CMB_TargetFrame = 0;
    CRP_CMB_NeverChecked1 = 1;
    CMB_AIGoesFirst = ST_FALSE;
    if(_combat_defender_player == combat_computer_player)
        AI_CMB_PlayTurn__WIP(_combat_defender_player);
        CMB_PrepareTurn__WIP();
        CMB_AIGoesFirst = ST_TRUE;
    Combat_Winner = Check_For_Winner__WIP();
    if(Combat_Winner != ST_UNDEFINED)
        leave_screen = ST_UNDEFINED;
    _human_out_of_moves = ST_FALSE;
    if(_combat_attacker_player == _human_player_idx)
        Next_Battle_Unit(_human_player_idx);
    else
        Switch_Active_Battle_Unit((_combat_total_unit_count - Defending_Unit_Count));
    Assign_Combat_Grids();
    CMB_CE_Refresh__WIP();
    CRP_CMB_NeverChecked1 = ST_TRUE;
    while(leave_screen == ST_FALSE)
