/*
    ENCHANT
    Make Item
    ...itemmake (IM)
    
    WIZARDS.EXE
        ovr115
*/

#include "ItemMake.h"

#include "ItemView.h"
#include "MOM_DEF.h"
#include "MOX/Allocate.h"
#include "MOX/FLIC_Draw.h"
#include "MOX/Fields.h"
#include "MOX/Fonts.h"
#include "MOX/GENDRAW.h"
#include "MOX/LBX_Load.h"
#include "MOX/MOM_Data.h"
#include "MOX/MOX_BASE.h"
#include "MOX/MOX_DAT.h"
#include "MOX/MOX_T4.h"
#include "MOX/random.h"

#include "MOX/Util.h"
#include "NEXTTURN.h"
#include "STU/STU_DBG.h"
#include "Spellbook.h"

#include <stdlib.h>
#include <string.h>



#pragma pack(push)
#pragma pack(2)
struct s_ITEM_TYPE_ICONS_ENTRY
{
    int16_t start;
    int16_t count;
};
#pragma pack(pop)



// WZD o115p01
static void Item_Make_Screen_Load(void);
// WZD o115p02
static void Item_Make_Screen_Draw(void);
// WZD o115p03
char * Get_Item_Type_Name(int16_t item_idx);
// WZD o115p04
static const char * Get_Item_Name(int16_t item_idx);
// WZD o115p05
static int16_t Get_Item_Cost(int16_t item_idx);
// WZD o115p06
static void Item_Make_Screen_Build_Spell_List(void);
// WZD o115p07
static void Item_Make_Screen_Add_Fields(void);
// WZD o115p08
int16_t Item_Make_Screen(int16_t player_idx, int16_t type);
// WZD o115p09
static void Create_Item_Record(int16_t item_idx);
// WZD o115p11
static int16_t Itam_Make_Screen_Build_Weapon_Powers_List(int16_t player_idx, int16_t max_cost);
// WZD o115p12
static void Item_Make_Screen_Spellbook_Popup_Draw(void);
// WZD o115p13
static int16_t Item_Make_Screen_Spellbook_Popup(void);
// WZD o115p14
int16_t Add_Item(int16_t itemdata_idx);
// WZD o115p15
int16_t Activate_Item(int16_t old_item_idx);
// WZD o115p16
static void Create_Random_Item(int16_t Power, int16_t Value);
// WZD o115p17
int16_t Make_Item(int16_t Power, int16_t spellranks[], int16_t Value);



// WZD dseg:5BCC                                                 BEGIN:  ovr115 - Initialized Data

// WZD dseg:5BCC
uint8_t COL_DeselectItemName[4] = { 17, 9, 8, 8 };

// WZD dseg:5BD0
uint8_t m_itemmake_colors1[4] = { 0, 177, 178, 179};
// WZD dseg:5BD4
uint8_t m_itemmake_colors2[4] = { 0, 31, 29, 28};

// WZD dseg:5BD8
int16_t m_item_type_base_cost[10] = {100, 100, 100, 100, 300, 200, 50, 100, 100, 300};

// WZD dseg:5BEC
/*
don't know why duplicated in CRP_ITEM_IconRanges_2
Item_Type_Icon_Ranges is used in Item_Make_Screen()
CRP_ITEM_IconRanges_2 is used in Create_Random_Item()
maybe defined locally and compiled moved them out?
*/
struct s_ITEM_TYPE_ICONS_ENTRY ITEM_IconRanges[10] =
{
    {0, 9},
    {9, 11},
    {20, 9},
    {29, 9},
    {38, 9},
    {107, 9},
    {72, 35},
    {62, 10},
    {47, 8},
    {55, 7}
};
// WZD dseg:5C14
struct s_ITEM_TYPE_ICONS_ENTRY CRP_ITEM_IconRanges_2[10] =
{
    {  0,  9},
    {  9, 11},
    { 20,  9},
    { 29,  9},
    { 38,  9},
    {107,  9},
    { 72, 35},
    { 62, 10},
    { 47,  8},
    { 55,  7}
};

// WZD dseg:5C3C
char itempow_lbx_file__ovr115[] = "ITEMPOW";
// WZD dseg:5C43
char empty_string__ovr115[] = "";
// WZD dseg:5C44
char spellscr_lbx_file__ovr115[] = "SPELLSCR";
// WZD dseg:5C4D
char cnst_TimesSign_2[] = " x";
// WZD dseg:5C50
char cnst_Cost_Col[] = "Cost:";
// WZD dseg:5C55
char cnst_Space_4[] = " ";
// WZD dseg:5C57
char cnst_Amulet[] = "Amulet";
// WZD dseg:5C5E
char cnst_Ring[] = "Ring";
// WZD dseg:5C63
char cnst_Cloak[] = "Cloak";
// WZD dseg:5C69
char cnst_Gauntlet[] = "Gauntlet";
// WZD dseg:5C72
char cnst_Helm[] = "Helm";
// WZD dseg:5C77
char cnst_Orb[] = "Orb";
// WZD dseg:5C7B
char cnst_Sword[] = "Sword";
// WZD dseg:5C81
char cnst_Mace[] = "Mace";
// WZD dseg:5C86
char cnst_Axe[] = "Axe";
// WZD dseg:5C8A
char cnst_Bow[] = "Bow";
// WZD dseg:5C8E
char cnst_Staff[] = "Staff";
// WZD dseg:5C94
char cnst_Wand[] = "Wand";
// WZD dseg:5C99
char cnst_Misc[] = "Misc";
// WZD dseg:5C9E
char cnst_Shield[] = "Shield";
// WZD dseg:5CA5
char cnst_ChainMail[] = "Chain";
// WZD dseg:5CB0
char cnst_PlateMail[] = "Plate";
// WZD dseg:5CBB
char cnst_Strength_2[] = "Strength";
// WZD dseg:5CC4
char cnst_Accuracy[] = "Accuracy";
// WZD dseg:5CCD
char cnst_Defense_3[] = "Defense";
// WZD dseg:5CD5
char cnst_Wizardry[] = "Wizardry";
// WZD dseg:5CDE
char cnst_Power[] = "Power";
// WZD dseg:5CE4
char cnst_Speed[] = "Speed";
// WZD dseg:5CEA
char cnst_Protection[] = "Protection";
// WZD dseg:5CF5
char cnst_Plus_4[] = "+";
// WZD dseg:5CF7
char cnst_Sp_Of_Sp_4[] = " of ";
// WZD dseg:5CFC
char cnst_HOTKEY_Esc18[] = "\x1B";
// WZD dseg:5CFE
char message_lbx_file__ovr115[] = "message";
// WZD dseg:5D06
char cnst_Charges[] = "Charges";
// WZD dseg:5D0E
char cnst_HOTKEY_F_3[] = "F";
// WZD dseg:5D10
char cnst_HOTKEY_B_4[] = "B";
// WZD dseg:5D12
char itemdata_lbx_file__ovr115[] = "Itemdata";

// WZD dseg:5D1B 00                                              align 2

// WZD dseg:5D1B                                                 END:  ovr115 - Initialized Data





// WZD dseg:C8B6                                                 BEGIN:  ovr115 - Uninitialized Data

// WZD dseg:C8B6
// ; array of 4 2-state images (unselected - selected)
SAMB_ptr m_itemmake_charges_segs[4];
// WZD dseg:C8BE
SAMB_ptr IMG_IC_SpellCharges;
// WZD dseg:C8C0
int16_t * Some_IC_Alloc_5;
// WZD dseg:C8C2
SAMB_ptr * m_itemmake_weapon_highlight_buttons_seg;
// WZD dseg:C8C4
SAMB_ptr * m_itemmake_weapon_buttons_seg;
// WZD dseg:C8C6
int16_t * ITEM_PwrLabel_States;
// WZD dseg:C8C8
int16_t * m_itemmake_weapon_type_powers_list;

// WZD dseg:C8CA
struct s_ITEM_POWER * _ITEM_POWERS;

// WZD dseg:C8CC
int16_t word_4336C;

// WZD dseg:C8CE
/*
    ¿ 4 arrays of 4 colors - normal, highlight, ... ?
    ~ [4][4] {177,178,179,180}
*/
uint8_t * m_itemmake_item_name_input_colors;
// WZD dseg:C8D0
int16_t m_itemmake_icon_window_left_arrow_button;
// WZD dseg:C8D2
int16_t m_itemmake_item_name_field;
// WZD dseg:C8D4
int16_t * m_itemmake_weapon_power_fields;
// WZD dseg:C8D6
int16_t m_itemmake_icon_window_right_arrow_button;
// WZD dseg:C8D8
int16_t m_itemmake_cost_window_ok_button;
// WZD dseg:C8DA
int16_t * m_itemmake_weapon_fields;

// WZD dseg:C8DC
/*
shared with 'Hire / Merchant' ovr127
DEDU  ¿ should use UV's variable ?

Merchant_Popup_Load()
    ITEM_Draw_Seg = Allocate_Next_Block(_screen_seg, 1200);

*/
SAMB_ptr m_itemmake_item_icon_bitmap_seg;

// WZD dseg:C8DE
int16_t m_itemmake_spellbook_idx;

// WZD dseg:C8E0
int16_t niu_did_select_spell;

// WZD dseg:C8E2
int16_t m_itemmake_item_icon_idx;

// WZD dseg:C8E4
SAMB_ptr m_itemmake_cost_window_ok_seg;

// WZD dseg:C8E6
int16_t m_itemmake_item_powers_count;

// WZD dseg:C8E8
int16_t m_itemmake_item_powers_array[MAX_ITEM_POWERS];

// WZD dseg:C8F0
int16_t m_itemmake_power_list_counts[3];

// WZD dseg:C8F6
int16_t m_itemmake_item_cost;

// WZD dseg:C8F8
int16_t m_itemmake_weapon_type_bits;

// WZD dseg:C8F8                                                 END:  ovr115 - Uninitialized Data



/*
    WIZARDS.EXE  ovr115
*/

// WZD o115p01
// drake178: ITEM_LoadCraftingRes()
/*
; creates the memory allocations and loads the
; resources for the item crafting interface
; NOT ALL VALUES ARE IDENTIFIED YET!
*/
/*

*/
static void Item_Make_Screen_Load(void)
{
    SAMB_ptr Item_Crafting_BG = 0;
    int16_t itr = 0;  // _SI_

    _ITEM_POWERS = (struct s_ITEM_POWER * )Near_Allocate_First(2010);

    m_itemmake_weapon_type_powers_list = (int16_t *)Near_Allocate_Next(108);  // 54 2-byte values

    ITEM_PwrLabel_States = (int16_t *)Near_Allocate_Next(108);  // 54 2-byte values

    m_itemmake_weapon_buttons_seg = (SAMB_ptr *)Near_Allocate_Next((10 * sizeof(SAMB_ptr)));  // was 10 * 2 = 20, now 10 * 8 = 80

    m_itemmake_weapon_fields = (int16_t * )Near_Allocate_Next(20);  // 10 2-byte values

    m_itemmake_item_name_input_colors = (uint8_t *)Near_Allocate_Next(16);  // [4][4] 16, 1-byte, unsigned color values

    m_itemmake_weapon_power_fields = (int16_t *)Near_Allocate_Next(108);  // 54 2-byte values

    m_itemmake_weapon_highlight_buttons_seg = (SAMB_ptr *)Near_Allocate_Next((10 * sizeof(SAMB_ptr)));  // was 10 * 2 = 20, now 10 * 8 = 80

    Some_IC_Alloc_5 = (int16_t *)Near_Allocate_Next(120);

    LBX_Load_Data_Static(itempow_lbx_file__ovr115, 0, (SAMB_ptr)&_ITEM_POWERS[0], 0, 67, 30);

    // SPELLSCR.LBX, 013  "ENCHANT"     ""
    Item_Crafting_BG = LBX_Reload(spellscr_lbx_file__ovr115, 13, _screen_seg);

    Set_Page_Off();

    FLIC_Draw(0, 0, Item_Crafting_BG);

    Copy_Off_To_Back();

    Set_Page_On();

    // SPELLSCR.LBX, 024  "ENCHBUTT"    ""
    m_itemmake_cost_window_ok_seg = LBX_Reload(spellscr_lbx_file__ovr115, 24, _screen_seg);

    for(itr = 0; itr < 16; itr++)
    {

        m_itemmake_item_name_input_colors[itr] = (177 + (itr % 4));

    }

    for(itr = 0; itr < 10; itr++)
    {

        // SPELLSCR.LBX, 014  "ENCHBUTT"    "sword"
        // SPELLSCR.LBX, 015  "ENCHBUTT"    "mace"
        // SPELLSCR.LBX, 016  "ENCHBUTT"    "axe"
        // SPELLSCR.LBX, 017  "ENCHBUTT"    "bow"
        // SPELLSCR.LBX, 018  "ENCHBUTT"    "staff"
        // SPELLSCR.LBX, 019  "ENCHBUTT"    "wand"
        // SPELLSCR.LBX, 020  "ENCHBUTT"    "misc"
        // SPELLSCR.LBX, 021  "ENCHBUTT"    "shield"
        // SPELLSCR.LBX, 022  "ENCHBUTT"    "chain"
        // SPELLSCR.LBX, 023  "ENCHBUTT"    "plate"
        m_itemmake_weapon_buttons_seg[itr] = LBX_Reload_Next(spellscr_lbx_file__ovr115, (14 + itr), _screen_seg);

    }

    for(itr = 0; itr < 10; itr++)
    {

        // SPELLSCR.LBX, 025  "ENCHBUTT"    "sword gold"
        // SPELLSCR.LBX, 026  "ENCHBUTT"    "mace gold"
        // SPELLSCR.LBX, 027  "ENCHBUTT"    "axe gold"
        // SPELLSCR.LBX, 028  "ENCHBUTT"    "bow gold"
        // SPELLSCR.LBX, 029  "ENCHBUTT"    "staff gold"
        // SPELLSCR.LBX, 030  "ENCHBUTT"    "wand gold"
        // SPELLSCR.LBX, 031  "ENCHBUTT"    "misc gold"
        // SPELLSCR.LBX, 032  "ENCHBUTT"    "shield gold"
        // SPELLSCR.LBX, 033  "ENCHBUTT"    "chain gold"
        // SPELLSCR.LBX, 034  "ENCHBUTT"    "plate gold"
        m_itemmake_weapon_highlight_buttons_seg[itr] = LBX_Reload_Next(spellscr_lbx_file__ovr115, (25 + itr), _screen_seg);

    }

    // SPELLSCR.LBX, 035  "ENCHBUTT"    "left pic button"
    IMG_SBK_SliderDot = LBX_Reload_Next(spellscr_lbx_file__ovr115, 35, _screen_seg);

    // SPELLSCR.LBX, 036  "ENCHBUTT"    "right pic button"
    IMG_SBK_SliderBar = LBX_Reload_Next(spellscr_lbx_file__ovr115, 36, _screen_seg);

    // SPELLSCR.LBX, 037  "CHARGES1"    ""
    IMG_IC_SpellCharges = LBX_Reload_Next(spellscr_lbx_file__ovr115, 37, _screen_seg);

    for(itr = 0; itr < 4; itr++)
    {

        // SPELLSCR.LBX, 038  "CHARGES"     "x1"
        // SPELLSCR.LBX, 039  "CHARGES"     "x2"
        // SPELLSCR.LBX, 040  "CHARGES"     "x3"
        // SPELLSCR.LBX, 041  "CHARGES"     "x4"
        m_itemmake_charges_segs[itr] = LBX_Reload_Next(spellscr_lbx_file__ovr115, (38 + itr), _screen_seg);

    }

    m_itemmake_item_icon_bitmap_seg = Allocate_Next_Block(_screen_seg, 30);  // 30 PR  480 B

    niu_did_select_spell = 0;

    m_itemmake_weapon_type_bits = 1;

}


// WZD o115p02
// drake178: sub_94FDD()
static void Item_Make_Screen_Draw(void)
{
    int16_t item_cost = 0;
    int16_t cost_reduction = 0;
    uint8_t * colors2 = 0;
    uint8_t * colors1 = 0;
    int16_t x_start = 0;
    int16_t y_start = 0;
    int16_t itr_current_columns_counts = 0;
    int16_t attribute_column = 0;
    int16_t itr = 0;  // _SI_
    int16_t powers_count = 0;  // _DI_

    // DEDU  why does these look like a 4-byte value? ...Long? Far Pointer?
    colors1 = m_itemmake_colors1;
    colors2 = m_itemmake_colors2;

    Copy_Back_To_Off();

    for(attribute_column = 0; attribute_column < 3; attribute_column++)
    {

        powers_count = 0;

        for(itr_current_columns_counts = 0; itr_current_columns_counts < attribute_column; itr_current_columns_counts++)
        {

            powers_count += m_itemmake_power_list_counts[itr_current_columns_counts];

        }

        if(
            (m_itemmake_weapon_type_powers_list[powers_count] == ST_UNDEFINED)
            &&
            (attribute_column == 2)
        )
        {

            y_start = 36;

        }
        else
        {

            y_start = 41;

        }

        x_start = (10 + (attribute_column * 85));

        if(attribute_column == 2)
        {

            x_start += 17;

        }

        for(itr = 0; m_itemmake_power_list_counts[attribute_column] > itr; itr++)
        {

            if(m_itemmake_weapon_type_powers_list[(powers_count + itr)] == ST_UNDEFINED)
            {

                y_start += 3;

            }
            else
            {

                if(ITEM_PwrLabel_States[(powers_count + itr)] != ST_TRUE)
                {

                    Set_Outline_Color(18);

                    Set_Font_Colors_15(3, &colors2[0]);

                    Set_Font_Style_Shadow_Up(3, 15, 0, 0);

                    if(attribute_column == 2)
                    {

                        Print(x_start, y_start, _ITEM_POWERS[(m_itemmake_weapon_type_powers_list[(powers_count + itr)])].name);

                    }
                    else
                    {

                        _ITEM_POWERS[(m_itemmake_weapon_type_powers_list[(powers_count + itr)])].name[3] = '\0';

                        Print_Right((x_start + 10), y_start, _ITEM_POWERS[(m_itemmake_weapon_type_powers_list[(powers_count + itr)])].name);

                        Print((x_start + 14), y_start, &_ITEM_POWERS[(m_itemmake_weapon_type_powers_list[(powers_count + itr)])].name[4]);

                        _ITEM_POWERS[(m_itemmake_weapon_type_powers_list[(powers_count + itr)])].name[3] = ' ';


                    }

                    Set_Outline_Color(16);

                    Set_Font_Colors_15(3, &colors2[0]);

                    Set_Font_Style_Shadow_Up(3, 15, 0, 0);

                    if(attribute_column == 2)
                    {
                        Print(x_start, y_start, _ITEM_POWERS[(m_itemmake_weapon_type_powers_list[(powers_count + itr)])].name);

                    }
                    else
                    {


                        _ITEM_POWERS[(m_itemmake_weapon_type_powers_list[(powers_count + itr)])].name[3] = '\0';

                        Print_Right((x_start + 10), y_start, _ITEM_POWERS[(m_itemmake_weapon_type_powers_list[(powers_count + itr)])].name);

                        Print((x_start + 14), y_start, &_ITEM_POWERS[(m_itemmake_weapon_type_powers_list[(powers_count + itr)])].name[4]);

                        _ITEM_POWERS[(m_itemmake_weapon_type_powers_list[(powers_count + itr)])].name[3] = ' ';

                    }

                }
                else
                {

                    Set_Outline_Color(18);

                    Set_Font_Colors_15(3, &colors1[0]);

                    Set_Font_Style_Shadow_Up(3, 15, 0, 0);

                    if(m_itemmake_weapon_type_powers_list[(powers_count + itr)] == 65)  // "Spell Charges"
                    {

                        itr_current_columns_counts = Print_Far(x_start, y_start, spell_data_table[_ITEMS[137].embed_spell_idx].name);

                        itr_current_columns_counts = Print(itr_current_columns_counts, y_start, cnst_TimesSign_2);

                        Print_Integer(itr_current_columns_counts, y_start, _ITEMS[137].embed_spell_cnt);

                    }
                    else
                    {

                        if(attribute_column == 2)
                        {

                            Print(x_start, y_start, _ITEM_POWERS[(m_itemmake_weapon_type_powers_list[(powers_count + itr)])].name);

                        }
                        else
                        {

                            _ITEM_POWERS[(m_itemmake_weapon_type_powers_list[(powers_count + itr)])].name[3] = '\0';

                            Print_Right((x_start + 10), y_start, _ITEM_POWERS[(m_itemmake_weapon_type_powers_list[(powers_count + itr)])].name);

                            Print((x_start + 14), y_start, &_ITEM_POWERS[(m_itemmake_weapon_type_powers_list[(powers_count + itr)])].name[4]);

                            _ITEM_POWERS[(m_itemmake_weapon_type_powers_list[(powers_count + itr)])].name[3] = ' ';


                        }

                    }

                    Set_Outline_Color(16);

                    Set_Font_Colors_15(3, &colors1[0]);

                    Set_Font_Style_Shadow_Up(3, 15, 0, 0);

                    if(m_itemmake_weapon_type_powers_list[(powers_count + itr)] == 65)  // "Spell Charges"
                    {

                        itr_current_columns_counts = Print_Far(x_start, y_start, spell_data_table[_ITEMS[137].embed_spell_idx].name);

                        itr_current_columns_counts = Print(itr_current_columns_counts, y_start, cnst_TimesSign_2);

                        Print_Integer(itr_current_columns_counts, y_start, _ITEMS[137].embed_spell_cnt);

                    }
                    else
                    {

                        if(attribute_column == 2)
                        {
                            Print(x_start, y_start, _ITEM_POWERS[(m_itemmake_weapon_type_powers_list[(powers_count + itr)])].name);

                        }
                        else
                        {

                            _ITEM_POWERS[(m_itemmake_weapon_type_powers_list[(powers_count + itr)])].name[3] = '\0';

                            Print_Right((x_start + 10), y_start, _ITEM_POWERS[(m_itemmake_weapon_type_powers_list[(powers_count + itr)])].name);

                            Print((x_start + 14), y_start, &_ITEM_POWERS[(m_itemmake_weapon_type_powers_list[(powers_count + itr)])].name[4]);

                            _ITEM_POWERS[(m_itemmake_weapon_type_powers_list[(powers_count + itr)])].name[3] = ' ';

                        }
                        
                    }

                }

                y_start += 11;

            }

        }

    }

    Set_Font_Colors_15(3, &COL_DeselectItemName[0]);

    Set_Font_Style_Shadow_Down(3, 15, 4, 4);

    Set_Outline_Color(17);

    x_start = Print(198, 185, cnst_Cost_Col);

    cost_reduction = Casting_Cost_Reduction(HUMAN_PLAYER_IDX, spl_Enchant_Item);

    item_cost = m_itemmake_item_cost;

    item_cost -= ((item_cost * cost_reduction) / 100);

    // x_start = Print_Integer((x_start - 1), 185, item_cost);
    /* HACK */  x_start = Print_Integer((x_start + 4), 185, item_cost);

    Draw_Item_Icon_With_Enchantment_Outline(137, m_itemmake_item_icon_bitmap_seg);

    Draw_Picture(4, 5, m_itemmake_item_icon_bitmap_seg);

    Cycle_Item_Enchantment_Animation();

}


// WZD o115p03
/*
; returns a pointer to the near string that corresponds to the base item type name for the item's icon
Sword, Mace, Axe, Bow, Staff, Wand, Misc, Shield, Chain, Plate, Amulet, Ring, Cloak, Gauntlet, Helm, Orb
*/
char * Get_Item_Type_Name(int16_t item_idx)
{
    char * Misc_Item_Names[6];
    char * Base_Item_Names[10];
    int16_t item_icon_idx;  // _SI_

    Misc_Item_Names[0] = cnst_Amulet;
    Misc_Item_Names[1] = cnst_Ring;
    Misc_Item_Names[2] = cnst_Cloak;
    Misc_Item_Names[3] = cnst_Gauntlet;
    Misc_Item_Names[4] = cnst_Helm;
    Misc_Item_Names[5] = cnst_Orb;

    Base_Item_Names[0] = cnst_Sword;
    Base_Item_Names[1] = cnst_Mace;
    Base_Item_Names[2] = cnst_Axe;
    Base_Item_Names[3] = cnst_Bow;
    Base_Item_Names[4] = cnst_Staff;
    Base_Item_Names[5] = cnst_Wand;
    Base_Item_Names[6] = cnst_Misc;
    Base_Item_Names[7] = cnst_Shield;
    Base_Item_Names[8] = cnst_ChainMail;
    Base_Item_Names[9] = cnst_PlateMail;

    item_icon_idx = _ITEMS[item_idx].icon_idx;

    if(_ITEMS[item_idx].type == it_Misc)
    {
        if(item_icon_idx <= 77)
        {
            return Misc_Item_Names[0];
        }
        else if(item_icon_idx <= 83)
        {
            return Misc_Item_Names[1];
        }
        if(item_icon_idx <= 89)
        {
            return Misc_Item_Names[2];
        }
        else if(item_icon_idx <= 93)
        {
            return Misc_Item_Names[3];
        }
        else if(item_icon_idx <= 100)
        {
            return Misc_Item_Names[4];
        }
        else
        {
            return Misc_Item_Names[5];
        }
    }

    return Base_Item_Names[_ITEMS[item_idx].type];
}


// WZD o115p04
/*

; returns a volatile pointer to a default name generated from the item's selected powers

*/
static const char * Get_Item_Name(int16_t item_idx)
{
    char spell_name[LEN_SPELL_NAME];
    char * Attrib_Suffixes[7];
    static char item_name[LEN_ITEM_NAME];
    char temp_buffer[LEN_TEMP_BUFFER];
    int16_t Have_Prefix;
    int16_t Unprocessed_Powers[NUM_ITEM_POWERS];
    int16_t Highest_Index_Power;
    int16_t item_type;
    int16_t item_type_bit_idx;  // _DI_
    int16_t itr;  // _DI_


    Attrib_Suffixes[0] = cnst_Strength_2;
    Attrib_Suffixes[1] = cnst_Accuracy;
    Attrib_Suffixes[2] = cnst_Defense_3;
    Attrib_Suffixes[3] = cnst_Wizardry;
    Attrib_Suffixes[4] = cnst_Power;
    Attrib_Suffixes[5] = cnst_Speed;
    Attrib_Suffixes[6] = cnst_Protection;


    item_type_bit_idx = m_itemmake_weapon_type_bits;
    item_type = 0;
    while(item_type_bit_idx > 1)
    {
        item_type++;
        item_type_bit_idx = (item_type_bit_idx >> 1);
    }

    for(itr = 0; itr < NUM_ITEM_POWERS; itr++)
    {
        Unprocessed_Powers[itr] = m_itemmake_item_powers_array[itr];
    }


    Have_Prefix = ST_FALSE;

    for(itr = 0; itr < NUM_ITEM_POWERS; itr++)
    {
        if(
            (Unprocessed_Powers[itr] > -1)
            &&
            (Unprocessed_Powers[itr] < 6)
            &&
            (item_type < it_Misc)
        )
        {
            strcpy(item_name, cnst_Plus_4);
            itoa((Unprocessed_Powers[itr] + 1), temp_buffer, 10);
            strcat(item_name, temp_buffer);
            strcat(item_name, cnst_Space_4);
            strcat(item_name, Get_Item_Type_Name(item_idx));  // Sword, Mace, Axe, Bow, Staff, Wand, Misc, Shield, Chain, Plate, Amulet, Ring, Cloak, Gauntlet, Helm, Orb

            Unprocessed_Powers[itr] = -2;

            Have_Prefix = ST_TRUE;
        }
    }


    if(Have_Prefix != ST_TRUE)
    {
        if(item_type == it_Misc)
        {
            // jmp     short $+2
        }
        strcpy(item_name, Get_Item_Type_Name(item_idx));
    }


    Highest_Index_Power = -1;

    for(itr = 0; itr < NUM_ITEM_POWERS; itr++)
    {
        if(Unprocessed_Powers[itr] > Highest_Index_Power)
        {
            Highest_Index_Power = Unprocessed_Powers[itr];
        }
    }

    if(Highest_Index_Power > -1)
    {
        strcat(item_name, cnst_Sp_Of_Sp_4);

        if(Highest_Index_Power < 33)
        {
            strcat(item_name, Attrib_Suffixes[_ITEM_POWERS[Highest_Index_Power].type]);
        }
        else if(Highest_Index_Power < 65)
        {
            strcat(item_name, _ITEM_POWERS[Highest_Index_Power].name);
        }
        else
        {
            strcpy(spell_name, spell_data_table[_ITEMS[item_idx].embed_spell_idx].name);
            strcat(item_name, spell_name);
            if(strlen(item_name) < 26)
            {
                strcat(item_name, cnst_TimesSign_2);
                itoa(_ITEMS[item_idx].embed_spell_cnt, temp_buffer, 10);
                strcat(item_name, temp_buffer);
            }
        }

    }

    if(Have_Prefix == ST_TRUE)
    {
        if(strlen(item_name) > 26)
        {
            for(itr = 0; itr < 3; itr++)
            {
                Clear_Structure(0, (uint8_t *)&item_name, 1, 30);
            }
        }
    }

    return item_name;
}


// WZD o115p05
/*
; calculates and returns the total cost of crafting the
; selected item, accounting for its base cost (but not
; any retorts)
*/
static int16_t Get_Item_Cost(int16_t item_idx)
{
    int16_t item_type_base_cost[10];
    int16_t item_type;
    int16_t item_type_bit_idx;
    int16_t itr;  // _SI_
    int16_t item_cost;  // _DI_

    memcpy(item_type_base_cost, m_item_type_base_cost, 20);

    item_type_bit_idx = m_itemmake_weapon_type_bits;
    item_type = 0;
    while(item_type_bit_idx > 1)
    {
        item_type++;
        item_type_bit_idx = (item_type_bit_idx >> 1);
    }

    item_cost = item_type_base_cost[item_type];

    for(itr = 0; itr < NUM_ITEM_POWERS; itr++)
    {
        if(m_itemmake_item_powers_array[itr] > -1)
        {
            if(m_itemmake_item_powers_array[itr] == 65)
            {
                item_cost += (spell_data_table[_ITEMS[item_idx].embed_spell_idx].casting_cost * _ITEMS[item_idx].embed_spell_cnt * 20);
            }
            else
            {
                if(
                    (item_type == it_Misc)
                    &&
                    (m_itemmake_item_powers_array[itr] < 33)
                )
                {
                    (item_cost += _ITEM_POWERS[m_itemmake_item_powers_array[itr]].cost * 2);
                }
                else
                {
                    item_cost += _ITEM_POWERS[m_itemmake_item_powers_array[itr]].cost;
                }
            }
        }
    }

    return item_cost;
}


// WZD o115p06
// drake178: sub_95B62()
static void Item_Make_Screen_Build_Spell_List(void)
{
    int16_t var_5 = 0;
    int16_t var_4 = 0;
    int16_t itr2 = 0;
    int16_t itr1 = 0;  // _CX_

    m_spell_list_count = 0;

    for(itr1 = 0; itr1 < 92; itr1++)
    {

        Some_IC_Alloc_5[itr1] = 0;

    }

    for(itr1 = 0; itr1 < NUM_MAGIC_REALMS; itr1++)
    {

        for(itr2 = 0; itr2 < NUM_SPELLS_PER_MAGIC_REALM; itr2++)
        {

            var_4 = 0;

            var_5 = (((itr1 * NUM_SPELLS_PER_MAGIC_REALM) + itr2) + 1);

            if(_players[HUMAN_PLAYER_IDX].spells_list[((itr1 * NUM_SPELLS_PER_MAGIC_REALM) + itr2)] == 2)
            {

                if(
                    (spell_data_table[var_5].Eligibility != 1)
                    &&
                    (spell_data_table[var_5].Eligibility != 5)
                )
                {

                    Some_IC_Alloc_5[m_spell_list_count] = var_5;

                    m_spell_list_count++;

                }

            }

        }

    }


}


// WZD o115p07
// drake178: ITEM_CreateControls()
/*
; creates the controls for the item creation screen,
; including the name edit box and all the buttons and
; power labels, setting inactive ones to index -1000
*/
/*
 2 icon window arrows
 1 item name window input field
 ? attribute enchantments
 ? spell-specific enchantments
 1 cost window ok button
10 weapon buttons
*/
static void Item_Make_Screen_Add_Fields(void)
{
    int16_t x_start = 0;
    int16_t FullList_LoopVar = 0;
    int16_t itr_current_columns_counts = 0;
    int16_t itr = 0;
    int16_t itr_columns = 0;  // _SI_
    int16_t y_start = 0;  // _DI_

    Clear_Fields();

    Set_Font_Colors_15(1, COL_DeselectItemName);

    Set_Font_Style(1, 15, 4, 4);

    Set_Alias_Color(17);

    // item name window
    m_itemmake_item_name_field = Add_Input_Field(30, 13, 118, &GUI_NearMsgString[0], LEN_ITEM_MAKE_NAME, 0, 0, 0, m_itemmake_item_name_input_colors, empty_string__ovr115, ST_UNDEFINED);

    // weapon buttons
    for(itr = 0; itr < 10; itr++)
    {

        if((m_itemmake_weapon_type_bits & (1 << itr)) != 0)
        {

            // selected / highlighted
            m_itemmake_weapon_fields[itr] = Add_Button_Field((156 + (33 * (itr % 5))), (3 + (15 * (itr / 5))), empty_string__ovr115, m_itemmake_weapon_highlight_buttons_seg[itr], empty_string__ovr115[0], ST_UNDEFINED);

        }
        else
        {

            // deselected / normal
            m_itemmake_weapon_fields[itr] = Add_Button_Field((156 + (33 * (itr % 5))), (3 + (15 * (itr / 5))), empty_string__ovr115, m_itemmake_weapon_buttons_seg[itr], empty_string__ovr115[0], ST_UNDEFINED);

        }

    }

    m_itemmake_cost_window_ok_button = Add_Button_Field(281, 180, empty_string__ovr115, m_itemmake_cost_window_ok_seg, cnst_HOTKEY_Esc18[0], ST_UNDEFINED);

    m_itemmake_icon_window_left_arrow_button = Add_Button_Field(4, 24, empty_string__ovr115, m_itemmake_icon_window_left_arrow_seg, empty_string__ovr115[0], ST_UNDEFINED);

    m_itemmake_icon_window_right_arrow_button = Add_Button_Field(17, 24, empty_string__ovr115, m_itemmake_icon_window_right_arrow_seg, empty_string__ovr115[0], ST_UNDEFINED);

    // 
    for(itr_columns = 0; itr_columns < 3; itr_columns++)
    {

        FullList_LoopVar = 0;

        for(itr_current_columns_counts = 0; itr_current_columns_counts < itr_columns; itr_current_columns_counts++)
        {

            FullList_LoopVar += m_itemmake_power_list_counts[itr_current_columns_counts];

        }

        if(
            (m_itemmake_weapon_type_powers_list[FullList_LoopVar] == ST_UNDEFINED)
            &&
            (itr_columns == 2)
        )
        {

            y_start = 36;

        }
        else
        {

            y_start = 41;

        }

        x_start = (10 + (itr_columns * 85));

        if(itr_columns == 2)
        {
            
            x_start += 17;

        }

        for(itr = 0; m_itemmake_power_list_counts[itr_columns] > itr; itr++)
        {

            if(m_itemmake_weapon_type_powers_list[(FullList_LoopVar + itr)] == ST_UNDEFINED)
            {

                m_itemmake_weapon_power_fields[(FullList_LoopVar + itr)] = INVALID_FIELD;

                y_start += 3;

            }
            else
            {

                if(itr_columns != 2)
                {

                    m_itemmake_weapon_power_fields[(FullList_LoopVar + itr)] = Add_Hidden_Field(x_start, (y_start - 2), (x_start + 78),(y_start + 7), empty_string__ovr115[0], ST_UNDEFINED);

                    y_start += 11;

                }
                else
                {

                    m_itemmake_weapon_power_fields[(FullList_LoopVar + itr)] = Add_Hidden_Field(x_start, (y_start - 2), (x_start + 98),(y_start + 7), empty_string__ovr115[0], ST_UNDEFINED);

                    y_start += 11;

                }

            }

        }

    }

}

// WZD o115p08
// drake178: sub_95E65()
/*

if(spell_data_table[spell_idx].type == scc_Crafting_Spell)
    if(spell_idx == spl_Enchant_Item)
        _players[player_idx].casting_cost_remaining = IDK_CreateArtifact(0, 0);
    else if(spell_idx == spl_Create_Artifact)
        _players[player_idx].casting_cost_remaining = IDK_CreateArtifact(0, 1);

*/
int16_t Item_Make_Screen(int16_t player_idx, int16_t type)
{
    struct s_ITEM_TYPE_ICONS_ENTRY Item_Type_Icon_Ranges[10] = { {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0} };
    char message[LEN_MESSAGE_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t weapon_type_num = 0;
    int16_t itr2 = 0;
    int16_t cost_reduction = 0;
    int16_t G_Removed_Power = 0;
    int16_t in_text_edit = 0;
    int16_t leave_screen = 0;
    int16_t input_field_idx = 0;
    int16_t var_6 = 0;
    int16_t itr_field_arrays = 0;  // used to iter on powers fields and weapons fields
    int16_t max_power_cost = 0;
    int16_t itr = 0;  // _DI_
    int16_t item_power_type = 0;  // _SI_

    // F_SCOPY@(ITEM_IconRanges, Item_Type_Icon_Ranges, 40)
    memcpy(Item_Type_Icon_Ranges, ITEM_IconRanges, (10 * sizeof(struct s_ITEM_TYPE_ICONS_ENTRY)));
    
    // MEM_Clear_Far(_ITEMS, 50);
    memset(_ITEMS, 0, sizeof(struct s_ITEM));

    if(type != 0)  /* Create Artifact */
    {

        max_power_cost = 10000;

    }
    else  /* Enchant Item */
    {

        max_power_cost = 200;

    }
    
    m_itemmake_item_icon_idx = 0;

    weapon_type_num = it_Sword;

    m_itemmake_weapon_type_bits = 1;  // meaningless value or actually the correct bits for sword?

    in_text_edit = ST_FALSE;

    Item_Make_Screen_Load();

    Itam_Make_Screen_Build_Weapon_Powers_List(player_idx, max_power_cost);

    Assign_Auto_Function(Item_Make_Screen_Draw, 2);

    m_itemmake_item_powers_count = 0;

    for(itr = 0; itr < 4; itr++)
    {

        m_itemmake_item_powers_array[itr] = -2;

    }

    _page_flip_effect = pfe_Dissolve;

    leave_screen = ST_FALSE;

    for(itr2 = 0; itr2 < 54; itr2++)
    {

        ITEM_PwrLabel_States[itr2] = 0;

    }

    m_itemmake_item_cost = Get_Item_Cost(137);

    // BUGBUG  extra, unused parameter?  strcpy(GUI_NearMsgString, Get_Item_Name(137, 0));
    strcpy(GUI_NearMsgString, Get_Item_Name(137));

    Create_Item_Record(137);

    Item_Make_Screen_Add_Fields();

    while(leave_screen == ST_FALSE)
    {

        input_field_idx = Get_Input();


        if(input_field_idx == m_itemmake_cost_window_ok_button)
        {

            leave_screen = ST_TRUE;

        }


        /*
            BEGIN: Left-Click Item Power
        */
        for(itr_field_arrays = 0; (m_itemmake_power_list_counts[0] + m_itemmake_power_list_counts[1] + m_itemmake_power_list_counts[2]) > itr_field_arrays; itr_field_arrays++)
        {

            if(m_itemmake_weapon_power_fields[itr_field_arrays] == input_field_idx)
            {


                if(_ITEM_POWERS[m_itemmake_weapon_type_powers_list[itr_field_arrays]].type != ipt_Special)
                {

                    var_6 = _ITEM_POWERS[m_itemmake_weapon_type_powers_list[itr_field_arrays]].type;

                    itr2 = 0;
                    while(_ITEM_POWERS[m_itemmake_weapon_type_powers_list[itr2]].type != var_6)
                    {
                        itr2++;
                    }

                    while(_ITEM_POWERS[m_itemmake_weapon_type_powers_list[itr2++]].type == var_6)
                    {
                    
                        if(itr_field_arrays != itr2)
                        {

                            if(ITEM_PwrLabel_States[itr2] > 0)
                            {

                                m_itemmake_item_powers_count--;

                                for(itr = 0; itr < 4; itr++)
                                {

                                    if(m_itemmake_item_powers_array[itr] == m_itemmake_weapon_type_powers_list[itr2])
                                    {

                                        _ITEMS[137].Powers ^= _ITEM_POWERS[m_itemmake_weapon_type_powers_list[itr2]].powers;

                                        Clear_Structure(itr, (uint8_t *)&m_itemmake_item_powers_array[0], 2, 4);

                                        m_itemmake_item_powers_array[3] = -2;

                                    }

                                }

                            }
                            else
                            {

                                ITEM_PwrLabel_States[itr2] = 0;

                            }

                        }

                    }

                }

                for(itr = 0; itr < 4; itr++)
                {

                    if(m_itemmake_item_powers_array[itr] == m_itemmake_weapon_type_powers_list[itr_field_arrays])
                    {

                        _ITEMS[137].Powers ^= m_itemmake_weapon_type_powers_list[itr_field_arrays];

                        Clear_Structure(itr, (uint8_t *)&m_itemmake_item_powers_array[0], 2, 4);

                        m_itemmake_item_powers_array[3] = -2;

                        m_itemmake_item_powers_count--;

                        TOGGLE(ITEM_PwrLabel_States[itr_field_arrays]);

                        G_Removed_Power = ST_TRUE;

                    }

                }

                if(m_itemmake_item_powers_count == 4)
                {

                    Clear_Fields();

                    LBX_Load_Data_Static(message_lbx_file__ovr115, 0, (SAMB_ptr)&message[0], 0, 1, 150);  // "only 4 powers may be enchanted into an item"

                    Warn0(message);

                }
                else
                {

                    if(G_Removed_Power != ST_TRUE)
                    {

                        if(m_itemmake_weapon_type_powers_list[itr_field_arrays] != 65)
                        {

                            m_itemmake_item_powers_array[m_itemmake_item_powers_count] = m_itemmake_weapon_type_powers_list[itr_field_arrays];

                            m_itemmake_item_powers_count++;

                            TOGGLE(ITEM_PwrLabel_States[itr_field_arrays]);

                        }
                        else
                        {

                            Item_Make_Screen_Build_Spell_List();

                            itr2 = Item_Make_Screen_Spellbook_Popup();

                            if(itr2 > 0)
                            {

                                _ITEMS[137].embed_spell_idx = itr2;

                                niu_did_select_spell = ST_TRUE;

                                m_itemmake_item_powers_array[m_itemmake_item_powers_count] = m_itemmake_weapon_type_powers_list[itr_field_arrays];

                                m_itemmake_item_powers_count++;

                                TOGGLE(ITEM_PwrLabel_States[itr_field_arrays]);

                            }
                            else
                            {

                                _ITEMS[137].embed_spell_cnt = 0;

                                _ITEMS[137].embed_spell_idx = 0;

                                niu_did_select_spell = ST_FALSE;

                            }

                            Assign_Auto_Function(Item_Make_Screen_Draw, 2);

                        }

                    }

                }

                G_Removed_Power = ST_FALSE;

                m_itemmake_item_cost = Get_Item_Cost(137);

            }

        }
        /*
            END: Left-Click Item Power
        */


        /*
            BEGIN:  Weapon Buttons
        */
        for(itr_field_arrays = 0; itr_field_arrays < 10; itr_field_arrays++)
        {

            if(m_itemmake_weapon_fields[itr_field_arrays] == input_field_idx)
            {

                m_itemmake_weapon_type_bits = (1 << itr_field_arrays);

                itr2 = m_itemmake_weapon_type_bits;

                weapon_type_num = 0;

                while(itr2 > 1)
                {

                    weapon_type_num += 1;

                    itr2 >>= 1;

                }

                m_itemmake_item_icon_idx = Item_Type_Icon_Ranges[weapon_type_num].start;

                _ITEMS[137].icon_idx = m_itemmake_item_icon_idx;

                Itam_Make_Screen_Build_Weapon_Powers_List(player_idx, max_power_cost);

                for(itr2 = 0; (m_itemmake_power_list_counts[0] + m_itemmake_power_list_counts[1] + m_itemmake_power_list_counts[2]) > itr2; itr2++)
                {

                    ITEM_PwrLabel_States[itr2] = ST_FALSE;

                    for(itr = 0; itr < 4; itr++)
                    {

                        if(m_itemmake_weapon_type_powers_list[itr2] == m_itemmake_item_powers_array[itr])
                        {

                            m_itemmake_item_powers_array[itr] += 100;

                            ITEM_PwrLabel_States[itr2] = ST_TRUE;

                        }

                    }

                }

                for(itr = 0; itr < 4; itr++)
                {

                    if(m_itemmake_item_powers_array[itr] >= 100)
                    {

                        m_itemmake_item_powers_array[itr] -= 100;

                    }
                    else
                    {

                        if(m_itemmake_item_powers_array[itr] > -1)
                        {

                            if(m_itemmake_item_powers_array[itr] == 65)
                            {

                                _ITEMS[137].embed_spell_cnt = 0;

                                _ITEMS[137].embed_spell_idx = 0;

                            }

                            _ITEMS[137].Powers ^= m_itemmake_item_powers_array[itr];

                            Clear_Structure(itr, (uint8_t *)&m_itemmake_item_powers_array[0], 2, 4);

                            itr--;

                            m_itemmake_item_powers_array[3] = -2;

                            m_itemmake_item_powers_count--;

                        }

                    }

                }

                m_itemmake_item_cost = Get_Item_Cost(137);

            }

        }
        /*
            END:  Weapon Buttons
        */


        // item name window
        if(input_field_idx == m_itemmake_item_name_field)
        {

            in_text_edit = ST_TRUE;

        }


        // icon window
        if(input_field_idx == m_itemmake_icon_window_left_arrow_button)
        {

            m_itemmake_item_icon_idx--;

            if(Item_Type_Icon_Ranges[weapon_type_num].start > m_itemmake_item_icon_idx)
            {
                m_itemmake_item_icon_idx = Item_Type_Icon_Ranges[weapon_type_num].count;
            }

        }

        if(input_field_idx == m_itemmake_icon_window_right_arrow_button)
        {
            
            m_itemmake_item_icon_idx++;

            if((Item_Type_Icon_Ranges[weapon_type_num].start + Item_Type_Icon_Ranges[weapon_type_num].count) == m_itemmake_item_icon_idx)
            {
                m_itemmake_item_icon_idx = Item_Type_Icon_Ranges[weapon_type_num].start;
            }

        }


        Item_Make_Screen_Add_Fields();

        Create_Item_Record(137);

        if(in_text_edit == ST_FALSE)
        {

            // BUGBUG  extra, unused parameter?  strcpy(GUI_NearMsgString, Get_Item_Name(137, 0));
            strcpy(GUI_NearMsgString, Get_Item_Name(137));

        }

        if(leave_screen == ST_FALSE)
        {
            
            Set_Page_Off();

            Item_Make_Screen_Draw();

            Draw_Fields();

            PageFlip_FX();

            _page_flip_effect = pfe_None;

        }

        if(leave_screen != ST_FALSE)
        {

            cost_reduction = Casting_Cost_Reduction(HUMAN_PLAYER_IDX, spl_Enchant_Item);

            m_itemmake_item_cost -= ((m_itemmake_item_cost * cost_reduction) / 100);

            if(m_itemmake_item_powers_count == 0)
            {

                LBX_Load_Data_Static(message_lbx_file__ovr115, 0, (SAMB_ptr)&message[0], 1, 1, 150);

                leave_screen = Confirmation_Box(message);

                m_itemmake_item_cost = 0;

                if(leave_screen != ST_FALSE)
                {
                    break;
                }

            }

        }

    }

    Clear_Fields();

    return m_itemmake_item_cost;

}

// WZD o115p09
static void Create_Item_Record(int16_t item_idx)
{
    int16_t embed_spell_cnt;
    int16_t embed_spell_idx;
    int16_t weapon_type_num;
    int16_t weapon_type_bits;  // _DI_
    int16_t itr;  // _DI_

    embed_spell_idx = _ITEMS[item_idx].embed_spell_idx;

    embed_spell_cnt = _ITEMS[item_idx].embed_spell_cnt;

    memset(&_ITEMS[item_idx], 0, sizeof(_ITEMS[0]));

    strcpy(_ITEMS[item_idx].name, GUI_NearMsgString);

    _ITEMS[item_idx].embed_spell_idx = embed_spell_idx;

    _ITEMS[item_idx].embed_spell_cnt = embed_spell_cnt;

    weapon_type_bits = m_itemmake_weapon_type_bits;

    weapon_type_num = 0;

    while(weapon_type_bits > 1)
    {

        weapon_type_num++;

        weapon_type_bits = (weapon_type_bits >> 1);

    }

    _ITEMS[item_idx].icon_idx = m_itemmake_item_icon_idx;

    _ITEMS[item_idx].type = weapon_type_num;

    _ITEMS[item_idx].cost = m_itemmake_item_cost;

    switch(weapon_type_num)
    {

        case it_Sword:
        case it_Mace:
        case it_Axe:
        {
            _ITEMS[item_idx].Slot_Type = 1;  /* Sword_Slot */
        } break;

        case it_Bow:
        {
            _ITEMS[item_idx].Slot_Type = 2;  /* Bow_Slot */
        } break;

        case it_Staff:
        case it_Wand:
        {
            _ITEMS[item_idx].Slot_Type = 4;  /* Staff_Slot */
        } break;

        case it_Misc:
        {
            _ITEMS[item_idx].Slot_Type = 6;  /* Misc_Slot */
        } break;

        case it_Shield:
        case it_Chainmail:
        case it_Platemail:
        {
            _ITEMS[item_idx].Slot_Type = 5;  /* Armor_Slot */
        } break;

        default:
        {
            STU_DEBUG_BREAK();
        } break;

    }

    for(itr = 0; itr < NUM_ITEM_POWERS; itr++)
    {
        
        if(m_itemmake_item_powers_array[itr] > -1)
        {
            if(m_itemmake_item_powers_array[itr] < 6)
            {
                _ITEMS[item_idx].attack = (m_itemmake_item_powers_array[itr] + 1);
            }
            else if(m_itemmake_item_powers_array[itr] < 12)
            {
                _ITEMS[item_idx].defense = (m_itemmake_item_powers_array[itr] - 5);
            }
            else if(m_itemmake_item_powers_array[itr] < 15)
            {
                _ITEMS[item_idx].tohit = (m_itemmake_item_powers_array[itr] - 11);
            }
            else if(m_itemmake_item_powers_array[itr] < 19)
            {
                _ITEMS[item_idx].moves2 = ((m_itemmake_item_powers_array[itr] - 14) * 2);
            }
            else if(m_itemmake_item_powers_array[itr] < 25)
            {
                _ITEMS[item_idx].resistance = (m_itemmake_item_powers_array[itr] - 18);
            }
            else if(m_itemmake_item_powers_array[itr] < 29)
            {
                _ITEMS[item_idx].spell_skill = ((m_itemmake_item_powers_array[itr] - 24) * 5);
            }
            else if(m_itemmake_item_powers_array[itr] < 33)
            {
                _ITEMS[item_idx].spell_save = (m_itemmake_item_powers_array[itr] - 28);
            }
            else
            {
                _ITEMS[item_idx].Powers |= _ITEM_POWERS[m_itemmake_item_powers_array[itr]].powers;
            }
        }
    }

    if(weapon_type_num == it_Chainmail)
    {
        _ITEMS[item_idx].defense += 1;
    }

    if(weapon_type_num == it_Platemail)
    {
        _ITEMS[item_idx].defense += 2;
    }

}


// WZD o115p11
// ITEM_Get_IC_Powers()
/*
; sets up the global arrays for displaying the item
; powers that the player can enchant into an item of
; the type specified by ITEM_Current_TypeBit
*/
/*

called when a weapon button field is matched

*/
static int16_t Itam_Make_Screen_Build_Weapon_Powers_List(int16_t player_idx, int16_t max_power_cost)
{
    int16_t last_powers_count = 0;
    int16_t prev_weapon_type = 0;
    int16_t power_count = 0;  // _DI_
    int16_t column_idx = 0;  // _SI_
    int16_t itr = 0;  // _CX_

    power_count = ST_FALSE;

    last_powers_count = 0;

    column_idx = 0;

// clear the counts per column
    for(itr = 0; itr < 3; itr++)
    {

        m_itemmake_power_list_counts[itr] = 0;

    }

    for(itr = 0; itr < NUM_ITEM_POWER_BASIC; itr++)
    {

        if((_ITEM_POWERS[itr].item_types & m_itemmake_weapon_type_bits) > 0)
        {

            if(_ITEM_POWERS[itr].cost <= max_power_cost)
            {

                if(power_count != 0)
                {

                    if(_ITEM_POWERS[itr].type != prev_weapon_type)
                    {

                        m_itemmake_weapon_type_powers_list[power_count] = ST_UNDEFINED;

                        last_powers_count = power_count;

                        m_itemmake_power_list_counts[column_idx] += 1;

                        power_count++;

                    }

                }

                prev_weapon_type = _ITEM_POWERS[itr].type;

                m_itemmake_power_list_counts[column_idx] += 1;

                if(m_itemmake_power_list_counts[column_idx] > 17)
                {

                    if(m_itemmake_weapon_type_powers_list[(power_count - 1)] == ST_UNDEFINED)
                    {
                        
                        power_count--;

                        m_itemmake_power_list_counts[column_idx] -= 1;

                    }

                }

                if(m_itemmake_power_list_counts[column_idx] > 16)
                {

                    // put it in the second column
                    m_itemmake_power_list_counts[(1 + column_idx)] = (m_itemmake_power_list_counts[column_idx] - last_powers_count);

                    // mark the end of the first column
                    m_itemmake_power_list_counts[column_idx] = last_powers_count;

                    column_idx++;

                }

                m_itemmake_weapon_type_powers_list[power_count] = itr;

                power_count++;

            }

        }

    }

    column_idx = 2;

    for(itr = 33; itr < 66; itr++)
    {

        if((_ITEM_POWERS[itr].item_types & m_itemmake_weapon_type_bits) > 0)
        {

            if(
                (_players[player_idx].spellranks[_ITEM_POWERS[itr].magic_realm] >= _ITEM_POWERS[itr].Books_or_Bonus)
                &&
                _ITEM_POWERS[itr].cost <= max_power_cost
            )
            {

                m_itemmake_weapon_type_powers_list[power_count] = itr;

                m_itemmake_power_list_counts[column_idx] += 1;

                power_count++;

            }

        }

    }

    return power_count;

}


// WZD o115p12
static void Item_Make_Screen_Spellbook_Popup_Draw(void)
{
    uint8_t colors2[5] = { 0, 0, 0, 0, 0 };
    uint8_t colors1[5] = { 0, 0, 0, 0, 0 };
    int16_t var_6 = 0;
    int16_t var_4 = 0;
    int16_t y_start = 0;
    int16_t x_start = 0;  // _DI_
    int16_t itr = 0;  // _SI_

    x_start = 28;
    y_start = 12;

    Item_Make_Screen_Draw();

    Reset_Animation_Frame(m_itemmake_cost_window_ok_seg);

    FLIC_Draw(281, 180, m_itemmake_cost_window_ok_seg);

    FLIC_Draw(x_start, y_start, _spellbook_small_seg);

    colors1[0] =  19;
    colors1[1] = 177;
    colors1[2] = 178;
    colors1[3] = 179;
    colors1[4] = 179;

    Set_Outline_Color(0);

    Set_Alias_Color(19);

    Set_Font_Colors_15(4, &colors1[0]);

    Set_Font_Style_Outline(4, 15, ST_NULL, ST_NULL);

    LBX_Load_Data_Static(message_lbx_file__ovr115, 0, (SAMB_ptr)&GUI_NearMsgString[0], 2, 1, 150);  // "Choose a spell to embed in this item"

    Print_Centered(160, 2, GUI_NearMsgString);

    for(itr = 1; itr < 5; itr++)
    {

        colors1[itr] = 39;

    }

    for(itr = 1; itr < 5; itr++)
    {

        colors2[itr] = 166;

    }

    if(word_4336C > 0)
    {

        FLIC_Draw((x_start + 13), (y_start + 4), _spellbook_small_left_corner_seg);

    }

    if((word_4336C + 28) < m_spell_list_count)
    {

        FLIC_Draw((x_start + 258), (y_start + 4), _spellbook_small_right_corner_seg);

    }

    for(itr = 0; itr < 28; itr++)
    {

        if(m_itemmake_spellbook_idx == itr)
        {

            Set_Outline_Color(186);

            Set_Font_Colors_15(3, &colors2[0]);

        }
        else
        {

            Set_Outline_Color(192);

            Set_Font_Colors_15(3, &colors1[0]);

        }

        Set_Font_Style_Shadow_Down(3, 15, ST_NULL, ST_NULL);

        Print_Far((x_start + 30 + ((itr / 14) * 130)), (y_start + 7 + ((itr % 14) * 10)), spell_data_table[Some_IC_Alloc_5[(word_4336C + itr)]].name);

    }

    if(m_itemmake_spellbook_idx >= 0)
    {

        if(m_itemmake_spellbook_idx > 13)
        {

            var_6 = (m_itemmake_spellbook_idx - 13);

            var_4 = 1;

        }
        else
        {

            var_6 = m_itemmake_spellbook_idx;

            var_4 = 0;

        }

        SETMAX(var_6, 11);

        if(m_itemmake_spellbook_idx == 0)
        {

            var_6 = 1;

        }

        FLIC_Draw((x_start + 155) - (var_4 * 140), (y_start + (var_6 * 10) - 4), IMG_IC_SpellCharges);

        Set_Outline_Color(192);

        Set_Font_Colors_15(3, &colors1[0]);

        Set_Font_Style_Shadow_Down(3, 15, ST_NULL, ST_NULL);

        Print_Centered(((x_start + 204) - (var_4 * 140)), (y_start + (var_6 * 10) + 1), cnst_Charges);  // "Charges"

    }

}


// WZD o115p13
static int16_t Item_Make_Screen_Spellbook_Popup(void)
{
    char string[LEN_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t spell_fields[28] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t backward_field = 0;
    int16_t foreward_field = 0;
    int16_t y_start = 0;
    int16_t x_start = 0;
    int16_t itr2 = 0;
    int16_t bookmark_field = 0;
    int16_t input_field_idx = 0;
    int16_t return_value = 0;
    int16_t itr = 0;  // _SI_
    int16_t leave_screen = 0;  // _DI_

    strcpy(string, GUI_NearMsgString);


    x_start = 28;
    y_start = 12;

    Load_Palette_From_Animation(_spellbook_small_seg);

    Update_Remap_Gray_Palette();

    m_itemmake_spellbook_idx = ST_UNDEFINED;

    Clear_Fields();

    word_4336C = 0;

    Assign_Auto_Function(Item_Make_Screen_Spellbook_Popup_Draw, 2);

    bookmark_field = Add_Hidden_Field(187, 168, 205, 199, ST_UNDEFINED, ST_UNDEFINED);

    foreward_field = Add_Hidden_Field((x_start + 259), (y_start + 2), (x_start + 272), (y_start + 15), (int16_t)cnst_HOTKEY_F_3[0], ST_UNDEFINED);

    backward_field = Add_Hidden_Field((x_start +  13), (y_start + 2), (x_start +  26), (y_start + 14), (int16_t)cnst_HOTKEY_B_4[0], ST_UNDEFINED);

    Set_Font_Style_Shadow_Down(3, 15, ST_NULL, ST_NULL);


    for(itr = 0; itr < 28; itr++)
    {

        spell_fields[itr] = Add_Hidden_Field((x_start + 28 + ((itr / 14) * 130)), (y_start + 7 + ((itr % 14) * 10)), (x_start + 133 + ((itr / 14) * 130)), (y_start + 16 + ((itr % 14) * 10)), (int16_t)empty_string__ovr115[0], ST_UNDEFINED);

    }

    leave_screen = ST_FALSE;

    while(leave_screen == ST_FALSE)
    {

        input_field_idx = Get_Input();

        if(input_field_idx == bookmark_field)
        {
            
            return_value = ST_FALSE;

            leave_screen = ST_TRUE;

        }

        if(input_field_idx == foreward_field)
        {

            if((word_4336C + 28) < m_spell_list_count)
            {

                word_4336C += 28;
                
            }

        }

        if(input_field_idx == backward_field)
        {
            
            if(word_4336C > 0)
            {
                
                word_4336C -= 28;

            }

        }

        for(itr = 0; itr < 28; itr++)
        {

            if(spell_fields[itr] == input_field_idx)
            {

                if(Some_IC_Alloc_5[(word_4336C + itr)] > 0)
                {

                    return_value = Some_IC_Alloc_5[(word_4336C + itr)];
                    
                    m_itemmake_spellbook_idx = itr;

                    // 1AEE  6894  137 * 50 = 6850  6894 - 6850 = 44  2C  /* 2C */  int16_t  embed_spell_cnt;
                    _ITEMS[137].embed_spell_cnt = 0;

                    Clear_Fields();

                    if(itr > 13)
                    {

                        leave_screen = ST_TRUE;

                        itr -= 14;
                        
                        if(itr > 10)
                        {

                            itr = 10;

                        }

                    }
                    else
                    {

                        if(itr > 11)
                        {

                            itr = 11;

                        }

                    }

                }

                for(itr2 = 0; itr2 < 4; itr2++)
                {

                    spell_fields[itr2] = Add_Button_Field(((itr2 * 23) + (158 + x_start) - (leave_screen * 140)), (11 + (itr * 10) + y_start + (leave_screen * 10)), empty_string__ovr115, m_itemmake_charges_segs[itr2], (int16_t)empty_string__ovr115[0], ST_UNDEFINED);

                }

                itr = m_spell_list_count;

                while(_ITEMS[137].embed_spell_cnt == 0)
                {

                    input_field_idx = Get_Input();

                    for(itr2 = 0; itr2 < 4; itr2++)
                    {

                        if(spell_fields[itr2] == input_field_idx)
                        {

                            _ITEMS[137].embed_spell_cnt = (itr2 + 1);
                            
                        }

                    }

                    if(_ITEMS[137].embed_spell_cnt == 0)
                    {

                        Set_Page_Off();

                        Item_Make_Screen_Spellbook_Popup_Draw();

                        PageFlip_FX();

                    }

                }

                leave_screen = ST_TRUE;

            }

        }

        if(leave_screen == ST_FALSE)
        {

            Set_Page_Off();

            Item_Make_Screen_Spellbook_Popup_Draw();
            
            PageFlip_FX();

        }

    }

    strcpy(GUI_NearMsgString, string);

    return return_value;

}


// WZD o115p14
/*
; loads and adds the specified prefabricated item to
; the item table or, if passed -1, moves the item from
; index 88h (136) to the active table (0-131)
; returns the item index if successful, or -1 otherwise
*/
static int16_t Add_Item(int16_t itemdata_idx)
{
    struct s_ITEM_DATA itemdata;
    int16_t item_idx;

    if(itemdata_idx != -1)
    {

        TBL_Premade_Items[itemdata_idx] = 1;

        LBX_Load_Data_Static(itemdata_lbx_file__ovr115, 0, (SAMB_ptr)&itemdata, itemdata_idx, 1, 56);

        itemdata.moves *= 2;  // convert moves to moves2

        memcpy(&itemdata, &_ITEMS[136], sizeof(_ITEMS[0]));
    }

    item_idx = Activate_Item(136);

    return item_idx;
}

// WZD o115p15
/*
; copies the item into the first empty slot of the
; active part of the item table (0-131)
; returns the new index if successful, or -1 if not

XREF:


e.g.,
Add_Item()
    Activate_Item(136)
G_OVL_Cast()
    j_ITEM_MoveToActive(136)
    j_ITEM_MoveToActive(137)

*/
int16_t Activate_Item(int16_t old_item_idx)
{
    int16_t new_item_idx;  // _DI_
    int16_t itr;  // _SI_

    new_item_idx = -1;

    for(itr = 0; itr < 132; itr++)
    {
        if(_ITEMS[itr].cost <= 0)
        {
            memcpy(&_ITEMS[itr], &_ITEMS[old_item_idx], sizeof(_ITEMS[0]));
            new_item_idx = itr;
        }
    }

    return new_item_idx;
}


// WZD o115p16
/*
; generates a random item into index $88 (136)
; BUG: uses an unintialized variable to set the desired amount of powers on artifact quality items
; WARNING: can only use stat-boosting powers
; WARNING: value is checked only after adding a power

pass Value = 0 to randomly generate a value between 700 and 1700

*/
static void Create_Random_Item(int16_t Power, int16_t Value)
{
    struct s_ITEM_TYPE_ICONS_ENTRY Item_Type_Icon_Ranges[10];
    int16_t tries;
    int16_t max_item_power_cnt;
    int16_t not_done;
    int16_t item_power_idx;
    int16_t max_item_cost;
    int16_t weapon_type = 0;  // BUGBUG  used before initialized
    int16_t itr;  // _SI_
    int16_t power_ctr;  // _DI_

    memcpy(&Item_Type_Icon_Ranges, &CRP_ITEM_IconRanges_2, 40);

    power_ctr = 0;

    m_itemmake_item_cost = 0;

    for(itr = 0; itr < 4; itr++)
    {
        m_itemmake_item_powers_array[itr] = -2;
    }

    if(Value == 0)
    {
        Value = (700 + (Random(10) * 100));  // {700,...,1700}
    }

    if(Power == 1)
    {
        max_item_cost = 200;
        max_item_power_cnt = 2;
    }
    else
    {
        max_item_cost = 20000;
        // ¿ ; BUG: variable not set yet ?
        // ¿ so, always = 4 ?
        if(
            (weapon_type < it_Axe)
            ||
            (weapon_type == it_Staff)
            ||
            (weapon_type == it_Wand)
        )
        {
            max_item_power_cnt = 4;  // ¿ ~== melee weapon ?  ...Sword, Mace, Axe, Staff, Wand
        }
        else
        {
            max_item_power_cnt = 3;  // ...Bow, Chain, Plate, Shield, Pendant, Brooch, Ring, Cloak, Gauntlet, Helmet, Orb
        }
    }

    weapon_type = (Random(10) - 1);  // {0, ..., 9}

    m_itemmake_weapon_type_bits = (1 << weapon_type);

    m_itemmake_item_icon_idx = ((Item_Type_Icon_Ranges[weapon_type].start + Random(Item_Type_Icon_Ranges[weapon_type].count)) - 1);

    _ITEMS[136].embed_spell_idx = 0;

    _ITEMS[136].embed_spell_cnt = 0;

    _ITEMS[136].type = weapon_type;

    _ITEMS[136].icon_idx = m_itemmake_item_icon_idx;

    // allocate for and load all of the item power data records
    _ITEM_POWERS = (struct s_ITEM_POWER * )Near_Allocate_First(2010);  // 2010 B;  67 * 30 = 2010
    LBX_Load_Data_Static(itempow_lbx_file__ovr115, 0, (SAMB_ptr)_ITEM_POWERS, 0, 67, 30);

    not_done = ST_TRUE;
    tries = 0;
    while(not_done != ST_FALSE)
    {
        tries++;
        item_power_idx = (Random(NUM_ITEM_POWER_BASIC) - 1);  // {0,...,32} *Basic* Item Powers
        if((_ITEM_POWERS[item_power_idx].item_types & m_itemmake_weapon_type_bits) > 0)
        {
            if(_ITEM_POWERS[item_power_idx].cost <= max_item_cost)
            {

                for(itr = 0; itr < power_ctr; itr++)
                {
                    if(_ITEM_POWERS[item_power_idx].type == _ITEM_POWERS[m_itemmake_item_powers_array[itr]].type)
                    {
                        item_power_idx = -1;
                        break;
                    }
                }

                if(item_power_idx >= 0)
                {
                    m_itemmake_item_powers_array[power_ctr] = item_power_idx;
                    m_itemmake_item_cost = Get_Item_Cost(136);
                    strcpy(GUI_NearMsgString, Get_Item_Name(136));
                    Create_Item_Record(136);
                    power_ctr++;
                }
            }
        }

        if(power_ctr > 0)
        {
            if(power_ctr == max_item_power_cnt)
            {
                not_done = ST_FALSE;
            }
            
            if(m_itemmake_item_cost > Value)
            {
                not_done = ST_FALSE;
            }
            if(tries > 50)
            {
                not_done = ST_FALSE;
            }
        }

    }

}


// WZD o115p17
/*

int16_t Power           {0,1,2}
int16_t spellranks[]    uint8_t [5] of magic_realms/spellranks/books
int16_t Value           {0,1000,1200,30000}  ...if value == 0 then {200,2000}

; generates a random item of the specified power level and value (roughly), either by choosing a prefab one or rolling randomly if that fails returns the index of the created item
; BUG: random artifacts don't always have the intended amount of powers
; WARNING: prefab items must match the books of the player
; WARNING: random items can only have stat-boosts

XREF:
    j_ITEM_Generate()
        EVNT_GenerateRandom()
        Determine_Offer()
        Lair_Generate_Treasure()
        Cast_Spell_Overland_Do()


e.g.,
        EVNT_GenerateRandom()
            j_ITEM_Generate(2, &_players[].spellranks, 0)
        Determine_Offer()
            j_ITEM_Generate(0, &Bookshelf, 0)
        Lair_Generate_Treasure()
            j_ITEM_Generate(_LAIRS[].Item_Values, &_players[].spellranks, 0)
            j_ITEM_Generate(2, &_players[].spellranks, 1200)
            j_ITEM_Generate(0, &_players[].spellranks, 1200)
        Cast_Spell_Overland_Do()
            j_ITEM_Generate(1, &_players[].spellranks, 1000)
            j_ITEM_Generate(2, &_players[].spellranks, 30000)

*/
int16_t Make_Item(int16_t Power, int16_t spellranks[], int16_t Value)
{
    struct s_ITEM_DATA itemdata;
    int8_t magic[5];
    int16_t Artifact;
    int16_t Tries;
    int16_t itr;  // _DI_
    int16_t item_idx;  // _SI_

    item_idx = -1;

    Tries = 0;

    while((item_idx == -1) && (Tries < 50))
    {
        Tries++;

        item_idx = (Random(250) - 1);  // {0, ..., 249}

        LBX_Load_Data_Static(itemdata_lbx_file__ovr115, 0, (SAMB_ptr)&itemdata, item_idx, 1, 56);

        if(TBL_Premade_Items[item_idx] == 1)
        {
            item_idx = -1;
        }

        for(itr = 0; itr < NUM_MAGIC_TYPES; itr++)
        {
            if(itemdata.spellranks[itr] > spellranks[itr])
            {
                item_idx = -1;
            }
        }

        if(Power == 1)
        {
            if(itemdata.flag > 0)  /* ~== flag == 1*/
            {
                item_idx = -1;
            }
        }

        if(Power == 2)
        {
            if(itemdata.flag == 0)
            {
                item_idx = -1;
            }
        }

        // DEDU:  What's with the (itemdata.cost <= (Value / 2))? 
        if(
            (itemdata.cost > Value)
            ||
            ((Value / 2) > itemdata.cost)
        )
        {
            if(Value == 0)
            {
                item_idx = -1;
            }
        }
    }

    if(item_idx == -1)
    {
        Create_Random_Item(Power, Value);
    }

    item_idx = Add_Item(item_idx);

    return item_idx;
}
