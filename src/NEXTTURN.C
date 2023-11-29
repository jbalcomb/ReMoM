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


// WZD o121p02
// AKAK Calc_Nominal_Skill()
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

// WZD o121p10
// WZD o121p11
// WZD o121p12


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




/*
    WIZARDS.EXE  ovr140
*/

// WZD s140p01
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


// WZD s140p02
// WZD s140p03

// WZD s140p04
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


// WZD s140p06
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


// WZD s140p07
// WZD s140p08
// WZD s140p09
// WZD s140p10
// WZD s140p11
// WZD s140p12
// WZD s140p13
// WZD s140p14
// WZD s140p15
// WZD s140p16


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
            (_UNITS[_players[player_idx].Heroes[itr_heroes].Unit_Index].world_x == _FORTRESSES[player_idx].world_x) &&
            (_UNITS[_players[player_idx].Heroes[itr_heroes].Unit_Index].world_y == _FORTRESSES[player_idx].world_y) &&
            (_UNITS[_players[player_idx].Heroes[itr_heroes].Unit_Index].world_plane == _FORTRESSES[player_idx].world_plane)
        )
        {
            UNIT_Create_BURecord(_players[player_idx].Heroes[itr_heroes].Unit_Index, Active_Unit);

            half_hero_spell_casting_skill_points = (Active_Unit->mana_max / 2);

            heroes_spell_casting_skill_points += half_hero_spell_casting_skill_points;
        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Player_Hero_Casting_Skill()\n", __FILE__, __LINE__);
#endif

    return heroes_spell_casting_skill_points;
}


// WZD s140p18
// WZD s140p19
// WZD s140p20
// WZD s140p21
// WZD s140p22
// WZD s140p23
// WZD s140p24
// WZD s140p25
// WZD s140p26
// WZD s140p27
// WZD s140p28
// Cool_Off_Volcanoes
