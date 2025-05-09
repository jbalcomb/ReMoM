/*
    Events

    WIZARDS.EXE
        ovr080
        ovr081
*/

#ifndef EVENTS_H
#define EVENTS_H

#include "MOM.H"

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
