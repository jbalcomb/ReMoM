/*

    MAGIC.EXE
        ovr050
*/

#include "../../MoX/src/MOX_SET.h"
#include "../../MoX/src/Graphics.h"
#include "../../MoX/src/LOADSAVE.h"
#include "../../MoX/src/MOX_T4.h"
#include "../../MoX/src/random.h"
#include "../../MoX/src/LBX_Load.h"
#include "../../MoX/src/FLIC_Draw.h"
#include "../../MoX/src/Fields.h"
#include "../../MoX/src/DOS.h"
#include "../../MoX/src/Fonts.h"
#include "../../MoX/src/Help.h"
#include "../../MoX/src/Mouse.h"
#include "../../MoX/src/MOX_DAT.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_SET.h"
#include "../../MoX/src/MOX_TYPE.h"
#include "../../MoX/src/MOM_Data.h"  /* _difficulty, _magic, _landsize, _num_players */
#include "../../MoX/src/Timer.h"

#include "Settings.h"
#include "Spellbook.h"

#include <stdio.h>      /* FILE; fclose(), fopen(), fread(), frite(), fseek(); */
#include <string.h>

#include "NewGame.h"



// MGC dseg:1F96 34 27 3C 27 44 27 52 27 61 27 71 27 80 27 8D 27+
// STR_Retorts@ dw offset cnst_Alchemy, offset cnst_Warlord, offset cnst_ChaosMastery, offset cnst_NatureMastery, offset cnst_SorceryMastery, offset cnst_InfernalPower, offset cnst_DivinePower, offset cnst_SageMaster, offset cnst_Channeler, offset cnst_Myrran, offset cnst_Archmage, offset cnst_ManaFocusing, offset cnst_NodeMastery, offset cnst_Famous, offset cnst_Runemaster, offset cnst_Conjurer, offset cnst_Charismatic, offset cnst_Artificer
// MGC dseg:2734 41 6C 63 68 65 6D 79 00                         cnst_Alchemy db 'Alchemy',0             
// MGC dseg:273C 57 61 72 6C 6F 72 64 00                         cnst_Warlord db 'Warlord',0             
// MGC dseg:2744 43 68 61 6F 73 20 4D 61 73 74 65 72 79 00       cnst_ChaosMastery db 'Chaos Mastery',0  
// MGC dseg:2752 4E 61 74 75 72 65 20 4D 61 73 74 65 72 79 00    cnst_NatureMastery db 'Nature Mastery',0
// MGC dseg:2761 53 6F 72 63 65 72 79 20 4D 61 73 74 65 72 79 00 cnst_SorceryMastery db 'Sorcery Mastery',0
// MGC dseg:2771 49 6E 66 65 72 6E 61 6C 20 50 6F 77 65 72 00    cnst_InfernalPower db 'Infernal Power',0
// MGC dseg:2780 44 69 76 69 6E 65 20 50 6F 77 65 72 00          cnst_DivinePower db 'Divine Power',0    
// MGC dseg:278D 53 61 67 65 20 4D 61 73 74 65 72 00             cnst_SageMaster db 'Sage Master',0      
// MGC dseg:2799 43 68 61 6E 6E 65 6C 65 72 00                   cnst_Channeler db 'Channeler',0         
// MGC dseg:27A3 4D 79 72 72 61 6E 00                            cnst_Myrran db 'Myrran',0               
// MGC dseg:27AA 41 72 63 68 6D 61 67 65 00                      cnst_Archmage db 'Archmage',0           
// MGC dseg:27B3 4D 61 6E 61 20 46 6F 63 75 73 69 6E 67 00       cnst_ManaFocusing db 'Mana Focusing',0  
// MGC dseg:27C1 4E 6F 64 65 20 4D 61 73 74 65 72 79 00          cnst_NodeMastery db 'Node Mastery',0    
// MGC dseg:27CE 46 61 6D 6F 75 73 00                            cnst_Famous db 'Famous',0               
// MGC dseg:27D5 52 75 6E 65 6D 61 73 74 65 72 00                cnst_Runemaster db 'Runemaster',0       
// MGC dseg:27E0 43 6F 6E 6A 75 72 65 72 00                      cnst_Conjurer db 'Conjurer',0           
// MGC dseg:27E9 43 68 61 72 69 73 6D 61 74 69 63 00             cnst_Charismatic db 'Charismatic',0     
// MGC dseg:27F5 41 72 74 69 66 69 63 65 72 00                   cnst_Artificer db 'Artificer',0       

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

char * STR_Retorts[NUM_RETORTS] =
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
void NEWG_CreateWorld__WIP(void);
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





// MGC dseg:354C 00 00 01 00 01 00 01 00 05 00 02 00 01 00 02 00+TBL_AI_Realm_PER AI_PERS_Modifiers <0, 1, 1, 1, 5, 2>    ; 0
// MGC dseg:354C 03 00 01 00 02 00 01 00 01 00 02 00 02 00 05 00+                                        ; DATA XREF: WIZ_SetPersonalities__WIP+2A7r
// MGC dseg:354C 00 00 00 00 00 00 01 00 02 00 00 00 02 00 05 00+AI_PERS_Modifiers <1, 2, 3, 1, 2, 1>    ; 1
// MGC dseg:354C 03 00 03 00 02 00 01 00 01 00 00 00 00 00 00 00+AI_PERS_Modifiers <1, 2, 2, 5, 0, 0>    ; 2
// MGC dseg:354C 00 00 00 00 00 00 00 00                         AI_PERS_Modifiers <0, 1, 2, 0, 2, 5>    ; 3
// MGC dseg:354C                                                 AI_PERS_Modifiers <3, 3, 2, 1, 1, 0>    ; 4
// MGC dseg:354C                                                 AI_PERS_Modifiers  <0>
// MGC dseg:3594 00 00 02 00 02 00 04 00 02 00 00 00 01 00 04 00+TBL_AI_Realm_OBJ AI_OBJ_Modifiers <0, 2, 2, 4, 2>        ; 0
// MGC dseg:3594 02 00 03 00 00 00 03 00 02 00 02 00 03 00 00 00+                                        ; DATA XREF: WIZ_SetPersonalities__WIP+2D0r
// MGC dseg:3594 02 00 03 00 04 00 01 00 00 00 04 00 01 00 01 00+AI_OBJ_Modifiers <0, 1, 4, 2, 3>        ; 1
// MGC dseg:3594 04 00 00 00 00 00 00 00 00 00 00 00             AI_OBJ_Modifiers <0, 3, 2, 2, 3>        ; 2
// MGC dseg:3594                                                 AI_OBJ_Modifiers <0, 2, 3, 4, 1>        ; 3
// MGC dseg:3594                                                 AI_OBJ_Modifiers <0, 4, 1, 1, 4>        ; 4
// MGC dseg:3594                                                 AI_OBJ_Modifiers  <0>
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


// MGC dseg:52C0
int16_t _quit_button = 0;

// MGC dseg:5CB0
SAMB_ptr newgame_ok_button_seg = 0;

// MGC dseg:64AE
int16_t screen_changed = 0;

// MGC dseg:8E8E
SAMB_ptr _quit_active_seg = 0;

// MGC dseg:8E92
int16_t _ok_button = 0;




// MGC dseg:52B2                                                 ? BEGIN: Main Menu Screen ?
// MGC dseg:52B2
// MGC dseg:52B2                                                 ; char *GUI_String_2
// MGC dseg:52B2 00 00                                           GUI_String_2 dw 0                       ; DATA XREF: start+C7o ...
// MGC dseg:52B2                                                                                         ; _bdata_start
// MGC dseg:52B2                                                                                         ; 100 LBX_NearAlloc_Next bytes
// MGC dseg:52B4                                                 ; char *GUI_String_1
// MGC dseg:52B4 00 00                                           GUI_String_1 dw 0                       ; DATA XREF: Hall_Of_Fame_Screen+8Dw ...
// MGC dseg:52B4                                                                                         ; 100 LBX_NearAlloc_First bytes
// MGC dseg:52B6 00 00                                           _load_button dw 0                       ; DATA XREF: Main_Menu_Screen+19Fw ...
// MGC dseg:52B8 00 00                                           _background_seg dw 0                    ; DATA XREF: Load_Screen+1Bw ...
SAMB_ptr newgame_background_seg = 0;
// MGC dseg:52BA 00 00                                           menu_quit_button dw 0                   ; DATA XREF: Load_MGC_Resources+76w ...
// MGC dseg:52BA                                                                                         ; single-loaded 2-frame image
// MGC dseg:52BC 00 00                                           _halloffame_button dw 0                 ; DATA XREF: Main_Menu_Screen+29Ew ...
// MGC dseg:52BE 00 00                                           menu_hof_button dw 0                    ; DATA XREF: Load_MGC_Resources+40w ...
// MGC dseg:52BE                                                                                         ; single-loaded 2-frame image
// MGC dseg:52C0 00 00                                           _quit_button dw 0                       ; DATA XREF: Main_Menu_Screen+2DFw ...
// MGC dseg:52C2 00 00                                           menu_load_button dw 0                   ; DATA XREF: Load_MGC_Resources+52w ...

// MGC dseg:52C4 00 00                                           current_menu_screen dw 0                ; DATA XREF: Main_Menu_Screen+32w ...
int16_t auto_input_field_idx = 0;

// MGC dseg:52C4                                                                                         ; 0 - continue, 1 -load, 2 - new, 3 - quit, 4 - HoF
// MGC dseg:52C4                                                                                         ; holds the mouse focus control instead during the
// MGC dseg:52C4                                                                                         ;  redraw functions of other screens
// MGC dseg:52C6 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+wizard_portrait_segs dw 14 dup(    0)   ; DATA XREF: Load_MGC_Resources+A9w ...
// MGC dseg:52C6 00 00 00 00 00 00 00 00 00 00 00 00                                                     ; array of 14 single-loaded images
// MGC dseg:52E2 00 00                                           IMG_NewG_BtnBorder@ dw 0                ; used by the map customizer and profile reloader
// MGC dseg:52E4 00 00                                           load_flag dw 0                          ; DATA XREF: Main_Menu_Screen+38w ...
// MGC dseg:52E4                                                                                         ; OXRB:Menu,MenuDraw.:.static?module-scoped?private?
// MGC dseg:52E4                                                                                         ; Da-Funk? "OXRB"? "only x-ref'd by"? if so, nowadays, use "OON XREF"
// MGC dseg:52E6 00 00                                           NEWG_HaveLastProfile dw 0               ; used by the profile reloader
// MGC dseg:52E8 00 00                                           mainmenu_top dw 0                       ; DATA XREF: Load_MGC_Resources+1Cw ...
// MGC dseg:52E8                                                                                         ; single-loaded 20 frame animation
// MGC dseg:52E8                                                                                         ; includes the background for the top of the screen too
// MGC dseg:52EA 00 00                                           _newgame_button dw 0                    ; DATA XREF: Main_Menu_Screen+25Dw ...
// MGC dseg:52EC 00 00                                           menu_new_button dw 0                    ; DATA XREF: Load_MGC_Resources+64w ...
// MGC dseg:52EC                                                                                         ; single-loaded 2-frame image
// MGC dseg:52EE 00 00                                           _continue_button dw 0                   ; DATA XREF: Main_Menu_Screen+204w ...
// MGC dseg:52F0 00 00                                           menu_continue_button dw 0               ; DATA XREF: Load_MGC_Resources+2Ew ...
// MGC dseg:52F0                                                                                         ; single-loaded 2-frame image
// MGC dseg:52F2 00 00                                           cont_flag dw 0                          ; DATA XREF: Main_Menu_Screen+2Cw ...
// MGC dseg:52F2                                                                                         ; OXRB:Menu,MenuDraw.:.static?module-scoped?private?
// MGC dseg:52F4 00 00                                           mainmenu_bottom dw 0                    ; DATA XREF: Load_MGC_Resources+88w ...
// MGC dseg:52F4                                                                                         ; single-loaded image
// MGC dseg:52F6 00 00                                           mainmenu_bot dw 0                       ; used by the map customizer to indicate which screen
// MGC dseg:52F6                                                                                         ; is currently active (basic/patch/world/world2)
// MGC dseg:52F8 00 00                                           _load_inactive_seg dw 0                 ; DATA XREF: Load_Screen+60w ...
// MGC dseg:52FA 00 00                                           NEWG_LastRetorts@ dw 0                  ; used by the profile reloader
// MGC dseg:52FC 00 00                                           g_GUI_MainScreenJump dw 0               ; DATA XREF: Menu_Screen_Control+5Aw ...
// MGC dseg:52FE 00 00                                           save_game_count dw 0                    ; DATA XREF: Main_Menu_Screen+C6w ...
// MGC dseg:5300 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 save_game_slots dw 8 dup(     0)        ; DATA XREF: Main_Menu_Screen+137w ...
// MGC dseg:5300
// MGC dseg:5300                                                 ? END: Main Menu Screen ?
// MGC dseg:5300
// MGC dseg:5310
// MGC dseg:5310                                                 ? Begin: New Game Screen ?
// MGC dseg:5310
// MGC dseg:5310 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+NEWG_WorldgenSet dw 40h dup(0)          ; extended worldgen settings are loaded here by the
// MGC dseg:5310 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; worldgen customizer
// MGC dseg:5390 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+NEWG_LastProfile db 0B6h dup(0)         ; the last saved wizard profile is loaded here by
// MGC dseg:5390 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; the profile reloader
// MGC dseg:5446 00 00                                           IMG_NewG_ValueBox@ dw 0                 ; NEWGAME.LBX entry 51, used by the world generator
// MGC dseg:5446                                                                                         ; customizer to display the setting values
// MGC dseg:5446                                                                                         ; (number, On/Off, Low/Norm/High, or Rand)
// MGC dseg:5448 00 00                                           IMG_NewG_CheckMark@ dw 0
// MGC dseg:544A 00 00                                           IMG_NewG_ScrollUp@ dw 0
// MGC dseg:544C 00 00                                           IMG_NewG_ScrollDown@ dw 0
// MGC dseg:544E 00 00                                           NEWG_WGP_OK_Label dw 0
// MGC dseg:5450 00 00                                           NEWG_WGP_Reset_Lbl dw 0
// MGC dseg:5452 00 00                                           NEWG_WGP_ScrollUp dw 0
// MGC dseg:5454 00 00                                           NEWG_WGP_ScrollDn dw 0
// MGC dseg:5456 00 00                                           NEWG_FirstPatchSet dw 0
// MGC dseg:5458 00 00                                           NEWG_FirstWorldSet dw 0
// MGC dseg:545A 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+NEWG_WGP_SetCtrls dw 24h dup(0)
// MGC dseg:54A2 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+NEWG_WGPSliderStates dw 12h dup(0)
// MGC dseg:54C6 00 00                                           NEWG_ConnMap_Seg dw 0                   ; a 9600 byte buffer in GFX_Swap_Seg
// MGC dseg:54C8 00                                              NEWG_CustomWorld db 0
// MGC dseg:54C9 00                                              NEWG_CustomPlanes db 0
// MGC dseg:54CA 00                                              NEWG_SliderDragged db 0
// MGC dseg:54CB 00                                              align 2
// MGC dseg:54CC 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+UU_Some_AI_Array db 54Eh dup(0)
// MGC dseg:5A1A 00 00                                           Wiz5_Spell_E0h@ dw 0                    ; DATA XREF: NEWG_FinalizeTables+95r ...
// MGC dseg:5A1A                                                                                         ; 80 + rnd(40) is stored here for each AI wizard (word
// MGC dseg:5A1A                                                                                         ; array pointer, human excluded) when setting initial
// MGC dseg:5A1A                                                                                         ; gold
// MGC dseg:5A1C 00 00                                           Wiz5_Spell_C8h@ dw 0                    ; DATA XREF: Allocate_Data_Space+3F6w
// MGC dseg:5A1E 00 00                                           Wiz5_Spell_D4h@ dw 0                    ; DATA XREF: Allocate_Data_Space+3FCw
// MGC dseg:5A20 00 00                                           Wiz5_Spell_A0h@ dw 0                    ; DATA XREF: Allocate_Data_Space+3EAw
// MGC dseg:5A22 00 00                                           Wiz5_Spell_B4h@ dw 0                    ; DATA XREF: Allocate_Data_Space+3F0w
// MGC dseg:5A24 00 00                                           Wiz5_Spell_50h@ dw 0                    ; DATA XREF: Allocate_Data_Space+3D2w
// MGC dseg:5A26 00 00                                           Wiz5_Spell_64h@ dw 0                    ; DATA XREF: Allocate_Data_Space+3D8w
// MGC dseg:5A28 00 00                                           Wiz5_Spell_28h@ dw 0                    ; DATA XREF: Allocate_Data_Space+3C6w
// MGC dseg:5A2A 00 00                                           Wiz5_Spell_3Ch@ dw 0                    ; DATA XREF: Allocate_Data_Space+3CCw
// MGC dseg:5A2C 00 00                                           Wiz5_Spell_18h@ dw 0                    ; DATA XREF: Allocate_Data_Space+3C0w
// MGC dseg:5A2E 00 00                                           Wiz5_Spell_78h@ dw 0                    ; DATA XREF: Allocate_Data_Space+3DEw
// MGC dseg:5A30 00 00                                           Wiz5_Spell_8Ch@ dw 0                    ; DATA XREF: Allocate_Data_Space+3E4w
// MGC dseg:5A32 00 00                                           Wiz5_Spell_00h@ dw 0                    ; DATA XREF: Allocate_Data_Space+3B4w
// MGC dseg:5A34 00 00                                           Wiz5_Spell_0Ch@ dw 0                    ; DATA XREF: Allocate_Data_Space+3BAw
// MGC dseg:5A36 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+G_UU_Some_AI_Array_2 db 176h dup(0)
// MGC dseg:5BAC 00 00 00 00                                     terrain_stats_table dd 0                ; DATA XREF: Load_TERRSTAT+28w ...





// MGC dseg:2ABC                                                 BEGIN:  ovr050 - Initialized Data

// MGC dseg:2ABC
struct s_mouse_list mouse_list_newgame_0_1[1] = {
    { crsr_Finger, 0, SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX }
};

// MGC dseg:2AC8 1F 1E 1D 1C 1B 1A 19 18                         COL_NEWG_NameEdit db 31, 30, 29, 28, 27, 26, 25, 24

// MGC dseg:2AD0
struct WIZARD_Preset TBL_Default_Wizards[15] = {
    {"Merlin",  5,  0,  5, 0,  0, _Sage_Master },
    {"Raven",   0,  6,  5, 0,  0, _No_Retort},
    {"Sharee",  0,  0,  0, 5,  5, _Conjurer},
    {"Lo Pan",  0,  5,  0, 0,  5, _Channeller},
    {"Jafar",   0, 10,  0, 0,  0, _Alchemy},
    {"Oberic",  0,  0,  5, 0,  5, _Mana_Focusing},
    {"Rjak",    0,  0,  0, 9,  0, _Infernal_Power},
    {"Sss'ra",  4,  0,  0, 0,  4, _Myrran},
    {"Tauron",  0,  0,  0, 0, 10, _Chaos_Mastery},
    {"Freya",   0,  0, 10, 0,  0, _Nature_Mastery},
    {"Horus",   5,  5,  0, 0,  0, _Archmage},
    {"Ariel",  10,  0,  0, 0,  0, _Charismatic},
    {"Tlaloc",  0,  0,  4, 5,  0, _Warlord},
    {"Kali",    0,  5,  0, 5,  0, _Artificer},
    {"Custom",  0,  0,  0, 0,  0, _No_Retort}
};

// MGC dseg:2C1A 02 00 03 00 04 00 05 00 06 00 07 00 08 00 09 00+RP_Book_Table dw 2, 3, 4, 5, 6, 7, 8, 9, 0Ah, 0Bh ; repurposed in the worldgen customizer
// MGC dseg:2C2E
int16_t TBL_SpellsPerBook_C[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
// MGC dseg:2C42
int16_t TBL_SpellsPerBook_U[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 };
// MGC dseg:2C56
int16_t TBL_SpellsPerBook_R[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };
// MGC dseg:2C6A
// Statically define a variable named 'player_spells'
struct Default_Spells player_spells = {
    {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},  // Common spells initialization
    {20, 21},                         // Uncommon spells initialization
    50                                // Rare spell initialization
};
struct Default_Spells TBL_Def_Spl_Nature = {
    { spl_War_Bears, spl_Stone_Skin, spl_Sprites, spl_Water_Walking, spl_Giant_Strength, spl_Web, spl_Earth_To_Mud, spl_Wall_Of_Stone, spl_Resist_Elements, spl_Earth_Lore }, 
    { spl_Cockatrices, spl_Change_Terrain },
    spl_Gorgons
};
// MGC dseg:2C84
struct Default_Spells TBL_Def_Spl_Sorcery = {
    { spl_Nagas, spl_Psionic_Blast, spl_Phantom_Warriors, spl_Floating_Island, spl_Confusion, spl_Counter_Magic, spl_Word_Of_Recall, spl_Dispel_Magic_True, spl_Resist_Magic, spl_Guardian_Wind },
    { spl_Flight, spl_Phantom_Beast },
    spl_Storm_Giant
};
// MGC dseg:2C9E
struct Default_Spells TBL_Def_Spl_Chaos = {
    { spl_Fire_Bolt, spl_Fire_Elemental, spl_Eldritch_Weapon, spl_Hell_Hounds, spl_Corruption, spl_Warp_Creature, spl_Shatter, spl_Wall_Of_Fire, spl_Disrupt, spl_Warp_Wood },
    { spl_Lightning_Bolt, spl_Doom_Bat },
    spl_Efreet
};
// MGC dseg:2CB8
struct Default_Spells TBL_Def_Spl_Life = {
    { spl_Heroism, spl_Guardian_Spirit, spl_Holy_Armor, spl_Just_Cause, spl_Healing, spl_Holy_Weapon, spl_Star_Fires, spl_Endurance, spl_True_Light, spl_Bless },
    { spl_Resurrection, spl_Unicorns },
    spl_Angel
};
// MGC dseg:2CD2
struct Default_Spells TBL_Def_Spl_Death = {
{ spl_Life_Drain, spl_Ghouls, spl_Weakness, spl_Dark_Rituals, spl_Black_Sleep, spl_Darkness, spl_Terror, spl_Skeletons, spl_Mana_Leak, spl_Cloak_Of_Fear},
{ spl_Black_Prayer, spl_Black_Channels },
spl_Wraiths
};

// MGC dseg:2CEC 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+TBL_Retort_Reqs Retort_Req 2 dup(<0, 0, 0, 0, 0, 0, 0>)
// MGC dseg:2CEC 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00+                                        ; DATA XREF: GAME_New_Screen_4+167o ...
// MGC dseg:2CEC 00 00 00 00 04 00 00 00 00 00 00 00 01 00 00 00+Retort_Req <0, 1, 0, 0, 4, 0, 0>
// MGC dseg:2CEC 00 00 00 00 04 00 00 00 00 00 01 00 00 00 00 00+Retort_Req <0, 1, 0, 0, 0, 4, 0>
// MGC dseg:2CEC 00 00 00 00 04 00 00 00 01 00 00 00 04 00 00 00+Retort_Req <0, 1, 0, 0, 0, 0, 4>
// MGC dseg:2CEC 00 00 00 00 00 00 01 00 04 00 00 00 00 00 00 00+Retort_Req <0, 1, 0, 4, 0, 0, 0>
// MGC dseg:2CEC 00 00 01 00 02 00 00 00 00 00 00 00 00 00 00 00+Retort_Req <0, 1, 4, 0, 0, 0, 0>
// MGC dseg:2CEC 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+Retort_Req <1, 2, 0, 0, 0, 0, 0>
// MGC dseg:2CEC 00 00 00 00 00 00 00 00 00 00 00 00 04 00 01 00+Retort_Req 2 dup(<0, 0, 0, 0, 0, 0, 0>)
// MGC dseg:2CEC 00 00 00 00 00 00 00 00 00 00 04 00 01 00 00 00+Retort_Req 2 dup(<4, 1, 0, 0, 0, 0, 0>)
// MGC dseg:2CEC 00 00 00 00 00 00 00 00 00 00 03 00 00 00 00 00+Retort_Req <0, 3, 0, 0, 1, 1, 1>
// MGC dseg:2CEC 01 00 01 00 01 00 00 00 00 00 00 00 00 00 00 00+Retort_Req <0, 0, 0, 0, 0, 0, 0>
// MGC dseg:2CEC 00 00 00 00 02 00 03 00 00 00 00 00 00 00 00 00+Retort_Req <2, 3, 0, 0, 0, 0, 0>
// MGC dseg:2CEC 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+Retort_Req  3 dup(<0>)
// MGC dseg:2DE8 00 00 01 00 08 00 0A 00 11 00 0F 00 07 00 09 00+TBL_Retort_Offsets dw RETORTS, RETORTS.Warlord, RETORTS.Channeler, RETORTS.Archmage, RETORTS.Artificer, RETORTS.Conjurer, RETORTS.Sage_Master, RETORTS.Myrran, RETORTS.Divine_Power, RETORTS.Famous, RETORTS.Runemaster, RETORTS.Charismatic, RETORTS.Chaos_Mastery, RETORTS.Nature_Mastery, RETORTS.Sorcery_Mastery, RETORTS.Infernal_Power, RETORTS.Mana_Focusing, RETORTS.Node_Mastery
// MGC dseg:2DE8 06 00 0D 00 0E 00 10 00 02 00 03 00 04 00 05 00+                                        ; DATA XREF: GAME_New_Screen_4+71Fr ...
// MGC dseg:2E0C 66 66 66 66                                     COL_SaveLoad1 db 4 dup( 66h)            ; DATA XREF: Load_Screen_Draw+Br ...
// MGC dseg:2E10 60 61 60 61                                     COL_SaveLoad2 db  60h, 61h, 60h, 61h    ; DATA XREF: Load_Screen_Draw+18r ...

// MGC dseg:2E14
uint8_t RP_COL_NEWG_4Shadow[4] = { 31, 31, 31, 31 };
// MGC dseg:2E18
uint8_t RP_COL_NEWG_4Font[4] = { 187, 187, 187, 187 };

// MGC dseg:2E1C
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

// MGC dseg:2E53
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

// MGC dseg:2E68
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

// MGC dseg:2E83
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

// MGC dseg:2EA3
uint8_t COL_NEWG_FontShadow[8] = { 31, 31, 31, 31, 31, 31, 31, 0 };
// MGC dseg:2EAB
uint8_t COL_NEWG_Font[8] = { 187, 187, 187, 187, 187, 187, 187, 187 };
// MGC dseg:2EB3
uint8_t COL_NEWG_Retorts[2] = {183, 178};
// MGC dseg:2EB5 1F 1F 1F 1F 1F 1F 1F 00                         RP_COL_NEWG_FontShadow2 db 7 dup(1Fh), 0
// MGC dseg:2EB5                                                                                         ; DATA XREF: GAME_Draw_NewScr3+Bo
// MGC dseg:2EB5                                                                                         ; redirected to dseg:2ea3 and repurposed in the
// MGC dseg:2EB5                                                                                         ; worldgen customizer / last profile loader
// MGC dseg:2EBD BB BB BB BB BB BB BB BB                         RP_COL_NEWG_Font2 db 8 dup(0BBh)        ; DATA XREF: GAME_Draw_NewScr3+1Do
// MGC dseg:2EBD                                                                                         ; redirected to dseg:2eab and repurposed in the
// MGC dseg:2EBD                                                                                         ; worldgen customizer / last profile loader
// MGC dseg:2EC5 BB BB BB BB BB BB BB BB                         RP_COL_NEWG_Font3 db 8 dup(0BBh)        ; DATA XREF: GAME_New_Screen_7+Do
// MGC dseg:2EC5                                                                                         ; redirected to dseg:2eab and repurposed in the
// MGC dseg:2EC5                                                                                         ; worldgen customizer / last profile loader
// MGC dseg:2ECD 1F 1F 1F 1F 1F                                  RP_COL_NEWG_5Shadow db 5 dup(1Fh)       ; DATA XREF: GAME_Draw_NewScr7+Do
// MGC dseg:2ECD                                                                                         ; redirected to dseg:2ea3 and repurposed in the
// MGC dseg:2ECD                                                                                         ; worldgen customizer / last profile loader
// MGC dseg:2ED2 BB BB BB BB BB                                  RP_COL_NEWG_5Font db 5 dup(0BBh)        ; DATA XREF: GAME_Draw_NewScr7+1Fo
// MGC dseg:2ED2                                                                                         ; redirected to dseg:2eab and repurposed in the
// MGC dseg:2ED2                                                                                         ; worldgen customizer / last profile loader

// MGC dseg:2ED7 00                                              align 2                                 ; 2ed6 is still free

// MGC dseg:2ED8 00 00 05 00 06 00 07 00 08 00 09 00 0A 00 0B 00+TBL_Arcanian_Races dw R_Barbarian, R_Gnoll, R_Halfling, R_High_Elf, R_High_Man, R_Klackon, R_Lizardman, R_Nomad, R_Orc
// MGC dseg:2ED8 0C 00                                                                                   ; DATA XREF: GAME_New_Screen_6+Do
// MGC dseg:2EEA 01 00 02 00 03 00 04 00 0D 00                   TBL_Myrran_Races dw R_Beastman, R_Dark_Elf, R_Draconian, R_Dwarf, R_Troll
// MGC dseg:2EEA                                                                                         ; DATA XREF: GAME_New_Screen_6+1Fo
// MGC dseg:2EF4 BB BB BB BB BB BB BB BB                         RP_COL_NEWG_Font4 db 8 dup(0BBh)        ; DATA XREF: GAME_New_Screen_6+31o
// MGC dseg:2EF4                                                                                         ; redirected to dseg:2eab and repurposed in the
// MGC dseg:2EF4                                                                                         ; worldgen customizer / last profile loader
// MGC dseg:2EFC 14 B0                                           COL_NEWG_Highlight dw 0B014h            ; DATA XREF: GAME_New_Screen_6+3Er
// MGC dseg:2EFE 19 32                                           COL_Available dw 3219h                  ; DATA XREF: GAME_New_Screen_6+44r
// MGC dseg:2F00 02 02                                           COL_NEWG_RacesShadow dw 202h            ; DATA XREF: GAME_New_Screen_6+4Ar
// MGC dseg:2F02 00 00 05 00 06 00 07 00 08 00 09 00 0A 00 0B 00+RP_TBL_Arcanian_Races_2 dw R_Barbarian, R_Gnoll, R_Halfling, R_High_Elf, R_High_Man, R_Klackon, R_Lizardman, R_Nomad, R_Orc
// MGC dseg:2F02 0C 00                                                                                   ; DATA XREF: GAME_Draw_NewScr6+Do
// MGC dseg:2F02                                                                                         ; redirected to dseg:2ed8 and repurposed in the
// MGC dseg:2F02                                                                                         ; worldgen customizer / last profile loader
// MGC dseg:2F14 01 00 02 00 03 00 04 00 0D 00                   RP_TBL_Myrran_Races_2 dw R_Beastman, R_Dark_Elf, R_Draconian, R_Dwarf, R_Troll
// MGC dseg:2F14                                                                                         ; DATA XREF: GAME_Draw_NewScr6+1Fo
// MGC dseg:2F14                                                                                         ; redirected to dseg:2eea and repurposed in the
// MGC dseg:2F14                                                                                         ; worldgen customizer / last profile loader
// MGC dseg:2F1E 1F 1F 1F 1F 1F                                  RP_COL_NEWG_5Shadow2 db 5 dup(1Fh)      ; DATA XREF: GAME_Draw_NewScr6+31o
// MGC dseg:2F1E                                                                                         ; redirected to dseg:2ea3 and repurposed in the
// MGC dseg:2F1E                                                                                         ; worldgen customizer / last profile loader
// MGC dseg:2F23 BB BB BB BB BB                                  RP_COL_NEWG_5Font2 db 5 dup(0BBh)       ; DATA XREF: GAME_Draw_NewScr6+43o
// MGC dseg:2F23                                                                                         ; redirected to dseg:2eab and repurposed in the
// MGC dseg:2F23                                                                                         ; worldgen customizer / last profile loader
// MGC dseg:2F28 B4 B4 B4 B4 B4                                  COL_NEWG_RaceTitles db 5 dup(0B4h)      ; DATA XREF: GAME_Draw_NewScr6+55o
// MGC dseg:2F2D 14 B0                                           COL_NEWG_Highlight2 dw 0B014h           ; DATA XREF: GAME_Draw_NewScr6+62r
// MGC dseg:2F2D                                                                                         ; should use dseg:2efc
// MGC dseg:2F2F 19 32                                           COL_Available_2 dw 3219h                ; DATA XREF: GAME_Draw_NewScr6+68r
// MGC dseg:2F2F                                                                                         ; should use dseg:2efe
// MGC dseg:2F31 19 16                                           COL_NEWG_NA dw 1619h                    ; DATA XREF: GAME_Draw_NewScr6+6Er
// MGC dseg:2F33 02 02                                           COL_NEWG_RacesShdw2 dw 202h             ; DATA XREF: GAME_Draw_NewScr6+74r
// MGC dseg:2F33                                                                                         ; should use dseg:2f00
// MGC dseg:2F35 BB BB BB BB                                     COL_NEWG_4Font2 db 4 dup(0BBh)          ; DATA XREF: GAME_DrawRetortsStr+Cr ...
// MGC dseg:2F35                                                                                         ; should use dseg:2eab
// MGC dseg:2F39 B7 B2                                           COL_NEWG_Retorts2 dw 0B2B7h             ; DATA XREF: GAME_DrawRetortsStr+16r
// MGC dseg:2F39                                                                                         ; should use dseg:2eb3

// MGC dseg:2F3B 00                                              align 2

// MGC dseg:2F3C FF FF FF FF FF FF FF FF FF FF FF FF             CRP_Unused_Array db 0Ch dup(0FFh)       ; DATA XREF: GAME_DrawRetortsStr+22o
// MGC dseg:2F48 4C 69 66 65 00 00 00 00 00 00                   cnst_Realm0_Text_10b db 'Life',0,0,0,0,0,0
// MGC dseg:2F48                                                                                         ; DATA XREF: GAME_New_Screen_4+Fo
// MGC dseg:2F52 44 65 61 74 68 00 00 00 00 00                   cnst_Realm1_Text_10b db 'Death',0,0,0,0,0
// MGC dseg:2F5C 43 68 61 6F 73 00 00 00 00 00                   cnst_Realm2_Text_10b db 'Chaos',0,0,0,0,0
// MGC dseg:2F66 4E 61 74 75 72 65 00 00 00 00                   cnst_Realm3_Text_10b db 'Nature',0,0,0,0
// MGC dseg:2F70 53 6F 72 63 65 72 79 00 00 00                   cnst_Realm4_Text_10b db 'Sorcery',0,0,0
// MGC dseg:2F7A 01 00 02 00 08 00 09 00 0F 00                   TBL_2pickRetort_Lbls dw RETORT_Labels.Warlord, RETORT_Labels.Channeler, RETORT_Labels.Divine_Power, RETORT_Labels.Famous, RETORT_Labels.Infernal_Power
// MGC dseg:2F7A                                                                                         ; DATA XREF: GAME_New_Screen_4+21o
// MGC dseg:2F84 01 00 02 00 08 00 09 00 0F 00                   RP_TBL_2pickRet_Lbls dw RETORT_Labels.Warlord, RETORT_Labels.Channeler, RETORT_Labels.Divine_Power, RETORT_Labels.Famous, RETORT_Labels.Infernal_Power
// MGC dseg:2F84                                                                                         ; DATA XREF: GAME_Draw_NewScr4+Do
// MGC dseg:2F84                                                                                         ; redirected to dseg:2f7a and re-purposed in the realm
// MGC dseg:2F84                                                                                         ; ordering patch
// MGC dseg:2F8E 1F 1F 1F 1F 1F                                  COL_NEWG_5Shadow3 db 5 dup(1Fh)         ; DATA XREF: GAME_Draw_NewScr4+1Fo
// MGC dseg:2F8E                                                                                         ; should use dseg:2ea3
// MGC dseg:2F93 BB BB BB BB BB                                  COL_NEWG_5Font3 db 5 dup(0BBh)          ; DATA XREF: GAME_Draw_NewScr4+31o
// MGC dseg:2F93                                                                                         ; should use dseg:2eab
// MGC dseg:2F98 19 32                                           COL_Available_3 dw 3219h                ; DATA XREF: GAME_Draw_NewScr4+3Er
// MGC dseg:2F98                                                                                         ; should use dseg:2efe
// MGC dseg:2F9A 19 16                                           COL_NEWG_NA2 dw 1619h                   ; DATA XREF: GAME_Draw_NewScr4+44r
// MGC dseg:2F9A                                                                                         ; should use dseg:2f31
// MGC dseg:2F9C 16 B2                                           COL_NewG_Ret_Known dw 0B216h            ; DATA XREF: GAME_Draw_NewScr4+4Ar
// MGC dseg:2F9E 10 10                                           COL_NewG_Ret_Shdw dw 1010h              ; DATA XREF: GAME_Draw_NewScr4+50r
// MGC dseg:2FA0 01 01 01 00 01 01 00 01 00 01 00 01 00 00 00 00+TBL_Retort_Lbl_Init RETORT_Labels <1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0>
// MGC dseg:2FA0 00 00                                                                                   ; DATA XREF: GAME_Draw_NewScr4+5Bo
// MGC dseg:2FB2 03 00 04 00 02 00 00 00 01 00                   TBL_Realm_Order dw _Life, _Death, _Chaos, _Nature, _Sorcery
// MGC dseg:2FB2                                                                                         ; DATA XREF: GAME_New_Screen_5+Eo
// MGC dseg:2FBC 1A 00 4D 00 80 00                               UU_Label_Box_TitleTops dw 26, 77, 128   ; DATA XREF: GAME_SpellSel_GUI+Do
// MGC dseg:2FC2 25 00 58 00 8B 00                               UU_Label_Box_TextTops dw 37, 88, 139    ; DATA XREF: GAME_SpellSel_GUI+1Fo
// MGC dseg:2FC8 1F 1F 1F 1F 1F                                  COL_NEWG_5Shadow4 db 5 dup(1Fh)         ; DATA XREF: SCRN_Draw_NewScr5+Do
// MGC dseg:2FC8                                                                                         ; should use dseg:2ea3
// MGC dseg:2FCD BB BB BB BB BB                                  COL_NEWG_5Font4 db 5 dup(0BBh)          ; DATA XREF: SCRN_Draw_NewScr5+1Fo
// MGC dseg:2FCD                                                                                         ; should use dseg:2eab
// MGC dseg:2FD2 1A 00 4D 00 80 00                               Label_Box_TitleTops2 dw 26, 77, 128     ; DATA XREF: SCRN_Draw_NewScr5_2+Eo
// MGC dseg:2FD2                                                                                         ; should use dseg:2fbc
// MGC dseg:2FD8 25 00 58 00 8B 00                               Label_Box_TextTops2 dw 37, 88, 139      ; DATA XREF: SCRN_Draw_NewScr5_2+20o
// MGC dseg:2FD8                                                                                         ; should use dseg:2fc2
// MGC dseg:2FDE 19 32                                           COL_Available_4 dw 3219h                ; DATA XREF: SCRN_Draw_NewScr5_2+2Dr
// MGC dseg:2FDE                                                                                         ; should use dseg:2efe
// MGC dseg:2FE0 16 B2                                           COL_NewG_Spl_Known dw 0B216h            ; DATA XREF: SCRN_Draw_NewScr5_2+33r
// MGC dseg:2FE0                                                                                         ; should use dseg:2f9c
// MGC dseg:2FE2 10 10                                           COL_NewG_Spl_Shdw dw 1010h              ; DATA XREF: SCRN_Draw_NewScr5_2+39r
// MGC dseg:2FE2                                                                                         ; should use dseg:2f9e
// MGC dseg:2FE4 43 6F 6D 6D 6F 6E 00 00 00 00                   cnst_Rarity0_10b db 'Common',0,0,0,0    ; DATA XREF: SCRN_Draw_NewScr5_2+44o
// MGC dseg:2FEE 55 6E 63 6F 6D 6D 6F 6E 00 00                   cnst_Rarity1_10b db 'Uncommon',0,0
// MGC dseg:2FF8 52 61 72 65 00 00 00 00 00 00                   cnst_Rarity2_10b db 'Rare',0,0,0,0,0,0
// MGC dseg:3002 56 65 72 79 20 52 61 72 65 00                   cnst_Rarity3_10b db 'Very Rare',0
// MGC dseg:300C 4E 61 74 75 72 65 00 00 00 00                   cnst_Realm0_10b db 'Nature',0,0,0,0     ; DATA XREF: SCRN_Draw_NewScr5_2+57o
// MGC dseg:300C                                                                                         ; should use / convert dseg:2f48
// MGC dseg:3016 53 6F 72 63 65 72 79 00 00 00                   cnst_Realm1_10b db 'Sorcery',0,0,0
// MGC dseg:3020 43 68 61 6F 73 00 00 00 00 00                   cnst_Realm2_10b db 'Chaos',0,0,0,0,0
// MGC dseg:302A 4C 69 66 65 00 00 00 00 00 00                   cnst_Realm3_10b db 'Life',0,0,0,0,0,0
// MGC dseg:3034 44 65 61 74 68 00 00 00 00 00                   cnst_Realm4_10b db 'Death',0,0,0,0,0
// MGC dseg:303E 57 49 5A 41 52 44 53 2E 45 58 45                cnst_EXESwap_File db 'WIZARDS.EXE'      ; DATA XREF: GAME_WizardsLaunch+38o

// MGC dseg:3049
char empty_string__ovr050[] = "";

// MGC dseg:304A 4A 45 4E 4E 59 00                               cnst_EXESwap_Arg db 'JENNY',0           ; DATA XREF: GAME_WizardsLaunch+30o ...
// MGC dseg:3050 4C 4F 41 44 2E 4C 42 58 00                      load_lbx_file db 'LOAD.LBX',0           ; DATA XREF: Load_Screen+Fo ...

// MGC dseg:3059
char str_ESC__ovr050[] = "\x1B";

// MGC dseg:305B 4C 00                                           cnst_HOTKEY_L_2 db 'L',0                ; DATA XREF: Load_Screen+196o
// MGC dseg:305D 53 41 56 45 00                                  cnst_SAVE3 db 'SAVE',0                  ; DATA XREF: Load_Screen:@@Loop_SaveSlotso
// MGC dseg:3062 2E 47 41 4D 00                                  cnst_SAVE_ext3 db '.GAM',0              ; DATA XREF: Load_Screen+1FDo
// MGC dseg:3067 43 6F 70 79 72 69 67 68 74 20 20 53 69 6D 74 65+load_screen_copyright db 'Copyright  Simtex Software, 1995   V1.31',0

// MGC dseg:3090
char newgame_lbx_file__ovr050[] = "NEWGAME.LBX";

// MGC dseg:309C
char str_MAGIC_SET__ovr050[] = "MAGIC.SET";

// MGC dseg:30A6
char str_rb__ovr050[] = "rb";

// MGC dseg:30A9
char str_wb__ovr050[] = "wb";

// MGC dseg:30AC
char cnst_Wiz_Select[] = "Select Wizard";
// MGC dseg:30BA
char cnst_Pic_Select[] = "Select Picture";
// MGC dseg:30C9
char cnst_DOT[] = ".";
// MGC dseg:30CB 57 69 7A 61 72 64 27 73 20 4E 61 6D 65 00       cnst_Name_Select db 'Wizard',27h,'s Name',0
// MGC dseg:30D9 53 65 6C 65 63 74 20 42 61 6E 6E 65 72 00       cnst_Banner_Select db 'Select Banner',0 ; DATA XREF: GAME_Draw_NewScr7+60o
// MGC dseg:30E7 59 6F 75 20 63 61 6E 20 6E 6F 74 20 73 65 6C 65+cnst_Race_Error db 'You can not select a Myrran race unless you have the Myrran special.',0
// MGC dseg:312C 53 65 6C 65 63 74 20 52 61 63 65 00             cnst_Race_Select db 'Select Race',0     ; DATA XREF: GAME_Draw_NewScr6+C3o
// MGC dseg:3138 41 72 63 61 6E 69 61 6E 20 52 61 63 65 73 3A 00 cnst_Arcanian_Races db 'Arcanian Races:',0
// MGC dseg:3148 4D 79 72 72 61 6E 20 52 61 63 65 73 3A 00       cnst_Myrran_Races db 'Myrran Races:',0  ; DATA XREF: GAME_Draw_NewScr6+337o
// MGC dseg:3156 59 6F 75 20 6E 65 65 64 20 74 6F 20 6D 61 6B 65+cnst_Pick_Error_0 db 'You need to make all your picks before you can continue',0
// MGC dseg:318E 59 6F 75 20 6D 61 79 20 6E 6F 74 20 73 65 6C 65+cnst_Pick_Error_1 db 'You may not select more than 6 special abilities',0
// MGC dseg:31BF 54 6F 20 73 65 6C 65 63 74 20 00                cnst_Pick_Error_20 db 'To select ',0    ; DATA XREF: GAME_New_Screen_4:loc_40AA3o
// MGC dseg:31CA 20 79 6F 75 20 6E 65 65 64 3A 20 20 20 00       cnst_Pick_Error_21 db ' you need:   ',0 ; DATA XREF: GAME_New_Screen_4+44Co
// MGC dseg:31D8 20 70 69 63 6B 73 20 69 6E 20 00                cnst_Pick_Error_22 db ' picks in ',0    ; DATA XREF: GAME_New_Screen_4+4D8o
// MGC dseg:31E3 20 4D 61 67 69 63 00                            cnst_Pick_Error_23 db ' Magic',0        ; DATA XREF: GAME_New_Screen_4+502o ...
// MGC dseg:31EA 20 70 69 63 6B 00                               cnst_Pick_Error_24 db ' pick',0         ; DATA XREF: GAME_New_Screen_4+561o
// MGC dseg:31F0 73                                              cnst_Pick_Error_25 db 's'               ; DATA XREF: GAME_New_Screen_4+583o
// MGC dseg:31F1 20 69 6E 20 61 6E 79 20 00                      cnst_Pick_Error_26 db ' in any ',0      ; DATA XREF: GAME_New_Screen_4:loc_40C05o
// MGC dseg:31FA 20 52 65 61 6C 6D 73 20 6F 66 20 4D 61 67 69 63+cnst_Pick_Error_27 db ' Realms of Magic',0
// MGC dseg:31FA 00                                                                                      ; DATA XREF: GAME_New_Screen_4+5DFo
// MGC dseg:320B 52 65 61 6C 6D 20 6F 66 20 4D 61 67 69 63 00    cnst_Pick_Error_28 db 'Realm of Magic',0
// MGC dseg:320B                                                                                         ; DATA XREF: GAME_New_Screen_4:loc_40C61o
// MGC dseg:321A 2C 20 00                                        cnst_Pick_Error_29 db ', ',0            ; DATA XREF: GAME_New_Screen_4+623o ...
// MGC dseg:321D 20 70 69 63 6B 20 69 6E 20 00                   cnst_Pick_Error_2A db ' pick in ',0     ; DATA XREF: GAME_New_Screen_4+669o
// MGC dseg:3227 59 6F 75 20 63 61 6E 20 6E 6F 74 20 73 65 6C 65+cnst_Pick_Error_3 db 'You can not select both Life and Death magic',0
// MGC dseg:3227 63 74 20 62 6F 74 68 20 4C 69 66 65 20 61 6E 64+                                        ; DATA XREF: GAME_New_Screen_4:loc_40D2Eo
// MGC dseg:3254 59 6F 75 20 68 61 76 65 20 61 6C 72 65 61 64 79+cnst_Pick_Error_4 db 'You have already made all your'
// MGC dseg:3254 20 6D 61 64 65 20 61 6C 6C 20 79 6F 75 72                                               ; DATA XREF: GAME_New_Screen_4:loc_40D34o
// MGC dseg:3272 20 70 69 63 6B 73 00                            cnst_Picks db ' picks',0                ; DATA XREF: GAME_Draw_NewScr4+AA9o ...
// MGC dseg:3279 59 6F 75 20 64 6F 6E 27 74 20 68 61 76 65 20 65+cnst_Pick_Error_5 db 'You don',27h,'t have enough picks left to make this selection. You need 2 picks',0
// MGC dseg:3279 6E 6F 75 67 68 20 70 69 63 6B 73 20 6C 65 66 74+                                        ; DATA XREF: GAME_New_Screen_4:loc_40D3Ao

// MGC dseg:32C3
char hlpentry_lbx_file__MGC_ovr050[] = "hlpentry.lbx";

// MGC dseg:32D0 59 6F 75 20 68 61 76 65 20 6E 6F 20 70 69 63 6B+cnst_Pick_Error_6 db 'You have no picks left in this area, to deselect click on a selected item',0
// MGC dseg:32D0 73 20 6C 65 66 74 20 69 6E 20 74 68 69 73 20 61+                                        ; DATA XREF: GAME_New_Screen_5:loc_42154o
// MGC dseg:331A 53 65 6C 65 63 74 20 00                         cnst_Spell_Select_0 db 'Select ',0      ; DATA XREF: SCRN_Draw_NewScr5_2+DDo
// MGC dseg:3322 20 53 70 65 6C 6C 73 00                         cnst_Spell_Select_1 db ' Spells',0      ; DATA XREF: SCRN_Draw_NewScr5_2+108o
// MGC dseg:332A 3A 20 00                                        cnst_Spell_Select_2 db ': ',0           ; DATA XREF: SCRN_Draw_NewScr5_2+22Bo ...
// MGC dseg:332D 20 61 6E 64 20 00                               cnst_And db ' and ',0                   ; DATA XREF: STR_ListSeparator+1Co

// MGC dseg:3333 00                                              align 2

// MGC dseg:3333                                                 END:  ovr050 - Initialized Data



// MGC dseg:8A42                                                 BEGIN:  ovr050 - Uninitialized Data

// MGC dseg:8A42
struct Default_Spells TBL_Spells_Nature;
// MGC dseg:8A5C
struct Default_Spells TBL_Spells_Sorcery;
// MGC dseg:8A76
struct Default_Spells TBL_Spells_Chaos;
// MGC dseg:8A90
struct Default_Spells TBL_Spells_Life;
// MGC dseg:8AAA
struct Default_Spells TBL_Spells_Death;

// MGC dseg:8AC4
struct Default_Spells TBL_Spells_P1;
// MGC dseg:8B46
struct Default_Spells TBL_Spells_P2;
// MGC dseg:8BC8
struct Default_Spells TBL_Spells_P3;
// MGC dseg:8C4A
struct Default_Spells TBL_Spells_P4;
// MGC dseg:8CCC
struct Default_Spells UU_TBL_Spells_P5;

// MGC dseg:8D4E 00 00                                           NEWG_PickAttempt dw 0                   ; DATA XREF: GAME_New_Screen_4+436r ...
// MGC dseg:8D50 00 00                                           NEWG_PickError dw 0                     ; DATA XREF: GAME_New_Screen_4+15Bw ...

// MGC dseg:8D52
// ; initialized to random values of 0, 1, or 2
int16_t TBL_Bookshelf_Books[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
// MGC dseg:8D68
int16_t TBL_Realm0_Books[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
// MGC dseg:8D7E
int16_t TBL_Realm1_Books[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
// MGC dseg:8D94
int16_t TBL_Realm2_Books[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
// MGC dseg:8DAA
int16_t TBL_Realm3_Books[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
// MGC dseg:8DC0
int16_t TBL_Realm4_Books[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// MGC dseg:8DD6
// drake78: IMG_NEWG_MapBuildBG
/*
    loaded in GAME_New_Screen_7()
    used in NEWG_CreateWorld__WIP()
*/
SAMB_ptr IMG_NEWG_MapBuildBG = 0;

// MGC dseg:8DD8 00 00                                           NEWG_SliderPos_Sorc dw 0                ; DATA XREF: GAME_New_Screen_4+155w ...
// MGC dseg:8DDA 00 00                                           NEWG_SliderPos_Nat dw 0                 ; DATA XREF: GAME_New_Screen_4+14Fw ...
// MGC dseg:8DDC 00 00                                           NEWG_SliderPos_Chaos dw 0               ; DATA XREF: GAME_New_Screen_4+149w ...
// MGC dseg:8DDE 00 00                                           NEWG_SliderPos_Death dw 0               ; DATA XREF: GAME_New_Screen_4+143w ...
// MGC dseg:8DE0 00 00                                           NEWG_SliderPos_Life dw 0                ; DATA XREF: GAME_New_Screen_4+13Dw ...

// MGC dseg:8DE2
/*
set to 8 in Newgame_Screen1__WIP() set-up portion

"Select Wizard" vs. "Select Picture"

WTF?
init'd to 0, defaulted to 8, never changed

*/
int16_t NEWG_PortraitSelType = 0;

// MGC dseg:8DE4 00                                              db    0
// MGC dseg:8DE5 00                                              db    0
// MGC dseg:8DE6
// ; 5 groups of 3 images each (L - S - N - D - C)
SAMB_ptr IMG_NewG_Books[15] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// MGC dseg:8E04
int16_t newgame_landsize_button_field = 0;
// MGC dseg:8E06
int16_t newgame_magic_button_field = 0;
// MGC dseg:8E08
int16_t newgame_opponents_button_field = 0;
// MGC dseg:8E0A
int16_t newgame_difficulty_button_field = 0;

// MGC dseg:8E0C
int16_t NEWG_Select_Labels[15] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int16_t NEWG_Moused_Wizard = 0;

// MGC dseg:8E4A
SAMB_ptr IMG_NewG_ButtonBGs[15] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// MGC dseg:8E68
SAMB_ptr IMG_NewG_RgtOverlay = 0;

// MGC dseg:8E6A 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+NEWG_Retort_Labels dw 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
// MGC dseg:8E6A 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; DATA XREF: GAME_New_Screen_4+2BEw ...
// MGC dseg:8E8E 00 00                                           _quit_active_seg dw 0                   ; DATA XREF: Load_Screen+32w ...
// MGC dseg:8E90 00 00                                           _load_active_seg dw 0                   ; DATA XREF: Load_Screen+49w ...
// MGC dseg:8E92 00 00                                           _ok_button dw 0                         ; DATA XREF: Load_Screen+1BDw ...
// MGC dseg:8E94 00 00                                           NEWG_Picks_Remaining dw 0               ; DATA XREF: GAME_New_Screen_4+10Ew ...
// MGC dseg:8E96 00 00                                           _selection_marker_seg dw 0              ; DATA XREF: Load_Screen+D3w ...
// MGC dseg:8E98 00 00                                           selected_load_game_slot_idx dw 0        ; DATA XREF: Load_Screen+2D8w ...
// MGC dseg:8E9A 00 00                                           MAY__selected_save_game_slot_idx dw 0
// MGC dseg:8E9C 00 00                                           NEWG_SpellSel_Realm dw 0                ; DATA XREF: GAME_New_Screen_5+1D3w ...
// MGC dseg:8E9E 00 00                                           _settings_button dw 0                   ; DATA XREF: Load_Screen+2D5w ...
// MGC dseg:8EA0 00 00                                           _settings_button_seg dw 0               ; DATA XREF: Load_Screen+BCw ...
// MGC dseg:8EA2 00 00                                           _text_fill_seg dw 0                     ; DATA XREF: Load_Screen+A5w ...
// MGC dseg:8EA4 00 00                                           NEWG_Clicked_Race dw 0                  ; DATA XREF: GAME_New_Screen_6+1EDw ...
// MGC dseg:8EA6 00 00                                           dw 0
// MGC dseg:8EA8 00 00                                           dw 0
// MGC dseg:8EAA 00 00                                           NEWG_ProfileComplete dw 0               ; DATA XREF: GAME_New_Screen_4+114w ...
// MGC dseg:8EAC 00 00                                           _ok_inactive_seg dw 0                   ; DATA XREF: Load_Screen+8Ew ...
// MGC dseg:8EAE 00 00                                           _save_inactive_seg dw 0                 ; DATA XREF: Load_Screen+77w ...

// MGC dseg:8EAE                                                 END:  ovr050 - Uninitialized Data



// MGC dseg:8EB0                                                 BEGIN:  ovr051 - Uninitialized Data

// MGC dseg:8EB0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+NEWG_Landmass_Races dw 200 dup(0)       ; DATA XREF: NEWG_CreateNeutrals+39w ...
// MGC dseg:9040 00 00                                           NEWG_LandmassCount dw 0                 ; DATA XREF: NEWG_ClearLandmasses+4w ...
// MGC dseg:9042 00 00                                           dw 0
// MGC dseg:9044 00 00                                           dw 0
// MGC dseg:9046 00 00                                           dw 0
// MGC dseg:9048 00 00                                           dw 0
// MGC dseg:904A 00 00                                           dw 0

// MGC dseg:904A                                                 END:  ovr051 - Uninitialized Data





/*
    MAGIC.EXE
    ovr050
*/

// o50p01
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
Newgame_Control__WIP();  // MAGIC.EXE  ovr050  o050p001

Newgame_Screen0();  // returns 1 on input field is ok button

NOTE(JimBalcomb,20251221): definitely done-done, non-WIP
*/
void Newgame_Control__WIP(void)
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
            case -1:
            {
                return;  // ¿ cancel ?
            } break;
            case 0:
            {
                newgame_state = Newgame_Screen0();  // returns 1 on input field is ok button
            } break;
            case 1:
            {
                newgame_state = Newgame_Screen1__WIP();  // returns {0,2,3} - {0:cancel,2:custom,3:prefab}
                if(newgame_state == 2)
                {
                    custom_game_flag = ST_TRUE;
                }
                else
                {
                    custom_game_flag = ST_FALSE;
                }
            } break;
            case 2:
            {
                newgame_state = Newgame_Screen2__WIP();
            } break;
            case 3:
            {
                newgame_state = Newgame_Screen3__WIP();
                if(custom_game_flag != ST_FALSE)
                {
                    if(newgame_state == ST_UNDEFINED)
                    {
                        newgame_state = 2;
                    }
                    else
                    {
                        newgame_state = 4;
                    }
                }
                else
                {
                    if(newgame_state == ST_UNDEFINED)
                    {
                        newgame_state = 1;
                    }
                    else
                    {
                        newgame_state = 4;
                    }
                }
            } break;
            case 4:
            {
                Newgame_Screen4__WIP();
            } break;
            case 5:
            {
                Newgame_Screen5__WIP();
            } break;
            case 6:
            {
                newgame_state = Newgame_Screen6__WIP();
                if(newgame_state == ST_UNDEFINED)
                {
                    if(custom_game_flag != ST_FALSE)
                    {
                        newgame_state = 5;
                    }
                    else
                    {
                        newgame_state = 3;
                    }
                }
            } break;
            case 7:
            {
                Newgame_Screen7__WIP();
            } break;
            case 99:
            {

            } break;

        }
        
    }

    NEWG_CreateWorld__WIP();

    Initialize_Events();

    NEWG_FinalizeTables__WIP();

    Save_SAVE_GAM(8);

    // TODO  GAME_WizardsLaunch(8);

}


// o50p02
// drake178: GAME_WizardsLaunch()
/*
; fades out the screen, if the passed save index is not
; that of the continue save (index 8), loads that game
; and saves it as the current continue, then launches
; WIZARDS.EXE (which always loads that save on startup)
*/
/*

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


// o50p03
// Load_Screen()

// o50p04
// Load_Screen_Draw()

// o50p05
/*
PATCHED - rewritten completely in the last profile
loader/worldgen customizer/patch enabler
displays and processes the new game creation screen
for selecting the basic campaign options (difficulty,
# of opponents, land size, and magic)
returns 1 and saves the selection to both disk and
memory if the OK button is clicked, or returns -1
without saving if the Esc key is pressed
*/
/*
returns 1 on input field is ok button
*/
int16_t Newgame_Screen0(void)
{
    char file_found[LEN_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t leave_screen = 0;
    int16_t First_Draw_Done = 0;
    int16_t input_field_idx = 0;  // _SI_
    FILE * file_pointer = 0;  // _DI_
    
    screen_changed = ST_TRUE;
    
    // ; conflicting condition - will never jump
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
    IMG_NewG_ButtonBGs[0] = LBX_Reload_Next(newgame_lbx_file__ovr050, 4, _screen_seg);
    IMG_NewG_ButtonBGs[1] = LBX_Reload_Next(newgame_lbx_file__ovr050, 5, _screen_seg);
    IMG_NewG_ButtonBGs[2] = LBX_Reload_Next(newgame_lbx_file__ovr050, 6, _screen_seg);
    IMG_NewG_ButtonBGs[3] = LBX_Reload_Next(newgame_lbx_file__ovr050, 7, _screen_seg);

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

    Set_Mouse_List(1, mouse_list_newgame_0_1);

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

    Assign_Auto_Function(Newgame_Screen0_Draw, 1);

    Set_Newgame_Screen0_Help_List();

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
            
            return 1;  // 
        }
        
        if(input_field_idx == newgame_difficulty_button_field)
        {

            magic_set.Difficulty++;

            if(magic_set.Difficulty > 4)
            {
                magic_set.Difficulty = 1;
            }

        }

        if(input_field_idx == newgame_opponents_button_field)
        {

            magic_set.Opponents++;

            if(magic_set.Opponents > 4)
            {
                magic_set.Opponents = 1;
            }

        }

        if(input_field_idx == newgame_landsize_button_field)
        {

            magic_set.LandSize++;

            if(magic_set.LandSize > 2)
            {
                magic_set.LandSize = 0;
            }

        }

        if(input_field_idx == newgame_magic_button_field)
        {

            magic_set.MagicPower++;

            if(magic_set.MagicPower > 2)
            {
                magic_set.MagicPower = 0;
            }

        }

        if(leave_screen == ST_FALSE)
        {

            Newgame_Screen0_Draw();

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

// o50p06
/*
PATCHED - rewritten completely in the last profile
loader/worldgen customizer/patch enabler

draws the first screen of the new game creation
options into the current draw segment
*/
/*

*/
void Newgame_Screen0_Draw(void)
{
    struct s_OPPONENT_COUNT_NAMES l_opponent_count_names[4] = { 0, 0, 0, 0 };
    struct s_MAGIC_STRENGTH_NAMES l_magic_strength_names[3] = { 0, 0, 0 };
    struct s_LAND_SIZE_NAMES l_land_size_names[3] = { 0, 0, 0 };
    struct s_DIFFICULTY_NAMES l_difficulty_names[5] = { 0, 0, 0, 0, 0 };
    // uint8_t colors2[4] = { 0, 0, 0, 0 };
    // uint8_t colors1[4] = { 0, 0, 0, 0 };
    uint8_t * colors2;
    uint8_t * colors1;

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
        FLIC_Draw(251, 39, IMG_NewG_ButtonBGs[0]);
        Set_Font_Colors_15(3, &colors1[0]);
        Print_Centered(283, 43, l_difficulty_names[magic_set.Difficulty].name);
        Set_Font_Colors_15(3, &colors2[0]);
        Print_Centered(282, 42, l_difficulty_names[magic_set.Difficulty].name);
    }
    else
    {
        FLIC_Draw(252, 40, IMG_NewG_ButtonBGs[0]);
        Set_Font_Colors_15(3, &colors1[0]);
        Print_Centered(284, 44, l_difficulty_names[magic_set.Difficulty].name);
        Set_Font_Colors_15(3, &colors2[0]);
        Print_Centered(283, 43, l_difficulty_names[magic_set.Difficulty].name);
    }

    if(auto_input_field_idx != newgame_opponents_button_field)
    {
        FLIC_Draw(251, 66, IMG_NewG_ButtonBGs[1]);
        Set_Font_Colors_15(3, &colors1[0]);
        Print_Centered(283, 71, l_opponent_count_names[magic_set.Opponents].name);
        Set_Font_Colors_15(3, &colors2[0]);
        Print_Centered(282, 70, l_opponent_count_names[magic_set.Opponents].name);
    }
    else
    {
        FLIC_Draw(252, 67, IMG_NewG_ButtonBGs[1]);
        Set_Font_Colors_15(3, &colors1[0]);
        Print_Centered(284, 72, l_opponent_count_names[magic_set.Opponents].name);
        Set_Font_Colors_15(3, &colors2[0]);
        Print_Centered(283, 71, l_opponent_count_names[magic_set.Opponents].name);
    }

    if(auto_input_field_idx != newgame_landsize_button_field)
    {
        FLIC_Draw(251, 93, IMG_NewG_ButtonBGs[2]);
        Set_Font_Colors_15(3, &colors1[0]);
        Print_Centered(283, 97, l_land_size_names[magic_set.LandSize].name);
        Set_Font_Colors_15(3, &colors2[0]);
        Print_Centered(282, 96, l_land_size_names[magic_set.LandSize].name);
    }
    else
    {
        FLIC_Draw(252, 94, IMG_NewG_ButtonBGs[2]);
        Set_Font_Colors_15(3, &colors1[0]);
        Print_Centered(284, 98, l_land_size_names[magic_set.LandSize].name);
        Set_Font_Colors_15(3, &colors2[0]);
        Print_Centered(283, 97, l_land_size_names[magic_set.LandSize].name);
    }

    if(auto_input_field_idx != newgame_magic_button_field)
    {
        FLIC_Draw(251, 120, IMG_NewG_ButtonBGs[3]);
        Set_Font_Colors_15(3, &colors1[0]);
        Print_Centered(283, 124, l_magic_strength_names[magic_set.MagicPower].name);
        Set_Font_Colors_15(3, &colors2[0]);
        Print_Centered(282, 123,     l_magic_strength_names[magic_set.MagicPower].name);
    }
    else
    {
        FLIC_Draw(252, 121, IMG_NewG_ButtonBGs[3]);
        Set_Font_Colors_15(3, &colors1[0]);
        Print_Centered(284, 125, l_magic_strength_names[magic_set.MagicPower].name);
        Set_Font_Colors_15(3, &colors2[0]);
        Print_Centered(283, 124, l_magic_strength_names[magic_set.MagicPower].name);
    }

}

// o50p07
// drake178: GAME_RandBookBinders()
/*
; randomizes the spellbook binder image arrays used
; for displaying bookshelves
*/
/*
Pretty sure, ...
    randomizes the bookshelf book heights
    for the bookshelf under the wizard portrait
    and while selecting books for a custom wizard
    only has 4 because you can't have Life *and* Death

¿ UU_Settings_Product was to be a random seed ?

*/
void Randomize_Book_Heights(void)
{
    int32_t itr = 0;
    int32_t UU_Settings_Product = 0;

    UU_Settings_Product = ((((magic_set.MagicPower * magic_set.Opponents) * (magic_set.Difficulty * magic_set.LandSize)) * 123) + 1234);

    for(itr = 0; itr < 11; itr++)
    {
        TBL_Bookshelf_Books[itr] = (Random(3) - 1);
        TBL_Realm4_Books[itr]    = (Random(3) - 1);
        TBL_Realm3_Books[itr]    = (Random(3) - 1);
        TBL_Realm2_Books[itr]    = (Random(3) - 1);
        TBL_Realm0_Books[itr]    = (Random(3) - 1);
        TBL_Realm1_Books[itr]    = (Random(3) - 1);
        TBL_Bookshelf_Books[itr] = (Random(3) - 1);
    }

}

// o50p08
// drake178: GAME_New_Screen_1()
/*
; displays and processes the second screen of new game
; creation: choosing a wizard, enabling custom creation
; only on the Easy and higher difficulties
; returns 2 if the Custom button is clicked, 0 if the
; Esc key is pressed, or 3 after copying default wizard
; data if a pre-defined character is selected
*/
/*
returns {0,2,3} - {0:cancel,2:custom,3:prefab}
*/
int16_t Newgame_Screen1__WIP(void)
{
    int16_t Escape_Hotkey_Control = 0;
    int16_t input_field_idx = 0;
    int16_t leave_screen = 0;
    int16_t First_Draw_Done = 0;
    int16_t IDK = 0;  // _DI_
    int16_t itr = 0;  // _SI_

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
        IMG_NewG_ButtonBGs[itr] = LBX_Reload_Next(newgame_lbx_file__ovr050, (9 + itr), _screen_seg);
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

    // ; load the spellbook binder images
    // ; PATCHED here for realm reordering
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
        IMG_NewG_Books[itr] = LBX_Reload_Next(newgame_lbx_file__ovr050, (24 + itr), _screen_seg);
    }

    Set_Mouse_List(1, mouse_list_newgame_0_1);  // ; should use Normal_Fullscreen

    Clear_Fields();

    leave_screen = ST_FALSE;
    
    First_Draw_Done = ST_FALSE;
    
    NEWG_Moused_Wizard = 0;

    NEWG_PortraitSelType = 8;  // Default: "Select Wizard"

    NEWG_Select_Labels[14] = INVALID_FIELD;

    // ; create the necessary amount of click label controls
    for(itr = 0; itr < 7; itr++)
    {

        NEWG_Select_Labels[itr] = Add_Hidden_Field(168, (26 + (22 * itr)), 237, (42 + (22 * itr)), empty_string__ovr050, ST_UNDEFINED);

    }

    for(itr = 0; itr < IDK; itr++)
    {

        NEWG_Select_Labels[(7+itr)] = Add_Hidden_Field(244, (26 + (22 * itr)), 313, (42 + (22 * itr)), empty_string__ovr050, ST_UNDEFINED);

    }

    Escape_Hotkey_Control = Add_Hot_Key(str_ESC__ovr050);

    Assign_Auto_Function(Newgame_Screen1_Draw__WIP, 1);

    if(magic_set.Difficulty < god_Easy)
    {
        Set_Newgame_Screen1_Help_List(ST_FALSE);
    }
    else
    {
        Set_Newgame_Screen1_Help_List(ST_TRUE);
    }

    Set_Input_Delay(2);

    while(leave_screen == ST_FALSE)
    {

        input_field_idx = Get_Input();

        Mark_Time();

        if(input_field_idx == Escape_Hotkey_Control)
        {
            return 0;
        }

        for(itr = 0; itr < 14; itr++)
        {
            if(NEWG_Select_Labels[itr] == input_field_idx)
            {
                Deactivate_Auto_Function();
                Deactivate_Help_List();
                WIZ_CopyDefault__WIP(itr);
                return 3;
            }
        }

        if(input_field_idx == NEWG_Select_Labels[14])  // "Custom"
        {
            Deactivate_Auto_Function();
            Deactivate_Help_List();
            return 2;
        }

        if(leave_screen == ST_FALSE)
        {
            Newgame_Screen1_Draw__WIP();
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

// o50p09
/*
; draws the new game wizard portrait selection screen
; into the current draw frame, with or without retort
; and bookshelf display, and with or without showing
; the "Custom" button, all based on global variables
*/
/*
unresolved external symbol Set_Newgame_Screen1_Help_List referenced in function Newgame_Screen1__WIP
*/
void Newgame_Screen1_Draw__WIP(void)
{
    char Retort_String[30] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    uint8_t * Retort_Text_Color = 0;
    uint8_t Font_Colors[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    uint8_t Shadow_Colors[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Right_Column_Count = 0;
    int16_t l_auto_input_field_idx = 0;  // _DI_
    int16_t itr = 0;  // _SI_

// lea     ax, [bp+Shadow_Colors]
// push    ss
// push    ax                                ; Dest_Struct
// mov     ax, offset COL_NEWG_FontShadow
// push    ds
// push    ax                                ; src
// mov     cx, 8
// call    SCOPY@                            ; compiler generate - copy struct to struct
    memcpy(Shadow_Colors, COL_NEWG_FontShadow, 8);

// lea     ax, [bp+Font_Colors]
// push    ss
// push    ax                                ; Dest_Struct
// mov     ax, offset COL_NEWG_Font
// push    ds                                ; Color_3
// push    ax                                ; Color_2
// mov     cx, 8
// call    SCOPY@                            ; compiler generate - copy struct to struct
    memcpy(Font_Colors, COL_NEWG_Font, 8);

    Retort_Text_Color = COL_NEWG_Retorts;

    l_auto_input_field_idx = Auto_Input();

    if(Scan_Input() != ST_NULL)
    {
        NEWG_Moused_Wizard = (Scan_Input() - 1);
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

    // draws the blank portrait
    if(NEWG_Moused_Wizard < 14)  // "Custom"
    {
        FLIC_Draw(24, 10, wizard_portrait_segs[NEWG_Moused_Wizard]);
    }

    Set_Font_Style(3, 15, ST_NULL, ST_NULL);

    // ; draw the wizard selection buttons of the left column
    for(itr = 0; itr < 7; itr++)
    {

        if(NEWG_Select_Labels[itr] != l_auto_input_field_idx)
        {
            FLIC_Draw(169, (27 + (22 * itr)), IMG_NewG_ButtonBGs[itr]);
            Set_Font_Colors_15(3, &Shadow_Colors[0]);
            Print_Centered(203, (31 + (22 * itr)), TBL_Default_Wizards[itr].Name);
            Set_Font_Colors_15(3, &Font_Colors[0]);
            Print_Centered(202, (30 + (22 * itr)), TBL_Default_Wizards[itr].Name);
        }
        else
        {
            FLIC_Draw(170, (28 + (22 * itr)), IMG_NewG_ButtonBGs[itr]);
            Set_Font_Colors_15(3, &Shadow_Colors[0]);
            Print_Centered(204, (32 + (22 * itr)), TBL_Default_Wizards[itr].Name);
            Set_Font_Colors_15(3, &Font_Colors[0]);
            Print_Centered(203, (31 + (22 * itr)), TBL_Default_Wizards[itr].Name);
        }

    }

    Right_Column_Count = NEWG_PortraitSelType;

    if(magic_set.Difficulty < god_Easy)
    {
        Right_Column_Count = 7;
    }

    for(itr = 0; itr < Right_Column_Count; itr++)
    {

        if(NEWG_Select_Labels[(7+itr)] == l_auto_input_field_idx)
        {
            FLIC_Draw(246, (28 + (22 * itr)), IMG_NewG_ButtonBGs[(7 + itr)]);
            Set_Font_Colors_15(3, &Shadow_Colors[0]);
            Print_Centered(280, (32 + (22 * itr)), TBL_Default_Wizards[(7 + itr)].Name);
            Set_Font_Colors_15(3, &Font_Colors[0]);
            Print_Centered(279, (31 + (22 * itr)), TBL_Default_Wizards[(7 + itr)].Name);
        }
        else
        {
            FLIC_Draw(245, (27 + (22 * itr)), IMG_NewG_ButtonBGs[(7 + itr)]);
            Set_Font_Colors_15(3, &Shadow_Colors[0]);
            Print_Centered(279, (31 + (22 * itr)), TBL_Default_Wizards[(7 + itr)].Name);
            Set_Font_Colors_15(3, &Font_Colors[0]);
            Print_Centered(278, (30 + (22 * itr)), TBL_Default_Wizards[(7 + itr)].Name);
        }

    }

    /* Print Retort Name */
    if(
        (TBL_Default_Wizards[NEWG_Moused_Wizard].Retort != ST_UNDEFINED)
        &&
        (NEWG_PortraitSelType == 8)  // defaulted to 8 and god is not Intro so didn't get reduced to 7  second has 7 wizards + custom
        &&
        (NEWG_Moused_Wizard < 14)  // "Custom" is not the mouse-over
    )
    {
        Set_Font_Style(0, 15, ST_NULL, ST_NULL);
        Set_Font_Colors_15(0, &Font_Colors[0]);
        strcpy(Retort_String, STR_Retorts[TBL_Default_Wizards[NEWG_Moused_Wizard].Retort]);  // 1 + {-1,0,1,2,...}
        strcat(Retort_String, cnst_DOT);
        Print(13, 101, Retort_String);
        Set_Font_Colors_15(0, &Retort_Text_Color[0]);
        Print(12, 180, Retort_String);
    }

    if(NEWG_PortraitSelType == 8)
    {
        NEWG_DrawDefShelf__WIP(NEWG_Moused_Wizard);
    }

    /* Print Wizard Name */
    if(NEWG_Moused_Wizard < 14)  // "Custom"
    {
        Set_Font_Style(4, 15, ST_NULL, ST_NULL);
        Set_Font_Colors_15(4, &Shadow_Colors[0]);
        Print_Centered(78, 120, TBL_Default_Wizards[NEWG_Moused_Wizard].Name);
        Print_Centered(77, 120, TBL_Default_Wizards[NEWG_Moused_Wizard].Name);
        Set_Font_Colors_15(4, &Font_Colors[0]);
        Print_Centered(77, 119, TBL_Default_Wizards[NEWG_Moused_Wizard].Name);
    }

}

// o50p10
/*

*/
int16_t Newgame_Screen2__WIP(void)
{



    return 0;

}

// o50p11
/*

*/
int16_t Newgame_Screen3__WIP(void)
{

    return 1;

}


// o50p12
// GAME_Draw_NewScr3()

// o50p13
/*

*/
void Newgame_Screen7__WIP(void)
{



}


// o50p14
// GAME_Draw_NewScr7()

// o50p15
/*

*/
int16_t Newgame_Screen6__WIP(void)
{

    return 0;

}


// o50p16
// GAME_Draw_NewScr6()

// o50p17
// GAME_DrawRetortsStr()

// o50p18
/*

; PATCHED / rewritten in the realm reordering patch
;
; draws the bookshelf of the selected default profile
; into the current draw segment under the large wizard
; portait location on the left side of the new game
; creation screen
;
; WARNING: limited to only two realms of books
*/
/*

*/
void NEWG_DrawDefShelf__WIP(int16_t Portrait_Index)
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

    Profile_Books_Offset = &TBL_Default_Wizards[Portrait_Index].Life;

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
            // ; initialized to random values of 0, 1, or 2
            FLIC_Draw(IDK, 135, IMG_NewG_Books[(First_Realm_Books_Index + TBL_Bookshelf_Books[itr])]);
            IDK += 8;
        }
    }

    if(Second_Realm_Books_Index != ST_UNDEFINED)
    {
        for(itr = 0; itr < Second_Book_Count; itr++)
        {
            // ; initialized to random values of 0, 1, or 2
            FLIC_Draw(IDK, 135, IMG_NewG_Books[(Second_Realm_Books_Index + TBL_Bookshelf_Books[itr])]);
            IDK += 8;
        }
    }

}

// o50p19
/*

*/
void Newgame_Screen4__WIP(void)
{



}


// o50p20
// GAME_Draw_NewScr4()

// o50p21
/*

*/
void Newgame_Screen5__WIP(void)
{



}


// o50p22
// GAME_SpellSel_GUI()

// o50p23
// SCRN_Draw_NewScr5()

// o50p24
// SCRN_Draw_NewScr5_2()

// o50p25
/*
; copies the name and profile traits of the selected
; default wizard to the human player's wizard record,
; and their default spells to the new game spell
; selection array, clearing any previous data in these
; fields
*/
/*

*/
void WIZ_CopyDefault__WIP(int16_t Portrait_Index)
{
    int8_t * Retorts_Pointer = 0;
    int16_t itr = 0;  // _SI_
    int16_t spellranks = 0;  // _DI_

    _players[0].wizard_id = Portrait_Index;

    _players[0].spellranks[0] = TBL_Default_Wizards[Portrait_Index].Nature;
    _players[0].spellranks[1] = TBL_Default_Wizards[Portrait_Index].Sorcery;
    _players[0].spellranks[2] = TBL_Default_Wizards[Portrait_Index].Chaos;
    _players[0].spellranks[3] = TBL_Default_Wizards[Portrait_Index].Life;
    _players[0].spellranks[4] = TBL_Default_Wizards[Portrait_Index].Death;

    Retorts_Pointer = &_players[0].alchemy;

    // ; clear all retorts in the human player's wizard record
    // ; while copying any that are in the preset profile
    for(itr = 0; itr < NUM_RETORTS; itr++)
    {

        if(TBL_Default_Wizards[Portrait_Index].Retort == itr)
        {
            Retorts_Pointer[itr] = 1;
        }
        else
        {
            Retorts_Pointer[itr] = 0;
        }

    }

    // ; clear the spell selection list
    for(itr = 0; itr < 13; itr++)
    {

        TBL_Spells_Nature.Common[itr] = 0;
        TBL_Spells_Sorcery.Common[itr] = 0;
        TBL_Spells_Chaos.Common[itr] = 0;
        TBL_Spells_Life.Common[itr] = 0;
        TBL_Spells_Death.Common[itr] = 0;

    }

    spellranks = _players[0].spellranks[0];
    // ; PATCHED here to fix 11-book default wizard spells
    if(spellranks > 1)
    {
        for(itr = 0; (TBL_SpellsPerBook_C[spellranks] - 2) > itr; itr++)
        {
            TBL_Spells_Nature.Common[itr] = TBL_Def_Spl_Nature.Common[itr];
        }
        for(itr = 0; (TBL_SpellsPerBook_U[spellranks] - 2) > itr; itr++)
        {
            TBL_Spells_Nature.Uncommon[itr] = TBL_Def_Spl_Nature.Uncommon[itr];
        }
        if((TBL_SpellsPerBook_U[spellranks] - 2) > 0)
        {
            TBL_Spells_Nature.Rare = TBL_Def_Spl_Nature.Rare;
        }
    }

    spellranks = _players[0].spellranks[1];
    // ; PATCHED here to fix 11-book default wizard spells
    if(spellranks > 1)
    {
        for(itr = 0; (TBL_SpellsPerBook_C[spellranks] - 2) > itr; itr++)
        {
            TBL_Spells_Sorcery.Common[itr] = TBL_Def_Spl_Nature.Common[itr];
        }
        for(itr = 0; (TBL_SpellsPerBook_U[spellranks] - 2) > itr; itr++)
        {
            TBL_Spells_Sorcery.Uncommon[itr] = TBL_Def_Spl_Nature.Uncommon[itr];
        }
        if((TBL_SpellsPerBook_U[spellranks] - 2) > 0)
        {
            TBL_Spells_Sorcery.Rare = TBL_Def_Spl_Nature.Rare;
        }
    }

    spellranks = _players[0].spellranks[2];
    // ; PATCHED here to fix 11-book default wizard spells
    if(spellranks > 1)
    {
        for(itr = 0; (TBL_SpellsPerBook_C[spellranks] - 2) > itr; itr++)
        {
            TBL_Spells_Chaos.Common[itr] = TBL_Def_Spl_Nature.Common[itr];
        }
        for(itr = 0; (TBL_SpellsPerBook_U[spellranks] - 2) > itr; itr++)
        {
            TBL_Spells_Chaos.Uncommon[itr] = TBL_Def_Spl_Nature.Uncommon[itr];
        }
        if((TBL_SpellsPerBook_U[spellranks] - 2) > 0)
        {
            TBL_Spells_Chaos.Rare = TBL_Def_Spl_Nature.Rare;
        }
    }

    spellranks = _players[0].spellranks[3];
    // ; PATCHED here to fix 11-book default wizard spells
    if(spellranks > 1)
    {
        for(itr = 0; (TBL_SpellsPerBook_C[spellranks] - 2) > itr; itr++)
        {
            TBL_Spells_Life.Common[itr] = TBL_Def_Spl_Nature.Common[itr];
        }
        for(itr = 0; (TBL_SpellsPerBook_U[spellranks] - 2) > itr; itr++)
        {
            TBL_Spells_Life.Uncommon[itr] = TBL_Def_Spl_Nature.Uncommon[itr];
        }
        if((TBL_SpellsPerBook_U[spellranks] - 2) > 0)
        {
            TBL_Spells_Life.Rare = TBL_Def_Spl_Nature.Rare;
        }
    }

    spellranks = _players[0].spellranks[4];
    // ; PATCHED here to fix 11-book default wizard spells
    if(spellranks > 1)
    {
        for(itr = 0; (TBL_SpellsPerBook_C[spellranks] - 2) > itr; itr++)
        {
            TBL_Spells_Death.Common[itr] = TBL_Def_Spl_Nature.Common[itr];
        }
        for(itr = 0; (TBL_SpellsPerBook_U[spellranks] - 2) > itr; itr++)
        {
            TBL_Spells_Death.Uncommon[itr] = TBL_Def_Spl_Nature.Uncommon[itr];
        }
        if((TBL_SpellsPerBook_U[spellranks] - 2) > 0)
        {
            TBL_Spells_Death.Rare = TBL_Def_Spl_Nature.Rare;
        }
    }

    strcpy(_players[0].name, TBL_Default_Wizards[Portrait_Index].Name);

}


// o50p26
// Fade_Out()

// o50p27
// VGA_Fade_In()

// o50p28
// Set_Load_Screen_Help_List_MGC()

// o50p29
// drake178: HLP_Load_NewGOptions()
/*
loads and sets the GUI help entry area array for the
new game options selection screen
*/
/*

¿ Set_Options_Screen_Help_List ?

*/
void Set_Newgame_Screen0_Help_List(void)
{

    // HLPENTRY.LBX, 29  Options Screen Help
    LBX_Load_Data_Static(hlpentry_lbx_file__MGC_ovr050, 29, (SAMB_ptr)_help_entries, 0, 6, 10);

    Set_Help_List(_help_entries, 6);

}


// o50p30
// HLP_Load_BannerSel()

// o50p31
// HLP_Load_PortraitSel()

// o50p32
/*
; loads and sets the GUI help entry area array for the
; wizard selection screen
*/
/*

*/
void Set_Newgame_Screen1_Help_List(int16_t has_custom)
{

    // HLPENTRY.LBX, 032  ""  "Wizard Selection Help"
    LBX_Load_Data_Static(hlpentry_lbx_file__MGC_ovr050, 32, _help_entries, 0, 15, 10);

    if(has_custom != ST_FALSE)
    {
        _help_entries[140].help_idx = HLP_CUSTOM;
    }

    Set_Help_List(_help_entries, 15);

}


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
