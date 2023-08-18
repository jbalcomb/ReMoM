
#include "MoX.H"

#include "LoadScr.H"

#include "MainScr.H"
#include "MainScr_Maps.H"



/*
    WIZARDS.EXE ovr160
*/

// WZD o160p01
void Load_Screen(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Load_Screen()\n", __FILE__, __LINE__);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Load_Screen()\n", __FILE__, __LINE__);
#endif
}

// WZD o160p02
void Load_Screen_Draw(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Load_Screen_Draw()\n", __FILE__, __LINE__);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Load_Screen_Draw()\n", __FILE__, __LINE__);
#endif
}

// WZD o160p03
void Load_Screen_Help(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Load_Screen_Help()\n", __FILE__, __LINE__);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Load_Screen_Help()\n", __FILE__, __LINE__);
#endif
}

// WZD o160p04
void Loaded_Game_Update(void)
{
    int16_t itr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Loaded_Game_Update()\n", __FILE__, __LINE__);
#endif

// DIFF DNE   WZD  s01p06  Loaded_Game_Update_WZD()
    // mov     [GAME_RazeCity], 0

    _human_player_idx = 0;

    PageFlipEffect = 0;
    
    // j_CTY_CatchmentRefresh();
    GAME_Overland_Init();
    // j_RP_GAME_UnitTypesUpdate();
    // j_LD_CTY_ResRefresh();
    // j_LD_MAP_TFUnk40_Eval();           ; not sure what this resource is or would have been, this function enumerates the first five tiles that have it, and records their coordinates
    // j_CTY_CheckMinFarmers();
    _unit_stack_count = 0;
    // j_SND_PlayBkgrndTrack();
    // j_GFX_Swap_Cities();
    // j_CTY_ResetRoadConns();
    // j_GAME_DeleteDeadUnits();
    // j_AI_ResetUnitMoves();

// DIFF DNE   WZD  s01p06  Loaded_Game_Update_WZD()
    // for(itr = 1; itr < NUM_PLAYERS; itr++)
    // {
    //     TBL_Wizards[itr].Avg_Unit_Value = 0;
    // }


    // j_GAME_NextHumanStack();
    // if(_difficulty = 0 /* "Intro" */) { magic_set.Random_Events = ST_FALSE; }
    // for(itr = 0; itr < 100; itr++)
    // {
    //     TBL_OvlMovePathsEMS[itr] = ST_UNDEFINED;
    // }
    // CRP_UNIT_OverlandPath = ST_UNDEFINED;
    // j_CONTX_CreateChains();
    // j_CONTX_CreateLChains();
    // for(itr = 0; itr < _num_players; itr++)
    // {
    //     AI_CONTX_Reevals[itr] = ST_FALSE;
    // }
    // g_TimeStop_PlayerNum = 0;
    // for(itr = 0; itr < _num_players; itr++)
    // {
    //     if(_players.Globals.Time_Stop == ST_FALSE)
    //     {
    //         g_TimeStop_PlayerNum = itr + 1;
    //     }
    // }
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Loaded_Game_Update()\n", __FILE__, __LINE__);
#endif
}


// WZD o51p01
// _main() |-> Loaded_Game_Update_WZD() |-> GAME_Overland_Init()
// Load_Screen |-> [WZD ovr160] Loaded_Game_Update() |-> GAME_Overland_Init()
/*
    Does this not feel like ~ Init Main Screen?
    There are a few oddities...
        city buildings
        staff lock flags
        nominal skill calc
        ! City Recalculate All - Main Screen needs that for the Resources Window
        GFX_Swap_Cities(), which has a terribly misleading name, loads other screens stuff
        ? G_WLD_StaticAssetRfrsh() Meh. More oddities...

*/
void GAME_Overland_Init(void)
{
    int16_t itr_cities;
    int16_t itr_units;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: GAME_Overland_Init()\n", __FILE__, __LINE__);
#endif


    G_WLD_StaticAssetRfrsh();


    itr_cities = 0;
    while(itr_cities++ < _cities)
    {
        _CITIES[itr_cities].buildings.None = 0; // ? ~ enum City Building Status B_Replaced;
    }


    itr_units = 0;
    while(itr_units++ < _units)
    {
        if(_UNITS[itr_units].world_plane == 2) {_UNITS[itr_units].world_plane = 0; }

        // NOTE: the DASM thinks world_plane is passed here as well, but IsPassableTower() makes no xref to it
        if(IsPassableTower(_UNITS[itr_units].world_x, _UNITS[itr_units].world_y) == ST_TRUE)
        {
            _UNITS[itr_units].In_Tower = ST_TRUE;
        }
        else
        {
            _UNITS[itr_units].In_Tower = ST_FALSE;
        }

    }


    // TODO  skill_staff_lock_flag = 0;
    // TODO  mana_staff_lock_flag = 0;
    // TODO  research_staff_lock_flag = 0;

    // _players[0].Nominal_Skill = Calc_Nominal_Skill(0);

    // NIU?  CRP_OVL_MapWindowX = 0;
    // NIU?  CRP_OVL_MapWindowY = 0;
    _prev_world_x = 0;
    _prev_world_y = 0;
    // _curr_world_x = 0;
    // _curr_world_y = 0;
    _map_x = 0;
    _map_y = 0;

    // j_RP_WIZ_ReturnZero(_human_player_idx)

    _unit = 0;  // 0: None / No Unit

    // TODO(JimBalcomb,20230629): validate the SAVE_GAM data for _FORTRESSES
    // // _active_world_x = _FORTRESSES[0].world_x;
    // // _active_world_y = _FORTRESSES[0].world_y;
    // OVL_Map_CenterX = _FORTRESSES[0].world_x;
    // OVL_Map_CenterY = _FORTRESSES[0].world_y;
    OVL_Map_CenterX = 24;
    OVL_Map_CenterY = 16;

    _unit_window_start_x = 247;
    _unit_window_start_y = 79;

    // TODO(JimBalcomb,20230629): validate the SAVE_GAM data for _FORTRESSES
    // _world_plane = _FORTRESSES[0].world_plane;
    // _map_plane = _FORTRESSES[0].world_plane;  // TODO(JimBalcomb,20230614): Why is this getting set to 100?
    _map_plane = 0;


    TILE_VisibilityUpdt();


    Allocate_Reduced_Map();

    // Center_Map(&_curr_world_x, &_curr_world_y, _FORTRESSES[0].world_x, _FORTRESSES[0].world_y, _world_plane);
    // TODO(JimBalcomb,20230629): validate the SAVE_GAM data for _FORTRESSES
    // Center_Map(&_map_x, &_map_y, _FORTRESSES[0].world_x, _FORTRESSES[0].world_y, _map_plane);
    Center_Map(&_map_x, &_map_y, 24, 16, 0);

    Set_Unit_Draw_Priority();
    Reset_Stack_Draw_Priority();
    Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);


    GFX_Swap_Cities();


    // TODO  j_WIZ_NextIdleStack(_human_player_idx, &_curr_world_x, &_curr_world_y, &_world_plane)
    WIZ_NextIdleStack(_human_player_idx, &_map_x, &_map_y, &_map_plane);


    // j_o108p02_Empty_pFxn()

    // j_CTY_RecalculateAll
    // ; calls CTY_Recalculate for all cities  ; (with all its BUGs)


    // TODO  if(Check_Release_Version() != ST_FALSE)
    // TODO  {
    // TODO      // WZD s22p05
    // TODO      RNG_SetSeed(0x2737, 0);  // LFSR_LO, LFSR_HI
    // TODO      // ; sets the linear feedback shift register
    // TODO  }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: GAME_Overland_Init()\n", __FILE__, __LINE__);
#endif
}

// WZD o51p02
// Loaded_Game_Update_WZD()  |-> GAME_Overland_Init()
// Loaded_Game_Update()    |-> GAME_Overland_Init()
// GAME_Overland_Init()  |-> G_WLD_StaticAssetRfrsh()
// NOTE: no XREFs to j_G_WLD_StaticAssetRfrsh()
void G_WLD_StaticAssetRfrsh(void)
{
    int16_t itr_cities;
    int16_t itr_players;

//    Randomize();

//    LFSR_HI = 0;
//    LFSR_LO = 0x03E8;
//    if(!Check_Release_Version())
//    {
//        RNG_SetSeed(LFSR_LO, LFSR_HI)
//    }

    all_units_moved = ST_FALSE;
    // G_OVL_MapVar4 = 1;  // ? ST_TRUE ?
    Reset_Draw_Active_Stack();
    _map_plane = 0;  // Arcanus

//    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
//    {
//        CTY_Recalculate(itr_cities);
//    }

//    for(itr_players = 0; itr_players < _num_players; itr_players++)
//    {
//        WIZ_RefreshResearch(itr_players);
//    }

//    _WIZ_SetPowerBases();

//    SBK_SomePageSaveVar = 0;
//    CMB_SpellBookPage = 0;
//    SBK_Candidate_Page = 0;

}
