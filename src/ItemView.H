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

// WZD s82p0
// drake178: ITEM_ShowViewWindow()
// ITEM_ShowViewWindow()

// WZD s82p0
// drake178: ITEM_ViewRedraw()
// ITEM_ViewRedraw()

// WZD s82p0
// drake178: ITEM_PrepViewWindow()
// ITEM_PrepViewWindow()

// WZD s82p0
// drake178: ITEM_DescribeAward()
// ITEM_DescribeAward()

// WZD s82p0
// drake178: ITEM_DrawAwardView()
// ITEM_DrawAwardView()

// WZD s82p0
// drake178: ITEM_DrawIntoIMGSeg()
void ITEM_DrawIntoIMGSeg__WIP(int16_t item_idx, SAMB_ptr item_icon_pict_seg);

// WZD s82p0
// drake178: GAME_Animate_Items()
void Cycle_Item_Enchantment_Animation(void);



#ifdef __cplusplus
}
#endif


#endif  /* ITEMVIEW_H */
