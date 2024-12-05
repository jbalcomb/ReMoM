
Combat Unit Display


CMB_DrawUnitDisplay__WIP() ==> Draw_Combat_Unit_Display()
CMB_CursorBattleUnit ==> _scanned_battle_unit
IMG_CMB_ScanIcons



Tactical_Combat__WIP()
    CMB_DrawFullScreen__WIP()
        if(magic_set.Auto_Unit_Info == ST_TRUE)
            Draw_Combat_Unit_Display()


CMB_CursorBattleUnit
...scanned_battle_unit, !_active_battle_unit



Attr_Display_Var
Attribute_Value
...
melee weapon type icon index
melee attack strength





## _scanned_battle_unit

set in Assign_Mouse_Image(), from CMB_TargetRows[grid_y][grid_x]

    Tactical_Combat__WIP+1DC           mov     [_scanned_battle_unit], e_ST_UNDEFINED             
    CMB_PrepareTurn__WIP+1B            mov     [_scanned_battle_unit], e_ST_UNDEFINED             
    CMB_ProgressTurnFlow__WIP+6        mov     [_scanned_battle_unit], e_ST_UNDEFINED             
    Assign_Mouse_Image+8               mov     [_scanned_battle_unit], e_ST_UNDEFINED             
    Assign_Mouse_Image+212             mov     [_scanned_battle_unit], _SI_scanned_battle_unit_idx
    Combat_Figure_Active_Red_Outline+3 mov     ax, [_scanned_battle_unit]                         
    Draw_Combat_Unit_Display+8         cmp     [_scanned_battle_unit], e_ST_UNDEFINED             
    Draw_Combat_Unit_Display:loc_86518 cmp     [_scanned_battle_unit], 36                         
    Draw_Combat_Unit_Display+D8        mov     ax, [_scanned_battle_unit]                         
    Draw_Combat_Unit_Display+188       push    [_scanned_battle_unit]          ; battle_unit_idx  
    Draw_Combat_Unit_Display+19A       mov     ax, [_scanned_battle_unit]                         
    Draw_Combat_Unit_Display:loc_866E9 push    [_scanned_battle_unit]          ; battle_unit_idx  
    Draw_Combat_Unit_Display+1F5       mov     ax, [_scanned_battle_unit]                         
    Draw_Combat_Unit_Display:loc_86745 mov     ax, [_scanned_battle_unit]                         
    Draw_Combat_Unit_Display+258       mov     ax, [_scanned_battle_unit]                         
    Draw_Combat_Unit_Display:loc_86788 mov     ax, [_scanned_battle_unit]                         
    Draw_Combat_Unit_Display:loc_867B0 push    [_scanned_battle_unit]          ; BU_Index         
    Draw_Combat_Unit_Display+361       push    [_scanned_battle_unit]          ; BU_Index         
    Draw_Combat_Unit_Display+36D       mov     ax, [_scanned_battle_unit]                         
    Draw_Combat_Unit_Display+3B2       mov     ax, [_scanned_battle_unit]                         
    Draw_Combat_Unit_Display+3F7       mov     ax, [_scanned_battle_unit]                         
    Draw_Combat_Unit_Display+40C       mov     ax, [_scanned_battle_unit]                         
    Draw_Combat_Unit_Display:loc_86955 mov     ax, [_scanned_battle_unit]                         
    Draw_Combat_Unit_Display+464       mov     ax, [_scanned_battle_unit]                         
    CMB_SetTargetCursor+10             mov     [_scanned_battle_unit], e_ST_UNDEFINED             
    CMB_SetTargetCursor+E8             mov     [_scanned_battle_unit], ax                         

Usages?
    Tactical_Combat__WIP+1DC           mov     [_scanned_battle_unit], e_ST_UNDEFINED             
    CMB_PrepareTurn__WIP+1B            mov     [_scanned_battle_unit], e_ST_UNDEFINED             
    CMB_ProgressTurnFlow__WIP+6        mov     [_scanned_battle_unit], e_ST_UNDEFINED             
    Assign_Mouse_Image+8               mov     [_scanned_battle_unit], e_ST_UNDEFINED             
    Combat_Figure_Active_Red_Outline+3 mov     ax, [_scanned_battle_unit]                         
    Draw_Combat_Unit_Display+8         cmp     [_scanned_battle_unit], e_ST_UNDEFINED             
    CMB_SetTargetCursor+10             mov     [_scanned_battle_unit], e_ST_UNDEFINED             


Tactical_Combat__WIP()
    if(leave_screen == ST_FALSE)
        Assign_Mouse_Image();

Assign_Mouse_Image()
    Screen_X = (Pointer_X() + 4);  // ¿ why the (+ 4) ?
    Screen_Y = (Pointer_Y() + 4);  // ¿ why the (+ 4) ?
    ...
    // ¿ cgc2, cgc1 ?
    Tile_X = Get_Combat_Grid_Cell_X(Screen_X, Screen_Y);
    // ¿ cgc2, cgc1 ?
    Tile_Y = Get_Combat_Grid_Cell_Y(Screen_X, Screen_Y);


    _scanned_battle_unit = ST_UNDEFINED
    ...
    _SI_scanned_battle_unit_idx = CMB_TargetRows[grid_y][grid_x]
    _scanned_battle_unit = _SI_scanned_battle_unit_idx



## CMB_TargetRows[]
What is stored in here?
...at the least, battle_unit_idx for each battle unit
code checks for -2, -1, 99, 









##### Naming Things is Hard

Combat unit display
a combat unit display
upper right corner of the combat screen
active unit



Settings Screen

Auto Unit Information


Page 90  (PDF Page 95)

Combat unit display


Page 92  (PDF Page 97)

Finally,  
  if you have toggled on the additional unit information button in your game settings window (see Settings),  
  you have a combat unit display open in the upper right corner of the combat screen.  
This window can be toggled on and off by pressing L on the keyboard.  
The combat unit display contains some useful information to have available during battle,  
  including the remaining ammunition (if relevant) of the active unit,   
  mana and health of the "front" figure in the active unit.
