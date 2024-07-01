
MoM
MoX
Items && Heroes


'Item Screen' is a 'pop-up'  (copy off to back and all that)
pops up from 'Armies Screen', combat, and casting item/artifact
handles 1+ items being *awarded* from lair treasure or dead heroes
items have a location
    hero items have the heroes location
    vault items have the fortress location
    awarded items have a temporary location, stored in the items module
    purchased from merchants
..."view", "move", "destoy"...
..."allocate", "assign", "equip"...





### item_pool_in_process
drake178: ITEM_MapLocMove
14 xrefs; 13 mov, 1 cmp

OON TEST:
mov     [m_item_slot_idx], e_VAULT_HERO_SLOT_IDX
cmp     [ITEM_MapLocMove], e_ST_TRUE
mov     [m_item_slot_idx], e_AWARD_HERO_SLOT_IDX
overrides the default of vault
¿ just item slot for pickup/putdown ?
So, just means we're in the process of processing an item from the item pool?



¿ ?
item from treasure/award/loot
item from dead hero
item from item screen (hero or vault)
Hero Slot Index #6 is the Vault
¿ Hero Slot Index #7 is the ? mouse cursor / temporary item screen holder

¿ like MoO2  Module: TECH ?
    data (0 bytes) _g_selected_list_entry_p
        Address: 02:001A5694
    data (0 bytes) _g_entries_p
        Address: 02:001A5698
    data (0 bytes) _g_selected_entry_p
        Address: 02:001A569C
    data (0 bytes) _g_scrn_x
        Address: 02:001A56B4
    data (0 bytes) _g_scrn_y
        Address: 02:001A56B6
    data (0 bytes) _g_changing_tech
        Address: 02:001A56BA



API Boundaries

    item_list[] parameter copied to ITEM_AwardList[]



    |-> EZ_GenerateRewards()
        |-> 




### ITEM_ComposeBuffer
WZD dseg:C25E
    BEGIN:  ovr075
WZD dseg:C25E
    ITEM_ComposeBuffer
¿ like UnitDraw_WorkArea[] ?

XREF:
    ITEM_ScreenRedraw()
    Item_Screen_Load()
    ITEM_DrawWithName()
    ITEM_ShowViewWindow()
    ITEM_ViewRedraw()
    ITEM_DescribeAward()
    ITEM_DrawAwardView()

Item_Screen_Load()
    30 Paragraphs, 480 Bytes;
    ITEM_ComposeBuffer = Allocate_First_Block(_screen_seg, 30);
ITEM_ShowViewWindow()
    50 Paragraphs, 800 Bytes;
    ITEM_ComposeBuffer = Allocate_Next_Block(_screen_seg, 50)
ITEM_DescribeAward()
    60 Paragraphs, 960 Bytes;
    ITEM_ComposeBuffer = Allocate_Next_Block(_screen_seg, 60)

Allocate_Data_Space()
    60 Paragraphs, 960 Bytes;
    UnitDraw_WorkArea = Allocate_Space(60)

XREF:
    Draw_Unit_Enchantment_Outline()
    Draw_Unit_StatFig()
    Unit_Figure_Draw()
    Allocate_Data_Space()





Items <-> Heroes

vs.
struct s_BATTLE_UNIT
    // TODO  /* 0x2C */  int16_t Item_UEs_L;                         // ; enum UE_FLAGS_L
    // TODO  /* 0x2E */  int16_t Item_UEs_H;                         // ; enum UE_FLAGS_H
    /* 0x2C */  uint32_t item_enchantments;                         // Item Powers, as Unit Enchantments

struct s_OWNED_HERO
    /* 10 */ int16_t Items[3];      /* ¿ index into _ITEMS ? */

struct s_WIZARD
    /* 0120 */ uint16_t Vault_Items[4];



enum enum_ITEM_TYPE
FFFFFFFF it_Sword  = 0
FFFFFFFF it_Mace  = 1
FFFFFFFF it_Axe  = 2
FFFFFFFF it_Bow  = 3
FFFFFFFF it_Staff  = 4
FFFFFFFF it_Wand  = 5
FFFFFFFF it_Misc  = 6
FFFFFFFF it_Shield  = 7
FFFFFFFF it_Chainmail  = 8
FFFFFFFF it_Platemail  = 9

// sizeof:  32h  50d
/*
    "attribute enhancements"
*/
struct s_ITEM
{
    /* 00 */  char Name[30];
    /* 1E */  int16_t Icon_Index;      // ¿ entry_num for ITEMS.LBX ?
    /* 20 */  int8_t Slot_Type;        // ¿ ITEMMISC.LBX  ITEMSLOT  sword, bow, weapon/staff, wand, misc, armor ?
    /* 21 */  int8_t type;             // ¿ {sword, mace, axe, bow, staff, wand, misc, shield, chain, plate} ?
    /* 22 */  int16_t Mana_Value;      // ¿ "Cost" ?
    /* 24 */  int8_t Bonus_Attack;     // "Attack"
    /* 25 */  int8_t Bonus_To_Hit;     // "To Hit"
    /* 26 */  int8_t defense_bonus;    // "Defense"
    /* 27 */  int8_t moves2;           // "Movement"
    /* 28 */  int8_t Bonus_Resist;     // "Resistance"
    /* 29 */  int8_t mana_bonus;       // "Spell Skill"
    /* 2A */  int8_t Spell_Save;       // "Spell Save"
    /* 2B */  int8_t Charged_Spell;    // Embedded Spell  ("Choose a spell to embed in this item")
    /* 2C */  int16_t Charges;         // "Spell Charges"
    // TODO  /* 2E */  Powers_LOWord dw ?                      // ; enum IPOW_LO
    // TODO  /* 30 */  Powers_HOWord dw ?                      // ; enum IPOW_HO
    /* 2E */  uint32_t Powers;
    /* 32 */
};


## Item Powers

    // TODO  /* 2E */  Powers_LOWord dw ?                      // ; enum IPOW_LO
    // TODO  /* 30 */  Powers_HOWord dw ?                      // ; enum IPOW_HO
    /* 2E */  uint32_t Powers;


tested in ITEM_DrawIntoIMGSeg__WIP()  // WZD s82p0

dx, IP_Haste
ax, IP_Phantasmal
dx, IP_Chaos
ax, IP_True_Sight
ax, IP_Lion_Heart
dx, IP_Regeneration
dx, IP_Invisibility
dx, IP_Wraith_Form
ax, IP_Planar_Travel
ax, IP_Invulnerability
dx, IP_Vampiric
dx, IP_Lightning
dx, IP_Destruction
ax, IP_Power_Drain
dx, IP_Elemental_Armour
ax, IP_Magic_Immunity
ax, IP_Righteousness
ax, IP_Merging
ax, IP_Flight
dx, IP_Pathfinding
dx, IP_Guardian_Wind
ax, IP_Giant_Strength
ax, IP_Holy_Avenger
ax, IP_Death
ax, IP_Flaming
dx, IP_Cloak_Of_Fear
dx, IP_Resist_Elements
dx, IP_Stoning
ax, IP_Resist_Magic
dx, IP_Cloak_Of_Fear    ; BUG: already checked
ax, IP_Bless
dx, IP_Endurance
dx, IP_Water_Walking

dx, IP_Haste
ax, IP_Phantasmal
dx, IP_Chaos
ax, IP_True_Sight
ax, IP_Lion_Heart
dx, IP_Regeneration
dx, IP_Invisibility
dx, IP_Wraith_Form
ax, IP_Planar_Travel
ax, IP_Invulnerability
dx, IP_Vampiric
dx, IP_Lightning
dx, IP_Destruction
ax, IP_Power_Drain
dx, IP_Elemental_Armour
ax, IP_Magic_Immunity
ax, IP_Righteousness
ax, IP_Merging
ax, IP_Flight
dx, IP_Pathfinding
dx, IP_Guardian_Wind
ax, IP_Giant_Strength
ax, IP_Holy_Avenger
ax, IP_Death
ax, IP_Flaming
dx, IP_Cloak_Of_Fear
dx, IP_Resist_Elements
dx, IP_Stoning
ax, IP_Resist_Magic
dx, IP_Cloak_Of_Fear
ax, IP_Bless
dx, IP_Endurance
dx, IP_Water_Walking


IP_Haste             = 0x00004000
IP_Phantasmal        = 0x00800000
IP_Chaos             = 0x00000800
IP_True_Sight        = 0x00400000
IP_Lion_Heart        = 0x04000000
IP_Regeneration      = 0x00000040
IP_Invisibility      = 0x00008000
IP_Wraith_Form       = 0x00000020
IP_Planar_Travel     = 0x10000000
IP_Invulnerability   = 0x80000000
IP_Vampiric          = 0x00000001
IP_Lightning         = 0x00000004
IP_Destruction       = 0x00000010
IP_Power_Drain       = 0x01000000
IP_Elemental_Armour  = 0x00000400
IP_Magic_Immunity    = 0x00080000
IP_Righteousness     = 0x40000000
IP_Merging           = 0x20000000
IP_Flight            = 0x00020000
IP_Pathfinding       = 0x00000080
IP_Guardian_Wind     = 0x00000002
IP_Giant_Strength    = 0x08000000
IP_Holy_Avenger      = 0x00200000
IP_Death             = 0x00010000
IP_Flaming           = 0x00100000
IP_Cloak_Of_Fear     = 0x00000008
IP_Resist_Elements   = 0x00000200
IP_Stoning           = 0x00001000
IP_Resist_Magic      = 0x00040000
IP_Cloak_Of_Fear     = 0x00000008
IP_Bless             = 0x02000000
IP_Endurance         = 0x00002000
IP_Water_Walking     = 0x00000100


32-bit Item Power Bitfield Masks
IP_Vampiric          = 0x00000001
IP_Guardian_Wind     = 0x00000002
IP_Lightning         = 0x00000004
IP_Cloak_Of_Fear     = 0x00000008
IP_Destruction       = 0x00000010
IP_Wraith_Form       = 0x00000020
IP_Regeneration      = 0x00000040
IP_Pathfinding       = 0x00000080
IP_Water_Walking     = 0x00000100
IP_Resist_Elements   = 0x00000200
IP_Elemental_Armour  = 0x00000400
IP_Chaos             = 0x00000800
IP_Stoning           = 0x00001000
IP_Endurance         = 0x00002000
IP_Haste             = 0x00004000
IP_Invisibility      = 0x00008000
IP_Death             = 0x00010000
IP_Flight            = 0x00020000
IP_Resist_Magic      = 0x00040000
IP_Magic_Immunity    = 0x00080000
IP_Flaming           = 0x00100000
IP_Holy_Avenger      = 0x00200000
IP_True_Sight        = 0x00400000
IP_Phantasmal        = 0x00800000
IP_Power_Drain       = 0x01000000
IP_Bless             = 0x02000000
IP_Lion_Heart        = 0x04000000
IP_Giant_Strength    = 0x08000000
IP_Planar_Travel     = 0x10000000
IP_Merging           = 0x20000000
IP_Righteousness     = 0x40000000
IP_Invulnerability   = 0x80000000


enum IPOW_LO (bitfield)
00000001    IP_Vampiric         = 0x0001
00000002    IP_Guardian_Wind    = 0x0002
00000004    IP_Lightning        = 0x0004
00000008    IP_Cloak_Of_Fear    = 0x0008
00000010    IP_Destruction      = 0x0010
00000020    IP_Wraith_Form      = 0x0020
00000040    IP_Regeneration     = 0x0040
00000080    IP_Pathfinding      = 0x0080
00000100    IP_Water_Walking    = 0x0100
00000200    IP_Resist_Elements  = 0x0200
00000400    IP_Elemental_Armour = 0x0400
00000800    IP_Chaos            = 0x0800
00001000    IP_Stoning          = 0x1000
00002000    IP_Endurance        = 0x2000
00004000    IP_Haste            = 0x4000
00008000    IP_Invisibility     = 0x8000

enum IPOW_HI (bitfield)
00000001    IP_Death             = 0x1000
00000002    IP_Flight            = 0x2000
00000004    IP_Resist_Magic      = 0x4000
00000008    IP_Magic_Immunity    = 0x8000
00000010    IP_Flaming           = 10h
00000020    IP_Holy_Avenger      = 20h
00000040    IP_True_Sight        = 40h
00000080    IP_Phantasmal        = 80h
00000100    IP_Power_Drain       = 100h
00000200    IP_Bless             = 200h
00000400    IP_Lion_Heart        = 400h
00000800    IP_Giant_Strength    = 800h
00001000    IP_Planar_Travel     = 1000h
00002000    IP_Merging           = 2000h
00004000    IP_Righteousness     = 4000h
00008000    IP_Invulnerability   = 8000h





### GAME_AwardItems()
; copies the passed items into the global award array
; and opens the items screen for the player to process them
; WARNING: can only handle 6 items at a time

¿ Item module scoped variables ?
    ITEM_AwardPending
    ITEM_AwardCount
    ITEM_AwardListIndex
    ITEM_AwardList
    ITEM_CurrentAward
    e.g., copies the passed in item_list[] to ITEM_AwardList[]

ITEM_AwardPending
    WZD dseg:C26C  ovr075
    flag {ST_FALSE,ST_TRUE}
    only checked in Item_Screen()  (along with m_cursor_item_idx)


ITEM_CurrentAward
    undefined in 'Army List Screen', when the 'Items Button' is clicked
    used to set m_cursor_item_idx, so must be item_idx for _ITEMS[]
    sets ITEM_CurrentAward from ITEM_AwardList[ITEM_AwardListIndex]
        then sets m_cursor_item_idx to ITEM_CurrentAward

m_cursor_item_idx
    used in ITEM_ScreenControls() as ~ cursor not available to click on "Alchemy" or "OK"
    used to set the mouse cursor to 'None', as in the cursor is the item so don't mess with it
    Move_Item() swaps m_cursor_item_idx with Item_Location_Pointer


### WIZ_AwardItems()
; processes the passed list of items being awarded to
; the selected wizard - either through the items screen
; if it's the human player, or AI_ProcessItems
...
called from Combat(), with an item list that comes from either combat function called End_Of_Combat()
¿ looks to be most handling items from heroes that have been killed ?
for Lair_Combat()
    local item_list comes from EZ_GenerateRewards()
    gets passed to GAME_AwardItems(), with _LAIRS[].Item_Count as item_count








### Check_Same_Location()

; checks whether the item destination is at the same location as the source,
; using slot index 6 for the fortress vault and 7 for overland battle spoils
; returns 1 if the item is at the destination location or 0 if it is not


if Check_Same_Location() == ST_TRUE
    Can_Equip = ST_TRUE
    if Can_Equip == ST_TRUE
        do equip/exchange
elsewhere, Item_Pickup is equivalent for do pickup

XREF:
    NX_j_Check_Same_Location()
    Item_Screen_Draw()
    Move_Item()

Item_Screen_Draw()
    if(m_item_slot_idx > -1)
    |-> Check_Same_Location(m_item_slot_idx, itr_heroes)

m_item_slot_idx = {-1, 6, 7}

Item_Screen()           mov     [m_item_slot_idx], -1
Item_Screen()           mov     [m_item_slot_idx],  6
Item_Screen()           mov     [m_item_slot_idx],  7
Item_Screen_Draw()      cmp     [m_item_slot_idx], -1                    
Item_Screen_Draw()      push    [m_item_slot_idx]
ITEM_Dismantle()        mov     [m_item_slot_idx], -1
Move_Item()   mov     [m_item_slot_idx], _SI_Source_Slot
Move_Item()   push    [m_item_slot_idx]
Move_Item()   mov     [m_item_slot_idx], -1
Move_Item()   mov     [m_item_slot_idx], _SI_Source_Slot

Move_Item()
...
Item_Screen()
    Move_Item(itr_hero_slots, unit_idx);
So, ...
    drake178 means "slot index"/"Source_Slot" as in "Hero Slot"  (NOT "Item Slot")
Player Hero Slot Index
Hero Item Slot Index

if src_hero_slot_idx is 7 / Overland
    uses m_item_wx, m_item_wy, m_item_wp
dseg:C300 00 00                                           m_item_wp dw 0
dseg:C302 00 00                                           m_item_wy dw 0
dseg:C304 00 00                                           m_item_wx dw 0
dseg:C306 00 00                                           ITEM_MapLocMove dw 0





### Move_Item()

XREF:
    NX_j_Move_Item()
    Item_Screen()

; picks up, puts down, or swaps an item with the one on the mouse cursor, if possible
; BUG: reads out of bounds memory every time the vault is accessed (luckily no writes or gameplay effect)
; BUGs inherited from the function checking whether an item can be removed from a hero without drowning

Move_Item(hero_slot_idx, item_slot_idx)

Item_Screen()
    checks for a left click on an item
    loops through hero slots and item slots
    |-> Move_Item(itr_hero_slots, itr_hero_items)

Item_Screen()
    checks for a left click on an item
    loops through vault slots
    |-> Move_Item(VAULT_HERO_SLOT_IDX, itr_vault_slots)


### Check_Wont_Drown()

int16_t Check_Wont_Drown(int16_t hero_slot_idx, int16_t item_idx)

has a handful of bugs
    a few are bogus/bad/gnarly
uses creepy Unit_Has_WindWalkingItem empty function pointer

Move_Item()
    |-> Check_Wont_Drown()





##### Naming Things Is Hard


Page 27 (PDF Page 32)  

ITEMS  
Clicking on the items button takes you to the items screen.  
The items screen allows you to view, move and destroy artifacts  
that are currently being used by your heroes  
or in storage in your fortress vault  
(a holding area for unassigned items).  


Page 28 (PDF Page 33)  

Right-clicking on any item in the screen  
displays a list of attribute enhancements  
the equipped item provides.  



If you obtain too many artifacts to store or divide among your heroes,  
you may break extra artifacts  
by picking them up and clicking them on the anvil  
in the lower left corner of the items screen.  
Breaking an artifact over the anvil  
adds one half the base creation cost of the artifact in mana  
to your mana reserve (see Artifacts).  
