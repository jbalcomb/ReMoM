/*
    Events

    WIZARDS.EXE
        ovr080
        ovr081
*/

#ifndef EVENTS_H
#define EVENTS_H

#include "../../MoX/src/MOX_TYPE.h"




/*
19 events
{0, ..., 18}

*/
enum e_EVENT_TYPE
{
    et_None                 =  0,
    et_Meteor               =  1,
    et_Gift                 =  2,
    et_Disjunction          =  3,
    et_Marriage             =  4,
    et_Earthquake           =  5,
    et_Pirates              =  6,
    et_Plague               =  7,
    et_Rebellion            =  8,
    et_Donation             =  9,
    et_Depletion            = 10,
    et_New_Mine             = 11,
    et_Population_Boom      = 12,
    et_Good_Moon            = 13,
    et_Bad_Moon             = 14,
    et_Conjunction_Chaos    = 15,
    et_Conjunction_Nature   = 16,
    et_Conjunction_Sorcery  = 17,
    et_Mana_Short           = 18
};

enum e_EVENT_STATUS
{
    es_Nothing = 0,
    es_Starting = 1,
    es_Ongoing = 2,
    es_Ending = 3
};

enum e_EVENT_MESSAGE_TYPE
{
    emt_Started = 0,
    emt_Ended = 1,
    emt_IDK_two = 2,
    emt_Happened = 3,
    emt_Elsewhere = 4
};



#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr080
*/

// WZD s080p01
void Determine_Event(void);

// WZD s080p02
void Event_Twiddle(void);

// WZD s080p03
int16_t Any_Overland_Enchantments(void);



/*
    WIZARDS.EXE  ovr081
*/

// WZD s081p01
void Show_Event_Message(void);

// WZD s081p02
void Get_Event_Message(void);

// WZD s081p03
int16_t Get_Event_Victim(int16_t event_type);

// WZD s081p04
int16_t Pick_Random_City(int16_t player_idx);



#ifdef __cplusplus
}
#endif

#endif  /* EVENTS_H */
