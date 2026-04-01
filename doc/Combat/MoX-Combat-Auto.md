


NOTE(JimBalcomb,20260331):
    Enter Combat
    Turn - Defender & Computer Player
    Hit Auto
    Turn - Attacker & Human Player
    ...
    ...
    ...
    Target: Gatekeeper (8,12)
    Action: Melee
    
    AI_BU_ProcessAction




NOTE(JimBalcomb,20260330):
    Enter Combat, Hit Auto, Immediately Ends With All Exhausted.
    Because, City Walls.
        [ChooseTarget] bu=0 vs target=8 @ (8,12) value=3 highest=-100
        [ChooseTarget] bu=0 vs target=9 @ (8,11) value=3 highest=3
        [ChooseTarget] bu=0 vs target=10 @ (7,12) value=3 highest=3
        [ChooseTarget] bu=0 vs target=11 @ (7,11) value=3 highest=3
        [ChooseTarget] bu=0 vs target=12 @ (7,13) value=14 highest=3
        [ChooseTarget] bu=0 vs target=13 @ (7,10) value=4 highest=14
        [ChooseTarget] bu=0 vs target=14 @ (6,12) value=4 highest=14
        [ChooseTarget] bu=0 vs target=15 @ (6,11) value=4 highest=14
        [ChooseTarget] bu=0 vs target=16 @ (6,13) value=-3 highest=14
        [AssignAction] bu=0 @ (14,12) action=100 target=12 has_ranged=0
    Target should have been 8, because it knows it needs to clear the city wall gate.
        [ChooseTarget] bu=0 vs target=8 @ (8,12) value=3 highest=-100




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
    _human_out_of_moves = ST_FALSE;
    AI_CMB_PlayTurn__WIP(combat_human_player);
    winner = Check_For_Winner();
    if(winner != ST_UNDEFINED)
        leave_screen = ST_UNDEFINED;


## AI_CMB_PlayTurn__WIP()
    if(player_idx != combat_human_player)
        // SPELLY  G_CMB_CastSpell((player_idx + 20), _combat_wx, _combat_wy, _combat_wp);
    winner = Check_For_Winner();  // ¿ because spell cast may resulted in a win/loss ?
    if(winner == ST_UNDEFINED)
        CMB_CE_Refresh__WIP();  // ¿ because spell cast may been an enchantment ?
        AI_MoveBattleUnits__WIP(player_idx);




¿ End_Of_Turn ?
...has to have something to do with where _combat_turn gets incremented?

CMB_ProgressTurnFlow__WIP()
    sets m_turn_is_local = ST_FALSE
    ...does stuff
    sets m_turn_is_local = ST_TRUE




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



