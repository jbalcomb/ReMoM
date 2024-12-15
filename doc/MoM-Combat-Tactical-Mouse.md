

Per the manual, "Squares that are not valid as targets for an active unit's actions show a red "X" when the mouse cursor moves over them."

That is exactly and only what you get from Assign_Mouse_Image(), with `if(CMB_TargetRows[cgy][cgx] == -2)`.

And, all of CMB_TargetRows[] is defaulted to -2 in Assign_Combat_Grids().

CMB_TargetRows




Combat_Move_Path_Valid()
...closes with indexing CMB_NearBuffer_3[], CMB_Path_Costs[], and CMB_ActiveMoveMap[] using an indexing variable of ((itr_y * 21) + itr_x)



Assign_Combat_Grids()
    |-> Combat_Move_Path_Valid()


Assign_Combat_Grids()

    for(itr_y = 0; itr_y < 22; itr_y++)
        for(itr_x = 0; itr_x < 21; itr_x++ )
            CMB_TargetRows[itr_y][itr_x] = -2;

    if((battlefield->Walled_City == ST_TRUE) && (battle_units[_active_battle_unit].controller_idx == _combat_attacker_player) && ((battle_units[_active_battle_unit].Abilities & UA_WALLCRUSHER) != 0))
        for(itr_y = 0; itr_y < 22; itr_y++)
            for(itr_x = 0; itr_x < 21; itr_x++ )
                if(Combat_Grid_Cell_Has_City_Wall(itr_x, itr_y) == ST_TRUE)
                    CMB_TargetRows[itr_y][itr_x] = 99;

    for(itr = 0; itr < _combat_total_unit_count; itr++)
        if((battle_units[itr].Status == bus_Active) && (battle_units[itr].Image_Effect != 5))
            CMB_TargetRows[battle_units[itr].cgy][battle_units[itr].cgx] = itr;  // batle_unit_idx
    Combat_Move_Path_Valid(battle_units[_active_battle_unit].cgx, battle_units[_active_battle_unit].cgy, useable_moves2);
    for(itr_y = 0; itr_y < 22; itr_y++)
        Row_Origin_Offset = (itr_y * 21);
        for(itr_x = 0; itr_x < 21; itr_x++)
            if(CMB_NearBuffer_3[Row_Origin_Offset + itr_x] == 1)
                CMB_TargetRows[itr_y][itr_x] = -1;
                UU_TotalMoveArea++;
    CMB_TargetRows[battle_units[_active_battle_unit].cgy][battle_units[_active_battle_unit].cgx] = _active_battle_unit;





##### Naming Things is Hard

mouse cursor
map squares
combat grid
outlined in blue
squares
not valid as targets
active unit’s actions  
red "X"
mouse cursor
valid target squares
actions
attacking or movement
show different symbols
active unit
mouse cursor
empty squares
battlefield
cursor
small winged boot
square
winged boot
currently active unit
unit
specific map square
destination square



Page 92  (PDF Page 97)

Note that as you run the mouse cursor over the map squares of the combat grid,  
  the squares appear outlined in blue.  
Squares that are not valid as targets 
  for an active unit’s actions  
  show a red "X" when the mouse cursor moves over them.  
Valid target squares for actions  
  (attacking or movement)  
  show different symbols  
  (described in the following sections).  
Right-clicking on friendly or enemy units calls up a unit statistics window of the unit (see Normal Units).  

MOVEMENT

To move an active unit (see above section),  
move the mouse cursor over empty squares on the battlefield until you see that the cursor has the appearance of a small winged boot.  
Any square over which you see this winged boot is a square to which you can move the currently active unit.  
To move your unit to a specific map square, click on the destination square.
