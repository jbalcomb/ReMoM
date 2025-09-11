/*
    Item Screen

    WIZARDS.EXE
        ovr075
    
*/

#ifndef ITEMSCRN_H
#define ITEMSCRN_H

#include "../../MoX/src/MOX_TYPE.h"



#ifdef __cplusplus
extern "C" {
#endif



// WZD dseg:C25E                                                 BEGIN:  ovr075

// WZD dseg:C25E
extern SAMB_ptr m_item_icon_workarea;

// WZD dseg:C270
extern int16_t item_pool_item_idx;



/*
    WIZARDS.EXE  ovr075
*/

// WZD o75p01
void Item_Screen(void);

// WZD o75p02
void Item_Screen_Draw_Do(void);

// WZD o75p03
void Item_Screen_Draw(void);

// WZD o75p04
void Item_Screen_Load(void);

// WZD o75p05
int16_t Check_Same_Location(int16_t item_slot_idx, int16_t hero_idx);

// WZD o75p06
void Draw_Item_With_Name(int16_t x, int16_t y, int16_t item_idx);

// WZD o75p07
void Destroy_Item(void);

// WZD o75p08
void Move_Item(int16_t hero_slot_idx, int16_t item_slot_idx);

// WZD o75p09
void Item_Screen_Add_Fields(void);

// WZD o75p10
void Process_Item_Pool(int16_t item_count, int16_t item_list[]);

// WZD o75p11
void Item_Window_Picture_Coords(int16_t hero_idx, int16_t * x1, int16_t * y1, int16_t * x2, int16_t * y2);

// WZD o75p12
int16_t Check_Wont_Drown(int16_t hero_slot_idx, int16_t item_slot);



#ifdef __cplusplus
}
#endif


#endif  /* ITEMSCRN_H */
