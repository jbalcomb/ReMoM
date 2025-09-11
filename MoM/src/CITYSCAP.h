/*
    Cityscape  (City Screen)

    WIZARDS.EXE
        ovr144
*/
#ifndef CITYSCAP_H
#define CITYSCAP_H

#include "../../MoX/src/MOX_TYPE.h"



#define NUM_CITYSCAPE_ROWS         15
#define NUM_CITYSCAPE_COLS         23
#define SZ_CITYSCAPE_ROW           23  // 46 B, 23 int16_t



// WZD dseg:CA7E
extern SAMB_ptr cityscape_roads_vertical_mask_seg;
// WZD dseg:CA80
extern SAMB_ptr cityscape_roads_horizontal_mask_seg;



#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr144
*/


// WZD o144p01
void Cityscape_Build_Anim_Reset(void);

// WZD o144p02
void Cityscape_Window__WIP(int16_t city_idx, int16_t xstart, int16_t ystart, int16_t bldg_idx_1, int16_t bldg_idx_2);

// WZD o144p03
void Cityscape_Roads_1__WIP(int16_t xstart, int16_t ystart);

// WZD o144p04
void Cityscape_Roads_2__WIP(SAMB_ptr picture);

// WZD o144p05
void Cityscape_Roads_3__WIP(int16_t x, int16_t y, SAMB_ptr picture);

// WZD o144p06
void Cityscape_Roads_4__WIP(int16_t x, int16_t y, SAMB_ptr picture);

// WZD o144p07
void Cityscape_Roads_5__WIP(int16_t x, int16_t y, SAMB_ptr picture);

// WZD o144p08
void Cityscape_Draw_Buildings(int16_t city_idx, int16_t x_start, int16_t y_start, int16_t bldg_idx);

// WZD o144p09
void Cityscape_Make_Buildings_Array(int16_t city_idx, int16_t bldg_idx);

// WZD o144p10
void Cityscape_Draw_Foreground(int16_t city_idx, int16_t xstart, int16_t ystart);

// WZD o144p11
void Cityscape_Draw_Background(int16_t city_idx, int16_t xstart, int16_t ystart, int16_t city_wx, int16_t city_wy, int16_t city_wp);

// WZD o144p12
void Cityscape_Add_Bldg_To_Fields_Array(int16_t x, int16_t y, int16_t bldg_idx, int16_t type);

// WZD o144p13
void Outpost_Cityscape(int16_t city_idx, int16_t x_start, int16_t y_start);

// WZD o144p14
void Outpost_Cityscape_Draw(int16_t city_idx, int16_t x_start, int16_t y_start);



#ifdef __cplusplus
}
#endif



#endif /* CITYSCAP_H */
