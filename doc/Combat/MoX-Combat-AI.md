





Tactical_Combat__WIP()
    |-> AI_CMB_PlayTurn__WIP()
        |-> AI_MoveBattleUnits__WIP()



When does happen?
...
if defender is computer-player...
...during init section of tactical combat screen
    if(_combat_defender_player == combat_computer_player)
    {
        AI_CMB_PlayTurn__WIP(_combat_defender_player);
        // ; BUG: the defending AI gets an extra turn?
        CMB_PrepareTurn__WIP();
        CMB_AIGoesFirst = ST_TRUE;
    }

AI_MoveBattleUnits__WIP()


...towards the end of the screen-loop, right before the screen re-draw...
    if((leave_screen == ST_FALSE) && (CMB_HumanUnitsDone == ST_TRUE) && (CMB_ImmobileCanAct == ST_FALSE))
    {
        CMB_HumanUnitsDone = ST_FALSE;
        CMB_ProgressTurnFlow__WIP();
        Next_Battle_Unit(_human_player_idx);
        // ...
        // ...
        // ...
    }



BU_GetDistanceFrom() ==> Range_To_Battle_Unit()
AI_GetThreat_BU() ==> Effective_Battle_Unit_Strength()
AI_CmbtWall_BitField ==> _battlefield_city_walls




AI_MoveBattleUnits__WIP()

WIZ_GetLastRangedStr__WIP()

AI_SetBasicAttacks__WIP()

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



AI_MoveBattleUnits__WIP()
    |-> WIZ_GetLastRangedStr__WIP()

AI_MoveBattleUnits__WIP()
    |-> AI_BU_ProcessAction__WIP()
        |-> AI_BU_AssignAction__WIP()



Do_Auto_Ship_Turn()
    |-> Auto_Move_Unit()




if(_combat_defender_player == combat_computer_player)
    AI_CMB_PlayTurn__WIP(_combat_defender_player)
        |-> AI_MoveBattleUnits__WIP(_combat_defender_player)



AI_CMB_PlayTurn__WIP()
    |-> AI_MoveBattleUnits__WIP


AI_BU_ProcessAction()
    |-> AI_BU_AssignAction()
        |-> AI_BU_SelectAction__WIP()
    ...
    |-> Do_Auto_Ship_Turn()

AI_MoveBattleUnits()
    |-> AI_BU_AssignAction()
        |-> AI_BU_SelectAction__WIP()
AI_SetBasicAttacks__WIP()






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
