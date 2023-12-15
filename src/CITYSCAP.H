/*
    Cityscape  (City Screen)

    WIZARDS.EXE
        ovr144
*/
#ifndef CITYSCAP_H
#define CITYSCAP_H

#include "MoM.H"


#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr144
*/


// WZD o144p01
void IDK_Clear_Cityscape_Vanish_Percent(void);

// WZD o144p02
void Cityscape_Draw__WIP(int16_t city_idx, int16_t xstart, int16_t ystart, int16_t bldg_idx_1, int16_t bldg_idx_2);

// WZD o144p03
// sub_CD8B0()

// WZD o144p04
// IDK_City_Screen_Draw_Cityscape()

// WZD o144p05
// IDK_CityScreen_Cityscape_sCDC32()

// WZD o144p06
// IDK_CityScreen_Cityscape_sCEB93()

// WZD o144p07
// sub_CF555()

// WZD o144p08
// sub_CF5FA()

// WZD o144p09
// sub_CF69F()

// WZD o144p10
// IDK_CityScreen_Cityscape_sCF7F1()

// WZD o144p11
void IDK_Cityscape_Set_BldgStruc__STUB(int16_t city_idx, int16_t bldg_idx);

// WZD o144p12
void Cityscape_Draw_Wards_And_Walls__STUB(int16_t city_idx, int16_t xstart, int16_t ystart);

// WZD o144p13
void Cityscape_Draw_Background(int16_t city_idx, int16_t xstart, int16_t ystart, int16_t city_wx, int16_t city_wy, int16_t city_wp);

// WZD o144p14
// IDK_CityScreen_Cityscape_sD1A17()

// WZD o144p15
// CTY_DrawOutpost()

// WZD o144p16
// CTY_DrawOutpostScape()



#ifdef __cplusplus
}
#endif



#endif /* CITYSCAP_H */
