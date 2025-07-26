
AI_CMB_PlayTurn__WIP()
    |-> AI_MoveBattleUnits__WIP()
        |-> AI_BU_ProcessAction__WIP()
            |-> Do_Auto_Ship_Turn()
                |-> Auto_Move_Unit()





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
