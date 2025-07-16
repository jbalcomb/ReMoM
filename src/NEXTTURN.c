/*
    Next Turn Procedure

    WIZARDS.EXE
        ovr060
        ovr119
        ovr121
        ovr140
        ovr141
*/

#include "MOX/LOADSAVE.h"
#include "MOX/MOX_DEF.h"
#include "MOX/MOX_DAT.h"  /* _screen_seg */
#include "MOX/MOX_SET.h"  /* magic_set */
#include "MOX/MOX_TYPE.h"
#include "MOX/Allocate.h"
#include "MOX/GENDRAW.h"
#include "MOX/LBX_Load.h"
#include "MOX/Util.h"       /* Clear_Structure(); Delete_Structure() */
#include "MOX/random.h"

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
#include "MOM_DEF.h"
#include "MOM_SCR.h"
#include "RACETYPE.h"
#include "REPORT.h"
#include "Spellbook.h"
#include "Spells128.h"
#include "Spells130.h"
#include "TerrType.h"
#include "Terrain.h"
#include "UNITSTK.h"
#include "UNITTYPE.h"
#include "UnitView.h"
#include "WZD_o059.h"
#include "WZD_o143.h"

#include "STU/STU_DBG.h"

#include <string.h>

#include "NEXTTURN.h"



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
// drake178: UNIT_HealArray
SAMB_ptr UNIT_HealArray;

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
/*
    Eh?
        for each _units
    ...reduces _units...

XREF:
Next_Turn_Proc__WIP()
Next_Turn_Proc__WIP()
j_Delete_Dead_Units()
    Loaded_Game_Update_WZD()
    Next_Turn_Calc()
    Loaded_Game_Update()

*/
void Delete_Dead_Units(void)
{
    int16_t unit_type = 0;
    int16_t itr_heroes = 0;
    int16_t itr_units = 0; // _SI_
    int16_t itr_players = 0; // _DI_

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Delete_Dead_Units()\n", __FILE__, __LINE__);
#endif

    for(itr_units = 0; itr_units < _units; itr_units++)
    {

        unit_type = _UNITS[itr_units].type;

        if(
            (_UNITS[itr_units].owner_idx < HUMAN_PLAYER_IDX)
            ||
            (_UNITS[itr_units].owner_idx > NEUTRAL_PLAYER_IDX)
        )
        {

Check_Game_Data();
            Delete_Structure(itr_units, (uint8_t *)&_UNITS[0], sizeof(struct s_UNIT), _units);
// Check_Game_Data();
Capture_Units_Data();  // TODO  Delete_Structure() changes all of the unit data

            for(itr_players = 0; itr_players < _num_players; itr_players++)
            {

                for(itr_heroes = 0; itr_heroes < NUM_HEROES; itr_heroes++)
                {

                    if(
                        (_players[itr_players].Heroes[itr_heroes].unit_idx != ST_UNDEFINED)
                        &&
                        (_players[itr_players].Heroes[itr_heroes].unit_idx > itr_units)
                    )
                    {

                        _players[itr_players].Heroes[itr_heroes].unit_idx -= 1;

                    }

                }

            }

            dbg_prn("DEBUG: [%s, %d]: Delete_Dead_Units(): _units: %d\n", __FILE__, __LINE__, _units);
            _units -= 1;
            dbg_prn("DEBUG: [%s, %d]: Delete_Dead_Units(): _units: %d\n", __FILE__, __LINE__, _units);

        }

    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Delete_Dead_Units()\n", __FILE__, __LINE__);
#endif

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
void o060p03_empty_function(void)
{
/*
push    bp
mov     bp, sp
pop     bp
retf
*/
}


// WZD o60p04
/*



*/
void Next_Turn_Proc(void)
{
    char temp_string[LEN_TEMP_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t orig_map_plane = 0;
    int16_t itr_msg = 0;  // _SI_
    int16_t curr_prod_idx = 0;  // _DI_
    int16_t DBG_itr_cities = 0;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Next_Turn_Proc()\n", __FILE__, __LINE__);
#endif


Check_Game_Data();
    Delete_Dead_Units();
Check_Game_Data();
    All_Units_In_Towers();
Check_Game_Data();


Check_Game_Data();
    Set_Unit_Draw_Priority();
Check_Game_Data();
    Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);
Check_Game_Data();
    Reset_Map_Draw();
Check_Game_Data();


Check_Game_Data();
    Next_Turn_Calc();
Check_Game_Data();


Check_Game_Data();
    Cache_Graphics_Overland();
Check_Game_Data();


Check_Game_Data();
    Delete_Dead_Units();
Check_Game_Data();
    All_Units_In_Towers();
Check_Game_Data();



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

        o62p01_empty_function(_human_player_idx);

        for(itr_msg = 0; itr_msg < g_bldg_msg_ctr; itr_msg++)
        {
            if(MSG_Building_Complete[itr_msg].city_idx != ST_UNDEFINED)
            {
                _city_idx = MSG_Building_Complete[itr_msg].city_idx;
                orig_map_plane = _map_plane;
                _map_plane = _CITIES[_city_idx].wp;
                if(MSG_Building_Complete[itr_msg].bldg_type_idx >= bt_NONE)
                {
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
                        strcpy(GUI_NearMsgString, "The ");
                        strcat(GUI_NearMsgString, _city_size_names[_CITIES[_city_idx].size]);
                        strcat(GUI_NearMsgString, " of ");
                        strcpy(near_buffer, _CITIES[_city_idx].name);
                        strcat(GUI_NearMsgString, near_buffer);
                        strcat(GUI_NearMsgString, " can no longer produce ");
                        curr_prod_idx = _CITIES[_city_idx].construction;
                        if(curr_prod_idx >= 100)
                        {
                            curr_prod_idx -= 100;
                            strcat(GUI_NearMsgString, *_unit_type_table[curr_prod_idx].name);
                        }
                        else
                        {
                            strcpy(temp_string, bldg_data_table[curr_prod_idx].name);
                            strcat(GUI_NearMsgString, An(&temp_string[0]));
                            strcat(GUI_NearMsgString, " ");
                            strcat(GUI_NearMsgString, temp_string);
                        }
                        strcat(GUI_NearMsgString, ".");
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



Check_Game_Data();
    Update_Units_MvsSts();
Capture_Units_Data();
Check_Game_Data();

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

Check_Game_Data();
        Cast_Spell_Overland__WIP(_human_player_idx);
Check_Game_Data();

    }


    all_units_moved = ST_FALSE;

Check_Game_Data();
    WIZ_NextIdleStack(_human_player_idx, &_map_x, &_map_y, &_map_plane);
Check_Game_Data();

Check_Game_Data();
    Reset_Draw_Active_Stack();
Check_Game_Data();


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Next_Turn_Proc()\n", __FILE__, __LINE__);
#endif

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
calls All_AI_Players_Contacted(), which calls Update_Scouted_And_Contacted__WIP()

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

                _UNITS[itr_units].moves2_max = Unit_Moves2(itr_units);

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

        _UNITS[itr_units].moves2_max = Unit_Moves2(itr_units);

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
    int16_t itr_players = 0;  // _SI_
    int16_t itr_units = 0;  //_DI_

    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {

        _players[(itr_players + 1)].Dipl.Contacted[HUMAN_PLAYER_IDX] = ST_TRUE;

        if(_players[(itr_players + 1)].Globals[NATURES_AWARENESS] != ST_FALSE)
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
                    ((_UNITS[itr_units].enchantments & UE_INVISIBILITY) == 0)  // BUG: only checks enchantment, not ability or item
                )
                {

                    _players[itr_players].Dipl.Contacted[_UNITS[itr_units].owner_idx] = ST_TRUE;

                    _players[_UNITS[itr_units].owner_idx].Dipl.Contacted[itr_players] = ST_TRUE;

                }

            }

        }

    }

    Update_Scouted_And_Contacted__WIP();
    // TST  Validate_Square_Scouted(18,11,0);

}



/*
    WIZARDS.EXE  ovr119
*/

// WZD o119p01
void Next_Turn_Calc(void)
{
    int16_t itr_players = 0;  // _SI_
    int16_t DBG_itr_cities = 0;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Next_Turn_Calc()\n", __FILE__, __LINE__);
#endif

Check_Game_Data();

    Set_Random_Seed(RNG_AI_Turn_Seed);

    Set_Mouse_List(1, mouse_list_hourglass);

Check_Game_Data();
    All_City_Calculations();
Check_Game_Data();

Check_Game_Data();
    AI_Kill_Lame_Units();  // ¿ BUGBUG  leaves dead/deleteable units lying around ?
Check_Game_Data();

Check_Game_Data();
    Delete_Dead_Units();  // DNE in Dasm
Check_Game_Data();

Check_Game_Data();
    AI_Next_Turn__WIP();
Check_Game_Data();

Check_Game_Data();
    Delete_Dead_Units();  // DNE in Dasm
Check_Game_Data();

Check_Game_Data();
    Next_Turn_Process_Purify();
Check_Game_Data();

Check_Game_Data();
    Initialize_Reports();
Check_Game_Data();

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

Check_Game_Data();
        Decrease_Peace_Duration();
Check_Game_Data();

Check_Game_Data();
        Update_Players_Gold_Reserve();
Check_Game_Data();

Check_Game_Data();
        Players_Update_Magic_Power();
Check_Game_Data();

Check_Game_Data();
        Players_Apply_Magic_Power();
Check_Game_Data();

Check_Game_Data();
        Players_Check_Spell_Research();
Check_Game_Data();

Check_Game_Data();
        OVL_DisableIncmBlink();
Check_Game_Data();

        if(
            (DBG_Alt_A_State == ST_FALSE)
            &&
            (magic_set.random_events == ST_TRUE)
        )
        {

Check_Game_Data();
            Determine_Event();
Check_Game_Data();

        }

Check_Game_Data();
        Event_Twiddle();
Check_Game_Data();

Check_Game_Data();
        Players_Apply_Upkeeps__WIP();
Check_Game_Data();

        // DONT  EMM_Map_DataH()

Check_Game_Data();
        All_Outpost_Population_Growth();
Check_Game_Data();

Check_Game_Data();
        Apply_City_Changes();
Check_Game_Data();

Check_Game_Data();
        Diplomacy_Growth_For_Enchantments__WIP();
Check_Game_Data();


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


Check_Game_Data();
        Determine_Offer();
Check_Game_Data();


        Set_Mouse_List(1, mouse_list_hourglass);


Check_Game_Data();
        All_City_Nightshade_Count();
Check_Game_Data();


        /*
            BEGIN:  NPC Diplomacy
        */

Check_Game_Data();
        Diplomacy_Growth();
Check_Game_Data();

Check_Game_Data();
        Determine_First_Contacts();
Check_Game_Data();

        Set_Mouse_List(1, mouse_list_hourglass);

        // TODO  NPC_To_NPC_Treaty_Negotiations__STUB();

        Set_Mouse_List(1, mouse_list_hourglass);

Check_Game_Data();
        NPC_To_Human_Diplomacy__WIP();
Check_Game_Data();

Check_Game_Data();
        Resolve_Delayed_Diplomacy_Orders();
Check_Game_Data();

        Set_Mouse_List(1, mouse_list_hourglass);

Check_Game_Data();
        End_Of_Turn_Diplomacy_Adjustments();
Check_Game_Data();

Check_Game_Data();
        Modifier_Diplomacy_Adjustments();
Check_Game_Data();

        /*
            END:  NPC Diplomacy
        */


Check_Game_Data();
        Cool_Off_Volcanoes();
Check_Game_Data();

    }


Check_Game_Data();
    All_Players_Apply_Spell_Casting();
Check_Game_Data();


Check_Game_Data();
    Delete_Dead_Units();  // ¿ here, because we may have killed units with a spell, just above ?
Capture_Units_Data();
Check_Game_Data();


Check_Game_Data();
    Set_Unit_Draw_Priority();
Check_Game_Data();

Check_Game_Data();
    Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);
Check_Game_Data();


Check_Game_Data();
    All_City_Removed_Buildings();
Capture_Cities_Data();
Check_Game_Data();


Check_Game_Data();
    Do_All_Units_XP_Check_();
Check_Game_Data();


Check_Game_Data();
    Heal_All_Units();
Check_Game_Data();


Check_Game_Data();
    Record_History();
Check_Game_Data();


Check_Game_Data();
    Increment_Background_Music();
Check_Game_Data();


    _turn++;


    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {

        SETMAX(_players[itr_players].gold_reserve, MAX_GOLD_RESERVE);

        SETMAX(_players[itr_players].mana_reserve, MAX_MANA_RESERVE);

    }


Check_Game_Data();
    OVL_EnableIncmBlink();
Check_Game_Data();
    

Check_Game_Data();
    Do_Autosave();
Check_Game_Data();


Check_Game_Data();
    All_City_Calculations();
Check_Game_Data();


    RNG_AI_Turn_Seed = Get_Random_Seed();


    Set_Mouse_List(1, mouse_list_default);


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Next_Turn_Calc()\n", __FILE__, __LINE__);
#endif

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
    int16_t itr_cities = 0;  // _SI_

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {

        Do_City_Calculations(itr_cities);

        minimum_farmer_count = City_Minimum_Farmers(itr_cities);

        if(_CITIES[itr_cities].farmer_count < minimum_farmer_count)
        {

            CITIES_FARMER_COUNT(itr_cities, minimum_farmer_count);

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
        Create_Unit__WIP(unit_type_idx, player_idx, FORTX(), FORTY(), FORTP(), -1);
    Lair_Make_Guardians()
        Create_Unit__WIP(_LAIRS[lair_idx].guard2_unit_type, NEUTRAL_PLAYER_IDX, _LAIRS[lair_idx].wx, _LAIRS[lair_idx].wy, _LAIRS[lair_idx].wp, 2000)
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
int16_t Create_Unit__WIP(int16_t unit_type, int16_t owner_idx, int16_t wx, int16_t wy, int16_t wp, int16_t R_Param)
{
    
    int16_t did_create_unit;  // DNE in Dasm
    int16_t itr;  // _DI_

    did_create_unit = ST_FALSE;

    if(
        (owner_idx == HUMAN_PLAYER_IDX)
        ||
        (_units <= 950)
        ||
        (R_Param == 2000)
    )
    {
        if(
            (R_Param == 2000)
            ||
            (_units <= 980)
        )
        {
            
            if(_units != MAX_UNIT_COUNT)
            {
                _UNITS[_units].wx = wx;
                _UNITS[_units].wy = wy;
                _UNITS[_units].wp = wp;
                _UNITS[_units].owner_idx = owner_idx;
                _UNITS[_units].moves2_max = _unit_type_table[unit_type].Move_Halves;
                _UNITS[_units].type = unit_type;
                _UNITS[_units].Hero_Slot = -1;
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
                _UNITS[_units].Rd_Constr_Left = -1;

                if((R_Param < 0) || R_Param >= 2000)
                {
                    if(R_Param < -1)
                    {
                        R_Param = (abs(R_Param) - 1);
                        _UNITS[_units].XP = TBL_Experience[R_Param];
                        _UNITS[_units].Level = Calc_Unit_Level(_units);
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

                    // TODO  double check this is a direct assignment to the bitfield
                    _UNITS[_units].mutations = City_Best_Weapon(R_Param);

                    if((_unit_type_table[unit_type].Abilities & UA_CREATEOUTPOST) != 0)
                    {

Check_Game_Data();
                        _CITIES[R_Param].population -= 1;
// Check_Game_Data();
Capture_Cities_Data();

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
                                            goto Done_Failure;
                                        }
                                    }
                                }
                            }
                            // TODO  Destroy_City(R_Param);
                        }
                    }

                    if(
                        (_players[owner_idx].alchemy > 0) &&
                        (_UNITS[_units].mutations == 0)
                    )
                    {
                        _UNITS[_units].mutations = wq_Magic;                        
                    }

                    if(
                        (_players[owner_idx].Globals[CHAOS_SURGE] > 0) &&
                        ((_unit_type_table[unit_type].Abilities & UA_FANTASTIC) == 0)
                    )
                    {
                        // TODO  UNIT_ChaosChannel(_units);
                    }

                    _UNITS[_units].Level = Calc_Unit_Level(_units);
                }

                goto Done_Success;
            }
        }
    }

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
// drake178: Calc_Nominal_Skill()
// AKA Player_Nominal_Skill()
// MGC ovr056 o56p16
// int16_t Player_Base_Casting_Skill(int16_t player_idx)
// {
// 
//     int16_t casting_skill;  // _CX_
// 
//     casting_skill = sqrt(_players[player_idx].spell_casting_skill);
// 
//     casting_skill += 1;  // STUBUG(JimBalcomb,20240113): getting 71 instead of 72; truncated? round up? imperfect square? isqrt()? CORDIC Algorithm - Successive Approximation?
// 
//     if(_players[player_idx].archmage > 0)
//     {
//         casting_skill += 10;
//     }
// 
//     return casting_skill;
// }
/*

    Nope. ¿ estimate the square root of a number ?
    Nope. ¿ the Babylonian method AKA the Newton-Raphson method ?
    Nope. ¿ CORDIC Algorithm - Successive Approximation?
    https://www.kindfortress.com/2018/04/24/design-patterns-triangular-scoring/
    https://en.wikipedia.org/wiki/Triangular_number
    https://en.wikipedia.org/wiki/File:Triangular_Numbers_Plot.svg
*/
// int16_t Player_Base_Casting_Skill__ORIG(int16_t player_idx)
int16_t Player_Base_Casting_Skill(int16_t player_idx)
{
    int32_t num = 0;
    int16_t casting_skill;  // _CX_
    int16_t twos = 0;  // _SI_

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

        if (Evil_Omens_Up == ST_TRUE)
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
    int16_t itr_hero_types;  // _SI_
    int16_t hero_count;  // _DI_

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

    unit_type_strength += Get_Effective_Hits(_unit_type_table[unit_type].Defense, temp);

    if((_unit_type_table[unit_type].Ranged_Type / 10) <= rag_Magic)
    {

        unit_type_strength += (Get_Effective_Ranged_Strength(_unit_type_table[unit_type].Ranged, _unit_type_table[unit_type].Figures, _unit_type_table[unit_type].attack_attributes) * 2);

    }

    if((_unit_type_table[unit_type].Ranged_Type / 10) >= srat_Thrown)
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
    int16_t effective_strength = 0;  // _SI_
    int16_t temp = 0;  // _DI_

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
        ||
        (global_battle_unit->ranged_type != rat_None)
    )
    {

        effective_strength += Get_Effective_Ranged_Strength(global_battle_unit->ranged, global_battle_unit->Cur_Figures, (global_battle_unit->attack_attributes | global_battle_unit->ranged_attack_attributes));

    }

    if(global_battle_unit->ranged_type >= srat_Thrown)
    {

        temp = 0;

        effective_strength += Get_Effective_Melee_Strength(global_battle_unit->melee, temp, global_battle_unit->Cur_Figures, (global_battle_unit->attack_attributes | global_battle_unit->melee_attack_attributes), ST_UNDEFINED);

    }
    else
    {

        temp = global_battle_unit->ranged;

        effective_strength += Get_Effective_Melee_Strength(global_battle_unit->melee, temp, global_battle_unit->Cur_Figures, (global_battle_unit->attack_attributes | global_battle_unit->melee_attack_attributes), global_battle_unit->ranged_type);

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
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Patch_Units_Upkeep_And_Sound()\n", __FILE__, __LINE__);
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
    dbg_prn("DEBUG: [%s, %d]: END: Patch_Units_Upkeep_And_Sound()\n", __FILE__, __LINE__);
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
    MSG_UnitLost_Count = 0;
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
// drake178: CTY_ProdProgress()
// MoO2  Module: COLCALC  Apply_Production_()
/*
    accumulate production points
        apply production points
            create building
            create unit
    do 'Grand Vizier' // do 'Computer Player' product decision/selection

*/
void City_Apply_Production(int16_t city_idx)
{
    char city_name[LEN_NAME];
    int16_t uu_troops[MAX_STACK] = { 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB };  // HACK
    int16_t uu_troop_count;
    int16_t product_cost;  // _SI_

Check_Game_Data();
    product_cost = City_Current_Product_Cost(city_idx);
Check_Game_Data();

    if(_CITIES[city_idx].population <= 0)
    {

        return;

    }

    if(_CITIES[city_idx].construction >= 100)  /* *Product* is 'Unit' */
    {

Check_Game_Data();
        _CITIES[city_idx].Prod_Accu += _CITIES[city_idx].production_units;
// Check_Game_Data();
Capture_Cities_Data();

        if(_CITIES[city_idx].Prod_Accu >= product_cost)
        {
            
            if((_units + 1) < MAX_UNIT_COUNT)
            {

Check_Game_Data();
                Create_Unit__WIP((_CITIES[city_idx].construction - 100), _CITIES[city_idx].owner_idx, _CITIES[city_idx].wx, _CITIES[city_idx].wy, _CITIES[city_idx].wp, city_idx);
Capture_Units_Data();
Check_Game_Data();

Check_Game_Data();
                UNIT_RemoveExcess((_units - 1));
Capture_Units_Data();
Check_Game_Data();

Check_Game_Data();
                Army_At_City(city_idx, &uu_troop_count, &uu_troops[0]);
Check_Game_Data();

                if(
                    (_CITIES[city_idx].owner_idx != HUMAN_PLAYER_IDX)
                    ||
                    (grand_vizier == ST_TRUE)
                )
                {

Check_Game_Data();
                    _CITIES[city_idx].construction = bt_AUTOBUILD;
// Check_Game_Data();
Capture_Cities_Data();

                }

            }
            else
            {

                if(_CITIES[city_idx].owner_idx != HUMAN_PLAYER_IDX)
                {

Check_Game_Data();
                    _CITIES[city_idx].construction = bt_AUTOBUILD;
Check_Game_Data();

                }
                else
                {

Check_Game_Data();
                    LBX_Load_Data_Static(message_lbx_file, 0, (SAMB_ptr)GUI_NearMsgString, 66, 1, 150);  // "Maximum number of units exceeded"
Check_Game_Data();

Check_Game_Data();
                    strcpy(city_name, _CITIES[city_idx].name);
Check_Game_Data();

Check_Game_Data();
                    strcat(GUI_NearMsgString, city_name);
Check_Game_Data();

Check_Game_Data();
                    strcat(GUI_NearMsgString, cnst_TooManyUnits);  // ". You must disband some units if you wish to build or summon any more."
Check_Game_Data();

Check_Game_Data();
                    Warn0(GUI_NearMsgString);
Check_Game_Data();

                    if(
                        (_CITIES[city_idx].owner_idx == HUMAN_PLAYER_IDX)
                        &&
                        (grand_vizier == ST_TRUE)
                    )
                    {

Check_Game_Data();
                        _CITIES[city_idx].construction = bt_AUTOBUILD;
Check_Game_Data();

                    }
                    else
                    {

Check_Game_Data();
                        _CITIES[city_idx].construction = bt_TradeGoods;
Check_Game_Data();

                    }
                }

            }


Check_Game_Data();
            _CITIES[city_idx].Prod_Accu = 0;  // BUGBUG ¿ drake178: discards excess ? not actually a bug, just prescribed behavior? "surplus production units will be lost"
// Check_Game_Data();
Capture_Cities_Data();

        }

    }
    else  /* *Product* is 'Building' */
    {

        if(_CITIES[city_idx].construction < bt_Barracks)  /* ~== not a *real* building */
        {

            if(_CITIES[city_idx].owner_idx != HUMAN_PLAYER_IDX)
            {

Check_Game_Data();
                _CITIES[city_idx].construction = bt_AUTOBUILD;
// Check_Game_Data();
Capture_Cities_Data();

            }

        }
        else
        {

            if(_CITIES[city_idx].owner_idx == NEUTRAL_PLAYER_IDX)
            {

Check_Game_Data();
                _CITIES[city_idx].Prod_Accu += (_CITIES[city_idx].production_units / 2);
// Check_Game_Data();
Capture_Cities_Data();

            }
            else
            {

Check_Game_Data();
                _CITIES[city_idx].Prod_Accu += _CITIES[city_idx].production_units;
// Check_Game_Data();
Capture_Cities_Data();

            }

            if(_CITIES[city_idx].Prod_Accu >= product_cost)
            {

// Severity	Code	Description	Project	File	Line	Suppression State	Details
// Warning	C6385	Reading invalid data from '_CITIES[city_idx].bldg_status'.ReMoM	C : \STU\devel\ReMoM\src\NEXTTURN.C	1934
                // NOTE(JimBalcomb,20250221):  just got triggered, don't recall why I had this here...  assert((_CITIES[city_idx].bldg_status[_CITIES[city_idx].construction] < 0));

                // IDGI:  ¿ impossible state - unreachable code ?
                // #CRASHME
                // BUGBUG
                if(_CITIES[city_idx].bldg_status[_CITIES[city_idx].construction] >= 0)  /* bs_Replaced, bs_Built, bs_Removed */
                {

Check_Game_Data();
                    _CITIES[city_idx].bldg_status[_CITIES[city_idx].construction] += 1;
// Check_Game_Data();
Capture_Cities_Data();

                }
                else
                {

Check_Game_Data();
                    _CITIES[city_idx].bldg_status[_CITIES[city_idx].construction] = bs_Built;
// Check_Game_Data();
Capture_Cities_Data();

                    if(bldg_data_table[_CITIES[city_idx].construction].replace_bldg != ST_UNDEFINED)
                    {

Check_Game_Data();
                        _CITIES[city_idx].bldg_status[bldg_data_table[_CITIES[city_idx].construction].replace_bldg] = bs_Replaced;
// Check_Game_Data();
Capture_Cities_Data();

                    }

                    if(
                        (_CITIES[city_idx].construction == bt_Oracle)
                        &&
                        (_CITIES[city_idx].owner_idx == HUMAN_PLAYER_IDX)
                    )
                    {

Check_Game_Data();
                        Set_Map_Square_Explored_Flags_XYP_Range(_CITIES[city_idx].wx, _CITIES[city_idx].wy, _CITIES[city_idx].wp, 6);
Check_Game_Data();

                    }

Check_Game_Data();
                    _CITIES[city_idx].bldg_cnt += 1;
// Check_Game_Data();
Capture_Cities_Data();

Check_Game_Data();
                    _CITIES[city_idx].Prod_Accu = 0;
// Check_Game_Data();
Capture_Cities_Data();

                    if(_CITIES[city_idx].owner_idx != HUMAN_PLAYER_IDX)
                    {

Check_Game_Data();
                        _CITIES[city_idx].construction = bt_AUTOBUILD;
// Check_Game_Data();
Capture_Cities_Data();

                    }
                    else
                    {

                        if(g_bldg_msg_ctr < 20)
                        {

                            MSG_Building_Complete[g_bldg_msg_ctr].city_idx = city_idx;

                            MSG_Building_Complete[g_bldg_msg_ctr].bldg_type_idx = _CITIES[city_idx].construction;

                            g_bldg_msg_ctr++;

                        }

                        if(grand_vizier == ST_TRUE)
                        {

Check_Game_Data();
                            _CITIES[city_idx].construction = bt_AUTOBUILD;
Check_Game_Data();

                        }
                        else
                        {

Check_Game_Data();
                            _CITIES[city_idx].construction = bt_Housing;
// Check_Game_Data();
Capture_Cities_Data();

                        }
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

Check_Game_Data();
        Player_Colony_Autobuild_HP(city_idx);
Check_Game_Data();

    }

Check_Game_Data();

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

Check_Game_Data();
            WIZ_MatchFoodUpkeep__WIP(itr, excess_foods[itr], food_upkeep);
Check_Game_Data();

        }

        gold_upkeeps[itr] = Player_Armies_Gold_Upkeep(itr);

        if(
            (gold_upkeeps[itr] > _players[itr].gold_reserve)
            &&
            (itr == 0)
        )
        {

Check_Game_Data();
            gold_upkeeps[itr] = WIZ_MatchGoldUpkeep(itr, gold_upkeeps[itr]);
Check_Game_Data();

        }

        _players[itr].gold_reserve = (_players[itr].gold_reserve - gold_upkeeps[itr]);

        if(_players[itr].gold_reserve < 0)
        {

            _players[itr].gold_reserve = 0;

        }

Check_Game_Data();
        mana_upkeeps[itr] = Player_Armies_And_Enchantments_Mana_Upkeep(itr);
Check_Game_Data();

        if(
            (mana_upkeeps[itr] > _players[itr].mana_reserve)
            &&
            (itr == 0)
        )
        {

Check_Game_Data();
            mana_upkeeps[itr] = WIZ_MatchManaUpkeep__WIP(itr, mana_upkeeps[itr]);
Check_Game_Data();

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

Check_Game_Data();
            UNIT_GetDependants__WIP(itr_units, &troop_count, &troops[0]);
Check_Game_Data();

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
                            (MSG_UnitLost_Count < 20)
                            &&
                            (player_idx == HUMAN_PLAYER_IDX)
                        )
                        {

                            MSG_UnitLost_Array[MSG_UnitLost_Count].Unit_Type = _UNITS[itr_units].type;

                            MSG_UnitLost_Array[MSG_UnitLost_Count].Cause = 4;

                            MSG_UnitLost_Count++;

                        }

Check_Game_Data();
                        Kill_Unit(troops[itr_troops], kt_Normal);
// TODO  Check_Game_Data();
Capture_Units_Data();

                    }

                }

            }

        }

        if(
            (MSG_UnitLost_Count < 20)
            &&
            (player_idx == HUMAN_PLAYER_IDX)
        )
        {

            MSG_UnitLost_Array[MSG_UnitLost_Count].Unit_Type = _UNITS[itr_units].type;

            MSG_UnitLost_Array[MSG_UnitLost_Count].Cause = 0;

            MSG_UnitLost_Count++;

        }

Check_Game_Data();
        Kill_Unit(itr_units, kt_Dismissed);
// TODO  Check_Game_Data();
Capture_Units_Data();

    }

}


// WZD o140p08
// drake178: WIZ_MatchGoldUpkeep()
int16_t WIZ_MatchGoldUpkeep(int16_t player_idx, int16_t gold_upkeep)
{
    int16_t troops[MAX_STACK];
    int16_t itr_troops;
    int16_t troop_count;
    int16_t unit_gold_upkeep;
    int16_t itr_units;  // _SI_

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

Check_Game_Data();
                UNIT_GetDependants__WIP(itr_units, &troop_count, &troops[0]);
Check_Game_Data();

                if(troop_count > 0)
                {
                    for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
                    {

                        gold_upkeep -= Unit_Gold_Upkeep(troops[itr_troops]);
                        
                        if(
                            (MSG_UnitLost_Count < 20) &&
                            (player_idx == HUMAN_PLAYER_IDX)
                        )
                        {
                            MSG_UnitLost_Array[MSG_UnitLost_Count].Unit_Type = _UNITS[itr_units].type;
                            MSG_UnitLost_Array[MSG_UnitLost_Count].Cause = 4;
                            MSG_UnitLost_Count++;
                        }
Check_Game_Data();
                        Kill_Unit(troops[itr_troops], kt_Normal);
// TODO  Check_Game_Data();
Capture_Units_Data();

                    }
                }

                if(
                    (MSG_UnitLost_Count < 20) &&
                    (player_idx == HUMAN_PLAYER_IDX)
                )
                {
                    MSG_UnitLost_Array[MSG_UnitLost_Count].Unit_Type = _UNITS[itr_units].type;
                    MSG_UnitLost_Array[MSG_UnitLost_Count].Cause = 1;
                    MSG_UnitLost_Count++;
                }

Check_Game_Data();
                Kill_Unit(itr_units, kt_Dismissed);
// TODO  Check_Game_Data();
Capture_Units_Data();

            }
        }
    }

    return gold_upkeep;
}


// WZD o140p09
// drake178: WIZ_MatchManaUpkeep()
int16_t WIZ_MatchManaUpkeep__WIP(int16_t player_idx, int16_t mana_upkeep)
{
    int16_t Asset_Type;
    int16_t Asset_Types_Checked;
    int16_t mana_expense_type[4];
    int16_t itr;

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
    int16_t troops[MAX_STACK];
    int16_t Channeler_Divisor;
    int16_t itr_troops;
    int16_t troop_count;
    int16_t itr_units;  // _SI_

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

Check_Game_Data();
            UNIT_GetDependants__WIP(itr_units, &troop_count, &troops[0]);
Check_Game_Data();

            if(troop_count > 0)
            {
                for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
                {

                    mana_upkeep -= (Unit_Mana_Upkeep(itr_units) / Channeler_Divisor);
                    
                    if(
                        (MSG_UnitLost_Count < 20) &&
                        (player_idx == HUMAN_PLAYER_IDX)
                    )
                    {
                        MSG_UnitLost_Array[MSG_UnitLost_Count].Unit_Type = _UNITS[itr_units].type;
                        MSG_UnitLost_Array[MSG_UnitLost_Count].Cause = 4;
                        MSG_UnitLost_Count++;
                    }
Check_Game_Data();
                    Kill_Unit(troops[itr_troops], kt_Normal);
// TODO  Check_Game_Data();
Capture_Units_Data();
                }
            }

            if(
                (MSG_UnitLost_Count < 20) &&
                (player_idx == HUMAN_PLAYER_IDX)
            )
            {
                MSG_UnitLost_Array[MSG_UnitLost_Count].Unit_Type = _UNITS[itr_units].type;
                MSG_UnitLost_Array[MSG_UnitLost_Count].Cause = 2;
                MSG_UnitLost_Count++;
            }
Check_Game_Data();
            Kill_Unit(itr_units, kt_Dismissed);
// TODO  Check_Game_Data();
Capture_Units_Data();

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
// drake178: CTY_ProgressTurn()
// MoO2  Module: COLCALC  Apply_Colony_Changes_()  Apply_Colony_Pop_Growth_()
/*
    Outpost loss
    Outpost graduation  (NOT growth)
    City population growth
    City size increase
    ¿ City size 1 Pop_10s default 5 ?
    City size decrease
    Pestilence City size decrease

*/
void Apply_City_Changes(void)
{
    int16_t excess_farmer_count = 0;
    int16_t New_Min_Farmers = 0;
    int16_t Population_Growth = 0;
    int16_t itr_cities = 0;
Check_Game_Data();

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
Check_Game_Data();

        // if 'City' is 'Outpost'
        if(_CITIES[itr_cities].population == 0)  /* assume "City" is 'Outpost' */
        {
Check_Game_Data();

            // 'Outpost' failed
            if(_CITIES[itr_cities].Pop_10s <= 0)
            {

                if((_CITIES[itr_cities].owner_idx == HUMAN_PLAYER_IDX) && (MSG_CityLost_Count < 20))
                {

                    strcpy(&MSG_CityLost_Names[(MSG_CityLost_Count * 20)], _CITIES[itr_cities].name);

                    MSG_CityLost_Count++;

                }

Check_Game_Data();
                Destroy_City(itr_cities);
Check_Game_Data();

            }

            // 'Outpost' graduated to 'City'
            if(_CITIES[itr_cities].Pop_10s >= 10)
            {

                CITIES_POPULATION(itr_cities, 1);

                CITIES_SIZE(itr_cities, 1);  /* CTY_Hamlet */

                CITIES_FARMER_COUNT(itr_cities, City_Minimum_Farmers(itr_cities));

Check_Game_Data();
                Do_City_Calculations(itr_cities);
Check_Game_Data();

                if(
                    (_CITIES[itr_cities].owner_idx == HUMAN_PLAYER_IDX)
                    &&
                    (MSG_CityGained_Count < 20)
                )
                {

                    MSG_CityGained_Array[MSG_CityGained_Count] = itr_cities;

                    MSG_CityGained_Count++;

                }

            }

        }
        else  /* (_CITIES[itr_cities].population != 0) */
        {

Check_Game_Data();
            // apply population growth
            Population_Growth = City_Growth_Rate(itr_cities);

Check_Game_Data();
            CITIES_POP_10S(itr_cities, (_CITIES[itr_cities].Pop_10s + Population_Growth));

            // increase population
            if((_CITIES[itr_cities].Pop_10s >= 100) && (_CITIES[itr_cities].population < 25))
            {

                excess_farmer_count = City_Minimum_Farmers(itr_cities);

                excess_farmer_count = (_CITIES[itr_cities].farmer_count - excess_farmer_count);

                CITIES_POPULATION(itr_cities, (_CITIES[itr_cities].population + 1));

                CITIES_POP_10S(itr_cities, 0);  /* ; BUG: discards excess population */

                New_Min_Farmers = City_Minimum_Farmers(itr_cities);

                CITIES_FARMER_COUNT(itr_cities, (New_Min_Farmers + excess_farmer_count));  /* new minimum farmers +/- too many/few */

                if(_CITIES[itr_cities].farmer_count > _CITIES[itr_cities].population)
                {

                    CITIES_FARMER_COUNT(itr_cities, _CITIES[itr_cities].population);

                }

                CITIES_SIZE(itr_cities, ((_CITIES[itr_cities].population + 3) / 4));  /* {0, ..., 24} {3, ..., 27} {0, 1, 2, 3, 4, 5, 6} */

                if(_CITIES[itr_cities].size > MAX_CITY_SIZE)
                {

                    CITIES_SIZE(itr_cities, MAX_CITY_SIZE);  /* CTY_Capital */

                }

                if(_CITIES[itr_cities].owner_idx == NEUTRAL_PLAYER_IDX)
                {

                    // BUGBUG:  In City_Growth_Rate(), uses `if(_CITIES[city_idx].population >= ((_difficulty + 1) * 2))`
                    if(_CITIES[itr_cities].population > MAX_CITY_POPULATION_NEUTRAL_PLAYER)
                    {

                        CITIES_POPULATION(itr_cities, MAX_CITY_POPULATION_NEUTRAL_PLAYER);

                    }

                }

                if(
                    (_CITIES[itr_cities].owner_idx == HUMAN_PLAYER_IDX)
                    &&
                    (MSG_CityGrowth_Count < 20)
                )
                {

                    MSG_CityGrowth_Array[MSG_CityGrowth_Count] = itr_cities;

                    MSG_CityGrowth_Count++;

                }

            }

            // decrease population
            if(_CITIES[itr_cities].Pop_10s < 0)
            {

                if(_CITIES[itr_cities].population <= 1)  /* cant be 0 in this branch, so must be == 1 or <= -1 */
                {

                    CITIES_POP_10S(itr_cities, 5);

                }
                else
                {

                    CITIES_POPULATION(itr_cities, (_CITIES[itr_cities].population - 1));

                    CITIES_SIZE(itr_cities, ((_CITIES[itr_cities].population + 3) / 4));

                    if((
                        _CITIES[itr_cities].owner_idx == HUMAN_PLAYER_IDX)
                        &&
                        (MSG_CityDeath_Count < 20)
                    )
                    {

                        MSG_CityDeath_Array[MSG_CityDeath_Count] = itr_cities;

                        MSG_CityDeath_Count++;

                    }

                    CITIES_POP_10S(itr_cities, (_CITIES[itr_cities].Pop_10s + 100));

                }

            }

            // decrease population
            if(_CITIES[itr_cities].enchantments[PESTILENCE] > 0)
            {

                if(_CITIES[itr_cities].population > Random(10))
                {

                    CITIES_POPULATION(itr_cities, (_CITIES[itr_cities].population - 1));

                    if((
                        _CITIES[itr_cities].owner_idx == HUMAN_PLAYER_IDX)
                        &&
                        (MSG_CityDeath_Count < 20)
                    )
                    {

                        MSG_CityDeath_Array[MSG_CityDeath_Count] = itr_cities;

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

Check_Game_Data();
            City_Apply_Production(itr_cities);
Check_Game_Data();

        }


        if(_CITIES[itr_cities].enchantments[CONSECRATION] > 0)
        {
Check_Game_Data();
            Apply_Consecration(itr_cities);
Check_Game_Data();
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

Check_Game_Data();
    Volcano_Counts();
Check_Game_Data();

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
// drake178: EVNT_RandomOffers()
/*
; processes the wandering merchant, mercenary, and hero for hire offers for all players
; has multiple BUGs, both own and inherited
*/
/*

*/
void Determine_Offer(void)
{
    int16_t Bookshelf[NUM_MAGIC_TYPES] = { 0, 0, 0, 0, 0 };
    int16_t item_price = 0;
    int16_t Hire_Response = 0;
    int16_t Merc_Level = 0;
    int16_t Merc_Cost = 0;
    int16_t Merc_Amount = 0;
    int16_t wp = 0;
    int16_t wy = 0;
    int16_t wx = 0;
    int16_t Hero_Slot = 0;
    int16_t unit_type = 0;  // used for Generate_Mercenaries() and Pick_Random_Hero()
    int16_t player_fame = 0;
    int16_t itr = 0;
    int16_t itr_players = 0;  // _SI_
    int16_t IDK = 0;  // _DI_

    for(itr = 0; itr < NUM_MAGIC_TYPES; itr++)
    {
        Bookshelf[itr] = 12;
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

            IDK = (2 + (player_fame / 25));

            if(_players[itr_players].famous > 0)
            {
                IDK = (IDK * 2);
            }

            SETMAX(IDK, 10);

            
            /* HACK: */ if(DBG_trigger_offer_item == ST_TRUE)
            {
                IDK = 100;
                _players[HUMAN_PLAYER_IDX].gold_reserve = 9999;
                DBG_trigger_offer_item = ST_FALSE;
            }

            if(Random(100) <= IDK)
            {

                GUI_InHeroNaming = Make_Item(0, &Bookshelf[0], 0);

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

        IDK = (1 + (player_fame / 20));

        if(_players[itr_players].famous > 0)
        {
            IDK *= 2;
        }

        SETMAX(IDK, 10);

        if(itr_players > 0)
        {
            IDK += 10;
        }

        /* HACK: */ if(DBG_trigger_offer_merc == ST_TRUE)
        {
            IDK = 100;
            _players[HUMAN_PLAYER_IDX].gold_reserve = 9999;
            DBG_trigger_offer_merc = ST_FALSE;
        }

        if(Random(100) <= IDK)
        {

            unit_type = 0;

Check_Game_Data();
            Generate_Mercenaries(itr_players, &wx, &wy, &wp, &Merc_Amount, &unit_type, &Merc_Cost, &Merc_Level);
Check_Game_Data();

            wx = _FORTRESSES[itr_players].wx;
            wy = _FORTRESSES[itr_players].wy;
            wp = _FORTRESSES[itr_players].wp;

            if(
                (Merc_Amount > 0)
                &&
                ((_units + Merc_Amount) < 1000)
            )
            {

                // ; conflicting condition - will always jump
                if(_players[itr_players].gold_reserve >= Merc_Cost)
                {

                    Hire_Response = ST_TRUE;

                    if(itr_players == HUMAN_PLAYER_IDX)
                    {

                        Set_Mouse_List(1, mouse_list_default);

Check_Game_Data();
                        Hire_Response = Hire_Merc_Popup(unit_type, Merc_Amount, Merc_Level, Merc_Cost);
Check_Game_Data();

                        Set_Mouse_List(1, mouse_list_hourglass);

                    }

                    if(Hire_Response == ST_TRUE)
                    {

                        _players[itr_players].gold_reserve -= Merc_Cost;

Check_Game_Data();
                        for(itr = 0; itr < Merc_Amount; itr++)
                        {

                            Create_Unit__WIP(unit_type, itr_players, wx, wy, wp, -1);

                            _UNITS[(_units - 1)].Level = Merc_Level;

                            _UNITS[(_units - 1)].XP = TBL_Experience[Merc_Level];

                            _UNITS[(_units - 1)].Finished = 0;

                            _UNITS[(_units - 1)].moves2 = _UNITS[(_units - 1)].moves2_max;

                            UNIT_RemoveExcess((_units - 1));

                        }
Capture_Units_Data();
Check_Game_Data();

                        if(itr_players == HUMAN_PLAYER_IDX)
                        {

                            _active_world_x = wx;
                            _active_world_y = wy;
                            _map_plane = wp;

                            // DONT  o62p01_Empty_pFxn(itr_players);

                        }

                    }

                }

            }

        }

        IDK = (3 + (player_fame / 25));

        if(_players[itr_players].famous > 0)
        {
            IDK *= 2;
        }

        SETMAX(IDK, 10);

        if(itr_players > HUMAN_PLAYER_IDX)
        {
            IDK += 10;
        }

        IDK = (IDK / (((Player_Hero_Count(itr_players) + 1) / 2) + 1));

        /* HACK*/ if(DBG_trigger_offer_hero == ST_TRUE)
        {
            IDK = 100;
            _players[HUMAN_PLAYER_IDX].gold_reserve = 9999;
            DBG_trigger_offer_hero = ST_FALSE;
        }

        if(
            (Random(100) <= IDK)
            &&
            ((_units + 1) < MAX_UNIT_COUNT)
        )
        {

            Hero_Slot = Hero_Slot_Open(itr_players);

            unit_type = Pick_Random_Hero(itr_players, 0, 0);

            if(
                (Hero_Slot > -1)
                &&
                (unit_type > -1)
            )
            {

                if(itr_players == HUMAN_PLAYER_IDX)
                {

                    Set_Mouse_List(1, mouse_list_default);

Check_Game_Data();
                    Hire_Hero_Popup(Hero_Slot, unit_type, 0);
Check_Game_Data();

                    Set_Mouse_List(1, mouse_list_hourglass);

                }
                else
                {

Check_Game_Data();
                    AI_Accept_Hero(itr_players, Hero_Slot, unit_type);
Check_Game_Data();

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


Q: Where does research_cost_remaining get updated?
A: Players_Apply_Magic_Power()
*/
void Players_Check_Spell_Research(void)
{
    int16_t itr_players = 0;  // _SI_
    int16_t IDK = 0;  // _DI_

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
                    // DOMSDOS  Stop_All_Sounds__STUB();
                    // DOMSDOS  Play_Background_Music__STUB();
                    sdl2_Play_Background_Music__WIP();
                }

            }
            else
            {

                AI_Research_Picker__STUB(itr_players);

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
/*
    WIP: needs GAME_LearnSpellAnim() and AI_Research_Picker()

handles researched spell, traded/gifted spell, treasure spell, and conquest spell

*/
void Player_Gets_Spell(int16_t player_idx, int16_t spell_idx, int16_t New_Research)
{
    int16_t spells_cnt;
    int16_t Was_Research_Target;
    int16_t spell_realm_idx;
    int16_t spell_realm;
    int16_t itr;  // _DI_

    Was_Research_Target = ST_FALSE;

    if(_players[player_idx].som_research_cost > spell_data_table[spell_idx].research_cost)
    {
        _players[player_idx].som_research_cost = spell_data_table[spell_idx].research_cost;
    }
    else
    {
        _players[player_idx].som_research_cost = 0;
    }

    spell_realm = ((spell_idx - 1) / NUM_SPELLS_PER_MAGIC_REALM);

    spell_realm_idx = ((spell_idx - 1) % NUM_SPELLS_PER_MAGIC_REALM);

    _players[player_idx].spells_list[((spell_realm * NUM_SPELLS_PER_MAGIC_REALM) + spell_realm_idx)] = sls_Known;

    if(_players[player_idx].researching_spell_idx == spell_idx)
    {
        Was_Research_Target = ST_TRUE;

        for(itr = 0; itr < NUM_RESEARCH_SPELLS; itr++)
        {
            if(_players[player_idx].research_spells[itr] == _players[player_idx].researching_spell_idx)
            {
                Clear_Structure(itr, (uint8_t *)&_players[player_idx].research_spells[0], sizeof(_players[player_idx].research_spells[0]), NUM_RESEARCH_SPELLS);
                _players[player_idx].research_spells[(NUM_RESEARCH_SPELLS - 1)] = 0; // BUG  ¿ drake178: ; already done above ?
            }
        }

        spells_cnt = Player_Research_Spells(player_idx);

        _players[player_idx].research_cost_remaining = 0;

        if(spells_cnt == 0)
        {
            _players[player_idx].researching_spell_idx = spl_UNDEFINED;
        }
        else
        {
            _players[player_idx].researching_spell_idx = -(_players[player_idx].researching_spell_idx);
        }
    }
    else
    {
        // BUGBUG  ¿ refactor or this just shouldn't being doing the same as above ?
        for(itr = 0; itr < NUM_RESEARCH_SPELLS; itr++)
        {
            if(_players[player_idx].research_spells[itr] == _players[player_idx].researching_spell_idx)
            {
                Clear_Structure(itr, (uint8_t *)&_players[player_idx].research_spells[0], sizeof(_players[player_idx].research_spells[0]), NUM_RESEARCH_SPELLS);
                _players[player_idx].research_spells[(NUM_RESEARCH_SPELLS - 1)] = spl_NONE; // BUG  ¿ drake178: ; this is already done by the above function ?
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

        Learn_Spell_Animation(spell_idx, Was_Research_Target);

        if(Was_Research_Target == ST_FALSE)
        {
            // DOMSDOS  Stop_All_Sounds__STUB();
            // DOMSDOS  Play_Background_Music__STUB();
            sdl2_Play_Background_Music__WIP();
        }

    }
    else if(
        (Was_Research_Target == ST_TRUE)
        &&
        (player_idx != HUMAN_PLAYER_IDX)
    )
    {

        AI_Research_Picker__STUB(player_idx);

        if(_players[player_idx].researching_spell_idx == spl_Spell_Of_Mastery)
        {

            _players[player_idx].research_cost_remaining = _players[player_idx].som_research_cost;

        }
        else
        {

            _players[player_idx].research_cost_remaining = spell_data_table[spell_idx].research_cost;
            
        }

    }

}


// WZD o140p22
// drake178: G_UNIT_OvlHealing()
/*
Page 74  (PDF Page 79)
Unit Size and Healing
Healing rates are 5% of total hit points (of the undamaged figure) per turn when units are outside of cities, 10% when garrisoned in cities, and 15% when garrisoned in cities that have an animist’s guild. 
Finally, when units occupy map squares with Natural Healers (see Special Unit Abilities), they heal an additional 20% of their total hit points per game turn!
*/
/*

OON XREF  Repair_All_Units()
called in two different places
    one with flag = ST_FALSE
    one with flag = ST_TRUE

*/
void Heal_Unit(int16_t unit_idx, int16_t fraction, int16_t flag)
{

    int16_t hits_total;  // _DI_

    hits_total = _unit_type_table[_UNITS[unit_idx].type].Figures * Unit_Hit_Points(unit_idx);

    if(flag == ST_FALSE)
    {

        if(_UNITS[unit_idx].Damage < hits_total)
        {

            // ; BUG: damage over 127 can't be healed
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

        _UNITS[unit_idx].Damage = (_UNITS[unit_idx].Damage - (hits_total / 5));  // 20%

        // ; BUG: will always remove damage over 127
        SETMIN(_UNITS[unit_idx].Damage, 0);

        UNIT_HealArray[unit_idx] = 66;  // TODO  DEDU  IDGI

    }

}


// WZD o140p23
/*
Page 74  (PDF Page 79)
Unit Size and Healing
Healing rates are 5% of total hit points (of the undamaged figure) per turn when units are outside of cities, 10% when garrisoned in cities, and 15% when garrisoned in cities that have an animist’s guild. 
Finally, when units occupy map squares with Natural Healers (see Special Unit Abilities), they heal an additional 20% of their total hit points per game turn!
*/
/*

*/
void Heal_All_Units(void)
{
    int16_t troops[MAX_STACK];
    int16_t var_16[MAX_STACK];
    int16_t Human_Player_Units;
    int16_t troop_count;
    int16_t itr_troops;  // _SI_
    int16_t itr;  // _DI_
    int16_t itr_units;  // _DI_
    int16_t itr_cities;  // _DI_

    // DONT  UNIT_HealArray = SA_MK_FP0(Allocate_First_Block(_screen_seg, (4 + (_units_ / 16))););
    UNIT_HealArray = Allocate_First_Block(_screen_seg, (4 + (_units / 16)));  // calculate allocation in 16-byte paragraphs/memory segments
    
    // ; set every unit to 1/20th per turn natural healing
    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        UNIT_HealArray[itr_units] = 20;
    }

    // ; set the healing rate for units inside cities
    // ; BUG: was supposed to mark removed any units that are
    // ; on a tile that has a city with a non-matching owner,
    // ; but the design is botched, as the function that
    // ; returns the list of units does not actually include
    // ; these
    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        Army_At_City(itr_cities, &troop_count, &troops[0]);

        for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
        {

            // BUGBUG  ; conflicting condition: will always jump  ; (the function filling the array won't return enemies)
            if(_UNITS[troops[itr_troops]].owner_idx != _CITIES[itr_cities].owner_idx)
            {
Check_Game_Data();
                Kill_Unit(troops[itr_troops], kt_Disappeared);
Capture_Units_Data();
Check_Game_Data();
            }
        }

        for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
        {
            UNIT_HealArray[troops[itr_troops]] = 10;
        }

        if(_CITIES[itr_cities].bldg_status[ANIMISTS_GUILD] > bs_Replaced)  /* {-1:NotBuilt,0:Replaced,1:Built,2:Removed} */
        {
            for (itr_troops = 0; itr_troops < troop_count; itr_troops++)
            {
                UNIT_HealArray[troops[itr_troops]] = 6;
            }
        }

    }

    for(itr_units = 0; itr_units < _units; itr_units++)
    {

        /* HACK: */ if (_UNITS[itr_units].owner_idx == ST_UNDEFINED)
        {
            continue;
        }

        if(
            (g_timestop_player_num == 0)
            ||
            (_UNITS[itr_units].owner_idx == g_timestop_player_num)
        )
        {

            if(
                (_unit_type_table[_UNITS[itr_units].type].Race != rt_Death)
                &&
                ((_UNITS[itr_units].mutations & UM_UNDEAD) == 0)
            )
            {
Check_Game_Data();
                Heal_Unit(itr_units, UNIT_HealArray[itr_units], ST_FALSE);
Capture_Units_Data();
Check_Game_Data();
            }

            if((_unit_type_table[_UNITS[itr_units].type].Abilities & UA_HEALER) != 0)
            {

                Army_At_Square_1(_UNITS[itr_units].wx, _UNITS[itr_units].wy, _UNITS[itr_units].wp, &troop_count, &troops[0]);

                if(troop_count > MAX_STACK)
                {
                    Human_Player_Units = ST_FALSE;

                    // ; remove any neutral units, check if any of the rest belong to the human player
                    for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
                    {

                        if(_UNITS[itr_troops].owner_idx == NEUTRAL_PLAYER_IDX)
                        {
Check_Game_Data();
                            Kill_Unit(troops[itr_troops], kt_Disappeared);
Capture_Units_Data();
Check_Game_Data();

                        }

                        if(_UNITS[itr_troops].owner_idx == HUMAN_PLAYER_IDX)
                        {
                            Human_Player_Units = ST_TRUE;
                        }

                    }

                    if(Human_Player_Units == ST_TRUE)
                    {
                        // ; remove any units that don't belong to the human player
                        for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
                        {

                            if(_UNITS[itr_troops].owner_idx != HUMAN_PLAYER_IDX)
                            {
Check_Game_Data();
                                Kill_Unit(troops[itr_troops], kt_Disappeared);
Capture_Units_Data();
Check_Game_Data();
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
                        (_unit_type_table[_UNITS[itr_units].type].Race != rt_Death)
                        &&
                        ((_UNITS[itr_units].mutations & UM_UNDEAD) == 0)
                        &&
                        (UNIT_HealArray[troops[itr_troops]] != 66)  // TODO  DEDU  IDGI
                    )
                    {
Check_Game_Data();
                        Heal_Unit(itr_units, UNIT_HealArray[itr_units], ST_FALSE);
Capture_Units_Data();
Check_Game_Data();
                    }

                }

            }

        }

    }

}


// WZD o140p24
// drake178: WIZ_ProcessGlobals()
// MoO2  Module: DIPLOMAC  Diplomacy_Growth_()
/*
; processes the diplomatic reactions and persistent
; effects of global enchantments and the Spell of Mastery
; WARNING: Herb Mastery is not included here despite
; having a persistent effect
; also contains multiple inherited BUGs
*/
void Diplomacy_Growth_For_Enchantments__WIP(void)
{
    int16_t itr_players = 0;  // _SI_

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



// ovr140:29EA                                                 loc_C693A:                              ; CODE XREF: WIZ_ProcessGlobals+58j
// ovr140:29EA 8B C6                                           mov     ax, _SI_itr_players
// ovr140:29EC BA C8 04                                        mov     dx, size s_WIZARD
// ovr140:29EF F7 EA                                           imul    dx
// ovr140:29F1 8B D8                                           mov     bx, ax
// ovr140:29F3 80 BF 4D A3 00                                  cmp     [_players.Globals.Evil_Omens+bx], 0
// ovr140:29F8 7E 11                                           jle     short loc_C695B
// ovr140:29FA B8 0A 00                                        mov     ax, 10
// ovr140:29FD 50                                              push    ax                              ; Divisor
// ovr140:29FE B8 C6 00                                        mov     ax, spl_Evil_Omens
// ovr140:2A01 50                                              push    ax                              ; Spell_Index
// ovr140:2A02 56                                              push    _SI_itr_players                 ; Player_Index
// ovr140:2A03 9A 43 00 9D 35                                  call    j_Global_Enchantment_Change_Relations
// ovr140:2A08 83 C4 06                                        add     sp, 6
// ovr140:2A0B
// ovr140:2A0B                                                 loc_C695B:                              ; CODE XREF: WIZ_ProcessGlobals+79j
// ovr140:2A0B 8B C6                                           mov     ax, _SI_itr_players
// ovr140:2A0D BA C8 04                                        mov     dx, size s_WIZARD
// ovr140:2A10 F7 EA                                           imul    dx
// ovr140:2A12 8B D8                                           mov     bx, ax
// ovr140:2A14 80 BF 4E A3 00                                  cmp     [_players.Globals.Zombie_Mastery+bx], 0
// ovr140:2A19 7E 11                                           jle     short loc_C697C
// ovr140:2A1B B8 0A 00                                        mov     ax, 10
// ovr140:2A1E 50                                              push    ax                              ; Divisor
// ovr140:2A1F B8 BC 00                                        mov     ax, spl_Zombie_Mastery
// ovr140:2A22 50                                              push    ax                              ; Spell_Index
// ovr140:2A23 56                                              push    _SI_itr_players                 ; Player_Index
// ovr140:2A24 9A 43 00 9D 35                                  call    j_Global_Enchantment_Change_Relations
// ovr140:2A29 83 C4 06                                        add     sp, 6
// ovr140:2A2C
// ovr140:2A2C                                                 loc_C697C:                              ; CODE XREF: WIZ_ProcessGlobals+9Aj
// ovr140:2A2C 8B C6                                           mov     ax, _SI_itr_players
// ovr140:2A2E BA C8 04                                        mov     dx, size s_WIZARD
// ovr140:2A31 F7 EA                                           imul    dx
// ovr140:2A33 8B D8                                           mov     bx, ax
// ovr140:2A35 80 BF 4F A3 00                                  cmp     [_players.Globals.Aura_of_Majesty+bx], 0
// ovr140:2A3A 7E 11                                           jle     short loc_C699D
// ovr140:2A3C B8 0A 00                                        mov     ax, 10
// ovr140:2A3F 50                                              push    ax                              ; Divisor
// ovr140:2A40 B8 3B 00                                        mov     ax, spl_Aura_Of_Majesty
// ovr140:2A43 50                                              push    ax                              ; Spell_Index
// ovr140:2A44 56                                              push    _SI_itr_players                 ; Player_Index
// ovr140:2A45 9A 43 00 9D 35                                  call    j_Global_Enchantment_Change_Relations
// ovr140:2A4A 83 C4 06                                        add     sp, 6
// ovr140:2A4D
// ovr140:2A4D                                                 loc_C699D:                              ; CODE XREF: WIZ_ProcessGlobals+BBj
// ovr140:2A4D 8B C6                                           mov     ax, _SI_itr_players
// ovr140:2A4F BA C8 04                                        mov     dx, size s_WIZARD
// ovr140:2A52 F7 EA                                           imul    dx
// ovr140:2A54 8B D8                                           mov     bx, ax
// ovr140:2A56 80 BF 50 A3 00                                  cmp     [_players.Globals.Wind_Mastery+bx], 0
// ovr140:2A5B 7E 11                                           jle     short loc_C69BE
// ovr140:2A5D B8 0A 00                                        mov     ax, 10
// ovr140:2A60 50                                              push    ax                              ; Divisor
// ovr140:2A61 B8 39 00                                        mov     ax, spl_Wind_Mastery
// ovr140:2A64 50                                              push    ax                              ; Spell_Index
// ovr140:2A65 56                                              push    _SI_itr_players                 ; Player_Index
// ovr140:2A66 9A 43 00 9D 35                                  call    j_Global_Enchantment_Change_Relations
// ovr140:2A6B 83 C4 06                                        add     sp, 6
// ovr140:2A6E
// ovr140:2A6E                                                 loc_C69BE:                              ; CODE XREF: WIZ_ProcessGlobals+DCj
// ovr140:2A6E 8B C6                                           mov     ax, _SI_itr_players
// ovr140:2A70 BA C8 04                                        mov     dx, size s_WIZARD
// ovr140:2A73 F7 EA                                           imul    dx
// ovr140:2A75 8B D8                                           mov     bx, ax
// ovr140:2A77 80 BF 51 A3 00                                  cmp     [_players.Globals.Suppress_Magic+bx], 0
// ovr140:2A7C 7E 11                                           jle     short loc_C69DF
// ovr140:2A7E B8 0A 00                                        mov     ax, 10
// ovr140:2A81 50                                              push    ax                              ; Divisor
// ovr140:2A82 B8 4F 00                                        mov     ax, spl_Suppress_Magic
// ovr140:2A85 50                                              push    ax                              ; Spell_Index
// ovr140:2A86 56                                              push    _SI_itr_players                 ; Player_Index
// ovr140:2A87 9A 43 00 9D 35                                  call    j_Global_Enchantment_Change_Relations
// ovr140:2A8C 83 C4 06                                        add     sp, 6
// ovr140:2A8F
// ovr140:2A8F                                                 loc_C69DF:                              ; CODE XREF: WIZ_ProcessGlobals+FDj
// ovr140:2A8F 8B C6                                           mov     ax, _SI_itr_players
// ovr140:2A91 BA C8 04                                        mov     dx, size s_WIZARD
// ovr140:2A94 F7 EA                                           imul    dx
// ovr140:2A96 8B D8                                           mov     bx, ax
// ovr140:2A98 80 BF 52 A3 00                                  cmp     [_players.Globals.Time_Stop+bx], 0
// ovr140:2A9D 7E 11                                           jle     short loc_C6A00
// ovr140:2A9F B8 0A 00                                        mov     ax, 10
// ovr140:2AA2 50                                              push    ax                              ; Divisor
// ovr140:2AA3 B8 50 00                                        mov     ax, spl_Time_Stop
// ovr140:2AA6 50                                              push    ax                              ; Spell_Index
// ovr140:2AA7 56                                              push    _SI_itr_players                 ; Player_Index
// ovr140:2AA8 9A 43 00 9D 35                                  call    j_Global_Enchantment_Change_Relations
// ovr140:2AAD 83 C4 06                                        add     sp, 6
// ovr140:2AB0
// ovr140:2AB0                                                 loc_C6A00:                              ; CODE XREF: WIZ_ProcessGlobals+11Ej
// ovr140:2AB0 8B C6                                           mov     ax, _SI_itr_players
// ovr140:2AB2 BA C8 04                                        mov     dx, size s_WIZARD
// ovr140:2AB5 F7 EA                                           imul    dx
// ovr140:2AB7 8B D8                                           mov     bx, ax
// ovr140:2AB9 80 BF 53 A3 00                                  cmp     [_players.Globals.Nature_Awareness+bx], 0
// ovr140:2ABE 7E 11                                           jle     short loc_C6A21
// ovr140:2AC0 B8 0A 00                                        mov     ax, 10
// ovr140:2AC3 50                                              push    ax                              ; Divisor
// ovr140:2AC4 B8 22 00                                        mov     ax, spl_Natures_Awareness
// ovr140:2AC7 50                                              push    ax                              ; Spell_Index
// ovr140:2AC8 56                                              push    _SI_itr_players                 ; Player_Index
// ovr140:2AC9 9A 43 00 9D 35                                  call    j_Global_Enchantment_Change_Relations
// ovr140:2ACE 83 C4 06                                        add     sp, 6
// ovr140:2AD1
// ovr140:2AD1                                                 loc_C6A21:                              ; CODE XREF: WIZ_ProcessGlobals+13Fj
// ovr140:2AD1 8B C6                                           mov     ax, _SI_itr_players
// ovr140:2AD3 BA C8 04                                        mov     dx, size s_WIZARD
// ovr140:2AD6 F7 EA                                           imul    dx
// ovr140:2AD8 8B D8                                           mov     bx, ax
// ovr140:2ADA 80 BF 54 A3 00                                  cmp     [_players.Globals.Natures_Wrath+bx], 0
// ovr140:2ADF 7E 11                                           jle     short loc_C6A42
// ovr140:2AE1 B8 0A 00                                        mov     ax, 10
// ovr140:2AE4 50                                              push    ax                              ; Divisor
// ovr140:2AE5 B8 28 00                                        mov     ax, spl_Natures_Wrath
// ovr140:2AE8 50                                              push    ax                              ; Spell_Index
// ovr140:2AE9 56                                              push    _SI_itr_players                 ; Player_Index
// ovr140:2AEA 9A 43 00 9D 35                                  call    j_Global_Enchantment_Change_Relations
// ovr140:2AEF 83 C4 06                                        add     sp, 6
// ovr140:2AF2
// ovr140:2AF2                                                 loc_C6A42:                              ; CODE XREF: WIZ_ProcessGlobals+160j
// ovr140:2AF2 8B C6                                           mov     ax, _SI_itr_players
// ovr140:2AF4 BA C8 04                                        mov     dx, size s_WIZARD
// ovr140:2AF7 F7 EA                                           imul    dx
// ovr140:2AF9 8B D8                                           mov     bx, ax
// ovr140:2AFB 80 BF 55 A3 00                                  cmp     [_players.Globals.Herb_Mastery+bx], 0
// ovr140:2B00 7E 11                                           jle     short loc_C6A63
// ovr140:2B02 B8 0A 00                                        mov     ax, 10
// ovr140:2B05 50                                              push    ax                              ; Divisor
// ovr140:2B06 B8 26 00                                        mov     ax, spl_Herb_Mastery
// ovr140:2B09 50                                              push    ax                              ; Spell_Index
// ovr140:2B0A 56                                              push    _SI_itr_players                 ; Player_Index
// ovr140:2B0B 9A 43 00 9D 35                                  call    j_Global_Enchantment_Change_Relations
// ovr140:2B10 83 C4 06                                        add     sp, 6
// ovr140:2B13
// ovr140:2B13                                                 loc_C6A63:                              ; CODE XREF: WIZ_ProcessGlobals+181j
// ovr140:2B13 8B C6                                           mov     ax, _SI_itr_players
// ovr140:2B15 BA C8 04                                        mov     dx, size s_WIZARD
// ovr140:2B18 F7 EA                                           imul    dx
// ovr140:2B1A 8B D8                                           mov     bx, ax
// ovr140:2B1C 80 BF 56 A3 00                                  cmp     [_players.Globals.Chaos_Surge+bx], 0
// ovr140:2B21 7E 11                                           jle     short loc_C6A84
// ovr140:2B23 B8 0A 00                                        mov     ax, 10
// ovr140:2B26 50                                              push    ax                              ; Divisor
// ovr140:2B27 B8 74 00                                        mov     ax, spl_Chaos_Surge
// ovr140:2B2A 50                                              push    ax                              ; Spell_Index
// ovr140:2B2B 56                                              push    _SI_itr_players                 ; Player_Index
// ovr140:2B2C 9A 43 00 9D 35                                  call    j_Global_Enchantment_Change_Relations
// ovr140:2B31 83 C4 06                                        add     sp, 6
// ovr140:2B34
// ovr140:2B34                                                 loc_C6A84:                              ; CODE XREF: WIZ_ProcessGlobals+1A2j
// ovr140:2B34 8B C6                                           mov     ax, _SI_itr_players
// ovr140:2B36 BA C8 04                                        mov     dx, size s_WIZARD
// ovr140:2B39 F7 EA                                           imul    dx
// ovr140:2B3B 8B D8                                           mov     bx, ax
// ovr140:2B3D 80 BF 57 A3 00                                  cmp     [_players.Globals.Doom_Mastery+bx], 0
// ovr140:2B42 7E 11                                           jle     short loc_C6AA5
// ovr140:2B44 B8 0A 00                                        mov     ax, 10
// ovr140:2B47 50                                              push    ax                              ; Divisor
// ovr140:2B48 B8 75 00                                        mov     ax, spl_Doom_Mastery
// ovr140:2B4B 50                                              push    ax                              ; Spell_Index
// ovr140:2B4C 56                                              push    _SI_itr_players                 ; Player_Index
// ovr140:2B4D 9A 43 00 9D 35                                  call    j_Global_Enchantment_Change_Relations
// ovr140:2B52 83 C4 06                                        add     sp, 6
// ovr140:2B55
// ovr140:2B55                                                 loc_C6AA5:                              ; CODE XREF: WIZ_ProcessGlobals+1C3j
// ovr140:2B55 8B C6                                           mov     ax, _SI_itr_players
// ovr140:2B57 BA C8 04                                        mov     dx, size s_WIZARD
// ovr140:2B5A F7 EA                                           imul    dx
// ovr140:2B5C 8B D8                                           mov     bx, ax
// ovr140:2B5E 80 BF 58 A3 00                                  cmp     [_players.Globals.Great_Wasting+bx], 0
// ovr140:2B63 7E 18                                           jle     short loc_C6ACD
// ovr140:2B65 56                                              push    _SI_itr_players                 ; Player_Index
// ovr140:2B66 9A 52 00 B8 35                                  call    j_WIZ_GreatWasting              ; attempts to corrupt 4 to 6 tiles in the world,
// ovr140:2B66                                                                                         ; excluding oceans, already corrupted tiles, and tiles
// ovr140:2B66                                                                                         ; within a 2 radius of the caster's cities
// ovr140:2B66                                                                                         ;
// ovr140:2B66                                                                                         ; BUG: excludes corner tiles outside of catchments too
// ovr140:2B66                                                                                         ; BUG: can corrupt node tiles
// ovr140:2B66                                                                                         ; BUG: scuttles the near buffer for no reason
// ovr140:2B66                                                                                         ; BUG: ignores all protective measures (Consecration,
// ovr140:2B66                                                                                         ;  Spell Ward, Nightshade) even if the target is a city
// ovr140:2B6B 59                                              pop     cx
// ovr140:2B6C B8 0A 00                                        mov     ax, 10
// ovr140:2B6F 50                                              push    ax                              ; Divisor
// ovr140:2B70 B8 72 00                                        mov     ax, spl_Great_Wasting
// ovr140:2B73 50                                              push    ax                              ; Spell_Index
// ovr140:2B74 56                                              push    _SI_itr_players                 ; Player_Index
// ovr140:2B75 9A 43 00 9D 35                                  call    j_Global_Enchantment_Change_Relations
// ovr140:2B7A 83 C4 06                                        add     sp, 6
// ovr140:2B7D
// ovr140:2B7D                                                 loc_C6ACD:                              ; CODE XREF: WIZ_ProcessGlobals+1E4j
// ovr140:2B7D 8B C6                                           mov     ax, _SI_itr_players
// ovr140:2B7F BA C8 04                                        mov     dx, size s_WIZARD
// ovr140:2B82 F7 EA                                           imul    dx
// ovr140:2B84 8B D8                                           mov     bx, ax
// ovr140:2B86 80 BF 59 A3 00                                  cmp     [_players.Globals.Meteor_Storm+bx], 0
// ovr140:2B8B 7E 18                                           jle     short loc_C6AF5
// ovr140:2B8D 56                                              push    _SI_itr_players                 ; Player_Index
// ovr140:2B8E 9A 2F 00 B8 35                                  call    j_WIZ_MeteorStorm               ; processes the effect of Meteor Storm - hit all cities
// ovr140:2B8E                                                                                         ; for 1% building destruction, and all units outside of
// ovr140:2B8E                                                                                         ; cities for 4 points of area fire damage
// ovr140:2B8E                                                                                         ;
// ovr140:2B8E                                                                                         ; WARNING: stores the spell attack details in the
// ovr140:2B8E                                                                                         ;  Call Chaos spell record
// ovr140:2B93 59                                              pop     cx
// ovr140:2B94 B8 0A 00                                        mov     ax, 10
// ovr140:2B97 50                                              push    ax                              ; Divisor
// ovr140:2B98 B8 71 00                                        mov     ax, spl_Meteor_Storm
// ovr140:2B9B 50                                              push    ax                              ; Spell_Index
// ovr140:2B9C 56                                              push    _SI_itr_players                 ; Player_Index
// ovr140:2B9D 9A 43 00 9D 35                                  call    j_Global_Enchantment_Change_Relations
// ovr140:2BA2 83 C4 06                                        add     sp, 6
// ovr140:2BA5
// ovr140:2BA5                                                 loc_C6AF5:                              ; CODE XREF: WIZ_ProcessGlobals+20Cj
// ovr140:2BA5 8B C6                                           mov     ax, _SI_itr_players
// ovr140:2BA7 BA C8 04                                        mov     dx, size s_WIZARD
// ovr140:2BAA F7 EA                                           imul    dx
// ovr140:2BAC 8B D8                                           mov     bx, ax
// ovr140:2BAE 80 BF 5A A3 00                                  cmp     [_players.Globals.Armageddon+bx], 0
// ovr140:2BB3 7E 18                                           jle     short loc_C6B1D
// ovr140:2BB5 56                                              push    _SI_itr_players                 ; Player_Index
// ovr140:2BB6 9A 5C 00 B8 35                                  call    j_WIZ_Armageddon                ; attempts to turn 4 to 6 tiles in the world, excluding
// ovr140:2BB6                                                                                         ; oceans, rivers, volcanoes, nodes, and tiles within a
// ovr140:2BB6                                                                                         ; 2 radius of the caster's cities
// ovr140:2BB6                                                                                         ;
// ovr140:2BB6                                                                                         ; BUG: excludes corner tiles outside of catchments too
// ovr140:2BB6                                                                                         ; BUG: ignores all protective measures (Consecration,
// ovr140:2BB6                                                                                         ;  Spell Ward, Nightshade) even if the target is a city
// ovr140:2BBB 59                                              pop     cx
// ovr140:2BBC B8 0A 00                                        mov     ax, 10
// ovr140:2BBF 50                                              push    ax                              ; Divisor
// ovr140:2BC0 B8 78 00                                        mov     ax, spl_Armageddon
// ovr140:2BC3 50                                              push    ax                              ; Spell_Index
// ovr140:2BC4 56                                              push    _SI_itr_players                 ; Player_Index
// ovr140:2BC5 9A 43 00 9D 35                                  call    j_Global_Enchantment_Change_Relations
// ovr140:2BCA 83 C4 06                                        add     sp, 6
// ovr140:2BCD
// ovr140:2BCD                                                 loc_C6B1D:                              ; CODE XREF: WIZ_ProcessGlobals+234j
// ovr140:2BCD 8B C6                                           mov     ax, _SI_itr_players
// ovr140:2BCF BA C8 04                                        mov     dx, size s_WIZARD
// ovr140:2BD2 F7 EA                                           imul    dx
// ovr140:2BD4 8B D8                                           mov     bx, ax
// ovr140:2BD6 80 BF 5B A3 00                                  cmp     [_players.Globals.Tranquility+bx], 0
// ovr140:2BDB 7E 11                                           jle     short loc_C6B3E
// ovr140:2BDD B8 0A 00                                        mov     ax, 10
// ovr140:2BE0 50                                              push    ax                              ; Divisor
// ovr140:2BE1 B8 9D 00                                        mov     ax, spl_Tranquility
// ovr140:2BE4 50                                              push    ax                              ; Spell_Index
// ovr140:2BE5 56                                              push    _SI_itr_players                 ; Player_Index
// ovr140:2BE6 9A 43 00 9D 35                                  call    j_Global_Enchantment_Change_Relations
// ovr140:2BEB 83 C4 06                                        add     sp, 6
// ovr140:2BEE
// ovr140:2BEE                                                 loc_C6B3E:                              ; CODE XREF: WIZ_ProcessGlobals+25Cj
// ovr140:2BEE 8B C6                                           mov     ax, _SI_itr_players
// ovr140:2BF0 BA C8 04                                        mov     dx, size s_WIZARD
// ovr140:2BF3 F7 EA                                           imul    dx
// ovr140:2BF5 8B D8                                           mov     bx, ax
// ovr140:2BF7 80 BF 5C A3 00                                  cmp     [_players.Globals.Life_Force+bx], 0
// ovr140:2BFC 7E 11                                           jle     short loc_C6B5F
// ovr140:2BFE B8 0A 00                                        mov     ax, 10
// ovr140:2C01 50                                              push    ax                              ; Divisor
// ovr140:2C02 B8 9C 00                                        mov     ax, spl_Life_Force
// ovr140:2C05 50                                              push    ax                              ; Spell_Index
// ovr140:2C06 56                                              push    _SI_itr_players                 ; Player_Index
// ovr140:2C07 9A 43 00 9D 35                                  call    j_Global_Enchantment_Change_Relations
// ovr140:2C0C 83 C4 06                                        add     sp, 6
// ovr140:2C0F
// ovr140:2C0F                                                 loc_C6B5F:                              ; CODE XREF: WIZ_ProcessGlobals+27Dj
// ovr140:2C0F 8B C6                                           mov     ax, _SI_itr_players
// ovr140:2C11 BA C8 04                                        mov     dx, size s_WIZARD
// ovr140:2C14 F7 EA                                           imul    dx
// ovr140:2C16 8B D8                                           mov     bx, ax
// ovr140:2C18 80 BF 5D A3 00                                  cmp     [_players.Globals.Crusade+bx], 0
// ovr140:2C1D 7E 11                                           jle     short loc_C6B80
// ovr140:2C1F B8 0A 00                                        mov     ax, 10
// ovr140:2C22 50                                              push    ax                              ; Divisor
// ovr140:2C23 B8 9E 00                                        mov     ax, spl_Crusade
// ovr140:2C26 50                                              push    ax                              ; Spell_Index
// ovr140:2C27 56                                              push    _SI_itr_players                 ; Player_Index
// ovr140:2C28 9A 43 00 9D 35                                  call    j_Global_Enchantment_Change_Relations
// ovr140:2C2D 83 C4 06                                        add     sp, 6



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
/*
    heals units
    clears stasis
    ...
    _UNITS[itr_units].XP += 1;
    hero_level = Calc_Unit_Level(itr_units);
    Hero_LevelUp_Popup(itr_units);
    _UNITS[itr_units].Level = Calc_Unit_Level(itr_units);    
    ...
    _UNITS[troop_list[itr_troops]].XP += Highest_Armsmaster_XP;
    _UNITS[troop_list[itr_troops]].Level = Calc_Unit_Level(troop_list[itr_troops]);

*/
void Do_All_Units_XP_Check_(void)
{
    /* TODO  init to zeroes (But, HOW!?!?!) */  static struct s_BATTLE_UNIT battle_unit;  // // sizeof: 6Eh  110d
    int16_t Processed_Hero_List[NUM_HEROES] = { 0, 0, 0, 0, 0, 0 };
    int16_t troop_list[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t hero_level = 0;
    int16_t hero_unit_idx = 0;
    int16_t troop_count = 0;
    int16_t Highest_Armsmaster_XP = 0;
    int16_t XP_Gain = 0;
    int16_t itr_heroes = 0;
    int16_t itr_units = 0;  // _SI_
    int16_t itr_players = 0;  // _SI_
    int16_t itr_troops = 0;  // _DI_

    memset(&battle_unit, 0, sizeof(struct s_BATTLE_UNIT));

    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        // Herb Mastery:  Nature. Global Enchantment;  Casting Cost: 1000 mana;  Upkeep: 10 mana/turn. Very Rare.
        // Completely heals all of a wizard’s damaged units every game turn.
        if(
            (_players[_UNITS[itr_units].owner_idx].Globals[HERB_MASTERY] > 0)
            &&
            (_unit_type_table[_UNITS[itr_units].type].Race != rt_Death)
            &&
            ((_UNITS[itr_units].mutations & UM_UNDEAD) == 0)
        )
        {

Check_Game_Data();
            _UNITS[itr_units].Damage = 0;
Capture_Units_Data();
Check_Game_Data();

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

Check_Game_Data();
            _UNITS[itr_units].mutations |= C_STASISLINGER;

            _UNITS[itr_units].mutations ^= C_STASISINIT;
Capture_Units_Data();
Check_Game_Data();

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
                (_UNITS[itr_units].owner_idx == g_timestop_player_num)
            )
        )
        {

Check_Game_Data();
            _UNITS[itr_units].XP += 1;
Capture_Units_Data();
Check_Game_Data();

            hero_level = Calc_Unit_Level(itr_units);

            if(_UNITS[itr_units].Hero_Slot > -1)
            {

                if(_UNITS[itr_units].owner_idx == HUMAN_PLAYER_IDX)
                {

                    if(_UNITS[itr_units].Level < hero_level)
                    {

                        Hero_LevelUp_Popup(itr_units);

                    }

                }

            }

Check_Game_Data();
            _UNITS[itr_units].Level = Calc_Unit_Level(itr_units);
Capture_Units_Data();
Check_Game_Data();

        }
        /*
            END:  Experience Points & Experience Level
        */


    }

    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {

        for(itr_heroes = 0; itr_heroes < NUM_HEROES; itr_heroes++)
        {

            Processed_Hero_List[itr_heroes] = ST_FALSE;

        }

        if(
            (g_timestop_player_num == 0)
            ||
            (g_timestop_player_num == itr_players)
        )
        {

            for(itr_heroes = 0; itr_heroes < NUM_HEROES; itr_heroes++)
            {

                if(
                    (_players[itr_players].Heroes[itr_heroes].unit_idx > -1)
                    &&
                    (Processed_Hero_List[itr_heroes] == ST_FALSE)
                )
                {

                    hero_unit_idx = _players[itr_players].Heroes[itr_heroes].unit_idx;

                    Highest_Armsmaster_XP = 0;

                    Army_At_Square_1(_UNITS[hero_unit_idx].wx, _UNITS[hero_unit_idx].wy, _UNITS[hero_unit_idx].wp, &troop_count, &troop_list[0]);

                    Processed_Hero_List[itr_heroes] = ST_TRUE;

                    for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
                    {

                        XP_Gain = 0;

                        /*
                            ¿ BUGBUG ?
                            Run-Time Check Failure #2 - Stack around the variable 'Processed_Hero_List' was corrupted.
                        */
                        // Processed_Hero_List[_UNITS[troop_list[itr_troops]].Hero_Slot] = ST_TRUE;  // ; BUG: this can also be -1!
                        if(_UNITS[troop_list[itr_troops]].Hero_Slot > ST_UNDEFINED)
                        {

                            Processed_Hero_List[_UNITS[troop_list[itr_troops]].Hero_Slot] = ST_TRUE;

                        }

                        if(_UNITS[troop_list[itr_troops]].Hero_Slot > -1)
                        {

                            if((_HEROES2[HUMAN_PLAYER_IDX]->heroes[_UNITS[troop_list[itr_troops]].type].abilities & HSA_ARMSMASTER) != 0)
                            {

                                XP_Gain = ((_UNITS[troop_list[itr_troops]].Level + 1) * 2);

                            }

                            else if((_HEROES2[HUMAN_PLAYER_IDX]->heroes[_UNITS[troop_list[itr_troops]].type].abilities & HSA_ARMSMASTER2) != 0)
                            {

                                XP_Gain = ((_UNITS[troop_list[itr_troops]].Level + 1) * 3);

                            }

                            if(XP_Gain > Highest_Armsmaster_XP)
                            {

                                Highest_Armsmaster_XP = XP_Gain;

                            }

                        }

                    }

                    if(Highest_Armsmaster_XP > 0)
                    {

                        for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
                        {

                            if(_UNITS[troop_list[itr_troops]].Hero_Slot == -1)
                            {

Check_Game_Data();
                                _UNITS[troop_list[itr_troops]].XP += Highest_Armsmaster_XP;

                                _UNITS[troop_list[itr_troops]].Level = Calc_Unit_Level(troop_list[itr_troops]);
Capture_Units_Data();
Check_Game_Data();

                            }

                        }
                        
                    }

                }

            }

        }
        
    }

}


// WZD o140p26
void Cool_Off_Volcanoes(void)
{
    int16_t terrain_type;
    int16_t itr_wp;
    int16_t itr_wy;  // _DI_
    int16_t itr_wx;  // _SI_

    for(itr_wp = 0; itr_wp < NUM_PLANES; itr_wp++)
    {
        for(itr_wx = 0; itr_wx < WORLD_WIDTH; itr_wx++)
        {
            for(itr_wy = 0; itr_wy < WORLD_HEIGHT; itr_wy++)
            {
                terrain_type = (GET_TERRAIN_TYPE(itr_wx, itr_wy, itr_wp) % _TerType_Count);

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

Check_Game_Data();
                        Kill_Unit(troops[itr_troops], kt_Normal);
Capture_Units_Data();
Check_Game_Data();

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

Check_Game_Data();
                Kill_Unit(itr_units, kt_Normal);
Capture_Units_Data();
Check_Game_Data();

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
    struct s_mouse_list mouse_list[1];

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
