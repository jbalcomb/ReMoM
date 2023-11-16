
#include "MoX_TYPE.H"
#include "MoX_DEF.H"
#include "MoM_DEF.H"

#include "MoX_Data.H"

#include "Mouse.H"  /* struct s_mouse_list */






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

// WZD dseg:00C8 62 D8 7B 2B B3 32                               COL_Cartographer db 98, 216, 123, 43, 179, 50
// WZD dseg:00CE C9 00 A5 00 CB 00                               UU_COL_Setof3_1 db 201,  0,165,  0,203,  0
// WZD dseg:00D4 79 00 7A 00 7B 00                               UU_COL_Setof3_2 db 121,  0,122,  0,123,  0
// WZD dseg:00DA 0D 00 0E 00 0F 00                               UU_COL_Setof3_3 db  13,  0, 14,  0, 15,  0
// WZD dseg:00E0 49 00 4A 00 4B 00                               UU_COL_Setof3_4 db  73,  0, 74,  0, 75,  0
// WZD dseg:00E6 69 00 6A 00 6B 00                               UU_COL_Setof3_5 db 105,  0,106,  0,107,  0
// WZD dseg:00EC 00                                              db    0
// WZD dseg:00ED 00                                              db    0


// WZD dseg:00EE 01 00 00 00 00 00 00 00 3F 01 C7 00             mouse_list_default s_MOUSE_LIST <crsr_Finger, 0, 0, 0, 319, 199>
// struct s_mouse_list mouse_list_default[1] = {
//     {crsr_Finger, 0, 0, 0, 319, 199}
// };
// WZD dseg:00FA 00 00 00 00 00 00 00 00 3F 01 C7 00             mouse_list_none s_MOUSE_LIST <0, 0, 0, 0, 319, 199>
// struct s_mouse_list mouse_list_none[1] = {
//     {crsr_None, 0, 0, 0, 319, 199}
// };
// WZD dseg:0106 06 00 00 00 00 00 00 00 3F 01 C7 00             mouse_list_hourglass s_MOUSE_LIST <crsr_Hourglass, 0, 0, 0, 319, 199>
struct s_mouse_list mouse_list_hourglass[1] = {
    {crsr_Hourglass, 0, 0, 0, 319, 199}
};
// WZD dseg:0112 01 00 00 00 00 00 00 00 3F 01 C7 00             NIU_mouse_list_normal s_MOUSE_LIST <crsr_Finger, 0, 0, 0, 319, 199>
// WZD dseg:011E 07 00 04 00 00 00 00 00 3F 01 9E 00             NIU_mouse_list_boot s_MOUSE_LIST <crsr_WingedBoot, 4, 0, 0, 319, 158> ; ? 158 is main map width ?

// WZD dseg:012A 01 02 04 08 10 20                               byte_36BCA db   1,  2,  4,  8, 16, 32
// WZD dseg:0130 82 20 8A 20 92 20 A0 20 AF 20 BF 20 CE 20 DB 20+wizard_abilities_names dw offset cnst_Alchemy, offset cnst_Warlord, offset cnst_ChaosMastery, offset cnst_NatureMastery, offset cnst_SorceryMastery, offset cnst_InfernalPower, offset cnst_DivinePower, offset cnst_SageMaster, offset cnst_Channeler, offset cnst_Myrran, offset cnst_Archmage
// WZD dseg:0130 E7 20 F1 20 F8 20 01 21 0F 21 1C 21 23 21 2E 21+                                        ; DATA XREF: Mirror_Screen_Draw+61Er ...
// WZD dseg:0130 37 21 43 21                                     dw offset cnst_ManaFocusing, offset cnst_NodeMastery, offset cnst_Famous, offset cnst_Runemaster, offset cnst_Conjurer, offset cnst_Charismatic, offset cnst_Artificer ; "Alchemy"
// WZD dseg:0154 01 00                                           EVNT_Enabled dw 1                       ; DATA XREF: EVNT_GenerateRandom:loc_6AD68r






// WZD dseg:1D86                                                 dw 2                                    ; HouseType
// WZD dseg:1ED6 89 20                                           STR_USW_NoRace@ dw offset cnst_ZeroString_3
// WZD dseg:1ED8 DE 26 E5 26 EC 26 F4 26 9C 21 FA 26             STR_USW_MagicRaces@ dw offset cnst_Rlm5_Arcane, offset cnst_Rlm0_Nature, offset cnst_Rlm1_Sorcery, offset cnst_Rlm2_Chaos, offset cnst_Rlm3_Life, offset cnst_Rlm4_Death
// WZD dseg:1ED8                                                                                         ; DATA XREF: USW_DrawView+453t ...
// WZD dseg:1ED8                                                                                         ; "Life"
// WZD dseg:1EE4 00 27 0D 27 B6 21 18 27 26 27 33 27 3E 27 4E 27+STR_CityEnchants@ dw offset cnst_WallofFire





// WZD dseg:1F18
// drake178: TBL_UE_Upkeep
// ; Unit Enchantment upkeeps in bit order, from Immolation to Invulnerability
// ; DATA XREF: UNIT_GetManaUpkeep+13B
// 02 00 02 00 00 00 01 00 01 00 03 00 0A 00 01 00
// 01 00 01 00 05 00 01 00 05 00 01 00 01 00 0A 00
// 0A 00 03 00 01 00 05 00 02 00 01 00 02 00 01 00
// 02 00 01 00 04 00 01 00 05 00 02 00 02 00 05 00
// dw 2   ; 0
// dw 2   ; 1 
// dw 0   ; 2 
// dw 1   ; 3
// dw 1   ; 4
// dw 3   ; 5
// dw 10  ; 6
// dw 1   ; 7
// dw 1   ; 8
// dw 1   ; 9
// dw 5   ; 10
// dw 1   ; 11
// dw 5   ; 12
// dw 1   ; 13
// dw 1   ; 14
// dw 10  ; 15
// dw 10  ; 16
// dw 3   ; 17
// dw 1   ; 18
// dw 5   ; 19
// dw 2   ; 20
// dw 1   ; 21
// dw 2   ; 22
// dw 1   ; 23
// dw 2   ; 24
// dw 1   ; 25
// dw 4   ; 26
// dw 1   ; 27
// dw 5   ; 28
// dw 2   ; 29
// dw 2   ; 30
// dw 5   ; 31
int16_t unit_enchantment_upkeep_table[32] = {2, 2, 0, 1, 1, 3, 10, 1, 1, 1, 5, 1, 5, 1, 1, 10, 10, 3, 1, 5, 2, 1, 2, 1, 2, 1, 4, 1, 5, 2, 2, 5};

// WZD dseg:1F58
// drake178: TBL_CE_Upkeep
// ; City Enchantment upkeeps in byte order, from Wall of Fire to Altar of Battle, plus Nightshade
// ; DATA XREF: CTY_EnchantsUpkeep+3D
// 02 00 0A 00 00 00 04 00 02 00 05 00 03 00 05 00
// 19 00 05 00 05 00 05 00 05 00 05 00 01 00 05 00
// 08 00 03 00 02 00 02 00 05 00 02 00 08 00 05 00
// 05 00 00 00
// dw 2   ; 0
// dw 10  ; 1 
// dw 0   ; 2 
// dw 4   ; 3
// dw 2   ; 4
// dw 5   ; 5
// dw 3   ; 6
// dw 5   ; 7
// dw 25  ; 8
// dw 5   ; 9
// dw 5   ; 10
// dw 5   ; 11
// dw 5   ; 12
// dw 5   ; 13
// dw 1   ; 14
// dw 5   ; 15
// dw 8   ; 16
// dw 3   ; 17
// dw 2   ; 18
// dw 2   ; 19
// dw 5   ; 20
// dw 2   ; 21
// dw 8   ; 22
// dw 5   ; 23
// dw 5   ; 24
// dw 0   ; 25
int16_t city_enchantment_upkeep_table[26] = {2, 10, 0, 4, 2, 5, 3, 5, 25, 5, 5, 5, 5, 5, 1, 5, 8, 3, 2, 2, 5, 2, 8, 5, 5, 0};

// WZD dseg:1F8C
/// drake178: TBL_GE_Upkeep
// ; Global Enchantment upkeeps in byte order, from Eternal Night to Awareness
// DATA XREF: WIZ_GlobalsUpkeep+29
// 0F 00 0A 00 28 00 05 00 05 00 32 00 C8 00 07 00
// 0A 00 0A 00 28 00 0F 00 14 00 0A 00 28 00 0A 00
// 0A 00 0A 00 03 00 0A 00 05 00 0A 00 03 00 03 00
// dw 15   ; 0
// dw 10   ; 1 
// dw 40   ; 2 
// dw 5    ; 3
// dw 5    ; 4
// dw 50   ; 5
// dw 200  ; 6
// dw 7    ; 7
// dw 10   ; 8
// dw 10   ; 9
// dw 40   ; 10
// dw 15   ; 11
// dw 20   ; 12
// dw 10   ; 13
// dw 40   ; 14
// dw 10   ; 15
// dw 10   ; 16
// dw 10   ; 17
// dw 3    ; 18
// dw 10   ; 19
// dw 5    ; 20
// dw 10   ; 21
// dw 3    ; 22
// dw 3    ; 23
int16_t overland_enchantment_upkeep_table[24] = {15, 10, 40, 5, 5, 50, 200, 7, 10, 10, 40, 15, 20, 10, 40, 10, 10, 10, 3, 10, 5, 10, 3, 3};





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

// WZD dseg:1FDC 74 28                                           dw offset aExpansionist                 ; "Expansionist"
// WZD dseg:1FDE 28 00 14 00 0A 00 0A 00 D8 FF EC FF             TBL_AI_PRS_War_Mod dw 40, 20, 10, 10, 65496, 65516
// WZD dseg:1FDE                                                                                         ; DATA XREF: AI_OVL_SplCat_Picker+9CEr ...

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


// WZD dseg:203A 00 00 00 05 00 00 00 00 05 00 00 00 0A 00 00 00+TBL_AI_BLD_OBJWgts AI_BLD_Obj_Priorities <<0, 0, 0, 5, 0>, <0, 0, 0, 5, 0>, <0, 0, 10, 0, 0>, <0, 0, 10, 0, 0>, <0, 10, 0, 0, 2>, <0, 0, 0, 5, 0>, <0, 0, 0, 5, 0>, <0, 5, 0, 0, 5>, <0, 5, 0, 0, 3>, <0, 0, 0, 0, 10>>
// WZD dseg:203A 00 0A 00 00 00 0A 00 00 02 00 00 00 05 00 00 00+                                        ; DATA XREF: AI_CTY_SetProduction+49Fr ...
// WZD dseg:206C 0A 00 0F 00 1E 00 0A 00 0A 00 0F 00 0F 00 0A 00+TBL_AI_BLD_BaseWgts dw 10, 15, 30, 10, 10, 15, 15, 10, 10, 10
// WZD dseg:206C 0A 00 0A 00                                                                             ; DATA XREF: AI_CTY_SetProduction+4C2r ...


// WZD dseg:2080                                                 ¿  BEGIN: meaningful boundary ?
// WZD dseg:2080 00 10                                           __ovrbuffer dw 4096
// ; DATA XREF: __OvrPrepare+19r ...
// ; declared in overlay.lib\OVRBUFF
// WZD dseg:2080                                                 ¿  BEGIN: meaningful boundary ?

// WZD dseg:2082 41 6C 63 68 65 6D 79                            cnst_Alchemy db 'Alchemy'               ; DATA XREF: dseg:wizard_abilities_nameso





// WZD dseg:2A12
char builddat_lbx_file[] = "BUILDDAT.LBX";







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
// drake178: TBL_Tax_Unrest_Pcnts
int16_t tax_unrest_pct_table[7] = {0,10,20,30,45,60,75};



// WZD dseg:6FFE
// AKA Map_LastDraw_X
int16_t prev_map_x = ST_UNDEFINED;
// WZD dseg:7000
// AKA Map_LastDraw_Y
int16_t prev_map_y = ST_UNDEFINED;
// WZD dseg:7002
// AKA OVL_NewMapSustain
int16_t map_sustain = 2;



// dseg:76DA
int16_t screen_window_x1 = SCREEN_XMIN;
// dseg:76DC
int16_t screen_window_y1 = SCREEN_YMIN;
// dseg:76DE
int16_t screen_window_x2 = SCREEN_XMAX;
// dseg:76E0
int16_t screen_window_y2 = SCREEN_YMAX;





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

// WZD dseg:912C                                                                                         ; 215 records of 36 bytes
// WZD dseg:9130 00 00                                           IMG_GAME_SpellAnim@ dw 0                ; DATA XREF: GAME_LearnSpellAnim+374w ...
// WZD dseg:9130                                                                                         ; appended into the LBX_Sandbox_Segment
// WZD dseg:9132 00 00                                           SND_SpellCast@ dw 0                     ; DATA XREF: GAME_LoadSpellSound+7Cw ...
// WZD dseg:9132                                                                                         ; appended into World_Data@ during combat, or the
// WZD dseg:9132                                                                                         ; Sandbox overland (-1 if none or SFX are disabled)
// WZD dseg:9134 00 00                                           SND_Spell_Music@ dw 0                   ; DATA XREF: GAME_LearnSpellAnim+39w ...

// WZD dseg:9136                                                 ? BEGIN:  - Uninitialized Data ?



// WZD dseg:9136
struct s_ITEM * TBL_Items;

// WZD dseg:913A
SAMB_ptr TBL_Premade_Items;
// WZD dseg:913E
// IMG_USW_Items ITEM_ICONS

// WZD dseg:9226
struct s_BU_REC * Active_Unit;                       // alloc in Allocate_Data_Space()

// WZD dseg:922A
// TBL_BattleUnits

// WZD dseg:922E
// TBL_CombatEnchants


// struct s_HERO p_heroes[6][35];
struct s_HERO * p_heroes[6];
// WZD dseg:9232
SAMB_ptr p0_heroes;
// WZD dseg:9236
SAMB_ptr p1_heroes;
// WZD dseg:923A
SAMB_ptr p2_heroes;
// WZD dseg:923E
SAMB_ptr p3_heroes;
// WZD dseg:9242
SAMB_ptr p4_heroes;
// WZD dseg:9246
SAMB_ptr p5_heroes;


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





// WZD dseg:938C
struct s_BUILDDAT * build_data_table;





/* -2: NEVER, -1: ALWAYS, {0,1,2,3}: frame - draw off, {4,5,6,7}: frame - draw on */
// ? Set_Draw_Active_Stack_...() means this variable is private to ovr067 ?
// WZD dseg:9442
int16_t draw_active_stack_flag;  // AKA Active_Stack_Draw

// WZD dseg:9444
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
// WZD dseg:9460 00 00                                           IMG_SPL_BigBook@ dw 0                   ; DATA XREF: GAME_LearnSpellAnim+16Dw ...
// WZD dseg:9460                                                                                         ; LBXE_LoadReplace into the LBX_Sandbox_Segment
// WZD dseg:9462 00 00                                           dw 0
// WZD dseg:9464 00 00                                           IMG_SPL_BigRDogear@ dw 0                ; DATA XREF: SBK_PageFlip_Big+79w ...
// WZD dseg:9466 00 00                                           IMG_SPL_BigLDogear@ dw 0                ; DATA XREF: SBK_PageFlip_Big+62w ...
// WZD dseg:9468 00 00                                           _spellbook_small_right_corner_seg dw 0  ; DATA XREF: Spellbook_Load_Small_Pictures+46w ...
// WZD dseg:9468                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:946A 00 00                                           _spellbook_small_left_corner_seg dw 0   ; DATA XREF: Spellbook_Load_Small_Pictures+2Fw ...
// WZD dseg:946A                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:946C 00 00                                           word_3FF0C dw 0                         ; DATA XREF: sub_BECD9+23w ...
// WZD dseg:946E 00 00                                           _spellbook_small_seg dw 0               ; DATA XREF: Spellbook_Load_Small_Pictures+18w ...
// WZD dseg:946E                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:9470 00 00                                           IMG_GUI_ScrollBtm@ dw 0                 ; DATA XREF: CMB_DrawSummary+3Br ...
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

// WZD dseg:9484 00 00                                           IMG_GUI_AlchRevText@ dw 0               ; DATA XREF: GAME_AlchemyWindow+10Bw ...
// WZD dseg:9486 00 00                                           IMG_GUI_AlchemyBar@ dw 0                ; DATA XREF: GAME_AlchemyWindow+DDw ...
// WZD dseg:9488 00 00                                           IMG_GUI_AlchemyRArr@ dw 0               ; DATA XREF: GAME_AlchemyWindow+C6w ...
// WZD dseg:9488                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:948A 00 00                                           IMG_GUI_AlchemyLArr@ dw 0               ; DATA XREF: GAME_AlchemyWindow+AFw ...
// WZD dseg:948A                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:948C 00 00                                           IMG_GUI_AlchemyCncl@ dw 0               ; DATA XREF: GAME_AlchemyWindow+81w ...
// WZD dseg:948C                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:948E 00 00                                           IMG_GUI_AlchemyOk@ dw 0                 ; DATA XREF: GAME_AlchemyWindow+98w ...
// WZD dseg:948E                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:9490 00 00                                           UU_IMG_NewG_OKBtn@ dw 0                 ; MAGIC.EXE variable, unused in WIZARDS.EXE
// WZD dseg:9492 00                                              db    0
// WZD dseg:9493 00                                              db    0
// WZD dseg:9494 00 00                                           IMG_GUI_AlchemyStar@ dw 0               ; DATA XREF: GAME_AlchemyWindow+F4w ...
// WZD dseg:9496 00 00                                           IMG_GUI_Alchemy_BG@ dw 0                ; DATA XREF: GAME_AlchemyWindow+6Aw ...
// WZD dseg:9498 00 00                                           IMG_CTY_RED_Btn@ dw 0                   ; DATA XREF: City_Screen_Load_Pictures+115w ...
// WZD dseg:9498                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:9498                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:949A 00 00                                           IMG_CTY_RightBldTab@ dw 0               ; DATA XREF: GFX_Swap_AppendUView+1E9w ...
// WZD dseg:949A                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:949C 00 00                                           IMG_CTY_LeftBldTab@ dw 0                ; DATA XREF: GFX_Swap_AppendUView+1D2w ...
// WZD dseg:949C                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:949E 00 00                                           IMG_OVL_EnemyCityBG@ dw 0               ; DATA XREF: IDK_EnemyCityScreen_s4A3F0+115w ...
// WZD dseg:94A0 00 00                                           IMG_CTY_10_Food@ dw 0                   ; DATA XREF: City_Screen_Load_Pictures+1EDw ...
// WZD dseg:94A0                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:94A2 00 00                                           IMG_CTY_10_Prod@ dw 0                   ; DATA XREF: sub_4DA19+2CEr
// WZD dseg:94A2                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:94A4 00 00                                           IMG_CTY_10_Gold@ dw 0                   ; DATA XREF: sub_4DA19+484r ...
// WZD dseg:94A4                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:94A6 00 00                                           IMG_CTY_10_Power@ dw 0                  ; DATA XREF: sub_4DA19+7A6r ...
// WZD dseg:94A6                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:94A8 00 00                                           IMG_CTY_10_Books@ dw 0                  ; DATA XREF: sub_4DA19+9B2r
// WZD dseg:94A8                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:94AA 00 00                                           IMG_CTY_Neg_10_Gold@ dw 0               ; DATA XREF: IDK_CityScreen_AddResourcesFields+44Er ...
// WZD dseg:94AA                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:94AC 00 00                                           IMG_CTY_Neg_10_Food@ dw 0               ; DATA XREF: City_Screen_Load_Pictures+20Bw ...
// WZD dseg:94AE 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+IMG_CTY_Rebels@ dw 0Eh dup(0)           ; DATA XREF: City_Screen_Load_Pictures+186w ...
// WZD dseg:94AE 00 00 00 00 00 00 00 00 00 00 00 00                                                     ; array of 14 appended reserved EMM headers in
// WZD dseg:94AE                                                                                         ; GFX_Swap_Seg, 1 rebel image per header
// WZD dseg:94CA 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+IMG_CTY_Farmers@ dw 0Eh dup(0)          ; DATA XREF: City_Screen_Load_Pictures+15Ew ...
// WZD dseg:94CA 00 00 00 00 00 00 00 00 00 00 00 00                                                     ; array of 14 appended reserved EMM headers in
// WZD dseg:94CA                                                                                         ; GFX_Swap_Seg, 1 farmer image per header
// WZD dseg:94E6 00 00                                           IMG_CTY_1_Food@ dw 0                    ; DATA XREF: City_Screen_Load_Pictures+1AEw ...
// WZD dseg:94E6                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:94E8 00 00                                           IMG_CTY_1_Prod@ dw 0                    ; DATA XREF: sub_4DA19+2D2r
// WZD dseg:94E8                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:94EA 00 00                                           IMG_CTY_1_Gold@ dw 0                    ; DATA XREF: sub_4DA19+488r ...
// WZD dseg:94EA                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:94EC 00 00                                           IMG_CTY_1_Power@ dw 0                   ; DATA XREF: sub_4DA19+7AAr ...
// WZD dseg:94EC                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:94EE 00 00                                           IMG_CTY_1_Book@ dw 0                    ; DATA XREF: sub_4DA19+9B6r
// WZD dseg:94EE                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:94F0 00 00                                           IMG_CTY_Neg_1_Food@ dw 0                ; DATA XREF: City_Screen_Load_Pictures+1CCw ...
// WZD dseg:94F2 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+IMG_CTY_Workers@ dw 0Eh dup(0)          ; DATA XREF: City_Screen_Load_Pictures+136w ...
// WZD dseg:94F2 00 00 00 00 00 00 00 00 00 00 00 00                                                     ; array of 14 appended reserved EMM headers in
// WZD dseg:94F2                                                                                         ; GFX_Swap_Seg, 1 worker image per header
// WZD dseg:950E                                                 BEGIN: CityList_Screen_Load()
// WZD dseg:950E 00 00                                           citylist_ok_button_seg dw 0             ; DATA XREF: CityList_Screen+101r ...
// WZD dseg:950E                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:9510 00 00                                           citylist_down_button_seg dw 0           ; DATA XREF: CityList_Screen+1FFr ...
// WZD dseg:9510                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:9512 00 00                                           citylist_up_button_seg dw 0             ; DATA XREF: CityList_Screen+16Er ...
// WZD dseg:9512                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:9514 00 00                                           citylist_background_seg dw 0            ; DATA XREF: CityList_Screen:loc_5767Dr ...
// WZD dseg:9514                                                 END: CityList_Screen_Load()
// WZD dseg:9516
// WZD dseg:9516
// WZD dseg:9516
// WZD dseg:9516                                                 BEGIN:  Main Screen Pictures
// WZD dseg:9516
// WZD dseg:9516
// WZD dseg:9516
// WZD dseg:9516 00 00                                           next_turn_button_seg dw 0               ; DATA XREF: Main_Screen_Load_Pictures+299w ...
// WZD dseg:9516                                                                                         ; single-loaded image
// WZD dseg:9518 00 00                                           survey_background dw 0                  ; DATA XREF: Main_Screen_Load_Pictures+34Dw ...
// WZD dseg:9518                                                                                         ; single-loaded image
// WZD dseg:951A 00 00                                           road_button_border dw 0                 ; DATA XREF: Main_Screen_Load_Pictures+33Bw ...
// WZD dseg:951A                                                                                         ; single-loaded image
// WZD dseg:951C 00 00                                           road_background dw 0                    ; DATA XREF: Main_Screen_Load_Pictures+317w ...
// WZD dseg:951C                                                                                         ; single-loaded image
// WZD dseg:951E 00 00                                           road_ok_button dw 0                     ; DATA XREF: Main_Screen_Load_Pictures+329w ...
// WZD dseg:951E                                                                                         ; single-loaded 2 frame image (normal - clicked)
// WZD dseg:9520 00 00                                           dw 0
// WZD dseg:9522 00 00                                           DESELECT_button_blockout dw 0           ; DATA XREF: Main_Screen_Load_Pictures+305w ...
// WZD dseg:9522                                                                                         ; single-loaded image
// WZD dseg:9524 00 00                                           cast_cancel_border dw 0                 ; DATA XREF: Main_Screen_Load_Pictures+2F3w ...
// WZD dseg:9524                                                                                         ; single-loaded image
// WZD dseg:9526 00 00                                           cast_cancel_button dw 0                 ; DATA XREF: Main_Screen_Load_Pictures+2E1w ...
// WZD dseg:9526                                                                                         ; single-loaded 2 frame image (normal - clicked)
// WZD dseg:9528 00 00                                           cast_background dw 0                    ; DATA XREF: Main_Screen_Load_Pictures+2CFw ...
// WZD dseg:9528                                                                                         ; single-loaded image
// WZD dseg:952A 00 00                                           dw 0
// WZD dseg:952C 00 00                                           next_turn_background_seg dw 0           ; DATA XREF: Main_Screen_Load_Pictures:loc_466E8w ...
// WZD dseg:952C                                                                                         ; single-loaded image
// WZD dseg:952E 00 00                                           deselect_background dw 0                ; DATA XREF: Main_Screen_Load_Pictures+2ABw ...
// WZD dseg:952E                                                                                         ; single-loaded image
// WZD dseg:952E
// WZD dseg:952E
// WZD dseg:952E                                                 END:  Main Screen Pictures
// WZD dseg:952E
// WZD dseg:952E
// WZD dseg:952E
// WZD dseg:9530 00 00 00 00 00 00                               IMG_CTY_EmptyHuts@ dw 3 dup(0)          ; DATA XREF: City_Screen_Load_Pictures:loc_46A47w ...
// WZD dseg:9530                                                                                         ; array of 3 appended res EMM hdrs in GFX_Swap_Seg
// WZD dseg:9536 00 00 00 00 00 00                               IMG_CTY_FilledHuts@ dw 3 dup(0)         ; DATA XREF: City_Screen_Load_Pictures+25Aw ...
// WZD dseg:9536                                                                                         ; array of 3 appended res EMM hdrs in GFX_Swap_Seg
// WZD dseg:953C 00 00                                           namecity_background_seg dw 0            ; DATA XREF: City_Screen_Load_Pictures+239w ...
// WZD dseg:953E 00 00                                           IMG_CTY_Outpost_BG@ dw 0                ; DATA XREF: City_Screen_Load_Pictures+222w ...
// WZD dseg:9540 00 00                                           IMG_USW_SideBtns_BG@ dw 0               ; DATA XREF: GFX_Swap_AppendUView+8Bw ...
// WZD dseg:9542 00 00                                           dw 0
// WZD dseg:9544 00 00                                           dw 0
// WZD dseg:9546 00 00                                           dw 0
// WZD dseg:9548 00 00                                           IMG_OVL_BuildBtn_BG@ dw 0               ; DATA XREF: GFX_Swap_AppendUView+176w
// WZD dseg:9548                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:954A 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+mirrow_screen_18_books_icons dw 12h dup(0)
// WZD dseg:954A 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; DATA XREF: IDK_MirrorScreen_s6343B+88w ...
// WZD dseg:956E 00 00                                           mirror_screen_background dw 0           ; DATA XREF: Main_Screen_Load_Pictures+35Fw ...
// WZD dseg:956E                                                                                         ; single-loaded image
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
// WZD dseg:957A 00 00 00 00 00 00 00 00 00 00 00 00             IMG_ARMY_ItemSlots@ ISLT_ICONS <0>      ; DATA XREF: GFX_Swap_AppendItems+A3w ...
// WZD dseg:957A                                                                                         ; array of 6 appended reserved EMM headers in
// WZD dseg:957A                                                                                         ; GFX_Swap_Seg, each with one item slot image
// WZD dseg:9586 00 00                                           IMG_ItemScrn_Ok@ dw 0                   ; DATA XREF: ITEM_ScreenRedraw+3D6r ...
// WZD dseg:9586                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:9588 00 00                                           IMG_ItemScrn_Alch@ dw 0                 ; DATA XREF: ITEM_ScreenRedraw+3AEr ...
// WZD dseg:9588                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:958A 00 00                                           IMG_ItemScrn_HeroBr@ dw 0               ; DATA XREF: ITEM_ScreenRedraw+9Cr ...
// WZD dseg:958C 00 00                                           IMG_ItemScrn_BG@ dw 0                   ; DATA XREF: Items_Screen+2Fr ...
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
// WZD dseg:95F0 00 00                                           IMG_CTY_HalfRsc@ dw 0                   ; DATA XREF: City_Screen_Load_Pictures+2E4w ...
// WZD dseg:95F0                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:95F2 00 00                                           goto_booty_icon dw 0                    ; DATA XREF: Main_Screen_Load_Pictures+371w ...
// WZD dseg:95F2                                                                                         ; single-loaded image
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





// dseg:9644 00 00                                           _city_idx dw 0                          ; DATA XREF: City_Screen+55r ...
// dseg:9646 00                                              db    0
// dseg:9647 00                                              db    0

// WZD dseg:9648
SAMB_ptr UnitDraw_WorkArea;  // alloc in MoM_Init_Tables(), 60 PR, 960 DB

// dseg:964A 00                                              db    0
// dseg:964B 00                                              db    0

// WZD dseg:964C
// MoO2: ~== _ship_node
int16_t entities_on_movement_map[120];  //  12 * 10  MAP_WIDTH * MAP_HEIGHT

// dseg:973C                                                 ; unsigned int gsa_BACKGRND_3_IMG_CMB_Bottom_BG
// dseg:973C 00 00                                           gsa_BACKGRND_3_IMG_CMB_Bottom_BG dw 0   ; DATA XREF: Load_Combat_Background_Bottom+12w ...










// WZD dseg:92B6 00 00                                           
// IMG_USW_GrassBase@ dw 0                 ; DATA XREF: GFX_Swap_AppndCtScap+57Ew ...
SAMB_ptr IMG_USW_GrassBase;



// WZD dseg:9498 00 00                                           
// IMG_CTY_RED_Btn@ dw 0                   ; DATA XREF: City_Screen_Load_Pictures+115w ...
// BACKGRND.LBX, 24  REDBUTT red button
SAMB_ptr red_button_seg;
// WZD dseg:9498                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:9498                                                                                         ; 2 frame image (normal - clicked)

// WZD dseg:949A 00 00                                           
// IMG_CTY_RightBldTab@ dw 0               ; DATA XREF: GFX_Swap_AppendUView+1E9w ...
SAMB_ptr IMG_CTY_RightBldTab;
// WZD dseg:949A                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

// WZD dseg:949C 00 00                                           
// IMG_CTY_LeftBldTab@ dw 0                ; DATA XREF: GFX_Swap_AppendUView+1D2w ...
SAMB_ptr IMG_CTY_LeftBldTab;
// WZD dseg:949C                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

// WZD dseg:949E 00 00                                           IMG_OVL_EnemyCityBG@ dw 0               ; DATA XREF: IDK_EnemyCityScreen_s4A3F0+115w ...

// WZD dseg:94A0 00 00                                           
// IMG_CTY_10_Food@ dw 0                   ; DATA XREF: City_Screen_Load_Pictures+1EDw ...
SAMB_ptr IMG_CTY_10_Food[6];
// WZD dseg:94A0                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

// WZD dseg:94A2 00 00                                           IMG_CTY_10_Prod@ dw 0                   ; DATA XREF: sub_4DA19+2CEr
// WZD dseg:94A2                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:94A4 00 00                                           IMG_CTY_10_Gold@ dw 0                   ; DATA XREF: sub_4DA19+484r ...
// WZD dseg:94A4                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:94A6 00 00                                           IMG_CTY_10_Power@ dw 0                  ; DATA XREF: sub_4DA19+7A6r ...
// WZD dseg:94A6                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:94A8 00 00                                           IMG_CTY_10_Books@ dw 0                  ; DATA XREF: sub_4DA19+9B2r
// WZD dseg:94A8                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:94AA 00 00                                           IMG_CTY_Neg_10_Gold@ dw 0               ; DATA XREF: IDK_CityScreen_AddResourcesFields+44Er ...
// WZD dseg:94AA                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

// WZD dseg:94AC 00 00                                           
// IMG_CTY_Neg_10_Food@ dw 0               ; DATA XREF: City_Screen_Load_Pictures+20Bw ...
SAMB_ptr IMG_CTY_Neg_10_Food;
// WZD dseg:94AE 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+IMG_CTY_Rebels@ dw 0Eh dup(0)           ; DATA XREF: City_Screen_Load_Pictures+186w ...
// WZD dseg:94AE 00 00 00 00 00 00 00 00 00 00 00 00                                                     ; array of 14 appended reserved EMM headers in

// WZD dseg:94AE                                                                                         ; GFX_Swap_Seg, 1 rebel image per header
// WZD dseg:94CA 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+IMG_CTY_Farmers@ dw 0Eh dup(0)          ; DATA XREF: City_Screen_Load_Pictures:loc_46923w ...
// WZD dseg:94CA 00 00 00 00 00 00 00 00 00 00 00 00                                                     ; array of 14 appended reserved EMM headers in
// WZD dseg:94CA                                                                                         ; GFX_Swap_Seg, 1 farmer image per header

// WZD dseg:94E6 00 00                                           
// IMG_CTY_1_Food@ dw 0                    ; DATA XREF: City_Screen_Load_Pictures+1AEw ...
SAMB_ptr IMG_CTY_1_Food[5];
// WZD dseg:94E6                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

// WZD dseg:94E8 00 00                                           IMG_CTY_1_Prod@ dw 0                    ; DATA XREF: sub_4DA19+2D2r
// WZD dseg:94E8                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:94EA 00 00                                           IMG_CTY_1_Gold@ dw 0                    ; DATA XREF: sub_4DA19+488r ...
// WZD dseg:94EA                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:94EC 00 00                                           IMG_CTY_1_Power@ dw 0                   ; DATA XREF: sub_4DA19+7AAr ...
// WZD dseg:94EC                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:94EE 00 00                                           IMG_CTY_1_Book@ dw 0                    ; DATA XREF: sub_4DA19+9B6r
// WZD dseg:94EE                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

// WZD dseg:94F0 00 00                                           
// IMG_CTY_Neg_1_Food@ dw 0                ; DATA XREF: City_Screen_Load_Pictures+1CCw ...
SAMB_ptr IMG_CTY_Neg_1_Food;



// WZD dseg:952E                                                 END:  Main Screen Pictures
// WZD dseg:952E
// WZD dseg:952E
// WZD dseg:952E
// WZD dseg:9530 00 00 00 00 00 00                               IMG_CTY_EmptyHuts@ dw 3 dup(0)          ; DATA XREF: City_Screen_Load_Pictures:loc_46A47w ...
// WZD dseg:9530                                                                                         ; array of 3 appended res EMM hdrs in GFX_Swap_Seg
// WZD dseg:9536 00 00 00 00 00 00                               IMG_CTY_FilledHuts@ dw 3 dup(0)         ; DATA XREF: City_Screen_Load_Pictures+25Aw ...
// WZD dseg:9536                                                                                         ; array of 3 appended res EMM hdrs in GFX_Swap_Seg
// WZD dseg:953C 00 00                                           namecity_background_seg dw 0            ; DATA XREF: City_Screen_Load_Pictures+239w ...
// WZD dseg:953E 00 00                                           IMG_CTY_Outpost_BG@ dw 0                ; DATA XREF: City_Screen_Load_Pictures+222w ...

// WZD dseg:9540 00 00                                           
// IMG_USW_SideBtns_BG@ dw 0               ; DATA XREF: GFX_Swap_AppendUView+8Bw ...
SAMB_ptr unitview_button_background_seg;

// WZD dseg:9542 00 00                                           dw 0
// WZD dseg:9544 00 00                                           dw 0
// WZD dseg:9546 00 00                                           dw 0

// WZD dseg:9548 00 00                                           
// IMG_OVL_BuildBtn_BG@ dw 0               ; DATA XREF: GFX_Swap_AppendUView+176w
SAMB_ptr IMG_OVL_BuildBtn_BG;
// WZD dseg:9548                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

// WZD dseg:954A 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+mirrow_screen_18_books_icons dw 12h dup(0)
// WZD dseg:954A 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; DATA XREF: IDK_MirrorScreen_s6343B+88w ...
// WZD dseg:956E 00 00                                           mirror_screen_background dw 0           ; DATA XREF: Main_Screen_Load_Pictures+35Fw ...



// WZD dseg:9750                                                 ¿ END: maps / map grid fields - Uninitialized Data ?
// WZD dseg:9750
// WZD dseg:9752
// WZD dseg:9752                                                 ? BEGIN: City Screen ?
// WZD dseg:9752
// WZD dseg:9752 00 00                                           scanned_field__G_CTY_ClickedLabel dw 0  ; DATA XREF: City_Screen+2A9w ...

// WZD dseg:9754 00 00                                           
// IMG_OVL_UnitListBtm@ dw 0               ; DATA XREF: GFX_Swap_AppendUView+15Fw ...
SAMB_ptr IMG_OVL_UnitListBtm;
// WZD dseg:9754                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

// WZD dseg:9756 00 00                                           
// IMG_OVL_UnitList_BG@ dw 0               ; DATA XREF: GFX_Swap_AppendUView+148w ...
SAMB_ptr IMG_OVL_UnitList_BG;
// WZD dseg:9756                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

// WZD dseg:9758 00 00                                           IMG_CTY_NewBuild_BG@ dw 0               ; DATA XREF: City_Screen_Load_Pictures+74w ...
// WZD dseg:9758                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:975A 00 00                                           IMG_CTY_SplScrlDn@ dw 0                 ; DATA XREF: City_Screen_Load_Pictures+FEw ...
// WZD dseg:975A                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:975A                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:975C 00 00                                           IMG_CTY_SplScrlUp@ dw 0                 ; DATA XREF: City_Screen_Load_Pictures+E7w ...
// WZD dseg:975C                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:975C                                                                                         ; 2 frame image (normal - clicked)
// WZD dseg:975E 00 00 00 00 00 00 00 00 00 00 00 00             IMG_USW_ItemSlots ISLT_ICONS <0>        ; DATA XREF: GFX_Swap_AppendItems+15Bw ...
// WZD dseg:975E                                                                                         ; array of 6 appended reserved EMM headers in
// WZD dseg:975E                                                                                         ; GFX_Swap_Seg, each with one item slot image
// WZD dseg:976A 00 00                                           IMG_USW_ItemHelpBlt@ dw 0               ; DATA XREF: GFX_Swap_AppendItems+144w ...
// WZD dseg:976A                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:976C 00 00                                           IMG_USW_ItemHelp_BG@ dw 0               ; DATA XREF: GFX_Swap_AppendItems+12Dw ...
// WZD dseg:976C                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:976E 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+IMG_USW_ItemPowers IPOW_ICONS <0>       ; DATA XREF: GFX_Swap_AppendItems+40w ...
// WZD dseg:976E 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; array of 22 appended reserved EMM headers in
// WZD dseg:976E 00 00 00 00 00 00 00 00 00 00 00 00                                                     ; GFX_Swap_Seg, each with one item power image
// WZD dseg:979A 00 00                                           IMG_MOODWIZPortrait@ dw 0               ; DATA XREF: IDK_DiplAnim_s6FDA1+4Cr ...
// WZD dseg:979A                                                                                         ; 3 frame image (good, bad, neutral)

// WZD dseg:979C 00 00                                           
// IMG_USW_WaterBase@ dw 0                 ; DATA XREF: GFX_Swap_AppndCtScap+567w ...
SAMB_ptr IMG_USW_WaterBase;
// WZD dseg:979C                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

// WZD dseg:979E 00 00                                           
// IMG_USW_Portrt_Brdr@ dw 0               ; DATA XREF: GFX_Swap_AppendUView+200w ...
SAMB_ptr IMG_USW_Portrt_Brdr;
// WZD dseg:979E                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

// WZD dseg:97A0 00 00                                           
// IMG_USW_1_Gold@ dw 0                    ; DATA XREF: GFX_Swap_AppendUView+1BBw ...
SAMB_ptr IMG_USW_1_Gold;
// WZD dseg:97A0                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:97A0                                                                                         ; should have put this in the dseg:94ea pointer

// WZD dseg:97A2 00 00                                           
// IMG_CTY_Neg_1_Gold@ dw 0                ; DATA XREF: GFX_Swap_AppndCtScap+619w ...
SAMB_ptr IMG_CTY_Neg_1_Gold;
// WZD dseg:97A2                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

// WZD dseg:97A4 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
// IMG_USW_AbBorders@ dw 8 dup(0)          ; DATA XREF: GFX_Swap_AppendUView+DAw ...
SAMB_ptr IMG_USW_AbBorders[8];
// WZD dseg:97A4                                                                                         ; array of 8 appended reserved EMM headers in
// WZD dseg:97A4                                                                                         ; GFX_Swap_Seg, one for around each list slot

// WZD dseg:97B4 00 00                                           
// IMG_USW_ArrowDown@ dw 0                 ; DATA XREF: GFX_Swap_AppendUView+B9w ...
SAMB_ptr unitview_down_arrow_seg;
// WZD dseg:97B4                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:97B4                                                                                         ; 2 images (normal - clicked)

// WZD dseg:97B6 00 00                                           
// IMG_USW_ArrowUp@ dw 0                   ; DATA XREF: GFX_Swap_AppendUView+A2w ...
SAMB_ptr unitview_up_arrow_seg;
// WZD dseg:97B6                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:97B6                                                                                         ; 2 images (normal - clicked)

// WZD dseg:97B8 00 00                                           
// IMG_USW_Background@ dw 0                ; DATA XREF: GFX_Swap_AppendUView+74w ...
SAMB_ptr unitview_large_background_seg;
// WZD dseg:97B8                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

// WZD dseg:97BA 00 00                                           
// IMG_USW_UnitHire_BG@ dw 0               ; DATA XREF: GFX_Swap_AppendUView+5Dw ...
SAMB_ptr unitview_small_background_seg;
// WZD dseg:97BA                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

// WZD dseg:97BC 00 00                                           
// IMG_USW_Stats_Gold dw 0                 ; DATA XREF: GFX_Swap_AppendUView+12Aw ...
SAMB_ptr IMG_USW_Stats_Gold[15];

// WZD dseg:97DA 00 00                                           
// IMG_USW_Stat_Icons dw 0                 ; DATA XREF: GFX_Swap_AppendUView+102w ...
SAMB_ptr IMG_USW_Stat_Icons[15];
// WZD dseg:97DA                                                                                         ; array of 15 appended reserved EMM header in
// WZD dseg:97DA                                                                                         ; GFX_Swap_Seg, each with one regular attribute image

// WZD dseg:97F8 00                                              
// special_seg db    0                     ; DATA XREF: GFX_Swap_AppendUView+1Dw ...
SAMB_ptr special_seg[111];
// WZD dseg:97F8                                                                                         ; array of 145 appended reserved EMM headers in
// WZD dseg:97F8                                                                                         ; GFX_Swap_Seg, each with one image
// WZD dseg:97F8                                                                                         ; UU_Guises@ is also used to hold the diplomacy mirror
// WZD dseg:97F8                                                                                         ; image (BACKGRND.LBX entry 18, 5740 bytes in sandbox)

// WZD dseg:98D6 00                                              
// special2_seg db    0                    ; DATA XREF: GFX_Swap_AppendUView+40w
SAMB_ptr special2_seg[34];

// WZD dseg:991A 00 00                                           IMG_OVL_EZConfirmBG@ dw 0               ; DATA XREF: City_Screen_Load_Pictures+2A0w ...
// WZD dseg:991A                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:991C 00 00                                           IMG_OVL_EZBtm_BG@ dw 0                  ; DATA XREF: City_Screen_Load_Pictures+2CEw ...
// WZD dseg:991C                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:991E 00 00                                           IMG_OVL_EZBtn_BG@ dw 0                  ; DATA XREF: City_Screen_Load_Pictures+2B7w ...
// WZD dseg:991E                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:9920 00 00                                           CRP_DBG_Alt_T_State dw 0                ; DATA XREF: Main_Screen+47Fr ...

// WZD dseg:9920                                                 ? END: City Screen ?

// WZD dseg:9922                                                 BEGIN:  Main Screen Pictures





// dseg:998A END:  Main Screen Pictures

// WZD dseg:998C
// GUI_SmallWork_IMG@ dw 0
    // 96h paragraphs used for building GUI notification images
    // (although the pointer variable is also used for building combat figure images, after which it is reassigned)

// WZD dseg:998E
SAMB_ptr _screen_seg;
// allocated in MoM_Init_Tables()

// WZD dseg:9990
// _current_screen

// WZD dseg:9992 RP_GUI_GrowOutFrames dw 0               ; DATA XREF: VGA_PageFlip_FX+41r ...
// WZD dseg:9994 RP_GUI_GrowOutTop dw 0                  ; DATA XREF: VGA_PageFlip_FX+45r ...
// WZD dseg:9996 RP_GUI_GrowOutLeft dw 0                 ; DATA XREF: VGA_PageFlip_FX+49r ...

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
int16_t * events_table;

// WZD dseg:999C
int16_t _unit_stack_count;
// WZD dseg:999E
struct s_STACK _unit_stack[9];




// WZD dseg:9C90
SAMB_ptr TBL_OvlMovePaths_EMS;
// WZD dseg:9C94
// CONTX_Arc_NmeStrMap
// WZD dseg:9C98
// CONTX_Myr_NmeStrMap
// WZD dseg:9C9C
SAMB_ptr TBL_Catchments_EMS;

// WZD dseg:9CA0
// drake178: TBL_SharedTiles_EMS
uint8_t * square_shared_bits;               // alloc'd in Allocate_Data_Space()

// WZD dseg:9CA4
SAMB_ptr TBL_TempMoveMap_EMS;
// WZD dseg:9CA8
// COL_MinimapTiles

// WZD dseg:9CAC
SAMB_ptr TBL_MoveMaps_EMS;


// WZD dseg:9CB0
/*
    302 Paragraphs, 4832 Bytes
    4800 bytes used - 1-byte values, 2400 world squares, 2 planes
*/
uint8_t * TBL_Scouting;                     // load in Load_SAVE_GAM()

// WZD dseg:9CB4  
uint8_t * TBL_Terrain_Flags;                // load in Load_SAVE_GAM()
// WZD dseg:9CB8  
SAMB_ptr TBL_Terr_Specials;                 // load in Load_SAVE_GAM()

// WZD dseg:9CBC  
// SAMB_ptr _CITIES;
// struct s_CITY _CITIES[CITY_COUNT_MAX];  // 100 * sizeof(114)
struct s_CITY * _CITIES;

// WZD dseg:9CC0
// drake178: TBL_Encounters
/*
    alloc in Allocate_Data_Space()
    load in Load_SAVE_GAM() - read 102 of 24

*/
// AKA TBL_Lairs
struct s_LAIR * TBL_Lairs;
struct s_LAIR * _LAIRS;

// WZD dseg:9CC4  
// struct s_TOWER _TOWERS[NUM_TOWERS];  // 6 * sizeof(4)
// struct s_TOWER _TOWERS[];
struct s_TOWER * _TOWERS;

// WZD dseg:9CC8  
struct s_FORTRESS * _FORTRESSES;

// WZD dseg:9CCC  
struct s_NODE * TBL_Nodes;

// WZD dseg:9CD0
uint8_t * TBL_Landmasses;  // 12Eh 302d PR 302*16=4832, in World_Data
// WZD dseg:9CD4
SAMB_ptr UU_TBL_2;
// WZD dseg:9CD8
SAMB_ptr UU_TBL_1;
// WZD dseg:9CDC
// AKA TBL_Maps;
// SAMB_ptr _world_maps;
uint8_t * _world_maps;

// WZD dseg:9CE0
// drake178: 14 individual pointers, one to each row of the table
SAMB_ptr TBL_Unrest[14];
SAMB_ptr TBL_Unrest_Hack;

// WZD dseg:9D18 00 00                                           IMG_CTY_Bldngs_Wall@ dw 0               ; DATA XREF: GFX_Swap_AppndCtScap+602w ...
// WZD dseg:9D18                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

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
SAMB_ptr TBL_Hero_Names;  // 25h 37d PR 37*16=592

// WZD dseg:9ECA
struct s_WIZARD _players[6];
// struct s_WIZARD * _players;


// WZD dseg:BB7A
// // // struct s_HLP_ENTRY _help_entries[50];
// // uint8_t _help_entries[500];
// int16_t _help_entries[250];
char _help_entries[500];


// WZD dseg:BD6E 
// config_mom s_CONFIG_MOM
// WZD dseg:BD80 
// UU_IMG_OVL_WorkMark@

// WZD dseg:974A
// AKA G_OVL_MapDisplay_Y
int16_t _prev_world_y;
// WZD dseg:974C
// G_OVL_MapDisplay_X
int16_t _prev_world_x;

// WZD dseg:BD82 
// int16_t OVL_Map_CenterY;  // AKA _active_world_y
int16_t _active_world_y;  // AKA _active_world_y
// WZD dseg:BD84 
// int16_t OVL_Map_CenterX;  // AKA _active_world_x
int16_t _active_world_x;  // AKA _active_world_x

// WZD dseg:BD86 
int16_t _map_plane;  // AKA _world_plane
// WZD dseg:BD88 
int16_t _map_y;  // AKA _curr_world_y
// WZD dseg:BD8A 
int16_t _map_x;  // AKA _curr_world_x

// WZD dseg:BD8C
int16_t _human_player_idx = 0;

// WZD dseg:BD8E
int16_t _unit;
int16_t _turn;
int16_t _units;  // MoO2 _NUM_SHIPS
int16_t _cities;
int16_t _difficulty;
int16_t _magic;
int16_t _landsize;
int16_t _num_players;  // MoO2 _NUM_PLAYERS    New Game: magic_set.opponents + 1
struct s_GAME_DATA game_data;
// WZD dseg:BD9E
// magic_set s_MAGIC_SET

// ovr150 MainScr_Maps  // WZD dseg:CB5C
// ovr150 MainScr_Maps  // AKA OVL_NewMapDrawing
// ovr150 MainScr_Maps  int16_t draw_map_full;  //; determines whether non-animated terrain tiles will be redrawn or not







// WZD dseg:BF9A
int16_t CTY_EnchantCount;

// WZD dseg:BF9C
char * CTY_EnchantOwners;

// WZD dseg:BF9E
char * CTY_EnchantList;





// WZD dseg:BFB6 00 00                                           production_screen_return_screen dw 0    ; DATA XREF: City_Screen+657w ...
// WZD dseg:BFB6                                                                                         ; {1: CityList Screen, 2: City Screen}
// WZD dseg:BFB8 00 00                                           dw 0

// WZD dseg:BFBA
char * GUI_String_2;

// WZD dseg:BFBC
char * GUI_String_1;





// WZD dseg:C03E                                                 BEGIN: Main_Screen
// WZD dseg:C03E
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
// WZD dseg:C052 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+Unit_Window_Fields dw 9 dup(0)          ; DATA XREF: IDK_CityScreen_AddFields+2Cw ...
// WZD dseg:C064 00 00                                           CRP_OverlandVar_3 dw 0                  ; DATA XREF: Main_Screen+9Ar ...
// WZD dseg:C066 00 00                                           OVL_Path_Length dw 0                    ; DATA XREF: Main_Screen+CBDr ...
// WZD dseg:C068 00 00                                           OVL_StackHasPath dw 0                   ; DATA XREF: City_Screen:loc_47BE1w ...
// WZD dseg:C068                                                                                         ; set to 0 after display-sorting the active stack
// WZD dseg:C068                                                                                         ; set to 1 if road-building, but the unit is not on any
// WZD dseg:C068                                                                                         ;   of the plotted line tiles (before returning)
// WZD dseg:C068                                                                                         ; set to 1 if road-building, and tiles left to do
// WZD dseg:C068                                                                                         ; set to 1 if moving with path left to go
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

// WZD dseg:C082 00 00                                           _unit_window_start_y dw 0               ; DATA XREF: GAME_Overland_Init+130w ...
// WZD dseg:C082                                                                                         ; set to 4Fh
// WZD dseg:C082
// AKA OVL_STKUnitCards_Top
int16_t _unit_window_start_y;

// WZD dseg:C084 00 00                                           _unit_window_start_x dw 0               ; DATA XREF: GAME_Overland_Init+12Aw ...
// WZD dseg:C084                                                                                         ; set to 0F7h
// WZD dseg:C084
// AKA OVL_STKUnitCards_Lft
int16_t _unit_window_start_x;

// WZD dseg:C086 00 00                                           _reduced_map_seg dw 0                   ; DATA XREF: sub_49F38+15w ...
// WZD dseg:C086                                                                                         ; 12Fh LBX_Alloc_First paragraphs in the
// WZD dseg:C086                                                                                         ; LBX_Sandbox_Segment
// WZD dseg:C086
SAMB_ptr _reduced_map_seg;
// ; 12Fh LBX_Alloc_First paragraphs in the LBX_Sandbox_Segment

// WZD dseg:C088 00 00                                           NIU_MainScreen_local_flag dw 0          ; DATA XREF: Main_Screen:loc_4FB72w ...
// WZD dseg:C088                                                                                         ; drake178: CRP_OverlandVar
// WZD dseg:C088                                                                                         ; only XREF Main_Screen(), sets TRUE, never tests
// WZD dseg:C08A 00 00                                           SND_Bkgrnd_Track dw 0                   ; DATA XREF: IDK_SND_BkGrnd_s518AE+1Er ...
// WZD dseg:C08A                                                                                         ; the current power-based background music track index
// WZD dseg:C08C 00 00                                           word_42B2C dw 0                         ; DATA XREF: sub_499B1+E0r ...
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
int16_t IDK_CityList_list_first_item;

// WZD dseg:C09C
int16_t citylist_city_count;

// WZD dseg:C09E
int16_t citylist_item_count;

// WZD dseg:C0A0
int16_t citylist_item_fields[9];

// WZD dseg:C0B2 00 00                                           word_42B52 dw 0                         ; DATA XREF: CityList_Screen+4Cw
// WZD dseg:C0B4 00 00                                           word_42B54 dw 0                         ; DATA XREF: CityList_Screen+46w

// WZD dseg:C0B6
SAMB_ptr city_list;

// WZD dseg:C0B8
SAMB_ptr list_cities;

// WZD dseg:C0BA
int16_t armylist_item_scanned_field;

// WZD dseg:C0BC 00 00                                           dw 0
// WZD dseg:C0BE 00 00                                           word_42B5E dw 0                         ; DATA XREF: CityList_Screen_Draw:loc_58114w
// WZD dseg:C0C0 00 00                                           word_42B60 dw 0                         ; DATA XREF: ArmyList_Add_Fields+92w ...

// WZD dseg:C0C0
int16_t IDK_armylist_row_count;

// WZD dseg:C0C2
byte_ptr IDK_armylist_row_fields;

// WZD dseg:C0C4
int16_t armylist_army_count;

// WZD dseg:C0C6
int16_t list_item_count;

// WZD dseg:C0C8
int16_t list_first_item;

// WZD dseg:C0CA
int16_t armylist_unit_count;

// WZD dseg:C0CC
// MoO2  Module: COLREFIT  _n_ships_in_list  Address: 02:00197A28
int16_t armylist_item_count;

// WZD dseg:C0CE 00 00                                           word_42B6E dw 0                         ; DATA XREF: ArmyList_Screen+2Bw
// WZD dseg:C0D0 00 00                                           word_42B70 dw 0                         ; DATA XREF: ArmyList_Screen+25w

// WZD dseg:C0D2
int16_t armylist_hero_portrait_start_y;

// WZD dseg:C0D4
int16_t armylist_hero_portrait_start_x;

// WZD dseg:C0D6
byte_ptr armylist_hero_portrait_unit_indices;

// WZD dseg:C0D8
int16_t armylist_hero_portrait_count;

// WZD dseg:C0DA
 byte_ptr armylist_hero_portrait_fields;

// WZD dseg:C0DC
byte_ptr armylist_unit_fields;

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
char * armylist_20x20[20];

// WZD dseg:C110
SAMB_ptr list_armies;                       // 108 bytes;  2-byte values, 6 x 9  armies x units;

// WZD dseg:C112
byte_ptr armylist_world_p_1000;

// WZD dseg:C114
SAMB_ptr armylist_list_item_count;          // 54 bytes;  6 x 9; 

// WZD dseg:C116
byte_ptr armylist_world_y_1000;

// WZD dseg:C118
byte_ptr armylist_world_x_1000;

// WZD dseg:C11A 00 00                                           dw 0

//                                          ¿ END: List / ArmyList / CityList - Uninitialized Data ?



//                                          ¿ BEGIN: ~ Reduced/World Map - Uninitialized Data ?

// WZD dseg:C11C 00 00                                           minimap_height dw 0                     ; DATA XREF: Draw_Maps:loc_59CF4r ...
// WZD dseg:C11C 00 00                                           reduced_map_height dw 0                 ; DATA XREF: Draw_Maps:loc_59CF4r ...





// WZD dseg:C47C
int16_t OVL_Action_Plane;
// WZD dseg:C47E
int16_t OVL_Action_YPos;
// WZD dseg:C480
int16_t OVL_Action_XPos;



// WZD dseg:C5DC                                                 ¿ BEGIN: Move_Stack() || UNITMOVE ?

// WZD dseg:C5DC 00 00                                           OVL_SWardTriggered dw 0                 ; DATA XREF: Move_Stack+42Cw ...

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

// IDGI  // WZD dseg:C5F0
// IDGI  /*
// IDGI      Move_Stack() uses `mov al; cbw` so, definitely DB
// IDGI  */
// IDGI  uint8_t OVL_Path_Costs[35];
// IDGI  // WZD dseg:C5F0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        
// IDGI  // WZD dseg:C613 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+UU_Botched_Cost_Array db 55h dup(0)
// IDGI  // WZD dseg:C667 00                                              
// IDGI  uint8_t IDK_MovePath_DestinationY[36];  // DATA XREF: OVL_MoveUnitStack+2C9r
// IDGI  // WZD dseg:C668
// IDGI  /*
// IDGI      Move_Stack() uses `mov al; cbw` so, definitely DB
// IDGI  */
// IDGI  // uint8_t OVL_Path_Ys[35];
// IDGI  // WZD dseg:C68B 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+UU_Botched_Y_Array db 55h dup(0)
// IDGI  // WZD dseg:C6DF 00                                              
// IDGI  uint8_t IDK_MovePath_DestinationX[36];  // DATA XREF: OVL_MoveUnitStack+2BEr
// IDGI  // WZD dseg:C6E0
// IDGI  /*
// IDGI      Move_Stack() uses `mov al; cbw` so, definitely DB
// IDGI  */
// IDGI  // uint8_t OVL_Path_Xs[35];
// IDGI  WZD dseg:C703 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+UU_Botched_X_Array db 55h dup(0)

// WZD dseg:C5F0
uint8_t OVL_Path_Costs[118];
// WZD dseg:C666
uint8_t Fst_Dst_Y;
// WZD dseg:C667
uint8_t Scd_Dst_Y;
// WZD dseg:C668
uint8_t MovePath_Y[118];
// WZD dseg:C6DE
uint8_t Fst_Dst_X;
// WZD dseg:C6DF
uint8_t Scd_Dst_X;
// WZD dseg:C6E0
uint8_t MovePath_X[118];
// WZD dseg:C756
uint8_t IDK_MovePath[62];



// WZD dseg:C794 00                                              STK_HasNatureUnits db 0                 ; DATA XREF: CTY_CheckSpellWard:loc_7E798w ...
// WZD dseg:C795 00                                              STK_HasSorceryUnits db 0                ; DATA XREF: CTY_CheckSpellWard+C3w
// WZD dseg:C796 00                                              STK_HasChaosUnits db 0                  ; DATA XREF: CTY_CheckSpellWard+121w ...
// WZD dseg:C797 00                                              STK_HasLifeUnits db 0                   ; DATA XREF: CTY_CheckSpellWard+1A2w
// WZD dseg:C798 00                                              STK_HasDeathUnits db 0                  ; DATA XREF: CTY_CheckSpellWard+200w ...
// WZD dseg:C799 00                                              UU_Global_Byte db 0

// WZD dseg:C799                                                 ¿ END: Move_Stack() || UNITMOVE ?

// WZD dseg:C79A 00 00                                           CMB_HumanTurn dw 0                      ; DATA XREF: CMB_TacticalCombat+26Dw ...


// WZD dseg:C79E
SAMB_ptr IMG_USW_HeroPortrt;












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
byte_ptr _VGAFILEH_seg;

// WZD dseg:E5F4                                                                                         ; 5 reserved pages
// WZD dseg:E5F6 00 00                                           g_EmmRsvd dw 0                          ; DATA XREF: EMM_LBX_EntryLoader:@@EmmHndlNmExistsr ...
// WZD dseg:E5F8 00 00                                           EmmHndlNbr_YOMOMA dw 0                  ; DATA XREF: EMM_Startup+1Bw ...
// WZD dseg:E5F8                                                                                         ; 1 non-reserved page
// WZD dseg:E5FA 00 00                                           EMM_OK dw 0                             ; DATA XREF: EMM_Startup:loc_16E33w ...
// WZD dseg:E5FA                                                                                         ; set to 1 after creating the YO MOMA handle
// WZD dseg:E5FC 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+EMM_Table EMM_Record 28h dup(<0>)       ; DATA XREF: EMM_Startup+8Bt ...
// WZD dseg:E7DC 00 00                                           EMM_MinKB dw 0                          ; DATA XREF: EMS_SetMinKB+6w ...
// WZD dseg:E7DC                                                                                         ; set to 2700 in _main
// WZD dseg:E7DE
// WZD dseg:E7DE                                                 BEGIN:  Fonts
// WZD dseg:E7DE
// WZD dseg:E7DE 00 00                                           gsa_VGAFILEH_Header dw 0                ; DATA XREF: Load_Font_File+78w ...




// WZD dseg:E872                                                 ¿ BEGIN: fields, intput, ... ?
// WZD dseg:E872                                                 ¿ BEGIN: Help - Uninitialized Data ?

// WZD dseg:E872
// AKA _help_list
// struct s_HLP_ENTRY * help_struct_pointer[50];
/*
    ¿ pointer to an array of pointers of data type 'struct s_HLP_ENTRY' ?
*/
// struct s_HLP_ENTRY ** help_struct_pointer;
struct s_HLP_ENTRY * help_struct_pointer;


// WZD dseg:E872                                                 ¿ END: Help - Uninitialized Data ?
// ...
// ...
// ...
// WZD dseg:82A6                                                 END: Fields, Input, Mouse, Keyboard

