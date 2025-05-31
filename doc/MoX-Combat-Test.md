


drop-in replacements for Tactical_Combat__WIP()
Combat_Screen_TST_001()

Combat__WIP()
    Battle_Outcome = Tactical_Combat__WIP(combat_attacker_player_idx, defender_idx, troops, troop_count, _combat_wx, _combat_wy, _combat_wp, &Item_Count, &Item_List[0]);

Combat_Screen_TST_001(); Combat_Screen_TST_001_Draw;
    ...absolute bare minimum...

Combat_Screen_TST_002(); Combat_Screen_TST_002_Draw;
    ...complete base...

Combat_Screen_TST_003(); Combat_Screen_TST_003_Draw;
    ...terrain...

Combat_Screen_TST_004(); Combat_Screen_TST_004_Draw;
    ...units...



...absolute bare minimum...
...complete base...
    city name, player name, monsters/raiders, 
...terrain...
...units...
...structure...
...animations...



...enchantment windows...  (attacker, defender)
...active unit window...
...combat unit window...  (Settings::"Auto Unit Information")











CMB_ComposeBackgrnd__WIP()  ==>  Combat_Screen_Compose_Background()

CMB_Terrain_Init__WIP()
Load_Combat_Terrain_Pictures()
Generate_Combat_Map__WIP()

// WZD ovr154p02
// drake178: CMB_GenerateMap()
    CMB_TileGen__WIP()
    Set_Movement_Cost_Maps()



```c
Screen()
{
    leave_screen = ST_FALSE;
    while(leave_screen == ST_FALSE)
    {
        Mark_Time();
        if(leave_screen == ST_FALSE)
        {
            Screen_Draw();
            PageFlip_FX();
            Release_Time(1);
        }
    }
}
```


## CMB_ComposeBackgrnd__WIP()
    Set_Page_Off();
    Reset_Window();
    EMM_TILEX_Init__HACK();
        Clipped_Draw(screen_x, screen_y, _combat_terrain_pict_segs[combat_terrain_type]);
    FLIC_Draw(0, 164, combat_background_bottom);
    Copy_Off_To_Back();
...where is Copy_Back_To_Off()?




...data and resources...

Load_WZD_Resources()
    |-> Load_Combat_Background_Bottom(void)
            combat_background_bottom = LBX_Load(backgrnd_lbx_file, 3);

CMB_BaseAllocs__WIP()
CMB_LoadResources__WIP()
    |-> CMB_SetNearAllocs__WIP()

    Cache_Graphics_Combat();


    CMB_Terrain_Init__WIP(wx, wy, wp);


    Defending_Unit_Count = CMB_Units_Init__WIP(troop_count, troops);


OVL_Action_Type

AI_ImmobileCounter = 0;
CMB_WizardCitySiege = ST_FALSE;
CMB_AI_Fled = ST_FALSE;  // ; set to 1 if the AI decides to flee
Player_Fled = ST_FALSE;
    _combat_wx = wx;
    _combat_wy = wy;
    _combat_wp = wp;
    _combat_attacker_player = combat_attacker_player_idx;
    _combat_defender_player = combat_defender_player_idx;
    CMB_ATKR_First_CE = 0;
    CMB_DEFR_First_CE = 0;
    CMB_combat_structure = Combat_Structure(wx, wy, wp, 0);
    CMB_CE_Refresh__WIP();
    Combat_Node_Type();
    CMB_HumanUnitsDone = ST_FALSE;
    _combat_turn = 0;
    CMB_WizCastAvailable = ST_TRUE;
    _combat_total_battle_effect_count = Combat_Info_Effects_Count();
    Init_Battlefield_Effects(CMB_combat_structure);
    Combat_Cache_Write();
    CMB_ImmobileCanAct = ST_FALSE;
    G_AI_StayInTownProper = ST_TRUE;
    _scanned_battle_unit = ST_UNDEFINED;
    frame_active_flag = 0;
    frame_scanned_flag = 0;
    CRP_CMB_NeverChecked1 = ST_TRUE;
    CMB_CityDamage = ST_FALSE;
    CMB_HumanTurn = ST_TRUE;
    _auto_combat_flag = ST_FALSE;
    CMB_PrepareTurn__WIP();
    CMB_ImmobileCanAct = ST_FALSE;

    Assign_Combat_Grids();

    CMB_CE_Refresh__WIP();

    CRP_CMB_NeverChecked1 = ST_TRUE;
