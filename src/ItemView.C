/*
    Item View

    WIZARDS.EXE
        ovr082
*/

#include "MoM.H"
#include "ItemView.H"
#include "MainScr.H"  /* enchantment_outline_colors */



// WZD dseg:C2F0                                                 ¿ BEGIN:  ovr082  'Item View' ?

// WZD dseg:C2F0
int16_t item_enchantment_animation_count;

// WZD dseg:C2F2
int16_t item_enchantment_animation_flag;

// WZD dseg:C2F4 00 00                                           ITEM_ViewTop dw 0                       ; DATA XREF: ITEM_ShowViewWindow+16w ...
// WZD dseg:C2F6 00 00                                           ITEM_ViewLeft dw 0                      ; DATA XREF: ITEM_ShowViewWindow+10w ...
// WZD dseg:C2F8 00 00                                           ITEM_ViewIndex dw 0                     ; DATA XREF: ITEM_ShowViewWindow+Aw ...
// WZD dseg:C2FA                                                 ; unsigned int IMG_ITEM_ViewWindow
// WZD dseg:C2FA 00 00                                           IMG_ITEM_ViewWindow@ dw 0               ; DATA XREF: ITEM_ShowViewWindow+41w ...

// WZD dseg:C2FA                                                 ¿ END:  ovr082  'Item View' ?



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
void ITEM_DrawIntoIMGSeg__WIP(int16_t item_idx, SAMB_ptr item_icon_pict_seg)
{
    uint32_t item_powers;
    int16_t magic_realm;  // _SI_
    uint8_t * color_list;  // DNE in Dasm

    Draw_Picture_To_Bitmap(item_icons_seg[TBL_Items[item_idx].Icon_Index], GfxBuf_2400B);
    Create_Picture(19, 19, item_icon_pict_seg);
    Clipped_Copy_Bitmap(2, 2, item_icon_pict_seg, GfxBuf_2400B);

    item_powers = TBL_Items[item_idx].Powers;

    magic_realm = -1;

    if(item_powers != 0)
    {
        if((item_powers & 0x4000) != 0)  /* IP_Haste */
        {
            magic_realm = 4;
        }
        // else if((item_powers & 0x0080) != 0)  /* IP_Phantasmal */
        // NO ...else { ... }
    }

    /* HACK */  magic_realm = 4;  // ALWAYS Sorcery

    if(magic_realm != -1)
    {
        Outline_Bitmap_Pixels(item_icon_pict_seg, 255);
        color_list = (uint8_t *)(enchantment_outline_colors + ((magic_realm * 64) + (item_enchantment_animation_count * 8)));
        Bitmap_Aura_Pixels(item_icon_pict_seg, 255, color_list);
    }

}


// WZD s82p0
// drake178: GAME_Animate_Items()
void Cycle_Item_Enchantment_Animation(void)
{
    item_enchantment_animation_flag++;

    if(item_enchantment_animation_flag > 1)
    {
        item_enchantment_animation_flag = 0;
    }

    if(item_enchantment_animation_flag < 1)
    {
        item_enchantment_animation_count++;
    }

    if(item_enchantment_animation_count > 7)
    {
        item_enchantment_animation_count = 0;
    }
}
