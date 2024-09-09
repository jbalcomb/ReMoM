/*
    Next Turn Procedure

    WIZARDS.EXE
        ovr060
        ovr119
        ovr121
        ovr140
        ovr141
*/

#include "MoM.H"
#include "MainScr.H"
#include "MainScr_Maps.H"
#include "NEXTTURN.H"
#include "WZD_o059.H"
#include "CityScr.H"  /* City_Screen__WIP(); */



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
*/
void Delete_Dead_Units(void)
{
    int16_t unit_type;
    int16_t itr_heroes;
    int16_t itr_units; // _SI_
    int16_t itr_players; // _DI_

    for(itr_units = 0; itr_units < _units; itr_units++)
    {

        unit_type = _UNITS[itr_units].type;

        if(
            (_UNITS[itr_units].owner_idx < HUMAN_PLAYER_IDX)
            ||
            (_UNITS[itr_units].owner_idx > NEUTRAL_PLAYER_IDX)
        )
        {
            Delete_Structure(itr_units, (uint8_t *)&_UNITS[0], sizeof(struct s_UNIT), _units);

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

            _units -= 1;
        }
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
int16_t IDK_City_Radius_s34255__STUB(int16_t player_idx, int16_t wx, int16_t wy, int16_t wp)
{

}


// WZD o60p03
void empty_function__o060p03(void)
{
/*
push    bp
mov     bp, sp
pop     bp
retf
*/
}


// WZD o60p04
void Next_Turn_Proc(void)
{
    char temp_string[LEN_TEMP_STRING];
    int16_t orig_map_plane;
    int16_t itr_msg;  // _SI_
    int16_t curr_prod_idx;  // _DI_

    Delete_Dead_Units();

    All_Units_In_Towers();

    Set_Unit_Draw_Priority();
    Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);
    Reset_Map_Draw();


    Next_Turn_Calc();


    GFX_Swap_Cities();

    Delete_Dead_Units();

    All_Units_In_Towers();


    if(magic_set.EoT_Summary == ST_TRUE)
    {
        // TODO  Chancellor_Screen__STUB();
    }



    /*
        BEGIN: Messages
    */
    if(g_bldg_msg_ctr > 0)
    {
        // DONT  j_o62p01_Empty_pFxn(_human_player_idx);  // drake178: does nothing and returns zero; at some point must have been some wizard data refresh function
        for(itr_msg = 0; itr_msg < g_bldg_msg_ctr; itr_msg++)
        {
            if(MSG_Building_Complete[itr_msg].city_idx != -1)
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
                            strcat(GUI_NearMsgString, STR_GetIndefinite(&temp_string[0]));
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



// @@Done_WrapItUp
    current_screen = scr_Main_Screen;

    g_bldg_msg_ctr = 0;

    Update_Units_MvsSts();

    // DONT  o62p01_Empty_pFxn(_human_player_idx);

    // DONT  fxnptr_o59p();

    if(
        (_players[_human_player_idx].Cast_Cost_Left <= 0)
        &&
        (_players[_human_player_idx].Spell_Cast > 0))
    {
        // TODO  G_OVL_Cast__STUB(_human_player_idx);
    }

// @@Done_Done
    all_units_moved = ST_FALSE;
    
    WIZ_NextIdleStack(_human_player_idx, &_map_x, &_map_y, &_map_plane);

    Reset_Draw_Active_Stack();

}


// WZD o60p05
// AI_ResetUnitMoves()


// WZD o60p06
// ¿ AI version ?
/*
updates Finished, Status, moves2_max, moves2
*/
void Update_Units_MvsSts(void)
{
    int16_t itr_units;

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

    TILE_VisibilityUpdt();

}


// WZD o60p07
void TILE_VisibilityUpdt(void)
{
    int16_t itr_players;
    int16_t itr_units;
    int16_t tmp_unit_enchantments_hiword;

    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        // TODO(JimBalcomb,2023075): figure out the indexing in the Dasm - doesn't look like array of struct  also, this'll set the neutral player?
        _players[itr_players + 1].Dipl.Contacted[HUMAN_PLAYER_IDX] = 1;

        // if(_players[itr_players + 1].Globals.Nature_Awareness != ST_FALSE)
        if(_players[itr_players + 1].Globals[NATURE_AWARENESS] != ST_FALSE)
        {
            for(itr_units = 0; itr_units < _units; itr_units++)
            {
                if(_UNITS[itr_units].owner_idx != ST_UNDEFINED)
                {
                    if(_UNITS[itr_units].owner_idx != itr_players)
                    {
                        if(_UNITS[itr_units].owner_idx != NEUTRAL_PLAYER_IDX)
                        {
                            // BUG: only checks enchantment, not ability or item
                            if((_UNITS[itr_units].enchantments & UE_INVISIBILITY) != 0)
                            {
                                _players[itr_players].Dipl.Contacted[_UNITS[itr_units].owner_idx] = 1;
                                _players[_UNITS[itr_units].owner_idx].Dipl.Contacted[itr_players] = 1;
                            }
                        }
                    }
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
void Next_Turn_Calc(void)
{

    // TOOD  Set_Random_Seed(RNG_AI_Turn_Seed);

    Set_Mouse_List(1, mouse_list_hourglass);


    All_City_Calculations();


// call    j_AI_CullTheWeak                ; after turn 99, disbands any non-garrison AI unit that
//                                         ; costs less than its wizard's average, and every 25
//                                         ; turns, recalculates the average and disbands up to
//                                         ; one such unit from each garrison too

// call    j_AI_Overland_Turn              ; process all aspects of the AI turns except diplomacy,
//                                         ; including the neutral player
//                                         ;
//                                         ; contains a hoard of BUGs
//                                         ; plans first for all players before processing for all
//                                         ; players


    Next_Turn_Process_Purify();


    MSG_Clear();


    if(g_TimeStop_PlayerNum != 0)
    {

// mov     ax, [g_TimeStop_PlayerNum]
// mov     dx, 4C8h
// imul    dx
// mov     bx, ax
// sub     _players.mana_reserve[bx-4c8h], 200
// mov     ax, [g_TimeStop_PlayerNum]      ; index of the Time Stop wizard plus 1 (0 if none)
// mov     dx, 4C8h
// imul    dx
// mov     dx, (offset _players)-4c8h
// add     dx, banner_id.mana_reserve
// add     ax, dx
// mov     bx, ax
// cmp     [word ptr bx], 0
// jge     short loc_9EDF3

// loc_9EDC2:                              ; index of the Time Stop wizard plus 1 (0 if none)
// mov     ax, [g_TimeStop_PlayerNum]
// mov     dx, 4C8h
// imul    dx
// mov     bx, ax
// mov     _players.mana_reserve[bx-4c8h], 0
// 
// 
// mov     ax, [g_TimeStop_PlayerNum]      ; index of the Time Stop wizard plus 1 (0 if none)
// mov     dx, 4C8h
// imul    dx
// mov     bx, ax
// mov     _players.Globals.Time_Stop[bx-4c8h], 0
// 
// 
// cmp     [g_TimeStop_PlayerNum], 1       ; index of the Time Stop wizard plus 1 (0 if none)
// jnz     short loc_9EDED

// mov     [MSG_GEs_Lost], 1

// loc_9EDED:                              ; index of the Time Stop wizard plus 1 (0 if none)
// mov     [g_TimeStop_PlayerNum], 0

    }
    else
    {

        // drake178: decreases the peace counters for all wizards towards all others
        // call    j_WIZ_DecreasePeaceCs

        Update_Players_Gold_Reserve();

        Players_Update_Magic_Power();

        Players_Apply_Magic_Power();

    // call    j_WIZ_ResearchProgress          ; checks whether any of the wizards has finished their
    //                                         ; current research, and if so, allows picking a new one
    //                                         ; if possible, showing the new spell animation for the
    //                                         ; human player followed by the research dialog
    //                                         ; WARNING: can temporarily store a negative spell index
    //                                         ; as the player's researched spell

        // TODO  OVL_DisableIncmBlink();



        if(DBG_Alt_A_State == ST_FALSE)
        {
            if(magic_set.Random_Events == ST_TRUE)
            {
                Determine_Event();
            }
        }


        Event_Twiddle();


        Players_Apply_Upkeeps__WIP();


        // DONT  EMM_Map_DataH()


        All_Outpost_Population_Growth();


        Apply_City_Changes();


        // ; processes the diplomatic reactions and persistent
        // ; effects of global enchantments and the Spell of Mastery
        // ; WARNING: Herb Mastery is not included here despite
        // ; having a persistent effect
        // ; also contains multiple inherited BUGs
        // TODO  WIZ_ProcessGlobals()            


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


        Determine_Offer();


        Set_Mouse_List(1, mouse_list_hourglass);


        All_City_Nightshade_Count();


    // call    j_DIPL_Gravitation              ; process wizard pact and alliance relation
    //                                         ; gravitation, military and city overextension, and
    //                                         ; gravitation towards default relation
    //                                         ; RE-EXPLORE!

    // call    j_DIPL_ContactProgress          ; progress the human player's contact with other
    //                                         ; wizards - if progress is 0, but contacted is 1,
    //                                         ; set progress to 1 and relations to no treaty


        Set_Mouse_List(1, mouse_list_hourglass);


    // call    j_DIPL_AI_To_AI                 ; many BUGs and INCONSISTENCIES inside ; RE-EXPLORE in more context!


        Set_Mouse_List(1, mouse_list_hourglass);


    // call    j_IDK_Dipl_s7373B

    // call    j_IDK_Dipl_s73FBF


        Set_Mouse_List(1, mouse_list_hourglass);

        // TODO  End_Of_Turn_Diplomacy_Adjustments_

        // TODO  Modifier_Diplomacy_Adjustments()

        Cool_Off_Volcanoes();

    }


// call    j_IDK_SplCst_SplSkl_sC5AB1

    /* DEMOHACK */  // Delete_Dead_Units();


    Set_Unit_Draw_Priority();
    Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);


    All_City_Removed_Buildings();


    Do_All_Units_XP_Check_();


    Heal_All_Units();


    Record_History();


// call    j_IDK_SND_BkGrnd_s518AE


    _turn++;


    // TODO  cap gold, mana 30000


    // TODO  OVL_EnableIncmBlink();
    

    Do_Autosave();

// ; if the current turn is divisible by 4, saves the game
//                                         ; into slot index 8 (SAVE9.GAM), the "continue" save
//                                         ; that can not be loaded from the save/load screen, but
//                                         ; is started automatically by wizards.exe


    All_City_Calculations();


    // TODO  Get_Random_Seed(RNG_AI_Turn_Seed);


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
    int16_t minimum_farmer_count;
    int16_t itr_cities;  // _SI_

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        Do_City_Calculations(itr_cities);
        minimum_farmer_count = City_Minimum_Farmers(itr_cities);
        if(_CITIES[itr_cities].farmer_count < minimum_farmer_count)
        {
            _CITIES[itr_cities].farmer_count = minimum_farmer_count;
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
    Lair_Make_Guardians()
        Create_Unit__WIP(_LAIRS[lair_idx].guard2_unit_type, NEUTRAL_PLAYER_IDX, _LAIRS[lair_idx].wx, _LAIRS[lair_idx].wy, _LAIRS[lair_idx].wp, 2000)

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
        ¿ just some number greater than what could be a city_idx ?
        overrides unit count limit
        ¿ only way to create units above 980 ?

*/
int16_t Create_Unit__WIP(int16_t unit_type, int16_t owner_idx, int16_t wx, int16_t wy, int16_t wp, int16_t R_Param)
{
    
    int16_t did_create_unit;  // DNE in Dasm
    int16_t itr;  // _DI_

    did_create_unit = ST_FALSE;

    if(
        (owner_idx == HUMAN_PLAYER_IDX) ||
        (_units <= 950) ||
        (R_Param == 2000)
    )
    {
        if(
            (R_Param == 2000) ||
            (_units <= 980)
        )
        {
            if(_units != 1000)
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
                _UNITS[_units].Move_Failed = 0;
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
int16_t Player_Base_Casting_Skill(int16_t player_idx)
{

    int16_t casting_skill;  // _CX_

    casting_skill = sqrt(_players[player_idx].Casting_Skill);

    casting_skill += 1;  // STUBUG(JimBalcomb,20240113): getting 71 instead of 72; truncated? round up? imperfect square? isqrt()? CORDIC Algorithm - Successive Approximation?

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

    casting_cost = spell_data_table[spell_idx].Casting_Cost;

    if(
        (spell_data_table[spell_idx].Realm == sbr_Nature) ||
        (spell_data_table[spell_idx].Realm == sbr_Life)
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

    if(spell_data_table[spell_idx].Realm == 5)  /* _Arcane */
    {
        Spellbooks = 0;
    }
    else
    {
        Spellbooks = _players[player_idx].spellranks[spell_data_table[spell_idx].Realm];
    }

    if(Spellbooks > 7)
    {
        Over7_Books = (Spellbooks - 7);
        casting_cost_reduction = (Over7_Books * 10);
    }

    if(
        (spell_data_table[spell_idx].Realm == 0) &&  /* _Nature */
        (_players[player_idx].nature_mastery > 0)
    )
    {
        casting_cost_reduction += 15;
    }

    if(
        (spell_data_table[spell_idx].Realm == 1) &&  /* _Sorcery */
        (_players[player_idx].sorcery_mastery > 0)
    )
    {
        casting_cost_reduction += 15;
    }

    if(
        (spell_data_table[spell_idx].Realm == 2) &&  /* _Chaos */
        (_players[player_idx].chaos_mastery > 0)
    )
    {
        casting_cost_reduction += 15;
    }

    if(
        (_players[player_idx].artificer > 0) &&
        (spell_data_table[spell_idx].type == 11)  /* Crafting_Spell */
    )
    {
        casting_cost_reduction += 50;
    }

    if(
        (_players[player_idx].conjurer > 0) &&
        (spell_data_table[spell_idx].type == 0)  /* Summoning_Spell */
    )
    {
        casting_cost_reduction += 25;
    }

    if(
        (spell_data_table[spell_idx].Realm == 5) &&  /* _Arcane */
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
; returns the amount of dead, but not disintegrated
; heroes formerly in the player's service
*/
int16_t WIZ_DeadHeroCount(int16_t player_idx)
{
    int16_t itr_hero_types;  // _SI_
    int16_t hero_count;  // _DI_

    for(itr_hero_types = 0; itr_hero_types < 34; itr_hero_types++)
    {
        if(
            (_HEROES2[player_idx]->heroes[itr_hero_types].Level < 0)
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
        player_fame = 200;
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

            if(_HEROES2[player_idx]->heroes[hero_type_idx].Level >= 0)
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
                    (((_unit_type_table[hero_type_idx].Cost - 100) / 10) < player_fame)
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
    int16_t movement_points;  // _DI_
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

        for (itr_hero_items = 0; itr_hero_items < NUM_HERO_ITEMS; itr_hero_items++)
        {
            if (hero_items[itr_hero_items] > -1)
            {
                if (ITEM_POWER(hero_items[itr_hero_items], ip_Endurance))
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

    movement_points = _unit_type_table[_UNITS[unit_idx].type].Move_Halves;

    enchantments |= _UNITS[unit_idx].enchantments;

    if(movement_points < 6)
    {
        if((enchantments & UE_FLIGHT) != 0)
        {
            movement_points = 6;
        }
    }

    if((_UNITS[unit_idx].mutations & CC_FLIGHT) != 0)
    {
        movement_points = 4;
    }

    if((enchantments & UE_ENDURANCE) != 0)
    {
        endurance = ST_TRUE;
    }

    if(endurance == ST_TRUE)
    {
        movement_points += 2;
    }

    movement_points += item_moves2;

    /*
        BEGIN: Wind Mastery
    */
    if(_unit_type_table[_UNITS[unit_idx].type].Transport > ST_FALSE)
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
            movement_points = ((movement_points * 3) / 2);
        }
        if(wind_mastery < 0)
        {
            movement_points = (movement_points / 2);
        }
    }
    /*
        END: Wind Mastery
    */

    return movement_points;
}


// WZD o121p09
// drake178: UNIT_IsNormalUnit()
// UNIT_IsNormalUnit()

// WZD o121p10
// ¿ MoO2  ..._ALL_TECH_CHEAT..._MORE_MONEY_CHEAT... ?
void Cheat_Power(void)
{
    int16_t itr3;
    int16_t itr;  // _SI_
    int16_t itr2;  // _DI_

    for(itr = 0; itr < 8; itr++)
    {
        _players[HUMAN_PLAYER_IDX].research_spells[itr] = 0;
    }

    for(itr = 0; itr < _num_players; itr++)
    {
        _players[itr].mana_reserve = 10000;
        _players[itr].gold_reserve = 10000;
        _players[itr].Casting_Skill = 10000;
        _players[itr].Nominal_Skill = Player_Base_Casting_Skill(itr);
        _players[itr].Skill_Left = _players[itr].Nominal_Skill;
        for(itr2 = 0; itr2 < 5; itr2++)
        {
            for(itr3 = 0; itr3 < 40; itr3++)
            {
                if(_players[itr].Spells_Known[((itr2 * 40) + itr3)] == 1)
                {
                    _players[itr].Spells_Known[((itr2 * 40) + itr3)] = 2;
                }
            }
        }
    }

    _players[HUMAN_PLAYER_IDX].Globals[DETECT_MAGIC] = ST_TRUE;

    _players[HUMAN_PLAYER_IDX].research_spell_idx = 0;

}


// WZD o121p11
// drake178: AI_GetThreat_UType()
// AI_GetThreat_UType()

// WZD o121p12
// drake178: AI_GetThreat_UNIT()
// AI_GetThreat_UNIT()


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
                _unit_type_table[itr_unit_types].Upkeep = ((_unit_type_table[itr_unit_types].Cost + 49) / 50);  // Dasm is doing a ceil(), somehow?
            }
        }
    }

/*
[byte ptr _unit_type_table.Sound], 102
[byte ptr _unit_type_table.Sound+24h], 102
[byte ptr _unit_type_table.Sound+48h], 103
[byte ptr _unit_type_table.Sound+6Ch], 102
[byte ptr _unit_type_table.Sound+90h], 102
[byte ptr _unit_type_table.Sound+0B4h], 102
[byte ptr _unit_type_table.Sound+0D8h], 102
[byte ptr _unit_type_table.Sound+0FCh], 102
[byte ptr _unit_type_table.Sound+120h], 102
[byte ptr _unit_type_table.Sound+144h], 102
[byte ptr _unit_type_table.Sound+168h], 103
[byte ptr _unit_type_table.Sound+18Ch], 102
[byte ptr _unit_type_table.Sound+1B0h], 102
[byte ptr _unit_type_table.Sound+1D4h], 103
[byte ptr _unit_type_table.Sound+1F8h], 102
[byte ptr _unit_type_table.Sound+21Ch], 103
[byte ptr _unit_type_table.Sound+240h], 102
[byte ptr _unit_type_table.Sound+264h], 134
[byte ptr _unit_type_table.Sound+288h], 103
[byte ptr _unit_type_table.Sound+2ACh], 102
[byte ptr _unit_type_table.Sound+2D0h], 102
[byte ptr _unit_type_table.Sound+2F4h], 102
[byte ptr _unit_type_table.Sound+318h], 102
[byte ptr _unit_type_table.Sound+33Ch], 103
[byte ptr _unit_type_table.Sound+360h], 102
[byte ptr _unit_type_table.Sound+384h], 103
[byte ptr _unit_type_table.Sound+3A8h], 102
[byte ptr _unit_type_table.Sound+3CCh], 103
[byte ptr _unit_type_table.Sound+3F0h], 102
[byte ptr _unit_type_table.Sound+414h], 102
[byte ptr _unit_type_table.Sound+438h], 102
[byte ptr _unit_type_table.Sound+45Ch], 102
[byte ptr _unit_type_table.Sound+480h], 103
[byte ptr _unit_type_table.Sound+4A4h], 102
[byte ptr _unit_type_table.Sound+4C8h], 102
[byte ptr _unit_type_table.Sound+4ECh], 134
[byte ptr _unit_type_table.Sound+510h], 134
[byte ptr _unit_type_table.Sound+534h], 134
[byte ptr _unit_type_table.Sound+558h], 134
[byte ptr _unit_type_table.Sound+57Ch], 86
[byte ptr _unit_type_table.Sound+5A0h], 86
[byte ptr _unit_type_table.Sound+5C4h], 86
[byte ptr _unit_type_table.Sound+5E8h], 102
[byte ptr _unit_type_table.Sound+60Ch], 151
[byte ptr _unit_type_table.Sound+630h], 150
[byte ptr _unit_type_table.Sound+654h], 86
[byte ptr _unit_type_table.Sound+678h], 86
[byte ptr _unit_type_table.Sound+69Ch], 86
[byte ptr _unit_type_table.Sound+6C0h], 86
[byte ptr _unit_type_table.Sound+6E4h], 86
[byte ptr _unit_type_table.Sound+708h], 151
[byte ptr _unit_type_table.Sound+72Ch], 151
[byte ptr _unit_type_table.Sound+750h], 86
[byte ptr _unit_type_table.Sound+774h], 150
[byte ptr _unit_type_table.Sound+798h], 102
[byte ptr _unit_type_table.Sound+7BCh], 131
[byte ptr _unit_type_table.Sound+7E0h], 85
[byte ptr _unit_type_table.Sound+804h], 86
[byte ptr _unit_type_table.Sound+828h], 86
[byte ptr _unit_type_table.Sound+84Ch], 86
[byte ptr _unit_type_table.Sound+870h], 102
[byte ptr _unit_type_table.Sound+894h], 151
[byte ptr _unit_type_table.Sound+8B8h], 150
[byte ptr _unit_type_table.Sound+8DCh], 6
[byte ptr _unit_type_table.Sound+900h], 151
[byte ptr _unit_type_table.Sound+924h], 134
[byte ptr _unit_type_table.Sound+948h], 134
[byte ptr _unit_type_table.Sound+96Ch], 134
[byte ptr _unit_type_table.Sound+990h], 134
[byte ptr _unit_type_table.Sound+9B4h], 134
[byte ptr _unit_type_table.Sound+9D8h], 135
[byte ptr _unit_type_table.Sound+9FCh], 135
[byte ptr _unit_type_table.Sound+0A20h], 134
[byte ptr _unit_type_table.Sound+0A44h], 150
[byte ptr _unit_type_table.Sound+0A68h], 116
[byte ptr _unit_type_table.Sound+0A8Ch], 134
[byte ptr _unit_type_table.Sound+0AB0h], 86
[byte ptr _unit_type_table.Sound+0AD4h], 86
[byte ptr _unit_type_table.Sound+0AF8h], 86
[byte ptr _unit_type_table.Sound+0B1Ch], 86
[byte ptr _unit_type_table.Sound+0B40h], 150
[byte ptr _unit_type_table.Sound+0B64h], 214
[byte ptr _unit_type_table.Sound+0B88h], 150
[byte ptr _unit_type_table.Sound+0BACh], 86
[byte ptr _unit_type_table.Sound+0BD0h], 86
[byte ptr _unit_type_table.Sound+0BF4h], 86
[byte ptr _unit_type_table.Sound+0C18h], 86
[byte ptr _unit_type_table.Sound+0C3Ch], 150
[byte ptr _unit_type_table.Sound+0C60h], 150
[byte ptr _unit_type_table.Sound+0C84h], 86
[byte ptr _unit_type_table.Sound+0CA8h], 86
[byte ptr _unit_type_table.Sound+0CCCh], 86
[byte ptr _unit_type_table.Sound+0CF0h], 151
[byte ptr _unit_type_table.Sound+0D14h], 150
[byte ptr _unit_type_table.Sound+0D38h], 150
[byte ptr _unit_type_table.Sound+0D5Ch], 86
[byte ptr _unit_type_table.Sound+0D80h], 86
[byte ptr _unit_type_table.Sound+0DA4h], 86
[byte ptr _unit_type_table.Sound+0DC8h], 102
[byte ptr _unit_type_table.Sound+0DECh], 151
[byte ptr _unit_type_table.Sound+0E10h], 150
[byte ptr _unit_type_table.Sound+0E34h], 86
[byte ptr _unit_type_table.Sound+0E58h], 102
[byte ptr _unit_type_table.Sound+0E7Ch], 134
[byte ptr _unit_type_table.Sound+0EA0h], 86
[byte ptr _unit_type_table.Sound+0EC4h], 86
[byte ptr _unit_type_table.Sound+0EE8h], 86
[byte ptr _unit_type_table.Sound+0F0Ch], 102
[byte ptr _unit_type_table.Sound+0F30h], 151
[byte ptr _unit_type_table.Sound+0F54h], 151
[byte ptr _unit_type_table.Sound+0F78h], 86
[byte ptr _unit_type_table.Sound+0F9Ch], 150
[byte ptr _unit_type_table.Sound+0FC0h], 86
[byte ptr _unit_type_table.Sound+0FE4h], 102
[byte ptr _unit_type_table.Sound+1008h], 150
[byte ptr _unit_type_table.Sound+102Ch], 150
[byte ptr _unit_type_table.Sound+1050h], 150
[byte ptr _unit_type_table.Sound+1074h], 150
[byte ptr _unit_type_table.Sound+1098h], 150
[byte ptr _unit_type_table.Sound+10BCh], 150
[byte ptr _unit_type_table.Sound+10E0h], 86
[byte ptr _unit_type_table.Sound+1104h], 86
[byte ptr _unit_type_table.Sound+1128h], 86
[byte ptr _unit_type_table.Sound+114Ch], 86
[byte ptr _unit_type_table.Sound+1170h], 151
[byte ptr _unit_type_table.Sound+1194h], 150
[byte ptr _unit_type_table.Sound+11B8h], 214
[byte ptr _unit_type_table.Sound+11DCh], 86
[byte ptr _unit_type_table.Sound+1200h], 86
[byte ptr _unit_type_table.Sound+1224h], 86
[byte ptr _unit_type_table.Sound+1248h], 151
[byte ptr _unit_type_table.Sound+126Ch], 151
[byte ptr _unit_type_table.Sound+1290h], 150
[byte ptr _unit_type_table.Sound+12B4h], 102
[byte ptr _unit_type_table.Sound+12D8h], 86
[byte ptr _unit_type_table.Sound+12FCh], 86
[byte ptr _unit_type_table.Sound+1320h], 132
[byte ptr _unit_type_table.Sound+1344h], 86
[byte ptr _unit_type_table.Sound+1368h], 86
[byte ptr _unit_type_table.Sound+138Ch], 86
[byte ptr _unit_type_table.Sound+13B0h], 86
[byte ptr _unit_type_table.Sound+13D4h], 102
[byte ptr _unit_type_table.Sound+13F8h], 151
[byte ptr _unit_type_table.Sound+141Ch], 151
[byte ptr _unit_type_table.Sound+1440h], 86
[byte ptr _unit_type_table.Sound+1464h], 150
[byte ptr _unit_type_table.Sound+1488h], 116
[byte ptr _unit_type_table.Sound+14ACh], 85
[byte ptr _unit_type_table.Sound+14D0h], 85
[byte ptr _unit_type_table.Sound+14F4h], 85
[byte ptr _unit_type_table.Sound+1518h], 151
[byte ptr _unit_type_table.Sound+153Ch], 149
[byte ptr _unit_type_table.Sound+1560h], 85
[byte ptr _unit_type_table.Sound+1584h], 162
[byte ptr _unit_type_table.Sound+15A8h], 147
[byte ptr _unit_type_table.Sound+15CCh], 147
[byte ptr _unit_type_table.Sound+15F0h], 130
[byte ptr _unit_type_table.Sound+1614h], 213
[byte ptr _unit_type_table.Sound+1638h], 131
[byte ptr _unit_type_table.Sound+165Ch], 130
[byte ptr _unit_type_table.Sound+1680h], 130
[byte ptr _unit_type_table.Sound+16A4h], 130
[byte ptr _unit_type_table.Sound+16C8h], 134
[byte ptr _unit_type_table.Sound+16ECh], 161
[byte ptr _unit_type_table.Sound+1710h], 112
[byte ptr _unit_type_table.Sound+1734h], 150
[byte ptr _unit_type_table.Sound+1758h], 150
[byte ptr _unit_type_table.Sound+177Ch], 147
[byte ptr _unit_type_table.Sound+17A0h], 147
[byte ptr _unit_type_table.Sound+17C4h], 130
[byte ptr _unit_type_table.Sound+17E8h], 130
[byte ptr _unit_type_table.Sound+180Ch], 130
[byte ptr _unit_type_table.Sound+1830h], 102
[byte ptr _unit_type_table.Sound+1854h], 113
[byte ptr _unit_type_table.Sound+1878h], 150
[byte ptr _unit_type_table.Sound+189Ch], 179
[byte ptr _unit_type_table.Sound+18C0h], 131
[byte ptr _unit_type_table.Sound+18E4h], 134
[byte ptr _unit_type_table.Sound+1908h], 134
[byte ptr _unit_type_table.Sound+192Ch], 147
[byte ptr _unit_type_table.Sound+1950h], 135
[byte ptr _unit_type_table.Sound+1974h], 135
[byte ptr _unit_type_table.Sound+1998h], 210
[byte ptr _unit_type_table.Sound+19BCh], 147
[byte ptr _unit_type_table.Sound+19E0h], 213
[byte ptr _unit_type_table.Sound+1A04h], 165
[byte ptr _unit_type_table.Sound+1A28h], 114
[byte ptr _unit_type_table.Sound+1A4Ch], 161
[byte ptr _unit_type_table.Sound+1A70h], 161
[byte ptr _unit_type_table.Sound+1A94h], 192
[byte ptr _unit_type_table.Sound+1AB8h], 8
[byte ptr _unit_type_table.Sound+1ADCh], 8
[byte ptr _unit_type_table.Sound+1B00h], 8
[byte ptr _unit_type_table.Sound+1B24h], 165
[byte ptr _unit_type_table.Sound+1B48h], 130
[byte ptr _unit_type_table.Sound+1B6Ch], 182
[byte ptr _unit_type_table.Sound+1B90h], 112
[byte ptr _unit_type_table.Sound+1BB4h], 146

1BB4h  7092d
7092 / 36 = 197

*/

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
    int16_t itr_cities;

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        Do_City_Calculations(itr_cities);
    }

}


// WZD o140p02
// drake178: MSG_Clear()
/*
    Unit: Lost, Killed
    UE, CE, OE: Lost
    City: Loss, Gain
    City: Grow, Shrink
    Building: Lost

    does not reset g_bldg_msg_ctr
*/
void MSG_Clear(void)
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
                Create_Unit__WIP((_CITIES[city_idx].construction - 100), _CITIES[city_idx].owner_idx, _CITIES[city_idx].wx, _CITIES[city_idx].wy, _CITIES[city_idx].wp, city_idx);

                UNIT_RemoveExcess((_units - 1));

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
                    strcpy(city_name, _CITIES[city_idx].name);
                    strcat(GUI_NearMsgString, city_name);
                    strcat(GUI_NearMsgString, cnst_TooManyUnits);  // ". You must disband some units if you wish to build or summon any more."
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

            _CITIES[city_idx].Prod_Accu = 0;  // BUGBUG ¿ drake178: discards excess ? not actually a bug, just prescribed behavior? "surplus production units will be lost"
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

            if(_CITIES[city_idx].Prod_Accu >= product_cost)
            {

                assert((_CITIES[city_idx].bldg_status[_CITIES[city_idx].construction] < 0));
                // IDGI:  ¿ impossible state - unreachable code ?
                // #CRASHME
                // BUGBUG
                if(_CITIES[city_idx].bldg_status[_CITIES[city_idx].construction] >= 0)  /* bs_Replaced, bs_Built, bs_Removed */
                {
                    _CITIES[city_idx].bldg_status[_CITIES[city_idx].construction] += 1;
                }
                else
                {
                    _CITIES[city_idx].bldg_status[_CITIES[city_idx].construction] = bs_Built;
                    if(bldg_data_table[_CITIES[city_idx].construction].replace_bldg != ST_UNDEFINED)
                    {
                        _CITIES[city_idx].bldg_status[bldg_data_table[_CITIES[city_idx].construction].replace_bldg] = bs_Replaced;
                    }

                    if(
                        (_CITIES[city_idx].construction == bt_Oracle)
                        &&
                        (_CITIES[city_idx].owner_idx == HUMAN_PLAYER_IDX)
                    )
                    {
                        TILE_ExploreRadius__WIP(_CITIES[city_idx].wx, _CITIES[city_idx].wy, _CITIES[city_idx].wp, 6);
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
                            MSG_Building_Complete[g_bldg_msg_ctr].city_idx = city_idx;
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
    int16_t normal_units[NUM_PLAYERS];
    int16_t food_incomes[NUM_PLAYERS];
    int16_t gold_incomes[NUM_PLAYERS];
    int16_t Excess_Food;

    int16_t itr_players;  // _SI_
    int16_t itr_heroes;  // _DI_
    int16_t itr_cities;  // _SI_

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

        if((gold_incomes[itr_players] + food_incomes[itr_players]) < 32000)
        {
            _players[itr_players].gold_reserve += (gold_incomes[itr_players] + food_incomes[itr_players]);
        }
        else
        {
            _players[itr_players].gold_reserve = 32000;
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
    _players[itr_players].Research_Left
    _players[itr_players].mana_reserve
    _players[itr_players].Casting_Skill

*/
void Players_Apply_Magic_Power(void)
{
    int16_t Research_Income;
    int16_t Mana_Income;
    int16_t Skill_Income;

    int16_t itr_players;  // _SI_

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Players_Apply_Magic_Power()\n", __FILE__, __LINE__);
#endif

    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        if(_players[itr_players].Spell_Cast != 0xD6 /* Spell_Of_Return */)
        {
            Player_Magic_Power_Income_Total(&Mana_Income, &Research_Income, &Skill_Income, itr_players);
        }

        if(_players[itr_players].Research_Left <= Research_Income)
        {
            _players[itr_players].Research_Left = 0;
        }
        else
        {
            _players[itr_players].Research_Left -= Research_Income;
        }

        if((32000 - _players[itr_players].mana_reserve) >= Mana_Income)
        {
            _players[itr_players].mana_reserve += Mana_Income;
        }
        else
        {
            _players[itr_players].mana_reserve = 32000;
        }

        // 'Archmage' Special Ability 50% bonus to all mana spent on increasing skill
        // ¿ vs. Calc_Nominal_Skill() ?
        if(_players[itr_players].archmage <= 0)
        {
            _players[itr_players].Casting_Skill += Skill_Income;
        }
        else
        {
            _players[itr_players].Casting_Skill += ((Skill_Income * 3) / 2);
        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Players_Apply_Magic_Power()\n", __FILE__, __LINE__);
#endif

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
    int16_t excess_foods[NUM_PLAYERS];
    int16_t mana_upkeeps[NUM_PLAYERS];
    int16_t gold_upkeeps[NUM_PLAYERS];
    int16_t food_upkeep;
    int16_t itr;  // _DI_

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
            (itr == 0)
        )
        {
            WIZ_MatchFoodUpkeep(itr, excess_foods[itr], food_upkeep);
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
void WIZ_MatchFoodUpkeep(int16_t player_idx, int16_t food_excess, int16_t food_upkeep)
{
    int16_t troops[MAX_STACK];
    int16_t itr_troops;
    int16_t troop_count;
    int16_t itr_units;  // _SI_

    for(itr_units = (_units - 1); ((itr_units > -1) && (food_upkeep > food_excess)); itr_units--)
    {
        if(
            (_UNITS[itr_units].owner_idx == player_idx) &&
            ((_unit_type_table[_UNITS[itr_units].type].Abilities & UA_FANTASTIC) == 0) &&
            (_UNITS[itr_units].type > ut_Chosen) &&
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
                        ((_unit_type_table[_UNITS[itr_units].type].Abilities & UA_FANTASTIC) == 0) &&
                        (_UNITS[itr_units].type > ut_Chosen)
                    )
                    {
                        food_upkeep--;

                        if(
                            (MSG_UnitLost_Count < 20) &&
                            (player_idx == HUMAN_PLAYER_IDX)
                        )
                        {
                            MSG_UnitLost_Array[MSG_UnitLost_Count].Unit_Type = _UNITS[itr_units].type;
                            MSG_UnitLost_Array[MSG_UnitLost_Count].Cause = 4;
                            MSG_UnitLost_Count++;
                        }
                        UNIT_MarkRemoved(troops[itr_troops], 0);
                    }
                }
            }
        }

        if(
            (MSG_UnitLost_Count < 20) &&
            (player_idx == HUMAN_PLAYER_IDX)
        )
        {
            MSG_UnitLost_Array[MSG_UnitLost_Count].Unit_Type = _UNITS[itr_units].type;
            MSG_UnitLost_Array[MSG_UnitLost_Count].Cause = 0;
            MSG_UnitLost_Count++;
        }
        UNIT_MarkRemoved(itr_units, 1);
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

                UNIT_GetDependants__WIP(itr_units, &troop_count, &troops[0]);

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
                        UNIT_MarkRemoved(troops[itr_troops], 0);
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
                UNIT_MarkRemoved(itr_units, 1);
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

            UNIT_GetDependants__WIP(itr_units, &troop_count, &troops[0]);

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
                    UNIT_MarkRemoved(troops[itr_troops], 0);
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
            UNIT_MarkRemoved(itr_units, 1);
            
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
    int16_t Surplus_Farmers;
    int16_t New_Min_Farmers;
    int16_t Population_Growth;
    int16_t itr_cities;

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
                    strcpy(&MSG_CityLost_Names[(MSG_CityLost_Count * 20)], _CITIES[itr_cities].name);
                    MSG_CityLost_Count++;
                }
                Destroy_City(itr_cities);
            }

            // 'Outpost' graduated to 'City'
            if(_CITIES[itr_cities].Pop_10s >= 10)
            {
                _CITIES[itr_cities].population = 1;
                _CITIES[itr_cities].size = 1;  /* CTY_Hamlet */
                _CITIES[itr_cities].farmer_count = City_Minimum_Farmers(itr_cities);
                Do_City_Calculations(itr_cities);
                if((_CITIES[itr_cities].owner_idx == HUMAN_PLAYER_IDX) && (MSG_CityGained_Count < 20))
                {
                    MSG_CityGained_Array[MSG_CityGained_Count] = itr_cities;
                    MSG_CityGained_Count++;
                }
            }
        }
        else
        {
            // apply population growth
            Population_Growth = City_Growth_Rate(itr_cities);

            _CITIES[itr_cities].Pop_10s += Population_Growth;

            // increase population
            if((_CITIES[itr_cities].Pop_10s >= 100) && (_CITIES[itr_cities].population < 25))
            {
                Surplus_Farmers = City_Minimum_Farmers(itr_cities);

                Surplus_Farmers -= _CITIES[itr_cities].farmer_count;

                _CITIES[itr_cities].population += 1;

                _CITIES[itr_cities].Pop_10s = 0;  /* ; BUG: discards excess population */

                New_Min_Farmers = City_Minimum_Farmers(itr_cities);

                _CITIES[itr_cities].farmer_count = (New_Min_Farmers + Surplus_Farmers);

                if(_CITIES[itr_cities].farmer_count > _CITIES[itr_cities].population)
                {
                    _CITIES[itr_cities].farmer_count = _CITIES[itr_cities].population;
                }

                _CITIES[itr_cities].size = ((_CITIES[itr_cities].population + 3) / 4);  // {0, ..., 24} {3, ..., 27} {0, 1, 2, 3, 4, 5, 6}
                // _CITIES[itr_cities].size = BUCKET(_CITIES[itr_cities].population, 4);

                SETMAX(_CITIES[itr_cities].size, MAX_CITY_SIZE);  // CTY_Capital

                if(_CITIES[itr_cities].owner_idx == NEUTRAL_PLAYER_IDX)
                {
                    // BUGBUG:  In City_Growth_Rate(), uses `if(_CITIES[city_idx].population >= ((_difficulty + 1) * 2))`
                    SETMAX(_CITIES[itr_cities].population, MAX_CITY_POPULATION_NEUTRAL_PLAYER);
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
                    _CITIES[itr_cities].Pop_10s = 5;
                }
                else
                {
                    _CITIES[itr_cities].population -= 1;

                    _CITIES[itr_cities].size = ((_CITIES[itr_cities].population + 3) / 4);

                    if((
                        _CITIES[itr_cities].owner_idx == HUMAN_PLAYER_IDX)
                        &&
                        (MSG_CityDeath_Count < 20)
                    )
                    {
                        MSG_CityDeath_Array[MSG_CityDeath_Count] = itr_cities;
                        MSG_CityDeath_Count++;
                    }

                    _CITIES[itr_cities].Pop_10s += 100;
                }
            }

            // decrease population
            if(_CITIES[itr_cities].enchantments[PESTILENCE] > 0)
            {
                if(_CITIES[itr_cities].population > Random(10))
                {
                    _CITIES[itr_cities].population -= 1;

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
            
            City_Apply_Production(itr_cities);

        }


        if(_CITIES[itr_cities].enchantments[CONSECRATION] > 0)
        {
            // TODO  CTY_Consecration(itr_cities);
        }

        if(_CITIES[itr_cities].enchantments[STREAM_OF_LIFE] > 0)
        {
            // TODO  CTY_StreamOfLife(itr_cities);
        }

        if(_CITIES[itr_cities].enchantments[CHAOS_RIFT] > 0)
        {
            // TODO  CTY_ChaosRift(itr_cities);
        }

        if(_CITIES[itr_cities].enchantments[GAIAS_BLESSING] > 0)
        {
            // TODO  CTY_GaiasBlessing(itr_cities);
        }
        
        if(_CITIES[itr_cities].enchantments[NIGHTSHADE] > 0)
        {
            // TODO  CTY_NightshadeDispel(itr_cities);
        }
    }

    Volcano_Counts();

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
// IDK_SplCst_SplSkl_sC5AB1()

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
    int16_t Bookshelf[NUM_MAGIC_TYPES];
    int16_t item_price;
    int16_t Hire_Response;
    int16_t Merc_Level;
    int16_t Merc_Cost;
    int16_t Merc_Amount;
    int16_t wp;
    int16_t wy;
    int16_t wx;
    int16_t Hero_Slot;
    int16_t unit_type;  // used for Generate_Mercenaries() and Pick_Random_Hero()
    int16_t player_fame;
    int16_t itr;
    int16_t itr_players;  // _SI_
    int16_t IDK;  // _DI_

    for(itr = 0; itr < NUM_MAGIC_TYPES; itr++)
    {
        Bookshelf[itr] = 12;
    }

    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {

        if(_players[itr_players].Spell_Cast == 214)  /* Spell_Of_Return */
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

            // HACK: 
            if(DBG_trigger_offer_item == ST_TRUE)
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

        // HACK: 
        if(DBG_trigger_offer_merc == ST_TRUE)
        {
            IDK = 100;
            _players[HUMAN_PLAYER_IDX].gold_reserve = 9999;
            DBG_trigger_offer_merc = ST_FALSE;
        }

        if(Random(100) <= IDK)
        {

            unit_type = 0;

            Generate_Mercenaries(itr_players, wx, wy, wp, &Merc_Amount, &unit_type, &Merc_Cost, &Merc_Level);

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

                        Hire_Response = Hire_Merc_Popup(unit_type, Merc_Amount, Merc_Level, Merc_Cost);

                        Set_Mouse_List(1, mouse_list_hourglass);

                    }

                    if(Hire_Response == ST_TRUE)
                    {

                        _players[itr_players].gold_reserve -= Merc_Cost;

                        for(itr = 0; itr < Merc_Amount; itr++)
                        {

                            Create_Unit__WIP(unit_type, itr_players, wx, wy, wp, -1);

                            _UNITS[(_units - 1)].Level = Merc_Level;

                            _UNITS[(_units - 1)].XP = TBL_Experience[Merc_Level];

                            _UNITS[(_units - 1)].Finished = 0;

                            _UNITS[(_units - 1)].moves2 = _UNITS[(_units - 1)].moves2_max;

                            UNIT_RemoveExcess((_units - 1));

                        }

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

        // HACK: 
        if(DBG_trigger_offer_hero == ST_TRUE)
        {
            IDK = 100;
            _players[HUMAN_PLAYER_IDX].gold_reserve = 9999;
            DBG_trigger_offer_hero = ST_FALSE;
        }

        if(
            (Random(100) <= IDK)
            &&
            ((_units + 1) < 1000)
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
                    Hire_Hero_Popup(Hero_Slot, unit_type, 0);
                    Set_Mouse_List(1, mouse_list_hourglass);
                }
                else
                {
                    AI_Accept_Hero(itr_players, Hero_Slot, unit_type);
                }

            }

        }

    }

}


// WZD o140p20
// WIZ_ResearchProgress()

// WZD o140p21
/*
    WIP: needs GAME_LearnSpellAnim() and AI_Research_Picker()

handles researched spell, traded/gifted spell, treasure spell, and conquest spell

*/
void WIZ_LearnSpell__WIP(int16_t player_idx, int16_t spell_idx, int16_t New_Research)
{
    int16_t Candidate_Count;
    int16_t Was_Research_Target;
    int16_t InRealm_Index;
    int16_t Spell_Realm;
    int16_t itr;  // _DI_

    Was_Research_Target = ST_FALSE;

    if(_players[player_idx].SoM_RC > spell_data_table[spell_idx].research_cost)
    {
        _players[player_idx].SoM_RC = spell_data_table[spell_idx].research_cost;
    }
    else
    {
        _players[player_idx].SoM_RC = 0;
    }

    Spell_Realm = ((spell_idx - 1) / 40);

    InRealm_Index = ((spell_idx - 1) % 40);

    _players[player_idx].Spells_Known[((Spell_Realm * 40) + InRealm_Index)] = 2;  /* S_Known */

    if(_players[player_idx].research_spell_idx == spell_idx)
    {
        Was_Research_Target = ST_TRUE;

        for(itr = 0; itr < NUM_RESEARCH_SPELLS; itr++)
        {
            if(_players[player_idx].research_spells[itr] == _players[player_idx].research_spell_idx)
            {
                Clear_Structure(itr, (uint8_t *)&_players[player_idx].research_spells[0], sizeof(_players[player_idx].research_spells[0]), NUM_RESEARCH_SPELLS);
                _players[player_idx].research_spells[(NUM_RESEARCH_SPELLS - 1)] = 0; // BUG  ¿ drake178: ; already done above ?
            }
        }

        Candidate_Count = WIZ_RefreshResearch__STUB(player_idx);

        _players[player_idx].Research_Left = 0;

        if(Candidate_Count == 0)
        {
            _players[player_idx].research_spell_idx = -1; /* No_spell */
        }
        else
        {
            _players[player_idx].research_spell_idx = -(_players[player_idx].research_spell_idx);
        }
    }
    else
    {
        // BUG  ¿ refactor or this just shouldn't being doing the same as above ?
        for(itr = 0; itr < NUM_RESEARCH_SPELLS; itr++)
        {
            if(_players[player_idx].research_spells[itr] == _players[player_idx].research_spell_idx)
            {
                Clear_Structure(itr, (uint8_t *)&_players[player_idx].research_spells[0], sizeof(_players[player_idx].research_spells[0]), NUM_RESEARCH_SPELLS);
                _players[player_idx].research_spells[(NUM_RESEARCH_SPELLS - 1)] = 0; // BUG  ¿ drake178: ; this is already done by the above function ?
                WIZ_RefreshResearch__STUB(player_idx);
            }
        }
    }


    if(
        (player_idx == HUMAN_PLAYER_IDX)
        &&
        (New_Research == ST_TRUE)
    )
    {

        // ; displays the spell learning animations and, if new
        // ; research is enabled, flips the apprentice book to the
        // ; pages with the research candidates afterwards
        // TODO  GAME_LearnSpellAnim(spell_idx, Was_Research_Target);

        if(Was_Research_Target == ST_FALSE)
        {
            // TODO  SND_Silence();
            // TODO  SND_PlayBkgrndTrack();
        }
    }
    else if(
        (Was_Research_Target == ST_TRUE)
        &&
        (player_idx == HUMAN_PLAYER_IDX)
    )
    {
        // ; selects the spell to research from the wizard's list
        // ; of candidates using a weighted random roll,
        // ; prioritizing combat spells in research groups from
        // ; which the player does not yet have a known spell
        // ; contains multipe BUGs that prevent research-related
        // ; profile traits from properly affecting the outcome
        // TODO  AI_Research_Picker(player_idx);

        if(_players[player_idx].research_spell_idx == 213)  /* Spell_Of_Mastery */
        {
            _players[player_idx].Research_Left = _players[player_idx].SoM_RC;
        }
        else
        {
            _players[player_idx].Research_Left = spell_data_table[spell_idx].research_cost;
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
                UNIT_MarkRemoved(troops[itr_troops], 2);
            }
        }

        for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
        {
            UNIT_HealArray[troops[itr_troops]] = 10;
        }

        if(_CITIES[itr_cities].bldg_status[ANIMISTS_GUILD] > bs_Replaced)  /* {-1:NotBuilt,0:Replaced,1:Built,2:Removed} */
        {
            UNIT_HealArray[troops[itr_troops]] = 6;
        }
    }

    for(itr_units = 0; itr_units < _units; itr_units++)
    {

        // HACK: 
        if (_UNITS[itr_units].owner_idx == ST_UNDEFINED)
        {
            continue;
        }

        if(
            (g_TimeStop_PlayerNum == 0)
            ||
            (_UNITS[itr_units].owner_idx == g_TimeStop_PlayerNum)
        )
        {

            if(
                (_unit_type_table[_UNITS[itr_units].type].Race != rt_Death)
                &&
                ((_UNITS[itr_units].mutations & UM_UNDEAD) == 0)
            )
            {
                Heal_Unit(itr_units, UNIT_HealArray[itr_units], ST_FALSE);
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
                            UNIT_MarkRemoved(troops[itr_troops], 2);
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
                                UNIT_MarkRemoved(troops[itr_troops], 2);
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
                        Heal_Unit(itr_units, UNIT_HealArray[itr_units], ST_FALSE);
                    }

                }

            }

        }

    }



}


// WZD o140p24
// WIZ_ProcessGlobals()

// WZD o140p25
// MoO2  Module: INITSHIP  Repair_Ships_At_Colonies_()
// Next_Turn_Calc_() |-> Do_All_Ships_XP_Check_() |-> Best_Instructor_Bonus_(); Do_XP_For_Ship_() |-> Do_Academy_At_Location_Check_(); Calc_Ship_Level_()
/*
    heals units
    clears stasis
    ...
    _UNITS[itr_units].XP += 1;
    New_Level = Calc_Unit_Level(itr_units);
    Hero_LevelUp_Popup(itr_units);
    _UNITS[itr_units].Level = Calc_Unit_Level(itr_units);    
    ...
    _UNITS[troop_list[itr_troops]].XP += Highest_Armsmaster_XP;
    _UNITS[troop_list[itr_troops]].Level = Calc_Unit_Level(troop_list[itr_troops]);

*/
void Do_All_Units_XP_Check_(void)
{
    struct s_BATTLE_UNIT battle_unit;
    int16_t Processed_Hero_List[NUM_HEROES];
    int16_t troop_list[MAX_STACK];
    int16_t New_Level;
    int16_t hero_unit_idx;
    int16_t troop_count;
    int16_t Highest_Armsmaster_XP;
    int16_t XP_Gain;
    int16_t itr_heroes;
    int16_t itr_units;  // _SI_
    int16_t itr_players;  // _SI_
    int16_t itr_troops;  // _DI_

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
            _UNITS[itr_units].Damage = 0;
        }

        if((_UNITS[itr_units].mutations & C_STASISLINGER) != 0)
        {
            Load_Battle_Unit(itr_units, &battle_unit);

            if(BU_ResistRoll__STUB(battle_unit, -5, sbr_Sorcery) == 0)
            {
                _UNITS[itr_units].mutations = (_UNITS[itr_units].mutations & 0x7F /*0b01111111*/);  // ¿ xor     al, C_STASISLINGER  10000000b ?
            }
        }

        if((_UNITS[itr_units].mutations & C_STASISINIT) != 0)
        {
            _UNITS[itr_units].mutations = (_UNITS[itr_units].mutations | C_STASISLINGER);
            _UNITS[itr_units].mutations = (_UNITS[itr_units].mutations & 0xBF /*0b10111111*/);  // ¿ xor     al, C_STASISINIT  01000000b ?
        }

        if(
            ((_unit_type_table[_UNITS[itr_units].type].Abilities & UA_FANTASTIC) == 0)
            &&
            ((_UNITS[itr_units].mutations & UM_UNDEAD) == 0)
            &&
            (
                (g_TimeStop_PlayerNum == 0)
                ||
                (_UNITS[itr_units].owner_idx == g_TimeStop_PlayerNum)
            )
        )
        {

            _UNITS[itr_units].XP += 1;

            New_Level = Calc_Unit_Level(itr_units);

            if(_UNITS[itr_units].Hero_Slot > -1)
            {
                if(_UNITS[itr_units].owner_idx == HUMAN_PLAYER_IDX)
                {
                    if(_UNITS[itr_units].Level < New_Level)
                    {
                        Hero_LevelUp_Popup(itr_units);
                    }
                }
            }

            _UNITS[itr_units].Level = Calc_Unit_Level(itr_units);

        }


    }

    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        for(itr_heroes = 0; itr_heroes < NUM_HEROES; itr_heroes++)
        {
            Processed_Hero_List[itr_heroes] = ST_FALSE;
        }

        if(
            (g_TimeStop_PlayerNum == 0)
            ||
            (g_TimeStop_PlayerNum == itr_players)
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
						int idxHeroSlot = _UNITS[troop_list[itr_troops]].Hero_Slot;	// ; BUG: this can also be -1!
                        XP_Gain = 0;

                        if(idxHeroSlot > -1)
                        {
							Processed_Hero_List[idxHeroSlot] = ST_TRUE;

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
                                _UNITS[troop_list[itr_troops]].XP += Highest_Armsmaster_XP;

                                _UNITS[troop_list[itr_troops]].Level = Calc_Unit_Level(troop_list[itr_troops]);
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
// AI_CullTheWeak()

// WZD o140p28
// AI_GetAvgUnitCosts()



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
