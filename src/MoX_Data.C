
#include "MoX_TYPE.H"
#include "MoX_DEF.H"
#include "MoM_DEF.H"

#include "MoX_Data.H"

#include "Mouse.H"  /* struct s_mouse_list */





// WZD dseg:0000                                                 ; ===========================================================================
// WZD dseg:0000
// WZD dseg:0000                                                 ; Segment type: Pure data
// WZD dseg:0000                                                 segment dseg para public 'DATA' use16
// WZD dseg:0000                                                 assume cs:dseg
// WZD dseg:0000 00 00 00 00                                     oopsie_accidentally_renamed db 4 dup(0) ; start of c0.asm _DATA
// WZD dseg:0004 42 6F 72 6C 61 6E 64 20 43 2B 2B 20 2D 20 43 6F+Copyright_Msg db 'Borland C++ - Copyright 1991 Borland Intl.',0
// WZD dseg:002F 4E 75 6C 6C 20 70 6F 69 6E 74 65 72 20 61 73 73+NullCheck_Msg db 'Null pointer assignment',0Dh,0Ah
// WZD dseg:002F 69 67 6E 6D 65 6E 74 0D 0A                                                              ; DATA XREF: __checknull+1Fo
// WZD dseg:0048 44 69 76 69 64 65 20 65 72 72 6F 72 0D 0A       ZeroDiv_Msg db 'Divide error',0Dh,0Ah   ; DATA XREF: ZeroDivision:loc_101AAo
// WZD dseg:0056 41 62 6E 6F 72 6D 61 6C 20 70 72 6F 67 72 61 6D+Abort_Msg db 'Abnormal program termination',0Dh,0Ah
// WZD dseg:0056 20 74 65 72 6D 69 6E 61 74 69 6F 6E 0D 0A                                               ; DATA XREF: _abort+3o
// WZD dseg:0074 00 00 00 00                                     _Int0Vector dd 0                        ; DATA XREF: SaveVectors:loc_101B6w ...
// WZD dseg:0078 00 00 00 00                                     _Int4Vector dd 0                        ; DATA XREF: SaveVectors+13w ...
// WZD dseg:007C 00 00 00 00                                     _Int5Vector dd 0                        ; DATA XREF: SaveVectors+20w ...
// WZD dseg:0080 00 00 00 00                                     _Int6Vector dd 0                        ; DATA XREF: SaveVectors+2Dw ...
// WZD dseg:0084 00 00                                           __C0argc dw 0                           ; DATA XREF: start+154r
// WZD dseg:0086 00 00                                           __C0argv dw 0                           ; DATA XREF: start+150r
// WZD dseg:0088 00 00                                           __C0environ dw 0                        ; DATA XREF: start+14Cr ...
// WZD dseg:008A 00 00                                           _envLng dw 0                            ; DATA XREF: start+46w ...
// WZD dseg:008C 00 00                                           _envseg dw 0                            ; DATA XREF: start+1Dw ...
// WZD dseg:008E 00 00                                           _envSize dw 0                           ; DATA XREF: start+55w ...
// WZD dseg:0090 00 00                                           _psp dw 0                               ; DATA XREF: start+19w ...
// WZD dseg:0092 00 00                                           _version dw 0                           ; DATA XREF: start+16w ...
// WZD dseg:0094 00 00                                           errno dw 0                              ; DATA XREF: __IOerror:ser_endw ...
// WZD dseg:0096 00 00 00 00                                     _StartTime dd 0                         ; DATA XREF: start+124w ...
// WZD dseg:009A 54 EA                                           __heapbase dw 0EA54h                    ; DATA XREF: Cityscape_Draw_Scanned_Building_Name+14Fr ...
// WZD dseg:009C 54 EA                                           __brklvl dw 0EA54h                      ; DATA XREF: __brk+10w ...
// WZD dseg:009E 00 00 00 00                                     _heapbase dd 0                          ; DATA XREF: _brk+7r ...
// WZD dseg:00A2 00 00 00 00                                     _brklvl dd 0                            ; DATA XREF: normalize+22w ...
// WZD dseg:00A6 00 00 00 00                                     _heaptop dd 0                           ; DATA XREF: normalize+69w ...
// WZD dseg:00A6                                                                                         ; end of c0.asm _DATA
// WZD dseg:00AA
// WZD dseg:00AA
// WZD dseg:00AA                                                 BEGIN: Data Segment - Initialized Data
// WZD dseg:00AA
// WZD dseg:00AA
// WZD dseg:00AA 69 6A 6B 6C 6D 49 4A 4B 4C 4D 79 7A 7B 7C 7D C9+COL_Banners db 105,106,107,108,109      ; DATA XREF: Draw_Unit_StatFig+BFr ...
// WZD dseg:00AA A5 CB A6 2D A0 A1 A2 B2 B4 30 31 32 33 34       db 73,74,75,76,77
// WZD dseg:00AA                                                 db 121,122,123,124,125
// WZD dseg:00AA                                                 db 201,165,203,166,45
// WZD dseg:00AA                                                 db 160,161,162,178,180
// WZD dseg:00AA                                                 db 48,49,50,51,52
// WZD dseg:00C8 62 D8 7B 2B B3 32 C9 00 A5 00 CB 00 79 00 7A 00+COL_Banners2 db 98,216,123,43,179,50    ; DATA XREF: Unit_List_Window_Draw+2Fr ...
// WZD dseg:00C8 7B 00 0D 00 0E 00 0F 00 49 00 4A 00 4B 00 69 00+db 201,0,165,0,203,0
// WZD dseg:00C8 6A 00 6B 00                                     db 121,0,122,0,123,0
// WZD dseg:00C8                                                 db 13,0,14,0,15,0
// WZD dseg:00C8                                                 db 73,0,74,0,75,0
// WZD dseg:00C8                                                 db 105,0,106,0,107,0
// WZD dseg:00EC 00                                              db    0
// WZD dseg:00ED 00                                              db    0
// WZD dseg:00ED
// WZD dseg:00ED
// WZD dseg:00EE
// WZD dseg:00EE
// WZD dseg:00EE 01 00 00 00 00 00 00 00 3F 01 C7 00             mouse_list_default s_MOUSE_LIST <crsr_Finger, 0, 0, 0, 319, 199>
// WZD dseg:00EE                                                                                         ; DATA XREF: Screen_Control+11o ...
// WZD dseg:00FA 00 00 00 00 00 00 00 00 3F 01 C7 00             mouse_list_none s_MOUSE_LIST <0, 0, 0, 0, 319, 199>
// WZD dseg:00FA                                                                                         ; DATA XREF: Item_Screen+202o ...
// WZD dseg:0106 06 00 00 00 00 00 00 00 3F 01 C7 00             mouse_list_hourglass s_MOUSE_LIST <crsr_Hourglass, 0, 0, 0, 319, 199>
// WZD dseg:0106                                                                                         ; DATA XREF: Next_Turn_Calc+14o ...
// WZD dseg:0112 01 00 00 00 00 00 00 00 3F 01 C7 00             NIU_mouse_list_normal s_MOUSE_LIST <crsr_Finger, 0, 0, 0, 319, 199>
// WZD dseg:011E 07 00 04 00 00 00 00 00 3F 01 9E 00             NIU_mouse_list_boot s_MOUSE_LIST <crsr_WingedBoot, 4, 0, 0, 319, 158> ; ? 158 is main map width ?
// WZD dseg:011E
// WZD dseg:011E
// WZD dseg:012A 01 02 04 08 10 20                               NIU_byte_36BCA db         1,      10b,     100b,    1000b,   10000b,  100000b

// WZD dseg:0130
// char * _wizard_abilities_names[] =

// DELETEME  // WZD dseg:0154 01 00                                           
// DELETEME  int16_t EVNT_Enabled = ST_TRUE;

// WZD dseg:0156 4D 21 55 21 5C 21 64 21 69 21 6E 21             _city_size_names dw offset cnst_Outpost, offset cnst_Hamlet, offset cnst_Village, offset cnst_Town, offset cnst_City, offset cnst_Capital

// WZD dseg:0162
char * STR_MagicBuildings[7] =
{
    cnst_SummoningCircle, 
    cnst_EarthGate, 
    cnst_StreamOfLife, 
    cnst_AstralGate, 
    cnst_Fortress, 
    cnst_DarkRituals, 
    cnst_AltarofBattle
};

// WZD dseg:0170 8C 00 00 00 82 00 05 00 78 00 0A 00 6E 00 0F 00+UU_UnknownValuePairs dw 140, 0, 130, 5, 120, 10, 110, 15, 100, 20, 90, 25, 80, 30, 70, 35, 60, 40, 50, 45, 40, 50
// WZD dseg:019C





// WZD dseg:2080                                                 ¿  BEGIN: meaningful boundary ?
// WZD dseg:2080
// WZD dseg:2080 00 10                                           __ovrbuffer dw 4096                     ; DATA XREF: __OvrPrepare+19r ...
// WZD dseg:2080                                                                                         ; declared in overlay.lib\OVRBUFF
// WZD dseg:2080                                                 ¿  BEGIN: meaningful boundary ?

// WZD dseg:2082
char cnst_Alchemy[] = "Alchemy";
// WZD dseg:2089
char empty_string__MAIN[] = "";
// WZD dseg:208A
char cnst_Warlord[] = "Warlord";
// WZD dseg:2092
char cnst_ChaosMastery[] = "Chaos Mastery";
// WZD dseg:20A0
char cnst_NatureMastery[] = "Nature Mastery";
// WZD dseg:20AF
char cnst_SorceryMastery[] = "Sorcery Mastery";
// WZD dseg:20BF
char cnst_InfernalPower[] = "Infernal Power";
// WZD dseg:20CE
char cnst_DivinePower[] = "Divine Power";
// WZD dseg:20DB
char cnst_SageMaster[] = "Sage Master";
// WZD dseg:20E7
char cnst_Channeler[] = "Channeler";
// WZD dseg:20F1
char cnst_Myrran[] = "Myrran";
// WZD dseg:20F8
char cnst_Archmage[] = "Archmage";
// WZD dseg:2101
char cnst_ManaFocusing[] = "Mana Focusing";
// WZD dseg:210F
char cnst_NodeMastery[] = "Node Mastery";
// WZD dseg:211C
char cnst_Famous[] = "Famous";
// WZD dseg:2123
char cnst_Runemaster[] = "Runemaster";
// WZD dseg:212E
char cnst_Conjurer[] = "Conjurer";
// WZD dseg:2137
char cnst_Charismatic[] = "Charismatic";
// WZD dseg:2143
char cnst_Artificer[] = "Artificer";

// WZD dseg:214D
char cnst_Outpost[] = "Outpost";
// WZD dseg:2155
char cnst_Hamlet[] = "Hamlet";
// WZD dseg:215C
char cnst_Village[] = "Village";
// WZD dseg:2164
char cnst_Town[] = "Town";
// WZD dseg:2169
char cnst_City[] = "City";
// WZD dseg:216E
char cnst_Capital[] = "Capital";

// WZD dseg:2176
char cnst_SummoningCircle[] = "Summoning Circle";
// WZD dseg:2187
char cnst_EarthGate[] = "Earth Gate";
// WZD dseg:2192
char cnst_StreamOfLife[] = "Stream Of Life";
// WZD dseg:219C
char cnst_Rlm3_Life[] = "Life";
// WZD dseg:21A1
char cnst_AstralGate[] = "Astral Gate";
// WZD dseg:21AD
char cnst_Fortress[] = "Fortress";
// WZD dseg:21B6
char cnst_DarkRituals[] = "Dark Rituals";
// WZD dseg:21C3
char cnst_AltarofBattle[] = "Altar of Battle";





// WZD dseg:21D3                                                 ¿ BEGIN: Unit Names ?

// WZD dseg:26DE                                                 ¿ BEGIN:  Summoned Units Race Names ?

// WZD dseg:26DE
char cnst_Rlm5_Arcane[] = "Arcane";
// WZD dseg:26E5
char cnst_Rlm0_Nature[] = "Nature";
// WZD dseg:26EC
char cnst_Rlm1_Sorcery[] = "Sorcery";
// WZD dseg:26F4
char cnst_Rlm2_Chaos[] = "Chaos";
// WZD dseg:26FA
char cnst_Rlm4_Death[] = "Death";

// WZD dseg:26FA                                                 ¿ END:  Summoned Units Race Names ?







// WZD dseg:00AA                                                 BEGIN: Data Segment - Initialized Data


// ¿ MoO2: ~ remap_picture_colors ?
// WZD dseg:00AA 69 6A 6B 6C 6D                                  COL_Banners_P0 db 105, 106, 107, 108, 109
// WZD dseg:00AF 49 4A 4B 4C 4D                                  COL_Banners_P1 db 73, 74, 75, 76, 77
// WZD dseg:00B4 79 7A 7B 7C 7D                                  COL_Banners_P2 db 121, 122, 123, 124, 125
// WZD dseg:00B9 C9 A5 CB A6 2D                                  COL_Banners_P3 db 201, 165, 203, 166, 45
// WZD dseg:00BE A0 A1 A2 B2 B4                                  COL_Banners_P4 db 160, 161, 162, 178, 180
// WZD dseg:00C3 30 31 32 33 34                                  COL_Banners_P5 db 48, 49, 50, 51, 52
uint8_t COL_Banners[] = {
    0x69, 0x6A, 0x6B, 0x6C, 0x6D,
    0x49, 0x4A, 0x4B, 0x4C, 0x4D,
    0x79, 0x7A, 0x7B, 0x7C, 0x7D,
    0xC9, 0xA5, 0xCB, 0xA6, 0x2D,
    0xA0, 0xA1, 0xA2, 0xB2, 0xB4,
    0x30, 0x31, 0x32, 0x33, 0x34
};

// WZD dseg:00C8
uint8_t COL_Banners2[36] = {
    98, 216, 123, 43, 179, 50,
    201,  0, 165,  0, 203,  0,
    121,  0, 122,  0, 123,  0,
     13,  0,  14,  0,  15,  0,
     73,  0,  74,  0,  75,  0,
    105,  0, 106,  0, 107,  0
};

// WZD dseg:00EE 01 00 00 00 00 00 00 00 3F 01 C7 00             mouse_list_default s_MOUSE_LIST <crsr_Finger, 0, 0, 0, 319, 199>
// struct s_mouse_list mouse_list_default[1] = {
//     {crsr_Finger, 0, 0, 0, 319, 199}
// };
// WZD dseg:00FA 00 00 00 00 00 00 00 00 3F 01 C7 00             mouse_list_none s_MOUSE_LIST <0, 0, 0, 0, 319, 199>
// struct s_mouse_list mouse_list_none[1] = {
//     {crsr_None, 0, 0, 0, SCREEN_XMAX, SCREEN_YMAX}
// };
// WZD dseg:0106 06 00 00 00 00 00 00 00 3F 01 C7 00             mouse_list_hourglass s_MOUSE_LIST <crsr_Hourglass, 0, 0, 0, 319, 199>
struct s_mouse_list mouse_list_hourglass[1] = {
    {crsr_Hourglass, 0, 0, 0, SCREEN_XMAX, SCREEN_YMAX}
};
// WZD dseg:0112 01 00 00 00 00 00 00 00 3F 01 C7 00             NIU_mouse_list_normal s_MOUSE_LIST <crsr_Finger, 0, 0, 0, 319, 199>
// WZD dseg:011E 07 00 04 00 00 00 00 00 3F 01 9E 00             NIU_mouse_list_boot s_MOUSE_LIST <crsr_WingedBoot, 4, 0, 0, 319, 158> ; ? 158 is main map width ?

// WZD dseg:012A 01 02 04 08 10 20                               byte_36BCA db   1,  2,  4,  8, 16, 32

// WZD dseg:0130
char * _wizard_abilities_names[18] =
{
    cnst_Alchemy, 
    cnst_Warlord, 
    cnst_ChaosMastery, 
    cnst_NatureMastery, 
    cnst_SorceryMastery, 
    cnst_InfernalPower, 
    cnst_DivinePower, 
    cnst_SageMaster, 
    cnst_Channeler, 
    cnst_Myrran, 
    cnst_Archmage,
    cnst_ManaFocusing, 
    cnst_NodeMastery, 
    cnst_Famous, 
    cnst_Runemaster, 
    cnst_Conjurer, 
    cnst_Charismatic, 
    cnst_Artificer
};

// DELETEME  // WZD dseg:0154 01 00                                           EVNT_Enabled dw 1                       ; DATA XREF: EVNT_GenerateRandom:loc_6AD68r

// WZD dseg:0156
char * _city_size_names[6] =
{
    cnst_Outpost,
    cnst_Hamlet,
    cnst_Village,
    cnst_Town,
    cnst_City,
    cnst_Capital
};
 




// WZD dseg:1D74
// drake178: struct XP_TABLE TBL_Experience
int16_t TBL_Experience[9] = {0, 20, 60, 120, 200, 300, 450, 600, 1000};
// 00 struc XP_TABLE ; (sizeof=0x12, standard type)
// 00 Hero_or_Recruit dw ?                    ; base 10
// 02 Myrmidon_or_Regular dw ?                ; base 10
// 04 Captain_or_Veteran dw ?                 ; base 10
// 06 Cmdr_or_Elite dw ?                      ; base 10
// 08 Champion dw ?                           ; base 10
// 0A Lord_or_Ultra_Elite dw ?                ; base 10
// 0C Grand_Lord dw ?                         ; base 10
// 0E Super_Hero dw ?                         ; base 10
// 10 Demi_God dw ?                           ; base 10
// 12 ends XP_TABLE





// WZD dseg:1D86                                                 dw 2                                    ; HouseType
// WZD dseg:1ED6 89 20                                           STR_USW_NoRace@ dw offset cnst_ZeroString_3
// WZD dseg:1ED8 DE 26 E5 26 EC 26 F4 26 9C 21 FA 26             STR_USW_MagicRaces@ dw offset cnst_Rlm5_Arcane, offset cnst_Rlm0_Nature, offset cnst_Rlm1_Sorcery, offset cnst_Rlm2_Chaos, offset cnst_Rlm3_Life, offset cnst_Rlm4_Death
// WZD dseg:1ED8                                                                                         ; DATA XREF: USW_DrawView+453t ...
// WZD dseg:1ED8                                                                                         ; "Life"

// WZD dseg:1ED6 89 20                                           rtn14 dw offset empty_string__MAIN
// WZD dseg:1ED8 DE 26                                           rtn15 dw offset cnst_Rlm5_Arcane        ; "Arcane"
// WZD dseg:1EDA E5 26                                           rtn16 dw offset cnst_Rlm0_Nature        ; DATA XREF: Lair_Treasure_Popup+28Cr
// WZD dseg:1EDA                                                                                         ; "Nature"
// WZD dseg:1EDC EC 26                                           rtn17 dw offset cnst_Rlm1_Sorcery       ; "Sorcery"
// WZD dseg:1EDE F4 26                                           rtn18 dw offset cnst_Rlm2_Chaos         ; "Chaos"
// WZD dseg:1EE0 9C 21                                           rtn19 dw offset cnst_Rlm3_Life          ; "Life"
// WZD dseg:1EE2 FA 26                                           rtn20 dw offset cnst_Rlm4_Death         ; "Death"

// WZD dseg:1EDA
char * rtn16[5] =
{
    cnst_Rlm0_Nature,
    cnst_Rlm1_Sorcery, 
    cnst_Rlm2_Chaos, 
    cnst_Rlm3_Life, 
    cnst_Rlm4_Death
};

// ; "Nature"
// ; "Sorcery"
// ; "Chaos"
// ; "Life"
// ; "Death"








// WZD dseg:1EE4
char * _city_enchantment_names[26] = 
{
    cnst_WallofFire,
    cnst_ChaosRift,
    cnst_DarkRituals, 
    cnst_EvilPresence, 
    cnst_CursedLands, 
    cnst_Pestilence, 
    cnst_CloudofShadow, 
    cnst_Famine, 
    cnst_FlyingFortress1, 
    cnst_NatureWard, 
    cnst_SorceryWard, 
    cnst_ChaosWard, 
    cnst_LifeWard, 
    cnst_DeathWard, 
    cnst_NaturesEye, 
    cnst_EarthGate, 
    cnst_StreamofLife, 
    cnst_GaiasBlessing, 
    cnst_Inspirations, 
    cnst_Prosperity, 
    cnst_AstralGate, 
    cnst_HeavenlyLight, 
    cnst_Consecration, 
    cnst_WallofDarkness, 
    cnst_AltarofBattle, 
    cnst_Nightshade
};



// WZD dseg:1F18
/*
Unit Enchantment upkeeps in bit order, from Immolation to Invulnerability
*/
int16_t unit_enchantment_upkeep_table[32] = {2, 2, 0, 1, 1, 3, 10, 1, 1, 1, 5, 1, 5, 1, 1, 10, 10, 3, 1, 5, 2, 1, 2, 1, 2, 1, 4, 1, 5, 2, 2, 5};

// WZD dseg:1F58
/*
City Enchantment upkeeps in byte order, from Wall of Fire to Altar of Battle, plus Nightshade
*/
int16_t city_enchantment_upkeep_table[26] = {2, 10, 0, 4, 2, 5, 3, 5, 25, 5, 5, 5, 5, 5, 1, 5, 8, 3, 2, 2, 5, 2, 8, 5, 5, 0};

// WZD dseg:1F8C
/*
Global Enchantment upkeeps in byte order, from Eternal Night to Awareness
*/
int16_t overland_enchantment_upkeep_table[24] = {15, 10, 40, 5, 5, 50, 200, 7, 10, 10, 40, 15, 20, 10, 40, 10, 10, 10, 3, 10, 5, 10, 3, 3};

// WZD dseg:1FBC 11 28                                           
char * _personality_type_names[] =
{
    aManiacal,
    aRuthless, 
    aAggressive,
    aChaotic,
    aLawful,
    aPeaceful
};


// ; added to chance of forming treaties
// ; added to chance of avoiding superiority wars
// WZD dseg:1FC8 00 00 0A 00 14 00 1E 00 28 00 32 00             TBL_AI_PRS_???_Mod dw 0, 10, 20, 30, 40, 50


// WZD dseg:1FD4
char * _objective_type_names[] =
{
    aPragmatist,
    aMilitarist,
    aTheurgist,
    aPerfectionist,
    aExpansionist
};


// WZD dseg:1FDE 28 00 14 00 0A 00 0A 00 D8 FF EC FF             TBL_AI_PRS_War_Mod dw 40, 20, 10, 10, 65496, 65516

// WZD dseg:1FEA 
// drake178: TBL_AI_DIFF_Mods 

// 64 00 64 00 64 00 64 00 64 00 64 00 64 00 64 00 
// 64 00 64 00 7D 00 7D 00 7D 00 64 00 7D 00 5A 00 
// 96 00 96 00 96 00 96 00 96 00 64 00 96 00 4B 00 
// C8 00 C8 00 C8 00 C8 00 C8 00 6E 00 C8 00 3C 00 
// 90 01 90 01 90 01 90 01 90 01 96 00 90 01 1E 00 

// TBL_AI_DIFF_Mods 
// DIFF_REC <100, 100, 100, 100, 100, 100, 100, 100>; 0
// DIFF_REC <100, 100, 125, 125, 125, 100, 125, 90>; 1
// DIFF_REC <150, 150, 150, 150, 150, 100, 150, 75>; 2
// DIFF_REC <200, 200, 200, 200, 200, 110, 200, 60>; 3
// DIFF_REC <400, 400, 400, 400, 400, 150, 400, 30>; 4



// MoX_Data.H
// struct s_DIFFICULTY_MODIFIERS
// {
//     /* 00 */  int16_t population_growth;
//     /* 02 */  int16_t outpost_growth;
//     /* 04 */  int16_t production;
//     /* 06 */  int16_t gold;
//     /* 08 */  int16_t power;
//     /* 0A */  int16_t research;
//     /* 0C */  int16_t food;
//     /* 0E */  int16_t maintenance;
//     /* 10 */
// };

/* Intro, Easy, Normal, Hard, Impossible */
/* population_growth, outpost_growth, production, gold, mana, research, food, maintenance */
struct s_DIFFICULTY_MODIFIERS difficulty_modifiers_table[NUM_DIFFICULTY_LEVEL] =
{
    {100, 100, 100, 100, 100, 100, 100, 100},
    {100, 100, 125, 125, 125, 100, 125,  90},
    {150, 150, 150, 150, 150, 100, 150,  75},
    {200, 200, 200, 200, 200, 110, 200,  60},
    {400, 400, 400, 400, 400, 150, 400,  30}
};


// WZD dseg:203A
// TBL_AI_BLD_OBJWgts AI_BLD_Obj_Priorities
/*
    10 categories
     5 objectives   Pragmatist, Militarist, Theurgist, Perfectionist, Expansionist
*/
int16_t TBL_AI_BLD_OBJWgts[10][5] = 
{
    { 0,  0,  0,  5,  0},
    { 0,  0,  0,  5,  0},
    { 0,  0, 10,  0,  0},
    { 0,  0, 10,  0,  0},
    { 0, 10,  0,  0,  2},
    { 0,  0,  0,  5,  0},
    { 0,  0,  0,  5,  0},
    { 0,  5,  0,  0,  5},
    { 0,  5,  0,  0,  3},
    { 0,  0,  0,  0, 10}
};

// WZD dseg:206C
/*
    ¿ indexed by bldg_data_table[product_array[itr]].Category ?
    ¿ indexed by bldg_data_table[bldg_idx].Category ?
    
*/
int16_t TBL_AI_BLD_BaseWgts[10] =
{
    10, 15, 30, 10, 10, 15, 15, 10, 10, 10
};


// WZD dseg:2080                                                 ¿  BEGIN: meaningful boundary ?
// WZD dseg:2080 00 10                                           __ovrbuffer dw 4096
// ; DATA XREF: __OvrPrepare+19r ...
// ; declared in overlay.lib\OVRBUFF
// WZD dseg:2080                                                 ¿  BEGIN: meaningful boundary ?

// WZD dseg:2082 41 6C 63 68 65 6D 79                            cnst_Alchemy db 'Alchemy'


// WZD dseg:2143 41 72 74 69 66 69 63 65 72 00                   cnst_Artificer db 'Artificer'





// WZD dseg:2700
char cnst_WallofFire[] = "Wall of Fire";
// WZD dseg:270D
char cnst_ChaosRift[] = "Chaos Rift";
// WZD dseg:2718
char cnst_EvilPresence[] = "Evil Presence";
// WZD dseg:2726
char cnst_CursedLands[] = "Cursed Lands";
// WZD dseg:2733
char cnst_Pestilence[] = "Pestilence";
// WZD dseg:273E
char cnst_CloudofShadow[] = "Cloud of Shadow";
// WZD dseg:274E
char cnst_Famine[] = "Famine";
// WZD dseg:2755
char cnst_FlyingFortress1[] = "Flying";
// WZD dseg:275C
char cnst_NatureWard[] = "Nature Ward";
// WZD dseg:2768
char cnst_SorceryWard[] = "Sorcery Ward";
// WZD dseg:2775
char cnst_ChaosWard[] = "Chaos Ward";
// WZD dseg:2780
char cnst_LifeWard[] = "Life Ward";
// WZD dseg:278A
char cnst_DeathWard[] = "Death Ward";
// WZD dseg:2795
char cnst_NaturesEye[] = "Nature's Eye";
// WZD dseg:27A2
char cnst_StreamofLife[] = "Stream of Life";
// WZD dseg:27B1
char cnst_GaiasBlessing[] = "Gaia's Blessing";
// WZD dseg:27C1
char cnst_Inspirations[] = "Inspirations";
// WZD dseg:27CE
char cnst_Prosperity[] = "Prosperity";
// WZD dseg:27D9
char cnst_HeavenlyLight[] = "Heavenly Light";
// WZD dseg:27E8
char cnst_Consecration[] = "Consecration";
// WZD dseg:27F5
char cnst_WallofDarkness[] = "Wall of Darkness";
// WZD dseg:2806
char cnst_Nightshade[] = "Nightshade";



// WZD dseg:2811                                                 BEGIN:  _personality_type_names
// WZD dseg:2811
char aManiacal[] = "Maniacal";
// WZD dseg:281A
char aRuthless[] = "Ruthless";
// WZD dseg:2823
char aAggressive[] = "Aggressive";
// WZD dseg:282E
char aChaotic[] = "Chaotic";
// WZD dseg:2836
char aLawful[] = "Lawful";
// WZD dseg:283D
char aPeaceful[] = "Peaceful";
// WZD dseg:283D                                                 END:  _personality_type_names

// WZD dseg:2846                                                 BEGIN:  _objective_type_names
// WZD dseg:2846
char aPragmatist[] = "Pragmatist";
// WZD dseg:2851
char aMilitarist[] = "Militarist";
// WZD dseg:285C
char aTheurgist[] = "Theurgist";
// WZD dseg:2866
char aPerfectionist[] = "Perfectionist";
// WZD dseg:2874
char aExpansionist[] = "Expansionist";
// WZD dseg:2874                                                 END:  _objective_type_names



// WZD dseg:2F8C
char _msg_planar_seal_prevents[] = "Planar Seal prevents your units from changing planes.";
// WZD dseg:2FC2
char _msg_units_cant_planar_travel[] = "The selected units cannot Planar Travel at this location.";
// WZD dseg:2FFC
char _msg_city_cant_produce_1[] = "The";
// WZD dseg:2FFF
char _msg_city_cant_produce_4[] = " ";
// WZD dseg:3001
char _msg_city_cant_produce_2[] = " of ";
// WZD dseg:3006
char _msg_city_cant_produce_3[] = " can no longer produce ";
// WZD dseg:301E
char _msg_city_cant_produce_5[] = ".";
// WZD dseg:3020
char _msg_guardian_kills_spirit[] = "The Guardian Spirit occupying this node kills your spirit.";
// WZD dseg:305B 00                                              align 2





// WZD dseg:33B8
// drake178: MoveFlag_Array
// 01 00 20 00 40 00 04 00 02 00 08 00             
// MoveFlag_Array dw M_Cavalry, M_Forester, M_Mntnr, M_Swimming, M_Sailing, M_Flying
// OON XREF: STK_GetMoveTypes()
int16_t MoveType_Flags[6];  // = { 0x0001, 0x0020, 0x0040, 0x0004, 0x0002, 0x0008 }

// // struct s_Movement_Modes
// // {
// //     /* 0x00 */  uint16_t Cavalry = 0x0001;
// //     /* 0x02 */  uint16_t Forester = 0x0020;
// //     /* 0x04 */  uint16_t Mountaineer = 0x0040;
// //     /* 0x06 */  uint16_t Swimming = 0x0004;
// //     /* 0x08 */  uint16_t Sailing = 0x0002;
// //     /* 0x0A */  uint16_t Flying = 0x0008;
// //     /* 0x0C */  uint16_t PlanarTravel;
// //     /* 0x0E */
// // };
// 
// struct s_Movement_Modes
// {
//     /* 0x00 */  uint16_t Cavalry;
//     /* 0x02 */  uint16_t Forester;
//     /* 0x04 */  uint16_t Mountaineer;
//     /* 0x06 */  uint16_t Swimming;
//     /* 0x08 */  uint16_t Sailing;
//     /* 0x0A */  uint16_t Flying;
//     /* 0x0C */  uint16_t PlanarTravel;
//     /* 0x0E */  uint16_t Walking;
// };

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
struct s_Movement_Modes movement_modes_array = { CAVALRY, FORESTER, MOUNTAINEER, SWIMMING, SAILING, FLYING };





// MGC dseg:52C6
// AKA gsa_WIZARDS_0to13
SAMB_ptr wizard_portrait_segs[14];  // ¿ here, because used by MGC Newgame_Screen(), but, also used by WZD Magic_Screen() ?





// WZD dseg:599C
char hlpentry_lbx_file[] = "hlpentry";






// WZD dseg:6E9E
int16_t tax_unrest_pct_table[7] = {0, 10, 20, 30, 45, 60, 75};





// WZD dseg:6ED0                                                 ¿ BEGIN: Cityscape - Initialized Data ?

// WZD dseg:6ED0
// drake178: CTY_EnchantAnimStage
int16_t cityscape_bldg_anim_ctr = 0;

// WZD dseg:6ED2
int16_t cityscape_water_anim_ctr = 0;

// WZD dseg:6ED4
int16_t cityscape_wall_anim_ctr = 0;

// WZD dseg:6ED6
int16_t IDK_cityscape_vanish_percent = 100;

// WZD dseg:6ED8
struct s_BLDG_CR cityscape_cr[5][3] =
{
    {
        {  0, -12,  18,   0 },
        {  5, -20,  24, -13 },
        { 19, -13,  24,  -7 },
    },
    {
        {  0, -14,  29,   0 },
        {  4, -17,  31,  -4 },
        {  7, -21,  35,  -6 },
    },
    {
        {  0, -15,  19,   0 },
        {  6, -20,  23,  -6 },
        { 10, -25,  30, -11 },
    },
    {
        {  0, -14,  29,   0 },
        {  6, -19,  34,  -5 },
        { 11, -25,  39, -11 },
    },
    {
        {  6, -41,  30,  -3 },
        { 11, -47,  30, -42 },
        { 25, -41,  30,  -9 },
    }

};

// WZD dseg:6F5E                                                 ¿ END: Cityscape - Initialized Data ?





// WZD dseg:6FFE
// AKA Map_LastDraw_X
int16_t prev_map_x = ST_UNDEFINED;
// WZD dseg:7000
// AKA Map_LastDraw_Y
int16_t prev_map_y = ST_UNDEFINED;
// WZD dseg:7002
// AKA OVL_NewMapSustain
int16_t map_sustain = 2;





// WZD dseg:76DA                                                 BEGIN: ovr014
// WZD dseg:76DA                                                 BEGIN: graphics

// WZD dseg:76E0                                                 END: graphics
// WZD dseg:76E2                                                 ¿ Init ?





// WZD dseg:7846 68 35 68 35                                     random_seed dd 35683568h                ; DATA XREF: Set_Random_Seed+6w ...

// MoX_Util  // WZD dseg:784A 01 00 02 00 04 00 08 00 10 00 20 00 40 00 80 00 bit_field_test_bits dw 1, 10b, 100b, 1000b, 10000b, 100000b, 1000000b, 10000000b
// MoX_Util  // WZD dseg:784A                                                                                         ; DATA XREF: Test_Bit_Field_+1Cr ...
// MoX_Util  uint16_t bit_field_test_bits[8] = {
// MoX_Util      0x0001,
// MoX_Util      0x0002,
// MoX_Util      0x0004,
// MoX_Util      0x0008,
// MoX_Util      0x0010,
// MoX_Util      0x0020,
// MoX_Util      0x0040,
// MoX_Util      0x0080
// MoX_Util  };

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
int16_t help_list_active = ST_FALSE;

// WZD dseg:8298
// AKA _help_count
int16_t help_list_count = 0;

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
int16_t g_TimeStop_PlayerNum;

// WZD dseg:912C
struct s_SPELL_DATA * spell_data_table;

// WZD dseg:9130                                                 ; unsigned int IMG_GAME_SpellAnim
// WZD dseg:9130 00 00                                           IMG_GAME_SpellAnim@ dw 0                ; DATA XREF: GAME_LearnSpellAnim+374w ...
// WZD dseg:9130                                                                                         ; appended into the LBX_Sandbox_Segment
// WZD dseg:9132 00 00                                           SND_SpellCast@ dw 0                     ; DATA XREF: GAME_LoadSpellSound+7Cw ...
// WZD dseg:9132                                                                                         ; appended into World_Data@ during combat, or the
// WZD dseg:9132                                                                                         ; Sandbox overland (-1 if none or SFX are disabled)
// WZD dseg:9134 00 00                                           SND_Spell_Music@ dw 0                   ; DATA XREF: GAME_LearnSpellAnim+39w ...

// WZD dseg:9136                                                 ? BEGIN:  - Uninitialized Data ?

// WZD dseg:9136
struct s_ITEM * _ITEMS;

// WZD dseg:913A
SAMB_ptr TBL_Premade_Items;

// WZD dseg:913E
// drake178: IMG_USW_Items ITEM_ICONS
SAMB_ptr item_icons_seg[116];









// WZD dseg:9226
// MoO2  Module: Mox  _global_combat_data_
struct s_BATTLE_UNIT * global_battle_unit;                       // alloc in Allocate_Data_Space()

// WZD dseg:922A
// MoO  Module: Mox  _combat_data_
// WZD dseg:922A 00 00 00 00                                     _combat_data_ dd 0                      ; DATA XREF: USW_Build_Effect_List+AB7r ...
struct s_BATTLE_UNIT * battle_units;                            // alloc in IDK_Combat_Allocate() and CMB_LoadResources()

// WZD dseg:922E
// ; 3 LBX_Alloc_Next paragraphs, sandbox segment; the first 30 (1Eh) bytes are zeroed when allocated
struct s_COMBAT_ENCHANTMENTS * combat_enchantments;             // alloc in IDK_Combat_Allocate() and CMB_LoadResources()



// WZD dseg:9232
struct s_HEROES * _HEROES2[NUM_PLAYERS];


// WZD dseg:924A 00 00                                           CMB_AI_Fled dw 0                        ; DATA XREF: Tactical_Combat__WIP:loc_75ED7w ...
// WZD dseg:924A                                                                                         ; now 0 (non-strategic battle init)
// WZD dseg:924A                                                                                         ; set to 1 if the AI decides to flee
// WZD dseg:924C 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+IMG_CMB_ScanIcons@ CSCAN_ICONS <0>      ; DATA XREF: CMB_LoadResources+39Bw ...
// WZD dseg:924C 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; array of 18 pointers to sandbox appended images
// WZD dseg:9270 00 00                                           CMB_WallRise_Frame dw 0                 ; DATA XREF: CMB_WallRise_Anim:loc_B6D7Ew ...
// WZD dseg:9272 00 00                                           CMB_WallRise_Going dw 0                 ; DATA XREF: CMB_WallRise_Anim+35w ...
// WZD dseg:9272                                                                                         ; set to 0 at the beginning of combat map creation
// WZD dseg:9272                                                                                         ; 1 branches wall entity creation (unknown IMG array)

// WZD dseg:9274
// ; 15Ch LBX_Alloc_First paragraphs, sandbox segment
struct s_BATTLEFIELD * battlefield;

// WZD dseg:9278 00 00                                           CMB_ActiveUnitFrameY dw 0               ; DATA XREF: CMB_SetActionCursor+48w ...
// WZD dseg:927A 00 00                                           CMB_ActiveUnitFrameX dw 0               ; DATA XREF: CMB_SetActionCursor+33w ...
// WZD dseg:927C 00 00                                           CMB_ActiveUnitFrame dw 0                ; DATA XREF: Tactical_Combat__WIP+246w ...
// WZD dseg:927E 00 00                                           CMB_TargetFrame_Y dw 0                  ; DATA XREF: Tactical_Combat__WIP+92Dw ...
// WZD dseg:9280 00 00                                           CMB_TargetFrame_X dw 0                  ; DATA XREF: Tactical_Combat__WIP+915w ...
// WZD dseg:9282 00 00                                           CMB_TargetFrame dw 0                    ; DATA XREF: Tactical_Combat__WIP+24Cw ...

// WZD dseg:9284
int16_t OVL_Action_Type;
// WZD dseg:9286
int16_t OVL_Action_Structure;

// WZD dseg:9288 00                                              db    0
// WZD dseg:9289 00                                              db    0
// WZD dseg:928A 00                                              db    0
// WZD dseg:928B 00                                              db    0
// WZD dseg:928C 00                                              db    0
// WZD dseg:928D 00                                              db    0
// WZD dseg:928E 00                                              db    0
// WZD dseg:928F 00                                              db    0
// WZD dseg:9290 00                                              db    0
// WZD dseg:9291 00                                              db    0
// WZD dseg:9292 00                                              db    0
// WZD dseg:9293 00                                              db    0

// WZD dseg:9294 00 00                                           RP_AI_UnsetRealmVar1 dw 0               ; DATA XREF: AITP_DispelMagic:loc_8B8A0r ...
// WZD dseg:9296 00 00                                           RP_AI_UnsetRealmVar2 dw 0               ; DATA XREF: AI_EvaluateCmbtSpell+141r
// WZD dseg:9298 00 00 00 00 00 00 00 00 00 00                   AI_DEFR_UnitRealms dw 5 dup(0)          ; DATA XREF: AITP_DispelMagic:loc_8B878o ...
// WZD dseg:9298                                                                                         ; array of 5 percentages of the total
// WZD dseg:92A2 00 00 00 00 00 00 00 00 00 00                   AI_ATKR_UnitRealms dw 5 dup(0)          ; DATA XREF: AITP_DispelMagic+2Co ...
// WZD dseg:92A2                                                                                         ; array of 5 percentages of the total
// WZD dseg:92AC 00 00                                           AI_DEFR_RealmFlags dw 0                 ; DATA XREF: AITP_DispelMagic:loc_8B872r ...
// WZD dseg:92AE 00 00                                           AI_ATKR_RealmFlags dw 0                 ; DATA XREF: AITP_DispelMagic:loc_8B87Dr ...








// WZD dseg:92B0
// drake178: cityscape_bldgs
struct s_BLDG * cityscape_bldgs;

// WZD dseg:92B4
int16_t cityscape_bldg_count;

// WZD dseg:92B6
// drake178: IMG_USW_GrassBase@
SAMB_ptr unit_grass_diamond_seg;

// WZD dseg:92B8
// drake178: IMG_CTY_Buildings BLDNG_GFX
SAMB_ptr bldg_picts_seg[35];

// WZD dseg:92FE
// drake178: IMG_CTY_CITYSPL4
SAMB_ptr IDK_CITYSPL4_city_walls_build_seg;

// WZD dseg:9300
// drake178: IMG_CTY_TradeGoods@
SAMB_ptr building_trade_goods_seg;

// WZD dseg:9302
// drake178: IMG_CTY_Housing_Std@, IMG_CTY_Housing_Tre@, IMG_CTY_Housing_Mnd@
SAMB_ptr bldg_housing_seg[3];

// WZD dseg:9308
SAMB_ptr cityscape_fortress_seg;

// WZD dseg:930A
SAMB_ptr cityscape_city_walls_seg;

// WZD dseg:930C 00 00                                           IMG_CTY_AltarofBtl@ dw 0                ; DATA XREF: GFX_Swap_AppndCtScap+498w ...
// WZD dseg:930E 00 00                                           IMG_CTY_EvilPresnc@ dw 0                ; DATA XREF: GFX_Swap_AppndCtScap+4C6w ...
// WZD dseg:9310 00 00                                           IMG_CTY_Dark_Ritual@ dw 0               ; DATA XREF: GFX_Swap_AppndCtScap+4AFw ...
// WZD dseg:9312 00 00                                           IMG_CTY_Astral_Gate@ dw 0               ; DATA XREF: GFX_Swap_AppndCtScap+481w ...
// WZD dseg:9314 00 00                                           IMG_CTY_StreamofLif@ dw 0               ; DATA XREF: GFX_Swap_AppndCtScap+46Aw ...
// WZD dseg:9316 00 00                                           IMG_CTY_Earth_Gate@ dw 0                ; DATA XREF: GFX_Swap_AppndCtScap+453w ...

// WZD dseg:9318
SAMB_ptr cityscape_summon_circle_seg;

// WZD dseg:931A
SAMB_ptr cityscape_rocks_seg[3];

// WZD dseg:9320
SAMB_ptr cityscape_trees_seg[3];

// WZD dseg:9326
// drake178: IMG_CTY_Scap_Houses@[15]
SAMB_ptr cityscape_houses_seg[15];

// WZD dseg:9344
// drake178: IMG_CTY_Scap_Roads
SAMB_ptr cityscape_roads_seg;

// WZD dseg:9346 00 00                                           IMG_CTY_WallofDark@ dw 0                ; DATA XREF: GFX_Swap_AppndCtScap+30Fw ...
// WZD dseg:9348 00 00                                           IMG_CTY_Unk dw 0
// WZD dseg:934A 00 00                                           IMG_CTY_WallofFire@ dw 0                ; DATA XREF: GFX_Swap_AppndCtScap+2F8w ...
// WZD dseg:934C 00 00                                           IMG_CTY_Consecrate@ dw 0                ; DATA XREF: GFX_Swap_AppndCtScap+2E1w ...
// WZD dseg:934E 00 00                                           IMG_CTY_Prosperity@ dw 0                ; DATA XREF: GFX_Swap_AppndCtScap+2CAw ...
// WZD dseg:9350 00 00                                           IMG_CTY_Inspiratn@ dw 0                 ; DATA XREF: GFX_Swap_AppndCtScap+2B3w ...
// WZD dseg:9352 00 00                                           IMG_CTY_NatsEye@ dw 0                   ; DATA XREF: GFX_Swap_AppndCtScap+29Cw ...
// WZD dseg:9354 00 00                                           IMG_CTY_SorcWard@ dw 0                  ; DATA XREF: GFX_Swap_AppndCtScap+285w ...
// WZD dseg:9356 00 00                                           IMG_CTY_LifeWard@ dw 0                  ; DATA XREF: GFX_Swap_AppndCtScap+26Ew ...
// WZD dseg:9358 00 00                                           IMG_CTY_NatWard@ dw 0                   ; DATA XREF: GFX_Swap_AppndCtScap+257w ...
// WZD dseg:935A 00 00                                           IMG_CTY_ChaosWard@ dw 0                 ; DATA XREF: GFX_Swap_AppndCtScap+240w ...
// WZD dseg:935C 00 00                                           IMG_CTY_DeathWard@ dw 0                 ; DATA XREF: GFX_Swap_AppndCtScap+229w ...

// WZD dseg:935E
SAMB_ptr cityscape_famine_mask_seg;

// WZD dseg:9360
// drake178: IMG_CTY_Arc_CRift@
SAMB_ptr cityscape_background_arcanus_chaosrift_seg;

// WZD dseg:9362
// drake178: IMG_CTY_Arc_HLight@
SAMB_ptr cityscape_background_arcanus_alkar_seg;

// WZD dseg:9364
// drake178: IMG_CTY_Arc_CDark@
SAMB_ptr cityscape_background_arcanus_darkcloud_seg;

// WZD dseg:9366
// drake178: IMG_CTY_Arc_MtnBG@
SAMB_ptr cityscape_background_arcanus_mountain_seg;

// WZD dseg:9368
// drake178: IMG_CTY_Arc_HillBG@
SAMB_ptr cityscape_background_arcanus_hills_seg;

// WZD dseg:936A
// drake178: IMG_CTY_Arc_Ocean@
SAMB_ptr cityscape_background_arcanus_ocean_seg;

// WZD dseg:936C
// drake178: IMG_CTY_Arc_River@
SAMB_ptr cityscape_background_arcanus_river_seg;

// WZD dseg:936E
SAMB_ptr cityscape_cursedland_mask_seg;
// WZD dseg:9370
SAMB_ptr cityscape_livinglands_mask_seg;
// WZD dseg:9372
SAMB_ptr cityscape_gaiasblessing_mask_seg;
// WZD dseg:9374
SAMB_ptr cityscape_flyingfortress_mask_seg;

// WZD dseg:9376
// drake178: IMG_CTY_Arc_BGs@
SAMB_ptr cityscape_background_arcanus_ground_seg;

// WZD dseg:9378
// drake178: IMG_CTY_Myr_River@
SAMB_ptr cityscape_background_myrror_river_seg;

// WZD dseg:937A
// drake178: IMG_CTY_Myr_Ocean@
SAMB_ptr cityscape_background_myrror_ocean_seg;

// WZD dseg:937C 00 00                                           IMG_CTY_Arc_GrassBG@ dw 0               ; DATA XREF: GFX_Swap_AppndCtScap+143w ...

// WZD dseg:937E
// drake178: IMG_CTY_Myr_CRift@
SAMB_ptr cityscape_background_myrror_chaosrift_seg;

// WZD dseg:9380
// drake178: IMG_CTY_Myr_CDark@
SAMB_ptr cityscape_background_myrror_darkcloud_seg;

// WZD dseg:9382
// drake178: IMG_CTY_Myr_HLight@
SAMB_ptr cityscape_background_myrror_alkar_seg;

// WZD dseg:9384 00 00                                           IMG_CTY_Myr_GrassBG@ dw 0               ; DATA XREF: GFX_Swap_AppndCtScap+19Fw ...

// WZD dseg:9386
// drake178: IMG_CTY_Myr_MtnBG@
SAMB_ptr cityscape_background_myrror_mountain_seg;

// WZD dseg:9388
// drake178: IMG_CTY_Myr_HillBG@
SAMB_ptr cityscape_background_myrror_hills_seg;

// WZD dseg:938A
// drake178: IMG_CTY_Myr_BGs@
SAMB_ptr cityscape_background_myrror_ground_seg;

// WZD dseg:938C
struct s_BLDG_DATA * bldg_data_table;

// WZD dseg:9390
SAMB_ptr SND_Music_Segment;



/*
    scouted / scouting - Unit Ability, Unit Special Ability, Building Effect, Spell Effect (City Enchantment)
    ~ scouted_table
    ~ TBL_scouted
    updated per turn and on move
    300 byte bit-field
    Set,Clear,Test Bit Field
    boolean {F,T} flag for Fog-of-War feature
    ~ IsScouted()
*/
// WZD dseg:9392
// drake178: Visibility_Myrror
uint8_t * square_scouted_p0;                // Bit_Field  alloc in Allocate_Data_Space()
// WZD dseg:9396
// drake178: Visibility_Arcanus
uint8_t * square_scouted_p1;                // Bit_Field  alloc in Allocate_Data_Space()



// WZD  dseg:939A 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+IMG_CMB_TerrTiles@ dw 30h dup(0)        ; DATA XREF: CMB_LoadTerrainGFX+D0w ...
// WZD  dseg:939A 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; array of 48 images appended into the EMM TILEX handle
// WZD  dseg:93FA 00                                              db    0
// WZD  dseg:93FB 00                                              db    0
// WZD  dseg:93FC 00                                              db    0
// WZD  dseg:93FD 00                                              db    0
// WZD  dseg:93FE 00                                              db    0
// WZD  dseg:93FF 00                                              db    0
// WZD  dseg:9400 00                                              db    0
// WZD  dseg:9401 00                                              db    0
// WZD  dseg:9402 00                                              db    0
// WZD  dseg:9403 00                                              db    0
// WZD  dseg:9404 00                                              db    0
// WZD  dseg:9405 00                                              db    0
// WZD  dseg:9406 00                                              db    0
// WZD  dseg:9407 00                                              db    0
// WZD  dseg:9408 00 00 00 00 00 00 00 00 00 00 00 00             notify_magic_picts_seg dw 6 dup(0)      ; DATA XREF: Notify_Load+17Bw ...
// WZD  dseg:9408                                                                                         ; 5 appended single images in the LBX_Sandbox_Segment,
// WZD  dseg:9408                                                                                         ; with the sixth (Arcane) left as zero

// WZD  dseg:9414
SAMB_ptr snd_left_button_click;

// WZD  dseg:9416 00                                              db    0
// WZD  dseg:9417 00                                              db    0
// WZD  dseg:9418 00                                              db    0
// WZD  dseg:9419 00                                              db    0

// WZD  dseg:941A
SAMB_ptr snd_standard_button_click;

// WZD  dseg:941C 00 00 00 00 00                                  TF_Unk_40_Planes db 5 dup(0)                             ; 0
// WZD  dseg:941C                                                                                         ; DATA XREF: LD_MAP_TFUnk40_Eval+5Fw
// WZD  dseg:9421 00 00 00 00 00                                  TF_Unk_40_YCoords db 5 dup(0)                             ; 0
// WZD  dseg:9421                                                                                         ; DATA XREF: LD_MAP_TFUnk40_Eval+57w
// WZD  dseg:9426 00 00 00 00 00 00                               TF_Unk_40_XCoords db 6 dup(0)                             ; 0
// WZD  dseg:9426                                                                                         ; DATA XREF: LD_MAP_TFUnk40_Eval+4Cw
// WZD  dseg:942C 00 00                                           TF_Unk_40_Counter dw 0                  ; DATA XREF: LD_MAP_TFUnk40_Eval+6w ...

// WZD  dseg:942E
SAMB_ptr main_lilevent_icons[6];
// WZD  dseg:942E                                                                                         ; array of 6 single-loaded images
// WZD  dseg:942E                                                                                         ; (blue - red - green - bad - good - short)
// WZD  dseg:942E                                                                                         ; not actually used as far as  i can tell...

// WZD  dseg:943A 00 00 00 00 00 00 00 00                         IMG_CTY_Rubble@ dw 4 dup(0)             ; DATA XREF: GFX_Swap_AppndCtScap+5B6w ...
// WZD  dseg:943A                                                                                         ; array of 4 appended reserved EMM headers in
// WZD  dseg:943A                                                                                         ; GFX_Swap_Seg, 2x2 - 2x3 - 3x2 - 3x3 rubble images



/* -2: NEVER, -1: ALWAYS, {0,1,2,3}: frame - draw off, {4,5,6,7}: frame - draw on */
// ? Set_Draw_Active_Stack_...() means this variable is private to ovr067 ?
// WZD dseg:9442
int16_t draw_active_stack_flag;  // AKA Active_Stack_Draw

// WZD dseg:9444
// ¿ MoO2  Module: PLNTSUM  _last_ship_moved ?
// ¿ MoO2  Module: MOX  _last_ship_status ?
/*

*/
int16_t all_units_moved;





// WZD dseg:9446
// drake178: G_OVL_MapVar4
int16_t reset_active_stack;

// WZD dseg:9448 00 00                                           SBK_Candidate_Page dw 0                 ; DATA XREF: G_WLD_StaticAssetRfrsh+67w ...
// WZD dseg:944A 00 00                                           SBK_SomePageSaveVar dw 0                ; DATA XREF: G_WLD_StaticAssetRfrsh+5Bw ...
// WZD dseg:944C 00 00                                           CMB_SpellBookPage dw 0                  ; DATA XREF: G_WLD_StaticAssetRfrsh+61w ...
// WZD dseg:944E 00 00                                           dw 0
// WZD dseg:9450 00 00                                           dw 0
// WZD dseg:9452 00 00                                           _spellbook_small_text dw 0              ; DATA XREF: Spellbook_Load_Small_Pictures+85w ...
// WZD dseg:9454 00 00 00 00 00 00 00 00 00 00 00 00             _spellbook_small_symbols SPBK_ICONS <0> ; DATA XREF: Spellbook_Load_Small_Pictures+67w ...
// WZD dseg:9460 00 00                                           _spellbook_big_seg@ dw 0                   ; DATA XREF: GAME_LearnSpellAnim+16Dw ...
// WZD dseg:9460                                                                                         ; LBXE_LoadReplace into the LBX_Sandbox_Segment
// WZD dseg:9462 00 00                                           dw 0
// WZD dseg:9464 00 00                                           _spellbook_big_right_corner_seg@ dw 0                ; DATA XREF: SBK_PageFlip_Big+79w ...
// WZD dseg:9466 00 00                                           _spellbook_big_left_corner_seg@ dw 0                ; DATA XREF: SBK_PageFlip_Big+62w ...
// WZD dseg:9468 00 00                                           _spellbook_small_right_corner_seg dw 0  ; DATA XREF: Spellbook_Load_Small_Pictures+46w ...
// WZD dseg:9468                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:946A 00 00                                           _spellbook_small_left_corner_seg dw 0   ; DATA XREF: Spellbook_Load_Small_Pictures+2Fw ...
// WZD dseg:946A                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:946C 00 00                                           word_3FF0C dw 0                         ; DATA XREF: sub_BECD9+23w ...
// WZD dseg:946E 00 00                                           _spellbook_small_seg dw 0               ; DATA XREF: Spellbook_Load_Small_Pictures+18w ...
// WZD dseg:946E                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

// WZD dseg:9470
SAMB_ptr _combat_results_scroll_bottom_seg;

// WZD dseg:9472 00 00                                           dw 0
// WZD dseg:9474 00 00                                           dw 0
// WZD dseg:9476 00 00                                           dw 0
// WZD dseg:9478 00 00                                           dw 0
// WZD dseg:947A 00 00                                           dw 0
// WZD dseg:947C 00 00                                           dw 0

// WZD dseg:947E
int16_t mana_staff_locked;

// WZD dseg:9480
int16_t research_staff_locked;

// WZD dseg:9482
int16_t skill_staff_locked;

// WZD dseg:9484
SAMB_ptr m_alchemy_pow2gold_seg;
// WZD dseg:9486
SAMB_ptr m_alchemy_bar_pict_seg;
// WZD dseg:9488
SAMB_ptr m_alchemy_right_arrow_button_seg;
// WZD dseg:948A
SAMB_ptr m_alchemy_left_arrow_button_seg;
// WZD dseg:948C
SAMB_ptr m_alchemy_cancel_button_seg;
// WZD dseg:948E
SAMB_ptr m_alchemy_ok_button_seg;
// WZD dseg:9490
SAMB_ptr UU_IMG_NewG_OKBtn;
// WZD dseg:9492
SAMB_ptr IDK_Alchemy_Pict;
// WZD dseg:9494
SAMB_ptr m_alchemy_star_seg;
// WZD dseg:9496
SAMB_ptr m_alchemy_background_seg;
























// WZD dseg:9498
// drake178: IMG_CTY_RED_Btn@
SAMB_ptr red_button_seg;

// WZD dseg:949A
// drake178: IMG_CTY_RightBldTab@
SAMB_ptr IMG_CTY_RightBldTab;

// WZD dseg:949C
// drake178: IMG_CTY_LeftBldTab@
SAMB_ptr IMG_CTY_LeftBldTab;

// WZD dseg:949E
SAMB_ptr _enemy_city_seg;

// WZD dseg:94A0
// drake178: IMG_CTY_10_Food@
// WZD dseg:94A2
// drake178: IMG_CTY_10_Prod@
// WZD dseg:94A4
// drake178: IMG_CTY_10_Gold@
// WZD dseg:94A6
// drake178: IMG_CTY_10_Power@
// WZD dseg:94A8
// drake178: IMG_CTY_10_Books@
// WZD dseg:94AA
// drake178: IMG_CTY_Neg_10_Gold@
SAMB_ptr city_big_resource_icon_seg[6];

// WZD dseg:94AC
// drake178: IMG_CTY_Neg_10_Food@
SAMB_ptr city_grey_big_bread_icon_seg;

// WZD dseg:94AE
// drake178: IMG_CTY_Rebels@[14]
SAMB_ptr city_rebel_icon_seg[14];

// WZD dseg:94CA
// drake178: IMG_CTY_Farmers@[14]
SAMB_ptr city_farmer_icon_seg[14];


// WZD dseg:94E6
// drake178: IMG_CTY_1_Food@
// WZD dseg:94E8
// drake178: IMG_CTY_1_Prod@
// WZD dseg:94EA
// drake178: IMG_CTY_1_Gold@
// WZD dseg:94EC
// drake178: IMG_CTY_1_Power@
// WZD dseg:94EE
// drake178: IMG_CTY_1_Book@
SAMB_ptr city_lil_resource_icon_seg[5];

// WZD dseg:94F0
// drake178: IMG_CTY_Neg_1_Food@
SAMB_ptr city_grey_lil_bread_icon_seg;



// WZD dseg:94F2
// drake178: IMG_CTY_Workers@[14];
SAMB_ptr city_worker_icon_seg[14];

// WZD dseg:950E                                                 BEGIN: CityList_Screen_Load()

// WZD dseg:950E 00 00                                           citylist_ok_button_seg dw 0             ; DATA XREF: CityList_Screen+101r ...
// WZD dseg:950E                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:9510 00 00                                           citylist_down_button_seg dw 0           ; DATA XREF: CityList_Screen+1FFr ...
// WZD dseg:9510                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:9512 00 00                                           citylist_up_button_seg dw 0             ; DATA XREF: CityList_Screen+16Er ...
// WZD dseg:9512                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:9514 00 00                                           citylist_background_seg dw 0            ; DATA XREF: CityList_Screen:loc_5767Dr ...

// WZD dseg:9514                                                 END: CityList_Screen_Load()

// WZD dseg:9516                                                 BEGIN:  Main Screen Pictures

// WZD dseg:9516
SAMB_ptr next_turn_button_seg;
// MAIN.LBX,58  DESELECT    next turn button..

// WZD dseg:9518
SAMB_ptr survey_background;
// MAIN.LBX,57  MAINSRVY    survey backgrnd

// WZD dseg:951A
SAMB_ptr road_button_border;
// MAIN.LBX,48  C&RBORDR    road button border

// WZD dseg:951C
SAMB_ptr road_background;
// MAIN.LBX,45  MAINROAD    road background

// WZD dseg:951E
SAMB_ptr road_ok_button;
// MAIN.LBX,46  CASTCNCL    road ok button

// WZD dseg:9522
SAMB_ptr deselect_button_blockout;
// MAIN.LBX,44    DESELECT    button blockout

// WZD dseg:9524
SAMB_ptr cast_button_border;
// MAIN.LBX,47  C&RBORDR    cast button border

// WZD dseg:9526
SAMB_ptr cast_cancel_button;
// MAIN.LBX,41  CASTCNCL    cast cancel button

// WZD dseg:9528
SAMB_ptr cast_background;
// MAIN.LBX,40  MAINCAST    cast background

// WZD dseg:952C
SAMB_ptr next_turn_background_seg;
// MAIN.LBX,35  DESELECT    next turn backgrnd

// WZD dseg:952E
SAMB_ptr deselect_background;
// MAIN.LBX,34  DESELECT    deselect backgrnd

// WZD dseg:952E                                                 END:  Main Screen Pictures

// WZD dseg:9530
// drake178: IMG_CTY_EmptyHuts@
SAMB_ptr outpost_dark_icon_seg[3];

// WZD dseg:9536
// drake178: IMG_CTY_FilledHuts@
SAMB_ptr outpost_bright_icon_seg[3];

// WZD dseg:953C
SAMB_ptr outpost_name_background_seg;

// WZD dseg:953E
SAMB_ptr outpost_background_seg;

// WZD dseg:9540
// drake178: IMG_USW_SideBtns_BG@
SAMB_ptr unitview_button_background_seg;

// WZD dseg:9542 00 00                                           dw 0
// WZD dseg:9544 00 00                                           dw 0
// WZD dseg:9546 00 00                                           dw 0

// WZD dseg:9548
// drake178: IMG_OVL_BuildBtn_BG@
SAMB_ptr IMG_OVL_BuildBtn_BG;

// WZD dseg:954A
SAMB_ptr mirrow_screen_books_segs[18];

// WZD dseg:956E
SAMB_ptr mirror_screen_background_seg;

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



// WZD dseg:957A
// ; array of 6 appended reserved EMM headers in GFX_Swap_Seg, each with one item slot image
// item_slot_icons_seg@ ISLT_ICONS <0>
SAMB_ptr item_slot_icons_seg[6];

// WZD dseg:9586
SAMB_ptr hero_ok_button_seg;

// WZD dseg:9588
SAMB_ptr hero_alchemy_button_seg;

// WZD dseg:958A
SAMB_ptr hero_outline_seg;

// WZD dseg:958C
SAMB_ptr hero_background_seg;



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

// WZD dseg:95F0
// drake178: IMG_CTY_HalfRsc@
SAMB_ptr city_half_background_seg;

// WZD dseg:95F2
SAMB_ptr goto_booty_icon;

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





// WZD dseg:9644
// MoO2  Module: MOX  _colony  Address: 02:0018AB18  _colony = Allocate_Space(90250);
int16_t _city_idx;

// WZD dseg:9646 00                                              db    0
// WZD dseg:9647 00                                              db    0

// WZD dseg:9648
SAMB_ptr UnitDraw_WorkArea;  // alloc in MoM_Init_Tables(), 60 PR, 960 DB

// WZD dseg:964A 00                                              db    0
// WZD dseg:964B 00                                              db    0

// WZD dseg:964C
// MoO2: ~== _ship_node
int16_t entities_on_movement_map[120];  //  12 * 10  MAP_WIDTH * MAP_HEIGHT

// dseg:973C                                                 ; unsigned int gsa_BACKGRND_3_IMG_CMB_Bottom_BG
// dseg:973C 00 00                                           gsa_BACKGRND_3_IMG_CMB_Bottom_BG dw 0   ; DATA XREF: Load_Combat_Background_Bottom+12w ...



// WZD dseg:9750                                                 ¿ END: maps / map grid fields - Uninitialized Data ?

// WZD dseg:9752                                                 ? BEGIN: City Screen ?

// WZD dseg:9752 00 00                                           scanned_field__G_CTY_ClickedLabel dw 0  ; DATA XREF: City_Screen+2A9w ...
// WZD dseg:9752
int16_t city_screen_scanned_field;

// WZD dseg:9754
SAMB_ptr _unitlist_bottom_seg;

// WZD dseg:9756
SAMB_ptr _unitlist_background_seg;

// WZD dseg:9758
// drake178: IMG_CTY_NewBuild_BG@
SAMB_ptr city_new_build_notify_grass_seg;

// WZD dseg:975A
// drake178: IMG_CTY_SplScrlDn@
SAMB_ptr city_spell_dn_arrow_button_seg;

// WZD dseg:975C
// drake178: IMG_CTY_SplScrlUp@
SAMB_ptr city_spell_up_arrow_button_seg;

// WZD dseg:975E
SAMB_ptr itemtile_icons_seg[6];

// WZD dseg:976
// XREF: GFX_Swap_AppendItems(); Item_View_Prepare(); Hero_LevelUp_Popup_Draw();
SAMB_ptr item_view_bullet_seg;

// WZD dseg:976C
// XREF: GFX_Swap_AppendItems(); Item_View_Prepare();
SAMB_ptr item_view_background_seg;

// WZD dseg:976E
// drake178: IMG_USW_ItemPowers IPOW_ICONS
// XREF:  GFX_Swap_AppendItems()  USW_Build_Effect_List()
SAMB_ptr item_power_icons_seg[22];

// WZD dseg:979A 00 00                                           IMG_MOODWIZPortrait@ dw 0               ; DATA XREF: IDK_DiplAnim_s6FDA1+4Cr ...
// WZD dseg:979A                                                                                         ; 3 frame image (good, bad, neutral)

// WZD dseg:979C
// drake178: IMG_USW_WaterBase@
SAMB_ptr unit_water_diamond_seg;

// WZD dseg:979E
// drake178: IMG_USW_Portrt_Brdr@
SAMB_ptr IMG_USW_Portrt_Brdr;

// WZD dseg:97A0
// drake178: IMG_USW_1_Gold@
SAMB_ptr gold1_icon_seg;

// WZD dseg:97A2
// drake178: IMG_CTY_Neg_1_Gold@
SAMB_ptr black_gold1_icon_seg;

// WZD dseg:97A4
// drake178: IMG_USW_AbBorders@
SAMB_ptr unitview_specials_borders_seg[8];

// WZD dseg:97B4
// drake178: IMG_USW_ArrowDown@
SAMB_ptr unitview_down_arrow_seg;

// WZD dseg:97B6
// drake178: IMG_USW_ArrowUp@
SAMB_ptr unitview_up_arrow_seg;

// WZD dseg:97B8
// drake178: IMG_USW_Background@
SAMB_ptr unitview_large_background_seg;

// WZD dseg:97BA 00 00                                           
// IMG_USW_UnitHire_BG@ dw 0               ; DATA XREF: GFX_Swap_AppendUView+5Dw ...
SAMB_ptr unitview_small_background_seg;
// WZD dseg:97BA                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

// WZD dseg:97BC
// drake178: IMG_USW_Stats_Gold
SAMB_ptr unitview_stat_gold_icons_seg[15];

// WZD dseg:97DA
// drake178: IMG_USW_Stat_Icons
SAMB_ptr unitview_stat_icons_seg[15];

// WZD dseg:97F8 00                                              
// special_seg db    0                     ; DATA XREF: GFX_Swap_AppendUView+1Dw ...
SAMB_ptr special_seg[145];

// WZD dseg:991A
// drake178: IMG_OVL_EZConfirmBG@
SAMB_ptr lair_confirm_border_seg;

// WZD dseg:991C
// drake178: IMG_OVL_EZBtm_BG@
SAMB_ptr lair_bottom_without_button_seg;

// WZD dseg:991E
// drake178: IMG_OVL_EZBtn_BG@
SAMB_ptr lair_bottom_with_button_seg;

// WZD dseg:9920 00 00                                           CRP_DBG_Alt_T_State dw 0                ; DATA XREF: Main_Screen+47Fr ...

// WZD dseg:9920                                                 ? END: City Screen ?

// WZD dseg:9922                                                 BEGIN:  Main Screen Pictures

// WZD dseg:9922
SAMB_ptr main_background;

// WZD dseg:9924
SAMB_ptr unit_backgrounds[9];

// WZD dseg:9924                                                 END:  Main Screen Pictures

// WZD dseg:9936 00 00                                           city_background_seg dw 0                ; DATA XREF: City_Screen_Load_Pictures+18w ...
// WZD dseg:9938 00 00                                           city_block_out_seg dw 0                 ; DATA XREF: City_Screen_Load_Pictures+B9w ...
// WZD dseg:993A 00 00                                           city_full_resource_seg dw 0             ; DATA XREF: City_Screen_Load_Pictures+A2w ...
// WZD dseg:993C 00 00                                           city_reqd_resource_seg dw 0             ; DATA XREF: City_Screen_Load_Pictures+8Bw ...
// WZD dseg:993E 00 00                                           city_lock_buy_button_seg dw 0           ; DATA XREF: City_Screen_Load_Pictures+D0w ...
// WZD dseg:9940 00 00                                           city_ok_button_seg dw 0                 ; DATA XREF: City_Screen_Load_Pictures+5Dw ...
// WZD dseg:9942 00 00                                           city_buy_button_seg dw 0                ; DATA XREF: City_Screen_Load_Pictures+2Fw ...
// WZD dseg:9944 00 00                                           city_change_button_seg dw 0             ; DATA XREF: City_Screen_Load_Pictures+46w ...

// WZD dseg:9946                                                 BEGIN:  Main Screen Pictures

// WZD dseg:9946
SAMB_ptr main_red_medal_icon;
SAMB_ptr main_gold_medal_icon;
SAMB_ptr main_white_medal_icon;
SAMB_ptr main_adamantium_weapon_icon;
SAMB_ptr main_mithril_weapon_icon;
SAMB_ptr main_magic_weapon_icon;

// Screen Layout: L-R, T-B
//     Done, Patrol, Wait, Build
SAMB_ptr main_lock_purify_button;
SAMB_ptr main_lock_build_button;
SAMB_ptr main_lock_wait_button;
SAMB_ptr main_lock_patrol_button;
SAMB_ptr main_lock_done_button;

SAMB_ptr main_meld_button;
// WZD dseg:995E
SAMB_ptr main_purify_button;
// WZD dseg:9960 00 00                                           IDK_UU_action_C dw 0
// WZD dseg:9962
SAMB_ptr main_build_button;
SAMB_ptr main_done_button;
SAMB_ptr main_wait_button;
SAMB_ptr main_patrol_button;

// ? block of Main Screen - Top Buttons Bar ?
// XREF: Main_Screen_Load_Pictures(); OVL_SetMenuButtons(); OVL_DrawMainMenu()
// ; single-loaded 2 frame image (normal - clicked)
// Code Order:   ?
// Data Order:   S,I,M,C,G,A,P
// Load Order:   G,S,A,C,M,I,P
// LBX Order:    G,S,A,C,M,I,P
// Screen Order: G,S,A,C,M,I,P
SAMB_ptr main_spells_button;
SAMB_ptr main_info_button;
SAMB_ptr main_magic_button;
SAMB_ptr main_cities_button;
SAMB_ptr main_game_button;
SAMB_ptr main_armies_button;
SAMB_ptr main_plane_button;

// XREF: Main_Screen_Load_Pictures(); Draw_Movement_Mode_Icons()
// single-loaded image
// gets indexed like an array of pictures in Draw_Movement_Mode_Icons()
SAMB_ptr move_sail_icon;
SAMB_ptr move_swim_icon;
SAMB_ptr move_mt_icon;
SAMB_ptr move_forest_icon;
SAMB_ptr move_fly_icon;
SAMB_ptr move_path_icon;
SAMB_ptr move_astral_icon;
SAMB_ptr move_wind_icon;
SAMB_ptr move_boot_icon;
// WZD dseg:998A 00 00                                           IDK_UU_Cavalry dw 0

// dseg:998A END:  Main Screen Pictures



SAMB_ptr movement_mode_icons[10];  // {0,...,9} 10 icons




// WZD dseg:998C
// GUI_SmallWork_IMG@ dw 0
    // 96h paragraphs used for building GUI notification images
    // (although the pointer variable is also used for building combat figure images, after which it is reassigned)
// TODO  confirm this is only used for 'bitmaps'
SAMB_ptr GfxBuf_2400B;

// WZD dseg:998E
SAMB_ptr _screen_seg;
// allocated in MoM_Init_Tables()

// WZD dseg:9990
// _current_screen

/*
only used twice, from 'Main Screen'

PageFlip_FX()
    PageFlipEffect == 4
    RP_VGA_GrowOutFlip(GrowOutLeft, GrowOutTop, GrowOutFrames, (_screen_seg + 400));
*/
// WZD dseg:9992
int16_t GrowOutFrames;
// WZD dseg:9994
int16_t GrowOutTop;
// WZD dseg:9996
int16_t GrowOutLeft;

// MoO2  Module: EVENTS  data (0 bytes) _last_event_year Address: 02:00192CE8

// MoO2  Module: EVENTS  data (0 bytes) _event_data Address: 02:00192BA4

// WZD dseg:9998
// AKA TBL_Events
/*
    Allocate_Data_Space()
        Allocate_Space(7)
            7 PR 112 B
            type? 1-byte,2-byte signed,unsigned
            Hrrrmm... all the indexing is in evens, so 2-byte?
    Load_SAVE_GAME()
        fread 1 of 100 
*/
// struct s_EVENT_DATA * events_table;
// int16_t * events_table;
struct s_EVENT_DATA * events_table;

int16_t last_event_turn_2;
struct s_EVENT_DATA_2 * events_table_2;


// WZD dseg:999C
/*
set by Build_Unit_Stack()
*ALWAYS* use `itr_stack`
*/
int16_t _unit_stack_count;
// WZD dseg:999E
struct s_STACK _unit_stack[MAX_STACK];



// WZD dseg:99C2
// drake178: MSG_CityLost_Count
int8_t MSG_CityLost_Count;

// WZD dseg:99C3
// drake178: MSG_CityLost_Names db 118h dup(0)
char MSG_CityLost_Names[280];

// WZD dseg:9ADB
// drake178: MSG_CityGained_Count
int8_t MSG_CityGained_Count;

// WZD dseg:9ADC
// drake178: MSG_CityGained_Array
int8_t MSG_CityGained_Array[20];

// WZD dseg:9AF0
// drake178: MSG_BuildDone_Count
int8_t g_bldg_msg_ctr;

// WZD dseg:9AF1 00                                              db    0

// WZD dseg:9AF2
// drake178: MSG_BuildDone_Array Building_Done_Msg_Item 14h dup(<0>)
/*
    not just "Message", not just "Building", not just "Complete"
...
City_Cancel_Production() sets bldg_type_idx to a negative value ... ends up at "...can no longer produce..."
    doesn't change `construction` though, so must trigger some *action*
*/
struct s_MSG_BUILDING_COMPLETE MSG_Building_Complete[20];

// WZD dseg:9B42
// drake178: MSG_BldLost_Count
int8_t MSG_BldLost_Count;

// WZD dseg:9B43 00                                              db    0

// WZD dseg:9B44
// drake178: MSG_BldLost_Array Building_Lost_Msg_Item 14h dup(<0>)
struct s_MSG_BUILDING_LOST MSG_BldLost_Array[20];

// WZD dseg:9B94
// drake178: MSG_UnitLost_Count
int8_t MSG_UnitLost_Count;

// WZD dseg:9B95 00                                              db    0

// WZD dseg:9B96
// drake178: MSG_UnitLost_Array Unit_Lost_Msg_Item 14h dup(<0>)
struct s_MSG_UNIT_LOST MSG_UnitLost_Array[20];

// WZD dseg:9BE6
// drake178: MSG_UnitKilled_Count
int8_t MSG_UnitKilled_Count;

// WZD dseg:9BE7 00                                              db    0

// WZD dseg:9BE8
// drake178: MSG_UnitKilled_Array Unit_Killed_Msg_Item 14h dup(<0>)
struct s_MSG_UNIT_KILLED MSG_UnitKilled_Array[20];

// WZD dseg:9C38
// drake178: MSG_CityGrowth_Count
/*
    1-byte, signed
*/
int8_t MSG_CityGrowth_Count;

// WZD dseg:9C39
// drake178: MSG_CityGrowth_Array
int8_t MSG_CityGrowth_Array[20];  // 1-byte, unsigned

// WZD dseg:9C4D
// drake178: MSG_CityDeath_Count
int8_t MSG_CityDeath_Count;

// WZD dseg:9C4E
// drake178: MSG_CityDeath_Array
uint8_t MSG_CityDeath_Array[20];  // 1-byte, unsigned

// WZD dseg:9C62
// drake178: MSG_UEsLost_Count
int8_t MSG_UEsLost_Count;

// WZD dseg:9C63
// drake178: MSG_UEsLost_Array db 14h dup(0)   
uint8_t MSG_UEsLost_Array[20];  // 1-byte, unsigned

// WZD dseg:9C77
// drake178: MSG_CEsLost_Count
int8_t MSG_CEsLost_Count;

// WZD dseg:9C78
// drake178: MSG_CEsLost_Array db 14h dup(0)   
uint8_t MSG_CEsLost_Array[20];  // 1-byte, unsigned

// WZD dseg:9C8C
// drake178: MSG_GEs_Lost
int8_t MSG_GEs_Lost;

// WZD dseg:9C8D 00                                              db    0



// WZD dseg:9C90
SAMB_ptr TBL_OvlMovePaths_EMS;

// WZD dseg:9C94
// CONTX_Arc_NmeStrMap
// WZD dseg:9C98
// CONTX_Myr_NmeStrMap

// WZD dseg:9C9C
// drake178: TBL_Catchments_EMS
uint8_t * city_area_bits;                       // alloc'd in Allocate_Data_Space()

// WZD dseg:9CA0
// drake178: TBL_SharedTiles_EMS
uint8_t * city_area_shared_bits;                // alloc'd in Allocate_Data_Space()

// WZD dseg:9CA4
// TBL_TempMoveMap_EMS
struct s_MOVE_PATH * movepath_cost_map;

// WZD dseg:9CA8
// COL_MinimapTiles

// WZD dseg:9CAC
// drake178: TBL_MoveMaps_EMS
struct s_MOVE_MODE_COST_MAPS * movement_mode_cost_maps;


// WZD dseg:9CB0
// drake178: TBL_Scouting
// MoO2  Module: PLNTSUM  data (0 bytes) _planet_explored  Address: 02:001945B2
// 1oom: ¿ ?
/*
    302 Paragraphs, 4832 Bytes
    4800 bytes used - 1-byte values, 2400 world squares, 2 planes
    {F,T}

*/
uint8_t * _square_explored;                     // load in Load_SAVE_GAM()


// WZD dseg:9CB4
/*
8 bit bitfield
0 0 0 0 0 0 0 0
| | | | | | | |
| | | | | \-\-\- {2,1,0}: Volcano's Player Number (player_idx + 1) {1,2,3,4,5,6,7}
| | | | | 
| | | | | 
| | | | \- 3: regular road
| | | \- 4: enchanted road
| | \- 5: corruption
| \- 6: unknown
\- 7: unknown

// 0x01
// 0x02
// 0x04
#define TF_Road         0x08
#define TF_Enc_Road     0x10
#define TF_Corruption   0x20
#define TF_Unk_40       0x40
#define TF_NoRoad80     0x80  // ~== -1 ST_UNDEFINED
#define TF_AnyRoad      0x18

TILE_MakeGrassland() ...Forest,Desert,Mountain,Hill
    and     al, 11111000b
    AKA and     al, not 111b

Volcano_Counts()

*/
/*
1-byte, unsigned
*/
uint8_t * _map_square_flags;                // load in Load_SAVE_GAM()

// WZD dseg:9CB8  
SAMB_ptr TBL_Terr_Specials;                 // load in Load_SAVE_GAM()

// WZD dseg:9CBC  
// WZD dseg:9CC0
// WZD dseg:9CC4  
// WZD dseg:9CC8
// WZD dseg:9CCC
struct s_CITY * _CITIES;
struct s_LAIR * _LAIRS;
struct s_TOWER * _TOWERS;
/*
array of struct s_FORTRESS
3 PR = 48 B;  actual: 6 * sizeof(struct s_FORTRESS) = 24
allocated in Allocate_Data_Space()
populated in Load_SAVE_GAM()
*/
struct s_FORTRESS * _FORTRESSES;
struct s_NODE * _NODES;

// WZD dseg:9CD0
uint8_t * TBL_Landmasses;  // 1-byte, unsigned;  302 PR, 4832 B, in World_Data

// WZD dseg:9CD4
SAMB_ptr UU_TBL_2;
// WZD dseg:9CD8
SAMB_ptr UU_TBL_1;

// WZD dseg:9CDC
// AKA TBL_Maps;
// SAMB_ptr _world_maps;
/*
Allocate_Data_Space()
    _world_maps = (uint8_t *)Allocate_Next_Block(World_Data, 602);         // 602 PR, 9632 B

STU_DBG.C
    uint8_t * DBG_ORIG__world_maps;

*/
uint8_t * _world_maps;

// WZD dseg:9CE0
int8_t * TBL_Unrest[NUM_RACES];

// WZD dseg:9D18
// drake178: IMG_CTY_Bldngs_Wall@
SAMB_ptr cityscape_big_city_wall_seg;

// WZD dseg:9D1A
uint16_t tmp_World_Data_Paras;

// WZD dseg:9D1C
SAMB_ptr World_Data_Extra;

// WZD dseg:9D1E
// ? blocks, alloc in MoM_Init_Tables()
SAMB_ptr GFX_Swap_Seg;

// WZD dseg:9D20
SAMB_ptr World_Data;

// WZD dseg:9D22
uint16_t grand_vizier;


// WZD dseg:9EC2
// SAMB_ptr TBL_Units;
// struct s_UNIT _UNITS[UNIT_COUNT_MAX];  // 1009 * sizeof(32)
// struct s_UNIT _UNITS[];
struct s_UNIT * _UNITS;

// WZD dseg:9EC6
struct s_INACTV_HERO * hero_names_table;

// WZD dseg:9ECA
struct s_WIZARD _players[6];
// struct s_WIZARD * _players;


// WZD dseg:BB7A
// // // struct s_HELP_FIELD _help_entries[50];
// // uint8_t _help_entries[500];
// int16_t _help_entries[250];
// char _help_entries[500];
// struct s_HELP_LIST _help_entries;
struct s_HELP_FIELD _help_entries[50];


// WZD dseg:BD6E 
// config_mom s_CONFIG_MOM
// WZD dseg:BD80 
// UU_IMG_OVL_WorkMark@

// WZD dseg:974A
// WZD dseg:974C
int16_t _prev_world_y;      // AKA G_OVL_MapDisplay_Y
int16_t _prev_world_x;      // AKA G_OVL_MapDisplay_X

// WZD dseg:BD82 
// WZD dseg:BD84 
/*

set in Next_Unit_Nearest_Available()
    _active_world_x = _UNITS[_unit].wx;
    _active_world_y = _UNITS[_unit].wy;

*/
int16_t _active_world_y;    // AKA OVL_Map_CenterY
int16_t _active_world_x;    // AKA OVL_Map_CenterX

// WZD dseg:BD86 
int16_t _map_plane;     // AKA _world_plane
// WZD dseg:BD88 
int16_t _map_y;         // AKA _curr_world_y
// WZD dseg:BD8A 
int16_t _map_x;         // AKA _curr_world_x

// WZD dseg:BD8C
/*
¿ Curren Player?
¿ ~ Hot-Seat Multi-Player ?
MoO2
    _PLAYER_NUM
*/
int16_t _human_player_idx = HUMAN_PLAYER_IDX;

// WZD dseg:BD8E
// TODO  why,what,where is `_unit`?  ¿ MoO2 _g_ship ? "active"/"current"?
/*
set by 
*/
int16_t _unit;
int16_t _turn;
/*
*always* use itr_units
*/
int16_t _units;                             // MoO2 _NUM_SHIPS
/*
*always* use itr_cities
*/
int16_t _cities;
int16_t _difficulty;                        // enum e_Difficulty {Intro, Easy, Normal, Hard, Impossible}
int16_t _magic;
int16_t _landsize;
/*
*always* use itr_players
*/
int16_t _num_players;                       // MoO2 _NUM_PLAYERS    New Game: magic_set.opponents + 1
struct s_GAME_DATA game_data;
// WZD dseg:BD9E
// magic_set s_MAGIC_SET

// ovr150 MainScr_Maps  // WZD dseg:CB5C
// ovr150 MainScr_Maps  // AKA OVL_NewMapDrawing
// ovr150 MainScr_Maps  int16_t draw_map_full;  //; determines whether non-animated terrain tiles will be redrawn or not





// WZD dseg:BF70
int16_t city_resources_rows_fields[7];

// WZD dseg:BF7E
int16_t * city_population_row_fields;

// WZD dseg:BF80
int16_t m_city_no_buy;

// WZD dseg:BF82
int16_t * city_screen_required_buildings_list;

// WZD dseg:BF84
// drake178: CTY_EScrollDn_Label
int16_t city_dn_button;

// WZD dseg:BF86
// drake178: CTY_EScrollUp_Label
int16_t city_up_button;

// WZD dseg:BF88
int16_t UU_CityScreen_Field;

// WZD dseg:BF8A
int16_t city_map_wy;

// WZD dseg:BF8C
int16_t city_map_wx;

// WZD dseg:BF8E
int16_t city_cityscape_field_count;

// WZD dseg:BF90
byte_ptr city_cityscape_fields;

// WZD dseg:BF92
// drake178: CTY_EnchDisplayCount
int16_t city_enchantment_display_count;

// WZD dseg:BF94
// drake178: N/A
int16_t UU_CityScreen_Field_Something;

// WZD dseg:BF96
// drake178: CTY_EnchantScrolling
int16_t city_enchantment_display_scroll_flag;

// WZD dseg:BF98
// drake178: CTY_FirstShownEnch
int16_t city_enchantment_display_first;

// WZD dseg:BF9A
int16_t city_enchantment_list_count;

// WZD dseg:BF9C
// drake178: CTY_EnchantOwners
int16_t * city_enchantment_owner_list;

// WZD dseg:BF9E
// drake178: CTY_EnchantList
int16_t * city_enchantment_list;

// WZD dseg:BFA0
int16_t * city_enchantment_fields;

// WZD dseg:BFA2
int16_t _garrison_window_start_y;

// WZD dseg:BFA4
int16_t _garrison_window_start_x;

// WZD dseg:BFA6
int16_t city_screen_esc_hotkey;

// WZD dseg:BFA8
int16_t city_screen_ok_button;

// WZD dseg:BFAA
int16_t city_sceen_change_button;

// WZD dseg:BFAC
int16_t city_screen_buy_button;

// WZD dseg:BFAE
char * m_city_screen_product_name;

// WZD dseg:BFB0
int16_t m_city_production_cost;

// WZD dseg:BFB2
int16_t m_city_n_turns_to_produce;

// WZD dseg:BFB4
int16_t cityscreen_city_built_bldg_idx;

// WZD dseg:BFB6
/*
    {1: CityList Screen, 2: City Screen}
*/
int16_t production_screen_return_screen;

// WZD dseg:BFB8 00 00                                           dw 0

// WZD dseg:BFBA
char * GUI_String_2;

// WZD dseg:BFBC
char * GUI_String_1;

// WZD dseg:BFBE 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+word_42A5E dw 0Fh dup(0)                ; DATA XREF: sub_4D5EA+47w
// WZD dseg:BFDC 00 00                                           Retn_Struct@ dw 0  // ; FOOD_BRKDN *Retn_Struct
// WZD dseg:BFDE 00 00                                           word_42A7E dw 0                         ; DATA XREF: sub_4D5EA+B1w ...
// WZD dseg:BFE0 00 00                                           word_42A80 dw 0                         ; DATA XREF: sub_4D5EA+ABw ...
// WZD dseg:BFE2 00 00                                           word_42A82 dw 0                         ; DATA XREF: sub_4D5EA+7Bw ...
// WZD dseg:BFE4 00 00                                           IMG_Seg dw 0                            ; DATA XREF: sub_4D5EA+64w ...
// WZD dseg:BFE6 00 00                                           word_42A86 dw 0                         ; DATA XREF: sub_4D5EA+Bw ...
// WZD dseg:BFE8 00 00                                           m_troop_fields dw 0    ; DATA XREF: Enemy_City_Screen+1D3r ...
// WZD dseg:BFEA 00 00                                           IDK_EnemyCityScreen_UnitStack_1 dw 0    ; DATA XREF: Enemy_City_Screen:loc_4A454r ...
// WZD dseg:BFEC 00 00                                           RecTotal dw 0                           ; DATA XREF: Enemy_City_Screen+68o ...
// WZD dseg:BFEE 00 00                                           start_y dw 0                            ; DATA XREF: Enemy_City_Screen:loc_4A43Fw ...
// WZD dseg:BFF0 00 00                                           IDK_EnemyCityScreen_xstart dw 0         ; DATA XREF: Enemy_City_Screen:loc_4A439w ...



// WZD dseg:BFF2                                                 ¿ BEGIN:  ovr056  Production Screen ?

// WZD dseg:C03C                                                 ¿ END:  ovr056  Production Screen ?



// WZD dseg:C03E                                                 BEGIN: Main_Screen

// WZD dseg:C03E 00 00                                           CRP_OverlandVar_2 dw 0                  ; DATA XREF: Main_Screen+60w
// WZD dseg:C040 00 00                                           CRP_OverlandVar_4 dw 0                  ; DATA XREF: Main_Screen:loc_4FC07r ...
// WZD dseg:C042 00 00                                           OVL_MapVar3 dw 0                        ; DATA XREF: Main_Screen+6Cw ...
// WZD dseg:C044 00 00                                           CRP_OVL_Obstacle_Var1 dw 0              ; DATA XREF: Main_Screen+66w ...
// WZD dseg:C046 00 00                                           CRP_OVL_UU_Control_1 dw 0               ; DATA XREF: MainScreen_Add_Fields+16w ...
// WZD dseg:C048 00 00                                           CRP_OVL_UU_Control_2 dw 0               ; DATA XREF: MainScreen_Add_Fields+10w ...
// WZD dseg:C04A 00 00                                           CRP_OVL_UU_Control_3 dw 0               ; DATA XREF: MainScreen_Add_Fields+Aw ...
// WZD dseg:C04C 00 00                                           CRP_OVL_UU_Control_4 dw 0               ; DATA XREF: MainScreen_Add_Fields+4w ...
// WZD dseg:C04E 00 00                                           special_action_flag dw 0                ; DATA XREF: Main_Screen+727r ...
// WZD dseg:C04E                                                                                         ; set to indicate any extra action possible for the
// WZD dseg:C04E                                                                                         ; selected stack
// WZD dseg:C04E                                                                                         ;   -1: no extra action possible
// WZD dseg:C04E                                                                                         ;    0: road building possible
// WZD dseg:C04E                                                                                         ;    1: settling possible
// WZD dseg:C04E                                                                                         ;    2: purifying possible
// WZD dseg:C04E                                                                                         ;    9: melding possible
// WZD dseg:C050 00 00                                           _next_turn_button dw 0                  ; DATA XREF: Main_Screen:@@Check_Input_NextTurnButtonr ...
// WZD dseg:C052 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+g_unit_window_fields dw 9 dup(0)          ; DATA XREF: IDK_CityScreen_AddFields+2Cw ...
// WZD dseg:C064 00 00                                           CRP_OverlandVar_3 dw 0                  ; DATA XREF: Main_Screen+9Ar ...
// WZD dseg:C066 00 00                                           _active_stack_path_length dw 0                    ; DATA XREF: Main_Screen+CBDr ...

// WZD dseg:C068
/*
; set to 0 after display-sorting the active stack
; set to 1 if road-building, but the unit is not on any
;   of the plotted line tiles (before returning)
; set to 1 if road-building, and tiles left to do
; set to 1 if moving with path left to go
*/
int16_t _active_stack_has_path;

// WZD dseg:C06A 00 00                                           _done_button dw 0                       ; DATA XREF: Main_Screen:CheckDoneButtonr ...
// WZD dseg:C06C 00 00                                           _wait_button dw 0                       ; DATA XREF: Main_Screen:loc_50419r ...
// WZD dseg:C06E 00 00                                           _patrol_button dw 0                     ; DATA XREF: Main_Screen:loc_50207r ...
// WZD dseg:C070 00 00                                           _special_button dw 0                    ; DATA XREF: Main_Screen:loc_50279r ...
// WZD dseg:C072 00 00                                           _plane_button dw 0                      ; DATA XREF: Main_Screen:loc_5048Br ...
// WZD dseg:C074 00 00                                           _info_button dw 0                       ; DATA XREF: Main_Screen:loc_500CBr ...
// WZD dseg:C076 00 00                                           _magic_button dw 0                      ; DATA XREF: Main_Screen:loc_5010Dr ...
// WZD dseg:C078 00 00                                           _cities_button dw 0                     ; DATA XREF: Main_Screen:loc_500F7r ...
// WZD dseg:C07A 00 00                                           _armies_button dw 0                     ; DATA XREF: Main_Screen:loc_50123r ...
// WZD dseg:C07C 00 00                                           _spells_button dw 0                     ; DATA XREF: Main_Screen:loc_50139r ...
// WZD dseg:C07E 00 00                                           _game_button dw 0                       ; DATA XREF: Main_Screen:loc_500E1r ...
// WZD dseg:C080 00 00                                           dw 0

// WZD dseg:C082
// AKA OVL_STKUnitCards_Top
int16_t _unit_window_start_y;

// WZD dseg:C084
// AKA OVL_STKUnitCards_Lft
int16_t _unit_window_start_x;

// WZD dseg:C086
SAMB_ptr _reduced_map_seg;

// WZD dseg:C088 00 00                                           NIU_MainScreen_local_flag dw 0          ; DATA XREF: Main_Screen:loc_4FB72w ...
// WZD dseg:C088                                                                                         ; drake178: CRP_OverlandVar
// WZD dseg:C088                                                                                         ; only XREF Main_Screen(), sets TRUE, never tests
// WZD dseg:C08A 00 00                                           SND_Bkgrnd_Track dw 0                   ; DATA XREF: IDK_SND_BkGrnd_s518AE+1Er ...
// WZD dseg:C08A                                                                                         ; the current power-based background music track index

// WZD dseg:C08C
int16_t city_built_bldg_idx;

// WZD dseg:C08E 00 00                                           STK_HMoves_Left dw 0                    ; DATA XREF: OVL_GetStackHMoves:@@Donew ...
// WZD dseg:C090 00 00                                           dw 0

// WZD dseg:C092 00 00                                           cycle_incomes dw 0                      ; DATA XREF: Main_Screen_Draw_Summary_Window:@@Gold_Negativer ...
// WZD dseg:C092                                                                                         ; -1 draws negative incomes with a static color instead
// WZD dseg:C092
int16_t cycle_incomes; //  dw 0                      ; -1 draws negative incomes with a static color instead

// WZD dseg:C094 00 00                                           dw 0
// WZD dseg:C096 00 00                                           dw 0



//                                          ¿ BEGIN: List / ArmyList / CityList - Uninitialized Data ?

// WZD dseg:C098
int16_t citylist_item_scanned_field;

// WZD dseg:C09A
// AKA G_Some_OVL_Var_1
int16_t m_citylist_first_item;

// WZD dseg:C09C
int16_t m_city_list_count;

// WZD dseg:C09E
int16_t m_cities_list_field_count;

// WZD dseg:C0A0
int16_t m_cities_list_fields[9];

// WZD dseg:C0B2
int16_t UU_citylist_some_y;
// WZD dseg:C0B4
int16_t UU_citylist_some_x;

// WZD dseg:C0B6
SAMB_ptr m_city_list_array;

// WZD dseg:C0B8
SAMB_ptr list_cities;

// WZD dseg:C0BA
int16_t armylist_item_scanned_field;

// WZD dseg:C0BC 00 00                                           dw 0

// WZD dseg:C0BE
int16_t UU_CityListDraw_ScannedHighlight;

// WZD dseg:C0C0
int16_t IDK_armylist_row_count;

// WZD dseg:C0C2
byte_ptr IDK_armylist_row_fields;

// WZD dseg:C0C4
int16_t m_army_list_count;

// WZD dseg:C0C6
int16_t list_item_count;

// WZD dseg:C0C8
int16_t list_first_item;

// WZD dseg:C0CA
/*
useless count of units
may be left over from copy-pasta from City List
*/
int16_t armylist_unit_count;

// WZD dseg:C0CC
// MoO2  Module: COLREFIT  _n_ships_in_list  Address: 02:00197A28
int16_t m_armies_list_field_count;

// WZD dseg:C0CE 00 00                                           UU_IDK_ArmyList_w42B70 dw 0                         ; DATA XREF: ArmyList_Screen+2Bw
// WZD dseg:C0D0 00 00                                           UU_IDK_ArmyList_w42B70 dw 0                         ; DATA XREF: ArmyList_Screen+25w

// WZD dseg:C0D2
int16_t armylist_hero_portrait_start_y;

// WZD dseg:C0D4
int16_t armylist_hero_portrait_start_x;

// WZD dseg:C0D6
int16_t * armylist_hero_portrait_unit_indices;

// WZD dseg:C0D8
int16_t armylist_hero_portrait_count;

// WZD dseg:C0DA
int16_t * armylist_hero_portrait_fields;

// WZD dseg:C0DC
int16_t * m_armies_list_fields;

// WZD dseg:C0DE
int16_t armylist_start_y;

// WZD dseg:C0E0
int16_t armylist_start_x;

// WZD dseg:C0E2
int16_t armylist_upkeep_food;

// WZD dseg:C0E4
int16_t armylist_upkeep_mana;

// WZD dseg:C0E6
int16_t armylist_upkeep_gold;

// WZD dseg:C0E8
// list of unit enchantment names
char * armylist_unit_enchantment_names[20];

// WZD dseg:C110
int16_t * list_armies;                       // 108 bytes;  2-byte values, 6 x 9  armies x units;

// WZD dseg:C112
byte_ptr m_armies_wp;

// WZD dseg:C114
SAMB_ptr armylist_list_item_count;          // 54 bytes;  6 x 9; 

// WZD dseg:C116
byte_ptr m_armies_wy;

// WZD dseg:C118
byte_ptr m_armies_wx;

// WZD dseg:C11A 00 00                                           dw 0

//                                          ¿ END: List / ArmyList / CityList - Uninitialized Data ?



//                                          ¿ BEGIN: ~ Reduced/World Map - Uninitialized Data ?

// WZD dseg:C11C 00 00                                           minimap_height dw 0                     ; DATA XREF: Draw_Maps:loc_59CF4r ...
// WZD dseg:C11C 00 00                                           reduced_map_height dw 0                 ; DATA XREF: Draw_Maps:loc_59CF4r ...





// WZD dseg:C2FE                                                 ¿ END" ovr083 - Uninitialized Data ?

// WZD dseg:C300
int16_t m_item_wp;
// WZD dseg:C302
int16_t m_item_wy;
// WZD dseg:C304
int16_t m_item_wx;
// WZD dseg:C306
int16_t item_pool_in_process;

// WZD dseg:C308 00 00                                           word_42DA8 dw 0                         ; DATA XREF: sub_6EFA5:loc_6F07Aw ...
// WZD dseg:C30A 00 00                                           G_Some_DIPL_Alloc_5 dw 0                ; DATA XREF: sub_6EFA5+111r ...






// WZD dseg:C5DC                                                 ¿ BEGIN: Move_Stack() || UNITMOVE ?

// WZD dseg:C5DC
int16_t OVL_SWardTriggered;

// WZD dseg:C5DE
// drake178: set to the path tile before the last before moving units overland
int16_t OVL_Action_OriginY;

// WZD dseg:C5E0
// drake178: set to the path tile before the last before moving units overland
int16_t OVL_Action_OriginX;

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
int8_t movepath_cost_array[118];
// WZD dseg:C666
int8_t movepath_y_array[120];
// WZD dseg:C6DE
int8_t movepath_x_array[120];

// WZD dseg:C794 00                                              STK_HasNatureUnits db 0                 ; DATA XREF: CTY_CheckSpellWard:loc_7E798w ...
// WZD dseg:C795 00                                              STK_HasSorceryUnits db 0                ; DATA XREF: CTY_CheckSpellWard+C3w
// WZD dseg:C796 00                                              STK_HasChaosUnits db 0                  ; DATA XREF: CTY_CheckSpellWard+121w ...
// WZD dseg:C797 00                                              STK_HasLifeUnits db 0                   ; DATA XREF: CTY_CheckSpellWard+1A2w
// WZD dseg:C798 00                                              STK_HasDeathUnits db 0                  ; DATA XREF: CTY_CheckSpellWard+200w ...
// WZD dseg:C799 00                                              UU_Global_Byte db 0
// WZD dseg:C799 00                                              align 2

// WZD dseg:C799                                                 ¿ END: Move_Stack() || UNITMOVE ?

// WZD dseg:C79A 00 00                                           CMB_HumanTurn dw 0                      ; DATA XREF: CMB_TacticalCombat+26Dw ...


// WZD dseg:C79E
SAMB_ptr m_hero_portrait_seg;



// WZD dseg:C7FC
// drake178: GUI_NearMsgString
char GUI_NearMsgString[154];





// WZD dseg:C9CA                                                 ¿ END: Settings / Load Screen - Uninitialized Data ?
// WZD dseg:C9CA                                                     ¿ END: ovr160 - Uninitialized Data  LOADSAVE ?
// WZD dseg:C9CC                                                 ¿ BEGIN:  ?

// WZD dseg:C9CC 00 00                                           IMG_GUI_ScrollDnArr@ dw 0               ; DATA XREF: Chancellor_Screen_Draw+31r ...
// WZD dseg:C9CE 00 00                                           IMG_GUI_ScrollUpArr@ dw 0               ; DATA XREF: Chancellor_Screen_Draw:loc_A9A3Fr ...

// WZD dseg:C9D0
SAMB_ptr _scroll_paper_seg;

// WZD dseg:C9D2 00 00                                           word_43472 dw 0                         ; DATA XREF: Chancellor_Screen+1Aw ...
// WZD dseg:C9D4
int16_t CMB_Scroll_MinHeight;
// WZD dseg:C9D6
int16_t _scroll_text_height;
// WZD dseg:C9D8
int16_t _scroll_start_x;
// WZD dseg:C9DA 00 00                                           m_report_scroll_text_height dw 0                         ; DATA XREF: Chancellor_Screen_Draw:loc_A9A53r ...
// WZD dseg:C9DC
int16_t _scroll_text_top;
// WZD dseg:C9DE 00 00                                           hirehero_unit_type_name dw 0               ; DATA XREF: USW_LoadHireScreen+85w ...
// WZD dseg:C9E0 00 00                                           GUI_InHeroNaming dw 0                   ; DATA XREF: Merchant_Popup_Draw+1Br ...
// WZD dseg:C9E2 00 00                                           GAME_HeroHireType dw 0                  ; DATA XREF: USW_HireHeroRedraw+69r ...
// WZD dseg:C9E2                                                                                         ; 0: random, 1: summon, 2: prisoner, 3: champion
// WZD dseg:C9E4 00 00                                           hire_namebox_seg@ dw 0                   ; DATA XREF: USW_LoadHireScreen+78w
// WZD dseg:C9E6 00 00                                           IMG_MerchantBtns@ dw 0                  ; DATA XREF: EVNT_LoadMerchantWnd:loc_AAB67w ...
// WZD dseg:C9E8 00 00                                           hire_banner_seg@ dw 0                    ; DATA XREF: EVNT_LoadMerchantWnd:loc_AAB50w ...
// WZD dseg:C9EA 00 00                                           EVNT_MercUnitCount dw 0                 ; DATA XREF: EVNT_DrawMercHire+67r ...
// WZD dseg:C9EC 00 00                                           CRP_EVNT_MercUnitType dw 0              ; DATA XREF: EVNT_MercHireDialog+3Dw ...
// WZD dseg:C9EE 00 00                                           GAME_AssetCost dw 0                     ; DATA XREF: Merchant_Popup_Draw+91r ...

// WZD dseg:C9EE                                                 ¿ END:  ?



// WZD dseg:CA10                                                 BEGIN: ovr136 - Uninitialized Data

// WZD dseg:CA10 00 00                                           CMB_SliderLimit dw 0                    ; DATA XREF: CMB_SliderRedraw+7Br ...
// WZD dseg:CA10                                                                                         ; is this really necessary?
// WZD dseg:CA12 00 00                                           GAME_MP_SpellVar_1 dw 0                 ; DATA XREF: SBK_SliderRedraw+9Fw ...
// WZD dseg:CA12                                                                                         ; holds the spell strength during sliders
// WZD dseg:CA12                                                                                         ; holds the anim stage during global cast anims
// WZD dseg:CA14 00 00                                           SBK_SliderAnimStage dw 0                ; DATA XREF: SBK_SliderRedraw+2Cr ...
// WZD dseg:CA14                                                                                         ; steps 0 to 7 for sliders
// WZD dseg:CA16 00 00                                           SBK_Spell_Index dw 0                    ; DATA XREF: GAME_LearnSpellAnim+Cw ...
// WZD dseg:CA18 00 00                                           SBK_SliderState dw 0                    ; DATA XREF: SBK_SliderRedraw+6Br ...
// WZD dseg:CA1A 00 00 00 00 00 00 00 00 00 00                   word_434BA dw 5 dup(0)                  ; DATA XREF: IDK_Spell_DisjunctOrBind_Load+308w ...
// WZD dseg:CA24 00 00                                           word_434C4 dw 0                         ; DATA XREF: IDK_SomScr_Load+8Cw ...
// WZD dseg:CA26 00 00                                           IMG_SBK_SliderBG@ dw 0                  ; DATA XREF: SBK_LoadSpellSlider+38w ...
// WZD dseg:CA28 00 00                                           GAME_MP_SpellVar_2 dw 0                 ; DATA XREF: CMB_SliderRedraw+3Fr ...
// WZD dseg:CA28                                                                                         ; holds the caster ID during combat sliders
// WZD dseg:CA28                                                                                         ; holds the mirror reveal mask during global cast anims
// WZD dseg:CA2A 00 00                                           IMG_OVL_TrgtWizCncl@ dw 0               ; DATA XREF: IDK_SplScr_sBFAA5+50w ...
// WZD dseg:CA2A                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:CA2C 00 00                                           IDK_DiploScrn_scanned_field dw 0        ; DATA XREF: IDK_Spell_DisjunctOrBind_Draw+44r ...
// WZD dseg:CA2E 00 00                                           IDK_SUMMONBK_pict_seg dw 0              ; DATA XREF: IDK_Spell_DisjunctOrBind_Load+17Dw ...
// WZD dseg:CA30 00 00                                           IMG_SBK_SliderOK@ dw 0                  ; DATA XREF: SBK_LoadSpellSlider+7Dw ...
// WZD dseg:CA32 00 00                                           IMG_OVL_TargetWizBG@ dw 0               ; DATA XREF: IDK_SplScr_sBFAA5+39w ...
// WZD dseg:CA34 00 00                                           IMG_SBK_Anims@ dw 0                     ; DATA XREF: CMB_ShowSpellbook+30w ...
// WZD dseg:CA34                                                                                         ; 145h paragraphs in the sandbox during spell decode
// WZD dseg:CA34                                                                                         ; 442h paragraphs in the sandbox during combat casting

// WZD dseg:CA36
// drake178: IMG_SBK_PageText
/*
Merchant_Popup_Load()
    IMG_SBK_PageText = Allocate_Next_Block(_screen_seg, 30);
*/
SAMB_ptr IMG_SBK_PageText;

// WZD dseg:CA38 00 00                                           IDK_MONSTER_seg dw 0                    ; DATA XREF: IDK_SummonAnim_Load+EEw ...
// WZD dseg:CA3A 00 00                                           word_434DA dw 0                         ; DATA XREF: IDK_SummonAnim_Load+CBw ...
// WZD dseg:CA3C 00 00                                           IDK_SPELLSCR_seg__1 dw 0                ; DATA XREF: IDK_SummonAnim_Load+49w ...
// WZD dseg:CA3E 00 00                                           word_434DE dw 0                         ; DATA XREF: IDK_SummonAnim_Load+B4w ...
// WZD dseg:CA40 00 00                                           IMG_SBK_SliderBar@ dw 0                 ; DATA XREF: ITEM_LoadCraftingRes+188w ...
// WZD dseg:CA40                                                                                         ; also the item crafting right arrow (2 state image)
// WZD dseg:CA42 00 00                                           IMG_SBK_SliderDot@ dw 0                 ; DATA XREF: ITEM_LoadCraftingRes+171w ...
// WZD dseg:CA42                                                                                         ; also the item crafting left arrow (2 state image)
// WZD dseg:CA44 00 00                                           IMG_MoodWizPortrait@ dw 0               ; DATA XREF: OVL_LoadGlobalAnim+28w ...
// WZD dseg:CA46 00                                              db    0
// WZD dseg:CA47 00                                              db    0
// WZD dseg:CA48 00                                              db    0
// WZD dseg:CA49 00                                              db    0
// WZD dseg:CA4A 00                                              db    0
// WZD dseg:CA4B 00                                              db    0
// WZD dseg:CA4C 00 00                                           GUI_Interaction_Done dw 0               ; DATA XREF: SBK_SpellSlider+D5w ...
// WZD dseg:CA4E 00 00                                           IDK_WizTgt_SplCmpl_w434EE dw 0          ; DATA XREF: IDK_Spell_DisjunctOrBind_Draw+20Ar ...
// WZD dseg:CA50 00 00                                           GAME_MP_SpellVar_3 dw 0                 ; DATA XREF: SBK_SliderRedraw+8r ...
// WZD dseg:CA50                                                                                         ; clicked spell label index during combat sliders
// WZD dseg:CA50                                                                                         ; player_idx during global cast anims
// WZD dseg:CA50                                                                                         ; city_idx during ¿ ?
// WZD dseg:CA52 00 00                                           word_434F2 dw 0                         ; DATA XREF: IDK_Spell_DisjunctOrBind_Draw+25Fr ...

// WZD dseg:CA52                                                 END: ovr136 - Uninitialized Data



// WZD dseg:CA78                                                 ¿ BEGIN: Cityscape - ?

// WZD dseg:CA78
// drake178: IDK_animate_new_building_idx
// WZD dseg:CA7A
// drake178: dword_4351A

// WZD dseg:CA7E
// drake178: IMG_CTY_Vtrcl_Mask@
SAMB_ptr IDK_BUILDS1_vertical_mask_seg;

// WZD dseg:CA80
// drake178: IMG_CTY_Horz_Mask@
SAMB_ptr IDK_BUILDS1_horizontal_mask_seg;

// WZD dseg:CA82
// drake178: Frame_Index
// WZD dseg:CA84
// drake178: word_43524
// WZD dseg:CA86
// drake178: word_43526
// WZD dseg:CA88
// drake178: pict_seg

// WZD dseg:CA88                                                 ¿ END: Cityscape - ?





// WZD dseg:D490 00 00                                           UU_AI_TargetingVar dw 0                 ; DATA XREF: AI_ProcessRoamers:loc_ED7F2w
// WZD dseg:D492 00 00                                           dw 0
// WZD dseg:D494 00 00                                           GUI_Help_NearAlloc dw 0                 ; DATA XREF: Draw_Help_Entry_+BBw ...
// WZD dseg:D494                                                                                         ; 1048 bytes of LBX_NearAlloc_First space
// Allocate  // WZD dseg:D496 00 00                                           save_active dw 0                        ; DATA XREF: Load_Screen+D1w ...

// Allocate  // WZD dseg:D498
// Allocate  int16_t LBX_NearBuf_Mark
// Allocate  // WZD dseg:D49A
// Allocate  char Temp_String[100];
// Allocate  // WZD dseg:D4FE
// Allocate  char Tmp_Conv_Str_1[20];
// Allocate  // WZD dseg:D512
// Allocate  char Tmp_Conv_Str_2[30];
// Allocate  // WZD dseg:D530
// Allocate  char Tmp_Conv_Str_3[106];
// Allocate  // WZD dseg:D59A
// Allocate  char LBX_Near_Buffer[4144];

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
// AKA  EmmHndlNbr_VGAFILEH
byte_ptr _VGAFILEH_seg;
SAMB_ptr report_scroll_out_seg;
/* MoO2 */

// WZD dseg:E5F4                                                                                         ; 5 reserved pages
// WZD dseg:E5F6 00 00                                           g_EmmRsvd dw 0                          ; DATA XREF: EMM_LBX_EntryLoader:@@EmmHndlNmExistsr ...
// WZD dseg:E5F8 00 00                                           EmmHndlNbr_YOMOMA dw 0                  ; DATA XREF: EMM_Startup+1Bw ...
// WZD dseg:E5F8                                                                                         ; 1 non-reserved page
// WZD dseg:E5FA 00 00                                           EMM_OK dw 0                             ; DATA XREF: EMM_Startup:loc_16E33w ...
// WZD dseg:E5FA                                                                                         ; set to 1 after creating the YO MOMA handle
// WZD dseg:E5FC 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+EMM_Table EMM_Record 28h dup(<0>)       ; DATA XREF: EMM_Startup+8Bt ...
// WZD dseg:E7DC 00 00                                           EMM_MinKB dw 0                          ; DATA XREF: EMS_SetMinKB+6w ...
// WZD dseg:E7DC                                                                                         ; set to 2700 in _main



// WZD dseg:E7DE                                                 BEGIN:  Fonts

// WZD dseg:E7DE 00 00                                           gsa_VGAFILEH_Header dw 0                ; DATA XREF: Load_Font_File+78w ...
// WZD dseg:E7DE
// struct s_animation_header file_animation_header = 0;
// TODO  relocate  struct s_FLIC_HDR file_animation_header;

/*
MoO2
dseg02:001A86F8                                                 BEGIN: Create_IO_Buffer()
dseg02:001A86F8 ?? ?? ?? ??                                     io_buffer_seg       dd ?                                                                            ; DATA XREF: Create_IO_Buffer+25w ...
dseg02:001A86FC ?? ?? ?? ??                                     io_buffer           dd ?                                                                            ; DATA XREF: Do_Main_Menu_File_Animation_:loc_80E05r ...
dseg02:001A8700 ?? ??                                           io_buffer_selector  dw ?                                                                            ; DATA XREF: Create_IO_Buffer+3Dw
dseg02:001A8700                                                 END: Create_IO_Buffer()
*/



// WZD dseg:E872                                                 ¿ BEGIN: fields, intput, ... ?
// WZD dseg:E872                                                 ¿ BEGIN: Help - Uninitialized Data ?

// WZD dseg:E872
// AKA _help_list
// struct s_HELP_FIELD * help_struct_pointer[50];
/*
    ¿ pointer to an array of pointers of data type 'struct s_HELP_FIELD' ?
*/
// struct s_HELP_FIELD ** help_struct_pointer;
struct s_HELP_FIELD * help_struct_pointer;


// WZD dseg:E872                                                 ¿ END: Help - Uninitialized Data ?
// ...
// ...
// ...
// WZD dseg:82A6                                                 END: Fields, Input, Mouse, Keyboard

