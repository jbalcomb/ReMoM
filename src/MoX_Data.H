#ifndef MOX_DATA_H
#define MOX_DATA_H

#include "MoX_TYPE.H"



// sizeof:  06h  6d
struct s_LOC_16BIT
{
    /* 00 */ int16_t X_Coord;
    /* 02 */ int16_t Y_Coord;
    /* 04 */ int16_t Plane;
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



// sizeof=0x20
struct s_UNIT
{
    /* 00 */ int8_t world_x;
    /* 01 */ int8_t world_y;
    /* 02 */ int8_t world_plane;
    /* 03 */ int8_t owner_idx;
    /* 04 */ int8_t HalfMoves_Max;
    /* 05 */ int8_t type;
    /* 06 */ int8_t Hero_Slot;
    /* 07 */ int8_t Finished;
    /* 08 */ int8_t HMoves;
    /* 09 */ int8_t Target_X;
    /* 0A */ int8_t Target_Y;
    /* 0B */ int8_t Status;
    /* 0C */ int8_t Level;
    /* 0D */ int8_t Unused_0Dh;
    /* 0E */ int16_t XP;
    /* 10 */ int8_t Move_Failed;
    /* 11 */ int8_t Damage;
    /* 12 */ int8_t Draw_Priority;
    /* 13 */ int8_t Unknown_13h;
    /* 14 */ int16_t In_Tower;
    /* 16 */ int8_t Sight_Range;
    /* 17 */ int8_t Mutations;  // ; enum MUT_FLAGS
    /* 18 */ int16_t Enchants_LO;  // ; enum UE_FLAGS_L
    /* 1A */ int16_t Enchants_HO;  // ; enum UE_FLAGS_H
    /* 1C */ int8_t Rd_Constr_Left;
    /* 1D */ int8_t Rd_From_X;
    /* 1E */ int8_t Rd_From_Y;
    /* 1F */ int8_t Unknown_1Fh;
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



// sizeof=0x4
struct s_STACK
{
    /* 00 */ int16_t unit_idx;
    /* 02 */ int16_t active;  // AKA Can_Move
};










// WZD dseg:6FFE
extern int16_t Map_LastDraw_X;
// WZD dseg:7000
extern int16_t Map_LastDraw_Y;
// WZD dseg:7002
extern int16_t OVL_NewMapSustain;


// WZD dseg:9136
extern SAMB_ptr TBL_Items;
// WZD dseg:913A
extern SAMB_ptr TBL_Premade_Items;
// WZD dseg:913E
// IMG_USW_Items ITEM_ICONS
// WZD dseg:9226
// Active_Unit
// WZD dseg:922A
// TBL_BattleUnits
// WZD dseg:922E
// TBL_CombatEnchants

// WZD dseg:9232
extern SAMB_ptr p0_heroes;
// WZD dseg:9236
extern SAMB_ptr p1_heroes;
// WZD dseg:923A
extern SAMB_ptr p2_heroes;
// WZD dseg:923E
extern SAMB_ptr p3_heroes;
// WZD dseg:9242
extern SAMB_ptr p4_heroes;
// WZD dseg:9246
extern SAMB_ptr p5_heroes;

// WZD dseg:9392
extern SAMB_ptr Visibility_Myrror;
// WZD dseg:9396
extern SAMB_ptr Visibility_Arcanus;


// WZD dseg:9444
extern int16_t all_units_moved;



// WZD dseg:9648
extern SAMB_ptr UnitDraw_WorkArea;



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
extern SAMB_ptr _events_table;  // AKA TBL_Events;

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
extern SAMB_ptr TBL_Scouting;
// WZD dseg:9CB4
extern SAMB_ptr TBL_Terrain_Flags;
// WZD dseg:9CB8
extern SAMB_ptr TBL_Terr_Specials;
// WZD dseg:9CBC
// extern SAMB_ptr _CITIES;
// extern struct s_CITY _CITIES[];
extern struct s_CITY * _CITIES;

// WZD dseg:9CC0
extern SAMB_ptr TBL_Encounters;

// WZD dseg:9CC4
// extern SAMB_ptr TBL_Towers;
// extern struct s_TOWER _TOWERS[];
extern struct s_TOWER * _TOWERS;

// WZD dseg:9CC8
// extern SAMB_ptr TBL_Fortresses;
extern struct s_FORTRESS * _FORTRESSES;

// WZD dseg:9CCC
extern SAMB_ptr TBL_Nodes;
// WZD dseg:9CD0
extern SAMB_ptr TBL_Landmasses;  // 12Eh 302d PR 302*16=4832, in World_Data
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


// WZD dseg:974A
extern int16_t G_OVL_MapDisplay_Y;
// WZD dseg:974C
extern int16_t G_OVL_MapDisplay_X;

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
extern int16_t OVL_NewMapDrawing;  //; determines whether non-animated terrain tiles will be redrawn or not





#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */



#ifdef __cplusplus
}
#endif


#endif  /* MOX_DATA_H */
