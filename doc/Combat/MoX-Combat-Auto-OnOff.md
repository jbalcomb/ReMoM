


Combat.c
Combat_Screen__WIP()

AI_CMB_PlayTurn__WIP()

CMB_ProgressTurnFlow__WIP()




## Combat_Screen__WIP()

    while(leave_screen == ST_FALSE)
        BEGIN: Auto Combat
        if(_auto_combat_flag == ST_TRUE)
            auto_combat_cancel_ESC_field = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, str_hotkey_ESC__ovr090[0], ST_UNDEFINED);
            input_field_idx = Get_Input();
            if(input_field_idx == auto_combat_cancel_ESC_field)  /* turn off 'Auto Combat' */
                Play_Left_Click();
                _auto_combat_flag = ST_FALSE;
                CMB_AIGoesFirst = ST_FALSE;
                CRP_CMB_NeverChecked1 = ST_TRUE;
                CMB_ProgressTurnFlow__WIP();
                Turn_Off_Auto_Combat();
                CRP_CMB_NeverChecked1 = ST_TRUE;
                CMB_ImmobileCanAct = ST_FALSE;
            else  /* do 'Auto Combat' */
                CMB_ProgressTurnFlow__WIP();
            Combat_Winner = Check_For_Winner__WIP();
            if(Combat_Winner != ST_UNDEFINED)
                leave_screen = ST_UNDEFINED;
                input_field_idx = ST_UNDEFINED;
                continue;
        END: Auto Combat
        
        auto_button_field = Add_Button_Field(170, 178, str_empty_string__ovr090, _cmbt_auto_button_seg, cnst_HOTKEY_A_4[0], ST_UNDEFINED);
        BEGIN:  Left-Click Auto Button
        if(input_field_idx == auto_button_field)
            Play_Left_Click();
            Set_Mouse_List(1, mouse_list_default);
            frame_active_flag = 0;
            frame_scanned_flag = 0;
            TOGGLE(_auto_combat_flag);
            CMB_HumanUnitsDone = ST_FALSE;
            Clear_Fields();
            input_field_idx = 0;
            AI_CMB_PlayTurn__WIP(combat_human_player);
            Combat_Winner = Check_For_Winner__WIP();
            if(Combat_Winner == ST_UNDEFINED)
                leave_screen = ST_UNDEFINED;
                input_field_idx = 0;
        END:  Left-Click Auto Button



## CMB_ProgressTurnFlow__WIP()


## AI_CMB_PlayTurn__WIP()
    if(player_idx == combat_human_player)
        // SPELLY  G_CMB_CastSpell((player_idx + 20), _combat_wx, _combat_wy, _combat_wp);
    Combat_Winner = Check_For_Winner__WIP();  // ¿ because spell cast may resulted in a win/loss ?
    if(Combat_Winner == ST_UNDEFINED)
        CMB_CE_Refresh__WIP();  // ¿ because spell cast may been an enchantment ?
        AI_MoveBattleUnits__WIP(player_idx);

## AI_MoveBattleUnits__WIP()



## Turn_Off_Auto_Combat()
    _auto_combat_flag = ST_FALSE;
    Next_Battle_Unit(_human_player_idx);
    Assign_Combat_Grids();
