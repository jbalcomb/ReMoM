/*
    City Screen

    WIZARDS.EXE
        ovr054

*/

#ifndef CITYSCR_H
#define CITYSCR_H

#include "MOX/MOX_TYPE.h"



// WZD dseg:9936
extern SAMB_ptr city_background_seg;                  // BACKGRND.LBX, 6

// WZD dseg:9938
extern SAMB_ptr city_block_out_seg;                  // BACKGRND.LBX, 

// WZD dseg:993A
extern SAMB_ptr city_full_resource_seg;                  // BACKGRND.LBX, 

// WZD dseg:993C
extern SAMB_ptr city_reqd_resource_seg;                  // BACKGRND.LBX, 

// WZD dseg:993E
extern SAMB_ptr city_lock_buy_button_seg;                  // BACKGRND.LBX, 

// WZD dseg:9940
extern SAMB_ptr city_ok_button_seg;                  // BACKGRND.LBX, 

// WZD dseg:9942
extern SAMB_ptr city_buy_button_seg;                  // BACKGRND.LBX, 

// WZD dseg:9944
extern SAMB_ptr city_change_button_seg;                  // BACKGRND.LBX, 



#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr054
*/

// WZD o54p01
void City_Screen__WIP(void);

// WZD o54p02
void City_Screen_Draw__WIP(void);

// WZD o54p03
void City_Screen_Draw2__WIP(void);

// WZD o54p04
void City_Screen_Add_Fields__WIP(void);

// WZD o54p05
void Cityscape_Add_Fields(void);

// WZD o54p06
void City_Screen_Add_Fields_Production_Window(void);

// WZD o54p07
void City_Screen_Draw_Buttons(void);

// WZD o54p08
void City_Built_Building_Message(int16_t x, int16_t y, int16_t city_idx, int16_t bldg_idx);

// WZD o54p09
void Change_Home_City_Name_Popup(int16_t city_idx);

// WZD o54p10
void Draw_NameStartingCity_Background(void);

// WZD o54p11
void City_Screen_Required_Buildings_List(int16_t city_idx);

// WZD o54p12
void City_Screen_Draw_Garrison_Window(void);

// WZD o54p13
void City_Screen_Load(void);

// WZD o54p14
void City_Screen_Allocate_First_Block(void);

// WZD o54p15
void Draw_Building_Picture_To_Bitmap(int16_t city_idx, int16_t bldg_idx, int16_t * x1, int16_t * y1, int16_t * width, int16_t * height, SAMB_ptr bitmap);

// WZD o54p16
// MoO2  Module: COLCALC  Colony_Can_Afford_To_Buy_Product_()
int16_t City_Can_Buy_Product(void);

// WZD o54p17
void City_Screen_Draw_Map(void);



#ifdef __cplusplus
}
#endif



#endif /* CITYSCR_H */
