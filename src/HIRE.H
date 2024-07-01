/*
    Hire - Merchant

    WIZARDS.EXE
        ovr127
*/

#ifndef HIRE_H
#define HIRE_H

#include "MoX.H"

#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr127
*/

// WZD s127p01
// EVNT_LoadMerchantWnd()

// WZD s127p02
// EVNT_DrawMerchantWnd()

// WZD s127p03
// EVNT_MerchantDialog()

// WZD s127p04
void Hire_Hero_Load(int16_t unit_type);

// WZD s127p05
void Hire_Hero_Popup_Draw(void);

// WZD s127p06
int16_t Hire_Hero_Popup(int16_t hero_slot_idx, int16_t unit_type_idx, int16_t hire_type);

// WZD s127p07
// EVNT_DrawMercHire()

// WZD s127p08
// EVNT_MercHireDialog()

// WZD s127p09
// AI_OfferHero()



#ifdef __cplusplus
}
#endif

#endif  /* HIRE_H */
