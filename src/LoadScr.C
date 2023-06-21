
#include "MoX_TYPE.H"

#include "MoX_Data.H"
#include "MoM_main.H"

#include "LoadScr.H"




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
    ; int16_t itr;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Loaded_Game_Update()\n", __FILE__, __LINE__);
#endif
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
