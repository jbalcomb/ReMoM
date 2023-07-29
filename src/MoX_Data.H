#ifndef MOX_DATA_H
#define MOX_DATA_H

#include "MoX_TYPE.H"


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
    US_Ready,
    US_Patrol,
    US_BuildRoad,
    US_GoingTo,
    US_ReachedDest,
    US_Wait,
    US_Unknown_6,
    US_Unknown_7,
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



// sizeof=0xA
struct s_BOOKSHELF
{
    /* 00 */ int16_t Nature;
    /* 02 */ int16_t Sorcery;
    /* 04 */ int16_t Chaos;
    /* 06 */ int16_t Life;
    /* 08 */ int16_t Death;
};

// sizeof=0x12
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

// sizeof:  1Ch  28d
struct s_HIRED_HERO
{
    /* 00 */ int16_t Unit_Index;
    /* 02 */ char Name[14];
    /* 10 */ int16_t Items[3];
    /* 16 */ int16_t Item_Slots[3];
};

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
    /* 0000 */ uint8_t Portrait;
    /* 0001 */ char Name[20];
    /* 0015 */ uint8_t Cptl_Race;
    /* 0016 */ uint8_t Banner;
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
    /* 0058 */ uint16_t Tax_Rate;

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

    /* 0076 */ struct s_HIRED_HERO Heroes[6];
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
    /* 0482 */ struct s_GLOBALE Globals;
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



// sizeof=0x24
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




// sizeof=0x19
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
};



// sizeof=0x72
struct s_CITY
{
    /* 00 */ char name[14];
    /* 0E */ int8_t race;
    /* 0F */ int8_t world_x;
    /* 10 */ int8_t world_y;
    /* 11 */ int8_t world_plane;
    /* 12 */ int8_t owner_idx;
    /* 13 */ int8_t size;
    /* 14 */ int8_t Pop_K;
    /* 15 */ int8_t farmer_count;
    /* 16 */ int8_t Building_Sold;
    /* 17 */ int8_t Unused_17h;
    /* 18 */ int16_t Pop_10s;
    /* 1A */ int8_t PlayerBits;
    /* 1B */ int8_t Unused_1Bh;
    /* 1C */ int16_t Producing;
    /* 1E */ int8_t Building_Count;
    /* 1F */ struct s_CITY_BUILDINGS buildings;
    /* 43 */ struct s_CITY_ENCHANTMENTS enchantments;
    /* 5C */ int8_t Nightshade;
    /* 5D */ int8_t Production;
    /* 5E */ int16_t Prod_Accu;
    /* 60 */ int8_t Gold;
    /* 61 */ int8_t Upkeep;
    /* 62 */ int8_t Power;
    /* 63 */ int8_t Research;
    /* 64 */ int8_t Food;
    /* 65 */ int8_t RoadConn_BitField[13];
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




// sizeof=0x20
struct s_UNIT
{
    /* 00 */  int8_t  world_x;
    /* 01 */  int8_t  world_y;
    /* 02 */  int8_t  world_plane;
    /* 03 */  int8_t  owner_idx;
    /* 04 */  int8_t  HalfMoves_Max;
    /* 05 */  uint8_t type;
    /* 06 */  int8_t  Hero_Slot;
    /* 07 */  int8_t  Finished;
    /* 08 */  int8_t  HMoves;
    /* 09 */  int8_t  Target_X;
    /* 0A */  int8_t  Target_Y;
    /* 0B */  int8_t  Status;
    /* 0C */  int8_t  Level;
    /* 0D */  int8_t  Unused_0Dh;
    /* 0E */  int16_t XP;
    /* 10 */  int8_t  Move_Failed;
    /* 11 */  int8_t  Damage;
    /* 12 */  int8_t  Draw_Priority;
    /* 13 */  int8_t  Unknown_13h;
    /* 14 */  int16_t In_Tower;
    /* 16 */  int8_t  Sight_Range;
    /* 17 */  int8_t  Mutations;  // ; enum MUT_FLAGS
    /* 18 */  int16_t Enchants_LO;  // ; enum UE_FLAGS_L
    /* 1A */  int16_t Enchants_HI;  // ; enum UE_FLAGS_H
    /* 1C */  int8_t  Rd_Constr_Left;
    /* 1D */  int8_t  Rd_From_X;
    /* 1E */  int8_t  Rd_From_Y;
    /* 1F */  int8_t  Unknown_1Fh;
    /* 20 */
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



// // sizeof 0Ah 10d
// struct s_HLP_ENTRY
// {
//     int16_t help_idx;
//     int16_t x1;
//     int16_t y1;
//     int16_t x2;
//     int16_t y2;
// };
// 
// // sizeof 0418h 1048d
// struct HLP_DATA
// {
//     char Title[30];
//     char IMG_File[14];
//     int16_t IMG_Entry;
//     int16_t Include_Entry;
//     char Help_Text[1000];
// }





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





// MGC dseg:52C6
// AKA gsa_WIZARDS_0to13
extern SAMB_ptr wizard_portrait_segs[];  // ¿ here, because used by MGC Newgame_Screen(), but, also used by WZD Magic_Screen() ? DSEG in WZD?





// WZD dseg:599C
// TODO  extern char hlpentry_lbx_file[];



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
// WZD dseg:912A
// WZD dseg:912A                                                 ¿ BEGIN: ?
// WZD dseg:912A
// WZD dseg:912A 00 00                                           g_TimeStop_PlayerNum dw 0               ; DATA XREF: Loaded_Game_Update_WZD+C5w ...
// WZD dseg:912A                                                                                         ; index of the Time Stop wizard plus 1 (0 if none)

// WZD dseg:912C
extern uint8_t * spell_data_table;

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
extern SAMB_ptr TBL_Items;
// WZD dseg:913A
extern SAMB_ptr TBL_Premade_Items;
// WZD dseg:913E
// IMG_USW_Items ITEM_ICONS
// WZD dseg:9226
extern SAMB_ptr Active_Unit;                // alloc in MoM_Tables_Init()
// WZD dseg:922A
// TBL_BattleUnits
// WZD dseg:922E
// TBL_CombatEnchants

// WZD dseg:9232
extern SAMB_ptr p0_heroes;                  // alloc in MoM_Tables_Init()
// WZD dseg:9236
extern SAMB_ptr p1_heroes;                  // alloc in MoM_Tables_Init()
// WZD dseg:923A
extern SAMB_ptr p2_heroes;                  // alloc in MoM_Tables_Init()
// WZD dseg:923E
extern SAMB_ptr p3_heroes;                  // alloc in MoM_Tables_Init()
// WZD dseg:9242
extern SAMB_ptr p4_heroes;                  // alloc in MoM_Tables_Init()
// WZD dseg:9246
extern SAMB_ptr p5_heroes;                  // alloc in MoM_Tables_Init()

// WZD dseg:9392
// drake178: Visibility_Myrror
extern uint8_t * square_scouted_p0;         // Bit_Field  alloc in MoM_Tables_Init()
// WZD dseg:9396
// drake178: Visibility_Arcanus
extern uint8_t * square_scouted_p1;         // Bit_Field  alloc in MoM_Tables_Init()


/* -2: NEVER, -1: ALWAYS, {0,1,2,3}: frame - draw off, {4,5,6,7}: frame - draw on */
// ? Set_Draw_Active_Stack_...() means this variable is private to ovr067 ?
// WZD dseg:9442
extern int16_t draw_active_stack_flag;  // AKA Active_Stack_Draw

// WZD dseg:9444
extern int16_t all_units_moved;


// dseg:9644 00 00                                           _city_idx dw 0                          ; DATA XREF: City_Screen+55r ...
// dseg:9646 00                                              db    0
// dseg:9647 00                                              db    0

// WZD dseg:9648
extern SAMB_ptr UnitDraw_WorkArea;

// dseg:964A 00                                              db    0
// dseg:964B 00                                              db    0

// WZD dseg:964C
// MoO2: ~== _ship_node
extern int16_t entities_on_movement_map[];


// dseg:973C 00 00                                           gsa_BACKGRND_3_IMG_CMB_Bottom_BG dw 0   ; DATA XREF: Load_Combat_Background_Bottom+12w ...


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
extern uint8_t * _events_table;  // alloc in MoM_Tables_Init()  7 PR 112 B  type? 1-byte,2-byte signed,unsigned 112 events or 56 events?

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
extern SAMB_ptr TBL_SharedTiles_EMS;
// WZD dseg:9CA4
extern SAMB_ptr TBL_TempMoveMap_EMS;
// WZD dseg:9CA8
// COL_MinimapTiles
// WZD dseg:9CAC
extern SAMB_ptr TBL_MoveMaps_EMS;


// WZD dseg:9CB0  
extern uint8_t * TBL_Scouting;              // alloc in MoM_Tables_Init()
// WZD dseg:9CB4
extern uint8_t * TBL_Terrain_Flags;         // alloc in MoM_Tables_Init()
// WZD dseg:9CB8
extern SAMB_ptr TBL_Terr_Specials;          // alloc in MoM_Tables_Init()

// WZD dseg:9CBC
// extern SAMB_ptr _CITIES;
// extern struct s_CITY _CITIES[];
extern struct s_CITY * _CITIES;

// WZD dseg:9CC0
// drake178: TBL_Encounters
extern struct s_LAIR * TBL_Lairs;

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
// extern SAMB_ptr TBL_Maps;
extern SAMB_ptr _world_maps;


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
extern struct s_WIZARD _players[6];
// extern struct s_WIZARD * _players;

// WZD dseg:BB7A
// extern struct s_HLP_ENTRY * _help_entries;
extern uint8_t _help_entries[];


// WZD dseg:974A
// AKA G_OVL_MapDisplay_Y
extern int16_t _prev_world_y;
// WZD dseg:974C
// AKA G_OVL_MapDisplay_X
extern int16_t _prev_world_x;

// WZD dseg:BD82 
extern int16_t OVL_Map_CenterY;
// WZD dseg:BD84 
extern int16_t OVL_Map_CenterX;
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


// WZD dseg:CB5C
// AKA OVL_NewMapDrawing
extern int16_t draw_map_full;  //; determines whether non-animated terrain tiles will be redrawn or not





#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */



#ifdef __cplusplus
}
#endif


#endif  /* MOX_DATA_H */
