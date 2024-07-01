/*
    Item View

    WIZARDS.EXE
        ovr082
*/

#ifndef ITEMVIEW_H
#define ITEMVIEW_H

#include "MoM.H"



#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr082
*/

// WZD s82p01
void Item_View_Popup(int16_t item_idx, int16_t start_x, int16_t start_y);

// WZD s82p02
void Item_View_Popup_Draw(void);

// WZD s82p02
void Item_View_Prepare(int16_t item_idx, SAMB_ptr item_bitmap_seg, SAMB_ptr item_icon_pict_seg);

// WZD s82p04
void Item_Pool_View(void);

// WZD s82p05
void Item_Pool_View_Draw(void);

// WZD s82p06
void Draw_Item_Icon_With_Enchantment_Outline(int16_t item_idx, SAMB_ptr item_icon_pict_seg);

// WZD s82p07
void Cycle_Item_Enchantment_Animation(void);



#ifdef __cplusplus
}
#endif


#endif  /* ITEMVIEW_H */
