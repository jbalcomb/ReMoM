



AI_CMB_PlayTurn__WIP()
    |-> AI_MoveBattleUnits__WIP()
        |-> AI_BU_ProcessAction__WIP()
            |-> Do_Auto_Ship_Turn()
                |-> Auto_Move_Unit()


CMB_ProgressTurnFlow__WIP()
    |-> AI_CMB_PlayTurn__WIP()




Used in Auto-Combat, but also used elsewhere



PoI's?
'Human Player' clicks 'Auto'
    CMB_HumanUnitsDone = ST_FALSE;
    AI_CMB_PlayTurn__WIP(combat_human_player);
    winner = Check_For_Winner__WIP();
    if(winner != ST_UNDEFINED)
        leave_screen = ST_UNDEFINED;


## AI_CMB_PlayTurn__WIP()
    if(player_idx != combat_human_player)
        // SPELLY  G_CMB_CastSpell((player_idx + 20), _combat_wx, _combat_wy, _combat_wp);
    winner = Check_For_Winner__WIP();  // ¿ because spell cast may resulted in a win/loss ?
    if(winner == ST_UNDEFINED)
        CMB_CE_Refresh__WIP();  // ¿ because spell cast may been an enchantment ?
        AI_MoveBattleUnits__WIP(player_idx);




¿ End_Of_Turn ?
...has to have something to do with where _combat_turn gets incremented?

CMB_ProgressTurnFlow__WIP()
    sets CMB_HumanTurn = ST_FALSE
    ...does stuff
    sets CMB_HumanTurn = ST_TRUE




Moo2
Module: MOX
    _side_end_of_turn
Module: COMBAT1
    _end_of_turn_button
Module: COMBINIT
    End_Of_Turn_Bookeeping_()





MoO2  Auto_Move_Ship_() <-| OON XREF:  Do_Auto_Ship_Turn_() <-| OON XREF:  Do_Combat_Turn_() <-| OON XREF:  Tactical_Combat_()

Do_Combat_Turn_()
    handles individual combat actions, including actions from multiplayer network messages

MoO2
Module: CMBTAI
Auto_Move_Ship_()
OON XREF:
Module: CMBTAI
Do_Auto_Ship_Turn_()
OON XREF:
Module: COMBAT1
Do_Combat_Turn_()
OON XREF:
Module: COMBINIT
Tactical_Combat_()



## AI_BU_ProcessAction__WIP()



