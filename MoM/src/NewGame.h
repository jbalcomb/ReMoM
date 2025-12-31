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
struct WIZARD_Preset
{
    /* 00 */  char Name[10];
    /* 0A */  int16_t Life;
    /* 0C */  int16_t Sorcery;
    /* 0E */  int16_t Nature;
    /* 10 */  int16_t Death;
    /* 12 */  int16_t Chaos;
    /* 14 */  int16_t Retort;  // ; enum RET_Enum
    /* 16 */
};
enum RET_Enum
{
    _No_Retort  = -1,
    _Alchemy  = 0,
    _Warlord  = 1,
    _Chaos_Mastery  = 2,
    _Nature_Mastery  = 3,
    _Sorcery_Mastery  = 4,
    _Infernal_Power  = 5,
    _Divine_Power  = 6,
    _Sage_Master  = 7,
    _Channeller  = 8,
    _Myrran  = 9,
    _Archmage  = 10,
    _Mana_Focusing  = 11,
    _Node_Mastery  = 12,
    _Famous  = 13,
    _Runemaster  = 14,
    _Conjurer  = 15,
    _Charismatic  = 16,
    _Artificer  = 17
};

// sizeof=0x1A
struct Default_Spells
{
    /* 00 */    int16_t Common[10];
    /* 14 */    int16_t Uncommon[2];
    /* 18 */    int16_t Rare;
    /* 1A */
};

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


#ifdef __cplusplus
extern "C" {
#endif



/*
    MAGIC.EXE
    ovr050
*/

// o50p01
void Newgame_Control__WIP(void);

// o50p02
void GAME_WizardsLaunch__WIP(int16_t save_gam_idx);

// o50p03
// Load_Screen()

// o50p04
// Load_Screen_Draw()

// o50p05
int16_t Newgame_Screen_0(void);

// o50p06
void Newgame_Screen_0_Draw(void);

// o50p07
void Randomize_Book_Heights(void);

// o50p08
int16_t Newgame_Screen_1__WIP(void);

// o50p09
void Newgame_Screen_1_2_Draw(void);

// o50p10
int16_t Newgame_Screen_2__WIP(void);

// o50p11
int16_t Newgame_Screen_3__WIP(void);

// o50p12
void Newgame_Screen_3_Draw__WIP(void);

// o50p13
void Newgame_Screen7__WIP(void);

// o50p14
// GAME_Draw_NewScr7()

// o50p15
int16_t Newgame_Screen_6__WIP(void);

// o50p16
void Newgame_Screen_6_Draw__WIP(void);

// o50p17
void GAME_DrawRetortsStr(void);

// o50p18
void NEWG_DrawDefShelf__WIP(int16_t Portrait_Index);

// o50p19
void Newgame_Screen4__WIP(void);

// o50p20
// GAME_Draw_NewScr4()

// o50p21
void Newgame_Screen5__WIP(void);

// o50p22
// GAME_SpellSel_GUI()

// o50p23
// SCRN_Draw_NewScr5()

// o50p24
// SCRN_Draw_NewScr5_2()

// o50p25
void WIZ_CopyDefault__WIP(int16_t Portrait_Index);

// o50p26
// Fade_Out()

// o50p27
// VGA_Fade_In()

// o50p28
// Set_Load_Screen_Help_List_MGC()

// o50p29
void Set_Newgame_Screen_0_Help_List(void);

// o50p30
// HLP_Load_BannerSel()

// o50p31
void Set_Newgame_Screen_2_Help_List(void);

// o50p32
void Set_Newgame_Screen_1_Help_List(int16_t Can_Customize);

// o50p33
// HLP_Load_WizCreate()

// o50p34
void Set_Newgame_Screen_6_Help_List(void);

// o50p35
void STR_ListSeparator(int16_t * List_Size, int16_t Total, char * Dest);

// o50p36
// CRP_Empty_Dialog_Fn2()

// o50p37
// CRP_Empty_Dialog_Fn1()

// o50p38
// Do_Toggle_Pages()



#ifdef __cplusplus
}
#endif

#endif  /* NEWGAME_H */
