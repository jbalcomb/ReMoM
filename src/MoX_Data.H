#ifndef MOX_DATA_H
#define MOX_DATA_H

#include "MoX_TYPE.H"



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

// sizeof=0x1C
struct s_HIRED_HERO
{
    /* 00 */ int16_t Unit_Index;
    /* 02 */ char Name[14];
    /* 10 */ int16_t Items[3];
    /* 16 */ int16_t Item_Slots;
};

// sizeof=0x132
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
};

// sizeof=0x6
struct s_Astr_Data
{
    /* 00 */ int16_t Magic_Power;
    /* 02 */ int16_t Spell_Research;
    /* 04 */ int16_t Army_Strength;
};

// sizeof=0x18
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

// (sizeof=0x4C8)
struct s_WIZARD_DATA
{
    /* 0000 */ uint8_t Portrait;
    /* 0001 */ char Name[20];
    /* 0015 */ uint8_t Cptl_Race;
    /* 0016 */ uint8_t Banner;
    /* 0017 */ uint8_t Unused_017h;
    /* 0018 */ uint16_t Personality;  // enum AI_Personality
    /* 001A */ uint16_t Objective;  // enum AI_Objective
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
    /* 0048 */ uint16_t Avg_Unit_Value;  // neutral: rampage accumulator
    /* 004A */ uint16_t Unknown_04Ah;
    /* 004C */ uint16_t Cmbt_Skill_Left;
    /* 004E */ uint16_t Cast_Cost_Left;
    /* 0050 */ uint16_t Cast_Cost_Initial;  // neutral: raiders accumulator
    /* 0052 */ uint16_t Spell_Cast;
    /* 0054 */ uint16_t Skill_Left;
    /* 0056 */ uint16_t Nominal_Skill;
    /* 0058 */ uint16_t Tax_Rate;
    /* 005A */ struct s_BOOKSHELF Books;
    /* 0064 */ struct s_RETORTS Retorts;
    /* 0076 */ struct s_HIRED_HERO Heroes[6];
    /* 011E */ uint16_t Unknown_11Eh;
    /* 0120 */ uint16_t Vault_Items[4];
    /* 0128 */ struct s_WIZ_DIPL Dipl;
    /* 025A */ uint16_t Research_Left;
    /* 025C */ uint16_t Mana;
    /* 025E */ uint32_t Casting_Skill;
    /* 0262 */ uint16_t Researching;
    /* 0264 */ uint8_t Spells_Known[240];
    /* 0354 */ uint16_t Defeated_Wizards;
    /* 0356 */ uint16_t Gold;
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


// WZD dseg:9998
extern SAMB_ptr TBL_Events;


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
extern SAMB_ptr TBL_Cities;
// WZD dseg:9CC0
extern SAMB_ptr TBL_Encounters;
// WZD dseg:9CC4
extern SAMB_ptr TBL_Towers;
// WZD dseg:9CC8
extern SAMB_ptr TBL_Fortresses;
// WZD dseg:9CCC
extern SAMB_ptr TBL_Nodes;
// WZD dseg:9CD0
extern SAMB_ptr TBL_Landmasses;  // 12Eh 302d PR 302*16=4832, in World_Data
// WZD dseg:9CD4
extern SAMB_ptr UU_TBL_2;
// WZD dseg:9CD8
extern SAMB_ptr UU_TBL_1;
// WZD dseg:9CDC
extern SAMB_ptr TBL_Maps;


// WZD dseg:9D1A
extern uint16_t tmp_World_Data_Paras;

// WZD dseg:9D1C
extern SAMB_ptr World_Data_Extra;

// WZD dseg:9D1E
// Gfx_Swap

// WZD dseg:9D20
extern SAMB_ptr World_Data;

// WZD dseg:9D22
extern uint16_t grand_vizier;


// WZD dseg:9EC2
extern SAMB_ptr TBL_Units;
// WZD dseg:9EC6
extern SAMB_ptr TBL_Hero_Names;  // 25h 37d PR 37*16=592
// WZD dseg:9ECA
struct s_WIZARD_DATA TBL_Wizards[6];




// WZD dseg:BD8E
extern int16_t _unit;
extern int16_t _turn;
extern int16_t _units;
extern int16_t _cities;
extern int16_t _difficulty;
extern int16_t _magic;
extern int16_t _landsize;
extern int16_t _wizards;
extern struct s_GAME_DATA game_data;
// WZD dseg:BD9E
// magic_set s_MAGIC_SET





#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */



#ifdef __cplusplus
}
#endif


#endif  /* MOX_DATA_H */
