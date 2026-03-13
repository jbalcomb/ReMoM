/*

    MAGIC.EXE
        ovr050
*/

#ifdef STU_DEBUG
#include "../../STU/src/STU_DBG.h"
#endif

#include "../../ext/stu_compat.h"

#include "../../MoX/src/DOS.h"
#include "../../MoX/src/Fields.h"
#include "../../MoX/src/FLIC_Draw.h"
#include "../../MoX/src/Fonts.h"
#include "../../MoX/src/Graphics.h"
#include "../../MoX/src/Help.h"
#include "../../MoX/src/LBX_Load.h"
#include "../../MoX/src/LOADSAVE.h"
#include "../../MoX/src/MOX_T4.h"
#include "../../MoX/src/MOM_DAT.h"  /* _difficulty, _magic, _landsize, _num_players */
#include "../../MoX/src/Mouse.h"
#include "../../MoX/src/MOX_DAT.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_SET.h"
#include "../../MoX/src/MOX_TYPE.h"
#include "../../MoX/src/random.h"
#include "../../MoX/src/Timer.h"

#include "MOM_SCR.h"
#include "RACETYPE.h"
#include "Settings.h"
#include "Spellbook.h"

#include <assert.h>
#include <stdio.h>      /* FILE; fclose(), fopen(), fread(), frite(), fseek(); */
#include <string.h>

#include <SDL_stdinc.h>

#include "NewGame.h"



// MGC  dseg:90C6 00 00                                           m_warning_box_bottom_seg dw 0
// MGC  dseg:90C8 00 00                                           m_warning_box_top_seg dw 0
// WZD dseg:CB06
extern SAMB_ptr m_warning_box_bottom_seg;
// WZD dseg:CB08
extern SAMB_ptr m_warning_box_top_seg;

// WZD o149p04
// drake178: GUI_WarningType0()
void Warn0(char * msg);

// paragrph.c
// WZD s19p01
void Print_Paragraph(int16_t x, int16_t y, int16_t max_width, char * string, int16_t print_type);





// MGC  dseg:1F96 34 27 3C 27 44 27 52 27 61 27 71 27 80 27 8D 27+
// wsa_names@ dw offset cnst_Alchemy, offset cnst_Warlord, offset cnst_ChaosMastery, offset cnst_NatureMastery, offset cnst_SorceryMastery, offset cnst_InfernalPower, offset cnst_DivinePower, offset cnst_SageMaster, offset cnst_Channeler, offset cnst_Myrran, offset cnst_Archmage, offset cnst_ManaFocusing, offset cnst_NodeMastery, offset cnst_Famous, offset cnst_Runemaster, offset cnst_Conjurer, offset cnst_Charismatic, offset cnst_Artificer
// MGC  dseg:2734 41 6C 63 68 65 6D 79 00                         cnst_Alchemy db 'Alchemy',0             
// MGC  dseg:273C 57 61 72 6C 6F 72 64 00                         cnst_Warlord db 'Warlord',0             
// MGC  dseg:2744 43 68 61 6F 73 20 4D 61 73 74 65 72 79 00       cnst_ChaosMastery db 'Chaos Mastery',0  
// MGC  dseg:2752 4E 61 74 75 72 65 20 4D 61 73 74 65 72 79 00    cnst_NatureMastery db 'Nature Mastery',0
// MGC  dseg:2761 53 6F 72 63 65 72 79 20 4D 61 73 74 65 72 79 00 cnst_SorceryMastery db 'Sorcery Mastery',0
// MGC  dseg:2771 49 6E 66 65 72 6E 61 6C 20 50 6F 77 65 72 00    cnst_InfernalPower db 'Infernal Power',0
// MGC  dseg:2780 44 69 76 69 6E 65 20 50 6F 77 65 72 00          cnst_DivinePower db 'Divine Power',0    
// MGC  dseg:278D 53 61 67 65 20 4D 61 73 74 65 72 00             cnst_SageMaster db 'Sage Master',0      
// MGC  dseg:2799 43 68 61 6E 6E 65 6C 65 72 00                   cnst_Channeler db 'Channeler',0         
// MGC  dseg:27A3 4D 79 72 72 61 6E 00                            cnst_Myrran db 'Myrran',0               
// MGC  dseg:27AA 41 72 63 68 6D 61 67 65 00                      cnst_Archmage db 'Archmage',0           
// MGC  dseg:27B3 4D 61 6E 61 20 46 6F 63 75 73 69 6E 67 00       cnst_ManaFocusing db 'Mana Focusing',0  
// MGC  dseg:27C1 4E 6F 64 65 20 4D 61 73 74 65 72 79 00          cnst_NodeMastery db 'Node Mastery',0    
// MGC  dseg:27CE 46 61 6D 6F 75 73 00                            cnst_Famous db 'Famous',0               
// MGC  dseg:27D5 52 75 6E 65 6D 61 73 74 65 72 00                cnst_Runemaster db 'Runemaster',0       
// MGC  dseg:27E0 43 6F 6E 6A 75 72 65 72 00                      cnst_Conjurer db 'Conjurer',0           
// MGC  dseg:27E9 43 68 61 72 69 73 6D 61 74 69 63 00             cnst_Charismatic db 'Charismatic',0     
// MGC  dseg:27F5 41 72 74 69 66 69 63 65 72 00                   cnst_Artificer db 'Artificer',0       

// cnst_Warlord already defined in 003_MoM.lib(NewGame.obj)

char cnst_Alchemy__NEWGAME[] = "Alchemy";
char cnst_Warlord__NEWGAME[] = "Warlord";
char cnst_ChaosMastery__NEWGAME[] = "Chaos Mastery";
char cnst_NatureMastery__NEWGAME[] = "Nature Mastery";
char cnst_SorceryMastery__NEWGAME[] = "Sorcery Mastery";
char cnst_InfernalPower__NEWGAME[] = "Infernal Power";
char cnst_DivinePower__NEWGAME[] = "Divine Power";
char cnst_SageMaster__NEWGAME[] = "Sage Master";
char cnst_Channeler__NEWGAME[] = "Channeler";
char cnst_Myrran__NEWGAME[] = "Myrran";
char cnst_Archmage__NEWGAME[] = "Archmage";
char cnst_ManaFocusing__NEWGAME[] = "Mana Focusing";
char cnst_NodeMastery__NEWGAME[] = "Node Mastery";
char cnst_Famous__NEWGAME[] = "Famous";
char cnst_Runemaster__NEWGAME[] = "Runemaster";
char cnst_Conjurer__NEWGAME[] = "Conjurer";
char cnst_Charismatic__NEWGAME[] = "Charismatic";
char cnst_Artificer__NEWGAME[] = "Artificer";

char * wsa_names[NUM_RETORTS] =
{
cnst_Alchemy__NEWGAME,
cnst_Warlord__NEWGAME,
cnst_ChaosMastery__NEWGAME,
cnst_NatureMastery__NEWGAME,
cnst_SorceryMastery__NEWGAME,
cnst_InfernalPower__NEWGAME,
cnst_DivinePower__NEWGAME,
cnst_SageMaster__NEWGAME,
cnst_Channeler__NEWGAME,
cnst_Myrran__NEWGAME,
cnst_Archmage__NEWGAME,
cnst_ManaFocusing__NEWGAME,
cnst_NodeMastery__NEWGAME,
cnst_Famous__NEWGAME,
cnst_Runemaster__NEWGAME,
cnst_Conjurer__NEWGAME,
cnst_Charismatic__NEWGAME,
cnst_Artificer__NEWGAME
};



// MGC o51p01
void Init_New_Game(void);
// MGC o56p2
void NEWG_FinalizeTables__WIP(void);
// MGC o56p11
void Initialize_Events(void);



/*

    MoO2
        Module: NEWGAME
            Newgame_Screen_
                Address: 01:000CD435
            Reload_Newgame_Screen_
                Address: 01:000CCA1C
            Draw_Newgame_Screen_
                Address: 01:000CCD3C
            Draw_Pictures_
                Address: 01:000CCC3D
            Add_Fields_
                Address: 01:000CCE2E
            Update_Fields_
                Address: 01:000CCE24
            data (0 bytes) _fields
                Address: 02:0019933C
            data (0 bytes) _background_seg
                Address: 02:00199354

        Module: MAPGEN
    
Reload_Newgame_Screen_
is where the loading happens
i.e., (Re-)Load

*/
/*

terrain_stats_table
770 records
6 bytes
half movement cost per movement type
0 unknwon
1 walking
2 Forster
3 Mountaineer
4 Swimming
5 Sailing
But, also, why 770? world_map terrain_type offset is 762 ?

*/

/*
Load_TERRSTAT()
terrain_stats_table = SA_MK_FP0( LBXR_LoadSingle("TERRSTAT", 0, 0, 770, 6) )

    XREF NEWG_SetMoveMaps+61  les     si, [terrain_stats_table]
    XREF NEWG_SetMoveMaps+94  les     si, [terrain_stats_table]
    XREF NEWG_SetMoveMaps+C4  les     si, [terrain_stats_table]
    XREF NEWG_SetMoveMaps+F7  les     si, [terrain_stats_table]
    XREF NEWG_SetMoveMaps+12A les     si, [terrain_stats_table]
    XREF NEWG_SetMoveMaps+15D les     si, [terrain_stats_table]
*/

/*
Load_SPELLDAT()
spell_data_table    = SA_MK_FP0( LBXR_LoadSingle("SPELLDAT", 0, 0, 215, 36) )

*/





// MGC  dseg:354C 00 00 01 00 01 00 01 00 05 00 02 00 01 00 02 00+TBL_AI_Realm_PER AI_PERS_Modifiers <0, 1, 1, 1, 5, 2>    ; 0
// MGC  dseg:354C 03 00 01 00 02 00 01 00 01 00 02 00 02 00 05 00+                                        ; DATA XREF: WIZ_SetPersonalities__WIP+2A7r
// MGC  dseg:354C 00 00 00 00 00 00 01 00 02 00 00 00 02 00 05 00+AI_PERS_Modifiers <1, 2, 3, 1, 2, 1>    ; 1
// MGC  dseg:354C 03 00 03 00 02 00 01 00 01 00 00 00 00 00 00 00+AI_PERS_Modifiers <1, 2, 2, 5, 0, 0>    ; 2
// MGC  dseg:354C 00 00 00 00 00 00 00 00                         AI_PERS_Modifiers <0, 1, 2, 0, 2, 5>    ; 3
// MGC  dseg:354C                                                 AI_PERS_Modifiers <3, 3, 2, 1, 1, 0>    ; 4
// MGC  dseg:354C                                                 AI_PERS_Modifiers  <0>
// MGC  dseg:3594 00 00 02 00 02 00 04 00 02 00 00 00 01 00 04 00+TBL_AI_Realm_OBJ AI_OBJ_Modifiers <0, 2, 2, 4, 2>        ; 0
// MGC  dseg:3594 02 00 03 00 00 00 03 00 02 00 02 00 03 00 00 00+                                        ; DATA XREF: WIZ_SetPersonalities__WIP+2D0r
// MGC  dseg:3594 02 00 03 00 04 00 01 00 00 00 04 00 01 00 01 00+AI_OBJ_Modifiers <0, 1, 4, 2, 3>        ; 1
// MGC  dseg:3594 04 00 00 00 00 00 00 00 00 00 00 00             AI_OBJ_Modifiers <0, 3, 2, 2, 3>        ; 2
// MGC  dseg:3594                                                 AI_OBJ_Modifiers <0, 2, 3, 4, 1>        ; 3
// MGC  dseg:3594                                                 AI_OBJ_Modifiers <0, 4, 1, 1, 4>        ; 4
// MGC  dseg:3594                                                 AI_OBJ_Modifiers  <0>
// struct AI_PERS_Modifiers
// {
//     /* 00 */  int16_t Maniacal;
//     /* 02 */  int16_t Ruthless;
//     /* 04 */  int16_t Aggressive;
//     /* 06 */  int16_t Chaotic;
//     /* 08 */  int16_t Lawful;
//     /* 0A */  int16_t Peaceful;
//     /* 0C */
// };
// struct AI_OBJ_Modifiers
// {
//     /* 00 */  int16_t Pragmatist;
//     /* 02 */  int16_t Militarist;
//     /* 04 */  int16_t Theurgist;
//     /* 06 */  int16_t Perfectionist;
//     /* 08 */  int16_t Expansionist;
//     /* 0A */
// };
int16_t TBL_AI_Realm_PRS[6][6] = {
    { 0, 1, 1, 1, 5, 2 },
    { 1, 2, 3, 1, 2, 1 },
    { 1, 2, 2, 5, 0, 0 },
    { 0, 1, 2, 0, 2, 5 },
    { 3, 3, 2, 1, 1, 0 },
    { 0, 0, 0, 0, 0, 0 }
};
int16_t TBL_AI_Realm_OBJ[6][5] = {
    { 0, 2, 2, 4, 2 },
    { 0, 1, 4, 2, 3 },
    { 0, 3, 2, 2, 3 },
    { 0, 2, 3, 4, 1 },
    { 0, 4, 1, 1, 4 },
    { 0, 0, 0, 0, 0 }
};





struct S_HERO_TEMPLATE _hero_premade_table[NUM_HERO_TYPES] = {
    /* picks        special                  casting  spells     */
    /* count, type, abilities,               skill    1,  2,  3,  4 */
    /*  */  {      0,   0, HSA_CONSTITUTION,              0,  0,  0,  0,  0 },
    /*  */  {      0,   0, HSA_MIGHT,                     0,  0,  0, 0, 0 },
    /*  */  {      0,   1, HSA_SAGE,                      2,  42, 48, 0, 0 },
    /*  */  {      0,   0, HSA_NOBLE,                     0,  0, 0, 0, 0 },
    /*  */  {      0,   0, 0,                             1,  spl_Resist_Elements, 0, 0, 0 },
    /*  */  {      0,   0, (HSA_LEADERSHIP | HSA_FEMALE), 1, 46, 53, 0, 0 },
    /*  */  {      1,   0, 0,                             0, 0, 0, 0, 0 },
    /*  */  {      1,   1, 0x40000000, 2, 125, 0, 0, 0 },
    /*  */  {      1,   0, 0x40000040, 0, 0, 0, 0, 0 },
    /*  */  {      0,   0, 0xD0000000, 0, 0, 0, 0, 0 },
    /*  */  {      0,   0, 0, 2, 13, 22, 5, 0 },
    /*  */  {      1,   0, 0x4000000, 0, 0, 0, 0, 0 },
    /*  */  {      1,   2, 0, 1, 86, 88, 94, 0 },
    /*  */  {      1,   1, 0x40000, 3, 83, 96, 90, 109 },
    /*  */  {      1,   0, 0x40, 0, 0, 0, 0, 0 },
    /*  */  {      1,   1, 0, 2, 47, 44, 0, 0 },
    /*  */  {      0,   0, 0x8000, 1, 2, 7, 0, 0 },
    /*  */  {      2,   0, 0x8000, 0, 0, 0, 0, 0 },
    /*  */  {      2,   1, 0x50000000, 3, 169, 170, 174, 172 },
    /*  */  {      2,   2, 0, 1, 0, 0, 0, 0 },
    /*  */  {      2,   0, 0x40, 0, 0, 0, 0, 0 },
    /*  */  {      1,   0, 0x11, 0, 0, 0, 0, 0 },
    /*  */  {      1,   0, 0x50008040, 0, 0, 0, 0, 0 },
    /*  */  {      1,   1, 0, 5, 91, 101, 104, 0 },
    /*  */  {      5,   2, 0, 1, 0, 0, 0, 0 },
    /*  */  {      2,   1, 0x40000000, 3, 50, 53, 67, 0 },
    /* Deathstryke Swordsman */  {      1,   0, 0x9209, 0, 0, 0, 0, 0 },
    /* Elana Priestess */        {      0,   0, 0x72040000, 4, 134, 125, 140, 150 },
    /* Roland Paladin */         {      1,   0, 0x1010008, 0, 0, 0, 0, 0 },
    /* Mortu Black Night */      {      1,   0, 0x9048, 0, 0, 0, 0, 0 },
    /* Alorra Elven Archer */    {      3,   2, 0x40000040, 1, 41, 56, 0, 0 },
    /* Sir Harold Knight */      {      1,   0, 0x20001012, 0, 0, 0, 0, 0 },
    /* Ravashack Necromancer */  {      2,   1, 0x40000, 4, 162, 165, 195, 182 },
    /* Warrax Chaos Warrior */   {      3,   2, 0x41000, 3, 0, 0, 0, 0 },
    /* Torin The Chosen */       {      2,   2, 0x1011002, 5, 128, 125, 126, 141 }
};

// dseg:35D0 00 00 00 00 00 10 00 00 00 00 00 00 00 00 00 00+_hero_premade_table s_HERO_TEMPLATE <0, 0, 1000h, 0, 0, 0, 0, 0>
// dseg:35D0 00 00 00 00 00 00 00 80 00 00 00 00 00 00 00 00+                                        ; DATA XREF: NEWG_CreateHeroes+66r ...
// dseg:35D0 00 00 00 00 00 00 01 00 00 00 20 00 02 00 2A 00+s_HERO_TEMPLATE <0, 0, 8000h, 0, 0, 0, 0, 0>
// dseg:35D0 30 00 00 00 00 00 00 00 00 00 00 00 00 20 00 00+s_HERO_TEMPLATE <0, 1, 200000h, 2, 42, 48, 0, 0>
// dseg:35D0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+s_HERO_TEMPLATE <0, 0, 20000000h, 0, 0, 0, 0, 0>
// dseg:35D0 01 00 02 00 00 00 00 00 00 00 00 00 00 00 01 00+s_HERO_TEMPLATE <0, 0, 0, 1, 2, 0, 0, 0>
// dseg:35D0 00 40 01 00 2E 00 35 00 00 00 00 00 01 00 00 00+s_HERO_TEMPLATE <0, 0, 40000001h, 1, 46, 53, 0, 0>
// dseg:35D0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00+s_HERO_TEMPLATE <1, 0, 0, 0, 0, 0, 0, 0>
// dseg:35D0 01 00 00 00 00 40 02 00 7D 00 00 00 00 00 00 00+s_HERO_TEMPLATE <1, 1, 40000000h, 2, 125, 0, 0, 0>
// dseg:35D0 01 00 00 00 40 00 00 40 00 00 00 00 00 00 00 00+s_HERO_TEMPLATE <1, 0, 40000040h, 0, 0, 0, 0, 0>
// dseg:35D0 00 00 00 00 00 00 00 00 00 D0 00 00 00 00 00 00+s_HERO_TEMPLATE <0, 0, 0D0000000h, 0, 0, 0, 0, 0>
// dseg:35D0 00 00 00 00 00 00 00 00 00 00 00 00 02 00 0D 00+s_HERO_TEMPLATE <0, 0, 0, 2, 13, 22, 5, 0>
// dseg:35D0 16 00 05 00 00 00 01 00 00 00 00 00 00 04 00 00+s_HERO_TEMPLATE <1, 0, 4000000h, 0, 0, 0, 0, 0>
// dseg:35D0 00 00 00 00 00 00 00 00 01 00 02 00 00 00 00 00+s_HERO_TEMPLATE <1, 2, 0, 1, 86, 88, 94, 0>
// dseg:35D0 01 00 56 00 58 00 5E 00 00 00 01 00 01 00 00 00+s_HERO_TEMPLATE <1, 1, 40000h, 3, 83, 96, 90, 109>
// dseg:35D0 04 00 03 00 53 00 60 00 5A 00 6D 00 01 00 00 00+s_HERO_TEMPLATE <1, 0, 40h, 0, 0, 0, 0, 0>
// dseg:35D0 40 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00+s_HERO_TEMPLATE <1, 1, 0, 2, 47, 44, 0, 0>
// dseg:35D0 01 00 00 00 00 00 02 00 2F 00 2C 00 00 00 00 00+s_HERO_TEMPLATE <0, 0, 8000h, 1, 2, 7, 0, 0>
// dseg:35D0 00 00 00 00 00 80 00 00 01 00 02 00 07 00 00 00+s_HERO_TEMPLATE <2, 0, 8000h, 0, 0, 0, 0, 0>
// dseg:35D0 00 00 02 00 00 00 00 80 00 00 00 00 00 00 00 00+s_HERO_TEMPLATE <2, 1, 50000000h, 3, 169, 170, 174, 172>
// dseg:35D0 00 00 00 00 02 00 01 00 00 00 00 50 03 00 A9 00+s_HERO_TEMPLATE <2, 2, 0, 1, 0, 0, 0, 0>
// dseg:35D0 AA 00 AE 00 AC 00 02 00 02 00 00 00 00 00 01 00+s_HERO_TEMPLATE <2, 0, 40h, 0, 0, 0, 0, 0>
// dseg:35D0 00 00 00 00 00 00 00 00 02 00 00 00 40 00 00 00+s_HERO_TEMPLATE <1, 0, 11h, 0, 0, 0, 0, 0>
// dseg:35D0 00 00 00 00 00 00 00 00 00 00 01 00 00 00 11 00+s_HERO_TEMPLATE <1, 0, 50008040h, 0, 0, 0, 0, 0>
// dseg:35D0 00 00 00 00 00 00 00 00 00 00 00 00 01 00 00 00+s_HERO_TEMPLATE <1, 1, 0, 5, 91, 101, 104, 0>
// dseg:35D0 40 80 00 50 00 00 00 00 00 00 00 00 00 00 01 00+s_HERO_TEMPLATE <5, 2, 0, 1, 0, 0, 0, 0>
// dseg:35D0 01 00 00 00 00 00 05 00 5B 00 65 00 68 00 00 00+s_HERO_TEMPLATE <2, 1, 40000000h, 3, 50, 53, 67, 0>
// dseg:35D0 05 00 02 00 00 00 00 00 01 00 00 00 00 00 00 00+s_HERO_TEMPLATE <1, 0, 9209h, 0, 0, 0, 0, 0>
// dseg:35D0 00 00 02 00 01 00 00 00 00 40 03 00 32 00 35 00+s_HERO_TEMPLATE <0, 0, 72040000h, 4, 134, 125, 140, 150>
// dseg:35D0 43 00 00 00 01 00 00 00 09 92 00 00 00 00 00 00+s_HERO_TEMPLATE <1, 0, 1010008h, 0, 0, 0, 0, 0>
// dseg:35D0 00 00 00 00 00 00 00 00 00 00 00 00 04 72 04 00+s_HERO_TEMPLATE <1, 0, 9048h, 0, 0, 0, 0, 0>
// dseg:35D0 86 00 7D 00 8C 00 96 00 01 00 00 00 08 00 01 01+s_HERO_TEMPLATE <3, 2, 40000040h, 1, 41, 56, 0, 0>
// dseg:35D0 00 00 00 00 00 00 00 00 00 00 01 00 00 00 48 90+s_HERO_TEMPLATE <1, 0, 20001012h, 0, 0, 0, 0, 0>
// dseg:35D0 00 00 00 00 00 00 00 00 00 00 00 00 03 00 02 00+s_HERO_TEMPLATE <2, 1, 40000h, 4, 162, 165, 195, 182>
// dseg:35D0 40 00 00 40 01 00 29 00 38 00 00 00 00 00 01 00+s_HERO_TEMPLATE <3, 2, 41000h, 3, 0, 0, 0, 0>
// dseg:35D0 00 00 12 10 00 20 00 00 00 00 00 00 00 00 00 00+s_HERO_TEMPLATE <2, 2, 1011002h, 5, 128, 125, 126, 141>


// MGC  dseg:52C0
// MainMenu.c  int16_t _quit_button = 0;
extern int16_t _quit_button;

// MGC  dseg:5CB0
SAMB_ptr newgame_ok_button_seg = 0;

// MGC  dseg:64AE
int16_t screen_changed = 0;

// MGC  dseg:8E8E
SAMB_ptr _quit_active_seg = 0;

// MGC  dseg:8E92
int16_t _ok_button = 0;




// MGC  dseg:52B2                                                 ? BEGIN: Main Menu Screen ?
// MGC  dseg:52B2
// MGC  dseg:52B2                                                 ; char *GUI_String_2
// MGC  dseg:52B2 00 00                                           GUI_String_2 dw 0                       ; DATA XREF: start+C7o ...
// MGC  dseg:52B2                                                                                         ; _bdata_start
// MGC  dseg:52B2                                                                                         ; 100 LBX_NearAlloc_Next bytes
// MGC  dseg:52B4                                                 ; char *GUI_String_1
// MGC  dseg:52B4 00 00                                           GUI_String_1 dw 0                       ; DATA XREF: Hall_Of_Fame_Screen+8Dw ...
// MGC  dseg:52B4                                                                                         ; 100 LBX_NearAlloc_First bytes
// MGC  dseg:52B6 00 00                                           _load_button dw 0                       ; DATA XREF: Main_Menu_Screen+19Fw ...
// MGC  dseg:52B8 00 00                                           _background_seg dw 0                    ; DATA XREF: Load_Screen+1Bw ...
SAMB_ptr newgame_background_seg = 0;
// MGC  dseg:52BA 00 00                                           menu_quit_button dw 0                   ; DATA XREF: Load_MGC_Resources+76w ...
// MGC  dseg:52BA                                                                                         ; single-loaded 2-frame image
// MGC  dseg:52BC 00 00                                           _halloffame_button dw 0                 ; DATA XREF: Main_Menu_Screen+29Ew ...
// MGC  dseg:52BE 00 00                                           menu_hof_button dw 0                    ; DATA XREF: Load_MGC_Resources+40w ...
// MGC  dseg:52BE                                                                                         ; single-loaded 2-frame image
// MGC  dseg:52C0 00 00                                           _quit_button dw 0                       ; DATA XREF: Main_Menu_Screen+2DFw ...
// MGC  dseg:52C2 00 00                                           menu_load_button dw 0                   ; DATA XREF: Load_MGC_Resources+52w ...

// MGC  dseg:52C4 00 00                                           current_menu_screen dw 0                ; DATA XREF: Main_Menu_Screen+32w ...
int16_t auto_input_field_idx = 0;

// MGC  dseg:52C4                                                                                         ; 0 - continue, 1 -load, 2 - new, 3 - quit, 4 - HoF
// MGC  dseg:52C4                                                                                         ; holds the mouse focus control instead during the
// MGC  dseg:52C4                                                                                         ;  redraw functions of other screens
// MGC  dseg:52C6 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+wizard_portrait_segs dw 14 dup(    0)   ; DATA XREF: Load_MGC_Resources+A9w ...
// MGC  dseg:52C6 00 00 00 00 00 00 00 00 00 00 00 00                                                     ; array of 14 single-loaded images
// MGC  dseg:52E2 00 00                                           IMG_NewG_BtnBorder@ dw 0                ; used by the map customizer and profile reloader
// MGC  dseg:52E4 00 00                                           load_flag dw 0                          ; DATA XREF: Main_Menu_Screen+38w ...
// MGC  dseg:52E4                                                                                         ; OXRB:Menu,MenuDraw.:.static?module-scoped?private?
// MGC  dseg:52E4                                                                                         ; Da-Funk? "OXRB"? "only x-ref'd by"? if so, nowadays, use "OON XREF"
// MGC  dseg:52E6 00 00                                           NEWG_HaveLastProfile dw 0               ; used by the profile reloader
// MGC  dseg:52E8 00 00                                           mainmenu_top dw 0                       ; DATA XREF: Load_MGC_Resources+1Cw ...
// MGC  dseg:52E8                                                                                         ; single-loaded 20 frame animation
// MGC  dseg:52E8                                                                                         ; includes the background for the top of the screen too
// MGC  dseg:52EA 00 00                                           _newgame_button dw 0                    ; DATA XREF: Main_Menu_Screen+25Dw ...
// MGC  dseg:52EC 00 00                                           menu_new_button dw 0                    ; DATA XREF: Load_MGC_Resources+64w ...
// MGC  dseg:52EC                                                                                         ; single-loaded 2-frame image
// MGC  dseg:52EE 00 00                                           _continue_button dw 0                   ; DATA XREF: Main_Menu_Screen+204w ...
// MGC  dseg:52F0 00 00                                           menu_continue_button dw 0               ; DATA XREF: Load_MGC_Resources+2Ew ...
// MGC  dseg:52F0                                                                                         ; single-loaded 2-frame image
// MGC  dseg:52F2 00 00                                           cont_flag dw 0                          ; DATA XREF: Main_Menu_Screen+2Cw ...
// MGC  dseg:52F2                                                                                         ; OXRB:Menu,MenuDraw.:.static?module-scoped?private?
// MGC  dseg:52F4 00 00                                           mainmenu_bottom dw 0                    ; DATA XREF: Load_MGC_Resources+88w ...
// MGC  dseg:52F4                                                                                         ; single-loaded image
// MGC  dseg:52F6 00 00                                           mainmenu_bot dw 0                       ; used by the map customizer to indicate which screen
// MGC  dseg:52F6                                                                                         ; is currently active (basic/patch/world/world2)
// MGC  dseg:52F8 00 00                                           _load_inactive_seg dw 0                 ; DATA XREF: Load_Screen+60w ...
// MGC  dseg:52FA 00 00                                           NEWG_LastRetorts@ dw 0                  ; used by the profile reloader
// MGC  dseg:52FC 00 00                                           g_GUI_MainScreenJump dw 0               ; DATA XREF: Menu_Screen_Control+5Aw ...
// MGC  dseg:52FE 00 00                                           save_game_count dw 0                    ; DATA XREF: Main_Menu_Screen+C6w ...
// MGC  dseg:5300 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 save_game_slots dw 8 dup(     0)        ; DATA XREF: Main_Menu_Screen+137w ...
// MGC  dseg:5300
// MGC  dseg:5300                                                 ? END: Main Menu Screen ?
// MGC  dseg:5300
// MGC  dseg:5310
// MGC  dseg:5310                                                 ? Begin: New Game Screen ?
// MGC  dseg:5310
// MGC  dseg:5310 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+NEWG_WorldgenSet dw 40h dup(0)          ; extended worldgen settings are loaded here by the
// MGC  dseg:5310 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; worldgen customizer
// MGC  dseg:5390 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+NEWG_LastProfile db 0B6h dup(0)         ; the last saved wizard profile is loaded here by
// MGC  dseg:5390 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; the profile reloader
// MGC  dseg:5446 00 00                                           IMG_NewG_ValueBox@ dw 0                 ; NEWGAME.LBX entry 51, used by the world generator
// MGC  dseg:5446                                                                                         ; customizer to display the setting values
// MGC  dseg:5446                                                                                         ; (number, On/Off, Low/Norm/High, or Rand)
// MGC  dseg:5448 00 00                                           IMG_NewG_CheckMark@ dw 0
// MGC  dseg:544A 00 00                                           IMG_NewG_ScrollUp@ dw 0
// MGC  dseg:544C 00 00                                           IMG_NewG_ScrollDown@ dw 0
// MGC  dseg:544E 00 00                                           NEWG_WGP_OK_Label dw 0
// MGC  dseg:5450 00 00                                           NEWG_WGP_Reset_Lbl dw 0
// MGC  dseg:5452 00 00                                           NEWG_WGP_ScrollUp dw 0
// MGC  dseg:5454 00 00                                           NEWG_WGP_ScrollDn dw 0
// MGC  dseg:5456 00 00                                           NEWG_FirstPatchSet dw 0
// MGC  dseg:5458 00 00                                           NEWG_FirstWorldSet dw 0
// MGC  dseg:545A 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+NEWG_WGP_SetCtrls dw 24h dup(0)
// MGC  dseg:54A2 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+NEWG_WGPSliderStates dw 12h dup(0)
// MGC  dseg:54C6 00 00                                           NEWG_ConnMap_Seg dw 0                   ; a 9600 byte buffer in GFX_Swap_Seg
// MGC  dseg:54C8 00                                              NEWG_CustomWorld db 0
// MGC  dseg:54C9 00                                              NEWG_CustomPlanes db 0
// MGC  dseg:54CA 00                                              NEWG_SliderDragged db 0
// MGC  dseg:54CB 00                                              align 2
// MGC  dseg:54CC 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+UU_Some_AI_Array db 54Eh dup(0)
// MGC  dseg:5A1A 00 00                                           Wiz5_Spell_E0h@ dw 0                    ; DATA XREF: NEWG_FinalizeTables+95r ...
// MGC  dseg:5A1A                                                                                         ; 80 + rnd(40) is stored here for each AI wizard (word
// MGC  dseg:5A1A                                                                                         ; array pointer, human excluded) when setting initial
// MGC  dseg:5A1A                                                                                         ; gold
// MGC  dseg:5A1C 00 00                                           Wiz5_Spell_C8h@ dw 0                    ; DATA XREF: Allocate_Data_Space+3F6w
// MGC  dseg:5A1E 00 00                                           Wiz5_Spell_D4h@ dw 0                    ; DATA XREF: Allocate_Data_Space+3FCw
// MGC  dseg:5A20 00 00                                           Wiz5_Spell_A0h@ dw 0                    ; DATA XREF: Allocate_Data_Space+3EAw
// MGC  dseg:5A22 00 00                                           Wiz5_Spell_B4h@ dw 0                    ; DATA XREF: Allocate_Data_Space+3F0w
// MGC  dseg:5A24 00 00                                           Wiz5_Spell_50h@ dw 0                    ; DATA XREF: Allocate_Data_Space+3D2w
// MGC  dseg:5A26 00 00                                           Wiz5_Spell_64h@ dw 0                    ; DATA XREF: Allocate_Data_Space+3D8w
// MGC  dseg:5A28 00 00                                           Wiz5_Spell_28h@ dw 0                    ; DATA XREF: Allocate_Data_Space+3C6w
// MGC  dseg:5A2A 00 00                                           Wiz5_Spell_3Ch@ dw 0                    ; DATA XREF: Allocate_Data_Space+3CCw
// MGC  dseg:5A2C 00 00                                           Wiz5_Spell_18h@ dw 0                    ; DATA XREF: Allocate_Data_Space+3C0w
// MGC  dseg:5A2E 00 00                                           Wiz5_Spell_78h@ dw 0                    ; DATA XREF: Allocate_Data_Space+3DEw
// MGC  dseg:5A30 00 00                                           Wiz5_Spell_8Ch@ dw 0                    ; DATA XREF: Allocate_Data_Space+3E4w
// MGC  dseg:5A32 00 00                                           Wiz5_Spell_00h@ dw 0                    ; DATA XREF: Allocate_Data_Space+3B4w
// MGC  dseg:5A34 00 00                                           Wiz5_Spell_0Ch@ dw 0                    ; DATA XREF: Allocate_Data_Space+3BAw
// MGC  dseg:5A36 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+G_UU_Some_AI_Array_2 db 176h dup(0)

// MGC  dseg:5BAC  terrain_stats_table





// MGC  dseg:2ABC                                                 BEGIN:  ovr050 - Initialized Data

// MGC  dseg:2ABC
struct s_mouse_list mouse_list_newgame[1] = {
    { crsr_Finger, 0, SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX }
};

// MGC  dseg:2AC8
uint8_t wziards_name_colors[8] = { 31, 30, 29, 28, 27, 26, 25, 24 };

// MGC  dseg:2AD0
struct s_WIZARD_PRESET _wizard_presets_table[15] = {
    {"Merlin",  5,  0,  5, 0,  0, wsa_Sage_Master },
    {"Raven",   0,  6,  5, 0,  0, wsa_NONE},
    {"Sharee",  0,  0,  0, 5,  5, wsa_Conjurer},
    {"Lo Pan",  0,  5,  0, 0,  5, wsa_Channeller},
    {"Jafar",   0, 10,  0, 0,  0, wsa_Alchemy},
    {"Oberic",  0,  0,  5, 0,  5, wsa_Mana_Focusing},
    {"Rjak",    0,  0,  0, 9,  0, wsa_Infernal_Power},
    {"Sss'ra",  4,  0,  0, 0,  4, wsa_Myrran},
    {"Tauron",  0,  0,  0, 0, 10, wsa_Chaos_Mastery},
    {"Freya",   0,  0, 10, 0,  0, wsa_Nature_Mastery},
    {"Horus",   5,  5,  0, 0,  0, wsa_Archmage},
    {"Ariel",  10,  0,  0, 0,  0, wsa_Charismatic},
    {"Tlaloc",  0,  0,  4, 5,  0, wsa_Warlord},
    {"Kali",    0,  5,  0, 5,  0, wsa_Artificer},
    {"Custom",  0,  0,  0, 0,  0, wsa_NONE}
};

// MGC  dseg:2C1A 02 00 03 00 04 00 05 00 06 00 07 00 08 00 09 00+RP_Book_Table dw 2, 3, 4, 5, 6, 7, 8, 9, 0Ah, 0Bh ; repurposed in the worldgen customizer
// MGC  dseg:2C2E
int16_t m_select_count_common[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
// MGC  dseg:2C42
int16_t m_select_count_uncommon[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 };
// MGC  dseg:2C56
int16_t m_select_count_rare[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };

// // MGC  dseg:2C6A
// // Statically define a variable named 'player_spells'
// struct Default_Spells player_spells = {
//     {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},        // Common spells initialization
//     {spl_Basilisk, spl_Elemental_Armor},    // Uncommon spells initialization
//     spl_Psionic_Blast                       // Rare spell initialization
// };
// // MGC  dseg:2C6A
// struct Default_Spells TBL_Def_Spl_Nature = {
//     { spl_War_Bears, spl_Stone_Skin, spl_Sprites, spl_Water_Walking, spl_Giant_Strength, spl_Web, spl_Earth_To_Mud, spl_Wall_Of_Stone, spl_Resist_Elements, spl_Earth_Lore }, 
//     { spl_Cockatrices, spl_Change_Terrain },
//     spl_Gorgons
// };
// // MGC  dseg:2C84
// struct Default_Spells TBL_Def_Spl_Sorcery = {
//     { spl_Nagas, spl_Psionic_Blast, spl_Phantom_Warriors, spl_Floating_Island, spl_Confusion, spl_Counter_Magic, spl_Word_Of_Recall, spl_Dispel_Magic_True, spl_Resist_Magic, spl_Guardian_Wind },
//     { spl_Flight, spl_Phantom_Beast },
//     spl_Storm_Giant
// };
// // MGC  dseg:2C9E
// struct Default_Spells TBL_Def_Spl_Chaos = {
//     { spl_Fire_Bolt, spl_Fire_Elemental, spl_Eldritch_Weapon, spl_Hell_Hounds, spl_Corruption, spl_Warp_Creature, spl_Shatter, spl_Wall_Of_Fire, spl_Disrupt, spl_Warp_Wood },
//     { spl_Lightning_Bolt, spl_Doom_Bat },
//     spl_Efreet
// };
// // MGC  dseg:2CB8
// struct Default_Spells TBL_Def_Spl_Life = {
//     { spl_Heroism, spl_Guardian_Spirit, spl_Holy_Armor, spl_Just_Cause, spl_Healing, spl_Holy_Weapon, spl_Star_Fires, spl_Endurance, spl_True_Light, spl_Bless },
//     { spl_Resurrection, spl_Unicorns },
//     spl_Angel
// };
// // MGC  dseg:2CD2
// struct Default_Spells TBL_Def_Spl_Death = {
// { spl_Life_Drain, spl_Ghouls, spl_Weakness, spl_Dark_Rituals, spl_Black_Sleep, spl_Darkness, spl_Terror, spl_Skeletons, spl_Mana_Leak, spl_Cloak_Of_Fear},
// { spl_Black_Prayer, spl_Black_Channels },
// spl_Wraiths
// };

// /*
//     BEGIN: take 2 - defaults spells and wizard profiles
// Human_Player_Wizard_Profile()
// iters over 13 ... TBL_Spells_Nature.Common[itr] = 0
// so, just an array of 13
// uses `word ptr`
// so, 2-byte values
// */
// // MGC  dseg:2C6A
// int16_t _default_spells_nature[13] = {
//     spl_War_Bears, spl_Stone_Skin, spl_Sprites, spl_Water_Walking, spl_Giant_Strength, spl_Web, spl_Earth_To_Mud, spl_Wall_Of_Stone, spl_Resist_Elements, spl_Earth_Lore,
//     spl_Cockatrices, spl_Change_Terrain,
//     spl_Gorgons
// };
// // MGC  dseg:2C84
// int16_t _default_spells_sorcery[13] = {
//     spl_Nagas, spl_Psionic_Blast, spl_Phantom_Warriors, spl_Floating_Island, spl_Confusion, spl_Counter_Magic, spl_Word_Of_Recall, spl_Dispel_Magic_True, spl_Resist_Magic, spl_Guardian_Wind,
//     spl_Flight, spl_Phantom_Beast,
//     spl_Storm_Giant
// };
// // MGC  dseg:2C9E
// int16_t _default_spells_chaos[13] = {
//     spl_Fire_Bolt, spl_Fire_Elemental, spl_Eldritch_Weapon, spl_Hell_Hounds, spl_Corruption, spl_Warp_Creature, spl_Shatter, spl_Wall_Of_Fire, spl_Disrupt, spl_Warp_Wood,
//     spl_Lightning_Bolt, spl_Doom_Bat,
//     spl_Efreet
// };
// // MGC  dseg:2CB8
// int16_t _default_spells_life[13] = {
//     spl_Heroism, spl_Guardian_Spirit, spl_Holy_Armor, spl_Just_Cause, spl_Healing, spl_Holy_Weapon, spl_Star_Fires, spl_Endurance, spl_True_Light, spl_Bless,
//     spl_Resurrection, spl_Unicorns,
//     spl_Angel
// };
// // MGC  dseg:2CD2
// int16_t _default_spells_death[13] = {
//     spl_Life_Drain, spl_Ghouls, spl_Weakness, spl_Dark_Rituals, spl_Black_Sleep, spl_Darkness, spl_Terror, spl_Skeletons, spl_Mana_Leak, spl_Cloak_Of_Fear,
//     spl_Black_Prayer, spl_Black_Channels,
//     spl_Wraiths
// };
// /*
//     END:  take 2 - defaults spells and wizard profiles
// */

/*
    BEGIN: take 3 - 
*/

// MGC  dseg:2C6A
struct s_DEFAULT_SPELLS _default_spells[5] = {
    {
        spl_War_Bears, spl_Stone_Skin, spl_Sprites, spl_Water_Walking, spl_Giant_Strength, spl_Web, spl_Earth_To_Mud, spl_Wall_Of_Stone, spl_Resist_Elements, spl_Earth_Lore,
        spl_Cockatrices, spl_Change_Terrain,
        spl_Gorgons
    },
    {
        spl_Nagas, spl_Psionic_Blast, spl_Phantom_Warriors, spl_Floating_Island, spl_Confusion, spl_Counter_Magic, spl_Word_Of_Recall, spl_Dispel_Magic_True, spl_Resist_Magic, spl_Guardian_Wind,
        spl_Flight, spl_Phantom_Beast,
        spl_Storm_Giant
    },
    {
        spl_Fire_Bolt, spl_Fire_Elemental, spl_Eldritch_Weapon, spl_Hell_Hounds, spl_Corruption, spl_Warp_Creature, spl_Shatter, spl_Wall_Of_Fire, spl_Disrupt, spl_Warp_Wood,
        spl_Lightning_Bolt, spl_Doom_Bat,
        spl_Efreet
    },
    {
        spl_Heroism, spl_Guardian_Spirit, spl_Holy_Armor, spl_Just_Cause, spl_Healing, spl_Holy_Weapon, spl_Star_Fires, spl_Endurance, spl_True_Light, spl_Bless,
        spl_Resurrection, spl_Unicorns,
        spl_Angel
    },
    {
        spl_Life_Drain, spl_Ghouls, spl_Weakness, spl_Dark_Rituals, spl_Black_Sleep, spl_Darkness, spl_Terror, spl_Skeletons, spl_Mana_Leak, spl_Cloak_Of_Fear,
        spl_Black_Prayer, spl_Black_Channels,
        spl_Wraiths
    }
};

/*
    END: take 3 - 
*/

// MGC  dseg:2CEC
// sizeof: 14d
// struct s_WIZARD_SPECIAL_ABILITIES_REQUIREMENTS
// {
//     /* 0 */  int16_t Any_Realm;
//     /* 2 */  int16_t Realm_Count;
//     /* 4 */  int16_t Life;
//     /* 6 */  int16_t Death;
//     /* 8 */  int16_t Chaos;
//     /* A */  int16_t Nature;
//     /* C */  int16_t Sorcery;
// };
// struct s_WIZARD_SPECIAL_ABILITIES_REQUIREMENTS wsa_prerequisites[18];
/*
column 1            column 2            column 3
wsa_Alchemy         wsa_Sage_Master     wsa_Chaos_Mastery
wsa_Warlord         wsa_Myrran          wsa_Nature_Mastery
wsa_Channeller      wsa_Divine_Power    wsa_Sorcery_Mastery
wsa_Archmage        wsa_Famous          wsa_Infernal_Power
wsa_Artificer       wsa_Runemaster      wsa_Mana_Focusing
wsa_Conjurer        wsa_Charismatic     wsa_Node_Mastery
*/
/*
enum e_WIZARD_SPECIAL_ABILITY
    wsa_Alchemy          =  0
    wsa_Warlord          =  1
    wsa_Chaos_Mastery    =  2
    wsa_Nature_Mastery   =  3
    wsa_Sorcery_Mastery  =  4
    wsa_Infernal_Power   =  5
    wsa_Divine_Power     =  6
    wsa_Sage_Master      =  7
    wsa_Channeller       =  8
    wsa_Myrran           =  9
    wsa_Archmage         = 10
    wsa_Mana_Focusing    = 11
    wsa_Node_Mastery     = 12
    wsa_Famous           = 13
    wsa_Runemaster       = 14
    wsa_Conjurer         = 15
    wsa_Charismatic      = 16
    wsa_Artificer        = 17

0:
1:
2:
...
17:

Sorcery Mastery: 

Node Mastery: To select Node Mastery you need: 1 pick in Chaos Magic, 1 pick in Nature Magic, 1 pick in Sorcery Magic

Runemaster: To select Runemaster you need: 2 picks in any 3 Realms of Magic

*/
int16_t wsa_prerequisites[NUM_WIZARD_SPECIAL_ABILITIES][7] =
{
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 1, 0, 0, 4, 0, 0 },  /* Nature */
    { 0, 1, 0, 0, 0, 4, 0 },  /* Sorcery Mastery */
    { 0, 1, 0, 0, 0, 0, 4 },  /* Chaos */
    { 0, 1, 0, 4, 0, 0, 0 },  /* Death */
    { 0, 1, 4, 0, 0, 0, 0 },  /* Life */
    { 1, 2, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { 4, 1, 0, 0, 0, 0, 0 },  /* Mana Focusing or Archmage */
    { 4, 1, 0, 0, 0, 0, 0 },  /* Mana Focusing or Archmage */
    { 0, 3, 0, 0, 1, 1, 1 },  /* Node Mastery */
    { 0, 0, 0, 0, 0, 0, 0 },
    { 2, 3, 0, 0, 0, 0, 0 },  /* Runemaster */
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
};

// MGC  dseg:2DE8
int16_t wsa_picklist_order[NUM_WIZARD_SPECIAL_ABILITIES] =
{
    wsa_Alchemy,         /*  0 */
    wsa_Warlord,         /*  1 */
    wsa_Channeller,      /*  2 */
    wsa_Archmage,        /*  3 */
    wsa_Artificer,       /*  4 */
    wsa_Conjurer,        /*  5 */
    wsa_Sage_Master,     /*  6 */
    wsa_Myrran,          /*  7 */
    wsa_Divine_Power,    /*  8 */
    wsa_Famous,          /*  9 */
    wsa_Runemaster,      /* 10 */
    wsa_Charismatic,     /* 11 */
    wsa_Chaos_Mastery,   /* 12 */
    wsa_Nature_Mastery,  /* 13 */
    wsa_Sorcery_Mastery, /* 14 */
    wsa_Infernal_Power,  /* 15 */
    wsa_Mana_Focusing,   /* 16 */
    wsa_Node_Mastery     /* 17 */
};

// MGC  dseg:2E0C 66 66 66 66                                     COL_SaveLoad1 db 4 dup( 66h)            ; DATA XREF: Load_Screen_Draw+Br ...
// MGC  dseg:2E10 60 61 60 61                                     COL_SaveLoad2 db  60h, 61h, 60h, 61h    ; DATA XREF: Load_Screen_Draw+18r ...

// MGC  dseg:2E14
uint8_t RP_COL_NEWG_4Shadow[4] = { 31, 31, 31, 31 };
// MGC  dseg:2E18
uint8_t RP_COL_NEWG_4Font[4] = { 187, 187, 187, 187 };

// MGC  dseg:2E1C
/*
¿ struct ?
55 bytes
5 * 11
*/
// cnst_Diff0_Text_11b db 'Intro',0,0,0,0,0,0
// cnst_Diff1_Text_11b db 'Easy',0,0,0,0,0,0,0
// cnst_Diff2_Text_11b db 'Normal',0,0,0,0,0
// cnst_Diff3_Text_11b db 'Hard',0,0,0,0,0,0,0
// cnst_Diff4_Text_11b db 'Impossible',0
struct s_DIFFICULTY_NAMES
{
    char name[11];
};
struct s_DIFFICULTY_NAMES _difficulty_names[5] =
{
    { 'I', 'n', 't', 'r', 'o',  '\0', '\0', '\0', '\0', '\0', '\0' },
    { 'E', 'a', 's', 'y', '\0', '\0', '\0', '\0', '\0', '\0', '\0' },
    { 'N', 'o', 'r', 'm', 'a',  'l',  '\0', '\0', '\0', '\0', '\0' },
    { 'H', 'a', 'r', 'd', '\0', '\0', '\0', '\0', '\0', '\0', '\0' },
    { 'I', 'm', 'p', 'o', 's',  's',  'i',  'b',  'l',  'e',  '\0' }
};

// MGC  dseg:2E53
// cnst_LandS0_Text_7b db 'Small',0,0
// cnst_LandS1_Text_7b db 'Medium',0
// cnst_LandS2_Text_7b db 'Large',0,0
struct s_LAND_SIZE_NAMES
{
    char name[7];
};
struct s_LAND_SIZE_NAMES _land_size_names[3] =
{
    { 'S', 'm', 'a', 'l', 'l', '\0', '\0' },
    { 'M', 'e', 'd', 'i', 'u', 'm',  '\0' },
    { 'L', 'a', 'r', 'g', 'e', '\0', '\0' }
};

// MGC  dseg:2E68
// cnst_Magic0_Text_9b db 'Weak',0,0,0,0,0
// cnst_Magic1_Text_9b db 'Normal',0,0,0
// cnst_Magic2_Text_9b db 'Powerful',0
struct s_MAGIC_STRENGTH_NAMES
{
    char name[9];
};
struct s_MAGIC_STRENGTH_NAMES _magic_strength_names[3] =
{
    { 'W', 'e', 'a', 'k', '\0', '\0', '\0', '\0', '\0' },
    { 'N', 'o', 'r', 'm', 'a',  'l',  '\0', '\0', '\0' },
    { 'P', 'o', 'w', 'e', 'r',  'f',  'u',  'l',  '\0' }
};

// MGC  dseg:2E83
// cnst_Opnts0_Text_8b db 'One',0,0,0,0,0
// cnst_Opnts1_Text_8b db 'Two',0,0,0,0,0
// cnst_Opnts2_Text_8b db 'Three',0,0,0
// cnst_Opnts3_Text_8b db 'Four',0,0,0,0
struct s_OPPONENT_COUNT_NAMES
{
    char name[8];
};
struct s_OPPONENT_COUNT_NAMES _opponent_count_names[4] =
{
    { 'O', 'n', 'e', '\0', '\0', '\0', '\0', '\0' },
    { 'T', 'w', 'o', '\0', '\0', '\0', '\0', '\0' },
    { 'T', 'h', 'r', 'e',  'e',  '\0', '\0', '\0' },
    { 'F', 'o', 'u', 'r',  '\0', '\0', '\0', '\0' }
};

// MGC  dseg:2EA3
uint8_t COL_NEWG_FontShadow[8] = { 31, 31, 31, 31, 31, 31, 31, 0 };
// MGC  dseg:2EAB
uint8_t COL_NEWG_Font[8] = { 187, 187, 187, 187, 187, 187, 187, 187 };
// MGC  dseg:2EB3
uint8_t alias_colors[2] = {183, 178};
// MGC  dseg:2EB5
uint8_t RP_COL_NEWG_FontShadow2[8] = { 31, 31, 31, 31, 31, 31, 31, 0 };
// MGC  dseg:2EBD
uint8_t RP_COL_NEWG_Font2[8] = { 187, 187, 187, 187, 187, 187, 187, 187 };

// MGC  dseg:2EC5
uint8_t RP_COL_NEWG_Font3[8] = { 187, 187, 187, 187, 187, 187, 187, 187 };

// MGC  dseg:2ECD
/*
used for player name in Newgame_Screen_7_Draw__WIP()
    Set_Font_Colors_15(4, &shadow_colors[0]);
    Set_Font_Colors_15(4, &text_colors[0]);
*/
uint8_t RP_COL_NEWG_5Shadow[5] = { 31, 31, 31, 31, 31 };
uint8_t RP_COL_NEWG_5Font[5] = { 187, 187, 187, 187, 187 };

// MGC  dseg:2ED7 00                                              align 2                                 ; 2ed6 is still free

// MGC  dseg:2ED8
int16_t TBL_Arcanian_Races[9] = { R_Barbarian, R_Gnoll, R_Halfling, R_High_Elf, R_High_Man, R_Klackon, R_Lizardman, R_Nomad, R_Orc };

// MGC  dseg:2EEA
int16_t TBL_Myrran_Races[5] = { R_Beastman, R_Dark_Elf, R_Draconian, R_Dwarf, R_Troll };

// MGC  dseg:2EF4
uint8_t RP_COL_NEWG_Font4[8] = { 187, 187, 187, 187, 187, 187, 187, 187 };

// MGC  dseg:2EFC
uint8_t COL_NEWG_Highlight[2] = { 20, 176 };
// MGC  dseg:2EFE
uint8_t COL_Available[2] = { 25, 50 };
// MGC  dseg:2F00
uint8_t COL_NEWG_RacesShadow[2] = { 2, 2 };

// MGC  dseg:2F02
int16_t RP_TBL_Arcanian_Races_2[9] = { R_Barbarian, R_Gnoll, R_Halfling, R_High_Elf, R_High_Man, R_Klackon, R_Lizardman, R_Nomad, R_Orc };

// MGC  dseg:2F14
int16_t RP_TBL_Myrran_Races_2[5] = { R_Beastman, R_Dark_Elf, R_Draconian, R_Dwarf, R_Troll };

// MGC  dseg:2F1E
uint8_t RP_COL_NEWG_5Shadow2[5] = { 31, 31, 31, 31, 31 };

// MGC  dseg:2F23
uint8_t RP_COL_NEWG_5Font2[5] = {187, 187, 187, 187, 187};

// MGC  dseg:2F28
uint8_t COL_NEWG_RaceTitles[5] = { 180, 180, 180, 180, 180 };

// MGC  dseg:2F2D
uint8_t COL_NEWG_Highlight2[2] = { 20, 176 };

// MGC  dseg:2F2F
uint8_t COL_Available_2[2] = { 25, 50 };
// MGC  dseg:2F31
uint8_t COL_NEWG_NA[2] = { 25, 22 };
// MGC  dseg:2F33
uint8_t COL_NEWG_RacesShdw2[2] = { 2, 2};

// MGC  dseg:2F35
uint8_t string_colors1[4] = { 187, 187, 187, 187 };
// MGC  dseg:2F39
uint8_t string_colors2[2] = { 183, 178 };

// MGC  dseg:2F3B 00                                              align 2

// MGC  dseg:2F3C
uint8_t m_niu_array[12] = { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 };

// MGC  dseg:2F48
char realm_name_character_array[5][10] =
{
    {'L','i','f','e',0,0,0,0,0,0},
    {'D','e','a','t','h',0,0,0,0,0},
    {'C','h','a','o','s',0,0,0,0,0},
    {'N','a','t','u','r','e',0,0,0,0},
    {'S','o','r','c','e','r','y',0,0,0}
};

// MGC  dseg:2F7A
int16_t TBL_2pickRetort_Lbls[5] = { 1, 2, 8, 9, 15 };
// TBL_2pickRetort_Lbls dw RETORT_Labels.Warlord, RETORT_Labels.Channeler, RETORT_Labels.Divine_Power, RETORT_Labels.Famous, RETORT_Labels.Infernal_Power

// MGC  dseg:2F84
int16_t RP_TBL_2pickRet_Lbls[5] = { 1, 2, 8, 9, 15 };
// RP_TBL_2pickRet_Lbls dw RETORT_Labels.Warlord, RETORT_Labels.Channeler, RETORT_Labels.Divine_Power, RETORT_Labels.Famous, RETORT_Labels.Infernal_Power

// MGC  dseg:2F8E
uint8_t COL_NEWG_5Shadow3[5] = { 31, 31, 31, 31, 31 };

// MGC  dseg:2F93
uint8_t COL_NEWG_5Font3[5] = { 187, 187, 187, 187, 187 };

// MGC  dseg:2F98
uint8_t COL_Available_3[2] = { 25, 50};
// MGC  dseg:2F9A
uint8_t COL_NEWG_NA2[2] = { 25, 22};
// MGC  dseg:2F9C
uint8_t COL_NewG_Ret_Known[2] = { 22, 178};
// MGC  dseg:2F9E
uint8_t COL_NewG_Ret_Shdw[2] = { 16, 16 };

// MGC  dseg:2FA0
/*
starting setting for wsa picklist on screen4 wizard creation
1 means the wsa is available for selection, 0 means it is not
*/
int8_t wsa_picklist_start[NUM_WIZARD_SPECIAL_ABILITIES] =
{
    1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0
};

// MGC  dseg:2FB2
int16_t TBL_Realm_Order[5] = { sbr_Life, sbr_Death, sbr_Chaos, sbr_Nature, sbr_Sorcery };

// MGC  dseg:2FBC
/*
y srceen coordinate of the top of the section title
*/
int16_t m_niu_title_start_y[3] = { 26, 77, 128 };
// MGC  dseg:2FC2
/*
y srceen coordinate of the top of the section box
*/
int16_t m_niu_box_start_y[3] = { 37, 88, 139 };

// MGC  dseg:2FC8
uint8_t m_dark_colors[5] = { 31, 31, 31, 31, 31 };

// MGC  dseg:2FCD
uint8_t m_light_colors[5] = { 187, 187, 187, 187, 187 };

// MGC  dseg:2FD2
int16_t Label_Box_TitleTops2[3] = { 26, 77, 128 };
// MGC  dseg:2FD8
int16_t Label_Box_TextTops2[3] = { 37, 88, 139 };

// MGC  dseg:2FDE
uint8_t m_select_spells_deselected_colors[2] = { 25, 50 };

// MGC  dseg:2FE0
uint8_t m_select_spells_selected_colors[2] = { 22, 178 };

// MGC  dseg:2FE2
uint8_t m_select_spells_shadow_colors[2] = { 16, 16 };

// MGC  dseg:2FE4
char m_rarity_strings[4][10] =
{
    { 'C','o','m','m','o','n',  0,  0,  0, 0 },
    { 'U','n','c','o','m','m','o','n',  0, 0 },
    { 'R','a','r','e',  0,  0,  0,  0,  0, 0 },
    { 'V','e','r','y',' ','R','a','r','e', 0 }
};

// MGC  dseg:300C
char m_realm_strings[5][10] =
{
    { 'N','a','t','u','r','e',  0,  0,  0,  0 },
    { 'S','o','r','c','e','r','y',  0,  0,  0 },
    { 'C','h','a','o','s',  0,  0,  0,  0,  0 },
    { 'L','i','f','e',  0,  0,  0,  0,  0,  0 },
    { 'D','e','a','t','h',  0,  0,  0,  0,  0 }
};

// MGC  dseg:303E 57 49 5A 41 52 44 53 2E 45 58 45                cnst_EXESwap_File db 'WIZARDS.EXE'      ; DATA XREF: GAME_WizardsLaunch+38o

// MGC  dseg:3049
char empty_string__ovr050[] = "";

// MGC  dseg:304A 4A 45 4E 4E 59 00                               cnst_EXESwap_Arg db 'JENNY',0           ; DATA XREF: GAME_WizardsLaunch+30o ...
// MGC  dseg:3050 4C 4F 41 44 2E 4C 42 58 00                      load_lbx_file db 'LOAD.LBX',0           ; DATA XREF: Load_Screen+Fo ...

// MGC  dseg:3059
char str_ESC__ovr050[] = "\x1B";

// MGC  dseg:305B 4C 00                                           cnst_HOTKEY_L_2 db 'L',0                ; DATA XREF: Load_Screen+196o
// MGC  dseg:305D 53 41 56 45 00                                  cnst_SAVE3 db 'SAVE',0                  ; DATA XREF: Load_Screen:@@Loop_SaveSlotso
// MGC  dseg:3062 2E 47 41 4D 00                                  cnst_SAVE_ext3 db '.GAM',0              ; DATA XREF: Load_Screen+1FDo
// MGC  dseg:3067 43 6F 70 79 72 69 67 68 74 20 20 53 69 6D 74 65+load_screen_copyright db 'Copyright  Simtex Software, 1995   V1.31',0

// MGC  dseg:3090
char newgame_lbx_file__ovr050[] = "NEWGAME.LBX";

// MGC  dseg:309C
char str_MAGIC_SET__ovr050[] = "MAGIC.SET";

// MGC  dseg:30A6
char str_rb__ovr050[] = "rb";

// MGC  dseg:30A9
char str_wb__ovr050[] = "wb";

// MGC  dseg:30AC
char cnst_Wiz_Select[] = "Select Wizard";

// MGC  dseg:30BA
char cnst_Pic_Select[] = "Select Picture";

// MGC  dseg:30C9
char cnst_DOT__ovr050[] = ".";

// MGC  dseg:30CB
char cnst_Name_Select[] = "Wizard's Name";

// MGC  dseg:30D9
char cnst_Banner_Select[] = "Select Banner";

// MGC  dseg:30E7
char cnst_Race_Error[] = "You can not select a Myrran race unless you have the Myrran special.";
// MGC  dseg:312C
char cnst_Race_Select[] = "Select Race";
// MGC  dseg:3138
char cnst_Arcanian_Races[] = "Arcanian Races:";
// MGC  dseg:3148
char cnst_Myrran_Races[] = "Myrran Races:";

// MGC  dseg:3156
char cnst_Pick_Error_0[] = "You need to make all your picks before you can continue";
char cnst_Pick_Error_1[] = "You may not select more than 6 special abilities";
char cnst_Pick_Error_20[] = "To select ";
char cnst_Pick_Error_21[] = " you need:   ";
char cnst_Pick_Error_22[] = " picks in ";
char cnst_Pick_Error_23[] = " Magic";
char cnst_Pick_Error_24[] = " pick";
char cnst_Pick_Error_25[] = "s";
char cnst_Pick_Error_26[] = " in any ";
char cnst_Pick_Error_27[] = " Realms of Magic";
char cnst_Pick_Error_28[] = "Realm of Magic";
char cnst_Pick_Error_29[] = ", ";
char cnst_Pick_Error_2A[] = " pick in ";
char cnst_Pick_Error_3[] = "You can not select both Life and Death magic";
char cnst_Pick_Error_4[] = "You have already made all your";
char cnst_Picks[] = " picks";
char cnst_Pick_Error_5[] = "You don't have enough picks left to make this selection. You need 2 picks";

// MGC  dseg:32C3
char hlpentry_lbx_file__MGC_ovr050[] = "hlpentry.lbx";

// MGC  dseg:32D0
char cnst_Pick_Error_6[] = "You have no picks left in this area, to deselect click on a selected item";

// MGC  dseg:331A
char cnst_Spell_Select_0[] = "Select ";
// MGC  dseg:3322
char cnst_Spell_Select_1[] = " Spells";
// MGC  dseg:332A
char cnst_Spell_Select_2[] = ": ";

// MGC  dseg:332D
char cnst_And__ovr050[] = " and ";

// MGC  dseg:3333 00                                              align 2

// MGC  dseg:3333                                                 END:  ovr050 - Initialized Data



// MGC  dseg:8A42                                                 BEGIN:  ovr050 - Uninitialized Data

// // // // MGC  dseg:8A42
// // // struct Default_Spells TBL_Spells_Nature;
// // // // MGC  dseg:8A5C
// // // struct Default_Spells TBL_Spells_Sorcery;
// // // // MGC  dseg:8A76
// // // struct Default_Spells TBL_Spells_Chaos;
// // // // MGC  dseg:8A90
// // // struct Default_Spells TBL_Spells_Life;
// // // // MGC  dseg:8AAA
// // // struct Default_Spells TBL_Spells_Death;
// // // MGC  dseg:8A42
// // int16_t _start_spells_nature[13];
// // // MGC  dseg:8A5C
// // int16_t _start_spells_sorcery[13];
// // // MGC  dseg:8A76
// // int16_t _start_spells_chaos[13];
// // // MGC  dseg:8A90
// // int16_t _start_spells_life[13];
// // // MGC  dseg:8AAA
// // int16_t _start_spells_death[13];
// // MGC  dseg:8AC4
// struct Default_Spells TBL_Spells_P1;
// // MGC  dseg:8B46
// struct Default_Spells TBL_Spells_P2;
// // MGC  dseg:8BC8
// struct Default_Spells TBL_Spells_P3;
// // MGC  dseg:8C4A
// struct Default_Spells TBL_Spells_P4;
// // MGC  dseg:8CCC
// struct Default_Spells UU_TBL_Spells_P5;

// MGC  dseg:8A42
struct s_Init_Base_Realms _player_start_spells[PLAYER_COUNT_MAX];

// MGC  dseg:8D4E
int16_t NEWG_PickAttempt;
// MGC  dseg:8D50
int16_t NEWG_PickError;

// MGC  dseg:8D52
// ; initialized to random values of 0, 1, or 2
int16_t TBL_Bookshelf_Books[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
// MGC  dseg:8D68
int16_t TBL_Realm0_Books[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
// MGC  dseg:8D7E
int16_t TBL_Realm1_Books[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
// MGC  dseg:8D94
int16_t TBL_Realm2_Books[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
// MGC  dseg:8DAA
int16_t TBL_Realm3_Books[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
// MGC  dseg:8DC0
int16_t TBL_Realm4_Books[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// MGC  dseg:8DD6
// drake78: IMG_NEWG_MapBuildBG
/*
    loaded in GAME_New_Screen_7()
    used in Init_New_Game()
MoO2  Module: MAPGEN  _fill_msg_bitmap  <-|  Module: MISC  Draw_Advancing_Fill_Message_()

*/
SAMB_ptr newgame_BUILDWOR_map_build_bar_seg = 0;

// MGC  dseg:8DD8
int16_t m_bookshelf_sorcery;
// MGC  dseg:8DDA
int16_t m_bookshelf_nature;
// MGC  dseg:8DDC
int16_t m_bookshelf_chaos;
// MGC  dseg:8DDE
int16_t m_bookshelf_death;
// MGC  dseg:8DE0
int16_t m_bookshelf_life;

// MGC  dseg:8DE2
/*
set to 8 in Newgame_Screen_1__WIP() set-up portion

"Select Wizard" vs. "Select Picture"

WTF?
init'd to 0, defaulted to 8, never changed

*/
int16_t NEWG_PortraitSelType = 0;

// MGC  dseg:8DE4 00                                              db    0
// MGC  dseg:8DE5 00                                              db    0
// MGC  dseg:8DE6
// ; 5 groups of 3 images each (L - S - N - D - C)
SAMB_ptr m_spellbook_pict_segs[15] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// MGC  dseg:8E04
int16_t newgame_landsize_button_field = 0;
// MGC  dseg:8E06
int16_t newgame_magic_button_field = 0;
// MGC  dseg:8E08
int16_t newgame_opponents_button_field = 0;
// MGC  dseg:8E0A
int16_t newgame_difficulty_button_field = 0;

// MGC  dseg:8E0C
/*
MoO2  _race_button_seg
used for Wizard and Race
*/
int16_t m_newgame_fields[30] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
// drake178: NEWG_Moused_Wizard
/*
~== MoO2 _displayed_race
*/
int16_t m_displayed_wizard = 0;

// MGC  dseg:8E4A
SAMB_ptr m_shared_pict_segs[15] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// MGC  dseg:8E68
/*

Newgame_Screen_0()
NEWGAME.LBX, 001  NEWGAMP2    New game border
IMG_NewG_RgtOverlay = LBX_Reload_Next(newgame_lbx_file__ovr050, 1, _screen_seg);

Newgame_Screen_3__WIP()
NEWGAME.LBX, 040  NEWGMNAM   Name box
IMG_NewG_RgtOverlay = LBX_Reload_Next(newgame_lbx_file__ovr050, 40, _screen_seg);
*/
SAMB_ptr IMG_NewG_RgtOverlay = 0;

// MGC  dseg:8E6A
int16_t wsa_picklist_fields[NUM_WIZARD_SPECIAL_ABILITIES];

// MGC  dseg:8E8E 00 00                                           _quit_active_seg dw 0                   ; DATA XREF: Load_Screen+32w ...
// MGC  dseg:8E90 00 00                                           _load_active_seg dw 0                   ; DATA XREF: Load_Screen+49w ...
// MGC  dseg:8E92 00 00                                           _ok_button dw 0                         ; DATA XREF: Load_Screen+1BDw ...
// MGC  dseg:8E94
int16_t spellpicks_count;

// MGC  dseg:8E96
SAMB_ptr _selection_marker_seg;

// MGC  dseg:8E98
// WZD dseg:C9B2
// LoadScr.c  int16_t selected_load_game_slot_idx;
extern int16_t selected_load_game_slot_idx;

// MGC  dseg:8E9A 00 00                                           MAY__selected_save_game_slot_idx dw 0

// MGC  dseg:8E9C
int16_t m_select_spells_realm;

// MGC  dseg:8E9E 00 00                                           _settings_button dw 0                   ; DATA XREF: Load_Screen+2D5w ...
// MGC  dseg:8EA0 00 00                                           _settings_button_seg dw 0               ; DATA XREF: Load_Screen+BCw ...
// MGC  dseg:8EA2 00 00                                           _text_fill_seg dw 0                     ; DATA XREF: Load_Screen+A5w ...

// MGC  dseg:8EA4
/*
 * NEWG_Clicked_Race
 *
 * Holds the enum_RACE_BYTE value of the race the human player (player 0) selected on New Game Screen 6 ("Select Race").
 *
 * Written by Newgame_Screen_6__WIP() when the player clicks one of the 14 race-name labels in the race list panel.  If the clicked
 * label index falls in [0..8], the value is looked up from the local Arcanus_Races[9] table (Barbarian, Gnoll, Halfling, High Elf,
 * High Man, Klackon, Lizardman, Nomad, Orc).  If the index falls in [9..13] AND the player has the Myrran special ability, the value is
 * looked up from the local Myrran_Races[5] table (Beastman, Dark Elf, Draconian, Dwarf, Troll); otherwise the click is rejected
 * and a warning dialog is shown ("You can not select a Myrran race unless you have the Myrran special.").
 *
 * After the selection loop exits, Newgame_Screen_6__WIP() uses NEWG_Clicked_Race to drive the four-iteration "blink" animation:
 * the chosen race name is redrawn in alternating highlight/dim colors so the player gets visual feedback before advancing to the
 * next new-game screen (Screen 7 -- "Select Banner").
 *
 * Consumed downstream by MAPGEN's city-placement logic: when itr==0 (i.e. the human player), the race of the player's starting
 * city is assigned directly from this variable (_CITIES[_cities].race = (int8_t)NEWG_Clicked_Race).  AI players do not use this
 * variable; their starting-city races are chosen randomly from the Myrran or Arcanian race pools according to difficulty and the
 * Myrran special ability.
 *
 * Type is int16_t (matching the original 16-bit word at dseg:8EA4) even though enum_RACE_BYTE values fit in a byte, because the
 * original compiled code stores and loads this as a full word in the data segment.
 *
 * Lifetime: set once during new-game setup, read during map generation, not meaningful after the game begins.
 */
int16_t NEWG_Clicked_Race;

// MGC  dseg:8EA6 00 00                                           dw 0
// MGC  dseg:8EA8 00 00                                           dw 0

// MGC  dseg:8EAA
int16_t NEWG_ProfileComplete;

// MGC  dseg:8EAC
/*
NEWGAME.LBX, 043  MAGICOK    non selectable ok
*/
SAMB_ptr _ok_inactive_seg;

// MGC  dseg:8EAE 00 00                                           _save_inactive_seg dw 0                 ; DATA XREF: Load_Screen+77w ...

// MGC  dseg:8EAE                                                 END:  ovr050 - Uninitialized Data



// MGC  dseg:8EB0                                                 BEGIN:  ovr051 - Uninitialized Data

// MGC  dseg:8EB0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+m_landmasses_default_race dw 200 dup(0)       ; DATA XREF: NEWG_CreateNeutrals+39w ...
// MGC  dseg:9040 00 00                                           m_landmasses_ctr dw 0                 ; DATA XREF: NEWG_ClearLandmasses+4w ...
// MGC  dseg:9042 00 00                                           dw 0
// MGC  dseg:9044 00 00                                           dw 0
// MGC  dseg:9046 00 00                                           dw 0
// MGC  dseg:9048 00 00                                           dw 0
// MGC  dseg:904A 00 00                                           dw 0

// MGC  dseg:904A                                                 END:  ovr051 - Uninitialized Data



// MGC  dseg:904E                                                 BEGIN:  ovr053 - Uninitialized Data

// MGC  dseg:904E 00 00                                           HLP_ExpandLines dw 0                    ; DATA XREF: UU_HLP_DrawExpanding+Bw ...
// MGC  dseg:9050 00 00                                           HLP_ExpandOrigBottom dw 0               ; DATA XREF: UU_HLP_DrawExpanding+3Bw ...
// MGC  dseg:9052 00 00                                           HLP_ExpandOrigRight dw 0                ; DATA XREF: UU_HLP_DrawExpanding+35w ...
// MGC  dseg:9054 00 00                                           HLP_ExpandOrigTop dw 0                  ; DATA XREF: UU_HLP_DrawExpanding+2Fw ...
// MGC  dseg:9056 00 00                                           HLP_ExpandOrigLeft dw 0                 ; DATA XREF: UU_HLP_DrawExpanding+29w ...
// MGC  dseg:9058 00 00                                           HLP_ExpandTgtBottom dw 0                ; DATA XREF: UU_HLP_DrawExpanding+23w ...
// MGC  dseg:905A 00 00                                           HLP_ExpandTgtRight dw 0                 ; DATA XREF: UU_HLP_DrawExpanding+1Dw ...
// MGC  dseg:905C 00 00                                           HLP_ExpandTgtTop dw 0                   ; DATA XREF: UU_HLP_DrawExpanding+17w ...
// MGC  dseg:905E 00 00                                           HLP_ExpandTgtLeft dw 0                  ; DATA XREF: UU_HLP_DrawExpanding+11w ...
// MGC  dseg:9060 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+GUI_ListSel_Buttons dw 0Ah dup(     0)  ; DATA XREF: UU_GUI_ListSelect_Dlg+217w ...
// MGC  dseg:9074 00 00                                           GUI_ListSel_Title@ dw 0                 ; DATA XREF: UU_GUI_ListSelect_Dlg+1CBw ...
// MGC  dseg:9076 00 00                                           GUI_ListSel_Strings@ dw 0               ; DATA XREF: UU_GUI_ListSelect_Dlg+1C5w ...
// MGC  dseg:9078 00 00                                           GUI_ListSel_DispCnt dw 0                ; DATA XREF: UU_GUI_ListSelect_Dlg+1BFw ...
// MGC  dseg:907A 00 00                                           GUI_ListSel_Disp1st dw 0                ; DATA XREF: UU_GUI_ListSelect_Dlg+1CEw ...
// MGC  dseg:907C 00 00                                           GUI_ListSelHighlight dw 0               ; DATA XREF: UU_GUI_ListSelect_Dlg+1D4w ...
// MGC  dseg:907E 00 00                                           dw 0
// MGC  dseg:9080 00 00                                           GUI_ListSel_Bottom dw 0                 ; DATA XREF: UU_GUI_ListSelect_Dlg+441r ...
// MGC  dseg:9082 00 00                                           GUI_ListSel_Right dw 0                  ; DATA XREF: UU_GUI_ListSelect_Dlg+2EFr ...
// MGC  dseg:9084 00 00                                           GUI_ListSel_Top dw 0                    ; DATA XREF: UU_GUI_ListSelect_Dlg+2DAr ...
// MGC  dseg:9086 00 00                                           GUI_ListSel_Left dw 0                   ; DATA XREF: UU_GUI_ListSelect_Dlg+321r ...
// MGC  dseg:9088 00 00                                           GUI_ListSel_MultiPg dw 0                ; DATA XREF: UU_GUI_ListSelect_Dlg+1B9w ...
// MGC  dseg:908A 00 00                                           GUI_ListSel_Count dw 0                  ; DATA XREF: UU_GUI_ListSelect_Dlg+264r ...
// MGC  dseg:908C 00 00                                           IMG_GUI_NoButton@ dw 0                  ; DATA XREF: UU_GUI_Confirm_Dialog+78w ...
// MGC  dseg:908C                                                                                         ; 2 frame image appended in the LBX_Sandbox_Segment
// MGC  dseg:908E 00 00                                           IMG_GUI_YesButton@ dw 0                 ; DATA XREF: UU_GUI_Confirm_Dialog+61w ...
// MGC  dseg:908E                                                                                         ; 2 frame image appended in the LBX_Sandbox_Segment
// MGC  dseg:9090 00 00                                           IMG_GUI_Confirm_Btm@ dw 0               ; DATA XREF: UU_GUI_Confirm_Dialog:loc_4D5FAw ...
// MGC  dseg:9090                                                                                         ; single image appended in the LBX_Sandbox_Segment
// MGC  dseg:9092 00 00                                           IMG_GUI_Confirm_BG@ dw 0                ; DATA XREF: UU_GUI_Confirm_Dialog+33w ...
// MGC  dseg:9092                                                                                         ; LBXE_LoadReplace into the LBX_Sandbox_Segment
// MGC  dseg:9094 00 00                                           GUI_Notify_IMG2_Left dw 0               ; DATA XREF: UU_GUI_Notify_Dialog+11Cw ...
// MGC  dseg:9096 00 00                                           GUI_Notify_IMG2_Top dw 0                ; DATA XREF: UU_GUI_Notify_Dialog+122w ...
// MGC  dseg:9098 00 00                                           GUI_Notify_IMG_Top dw 0                 ; DATA XREF: UU_GUI_Notify_Dialog+116w ...
// MGC  dseg:909A 00 00                                           GUI_Notify_IMG_Left dw 0                ; DATA XREF: UU_GUI_Notify_Dialog+110w ...
// MGC  dseg:909C 00 00                                           GUI_Notify_GemBorder dw 0               ; DATA XREF: UU_GUI_Notify_Dialog+10Aw ...
// MGC  dseg:909E 00 00                                           IMG_GUI_Dialog_Pic2@ dw 0               ; DATA XREF: UU_GUI_Notify_Dialog+100w ...
// MGC  dseg:90A0 00 00                                           IMG_GUI_Dialog_Pic@ dw 0                ; DATA XREF: UU_GUI_Notify_Dialog+F9w ...
// MGC  dseg:90A2 00 00                                           GUI_Notify_Color dw 0                   ; DATA XREF: UU_GUI_Notify_Dialog+103w ...
// MGC  dseg:90A2                                                                                         ; blue - red - green - brown
// MGC  dseg:90A4 00 00                                           dw 0
// MGC  dseg:90A6 00 00                                           IMG_GUI_BlueNtfyGem@ dw 0               ; DATA XREF: UU_GUI_LoadNotifyGfx+81w ...
// MGC  dseg:90A8 00 00                                           IMG_GUI_RedNtfyGem@ dw 0                ; DATA XREF: UU_GUI_LoadNotifyGfx+C9w
// MGC  dseg:90AA 00 00                                           IMG_GUI_GrnNtfyGem@ dw 0                ; DATA XREF: UU_GUI_LoadNotifyGfx+111w
// MGC  dseg:90AC 00 00                                           IMG_GUI_BrwnNtfyGem@ dw 0               ; DATA XREF: UU_GUI_LoadNotifyGfx+158w
// MGC  dseg:90AE 00 00                                           IMG_GUI_BlueNtfyPic@ dw 0               ; DATA XREF: UU_GUI_LoadNotifyGfx+6Aw ...
// MGC  dseg:90B0 00 00                                           IMG_GUI_RedNtfyPic@ dw 0                ; DATA XREF: UU_GUI_LoadNotifyGfx+B2w
// MGC  dseg:90B2 00 00                                           IMG_GUI_GrnNtfyPic@ dw 0                ; DATA XREF: UU_GUI_LoadNotifyGfx+FAw
// MGC  dseg:90B4 00 00                                           IMG_GUI_BrwnNtfyPic@ dw 0               ; DATA XREF: UU_GUI_LoadNotifyGfx+141w
// MGC  dseg:90B6 00 00                                           IMG_GUI_BlueNotify@ dw 0                ; DATA XREF: UU_GUI_LoadNotifyGfx+53w ...
// MGC  dseg:90B8 00 00                                           IMG_GUI_RedNotify@ dw 0                 ; DATA XREF: UU_GUI_LoadNotifyGfx+9Bw
// MGC  dseg:90BA 00 00                                           IMG_GUI_GrnNotify@ dw 0                 ; DATA XREF: UU_GUI_LoadNotifyGfx+E3w
// MGC  dseg:90BC 00 00                                           IMG_GUI_BrwnNotify@ dw 0                ; DATA XREF: UU_GUI_LoadNotifyGfx+12Aw
// MGC  dseg:90BE 00 00                                           GUI_ColorSlideType dw 0                 ; DATA XREF: UU_GUI_Notify_Dialog+AFw ...
// MGC  dseg:90C0                                                 ; int GUI_ColorSlide_State
// MGC  dseg:90C0 00 00                                           GUI_ColorSlide_State dw 0               ; DATA XREF: UU_GUI_Notify_Dialog+B2w ...
/*
    BEGIN:  Warning Box
*/
// MGC  dseg:90C2 00 00                                           m_warning2_box_bottom_seg dw 0          ; DATA XREF: Warning_Message+68w ...
// MGC  dseg:90C2                                                                                         ; appended into the LBX_Sandbox_Segment
// MGC  dseg:90C4 00 00                                           m_warning2_box_top_seg dw 0             ; DATA XREF: Warning_Message+51w ...
// MGC  dseg:90C4                                                                                         ; appended into the LBX_Sandbox_Segment
// MGC  dseg:90C6 00 00                                           m_warning_box_bottom_seg dw 0           ; DATA XREF: Newgame_Screen_6__WIP+BFw ...
// MGC  dseg:90C6                                                                                         ; appended into the LBX_Sandbox_Segment
// MGC  dseg:90C8 00 00                                           m_warning_box_top_seg dw 0              ; DATA XREF: Newgame_Screen_6__WIP+A8w ...
// MGC  dseg:90C8                                                                                         ; LBXE_LoadReplace into the LBX_Sandbox_Segment
/*
    END:  Warning Box
*/
/*
    BEGIN:  Selection Box
*/
// MGC  dseg:90CA 00 00                                           IMG_GUI_ListSelLkDn@ dw 0               ; DATA XREF: UU_GUI_ListSelect_Dlg+144w ...
// MGC  dseg:90CC 00 00                                           IMG_GUI_ListSelLkUp@ dw 0               ; DATA XREF: UU_GUI_ListSelect_Dlg+12Dw ...
// MGC  dseg:90CE 00 00                                           IMG_GUI_ListS_BScrl@ dw 0               ; DATA XREF: UU_GUI_ListSelect_Dlg+116w ...
// MGC  dseg:90D0 00 00                                           IMG_GUI_ListS_TScrl@ dw 0               ; DATA XREF: UU_GUI_ListSelect_Dlg+FFw ...
// MGC  dseg:90D2 00 00                                           IMG_GUI_ListSelDnAr@ dw 0               ; DATA XREF: UU_GUI_ListSelect_Dlg+E8w ...
// MGC  dseg:90D4 00 00                                           IMG_GUI_ListSelUpAr@ dw 0               ; DATA XREF: UU_GUI_ListSelect_Dlg+D1w ...
// MGC  dseg:90D6 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+IMG_GUI_ListS_IBGRs@ dw 0Ah dup(     0) ; DATA XREF: UU_GUI_ListSelect_Dlg+181w ...
// MGC  dseg:90D6 00 00 00 00                                                                             ; array of 5 2-frame images appended into the sandbox
// MGC  dseg:90EA 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+IMG_GUI_ListS_IBGs@ dw 0Ah dup(     0)  ; DATA XREF: UU_GUI_ListSelect_Dlg+165w ...
// MGC  dseg:90EA 00 00 00 00                                                                             ; array of 5 2-frame images appended into the sandbox
// MGC  dseg:90FE 00 00                                           IMG_GUI_ListSelScrl@ dw 0               ; DATA XREF: UU_GUI_ListSelect_Dlg+BAw ...
// MGC  dseg:9100 00 00                                           IMG_GUI_ListSel_BR@ dw 0                ; DATA XREF: UU_GUI_ListSelect_Dlg+A3w ...
// MGC  dseg:9102 00 00                                           IMG_GUI_ListSelRght@ dw 0               ; DATA XREF: UU_GUI_ListSelect_Dlg+8Cw ...
// MGC  dseg:9104 00 00                                           IMG_GUI_ListSel_Btm@ dw 0               ; DATA XREF: UU_GUI_ListSelect_Dlg+75w ...
// MGC  dseg:9106 00 00                                           IMG_GUI_ListSel_Top@ dw 0               ; DATA XREF: UU_GUI_ListSelect_Dlg+5Ew ...
// MGC  dseg:9108 00 00                                           IMG_GUI_ListSel_BL@ dw 0                ; DATA XREF: UU_GUI_ListSelect_Dlg+47w ...
// MGC  dseg:910A 00 00                                           IMG_GUI_ListSelLeft@ dw 0               ; DATA XREF: UU_GUI_ListSelect_Dlg+30w ...
// MGC  dseg:910C 00 00                                           GUI_FamiliarIMGIndex dw 0               ; DATA XREF: UU_GUI_Notify_Dialog+16w ...
// MGC  dseg:910E 00 00                                           GUI_Dialog_Text@ dw 0                   ; DATA XREF: UU_GUI_Confirm_Dialog+83w ...
// MGC  dseg:9110 00 00                                           GUI_Dialog_Top dw 0                     ; DATA XREF: UU_GUI_Confirm_Dialog+CAw ...
// MGC  dseg:9112 00 00                                           GUI_Dialog_Left dw 0                    ; DATA XREF: UU_GUI_Confirm_Dialog+B8w ...
// MGC  dseg:9114 00 00                                           m_warning_type dw 0                        ; DATA XREF: UU_GUI_WarningType1+3w ...
// MGC  dseg:9114                                                                                         ; determines whether to use the first or the second
// MGC  dseg:9114                                                                                         ; of the two different warning message backgrounds
// MGC  dseg:9114                                                                                         ; (both of which are red, but slightly different and
// MGC  dseg:9114                                                                                         ; use a different font color)
// MGC  dseg:9116 00 00                                           align 4

// MGC  dseg:9116                                                 END:  ovr053 - Uninitialized Data






/*
    MAGIC.EXE
    ovr050
*/

// MGC  o50p01
// drake178: GAME_New_Create()
/*
; displays and processes the series of screens that
; control the creation of a new game, from generic
; options to wizard customization
; returns 0 if the Esc key is pressed from the first
; screen, but if successful, this function does not
; return, instead saving the created game into the
; continue slot (index 8), and launching WIZARDS.EXE
*/
/*
HACK returns {F,T} did create new game

Newgame_Control();  // MAGIC.EXE  ovr050  o050p001

NOTE(JimBalcomb,20251221): definitely done-done, non-WIP
*/
// void Newgame_Control(void)
/* HACK */  int16_t Newgame_Control(void)
{
    int16_t Create_State = 0;
    int16_t Can_Create = 0;
    int16_t custom_game_flag = 0;  // _DI_
    int16_t newgame_state = 0;  // _SI_

    _landsize = 1;
    _magic = 1;
    _num_players = 1;  // ; NewGame: magic.opponents + 1
    _difficulty = 1;

    newgame_state = 0;

    Can_Create = ST_FALSE;

    while(Can_Create == ST_FALSE)
    {

        Deactivate_Help_List();
        
        Create_State = newgame_state;

        switch(Create_State)
        {
            case ST_UNDEFINED:
            {
                // return;  // ¿ cancel ?
                /* HACK */  return ST_FALSE;
            } break;
            case ngscr_Options:
            {
                newgame_state = Newgame_Screen_0();  // returns 1 on input field is ok button, -1 on quit
            } break;
            case ngscr_Wizard:
            {
                newgame_state = Newgame_Screen_1__WIP();  // returns {0,2,3} - {0:cancel,2:custom,3:prefab}
                if(newgame_state == 2)
                {
                    custom_game_flag = ST_TRUE;
                }
                else
                {
                    custom_game_flag = ST_FALSE;
                }
            } break;
            case ngscr_Portrait:
            {
                newgame_state = Newgame_Screen_2__WIP();
            } break;
            case ngscr_Name:
            {
                newgame_state = Newgame_Screen_3__WIP();
                if(custom_game_flag != ST_FALSE)
                {
                    if(newgame_state == ST_UNDEFINED)  // cancelled, go back a screen
                    {
                        newgame_state = ngscr_Portrait;
                    }
                    else
                    {
                        newgame_state = ngscr_Creation;  // custom wizard creation
                    }
                }
                else  // standard / non-custom
                {
                    if(newgame_state == ST_UNDEFINED)  // cancelled, go back a screen
                    {
                        newgame_state = ngscr_Wizard;
                    }
                    else
                    {
                        newgame_state = ngscr_Race;  // Wizards Race
                    }
                }
            } break;
            case ngscr_Creation:  //  (Custom Wizard) ..."spell pick screen"
            {
                newgame_state = Newgame_Screen_4__WIP();
            } break;
            case ngscr_Spells:  //  (Custom Wizard) ..."select spells screen"
            {
                newgame_state = Newgame_Screen_5();
            } break;
            case ngscr_Race:  // Wizard's Race
            {
                newgame_state = Newgame_Screen_6__WIP();
                if(newgame_state == ST_UNDEFINED)  // Cancel Wizards Race
                {
                    if(custom_game_flag != ST_FALSE)
                    {
                        newgame_state = 5;
                    }
                    else
                    {
                        newgame_state = 3;  // Wizards Name
                    }
                }
            } break;
            case ngscr_Banner:
            {
                newgame_state = Newgame_Screen_7__WIP();
            } break;
            case 99:  /* What sets 99? */
            {
                Can_Create = ST_UNDEFINED;  // ¿ force an early exit of the new game creation process ?
            } break;

        }
        
    }

    // ¿ MoO2  Module: HOMEGEN  Generate_Home_Worlds_() ?
    // ¿ MoO2  Module: INITGAME  Init_New_Game_() ?
    Init_New_Game();

    Initialize_Events();

    NEWG_FinalizeTables__WIP();  // ... wrap it up ... 100%!

    // save new game as continue save
    Save_SAVE_GAM(8);

    // printf("PLATFORM: %s\n", PLATFORM);

    // // Helper macros to stringify the macro value
    // #define STRINGIFY_(msg) #msg
    // #define STRINGIFY(msg) STRINGIFY_(msg)
    // // GCC output example: source.c: note: #pragma message: MY_VALUE=123
    // #pragma message("PLATFORM=" STRINGIFY(PLATFORM))

#ifdef PLATFORM
    printf("SUCCESS: PLATFORM is #defined\n");
#endif

    /* HACK */  return ST_TRUE;

#if   (PLATFORM == DOS16)
    GAME_WizardsLaunch__WIP(8);
#elif (PLATFORM == SDL2)
    current_screen = scr_Main_Screen;
#elif (PLATFORM == LIN64)
    current_screen = scr_Main_Screen;
#elif (PLATFORM == MAC64)
    current_screen = scr_Main_Screen;
#elif (PLATFORM == WIN64)
    current_screen = scr_Main_Screen;
#else
    printf("FAILURE: BAD_PLATFORM_VALUE: Newgame_Control()\n");
#endif

    /* HACK */  return ST_UNDEFINED;
}


// MGC  o50p02
// drake178: GAME_WizardsLaunch()
/*
; fades out the screen, if the passed save index is not
; that of the continue save (index 8), loads that game
; and saves it as the current continue, then launches
; WIZARDS.EXE (which always loads that save on startup)
*/
/*

Menu_Screen_Control()
    fid0_Continue:                            ; case 0x0
        Stop_Music__STUB()
        j_GAME_WizardsLaunch__WIP(e_SAVE9GAM)

*/
void GAME_WizardsLaunch__WIP(int16_t save_gam_idx)
{
    Fade_Out();
    // NEWCODE  Stop_Music__STUB();
    // NEWCODE  Audio_Uninit__STUB();
    if(save_gam_idx != 8)
    {
        Load_SAVE_GAM(save_gam_idx);
        Save_SAVE_GAM(8);
    }
    // TODO  GAME_EXE_Swap(cnst_EXESwap_File, cnst_EXESwap_Arg, cnst_EXESwap_Arg, empty_string__ovr050);
}


// MGC  o50p03
// Load_Screen()

// MGC  o50p04
// Load_Screen_Draw()

// MGC  o50p05
/**
 * @brief Runs the New Game options controller screen (screen 0).
 *
 * @details
 * Initializes the New Game options UI, loads/persists `magic_set`, and handles
 * user interaction for difficulty, opponent count, land size, and magic power
 * before advancing or cancelling.
 *
 * Main flow:
 * 1) Prepares display pages/palette and loads required LBX assets (background,
 *    frame, action buttons, and option-row button images).
 * 2) Loads `MAGIC.SET` if present/non-empty; otherwise applies default game
 *    settings.
 * 3) Clamps loaded `magic_set` fields to valid ranges.
 * 4) Creates input fields for OK/Cancel and four option rows, then installs
 *    draw callback (`Newgame_Screen_0_Draw`) and help list.
 * 5) Runs input loop:
 *    - Cancel returns `ST_UNDEFINED`.
 *    - OK writes `magic_set` to disk, deactivates handlers, randomizes book
 *      height tables, copies selected options into runtime globals, and
 *      returns `1`.
 *    - Option-row clicks cycle each corresponding setting with wrap-around.
 *    - While active, performs per-frame draw/page/timing updates.
 *
 * @param void This function accepts no parameters.
 *
 * @return int16_t
 * @retval ST_UNDEFINED User cancelled via quit button.
 * @retval 1            User confirmed options via OK button.
 * @retval 0            Fallback path (defensive HACK return).
 *
 * @note Mutates global game settings/UI state including `magic_set`,
 *       `_landsize`, `_magic`, `_num_players`, `_difficulty`, and input field
 *       globals for this screen.
 *
 * @see Newgame_Screen_0_Draw
 * @see Randomize_Book_Heights
 */
int16_t Newgame_Screen_0(void)
{
    char file_found[LEN_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t leave_screen = 0;
    int16_t First_Draw_Done = 0;
    int16_t input_field_idx = 0;  // _SI_
    FILE * file_pointer = 0;  // _DI_
    struct s_MAGIC_SET magic_set_snapshot;
    
    screen_changed = ST_TRUE;
    
    // KNOWNBUG  conflicting condition - will never jump
    if(screen_changed != ST_FALSE)
    {
        Fade_Out();
    }

    Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, ST_TRANSPARENT);

    Set_Page_On();

    Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, ST_TRANSPARENT);

    Set_Page_Off();

    Load_Palette(0, -1, ST_NULL);

    /*
    NEWGAME.LBX, 000  BACKGRND    Main screen back
    NEWGAME.LBX, 001  NEWGAMP2    New game border
    NEWGAME.LBX, 002  NEWGMBT2    OK button
    NEWGAME.LBX, 003  NEWGMBT2    Cancel button
    NEWGAME.LBX, 004  NEWGMBT3    Difficulty button
    NEWGAME.LBX, 005  NEWGMBT3    Opponents button
    NEWGAME.LBX, 006  NEWGMBT3    Land Size button
    NEWGAME.LBX, 007  NEWGMBT3    Magic buton
    */
    newgame_background_seg = LBX_Reload(newgame_lbx_file__ovr050, 0, _screen_seg);
    IMG_NewG_RgtOverlay = LBX_Reload_Next(newgame_lbx_file__ovr050, 1, _screen_seg);
    newgame_ok_button_seg = LBX_Reload_Next(newgame_lbx_file__ovr050, 2, _screen_seg);
    _quit_active_seg = LBX_Reload_Next(newgame_lbx_file__ovr050, 3, _screen_seg);
    m_shared_pict_segs[0] = LBX_Reload_Next(newgame_lbx_file__ovr050, 4, _screen_seg);
    m_shared_pict_segs[1] = LBX_Reload_Next(newgame_lbx_file__ovr050, 5, _screen_seg);
    m_shared_pict_segs[2] = LBX_Reload_Next(newgame_lbx_file__ovr050, 6, _screen_seg);
    m_shared_pict_segs[3] = LBX_Reload_Next(newgame_lbx_file__ovr050, 7, _screen_seg);

    if(
        (DIR(str_MAGIC_SET__ovr050, file_found) == 0)
        ||
        (LOF(str_MAGIC_SET__ovr050) == 0)
    )
    {

        Set_Default_Game_Settings();

    }
    else
    {

        file_pointer = fopen(str_MAGIC_SET__ovr050, str_rb__ovr050);

        fread(&magic_set, sizeof(struct s_MAGIC_SET), 1, file_pointer);

        fclose(file_pointer);
        
    }

    if(
        (magic_set.Difficulty < 0)
        ||
        (magic_set.Difficulty > 4)
    )
    {
        magic_set.Difficulty = 0;
    }

    if(
        (magic_set.Opponents < 0)
        ||
        (magic_set.Opponents > 4)
    )
    {
        magic_set.Opponents = 1;
    }

    if(
        (magic_set.LandSize < 0)
        ||
        (magic_set.LandSize > 2)
    )
    {
        magic_set.LandSize = 0;
    }

    if(
        (magic_set.MagicPower < 0)
        ||
        (magic_set.MagicPower > 2)
    )
    {
        magic_set.MagicPower = 0;
    }

#ifdef STU_DEBUG
    DBG_Print_MAGIC_SET("Newgame_Screen_0 after load");
    memcpy(&magic_set_snapshot, &magic_set, sizeof(struct s_MAGIC_SET));
#endif

    Set_Mouse_List(1, mouse_list_newgame);

    Clear_Fields();

    Set_Page_On();

    Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, ST_TRANSPARENT);

    Set_Page_Off();

    Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, ST_TRANSPARENT);

    _ok_button = Add_Button_Field(252, 179, &empty_string__ovr050[0], newgame_ok_button_seg, empty_string__ovr050[0], ST_UNDEFINED);

    _quit_button = Add_Button_Field(171, 179, &empty_string__ovr050[0], _quit_active_seg, str_ESC__ovr050[0], ST_UNDEFINED);

    newgame_difficulty_button_field = Add_Hidden_Field(250, 38, 315, 54, empty_string__ovr050[0], ST_UNDEFINED);

    newgame_opponents_button_field = Add_Hidden_Field(250, 65, 315, 81, empty_string__ovr050[0], ST_UNDEFINED);

    newgame_landsize_button_field = Add_Hidden_Field(250, 92, 315, 108, empty_string__ovr050[0], ST_UNDEFINED);

    newgame_magic_button_field = Add_Hidden_Field(250, 119, 315, 135, empty_string__ovr050[0], ST_UNDEFINED);

    leave_screen = ST_FALSE;

    First_Draw_Done = ST_FALSE;

    Assign_Auto_Function(Newgame_Screen_0_Draw, 1);

    Set_Newgame_Screen_0_Help_List();

    while(leave_screen == ST_FALSE)
    {

        input_field_idx = Get_Input();

        Mark_Time();

        if(input_field_idx == _quit_button)
        {
            Deactivate_Auto_Function();
            Deactivate_Help_List();
            return ST_UNDEFINED;
        }

        if(input_field_idx == _ok_button)
        {

#ifdef STU_DEBUG
            DBG_Compare_MAGIC_SET(&magic_set_snapshot, &magic_set, "Newgame_Screen_0 before save");
#endif

            file_pointer = fopen(str_MAGIC_SET__ovr050, str_wb__ovr050);

            fwrite(&magic_set, sizeof(struct s_MAGIC_SET), 1, file_pointer);

            fclose(file_pointer);

            Deactivate_Auto_Function();

            Deactivate_Help_List();

            Randomize_Book_Heights();

            _landsize = magic_set.LandSize;
            
            _magic = magic_set.MagicPower;
            
            _num_players = (magic_set.Opponents + 1);
            
            _difficulty = magic_set.Difficulty;
            
            return 1;          }
        
        if(input_field_idx == newgame_difficulty_button_field)
        {

            magic_set.Difficulty++;

            if(magic_set.Difficulty > god_Impossible)
            {
                magic_set.Difficulty = god_Intro;
            }

        }

        if(input_field_idx == newgame_opponents_button_field)
        {

            magic_set.Opponents++;

            if(magic_set.Opponents > goo_Four)
            {
                magic_set.Opponents = goo_One;
            }

        }

        if(input_field_idx == newgame_landsize_button_field)
        {

            magic_set.LandSize++;

            if(magic_set.LandSize > gol_Large)
            {
                magic_set.LandSize = gol_Small;
            }

        }

        if(input_field_idx == newgame_magic_button_field)
        {

            magic_set.MagicPower++;

            if(magic_set.MagicPower > gom_Powerful)
            {
                magic_set.MagicPower = gom_Weak;
            }

        }

        if(leave_screen == ST_FALSE)
        {

            Newgame_Screen_0_Draw();

            Toggle_Pages();

            // ; conflicting condition - will never jump
            if(screen_changed != ST_FALSE)
            {
                
                if(First_Draw_Done == ST_FALSE)
                {

                    Copy_On_To_Off_Page();

                    Fade_In();
                    
                    First_Draw_Done = ST_TRUE;
                    
                }
                
            }

            Release_Time(2);

        }

    }

// Non-void function does not return a value in all control paths
/* HACK */  return 0;

}

// MGC  o50p06
/**
 * @brief Draws the New Game options screen frame (screen 0).
 *
 * @details
 * Performs one render pass for the initial game-options UI by drawing the
 * options panel, clearing dynamic text/button regions, and rendering current
 * selections for difficulty, opponents, land size, and magic strength with
 * hover-aware button offsets.
 *
 * Current behavior:
 * 1) Copies global option-name tables to local arrays used for text rendering.
 * 2) Selects shadow/font color ramps (`RP_COL_NEWG_4Shadow`,
 *    `RP_COL_NEWG_4Font`).
 * 3) Draws background and right-side overlay.
 * 4) Clears transparent rectangles for each interactive option row and the
 *    lower action-button regions.
 * 5) Reads focused control via `Auto_Input()` into `auto_input_field_idx`.
 * 6) Draws each option button in normal or highlighted offset position,
 *    printing the corresponding current option label:
 *    - `magic_set.Difficulty`
 *    - `magic_set.Opponents`
 *    - `magic_set.LandSize`
 *    - `magic_set.MagicPower`
 *
 * @param void This function accepts no parameters.
 *
 * @return void
 *
 * @note Draw-only routine; option value mutation and navigation are handled by
 *       the enclosing controller loop (`Newgame_Screen_0__WIP`).
 * @note Writes to global `auto_input_field_idx` as part of input focus state.
 *
 * @see Newgame_Screen_0__WIP
 */
void Newgame_Screen_0_Draw(void)
{
    struct s_OPPONENT_COUNT_NAMES l_opponent_count_names[4] = { 0, 0, 0, 0 };
    struct s_MAGIC_STRENGTH_NAMES l_magic_strength_names[3] = { 0, 0, 0 };
    struct s_LAND_SIZE_NAMES l_land_size_names[3] = { 0, 0, 0 };
    struct s_DIFFICULTY_NAMES l_difficulty_names[5] = { 0, 0, 0, 0, 0 };
    uint8_t * colors2 = NULL;
    uint8_t * colors1 = NULL;

    colors1 = RP_COL_NEWG_4Shadow;
    
    colors2 = RP_COL_NEWG_4Font;
    
    memcpy(l_difficulty_names, _difficulty_names, sizeof(_difficulty_names));
    
    memcpy(l_land_size_names, _land_size_names, sizeof(_land_size_names));
    
    memcpy(l_magic_strength_names, _magic_strength_names, sizeof(_magic_strength_names));

    memcpy(l_opponent_count_names, _opponent_count_names, sizeof(_opponent_count_names));

    // FLIC_Draw(0, 0, _background_seg);
    FLIC_Draw(0, 0, newgame_background_seg);

    Fill(250, 38, 315, 54, ST_TRANSPARENT);
    
    Fill(250, 65, 315, 82, ST_TRANSPARENT);

    Fill(250, 92, 315, 107, ST_TRANSPARENT);
    
    Fill(250, 119, 315, 134, ST_TRANSPARENT);

    Fill(251, 178, 315, 193, ST_TRANSPARENT);

    Fill(170, 178, 234, 193, ST_TRANSPARENT);

    FLIC_Draw(165, 0, IMG_NewG_RgtOverlay);

    auto_input_field_idx = Auto_Input();

    Set_Font_Style(3, 15, ST_NULL, ST_NULL);

    if(auto_input_field_idx != newgame_difficulty_button_field)
    {
        FLIC_Draw(251, 39, m_shared_pict_segs[0]);
        Set_Font_Colors_15(3, &colors1[0]);
        Print_Centered(283, 43, l_difficulty_names[magic_set.Difficulty].name);
        Set_Font_Colors_15(3, &colors2[0]);
        Print_Centered(282, 42, l_difficulty_names[magic_set.Difficulty].name);
    }
    else
    {
        FLIC_Draw(252, 40, m_shared_pict_segs[0]);
        Set_Font_Colors_15(3, &colors1[0]);
        Print_Centered(284, 44, l_difficulty_names[magic_set.Difficulty].name);
        Set_Font_Colors_15(3, &colors2[0]);
        Print_Centered(283, 43, l_difficulty_names[magic_set.Difficulty].name);
    }

    if(auto_input_field_idx != newgame_opponents_button_field)
    {
        FLIC_Draw(251, 66, m_shared_pict_segs[1]);
        Set_Font_Colors_15(3, &colors1[0]);
        Print_Centered(283, 71, l_opponent_count_names[(magic_set.Opponents - 1)].name);
        Set_Font_Colors_15(3, &colors2[0]);
        Print_Centered(282, 70, l_opponent_count_names[(magic_set.Opponents - 1)].name);
    }
    else
    {
        FLIC_Draw(252, 67, m_shared_pict_segs[1]);
        Set_Font_Colors_15(3, &colors1[0]);
        Print_Centered(284, 72, l_opponent_count_names[(magic_set.Opponents - 1)].name);
        Set_Font_Colors_15(3, &colors2[0]);
        Print_Centered(283, 71, l_opponent_count_names[(magic_set.Opponents - 1)].name);
    }

    if(auto_input_field_idx != newgame_landsize_button_field)
    {
        FLIC_Draw(251, 93, m_shared_pict_segs[2]);
        Set_Font_Colors_15(3, &colors1[0]);
        Print_Centered(283, 97, l_land_size_names[magic_set.LandSize].name);
        Set_Font_Colors_15(3, &colors2[0]);
        Print_Centered(282, 96, l_land_size_names[magic_set.LandSize].name);
    }
    else
    {
        FLIC_Draw(252, 94, m_shared_pict_segs[2]);
        Set_Font_Colors_15(3, &colors1[0]);
        Print_Centered(284, 98, l_land_size_names[magic_set.LandSize].name);
        Set_Font_Colors_15(3, &colors2[0]);
        Print_Centered(283, 97, l_land_size_names[magic_set.LandSize].name);
    }

    if(auto_input_field_idx != newgame_magic_button_field)
    {
        FLIC_Draw(251, 120, m_shared_pict_segs[3]);
        Set_Font_Colors_15(3, &colors1[0]);
        Print_Centered(283, 124, l_magic_strength_names[magic_set.MagicPower].name);
        Set_Font_Colors_15(3, &colors2[0]);
        Print_Centered(282, 123, l_magic_strength_names[magic_set.MagicPower].name);
    }
    else
    {
        FLIC_Draw(252, 121, m_shared_pict_segs[3]);
        Set_Font_Colors_15(3, &colors1[0]);
        Print_Centered(284, 125, l_magic_strength_names[magic_set.MagicPower].name);
        Set_Font_Colors_15(3, &colors2[0]);
        Print_Centered(283, 124, l_magic_strength_names[magic_set.MagicPower].name);
    }

}

// MGC  o50p07
/**
 * @brief Randomizes spellbook sprite variation offsets for New Game shelves.
 *
 * @details
 * Fills per-book-index variation tables used when drawing realm book rows and
 * bookshelf stacks during wizard selection/creation screens.
 *
 * Current implementation:
 * 1) Computes a settings-derived integer (`niu`) from difficulty, opponents,
 *    land size, and magic power.
 * 2) Iterates indices 0..10 and assigns each table entry to `Random(3) - 1`,
 *    producing a value in the range `[-1, 1]`.
 * 3) Writes randomized offsets to:
 *    - `TBL_Realm4_Books`
 *    - `TBL_Realm3_Books`
 *    - `TBL_Realm2_Books`
 *    - `TBL_Realm0_Books`
 *    - `TBL_Realm1_Books`
 *    - `TBL_Bookshelf_Books`
 *
 * @param void This function accepts no parameters.
 *
 * @return void
 *
 * @note The computed `niu` value is currently not consumed in this function.
 * @note Mutates global table data used by bookshelf/book rendering paths.
 */
void Randomize_Book_Heights(void)
{
    int32_t itr = 0;
    int32_t niu = 0;

    niu = ((((magic_set.MagicPower * magic_set.Opponents) * (magic_set.Difficulty * magic_set.LandSize)) * 123) + 1234);

    for(itr = 0; itr < 11; itr++)
    {
        TBL_Realm4_Books[itr]    = (Random(3) - 1);
        TBL_Realm3_Books[itr]    = (Random(3) - 1);
        TBL_Realm2_Books[itr]    = (Random(3) - 1);
        TBL_Realm0_Books[itr]    = (Random(3) - 1);
        TBL_Realm1_Books[itr]    = (Random(3) - 1);
        TBL_Bookshelf_Books[itr] = (Random(3) - 1);
    }

}

// MGC  o50p08
// drake178: GAME_New_Screen_1()
/*
"Select Wizard"

returns {0,2,3} - {0:cancel,2:custom,3:prefab}
*/
/**
 * @brief Runs custom wizard creation screen 1 (select wizard preset/custom).
 *
 * @details
 * Initializes screen assets and wizard-entry controls for the first custom
 * wizard step, then processes selection input for preset wizard, custom path,
 * or cancel.
 *
 * Main flow:
 * 1) Chooses right-column count/mode by difficulty (`IDK`):
 *    - 7 entries when `magic_set.Difficulty < 1`
 *    - 8 entries otherwise.
 * 2) Loads background, button images, appropriate right overlay, and all
 *    spellbook sprites used by the shared draw routine.
 * 3) Sets mouse/input state, clears fields, initializes display globals
 *    (`m_displayed_wizard`, `NEWG_PortraitSelType`), and creates clickable
 *    list fields for left column + variable right column.
 * 4) Installs ESC hotkey, assigns `Newgame_Screen_1_2_Draw`, configures help
 *    list based on difficulty, and sets input delay.
 * 5) Runs input loop:
 *    - ESC returns `0`.
 *    - Preset click (indices 0..13) copies wizard defaults via
 *      `Human_Player_Wizard_Profile` and returns `3`.
 *    - "Custom" click (`m_newgame_fields[14]` when present) returns `2`.
 *    - Otherwise performs per-frame draw/palette/page/timing updates.
 *
 * @param void This function accepts no parameters.
 *
 * @return int16_t
 * @retval 0 Cancel/back path.
 * @retval 2 Custom wizard path selected.
 * @retval 3 Preset wizard path selected.
 *
 * @note Mutates global new-game UI/profile state and depends on preloaded game
 *       data tables for wizard presets, colors, and string resources.
 *
 * @see Newgame_Screen_1_2_Draw
 * @see Human_Player_Wizard_Profile
 */
int16_t Newgame_Screen_1__WIP(void)
{
    int16_t hotkey_ESC = 0;
    int16_t input_field_idx = 0;
    int16_t leave_screen = 0;
    int16_t First_Draw_Done = 0;
    int16_t IDK = 0;
    int16_t itr = 0;

    IDK = 0;

    if(magic_set.Difficulty < 1)
    {
        IDK = 7;
    }
    else
    {
        IDK = 8;
    }

    newgame_background_seg = LBX_Reload(newgame_lbx_file__ovr050, 0, _screen_seg);

    /*
    NEWGAME.LBX, 009  NEWGMWBT    Wizards button1
    NEWGAME.LBX, 010  NEWGMWBT     Wizards button2
    NEWGAME.LBX, 011  NEWGMWBT     Wizards button3
    NEWGAME.LBX, 012  NEWGMWBT     Wizards button4
    NEWGAME.LBX, 013  NEWGMWBT     Wizards button5
    NEWGAME.LBX, 014  NEWGMWBT     Wizards button6
    NEWGAME.LBX, 015  NEWGMWBT     Wizards button7
    NEWGAME.LBX, 016  NEWGMWBT     Wizards button8
    NEWGAME.LBX, 017  NEWGMWBT     Wizards button9
    NEWGAME.LBX, 018  NEWGMWBT     Wizards button10
    NEWGAME.LBX, 019  NEWGMWBT     Wizards button11
    NEWGAME.LBX, 020  NEWGMWBT     Wizards button12
    NEWGAME.LBX, 021  NEWGMWBT     Wizards button13
    NEWGAME.LBX, 022  NEWGMWBT     Wizards button14
    NEWGAME.LBX, 023  NEWGMWBT     Wizards button15
    */
    for(itr = 0; itr < 15; itr++)
    {
        m_shared_pict_segs[itr] = LBX_Reload_Next(newgame_lbx_file__ovr050, (9 + itr), _screen_seg);
    }

    if(IDK == 0)
    {
        // NEWGAME.LBX, 008  NEWGMWZ2    Wizards border
        IMG_NewG_RgtOverlay = LBX_Reload_Next(newgame_lbx_file__ovr050, 8, _screen_seg);
    }
    else
    {
        // NEWGAME.LBX, 039  NEWPICS     
        IMG_NewG_RgtOverlay = LBX_Reload_Next(newgame_lbx_file__ovr050, 39, _screen_seg);
    }

    /*
    NEWGAME.LBX, 024  BOOKS      White book 1
    NEWGAME.LBX, 025  BOOKS      White book 2
    NEWGAME.LBX, 026  BOOKS      White book 3
    NEWGAME.LBX, 027  BOOKS      Blue book 1
    NEWGAME.LBX, 028  BOOKS      Blue book 2
    NEWGAME.LBX, 029  BOOKS      Blue book 3
    NEWGAME.LBX, 030  BOOKS      Green book 1
    NEWGAME.LBX, 031  BOOKS      Green book 2
    NEWGAME.LBX, 032  BOOKS      Green book 3
    NEWGAME.LBX, 033  BOOKS      Black book 1
    NEWGAME.LBX, 034  BOOKS      Black book 2
    NEWGAME.LBX, 035  BOOKS      Black book 3
    NEWGAME.LBX, 036  BOOKS      Red book 1
    NEWGAME.LBX, 037  BOOKS      Red book 2
    NEWGAME.LBX, 038  BOOKS      Red book 3
    */
    for(itr = 0; itr < 15; itr++)
    {
        m_spellbook_pict_segs[itr] = LBX_Reload_Next(newgame_lbx_file__ovr050, (24 + itr), _screen_seg);
    }

    Set_Mouse_List(1, mouse_list_newgame);

    Clear_Fields();

    leave_screen = ST_FALSE;
    
    First_Draw_Done = ST_FALSE;
    
    m_displayed_wizard = 0;

    NEWG_PortraitSelType = 8;  // Default: "Select Wizard"

    m_newgame_fields[14] = INVALID_FIELD;

    for(itr = 0; itr < 7; itr++)
    {

        m_newgame_fields[itr] = Add_Hidden_Field(168, (26 + (22 * itr)), 237, (42 + (22 * itr)), (int16_t)empty_string__ovr050[0], ST_UNDEFINED);

    }

    for(itr = 0; itr < IDK; itr++)
    {

        m_newgame_fields[(7+itr)] = Add_Hidden_Field(244, (26 + (22 * itr)), 313, (42 + (22 * itr)), (int16_t)empty_string__ovr050[0], ST_UNDEFINED);

    }

    hotkey_ESC = Add_Hot_Key((int16_t)str_ESC__ovr050[0]);

    Assign_Auto_Function(Newgame_Screen_1_2_Draw, 1);

    if(magic_set.Difficulty < god_Easy)
    {
        Set_Newgame_Screen_1_Help_List(ST_FALSE);
    }
    else
    {
        Set_Newgame_Screen_1_Help_List(ST_TRUE);
    }

    Set_Input_Delay(2);

    while(leave_screen == ST_FALSE)
    {

        input_field_idx = Get_Input();

        Mark_Time();

        if(input_field_idx == hotkey_ESC)
        {
            return 0;
        }

        for(itr = 0; itr < 14; itr++)
        {
            if(m_newgame_fields[itr] == input_field_idx)
            {
                Deactivate_Auto_Function();
                Deactivate_Help_List();
                Human_Player_Wizard_Profile(itr);
                return 3;
            }
        }

        if(input_field_idx == m_newgame_fields[14])  // "Custom"
        {
            Deactivate_Auto_Function();
            Deactivate_Help_List();
            return 2;
        }

        if(leave_screen == ST_FALSE)
        {
            Newgame_Screen_1_2_Draw();
            Apply_Palette();
            Toggle_Pages();
            if(First_Draw_Done == ST_FALSE)
            {
                First_Draw_Done = ST_TRUE;
                Copy_On_To_Off_Page();
            }
            Release_Time(2);
        }

    }

    return 0;

}

// MGC  o50p09
/**
 * @brief Draws the shared wizard-selection / portrait-selection screen frame.
 *
 * @details
 * Performs one render/update pass used by both New Game screen 1 (select
 * wizard preset) and screen 2 (select picture/custom path). The routine draws
 * list buttons, title, portrait preview, optional retort text, and shelf books
 * according to current mode and hovered control.
 *
 * Current behavior:
 * 1) Pulls font/shadow color ramps and alias retort text color.
 * 2) Reads hover/focus from `Auto_Input()` and updates `m_displayed_wizard`
 *    from `Scan_Input()` when a control is active.
 * 3) Draws background and right overlay, then title text:
 *    - `cnst_Wiz_Select` when `NEWG_PortraitSelType == 8`
 *    - `cnst_Pic_Select` otherwise.
 * 4) Clears list button regions and draws left/right wizard name columns with
 *    highlighted offsets for the currently hovered field.
 * 5) Computes right-column count from `NEWG_PortraitSelType`, forcing 7 on low
 *    difficulty (`magic_set.Difficulty < god_Easy`).
 * 6) Draws selected portrait preview when `m_displayed_wizard < 14`.
 * 7) In wizard-select mode (`NEWG_PortraitSelType == 8`), optionally prints the
 *    preset special ability line and draws default shelf books.
 * 8) Draws displayed wizard name (non-custom entries).
 *
 * @param void This function accepts no parameters.
 *
 * @return void
 *
 * @note Updates global `m_displayed_wizard` based on current input scan.
 * @note Assumes wizard list fields/assets are already initialized by the caller
 *       and that `m_newgame_fields` indices map to the expected list layout.
 *
 * @see Newgame_Screen_1__WIP
 * @see Newgame_Screen_2__WIP
 * @see NEWG_DrawDefShelf__WIP
 */
void Newgame_Screen_1_2_Draw(void)
{
    char Retort_String[30] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    uint8_t * Retort_Text_Color = 0;
    uint8_t Font_Colors[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    uint8_t Shadow_Colors[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Right_Column_Count = 0;
    int16_t l_auto_input_field_idx = 0;
    int16_t itr = 0;

    memcpy(Shadow_Colors, COL_NEWG_FontShadow, 8);

    memcpy(Font_Colors, COL_NEWG_Font, 8);

    Retort_Text_Color = alias_colors;

    l_auto_input_field_idx = Auto_Input();

    if(Scan_Input() != ST_NULL)
    {
        m_displayed_wizard = (Scan_Input() - 1);
    }

    FLIC_Draw(0, 0, newgame_background_seg);

    FLIC_Draw(165, 17, IMG_NewG_RgtOverlay);

    // FTW  Set_Font_Style1(5, 5);
    Set_Font_Style_Shadow_Down(5, 5, ST_NULL, ST_NULL);

    if(NEWG_PortraitSelType == 8)
    {
        Print_Centered(242, 1, cnst_Wiz_Select);  // "Select Wizard"
    }
    else
    {
        Print_Centered(242, 1, cnst_Pic_Select);  // "Select Picture"
    }

    Fill(168,  27, 237,  42, ST_TRANSPARENT);
    Fill(168,  48, 237,  64, ST_TRANSPARENT);
    Fill(168,  70, 237,  86, ST_TRANSPARENT);
    Fill(168,  92, 237, 108, ST_TRANSPARENT);
    Fill(168, 114, 237, 130, ST_TRANSPARENT);
    Fill(168, 136, 237, 152, ST_TRANSPARENT);
    Fill(168, 158, 237, 174, ST_TRANSPARENT);
    Fill(244,  26, 313,  42, ST_TRANSPARENT);
    Fill(244,  48, 313,  64, ST_TRANSPARENT);
    Fill(244,  70, 313,  86, ST_TRANSPARENT);
    Fill(244,  92, 313, 108, ST_TRANSPARENT);
    Fill(244, 114, 313, 130, ST_TRANSPARENT);
    Fill(244, 136, 313, 152, ST_TRANSPARENT);
    Fill(244, 158, 313, 174, ST_TRANSPARENT);

    if(
        (NEWG_PortraitSelType == 8)
        &&
        (magic_set.Difficulty > god_Intro)
    )
    {
        Fill(244, 180, 313, 196, ST_TRANSPARENT);
    }

    if(m_displayed_wizard < 14)  // "Custom"
    {
        FLIC_Draw(24, 10, wizard_portrait_segs[m_displayed_wizard]);
    }

    Set_Font_Style(3, 15, ST_NULL, ST_NULL);

    for(itr = 0; itr < 7; itr++)
    {

        if(m_newgame_fields[itr] != l_auto_input_field_idx)
        {
            FLIC_Draw(169, (27 + (22 * itr)), m_shared_pict_segs[itr]);
            Set_Font_Colors_15(3, &Shadow_Colors[0]);
            Print_Centered(203, (31 + (22 * itr)), _wizard_presets_table[itr].name);
            Set_Font_Colors_15(3, &Font_Colors[0]);
            Print_Centered(202, (30 + (22 * itr)), _wizard_presets_table[itr].name);
        }
        else
        {
            FLIC_Draw(170, (28 + (22 * itr)), m_shared_pict_segs[itr]);
            Set_Font_Colors_15(3, &Shadow_Colors[0]);
            Print_Centered(204, (32 + (22 * itr)), _wizard_presets_table[itr].name);
            Set_Font_Colors_15(3, &Font_Colors[0]);
            Print_Centered(203, (31 + (22 * itr)), _wizard_presets_table[itr].name);
        }

    }

    Right_Column_Count = NEWG_PortraitSelType;

    if(magic_set.Difficulty < god_Easy)
    {
        Right_Column_Count = 7;
    }

    for(itr = 0; itr < Right_Column_Count; itr++)
    {

        if(m_newgame_fields[(7+itr)] == l_auto_input_field_idx)
        {
            FLIC_Draw(246, (28 + (22 * itr)), m_shared_pict_segs[(7 + itr)]);
            Set_Font_Colors_15(3, &Shadow_Colors[0]);
            Print_Centered(280, (32 + (22 * itr)), _wizard_presets_table[(7 + itr)].name);
            Set_Font_Colors_15(3, &Font_Colors[0]);
            Print_Centered(279, (31 + (22 * itr)), _wizard_presets_table[(7 + itr)].name);
        }
        else
        {
            FLIC_Draw(245, (27 + (22 * itr)), m_shared_pict_segs[(7 + itr)]);
            Set_Font_Colors_15(3, &Shadow_Colors[0]);
            Print_Centered(279, (31 + (22 * itr)), _wizard_presets_table[(7 + itr)].name);
            Set_Font_Colors_15(3, &Font_Colors[0]);
            Print_Centered(278, (30 + (22 * itr)), _wizard_presets_table[(7 + itr)].name);
        }

    }

    /* Print Special ability Name */
    if(
        (_wizard_presets_table[m_displayed_wizard].special != ST_UNDEFINED)
        &&
        (NEWG_PortraitSelType == 8)  // defaulted to 8 and god is not Intro so didn't get reduced to 7;  second has 7 wizards + custom
        &&
        (m_displayed_wizard < 14)  // "Custom" is not the mouse-over
    )
    {
        Set_Font_Style(0, 15, ST_NULL, ST_NULL);
        Set_Font_Colors_15(0, &Font_Colors[0]);
        strcpy(Retort_String, wsa_names[_wizard_presets_table[m_displayed_wizard].special]);  // 1 + {-1,0,1,2,...}
        strcat(Retort_String, cnst_DOT__ovr050);
        Print(13, 181, Retort_String);
        Set_Font_Colors_15(0, &Retort_Text_Color[0]);
        Print(12, 180, Retort_String);
    }

    if(NEWG_PortraitSelType == 8)
    {
        NEWG_DrawDefShelf__WIP(m_displayed_wizard);
    }

    /* Print Wizard Name */
    if(m_displayed_wizard < 14)  // "Custom"
    {
        Set_Font_Style(4, 15, ST_NULL, ST_NULL);
        Set_Font_Colors_15(4, &Shadow_Colors[0]);
        Print_Centered(78, 120, _wizard_presets_table[m_displayed_wizard].name);
        Print_Centered(77, 120, _wizard_presets_table[m_displayed_wizard].name);
        Set_Font_Colors_15(4, &Font_Colors[0]);
        Print_Centered(77, 119, _wizard_presets_table[m_displayed_wizard].name);
    }

}

// MGC  o50p10
/**
 * @brief Runs custom wizard creation screen 2 (wizard portrait selection).
 *
 * @details
 * Initializes portrait-selection assets and controls, then processes input
 * until the user either cancels or chooses one of the preset wizard portraits.
 *
 * Main flow:
 * 1) Loads background, 15 button background images, and portrait-screen
 *    right-side overlay art.
 * 2) Sets mouse list, clears fields, resets local loop state, and initializes
 *    display globals (`m_displayed_wizard`, `NEWG_PortraitSelType`).
 * 3) Creates clickable hidden fields for two columns of wizard entries
 *    (7 + 7 selectable slots in current implementation).
 * 4) Installs ESC hotkey, assigns `Newgame_Screen_1_2_Draw` as auto draw
 *    callback, sets help list, and enables input delay.
 * 5) Runs input loop:
 *    - ESC returns `ngscr_Wizard`.
 *    - Clicking a wizard entry sets `_players[0].wizard_id`, copies default
 *      wizard name into `_players[0].name`, deactivates draw/help handlers,
 *      and exits the loop.
 *    - If still active, performs frame draw/palette/page/timing updates.
 * 6) Returns `ngscr_Name` after a successful portrait selection.
 *
 * @param void This function accepts no parameters.
 *
 * @return int16_t
 * @retval ngscr_Wizard User cancelled with ESC; caller should return to the
 *                      previous wizard setup screen.
 * @retval ngscr_Name   Portrait selected; caller should advance to name entry.
 *
 * @note Mutates global new-game state including selected wizard id, wizard
 *       name, draw/help callbacks, and screen interaction fields.
 *
 * @see Newgame_Screen_1_2_Draw
 * @see Set_Newgame_Screen_2_Help_List
 */
int16_t Newgame_Screen_2__WIP(void)
{
    int16_t hotkey_ESC = 0;
    int16_t itr = 0;
    int16_t input_field_idx = 0;
    int16_t leave_screen = 0;
    int16_t First_Draw_Done = 0;

    newgame_background_seg = LBX_Reload(newgame_lbx_file__ovr050, 0, _screen_seg);

    /*
    NEWGAME.LBX, 009  NEWGMWBT    Wizards button1
    NEWGAME.LBX, 010  NEWGMWBT     Wizards button2
    NEWGAME.LBX, 011  NEWGMWBT     Wizards button3
    NEWGAME.LBX, 012  NEWGMWBT     Wizards button4
    NEWGAME.LBX, 013  NEWGMWBT     Wizards button5
    NEWGAME.LBX, 014  NEWGMWBT     Wizards button6
    NEWGAME.LBX, 015  NEWGMWBT     Wizards button7
    NEWGAME.LBX, 016  NEWGMWBT     Wizards button8
    NEWGAME.LBX, 017  NEWGMWBT     Wizards button9
    NEWGAME.LBX, 018  NEWGMWBT     Wizards button10
    NEWGAME.LBX, 019  NEWGMWBT     Wizards button11
    NEWGAME.LBX, 020  NEWGMWBT     Wizards button12
    NEWGAME.LBX, 021  NEWGMWBT     Wizards button13
    NEWGAME.LBX, 022  NEWGMWBT     Wizards button14
    NEWGAME.LBX, 023  NEWGMWBT     Wizards button15
    */
    for(itr = 0; itr < 15; itr++)
    {
        m_shared_pict_segs[itr] = LBX_Reload_Next(newgame_lbx_file__ovr050, (9 + itr), _screen_seg);
    }

    // NEWGAME.LBX, 039  NEWPICS     
    IMG_NewG_RgtOverlay = LBX_Reload_Next(newgame_lbx_file__ovr050, 39, _screen_seg);

    Set_Mouse_List(1, mouse_list_newgame);

    Clear_Fields();

    leave_screen = ST_FALSE;

    First_Draw_Done = ST_FALSE;

    m_displayed_wizard = 0;

    NEWG_PortraitSelType = 7;

    for(itr = 0; itr < 7; itr++)
    {
        m_newgame_fields[itr] = Add_Hidden_Field(168, (26 + (22 * itr)), 237, (42 + (22 * itr)), (int16_t)empty_string__ovr050[0], ST_UNDEFINED);
    }

    for(itr = 0; itr < 7; itr++)
    {
        m_newgame_fields[(7 + itr)] = Add_Hidden_Field(244, (26 + (22 * itr)), 313, (42 + (22 * itr)), (int16_t)empty_string__ovr050[0], ST_UNDEFINED);
    }

    hotkey_ESC = Add_Hot_Key((int16_t)str_ESC__ovr050[0]);

    Assign_Auto_Function(Newgame_Screen_1_2_Draw, 1);

    Set_Newgame_Screen_2_Help_List();

    Set_Input_Delay(2);

    while(leave_screen == ST_FALSE)
    {

        input_field_idx = Get_Input();

        Mark_Time();

        if(input_field_idx == hotkey_ESC)
        {
            return ngscr_Wizard;
        }

        for(itr = 0; itr < 14; itr++)
        {
            if(m_newgame_fields[itr] == input_field_idx)
            {
                Deactivate_Auto_Function();
                Deactivate_Help_List();
                _players[0].wizard_id = (uint8_t)itr;
                strcpy(_players[0].name, _wizard_presets_table[itr].name);
                leave_screen = ST_TRUE;
            }
        }

        if(leave_screen == ST_FALSE)
        {
            Newgame_Screen_1_2_Draw();
            Apply_Palette();
            Toggle_Pages();
            if(First_Draw_Done == ST_FALSE)
            {
                First_Draw_Done = ST_TRUE;
                Copy_On_To_Off_Page();
            }
            Release_Time(2);
        }

    }

    return ngscr_Name;

}


// MGC  o50p11
/**
 * @brief Runs custom wizard creation screen 3 (wizard name input).
 *
 * @details
 * Prepares the name-entry screen, invokes the text input popup initialized with
 * the current wizard name, then writes the resulting name back to the player
 * profile (falling back to the previous name if the entered string is empty).
 *
 * Processing steps:
 * 1) Loads background and name-box overlay assets.
 * 2) Sets mouse list, clears fields, and registers/draws
 *    `Newgame_Screen_3_Draw__WIP`.
 * 3) Applies palette/pages, configures input font style, and calls
 *    `Setup_Input_Box_Popup(...)` with a 10-character limit.
 * 4) Trims the resulting string and restores prior name when the trimmed input
 *    is empty.
 * 5) Updates `_players[0].name`, deactivates auto draw callback, and returns
 *    status based on popup result.
 *
 * @param void This function accepts no parameters.
 *
 * @return int16_t
 * @retval 0 Name accepted (continue forward).
 * @retval 1 Input cancelled (go back one screen).
 *
 * @note Mutates `_players[0].name` and uses global UI/draw state for the New
 *       Game flow.
 *
 * @see Newgame_Screen_3_Draw__WIP
 */
int16_t Newgame_Screen_3__WIP(void)
{
    char name[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t input_box_return_value = 0;

    // NEWGAME.LBX, 000  BACKGRND    Main screen back
    newgame_background_seg = LBX_Reload(newgame_lbx_file__ovr050, 0, _screen_seg);

    // NEWGAME.LBX, 040  NEWGMNAM   Name box
    IMG_NewG_RgtOverlay = LBX_Reload_Next(newgame_lbx_file__ovr050, 40, _screen_seg);

    Set_Mouse_List(1, mouse_list_newgame);

    Clear_Fields();

    Assign_Auto_Function(Newgame_Screen_3_Draw__WIP, 1);

    Newgame_Screen_3_Draw__WIP();

    Apply_Palette();

    Toggle_Pages();

    Set_Font_Style(3, 8, 8, ST_NULL);

    strcpy(name, _players[0].name);

    input_box_return_value = Setup_Input_Box_Popup(195, 36, 80, name, 10, 0, 0, 1, wziards_name_colors, ST_UNDEFINED);

    Trim(name);

    if(strlen(name) < 1)
    {
        strcpy(name, _players[0].name);
    }

    strcpy(_players[0].name, name);

    Deactivate_Auto_Function();

    if(input_box_return_value == -1)  // cancelled
    {
        return 1;  // cancelled, go back a screen
    }
    else
    {
        return 0;  // ok
    }

}


// MGC  o50p12
/**
 * @brief Draws the New Game wizard-name screen frame (screen 3).
 *
 * @details
 * Renders the name-entry screen background, title area, wizard portrait, and
 * the current wizard name with shadow/highlight layering. When portrait shelf
 * content is pending (`NEWG_PortraitSelType == 7`), it also triggers drawing of
 * the default spellbook shelf and special abilities summary.
 *
 * Render/update flow:
 * 1) Loads local text color and shadow ramps for the name label.
 * 2) Draws background and right-side name box overlay.
 * 3) Draws "Select Name" title and selected wizard portrait.
 * 4) Draws wizard name text with shadow then foreground colors.
 * 5) If `NEWG_PortraitSelType == 7`, clears that flag and draws additional
 *    profile summary elements (`NEWG_DrawDefShelf__WIP`,
 *    `Draw_Special_Abilities_String`).
 *
 * @param void This function accepts no parameters.
 *
 * @return void
 *
 * @note This function is draw-focused and intended to be called from the
 *       enclosing screen logic while name input is active.
 * @note Mutates global `NEWG_PortraitSelType` when the deferred shelf/ability
 *       draw path is executed.
 *
 * @see Newgame_Screen_3__WIP
 * @see NEWG_DrawDefShelf__WIP
 * @see Draw_Special_Abilities_String
 */
void Newgame_Screen_3_Draw__WIP(void)
{
    uint8_t colors1[8] = { 0, 0, 0, 0, 0, 0, 0, 0, };
    uint8_t Shadow_Colors[8] = { 0, 0, 0, 0, 0, 0, 0, 0, };
    memcpy(Shadow_Colors, RP_COL_NEWG_FontShadow2, 8);
    memcpy(colors1, RP_COL_NEWG_Font2, 8);
    FLIC_Draw(0, 0, newgame_background_seg);
    FLIC_Draw(181, 17, IMG_NewG_RgtOverlay);
    Set_Font_Style_Shadow_Down(5, 5, ST_NULL, ST_NULL);
    Print_Centered(242, 1, cnst_Name_Select);
    FLIC_Draw(24, 10, wizard_portrait_segs[_players[0].wizard_id]);
    Set_Font_Style(4, 15, ST_NULL, ST_NULL);
    Set_Font_Colors_15(4, &Shadow_Colors[0]);
    Print_Centered(78, 120, _players[0].name);
    Print_Centered(77, 120, _players[0].name);
    Set_Font_Colors_15(4, &colors1[0]);
    Print_Centered(77, 119, _players[0].name);
    if(NEWG_PortraitSelType == 7)
    {
        NEWG_PortraitSelType = 0;
        NEWG_DrawDefShelf__WIP(_players[0].wizard_id);
        Draw_Special_Abilities_String();
    }
}


// MGC  o50p13
/**
 * @brief Runs custom wizard creation screen 7 (banner selection).
 *
 * @details
 * Initializes banner-selection resources and input fields, then drives the
 * interactive loop where the player chooses one of five banner colors.
 *
 * Main flow:
 * 1) Loads screen assets (background, banner panel overlay, spellbook sprites,
 *    and map-build bar segment).
 * 2) Sets mouse list, clears fields, creates five hidden click fields matching
 *    banner rows, installs ESC hotkey, draw callback, and help list.
 * 3) Runs input loop until exit:
 *    - ESC returns `6` (back to race screen).
 *    - Clicking a banner row maps index 0..4 to `_players[0].banner_id`
 *      (`BNR_Green`, `BNR_Blue`, `BNR_Red`, `BNR_Purple`, `BNR_Yellow`) and
 *      ends the loop.
 *    - While waiting, performs per-frame draw/palette/page/timing updates.
 * 4) Deactivates auto draw and help handlers before returning.
 *
 * @param void This function accepts no parameters.
 *
 * @return int16_t
 * @retval 6   User pressed ESC; caller should navigate back.
 * @retval 99  Banner selected; caller continues new-game flow.
 *
 * @note Mutates global UI state (`m_newgame_fields`, auto/help handlers) and
 *       player profile state (`_players[0].banner_id`).
 * @note Requires LBX resources and shared New Game globals to be initialized.
 *
 * @see Newgame_Screen_7_Draw__WIP
 * @see Set_Newgame_Screen_7_Help_List
 */
int16_t Newgame_Screen_7__WIP(void)
{
    int16_t hotkey_ESC = 0;
    uint8_t Dest_Struct[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t input_field_idx = 0;
    int16_t First_Draw_Done = 0;
    int16_t leave_screen = 0;  // _DI_
    int16_t itr = 0;  // _SI_

    memcpy(Dest_Struct, RP_COL_NEWG_Font3, 8);

    // NEWGAME.LBX, 000  BACKGRND    Main screen back
    newgame_background_seg = LBX_Reload(newgame_lbx_file__ovr050, 0, _screen_seg);

    // NEWGAME.LBX, 046  FLAGSNEW
    IMG_NewG_RgtOverlay = LBX_Reload_Next(newgame_lbx_file__ovr050, 46, _screen_seg);

    for(itr = 0; itr < 15; itr++)
    {

        /*
        NEWGAME.LBX, 024  BOOKS      White book 1
        NEWGAME.LBX, 025  BOOKS      White book 2
        NEWGAME.LBX, 026  BOOKS      White book 3
        NEWGAME.LBX, 027  BOOKS      Blue book 1
        NEWGAME.LBX, 028  BOOKS      Blue book 2
        NEWGAME.LBX, 029  BOOKS      Blue book 3
        NEWGAME.LBX, 030  BOOKS      Green book 1
        NEWGAME.LBX, 031  BOOKS      Green book 2
        NEWGAME.LBX, 032  BOOKS      Green book 3
        NEWGAME.LBX, 033  BOOKS      Black book 1
        NEWGAME.LBX, 034  BOOKS      Black book 2
        NEWGAME.LBX, 035  BOOKS      Black book 3
        NEWGAME.LBX, 036  BOOKS      Red book 1
        NEWGAME.LBX, 037  BOOKS      Red book 2
        NEWGAME.LBX, 038  BOOKS      Red book 3
        */
        // ; 5 groups of 3 images each (L - S - N - D - C)
        m_spellbook_pict_segs[itr] = LBX_Reload_Next(newgame_lbx_file__ovr050, (24 + itr), _screen_seg);

    }

    // odd? this gets drawn in Init_New_Game()
    // NEWGAME.LBX, 053  BUILDWOR   map build bar
    newgame_BUILDWOR_map_build_bar_seg = LBX_Reload_Next(newgame_lbx_file__ovr050, 53, _screen_seg);

    Set_Mouse_List(1, mouse_list_newgame);

    Clear_Fields();

    leave_screen = ST_FALSE;

    First_Draw_Done = ST_FALSE;

    m_newgame_fields[0] = Add_Hidden_Field(175, 21, 313, 55, (int16_t)empty_string__ovr050[0], ST_UNDEFINED);

    m_newgame_fields[1] = Add_Hidden_Field(175, 56, 313, 93, (int16_t)empty_string__ovr050[0], ST_UNDEFINED);

    m_newgame_fields[2] = Add_Hidden_Field(175, 94, 313, 128, (int16_t)empty_string__ovr050[0], ST_UNDEFINED);

    m_newgame_fields[3] = Add_Hidden_Field(175, 129, 313, 162, (int16_t)empty_string__ovr050[0], ST_UNDEFINED);

    m_newgame_fields[4] = Add_Hidden_Field(175, 163, 313, 199, (int16_t)empty_string__ovr050[0], ST_UNDEFINED);

    hotkey_ESC = Add_Hot_Key((int16_t)str_ESC__ovr050[0]);

    Assign_Auto_Function(Newgame_Screen_7_Draw__WIP, 1);

    Set_Newgame_Screen_7_Help_List();

    while(leave_screen == ST_FALSE)
    {

        Mark_Time();

        input_field_idx = Get_Input();

        if(input_field_idx == hotkey_ESC)
        {

            return ngscr_Race;

        }

        for(itr = 0; itr < NUM_BANNER_SELECTIONS; itr++)
        {

            if(m_newgame_fields[itr] == input_field_idx)
            {

                switch(itr)
                {
                    case 0:
                    {
                        _players[0].banner_id = BNR_Green;
                    } break;
                    case 1:
                    {
                        _players[0].banner_id = BNR_Blue;

                    } break;
                    case 2:
                    {
                        _players[0].banner_id = BNR_Red;

                    } break;
                    case 3:
                    {
                        _players[0].banner_id = BNR_Purple;

                    } break;
                    case 4:
                    {
                        _players[0].banner_id = BNR_Yellow;

                    } break;

                }
                    
                leave_screen = ST_TRUE;

            }

        }


        if(leave_screen == ST_FALSE)
        {

            Newgame_Screen_7_Draw__WIP();

            Apply_Palette();

            Toggle_Pages();

            if(First_Draw_Done == ST_FALSE)
            {
                
                First_Draw_Done = ST_TRUE;

                Copy_On_To_Off_Page();

            }

            Release_Time(1);

        }

    }

    Deactivate_Auto_Function();

    Deactivate_Help_List();

    return 99;

}


// MGC  o50p14
/**
 * @brief Draws the New Game banner-selection screen (screen 7) frame.
 *
 * @details
 * Performs one draw pass for the banner selection UI, including the standard
 * wizard identity panel and the shared books/abilities summary used on screens
 * 6 and 7.
 *
 * Render sequence:
 * 1) Initializes local text/shadow color ramps.
 * 2) Draws background and right-side overlay panel.
 * 3) Draws title text ("Select Banner"), wizard portrait, and wizard name with
 *    layered shadow/highlight text.
 * 4) Draws shelf spellbook icons from `_players[0].spellranks[]` in realm order
 *    using `m_spellbook_pict_segs` and `TBL_Bookshelf_Books` offsets.
 * 5) Draws selected special abilities summary text via
 *    `Draw_Special_Abilities_String()`.
 *
 * @param void This function accepts no parameters.
 *
 * @return void
 *
 * @note This routine is draw-only; input handling and state transitions are
 *       managed by the enclosing screen loop.
 * @note Assumes required LBX image segments and wizard profile globals are
 *       already initialized by the caller.
 *
 * @see Draw_Special_Abilities_String
 */
void Newgame_Screen_7_Draw__WIP(void)
{
    uint8_t text_colors[5] = { 0, 0, 0, 0, 0 };
    uint8_t shadow_colors[5] = { 0, 0, 0, 0, 0 };
    int16_t start_X = 0;
    int16_t itr = 0;

    memcpy(shadow_colors, RP_COL_NEWG_5Shadow, 5);

    memcpy(text_colors, RP_COL_NEWG_5Font, 5);

    FLIC_Draw(0, 0, newgame_background_seg);

    FLIC_Draw(158, 0, IMG_NewG_RgtOverlay);

    Set_Font_Style_Shadow_Down(5, 5, ST_NULL, ST_NULL);

    Print_Centered(242, 1, cnst_Banner_Select);  // "Select Banner"

    FLIC_Draw(24, 10, wizard_portrait_segs[_players[0].wizard_id]);

    Set_Font_Style(4, 15, ST_NULL, ST_NULL);

    Set_Font_Colors_15(4, &shadow_colors[0]);
    Print_Centered(78, 120, _players[0].name);
    Print_Centered(77, 120, _players[0].name);

    Set_Font_Colors_15(4, &text_colors[0]);
    Print_Centered(77, 119, _players[0].name);
    Print_Centered(77, 120, _players[0].name);

    /*
        BEGIN: same on Screen 6 & 7
    */

    start_X = 36;

    for(itr = 0; itr < _players[0].spellranks[sbr_Life]; itr++)
    {
        FLIC_Draw(start_X, 135, m_spellbook_pict_segs[(0 + TBL_Bookshelf_Books[itr])]);
        start_X += 8;
    }

    for(itr = 0; itr < _players[0].spellranks[sbr_Sorcery]; itr++)
    {
        FLIC_Draw(start_X, 135, m_spellbook_pict_segs[(3 + TBL_Bookshelf_Books[itr])]);
        start_X += 8;
    }

    for(itr = 0; itr < _players[0].spellranks[sbr_Nature]; itr++)
    {
        FLIC_Draw(start_X, 135, m_spellbook_pict_segs[(6 + TBL_Bookshelf_Books[itr])]);
        start_X += 8;
    }

    for(itr = 0; itr < _players[0].spellranks[sbr_Death]; itr++)
    {
        FLIC_Draw(start_X, 135, m_spellbook_pict_segs[(9 + TBL_Bookshelf_Books[itr])]);
        start_X += 8;
    }

    for(itr = 0; itr < _players[0].spellranks[sbr_Chaos]; itr++)
    {
        FLIC_Draw(start_X, 135, m_spellbook_pict_segs[(12 + TBL_Bookshelf_Books[itr])]);
        start_X += 8;
    }

    Draw_Special_Abilities_String();

    /*
        END: same on Screen 6 & 7
    */

}


// MGC  o50p15
/*

Module: MOX
    data (0 bytes) _race_names
    Address: 02:0018A630
Module: RACEOPT
    data (0 bytes) _cur_race_name
    Address: 02:00193772
Module: RACESEL
    data (0 bytes) _race_name_list
    Address: 02:0019383C
    data (0 bytes) _last_race_seg
    Address: 02:00193840
    data (0 bytes) _race_flag
    Address: 02:00193848
    data (0 bytes) _old_race
    Address: 02:00193858
    data (0 bytes) _displayed_race
    Address: 02:0019385A
    data (0 bytes) _last_race_name
    Address: 02:0019385C

*/
/**
 * @brief Runs custom wizard creation screen 6 (race selection).
 *
 * @details
 * Initializes race-selection assets and controls, then drives the interactive
 * race selection loop for Arcanus/Myrran race labels.
 *
 * Main flow:
 * 1) Loads screen art/resources (background, right overlay, race panel, warning
 *    box, and spellbook sprites).
 * 2) Creates 14 hidden input fields mapped to race labels (9 Arcanus + 5
 *    Myrran), installs ESC hotkey, draw callback, and help list.
 * 3) Processes input until a valid race is selected or ESC is pressed:
 *    - ESC returns `ST_UNDEFINED`.
 *    - Arcanus label click selects race immediately.
 *    - Myrran label click selects race only when Myrran retort is active
 *      (`_players[0].alchemy[rtt_Myrran] == 1`); otherwise shows warning
 *      dialog (`Warn0(cnst_Race_Error)`) and resumes draw/help handlers.
 * 4) While waiting for selection, performs manual frame updates
 *    (`Newgame_Screen_6_Draw__WIP`, palette apply, page toggle, timing).
 * 5) After selection, redraws selected race label in a short 4-step blink
 *    sequence, then deactivates auto/help handlers.
 *
 * @param void This function accepts no parameters.
 *
 * @return int16_t
 * @retval ST_UNDEFINED User pressed ESC (cancel/back path).
 * @retval ngscr_Banner Race selected; proceed to banner selection screen.
 *
 * @note Mutates global state including `NEWG_Clicked_Race`, `m_newgame_fields`,
 *       and active draw/help callbacks.
 * @note Requires initialized wizard/profile globals and valid LBX resources.
 *
 * @see Newgame_Screen_6_Draw__WIP
 * @see Set_Newgame_Screen_6_Help_List
 */
int16_t Newgame_Screen_6__WIP(void)
{
    int8_t Arcanus_Races[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0 };  // TODO  DEDU  check Dasm for 1-byte, signed
    int16_t hotkey_ESC = 0;
    int8_t * wsa_ptr = 0;
    uint8_t * Shadow_Color = 0;
    uint8_t * Available_Color = 0;
    uint8_t * Not_Available_Color = 0;
    uint8_t Font_Colors[8] = {0, 0, 0, 0, 0, 0, 0, 0 };
    int8_t Myrran_Races[5] = { 0, 0, 0, 0, 0 };
    int16_t input_field_idx = 0;
    int16_t leave_screen = 0;
    int16_t First_Draw_Done = 0;
    int16_t itr = 0;
    int16_t itr2 = 0;

    memcpy(Arcanus_Races, TBL_Arcanian_Races, 9);

    memcpy(Myrran_Races, TBL_Myrran_Races, 5);

    memcpy(Font_Colors, RP_COL_NEWG_Font4, 8);

    Not_Available_Color = COL_NEWG_Highlight;

    Available_Color = COL_Available;

    Shadow_Color = COL_NEWG_RacesShadow;

    // NEWGAME.LBX, 000  BACKGRND    Main screen back
    newgame_background_seg = LBX_Reload(newgame_lbx_file__ovr050, 0, _screen_seg);
    // NEWGAME.LBX, 054  NEWGMWZ2   Wizards border
    IMG_NewG_RgtOverlay = LBX_Reload_Next(newgame_lbx_file__ovr050, 54, _screen_seg);
    // NEWGAME.LBX, 055  RACES      Races dark region
    m_shared_pict_segs[0] = LBX_Reload_Next(newgame_lbx_file__ovr050, 55, _screen_seg);
    // NEWGAME.LBX, 044  WARNBACK   Warning box top
    m_warning_box_top_seg = LBX_Reload_Next(newgame_lbx_file__ovr050, 44, _screen_seg);
    // NEWGAME.LBX, 045  WARNBACK   Warning box buttom
    m_warning_box_bottom_seg = LBX_Reload_Next(newgame_lbx_file__ovr050, 45, _screen_seg);

    for(itr = 0; itr < 15; itr++)
    {
        m_spellbook_pict_segs[itr] = LBX_Reload_Next(newgame_lbx_file__ovr050, (24 + itr), _screen_seg);
    }

    Set_Mouse_List(1, mouse_list_newgame);

    Clear_Fields();

    leave_screen = ST_FALSE;

    First_Draw_Done = ST_NULL;

    wsa_ptr = &_players[0].alchemy;

    for(itr = 0; itr < 9; itr++)
    {
        m_newgame_fields[itr] = Add_Hidden_Field(211, (38 + (itr * 10)), 270, (45 + (itr * 10)), (int16_t)empty_string__ovr050[0], ST_UNDEFINED);
    }

    for(itr = 0; itr < 5; itr++)
    {
        m_newgame_fields[(9 + itr)] = Add_Hidden_Field(211, (147 + (itr * 10)), 270, (154 + (itr * 10)), (int16_t)empty_string__ovr050[0], ST_UNDEFINED);
    }

    hotkey_ESC = Add_Hot_Key((int16_t)str_ESC__ovr050[0]);

    Assign_Auto_Function(Newgame_Screen_6_Draw__WIP, 1);

    Set_Newgame_Screen_6_Help_List();

    while(leave_screen == ST_FALSE)
    {

        Mark_Time();

        input_field_idx = Get_Input();

        if(input_field_idx == hotkey_ESC)
        {

            return ST_UNDEFINED;

        }

        for(itr = 0; itr < 9; itr++)
        {
            if(m_newgame_fields[itr] == input_field_idx)
            {
                NEWG_Clicked_Race = Arcanus_Races[itr];
                leave_screen = ST_TRUE;
            }
        }

        for(itr = 9; itr < 14; itr++)
        {

            if(wsa_ptr[rtt_Myrran] == 1)
            {

                if(m_newgame_fields[itr] == input_field_idx)
                {
                    NEWG_Clicked_Race = Myrran_Races[(itr - 9)];
                    leave_screen = ST_TRUE;
                }

            }
            else
            {

                if(m_newgame_fields[itr] == input_field_idx)
                {
                    
                    Deactivate_Auto_Function();
                    
                    Deactivate_Help_List();

                    Warn0(cnst_Race_Error);  // "You can not select a Myrran race unless you have the Myrran special."

                    Assign_Auto_Function(Newgame_Screen_6_Draw__WIP, 1);

                    Set_Help_List(_help_entries, 14);

                }

            }

        }

        if(leave_screen == ST_FALSE)
        {

            Newgame_Screen_6_Draw__WIP();

            Apply_Palette();

            Toggle_Pages();

            if(First_Draw_Done == ST_FALSE)
            {
                
                First_Draw_Done = ST_TRUE;

                Copy_On_To_Off_Page();

            }
 
            Release_Time(2);

        }

    }

    for(itr = 0; itr < 4; itr++)
    {

        for(itr2 = 0; itr2 < 9; itr2++)
        {

            if(Arcanus_Races[itr2] == NEWG_Clicked_Race)
            {

                Set_Font_Colors_15(2, &Shadow_Color[0]);

                Print(221, (39 + (itr2 * 10)), *_race_type_table[Arcanus_Races[itr2]].name);

                if(
                    (itr == 0)
                    ||
                    (itr == 2)
                )
                {
                    Set_Font_Colors_15(2, &Available_Color[0]);

                }
                else
                {
                    
                    Set_Font_Colors_15(2, &Not_Available_Color[0]);

                }

                Print(220, (38 + (itr2 * 10)), *_race_type_table[Arcanus_Races[itr2]].name);

                itr2 = 9;

            }

        }

        for(itr2 = 9; itr2 < 14; itr2++)
        {

            if(Arcanus_Races[itr2] == NEWG_Clicked_Race)
            {

                Set_Font_Colors_15(2, &Shadow_Color[0]);

                Print(221, (148 + ((itr2 - 9) * 10)), *_race_type_table[Myrran_Races[(itr2 - 9)]].name);

                if(
                    (itr == 0)
                    ||
                    (itr == 2)
                )
                {
                    Set_Font_Colors_15(2, &Available_Color[0]);

                }
                else
                {
                    
                    Set_Font_Colors_15(2, &Not_Available_Color[0]);

                }

                Print(220, (147 + ((itr2 - 9) * 10)), *_race_type_table[Myrran_Races[(itr2 - 9)]].name);

                itr2 = 14;

            }

        }


    }

    Deactivate_Auto_Function();

    Deactivate_Help_List();

    return ngscr_Banner;

}


// MGC  o50p16
/**
 * @brief Draws and updates the New Game race-selection screen (screen 6).
 *
 * @details
 * Performs one full draw/update pass for the race selection UI used during
 * custom wizard creation. The routine renders static art and wizard identity,
 * draws current books and selected abilities summary, scans hovered control,
 * and colors race labels according to availability/highlight state.
 *
 * Current behavior:
 * 1) Loads local race index tables (Arcanus/Myrran) and color ramps.
 * 2) Draws background, overlays, title, wizard portrait, and wizard name.
 * 3) Polls current hovered/active control via `Scan_Input()`.
 * 4) Draws bookshelf books from `_players[0].spellranks[]` and the composed
 *    abilities paragraph (`Draw_Special_Abilities_String()`).
 * 5) Draws Arcanus race labels as always available, with hover highlight.
 * 6) Draws Myrran race labels as available only when Myrran retort is selected
 *    (`_players[0].alchemy[rtt_Myrran]`), otherwise uses unavailable colors.
 *
 * @param void This function accepts no parameters.
 *
 * @return void
 *
 * @note Uses and updates global screen state only via rendering/input side
 *       effects; it does not perform screen-transition decisions.
 * @note Assumes required image segments, race tables, palette colors, and
 *       wizard profile globals are already initialized by the caller.
 *
 * @see Draw_Special_Abilities_String
 */
void Newgame_Screen_6_Draw__WIP(void)
{
    int16_t Arcanus_Races[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int8_t * wsa_ptr = 0;
    uint8_t * Shadow_Color = 0;
    uint8_t * Not_Available_Color = 0;
    uint8_t * Available_Color = 0;
    uint8_t * Race_Highlight_Color = 0;
    uint8_t Race_Title_Colors[5] = { 0, 0, 0, 0, 0 };
    uint8_t Font_Colors[5] = { 0, 0, 0, 0, 0 };
    uint8_t FontShadow_Colors[5] = { 0, 0, 0, 0, 0 };
    int16_t Myrran_Races[5] = { 0, 0, 0, 0, 0 };
    int16_t Moused_Control = 0;
    int16_t start_X = 0;
    int16_t itr = 0;
    int16_t itr2 = 0;
    int16_t DBG_race_type = 0;
    char * DBG_race_name_string = 0;

    memcpy(Arcanus_Races, RP_TBL_Arcanian_Races_2, 18);

    memcpy(Myrran_Races, RP_TBL_Myrran_Races_2, 10);

    memcpy(FontShadow_Colors, RP_COL_NEWG_5Shadow2, 5);

    memcpy(Font_Colors, RP_COL_NEWG_5Font2, 5);

    memcpy(Race_Title_Colors, COL_NEWG_RaceTitles, 5);

    Race_Highlight_Color = COL_NEWG_Highlight2;

    Available_Color = COL_Available_2;

    Not_Available_Color = COL_NEWG_NA;

    Shadow_Color = COL_NEWG_RacesShdw2;

    FLIC_Draw(0, 0, newgame_background_seg);

    FLIC_Draw(164, 18, IMG_NewG_RgtOverlay);

    FLIC_Draw(208, 34, m_shared_pict_segs[0]);  // IDGI

    Set_Font_Style_Shadow_Down(5, 5, ST_NULL, ST_NULL);

    Print_Centered(242, 1, cnst_Race_Select);  // "Select Race"

    FLIC_Draw(24, 10, wizard_portrait_segs[_players[0].wizard_id]);

    Set_Font_Style(4, 15, ST_NULL, ST_NULL);

    Set_Font_Colors_15(4, &FontShadow_Colors[0]);

    Print_Centered(78, 120, _players[0].name);

    Print_Centered(77, 120, _players[0].name);

    Set_Font_Colors_15(4, &Font_Colors[0]);

    Print_Centered(77, 119, _players[0].name);

    Moused_Control = Scan_Input();

    wsa_ptr = &_players[0].alchemy;

    /*
        BEGIN: same on Screen 6 & 7
    */

    start_X = 36;

    for(itr = 0; itr < _players[0].spellranks[sbr_Life]; itr++)
    {
        FLIC_Draw(start_X, 135, m_spellbook_pict_segs[(0 + TBL_Bookshelf_Books[itr])]);
        start_X += 8;
    }

    for(itr = 0; itr < _players[0].spellranks[sbr_Sorcery]; itr++)
    {
        FLIC_Draw(start_X, 135, m_spellbook_pict_segs[(3 + TBL_Bookshelf_Books[itr])]);
        start_X += 8;
    }

    for(itr = 0; itr < _players[0].spellranks[sbr_Nature]; itr++)
    {
        FLIC_Draw(start_X, 135, m_spellbook_pict_segs[(6 + TBL_Bookshelf_Books[itr])]);
        start_X += 8;
    }

    for(itr = 0; itr < _players[0].spellranks[sbr_Death]; itr++)
    {
        FLIC_Draw(start_X, 135, m_spellbook_pict_segs[(9 + TBL_Bookshelf_Books[itr])]);
        start_X += 8;
    }

    for(itr = 0; itr < _players[0].spellranks[sbr_Chaos]; itr++)
    {
        FLIC_Draw(start_X, 135, m_spellbook_pict_segs[(12 + TBL_Bookshelf_Books[itr])]);
        start_X += 8;
    }

    Draw_Special_Abilities_String();

    /*
        END: same on Screen 6 & 7
    */

    Set_Font_Colors_15(3, &Race_Title_Colors[0]);

    Set_Font_Style_Shadow_Down(3, 15, ST_NULL, ST_NULL);

    Print(198, 25, cnst_Arcanian_Races);  // "Arcanian Races:"

    for(itr2 = 0; itr2 < 9; itr2++)
    {

        Set_Font_Colors_15(2, Shadow_Color);

        // Print(221, (39 + (10 * itr2)), _race_type_table[Arcanus_Races[itr2]].name);
        DBG_race_type = Arcanus_Races[itr2];
        DBG_race_name_string = *_race_type_table[DBG_race_type].name;
        Print(221, (39 + (10 * itr2)), DBG_race_name_string);

        if(m_newgame_fields[itr2] == Moused_Control)
        {

            Set_Font_Colors_15(2, Race_Highlight_Color);

        }
        else
        {

            Set_Font_Colors_15(2, Available_Color);

        }

        // Print(220, (38 + (10 * itr2)), _race_type_table[Arcanus_Races[itr2]].name);
        DBG_race_type = Arcanus_Races[itr2];
        DBG_race_name_string = *_race_type_table[DBG_race_type].name;
        Print(220, (38 + (10 * itr2)), DBG_race_name_string);

    }

    Set_Font_Colors_15(3, &Race_Title_Colors[0]);

    Set_Font_Style_Shadow_Down(3, 15, ST_NULL, ST_NULL);

    Print(202, 134, cnst_Myrran_Races);  // "Myrran Races:"

    for(itr2 = 0; itr2 < 5; itr2++)
    {

        Set_Font_Colors_15(2, Shadow_Color);

        // Print(221, (148 + (10 * itr2)), _race_type_table[Myrran_Races[itr2]].name);
        DBG_race_type = Myrran_Races[itr2];
        DBG_race_name_string = *_race_type_table[DBG_race_type].name;
        Print(221, (148 + (10 * itr2)), DBG_race_name_string);

        if(wsa_ptr[rtt_Myrran] == 1)
        {

            if(m_newgame_fields[(9 + itr2)] == Moused_Control)
            {

                Set_Font_Colors_15(2, Race_Highlight_Color);

            }
            else
            {

                Set_Font_Colors_15(2, Available_Color);

            }

        }
        else
        {

            Set_Font_Colors_15(2, Not_Available_Color);

        }

        // Print(220, (147 + (10 * itr2)), _race_type_table[Myrran_Races[itr2]].name);
        DBG_race_type = Myrran_Races[itr2];
        DBG_race_name_string = *_race_type_table[DBG_race_type].name;
        Print(220, (147 + (10 * itr2)), DBG_race_name_string);

    }

}


// MGC  o50p17
/**
 * @brief Builds and draws the selected-special abilities text block on the New Game UI.
 *
 * @details
 * Reads the current human player's special ability flags, composes a comma-separated
 * list of enabled special ability names, appends a trailing period, and renders that
 * paragraph with shadow/highlight layered text colors.
 *
 * Processing flow:
 * 1) Initializes local buffers, color pointers, and special ability state counters.
 * 2) Points `wsa_ptr` at `_players[0].alchemy`, then treats the next 18 bytes
 *    as the contiguous special ability-flag array.
 * 3) Counts enabled special abilities (`== 1`) into `retort_count`.
 * 4) Rebuilds `string` by iterating enabled special abilities and calling
 *    `String_List_Builer(...)` before each appended special ability name from
 *    `wsa_names[itr]`.
 * 5) Appends `cnst_DOT__ovr050` when at least one special ability is present.
 * 6) Selects paragraph Y origin (`start_y`) based on line-density heuristic:
 *    > 5 special abilities starts one pixel higher.
 * 7) Draws the paragraph twice (shadow first, then highlight) with
 *    `Print_Paragraph(...)` to produce the final styled text.
 *
 * Key local variables:
 * - `string[126]`: composed output paragraph buffer.
 * - `retort_count`: number of active special abilities.
 * - `list_size`: running count used by `String_List_Builer` formatting.
 * - `colors1` / `colors2`: palette ramps used for layered text draw.
 * - `start_y`: chosen Y origin for paragraph rendering.
 *
 * @param void This function accepts no parameters.
 *
 * @return void
 * No return value. Draws directly into the active new-game draw surface.
 *
 * @note Relies on contiguous special ability storage beginning at `_players[0].alchemy`
 *       and on global string/color assets used by the New Game screen.
 *
 * @see String_List_Builer
 */
void Draw_Special_Abilities_String(void)
{
    uint8_t l_niu_array[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char string[126] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t start_y = 0;
    int16_t list_size = 0;
    int16_t niu_flag = 0;
    int8_t * wsa_ptr = 0;
    uint8_t * colors2 = 0;
    uint8_t * colors1 = 0;
    int16_t ability_count = 0;
    int16_t itr = 0;

    colors1 = string_colors1;  /* DEDU  why is string_colors1 a far pointer and string_colors2 is not? */
    colors2 = string_colors2;
    memcpy(l_niu_array, m_niu_array, 12);
    niu_flag = 0;
    list_size = 0;
    string[0] = 0;
    wsa_ptr = &_players[0].alchemy;
    ability_count = 0;

    for(itr = 0; itr < 18; itr++)
    {
        if(wsa_ptr[itr] == 1)
        {
            ability_count++;
        }
    }
    strcpy(string, empty_string__ovr050);
    list_size = 0;
    for(itr = 0; itr < NUM_WIZARD_SPECIAL_ABILITIES; itr++)
    {
        if(wsa_ptr[itr] == 1)
        {
            String_List_Builer(&list_size, ability_count, &string[0]);
            strcat(string, wsa_names[itr]);
        }
    }
    if(ability_count > 0)
    {
        strcat(string, cnst_DOT__ovr050);
    }
    if(ability_count > 5)
    {
        start_y = 178;
    }
    else
    {
        start_y = 180;
    }
    Set_Font_Style(0, 15, ST_NULL, ST_NULL);
    Set_Font_Colors_15(0, &colors1[0]);
    Set_Font_LF(1);
    Print_Paragraph(13, (start_y + 1), 138, &string[0], 0);
    Set_Font_Colors_15(0, &colors2[0]);
    Set_Font_LF(1);
    Print_Paragraph(12, start_y, 138, &string[0], 0);
}


// MGC  o50p18
/**
 * @brief Draws preset wizard spellbooks on the default wizard shelf.
 *
 * @details
 * Reads the selected preset wizard's realm book counts from
 * `_wizard_presets_table[wizard_id]` and renders bookshelf book sprites at
 * y=135, starting from x=36.
 *
 * The routine scans the 5 magic realms in table order and captures up to two
 * realms with non-zero book counts:
 * - first non-zero realm -> first draw group
 * - second non-zero realm -> second draw group
 *
 * For each captured realm, it draws `count` books using that realm's sprite
 * base index (`realm * 3`) plus `TBL_Bookshelf_Books[itr]` variation offsets,
 * advancing x by 8 pixels per book.
 *
 * @param wizard_id Index of the wizard preset entry in
 *                  `_wizard_presets_table`.
 *
 * @return void
 *
 * @note Current WIP behavior only tracks/draws the first two non-zero realms.
 *       Additional non-zero realms in the preset are ignored by this function.
 * @note Assumes `m_spellbook_pict_segs` is already loaded and `wizard_id`
 *       references a valid preset record.
 */
void NEWG_DrawDefShelf__WIP(int16_t wizard_id)
{
    int16_t Second_Book_Realm = 0;
    int16_t First_Book_Realm = 0;
    int16_t Second_Book_Count = 0;
    int16_t First_Book_Count = 0;
    int16_t * Profile_Books_Offset = 0;
    int16_t Second_Realm_Books_Index = 0;
    int16_t First_Realm_Books_Index = 0;
    int16_t itr = 0;  // _SI_
    int16_t IDK = 0;  // _DI_

    First_Realm_Books_Index = ST_UNDEFINED;
    Second_Realm_Books_Index = ST_UNDEFINED;
    First_Book_Count = 0;
    Second_Book_Count = 0;
    IDK = 36;
    First_Book_Realm = ST_UNDEFINED;
    Second_Book_Realm = ST_UNDEFINED;

    Profile_Books_Offset = &_wizard_presets_table[wizard_id].life;

    for(itr = 0; itr < 5; itr++)
    {
        if(Profile_Books_Offset[itr] > 0)
        {
            if(First_Realm_Books_Index == ST_UNDEFINED)
            {
                First_Realm_Books_Index = (itr * 3);
                First_Book_Count += Profile_Books_Offset[itr];
                First_Book_Realm = itr;
            }
            else
            {
                Second_Realm_Books_Index = (itr * 3);
                Second_Book_Count += Profile_Books_Offset[itr];
                Second_Book_Realm = itr;
            }
        }
    }

    if(First_Realm_Books_Index != ST_UNDEFINED)
    {
        for(itr = 0; itr < First_Book_Count; itr++)
        {
            FLIC_Draw(IDK, 135, m_spellbook_pict_segs[(First_Realm_Books_Index + TBL_Bookshelf_Books[itr])]);
            IDK += 8;
        }
    }

    if(Second_Realm_Books_Index != ST_UNDEFINED)
    {
        for(itr = 0; itr < Second_Book_Count; itr++)
        {
            FLIC_Draw(IDK, 135, m_spellbook_pict_segs[(Second_Realm_Books_Index + TBL_Bookshelf_Books[itr])]);
            IDK += 8;
        }
    }

}

// MGC  o50p19
/*
"Wizard Creation"  (Custom Wizard)
*/
/**
 * @brief Runs custom wizard creation screen 4 (books + abilities).
 *
 * @details
 * Loads screen assets and UI controls, resets wizard pick state, then runs the
 * event loop for selecting spell books and special abilities.
 *
 * Current flow:
 * 1) Loads NEWGAME LBX assets for the screen background, OK button states,
 *    spellbook sprites, warning box, and checkmark marker.
 * 2) Sets mouse/help/input context and creates controls (5 sliders, special ability
 *    label hitboxes, ESC hotkey, and OK field).
 * 3) Resets mutable state for this screen, including spell ranks, special ability flags,
 *    remaining picks, and error state.
 * 4) Registers `Newgame_Screen_4_Draw__WIP` as auto draw/update callback.
 * 5) Runs input loop until exit:
 *    - ESC returns `ngscr_Name`.
 *    - OK leaves only when `NEWG_ProfileComplete` is true.
 *    - Slider/special ability input updates profile picks and may set `NEWG_PickError`.
 *    - On error, builds warning text, shows `Warn0()`, and restores draw/help.
 * 6) Deactivates auto/help handlers and returns `ngscr_Spells`.
 *
 * Notes:
 * - Special ability toggle handling appears twice in the loop in the current WIP code;
 *   this comment reflects implemented behavior rather than intended cleanup.
 * - Special ability flags are accessed through `_players[0].alchemy` via `wsa_ptr`.
 *
 * @param void This function accepts no parameters.
 *
 * @return int16_t
 * @retval ngscr_Name   User cancelled with ESC; caller should navigate back.
 * @retval ngscr_Spells User completed this step; caller should advance.
 *
 * @note This function mutates global new-game creation state and UI globals and
 *       depends on loaded resource segments and input/draw subsystems.
 *
 * @see Newgame_Screen_4_Draw__WIP
 */
int16_t Newgame_Screen_4__WIP(void)
{
    char l_realm_name_character_array[5][10] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };
    char message[LEN_MESSAGE_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t hotkey_ESC = 0;
    int16_t DoublePick_Retort_Labels[5] = { 0, 0, 0, 0, 0 };
    char buffer[6] = { 0, 0, 0, 0, 0, 0 };
    int8_t * wsa_ptr = 0;
    int16_t Realms_Added = 0;
    int16_t (*p_wsa_prerequisites)[7] = 0;
    int16_t ability_count = 0;
    int16_t input_field_idx = 0;
    int16_t leave_screen = 0;
    int16_t First_Draw_Done = 0;
    int16_t itr = 0;
    int16_t itr2 = 0;
    int16_t some_flag = 0;

    memcpy(&l_realm_name_character_array, realm_name_character_array, 50);
    memcpy(&DoublePick_Retort_Labels, TBL_2pickRetort_Lbls, 10);

    // NEWGAME.LBX, 041  NEWSPELL   Magic pick screen
    // NEWGAME.LBX, 042  MAGICPIK   ok button
    // NEWGAME.LBX, 043  MAGICOK    non selectable ok
    newgame_background_seg = LBX_Reload(newgame_lbx_file__ovr050, 41, _screen_seg);
    newgame_ok_button_seg = LBX_Reload_Next(newgame_lbx_file__ovr050, 42, _screen_seg);
    _ok_inactive_seg = LBX_Reload_Next(newgame_lbx_file__ovr050, 43, _screen_seg);

    for(itr = 0; itr < 15; itr++)
    {
        // NEWGAME.LBX, 024  BOOKS      White book 1
        // NEWGAME.LBX, 025  BOOKS      White book 2
        // NEWGAME.LBX, 026  BOOKS      White book 3
        // NEWGAME.LBX, 027  BOOKS      Blue book 1
        // NEWGAME.LBX, 028  BOOKS      Blue book 2
        // NEWGAME.LBX, 029  BOOKS      Blue book 3
        // NEWGAME.LBX, 030  BOOKS      Green book 1
        // NEWGAME.LBX, 031  BOOKS      Green book 2
        // NEWGAME.LBX, 032  BOOKS      Green book 3
        // NEWGAME.LBX, 033  BOOKS      Black book 1
        // NEWGAME.LBX, 034  BOOKS      Black book 2
        // NEWGAME.LBX, 035  BOOKS      Black book 3
        // NEWGAME.LBX, 036  BOOKS      Red book 1
        // NEWGAME.LBX, 037  BOOKS      Red book 2
        // NEWGAME.LBX, 038  BOOKS      Red book 3
        m_spellbook_pict_segs[itr] = LBX_Reload_Next(newgame_lbx_file__ovr050, (24 + itr), _screen_seg);
    }

    // NEWGAME.LBX, 044  WARNBACK   Warning box top
    // NEWGAME.LBX, 045  WARNBACK   Warning box buttom
    m_warning_box_top_seg = LBX_Reload_Next(newgame_lbx_file__ovr050, 44, _screen_seg);
    m_warning_box_bottom_seg = LBX_Reload_Next(newgame_lbx_file__ovr050, 45, _screen_seg);

    // NEWGAME.LBX, 052  CHECKMRK
    _selection_marker_seg = LBX_Reload_Next(newgame_lbx_file__ovr050, 52, _screen_seg);


    Set_Mouse_List(1, mouse_list_newgame);

    Clear_Fields();

    Disable_Cancel();

    wsa_ptr = &_players[0].alchemy;

    leave_screen = ST_FALSE;

    First_Draw_Done = ST_FALSE;

    m_displayed_wizard = 0;

    spellpicks_count = 11;

    NEWG_ProfileComplete = 0;

    _ok_button = Add_Hidden_Field(251, 181, 282, 196, empty_string__ovr050[0], ST_UNDEFINED);

    m_bookshelf_life = 0;
    m_bookshelf_death = 0;
    m_bookshelf_chaos = 0;
    m_bookshelf_nature = 0;
    m_bookshelf_sorcery = 0;
    NEWG_PickError = 0;
    selected_load_game_slot_idx = 0;
    p_wsa_prerequisites = &wsa_prerequisites[0];

    for(itr = 0; itr < NUM_WIZARD_SPECIAL_ABILITIES; itr++)
    {
        wsa_ptr[itr] = 0;
    }

    m_newgame_fields[0] = Add_Scroll_Field(190,  47, 0, 96, 0, 95, 96, 22, &m_bookshelf_life, empty_string__ovr050[0], ST_UNDEFINED);
    m_newgame_fields[1] = Add_Scroll_Field(190,  73, 0, 96, 0, 95, 96, 22, &m_bookshelf_death, empty_string__ovr050[0], ST_UNDEFINED);
    m_newgame_fields[2] = Add_Scroll_Field(190,  99, 0, 96, 0, 95, 96, 22, &m_bookshelf_chaos, empty_string__ovr050[0], ST_UNDEFINED);
    m_newgame_fields[3] = Add_Scroll_Field(190, 125, 0, 96, 0, 95, 96, 22, &m_bookshelf_nature, empty_string__ovr050[0], ST_UNDEFINED);
    m_newgame_fields[4] = Add_Scroll_Field(190, 151, 0, 96, 0, 95, 96, 22, &m_bookshelf_sorcery, empty_string__ovr050[0], ST_UNDEFINED);

    for(itr = 0; itr < (NUM_WIZARD_SPECIAL_ABILITIES / 3); itr++)
    {
        wsa_picklist_fields[( 0 + itr)] = Add_Hidden_Field(164, (5 + (itr * 7)), 202, (10 + (itr * 7)), empty_string__ovr050[0], ST_UNDEFINED);
        wsa_picklist_fields[( 6 + itr)] = Add_Hidden_Field(203, (5 + (itr * 7)), 252, (10 + (itr * 7)), empty_string__ovr050[0], ST_UNDEFINED);
        wsa_picklist_fields[(12 + itr)] = Add_Hidden_Field(253, (5 + (itr * 7)), 319, (10 + (itr * 7)), empty_string__ovr050[0], ST_UNDEFINED);
    }

    hotkey_ESC = Add_Hot_Key((int16_t)str_ESC__ovr050[0]);

    _players[0].spellranks[0] = 0;
    _players[0].spellranks[1] = 0;
    _players[0].spellranks[2] = 0;
    _players[0].spellranks[3] = 0;
    _players[0].spellranks[4] = 0;

    Assign_Auto_Function(Newgame_Screen_4_Draw__WIP, 1);

    Set_Newgame_Screen_4_Help_List();

    // BUGBUG  DNE  Set_Input_Delay(2);?

    while(leave_screen == ST_FALSE)
    {

        input_field_idx = Get_Input();

        Mark_Time();

        if(input_field_idx == hotkey_ESC)
        {
            return ngscr_Name;
        }

        if(input_field_idx == _ok_button)
        {
            if(NEWG_ProfileComplete != ST_FALSE)
            {
                leave_screen = ST_TRUE;
            }
        }
        
        ability_count = 0;
        for(itr = 0; itr < NUM_WIZARD_SPECIAL_ABILITIES; itr++)
        {
            if(wsa_ptr[itr] == 1)
            {
                ability_count++;
            }
        }

        if(
            (
                (input_field_idx == m_newgame_fields[0])
                &&
                (_players[0].spellranks[sbr_Death] != 0)
            )
            ||
            (
                (input_field_idx == m_newgame_fields[1])
                &&
                (_players[0].spellranks[sbr_Life] != 0)
            )
        )
        {
            NEWG_PickError = 4;
        }

        if(NEWG_PickError > 0)
        {
            switch(NEWG_PickError)
            {
                case 1: { strcpy(message, cnst_Pick_Error_0); } break;
                case 2: { strcpy(message, cnst_Pick_Error_1); } break;
                case 3: {
                    strcpy(message, cnst_Pick_Error_20);  /* "To select " */
                    strcat(message, wsa_names[NEWG_PickAttempt]);
                    strcat(message, cnst_Pick_Error_21);  /* " you need:   " */
                    if(
                        (wsa_prerequisites[NEWG_PickAttempt][1] == 1)  // Realm_Count
                        &&
                        (wsa_prerequisites[NEWG_PickAttempt][0] == 0)  // Any_Realm
                    )
                    {
                        for(itr = 0; itr < 5; itr++)
                        {
                            if(p_wsa_prerequisites[NEWG_PickAttempt][(2 + itr)] > 0)
                            {
                                SDL_itoa(p_wsa_prerequisites[NEWG_PickAttempt][(2 + itr)], buffer, 10);
                                strcat(message, buffer);
                                strcat(message, cnst_Pick_Error_22);  /* " picks in " */
                                strcat(message, l_realm_name_character_array[itr]);
                                strcat(message, cnst_Pick_Error_23);  /* " Magic" */
                            }
                        }
                    }
                    else
                    {
                        if(wsa_prerequisites[NEWG_PickAttempt][0] <= 0)  // Any_Realm
                        {
                            Realms_Added = 0;
                            for(itr2 = 0; itr2 < 5; itr2++)
                            {
                                if(p_wsa_prerequisites[NEWG_PickAttempt][(2 + itr2)] > 0)
                                {
                                    if(Realms_Added > 0)
                                    {
                                        strcat(message, cnst_Pick_Error_29);
                                    }
                                    SDL_itoa(p_wsa_prerequisites[NEWG_PickAttempt][(2 + itr2)], buffer, 10);
                                    strcat(message, buffer);
                                    strcat(message, cnst_Pick_Error_2A);
                                    strcat(message, l_realm_name_character_array[itr2]);
                                    strcat(message, cnst_Pick_Error_23);
                                    Realms_Added++;
                                }
                            }
                        }
                        else
                        {
                            SDL_itoa(p_wsa_prerequisites[NEWG_PickAttempt][0], buffer, 10);
                            strcat(message, buffer);
                            strcat(message, cnst_Pick_Error_24);  /* " pick"*/
                            if(p_wsa_prerequisites[NEWG_PickAttempt][0] > 1)
                            {
                                strcat(message, cnst_Pick_Error_25);  /* "s" */
                            }
                            else
                            {
                                strcat(message, cnst_Pick_Error_26);  /* " in any " */
                            }
                            if(p_wsa_prerequisites[NEWG_PickAttempt][0] > 1)
                            {
                                SDL_itoa(p_wsa_prerequisites[NEWG_PickAttempt][1], buffer, 10);
                                strcat(message, buffer);
                                strcat(message, cnst_Pick_Error_27);
                            }
                            else
                            {
                                strcat(message, cnst_Pick_Error_28);
                            }
                        }
                    }
                } break;
                case 4: { strcpy(message, cnst_Pick_Error_3); } break;
                case 5: { strcpy(message, cnst_Pick_Error_4); } break;
                case 6: { strcpy(message, cnst_Pick_Error_5); } break;
            }
            Deactivate_Auto_Function();
            Deactivate_Help_List();
            Warn0(message);
            Assign_Auto_Function(Newgame_Screen_4_Draw__WIP, 1);
            Set_Help_List(_help_entries, 25);
            NEWG_PickError = 0;
        }

        for(itr = 0; itr < NUM_WIZARD_SPECIAL_ABILITIES; itr++)
        {
            if(wsa_picklist_fields[itr] == input_field_idx)
            {
                if(wsa_ptr[wsa_picklist_order[itr]] == ST_TRUE)  // selected
                {
                    wsa_ptr[wsa_picklist_order[itr]] = ST_FALSE;  // deselect
                }
                else  // unselected
                {
                    if(spellpicks_count <= 0)
                    {
                        NEWG_PickError = 5;
                    }
                    else
                    {
                        if(ability_count > 5)
                        {
                            NEWG_PickError = 2;
                        }
                        else
                        {
                            some_flag = ST_FALSE;
                            for(itr2 = 0; itr2 < 5; itr2++)
                            {
                                if(DoublePick_Retort_Labels[itr2] == itr)
                                {
                                    if(spellpicks_count < 2)
                                    {
                                        NEWG_PickError = 6;
                                    }
                                    else
                                    {
                                        wsa_ptr[wsa_picklist_order[itr]] = ST_TRUE;  // select
                                    }
                                    some_flag = ST_TRUE;
                                }
                            }
                            if(some_flag == ST_FALSE)
                            {
                                if(itr == 7)  // Myrran 3-picks
                                {
                                    if(spellpicks_count < 3)
                                    {
                                        NEWG_PickError = 6;
                                    }
                                    else
                                    {
                                        wsa_ptr[wsa_Myrran] = ST_TRUE; // select Myrran
                                    }
                                    some_flag = ST_TRUE;
                                }
                            }
                            if(some_flag == ST_FALSE)
                            {
                                wsa_ptr[wsa_picklist_order[itr]] = ST_TRUE;  // select
                            }
                        }
                    }
                }
            }
        }

        if(leave_screen == ST_FALSE)
        {
            Newgame_Screen_4_Draw__WIP();
            Apply_Palette();
            Toggle_Pages();
            if(First_Draw_Done == ST_FALSE)
            {
                First_Draw_Done = ST_TRUE;
                Copy_On_To_Off_Page();
            }
            Release_Time(2);
        }

    }

    Deactivate_Auto_Function();
    Deactivate_Help_List();

    return ngscr_Spells;

}


// MGC  o50p20
/**
 * @brief Draw/update callback for custom wizard screen 4.
 *
 * @details
 * Performs one frame of rendering and immediate state reconciliation for spell
 * rank sliders, special ability availability, and remaining-pick accounting.
 *
 * Current responsibilities:
 * 1) Polls focus via `Auto_Input()` and draws static screen elements.
 * 2) Recomputes special ability count/cost (including double-pick special abilities and Myrran)
 *    and updates global `spellpicks_count` from the 11-pick budget.
 * 3) Applies slider-driven spell rank changes with Life/Death exclusivity and
 *    per-realm pick spending rules.
 * 4) Draws selected books on shelves and rank rows.
 * 5) Enables active/inactive OK rendering and raises pick error 1 when OK is
 *    clicked while the profile is incomplete.
 * 6) Rebuilds local special ability availability map from spell-rank prerequisites and
 *    remaining picks, then draws special ability labels in available/known/unavailable
 *    colors, auto-clearing now-invalid selected special abilities.
 * 7) Updates `NEWG_ProfileComplete` and draws picks-left + wizard name labels.
 * 8) Finishes by drawing special ability description text (`Draw_Special_Abilities_String`).
 *
 * Key data:
 * - Special ability flags are `_players[0].alchemy` (`wsa_ptr`).
 * - `selected_load_game_slot_idx` is updated with the current focused control.
 * - `NEWG_PickError` may be set here for invalid immediate interactions.
 *
 * @param void This function accepts no parameters.
 *
 * @return void
 * No return value. Mutates global new-game UI/profile state and renders into
 * active draw buffers.
 *
 * @note Designed to be called repeatedly (auto-function draw callback) while
 *       the Spells & Special Abilities screen is active.
 *
 * @see Newgame_Screen_4__WIP
 * @see Draw_Special_Abilities_String
 */
void Newgame_Screen_4_Draw__WIP(void)
{
    int8_t l_wsa_picklist_start[NUM_WIZARD_SPECIAL_ABILITIES] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char spellpicks_count_string[20] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int8_t * wsa_ptr = NULL;
    uint8_t * Retort_Shadow_Color = 0;
    uint8_t * Known_Retort_Color = 0;
    uint8_t * Unavailable_Retort_Color = 0;
    uint8_t * Available_Retort_Color = 0;
    uint8_t WizName_Font_Colors[5] = { 0, 0, 0, 0, 0 };
    uint8_t WizName_Shadow_Colors[5] = { 0, 0, 0, 0, 0 };
    int16_t TwoPick_Label_Indices[5] = { 0, 0, 0, 0, 0 };
    int16_t Retorts_Cost_Total = 0;
    int16_t Possessed_Realm_Count = 0;
    int16_t Two_Book_Realm_Count = 0;
    int16_t ability_count = 0;
    int16_t bookshelf_start_x = 0;
    int16_t Picks_Before_Slider_Check = 0;
    int16_t Mouse_Focus_Control = 0;
    int16_t itr = 0;
    int16_t itr2 = 0;

    memcpy(&TwoPick_Label_Indices, RP_TBL_2pickRet_Lbls, 10);

    memcpy(&WizName_Shadow_Colors, COL_NEWG_5Shadow3, 5);

    memcpy(&WizName_Font_Colors, COL_NEWG_5Font3, 5);

    Available_Retort_Color = COL_Available_3;
    Unavailable_Retort_Color = COL_NEWG_NA2;
    Known_Retort_Color = COL_NewG_Ret_Known;
    Retort_Shadow_Color = COL_NewG_Ret_Shdw;

    memcpy(l_wsa_picklist_start, wsa_picklist_start, NUM_WIZARD_SPECIAL_ABILITIES);

    wsa_ptr = &_players[0].alchemy;

    Mouse_Focus_Control = Auto_Input();

    bookshelf_start_x = 36;

    FLIC_Draw(0, 0, newgame_background_seg);

    FLIC_Draw(24, 10, wizard_portrait_segs[_players[0].wizard_id]);

    Fill(251, 181, 282, 196, 0);

    ability_count = 0;
    for(itr = 0; itr < NUM_WIZARD_SPECIAL_ABILITIES; itr++)
    {
        if(wsa_ptr[itr] == 1) { ability_count += 1; }
    }
    Retorts_Cost_Total = ability_count;
    for(itr = 0; itr < 5; itr++)
    {
        if(wsa_ptr[TwoPick_Label_Indices[itr]] == ST_TRUE)
        {
            Retorts_Cost_Total += 1;
        }
    }
    if(wsa_ptr[wsa_Myrran] == ST_TRUE)
    {
        Retorts_Cost_Total += 2;
    }

    spellpicks_count = (
        11
        - Retorts_Cost_Total
        - _players[0].spellranks[3]
        - _players[0].spellranks[4]
        - _players[0].spellranks[2]
        - _players[0].spellranks[0]
        - _players[0].spellranks[1]
    );

    Picks_Before_Slider_Check = spellpicks_count;

    for(itr = 0; itr < 5; itr++)
    {
        if(m_newgame_fields[itr] == Mouse_Focus_Control)
        {
            switch(itr)
            {
                case 0:
                {
                    spellpicks_count += _players[0].spellranks[sbr_Life];
                    for(itr2 = 0; ((m_bookshelf_life / 8) > itr2); itr2++)  /* spellbook count = scroll position / spellbook picture width */
                    {
                        if(_players[0].spellranks[sbr_Death] != 0)
                        {
                            break;
                        }
                        if(spellpicks_count == 0)
                        {
                            break;
                        }
                        spellpicks_count -= 1;
                    }
                    _players[0].spellranks[sbr_Life] = itr2;  /* update spell rank based on slider position */
                    if((m_bookshelf_life / 8) == itr2)
                    {
                        if(spellpicks_count == Picks_Before_Slider_Check)
                        {
                            if(spellpicks_count != 0)
                            {
                                spellpicks_count -= 1;
                            }
                        }
                    }
                } break;
                case 1:
                {
                    spellpicks_count += _players[0].spellranks[sbr_Death];
                    for(itr2 = 0; ((m_bookshelf_death / 8) > itr2); itr2++)
                    {
                        if(_players[0].spellranks[sbr_Life] != 0)
                        {
                            break;
                        }
                        if(spellpicks_count == 0)
                        {
                            break;
                        }
                        spellpicks_count -= 1;
                    }
                    _players[0].spellranks[sbr_Death] = itr2;
                    if((m_bookshelf_death / 8) == itr2)
                    {
                        if(spellpicks_count == Picks_Before_Slider_Check)
                        {
                            if(spellpicks_count != 0)
                            {
                                spellpicks_count -= 1;
                            }
                        }
                    }
                } break;
                case 2:
                {
                    spellpicks_count += _players[0].spellranks[sbr_Chaos];
                    for(itr2 = 0; ((m_bookshelf_chaos / 8) > itr2); itr2++)
                    {
                        if(spellpicks_count == 0)
                        {
                            break;
                        }
                        spellpicks_count -= 1;
                    }
                    _players[0].spellranks[sbr_Chaos] = itr2;
                    if((m_bookshelf_chaos / 8) == itr2)
                    {
                        if(spellpicks_count == Picks_Before_Slider_Check)
                        {
                            if(spellpicks_count != 0)
                            {
                                spellpicks_count -= 1;
                            }
                        }
                    }
                } break;
                case 3:
                {
                    spellpicks_count += _players[0].spellranks[sbr_Nature];
                    for(itr2 = 0; ((m_bookshelf_nature / 8) > itr2); itr2++)
                    {
                        if(spellpicks_count == 0)
                        {
                            break;
                        }
                        spellpicks_count -= 1;
                    }
                    _players[0].spellranks[sbr_Nature] = itr2;
                    if((m_bookshelf_nature / 8) == itr2)
                    {
                        if(spellpicks_count == Picks_Before_Slider_Check)
                        {
                            if(spellpicks_count != 0)
                            {
                                spellpicks_count -= 1;
                            }
                        }
                    }
                } break;
                case 4:
                {
                    spellpicks_count += _players[0].spellranks[sbr_Sorcery];
                    for(itr2 = 0; ((m_bookshelf_sorcery / 8) > itr2); itr2++)
                    {
                        if(spellpicks_count == 0)
                        {
                            break;
                        }
                        spellpicks_count -= 1;
                    }
                    _players[0].spellranks[sbr_Sorcery] = itr2;
                    if((m_bookshelf_sorcery / 8) == itr2)
                    {
                        if(spellpicks_count == Picks_Before_Slider_Check)
                        {
                            if(spellpicks_count != 0)
                            {
                                spellpicks_count -= 1;
                            }
                        }
                    }
                } break;
            }
        }
    }

    selected_load_game_slot_idx = Mouse_Focus_Control;

    for(itr = 0; itr < _players[0].spellranks[sbr_Life]; itr++)
    {
        FLIC_Draw((197 + (8 * itr)), (49 + (0 * 26)), m_spellbook_pict_segs[( 0 + TBL_Realm1_Books[itr])]);
        FLIC_Draw(bookshelf_start_x, 135, m_spellbook_pict_segs[(0 + TBL_Bookshelf_Books[itr])]);
        bookshelf_start_x += 8;
    }
    for(itr = 0; itr < _players[0].spellranks[sbr_Death]; itr++)
    {
        FLIC_Draw((197 + (8 * itr)), (49 + (1 * 26)), m_spellbook_pict_segs[(9 + TBL_Realm1_Books[itr])]);
        FLIC_Draw(bookshelf_start_x, 135, m_spellbook_pict_segs[(9 + TBL_Bookshelf_Books[itr])]);
        bookshelf_start_x += 8;
    }
    for(itr = 0; itr < _players[0].spellranks[sbr_Chaos]; itr++)
    {
        FLIC_Draw((197 + (8 * itr)), (49 + (2 * 26)), m_spellbook_pict_segs[(12 + TBL_Realm1_Books[itr])]);
        FLIC_Draw(bookshelf_start_x, 135, m_spellbook_pict_segs[(12 + TBL_Bookshelf_Books[itr])]);
        bookshelf_start_x += 8;
    }
    for(itr = 0; itr < _players[0].spellranks[sbr_Nature]; itr++)
    {
        FLIC_Draw((197 + (8 * itr)), (49 + (3 * 26)), m_spellbook_pict_segs[(6 + TBL_Realm1_Books[itr])]);
        FLIC_Draw(bookshelf_start_x, 135, m_spellbook_pict_segs[(6 + TBL_Bookshelf_Books[itr])]);
        bookshelf_start_x += 8;
    }
    for(itr = 0; itr < _players[0].spellranks[sbr_Sorcery]; itr++)
    {
        FLIC_Draw((197 + (8 * itr)), (49 + (4 * 26)), m_spellbook_pict_segs[(3 + TBL_Realm1_Books[itr])]);
        FLIC_Draw(bookshelf_start_x, 135, m_spellbook_pict_segs[(3 + TBL_Bookshelf_Books[itr])]);
        bookshelf_start_x += 8;
    }

    if(NEWG_ProfileComplete != ST_FALSE)
    {
        if(Mouse_Focus_Control == _ok_button)
        {
            FLIC_Draw(253, 183, newgame_ok_button_seg);
        }
        else
        {
            FLIC_Draw(252, 182, newgame_ok_button_seg);
        }
    }
    else
    {
        FLIC_Draw(252, 182, _ok_inactive_seg);
        if(Mouse_Focus_Control == _ok_button)
        {
            NEWG_PickError = 1;
        }
    }

    if(_players[0].spellranks[sbr_Nature] > 3)
    {
        l_wsa_picklist_start[13] = 1;
        l_wsa_picklist_start[3] = 1;
        l_wsa_picklist_start[16] = 1;
    }
    if(_players[0].spellranks[sbr_Sorcery] > 3)
    {
        l_wsa_picklist_start[14] = 1;
        l_wsa_picklist_start[3] = 1;
        l_wsa_picklist_start[16] = 1;
    }
    if(_players[0].spellranks[sbr_Chaos] > 3)
    {
        l_wsa_picklist_start[12] = 1;
        l_wsa_picklist_start[3] = 1;
        l_wsa_picklist_start[16] = 1;
    }
    if(_players[0].spellranks[sbr_Death] > 3)
    {
        l_wsa_picklist_start[15] = 1;
        l_wsa_picklist_start[3] = 1;
        l_wsa_picklist_start[16] = 1;
    }
    if(_players[0].spellranks[sbr_Life] > 3)
    {
        l_wsa_picklist_start[8] = 1;
        l_wsa_picklist_start[3] = 1;
        l_wsa_picklist_start[16] = 1;
    }
    Possessed_Realm_Count = 0;
    Two_Book_Realm_Count = 0;
    for(itr = 0; itr < 5; itr++)
    {
        if(_players[0].spellranks[itr] > 0)
        {
            Possessed_Realm_Count++;
        }
        if(_players[0].spellranks[itr] > 1)
        {
            Two_Book_Realm_Count++;
        }   
    }

    if(Possessed_Realm_Count > 1)
    {
        l_wsa_picklist_start[6] = ST_TRUE;
    }

    if(Two_Book_Realm_Count > 2)  /* 2+ picks in 3+ realms */
    {
        l_wsa_picklist_start[10] = ST_TRUE;
    }

    for(itr = 0; itr < 5; itr++)
    {
        if(wsa_ptr[wsa_picklist_order[TwoPick_Label_Indices[itr]]] != ST_TRUE)
        {
            if(spellpicks_count < 2)
            {
                l_wsa_picklist_start[TwoPick_Label_Indices[itr]] = ST_FALSE;
            }
        }
    }

    if(wsa_ptr[wsa_Myrran] != ST_TRUE)
    {
        if(spellpicks_count < 3)
        {
            l_wsa_picklist_start[7] = ST_FALSE;
        }
    }

    if(
        (_players[0].spellranks[sbr_Chaos] > 0)
        &&
        (_players[0].spellranks[sbr_Sorcery] > 0)
        &&
        (_players[0].spellranks[sbr_Nature] > 0)
    )
    {
        l_wsa_picklist_start[17] = ST_TRUE;
    }

    Set_Font_Style(0, 15, ST_NULL, ST_NULL);

    for(itr2 = 0; itr2 < 2; itr2++)
    {
        for(itr = 0; itr < 6; itr++)
        {
            if(
                (wsa_ptr[wsa_picklist_order[(itr + (itr2 * 6))]] != ST_TRUE)
                ||
                (l_wsa_picklist_start[(itr + (itr2 * 6))] != ST_TRUE)
                ||
                (spellpicks_count < 0)
            )
            {
                if(wsa_ptr[wsa_picklist_order[(itr + (itr2 * 6))]] == ST_TRUE)
                {
                    if(l_wsa_picklist_start[(itr + (itr2 * 6))] == ST_FALSE)
                    {
                        NEWG_PickError = 3;
                        NEWG_PickAttempt = wsa_picklist_order[(itr + (itr2 * 6))];
                    }
                    wsa_ptr[wsa_picklist_order[(itr + (itr2 * 6))]] = ST_FALSE;
                }
                if(
                    (l_wsa_picklist_start[(itr + (itr2 * 6))] == ST_TRUE)
                    &&
                    (ability_count < 6)
                    &&
                    (spellpicks_count > 0)
                )
                {
                    Set_Font_Colors_15(0, &Retort_Shadow_Color[0]);
                    Print((172 + (itr2 * 39)), (6 + (itr * 7)), wsa_names[wsa_picklist_order[(itr + (itr2 * 6))]]);
                    Set_Font_Colors_15(0, &Available_Retort_Color[0]);
                }
                else
                {
                    Set_Font_Colors_15(0, &Retort_Shadow_Color[0]);
                    Print((172 + (itr2 * 39)), (6 + (itr * 7)), wsa_names[wsa_picklist_order[(itr + (itr2 * 6))]]);
                    Set_Font_Colors_15(0, &Unavailable_Retort_Color[0]);
                }
                Print((171 + (itr2 * 39)), (5 + (itr * 7)), wsa_names[wsa_picklist_order[(itr + (itr2 * 6))]]);
            }
            else
            {
                Set_Font_Colors_15(0, &Retort_Shadow_Color[0]);
                Print((172 + (itr2 * 39)), (6 + (itr * 7)), wsa_names[wsa_picklist_order[(itr + (itr2 * 6))]]);
                Set_Font_Colors_15(0, &Known_Retort_Color[0]);
                Print((171 + (itr2 * 39)), (5 + (itr * 7)), wsa_names[wsa_picklist_order[(itr + (itr2 * 6))]]);
                FLIC_Draw((165 + (itr2 * 39)), (6 + (itr * 7)), _selection_marker_seg);  // check mark
            }
        }
    }

    for(itr = 0; itr < 6; itr++)
    {
        if(
            (wsa_ptr[wsa_picklist_order[(12 + itr)]] != ST_TRUE)
            ||
            (l_wsa_picklist_start[(12 + itr)] != ST_TRUE)
            ||
            (spellpicks_count < 0)
        )
        {
            if(wsa_ptr[wsa_picklist_order[(12 + itr)]] == ST_TRUE)
            {
                if(l_wsa_picklist_start[(12 + itr)] == ST_FALSE)
                {
                    NEWG_PickError = 3;
                    NEWG_PickAttempt = wsa_picklist_order[(12 + itr)];
                }
                wsa_ptr[wsa_picklist_order[(12 + itr)]] = ST_FALSE;
            }
            if(
                (l_wsa_picklist_start[(12 + itr)] == ST_TRUE)
                &&
                (ability_count < 6)
                &&
                (spellpicks_count > 0)
            )
            {
                Set_Font_Colors_15(0, &Retort_Shadow_Color[0]);
                Print(261, (6 + (itr * 7)), wsa_names[wsa_picklist_order[(12 + itr)]]);
                Set_Font_Colors_15(0, &Available_Retort_Color[0]);
            }
            else
            {
                Set_Font_Colors_15(0, &Retort_Shadow_Color[0]);
                Print(261, (6 + (itr * 7)), wsa_names[wsa_picklist_order[(12 + itr)]]);
                Set_Font_Colors_15(0, &Unavailable_Retort_Color[0]);
            }
                Print(260, (5 + (itr * 7)), wsa_names[wsa_picklist_order[(12 + itr)]]);
        }
        else
        {
            Set_Font_Colors_15(0, &Retort_Shadow_Color[0]);
            Print(261, (6 + (itr * 7)), wsa_names[wsa_picklist_order[(12 + itr)]]);
            Set_Font_Colors_15(0, &Known_Retort_Color[0]);
            Print(260, (5 + (itr * 7)), wsa_names[wsa_picklist_order[(12 + itr)]]);
            FLIC_Draw(254, (6 + (itr * 7)), _selection_marker_seg);  // check mark
        }
    }

    SDL_itoa(spellpicks_count, spellpicks_count_string, 10);

    strcat(spellpicks_count_string, cnst_Picks);

    if(spellpicks_count == 0)
    {
        NEWG_ProfileComplete = ST_TRUE;
    }
    else
    {
        NEWG_ProfileComplete = ST_FALSE;
    }

    Set_Font_Style(3, 15, ST_NULL, ST_NULL);
    Set_Font_Colors_15(3, WizName_Font_Colors);
    Print_Centered(222, 186, spellpicks_count_string);

    Set_Font_Colors_15(3, WizName_Shadow_Colors);
    Print_Centered(221, 185, spellpicks_count_string);

    Set_Font_Style(4, 15, ST_NULL, ST_NULL);
    Set_Font_Colors_15(4, &WizName_Shadow_Colors[0]);
    Print_Centered(78, 120, _players[0].name);
    Print_Centered(77, 120, _players[0].name);

    Set_Font_Colors_15(4, &WizName_Font_Colors[0]);
    Print_Centered(77, 119, _players[0].name);

    Draw_Special_Abilities_String();
    
}


// MGC  o50p21
/**
 * @brief Runs custom wizard creation screen 5 (starting spell selection).
 *
 * @details
 * Initializes spell-selection assets/state, prepares per-realm start-spell
 * buffers, and executes the interactive selection loop for the currently
 * selected magic realm.
 *
 * Current flow:
 * 1) Loads background/UI assets (overlay, list/button art, OK states, books,
 *    warning box, check marker) and resets pick/error state.
 * 2) Clears player start-spell storage for all five realms.
 * 3) Loads help entries for "Select Spells" and initializes fields/hotkeys.
 * 4) Selects current realm from `TBL_Realm_Order` and, when that realm has
 *    more than one book, builds GUI and initializes common/uncommon/rare spell
 *    slots from `_default_spells` according to `TBL_SpellsPerBook_*`.
 * 5) Runs input loop:
 *    - ESC returns `ngscr_Creation`.
 *    - OK exits loop only when `NEWG_ProfileComplete` is true.
 *    - Handles common-spell label clicks (toggle/add/remove) and raises
 *      `NEWG_PickError` when selection constraints fail.
 *    - Repeatedly calls `Newgame_Screen_5_Draw`, palette/page updates, and
 *      frame timing while active.
 *
 * @param void This function accepts no parameters.
 *
 * @return void
 *
 * @note This function is WIP; uncommon/rare click handlers and some error
 *       message paths are currently stub/incomplete in source.
 * @note Mutates global New Game state, including spell selection buffers,
 *       active input fields, and profile-complete/error flags.
 *
 * @see Newgame_Screen_5_Draw
 * @see Newgame_Screen_5_Spell_Fields
 */
int16_t Newgame_Screen_5(void)
{
    char message[150] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Realm_Order_Array[5] = { 0, 0, 0, 0, 0 };
    int16_t * p_default_spells = 0;
    int16_t Click_Processed = 0;
    int16_t * p_start_spells = 0;
    int16_t spell_idx = 0;
    int16_t Selection_Realm = 0;
    int16_t spellrank_idx = 0;
    int16_t spellrank_cnt = 0;
    int16_t section_idx = 0;
    int16_t itr2 = 0;
    int16_t itr3 = 0;  /* only use for per column spell count?*/
    int16_t input_field_idx = 0;
    int16_t leave_screen = 0;
    int16_t First_Draw_Done = 0;
    int16_t itr = 0;  /* used for column count */
    int16_t itr4 = 0;

    memcpy(&Realm_Order_Array, TBL_Realm_Order, 10);

    // NEWGAME.LBX, 000  BACKGRND    Main screen back
    newgame_background_seg = LBX_Reload(newgame_lbx_file__ovr050, 0, _screen_seg);

    // NEWGAME.LBX, 047  SPELLS2    Title underline
    // NEWGAME.LBX, 048  SPELLS2    First dark region
    // NEWGAME.LBX, 049  SPELLS2    Second dark region
    // NEWGAME.LBX, 050  SPELLS2    Third dark region
    // NEWGAME.LBX, 051  PIKMAGIC   Ok & picks back
    IMG_NewG_RgtOverlay = LBX_Reload_Next(newgame_lbx_file__ovr050, 47, _screen_seg);
    m_shared_pict_segs[0] = LBX_Reload_Next(newgame_lbx_file__ovr050, 48, _screen_seg);
    m_shared_pict_segs[1] = LBX_Reload_Next(newgame_lbx_file__ovr050, 49, _screen_seg);
    m_shared_pict_segs[2] = LBX_Reload_Next(newgame_lbx_file__ovr050, 50, _screen_seg);
    m_shared_pict_segs[3] = LBX_Reload_Next(newgame_lbx_file__ovr050, 51, _screen_seg);

    // NEWGAME.LBX, 042  MAGICPIK   ok button
    // NEWGAME.LBX, 043  MAGICOK    non selectable ok
    newgame_ok_button_seg = LBX_Reload_Next(newgame_lbx_file__ovr050, 42, _screen_seg);
    _ok_inactive_seg = LBX_Reload_Next(newgame_lbx_file__ovr050, 43, _screen_seg);

    /*
    NEWGAME.LBX, 024  BOOKS      White book 1
    NEWGAME.LBX, 025  BOOKS      White book 2
    NEWGAME.LBX, 026  BOOKS      White book 3
    NEWGAME.LBX, 027  BOOKS      Blue book 1
    NEWGAME.LBX, 028  BOOKS      Blue book 2
    NEWGAME.LBX, 029  BOOKS      Blue book 3
    NEWGAME.LBX, 030  BOOKS      Green book 1
    NEWGAME.LBX, 031  BOOKS      Green book 2
    NEWGAME.LBX, 032  BOOKS      Green book 3
    NEWGAME.LBX, 033  BOOKS      Black book 1
    NEWGAME.LBX, 034  BOOKS      Black book 2
    NEWGAME.LBX, 035  BOOKS      Black book 3
    NEWGAME.LBX, 036  BOOKS      Red book 1
    NEWGAME.LBX, 037  BOOKS      Red book 2
    NEWGAME.LBX, 038  BOOKS      Red book 3
    */
    for(itr = 0; itr < 15; itr++)
    {
        m_spellbook_pict_segs[itr] = LBX_Reload_Next(newgame_lbx_file__ovr050, (24 + itr), _screen_seg);
    }

    // NEWGAME.LBX, 044  WARNBACK   Warning box top
    // NEWGAME.LBX, 045  WARNBACK   Warning box buttom
    m_warning_box_top_seg = LBX_Reload_Next(newgame_lbx_file__ovr050, 44, _screen_seg);
    m_warning_box_bottom_seg = LBX_Reload_Next(newgame_lbx_file__ovr050, 45, _screen_seg);

    // NEWGAME.LBX, 052  CHECKMRK
    _selection_marker_seg = LBX_Reload_Next(newgame_lbx_file__ovr050, 52, _screen_seg);

    spellpicks_count = 0;

    NEWG_PickError = 0;

    Assign_Auto_Function(Newgame_Screen_5_Draw, 1);

    for(itr = 0; itr < 13; itr++)
    {
        _player_start_spells[0].realms[sbr_Nature].spells[itr] = 0;
        _player_start_spells[0].realms[sbr_Sorcery].spells[itr] = 0;
        _player_start_spells[0].realms[sbr_Chaos].spells[itr] = 0;
        _player_start_spells[0].realms[sbr_Life].spells[itr] = 0;
        _player_start_spells[0].realms[sbr_Death].spells[itr] = 0;
    }

    // HLPENTRY.LBX, 036  ""  "Select Spells Help"
    LBX_Load_Data_Static(hlpentry_lbx_file__MGC_ovr050, 36, (SAMB_ptr)_help_entries, 0, 30, 10);

    for(itr2 = 0; itr2 < 5; itr2++)
    {
            
        leave_screen = ST_TRUE;

        m_select_spells_realm = Realm_Order_Array[itr2];  // WTF

        NEWG_ProfileComplete = ST_FALSE;

        Clear_Fields();

        _quit_button = Add_Hot_Key(str_ESC__ovr050[0]);

        _ok_button = Add_Hidden_Field(251, 181, 282, 196, empty_string__ovr050[0], ST_UNDEFINED);

        for(itr = 0; itr < 30; itr++)
        {
            m_newgame_fields[itr] = INVALID_FIELD;
        }

        /*
            NOTE: The normal screen-loop is inside this magic realms spellrank loop
            "Select [Magic Realm] Spells"
        */
        if(_players[0].spellranks[m_select_spells_realm] > 1)
        {

            Newgame_Screen_5_Spell_Fields();

            leave_screen = ST_FALSE;

            First_Draw_Done = ST_FALSE;

            if(m_select_spells_realm == sbr_Chaos)
            {
                p_start_spells = &_player_start_spells[0].realms[sbr_Chaos].spells[0];
                p_default_spells = &_default_spells[sbr_Chaos].spells[0];
                Selection_Realm = sbr_Chaos;
            }
            if(m_select_spells_realm == sbr_Sorcery)
            {
                p_start_spells = &_player_start_spells[0].realms[sbr_Sorcery].spells[0];
                p_default_spells = &_default_spells[sbr_Sorcery].spells[0];
                Selection_Realm = sbr_Sorcery;
            }
            if(m_select_spells_realm == sbr_Nature)
            {
                p_start_spells = &_player_start_spells[0].realms[sbr_Nature].spells[0];
                p_default_spells = &_default_spells[sbr_Nature].spells[0];
                Selection_Realm = sbr_Nature;
            }
            if(m_select_spells_realm == sbr_Life)
            {
                p_start_spells = &_player_start_spells[0].realms[sbr_Life].spells[0];
                p_default_spells = &_default_spells[sbr_Life].spells[0];
                Selection_Realm = sbr_Life;
            }
            if(m_select_spells_realm == sbr_Death)
            {
                p_start_spells = &_player_start_spells[0].realms[sbr_Death].spells[0];
                p_default_spells = &_default_spells[sbr_Death].spells[0];
                Selection_Realm = sbr_Death;
            }
            spellrank_cnt = (_players[0].spellranks[m_select_spells_realm] - 1);
            spellrank_idx = (spellrank_cnt - 1);
            for(itr = 0; m_select_count_common[spellrank_idx] > itr; itr++)
            {
                p_start_spells[itr] = p_default_spells[itr];
            }
            for(itr = 0; m_select_count_uncommon[spellrank_idx] > itr; itr++)
            {
                p_start_spells[(10 + itr)] = p_default_spells[(10 + itr)];
            }
            for(itr = 0; m_select_count_rare[spellrank_idx] > itr; itr++)
            {
                p_start_spells[(12 + itr)] = p_default_spells[(12 + itr)];
            }

        }

        while(leave_screen == ST_FALSE)
        {
            Mark_Time();
            input_field_idx = Get_Input();
            if(input_field_idx == _quit_button)
            {
                return ngscr_Creation;
            }
            if(input_field_idx == _ok_button)
            {
                if(NEWG_ProfileComplete != ST_FALSE)
                {
                    leave_screen = ST_TRUE;
                }
            }
            if(spellrank_cnt > 0)
            {
                if(First_Draw_Done != ST_FALSE)
                {
                    if(input_field_idx != 0)
                    {
                        section_idx = 0;
                        spellrank_idx = (spellrank_cnt - 1);
                        if(
                            (m_select_count_common[spellrank_idx] > 0)
                            &&
                            (m_select_count_common[spellrank_idx] < 10)
                        )
                        {
                            for(itr = 0; itr < 2; itr++)
                            {
                                for(itr3 = 0; itr3 < 5; itr3++)
                                {
                                    spell_idx = ( 0 + (Selection_Realm * NUM_SPELLS_PER_MAGIC_REALM) + (itr * 5) + itr3 + 1);
                                    if(m_newgame_fields[((itr * 5) + itr3)] == input_field_idx)
                                    {
                                        Click_Processed = ST_FALSE;
                                        for(itr4 = 0; m_select_count_common[spellrank_idx] > itr4; itr4++)
                                        {
                                            if(p_start_spells[itr4] == spell_idx)
                                            {
                                                if(Click_Processed == ST_FALSE)
                                                {
                                                    p_start_spells[itr4] = spl_NONE;
                                                    Click_Processed = ST_TRUE;
                                                }
                                            }
                                        }
                                        if(Click_Processed == ST_FALSE)
                                        {
                                            for(itr4 = 0; m_select_count_common[spellrank_idx] > itr4; itr4++)
                                            {
                                                if(
                                                    (
                                                        (p_start_spells[itr4] == spl_NONE)
                                                        &&
                                                        (Click_Processed == ST_FALSE)
                                                    )
                                                    ||
                                                    (m_select_count_common[spellrank_idx] == ST_TRUE)
                                                )
                                                {
                                                    p_start_spells[itr4] = spell_idx;
                                                    Click_Processed = ST_TRUE;
                                                }
                                            }
                                        }
                                        if(Click_Processed == ST_FALSE)
                                        {
                                            NEWG_PickError = 2;
                                        }
                                    }
                                }
                            }
                            section_idx++;
                        }
                        /*
                            BEGIN:  Uncommon
                        */
                        if(m_select_count_uncommon[spellrank_idx] > 0)
                        {
                            for(itr = 0; itr < 2; itr++)
                            {
                                for(itr3 = 0; itr3 < 5; itr3++)
                                {
                                    spell_idx = (10 + (Selection_Realm * NUM_SPELLS_PER_MAGIC_REALM) + (itr * 5) + itr3 + 1);
                                    if(m_newgame_fields[((section_idx * 10) + (itr * 5) + itr3)] == input_field_idx)
                                    {
                                        Click_Processed = ST_FALSE;
                                        for(itr4 = 0; m_select_count_uncommon[spellrank_idx] > itr4; itr4++)
                                        {
                                            if(p_start_spells[(10 + itr4)] == spell_idx)
                                            {
                                                if(Click_Processed == ST_FALSE)
                                                {
                                                    p_start_spells[(10 + itr4)] = spl_NONE;
                                                    Click_Processed = ST_TRUE;
                                                }
                                            }
                                        }
                                        if(Click_Processed == ST_FALSE)
                                        {
                                            for(itr4 = 0; m_select_count_uncommon[spellrank_idx] > itr4; itr4++)
                                            {
                                                if(
                                                    (
                                                        (p_start_spells[(10 + itr4)] == spl_NONE)
                                                        &&
                                                        (Click_Processed == ST_FALSE)
                                                    )
                                                    ||
                                                    (m_select_count_uncommon[spellrank_idx] == ST_TRUE)
                                                )
                                                {
                                                    p_start_spells[(10 + itr4)] = spell_idx;
                                                    Click_Processed = ST_TRUE;
                                                }
                                            }
                                        }
                                        if(Click_Processed == ST_FALSE)
                                        {
                                            NEWG_PickError = 2;
                                        }
                                    }
                                }
                            }
                            section_idx++;
                        }
                        /*
                            END:  Uncommon
                        */
                        /*
                            BEGIN:  Rare
                        */
                        if(m_select_count_rare[spellrank_idx] > 0)
                        {
                            for(itr = 0; itr < 2; itr++)
                            {
                                for(itr3 = 0; itr3 < 5; itr3++)
                                {
                                    spell_idx = (20 + (Selection_Realm * NUM_SPELLS_PER_MAGIC_REALM) + (itr * 5) + itr3 + 1);
                                    if(m_newgame_fields[((section_idx * 10) + (itr * 5) + itr3)] == input_field_idx)
                                    {
                                        Click_Processed = ST_FALSE;
                                        for(itr4 = 0; m_select_count_rare[spellrank_idx] > itr4; itr4++)
                                        {
                                            if(p_start_spells[(12 + itr4)] == spell_idx)
                                            {
                                                if(Click_Processed == ST_FALSE)
                                                {
                                                    p_start_spells[(12 + itr4)] = spl_NONE;
                                                    Click_Processed = ST_TRUE;
                                                }
                                            }
                                        }
                                        if(Click_Processed == ST_FALSE)
                                        {
                                            for(itr4 = 0; m_select_count_rare[spellrank_idx] > itr4; itr4++)
                                            {
                                                if(
                                                    (
                                                        (p_start_spells[(12 + itr4)] == spl_NONE)
                                                        &&
                                                        (Click_Processed == ST_FALSE)
                                                    )
                                                    ||
                                                    (m_select_count_rare[spellrank_idx] == ST_TRUE)
                                                )
                                                {
                                                    p_start_spells[(12 + itr4)] = spell_idx;
                                                    Click_Processed = ST_TRUE;
                                                }
                                            }
                                        }
                                        if(Click_Processed == ST_FALSE)
                                        {
                                            NEWG_PickError = 2;
                                        }
                                    }
                                }
                            }
                            section_idx++;
                        }
                        /*
                            END:  Rare
                        */
                    }
                }
            }

// ; display and clear the stored error

            if(NEWG_PickError > 0)
            {
                switch(NEWG_PickError)
                {
                    case 1: { strcpy(message, cnst_Pick_Error_0); } break;
                    case 2: { strcpy(message, cnst_Pick_Error_6); } break;
                }
            }

            if(leave_screen == ST_FALSE)
            {
                Newgame_Screen_5_Draw();
                Apply_Palette();
                Toggle_Pages();
                if(First_Draw_Done == ST_FALSE)
                {
                    First_Draw_Done = ST_TRUE;
                    Copy_On_To_Off_Page();
                }
                Release_Time(2);
            }

        }  /* while(leave_screen == ST_FALSE) */

    }  /* for(itr2 = 0; itr2 < 5; itr2++) */

    Deactivate_Auto_Function();

    // NOTE(drake178); BUG: prevents moving backwards if there are no spells to select
    return ngscr_Race;

}


// MGC  o50p22
/**
 * @brief Builds spell-label input fields and help mapping for screen 5.
 *
 * @details
 * Configures clickable label boxes (`m_newgame_fields`) and help-entry indices
 * (`_help_entries`) for the currently selected spell realm on the custom
 * wizard spell-selection screen.
 *
 * Current flow:
 * 1) Copies local layout helper arrays (`m_niu_title_start_y`,
 *    `m_niu_box_start_y`) into local temporaries.
 * 2) Resolves `Selection_Realm` from `m_select_spells_realm`.
 * 3) Computes book-derived rank count/index from
 *    `_players[0].spellranks[m_select_spells_realm]`.
 * 4) Clears all 30 help-entry slots to `ST_UNDEFINED`.
 * 5) For each enabled rarity section (Common/Uncommon/Rare), creates a 2x5
 *    grid of hidden label fields, assigns corresponding help indices for that
 *    section/realm, and advances section offsets.
 * 6) Activates the assembled help list via `Set_Help_List(_help_entries, 30)`.
 *
 * @param void This function accepts no parameters.
 *
 * @return void
 *
 * @note Mutates global arrays `m_newgame_fields` and `_help_entries`.
 * @note The copied `l_niu_*` layout arrays are currently not consumed further
 *       in this implementation.
 *
 * @see Newgame_Screen_5
 * @see Set_Help_List
 */
void Newgame_Screen_5_Spell_Fields(void)
{
    int16_t l_niu_box_start_y[3] = { 0, 0, 0, };
    int16_t l_niu_title_start_y[3] = { 0, 0, 0, };
    int16_t field_idx = 0;
    int16_t start_y = 0;
    int16_t Selection_Realm = 0;
    int16_t spellrank_idx = 0;
    int16_t help_section = 0;
    int16_t spellrank_cnt = 0;
    int16_t itr = 0;
    int16_t itr2 = 0;

    memcpy(l_niu_title_start_y, m_niu_title_start_y, 6);
    memcpy(l_niu_box_start_y, m_niu_box_start_y, 6);

    if(m_select_spells_realm == sbr_Chaos)
    {
        Selection_Realm = sbr_Chaos;
    }
    if(m_select_spells_realm == sbr_Sorcery)
    {
        Selection_Realm = sbr_Sorcery;
    }
    if(m_select_spells_realm == sbr_Nature)
    {
        Selection_Realm = sbr_Nature;
    }
    if(m_select_spells_realm == sbr_Life)
    {
        Selection_Realm = sbr_Life;
    }
    if(m_select_spells_realm == sbr_Death)
    {
        Selection_Realm = sbr_Death;
    }

    spellrank_cnt = (_players[0].spellranks[m_select_spells_realm] - 1);

    help_section = 0;

    field_idx = 0;

    start_y = (39 + (help_section * 51));

    for(itr = 0; itr < 30; itr++)
    {

        _help_entries[itr].help_idx = ST_UNDEFINED;

    }

    if(spellrank_cnt > 0)
    {
        spellrank_idx = (spellrank_cnt - 1);
        if(
            (m_select_count_common[spellrank_idx] > 0)
            &&
            (m_select_count_common[spellrank_idx] < 10)
        )
        {
            for(itr = 0; itr < 2; itr++)
            {
                for(itr2 = 0; itr2 < 5; itr2++)
                {
                    m_newgame_fields[field_idx] = Add_Hidden_Field((170 + (74 * itr)), (start_y + (7 * itr2)), (240 + (78 * itr)), (start_y + 5 + (7 * itr2)), (int16_t)empty_string__ovr050[0], ST_UNDEFINED);
                    field_idx++;
                }
            }
            for(itr = 0; itr < 10; itr++)
            {
                _help_entries[((help_section * 10) + itr)].help_idx = (((Selection_Realm * 40) + itr) + (1 + (0 * 10)));
            }
            help_section++;
        }

        if(m_select_count_uncommon[spellrank_idx] > 0)
        {
            start_y = (39 + (help_section * 51));
            for(itr = 0; itr < 2; itr++)
            {
                for(itr2 = 0; itr2 < 5; itr2++)
                {
                    m_newgame_fields[field_idx] = Add_Hidden_Field((170 + (74 * itr)), (start_y + (7 * itr2)), (240 + (78 * itr)), (start_y + 5 + (7 * itr2)), (int16_t)empty_string__ovr050[0], ST_UNDEFINED);
                    field_idx++;
                }
            }
            for(itr = 0; itr < 10; itr++)
            {
                _help_entries[((help_section * 10) + itr)].help_idx = (((Selection_Realm * 40) + itr) + (1 + (1 * 10)));
            }
            help_section++;
        }

        if(m_select_count_rare[spellrank_idx] > 0)
        {
            start_y = (39 + (help_section * 51));
            for(itr = 0; itr < 2; itr++)
            {
                for(itr2 = 0; itr2 < 5; itr2++)
                {
                    m_newgame_fields[field_idx] = Add_Hidden_Field((170 + (74 * itr)), (start_y + (7 * itr2)), (240 + (78 * itr)), (start_y + 5 + (7 * itr2)), (int16_t)empty_string__ovr050[0], ST_UNDEFINED);
                    field_idx++;
                }
            }
            for(itr = 0; itr < 10; itr++)
            {
                _help_entries[((help_section * 10) + itr)].help_idx = (((Selection_Realm * 40) + itr) + (1 + (2 * 10)));
            }
            help_section++;
        }

    }

    Set_Help_List(_help_entries, 30);

}


/**
 * @brief Draws one frame of custom-wizard screen 5 (spell selection).
 *
 * @details
 * Performs the current render/update pass for the spell-selection screen:
 * background/layout draw, wizard name and bookshelf draw, OK button state, and
 * remaining-picks label draw.
 *
 * Current implemented behavior:
 * 1) Reads hovered control via `Auto_Input()` into `auto_input_field_idx`.
 * 2) Draws background, right overlay, and lower panel/button art.
 * 3) Draws wizard name text with shadow/foreground color layers.
 * 4) Draws bookshelf books from `_players[0].spellranks[]` across realms.
 * 5) Draws active/inactive OK button based on `NEWG_ProfileComplete`; if
 *    incomplete and hovered over OK, sets `NEWG_PickError = 1`.
 * 6) Builds and prints picks-remaining text from `spellpicks_count` and
 *    updates `NEWG_ProfileComplete` when picks reach zero.
 * 7) Draws selected-abilities summary and delegates extra screen-5 detail draw
 *    to `Newgame_Screen_5_Draw_Spells()`.
 *
 * @param void This function accepts no parameters.
 *
 * @return void
 *
 * @note This routine is still WIP and currently contains rough edges in local
 *       temporary usage (`buffer`, `Font_Colors`, `Shadow_Colors`) in source.
 * @note Intended to be called repeatedly from `Newgame_Screen_5` during
 *       the interactive selection loop.
 *
 * @see Newgame_Screen_5
 * @see Newgame_Screen_5_Draw_Spells
 */
void Newgame_Screen_5_Draw(void)
{
    char buffer[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    uint8_t light_colors[5] = { 0, 0, 0, 0, 0 };
    uint8_t dark_colors[5] = { 0, 0, 0, 0, 0 };
    int16_t x_start = 0;
    int16_t itr = 0;
    memcpy(dark_colors, m_dark_colors, 5);
    memcpy(light_colors, m_light_colors, 5);
    auto_input_field_idx = Auto_Input();
    FLIC_Draw(0, 0, newgame_background_seg);
    FLIC_Draw(181, 18, IMG_NewG_RgtOverlay);  /* ((360 / 2) + 1); 18 is the start of the rope */
    FLIC_Draw(196, 180, m_shared_pict_segs[3]);  /* "n picks" box */
    Fill(251, 181, 282, 196, ST_TRANSPARENT);  /* OK button box */
    /*
        BEGIN:  Player Name
    */
    Set_Font_Style(0, 15, ST_NULL, ST_NULL);
    Set_Font_Colors_15(4, &dark_colors[0]);
    Print_Centered(78, 120, _players[0].name);
    Print_Centered(77, 120, _players[0].name);
    Set_Font_Colors_15(4, &light_colors[0]);
    Print_Centered(77, 119, _players[0].name);
    /*
        END:  Player Name
    */
    /*
        BEGIN:  Bookshelf
    */
    x_start = 36;
    for(itr = 0; itr < _players[0].spellranks[3]; itr++)
    {
        FLIC_Draw(x_start, 135, m_spellbook_pict_segs[( 0 + TBL_Bookshelf_Books[itr])]);
        x_start += 8;
    }
    for(itr = 0; itr < _players[0].spellranks[4]; itr++)
    {
        FLIC_Draw(x_start, 135, m_spellbook_pict_segs[(9 + TBL_Bookshelf_Books[itr])]);
        x_start += 8;
    }
    for(itr = 0; itr < _players[0].spellranks[2]; itr++)
    {
        FLIC_Draw(x_start, 135, m_spellbook_pict_segs[(12 + TBL_Bookshelf_Books[itr])]);
        x_start += 8;
    }
    for(itr = 0; itr < _players[0].spellranks[0]; itr++)
    {
        FLIC_Draw(x_start, 135, m_spellbook_pict_segs[(6 + TBL_Bookshelf_Books[itr])]);
        x_start += 8;
    }
    for(itr = 0; itr < _players[0].spellranks[1]; itr++)
    {
        FLIC_Draw(x_start, 135, m_spellbook_pict_segs[(3 + TBL_Bookshelf_Books[itr])]);
        x_start += 8;
    }
    /*
        END:  Bookshelf
    */
    if(NEWG_ProfileComplete != ST_FALSE)
    {
        if(auto_input_field_idx == _ok_button)
        {
            FLIC_Draw(253, 183, newgame_ok_button_seg);  // BUGBUG  shoudl be up/down, but probably should just let Fields, Push Down handle it?
        }
        else
        {
            FLIC_Draw(252, 182, newgame_ok_button_seg);
        }
    }
    else
    {
        FLIC_Draw(252, 182, _ok_inactive_seg);
        if(auto_input_field_idx == _ok_button)
        {
            NEWG_PickError = 1;
        }
    }
    SDL_itoa(spellpicks_count, buffer, 10);
    strcat(buffer, cnst_Picks);
/* WTF */    if(spellpicks_count == 0)
/* WTF */    {
/* WTF */        NEWG_ProfileComplete = ST_TRUE;
/* WTF */    }
/* WTF */    else
/* WTF */    {
/* WTF */        NEWG_ProfileComplete = ST_FALSE;
/* WTF */    }
    Set_Font_Style(3, 15, ST_NULL, ST_NULL);
    Set_Font_Colors_15(3, &light_colors[0]);
    Print_Centered(222, 186, buffer);
    Set_Font_Colors_15(3, &dark_colors[0]);
    Print_Centered(221, 185, buffer);

    Draw_Special_Abilities_String();

    Newgame_Screen_5_Draw_Spells();

}


// MGC  o50p24
void Newgame_Screen_5_Draw_Spells(void)
{
    char section_title_string[30] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char realm_strings[5][10] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };
    char rarity_strings[4][10] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };
    int16_t selected_spells[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    uint8_t realm_color = 0;
    uint8_t realm_colors[5] = { 0, 0, 0, 0, 0 };
    char buffer[4] = { 0, 0, 0, 0 };
    uint8_t * l_shadow_colors = NULL;
    uint8_t * l_selected_colors = NULL;
    uint8_t * l_deselected_colors = NULL;
    int16_t section_box_start_y[3] = { 0, 0, 0 };
    int16_t section_title_start_y[3] = { 0, 0, 0 };
    int16_t Display_Type = 0;
    int16_t niu_selection_count = 0;  // ; always written, never read
    int16_t * p_start_spells = NULL;
    int16_t spell_idx = 0;
    int16_t Label_Box_Height = 0;
    int16_t Selection_Realm = 0;
    int16_t spellrank_idx = 0;
    int16_t section_idx = 0;
    int16_t spellrank_cnt = 0;
    int16_t Selection_Index = 0;
    int16_t itr2 = 0;
    int16_t itr = 0;
    
    memcpy(section_title_start_y, Label_Box_TitleTops2, 6);
    memcpy(section_box_start_y, Label_Box_TextTops2, 6);
    l_deselected_colors = m_select_spells_deselected_colors;
    l_selected_colors = m_select_spells_selected_colors;
    l_shadow_colors = m_select_spells_shadow_colors;
    memcpy(rarity_strings, m_rarity_strings, 40);
    memcpy(realm_strings, m_realm_strings, 50);

    if(m_select_spells_realm == sbr_Chaos)
    {
        p_start_spells = &_player_start_spells[0].realms[sbr_Chaos].spells[0];
        realm_color = CHAOS_RED;
        Selection_Realm = sbr_Chaos;
    }
    if(m_select_spells_realm == sbr_Sorcery)
    {
        p_start_spells = &_player_start_spells[0].realms[sbr_Sorcery].spells[0];
        realm_color = SORCERY_BLUE;
        Selection_Realm = sbr_Sorcery;
    }
    if(m_select_spells_realm == sbr_Nature)
    {
        p_start_spells = &_player_start_spells[0].realms[sbr_Nature].spells[0];
        realm_color = NATURE_GREEN;
        Selection_Realm = sbr_Nature;
    }
    if(m_select_spells_realm == sbr_Life)
    {
        p_start_spells = &_player_start_spells[0].realms[sbr_Life].spells[0];
        realm_color = LIFE_WHITE;
        Selection_Realm = sbr_Life;
    }
    if(m_select_spells_realm == sbr_Death)
    {
        p_start_spells = &_player_start_spells[0].realms[sbr_Death].spells[0];
        realm_color = DEATH_PURPLE;
        Selection_Realm = sbr_Death;
    }

    for(itr = 0; itr < 5; itr++)
    {
        realm_colors[itr] = realm_color;
    }

    strcpy(section_title_string, cnst_Spell_Select_0);
    strcat(section_title_string, realm_strings[Selection_Realm]);
    strcat(section_title_string, cnst_Spell_Select_1);
    Set_Font_Colors_15(4, &realm_colors[0]);
    Set_Font_Style_Shadow_Down(4, 15, ST_NULL, ST_NULL);
    Print_Centered(241, 6, section_title_string);  /* "Select " [] " Spells" */

    FLIC_Draw(24, 10, wizard_portrait_segs[_players[0].wizard_id]);

    spellrank_cnt = (_players[0].spellranks[m_select_spells_realm] - 1);

    section_idx = 0;

    Label_Box_Height = 39;

    spellpicks_count = 0;

    if(spellrank_cnt > 0)
    {

        spellrank_idx = (spellrank_cnt - 1);

        /*
            BEGIN:  Common
        */
        if(m_select_count_common[spellrank_idx] < 10)
        {
            // p_start_spells[m_select_count_common[spellrank_idx]] = 0;
            // Warning	C6011	Dereferencing NULL pointer 'p_start_spells'. 	003_MoM	C:\STU\devel\ReMoM\MoM\src\NewGame.c	5505		
            if(p_start_spells)
            {
                p_start_spells[m_select_count_common[spellrank_idx]] = spl_NONE;
            }
        }
// ; copy the common spell selection to the display array,
// ; marking empty slots as -1s instead of 0s
        for(itr = 0; itr < 10; itr++)
        {
            // selected_spells[itr] = p_start_spells[itr];
            // Warning	C6011	Dereferencing NULL pointer 'p_start_spells'. 	003_MoM	C:\STU\devel\ReMoM\MoM\src\NewGame.c	5577		
            if(p_start_spells)
            {
                selected_spells[itr] = p_start_spells[itr];
            }
            if(selected_spells[itr] == spl_NONE)
            {
                selected_spells[itr] = ST_UNDEFINED;
            }
        }
        if(
            (m_select_count_common[spellrank_idx] > 0)
            &&
            (m_select_count_common[spellrank_idx] < 10)
        )
        {
            spellpicks_count += m_select_count_common[spellrank_idx];
            strcpy(section_title_string, rarity_strings[0]);
            strcat(section_title_string, cnst_Spell_Select_2);
            SDL_itoa(m_select_count_common[spellrank_idx], buffer, 10);
            strcat(section_title_string, buffer);
            Set_Font_Colors_15(3, &realm_colors[0]);
            Set_Font_Style_Shadow_Down(3, 15, ST_NULL, ST_NULL);
            Print(167, section_title_start_y[section_idx], section_title_string);
            FLIC_Draw(167, section_box_start_y[section_idx], m_shared_pict_segs[section_idx]);
            niu_selection_count = 0;
            for(itr = 0; itr < 2; itr++)
            {
                for(itr2 = 0; itr2 < 5; itr2++)
                {
                    spell_idx = ( 0 + (Selection_Realm * NUM_SPELLS_PER_MAGIC_REALM) + (itr * 5) + itr2 + 1);
                    Set_Font_Colors_15(0, &l_shadow_colors[0]);
                    Print_Far((176 + (74 * itr)), (Label_Box_Height + (7 * itr2) + 1), spell_data_table[spell_idx].name);
                    Display_Type = ST_UNDEFINED;
                    // ; if the spell is in the display array, remove it from
                    // ; there, change the display type, and increase the
                    // ; selection count
                    for(Selection_Index = 0; (m_select_count_common[spellrank_idx] > Selection_Index); Selection_Index++)
                    {
                        if(selected_spells[Selection_Index] == spell_idx)
                        {
                            niu_selection_count++;
                            Display_Type = 1;
                            selected_spells[Selection_Index] = ST_UNDEFINED;
                            Selection_Index = m_select_count_common[spellrank_idx];
                        }
                    }
                    if(Display_Type != ST_UNDEFINED)
                    {
                        Set_Font_Colors_15(0, &l_selected_colors[0]);
                        FLIC_Draw((169 + (74 * itr)), (Label_Box_Height + (itr2 * 7) + 1), _selection_marker_seg);
                        spellpicks_count--;
                    }
                    else
                    {
                        Set_Font_Colors_15(0, &l_deselected_colors[0]);
                    }
                    Print_Far((175 + (74 * itr)), (Label_Box_Height + (7 * itr2) + 1), spell_data_table[spell_idx].name);
                }
            }
            section_idx++;
        }
        /*
            END:  Common
        */
        /*
            BEGIN:  Uncommon
        */
        if(m_select_count_uncommon[spellrank_idx] < 2)
        {
            // p_start_spells[(10 + m_select_count_uncommon[spellrank_idx])] = 0;
            // Warning	C6011	Dereferencing NULL pointer 'p_start_spells'. 	003_MoM	C:\STU\devel\ReMoM\MoM\src\NewGame.c	5603		
            if(p_start_spells)
            {
                p_start_spells[(10 + m_select_count_uncommon[spellrank_idx])] = 0;
            }
        }
// ; copy the uncommon spell selection to the display
// ; array, marking empty slots as -1s instead of 0s
        for(itr = 0; itr < 2; itr++)
        {
            selected_spells[itr] = p_start_spells[(10 + itr)];
            if(selected_spells[itr] == 0)
            {
                selected_spells[itr] = ST_UNDEFINED;
            }
        }
        if(m_select_count_uncommon[spellrank_idx] > 0)
        {
// ; draw the uncommon spell selection box complete with
// ; the current selection highlights and tick marks
            spellpicks_count += m_select_count_uncommon[spellrank_idx];
            Label_Box_Height = (39 + (section_idx * 51));
            strcpy(section_title_string, rarity_strings[1]);
            strcat(section_title_string, cnst_Spell_Select_2);
            SDL_itoa(m_select_count_uncommon[spellrank_idx], buffer, 10);
            strcat(section_title_string, buffer);
            Set_Font_Colors_15(3, &realm_colors[0]);
            Set_Font_Style_Shadow_Down(3, 15, ST_NULL, ST_NULL);
            Print(167, section_title_start_y[section_idx], section_title_string);
            FLIC_Draw(167, section_title_start_y[section_idx], m_shared_pict_segs[section_idx]);
            niu_selection_count = 0;
            for(itr = 0; itr < 2; itr++)
            {
                for(itr2 = 0; itr2 < 5; itr2++)
                {
                    spell_idx = (10 + (Selection_Realm * NUM_SPELLS_PER_MAGIC_REALM) + (itr * 5) + itr2 + 1);
                    Set_Font_Colors_15(0, &l_shadow_colors[0]);
                    Print_Far((176 + (74 * itr)), (Label_Box_Height + (7 * itr2) + 1), spell_data_table[spell_idx].name);
                    Display_Type = ST_UNDEFINED;
                    // ; if the spell is in the display array, remove it from
                    // ; there, change the display type, and increase the
                    // ; selection count
                    for(Selection_Index = 0; (m_select_count_uncommon[spellrank_idx] > Selection_Index); Selection_Index++)
                    {
                        if(selected_spells[Selection_Index] == spell_idx)
                        {
                            niu_selection_count++;
                            Display_Type = 1;
                            selected_spells[Selection_Index] = ST_UNDEFINED;
                            Selection_Index = m_select_count_uncommon[spellrank_idx];
                        }
                    }
                    if(Display_Type != ST_UNDEFINED)
                    {
                        Set_Font_Colors_15(0, &l_selected_colors[0]);
                        FLIC_Draw((169 + (74 * itr)), (Label_Box_Height + (itr2 * 7) + 1), _selection_marker_seg);
                        spellpicks_count--;
                    }
                    else
                    {
                        Set_Font_Colors_15(0, &l_deselected_colors[0]);
                    }
                    Print_Far((175 + (74 * itr)), (Label_Box_Height + (7 * itr2) + 1), spell_data_table[spell_idx].name);
                }
            }
            section_idx++;
        }
        /*
            END:  Uncommon
        */
        /*
            BEGIN:  Rare
        */
        if(m_select_count_rare[spellrank_idx] < 1)
        {
            // p_start_spells[(12 + m_select_count_rare[spellrank_idx])] = 0;
            // Warning	C6011	Dereferencing NULL pointer 'p_start_spells'. 	003_MoM	C:\STU\devel\ReMoM\MoM\src\NewGame.c	5624		
            if(p_start_spells)
            {
                p_start_spells[(12 + m_select_count_rare[spellrank_idx])] = 0;
            }
        }
// ; copy the rare spell selection to the display array,
// ; marking empty slots as -1s instead of 0s
        for(itr = 0; itr < 1; itr++)
        {
            selected_spells[itr] = p_start_spells[(12 + itr)];
            if(selected_spells[itr] == 0)
            {
                selected_spells[itr] = ST_UNDEFINED;
            }
        }
        if(m_select_count_rare[spellrank_idx] > 0)
        {
// ; draw the rare spell selection box complete with
// ; the current selection highlights and tick marks
            spellpicks_count += m_select_count_rare[spellrank_idx];
            Label_Box_Height = (39 + (section_idx * 51));
            strcpy(section_title_string, rarity_strings[2]);
            strcat(section_title_string, cnst_Spell_Select_2);
            SDL_itoa(m_select_count_rare[spellrank_idx], buffer, 10);
            strcat(section_title_string, buffer);
            Set_Font_Colors_15(3, &realm_colors[0]);
            Set_Font_Style_Shadow_Down(3, 15, ST_NULL, ST_NULL);
            Print(167, section_title_start_y[section_idx], section_title_string);
            FLIC_Draw(167, section_title_start_y[section_idx], m_shared_pict_segs[section_idx]);
            niu_selection_count = 0;
            for(itr = 0; itr < 2; itr++)
            {
                for(itr2 = 0; itr2 < 5; itr2++)
                {
                    spell_idx = (20 + (Selection_Realm * NUM_SPELLS_PER_MAGIC_REALM) + (itr * 5) + itr2 + 1);
                    Set_Font_Colors_15(0, &l_shadow_colors[0]);
                    Print_Far((176 + (74 * itr)), (Label_Box_Height + (7 * itr2) + 1), spell_data_table[spell_idx].name);
                    Display_Type = ST_UNDEFINED;
                    // ; if the spell is in the display array, remove it from
                    // ; there, change the display type, and increase the
                    // ; selection count
                    for(Selection_Index = 0; (m_select_count_rare[spellrank_idx] > Selection_Index); Selection_Index++)
                    {
                        if(selected_spells[Selection_Index] == spell_idx)
                        {
                            niu_selection_count++;
                            Display_Type = 1;
                            selected_spells[Selection_Index] = ST_UNDEFINED;
                            Selection_Index = m_select_count_rare[spellrank_idx];
                        }
                    }
                    if(Display_Type != ST_UNDEFINED)
                    {
                        Set_Font_Colors_15(0, &l_selected_colors[0]);
                        FLIC_Draw((169 + (74 * itr)), (Label_Box_Height + (itr2 * 7) + 1), _selection_marker_seg);
                        spellpicks_count--;
                    }
                    else
                    {
                        Set_Font_Colors_15(0, &l_deselected_colors[0]);
                    }
                    Print_Far((175 + (74 * itr)), (Label_Box_Height + (7 * itr2) + 1), spell_data_table[spell_idx].name);
                }
            }
            section_idx++;
        }
        /*
            END:  Rare
        */
    }

}


// MGC  o50p25
/**
 * @brief Copies a preset wizard profile into the human player record.
 *
 * @details
 * Applies the selected preset wizard's identity, spell ranks, special ability,
 * default name, and starting spell list to player slot 0.
 *
 * Processing flow:
 * 1) Sets `_players[0].wizard_id` from `wizard_id`.
 * 2) Copies preset realm book counts (`nature`, `sorcery`, `chaos`, `life`,
 *    `death`) into `_players[0].spellranks[]`.
 * 3) Rebuilds retort flags in `_players[0].alchemy` so only the preset's
 *    `special` retort index is enabled.
 * 4) Clears all 5 realm start-spell arrays for player 0.
 * 5) For each realm with `spellranks > 1`, copies default common/uncommon spell
 *    entries from `_default_spells` using `m_select_count_common/U` thresholds.
 * 6) Copies preset wizard name into `_players[0].name`.
 *
 * @param wizard_id Preset wizard index into `_wizard_presets_table`.
 *
 * @return void
 *
 * @note This routine overwrites existing player-0 wizard customization state,
 *       including name, retorts, ranks, and starting spell selections.
 * @note Assumes `wizard_id` is a valid preset index and global spell tables are
 *       initialized.
 */
void Human_Player_Wizard_Profile(int16_t wizard_id)
{
    int8_t * wsa_ptr = 0;
    int16_t itr = 0;  // _SI_
    int16_t spellranks = 0;  // _DI_

    _players[0].wizard_id = (uint8_t)wizard_id;

    _players[0].spellranks[sbr_Nature] = _wizard_presets_table[wizard_id].nature;
    _players[0].spellranks[sbr_Sorcery]= _wizard_presets_table[wizard_id].sorcery;
    _players[0].spellranks[sbr_Chaos] = _wizard_presets_table[wizard_id].chaos;
    _players[0].spellranks[sbr_Life] = _wizard_presets_table[wizard_id].life;
    _players[0].spellranks[sbr_Death] = _wizard_presets_table[wizard_id].death;

    wsa_ptr = &_players[0].alchemy;

    for(itr = 0; itr < NUM_RETORTS; itr++)
    {

        if(_wizard_presets_table[wizard_id].special == itr)
        {
            wsa_ptr[itr] = 1;
        }
        else
        {
            wsa_ptr[itr] = 0;
        }

    }

    for(itr = 0; itr < 13; itr++)
    {
        _player_start_spells[0].realms[sbr_Nature].spells[itr] = 0;
        _player_start_spells[0].realms[sbr_Sorcery].spells[itr] = 0;
        _player_start_spells[0].realms[sbr_Chaos].spells[itr] = 0;
        _player_start_spells[0].realms[sbr_Life].spells[itr] = 0;
        _player_start_spells[0].realms[sbr_Death].spells[itr] = 0;
    }

    spellranks = _players[0].spellranks[sbr_Nature];
    if(spellranks > 1)
    {
        for(itr = 0; (m_select_count_common[spellranks] - 2) > itr; itr++)
        {
            _player_start_spells[0].realms[sbr_Nature].spells[( 0 + itr)] = _default_spells[sbr_Nature].spells[(0 + itr)];
        }
        for(itr = 0; (m_select_count_uncommon[spellranks] - 2) > itr; itr++)
        {
            _player_start_spells[0].realms[sbr_Nature].spells[(10 + itr)] = _default_spells[sbr_Nature].spells[(10 + itr)];
        }
        if((m_select_count_uncommon[spellranks] - 2) > 0)
        {
            _player_start_spells[0].realms[sbr_Nature].spells[(12 + itr)] = _default_spells[sbr_Nature].spells[(12 + itr)];
        }
    }

    spellranks = _players[0].spellranks[sbr_Sorcery];
    if(spellranks > 1)
    {
        for(itr = 0; (m_select_count_common[spellranks] - 2) > itr; itr++)
        {
            _player_start_spells[0].realms[sbr_Sorcery].spells[( 0 + itr)] = _default_spells[sbr_Sorcery].spells[(0 + itr)];
        }
        for(itr = 0; (m_select_count_uncommon[spellranks] - 2) > itr; itr++)
        {
            _player_start_spells[0].realms[sbr_Sorcery].spells[(10 + itr)] = _default_spells[sbr_Sorcery].spells[(10 + itr)];
        }
        if((m_select_count_uncommon[spellranks] - 2) > 0)
        {
            _player_start_spells[0].realms[sbr_Sorcery].spells[(12 + itr)] = _default_spells[sbr_Sorcery].spells[(12 + itr)];
        }
    }

    spellranks = _players[0].spellranks[sbr_Chaos];
    if(spellranks > 1)
    {
        for(itr = 0; (m_select_count_common[spellranks] - 2) > itr; itr++)
        {
            _player_start_spells[0].realms[sbr_Chaos].spells[( 0 + itr)] = _default_spells[sbr_Chaos].spells[(0 + itr)];
        }
        for(itr = 0; (m_select_count_uncommon[spellranks] - 2) > itr; itr++)
        {
            _player_start_spells[0].realms[sbr_Chaos].spells[(10 + itr)] = _default_spells[sbr_Chaos].spells[(10 + itr)];
        }
        if((m_select_count_uncommon[spellranks] - 2) > 0)
        {
            _player_start_spells[0].realms[sbr_Chaos].spells[(12 + itr)] = _default_spells[sbr_Chaos].spells[(12 + itr)];
        }
    }

    spellranks = _players[0].spellranks[sbr_Life];
    if(spellranks > 1)
    {
        for(itr = 0; (m_select_count_common[spellranks] - 2) > itr; itr++)
        {
            _player_start_spells[0].realms[sbr_Life].spells[( 0 + itr)] = _default_spells[sbr_Life].spells[(0 + itr)];
        }
        for(itr = 0; (m_select_count_uncommon[spellranks] - 2) > itr; itr++)
        {
            _player_start_spells[0].realms[sbr_Life].spells[(10 + itr)] = _default_spells[sbr_Life].spells[(10 + itr)];
        }
        if((m_select_count_uncommon[spellranks] - 2) > 0)
        {
            _player_start_spells[0].realms[sbr_Life].spells[(12 + itr)] = _default_spells[sbr_Life].spells[(12 + itr)];
        }
    }

    spellranks = _players[0].spellranks[sbr_Death];
    if(spellranks > 1)
    {
        for(itr = 0; (m_select_count_common[spellranks] - 2) > itr; itr++)
        {
            _player_start_spells[0].realms[sbr_Death].spells[( 0 + itr)] = _default_spells[sbr_Death].spells[(0 + itr)];
        }
        for(itr = 0; (m_select_count_uncommon[spellranks] - 2) > itr; itr++)
        {
            _player_start_spells[0].realms[sbr_Death].spells[(10 + itr)] = _default_spells[sbr_Death].spells[(10 + itr)];
        }
        if((m_select_count_uncommon[spellranks] - 2) > 0)
        {
            _player_start_spells[0].realms[sbr_Death].spells[(12 + itr)] = _default_spells[sbr_Death].spells[(12 + itr)];
        }
    }

    strcpy(_players[0].name, _wizard_presets_table[wizard_id].name);

}


// MGC  o50p26
// Fade_Out()

// MGC  o50p27
// VGA_Fade_In()

// MGC  o50p28
// Set_Load_Screen_Help_List_MGC()

// MGC  o50p29
// drake178: HLP_Load_NewGOptions()
/*
loads and sets the GUI help entry area array for the
new game options selection screen
*/
/*

¿ Set_Options_Screen_Help_List ?

*/
void Set_Newgame_Screen_0_Help_List(void)
{

    // HLPENTRY.LBX, 29  Options Screen Help
    LBX_Load_Data_Static(hlpentry_lbx_file__MGC_ovr050, 29, (SAMB_ptr)_help_entries, 0, 6, 10);

    Set_Help_List(_help_entries, 6);

}


// MGC  o50p30
// HLP_Load_BannerSel()
/*

*/
void Set_Newgame_Screen_7_Help_List(void)
{

    // HLPENTRY.LBX, 030  ""  "Banner Selection Help"
    LBX_Load_Data_Static(hlpentry_lbx_file__MGC_ovr050, 30, (SAMB_ptr)_help_entries, 0, 1, 10);

    Set_Help_List(_help_entries, 1);

}

// MGC  o50p31
// HLP_Load_PortraitSel()
/* 
; loads and sets the GUI help entry area array for the
; wizard portrait selection screen
*/
void Set_Newgame_Screen_2_Help_List(void)
{

    // HLPENTRY.LBX, 031  ""  "Wizard Picture Select"
    LBX_Load_Data_Static(hlpentry_lbx_file__MGC_ovr050, 31, (SAMB_ptr)_help_entries, 0, 1, 10);

    Set_Help_List(_help_entries, 1);

}


// MGC  o50p32
/*
; loads and sets the GUI help entry area array for the
; wizard selection screen
*/
/*

*/
void Set_Newgame_Screen_1_Help_List(int16_t has_custom)
{

    // HLPENTRY.LBX, 032  ""  "Wizard Selection Help"
    LBX_Load_Data_Static(hlpentry_lbx_file__MGC_ovr050, 32, (SAMB_ptr)_help_entries, 0, 15, 10);

    if(has_custom != ST_FALSE)
    {
        _help_entries[14].help_idx = HLP_CUSTOM;
    }

    Set_Help_List(_help_entries, 15);

}


// MGC  o50p33
void Set_Newgame_Screen_4_Help_List(void)
{

    // HLPENTRY.LBX, 033  ""  "Wizard Creation Help"
    LBX_Load_Data_Static(hlpentry_lbx_file__MGC_ovr050, 33, (SAMB_ptr)_help_entries, 0, 25, 10);

    Set_Help_List(_help_entries, 25);

}

// MGC  o50p34
void Set_Newgame_Screen_6_Help_List(void)
{

    // HLPENTRY.LBX, 035  ""  "Wizard Race Help"
    LBX_Load_Data_Static(hlpentry_lbx_file__MGC_ovr050, 35, (SAMB_ptr)_help_entries, 0, 14, 10);

    Set_Help_List(_help_entries, 14);

}

// MGC  o50p35
/**
 * @brief Appends a human-readable separator before the next list item.
 *
 * @details
 * Increments the running item counter and appends either a comma separator or
 * the final "and" separator into `Dest`, depending on whether the current item
 * is the last element in the list.
 *
 * Behavior:
 * - Increments `*List_Size` by 1.
 * - If `*List_Size == Total` and there is more than one item, appends
 *   `cnst_And__ovr050`.
 * - If `*List_Size < Total` and there is more than one item, appends
 *   `cnst_Pick_Error_29` (comma-style list separator).
 * - Appends nothing for the first item.
 *
 * @param List_Size Pointer to running count of items already processed; updated
 *                  in place.
 * @param Total Total number of items expected in the final formatted list.
 * @param Dest Destination C string buffer receiving the separator text via
 *             `strcat`.
 *
 * @return void
 *
 * @note `Dest` must reference a writable, NUL-terminated buffer with enough
 *       remaining capacity for appended separator text.
 */
void String_List_Builer(int16_t * List_Size, int16_t Total, char * Dest)
{

    *List_Size += 1;

    if(*List_Size == Total)
    {
        if(*List_Size > 1)
        {
            strcat(Dest, &cnst_And__ovr050[0]);
        }

    }
    else
    {
        if(*List_Size > 1)
        {
            strcat(Dest, &cnst_Pick_Error_29[0]);
        }

    }

}


// MGC  o50p36
// CRP_Empty_Dialog_Fn2()

// MGC  o50p37
// CRP_Empty_Dialog_Fn1()

// MGC  o50p38
// Do_Toggle_Pages()
