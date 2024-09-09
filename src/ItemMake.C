/*
    ENCHANT
    Make Item
    ...itemmake (IM)
    
    WIZARDS.EXE
        ovr115
*/

#include "MoX.H"




#pragma pack(push)
#pragma pack(2)
struct s_ITEM_TYPE_ICONS_ENTRY
{
    int16_t start;
    int16_t count;
};
#pragma pack(pop)



// WZD dseg:5BCC                                                 BEGIN:  ovr115 - Strings

// WZD dseg:5BCC 11 09 08 08                                     COL_DeselectItemName db 11h, 9, 2 dup(8)
// WZD dseg:5BD0 00 B1                                           word_3C670 dw 0B100h
// WZD dseg:5BD2 B2 B3                                           word_3C672 dw 0B3B2h
// WZD dseg:5BD4 00 1F                                           word_3C674 dw 1F00h
// WZD dseg:5BD6 1D 1C                                           word_3C676 dw 1C1Dh

// WZD dseg:5BD8
int16_t m_item_type_base_cost[10] = {100, 100, 100, 100, 300, 200, 50, 100, 100, 300};

// WZD dseg:5BEC 00 00 09 00 09 00 0B 00 14 00 09 00 1D 00 09 00+ITEM_IconRanges dw 0, 9, 9, 11, 20, 9, 29, 9, 38, 9, 107, 9, 72, 35, 62, 10, 47, 8, 55, 7
// WZD dseg:5C14 00 00 09 00 09 00 0B 00 14 00 09 00 1D 00 09 00+
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
char itempow_lbx_file[] = "ITEMPOW";
// WZD dseg:5C43
char empty_string__ovr115[] = "";

// WZD dseg:5C44 53 50 45 4C 4C 53 43 52 00                      cnst_SPELLSCR_File db 'SPELLSCR',0

// WZD dseg:5C4D
char cnst_TimesSign_2[] = " x";

// WZD dseg:5C50 43 6F 73 74 3A                                  cnst_Cost_Col db 'Cost:'

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

// WZD dseg:5CFC 1B 00                                           cnst_HOTKEY_Esc18 db 1Bh,0
// WZD dseg:5CFE 6D 65 73 73 61 67 65 00                         cnst_MESSAGE_File5 db 'message',0
// WZD dseg:5D06 43 68 61 72 67 65 73 00                         cnst_Charges db 'Charges',0
// WZD dseg:5D0E 46 00                                           cnst_HOTKEY_F_3 db 'F',0
// WZD dseg:5D10 42 00                                           cnst_HOTKEY_B_4 db 'B',0

// WZD dseg:5D12
char itemdata_lbx_file[] = "Itemdata";

// WZD dseg:5D1B 00                                              align 2

// WZD dseg:5D1B                                                 END:  ovr115 - Strings





// WZD dseg:C8B6                                                 BEGIN:  ovr115

// ; array of 4 2-state images (unselected - selected)
// WZD dseg:C8B6 00 00 00 00 00 00 00 00                         IMG_IC_NumCharges@ dw 4 dup(0
// WZD dseg:C8BE 00 00                                           IMG_IC_SpellCharges@ dw 0
// WZD dseg:C8C0 00 00                                           Some_IC_Alloc_5 dw 0
// WZD dseg:C8C2 00 00                                           IMG_IC_SelItemBtns@ dw 0                ; 20 LBX_NearAlloc_Next bytes (Item Crafting base item button image pointers for the selected type)
// WZD dseg:C8C4 00 00                                           IMG_IC_ItemButtons@ dw 0                ; 20 LBX_NearAlloc_Next bytes (Item Crafting base item button image pointers)
// WZD dseg:C8C6 00 00                                           ITEM_PwrLabel_States dw 0
// WZD dseg:C8C8 00 00                                           ITEM_AvailablePowers dw 0

// WZD dseg:C8CA
struct s_ITEM_POWER * _ITEM_POWERS;

// WZD dseg:C8CC 00 00                                           word_4336C dw 0
// WZD dseg:C8CE 00 00                                           COL_IC_EditBox@ dw 0 // ~ [4][4] {177,178,179,180}
// WZD dseg:C8D0 00 00                                           ITEM_Left_Arrow_Btn dw 0
// WZD dseg:C8D2 00 00                                           ITEM_Name_EditBox dw 0
// WZD dseg:C8D4 00 00                                           ITEM_Power_Labels dw 0
// WZD dseg:C8D6 00 00                                           ITEM_Right_Arrow_Btn dw 0
// WZD dseg:C8D8 00 00                                           ITEM_Ok_Button_Index dw 0
// WZD dseg:C8DA 00 00                                           ITEM_Type_Buttons dw 0

// WZD dseg:C8DC
/*
shared with 'Hire / Merchant' ovr127
DEDU  ¿ should use UV's variable ?

Merchant_Popup_Load()
    ITEM_Draw_Seg = Allocate_Next_Block(_screen_seg, 1200);

*/
SAMB_ptr ITEM_Draw_Seg;

// WZD dseg:C8DE 00 00                                           word_4337E dw 0
// WZD dseg:C8E0 00 00                                           Some_IC_Var_3 dw 0

// WZD dseg:C8E2
int16_t itemmake_item_icon_idx;

// WZD dseg:C8E4 00 00                                           IMG_IC_EnchantBtn@ dw 0

// WZD dseg:C8E6
int16_t itemmake_item_powers_count;

// WZD dseg:C8E8
int16_t itemmake_item_powers_array[MAX_ITEM_POWERS];

// WZD dseg:C8F0 00 00                                           ITEM_Col1_Powers dw 0
// WZD dseg:C8F2 00 00                                           ITEM_Col2_Powers dw 0
// WZD dseg:C8F4 00 00                                           ITEM_Col3_Powers dw 0

// WZD dseg:C8F6
int16_t itemmake_item_cost;

// WZD dseg:C8F8
int16_t itemmake_item_type_bit_idx;

// WZD dseg:C8F8                                                 END:  ovr115 - Uninitialized Data




/*
    WIZARDS.EXE  ovr115
*/

// WZD o115p01
// ITEM_LoadCraftingRes()

// WZD o115p02
// sub_94FDD()

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
static char * Get_Item_Name(int16_t item_idx)
{

    char Spell_Name[20];
    char * Attrib_Suffixes[7];
    char Temp_String[30];
    char Conversion_String[10];
    int16_t Have_Prefix;
    int16_t Unprocessed_Powers[4];
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


    item_type_bit_idx = itemmake_item_type_bit_idx;
    item_type = 0;
    while(item_type_bit_idx > 1)
    {
        item_type++;
        item_type_bit_idx = (item_type_bit_idx >> 1);
    }

    for(itr = 0; itr < NUM_ITEM_POWERS; itr++)
    {
        Unprocessed_Powers[itr] = itemmake_item_powers_array[itr];
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
            strcpy(Temp_String, cnst_Plus_4);
            itoa((Unprocessed_Powers[itr] + 1), Conversion_String, 10);
            strcat(Temp_String, Conversion_String);
            strcat(Temp_String, cnst_Space_4);
            strcat(Temp_String, Get_Item_Type_Name(item_idx));  // Sword, Mace, Axe, Bow, Staff, Wand, Misc, Shield, Chain, Plate, Amulet, Ring, Cloak, Gauntlet, Helm, Orb

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
        strcpy(Temp_String, Get_Item_Type_Name(item_idx));
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
        strcat(Temp_String, cnst_Sp_Of_Sp_4);

        if(Highest_Index_Power < 33)
        {
            strcat(Temp_String, Attrib_Suffixes[_ITEM_POWERS[Highest_Index_Power].type]);
        }
        else if(Highest_Index_Power < 65)
        {
            strcat(Temp_String, _ITEM_POWERS[Highest_Index_Power].name);
        }
        else
        {
            strcpy(Spell_Name, spell_data_table[_ITEMS[item_idx].embed_spell_idx].name);
            strcat(Temp_String, Spell_Name);
            if(strlen(Temp_String) < 26)
            {
                strcat(Temp_String, cnst_TimesSign_2);
                itoa(_ITEMS[item_idx].embed_spell_cnt, Conversion_String, 10);
                strcat(Temp_String, Conversion_String);
            }
        }

    }

    if(Have_Prefix == ST_TRUE)
    {
        if(strlen(Temp_String) > 26)
        {
            for(itr = 0; itr < 3; itr++)
            {
                Clear_Structure(0, (uint8_t *)&Temp_String, 1, 30);
            }
        }
    }

    return &Temp_String[0];
}


// WZD o115p05
/*

; calculates and returns the total cost of crafting the selected item, accounting for its base cost (but not any retorts)

*/
static int16_t Get_Item_Cost(int16_t item_idx)
{
    int16_t item_type_base_cost[10];
    int16_t item_type;
    int16_t item_type_bit_idx;
    int16_t itr;  // _SI_
    int16_t item_cost;  // _DI_

    memcpy(item_type_base_cost, m_item_type_base_cost, 20);

    item_type_bit_idx = itemmake_item_type_bit_idx;
    item_type = 0;
    while(item_type_bit_idx > 1)
    {
        item_type++;
        item_type_bit_idx = (item_type_bit_idx >> 1);
    }

    item_cost = item_type_base_cost[item_type];

    for(itr = 0; itr < NUM_ITEM_POWERS; itr++)
    {
        if(itemmake_item_powers_array[itr] > -1)
        {
            if(itemmake_item_powers_array[itr] == 65)
            {
                item_cost += (spell_data_table[_ITEMS[item_idx].embed_spell_idx].Casting_Cost * _ITEMS[item_idx].embed_spell_cnt * 20);
            }
            else
            {
                if(
                    (item_type == it_Misc)
                    &&
                    (itemmake_item_powers_array[itr] < 33)
                )
                {
                    (item_cost += _ITEM_POWERS[itemmake_item_powers_array[itr]].cost * 2);
                }
                else
                {
                    item_cost += _ITEM_POWERS[itemmake_item_powers_array[itr]].cost;
                }
            }
        }
    }

    return item_cost;
}


// WZD o115p06
// sub_95B62()

// WZD o115p07
// ITEM_CreateControls()

// WZD o115p08
// sub_95E65()

// WZD o115p09
static void Create_Item_Record(int16_t item_idx)
{
    int16_t embed_spell_cnt;
    int16_t embed_spell_idx;
    int16_t item_type;
    int16_t item_type_bit_idx;  // _DI_
    int16_t itr;  // _DI_


    embed_spell_idx = _ITEMS[item_idx].embed_spell_idx;
    embed_spell_cnt = _ITEMS[item_idx].embed_spell_cnt;
    memset(&_ITEMS[item_idx], 0, sizeof(_ITEMS[0]));
    strcpy(_ITEMS[item_idx].name, GUI_NearMsgString);
    _ITEMS[item_idx].embed_spell_idx = embed_spell_idx;
    _ITEMS[item_idx].embed_spell_cnt = embed_spell_cnt;

    item_type_bit_idx = itemmake_item_type_bit_idx;
    item_type = 0;
    while(item_type_bit_idx > 1)
    {
        item_type++;
        item_type_bit_idx = (item_type_bit_idx >> 1);
    }

    _ITEMS[item_idx].icon_idx = itemmake_item_icon_idx;
    _ITEMS[item_idx].type = item_type;
    _ITEMS[item_idx].cost = itemmake_item_cost;

    switch(item_type)
    {
        case 0:
        case 1:
        case 2:
        {
            _ITEMS[item_idx].Slot_Type = 1;  /* Sword_Slot */
        } break;
        case 3:
        {
            _ITEMS[item_idx].Slot_Type = 2;  /* Bow_Slot */
        } break;
        case 4:
        case 5:
        {
            _ITEMS[item_idx].Slot_Type = 4;  /* Staff_Slot */
        } break;
        case 6:
        {
            _ITEMS[item_idx].Slot_Type = 6;  /* Misc_Slot */
        } break;
        case 7:
        case 8:
        case 9:
        {
            _ITEMS[item_idx].Slot_Type = 5;  /* Armor_Slot */
        } break;
    }

    for(itr = 0; itr < NUM_ITEM_POWERS; itr++)
    {
        if(itemmake_item_powers_array[itr] > -1)
        {
            if(itemmake_item_powers_array[itr] < 6)
            {
                _ITEMS[item_idx].attack = (itemmake_item_powers_array[itr] + 1);
            }
            else if(itemmake_item_powers_array[itr] < 12)
            {
                _ITEMS[item_idx].defense = (itemmake_item_powers_array[itr] - 5);
            }
            else if(itemmake_item_powers_array[itr] < 15)
            {
                _ITEMS[item_idx].tohit = (itemmake_item_powers_array[itr] - 11);
            }
            else if(itemmake_item_powers_array[itr] < 19)
            {
                _ITEMS[item_idx].moves2 = ((itemmake_item_powers_array[itr] - 14) * 2);
            }
            else if(itemmake_item_powers_array[itr] < 25)
            {
                _ITEMS[item_idx].resistance = (itemmake_item_powers_array[itr] - 18);
            }
            else if(itemmake_item_powers_array[itr] < 29)
            {
                _ITEMS[item_idx].spell_skill = ((itemmake_item_powers_array[itr] - 24) * 5);
            }
            else if(itemmake_item_powers_array[itr] < 33)
            {
                _ITEMS[item_idx].spell_save = (itemmake_item_powers_array[itr] - 28);
            }
            else
            {
                _ITEMS[item_idx].Powers |= _ITEM_POWERS[itemmake_item_powers_array[itr]].powers;
            }
        }
    }

    if(item_type == it_Chainmail)
    {
        _ITEMS[item_idx].defense += 1;
    }

    if(item_type == it_Platemail)
    {
        _ITEMS[item_idx].defense += 2;
    }

}


// WZD o115p11
// ITEM_Get_IC_Powers()

// WZD o115p12
// sub_96AF2()

// WZD o115p13
// sub_96D84()

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

        LBX_Load_Data_Static(itemdata_lbx_file, 0, (SAMB_ptr)&itemdata, itemdata_idx, 1, 56);

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
static int16_t Activate_Item(int16_t old_item_idx)
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
    int16_t Tries;
    int16_t max_item_power_cnt;
    int16_t Powers_Left;
    int16_t item_power_idx;
    int16_t max_item_cost;
    int16_t Item_Type = 0;  // BUGBUG  used before initialized
    int16_t itr;  // _SI_
    int16_t IDK_Power_Count;  // _DI_

    memcpy(&Item_Type_Icon_Ranges, &CRP_ITEM_IconRanges_2, 40);

    IDK_Power_Count = 0;

    itemmake_item_cost = 0;

    for(itr = 0; itr < 4; itr++)
    {
        itemmake_item_powers_array[itr] = -2;
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
            (Item_Type < it_Axe)
            ||
            (Item_Type == it_Staff)
            ||
            (Item_Type == it_Wand)
        )
        {
            max_item_power_cnt = 4;  // ¿ ~== melee weapon ?  ...Sword, Mace, Axe, Staff, Wand
        }
        else
        {
            max_item_power_cnt = 3;  // ...Bow, Chain, Plate, Shield, Pendant, Brooch, Ring, Cloak, Gauntlet, Helmet, Orb
        }
    }

    Item_Type = (Random(10) - 1);  // {0, ..., 9}

    itemmake_item_type_bit_idx = (1 << Item_Type);

    itemmake_item_icon_idx = ((Item_Type_Icon_Ranges[Item_Type].start + Random(Item_Type_Icon_Ranges[Item_Type].count)) - 1);

    _ITEMS[136].embed_spell_idx = 0;

    _ITEMS[136].embed_spell_cnt = 0;

    _ITEMS[136].type = Item_Type;

    _ITEMS[136].icon_idx = itemmake_item_icon_idx;

    // allocate for and load all of the item power data records
    _ITEM_POWERS = (struct s_ITEM_POWER * )Near_Allocate_First(2010);  // 2010 B;  67 * 30 = 2010
    LBX_Load_Data_Static(itempow_lbx_file, 0, (SAMB_ptr)_ITEM_POWERS, 0, 67, 30);


    Powers_Left = ST_TRUE;
    Tries = 0;
    while(Powers_Left != ST_FALSE)
    {
        Tries++;
        item_power_idx = (Random(NUM_ITEM_POWER_BASIC) - 1);  // {0,...,32} *Basic* Item Powers
        if((_ITEM_POWERS[item_power_idx].item_types & itemmake_item_type_bit_idx) > 0)
        {
            if(_ITEM_POWERS[item_power_idx].cost <= max_item_cost)
            {

                for(itr = 0; itr < IDK_Power_Count; itr++)
                {
                    if(_ITEM_POWERS[item_power_idx].type == _ITEM_POWERS[itemmake_item_powers_array[itr]].type)
                    {
                        item_power_idx = -1;
                        break;
                    }
                }

                if(item_power_idx >= 0)
                {
                    itemmake_item_powers_array[IDK_Power_Count] = item_power_idx;
                    itemmake_item_cost = Get_Item_Cost(136);
                    strcpy(GUI_NearMsgString, Get_Item_Name(136));
                    Create_Item_Record(136);
                    IDK_Power_Count++;
                }
            }
        }

        if(IDK_Power_Count > 0)
        {
            if(IDK_Power_Count == max_item_power_cnt)
            {
                Powers_Left = ST_FALSE;
            }
            
            if(itemmake_item_cost > Value)
            {
                Powers_Left = ST_FALSE;
            }
            if(Tries > 50)
            {
                Powers_Left = ST_FALSE;
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
        WIZ_SetOverlandSpell()


e.g.,
        EVNT_GenerateRandom()
            j_ITEM_Generate(2, &_players[].spellranks, 0)
        Determine_Offer()
            j_ITEM_Generate(0, &Bookshelf, 0)
        Lair_Generate_Treasure()
            j_ITEM_Generate(_LAIRS[].Item_Values, &_players[].spellranks, 0)
            j_ITEM_Generate(2, &_players[].spellranks, 1200)
            j_ITEM_Generate(0, &_players[].spellranks, 1200)
        WIZ_SetOverlandSpell()
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

        LBX_Load_Data_Static(itemdata_lbx_file, 0, (SAMB_ptr)&itemdata, item_idx, 1, 56);

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

