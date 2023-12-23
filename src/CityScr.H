/*
    City Screen

    WIZARDS.EXE
        ovr054

*/

#ifndef CITYSCR_H
#define CITYSCR_H



// WZD dseg:9936
// drake178: IMG_CTY_Screen_BG
extern SAMB_ptr city_background_seg;                  // BACKGRND.LBX, 6

// WZD dseg:9938
// drake178: IMG_CTY_Block_Out
extern SAMB_ptr city_block_out_seg;                  // BACKGRND.LBX, 

// WZD dseg:993A
// drake178: IMG_CTY_ProdDone
extern SAMB_ptr city_full_resource_seg;                  // BACKGRND.LBX, 

// WZD dseg:993C
// drake178: IMG_CTY_ProdLeft
extern SAMB_ptr city_reqd_resource_seg;                  // BACKGRND.LBX, 

// WZD dseg:993E
// drake178: IMG_CTY_BUY_Lock
extern SAMB_ptr city_lock_buy_button_seg;                  // BACKGRND.LBX, 

// WZD dseg:9940
// drake178: IMG_CTY_OK_Btn
extern SAMB_ptr city_ok_button_seg;                  // BACKGRND.LBX, 

// WZD dseg:9942
// drake178: IMG_CTY_BUY_Btn
extern SAMB_ptr city_buy_button_seg;                  // BACKGRND.LBX, 

// WZD dseg:9944
// drake178: IMG_CTY_CHNG_Btn
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
void City_Add_Fields_Buildings__WIP(void);

// WZD o54p06
void City_Screen_Add_Fields_Production_Window(void);

// WZD o54p07
void City_Screen_Draw_Buttons(void);

// WZD o54p08
// IDK_BuilingCompletedMessage

// WZD o54p09
// NameStartingCity_Dialog_Popup

// WZD o54p10
// Draw_NameStartingCity_Background

// WZD o54p11
void City_Screen_Required_Buildings_List(int16_t city_idx);

// WZD o54p12
void City_Screen_Draw_Garrison_Window(void);

// WZD o54p13
void City_Screen_Load(void);

// WZD o54p14
void City_Screen_Allocate_First_Block(void);

// WZD o54p15
// drake178: CTY_GetBldngGFXSize()
void Draw_Building_Picture_To_Bitmap(int16_t city_idx, int16_t bldg_idx, int16_t * x1, int16_t * y1, int16_t * width, int16_t * height, SAMB_ptr bitmap);

// WZD o54p16
// drake178: N/A
// MoO2  Module: COLCALC  Colony_Can_Afford_To_Buy_Product_()
int16_t City_Can_Buy_Product(void);

// WZD o54p17
void City_Screen_Draw_Map(void);



#ifdef __cplusplus
}
#endif



#endif /* CITYSCR_H */
