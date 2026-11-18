/*
    MAGIC.EXE
    ovr050
*/

#ifndef NEWGAME_H
#define NEWGAME_H

#include "../../MoX/src/MOX_TYPE.h"


enum e_Newgame_Screens
{
    ngscr_Options = 0,
    ngscr_Wizard = 1,  // "Select Wizard"
    ngscr_Portrait = 2,  // "Select Picture"  (Custom Wizard)
    ngscr_Name = 3,    // "Wizard's Name"  (Prefab/Custom)
    ngscr_Creation = 4,     /* Manual: "spell pick screen" */
    ngscr_Spells = 5,       /* Manual: "select spells screen" */
    ngscr_Race = 6,
    ngscr_Banner = 7,

    ngscr_DONE = 99
};

#define AI_PRS_MANIACAL         0
#define AI_PRS_RUTHLESS         1
#define AI_PRS_AGGRESSIVE       2
#define AI_PRS_CHAOTIC          3
#define AI_PRS_LAWFUL           4
#define AI_PRS_PEACEFUL         5
#define AI_OBJ_PRAGMATIST       0
#define AI_OBJ_MILITARIST       1
#define AI_OBJ_THEURGIST        2
#define AI_OBJ_PERFECTIONIST    3
#define AI_OBJ_EXPANSIONIST     4

/* the New Game bookshelf under the wizard portrait (Screens 1-4, 6, 7) */
#define BOOKSHELF_X                 36
#define BOOKSHELF_Y                135
#define BOOKSHELF_BOOK_WIDTH         8
/* m_spellbook_pict_segs[]: 3 book pictures (heights) per realm, the realms in the s_WIZARD_PRESET order life, sorcery, nature, death, chaos */
#define NUM_BOOK_PICTS_PER_REALM     3
#define LIFE_BOOK_PICTS              0
#define SORCERY_BOOK_PICTS           3
#define NATURE_BOOK_PICTS            6
#define DEATH_BOOK_PICTS             9
#define CHAOS_BOOK_PICTS            12
/* Select Wizard / Select Picture (Newgame_Screen_1() / _2()): two columns of wizard buttons, 7 rows each; the right column adds Custom on Select Wizard */
#define NUM_WIZARD_PRESETS          15  /* _wizard_presets_table[] and the wizard buttons: 14 wizards + Custom */
#define NUM_PRESET_WIZARDS          14
#define CUSTOM_WIZARD_IDX           14
#define WIZARD_COLUMN_ROWS           7
#define RIGHT_COLUMN_ROWS_CUSTOM     8
#define WIZARD_NAME_INPUT_MAX       10  /* characters typed on Newgame_Screen_3() */
/* Custom wizard creation (Newgame_Screen_4()) */
#define CUSTOM_WIZARD_PICKS         11
#define MAX_SPECIAL_ABILITIES        6  /* "You may not select more than 6 special abilities" */
#define NUM_TWO_PICK_ABILITIES       5  /* the picklist slots of the abilities that cost 2 picks */
#define TWO_PICK_ABILITY_COST        2
#define MYRRAN_PICK_COST             3
#define MYRRAN_PICKLIST_SLOT         7  /* wsa_picklist_order[7] == wsa_Myrran */
#define NUM_NEWGAME_SCREEN_4_HELP_ENTRIES  25
/* NEWG_PickError: which warning Newgame_Screen_4() shows */
#define PICK_ERROR_NONE                0
#define PICK_ERROR_PICKS_LEFT          1  /* "You need to make all your picks before you can continue" */
#define PICK_ERROR_TOO_MANY_ABILITIES  2  /* "You may not select more than 6 special abilities" */
#define PICK_ERROR_PREREQUISITES       3  /* "To select <ability> you need: ..." */
#define PICK_ERROR_LIFE_AND_DEATH      4  /* "You can not select both Life and Death magic" */
#define PICK_ERROR_NO_PICKS_LEFT       5  /* "You have already made all your picks" */
#define PICK_ERROR_NOT_ENOUGH_PICKS    6  /* "You don't have enough picks left to make this selection. You need 2 picks" */
/* NEWG_PickError on Select Spells (Newgame_Screen_5()) - its own two codes */
#define SPELL_PICK_ERROR_PICKS_LEFT    1  /* "You need to make all your picks before you can continue" */
#define SPELL_PICK_ERROR_SECTION_FULL  2  /* "You have no picks left in this area, to deselect click on a selected item" */
/* Select Spells: s_Init_Base_Spells.spells[] holds 10 common, 2 uncommon and 1 rare pick; each rarity is a 2 x 5 grid of spell labels */
#define NUM_START_SPELLS            13
#define NUM_START_SPELLS_COMMON     10
#define NUM_START_SPELLS_UNCOMMON    2
#define NUM_START_SPELLS_RARE        1
#define START_SPELLS_UNCOMMON       10  /* the first uncommon slot */
#define START_SPELLS_RARE          12  /* the rare slot */
#define SPELL_LIST_COLUMNS           2
#define SPELL_LIST_ROWS              5
#define SPELL_LIST_FIELDS_PER_SECTION  10
#define NUM_SPELL_LIST_FIELDS       30  /* three sections */
#define SPELL_SECTION_FIRST_Y       39  /* the first label of the first section */
#define SPELL_SECTION_HEIGHT        51

struct S_HERO_TEMPLATE
{
    /* 00 */  int16_t pick_count;
    /* 02 */  int16_t pick_type;  // {warrior, mage, any/both/either} ; enum e_RANDOM_PICK_TYPES
    /* 04 */  int32_t abilities;
    /* 08 */  int16_t casting_skill;
    /* 0A */  int16_t spell_1;  // ; enum Spells
    /* 0C */  int16_t spell_2;  // ; enum Spells
    /* 0E */  int16_t spell_3;  // ; enum Spells
    /* 10 */  int16_t spell_4;  // ; enum Spells
    /* 12 */
};
enum e_RANDOM_PICK_TYPES
{
    Warrior_Picks  = 0,
    mage_picks     = 1,
    Any_Picks      = 2
};

// sizeof=0x16
struct s_WIZARD_PRESET
{
    /* 00 */  char name[10];
    /* 0A */  int16_t life;  // book count
    /* 0C */  int16_t sorcery;
    /* 0E */  int16_t nature;
    /* 10 */  int16_t death;
    /* 12 */  int16_t chaos;
    /* 14 */  int16_t special;  // wizard special skill  (AKA retort) ; enum e_WIZARD_SPECIAL_ABILITY
    /* 16 */
};
enum e_WIZARD_SPECIAL_ABILITY
{
    wsa_NONE             = -1,
    wsa_Alchemy          =  0,
    wsa_Warlord          =  1,

    wsa_Chaos_Mastery    =  2,  // >=
    wsa_Nature_Mastery   =  3,
    wsa_Sorcery_Mastery  =  4,
    wsa_Infernal_Power   =  5,
    wsa_Divine_Power     =  6,  // <=

    wsa_Sage_Master      =  7,
    wsa_Channeller       =  8,
    wsa_Myrran           =  9,
    wsa_Archmage         = 10,
    wsa_Mana_Focusing    = 11,
    wsa_Node_Mastery     = 12,
    wsa_Famous           = 13,
    wsa_Runemaster       = 14,
    wsa_Conjurer         = 15,
    wsa_Charismatic      = 16,
    wsa_Artificer        = 17
};

// // sizeof=0x1A
// struct Default_Spells
// {
//     /* 00 */    int16_t Common[10];
//     /* 14 */    int16_t Uncommon[2];
//     /* 18 */    int16_t Rare;
//     /* 1A */
// };
// 
enum enum_RACE_BYTE
{
    R_Barbarian  = 0,
    R_Beastman  = 1,
    R_Dark_Elf  = 2,
    R_Draconian  = 3,
    R_Dwarf  = 4,
    R_Gnoll  = 5,
    R_Halfling  = 6,
    R_High_Elf  = 7,
    R_High_Man  = 8,
    R_Klackon  = 9,
    R_Lizardman  = 10,
    R_Nomad  = 11,
    R_Orc  = 12,
    R_Troll  = 13,
    Race_Generic  = 14,
    Race_Arcane  = 15,
    Race_Nature  = 16,
    Race_Sorcery  = 17,
    Race_Chaos  = 18,
    Race_Life  = 19,
    Race_Death  = 20
};


// MGC  dseg:8A42 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+_start_spells s_Init_Spells_Table <0>   ; DATA XREF: GAME_New_Screen_5+171vw ...
// ...needed by INITGAME.c
// ; (sizeof=0x1A)
struct s_Init_Base_Spells
{
    /* 0000 */  int16_t spells[13];
    /* 001A */
};
// ; (sizeof=0x82)
struct s_Init_Base_Realms
{
    /* 0000 */  struct s_Init_Base_Spells realms[5];
    /* 0082 */
};
// ...oops...  // ; (sizeof=0x30C)
// ...oops...  struct s_Init_Spells_Table
// ...oops...  {
// ...oops...      /* 030C */
// ...oops...      /* 0000 */  struct s_Init_Base_Realms players_starting_spells[PLAYER_COUNT_MAX];
// ...oops...  };
struct s_DEFAULT_SPELLS
{
    /* 0000 */  int16_t spells[13];
    /* */
};



#ifdef __cplusplus
extern "C" {
#endif


// MGC  dseg:8EA4
extern int16_t NEWG_Clicked_Race;


/*
    MAGIC.EXE
    ovr050
*/

// MGC  o50p01
// void Newgame_Control(void);
/* HACK */  int16_t Newgame_Control(void);

// MGC o50p02
void GAME_WizardsLaunch__WIP(int16_t save_gam_idx);

// MGC  o50p03
// Load_Screen()

// MGC  o50p04
// Load_Screen_Draw()

// MGC  o50p05
int16_t Newgame_Screen_0(void);

// MGC  o50p06
void Newgame_Screen_0_Draw(void);

// MGC  o50p07
void Randomize_Book_Heights(void);

// MGC  o50p08
// int16_t Newgame_Screen_1__WIP(void);
int16_t Newgame_Screen_1(void);

// MGC  o50p09
void Newgame_Screen_1_2_Draw(void);

// MGC  o50p10
// int16_t Newgame_Screen_2__WIP(void);
int16_t Newgame_Screen_2(void);

// MGC  o50p11
// int16_t Newgame_Screen_3__WIP(void);
int16_t Newgame_Screen_3(void);

// MGC  o50p12
// void Newgame_Screen_3_Draw__WIP(void);
void Newgame_Screen_3_Draw(void);

// MGC  o50p13
// int16_t Newgame_Screen_7__WIP(void);
int16_t Newgame_Screen_7(void);

// MGC  o50p14
// void Newgame_Screen_7_Draw__WIP(void);
void Newgame_Screen_7_Draw(void);

// MGC  o50p15
// int16_t Newgame_Screen_6__WIP(void);
int16_t Newgame_Screen_6(void);

// MGC  o50p16
void Newgame_Screen_6_Draw(void);

// MGC  o50p17
void Draw_Special_Abilities_String(void);

// MGC  o50p18
// void NEWG_DrawDefShelf__WIP(int16_t wizard_id);
void Draw_Default_Bookshelf(int16_t wizard_id);

// MGC  o50p19
// int16_t Newgame_Screen_4__WIP(void);
int16_t Newgame_Screen_4(void);

// MGC  o50p20
// void Newgame_Screen_4_Draw__WIP(void);
void Newgame_Screen_4_Draw(void);

// MGC  o50p21
int16_t Newgame_Screen_5(void);

// MGC  o50p22
void Newgame_Screen_5_Spell_Fields(void);

// MGC  o50p23
void Newgame_Screen_5_Draw(void);

// MGC  o50p24
void Newgame_Screen_5_Draw_Spells(void);

// MGC  o50p25
void Human_Player_Wizard_Profile(int16_t wizard_id);

// MGC  o50p26
// Fade_Out()

// MGC  o50p27
// VGA_Fade_In()

// MGC  o50p28
// Set_Load_Screen_Help_List_MGC()

// MGC  o50p29
void Set_Newgame_Screen_0_Help_List(void);

// MGC  o50p30
void Set_Newgame_Screen_7_Help_List(void);

// MGC  o50p31
void Set_Newgame_Screen_2_Help_List(void);

// MGC  o50p32
void Set_Newgame_Screen_1_Help_List(int16_t has_custom);

// MGC  o50p33
void Set_Newgame_Screen_4_Help_List(void);

// MGC  o50p34
void Set_Newgame_Screen_6_Help_List(void);

// MGC  o50p35
// void String_List_Builer(int16_t * List_Size, int16_t Total, char * Dest);
void Add_Comma_Or_And_MGC(int16_t * size, int16_t count, char * list);

// MGC o50p36
void s01p12_empty_function_MGC(void);

// MGC o50p37
void s01p11_empty_function_MGC(void);

// MGC o50p38
void PageFlip_FX_MGC(void);



#ifdef __cplusplus
}
#endif

#endif  /* NEWGAME_H */
