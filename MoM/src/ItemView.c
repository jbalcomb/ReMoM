/*
    Item View

    WIZARDS.EXE
        ovr082
*/

#include "ItemView.h"

#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/FLIC_Draw.h"
#include "../../MoX/src/Fields.h"
#include "../../MoX/src/Fonts.h"
#include "../../MoX/src/GENDRAW.h"
#include "../../MoX/src/MOM_Data.h"
#include "../../MoX/src/MOX_DAT.h"  /* _screen_seg */
#include "../../MoX/src/MOX_T4.h"
#include "../../MoX/src/MOX_TYPE.h"

#include "ItemScrn.h"
#include "../../MoX/src/Timer.h"
#include "../../MoX/src/Video.h"
#include "MainScr.h"  /* enchantment_outline_colors */
#include "UnitView.h"



// WZD dseg:C2F0                                                 ¿ BEGIN:  ovr082  'Item View' ?

// WZD dseg:C2F0
int16_t item_enchantment_animation_count;

// WZD dseg:C2F2
int16_t item_enchantment_animation_flag;

// WZD dseg:C2F4
int16_t item_view_start_y;
// WZD dseg:C2F6
int16_t item_view_start_x;
// WZD dseg:C2F8
int16_t item_view_item_idx;
// WZD dseg:C2FA
SAMB_ptr item_view_popup_bitm;

// WZD dseg:C2FA                                                 ¿ END:  ovr082  'Item View' ?



/*
    WIZARDS.EXE  ovr082
*/

// WZD s82p0
void Item_View_Popup(int16_t item_idx, int16_t start_x, int16_t start_y)
{
    int16_t hotkey_ESC;
    int16_t input_field_idx;
    int16_t leave_screen;  // _SI_

    item_view_item_idx = item_idx;
    item_view_start_x = start_x;
    item_view_start_y = start_y;

    Assign_Auto_Function(Item_View_Popup_Draw, 1);

    // ; saves the contents of the current draw frame into the 4th VGA frame, starting at $AC00, overwriting whatever was there before
    Copy_Off_To_Page4();

    item_view_popup_bitm = Allocate_Next_Block(_screen_seg, 1200);
    m_item_icon_workarea = Allocate_Next_Block(_screen_seg, 50);

    // ; resets the view growth animation stage for the item view window, starting / restarting the animation
    ITEM_ResetViewGrow();

    leave_screen = ST_FALSE;

    while(leave_screen == ST_FALSE)
    {
        Mark_Time();
        Clear_Fields();

        hotkey_ESC = Add_Hidden_Field(0, 0, SCREEN_XMAX, SCREEN_YMAX, ST_NULL, ST_UNDEFINED);

        input_field_idx = Get_Input();

        if(input_field_idx == hotkey_ESC)
        {
            Play_Left_Click();
            leave_screen = ST_UNDEFINED;
        }

        if(leave_screen == ST_FALSE)
        {
            Item_View_Popup_Draw();
            PageFlip_FX();
            Release_Time(1);
        }
    }

    Deactivate_Auto_Function();
    Clear_Fields();
}


// WZD s82p0
/*

Item_Screen()
    item_slot_x = (_item_window_start_x + 54 + (((itr_hero_slots % 2) * 136) + (itr_item_slots * 29)));
    item_slot_y = (_item_window_start_y + ((itr_hero_slots / 2) * 28));
    Item_View_Popup(_players[HUMAN_PLAYER_IDX].Heroes[itr_hero_slots].Items[itr_item_slots], (item_slot_x + 8), (item_slot_y + 8));

Item_View_Popup(int16_t item_idx, int16_t start_x, int16_t start_y)
    item_view_start_x = start_x;
    item_view_start_y = start_y;

*/
void Item_View_Popup_Draw(void)
{
    int16_t IView_Draw_Top;
    int16_t IView_Draw_Left;

    Set_Page_Off();

    Copy_Page4_To_Off();

    Item_View_Prepare(item_view_item_idx, item_view_popup_bitm, m_item_icon_workarea);

    Cycle_Item_Enchantment_Animation();

    ITEM_GetViewDrawPos(item_view_start_x, item_view_start_y, item_view_popup_bitm, 5, &IView_Draw_Left, &IView_Draw_Top);

    Draw_Picture(IView_Draw_Left, IView_Draw_Top, item_view_popup_bitm);
}


// WZD s82p0
/*

XREF:
    Item_View_Popup_Draw()
    Item_Pool_View_Draw()
    j_Item_View_Prepare()
        Merchant_Popup_Draw()

*/
void Item_View_Prepare(int16_t item_idx, SAMB_ptr item_bitmap_seg, SAMB_ptr item_icon_pict_seg)
{
    char Power_Texts[5][40];
    char Power_Names[5][20];
    uint8_t colors[4];
    int16_t y_start;
    int16_t line_space;
    int16_t Power_Count;
    int16_t Name_Width;
    int16_t itr_item_powers;  // _SI_

    Draw_Picture_To_Bitmap(item_view_background_seg, item_bitmap_seg);
    Draw_Item_Icon_With_Enchantment_Outline(item_idx, item_icon_pict_seg);
    Clipped_Copy_Bitmap(9, 6, item_bitmap_seg, item_icon_pict_seg);
    
    Set_Font_Style_Shadow_Down(4, 4, 0, 0);
    Set_Alias_Color(46);
    Set_Font_Spacing_Width(1);
    Set_Outline_Color(47);
    Print_To_Bitmap_Far(31, 12, _ITEMS[item_idx].name, item_bitmap_seg);

    ITEM_GetPowerNames(item_idx, Power_Names, Power_Texts, &Power_Count);
    Draw_Picture_To_Bitmap(item_view_bullet_seg, item_icon_pict_seg);

    colors[0] = 46;
    colors[1] = 177;
    if(Power_Count == 5)
    {
        Set_Font_Colors_15(1, &colors[0]);
        Set_Font_Style_Shadow_Down(2, 15, 0, 0);
        line_space = 8;
        y_start = 34;
    }
    else
    {
        Set_Font_Colors_15(2, &colors[0]);
        Set_Font_Style_Shadow_Down(2, 15, 0, 0);
        line_space = 11;
        y_start = 33;
    }
    Set_Font_Spacing_Width(1);
    Set_Outline_Color(47);
    for(itr_item_powers = 0; itr_item_powers < Power_Count; itr_item_powers++)
    {
        Clipped_Copy_Bitmap(11, (34 + (itr_item_powers * line_space)), item_bitmap_seg, item_icon_pict_seg);
        Print_To_Bitmap(20, (y_start + (itr_item_powers * line_space)), Power_Names[itr_item_powers], item_bitmap_seg);
        Name_Width = Get_String_Width(Power_Names[itr_item_powers]);
        Print_To_Bitmap((21 + Name_Width), (y_start + (itr_item_powers * line_space)), Power_Texts[itr_item_powers], item_bitmap_seg);
    }
}


// WZD s82p0
/*
; shows the description of the item being awarded as a
; dialogue window, kept on screen until clicked away
*/
void Item_Pool_View(void)
{
    int16_t hotkey_ESC;
    int16_t input_field_idx;
    int16_t leave_screen;  // _SI_
    Copy_Off_To_Back();
    Load_Palette_From_Animation(hero_background_seg);
    Update_Remap_Gray_Palette();
    item_view_popup_bitm = Allocate_First_Block(_screen_seg, 1200);  // 1200 PR, 19200 B
    m_item_icon_workarea = Allocate_Next_Block(_screen_seg, 60);  // 60 PR, 960 B
    Assign_Auto_Function(Item_Pool_View_Draw, 1);
    Set_Mouse_List(1, mouse_list_default);
    leave_screen = ST_FALSE;
    while(leave_screen == ST_FALSE)
    {
        Mark_Time();
        Clear_Fields();
        hotkey_ESC = Add_Hidden_Field(0, 0, SCREEN_XMAX, SCREEN_YMAX, '\x1B', ST_UNDEFINED);
        input_field_idx = Get_Input();
        if(input_field_idx == hotkey_ESC)
        {
            leave_screen = ST_UNDEFINED;
        }
        if(leave_screen == ST_FALSE)
        {
            Item_Pool_View_Draw();
            PageFlip_FX();
            Release_Time(1);
        }
    }
    Set_Mouse_List(1, mouse_list_none);
}


// WZD s82p0
/*
; prepares and draws the description window of the item
; being awarded into the current draw segment onto a
; background loaded from the third VGA frame
*/
void Item_Pool_View_Draw(void)
{
    Set_Page_Off();
    Copy_Back_To_Off();
    Item_View_Prepare(item_pool_item_idx, item_view_popup_bitm, m_item_icon_workarea);
    Draw_Picture(50, 50, item_view_popup_bitm);
    Cycle_Item_Enchantment_Animation();
}

// WZD s82p0
/*
    ...Draw Item, with Enchantment Outline
    ~ Draw_Unit_Picture()

; draws the specified item, including its power outline, into the passed buffer, at pixel coordinates [2,2],
; using GUI_SmallWork_IMG@ as temporary storage for the item icon itself
; - the final image size is 19 by 19 pixels

2 pixel border, because 1 pixel reserved for optional outline

background picture is 19x19
icon bitmap is 17x17

Draw_Picture_To_Bitmap()
    item_icons_seg ==> GfxBuf_2400B
Create_Picture()
    item_icon_pict_seg
Clipped_Copy_Bitmap()
    GfxBuf_2400B ==> item_icon_pict_seg
Outline_Bitmap_Pixels()
    item_icon_pict_seg
Bitmap_Aura_Pixels()
    item_icon_pict_seg

~ Draw_Unit_Enchantment_Outline(int16_t unit_idx)  // WZD o063p03
  but, bitmap/picture already in UnitDraw_WorkArea

Draw_Unit_Picture()
    |-> Draw_Unit_StatFig()
        |-> Draw_Picture_To_Bitmap()
        |-> Draw_Unit_Enchantment_Outline()
        |-> Draw_Picture()

Draw_Item_With_Name()
    |-> Draw_Item_Icon_With_Enchantment_Outline(item_idx, m_item_icon_workarea)

*/
void Draw_Item_Icon_With_Enchantment_Outline(int16_t item_idx, SAMB_ptr item_icon_bitm)
{
    uint32_t item_powers;
    uint16_t item_powers_HI;  // _DX_
    uint16_t item_powers_LO;  // _AX_
    int16_t magic_realm;  // _SI_
    uint8_t * color_list;  // DNE in Dasm
    struct s_ITEM * item_ptr;

    Draw_Picture_To_Bitmap(item_icons_seg[_ITEMS[item_idx].icon_idx], GfxBuf_2400B);

    Create_Picture(19, 19, item_icon_bitm);

    Clipped_Copy_Bitmap(2, 2, item_icon_bitm, GfxBuf_2400B);

    item_ptr = &_ITEMS[item_idx];

    item_powers = _ITEMS[item_idx].Powers;

    magic_realm = ST_UNDEFINED;

// #define ITEM_POWER(_item_idx_,_item_power_)  ( ((_ITEMS[(_item_idx_)].Powers & (1 << (_item_power_))) != 0) )
// ip_Invulnerability = 32 ... warning C4293: '<<': shift count negative or too big, undefined behavior
// 1000 0000 0000 0000 0000 0000 0000 0000
//                                       ^ bit 1  ((_ITEMS[(_item_idx_)].Powers & (1 << (0))) != 0)
// ^ bit 32 ((_ITEMS[(_item_idx_)].Powers & (1 << (31))) != 0)
// else if((item_powers & IP_PHANTASMAL)          != 0) { magic_realm = 4; }
// else if((item_powers & (1 << (ip_Phantasmal))) != 0) { magic_realm = 4; }

    if(item_powers != 0)
    {
        if(      (item_powers & IP_HASTE             ) != 0) { magic_realm = mr_Sorcery;  }
        else if( (item_powers & IP_PHANTASMAL        ) != 0) { magic_realm = mr_Sorcery;  }
        else if( (item_powers & IP_CHAOS             ) != 0) { magic_realm = mr_Chaos;    }
        else if( (item_powers & IP_TRUE_SIGHT        ) != 0) { magic_realm = mr_Life;     }
        else if( (item_powers & IP_LION_HEART        ) != 0) { magic_realm = mr_Life;     }
        else if( (item_powers & IP_REGENERATION      ) != 0) { magic_realm = mr_Nature;   }
        else if( (item_powers & IP_INVISIBILITY      ) != 0) { magic_realm  = mr_Sorcery; }
        else if( (item_powers & IP_WRAITH_FORM       ) != 0) { magic_realm = mr_Death;    }
        else if( (item_powers & IP_PLANAR_TRAVEL     ) != 0) { magic_realm = mr_Life;     }
        else if( (item_powers & IP_INVULNERABILITY   ) != 0) { magic_realm = mr_Life;     }
        else if( (item_powers & IP_VAMPIRIC          ) != 0) { magic_realm = mr_Chaos;    }
        else if( (item_powers & IP_LIGHTNING         ) != 0) { magic_realm = mr_Chaos;    }
        else if( (item_powers & IP_DESTRUCTION       ) != 0) { magic_realm = mr_Chaos;    }
        else if( (item_powers & IP_POWER_DRAIN       ) != 0) { magic_realm = mr_Death;    }
        else if( (item_powers & IP_ELEMENTAL_ARMOUR  ) != 0) { magic_realm = mr_Nature;   }
        else if( (item_powers & IP_MAGIC_IMMUNITY    ) != 0) { magic_realm  = mr_Sorcery; }
        else if( (item_powers & IP_RIGHTEOUSNESS     ) != 0) { magic_realm = mr_Life;     }
        else if( (item_powers & IP_MERGING           ) != 0) { magic_realm = mr_Nature;   }
        else if( (item_powers & IP_FLIGHT            ) != 0) { magic_realm  = mr_Sorcery; }
        else if( (item_powers & IP_PATHFINDING       ) != 0) { magic_realm = mr_Nature;   }
        else if( (item_powers & IP_GUARDIAN_WIND     ) != 0) { magic_realm  = mr_Sorcery; }
        else if( (item_powers & IP_GIANT_STRENGTH    ) != 0) { magic_realm = mr_Nature;   }
        else if( (item_powers & IP_HOLY_AVENGER      ) != 0) { magic_realm = mr_Life;     }
        else if( (item_powers & IP_DEATH             ) != 0) { magic_realm = mr_Death;    }
        else if( (item_powers & IP_FLAMING           ) != 0) { magic_realm = mr_Chaos;    }
        else if( (item_powers & IP_CLOAK_OF_FEAR     ) != 0) { magic_realm = mr_Death;    }
        else if( (item_powers & IP_RESIST_ELEMENTS   ) != 0) { magic_realm = mr_Nature;   }
        else if( (item_powers & IP_STONING           ) != 0) { magic_realm = mr_Nature;   }
        else if( (item_powers & IP_RESIST_MAGIC      ) != 0) { magic_realm  = mr_Sorcery; }
        else if( (item_powers & IP_CLOAK_OF_FEAR     ) != 0) { magic_realm = mr_Death;    }  // BUGBUG duplicated
        else if( (item_powers & IP_BLESS             ) != 0) { magic_realm = mr_Life;     }
        else if( (item_powers & IP_ENDURANCE         ) != 0) { magic_realm = mr_Life;     }
        else if( (item_powers & IP_WATER_WALKING     ) != 0) { magic_realm = mr_Nature;   }
        // NO ...else { ... }
    }

    if(magic_realm != ST_UNDEFINED)
    {
        Outline_Bitmap_Pixels(item_icon_bitm, 255);
        Bitmap_Aura_Pixels(item_icon_bitm, 255, enchantment_outline_colors[magic_realm][item_enchantment_animation_count]);
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
