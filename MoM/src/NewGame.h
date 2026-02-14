/*
    MAGIC.EXE
    ovr050
*/

#ifndef NEWGAME_H
#define NEWGAME_H

#include "../../MoX/src/MOX_TYPE.h"




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


// MGC  dseg:8A42 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+_start_spells s_Init_Spells_Table <0>   ; DATA XREF: GAME_New_Screen_5+171w ...
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



/*
    MAGIC.EXE
    ovr050
*/

// MGC  o50p01
// void Newgame_Control__WIP(void);
/* HACK */  int16_t Newgame_Control__WIP(void);

// o50p02
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
int16_t Newgame_Screen_1__WIP(void);

// MGC  o50p09
void Newgame_Screen_1_2_Draw(void);

// MGC  o50p10
int16_t Newgame_Screen_2__WIP(void);

// MGC  o50p11
int16_t Newgame_Screen_3__WIP(void);

// MGC  o50p12
void Newgame_Screen_3_Draw__WIP(void);

// MGC  o50p13
int16_t Newgame_Screen_7__WIP(void);

// MGC  o50p14
void Newgame_Screen_7_Draw__WIP(void);

// MGC  o50p15
int16_t Newgame_Screen_6__WIP(void);

// MGC  o50p16
void Newgame_Screen_6_Draw__WIP(void);

// MGC  o50p17
void GAME_DrawRetortsStr(void);

// MGC  o50p18
void NEWG_DrawDefShelf__WIP(int16_t wizard_id);

// MGC  o50p19
void Newgame_Screen4__WIP(void);

// MGC  o50p20
// GAME_Draw_NewScr4()

// MGC  o50p21
void Newgame_Screen5__WIP(void);

// MGC  o50p22
// GAME_SpellSel_GUI()

// MGC  o50p23
// SCRN_Draw_NewScr5()

// MGC  o50p24
// SCRN_Draw_NewScr5_2()

// MGC  o50p25
void WIZ_CopyDefault__WIP(int16_t wizard_id);

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
// HLP_Load_WizCreate()

// MGC  o50p34
void Set_Newgame_Screen_6_Help_List(void);

// MGC  o50p35
void STR_ListSeparator(int16_t * List_Size, int16_t Total, char * Dest);

// MGC  o50p36
// CRP_Empty_Dialog_Fn2()

// MGC  o50p37
// CRP_Empty_Dialog_Fn1()

// MGC  o50p38
// Do_Toggle_Pages()



#ifdef __cplusplus
}
#endif

#endif  /* NEWGAME_H */
