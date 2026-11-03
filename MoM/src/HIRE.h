/*
    Hire - Merchant

    WIZARDS.EXE
        ovr127
*/

#ifndef HIRE_H
#define HIRE_H

#include "../../MoX/src/MOX_TYPE.h"



#ifdef __cplusplus
extern "C" {
#endif



// WZD dseg:C9E0
extern int16_t GUI_InHeroNaming;



/*
    WIZARDS.EXE  ovr127
*/

// WZD o127p01
void Merchant_Popup_Load(void);

// WZD o127p02
void Merchant_Popup_Draw(void);

// WZD o127p03
void Merchant_Popup(void);

// WZD o127p04
void Hire_Hero_Load(int16_t unit_type);

// WZD o127p05
void Hire_Hero_Popup_Draw(void);

// WZD o127p06
int16_t Hire_Hero_Popup(int16_t hero_slot_idx, int16_t unit_type_idx, int16_t hire_type);

// WZD o127p07
void Hire_Merc_Popup_Draw(void);

// WZD o127p08
int16_t Hire_Merc_Popup(int16_t type, int16_t count, int16_t level, int16_t cost);

// WZD o127p09
void AI_Accept_Hero(int16_t player_idx, int16_t hero_slot_idx, int16_t unit_type_idx);



#ifdef __cplusplus
}
#endif

#endif  /* HIRE_H */
