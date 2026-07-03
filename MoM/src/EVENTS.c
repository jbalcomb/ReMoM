/*
    Events

    WIZARDS.EXE
        ovr080
        ovr081
*/

#include "../../STU/src/STU_LOG.h"

#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/Fields.h"
#include "../../MoX/src/Fonts.h"
#include "../../MoX/src/GENDRAW.h"
#include "../../MoX/src/Graphics.h"
#include "../../MoX/src/LBX_Load.h"
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_DAT.h"  /* _screen_seg */
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_SET.h"  /* magic_set */
#include "../../MoX/src/MOX_T4.h"
#include "../../MoX/src/random.h"
#include "../../MoX/src/SOUND.h"

#include "City_ovr55.h"
#include "CITYCALC.h"
#include "EVENTS.h"
#include "ItemMake.h"
#include "Items.h"
#include "ItemScrn.h"
#include "MainScr.h"
#include "MainScr_Maps.h"
#include "MOM_DBG.h"
#include "RACETYPE.h"
#include "SBookScr.h"
#include "Spells129.h"
#include "UNITTYPE.h"   // WTFMATE
#include "WZD_o059.h"
#include "WZD_o143.h"

#include <stdlib.h>
#include <string.h>

#include "../../ext/stu_compat.h"



// WZD dseg:0154                                                 BEGIN:  ovr080 - Initialized Data

// WZD dseg:0154
/*
hard-coded to true, never set; only tested in Determine_Event() to disable random events;
The real events gate is in Next_Turn_Calc() - only calls Determine_Event() when magic_set.random_events == ST_TRUE.
DEDU  ¿ deprecated debug code ?  ¿ is in MoO1 ?
*/
int16_t g_random_events_enabled = ST_TRUE;

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

/*
    DEDU:  Out-Of-Order!!  How to make this make sense? String-Optimizer!?!
*/
// WZD dseg:4A48
char cnst_ZeroString[] = "";
// WZD dseg:4A49
char cnst_EVNT_Mineral_1[] = "n iron";
// WZD dseg:4A50
char cnst_EVNT_Mineral_2[] = " coal";
// WZD dseg:4A56
char cnst_EVNT_Mineral_3[] = " silver";
// WZD dseg:4A5E
char cnst_EVNT_Mineral_4[] = " gold";
// WZD dseg:4A64
char cnst_EVNT_Mineral_5[] = " gem";
// WZD dseg:4A69
char cnst_EVNT_Mineral_6[] = " mithril";
// WZD dseg:4A72
char cnst_EVNT_Mineral_7[] = "n adamantium";
// WZD dseg:4A7F
char cnst_EVNT_Mineral_8[] = " quork";
// WZD dseg:4A86
char cnst_EVNT_Mineral_9[] = " crysx";

// WZD dseg:4A34
char * TBL_EVNT_OreNames[] =
{
    cnst_ZeroString, 
    cnst_EVNT_Mineral_1, 
    cnst_EVNT_Mineral_2, 
    cnst_EVNT_Mineral_3, 
    cnst_EVNT_Mineral_4, 
    cnst_EVNT_Mineral_5, 
    cnst_EVNT_Mineral_6, 
    cnst_EVNT_Mineral_7, 
    cnst_EVNT_Mineral_8, 
    cnst_EVNT_Mineral_9
};

// WZD dseg:4A8D
char music_lbx_file__ovr081[] = "MUSIC";

// WZD dseg:4A93
char events_lbx_file__ovr081[] = "EVENTS";

// WZD dseg:4A9A
char eventmsg_lbx_file__ovr081[] = "EVENTMSG";

// WZD dseg:4AA3
char cnst_EventMsg_1[] = "no";
// WZD dseg:4AA6
char cnst_EventMsg_2[] = " thousand";
// WZD dseg:4AB0
char cnst_EventMsg_3[] = "s";

// WZD dseg:4AB0                                                 END:  ovr081 - Initialized Data



// WZD dseg:C2D8                                                 BEGIN:  ovr080 - Uninitialized Data

// WZD dseg:C2D8
int16_t EVNT_LostPopulation;

// WZD dseg:C2DA
int16_t EVNT_LostUnitCount;

// WZD dseg:C2DC
int16_t EVNT_DestroyedBldngs;

// WZD dseg:C2DE
// drake178: EVNT_TargetPlayer
int16_t m_event_player_idx;

// WZD dseg:C2E0
int16_t niu_event_twiddle_type_flag;

// WZD dseg:C2E2
int16_t EVNT_MsgDataValue;

// WZD dseg:C2E4
// drake178: EVNT_TargetCity
int16_t m_event_city_idx;

// WZD dseg:C2E6
// drake178: EVNT_Type
// MoO2  Module: EVENTS  _current_event
int16_t m_current_event;

// WZD dseg:C2E8
// drake178: EVNT_MessageIndex
int16_t m_event_message_type;

// WZD dseg:C2EA
int16_t niu_event_twiddle_flag;

// WZD dseg:C2EA                                                 END:  ovr080 - Uninitialized Data



// WZD dseg:C2EC                                                 BEGIN:  ovr081 - Uninitialized Data

// WZD dseg:C2EC
SAMB_ptr m_event_message_record;

// WZD dseg:C2EE
// drake178: EVNT_Message
char * m_event_message;

// WZD dseg:C2EE                                                 END:  ovr081 - Uninitialized Data



/*
    WIZARDS.EXE  ovr080
*/

// WZD s080p01
void Determine_Event(void)
{
    int16_t troops[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t conjunction_active = 0;
    int16_t normal_units = 0;
    int16_t summoned_units = 0;
    int16_t troop_count = 0;
    int16_t terrain_special = 0;
    int16_t wp = 0;
    int16_t marriage_city_idx = 0;
    int16_t city_idx = 0;
    int16_t wy = 0;
    int16_t wx = 0;
    int16_t niu_escalation_scalar = 0;
    int16_t itr_troops = 0;
    int16_t tries = 0;
    int16_t event_pressure_accumulator = 0;
    int16_t event_type = 0;
    int16_t player_idx = 0;
    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-ENTER] name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);
    if(g_random_events_enabled == ST_FALSE)
    {
        events_table->Meteor_Status = 3;
        events_table->Gift_Status = 3;
        events_table->Disjunction_Status = 3;
        events_table->marriage_status = 3;
        events_table->Earthquake_Status = 3;
        events_table->Pirates_Status = 3;
        events_table->Plague_Status = 3;
        events_table->Plague_Duration = 0;
        events_table->Rebellion_Status = 3;
        events_table->Donation_Status = 3;
        events_table->Depletion_Status = 3;
        events_table->minerals_status = 3;
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
        goto Done;
    }
    if(events_table->last_event_turn > _turn)
    {
        event_pressure_accumulator = 0;
    }
    else
    {
        event_pressure_accumulator = (_turn - events_table->last_event_turn);
        SETMIN(event_pressure_accumulator, 0);
    }
    /* OGBUG  events can not be processed in the first 5 turns after launching the game, should be _turn based for 'Load Game' */
    if(_event_delay_count < 5)
    {
        _event_delay_count++;
        event_pressure_accumulator = 0;
    }
    /* scale chance by difficulty */
    switch(_difficulty)
    {
        case god_Intro:  { event_pressure_accumulator = ((event_pressure_accumulator * 1) / 2); } break;  /* 50% */
        case god_Easy:   { event_pressure_accumulator = ((event_pressure_accumulator * 2) / 3); } break;  /* 66% */
        case god_Normal: { event_pressure_accumulator = ((event_pressure_accumulator * 3) / 4); } break;  /* 75% */
        case god_Hard:   { event_pressure_accumulator = ((event_pressure_accumulator * 4) / 5); } break;  /* 80% */
        /* Impossible is 100% */
    }
    
    /* DEBUG HACK */  if(DBG_trigger_event == ST_TRUE) { event_pressure_accumulator = 512; }  /* force it to fire off an event */

    /* fire test  (¿ power of two avoids modulo bias ?)*/
    if(Random(512) > event_pressure_accumulator)
    {
        goto Done;
    }
    /* pick a candidate event, then run the veto gauntlet; up to 5 tries */
    event_type = ST_UNDEFINED;
    for(tries = 0; ((tries < 5) && (event_type == ST_UNDEFINED)); tries++)
    {
        event_type = Random(18);

        /* pick a victim appropriate to the event's good/bad alignment */
        if(event_type > ST_UNDEFINED)
        {
            player_idx = Get_Event_Victim(m_event_good_array[event_type]);
        }

        /* DEBUG HACK */  if(DBG_trigger_event == ST_TRUE) { player_idx = HUMAN_PLAYER_IDX; }

        if(player_idx == ST_UNDEFINED)
        {
            event_type = ST_UNDEFINED;
        }

        if(_FORTRESSES[player_idx].active == ST_FALSE)
        {
            event_type = ST_UNDEFINED;
        }

        /* veto any event that is already active (2) or parked/disabled (3) */
        /* et_None */
        if((event_type == et_Meteor              ) && ((events_table->Meteor_Status               == 3) || (events_table->Meteor_Status               == 2))) { event_type = ST_UNDEFINED; } /*  1 */
        if((event_type == et_Gift                ) && ((events_table->Gift_Status                 == 3) || (events_table->Gift_Status                 == 2))) { event_type = ST_UNDEFINED; } /*  2 */
        if((event_type == et_Disjunction         ) && ((events_table->Disjunction_Status          == 3) || (events_table->Disjunction_Status          == 2))) { event_type = ST_UNDEFINED; } /*  3 */
        if((event_type == et_Marriage            ) && ((events_table->marriage_status             == 3) || (events_table->marriage_status             == 2))) { event_type = ST_UNDEFINED; } /*  4 */
        if((event_type == et_Earthquake          ) && ((events_table->Earthquake_Status           == 3) || (events_table->Earthquake_Status           == 2))) { event_type = ST_UNDEFINED; } /*  5 */
        if((event_type == et_Pirates             ) && ((events_table->Pirates_Status              == 3) || (events_table->Pirates_Status              == 2))) { event_type = ST_UNDEFINED; } /*  6 */
        if((event_type == et_Plague              ) && ((events_table->Plague_Status               == 3) || (events_table->Plague_Status               == 2))) { event_type = ST_UNDEFINED; } /*  7 */
        if((event_type == et_Donation            ) && ((events_table->Donation_Status             == 3) || (events_table->Donation_Status             == 2))) { event_type = ST_UNDEFINED; } /*  9 OoO */
        if((event_type == et_Rebellion           ) && ((events_table->Rebellion_Status            == 3) || (events_table->Rebellion_Status            == 2))) { event_type = ST_UNDEFINED; } /*  8 OoO */
        if((event_type == et_Depletion           ) && ((events_table->Depletion_Status            == 3) || (events_table->Depletion_Status            == 2))) { event_type = ST_UNDEFINED; } /* 10 */
        if((event_type == et_New_Mine            ) && ((events_table->minerals_status             == 3) || (events_table->minerals_status             == 2))) { event_type = ST_UNDEFINED; } /* 11 */
        if((event_type == et_Population_Boom     ) && ((events_table->Population_Boom_Status      == 3) || (events_table->Population_Boom_Status      == 2))) { event_type = ST_UNDEFINED; } /* 12 */
        if((event_type == et_Good_Moon           ) && ((events_table->Good_Moon_Status            == 3) || (events_table->Good_Moon_Status            == 2))) { event_type = ST_UNDEFINED; } /* 13 */
        if((event_type == et_Bad_Moon            ) && ((events_table->Bad_Moon_Status             == 3) || (events_table->Bad_Moon_Status             == 2))) { event_type = ST_UNDEFINED; } /* 14 */
        if((event_type == et_Conjunction_Chaos   ) && ((events_table->Conjunction_Chaos_Status    == 3) || (events_table->Conjunction_Chaos_Status    == 2))) { event_type = ST_UNDEFINED; } /* 15 */
        if((event_type == et_Conjunction_Nature  ) && ((events_table->Conjunction_Nature_Status   == 3) || (events_table->Conjunction_Nature_Status   == 2))) { event_type = ST_UNDEFINED; } /* 16 */
        if((event_type == et_Conjunction_Sorcery ) && ((events_table->Conjunction_Sorcery_Status  == 3) || (events_table->Conjunction_Sorcery_Status  == 2))) { event_type = ST_UNDEFINED; } /* 17 */
        if((event_type == et_Mana_Short          ) && ((events_table->Mana_Short_Status           == 3) || (events_table->Mana_Short_Status           == 2))) { event_type = ST_UNDEFINED; } /* 18 */
        
        conjunction_active = ST_FALSE;
        if(events_table->Good_Moon_Status            == 2) { conjunction_active++; }
        if(events_table->Bad_Moon_Status             == 2) { conjunction_active++; }
        if(events_table->Conjunction_Chaos_Status    == 2) { conjunction_active++; }
        if(events_table->Conjunction_Nature_Status   == 2) { conjunction_active++; }
        if(events_table->Conjunction_Sorcery_Status  == 2) { conjunction_active++; }
        if(events_table->Mana_Short_Status           == 2) { conjunction_active++; }
        if(conjunction_active >= ST_TRUE)
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
            summoned_units = 0;
            normal_units = 0;
            if(Player_Fortress_City(player_idx) == city_idx)
            {
                event_type = ST_UNDEFINED;
            }
            else
            {
                Army_At_City(city_idx, &troop_count, &troops[0]);
                for(itr_troops = 0; ((itr_troops < troop_count) && (event_type == et_Rebellion)); itr_troops++)
                {
                    if(_unit_type_table[_UNITS[troops[itr_troops]].type].race_type < rt_Arcane)
                    {
                        normal_units++;
                    }
                    else
                    {
                        summoned_units++;
                    }
                    if(_UNITS[troops[itr_troops]].Hero_Slot > -1)
                    {
                        event_type = ST_UNDEFINED;
                    }
                }
                if(summoned_units > normal_units)
                {
                    event_type = ST_UNDEFINED;
                }
            }
        }

        if(event_type == et_Depletion)
        {
            if(EVNT_TargetDepletion__STUB(player_idx, &wx, &wy, &wp, &terrain_special) == ST_UNDEFINED)
            {
                event_type = ST_UNDEFINED;
            }
        }

        if(event_type == et_New_Mine)
        {
            if(EVNT_FindNewMineral__STUB(player_idx, &wx, &wy, &wp, &terrain_special) == ST_UNDEFINED)
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
        goto Done;
    }

    events_table->last_event_turn = _turn;
    _event_delay_count = 0;
    switch(_difficulty)
    {
        case god_Intro:      { niu_escalation_scalar = (_turn / 20); } break;
        case god_Easy:       { niu_escalation_scalar = (_turn / 18); } break;
        case god_Normal:     { niu_escalation_scalar = (_turn / 15); } break;
        case god_Hard:       { niu_escalation_scalar = (_turn / 13); } break;
        case god_Impossible: { niu_escalation_scalar = (_turn / 10); } break;
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
            events_table->marriage_status = 1;
            events_table->marriage_neutral_city_idx = marriage_city_idx;
            events_table->marriage_player_city_idx = city_idx;
            events_table->marriage_player_idx = player_idx;
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
            conjunction_active = ((_players[player_idx].gold_reserve * 100) / (29 + Random(21)));
            conjunction_active -= (conjunction_active % 10);
            events_table->Pirates_Data = conjunction_active;
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
            events_table->minerals_status = 1;
            events_table->minerals_player = player_idx;
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
Done:
    LOG_TRACE(LOG_CAT_CALL_TRACE, "[FN-EXIT]  name=%s rng_call=%llu", __func__, (unsigned long long)g_random_call_count);
}


// WZD s080p02
// MoO2  Module: Events  Event_Twiddle_()
/**
 * @brief Advances and resolves all currently active world events for the turn.
 *
 * @details
 * Processes each event slot in @c events_table that is in @c es_Starting or
 * @c es_Ongoing state, applies event-specific gameplay effects, updates event
 * state and duration counters, and triggers player-facing messages when
 * appropriate.
 *
 * Processing responsibilities include:
 * - One-shot events that immediately resolve and clear state
 *   (e.g., Meteor, Gift, Disjunction, Marriage, Earthquake, Pirates,
 *   Donation, Depletion, New Mine).
 * - Ongoing timed events that can continue or terminate probabilistically
 *   (e.g., Plague, Population Boom, Good Moon, Bad Moon,
 *   Conjunctions, Mana Short).
 * - Mutating shared event message context globals
 *   (@c m_current_event, @c m_event_player_idx, @c m_event_city_idx,
 *   @c m_event_message_type, @c EVNT_MsgDataValue, etc.) used by
 *   @c Show_Event_Message().
 * - Applying world/game-state side effects such as population changes,
 *   city ownership transitions, enchantment/global cleanup, gold changes,
 *   terrain-special edits, and item rewards.
 *
 * @return This function returns no value.
 *
 * @note Operates heavily via global state and in-place mutation of
 *       @c events_table, @c _CITIES, @c _players, and unit tables.
 * @note Message visibility and formatting are delegated to
 *       @c Show_Event_Message() and @c Get_Event_Message().
 * @note Event sub-blocks are evaluated sequentially in a single call, so
 *       multiple events can be progressed during one turn tick.
 *
 * @see Determine_Event(), Show_Event_Message(), Get_Event_Message()
 */
void Event_Twiddle(void)
{
    int16_t item_list[18] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t troops[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t item_count = 0;
    int16_t troop_count = 0;
    int16_t city_population = 0;
    int16_t terrain_special = 0;
    int16_t wp = 0;
    int16_t wy = 0;
    int16_t wx = 0;
    int16_t post_event_troop_count = 0;
    uint8_t * ptr_players_globals = 0;
    int16_t player_idx = 0;
    int16_t itr_globals = 0;
    int16_t itr_players = 0;
    int16_t city_idx = 0;

    if(DBG_trigger_event == ST_TRUE) { DBG_trigger_event = ST_FALSE; }

    niu_event_twiddle_flag = 0;

    /*
        BEGIN: Meteor
    */
    if((events_table->Meteor_Status == es_Starting) || (events_table->Meteor_Status == es_Ongoing))
    {
        if(_CITIES[events_table->Meteor_Data].owner_idx != events_table->Meteor_Player)
        {
            events_table->Meteor_Status = es_Nothing;
        }
        else
        {
            player_idx = events_table->Meteor_Player;
            EVNT_MsgDataValue = 0;
            niu_event_twiddle_type_flag = 0;
            m_current_event = et_Meteor;
            m_event_city_idx = events_table->Meteor_Data;
            m_event_player_idx = player_idx;
            if(player_idx == _human_player_idx)
            {
                m_event_message_type = 0;
            }
            else
            {
                m_event_message_type = 4;
            }
            events_table->Meteor_Status = es_Nothing;
            Army_At_City(m_event_city_idx, &troop_count, &troops[0]);
            city_population = _CITIES[m_event_city_idx].population;
            EVNT_DestroyedBldngs = Apply_Call_The_Void(m_event_city_idx);
            EVNT_LostPopulation = (city_population - _CITIES[m_event_city_idx].population);
            Army_At_City(m_event_city_idx, &post_event_troop_count, &troops[0]);
            EVNT_LostUnitCount = (troop_count - post_event_troop_count);
            Show_Event_Message();
        }
    }
    /*
        END: Meteor
    */

    /*
        BEGIN: Gift
    */
    if((events_table->Gift_Status == es_Starting) || (events_table->Gift_Status == es_Ongoing))
    {
        EVNT_MsgDataValue = events_table->Gift_Data;
        niu_event_twiddle_type_flag = 0;
        m_current_event = et_Gift;
        m_event_city_idx = 0;
        events_table->Gift_Player = m_event_player_idx;
        events_table->Gift_Status = es_Nothing;
        if(events_table->Gift_Player == _human_player_idx)
        {
            m_event_message_type = 0;
        }
        else
        {
            m_event_message_type = 4;
        }
        Show_Event_Message();
        if(m_event_message_type == 0)
        {
            Allocate_Reduced_Map();
            Clear_Palette_Changes(0, 255);
            Set_Palette_Changes(0, 223);
            Update_Remap_Gray_Palette();
            Set_Page_Off();
            Main_Screen_Draw();
            DARKEN_SCREEN();
            PageFlip_FX();
            Copy_On_To_Off_Page();
            troops[0] = events_table->Gift_Data;
            Process_Item_Pool(1, &troops[0]);
        }
    }
    /*
        END: Gift
    */

    /*
        BEGIN: Disjunction
    */
    if((events_table->Disjunction_Status == es_Starting) || (events_table->Disjunction_Status == es_Ongoing))
    {
        EVNT_MsgDataValue = 0;
        niu_event_twiddle_type_flag = 0;
        m_current_event = et_Disjunction;
        m_event_city_idx = 0;
        m_event_player_idx = 99;
        m_event_message_type = 3;
        events_table->Disjunction_Status = es_Nothing;
        for(itr_players = 0; itr_players < NUM_PLAYERS; itr_players++)
        {
            ptr_players_globals = (uint8_t *)&_players[itr_players].Globals;
            for(itr_globals = 0; itr_globals < NUM_OVERLAND_ENCHANTMENTS; itr_globals++)
            {
                if(ptr_players_globals[itr_globals] > 0)
                {
                    ptr_players_globals[itr_globals] = 0;
                }
            }
        }
        Show_Event_Message();
    }
    /*
        END: Disjunction
    */

    /*
        BEGIN: Marriage
    */
    if((events_table->marriage_status == es_Starting) || (events_table->marriage_status == es_Ongoing))
    {
        player_idx = events_table->marriage_player_idx;
        m_event_city_idx = events_table->marriage_neutral_city_idx;
        EVNT_MsgDataValue = events_table->marriage_neutral_city_idx;
        m_current_event = et_Marriage;
        m_event_player_idx = events_table->marriage_player_idx;
        if(events_table->marriage_player_idx == _human_player_idx)
        {
            m_event_message_type = 0;
        }
        else
        {
            m_event_message_type = 4;
        }
        city_idx = events_table->marriage_neutral_city_idx;
        Army_At_Square_2(CITYX(), CITYY(), CITYP(), &post_event_troop_count, &troops[0]);
        for(itr_players = 0; itr_players < post_event_troop_count; itr_players++)
        {
            if(_UNITS[troops[itr_players]].owner_idx != ST_UNDEFINED)
            {
                _UNITS[troops[itr_players]].owner_idx = (int8_t)player_idx;
            }
        }
        _CITIES[city_idx].owner_idx = (int8_t)player_idx;
        _CITIES[city_idx].construction = bt_TradeGoods;
        _CITIES[city_idx].Prod_Accu = 0;
        _CITIES[city_idx].sold_building = ST_FALSE;
        if(player_idx == _human_player_idx)  /* ¿ current player ? */
        {
            Set_Map_Square_Explored_Flags_XYP_Range(CITYX(), CITYY(), CITYP(), 3);
        }
        NOOP_Current_Player_All_City_Areas();
        Do_City_Calculations(city_idx);
        Reset_City_Area_Bitfields();
        Reset_City_Road_Connection_Bitfields();
        Show_Event_Message();
        events_table->marriage_status = es_Nothing;
    }
    /*
        END: Marriage
    */

    /*
        BEGIN: Earthquake
    */
    if((events_table->Earthquake_Status == es_Starting) || (events_table->Earthquake_Status == es_Ongoing))
    {
        player_idx = events_table->Earthquake_Player;
        m_current_event = et_Earthquake;
        m_event_player_idx = events_table->Earthquake_Player;
        EVNT_MsgDataValue = 0;
        m_event_city_idx = events_table->Earthquake_Data;
        events_table->Earthquake_Status = es_Nothing;
        if(events_table->Earthquake_Player == _human_player_idx)
        {
            m_event_message_type = 0;
        }
        else
        {
            m_event_message_type = 4;
        }
        Army_At_City(m_event_city_idx, &troop_count, &troops[0]);
        city_population = _CITIES[m_event_city_idx].population;
        EVNT_DestroyedBldngs = Apply_Earthquake(m_event_city_idx, &item_count, &item_list[0]);
        EVNT_LostPopulation = (city_population - _CITIES[m_event_city_idx].population);
        Army_At_City(m_event_city_idx, &post_event_troop_count, &troops[0]);
        EVNT_LostUnitCount = (troop_count - post_event_troop_count);
        if(
            (EVNT_LostUnitCount != 0)
            ||
            (EVNT_LostPopulation != 0)
            ||
            (EVNT_DestroyedBldngs != 0)
        )
        {
            Show_Event_Message();
            Player_Process_Item_Pool(_CITIES[m_event_city_idx].owner_idx, item_count, &item_list[0]);
        }

    }
    /*
        END: Earthquake
    */

    /*
        BEGIN: Pirates
    */
    if((events_table->Pirates_Status == es_Starting) || (events_table->Pirates_Status == es_Ongoing))
    {
        player_idx = events_table->Pirates_Player;
        m_current_event = et_Pirates;
        m_event_player_idx = events_table->Pirates_Player;
        EVNT_MsgDataValue = events_table->Pirates_Data;
        events_table->Pirates_Status = es_Nothing;
        if(events_table->Pirates_Player == _human_player_idx)
        {
            m_event_message_type = 0;
        }
        else
        {
            m_event_message_type = 4;
        }
        Show_Event_Message();
        _players[player_idx].gold_reserve -= events_table->Pirates_Data;
        SETMIN(_players[player_idx].gold_reserve, 0);
    }
    /*
        END: Pirates
    */

    /*
        BEGIN: Plague
    */
    if((events_table->Plague_Status == es_Starting) || (events_table->Plague_Status == es_Ongoing))
    {
        player_idx = events_table->Plague_Player;
        m_current_event = et_Plague;
        m_event_city_idx = events_table->Plague_Data;
        m_event_player_idx = events_table->Plague_Player;
        if(events_table->Plague_Player == _human_player_idx)
        {
            m_event_message_type = 0;
        }
        else
        {
            m_event_message_type = 4;
        }
        if(Random(10) < _CITIES[events_table->Plague_Data].population)
        {
            if(_CITIES[events_table->Plague_Data].population > 2)
            {
                _CITIES[events_table->Plague_Data].population -= 1;
            }
        }
        if(events_table->Plague_Status == es_Starting)
        {
            Show_Event_Message();
            events_table->Plague_Status = es_Ongoing;
            events_table->Plague_Duration = 0;
        }
        else
        {
            if(Random(20) == 1)
            {
                if(events_table->Plague_Duration > 3)
                {
                    events_table->Plague_Status = es_Nothing;
                }
            }
            events_table->Plague_Duration += 1;
            if(_CITIES[events_table->Plague_Data].population < 2)
            {
                events_table->Plague_Status = es_Nothing;
                _CITIES[events_table->Plague_Data].population = 2;
            }
            if(player_idx == _human_player_idx)
            {
                if(events_table->Plague_Status == es_Nothing)
                {
                    m_event_message_type = emt_Ended;
                    Show_Event_Message();
                }
            }
        }
    }
    /*
        END: Plague
    */

    /*
        BEGIN: Rebellion
    */
    if((events_table->Rebellion_Status == es_Starting) || (events_table->Rebellion_Status == es_Ongoing))
    {
        player_idx = events_table->Rebellion_Player;
        m_event_city_idx = events_table->Rebellion_Data;
        m_event_player_idx = events_table->Rebellion_Player;
        m_current_event = et_Rebellion;
        events_table->Rebellion_Status = es_Nothing;
        if(events_table->Rebellion_Player == _human_player_idx)
        {
            m_event_message_type = emt_Started;
        }
        else
        {
            m_event_message_type = emt_Elsewhere;
        }
        Army_At_City(m_event_city_idx, &post_event_troop_count, &troops[0]);
        for(itr_players = 0; itr_players < post_event_troop_count; itr_players++)
        {
            if(_unit_type_table[_UNITS[troops[itr_players]].type].race_type < rt_Arcane)
            {
                _UNITS[troops[itr_players]].owner_idx = NEUTRAL_PLAYER_IDX;
            }
            else
            {
                Dismiss_Unit(troops[itr_players]);
            }
        }
        Change_City_Ownership(m_event_city_idx, NEUTRAL_PLAYER_IDX);
        Show_Event_Message();
    }
    /*
        END: Rebellion
    */

    /*
        BEGIN: Donation
    */
    if((events_table->Donation_Status == es_Starting) || (events_table->Donation_Status == es_Ongoing))
    {
        player_idx = events_table->Donation_Player;
        m_current_event = et_Donation;
        m_event_player_idx = events_table->Donation_Player;
        EVNT_MsgDataValue = events_table->Donation_Data;
        if(events_table->Donation_Player == _human_player_idx)
        {
            m_event_message_type = 0;
        }
        else
        {
            m_event_message_type = 4;
        }
        events_table->Donation_Status = es_Nothing;
        _players[player_idx].gold_reserve += EVNT_MsgDataValue;
        Show_Event_Message();
    }
    /*
        END: Donation
    */

    /*
        BEGIN: Depletion
    */
    if((events_table->Depletion_Status == es_Starting) || (events_table->Depletion_Status == es_Ongoing))
    {
        player_idx = events_table->Depletion_Player;
        m_current_event = et_Depletion;
        m_event_player_idx = events_table->Depletion_Player;
        /* TODO */  itr_players = EVNT_TargetDepletion__STUB(player_idx, &wx, &wy, &wp, &terrain_special);
        events_table->Depletion_Data = itr_players;
        m_event_city_idx = events_table->Depletion_Data;
        Clear_Terrain_Specials(wx, wy, wp);
        EVNT_MsgDataValue = terrain_special;
        if(events_table->Depletion_Player == _human_player_idx)
        {
            m_event_message_type = 0;
        }
        else
        {
            m_event_message_type = 4;
        }
        events_table->Depletion_Status = es_Nothing;
        Show_Event_Message();
    }
    /*
        END: Depletion
    */

    /*
        BEGIN: Minerals
    */
    if((events_table->minerals_status == es_Starting) || (events_table->minerals_status == es_Ongoing))
    {
        player_idx = events_table->minerals_player;
        m_current_event = et_New_Mine;
        m_event_player_idx = events_table->minerals_player;
        /* TODO */  itr_players = EVNT_FindNewMineral__STUB(player_idx, &wx, &wy, &wp, &terrain_special);
        events_table->minerals_data = itr_players;
        m_event_city_idx = events_table->minerals_data;
        /* TODO */  TILE_ReplaceMinerals__STUB(wx, wy, wp, terrain_special);
        EVNT_MsgDataValue = terrain_special;
        if(events_table->minerals_player == _human_player_idx)
        {
            m_event_message_type = 0;
        }
        else
        {
            m_event_message_type = 4;
        }
        events_table->minerals_status = es_Nothing;
        Show_Event_Message();
    }
    /*
        END: Minerals
    */

    /*
        BEGIN: Population Boom
    */
    if((events_table->Population_Boom_Status == es_Starting) || (events_table->Population_Boom_Status == es_Ongoing))
    {
        player_idx = events_table->Population_Boom_Player;
        m_current_event = et_Population_Boom;
        m_event_player_idx = events_table->Population_Boom_Player;
        m_event_city_idx = events_table->Population_Boom_Data;
        if(events_table->Population_Boom_Player == _human_player_idx)
        {
            m_event_message_type = 0;
        }
        else
        {
            m_event_message_type = 4;
        }
        if(events_table->Population_Boom_Status == es_Starting)
        {
            Show_Event_Message();
            events_table->Population_Boom_Status = es_Ongoing;
            events_table->Population_Boom_Duration = 0;
        }
        else
        {
            if(Random(20) == 1)
            {
                if(events_table->Population_Boom_Duration > 3)
                {
                    events_table->Population_Boom_Status = es_Nothing;
                    if(player_idx == _human_player_idx)
                    {
                        m_event_message_type = 1;
                        Show_Event_Message();
                    }
                }
            }
            events_table->Population_Boom_Duration += 1;
        }
    }
    /*
        END: Population Boom
    */

    /*
        BEGIN: Good Moon
    */
    if((events_table->Good_Moon_Status == es_Starting) || (events_table->Good_Moon_Status == es_Ongoing))
    {
        m_current_event = et_Good_Moon;
        m_event_player_idx = 99;
        m_event_message_type = emt_Happened;
        if(events_table->Good_Moon_Status == es_Starting)
        {
            Show_Event_Message();
            events_table->Good_Moon_Status = es_Ongoing;
            events_table->Good_Moon_Duration = 0;
        }
        else
        {
            if(Random(20) <= (events_table->Good_Moon_Duration - 2))
            {
                if(events_table->Good_Moon_Duration > 3)
                {
                    events_table->Good_Moon_Status = es_Nothing;
                    m_event_message_type = emt_Ended;
                    Show_Event_Message();
                }
            }
            events_table->Good_Moon_Duration += 1;
        }
    }
    /*
        END: Good Moon
    */

    /*
        BEGIN: Bad Moon
    */
    if((events_table->Bad_Moon_Status == es_Starting) || (events_table->Bad_Moon_Status == es_Ongoing))
    {
        m_current_event = et_Bad_Moon;
        m_event_player_idx = 99;
        m_event_message_type = emt_Happened;
        if(events_table->Bad_Moon_Status == es_Starting)
        {
            Show_Event_Message();
            events_table->Bad_Moon_Status = es_Ongoing;
            events_table->Bad_Moon_Duration = 0;
        }
        else
        {
            if(Random(20) <= (events_table->Bad_Moon_Duration - 2))
            {
                if(events_table->Bad_Moon_Duration > 3)
                {
                    events_table->Bad_Moon_Status = es_Nothing;
                    m_event_message_type = emt_Ended;
                    Show_Event_Message();
                }
            }
            events_table->Bad_Moon_Duration += 1;
        }
    }
    /*
        END: Bad Moon
    */

    /*
        BEGIN: Conjunction - Chaos
    */
    if((events_table->Conjunction_Chaos_Status == es_Starting) || (events_table->Conjunction_Chaos_Status == es_Ongoing))
    {
        m_current_event = et_Conjunction_Chaos;
        m_event_player_idx = 99;
        m_event_message_type = emt_Happened;
        if(events_table->Conjunction_Chaos_Status == es_Starting)
        {
            Show_Event_Message();
            events_table->Conjunction_Chaos_Status = es_Ongoing;
            events_table->Conjunction_Chaos_Duration = 0;
        }
        else
        {
            if(Random(20) <= (events_table->Conjunction_Chaos_Duration - 2))
            {
                if(events_table->Conjunction_Chaos_Duration > 3)
                {
                    events_table->Conjunction_Chaos_Status = es_Nothing;
                    m_event_message_type = emt_Ended;
                    Show_Event_Message();
                }
            }
            events_table->Conjunction_Chaos_Duration += 1;
        }
    }
    /*
        END: Conjunction - Chaos
    */

    /*
        BEGIN: Conjunction - Nature
    */
    if((events_table->Conjunction_Nature_Status == es_Starting) || (events_table->Conjunction_Nature_Status == es_Ongoing))
    {
        m_current_event = et_Conjunction_Nature;
        m_event_player_idx = 99;
        m_event_message_type = emt_Happened;
        if(events_table->Conjunction_Nature_Status == es_Starting)
        {
            Show_Event_Message();
            events_table->Conjunction_Nature_Status = es_Ongoing;
            events_table->Conjunction_Nature_Duration = 0;
        }
        else
        {
            if(Random(20) <= (events_table->Conjunction_Nature_Duration - 2))
            {
                if(events_table->Conjunction_Nature_Duration > 3)
                {
                    events_table->Conjunction_Nature_Status = es_Nothing;
                    m_event_message_type = emt_Ended;
                    Show_Event_Message();
                }
            }
            events_table->Conjunction_Nature_Duration += 1;
        }
    }
    /*
        END: Conjunction - Nature
    */

    /*
        BEGIN: Conjunction - Sorcery
    */
    if((events_table->Conjunction_Sorcery_Status == es_Starting) || (events_table->Conjunction_Sorcery_Status == es_Ongoing))
    {
        m_current_event = et_Conjunction_Sorcery;
        m_event_player_idx = 99;
        m_event_message_type = emt_Happened;
        if(events_table->Conjunction_Sorcery_Status == es_Starting)
        {
            Show_Event_Message();
            events_table->Conjunction_Sorcery_Status = es_Ongoing;
            events_table->Conjunction_Sorcery_Duration = 0;
        }
        else
        {
            if(Random(20) <= (events_table->Conjunction_Sorcery_Duration - 2))
            {
                if(events_table->Conjunction_Sorcery_Duration > 3)
                {
                    events_table->Conjunction_Sorcery_Status = es_Nothing;
                    m_event_message_type = emt_Ended;
                    Show_Event_Message();
                }
            }
            events_table->Conjunction_Sorcery_Duration += 1;
        }
    }
    /*
        END: Conjunction - Sorcery
    */

    /*
        BEGIN: Mana Short
    */
    if((events_table->Mana_Short_Status == es_Starting) || (events_table->Mana_Short_Status == es_Ongoing))
    {
        m_current_event = et_Mana_Short;
        m_event_player_idx = 99;
        m_event_message_type = emt_Happened;
        if(events_table->Mana_Short_Status == es_Starting)
        {
            Show_Event_Message();
            events_table->Mana_Short_Status = es_Ongoing;
            events_table->Mana_Short_Duration = 0;
        }
        else
        {
            if(Random(20) <= (events_table->Mana_Short_Duration - 2))
            {
                if(events_table->Mana_Short_Duration > 3)
                {
                    events_table->Mana_Short_Status = es_Nothing;
                    m_event_message_type = emt_Ended;
                    Show_Event_Message();
                }
            }
            events_table->Mana_Short_Duration += 1;
        }
    }
    /*
        END: Mana Short
    */

}


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
// drake178: EVNT_ShowMessage()
void Show_Event_Message(void)
{
    SAMB_ptr sound_seg;
    SAMB_ptr m_event_display_seg;
    int16_t event_music_num;  // _SI_
    uint32_t sound_seg_size = 0;  // DNE in Dasm

    if(
        (m_event_player_idx != 99)
        &&
        (m_event_player_idx != _human_player_idx)
        &&
        (_players[_human_player_idx].Dipl.Contacted[m_event_player_idx] != 1)
    )
    {
        return;
    }

    Stop_All_Sounds__STUB();

    Allocate_Reduced_Map();

    Set_Draw_Active_Stack_Always();

    Set_Unit_Draw_Priority();

    Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);

    Full_Draw_Main_Screen();

    Copy_On_To_Off_Page();

    m_event_message = (char *)Near_Allocate_First(500);

    m_event_message_record = Near_Allocate_Next(500);


    Get_Event_Message();


    Clear_Fields();

    Set_Input_Delay(1);

    if(magic_set.event_music == ST_TRUE)
    {

        if(m_event_good_array[m_current_event] == ST_TRUE)
        {
            event_music_num = MUSIC_Events;
        }
        else
        {
            event_music_num = MUSIC_Bad_Event;
        }

        if(
            (m_current_event == et_Good_Moon)
            &&
            (_players[HUMAN_PLAYER_IDX].spellranks[sbr_Death] == 0)
        )
        {
            event_music_num = MUSIC_Bad_Event;
        }

        if(
            (m_current_event == et_Bad_Moon)
            &&
            (_players[HUMAN_PLAYER_IDX].spellranks[sbr_Life] == 0)
        )
        {
            event_music_num = MUSIC_Bad_Event;
        }

        sound_seg = LBX_Reload(music_lbx_file__ovr081, event_music_num, SND_Music_Segment);
        sound_seg_size = lbxload_entry_length;
        Play_Sound(sound_seg, sound_seg_size);

    }

    m_event_display_seg = LBX_Reload(events_lbx_file__ovr081, (m_current_event - 1), _screen_seg);

    Notify2(160, 70, 2, m_event_message, 1, m_event_display_seg, 0, 8, 0, 0, 0, 1, 1);

    Clear_Palette_Changes(0, 255);

    Set_Palette_Changes(0, 223);

    Update_Remap_Gray_Palette();

    // DOMSDOS  Play_Background_Music__STUB();
    Play_Background_Music();

    Allocate_Reduced_Map();

    if(m_current_event != et_Gift)
    {
        Dissolve_Main_Screen();
    }

}


// WZD s081p02
// drake178: EVNT_ComposeMessage()
// MoO2  Module: EVENTS  Get_Event_Message_()
/*
; composes an event message based on global variables,
; replacing any data bytes found in the string loaded
; from EVENTMSG.LBX as follows:
;   $80 - EVNT_TargetCity name
;   $81 - EVNT_MsgDataValue as city index name
;   $82 - EVNT_MsgDataValue as city index size name
;   $83 - EVNT_MsgDataValue as integer string
;   $89 - EVNT_MsgDataValue as TBL_EVNT_OreNames@ index
;   $8A - EVNT_MsgDataValue as item index name
;   $8B - EVNT_TargetCity size name
;   $8C - EVNT_TargetPlayer name
;   $8F - EVNT_LostUnitCount, or 'no'
;   $90 - EVNT_LostPopulation + 'thousand', or 'no'
;   $91 - EVNT_DestroyedBldngs, or 'no'
;   $93 - plural 's' for EVNT_LostUnitCount
;   $94 - plural 's' for EVNT_DestroyedBldngs
*/
/*
    builds the event message in m_event_message[]
*/
void Get_Event_Message(void)
{
    char temp_string[LEN_STRING];
    int16_t event_message_record_idx;
    int16_t IDK;  // _SI_
    int16_t itr;  // _DI_

    IDK = 0;

    event_message_record_idx = (((m_current_event - 1) * 7) + m_event_message_type);

    LBX_Load_Data_Static(eventmsg_lbx_file__ovr081, 0, m_event_message_record, event_message_record_idx, 1, 200);

    for(itr = 0; ((itr < 200) & (m_event_message_record[itr] != 0)); itr++)
    {

        m_event_message[IDK] = m_event_message_record[itr];

        // ¿ no 4, 5, 6, 7, 8, 13, 14, or 18 ?
        switch((m_event_message[IDK] - 128))
        {
            case 0:
            {
                stu_strcpy(&m_event_message[IDK], _CITIES[m_event_city_idx].name);
                IDK = (int16_t)strlen(m_event_message);
            } break;
            case 1:
            {
                stu_strcpy(&m_event_message[IDK], _CITIES[EVNT_MsgDataValue].name);
                IDK = (int16_t)strlen(m_event_message);
            } break;
            case 2:
            {
                stu_strcpy(&m_event_message[IDK], _city_size_names[_CITIES[EVNT_MsgDataValue].size]);
                IDK = (int16_t)strlen(m_event_message);
            } break;
            case 3:
            {
                m_event_message[IDK] = 0;
                stu_itoa(EVNT_MsgDataValue, temp_string, 10);
                stu_strcat(m_event_message, temp_string);
                IDK = (int16_t)strlen(m_event_message);
            } break;
            // case 4:
            // case 5:
            // case 6:
            // case 7:
            // case 8:
            case 9:
            {
                m_event_message[IDK] = 0;
                stu_strcat(m_event_message, TBL_EVNT_OreNames[EVNT_MsgDataValue]);
                IDK = (int16_t)strlen(m_event_message);
            } break;
            case 10:
            {
                stu_strcpy(&m_event_message[IDK], _ITEMS[EVNT_MsgDataValue].name);
                IDK = (int16_t)strlen(m_event_message);
            } break;
            case 11:
            {
                stu_strcpy(&m_event_message[IDK], _city_size_names[_CITIES[m_event_city_idx].size]);
                IDK = (int16_t)strlen(m_event_message);
            } break;
            case 12:
            {
                stu_strcpy(&m_event_message[IDK], _players[m_event_player_idx].name);
                IDK = (int16_t)strlen(m_event_message);
            } break;
            // case 13:
            // case 14:
            case 15:
            {
                if(EVNT_LostUnitCount > 0)
                {
                    m_event_message[IDK] = 0;
                    stu_itoa(EVNT_LostUnitCount, temp_string, 10);
                    stu_strcat(&m_event_message[0], temp_string);
                }
                else
                {
                    m_event_message[IDK] = 0;
                    stu_strcat(&m_event_message[0], cnst_EventMsg_1);  // "no"
                }
                IDK = (int16_t)strlen(m_event_message);
            } break;
            case 16:
            {
                if(EVNT_LostPopulation > 0)
                {
                    m_event_message[IDK] = 0;
                    stu_itoa(EVNT_LostPopulation, temp_string, 10);
                    stu_strcat(&m_event_message[0], temp_string);
                    stu_strcpy(&m_event_message[0], cnst_EventMsg_2);  // " thousand"
                }
                else
                {
                    m_event_message[IDK] = 0;
                    stu_strcat(&m_event_message[0], cnst_EventMsg_1);  // "no"
                }
                IDK = (int16_t)strlen(m_event_message);
            } break;
            case 17:
            {
                if(EVNT_DestroyedBldngs > 0)
                {
                    m_event_message[IDK] = 0;
                    stu_itoa(EVNT_DestroyedBldngs, temp_string, 10);
                    stu_strcat(&m_event_message[0], temp_string);
                }
                else
                {
                    m_event_message[IDK] = 0;
                    stu_strcat(&m_event_message[0], cnst_EventMsg_1);  // "no"
                }
                IDK = (int16_t)strlen(m_event_message);
            } break;
            // case 18:
            case 19:
            {
                if(EVNT_LostUnitCount == 1)
                {
                    IDK++;
                }
                else
                {
                    m_event_message[IDK] = 0;
                    stu_strcat(&m_event_message[0], cnst_EventMsg_3);
                    IDK = (int16_t)strlen(m_event_message);
                }
            } break;
            case 20:
            {
                if(EVNT_DestroyedBldngs == 1)
                {
                    IDK++;
                }
                else
                {
                    m_event_message[IDK] = 0;
                    stu_strcat(&m_event_message[0], cnst_EventMsg_3);
                    IDK = (int16_t)strlen(m_event_message);
                }
            } break;
            default:
            {
                IDK++;
            }
        }

    }

    m_event_message[IDK] = '\0';

}


// WZD s081p03
// MoO2  Module: EVENTS  Get_Event_Victim_()
/**
 * @brief Selects a target wizard for a random event using weighted power.
 *
 * @details
 * Builds per-player selection weights from each wizard's astrologer totals:
 * magic power, spell research, and army strength. The @p event_type argument
 * controls whether strong or weak wizards are favored.
 *
 * Weight construction:
 * - Base weight per active player: 
 *   @code
 *   astrologer.magic_power + astrologer.spell_research + astrologer.army_strength
 *   @endcode
 * - For good events (@c event_type == ST_TRUE), weights are inverted as
 *   @c (600 - weight), then clamped at 0.
 * - Total weight is repeatedly halved across all candidates until the sum fits
 *   within RNG range constraints (<= 500).
 *
 * Selection:
 * - If the final total weight is < 1, returns @c ST_UNDEFINED (no valid victim).
 * - Otherwise rolls a random remainder in the total-weight range and walks
 *   player indices from @c HUMAN_PLAYER_IDX upward, subtracting weights until
 *   the selected bucket is reached.
 *
 * @param event_type Event polarity selector used by caller-provided alignment:
 *                   typically @c ST_TRUE for good-event targeting behavior and
 *                   @c ST_FALSE for bad-event targeting behavior.
 *
 * @return Selected player index in the active player range, or
 *         @c ST_UNDEFINED when all candidate weights collapse to zero.
 *
 * @note Uses only the first @c _num_players entries and ignores inactive slots.
 * @note The scan starts from @c HUMAN_PLAYER_IDX by design of legacy logic.
 * @note RNG calls use @c Random(), which returns a 1-based result in this codebase.
 *
 * @see Determine_Event(), m_event_good_array, Random()
 */
int16_t Get_Event_Victim(int16_t event_type)
{
    int16_t wizard_power[NUM_PLAYERS] = { 0, 0, 0, 0, 0, 0 };
    int16_t weight_total_in_range = 0;  /* exit flag for the weight-scaling loop */
    int16_t victim_idx = 0;
    int16_t roll_remaining = 0;         /* the roulette-wheel position for the weighted pick */
    int16_t itr = 0;
    int16_t total_weight = 0;           /* running weight sum */
    for(itr = 0; itr < NUM_PLAYERS; itr++)
    {
        wizard_power[itr] = 0;
    }
    for(itr = 0; itr < _num_players; itr++)
    {
        wizard_power[itr] = (_players[itr].astrologer.magic_power + _players[itr].astrologer.spell_research + _players[itr].astrologer.army_strength);
    }
    if(event_type == ST_TRUE)
    {
        for(itr = 0; itr < _num_players; itr++)
        {
            wizard_power[itr] = (600 - wizard_power[itr]);
            if(wizard_power[itr] < 1)
            {
                wizard_power[itr] = 0;
            }
        }
    }
    weight_total_in_range = ST_FALSE;
    while(weight_total_in_range == ST_FALSE)
    {
        total_weight = 0;
        for(itr = 0; itr < _num_players; itr++)
        {
            total_weight += wizard_power[itr];
        }
        if(total_weight > 500)
        {
            for(itr = 0; itr < _num_players; itr++)
            {
                wizard_power[itr] = (wizard_power[itr] / 2);
            }
        }
        else
        {
            weight_total_in_range = ST_TRUE;
        }
    }
    if(total_weight < 1)
    {
        return ST_UNDEFINED;
    }
    roll_remaining = (Random(total_weight) - 1);
    victim_idx = HUMAN_PLAYER_IDX;
    while(roll_remaining >= 0)
    {
        if((_num_players - 1) <= victim_idx)
        {
            break;
        }
        roll_remaining -= wizard_power[victim_idx];
        if(roll_remaining >= 0)
        {
            victim_idx++;
        }
    }
    return victim_idx;
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
    int16_t city_idx;
    int16_t itr_cities;
    int16_t IDK;  // _DI_

    if(_cities < 1)
    {
        return ST_UNDEFINED;
    }


    city_idx = (Random(_cities) - 1);

    IDK = ST_UNDEFINED;

    for(itr_cities = 0; ((itr_cities < _cities) && (IDK == ST_UNDEFINED)); itr_cities++)
    {

        if(_CITIES[city_idx].owner_idx == player_idx)
        {

            IDK = city_idx;

        }

        if(IDK != ST_UNDEFINED)
        {

            if(TBL_Unrest[_players[player_idx].capital_race][_CITIES[city_idx].race] > 1)
            {

                IDK = ST_UNDEFINED;

            }

        }

        city_idx = ((city_idx + 1) % _cities);

    }

    
    return city_idx;
}
