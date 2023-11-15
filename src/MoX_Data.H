#ifndef MOX_DATA_H
#define MOX_DATA_H

#include "MoX_TYPE.H"
#include "MoM_DEF.H"



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
    US_Ready,  /* "NO ORDERS" in 'MoM Demo' */
    US_Patrol,
    US_BuildRoad,
    US_GoingTo,
    US_ReachedDest,
    US_Wait,
    US_Casting,  /* drake178: US_Unknown_6; "CASTING" in 'MoM Demo' */
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

    US_Unknown_100 = 100,  // 64h  100d  01100100b
    US_PurifyDone = 111    // 6Fh  111d  01101111b
};



/*
    Magic Realm
*/

/*
enum Magic Realm
mr_Chaos = 0,
mr_Death = 1,
mr_Life = 2,
mr_Nature = 3,
mr_Sorcery = 4,
*/


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



 // sizeof:  03h  3d
struct LOC_8BIT
{
    /* 1 00 */ int8_t X_Coord;
    /* 2 01 */ int8_t Y_Coord;
    /* 3 02 */ int8_t Plane;
};
// sizeof:  06h  6d
struct s_LOC_16BIT
{
    /* 1 00 */ int16_t X_Coord;
    /* 2 02 */ int16_t Y_Coord;
    /* 3 04 */ int16_t Plane;
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
    /* 2E */  uint32_t Powers;                      // ; enum IPOW_LO
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
    /* 002E */ struct s_LOC_16BIT Summon_To;
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
    /* 005A */ int16_t spellrank_nature;
    /* 005C */ int16_t spellrank_sorcery;
    /* 005E */ int16_t spellrank_chaos;
    /* 0060 */ int16_t spellrank_life;
    /* 0062 */ int16_t spellrank_death;

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
    /* 025E */ uint32_t Casting_Skill;
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

struct s_BUILDDAT
{
    /* 00 */  char name[20];
    /* 14 */  int16_t Prereq_1;         // ; enum BLD_TYPE
    /* 16 */  int16_t Prereq_2;         // ; enum BLD_TYPE
    /* 18 */  int16_t Replaces;         // ; enum BLD_TYPE
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
    /* 00 */ char name[14];
    /* 0E */ int8_t race;
    /* 0F */ int8_t world_x;
    /* 10 */ int8_t world_y;
    /* 11 */ int8_t world_plane;
    /* 12 */ int8_t owner_idx;
    /* 13 */ int8_t size;
    /* 14 */ int8_t population;  // in Units of 1000 (1K)
    /* 15 */ int8_t farmer_count;
    /* 16 */ int8_t Building_Sold;
    /* 17 */ int8_t Unused_17h;
    /* 18 */ int16_t Pop_10s;
    /* 1A */ int8_t PlayerBits;
    /* 1B */ int8_t Unused_1Bh;
    /* 1C */ int16_t construction;  /* < 100 building, >= 100 unit .. 0 is meaning? ... 99 max bldg, 256 - 1 - 99 = 156 max unit types? */
    /* 1E */ int8_t Building_Count;
    /* 1F */ // struct s_CITY_BUILDINGS buildings;
             int8_t buildings[NUM_BUILDINGS];  // ¿ {..., -1, 0, 1, ...} ?
    /* 43 */ // struct s_CITY_ENCHANTMENTS enchantments;
             // uint8_t enchantments[26];  /* ¿ 0 for NONE, (player_idx + 1) for which wizard has which spell on this city ? */
    /* 5C */ // int8_t Nightshade;  as seens in CTY_EnchantsUpkeep() for city_enchantment_upkeep_table
             uint8_t enchantments[NUM_CITY_ENCHANTMENTS];
    /* 5D */ int8_t production_units;
    /* 5E */ int16_t Prod_Accu;
    /* 60 */ int8_t gold_units;  // AKA Income
    /* 61 */ int8_t building_maintenance;  // AKA Upkeep
    /* 62 */ int8_t Power;
    /* 63 */ int8_t research_units;
    /* 64 */ int8_t food_units;
    /* 65 */ int8_t road_connections[13];  // Bit-Field; use Test_Bit_Field(); cit_idx is city_idx;
};

// sizeof:  18h  24d
struct s_LAIR
{
    /* 00 */  int8_t   world_x;
    /* 01 */  int8_t   world_y;
    /* 02 */  int8_t   world_plane;
    /* 03 */  int8_t   Intact;
    /* 04 */  int8_t   Type;
    /* 05 */  int8_t   Defender1_Index;
    /* 06 */  int8_t   Def1_Had_n_Left;
    /* 07 */  int8_t   Defender2_Index;
    /* 08 */  int8_t   Def2_Had_n_Left;
    /* 09 */  int8_t   Unused_09h;
    /* 0A */  int16_t  Loot_Gold;
    /* 0C */  int16_t  Loot_Mana;
    /* 0E */  int8_t   Spell_n_Special;
    /* 0F */  int8_t   Misc_Flags;
    /* 10 */  int8_t   Item_Count;
    /* 11 */  int8_t   Unused_11h;
    /* 12 */  int16_t  Item_Values[3];
    /* 18 */
};

// sizeof:  30h  48d
struct s_NODE
{
    /* 00 */  int8_t  world_x;
    /* 01 */  int8_t  world_y;
    /* 02 */  int8_t  world_plane;
    /* 03 */  int8_t  Owner;
    /* 04 */  int8_t  Power;        // used to index Aura_Xs,Ys and added to node_anim_ctr for FLIC_Set_CurrentFrame()
    /* 05 */  int8_t  Aura_Xs[20];
    /* 19 */  int8_t  Aura_Ys[20];
    /* 2D */  int8_t  Node_Type;
    /* 2E */  int8_t  Meld_Flags;
    /* 2F */  int8_t  Unused_2Fh;
};



// sizeof:  20h  32d
struct s_UNIT
{
    /* 00 */  int8_t  world_x;
    /* 01 */  int8_t  world_y;
    /* 02 */  int8_t  world_plane;
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
    /* 0D */  int8_t  Unused_0Dh;
    /* 0E */  int16_t XP;
    /* 10 */  int8_t  Move_Failed;
    /* 11 */  int8_t  Damage;
    /* 12 */  int8_t  Draw_Priority;
    /* 13 */  int8_t  Unknown_13h;
    /* 14 */  int16_t In_Tower;     /* FALSE/TRUE;  Unit is in a Tower of Wizardry */
    /* 16 */  int8_t  Sight_Range;
    /* 17 */  int8_t  Mutations;    // ; enum MUT_FLAGS
    /* 18 */  int16_t Enchants_LO;  // ; enum UE_FLAGS_L
    /* 1A */  int16_t Enchants_HI;  // ; enum UE_FLAGS_H
    /* 1C */  int8_t  Rd_Constr_Left;
    /* 1D */  int8_t  Rd_From_X;
    /* 1E */  int8_t  Rd_From_Y;
    /* 1F */  int8_t  Unknown_1Fh;
    /* 20 */
};


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
struct s_BU_REC
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
    /* 0x17 */  int8_t Unused_17h;
    /* 0x18 */  int16_t Attribs_1;                          // ; enum ATTRIB_1
    /* 0x1A */  int8_t Attribs_2;
    /* 0x1B */  int8_t Unused_1Bh;
    /* 0x1C */  int16_t Abilities;                          // ; enum ABL_FLAGS
    /* 0x1E */  int16_t Attack_Flags;                       // ; enum ATK_FLAGS
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
    /* 0x57 */  int8_t Unknown_57h;
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
    /* 0x6D */  int8_t Unused_Grey_Hits;
    /* 0x6E */
};




// sizeof=0x4
struct s_TOWER
{
    /* 00 */ int8_t world_x;
    /* 01 */ int8_t world_y;
    /* 02 */ int8_t owner_idx;
    /* 03 */ int8_t Unused_03h;
};

// sizeof=0x4
struct s_FORTRESS
{
    /* 00 */ int8_t world_x;
    /* 01 */ int8_t world_y;
    /* 02 */ int8_t world_plane;
    /* 03 */ int8_t Active;
};


// sizeof 04h 4d
struct s_STACK
{
    /* 00 */ int16_t unit_idx;
    /* 02 */ int16_t active;  // AKA Can_Move
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





// WZD dseg:2A12
extern char builddat_lbx_file[];





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

// WZD dseg:912C                                                                                         ; 215 records of 36 bytes
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
// IMG_USW_Items ITEM_ICONS

// WZD dseg:9226
extern struct s_BU_REC * Active_Unit;                // alloc in Allocate_Data_Space()

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





// WZD dseg:938C
extern struct s_BUILDDAT * build_data_table;






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
// WZD dseg:9498 00 00                                           IMG_CTY_RED_Btn@ dw 0                   ; DATA XREF: City_Screen_Load_Pictures+115w ...
// WZD dseg:9498                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:9498                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:949A 00 00                                           IMG_CTY_RightBldTab@ dw 0               ; DATA XREF: GFX_Swap_AppendUView+1E9w ...
// WZD dseg:949A                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:949C 00 00                                           IMG_CTY_LeftBldTab@ dw 0                ; DATA XREF: GFX_Swap_AppendUView+1D2w ...
// WZD dseg:949C                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:949E 00 00                                           IMG_OVL_EnemyCityBG@ dw 0               ; DATA XREF: IDK_EnemyCityScreen_s4A3F0+115w ...
// WZD dseg:94A0 00 00                                           IMG_CTY_10_Food@ dw 0                   ; DATA XREF: City_Screen_Load_Pictures+1EDw ...
// WZD dseg:94A0                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:94A2 00 00                                           IMG_CTY_10_Prod@ dw 0                   ; DATA XREF: sub_4DA19+2CEr
// WZD dseg:94A2                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:94A4 00 00                                           IMG_CTY_10_Gold@ dw 0                   ; DATA XREF: sub_4DA19+484r ...
// WZD dseg:94A4                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:94A6 00 00                                           IMG_CTY_10_Power@ dw 0                  ; DATA XREF: sub_4DA19+7A6r ...
// WZD dseg:94A6                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:94A8 00 00                                           IMG_CTY_10_Books@ dw 0                  ; DATA XREF: sub_4DA19+9B2r
// WZD dseg:94A8                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:94AA 00 00                                           IMG_CTY_Neg_10_Gold@ dw 0               ; DATA XREF: IDK_CityScreen_AddResourcesFields+44Er ...
// WZD dseg:94AA                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:94AC 00 00                                           IMG_CTY_Neg_10_Food@ dw 0               ; DATA XREF: City_Screen_Load_Pictures+20Bw ...
// WZD dseg:94AE 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+IMG_CTY_Rebels@ dw 0Eh dup(0)           ; DATA XREF: City_Screen_Load_Pictures+186w ...
// WZD dseg:94AE 00 00 00 00 00 00 00 00 00 00 00 00                                                     ; array of 14 appended reserved EMM headers in
// WZD dseg:94AE                                                                                         ; GFX_Swap_Seg, 1 rebel image per header
// WZD dseg:94CA 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+IMG_CTY_Farmers@ dw 0Eh dup(0)          ; DATA XREF: City_Screen_Load_Pictures+15Ew ...
// WZD dseg:94CA 00 00 00 00 00 00 00 00 00 00 00 00                                                     ; array of 14 appended reserved EMM headers in
// WZD dseg:94CA                                                                                         ; GFX_Swap_Seg, 1 farmer image per header
// WZD dseg:94E6 00 00                                           IMG_CTY_1_Food@ dw 0                    ; DATA XREF: City_Screen_Load_Pictures+1AEw ...
// WZD dseg:94E6                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:94E8 00 00                                           IMG_CTY_1_Prod@ dw 0                    ; DATA XREF: sub_4DA19+2D2r
// WZD dseg:94E8                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:94EA 00 00                                           IMG_CTY_1_Gold@ dw 0                    ; DATA XREF: sub_4DA19+488r ...
// WZD dseg:94EA                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:94EC 00 00                                           IMG_CTY_1_Power@ dw 0                   ; DATA XREF: sub_4DA19+7AAr ...
// WZD dseg:94EC                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:94EE 00 00                                           IMG_CTY_1_Book@ dw 0                    ; DATA XREF: sub_4DA19+9B6r
// WZD dseg:94EE                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:94F0 00 00                                           IMG_CTY_Neg_1_Food@ dw 0                ; DATA XREF: City_Screen_Load_Pictures+1CCw ...
// WZD dseg:94F2 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+IMG_CTY_Workers@ dw 0Eh dup(0)          ; DATA XREF: City_Screen_Load_Pictures+136w ...
// WZD dseg:94F2 00 00 00 00 00 00 00 00 00 00 00 00                                                     ; array of 14 appended reserved EMM headers in
// WZD dseg:94F2                                                                                         ; GFX_Swap_Seg, 1 worker image per header
// WZD dseg:950E                                                 BEGIN: CityList_Screen_Load()
// WZD dseg:950E 00 00                                           citylist_ok_button_seg dw 0             ; DATA XREF: CityList_Screen+101r ...
// WZD dseg:950E                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:9510 00 00                                           citylist_down_button_seg dw 0           ; DATA XREF: CityList_Screen+1FFr ...
// WZD dseg:9510                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:9512 00 00                                           citylist_up_button_seg dw 0             ; DATA XREF: CityList_Screen+16Er ...
// WZD dseg:9512                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:9514 00 00                                           citylist_background_seg dw 0            ; DATA XREF: CityList_Screen:loc_5767Dr ...
// WZD dseg:9514                                                 END: CityList_Screen_Load()
// WZD dseg:9516
// WZD dseg:9516
// WZD dseg:9516
// WZD dseg:9516                                                 BEGIN:  Main Screen Pictures
// WZD dseg:9516
// WZD dseg:9516
// WZD dseg:9516
// WZD dseg:9516 00 00                                           next_turn_button_seg dw 0               ; DATA XREF: Main_Screen_Load_Pictures+299w ...
// WZD dseg:9516                                                                                         ; single-loaded image
// WZD dseg:9518 00 00                                           survey_background dw 0                  ; DATA XREF: Main_Screen_Load_Pictures+34Dw ...
// WZD dseg:9518                                                                                         ; single-loaded image
// WZD dseg:951A 00 00                                           road_button_border dw 0                 ; DATA XREF: Main_Screen_Load_Pictures+33Bw ...
// WZD dseg:951A                                                                                         ; single-loaded image
// WZD dseg:951C 00 00                                           road_background dw 0                    ; DATA XREF: Main_Screen_Load_Pictures+317w ...
// WZD dseg:951C                                                                                         ; single-loaded image
// WZD dseg:951E 00 00                                           road_ok_button dw 0                     ; DATA XREF: Main_Screen_Load_Pictures+329w ...
// WZD dseg:951E                                                                                         ; single-loaded 2 frame image (normal - clicked)
// WZD dseg:9520 00 00                                           dw 0
// WZD dseg:9522 00 00                                           DESELECT_button_blockout dw 0           ; DATA XREF: Main_Screen_Load_Pictures+305w ...
// WZD dseg:9522                                                                                         ; single-loaded image
// WZD dseg:9524 00 00                                           cast_cancel_border dw 0                 ; DATA XREF: Main_Screen_Load_Pictures+2F3w ...
// WZD dseg:9524                                                                                         ; single-loaded image
// WZD dseg:9526 00 00                                           cast_cancel_button dw 0                 ; DATA XREF: Main_Screen_Load_Pictures+2E1w ...
// WZD dseg:9526                                                                                         ; single-loaded 2 frame image (normal - clicked)
// WZD dseg:9528 00 00                                           cast_background dw 0                    ; DATA XREF: Main_Screen_Load_Pictures+2CFw ...
// WZD dseg:9528                                                                                         ; single-loaded image
// WZD dseg:952A 00 00                                           dw 0
// WZD dseg:952C 00 00                                           next_turn_background_seg dw 0           ; DATA XREF: Main_Screen_Load_Pictures:loc_466E8w ...
// WZD dseg:952C                                                                                         ; single-loaded image
// WZD dseg:952E 00 00                                           deselect_background dw 0                ; DATA XREF: Main_Screen_Load_Pictures+2ABw ...
// WZD dseg:952E                                                                                         ; single-loaded image
// WZD dseg:952E
// WZD dseg:952E
// WZD dseg:952E                                                 END:  Main Screen Pictures
// WZD dseg:952E
// WZD dseg:952E
// WZD dseg:952E
// WZD dseg:9530 00 00 00 00 00 00                               IMG_CTY_EmptyHuts@ dw 3 dup(0)          ; DATA XREF: City_Screen_Load_Pictures:loc_46A47w ...
// WZD dseg:9530                                                                                         ; array of 3 appended res EMM hdrs in GFX_Swap_Seg
// WZD dseg:9536 00 00 00 00 00 00                               IMG_CTY_FilledHuts@ dw 3 dup(0)         ; DATA XREF: City_Screen_Load_Pictures+25Aw ...
// WZD dseg:9536                                                                                         ; array of 3 appended res EMM hdrs in GFX_Swap_Seg
// WZD dseg:953C 00 00                                           namecity_background_seg dw 0            ; DATA XREF: City_Screen_Load_Pictures+239w ...
// WZD dseg:953E 00 00                                           IMG_CTY_Outpost_BG@ dw 0                ; DATA XREF: City_Screen_Load_Pictures+222w ...
// WZD dseg:9540 00 00                                           IMG_USW_SideBtns_BG@ dw 0               ; DATA XREF: GFX_Swap_AppendUView+8Bw ...
// WZD dseg:9542 00 00                                           dw 0
// WZD dseg:9544 00 00                                           dw 0
// WZD dseg:9546 00 00                                           dw 0
// WZD dseg:9548 00 00                                           IMG_OVL_BuildBtn_BG@ dw 0               ; DATA XREF: GFX_Swap_AppendUView+176w
// WZD dseg:9548                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:954A 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+mirrow_screen_18_books_icons dw 12h dup(0)
// WZD dseg:954A 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; DATA XREF: IDK_MirrorScreen_s6343B+88w ...
// WZD dseg:956E 00 00                                           mirror_screen_background dw 0           ; DATA XREF: Main_Screen_Load_Pictures+35Fw ...
// WZD dseg:956E                                                                                         ; single-loaded image
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
// WZD dseg:95F0 00 00                                           IMG_CTY_HalfRsc@ dw 0                   ; DATA XREF: City_Screen_Load_Pictures+2E4w ...
// WZD dseg:95F0                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
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
// WZD dseg:9644 00 00                                           _city_idx dw 0                          ; DATA XREF: City_Screen+55r ...





// dseg:9644 00 00                                           _city_idx dw 0                          ; DATA XREF: City_Screen+55r ...
// dseg:9646 00                                              db    0
// dseg:9647 00                                              db    0

// WZD dseg:9648
// MoO2:  _draw_ship_bitmap 0018A83C  _ship_bitmap 0018A840
extern SAMB_ptr UnitDraw_WorkArea;

// dseg:964A 00                                              db    0
// dseg:964B 00                                              db    0

// WZD dseg:964C
// MoO2: ~== _ship_node
extern int16_t entities_on_movement_map[];


// dseg:973C 00 00                                           gsa_BACKGRND_3_IMG_CMB_Bottom_BG dw 0   ; DATA XREF: Load_Combat_Background_Bottom+12w ...












// WZD dseg:92B6 00 00                                           
// IMG_USW_GrassBase@ dw 0                 ; DATA XREF: GFX_Swap_AppndCtScap+57Ew ...
extern SAMB_ptr IMG_USW_GrassBase;



// WZD dseg:9498 00 00                                           
// IMG_CTY_RED_Btn@ dw 0                   ; DATA XREF: City_Screen_Load_Pictures+115w ...
extern SAMB_ptr red_button_seg;
// WZD dseg:9498                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:9498                                                                                         ; 2 frame image (normal - clicked)

// WZD dseg:949A 00 00                                           
// IMG_CTY_RightBldTab@ dw 0               ; DATA XREF: GFX_Swap_AppendUView+1E9w ...
extern SAMB_ptr IMG_CTY_RightBldTab;
// WZD dseg:949A                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

// WZD dseg:949C 00 00                                           
// IMG_CTY_LeftBldTab@ dw 0                ; DATA XREF: GFX_Swap_AppendUView+1D2w ...
extern SAMB_ptr IMG_CTY_LeftBldTab;
// WZD dseg:949C                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

// WZD dseg:949E 00 00                                           IMG_OVL_EnemyCityBG@ dw 0               ; DATA XREF: IDK_EnemyCityScreen_s4A3F0+115w ...

// WZD dseg:94A0 00 00                                           
// IMG_CTY_10_Food@ dw 0                   ; DATA XREF: City_Screen_Load_Pictures+1EDw ...
extern SAMB_ptr IMG_CTY_10_Food[6];
// WZD dseg:94A0                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

// WZD dseg:94A2 00 00                                           IMG_CTY_10_Prod@ dw 0                   ; DATA XREF: sub_4DA19+2CEr
// WZD dseg:94A2                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:94A4 00 00                                           IMG_CTY_10_Gold@ dw 0                   ; DATA XREF: sub_4DA19+484r ...
// WZD dseg:94A4                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:94A6 00 00                                           IMG_CTY_10_Power@ dw 0                  ; DATA XREF: sub_4DA19+7A6r ...
// WZD dseg:94A6                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:94A8 00 00                                           IMG_CTY_10_Books@ dw 0                  ; DATA XREF: sub_4DA19+9B2r
// WZD dseg:94A8                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:94AA 00 00                                           IMG_CTY_Neg_10_Gold@ dw 0               ; DATA XREF: IDK_CityScreen_AddResourcesFields+44Er ...
// WZD dseg:94AA                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

// WZD dseg:94AC 00 00                                           
// IMG_CTY_Neg_10_Food@ dw 0               ; DATA XREF: City_Screen_Load_Pictures+20Bw ...
extern SAMB_ptr IMG_CTY_Neg_10_Food;

// WZD dseg:94AE 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+IMG_CTY_Rebels@ dw 0Eh dup(0)           ; DATA XREF: City_Screen_Load_Pictures+186w ...
// WZD dseg:94AE 00 00 00 00 00 00 00 00 00 00 00 00                                                     ; array of 14 appended reserved EMM headers in
// WZD dseg:94AE                                                                                         ; GFX_Swap_Seg, 1 rebel image per header
// WZD dseg:94CA 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+IMG_CTY_Farmers@ dw 0Eh dup(0)          ; DATA XREF: City_Screen_Load_Pictures:loc_46923w ...
// WZD dseg:94CA 00 00 00 00 00 00 00 00 00 00 00 00                                                     ; array of 14 appended reserved EMM headers in
// WZD dseg:94CA                                                                                         ; GFX_Swap_Seg, 1 farmer image per header

// WZD dseg:94E6 00 00                                           
// IMG_CTY_1_Food@ dw 0                    ; DATA XREF: City_Screen_Load_Pictures+1AEw ...
extern SAMB_ptr IMG_CTY_1_Food[5];
// WZD dseg:94E6                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

// WZD dseg:94E8 00 00                                           IMG_CTY_1_Prod@ dw 0                    ; DATA XREF: sub_4DA19+2D2r
// WZD dseg:94E8                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:94EA 00 00                                           IMG_CTY_1_Gold@ dw 0                    ; DATA XREF: sub_4DA19+488r ...
// WZD dseg:94EA                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:94EC 00 00                                           IMG_CTY_1_Power@ dw 0                   ; DATA XREF: sub_4DA19+7AAr ...
// WZD dseg:94EC                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:94EE 00 00                                           IMG_CTY_1_Book@ dw 0                    ; DATA XREF: sub_4DA19+9B6r
// WZD dseg:94EE                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

// WZD dseg:94F0 00 00                                           
// IMG_CTY_Neg_1_Food@ dw 0                ; DATA XREF: City_Screen_Load_Pictures+1CCw ...
extern SAMB_ptr IMG_CTY_Neg_1_Food;



// WZD dseg:952E                                                 END:  Main Screen Pictures
// WZD dseg:952E
// WZD dseg:952E
// WZD dseg:952E
// WZD dseg:9530 00 00 00 00 00 00                               IMG_CTY_EmptyHuts@ dw 3 dup(0)          ; DATA XREF: City_Screen_Load_Pictures:loc_46A47w ...
// WZD dseg:9530                                                                                         ; array of 3 appended res EMM hdrs in GFX_Swap_Seg
// WZD dseg:9536 00 00 00 00 00 00                               IMG_CTY_FilledHuts@ dw 3 dup(0)         ; DATA XREF: City_Screen_Load_Pictures+25Aw ...
// WZD dseg:9536                                                                                         ; array of 3 appended res EMM hdrs in GFX_Swap_Seg
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



// WZD dseg:9750                                                 ¿ END: maps / map grid fields - Uninitialized Data ?
// WZD dseg:9750
// WZD dseg:9752
// WZD dseg:9752                                                 ? BEGIN: City Screen ?
// WZD dseg:9752
// WZD dseg:9752 00 00                                           scanned_field__G_CTY_ClickedLabel dw 0  ; DATA XREF: City_Screen+2A9w ...

// WZD dseg:9754 00 00                                           
// IMG_OVL_UnitListBtm@ dw 0               ; DATA XREF: GFX_Swap_AppendUView+15Fw ...
extern SAMB_ptr IMG_OVL_UnitListBtm;
// WZD dseg:9754                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

// WZD dseg:9756 00 00                                           
// IMG_OVL_UnitList_BG@ dw 0               ; DATA XREF: GFX_Swap_AppendUView+148w ...
extern SAMB_ptr IMG_OVL_UnitList_BG;
// WZD dseg:9756                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

// WZD dseg:9758 00 00                                           IMG_CTY_NewBuild_BG@ dw 0               ; DATA XREF: City_Screen_Load_Pictures+74w ...
// WZD dseg:9758                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:975A 00 00                                           IMG_CTY_SplScrlDn@ dw 0                 ; DATA XREF: City_Screen_Load_Pictures+FEw ...
// WZD dseg:975A                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:975A                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:975C 00 00                                           IMG_CTY_SplScrlUp@ dw 0                 ; DATA XREF: City_Screen_Load_Pictures+E7w ...
// WZD dseg:975C                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:975C                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:975E 00 00 00 00 00 00 00 00 00 00 00 00             IMG_USW_ItemSlots ISLT_ICONS <0>        ; DATA XREF: GFX_Swap_AppendItems+15Bw ...
// WZD dseg:975E                                                                                         ; array of 6 appended reserved EMM headers in
// WZD dseg:975E                                                                                         ; GFX_Swap_Seg, each with one item slot image
// WZD dseg:976A 00 00                                           IMG_USW_ItemHelpBlt@ dw 0               ; DATA XREF: GFX_Swap_AppendItems+144w ...
// WZD dseg:976A                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:976C 00 00                                           IMG_USW_ItemHelp_BG@ dw 0               ; DATA XREF: GFX_Swap_AppendItems+12Dw ...
// WZD dseg:976C                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:976E 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+IMG_USW_ItemPowers IPOW_ICONS <0>       ; DATA XREF: GFX_Swap_AppendItems+40w ...
// WZD dseg:976E 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; array of 22 appended reserved EMM headers in
// WZD dseg:976E 00 00 00 00 00 00 00 00 00 00 00 00                                                     ; GFX_Swap_Seg, each with one item power image
// WZD dseg:979A 00 00                                           IMG_MOODWIZPortrait@ dw 0               ; DATA XREF: IDK_DiplAnim_s6FDA1+4Cr ...
// WZD dseg:979A                                                                                         ; 3 frame image (good, bad, neutral)

// WZD dseg:979C 00 00                                           
// IMG_USW_WaterBase@ dw 0                 ; DATA XREF: GFX_Swap_AppndCtScap+567w ...
extern SAMB_ptr IMG_USW_WaterBase;
// WZD dseg:979C                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

// WZD dseg:979E 00 00                                           
// IMG_USW_Portrt_Brdr@ dw 0               ; DATA XREF: GFX_Swap_AppendUView+200w ...
extern SAMB_ptr IMG_USW_Portrt_Brdr;
// WZD dseg:979E                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

// WZD dseg:97A0 00 00                                           
// IMG_USW_1_Gold@ dw 0                    ; DATA XREF: GFX_Swap_AppendUView+1BBw ...
extern SAMB_ptr IMG_USW_1_Gold;
// WZD dseg:97A0                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:97A0                                                                                         ; should have put this in the dseg:94ea pointer

// WZD dseg:97A2 00 00                                           
// IMG_CTY_Neg_1_Gold@ dw 0                ; DATA XREF: GFX_Swap_AppndCtScap+619w ...
extern SAMB_ptr IMG_CTY_Neg_1_Gold;
// WZD dseg:97A2                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

// WZD dseg:97A4 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
// IMG_USW_AbBorders@ dw 8 dup(0)          ; DATA XREF: GFX_Swap_AppendUView+DAw ...
extern SAMB_ptr IMG_USW_AbBorders[8];
// WZD dseg:97A4                                                                                         ; array of 8 appended reserved EMM headers in
// WZD dseg:97A4                                                                                         ; GFX_Swap_Seg, one for around each list slot

// WZD dseg:97B4 00 00                                           
// IMG_USW_ArrowDown@ dw 0                 ; DATA XREF: GFX_Swap_AppendUView+B9w ...
extern SAMB_ptr unitview_down_arrow_seg;
// WZD dseg:97B4                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:97B4                                                                                         ; 2 images (normal - clicked)

// WZD dseg:97B6 00 00                                           
// IMG_USW_ArrowUp@ dw 0                   ; DATA XREF: GFX_Swap_AppendUView+A2w ...
extern SAMB_ptr unitview_up_arrow_seg;
// WZD dseg:97B6                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:97B6                                                                                         ; 2 images (normal - clicked)

// WZD dseg:97B8 00 00                                           
// IMG_USW_Background@ dw 0                ; DATA XREF: GFX_Swap_AppendUView+74w ...
extern SAMB_ptr unitview_large_background_seg;
// WZD dseg:97B8                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

// WZD dseg:97BA 00 00                                           
// IMG_USW_UnitHire_BG@ dw 0               ; DATA XREF: GFX_Swap_AppendUView+5Dw ...
extern SAMB_ptr unitview_small_background_seg;
// WZD dseg:97BA                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

// WZD dseg:97BC 00 00                                           
// IMG_USW_Stats_Gold dw 0                 ; DATA XREF: GFX_Swap_AppendUView+12Aw ...
extern SAMB_ptr IMG_USW_Stats_Gold[15];

// WZD dseg:97DA 00 00                                           
// IMG_USW_Stat_Icons dw 0                 ; DATA XREF: GFX_Swap_AppendUView+102w ...
extern SAMB_ptr IMG_USW_Stat_Icons[15];
// WZD dseg:97DA                                                                                         ; array of 15 appended reserved EMM header in
// WZD dseg:97DA                                                                                         ; GFX_Swap_Seg, each with one regular attribute image

// WZD dseg:97F8 00                                              
// special_seg db    0                     ; DATA XREF: GFX_Swap_AppendUView+1Dw ...
extern SAMB_ptr special_seg[111];
// WZD dseg:97F8                                                                                         ; array of 145 appended reserved EMM headers in
// WZD dseg:97F8                                                                                         ; GFX_Swap_Seg, each with one image
// WZD dseg:97F8                                                                                         ; UU_Guises@ is also used to hold the diplomacy mirror
// WZD dseg:97F8                                                                                         ; image (BACKGRND.LBX entry 18, 5740 bytes in sandbox)

// WZD dseg:98D6 00                                              
// special2_seg db    0                    ; DATA XREF: GFX_Swap_AppendUView+40w
extern SAMB_ptr special2_seg[34];

// WZD dseg:991A 00 00                                           IMG_OVL_EZConfirmBG@ dw 0               ; DATA XREF: City_Screen_Load_Pictures+2A0w ...
// WZD dseg:991A                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:991C 00 00                                           IMG_OVL_EZBtm_BG@ dw 0                  ; DATA XREF: City_Screen_Load_Pictures+2CEw ...
// WZD dseg:991C                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:991E 00 00                                           IMG_OVL_EZBtn_BG@ dw 0                  ; DATA XREF: City_Screen_Load_Pictures+2B7w ...
// WZD dseg:991E                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:9920 00 00                                           CRP_DBG_Alt_T_State dw 0                ; DATA XREF: Main_Screen+47Fr ...

// WZD dseg:9920                                                 ? END: City Screen ?

// WZD dseg:9922                                                 BEGIN:  Main Screen Pictures




















// dseg:998A END:  Main Screen Pictures

// WZD dseg:998C
// GUI_SmallWork_IMG@ dw 0

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
extern uint8_t * _events_table;  // alloc in Allocate_Data_Space()  7 PR 112 B  type? 1-byte,2-byte signed,unsigned 112 events or 56 events?

// WZD dseg:999C
extern int16_t _unit_stack_count;
// WZD dseg:999E
extern struct s_STACK _unit_stack[];


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
extern SAMB_ptr TBL_TempMoveMap_EMS;
// WZD dseg:9CA8
// COL_MinimapTiles
// WZD dseg:9CAC
extern SAMB_ptr TBL_MoveMaps_EMS;


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
// extern SAMB_ptr TBL_Towers;
// extern struct s_TOWER _TOWERS[];
extern struct s_TOWER * _TOWERS;

// WZD dseg:9CC8
// extern SAMB_ptr TBL_Fortresses;
extern struct s_FORTRESS * _FORTRESSES;

// WZD dseg:9CCC
extern struct s_NODE * TBL_Nodes;
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

// WZD dseg:9D18 00 00                                           IMG_CTY_Bldngs_Wall@ dw 0               ; DATA XREF: GFX_Swap_AppndCtScap+602w ...
// WZD dseg:9D18                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

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
extern SAMB_ptr TBL_Hero_Names;  // 25h 37d PR 37*16=592
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







// WZD dseg:BF9A
extern int16_t CTY_EnchantCount;

// WZD dseg:BF9C
extern char * CTY_EnchantOwners;

// WZD dseg:BF9E
extern char * CTY_EnchantList;




// WZD dseg:BFB6 00 00                                           production_screen_return_screen dw 0    ; DATA XREF: City_Screen+657w ...
// WZD dseg:BFB6                                                                                         ; {1: CityList Screen, 2: City Screen}
// WZD dseg:BFB8 00 00                                           dw 0

// WZD dseg:BFBA
extern char * GUI_String_2;

// WZD dseg:BFBC
extern char * GUI_String_1;





// WZD dseg:C07E 00 00                                           _game_button dw 0                       ; DATA XREF: Main_Screen:loc_500E1r ...
// WZD dseg:C080 00 00                                           dw 0

// WZD dseg:C082 00 00                                           _unit_window_start_y dw 0               ; DATA XREF: GAME_Overland_Init+130w ...
// WZD dseg:C082                                                                                         ; set to 4Fh
// WZD dseg:C082
// AKA OVL_STKUnitCards_Top
extern int16_t _unit_window_start_y;

// WZD dseg:C084 00 00                                           _unit_window_start_x dw 0               ; DATA XREF: GAME_Overland_Init+12Aw ...
// WZD dseg:C084                                                                                         ; set to 0F7h
// WZD dseg:C084
// AKA OVL_STKUnitCards_Lft
extern int16_t _unit_window_start_x;

// WZD dseg:C086 00 00                                           _reduced_map_seg dw 0                   ; DATA XREF: sub_49F38+15w ...
// WZD dseg:C086                                                                                         ; 12Fh LBX_Alloc_First paragraphs in the
// WZD dseg:C086                                                                                         ; LBX_Sandbox_Segment
// WZD dseg:C086
extern SAMB_ptr _reduced_map_seg;
// ; 12Fh LBX_Alloc_First paragraphs in the LBX_Sandbox_Segment

// WZD dseg:C088 00 00                                           NIU_MainScreen_local_flag dw 0          ; DATA XREF: Main_Screen:loc_4FB72w ...
// WZD dseg:C088                                                                                         ; drake178: CRP_OverlandVar
// WZD dseg:C088                                                                                         ; only XREF Main_Screen(), sets TRUE, never tests
// WZD dseg:C08A 00 00                                           SND_Bkgrnd_Track dw 0                   ; DATA XREF: IDK_SND_BkGrnd_s518AE+1Er ...
// WZD dseg:C08A                                                                                         ; the current power-based background music track index
// WZD dseg:C08C 00 00                                           word_42B2C dw 0                         ; DATA XREF: sub_499B1+E0r ...
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

// WZD dseg:C5DC 00 00                                           OVL_SWardTriggered dw 0                 ; DATA XREF: Move_Stack+42Cw ...

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
extern uint8_t OVL_Path_Costs[118];
// WZD dseg:C666
extern uint8_t Fst_Dst_Y;
// WZD dseg:C667
extern uint8_t Scd_Dst_Y;
// WZD dseg:C668
extern uint8_t MovePath_Y[118];
// WZD dseg:C6DE
extern uint8_t Fst_Dst_X;
// WZD dseg:C6DF
extern uint8_t Scd_Dst_X;
// WZD dseg:C6E0
extern uint8_t MovePath_X[118];
// WZD dseg:C756
extern uint8_t IDK_MovePath[62];



// WZD dseg:C794 00                                              STK_HasNatureUnits db 0                 ; DATA XREF: CTY_CheckSpellWard:loc_7E798w ...
// WZD dseg:C795 00                                              STK_HasSorceryUnits db 0                ; DATA XREF: CTY_CheckSpellWard+C3w
// WZD dseg:C796 00                                              STK_HasChaosUnits db 0                  ; DATA XREF: CTY_CheckSpellWard+121w ...
// WZD dseg:C797 00                                              STK_HasLifeUnits db 0                   ; DATA XREF: CTY_CheckSpellWard+1A2w
// WZD dseg:C798 00                                              STK_HasDeathUnits db 0                  ; DATA XREF: CTY_CheckSpellWard+200w ...
// WZD dseg:C799 00                                              UU_Global_Byte db 0

// WZD dseg:C799                                                 ¿ END: Move_Stack() || UNITMOVE ?

// WZD dseg:C79A 00 00                                           CMB_HumanTurn dw 0                      ; DATA XREF: CMB_TacticalCombat+26Dw ...


// WZD dseg:C79E
extern SAMB_ptr IMG_USW_HeroPortrt;





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
