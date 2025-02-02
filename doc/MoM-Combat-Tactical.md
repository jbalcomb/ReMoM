
MoM / Mox
Tactical Combat



Active Unit Window
Spell Information Window
Defender Enchantment Window
Attacker Enchantment Window
Combat Unit Display
Combat Information Window



Wall Crushers
    Catapult
    Engineers
    War Mammoths
    Colossus
    Earth Elemental
    Fire Giant
    Stone Giant
    Storm Giants



¿ "entity" vs. unit/object/combat_grid_datum ?
unit level
figure level
unit vs. rock vs. tree vs. wall vs. structure



Per BU_IsInCityProper()
city area on combat grid
cgx >=  5 && cgx <=  8
cgy >= 10 && cgy <= 13
cgx: {  5,  6,  7,  8 }
cgy: { 10, 11, 12, 13 }
{
    { 5, 10 }, { 5, 11 }, { 5, 12 }, { 5, 13 },
    { 6, 10 }, { 6, 11 }, { 6, 12 }, { 6, 13 },
    { 7, 10 }, { 7, 11 }, { 7, 12 }, { 7, 13 },
    { 8, 10 }, { 8, 11 }, { 8, 12 }, { 8, 13 }
}



CMB_SelectedUnit ==> _active_battle_unit
G_BU_SelectUnit__WIP() ==> Switch_Active_Battle_Unit()
G_CMB_Auto_OFF__WIP() ==> Turn_Off_Auto_Combat()   ~ MoO2 Toggle_Auto_Combat_Flags_()
WIZ_BU_SelectClosest__WIP() ==> Next_Battle_Unit_Nearest_Available()
WIZ_BU_SelectNext__WIP() ==> Next_Battle_Unit()


## Active Battle Unit


set in 8 places
    CMB_VortexPlayerMove+63       mov     [_active_battle_unit], -1                  
    End_Of_Combat__WIP:loc_A5B34  mov     [_active_battle_unit], 667                 
    Next_Battle_Unit+AA           mov     [_active_battle_unit], _SI_itr_battle_units
    STK_CaptureCity+372           mov     [_active_battle_unit], ax                  
    CMB_PrepareTurn__WIP+8E       mov     [_active_battle_unit], ax                  
    Switch_Active_Battle_Unit+1A  mov     [_active_battle_unit], ax                  
    End_Of_Combat__WIP+B8F        mov     [_active_battle_unit], ax                  
    AI_BU_ProcessAction+64        mov     [_active_battle_unit], si                  

[x] Switch_Active_Battle_Unit()
[x] Next_Battle_Unit()



Tactical_Combat__WIP()
    if(_combat_attacker_player == _human_player_idx)
        Switch_Active_Battle_Unit(0);
    else
        Switch_Active_Battle_Unit((_combat_total_unit_count - Defending_Unit_Count));
NOTE: there's a call to CMB_PrepareTurn__WIP() immediately preceding


Tactical_Combat__WIP+304
Tactical_Combat__WIP+70A
Tactical_Combat__WIP+775
Tactical_Combat__WIP+EB1
Tactical_Combat__WIP+F5D
Tactical_Combat__WIP+1189
Turn_Off_Auto_Combat+D
    |-> j_Next_Battle_Unit()
        |-> Next_Battle_Unit()


Tactical_Combat__WIP()
    ***cancel auto combat***
        j_CMB_ProgressTurnFlow__WIP()
        j_Turn_Off_Auto_Combat()
            Turn_Off_Auto_Combat()
                j_Next_Battle_Unit(_human_player_idx)
                    Next_Battle_Unit(_human_player_idx)


Next_Battle_Unit()

XREF:
    j_Next_Battle_Unit()
        Next_Battle_Unit()
XREF:
j_Next_Battle_Unit
    Tactical_Combat__WIP+304  call    j_Next_Battle_Unit
    Tactical_Combat__WIP+70A  call    j_Next_Battle_Unit
    Tactical_Combat__WIP+775  call    j_Next_Battle_Unit
    Tactical_Combat__WIP+EB1  call    j_Next_Battle_Unit
    Tactical_Combat__WIP+F5D  call    j_Next_Battle_Unit
    Tactical_Combat__WIP+1189 call    j_Next_Battle_Unit
    Turn_Off_Auto_Combat+D    call    j_Next_Battle_Unit

sets _active_battle_unit, via Next_Battle_Unit_Nearest_Available(player_idx)
but, ...
    if(battle_units[_active_battle_unit].controller_idx != combat_human_player)
        for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
            if(battle_units[itr_battle_units].controller_idx == _combat_attacker_player)
                _active_battle_unit = itr_battle_units;

called for Turn_Off_Auto_Combat(), so doesn't *feel* like "Next"
other six?
Tactical_Combat__WIP()
    Pre Screen Loop
        if(_combat_attacker_player == _human_player_idx)
            j_Next_Battle_Unit(_human_player_idx)
    Screen / Input Loop
        Eh? _active_battle_unit has become unavailable?
            j_Next_Battle_Unit(_human_player_idx)
        _active_battle_unit is *confused*
            j_Next_Battle_Unit(_human_player_idx)
        input_field_idx == wait_button_field
            j_Next_Battle_Unit(_human_player_idx)
        input_field_idx == done_button_field
            j_Next_Battle_Unit(_human_player_idx)
    Closing The Loop
        if leave_screen == ST_FALSE && CMB_HumanUnitsDone == ST_TRUE && CMB_ImmobileCanAct == ST_FALSE
            CMB_HumanUnitsDone = ST_FALSE
            j_CMB_ProgressTurnFlow__WIP()
            j_Next_Battle_Unit(_human_player_idx)





## Next_Battle_Unit_Nearest_Available()

Next_Battle_Unit_Nearest_Available()
OON XREF:  Next_Battle_Unit()

UNITSTK.C Next_Unit_Nearest_Available()
OON XREF:  WIZ_NextIdleStack()

CMB_HumanUnitsDone
CMB_ImmobileCanAct
in Next_Battle_Unit()
    if(all_done_none_available == ST_TRUE)
        CMB_HumanUnitsDone = ST_TRUE;
    else
        CMB_ImmobileCanAct = ST_FALSE;




Move_Units()
    Eval_Move_Path__WIP(..., &attack_flag, ...)
    if((attack_flag == ST_TRUE) || (UU_flag_FALSE == ST_TRUE))
        goto Combat_Handlers;
Combat_Handlers:
    if(OVL_Action_Type == 5)
    else
    combat_result = Combat__WIP(player_idx, defender_idx, troop_count, &troops[0]);
    if(combat_result == 1)
        Player_Army_At_Square(_combat_wx, _combat_wy, _combat_wp, player_idx, &troop_count, troops);



combat_result = Combat__WIP(player_idx, defender_idx, troop_count, &troops[0]);
    if(((combat_attacker_player_idx == _human_player_idx) || (defender_idx == _human_player_idx)) && (magic_set.strategic_combat_only == ST_FALSE))
        Battle_Outcome = Tactical_Combat__WIP(combat_attacker_player_idx, defender_idx, troops, troop_count, _combat_wx, _combat_wy, _combat_wp, &Item_Count, &Item_List[0]);



int16_t Tactical_Combat__WIP(int16_t combat_attacker_player_idx, int16_t combat_defender_player_idx, int16_t troops[], int16_t troop_count, int16_t wx, int16_t wy, int16_t wp, int16_t * item_count, int16_t item_list[])




MoO2
_combat_total_ship_count gets incremented in Tactical_Combat_()
while calling Load_Combat_Ship_(_combat_total_ship_count)
¿ something should be setting data that looks like `battle_units[]` ?





Tactical_Combat__WIP()
    |-> CMB_Units_Init__WIP()
        |-> Deploy_Battle_Units()
            |-> Undeployable_Battle_Units_On_Water()







selected unit vs. scanned unit
selected/active unit is shown in the "active unit window"
scanned unit is shown in the "combat unit display"
    ..."if you have toggled on the additional unit information button in your game settings window"





## CMB_SelectedUnit

XREF:  (112)
    Tactical_Combat__WIP()
    CMB_PrepareTurn__WIP()
    G_BU_SelectUnit()
    Assign_Combat_Grids()
    CMB_SetActionCursor__WIP()
    Tactical_Combat_Draw()
    CMB_DrawActiveUnitW()
    Tactical_Combat_Draw_Buttons()
    CMB_DrawAUWStats()
    Next_Battle_Unit()
    WIZ_BU_SelectClosest()
    AI_BU_ProcessAction()
    End_Of_Combat__WIP()
    Combat_Results_Scroll_Text()
    STK_CaptureCity()
    CMB_VortexPlayerMove()







## CMB_CursorBattleUnit

    CMB_SetTargetCursor()
        CMB_CursorBattleUnit = ST_UNDEFINED

    CMB_SetActionCursor__WIP()
        CMB_CursorBattleUnit = ST_UNDEFINED
        CMB_CursorBattleUnit = scanned_battle_unit_idx



## CMB_ActiveUnitFrame




## CMB_SetActionCursor__WIP()

CMB_ActiveUnitFrame = 0;
CMB_TargetFrame = 0;


GUI_CombatWindow.image_num
    GUI_CombatWindow.center_offset = 0;
    GUI_CombatWindow.x1 = SCREEN_XMIN;
    GUI_CombatWindow.y1 = SCREEN_YMIN;
    GUI_CombatWindow.x2 = SCREEN_XMAX;
    GUI_CombatWindow.y2 = SCREEN_YMAX;


            CMB_TargetFrame = 1;

            CMB_TargetFrame_X = Tile_X;

            CMB_TargetFrame_Y = Tile_Y;

            CMB_TargetFrameStage = ((CMB_TargetFrameStage + 1) % 3);





##### Naming Things is Hard

Combat unit display
a combat unit display
upper right corner of the combat screen
active unit

standard city walls
magic walls
a magic wall
Wall Crusher ability

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

Page 57  (PDF Page 62)

CITY ENCHANTMENTS AND CITY WALLS

Some specific city enchantments create magic walls around the city  
  (wall of darkness and wall of fire; see Spellbook supplement).  
Only one magic wall can encircle a city at any one time;  
  however, a city can have standard city walls (an option in the production screen; see Buildings)  
  in addition to a magic wall.  
The Wall Crusher ability that some units have only applies to the standard city walls,  
  not to magic ones (see Special Unit Abilities).


Page 85  (PDF Page 90)

Wall Crusher . . . . . . .  
Ability to destroy a section of stone wall 50% of the time if unit is adjacent to the target wall section.  
This probability falls to 25% if the target wall section is attacked at range.  
Units with this ability melee attack adjacent enemy units across wall sections that they destroy that turn.  
When conducting ranged attacks, wall crushers also attack an enemy unit that is in the targeted wall square.  
If the wall is not crushed, the defenders receive the wall’s full benefit against the attack.  
If the wall crumbles, they don’t.  


Page 98  (PDF Page 103)

STRUCTURES INFLUENCING COMBAT

City Walls

City walls prevent all nonflying creatures from entering a city except through the city gates.  
Ranged attacks can be used on garrisoned units cowering behind the walls, but nonflying defenders gain a +3 bonus to their defense (shields) against such attacks.  
Flyers cannot really cower as well and are more exposed to ranged attacks, so they only receive a defense bonus of one shield when "behind" a wall.  
Units that have the special ability Wall Crusher (see Special Unit Abilities) can systematically destroy city walls, allowing attackers greater access to any garrisoned units.  
In addition, nonflying defenders behind destroyed city wall sections only get a +1 bonus to their defense against ranged attacks.  
Note that both magic and city walls can exist concurrently.  
They also have different effects (see Building Types and Spellbook supplement).  
Magic walls may not be crushed; they may only be dispelled.  
