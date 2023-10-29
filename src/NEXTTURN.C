/*
    Next Turn Procedure

    WIZARDS.EXE
        ovr060
        ovr119
        ovr121
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



    /*
        END: Messages
    */



// @@Done_WrapItUp
    current_screen = scr_Main_Screen;

    // TODO  MSG_BuildDone_Count = 0;

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



// push    [word ptr RNG_AI_Turn_Seed+2]   ; LFSR_HI
// push    [word ptr RNG_AI_Turn_Seed]     ; LFSR_LO
// call    Set_Random_Seed                 ; sets the linear feedback shift register to the
//                                         ; specified state


// mov     ax, offset mouse_list_hourglass
// push    ax                              ; list
// mov     ax, 1
// push    ax                              ; count
// call    Set_Mouse_List


// call    j_CTY_RecalculateAll            ; calls CTY_Recalculate for all cities
//                                         ; (with all its BUGs)

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

// call    j_MSG_Clear                     ; zeroes the counters of all chancellor (scroll)
//                                         ; message types, clearing all event messages

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

// loc_9EC56:                              ; decreases the peace counters for all wizards towards
// call    j_WIZ_DecreasePeaceCs           ; all others

// call    j_WIZ_GoldIncomes               ; calculate and apply each wizard's gold income from
//                                         ; heroes, cities, and excess food, also including
//                                         ; maintenance costs paid for town buildings
//                                         ; INCONSISTENT: allows gold up to 32000

// call    j_WIZ_SetPowerBases             ; zeroes and recalculates the power base for every
//                                         ; wizard, although cities are not reset, but instead
//                                         ; will use the power income they have defined already

// call    j_WIZ_PowerIncomes              ; apply the power incomes of each wizard to their
//                                         ; current research, mana reserves, and casting skill
//                                         ; INCONSISTENT: allows mana up to 32000

// call    j_WIZ_ResearchProgress          ; checks whether any of the wizards has finished their
//                                         ; current research, and if so, allows picking a new one
//                                         ; if possible, showing the new spell animation for the
//                                         ; human player followed by the research dialog
//                                         ; WARNING: can temporarily store a negative spell index
//                                         ; as the player's researched spell

// call    j_OVL_DisableIncmBlink          ; sets OVL_BlinkNegIncomes to -1, which causes negative
//                                         ; incomes to be drawn with a static color instead



// cmp     [DBG_Alt_A_State], e_ST_FALSE
// jnz     short loc_9EC87
// cmp     [magic_set.Random_Events], e_ST_TRUE
// jnz     short loc_9EC87

// call    j_EVNT_GenerateRandom           ; attempts to generate a random event
//                                         ; has some BUGs that need review


// loc_9EC87:                              ; progresses any ongoing events (status 1 or 2)
// call    j_EVNT_Progress                 ;
//                                         ; has several BUGs relating to specific events

// call    j_WIZ_ProcessUpkeep             ; processes food, gold, and mana upkeep for all
//                                         ; wizards, and for the human player also attempts to
//                                         ; remove sources of upkeep if they can not be paid for
//                                         ; BUG: AI players can ignore negative income
//                                         ; also inherits a severe BUG from UNIT_GetDependants

// call    EMM_Map_DataH                   ; maps the EMM Data block into the page frame

// call    j_CTY_OutpostGrowth             ; calculates and records the growth and/or shrinkage
//                                         ; of all outposts, although turning into a full city
//                                         ; is not in this function
//                                         ; BUG: this function ignores Wild Games altogether,
//                                         ; both as a source of food and as a terrain special
//                                         ; BUG: difficulty-based outpost growth modifiers are
//                                         ; applied to both AI and human player outposts

// call    j_CTY_ProgressTurn              ; processes population growth, production, and
//                                         ; persistent city enchantment effects for all cities
//                                         ; contains a host of BUGs, both own and inherited

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

// mov     ax, offset mouse_list_hourglass
// push    ax                              ; list
// mov     ax, 1
// push    ax                              ; count
// call    Set_Mouse_List

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

// mov     ax, offset mouse_list_hourglass
// push    ax                              ; list
// mov     ax, 1
// push    ax                              ; count
// call    Set_Mouse_List

// call    j_DIPL_AI_To_AI                 ; many BUGs and INCONSISTENCIES inside
//                                         ; RE-EXPLORE in more context!

// mov     ax, offset mouse_list_hourglass
// push    ax                              ; list
// mov     ax, 1
// push    ax                              ; count
// call    Set_Mouse_List

// call    j_IDK_Dipl_s7373B

// call    j_IDK_Dipl_s73FBF

// mov     ax, offset mouse_list_hourglass
// push    ax                              ; list
// mov     ax, 1
// push    ax                              ; count
// call    Set_Mouse_List

// call    j_IDK_Dipl_s_7436F

// call    j_IDK_Dipl_s74B68

// call    j_Cool_Off_Volcanoes

// jmp     short loc_9EDF3

    }

// loc_9EDF3:
// call    j_IDK_SplCst_SplSkl_sC5AB1

// call    j_GAME_DeleteDeadUnits          ; deletes all removed units from the unit table,
//                                         ; updating the indices of all hired heroes accordingly

// call    j_Set_Unit_Draw_Priority        ; sets the draw priority field of each unit record
//                                         ; based on attack strength, transport capability, and
//                                         ; visibility (in the case of AI units)

// push    [_map_plane]                    ; Plane
// push    [_map_y]                        ; TopY
// push    [_map_x]                        ; LeftX
// call    j_Set_Entities_On_Map_Window    ; fills out the OVL_UnitsOnMap array with the unit or
//                                         ; city shown on each of the map tiles visible in the
//                                         ; 12 by 10 map window of the main overland display

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


// call    j_OVL_EnableIncmBlink           ; sets OVL_BlinkNegIncomes to 0, enabling negative
//                                         ; incomes to be drawn with their default blinking color

// call    j_GAME_AutoSave                 ; if the current turn is divisible by 4, saves the game
//                                         ; into slot index 8 (SAVE9.GAM), the "continue" save
//                                         ; that can not be loaded from the save/load screen, but
//                                         ; is started automatically by wizards.exe

// call    j_CTY_RecalculateAll            ; calls CTY_Recalculate for all cities
//                                         ; (with all its BUGs)

// call    Get_Random_Seed                 ; returns the current state of the linear feedback
//                                         ; shift register in dx:ax
// mov     [word ptr RNG_AI_Turn_Seed+2], dx
// mov     [word ptr RNG_AI_Turn_Seed], ax

// mov     ax, offset mouse_list_default
// push    ax                              ; list
// mov     ax, 1
// push    ax                              ; count
// call    Set_Mouse_List



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
// WZD o121p02
// WZD o121p03
// WZD o121p04
// WZD o121p05
// WZD o121p06
// WZD o121p07


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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: UNIT_GetHalfMoves(unit_idx = %d)\n", __FILE__, __LINE__, unit_idx);
#endif

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



#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: UNIT_GetHalfMoves(unit_idx = %d) { movement_points = %d }\n", __FILE__, __LINE__, unit_idx, movement_points);
#endif

    return movement_points;
}


// WZD o121p09

// WZD o121p10
// WZD o121p11
// WZD o121p12
// WZD o121p13
// WZD o121p14
