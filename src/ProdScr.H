/*
    Production Screen

    WIZARDS.EXE
        ovr056

*/

#ifndef PRODSCR_H
#define PRODSCR_H

#include "MoM.H"


#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr056
*/

// WZD o56p01
// WZD o56p02
// WZD o56p03
// WZD o56p04
// WZD o56p05
// WZD o56p06
// WZD o56p07
// WZD o56p08
// WZD o56p09

// WZD o56p10
// drake178: CTY_GetProduceCost()
int16_t City_Production_Cost(int16_t production_idx, int16_t city_idx);

// WZD o56p11
// drake178: N/A
// MoO2  Module: COLCALC  Colony_N_Turns_To_Produce_()
int16_t City_N_Turns_To_Produce(int16_t production_cost, int16_t city_idx);

// WZD o56p12



#ifdef __cplusplus
}
#endif



#endif /* PRODSCR_H */
