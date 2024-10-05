/*
    Unit View

    WIZARDS.EXE
        ovr072
        ovr078
        ovr089
*/
/*

USW_HeroAbilities is XREF'd by ovr102 Hero_Build_Specials_List

*/
/*
    HELP

in order, except 0x273  627
USW_HeroAbilities
0x0203  515
...
0x020E  526

USW_Abilities
0x018C  396
...
0x019A  410


USW_Attributes_1

*/

#include "MoM.H"
#include "UnitStat.H"
#include "UnitView.H"
#include <intrin.h>



// drake178: USW_Ability
// sizeof:  08h  8d
struct USW_Ability
{
    /* 00 */  char ** name;
    /* 02 */  int16_t bit_idx;  // enum ABL_FLAGS  ... tested against global_battle_unit->Abilities, which is copied from unit_type_table.Abilities
    /* 04 */  uint8_t icon_idx;  // 
    /* 05 */  // 2-byte alignment padding
    /* 06 */  int16_t help_idx;
    /* 08 */
};


// drake178: USW_Attribute_1
// sizeof:  08h  8d
struct USW_Attribute_1
{
    /* 00 */ char ** name;
    /* 02 */ int16_t bit_idx;  // enum ATTRIB_1
    /* 04 */ uint8_t icon_idx;
    /* 05 */  // 2-byte alignment padding
    /* 06 */ int16_t help_idx;
    /* 08 */
};


// drake178: USW_Attribute_2
// sizeof:  08h  8d
struct USW_Attribute_2
{
    /* 00 */ char ** name;
    /* 02 */ int16_t bit_idx;  // enum ATTRIB_2
    /* 04 */ uint8_t icon_idx;
    /* 05 */  // 2-byte alignment padding
    /* 06 */ int16_t help_idx;
    /* 08 */
};


// drake178: USW_AttackFlag
// sizeof:  08h  8d
struct USW_AttackFlag
{
    /* 00 */ char ** name;
    /* 02 */ int16_t bit_idx;  // enum ATK_FLAGS
    /* 04 */ uint8_t icon_idx;
    /* 05 */  // 2-byte alignment padding
    /* 06 */ int16_t help_idx;
    /* 08 */
};


// sizeof:  06h  6d
#pragma pack(push)
#pragma pack(2)
struct s_UNIT_MUTATION
{
    /* 00 */ char ** name;
    /* 02 */ int8_t flag;  // enum MUT_FLAGS
    /* 03 */ uint8_t icon_idx;
    /* 04 */ int16_t help_idx;
    /* 06 */
};
#pragma pack(pop)


// drake178: USW_SR_Attack
// sizeof:  06h  6d
struct USW_SR_Attack
{
    /* 00 */ char ** name;
    /* 02 */ int8_t Ranged_Type;  // enum Ranged_Type
    /* 03 */ uint8_t icon_idx;
    /* 04 */ int16_t help_idx;
    /* 06 */
};


// sizeof:  8h  8d
struct s_ITEM_VIEW_TEXT
{
/* 00 */ char * Name;
/* 02 */ char * Text;
/* 04 */ uint32_t Flags;  // ~== s_ITEM.Powers
/* 08 */
};





/*
¿ BEGIN:  ovr078  UV / USW ?
WZD dseg:3A50

USW_HeroAbilities
...
...
...
*/


// WZD dseg:4064
char cnst_LargeShield[] = "Large Shield";
char cnst_PlaneShift[] = "Plane Shift";
char cnst_WallCrusher[] = "Wall Crusher";
char cnst_Healer_2[] = "Healer";
char cnst_CreateOutpost[] = "Create Outpost";
char cnst_Invisibility[] = "Invisibility";
char cnst_CreateUndead[] = "Create Undead";
char cnst_Undead[] = "Undead";
char cnst_LongRange[] = "Long Range";
char cnst_LandCorruption[] = "Land Corruption";
char cnst_MeldWithNode[] = "Meld With Node";
char cnst_NonCorporeal[] = "Non Corporeal";
char cnst_WindWalking[] = "Wind Walking";
char cnst_Regeneration[] = "Regeneration";
char cnst_Purify[] = "Purify";
char cnst_Negate1stStrike[] = "Negate First Strike";

char cnst_Lucky[] = "Lucky";
char cnst_PoisonImmunity[] = "Poison Immunity";
char cnst_FireImmunity[] = "Fire Immunity";
char cnst_StoningImmunity[] = "Stoning Immunity";
char cnst_WeaponImmunity[] = "Weapon Immunity";
char cnst_MissileImmunity[] = "Missiles Immunity";
char cnst_IllusionsImm[] = "Illusions Immunity";
char cnst_ColdImmunity[] = "Cold Immunity";
char cnst_MagicImmunity[] = "Magic Immunity";
char cnst_DeathImmunity[] = "Death Immunity";

char cnst_HealingSpell[] = "Healing Spell";
char cnst_Spell_2[] = "Spell";
char cnst_FireBallSpell[] = "Fire Ball Spell";
char cnst_DoomboltSpell[] = "Doombolt Spell";
char cnst_Immolation[] = "Immolation";
char cnst_CauseFearSpell[] = "Cause Fear Spell";
char cnst_WebSpell[] = "Web Spell";
char cnst_ResistanceToAll[] = "Resistance To All";
char cnst_HolyBonus[] = "Holy Bonus";

char cnst_ArmorPiercing[] = "Armor Piercing";
char cnst_Poison[] = "Poison";
char cnst_LifeSteal[] = "Life Steal";
char cnst_AutomaticDmg[] = "Automatic Damage";
char cnst_Destruction[] = "Destruction";
char cnst_Illusion[] = "Illusion";
char cnst_StoningTouch[] = "Stoning Touch";
char cnst_DeathTouch[] = "Death Touch";
char cnst_PowerDrain[] = "Power Drain";
char cnst_DispelEvil[] = "Dispel Evil";



// WZD dseg:42A7
char cnst_ChaosChannel[] = "Chaos Channel";



// WZD dseg:42B5
char cnst_Thrown[] = "Thrown";
char cnst_FireBreath[] = "Fire Breath";
char cnst_Lightning[] = "Lightning";
char cnst_StoningGaze[] = "Stoning Gaze";
char cnst_DoomGaze[] = "Doom Gaze";
char cnst_DeathGaze[] = "Death Gaze";

// WZD dseg:4064 4C 61 72 67 65 20 53 68 69 65 6C 64             cnst_LargeShield db 'Large Shield'      ; DATA XREF: dseg:USW_Abilitieso



// WZD dseg:4070
/*
XREF'd by all three functions in ovr078
so, this is the boundary of the code module for the compiler's string optimizer/deduper
USW_Build_Effect_List:loc_679A8 mov     ax, offset cnst_ZeroString_18
Prod_Build_Specials_List+B      mov     ax, offset cnst_ZeroString_18
ITEM_GetPowerNames:loc_69DAA    mov     ax, offset cnst_ZeroString_18
*/
char empty_string_ovr078[] = "";

// WZD dseg:422C
// pilfered from cnst_HolyBonus
char cnst_Ammo_Plural[] = "s";





// WZD dseg:42F4
char cnst_GuardianWind[] = "Guardian Wind";
// WZD dseg:4302
char cnst_Berserk[] = "Berserk";
// WZD dseg:430A
char cnst_CloakOfFear[] = "Cloak Of Fear";
// WZD dseg:4318
char cnst_BlackChannels[] = "Black Channels";
// WZD dseg:4327
char cnst_WraithForm[] = "Wraith Form";
// WZD dseg:4333
char cnst_Regenerate[] = "Regenerate";
// WZD dseg:433E
char cnst_PathFinding[] = "Path Finding";
// WZD dseg:434B
char cnst_WaterWalking[] = "Water Walking";
// WZD dseg:4359
char cnst_ElementalArmor[] = "Elemental Armor";
// WZD dseg:4369
char cnst_ResistElements[] = "Resist Elements";
// WZD dseg:4379
char cnst_StoneSkin[] = "Stone Skin";
// WZD dseg:4384
char cnst_IronSkin[] = "Iron Skin";
// WZD dseg:438E
char cnst_Endurance[] = "Endurance";
// WZD dseg:4398
char cnst_SpellLock[] = "Spell Lock";
// WZD dseg:43A3
char cnst_Flight[] = "Flight";
// WZD dseg:43AA
char cnst_ResistMagic[] = "Resist Magic";
// WZD dseg:43B7
char cnst_FlameBlade[] = "Flame Blade";
// WZD dseg:43C3
char cnst_EldritchWeapon[] = "Eldritch Weapon";
// WZD dseg:43D3
char cnst_TrueSight[] = "True Sight";
// WZD dseg:43DE
char cnst_HolyWeapon[] = "Holy Weapon";
// WZD dseg:43EA
char cnst_Heroism[] = "Heroism";
// WZD dseg:43F2
char cnst_Bless[] = "Bless";
// WZD dseg:43F8
char cnst_LionHeart[] = "Lion Heart";
// WZD dseg:4403
char cnst_GiantStrength[] = "Giant Strength";
// WZD dseg:4412
char cnst_PlanarTravel[] = "Planar Travel";
// WZD dseg:4420
char cnst_HolyArmor[] = "Holy Armor";
// WZD dseg:442B
char cnst_Righteousness[] = "Righteousness";
// WZD dseg:4439
char cnst_Invulnerability[] = "Invulnerability";

// WZD dseg:4449
char cnst_Vertigo[] = "Vertigo";
// WZD dseg:4451
char cnst_Confusion[] = "Confusion";
// WZD dseg:445B
char cnst_Whirlwind[] = "Whirlwind";
// WZD dseg:4465
char cnst_MindStorm[] = "Mind Storm";
// WZD dseg:4470
char cnst_Shatter[] = "Shatter";
// WZD dseg:4478
char cnst_Weakness[] = "Weakness";
// WZD dseg:4481
char cnst_BlackSleep[] = "Black Sleep";
// WZD dseg:448D
char cnst_WarpCreature[] = "Warp Creature";
// WZD dseg:449B
char cnst_MindTwist[] = "Mind Twist";
// WZD dseg:44A6
char cnst_Haste[] = "Haste";
// WZD dseg:44AC
char cnst_Web[] = "Web";
// WZD dseg:44B0
char cnst_CreatureBinding[] = "Creature Binding";
// WZD dseg:44C1
char cnst_Possession[] = "Posession";
// WZD dseg:44CB
char cnst_Merging[] = "Merging";

// WZD dseg:44D3
char cnst_UnitLevel_1[] = "Recruit";
// WZD dseg:44DB
char cnst_UnitLevel_2[] = "Regular";
// WZD dseg:44E3
char cnst_UnitLevel_3[] = "Veteran";
// WZD dseg:44EB
char cnst_UnitLevel_4[] = "Elite";
// WZD dseg:44F1
char cnst_UnitLevel_5[] = "Ultra Elite";
// WZD dseg:44FD
char cnst_UnitLevel_6[] = "Champion";
// WZD dseg:4506
char cnst_HeroLevel_1[] = "Hero";
// WZD dseg:450B
char cnst_HeroLevel_2[] = "Myrmidon";
// WZD dseg:4514
char cnst_HeroLevel_3[] = "Captain";
// WZD dseg:451C
char cnst_HeroLevel_4[] = "Commander";
// WZD dseg:4526
char cnst_HeroLevel_6[] = "Lord";
// WZD dseg:452B
char cnst_HeroLevel_7[] = "Grand Lord";
// WZD dseg:4536
char cnst_HeroLevel_8[] = "Super Hero";
// WZD dseg:4541
char cnst_HeroLevel_9[] = "Demi-God";

// WZD dseg:454A
char cnst_Rank_I[] = "I";
// WZD dseg:454C
char cnst_Rank_II[] = "II";
// WZD dseg:454F
char cnst_Rank_III[] = "III";
// WZD dseg:4553
char cnst_Rank_IV[] = "IV";
// WZD dseg:4556
char cnst_SpaceOpenBrace2[] = " (";

// WZD dseg:4559
char cnst_ExperiencePts[] = " ep)";
// WZD dseg:455C
char cnst_ClosingBrace_3[] = ")";

// WZD dseg:455E
char cnst_Leadership[] = "Leadership";
// WZD dseg:4569
char cnst_SuperAbStar[] = "}";
// WZD dseg:456B
char cnst_Legendary[] = "Legendary";
// WZD dseg:4575
char cnst_Blademaster[] = "Blademaster";
// WZD dseg:4581
char cnst_Armsmaster[] = "Armsmaster";
// WZD dseg:458C
char cnst_Constitution[] = "Constitution";
// WZD dseg:4599
char cnst_Might[] = "Might";
// WZD dseg:459F
char cnst_ArcanePower[] = "Arcane Power";
// WZD dseg:45AC
char cnst_Sage_2[] = "Sage";

// WZD dseg:45B1
char cnst_Space_3[] = " ";

// WZD dseg:45B3
char cnst_ResearchPoints[] = " rp";
// WZD dseg:45B7
char cnst_Prayermaster[] = "Prayermaster";
// WZD dseg:45C4
char cnst_Agility[] = "Agility";
// WZD dseg:45CC
char cnst_Charmed[] = "Charmed";
// WZD dseg:45D4
char cnst_Noble[] = "Noble";

// WZD dseg:45DA
char cnst_SpacePlus[] = " +";

// WZD dseg:45DB
char cnst_Plus_1[] = "+";

// WZD dseg:45DD
char cnst_Caster[] = "Caster";
// WZD dseg:45E4
char cnst_Space_mp[] = " mp";
// WZD dseg:45E8
char cnst_Rock[] = "Rock";
// WZD dseg:45ED
char cnst_Arrow[] = "Arrow";
// WZD dseg:45F3
char cnst_Bullet[] = "Bullet";

// WZD dseg:45FA
char cnst_TimesSign[] = " x";

// WZD dseg:45FD
char cnst_ToHit[] = " To Hit";

// WZD dseg:4605
char cnst_Construction[] = "Construction";
// WZD dseg:4612
char cnst_Scouting[] = "Scouting";
// WZD dseg:461B
char cnst_Teleporting[] = "Teleporting";
// WZD dseg:4627
char cnst_Pathfinding[] = "Pathfinding";
// WZD dseg:4633
char cnst_Mountaineer[] = "Mountaineer";
// WZD dseg:463F
char cnst_Forester[] = "Forester";
// WZD dseg:4648
char cnst_SummonDemons[] = "Summon Demons";
// WZD dseg:4656
char cnst_Stasis[] = "Stasis";

// WZD dseg:465D
char cnst_Vampiric[] = "Vampiric";
// WZD dseg:4666
char cnst_VampiricText[] = ", life draining";
// WZD dseg:4676
char cnst_LightningText[] = ", armor piercing";
// WZD dseg:4687
char cnst_CloakofFear[] = "Cloak of Fear";
// WZD dseg:4695
char cnst_CoF_Text[] = ", as cloak of fear spell";
// WZD dseg:46AE
char cnst_DestructionText[] = ", resist or be disintegrated";
// WZD dseg:46CB
char cnst_Wraithform[] = "Wraithform";
// WZD dseg:46D6
char cnst_WraithformText[] = ", as wraithform spell";
// WZD dseg:46EC
char cnst_RegenerateText[] = ", as regeneration spell";
// WZD dseg:4704
char cnst_PF_Text[] = ", as pathfinding spell";
// WZD dseg:471B
char cnst_WaterWalkText[] = ", as water walking spell";
// WZD dseg:4734
char cnst_ResistElemText[] = ", as resist elements spell";
// WZD dseg:474F
char cnst_ElemArmorText[] = ", as elemental armor spell";
// WZD dseg:476A
char cnst_Doom[] = "Doom";
// WZD dseg:476F
char cnst_DoomText[] = ", ignores armor";
// WZD dseg:477F
char cnst_EnduranceText[] = ", as swiftness spell";  // ; BUG: there is no swiftness spell in the game
// WZD dseg:4794
char cnst_HasteText[] = ", doubles attacks";
// WZD dseg:47A6
char cnst_InvisText[] = ", as the invisibility spell";
// WZD dseg:47C2
char cnst_Death[] = "Death";
// WZD dseg:47C8
char cnst_DeathText[] = ", save at -3 or die";
// WZD dseg:47DC
char cnst_FlightText[] = ", as flight spell";
// WZD dseg:47EE
char cnst_ResMagicText[] = ", as resist magic spell";
// WZD dseg:4806
char cnst_MagicImmText[] = ", as magic immunity spell";
// WZD dseg:4820
char cnst_PDrainText[] = ", target loses 2-20 mana";
// WZD dseg:4839
char cnst_HolyAvenger[] = "Holy Avenger";
// WZD dseg:4846
char cnst_HolyAvengerText[] = ", as bless & dispel evil spell";
// WZD dseg:4865
char cnst_TrueSightText[] = ", as true sight spell";
// WZD dseg:487B
char cnst_Phantasmal[] = "Phantasmal";
// WZD dseg:4886
char cnst_PhantasmalText[] = ", ignores armor & illusion attacks";
// WZD dseg:48A9
char cnst_Flaming[] = "Flaming";
// WZD dseg:48B1
char cnst_FlamingText[] = ", +3 attack strength";
// WZD dseg:48C6
char cnst_BlessText[] = ", as bless spell";
// WZD dseg:48D7
char cnst_LionheartText[] = ", as lion heart spell";
// WZD dseg:48ED
char cnst_Stoning[] = "Stoning";
// WZD dseg:48F5
char cnst_StoningText[] = ", resist at -1 or be stoned";
// WZD dseg:4911
char cnst_PlanarTrvlText[] = ", as planar travel spell";
// WZD dseg:492A
char cnst_MergingText[] = ", as merging spell";  // ; BUG: there is no merging spell in the game
// WZD dseg:493D
char cnst_RightsnssText[] = ", as righteousness spell";
// WZD dseg:4956
char cnst_InvulnText[] = ", as invulnerability spell";
// WZD dseg:4971
char cnst_GWindText[] = ", as guardian wind spell";
// WZD dseg:498A
char cnst_GStrText[] = ", as giant strength spell";
// WZD dseg:49A4
char cnst_Space_ATK[] = " Attack";
// WZD dseg:49AC
char cnst_Space_DEF[] = " Defense";
// WZD dseg:49B5
char cnst_Space_Moves[] = " Movement";
// WZD dseg:49BF
char cnst_Space_RES[] = " Resistance";
// WZD dseg:49CB
char cnst_Space_Skill[] = " Spell Skill";
// WZD dseg:49D8
char cnst_MinusSign[] = "-";
// WZD dseg:49DA
char cnst_Space_SAVE[] = " Spell Save";

// WZD dseg:49DA                                                 ¿ END:  ovr078 - String (UV / USW) ?





// WZD dseg:3A50                                                 ¿ BEGIN:  ovr078  UV / USW ?

// WZD dseg:3A50
// drake178: struct USW_HeroAbl USW_HeroAbilities
struct USW_HeroAbl USW_HeroAbilities[23] =
{
/*    flags        pict  help               */
    { 0x00000001,   43,  HLP_LEADERSHIP   },
    { 0x00000002,   43,  HLP_LEADERSHIP   },
    { 0x00000008,   45,  HLP_LEGENDARY    },
    { 0x00000010,   45,  HLP_LEGENDARY    },
    { 0x00000040,   47,  HLP_BLADEMASTER  },
    { 0x00000080,   47,  HLP_BLADEMASTER  },
    { 0x00000200,   46,  HLP_ARMSMASTER   },
    { 0x00000400,   46,  HLP_ARMSMASTER   },
    { 0x00001000,   50,  HLP_CONSTITUTION },
    { 0x00002000,   50,  HLP_CONSTITUTION },
    { 0x00008000,   52,  HLP_MIGHT        },
    { 0x00010000,   52,  HLP_MIGHT        },
    { 0x00040000,   54,  HLP_ARCANE_POWER },
    { 0x00080000,   54,  HLP_ARCANE_POWER },
    { 0x00200000,   61,  HLP_SAGE         },
    { 0x00400000,   61,  HLP_SAGE         },
    { 0x01000000,   57,  HLP_PRAYERMASTER },
    { 0x02000000,   57,  HLP_PRAYERMASTER },
    { 0x80000000,  143,  HLP_AGILE        },  /* out of order */
    { 0x04000000,  143,  HLP_AGILE        },  /* out of order */
    { 0x08000000,   58,  HLP_LUCKY_HERO   },
    { 0x10000000,   59,  HLP_CHARMED      },
    { 0x20000000,   60,  HLP_NOBLE        }
};

char * unit_ability_names[15] =
{
    cnst_LargeShield,
    cnst_PlaneShift,
    cnst_WallCrusher,
    cnst_Healer_2,
    cnst_CreateOutpost,
    cnst_Invisibility,
    cnst_CreateUndead,
    cnst_LongRange,
    cnst_LandCorruption,
    cnst_MeldWithNode,
    cnst_NonCorporeal,
    cnst_WindWalking,
    cnst_Regeneration,
    cnst_Purify,
    cnst_Negate1stStrike
};


// WZD dseg:3B08
// drake178: struct USW_Ability USW_Abilities
struct USW_Ability USW_Abilities[15] =
{
/*   name                      flags    pict  help    */
    {&unit_ability_names[ 0],  0x0002,   14,  396},
    {&unit_ability_names[ 1],  0x0004,    4,  0x018D},
    {&unit_ability_names[ 2],  0x0008,   15,  0x018E},
    {&unit_ability_names[ 3],  0x0010,   16,  0x018F},
    {&unit_ability_names[ 4],  0x0020,   17,  0x0190},
    {&unit_ability_names[ 5],  0x0040,   18,  0x0191},
    {&unit_ability_names[ 6],  0x0080,   19,  0x0192},
    {&unit_ability_names[ 7],  0x0100,  129,  0x0193},
    {&unit_ability_names[ 8],  0x0200,   21,  0x0194},
    {&unit_ability_names[ 9],  0x0400,   40,  0x0195},
    {&unit_ability_names[10],  0x0800,   22,  0x0196},
    {&unit_ability_names[11],  0x1000,   23,  0x0197},
    {&unit_ability_names[12],  0x2000,   24,  0x0198},
    {&unit_ability_names[13],  0x4000,   25,  0x0199},
    {&unit_ability_names[14],  0x8000,   48,  0x019A}
};



char * unit_attribute_1_names[10] =
{
    cnst_Lucky,
    cnst_PoisonImmunity,
    cnst_FireImmunity,
    cnst_StoningImmunity,
    cnst_WeaponImmunity,
    cnst_MissileImmunity,
    cnst_IllusionsImm,
    cnst_ColdImmunity,
    cnst_MagicImmunity,
    cnst_DeathImmunity
};

// WZD dseg:3B80
// drake178: struct USW_Attribute_1 USW_Attributes_1
struct USW_Attribute_1 USW_Attributes_1[10] =
{
    { &unit_attribute_1_names[0],  0x0400,  0x007F,  0x019B },
    { &unit_attribute_1_names[1],  0x0080,  0x0005,  0x019C },
    { &unit_attribute_1_names[2],  0x0001,  0x0006,  0x019D },
    { &unit_attribute_1_names[3],  0x0002,  0x0007,  0x019E },
    { &unit_attribute_1_names[4],  0x0100,  0x0008,  0x019F },
    { &unit_attribute_1_names[5],  0x0004,  0x0009,  0x01A0 },
    { &unit_attribute_1_names[6],  0x0008,  0x000A,  0x01A1 },
    { &unit_attribute_1_names[7],  0x0010,  0x000B,  0x01A2 },
    { &unit_attribute_1_names[8],  0x0020,  0x000C,  0x01A3 },
    { &unit_attribute_1_names[9],  0x0040,  0x0031,  0x01A4 }
};

char * unit_attribute_2_names[8] =
{
    cnst_HealingSpell,
    cnst_FireBallSpell,
    cnst_DoomboltSpell,
    cnst_Immolation,
    cnst_CauseFearSpell,
    cnst_WebSpell,
    cnst_ResistanceToAll,
    cnst_HolyBonus
};

// WZD dseg:3BD0
struct USW_Attribute_2 USW_Attributes_2[8] =
{
    { &unit_attribute_2_names[0],  0x0001,  0x0026,  0x01A5 },
    { &unit_attribute_2_names[1],  0x0002,  0x0027,  0x01A6 },
    { &unit_attribute_2_names[2],  0x0004,  0x0029,  0x01A7 },
    { &unit_attribute_2_names[3],  0x0008,  0x0020,  0x01A8 },
    { &unit_attribute_2_names[4],  0x0020,  0x0084,  0x01A9 },
    { &unit_attribute_2_names[5],  0x0010,  0x0083,  0x01AA },
    { &unit_attribute_2_names[6],  0x0040,  0x0021,  0x01AB },
    { &unit_attribute_2_names[7],  0x0080,  0x0022,  0x01AC },
};



/*
    Strings by Address (offset)
        WZD dseg:4064
        WZD dseg:49DA
*/

char * unit_attack_flag_names[11] =
{
    cnst_ArmorPiercing,
    cnst_Poison,
    cnst_LifeSteal,
    cnst_AutomaticDmg,
    cnst_Destruction,
    cnst_Illusion,
    cnst_StoningTouch,
    cnst_DeathTouch,
    cnst_PowerDrain,
    cnst_DispelEvil
};

// WZD dseg:3C10
struct USW_AttackFlag USW_ATK_Flags[11] = 
{
    { &unit_attack_flag_names[ 0],  0x0001,  0x001C,  0x01AD },
    { &unit_attack_flag_names[ 1],  0x0002,  0x001D,  0x01AE },
    { &unit_attack_flag_names[ 2],  0x0004,  0x001E,  0x01AF },
    { &unit_attack_flag_names[ 3],  0x0008,  0x001F,  0x01B0 },
    { &unit_attack_flag_names[ 4],  0x0010,  0x008C,  0x01B1 },
    { &unit_attack_flag_names[ 5],  0x0020,  0x0074,  0x01B2 },
    { &unit_attack_flag_names[ 6],  0x0040,  0x0023,  0x01B3 },
    { &unit_attack_flag_names[ 7],  0x0080,  0x001B,  0x01B4 },
    { &unit_attack_flag_names[ 8],  0x0200,  0x008D,  0x01B5 },
    { &unit_attack_flag_names[ 9],  0x0400,  0x008E,  0x01B6 },
    { &unit_attack_flag_names[10],  0x0800,  0x0085,  0x01B7 }
};



char * unit_mutation_names[] =
{
    cnst_ChaosChannel
};

// WZD dseg:3C68
// { name, flag, icon_idx, help_idx }
struct s_UNIT_MUTATION _unit_mutation_data[3] =
{
    { &unit_mutation_names[0],  0x04,  SPECIAL_ICON_WARPED_DEFENSE,  HLP_WARPED_DEFENSE },
    { &unit_mutation_names[0],  0x08,  SPECIAL_ICON_WARPED_FLIGHT,   HLP_WARPED_FLIGHT  },
    { &unit_mutation_names[0],  0x10,  SPECIAL_ICON_WARPED_BREATH,   HLP_WARPED_BREATH  }
};


char * unit_sr_attack_names[] =
{
    cnst_Thrown,
    cnst_FireBreath,
    cnst_Lightning,
    cnst_StoningGaze,
    cnst_DoomGaze,
    cnst_DeathGaze
};

// WZD dseg:3C7A
struct USW_SR_Attack USW_SR_Attacks[] =
{
    { &unit_sr_attack_names[0], srat_Thrown,      SPECIAL_ICON_THROWN,            HLP_THROWN           },
    { &unit_sr_attack_names[1], srat_FireBreath,  SPECIAL_ICON_FIRE_BREATH,       HLP_FIRE_BREATH      },
    { &unit_sr_attack_names[2], srat_Lightning,   SPECIAL_ICON_LIGHTNING_BREATH,  HLP_LIGHTNING_BREATH },
    { &unit_sr_attack_names[3], srat_StoneGaze,   SPECIAL_ICON_STONING_GAZE,      HLP_STONING_GAZE     },
    { &unit_sr_attack_names[4], srat_MultiGaze,   SPECIAL_ICON_DOOM_GAZE,         HLP_DOOM_GAZE        },
    { &unit_sr_attack_names[5], srat_DeathGaze,   SPECIAL_ICON_DEATH_GAZE,        HLP_DEATH_GAZE       }
};



// WZD dseg:3C9E
// _unit_enchantment_data
// sizeof:  0Ah  10d
#pragma pack(push)
#pragma pack(2)
struct s_UNIT_ENCHANTMENT
{
/* 00 */ char ** name;
/* 02 */ uint32_t flag;  // ¿ enum e_UNIT_ENCHANTMENT ?
/* 06 */ uint8_t icon_idx;
/* 07 */ uint8_t align2_07h;  // 2-byte alignment
/* 08 */ int16_t help_idx;    // ; enum HELP_TXT
/* 0A */
};
#pragma pack(pop)

char * unit_enchantment_names[NUM_UNIT_ENCHANTMENTS] =
{
    cnst_Immolation,
    cnst_GuardianWind,
    cnst_Berserk,
    cnst_CloakOfFear,
    cnst_BlackChannels,
    cnst_WraithForm,
    cnst_Regenerate,
    cnst_PathFinding,
    cnst_WaterWalking,
    cnst_ElementalArmor,
    cnst_ResistElements,
    cnst_StoneSkin,
    cnst_IronSkin,
    cnst_Endurance,
    cnst_SpellLock,
    cnst_Invisibility,
    cnst_WindWalking,
    cnst_Flight,
    cnst_ResistMagic,
    cnst_MagicImmunity,
    cnst_FlameBlade,
    cnst_EldritchWeapon,
    cnst_TrueSight,
    cnst_HolyWeapon,
    cnst_Heroism,
    cnst_Bless,
    cnst_LionHeart,
    cnst_GiantStrength,
    cnst_PlanarTravel,
    cnst_HolyArmor,
    cnst_Righteousness,
    cnst_Invulnerability
};

struct s_UNIT_ENCHANTMENT _unit_enchantment_data[NUM_UNIT_ENCHANTMENTS] =
{
{ &unit_enchantment_names[ 0],  0x00000001,  0x75,  HLP_IMMOLATION_UE      },
{ &unit_enchantment_names[ 1],  0x00000002,  0x76,  HLP_GUARDIAN_WIND      },
{ &unit_enchantment_names[ 2],  0x00000004,  0x80,  HLP_BERSERK            },
{ &unit_enchantment_names[ 3],  0x00000008,  0x61,  HLP_CLOAK_OF_FEAR      },
{ &unit_enchantment_names[ 4],  0x00000010,  0x43,  HLP_BLACK_CHANNELS     },
{ &unit_enchantment_names[ 5],  0x00000020,  0x44,  HLP_WRAITH_FORM        },
{ &unit_enchantment_names[ 6],  0x00000040,  0x45,  HLP_REGENERATE         },
{ &unit_enchantment_names[ 7],  0x00000080,  0x46,  HLP_PATHFINDING_UE     },
{ &unit_enchantment_names[ 8],  0x00000100,  0x47,  HLP_WATER_WALKING      },
{ &unit_enchantment_names[ 9],  0x00000400,  0x49,  HLP_ELEMENTAL_ARMOR    },
{ &unit_enchantment_names[10],  0x00000200,  0x48,  HLP_RESIST_ELEMENTS    },
{ &unit_enchantment_names[11],  0x00000800,  0x4A,  HLP_STONE_SKIN         },
{ &unit_enchantment_names[12],  0x00001000,  0x4B,  HLP_IRON_SKIN          },
{ &unit_enchantment_names[13],  0x00002000,  0x4C,  HLP_ENDURANCE          },
{ &unit_enchantment_names[14],  0x00004000,  0x77,  HLP_SPELL_LOCK         },
{ &unit_enchantment_names[15],  0x00008000,  0x4E,  HLP_INVISIBILITY_UE    },
{ &unit_enchantment_names[16],  0x00010000,  0x4F,  HLP_WIND_WALKING_UE    },
{ &unit_enchantment_names[17],  0x00020000,  0x50,  HLP_FLIGHT             },
{ &unit_enchantment_names[18],  0x00040000,  0x51,  HLP_RESIST_MAGIC       },
{ &unit_enchantment_names[19],  0x00080000,  0x52,  HLP_MAGIC_IMMUNITY_UE  },
{ &unit_enchantment_names[20],  0x00100000,  0x53,  HLP_FLAME_BLADE        },
{ &unit_enchantment_names[21],  0x00200000,  0x54,  HLP_ELDRITCH_WEAPON    },
{ &unit_enchantment_names[22],  0x00400000,  0x55,  HLP_TRUE_SIGHT         },
{ &unit_enchantment_names[23],  0x00800000,  0x56,  HLP_HOLY_WEAPON        },
{ &unit_enchantment_names[24],  0x01000000,  0x57,  HLP_HEROISM            },
{ &unit_enchantment_names[25],  0x02000000,  0x58,  HLP_BLESS              },
{ &unit_enchantment_names[26],  0x04000000,  0x59,  HLP_LION_HEART         },
{ &unit_enchantment_names[27],  0x08000000,  0x41,  HLP_GIANT_STRENGTH     },
{ &unit_enchantment_names[28],  0x10000000,  0x5B,  HLP_PLANAR_TRAVEL_UE   },
{ &unit_enchantment_names[29],  0x20000000,  0x5C,  HLP_HOLY_ARMOR         },
{ &unit_enchantment_names[30],  0x40000000,  0x5D,  HLP_RIGHTEOUSNESS      },
{ &unit_enchantment_names[31],  0x80000000,  0x5E,  HLP_INVULNERABILITY    }
};

/*
_unit_enchantment_data

EB 41 01 00 00 00 75 00 C1 01 F4 42 02 00 00 00
76 00 C2 01 02 43 04 00 00 00 80 00 C3 01 0A 43
08 00 00 00 61 00 C4 01 18 43 10 00 00 00 43 00
C5 01 27 43 20 00 00 00 44 00 C6 01 33 43 40 00
00 00 45 00 C7 01 3E 43 80 00 00 00 46 00 C8 01
4B 43 00 01 00 00 47 00 C9 01 59 43 00 04 00 00
49 00 CA 01 69 43 00 02 00 00 48 00 CB 01 79 43
00 08 00 00 4A 00 CC 01 84 43 00 10 00 00 4B 00
CD 01 8E 43 00 20 00 00 4C 00 CE 01 98 43 00 40
00 00 77 00 CF 01 A0 40 00 80 00 00 4E 00 D0 01
F3 40 00 00 01 00 4F 00 D1 01 A3 43 00 00 02 00
50 00 D2 01 AA 43 00 00 04 00 51 00 D3 01 A0 41
00 00 08 00 52 00 D4 01 B7 43 00 00 10 00 53 00
D5 01 C3 43 00 00 20 00 54 00 D6 01 D3 43 00 00
40 00 55 00 D7 01 DE 43 00 00 80 00 56 00 D8 01
EA 43 00 00 00 01 57 00 D9 01 F2 43 00 00 00 02
58 00 DA 01 F8 43 00 00 00 04 59 00 DB 01 03 44
00 00 00 08 41 00 DC 01 12 44 00 00 00 10 5B 00
DD 01 20 44 00 00 00 20 5C 00 DE 01 2B 44 00 00
00 40 5D 00 DF 01 39 44 00 00 00 80 5E 00 E0 01


s_UNIT_ENCHANTMENT <offset cnst_Immolation, 1, 75h, 0, HLP_IMMOLATION_UE>
s_UNIT_ENCHANTMENT <offset cnst_GuardianWind, 2, 76h, 0, HLP_GUARDIAN_WIND>
s_UNIT_ENCHANTMENT <offset cnst_Berserk, 4, 80h, 0, HLP_BERSERK>
s_UNIT_ENCHANTMENT <offset cnst_CloakOfFear, 8, 61h, 0, HLP_CLOAK_OF_FEAR>
s_UNIT_ENCHANTMENT <offset cnst_BlackChannels, 10h, 43h, 0, HLP_BLACK_CHANNELS>
s_UNIT_ENCHANTMENT <offset cnst_WraithForm, 20h, 44h, 0, HLP_WRAITH_FORM>
s_UNIT_ENCHANTMENT <offset cnst_Regenerate, 40h, 45h, 0, HLP_REGENERATE>
s_UNIT_ENCHANTMENT <offset cnst_PathFinding, 80h, 46h, 0, HLP_PATHFINDING_UE>
s_UNIT_ENCHANTMENT <offset cnst_WaterWalking, 100h, 47h, 0, HLP_WATER_WALKING>
s_UNIT_ENCHANTMENT <offset cnst_ElementalArmor, 400h, 49h, 0, HLP_ELEMENTAL_ARMOR>
s_UNIT_ENCHANTMENT <offset cnst_ResistElements, 200h, 48h, 0, HLP_RESIST_ELEMENTS>
s_UNIT_ENCHANTMENT <offset cnst_StoneSkin, 800h, 4Ah, 0, HLP_STONE_SKIN>
s_UNIT_ENCHANTMENT <offset cnst_IronSkin, 1000h, 4Bh, 0, HLP_IRON_SKIN>
s_UNIT_ENCHANTMENT <offset cnst_Endurance, 2000h, 4Ch, 0, HLP_ENDURANCE>
s_UNIT_ENCHANTMENT <offset cnst_SpellLock, 4000h, 77h, 0, HLP_SPELL_LOCK>
s_UNIT_ENCHANTMENT <offset cnst_Invisibility, 8000h, 4Eh, 0, HLP_INVISIBILITY_UE>
s_UNIT_ENCHANTMENT <offset cnst_WindWalking, 10000h, 4Fh, 0, HLP_WIND_WALKING_UE>
s_UNIT_ENCHANTMENT <offset cnst_Flight, 20000h, 50h, 0, HLP_FLIGHT>
s_UNIT_ENCHANTMENT <offset cnst_ResistMagic, 40000h, 51h, 0, HLP_RESIST_MAGIC>
s_UNIT_ENCHANTMENT <offset cnst_MagicImmunity, 80000h, 52h, 0, HLP_MAGIC_IMMUNITY_UE>
s_UNIT_ENCHANTMENT <offset cnst_FlameBlade, 100000h, 53h, 0, HLP_FLAME_BLADE>
s_UNIT_ENCHANTMENT <offset cnst_EldritchWeapon, 200000h, 54h, 0, HLP_ELDRITCH_WEAPON>
s_UNIT_ENCHANTMENT <offset cnst_TrueSight, 400000h, 55h, 0, HLP_TRUE_SIGHT>
s_UNIT_ENCHANTMENT <offset cnst_HolyWeapon, 800000h, 56h, 0, HLP_HOLY_WEAPON>
s_UNIT_ENCHANTMENT <offset cnst_Heroism, 1000000h, 57h, 0, HLP_HEROISM>
s_UNIT_ENCHANTMENT <offset cnst_Bless, 2000000h, 58h, 0, HLP_BLESS>
s_UNIT_ENCHANTMENT <offset cnst_LionHeart, 4000000h, 59h, 0, HLP_LION_HEART>
s_UNIT_ENCHANTMENT <offset cnst_GiantStrength, 8000000h, 41h, 0, HLP_GIANT_STRENGTH>
s_UNIT_ENCHANTMENT <offset cnst_PlanarTravel, 10000000h, 5Bh, 0, HLP_PLANAR_TRAVEL_UE>
s_UNIT_ENCHANTMENT <offset cnst_HolyArmor, 20000000h, 5Ch, 0, HLP_HOLY_ARMOR>
s_UNIT_ENCHANTMENT <offset cnst_Righteousness, 40000000h, 5Dh, 0, HLP_RIGHTEOUSNESS>
s_UNIT_ENCHANTMENT <offset cnst_Invulnerability, 80000000h, 5Eh, 0, HLP_INVULNERABILITY>

{ cnst_Immolation,       0x00000001,  0x75,  HLP_IMMOLATION_UE      },
{ cnst_GuardianWind,     0x00000002,  0x76,  HLP_GUARDIAN_WIND      },
{ cnst_Berserk,          0x00000004,  0x80,  HLP_BERSERK            },
{ cnst_CloakOfFear,      0x00000008,  0x61,  HLP_CLOAK_OF_FEAR      },
{ cnst_BlackChannels,    0x00000010,  0x43,  HLP_BLACK_CHANNELS     },
{ cnst_WraithForm,       0x00000020,  0x44,  HLP_WRAITH_FORM        },
{ cnst_Regenerate,       0x00000040,  0x45,  HLP_REGENERATE         },
{ cnst_PathFinding,      0x00000080,  0x46,  HLP_PATHFINDING_UE     },
{ cnst_WaterWalking,     0x00000100,  0x47,  HLP_WATER_WALKING      },
{ cnst_ElementalArmor,   0x00000400,  0x49,  HLP_ELEMENTAL_ARMOR    },
{ cnst_ResistElements,   0x00000200,  0x48,  HLP_RESIST_ELEMENTS    },
{ cnst_StoneSkin,        0x00000800,  0x4A,  HLP_STONE_SKIN         },
{ cnst_IronSkin,         0x00001000,  0x4B,  HLP_IRON_SKIN          },
{ cnst_Endurance,        0x00002000,  0x4C,  HLP_ENDURANCE          },
{ cnst_SpellLock,        0x00004000,  0x77,  HLP_SPELL_LOCK         },
{ cnst_Invisibility,     0x00008000,  0x4E,  HLP_INVISIBILITY_UE    },
{ cnst_WindWalking,      0x00010000,  0x4F,  HLP_WIND_WALKING_UE    },
{ cnst_Flight,           0x00020000,  0x50,  HLP_FLIGHT             },
{ cnst_ResistMagic,      0x00040000,  0x51,  HLP_RESIST_MAGIC       },
{ cnst_MagicImmunity,    0x00080000,  0x52,  HLP_MAGIC_IMMUNITY_UE  },
{ cnst_FlameBlade,       0x00100000,  0x53,  HLP_FLAME_BLADE        },
{ cnst_EldritchWeapon,   0x00200000,  0x54,  HLP_ELDRITCH_WEAPON    },
{ cnst_TrueSight,        0x00400000,  0x55,  HLP_TRUE_SIGHT         },
{ cnst_HolyWeapon,       0x00800000,  0x56,  HLP_HOLY_WEAPON        },
{ cnst_Heroism,          0x01000000,  0x57,  HLP_HEROISM            },
{ cnst_Bless,            0x02000000,  0x58,  HLP_BLESS              },
{ cnst_LionHeart,        0x04000000,  0x59,  HLP_LION_HEART         },
{ cnst_GiantStrength,    0x08000000,  0x41,  HLP_GIANT_STRENGTH     },
{ cnst_PlanarTravel,     0x10000000,  0x5B,  HLP_PLANAR_TRAVEL_UE   },
{ cnst_HolyArmor,        0x20000000,  0x5C,  HLP_HOLY_ARMOR         },
{ cnst_Righteousness,    0x40000000,  0x5D,  HLP_RIGHTEOUSNESS      },
{ cnst_Invulnerability,  0x80000000,  0x5E,  HLP_INVULNERABILITY    },


*/


// WZD dseg:3DDE
// drake178: USW_Combat_FX USW_CombatEffect
// sizeof:  0Ah  10d
struct USW_CombatEffect
{
    /* 00 */ char ** name;
    /* 02 */ uint32_t CEff_Flags;
    /* 06 */ uint8_t icon_idx;
    /* 07 */ // 2-byte alignment
    /* 08 */ int16_t help_idx;  // enum HELP_TXT
    /* 0A */
};

char * battle_unit_combat_effect_names[15] =
{
    cnst_Vertigo,
    cnst_Confusion,
    cnst_Whirlwind,
    cnst_MindStorm,
    cnst_Shatter,
    cnst_Weakness,
    cnst_BlackSleep,
    cnst_WarpCreature,
    cnst_WarpCreature,
    cnst_WarpCreature,
    cnst_MindTwist,
    cnst_Haste,
    cnst_Web,
    cnst_CreatureBinding,
    cnst_Possession
};

struct USW_CombatEffect USW_Combat_FX[15] =
{
{ &battle_unit_combat_effect_names[ 0],  0x0001,  0x65,  HLP_VERTIGO            },
{ &battle_unit_combat_effect_names[ 1],  0x0002,  0x6F,  HLP_CONFUSION          },
{ &battle_unit_combat_effect_names[ 2],  0x0004,  0x70,  HLP_WHIRLWIND          },
{ &battle_unit_combat_effect_names[ 3],  0x0008,  0x71,  HLP_MIND_STORM         },
{ &battle_unit_combat_effect_names[ 4],  0x0010,  0x5F,  HLP_SHATTER            },
{ &battle_unit_combat_effect_names[ 5],  0x0020,  0x60,  HLP_WEAKNESS           },
{ &battle_unit_combat_effect_names[ 6],  0x0040,  0x72,  HLP_BLACK_SLEEP        },
{ &battle_unit_combat_effect_names[ 7],  0x0080,  0x7A,  HLP_WARP_CREATURE_Atk  },
{ &battle_unit_combat_effect_names[ 8],  0x0100,  0x7B,  HLP_WARP_CREATURE_Def  },
{ &battle_unit_combat_effect_names[ 9],  0x0200,  0x7C,  HLP_WARP_CREATURE_Res  },
{ &battle_unit_combat_effect_names[10],  0x0400,  0x79,  HLP_MANA_LEAK          },
{ &battle_unit_combat_effect_names[11],  0x0800,  0x4D,  HLP_HASTE              },
{ &battle_unit_combat_effect_names[12],  0x1000,  0x63,  HLP_WEB                },
{ &battle_unit_combat_effect_names[13],  0x2000,  0x64,  HLP_CREATURE_BINDING   },
{ &battle_unit_combat_effect_names[14],  0x4000,  0x62,  HLP_POSSESSION         },
};
/*
USW_Combat_FX

49 44 01 00 00 00 65 00 E1 01 
51 44 02 00 00 00 6F 00 E2 01 
5B 44 04 00 00 00 70 00 E3 01 
65 44 08 00 00 00 71 00 E4 01 
70 44 10 00 00 00 5F 00 E5 01 
78 44 20 00 00 00 60 00 E6 01 
81 44 40 00 00 00 72 00 E7 01 
8D 44 80 00 00 00 7A 00 E8 01 
8D 44 00 01 00 00 7B 00 E9 01 
8D 44 00 02 00 00 7C 00 EA 01 
9B 44 00 04 00 00 79 00 EB 01 
A6 44 00 08 00 00 4D 00 EC 01 
AC 44 00 10 00 00 63 00 ED 01 
B0 44 00 20 00 00 64 00 EE 01 
C1 44 00 40 00 00 62 00 EF 01                              

USW_CombatEffect <offset cnst_Vertigo, 1, 65h, 0, HLP_VERTIGO>
USW_CombatEffect <offset cnst_Confusion, 2, 6Fh, 0, HLP_CONFUSION> ; "Vertigo"
USW_CombatEffect <offset cnst_Whirlwind, 4, 70h, 0, HLP_WHIRLWIND>
USW_CombatEffect <offset cnst_MindStorm, 8, 71h, 0, HLP_MIND_STORM>
USW_CombatEffect <offset cnst_Shatter, 10h, 5Fh, 0, HLP_SHATTER>
USW_CombatEffect <offset cnst_Weakness, 20h, 60h, 0, HLP_WEAKNESS>
USW_CombatEffect <offset cnst_BlackSleep, 40h, 72h, 0, HLP_BLACK_SLEEP>
USW_CombatEffect <offset cnst_WarpCreature, 80h, 7Ah, 0, HLP_WARP_CREATURE_Atk>
USW_CombatEffect <offset cnst_WarpCreature, 100h, 7Bh, 0, HLP_WARP_CREATURE_Def>
USW_CombatEffect <offset cnst_WarpCreature, 200h, 7Ch, 0, HLP_WARP_CREATURE_Res>
USW_CombatEffect <offset cnst_MindTwist, 400h, 79h, 0, HLP_MANA_LEAK>
USW_CombatEffect <offset cnst_Haste, 800h, 4Dh, 0, HLP_HASTE>
USW_CombatEffect <offset cnst_Web, 1000h, 63h, 0, HLP_WEB>
USW_CombatEffect <offset cnst_CreatureBinding, 2000h, 64h, 0, HLP_CREATURE_BINDING>
USW_CombatEffect <offset cnst_Possession, 4000h, 62h, 0, HLP_POSSESSION>

{ cnst_Vertigo,          0x0001,  65h,  HLP_VERTIGO            },
{ cnst_Confusion,        0x0002,  6Fh,  HLP_CONFUSION          },
{ cnst_Whirlwind,        0x0004,  70h,  HLP_WHIRLWIND          },
{ cnst_MindStorm,        0x0008,  71h,  HLP_MIND_STORM         },
{ cnst_Shatter,          0x0010,  5Fh,  HLP_SHATTER            },
{ cnst_Weakness,         0x0020,  60h,  HLP_WEAKNESS           },
{ cnst_BlackSleep,       0x0040,  72h,  HLP_BLACK_SLEEP        },
{ cnst_WarpCreature,     0x0080,  7Ah,  HLP_WARP_CREATURE_Atk  },
{ cnst_WarpCreature,     0x0100,  7Bh,  HLP_WARP_CREATURE_Def  },
{ cnst_WarpCreature,     0x0200,  7Ch,  HLP_WARP_CREATURE_Res  },
{ cnst_MindTwist,        0x0400,  79h,  HLP_MANA_LEAK          },
{ cnst_Haste,            0x0800,  4Dh,  HLP_HASTE              },
{ cnst_Web,              0x1000,  63h,  HLP_WEB                },
{ cnst_CreatureBinding,  0x2000,  64h,  HLP_CREATURE_BINDING   },
{ cnst_Possession,       0x4000,  62h,  HLP_POSSESSION         },

*/

// sizeof: 0Ah  10d
struct USW_ItemPower
{
/* 00 */  char * name;
/* 02 */  uint32_t bit_idx;
/* 06 */ SAMB_INT icon_idx;  // index into specials, starting at 102
/* 08 */ int16_t help_idx;  //  enum HELP_TXT
/* 0A */
};


// WZD dseg:3E74
// USW_Item_UEs
struct USW_ItemPower USW_Item_UEs[21] =
{
{ cnst_MagicImmunity,    0x80000000,   0,  HLP_MAGIC_IMMUNITY_I },
{ cnst_Regenerate,       0x00000040,   1,  HLP_REGENERATE_I },
{ cnst_Haste,            0x00004000,   2,  HLP_HASTE_I },
{ cnst_TrueSight,        0x00400000,   3,  HLP_TRUE_SIGHT_I },
{ cnst_PathFinding,      0x00000080,   4,  HLP_PATH_FINDING },
{ cnst_WaterWalking,     0x00000100,   5,  HLP_WATER_WALKING_I },
{ cnst_Bless,            0x02000000,   6,  HLP_BLESS_I },
{ cnst_LionHeart,        0x04000000,   7,  HLP_LION_HEART_I },
{ cnst_Invisibility,     0x00008000,   8,  HLP_INVISIBILITY_I },
{ cnst_PlanarTravel,     0x10000000,   9,  HLP_PLANAR_TRAVEL_I },
{ cnst_Endurance,        0x00002000,  10,  HLP_ENDURANCE_I },
{ cnst_Righteousness,    0x40000000,  11,  HLP_RIGHTEOUSNESS_I },
{ cnst_Invulnerability,  0x80000000,  12,  HLP_INVULNERABILITY_I },
{ cnst_ResistElements,   0x00000200,  13,  HLP_RESIST_ELEMENTS_I },
{ cnst_ElementalArmor,   0x00000400,  14,  HLP_ELEMENTAL_ARMOR_I },
{ cnst_CloakOfFear,      0x00000008,  15,  HLP_CLOAK_OF_FEAR_I },
{ cnst_Flight,           0x20000000,  16,  HLP_FLIGHT_I },
{ cnst_ResistMagic,      0x40000000,  17,  HLP_RESIST_MAGIC_I },
{ cnst_GuardianWind,     0x00000002,  21,  HLP_GUARDIAN_WIND_I },
{ cnst_GiantStrength,    0x08000000,  20,  HLP_GIANT_STRENGTH_I },
{ cnst_Merging,          0x20000000,  18,  HLP_MERGING_I }
};

/*
A0 41 00 00 08 00 00 00 2C 02 33 43 40 00 00 00 01 00 2D 02 A6 44 00 40 00 00 02 00 2E 02 D3 43 00 00 40 00 03 00 2F 02 3E 43 80 00 00 00 04 00 30 02 4B 43 00 01 00 00 05 00 31 02 F2 43 00 00 00 02 06 00 32 02 F8 43 00 00 00 04 07 00 33 02 A0 40 00 80 00 00 08 00 34 02 12 44 00 00 00 10 09 00 35 02 8E 43 00 20 00 00 0A 00 36 02 2B 44 00 00 00 40 0B 00 37 02 39 44 00 00 00 80 0C 00 38 02 69 43 00 02 00 00 0D 00 39 02 59 43 00 04 00 00 0E 00 3A 02 0A 43 08 00 00 00 0F 00 3B 02 A3 43 00 00 02 00 10 00 3C 02 AA 43 00 00 04 00 11 00 3D 02 F4 42 02 00 00 00 15 00 3E 02 03 44 00 00 00 08 14 00 3F 02 CB 44 00 00 00 20 12 00 40 02                                            

USW_ItemPower <offset cnst_MagicImmunity, 0, 8, 0, HLP_MAGIC_IMMUNITY_I>
USW_ItemPower <offset cnst_Regenerate, 40h, 0, 1, HLP_REGENERATE_I>
USW_ItemPower <offset cnst_Haste, 4000h, 0, 2, HLP_HASTE_I>
USW_ItemPower <offset cnst_TrueSight, 0, 40h, 3, HLP_TRUE_SIGHT_I>
USW_ItemPower <offset cnst_PathFinding, 80h, 0, 4, HLP_PATH_FINDING>
USW_ItemPower <offset cnst_WaterWalking, 100h, 0, 5, HLP_WATER_WALKING_I>
USW_ItemPower <offset cnst_Bless, 0, 200h, 6, HLP_BLESS_I>
USW_ItemPower <offset cnst_LionHeart, 0, 400h, 7, HLP_LION_HEART_I>
USW_ItemPower <offset cnst_Invisibility, 8000h, 0, 8, HLP_INVISIBILITY_I>
USW_ItemPower <offset cnst_PlanarTravel, 0, 1000h, 9, HLP_PLANAR_TRAVEL_I>
USW_ItemPower <offset cnst_Endurance, 2000h, 0, 0Ah, HLP_ENDURANCE_I>
USW_ItemPower <offset cnst_Righteousness, 0, 4000h, 0Bh, HLP_RIGHTEOUSNESS_I>
USW_ItemPower <offset cnst_Invulnerability, 0, 8000h, 0Ch, HLP_INVULNERABILITY_I>
USW_ItemPower <offset cnst_ResistElements, 200h, 0, 0Dh, HLP_RESIST_ELEMENTS_I>
USW_ItemPower <offset cnst_ElementalArmor, 400h, 0, 0Eh, HLP_ELEMENTAL_ARMOR_I>
USW_ItemPower <offset cnst_CloakOfFear, 8, 0, 0Fh, HLP_CLOAK_OF_FEAR_I>
USW_ItemPower <offset cnst_Flight, 0, 2, 10h, HLP_FLIGHT_I>
USW_ItemPower <offset cnst_ResistMagic, 0, 4, 11h, HLP_RESIST_MAGIC_I>
USW_ItemPower <offset cnst_GuardianWind, 2, 0, 15h, HLP_GUARDIAN_WIND_I>
USW_ItemPower <offset cnst_GiantStrength, 0, 800h, 14h, HLP_GIANT_STRENGTH_I>
USW_ItemPower <offset cnst_Merging, 0, 2000h, 12h, HLP_MERGING_I>

*/



// WZD dseg:3F46
char * USW_UnitLevels[6] = 
{
    cnst_UnitLevel_1,
    cnst_UnitLevel_2,
    cnst_UnitLevel_3,
    cnst_UnitLevel_4,
    cnst_UnitLevel_5,
    cnst_UnitLevel_6
};

// WZD dseg:3F52
char * USW_HeroLevels[9] =
{
    cnst_HeroLevel_1,
    cnst_HeroLevel_2,
    cnst_HeroLevel_3,
    cnst_HeroLevel_4,
    cnst_UnitLevel_6,
    cnst_HeroLevel_6,
    cnst_HeroLevel_7,
    cnst_HeroLevel_8,
    cnst_HeroLevel_9
};



// WZD dseg:3F64
// ; DATA XREF: ITEM_GetPowerNames+4E4r ...
// ; BUG: there is no swiftness spell in the game
// ITEM_ViewTexts
// IVIEW_TEXT <offset cnst_Vampiric, offset cnst_VampiricText, <IP_Vampiric, 0>>
// IVIEW_TEXT <offset cnst_Lightning, offset cnst_LightningText, <IP_Lightning, 0>>
// IVIEW_TEXT <offset cnst_CloakofFear, offset cnst_CoF_Text, <IP_Cloak_Of_Fear, 0>>
// IVIEW_TEXT <offset cnst_Destruction, offset cnst_DestructionText, <IP_Destruction, 0>>
// IVIEW_TEXT <offset cnst_Wraithform, offset cnst_WraithformText, <IP_Wraith_Form, 0>>
// IVIEW_TEXT <offset cnst_Regeneration, offset cnst_RegenerateText, <IP_Regeneration, 0>>
// IVIEW_TEXT <offset cnst_Pathfinding, offset cnst_PF_Text, <IP_Pathfinding, 0>>
// IVIEW_TEXT <offset cnst_WaterWalking, offset cnst_WaterWalkText, <IP_Water_Walking, 0>>
// IVIEW_TEXT <offset cnst_ResistElements, offset cnst_ResistElemText, <IP_Resist_Elements, 0>>
// IVIEW_TEXT <offset cnst_ElementalArmor, offset cnst_ElemArmorText, <IP_Elemental_Armour, 0>>
// IVIEW_TEXT <offset cnst_Doom, offset cnst_DoomText, <IP_Chaos, 0>>
// IVIEW_TEXT <offset cnst_Endurance, offset cnst_EnduranceText, <IP_Endurance, 0>>
// IVIEW_TEXT <offset cnst_Haste, offset cnst_HasteText, <IP_Haste, 0>>
// IVIEW_TEXT <offset cnst_Invisibility, offset cnst_InvisText, <IP_Invisibility, 0>>
// IVIEW_TEXT <offset cnst_Death, offset cnst_DeathText, <0, IP_Death>>
// IVIEW_TEXT <offset cnst_Flight, offset cnst_FlightText, <0, IP_Flight>>
// IVIEW_TEXT <offset cnst_ResistMagic, offset cnst_ResMagicText, <0, IP_Resist_Magic>>
// IVIEW_TEXT <offset cnst_MagicImmunity, offset cnst_MagicImmText, <0, IP_Magic_Immunity>>
// IVIEW_TEXT <offset cnst_PowerDrain, offset cnst_PDrainText, <0, IP_Power_Drain>>
// IVIEW_TEXT <offset cnst_HolyAvenger, offset cnst_HolyAvengerText, <0, IP_Holy_Avenger>>
// IVIEW_TEXT <offset cnst_TrueSight, offset cnst_TrueSightText, <0, IP_True_Sight>>
// IVIEW_TEXT <offset cnst_Phantasmal, offset cnst_PhantasmalText, <0, IP_Phantasmal>>
// IVIEW_TEXT <offset cnst_Flaming, offset cnst_FlamingText, <0, IP_Flaming>>
// IVIEW_TEXT <offset cnst_Bless, offset cnst_BlessText, <0, IP_Bless>>
// IVIEW_TEXT <offset cnst_LionHeart, offset cnst_LionheartText, <0, IP_Lion_Heart>>
// IVIEW_TEXT <offset cnst_Stoning, offset cnst_StoningText, <IP_Stoning, 0>>
// IVIEW_TEXT <offset cnst_PlanarTravel, offset cnst_PlanarTrvlText, <0, IP_Planar_Travel>>
// IVIEW_TEXT <offset cnst_Merging, offset cnst_MergingText, <0, IP_Merging>>
// IVIEW_TEXT <offset cnst_Righteousness, offset cnst_RightsnssText, <0, IP_Righteousness>>
// IVIEW_TEXT <offset cnst_Invulnerability, offset cnst_InvulnText, <0, IP_Invulnerability>>
// IVIEW_TEXT <offset cnst_GuardianWind, offset cnst_GWindText, <IP_Guardian_Wind, 0>>
// IVIEW_TEXT <offset cnst_GiantStrength, offset cnst_GStrText, <0, IP_Giant_Strength>>

struct s_ITEM_VIEW_TEXT ITEM_ViewTexts[] =
{
    {cnst_Vampiric, cnst_VampiricText, IP_VAMPIRIC},
    {cnst_Lightning, cnst_LightningText, IP_LIGHTNING},
    {cnst_CloakofFear, cnst_CoF_Text, IP_CLOAK_OF_FEAR},
    {cnst_Destruction, cnst_DestructionText, IP_DESTRUCTION},
    {cnst_Wraithform, cnst_WraithformText, IP_WRAITH_FORM},
    {cnst_Regeneration, cnst_RegenerateText, IP_REGENERATION},
    {cnst_Pathfinding, cnst_PF_Text, IP_PATHFINDING},
    {cnst_WaterWalking, cnst_WaterWalkText, IP_WATER_WALKING},
    {cnst_ResistElements, cnst_ResistElemText, IP_RESIST_ELEMENTS},
    {cnst_ElementalArmor, cnst_ElemArmorText, IP_ELEMENTAL_ARMOUR},
    {cnst_Doom, cnst_DoomText, IP_CHAOS},
    {cnst_Endurance, cnst_EnduranceText, IP_ENDURANCE},
    {cnst_Haste, cnst_HasteText, IP_HASTE},
    {cnst_Invisibility, cnst_InvisText, IP_INVISIBILITY},
    {cnst_Death, cnst_DeathText, IP_DEATH},
    {cnst_Flight, cnst_FlightText, IP_FLIGHT},
    {cnst_ResistMagic, cnst_ResMagicText, IP_RESIST_MAGIC},
    {cnst_MagicImmunity, cnst_MagicImmText, IP_MAGIC_IMMUNITY},
    {cnst_PowerDrain, cnst_PDrainText, IP_POWER_DRAIN},
    {cnst_HolyAvenger, cnst_HolyAvengerText, IP_HOLY_AVENGER},
    {cnst_TrueSight, cnst_TrueSightText, IP_TRUE_SIGHT},
    {cnst_Phantasmal, cnst_PhantasmalText, IP_PHANTASMAL},
    {cnst_Flaming, cnst_FlamingText, IP_FLAMING},
    {cnst_Bless, cnst_BlessText, IP_BLESS},
    {cnst_LionHeart, cnst_LionheartText, IP_LION_HEART},
    {cnst_Stoning, cnst_StoningText, IP_STONING},
    {cnst_PlanarTravel, cnst_PlanarTrvlText, IP_PLANAR_TRAVEL},
    {cnst_Merging, cnst_MergingText, IP_MERGING},
    {cnst_Righteousness, cnst_RightsnssText, IP_RIGHTEOUSNESS},
    {cnst_Invulnerability, cnst_InvulnText, IP_INVULNERABILITY},
    {cnst_GuardianWind, cnst_GWindText, IP_GUARDIAN_WIND},
    {cnst_GiantStrength, cnst_GStrText, IP_GIANT_STRENGTH}
};





// WZD dseg:500A                                                 BEGIN:  ovr089 - Strings

// WZD dseg:500A 46 49 47 55 52 45 00                            cnst_FIGFile_Base db 'FIGURE',0
// WZD dseg:5011 53 00                                           cnst_FIGFile_S db 'S',0
// WZD dseg:5013 2E 4C 42 58 00                                  cnst_LBX_Ext db '.LBX',0

// WZD dseg:5018
// 44 6F 20 79 6F 75 20 77 69 73 68 20 74 6F 20 74 75 72 6E 20 6F 66 66 20 74 68 65 20 02 00
char cnst_SpellCncl_Msg_4a[] = "Do you wish to turn off the \x02";

// WZD dseg:5036
// 01 20 73 70 65 6C 6C 3F 00
char cnst_SpellCnclMsg_4b[] = "\x01 spell?";

// WZD dseg:503F
char cnst_CnclDrown_Msg1[] = "  This unit will die if the spell is removed!";

// WZD dseg:506D
char cnst_CnclDrown_Msg2[] = "  This unit and other units will die if the spell is removed!";

// WZD dseg:50AB 00                                              align 2

// WZD dseg:50AB                                                 END:  ovr089 - Strings












/*
    WIZARDS.EXE  ovr072
*/



/*
    WIZARDS.EXE  ovr078
*/

// WZD o78p01
/*
    assumes global_battle_unit
    
Page 77 (PDF Page 82)
Special Abilities and Enchantments
Any special abilities and unit enchantments appear as icons in the bottom half of the unit statistics window.

*/
void USW_Build_Effect_List(int16_t unit_idx, struct s_UV_List * specials_list, int16_t * specials_count)
{
    char scout_rank[5][5];
    char temp_string[LEN_TEMP_STRING];
    uint32_t hero_ability_flag;
    int16_t ranged_attack_group;
    int16_t Unit_Mutations;
    int16_t unit_type;
    uint32_t hero_abilities_flags;
    uint32_t unit_enchantments;
    int16_t item_idx;
    int16_t hero_slot_idx;
    int16_t hero_owner_idx;
    int16_t itr;
    int16_t ability_value;
    int16_t specials_index;  // _DI_
    int16_t item_slot_type;  // DNE in Dasm

    strcpy(scout_rank[0], empty_string_ovr078);  // ""
    strcpy(scout_rank[1], cnst_Rank_I);          // "I"
    strcpy(scout_rank[2], cnst_Rank_II);         // "II"
    strcpy(scout_rank[3], cnst_Rank_III);        // "III"
    strcpy(scout_rank[4], cnst_Rank_IV);         // "IV"

    unit_type = _UNITS[unit_idx].type;

    *specials_count = 0;

    specials_index = 0;


    /*
        BEGIN:  
    */
    if(
        (_UNITS[unit_idx].Level >= 0)  /* TODO:  Can/If/When is level negative? What does it mean? */
        &&
        (unit_type < ut_Magic_Spirit)  /* Non-Fantastic ~ Normal/Recruit */
        &&
        ((_UNITS[unit_idx].mutations & UM_UNDEAD) == 0)  // ¿ ; BUG: this should either not be a condition here, or should be the same for Chaos Channeled units ?
    )
    {
        if(_UNITS[unit_idx].Hero_Slot == -1)
        {
            strcpy(specials_list->names[specials_index], USW_UnitLevels[_UNITS[unit_idx].Level]);
        }
        else
        {
            strcpy(specials_list->names[specials_index], USW_HeroLevels[_UNITS[unit_idx].Level]);
        }
        strcat(specials_list->names[specials_index], cnst_SpaceOpenBrace2);
        itoa(_UNITS[unit_idx].XP, temp_string, 10);
        strcat(specials_list->names[specials_index], temp_string);
        strcat(specials_list->names[specials_index], cnst_ExperiencePts);
        specials_list->flags[specials_index] = -1;
        specials_list->picts[specials_index] = (SAMB_INT)special_seg[(SPECIAL_ICON_LEVEL_MEDALS + _UNITS[unit_idx].Level)];
        if(_UNITS[unit_idx].Hero_Slot == -1)
        {
            specials_list->helps[specials_index] = (HLP_UNIT_LEVEL_0 + _UNITS[unit_idx].Level);
        }
        else
        {
            specials_list->helps[specials_index] = (HLP_HERO_LEVEL_0 + _UNITS[unit_idx].Level);
        }
        specials_index++;
    }


    /*
        BEGIN:  Hero Unit Specific
    */
    if(_UNITS[unit_idx].Hero_Slot != ST_UNDEFINED)
    {
        hero_owner_idx = _UNITS[unit_idx].owner_idx;
        hero_slot_idx = _UNITS[unit_idx].Hero_Slot;

        for(itr = 0; itr < NUM_HERO_ITEMS; itr++)
        {
            item_idx = _players[hero_owner_idx].Heroes[hero_slot_idx].Items[itr];

            if(item_idx > -1)
            {
                // String_Copy_Far(specials_list->names[specials_index], 0, _ITEMS[item_idx].name);
                strcpy(specials_list->names[specials_index], _ITEMS[item_idx].name);
                specials_list->flags[specials_index] = -99;
                specials_list->picts[specials_index] = (SAMB_INT)item_icons_seg[_ITEMS[item_idx].icon_idx];
                specials_list->helps[specials_index] = item_idx;
            }
            else
            {
                strcpy(specials_list->names[specials_index], empty_string_ovr078);
                specials_list->flags[specials_index] = -99;
                // specials_list->picts[specials_index] = (SAMB_INT)itemtile_icons_seg[_players[hero_owner_idx].Heroes[hero_slot_idx].Item_Slots[itr]];
                item_slot_type = _players[hero_owner_idx].Heroes[hero_slot_idx].Item_Slots[itr];
                specials_list->picts[specials_index] = (SAMB_INT)itemtile_icons_seg[(item_slot_type - 1)];
                specials_list->helps[specials_index] = -(HLP_MERGING_I + _players[hero_owner_idx].Heroes[hero_slot_idx].Item_Slots[itr]);
            }

            specials_index++;
        }

        /*
            BEGIN:  Item Powers
        */
        for(itr = 0; itr < 21; itr++)
        {
            if((global_battle_unit->item_enchantments & USW_Item_UEs[itr].bit_idx) != 0)
            {
                strcpy(specials_list->names[specials_index], USW_Item_UEs[itr].name);
                specials_list->flags[specials_index] = -1;
                specials_list->picts[specials_index] = (SAMB_INT)item_power_icons_seg[USW_Item_UEs[itr].icon_idx];
                specials_list->helps[specials_index] = USW_Item_UEs[itr].help_idx;

                specials_index++;
            }
        }
        /*
            END:  Item Powers
        */


        /*
            BEGIN:  Hero Special Abilities
        */
        for(itr = 0; itr < 23; itr++)
        {
            hero_abilities_flags = (_HEROES2[hero_owner_idx]->heroes[unit_type].abilities & USW_HeroAbilities[itr].flags);

            if(hero_abilities_flags != 0)
            {
                hero_ability_flag = hero_abilities_flags;
                switch(hero_ability_flag)
                {
                    case HSA_AGILITY:
                    {
                        strcpy(specials_list->names[specials_index], cnst_Agility);
                        ability_value = (_UNITS[unit_idx].Level + 1);
                    } break;
                    case HSA_LEADERSHIP:
                    {
                        strcpy(specials_list->names[specials_index], cnst_Leadership);
                        ability_value = ((_UNITS[unit_idx].Level + 1) / 3);
                    } break;
                    case HSA_LEADERSHIP2:
                    {
                        strcpy(specials_list->names[specials_index], cnst_Leadership);
                        strcat(specials_list->names[specials_index], cnst_SuperAbStar);
                        ability_value = ((_UNITS[unit_idx].Level + 1) / 2);
                    } break;
                    case HSA_LEGENDARY:
                    {
                        strcpy(specials_list->names[specials_index], cnst_Legendary);
                        ability_value = ((_UNITS[unit_idx].Level + 1) / 3);
                    } break;
                    case HSA_LEGENDARY2:
                    {
                        strcpy(specials_list->names[specials_index], cnst_Legendary);
                        strcat(specials_list->names[specials_index], cnst_SuperAbStar);
                        ability_value = (((_UNITS[unit_idx].Level + 1) * 9) / 2);
                    } break;
                    case HSA_BLADEMASTER:
                    {
                        strcpy(specials_list->names[specials_index], cnst_Blademaster);
                        ability_value = ((_UNITS[unit_idx].Level + 1) / 2);
                    } break;
                    case HSA_BLADEMASTER2:
                    {
                        strcpy(specials_list->names[specials_index], cnst_Blademaster);
                        strcat(specials_list->names[specials_index], cnst_SuperAbStar);
                        ability_value = (((_UNITS[unit_idx].Level + 1) * 3) / 4);
                    } break;
                    case HSA_ARMSMASTER:
                    {
                        strcpy(specials_list->names[specials_index], cnst_Armsmaster);
                        ability_value = ((_UNITS[unit_idx].Level + 1) * 2);
                    } break;
                    case HSA_ARMSMASTER2:
                    {
                        strcpy(specials_list->names[specials_index], cnst_Armsmaster);
                        strcat(specials_list->names[specials_index], cnst_SuperAbStar);
                        ability_value = ((_UNITS[unit_idx].Level + 1) * 3);
                    } break;
                    case HSA_CONSTITUTION:
                    {
                        strcpy(specials_list->names[specials_index], cnst_Constitution);
                        ability_value = (_UNITS[unit_idx].Level + 1);
                    } break;
                    case HSA_CONSTITUTION2:
                    {
                        strcpy(specials_list->names[specials_index], cnst_Constitution);
                        strcat(specials_list->names[specials_index], cnst_SuperAbStar);
                        ability_value = ((_UNITS[unit_idx].Level + 1) * 3);
                    } break;
                    case HSA_MIGHT:
                    {
                        strcpy(specials_list->names[specials_index], cnst_Might);
                        ability_value = (_UNITS[unit_idx].Level + 1);
                    } break;
                    case HSA_MIGHT2:
                    {
                        strcpy(specials_list->names[specials_index], cnst_Might);
                        strcat(specials_list->names[specials_index], cnst_SuperAbStar);
                        ability_value = ((_UNITS[unit_idx].Level + 1) * 3);
                    } break;
                    case HSA_ARCANE_POWER:
                    {
                        strcpy(specials_list->names[specials_index], cnst_ArcanePower);
                        ability_value = (_UNITS[unit_idx].Level + 1);
                    } break;
                    case HSA_ARCANE_POWER2:
                    {
                        strcpy(specials_list->names[specials_index], cnst_ArcanePower);
                        strcat(specials_list->names[specials_index], cnst_SuperAbStar);
                        ability_value = ((_UNITS[unit_idx].Level + 1) * 3);
                    } break;
                    case HSA_SAGE:
                    {
                        strcpy(specials_list->names[specials_index], cnst_Sage_2);
                        strcat(specials_list->names[specials_index], cnst_Space_3);
                        ability_value = ((_UNITS[unit_idx].Level + 1) * 3);
                        itoa(ability_value, temp_string, 10);
                        strcat(specials_list->names[specials_index], temp_string);
                        strcat(specials_list->names[specials_index], cnst_ResearchPoints);
                    } break;
                    case HSA_SAGE2:
                    {
                        strcpy(specials_list->names[specials_index], cnst_Sage_2);
                        strcat(specials_list->names[specials_index], cnst_SuperAbStar);
                        strcat(specials_list->names[specials_index], cnst_Space_3);
                        ability_value = (((_UNITS[unit_idx].Level + 1) * 9) / 2);
                        itoa(ability_value, temp_string, 10);
                        strcat(specials_list->names[specials_index], temp_string);
                        strcat(specials_list->names[specials_index], cnst_ResearchPoints);
                    } break;
                    case HSA_PRAYERMASTER:
                    {
                        strcpy(specials_list->names[specials_index], cnst_Prayermaster);
                        ability_value = (_UNITS[unit_idx].Level + 1);
                    } break;
                    case HSA_PRAYERMASTER2:
                    {
                        strcpy(specials_list->names[specials_index], cnst_Prayermaster);
                        strcat(specials_list->names[specials_index], cnst_SuperAbStar);
                        ability_value = ((_UNITS[unit_idx].Level + 1) * 3);
                    } break;
                    case HSA_AGILITY2:
                    {
                        strcpy(specials_list->names[specials_index], cnst_Agility);
                        strcat(specials_list->names[specials_index], cnst_SuperAbStar);
                        ability_value = ((_UNITS[unit_idx].Level + 1) * 3);
                    } break;
                    case HSA_LUCKY:
                    {
                        strcpy(specials_list->names[specials_index], cnst_Lucky);
                        ability_value = -1;
                    } break;
                    case HSA_CHARMED:
                    {
                        strcpy(specials_list->names[specials_index], cnst_Charmed);
                        ability_value = -1;
                    } break;
                    case HSA_NOBLE:
                    {
                        strcpy(specials_list->names[specials_index], cnst_Noble);
                        ability_value = -1;
                    } break;
                }

                if(
                    (_UNITS[unit_idx].Level >= 0)
                    &&
                    (ability_value > 0)
                )
                {
                    itoa(ability_value, temp_string, 10);
                    strcat(specials_list->names[specials_index], cnst_SpacePlus);
                    strcat(specials_list->names[specials_index], temp_string);
                }

                specials_list->flags[specials_index] = ST_UNDEFINED;

                specials_list->picts[specials_index] = (SAMB_INT)special_seg[USW_HeroAbilities[itr].icon_idx];

                specials_list->helps[specials_index] = USW_HeroAbilities[itr].help_idx;

                specials_index++;
            }
        }
        /*
            END:  Hero Special Abilities
        */

        /*
            BEGIN:  Spell Caster
        */
        {
            if(_HEROES2[hero_owner_idx]->heroes[unit_type].Casting_Skill > 0)
            {
                strcpy(specials_list->names[specials_index], cnst_Caster);
                if(_HEROES2[hero_owner_idx]->heroes[unit_type].Level >= 0)
                {
                    strcat(specials_list->names[specials_index], cnst_Space_3);
                    if(global_battle_unit->mana_max > global_battle_unit->mana)
                    {
                        ability_value = global_battle_unit->mana;
                        itoa(ability_value, temp_string, 10);
                        strcat(specials_list->names[specials_index], temp_string);
                        strcat(specials_list->names[specials_index], cnst_SpaceOpenBrace2);
                        ability_value = global_battle_unit->mana_max;
                        itoa(ability_value, temp_string, 10);
                        strcat(specials_list->names[specials_index], temp_string);
                        strcat(specials_list->names[specials_index], cnst_ClosingBrace_3);
                    }
                    else
                    {
                        ability_value = global_battle_unit->mana_max;
                        itoa(ability_value, temp_string, 10);
                        strcat(specials_list->names[specials_index], temp_string);
                    }
                    strcat(specials_list->names[specials_index], cnst_Space_mp);
                }
                specials_list->flags[specials_index] = -1;
                specials_list->picts[specials_index] = (SAMB_INT)special_seg[SPECIAL_ICON_CASTER];
                specials_list->helps[specials_index] = HLP_CASTER;
                specials_index++;
            }
        }
        /*
            END:  Spell Caster
        */

    }
    /*
        END:  Hero Unit Specific
    */


    /*
        BEGIN:  Quiver
    */
    {
        ranged_attack_group = (global_battle_unit->ranged_type / 10);
        if(
            (ranged_attack_group == rag_Boulder)
            ||
            (ranged_attack_group == rag_Missile)
            ||
            (ranged_attack_group == rag_Magic)
        )
        {
            if(
                (ranged_attack_group != rag_Magic)
                ||
                /* ((battle_units[unit_type].Attribs_1 & (USA_CASTER_20 || USA_CASTER_40)) == 0) * / /* ; BUG: this is not a battle unit index */ /* ¿ typo: ~ combat_data vs. global_combat_data ? */
                ((global_battle_unit->Attribs_1 & (USA_CASTER_20 || USA_CASTER_40)) == 0)
            )
            {
                if(
                    (_UNITS[unit_idx].Hero_Slot == -1)
                    ||
                    (_HEROES2[_UNITS[unit_idx].owner_idx]->heroes[unit_type].Casting_Skill <= 0)
                )
                {
                    if(ranged_attack_group == rag_Boulder)
                    {
                        strcpy(specials_list->names[specials_index], cnst_Rock);
                    }
                    if(ranged_attack_group == rag_Missile)
                    {
                        if(global_battle_unit->ranged_type != rat_Bow)
                        {
                            strcpy(specials_list->names[specials_index], cnst_Bullet);
                        }
                        else
                        {
                            strcpy(specials_list->names[specials_index], cnst_Arrow);
                        }
                    }
                    if(ranged_attack_group == rag_Magic)
                    {
                        strcpy(specials_list->names[specials_index], cnst_Spell_2);
                    }
                    ability_value = global_battle_unit->ammo;
                    itoa(ability_value, temp_string, 10);
                    if(ability_value > 1)
                    {
                        strcat(specials_list->names[specials_index], cnst_Ammo_Plural);
                    }
                    strcat(specials_list->names[specials_index], cnst_TimesSign);
                    strcat(specials_list->names[specials_index], temp_string);
                    specials_list->flags[specials_index] = -1;
                    specials_list->picts[specials_index] = (SAMB_INT)special_seg[SPECIAL_ICON_QUIVER];
                    specials_list->helps[specials_index] = HLP_QUIVER;
                    specials_index++;
                }
            }
        }
    }
    /*
        END:  Quiver
    */


    /*
        BEGIN:  To-Hit
    */
    {
        if(global_battle_unit->tohit != 0)
        {
            ability_value = global_battle_unit->tohit;
            itoa(ability_value, temp_string, 10);
            if(ability_value > 0)
            {
                strcpy(specials_list->names[specials_index], cnst_Plus_1);
                specials_list->picts[specials_index] = (SAMB_INT)special_seg[125];
                specials_list->helps[specials_index] = HLP_TOHIT_PLUS;
            }
            else
            {
                strcpy(specials_list->names[specials_index], empty_string_ovr078);
                specials_list->picts[specials_index] = (SAMB_INT)special_seg[126];
                specials_list->helps[specials_index] = HLP_TOHIT_MINUS;
            }
            strcat(specials_list->names[specials_index], temp_string);
            strcat(specials_list->names[specials_index], cnst_ToHit);
            specials_list->flags[specials_index] = -1;
            specials_index++;
        }
    }
    /*
        END:  To-Hit
    */


    /*
        BEGIN:  Construction
    */
    {
        // ¿ BUG: ignores the ability value ?
        if(global_battle_unit->Construction > 0)
        {
            strcpy(specials_list->names[specials_index], cnst_Construction);
            specials_list->flags[specials_index] = -1;
            specials_list->picts[specials_index] = (SAMB_INT)special_seg[SPECIAL_ICON_CONSTRUCTION];
            specials_list->helps[specials_index] = HLP_CONSTRUCTION;
            specials_index++;
        }
    }
    /*
        END:  Construction
    */


    /*
        BEGIN:  Scouting
    */
    {
        // ¿ ; BUG: this should be checked in the unit type to avoid double display for the item power ?
        // ¿ BUG: misrepresents the souting range by ranking it based on the value rather than the actual increase ?
        if(global_battle_unit->scout_range > 1)
        {
            strcpy(specials_list->names[specials_index], cnst_Scouting);
            ability_value = global_battle_unit->scout_range;
            strcpy(temp_string, scout_rank[ability_value]);
            strcat(specials_list->names[specials_index], cnst_Space_3);
            specials_list->flags[specials_index] = -1;
            specials_list->picts[specials_index] = (SAMB_INT)special_seg[SPECIAL_ICON_SCOUTING];
            specials_list->helps[specials_index] = HLP_SCOUTING;
            specials_index++;
        }
    }
    /*
        END:  Scouting
    */


    /*
        BEGIN:  Merging
    */
    {
        // ; BUG: this should be checked in the unit type to avoid double display for the item power
        if((global_battle_unit->Move_Flags & MV_MERGING) != 0)
        {
            strcpy(specials_list->names[specials_index], cnst_Merging);
            specials_list->flags[specials_index] = -1;
            specials_list->picts[specials_index] = (SAMB_INT)item_power_icons_seg[18];
            specials_list->helps[specials_index] = HLP_MERGING;
            specials_index++;
        }
    }
    /*
        END:  Merging
    */


    /*
        BEGIN:  Teleporting
    */
    {
        // ; BUG: this should be checked in the unit type to avoid double display for the item power
        if((global_battle_unit->Move_Flags & MV_TELEPORT) != 0)
        {
            strcpy(specials_list->names[specials_index], cnst_Teleporting);
            specials_list->flags[specials_index] = -1;
            specials_list->picts[specials_index] = (SAMB_INT)special_seg[SPECIAL_ICON_TELEPORTING];
            specials_list->helps[specials_index] = HLP_TELEPORTING;
            specials_index++;
        }
    }
    /*
        END:  Teleporting
    */


    /*
        BEGIN:  Pathfinding, Mountaineer, Forester
    */
    {
        if(
            ((global_battle_unit->Move_Flags & MV_FORESTER) != 0)
            &&
            ((global_battle_unit->Move_Flags & MV_MOUNTAINEER) != 0)
        )
        {
            strcpy(specials_list->names[specials_index], cnst_Pathfinding);
            specials_list->flags[specials_index] = -1;
            specials_list->picts[specials_index] = (SAMB_INT)special_seg[SPECIAL_ICON_PATHFINDING];
            specials_list->helps[specials_index] = HLP_PATHFINDING_Ab;
            specials_index++;
        }
        else if((global_battle_unit->Move_Flags & MV_FORESTER) != 0)
        {
            strcpy(specials_list->names[specials_index], cnst_Mountaineer);
            specials_list->flags[specials_index] = -1;
            specials_list->picts[specials_index] = (SAMB_INT)special_seg[SPECIAL_ICON_MOUNTAINEER];
            specials_list->helps[specials_index] = HLP_MOUNTAINEER_Ab;
            specials_index++;
        }
        else if((global_battle_unit->Move_Flags & MV_MOUNTAINEER) != 0)
        {
            strcpy(specials_list->names[specials_index], cnst_Forester);
            specials_list->flags[specials_index] = -1;
            specials_list->picts[specials_index] = (SAMB_INT)special_seg[SPECIAL_ICON_FORESTER];
            specials_list->helps[specials_index] = HLP_FORESTER_Ab;
            specials_index++;
        }
    }
    /*
        END:  Pathfinding, Mountaineer, Forester
    */


    /*
        BEGIN:  Unit Special Abilities - Unit Type
    */
    {
        // ¿ s_BATTLE_UNIT.Abilities is just copied from s_UNIT_TYPE.Abilities ?
        // LargeShield, PlaneShift, WallCrusher, Healer, CreateOutpost, Invisibility, CreateUndead, LongRange, LandCorruption, MeldWithNode, NonCorporeal, WindWalking, Regeneration, Purify, Negate1stStrike
        // ¿ BUG: these should be taken from the unit type to avoid double display of abilities granted by spells ?
        for(itr = 0; ((itr < 15) && (specials_index < 39)); itr++)
        {
            if((USW_Abilities[itr].bit_idx & global_battle_unit->Abilities) != 0)
            {
                strcpy(specials_list->names[specials_index], *USW_Abilities[itr].name);
                specials_list->flags[specials_index] = -1;
                specials_list->picts[specials_index] = (SAMB_INT)special_seg[USW_Abilities[itr].icon_idx];
                specials_list->helps[specials_index] = USW_Abilities[itr].help_idx;
                specials_index++;
            }
        }
    }
    /*
        END:  Unit Special Abilities - Unit Type
    */


    /*
        BEGIN:  Summon Demons
    */
    {
        // ¿ BUG: checks the unit type data rather than the battle unit, and will thus always show the maximum value instead of the current ?
        // ¿ BUG? does not have the record limit check from above ?
        // "Creature Summoning"
        if((_unit_type_table[unit_type].Attribs_1 & (USA_SUMMON_DEMON_1 || USA_SUMMON_DEMON_2)) != 0)
        {
            strcpy(specials_list->names[specials_index], cnst_SummonDemons);
            ability_value = 0;
            if((_unit_type_table[unit_type].Attribs_1 & USA_SUMMON_DEMON_2) != 0)
            {
                ability_value = 2;
            }
            if((_unit_type_table[unit_type].Attribs_1 & USA_SUMMON_DEMON_1) != 0)
            {
                ability_value++;
            }
            itoa(ability_value, temp_string, 10);
            strcat(specials_list->names[specials_index], cnst_Space_3);
            strcat(specials_list->names[specials_index], temp_string);
            specials_list->flags[specials_index] = -1;
            specials_list->picts[specials_index] = (SAMB_INT)special_seg[SPECIAL_ICON_SUMMON_DEMONS];
            specials_list->helps[specials_index] = HLP_SUMMON_DEMONS;
            specials_index++;
        }
    }
    /*
        BEGIN:  Summon Demons
    */


    /*
        BEGIN:  Spell Caster
    */
    {
        // ¿ BUG? does not have the record limit check from above ?
        if((_unit_type_table[unit_type].Attribs_1 & (USA_CASTER_20 || USA_CASTER_40)) != 0)  /* (Sum_Demon_1 || Sum_Demon_2) */
        {
            strcpy(specials_list->names[specials_index], cnst_Caster);
            strcat(specials_list->names[specials_index], cnst_Space_3);

            if(global_battle_unit->mana_max > global_battle_unit->mana)
            {
                ability_value = global_battle_unit->mana;
                itoa(ability_value, temp_string, 10);
                strcat(specials_list->names[specials_index], temp_string);
                strcat(specials_list->names[specials_index], cnst_SpaceOpenBrace2);
                ability_value = global_battle_unit->mana_max;
                itoa(ability_value, temp_string, 10);
                strcat(specials_list->names[specials_index], temp_string);
                strcat(specials_list->names[specials_index], cnst_ClosingBrace_3);
            }
            else
            {
                ability_value = global_battle_unit->mana_max;
                itoa(ability_value, temp_string, 10);
                strcat(specials_list->names[specials_index], temp_string);
            }
            strcat(specials_list->names[specials_index], cnst_Space_mp);

            specials_list->flags[specials_index] = -1;
            specials_list->picts[specials_index] = (SAMB_INT)special_seg[SPECIAL_ICON_CASTER];
            specials_list->helps[specials_index] = HLP_CASTER;
            specials_index++;
        }
    }
    /*
        END:  Spell Caster
    */


    /*
        BEGIN:  Unit Type Attributes 1
    */
    {
        // Lucky, PoisonImmunity, FireImmunity, StoningImmunity, WeaponImmunity, MissileImmunity, IllusionsImm, ColdImmunity, MagicImmunity, DeathImmunity
        for(itr = 0; ((itr < 10) && (specials_index < 39)); itr++)
        {
            if((USW_Attributes_1[itr].bit_idx & _unit_type_table[unit_type].Attribs_1) != 0)
            {
                strcpy(specials_list->names[specials_index], *USW_Attributes_1[itr].name);
                specials_list->flags[specials_index] = -1;
                specials_list->picts[specials_index] = (SAMB_INT)special_seg[USW_Attributes_1[itr].icon_idx];
                specials_list->helps[specials_index] = USW_Attributes_1[itr].help_idx;
                specials_index++;
            }
        }
    }
    /*
        END:  Unit Type Attributes 1
    */


    /*
        BEGIN:  Unit Type Attributes 2
    */
    {
        // HealingSpell, FireBallSpell, DoomboltSpell, Immolation, CauseFearSpell, WebSpell, ResistanceToAll, HolyBonus
        for(itr = 0; ((itr < 8) && (specials_index < 39)); itr++)
        {
            if((USW_Attributes_2[itr].bit_idx & _unit_type_table[unit_type].Attribs_2) != 0)
            {
                strcpy(specials_list->names[specials_index], *USW_Attributes_2[itr].name);
                specials_list->flags[specials_index] = -1;
                specials_list->picts[specials_index] = (SAMB_INT)special_seg[USW_Attributes_2[itr].icon_idx];
                specials_list->helps[specials_index] = USW_Attributes_2[itr].help_idx;
                specials_index++;
            }
        }
    }
    /*
        END:  Unit Type Attributes 1
    */


    /*
        BEGIN:  Attack Flags - Battle Unit
    */
    {
        // ¿ BUG: Life Steal does not show its -Save modifier (neither do Destruction and Death Touch if fixed) ?
        // ¿ BUG: only considers universal attack flags - those that apply only to melee or ranged are ignored ?
        // ArmorPiercing, Poison, LifeSteal, AutomaticDmg, Destruction, Illusion, StoningTouch, DeathTouch, PowerDrain, DispelEvil
        for(itr = 0; ((itr < 11) && (specials_index < 39)); itr++)
        {
            if((USW_ATK_Flags[itr].bit_idx & global_battle_unit->Attack_Flags) != 0)
            {
				const char* pcszName = *USW_ATK_Flags[itr].name;
				if(pcszName)
	                strcpy(specials_list->names[specials_index], pcszName);
				else
					specials_list->names[specials_index][0] = '\0';
                if(USW_ATK_Flags[itr].bit_idx == Att_Poison)
                {
                    ability_value = global_battle_unit->Poison_Strength;
                    itoa(ability_value, temp_string, 10);
                    strcat(specials_list->names[specials_index], cnst_Space_3);
                    strcat(specials_list->names[specials_index], temp_string);
                }
                if(USW_ATK_Flags[itr].bit_idx == Att_StnTouch)
                {
                    ability_value = global_battle_unit->Spec_Att_Attrib;
                    itoa(ability_value, temp_string, 10);
                    strcat(specials_list->names[specials_index], cnst_Space_3);
                    strcat(specials_list->names[specials_index], temp_string);
                }
                specials_list->flags[specials_index] = -1;
                specials_list->picts[specials_index] = (SAMB_INT)special_seg[USW_ATK_Flags[itr].icon_idx];
                specials_list->helps[specials_index] = USW_ATK_Flags[itr].help_idx;
                specials_index++;
            }
        }
    }
    /*
        END:  Attack Flags - Battle Unit
    */


    /*
        BEGIN:  Special Ranged Attacks
    */
    {
        // ¿ BUG: these should be taken from the unit type to avoid double display of chaos channels fire breath ?
        // ¿ BUG: Multi-Gaze shows the inverse modifier for its special damage gaze attacks ?
        // Thrown, FireBreath, Lightning, StoningGaze, DoomGaze, DeathGaze
        for(itr = 0; ((itr < 6) && (specials_index < 39)); itr++)
        {
            if(global_battle_unit->ranged_type == USW_SR_Attacks[itr].Ranged_Type)
            {
                strcpy(specials_list->names[specials_index], *USW_SR_Attacks[itr].name);
                ability_value = global_battle_unit->ranged;
                if(
                    (USW_SR_Attacks[itr].Ranged_Type == srat_StoneGaze)
                    &&
                    (USW_SR_Attacks[itr].Ranged_Type == srat_DeathGaze)
                )
                {
                    ability_value = global_battle_unit->Spec_Att_Attrib;
                }
                itoa(ability_value, temp_string, 10);
                strcat(specials_list->names[specials_index], cnst_Space_3);
                strcat(specials_list->names[specials_index], temp_string);
                specials_list->flags[specials_index] = -1;
                specials_list->picts[specials_index] = (SAMB_INT)special_seg[USW_SR_Attacks[itr].icon_idx];
                specials_list->helps[specials_index] = USW_ATK_Flags[itr].help_idx;
                specials_index++;

                // just added srat_MultiGaze (104), now add both srat_StoneGaze (103) and srat_DeathGaze (105)
                if(USW_SR_Attacks[itr].Ranged_Type == srat_MultiGaze)
                {
                    strcpy(specials_list->names[specials_index], *USW_SR_Attacks[(itr + 1)].name);
                    ability_value = global_battle_unit->Spec_Att_Attrib;
                    itoa(ability_value, temp_string, 10);
                    strcat(specials_list->names[specials_index], cnst_Space_3);
                    strcat(specials_list->names[specials_index], temp_string);
                    specials_list->flags[specials_index] = -1;
                    specials_list->picts[specials_index] = (SAMB_INT)special_seg[USW_SR_Attacks[(itr + 1)].icon_idx];
                    specials_list->helps[specials_index] = USW_ATK_Flags[(itr + 1)].help_idx;
                    specials_index++;

                    strcpy(specials_list->names[specials_index], *USW_SR_Attacks[(itr - 1)].name);
                    ability_value = global_battle_unit->Spec_Att_Attrib;
                    itoa(ability_value, temp_string, 10);
                    strcat(specials_list->names[specials_index], cnst_Space_3);
                    strcat(specials_list->names[specials_index], temp_string);
                    specials_list->flags[specials_index] = -1;
                    specials_list->picts[specials_index] = (SAMB_INT)special_seg[USW_SR_Attacks[(itr - 1)].icon_idx];
                    specials_list->helps[specials_index] = USW_ATK_Flags[(itr - 1)].help_idx;
                    specials_index++;
                }
            }
        }
    }
    /*
        END:  Special Ranged Attacks
    */


    /*
        BEGIN:  Battle Unit Combat Effects
    */
    {
        for(itr = 0; ((itr < 15) && (specials_index < 39)); itr++)
        {
            if((global_battle_unit->Combat_Effects & USW_Combat_FX[itr].CEff_Flags) != 0)
            {
                strcpy(specials_list->names[specials_index], *USW_Combat_FX[itr].name);
                specials_list->flags[specials_index] = -1;
                specials_list->picts[specials_index] = (SAMB_INT)special_seg[USW_Combat_FX[itr].icon_idx];
                specials_list->helps[specials_index] = USW_Combat_FX[itr].help_idx;
                specials_index++;
            }
        }
    }
    /*
        END:  Battle Unit Combat Effects
    */


    /*
        BEGIN:  Unit Mutations
    */
    {
        Unit_Mutations = _UNITS[unit_idx].mutations;
        for(itr = 0; ((itr < 3) && (specials_index < 39)); itr++)
        {
            if((_unit_mutation_data[itr].flag & Unit_Mutations) != 0)
            {
                strcpy(specials_list->names[specials_index], *_unit_mutation_data[itr].name);
                specials_list->flags[specials_index] = -1;
                specials_list->picts[specials_index] = (SAMB_INT)special_seg[_unit_mutation_data[itr].icon_idx];
                specials_list->helps[specials_index] = _unit_mutation_data[itr].help_idx;
                specials_index++;
            }
        }
    }
    /*
        END:  Unit Mutations
    */

    /*
        BEGIN:  Undead
    */
    {
        Unit_Mutations = _UNITS[unit_idx].mutations;
        if((Unit_Mutations & UM_UNDEAD) != 0)
        {
            strcpy(specials_list->names[specials_index], cnst_Undead);
            specials_list->flags[specials_index] = -1;
            specials_list->picts[specials_index] = (SAMB_INT)special_seg[SPECIAL_ICON_UNDEAD];
            specials_list->helps[specials_index] = HLP_UNDEAD;
            specials_index++;
        }
    }
    /*
        END:  Undead
    */


    /*
        BEGIN:  Stasis
    */
    {
        Unit_Mutations = _UNITS[unit_idx].mutations;
        if(
            ((Unit_Mutations & C_STASISINIT) != 0)
            ||
            ((Unit_Mutations & C_STASISLINGER) != 0)
        )
        {
            strcpy(specials_list->names[specials_index], cnst_Stasis);
            specials_list->flags[specials_index] = -1;
            specials_list->picts[specials_index] = (SAMB_INT)special_seg[SPECIAL_ICON_STASIS];
            specials_list->helps[specials_index] = HLP_STASIS;
            specials_index++;
        }
    }
    /*
        END:  Stasis
    */


    /*
        BEGIN:  Unit Enchantments
    */
    {
        unit_enchantments = _UNITS[unit_idx].enchantments;

        for(itr = 0; ((itr < 32) && (specials_index < 39)); itr++)
        {
            if((unit_enchantments & _unit_enchantment_data[itr].flag) != 0)
            {
                strcpy(specials_list->names[specials_index], *_unit_enchantment_data[itr].name);
                specials_list->flags[specials_index] = _unit_enchantment_data[itr].flag;
                specials_list->picts[specials_index] = (SAMB_INT)special_seg[_unit_enchantment_data[itr].icon_idx];
                specials_list->helps[specials_index] = _unit_enchantment_data[itr].help_idx;
                specials_index++;
            }
        }
    }
    /*
        END:  Unit Enchantments
    */

    *specials_count = specials_index;

}

// WZD o78p02
/*
    only called from 'Production Screen'
    therefore, no Unit Enchantments, no Hero Items
        and, only Norm specials, no Fant specials, no Hero specials
    Construction
    Scouting
    Teleporting, Forester, Mountaineer
    USW_Abilities
    USW_Attributes_1
    USW_Attributes_2
    USW_ATK_Flags
    USW_SR_Attacks

    ¿ ~== Hero_Build_Specials_List() ?

*/
void Prod_Build_Specials_List(int16_t unit_type, struct s_UV_List * specials_list, int16_t * specials_count)
{
    char scout_rank[5][5];
    int16_t value;
    char temp_buffer[10];
    int16_t itr;
    int16_t specials_index;  // SI

    strcpy(scout_rank[0], "");   // if > 1, so never used
    strcpy(scout_rank[1], "I");  // if > 1, so never used
    strcpy(scout_rank[2], "II");
    strcpy(scout_rank[3], "III");
    strcpy(scout_rank[4], "IV");

    specials_index = 0;

    if(_unit_type_table[unit_type].Construction > 0)
    {
        strcpy(specials_list->names[specials_index], cnst_Construction);
        specials_list->flags[specials_index] = -1;
        specials_list->picts[specials_index] = (SAMB_INT)special_seg[SPECIAL_ICON_CONSTRUCTION];
        specials_list->helps[specials_index] = HLP_CONSTRUCTION;
        specials_index++;
    }

    if(_unit_type_table[unit_type].Sight > 1)
    {
        strcpy(specials_list->names[specials_index], cnst_Scouting);
        value = _unit_type_table[unit_type].Sight;
        strcpy(temp_buffer, scout_rank[value]);  // BUGBUG: should use index, not value  Sight == 1 is not 'Special', so 2 should be "I"
        strcat(specials_list->names[specials_index], cnst_Space_3);
        strcat(specials_list->names[specials_index], temp_buffer);
        specials_list->flags[specials_index] = -1;
        specials_list->picts[specials_index] = (SAMB_INT)special_seg[SPECIAL_ICON_SCOUTING];
        specials_list->helps[specials_index] = HLP_SCOUTING;
        specials_index++;
    }

    if((_unit_type_table[unit_type].Move_Flags & TELEPORTING) != 0)
    {
        strcpy(specials_list->names[specials_index], cnst_Teleporting);
        specials_list->flags[specials_index] = -1;
        specials_list->picts[specials_index] = (SAMB_INT)special_seg[SPECIAL_ICON_TELEPORTING];
        specials_list->helps[specials_index] = HLP_TELEPORTING;
        specials_index++;
    }

    if((_unit_type_table[unit_type].Move_Flags & FORESTER) != 0)
    {
        strcpy(specials_list->names[specials_index], cnst_Forester);
        specials_list->flags[specials_index] = -1;
        specials_list->picts[specials_index] = (SAMB_INT)special_seg[SPECIAL_ICON_FORESTER];
        specials_list->helps[specials_index] = HLP_FORESTER_Ab;
        specials_index++;
    }

    if((_unit_type_table[unit_type].Move_Flags & MOUNTAINEER) != 0)
    {
        strcpy(specials_list->names[specials_index], cnst_Mountaineer);
        specials_list->flags[specials_index] = -1;
        specials_list->picts[specials_index] = (SAMB_INT)special_seg[SPECIAL_ICON_MOUNTAINEER];
        specials_list->helps[specials_index] = HLP_MOUNTAINEER_Ab;
        specials_index++;
    }

    for(itr = 0; itr < NUM_UNIT_ATTRIBUTES; itr++)
    {
        if((_unit_type_table[unit_type].Abilities & USW_Abilities[itr].bit_idx) != 0)
        {
            strcpy(specials_list->names[specials_index], *USW_Abilities[itr].name);
            specials_list->flags[specials_index] = -1;
            specials_list->picts[specials_index] = (SAMB_INT)special_seg[USW_Abilities[itr].icon_idx];
            specials_list->helps[specials_index] = USW_Abilities[itr].help_idx;
            specials_index++;
        }
    }

    for(itr = 0; itr < 10; itr++)
    {
        if((_unit_type_table[unit_type].Attribs_1 & USW_Attributes_1[itr].bit_idx) != 0)
        {
            strcpy(specials_list->names[specials_index], *USW_Attributes_1[itr].name);
            specials_list->flags[specials_index] = -1;
            specials_list->picts[specials_index] = (SAMB_INT)special_seg[USW_Attributes_1[itr].icon_idx];
            specials_list->helps[specials_index] = USW_Attributes_1[itr].help_idx;
            specials_index++;
        }
    }

    for(itr = 0; itr < 8; itr++)
    {
        if((_unit_type_table[unit_type].Attribs_2 & USW_Attributes_2[itr].bit_idx) != 0)
        {
            strcpy(specials_list->names[specials_index], *USW_Attributes_2[itr].name);
            specials_list->flags[specials_index] = -1;
            specials_list->picts[specials_index] = (SAMB_INT)special_seg[USW_Attributes_2[itr].icon_idx];
            specials_list->helps[specials_index] = USW_Attributes_2[itr].help_idx;
            specials_index++;
        }
    }

    for(itr = 0; itr < 11; itr++)
    {
        if((_unit_type_table[unit_type].Attack_Flags & USW_ATK_Flags[itr].bit_idx) != 0)
        {
            strcpy(specials_list->names[specials_index], *USW_ATK_Flags[itr].name);

            if((USW_ATK_Flags[itr].bit_idx & Att_Poison) != 0)  /* Att_Poison */
            {
                value = global_battle_unit->Poison_Strength;
                itoa(value, temp_buffer, 10);
                strcat(specials_list->names[specials_index], " ");
                strcat(specials_list->names[specials_index], temp_buffer);
            }

            if((USW_ATK_Flags[itr].bit_idx & Att_StnTouch) != 0)  /* Att_StnTouch */
            {
                value = global_battle_unit->Spec_Att_Attrib;
                itoa(value, temp_buffer, 10);
                strcat(specials_list->names[specials_index], " ");
                strcat(specials_list->names[specials_index], temp_buffer);
            }

            specials_list->flags[specials_index] = -1;
            specials_list->picts[specials_index] = (SAMB_INT)special_seg[USW_ATK_Flags[itr].icon_idx];
            specials_list->helps[specials_index] = USW_ATK_Flags[itr].help_idx;
            specials_index++;
        }
    }

    for(itr = 0; ((itr < 6) && (specials_index < 39)); itr++)
    {
        if(global_battle_unit->ranged_type == USW_SR_Attacks[itr].Ranged_Type)
        {
            strcpy(specials_list->names[specials_index], *USW_SR_Attacks[itr].name);

            value = global_battle_unit->ranged;

            if(
                (USW_SR_Attacks[itr].Ranged_Type == srat_StoneGaze)
                ||
                (USW_SR_Attacks[itr].Ranged_Type == srat_DeathGaze)
            )
            {
                value = global_battle_unit->Spec_Att_Attrib;
            }

            itoa(value, temp_buffer, 10);
            strcat(specials_list->names[specials_index], " ");
            strcat(specials_list->names[specials_index], temp_buffer);

            specials_list->flags[specials_index] = -1;
            specials_list->picts[specials_index] = (SAMB_INT)special_seg[USW_SR_Attacks[itr].icon_idx];
            specials_list->helps[specials_index] = USW_SR_Attacks[itr].help_idx;
            specials_index++;

            if(USW_SR_Attacks[itr].Ranged_Type == srat_MultiGaze)
            {
                strcpy(specials_list->names[specials_index], *USW_SR_Attacks[(itr + 1)].name);
                value = global_battle_unit->Spec_Att_Attrib;
                itoa(value, temp_buffer, 10);
                strcat(specials_list->names[specials_index], " ");
                strcat(specials_list->names[specials_index], temp_buffer);
                specials_list->flags[specials_index] = ST_UNDEFINED;
                specials_list->picts[specials_index] = (SAMB_INT)special_seg[USW_SR_Attacks[(itr + 1)].icon_idx];
                specials_list->helps[specials_index] = USW_SR_Attacks[(itr + 1)].help_idx;
                specials_index++;

                strcpy(specials_list->names[specials_index], *USW_SR_Attacks[(itr - 1)].name);
                value = global_battle_unit->Spec_Att_Attrib;
                itoa(value, temp_buffer, 10);
                strcat(specials_list->names[specials_index], " ");
                strcat(specials_list->names[specials_index], temp_buffer);
                specials_list->flags[specials_index] = ST_UNDEFINED;
                specials_list->picts[specials_index] = (SAMB_INT)special_seg[USW_SR_Attacks[(itr - 1)].icon_idx];
                specials_list->helps[specials_index] = USW_SR_Attacks[(itr - 1)].help_idx;
                specials_index++;
            }
        }
    }

    *specials_count = specials_index;

}


// WZD o78p03
// drake178: ITEM_GetPowerNames()
/*
    like UV/USW effect_list

*/
void ITEM_GetPowerNames(int16_t item_idx, char power_names[5][20], char power_descriptions[5][40], int16_t * power_count)
{
    char Temp_String[30];
    int16_t itr;
    int16_t display_count;  // _SI_

    *power_count = 0;
    display_count = 0;

    for(itr = 0; itr < 5; itr++)
    {
        strcpy(power_descriptions[itr], "");
    }

    if(_ITEMS[item_idx].attack > 0)
    {
        strcpy(power_names[display_count], cnst_Plus_1);
        itoa(_ITEMS[item_idx].attack, Temp_String, 10);
        strcat(power_names[display_count], Temp_String);
        strcat(power_names[display_count], cnst_Space_ATK);
        display_count++;
    }
    if((_ITEMS[item_idx].tohit > 0) && (display_count < 5))
    {
        strcpy(power_names[display_count], cnst_Plus_1);
        itoa(_ITEMS[item_idx].tohit, Temp_String, 10);
        strcat(power_names[display_count], Temp_String);
        strcat(power_names[display_count], cnst_ToHit);
        display_count++;
    }
    if((_ITEMS[item_idx].defense > 0) && (display_count < 5))
    {
        strcpy(power_names[display_count], cnst_Plus_1);
        itoa(_ITEMS[item_idx].defense, Temp_String, 10);
        strcat(power_names[display_count], Temp_String);
        strcat(power_names[display_count], cnst_Space_DEF);
        display_count++;
    }
    if((_ITEMS[item_idx].moves2 > 0) && (display_count < 5))
    {
        strcpy(power_names[display_count], cnst_Plus_1);
        itoa(_ITEMS[item_idx].moves2, Temp_String, 10);
        strcat(power_names[display_count], Temp_String);
        strcat(power_names[display_count], cnst_Space_Moves);
        display_count++;
    }
    if((_ITEMS[item_idx].resistance > 0) && (display_count < 5))
    {
        strcpy(power_names[display_count], cnst_Plus_1);
        itoa(_ITEMS[item_idx].resistance, Temp_String, 10);
        strcat(power_names[display_count], Temp_String);
        strcat(power_names[display_count], cnst_Space_RES);
        display_count++;
    }
    if((_ITEMS[item_idx].spell_skill > 0) && (display_count < 5))
    {
        strcpy(power_names[display_count], cnst_Plus_1);
        itoa(_ITEMS[item_idx].spell_skill, Temp_String, 10);
        strcat(power_names[display_count], Temp_String);
        strcat(power_names[display_count], cnst_Space_Skill);
        display_count++;
    }
    if((_ITEMS[item_idx].spell_save > 0) && (display_count < 5))
    {
        strcpy(power_names[display_count], cnst_MinusSign);
        itoa(_ITEMS[item_idx].spell_save, Temp_String, 10);
        strcat(power_names[display_count], Temp_String);
        strcat(power_names[display_count], cnst_Space_SAVE);
        display_count++;
    }

    if((_ITEMS[item_idx].embed_spell_idx > 0) && (display_count < 5))
    {
        // TODO  String_Copy_Far(Temp_String, 0, spell_data_table[_ITEMS[item_idx].embed_spell_idx].name)
        strcpy(Temp_String, spell_data_table[_ITEMS[item_idx].embed_spell_idx].name);
        strcpy(power_names[display_count], Temp_String);
        strcat(power_names[display_count], cnst_TimesSign);
        itoa(_ITEMS[item_idx].embed_spell_cnt, Temp_String, 10);
        strcat(power_names[display_count], cnst_Space_3);
        strcat(power_names[display_count], Temp_String);
        display_count++;
    }

    for(itr = 0; ((itr < 32) && (display_count < 5)); itr++)
    {
        if((_ITEMS[item_idx].Powers & ITEM_ViewTexts[itr].Flags) != 0)
        {
            strcpy(power_names[display_count], ITEM_ViewTexts[itr].Name);
            strcpy(power_descriptions[display_count], ITEM_ViewTexts[itr].Text);
            display_count++;
        }
    }

    SETMAX(display_count, 5);

    *power_count = display_count;
}



/*
    WIZARDS.EXE  ovr089
*/



/*
    WIZARDS.EXE  ovr089
*/

// WZD o89p01
// ¿ MoO2  Load_Display_Combat_Ship_() ?
void Prod_Load_Battle_Unit(int16_t unit_type, struct s_BATTLE_UNIT * battle_unit)
{
    //MoO2  Module: struct  Copy_Structure_()
    memcpy(battle_unit, &_unit_type_table[unit_type].Melee, sizeof(struct s_UNIT_TYPE));

    battle_unit->TopFig_Dmg = 0;

    battle_unit->Weapon_Plus1 = 0;

    battle_unit->Gold_Melee = 0;
    battle_unit->Gold_Ranged = 0;
    battle_unit->Gold_Defense = 0;
    battle_unit->Gold_Resist = 0;
    battle_unit->Gold_Hits = 0;
    battle_unit->Grey_Melee = 0;
    battle_unit->Grey_Ranged = 0;
    battle_unit->Grey_Defense = 0;
    battle_unit->Grey_Resist = 0;

    // in Load_Battle_Unit(), tests s_BATTLE_UNIT.Attack_Flags & Att_Poison
    battle_unit->Poison_Strength = _unit_type_table[unit_type].Spec_Att_Attr;

}


// WZD o89p02
/*
; resets the near allocation buffer, creates a 100
; byte GUI_String_1@ in it followed by an 1520 byte
; uv_specials_list_array@, and finishes with a 480 byte
; LBX_Alloc_Next into the sandbox for an item icon
; work area
*/
void Unit_View_Allocate(void)
{

    GUI_String_1 = (char *)Near_Allocate_First(100);  // 100 bytes
    uv_specials_list_array = (struct s_UV_List *)Near_Allocate_Next(1520);  // 1,520 bytes  40 * sizeof(s_UV_List)
    UV_item_icon_pict_seg = Allocate_Next_Block(_screen_seg, 30);  // 30 paragraphs, 480 bytes  19 x 19 pict

}

// WZD o89p03
/*
    loads a unit figure picture

*/
// void USW_LoadFigureImage(int16_t unit_idx, int16_t TypePass)
/*
    Loads a 'Unit Figure' entry into the screen_seg
    takes unit_idx or unit_type
    type_flag:  0 == unit_idx  1 == unit_type

USW_LoadAndShow() calls this with the 'Type Flag' unset
USW_LoadHireScreen calls this with the 'Type Flag' set
City_Screen_Allocate_First_Block() calls this with the 'Type Flag' set

    // AKA Unit_Figure_Load()
*/
void Load_Unit_Figure(int16_t type_or_unit, int16_t use_type)
{
    char file_name[LEN_FILE_NAME];
    char buffer[6];
    int16_t entry_num;
    int16_t figure_num;  // _SI_

    if(use_type != ST_TRUE)
    {
        figure_num = _UNITS[type_or_unit].type;  // got passed unit_idx
    }
    else
    {
        figure_num = type_or_unit;  // got passed unit_type
    }

    itoa(((figure_num / 15) + 1), buffer, 10);
    strcpy(file_name, "FIGURE");
    if(((figure_num / 15) + 1) < 10)
    {
        strcat(file_name, "S");
    }
    strcat(file_name, buffer);
    strcat(file_name, ".LBX");

    entry_num = (((figure_num % 15) * 8) + 2);  // TODO  document this algo to the unit figure picture  (knows how many figures are in each figure lbx)

    unit_figure_seg = LBX_Reload_Next(file_name, entry_num, _screen_seg);

}

// WZD o89p04
/*
OON XREF:
    ArmyList_Screen_Draw() |-> j_Get_Unit_Enchantment_Names() |-> Get_Unit_Enchantment_Names()

*/
void Get_Unit_Enchantment_Names(int16_t unit_idx, char ** unit_enchantment_names, int16_t * unit_enchantment_count)
{
    uint8_t unit_mutations;
    uint32_t unit_enchantments;
    int16_t itr;  // _SI_
    struct s_UNIT youknit;

    unit_enchantments = _UNITS[unit_idx].enchantments;

    *unit_enchantment_count = 0;

    for(itr = 0; itr < NUM_UNIT_ENCHANTMENTS; itr++)
    {
        if((unit_enchantments & _unit_enchantment_data[itr].flag) != 0)
        {
            strcpy(unit_enchantment_names[*unit_enchantment_count], *_unit_enchantment_data[itr].name);
            *unit_enchantment_count += 1;
        }
    }

    unit_mutations = _UNITS[unit_idx].mutations;

    for(itr = 0; itr < NUM_UNIT_MUTATIONS; itr++)
    {
        if((unit_mutations & _unit_mutation_data[itr].flag) != 0)
        {
            strcpy(unit_enchantment_names[*unit_enchantment_count], *_unit_mutation_data[itr].name);
            *unit_enchantment_count += 1;
        }
    }

}


// WZD o89p05
/*
    CalledFromFlag is passed from Thing_View_Draw__WIP()
    if CalledFromFlag == 0, assumes _human_player_idx
    if CalledFromFlag != 0, gets figure count from *combat data*, instead of _unit_type_table

    // AKA Unit_Figure_Draw
*/
void Draw_Unit_Figure(int16_t x_start, int16_t y_start, int16_t unit_or_type, int16_t CalledFromFlag)
{
    int16_t fig_x;
    int16_t fig_y;
    int16_t unit_owner_banner_idx;
    int16_t unit_owner_idx;
    int16_t unit_type;
    int16_t UU_Highest_Fig_Index;
    int16_t unit_figure_count;
    int16_t itr_figure_count;
    int16_t itr_banner_colors;

    if(CalledFromFlag != uvt_Prod)
    {
        unit_type = _UNITS[unit_or_type].type;
        unit_figure_count = global_battle_unit->Cur_Figures;
        unit_owner_idx = _UNITS[unit_or_type].owner_idx;
        unit_owner_banner_idx = _players[unit_owner_idx].banner_id;
        // 'Hydra' manual override of figure count, because 'Cur_Figures' is actually 'Heads'
        if(spell_data_table[SPL_HYDRA].Param0 == unit_type)
        {
            unit_figure_count = 1;
        }
    }
    else
    {
        unit_type = unit_or_type;
        unit_figure_count = _unit_type_table[unit_type].Figures;
        unit_owner_banner_idx = _players[_human_player_idx].banner_id;
    }

    UU_Highest_Fig_Index = (unit_figure_count - 1);

    for(itr_figure_count = 0; itr_figure_count < unit_figure_count; itr_figure_count++)
    {
        FLIC_Set_CurrentFrame(unit_figure_seg, 2);
        Draw_Picture_To_Bitmap(unit_figure_seg, UnitDraw_WorkArea);

        for(itr_banner_colors = 0; itr_banner_colors < 5; itr_banner_colors++)
        {
            // Draw_Map_Towers()
            // Replace_Color(Map_Square_WorkArea, 224 + itr_color_remap, *(COL_Banners + (_players[tower_owner_idx].banner_id * 5)));
            Replace_Color(UnitDraw_WorkArea, (214 + itr_banner_colors), COL_Banners[((unit_owner_banner_idx * 5) + itr_banner_colors)]);
        }

        if(CalledFromFlag == uvt_Prod)
        {
            if( (_unit_type_table[unit_type].Abilities & 0x40 /* Ab_Invisibility */) != 0)
            {
                Replace_Color_All(UnitDraw_WorkArea, 233);
            }
        }
        else
        {
            if(Unit_Has_Invisibility(unit_or_type) == ST_TRUE)
            {
                Replace_Color_All(UnitDraw_WorkArea, 233);
            }
            Draw_Unit_Enchantment_Outline(unit_or_type);
        }

        FLIC_Set_LoopFrame_1(UnitDraw_WorkArea);
        Unit_Figure_Position(unit_figure_count, itr_figure_count, &fig_y, &fig_x);
        Draw_Picture((x_start + fig_y), (y_start + fig_x), UnitDraw_WorkArea);

    }

    Cycle_Unit_Enchantment_Animation();

}

// WZD o89p06
void Unit_Figure_Position(int16_t figure_count, int16_t current_figure, int16_t * fig_x, int16_t * fig_y)
{

    assert(figure_count   >  0);
    assert(figure_count   <= 8);
    assert(current_figure >= 0);
    assert(current_figure <  8);

    switch(figure_count - 1)
    {
        case 0:
        {
            *fig_x = 1;
            *fig_y = 8;
        } break;
        case 1:
        {
            switch(current_figure)
            {
                case 0: { *fig_x = -8; *fig_y =  9; } break;
                case 1: { *fig_x =  8; *fig_y =  9; } break;
            }
        } break;
        case 2:
        {
            switch(current_figure)
            {
                case 0: { *fig_x =  0; *fig_y =  4; } break;
                case 1: { *fig_x = -7; *fig_y = 10; } break;
                case 2: { *fig_x =  8; *fig_y = 10; } break;
            }
        } break;
        case 3:
        {
            switch(current_figure)
            {
                case 0: { *fig_x =  1; *fig_y =  3; } break;
                case 1: { *fig_x = -9; *fig_y =  8; } break;
                case 2: { *fig_x = 11; *fig_y =  8; } break;
                case 3: { *fig_x =  1; *fig_y = 13; } break;
            }
        } break;
        case 4:
        {
            switch(current_figure)
            {
                case 0: { *fig_x =  1; *fig_y =  3; } break;
                case 1: { *fig_x = -9; *fig_y =  8; } break;
                case 2: { *fig_x =  1; *fig_y =  8; } break;
                case 3: { *fig_x =  9; *fig_y =  8; } break;
                case 4: { *fig_x =  1; *fig_y = 14; } break;
            }
        } break;
        case 5:
        {
            switch(current_figure)
            {
                case 0: { *fig_x =   1; *fig_y =  3; } break;
                case 1: { *fig_x =   4; *fig_y =  7; } break;
                case 2: { *fig_x = -10; *fig_y =  8; } break;
                case 3: { *fig_x =  10; *fig_y = 10; } break;
                case 4: { *fig_x =  -3; *fig_y = 11; } break;
                case 5: { *fig_x =   1; *fig_y = 15; } break;
            }
        } break;
        case 6:
        {
            switch(current_figure)
            {
                case 0: { *fig_x =  1; *fig_y =  3; } break;
                case 1: { *fig_x =  6; *fig_y =  6; } break;
                case 2: { *fig_x = -9; *fig_y =  8; } break;
                case 3: { *fig_x =  1; *fig_y =  8; } break;
                case 4: { *fig_x = 10; *fig_y =  8; } break;
                case 5: { *fig_x = -3; *fig_y = 11; } break;
                case 6: { *fig_x =  1; *fig_y = 14; } break;
            }
        } break;
        case 7:
        {
            switch(current_figure)
            {
                case 0: { *fig_x =   1; *fig_y =  3; } break;
                case 1: { *fig_x =   6; *fig_y =  6; } break;
                case 2: { *fig_x =  -2; *fig_y =  7; } break;
                case 3: { *fig_x = -10; *fig_y =  8; } break;
                case 4: { *fig_x =  10; *fig_y =  8; } break;
                case 5: { *fig_x =   3; *fig_y =  9; } break;
                case 6: { *fig_x =  -5; *fig_y = 11; } break;
                case 7: { *fig_x =   1; *fig_y = 14; } break;
            }
        } break;
    }

}


// WZD o89p07
/*
    returns ST_TRUE if unit was dismissed as a consequence of the enchantment removal

'Unit Statistics Window'
Page 77  (PDF Page 82)
"Clicking on the enchantments removes them (if they were cast by your wizard)."
*/
int16_t UV_Remove_Unit_Enchantment(int16_t unit_idx, uint32_t enchantment, char * message_buffer)
{
    int16_t troops[MAX_STACK];
    int16_t multiple_deaths;
    int16_t unit_has_windwalking;
    int16_t stack_has_windwalking;
    int16_t water_hazard;
    int16_t troop_count;
    int16_t unit_will_die;
    int16_t enchantment_idx;
    /* ¿ bogus ? */ // int16_t unit_idx;  // _DI_
    int16_t itr;  // _SI_

    enchantment_idx = ST_UNDEFINED;

    for(itr = 0; itr < 32; itr++)
    {
        if(_unit_enchantment_data[itr].flag == enchantment)
        {
            enchantment_idx = itr;
        }
    }

    if(enchantment_idx == ST_UNDEFINED)
    {
        return ST_FALSE;
    }

    strcpy(message_buffer, cnst_SpellCncl_Msg_4a);  // "Do you wish to turn off the \x02";
    strcat(message_buffer, *_unit_enchantment_data[enchantment_idx].name);
    strcat(message_buffer, cnst_SpellCnclMsg_4b);

    unit_will_die = 0;
    multiple_deaths = 0;

    if(
        ((enchantment & UE_FLIGHT) != 0)
        ||
        ((enchantment & UE_WATERWALKING) != 0)
        ||
        ((enchantment & UE_WINDWALKING) != 0)
    )
    {

        water_hazard = Square_Is_Sailable(UNITX(), UNITY(), UNITP());

        // ¿ BUG: does not check transport capacity ?
        if(_unit_type_table[_UNITS[unit_idx].type].Transport > 0)
        {
            water_hazard = ST_FALSE;
        }

        if(water_hazard == ST_TRUE)
        {
            Player_Army_At_Square(UNITX(), UNITY(), UNITP(), UNITOWNER(), &troop_count, &troops[0]);

            for(itr = 0; itr < troop_count; itr++)
            {
                if(troops[itr] != unit_idx)
                {
                    // ¿ BUG: does not check transport capacity ?
                    if(_unit_type_table[_UNITS[troops[itr]].type].Transport > 0)
                    {
                        water_hazard = ST_FALSE;
                    }
                }
            }
        }

        if(water_hazard == ST_TRUE)
        {
            unit_has_windwalking = ST_FALSE;
            // DONT  if((Unit_Has_WindWalking(unit_idx) == ST_TRUE) || (j_o71p08_Empty_pFxn(unit_idx) == ST_TRUE))
            if(Unit_Has_WindWalking(unit_idx) == ST_TRUE)
            {
                unit_has_windwalking = ST_TRUE;
            }

            stack_has_windwalking = ST_FALSE;

            for(itr = 0; itr < troop_count; itr++)
            {
                if(troops[itr] != unit_idx)
                {
                    // DONT  if((Unit_Has_WindWalking(troops[itr]) == ST_TRUE) || (j_o71p08_Empty_pFxn(troops[itr]) == ST_TRUE))
                    if(Unit_Has_WindWalking(troops[itr]) == ST_TRUE)
                    {
                        stack_has_windwalking = ST_TRUE;
                    }
                }
            }

            if(stack_has_windwalking == ST_FALSE)
            {
                unit_will_die = ST_TRUE;

                if(
                    ((enchantment & UE_FLIGHT) != 0)
                    &&
                    (
                        (Unit_Has_WaterTravel(unit_idx) == ST_TRUE)
                        ||
                        (Unit_Has_WaterTravel_Item(unit_idx) == ST_TRUE)
                        ||
                        (unit_has_windwalking = ST_TRUE)
                    )
                )
                {
                    unit_will_die = ST_FALSE;
                }

                if(
                    ((enchantment & UE_WATERWALKING) != 0)
                    &&
                    (
                        (Unit_Has_AirTravel(unit_idx) == ST_TRUE)
                        ||
                        (Unit_Has_AirTravel_Item(unit_idx) == ST_TRUE)
                        ||
                        (unit_has_windwalking = ST_TRUE)
                    )
                )
                {
                    unit_will_die = ST_FALSE;
                }

                if((enchantment & UE_WATERWALKING) != 0)
                {
                    multiple_deaths = ST_TRUE;
                }

            }

        }

    }

    // ¿ ; BUG: these conditions don't depend on each other (eg. removing Wind Walking from an otherwise flying unit) ?
    if(unit_will_die == ST_TRUE)
    {
        if(multiple_deaths != ST_FALSE)
        {
            strcat(message_buffer, cnst_CnclDrown_Msg2);  // "  This unit and other units will die if the spell is removed!"
        }
        else
        {
            strcat(message_buffer, cnst_CnclDrown_Msg1);  // "  This unit will die if the spell is removed!"
        }
    }

    if(Confirmation_Box(message_buffer) == ST_TRUE)
    {
        Unit_Remove_Enchantment(unit_idx, enchantment);

        if(unit_will_die == ST_TRUE)
        {
            if(multiple_deaths != ST_FALSE)
            {
                // ¿ BUG: fails to consider natural wind walking as well as non-corporeals of any kind, and will dismiss them too, although the former should prevent entering this branch, so shouldn't cause problems in practice ?
                for(itr = 0; itr < troop_count; itr++)
                {
                    if(
                        (Unit_Has_AirTravel(troops[itr]) != ST_TRUE)
                        &&
                        (Unit_Has_AirTravel_Item(troops[itr]) != ST_TRUE)
                        &&
                        (Unit_Has_WaterTravel(troops[itr]) != ST_TRUE)
                        &&
                        (Unit_Has_WaterTravel_Item(troops[itr]) != ST_TRUE)
                    )
                    {
                        Dismiss_Unit(troops[itr]);
                    }
                }
            }
            else
            {
                Dismiss_Unit(unit_idx);
            }
        }

    }

    if(_UNITS[unit_idx].owner_idx == -1)
    {
        return ST_TRUE;
    }
    else
    {
        return ST_FALSE;
    }

}

// WZD o89p08
void Unit_Remove_Enchantment(int16_t unit_idx, uint32_t enchantment)
{
    _UNITS[unit_idx].enchantments ^= enchantment;
}


// WZD o89p09
// drake178: sub_759DE()
// IDK_Drown_Units()


// WZD o89p10
/*
    this function looks terribly broken in the Dasm
    ¿ l_troop_count & l_troops should be troop_count & troops ?
    ¿ there should be a call out to populate l_troop_count & l_troops ?
    ¿ Unit_Count should be troop_count ? OR ¿ Unit_Count should be initialized ?
*/
void UNIT_GetDependants__WIP(int16_t unit_idx, int16_t * troop_count, int16_t troops[])
{
    int16_t l_troops[MAX_STACK];
    int16_t l_troop_count;
    int16_t Have_Dependants;
    int16_t Windwalking_Unit;
    int16_t Stack_Has_WW;
    int16_t On_Ocean_Tile;
    int16_t Unit_Count;
    int16_t itr_Unit_Count;  // _SI_

    Unit_Count = 0;  // DNE in Dasm

    l_troop_count = 0;

    *troop_count = 0;

    if(_UNITS[unit_idx].owner_idx != -1)
    {
        Have_Dependants = ST_FALSE;

        if(Unit_Has_WindWalking(unit_idx) == ST_TRUE)  /* || (o71p08_Empty_pFxn(unit_idx) == ST_TRUE) */
        {
            On_Ocean_Tile = Square_Is_Sailable(UNITX(), UNITY(), UNITP());

            if(_unit_type_table[_UNITS[unit_idx].type].Transport > 0)
            {
                On_Ocean_Tile = ST_FALSE;
            }

            if(On_Ocean_Tile == ST_TRUE)
            {
                Windwalking_Unit = ST_FALSE;

                if(Unit_Has_WindWalking(unit_idx) == ST_TRUE)  /* || (o71p08_Empty_pFxn(unit_idx) == ST_TRUE) */
                {
                    Windwalking_Unit = ST_TRUE;
                }

                Stack_Has_WW = ST_FALSE;

                // ovr089:0C94                                                 Unit_Count= word ptr -2
                // ovr089:0C94                                                 _SI_itr_Unit_Count = si
                // ovr089:0C97 83 EC 1E                                        sub     sp, 1Eh
                // ovr089:0DBB 3B 76 FE                                        cmp     _SI_itr_Unit_Count, [bp+Unit_Count]
                // ¿ cmp ?
                // ¿ SI ?
                // ¿ BP + -2 ? ... -2d FEh
                // ~== ```CMP SI,BP+FE```
                
                for(itr_Unit_Count = 0; itr_Unit_Count < Unit_Count; itr_Unit_Count++)
                {
                    if(l_troops[itr_Unit_Count] != unit_idx)
                    {
                        if(Unit_Has_WindWalking(l_troops[itr_Unit_Count]) == ST_TRUE)  /* || (o71p08_Empty_pFxn(l_troops[itr_Unit_Count]) == ST_TRUE) */
                        {
                            Stack_Has_WW = ST_TRUE;
                        }
                    }
                }

                if(
                    (Stack_Has_WW != ST_FALSE) &&
                    (Windwalking_Unit != ST_TRUE)
                )
                {
                    Have_Dependants = ST_TRUE;
                }


            }
        }

        if(Have_Dependants == ST_TRUE)
        {
            for(itr_Unit_Count = 0; itr_Unit_Count < Unit_Count; itr_Unit_Count++)
            {
                if(
                    (Unit_Has_AirTravel(l_troops[itr_Unit_Count]) != ST_TRUE) &&
                    (Unit_Has_AirTravel_Item(l_troops[itr_Unit_Count]) != ST_TRUE) &&
                    (Unit_Has_WaterTravel(l_troops[itr_Unit_Count]) != ST_TRUE) &&
                    (Unit_Has_WaterTravel_Item(l_troops[itr_Unit_Count]) != ST_TRUE)
                )
                {
                    troops[l_troop_count] = l_troops[itr_Unit_Count];
                    l_troop_count++;
                }
            }
        }

        *troop_count = l_troop_count;
    }

}
