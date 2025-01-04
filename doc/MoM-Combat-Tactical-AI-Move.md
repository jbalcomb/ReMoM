

AI_CMB_PlayTurn__WIP()
    |-> AI_MoveBattleUnits__WIP()
        |-> AI_BU_ProcessAction__WIP()
            |-> G_AI_BU_MoveOrRampage__WIP()
                |-> Auto_Move_Ship()
                    |-> Combat_Move_Path_Find()

¿¿¿
!!!!! NOT Auto/AI/CP !!!!!
    j_Battle_Unit_Action__WIP()
        Battle_Unit_Action__WIP()
            |-> j_BU_Attack__WIP()
            |-> Move_Battle_Unit()
???






Auto_Move_Ship()
    Set_Movement_Cost_Map(battle_unit_idx);
    ...
    ...updates CMB_ActiveMoveMap[] with INF's
    ...
    Combat_Move_Path_Find(battle_units[battle_unit_idx].cgx, battle_units[battle_unit_idx].cgy, Dest_X, Dest_Y);



movement_path_grid_cell_count

Combat_Move_Path_Find()


What does it mean for movement_path_grid_cell_count to end as 0?



Where does movement points get updated?







G_AI_BU_Move()
¿ ~== MoO2  Module: CMBTAI  Auto_Move_Ship_() ?

Pretty sure drake178's `G_AI_BU_Move()` is MoO2's `Auto_Move_Ship_()`
Battle_Unit_Move__WIP()





## Auto_Move_Ship()

Origin_X, Origin_Y
Last_Target_X, Last_Target_Y
Facing_X_Offset, Facing_Y_Offset

CMB_Path_Xs[]
CMB_Path_Ys[]
...set in Combat_Move_Path_Find()

destination_cgx, destination_cgy
passed in

target_battle_unit_idx
What-If -1/ST_UNDEFINED


What's it getting up to here?
    if((Max_X == Dest_X) && (Max_Y == Dest_Y))








## AI_GetCombatRallyPt__WIP()

sets Rally_X,Rally_Y for the sake of AI_MoveBattleUnits__WIP(), which gets them for the sake of AI_BU_ProcessAction__WIP()
...passes Rally_X,Rally_Y to AI_BU_ProcessAction__WIP()
...but, can also just pass 0,0 or Rally_X,Rally_Y could have been set to 0,0
What does AI_BU_ProcessAction__WIP() do with them?
...if Rally_X,Rally_Y == 0, uses target cgx,cgy
...passes them to G_AI_BU_MoveOrRampage__WIP()
What does G_AI_BU_MoveOrRampage__WIP() do with them?
...nothing, just passes them through to Auto_Move_Ship()



updates CMB_ActiveMoveMap[]
sets move cost to 2 for target battle unit
sets move cost to INF for all non-target battle units

Combat_Move_Path_Find(battle_units[battle_unit_idx].cgx, battle_units[battle_unit_idx].cgy, battle_units[battle_units[battle_unit_idx].Target_BU].cgx, battle_units[battle_units[battle_unit_idx].Target_BU].cgy);

XREF:
    NX_j_AI_GetCombatRallyPt__WIP()
    AI_MoveBattleUnits()

for
    BUA_MoveNAttack   103
    BUA_RangedAttack  101
sets rally/dest to own square
PoI?
...Max_X, Max_Y are passed in, but can get updated
...that updating includes the locals Min_X, Min_Y







# DE-BUGGING

update battle unit cgx,cgy

`battle_units[battle_unit_idx].cgx =`

    Auto_Move_Ship()
        battle_units[battle_unit_idx].cgx = CMB_Path_Xs[itr_grid];
        battle_units[battle_unit_idx].cgy = CMB_Path_Ys[itr_grid];

    Move_Battle_Unit__WIP()
        battle_units[battle_unit_idx].cgx = CMB_Path_Xs[Move_Step_Index];
        battle_units[battle_unit_idx].cgy = CMB_Path_Ys[Move_Step_Index];

            assert(battle_units[battle_unit_idx].cgx >= COMBAT_GRID_XMIN);
            assert(battle_units[battle_unit_idx].cgx <= COMBAT_GRID_XMAX);
            assert(battle_units[battle_unit_idx].cgy >= COMBAT_GRID_YMIN);
            assert(battle_units[battle_unit_idx].cgy <= COMBAT_GRID_YMAX);

`battle_units[battle_unit_idx].target_cgx =`

AI_SetBasicAttacks__WIP()
...does not set the target battle unit

Elsewhere, ...
target_battle_unit_idx = AI_BU_SelectAction__WIP()
...
battle_units[battle_unit_idx].target_battle_unit_idx = target_battle_unit_idx
26 88 47 43

ovr114:181C 8B C6                                           mov     ax, _SI_battle_unit_idx
ovr114:181E BA 6E 00                                        mov     dx, size s_BATTLE_UNIT
ovr114:1821 F7 EA                                           imul    dx
ovr114:1823 C4 1E 2A 92                                     les     bx, [battle_units]
ovr114:1827 03 D8                                           add     bx, ax
ovr114:1829 8A 46 FE                                        mov     al, [byte ptr bp+target_battle_unit_idx]
ovr114:182C 26 88 47 43                                     mov     [es:bx+s_BATTLE_UNIT.target_battle_unit_idx], al

    