

Lair_Combat_Do()
    |-> Combat()
        |-> WIZ_AwardItems()
            |-> GAME_AwardItems()
                ITEM_AwardPending = ST_TRUE
                ITEM_AwardCount = item_count
                ITEM_AwardList[itr] = item_list[itr]
                |-> Item_Screen()
                    |-> Item_Pool_View()
                        |-> Item_Pool_View_Draw()
                            |-> Item_View_Prepare()

ITEM_DescribeAward()


Item_View_Prepare()



Item_Screen()
    ((item_pool_active == ST_TRUE) &&(m_cursor_item_idx == ST_UNDEFINED))
    |-> j_Item_Pool_View()
        |-> Item_Pool_View()
            |-> j_Item_Pool_View_Draw()
            |-> Item_Pool_View_Draw()

Load_Palette_From_Animation(hero_background_seg)
item_view_popup_bitm = Allocate_First_Block(_screen_seg, 1200)
m_item_icon_workarea = Allocate_Next_Block(_screen_seg, 60)

Item_Screen_Load()
    m_item_icon_workarea = Allocate_First_Block(_screen_seg, 30)
    hero_background_seg = LBX_Reload_Next(armylist_lbx_file__ovr075, 5, _screen_seg);



### m_item_icon_workarea

ovr075
WZD dseg:C25E
m_item_icon_workarea

... m_item_icon_workarea@     ~ item_view_workarea_bitm
... item_view_popup_bitm@    ~ item_view_popup_bitm

Allocate:
    Item_Pool_View()
    Item_Screen_Load()
    Item_View_Popup()
Populate:
    Item_Screen_Draw()
    Draw_Item_With_Name()
    Item_View_Prepare()
    Item_View_Popup_Draw() |-> Item_View_Prepare()
    Item_Pool_View_Draw()  |-> Item_View_Prepare()
    ... |-> Draw_Item_Icon_With_Enchantment_Outline()
Render:
    Draw_Picture()
    Clipped_Copy_Bitmap()
    Draw_Picture_Windowed()

XREF:
    Draw_Item_With_Name()   +19     push  [m_item_icon_workarea]
    Draw_Item_With_Name()   +B      push  [m_item_icon_workarea]
    Item_Pool_View()        +3C     mov   [m_item_icon_workarea], ax
    Item_Pool_View_Draw()   +D      push  [m_item_icon_workarea]
    Item_Screen_Draw()      +12B    push  [m_item_icon_workarea]
    Item_Screen_Draw()      +142    push  [m_item_icon_workarea]
    Item_Screen_Draw()      +296    push  [m_item_icon_workarea]
    Item_Screen_Draw()      +2B5    push  [m_item_icon_workarea]
    Item_Screen_Load()      +64     mov   [m_item_icon_workarea], ax
    Item_View_Popup()       +53     mov   [m_item_icon_workarea], ax
    Item_View_Popup_Draw()  +10     push  [m_item_icon_workarea]


Item_Screen_Load()
    m_item_icon_workarea = Allocate_First_Block(_screen_seg, 30)  480 B

Item_Screen_Draw()
    j_Draw_Item_Icon_With_Enchantment_Outline(hero_ptr.Items[itr], m_item_icon_workarea)
    Draw_Picture((portrait_x1+40+(29*itr_hero_items)), (portrait_y1+15), m_item_icon_workarea)
    j_Draw_Item_Icon_With_Enchantment_Outline(_players[_human_player_idx].Vault_Items[itr], m_item_icon_workarea)
    Draw_Picture(item_icon_x, item_icon_y, m_item_icon_workarea)

Item_Pool_View()
    |-> item_view_popup_bitm = Allocate_First_Block(_screen_seg, 1200)  19200 B
    |-> m_item_icon_workarea = Allocate_Next_Block(_screen_seg, 60)        960 B

Item_Pool_View_Draw()
    |-> Item_View_Prepare(item_pool_item_idx, item_view_popup_bitm, m_item_icon_workarea)
    |-> Draw_Picture(50, 50, item_view_popup_bitm)

Item_View_Popup()
    |-> item_view_popup_bitm = Allocate_First_Block(_screen_seg, 1200)  19200 B
    |-> m_item_icon_workarea = Allocate_Next_Block(_screen_seg, 50)        800 B

Item_View_Popup_Draw()
    |-> Item_View_Prepare(item_view_item_idx, item_view_popup_bitm, m_item_icon_workarea)
        |-> Draw_Item_Icon_With_Enchantment_Outline(item_idx, item_view_workarea_bitm)
        |-> Clipped_Copy_Bitmap(9, 6, item_view_popup_bitm, item_view_workarea_bitm)

Draw_Item_With_Name()
    |-> j_Draw_Item_Icon_With_Enchantment_Outline(item_idx, m_item_icon_workarea)
    |-> Draw_Picture_Windowed(x, y, m_item_icon_workarea)



Destroy_Item()
    clears the cursor item index, so Item_Screen_Draw() won't draw it
    calls Item_Screen_Draw_Do()
    restores the cursor item index
    does its own Draw_Item_With_Name() for the item, over the Anvil





What is the item index at this point?
item_pool_item_idx
    ...
        |-> Item_Pool_View()
            |-> Item_Pool_View_Draw()
                |-> Item_View_Prepare()
from Item_Screen()
    item_pool_item_idx = item_pool_list[item_pool_list_index]



### item_view_popup_bitm

ITEM_DescribeAward()
    item_view_popup_bitm = Allocate_First_Block(_screen_seg, 1200);  // 1200 PR, 19200 B
    m_item_icon_workarea = Allocate_Next_Block(_screen_seg, 60);  // 60 PR, 960 B



### ITEM_DrawAwardView()

Item_View_Prepare(ITEM_CurrentAward, item_view_popup_bitm, m_item_icon_workarea);
Draw_Picture(50, 50, item_view_popup_bitm);



### Item_View_Prepare()
// WZD s82p0
void Item_View_Prepare(int16_t item_idx, SAMB_ptr item_bitmap_seg, SAMB_ptr item_icon_pict_seg)

called from ITEM_DrawAwardView()
    |-> Item_View_Prepare(ITEM_CurrentAward, item_view_popup_bitm, m_item_icon_workarea);
ITEM_CurrentAward
item_view_popup_bitm
m_item_icon_workarea

ITEM_GetPowerNames(item_idx, &Power_Names, &Power_Texts, &Power_Count);



### ITEM_GetPowerNames()
// WZD o78p03
void ITEM_GetPowerNames(int16_t item_idx, char * power_names[], char * power_descriptions[], int16_t * power_count)

attack, tohit, defense, moves2, resistence, spell skill, spell save, embedded spell


char Power_Names[5][20];
char Power_Texts[5][40];
...
ITEM_GetPowerNames()
    (char **)&Power_Names[0]
    (char **)&Power_Texts[0]
...
char * power_names[]
char * power_descriptions[]
...
for(itr = 0; itr < 5; itr++)
{
    strcpy(power_descriptions[itr], "");
}




