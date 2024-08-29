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
void Cityscape_Roads_1__WIP(int16_t xstart, int16_t ystart);

// WZD o144p06
void Cityscape_Roads_2__STUB(SAMB_ptr picture);

// WZD o144p07
void Cityscape_Roads_3__STUB(int16_t x, int16_t y, SAMB_ptr picture);

// WZD o144p08
void Cityscape_Roads_4__STUB(int16_t x, int16_t y, SAMB_ptr picture);

// WZD o144p09
void Cityscape_Roads_5__STUB(int16_t x, int16_t y, SAMB_ptr picture);

// WZD o144p10
void Cityscape_Draw_Buildings_And_Features__WIP(int16_t city_idx, int16_t x_start, int16_t y_start, int16_t bldg_idx);

// WZD o144p11
void Cityscape_Set_BldgStruc__WIP(int16_t city_idx, int16_t bldg_idx);

// WZD o144p12
void Cityscape_Draw_Wards_And_Walls__STUB(int16_t city_idx, int16_t xstart, int16_t ystart);

// WZD o144p13
void Cityscape_Draw_Background(int16_t city_idx, int16_t xstart, int16_t ystart, int16_t city_wx, int16_t city_wy, int16_t city_wp);

// WZD o144p14
void Cityscape_XY(int16_t x, int16_t y, int16_t bldg_idx, int16_t type);

// WZD o144p15
void Outpost_Cityscape(int16_t city_idx, int16_t x_start, int16_t y_start);

// WZD o144p16
void Outpost_Cityscape_Draw(int16_t city_idx, int16_t x_start, int16_t y_start);



#ifdef __cplusplus
}
#endif



#endif /* CITYSCAP_H */
