
Artifact Creation Screen
Itemmake Screen

icon window
item name window
attribute enchantments window
spell-specific enchantments window
cost window
weapon buttons
OK button



Item_Make_Screen()
Item_Make_Screen_Load()
Item_Make_Screen_Add_Fields()
Item_Make_Screen_Draw()
Itam_Make_Screen_Build_Weapon_Powers_List()



Item_Make_Screen()
Item_Make_Screen_Load()
Item_Make_Screen_Add_Fields()
Item_Make_Screen_Draw()
Itam_Make_Screen_Build_Weapon_Powers_List()

static data
    COL_DeselectItemName
    m_itemmake_colors1
    m_itemmake_colors2
    m_item_type_base_cost
    ITEM_IconRanges

pictures
    m_itemmake_charges_segs                     ~ "x1","x2","x3","x4"
    m_itemmake_attribute_highlight_buttons_seg  selected power name
    m_itemmake_attribute_buttons_seg            unselected/deselected power name
    m_itemmake_item_icon_bitmap_seg                               scratch space for item icon
    m_itemmake_cost_window_ok_seg

fields
    m_itemmake_icon_window_left_arrow_button    left arrow icon input field
    m_itemmake_item_name_field                  item name text input field
    m_itemmake_weapon_power_fields              power name input fields array, 3 columns
    m_itemmake_icon_window_right_arrow_button   right arrow icon input field
    m_itemmake_cost_window_ok_button            Ok button input field
    m_itemmake_weapon_fields                    weapon button input fields array

IMG_IC_SpellCharges         
Some_IC_Alloc_5         
ITEM_PwrLabel_States                            array of {F,T} for m_itemmake_weapon_power_fields
m_itemmake_weapon_type_powers_list              array of selectable powers, as three columns, based on the selected weapon type
_ITEM_POWERS                                    array of structs of all item powers
m_itemmake_item_name_input_colors               ¿ 4 arrays of 4 colors - normal, highlight, ... ?
niu_did_select_spell                                   
m_itemmake_item_icon_idx                        
m_itemmake_item_powers_count                    
m_itemmake_item_powers_array                    
m_itemmake_power_list_counts                    
m_itemmake_item_cost                            
m_itemmake_weapon_type_bits                     bits to test for matching item powers



struct s_ITEM
    /* 20 */  int8_t   Slot_Type;      // ¿ ITEMMISC.LBX  ITEMSLOT  sword, bow, weapon/staff, wand, misc, armor ?
    vs.
    /* 21 */  int8_t   type;           // ¿ {sword, mace, axe, bow, staff, wand, misc, shield, chain, plate} ?



## Itam_Make_Screen_Build_Weapon_Powers_List()

...initial or selected weapon type...
...build list of weapon attributes, as three columns...

...something with keeping track of which powers are valid weapon a different weapon type is selected...
...presumably, so that if they were selected they are still selected

powers  0 through 32 get put in the first and second columns


powers 33 through 65 get put in the 3rd column
    column_idx = 2;
    for(itr = 33; itr < 66; itr++)


_ITEM_POWERS[]
m_itemmake_power_list_counts[]
m_itemmake_weapon_type_powers_list[]

iter  0 < 33
iter 33 < 66

give & take?
...limits by player's books and total cost
...return attribute count?
takes itemmake_weapon_type_bits
gives attribute list

cost is per power, not total for item?

What's it doing with when the weapon type changes?
Isn't it just rebuilding the whole list on every call, which is every time the weapon changes?
Can only happen once?  ...on the first loop?
...so, Last_Empty_Line = _DI_return_value will always be 0?





## itemmake_weapon_type_bits

XREF:
    Item_Make_Screen_Load+1E2      mov     [itemmake_weapon_type_bits], 1 ; already done     
    Get_Item_Name+2B               mov     _DI_item_type_bit_idx, [itemmake_weapon_type_bits]
    Get_Item_Cost+1A               mov     ax, [itemmake_weapon_type_bits]                   
    Item_Make_Screen_Add_Fields+81 cmp     ax, [itemmake_weapon_type_bits]                   
    Item_Make_Screen+50          mov     [itemmake_weapon_type_bits], 1                    
    Item_Make_Screen+47A         mov     [itemmake_weapon_type_bits], ax                   
    Item_Make_Screen+47D         mov     ax, [itemmake_weapon_type_bits]                   
    Create_Item_Record+9D          mov     _DI_itr, [itemmake_weapon_type_bits]              
    Itam_Make_Screen_Build_Weapon_Powers_List+37          mov     ax, [itemmake_weapon_type_bits]                   
    Itam_Make_Screen_Build_Weapon_Powers_List+13F         mov     ax, [itemmake_weapon_type_bits]                   
    Create_Random_Item+9C          mov     [itemmake_weapon_type_bits], ax                   
    Create_Random_Item+14B         mov     ax, [itemmake_weapon_type_bits]                   


used for setting the pict on fields - normal or highlight
used for testing against _ITEM_POWERS[].item_types
    ...which attributes are available to which weapons?
used for setting the item slot type
    ...counts the bits?

¿¿¿
~ weapon / item weapon type
enum enum_ITEM_TYPE
it_Sword  = 0
it_Mace  = 1
it_Axe  = 2
it_Bow  = 3
it_Staff  = 4
it_Wand  = 5
it_Misc  = 6
it_Shield  = 7
it_Chainmail  = 8
it_Platemail  = 9
???



## itemmake_item_powers_array
iter 0 < 4
initialized to -2 at start of Item_Make_Screen()

if > -1
if == 65 ... spell
if it_Misc && < 33  cost * 2 else cost

Create_Item_Record()
if 6, 12, 15, 19, 25, 33

Create_Random_Item()
    itemmake_item_powers_array[power_ctr] = item_power_idx;



## ITEM_PwrLabel_States
initialized to 0 at start of Item_Make_Screen()
iter 0 < 54



## niu_did_select_spell

In Item_Make_Screen(),
    defaulted/initialized to ST_FALSE
    Left-Click Item Power
        set to ST_TRUE if a spell was selected, on the item make spellbook screen
        set to ST_FALSE if the selection cancelled/a spell was not selected
...nothing actually uses this, though?



## Item_Type_Icon_Ranges
struct?
array of item weapon types
index of first icon index and count of icon indicies

don't know why duplicated in CRP_ITEM_IconRanges_2
Item_Type_Icon_Ranges is used in Item_Make_Screen()
CRP_ITEM_IconRanges_2 is used in Create_Random_Item()
maybe defined locally and compiled moved them out?






## Weaon Button fields

m_itemmake_weapon_fields
Add_Button_Field()
m_itemmake_attribute_buttons_seg
m_itemmake_attribute_highlight_buttons_seg
x: (156 + (33 * (itr % 5)))  ... {0,1,2,3,4,0,1,2,3,4}
y: (  3 + (15 * (itr / 5)))  ... {0,0,0,0,0,1,1,1,1,1}



m_itemmake_icon_window_left_arrow_button
m_itemmake_icon_window_right_arrow_button




The itemmake screen looks a lot like the artifact creation screen

The itemmake screen has five windows: the
icon window, the item name window, the attribute enchantments window, the
spell-specific enchantments window, and the cost window. Finally, you have
the weapon buttons, and the OK button.
