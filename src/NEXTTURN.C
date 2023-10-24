/*
    Next Turn Procedure

    WIZARDS.EXE
        ovr060
        ovr119
*/

#include "MoM.H"
#include "NEXTTURN.H"



/*
    WIZARDS.EXE  ovr060
*/

// WZD o60p01
// WZD o60p02
// WZD o60p03


// WZD o60p04
void Next_Turn_Proc(void)
{
    
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Next_Turn_Proc()\n", __FILE__, __LINE__);
#endif

    // TODO  GAME_DeleteDeadUnits();
    // TODO  GAME_InTowerRefresh();

    Set_Unit_Draw_Priority();
    Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);
    Reset_Map_Draw();

    // TODO  IDK_EoT_s35680();

    GFX_Swap_Cities();

    // TODO  GAME_DeleteDeadUnits();
    // TODO  GAME_InTowerRefresh()

    if(magic_set.EoT_Summary == ST_TRUE)
    {
        // TODO  IDK_Chancellor_EoTSummary_EventsAnimScroll();
    }



    /*
        BEGIN: Messages
    */



    /*
        END: Messages
    */



// @@Done_WrapItUp
    current_screen = scr_Main_Screen;

    // TODO  MSG_BuildDone_Count = 0;
    // TODO  sub_52F3C();
    // DONT  o62p01_Empty_pFxn(_human_player_idx);
    // DONT  fxnptr_o59p();

    if( (_players[_human_player_idx].Cast_Cost_Left <= 0) && (_players[_human_player_idx].Spell_Cast > 0) )
    {
        // TODO  G_OVL_Cast(_human_player_idx);
    }

// @@Done_Done
    all_units_moved = ST_FALSE;
    WIZ_NextIdleStack(_human_player_idx, &_map_x, &_map_y, &_map_plane);
    Reset_Draw_Active_Stack();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Next_Turn_Proc()\n", __FILE__, __LINE__);
#endif
}


// WZD o60p05
// WZD o60p06
// WZD o60p07



/*
    WIZARDS.EXE  ovr119
*/

// WZD o119p01
void Next_Turn_Calc(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Next_Turn_Calc()\n", __FILE__, __LINE__);
#endif





#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Next_Turn_Calc()\n", __FILE__, __LINE__);
#endif

}

// WZD o119p02

// WZD o119p03
