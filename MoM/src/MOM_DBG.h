#ifndef MOM_DBG_H
#define MOM_DBG_H

#include "../../MoX/src/MOX_TYPE.h"



extern int16_t DBG_trigger_event;
extern int16_t DBG_trigger_event_plague;
extern int16_t DBG_trigger_event_population_boom;
extern int16_t DBG_trigger_event_good_moon;
extern int16_t DBG_trigger_event_bad_moon;
extern int16_t DBG_trigger_event_conjunction_chaos;
extern int16_t DBG_trigger_event_conjunction_nature;
extern int16_t DBG_trigger_event_conjunction_sorcery;
extern int16_t DBG_trigger_event_mana_short;
extern int16_t DBG_trigger_offer_item;
extern int16_t DBG_trigger_offer_merc;
extern int16_t DBG_trigger_offer_hero;
extern int16_t DBG_trigger_complete_research_spell;

extern uint8_t DBG_debug_flag;



#ifdef __cplusplus
extern "C" {
#endif



// Where did this go?  void TST_Load_SAVE_GAM(void);



void Test_Run(void);



#ifdef __cplusplus
}
#endif

#endif  /* MOM_DBG_H */
