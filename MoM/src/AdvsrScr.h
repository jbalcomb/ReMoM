/*
    Advisor Screen

    WIZARDS.EXE
        ovr076
    
*/

#ifndef ADVSRSCR_H
#define ADVSRSCR_H

#include "../../MoX/src/MOX_TYPE.h"



#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr076
*/

// WZD o76p01
void Advisor_Screen(int16_t advisor_idx);

// WZD o76p02
void TaxCollector_Window(void);

// WZD o76p03
void Powergraph_Screen(void);

// WZD o76p04
void Powergraph_Screen_Draw(void);

// WZD o76p05
void Powergraph_Draw_Wizard_History(int16_t player_idx, int16_t last_turn, int16_t first_turn);

// WZD o76p06
void Status_Screen(void);

// WZD o76p07
void Status_Screen_Draw(void);

// WZD o76p08
void Cartographer_Screen(void);

// WZD o76p09
void Cartographer_Screen_Draw(void);

// WZD o76p10
void Cartograph_Screen_Draw_Map(int16_t flag);

// WZD o76p11
void Grand_Vizier_Window(void);



#ifdef __cplusplus
}
#endif


#endif  /* ADVSRSCR_H */
