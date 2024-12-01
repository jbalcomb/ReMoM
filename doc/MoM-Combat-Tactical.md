
MoM / Mox
Tactical Combat



Active Unit Window
Spell Information Window
Defender Enchantment Window
Attacker Enchantment Window
Combat Unit Display
Combat Information Window










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
    CMB_FillTargetMaps__WIP()
    CMB_SetActionCursor__WIP()
    Tactical_Combat_Draw()
    CMB_DrawActiveUnitW()
    Tactical_Combat_Draw_Buttons()
    CMB_DrawAUWStats()
    WIZ_BU_SelectNext__WIP()
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



