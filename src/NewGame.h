/*
    MAGIC.EXE
    ovr050
*/

#ifndef NEWGAME_H
#define NEWGAME_H

#include "MOX/MOX_TYPE.h"




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



#ifdef __cplusplus
extern "C" {
#endif



/*
    MAGIC.EXE
    ovr050
*/

// o50p01
void Newgame_Control(void);

// o50p02
void GAME_WizardsLaunch__WIP(int16_t save_gam_idx);

// o50p03
// Load_Screen()

// o50p04
// Load_Screen_Draw()

// o50p05
int16_t Newgame_Screen0(void);

// o50p06
void Newgame_Screen0_Draw(void);

// o50p07
void Randomize_Book_Heights(void);

// o50p08
void Newgame_Screen1__WIP(void);

// o50p09
// Draw_NewGame_Screen1()

// o50p10
void Newgame_Screen2__WIP(void);

// o50p11
int16_t Newgame_Screen3__WIP(void);

// o50p12
// GAME_Draw_NewScr3()

// o50p13
void Newgame_Screen7__WIP(void);

// o50p14
// GAME_Draw_NewScr7()

// o50p15
int16_t Newgame_Screen6__WIP(void);

// o50p16
// GAME_Draw_NewScr6()

// o50p17
// GAME_DrawRetortsStr()

// o50p18
// NEWG_DrawDefShelf()

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
// WIZ_CopyDefault()

// o50p26
// Fade_Out()

// o50p27
// VGA_Fade_In()

// o50p28
// Set_Load_Screen_Help_List_MGC()

// o50p29
void Set_Newgame_Screen0_Help_List(void);

// o50p30
// HLP_Load_BannerSel()

// o50p31
// HLP_Load_PortraitSel()

// o50p32
// HLP_Load_WizardSel()

// o50p33
// HLP_Load_WizCreate()

// o50p34
// HLP_Load_RaceSel()

// o50p35
// STR_ListSeparator()

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
