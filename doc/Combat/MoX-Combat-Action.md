



BU_Attack__WIP() ==> Battle_Unit_Attack__WIP()
BU_MeleeFlightCheck__WIP() ==> Check_Attack_Melee()
BU_RangedValidate__WIP() ==> Check_Attack_Ranged()


Tactical_Combat__WIP()
        j_Battle_Unit_Action__WIP()
            Battle_Unit_Action__WIP()
                |-> j_Battle_Unit_Attack__WIP()
                    |-> Battle_Unit_Attack__WIP()



Tactical_Combat__WIP()
    /* BEGIN:  Left-Click Combat Grid */
    if(input_field_idx == combat_grid_field)
        _human_handle_immobile = ST_FALSE;
        CMB_TargetFrame_X = Get_Combat_Grid_Cell_X((Grid_X + 4), (Grid_Y + 4));
        CMB_TargetFrame_Y = Get_Combat_Grid_Cell_Y((Grid_X + 4), (Grid_Y + 4));
        Battle_Unit_Action__WIP(_active_battle_unit, CMB_TargetFrame_X, CMB_TargetFrame_Y);
        for(itr = 0; itr < _combat_total_unit_count; itr++)
            BU_SetVisibility__WIP(itr);
        Assign_Combat_Grids();
        Combat_Winner = Check_For_Winner__WIP();



void Battle_Unit_Action__WIP(int16_t battle_unit_idx, int16_t cgx, int16_t cgy)

## Battle_Unit_Action__WIP()

branching?

from ...
to ...
...somehow we get here...
...but, we may have actually bailed
        if(ranged_attack_check > 0)
            Attack_Type = 0;

4 paths to @@JmpDone_Return
3 paths to @@JmpJmpDone_Return
3 each from the bail paths, 4th from 1st is 2nd falling through
...bailing because range to target is > 1...not melee, but ranged attack
¿ after this, Attack_Type ONLY gets set to {0,1} ?
...which then gets passed to BU_AttackTarget()
...which uses it as {F,T} for 'do ranged attack'
...which, if ST_TRUE, passed attack type 2 to BU_ProcessAttack()
~== {0: melee, 1: thrown, 2: ranged}

HERE: `Attack_Type` is Range_To_Battle_Unit(attacker_battle_unit_idx, defender_battle_unit_idx)
(Attack_Type > 1)
ranged_attack_check == {0,1,2}

if
    Attack_Type > 1
    ranged_attack_check != 0
    ranged_attack_check == 1
        ...human...auto combat...Warn1(); return;

if
    Attack_Type > 1
    ranged_attack_check != 0
    ranged_attack_check != 1
    ranged_attack_check == 2
        ...human...auto combat...Warn1(); return;

drake178: return 0 and, if the unit belongs to the human player and auto combat is off, show the corresponding ranged targeting error message

What is `Attack_Type`?
What is `ranged_attack_check`?





CMB_TargetFrame_X, CMB_TargetFrame_Y
cgx, cgy
Target_X, Target_Y

    if(combat_grid_target == 99)
        Target_X = cgx;  // passed in - CMB_TargetFrame_X = Get_Combat_Grid_Cell_X((Grid_X + 4), (Grid_Y + 4));
        Target_Y = cgy;  // passed in - CMB_TargetFrame_Y = Get_Combat_Grid_Cell_Y((Grid_X + 4), (Grid_Y + 4));
    else
        Target_X = battle_units[combat_grid_target].cgx;
        Target_Y = battle_units[combat_grid_target].cgy;

    X_Distance = abs(Target_X - battle_units[battle_unit_idx].cgx);  // passed in - _active_battle_unit
    Y_Distance = abs(Target_Y - battle_units[battle_unit_idx].cgy);  // passed in - _active_battle_unit
So, ...
    X_Distance will be
        (battle_units[combat_grid_target].cgx - battle_units[_active_battle_unit].cgx)
        or
        (Get_Combat_Grid_Cell_X((Grid_X + 4), (Grid_Y + 4)) - battle_units[_active_battle_unit].cgx)

BU_Attack__WIP(battle_unit_idx, combat_grid_target, Target_X, Target_Y);
BU_Attack__WIP(battle_unit_idx, combat_grid_target, Target_X, Target_Y);
BU_Attack__WIP(battle_unit_idx, combat_grid_target, Target_X, Target_Y);
BU_Attack__WIP(battle_unit_idx, combat_grid_target, Target_X, Target_Y);
BU_Attack__WIP(battle_unit_idx, combat_grid_target, Target_X, Target_Y);



## {CMB_TargetFrame_X, CMB_TargetFrame_Y}

e.g.,
    Tactical_Combat__WIP()
        CMB_TargetFrame_X = Get_Combat_Grid_Cell_X((Grid_X + 4), (Grid_Y + 4));
        CMB_TargetFrame_Y = Get_Combat_Grid_Cell_Y((Grid_X + 4), (Grid_Y + 4));
        Battle_Unit_Action__WIP(_active_battle_unit, CMB_TargetFrame_X, CMB_TargetFrame_Y);


## {Get_Combat_Grid_Cell_X(), Get_Combat_Grid_Cell_Y() }

combat_grid_field = Add_Grid_Field(0, 0, 1, 1, 319, 164, &Grid_X, &Grid_Y, ST_UNDEFINED);

            BEGIN:  Left-Click Combat Grid
            if(input_field_idx == combat_grid_field)
                CMB_TargetFrame_X = Get_Combat_Grid_Cell_X((Grid_X + 4), (Grid_Y + 4));
                CMB_TargetFrame_Y = Get_Combat_Grid_Cell_Y((Grid_X + 4), (Grid_Y + 4));
                Battle_Unit_Action__WIP(_active_battle_unit, CMB_TargetFrame_X, CMB_TargetFrame_Y);

            BEGIN:  Right-Click Combat Grid
            if(-(combat_grid_field) == input_field_idx)
                RightClick_X = Get_Combat_Grid_Cell_X((Grid_X + 4), (Grid_Y + 4));
                RightClick_Y = Get_Combat_Grid_Cell_Y((Grid_X + 4), (Grid_Y + 4));
                battle_unit_idx = CMB_TargetRows[RightClick_Y][RightClick_X];


Why the +4?

same calc as in Screen_To_Combat_Grid_Cell_X_And_Offset()
Tactical_Combat__WIP() calls with (Grid_X + 4), (Grid_Y + 4)
Assign_Mouse_Image() calls with (Pointer_X() + 4), (Pointer_Y() + 4)

int16_t Get_Combat_Grid_Cell_X(int16_t screen_x, int16_t screen_y)
    int16_t combat_grid_cell_x = 0;
    combat_grid_cell_x = ((((screen_x - 158) / 2) + (screen_y + 80)) / 16);
    return combat_grid_cell_x;
int16_t Get_Combat_Grid_Cell_Y(int16_t screen_x, int16_t screen_y)
    int16_t combat_grid_cell_y = 0;
    combat_grid_cell_y = (((screen_y + 80) - ((screen_x - 158) / 2)) / 16);
    return combat_grid_cell_y;
