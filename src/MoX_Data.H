#ifndef MOX_DATA_H
#define MOX_DATA_H

#include "MoX_TYPE.H"
#include "MoM_DEF.H"

/*
    ¿ MoX vs. MoM ?

    Game Type

    Game Settings



    Player
    Unit
    City

    Fortress
    Tower
    Node
    Lair

    Unit Type
    Race Type
    Hero Type

    Building Type

    Items
    Spells

    Wizard Special Abilities
    Unit Special Abilities
    Hero Special Abilities



    struct s_UNIT
    struct s_BU_REC  (drake178: "Battle Unit")
    struct s_CMBT_DATA

*/



#define SAVEGAM_RECORD_SIZE 123300
#define MOXSET_RECORD_SIZE 466

// sizeof(struct s_CITY)
#define CITY_RECORD_SIZE         114
// sizeof(struct s_FORTRESS)
#define FORTRESS_RECORD_SIZE       4
// sizeof(struct s_HERO)
#define HERO_RECORD_SIZE          12
// sizeof(struct s_LAIR)
#define LAIR_RECORD_SIZE          24
// sizeof(struct s_NODE)
#define NODE_RECORD_SIZE          48
// sizeof(struct s_STACK)
#define STACK_RECORD_SIZE          4
// sizeof(struct s_TOWER)
#define TOWER_RECORD_SIZE          4
// sizeof(struct s_UNIT)
#define UNIT_RECORD_SIZE          32
// sizeof(struct s_WIZARD)
#define WIZARD_RECORD_SIZE      1224


#define HERO_NAME_RECORD_SIZE 16


// 6 * 35 * 12
#define PLAYER_HEROES_RECORD_SIZE      35*HERO_RECORD_SIZE

// #define NUM_PLAYERS_RECORD_SIZE 2
// #define LANDSIZE_RECORD_SIZE 2
// #define MAGIC_RECORD_SIZE 2
// #define DIFFICULTY_RECORD_SIZE 2
// #define NUM_CITIES_RECORD_SIZE 2
// #define NUM_UNITS_RECORD_SIZE 2
// #define TURN_RECORD_SIZE 2
// #define UNIT_RECORD_SIZE 2

// MAX_PLAYERS * sizeof(struct s_WIZARD)
#define WIZARDS_RECORD_SIE              6*WIZARD_RECORD_SIZE
#define WORLD_MAPS_RECORD_SIZE          2*2400*2
#define UU_TBL_1_RECORD_SIZE            2*96
#define UU_TBL_2_RECORD_SIZE            2*96
#define LANDMASSES_RECORD_SIZE          2*2400
#define NODES_RECORD_SIZE               30*NODE_RECORD_SIZE
// MAX_PLAYERS * sizeof(struct s_FORTRESS)
#define FORTRESSES_RECORD_SIZE          6*FORTRESS_RECORD_SIZE
// MAX_PLAYERS * sizeof(struct s_TOWER)
#define TOWERS_RECORD_SIZE              6*TOWER_RECORD_SIZE
#define LAIRS_RECORD_SIZE               102*LAIR_RECORD_SIZE
#define ITEMS_RECORD_SIZE               138*50
#define CITIES_RECORD_SIZE              100*CITY_RECORD_SIZE
#define UNITS_RECORD_SIZE               1009*UNIT_RECORD_SIZE
#define TERRAIN_SPECIALS_RECORD_SIZE    2*2400
#define UNEXPLORED_AREA_RECORD_SIZE     2*2400
#define MOVEMAPS_RECORD_SIZE            2*14400
#define EVENTS_TABLE_RECORD_SIZE        1*100
#define TERRAIN_FLAGS_RECORD_SIZE       2*2400
// #define GRAND_VIZIER_RECORD_SIZE     2
#define PREMADE_ITEMS_RECORD_SIZE       250*1
#define HERO_NAMES_RECORD_SIZE          35*HERO_NAME_RECORD_SIZE







// Game Option - Difficulty
enum e_Difficulty
{
    god_Intro,
    god_Easy,
    god_Normal,
    god_Hard,
    god_Impossible
};
enum e_Opponents
{
    goo_One,
    goo_Two,
    goo_Three,
    goo_Four
};
enum e_LandSize
{
    gol_Small,
    gol_Medium,
    gol_Large
};
enum e_Magic
{
    gom_Weak,
    gom_Normal,
    gom_Powerful
};




// _unit_type_table[].Attribs_1
// 00000000 struc s_UNIT_TYPE ; (sizeof=0x24)
// 0000001A Attribs_1 dw ?                          ; enum ATTRIB_1
// 0000001C Attribs_2 dw ?                          ; enum ATTRIB_2

// 01 ; enum ATTRIB_2 (bitfield)
// 01 Spl_Healing   = 0x01  // 0000 0001
// 02 Spl_Fireball  = 0x02
// 04 Spl_DoomBolt  = 0x04
// 08 Ab_Immolation = 0x08
// 10 Spl_Web       = 0x10
// 20 Ab_CauseFear  = 0x20
// 40 Ab_ResistAll  = 0x40
// 80 Ab_HolyBonus  = 0x80  // 1000 0000

// 0001 ; enum ATTRIB_1 (bitfield)
// 0001 Imm_Fire      = 1
// 0002 Imm_Stoning   = 2
// 0004 Imm_Missiles  = 4
// 0008 Imm_Illusion  = 8
// 0010 Imm_Cold      = 10h
// 0020 Imm_Magic     = 20h
// 0040 Imm_Death     = 40h
// 0080 Imm_Poison    = 80h
// 0100 Imm_Weapon    = 100h
// 0200 Ab_Flying     = 200h
// 0400 Abl_Lucky     = 400h
// 0800 Sum_Demon_1   = 800h
// 1000 Sum_Demon_2   = 1000h
// 2000 Ab_Caster_20  = 2000h
// 4000 Ab_Caster_40  = 4000h
// 8000 Std_Unit  = 8000h  // 1000 0000 0000 0000

enum Meld_Flags
{
    M_Warped,
    M_Guardian
};

enum EZ_Types
{
    EZ_Tower,
    EZ_Chaos_Node,
    EZ_Nature_Node,
    EZ_Sorcery_Node,
    EZ_Cave,
    EZ_Dungeon,
    EZ_Ancient_Temple,
    EZ_Keep,
    EZ_Monster_Lair,
    EZ_Ruins,
    EZ_Fallen_Temple
};



enum Unit_Status
{
    US_Ready,      /* "NO ORDERS" in 'MoM Demo' */
    US_Patrol,
    US_BuildRoad,
    US_GoingTo,
    US_ReachedDest,  /* "DONE" in 'MoM Demo' */
    US_Wait,
    US_Casting,    /* drake178: US_Unknown_6; "CASTING" in 'MoM Demo' */
    US_Unknown_7,  /* "PURIFY" in 'MoM Demo' */
    US_Purify,
    US_Meld,
    US_Settle,
    US_SeekTransport,
    US_Unknown_12,
    US_Unknown_13,
    US_Unknown_14,
    US_Unknown_15,
    US_Move,

    US_Unknown_100  = 100,   // 64h  100d  01100100b
    US_PurifyDone   = 111    // 6Fh  111d  01101111b
};



/*
    s_BLDG_DATA.Prereq_1
    s_BLDG_DATA.Prereq_2
    s_BLDG_DATA.Replaces

    s_UNIT_TYPE.Bldng1_or_Portrait
    s_UNIT_TYPE.Bldng2_or_HeroType

    s_RACE_TYPE.cant_build_count
    s_RACE_TYPE.cant_build[7]

NOTE: bldg_type_idx = product_idx if product_idx < 100 else unit_type_idx
*/
enum e_BUILDING_TYPE
{
    bt_GRANDVIZIER        =  -4,
    bt_INVALID            =  -1,
    bt_NONE               =   0,
    bt_TradeGoods         =   1,
    bt_Housing            =   2,
    bt_Barracks           =   3,
    bt_Armory             =   4,
    bt_FightersGuild      =   5,
    bt_ArmorersGuild      =   6,
    bt_WarCollege         =   7,
    bt_Smithy             =   8,
    bt_Stable             =   9,
    bt_AnimistsGuild      =  10,
    bt_FantasticStable    =  11,
    bt_Shipwright         =  12,
    bt_ShipYard           =  13,
    bt_MaritimeGuild      =  14,
    bt_Sawmill            =  15,
    bt_Library            =  16,
    bt_SagesGuild         =  17,
    bt_Oracle             =  18,
    bt_AlchemistsGuild    =  19,
    bt_University         =  20,
    bt_WizardsGuild       =  21,
    bt_Shrine             =  22,
    bt_Temple             =  23,
    bt_Parthenon          =  24,
    bt_Cathedral          =  25,
    bt_Marketplace        =  26,
    bt_Bank               =  27,
    bt_MerchantsGuild     =  28,
    bt_Granary            =  29,
    bt_FarmersMarket      =  30,
    bt_ForestersGuild     =  31,
    bt_BuildersHall       =  32,
    bt_MechaniciansGuild  =  33,
    bt_MinersGuild        =  34,
    bt_CityWalls          =  35,

    bt_BREQ_Forest        = 101,
    bt_BREQ_Grass         = 104,
    bt_BREQ_Water         = 110,
    bt_BREQ_Hill          = 200
};


/*
drake178:
enum Building_Status
B_Replaced  = 0
B_Built  = 1
B_Destroyed  = 2
B_Not_Built  = 0FFh
*/
/*
    int8_t
    s_CITY.buildings[NUM_BUILDINGS]

*/
enum e_BUILDING_STATUS
{
    bs_NotBuilt  = -1,
    bs_Replaced  =  0,
    bs_Built     =  1,
    bs_Removed   =  2   // Sold or Destroyed
};


/*
    Magic Realm

not in order of S_WIZARD.spellranks[]  {nature, sorcery, chaos, life, death}
in MsgBox, xlats from idx to mr  
*/
enum e_MAGIC_REALM
{
    mr_Chaos    = 0,
    mr_Death    = 1,
    mr_Life     = 2,
    mr_Nature   = 3,
    mr_Sorcery  = 4
};
/*
    s_WIZARD.spellranks
    'Message Box' - type/color
*/
enum e_SPELLBOOK_RANK
{
    sbr_Nature   = 0,
    sbr_Sorcery  = 1,
    sbr_Chaos    = 2,
    sbr_Life     = 3,
    sbr_Death    = 4
};


/*
    Enchantment

struct s_UNIT
    / * 18 * /  int16_t Enchants_LO;  // ; enum UE_FLAGS_L
    / * 1A * /  int16_t Enchants_HI;  // ; enum UE_FLAGS_H

DX:AX LO:HI
~ uint32_t

*/
//         if( (Unit_Enchantments_LO & 0x40) != 0)  /* UE_Regeneration */
//         {
//             outline_color_index = 3;  // Nature
//         }
//         if( (Unit_Enchantments_HI & 0x40) != 0)  /* UE_Resist_Magic */
//         {
//             outline_color_index = 3;  // Sorcery
//         }

/*
; enum UE_FLAGS_L (bitfield)
UE_Immolation  = 1
UE_Guardian_Wind  = 2
UE_Berserk  = 4
UE_Cloak_of_Fear  = 8
UE_Black_Channels  = 10h
UE_Wraith_Form  = 20h
UE_Regeneration  = 40h
UE_Path_Finding  = 80h
UE_Water_Walking  = 100h
UE_Resist_Elements  = 200h
UE_Elemental_Armor  = 400h
UE_Stone_Skin  = 800h
UE_Iron_Skin  = 1000h
UE_Endurance  = 2000h
UE_Spell_Lock  = 4000h
UE_Invisibility  = 8000h
*/
/*
; enum UE_FLAGS_H (bitfield)
UE_Wind_Walking  = 1
UE_Flight  = 2
UE_Resist_Magic  = 4
UE_Magic_Immunity  = 8
UE_Flame_Blade  = 10h
UE_Eldritch_Weapon  = 20h
UE_True_Sight  = 40h
UE_Holy_Weapon  = 80h
UE_Heroism  = 100h
UE_Bless  = 200h
UE_Lionheart  = 400h
UE_Giant_Strength  = 800h
UE_Planar_Travel  = 1000h
UE_Holy_Armor  = 2000h
UE_Righteousness  = 4000h
UE_Invulnerability  = 8000h
*/

// #define UE_Immolation       0x0001
// #define UE_Guardian_Wind    0x0002
// #define UE_Berserk          0x0004
// #define UE_Cloak_of_Fear    0x0008
// #define UE_Black_Channels   0x0010
// #define UE_Wraith_Form      0x0020
// #define UE_Regeneration     0x0040
// #define UE_Path_Finding     0x0080
// #define UE_Water_Walking    0x0100
// #define UE_Resist_Elements  0x0200
// #define UE_Elemental_Armor  0x0400
// #define UE_Stone_Skin       0x0800
// #define UE_Iron_Skin        0x1000
// #define UE_Endurance        0x2000
// #define UE_Spell_Lock       0x4000
// #define UE_Invisibility     0x8000

// #define UE_Wind_Walking     0x0001
// #define UE_Flight           0x0002
// #define UE_Resist_Magic     0x0004
// #define UE_Magic_Immunity   0x0008
// #define UE_Flame_Blade      0x0010
// #define UE_Eldritch_Weapon  0x0020
// #define UE_True_Sight       0x0040
// #define UE_Holy_Weapon      0x0080
// #define UE_Heroism          0x0100
// #define UE_Bless            0x0200
// #define UE_Lionheart        0x0400
// #define UE_Giant_Strength   0x0800
// #define UE_Planar_Travel    0x1000
// #define UE_Holy_Armor       0x2000
// #define UE_Righteousness    0x4000
// #define UE_Invulnerability  0x8000
// 
// #define UE_Immolation       0x00010000
// #define UE_Guardian_Wind    0x00020000
// #define UE_Berserk          0x00040000
// #define UE_Cloak_of_Fear    0x00080000
// #define UE_Black_Channels   0x00100000
// #define UE_Wraith_Form      0x00200000
// #define UE_Regeneration     0x00400000
// #define UE_Path_Finding     0x00800000
// #define UE_Water_Walking    0x01000000
// #define UE_Resist_Elements  0x02000000
// #define UE_Elemental_Armor  0x04000000
// #define UE_Stone_Skin       0x08000000
// #define UE_Iron_Skin        0x10000000
// #define UE_Endurance        0x20000000
// #define UE_Spell_Lock       0x40000000
// #define UE_Invisibility     0x80000000
// 
// #define UE_Wind_Walking     0x00000001
// #define UE_Flight           0x00000002
// #define UE_Resist_Magic     0x00000004
// #define UE_Magic_Immunity   0x00000008
// #define UE_Flame_Blade      0x00000010
// #define UE_Eldritch_Weapon  0x00000020
// #define UE_True_Sight       0x00000040
// #define UE_Holy_Weapon      0x00000080
// #define UE_Heroism          0x00000100
// #define UE_Bless            0x00000200
// #define UE_Lionheart        0x00000400
// #define UE_Giant_Strength   0x00000800
// #define UE_Planar_Travel    0x00001000
// #define UE_Holy_Armor       0x00002000
// #define UE_Righteousness    0x00004000
// #define UE_Invulnerability  0x00008000
// 
// #define GET_ENCHANTMENT_MAGIC_REALM((Unit_Enchantments_HI),(Unit_Enchantments_LO)) (  )


// drake178: WPN_QLTY
enum e_WEAPON_QUALITY
{
    wq_Normal,
    wq_Magic,
    wq_Mithril,
    wq_Adamantium
};





// sizeof:  0Ah  10d
struct s_BOOKSHELF
{
    /* 00 */ int16_t Nature;
    /* 02 */ int16_t Sorcery;
    /* 04 */ int16_t Chaos;
    /* 06 */ int16_t Life;
    /* 08 */ int16_t Death;
};

// sizeof:  12h  18d
struct s_RETORTS
{
    /* 00 */ int8_t Alchemy;
    /* 01 */ int8_t Warlord;
    /* 02 */ int8_t Chaos_Mastery;
    /* 03 */ int8_t Nature_Mastery;
    /* 04 */ int8_t Sorcery_Mastery;
    /* 05 */ int8_t Infernal_Power;
    /* 06 */ int8_t Divine_Power;
    /* 07 */ int8_t Sage_Master;
    /* 08 */ int8_t Channeler;
    /* 09 */ int8_t Myrran;
    /* 0A */ int8_t Archmage;
    /* 0B */ int8_t Mana_Focusing;
    /* 0C */ int8_t Node_Mastery;
    /* 0D */ int8_t Famous;
    /* 0E */ int8_t Runemaster;
    /* 0F */ int8_t Conjurer;
    /* 10 */ int8_t Charismatic;
    /* 11 */ int8_t Artificer;
};

// sizeof:  24h  36d
struct s_SPELL_DATA
{
    /* 00 */ char Name[19];
    /* 13 */  int8_t AI_Group;              //  ; enum AI_Spell_Groups
    /* 14 */  int8_t AI_Value;
    /* 15 */  int8_t Type;                  //  ; enum SPELLTYPES
    /* 16 */  int8_t Book_Section;
    /* 17 */  int8_t Realm;
    /* 18 */  int8_t Eligibility;
    /* 19 */  int8_t Unknown_19h;
    /* 1A */  int16_t Casting_Cost;
    /* 1C */  int16_t Resrch_Cost;
    /* 1E */  int8_t Sound;
    /* 1F */  int8_t Unknown_1Fh;
    /* 20 */  int8_t Param0;                //  ; unit type, base damage, UE flag, or CE index
    /* 21 */  int8_t Param1;
    /* 22 */  int16_t Params2_3;            //  ; ATK_Flags
    /* 24 */
};

// sizeof:  0Ch  12d
struct s_HERO
{
    /* 00 */  int16_t  Level;  // ¿ experience (level) ?
    /* 02 */  uint16_t Abilities_LO;  // ; enum Hero_Ability_LO
    /* 04 */  uint16_t Abilities_HI;  // ; enum Hero_Ability_HI
    /* 06 */  int8_t   Casting_Skill;
    /* 07 */  uint8_t  Spells[4];
    /* 0B */  uint8_t  Unused_0Bh;      /* ¿ padding byte to align-2 or Spells was [5] ? */
    /* 0C */
};

// sizeof:  1Ch  28d
struct s_HIRED_HERO
{
    /* 00 */ int16_t Unit_Index;  /* unit_dx, back into _UNITS[] */
    /* 02 */ char Name[14];
    /* 10 */ int16_t Items[3];      /* ¿ index into TBL_Items ? */
    /* 16 */ int16_t Item_Slots[3];
    /* 1C */
};

// drake178: INACTV_HERO
// sizeof:  10h  16d
#pragma pack(push)
#pragma pack(2)
struct s_INACTV_HERO
{
    /* 00 */  char Name[20];
    /* 0E */  int16_t XP;
    /* 10 */
};
#pragma pack(pop)

// sizeof:  32h  50d
struct s_ITEM
{
    /* 00 */  char Name[30];
    /* 1E */  int16_t Icon_Index;
    /* 20 */  int8_t Slot_Type;
    /* 21 */  int8_t Item_Type;
    /* 22 */  int16_t Mana_Value;
    /* 24 */  int8_t Bonus_Attack;
    /* 25 */  int8_t Bonus_To_Hit;
    /* 26 */  int8_t Bonus_Defense;
    /* 27 */  int8_t Bonus_HalfMoves;
    /* 28 */  int8_t Bonus_Resist;
    /* 29 */  int8_t Bonus_Mana;
    /* 2A */  int8_t Spell_Save;
    /* 2B */  int8_t Charged_Spell;
    /* 2C */  int16_t Charges;
    // TODO  /* 2E */  Powers_LOWord dw ?                      // ; enum IPOW_LO
    // TODO  /* 30 */  Powers_HOWord dw ?                      // ; enum IPOW_HO
    /* 2E */  uint32_t Powers;
    /* 32 */
};

// 00000001 ; enum IPOW_LO (bitfield)
// 00000001 IP_Vampiric  = 1
// 00000002 IP_Guardian_Wind  = 2
// 00000004 IP_Lightning  = 4
// 00000008 IP_Cloak_Of_Fear  = 8
// 00000010 IP_Destruction  = 10h
// 00000020 IP_Wraith_Form  = 20h
// 00000040 IP_Regeneration  = 40h
// 00000080 IP_Pathfinding  = 80h
// 00000100 IP_Water_Walking  = 100h
// 00000200 IP_Resist_Elements  = 200h
// 00000400 IP_Elemental_Armour  = 400h
// 00000800 IP_Chaos  = 800h
// 00001000 IP_Stoning  = 1000h
// 00002000 IP_Endurance  = 2000h
// 00004000 IP_Haste  = 4000h
// 00008000 IP_Invisibility  = 8000h
// 
// 00000001 ; enum IPOW_HI (bitfield)
// 00000001 IP_Death  = 1
// 00000002 IP_Flight  = 2
// 00000004 IP_Resist_Magic  = 4
// 00000008 IP_Magic_Immunity  = 8
// 00000010 IP_Flaming  = 10h
// 00000020 IP_Holy_Avenger  = 20h
// 00000040 IP_True_Sight  = 40h
// 00000080 IP_Phantasmal  = 80h
// 00000100 IP_Power_Drain  = 100h
// 00000200 IP_Bless  = 200h
// 00000400 IP_Lion_Heart  = 400h
// 00000800 IP_Giant_Strength  = 800h
// 00001000 IP_Planar_Travel  = 1000h
// 00002000 IP_Merging  = 2000h
// 00004000 IP_Righteousness  = 4000h
// 00008000 IP_Invulnerability  = 8000h


// sizeof:  132h  306d
struct s_WIZ_DIPL
{
    /* 0000 */ int8_t Contacted[6];
    /* 0006 */ int16_t Treaty_Interest[6];
    /* 0012 */ int16_t Peace_Interest[6];
    /* 001E */ int16_t Trade_Interest[6];
    /* 002A */ int8_t Visible_Rel[6];
    /* 0030 */ int8_t Dipl_Status[6];
    /* 0036 */ int16_t DA_Strength[6];
    /* 0042 */ int8_t Dipl_Action[6];
    /* 0048 */ int16_t DA_Spell[6];
    /* 0054 */ int8_t DA_City[6];
    /* 005A */ int8_t Default_Rel[6];
    /* 0060 */ int8_t Contact_Progress[6];
    /* 0066 */ int8_t Broken_Treaty[6];
    /* 006C */ int16_t Unknown_194h[6];
    /* 0078 */ int8_t Hidden_Rel[6];
    /* 007E */ int8_t Unknown_1A6h[24];
    /* 0096 */ int8_t Tribute_Spell[6];
    /* 009C */ int8_t Unknown_1C4h[90];
    /* 00F6 */ int16_t Tribute_Gold[6];
    /* 0102 */ int8_t Unknown_22Ah[30];
    /* 0120 */ int8_t Unknown_248h[6];
    /* 0126 */ int8_t Unknown_24Eh[6];
    /* 012C */ int8_t G_Warning_Progress[6];
    /* 0132 */
};

/*
    computer player wizards - difficulty level modifiers
*/
// sizeof:  10h  16d
struct s_DIFFICULTY_MODIFIERS
{
    /* 00 */  int16_t population_growth;
    /* 02 */  int16_t outpost_growth;
    /* 04 */  int16_t production;
    /* 06 */  int16_t gold;
    /* 08 */  int16_t mana;
    /* 0A */  int16_t research;
    /* 0C */  int16_t food;
    /* 0E */  int16_t maintenance;
    /* 10 */
};

// sizeof:  06h  6d
struct s_Astr_Data
{
    /* 00 */ int16_t Magic_Power;
    /* 02 */ int16_t Spell_Research;
    /* 04 */ int16_t Army_Strength;
};

// sizeof:  18h  24d
struct s_GLOBALE
{
    /* 00 */ int8_t Eternal_Night;
    /* 01 */ int8_t Evil_Omens;
    /* 02 */ int8_t Zombie_Mastery;
    /* 03 */ int8_t Aura_of_Majesty;
    /* 04 */ int8_t Wind_Mastery;
    /* 05 */ int8_t Suppress_Magic;
    /* 06 */ int8_t Time_Stop;
    /* 07 */ int8_t Nature_Awareness;
    /* 08 */ int8_t Natures_Wrath;
    /* 09 */ int8_t Herb_Mastery;
    /* 0A */ int8_t Chaos_Surge;
    /* 0B */ int8_t Doom_Mastery;
    /* 0C */ int8_t Great_Wasting;
    /* 0D */ int8_t Meteor_Storm;
    /* 0E */ int8_t Armageddon;
    /* 0F */ int8_t Tranquility;
    /* 10 */ int8_t Life_Force;
    /* 11 */ int8_t Crusade;
    /* 12 */ int8_t Just_Cause;
    /* 13 */ int8_t Holy_Arms;
    /* 14 */ int8_t Planar_Seal;
    /* 15 */ int8_t Charm_of_Life;
    /* 16 */ int8_t Detect_Magic;
    /* 17 */ int8_t Awareness;
    /* 18 */
};



// sizeof:  10h  16d
struct s_GAME_DATA
{
    /* 00 */ uint16_t Current_Unit;
    /* 02 */ uint16_t Turn;
    /* 04 */ uint16_t Total_Units;
    /* 06 */ uint16_t Total_Cities;
    /* 08 */ uint16_t Difficulty;
    /* 0A */ uint16_t Magic;
    /* 0C */ uint16_t Land_Size;
    /* 0E */ uint16_t Total_Wizards;
    /* 10 */
};


/*
    No idea if it actually matters, but
    the pragma pack is here because this thing
        uint32_t Casting_Skill
    is causing a 2 byte bump for alignment to 4-byte
    and it was messing with my head
    when I was trying to debug the struct for the file read
*/

#pragma pack(push)
#pragma pack(2)
// sizeof:  4C8  1224d
struct s_WIZARD
{
    /* 0000 */ uint8_t wizard_id; /* drake178: Protrait */
    /* 0001 */ char Name[20];
    /* 0015 */ uint8_t Cptl_Race;
    /* 0016 */ uint8_t banner_id;  /* drake178: Banner */
    /* 0017 */ uint8_t Unused_017h;
    /* 0018 */ uint16_t Personality;            /* enum AI_Personality */
    /* 001A */ uint16_t Objective;              /* enum AI_Objective */
    /* 001C */ uint8_t Unknown_01Ch[6];
    /* 0022 */ uint16_t SoM_RC;
    /* 0024 */ uint16_t Fame;
    /* 0026 */ uint16_t Power_Base;
    /* 0028 */ uint16_t Volcano_Count;
    /* 002A */ uint8_t Research_Pcnt;
    /* 002B */ uint8_t Mana_Pnct;
    /* 002C */ uint8_t Skill_Pcnt;
    /* 002D */ uint8_t Volcano_Power;
    /* 002E */ int16_t summon_wx;
    /* 0030 */ int16_t summon_wy;
    /* 0032 */ int16_t summon_wp;
    /* 0034 */ uint16_t Rsrch_Candidates[8];
    /* 0044 */ uint8_t Unknown_044h[4];
    /* 0048 */ uint16_t Avg_Unit_Value;         /* neutral: rampage accumulator */
    /* 004A */ uint16_t Unknown_04Ah;
    /* 004C */ uint16_t Cmbt_Skill_Left;
    /* 004E */ uint16_t Cast_Cost_Left;
    /* 0050 */ uint16_t Cast_Cost_Initial;      /* neutral: raiders accumulator */
    /* 0052 */ uint16_t Spell_Cast;
    /* 0054 */ uint16_t Skill_Left;
    /* 0056 */ uint16_t Nominal_Skill;
    /* 0058 */ uint16_t tax_rate;

/* 005A struct s_BOOKSHELF Books; */
//     /* 005A */ int16_t spellrank_nature;   +0
//     /* 005C */ int16_t spellrank_sorcery;  +2
//     /* 005E */ int16_t spellrank_chaos;    +4
//     /* 0060 */ int16_t spellrank_life;     +6
//     /* 0062 */ int16_t spellrank_death;    +8
    /* 005A */ int16_t spellranks[5];

/* 0064 struct s_RETORTS Retorts; */
    /* 064 */ int8_t alchemy;
    /* 065 */ int8_t warlord;
    /* 066 */ int8_t chaos_mastery;
    /* 067 */ int8_t nature_mastery;
    /* 068 */ int8_t sorcery_mastery;
    /* 069 */ int8_t infernal_power;
    /* 06A */ int8_t divine_power;
    /* 06B */ int8_t sage_master;
    /* 06C */ int8_t channeler;
    /* 06D */ int8_t myrran;
    /* 06E */ int8_t archmage;
    /* 06F */ int8_t mana_focusing;
    /* 070 */ int8_t node_mastery;
    /* 071 */ int8_t famous;
    /* 072 */ int8_t runemaster;
    /* 073 */ int8_t conjurer;
    /* 074 */ int8_t charismatic;
    /* 075 */ int8_t artificer;

    /* 0076 */ struct s_HIRED_HERO Heroes[6];  /* indexed by Hero_Slot, from s_UNIT */
    /* 011E */ uint16_t Unknown_11Eh;
    /* 0120 */ uint16_t Vault_Items[4];
    /* 0128 */ struct s_WIZ_DIPL Dipl;
    /* 025A */ uint16_t Research_Left;
    /* 025C */ uint16_t mana_reserve;  /*  */
    /* 025E */ int32_t Casting_Skill;
    /* 0262 */ uint16_t Researching;
    /* 0264 */ uint8_t Spells_Known[240];
    /* 0354 */ uint16_t Defeated_Wizards;
    /* 0356 */ uint16_t gold_reserve;  /*  */
    /* 0358 */ uint16_t Unknown_358h;
    /* 035A */ struct s_Astr_Data Astr;
    /* 0360 */ uint16_t Pop_div_10k;
    /* 0362 */ uint8_t Historian[288];
    /* 0482 */ // struct s_GLOBALE Globals;
               uint8_t Globals[24];
    /* 049A */ uint16_t Magic_Strategy;
    /* 049C */ uint16_t Unknown_49Ch;
    /* 049E */ uint16_t Hostility[6];
    /* 04AA */ uint16_t War_Reeval;
    /* 04AC */ uint16_t MgcStrat_Reeval;
    /* 04AE */ uint16_t Pow_Redist_Wait;
    /* 04B0 */ uint8_t Peace_Counters[6];
    /* 04B6 */ uint8_t field_4B6;
    /* 04B7 */ uint8_t field_4B7;
    /* 04B8 */ uint16_t field_4B8;
    /* 04BA */ uint16_t OVL_TargetWiz;
    /* 04BC */ uint8_t field_4BC;
    /* 04BD */ uint8_t field_4BD;
    /* 04BE */ uint8_t Unknown_4BEh[6];
    /* 04C4 */ uint16_t Prim_Realm;
    /* 04C6 */ uint16_t Sec_Realm;
};
#pragma pack(pop)


// drake178: struc BLD_REC
// sizeof:  34h  52d
struct s_BLDG_DATA
{
    /* 00 */  char name[20];
    /* 14 */  // int16_t reqd_bldg_1;          // bldg_idx  >= 100 is Terrain Type
    union
    {
        int16_t reqd_bldg_1;
        int16_t reqd_terrain;
    };
    /* 16 */  int16_t reqd_bldg_2;          // bldg_idx
    /* 18 */  int16_t replace_bldg;         // bldg_idx
    /* 1A */  int16_t Grants20XP;
    /* 1C */  int16_t Grants60XP;
    /* 1E */  int16_t Alchemist;
    /* 20 */  int16_t maintenance_cost;  // in gold units  AKA upkeep cost
    /* 22 */  int16_t AIVal_Growth;
    /* 24 */  int16_t Unknown_24h;
    /* 26 */  int16_t Unknown_26h;
    /* 28 */  int16_t AIVal_Religion;
    /* 2A */  int16_t AIVal_Research;
    /* 2C */  int16_t construction_cost;  // in production units
    /* 2E */  int16_t Unknown_2Eh;
    /* 30 */  int16_t G_Animation;
    /* 32 */  int16_t Category;
    /* 34 */
};


// sizeof:  0Eh  14d
struct s_BLDG
{
    /* 00 */  int16_t bldg_idx;  // enum e_BUILDING_TYPE
    /* 02 */  int16_t x1;
    /* 04 */  int16_t y1;
    /* 06 */  int16_t x2;
    /* 08 */  int16_t y2;
    /* 0A */  int16_t field_A;
    /* 0C */  int16_t field_C;
    /* 0E */
};


// sizeof:  24h  36d
struct s_CITY_BUILDINGS
{
    /* 00 */ int8_t None;
    /* 01 */ int8_t Housing;
    /* 02 */ int8_t Trade_Goods;
    /* 03 */ int8_t Barracks;
    /* 04 */ int8_t Armory;
    /* 05 */ int8_t Fighters_Gld;
    /* 06 */ int8_t Armorers_Gld;
    /* 07 */ int8_t War_College;
    /* 08 */ int8_t Smithy;
    /* 09 */ int8_t Stable;
    /* 0A */ int8_t Animists_Gld;
    /* 0B */ int8_t Fantastic_Stbl;
    /* 0C */ int8_t Shipwright_Gld;
    /* 0D */ int8_t Ship_Yard;
    /* 0E */ int8_t Maritime_Gld;
    /* 0F */ int8_t Sawmill;
    /* 10 */ int8_t Library;
    /* 11 */ int8_t Sages_Gld;
    /* 12 */ int8_t Oracle;
    /* 13 */ int8_t Alchemists_Gld;
    /* 14 */ int8_t University;
    /* 15 */ int8_t Wizards_Gld;
    /* 16 */ int8_t Shrine;
    /* 17 */ int8_t Temple;
    /* 18 */ int8_t Parthenon;
    /* 19 */ int8_t Cathedral;
    /* 1A */ int8_t Marketplace;
    /* 1B */ int8_t Bank;
    /* 1C */ int8_t Merchants_Gld;
    /* 1D */ int8_t Granary;
    /* 1E */ int8_t Farmers_Mrkt;
    /* 1F */ int8_t Foresters_Gld;
    /* 20 */ int8_t Builders_Hall;
    /* 21 */ int8_t Mechanicians;
    /* 22 */ int8_t Miners_Gld;
    /* 23 */ int8_t City_Walls;
};




// sizeof:  19h  25d
struct s_CITY_ENCHANTMENTS
{
    /* 00 */ int8_t Wall_of_Fire;
    /* 01 */ int8_t _Chaos_Rift;
    /* 02 */ int8_t Dark_Rituals;
    /* 03 */ int8_t _Evil_Presence;
    /* 04 */ int8_t _Cursed_Lands;
    /* 05 */ int8_t _Pestilence;
    /* 06 */ int8_t Cloud_of_Shadow;
    /* 07 */ int8_t _Famine;
    /* 08 */ int8_t Flying_Fortress;
    /* 09 */ int8_t Nature_Ward;
    /* 0A */ int8_t Sorcery_Ward;
    /* 0B */ int8_t Chaos_Ward;
    /* 0C */ int8_t Life_Ward;
    /* 0D */ int8_t Death_Ward;
    /* 0E */ int8_t Natures_Eye;
    /* 0F */ int8_t Earth_Gate;
    /* 10 */ int8_t Stream_of_Life;
    /* 11 */ int8_t Gaias_Blessing;
    /* 12 */ int8_t Inspirations;
    /* 13 */ int8_t Prosperity;
    /* 14 */ int8_t Astral_Gate;
    /* 15 */ int8_t Heavenly_Light;
    /* 16 */ int8_t Consecration;
    /* 17 */ int8_t Wall_of_Darkness;
    /* 18 */ int8_t Altar_of_Battle;
    /* 19 */
};



// sizeof:  72h  114d
struct s_CITY
{
    /* 00 */ char name[14];  // pointer  2-byte in 16-bit DOS  8-byte in 64-bit Windows
    /* 0E */ int8_t race;
    /* 0F */ int8_t wx;
    /* 10 */ int8_t wy;
    /* 11 */ int8_t wp;
    /* 12 */ int8_t owner_idx;
    /* 13 */ int8_t size;
    /* 14 */ int8_t population;  // in Units of 1000 (1K)
    /* 15 */ int8_t farmer_count;
    /* 16 */ int8_t Building_Sold;  // {F/T} sold a building a this turn
    /* 17 */ int8_t Unused_17h;  // probably align-2 padding
    /* 18 */ int16_t Pop_10s;
    /* 1A */ int8_t PlayerBits;
    /* 1B */ int8_t Unused_1Bh;  // probably align-2 padding
    /* 1C */ int16_t construction;  /* ~== `product_idx`  < 100 building, >= 100 unit .. 0 is meaning? ... 99 max bldg, 256 - 1 - 99 = 156 max unit types? */
    /* 1E */ int8_t bldg_cnt;
    /* 1F */ // struct s_CITY_BUILDINGS buildings;
             int8_t bldg_status[NUM_BUILDINGS];  // ~ Building Status  ¿ {..., -1, 0, 1, 2, ...} ?  unbuilt, replaced, built, sold/destroyed
    /* 43 */ // struct s_CITY_ENCHANTMENTS enchantments;
             // uint8_t enchantments[26];  /* ¿ 0 for NONE, (player_idx + 1) for which wizard has which spell on this city ? */
    /* 5C */ // int8_t Nightshade;  as seens in Player_City_Enchantments_Upkeep() for city_enchantment_upkeep_table
             uint8_t enchantments[NUM_CITY_ENCHANTMENTS];
    /* 5D */ int8_t production_units;
    /* 5E */ int16_t Prod_Accu;
    /* 60 */ uint8_t gold_units;  // AKA Income
    /* 61 */ int8_t building_maintenance;  // AKA Upkeep
    /* 62 */ int8_t mana_units;
    /* 63 */ int8_t research_units;
    /* 64 */ int8_t food_units;
    /* 65 */ int8_t road_connections[13];  // Bit-Field; use Test_Bit_Field(); cit_idx is city_idx;
    /* 66 */ 
};

// sizeof:  18h  24d
struct s_LAIR
{
    /* 00 */  int8_t   wx;
    /* 01 */  int8_t   wy;
    /* 02 */  int8_t   wp;
    /* 03 */  int8_t   Intact;
    /* 04 */  int8_t   Type;
    /* 05 */  int8_t   Defender1_Index;
    /* 06 */  int8_t   Def1_Had_n_Left;
    /* 07 */  int8_t   Defender2_Index;
    /* 08 */  int8_t   Def2_Had_n_Left;
    /* 09 */  int8_t   Unused_09h;  // ¿ 2-byte alignment padding ? CLUE: here is odd and next one is 2-byte value
    /* 0A */  int16_t  Loot_Gold;
    /* 0C */  int16_t  Loot_Mana;
    /* 0E */  int8_t   Spell_n_Special;
    /* 0F */  int8_t   Misc_Flags;
    /* 10 */  int8_t   Item_Count;
    /* 11 */  int8_t   Unused_11h;  // ¿ 2-byte alignment padding ? CLUE: here is odd and next one is 2-byte value
    /* 12 */  int16_t  Item_Values[3];
    /* 18 */
};

// sizeof:  30h  48d
struct s_NODE
{
    /* 00 */  int8_t  wx;
    /* 01 */  int8_t  wy;
    /* 02 */  int8_t  wp;
    /* 03 */  int8_t  owner_idx;
    /* 04 */  int8_t  power;        // used to index Aura_Xs,Ys and added to node_anim_ctr for FLIC_Set_CurrentFrame()
    /* 05 */  int8_t  Aura_Xs[20];
    /* 19 */  int8_t  Aura_Ys[20];
    /* 2D */  int8_t  type;
    /* 2E */  int8_t  Meld_Flags;
    /* 2F */  int8_t  Unused_2Fh;  // ¿ 2-byte alignment padding ?
};


// in RACETYPE.H
// // sizeof: 18h  24d
// struct s_RACE
// {
//     /* 0x00 */ int16_t Name;  // offset
//     /* 0x02 */ int16_t CantBuild_Count;
//     /* 0x04 */ int16_t CantBuild[7];  // enum BLD_TYPE
//     /* 0x12 */ int16_t Outpost_Growth_Pcnt;
//     /* 0x14 */ int16_t Growth_Mod;
//     /* 0x16 */ int16_t HouseType;
//     /* 0x18 */
// };

// sizeof:  20h  32d
#pragma pack(push)
#pragma pack(2)
struct s_UNIT
{
    /* 00 */  int8_t  wx;
    /* 01 */  int8_t  wy;
    /* 02 */  int8_t  wp;
    /* 03 */  int8_t  owner_idx;    /* set to ST_UNDEFINED when a unit dies */
    /* 04 */  int8_t  HalfMoves_Max;
    /* 05 */  uint8_t type;         /* Unit Type Index |-> Unit Type Table */
    /* 06 */  int8_t  Hero_Slot;    /* index into _players[owner_idx].Heroes[] */
    /* 07 */  int8_t  Finished;     /* FALSE/TRUE; Unit is out of Movement Points or is Busy performing a Unit Action (Go-To, Build Road, Purify, etc.) */
    /* 08 */  int8_t  HMoves;       /* Movement Points, as half-points */
    /* 09 */  int8_t  dst_wx;       /* Go-To Destination World X */
    /* 0A */  int8_t  dst_wy;       /* Go-To Destination World Y */
    /* 0B */  int8_t  Status;
    /* 0C */  int8_t  Level;
    /* 0D */  int8_t  Unused_0Dh;  // ¿ 2-byte alignment padding ? CLUE: here is odd and next one is 2-byte value
    /* 0E */  int16_t XP;
    /* 10 */  int8_t  Move_Failed;
    /* 11 */  int8_t  Damage;
    /* 12 */  int8_t  Draw_Priority;
    /* 13 */  int8_t  Unknown_13h;  // ¿ 2-byte alignment padding ? CLUE: here is odd and next one is 2-byte value
    /* 14 */  int16_t In_Tower;     /* FALSE/TRUE;  Unit is in a Tower of Wizardry */
    /* 16 */  int8_t  Sight_Range;
    /* 17 */  int8_t  Mutations;    // ; enum MUT_FLAGS
    /* 18 */  int16_t Enchants_LO;  // ; enum UE_FLAGS_L
    /* 1A */  int16_t Enchants_HI;  // ; enum UE_FLAGS_H
              // TODO  uint32_t enchantments;
    /* 1C */  int8_t  Rd_Constr_Left;
    /* 1D */  int8_t  Rd_From_X;
    /* 1E */  int8_t  Rd_From_Y;
    /* 1F */  int8_t  Unknown_1Fh;  // ¿ 2-byte alignment padding ?
    /* 20 */
};
#pragma pack(pop)


// // sizeof:  24h  36d
// struct s_UNIT_TYPE
//     /* 00 */  char **  Name;                    /* offset (00036AA0) */
//     /* 02 */  int8_t   Melee;                   /* base 10 */
//     /* 03 */  int8_t   Ranged;                  /* base 10 */
//     /* 04 */  int8_t   Ranged_Type;
//     /* 05 */  int8_t   Ammo;                    /* base 10 */
//     /* 06 */  int8_t   To_Hit;                  /* base 10 */
//     /* 07 */  int8_t   Defense;                 /* base 10 */
//     /* 08 */  int8_t   Resist;                  /* base 10 */
//     /* 09 */  int8_t   Move_Halves;             /* base 10 */
//     /* 0A */  int16_t  Cost;                    /* base 10 */
//     /* 0C */  int8_t   Upkeep;                  /* base 10 */
//     /* 0D */  int8_t   Race;                    /* enum Race_Code */
//     /* 0E */  int8_t   Bldng1_or_Portrait;      /* enum BLD_TYPE */
//     /* 0F */  int8_t   Bldng2_or_HeroType;      /* enum BLD_TYPE */
//     /* 10 */  SAMB_ptr pict_seg;                /* segment pointers to LBX_Alloc_Space headers for reserved EMM file links */
//     /* 12 */  int8_t   Hits;                    /* base 10 */
//     /* 13 */  int8_t   Sight;                   /* base 10 */
//     /* 14 */  int8_t   Transport;               /* base 10 */
//     /* 15 */  int8_t   Figures;                 /* base 10 */
//     /* 16 */  int8_t   Construction;            /* base 10 */
//     /* 17 */  int8_t   Spec_Att_Attr;           /* base 10 */
//     /* 18 */  uint16_t Move_Flags;              /* enum MOVEFLAGS */  /* TODO: check the data-type - UnitHasFlying() uses `byte ptr` and `CBW` */
//     /* 1A */  uint16_t Attribs_1;               /* enum ATTRIB_1 */
//     /* 1C */  uint16_t Attribs_2;               /* enum ATTRIB_2 */
//     /* 1E */  uint16_t Abilities;               /* enum ABL_FLAGS */
//     /* 20 */  uint16_t Attack_Flags;            /* enum ATK_FLAGS */
//     /* 22 */  int16_t  Sound;


//  ; (sizeof=0x6E)
// drake178: struct s_BU_REC
#pragma pack(push)
#pragma pack(2)
struct s_STRATEGIC_UNIT
{
    /* 0x00 */  int8_t melee;
    /* 0x01 */  int8_t ranged;
    /* 0x02 */  int8_t ranged_type;
    /* 0x03 */  int8_t ammo;
    /* 0x04 */  int8_t tohit;
    /* 0x05 */  int8_t defense;
    /* 0x06 */  int8_t resist;
    /* 0x07 */  int8_t movement_points;
    /* 0x08 */  int16_t cost;
    /* 0x0A */  int8_t upkeep;
    /* 0x0B */  int8_t race;                                // ; enum Race_Code
    /* 0x0C */  int8_t Unused_0Ch;      // reqd_bldg_1 & hero_portrait
    /* 0x0D */  int8_t Cur_Figures;     // reqd_bldg_2 & hero_type
    /* 0x0E */  uint64_t Fig_IMG_Index;  // SAMB_ptr pict_seg
    /* 0x10 */  int8_t hits;
    /* 0x11 */  int8_t scout_range;
    /* 0x12 */  int8_t carry_capacity;
    /* 0x13 */  int8_t Max_Figures;
    /* 0x14 */  int8_t Construction;
    /* 0x15 */  int8_t Spec_Att_Attrib;
    /* 0x16 */  uint16_t Move_Flags;                          // ; enum MOVEFLAGS
    /* 0x17 */  // int8_t Unused_17h;  // ¿ 2-byte alignment padding ? CLUE: here is odd and next one is 2-byte value
    /* 0x18 */  int16_t Attribs_1;                          // ; enum ATTRIB_1
    /* 0x1A */  int8_t Attribs_2;
    /* 0x1B */  int8_t Unused_1Bh;  // ¿ 2-byte alignment padding ? CLUE: here is odd and next one is 2-byte value
    /* 0x1C */  uint16_t Abilities;                         // ; enum ABL_FLAGS
    /* 0x1E */  uint16_t Attack_Flags;                      // ; enum ATK_FLAGS
    /* 0x20 */  int16_t Sound;

    /* 0x22 */  int16_t Combat_Effects;                     // ; enum Combat_Effect
    /* 0x24 */  int8_t Melee_To_Hit;
    /* 0x25 */  int8_t Ranged_To_Hit;
    /* 0x26 */  int8_t To_Block;
    /* 0x27 */  int8_t Weapon_Plus1;
    /* 0x28 */  int16_t Melee_ATK_Flags;                    // ; enum ATK_FLAGS
    /* 0x2A */  int16_t Ranged_ATK_Flags;                   // ; enum ATK_FLAGS
    // TODO  /* 0x2C */  int16_t Item_UEs_L;                         // ; enum UE_FLAGS_L
    // TODO  /* 0x2E */  int16_t Item_UEs_H;                         // ; enum UE_FLAGS_H
    /* 0x2C */  uint32_t Item_UEs;                         // ; enum UE_FLAGS
    /* 0x30 */  int16_t unit_idx;
    /* 0x32 */  int8_t Extra_Hits;
    /* 0x33 */  int8_t Web_HP;
    /* 0x34 */  int8_t Status;
    /* 0x35 */  int8_t owner_idx;
    /* 0x36 */  int8_t Regular_Dmg;
    /* 0x37 */  int8_t Undeath_Dmg;
    /* 0x38 */  int8_t Irreversible_Dmg;
    /* 0x39 */  int8_t TopFig_Dmg;
    // TODO  /* 0x3A */  int16_t Unit_Enchants_L;                    // ; enum UE_FLAGS_L
    // TODO  /* 0x3C */  int16_t Unit_Enchants_H;                    // ; enum UE_FLAGS_H
    /* 0x3A */  uint32_t Unit_Enchants;                    // ; enum UE_FLAGS
    /* 0x3E */  int8_t Suppression;
    /* 0x3F */  int8_t mana_max;
    /* 0x40 */  int8_t mana;
    /* 0x41 */  int8_t Item_Charges;
    /* 0x42 */  int8_t Poison_Strength;
    /* 0x43 */  int8_t Target_BU;
    /* 0x44 */  int16_t X_Pos;
    /* 0x46 */  int16_t Y_Pos;
    /* 0x48 */  int16_t Target_X;
    /* 0x4A */  int16_t Target_Y;
    /* 0x4C */  int16_t MoveStage;
    /* 0x4E */  int16_t Atk_FigLoss;
    /* 0x50 */  int16_t UE_Color;
    /* 0x52 */  int16_t Moving;
    /* 0x54 */  int16_t Action;
    /* 0x56 */  int8_t Confusion_State;
    /* 0x57 */  int8_t Unknown_57h;  // ¿ 2-byte alignment padding ? CLUE: here is odd and next one is 2-byte value
    /* 0x58 */  int16_t Blood_Amount;
    /* 0x5A */  int16_t Unknown_5Ah;
    /* 0x5C */  int16_t Always_Animate;
    /* 0x5E */  int16_t Melee_Anim;
    /* 0x60 */  int16_t Image_Effect;
    /* 0x62 */  int16_t Move_Bob;
    /* 0x64 */  int8_t Gold_Melee;
    /* 0x65 */  int8_t Gold_Ranged;
    /* 0x66 */  int8_t Gold_Defense;
    /* 0x67 */  int8_t Gold_Resist;
    /* 0x68 */  int8_t Gold_Hits;
    /* 0x69 */  int8_t Grey_Melee;
    /* 0x6A */  int8_t Grey_Ranged;
    /* 0x6B */  int8_t Grey_Defense;
    /* 0x6C */  int8_t Grey_Resist;
    /* 0x6D */  int8_t Unused_Grey_Hits;  // ¿ 2-byte alignment padding ?
    /* 0x6E */
};
#pragma pack(pop)


struct s_CMBT_DATA
{
    /* 0x00 */  int8_t melee;
    /* 0x01 */  int8_t ranged;
    /* 0x02 */  int8_t ranged_type;
    /* 0x03 */  int8_t ammo;
    /* 0x04 */  int8_t tohit;
    /* 0x05 */  int8_t defense;
    /* 0x06 */  int8_t resist;
    /* 0x07 */  int8_t movement_points;
    /* 0x08 */  int8_t cost;
    /* 0x0A */  int8_t upkeep;
    /* 0x0B */  int8_t race;                                // ; enum Race_Code
    /* 0x0C */  int8_t Unused_0Ch;
    /* 0x0D */  int8_t Cur_Figures;
    /* 0x0E */  int16_t Fig_IMG_Index;
    /* 0x10 */  int8_t hits;
    /* 0x11 */  int8_t scout_range;
    /* 0x12 */  int8_t carry_capacity;
    /* 0x13 */  int8_t Max_Figures;
    /* 0x14 */  int8_t Construction;
    /* 0x15 */  int8_t Spec_Att_Attrib;
    /* 0x16 */  int8_t Move_Flags;                          // ; enum MOVEFLAGS
    /* 0x17 */  int8_t Unused_17h;  // ¿ 2-byte alignment padding ? CLUE: here is odd and next one is 2-byte value
    /* 0x18 */  int16_t Attribs_1;                          // ; enum ATTRIB_1
    /* 0x1A */  int8_t Attribs_2;
    /* 0x1B */  int8_t Unused_1Bh;  // ¿ 2-byte alignment padding ? CLUE: here is odd and next one is 2-byte value
    /* 0x1C */  int16_t Abilities;                          // ; enum ABL_FLAGS
    /* 0x1E */  int16_t Attack_Flags;                       // ; enum ATK_FLAGS
    /* 0x20 */  int16_t Sound;

};


// sizeof: 04h  4d
struct s_TOWER
{
    /* 00 */ int8_t wx;
    /* 01 */ int8_t wy;
    /* 02 */ int8_t owner_idx;
    /* 03 */ // 2-byte alignment padding
    /* 04 */
};


// sizeof: 04h  4d
struct s_FORTRESS
{
    /* 00 */ int8_t wx;
    /* 01 */ int8_t wy;
    /* 02 */ int8_t wp;
    /* 03 */ int8_t active;
    /* 04 */
};


// sizeof: 04h  4d
struct s_STACK
{
    /* 00 */ int16_t unit_idx;
    /* 02 */ int16_t active;  // AKA Can_Move
    /* 04 */
};


// drake178: struc STD_EVENT
// sizeof:  6h  6d
struct STD_EVENT
{
    /* 0x00 */ int16_t Status;
    /* 0x02 */ int16_t Player;
    /* 0x04 */ int16_t Data;
    /* 0x06 */
};

// drake178: struc REV_EVENT
// sizeof:  6h  6d
struct REV_EVENT
{
    /* 0x00 */ int16_t Status;
    /* 0x02 */ int16_t Data;
    /* 0x04 */ int16_t Player;
    /* 0x06 */
};

// drake178: struc CON_EVENT
// sizeof:  4h  4d
struct CON_EVENT
{
    /* 0x00 */ int16_t Status;
    /* 0x02 */ int16_t Duration;
    /* 0x04 */
};

// drake178: ¿ ?
// sizeof:  64h  100d
struct s_EVENT_DATA
{
    /* 0x00 */  int16_t Last_Event_Turn;
    /* 0x02 */  struct STD_EVENT Meteor;
    /* 0x08 */  struct STD_EVENT Gift;
    /* 0x0E */  int16_t Disjunction;
    /* 0x10 */  struct STD_EVENT Dpl_Marriage;
    /* 0x16 */  int16_t Unused_16h;
    /* 0x18 */  struct STD_EVENT Earthquake;
    /* 0x1E */  struct STD_EVENT Pirates;
    /* 0x24 */  struct STD_EVENT Plague;
    /* 0x2A */  int16_t Plague_Duration;
    /* 0x2C */  struct STD_EVENT Rebellion;
    /* 0x32 */  struct STD_EVENT Donation;
    /* 0x38 */  struct STD_EVENT Depletion;
    /* 0x3E */  struct REV_EVENT New_Mine;
    /* 0x44 */  struct REV_EVENT Pop_Boom;
    /* 0x4A */  int16_t Pop_Boom_Duration;
    /* 0x4C */  struct CON_EVENT Good_Moon;
    /* 0x50 */  struct CON_EVENT Bad_Moon;
    /* 0x54 */  struct CON_EVENT Conjunction_Chaos;
    /* 0x58 */  struct CON_EVENT Conjunction_Nature;
    /* 0x5C */  struct CON_EVENT Conjunction_Sorcery;
    /* 0x60 */  struct CON_EVENT Mana_Short;
    /* 0x64 */
};


// records from HLPENTRY.LBX
// max 50 * 10 loaded into _help_entries
// sizeof 0Ah 10d
struct s_HLP_ENTRY
{
    int16_t help_idx;  // enum HELP_TXT
    int16_t x1;
    int16_t y1;
    int16_t x2;
    int16_t y2;
};

// sizeof 0418h 1048d
struct s_HLP_DATA
{
    char title[30];
    char lbx_name[14];
    int16_t lbx_entry;
    int16_t Include_Entry;
    char help_text[1000];
};


// drake178: USW_List

struct s_UV_List
{
 /* 00 */ char names[40][30];
 /* 1E */ int32_t flags[40];
 /* 22 */ SAMB_INT picts[40];  // ¿ ALWAYS pointer to picture from SPECIALS.LBX/SPECIALS2.LBX ?
 /* 24 */ int16_t helps[40];  // enum HELP_TXT
 /* 26 */
};



/*
    Messages
*/

// drake178: Building_Done_Msg_Item
// sizeof:  04h  4d
#pragma pack(push)
#pragma pack(2)
struct s_MSG_BUILDING_COMPLETE
{
    /* 00 */ int8_t city_idx;
    /* 01 */ // 2-byte alignment padding
    /* 02 */ int16_t bldg_type_idx;
    /* 04 */
};
#pragma pack(pop)

// drake178: Unit_Lost_Msg_Item
// sizeof:  04h  4d
#pragma pack(push)
#pragma pack(2)
struct s_MSG_UNIT_LOST
{
    /* 00 */ int16_t Unit_Type;
    /* 02 */ int8_t Cause;
    /* 03 */ // 2-byte padding?  Unused_3h db ?
    /* 04 */
};
#pragma pack(pop)







// WZD dseg:00AA                                                 BEGIN: Data Segment - Initialized Data

// ¿ MoO2: ~ remap_picture_colors ?
// WZD dseg:00AA 69 6A 6B 6C 6D                                  COL_Banners_P0 db 105, 106, 107, 108, 109
// WZD dseg:00AF 49 4A 4B 4C 4D                                  COL_Banners_P1 db 73, 74, 75, 76, 77
// WZD dseg:00B4 79 7A 7B 7C 7D                                  COL_Banners_P2 db 121, 122, 123, 124, 125
// WZD dseg:00B9 C9 A5 CB A6 2D                                  COL_Banners_P3 db 201, 165, 203, 166, 45
// WZD dseg:00BE A0 A1 A2 B2 B4                                  COL_Banners_P4 db 160, 161, 162, 178, 180
// WZD dseg:00C3 30 31 32 33 34                                  COL_Banners_P5 db 48, 49, 50, 51, 52
extern uint8_t COL_Banners[];

// WZD dseg:00C8 62 D8 7B 2B B3 32                               COL_Cartographer db 98, 216, 123, 43, 179, 50
// WZD dseg:00CE C9 00 A5 00 CB 00                               UU_COL_Setof3_1 db 201,  0,165,  0,203,  0
// WZD dseg:00D4 79 00 7A 00 7B 00                               UU_COL_Setof3_2 db 121,  0,122,  0,123,  0
// WZD dseg:00DA 0D 00 0E 00 0F 00                               UU_COL_Setof3_3 db  13,  0, 14,  0, 15,  0
// WZD dseg:00E0 49 00 4A 00 4B 00                               UU_COL_Setof3_4 db  73,  0, 74,  0, 75,  0
// WZD dseg:00E6 69 00 6A 00 6B 00                               UU_COL_Setof3_5 db 105,  0,106,  0,107,  0
// WZD dseg:00EC 00                                              db    0
// WZD dseg:00ED 00                                              db    0


// WZD dseg:00EE 01 00 00 00 00 00 00 00 3F 01 C7 00             mouse_list_default s_MOUSE_LIST <crsr_Finger, 0, 0, 0, 319, 199>
// extern struct s_mouse_list mouse_list_default[1];
// WZD dseg:00FA 00 00 00 00 00 00 00 00 3F 01 C7 00             mouse_list_none s_MOUSE_LIST <0, 0, 0, 0, 319, 199>
// extern struct s_mouse_list mouse_list_none[1];
// WZD dseg:0106 06 00 00 00 00 00 00 00 3F 01 C7 00             mouse_list_hourglass s_MOUSE_LIST <crsr_Hourglass, 0, 0, 0, 319, 199>
extern struct s_mouse_list mouse_list_hourglass[1];
// WZD dseg:0112 01 00 00 00 00 00 00 00 3F 01 C7 00             NIU_mouse_list_normal s_MOUSE_LIST <crsr_Finger, 0, 0, 0, 319, 199>
// WZD dseg:011E 07 00 04 00 00 00 00 00 3F 01 9E 00             NIU_mouse_list_boot s_MOUSE_LIST <crsr_WingedBoot, 4, 0, 0, 319, 158> ; ? 158 is main map width ?

// WZD dseg:012A 01 02 04 08 10 20                               byte_36BCA db   1,  2,  4,  8, 16, 32
// WZD dseg:0130 82 20 8A 20 92 20 A0 20 AF 20 BF 20 CE 20 DB 20+wizard_abilities_names dw offset cnst_Alchemy, offset cnst_Warlord, offset cnst_ChaosMastery, offset cnst_NatureMastery, offset cnst_SorceryMastery, offset cnst_InfernalPower, offset cnst_DivinePower, offset cnst_SageMaster, offset cnst_Channeler, offset cnst_Myrran, offset cnst_Archmage
// WZD dseg:0130 E7 20 F1 20 F8 20 01 21 0F 21 1C 21 23 21 2E 21+                                        ; DATA XREF: Mirror_Screen_Draw+61Er ...
// WZD dseg:0130 37 21 43 21                                     dw offset cnst_ManaFocusing, offset cnst_NodeMastery, offset cnst_Famous, offset cnst_Runemaster, offset cnst_Conjurer, offset cnst_Charismatic, offset cnst_Artificer ; "Alchemy"
// WZD dseg:0154 01 00                                           EVNT_Enabled dw 1                       ; DATA XREF: EVNT_GenerateRandom:loc_6AD68r

// WZD dseg:0156
// drake178: STR_TownSizes
extern char * STR_TownSizes[6];





// WZD dseg:1D74
// drake178: struct XP_TABLE TBL_Experience
extern int16_t TBL_Experience[9];
// 00 struc XP_TABLE ; (sizeof=0x12, standard type)
// 00 Hero_or_Recruit dw ?                    ; base 10
// 02 Myrmidon_or_Regular dw ?                ; base 10
// 04 Captain_or_Veteran dw ?                 ; base 10
// 06 Cmdr_or_Elite dw ?                      ; base 10
// 08 Champion dw ?                           ; base 10
// 0A Lord_or_Ultra_Elite dw ?                ; base 10
// 0C Grand_Lord dw ?                         ; base 10
// 0E Super_Hero dw ?                         ; base 10
// 10 Demi_God dw ?                           ; base 10
// 12 ends XP_TABLE





// WZD dseg:1EE4
extern char * STR_CityEnchants[26];



// WZD dseg:1F18
// drake178: TBL_UE_Upkeep
extern int16_t unit_enchantment_upkeep_table[32];

// WZD dseg:1F58
// drake178: TBL_CE_Upkeep
extern int16_t city_enchantment_upkeep_table[26];

// WZD dseg:1F8C
// drake178: TBL_GE_Upkeep
extern int16_t overland_enchantment_upkeep_table[24];





// WZD dseg:1FDC 74 28                                           dw offset aExpansionist                 ; "Expansionist"
// WZD dseg:1FDE 28 00 14 00 0A 00 0A 00 D8 FF EC FF             TBL_AI_PRS_War_Mod dw 40, 20, 10, 10, 65496, 65516
// WZD dseg:1FDE                                                                                         ; DATA XREF: AI_OVL_SplCat_Picker+9CEr ...

// WZD dseg:1FEA 
// drake178: TBL_AI_DIFF_Mods 
extern struct s_DIFFICULTY_MODIFIERS difficulty_modifiers_table[NUM_DIFFICULTY_LEVEL];

// WZD dseg:203A 00 00 00 05 00 00 00 00 05 00 00 00 0A 00 00 00+TBL_AI_BLD_OBJWgts AI_BLD_Obj_Priorities <<0, 0, 0, 5, 0>, <0, 0, 0, 5, 0>, <0, 0, 10, 0, 0>, <0, 0, 10, 0, 0>, <0, 10, 0, 0, 2>, <0, 0, 0, 5, 0>, <0, 0, 0, 5, 0>, <0, 5, 0, 0, 5>, <0, 5, 0, 0, 3>, <0, 0, 0, 0, 10>>
// WZD dseg:203A 00 0A 00 00 00 0A 00 00 02 00 00 00 05 00 00 00+                                        ; DATA XREF: AI_CTY_SetProduction+49Fr ...
// WZD dseg:206C 0A 00 0F 00 1E 00 0A 00 0A 00 0F 00 0F 00 0A 00+TBL_AI_BLD_BaseWgts dw 10, 15, 30, 10, 10, 15, 15, 10, 10, 10
// WZD dseg:206C 0A 00 0A 00                                                                             ; DATA XREF: AI_CTY_SetProduction+4C2r ...


// WZD dseg:2080                                                 ¿  BEGIN: meaningful boundary ?
// WZD dseg:2080 00 10                                           __ovrbuffer dw 4096
// ; DATA XREF: __OvrPrepare+19r ...
// ; declared in overlay.lib\OVRBUFF
// WZD dseg:2080                                                 ¿  BEGIN: meaningful boundary ?

// WZD dseg:2082 41 6C 63 68 65 6D 79                            cnst_Alchemy db 'Alchemy'               ; DATA XREF: dseg:wizard_abilities_nameso

// WZD dseg:2143 41 72 74 69 66 69 63 65 72 00                   cnst_Artificer db 'Artificer',0         ; DATA XREF: dseg:wizard_abilities_nameso

// WZD dseg:214D
// drake178: cnst_Outpost
extern char cnst_Outpost[];
// WZD dseg:2155
// drake178: cnst_Hamlet
extern char cnst_Hamlet[];
// WZD dseg:215C
// drake178: cnst_Village
extern char cnst_Village[];
// WZD dseg:2164
// drake178: cnst_Town
extern char cnst_Town[];
// WZD dseg:2169
// drake178: cnst_City
extern char cnst_City[];
// WZD dseg:216E
// drake178: cnst_Capital
extern char cnst_Capital[];

// WZD dseg:2176 53 75 6D 6D 6F 6E 69 6E 67 20 43 69 72 63 6C 65+cnst_SummoningCircle db 'Summoning Circle',0

// WZD dseg:2187
extern char cnst_EarthGate[];
// WZD dseg:2192
extern char cnst_StreamOfLife[];
// WZD dseg:219C
extern char cnst_Rlm3_Life[];
// WZD dseg:21A1
extern char cnst_AstralGate[];
// WZD dseg:21AD
extern char cnst_Fortress[];
// WZD dseg:21B6
extern char cnst_DarkRituals[];
// WZD dseg:21C3
extern char cnst_AltarofBattle[];



// WZD dseg:2700
extern char cnst_WallofFire[];
// WZD dseg:270D
extern char cnst_ChaosRift[];
// WZD dseg:2718
extern char cnst_EvilPresence[];
// WZD dseg:2726
extern char cnst_CursedLands[];
// WZD dseg:2733
extern char cnst_Pestilence[];
// WZD dseg:273E
extern char cnst_CloudofShadow[];
// WZD dseg:274E
extern char cnst_Famine[];
// WZD dseg:2755
extern char cnst_FlyingFortress1[];
// WZD dseg:275C
extern char cnst_NatureWard[];
// WZD dseg:2768
extern char cnst_SorceryWard[];
// WZD dseg:2775
extern char cnst_ChaosWard[];
// WZD dseg:2780
extern char cnst_LifeWard[];
// WZD dseg:278A
extern char cnst_DeathWard[];
// WZD dseg:2795
extern char cnst_NaturesEye[];
// WZD dseg:27A2
extern char cnst_StreamofLife[];
// WZD dseg:27B1
extern char cnst_GaiasBlessing[];
// WZD dseg:27C1
extern char cnst_Inspirations[];
// WZD dseg:27CE
extern char cnst_Prosperity[];
// WZD dseg:27D9
extern char cnst_HeavenlyLight[];
// WZD dseg:27E8
extern char cnst_Consecration[];
// WZD dseg:27F5
extern char cnst_WallofDarkness[];
// WZD dseg:2806
extern char cnst_Nightshade[];





// WZD dseg:2F8C
extern char _msg_planar_seal_prevents[];
// WZD dseg:2FC2
extern char _msg_units_cant_planar_travel[];
// WZD dseg:2FFC
extern char _msg_city_cant_produce_1[];
// WZD dseg:2FFF
extern char _msg_city_cant_produce_4[];
// WZD dseg:3001
extern char _msg_city_cant_produce_2[];
// WZD dseg:3006
extern char _msg_city_cant_produce_3[];
// WZD dseg:301E
extern char _msg_city_cant_produce_5[];
// WZD dseg:3020
extern char _msg_guardian_kills_spirit[];
// WZD dseg:305B 00                                              align 2





// WZD dseg:33B8
// drake178: MoveFlag_Array
// 01 00 20 00 40 00 04 00 02 00 08 00             
// MoveFlag_Array dw M_Cavalry, M_Forester, M_Mntnr, M_Swimming, M_Sailing, M_Flying
// OON XREF: STK_GetMoveTypes()
extern int16_t MoveType_Flags[6];  // = { 0x0001, 0x0020, 0x0040, 0x0004, 0x0002, 0x0008 }
// struct s_Movement_Modes
// {
//     /* 0x00 */  uint16_t Cavalry = 0x0001;
//     /* 0x02 */  uint16_t Forester = 0x0020;
//     /* 0x04 */  uint16_t Mountaineer = 0x0040;
//     /* 0x06 */  uint16_t Swimming = 0x0004;
//     /* 0x08 */  uint16_t Sailing = 0x0002;
//     /* 0x0A */  uint16_t Flying = 0x0008;
//     /* 0x0C */  uint16_t PlanarTravel;
//     /* 0x0E */
// };

struct s_Movement_Modes
{
    /* 0x00 */  uint16_t Cavalry;
    /* 0x02 */  uint16_t Forester;
    /* 0x04 */  uint16_t Mountaineer;
    /* 0x06 */  uint16_t Swimming;
    /* 0x08 */  uint16_t Sailing;
    /* 0x0A */  uint16_t Flying;
    /* 0x0C */  uint16_t PlanarTravel;
    /* 0x0E */  uint16_t Walking;
};

// struct s_Movement_Modes movement_modes_array = 
// {
//     Cavalry = 0x0001
//     Forester = 0x0020,
//     Mountaineer = 0x0040,
//     Swimming = 0x0004,
//     Sailing = 0x0002,
//     Flying = 0x0008
// };
// struct s_Movement_Modes movement_modes_array = { 0x0001, 0x0020, 0x0040, 0x0004, 0x0002, 0x0008 };
// extern struct s_Movement_Modes movement_modes_array = { CAVALRY, FORESTER, MOUNTAINEER, SWIMMING, SAILING, FLYING };
extern struct s_Movement_Modes movement_modes_array;





// MGC dseg:52C6
// AKA gsa_WIZARDS_0to13
extern SAMB_ptr wizard_portrait_segs[];  // ¿ here, because used by MGC Newgame_Screen(), but, also used by WZD Magic_Screen() ? DSEG in WZD?





// WZD dseg:599C
extern char hlpentry_lbx_file[];



// WZD dseg:6E9E
// drake178: TBL_Tax_Unrest_Pcnts
extern int16_t tax_unrest_pct_table[7];






// WZD dseg:6ED0                                                 ¿ BEGIN: Cityscape - Initialized Data ?

// WZD dseg:6ED0
// drake178: CTY_EnchantAnimStage
extern int16_t cityscape_bldg_anim_ctr;

// WZD dseg:6ED2
extern int16_t cityscape_water_anim_ctr;

// WZD dseg:6ED4
extern int16_t cityscape_wall_anim_ctr;

// WZD dseg:6ED6
extern int16_t IDK_cityscape_vanish_percent;

// WZD dseg:6ED8
// ¿ usage/pattern ? sets of 4?
// IDK_cityscape_RC_4x2
// 4, 9, 13, 18, 0, 5, 9, 14, 0, -12, 18, 0, 5,-20,24,-13,19,-13,24,-7, 0,-14,29, 0, 4,-17,31,-4, 7,-21,35,-6, 0,-15,19, 0, 6,-20,23,-6,10,-25,30,-11, 0,-14,29, 0, 6,-19,34,-5,11,-25,39,-11, 6,-41,30,-3,11,-47,30,-42,25,-41,30,-9

// WZD dseg:6F5E                                                 ¿ END: Cityscape - Initialized Data ?





// WZD dseg:6FFE
extern int16_t Map_LastDraw_X;
// WZD dseg:7000
extern int16_t Map_LastDraw_Y;
// WZD dseg:7002
extern int16_t OVL_NewMapSustain;



// dseg:76DA
extern int16_t screen_window_x1;
// dseg:76DC
extern int16_t screen_window_y1;
// dseg:76DE
extern int16_t screen_window_x2;
// dseg:76E0
extern int16_t screen_window_y2;





// WZD dseg:7846 68 35 68 35                                     random_seed dd 35683568h                ; DATA XREF: Set_Random_Seed+6w ...

// MoX_Util  // WZD dseg:784A 01 00 02 00 04 00 08 00 10 00 20 00 40 00 80 00 bit_field_test_bits dw 1, 10b, 100b, 1000b, 10000b, 100000b, 1000000b, 10000000b
// MoX_Util  extern uint16_t bit_field_test_bits[];

// WZD dseg:785A 64 00                                           UU_DBG_OptionBoxColor dw 64h            ; DATA XREF: UU_DBG_SetSelectSetting+Cw ...
// WZD dseg:785C 32 00                                           UU_DBG_UnknownOValue dw 32h             ; DATA XREF: UU_DBG_SetSelectSetting+12w
// WZD dseg:785E 00 00                                           UU_DBG_OptionsFontColor dw 0            ; DATA XREF: UU_DBG_SetSelectSetting+18w ...
// WZD dseg:7860 00 00                                           UU_DBG_OptionsFont dw 0                 ; DATA XREF: UU_DBG_SetSelectSetting+6w ...
// WZD dseg:7862 52 4E 44 20 6E 6F 20 30 27 73                   cnst_RND_Error db 'RND no 0',27h,'s'    ; DATA XREF: Random+Eo
// WZD dseg:786C 00                                              cnst_ZeroString_2 db 0                  ; DATA XREF: UU_DBG_SelectDialog+138o ...
// WZD dseg:786D 4E 4F 5F 48 45 4C 50 00                         UU_cnst_NoHelp db 'NO_HELP',0           ; DATA XREF: UU_DBG_SelectDialog+134o ...
// WZD dseg:7875 00                                              db    0
// WZD dseg:7876 00 00                                           release_version dw 0                    ; DATA XREF: DBG_Quit:loc_1E469r ...
// WZD dseg:7878 53 43 52 44 4D 50 30 30                         cnst_Scrdmp00_Full db 'SCRDMP00'        ; DATA XREF: DBG_ScreenDump+24o
// WZD dseg:7880 2E 46 4C 49 00                                  cnst_Scrdmp_Ext db '.FLI',0             ; DATA XREF: DBG_ScreenDump+AAo
// WZD dseg:7885 30 00                                           cnst_Scrdmp_0 db '0',0                  ; DATA XREF: DBG_ScreenDump+3Eo
// WZD dseg:7887 53 43 52 44 4D 50 00                            cnst_Scrdmp_Base db 'SCRDMP',0          ; DATA XREF: DBG_ScreenDump:loc_1E504o
// WZD dseg:788E 77 62 00                                        cnst_WB7 db 'wb',0                      ; DATA XREF: DBG_ScreenDump:loc_1E54Do





// WZD dseg:8296                                                 ¿ BEGIN: Help - Initialized Data ?

// WZD dseg:8296
// AKA have_help
extern int16_t help_list_active;

// WZD dseg:8298
// AKA _help_count
extern int16_t help_list_count;

// WZD dseg:8298                                                 ¿ END: Help - Initialized Data ?






// WZD dseg:888A 01 01 93 01 43 32                               ExitStart _start_exit_proc <1, 1, __OVREXIT>

// WZD dseg:8890                                                 ¿ BEGIN: ?

// WZD dseg:8890 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+TBL_Arcanus CONT_TBL <0>                ; DATA XREF: start+C7o ...
// WZD dseg:8890 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; _bdata_start
// WZD dseg:8C14 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+TBL_Myrror CONT_TBL <0>
// WZD dseg:8F98 00 00                                           AI_SCircle_Reevals@ dw 0                ; DATA XREF: AI_Overland_Turn+136r ...
// WZD dseg:8F98                                                                                         ; 16 bytes
// WZD dseg:8F98                                                                                         ; 80 + rnd(40) is stored here for each AI wizard (word
// WZD dseg:8F98                                                                                         ; array pointer, human excluded) when setting initial
// WZD dseg:8F98                                                                                         ; gold during game creation in MAGIC.EXE
// WZD dseg:8F98                                                                                         ; decreased during the AI's turn, likely some relation
// WZD dseg:8F98                                                                                         ; or interest value
// WZD dseg:8F9A 00 00                                           AI_Arc_NewColTgtYs@ dw 0                ; DATA XREF: MoM_Init_Tables+3F6w ...
// WZD dseg:8F9A                                                                                         ; 12 bytes
// WZD dseg:8F9C 00 00                                           AI_Myr_NewColTgtYs@ dw 0                ; DATA XREF: MoM_Init_Tables+3FCw
// WZD dseg:8F9C                                                                                         ; 12 bytes
// WZD dseg:8F9E 00 00                                           AI_Arc_NewColTgtXs@ dw 0                ; DATA XREF: MoM_Init_Tables+3EAw ...
// WZD dseg:8F9E                                                                                         ; 20 bytes
// WZD dseg:8FA0 00 00                                           AI_Myr_NewColTgtXs@ dw 0                ; DATA XREF: MoM_Init_Tables+3F0w
// WZD dseg:8FA0                                                                                         ; 20 bytes
// WZD dseg:8FA2 00 00                                           Wiz5_Spell_50h@ dw 0                    ; DATA XREF: MoM_Init_Tables+3D2w
// WZD dseg:8FA2                                                                                         ; 20 bytes
// WZD dseg:8FA4 00 00                                           Wiz5_Spell_64h@ dw 0                    ; DATA XREF: MoM_Init_Tables+3D8w
// WZD dseg:8FA4                                                                                         ; 20 bytes
// WZD dseg:8FA6 00 00                                           Wiz5_Spell_28h@ dw 0                    ; DATA XREF: MoM_Init_Tables+3C6w
// WZD dseg:8FA6                                                                                         ; 20 bytes
// WZD dseg:8FA8 00 00                                           Wiz5_Spell_3Ch@ dw 0                    ; DATA XREF: MoM_Init_Tables+3CCw
// WZD dseg:8FA8                                                                                         ; 20 bytes
// WZD dseg:8FAA 00 00                                           AI_CONTX_Reevals@ dw 0                  ; DATA XREF: Loaded_Game_Update_WZD+B4r ...
// WZD dseg:8FAA                                                                                         ; 16 bytes
// WZD dseg:8FAC 00 00                                           AI_Arc_NewColConts@ dw 0                ; DATA XREF: MoM_Init_Tables+3DEw ...
// WZD dseg:8FAC                                                                                         ; 20 bytes
// WZD dseg:8FAE 00 00                                           AI_Myr_NewColConts@ dw 0                ; DATA XREF: MoM_Init_Tables+3E4w
// WZD dseg:8FAE                                                                                         ; 20 bytes
// WZD dseg:8FB0 00 00                                           AI_Arc_MainWarConts@ dw 0               ; DATA XREF: MoM_Init_Tables+3B4w ...
// WZD dseg:8FB0                                                                                         ; 12 bytes, Arcanus array
// WZD dseg:8FB2 00 00                                           AI_Myr_MainWarConts@ dw 0               ; DATA XREF: MoM_Init_Tables+3BAw
// WZD dseg:8FB2                                                                                         ; 12 bytes, Myrror array
// WZD dseg:8FB4 00                                              unk_3FA54 db    0                       ; DATA XREF: sub_F6CAB+103o
// WZD dseg:8FB5 00                                              db    0
// WZD dseg:8FB6 00                                              db    0
// WZD dseg:8FB7 00                                              db    0
// WZD dseg:8FB8 00                                              db    0
// WZD dseg:8FB9 00                                              db    0
// WZD dseg:8FBA 00                                              db    0
// WZD dseg:8FBB 00                                              db    0
// WZD dseg:8FBC 00                                              db    0
// WZD dseg:8FBD 00                                              db    0
// WZD dseg:8FBE 00                                              db    0
// WZD dseg:8FBF 00                                              db    0
// WZD dseg:8FC0 00                                              db    0
// WZD dseg:8FC1 00                                              db    0
// WZD dseg:8FC2 00                                              db    0
// WZD dseg:8FC3 00                                              db    0
// WZD dseg:8FC4 00 00                                           Some_AI_Turn_Var_2 dw 0                 ; DATA XREF: AI_Overland_Turn+212w
// WZD dseg:8FC4                                                                                         ; now 0
// WZD dseg:8FC6 00 00                                           Some_AI_Turn_Var_3 dw 0                 ; DATA XREF: AI_Overland_Turn+218w
// WZD dseg:8FC6                                                                                         ; now 0
// WZD dseg:8FC8 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+AI_Target_Values dw 19h dup(     0)     ; DATA XREF: AI_AddTarget+3Aw ...
// WZD dseg:8FFA 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+AI_Target_Strengths dw 19h dup(     0)  ; DATA XREF: AI_AddTarget+2Dw ...
// WZD dseg:902C 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+AI_Target_Ys dw 19h dup(     0)         ; DATA XREF: AI_AddTarget+20w ...
// WZD dseg:905E 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+AI_Target_Xs dw 19h dup(     0)         ; DATA XREF: AI_AddTarget+13w ...
// WZD dseg:9090 00                                              db    0
// WZD dseg:9091 00                                              db    0
// WZD dseg:9092 00 00                                           AI_Target_Count dw 0                    ; DATA XREF: AI_CreateTargetList+8w ...
// WZD dseg:9094 00 00                                           AI_Transport_Count dw 0                 ; DATA XREF: AI_OVL_Eval_Assets+1262w ...
// WZD dseg:9096 00 00                                           AI_SeekTransport_Cnt dw 0               ; DATA XREF: AI_SetUnitOrders:loc_EBDECw ...
// WZD dseg:9098 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+AI_SeekTransport_Ps dw 0Fh dup(0)       ; DATA XREF: G_AI_ProcessTransports+B4r ...
// WZD dseg:90B6 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+AI_SeekTransport_Ys dw 0Fh dup(0)       ; DATA XREF: G_AI_ProcessTransports+AAr ...
// WZD dseg:90D4 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+AI_SeekTransport_Xs dw 0Fh dup(0)       ; DATA XREF: G_AI_ProcessTransports+8Er ...
// WZD dseg:90F2 00 00                                           AI_Arc_ContBalances@ dw 0               ; DATA XREF: AI_Continent_Eval+4C5r ...
// WZD dseg:90F2                                                                                         ; 120 LBX_NearAlloc_Next bytes
// WZD dseg:90F2                                                                                         ; own str - enemy str - own value
// WZD dseg:90F4 00 00                                           AI_Myr_ContBalances@ dw 0               ; DATA XREF: AI_Turn_NearAllocs+DCw
// WZD dseg:90F4                                                                                         ; 120 LBX_NearAlloc_Next bytes
// WZD dseg:90F4                                                                                         ; own str - enemy str - own value
// WZD dseg:90F6 00 00                                           AI_Arc_Cont_Nme_Val@ dw 0               ; DATA XREF: AI_Continent_Eval+64r ...
// WZD dseg:90F6                                                                                         ; 120 LBX_NearAlloc_Next bytes
// WZD dseg:90F8 00 00                                           AI_Myr_Cont_Nme_Val@ dw 0               ; DATA XREF: AI_Continent_Eval+72r ...
// WZD dseg:90F8                                                                                         ; 120 LBX_NearAlloc_Next bytes
// WZD dseg:90FA 00 00                                           AI_Arc_Cont_Own_Val@ dw 0               ; DATA XREF: AI_Continent_Eval+48r ...
// WZD dseg:90FA                                                                                         ; 120 LBX_NearAlloc_Next bytes
// WZD dseg:90FC 00 00                                           AI_Myr_Cont_Own_Val@ dw 0               ; DATA XREF: AI_Continent_Eval+56r ...
// WZD dseg:90FC                                                                                         ; 120 LBX_NearAlloc_Next bytes
// WZD dseg:90FE 00 00                                           CRP_AI_Arc_Cont_Nme_Str@ dw 0           ; DATA XREF: AI_Continent_Eval+2Cr ...
// WZD dseg:90FE                                                                                         ; redundant, should not be the primary pointer
// WZD dseg:9100 00 00                                           CRP_AI_Myr_Cont_Nme_Str@ dw 0           ; DATA XREF: AI_Continent_Eval+3Ar ...
// WZD dseg:9100                                                                                         ; redundant, should not be the primary pointer
// WZD dseg:9102 00 00                                           AI_Arc_Cont_Own_Str@ dw 0               ; DATA XREF: AI_Continent_Eval+10r ...
// WZD dseg:9102                                                                                         ; 120 LBX_NearAlloc_Next bytes
// WZD dseg:9104 00 00                                           AI_Myr_Cont_Own_Str@ dw 0               ; DATA XREF: AI_Continent_Eval+1Er ...
// WZD dseg:9104                                                                                         ; 120 LBX_NearAlloc_Next bytes
// WZD dseg:9106 00 00                                           AI_Enemy_Stacks@ dw 0                   ; DATA XREF: AI_Continent_Eval+121r ...
// WZD dseg:9106                                                                                         ; 1440 LBX_NearAlloc_Next bytes
// WZD dseg:9108 00 00                                           AI_Own_Stacks@ dw 0                     ; DATA XREF: AI_Continent_Eval+8Er ...
// WZD dseg:9108                                                                                         ; 800 LBX_NearAlloc_Next bytes
// WZD dseg:910A 00 00                                           AI_Enemy_Stack_Count dw 0               ; DATA XREF: AI_Continent_Eval:loc_D2593r ...
// WZD dseg:910C 00 00                                           AI_Own_Stack_Count dw 0                 ; DATA XREF: AI_Continent_Eval:loc_D24FFr ...
// WZD dseg:910E 00 00 00 00 00 00 00 00 00 00 00 00             AI_AtWarWith_Players dw 6 dup(0)        ; DATA XREF: AI_Count_Active_Wars+43w ...
// WZD dseg:911A 00 00                                           AI_Active_War_Count dw 0                ; DATA XREF: AI_Count_Active_Wars+7w ...
// WZD dseg:911C 00 00                                           CRP_AI_OVL_SpellList dw 0               ; DATA XREF: AI_OVL_GetSpellList+DBr ...
// WZD dseg:911C                                                                                         ; 50 LBX_NearAlloc_Next bytes
// WZD dseg:911C                                                                                         ; redundant - filled out but never used
// WZD dseg:911E 00 00                                           CRP_AI_OVL_SpellCount dw 0              ; DATA XREF: AI_OVL_GetSpellList+8w ...
// WZD dseg:911E                                                                                         ; redundant - calculated but never used
// WZD dseg:9120 00 00                                           AI_OVL_Spell_Cats dw 0                  ; DATA XREF: AI_OVL_SplCat_Picker:loc_E4471r ...
// WZD dseg:9120                                                                                         ; 92 LBX_NearAlloc_Next bytes
// WZD dseg:9122 00 00                                           AI_NME_Garrison_Strs dw 0               ; DATA XREF: AI_OVL_Eval_Assets+15r ...
// WZD dseg:9122                                                                                         ; 200 LBX_NearAlloc_Next bytes
// WZD dseg:9124 00 00                                           AI_Own_Garr_Strs dw 0                   ; DATA XREF: AI_OVL_Eval_Assets+24r ...
// WZD dseg:9124                                                                                         ; 200 LBX_NearAlloc_Next bytes
// WZD dseg:9126 00 00                                           AI_Enemy_City_Values dw 0               ; DATA XREF: AI_Continent_Eval+36Fr ...
// WZD dseg:9126                                                                                         ; 200 LBX_NearAlloc_Next bytes
// WZD dseg:9128 00 00                                           AI_Own_City_Values dw 0                 ; DATA XREF: AI_Continent_Eval+351r ...
// WZD dseg:9128                                                                                         ; 200 LBX_NearAlloc_Next bytes

// WZD dseg:912A                                                 ¿ BEGIN: ?

// WZD dseg:912A
// index of the Time Stop wizard plus 1 (0 if none)
extern int16_t g_TimeStop_PlayerNum;

// WZD dseg:912C
extern struct s_SPELL_DATA * spell_data_table;

// WZD dseg:9130                                                 ; unsigned int IMG_GAME_SpellAnim
// WZD dseg:9130 00 00                                           IMG_GAME_SpellAnim@ dw 0                ; DATA XREF: GAME_LearnSpellAnim+374w ...
// WZD dseg:9130                                                                                         ; appended into the LBX_Sandbox_Segment
// WZD dseg:9132 00 00                                           SND_SpellCast@ dw 0                     ; DATA XREF: GAME_LoadSpellSound+7Cw ...
// WZD dseg:9132                                                                                         ; appended into World_Data@ during combat, or the
// WZD dseg:9132                                                                                         ; Sandbox overland (-1 if none or SFX are disabled)
// WZD dseg:9134 00 00                                           SND_Spell_Music@ dw 0                   ; DATA XREF: GAME_LearnSpellAnim+39w ...

// WZD dseg:9136                                                 ? BEGIN:  - Uninitialized Data ?



// in MGC, screen_change for Main_Menu_Screen() is the next one up fro TBL_Items
// but there is nothing of the sort in WZD

// WZD dseg:9136
extern struct s_ITEM * TBL_Items;

// WZD dseg:913A
extern SAMB_ptr TBL_Premade_Items;

// WZD dseg:913E
// drake178: IMG_USW_Items ITEM_ICONS
extern SAMB_ptr item_icons_seg[116];

// WZD dseg:9226
extern struct s_STRATEGIC_UNIT * global_strategic_unit;                // alloc in Allocate_Data_Space()

// WZD dseg:922A
// TBL_BattleUnits
// WZD dseg:922E
// TBL_CombatEnchants


// extern struct s_HERO p_heroes[][];
// extern struct s_HERO ** p_heroes;
// extern struct s_HERO p_heroes[6][35];
extern struct s_HERO * p_heroes[6];
// WZD dseg:9232
extern SAMB_ptr p0_heroes;                  // alloc in Allocate_Data_Space()
// WZD dseg:9236
extern SAMB_ptr p1_heroes;                  // alloc in Allocate_Data_Space()
// WZD dseg:923A
extern SAMB_ptr p2_heroes;                  // alloc in Allocate_Data_Space()
// WZD dseg:923E
extern SAMB_ptr p3_heroes;                  // alloc in Allocate_Data_Space()
// WZD dseg:9242
extern SAMB_ptr p4_heroes;                  // alloc in Allocate_Data_Space()
// WZD dseg:9246
extern SAMB_ptr p5_heroes;                  // alloc in Allocate_Data_Space()






// WZD dseg:9284
extern int16_t OVL_Action_Type;
// WZD dseg:9286
extern int16_t OVL_Action_Structure;




// WZD dseg:92B0
// drake178: G_TBL_CityscapeBldngs
extern struct s_BLDG * G_TBL_CityscapeBldngs;

// WZD dseg:92B4
extern int16_t IDK_Cityscape_Bldg_Cnt;

// WZD dseg:92B6
// drake178: IMG_USW_GrassBase@
extern SAMB_ptr unit_grass_diamond_seg;

// WZD dseg:92B8
// drake178: IMG_CTY_Buildings BLDNG_GFX
extern SAMB_ptr bldg_picts_seg[35];

// WZD dseg:92FE
// drake178: IMG_CTY_CITYSPL4
extern SAMB_ptr IDK_CITYSPL4_city_walls_build_seg;

// WZD dseg:9300
// drake178: IMG_CTY_TradeGoods@
extern SAMB_ptr building_trade_goods_seg;

// WZD dseg:9302
// drake178: IMG_CTY_Housing_Std@, IMG_CTY_Housing_Tre@, IMG_CTY_Housing_Mnd@
extern SAMB_ptr bldg_housing_seg[3];

// WZD dseg:9308 00 00                                           IMG_CTY_WizFortress@ dw 0               ; DATA XREF: GFX_Swap_AppndCtScap+4F4w ...
// WZD dseg:930A 00 00                                           IMG_CTY_Scap_Wall@ dw 0                 ; DATA XREF: GFX_Swap_AppndCtScap+4DDw ...
// WZD dseg:930C 00 00                                           IMG_CTY_AltarofBtl@ dw 0                ; DATA XREF: GFX_Swap_AppndCtScap+498w ...
// WZD dseg:930E 00 00                                           IMG_CTY_EvilPresnc@ dw 0                ; DATA XREF: GFX_Swap_AppndCtScap+4C6w ...
// WZD dseg:9310 00 00                                           IMG_CTY_Dark_Ritual@ dw 0               ; DATA XREF: GFX_Swap_AppndCtScap+4AFw ...
// WZD dseg:9312 00 00                                           IMG_CTY_Astral_Gate@ dw 0               ; DATA XREF: GFX_Swap_AppndCtScap+481w ...
// WZD dseg:9314 00 00                                           IMG_CTY_StreamofLif@ dw 0               ; DATA XREF: GFX_Swap_AppndCtScap+46Aw ...
// WZD dseg:9316 00 00                                           IMG_CTY_Earth_Gate@ dw 0                ; DATA XREF: GFX_Swap_AppndCtScap+453w ...
// WZD dseg:9318 00 00                                           IMG_CTY_Sum_Circle@ dw 0                ; DATA XREF: GFX_Swap_AppndCtScap+43Cw ...
// WZD dseg:931A 00 00 00 00 00 00                               IMG_CTY_Scap_Rocks@ dw 3 dup(0)         ; DATA XREF: GFX_Swap_AppndCtScap+41Ew ...
// WZD dseg:9320 00 00 00 00 00 00                               IMG_CTY_Scap_Trees@ dw 3 dup(0)         ; DATA XREF: GFX_Swap_AppndCtScap+3F6w ...

// WZD dseg:9326
// drake178: IMG_CTY_Scap_Houses@[15]
extern SAMB_ptr cityscape_houses_seg[15];

// WZD dseg:9344 00 00                                           IMG_CTY_Scap_Roads@ dw 0                ; DATA XREF: GFX_Swap_AppndCtScap+38Fw ...
// WZD dseg:9346 00 00                                           IMG_CTY_WallofDark@ dw 0                ; DATA XREF: GFX_Swap_AppndCtScap+30Fw ...
// WZD dseg:9348 00 00                                           IMG_CTY_Unk dw 0
// WZD dseg:934A 00 00                                           IMG_CTY_WallofFire@ dw 0                ; DATA XREF: GFX_Swap_AppndCtScap+2F8w ...
// WZD dseg:934C 00 00                                           IMG_CTY_Consecrate@ dw 0                ; DATA XREF: GFX_Swap_AppndCtScap+2E1w ...
// WZD dseg:934E 00 00                                           IMG_CTY_Prosperity@ dw 0                ; DATA XREF: GFX_Swap_AppndCtScap+2CAw ...
// WZD dseg:9350 00 00                                           IMG_CTY_Inspiratn@ dw 0                 ; DATA XREF: GFX_Swap_AppndCtScap+2B3w ...
// WZD dseg:9352 00 00                                           IMG_CTY_NatsEye@ dw 0                   ; DATA XREF: GFX_Swap_AppndCtScap+29Cw ...
// WZD dseg:9354 00 00                                           IMG_CTY_SorcWard@ dw 0                  ; DATA XREF: GFX_Swap_AppndCtScap+285w ...
// WZD dseg:9356 00 00                                           IMG_CTY_LifeWard@ dw 0                  ; DATA XREF: GFX_Swap_AppndCtScap+26Ew ...
// WZD dseg:9358 00 00                                           IMG_CTY_NatWard@ dw 0                   ; DATA XREF: GFX_Swap_AppndCtScap+257w ...
// WZD dseg:935A 00 00                                           IMG_CTY_ChaosWard@ dw 0                 ; DATA XREF: GFX_Swap_AppndCtScap+240w ...
// WZD dseg:935C 00 00                                           IMG_CTY_DeathWard@ dw 0                 ; DATA XREF: GFX_Swap_AppndCtScap+229w ...
// WZD dseg:935E 00 00                                           IMG_CTY_FamineMask@ dw 0                ; DATA XREF: GFX_Swap_AppndCtScap+12Cw ...

// WZD dseg:9360
// drake178: IMG_CTY_Arc_CRift@
extern SAMB_ptr cityscape_background_arcanus_chaosrift_seg;

// WZD dseg:9362
// drake178: IMG_CTY_Arc_HLight@
extern SAMB_ptr cityscape_background_arcanus_alkar_seg;

// WZD dseg:9364
// drake178: IMG_CTY_Arc_CDark@
extern SAMB_ptr cityscape_background_arcanus_darkcloud_seg;

// WZD dseg:9366
// drake178: IMG_CTY_Arc_MtnBG@
extern SAMB_ptr cityscape_background_arcanus_mountain_seg;

// WZD dseg:9368
// drake178: IMG_CTY_Arc_HillBG@
extern SAMB_ptr cityscape_background_arcanus_hills_seg;

// WZD dseg:936A
// drake178: IMG_CTY_Arc_Ocean@
extern SAMB_ptr cityscape_background_arcanus_ocean_seg;

// WZD dseg:936C
// drake178: IMG_CTY_Arc_River@
extern SAMB_ptr cityscape_background_arcanus_river_seg;

// WZD dseg:936E 00 00                                           IMG_CTY_CursedLands@ dw 0               ; DATA XREF: GFX_Swap_AppndCtScap+74w ...
// WZD dseg:9370 00 00                                           IMG_CTY_LivingLands@ dw 0               ; DATA XREF: GFX_Swap_AppndCtScap+5Dw
// WZD dseg:9372 00 00                                           IMG_CTY_GaiasMask@ dw 0                 ; DATA XREF: GFX_Swap_AppndCtScap+46w ...
// WZD dseg:9374 00 00                                           IMG_CTY_FlyFortress@ dw 0               ; DATA XREF: GFX_Swap_AppndCtScap+2Fw ...

// WZD dseg:9376
// drake178: IMG_CTY_Arc_BGs@
extern SAMB_ptr cityscape_background_arcanus_ground_seg;

// WZD dseg:9378
// drake178: IMG_CTY_Myr_River@
extern SAMB_ptr cityscape_background_myrror_river_seg;

// WZD dseg:937A
// drake178: IMG_CTY_Myr_Ocean@
extern SAMB_ptr cityscape_background_myrror_ocean_seg;

// WZD dseg:937C 00 00                                           IMG_CTY_Arc_GrassBG@ dw 0               ; DATA XREF: GFX_Swap_AppndCtScap+143w ...

// WZD dseg:937E
// drake178: IMG_CTY_Myr_CRift@
extern SAMB_ptr cityscape_background_myrror_chaosrift_seg;

// WZD dseg:9380
// drake178: IMG_CTY_Myr_CDark@
extern SAMB_ptr cityscape_background_myrror_darkcloud_seg;

// WZD dseg:9382
// drake178: IMG_CTY_Myr_HLight@
extern SAMB_ptr cityscape_background_myrror_alkar_seg;

// WZD dseg:9384 00 00                                           IMG_CTY_Myr_GrassBG@ dw 0               ; DATA XREF: GFX_Swap_AppndCtScap+19Fw ...

// WZD dseg:9386
// drake178: IMG_CTY_Myr_MtnBG@
extern SAMB_ptr cityscape_background_myrror_mountain_seg;

// WZD dseg:9388
// drake178: IMG_CTY_Myr_HillBG@
extern SAMB_ptr cityscape_background_myrror_hills_seg;

// WZD dseg:938A
// drake178: IMG_CTY_Myr_BGs@
extern SAMB_ptr cityscape_background_myrror_ground_seg;

// WZD dseg:938C
extern struct s_BLDG_DATA * bldg_data_table;

// WZD dseg:9390 00 00                                           SND_Music_Segment dw 0                  ; DATA XREF: IDK_BuilingCompletedMessage+16Br ...
// WZD dseg:9390                                                                                         ; 15Eh LBX_Alloc_Space paragraphs





// WZD dseg:9392
// drake178: Visibility_Myrror
extern uint8_t * square_scouted_p0;         // Bit_Field  alloc in Allocate_Data_Space()
// WZD dseg:9396
// drake178: Visibility_Arcanus
extern uint8_t * square_scouted_p1;         // Bit_Field  alloc in Allocate_Data_Space()





/* -2: NEVER, -1: ALWAYS, {0,1,2,3}: frame - draw off, {4,5,6,7}: frame - draw on */
// ? Set_Draw_Active_Stack_...() means this variable is private to ovr067 ?
// WZD dseg:9442
extern int16_t draw_active_stack_flag;  // AKA Active_Stack_Draw

// WZD dseg:9444
extern int16_t all_units_moved;

// WZD dseg:9446
// drake178: G_OVL_MapVar4
extern int16_t reset_active_stack;

// WZD dseg:9448 00 00                                           SBK_Candidate_Page dw 0                 ; DATA XREF: G_WLD_StaticAssetRfrsh+67w ...
// WZD dseg:944A 00 00                                           SBK_SomePageSaveVar dw 0                ; DATA XREF: G_WLD_StaticAssetRfrsh+5Bw ...
// WZD dseg:944C 00 00                                           CMB_SpellBookPage dw 0                  ; DATA XREF: G_WLD_StaticAssetRfrsh+61w ...
// WZD dseg:944E 00 00                                           dw 0
// WZD dseg:9450 00 00                                           dw 0
// WZD dseg:9452 00 00                                           _spellbook_small_text dw 0              ; DATA XREF: Spellbook_Load_Small_Pictures+85w ...
// WZD dseg:9454 00 00 00 00 00 00 00 00 00 00 00 00             _spellbook_small_symbols SPBK_ICONS <0> ; DATA XREF: Spellbook_Load_Small_Pictures+67w ...
// WZD dseg:9460 00 00                                           IMG_SPL_BigBook@ dw 0                   ; DATA XREF: GAME_LearnSpellAnim+16Dw ...
// WZD dseg:9460                                                                                         ; LBXE_LoadReplace into the LBX_Sandbox_Segment
// WZD dseg:9462 00 00                                           dw 0
// WZD dseg:9464 00 00                                           IMG_SPL_BigRDogear@ dw 0                ; DATA XREF: SBK_PageFlip_Big+79w ...
// WZD dseg:9466 00 00                                           IMG_SPL_BigLDogear@ dw 0                ; DATA XREF: SBK_PageFlip_Big+62w ...
// WZD dseg:9468 00 00                                           _spellbook_small_right_corner_seg dw 0  ; DATA XREF: Spellbook_Load_Small_Pictures+46w ...
// WZD dseg:9468                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:946A 00 00                                           _spellbook_small_left_corner_seg dw 0   ; DATA XREF: Spellbook_Load_Small_Pictures+2Fw ...
// WZD dseg:946A                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:946C 00 00                                           word_3FF0C dw 0                         ; DATA XREF: sub_BECD9+23w ...
// WZD dseg:946E 00 00                                           _spellbook_small_seg dw 0               ; DATA XREF: Spellbook_Load_Small_Pictures+18w ...
// WZD dseg:946E                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:9470 00 00                                           IMG_GUI_ScrollBtm@ dw 0                 ; DATA XREF: CMB_DrawSummary+3Br ...
// WZD dseg:9472 00 00                                           dw 0
// WZD dseg:9474 00 00                                           dw 0
// WZD dseg:9476 00 00                                           dw 0
// WZD dseg:9478 00 00                                           dw 0
// WZD dseg:947A 00 00                                           dw 0
// WZD dseg:947C 00 00                                           dw 0

// WZD dseg:947E
extern int16_t mana_staff_locked;

// WZD dseg:9480
extern int16_t research_staff_locked;

// WZD dseg:9482
extern int16_t skill_staff_locked;

// WZD dseg:9484 00 00                                           IMG_GUI_AlchRevText@ dw 0               ; DATA XREF: GAME_AlchemyWindow+10Bw ...
// WZD dseg:9486 00 00                                           IMG_GUI_AlchemyBar@ dw 0                ; DATA XREF: GAME_AlchemyWindow+DDw ...
// WZD dseg:9488 00 00                                           IMG_GUI_AlchemyRArr@ dw 0               ; DATA XREF: GAME_AlchemyWindow+C6w ...
// WZD dseg:9488                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:948A 00 00                                           IMG_GUI_AlchemyLArr@ dw 0               ; DATA XREF: GAME_AlchemyWindow+AFw ...
// WZD dseg:948A                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:948C 00 00                                           IMG_GUI_AlchemyCncl@ dw 0               ; DATA XREF: GAME_AlchemyWindow+81w ...
// WZD dseg:948C                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:948E 00 00                                           IMG_GUI_AlchemyOk@ dw 0                 ; DATA XREF: GAME_AlchemyWindow+98w ...
// WZD dseg:948E                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:9490 00 00                                           UU_IMG_NewG_OKBtn@ dw 0                 ; MAGIC.EXE variable, unused in WIZARDS.EXE
// WZD dseg:9492 00                                              db    0
// WZD dseg:9493 00                                              db    0
// WZD dseg:9494 00 00                                           IMG_GUI_AlchemyStar@ dw 0               ; DATA XREF: GAME_AlchemyWindow+F4w ...
// WZD dseg:9496 00 00                                           IMG_GUI_Alchemy_BG@ dw 0                ; DATA XREF: GAME_AlchemyWindow+6Aw ...

// WZD dseg:9498
// drake178: IMG_CTY_RED_Btn@
extern SAMB_ptr red_button_seg;

// WZD dseg:949A
// drake178: IMG_CTY_RightBldTab@
extern SAMB_ptr IMG_CTY_RightBldTab;

// WZD dseg:949C 00 00                                           
// drake178: IMG_CTY_LeftBldTab@
extern SAMB_ptr IMG_CTY_LeftBldTab;

// WZD dseg:949E 00 00                                           IMG_OVL_EnemyCityBG@ dw 0               ; DATA XREF: IDK_EnemyCityScreen_s4A3F0+115w ...

// WZD dseg:94A0
// drake178: IMG_CTY_10_Food@
// WZD dseg:94A2
// drake178: IMG_CTY_10_Prod@
// WZD dseg:94A4
// drake178: IMG_CTY_10_Gold@
// WZD dseg:94A6
// drake178: IMG_CTY_10_Power@
// WZD dseg:94A8
// drake178: IMG_CTY_10_Books@
// WZD dseg:94AA
// drake178: IMG_CTY_Neg_10_Gold@
extern SAMB_ptr city_big_resource_icon_seg[6];

// WZD dseg:94AC
// drake178: IMG_CTY_Neg_10_Food@
extern SAMB_ptr city_grey_big_bread_icon_seg;

// WZD dseg:94AE
// drake178: IMG_CTY_Rebels@[14]
extern SAMB_ptr city_rebel_icon_seg[14];

// WZD dseg:94CA
// drake178: IMG_CTY_Farmers@[14]
extern SAMB_ptr city_farmer_icon_seg[14];

// WZD dseg:94E6
// drake178: IMG_CTY_1_Food@
// WZD dseg:94E8
// drake178: IMG_CTY_1_Prod@
// WZD dseg:94EA
// drake178: IMG_CTY_1_Gold@
// WZD dseg:94EC
// drake178: IMG_CTY_1_Power@
// WZD dseg:94EE
// drake178: IMG_CTY_1_Book@
extern SAMB_ptr city_lil_resource_icon_seg[5];

// WZD dseg:94F0
// drake178: IMG_CTY_Neg_1_Food@
extern SAMB_ptr city_grey_lil_bread_icon_seg;

// WZD dseg:94F2
// drake178: IMG_CTY_Workers@[14];
extern SAMB_ptr city_worker_icon_seg[14];





// WZD dseg:950E                                                 BEGIN: CityList_Screen_Load()

// WZD dseg:950E 00 00                                           citylist_ok_button_seg dw 0             ; DATA XREF: CityList_Screen+101r ...
// WZD dseg:950E                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:9510 00 00                                           citylist_down_button_seg dw 0           ; DATA XREF: CityList_Screen+1FFr ...
// WZD dseg:9510                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:9512 00 00                                           citylist_up_button_seg dw 0             ; DATA XREF: CityList_Screen+16Er ...
// WZD dseg:9512                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:9514 00 00                                           citylist_background_seg dw 0            ; DATA XREF: CityList_Screen:loc_5767Dr ...

// WZD dseg:9514                                                 END: CityList_Screen_Load()

// WZD dseg:9516                                                 BEGIN:  Main Screen Pictures

// WZD dseg:9516
extern SAMB_ptr next_turn_button_seg;
// MAIN.LBX,58  DESELECT    next turn button..

// WZD dseg:9518
extern SAMB_ptr survey_background;
// MAIN.LBX,57  MAINSRVY    survey backgrnd

// WZD dseg:951A
extern SAMB_ptr road_button_border;
// MAIN.LBX,48  C&RBORDR    road button border

// WZD dseg:951C
extern SAMB_ptr road_background;
// MAIN.LBX,45  MAINROAD    road background

// WZD dseg:951E
extern SAMB_ptr road_ok_button;
// MAIN.LBX,46  CASTCNCL    road ok button

// WZD dseg:9522
extern SAMB_ptr DESELECT_button_blockout;
// MAIN.LBX,44    DESELECT    button blockout

// WZD dseg:9524
extern SAMB_ptr cast_button_border;
// MAIN.LBX,47  C&RBORDR    cast button border

// WZD dseg:9526
extern SAMB_ptr cast_cancel_button;
// MAIN.LBX,41  CASTCNCL    cast cancel button

// WZD dseg:9528
extern SAMB_ptr cast_background;
// MAIN.LBX,40  MAINCAST    cast background

// WZD dseg:952C
extern SAMB_ptr next_turn_background_seg;
// MAIN.LBX,35  DESELECT    next turn backgrnd

// WZD dseg:952E
extern SAMB_ptr deselect_background;
// MAIN.LBX,34  DESELECT    deselect backgrnd

// WZD dseg:952E                                                 END:  Main Screen Pictures

// WZD dseg:9530
// drake178: IMG_CTY_EmptyHuts@
extern SAMB_ptr outpost_dark_icon_seg[3];

// WZD dseg:9536
// drake178: IMG_CTY_FilledHuts@
extern SAMB_ptr outpost_bright_icon_seg[3];

// WZD dseg:953C 00 00                                           namecity_background_seg dw 0            ; DATA XREF: City_Screen_Load_Pictures+239w ...
// WZD dseg:953E 00 00                                           IMG_CTY_Outpost_BG@ dw 0                ; DATA XREF: City_Screen_Load_Pictures+222w ...

// WZD dseg:9540 00 00                                           
// IMG_USW_SideBtns_BG@ dw 0               ; DATA XREF: GFX_Swap_AppendUView+8Bw ...
extern SAMB_ptr unitview_button_background_seg;

// WZD dseg:9542 00 00                                           dw 0
// WZD dseg:9544 00 00                                           dw 0
// WZD dseg:9546 00 00                                           dw 0

// WZD dseg:9548 00 00                                           
// IMG_OVL_BuildBtn_BG@ dw 0               ; DATA XREF: GFX_Swap_AppendUView+176w
extern SAMB_ptr IMG_OVL_BuildBtn_BG;
// WZD dseg:9548                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

// WZD dseg:954A 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+mirrow_screen_18_books_icons dw 12h dup(0)
// WZD dseg:954A 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; DATA XREF: IDK_MirrorScreen_s6343B+88w ...
// WZD dseg:956E 00 00                                           mirror_screen_background dw 0           ; DATA XREF: Main_Screen_Load_Pictures+35Fw ...



// WZD dseg:9570 00 00                                           armylist_ok_button_seg dw 0             ; DATA XREF: ArmyList_Screen+12Cr ...
// WZD dseg:9570                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:9572 00 00                                           armylist_items_button_seg dw 0          ; DATA XREF: ArmyList_Screen+109r ...
// WZD dseg:9572                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:9574 00 00                                           armylist_down_button_seg dw 0           ; DATA XREF: ArmyList_Screen+1FAr ...
// WZD dseg:9574                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:9574                                                                                         ; should use dseg:9510
// WZD dseg:9576 00 00                                           armylist_up_button_seg dw 0             ; DATA XREF: ArmyList_Screen+A1r ...
// WZD dseg:9576                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:9576                                                                                         ; should use dseg:9512
// WZD dseg:9578 00 00                                           armylist_background_seg dw 0            ; DATA XREF: ArmyList_Screen_Load+BBw ...
// WZD dseg:957A 00 00 00 00 00 00 00 00 00 00 00 00             IMG_ARMY_ItemSlots@ ISLT_ICONS <0>      ; DATA XREF: GFX_Swap_AppendItems+A3w ...
// WZD dseg:957A                                                                                         ; array of 6 appended reserved EMM headers in
// WZD dseg:957A                                                                                         ; GFX_Swap_Seg, each with one item slot image
// WZD dseg:9586 00 00                                           IMG_ItemScrn_Ok@ dw 0                   ; DATA XREF: ITEM_ScreenRedraw+3D6r ...
// WZD dseg:9586                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:9588 00 00                                           IMG_ItemScrn_Alch@ dw 0                 ; DATA XREF: ITEM_ScreenRedraw+3AEr ...
// WZD dseg:9588                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:958A 00 00                                           IMG_ItemScrn_HeroBr@ dw 0               ; DATA XREF: ITEM_ScreenRedraw+9Cr ...
// WZD dseg:958C 00 00                                           IMG_ItemScrn_BG@ dw 0                   ; DATA XREF: Items_Screen+2Fr ...
// WZD dseg:958E 00 00 00 00 00 00 00 00 00 00 00 00             lilwiz_gem_segs dw 6 dup(0)             ; DATA XREF: Magic_Screen_Load_Pictures+BFw ...
// WZD dseg:959A 00                                              db    0
// WZD dseg:959B 00                                              db    0
// WZD dseg:959C 00                                              db    0
// WZD dseg:959D 00                                              db    0
// WZD dseg:959E 00                                              db    0
// WZD dseg:959F 00                                              db    0
// WZD dseg:95A0 00                                              db    0
// WZD dseg:95A1 00                                              db    0
// WZD dseg:95A2 00                                              db    0
// WZD dseg:95A3 00                                              db    0
// WZD dseg:95A4 00                                              db    0
// WZD dseg:95A5 00                                              db    0
// WZD dseg:95A6 00                                              db    0
// WZD dseg:95A7 00                                              db    0
// WZD dseg:95A8 00                                              db    0
// WZD dseg:95A9 00                                              db    0
// WZD dseg:95AA 00 00                                           skill_staff_locked_seg dw 0             ; DATA XREF: Magic_Screen_Draw+59Er ...
// WZD dseg:95AC 00 00                                           research_staff_locked_seg dw 0          ; DATA XREF: Magic_Screen_Draw+525r ...
// WZD dseg:95AE 00 00                                           mana_staff_locked_seg dw 0              ; DATA XREF: Magic_Screen_Draw+4ACr ...
// WZD dseg:95B0 00 00                                           magic_ok_button_seg dw 0                ; DATA XREF: Magic_Screen_Draw+10Er ...
// WZD dseg:95B0                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:95B2 00 00                                           magic_alchemy_button_seg dw 0           ; DATA XREF: Magic_Screen_Draw+130r ...
// WZD dseg:95B2                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:95B4 00 00                                           skill_staff_full_seg dw 0               ; DATA XREF: Magic_Screen_Draw:loc_61445r ...
// WZD dseg:95B6 00 00                                           skill_staff_empty_seg dw 0              ; DATA XREF: Magic_Screen_Draw+568r ...
// WZD dseg:95B8 00 00                                           research_staff_full_seg dw 0            ; DATA XREF: Magic_Screen_Draw:loc_613CCr ...
// WZD dseg:95BA 00 00                                           research_staff_empty_seg dw 0           ; DATA XREF: Magic_Screen_Draw+4EFr ...
// WZD dseg:95BC 00 00                                           mana_staff_full_seg dw 0                ; DATA XREF: Magic_Screen_Draw:loc_61353r ...
// WZD dseg:95BE 00 00                                           mana_staff_empty_seg dw 0               ; DATA XREF: Magic_Screen_Draw+476r ...
// WZD dseg:95C0 00 00                                           grey_gem_seg dw 0                       ; DATA XREF: Magic_Screen_Draw:loc_617EEr ...
// WZD dseg:95C2 00 00 00 00 00 00 00 00 00 00 00 00             broken_grey_gem_seg dw 6 dup(0)         ; DATA XREF: Magic_Screen_Draw:loc_61851r ...
// WZD dseg:95CE 00 00                                           magic_background_seg dw 0               ; DATA XREF: Magic_Screen_Draw+F9r ...
// WZD dseg:95D0 00 00                                           dw 0
// WZD dseg:95D2 00 00                                           dw 0
// WZD dseg:95D4 00 00                                           dw 0
// WZD dseg:95D6 00 00                                           dw 0
// WZD dseg:95D8 00 00                                           dw 0
// WZD dseg:95DA 00 00                                           dw 0
// WZD dseg:95DC 00 00                                           dw 0
// WZD dseg:95DE 00 00                                           dw 0
// WZD dseg:95E0 00 00                                           dw 0
// WZD dseg:95E2 00 00                                           dw 0
// WZD dseg:95E4 00 00                                           dw 0
// WZD dseg:95E6 00 00                                           dw 0
// WZD dseg:95E8 00 00                                           dw 0
// WZD dseg:95EA 00 00                                           dw 0
// WZD dseg:95EC 00 00                                           dw 0
// WZD dseg:95EE 00 00                                           dw 0

// WZD dseg:95F0
// drake178: IMG_CTY_HalfRsc@
extern SAMB_ptr city_half_background_seg;

// WZD dseg:95F2 00 00                                           goto_booty_icon dw 0                    ; DATA XREF: Main_Screen_Load_Pictures+371w ...
// WZD dseg:95F2                                                                                         ; single-loaded image
// WZD dseg:95F4 00 00 00 00 00 00 00 00 00 00 00 00             hero_portraits_seg dw 6 dup(0)          ; DATA XREF: ArmyList_Screen_Draw+38Dr ...
// WZD dseg:9600 00 00 00 00                                     TBL_TERRSTAT_EMS@ dd 0                  ; allocated in the overland djikstra patch
// WZD dseg:9604 00 00 00 00                                     TBL_PF_Conversions@ dd 0                ; allocated in the overland djikstra patch
// WZD dseg:9608 00 00 00 00                                     TBL_PF_Tentative@ dd 0                  ; allocated in the overland djikstra patch
// WZD dseg:960C 00 00 00 00                                     TBL_PF_Results@ dd 0                    ; allocated in the overland djikstra patch
// WZD dseg:9610 00                                              db    0
// WZD dseg:9611 00                                              db    0
// WZD dseg:9612 00                                              db    0
// WZD dseg:9613 00                                              db    0
// WZD dseg:9614 00                                              db    0
// WZD dseg:9615 00                                              db    0
// WZD dseg:9616 00                                              db    0
// WZD dseg:9617 00                                              db    0
// WZD dseg:9618 00                                              db    0
// WZD dseg:9619 00                                              db    0
// WZD dseg:961A 00                                              db    0
// WZD dseg:961B 00                                              db    0
// WZD dseg:961C 00                                              db    0
// WZD dseg:961D 00                                              db    0
// WZD dseg:961E 00                                              db    0
// WZD dseg:961F 00                                              db    0
// WZD dseg:9620 00                                              db    0
// WZD dseg:9621 00                                              db    0
// WZD dseg:9622 00                                              db    0
// WZD dseg:9623 00                                              db    0
// WZD dseg:9624 00                                              db    0
// WZD dseg:9625 00                                              db    0
// WZD dseg:9626 00                                              db    0
// WZD dseg:9627 00                                              db    0
// WZD dseg:9628 00                                              db    0
// WZD dseg:9629 00                                              db    0
// WZD dseg:962A 00                                              db    0
// WZD dseg:962B 00                                              db    0
// WZD dseg:962C 00                                              db    0
// WZD dseg:962D 00                                              db    0
// WZD dseg:962E 00                                              db    0
// WZD dseg:962F 00                                              db    0
// WZD dseg:9630 00                                              db    0
// WZD dseg:9631 00                                              db    0
// WZD dseg:9632 00                                              db    0
// WZD dseg:9633 00                                              db    0
// WZD dseg:9634 00                                              db    0
// WZD dseg:9635 00                                              db    0
// WZD dseg:9636 00                                              db    0
// WZD dseg:9637 00                                              db    0
// WZD dseg:9638 00                                              db    0
// WZD dseg:9639 00                                              db    0
// WZD dseg:963A 00                                              db    0
// WZD dseg:963B 00                                              db    0
// WZD dseg:963C 00                                              db    0
// WZD dseg:963D 00                                              db    0
// WZD dseg:963E 00                                              db    0
// WZD dseg:963F 00                                              db    0
// WZD dseg:9640 00                                              db    0
// WZD dseg:9641 00                                              db    0
// WZD dseg:9642 00                                              db    0
// WZD dseg:9643 00                                              db    0

// WZD dseg:9644
extern int16_t _city_idx;

// WZD dseg:9646 00                                              db    0
// WZD dseg:9647 00                                              db    0

// WZD dseg:9648
// MoO2:  _draw_ship_bitmap 0018A83C  _ship_bitmap 0018A840
extern SAMB_ptr UnitDraw_WorkArea;

// WZD dseg:964A 00                                              db    0
// WZD dseg:964B 00                                              db    0

// WZD dseg:964C
// MoO2: ~== _ship_node
extern int16_t entities_on_movement_map[];


// dseg:973C 00 00                                           gsa_BACKGRND_3_IMG_CMB_Bottom_BG dw 0   ; DATA XREF: Load_Combat_Background_Bottom+12w ...



// WZD dseg:9750                                                 ¿ END: maps / map grid fields - Uninitialized Data ?

// WZD dseg:9752                                                 ? BEGIN: City Screen ?

// WZD dseg:9752 00 00                                           scanned_field__G_CTY_ClickedLabel dw 0  ; DATA XREF: City_Screen+2A9w ...
// WZD dseg:9752
extern int16_t city_screen_scanned_field;

// WZD dseg:9754 00 00                                           
// IMG_OVL_UnitListBtm@ dw 0               ; DATA XREF: GFX_Swap_AppendUView+15Fw ...
extern SAMB_ptr IMG_OVL_UnitListBtm;
// WZD dseg:9754                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

// WZD dseg:9756 00 00                                           
// IMG_OVL_UnitList_BG@ dw 0               ; DATA XREF: GFX_Swap_AppendUView+148w ...
extern SAMB_ptr IMG_OVL_UnitList_BG;
// WZD dseg:9756                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

// WZD dseg:9758
// drake178: IMG_CTY_NewBuild_BG@
extern SAMB_ptr city_new_build_notify_grass_seg;

// WZD dseg:975A
// drake178: IMG_CTY_SplScrlDn@
extern SAMB_ptr city_spell_dn_arrow_button_seg;

// WZD dseg:975C
// drake178: IMG_CTY_SplScrlUp@
extern SAMB_ptr city_spell_up_arrow_button_seg;

// WZD dseg:975E 00 00 00 00 00 00 00 00 00 00 00 00             IMG_USW_ItemSlots ISLT_ICONS <0>        ; DATA XREF: GFX_Swap_AppendItems+15Bw ...
// WZD dseg:975E                                                                                         ; array of 6 appended reserved EMM headers in
// WZD dseg:975E                                                                                         ; GFX_Swap_Seg, each with one item slot image
// WZD dseg:976A 00 00                                           IMG_USW_ItemHelpBlt@ dw 0               ; DATA XREF: GFX_Swap_AppendItems+144w ...
// WZD dseg:976A                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:976C 00 00                                           IMG_USW_ItemHelp_BG@ dw 0               ; DATA XREF: GFX_Swap_AppendItems+12Dw ...
// WZD dseg:976C                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

// WZD dseg:976E
// drake178: IMG_USW_ItemPowers IPOW_ICONS
extern SAMB_ptr IMG_USW_ItemPowers[22];

// WZD dseg:979A 00 00                                           IMG_MOODWIZPortrait@ dw 0               ; DATA XREF: IDK_DiplAnim_s6FDA1+4Cr ...
// WZD dseg:979A                                                                                         ; 3 frame image (good, bad, neutral)


// WZD dseg:979C
// drake178: IMG_USW_WaterBase@
extern SAMB_ptr unit_water_diamond_seg;

// WZD dseg:979E
// drake178: IMG_USW_Portrt_Brdr@
extern SAMB_ptr IMG_USW_Portrt_Brdr;

// WZD dseg:97A0
// drake178: IMG_USW_1_Gold@
extern SAMB_ptr gold1_icon_seg;

// WZD dseg:97A2
// drake178: IMG_CTY_Neg_1_Gold@
extern SAMB_ptr black_gold1_icon_seg;

// WZD dseg:97A4
// drake178: IMG_USW_AbBorders@
extern SAMB_ptr unitview_specials_borders_seg[8];

// WZD dseg:97B4
// drake178: IMG_USW_ArrowDown@
extern SAMB_ptr unitview_down_arrow_seg;

// WZD dseg:97B6
// drake178: IMG_USW_ArrowUp@
extern SAMB_ptr unitview_up_arrow_seg;

// WZD dseg:97B8
// drake178: IMG_USW_Background@
extern SAMB_ptr unitview_large_background_seg;

// WZD dseg:97BA 00 00                                           
// IMG_USW_UnitHire_BG@ dw 0               ; DATA XREF: GFX_Swap_AppendUView+5Dw ...
extern SAMB_ptr unitview_small_background_seg;
// WZD dseg:97BA                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

// WZD dseg:97BC
// drake178: IMG_USW_Stats_Gold
extern SAMB_ptr unitview_stat_gold_icons_seg[15];

// WZD dseg:97DA
// drake178: IMG_USW_Stat_Icons
extern SAMB_ptr unitview_stat_icons_seg[15];

// WZD dseg:97F8 00                                              
extern SAMB_ptr special_seg[145];

// WZD dseg:991A
// drake178: IMG_OVL_EZConfirmBG@
extern SAMB_ptr lair_confirm_border_seg;

// WZD dseg:991C
// drake178: IMG_OVL_EZBtm_BG@
extern SAMB_ptr lair_bottom_without_button_seg;

// WZD dseg:991E
// drake178: IMG_OVL_EZBtn_BG@
extern SAMB_ptr lair_bottom_with_button_seg;

// WZD dseg:9920 00 00                                           CRP_DBG_Alt_T_State dw 0                ; DATA XREF: Main_Screen+47Fr ...

// WZD dseg:9920                                                 ? END: City Screen ?

// WZD dseg:9922                                                 BEGIN:  Main Screen Pictures

// WZD dseg:9922
extern SAMB_ptr main_background;

// WZD dseg:9924
extern SAMB_ptr unit_backgrounds[9];


// WZD dseg:9924                                                 END:  Main Screen Pictures

// WZD dseg:9936 00 00                                           city_background_seg dw 0                ; DATA XREF: City_Screen_Load_Pictures+18w ...
// WZD dseg:9938 00 00                                           city_block_out_seg dw 0                 ; DATA XREF: City_Screen_Load_Pictures+B9w ...
// WZD dseg:993A 00 00                                           city_full_resource_seg dw 0             ; DATA XREF: City_Screen_Load_Pictures+A2w ...
// WZD dseg:993C 00 00                                           city_reqd_resource_seg dw 0             ; DATA XREF: City_Screen_Load_Pictures+8Bw ...
// WZD dseg:993E 00 00                                           city_lock_buy_button_seg dw 0           ; DATA XREF: City_Screen_Load_Pictures+D0w ...
// WZD dseg:9940 00 00                                           city_ok_button_seg dw 0                 ; DATA XREF: City_Screen_Load_Pictures+5Dw ...
// WZD dseg:9942 00 00                                           city_buy_button_seg dw 0                ; DATA XREF: City_Screen_Load_Pictures+2Fw ...
// WZD dseg:9944 00 00                                           city_change_button_seg dw 0             ; DATA XREF: City_Screen_Load_Pictures+46w ...

// WZD dseg:9946                                                 BEGIN:  Main Screen Pictures

// WZD dseg:9946
extern SAMB_ptr main_red_medal_icon;
extern SAMB_ptr main_gold_medal_icon;
extern SAMB_ptr main_white_medal_icon;
extern SAMB_ptr main_adamantium_weapon_icon;
extern SAMB_ptr main_mithril_weapon_icon;
extern SAMB_ptr main_magic_weapon_icon;

// Screen Layout: L-R, T-B
//     Done, Patrol, Wait, Build
extern SAMB_ptr main_lock_purify_button;
extern SAMB_ptr main_lock_build_button;
extern SAMB_ptr main_lock_wait_button;
extern SAMB_ptr main_lock_patrol_button;
extern SAMB_ptr main_lock_done_button;

extern SAMB_ptr main_meld_button;
// WZD dseg:995E
extern SAMB_ptr main_purify_button;
// WZD dseg:9960 00 00                                           IDK_UU_action_C dw 0
// WZD dseg:9962
extern SAMB_ptr main_build_button;
extern SAMB_ptr main_done_button;
extern SAMB_ptr main_wait_button;
extern SAMB_ptr main_patrol_button;

// ? block of Main Screen - Top Buttons Bar ?
// XREF: Main_Screen_Load_Pictures(); OVL_SetMenuButtons(); OVL_DrawMainMenu()
// ; single-loaded 2 frame image (normal - clicked)
// Code Order:   ?
// Data Order:   S,I,M,C,G,A,P
// Load Order:   G,S,A,C,M,I,P
// LBX Order:    G,S,A,C,M,I,P
// Screen Order: G,S,A,C,M,I,P
extern SAMB_ptr main_spells_button;
extern SAMB_ptr main_info_button;
extern SAMB_ptr main_magic_button;
extern SAMB_ptr main_cities_button;
extern SAMB_ptr main_game_button;
extern SAMB_ptr main_armies_button;
extern SAMB_ptr main_plane_button;

// XREF: Main_Screen_Load_Pictures(); Draw_Movement_Mode_Icons()
// single-loaded image
// gets indexed like an array of pictures in Draw_Movement_Mode_Icons()
extern SAMB_ptr move_sail_icon;
extern SAMB_ptr move_swim_icon;
extern SAMB_ptr move_mt_icon;
extern SAMB_ptr move_forest_icon;
extern SAMB_ptr move_fly_icon;
extern SAMB_ptr move_path_icon;
extern SAMB_ptr move_astral_icon;
extern SAMB_ptr move_wind_icon;
extern SAMB_ptr move_boot_icon;
// WZD dseg:998A 00 00                                           IDK_UU_Cavalry dw 0

// dseg:998A END:  Main Screen Pictures



extern SAMB_ptr movement_mode_icons[10];  // {0,...,9} 10 icons


















// dseg:998A END:  Main Screen Pictures

// WZD dseg:998C
// GUI_SmallWork_IMG@ dw 0
extern SAMB_ptr GfxBuf_2400B;

// WZD dseg:998E
extern SAMB_ptr _screen_seg;
// allocated in MoM_Init_Tables()

// WZD dseg:9990
// _current_screen
// WZD dseg:9992 RP_GUI_GrowOutFrames dw 0               ; DATA XREF: VGA_PageFlip_FX+41r ...
// WZD dseg:9994 RP_GUI_GrowOutTop dw 0                  ; DATA XREF: VGA_PageFlip_FX+45r ...
// WZD dseg:9996 RP_GUI_GrowOutLeft dw 0                 ; DATA XREF: VGA_PageFlip_FX+49r ...

// WZD dseg:9998
// AKA TBL_Events
// extern struct s_EVENT_DATA * events_table;  // alloc in Allocate_Data_Space()  7 PR 112 B  type? 1-byte,2-byte signed,unsigned 112 events or 56 events?
extern int16_t * events_table;

// WZD dseg:999C
extern int16_t _unit_stack_count;
// WZD dseg:999E
extern struct s_STACK _unit_stack[];



// WZD dseg:99C2
// drake178: MSG_CityLost_Count
extern int8_t MSG_CityLost_Count;

// WZD dseg:99C3
// drake178: MSG_CityLost_Names db 118h dup(0)
extern char MSG_CityLost_Names[280];

// WZD dseg:9ADB
// drake178: MSG_CityGained_Count
extern int8_t MSG_CityGained_Count;

// WZD dseg:9ADC
// drake178: MSG_CityGained_Array
extern int8_t MSG_CityGained_Array[20];

// WZD dseg:9AF0
// drake178: MSG_BuildDone_Count
extern int8_t MSG_Building_Complete_Count;

// WZD dseg:9AF1 00                                              db    0

// WZD dseg:9AF2
// drake178: MSG_BuildDone_Array Building_Done_Msg_Item 14h dup(<0>)
extern struct s_MSG_BUILDING_COMPLETE MSG_Building_Complete[20];

// WZD dseg:9B42
// drake178: MSG_BldLost_Count
extern int8_t MSG_BldLost_Count;

// WZD dseg:9B43 00                                              db    0

// WZD dseg:9B44
// drake178: MSG_BldLost_Array Building_Lost_Msg_Item 14h dup(<0>)

// WZD dseg:9B94
// drake178: MSG_UnitLost_Count
extern int8_t MSG_UnitLost_Count;

// WZD dseg:9B95 00                                              db    0

// WZD dseg:9B96
// drake178: MSG_UnitLost_Array Unit_Lost_Msg_Item 14h dup(<0>)
extern struct s_MSG_UNIT_LOST MSG_UnitLost_Array[20];

// WZD dseg:9BE6
// drake178: MSG_UnitKilled_Count
extern int8_t MSG_UnitKilled_Count;

// WZD dseg:9BE7 00                                              db    0

// WZD dseg:9BE8
// drake178: MSG_UnitKilled_Array Unit_Killed_Msg_Item 14h dup(<0>)

// WZD dseg:9C38
// drake178: MSG_CityGrowth_Count
extern int8_t MSG_CityGrowth_Count;

// WZD dseg:9C39
// drake178: MSG_CityGrowth_Array db 14h dup(0)

// WZD dseg:9C4D
// drake178: MSG_CityDeath_Count
extern int8_t MSG_CityDeath_Count;

// WZD dseg:9C4E
// drake178: MSG_CityDeath_Array db 14h dup(0) 

// WZD dseg:9C62
// drake178: MSG_UEsLost_Count
extern int8_t MSG_UEsLost_Count;

// WZD dseg:9C63
// drake178: MSG_UEsLost_Array db 14h dup(0)   

// WZD dseg:9C77
// drake178: MSG_CEsLost_Count
extern int8_t MSG_CEsLost_Count;

// WZD dseg:9C78
// drake178: MSG_CEsLost_Array db 14h dup(0)   

// WZD dseg:9C8C
// drake178: MSG_GEs_Lost
extern int8_t MSG_GEs_Lost;

// WZD dseg:9C8D 00                                              db    0



// WZD dseg:9C90
extern SAMB_ptr TBL_OvlMovePaths_EMS;
// WZD dseg:9C94
// CONTX_Arc_NmeStrMap
// WZD dseg:9C98
// CONTX_Myr_NmeStrMap
// WZD dseg:9C9C
extern SAMB_ptr TBL_Catchments_EMS;

// WZD dseg:9CA0
// drake178: TBL_SharedTiles_EMS
extern uint8_t * square_shared_bits;               // alloc'd in Allocate_Data_Space()

// WZD dseg:9CA4
// drake178: TBL_TempMoveMap_EMS
extern struct s_MOVE_PATH * movepath_cost_map;

// WZD dseg:9CA8
// COL_MinimapTiles

// WZD dseg:9CAC
// drake178: TBL_MoveMaps_EMS
extern struct s_MOVE_MODE_COST_MAPS * movement_mode_cost_maps;



// WZD dseg:9CB0  
extern uint8_t * TBL_Scouting;              // alloc in Allocate_Data_Space()
// WZD dseg:9CB4
extern uint8_t * TBL_Terrain_Flags;         // alloc in Allocate_Data_Space()
// WZD dseg:9CB8
extern SAMB_ptr TBL_Terr_Specials;          // alloc in Allocate_Data_Space()

// WZD dseg:9CBC
// extern SAMB_ptr _CITIES;
// extern struct s_CITY _CITIES[];
extern struct s_CITY * _CITIES;

// WZD dseg:9CC0
// drake178: TBL_Encounters
// AKA TBL_Lairs
extern struct s_LAIR * _LAIRS;

// WZD dseg:9CC4
// drake178: TBL_Towers;
extern struct s_TOWER * _TOWERS;

// WZD dseg:9CC8
// drake178: TBL_Fortresses;
extern struct s_FORTRESS * _FORTRESSES;

// WZD dseg:9CCC
// drake178: TBL_Nodes
extern struct s_NODE * _NODES;

// WZD dseg:9CD0
extern uint8_t * TBL_Landmasses;  // 12Eh 302d PR 302*16=4832, in World_Data
// WZD dseg:9CD4
extern SAMB_ptr UU_TBL_2;
// WZD dseg:9CD8
extern SAMB_ptr UU_TBL_1;
// WZD dseg:9CDC
// AKA TBL_Maps;
// extern SAMB_ptr _world_maps;
extern uint8_t * _world_maps;

// WZD dseg:9CE0
// drake178: 14 individual pointers, one to each row of the table
extern SAMB_ptr TBL_Unrest[14];
extern SAMB_ptr TBL_Unrest_Hack;

// WZD dseg:9D18
// drake178: IMG_CTY_Bldngs_Wall@
extern SAMB_ptr cityscape_big_city_wall_seg;

// WZD dseg:9D1A
extern uint16_t tmp_World_Data_Paras;

// WZD dseg:9D1C
extern SAMB_ptr World_Data_Extra;

// WZD dseg:9D1E
extern SAMB_ptr GFX_Swap_Seg;

// WZD dseg:9D20
extern SAMB_ptr World_Data;

// WZD dseg:9D22
extern uint16_t grand_vizier;


// WZD dseg:9EC2
// extern SAMB_ptr TBL_Units;
// extern struct s_UNIT _UNITS[];
extern struct s_UNIT * _UNITS;

// WZD dseg:9EC6
extern struct s_INACTV_HERO * TBL_Hero_Names;

// WZD dseg:9ECA
// extern struct s_WIZARD _players[6];
extern struct s_WIZARD _players[];
// extern struct s_WIZARD * _players;

// WZD dseg:BB7A
// // // extern struct s_HLP_ENTRY * _help_entries;
// // extern uint8_t _help_entries[];
// extern int16_t _help_entries[];
extern char _help_entries[];


// WZD dseg:974A
// AKA G_OVL_MapDisplay_Y
extern int16_t _prev_world_y;
// WZD dseg:974C
// AKA G_OVL_MapDisplay_X
extern int16_t _prev_world_x;

// WZD dseg:BD82 
extern int16_t OVL_Map_CenterY;  // AKA _active_world_y
extern int16_t _active_world_y;  // AKA _active_world_y
// WZD dseg:BD84 
extern int16_t OVL_Map_CenterX;  // AKA _active_world_x
extern int16_t _active_world_x;  // AKA _active_world_x

// WZD dseg:BD86 
extern int16_t _map_plane;
// WZD dseg:BD88 
extern int16_t _map_y;
// WZD dseg:BD8A 
extern int16_t _map_x;

// WZD dseg:BD8C
extern int16_t _human_player_idx;


// WZD dseg:BD8E
extern int16_t _unit;
extern int16_t _turn;
extern int16_t _units;
extern int16_t _cities;
extern int16_t _difficulty;
extern int16_t _magic;
extern int16_t _landsize;
extern int16_t _num_players;  // New Game: magic_set.opponents + 1
extern struct s_GAME_DATA game_data;
// WZD dseg:BD9E
// magic_set s_MAGIC_SET










// WZD dseg:BF70
extern int16_t city_resources_rows_fields[7];

// WZD dseg:BF7E
extern int16_t * city_population_row_fields;

// WZD dseg:BF80
extern int16_t city_no_buy;

// WZD dseg:BF82
extern int16_t * city_screen_required_buildings_list;

// WZD dseg:BF84
// drake178: CTY_EScrollDn_Label
extern int16_t city_dn_button;

// WZD dseg:BF86
// drake178: CTY_EScrollUp_Label
extern int16_t city_up_button;

// WZD dseg:BF88
extern int16_t UU_CityScreen_Field;

// WZD dseg:BF8A
extern int16_t city_map_y;

// WZD dseg:BF8C
extern int16_t city_map_x;

// WZD dseg:BF8E
extern int16_t G_CTY_ClickLabelCount;

// WZD dseg:BF90
extern byte_ptr G_CTY_ClickLabelArray;

// WZD dseg:BF92
// drake178: CTY_EnchDisplayCount
extern int16_t city_enchantment_display_count;

// WZD dseg:BF94
// drake178: N/A
extern int16_t UU_CityScreen_Field_Something;

// WZD dseg:BF96
// drake178: CTY_EnchantScrolling
extern int16_t city_enchantment_display_scroll_flag;

// WZD dseg:BF98
// drake178: CTY_FirstShownEnch
extern int16_t city_enchantment_display_first;

// WZD dseg:BF9A
extern int16_t city_enchantment_list_count;

// WZD dseg:BF9C
// drake178: CTY_EnchantOwners
extern int16_t * city_enchantment_owner_list;

// WZD dseg:BF9E
// drake178: CTY_EnchantList
extern int16_t * city_enchantment_list;

// WZD dseg:BFA0
extern int16_t * city_enchantment_fields;

// WZD dseg:BFA2
extern int16_t _garrison_window_start_y;

// WZD dseg:BFA4
extern int16_t _garrison_window_start_x;

// WZD dseg:BFA6
extern int16_t city_screen_esc_hotkey;

// WZD dseg:BFA8
extern int16_t city_screen_ok_button;

// WZD dseg:BFAA
extern int16_t city_sceen_change_button;

// WZD dseg:BFAC
extern int16_t city_screen_buy_button;

// WZD dseg:BFAE
extern char * city_screen_product_name;

// WZD dseg:BFB0
extern int16_t IDK_city_production_cost;

// WZD dseg:BFB2
extern int16_t IDK_city_n_turns_to_produce;

// WZD dseg:BFB4
extern int16_t cityscreen_city_built_bldg_idx;

// WZD dseg:BFB6
extern int16_t production_screen_return_screen;

// WZD dseg:BFB8 00 00                                           dw 0

// WZD dseg:BFBA
extern char * GUI_String_2;

// WZD dseg:BFBC
extern char * GUI_String_1;


// WZD dseg:BFBE 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+word_42A5E dw 0Fh dup(0)                ; DATA XREF: sub_4D5EA+47w
// WZD dseg:BFDC 00 00                                           Retn_Struct@ dw 0  // ; FOOD_BRKDN *Retn_Struct
// WZD dseg:BFDE 00 00                                           word_42A7E dw 0                         ; DATA XREF: sub_4D5EA+B1w ...
// WZD dseg:BFE0 00 00                                           word_42A80 dw 0                         ; DATA XREF: sub_4D5EA+ABw ...
// WZD dseg:BFE2 00 00                                           word_42A82 dw 0                         ; DATA XREF: sub_4D5EA+7Bw ...
// WZD dseg:BFE4 00 00                                           IMG_Seg dw 0                            ; DATA XREF: sub_4D5EA+64w ...
// WZD dseg:BFE6 00 00                                           word_42A86 dw 0                         ; DATA XREF: sub_4D5EA+Bw ...
// WZD dseg:BFE8 00 00                                           IDK_EnemyCityScreen_UnitStack_2 dw 0    ; DATA XREF: Enemy_City_Screen+1D3r ...
// WZD dseg:BFEA 00 00                                           IDK_EnemyCityScreen_UnitStack_1 dw 0    ; DATA XREF: Enemy_City_Screen:loc_4A454r ...
// WZD dseg:BFEC 00 00                                           RecTotal dw 0                           ; DATA XREF: Enemy_City_Screen+68o ...
// WZD dseg:BFEE 00 00                                           start_y dw 0                            ; DATA XREF: Enemy_City_Screen:loc_4A43Fw ...
// WZD dseg:BFF0 00 00                                           IDK_EnemyCityScreen_xstart dw 0         ; DATA XREF: Enemy_City_Screen:loc_4A439w ...



// WZD dseg:BFF2                                                 ¿ BEGIN:  ovr056  Production Screen ?

// WZD dseg:C03C                                                 ¿ END:  ovr056  Production Screen ?



// WZD dseg:C03E                                                 BEGIN: Main_Screen

// WZD dseg:C03E 00 00                                           CRP_OverlandVar_2 dw 0                  ; DATA XREF: Main_Screen+60w
// WZD dseg:C040 00 00                                           CRP_OverlandVar_4 dw 0                  ; DATA XREF: Main_Screen:loc_4FC07r ...
// WZD dseg:C042 00 00                                           OVL_MapVar3 dw 0                        ; DATA XREF: Main_Screen+6Cw ...
// WZD dseg:C044 00 00                                           CRP_OVL_Obstacle_Var1 dw 0              ; DATA XREF: Main_Screen+66w ...
// WZD dseg:C046 00 00                                           CRP_OVL_UU_Control_1 dw 0               ; DATA XREF: MainScreen_Add_Fields+16w ...
// WZD dseg:C048 00 00                                           CRP_OVL_UU_Control_2 dw 0               ; DATA XREF: MainScreen_Add_Fields+10w ...
// WZD dseg:C04A 00 00                                           CRP_OVL_UU_Control_3 dw 0               ; DATA XREF: MainScreen_Add_Fields+Aw ...
// WZD dseg:C04C 00 00                                           CRP_OVL_UU_Control_4 dw 0               ; DATA XREF: MainScreen_Add_Fields+4w ...
// WZD dseg:C04E 00 00                                           special_action_flag dw 0                ; DATA XREF: Main_Screen+727r ...
// WZD dseg:C04E                                                                                         ; set to indicate any extra action possible for the
// WZD dseg:C04E                                                                                         ; selected stack
// WZD dseg:C04E                                                                                         ;   -1: no extra action possible
// WZD dseg:C04E                                                                                         ;    0: road building possible
// WZD dseg:C04E                                                                                         ;    1: settling possible
// WZD dseg:C04E                                                                                         ;    2: purifying possible
// WZD dseg:C04E                                                                                         ;    9: melding possible
// WZD dseg:C050 00 00                                           _next_turn_button dw 0                  ; DATA XREF: Main_Screen:@@Check_Input_NextTurnButtonr ...
// WZD dseg:C052 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+Unit_Window_Fields dw 9 dup(0)          ; DATA XREF: IDK_CityScreen_AddFields+2Cw ...
// WZD dseg:C064 00 00                                           CRP_OverlandVar_3 dw 0                  ; DATA XREF: Main_Screen+9Ar ...
// WZD dseg:C066 00 00                                           OVL_Path_Length dw 0                    ; DATA XREF: Main_Screen+CBDr ...

// WZD dseg:C068 00 00                                           OVL_StackHasPath dw 0                   ; DATA XREF: City_Screen:loc_47BE1w ...
// WZD dseg:C068
extern int16_t OVL_StackHasPath;

// WZD dseg:C068                                                                                         ; set to 0 after display-sorting the active stack
// WZD dseg:C068                                                                                         ; set to 1 if road-building, but the unit is not on any
// WZD dseg:C068                                                                                         ;   of the plotted line tiles (before returning)
// WZD dseg:C068                                                                                         ; set to 1 if road-building, and tiles left to do
// WZD dseg:C068                                                                                         ; set to 1 if moving with path left to go
// WZD dseg:C06A 00 00                                           _done_button dw 0                       ; DATA XREF: Main_Screen:CheckDoneButtonr ...
// WZD dseg:C06C 00 00                                           _wait_button dw 0                       ; DATA XREF: Main_Screen:loc_50419r ...
// WZD dseg:C06E 00 00                                           _patrol_button dw 0                     ; DATA XREF: Main_Screen:loc_50207r ...
// WZD dseg:C070 00 00                                           _special_button dw 0                    ; DATA XREF: Main_Screen:loc_50279r ...
// WZD dseg:C072 00 00                                           _plane_button dw 0                      ; DATA XREF: Main_Screen:loc_5048Br ...
// WZD dseg:C074 00 00                                           _info_button dw 0                       ; DATA XREF: Main_Screen:loc_500CBr ...
// WZD dseg:C076 00 00                                           _magic_button dw 0                      ; DATA XREF: Main_Screen:loc_5010Dr ...
// WZD dseg:C078 00 00                                           _cities_button dw 0                     ; DATA XREF: Main_Screen:loc_500F7r ...
// WZD dseg:C07A 00 00                                           _armies_button dw 0                     ; DATA XREF: Main_Screen:loc_50123r ...
// WZD dseg:C07C 00 00                                           _spells_button dw 0                     ; DATA XREF: Main_Screen:loc_50139r ...
// WZD dseg:C07E 00 00                                           _game_button dw 0                       ; DATA XREF: Main_Screen:loc_500E1r ...
// WZD dseg:C080 00 00                                           dw 0

// WZD dseg:C082
// AKA OVL_STKUnitCards_Top
extern int16_t _unit_window_start_y;

// WZD dseg:C084
// AKA OVL_STKUnitCards_Lft
extern int16_t _unit_window_start_x;

// WZD dseg:C086
extern SAMB_ptr _reduced_map_seg;

// WZD dseg:C088 00 00                                           NIU_MainScreen_local_flag dw 0          ; DATA XREF: Main_Screen:loc_4FB72w ...
// WZD dseg:C088                                                                                         ; drake178: CRP_OverlandVar
// WZD dseg:C088                                                                                         ; only XREF Main_Screen(), sets TRUE, never tests
// WZD dseg:C08A 00 00                                           SND_Bkgrnd_Track dw 0                   ; DATA XREF: IDK_SND_BkGrnd_s518AE+1Er ...
// WZD dseg:C08A                                                                                         ; the current power-based background music track index

// WZD dseg:C08C
extern int16_t city_built_bldg_idx;

// WZD dseg:C08E 00 00                                           STK_HMoves_Left dw 0                    ; DATA XREF: OVL_GetStackHMoves:@@Donew ...
// WZD dseg:C090 00 00                                           dw 0

// WZD dseg:C092 00 00                                           cycle_incomes dw 0                      ; DATA XREF: Main_Screen_Draw_Summary_Window:@@Gold_Negativer ...
// WZD dseg:C092                                                                                         ; -1 draws negative incomes with a static color instead
// WZD dseg:C092
extern int16_t cycle_incomes; //  dw 0                      ; -1 draws negative incomes with a static color instead

// WZD dseg:C094 00 00                                           dw 0
// WZD dseg:C096 00 00                                           dw 0










//                                          ¿ BEGIN: List / ArmyList / CityList - Uninitialized Data ?

// WZD dseg:C098
extern int16_t citylist_item_scanned_field;

// WZD dseg:C09A
// AKA G_Some_OVL_Var_1
extern int16_t IDK_CityList_list_first_item;

// WZD dseg:C09C
extern int16_t citylist_city_count;

// WZD dseg:C09E
extern int16_t citylist_item_count;

// WZD dseg:C0A0
extern int16_t citylist_item_fields[];

// WZD dseg:C0B2 00 00                                           word_42B52 dw 0                         ; DATA XREF: CityList_Screen+4Cw
// WZD dseg:C0B4 00 00                                           word_42B54 dw 0                         ; DATA XREF: CityList_Screen+46w

// WZD dseg:C0B6
extern SAMB_ptr city_list;

// WZD dseg:C0B8
extern SAMB_ptr list_cities;

// WZD dseg:C0BA
extern int16_t armylist_item_scanned_field;

// WZD dseg:C0BC 00 00                                           dw 0
// WZD dseg:C0BE 00 00                                           word_42B5E dw 0                         ; DATA XREF: CityList_Screen_Draw:loc_58114w

// WZD dseg:C0C0
extern int16_t IDK_armylist_row_count;

// WZD dseg:C0C2
extern byte_ptr IDK_armylist_row_fields;

// WZD dseg:C0C4
extern int16_t armylist_army_count;

// WZD dseg:C0C6
extern int16_t list_item_count;

// WZD dseg:C0C8
extern int16_t list_first_item;

// WZD dseg:C0CA
extern int16_t armylist_unit_count;

// WZD dseg:C0CC
// MoO2  Module: COLREFIT  _n_ships_in_list  Address: 02:00197A28
extern int16_t armylist_item_count;

// WZD dseg:C0CE 00 00                                           word_42B6E dw 0                         ; DATA XREF: ArmyList_Screen+2Bw
// WZD dseg:C0D0 00 00                                           word_42B70 dw 0                         ; DATA XREF: ArmyList_Screen+25w

// WZD dseg:C0D2
extern int16_t armylist_hero_portrait_start_y;

// WZD dseg:C0D4
extern int16_t armylist_hero_portrait_start_x;

// WZD dseg:C0D6
extern byte_ptr armylist_hero_portrait_unit_indices;

// WZD dseg:C0D8
extern int16_t armylist_hero_portrait_count;

// WZD dseg:C0DA
extern byte_ptr armylist_hero_portrait_fields;

// WZD dseg:C0DC
extern byte_ptr armylist_unit_fields;

// WZD dseg:C0DE
extern int16_t armylist_start_y;

// WZD dseg:C0E0
extern int16_t armylist_start_x;

// WZD dseg:C0E2
extern int16_t armylist_upkeep_food;

// WZD dseg:C0E4
extern int16_t armylist_upkeep_mana;

// WZD dseg:C0E6
extern int16_t armylist_upkeep_gold;

// WZD dseg:C0E8
extern char * armylist_20x20[20];

// WZD dseg:C110
extern SAMB_ptr list_armies;                       // 108 bytes;  2-byte values, 6 x 9  armies x units;

// WZD dseg:C112
extern byte_ptr armylist_world_p_1000;

// WZD dseg:C114
extern SAMB_ptr armylist_list_item_count;          // 54 bytes;  6 x 9; 

// WZD dseg:C116
extern byte_ptr armylist_world_y_1000;

// WZD dseg:C118
extern byte_ptr armylist_world_x_1000;

// WZD dseg:C11A 00 00                                           dw 0

//                                          ¿ END: List / ArmyList / CityList - Uninitialized Data ?



//                                          ¿ BEGIN: ~ Reduced/World Map - Uninitialized Data ?

// WZD dseg:C11C 00 00                                           minimap_height dw 0                     ; DATA XREF: Draw_Maps:loc_59CF4r ...
// WZD dseg:C11C 00 00                                           reduced_map_height dw 0                 ; DATA XREF: Draw_Maps:loc_59CF4r ...





// WZD dseg:C47C
extern int16_t OVL_Action_Plane;
// WZD dseg:C47E
extern int16_t OVL_Action_YPos;
// WZD dseg:C480
extern int16_t OVL_Action_XPos;



// WZD dseg:C5DC                                                 ¿ BEGIN: Move_Stack() || UNITMOVE ?

// WZD dseg:C5DC
extern int16_t OVL_SWardTriggered;

// WZD dseg:C5DE
// drake178: set to the path tile before the last before moving units overland
extern int16_t OVL_Action_OriginY;

// WZD dseg:C5E0
// drake178: set to the path tile before the last before moving units overland
extern int16_t OVL_Action_OriginX;

// WZD dseg:C5E2 00                                              db    0
// WZD dseg:C5E3 00                                              db    0
// WZD dseg:C5E4 00                                              db    0
// WZD dseg:C5E5 00                                              db    0
// WZD dseg:C5E6 00                                              db    0
// WZD dseg:C5E7 00                                              db    0
// WZD dseg:C5E8 00                                              db    0
// WZD dseg:C5E9 00                                              db    0
// WZD dseg:C5EA 00                                              db    0
// WZD dseg:C5EB 00                                              db    0

// WZD dseg:C5EC 00 00                                           CRP_OVL_Obstacle_Var3 dw 0              ; DATA XREF: Move_Stack+50Dw
// WZD dseg:C5EE 00 00                                           CRP_OVL_Obstacle_Var2 dw 0              ; DATA XREF: Move_Stack+507w

// WZD dseg:C5F0
extern int8_t movepath_cost_array[118];
// WZD dseg:C666
extern int8_t movepath_y_array[120];
// WZD dseg:C6DE
extern int8_t movepath_x_array[120];

// WZD dseg:C794 00                                              STK_HasNatureUnits db 0                 ; DATA XREF: CTY_CheckSpellWard:loc_7E798w ...
// WZD dseg:C795 00                                              STK_HasSorceryUnits db 0                ; DATA XREF: CTY_CheckSpellWard+C3w
// WZD dseg:C796 00                                              STK_HasChaosUnits db 0                  ; DATA XREF: CTY_CheckSpellWard+121w ...
// WZD dseg:C797 00                                              STK_HasLifeUnits db 0                   ; DATA XREF: CTY_CheckSpellWard+1A2w
// WZD dseg:C798 00                                              STK_HasDeathUnits db 0                  ; DATA XREF: CTY_CheckSpellWard+200w ...
// WZD dseg:C799 00                                              UU_Global_Byte db 0
// align 2
// WZD dseg:C799                                                 ¿ END: Move_Stack() || UNITMOVE ?

// WZD dseg:C79A 00 00                                           CMB_HumanTurn dw 0                      ; DATA XREF: CMB_TacticalCombat+26Dw ...


// WZD dseg:C79E
extern SAMB_ptr IMG_USW_HeroPortrt;



// WZD dseg:C7FC
// drake178: GUI_NearMsgString
extern char GUI_NearMsgString[154];





// WZD dseg:CA78                                                 ¿ BEGIN: Cityscape - ?

// WZD dseg:CA78
// drake178: IDK_animate_new_building_idx
// WZD dseg:CA7A
// drake178: dword_4351A

// WZD dseg:CA7E
// drake178: IMG_CTY_Vtrcl_Mask@
extern SAMB_ptr IDK_BUILDS1_vertical_mask_seg;

// WZD dseg:CA80
// drake178: IMG_CTY_Horz_Mask@
extern SAMB_ptr IDK_BUILDS1_horizontal_mask_seg;

// WZD dseg:CA82
// drake178: Frame_Index
// WZD dseg:CA84
// drake178: word_43524
// WZD dseg:CA86
// drake178: word_43526
// WZD dseg:CA88
// drake178: pict_seg

// WZD dseg:CA88                                                 ¿ END: Cityscape - ?





// WZD dseg:CB5C
// AKA OVL_NewMapDrawing
extern int16_t draw_map_full;  //; determines whether non-animated terrain tiles will be redrawn or not





// WZD dseg:D490 00 00                                           UU_AI_TargetingVar dw 0                 ; DATA XREF: AI_ProcessRoamers:loc_ED7F2w
// WZD dseg:D492 00 00                                           dw 0
// WZD dseg:D494 00 00                                           GUI_Help_NearAlloc dw 0                 ; DATA XREF: Draw_Help_Entry_+BBw ...
// WZD dseg:D494                                                                                         ; 1048 bytes of LBX_NearAlloc_First space
// WZD dseg:D496 00 00                                           save_active dw 0                        ; DATA XREF: Load_Screen+D1w ...

// Allocate  // WZD dseg:D498 00 00                                           LBX_NearBuf_Mark dw 0                   ; DATA XREF: LBX_NearAlloc_Mark:loc_150B1w ...
// Allocate  // WZD dseg:D49A
// Allocate  extern char Temp_String[];
// Allocate  // WZD dseg:D4FE 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+Tmp_Conv_Str_1 db 14h dup(0)            ; DATA XREF: LBX_NearAlloc_Error+16o ...
// Allocate  // WZD dseg:D512 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+Tmp_Conv_Str_2 db 1Eh dup(0)            ; DATA XREF: EMM_GetHandle+1A2o ...
// Allocate  // WZD dseg:D530 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+Tmp_Conv_Str_3 db 6Ah dup(   0)         ; DATA XREF: EMM_GetHandle+1B3o ...
// Allocate  // WZD dseg:D59A 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+LBX_Near_Buffer db 1030h dup(   0)

// WZD dseg:E5CA 00 00 00 00                                     fp_tmpSAMB dd 0                         ; DATA XREF: Allocate_Space+28w ...
// WZD dseg:E5CE 00 00                                           RAM_MinKbytes dw 0                      ; DATA XREF: SA_Alloc_Error+2Cr ...
// WZD dseg:E5CE                                                                                         ; set to 583 in _main
// WZD dseg:E5D0 00 00                                           LBX_LastLoadECount dw 0                 ; DATA XREF: LBX_Load_Entry+1C6w ...
// WZD dseg:E5D2 00 00                                           UU_farload_hdr_fmt dw 0                 ; DATA XREF: LBX_Load_Entry:loc_159F1r ...
// WZD dseg:E5D4 00 00                                           farload_lbx_header dw 0                 ; DATA XREF: LBX_Load_Entry+37w ...
// WZD dseg:E5D6 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 farload_file_name db 10h dup(0)         ; DATA XREF: LBX_Load_Entry+80o ...
// WZD dseg:E5E6 00 00                                           Disk_LastByteCount dw 0                 ; DATA XREF: DISK_ReadFile+9w ...
// WZD dseg:E5E8 00 00                                           Disk_LastOpenHandle dw 0                ; DATA XREF: DISK_OpenCreate+2Fw ...
// WZD dseg:E5EA 00 00                                           UU_EMMData_PrevLvl dw 0                 ; DATA XREF: UU_EMM_Data_Mark+6w ...
// WZD dseg:E5EC 00 00                                           g_EmmHndl_OVERXYZ dw 0                  ; DATA XREF: _main+302w ...
// WZD dseg:E5EE 00 00                                           EMM_Data_Level dw 0                     ; DATA XREF: EMM_Startup:loc_16EE9w ...
// WZD dseg:E5EE                                                                                         ; set to 0 in EMM_Startup
// WZD dseg:E5F0 00 00                                           EmmHndlNbr_EMMDATAH dw 0                ; DATA XREF: EMM_Startup+18Bw ...
// WZD dseg:E5F0                                                                                         ; 4 reserved pages
// WZD dseg:E5F2 00 00                                           dw 0
// WZD dseg:E5F4 00 00                                           g_EmmHndl_VGAFILEH dw 0                 ; DATA XREF: EMM_Startup+174w ...
extern byte_ptr _VGAFILEH_seg;
// WZD dseg:E5F4                                                                                         ; 5 reserved pages
// WZD dseg:E5F6 00 00                                           g_EmmRsvd dw 0                          ; DATA XREF: EMM_LBX_EntryLoader:@@EmmHndlNmExistsr ...
// WZD dseg:E5F8 00 00                                           EmmHndlNbr_YOMOMA dw 0                  ; DATA XREF: EMM_Startup+1Bw ...
// WZD dseg:E5F8                                                                                         ; 1 non-reserved page
// WZD dseg:E5FA 00 00                                           EMM_OK dw 0                             ; DATA XREF: EMM_Startup:loc_16E33w ...
// WZD dseg:E5FA                                                                                         ; set to 1 after creating the YO MOMA handle
// WZD dseg:E5FC 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+EMM_Table EMM_Record 28h dup(<0>)       ; DATA XREF: EMM_Startup+8Bt ...
// WZD dseg:E7DC 00 00                                           EMM_MinKB dw 0                          ; DATA XREF: EMS_SetMinKB+6w ...
// WZD dseg:E7DC                                                                                         ; set to 2700 in _main
// WZD dseg:E7DE
// WZD dseg:E7DE                                                 BEGIN:  Fonts
// WZD dseg:E7DE
// WZD dseg:E7DE 00 00                                           gsa_VGAFILEH_Header dw 0                ; DATA XREF: Load_Font_File+78w ...





// WZD dseg:E872                                                 ¿ BEGIN: fields, intput, ... ?
// WZD dseg:E872                                                 ¿ BEGIN: Help - Uninitialized Data ?

// WZD dseg:E872
// AKA _help_list
// extern struct s_HLP_ENTRY * help_struct_pointer;
/*
    ¿ pointer to an array of pointers of data type 'struct s_HLP_ENTRY' ?
*/
// extern struct s_HLP_ENTRY ** help_struct_pointer;
extern struct s_HLP_ENTRY * help_struct_pointer;


// WZD dseg:E872                                                 ¿ END: Help - Uninitialized Data ?
// ...
// ...
// ...
// WZD dseg:82A6                                                 END: Fields, Input, Mouse, Keyboard






#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */



#ifdef __cplusplus
}
#endif


#endif  /* MOX_DATA_H */
