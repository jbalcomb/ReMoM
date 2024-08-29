




MoO2  Module: SHIPSTK
MoO2  Module: SHIPMOVE



WIZ_NextIdleStack()
    |-> WIZ_NextUnit()
    |-> Select_Unit_Stack()
        |-> Build_Unit_Stack()
        |-> Sort_Unit_Stack()


WIZ_NextUnit() updates `_unit`, `_active_world_x`, `_active_world_y` and, maybe, `map_plane` and, maybe, updates all 'Wait' to 'Ready'
Sort_Unit_Stack() updates `_unit`



## WIZ_NextIdleStack()



## WIZ_NextUnit()
    Delta_XY_With_Wrap()
    Closest_Active_Unit
    Closest_Waiting_Unit
    _unit = Closest_Active_Unit;
    _unit = Closest_Waiting_Unit;
    _active_world_x = _UNITS[_unit].wx;
    _active_world_y = _UNITS[_unit].wy;

Closest_Waiting_Unit
    if(_UNITS[itr_units].Status == US_Wait)
Closest_Active_Unit
    if((_UNITS[itr_units].Status != US_Purify) && (_UNITS[itr_units].Status != US_Unknown_100))




OVL_StackSelect()



j_OVL_StackSelect() |-> OVL_StackSelect()

j_Build_Unit_Stack() |-> Build_Unit_Stack()



XREF's
NONE    j_Build_Unit_Stack()
OON     Build_Unit_Stack() <-| OVL_StackSelect()




### OVL_StackSelect()

XREF:
    City_Screen()
    Main_Screen()
    sub_51A1A()
    sub_51AA0()
    IDK_MaybeSwitchStackPlane_s52514()
    IDK_DoMoveStack_s5336C()
    WIZ_NextIdleStack()
    Armies_Screen()
    UnitStatPup_Draw()
    Outpost_Screen()
