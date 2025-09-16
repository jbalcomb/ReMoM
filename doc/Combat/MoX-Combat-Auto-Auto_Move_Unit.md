



...debugging SAVETEST.GAM...
Combat_Move_Path_Find(battle_units[battle_unit_idx].cgx, battle_units[battle_unit_idx].cgy, Dest_X, Dest_Y);
9,14
6,13
movement_path_grid_cell_count: 3









Set_Movement_Cost_Map() sets CMB_ActiveMoveMap[] from battlefield->MoveCost_Ground[], MoveCost_Teleport[], MoveCost_Ground2[], MoveCost_Sailing[]


Do_Auto_Unit_Turn()
    Set_Movement_Cost_Map()
    BU_SetCityMovement__WIP()
    AI_RestrictToCity__WIP()
    Combat_Move_Path_Find()



## Auto_Move_Unit()

OON XREF:  Do_Auto_Unit_Turn()

~ Combat.c   Move_Battle_Unit__WIP()
~ MainScr.c  Move_Units_Draw()

...
do move-path process
...

Origin_X = battle_units[battle_unit_idx].cgx;
Origin_Y = battle_units[battle_unit_idx].cgy;


delta_x = abs(RP_Origin_X_2 - dst_cgx);
delta_y = abs(RP_Origin_Y_2 - dst_cgy);

dst_cgx,cgy
    passed in
    battle_units[battle_unit_idx].target_cgx = dst_cgx;
    battle_units[battle_unit_idx].target_cgy = dst_cgy;
    if(target_battle_unit_idx > -1)
        dst_cgx = battle_units[target_battle_unit_idx].cgx;
        dst_cgy = battle_units[target_battle_unit_idx].cgy;
    ...used for bounding box calculation

RP_Origin_X_2,RP_Origin_Y_2
    RP_Origin_X_2 = battle_units[battle_unit_idx].cgx;
    RP_Origin_Y_2 = battle_units[battle_unit_idx].cgy;
    delta_x = abs(RP_Origin_X_2 - dst_cgx);
    delta_y = abs(RP_Origin_Y_2 - dst_cgy);

What is the point of the bounding box/move area limit?

    x,y distance?


    if((Max_X == dst_cgx) && (Max_Y == dst_cgy))
        Min_X = COMBAT_GRID_XMIN;
        Max_X = COMBAT_GRID_XMAX;
        Min_Y = COMBAT_GRID_YMIN;
        Max_Y = COMBAT_GRID_YMAX;

Facing:  (~ MoO2  Turn_To_Face_())
facing_cgx = (final_cgx - origin_cgx);
facing_cgy = (final_cgy - origin_cgy);
battle_units[].target_cgx = (final_cgx + facing_cgx);
battle_units[].target_cgy = (final_cgy + facing_cgy);
...positive,negative...


Auto_Move_Unit()
14,12
6,13
movement_path_grid_cell_count: 8
...set Origin_X,Origin_Y; set battle_units[].target_cgx,battle_units[].target_cgy; set RP_Origin_X_2,RP_Origin_Y_2
...make bounding box
First Step:
            battle_units[battle_unit_idx].target_cgx = _cmbt_mvpth_x[itr_grid];
            battle_units[battle_unit_idx].target_cgy = _cmbt_mvpth_y[itr_grid];
13,12  ...first next path square
...if next square is target's square, do attack
...play movement animation
                        battle_units[battle_unit_idx].cgx = _cmbt_mvpth_x[itr_grid];
                        battle_units[battle_unit_idx].cgy = _cmbt_mvpth_y[itr_grid];
...the unit has officially moved
LOOP itr_path
...the move we just made was 2 mp, which is all we have, so break out of the move-path loop
...
        Last_Target_X = dst_cgx;
        Last_Target_Y = dst_cgy;
...final destination
¿ MoO2  Get_Facing_(); Rotate_Ship_(); ... Turn_To_Face_() ?

 6 - 14 = -8
13 - 12 =  1
 6 + -8 = -2
13 +  1 = 14
Eh? Just 'facing cgx,cgy can be negative'?

Movement Animation

Combat_Screen_Draw()
    Combat_Grid_Entities__WIP()
    Combat_Screen_Map_Draw__WIP()
        Combat_Screen_Map_Draw_Entities__WIP()





## Combat_Move_Path_Find()



## Do_Auto_Unit_Turn()

OON XREF:  AI_BU_ProcessAction__WIP()

reset CMB_ActiveMoveMap[]; update with non-target units as impassible; update with city squares as impassible; update with city area perimeter as impassible
maybe, Stomp-Around
does the original move

    // sets movement_path_grid_cell_count
    Combat_Move_Path_Find(battle_units[battle_unit_idx].cgx, battle_units[battle_unit_idx].cgy, dst_cgx, dst_cgy);
    ...
    // HERE: dst_cgx,cgy and rally_cgx,cgy are as passed in
    Auto_Move_Unit(battle_unit_idx, dst_cgx, dst_cgy, target_battle_unit_idx, rally_cgx, rally_cgy);

XREF:
    NX_j_Do_Auto_Unit_Turn__WIP()
    AI_BU_ProcessAction__WIP()
MoO2
    Auto_Move_Ship_()
    OON XREF:  Do_Auto_Unit_Turn_()
    Do_Auto_Unit_Turn_()
    OON XREF:  Do_Combat_Turn_()
MoO2  Module: CMBTAI  Auto_Move_Ship_() <-| OON XREF:  Module: CMBTAI  Do_Auto_Ship_Turn_() <-| OON XREF:  Module: COMBAT1  Do_Combat_Turn_()
Do_Combat_Turn_()
    handles individual combat actions, including actions from multiplayer network messages
