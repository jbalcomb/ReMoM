
MAGIC.EXE
ovr050
ovr051
ovr056

main()
Main_Menu_Control()
    main_menu_selection = Main_Menu_Screen()
    switch(main_menu_selection)
    case 2:  scr_New_Game
    Newgame_Control()
        Newgame_Screen0__WIP()
        Newgame_Screen1__WIP()
        Newgame_Screen2__WIP()
        Newgame_Screen3__WIP()
        Newgame_Screen5__WIP()
        Newgame_Screen6__WIP()
        Newgame_Screen7__WIP()
        if(Can_Create != ST_FALSE)
            Init_New_Game__WIP()
            NEWG_ClearEvents__WIP()
            NEWG_FinalizeTables__WIP()
            Save_SAVE_GAM(8)
            GAME_WizardsLaunch__WIP(8)



Init_New_Game()
    |-> Init_Computer_Players()
        |-> Init_Computer_Players_Wizard_Profile()
        |-> Init_Computer_Players_Spell_Library()




    // ¿ MoO2  Module: HOMEGEN  Generate_Home_Worlds_() ?
    // ¿ MoO2  Module: INITGAME  Init_New_Game_() ?
    Init_New_Game();
    Initialize_Events();
    NEWG_FinalizeTables__WIP();  // ... wrap it up ... 100%!
    Save_SAVE_GAM(8);
    GAME_WizardsLaunch__WIP(8);




MGC  dseg:89C6 00 00 00 00                                     TBL_Scouting dd 0                       ; DATA XREF: Save_SAVE_GAM+304r ...

WZD  dseg:9CB0 00 00 00 00                                     _square_explored dd 0                   ; DATA XREF: Save_SAVE_GAM+304r ...







Universe_Generation_()
Generate_Home_Worlds_()




000004B0 peace_duration db 6 dup(?)
000004B6 Unknown_4B6h dw ?
000004B8 Unknown_4B8h dw ?
000004BA Unknown_4BAh dw ?
000004BC Unknown_4BCh dw ?
000004BE Unknown_4BEh db 6 dup(?)

AI_WIZ_StrategyReset()





## Init_New_Game__WIP()




## GAME_WizardsLaunch()
    Fade_Out();
    Stop_Music__STUB();
    Audio_Uninit__STUB();
    if(save_gam_idx != 8)
        Load_SAVE_GAM(save_gam_idx);
        Save_SAVE_GAM(8);
    GAME_EXE_Swap(cnst_EXESwap_File, cnst_EXESwap_Arg, cnst_EXESwap_Arg, empty_string__ovr050);



## NEWG_FinalizeTables()
    NEWG_UpdateProgress(100);
    NEWG_ClearItems();
    NEWG_CreateHeroes();
    WIZ_SetStartingStats();
    AI_WIZ_StrategyReset();
    WIZ_SetPersonalities();
    WIZ_Set_Sum_n_SoM();
    CRP_MSG_Clear();
    for(itr_players = 0; itr_players < _num_players; itr_players++)
        _players[itr_players].capital_race = _CITIES[itr_players].race;
    AI_WIZ_SetRelations();
    _players[HUMAN_PLAYER_IDX].gold_reserve = ((5 - _difficulty) * 25);
    for(itr_players = 1; itr_players < _num_players; itr_players++)
        _players[itr_players].gold_reserve = 150;
        Wiz5_Spell_E0h[itr_players] = (80 + Random(40));
