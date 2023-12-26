/*
    Next Turn Procedure

    WIZARDS.EXE
        ovr060
        ovr119
        ovr121
        ovr140
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



/*
    WIZARDS.EXE  ovr060
*/

// WZD o60p01
// WZD o60p02
// WZD o60p03


// WZD o60p04
void Next_Turn_Proc(void)
{
    char temp_string[20];
    int16_t orig_map_plane;
    int16_t itr_msg;  // _SI_
    int16_t curr_prod_idx;  // _DI_

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Next_Turn_Proc()\n", __FILE__, __LINE__);
#endif

    // TODO  GAME_DeleteDeadUnits();
    // TODO  GAME_InTowerRefresh();

    Set_Unit_Draw_Priority();
    Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);
    Reset_Map_Draw();



    Next_Turn_Calc();



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
    if(MSG_Building_Complete_Count > 0)
    {
        // DONT  j_o62p01_Empty_pFxn(_human_player_idx);  // drake178: does nothing and returns zero; at some point must have been some wizard data refresh function
        for(itr_msg = 0; itr_msg < MSG_Building_Complete_Count; itr_msg++)
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
                    City_Built_Building_Message(5, 101, city_built_bldg_idx);
                }
                else  /* (MSG_Building_Complete[itr_msg].bldg_type_idx < bt_NONE) */
                {
                    if(_CITIES[_city_idx].construction == bt_GRANDVIZIER)
                    {
                        _CITIES[_city_idx].construction = bt_Housing;
                    }
                    else
                    {
                        strcpy(GUI_NearMsgString, "The ");
                        strcat(GUI_NearMsgString, STR_TownSizes[_CITIES[_city_idx].size]);
                        strcat(GUI_NearMsgString, " of ");
                        // TODO  String_Copy_Far(near_buffer, _CITIES[_city_idx].name);
                        strcpy(near_buffer, _CITIES[_city_idx].name);
                        strcat(GUI_NearMsgString, near_buffer);
                        strcat(GUI_NearMsgString, " can no longer produce ");
                        curr_prod_idx = _CITIES[_city_idx].construction;
                        if(curr_prod_idx >= 100)
                        {
                            curr_prod_idx -= 100;
                            strcat(GUI_NearMsgString, *_unit_type_table[curr_prod_idx].Name);
                        }
                        else
                        {
                            // String_Copy_Far(temp_string, bldg_data_table[curr_prod_idx].name);
                            strcpy(temp_string, bldg_data_table[curr_prod_idx].name);
                            strcat(GUI_NearMsgString, STR_GetIndefinite(&temp_string[0]));
                            strcat(GUI_NearMsgString, " ");
                            strcat(GUI_NearMsgString, temp_string);
                        }
                        strcat(GUI_NearMsgString, ".");
                        // TODO  GUI_WarningType0(GUI_NearMsgString);
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

    // TODO  MSG_Building_Complete_Count = 0;

    Update_Units_MvsSts();
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
// ¿ AI version ?
void Update_Units_MvsSts(void)
{
    int16_t itr_units;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Update_Units_MvsSts()\n", __FILE__, __LINE__);
#endif

    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        // default all to 'orderable'
        _UNITS[itr_units].Finished = ST_FALSE;

        // if *busy* doing "PATROL", "BUILD", or "CASTING", (re-)set to 'unorderable'
        if(
            ((_UNITS[itr_units].Status & US_Patrol) != 0) ||
            ((_UNITS[itr_units].Status & US_BuildRoad) != 0) ||
            ((_UNITS[itr_units].Status & US_Casting) != 0)
        )
        {
            _UNITS[itr_units].Finished = ST_TRUE;
        }

        // if(
        //     ((_UNITS[itr_units].Status & US_Wait) != 0) ||
        //     ((_UNITS[itr_units].Status & US_ReachedDest) != 0)
        // )
        // 'Unit Status'  4  0x04  "DONE"       enum US_ReachedDest
        // 'Unit Status'  5  0x05  "WAIT"       enum US_Wait
        // 'Unit Status'  0  0x00  "NO ORDERS"  enum US_Ready
        if( (_UNITS[itr_units].Status == US_Wait) || (_UNITS[itr_units].Status == US_ReachedDest) )
        {
            _UNITS[itr_units].Status = US_Ready;  // "NO ORDERS"
        }


        // ; calculates and returns the unit's movement allowance in half movement point units, accounting for all non-combat factors
        _UNITS[itr_units].HalfMoves_Max = UNIT_GetHalfMoves_WIP(itr_units);

        _UNITS[itr_units].HMoves = _UNITS[itr_units].HalfMoves_Max;


    }

    TILE_VisibilityUpdt();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Update_Units_MvsSts()\n", __FILE__, __LINE__);
#endif

}


// WZD o60p07
// drake78: TILE_VisibilityUpdt()
/*
LoadScr.C:    TILE_VisibilityUpdt();
MoX_UnOrg.C:// TILE_VisibilityUpdt()
MoX_UnOrg.C:void TILE_VisibilityUpdt(void)
MoX_UnOrg.H:void TILE_VisibilityUpdt(void);
*/
// void TILE_VisibilityUpdt(void);



/*
    WIZARDS.EXE  ovr119
*/

// WZD o119p01
void Next_Turn_Calc(void)
{

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Next_Turn_Calc()\n", __FILE__, __LINE__);
#endif

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

// call    j_GAME_ProcessPurify            ; progress the purification counters of all purifying
//                                         ; units, cleansing any tiles where the treshold has
//                                         ; been reached
//                                         ; WARNING: the Target_X field may not be the best
//                                         ; place to hold this information?

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



// cmp     [DBG_Alt_A_State], e_ST_FALSE
// jnz     short loc_9EC87
// cmp     [magic_set.Random_Events], e_ST_TRUE
// jnz     short loc_9EC87

// call    j_EVNT_GenerateRandom           ; attempts to generate a random event
//                                         ; has some BUGs that need review


// loc_9EC87:                              ; progresses any ongoing events (status 1 or 2)
// call    j_EVNT_Progress                 ;
//                                         ; has several BUGs relating to specific events



    Players_Apply_Upkeeps__WIP();



// call    EMM_Map_DataH                   ; maps the EMM Data block into the page frame

// call    j_CTY_OutpostGrowth             ; calculates and records the growth and/or shrinkage
//                                         ; of all outposts, although turning into a full city
//                                         ; is not in this function
//                                         ; BUG: this function ignores Wild Games altogether,
//                                         ; both as a source of food and as a terrain special
//                                         ; BUG: difficulty-based outpost growth modifiers are
//                                         ; applied to both AI and human player outposts


    CTY_ProgressTurn();


// call    j_WIZ_ProcessGlobals            ; processes the diplomatic reactions and persistent
//                                         ; effects of global enchantments and the Spell of
//                                         ; Mastery
//                                         ; WARNING: Herb Mastery is not included here despite
//                                         ; having a persistent effect
//                                         ; also contains multiple inherited BUGs



    /*
        BEGIN: Grand Vizier
    */
    if(grand_vizier == ST_TRUE)
    {

    }
    /*
        END: Grand Vizier
    */



// loc_9ED2A:                              ; processes the wandering merchant, mercenary, and hero
// call    j_EVNT_RandomOffers             ; for hire offers for all players
//                                         ; has multiple BUGs, both own and inherited


    Set_Mouse_List(1, mouse_list_hourglass);


// call    j_CTY_CountNightshades          ; counts and sets the amount of Nightshades affecting
//                                         ; every city (returns the last count)
//                                         ; BUG: only updates the count if a relevant building is
//                                         ; present, meaning that it can't be cleared either if
//                                         ; one isn't

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


// call    j_IDK_Dipl_s_7436F

// call    j_IDK_Dipl_s74B68

// call    j_Cool_Off_Volcanoes

// jmp     short loc_9EDF3

    }


// call    j_IDK_SplCst_SplSkl_sC5AB1

// call    j_GAME_DeleteDeadUnits          ; deletes all removed units from the unit table, updating the indices of all hired heroes accordingly


    Set_Unit_Draw_Priority();
    Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);


// call    j_IDK_CtyBld_s4D357

// call    j_IDK_Unit_XP_sC6BCF

// call    j_IDK_Unit_Heal_sC6572


// MoO2
// Module: BILL
//     code (0 bytes) Record_History_
//     Address: 01:0010208A
// call    j_WIZ_RecordHistory             ; calculates and records the astrologer/historian data
//                                         ; Power = power base - 10
//                                         ; Army Strength = (gold upkeep + mana upkeep) / 5
//                                         ; Research = sum of known in-realm indexes / 10
//                                         ; Historian = (above 3) + ((cities pop_K sum) / 10)
//                                         ; Astrologer's max scale is 200, Historian's is 160



// call    j_IDK_SND_BkGrnd_s518AE


    _turn++;


    // TODO  cap gold, mana 30000


    // TODO  OVL_EnableIncmBlink();
    

// call    j_GAME_AutoSave                 ; if the current turn is divisible by 4, saves the game
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

// WZD o119p03





/*
    WIZARDS.EXE  ovr121
*/

// WZD o121p01
// drake178: UNIT_Create()
// MoO2  Module: COLBLDG  Colbldg_Create_Ship_() |-> Create_Ship_()
/*

    CTY_ProdProgress()
        UNIT_Create((_CITIES[city_idx].construction - 100), _CITIES[city_idx].owner_idx, _CITIES[city_idx].wx, _CITIES[city_idx].wy, _CITIES[city_idx].wp, city_idx);

R_Param
    if >= 0 && < 2000
        treats it as city_idx
    if < -1
        takes abs()-1 to index into TBL_Experience[9] = {0, 20, 60, 120, 200, 300, 450, 600, 1000}
        {-2, -3, -4, -5, -6, -7, -8, -9 }
        { 1,  2,  3,  4,  5,  6,  7,  8 }
        then, updates s_UNIT.level from UNIT_GetLevel()

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
                _UNITS[_units].HalfMoves_Max = _unit_type_table[unit_type].Move_Halves;
                _UNITS[_units].type = unit_type;
                _UNITS[_units].Hero_Slot = -1;
                _UNITS[_units].In_Tower = ST_FALSE;
                _UNITS[_units].Finished = ST_TRUE;
                _UNITS[_units].HMoves = 0;
                _UNITS[_units].Sight_Range = _unit_type_table[unit_type].Sight;
                _UNITS[_units].dst_wx = 0;
                _UNITS[_units].dst_wy = 0;
                _UNITS[_units].Status = 0;
                _UNITS[_units].Level = 0;
                _UNITS[_units].XP = 0;
                _UNITS[_units].Damage = 0;
                _UNITS[_units].Draw_Priority = 0;
                _UNITS[_units].Enchants_HI = 0;
                _UNITS[_units].Enchants_LO = 0;
                _UNITS[_units].Mutations = 0;
                _UNITS[_units].Move_Failed = 0;
                _UNITS[_units].Rd_Constr_Left = -1;

                if((R_Param < 0) || R_Param >= 2000)
                {
                    if(R_Param < -1)
                    {
                        R_Param = (abs(R_Param) - 1);
                        _UNITS[_units].XP = TBL_Experience[R_Param];
                        _UNITS[_units].Level = Unit_Level(_units);
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

                    _UNITS[_units].Mutations = City_Best_Weapon(R_Param);

                    if((_unit_type_table[unit_type].Abilities & 0x20) != 0)  /* Ab_CrOutpost */
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
                                        (_CITIES[R_Param].wx == _FORTRESSES[itr].wx) &&
                                        (_CITIES[R_Param].wy == _FORTRESSES[itr].wy) &&
                                        (_CITIES[R_Param].wp == _FORTRESSES[itr].wp)
                                    )
                                    {
                                        _CITIES[R_Param].population += 1;
                                        if(itr == HUMAN_PLAYER_IDX)
                                        {
                                            LBX_Load_Data_Static(message_lbx_file, 0, (SAMB_ptr)GUI_NearMsgString, 11, 1, 150);
                                            // TODO  GUI_WarningType0(GUI_NearMsgString);
                                            goto Done_Failure;
                                        }
                                    }
                                }
                            }
                            // TODO  CTY_Remove(R_Param);
                        }
                    }

                    if(
                        (_players[owner_idx].alchemy > 0) &&
                        (_UNITS[_units].Mutations == 0)
                    )
                    {
                        _UNITS[_units].Mutations = wq_Magic;                        
                    }

                    if(
                        (_players[owner_idx].Globals[CHAOS_SURGE] > 0) &&
                        ((_unit_type_table[unit_type].Abilities & 0x01) == 0)  /* attr_FantasticUnit */
                    )
                    {
                        // TODO  UNIT_ChaosChannel(_units);
                    }

                    _UNITS[_units].Level = Unit_Level(_units);
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Player_Base_Casting_Skill()\n", __FILE__, __LINE__);
#endif

    casting_skill = sqrt(_players[player_idx].Casting_Skill);

    if(_players[player_idx].archmage > 0)
    {
        casting_skill += 10;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Player_Base_Casting_Skill()\n", __FILE__, __LINE__);
#endif

    return casting_skill;
}


// WZD o121p03
// drake178: WIZ_GetCastingCost()
// WIZ_GetCastingCost()

// WZD o121p04
// drake178: WIZ_CastingCostBonus()
// WIZ_CastingCostBonus()

// WZD o121p05
// drake178: WIZ_GetHeroCount()
// WIZ_GetHeroCount()

// WZD o121p06
// drake178: WIZ_DeadHeroCount()
// WIZ_DeadHeroCount()

// WZD o121p07
// drake178: WIZ_GetRandomHero()
// WIZ_GetRandomHero()


// WZD o121p08
/*
    Calculate Movement Points for Unit
        Unit Type Table :: Movement Points
        Hero Unit Items - ¿ ... ?
        Chaos Channels - Demon Wings
        Unit Enchantment - Endurance
        Wind Mastery
*/
int16_t UNIT_GetHalfMoves_WIP(int16_t unit_idx)
{
    uint16_t Enchant_Flags_LO;
    uint16_t Enchant_Flags_HI;
    int16_t * ptr_players_heroes_items;
// Wind_Mastery_Total= word ptr -6
    int16_t Item_HalfMoves;
    int16_t Endurance;

    int16_t movement_points;

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: BEGIN: UNIT_GetHalfMoves(unit_idx = %d)\n", __FILE__, __LINE__, unit_idx);
// #endif

    /* ¿DNE? */ Endurance = ST_FALSE;

    /*
        BEGIN: Hero Items
    */
    if(_UNITS[unit_idx].Hero_Slot > -1)
    {
        ptr_players_heroes_items = &(_players[_UNITS[unit_idx].owner_idx].Heroes[_UNITS[unit_idx].Hero_Slot].Items[0]);
    }
    // ...
    Item_HalfMoves = 0;
    /*
        END: Hero Items
    */

    movement_points = _unit_type_table[_UNITS[unit_idx].type].Move_Halves;

    Enchant_Flags_HI = _UNITS[unit_idx].Enchants_HI;
    Enchant_Flags_LO = _UNITS[unit_idx].Enchants_LO;

    if(movement_points < 6)
    {
        if((Enchant_Flags_HI & 0x02) != 0)  /* UE_Flight */
        {
            movement_points = 6;
        }
    }

    if((_UNITS[unit_idx].Mutations & 0x08) != 0)  /* CC_Flight */
    {
        movement_points = 4;
    }

    if((Enchant_Flags_LO & 0x2000) != 0)  /* UE_Endurance */
    {
        Endurance = ST_TRUE;
    }

    if(Endurance == ST_TRUE)
    {
        movement_points += 2;
    }

    movement_points += Item_HalfMoves;



    /*
        BEGIN: Wind Mastery
    */



    /*
        END: Wind Mastery
    */



// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: END: UNIT_GetHalfMoves(unit_idx = %d) { movement_points = %d }\n", __FILE__, __LINE__, unit_idx, movement_points);
// #endif

    return movement_points;
}


// WZD o121p09
// drake178: UNIT_IsNormalUnit()
// UNIT_IsNormalUnit()

// WZD o121p10
// drake178: ¿ ?
// IDK_MagicScreen_PWR()

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
// CTY_RemoveMessages()



/*
    WIZARDS.EXE  ovr140
*/

// WZD o140p01
// drake178: CTY_RecalculateAll()
void All_City_Calculations(void)
{
    int16_t itr_cities;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: All_City_Calculations()\n", __FILE__, __LINE__);
#endif

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        Do_City_Calculations(itr_cities);
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: All_City_Calculations()\n", __FILE__, __LINE__);
#endif

}


// WZD o140p02
// drake178: MSG_Clear()
/*
    Unit: Lost, Killed
    UE, CE, OE: Lost
    City: Loss, Gain
    City: Grow, Shrink
    Building: Lost

    does not reset MSG_Building_Complete_Count
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
/*
    accumulate production points
        apply production points
            create building
            create unit
    do 'Grand Vizier' // do 'Computer Player' product decision/selection

*/
void CTY_ProdProgress(int16_t city_idx)
{
    char city_name[20];
    int16_t UU_garrison_units[9];
    int16_t UU_garrison_count;
    int16_t curr_prod_cost;  // _SI_

    curr_prod_cost = City_Current_Product_Cost(city_idx);

    if(_CITIES[city_idx].population > 0)
    {
        if(_CITIES[city_idx].construction >= 100)  /* *Product* is 'Unit' */
        {
            _CITIES[city_idx].Prod_Accu += _CITIES[city_idx].production_units;

            if(_CITIES[city_idx].Prod_Accu >= curr_prod_cost)
            {
                if((_units + 1) < MAX_UNIT_COUNT)
                {
                    Create_Unit__WIP(
                        (_CITIES[city_idx].construction - 100), 
                        _CITIES[city_idx].owner_idx,
                        _CITIES[city_idx].wx,
                        _CITIES[city_idx].wy,
                        _CITIES[city_idx].wp,
                        city_idx
                    );

                    // TODO  UNIT_RemoveExcess(_units - 1);

                    Get_Units_City(city_idx, &UU_garrison_count, &UU_garrison_units[0]);

                    if(
                        (_CITIES[city_idx].owner_idx != HUMAN_PLAYER_IDX) ||
                        (grand_vizier == ST_TRUE)
                    )
                    {
                        _CITIES[city_idx].construction = bt_GRANDVIZIER;
                    }

                }
                else
                {
                    if(_CITIES[city_idx].owner_idx != HUMAN_PLAYER_IDX)
                    {
                        _CITIES[city_idx].construction = bt_GRANDVIZIER;
                    }
                    else
                    {
                        LBX_Load_Data_Static(message_lbx_file, 0, (SAMB_ptr)GUI_NearMsgString, 66, 1, 150);
                        // TODO  String_Copy_Far(city_name, _CITIES[city_idx].name)
                        strcpy(city_name, _CITIES[city_idx].name);
                        strcat(GUI_NearMsgString, city_name);
                        strcat(GUI_NearMsgString, cnst_TooManyUnits);
                        // TODO  GUI_WarningType0(GUI_NearMsgString);

                        if(
                            (_CITIES[city_idx].owner_idx == HUMAN_PLAYER_IDX) &&
                            (grand_vizier == ST_TRUE)
                        )
                        {
                            _CITIES[city_idx].construction = bt_GRANDVIZIER;
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
            if(_CITIES[city_idx].construction < bt_Barracks)
            {
                if(_CITIES[city_idx].owner_idx != HUMAN_PLAYER_IDX)
                {
                    _CITIES[city_idx].construction = bt_GRANDVIZIER;
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

                if(_CITIES[city_idx].Prod_Accu >= curr_prod_cost)
                {

                    assert((_CITIES[city_idx].bldg_status[_CITIES[city_idx].construction] < 0));
                    // IDGI:  ¿ impossible state - unreachable code ?
                    // #CRASHME
                    if(_CITIES[city_idx].bldg_status[_CITIES[city_idx].construction] >= 0)
                    {
                        _CITIES[city_idx].bldg_status[_CITIES[city_idx].construction] += 1;
                    }
                    else
                    {
                        _CITIES[city_idx].bldg_status[_CITIES[city_idx].construction] = bs_Built;
                        if(bldg_data_table[_CITIES[city_idx].construction].replace_bldg != -1)
                        {
                            _CITIES[city_idx].bldg_status[bldg_data_table[_CITIES[city_idx].construction].replace_bldg] = bs_Replaced;
                        }

                        if(
                            (_CITIES[city_idx].construction == bt_Oracle) &&
                            (_CITIES[city_idx].owner_idx == HUMAN_PLAYER_IDX)
                        )
                        {
                            TILE_ExploreRadius(_CITIES[city_idx].wx, _CITIES[city_idx].wy, _CITIES[city_idx].wp, 6);
                        }

                        _CITIES[city_idx].bldg_cnt += 1;

                        _CITIES[city_idx].Prod_Accu = 0;

                        if(_CITIES[city_idx].owner_idx != HUMAN_PLAYER_IDX)
                        {
                            _CITIES[city_idx].construction = bt_GRANDVIZIER;
                        }
                        else
                        {
                            if(MSG_Building_Complete_Count < 20)
                            {
                                MSG_Building_Complete[MSG_Building_Complete_Count].city_idx = city_idx;
                                MSG_Building_Complete[MSG_Building_Complete_Count].bldg_type_idx = _CITIES[city_idx].construction;
                                MSG_Building_Complete_Count++;
                            }

                            if(grand_vizier == ST_TRUE)
                            {
                                _CITIES[city_idx].construction = bt_GRANDVIZIER;
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
            (_CITIES[city_idx].owner_idx == HUMAN_PLAYER_IDX) &&
            (grand_vizier == ST_TRUE) &&
            (_CITIES[city_idx].construction != -4)
        )
        {
            // TODO  CTY_GrandVizier(city_idx);
        }
    }

}


// WZD o140p04
// drake178: WIZ_GoldIncomes()
void Update_Players_Gold_Reserve(void)
{
    int16_t normal_units[6];
    int16_t food_incomes[6];
    int16_t gold_incomes[6];
    int16_t Excess_Food;

    int16_t itr_players;  // _SI_
    int16_t itr_heroes;  // _DI_
    int16_t itr_cities;  // _SI_

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Update_Players_Gold_Reserve()\n", __FILE__, __LINE__);
#endif

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
            if(_players[itr_players].Heroes[itr_heroes].Unit_Index > -1)
            {

                if( (p_heroes[itr_players][_UNITS[_players[itr_players].Heroes[itr_heroes].Unit_Index].type].Abilities_HI & 0x2000 /* Ab_Noble */) != 0)
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Update_Players_Gold_Reserve()\n", __FILE__, __LINE__);
#endif

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
    int16_t excess_foods[6];
    int16_t mana_upkeeps[6];
    int16_t gold_upkeeps[6];
    int16_t food_upkeep;

    int16_t itr;  // _DI_

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Players_Apply_Upkeeps()\n", __FILE__, __LINE__);
#endif

    for(itr = 0; itr < 6; itr++)
    {
        excess_foods[itr] = 0;
    }

    for(itr = 0; itr < _cities; itr++)
    {
        if(
            (_CITIES[itr].owner_idx != -1) &&
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
            (excess_foods[itr] < food_upkeep) &&
            (itr == 0)
        )
        {
        // TODO  WIZ_MatchFoodUpkeep(itr, excess_foods[itr], food_upkeep);
            // ; attempts to disband non-hero, non-transport normal
            // ; units until the food upkeep matches or is less than
            // ; the food income
            // ; inherits a severe BUG from UNIT_GetDependants
        }

        gold_upkeeps[itr] = Player_Armies_Gold_Upkeep(itr);

        if(
            (gold_upkeeps[itr] > _players[itr].gold_reserve) &&
            (itr == 0)
        )
        {
        // TODO  gold_upkeeps[itr] = WIZ_MatchGoldUpkeep(itr, gold_upkeeps[itr]);
            // ; attempts to disband non-hero, non-transport normal
            // ; units until the gold upkeep matches or is less than
            // ; the wizard's current gold
            // ; inherits a severe BUG from UNIT_GetDependants
        }

        _players[itr].gold_reserve = (_players[itr].gold_reserve - gold_upkeeps[itr]);

        if(_players[itr].gold_reserve < 0)
        {
            _players[itr].gold_reserve = 0;
        }

        mana_upkeeps[itr] = Player_Armies_And_Enchantments_Mana_Upkeep(itr);

        if(
            (mana_upkeeps[itr] > _players[itr].mana_reserve) &&
            (itr == 0)
        )
        {
        // TODO  mana_upkeeps[itr] = WIZ_MatchManaUpkeep(itr, mana_upkeeps[itr]);
            // ; attempts to remove unit enchantments, global
            // ; enchantments, city enchantments, or summoned units
            // ; until the mana upkeep matches or is less than the
            // ; wizard's current mana
            // ; inherits multiple BUGs from the subfunctions
        }

        _players[itr].mana_reserve = (_players[itr].mana_reserve - mana_upkeeps[itr]);

        if(_players[itr].mana_reserve < 0)
        {
            _players[itr].mana_reserve = 0;
        }

    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Players_Apply_Upkeeps()\n", __FILE__, __LINE__);
#endif

}


// WZD o140p07
// drake178: WIZ_MatchFoodUpkeep()
// WIZ_MatchFoodUpkeep()

// WZD o140p08
// drake178: WIZ_MatchGoldUpkeep()
// WIZ_MatchGoldUpkeep()

// WZD o140p09
// drake178: WIZ_MatchManaUpkeep()
// WIZ_MatchManaUpkeep()

// WZD o140p10
// drake178: WIZ_DisbandSummons()
// WIZ_DisbandSummons()

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
/*
    Outpost loss
    Outpost graduation  (NOT growth)
    City population growth
    City size increase
    ¿ City size 1 Pop_10s default 5 ?
    City size decrease
    Pestilence City size decrease


*/
void CTY_ProgressTurn(void)
{
    int16_t Surplus_Farmers;
    int16_t New_Min_Farmers;
    int16_t Population_Growth;
    int16_t itr;

    for(itr = 0; itr < _cities; itr++)
    {
        if(_CITIES[itr].population == 0)
        {
            if(_CITIES[itr].Pop_10s <= 0)
            {
                if((_CITIES[itr].owner_idx == HUMAN_PLAYER_IDX) && (MSG_CityLost_Count < 20))
                {
                    // TODO  String_Copy_Far(&MSG_CityLost_Names[(MSG_CityLost_Count], _CITIES[itr].name);
                    strcpy(&MSG_CityLost_Names[(MSG_CityLost_Count * 20)], _CITIES[itr].name);
                    MSG_CityLost_Count++;
                }
                // TODO  CTY_Remove(itr);
            }

            if(_CITIES[itr].Pop_10s >= 10)
            {
                _CITIES[itr].population = 1;
                _CITIES[itr].size = 1;  /* CTY_Hamlet */
                _CITIES[itr].farmer_count = City_Minimum_Farmers(itr);
                Do_City_Calculations(itr);
                if((_CITIES[itr].owner_idx == HUMAN_PLAYER_IDX) && (MSG_CityGained_Count < 20))
                {
                    MSG_CityGained_Array[MSG_CityGained_Count] = itr;
                    MSG_CityGained_Count++;
                }
            }
        }
        else
        {
            Population_Growth = City_Growth_Rate(itr);
            _CITIES[itr].Pop_10s += Population_Growth;

            if((_CITIES[itr].Pop_10s >= 100) && (_CITIES[itr].population < 25))
            {
                // TODO  grow city, add message
            }

            if(_CITIES[itr].Pop_10s < 0)
            {
                if(_CITIES[itr].population <= 1)  /* cant be 0 in this branch, so must be == 1 or <= -1 */
                {
                    _CITIES[itr].Pop_10s = 5;
                }
                else
                {
                    // TODO  shrink city, add message
                }
            }

            if(_CITIES[itr].enchantments[PESTILENCE] > 0)
            {
                if(_CITIES[itr].population > Random(10))
                {
                    // TODO  shrink city, add message
                }
            }
            
            CTY_ProdProgress(itr);

        }


        if(_CITIES[itr].enchantments[CONSECRATION] > 0)
        {
            // TODO  CTY_Consecration(itr);
        }
        if(_CITIES[itr].enchantments[STREAM_OF_LIFE] > 0)
        {
            // TODO  CTY_StreamOfLife(itr);
        }
        if(_CITIES[itr].enchantments[CHAOS_RIFT] > 0)
        {
            // TODO  CTY_ChaosRift(itr);
        }
        if(_CITIES[itr].enchantments[GAIAS_BLESSING] > 0)
        {
            // TODO  CTY_GaiasBlessing(itr);
        }
        if(_CITIES[itr].enchantments[NIGHTSHADE] > 0)
        {
            // TODO  CTY_NightshadeDispel(itr);
        }
    }

    // TODO  TILE_CountVolcanoes()

}


// WZD o140p17
// drake178: WIZ_SkillFromHeroes()
int16_t Player_Hero_Casting_Skill(int16_t player_idx)
{
    int16_t heroes_spell_casting_skill_points;
    int16_t half_hero_spell_casting_skill_points;

    int16_t itr_heroes;  // _DI_

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Player_Hero_Casting_Skill()\n", __FILE__, __LINE__);
#endif

    half_hero_spell_casting_skill_points = 0;
    heroes_spell_casting_skill_points = 0;

    for(itr_heroes = 0; itr_heroes < NUM_HEROES; itr_heroes++)
    {
        if(
            (_players[player_idx].Heroes[itr_heroes].Unit_Index > -1) &&
            (_UNITS[_players[player_idx].Heroes[itr_heroes].Unit_Index].wx == _FORTRESSES[player_idx].wx) &&
            (_UNITS[_players[player_idx].Heroes[itr_heroes].Unit_Index].wy == _FORTRESSES[player_idx].wy) &&
            (_UNITS[_players[player_idx].Heroes[itr_heroes].Unit_Index].wp == _FORTRESSES[player_idx].wp)
        )
        {
            UNIT_Create_BURecord(_players[player_idx].Heroes[itr_heroes].Unit_Index, global_strategic_unit);

            half_hero_spell_casting_skill_points = (global_strategic_unit->mana_max / 2);

            heroes_spell_casting_skill_points += half_hero_spell_casting_skill_points;
        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Player_Hero_Casting_Skill()\n", __FILE__, __LINE__);
#endif

    return heroes_spell_casting_skill_points;
}


// WZD o140p18
// drake178: N/A
// IDK_SplCst_SplSkl_sC5AB1()

// WZD o140p19
// drake178: EVNT_RandomOffers()
// EVNT_RandomOffers()

// WZD o140p20
// drake178: WIZ_ResearchProgress()
// WIZ_ResearchProgress()

// WZD o140p21
// drake178: WIZ_LearnSpell()
// WIZ_LearnSpell()

// WZD o140p22
// drake178: G_UNIT_OvlHealing()
// G_UNIT_OvlHealing()

// WZD o140p23
// drake178: N/A
// IDK_Unit_Heal_sC6572()

// WZD o140p24
// drake178: WIZ_ProcessGlobals()
// WIZ_ProcessGlobals()

// WZD o140p25
// drake178: N/A
// IDK_Unit_XP_sC6BCF()

// WZD o140p26
// drake178: ¿ ?
// Cool_Off_Volcanoes()

// WZD o140p27
// drake178: AI_CullTheWeak()
// AI_CullTheWeak()

// WZD o140p28
// drake178: AI_GetAvgUnitCosts()
// AI_GetAvgUnitCosts()
