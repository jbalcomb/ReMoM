

10 item types
    7 misc item sub-types
33 item powers
    ¿ 33 item powers vs. 67 ITEMPOW records ?
    used to index into _ITEM_POWERS[]
    ITEM_CreateRandom()
    _ITEM_POWERS = (struct s_ITEM_POWER * )Near_Allocate_First(2010);  // 2010 B;  67 * 30 = 2010
    LBX_Load_Data_Static(itempow_lbx_file, 0, (SAMB_ptr)_ITEM_POWERS, 0, 67, 30);


// start, count
int16_t CRP_ITEM_IconRanges_2[20] = 
{
      0,  9,    // Sword  
      9, 11,    // Mace
     20,  9,    // Axe
     29,  9,    // Bow
     38,  9,    // Staff
    107,  9,    // Wand
     72, 35,    // Misc
     62, 10,    // Shield
     47,  8,    // Chain
     55,  7     // Plate
};






### _ITEM_POWERS

indexed by item_power_idx = (Random(33) - 1);

ITEM_CreateRandom()
_ITEM_POWERS = (struct s_ITEM_POWER * )Near_Allocate_First(2010);  // 2010 B;  67 * 30 = 2010
LBX_Load_Data_Static(itempow_lbx_file, 0, (SAMB_ptr)_ITEM_POWERS, 0, 67, 30);

Zero to Thirty-Three are all:
    {Attack, Defense, To Hit, Movement, Resistance, Spell Skill, Spell Save}
¿ "basic" ?











ITEM_CreateRandom()

Item_Type = (Random(10) - 1);  // {0, ..., 9}

Power_Index = (Random(33) - 1);

Item Icon Index

    Random(Icon_Range_Count[Item_Type]);

    ITEM_Crafting_Icon = (Icon_Range_Start[Item_Type] - 1);




#### ITEM_GetDefaultName()

convert item type bitfield value to item type value  ~ bit position
e.g.,
    0x00000200  0000001000000000
                      >>>>>>>>>   9
obverse of (1 << it_Platemail)







enum eITEM_TYPE
{
    it_Sword  = 0,
    it_Mace  = 1,
    it_Axe  = 2,
    it_Bow  = 3,
    it_Staff  = 4,
    it_Wand  = 5,
    it_Misc  = 6,
    it_Shield  = 7,
    it_Chainmail  = 8,
    it_Platemail  = 9
};

00000001 ; enum I_TypeBits (bitfield)
00000001 b_Sword  = 1
00000002 b_Mace  = 2
00000004 b_Axe  = 4
00000008 b_Bow  = 8
00000010 b_Staff  =      10h
00000020 b_Wand  =       20h
00000040 b_Misc_Item  =  40h
00000080 b_Shield  =     80h
00000100 b_Chainmail  = 100h
00000200 b_Platemail  = 200h

1, 2, 4, 8, 16, 32, 64, 128, 256, 512
00000000 00000001    1  0x0001
00000000 00000010    2  0x0002
00000000 00000100    4  0x0004
00000000 00001000    8  0x0008
00000000 00010000   16  0x0010
00000000 00100000   32  0x0020
00000000 01000000   64  0x0040
00000000 10000000  128  0x0080
00000001 00000000  256  0x0100       
00000010 00000000  512  0x0200

Sword    9
Mace    11
Axe      9
Bow      9
Staff    9
Chain    9
Plate    7
Shield  10
Pendant  2
Brooch   4
Ring     6
Cloak    6
Gauntlet 4
Helmet   7
Orb      6
Wand     8


enum Item_Icon
    Sword_[1-9]       0,   1,   2,   3,   4,   5,   6,   7,   8
    Mace_[01-11]      9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19
    Axe_[1-9]        20,  21,  22,  23,  24,  25,  26,  27,  28
    Bow_[1-9]        29,  30,  31,  32,  33,  34,  35,  36,  37
    Staff_[1-9]      38,  39,  40,  41,  42,  43,  44,  45,  46
    Chain_[1-8]      47,  48,  49,  50,  51,  52,  53,  54
    Plate_[1-7]      55,  56,  57,  58,  59,  60,  61
    Shield_[1-10]    62,  63,  64,  65,  66,  67,  68,  69,  70,  71
    Pendant_[1-2]    72,  73
    Brooch_[1-4]     74,  75,  76,  77
    Ring_[1-6]       78,  79,  80,  81,  82, 83
    Cloak_[1-6]      84,  85,  86,  87,  88, 89
    Gauntlet_[1-4]   90,  91,  92,  93
    Helmet_[1-7]     94,  95,  96,  97,  98,  99, 100
    Orb_[1-6]       101, 102, 103, 104, 105, 106
    Wand_[1-8]      107, 108, 109, 110, 111, 112, 113, 114






##### Naming Things Is Hard

"Enchant Item"
"Create Artifact"
"special powers"
"purchase ... merchant"  ("...hire...mercenaries...heroes...")


Page 7  (PDF Page 12)  

In addition to the five magic types noted above,  
  there is a sixth type, arcane magic,  
  that requires no spell ranks to learn.  
Arcane magic is available to all spell casters.  
· Arcane magic is the most basic of all magic on Arcanus and Myrror.  
Its spells allow wizards to summon heroes (see Heroes),
  to create amazing objects of great power (see Artifacts)  
  and to cast basic magic spells, such as those that remove spells that have been cast on your wizard by enemies.  


Page 28  (PDF Page 33)

IMPORTANT: The three slots next to each hero’s portrait are limited to  
holding specific types of items. Some slots can only hold special items  
(such as rings and orbs), while others are suited to holding armor (mail and  
shields). Below is a chart of the slots and the sorts of items they can hold.  
Note that the weapons and armor a hero can be equipped with depend on  
the hero’s type; some heroes are mages, others are warriors, etc.  

Slot For Artifact       Artifacts Allowed


Page 78  (PDF Page 83)

HEROES

Heroes are mercenaries of exceptional abilities. Not only can they  
wield powerful artifacts (see Artifacts), but they also frequently  
possess unique skills that enhance the controlling wizard’s capabilities.  


Page 79  (PDF Page 84)

When heroes die, the powerful artifacts they carry become the property of the victor.  
If, however, the hero dies or disappears because one of the following spells is cast:  
banish, cracks call, disintegrate, dispel evil, holy word or unsummon,  
any items he or she is carrying disappear with the hero.  


Page 85  (PDF Page 90)

Artifacts
The artifacts that you currently have in your possession can be  
viewed by entering the items screen from the armies screen.  
To view an item, right-click on its picture.  
Artifacts can be used by heroes  
(artifacts are placed in slots next to the heroes’ portraits in the items screen)  
or destroyed  
(clicking an artifact over the anvil in the items screen)  
to obtain mana (see Items).  
Artifacts can be acquired in one of several ways:  
as treasure from creature lairs and ancient ruins,  
from merchants and  
by casting the arcane magic spells enchant item or create artifact.  




Page 8  (PDF Page 10)  

Create Artifact:  
Arcane. Casting Cost: Varies by Enchantments Chosen;  
Upkeep: None. Rare.  
Allows you to create a powerful magic item for your heroes.  
See Artifact Creation.  

Page 13  (PDF Page 15)  

Enchant Item:  
Arcane. Casting Cost: Varies by Enchantment; Upkeep: None.  
Uncommon.  
Allows you to create a magic item for your heroes.
Enchant item is weaker than create artifact  
and cannot be used to create magic items  
with special powers that cost more than 200 mana each to embed.  


ITEMDATA.LBX  


ITEMPOW.LBX  

