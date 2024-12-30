

AI_CMB_PlayTurn__WIP()
    |-> AI_MoveBattleUnits__WIP()
        |-> AI_BU_ProcessAction__WIP()
            |-> G_AI_BU_MoveOrRampage__WIP()
                |-> Auto_Move_Ship()
                    |-> Combat_Move_Path_Find()




Auto_Move_Ship()
    Set_Movement_Cost_Map(battle_unit_idx);
    ...
    ...updates CMB_ActiveMoveMap[] with INF's
    ...
    Combat_Move_Path_Find(battle_units[battle_unit_idx].cgx, battle_units[battle_unit_idx].cgy, Dest_X, Dest_Y);



movement_path_grid_cell_count

Combat_Move_Path_Find()


What does it mean for movement_path_grid_cell_count to end as 0?







G_AI_BU_Move()
¿ ~== MoO2  Module: CMBTAI  Auto_Move_Ship_() ?

Pretty sure drake178's `G_AI_BU_Move()` is MoO2's `Auto_Move_Ship_()`
Battle_Unit_Move__WIP()
