





Tactical_Combat__WIP()

CMB_PrepareTurn__WIP()

AI_CMB_PlayTurn__WIP()

CMB_ProgressTurnFlow__WIP()

CMB_WinLoseFlee__WIP()


CMB_PrepareTurn__WIP(), AI_CMB_PlayTurn__WIP(), CMB_ProgressTurnFlow__WIP(), CMB_WinLoseFlee__WIP()


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
    |-> BU_CombatAction__WIP(_active_battle_unit, CMB_TargetFrame_X, CMB_TargetFrame_Y);

BU_CombatAction__WIP()
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
    CMB_HumanUnitsDone = ST_FALSE;
    _combat_turn = 0;
    CMB_WizCastAvailable = ST_TRUE;
    _combat_total_battle_effect_count = Combat_Info_Effects_Count();
    Init_Battlefield_Effects(CMB_combat_structure);
    CMB_ImmobileCanAct = 0;
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
    CMB_ImmobileCanAct = 0;
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
    Combat_Winner = CMB_WinLoseFlee__WIP();
    if(Combat_Winner != ST_UNDEFINED)
        leave_screen = ST_UNDEFINED;
    CMB_HumanUnitsDone = ST_FALSE;
    if(_combat_attacker_player == _human_player_idx)
        Next_Battle_Unit(_human_player_idx);
    else
        Switch_Active_Battle_Unit((_combat_total_unit_count - Defending_Unit_Count));
    Assign_Combat_Grids();
    CMB_CE_Refresh__WIP();
    CRP_CMB_NeverChecked1 = ST_TRUE;
    while(leave_screen == ST_FALSE)
