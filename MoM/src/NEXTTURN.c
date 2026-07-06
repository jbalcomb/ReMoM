/**
 * @file NEXTTURN.c
 * @brief Implements the main strategic next-turn update pipeline.
 *
 * Coordinates the end-of-turn processing for units, cities, players, spell
 * casting, research, diplomacy, AI maintenance, healing, and autosave.
 */
/*
    Next Turn Procedure

    WIZARDS.EXE
        ovr060
        ovr119
        ovr121
        ovr140
        ovr141
*/

#include "../../STU/src/STU_DBG.h"
#include "../../STU/src/STU_LOG.h"

#include "../../platform/include/Platform.h"  /* Platform_Get_Millies() */

#include "../../MoX/src/LOADSAVE.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_DAT.h"  /* _screen_seg */
#include "../../MoX/src/MOX_SET.h"  /* magic_set */
#include "../../MoX/src/MOX_TYPE.h"
#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/GENDRAW.h"
#include "../../MoX/src/LBX_Load.h"
#include "../../MoX/src/SOUND.h"
#include "../../MoX/src/Util.h"       /* Clear_Structure(); Delete_Structure() */
#include "../../MoX/src/random.h"

#include "AIBUILD.h"
#include "AIDUDES.h"
#include "AISPELL.h"
#include "CITYCALC.h"
#include "City_ovr55.h"
#include "Combat.h"
#include "EVENTS.h"
#include "Explore.h"
#include "HIRE.h"
#include "ItemMake.h"
#include "Items.h"
#include "LOADER.h"
#include "Lair.h"
#include "OverSpel.h"
#include "DIPLOMAC.h"
#include "LVLMAKE.h"
#include "CityScr.h"  /* City_Screen__WIP(); */
#include "MainScr.h"
#include "MainScr_Maps.h"
#include "MOM_DBG.h"
#include "MOM_SCR.h"
#include "NEXTTURN.h"
#include "RACETYPE.h"
#include "REPORT.h"
#include "Spellbook.h"
#include "Spells128.h"
#include "Spells129.h"
#include "Spells130.h"
#include "TerrType.h"
#include "Terrain.h"
#include "UNITSTK.h"
#include "UNITTYPE.h"
#include "UnitView.h"
#include "WZD_o059.h"
#include "WZD_o143.h"

#include "../../ext/stu_compat.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "Spells132.h"



// WZD dseg:5E92                                                 BEGIN:  ovr119 - Initialized Data

// WZD dseg:5E92
uint32_t RNG_AI_Turn_Seed = 0x2A57;  // 10839d  10101001010111b

// WZD dseg:5E92                                                 END:  ovr119 - Initialized Data



// WZD dseg:5E96                                                 ¿ BEGIN:  ovr121 - Strings ?

// WZD dseg:5E96
// message_lbx_file__121[] = "message";
char message_lbx_file[] = "message";

// WZD dseg:5E96                                                 ¿ END:  ovr121 - Strings ?              ; should use dseg:5aea



// WZD dseg:6E4C                                                 ¿ BEGIN:  ovr140 - Strings ?

// WZD dseg:6E4C
// message_lbx_file__ovr140[] = "message";

// WZD dseg:6E54
char cnst_TooManyUnits[] = ". You must disband some units if you wish to build or summon any more.";
// WZD dseg:6E9B 00                                              db    0
// WZD dseg:6E9C 00                                              db    0
// WZD dseg:6E9D 00                                              db    0

// WZD dseg:6E9D                                                 ¿ END:  ovr140 - Strings ?



// WZD dseg:CA66                                                 BEGIN: ovr140 - Uninitialized Data

// WZD dseg:CA66
/*
heal_points = max_hp / divisor
*/
SAMB_ptr unit_heal_rate_divisors;

// WZD dseg:CA6A 00 00                                           dw 0
// WZD dseg:CA6C 00 00                                           dw 0
// WZD dseg:CA6E 00 00                                           dw 0
// WZD dseg:CA70 00 00                                           dw 0
// WZD dseg:CA72 00 00                                           dw 0

// WZD dseg:CA72                                                 END: ovr140 - Uninitialized Data



// WZD dseg:CA74                                                 BEGIN:  ovr141 - Uninitialized Data

// WZD dseg:CA74
int16_t IDK_autosave;

// WZD dseg:CA74                                                 END:  ovr141 - Uninitialized Data



/*
    WIZARDS.EXE  ovr060
*/

// WZD o60p01
// ~ MoO2  Module: SHIPSTAK  Remove_Non_Detected_Ships_()  Delete_Ship_Stack_()  Delete_Ship_Node_()
/**
 * @brief Removes dead/dismissed units from the global unit table and compacts indices.
 *
 * @details
 * This routine scans @c _UNITS[] and deletes entries whose owner index is outside
 * the valid active-owner range (human through neutral). Such entries represent
 * units previously marked for removal (for example by @c Kill_Unit()).
 *
 * For each removed unit, the function compacts the unit array via
 * @c Delete_Structure(), decrements @c _units, and then fixes hero references:
 * every hero slot in every player whose @c unit_idx is greater than the deleted
 * position is decremented by one to stay aligned with shifted unit indices.
 *
 * Iteration uses a while-loop with in-place compaction semantics:
 * when a deletion occurs, the same index is re-checked (no increment) so the
 * unit shifted into that slot is processed correctly.
 *
 * @return This function returns no value.
 *
 * @note A unit is considered removable when
 *       @c owner_idx < HUMAN_PLAYER_IDX or @c owner_idx > NEUTRAL_PLAYER_IDX.
 * @note Hero references equal to @c ST_UNDEFINED are ignored during index-fixup.
 * @note The local @c unit_type variable is currently read but not used for
 *       decision-making; behavior is driven by owner index bounds.
 * @note This function mutates global structures: @c _UNITS[], @c _units,
 *       and @c _players[].Heroes[].unit_idx.
 *
 * @see Kill_Unit(), Delete_Structure(), Next_Turn_Proc(), Next_Turn_Calc()
 */
void Delete_Dead_Units(void)
{
    int16_t unit_type = 0;
    int16_t itr_heroes = 0;
    int16_t itr_units = 0;
    int16_t itr_players = 0;

    itr_units = 0;
    while(itr_units < _units)
    {
        unit_type = _UNITS[itr_units].type;
        if(
            (_UNITS[itr_units].owner_idx < HUMAN_PLAYER_IDX)
            ||
            (_UNITS[itr_units].owner_idx > NEUTRAL_PLAYER_IDX)
        )
        {
            Delete_Structure(itr_units, (uint8_t *)&_UNITS[0], sizeof(struct s_UNIT), _units);
            /* Decrement the unit index for any owned heroes whose unit index is greater than the deleted unit's index */
            for(itr_players = 0; itr_players < _num_players; itr_players++)
                for(itr_heroes = 0; itr_heroes < NUM_HEROES; itr_heroes++)
                    if((_players[itr_players].Heroes[itr_heroes].unit_idx != ST_UNDEFINED) && (_players[itr_players].Heroes[itr_heroes].unit_idx > itr_units))
                        _players[itr_players].Heroes[itr_heroes].unit_idx -= 1;
            _units -= 1;
            continue;  /* Re-check the unit that shifted into this slot. */
        }
        itr_units++;
    }
}


// WZD o60p02
/*
    returns the maximum scout range of all _units[] at wx, wy, wp

XREF:
    j_IDK_City_Radius_s34255()
        Do_Plane_Shift()
        EarthGateTeleport__WIP()
        Cast_PlaneShift()
*/
int16_t Player_Units_Max_Range(int16_t player_idx, int16_t wx, int16_t wy, int16_t wp)
{
    int16_t range;
    int16_t itr_units;  // _DI_

    range = 1;

    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(_UNITS[itr_units].owner_idx == player_idx)
        {
            if(
                (_UNITS[itr_units].wp == wp)
                ||
                (_UNITS[itr_units].in_tower == ST_TRUE)
            )
            {
                if(
                    (_UNITS[itr_units].wx == wx)
                    &&
                    (_UNITS[itr_units].wy == wy)
                )
                {
                    if(_UNITS[itr_units].Sight_Range > range)
                    {
                        range = _UNITS[itr_units].Sight_Range;
                    }
                }
            }
        }
    }

    return range;
}


// WZD o60p03
/*
GARBAGE
XREF:
    NX_j_o060p03_empty_function()
*/
static void o060p03_empty_function(void)
{
/*
push    bp
mov     bp, sp
pop     bp
retf
*/
}


// WZD o60p04
void gd_dump_units(const char* point);   /* CLAUDE: GD capture (defined in INITGAME.c) */

/* CLAUDE DEBUG: scan for the 3 neutral defenders (cities 9/13/17) by (wx,wy,type=104)
 * and log owner_idx/wp -- to find where they get killed (owner -> -1) between
 * Next_Turn_Calc and the 2nd Delete_Dead_Units.  Match on coords+type only: Kill_Unit
 * zaps owner_idx & wp but leaves wx/wy, so a just-killed defender is still found. */
static void dbg_scan_defenders(const char* where)
{
    int16_t i;
    int16_t found = 0;
    for(i = 0; i < _units; i++)
    {
        if(_UNITS[i].type == 104 &&
           ( (_UNITS[i].wx == 29 && _UNITS[i].wy == 32) ||
             (_UNITS[i].wx == 41 && _UNITS[i].wy == 12) ||
             (_UNITS[i].wx == 24 && _UNITS[i].wy == 22) ))
        {
            LOG_DEBUG(LOG_CAT_GENERAL, "[DBG-KILL] %-28s _UNITS[%d] (%d,%d) type=%d owner=%d wp=%d",
                where, (int)i, (int)_UNITS[i].wx, (int)_UNITS[i].wy, (int)_UNITS[i].type,
                (int)_UNITS[i].owner_idx, (int)_UNITS[i].wp);
            found++;
        }
    }
    LOG_DEBUG(LOG_CAT_GENERAL, "[DBG-KILL] %-28s defenders_found=%d _units=%d", where, (int)found, (int)_units);
}

void Next_Turn_Proc(void)
{
    char temp_string[LEN_TEMP_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t orig_map_plane = 0;
    int16_t itr_msg = 0;
    int16_t curr_prod_idx = 0;
    int16_t DBG_itr_cities = 0;

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);


    Delete_Dead_Units();
    /* CLAUDE: GD 619 -- _UNITS after Next_Turn_Proc's 1st Delete_Dead_Units (before
     * All_Units_In_Towers), matching OG's j_All_Units_In_Towers landmark #1.  Fire once. */
    { static int gd624_done = 0; if(!gd624_done) { gd624_done = 1; gd_dump_units("619_Delete_Dead_Units_NextTurnProc_1_U"); } }
    All_Units_In_Towers();


    Set_Unit_Draw_Priority();
    Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);
    Reset_Map_Draw();


    Next_Turn_Calc();
    dbg_scan_defenders("B after Next_Turn_Calc");


    Cache_Graphics_Overland();
    dbg_scan_defenders("C after Cache_Graphics");


    Delete_Dead_Units();
    dbg_scan_defenders("D after Delete_Dead_Units_2");
    /* CLAUDE: GD 634 -- _UNITS after Next_Turn_Proc's 2nd Delete_Dead_Units (before
     * All_Units_In_Towers), matching OG's j_All_Units_In_Towers landmark #2.  Fire once. */
    { static int gd626_done = 0; if(!gd626_done) { gd626_done = 1; gd_dump_units("634_Delete_Dead_Units_NextTurnProc_2_U"); } }
    All_Units_In_Towers();



    // OG-MoM does not show/wait/do anything here if there are no message
    if(magic_set.end_of_turn_summary == ST_TRUE)
    {

        Chancellor_Screen__WIP(0);

    }



    /*
        BEGIN: Messages
    */
    if(g_bldg_msg_ctr > 0)
    {

        LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: Next_Turn_Proc(): if(g_bldg_msg_ctr > 0)", __FILE__, __LINE__);

        o62p01_empty_function(_human_player_idx);

        for(itr_msg = 0; itr_msg < g_bldg_msg_ctr; itr_msg++)
        {
            if(MSG_Building_Complete[itr_msg].city_idx != ST_UNDEFINED)
            {

                LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: Next_Turn_Proc(): if(MSG_Building_Complete[itr_msg].city_idx != ST_UNDEFINED)", __FILE__, __LINE__);

                _city_idx = MSG_Building_Complete[itr_msg].city_idx;
                orig_map_plane = _map_plane;
                _map_plane = _CITIES[_city_idx].wp;
                if(MSG_Building_Complete[itr_msg].bldg_type_idx >= bt_NONE)
                {

                    LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: Next_Turn_Proc(): if(MSG_Building_Complete[itr_msg].bldg_type_idx >= bt_NONE)", __FILE__, __LINE__);

                    city_built_bldg_idx = MSG_Building_Complete[itr_msg].bldg_type_idx;
                    Center_Map(&_map_x, &_map_y, _CITIES[_city_idx].wx, _CITIES[_city_idx].wy, _map_plane);
                    City_Built_Building_Message(5, 101, _city_idx, city_built_bldg_idx);
                }
                else  /* (MSG_Building_Complete[itr_msg].bldg_type_idx < bt_NONE) */
                {
                    if(_CITIES[_city_idx].construction == bt_AUTOBUILD)
                    {
                        _CITIES[_city_idx].construction = bt_Housing;
                    }
                    else
                    {

                        LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: Next_Turn_Proc(): (MSG_Building_Complete[itr_msg].bldg_type_idx < bt_NONE)", __FILE__, __LINE__);

                        stu_strcpy(GUI_NearMsgString, "The ");
                        stu_strcat(GUI_NearMsgString, _city_size_names[_CITIES[_city_idx].size]);
                        stu_strcat(GUI_NearMsgString, " of ");
                        stu_strcpy(near_buffer, _CITIES[_city_idx].name);
                        stu_strcat(GUI_NearMsgString, near_buffer);
                        stu_strcat(GUI_NearMsgString, " can no longer produce ");
                        curr_prod_idx = _CITIES[_city_idx].construction;
                        if(curr_prod_idx >= 100)
                        {
                            curr_prod_idx -= 100;
                            stu_strcat(GUI_NearMsgString, *_unit_type_table[curr_prod_idx].name);
                        }
                        else
                        {
                            stu_strcpy(temp_string, bldg_data_table[curr_prod_idx].name);
                            stu_strcat(GUI_NearMsgString, An(&temp_string[0]));
                            stu_strcat(GUI_NearMsgString, " ");
                            stu_strcat(GUI_NearMsgString, temp_string);
                        }
                        stu_strcat(GUI_NearMsgString, ".");
                        Warn0(GUI_NearMsgString);
                    }
                }

                current_screen = scr_City_Screen;
                City_Screen__WIP();
                Set_Draw_Active_Stack_Always();
                _map_plane = orig_map_plane;
                MSG_Building_Complete[itr_msg].city_idx = -1;
            }
        }
    }
    /*
        END: Messages
    */

    current_screen = scr_Main_Screen;

    g_bldg_msg_ctr = 0;



    Update_Units_MvsSts();

    o62p01_empty_function(_human_player_idx);

    o59p10_empty_function();

#ifdef STU_DEBUG
    _players[HUMAN_PLAYER_IDX].casting_cost_remaining = 0;
#endif

    if(
        (_players[_human_player_idx].casting_cost_remaining <= 0)
        &&
        (_players[_human_player_idx].casting_spell_idx > spl_NONE))
    {

        Cast_Spell_Overland(_human_player_idx);

    }


    all_units_moved = ST_FALSE;

    WIZ_NextIdleStack(_human_player_idx, &_map_x, &_map_y, &_map_plane);

    Reset_Draw_Active_Stack();


    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

}


// WZD o60p05
// drake178: AI_ResetUnitMoves()
/*
marks all AI units that are on patrol, building a
road, or have the unknown status 6 as finished; marks
all AI units that are waiting or have reached their
destinations as ready; resets the movement allowance
of all AI units; and updates the visibility
*/
/*

sets Finished, moves2_max, moves2
handles Statis
calls All_AI_Players_Contacted(), which calls Update_Scouted_And_Contacted()

XREF
    j_All_AI_Refresh_Units_Movement()
        Loaded_Game_Update_WZD()
        Loaded_Game_Update()

*/
void All_AI_Refresh_Units_Movement(void)
{
    int16_t itr_units = 0;  // _SI_

    for(itr_units = 0; itr_units < _units; itr_units++)
    {

        if(_UNITS[itr_units].owner_idx != _human_player_idx)  // ¿ current player ?
        {

            _UNITS[itr_units].Finished = ST_FALSE;

            if(
                (_UNITS[itr_units].Status == us_Patrol)
                ||
                (_UNITS[itr_units].Status == us_BuildRoad)
                ||
                (_UNITS[itr_units].Status == us_Casting)
            )
            {

                _UNITS[itr_units].Finished = ST_TRUE;

            }

            if(
                (_UNITS[itr_units].Status == us_Wait)
                ||
                (_UNITS[itr_units].Status == us_ReachedDest)
                ||
                (_UNITS[itr_units].Status == us_Ready)
            )
            {

                _UNITS[itr_units].moves2_max = (int8_t)Unit_Moves2(itr_units);

                _UNITS[itr_units].moves2 = _UNITS[itr_units].moves2_max;

                if((_UNITS[itr_units].mutations & (C_STASISINIT | C_STASISLINGER)) != 0)
                {

                    _UNITS[itr_units].Finished = ST_TRUE;

                }

            }

        }

    }

    All_AI_Players_Contacted();

}


// WZD o60p06
// ¿ AI version ?
/*
updates Finished, Status, moves2_max, moves2
*/
void Update_Units_MvsSts(void)
{
    int16_t itr_units = 0;

    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        // default all to 'orderable'
        _UNITS[itr_units].Finished = ST_FALSE;

        // if *busy* doing "PATROL", "BUILD", or "CASTING", (re-)set to 'unorderable'
        if(
            (_UNITS[itr_units].Status == us_Patrol) ||
            (_UNITS[itr_units].Status == us_BuildRoad) ||
            (_UNITS[itr_units].Status == us_Casting)
        )
        {
            _UNITS[itr_units].Finished = ST_TRUE;
        }

        if((_UNITS[itr_units].Status == us_Wait) || (_UNITS[itr_units].Status == us_ReachedDest))
        {
            _UNITS[itr_units].Status = us_Ready;
        }

        _UNITS[itr_units].moves2_max = (int8_t)Unit_Moves2(itr_units);

        _UNITS[itr_units].moves2 = _UNITS[itr_units].moves2_max;

    }

    All_AI_Players_Contacted();

}


// WZD o60p07
// drake178: TILE_VisibilityUpdt()
// ¿ MoO2  Module: CONTACTS  Compute_Contacts_() ?
/*
marks all AI wizards as having contacted the human
player (one-sided), processes Nature Awareness
contacting wizards whose units are visible, and calls
TILE_VisibilityReset to refresh the bit maps

Invisibility BUG: only checks the enchantment in the
Nature Awareness loop, ignoring both natural ability
and item power
*/
/*

Cast_NaturesAwareness()
    |-> Cheat_Reveal()
    |-> All_AI_Players_Contacted()

*/
void All_AI_Players_Contacted(void)
{
    int16_t itr_players = 0;
    int16_t itr_units = 0;

    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {

        /* Mark self as contacted. (Why?)*/
        _players[itr_players].Dipl.Contacted[itr_players] = ST_TRUE;

        if(_players[itr_players].Globals[NATURES_AWARENESS] != ST_FALSE)
        {

            for(itr_units = 0; itr_units < _units; itr_units++)
            {

                if(
                    (_UNITS[itr_units].owner_idx != ST_UNDEFINED)
                    &&
                    (_UNITS[itr_units].owner_idx != itr_players)
                    &&
                    (_UNITS[itr_units].owner_idx != NEUTRAL_PLAYER_IDX)
                    &&
                    ((_UNITS[itr_units].enchantments & UE_INVISIBILITY) == 0)  /* OGBUG: only checks enchantment, not ability or item */
                )
                {

                    _players[itr_players].Dipl.Contacted[_UNITS[itr_units].owner_idx] = ST_TRUE;

                    _players[_UNITS[itr_units].owner_idx].Dipl.Contacted[itr_players] = ST_TRUE;

                }

            }

        }

    }

    Update_Scouted_And_Contacted();
    // TST  Validate_Square_Scouted(18,11,0);

}



/*
    WIZARDS.EXE  ovr119
*/

// WZD o119p01
void gd_dump_units(const char* point);   /* CLAUDE: GD capture (defined in INITGAME.c) */

void Next_Turn_Calc(void)
{
    int16_t itr_players = 0;
    int16_t DBG_itr_cities = 0;
#ifdef STU_DEBUG
    uint64_t ntc_start_ms = Platform_Get_Millies();
#endif

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

/* CLAUDE */ #ifdef STU_DEBUG
/* CLAUDE */ #define PHASE(CALL) do { uint64_t _ps = Platform_Get_Millies(); CALL; { uint64_t _pe = Platform_Get_Millies(); LOG_INFO(LOG_CAT_NEXTTURN, "[NEXTTURN] phase %-48s = %llu ms", #CALL, (unsigned long long)(_pe - _ps)); LOG_TRACE(LOG_CAT_GENERAL, "[NEXTTURN] phase %-48s = %llu ms", #CALL, (unsigned long long)(_pe - _ps)); } } while(0)
/* CLAUDE */ #else
/* CLAUDE */ #define PHASE(CALL) CALL
/* CLAUDE */ #endif


    Set_Random_Seed(RNG_AI_Turn_Seed);

    Set_Mouse_List(1, mouse_list_hourglass);

    // All_City_Calculations();
/* CLAUDE */ PHASE(All_City_Calculations());

    // AI_Kill_Lame_Units();  // ¿ BUGBUG  leaves dead/deleteable units lying around ?
/* CLAUDE */ PHASE(AI_Kill_Lame_Units());

    // AI_Next_Turn();
/* CLAUDE */ PHASE(AI_Next_Turn());
    /* CLAUDE: GD point 618 -- _UNITS AFTER AI_Next_Turn (captured at the call site,
     * BEFORE the following Next_Turn_Process_Purify(), mirroring OG's far-return into
     * Next_Turn_Calc).  Tells whether the divergent neutral units (_UNITS[48..50], owner
     * 5 / type 104 in OG, absent in ReMoM) exist by the end of the AI turn or are created
     * in a later Next_Turn_Calc phase.  Fire once. */
    { static int gd618_done = 0; if(!gd618_done) { gd618_done = 1; gd_dump_units("618_AI_Next_Turn_Return_U"); } }

    // Next_Turn_Process_Purify();
/* CLAUDE */ PHASE(Next_Turn_Process_Purify());

    // Initialize_Reports();
/* CLAUDE */ PHASE(Initialize_Reports());

    if(g_timestop_player_num != 0)
    {

        _players[PLAYER_IDX(g_timestop_player_num)].mana_reserve -= 200;  // manual says 150

        if(_players[PLAYER_IDX(g_timestop_player_num)].mana_reserve < 0)
        {

            _players[PLAYER_IDX(g_timestop_player_num)].mana_reserve = 0;

            _players[PLAYER_IDX(g_timestop_player_num)].Globals[TIME_STOP] = 0;

            if(PLAYER_IDX(g_timestop_player_num) == HUMAN_PLAYER_IDX)
            {

                MSG_GEs_Lost = 1;

            }

            g_timestop_player_num = 0;

        }

    }
    else
    {

        // Decrease_Peace_Duration();
/* CLAUDE */ PHASE(Decrease_Peace_Duration());

        // Update_Players_Gold_Reserve();
/* CLAUDE */ LOG_INFO(LOG_CAT_NEXTTURN, "[GOLD] BEFORE Update_Players_Gold_Reserve: gold_reserve=%d", _players[0].gold_reserve);
/* CLAUDE */ PHASE(Update_Players_Gold_Reserve());
/* CLAUDE */ LOG_INFO(LOG_CAT_NEXTTURN, "[GOLD] AFTER  Update_Players_Gold_Reserve: gold_reserve=%d", _players[0].gold_reserve);

        // Players_Update_Magic_Power();
/* CLAUDE */ PHASE(Players_Update_Magic_Power());

        // Players_Apply_Magic_Power();
/* CLAUDE */ PHASE(Players_Apply_Magic_Power());

        // Players_Check_Spell_Research();
/* CLAUDE */ PHASE(Players_Check_Spell_Research());

        // OVL_DisableIncmBlink();
/* CLAUDE */ PHASE(OVL_DisableIncmBlink());

        if(
            (DBG_Alt_A_State == ST_FALSE)
            &&
            (magic_set.random_events == ST_TRUE)
        )
        {

            // Determine_Event();
/* CLAUDE */ PHASE(Determine_Event());

        }

        // Event_Twiddle();
/* CLAUDE */ PHASE(Event_Twiddle());

/* CLAUDE */ LOG_INFO(LOG_CAT_NEXTTURN, "[GOLD] BEFORE Players_Apply_Upkeeps: gold_reserve=%d", _players[0].gold_reserve);
        // Players_Apply_Upkeeps__WIP();
/* CLAUDE */ PHASE(Players_Apply_Upkeeps__WIP());
/* CLAUDE */ LOG_INFO(LOG_CAT_NEXTTURN, "[GOLD] AFTER  Players_Apply_Upkeeps: gold_reserve=%d", _players[0].gold_reserve);

        EMMDATAH_Map();

        // All_Outpost_Population_Growth();
/* CLAUDE */ PHASE(All_Outpost_Population_Growth());
    /* CLAUDE: GD point 621 -- _UNITS after All_Outpost_Population_Growth, before the
     * following Apply_City_Changes(), mirroring OG (which fires at the j_Apply_City_Changes
     * landmark to dodge the ovr119 overlay eviction on return).  Fire once. */
    { static int gd623_done = 0; if(!gd623_done) { gd623_done = 1; gd_dump_units("621_All_Outpost_Population_Growth_Return_U"); } }

        // Apply_City_Changes();
/* CLAUDE */ PHASE(Apply_City_Changes());

        // Diplomacy_Growth_For_Enchantments();
/* CLAUDE */ PHASE(Diplomacy_Growth_For_Enchantments());


        /*
            BEGIN: Grand Vizier
        */
        if(grand_vizier == ST_TRUE)
        {
            // TODO
        }
        /*
            END: Grand Vizier
        */


        // Determine_Offer();
/* CLAUDE */ PHASE(Determine_Offer());
    /* CLAUDE: GD point 627 -- _UNITS after Determine_Offer (before the following
     * Set_Mouse_List), mirroring OG's far-return into Next_Turn_Calc+0x11F.  Fire once. */
    { static int gd624_done = 0; if(!gd624_done) { gd624_done = 1; gd_dump_units("627_Determine_Offer_Return_U"); } }


        Set_Mouse_List(1, mouse_list_hourglass);


        // All_City_Nightshade_Count();
/* CLAUDE */ PHASE(All_City_Nightshade_Count());


        /*
            BEGIN:  NPC Diplomacy
        */

        // Diplomacy_Growth();
/* CLAUDE */ PHASE(Diplomacy_Growth());

        // Determine_First_Contacts();
/* CLAUDE */ PHASE(Determine_First_Contacts());

        Set_Mouse_List(1, mouse_list_hourglass);

        PHASE(NPC_To_NPC_Treaty_Negotiations());

        Set_Mouse_List(1, mouse_list_hourglass);

        // NPC_To_Human_Diplomacy();
/* CLAUDE */ PHASE(NPC_To_Human_Diplomacy());

        // Resolve_Delayed_Diplomacy_Orders();
/* CLAUDE */ PHASE(Resolve_Delayed_Diplomacy_Orders());

        Set_Mouse_List(1, mouse_list_hourglass);

        // End_Of_Turn_Diplomacy_Adjustments();
/* CLAUDE */ PHASE(End_Of_Turn_Diplomacy_Adjustments());

        // Modifier_Diplomacy_Adjustments();
/* CLAUDE */ PHASE(Modifier_Diplomacy_Adjustments());

        /*
            END:  NPC Diplomacy
        */


        // Cool_Off_Volcanoes();
/* CLAUDE */ PHASE(Cool_Off_Volcanoes());

    }


    // All_Players_Apply_Spell_Casting();
/* CLAUDE */ PHASE(All_Players_Apply_Spell_Casting());


    // Delete_Dead_Units();  // ¿ here, because we may have killed units with a spell, just above ?
/* CLAUDE */ PHASE(Delete_Dead_Units());  /* j_Delete_Dead_Units() */
    /* CLAUDE: GD 630 -- _UNITS after Next_Turn_Calc's Delete_Dead_Units (before
     * Set_Unit_Draw_Priority), matching OG's far-return into Next_Turn_Calc+0x1ED.  Fire once. */
    { static int gd625_done = 0; if(!gd625_done) { gd625_done = 1; gd_dump_units("630_Delete_Dead_Units_NextTurnCalc_U"); } }


    // Set_Unit_Draw_Priority();
/* CLAUDE */ PHASE(Set_Unit_Draw_Priority());

    // Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);
/* CLAUDE */ PHASE(Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane));


    // All_City_Removed_Buildings();
/* CLAUDE */ PHASE(All_City_Removed_Buildings());


    // Do_All_Units_XP_Check();
/* CLAUDE */ PHASE(Do_All_Units_XP_Check());
    /* CLAUDE: GD point 631 -- _UNITS after Do_All_Units_XP_Check (before the following
     * Heal_All_Units), mirroring OG's far-return into Next_Turn_Calc+0x210.  Fire once. */
    { static int gd631x_done = 0; if(!gd631x_done) { gd631x_done = 1; gd_dump_units("631_Do_All_Units_XP_Check_Return_U"); } }


    // Heal_All_Units();
/* CLAUDE */ PHASE(Heal_All_Units());
    /* CLAUDE: GD point 632 -- _UNITS after Heal_All_Units, before the following
     * Record_History(), mirroring OG's far-return into Next_Turn_Calc.  Fire once. */
    { static int gd622_done = 0; if(!gd622_done) { gd622_done = 1; gd_dump_units("632_Heal_All_Units_Return_U"); } }


    // Record_History();
/* CLAUDE */ PHASE(Record_History());


    // Increment_Background_Music();
/* CLAUDE */ PHASE(Increment_Background_Music());


    _turn++;


    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {

        SETMAX(_players[itr_players].gold_reserve, MAX_GOLD_RESERVE);

        SETMAX(_players[itr_players].mana_reserve, MAX_MANA_RESERVE);

    }


    // OVL_EnableIncmBlink();
/* CLAUDE */ PHASE(OVL_EnableIncmBlink());


    // Do_Autosave();
/* CLAUDE */ PHASE(Do_Autosave());


    // All_City_Calculations();
/* CLAUDE */ PHASE(All_City_Calculations());
    /* CLAUDE: GD point 633 -- _UNITS after the LATE (end-of-turn) All_City_Calculations,
     * before the following Get_Random_Seed(), mirroring the OG capture point (OG fires at
     * Get_Random_Seed to dodge the ovr119 overlay eviction on return).  Fire once. */
    { static int gd619_done = 0; if(!gd619_done) { gd619_done = 1; gd_dump_units("633_All_City_Calculations_Return_U"); } }
    dbg_scan_defenders("A at 633 dump (NTC tail)");


    RNG_AI_Turn_Seed = Get_Random_Seed();


    Set_Mouse_List(1, mouse_list_default);


#ifdef STU_DEBUG
    {
        uint64_t ntc_end_ms = Platform_Get_Millies();
        uint64_t ntc_dur_ms = ntc_end_ms - ntc_start_ms;
        LOG_INFO(LOG_CAT_NEXTTURN, "[GOLD] END Next_Turn_Calc: gold_reserve=%d", _players[0].gold_reserve);
/* CLAUDE */ #undef PHASE
    }
#endif

    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);

}


// WZD o119p02
// MoO2  Module:  Do_Colony_Calculations_() &&|| All_Colony_Calculations_()

/*
    For All Cities
    Do_City_Calculations() ...sets _CITIES[].food2_units, Production, Gold, Upkeep, Research, Power
    make sane farmer and rebel count
*/
void All_Colony_Calculations(void)
{
    int16_t minimum_farmer_count = 0;
    int16_t itr_cities = 0;
    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        Do_City_Calculations(itr_cities);
        minimum_farmer_count = City_Minimum_Farmers(itr_cities);
        if(_CITIES[itr_cities].farmer_count < minimum_farmer_count)
        {
            _CITIES[itr_cities].farmer_count = (int8_t)minimum_farmer_count;
        }
        City_Rebel_Count(itr_cities);
    }
}


// WZD o119p03
// sub_9EF06                                      ovr119




/*
    WIZARDS.EXE  ovr121
*/

// WZD o121p01
// drake178: UNIT_Create()
// MoO2  Module: COLBLDG  Colbldg_Create_Ship_() |-> Create_Ship_()
/*

    City_Apply_Production()
        UNIT_Create((_CITIES[city_idx].construction - 100), _CITIES[city_idx].owner_idx, _CITIES[city_idx].wx, _CITIES[city_idx].wy, _CITIES[city_idx].wp, city_idx)
    WIZ_HireHero()
        Create_Unit(unit_type_idx, player_idx, FORTX(), FORTY(), FORTP(), -1);
    Lair_Make_Guardians()
        Create_Unit(_LAIRS[lair_idx].guard2_unit_type, NEUTRAL_PLAYER_IDX, _LAIRS[lair_idx].wx, _LAIRS[lair_idx].wy, _LAIRS[lair_idx].wp, 2000)
wp
    9 if in combat

R_Param
    if >= 0 && < 2000
        treats it as city_idx
    if < -1
        takes abs()-1 to index into TBL_Experience[9] = {0, 20, 60, 120, 200, 300, 450, 600, 1000}
        {-2, -3, -4, -5, -6, -7, -8, -9 }
        { 1,  2,  3,  4,  5,  6,  7,  8 }
        then, updates s_UNIT.level from UNIT_GetLevel()
    if == 2000
        e.g., Lair_Make_Guardians()
        ¿ just some number greater than what could be (unit_idx  + city_idx + node_idx ...lair, tower, etc.) ?
        overrides unit count limit
        ¿ only way to create units above 980 ?
        also used by Demon Lord's Summon Demon

*/
int16_t Create_Unit(int16_t unit_type, int16_t owner_idx, int16_t wx, int16_t wy, int16_t wp, int16_t R_Param)
{
    int16_t itr = 0;
    int16_t did_create_unit = 0;  // DNE in Dasm

    did_create_unit = ST_FALSE;

    if(
        (owner_idx != HUMAN_PLAYER_IDX)
        &&
        (_units > 950)
        &&
        (R_Param != 2000)
    )
    {
        goto Done_Failure;
    }

    if(
        (R_Param != 2000)
        &&
        (_units > 980)
    )
    {
        goto Done_Failure;
    }
    
    if(_units == MAX_UNIT_COUNT)
    {
        goto Done_Failure;
    }
    
    _UNITS[_units].wx = (int8_t)wx;
    _UNITS[_units].wy = (int8_t)wy;
    _UNITS[_units].wp = (int8_t)wp;
    _UNITS[_units].owner_idx = (int8_t)owner_idx;
    _UNITS[_units].moves2_max = _unit_type_table[unit_type].Move_Halves;
    _UNITS[_units].type = (uint8_t)unit_type;
    _UNITS[_units].Hero_Slot = ST_UNDEFINED;
    _UNITS[_units].in_tower = ST_FALSE;
    _UNITS[_units].Finished = ST_TRUE;
    _UNITS[_units].moves2 = 0;
    _UNITS[_units].Sight_Range = _unit_type_table[unit_type].Sight;
    _UNITS[_units].dst_wx = 0;
    _UNITS[_units].dst_wy = 0;
    _UNITS[_units].Status = us_Ready;
    _UNITS[_units].Level = 0;
    _UNITS[_units].XP = 0;
    _UNITS[_units].Damage = 0;
    _UNITS[_units].Draw_Priority = 0;
    _UNITS[_units].enchantments = 0;
    _UNITS[_units].mutations = 0;
    _UNITS[_units].Move_Failed = ST_FALSE;
    _UNITS[_units].Rd_Constr_Left = ST_UNDEFINED;

    if((R_Param < 0) || R_Param >= 2000)
    {
        /* ¿ OGBUG  this means level 0 Raiders never get created ? */
        if(R_Param < -1)
        {
            R_Param = (abs(R_Param) - 1);
            _UNITS[_units].XP = TBL_Experience[R_Param];
            _UNITS[_units].Level = (int8_t)Calc_Unit_Level(_units);
        }
    }
    else  /* ((R_Param >= 0) && R_Param < 2000) */
    {
        if(
            (_CITIES[R_Param].bldg_status[bt_FightersGuild] == bs_Built) ||
            (_CITIES[R_Param].bldg_status[bt_FightersGuild] == bs_Replaced)
        )
        {
            _UNITS[_units].XP = TBL_Experience[UL_REGULAR];
        }

        if(
            (_CITIES[R_Param].bldg_status[bt_WarCollege] == bs_Built) ||
            (_CITIES[R_Param].bldg_status[bt_WarCollege] == bs_Replaced)
        )
        {
            _UNITS[_units].XP = TBL_Experience[UL_VETERAN];
        }

        if(_CITIES[R_Param].enchantments[ALTAR_OF_BATTLE] > 0)
        {
            _UNITS[_units].XP = TBL_Experience[UL_ELITE];
        }

        _UNITS[_units].mutations = (int8_t)City_Best_Weapon(R_Param);

        /* Special handling for Settlers */
        if((_unit_type_table[unit_type].Abilities & UA_CREATEOUTPOST) != 0)
        {
            _CITIES[R_Param].population -= 1;
            if(_CITIES[R_Param].population == 0)
            {
                _CITIES[R_Param].Pop_10s = 3;
                if(_CITIES[R_Param].owner_idx >= _num_players)
                {
                    for(itr = 0; itr < _num_players; itr++)
                    {
                        if(
                            (_CITIES[R_Param].wx == _FORTRESSES[itr].wx)
                            &&
                            (_CITIES[R_Param].wy == _FORTRESSES[itr].wy)
                            &&
                            (_CITIES[R_Param].wp == _FORTRESSES[itr].wp)
                        )
                        {
                            _CITIES[R_Param].population += 1;
                            if(itr == HUMAN_PLAYER_IDX)
                            {
                                LBX_Load_Data_Static(message_lbx_file, 0, (SAMB_ptr)GUI_NearMsgString, 11, 1, 150);  // "The last few people are required to maintain your fortress. They may not become settlers.  Settler unit not built."
                                Warn0(GUI_NearMsgString);
                            }
                            goto Done_Failure;  /* Failure: Cannot destroy fortress city */
                        }
                    }
                }
                Destroy_City(R_Param);
            }
        }
        if(
            (_players[owner_idx].alchemy > 0)
            &&
            (_UNITS[_units].mutations == 0)
        )
        {
            _UNITS[_units].mutations = wq_Magic;                        
        }
        if(
            (_players[owner_idx].Globals[CHAOS_SURGE] > 0)
            &&
            ((_unit_type_table[unit_type].Abilities & UA_FANTASTIC) == 0)
        )
        {
            Apply_Chaos_Channels(_units);
        }
        _UNITS[_units].Level = (int8_t)Calc_Unit_Level(_units);
    }

    goto Done_Success;

Done_Failure:
    did_create_unit = ST_FALSE;
    goto Done;

Done_Success:
    _units += 1;
    did_create_unit = ST_TRUE;
    goto Done;

Done:
    return did_create_unit;
}


// WZD o121p02
// AKA Player_Nominal_Skill()
// MGC ovr056 o56p16
/*
    Nope. ¿ estimate the square root of a number ?
    Nope. ¿ the Babylonian method AKA the Newton-Raphson method ?
    Nope. ¿ CORDIC Algorithm - Successive Approximation?
    https://www.kindfortress.com/2018/04/24/design-patterns-triangular-scoring/
    https://en.wikipedia.org/wiki/Triangular_number
    https://en.wikipedia.org/wiki/File:Triangular_Numbers_Plot.svg
*/
/**
 * @brief Converts stored casting-skill investment into nominal base skill.
 *
 * @details
 * Reads `_players[player_idx].spell_casting_skill` as the invested skill-point
 * total and walks upward through the triangular growth thresholds used by the
 * game to derive the corresponding base casting skill. Each loop iteration
 * adds the next even increment to an accumulator until the stored investment
 * is no longer greater than the running threshold, then applies the Archmage
 * bonus if present.
 *
 * This makes the function the inverse of the startup skill-growth loop used
 * when initializing player casting skill from a target nominal value.
 *
 * @param player_idx Index into the global `_players[]` table.
 *
 * @return The player's nominal base casting skill, including the +10 Archmage
 *         bonus when `_players[player_idx].archmage > 0`.
 *
 * @note The computation uses `_players[player_idx].spell_casting_skill` as a
 *       cumulative investment total rather than a direct displayed skill value.
 */
int16_t Player_Base_Casting_Skill(int16_t player_idx)
{
    int32_t num = 0;
    int16_t casting_skill;
    int16_t twos = 0;
    twos = 0;
    casting_skill = 0;
    num = 0;
    while(_players[player_idx].spell_casting_skill > num)
    {
        casting_skill++;
        twos += 2;      // { 2, 4,  6,  8, 10, 12, 14, 16, 18,  20 ... }
        num += twos;    // { 2, 6, 12, 20, 30, 42, 56, 72, 90, 110, ... }
        // count of twos   { 1, 3,  6, 10, 15, 21, 28, 36, 45,  55, ... }
    }
    if(_players[player_idx].archmage > 0)
    {
        casting_skill += 10;
    }
    return casting_skill;
}


// WZD o121p03
// drake178: WIZ_GetCastingCost()
int16_t Casting_Cost(int16_t player_idx, int16_t spell_idx, int16_t combat_flag)
{
    int16_t cast_cost_reduction;
    int16_t Evil_Omens_Up;
    int16_t itr_players;
    int16_t casting_cost;  // _SI_

    cast_cost_reduction = Casting_Cost_Reduction(player_idx, spell_idx);

    casting_cost = spell_data_table[spell_idx].casting_cost;

    if(
        (spell_data_table[spell_idx].magic_realm == sbr_Nature) ||
        (spell_data_table[spell_idx].magic_realm == sbr_Life)
    )
    {
        Evil_Omens_Up = ST_FALSE;

        for(itr_players = 0; itr_players < _num_players; itr_players++)
        {
            if(_players[itr_players].Globals[EVIL_OMENS] > 0)
            {
                Evil_Omens_Up = ST_TRUE;
            }
        }

        if(Evil_Omens_Up == ST_TRUE)
        {
            casting_cost = ((casting_cost * 3) / 2);
        }

    }

    // DEDUCE(JimBalcomb,20240112):  What's this about? I don't see where this 5x is covered in the manual.
    if(
        (
            (spell_data_table[spell_idx].Eligibility == 0) ||
            (spell_data_table[spell_idx].Eligibility == 2) ||
            (spell_data_table[spell_idx].Eligibility == 3)
        ) &&
        combat_flag == ST_FALSE
    )
    {
        casting_cost = (casting_cost * 5);
    }

    casting_cost = (casting_cost - ((casting_cost * cast_cost_reduction) / 100));

    return casting_cost;
}


// WZD o121p04
// drake178: WIZ_CastingCostBonus()
int16_t Casting_Cost_Reduction(int16_t player_idx, int16_t spell_idx)
{
    int16_t Spellbooks;
    int16_t Over7_Books;
    int16_t casting_cost_reduction;  // _CX_

    casting_cost_reduction = 0;

    if(spell_data_table[spell_idx].magic_realm == 5)  /* _Arcane */
    {
        Spellbooks = 0;
    }
    else
    {
        Spellbooks = _players[player_idx].spellranks[spell_data_table[spell_idx].magic_realm];
    }

    if(Spellbooks > 7)
    {
        Over7_Books = (Spellbooks - 7);
        casting_cost_reduction = (Over7_Books * 10);
    }

    if(
        (spell_data_table[spell_idx].magic_realm == 0) &&  /* _Nature */
        (_players[player_idx].nature_mastery > 0)
    )
    {
        casting_cost_reduction += 15;
    }

    if(
        (spell_data_table[spell_idx].magic_realm == 1) &&  /* _Sorcery */
        (_players[player_idx].sorcery_mastery > 0)
    )
    {
        casting_cost_reduction += 15;
    }

    if(
        (spell_data_table[spell_idx].magic_realm == 2) &&  /* _Chaos */
        (_players[player_idx].chaos_mastery > 0)
    )
    {
        casting_cost_reduction += 15;
    }

    if(
        (_players[player_idx].artificer > 0) &&
        (spell_data_table[spell_idx].type == 11)  /* scc_Crafting_Spell */
    )
    {
        casting_cost_reduction += 50;
    }

    if(
        (_players[player_idx].conjurer > 0) &&
        (spell_data_table[spell_idx].type == 0)  /* scc_Summoning */
    )
    {
        casting_cost_reduction += 25;
    }

    if(
        (spell_data_table[spell_idx].magic_realm == 5) &&  /* _Arcane */
        (_players[player_idx].runemaster > 0)
    )
    {
        casting_cost_reduction += 25;
    }

    return casting_cost_reduction;
}


// WZD o121p05
// drake178: WIZ_GetHeroCount()
int16_t Player_Hero_Count(int16_t player_idx)
{
    int16_t itr_hero_slots;  // _CX_
    int16_t hero_count;  // _SI_

    hero_count = 0;

    for(itr_hero_slots = 0; itr_hero_slots < NUM_HERO_SLOTS; itr_hero_slots++)
    {
        if(_players[player_idx].Heroes[itr_hero_slots].unit_idx != ST_UNDEFINED)
        {
            hero_count++;
        }
    }

    return hero_count;
}


// WZD o121p06
// drake178: WIZ_DeadHeroCount()
/*
; returns the amount of dead, but not disintegrated heroes formerly in the player's service
*/
int16_t Player_Dead_Hero_Count(int16_t player_idx)
{
    int16_t itr_hero_types = 0;
    int16_t hero_count = 0;

    hero_count = 0;

    for(itr_hero_types = MIN_HERO_TYPE; itr_hero_types < MAX_HERO_TYPE; itr_hero_types++)
    {
        if(
            (_HEROES2[player_idx]->heroes[itr_hero_types].Level < 0)  /* ¿ DEDU negative means what now/again ? */
            &&
            (_HEROES2[player_idx]->heroes[itr_hero_types].Level != -20)  // DEDU  What's with the -20? Just saw that in AI_Accept_Hero()?
        )
        {
            hero_count++;
        }
    }

    return hero_count;
}


// WZD o121p07
// MoO2  Module: EVENTS  Pick_Random_Officer_()
/*
    zero_cost   {F,T} Hero Cost is 0
    hero_type_class {0,1,2}  0 any, 1 Hero (lesser), 2 Champion (greater)

; selects a random hero from those still available to
; the selected player, if any
; returns the unit type if successful, or -1 otherwise

*/
int16_t Pick_Random_Hero(int16_t player_idx, int16_t zero_cost, int16_t hero_type_class)
{
    int16_t max_attempts;
    int16_t attempts;
    int16_t player_fame;
    int16_t Success;
    int16_t hero_type_idx;  // _SI_

    attempts = 0;

    if(zero_cost > 0)
    {
        max_attempts = 100;
    }
    else
    {
        max_attempts = 10;
    }

    player_fame = Player_Fame(player_idx);

    if(zero_cost == ST_TRUE)
    {
        player_fame = 200;  // ¿ set high enough that all heroes are available ?  ¿ highest cost is 600  ((_unit_type_table[34].Cost - 100) / 10) = ((600 - 100) / 10) = (500 / 10) = 50 < player_fame ?
    }

    Success = ST_FALSE;

    while((_units - 1) < MAX_UNIT_COUNT)
    {
        do
        {
            attempts++;

            if(hero_type_class == 0)
            {
                hero_type_idx = (Random(34) - 1);
            }
            else if(hero_type_class == 1)
            {
                hero_type_idx = (Random(24) - 1);  // Hero
            }
            else
            {
                hero_type_idx = (23 + Random(10));  // Champion
            }

            if(_HEROES2[player_idx]->heroes[hero_type_idx].Level >= 0)  /* ¿ not disintegrated ... but, what about -20 ? */
            {
                if(
                    (
                        !((hero_type_idx == 27)  /* _Priestess */ || (hero_type_idx == 28)  /* _Paladin */)
                        ||
                        (_players[player_idx].spellranks[sbr_Life] > 0)
                    )
                    &&
                    (
                        !((hero_type_idx == 29)  /* _BlackKnight */ || (hero_type_idx == 32)  /* _Necromancer */)
                        ||
                        (_players[player_idx].spellranks[sbr_Death] > 0)
                    )
                    &&
                    (((_unit_type_table[hero_type_idx].cost - 100) / 10) < player_fame)
                )
                {
                    Success = ST_TRUE;
                }
            }

            if(Success == ST_TRUE)
            {
                break;
            }

        } while(attempts < max_attempts);

        if((hero_type_class == 2) && (Success == ST_FALSE))
        {
            hero_type_class = 0;
        }
        else
        {
            break;
        }

    }

    if(Success == ST_FALSE)
    {
        return ST_UNDEFINED;
    }
    else
    {
        return hero_type_idx;
    }
    
}


// WZD o121p08
/*
; calculates and returns the unit's movement allowance
; in half movement point units, accounting for all
; non-combat factors
*/
/*
    Calculate Movement Points for Unit
        Unit Type Table :: Movement Points
        Hero Unit Items - ¿ ... ?
        Chaos Channels - Demon Wings
        Unit Enchantment - Endurance
        Wind Mastery
*/
int16_t Unit_Moves2(int16_t unit_idx)
{
    uint32_t enchantments;  // Unit & Item
    int16_t * hero_items;
    int16_t wind_mastery;
    int16_t item_moves2;
    int16_t endurance;
    int16_t moves2;  // _DI_
    int16_t itr_hero_items;  // _SI_
    int16_t itr_players;  // _SI_

    item_moves2 = 0;

    endurance = ST_FALSE;

    enchantments = 0;

    /*
        BEGIN: Hero Items
    */
    if(_UNITS[unit_idx].Hero_Slot > -1)
    {

        hero_items = &(_players[_UNITS[unit_idx].owner_idx].Heroes[_UNITS[unit_idx].Hero_Slot].Items[0]);

        for(itr_hero_items = 0; itr_hero_items < NUM_HERO_ITEMS; itr_hero_items++)
        {
            
            if(hero_items[itr_hero_items] > -1)
            {

                if(ITEM_POWER(hero_items[itr_hero_items], ip_Endurance))
                {
                    endurance = ST_TRUE;
                }

                // TODO  enchantments |= _ITEMS[itr_hero_items].Powers;
                enchantments |= GET_4B_OFS((uint8_t*)&_ITEMS[hero_items[itr_hero_items]], 0x2E);

                item_moves2 += _ITEMS[hero_items[itr_hero_items]].moves2;

            }

        }

    }
    /*
        END: Hero Items
    */

    moves2 = _unit_type_table[_UNITS[unit_idx].type].Move_Halves;

    enchantments |= _UNITS[unit_idx].enchantments;

    if(moves2 < 6)
    {
        if((enchantments & UE_FLIGHT) != 0)
        {
            moves2 = 6;
        }
    }

    if((_UNITS[unit_idx].mutations & CC_FLIGHT) != 0)
    {
        if(moves2 < 4)
        {
            moves2 = 4;
        }
    }

    if((enchantments & UE_ENDURANCE) != 0)
    {
        endurance = ST_TRUE;
    }

    if(endurance == ST_TRUE)
    {
        moves2 += 2;
    }

    moves2 += item_moves2;

    /*
        BEGIN: Wind Mastery
    */
    if(_unit_type_table[_UNITS[unit_idx].type].Transport > 0)
    {
        wind_mastery = 0;
        for(itr_players = 0; itr_players < NUM_PLAYERS; itr_players++)
        {
            if(_players[itr_players].Globals[WIND_MASTERY] > 0)
            {
                if(_UNITS[unit_idx].owner_idx == itr_players)
                {
                    wind_mastery++;
                }
                else
                {
                    wind_mastery--;
                }
            }
        }
        if(wind_mastery > 0)
        {
            moves2 = ((moves2 * 3) / 2);  /* +50% */
        }
        if(wind_mastery < 0)
        {
            moves2 = (moves2 / 2);  /* -50% */
        }
    }
    /*
        END: Wind Mastery
    */

    return moves2;
}


// WZD o121p09
// drake178: UNIT_IsNormalUnit()
/*
; returns 1 for normal units, 0 for fantastic ones
; INCONSISTENT: treats Chaos Channeled units and Torin
; the Chosen as normal units
*/
/*
    returns ST_FALSE for summoned or undead

REMINDME:  ¿ Normal, Regular, Fantastic, Summoned, ... ?
*/
int16_t Unit_Is_Normal(int16_t unit_idx)
{
    int16_t unit_is_normal = 0;  // _DI_

    if(
        ((_unit_type_table[_UNITS[unit_idx].type].Abilities & UA_FANTASTIC) != 0)
        ||
        ((_UNITS[unit_idx].mutations & UM_UNDEAD) != 0)
    )
    {

        unit_is_normal = ST_FALSE;

    }

    return unit_is_normal;

}


// WZD o121p10
// ¿ MoO2  ..._ALL_TECH_CHEAT..._MORE_MONEY_CHEAT... ?
void Cheat_Power(void)
{
    int16_t itr3;
    int16_t itr1;  // _SI_
    int16_t itr2;  // _DI_

    for(itr1 = 0; itr1 < NUM_RESEARCH_SPELLS; itr1++)
    {
        _players[HUMAN_PLAYER_IDX].research_spells[itr1] = 0;
    }

    for(itr1 = 0; itr1 < _num_players; itr1++)
    {
        _players[itr1].mana_reserve = 10000;
        _players[itr1].gold_reserve = 10000;
        _players[itr1].spell_casting_skill = 10000;
        _players[itr1].Nominal_Skill = Player_Base_Casting_Skill(itr1);
        _players[itr1].Skill_Left = _players[itr1].Nominal_Skill;
        for(itr2 = 0; itr2 <= 5; itr2++)
        {
            for(itr3 = 0; itr3 < NUM_SPELLS_PER_MAGIC_REALM; itr3++)
            {
                if(_players[itr1].spells_list[((itr2 * NUM_SPELLS_PER_MAGIC_REALM) + itr3)] == sls_Knowable)
                {
                    _players[itr1].spells_list[((itr2 * NUM_SPELLS_PER_MAGIC_REALM) + itr3)] = sls_Known;
                }
            }
        }
    }

    _players[HUMAN_PLAYER_IDX].Globals[DETECT_MAGIC] = ST_TRUE;

    _players[HUMAN_PLAYER_IDX].researching_spell_idx = spl_NONE;

}


// WZD o121p11
// drake178: AI_GetThreat_UType()
/*
; calculates and returns an arbitrary unit threat value
; based on attack attributes, defense, and remaining
; hit points and figures; doubling ranged threat if any
; compared to the BU version of the same function
*/
/*

~== Effective_Unit_Strength()
~== Effective_Battle_Unit_Strength()

*/
int16_t Effective_Unit_Type_Strength(int16_t unit_type)
{
    int16_t temp = 0;
    int16_t unit_type_strength = 0;  // _DI_

    unit_type_strength = 0;

    temp = _unit_type_table[unit_type].Hits * _unit_type_table[unit_type].Figures;

    unit_type_strength += Get_Effective_Hits(temp, _unit_type_table[unit_type].Defense);

    if((_unit_type_table[unit_type].Ranged_Type / 10) <= rag_Magic)
    {

        unit_type_strength += (Get_Effective_Ranged_Strength(_unit_type_table[unit_type].Ranged, _unit_type_table[unit_type].Figures, _unit_type_table[unit_type].attack_attributes) * 2);

    }

    if(_unit_type_table[unit_type].Ranged_Type >= srat_Thrown)
    {

        temp = _unit_type_table[unit_type].Ranged;

        unit_type_strength += Get_Effective_Melee_Strength(_unit_type_table[unit_type].Melee, temp, _unit_type_table[unit_type].Figures, _unit_type_table[unit_type].attack_attributes, _unit_type_table[unit_type].Ranged_Type);

    }
    else
    {
        temp = 0;

        unit_type_strength += Get_Effective_Melee_Strength(_unit_type_table[unit_type].Melee, temp, _unit_type_table[unit_type].Figures, _unit_type_table[unit_type].attack_attributes, ST_UNDEFINED);

    }

    return unit_type_strength;

}


// WZD o121p12
// drake178: AI_GetThreat_UNIT()
/*
; calculates and returns an arbitrary unit threat value
; based on attack attributes, defense, and remaining
; hit points and figures; doubling ranged threat if any
; compared to the BU version of the same function
*/
/*

~== Effective_Unit_Type_Strength()
~== Effective_Battle_Unit_Strength()

*/
int16_t Effective_Unit_Strength(int16_t unit_idx)
{
    int16_t effective_strength = 0;
    int16_t temp = 0;
    if(
        (unit_idx < 0)
        ||
        (unit_idx > _units)
    )
    {
        return 0;
    }
    effective_strength = 0;
    Load_Battle_Unit(unit_idx, global_battle_unit);
    temp = ((global_battle_unit->Cur_Figures * global_battle_unit->hits) - global_battle_unit->front_figure_damage);
    effective_strength += Get_Effective_Hits(temp, global_battle_unit->defense);
    if(
        ((global_battle_unit->ranged_type / 10) <= rag_Magic)
        &&
        (global_battle_unit->ranged_type != rat_UNDEF)
    )
    {
        effective_strength += (Get_Effective_Ranged_Strength(global_battle_unit->ranged, global_battle_unit->Cur_Figures, (global_battle_unit->attack_attributes | global_battle_unit->ranged_attack_attributes)) * 2);
    }
    if(global_battle_unit->ranged_type >= srat_Thrown)
    {
        temp = global_battle_unit->ranged;
        effective_strength += Get_Effective_Melee_Strength(global_battle_unit->melee, temp, global_battle_unit->Cur_Figures, (global_battle_unit->attack_attributes | global_battle_unit->melee_attack_attributes), global_battle_unit->ranged_type);
    }
    else
    {
        temp = 0;
        effective_strength += Get_Effective_Melee_Strength(global_battle_unit->melee, temp, global_battle_unit->Cur_Figures, (global_battle_unit->attack_attributes | global_battle_unit->melee_attack_attributes), ST_UNDEFINED);
    }
    return effective_strength;
}


// WZD o121p13
// drake178: RP_GAME_UnitTypesUpdate()
/*
    sets Gold Upkeep Cost for 'Standard Units' (< 'Summoned Units' / 'Fantastic Creatures')
    sets Sound for all Unit Types
*/
void Patch_Units_Upkeep_And_Sound(void)
{
    int16_t itr_unit_types;  // _SI_

#ifdef STU_DEBUG
    LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: BEGIN: Patch_Units_Upkeep_And_Sound()", __FILE__, __LINE__);
#endif

/*
ovr121:1347
27 00 2E 00 39 00 42 00 4C 00 53 00 59 00 5F 00 68 00 72 00 78 00 7F 00 89 00 93 00
Zero_Gold_Units
_BarbSpearmen,
_BeastSpearmen,
_DrowSpearmen,
_DracSpearmen
_DwarfSwordsmen,
_GnollSpearmen,
_HflngSpearmen,
_HElfSpearmen
_HMenSpearmen,
_KlckSpearmen,
_LizSpearmen,
_NmdSpearmen
_OrcSpearmen,
_TrlSpearmen
*/
    for(itr_unit_types = 0; itr_unit_types < 154; itr_unit_types++)
    {
        switch(itr_unit_types)
        {
            case 0x27: /* _BarbSpearmen    */ 
            case 0x2E: /* _BeastSpearmen   */ 
            case 0x39: /* _DrowSpearmen    */ 
            case 0x42: /* _DracSpearme     */ 
            case 0x4C: /* _DwarfSwordsmen  */ 
            case 0x53: /* _GnollSpearmen   */ 
            case 0x59: /* _HflngSpearmen   */ 
            case 0x5F: /* _HElfSpearme     */ 
            case 0x68: /* _HMenSpearmen    */ 
            case 0x72: /* _KlckSpearmen    */ 
            case 0x78: /* _LizSpearmen     */ 
            case 0x7F: /* _NmdSpearme      */ 
            case 0x89: /* _OrcSpearmen     */ 
            case 0x93: /* _TrlSpearmen     */
            {
                _unit_type_table[itr_unit_types].Upkeep = 0;
            } break;
            default:
            {
                _unit_type_table[itr_unit_types].Upkeep = ((_unit_type_table[itr_unit_types].cost + 49) / 50);  // Dasm is doing a ceil(), somehow?
            }
        }
    }

    _unit_type_table[ut_Dwarf].Sound = 102;
    _unit_type_table[(0x24 / sizeof(struct s_UNIT_TYPE))].Sound = 102;
    _unit_type_table[(0x48 / sizeof(struct s_UNIT_TYPE))].Sound = 103;
    _unit_type_table[(0x6C / sizeof(struct s_UNIT_TYPE))].Sound = 102;
    _unit_type_table[(0x90 / sizeof(struct s_UNIT_TYPE))].Sound = 102;
    _unit_type_table[(0x0B4 / sizeof(struct s_UNIT_TYPE))].Sound = 102;
    _unit_type_table[(0x0D8 / sizeof(struct s_UNIT_TYPE))].Sound = 102;
    _unit_type_table[(0x0FC / sizeof(struct s_UNIT_TYPE))].Sound = 102;
    _unit_type_table[(0x120 / sizeof(struct s_UNIT_TYPE))].Sound = 102;
    _unit_type_table[(0x144 / sizeof(struct s_UNIT_TYPE))].Sound = 102;
    _unit_type_table[(0x168 / sizeof(struct s_UNIT_TYPE))].Sound = 103;
    _unit_type_table[(0x18C / sizeof(struct s_UNIT_TYPE))].Sound = 102;
    _unit_type_table[(0x1B0 / sizeof(struct s_UNIT_TYPE))].Sound = 102;
    _unit_type_table[(0x1D4 / sizeof(struct s_UNIT_TYPE))].Sound = 103;
    _unit_type_table[(0x1F8 / sizeof(struct s_UNIT_TYPE))].Sound = 102;
    _unit_type_table[(0x21C / sizeof(struct s_UNIT_TYPE))].Sound = 103;
    _unit_type_table[(0x240 / sizeof(struct s_UNIT_TYPE))].Sound = 102;
    _unit_type_table[(0x264 / sizeof(struct s_UNIT_TYPE))].Sound = 134;
    _unit_type_table[(0x288 / sizeof(struct s_UNIT_TYPE))].Sound = 103;
    _unit_type_table[(0x2AC / sizeof(struct s_UNIT_TYPE))].Sound = 102;
    _unit_type_table[(0x2D0 / sizeof(struct s_UNIT_TYPE))].Sound = 102;
    _unit_type_table[(0x2F4 / sizeof(struct s_UNIT_TYPE))].Sound = 102;
    _unit_type_table[(0x318 / sizeof(struct s_UNIT_TYPE))].Sound = 102;
    _unit_type_table[(0x33C / sizeof(struct s_UNIT_TYPE))].Sound = 103;
    _unit_type_table[(0x360 / sizeof(struct s_UNIT_TYPE))].Sound = 102;
    _unit_type_table[(0x384 / sizeof(struct s_UNIT_TYPE))].Sound = 103;
    _unit_type_table[(0x3A8 / sizeof(struct s_UNIT_TYPE))].Sound = 102;
    _unit_type_table[(0x3CC / sizeof(struct s_UNIT_TYPE))].Sound = 103;
    _unit_type_table[(0x3F0 / sizeof(struct s_UNIT_TYPE))].Sound = 102;
    _unit_type_table[(0x414 / sizeof(struct s_UNIT_TYPE))].Sound = 102;
    _unit_type_table[(0x438 / sizeof(struct s_UNIT_TYPE))].Sound = 102;
    _unit_type_table[(0x45C / sizeof(struct s_UNIT_TYPE))].Sound = 102;
    _unit_type_table[(0x480 / sizeof(struct s_UNIT_TYPE))].Sound = 103;
    _unit_type_table[(0x4A4 / sizeof(struct s_UNIT_TYPE))].Sound = 102;
    _unit_type_table[(0x4C8 / sizeof(struct s_UNIT_TYPE))].Sound = 102;
    _unit_type_table[(0x4EC / sizeof(struct s_UNIT_TYPE))].Sound = 134;
    _unit_type_table[(0x510 / sizeof(struct s_UNIT_TYPE))].Sound = 134;
    _unit_type_table[(0x534 / sizeof(struct s_UNIT_TYPE))].Sound = 134;
    _unit_type_table[(0x558 / sizeof(struct s_UNIT_TYPE))].Sound = 134;
    _unit_type_table[(0x57C / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x5A0 / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x5C4 / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x5E8 / sizeof(struct s_UNIT_TYPE))].Sound = 102;
    _unit_type_table[(0x60C / sizeof(struct s_UNIT_TYPE))].Sound = 151;
    _unit_type_table[(0x630 / sizeof(struct s_UNIT_TYPE))].Sound = 150;
    _unit_type_table[(0x654 / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x678 / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x69C / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x6C0 / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x6E4 / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x708 / sizeof(struct s_UNIT_TYPE))].Sound = 151;
    _unit_type_table[(0x72C / sizeof(struct s_UNIT_TYPE))].Sound = 151;
    _unit_type_table[(0x750 / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x774 / sizeof(struct s_UNIT_TYPE))].Sound = 150;
    _unit_type_table[(0x798 / sizeof(struct s_UNIT_TYPE))].Sound = 102;
    _unit_type_table[(0x7BC / sizeof(struct s_UNIT_TYPE))].Sound = 131;
    _unit_type_table[(0x7E0 / sizeof(struct s_UNIT_TYPE))].Sound = 85;
    _unit_type_table[(0x804 / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x828 / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x84C / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x870 / sizeof(struct s_UNIT_TYPE))].Sound = 102;
    _unit_type_table[(0x894 / sizeof(struct s_UNIT_TYPE))].Sound = 151;
    _unit_type_table[(0x8B8 / sizeof(struct s_UNIT_TYPE))].Sound = 150;
    _unit_type_table[(0x8DC / sizeof(struct s_UNIT_TYPE))].Sound = 6;
    _unit_type_table[(0x900 / sizeof(struct s_UNIT_TYPE))].Sound = 151;
    _unit_type_table[(0x924 / sizeof(struct s_UNIT_TYPE))].Sound = 134;
    _unit_type_table[(0x948 / sizeof(struct s_UNIT_TYPE))].Sound = 134;
    _unit_type_table[(0x96C / sizeof(struct s_UNIT_TYPE))].Sound = 134;
    _unit_type_table[(0x990 / sizeof(struct s_UNIT_TYPE))].Sound = 134;
    _unit_type_table[(0x9B4 / sizeof(struct s_UNIT_TYPE))].Sound = 134;
    _unit_type_table[(0x9D8 / sizeof(struct s_UNIT_TYPE))].Sound = 135;
    _unit_type_table[(0x9FC / sizeof(struct s_UNIT_TYPE))].Sound = 135;
    _unit_type_table[(0x0A20 / sizeof(struct s_UNIT_TYPE))].Sound = 134;
    _unit_type_table[(0x0A44 / sizeof(struct s_UNIT_TYPE))].Sound = 150;
    _unit_type_table[(0x0A68 / sizeof(struct s_UNIT_TYPE))].Sound = 116;
    _unit_type_table[(0x0A8C / sizeof(struct s_UNIT_TYPE))].Sound = 134;
    _unit_type_table[(0x0AB0 / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x0AD4 / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x0AF8 / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x0B1C / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x0B40 / sizeof(struct s_UNIT_TYPE))].Sound = 150;
    _unit_type_table[(0x0B64 / sizeof(struct s_UNIT_TYPE))].Sound = 214;
    _unit_type_table[(0x0B88 / sizeof(struct s_UNIT_TYPE))].Sound = 150;
    _unit_type_table[(0x0BAC / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x0BD0 / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x0BF4 / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x0C18 / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x0C3C / sizeof(struct s_UNIT_TYPE))].Sound = 150;
    _unit_type_table[(0x0C60 / sizeof(struct s_UNIT_TYPE))].Sound = 150;
    _unit_type_table[(0x0C84 / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x0CA8 / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x0CCC / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x0CF0 / sizeof(struct s_UNIT_TYPE))].Sound = 151;
    _unit_type_table[(0x0D14 / sizeof(struct s_UNIT_TYPE))].Sound = 150;
    _unit_type_table[(0x0D38 / sizeof(struct s_UNIT_TYPE))].Sound = 150;
    _unit_type_table[(0x0D5C / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x0D80 / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x0DA4 / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x0DC8 / sizeof(struct s_UNIT_TYPE))].Sound = 102;
    _unit_type_table[(0x0DEC / sizeof(struct s_UNIT_TYPE))].Sound = 151;
    _unit_type_table[(0x0E10 / sizeof(struct s_UNIT_TYPE))].Sound = 150;
    _unit_type_table[(0x0E34 / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x0E58 / sizeof(struct s_UNIT_TYPE))].Sound = 102;
    _unit_type_table[(0x0E7C / sizeof(struct s_UNIT_TYPE))].Sound = 134;
    _unit_type_table[(0x0EA0 / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x0EC4 / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x0EE8 / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x0F0C / sizeof(struct s_UNIT_TYPE))].Sound = 102;
    _unit_type_table[(0x0F30 / sizeof(struct s_UNIT_TYPE))].Sound = 151;
    _unit_type_table[(0x0F54 / sizeof(struct s_UNIT_TYPE))].Sound = 151;
    _unit_type_table[(0x0F78 / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x0F9C / sizeof(struct s_UNIT_TYPE))].Sound = 150;
    _unit_type_table[(0x0FC0 / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x0FE4 / sizeof(struct s_UNIT_TYPE))].Sound = 102;
    _unit_type_table[(0x1008 / sizeof(struct s_UNIT_TYPE))].Sound = 150;
    _unit_type_table[(0x102C / sizeof(struct s_UNIT_TYPE))].Sound = 150;
    _unit_type_table[(0x1050 / sizeof(struct s_UNIT_TYPE))].Sound = 150;
    _unit_type_table[(0x1074 / sizeof(struct s_UNIT_TYPE))].Sound = 150;
    _unit_type_table[(0x1098 / sizeof(struct s_UNIT_TYPE))].Sound = 150;
    _unit_type_table[(0x10BC / sizeof(struct s_UNIT_TYPE))].Sound = 150;
    _unit_type_table[(0x10E0 / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x1104 / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x1128 / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x114C / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x1170 / sizeof(struct s_UNIT_TYPE))].Sound = 151;
    _unit_type_table[(0x1194 / sizeof(struct s_UNIT_TYPE))].Sound = 150;
    _unit_type_table[(0x11B8 / sizeof(struct s_UNIT_TYPE))].Sound = 214;
    _unit_type_table[(0x11DC / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x1200 / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x1224 / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x1248 / sizeof(struct s_UNIT_TYPE))].Sound = 151;
    _unit_type_table[(0x126C / sizeof(struct s_UNIT_TYPE))].Sound = 151;
    _unit_type_table[(0x1290 / sizeof(struct s_UNIT_TYPE))].Sound = 150;
    _unit_type_table[(0x12B4 / sizeof(struct s_UNIT_TYPE))].Sound = 102;
    _unit_type_table[(0x12D8 / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x12FC / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x1320 / sizeof(struct s_UNIT_TYPE))].Sound = 132;
    _unit_type_table[(0x1344 / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x1368 / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x138C / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x13B0 / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x13D4 / sizeof(struct s_UNIT_TYPE))].Sound = 102;
    _unit_type_table[(0x13F8 / sizeof(struct s_UNIT_TYPE))].Sound = 151;
    _unit_type_table[(0x141C / sizeof(struct s_UNIT_TYPE))].Sound = 151;
    _unit_type_table[(0x1440 / sizeof(struct s_UNIT_TYPE))].Sound = 86;
    _unit_type_table[(0x1464 / sizeof(struct s_UNIT_TYPE))].Sound = 150;
    _unit_type_table[(0x1488 / sizeof(struct s_UNIT_TYPE))].Sound = 116;
    _unit_type_table[(0x14AC / sizeof(struct s_UNIT_TYPE))].Sound = 85;
    _unit_type_table[(0x14D0 / sizeof(struct s_UNIT_TYPE))].Sound = 85;
    _unit_type_table[(0x14F4 / sizeof(struct s_UNIT_TYPE))].Sound = 85;
    _unit_type_table[(0x1518 / sizeof(struct s_UNIT_TYPE))].Sound = 151;
    _unit_type_table[(0x153C / sizeof(struct s_UNIT_TYPE))].Sound = 149;
    _unit_type_table[(0x1560 / sizeof(struct s_UNIT_TYPE))].Sound = 85;
    _unit_type_table[(0x1584 / sizeof(struct s_UNIT_TYPE))].Sound = 162;
    _unit_type_table[(0x15A8 / sizeof(struct s_UNIT_TYPE))].Sound = 147;
    _unit_type_table[(0x15CC / sizeof(struct s_UNIT_TYPE))].Sound = 147;
    _unit_type_table[(0x15F0 / sizeof(struct s_UNIT_TYPE))].Sound = 130;
    _unit_type_table[(0x1614 / sizeof(struct s_UNIT_TYPE))].Sound = 213;
    _unit_type_table[(0x1638 / sizeof(struct s_UNIT_TYPE))].Sound = 131;
    _unit_type_table[(0x165C / sizeof(struct s_UNIT_TYPE))].Sound = 130;
    _unit_type_table[(0x1680 / sizeof(struct s_UNIT_TYPE))].Sound = 130;
    _unit_type_table[(0x16A4 / sizeof(struct s_UNIT_TYPE))].Sound = 130;
    _unit_type_table[(0x16C8 / sizeof(struct s_UNIT_TYPE))].Sound = 134;
    _unit_type_table[(0x16EC / sizeof(struct s_UNIT_TYPE))].Sound = 161;
    _unit_type_table[(0x1710 / sizeof(struct s_UNIT_TYPE))].Sound = 112;
    _unit_type_table[(0x1734 / sizeof(struct s_UNIT_TYPE))].Sound = 150;
    _unit_type_table[(0x1758 / sizeof(struct s_UNIT_TYPE))].Sound = 150;
    _unit_type_table[(0x177C / sizeof(struct s_UNIT_TYPE))].Sound = 147;
    _unit_type_table[(0x17A0 / sizeof(struct s_UNIT_TYPE))].Sound = 147;
    _unit_type_table[(0x17C4 / sizeof(struct s_UNIT_TYPE))].Sound = 130;
    _unit_type_table[(0x17E8 / sizeof(struct s_UNIT_TYPE))].Sound = 130;
    _unit_type_table[(0x180C / sizeof(struct s_UNIT_TYPE))].Sound = 130;
    _unit_type_table[(0x1830 / sizeof(struct s_UNIT_TYPE))].Sound = 102;
    _unit_type_table[(0x1854 / sizeof(struct s_UNIT_TYPE))].Sound = 113;
    _unit_type_table[(0x1878 / sizeof(struct s_UNIT_TYPE))].Sound = 150;
    _unit_type_table[(0x189C / sizeof(struct s_UNIT_TYPE))].Sound = 179;
    _unit_type_table[(0x18C0 / sizeof(struct s_UNIT_TYPE))].Sound = 131;
    _unit_type_table[(0x18E4 / sizeof(struct s_UNIT_TYPE))].Sound = 134;
    _unit_type_table[(0x1908 / sizeof(struct s_UNIT_TYPE))].Sound = 134;
    _unit_type_table[(0x192C / sizeof(struct s_UNIT_TYPE))].Sound = 147;
    _unit_type_table[(0x1950 / sizeof(struct s_UNIT_TYPE))].Sound = 135;
    _unit_type_table[(0x1974 / sizeof(struct s_UNIT_TYPE))].Sound = 135;
    _unit_type_table[(0x1998 / sizeof(struct s_UNIT_TYPE))].Sound = 210;
    _unit_type_table[(0x19BC / sizeof(struct s_UNIT_TYPE))].Sound = 147;
    _unit_type_table[(0x19E0 / sizeof(struct s_UNIT_TYPE))].Sound = 213;
    _unit_type_table[(0x1A04 / sizeof(struct s_UNIT_TYPE))].Sound = 165;
    _unit_type_table[(0x1A28 / sizeof(struct s_UNIT_TYPE))].Sound = 114;
    _unit_type_table[(0x1A4C / sizeof(struct s_UNIT_TYPE))].Sound = 161;
    _unit_type_table[(0x1A70 / sizeof(struct s_UNIT_TYPE))].Sound = 161;
    _unit_type_table[(0x1A94 / sizeof(struct s_UNIT_TYPE))].Sound = 192;
    _unit_type_table[(0x1AB8 / sizeof(struct s_UNIT_TYPE))].Sound = 8;
    _unit_type_table[(0x1ADC / sizeof(struct s_UNIT_TYPE))].Sound = 8;
    _unit_type_table[(0x1B00 / sizeof(struct s_UNIT_TYPE))].Sound = 8;
    _unit_type_table[(0x1B24 / sizeof(struct s_UNIT_TYPE))].Sound = 165;
    _unit_type_table[(0x1B48 / sizeof(struct s_UNIT_TYPE))].Sound = 130;
    _unit_type_table[(0x1B6C / sizeof(struct s_UNIT_TYPE))].Sound = 182;
    _unit_type_table[(0x1B90 / sizeof(struct s_UNIT_TYPE))].Sound = 112;

    _unit_type_table[ut_Nagas].Sound = 146;


#ifdef STU_DEBUG
    LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: END: Patch_Units_Upkeep_And_Sound()", __FILE__, __LINE__);
#endif

}


// WZD o121p14
// drake178: CTY_RemoveMessages()
/*
OON XREF: CTY_Remove()

drake178: ; removes any messages related to the city from the finished building message queue
drake178: ; BUG: fails to copy the subsequent records properly, resulting in mismatched reports

*/
void City_Delete_Building_Complete_Messages(int16_t city_idx)
{
    int16_t itr1;  // _DX_
    int16_t itr2;  // _SI_

    itr1 = 0;

    while(g_bldg_msg_ctr > itr1)
    {
        // ¿ shift the city_idx for the messages down 1 in preparation for shifting down all the cities in the city structures array ?
        if(MSG_Building_Complete[itr1].city_idx > city_idx)
        {
            MSG_Building_Complete[itr1].city_idx = (MSG_Building_Complete[itr1].city_idx - 1);
        }
        else
        {
            if(MSG_Building_Complete[itr1].city_idx == city_idx)
            {
                itr2 = itr1;

                // BUG BUGBUG OGBUG moves the city_idx for the message, but not the bldg_type_idx
                while(g_bldg_msg_ctr > itr2)
                {
                    MSG_Building_Complete[itr2].city_idx = MSG_Building_Complete[(itr2 + 1)].city_idx;
                    // OGBUG DNE  MSG_Building_Complete[itr2].bldg_type_idx = MSG_Building_Complete[(itr2 + 1)].bldg_type_idx;
                    itr2++;
                }

                g_bldg_msg_ctr--;
            }
        }

        // @@Next_Message
        itr1++;
    }

}



/*
    WIZARDS.EXE  ovr140
*/

// WZD o140p01
void All_City_Calculations(void)
{
    int16_t itr_cities = 0;

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        Do_City_Calculations(itr_cities);
    }

}


// WZD o140p02
// drake178: MSG_Clear()
// MoO2  Module: REPORT  Initialize_Reports_()
// ... memset_(_something_interesting_happened, 8, 0);
/*
    Unit: Lost, Killed
    UE, CE, OE: Lost
    City: Loss, Gain
    City: Grow, Shrink
    Building: Lost

    does not reset g_bldg_msg_ctr
*/
void Initialize_Reports(void)
{
    msg_unit_lost_ctr = 0;
    MSG_UnitKilled_Count = 0;
    MSG_UEsLost_Count = 0;
    MSG_CEsLost_Count = 0;
    MSG_GEs_Lost = 0;
    MSG_CityGrowth_Count = 0;
    MSG_CityDeath_Count = 0;
    MSG_BldLost_Count = 0;
    MSG_CityLost_Count = 0;
    MSG_CityGained_Count = 0;
}


// WZD o140p03
// MoO2  Module: COLCALC  Apply_Production_()
/**
 * @brief Applies one turn of production progress for a single city.
 *
 * @details
 * This routine advances the city's accumulated production and resolves completion
 * of either unit production or building construction based on
 * @c _CITIES[city_idx].construction.
 *
 * Behavior summary:
 * 1. Early-out if the city has no population.
 * 2. If constructing a unit (@c construction >= 100):
 *    - Add production to @c Prod_Accu.
 *    - If enough production is accumulated, attempt to create the unit.
 *    - If unit cap is reached, apply human/AI-specific fallback behavior
 *      (warning message for human, autobuild/trade-goods fallback).
 *    - Reset accumulated production on completion attempt.
 * 3. If constructing a building:
 *    - Handle pseudo-products (values below @c bt_Barracks) for AI cities.
 *    - Add production (neutral cities accumulate at half rate).
 *    - On completion, update building status, replacement relationships,
 *      building count, reports, and follow-up construction choice.
 * 4. If Grand Vizier is enabled for the human player and the city is in
 *    autobuild state, invoke automatic build selection.
 *
 * Human-specific side effects include building-complete messages and unit-cap
 * warning UI strings; AI cities generally switch to autobuild after completion.
 *
 * @param city_idx Index of the city in @c _CITIES[] whose production phase is processed.
 *
 * @return This function returns no value.
 *
 * @note Unit production uses @c Create_Unit() with unit type encoded as
 *       @c construction - 100.
 * @note Production overflow is discarded on completion by resetting
 *       @c _CITIES[city_idx].Prod_Accu to 0.
 * @note Building completion may mark replaced buildings as @c bs_Replaced when
 *       the new building defines @c replace_bldg.
 * @note Oracle completion for the human player triggers local exploration reveal.
 * @note With Grand Vizier enabled, human post-completion selection is automated
 *       through @c Player_Colony_Autobuild_HP().
 *
 * @see City_Current_Product_Cost(), Create_Unit(), Player_Colony_Autobuild_HP()
 */
void City_Apply_Production(int16_t city_idx)
{
    char city_name[LEN_NAME];
    int16_t uu_troops[MAX_STACK] = { 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB };  // HACK
    int16_t uu_troop_count;
    int16_t product_cost;

    product_cost = City_Current_Product_Cost(city_idx);

    if(_CITIES[city_idx].population <= 0)
    {
        return;
    }

    if(_CITIES[city_idx].construction >= 100)  /* *Product* is 'Unit' */
    {
        _CITIES[city_idx].Prod_Accu += _CITIES[city_idx].production_units;
        if(_CITIES[city_idx].Prod_Accu >= product_cost)
        {
            if((_units + 1) < MAX_UNIT_COUNT)
            {
                Create_Unit((_CITIES[city_idx].construction - 100), _CITIES[city_idx].owner_idx, _CITIES[city_idx].wx, _CITIES[city_idx].wy, _CITIES[city_idx].wp, city_idx);
                Evict_Weakest_Unit((_units - 1));
                Army_At_City(city_idx, &uu_troop_count, &uu_troops[0]);
                if(
                    (_CITIES[city_idx].owner_idx != HUMAN_PLAYER_IDX)
                    ||
                    (grand_vizier == ST_TRUE)
                )
                {
                    _CITIES[city_idx].construction = bt_AUTOBUILD;
                }
            }
            else
            {
                if(_CITIES[city_idx].owner_idx != HUMAN_PLAYER_IDX)
                {
                    _CITIES[city_idx].construction = bt_AUTOBUILD;
                }
                else
                {
                    LBX_Load_Data_Static(message_lbx_file, 0, (SAMB_ptr)GUI_NearMsgString, 66, 1, 150);  // "Maximum number of units exceeded"
                    stu_strcpy(city_name, _CITIES[city_idx].name);
                    stu_strcat(GUI_NearMsgString, city_name);
                    stu_strcat(GUI_NearMsgString, cnst_TooManyUnits);  // ". You must disband some units if you wish to build or summon any more."
                    Warn0(GUI_NearMsgString);
                    if(
                        (_CITIES[city_idx].owner_idx == HUMAN_PLAYER_IDX)
                        &&
                        (grand_vizier == ST_TRUE)
                    )
                    {
                        _CITIES[city_idx].construction = bt_AUTOBUILD;
                    }
                    else
                    {
                        _CITIES[city_idx].construction = bt_TradeGoods;
                    }
                }
            }
            _CITIES[city_idx].Prod_Accu = 0;  /* "surplus production units will be lost" */
        }
    }
    else  /* *Product* is 'Building' */
    {
        if(_CITIES[city_idx].construction < bt_Barracks)  /* ~== not a *real* building */
        {
            if(_CITIES[city_idx].owner_idx != HUMAN_PLAYER_IDX)
            {
                _CITIES[city_idx].construction = bt_AUTOBUILD;
            }
        }
        else
        {
            if(_CITIES[city_idx].owner_idx == NEUTRAL_PLAYER_IDX)
            {
                _CITIES[city_idx].Prod_Accu += (_CITIES[city_idx].production_units / 2);
            }
            else
            {
                _CITIES[city_idx].Prod_Accu += _CITIES[city_idx].production_units;
            }
            /* Did we build a Building? */
            if(_CITIES[city_idx].Prod_Accu >= product_cost)
            {
                // TODO  pragma ignore C6385  Reading invalid data from '_CITIES[city_idx].bldg_status':  the readable size is '36' bytes, but '_CITIES[city_idx].construction' bytes may be read.
                if(_CITIES[city_idx].bldg_status[_CITIES[city_idx].construction] >= 0)  /* bs_Replaced, bs_Built, bs_Removed */
                {
                    _CITIES[city_idx].bldg_status[_CITIES[city_idx].construction] += 1;
                }
                else
                {
                    // TODO  pragma ignore C6386  Buffer overrun while writing to '_CITIES[city_idx].bldg_status':  the writable size is '36' bytes, but '_CITIES[city_idx].construction' bytes might be written.
                    _CITIES[city_idx].bldg_status[_CITIES[city_idx].construction] = bs_Built;
                    if(bldg_data_table[_CITIES[city_idx].construction].replace_bldg != ST_UNDEFINED)
                    {
                        _CITIES[city_idx].bldg_status[bldg_data_table[_CITIES[city_idx].construction].replace_bldg] = bs_Replaced;
                    }
                }
                if(
                    (_CITIES[city_idx].construction == bt_Oracle)
                    &&
                    (_CITIES[city_idx].owner_idx == HUMAN_PLAYER_IDX)
                )
                {
                    Set_Map_Square_Explored_Flags_XYP_Range(_CITIES[city_idx].wx, _CITIES[city_idx].wy, _CITIES[city_idx].wp, 6);
                }
                _CITIES[city_idx].bldg_cnt += 1;
                _CITIES[city_idx].Prod_Accu = 0;
                if(_CITIES[city_idx].owner_idx != HUMAN_PLAYER_IDX)
                {
                    _CITIES[city_idx].construction = bt_AUTOBUILD;
                }
                else
                {
                    if(g_bldg_msg_ctr < 20)
                    {
                        MSG_Building_Complete[g_bldg_msg_ctr].city_idx = (int8_t)city_idx;
                        MSG_Building_Complete[g_bldg_msg_ctr].bldg_type_idx = _CITIES[city_idx].construction;
                        g_bldg_msg_ctr++;
                    }
                    if(grand_vizier == ST_TRUE)
                    {
                        _CITIES[city_idx].construction = bt_AUTOBUILD;
                    }
                    else
                    {
                        _CITIES[city_idx].construction = bt_Housing;
                    }
                }
            }
        }
    }

    if(
        (_CITIES[city_idx].owner_idx == HUMAN_PLAYER_IDX)
        &&
        (grand_vizier == ST_TRUE)
        &&
        (_CITIES[city_idx].construction == bt_AUTOBUILD)
    )
    {
        Player_Colony_Autobuild_HP(city_idx);
    }

}


// WZD o140p04
// drake178: WIZ_GoldIncomes()
void Update_Players_Gold_Reserve(void)
{
    int16_t normal_units[NUM_PLAYERS] = { 0, 0, 0, 0, 0, 0 };
    int16_t food_incomes[NUM_PLAYERS] = { 0, 0, 0, 0, 0, 0 };
    int16_t gold_incomes[NUM_PLAYERS] = { 0, 0, 0, 0, 0, 0 };
    int16_t Excess_Food = 0;
    int16_t itr_players = 0;  // _SI_
    int16_t itr_heroes = 0;  // _DI_
    int16_t itr_cities = 0;  // _SI_

    // for(itr_players = 0; itr_players < _num_players; itr_players++)
    // VS complains about leaving elements in the array uninitialized, even though they can never be accessed
    // for (itr_players = 0; itr_players < 6; itr_players++)
    // NM. Access Violation for player 5
    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        gold_incomes[itr_players] = 0;
        food_incomes[itr_players] = 0;

        for(itr_heroes = 0; itr_heroes < NUM_HEROES; itr_heroes++)
        {
            if(_players[itr_players].Heroes[itr_heroes].unit_idx > -1)
            {

                if(HERO_NOBLE(itr_players, _UNITS[_players[itr_players].Heroes[itr_heroes].unit_idx].type))
                {
                    gold_incomes[itr_players] += 10;
                }
            }
        }
    }

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        if(_CITIES[itr_cities].owner_idx != -1)
        {
            gold_incomes[_CITIES[itr_cities].owner_idx] += (_CITIES[itr_cities].gold_units - _CITIES[itr_cities].building_maintenance);
            Excess_Food = _CITIES[itr_cities].food_units - _CITIES[itr_cities].population;
            if(Excess_Food < 0) { Excess_Food = 0; }
            food_incomes[_CITIES[itr_cities].owner_idx] += (Excess_Food / 2);
        }
    }

    Players_Normal_Units(&normal_units[0]);

    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        food_incomes[itr_players] -= (normal_units[itr_players] / 2);
        
        if(food_incomes[itr_players] < 0)
        {
            food_incomes[itr_players] = 0;
        }

        if((gold_incomes[itr_players] + food_incomes[itr_players]) < MAX_SINT2)
        {
            _players[itr_players].gold_reserve += (gold_incomes[itr_players] + food_incomes[itr_players]);
        }
        else
        {
            _players[itr_players].gold_reserve = MAX_SINT2;
        }

        if(_players[itr_players].gold_reserve < 0)
        {
            _players[itr_players].gold_reserve = 0;
        }
    }

}


// WZD o140p05
// drake178: WIZ_PowerIncomes()
// OON XREF: Next_Turn_Calc()
/*
    sets
    _players[itr_players].research_cost_remaining
    _players[itr_players].mana_reserve
    _players[itr_players].spell_casting_skill

*/
void Players_Apply_Magic_Power(void)
{
    int16_t research_income = 0;
    int16_t mana_income = 0;
    int16_t skill_income = 0;
    int16_t itr_players = 0;  // _SI_

    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        if(_players[itr_players].casting_spell_idx != spl_Spell_Of_Return)
        {
            Player_Magic_Power_Income_Total(&mana_income, &research_income, &skill_income, itr_players);
        }

        if(_players[itr_players].research_cost_remaining <= research_income)
        {
            _players[itr_players].research_cost_remaining = 0;
        }
        else
        {
            _players[itr_players].research_cost_remaining -= research_income;
        }

        if((MAX_SINT2 - _players[itr_players].mana_reserve) >= mana_income)
        {
            _players[itr_players].mana_reserve += mana_income;
        }
        else
        {
            _players[itr_players].mana_reserve = MAX_SINT2;
        }

        // 'Archmage' Special Ability 50% bonus to all mana spent on increasing skill
        // ¿ vs. Calc_Nominal_Skill() ?
        if(_players[itr_players].archmage > 0)
        {
            _players[itr_players].spell_casting_skill += ((skill_income * 3) / 2);
        }
        else
        {
            _players[itr_players].spell_casting_skill += skill_income;
        }
    }

}


// WZD o140p06
// drake178: WIZ_ProcessUpkeep()
/*
    Applies Upkeeps to Gold and Mana
    updates _players[itr].gold_reserve
    updates _players[itr].mana_reserve

*/
void Players_Apply_Upkeeps__WIP(void)
{
    int16_t excess_foods[NUM_PLAYERS] = { 0, 0, 0, 0, 0, 0 };
    int16_t mana_upkeeps[NUM_PLAYERS] = { 0, 0, 0, 0, 0, 0 };
    int16_t gold_upkeeps[NUM_PLAYERS] = { 0, 0, 0, 0, 0, 0 };
    int16_t food_upkeep = 0;
    int16_t itr = 0;  // _DI_

    for(itr = 0; itr < NUM_PLAYERS; itr++)
    {

        excess_foods[itr] = 0;

    }

    for(itr = 0; itr < _cities; itr++)
    {

        if(
            (_CITIES[itr].owner_idx != ST_UNDEFINED)
            &&
            (_CITIES[itr].food_units > _CITIES[itr].population)
        )
        {

            excess_foods[_CITIES[itr].owner_idx] += (_CITIES[itr].food_units - _CITIES[itr].population);

        }

    }

    for(itr = 0; itr < _num_players; itr++)
    {

        food_upkeep = Player_Armies_Food_Upkeep(itr);

        if(
            (excess_foods[itr] < food_upkeep)
            &&
            (itr == HUMAN_PLAYER_IDX)
        )
        {

            WIZ_MatchFoodUpkeep__WIP(itr, excess_foods[itr], food_upkeep);

        }

        gold_upkeeps[itr] = Player_Armies_Gold_Upkeep(itr);

        if(
            (gold_upkeeps[itr] > _players[itr].gold_reserve)
            &&
            (itr == 0)
        )
        {

            gold_upkeeps[itr] = WIZ_MatchGoldUpkeep(itr, gold_upkeeps[itr]);

        }

        _players[itr].gold_reserve = (_players[itr].gold_reserve - gold_upkeeps[itr]);

        if(_players[itr].gold_reserve < 0)
        {

            _players[itr].gold_reserve = 0;

        }

        mana_upkeeps[itr] = Player_Armies_And_Enchantments_Mana_Upkeep(itr);

        if(
            (mana_upkeeps[itr] > _players[itr].mana_reserve)
            &&
            (itr == 0)
        )
        {

            mana_upkeeps[itr] = WIZ_MatchManaUpkeep__WIP(itr, mana_upkeeps[itr]);

        }

        _players[itr].mana_reserve = (_players[itr].mana_reserve - mana_upkeeps[itr]);

        if(_players[itr].mana_reserve < 0)
        {

            _players[itr].mana_reserve = 0;

        }

    }

}


// WZD o140p07
// drake178: WIZ_MatchFoodUpkeep()
/*
attempts to disband non-hero, non-transport normal
units until the food upkeep matches or is less than
the food income

inherits a severe BUG from UNIT_GetDependants
*/
/*

IIF
if((excess_foods[itr] < food_upkeep) && (itr == HUMAN_PLAYER_IDX))
...when Food is negative...

*/
void WIZ_MatchFoodUpkeep__WIP(int16_t player_idx, int16_t food_excess, int16_t food_upkeep)
{
    int16_t troops[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t itr_troops = 0;
    int16_t troop_count = 0;
    int16_t itr_units = 0;  // _SI_

    for(itr_units = (_units - 1); ((itr_units > -1) && (food_upkeep > food_excess)); itr_units--)
    {

        // just Normal/Regular units
        if(
            (_UNITS[itr_units].owner_idx == player_idx)
            &&
            ((_unit_type_table[_UNITS[itr_units].type].Abilities & UA_FANTASTIC) == 0)
            &&
            (_UNITS[itr_units].type > ut_Chosen)
            &&
            (_unit_type_table[_UNITS[itr_units].type].Transport == 0)
        )
        {

            food_upkeep--;

            UNIT_GetDependants__WIP(itr_units, &troop_count, &troops[0]);

            if(troop_count > 0)
            {

                for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
                {

                    if(
                        ((_unit_type_table[_UNITS[itr_units].type].Abilities & UA_FANTASTIC) == 0)
                        &&
                        (_UNITS[itr_units].type > ut_Chosen)
                    )
                    {

                        food_upkeep--;

                        if(
                            (msg_unit_lost_ctr < 20)
                            &&
                            (player_idx == HUMAN_PLAYER_IDX)
                        )
                        {

                            msg_unit_lost[msg_unit_lost_ctr].Unit_Type = _UNITS[itr_units].type;

                            msg_unit_lost[msg_unit_lost_ctr].Cause = 4;

                            msg_unit_lost_ctr++;

                        }

                        Kill_Unit(troops[itr_troops], kt_Normal);

                    }

                }

            }

        }

        if(
            (msg_unit_lost_ctr < 20)
            &&
            (player_idx == HUMAN_PLAYER_IDX)
        )
        {

            msg_unit_lost[msg_unit_lost_ctr].Unit_Type = _UNITS[itr_units].type;

            msg_unit_lost[msg_unit_lost_ctr].Cause = 0;

            msg_unit_lost_ctr++;

        }

        Kill_Unit(itr_units, kt_Dismissed);

    }

}


// WZD o140p08
// drake178: WIZ_MatchGoldUpkeep()
int16_t WIZ_MatchGoldUpkeep(int16_t player_idx, int16_t gold_upkeep)
{
    int16_t troops[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t itr_troops = 0;
    int16_t troop_count = 0;
    int16_t unit_gold_upkeep = 0;
    int16_t itr_units = 0;  // _SI_

    for(itr_units = (_units - 1); ((itr_units > -1) && (_players[player_idx].gold_reserve < gold_upkeep)); itr_units--)
    {
        if(
            (_UNITS[itr_units].owner_idx == player_idx) &&
            ((_unit_type_table[_UNITS[itr_units].type].Abilities & UA_FANTASTIC) == 0) &&
            (_UNITS[itr_units].type > ut_Chosen) &&
            (_unit_type_table[_UNITS[itr_units].type].Transport == 0)
        )
        {
            
            unit_gold_upkeep = Unit_Gold_Upkeep(itr_units);

            if(unit_gold_upkeep > 0)
            {
                gold_upkeep -= unit_gold_upkeep;

                UNIT_GetDependants__WIP(itr_units, &troop_count, &troops[0]);

                if(troop_count > 0)
                {
                    for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
                    {

                        gold_upkeep -= Unit_Gold_Upkeep(troops[itr_troops]);
                        
                        if(
                            (msg_unit_lost_ctr < 20) &&
                            (player_idx == HUMAN_PLAYER_IDX)
                        )
                        {
                            msg_unit_lost[msg_unit_lost_ctr].Unit_Type = _UNITS[itr_units].type;
                            msg_unit_lost[msg_unit_lost_ctr].Cause = 4;
                            msg_unit_lost_ctr++;
                        }
                        Kill_Unit(troops[itr_troops], kt_Normal);

                    }
                }

                if(
                    (msg_unit_lost_ctr < 20) &&
                    (player_idx == HUMAN_PLAYER_IDX)
                )
                {
                    msg_unit_lost[msg_unit_lost_ctr].Unit_Type = _UNITS[itr_units].type;
                    msg_unit_lost[msg_unit_lost_ctr].Cause = 1;
                    msg_unit_lost_ctr++;
                }

                Kill_Unit(itr_units, kt_Dismissed);

            }
        }
    }

    return gold_upkeep;
}


// WZD o140p09
// drake178: WIZ_MatchManaUpkeep()
int16_t WIZ_MatchManaUpkeep__WIP(int16_t player_idx, int16_t mana_upkeep)
{
    int16_t Asset_Type = 0;
    int16_t Asset_Types_Checked = 0;
    int16_t mana_expense_type[4] = { 0, 0, 0, 0 };
    int16_t itr = 0;

    Asset_Types_Checked = 0;

    for(itr = 0; itr < 4; itr++)
    {
        mana_expense_type[itr] = 0;
    }

    while((_players[player_idx].mana_reserve < mana_upkeep) && (Asset_Types_Checked < 4))
    {
        Asset_Types_Checked++;

        Asset_Type = (Random(4) - 1);

        while(mana_expense_type[Asset_Type] > 0)
        {
            Asset_Type = ((Asset_Type + 1) % 4);
        }

        switch(Asset_Type)
        {
            case 0:
            {
                // TODO  mana_upkeep = WIZ_RemoveUEs(player_idx, mana_upkeep);
                mana_expense_type[0] = 1;
            } break;
            case 1:
            {
                // TODO  mana_upkeep = WIZ_RemoveCEs(player_idx, mana_upkeep);
                mana_expense_type[1] = 1;
            } break;
            case 2:
            {
                // TODO  mana_upkeep = WIZ_RemoveGEs(player_idx, mana_upkeep);
                mana_expense_type[2] = 1;
            } break;
            case 3:
            {
                mana_upkeep = WIZ_DisbandSummons(player_idx, mana_upkeep);
                mana_expense_type[3] = 1;
            } break;
        }
    }

    return mana_upkeep;
}


// WZD o140p10
// drake178: WIZ_DisbandSummons()
int16_t WIZ_DisbandSummons(int16_t player_idx, int16_t mana_upkeep)
{
    int16_t troops[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Channeler_Divisor = 0;
    int16_t itr_troops = 0;
    int16_t troop_count = 0;
    int16_t itr_units = 0;  // _SI_

    if(_players[player_idx].channeler > 0)
    {
        Channeler_Divisor = 2;
    }
    else
    {
        Channeler_Divisor = 1;
    }

    for(itr_units = (_units - 1); ((itr_units > -1) && (_players[player_idx].mana_reserve < mana_upkeep)); itr_units--)
    {
        if(
            (_UNITS[itr_units].owner_idx == player_idx) &&
            ((_unit_type_table[_UNITS[itr_units].type].Abilities & UA_FANTASTIC) != 0)
        )
        {
            
            mana_upkeep -= (Unit_Mana_Upkeep(itr_units) / Channeler_Divisor);

            UNIT_GetDependants__WIP(itr_units, &troop_count, &troops[0]);

            if(troop_count > 0)
            {
                for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
                {

                    mana_upkeep -= (Unit_Mana_Upkeep(itr_units) / Channeler_Divisor);
                    
                    if(
                        (msg_unit_lost_ctr < 20) &&
                        (player_idx == HUMAN_PLAYER_IDX)
                    )
                    {
                        msg_unit_lost[msg_unit_lost_ctr].Unit_Type = _UNITS[itr_units].type;
                        msg_unit_lost[msg_unit_lost_ctr].Cause = 4;
                        msg_unit_lost_ctr++;
                    }
                    Kill_Unit(troops[itr_troops], kt_Normal);

                }
            }

            if(
                (msg_unit_lost_ctr < 20) &&
                (player_idx == HUMAN_PLAYER_IDX)
            )
            {
                msg_unit_lost[msg_unit_lost_ctr].Unit_Type = _UNITS[itr_units].type;
                msg_unit_lost[msg_unit_lost_ctr].Cause = 2;
                msg_unit_lost_ctr++;
            }
            Kill_Unit(itr_units, kt_Dismissed);

        }
    }

    return mana_upkeep;
}


// WZD o140p11
// drake178: WIZ_RemoveCEs()
// WIZ_RemoveCEs()

// WZD o140p12
// drake178: WIZ_RemoveGEs()
// WIZ_RemoveGEs()

// WZD o140p13
// drake178: WIZ_RemoveUEs()
// WIZ_RemoveUEs()

// WZD o140p14
// drake178: N/A
// sub_C538E()

// WZD o140p15
// drake178: N/A
// sub_C53FA()


// WZD o140p16
// MoO2  Module: COLCALC  Apply_Colony_Changes_()  Apply_Colony_Pop_Growth_()
/**
 * @brief Applies per-turn population and production updates to all cities and outposts.
 *
 * @details
 * This routine performs the main end-of-turn city-state transition pass. It iterates
 * all entries in @c _CITIES[] and handles outpost progression/failure, city population
 * growth and shrinkage, production processing, and selected city-enchantment upkeep hooks.
 *
 * Processing flow per city:
 * 1. Distinguish outpost state (@c population == 0) from normal city state
 * 2. For outposts:
 *    - Destroy failed outposts when @c Pop_10s <= 0
 *    - Promote to city when @c Pop_10s >= 10, initialize size/farmers, and emit gained-city report
 * 3. For cities:
 *    - Apply growth delta from @c City_Growth_Rate()
 *    - Handle growth threshold (@c Pop_10s >= 100), increase population, recompute farmers/size,
 *      clamp neutral-city cap, and emit growth report
 *    - Handle negative population delta (@c Pop_10s < 0), reduce population where allowed,
 *      and emit death report
 *    - Apply pestilence attrition chance (additional population loss path)
 *    - Apply production resolution via @c City_Apply_Production()
 * 4. Run per-city enchantment effect hooks (implemented and placeholder SPELLY hooks)
 * 5. After all cities, update volcano bookkeeping via @c Volcano_Counts()
 *
 * @return This function returns no value.
 *
 * @note Human-facing report arrays/counters are updated for city gain, growth, loss,
 *       and death events when capacity limits allow.
 * @note City size is derived from population and clamped to @c MAX_CITY_SIZE.
 * @note Neutral city growth is capped by @c MAX_CITY_POPULATION_NEUTRAL_PLAYER.
 * @note (OGBUG) On population growth, excess @c Pop_10s above 100 is discarded by
 *       resetting @c Pop_10s to 0.
 * @note Enchantment hooks for Stream of Life, Chaos Rift, Gaia's Blessing, and
 *       Nightshade are currently placeholders in this build path.
 *
 * @see City_Growth_Rate(), City_Apply_Production(), Destroy_City(), Volcano_Counts()
 */
void gd_dump_cities(const char* point);   /* CLAUDE: GD capture (defined in INITGAME.c) */

void Apply_City_Changes(void)
{
    int16_t excess_farmer_count = 0;
    int16_t New_Min_Farmers = 0;
    int16_t Population_Growth = 0;
    int16_t itr_cities = 0;

    /* CLAUDE: GD point 622 -- _CITIES BEFORE Apply_City_Changes (outpost graduation/
     * failure + population-growth application; runs right after
     * All_Outpost_Population_Growth in Next_Turn_Calc).  Pairs with 626 to isolate
     * this pass's effect on the city Pop_/size/construction fields (the 910 city
     * divergence).  Fire once. */
    { static int gd630_done = 0;
      if(!gd630_done) { gd630_done = 1; gd_dump_cities("622_Apply_City_Changes_Entry_C"); } }

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {

        // if 'City' is 'Outpost'
        if(_CITIES[itr_cities].population == 0)  /* assume "City" is 'Outpost' */
        {

            // 'Outpost' failed
            if(_CITIES[itr_cities].Pop_10s <= 0)
            {

                if((_CITIES[itr_cities].owner_idx == HUMAN_PLAYER_IDX) && (MSG_CityLost_Count < 20))
                {

                    stu_strcpy(&MSG_CityLost_Names[(MSG_CityLost_Count * 20)], _CITIES[itr_cities].name);

                    MSG_CityLost_Count++;

                }

                Destroy_City(itr_cities);

            }

            // 'Outpost' graduated to 'City'
            if(_CITIES[itr_cities].Pop_10s >= 10)
            {

                _CITIES[itr_cities].population = 1;

                _CITIES[itr_cities].size = 1;  /* CTY_Hamlet */

                _CITIES[itr_cities].farmer_count = (int8_t)City_Minimum_Farmers(itr_cities);

                Do_City_Calculations(itr_cities);

                if(
                    (_CITIES[itr_cities].owner_idx == HUMAN_PLAYER_IDX)
                    &&
                    (MSG_CityGained_Count < 20)
                )
                {

                    MSG_CityGained_Array[MSG_CityGained_Count] = (int8_t)itr_cities;

                    MSG_CityGained_Count++;

                }

            }

        }
        else  /* (_CITIES[itr_cities].population != 0) */
        {

            // apply population growth
            Population_Growth = City_Growth_Rate(itr_cities);

            _CITIES[(itr_cities)].Pop_10s += Population_Growth;

            // increase population
            if((_CITIES[itr_cities].Pop_10s >= 100) && (_CITIES[itr_cities].population < 25))
            {

                excess_farmer_count = City_Minimum_Farmers(itr_cities);

                excess_farmer_count = (_CITIES[itr_cities].farmer_count - excess_farmer_count);

                _CITIES[itr_cities].population = (_CITIES[itr_cities].population + 1);

                _CITIES[itr_cities].Pop_10s = 0;  /* OGBUG  discards excess population */

                New_Min_Farmers = City_Minimum_Farmers(itr_cities);

                _CITIES[itr_cities].farmer_count = (New_Min_Farmers + excess_farmer_count);  /* new minimum farmers +/- too many/few */

                if(_CITIES[itr_cities].farmer_count > _CITIES[itr_cities].population)
                {

                    _CITIES[itr_cities].farmer_count = _CITIES[itr_cities].population;

                }

                _CITIES[itr_cities].size = ((_CITIES[itr_cities].population + 3) / 4);  /* {0, ..., 24} {3, ..., 27} {0, 1, 2, 3, 4, 5, 6} */

                if(_CITIES[itr_cities].size > MAX_CITY_SIZE)
                {

                    _CITIES[itr_cities].size = MAX_CITY_SIZE;  /* CTY_Capital */

                }

                if(_CITIES[itr_cities].owner_idx == NEUTRAL_PLAYER_IDX)
                {

                    /* OGBUG  In City_Growth_Rate(), uses `if(_CITIES[city_idx].population >= ((_difficulty + 1) * 2))` */
                    if(_CITIES[itr_cities].population > MAX_CITY_POPULATION_NEUTRAL_PLAYER)
                    {

                        _CITIES[itr_cities].population = MAX_CITY_POPULATION_NEUTRAL_PLAYER;

                    }

                }

                if(
                    (_CITIES[itr_cities].owner_idx == HUMAN_PLAYER_IDX)
                    &&
                    (MSG_CityGrowth_Count < 20)
                )
                {

                    MSG_CityGrowth_Array[MSG_CityGrowth_Count] = (uint8_t)itr_cities;

                    MSG_CityGrowth_Count++;

                }

            }

            // decrease population
            if(_CITIES[itr_cities].Pop_10s < 0)
            {

                if(_CITIES[itr_cities].population <= 1)  /* cant be 0 in this branch, so must be == 1 or <= -1 */
                {

                    _CITIES[itr_cities].Pop_10s = 5;

                }
                else
                {

                    _CITIES[itr_cities].population = (_CITIES[itr_cities].population - 1);

                    _CITIES[itr_cities].size = ((_CITIES[itr_cities].population + 3) / 4);

                    if((
                        _CITIES[itr_cities].owner_idx == HUMAN_PLAYER_IDX)
                        &&
                        (MSG_CityDeath_Count < 20)
                    )
                    {

                        MSG_CityDeath_Array[MSG_CityDeath_Count] = (uint8_t)itr_cities;

                        MSG_CityDeath_Count++;

                    }

                    _CITIES[itr_cities].Pop_10s = (_CITIES[itr_cities].Pop_10s + 100);

                }

            }

            // decrease population
            if(_CITIES[itr_cities].enchantments[PESTILENCE] > 0)
            {

                if(_CITIES[itr_cities].population > Random(10))
                {

                    _CITIES[itr_cities].population = (_CITIES[itr_cities].population - 1);

                    if((
                        _CITIES[itr_cities].owner_idx == HUMAN_PLAYER_IDX)
                        &&
                        (MSG_CityDeath_Count < 20)
                    )
                    {

                        MSG_CityDeath_Array[MSG_CityDeath_Count] = (uint8_t)itr_cities;

                        MSG_CityDeath_Count++;

                    }

                }

            }

            /*
                HERE:
                    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
                        if(_CITIES[itr_cities].population == 0)  / * assume "City" is 'Outpost' * /
                        else  / * (_CITIES[itr_cities].population != 0) * /
            */

            City_Apply_Production(itr_cities);
            /* CLAUDE: GD 623_City_Apply_Production -- _UNITS after City_Apply_Production
             * (line 3141), which calls Create_Unit() for completed unit builds -- bisects
             * unit creation WITHIN Apply_City_Changes.  Fire once (first city). */
            // { static int gdcap_done = 0; if(!gdcap_done)                    { gdcap_done = 1; gd_dump_units("623_City_Apply_Production_Return_U"); } }
            { static int gdcap_done = 0; if(!gdcap_done && itr_cities == 9) { gdcap_done = 1; gd_dump_units("623_City_Apply_Production_Return_U"); } }

        }


        if(_CITIES[itr_cities].enchantments[CONSECRATION] > 0)
        {
            Apply_Consecration(itr_cities);
            /* CLAUDE: GD 624_Apply_Consecration -- _UNITS after Apply_Consecration (line 3152).
             * Only fires for consecrated cities (none in the current stage-2 turn).  Fire once. */
            { static int gdcons_done = 0; if(!gdcons_done) { gdcons_done = 1; gd_dump_units("624_Apply_Consecration_Return_U"); } }
        }

        if(_CITIES[itr_cities].enchantments[STREAM_OF_LIFE] > 0)
        {
            // SPELLY  CTY_StreamOfLife(itr_cities);
        }

        if(_CITIES[itr_cities].enchantments[CHAOS_RIFT] > 0)
        {
            // SPELLY  CTY_ChaosRift(itr_cities);
        }

        if(_CITIES[itr_cities].enchantments[GAIAS_BLESSING] > 0)
        {
            // SPELLY  CTY_GaiasBlessing(itr_cities);
        }
        
        if(_CITIES[itr_cities].enchantments[NIGHTSHADE] > 0)
        {
            // SPELLY  CTY_NightshadeDispel(itr_cities);
        }
    }

    Volcano_Counts();
    /* CLAUDE: GD 625_Volcano_Counts -- _UNITS after Volcano_Counts (line 3176), which can
     * Destroy_City via volcano eruptions.  NOTE: same _UNITS state as
     * 626_Apply_City_Changes_Return_U below (nothing runs between them).  Fire once. */
    { static int gdvolc_done = 0; if(!gdvolc_done) { gdvolc_done = 1; gd_dump_units("625_Volcano_Counts_Return_U"); } }

    /* CLAUDE: GD point 626 -- _CITIES and _UNITS AFTER Apply_City_Changes (post-Volcano_Counts,
     * matching where OG returns to Next_Turn_Calc).  _UNITS too because Apply_City_Changes
     * can Destroy_City (which may delete/relocate units).  Fire once. */
    { static int gd631_done = 0;
      if(!gd631_done) { gd631_done = 1;
        gd_dump_cities("626_Apply_City_Changes_Return_C");
        gd_dump_units("626_Apply_City_Changes_Return_U"); } }

}


// WZD o140p17
// drake178: WIZ_SkillFromHeroes()
int16_t Player_Hero_Casting_Skill(int16_t player_idx)
{
    int16_t heroes_spell_casting_skill_points;
    int16_t half_hero_spell_casting_skill_points;
    int16_t itr_heroes;  // _DI_

    half_hero_spell_casting_skill_points = 0;
    heroes_spell_casting_skill_points = 0;

    for(itr_heroes = 0; itr_heroes < NUM_HEROES; itr_heroes++)
    {
        if(
            (_players[player_idx].Heroes[itr_heroes].unit_idx > -1) &&
            (_UNITS[_players[player_idx].Heroes[itr_heroes].unit_idx].wx == _FORTRESSES[player_idx].wx) &&
            (_UNITS[_players[player_idx].Heroes[itr_heroes].unit_idx].wy == _FORTRESSES[player_idx].wy) &&
            (_UNITS[_players[player_idx].Heroes[itr_heroes].unit_idx].wp == _FORTRESSES[player_idx].wp)
        )
        {
            Load_Battle_Unit(_players[player_idx].Heroes[itr_heroes].unit_idx, global_battle_unit);

            half_hero_spell_casting_skill_points = (global_battle_unit->mana_max / 2);

            heroes_spell_casting_skill_points += half_hero_spell_casting_skill_points;
        }
    }

    return heroes_spell_casting_skill_points;
}


// WZD o140p18
/*
    IDK, but pretty sure this is what progresses spell casting

    sets
        _players[itr_players].Skill_Left
        _players[itr_players].Nominal_Skill
        _players[itr_players].casting_cost_remaining
        _players[itr_players].mana_reserve

Next_Turn_Proc()
    Next_Turn_Calc()
        All_Players_Apply_Spell_Casting()

*/
void All_Players_Apply_Spell_Casting(void)
{
    int16_t itr_players = 0;  // _SI_
    int16_t magic_units = 0;  // _DI_

    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {

        if(
            (g_timestop_player_num == 0)
            ||
            (itr_players + 1) == g_timestop_player_num
        )
        {

            _players[itr_players].Skill_Left += Player_Hero_Casting_Skill(itr_players);

            _players[itr_players].Nominal_Skill = Player_Base_Casting_Skill(itr_players);

            if(_players[itr_players].casting_spell_idx > 0)
            {

                if(_players[itr_players].Skill_Left > _players[itr_players].mana_reserve)
                {
                    magic_units = _players[itr_players].mana_reserve;
                }
                else
                {
                    magic_units = _players[itr_players].Skill_Left;
                }

                if(_players[itr_players].casting_cost_remaining < magic_units)
                {
                    magic_units = _players[itr_players].casting_cost_remaining;
                }

                // Sup, logic?
                if(
                    (itr_players == HUMAN_PLAYER_IDX)
                    ||
                    (_players[itr_players].casting_spell_idx != spl_Spell_Of_Return)
                )
                {

                    _players[itr_players].casting_cost_remaining -= magic_units;

                    _players[itr_players].mana_reserve -= magic_units;

                }
                else
                {
                    /*
                        (itr_players != HUMAN_PLAYER_IDX)
                        &&
                        (_players[itr_players].casting_spell_idx == spl_Spell_Of_Return)
                    */

                    _players[itr_players].casting_cost_remaining -= _players[itr_players].Nominal_Skill;

                    SETMIN(_players[itr_players].casting_cost_remaining, 0);

                    _players[itr_players].mana_reserve -= _players[itr_players].Nominal_Skill;

                    SETMIN(_players[itr_players].mana_reserve, 0);

                }

                magic_units = 0;

                _players[itr_players].Skill_Left = 0;

                if(_players[itr_players].casting_cost_remaining > 0)
                {

                    if(_players[itr_players].Nominal_Skill > _players[itr_players].mana_reserve)
                    {

                        magic_units = _players[itr_players].mana_reserve;

                    }
                    else
                    {

                        magic_units = _players[itr_players].Nominal_Skill;

                    }

                    if(_players[itr_players].casting_cost_remaining < magic_units)
                    {

                        magic_units = _players[itr_players].casting_cost_remaining;

                    }

                    if((_players[itr_players].casting_cost_remaining - magic_units) == 0)
                    {

                        _players[itr_players].casting_cost_remaining -= magic_units;

                        _players[itr_players].mana_reserve -= magic_units;

                        _players[itr_players].Skill_Left = -(magic_units);

                    }

                }

            }

            if(_players[itr_players].Skill_Left < 0)
            {
                _players[itr_players].Skill_Left += _players[itr_players].Nominal_Skill;
            }
            else
            {
                _players[itr_players].Skill_Left = _players[itr_players].Nominal_Skill;
            }

        }

    }

}


// WZD o140p19
/**
 * @brief Evaluates and processes periodic item, mercenary, and hero offers for each wizard.
 *
 * @details
 * Iterates all active players and runs three independent offer pipelines:
 * merchant item offers, mercenary offers, and hero-for-hire offers.
 *
 * High-level sequence per player:
 * 1. Skip players currently casting @c spl_Spell_Of_Return or with inactive fortress.
 * 2. Item offer branch (human player path):
 *    - Compute chance from fame/famous status.
 *    - Optionally force via debug trigger.
 *    - Generate a random item with @c Make_Item().
 *    - Price at 3x item cost (halved if charismatic), then show merchant popup.
 *    - Remove generated item if the player cannot afford the pre-discount price.
 * 3. Mercenary offer branch (all players if unit cap allows):
 *    - Compute chance from fame/famous plus AI bonus.
 *    - Optionally force via debug trigger.
 *    - Generate mercenary offer via @c Generate_Mercenaries().
 *    - If accepted/auto-accepted and affordable, deduct gold, create units at
 *      fortress location, initialize level/xp/moves, and enforce stack limits
 *      with @c Evict_Weakest_Unit().
 * 4. Hero offer branch:
 *    - Compute chance from fame/famous, AI bonus, and current hero count divisor.
 *    - Optionally force via debug trigger.
 *    - If triggered and under unit cap, resolve hero slot + random hero type,
 *      then route through human popup or AI acceptance path.
 *
 * @return This function returns no value.
 *
 * @note Offer probability uses integer arithmetic and is clamped to a minimum
 *       baseline with @c SETMAX(..., 10) at multiple stages.
 * @note The local @c player_fame value is refreshed in the human-item branch,
 *       then reused in later branches for the current loop iteration.
 * @note Debug triggers (@c DBG_trigger_offer_item, @c DBG_trigger_offer_merc,
 *       @c DBG_trigger_offer_hero) force corresponding offer checks and grant
 *       temporary human gold for testing.
 * @note Merchant affordability check is performed before charismatic discount,
 *       preserving historical behavior.
 * @note Mercenary generated coordinates are overwritten with fortress
 *       coordinates before unit creation.
 *
 * @see Player_Fame(), Make_Item(), Merchant_Popup(), Generate_Mercenaries(),
 *      Hire_Merc_Popup(), Create_Unit(), Evict_Weakest_Unit(),
 *      Hero_Slot_Open(), Pick_Random_Hero(), Hire_Hero_Popup(), AI_Accept_Hero()
 */
void Determine_Offer(void)
{
    int16_t bookshelf[NUM_MAGIC_TYPES] = { 0, 0, 0, 0, 0 };
    int16_t item_price = 0;
    int16_t hire_response = 0;
    int16_t merc_level = 0;
    int16_t merc_cost = 0;
    int16_t merc_amount = 0;
    int16_t wp = 0;
    int16_t wy = 0;
    int16_t wx = 0;
    int16_t hero_slot = 0;
    int16_t unit_type = 0;  // used for Generate_Mercenaries() and Pick_Random_Hero()
    int16_t player_fame = 0;
    int16_t itr = 0;
    int16_t itr_players = 0;
    int16_t offer_chance_pct = 0;
    for(itr = 0; itr < NUM_MAGIC_TYPES; itr++)
    {
        bookshelf[itr] = 12;
    }
    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        if(_players[itr_players].casting_spell_idx == spl_Spell_Of_Return)
        {
            continue;
        }
        if(_FORTRESSES[itr_players].active == ST_FALSE)
        {
            continue;
        }
        if(itr_players == HUMAN_PLAYER_IDX)
        {
            player_fame = Player_Fame(itr_players);
            offer_chance_pct = (2 + (player_fame / 25));
            if(_players[itr_players].famous > 0)
            {
                offer_chance_pct = (offer_chance_pct * 2);
            }
            SETMAX(offer_chance_pct, 10);
            /* HACK: */ if(DBG_trigger_offer_item == ST_TRUE)
            {
                offer_chance_pct = 100;
                _players[HUMAN_PLAYER_IDX].gold_reserve = 9999;
                DBG_trigger_offer_item = ST_FALSE;
            }
            if(Random(100) <= offer_chance_pct)
            {
                GUI_InHeroNaming = Make_Item(0, &bookshelf[0], 0);
                item_price = (_ITEMS[GUI_InHeroNaming].cost * 3);
                // drake178: ; BUG: wrong order of operations
                if(_players[HUMAN_PLAYER_IDX].gold_reserve < item_price)
                {
                    Remove_Item(GUI_InHeroNaming);
                }
                else
                {
                    if(_players[HUMAN_PLAYER_IDX].charismatic > 0)
                    {
                        item_price = (item_price / 2);  // "half price"
                    }
                    Set_Mouse_List(1, mouse_list_default);
                    Merchant_Popup();
                    Set_Mouse_List(1, mouse_list_hourglass);
                }
            }
        }
        if(_units >= 947)
        {
            continue;
        }
        offer_chance_pct = (1 + (player_fame / 20));
        if(_players[itr_players].famous > 0)
        {
            offer_chance_pct *= 2;
        }
        SETMAX(offer_chance_pct, 10);
        if(itr_players > 0)
        {
            offer_chance_pct += 10;
        }
        /* HACK: */ if(DBG_trigger_offer_merc == ST_TRUE)
        {
            offer_chance_pct = 100;
            _players[HUMAN_PLAYER_IDX].gold_reserve = 9999;
            DBG_trigger_offer_merc = ST_FALSE;
        }
        if(Random(100) <= offer_chance_pct)
        {
            unit_type = 0;
            Generate_Mercenaries(itr_players, &wx, &wy, &wp, &merc_amount, &unit_type, &merc_cost, &merc_level);
            wx = _FORTRESSES[itr_players].wx;
            wy = _FORTRESSES[itr_players].wy;
            wp = _FORTRESSES[itr_players].wp;
            if(
                (merc_amount > 0)
                &&
                ((_units + merc_amount) < 1000)
            )
            {
                // ; conflicting condition - will always jump
                if(_players[itr_players].gold_reserve >= merc_cost)
                {
                    hire_response = ST_TRUE;
                    if(itr_players == HUMAN_PLAYER_IDX)
                    {
                        Set_Mouse_List(1, mouse_list_default);
                        hire_response = Hire_Merc_Popup(unit_type, merc_amount, merc_level, merc_cost);
                        Set_Mouse_List(1, mouse_list_hourglass);
                    }
                    if(hire_response == ST_TRUE)
                    {
                        _players[itr_players].gold_reserve -= merc_cost;
                        for(itr = 0; itr < merc_amount; itr++)
                        {
                            Create_Unit(unit_type, itr_players, wx, wy, wp, -1);
                            _UNITS[(_units - 1)].Level = (int8_t)merc_level;
                            _UNITS[(_units - 1)].XP = TBL_Experience[merc_level];
                            _UNITS[(_units - 1)].Finished = ST_FALSE;
                            _UNITS[(_units - 1)].moves2 = _UNITS[(_units - 1)].moves2_max;
                            Evict_Weakest_Unit((_units - 1));
                        }
                        if(itr_players == HUMAN_PLAYER_IDX)
                        {
                            _active_world_x = wx;
                            _active_world_y = wy;
                            _map_plane = wp;
                            o62p01_empty_function(itr_players);
                        }
                    }
                }
            }
        }
        offer_chance_pct = (3 + (player_fame / 25));
        if(_players[itr_players].famous > 0)
        {
            offer_chance_pct *= 2;
        }
        SETMAX(offer_chance_pct, 10);
        if(itr_players > HUMAN_PLAYER_IDX)
        {
            offer_chance_pct += 10;
        }
        offer_chance_pct = (offer_chance_pct / (((Player_Hero_Count(itr_players) + 1) / 2) + 1));
        /* HACK*/ if(DBG_trigger_offer_hero == ST_TRUE)
        {
            offer_chance_pct = 100;
            _players[HUMAN_PLAYER_IDX].gold_reserve = 9999;
            DBG_trigger_offer_hero = ST_FALSE;
        }
        if(
            (Random(100) <= offer_chance_pct)
            &&
            ((_units + 1) < MAX_UNIT_COUNT)
        )
        {
            hero_slot = Hero_Slot_Open(itr_players);
            unit_type = Pick_Random_Hero(itr_players, 0, 0);
            if(
                (hero_slot > ST_UNDEFINED)
                &&
                (unit_type > ST_UNDEFINED)
            )
            {
                if(itr_players == HUMAN_PLAYER_IDX)
                {
                    Set_Mouse_List(1, mouse_list_default);
                    Hire_Hero_Popup(hero_slot, unit_type, 0);
                    Set_Mouse_List(1, mouse_list_hourglass);
                }
                else
                {
                    AI_Accept_Hero(itr_players, hero_slot, unit_type);
                }
            }
        }
    }
}


// WZD o140p20
// drake178: WIZ_ResearchProgress
/*
; checks whether any of the wizards has finished their
; current research, and if so, allows picking a new one
; if possible, showing the new spell animation for the
; human player followed by the research dialog
;
; WARNING: can temporarily store a negative spell index
; as the player's researched spell
*/
/*
ALL - HP & CP

Q: Where does research_cost_remaining get updated?
A: Players_Apply_Magic_Power()
*/
void Players_Check_Spell_Research(void)
{
    int16_t itr_players = 0;
    int16_t IDK = 0;

    IDK = ST_FALSE;

    if(DBG_trigger_complete_research_spell == ST_TRUE)
    {
        _players[HUMAN_PLAYER_IDX].research_cost_remaining = 0;
        DBG_trigger_complete_research_spell = ST_FALSE;
    }

    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {

        if(
            (_players[itr_players].research_cost_remaining == 0)
            &&
            (_players[itr_players].researching_spell_idx > spl_NONE)
        )
        {

            Player_Gets_Spell(itr_players, _players[itr_players].researching_spell_idx, 1);  // learned from research

            IDK = ST_TRUE;

        }

        if(
            (_players[itr_players].researching_spell_idx < spl_NONE)
            ||
            (
                (_players[itr_players].researching_spell_idx == spl_NONE)
                &&
                (_turn < 2)
            )
        )
        {

            if(itr_players == HUMAN_PLAYER_IDX)
            {

                Set_Mouse_List(1, mouse_list_default);

                Spell_Research_Select();

                Set_Mouse_List(1, mouse_list_hourglass);

                if(IDK == ST_TRUE)
                {
                    Stop_All_Sounds__STUB();
                    Play_Background_Music();
                }

            }
            else
            {

                AI_Spell_Research_Select(itr_players);

            }

            if(_players[itr_players].researching_spell_idx == spl_Spell_Of_Mastery)
            {

                _players[itr_players].research_cost_remaining = _players[itr_players].som_research_cost;

            }
            else
            {

                _players[itr_players].research_cost_remaining = spell_data_table[_players[itr_players].researching_spell_idx].research_cost;

            }

        }

    }

}


// WZD o140p21
// MoO2  Module: COLCALC  Player_Gets_Tech_App_()
/**
 * @brief Grants a spell to a player and updates research state/UI follow-up.
 *
 * @details
 * Marks @p spell_idx as known in the player's spell list and reconciles
 * research bookkeeping for both direct research completion and non-research
 * acquisition paths (trade, treasure, conquest, or scripted grants).
 *
 * High-level behavior:
 * 1. Applies Spell of Mastery progress carry-over adjustment by reducing
 *    @c som_research_cost using half the granted spell's research cost.
 * 2. Maps @p spell_idx into realm-local index space and writes
 *    @c sls_Known in @c spells_list.
 * 3. If the granted spell was the current research target:
 *    - Removes the spell from @c research_spells.
 *    - Rebuilds available research options.
 *    - Zeros @c research_cost_remaining.
 *    - Sets @c researching_spell_idx to @c spl_NONE if no choices remain,
 *      otherwise stores the negated spell id as the legacy "needs new pick"
 *      marker.
 * 4. If the spell was not the active research target:
 *    - Removes matching entry from @c research_spells (if present) and
 *      refreshes research options.
 * 5. Handles post-grant UX/AI flow:
 *    - Human + @p New_Research true: play learn animation and restore music
 *      when appropriate.
 *    - AI + completed research target: choose next research and initialize
 *      new remaining cost.
 *
 * @param player_idx Index of the player receiving the spell.
 * @param spell_idx Spell identifier being granted.
 * @param New_Research Nonzero when the spell was newly obtained through
 *                     research completion (drives animation/UX path).
 *
 * @return This function returns no value.
 *
 * @note Mutates multiple player research fields, including
 *       @c som_research_cost, @c spells_list, @c research_spells,
 *       @c research_cost_remaining, and @c researching_spell_idx.
 * @note Preserves legacy behavior where a negative
 *       @c researching_spell_idx indicates pending reselection.
 * @note The function assumes @p spell_idx is a valid spell-table index.
 *
 * @see Player_Research_Spells(), Learn_Spell_Animation(),
 *      AI_Spell_Research_Select(), Players_Check_Spell_Research()
 */
void Player_Gets_Spell(int16_t player_idx, int16_t spell_idx, int16_t New_Research)
{
    int16_t spells_cnt = 0;
    int16_t was_research_target = 0;
    int16_t spell_realm_idx = 0;
    int16_t spell_realm = 0;
    int16_t itr = 0;
    was_research_target = ST_FALSE;
    if(_players[player_idx].som_research_cost > (spell_data_table[spell_idx].research_cost / 2))
    {
        _players[player_idx].som_research_cost -= (spell_data_table[spell_idx].research_cost / 2);
    }
    else
    {
        _players[player_idx].som_research_cost = 0;
    }
    spell_realm = ((spell_idx - 1) / NUM_SPELLS_PER_MAGIC_REALM);
    spell_realm_idx = ((spell_idx - 1) % NUM_SPELLS_PER_MAGIC_REALM);
    _players[player_idx].spells_list[((spell_realm * NUM_SPELLS_PER_MAGIC_REALM) + spell_realm_idx)] = sls_Known;
    /* Check if this spell was the one currently being researched */
    if(_players[player_idx].researching_spell_idx == spell_idx)
    {
        was_research_target = ST_TRUE;
        for(itr = 0; itr < NUM_RESEARCH_SPELLS; itr++)
        {
            if(_players[player_idx].research_spells[itr] == _players[player_idx].researching_spell_idx)
            {
                Clear_Structure(itr, (uint8_t *)&_players[player_idx].research_spells[0], sizeof(_players[player_idx].research_spells[0]), NUM_RESEARCH_SPELLS);
                _players[player_idx].research_spells[(NUM_RESEARCH_SPELLS - 1)] = 0;
            }
        }
        spells_cnt = Player_Research_Spells(player_idx);
        _players[player_idx].research_cost_remaining = 0;
        if(spells_cnt == 0)
        {
            _players[player_idx].researching_spell_idx = spl_NONE;
        }
        else
        {
            _players[player_idx].researching_spell_idx = -(_players[player_idx].researching_spell_idx);
        }
    }
    else
    {
        /* Spell was learned via other means (e.g. event, treasure, trade) */
        for(itr = 0; itr < NUM_RESEARCH_SPELLS; itr++)
        {
            if(_players[player_idx].research_spells[itr] == spell_idx)
            {
                Clear_Structure(itr, (uint8_t *)&_players[player_idx].research_spells[0], sizeof(_players[player_idx].research_spells[0]), NUM_RESEARCH_SPELLS);
                _players[player_idx].research_spells[(NUM_RESEARCH_SPELLS - 1)] = spl_NONE;
                Player_Research_Spells(player_idx);
            }
        }
    }
    if(
        (player_idx == HUMAN_PLAYER_IDX)
        &&
        (New_Research == ST_TRUE)
    )
    {
        Learn_Spell_Animation(spell_idx, was_research_target);
        if(was_research_target == ST_FALSE)
        {
            Stop_All_Sounds__STUB();
            Play_Background_Music();
        }
    }
    else if(
        (was_research_target == ST_TRUE)
        &&
        (player_idx != HUMAN_PLAYER_IDX)
    )
    {
        AI_Spell_Research_Select(player_idx);
        if(_players[player_idx].researching_spell_idx == spl_Spell_Of_Mastery)
        {
            _players[player_idx].research_cost_remaining = _players[player_idx].som_research_cost;
        }
        else
        {
            _players[player_idx].research_cost_remaining = spell_data_table[_players[player_idx].researching_spell_idx].research_cost;
        }
    }
}


// WZD o140p22
/*
Page 74  (PDF Page 79)
Unit Size and Healing
Healing rates are 5% of total hit points (of the undamaged figure) per turn when units are outside of cities, 10% when garrisoned in cities, and 15% when garrisoned in cities that have an animist’s guild. 
Finally, when units occupy map squares with Natural Healers (see Special Unit Abilities), they heal an additional 20% of their total hit points per game turn!
*/
/**
 * @brief Applies healing to a single unit using either fractional or fixed-rate logic.
 *
 * @details
 * Computes the unit's total undamaged hit-point pool as:
 * @c Figures * Unit_Hit_Points(unit_idx), then applies one of two healing modes.
 *
 * When @p flag is @c ST_FALSE:
 * - Interprets @p fraction as a divisor for per-turn healing.
 * - Converts the fractional rate into whole damage-point recovery using integer
 *   accumulation plus one probabilistic remainder roll.
 * - Clamps resulting @c _UNITS[unit_idx].Damage to a minimum of zero.
 *
 * When @p flag is nonzero:
 * - Applies a fixed 20% heal using @c hits_total / 5.
 * - Marks @c unit_heal_rate_divisors[unit_idx] as
 *   @c NATURAL_HEALER_ALREADY_APPLIED to prevent duplicate natural-healer
 *   processing in the same turn.
 *
 * @param unit_idx Index of the unit in @c _UNITS[] to heal.
 * @param fraction Healing-rate divisor used in normal mode
 *                 (for example, @c 20 => 5%, @c 10 => 10%, @c 6 => ~15%).
 * @param flag Healing mode selector:
 *             @c ST_FALSE for divisor-based healing,
 *             nonzero for fixed 20% natural-healer healing.
 *
 * @return This function returns no value.
 *
 * @note Healing is skipped in normal mode when the unit already has zero damage.
 * @note The probabilistic remainder path uses @c Random(fraction).
 * @note This routine mutates @c _UNITS[unit_idx].Damage directly.
 *
 * @see Heal_All_Units(), Unit_Hit_Points()
 */
void Heal_Unit(int16_t unit_idx, int16_t fraction, int16_t flag)
{
    int16_t hits_total = 0;
    hits_total = _unit_type_table[_UNITS[unit_idx].type].Figures * Unit_Hit_Points(unit_idx);
    if(flag == ST_FALSE)
    {
        if(_UNITS[unit_idx].Damage < hits_total)
        {
            if(_UNITS[unit_idx].Damage > 0)
            {
                while(hits_total > fraction)
                {
                    _UNITS[unit_idx].Damage -= 1;
                    hits_total -= fraction;
                }
                if(Random(fraction) <= hits_total)
                {
                    _UNITS[unit_idx].Damage -= 1;
                }
                SETMIN(_UNITS[unit_idx].Damage, 0);
            }
        }
    }
    else
    {
        _UNITS[unit_idx].Damage = (_UNITS[unit_idx].Damage - (hits_total / 5));  /* 20% */
        SETMIN(_UNITS[unit_idx].Damage, 0);
        unit_heal_rate_divisors[unit_idx] = NATURAL_HEALER_ALREADY_APPLIED;
    }
}


// WZD o140p23
/*
Page 74  (PDF Page 79)
Unit Size and Healing
Healing rates are 5% of total hit points (of the undamaged figure) per turn when units are outside of cities, 10% when garrisoned in cities, and 15% when garrisoned in cities that have an animist’s guild. 
Finally, when units occupy map squares with Natural Healers (see Special Unit Abilities), they heal an additional 20% of their total hit points per game turn!
*/
/**
 * @brief Applies end-of-turn healing to all units on the overland map.
 *
 * @details
 * Builds a per-unit healing-rate table, then performs two healing passes:
 *
 * 1. Base healing-rate assignment:
 *    - Initializes every unit to field healing (divisor 20, i.e. 5%).
 *    - For units returned by @c Army_At_City(), upgrades rate to city healing
 *      (divisor 10, i.e. 10%).
 *    - If the city has an Animist's Guild, upgrades to divisor 6 (~15%).
 *
 * 2. Unit iteration and healing application:
 *    - Applies normal fractional healing through @c Heal_Unit(..., ST_FALSE)
 *      to living, non-undead units (subject to Time Stop filtering).
 *    - For stacks containing a unit with @c UA_HEALER, applies one extra fixed
 *      20% natural-healer heal via @c Heal_Unit(..., ST_TRUE) to eligible
 *      companions not already marked as healed this turn.
 *
 * The routine also preserves known legacy behavior in the city/stack filtering
 * branches, including OGBUG paths retained for compatibility.
 *
 * @return This function returns no value.
 *
 * @note Allocates and writes @c unit_heal_rate_divisors for per-turn healing
 *       bookkeeping.
 * @note Undead and Death-realm race-type units are excluded from healing.
 * @note Natural-healer extra healing uses
 *       @c NATURAL_HEALER_ALREADY_APPLIED as a per-unit sentinel.
 * @note Under Time Stop, only units of the active Time Stop player are
 *       processed.
 *
 * @see Heal_Unit(), Army_At_City(), Army_At_Square_1()
 */
void Heal_All_Units(void)
{
    int16_t troops[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t var_16[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t troops_has_human_player_units = 0;
    int16_t troop_count = 0;
    int16_t itr_troops = 0;
    int16_t itr = 0;
    int16_t itr_units = 0;
    int16_t itr_cities = 0;
    unit_heal_rate_divisors = Allocate_First_Block(_screen_seg, (4 + (_units / 16)));
    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        unit_heal_rate_divisors[itr_units] = 20;
    }
    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        Army_At_City(itr_cities, &troop_count, &troops[0]);
        for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
        {
            /* OGBUG  conflicting condition, will always jump; Army_At_City() already filters by owner */
            if(_UNITS[troops[itr_troops]].owner_idx != _CITIES[itr_cities].owner_idx)
            {
                Kill_Unit(troops[itr_troops], kt_Disappeared);
            }
        }
        for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
        {
            unit_heal_rate_divisors[troops[itr_troops]] = 10;
        }
        if(_CITIES[itr_cities].bldg_status[ANIMISTS_GUILD] > bs_Replaced)
        {
            for (itr_troops = 0; itr_troops < troop_count; itr_troops++)
            {
                unit_heal_rate_divisors[troops[itr_troops]] = 6;
            }
        }
    }
    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(g_timestop_player_num != 0)
        {
            if(_UNITS[itr_units].owner_idx != (g_timestop_player_num - 1))
            {
                continue;
            }
        }
        if(
            (_unit_type_table[_UNITS[itr_units].type].race_type != rt_Death)
            &&
            ((_UNITS[itr_units].mutations & UM_UNDEAD) == 0)
        )
        {
            Heal_Unit(itr_units, unit_heal_rate_divisors[itr_units], ST_FALSE);
        }
        if((_unit_type_table[_UNITS[itr_units].type].Abilities & UA_HEALER) != 0)
        {
            Army_At_Square_1(_UNITS[itr_units].wx, _UNITS[itr_units].wy, _UNITS[itr_units].wp, &troop_count, &troops[0]);
            /* OGBUG  `(troop_count > MAX_STACK)` is impossible - this whole block is dead code */
            if(troop_count > MAX_STACK)
            {
                troops_has_human_player_units = ST_FALSE;
                for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
                {
                    if(_UNITS[troops[itr_troops]].owner_idx == NEUTRAL_PLAYER_IDX)
                    {
                        Kill_Unit(troops[itr_troops], kt_Disappeared);
                    }
                    if(_UNITS[troops[itr_troops]].owner_idx == HUMAN_PLAYER_IDX)
                    {
                        troops_has_human_player_units = ST_TRUE;
                    }
                }
                if(troops_has_human_player_units == ST_TRUE)
                {
                    for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
                    {
                        if(_UNITS[troops[itr_troops]].owner_idx != HUMAN_PLAYER_IDX)
                        {
                            Kill_Unit(troops[itr_troops], kt_Disappeared);
                        }
                    }
                }
                else
                {
                    Army_At_Square_1(_UNITS[itr_units].wx, _UNITS[itr_units].wy, _UNITS[itr_units].wp, &troop_count, &troops[0]);
                }
            }
            for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
            {
                if(
                    (_unit_type_table[_UNITS[troops[itr_troops]].type].race_type != rt_Death)
                    &&
                    ((_UNITS[troops[itr_troops]].mutations & UM_UNDEAD) == 0)
                    &&
                    (unit_heal_rate_divisors[troops[itr_troops]] != NATURAL_HEALER_ALREADY_APPLIED)  /* NOTE: 66 is set in Heal_Unit() */
                )
                {
                    Heal_Unit(troops[itr_troops], 0, ST_TRUE);
                }
            }
        }
    }
}


// WZD o140p24
// MoO2  Module: DIPLOMAC  Diplomacy_Growth_()
/**
 * @brief Applies per-turn diplomacy and side effects for active global enchantments.
 *
 * @details
 * Iterates all active players and processes persistent overland-enchantment
 * effects that influence diplomatic relations and global world state.
 *
 * Per-player processing includes:
 * - Resetting casting bookkeeping when no spell is currently being cast.
 * - Applying relation drift via @c Change_Relations_For_Enchantments() for
 *   each active global enchantment flag in @c _players[itr_players].Globals.
 * - Executing side-effect handlers for specific globals before relation
 *   adjustment:
 *   - @c GREAT_WASTING -> @c WIZ_GreatWasting()
 *   - @c METEOR_STORMS -> @c WIZ_MeteorStorm()
 *   - @c ARMAGEDDON -> @c WIZ_Armageddon()
 *
 * Relation-change calls use per-enchantment divisors matching legacy behavior
 * (commonly @c 10, with @c spl_Spell_Of_Mastery using @c 2).
 *
 * @return This function returns no value.
 *
 * @note The routine mutates global diplomacy state and may mutate terrain/city/
 *       unit state indirectly through world-effect spell handlers.
 * @note Processing is skipped for player slots beyond @c _num_players.
 * @note This function is part of the end-of-turn pipeline and is invoked from
 *       @c Next_Turn_Calc().
 *
 * @see Next_Turn_Calc(), Change_Relations_For_Enchantments(),
 *      WIZ_GreatWasting(), WIZ_MeteorStorm(), WIZ_Armageddon()
 */
void Diplomacy_Growth_For_Enchantments(void)
{
    int16_t itr_players = 0;
    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        if(_players[itr_players].casting_spell_idx == spl_NONE)
        {
            _players[itr_players].casting_cost_original = 0;
        }
        if(_players[itr_players].casting_spell_idx == spl_Spell_Of_Mastery)
        {
            Change_Relations_For_Enchantments(itr_players, spl_Spell_Of_Mastery, 2);
        }
        if(_players[itr_players].Globals[ETERNAL_NIGHT] > 0)
        {
            Change_Relations_For_Enchantments(itr_players, spl_Eternal_Night, 10);
        }
        if(_players[itr_players].Globals[EVIL_OMENS] > 0)
        {
            Change_Relations_For_Enchantments(itr_players, spl_Evil_Omens, 10);
        }
        if(_players[itr_players].Globals[ZOMBIE_MASTERY] > 0)
        {
            Change_Relations_For_Enchantments(itr_players, spl_Zombie_Mastery, 10);
        }
        if(_players[itr_players].Globals[AURA_OF_MAJESTY] > 0)
        {
            Change_Relations_For_Enchantments(itr_players, spl_Aura_Of_Majesty, 10);
        }
        if(_players[itr_players].Globals[WIND_MASTERY] > 0)
        {
            Change_Relations_For_Enchantments(itr_players, spl_Wind_Mastery, 10);
        }
        if(_players[itr_players].Globals[SUPPRESS_MAGIC] > 0)
        {
            Change_Relations_For_Enchantments(itr_players, spl_Suppress_Magic, 10);
        }
        if(_players[itr_players].Globals[TIME_STOP] > 0)
        {
            Change_Relations_For_Enchantments(itr_players, spl_Time_Stop, 10);
        }
        if(_players[itr_players].Globals[NATURES_AWARENESS] > 0)
        {
            Change_Relations_For_Enchantments(itr_players, spl_Natures_Awareness, 10);
        }
        if(_players[itr_players].Globals[NATURES_WRATH] > 0)
        {
            Change_Relations_For_Enchantments(itr_players, spl_Natures_Wrath, 10);
        }
        if(_players[itr_players].Globals[HERB_MASTERY] > 0)
        {
            Change_Relations_For_Enchantments(itr_players, spl_Herb_Mastery, 10);
        }
        if(_players[itr_players].Globals[CHAOS_SURGE] > 0)
        {
            Change_Relations_For_Enchantments(itr_players, spl_Chaos_Surge, 10);
        }
        if(_players[itr_players].Globals[DOOM_MASTERY] > 0)
        {
            Change_Relations_For_Enchantments(itr_players, spl_Doom_Mastery, 10);
        }

        if(_players[itr_players].Globals[GREAT_WASTING] > 0)
        {
            WIZ_GreatWasting(itr_players);
            Change_Relations_For_Enchantments(itr_players, spl_Great_Wasting, 10);
        }
        if(_players[itr_players].Globals[METEOR_STORMS] > 0)
        {
            WIZ_MeteorStorm(itr_players);
            Change_Relations_For_Enchantments(itr_players, spl_Meteor_Storms, 10);
        }
        if(_players[itr_players].Globals[ARMAGEDDON] > 0)
        {
            WIZ_Armageddon(itr_players);
            Change_Relations_For_Enchantments(itr_players, spl_Armageddon, 10);
        }

        if(_players[itr_players].Globals[TRANQUILITY] > 0)
        {
            Change_Relations_For_Enchantments(itr_players, spl_Tranquility, 10);
        }
        if(_players[itr_players].Globals[LIFE_FORCE] > 0)
        {
            Change_Relations_For_Enchantments(itr_players, spl_Life_Force, 10);
        }
        if(_players[itr_players].Globals[CRUSADE] > 0)
        {
            Change_Relations_For_Enchantments(itr_players, spl_Crusade, 10);
        }
        if(_players[itr_players].Globals[JUST_CAUSE] > 0)
        {
            Change_Relations_For_Enchantments(itr_players, spl_Just_Cause, 10);
        }
        if(_players[itr_players].Globals[HOLY_ARMS] > 0)
        {
            Change_Relations_For_Enchantments(itr_players, spl_Holy_Arms, 10);
        }
    }
}


// WZD o140p25
// MoO2  Module: INITSHIP  Repair_Ships_At_Colonies_()
// Next_Turn_Calc_() |-> Do_All_Ships_XP_Check_() |-> Best_Instructor_Bonus_(); Do_XP_For_Ship_() |-> Do_Academy_At_Location_Check_(); Calc_Ship_Level_()
/**
 * @brief Advances per-turn unit experience and related hero mastery effects.
 *
 * @details
 * Iterates all units and applies the end-of-turn XP progression path, then
 * performs hero-specific and army-specific follow-up checks. The function:
 * - Fully heals units protected by Herb Mastery, except undead/Death units.
 * - Processes Stasis mutation state transitions and resistance checks.
 * - Grants 1 XP to eligible non-fantastic, non-undead units when not blocked
 *   by Time Stop.
 * - Recomputes each unit's level through @c Calc_Unit_Level().
 * - Tracks hero-level changes for human-controlled units so the UI can react
 *   when a hero levels up.
 * - Scans each player's heroes and their stacks to apply Armsmaster-related
 *   XP bonuses when a hero is stationed with other units on the same tile.
 *
 * @return This function returns no value.
 *
 * @note Uses a static battle-unit buffer for resistance checks.
 * @note The processed hero list is backed by a one-slot offset array so the
 *       legacy @c Hero_Slot == -1 write does not underflow.
 * @note XP gain is suppressed for units outside the active Time Stop player
 *       when Time Stop is in effect.
 *
 * @see Calc_Unit_Level(), Load_Battle_Unit(), Combat_Resistance_Check(),
 *      Player_Hero_Casting_Skill()
 */
void Do_All_Units_XP_Check(void)
{
    /* TODO  init to zeroes (But, HOW!?!?!) */  static struct s_BATTLE_UNIT battle_unit;  // // sizeof: 6Eh  110d
    // int16_t processed_hero_list[NUM_HEROES] = { 0, 0, 0, 0, 0, 0 };
    /* HACK */ int16_t processed_hero_list_backing[NUM_HEROES + 1] = { 0, 0, 0, 0, 0, 0, 0 };  /* +1 leading slot absorbs the OG Hero_Slot == -1 write */
    /* HACK */ int16_t * processed_hero_list = &processed_hero_list_backing[1];
    int16_t troop_list[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t hero_level = 0;
    int16_t hero_unit_idx = 0;
    int16_t troop_count = 0;
    int16_t highest_armsmaster_xp = 0;
    int16_t xp_gain = 0;
    int16_t itr_heroes = 0;
    int16_t itr_units = 0;
    int16_t itr_players = 0;
    int16_t itr_troops = 0;
    int8_t hero_slot = 0;  // DNE in Dasm
    memset(&battle_unit, 0, sizeof(struct s_BATTLE_UNIT));
    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        // Herb Mastery:  Nature. Global Enchantment;  Casting Cost: 1000 mana;  Upkeep: 10 mana/turn. Very Rare.
        // Completely heals all of a wizard’s damaged units every game turn.
        if(
            (_players[_UNITS[itr_units].owner_idx].Globals[HERB_MASTERY] > 0)
            &&
            (_unit_type_table[_UNITS[itr_units].type].race_type != rt_Death)
            &&
            ((_UNITS[itr_units].mutations & UM_UNDEAD) == 0)
        )
        {
            _UNITS[itr_units].Damage = 0;
        }
        /*
            BEGIN:  Stasis
        */
        if((_UNITS[itr_units].mutations & C_STASISLINGER) != 0)
        {
            Load_Battle_Unit(itr_units, &battle_unit);
            if(Combat_Resistance_Check(battle_unit, -5, sbr_Sorcery) == 0)
            {
                _UNITS[itr_units].mutations ^= C_STASISLINGER;
            }
        }
        if((_UNITS[itr_units].mutations & C_STASISINIT) != 0)
        {
            _UNITS[itr_units].mutations |= C_STASISLINGER;
            _UNITS[itr_units].mutations ^= C_STASISINIT;
        }
        /*
            END:  Stasis
        */
        /*
            BEGIN:  Experience Points & Experience Level
        */
        if(
            ((_unit_type_table[_UNITS[itr_units].type].Abilities & UA_FANTASTIC) == 0)
            &&
            ((_UNITS[itr_units].mutations & UM_UNDEAD) == 0)
            &&
            (
                (g_timestop_player_num == 0)
                ||
                (_UNITS[itr_units].owner_idx == (g_timestop_player_num - 1))
            )
        )
        {
            _UNITS[itr_units].XP += 1;
            hero_level = Calc_Unit_Level(itr_units);
            hero_slot = _UNITS[itr_units].Hero_Slot;
            if(hero_slot > ST_UNDEFINED)
            {
                if(_UNITS[itr_units].owner_idx == HUMAN_PLAYER_IDX)
                {
                    if(_UNITS[itr_units].Level < hero_level)
                    {
                        Hero_LevelUp_Popup(itr_units);
                    }
                }
            }
            _UNITS[itr_units].Level = (int8_t)Calc_Unit_Level(itr_units);
        }
        /*
            END:  Experience Points & Experience Level
        */
    }
    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        for(itr_heroes = 0; itr_heroes < NUM_HEROES; itr_heroes++)
        {
            processed_hero_list[itr_heroes] = ST_FALSE;
        }
        if(
            (g_timestop_player_num == 0)
            ||
            ((g_timestop_player_num - 1) == itr_players)
        )
        {
            for(itr_heroes = 0; itr_heroes < NUM_HEROES; itr_heroes++)
            {
                if(
                    (_players[itr_players].Heroes[itr_heroes].unit_idx > -1)
                    &&
                    (processed_hero_list[itr_heroes] == ST_FALSE)
                )
                {
                    hero_unit_idx = _players[itr_players].Heroes[itr_heroes].unit_idx;
                    highest_armsmaster_xp = 0;
                    Army_At_Square_1(_UNITS[hero_unit_idx].wx, _UNITS[hero_unit_idx].wy, _UNITS[hero_unit_idx].wp, &troop_count, &troop_list[0]);
                    processed_hero_list[itr_heroes] = ST_TRUE;
                    for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
                    {
                        xp_gain = 0;
                        /*
                            ¿ BUGBUG - /RTCs ?
                            Run-Time Check Failure #2 - Stack around the variable 'processed_hero_list' was corrupted.
                        */
                        // processed_hero_list[_UNITS[troop_list[itr_troops]].Hero_Slot] = ST_TRUE;  // ; BUG: this can also be -1!
                        hero_slot = _UNITS[troop_list[itr_troops]].Hero_Slot;
                        processed_hero_list[hero_slot] = ST_TRUE;
                        if(hero_slot > ST_UNDEFINED)
                        {
                            if((_HEROES2[itr_players]->heroes[_UNITS[troop_list[itr_troops]].type].abilities & HSA_ARMSMASTER) != 0)
                            {
                                xp_gain = ((_UNITS[troop_list[itr_troops]].Level + 1) * 2);
                            }
                            else if((_HEROES2[itr_players]->heroes[_UNITS[troop_list[itr_troops]].type].abilities & HSA_ARMSMASTER2) != 0)
                            {
                                xp_gain = ((_UNITS[troop_list[itr_troops]].Level + 1) * 3);
                            }
                            if(xp_gain > highest_armsmaster_xp)
                            {
                                highest_armsmaster_xp = xp_gain;
                            }
                        }
                    }
                    if(highest_armsmaster_xp > 0)
                    {
                        for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
                        {
                            hero_slot = _UNITS[troop_list[itr_troops]].Hero_Slot;
                            if(hero_slot == ST_UNDEFINED)
                            {
                                _UNITS[troop_list[itr_troops]].XP += highest_armsmaster_xp;
                                _UNITS[troop_list[itr_troops]].Level = (int8_t)Calc_Unit_Level(troop_list[itr_troops]);
                            }
                        }
                    }
                }
            }
        }
    }
}


// WZD o140p26
/**
 * @brief Randomly converts existing volcano tiles back into mountains.
 *
 * @details
 * Scans every map square on every plane, identifies tiles whose terrain type
 * is currently @c tt_Volcano, and gives each such tile an independent 3%
 * chance to cool off during this turn. When the roll succeeds, the tile is
 * converted to a mountain via @c Set_Terrain_Type_Mountain().
 *
 * After the full-world pass completes, the function refreshes volcano-related
 * bookkeeping by calling @c Volcano_Counts().
 *
 * @return This function returns no value.
 *
 * @note Terrain classification is derived from
 *       @c p_world_map[plane][y][x] % NUM_TERRAIN_TYPES.
 * @note The cooling check is stochastic and performed once per volcano tile
 *       per end-of-turn invocation.
 * @note This function affects both Arcanus and Myrror by iterating
 *       @c NUM_PLANES.
 *
 * @see Next_Turn_Calc(), Set_Terrain_Type_Mountain(), Volcano_Counts()
 */
void Cool_Off_Volcanoes(void)
{
    int16_t terrain_type = 0;
    int16_t itr_wp = 0;
    int16_t itr_wy = 0;
    int16_t itr_wx = 0;
    for(itr_wp = 0; itr_wp < NUM_PLANES; itr_wp++)
    {
        for(itr_wx = 0; itr_wx < WORLD_WIDTH; itr_wx++)
        {
            for(itr_wy = 0; itr_wy < WORLD_HEIGHT; itr_wy++)
            {
                terrain_type = (p_world_map[itr_wp][itr_wy][itr_wx] % NUM_TERRAIN_TYPES);
                if(terrain_type == tt_Volcano)
                {
                    if(Random(100) < 3)
                    {
                        Set_Terrain_Type_Mountain(itr_wx, itr_wy, itr_wp);
                    }
                }
            }
        }
    }
    Volcano_Counts();
}


// WZD o140p27
// drake178: AI_CullTheWeak()
/*
; after turn 99, disbands any non-garrison AI unit that
; costs less than its wizard's average, and every 25
; turns, recalculates the average and disbands up to
; one such unit from each garrison too
*/
/*

IDGI  uses GUI_Multipurpose_Int to track count of culled military units, for the every turn culling

¿ units_in_city is {F,T} of whether each _units[] is in a city that is not owned by the neutral or human player ?
...to what end?

*/
void AI_Kill_Lame_Units(void)
{
    int16_t troops[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t did_remove_unit = 0;
    int16_t troop_count = 0;
    int16_t * units_in_city = 0;
    int16_t unit_owner = 0;
    int16_t unit_type = 0;
    int16_t itr_units = 0;  // _SI_
    int16_t itr_cities = 0;  // _SI_
    int16_t itr_troops = 0;  // _DI_

    if(_turn < 100)
    {
        return;
    }

    units_in_city = (int16_t *)Near_Allocate_First(_units);

    for(itr_units = 0; itr_units < _units; itr_units++)
    {

        units_in_city[itr_units] = ST_FALSE;

    }

    if((_turn % 25) != 0)
    {

        for(itr_cities = 0; itr_cities < _cities; itr_cities++)
        {

            if(
                (_CITIES[itr_cities].owner_idx == NEUTRAL_PLAYER_IDX)
                ||
                (_CITIES[itr_cities].owner_idx == HUMAN_PLAYER_IDX)
            )
            {
                continue;
            }

            Army_At_City(itr_cities, &troop_count, &troops[0]);

            for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
            {

                units_in_city[troops[itr_troops]] = ST_TRUE;

            }

        }

    }
    else
    {

        AI_Calculate_Average_Unit_Cost();

        for(itr_cities = 0; itr_cities < _cities; itr_cities++)
        {

            if(
                (_CITIES[itr_cities].owner_idx == NEUTRAL_PLAYER_IDX)
                ||
                (_CITIES[itr_cities].owner_idx == HUMAN_PLAYER_IDX)
            )
            {
                continue;
            }

            Army_At_City(itr_cities, &troop_count, &troops[0]);

            for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
            {

                units_in_city[troops[itr_troops]] = ST_TRUE;

            }

            did_remove_unit = ST_FALSE;

            for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
            {

                if(did_remove_unit == ST_FALSE)
                {

                    if(_unit_type_table[_UNITS[troops[itr_troops]].type].cost < (_players[_CITIES[itr_cities].owner_idx].average_unit_cost / 2))
                    {

                        Kill_Unit(troops[itr_troops], kt_Normal);

                        did_remove_unit = ST_TRUE;

                    }

                }

            }

        }

    }


    // ¿ does this subsequently get used in  AI_Next_Turn() ?
    // count of killed military units? doesn't inlude units killed in cities...
    GUI_Multipurpose_Int = 0;

    for(itr_units = 0; itr_units < _units; itr_units++)
    {

        unit_owner = _UNITS[itr_units].owner_idx;

        unit_type = _UNITS[itr_units].type;

        if(
            (unit_owner > HUMAN_PLAYER_IDX)
            &&
            (unit_owner < NEUTRAL_PLAYER_IDX)
        )
        {

            if(
                ((_unit_type_table[unit_type].Abilities & UA_CREATEOUTPOST) == 0)
                &&
                ((_unit_type_table[unit_type].Abilities & UA_MELD) == 0)
                &&
                (_unit_type_table[unit_type].Construction == 0)
                &&
                (_unit_type_table[unit_type].Transport == 0)
                &&
                (unit_type > ut_Trireme)  /* ¿ BUGBUG  `>=` || ut_Chosen (34) || 'Last Hero Unit Index' ? */
                &&
                (_unit_type_table[unit_type].cost < (_players[unit_owner].average_unit_cost / 2))
                &&
                (units_in_city[itr_units] == ST_FALSE)
            )
            {

                Kill_Unit(itr_units, kt_Normal);

                GUI_Multipurpose_Int++;

            }

        }

    }

}


// WZD o140p28
// drake178: AI_GetAvgUnitCosts()
// ¿ MoO2  Module: AIBUILD  Mean_Ship_Strength_() ?
/*
; calculates and sets into the wizard record the
; average combat unit value (total cost / count) of
; each AI wizard
*/
/*

only for 'Computer Player', not 'Human Player' or 'Neutral Player'
does not include Settler; Magic Spirit, Guardian Spirit; Engineer, Trireme, Galley, Catapult, Warship;
OON XREF:  AI_Kill_Lame_Units()

*/
void AI_Calculate_Average_Unit_Cost(void)
{
    int16_t unit_counts[NUM_PLAYERS] = { 0, 0, 0, 0, 0, 0 };
    int32_t unit_costs[NUM_PLAYERS] = { 0, 0, 0, 0, 0, 0 };
    int16_t unit_owner = 0;
    // int16_t itr_players__units = 0;  // _SI_
    int16_t itr_players = 0;  // _SI_
    int16_t itr_units = 0;  // _SI_
    int16_t unit_type = 0;  // _DI_

    for(itr_players = 0; itr_players < NUM_PLAYERS; itr_players++)
    {

        unit_costs[itr_players] = 0;

        unit_counts[itr_players] = 0;

    }

    for(itr_units = 0; itr_units < _units; itr_units++)
    {

        unit_owner = _UNITS[itr_units].owner_idx;

        unit_type = _UNITS[itr_units].type;

        if(
            (unit_owner > HUMAN_PLAYER_IDX)
            &&
            (unit_owner < NEUTRAL_PLAYER_IDX)
        )
        {

            // NOT Settler; Magic Spirit, Guardian Spirit; Engineer, Trireme, Galley, Catapult, Warship;
            if(
                ((_unit_type_table[unit_type].Abilities & UA_CREATEOUTPOST) == 0)
                &&
                ((_unit_type_table[unit_type].Abilities & UA_MELD) == 0)
                &&
                (_unit_type_table[unit_type].Construction == 0)
                &&
                (_unit_type_table[unit_type].Transport == 0)
            )
            {

                unit_costs[unit_owner] += _unit_type_table[unit_type].cost;

                unit_counts[unit_owner]++;

            }

        }

    }

    for(itr_players = 1; itr_players < NUM_PLAYERS; itr_players++)
    {

        if(unit_counts[itr_players] != 0)
        {

            _players[itr_players].average_unit_cost = (unit_costs[itr_players] / unit_counts[itr_players]);

        }

    }

}



/*
    WIZARDS.EXE  ovr140
*/

// WZD o141p01
// drake178:  GAME_AutoSave()
// MoO2  Module: NEXTTURN  Do_Autosave_()
/*
; if the current turn is divisible by 4, saves the game
; into slot index 8 (SAVE9.GAM), the "continue" save
; that can not be loaded from the save/load screen, but
; is started automatically by wizards.exe
*/
void Do_Autosave(void)
{
    struct s_mouse_list mouse_list[1] = { 0, 0, 0, 0, 0, 0 };

    if((_turn % 4) != 0)
    {
        return;
    }

    mouse_list[0].image_num = crsr_Hourglass;
    mouse_list[0].center_offset = 0;
    mouse_list[0].x1 = SCREEN_XMIN;
    mouse_list[0].y1 = SCREEN_YMIN;
    mouse_list[0].x2 = SCREEN_XMAX;
    mouse_list[0].y2 = SCREEN_YMAX;

    Set_Mouse_List(1, mouse_list);

    Save_SAVE_GAM(8);

    Set_Mouse_List(1, mouse_list_default);

    IDK_autosave = ST_FALSE;

}
