


NOTE(JimBalcomb, 20260319):

Auto_Do_Combat_Turn()
    /* Initialize attack parameters */
    |-> AI_SetBasicAttacks()
    /* Determine Rally Point */
    |-> Sort_Battle_Units()
    /* Get rally point based on the median speed melee unit's destination */
    |-> AI_GetCombatRallyPt()
    /* Pass 1: Process non-hero units (or all units initially) */
    for (unit_idx = 0; unit_idx < _combat_total_unit_count; unit_idx++)
        Switch_Active_Battle_Unit(unit_idx);
        Assign_Combat_Grids();
        AI_BU_AssignAction(unit_idx, bua_Ready);
        ..if offensive
            AI_BU_ProcessAction(itr_battle_units, 0, 0);
        ...if defensive
            AI_BU_ProcessAction(itr_battle_units, rally_x, rally_y);  /* in a city with a wall, so behave defensively */
    /* Pass 2: Specific logic for Hero units */
    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
        if(bu_ptr->controller_idx == player_idx && bu_ptr->status == bus_Active &&  bu_ptr->movement_points > 0)
            if(_UNITS[bu_ptr->unit_idx].Hero_Slot > -1 && !(bu_ptr->Combat_Effects & bue_Black_Sleep))
                Switch_Active_Battle_Unit(itr_battle_units);
                Assign_Combat_Grids();
                AI_BU_AssignAction(itr_battle_units, bua_Ready);
                AI_BU_ProcessAction(itr_battle_units, rally_x, rally_y);

Auto_Cast_Spell_And_Do_Combat_Turn()
    |-> Auto_Do_Combat_Turn()
        |-> AI_SetBasicAttacks()
        |-> Sort_Battle_Units()
        |-> AI_GetCombatRallyPt()
        |-> Switch_Active_Battle_Unit(itr_battle_units);
        |-> Assign_Combat_Grids();
        |-> AI_BU_AssignAction(itr_battle_units, bua_Ready);
        |-> AI_BU_ProcessAction(itr_battle_units, rally_x, rally_y);






Tactical_Combat__WIP()
    |-> Auto_Cast_Spell_And_Do_Combat_Turn()
        |-> Auto_Do_Combat_Turn__WIP()



When does happen?
...
if defender is computer-player...
...during init section of tactical combat screen
    if(_combat_defender_player == combat_computer_player)
    {
        Auto_Cast_Spell_And_Do_Combat_Turn(_combat_defender_player);
        // ; BUG: the defending AI gets an extra turn?
        CMB_PrepareTurn__WIP();
        m_cp_took_turn = ST_TRUE;
    }

Auto_Do_Combat_Turn__WIP()


...towards the end of the screen-loop, right before the screen re-draw...
    if((leave_screen == ST_FALSE) && (_human_out_of_moves == ST_TRUE) && (_human_handle_immobile == ST_FALSE))
    {
        _human_out_of_moves = ST_FALSE;
        CMB_ProgressTurnFlow__WIP();
        Next_Battle_Unit(_human_player_idx);
        // ...
        // ...
        // ...
    }



BU_GetDistanceFrom() ==> Range_To_Battle_Unit()
AI_GetThreat_BU() ==> Effective_Battle_Unit_Strength()
AI_CmbtWall_BitField ==> _battlefield_city_walls




Auto_Do_Combat_Turn__WIP()

WIZ_GetLastRangedStr__WIP()

AI_SetBasicAttacks()

AI_BU_ProcessAction()

AI_BU_AssignAction()

Do_Auto_Ship_Turn()

Auto_Move_Unit()



MoO2
Do_Auto_Ship_Turn_()
    |-> Get_Current_Target_()
        |-> Get_Current_Ship_Target_()
            |-> Target_Ship_Value_()
Do_Auto_Ship_Turn_()
    |-> Choose_Target_()
        |-> Select_Ship_To_Target_()



Auto_Do_Combat_Turn__WIP()
    |-> WIZ_GetLastRangedStr__WIP()

Auto_Do_Combat_Turn__WIP()
    |-> AI_BU_ProcessAction__WIP()
        |-> AI_BU_AssignAction__WIP()



Do_Auto_Ship_Turn()
    |-> Auto_Move_Unit()




if(_combat_defender_player == combat_computer_player)
    Auto_Cast_Spell_And_Do_Combat_Turn(_combat_defender_player)
        |-> Auto_Do_Combat_Turn__WIP(_combat_defender_player)



Auto_Cast_Spell_And_Do_Combat_Turn()
    |-> Auto_Do_Combat_Turn__WIP


AI_BU_ProcessAction()
    |-> AI_BU_AssignAction()
        |-> AI_BU_SelectAction__WIP()
    ...
    |-> Do_Auto_Ship_Turn()

Auto_Do_Combat_Turn()
    |-> AI_BU_AssignAction()
        |-> AI_BU_SelectAction__WIP()
AI_SetBasicAttacks()








## AI_BU_ProcessAction()

if the combat grid x,y passed in is 0, it uses the target of the passed in BUI to get the combat grid x,y from the target BUI's record

switches on battle_units[].action

102  BUA_MoveNFire
jt_bua_02:                              ; case 0x66





## Do_Auto_Ship_Turn()



### Battle Unit Action

104 DoomBolt
105 Fireball
109 SummonDemon
110 Web

Doom Bolt, Fireball, Summon Demon, Web

jt_bua_00
jt_bua_01
jt_bua_02
jt_bua_03
jt_bua_04       104 DoomBolt
jt_bua_05       105 Fireball
jt_bua_06
jt_bua_07_08
jt_bua_07_08
jt_bua_09       109 SummonDemon
jt_bua_10       110 Web
