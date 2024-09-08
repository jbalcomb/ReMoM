/*
    Events

    WIZARDS.EXE
        ovr080
        ovr081
*/

#include "MoX.H"



// WZD dseg:0154                                                 BEGIN:  ovr080 - Initialized Data

// WZD dseg:0154
int16_t EVNT_Enabled = ST_TRUE;

// WZD dseg:0154                                                 END:  ovr080 - Initialized Data



// WZD dseg:4A0C                                                 BEGIN:  ovr080 - Initialized Data

// WZD dseg:4A0C
// drake178: EVNT_TargetTypes
// MoO2  Module: EVENTS  data (0 bytes) _event_good_array Address: 02:00178E84
/*
Good Event / Bad Event
Event - Good/Bad
{0: bad, 1: good}
*/
int16_t m_event_good_array[NUM_EVENT_TYPES] =
{
    ST_TRUE,   /* et_None        */
    ST_FALSE,  /* et_Meteor      */
    ST_TRUE,   /* et_Gift        */
    ST_TRUE,   /* et_Disjunction */
    ST_TRUE,   /* et_Marriage    */
    ST_FALSE,  /* et_Earthquake  */
    ST_FALSE,  /* et_Pirates     */
    ST_FALSE,  /* et_Plague      */
    ST_FALSE,  /* et_Rebellion   */
    ST_TRUE,   /* et_Donation    */
    ST_FALSE,  /* et_Depletion   */
    ST_TRUE,   /* et_New_Mine    */
    ST_TRUE,   /* et_Population_Boom    */
    ST_TRUE,   /* et_Good_Moon   */
    ST_TRUE,   /* et_Bad_Moon    */
    ST_TRUE,   /* et_Con_Chaos   */
    ST_TRUE,   /* et_Con_Nature  */
    ST_TRUE,   /* et_Con_Sorc    */
    ST_FALSE   /* et_Mana_Short   */
};

// WZD dseg:4A32
int16_t _event_delay_count = 0;

// WZD dseg:4A32                                                 END:  ovr080 - Initialized Data



// WZD dseg:4A34                                                 BEGIN:  ovr081 - Initialized Data

// WZD dseg:4A34 48 4A 49 4A 50 4A 56 4A 5E 4A 64 4A 69 4A 72 4A+TBL_EVNT_OreNames@ dw offset cnst_ZeroString, offset cnst_EVNT_Mineral_1, offset cnst_EVNT_Mineral_2, offset cnst_EVNT_Mineral_3, offset cnst_EVNT_Mineral_4, offset cnst_EVNT_Mineral_5, offset cnst_EVNT_Mineral_6, offset cnst_EVNT_Mineral_7, offset cnst_EVNT_Mineral_8, offset cnst_EVNT_Mineral_9
// WZD dseg:4A34 7F 4A 86 4A                                                                             ; DATA XREF: EVNT_ComposeMessage+112r
// WZD dseg:4A34                                                                                         ; "n iron"
// WZD dseg:4A48 00                                              cnst_ZeroString db 0                    ; DATA XREF: dseg:TBL_EVNT_OreNames@o
// WZD dseg:4A49 6E 20 69 72 6F 6E 00                            cnst_EVNT_Mineral_1 db 'n iron',0       ; DATA XREF: dseg:TBL_EVNT_OreNames@o
// WZD dseg:4A50 20 63 6F 61 6C 00                               cnst_EVNT_Mineral_2 db ' coal',0        ; DATA XREF: dseg:TBL_EVNT_OreNames@o
// WZD dseg:4A56 20 73 69 6C 76 65 72 00                         cnst_EVNT_Mineral_3 db ' silver',0      ; DATA XREF: dseg:TBL_EVNT_OreNames@o
// WZD dseg:4A5E 20 67 6F 6C 64 00                               cnst_EVNT_Mineral_4 db ' gold',0        ; DATA XREF: dseg:TBL_EVNT_OreNames@o
// WZD dseg:4A64 20 67 65 6D 00                                  cnst_EVNT_Mineral_5 db ' gem',0         ; DATA XREF: dseg:TBL_EVNT_OreNames@o
// WZD dseg:4A69 20 6D 69 74 68 72 69 6C 00                      cnst_EVNT_Mineral_6 db ' mithril',0     ; DATA XREF: dseg:TBL_EVNT_OreNames@o
// WZD dseg:4A72 6E 20 61 64 61 6D 61 6E 74 69 75 6D 00          cnst_EVNT_Mineral_7 db 'n adamantium',0 ; DATA XREF: dseg:TBL_EVNT_OreNames@o
// WZD dseg:4A7F 20 71 75 6F 72 6B 00                            cnst_EVNT_Mineral_8 db ' quork',0       ; DATA XREF: dseg:TBL_EVNT_OreNames@o
// WZD dseg:4A86 20 63 72 79 73 78 00                            cnst_EVNT_Mineral_9 db ' crysx',0       ; DATA XREF: dseg:TBL_EVNT_OreNames@o
// WZD dseg:4A8D 4D 55 53 49 43 00                               cnst_MUSIC_File4 db 'MUSIC',0           ; DATA XREF: EVNT_ShowMessage+D4o
// WZD dseg:4A93 45 56 45 4E 54 53 00                            cnst_EVENTS_File db 'EVENTS',0          ; DATA XREF: EVNT_ShowMessage+F5o
// WZD dseg:4A9A 45 56 45 4E 54 4D 53 47 00                      cnst_EVENTMSG_File db 'EVENTMSG',0      ; DATA XREF: EVNT_ComposeMessage:loc_6C6D8o
// WZD dseg:4AA3 6E 6F 00                                        cnst_EventMsg_1 db 'no',0               ; DATA XREF: EVNT_ComposeMessage+16Bo ...
// WZD dseg:4AA6 20 74 68 6F 75 73 61 6E 64 00                   cnst_EventMsg_2 db ' thousand',0        ; DATA XREF: EVNT_ComposeMessage+1BCo
// WZD dseg:4AB0 73 00                                           cnst_EventMsg_3 db 's',0                ; DATA XREF: EVNT_ComposeMessage+247o ...

// WZD dseg:4AB0                                                 END:  ovr081 - Initialized Data



// WZD dseg:C2D8                                                 BEGIN:  ovr080 - Uninitialized Data

// WZD dseg:C2D8 00 00                                           EVNT_LostPopulation dw 0                ; DATA XREF: EVNT_Progress+EAw ...
// WZD dseg:C2DA 00 00                                           EVNT_LostUnitCount dw 0                 ; DATA XREF: EVNT_Progress+108w ...
// WZD dseg:C2DC 00 00                                           EVNT_DestroyedBldngs dw 0               ; DATA XREF: EVNT_Progress+CFw ...
// WZD dseg:C2DE 00 00                                           EVNT_TargetPlayer dw 0                  ; DATA XREF: EVNT_Progress+77w ...
// WZD dseg:C2E0 00 00                                           CRP_EVNT_Var_2 dw 0                     ; DATA XREF: EVNT_Progress+5Dw ...
// WZD dseg:C2E2 00 00                                           EVNT_MsgDataValue dw 0                  ; DATA XREF: EVNT_Progress+57w ...
// WZD dseg:C2E4 00 00                                           EVNT_TargetCity dw 0                    ; DATA XREF: EVNT_Progress+71w ...

// WZD dseg:C2E6
// drake178: EVNT_Type
// MoO2  Module: EVENTS  _current_event
int16_t m_current_event;

// WZD dseg:C2E8 00 00                                           EVNT_MessageIndex dw 0                  ; DATA XREF: EVNT_Progress+83w ...
// WZD dseg:C2EA 00 00                                           CRP_EVNT_Var_1 dw 0                     ; DATA XREF: EVNT_Progress+8w

// WZD dseg:C2EA                                                 END:  ovr080 - Uninitialized Data



// WZD dseg:C2EC                                                 BEGIN:  ovr081 - Uninitialized Data

// WZD dseg:C2EC 00 00                                           EVNT_LoadMsgBuffer@ dw 0                ; DATA XREF: EVNT_ShowMessage+7Aw ...

// WZD dseg:C2EE 00 00                                           EVNT_Message@ dw 0                      ; DATA XREF: EVNT_ShowMessage+6Dw ...

// WZD dseg:C2EE                                                 END:  ovr081 - Uninitialized Data



/*
    WIZARDS.EXE  ovr080
*/

// WZD s080p01
/*
; attempts to generate a random event
; has some BUGs that need review
*/
/*

*/
void Determine_Event(void)
{
    int16_t troops[MAX_STACK];
    int16_t Conjunction_Active;
    int16_t Normal_Units;
    int16_t Summoned_Units;
    int16_t troop_count;
    int16_t Mineral_Type;
    int16_t Mineral_Plane;
    int16_t marriage_city_idx;
    int16_t city_idx;
    int16_t Mineral_Y;
    int16_t Mineral_X;
    int16_t Unused_Diff_Var;
    int16_t itr_troops;
    int16_t Tries;
    int16_t Event_Chance;
    int16_t event_type;  // _SI_
    int16_t player_idx;  // _DI_

    // ¿ clear events_table data ?
    if(EVNT_Enabled == ST_FALSE)
    {
        events_table->Meteor_Status = 3;

        events_table->Gift_Status = 3;

        events_table->Disjunction_Status = 3;

        events_table->Marriage_Status = 3;

        events_table->Earthquake_Status = 3;

        events_table->Pirates_Status = 3;

        events_table->Plague_Status = 3;
        events_table->Plague_Duration = 0;

        events_table->Rebellion_Status = 3;

        events_table->Donation_Status = 3;

        events_table->Depletion_Status = 3;

        events_table->New_Mine_Status = 3;

        events_table->Population_Boom_Status = 3;
        events_table->Population_Boom_Duration = 0;

        events_table->Good_Moon_Status = 3;
        events_table->Good_Moon_Duration = 0;

        events_table->Bad_Moon_Status = 3;
        events_table->Bad_Moon_Duration = 0;

        events_table->Conjunction_Chaos_Status = 3;
        events_table->Conjunction_Chaos_Duration = 0;

        events_table->Conjunction_Nature_Status = 3;
        events_table->Conjunction_Nature_Duration = 0;

        events_table->Conjunction_Sorcery_Status = 3;
        events_table->Conjunction_Sorcery_Duration = 0;

        events_table->Mana_Short_Status = 3;
        events_table->Mana_Short_Duration = 0;

        return;
    }


    if(events_table->last_event_turn > _turn)
    {
        Event_Chance = 0;
    }
    else
    {
        Event_Chance = (_turn - events_table->last_event_turn);
        SETMIN(Event_Chance, 0);
    }

    // ; BUG: events can not be processed in the first 5 turns after launching the game
    if(_event_delay_count < 5)
    {
        _event_delay_count++;
        Event_Chance = 0;
    }

    switch(_difficulty)
    {
        case god_Intro:
        {
            Event_Chance = ((Event_Chance * 1) / 2);  // 50%
        } break;
        case god_Easy:
        {
            Event_Chance = ((Event_Chance * 2) / 3);  // 66%

        } break;
        case god_Normal:
        {
            Event_Chance = ((Event_Chance * 3) / 4);  // 75%

        } break;
        case god_Hard:
        {
            Event_Chance = ((Event_Chance * 4) / 5);  // 80%
        } break;
        // ~ Impossible is 100%
    }

    if(Random(512) > Event_Chance)
    {
        return;
    }

    event_type = ST_UNDEFINED;

    for(Tries = 0; ((Tries < 5) && (event_type == ST_UNDEFINED)); Tries++)
    {
        event_type = Random(18);


        player_idx = Get_Event_Victim(m_event_good_array[event_type]);

        if(player_idx == ST_UNDEFINED)
        {
            event_type = ST_UNDEFINED;
        }

        if(_FORTRESSES[player_idx].active == ST_FALSE)
        {
            event_type = ST_UNDEFINED;
        }

        /* et_None */
        if((event_type == et_Meteor              ) && ((events_table->Meteor_Status               == 3) || (events_table->Meteor_Status               == 2))) { event_type = ST_UNDEFINED; } /*  1 */
        if((event_type == et_Gift                ) && ((events_table->Gift_Status                 == 3) || (events_table->Gift_Status                 == 2))) { event_type = ST_UNDEFINED; } /*  2 */
        if((event_type == et_Disjunction         ) && ((events_table->Disjunction_Status          == 3) || (events_table->Disjunction_Status          == 2))) { event_type = ST_UNDEFINED; } /*  3 */
        if((event_type == et_Marriage            ) && ((events_table->Marriage_Status             == 3) || (events_table->Marriage_Status             == 2))) { event_type = ST_UNDEFINED; } /*  4 */
        if((event_type == et_Earthquake          ) && ((events_table->Earthquake_Status           == 3) || (events_table->Earthquake_Status           == 2))) { event_type = ST_UNDEFINED; } /*  5 */
        if((event_type == et_Pirates             ) && ((events_table->Pirates_Status              == 3) || (events_table->Pirates_Status              == 2))) { event_type = ST_UNDEFINED; } /*  6 */
        if((event_type == et_Plague              ) && ((events_table->Plague_Status               == 3) || (events_table->Plague_Status               == 2))) { event_type = ST_UNDEFINED; } /*  7 */
        if((event_type == et_Donation            ) && ((events_table->Donation_Status             == 3) || (events_table->Donation_Status             == 2))) { event_type = ST_UNDEFINED; } /*  9 OoO */
        if((event_type == et_Rebellion           ) && ((events_table->Rebellion_Status            == 3) || (events_table->Rebellion_Status            == 2))) { event_type = ST_UNDEFINED; } /*  8 OoO */
        if((event_type == et_Depletion           ) && ((events_table->Depletion_Status            == 3) || (events_table->Depletion_Status            == 2))) { event_type = ST_UNDEFINED; } /* 10 */
        if((event_type == et_New_Mine            ) && ((events_table->New_Mine_Status             == 3) || (events_table->New_Mine_Status             == 2))) { event_type = ST_UNDEFINED; } /* 11 */
        if((event_type == et_Population_Boom     ) && ((events_table->Population_Boom_Status      == 3) || (events_table->Population_Boom_Status      == 2))) { event_type = ST_UNDEFINED; } /* 12 */
        if((event_type == et_Good_Moon           ) && ((events_table->Good_Moon_Status            == 3) || (events_table->Good_Moon_Status            == 2))) { event_type = ST_UNDEFINED; } /* 13 */
        if((event_type == et_Bad_Moon            ) && ((events_table->Bad_Moon_Status             == 3) || (events_table->Bad_Moon_Status             == 2))) { event_type = ST_UNDEFINED; } /* 14 */
        if((event_type == et_Conjunction_Chaos   ) && ((events_table->Conjunction_Chaos_Status    == 3) || (events_table->Conjunction_Chaos_Status    == 2))) { event_type = ST_UNDEFINED; } /* 15 */
        if((event_type == et_Conjunction_Nature  ) && ((events_table->Conjunction_Nature_Status   == 3) || (events_table->Conjunction_Nature_Status   == 2))) { event_type = ST_UNDEFINED; } /* 16 */
        if((event_type == et_Conjunction_Sorcery ) && ((events_table->Conjunction_Sorcery_Status  == 3) || (events_table->Conjunction_Sorcery_Status  == 2))) { event_type = ST_UNDEFINED; } /* 17 */
        if((event_type == et_Mana_Short          ) && ((events_table->Mana_Short_Status           == 3) || (events_table->Mana_Short_Status           == 2))) { event_type = ST_UNDEFINED; } /* 18 */
        
        Conjunction_Active = 0;

        if(events_table->Good_Moon_Status            == 2) { Conjunction_Active++; }
        if(events_table->Bad_Moon_Status             == 2) { Conjunction_Active++; }
        if(events_table->Conjunction_Chaos_Status    == 2) { Conjunction_Active++; }
        if(events_table->Conjunction_Nature_Status   == 2) { Conjunction_Active++; }
        if(events_table->Conjunction_Sorcery_Status  == 2) { Conjunction_Active++; }
        if(events_table->Mana_Short_Status           == 2) { Conjunction_Active++; }

        if(Conjunction_Active >= 1)
        {
            if(
                (event_type == et_Good_Moon)
                ||
                (event_type == et_Bad_Moon)
                ||
                (event_type == et_Conjunction_Chaos)
                ||
                (event_type == et_Conjunction_Nature)
                ||
                (event_type == et_Conjunction_Sorcery)
                ||
                (event_type == et_Mana_Short)
            )
            {
                event_type = ST_UNDEFINED;
            }
        }

        if(
            (event_type == et_Pirates)
            &&
            (_players[player_idx].gold_reserve < 100)
        )
        {
            event_type = ST_UNDEFINED;
        }

        if(
            (event_type == et_Meteor)
            ||
            (event_type == et_Marriage)
            ||
            (event_type == et_Earthquake)
            ||
            (event_type == et_Plague)
            ||
            (event_type == et_Rebellion)
            ||
            (event_type == et_Population_Boom)
        )
        {

            city_idx = Pick_Random_City(player_idx);

            if(city_idx == ST_UNDEFINED)
            {
                event_type = ST_UNDEFINED;
            }

            if(event_type == et_Marriage)
            {
                marriage_city_idx = Pick_Random_City(NEUTRAL_PLAYER_IDX);

                if(marriage_city_idx == ST_UNDEFINED)
                {
                    event_type = ST_UNDEFINED;
                }

            }

        }

        if(
            (event_type == et_Marriage)
            &&
            (_turn < 150)
        )
        {
            event_type = ST_UNDEFINED;
        }

        if(
            (event_type == et_Meteor)
            &&
            (_turn < 150)
        )
        {
            event_type = ST_UNDEFINED;
        }

        if(event_type == et_Rebellion)
        {

            Summoned_Units = 0;

            Normal_Units = 0;

            if(Player_Fortress_City(player_idx) == city_idx)
            {
                event_type = ST_UNDEFINED;
            }
            else
            {

                Army_At_City(city_idx, &troop_count, &troops[0]);

                for(itr_troops = 0; ((itr_troops < troop_count) && (event_type == et_Rebellion)); itr_troops++)
                {
                    if(_unit_type_table[_UNITS[troops[itr_troops]].type].Race < rt_Arcane)
                    {
                        Normal_Units++;
                    }
                    else
                    {
                        Summoned_Units++;
                    }

                    if(_UNITS[troops[itr_troops]].Hero_Slot > -1)
                    {
                        event_type = ST_UNDEFINED;
                    }

                }

                if(Summoned_Units > Normal_Units)
                {
                    event_type = ST_UNDEFINED;
                }

            }

        }

        if(event_type == et_Depletion)
        {
            if(EVNT_TargetDepletion__STUB(player_idx, &Mineral_X, &Mineral_Y, &Mineral_Plane, &Mineral_Type) == ST_UNDEFINED)
            {
                event_type = ST_UNDEFINED;
            }
        }

        if(event_type == et_New_Mine)
        {
            if(EVNT_FindNewMineral__STUB(player_idx, &Mineral_X, &Mineral_Y, &Mineral_Plane, &Mineral_Type) == ST_UNDEFINED)
            {
                event_type = ST_UNDEFINED;
            }
        }

        if(event_type == et_Disjunction)
        {
            if(Any_Overland_Enchantments() == ST_FALSE)
            {
                event_type = ST_UNDEFINED;
            }
        }

        if((event_type == et_Good_Moon) && (events_table->Bad_Moon_Status  == 2)) { event_type = ST_UNDEFINED; }
        if((event_type == et_Bad_Moon ) && (events_table->Good_Moon_Status == 2)) { event_type = ST_UNDEFINED; }

        if((event_type == et_Plague         ) && (events_table->Population_Boom_Status  == 2) && (events_table->Population_Boom_Data == city_idx)) { event_type = ST_UNDEFINED; }
        if((event_type == et_Population_Boom) && (events_table->Plague_Status           == 2) && (events_table->Plague_Data          == city_idx)) { event_type = ST_UNDEFINED; }

    }

    if(event_type == ST_UNDEFINED)
    {
        return;
    }

    events_table->last_event_turn = _turn;

    _event_delay_count = 0;

    switch(_difficulty)
    {
        case god_Intro:
        {
            Unused_Diff_Var = (_turn / 20);
        } break;
        case god_Easy:
        {
            Unused_Diff_Var = (_turn / 18);

        } break;
        case god_Normal:
        {
            Unused_Diff_Var = (_turn / 15);

        } break;
        case god_Hard:
        {
            Unused_Diff_Var = (_turn / 13);

        } break;
        case god_Impossible:
        {
            Unused_Diff_Var = (_turn / 10);

        } break;
    }

    switch(event_type)
    {
        case et_Meteor:
        {
            events_table->Meteor_Status = 1;
            events_table->Meteor_Data = city_idx;
            events_table->Meteor_Player = player_idx;
        } break;
        case et_Gift:
        {
            events_table->Gift_Status = 1;
            events_table->Gift_Player = player_idx;
            events_table->Gift_Data = Make_Item(2, _players[player_idx].spellranks, 0);
        } break;
        case et_Disjunction:
        {
            events_table->Disjunction_Status = 1;
        } break;
        case et_Marriage:
        {
            events_table->Marriage_Status = 1;
            events_table->marriage_neutral_city_idx = marriage_city_idx;
            events_table->marriage_player_city_idx = city_idx;
            events_table->Marriage_Player = player_idx;
        } break;
        case et_Earthquake:
        {
            events_table->Earthquake_Status = 1;
            events_table->Earthquake_Player = player_idx;
            events_table->Earthquake_Data = city_idx;
        } break;
        case et_Pirates:
        {
            events_table->Pirates_Status = 1;
            events_table->Pirates_Player = player_idx;
            Conjunction_Active = ((_players[player_idx].gold_reserve * 100) / (29 + Random(21)));
            Conjunction_Active -= (Conjunction_Active % 10);
            events_table->Pirates_Data = Conjunction_Active;
        } break;
        case et_Plague:
        {
            events_table->Plague_Status = 1;
            events_table->Plague_Data = city_idx;
            events_table->Plague_Player = player_idx;
        } break;
        case et_Rebellion:
        {
            events_table->Rebellion_Status = 1;
            events_table->Rebellion_Data = city_idx;
            events_table->Rebellion_Player = player_idx;
        } break;
        case et_Donation:
        {
            events_table->Donation_Status = 1;
            events_table->Donation_Player = player_idx;
            events_table->Donation_Data = (100 + (Random(100) * 5));
        } break;
        case et_Depletion:
        {
            events_table->Depletion_Status = 1;
            events_table->Depletion_Player = player_idx;
        } break;
        case et_New_Mine:
        {
            events_table->New_Mine_Status = 1;
            events_table->New_Mine_Player = player_idx;
        } break;
        case et_Population_Boom:
        {
            events_table->Population_Boom_Status = 1;
            events_table->Population_Boom_Data = city_idx;
            events_table->Population_Boom_Player = player_idx;
        } break;
        case et_Good_Moon:
        {
            events_table->Good_Moon_Status = 1;
        } break;
        case et_Bad_Moon:
        {
            events_table->Bad_Moon_Status = 1;
        } break;
        case et_Conjunction_Chaos:
        {
            events_table->Conjunction_Chaos_Status = 1;
        } break;
        case et_Conjunction_Nature:
        {
            events_table->Conjunction_Nature_Status = 1;
        } break;
        case et_Conjunction_Sorcery:
        {
            events_table->Conjunction_Sorcery_Status = 1;
        } break;
        case et_Mana_Short:
        {
            events_table->Mana_Short_Status = 1;
        } break;

    }

}


// WZD s080p02
// EVNT_Progress()

// WZD s080p03
// drake178: WIZ_HaveAnyGlobals()
/*
    returns {F,T} - any player has an overland enchantment in effect
*/
int16_t Any_Overland_Enchantments(void)
{
    uint8_t * ptr_player_globals;
    int16_t itr_players;  // _SI_
    int16_t itr_globals;  // _CX_
    int16_t any_overland_enchantment;  // _DI_

    any_overland_enchantment = ST_FALSE;

    for(itr_players = 0; ((itr_players < _num_players) && (any_overland_enchantment == ST_FALSE)); itr_players++)
    {
        ptr_player_globals = (uint8_t *)&_players[itr_players].Globals;

        for(itr_globals = 0; ((itr_globals < NUM_OVERLAND_ENCHANTMENTS) && (any_overland_enchantment == ST_FALSE)); itr_globals++)
        {
            if(ptr_player_globals[itr_globals] > 0)
            {
                any_overland_enchantment = ST_TRUE;
            }
        }
    }

    return any_overland_enchantment;
}



/*
    WIZARDS.EXE  ovr081
*/

// WZD s081p01
// EVNT_ShowMessage()

// WZD s081p02
// EVNT_ComposeMessage()

// WZD s081p03
// EVNT_SelectTarget()
// MoO2  Module: EVENTS  Get_Event_Victim_()
/*
; selects a target for an event using a weighted roll
; based on either the strength of the remaining wizards
; (bad events), or the inverse of that (good events)
; returns the selected target, or -1 if none qualify
*/
/*

*/
int16_t Get_Event_Victim(int16_t event_type)
{
	// TODO
	return ST_UNDEFINED;
}


// WZD s081p04
// drake178: EVNT_PickCity()
// MoO2  Module: Events  ~ Pick_Random_Colony_No_Outpost_()  || Pick_Random_Colony_No_Capitol_()
/*
; picks a random city of the selected player that has
; a racial unrest modifier of 1 or less (i.e. won't
; choose rebellious cities, only good ones)
; returns the index of the city, or -1 if none qualify
*/
/*

*/
int16_t Pick_Random_City(int16_t player_idx)
{
	// TODO
	return ST_UNDEFINED;
}
