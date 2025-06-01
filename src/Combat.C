/*
    Combat

    WIZARDS.EXE
        ovr090
        ovr091
        ovr096
        ovr098
        ovr099  ¿ MoO2  Module: CMBTDRW1 ?
        ovr103
        ovr105
        ovr110
        ovr112
        ovr114  ¿ MoO2  Module: CMBTAI ?
        ovr116
        ovr122
        ovr123
        ovr124
        ovr139
        ovr153
        ovr154
        ovr155
        ovr163

*/

#include "Combat.H"

#include "MOX/FLIC_Draw.H"
#include "MOX/MOM_Data.H"
#include "MOX/MOX_DAT.H"  /* _screen_seg */
#include "MOX/MOX_ITOA.H"  /* mox_itoa() */
#include "MOX/MOX_SET.H"  /* magic_set */
#include "MOX/sdl2_Audio.H"
#include "MOX/SOUND.H"

#include "CMBTDEF.H"

#include "City_ovr55.H"
#include "CITYCALC.H"
#include "DIPLOMAC.H"
#include "Help.H"
#include "Items.H"
#include "Lair.H"
#include "LOADER.H"
#include "MainScr.H"
#include "MainScr_Maps.H"
#include "MOM_DBG.H"
#include "MOM_DEF.H"
#include "NEXTTURN.H"
#include "RACETYPE.H"
#include "SBookScr.H"
#include "special.H"
#include "Spellbook.H"  /* Combat_Spellbook_Build__WIP() */
#include "Spells131.H"
#include "Spells133.H"
#include "Terrain.H"
#include "TerrType.H"
#include "UnitMove.H"   // WTFMATE
#include "UnitStat.H"
#include "UNITTYPE.H"   // WTFMATE
#include "UnitView.H"
#include "WZD_o059.H"

#include <stdlib.h>     /* abs(); itoa(); */
#include <string.h>     /* memcpy() memset(), strcat(), strcpy(), stricmp() */

#include "FIGURES8_LBX_063.H"

int16_t DBG_Ignore_Computer_Player = ST_FALSE; // {ST_FALSE, ST_TRUE} skips computer player's turn - AI_MoveBattleUnits__WIP()
int16_t DBG_player_idx = HUMAN_PLAYER_IDX;
int16_t DBG_cgx = 0;  // 14
int16_t DBG_cgy = 0;  // 12
int16_t DBG_target_cgx = 0;  // 13 W
int16_t DBG_target_cgy = 0;  // 11 N
int16_t DBG_PFA_1311_set = ST_FALSE;
int16_t DBG_PFA_1411_set = ST_FALSE;
int16_t DBG_PFA_1511_set = ST_FALSE;
int16_t * DBG_ptr_CMB_NearBuffer_3 = 0;

// DELETE  #define DEBUG_UNIT_IDX          825
// DELETE  #define DEBUG_FIGURE_SET_IDX    7  // DBG_figure_set_idx: 7
// DELETE  #define DEBUG_UNIT_TYPE         ut_HMenPikemen  // ut_HMenPikemen  = 112,  /* FIGURES8.LBX, 056    HMPIKE*/
// DELETE  #define DEBUG_FIGURE_COUNT      8

extern uint8_t DBG_debug_flag;
// uint8_t DBG_debug_flag = ST_FALSE;

// ~ void Main_Screen_Draw_Debug_Information(void);
void Combat_Screen_Draw_Debug_Information(void);



/*

MoO2
Module: MOX
    data (0 bytes) _auto_combat_flag
    Address: 02:0018AB10

    data (0 bytes) _auto_combat
    Address: 02:00191900

    data (0 bytes) _auto_cmbt_button_value
    Address: 02:00191902

*/



enum e_COMBAT_SPELL_TARGET_TYPE
{
    CTT_Tile_NoUnit  = 1,
    CTT_Tile  = 2,
    CTT_Wall  = 3,
    CTT_Tile_NoUnitA  = 4,
    CTT_Tile_NoUnitD  = 5,

    CTT_EnemyUnit  = 10,
    CTT_EnemyNU  = 11,

    CTT_FriendlyUnit  = 20,
    CTT_FriendlyNU  = 21,

    CTT_FriendlyHero  = 23,

    CTT_DispelMagic  = 30
};



// WZD dseg:50AC                                                 BEGIN:  ovr098
/*
¿ could just be [15][2] ?
*/
struct s_COMBAT_ENCHANTMENT_ICON_DATA combat_enchantment_icon_data[NUM_COMBAT_ENCHANTMENTS] = 
{
    {  0, HLP_LIGHT             },
    {  1, HLP_DARKNESS          },
    {  2, HLP_WARP_REALITY      },
    {  3, HLP_BLACK_PRAYER      },
    {  4, HLP_WRACK             },
    {  5, HLP_METAL_FIRES       },
    {  6, HLP_PRAYER            },
    {  7, HLP_HIGH_PRAYER       },
    {  8, HLP_TERROR            },
    {  9, HLP_CALL_LIGHTNING    },
    { 10, HLP_COUNTER_MAGIC     },
    { 11, HLP_MASS_INVISIBILITY },
    { 12, HLP_ENTANGLE          },
    { 13, HLP_MANA_LEAK_2       },
    { 14, HLP_BLUR              }
};




// WZD dseg:50E8                                                 BEGIN:  ovr090 - Intialized Data  (Tactical Combat)

// WZD dseg:50E8
char cnst_SOUNDFX_File2[] = "SOUNDFX";
// WZD dseg:50EF
char str_empty_string__ovr090[] = "";
// WZD dseg:50F0
char cnst_MUSIC_File7[] = "MUSIC";
// WZD dseg:50F6
char str_hotkey_ESC__ovr090[] = "\x1B";
// WZD dseg:50F8
char cnst_HOTKEY_S_3[] = "S";
// WZD dseg:50FA
char cnst_HOTKEY_W_2[] = "W";
// WZD dseg:50FC
char cnst_HOTKEY_D_5[] = "D";
// WZD dseg:50FE
char cnst_HOTKEY_U_5[] = "U";
// WZD dseg:5100
char cnst_HOTKEY_A_4[] = "A";
// WZD dseg:5102
char cnst_HOTKEY_F[] = "F";
// WZD dseg:5104
char cnst_HOTKEY_SPACE_4[] = " ";
// WZD dseg:5106
char cnst_FleeConfirm_Msg[] = "Do you wish to flee?";

// WZD dseg:511B 00                                              align 2

// WZD dseg:511B                                                 END:  ovr090 - Intialized Data  (Tactical Combat)



// WZD dseg:5660                                                 ¿ BEGIN: ovr096 - Strings ?

// WZD dseg:5660
char cnst_TreatyAtk_Msg1[] = "You have a treaty with ";
// WZD dseg:5678
char cnst_TreatyAtk_Msg2[] = ".  Do you still wish to attack?";

// WZD dseg:5678                                                 ¿ END: ovr096 - Strings ?



// WZD dseg:56CC                                                 BEGIN:  ovr098 - Initialized Data

// WZD dseg:56CC
char str_empty_string__ovr098[] = "";
// WZD dseg:56CD
char cnst_FleeLoss_Msg[] = "While fleeing you lost:";
// WZD dseg:56E4
char cnst_Space_6[] = " ";
// WZD dseg:56E6
char cnst_Dot8[] = ".";

// WZD dseg:56E6                                                 END:  ovr098 - Initialized Data



// WZD dseg:56E8                                                 BEGIN:  ovr099 - Initialized Data

// WZD dseg:56E8 61 62 63 64                                     COL_CMBUI_Banner0 db 97, 98, 99, 100    ; DATA XREF: Tactical_Combat_Draw+1EAt ...
// WZD dseg:56EC 42 43 44 45                                     COL_CMBUI_Banner1 db 66, 67, 68, 69
// WZD dseg:56F0 21 22 23 24                                     COL_CMBUI_Banner2 db 33, 34, 35, 36
// WZD dseg:56F4 C9 CA CB A6                                     COL_CMBUI_Banner3 db 201, 202, 203, 166
// WZD dseg:56F8 A0 A1 A2 A3                                     COL_CMBUI_Banner4 db 160, 161, 162, 163
// WZD dseg:56FC 1C 1B 1A 19                                     COL_CMBUI_Banner5 db 28, 27, 26, 25
uint8_t COL_CMBUI_Banner[6][4] = {
    { 97,  98,  99, 100}, 
    { 66,  67,  68,  69}, 
    { 33,  34,  35,  36}, 
    {201, 202, 203, 166}, 
    {160, 161, 162, 163}, 
    { 28,  27,  26,  25}, 
};
// WZD dseg:5700
char str_Monsters__ovr099[] = "Monsters";
// WZD dseg:5709
char str_Raiders__ovr099[] = "Raiders";
// WZD dseg:5711
char cnst_Cmbt_Immobile[] = "All units are immobilized. Select an action.";

// WZD dseg:573E
char cnst_VortexMove_1[] = "Move";
// WZD dseg:5743
char cnst_VortexMove_2[] = "Magic";
// WZD dseg:5749
char cnst_VortexMove_3[] = "Vortex";
// WZD dseg:5750
char cnst_VortexMove_4[] = "1 Space";

// WZD dseg:5758
char cnst_CMB_Skill[] = "Skill:";
// WZD dseg:575F
char cnst_CMB_Mana[] = "Mana:";
// WZD dseg:5765
char cnst_CMB_Range[] = "Range:";

// WZD dseg:5765                                                 END:  ovr099 - Initialized Data



// WZD dseg:5852                                                 BEGIN:  ovr103 - Initialized Data

// WZD dseg:5852
char compix_lbx_file__ovr103[] = "COMPIX";

// WZD dseg:5858
char str_empty_string__ovr103[] = "";

// WZD dseg:5859
char cnst_HOTKEY_Esc13[] = "\x1B";

// WZD dseg:585B
char cnst_SpaceSpells[] = " Spells";

// WZD dseg:5863
char cnst_Crusade[]= "Crusade";
// WZD dseg:586B
char cnst_HolyArms[] = "Holy Arms";
// WZD dseg:5875
char cnst_CharmofLife[] = "Charm Of Life";
// WZD dseg:5883
char cnst_ZombieMastery[] = "Zombie Mastery";

// WZD dseg:5892
char cnst_SorcNodeDispel[] = "Dispells Non-Sorcery";
// WZD dseg:58A7
char cnst_ChaosNodeDispel[] = "Dispells Non-Chaos";
// WZD dseg:58BA
char cnst_NatNodeDispel[] = "Dispells Non-Nature";

// WZD dseg:58CE
char cnst_SorceryAura[] = "Sorcery Node Aura";
// WZD dseg:58E0
char cnst_NatureAura[] = "Nature Node Aura";
// WZD dseg:58F1
char cnst_ChaosAura[] = "Chaos Node Aura";

// WZD dseg:5901
char cnst_CloudOfDarkness[] = "Cloud Of Darkness";
// WZD dseg:5913
char cnst_HolyLight[] = "Holy Light";

// WZD dseg:591E
char cnst_ChaosSurge[] = "Chaos Surge";
// WZD dseg:592A
char cnst_EternalNight[] = "Eternal Night";

// WZD dseg:5938
char cnst_CmbCastError_2[] = "You are unable to throw spells at this time.";
// WZD dseg:5965
char cnst_CmbCastError_1[] = "You may only cast once per turn.";
// WZD dseg:5986
char str_cancel__ovr103[] = "Cancel";
// WZD dseg:598D
char cnst_CasterSelectMsg[] = "Who Will Cast";

// WZD dseg:599B 00                                              align 2

// WZD dseg:599B                                                 END:  ovr103 - Initialized Data



// WZD dseg:59A6                                                 BEGIN:  ovr105 - Initialized Data

// WZD dseg:59A6
char cnst_Hits_2[] = "Hits";

// WZD dseg:59AB
char cnst_mp[] = "mp";

// WZD dseg:59AE
char cnst_Ammo[] = "ammo";

// WZD dseg:59B3 00                                              align 2

// WZD dseg:59B3                                                 END:  ovr105 - Initialized Data





// WZD dseg:5AF2                                                 BEGIN:  ovr112 - Initialized Data

// WZD dseg:5AF2
char cnst_Counter_Magic[] = "Counter Magic";
// WZD dseg:5AFF
char str_empty_string__ovr112[] = "";
// WZD dseg:5B00
char hlpentry_lbx_file__ovr112[] = "hlpentry";
// WZD dseg:5B09
char str_hotkey_F__ovr112[] = "F";
// WZD dseg:5B0B
char str_hotkey_B__ovr112[] = "B";
// WZD dseg:5B0D 1B 00                                           str_hotkey_ESC__ovr112 db 1Bh,0              ; DATA XREF: Combat_Spellbook_Screen+6E0o
// WZD dseg:5B0F
char message_lbx_file__ovr112[] = "message";
// WZD dseg:5B17 00                                              align 2

// WZD dseg:5B17                                                 END:  ovr112 - Initialized Data



// WZD dseg:5B18                                                 BEGIN:  ovr113 - Initialized Data

// WZD dseg:5B18
char cmbtfx_lbx_file__ovr113[] = "cmbtfx";
// WZD dseg:5B1E
char str_empty_string__ovr113[] = "";
// WZD dseg:5B1F
char str_hotkey_ESC__ovr113[] = "\x1B";
// WZD dseg:5B21
char cnst_CmbSpellMsg1[] = "Select a target for a ";
// WZD dseg:5B38
char cnst_SpaceSpellDot_3[] = " spell.";
// WZD dseg:5B40
char message_lbx_file__ovr113__1of2[] = "message";
// WZD dseg:5B48 54 68 61 74 20 75 6E 69 74 20 61 6C 72 65 61 64+cnst_SpellError_1 db 'That unit already has ',0
// WZD dseg:5B5F 20 63 61 73 74 20 6F 6E 20 69 74 00             cnst_SpellError_2 db ' cast on it',0    ; DATA XREF: CMB_TargetSpell+5CBo ...
// WZD dseg:5B6B 54 68 61 74 20 75 6E 69 74 20 69 73 20 69 6D 6D+cnst_SpellError_3 db 'That unit is immune to Death spells',0
// WZD dseg:5B8F 6D 65 73 73 61 67 65 2E 6C 62 78 00             message_lbx_file__ovr113__2of2 db 'message.lbx',0
// WZD dseg:5B9B 73 6F 75 6E 64 66 78 00                         soundfx_lbx_file__ovr113 db 'soundfx',0 ; DATA XREF: CMB_RangedAnim__STUB+14Bo ...
// WZD dseg:5BA3
char cnst_CombatCast_1[] = " has cast ";
// WZD dseg:5BAE
char cnst_CombatCast_2[] = "The ";
// WZD dseg:5BB3
char cnst_CombatCast_3[] = " have cast ";
// WZD dseg:5BBF
char cnst_CombatCast_4[] = "summon demon";

// WZD dseg:5BBF                                                 END:  ovr113 - Initialized Data



// WZD dseg:5E9E                                                 BEGIN:  ovr122 - Initialized Data

// WZD dseg:5E9E
char message_lbx_file__ovr122[] = "message";

// WZD dseg:5E9E                                                 END:  ovr122 - Initialized Data





// WZD dseg:5EA6                                                 ¿ BEGIN:  ovr123 - Strings ?

// WZD dseg:5EA6
char cnst_RazeCity_Msg[] = "Do you wish to completely destroy this city?";

// WZD dseg:5ED3
char cnst_Zombie_Msg_1[] = " units have";
// WZD dseg:5EDF
char cnst_Zombie_Msg_2[] = " unit has";
// WZD dseg:5EE9
char cnst_Zombie_Msg_3[] = " been transformed into zombies.";

// WZD dseg:5F07
char cnst_Dot9[] = ".";  // string optimizer

// WZD dseg:5F09
char cnst_Undead_Msg_1[] = " units rise from the dead to serve ";
// WZD dseg:5F2D
char cnst_Undead_Msg_2[] = " unit rises from the dead to serve ";
// WZD dseg:5F51
char cnst_Undead_Msg_3[] = "you.";
// WZD dseg:5F56
char cnst_Undead_Msg_4[] = " their creator.";
// WZD dseg:5F56
char cnst_Undead_Msg_5[] = " its creator.";

// WZD dseg:5F74
char scroll_lbx_file__ovr123[] = "scroll";

// WZD dseg:5F7B
char help_lbx_file__ovr123[] = "help";

// WZD dseg:5F80
char cnst_HOTKEY_R = 'r';

// WZD dseg:5F82
char cnst_HOTKEY_R_2 = 'R';

// WZD dseg:5F84
char cnst_HOTKEY_Esc1A = '\x1B';

// WZD dseg:5F86
char message_lbx_file__ovr123[] = "message";

// WZD dseg:5F8E
char cnst_CityLost_Msg[] = " has been conquered";

// WZD dseg:5FA2
char cnst_NewRuins_Msg[] = " has been reduced to ruins";

// WZD dseg:5FBD
char cnst_CityRaided_Msg[] = " has fallen to raiders";

// WZD dseg:5FD4
char cnst_ScrlFame_Msg_1[] = "You have ";

// WZD dseg:5FDE
char cnst_ScrlFame_Msg_2[] = "lost ";

// WZD dseg:5FE4
char cnst_ScrlFame_Msg_3[] = "gained ";

// WZD dseg:5FEC
char cnst_ScrlFame_Msg_4[] = " fame.";

// WZD dseg:5FF3
char cnst_ScrlGold_Msg[] = " gold pieces were looted.";

// WZD dseg:600D
char cnst_ScrlRaze_Msg[] = "The city has been completly destroyed.";

// WZD dseg:6034
char cnst_ScrlPop_Msg[] = " thousand inhabitants killed.";

// WZD dseg:6052
char cnst_ScrlBldng_Msg[] = "Destroyed Buildings";

// WZD dseg:6052                                                 ¿ END:  ovr123 - Strings ?



// WZD dseg:6066                                                 BEGIN:  ovr124 - Initialized Data

// WZD dseg:6066
char soundfx_lbx_file__ovr124__1of2[] = "soundfx";
// WZD dseg:606D
char str_empty_string__ovr124[] = "";
// WZD dseg:606E
char newsound_lbx_file__ovr124__1of2[] = "newsound";
// WZD dseg:6077
char cmbtsnd_lbx_file__ovr124[] = "cmbtsnd";
// WZD dseg:607F
char soundfx_lbx_file__ovr124__2of2[] = "SOUNDFX";
// WZD dseg:6087
char newsound_lbx_file__ovr124__2of2[] = "NEWSOUND";
// WZD dseg:6090
char cnst_RazeCity_Msg2[] = "Do you wish to completely destroy this city?";
// WZD dseg:60BD
char resource_lbx_file__ovr124[] = "RESOURCE";
// WZD dseg:60C6
char compix_lbx_file__ovr124[] = "compix";
// WZD dseg:60CD
char str_hotkey_N__ovr124 = 'N';
// WZD dseg:60CF
char str_hotkey_R__ovr124 = 'R';
// WZD dseg:60D1
char str_hotkey_ESC__ovr124 = '\x1B';

// WZD dseg:60D3 00                                              align 2

// WZD dseg:60D3                                                 END:  ovr124 - Initialized Data



// WZD dseg:706E                                                 BEGIN:  ovr153 - Initialized Data

// WZD dseg:706E
/*
; zeroed for deserts and mountains, unset otherwise,
; then set to 1 if on Myrror
*/
int16_t CMB_StoneWallType = 0;
// WZD dseg:7070
int16_t G_CMB_MWallAnimSkip = 0;
// WZD dseg:7070                                                                                         ; 0 or 1, controls whether something else is stepped
// WZD dseg:7072 01                                              db    1
// WZD dseg:7073 00                                              db    0
// WZD dseg:7074
int16_t G_CMB_MWallAnimStage = 0;
// WZD dseg:7074                                                                                         ; steps 0 to 4 if the above is 0
// WZD dseg:7076
int16_t CMB_RoadAnimStage = 0;
// WZD dseg:7076                                                                                         ; steps 0 to 4
// WZD dseg:7078
int16_t CMB_WaterAnimStage = 0;
// WZD dseg:707A
int16_t CMB_MudAnimStage = 0;
// WZD dseg:707C 00 00                                           CMB_ChasmAnimYMod dw 0                  ; originally unused, reused for alternate summon anim
// WZD dseg:707E
int16_t CMB_CNodeAnimStage = 0;
// WZD dseg:707E                                                                                         ; steps 0 to 7
// WZD dseg:7080
int16_t frame_anim_cycle = 0;
// WZD dseg:7082
int16_t RP_CMB_Movement_Var;
// WZD dseg:7082                                                                                         ; cleared before movement, never used for anything
// WZD dseg:7084 00 00                                           RP_CMB_ProjectileFrame2 dw 0            ; DATA XREF: CMB_RangedAnim+3ADw ...
// WZD dseg:7086 00 00                                           
int16_t CMB_ProjectileFrame = 0;
// WZD dseg:7086                                                                                         ; used with entity drawing type 3, steps 0 to 2
// WZD dseg:7088
int16_t CMB_VortexAnimStage = 0;
// WZD dseg:708A
int16_t CMB_ChasmAnimStage = 0;
// WZD dseg:708C 00 00                                           IMG_CMB_OrigChasm@ dw 0                 ; originally unused, reused for Demon/Fire summoning
// WZD dseg:708E
int16_t CMB_Chasm_Anim = 0;
// WZD dseg:7090
int16_t CMB_CurseAnimStage = 0;
// WZD dseg:7092
int16_t CMB_MoveAnimDir = 0;
// WZD dseg:7092                                                                                         ; controls the stepping direction of GUI_MoveAnimFrame
// WZD dseg:7094
int16_t CMB_MoveAnimFrame = 0;
// WZD dseg:7094                                                                                         ; two-way, steps 0 to 2 then back
// WZD dseg:7096
int16_t CMB_BaseAnimFrame = 0;
// WZD dseg:7096                                                                                         ; steps 0 to 2
// WZD dseg:7098
int16_t CMB_UnitATKAnimFrame = 1;
// WZD dseg:7098                                                                                         ; alternating between 1 and 3
// WZD dseg:709A
int16_t CMB_UnitDEFAnimFrame = 3;
// WZD dseg:709A                                                                                         ; alternating between 1 and 3

// WZD dseg:709C
// drake178: RP_CMB_NoCombatMap
/*

Eh?
if not false, CMB_Terrain_Init__WIP() doesn't call CMB_GenerateMap().

*/
int16_t RP_CMB_NoCombatMap = ST_FALSE;

// WZD dseg:709E
char cnst_CMBEntity_Error[] = "E1";

// WZD dseg:709E                                                 END:  ovr153 - Initialized Data



// WZD dseg:70A0                                                 BEGIN:  ovr154 - Initialized Data

// WZD dseg:70A0
// WZD dseg:70AA
// ; element 0 will not be used (Rnd min return is 1)
int16_t MoveDir_NextX_0[5] = { 0,  0, -1,  0,  1};
int16_t MoveDir_NextY_0[5] = { 0,  1,  0, -1,  0};

// WZD dseg:70B4 00                                              db    0
// WZD dseg:70B5 00                                              db    0
// WZD dseg:70B6
char str_COMBAT_TMP__ovr154[] = "COMBAT.TMP";
// WZD dseg:70B6                                                                                         ; DATA XREF: Combat_Cache_Read+29o ...
// WZD dseg:70C1
char str_RB__ovr154[] = "rb";
// WZD dseg:70C4
char str_WB__ovr154[] = "wb";
// WZD dseg:70C7 00                                              align 2

// WZD dseg:70C7                                                 END:  ovr154 - Initialized Data



// WZD dseg:70C8                                                 BEGIN:  ovr155 - Initialized Data

// WZD dseg:70C8 EC FF 14 00 16 00 EA FF EB FF 15 00 FF FF 01 00 CMB_AdjacentOffsets dw   -20,   20,   22,  -22,  -21,   21,   -1,    1
// WZD dseg:70D8 EC FF 18 FC 16 00 18 FC EB FF 15 00 18 FC 01 00 CMB_AdjctOfs_NoWest dw   -20,-1000,   22,-1000,  -21,   21,-1000,    1
// WZD dseg:70E8 18 FC 14 00 18 FC EA FF EB FF 15 00 FF FF 18 FC CMB_AdjctOfs_NoEast dw -1000,   20,-1000,  -22,  -21,   21,   -1,-1000
//                                    NE     SW     SE     NW  North  South   West   East
int16_t CMB_AdjacentOffsets[8] = {   -20,    20,    22,   -22,   -(COMBAT_GRID_WIDTH),    (COMBAT_GRID_WIDTH),    -1,     1 };
int16_t CMB_AdjctOfs_NoWest[8] = {   -20, -1000,    22, -1000,   -(COMBAT_GRID_WIDTH),    (COMBAT_GRID_WIDTH), -1000,     1 };
int16_t CMB_AdjctOfs_NoEast[8] = { -1000,    20, -1000,   -22,   -(COMBAT_GRID_WIDTH),    (COMBAT_GRID_WIDTH),    -1, -1000 };
int16_t adjacent_offsets[24] =
{
      -20,    20,    22,   -22,   -(COMBAT_GRID_WIDTH),    (COMBAT_GRID_WIDTH),    -1,     1,
      -20, -1000,    22, -1000,   -(COMBAT_GRID_WIDTH),    (COMBAT_GRID_WIDTH), -1000,     1,
    -1000,    20, -1000,   -22,   -(COMBAT_GRID_WIDTH),    (COMBAT_GRID_WIDTH),    -1, -1000
};



// WZD dseg:7116                                                 BEGIN:  ovr158
// WZD dseg:7116
int16_t ai_human_hostility = ST_FALSE;
// WZD dseg:7116                                                 END:  ovr158

// WZD dseg:7118 B9 2F 2F 2F 2F                                  COL_HLP_Titles db 0B9h, 4 dup(2Fh)      ; DATA XREF: Draw_Help_Entry:loc_F27EBo
// WZD dseg:7118                                                                                         ; this should ideally have been 16 bytes long
// WZD dseg:711D B8 37 37 37 37                                  COL_HLP_Text db 0B8h, 4 dup(37h)        ; DATA XREF: Draw_Help_Entry:loc_F25E4o ...
// WZD dseg:711D                                                                                         ; this should ideally have been 16 bytes long
// WZD dseg:7122 48 45 4C 50 00                                  cnst_HELP_File3 db 'HELP',0             ; DATA XREF: Draw_Help_Entry+E9o
// WZD dseg:7122                                                                                         ; should use dseg:2d08 (or dseg:6257)
// WZD dseg:7127 68 65 6C 70 00                                  cnst_HELP_File4 db 'help',0             ; DATA XREF: Draw_Help_Entry+211o ...
// WZD dseg:7127                                                                                         ; should use dseg:2d08 (or dseg:6257)



// WZD dseg:7188                                                 BEGIN:  ovr163 - Initialized Data

// WZD dseg:7188
char cmbgrass_lbx_file__ovr163[] = "CMBGRASS";
// WZD dseg:718F
char str_figure_plural_s__ovr163[] = "S";
// WZD dseg:7190
char str_empty_string__ovr163[] = "";
// WZD dseg:7191
char cmbgrasc_lbx_file__ovr163[] = "CMBGRASC";
// WZD dseg:719A
char cmbdesrt_lbx_file__ovr163[] = "CMBDESRT";
// WZD dseg:71A3
char cmbdesrc_lbx_file__ovr163[] = "CMBDESRC";
// WZD dseg:71AC
char cmbmount_lbx_file__ovr163[] = "CMBMOUNT";
// WZD dseg:71B5
char cmbmounc_lbx_file__ovr163[] = "CMBMOUNC";
// WZD dseg:71BE
char cmbtundr_lbx_file__ovr163[] = "CMBTUNDR";
// WZD dseg:71C7
char cmbtundc_lbx_file__ovr163[] = "CMBTUNDC";
// WZD dseg:71D0 57 41 4C 4C 52 49 53 45 00                      cnst_WALLRISE_File db 'WALLRISE',0      ; DATA XREF: CMB_LoadWallRiseGFX+5Do ...
// WZD dseg:71D9
char figure_lbx_file__ovr163[] = "FIGURE";

// WZD dseg:71D9                                                 END:  ovr163 - Initialized Data



// WZD dseg:C406                                                 ¿ BEGIN:  ?

// WZD dseg:C406
int16_t CMB_ImmobileCanAct;

// WZD dseg:C408
int16_t CMB_WizardCitySiege;

// WZD dseg:C40A
char CMB_CityName[14];

// WZD dseg:C418
int16_t CMB_CityDamage;

// WZD dseg:C41A
SAMB_ptr SND_CMB_Silence;
uint32_t SND_CMB_Silence_size;  // DNE in Dasm

// WZD dseg:C41C
SAMB_ptr SND_CMB_Music;
uint32_t SND_CMB_Music_size;  // DNE in Dasm

// WZD dseg:C41E
int16_t CMB_DEFR_TrueSight;
// WZD dseg:C420
int16_t CMB_ATKR_TrueSight;

// WZD dseg:C422
SAMB_ptr _cmbt_cancel_button_seg;

// WZD dseg:C424
SAMB_ptr _cmbt_lock_info_button_seg;
// WZD dseg:C426
SAMB_ptr _cmbt_lock_flee_button_seg;
// WZD dseg:C428
SAMB_ptr _cmbt_lock_auto_button_seg;
// WZD dseg:C42A
SAMB_ptr _cmbt_lock_done_button_seg;
// WZD dseg:C42C
SAMB_ptr _cmbt_lock_wait_button_seg;
// WZD dseg:C42E
SAMB_ptr _cmbt_lock_spell_button_seg;

// WZD dseg:C430
int16_t CMB_AIGoesFirst;

// WZD dseg:C432
int16_t _auto_combat_flag;

/*
    CP, NPC, or MONSTER
    (either attacker or defender)

vs. CMB_AI_Player

*/
// WZD dseg:C434
int16_t combat_computer_player;

/*
    human/current player
    (either attacker or defender)
*/
// WZD dseg:C436
int16_t combat_human_player;

// WZD dseg:C438
/*
{F,T} cast spell enabled
Combat_Cast_Spell_Error(1);  // "You may only cast once per turn."
*/
int16_t CMB_WizCastAvailable;

// WZD dseg:C43A
int16_t CMB_DEFR_First_CE;
// WZD dseg:C43C
int16_t CMB_ATKR_First_CE;

// WZD dseg:C43E
/*

vs. combat_computer_player
*/
int16_t CMB_AI_Player;

// WZD dseg:C43E                                                                                         ; when populating the BU table, set to the index of the
// WZD dseg:C43E                                                                                         ; AI player (the other participant being always human)
// WZD dseg:C440
int16_t defender_enchantment_fields[NUM_COMBAT_ENCHANTMENTS];
// WZD dseg:C45E
int16_t attacker_enchantment_fields[NUM_COMBAT_ENCHANTMENTS];


// WZD dseg:C47C
// WZD dseg:C47E
// WZD dseg:C480
// drake178: OVL_Action_Plane
// drake178: OVL_Action_YPos
// drake178: OVL_Action_XPos
/*

*/
int16_t _combat_wp;
int16_t _combat_wy;
int16_t _combat_wx;

// WZD dseg:C482
int16_t attacker_enchantment_count;
// WZD dseg:C484
int16_t defender_enchantment_count;
// WZD dseg:C486
struct  s_COMBAT_ENCHANTMENT_ICON CMB_DEFR_CE_Wnd[NUM_COMBAT_ENCHANTMENTS];
// WZD dseg:C4C2
struct  s_COMBAT_ENCHANTMENT_ICON CMB_ATKR_CE_Wnd[NUM_COMBAT_ENCHANTMENTS];
// WZD dseg:C4FE
SAMB_ptr combat_enchantment_icon_segs[NUM_COMBAT_ENCHANTMENTS];
// SAMB_ptr IMG_CMB_TrueLight;
// SAMB_ptr IMG_CMB_Darkness;
// SAMB_ptr IMG_CMB_WarpReality;
// SAMB_ptr IMG_CMB_BlackPrayer;
// SAMB_ptr IMG_CMB_Wrack;
// SAMB_ptr IMG_CMB_MetalFires;
// SAMB_ptr IMG_CMB_Prayer;
// SAMB_ptr IMG_CMB_HighPrayer;
// SAMB_ptr IMG_CMB_Terror;
// SAMB_ptr IMG_CMB_CallLightng;
// SAMB_ptr IMG_CMB_CounterMgc;
// SAMB_ptr IMG_CMB_MassInvis;
// SAMB_ptr IMG_CMB_Entangle;
// SAMB_ptr IMG_CMB_ManaLeak;
// SAMB_ptr IMG_CMB_Blur;

// WZD dseg:C51C
int16_t _combat_turn;
// WZD dseg:C51E
int16_t CMB_HumanUnitsDone;
// WZD dseg:C520
int16_t CMB_combat_structure;
// WZD dseg:C520                                                                                         ; a seemingly pointless variable whose state could have
// WZD dseg:C520                                                                                         ; been extrapolated from others
// WZD dseg:C520                                                                                         ;   1 - city battle (defender only for wall spells?)
// WZD dseg:C520                                                                                         ;   2 - sorcery node
// WZD dseg:C520                                                                                         ;   3 - nature node
// WZD dseg:C520                                                                                         ;   4 - chaos node
// WZD dseg:C520                                                                                         ;   6 - sailable tile

// WZD dseg:C522
// MoO2  Module: MOX  _combat_mouse_grid
struct s_mouse_list * _combat_mouse_grid;
// struct s_mouse_list _combat_mouse_grid[1] = {
//     {crsr_Finger, 0, SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX}
// };

// WZD dseg:C524
// drake178: CMB_TargetRows
// MoO2  Module: MOX  _combat_mouse_grid
/*
; array of 22 pointers to LBX_NearAlloc_Next
; allocations of 21 bytes each

indexed by sizeof(2)
uses Near_Allocate_Next(), so points to 1-byte values
mov     bx, [bp+YPos]
shl     bx, 1
mov     bx, [CMB_TargetRows@+bx]
add     bx, _SI_XPos
mov     al, [bx]
cbw
uses AL and CBW, so 1-byte, signed values

allocated in CMB_SetNearAllocs__WIP()
*/
/*
e.g., CMB_TargetRows[cgy][cgx]
1-byte, signed

'Right-Click Combat Grid' uses it to get battle_unit_idx

populated in Assign_Combat_Grids()
{-2, -1, battle_unit_idx, 99}
*/
int8_t * CMB_TargetRows[22];

// WZD dseg:C550
/*
array of pointers to composed bitmaps of combat battle unit figure pictures

CMB_DrawEntities__WIP() |-> Draw_Picture_Windowed()

*/
SAMB_ptr battle_unit_picts_seg[(2 * MAX_STACK)];

// WZD dseg:C574 00                                              db    0
// WZD dseg:C575 00                                              db    0

// WZD dseg:C576
SAMB_ptr _cmbt_info_button_seg;
// WZD dseg:C578
SAMB_ptr _cmbt_flee_button_seg;
// WZD dseg:C57A
SAMB_ptr _cmbt_auto_button_seg;
// WZD dseg:C57C
SAMB_ptr _cmbt_done_button_seg;
// WZD dseg:C57E
SAMB_ptr _cmbt_wait_button_seg;
// WZD dseg:C580
SAMB_ptr _cmbt_spell_button_seg;

// WZD dseg:C582
// drake178: _active_battle_unit
// MoO2  Module: MOX  _cur_ship
/*
; active battle unit in combat?
;
; 666 - rampage, no ruins
; 667 - raiders won (city neutral)
; 668 - rampage, created ruins
*/
/*
    Sup?
    ¿ vs. _unit ?

    ¿ unit_idx or battle_unit_idx ?

currently selected battle unit
as shown in the active unit window
*/
int16_t _active_battle_unit;

// WZD dseg:C584
int16_t _combat_defender_player;
// WZD dseg:C586
int16_t _combat_attacker_player;
// WZD dseg:C588
int16_t _combat_total_unit_count;

// WZD dseg:C58A
/*
    scanned battle unit, as shown in the combat unit display
    set in Assign_Mouse_Image(), from CMB_TargetRows[grid_y][grid_x]

    currently active battle unit index
    (active as in selected, not scanned)

Page 92  (PDF Page 97)
"(a red outline surrounds the active unit’s square on the combat grid)"

*/
int16_t _scanned_battle_unit;

// WZD dseg:C58C
SAMB_ptr _combat_info_effect_icon_segs[14];

// WZD dseg:C5A8
// drake178: struct ATK_ICONS IMG_CMB_ATK_Icons;
SAMB_ptr combat_weapon_icon_segs[9];

// WZD dseg:C5BA                                                 BEGIN:  ovr093 - Uninitialized Data



// WZD dseg:C79A                                                 ¿ ovr098 ?

// WZD dseg:C79A
int16_t CMB_HumanTurn;
// WZD dseg:C79C
int16_t CRP_CMB_NeverChecked1;

// WZD dseg:C79C                                                 ¿ ovr098 ?



// WZD dseg:C7AA                                                 ¿ BEGIN:  ovr103 ?

// WZD dseg:C7AA
// drake178: struct CMOVE_ICONS IMG_CMB_MoveIcons;
SAMB_ptr combat_movemode_icon_segs[3];

// WZD dseg:C7B0
SAMB_ptr unit_hit_bar_seg;

// WZD dseg:C7B2
SAMB_ptr IMG_CMB_FX_Figure;

// WZD dseg:C7B4
int16_t CMB_HeavenlyLight;
// WZD dseg:C7B5
int16_t CMB_CloudofShadow;
// WZD dseg:C7B6
int16_t CMB_CentralStructure;

// WZD dseg:C7B7 00                                              db    0

// WZD dseg:C7B8
SAMB_ptr _combat_info_wnd_box_seg;

// WZD dseg:C7BA
SAMB_ptr _combat_info_wnd_bot_seg;
// WZD dseg:C7BC
SAMB_ptr _combat_info_wnd_mid_seg;
// WZD dseg:C7BE
SAMB_ptr _combat_info_wnd_top_seg;

// WZD dseg:C7C0
int16_t CMB_PerSideInfo;

// WZD dseg:C7C2
// ; contains a Node_Type flag or -1 (unit bonus aura)
int16_t _combat_node_type;

// WZD dseg:C7C4
int16_t _combat_info_item_count;

// WZD dseg:C7C6
int16_t _combat_total_battle_effect_count;
// WZD dseg:C7C8
struct s_CMB_InfoItem * _combat_info_effects[14];


// WZD dseg:C7E4
int16_t _combat_info_wnd_start_y;
// WZD dseg:C7E6
int16_t _combat_info_wnd_start_x;

// WZD dseg:C7E6                                                 ¿ END:  ovr103 ?



// WZD dseg:C896                                                 ¿ BEGIN:  ovr112 ?

/*
NIU_Who_Has_More_Leadership()  ...in ovr098
CMB_LoadResources__WIP()
Strategic_Combat_Allocate()
BU_Apply_Battlefield_Effects__WIP()
Init_Battlefield_Effects()

CMB_PrepareTurn__WIP()
CMB_LoadResources__WIP()
Strategic_Combat_Allocate()
BU_Apply_Battlefield_Effects__WIP()
Init_Battlefield_Effects()

CMB_LoadResources__WIP()
Strategic_Combat_Allocate()
BU_Apply_Battlefield_Effects__WIP()
Init_Battlefield_Effects()
UNIT_ConvSpellATK()
UNIT_ConvSpellATK()

*/
// WZD dseg:C896
int16_t * CMB_LeadershipArray;
// WZD dseg:C89A
int16_t * CMB_ResistAllArray;
// WZD dseg:C89E
int16_t * CMB_HolyBonusArray;
// WZD dseg:C8A2
int16_t * CMB_IDK_4PR;
// WZD dseg:C8A6 00 00                                           
int16_t CMB_TargetingType;
// WZD dseg:C8A8 00 00                                           CMB_NearDispel_UCs@ dw 0                ; DATA XREF: TILE_DispelMagic+27w ...
// WZD dseg:C8AA 00 00                                           CMB_NearDispel_UEs@ dw 0                ; DATA XREF: TILE_DispelMagic+1Aw ...
// WZD dseg:C8AC 00 00                                           
uint16_t _combat_caster_idx;

// WZD dseg:C8AC                                                 ¿ END:  ovr112 ?







// WZD dseg:C8AE                                                 ¿ BEGIN:  ovr114 ?

// WZD dseg:C8AE
int16_t AI_NoMeleeHeroSafety;
// WZD dseg:C8B0
int16_t AI_ImmobileCounter;
// WZD dseg:C8B2
int16_t G_AI_StayInTownProper;
// WZD dseg:C8B2                                                                                         ; now 1 (during tactical combat init)
// WZD dseg:C8B2                                                                                         ; set to 0 if the enemy has a high-powered ranged unit,
// WZD dseg:C8B2                                                                                         ; Magic Vortex, Wrack, Call Lightning, or Mana Leak

// WZD dseg:C8B4
// drake178: AI_CmbtWall_BitField
/*
battlefield city wall bit-field

 0 0 0 0 0 0 0 0
           | | \- stone
           | \--- fire
           \----- darkness

Dear drake178, Why "AI"?

*/
int16_t _battlefield_city_walls;

// WZD dseg:C8B4                                                 ¿ END:  ovr114 ?





// WZD dseg:C972                                                 ¿ BEGIN: ovr123 - Uninitialized Data ?

// WZD dseg:C972
int16_t CMB_Winner;
// WZD dseg:C974
int16_t GAME_RazeCity;
// WZD dseg:C976
int16_t CMB_Gold_Reward;
// WZD dseg:C978
int16_t CMB_Buildings_Lost;
// WZD dseg:C97A
int16_t * CMB_LostBuildings;
// WZD dseg:C97C
int16_t CMB_Population_Lost;
// WZD dseg:C97E
int16_t CMB_ScrollMsg_Type;

// WZD dseg:C97E                                                 ¿ END: ovr123 - Uninitialized Data ?    



// WZD dseg:CF9A                                                 ovr153
// WZD dseg:CF9A
SAMB_ptr EmmHndl_TILEXXX;
// WZD dseg:CF9A                                                                                         ; 3 reserved pages
// WZD dseg:CF9C                                                 ovr153
// WZD dseg:CF9C
SAMB_ptr EmmHndl_FIGUREX;
// WZD dseg:CF9C                                                                                         ; 1Ch reserved pages
// WZD dseg:CF9E 00                                              db    0
// WZD dseg:CF9F 00                                              db    0

// WZD dseg:CFA0                                                 IDK ~ Combat
// WZD dseg:CFA0
// WZD dseg:CFA0
// WZD dseg:CFA0                                                 ovr153, ovr154

// WZD dseg:CFA0
/*
{F,T} map square has enchanted road
*/
int16_t CMB_Enchanted_Roads;

// WZD dseg:CFA2                                                 ovr153, ovr163
// WZD dseg:CFA2 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+IMG_CMB_WallRise@ dw 0Eh dup(0)         ; DATA XREF: CMB_SpawnStoneWall+69r ...
// WZD dseg:CFA2 00 00 00 00 00 00 00 00 00 00 00 00                                                     ; array of 12 or 14 appended animations in the sandbox
// WZD dseg:CFBE                                                 ovr052, ovr153
// WZD dseg:CFBE
SAMB_ptr IMG_CMB_Blood[5];
// WZD dseg:CFBE                                                                                         ; array of 5 appended reserved EMM headers in
// WZD dseg:CFBE                                                                                         ; GFX_Swap_Seg, with a 3 frame animation each
// WZD dseg:CFC8 00                                              db    0
// WZD dseg:CFC9 00                                              db    0
// WZD dseg:CFCA
SAMB_ptr IMG_CMB_Curses[8];
// WZD dseg:CFCA                                                                                         ; array of 8 appended reserved EMM headers in
// WZD dseg:CFCA                                                                                         ; GFX_Swap_Seg, with the first 7 containing 8 frame
// WZD dseg:CFCA                                                                                         ; animations, while the last one is single image (web)

// WZD dseg:CFDA
SAMB_ptr IMG_CMB_Rocks[5];

// WZD dseg:CFE4
SAMB_ptr IMG_CMB_Trees[5];

// WZD dseg:CFEE
SAMB_ptr IMG_CMB_RiverTile[12];
// WZD dseg:CFEE 00 00 00 00 00 00 00 00                                                                 ; array of 12 appended reserved EMM headers in
// WZD dseg:CFEE                                                                                         ; GFX_Swap_Seg; the first 6 are empty, the other 6
// WZD dseg:CFEE                                                                                         ; have 5 frame animations for rivers
// WZD dseg:D006
SAMB_ptr IMG_CMB_RivrNULLs[12];
// WZD dseg:D006 00 00 00 00 00 00 00 00                                                                 ; array of 12 appended reserved EMM headers in
// WZD dseg:D006                                                                                         ; GFX_Swap_Seg with no actual data (entry size 0)
// WZD dseg:D01E
SAMB_ptr IMG_CMB_RoadTiles[28];
// WZD dseg:D01E 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; array of 28 appended reserved EMM headers in
// WZD dseg:D01E 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; GFX_Swap_Seg with 5 frame animations each, although
// WZD dseg:D01E 00 00 00 00 00 00 00 00                                                                 ; the normal ones (first in array) are all static
// WZD dseg:D056
SAMB_ptr IMG_CMB_OceanTile[4];
// WZD dseg:D056                                                                                         ; array of 4 appended reserved EMM headers in
// WZD dseg:D056                                                                                         ; GFX_Swap_Seg, each with a 5 frame animation
// WZD dseg:D05E
SAMB_ptr IMG_CMB_ChaosOcn[4];
// WZD dseg:D05E                                                                                         ; array of 4 appended reserved EMM headers in
// WZD dseg:D05E                                                                                         ; GFX_Swap_Seg, each with a 5 frame animation
// WZD dseg:D066
SAMB_ptr IMG_CMB_Cloud;
// WZD dseg:D066                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:D068 00 00 00 00 00 00                               UU_IMG_CMB_Cloud_Array dw 3 dup(0)      ; unused, original single-tile cloud image space

// WZD dseg:D06E
SAMB_ptr IMG_CMB_FlotIsle;
// WZD dseg:D06E                                                                                         ; this image is actually empty
// WZD dseg:D070
SAMB_ptr IMG_CMB_Mud;
// WZD dseg:D070                                                                                         ; 5 frame (earth to) mud animation

// WZD dseg:D072
SAMB_ptr IMG_CMB_SorcNode;
// WZD dseg:D074
SAMB_ptr IMG_CMB_NatNode;

// WZD dseg:D076
SAMB_ptr IMG_CMB_Volcano[9];


// WZD dseg:D088
SAMB_ptr IMG_CMB_DarkWall[14];
// WZD dseg:D088 00 00 00 00 00 00 00 00 00 00 00 00                                                     ; array of 14 appended reserved EMM headers in
// WZD dseg:D088                                                                                         ; GFX_Swap_Seg, each with a 4 frame animation
// WZD dseg:D0A4
SAMB_ptr IMG_CMB_FireWall[14];
// WZD dseg:D0A4 00 00 00 00 00 00 00 00 00 00 00 00                                                     ; array of 14 appended reserved EMM headers in
// WZD dseg:D0A4                                                                                         ; GFX_Swap_Seg, each with a 4 frame animation
// WZD dseg:D0C0
SAMB_ptr IMG_CMB_StoneWalls[3][12];
// WZD dseg:D0C0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; array of 36 appended reserved EMM headers in
// WZD dseg:D0C0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; GFX_Swap_Seg, 3 runs of 12 wall section images
// WZD dseg:D0C0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; with 2 frames each (intact - destroyed)
// WZD dseg:D108
SAMB_ptr IMG_CMB_DirtTile;
// WZD dseg:D10A
SAMB_ptr IMG_CMB_RoadGrid;
// WZD dseg:D10C
SAMB_ptr IMG_CMB_Fort;
// WZD dseg:D10E
SAMB_ptr IMG_CMB_Ruins;
// WZD dseg:D110
SAMB_ptr IMG_CMB_Temple;
// WZD dseg:D112
SAMB_ptr IMG_CMB_Dungeon;
// WZD dseg:D114
SAMB_ptr IMG_CMB_Tower;
// WZD dseg:D116
SAMB_ptr IMG_CMB_Cave;
// WZD dseg:D118
SAMB_ptr IMG_CMB_Outpost;
// WZD dseg:D11A
SAMB_ptr IMG_CMB_Fortress;

// WZD dseg:D11C
// ; array of 15 appended reserved EMM headers in
// ; GFX_Swap_Seg, with 1 image each (3 groups of 5)
SAMB_ptr IMG_CMB_Houses[15];

// WZD dseg:D13A
SAMB_ptr IMG_GUI_Chasm;
// WZD dseg:D13A                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:D13A                                                                                         ; 8 frame animation
// WZD dseg:D13C                                                 ovr099, ovr153
// WZD dseg:D13C
SAMB_ptr IMG_GUI_Vortex;
// WZD dseg:D13C                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:D13C                                                                                         ; 6 frame animation

// WZD dseg:D13E
SAMB_ptr CMB_Path_Ys;
// WZD dseg:D140
SAMB_ptr CMB_Path_Xs;
// WZD dseg:D142
int16_t movement_path_grid_cell_count;

// WZD dseg:D144
/*
2-byte, signed
allocated in CMB_SetNearAllocs__WIP()
CMB_NearBuffer_3 = Near_Allocate_Next(1008);
504 2-byte values
*/
int16_t * CMB_NearBuffer_3;

// WZD dseg:D146
/*
1-byte, unsigned
*/
uint8_t * CMB_Path_Costs;

// WZD dseg:D148
/*
1-byte
used as movement cost map

-1: ¿ occupied or impassible ?
¿ otherwise, the movement cost of the combat grid cell ?
...which gets memcpy()'d over, in in Set_Movement_Cost_Map()

movement cost is set in 

*/
int8_t * CMB_ActiveMoveMap;

// WZD dseg:D14A
// WZD dseg:D14C
/*
cgx,cgy

set in BU_CombatSummon__SEGRAX()
    ...where used?
*/
int16_t CMB_Chasm_Anim_Y;
int16_t CMB_Chasm_Anim_X;

// WZD dseg:D14E
struct s_MAGIC_VORTEX * CMB_Vortex_Array;
// WZD dseg:D152
int16_t CMB_Vortex_Count;

// WZD dseg:D154
SAMB_ptr CMB_Projectiles;
// WZD dseg:D154                                                                                         ; (up to 11 records of 14 bytes each)
// WZD dseg:D158 00 00                                           CMB_ProjectileCount dw 0                ; DATA XREF: CMB_RangedAnim+464r ...

// WZD dseg:D15A
/*
~ battle unit figure
¿ ++ rocks, trees ?
¿ ++ city walls ?
¿ ++ lair ?
*/
struct s_COMBAT_ENTITY * combat_grid_entities;

// WZD dseg:D15E
/*
max: 256
~ MAX(1-byte, unsigned, integer)

*/
int16_t combat_grid_entity_count;
// WZD dseg:D160
int16_t CMB_BloodFrames[20];

// WZD dseg:D188
SAMB_ptr frame_active_seg;
// WZD dseg:D18A
SAMB_ptr frame_scanned_seg;

// WZD dseg:D18C
// RNGD_ANIM * CMB_RangedAtx_GFX[15];
SAMB_ptr CMB_RangedAtx_GFX[15][8];

// WZD dseg:D18C 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; DATA XREF: Reload_Combat_Graphics_Cache+32w ...
// WZD dseg:D18C 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; appended LBX_Alloc_Space header links to reserved
// WZD dseg:D18C 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; EMM resources, 8 4-frame animations for each of the
// WZD dseg:D18C 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; 15 ranged attack graphics types (last is unused?)
// WZD dseg:D27C
SAMB_ptr CMB_Damage_GFX;
// WZD dseg:D27C                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:D27C                                                                                         ; 4 frame animation
// WZD dseg:D27E 00 00                                           CMB_WallRise_Type dw 0                  ; DATA XREF: CMB_SpawnStoneWall+36r ...
// WZD dseg:D27E                                                                                         ; 0 branches wall entity creation (unknown IMG array)

// WZD dseg:D280
// drake178: CMB_ZSort_Array
// ; from farthest to nearest combat entities
/*
array of combat_grid_entity_idx
set in Set_Entity_Draw_Order()
entt->draw_order_value = ((cgy * 8000) + (cgx * 320) + (cgy_ofst * 16) + cgx_ofst)
*/
uint8_t combat_grid_entities_draw_order[MAX_ENTITIES];

// WZD dseg:D380
int16_t DEFR_FloatingIsland;
// WZD dseg:D380                                                                                         ; unset yet (thus 0)
// WZD dseg:D382                                                 ovr098, ovr154
// WZD dseg:D382
int16_t ATKR_FloatingIsland;
// WZD dseg:D382                                                                                         ; unset yet (thus 0)
// WZD dseg:D384                                                 ovr155

// ; duplicate of GUI_ActiveMoveMap (no real purpose)
// WZD dseg:D384
int16_t * RP_CMB_MoveMap;



/*
    WIZARDS.EXE  ovr090
*/

// WZD s90p01
// drake178: CMB_TacticalCombat()
// MoO2
// 1oom
/*
; processes tactical combat from start to finish
; returns 1 if the attacker won, or 0 otherwise
;
; contains a host of BUGs, both own and inherited
*/
/*

    returns ST_TRUE, if Combat_Winner is _combat_attacker_player
    otherwise, returns ST_FALSE

*/
int16_t Tactical_Combat__WIP(int16_t combat_attacker_player_idx, int16_t combat_defender_player_idx, int16_t troops[], int16_t troop_count, int16_t wx, int16_t wy, int16_t wp, int16_t * item_count, int16_t item_list[])
{
    int16_t did_win = 0;

    int16_t Battle_Result = 0;
    int16_t Defending_Unit_Count = 0;
    int16_t Player_Fled = 0;
    int16_t Combat_Winner = 0;
    int16_t auto_combat_cancel_ESC_field = 0;
    int16_t cast_status = 0;
    int16_t MoveHalves_Save = 0;
    uint32_t Overland_Enchants = 0;
    int16_t active_unit_window_field = 0;
    int16_t SPACE_Hotkey = 0;
    int16_t info_button_field = 0;
    int16_t Flee_Button_Index = 0;
    int16_t auto_button_field = 0;
    int16_t done_button_field = 0;
    int16_t wait_button_field = 0;
    int16_t spell_button_field = 0;
    int16_t RightClick_Y = 0;
    int16_t RightClick_X = 0;
    int16_t combat_grid_field = 0;
    // int16_t Grid_Y = 0;
    // int16_t Grid_X = 0;
    // TODO  rename Grid_X & Grid_Y - they are the screen x,y of a click on the combat grid
    int64_t Grid_Y = 0;
    int64_t Grid_X = 0;
    int16_t Bottom_GUI_Escape_Hotkey = 0;
    int16_t screen_changed = 0;
    int16_t leave_screen = 0;
    int16_t input_field_idx = 0;
    int16_t itr = 0;  // _SI_
    int16_t battle_unit_idx;  // _DI_
    int16_t hotkey_idx_Z = 0;  // debug_hotkey
    int16_t hotkey_idx_T = 0;  // test_hotkey

    AI_ImmobileCounter = 0;

    CMB_WizardCitySiege = ST_FALSE;

    if(
        (OVL_Action_Type == 1)  /* Enemy City */
        &&
        (combat_defender_player_idx != NEUTRAL_PLAYER_IDX)
    )
    {
        CMB_WizardCitySiege = ST_TRUE;
    }

    CMB_AI_Fled = ST_FALSE;  // ; set to 1 if the AI decides to flee

    Player_Fled = ST_FALSE;

    Deactivate_Auto_Function();

    Clear_Fields();

    Fade_Out();

    Set_Page_Off();

    Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, ST_TRANSPARENT);

    Toggle_Pages();

    PageFlipEffect = pf_Normal;

    if(OVL_Action_Type == 1)  /* Enemy City */
    {
        String_Copy_Far(CMB_CityName, _CITIES[OVL_Action_Structure].name);
    }


    CMB_BaseAllocs__WIP();


    CMB_LoadResources__WIP();  /* calls CMB_SetNearAllocs__WIP() */


    _combat_wx = wx;
    _combat_wy = wy;
    _combat_wp = wp;

    _combat_attacker_player = combat_attacker_player_idx;

    _combat_defender_player = combat_defender_player_idx;


    Cache_Graphics_Combat();


    CMB_Terrain_Init__WIP(wx, wy, wp);  // CMB_ComposeBackgrnd__WIP() |-> Copy_Off_To_Back()


    Defending_Unit_Count = CMB_Units_Init__WIP(troop_count, troops);


    Clear_Fields();

    Deactivate_Auto_Function();

    Assign_Auto_Function(Tactical_Combat_Draw, 1);


    CMB_ATKR_First_CE = 0;

    CMB_DEFR_First_CE = 0;


    CMB_combat_structure = Combat_Structure(wx, wy, wp, 0);


    CMB_CE_Refresh__WIP();


    Combat_Node_Type();

    CMB_HumanUnitsDone = ST_FALSE;

    _combat_turn = 0;


    CMB_WizCastAvailable = ST_TRUE;


    _combat_total_battle_effect_count = Combat_Info_Effects_Count();


    Init_Battlefield_Effects(CMB_combat_structure);


    Combat_Cache_Write();

    SND_CMB_Silence = LBX_Reload(cnst_SOUNDFX_File2, SFX_Silence, World_Data);
    SND_CMB_Silence_size = lbxload_entry_length;

    // ; when populating the BU table, set to the index of the
    // ; AI player (the other participant being always human)
    if(CMB_AI_Player != NEUTRAL_PLAYER_IDX)
    {

        SND_CMB_Music = LBX_Reload_Next(cnst_MUSIC_File7, ((MUSIC_Merlin_Cmbt1 - 1) + ((_players[CMB_AI_Player].wizard_id * 2) + Random(2))), World_Data);
        SND_CMB_Music_size = lbxload_entry_length;

    }
    else
    {

        itr = (MUSIC_Combat_1 - 1) + Random(2);

        SND_CMB_Music = LBX_Reload_Next(cnst_MUSIC_File7, itr, World_Data);
        SND_CMB_Music_size = lbxload_entry_length;

    }

    if(magic_set.background_music == ST_TRUE)
    {

        // DOMSDOS  Play_Sound__WIP(SND_CMB_Music);
        sdl2_Play_Sound__WIP(SND_CMB_Music, SND_CMB_Music_size);

    }


    CMB_ImmobileCanAct = ST_FALSE;

    G_AI_StayInTownProper = ST_TRUE;

    _scanned_battle_unit = ST_UNDEFINED;


    Reset_Cycle_Palette_Color();
    Load_Palette_From_Animation(combat_background_bottom);
    Apply_Palette();
    Cycle_Palette_Color__STUB(198, 0, 0, 0, 55, 0, 0, 8);
    Set_Palette_Changes(0, 243);
    Calculate_Remap_Colors();

    
    if(_combat_attacker_player == _human_player_idx)
    {

        Switch_Active_Battle_Unit(0);  /* first attacker battle_unit_idx */

    }
    else
    {

        // ; BUG: should set the selected unit manually!
        // ; corrupts memory is the last value is invalid

        Switch_Active_Battle_Unit((_combat_total_unit_count - Defending_Unit_Count));  /* first defender battle_unit_idx */

    }

    frame_active_flag = 0;

    frame_scanned_flag = 0;

    CRP_CMB_NeverChecked1 = ST_TRUE;


    Tactical_Combat_Draw();

    PageFlip_FX();


    CMB_CityDamage = ST_FALSE;

    leave_screen = ST_FALSE;

    CMB_HumanTurn = ST_TRUE;

    _auto_combat_flag = ST_FALSE;


    CMB_PrepareTurn__WIP();


    CMB_ImmobileCanAct = ST_FALSE;

    // ; NONE of the above functions change the focus unit
    if(_combat_attacker_player == _human_player_idx)
    {

        Switch_Active_Battle_Unit(0);

    }
    else
    {

        Switch_Active_Battle_Unit((_combat_total_unit_count - Defending_Unit_Count));

    }

    frame_active_flag = 0;

    frame_scanned_flag = 0;

    CRP_CMB_NeverChecked1 = ST_TRUE;


    Tactical_Combat_Draw();

    PageFlip_FX();


    CMB_AIGoesFirst = ST_FALSE;

    if(_combat_defender_player == combat_computer_player)
    {

        AI_CMB_PlayTurn__WIP(_combat_defender_player);

        // ; BUG: the defending AI gets an extra turn?

        CMB_PrepareTurn__WIP();

        CMB_AIGoesFirst = ST_TRUE;

    }

    Combat_Winner = Check_For_Winner__WIP();

    // ; BUG: second time clearing this without using it
    if(Combat_Winner != ST_UNDEFINED)
    {
        leave_screen = ST_UNDEFINED;
    }

    CMB_HumanUnitsDone = ST_FALSE;

    if(_combat_attacker_player == _human_player_idx)
    {

        Next_Battle_Unit(0);  /* first attacker battle_unit_idx */

    }
    else
    {

        Switch_Active_Battle_Unit((_combat_total_unit_count - Defending_Unit_Count));  /* first defender battle_unit_idx */

    }


    Tactical_Combat_Draw();

    PageFlip_FX();


    Assign_Combat_Grids();


    Set_Input_Delay(3);


    CMB_CE_Refresh__WIP();


    Deactivate_Help_List();

    Set_Combat_Help_List();

    CRP_CMB_NeverChecked1 = ST_TRUE;


    while(leave_screen == ST_FALSE)
    {

        Assign_Auto_Function(Tactical_Combat_Draw, 1);

        Mark_Time();

        Clear_Fields();



        /*
            BEGIN: Auto Combat
        */
        {
            if(_auto_combat_flag == ST_TRUE)
            {
                
                auto_combat_cancel_ESC_field = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, str_hotkey_ESC__ovr090[0], ST_UNDEFINED);

                input_field_idx = Get_Input();

                if(input_field_idx == auto_combat_cancel_ESC_field)  /* turn off 'Auto Combat' */
                {

                    Play_Left_Click();

                    _auto_combat_flag = ST_FALSE;

                    CMB_AIGoesFirst = ST_FALSE;

                    CRP_CMB_NeverChecked1 = ST_TRUE;

                    CMB_ProgressTurnFlow__WIP();

                    Turn_Off_Auto_Combat();

                    CRP_CMB_NeverChecked1 = ST_TRUE;

                    CMB_ImmobileCanAct = ST_FALSE;

                }
                else  /* do 'Auto Combat' */
                {
                    CMB_ProgressTurnFlow__WIP();
                }


                Combat_Winner = Check_For_Winner__WIP();

                if(Combat_Winner != ST_UNDEFINED)
                {

                    leave_screen = ST_UNDEFINED;

                    input_field_idx = ST_UNDEFINED;

                }

                continue;

            }
        }
        /*
            END: Auto Combat
        */



        if(
            (battle_units[_active_battle_unit].movement_points > 0)
            &&
            (
                (battle_units[_active_battle_unit].mana >= 3)
                ||
                (BU_HasSpellAbility__WIP(_active_battle_unit) == ST_TRUE)
                ||
                (battle_units[_active_battle_unit].Item_Charges > 0)
                ||
                (CMB_WizCastAvailable == ST_TRUE)
            )
        )
        {

            spell_button_field = Add_Button_Field(144, 168, str_empty_string__ovr090, _cmbt_spell_button_seg, cnst_HOTKEY_S_3[0], ST_UNDEFINED);

        }
        else
        {

            spell_button_field = INVALID_FIELD;

        }


        wait_button_field = Add_Button_Field(170, 168, str_empty_string__ovr090, _cmbt_wait_button_seg, cnst_HOTKEY_W_2[0], ST_UNDEFINED);


        done_button_field = Add_Button_Field(170, 188, str_empty_string__ovr090, _cmbt_done_button_seg, cnst_HOTKEY_D_5[0], ST_UNDEFINED);


        if(_combat_total_battle_effect_count > 0)
        {

            info_button_field = Add_Button_Field(144, 178, str_empty_string__ovr090, _cmbt_info_button_seg, cnst_HOTKEY_U_5[0], ST_UNDEFINED);

        }
        else
        {

            info_button_field = INVALID_FIELD;

        }


        auto_button_field = Add_Button_Field(170, 178, str_empty_string__ovr090, _cmbt_auto_button_seg, cnst_HOTKEY_A_4[0], ST_UNDEFINED);


        Flee_Button_Index = Add_Button_Field(144, 188, str_empty_string__ovr090, _cmbt_flee_button_seg, cnst_HOTKEY_F[0], ST_UNDEFINED);


        // ¿ Right-Click brings up 'Unit View' ?
        active_unit_window_field = Add_Hidden_Field(83, 173, 116, 198, str_empty_string__ovr090[0], ST_UNDEFINED);


        Add_Combat_Enchantment_Fields();


        Bottom_GUI_Escape_Hotkey = Add_Hidden_Field(0, 164, 319, 199, str_hotkey_ESC__ovr090[0], ST_UNDEFINED);


        combat_grid_field = Add_Grid_Field(0, 0, 1, 1, 319, 164, &Grid_X, &Grid_Y, ST_UNDEFINED);


        SPACE_Hotkey = Add_Hot_Key(cnst_HOTKEY_SPACE_4[0]);


        hotkey_idx_Z = Add_Hot_Key('Z');  // debug_hotkey  ...  Derp. 'D' is already used for the "Done" button.
        hotkey_idx_T = Add_Hot_Key('T');  // test_hotkey


        input_field_idx = Get_Input();


        /*
            BEGIN:  Left-Click Auto Button
        */
        if(input_field_idx == auto_button_field)
        {

            Play_Left_Click();

            Set_Mouse_List(1, mouse_list_default);

            frame_active_flag = 0;

            frame_scanned_flag = 0;

            _auto_combat_flag = (1 - _auto_combat_flag);

            CMB_HumanUnitsDone = 0;

            Clear_Fields();

            input_field_idx = 0;

            AI_CMB_PlayTurn__WIP(combat_human_player);

            Combat_Winner = Check_For_Winner__WIP();

            if(Combat_Winner == ST_UNDEFINED)
            {
                leave_screen = ST_UNDEFINED;
                input_field_idx = 0;
            }

        }
        /*
            END:  Left-Click Auto Button
        */


        /*
            BEGIN:  ¿ what is going on here ?

                seems like this would be where the battle unit that just moved would have its turn ended
                does bua_Finished get set anywhere else?
                what's status > bus_Active?  ...bus_Recalled, bus_Fleeing, bus_Uninvolved, bus_Dead, bus_Drained, bus_Gone
        */
        if(
            (
                (battle_units[_active_battle_unit].movement_points < 1)
                &&
                (battle_units[_active_battle_unit].action != bua_Finished)
            )
            ||
            (battle_units[_active_battle_unit].status > bus_Active)
        )
        {

            battle_units[_active_battle_unit].Moving = ST_FALSE;

            battle_units[_active_battle_unit].movement_points = 0;

            battle_units[_active_battle_unit].action = bua_Finished;

            if(
                ((battle_units[_active_battle_unit].Combat_Effects & bue_Confusion) != 0)
                &&
                (battle_units[_active_battle_unit].Confusion_State == 2)
            )
            {

                if(battle_units[_active_battle_unit].controller_idx == _combat_attacker_player)
                {

                    battle_units[_active_battle_unit].controller_idx = _combat_defender_player;

                }
                else
                {

                    battle_units[_active_battle_unit].controller_idx = _combat_attacker_player;

                }

            }

            Next_Battle_Unit(_human_player_idx);

            Assign_Combat_Grids();

            input_field_idx = ST_UNDEFINED;

            screen_changed = ST_TRUE;

            CRP_CMB_NeverChecked1 = ST_TRUE;

            Combat_Winner = Check_For_Winner__WIP();

            if(Combat_Winner != ST_UNDEFINED)
            {
                leave_screen = ST_UNDEFINED;
                input_field_idx = 0;
            }

        }
        /*
            END:  ¿ what is going on here ?
        */


        if(
            (battle_units[_active_battle_unit].Confusion_State == 1)
            &&
            (battle_units[_active_battle_unit].movement_points > 0)
        )
        {

            BU_MoveConfused__WIP(_active_battle_unit);

            Next_Battle_Unit(_human_player_idx);

            Assign_Combat_Grids();

            input_field_idx = ST_UNDEFINED;

            screen_changed = ST_TRUE;

            CRP_CMB_NeverChecked1 = ST_TRUE;

            Combat_Winner = Check_For_Winner__WIP();

            if(Combat_Winner != ST_UNDEFINED)
            {

                leave_screen = ST_UNDEFINED;

                input_field_idx = 0;

            }

        }


        for(itr = 0; itr < attacker_enchantment_count; itr++)
        {

            if(itr >= 4)
            {
                break;
            }

            if(attacker_enchantment_count <= 4)
            {
                break;
            }

            if(attacker_enchantment_fields[itr] == input_field_idx)
            {

                Play_Left_Click();

                CMB_ATKR_First_CE += 4;  // ; now 0 (after tactical BU init)
                                         // ; attacker's first shown combat enchant
                if(CMB_ATKR_First_CE > 8)
                {
                    CMB_ATKR_First_CE = 0;
                }

                CRP_CMB_NeverChecked1 = ST_TRUE;

                break;

            }

        }



        for(itr = 0; itr < defender_enchantment_count; itr++)
        {

            if(itr >= 4)
            {
                break;
            }

            if(defender_enchantment_count <= 4)
            {
                break;
            }

            if(defender_enchantment_fields[itr] == input_field_idx)
            {

                Play_Left_Click();

                CMB_DEFR_First_CE += 4;  // ; now 0 (after tactical BU init)
                                         // ; attacker's first shown combat enchant
                if(CMB_DEFR_First_CE > 8)
                {
                    CMB_DEFR_First_CE = 0;
                }

                CRP_CMB_NeverChecked1 = ST_TRUE;

                break;

            }

        }

        /*
            BEGIN:  Left-Click Flee Button
        */
        if(input_field_idx == Flee_Button_Index)
        {

            CMB_ImmobileCanAct = 0;

            Play_Left_Click();

            Set_Mouse_List(1, mouse_list_default);

            if(Confirmation_Box(cnst_FleeConfirm_Msg) == ST_TRUE)
            {

                for(itr = 0; itr < _combat_total_unit_count; itr++)
                {

                    if(battle_units[itr].controller_idx == combat_human_player)
                    {

                        battle_units[itr].action = BUA_Flee;

                        if(
                            (battle_units[itr].status == bus_Active)
                            ||
                            (battle_units[itr].status == bus_Uninvolved)
                        )
                        {
                            
                            battle_units[itr].status = bus_Fleeing;

                        }

                    }

                }

                Combat_Winner = combat_computer_player;
                Player_Fled = ST_TRUE;
                leave_screen = ST_UNDEFINED;

            }

            Assign_Combat_Grids();

            screen_changed = ST_TRUE;

        }
        /*
            END:  Left-Click Flee Button
        */


        /*
            BEGIN:  Left-Click Combat Grid
        */
        {
            // @@LeftClickCombatGrid
            if(input_field_idx == combat_grid_field)
            {

                CMB_ImmobileCanAct = ST_FALSE;

                frame_scanned_cgx = Get_Combat_Grid_Cell_X((Grid_X + 4), (Grid_Y + 4));
                frame_scanned_cgy = Get_Combat_Grid_Cell_Y((Grid_X + 4), (Grid_Y + 4));

                Battle_Unit_Action__WIP(_active_battle_unit, frame_scanned_cgx, frame_scanned_cgy);

                for(itr = 0; itr < _combat_total_unit_count; itr++)
                {

                    BU_SetVisibility__WIP(itr);

                }

                Assign_Combat_Grids();

                CRP_CMB_NeverChecked1 = ST_TRUE;

                Combat_Winner = Check_For_Winner__WIP();

                if(Combat_Winner != ST_UNDEFINED)  /* invalid / no winner / none / neither */
                {

                    leave_screen = ST_UNDEFINED;

                    input_field_idx = 0;

                }

            }
        }
        /*
            END:  Left-Click Combat Grid
        */


        /*
            BEGIN:  Right-Click Combat Grid
        */
        {
            if(-(combat_grid_field) == input_field_idx)
            {

                RightClick_X = Get_Combat_Grid_Cell_X((Grid_X + 4), (Grid_Y + 4));

                RightClick_Y = Get_Combat_Grid_Cell_Y((Grid_X + 4), (Grid_Y + 4));

                battle_unit_idx = CMB_TargetRows[RightClick_Y][RightClick_X];

                // ; BUG: this needs to be range checked, it can be 99!
                if(
                    (battle_unit_idx > 0)
                    &&
                    (battle_units[battle_unit_idx].status == bus_Active)
                )
                {

                    if(battle_units[battle_unit_idx].controller_idx != _human_player_idx)
                    {

                        Play_Left_Click();

                        Deactivate_Help_List();

                        Set_Mouse_List(1, mouse_list_default);

                        // ; byte-identical to the other branch

                        MoveHalves_Save = battle_units[battle_unit_idx].movement_points;

                        battle_units[battle_unit_idx].movement_points = Battle_Unit_Moves2(battle_unit_idx);

                        Overland_Enchants = _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments;

                        _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments = (_UNITS[battle_units[battle_unit_idx].unit_idx].enchantments | battle_units[battle_unit_idx].enchantments);

                        memcpy(global_battle_unit, &battle_units[_active_battle_unit], sizeof(struct s_BATTLE_UNIT));

                        USW_CombatDisplay__WIP(61, 6, 89, 174, 117, 194, 2, battle_units[battle_unit_idx].unit_idx);

                        battle_units[battle_unit_idx].movement_points += MoveHalves_Save;

                        _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments = Overland_Enchants;

                        CMB_SetNearAllocs__WIP();

                        Assign_Combat_Grids();

                        Assign_Auto_Function(Tactical_Combat_Draw, 1);

                        CMB_CE_Refresh__WIP();

                        CMB_ComposeBackgrnd__WIP();  // ... |-> Copy_Off_To_Back();

                        Deactivate_Help_List();

                        Set_Combat_Help_List();

                        CRP_CMB_NeverChecked1 = ST_TRUE;

                    }
                    else
                    {

                        if(_active_battle_unit != battle_unit_idx)
                        {

                            Switch_Active_Battle_Unit(battle_unit_idx);

                            Assign_Combat_Grids();

                        }
                        else
                        {

                            Play_Left_Click();

                            Deactivate_Help_List();

                            Set_Mouse_List(1, mouse_list_default);

                            MoveHalves_Save = battle_units[battle_unit_idx].movement_points;

                            battle_units[battle_unit_idx].movement_points = Battle_Unit_Moves2(battle_unit_idx);

                            Overland_Enchants = _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments;

                            _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments = (_UNITS[battle_units[battle_unit_idx].unit_idx].enchantments | battle_units[battle_unit_idx].enchantments);

                            // TODO  _fmemcpy(global_battle_unit, battle_units[battle_unit_idx], sizeof(struct s_BATTLE_UNIT));
                            memcpy(global_battle_unit, &battle_units[_active_battle_unit], sizeof(struct s_BATTLE_UNIT));

                            USW_CombatDisplay__WIP(61, 6, 89, 174, 117, 194, 2, battle_units[battle_unit_idx].unit_idx);

                            battle_units[battle_unit_idx].movement_points = MoveHalves_Save;

                            _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments = Overland_Enchants;

                            CMB_SetNearAllocs__WIP();

                            Assign_Combat_Grids();

                            Assign_Auto_Function(Tactical_Combat_Draw, 1);

                            CMB_CE_Refresh__WIP();

                            CMB_ComposeBackgrnd__WIP();  // ... |-> Copy_Off_To_Back();

                            Deactivate_Help_List();

                            Set_Combat_Help_List();

                        }

                        CRP_CMB_NeverChecked1 = ST_TRUE;

                    }

                }

            }
        }
        /*
            END:  Right-Click Combat Grid
        */


        /*
            BEGIN:  Left-Click Spell Button
                IDA Color:  Purple #32
        */
        {
            if(input_field_idx == spell_button_field)
            {

                Play_Left_Click();

                screen_changed = ST_TRUE;

                Deactivate_Help_List();

                if(
                    (battle_units[_active_battle_unit].movement_points > 0)
                    &&
                    (
                        (battle_units[_active_battle_unit].mana >= 3)  // ; BUG: units need at least 5 to cast anything
                        ||
                        (BU_HasSpellAbility__WIP(_active_battle_unit) == ST_TRUE)
                        ||
                        (battle_units[_active_battle_unit].Item_Charges > 0)
                    )
                )
                {

                    Combat_Cast_Spell_With_Caster(_active_battle_unit);

                }
                else
                {

                    if(CMB_WizCastAvailable == ST_TRUE)
                    {

                        cast_status = Combat_Cast_Spell__WIP((CASTER_IDX_BASE + _human_player_idx), _combat_wx, _combat_wy, _combat_wp);

                        switch(cast_status)
                        {
                            case 0:
                            {
                                Combat_Cast_Spell_Error(2);  // "You are unable to throw spells at this time."
                            } break;
                            case 1:  /* success */
                            {
                                // N/A
                            } break;
                            case 2:
                            {
                                CMB_WizCastAvailable = ST_FALSE;
                                CMB_ImmobileCanAct = ST_FALSE;
                            } break;
                            default:
                            {
                                // N/A
                            } break;
                        }

                    }
                    else
                    {

                        Combat_Cast_Spell_Error(1);  // "You may only cast once per turn."

                    }

                }

                screen_changed = ST_TRUE;

                Assign_Auto_Function(Tactical_Combat_Draw, 1);

                Assign_Combat_Grids();

                CMB_CE_Refresh__WIP();  // ¿ because you may just cast a 'Combat Enchantment'

                CMB_ComposeBackgrnd__WIP();  // ... |-> Copy_Off_To_Back();

                Deactivate_Help_List();

                Set_Combat_Help_List();

                CRP_CMB_NeverChecked1 = ST_TRUE;

                Combat_Winner = Check_For_Winner__WIP();  // ¿ because your spell may have just killed the last enemy unit ?

                if(Combat_Winner != ST_UNDEFINED)
                {
                    leave_screen = ST_UNDEFINED;
                    input_field_idx = 0;
                }

            }
        }
        /*
            END:  Left-Click Spell Button
        */


        /*
            BEGIN:  Left-Click Wait Button
        */
        {
            if(input_field_idx == wait_button_field)
            {

                CMB_ImmobileCanAct = 0;

                Play_Left_Click();

                battle_units[_active_battle_unit].action = bua_Wait;

                Next_Battle_Unit(_human_player_idx);

                Assign_Combat_Grids();

                CRP_CMB_NeverChecked1 = ST_TRUE;

            }
        }
        /*
            END:  Left-Click Wait Button
        */


        /*
            BEGIN:  Left-Click Info Button
        */
        {
            if(input_field_idx == info_button_field)
            {

                CMB_ImmobileCanAct = 0;

                Play_Left_Click();

                Deactivate_Help_List();

                Combat_Information_Window();

                CMB_SetNearAllocs__WIP();

                Assign_Auto_Function(Tactical_Combat_Draw, 1);

                Assign_Combat_Grids();

                CMB_CE_Refresh__WIP();

                CMB_ComposeBackgrnd__WIP();  // ... |-> Copy_Off_To_Back();

                Deactivate_Help_List();

                Set_Combat_Help_List();

                CRP_CMB_NeverChecked1 = ST_TRUE;

            }
        }
        /*
            END:  Left-Click Info Button
        */


        /*
            BEGIN:  Left-Click Done Button
        */
        {
            if(
                (input_field_idx == done_button_field)
                ||
                (input_field_idx == SPACE_Hotkey)
            )
            {

                CMB_ImmobileCanAct = ST_FALSE;

                Play_Left_Click();

                battle_units[_active_battle_unit].action = bua_Finished;

                battle_units[_active_battle_unit].movement_points = 0;

                Next_Battle_Unit(_human_player_idx);

                Assign_Combat_Grids();

                if(CMB_HumanUnitsDone == ST_TRUE)
                {
                    screen_changed = ST_TRUE;
                }

                CRP_CMB_NeverChecked1 = ST_TRUE;

                Combat_Winner = Check_For_Winner__WIP();

                if(Combat_Winner != ST_UNDEFINED)
                {
                    
                    leave_screen = ST_UNDEFINED;

                    input_field_idx = 0;

                }

            }
        }
        /*
            END:  Left-Click Done Button
        */


        /*
            BEGIN:  Right-Click Active Unit Window
        */
        if(-(active_unit_window_field) == input_field_idx)
        {

            Play_Left_Click();

            Deactivate_Help_List();

            MoveHalves_Save = battle_units[_active_battle_unit].movement_points;

            battle_units[_active_battle_unit].movement_points = Battle_Unit_Moves2(_active_battle_unit);

            Overland_Enchants = _UNITS[battle_units[_active_battle_unit].unit_idx].enchantments;

            _UNITS[battle_units[_active_battle_unit].unit_idx].enchantments = (_UNITS[battle_units[_active_battle_unit].unit_idx].enchantments | battle_units[_active_battle_unit].enchantments);

            memcpy(global_battle_unit, &battle_units[_active_battle_unit], sizeof(struct s_BATTLE_UNIT));

            USW_CombatDisplay__WIP(61, 6, 89, 174, 117, 194, 2, battle_units[_active_battle_unit].unit_idx);

            battle_units[_active_battle_unit].movement_points = MoveHalves_Save;

            _UNITS[battle_units[_active_battle_unit].unit_idx].enchantments = Overland_Enchants;

            CMB_SetNearAllocs__WIP();

            Assign_Combat_Grids();

            Assign_Auto_Function(Tactical_Combat_Draw, 1);

            CMB_ComposeBackgrnd__WIP();  // ... |-> Copy_Off_To_Back();

            CMB_CE_Refresh__WIP();

            Deactivate_Help_List();

            Set_Combat_Help_List();

            CRP_CMB_NeverChecked1 = ST_TRUE;

        }
        /*
            END:  Right-Click Unit
        */


        // ST_DEBUG Hot-Keys
        if(input_field_idx == hotkey_idx_Z)  /* Debug Hot-Key */
        {
            // TODO  DLOG("STU_DEBUG: debug_hotkey");
            DBG_debug_flag = !DBG_debug_flag;  // ~== `^= 1`
            if(DBG_debug_flag)
            {

            }

        }
        if(input_field_idx == hotkey_idx_T)  /* Test Hot-Key */
        {
            // TODO  DLOG("STU_DEBUG: test_hotkey");

        }


        if(battle_units[_active_battle_unit].controller_idx != combat_human_player)
        {

            STU_DEBUG_BREAK();
            
            CMB_HumanUnitsDone = ST_TRUE;

            CMB_ImmobileCanAct = ST_FALSE;

        }


        if(
            (leave_screen == ST_FALSE)
            &&
            (CMB_HumanUnitsDone == ST_TRUE)
            &&
            (CMB_ImmobileCanAct == ST_FALSE)
        )
        {

            CMB_HumanUnitsDone = ST_FALSE;

            CMB_ProgressTurnFlow__WIP();

            Next_Battle_Unit(_human_player_idx);

            Assign_Combat_Grids();

            input_field_idx = ST_UNDEFINED;

            screen_changed = ST_TRUE;

            CRP_CMB_NeverChecked1 = ST_TRUE;

            Combat_Winner = Check_For_Winner__WIP();

            if(Combat_Winner != ST_UNDEFINED)
            {

                leave_screen = ST_UNDEFINED;

                input_field_idx = 0;

            }

        }


        if(leave_screen == ST_FALSE)
        {

            Tactical_Combat_Draw();

            Assign_Mouse_Image();

            PageFlip_FX();

            Release_Time(1);

        }

        screen_changed = ST_FALSE;

    }

    Deactivate_Help_List();

    Clear_Palette_Changes(0, 255);

    Set_Palette_Changes(0, 223);

    Calculate_Remap_Colors();

    Reset_Cycle_Palette_Color();

    Clear_Fields();

    Deactivate_Auto_Function();

    Set_Mouse_List(1, mouse_list_default);

    Copy_On_To_Off_Page();

    Copy_Off_To_Page4();

    // ; now 0 (non-strategic battle init)
    // ; set to 1 if the AI decides to flee
    if(CMB_AI_Fled == ST_TRUE)
    {
        Battle_Result = 5;
    }
    else if(Combat_Winner == _human_player_idx)
    {
        Battle_Result = 1;
    }
    else if(Player_Fled == ST_TRUE)
    {
        Battle_Result = 3;
    }
    else if(_combat_turn > 50)
    {
        Battle_Result = 4;
    }
    else
    {
        Battle_Result = 2;
    }

    // ; $AC00, to the current draw frame, overwriting
    // ; whatever was there before
    Copy_Page4_To_Off();

    Copy_Off_To_Back();

    Stop_All_Sounds__STUB();

    Combat_Cache_Read();

    GFX_Swap_Cities();

    Mark_Time();

    Release_Time(1);

    // DOMSDOS  Play_Background_Music__STUB();
    sdl2_Play_Background_Music__WIP();

    End_Of_Combat__WIP(Combat_Winner, item_count, item_list, Battle_Result);

    if(Combat_Winner == _combat_attacker_player)
    {
        return ST_TRUE;
    }
    else
    {
        return ST_FALSE;
    }

}




/*
    WIZARDS.EXE  ovr091
*/

// WZD s91p01
// drake178: UU_BU_LoadFigureGFX()

// WZD s91p02
// drake178: CMB_PrepareTurn()
/*
; applies turn-based combat effects, recalculates unit
; statistics, and resets movement allowances
;
; contains a BUG regarding nearly every turn-based
; effect either by itself or inherited from one of the
; called functions
*/
/*

    calls BU_Init_Battle_Unit() on all battle units
    then, manually sets movement_points via Battle_Unit_Moves2()
    PS. BU_Init_Battle_Unit() does battle_unit->movement_points = Unit_Moves2(unit_idx);

*/
void CMB_PrepareTurn__WIP(void)
{
    int16_t Selected_Unit = 0;
    int16_t Roll_Result = 0;
    int16_t itr = 0;  // _SI_

    CMB_ImmobileCanAct = ST_TRUE;

    AI_ImmobileCounter++;

    _combat_turn++;

    CMB_WizCastAvailable = ST_TRUE;

    _scanned_battle_unit = ST_UNDEFINED;

    CMB_AIGoesFirst = ST_FALSE;


    // TODO  CMB_ManaLeak();
    // ; processes the effects of any Mana Leak combat
    // ; enchantments for both parties

    // if(combat_enchantments->Call_Lightning.Attkr != 0)
    if(combat_enchantments[CALL_LIGHTNING_ATTKR] != 0)
    {
        // TODO  WIZ_CallLightning(_combat_attacker_player);
        // ; processes the Call Lightning effect of the selected
        // ; player, including loading and playing both sound and
        // ; animation
        // ;
        // ; BUGs: will sometimes fire less bolts than intended,
        // ; ignores units with Wraith Form, and stores its spell
        // ; data parameters in Wall of Stone's, even though it
        // ; doesn't actually need to
    }

    // if(combat_enchantments->Call_Lightning.Dfndr != 0)
    if(combat_enchantments[CALL_LIGHTNING_DFNDR] != 0)
    {
        // TODO  WIZ_CallLightning(_combat_defender_player);
        // ; processes the Call Lightning effect of the selected
        // ; player, including loading and playing both sound and
        // ; animation
        // ;
        // ; BUGs: will sometimes fire less bolts than intended,
        // ; ignores units with Wraith Form, and stores its spell
        // ; data parameters in Wall of Stone's, even though it
        // ; doesn't actually need to
    }

    // if(combat_enchantments->Wrack.Attkr != 0)
    if(combat_enchantments[WRACK_ATTKR] != 0)
    {
        // TODO     (_combat_attacker_player);
        // ; processes the Wrack effect of the selected player,
        // ; including loading and playing its animation
        // ;
        // ; BUGs: can only affect up to 40 figures at a time, can
        // ; corrupt memory in unlucky scenarios, its effect is
        // ; prevented by Wraith Form, and it does squared the
        // ; intended damage to units
    }

    // if(combat_enchantments->Wrack.Dfndr != 0)
    if(combat_enchantments[WRACK_DFNDR] != 0)
    {
        // TODO  WIZ_Wrack(_combat_defender_player);
        // ; processes the Wrack effect of the selected player,
        // ; including loading and playing its animation
        // ;
        // ; BUGs: can only affect up to 40 figures at a time, can
        // ; corrupt memory in unlucky scenarios, its effect is
        // ; prevented by Wraith Form, and it does squared the
        // ; intended damage to units
    }


    Selected_Unit = _active_battle_unit;

    CMB_ProcessVortices__SEGRAX();

    _active_battle_unit = Selected_Unit;


    // TODO  Init_Battlefield_Effects(CMB_combat_structure);
    // ; calculates and stores the highest unit aura values
    // ; for each player (Holy Bonus, Resistance to All, and
    // ; Leadership), and applies Heavenly Light, Cloud of
    // ; Shadow, and Eternal Night
    // ;
    // ; BUG: grants +3 Defense to defending units in battle
    // ; condition 1 (else wouldn't even need this argument)

    // TODO  CMB_UnitCityDamage();
    // ; increases the probable building damage for each
    // ; offending unit currently in the city proper during
    // ; a town siege

    for(itr = 0; itr < _combat_total_unit_count; itr++)
    {

        battle_units[itr].Suppression = 0;

        if(battle_units[itr].status != bus_Active)
        {
            continue;
        }


        // ¿ Why not Load_Battle_Unit() |-> BU_Init_Battle_Unit() ?
        BU_Init_Battle_Unit(&battle_units[itr]);

        BU_Apply_Battlefield_Effects__WIP(&battle_units[itr]);

        battle_units[itr].movement_points = Battle_Unit_Moves2(itr);  // ¿ manually fix-up the value set in BU_Init_Battle_Unit() ?

        battle_units[itr].action = bus_Active;


        if(
            (
                /* (combat_enchantments->Terror.Attkr != 0) */
                (combat_enchantments[TERROR_ATTKR] != 0)
                &&
                (battle_units[itr].controller_idx == _combat_defender_player)
            )
            ||
            (
                /* (combat_enchantments->Terror.Dfndr != 0) */
                (combat_enchantments[TERROR_DFNDR] != 0)
                &&
                (battle_units[itr].controller_idx == _combat_attacker_player)
            )
        )
        {
                        
            // ¿ ; BUG: this is already applied to the unit itself ?
            // TODO  Roll_Result = BU_ResistRoll(SPUSH(&battle_units[itr]), (CMB_ResistAllArray[battle_units[itr].controller_idx] + 1), spell_data_table[spl_Terror].magic_realm);
            // ; calculates the unit's effective Resistance score,
            // ; then makes a resistance check with the passed
            // ; modifier, returning 0 if the unit succeeded, or the
            // ; difference from the target number if it didn't
            /* HACK */  Roll_Result = 0;

            if(Roll_Result > 0)
            {

                battle_units[itr].action = bua_Finished;

                battle_units[itr].movement_points = 0;

            }

        }


        if(
            (
                /* (combat_enchantments->Entangle.Attkr != 0) */
                (combat_enchantments[ENTANGLE_ATTKR] != 0)
                &&
                (battle_units[itr].controller_idx == _combat_defender_player)
            )
            ||
            (
                /* (combat_enchantments->Entangle.Dfndr != 0) */
                (combat_enchantments[ENTANGLE_DFNDR] != 0)
                &&
                (battle_units[itr].controller_idx == _combat_attacker_player)
            )
        )
        {

            if(battle_units[itr].movement_points > 0)
            {

                battle_units[itr].movement_points -= 2;

            }

            if(battle_units[itr].movement_points < 1)
            {

                battle_units[itr].movement_points = 0;

            }

        }

        if(battle_units[itr].Web_HP > 0)
        {

            if((battle_units[itr].ranged_type / 10) == rag_Missile)
            {

                if(battle_units[itr].melee > battle_units[itr].ranged)
                {

                    Roll_Result = battle_units[itr].melee;

                }
                else
                {

                    Roll_Result = battle_units[itr].ranged;

                }

            }
            else if(battle_units[itr].ranged_type > srat_Thrown)
            {

                Roll_Result = (Roll_Result = battle_units[itr].melee + battle_units[itr].ranged);

            }
            else
            {

                Roll_Result = battle_units[itr].melee;

            }

        }

        battle_units[itr].Web_HP -= Roll_Result; 

        if(battle_units[itr].Web_HP > 0)
        {

            battle_units[itr].movement_points = 0;

        }
        else
        {

            battle_units[itr].Web_HP = 0;

        }


        if((battle_units[itr].Combat_Effects & bue_Black_Sleep) != 0)
        {

            battle_units[itr].action = bua_Finished;

            battle_units[itr].movement_points = 0;

        }


        /*
            BEGIN:  Confusion

                "Every turn the confused unit randomly does one of the following: stand around and do nothing while looking foolish and confused, move randomly, attack allies, or attack enemies."
        */
        // ; BUG: resets the state without ever returning control of the unit to its previous owner!
        // ¿ NOBUG  pretty sure this is covered by having called BU_Init_Battle_Unit() to (re-)initialize the battle unit ?
        battle_units[itr].Confusion_State = 0;

        if((battle_units[itr].Combat_Effects & bue_Confusion) != 0)
        {

            Roll_Result = Random(4);

            if(Roll_Result == 1)
            {

                battle_units[itr].action = bua_Finished;

                battle_units[itr].movement_points = 0;

            }

            if(Roll_Result == 2)
            {

                battle_units[itr].Confusion_State = 1;

            }

            if(Roll_Result == 3)
            {

                battle_units[itr].Confusion_State = 2;

                if(battle_units[itr].controller_idx == _combat_attacker_player)
                {

                    battle_units[itr].controller_idx = _combat_defender_player;

                }
                else
                {

                    battle_units[itr].controller_idx = _combat_attacker_player;

                }

            }

        }
        /*
            END:  Confusion
        */


        /*
            BEGIN:  Regeneration
        */
        if(
            ((battle_units[itr].enchantments & UE_REGENERATION) != 0)
            ||
            ((battle_units[itr].item_enchantments & UE_REGENERATION) != 0)
            ||
            ((_UNITS[battle_units[itr].unit_idx].enchantments & UE_REGENERATION) != 0)
            ||
            ((battle_units[itr].Abilities & UA_REGENERATION) != 0)
        )
        {

            // ¿ ; conflicting condition - will never jump ?
            if(battle_units[itr].status == bus_Active)
            {

                Battle_Unit_Heal(itr, 1, 0);

            }

        }
        /*
            END:  Regeneration
        */

    }

}


// WZD s91p03
// drake178: BU_GetMoveMap()
/*
; checks the unit's combat movement type, and fills
; out the GUI_ActiveMoveMap allocation accordingly
*/
/*
    sets CMB_ActiveMoveMap[]

*/
void Set_Movement_Cost_Map(int16_t battle_unit_idx)
{
    int16_t movement_type = 0;
    int16_t instant_movement_type = 0;  // _SI_
    int16_t itr_y = 0;  // _SI_
    int16_t itr_x = 0;  // _DI_

    movement_type = Battle_Unit_Movement_Icon(battle_unit_idx);

    instant_movement_type = BU_GetInstaMoveType__WIP(battle_unit_idx);

    if(instant_movement_type > 0)
    {
        movement_type = instant_movement_type;
    }

    switch(movement_type)
    {
        case 0:
        case 1:
        {
            memcpy(CMB_ActiveMoveMap, &battlefield->MoveCost_Ground[0], COMBAT_GRID_CELL_COUNT);
        } break;
        case 2:
        {
            memcpy(CMB_ActiveMoveMap, &battlefield->MoveCost_Teleport[0], COMBAT_GRID_CELL_COUNT);
        } break;
        case 3:
        {
            memcpy(CMB_ActiveMoveMap, &battlefield->MoveCost_Ground2[0], COMBAT_GRID_CELL_COUNT);
        } break;
        case 4:
        {
            memcpy(CMB_ActiveMoveMap, &battlefield->MoveCost_Sailing[0], COMBAT_GRID_CELL_COUNT);
        } break;
        case 5:
        {
            for(itr_y = 0; itr_y < COMBAT_GRID_HEIGHT; itr_y++)
            {
                for(itr_x = 0; itr_x < COMBAT_GRID_WIDTH; itr_x++)
                {
                    if(battlefield->MoveCost_Sailing[((itr_y * COMBAT_GRID_WIDTH) + itr_x)] > battlefield->MoveCost_Ground[((itr_y * COMBAT_GRID_WIDTH) + itr_x)])
                    {
                        CMB_ActiveMoveMap[((itr_y * COMBAT_GRID_WIDTH) + itr_x)] = battlefield->MoveCost_Ground[((itr_y * COMBAT_GRID_WIDTH) + itr_x)];
                    }
                    else
                    {
                        CMB_ActiveMoveMap[((itr_y * COMBAT_GRID_WIDTH) + itr_x)] = battlefield->MoveCost_Sailing[((itr_y * COMBAT_GRID_WIDTH) + itr_x)];
                    }
                }
            }
        } break;
        case 6:
        case 7:
        {
            memcpy(CMB_ActiveMoveMap, &battlefield->MoveCost_Teleport[0], COMBAT_GRID_CELL_COUNT);
        } break;
    }

}


// WZD s91p04
// drake178: BU_Move()
/*
; plays the animation for, and moves the specified
; unit to the passed coordinates on the combat map
;
; BUG: prevents moving into the central structure tile
; in regular city battles
; BUG: teleporting units can't use roads
*/
/*


~ MainScr.C  Move_Units_Draw()

*/
void Move_Battle_Unit__WIP(int16_t battle_unit_idx, int16_t target_cgx, int16_t target_cgy)
{
    SAMB_ptr Sound_Data_Seg = 0;
    uint32_t Sound_Data_Seg_size = 0;  // DNE in Dasm
    int16_t Can_Teleport = 0;
    int16_t Teleport_Type = 0;
    int16_t Last_Facing_Y = 0;
    int16_t Last_Facing_X = 0;
    int16_t Origin_Y = 0;
    int16_t Origin_X = 0;
    int16_t battle_unit_owner_idx = 0;
    int16_t Facing_Diff_Y = 0;
    int16_t Facing_Diff_X = 0;
    int16_t Move_Step_Index = 0;
    int16_t itr;  // _DI_

    battle_unit_owner_idx = battle_units[battle_unit_idx].controller_idx;

    Set_Movement_Cost_Map(battle_unit_idx);

    for(itr = 0; itr < _combat_total_unit_count; itr++)
    {

        if(battle_units[itr].status == bus_Active)
        {

            CMB_ActiveMoveMap[((battle_units[itr].cgy * COMBAT_GRID_WIDTH) + battle_units[itr].cgx)] = INF;  /* ¿ occupied ? */

        }

    }

    for(itr = 0; itr < CMB_Vortex_Count; itr++)
    {

        CMB_ActiveMoveMap[((CMB_Vortex_Array[itr].cgy * COMBAT_GRID_WIDTH) + CMB_Vortex_Array[itr].cgx)] = INF;  /* ¿ occupied ? */

    }


    // ; BUG: outposts and cities don't have anything on that
    // ; tile either
    // same as in Assign_Combat_Grids()
    if(battlefield->Central_Structure != CS_None)
    {

        CMB_ActiveMoveMap[COMBAT_STRUCTURE_IDX] = INF;

    }


    if(
        (battlefield->Walled_City == ST_TRUE)
        ||
        (battlefield->city_enchantments[FLYING_FORTRESS] > 0)
    )
    {

        // TODO  BU_SetCityMovement(battle_unit_idx);
        // ; modifies the GUI_ActiveMoveMap based on whether the
        // ; unit is allowed to enter the city proper or not,
        // ; depending on any intact walls and Flying Fortress
        // ;
        // ; BUGs: allows Merging and Non-Corporeal units to enter
        // ; a Flying Fortress even if they don't fly, and can set
        // ; the central structure tile impassable even when there
        // ; isn't one
        
    }


    Can_Teleport = ST_FALSE;

    Teleport_Type = BU_GetInstaMoveType__WIP(battle_unit_idx);

    if(
        (Teleport_Type == 6)
        ||
        (Teleport_Type == 7)
    )
    {

        movement_path_grid_cell_count = 0;

        Can_Teleport = ST_TRUE;

    }
    else
    {
        // HERE: check the memory view of the path cost map
        Combat_Move_Path_Find(battle_units[battle_unit_idx].cgx, battle_units[battle_unit_idx].cgy, target_cgx, target_cgy);

        if(movement_path_grid_cell_count == 0)
        {
            return;
        }

    }

    Origin_X = battle_units[battle_unit_idx].cgx;

    Origin_Y = battle_units[battle_unit_idx].cgy;

    RP_CMB_Movement_Var = 0;  // ; cleared before movement, never used for anything

    battle_units[battle_unit_idx].Moving = ST_TRUE;

    // DOMSDOS Sound_Data_Seg = BU_PrepMoveSound(battle_unit_idx);
    BU_PrepMoveSound__WIP(battle_unit_idx, &Sound_Data_Seg, &Sound_Data_Seg_size);

    if(Can_Teleport != ST_FALSE)
    {

        if(Teleport_Type == 6)
        {

            // TODO  BU_Teleport(battle_unit_idx, target_cgx, target_cgy);
            // ; plays the teleport animation for, and moves the
            // ; specified unit to the selected tile on the field

        }
        else  /* Teleport_Type == 7 */
        {

            // TODO  BU_TunnelTo(battle_unit_idx, target_cgx, target_cgy);
            // ; plays the tunneling (Merging) animation for, and
            // ; moves the specified unit to the selected tile on the
            // ; battlefield
            // ;
            // ; BUG: ignores the unit movement animations setting

        }

    }
    else
    {

        battle_units[battle_unit_idx].target_cgx = target_cgx;

        battle_units[battle_unit_idx].target_cgy = target_cgy;

        for(Move_Step_Index = 0; Move_Step_Index < movement_path_grid_cell_count; Move_Step_Index++)
        {

            battle_units[battle_unit_idx].target_cgx = CMB_Path_Xs[Move_Step_Index];

            battle_units[battle_unit_idx].target_cgy = CMB_Path_Ys[Move_Step_Index];

            if(magic_set.sound_effects == ST_TRUE)
            {
                // Play_Sound__WIP(Sound_Data_Seg);
                sdl2_Play_Sound__WIP(Sound_Data_Seg, Sound_Data_Seg_size);
            }

            if(magic_set.Movement_Anims == ST_TRUE)
            {

                for(itr = 0; itr < 8; itr++)
                {

                    battle_units[battle_unit_idx].MoveStage += 1;

                    Tactical_Combat_Draw();

                    PageFlip_FX();

                }

            }
            else
            {

                battle_units[battle_unit_idx].MoveStage = 7;

                Tactical_Combat_Draw();

                PageFlip_FX();

            }

            if(magic_set.sound_effects == ST_TRUE)
            {
                // DOMSDOS  Play_Sound__WIP(SND_CMB_Silence);
                sdl2_Play_Sound__WIP(SND_CMB_Silence, SND_CMB_Silence_size);
            }

            // TODO  BU_WallofFire(battle_unit_idx);
            // ; check whether the unit's target coordinates are
            // ; through a Wall of Fire from the outside, and if so,
            // ; process and apply a base strength Fireball effect
            // ; targeted at it

            if(battle_units[battle_unit_idx].status != bus_Active)
            {
                break;
            }

            battle_units[battle_unit_idx].MoveStage = 0;

            battle_units[battle_unit_idx].cgx = CMB_Path_Xs[Move_Step_Index];

            battle_units[battle_unit_idx].cgy = CMB_Path_Ys[Move_Step_Index];

            assert(battle_units[battle_unit_idx].cgx >= COMBAT_GRID_XMIN);
            assert(battle_units[battle_unit_idx].cgx <= COMBAT_GRID_XMAX);
            assert(battle_units[battle_unit_idx].cgy >= COMBAT_GRID_YMIN);
            assert(battle_units[battle_unit_idx].cgy <= COMBAT_GRID_YMAX);

        }

    }


    battle_units[battle_unit_idx].MoveStage = 0;

    battle_units[battle_unit_idx].Moving = ST_FALSE;

    if(movement_path_grid_cell_count > 1)
    {

        Origin_X = CMB_Path_Xs[(movement_path_grid_cell_count - 2)];

        Origin_Y = CMB_Path_Ys[(movement_path_grid_cell_count - 2)];

        Last_Facing_X = CMB_Path_Xs[(movement_path_grid_cell_count - 1)];

        Last_Facing_Y = CMB_Path_Ys[(movement_path_grid_cell_count - 1)];

    }
    else
    {

        Last_Facing_X = target_cgx;

        Last_Facing_Y = target_cgy;

    }


    Facing_Diff_X = (Last_Facing_X - Origin_X);

    Facing_Diff_Y = (Last_Facing_Y - Origin_Y);

    if(Can_Teleport != ST_FALSE)
    {
        // TODO  Can_Teleport
    }
    else
    {

        battle_units[battle_unit_idx].cgx = battle_units[battle_unit_idx].target_cgx;

        battle_units[battle_unit_idx].cgy = battle_units[battle_unit_idx].target_cgy;  // ; already done during movement?

        assert(battle_units[battle_unit_idx].cgx >= COMBAT_GRID_XMIN);
        assert(battle_units[battle_unit_idx].cgx <= COMBAT_GRID_XMAX);
        assert(battle_units[battle_unit_idx].cgy >= COMBAT_GRID_YMIN);
        assert(battle_units[battle_unit_idx].cgy <= COMBAT_GRID_YMAX);

        battle_units[battle_unit_idx].target_cgx = (Last_Facing_X + Facing_Diff_X);

        battle_units[battle_unit_idx].target_cgy = (Last_Facing_Y + Facing_Diff_Y);

        battle_units[battle_unit_idx].movement_points -= CMB_Path_Costs[((battle_units[battle_unit_idx].cgy * COMBAT_GRID_WIDTH) + battle_units[battle_unit_idx].cgx)];

    }

    SETMIN(battle_units[battle_unit_idx].movement_points, 0);

}


// WZD s91p05
// drake178: G_BU_SelectUnit()
/*
; BUG: should set the selected unit manually!
; corrupts memory is the last value is invalid

; selects the active battle unit in combat?
;
; zeroes out field 52h in the old one
;
; WILL CORRUPT MEMORY IF THE CURRENT SELECTION IS
; INVALID / OUT OF THE TABLE BOUNDS!
*/
/*

XREF:  (9)
    j_Switch_Active_Battle_Unit()
        Tactical_Combat__WIP+240
        Tactical_Combat__WIP+298
        Tactical_Combat__WIP+312
        Tactical_Combat__WIP+BAA
        WIZ_BU_SelectClosest+186
        WIZ_BU_SelectClosest+1CD
        AI_BU_ProcessAction+5E
        AI_MoveBattleUnits+290
        AI_MoveBattleUnits+384

*/
void Switch_Active_Battle_Unit(int16_t battle_unit_idx)
{

    battle_units[_active_battle_unit].Moving = ST_FALSE;

    _active_battle_unit = battle_unit_idx;

}


// WZD s91p06
// drake178: BU_CombatAction()
/*
; if possible, processes an action for the specified
; unit, based on the passed target coordinates -
; movement if the tile is empty and within reach, or an
; attack if there's an enemy unit or crushable wall on
; the target tile
;
; BUGs: inherited from BU_MeleeFlightCheck and
;  BU_MeleeWallCheck
*/
/*
Left-Click Combat Grid
passed in ~x,y of combat grid cell that qualifies for the left-click
*action* deduced from *unit* in target grid cell


Tactical_Combat__WIP()
    frame_scanned_cgx = Get_Combat_Grid_Cell_X((Grid_X + 4), (Grid_Y + 4));
    frame_scanned_cgy = Get_Combat_Grid_Cell_Y((Grid_X + 4), (Grid_Y + 4));
    |-> Battle_Unit_Action__WIP(_active_battle_unit, frame_scanned_cgx, frame_scanned_cgy);

*/
void Battle_Unit_Action__WIP(int16_t battle_unit_idx, int16_t cgx, int16_t cgy)
{
    int16_t Target_Y = 0;
    int16_t Target_X = 0;
    int16_t ranged_group = 0;  /* ~ "ranged attack type" */
    int16_t Unused_Local = 0;
    int16_t Y_Distance = 0;
    int16_t X_Distance = 0;
    int16_t combat_grid_target = 0;  // _DI_

    combat_grid_target = CMB_TargetRows[cgy][cgx];

    /*
        DEDU  test condition for *action* of 'Left-Click Combat Grid'


    */
    if(
        (combat_grid_target == 99)  /* City Wall */
        ||
        (
            (combat_grid_target >= 0)
            &&
            (battle_units[combat_grid_target].controller_idx != battle_units[battle_unit_idx].controller_idx)
            &&
            (battle_units[combat_grid_target].status == bus_Active)
        )
    )
    {

        if(combat_grid_target == 99)  /* City Wall */
        {

            Target_X = cgx;  // passed in - frame_scanned_cgx = Get_Combat_Grid_Cell_X((Grid_X + 4), (Grid_Y + 4));

            Target_Y = cgy;  // passed in - frame_scanned_cgy = Get_Combat_Grid_Cell_Y((Grid_X + 4), (Grid_Y + 4));

        }
        else
        {

            Target_X = battle_units[combat_grid_target].cgx;

            Target_Y = battle_units[combat_grid_target].cgy;

        }

        X_Distance = abs(Target_X - battle_units[battle_unit_idx].cgx);  // passed in - _active_battle_unit

        Y_Distance = abs(Target_Y - battle_units[battle_unit_idx].cgy);  // passed in - _active_battle_unit

        Unused_Local = -2;  // ¿ -2 as in invalid target ?  ...can't think of any reason this would be here, not even as legacy debug code

        if(battle_units[battle_unit_idx].movement_points > 0)
        {

            if(combat_grid_target == 99)  /* City Wall */
            {

                // if "ranged attack" else "melee attack"
                if(
                    (X_Distance > 1)
                    ||
                    (Y_Distance > 1)
                )
                {

                    ranged_group = (battle_units[battle_unit_idx].ranged_type / 10);

                    if(
                        (ranged_group == rag_Boulder)
                        ||
                        (ranged_group == rag_Missile)
                        ||
                        (ranged_group == rag_Magic)
                    )
                    {

                        Battle_Unit_Attack__WIP(battle_unit_idx, combat_grid_target, Target_X, Target_Y);

                    }

                }
                else
                {

                    Battle_Unit_Attack__WIP(battle_unit_idx, combat_grid_target, Target_X, Target_Y);

                }

            }
            else  /* (combat_grid_target != 99) */
            {

                // ¿ can attack, if attacker has flight or both do ?
                if(Check_Attack_Melee(battle_unit_idx, combat_grid_target) == ST_TRUE)
                {

                    if(
                        (X_Distance > 1)
                        ||
                        (Y_Distance > 1)
                    )
                    {

                        ranged_group = (battle_units[battle_unit_idx].ranged_type / 10);

                        if(
                            (ranged_group == rag_Boulder)
                            ||
                            (ranged_group == rag_Missile)
                            ||
                            (ranged_group == rag_Magic)
                        )
                        {

                            Battle_Unit_Attack__WIP(battle_unit_idx, combat_grid_target, Target_X, Target_Y);

                        }

                    }
                    else  /* (X_Distance <= 0) && (Y_Distance <= ) */
                    {

                        if(BU_MeleeWallCheck__WIP(battle_unit_idx, combat_grid_target) == ST_TRUE)
                        {

                            Battle_Unit_Attack__WIP(battle_unit_idx, combat_grid_target, Target_X, Target_Y);

                        }
                        else
                        {

                            ranged_group = (battle_units[battle_unit_idx].ranged_type / 10);

                            if(
                                (ranged_group == rag_Boulder)
                                ||
                                (ranged_group == rag_Missile)
                                ||
                                (ranged_group == rag_Magic)
                            )
                            {

                                Battle_Unit_Attack__WIP(battle_unit_idx, combat_grid_target, Target_X, Target_Y);

                            }

                        }

                    }

                }

            }

        }

    }
    else
    {

        if(
            (combat_grid_target == -1)  /* ¿ empty and reachable ? */
            &&
            (battle_units[battle_unit_idx].movement_points > 0)
        )
        {

            Move_Battle_Unit__WIP(battle_unit_idx, cgx, cgy);

        }

    }


}


// WZD s91p07
// drake178: CMB_FillTargetMaps()
/*
; fills out the CMB_ActiveMoveMap, CMB_Path_Costs@,
; and CMB_TargetRows@ arrays, and sets into the
; CMB_NearBuffer_3 0s or 1s depending whether the
; corresponding tile can be reached - all based on the
; _active_battle_unit index
*/
/*

populates CMB_TargetRows[] and CMB_ActiveMoveMap[]
uses CMB_NearBuffer_3[]

CMB_TargetRows[]

CMB_ActiveMoveMap[]
...just sets occupied cells to *impassible* (255)?

*/
void Assign_Combat_Grids(void)
{
    int16_t Can_Teleport = 0;
    int16_t Teleport_Type = 0;
    int16_t moves2 = 0;
    int16_t uu_max_moves2 = 0;
    int16_t uu_count_of_reachable_cells = 0;
    int16_t uu_cgy = 0;
    int16_t uu_cgx = 0;
    int16_t cgy_offset = 0;
    int16_t itr = 0;
    int16_t itr_x = 0;  // DNE in Dasm, uses itr
    int16_t itr_y = 0;  // _SI_
    int16_t useable_moves2 = 0;  // _DI_

    
    /* DEBUG */  if(battle_units[_active_battle_unit].Unused_1Bh == spl_Animate_Dead)
    /* DEBUG */  {
    /* DEBUG */      STU_DEBUG_BREAK();
    /* DEBUG */  }


    Set_Movement_Cost_Map(_active_battle_unit);


    for(itr = 0; itr < _combat_total_unit_count; itr++)
    {

        /* DEBUG */  if(battle_units[itr].Unused_1Bh == spl_Animate_Dead)
        /* DEBUG */  {
        /* DEBUG */      STU_DEBUG_BREAK();
        /* DEBUG */  }

        if(battle_units[itr].status == bus_Active)
        {

            CMB_ActiveMoveMap[((battle_units[itr].cgy * COMBAT_GRID_WIDTH) + battle_units[itr].cgx)] = -1;

        }

    }


    for(itr= 0; itr < CMB_Vortex_Count; itr++)
    {

        CMB_ActiveMoveMap[((CMB_Vortex_Array[itr].cgy * COMBAT_GRID_WIDTH) + CMB_Vortex_Array[itr].cgx)] = -1;

    }


    // ; BUG: cities don't have anything on that tile either
    if(battlefield->Central_Structure!= CS_None)
    {

        CMB_ActiveMoveMap[COMBAT_STRUCTURE_IDX] = INF;  // EDh  237d  237 / 21 = 11.28571  11 * 21 = 231  237 - 231 = 6  ~ cgx = 6, cgy = 11

    }

    if(
        (battlefield->Walled_City == ST_TRUE)
        ||
        (battlefield->city_enchantments[FLYING_FORTRESS] > 0)
    )
    {

        BU_SetCityMovement__WIP(_active_battle_unit);

    }


/*
    NOTE: nothing else touches CMB_ActiveMoveMap[]
*/


    Can_Teleport = ST_FALSE;

    Teleport_Type = BU_GetInstaMoveType__WIP(_active_battle_unit);

    if(
        (Teleport_Type == 6)
        ||
        (Teleport_Type == 7)
    )
    {

        Can_Teleport = ST_TRUE;

    }


    moves2 = battle_units[_active_battle_unit].movement_points;

    uu_max_moves2 = Unit_Moves2(battle_units[_active_battle_unit].unit_idx);

    uu_cgx = battle_units[_active_battle_unit].cgx;

    uu_cgy = battle_units[_active_battle_unit].cgy;


    /*
        default all combat grid targets to invalid action targets
        ¿ where does this get checked ?
    */
    for(itr_y = 0; itr_y < COMBAT_GRID_HEIGHT; itr_y++)
    {

        for(itr_x = 0; itr_x < COMBAT_GRID_WIDTH; itr_x++ )
        {

            CMB_TargetRows[itr_y][itr_x] = -2;  /* not valid as targets for an active unit’s actions */

        }

    }


    /*
        tested for in Battle_Unit_Action__WIP()
    */
    if(
        (battlefield->Walled_City == ST_TRUE)
        &&
        (battle_units[_active_battle_unit].controller_idx == _combat_attacker_player)
        &&
        ((battle_units[_active_battle_unit].Abilities & UA_WALLCRUSHER) != 0)
    )
    {

        for(itr_y = 0; itr_y < COMBAT_GRID_HEIGHT; itr_y++)
        {

            for(itr_x = 0; itr_x < COMBAT_GRID_WIDTH; itr_x++ )
            {

                if(Combat_Grid_Cell_Has_City_Wall(itr_x, itr_y) == ST_TRUE)
                {

                    CMB_TargetRows[itr_y][itr_x] = 99;

                }

            }

        }

    }


    /*
        BEGIN:  assign Battle Unit Indices
    */
    for(itr = 0; itr < _combat_total_unit_count; itr++)
    {

        /* DEBUG */  if(battle_units[itr].Unused_1Bh == spl_Animate_Dead)
        /* DEBUG */  {
        /* DEBUG */      STU_DEBUG_BREAK();
        /* DEBUG */  }

        if(
            (battle_units[itr].status == bus_Active)
            &&
            (battle_units[itr].Image_Effect != 5)  // ~ Invisible
        )
        {

            CMB_TargetRows[battle_units[itr].cgy][battle_units[itr].cgx] = itr;  // batle_unit_idx

        }

    }
    /*
        END:  assign Battle Unit Indices
    */


    useable_moves2 = 0;

    if(Can_Teleport == ST_FALSE)
    {

        useable_moves2 = battle_units[_active_battle_unit].movement_points;

    }
    else
    {

        if(battle_units[_active_battle_unit].movement_points > 0)
        {

            useable_moves2 = 20;

        }

    }


    /*

    */
    uu_count_of_reachable_cells = 0;

    // sets CMB_NearBuffer_3[] to {F,T} - reachable
    // ¿ MoO2 Set_Legal_Moves_() ?
    Combat_Move_Path_Valid(battle_units[_active_battle_unit].cgx, battle_units[_active_battle_unit].cgy, useable_moves2);

    if(
        (battle_units[_active_battle_unit].cgx == 14)
        &&
        (battle_units[_active_battle_unit].cgy == 12)
    )
    {
        
        // STU_DEBUG_BREAK();

        // assert(CMB_NearBuffer_3[((battle_units[_active_battle_unit].cgy * COMBAT_GRID_WIDTH) + battle_units[_active_battle_unit].cgx)] == _active_battle_unit);

        DBG_ptr_CMB_NearBuffer_3 = &CMB_NearBuffer_3[( ((battle_units[_active_battle_unit].cgy - 1) * COMBAT_GRID_WIDTH) + (battle_units[_active_battle_unit].cgx - 1) )];

//        assert(CMB_NearBuffer_3[
//            (
//                ((battle_units[_active_battle_unit].cgy - 1) * COMBAT_GRID_WIDTH)
//                +
//                (battle_units[_active_battle_unit].cgx - 1)
//            ) ] == ST_TRUE
//        );

    }


    for(itr_y = 0; itr_y < COMBAT_GRID_HEIGHT; itr_y++)
    {

        cgy_offset = (itr_y * COMBAT_GRID_WIDTH);

        for(itr_x = 0; itr_x < COMBAT_GRID_WIDTH; itr_x++)
        {

            if(CMB_NearBuffer_3[cgy_offset + itr_x] == ST_TRUE)  /* combat grid cell is *reachable* */
            {

                CMB_TargetRows[itr_y][itr_x] = -1;

                uu_count_of_reachable_cells++;

            }

        }

    }


    CMB_TargetRows[battle_units[_active_battle_unit].cgy][battle_units[_active_battle_unit].cgx] = _active_battle_unit;

}


// WZD s91p08
// drake178: BU_IsVisible()
int16_t BU_IsVisible__STUB(int16_t battle_unit_idx)
{

    return ST_TRUE;

}


// WZD s91p09
// drake178: CMB_UnitCityDamage()

// WZD s91p10
// drake178: BU_GetInstaMoveType()
/*
; returns the type of any instant movement ability
; that the unit has (6 for Teleporting, 7 for Merging),
; or 0 if neither
*/
/*
    returns 6 for teleporting, 7 for merging, or 0 if neither
*/
int16_t BU_GetInstaMoveType__WIP(int16_t battle_unit_idx)
{

    if((battle_units[battle_unit_idx].Move_Flags & MV_TELEPORT) != 0)
    {

        return 6;

    }
    else if((battle_units[battle_unit_idx].Move_Flags & MV_MERGING) != 0)
    {

        return 7;

    }
    else
    {

        return 0;

    }

}



/*
    WIZARDS.EXE  ovr096
*/

// WZD s96p01
/*
Move_Units() |-> Combat()
Lair_Combat() |-> ... |-> Combat()
...
Combat()
    |-> { STK_CaptureCity(), Tactical_Combat(), Strategic_Combat() }

*/
int16_t Combat__WIP(int16_t attacker_player_idx, int16_t defender_player_idx, int16_t troop_count, int16_t troops[])
{
    int16_t Item_List[18] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t City_Destroyed = 0;
    int16_t Item_Count = 0;
    int16_t Garrison_Size = 0;
    int16_t combat_attacker_player_idx = 0;
    int16_t Battle_Outcome = 0;
    int16_t defender_idx = 0;  // _DI_
    int16_t itr = 0;  // _SI_
    int16_t combat_defender_player_idx = 0;
    int16_t return_value = 0;  // DNE in Dasm


    City_Destroyed = ST_FALSE;

    GAME_RazeCity = ST_FALSE;

    _unit_stack_count = 0;

    combat_attacker_player_idx = attacker_player_idx;
    defender_idx = defender_player_idx;  // unit_idx or player_idx

    for(itr = 0; itr < troop_count; itr++)
    {
        _UNITS[troops[itr]].wx = OVL_Action_OriginX;
        _UNITS[troops[itr]].wy = OVL_Action_OriginY;
    }

    if(combat_attacker_player_idx == NEUTRAL_PLAYER_IDX)
    {
        switch(OVL_Action_Type)
        {
            case 0:  // Stack vs. Stack
            {
                if(_UNITS[defender_idx].owner_idx == NEUTRAL_PLAYER_IDX)
                {
                    for(itr = 0; itr < troop_count; itr++)
                    {
                        _UNITS[troops[itr]].Status = us_Ready;
                    }
                    goto No_Combat;
                }
            } break;
            case 1:  // Stack vs. City
            {
                if(_CITIES[OVL_Action_Structure].owner_idx == NEUTRAL_PLAYER_IDX)
                {
                    for(itr = 0; itr < troop_count; itr++)
                    {
                        _UNITS[troops[itr]].Status = us_Ready;
                    }
                    goto No_Combat;
                }
            } break;
            case 5:  // Stack vs. Lair
            {
                for(itr = 0; itr < troop_count; itr++)
                {
                    _UNITS[troops[itr]].Status = us_Ready;
                }
                goto No_Combat;
            } break;
        }
    }

    switch(OVL_Action_Type)
    {
        case 0:  // Stack vs. Stack
        {
            _combat_wx = _UNITS[defender_idx].wx;
            _combat_wy = _UNITS[defender_idx].wy;
            _combat_wp = _UNITS[defender_idx].wp;
            defender_idx = _UNITS[defender_idx].owner_idx;  // NOTE: after this, defender_idx is player_idx for both combat type 0 and 1
        } break;
        case 1:  // Stack vs. City
        {
            // TODO  _players.banner_id+17E8h], BNR_Brown
            _combat_wx = _CITIES[OVL_Action_Structure].wx;
            _combat_wy = _CITIES[OVL_Action_Structure].wy;
            _combat_wp = _CITIES[OVL_Action_Structure].wp;
            Garrison_Size = 0;
            for(itr = 0; itr < _units; itr++)
            {
                if(
                    (_UNITS[itr].wx == _combat_wx) &&
                    (_UNITS[itr].wy == _combat_wy) &&
                    (_UNITS[itr].wp == _combat_wp) &&
                    (_UNITS[itr].owner_idx == defender_idx)
                )
                {
                    Garrison_Size++;
                }

            }

        } break;
        case 5:  // Stack vs. Lair
        {
            // TODO  _players.banner_id+17E8h], BNR_Brown
            Lair_Make_Guardians(OVL_Action_Structure);
            _combat_wx = _LAIRS[OVL_Action_Structure].wx;
            _combat_wy = _LAIRS[OVL_Action_Structure].wy;
            _combat_wp = _LAIRS[OVL_Action_Structure].wp;
        } break;
    }

    if(
        (combat_attacker_player_idx < NEUTRAL_PLAYER_IDX) &&
        (defender_idx < NEUTRAL_PLAYER_IDX) &&
        (
            (OVL_Action_Type == 1) ||
            (OVL_Action_Type == 0)
        )
    )
    {
        if(
            (_players[defender_idx].Dipl.Dipl_Status[combat_attacker_player_idx] == DIPL_Alliance)
            ||
            (_players[defender_idx].Dipl.Dipl_Status[combat_attacker_player_idx] == DIPL_WizardPact)
        )
        {
            if(combat_attacker_player_idx != _human_player_idx)
            {
                for(itr = 0; itr < troop_count; itr++)
                {
                    _UNITS[troops[itr]].Status = us_Ready;
                }
                goto No_Combat;
            }
            else
            {
                strcpy(GUI_NearMsgString, cnst_TreatyAtk_Msg1);  // "You have a treaty with "
                strcat(GUI_NearMsgString, _players[defender_idx].name);
                strcat(GUI_NearMsgString, cnst_TreatyAtk_Msg2);  // ".  Do you still wish to attack?"
                if(Confirmation_Box(GUI_NearMsgString) != ST_FALSE)
                {
                    if(
                        (combat_attacker_player_idx < 6)
                        &&
                        (defender_idx < 6)
                    )
                    {
                        // drake178: ; BUG: calling these two functions in this order can allow war without declaring or properly applying it
                        Change_Relations(-40, combat_attacker_player_idx, defender_idx, 5, 0, 0);
                        Break_Treaties(defender_idx, combat_attacker_player_idx);
                    }
                }
                else
                {
                    goto No_Combat;
                }
            }

        }
    }

    if(
        (OVL_Action_Type == 1)  /* Enemy City */
        &&
        (Garrison_Size < 1)
    )
    {
        Battle_Outcome = ST_TRUE;
        Item_Count = 0;
        STK_CaptureCity__WIP(troop_count, &troops[0]);
    }
    else
    {
        if(
            (
                (combat_attacker_player_idx == _human_player_idx)
                ||
                (defender_idx == _human_player_idx)
            )
            &&
            (magic_set.strategic_combat_only == ST_FALSE)
        )
        {

            // DOMSDOS  Stop_All_Sounds__STUB();

            Battle_Outcome = Tactical_Combat__WIP(combat_attacker_player_idx, defender_idx, troops, troop_count, _combat_wx, _combat_wy, _combat_wp, &Item_Count, &Item_List[0]);
            // Battle_Outcome = Combat_Screen_TST_001();
            // Battle_Outcome = Combat_Screen_TST_002();
            // Battle_Outcome = Combat_Screen_TST_003();
            // Battle_Outcome = Combat_Screen_TST_004(combat_attacker_player_idx, defender_idx, troops, troop_count, _combat_wx, _combat_wy, _combat_wp, &Item_Count, &Item_List[0]);

            // DOMSDOS  Play_Background_Music__STUB();
            sdl2_Play_Background_Music__WIP();
        }
        else
        {
            Battle_Outcome = Strategic_Combat__WIP(troops, troop_count, _combat_wx, _combat_wy, _combat_wp, &Item_Count, &Item_List[0]);
            Item_Count = 0;
        }
    }


// @@StartPostCombat
    if(Battle_Outcome == ST_TRUE)
    {
        for(itr = 0; itr < troop_count; itr++)
        {
            if(_UNITS[troops[itr]].owner_idx != 100)  // DEDU  ¿ another case of this field being a status, like 'dead' ?
            {
                _UNITS[troops[itr]].wx = _combat_wx;
                _UNITS[troops[itr]].wy = _combat_wy;
                _UNITS[troops[itr]].wp = _combat_wp;
            }
            else
            {
                _UNITS[troops[itr]].owner_idx = combat_attacker_player_idx;
                _UNITS[troops[itr]].Finished = ST_TRUE;
            }
        }

        item_pool_in_process = ST_TRUE;
        m_item_wx = _combat_wx;
        m_item_wy = _combat_wy;
        m_item_wp = _combat_wp;
        Player_Process_Item_Pool(combat_attacker_player_idx, Item_Count, &Item_List[0]);
        item_pool_in_process = ST_FALSE;

        if(
            (OVL_Action_Type == 1)  /* Enemy City */
            &&
            (_CITIES[OVL_Action_Structure].owner_idx != combat_attacker_player_idx)
        )
        {
            if(_CITIES[OVL_Action_Structure].size != 0)
            {
                if(
                    (combat_attacker_player_idx < NUM_PLAYERS) &&
                    (defender_idx < NUM_PLAYERS)
                )
                {
                    Change_Relations(-40, combat_attacker_player_idx, defender_idx, 9, OVL_Action_Structure, 0);
                }

                if(
                    (combat_attacker_player_idx != NEUTRAL_PLAYER_IDX) ||
                    ((_unit_type_table[_UNITS[troops[0]].type].Abilities & UA_FANTASTIC) != 0)
                )
                {
                    if(
                        (combat_attacker_player_idx < NUM_PLAYERS) &&
                        (defender_idx < NUM_PLAYERS)
                    )
                    {
                        Declare_War(combat_attacker_player_idx, defender_idx);
                    }

                    if(GAME_RazeCity == ST_FALSE)
                    {
                        Change_City_Ownership(OVL_Action_Structure, combat_attacker_player_idx);

                        if(combat_attacker_player_idx != _human_player_idx)
                        {
                            _CITIES[OVL_Action_Structure].construction = bt_AUTOBUILD;  // -4 gran vizier
                        }

                    }

                }

            }
            else
            {
                if(
                    (combat_attacker_player_idx != NEUTRAL_PLAYER_IDX) ||
                    ((_unit_type_table[_UNITS[troops[0]].type].Abilities & UA_FANTASTIC) != 0)
                )
                {
                    City_Destroyed = ST_TRUE;
                }
            }
        }

    }
    else
    {
        Player_Process_Item_Pool(defender_idx, Item_Count, &Item_List[0]);
        item_pool_in_process = ST_FALSE;  // drake178: BUG: never set it to 1 in this branch

        if(OVL_Action_Type == 1)  /* Enemy City */
        {
            if(_CITIES[OVL_Action_Structure].owner_idx == defender_idx)
            {
                if(
                    (combat_attacker_player_idx < 6) &&
                    (defender_idx < 6)
                )
                {
                    if(_CITIES[OVL_Action_Structure].size != 0)
                    {
                        if(Player_Fortress_City(defender_idx) != OVL_Action_Structure)
                        {
                            Change_Relations(-20, combat_attacker_player_idx, defender_idx, 9, OVL_Action_Structure, 0);
                        }
                        else
                        {
                            Change_Relations(-60, combat_attacker_player_idx, defender_idx, 9, OVL_Action_Structure, 0);
                        }
                    }

                }

            }
            else
            {
                if(_CITIES[OVL_Action_Structure].size == 0)
                {
                    City_Destroyed = ST_TRUE;
                }
                else
                {
                    if(GAME_RazeCity == ST_FALSE)
                    {
                        Change_City_Ownership(OVL_Action_Structure, defender_idx);
                    }
                }
            }

        }

    }



    Update_Defender_Hostility(combat_attacker_player_idx, defender_idx);

    if(City_Destroyed == ST_TRUE)
    {
        Change_City_Ownership(OVL_Action_Structure, combat_attacker_player_idx);
        Destroy_City(OVL_Action_Structure);
        GAME_RazeCity = ST_FALSE;
    }

    if(GAME_RazeCity == ST_TRUE)
    {
        Change_City_Ownership(OVL_Action_Structure, combat_attacker_player_idx);
        Destroy_City(OVL_Action_Structure);
        GAME_RazeCity = ST_FALSE;
    }

    OVL_Action_Structure = ST_UNDEFINED;
    OVL_Action_Type = ST_UNDEFINED;

    // DONT j_EmptyFxn_o153p24()

    Allocate_Reduced_Map();

    if(
        (combat_attacker_player_idx == _human_player_idx) ||
        (defender_idx == _human_player_idx)
    )
    {
        Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);
        Full_Draw_Main_Screen();
        Copy_On_To_Off_Page();
    }

    return_value = Battle_Outcome;

    goto Done;


No_Combat:
    OVL_Action_Structure = ST_UNDEFINED;
    OVL_Action_Type = ST_UNDEFINED;
    return_value = 0;
    goto Done;


Done:
    return return_value;
}


// WZD s96p02
// drake178: EZ_CreateGuardStack()
// MoO2: DNE
void Lair_Make_Guardians(int16_t lair_idx)
{
    int16_t guard_count = 0;
    int16_t unit_count = 0;
    int16_t itr = 0;  // _DI_

    // TODO figure out lair guardian count high and low nibbles, so you can make a macro for them
    // TODO make macro for lair guardian count high and low nibbles
    guard_count = (_LAIRS[lair_idx].guard1_count & 0x0F);

    unit_count = 0;

    for(itr = 0; ((itr < guard_count) && (unit_count < MAX_STACK)); itr++)
    {
        if(Create_Unit__WIP(_LAIRS[lair_idx].guard1_unit_type, NEUTRAL_PLAYER_IDX, _LAIRS[lair_idx].wx, _LAIRS[lair_idx].wy, _LAIRS[lair_idx].wp, 2000) == ST_TRUE)
        {
            if(_LAIRS[lair_idx].guard1_unit_type < ut_Magic_Spirit)
            {
                _UNITS[(_units - 1)].mutations = (_UNITS[(_units - 1)].mutations | UM_UNDEAD);
            }
            unit_count++;
        }
    }

    guard_count = (_LAIRS[lair_idx].guard2_count & 0x0F);

    for(itr = 0; ((itr < guard_count) && (unit_count < MAX_STACK)); itr++)
    {
        if(Create_Unit__WIP(_LAIRS[lair_idx].guard2_unit_type, NEUTRAL_PLAYER_IDX, _LAIRS[lair_idx].wx, _LAIRS[lair_idx].wy, _LAIRS[lair_idx].wp, 2000) == ST_TRUE)
        {
            if(_LAIRS[lair_idx].guard2_unit_type < ut_Magic_Spirit)
            {
                _UNITS[(_units - 1)].mutations = (_UNITS[(_units - 1)].mutations | UM_UNDEAD);
            }
            unit_count++;
        }
    }

}


// WZD s96p03
// drake178: EZ_ResolveEntry()
/*
    wrapper for combat for lair
    rebuilds troop array
    equivalent of Move_Units() |-> Combat() for Stack/City
    hard-coded with Neutral Player as Defender
*/
int16_t Lair_Combat_Do(int16_t lair_idx, int16_t player_idx)
{
    int16_t troops[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t troop_count = 0;
    int16_t winner = 0;

    Player_Army_At_Square(_LAIRS[lair_idx].wx, _LAIRS[lair_idx].wy, _LAIRS[lair_idx].wp, player_idx, &troop_count, &troops[0]);

    winner = Combat__WIP(player_idx, NEUTRAL_PLAYER_IDX, troop_count, &troops[0]);

    return winner;
}


// WZD s96p04
// drake178: sub_7DE08()

// WZD s96p05
// drake178: WIZ_SetHostile()
void Update_Defender_Hostility(int attacker_player_idx, int defender_player_idx)
{

    _players[defender_player_idx].Hostility[attacker_player_idx] = 2;

    if(_players[defender_player_idx].Dipl.Dipl_Status[attacker_player_idx] >= 3)  // DIPL_War
    {
        _players[defender_player_idx].Hostility[attacker_player_idx] = 3;
    }

    if(_players[defender_player_idx].Dipl.Dipl_Status[attacker_player_idx] == 2)  // DIPL_Alliance
    {
        _players[defender_player_idx].Hostility[attacker_player_idx] = 0;
    }

    _players[defender_player_idx].reevaluate_hostility_countdown = (15+ Random(10));

}


// WZD s96p06
// UU_IDK_Main_Screen_Draw()




/*
    WIZARDS.EXE  ovr098
*/


// WZD o98p01
// drake178: CMB_CE_Refresh()
/*
; refreshes the combat enchantment display arrays from
; the Combat_Enchants@ allocation for both players
*/
/*

special treatment for 'Counter Magic'

*/
void CMB_CE_Refresh__WIP(void)
{
    int16_t Active = 0;
    int16_t itr = 0;  // _SI_
    int16_t idx = 0;  // _DI_

    attacker_enchantment_count = 0;
    defender_enchantment_count = 0;

    for(itr = 0; itr < (NUM_COMBAT_ENCHANTMENTS * 2); itr++)
    {

        Active = ST_FALSE;

        if(combat_enchantments[itr] == 1)
        {

            Active = ST_TRUE;

        }

        if(
            (itr == 20)  /* Counter Magic - Defender */
            ||
            (itr == 21)  /* Counter Magic - Attacker */
        )
        {

            if(combat_enchantments[itr] > 0)
            {

                Active = ST_TRUE;

            }
            else
            {

                Active = ST_FALSE;

            }

        }

        if(Active == ST_TRUE)
        {

            // enchantment index
            idx = (itr / 2);  /* { 0, 1, 2, ..., 27, 28, 29}  { 0, 0, 1, ..., 13, 14, 14 } */

            // even/odd - attacker/defender
            if((itr % 2) == 0)
            {

                CMB_ATKR_CE_Wnd[attacker_enchantment_count].icon_seg = combat_enchantment_icon_segs[combat_enchantment_icon_data[idx].icon_idx];

                CMB_ATKR_CE_Wnd[attacker_enchantment_count].help_idx = combat_enchantment_icon_data[idx].help_idx;

                attacker_enchantment_count++;

            }
            else
            {

                CMB_DEFR_CE_Wnd[defender_enchantment_count].icon_seg = combat_enchantment_icon_segs[combat_enchantment_icon_data[idx].icon_idx];

                CMB_DEFR_CE_Wnd[defender_enchantment_count].help_idx = combat_enchantment_icon_data[idx].help_idx;

                defender_enchantment_count++;

            }

        }

    }

}


// WZD o98p02
// drake178: BU_HasSpellAbility()
int16_t BU_HasSpellAbility__WIP(int16_t battle_unit_idx)
{

    /* HACK */  return ST_FALSE;

}


// WZD o98p03
// drake178: sub_7EC81()
// NIU  int16_t NIU_Who_Has_More_Leadership(void)
// NIU  {
// NIU      if(CMB_LeadershipArray[_combat_attacker_player] > CMB_LeadershipArray[_combat_defender_player])
// NIU      {
// NIU          return _combat_attacker_player;
// NIU      }
// NIU      else
// NIU      {
// NIU          return _combat_defender_player;
// NIU      }
// NIU  }

// WZD o98p04
// drake178: AI_CMB_PlayTurn()
/*
*/
/*

*/
void AI_CMB_PlayTurn__WIP(int16_t player_idx)
{
    int16_t Combat_Winner = 0;

    if(player_idx == combat_human_player)
    {

// TODO  G_CMB_CastSpell((player_idx + 20), _combat_wx, _combat_wy, _combat_wp);
// ; resolves combat spellcasting by the specified entity,
// ; from opening the spellbook to casting the spell; or
// ; evaluating the castable list, selecting one, and
// ; casting it in case of the AI
// ; returns 0 if no spell can be cast, 1 if cancelled
// ; during targeting, or 2 if resolved (or countered)
// ;
// ; contains many BUGs related to casting costs, and
// ; inherints many more from the targeting-, spell list
// ; building-, and spell effect functions

    }

    Combat_Winner = Check_For_Winner__WIP();  // ¿ because spell cast may resulted in a win/loss ?

    if(Combat_Winner == ST_UNDEFINED)
    {

        CMB_CE_Refresh__WIP();  // ¿ because spell cast may been an enchantment ?

        AI_MoveBattleUnits__WIP(player_idx);

    }

}


// WZD o98p05
// drake178: CMB_ProgressTurnFlow()
/*
; plays out an AI turn either before or after
; processing beginning of turn events and, if automatic
; combat is turned on, also plays a turn for the human
; player
*/
/*

*/
void CMB_ProgressTurnFlow__WIP(void)
{
    int16_t Winner = 0;

    _scanned_battle_unit = ST_UNDEFINED;

    frame_active_flag = 0;

    frame_scanned_flag = 0;

    Set_Mouse_List(1, mouse_list_default);

    CMB_HumanTurn = ST_FALSE;

    if(CMB_AIGoesFirst == ST_FALSE)
    {

        frame_active_flag = 0;

        frame_scanned_flag = 0;

        AI_CMB_PlayTurn__WIP(combat_computer_player);

    }

    Winner =  Check_For_Winner__WIP();

    if(Winner == ST_UNDEFINED)
    {

        CMB_PrepareTurn__WIP();

        if(_combat_defender_player == combat_computer_player)
        {

            AI_CMB_PlayTurn__WIP(_combat_defender_player);

            CMB_AIGoesFirst = ST_TRUE;

        }

        CMB_HumanTurn = ST_TRUE;

        if(_auto_combat_flag == ST_TRUE)
        {

            AI_CMB_PlayTurn__WIP(combat_human_player);

        }

    }

}


// WZD o98p06
/*
    OON XREF:  End_Of_Combat()
    sets Battle Unit to *dead* for *unsummoned* Units (_UNITS[].wp == 9)

*/
void Retreat_From_Combat(int16_t player_idx)
{
    int16_t Fleeing_Units_Lost[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Wind_Walker = 0;
    int16_t Transport_Capacity = 0;
    int16_t Fleeing_Death_Count = 0;
    int16_t unit_idx =0 ;
    int16_t Checked_X = 0;  // also, boat_riders
    int16_t boat_riders = 0;  // in Dasm, `Checked_X`
    int16_t Diameter = 0;
    int16_t Min_Y = 0;
    int16_t Min_X = 0;
    int16_t Checked_Y = 0;
    int16_t Defender_Fleeing_Count = 0;
    int16_t Attacker_Fleeing_Count = 0;
    int16_t itr_battle_units = 0;  // _SI_
    int16_t fleeing_player_idx = 0;  // _DI_

    Fleeing_Death_Count = 0;

    if(player_idx != _combat_attacker_player)
    {
        fleeing_player_idx = _combat_attacker_player;
    }
    else
    {
        fleeing_player_idx = _combat_defender_player;
    }

    /*
        BEGIN:  Fleeing Battle Unit Counts
    */
    Attacker_Fleeing_Count = 0;
    Defender_Fleeing_Count = 0;

    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
    {
        if(_UNITS[battle_units[itr_battle_units].unit_idx].wp == 9)
        {
            battle_units[itr_battle_units].status = bus_Dead;
        }

        if(battle_units[itr_battle_units].status == bus_Fleeing)
        {
            if(battle_units[itr_battle_units].controller_idx == _combat_attacker_player)
            {
                Attacker_Fleeing_Count++;
            }
            else if(battle_units[itr_battle_units].controller_idx == _combat_attacker_player)
            {
                Defender_Fleeing_Count++;
            }
        }

    }
    /*
        END:  Fleeing Battle Unit Counts
    */


    /*
        BEGIN:  
    */

    if(
        (Attacker_Fleeing_Count != 0)
        ||
        (Defender_Fleeing_Count != 0)
    )
    {
// mark all non-involved units as fleeing
// INCONSISTENT: these units have already been marked
// as dead in the parent function, so there will never be any
        if(battle_units[itr_battle_units].status == bus_Uninvolved)
        {
            battle_units[itr_battle_units].status = bus_Fleeing;
        }
    }

    /*
        END:  
    */


    /*
        BEGIN:  
    */
    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
    {
        if(battle_units[itr_battle_units].status == bus_Fleeing)
        {
            unit_idx = battle_units[itr_battle_units].unit_idx;
            _UNITS[unit_idx].wx = OVL_Action_OriginX;
            _UNITS[unit_idx].wy = OVL_Action_OriginY;
            battle_units[itr_battle_units].status = bus_Active;

        }

    }
    /*
        END:  
    */


    /*
        BEGIN:  
    */
// process fleeing chance - 0% if the unit is under the
// effect of Confusion, Black Sleep, or Web, 100% chance
// if its a human hero on Intro or Easy, 75% if it's a
// hero otherwise, and 50% if it's any other unit
// (success means the unit is marked as active instead)
// clear all road building progress for fleeing units
    if(fleeing_player_idx == _combat_attacker_player)
    {
        if(battle_units[itr_battle_units].status == bus_Fleeing)
        {
            if(_UNITS[battle_units[itr_battle_units].unit_idx].Rd_Constr_Left > -1)
            {
                _UNITS[battle_units[itr_battle_units].unit_idx].Status = us_Ready;
                _UNITS[battle_units[itr_battle_units].unit_idx].Rd_Constr_Left = -1;
            }

            if(
                (_difficulty <= god_Easy)
                &&
                (fleeing_player_idx == HUMAN_PLAYER_IDX)
            )
            {
                battle_units[itr_battle_units].status = bus_Active;
            }
            else
            {
                if(_UNITS[battle_units[itr_battle_units].unit_idx].Hero_Slot == -1)
                {
                    if(Random(2) == 1)
                    {
                        battle_units[itr_battle_units].status = bus_Active;
                    }
                }
                else
                {
                    if(Random(4) != 1)
                    {
                        battle_units[itr_battle_units].status = bus_Active;
                    }
                }
            }

            if(
                ((battle_units[itr_battle_units].Combat_Effects & bue_Black_Sleep) != 0)
                ||
                ((battle_units[itr_battle_units].Combat_Effects & bue_Confusion) != 0)
                ||
                (
                    ((battle_units[itr_battle_units].Combat_Effects & bue_Web) != 0)
                    &&
                    (battle_units[itr_battle_units].Web_HP > 0)
                )
            )
            {
                battle_units[itr_battle_units].status = bus_Fleeing;
            }

        }


        /*
            BEGIN:  Ocean Combat
        */
        if(Square_Is_Sailable(_combat_wx, _combat_wy, _combat_wp) == ST_TRUE)
        {

    // calculate the transport capacity of the player's surviving units
            Transport_Capacity = 0;

            for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
            {
                if(
                    (battle_units[itr_battle_units].status == bus_Active)
                    &&
                    (battle_units[itr_battle_units].controller_idx == fleeing_player_idx)
                    &&
                    (battle_units[itr_battle_units].carry_capacity > 0)
                )
                {
                    Transport_Capacity += battle_units[itr_battle_units].carry_capacity;
                }
            }


            if(Transport_Capacity == 0)
            {

    // check if there's a wind walker among the surviving units
                Wind_Walker = ST_FALSE;

                for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
                {

                    if(
                        (battle_units[itr_battle_units].status == bus_Active)
                        &&
                        (battle_units[itr_battle_units].controller_idx == fleeing_player_idx)
                    )
                    {
                        unit_idx = battle_units[itr_battle_units].unit_idx;

                        if(Unit_Has_WindWalking(unit_idx) == ST_TRUE)
                        {
                            Wind_Walker = ST_TRUE;
                        }
                    }
                }

                if(Wind_Walker != ST_TRUE)
                {
    // mark all active units of the losing player as fleeing
    // if they don't have seafaring capability
    // BUG: ignores Non-Corporeal units
                    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
                    {
                        if(
                            (battle_units[itr_battle_units].status == bus_Active)
                            &&
                            (battle_units[itr_battle_units].controller_idx == fleeing_player_idx)
                        )
                        {
                            unit_idx = battle_units[itr_battle_units].unit_idx;

                            if(
                                (Unit_Has_AirTravel(unit_idx) == ST_TRUE)
                                ||
                                (Unit_Has_WaterTravel(unit_idx) == ST_TRUE)  /* BUG:  ¿ should be Unit_Has_Swimming() ? SEE BELOW */
                                ||
                                (Unit_Has_AirTravel_Item(unit_idx) == ST_TRUE)
                                ||
                                (Unit_Has_WaterTravel_Item(unit_idx) == ST_TRUE)
                            )
                            {
                                battle_units[itr_battle_units].status = bus_Active;  // BUG:  ¿ this check for filtered status is due to macro usage ?
                            }
                            else
                            {
                                battle_units[itr_battle_units].status = bus_Fleeing;
                            }
                        }
                    }
                }

            }
            else  /* if(Transport_Capacity == 0) */
            {
    // check if there's a wind walker among the surviving units
                Wind_Walker = ST_FALSE;

                for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
                {

                    if(
                        (battle_units[itr_battle_units].status == bus_Active)
                        &&
                        (battle_units[itr_battle_units].controller_idx == fleeing_player_idx)
                    )
                    {
                        unit_idx = battle_units[itr_battle_units].unit_idx;

                        if(Unit_Has_WindWalking(unit_idx) == ST_TRUE)
                        {
                            Wind_Walker = ST_TRUE;
                        }
                    }
                }

            }

    // process transport capacity, marking any active unit
    // that doesn't fit as fleeing again
    // BUG: the checks don't cover all seafaring ability

            if(Wind_Walker != ST_TRUE)
            {
                boat_riders = 0;

                for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
                {

                    if(
                        (battle_units[itr_battle_units].status == bus_Active)
                        &&
                        (battle_units[itr_battle_units].controller_idx == fleeing_player_idx)
                        &&
                        (battle_units[itr_battle_units].carry_capacity < 1)
                    )
                    {
                        if(_UNITS[battle_units[itr_battle_units].unit_idx].type > ut_Chosen)
                        {
                            if(
                                (Unit_Has_AirTravel_Item(battle_units[itr_battle_units].unit_idx) != ST_TRUE)
                                &&
                                (Unit_Has_WaterTravel_Item(battle_units[itr_battle_units].unit_idx) != ST_TRUE)
                                &&
                                (Unit_Has_Swimming(battle_units[itr_battle_units].unit_idx) != ST_TRUE)  /* BUG:  ¿ should be Unit_Has_WaterTravel() ? SEE ABOVE */
                                &&
                                (Unit_Has_WindWalking(battle_units[itr_battle_units].unit_idx) != ST_TRUE)  /* BUG:  only got here because there were no WindWalkers */
                                &&
                                (Unit_Has_AirTravel(battle_units[itr_battle_units].unit_idx) != ST_TRUE)
                            )
                            {
                                boat_riders++;

                                if(boat_riders > Transport_Capacity)
                                {
                                    battle_units[itr_battle_units].status = bus_Fleeing;
                                }
                            }
                        }
                    }
                }
            }

        }

        /*
            END:  Ocean Combat
        */


    // add all fleeing units of the losing player to the list of lost units
        for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
        {
            if(
                (battle_units[itr_battle_units].status == bus_Active)
                &&
                (battle_units[itr_battle_units].controller_idx == fleeing_player_idx)
            )
            {
                Fleeing_Units_Lost[Fleeing_Death_Count] = _UNITS[battle_units[itr_battle_units].unit_idx].type;

                Fleeing_Death_Count++;
            }
        }

        STK_ComposeFleeLost__STUB(Fleeing_Death_Count, Fleeing_Units_Lost);

    }
    else  /* if(fleeing_player_idx == _combat_attacker_player) */
    {
// process fleeing chance - 0% if the unit is under the
// effect of Confusion, Black Sleep, or Web, 100% chance
// if its a human hero on Intro or Easy, 75% if it's a
// hero otherwise, and 50% if it's any other unit
// (failure means the unit is marked as dead instead)
// clear all road building progress for fleeing units,
// and store a list of the lost units
// BUG? has an extra block for non-human units that
// set cancels road building and marks the unit ready overland

        for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
        {

            if(battle_units[itr_battle_units].status == bus_Fleeing)
            {

                if(fleeing_player_idx != _human_player_idx)
                {
                    _UNITS[battle_units[itr_battle_units].unit_idx].Status = us_Ready;
                    _UNITS[battle_units[itr_battle_units].unit_idx].Rd_Constr_Left = -1;
                }

                if(_UNITS[battle_units[itr_battle_units].unit_idx].Rd_Constr_Left > -1)
                {
                    _UNITS[battle_units[itr_battle_units].unit_idx].Status = us_Ready;
                    _UNITS[battle_units[itr_battle_units].unit_idx].Rd_Constr_Left = -1;
                }

                if(
                    (_difficulty > god_Easy)
                    ||
                    (fleeing_player_idx != HUMAN_PLAYER_IDX)
                )
                {
                    if(_UNITS[battle_units[itr_battle_units].unit_idx].Hero_Slot == -1)
                    {
                        if(Random(2) == 1)
                        {
                            battle_units[itr_battle_units].status = bus_Dead;
                        }
                    }
                    else
                    {
                        if(Random(4) == 1)
                        {
                            battle_units[itr_battle_units].status = bus_Dead;
                        }
                    }
                }

                if(
                    ((battle_units[itr_battle_units].Combat_Effects & bue_Black_Sleep) != 0)
                    ||
                    ((battle_units[itr_battle_units].Combat_Effects & bue_Confusion) != 0)
                    ||
                    (
                        ((battle_units[itr_battle_units].Combat_Effects & bue_Web) != 0)
                        &&
                        (battle_units[itr_battle_units].Web_HP > 0)
                    )
                )
                {
                    battle_units[itr_battle_units].status = bus_Dead;
                }

                if(battle_units[itr_battle_units].status == bus_Dead)
                {
                    Fleeing_Units_Lost[Fleeing_Death_Count] = _UNITS[battle_units[itr_battle_units].unit_idx].type;

                    Fleeing_Death_Count++;
                }

            }

        }


        /*
            BEGIN:  
        */

        // drake178:  ; BUG: this range check must be performed on the tile to be tested - here, it moves the entire area
        Min_Y = (_combat_wy - 1);

        SETMIN(Min_Y, 0);

        Min_X = (_combat_wx - 1);

        if(Min_X < 0)
        {
            Min_X += WORLD_WIDTH;
        }

// attempt to flee as many units as possible to the adjacent tiles
// BUG: the tile range checking allows 2-move flight if the origin X or Y is 0
// BUG: inherits a number of tile eligibility errors
        Diameter = 3;
        Checked_Y = Min_Y;
        while(((Min_Y + Diameter) > Checked_Y) && (Checked_Y < WORLD_HEIGHT))
        {
            itr_battle_units = Min_X;
            while((Min_X + Diameter) > itr_battle_units)
            {
                if(itr_battle_units < WORLD_WIDTH)
                {
                    Checked_X = itr_battle_units;
                }
                else
                {
                    Checked_X = (itr_battle_units - WORLD_WIDTH);
                }

                if(
                    (Checked_X != _combat_wx)
                    &&
                    (Checked_Y != _combat_wy)
                )
                {
                    if(
                        (Square_Has_Lair(Checked_X, Checked_Y, _combat_wp) == ST_UNDEFINED)
                        &&
                        (Player_City_At_Square(Checked_X, Checked_Y, _combat_wp, fleeing_player_idx) == ST_UNDEFINED)
                    )
                    {
                        Process_Retreating_Units(Checked_X, Checked_Y, _combat_wp, fleeing_player_idx);
                    }
                }

                itr_battle_units++;
            }

            Checked_Y++;
        }

        /*
            END:  
        */

// mark all fleeing units of the losing player as dead,
// and add them to the list of lost units
        for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
        {
            if(
                (battle_units[itr_battle_units].status == bus_Fleeing)
                &&
                (battle_units[itr_battle_units].controller_idx == fleeing_player_idx)
            )
            {
                battle_units[itr_battle_units].status = bus_Dead;

                Fleeing_Units_Lost[Fleeing_Death_Count] = _UNITS[battle_units[itr_battle_units].unit_idx].type;

                Fleeing_Death_Count++;
            }
        }

        STK_ComposeFleeLost__STUB(Fleeing_Death_Count, Fleeing_Units_Lost);

    }  /* else ... if(fleeing_player_idx == _combat_attacker_player) */
    /*
        END:  
    */


    // WTF, Mate?
    // @@JmpDone__Return_ZERO:
    // xor     ax, ax
    // jmp     @@Done
    // NOTE: jumps straight to here from way up at ```if((Attacker_Fleeing_Count != 0) || (Defender_Fleeing_Count != 0))```

}


// WZD o98p07
// drake178: TILE_HasCityOfPlayer()
int16_t Player_City_At_Square(int16_t wx, int16_t wy, int16_t wp, int16_t player_idx)
{
    int16_t city_idx;  // _SI_
    int16_t itr_cities;  // _CX_

    city_idx = -1;

    for(itr_cities = 0; ((itr_cities < _cities) && (city_idx == -1)); itr_cities++)
    {
        if(
            (_CITIES[itr_cities].wp == wp) &&
            (_CITIES[itr_cities].owner_idx == player_idx) &&
            (_CITIES[itr_cities].wy == wy) &&
            (_CITIES[itr_cities].wx == wx)
        )
        {
            city_idx = itr_cities;
        }
    }

    return city_idx;
}


// WZD o98p08
/*
    combat / battle unit
    calls Unit_Try_To_Move()
    returns F,T - successfully fleed
    OON XREF: WIZ_FleeCombat()
¿ MoO2  Module: COMBFIND  Process_Retreating_Ships_() ?
*/
int16_t Process_Retreating_Units(int16_t wx, int16_t wy, int16_t wp, int16_t player_idx)
{
    int16_t troop_list[9];
    int16_t Landlubber_Count;
    int16_t Transport_Capacity;
    int16_t Wind_Walker;
    int16_t Move_Possible;
    int16_t troop_count;
    int16_t itr_towers;  // _SI_
    int16_t itr_battle_units;  // _SI_
    int16_t itr_troops;  // _SI_
    int16_t unit_idx;  // _DI_
    int16_t return_value;  // DNE in Dasm

    Army_At_Square_2(wx, wy, wp, &troop_count, &troop_list[0]);

    if(
        (troop_count <= 0)
        ||
        (
            (_UNITS[troop_list[0]].owner_idx == player_idx)
            &&
            (troop_count < MAX_STACK)
        )
    )
    {

        Wind_Walker = ST_FALSE;

        for(itr_towers = 0; ((itr_towers < NUM_TOWERS) && (Wind_Walker == ST_FALSE)); itr_towers++)
        {
            if(
                (_TOWERS[itr_towers].wx == wx)
                &&
                (_TOWERS[itr_towers].wy == wy)
            )
            {
                goto Return_FALSE;
            }
        }

        Move_Possible = ST_FALSE;

        // Ocean Combat
        if(Square_Is_Sailable(wx, wy, wp) == ST_TRUE)
        {
            Wind_Walker = ST_FALSE;

            // check for Wind Walker on source square
            for(itr_battle_units = 0; ((itr_battle_units < _combat_total_unit_count) && (Wind_Walker == ST_FALSE)); itr_battle_units++)
            {
                if(
                    (battle_units[itr_battle_units].status == bus_Fleeing)
                    &&
                    (battle_units[itr_battle_units].controller_idx == player_idx)
                )
                {
                    unit_idx = battle_units[itr_battle_units].unit_idx;

                    if(unit_idx != -1)
                    {
                        if(Unit_Has_WindWalking(unit_idx) == ST_TRUE)
                        {
                            Move_Possible = ST_TRUE;
                            Wind_Walker = ST_TRUE;
                        }
                    }
                }
            }

            // check for Wind Walker on destination square
            if(Wind_Walker == ST_FALSE)
            {
                for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
                {
                    unit_idx = troop_list[itr_troops];

                    if(unit_idx != -1)
                    {
                        if(Unit_Has_WindWalking(unit_idx) == ST_TRUE)
                        {
                            Move_Possible = ST_TRUE;
                            Wind_Walker = ST_TRUE;
                        }
                    }
                }
            }

            if(Move_Possible == ST_FALSE)
            {
                Transport_Capacity = 0;

                if(
                    (battle_units[itr_battle_units].status == bus_Fleeing)
                    &&
                    (battle_units[itr_battle_units].controller_idx == player_idx)
                )
                {
                    unit_idx = battle_units[itr_battle_units].unit_idx;

                    if(_unit_type_table[_UNITS[unit_idx].type].Transport > 0)
                    {
                        Transport_Capacity += _unit_type_table[_UNITS[unit_idx].type].Transport;
                    }
                }

                Wind_Walker = ST_FALSE;

                Landlubber_Count = 0;

                for(itr_battle_units = 0; ((itr_battle_units < _combat_total_unit_count) && (Wind_Walker == ST_FALSE)); itr_battle_units++)
                {
                    if(
                        (battle_units[itr_battle_units].status == bus_Fleeing)
                        &&
                        (battle_units[itr_battle_units].controller_idx == player_idx)
                    )
                    {
                        unit_idx = battle_units[itr_battle_units].unit_idx;

                        if(
                            (Unit_Has_AirTravel(unit_idx) == ST_TRUE)
                            ||
                            (Unit_Has_WaterTravel(unit_idx) == ST_TRUE)
                            ||
                            (Unit_Has_AirTravel_Item(unit_idx) == ST_TRUE)
                            ||
                            (Unit_Has_WaterTravel_Item(unit_idx) == ST_TRUE)
                        )
                        {

                            if(Unit_Try_To_Move(wx, wy, wp, unit_idx, troop_count) == ST_TRUE)
                            {
                                troop_count++;

                                battle_units[itr_battle_units].status = bus_Active;
                            }
                        }
                        else
                        {
                            Landlubber_Count++;

                            if(Landlubber_Count < Transport_Capacity)
                            {
                                if(Unit_Try_To_Move(wx, wy, wp, unit_idx, troop_count) == ST_TRUE)
                                {
                                    troop_count++;

                                    battle_units[itr_battle_units].status = bus_Active;
                                }   
                            }
                        }
                    }
                }
            }  /* if(Move_Possible == ST_FALSE) */

        }
        else  /* if(Square_Is_Sailable(wx, wy, wp) == ST_TRUE) */
        {
// abort if any of the fleeing units has sailing movement
// BUG: flying and Non-Corporeal ships can move on land

            for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
            {
                if(
                    (battle_units[itr_battle_units].status == bus_Fleeing)
                    &&
                    (battle_units[itr_battle_units].controller_idx == player_idx)
                )
                {
                    unit_idx = battle_units[itr_battle_units].unit_idx;

                    if(Unit_Has_Sailing(unit_idx) == ST_TRUE)
                    {
                        goto Return_FALSE;
                    }
                }
            }

            Move_Possible = ST_TRUE;
        }


        if(Move_Possible == ST_TRUE)
        {
// move as many fleeing units over to the tile as possible,
// marking them as active if successful
// BUG: there is no guarantee that the wind walker is actually moved over

            for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
            {
                if(
                    (battle_units[itr_battle_units].status == bus_Fleeing)
                    &&
                    (battle_units[itr_battle_units].controller_idx == player_idx)
                )
                {
                    unit_idx = battle_units[itr_battle_units].unit_idx;

                    if(Unit_Try_To_Move(wx, wy, wp, unit_idx, troop_count) == ST_TRUE)
                    {
                        troop_count++;

                        battle_units[itr_battle_units].status = bus_Active;
                    }   
                }
            }
        }

        goto Return_TRUE;
    }


Return_FALSE:
    return_value = ST_FALSE;
    goto Done;

Return_TRUE:
    return_value = ST_TRUE;
    goto Done;

Done:
    return return_value;

}


// WZD o98p09
/*
    OON XREF:  TILE_FleeTo()
    moves the unit if there is space
    returns F,T - did the move
*/
int16_t Unit_Try_To_Move(int16_t wx, int16_t wy, int16_t wp, int16_t unit_idx, int16_t troop_count)
{
    int16_t troop_space;
    int16_t return_value;  // DNE in Dasm

    troop_space = (MAX_STACK - troop_count);

    if(troop_space < 1)
    {
        return_value = ST_FALSE;
    }
    else
    {
        _UNITS[unit_idx].wx = wx;
        _UNITS[unit_idx].wy = wy;
        _UNITS[unit_idx].wp = wp;

        return_value = ST_TRUE;
    }

    return return_value;
}


// WZD o98p10
// drake178: CMB_SetActionCursor()
// MoO2  Module:  COMBAT1  Assign_Mouse_Images_()
/*
; sets the cursor based on the currently selected unit
; and the contents of the space that the mouse is over
;
; has multiple BUGs regarding the melee and ranged
; cursors
*/
/*

finger on own unit
winged boot on reachable move
red x on unreachable
crossed swords on reachable attack

*/
void Assign_Mouse_Image(void)
{
    int16_t ranged_attack_type_group = 0;
    int16_t cgy = 0;
    int16_t screen_y = 0;
    int16_t screen_x = 0;
    int16_t Y_Distance = 0;
    int16_t X_Distance = 0;
    int16_t scanned_battle_unit_idx = 0;  // _SI_
    int16_t cgx = 0;  // _DI_

    _scanned_battle_unit = ST_UNDEFINED;

    frame_active_flag = ST_FALSE;

    if(_active_battle_unit > ST_UNDEFINED)
    {

        frame_active_flag = ST_TRUE;

        frame_active_cgx = battle_units[_active_battle_unit].cgx;

        frame_active_cgy = battle_units[_active_battle_unit].cgy;

        if(
            (CMB_ImmobileCanAct == ST_TRUE)
            &&
            (_auto_combat_flag == ST_FALSE)
        )
        {

            if(battle_units[_active_battle_unit].controller_idx == combat_human_player)
            {
                
                frame_active_flag = ST_FALSE;

            }

        }

    }

    _combat_mouse_grid->image_num = crsr_Finger;

    screen_x = (Pointer_X() + 4);  // ¿ why the (+ 4) ?

    screen_y = (Pointer_Y() + 4);  // ¿ why the (+ 4) ?

    frame_scanned_flag = ST_FALSE;

    if(screen_y < 168)
    {

        cgx = Get_Combat_Grid_Cell_X(screen_x, screen_y);

        cgy = Get_Combat_Grid_Cell_Y(screen_x, screen_y);

        if(CMB_TargetRows[cgy][cgx] == -2)
        {

            _combat_mouse_grid->image_num = crsr_RedCross;  /* Page 92  (PDF Page 97) "Squares that are not valid as targets for an active unit’s actions show a red "X" when the mouse cursor moves over them." */

        }
        else if(CMB_TargetRows[cgy][cgx] == -1)
        {

            frame_scanned_flag = ST_TRUE;

            frame_scanned_cgx = cgx;

            frame_scanned_cgy = cgy;

            _combat_mouse_grid->image_num = crsr_WingedBoot;

        }
        else if(CMB_TargetRows[cgy][cgx] == 99)
        {

            frame_scanned_flag = ST_TRUE;

            frame_scanned_cgx = cgx;

            frame_scanned_cgy = cgy;

            frame_anim_cycle = ((frame_anim_cycle + 1) % 3);

            scanned_battle_unit_idx = CMB_TargetRows[cgy][cgx];

            X_Distance = abs((cgx - battle_units[_active_battle_unit].cgx));

            Y_Distance = abs((cgy - battle_units[_active_battle_unit].cgy));

            _combat_mouse_grid->image_num = crsr_RedCross;

            if(battle_units[_active_battle_unit].movement_points > 0)
            {

                // ; BUG: ranged units will still make this as a ranged attack?
                if(
                    (X_Distance <= 1)
                    &&
                    (Y_Distance <= 1)
                )
                {

                    _combat_mouse_grid->image_num = crsr_Melee;

                }
                else
                {

                    ranged_attack_type_group = (battle_units[_active_battle_unit].ranged_type / 10);

                    if(
                        (ranged_attack_type_group == rag_Boulder)
                        ||
                        (ranged_attack_type_group == rag_Missile)
                        ||
                        (ranged_attack_type_group == rag_Magic)
                    )
                    {

                        STU_DEBUG_BREAK();

                        _combat_mouse_grid->image_num = crsr_Ranged;

                    }

                }

            }

        }
        else  /* not -2, -1, 99 */
        {

            frame_scanned_flag = ST_TRUE;

            frame_scanned_cgx = cgx;

            frame_scanned_cgy = cgy;

            frame_anim_cycle = ((frame_anim_cycle + 1) % 3);

            scanned_battle_unit_idx = CMB_TargetRows[cgy][cgx];

            _scanned_battle_unit = scanned_battle_unit_idx;  // ; the combat unit display is based on this

            if(
                (CMB_ImmobileCanAct != ST_TRUE)
                ||
                (_auto_combat_flag != ST_FALSE)
            )
            {

                if(battle_units[scanned_battle_unit_idx].controller_idx != _human_player_idx)
                {

                    X_Distance = abs(battle_units[scanned_battle_unit_idx].cgx - battle_units[_active_battle_unit].cgx);

                    Y_Distance = abs(battle_units[scanned_battle_unit_idx].cgy - battle_units[_active_battle_unit].cgy);

                    _combat_mouse_grid->image_num = crsr_RedCross;

                    if(battle_units[_active_battle_unit].movement_points > 0)
                    {

                        if(Check_Attack_Melee(_active_battle_unit, scanned_battle_unit_idx) == ST_TRUE)
                        {

                            if(
                                (X_Distance <= 1)
                                &&
                                (Y_Distance <= 1)
                            )
                            {

                                if(BU_MeleeWallCheck__WIP(_active_battle_unit, scanned_battle_unit_idx) == ST_TRUE)
                                {

                                    _combat_mouse_grid->image_num = crsr_Melee;

                                }
                                else
                                {

                                    _combat_mouse_grid->image_num = BU_GetRangedCursor__WIP(_active_battle_unit, scanned_battle_unit_idx);

                                }

                            }
                            else
                            {

                                _combat_mouse_grid->image_num = BU_GetRangedCursor__WIP(_active_battle_unit, scanned_battle_unit_idx);

                            }

                        }

                    }

                }

            }

        }

    }


    if(_auto_combat_flag == ST_TRUE)
    {
        
        frame_active_flag = ST_FALSE;

        frame_scanned_flag = ST_FALSE;

    }

    _combat_mouse_grid->center_offset = 0;
    _combat_mouse_grid->x1 = SCREEN_XMIN;
    _combat_mouse_grid->y1 = SCREEN_YMIN;
    _combat_mouse_grid->x2 = SCREEN_XMAX;
    _combat_mouse_grid->y2 = SCREEN_YMAX;

    Set_Mouse_List(1, _combat_mouse_grid);

}


// WZD o98p11
// drake178: BU_GetRangedCursor()
/*
; returns either the red cross (3) or ranged attack (4)
; cursor index depending on whether the selected unit
; can attack the target based on its visibility
;
; BUG: returns the ranged cursor if any of the
; attacker's units can see through invisibility,
; ignoring the unit's own ability, and always using the
; attacking player's variable
*/
/*

*/
int16_t BU_GetRangedCursor__WIP(int16_t src_battle_unit_idx, int16_t dst_battle_unit_idx)
{

    /* HACK */  return crsr_Ranged;

}


// WZD o98p12
// drake178: CMB_CreateCELabels()
/*
; creates up to 8 click labels for the combat
; enchantments currently displayed in the left and
; right enchantment windows, storing their indices into
; the corresponding global arrays
*/
/*

*/
void Add_Combat_Enchantment_Fields(void)
{
    int16_t start_x = 0;  // _DI_
    int16_t itr = 0;  // _SI_

    if(_combat_attacker_player == _human_player_idx)
    {

        start_x = 247;

    }
    else
    {

        start_x = 8;

    }

    for(itr = 0; (((CMB_ATKR_First_CE + itr)  < attacker_enchantment_count) && (itr < 4)); itr++)
    {

        attacker_enchantment_fields[itr] = Add_Picture_Field((start_x + (itr * 17)), 179, CMB_ATKR_CE_Wnd[(CMB_ATKR_First_CE + itr)].icon_seg, str_empty_string__ovr098[0], CMB_ATKR_CE_Wnd[(CMB_ATKR_First_CE + itr)].help_idx);

    }


    if(_combat_defender_player == _human_player_idx)
    {

        start_x = 247;

    }
    else
    {

        start_x = 8;

    }

    for(itr = 0; (((CMB_DEFR_First_CE + itr)  < defender_enchantment_count) && (itr < 4)); itr++)
    {

        defender_enchantment_fields[itr] = Add_Picture_Field((start_x + (itr * 17)), 179, CMB_DEFR_CE_Wnd[(CMB_DEFR_First_CE + itr)].icon_seg, str_empty_string__ovr098[0], CMB_DEFR_CE_Wnd[(CMB_DEFR_First_CE + itr)].help_idx);

    }

}


// WZD o98p13
// drake178: BU_GetCmbtMoveType()
/*
; returns the combat movement type of a battle unit:
;   1 - ground
;   2 - flight
;   4 - sailing
;   5 - swimming
;
; INCONSISTENT: forester and mountaineer will both
; shortcut the sailing check and default to ground
*/
/*

looks like some sort of macro based switch
with 4 macro blocks resolving to nothing?

*/
int16_t Battle_Unit_Movement_Icon(int16_t battle_unit_idx)
{
    int16_t icon_idx = 0;  // DNE in Dasm

    if((battle_units[battle_unit_idx].Move_Flags & MV_FLYING) != 0)
    {

        icon_idx = 2;

    }
    else if((battle_units[battle_unit_idx].Move_Flags & MV_SWIMMING) != 0)
    {

        icon_idx = 5;

    }
    else if((battle_units[battle_unit_idx].Move_Flags & MV_FORESTER) != 0)
    {

        icon_idx = 1;

    }
    else if((battle_units[battle_unit_idx].Move_Flags & MV_MOUNTAINEER) != 0)
    {

        icon_idx = 1;

    }
    else if((battle_units[battle_unit_idx].Move_Flags & MV_SAILING) != 0)
    {

        icon_idx = 4;

    }
    else
    {

        icon_idx = 1;

    }


    return icon_idx;
}


// segrax
// WZD o98p14
// drake178: UNIT_SummonToBattle()
/*
; creates a battle unit from the selected unit for the
; specified player at the passed location - if the
; battle unit table is full, will overwrite the first
; inactive unit with 9 set as its present Plane
; (typically combat summons and encounter zone guards)
; BUG: does not apply battlefield effects to the unit
; WARNING: will freeze up the game if all figure slots
; are occupied (already 18 units on the battlefield)
*/
/*

G_CMB_SpellEffect__WIP()
    UNIT_SummonToBattle__SEGRAX(player_idx, (_units - 1), target_cgx, target_cgy);
BU_SummonDemon__SEGRAX()
    BU_SummonDemon__SEGRAX(caster_idx);

BU_UnitLoadToBattle__SEGRAX()
    ~ CMB_Units_Init__WIP()  AKA Prepare_All_Battle_Units()
    OON XREF:  UNIT_SummonToBattle__SEGRAX()
    ...which is only used for scc_Summoning and USA 'Summon Demon'
*/
void UNIT_SummonToBattle__SEGRAX(int16_t player_idx, int16_t unit_idx, int16_t cgx, int16_t cgy)
{
    int16_t itr = 0;

    if(_combat_total_unit_count < MAX_BATTLE_UNIT_SLOT_COUNT)
    {
        BU_UnitLoadToBattle__SEGRAX(_combat_total_unit_count, player_idx, unit_idx, cgx, cgy);
        _combat_total_unit_count++;
    } 
    else
    {
        STU_DEBUG_BREAK();
        for(itr = 0; itr < _combat_total_unit_count; itr++)
        {
            struct s_BATTLE_UNIT * battle_unit = &battle_units[itr];
            if(battle_unit->status > bus_Active)
            {
                uint16_t unit_idx = battle_unit->unit_idx;
                struct s_UNIT * unit = &_UNITS[unit_idx];
                if(unit->wp == 9)
                {
                    BU_UnitLoadToBattle__SEGRAX(itr, player_idx, unit_idx, cgx, cgy);
                    return;
                }
            }
        }
    }
}

// segrax
// WZD o98p15
// drake178: BU_UnitLoadToBattle()
/*
; creates a battle unit from the specified unit, loaded
; at the selected index of the battle unit table, then
; finds an empty combat figure slot in the EMM FIGUREX
; handle, and loads the unit's figure sprites into it
; BUG: does not apply battlefield effects to the unit
; WARNING: will freeze up the game if all figure slots
; are occupied (already 18 units on the battlefield)
*/
/*

~ CMB_Units_Init__WIP()  AKA Prepare_All_Battle_Units()
OON XREF:  UNIT_SummonToBattle__SEGRAX()
...which is only used for scc_Summoning and USA 'Summon Demon'
*/
void BU_UnitLoadToBattle__SEGRAX(int16_t battle_unit_idx, int8_t player_idx, int16_t unit_idx, int16_t cgx, int16_t cgy)
{
    int16_t bufpi;
    struct s_BATTLE_UNIT* battle_unit = &battle_units[battle_unit_idx];

    Load_Battle_Unit(unit_idx, battle_unit);

    bufpi = Battle_Unit_Pict_Open();  // returns bufpi {0,...17}
    assert(bufpi >= 0);
    assert(bufpi < MAX_BATTLE_UNIT_COUNT);

    Combat_Figure_Load(_UNITS[unit_idx].type, bufpi);

    battle_unit->bufpi = bufpi;
    battle_unit->controller_idx = player_idx;

    // ~ Deploy_Battle_Units()
    battle_unit->cgx = cgx;
    battle_unit->cgy = cgy;
    battle_unit->target_cgx = cgx;
    battle_unit->target_cgy = cgy;

    // all exactly as just done in Load_Battle_Unit()
    battle_unit->MoveStage = 0;
    battle_unit->outline_magic_realm = 0;
    battle_unit->Atk_FigLoss = 0;
    battle_unit->Moving = 0;
    battle_unit->action = bua_Ready;

    // ¿ BUGBUG  should call BU_Apply_Battlefield_Effects__WIP() ?

}

// WZD o98p16
// drake178: CMB_Units_Init()
// MoO2  Module: COMBINIT  Deploy_Ships_()
/*
; creates battle unit structures for the passed stack
; as the attacking force, and the action target's units
; on the action tile as the defending army; maps and
; populates the EMM FIGUREX handle, and positions the
; armies on their starting locations
; returns the defender's unit count
*/
/*
    Load_Battle_Unit(); Combat_Figure_Load(); Deploy_Battle_Units();
    inits battle units, load figure picts, sets position & orientation

checks spl_Spell_Of_Return
sets _players[].Cmbt_Skill_Left

*/
int16_t CMB_Units_Init__WIP(int16_t troop_count, int16_t troops[])
{
    int16_t itr = 0;  // _SI_
    int16_t count = 0;  // _DI_

    for(itr = 0; itr < MAX_BATTLE_UNIT_SLOT_COUNT; itr++)
    {

        battle_units[itr].status = bus_Gone;

        battle_units[itr].bufpi = ST_UNDEFINED;

    }

    _combat_total_unit_count = 0;

    for(itr = 0; itr < troop_count; itr++)
    {

        Load_Battle_Unit(troops[itr], &battle_units[_combat_total_unit_count]);

        if(_UNITS[battle_units[_combat_total_unit_count].unit_idx].type == spell_data_table[spl_Floating_Island].unit_type)
        {

            ATKR_FloatingIsland = 1;

            battle_units[_combat_total_unit_count].status = bus_Uninvolved;

        }

        battle_units[_combat_total_unit_count].bufpi = Combat_Figure_Load(_UNITS[troops[itr]].type, itr);

        battle_units[_combat_total_unit_count].controller_idx = _combat_attacker_player;

        if(battle_units[_combat_total_unit_count].controller_idx != _human_player_idx)
        {

            _UNITS[battle_units[_combat_total_unit_count].unit_idx].Status = us_Ready;  // ; 'MoM Demo': NO ORDERS

            _UNITS[battle_units[_combat_total_unit_count].unit_idx].Rd_Constr_Left = ST_UNDEFINED;

        }

        _combat_total_unit_count++;

    }


    if(_combat_attacker_player != _human_player_idx)
    {

        CMB_AI_Player = _combat_attacker_player;

    }


    for(itr = 0; itr < _units; itr++)
    {

        if(
            (_UNITS[itr].wx == _combat_wx)
            &&
            (_UNITS[itr].wy == _combat_wy)
            &&
            (_UNITS[itr].wp == _combat_wp)
            &&
            (_UNITS[itr].owner_idx == _combat_defender_player)
        )
        {

            if(_UNITS[itr].owner_idx == HUMAN_PLAYER_IDX)
            {

                _UNITS[itr].Status = us_Ready;

            }

            Load_Battle_Unit(itr, &battle_units[_combat_total_unit_count]);

            if(_UNITS[battle_units[_combat_total_unit_count].unit_idx].type == spell_data_table[spl_Floating_Island].unit_type)
            {

                DEFR_FloatingIsland = ST_TRUE;

                battle_units[_combat_total_unit_count].status = bus_Uninvolved;

            }

            battle_units[_combat_total_unit_count].bufpi = Combat_Figure_Load(_UNITS[itr].type, _combat_total_unit_count);

            battle_units[_combat_total_unit_count].controller_idx = _combat_defender_player; // Why? Load_Battle_Unit() sets controller_idx = _UNITS[].owner_idx

            count++;

            _combat_total_unit_count++;

        }

    }

    if(_combat_defender_player == ST_UNDEFINED)
    {
        _combat_defender_player = MOO_MONSTER_PLAYER_IDX;  // ; THIS WILL CORRUPT MEMORY IF IT IS POSSIBLE AT ALL!
    }

    if(_combat_attacker_player == ST_UNDEFINED)
    {
        _combat_attacker_player = MOO_MONSTER_PLAYER_IDX;  // ; THIS WILL CORRUPT MEMORY IF IT IS POSSIBLE AT ALL!
    }

    if(_combat_defender_player != _human_player_idx)
    {
        
        CMB_AI_Player = _combat_defender_player;

    }

    if(CMB_AI_Player == ST_UNDEFINED)
    {
        CMB_AI_Player = MOO_MONSTER_PLAYER_IDX;  // ; THIS WILL CORRUPT MEMORY IF IT IS POSSIBLE AT ALL!
    }


    /*
        IDGI.
            attacker / defender
            current / opponent
    */
    if(_combat_attacker_player == _human_player_idx)
    {

        combat_human_player = _combat_attacker_player;

        combat_computer_player = _combat_defender_player;

    }
    else
    {

        combat_human_player = _combat_defender_player;

        combat_computer_player = _combat_attacker_player;

    }


    // reset all BU coordinates and action/AI fields
    // BUG: transport units with Wraith Form are excluded from defending cities
    for(itr = 0; itr < _combat_total_unit_count; itr++)
    {

        battle_units[itr].cgx = 0;
        battle_units[itr].cgy = 0;
        battle_units[itr].target_cgx = 0;
        battle_units[itr].target_cgy = 0;
        battle_units[itr].MoveStage = 0;
        battle_units[itr].outline_magic_realm = 0;
        battle_units[itr].Atk_FigLoss = 0;
        battle_units[itr].Moving = 0;
        battle_units[itr].action = bua_Ready;
        battle_units[itr].Blood_Amount = 0;
        battle_units[itr].Unknown_5A = 0;
        battle_units[itr].Always_Animate = 0;
        battle_units[itr].Melee_Anim = 0;
        battle_units[itr].Image_Effect = 0;
        battle_units[itr].Move_Bob = 0;

        if(OVL_Action_Type == 1)  /* Enemy City */
        {

            // ; BUG: excludes ships with Wraith Form (the comparison value should also include swimming)
            if(
                (_unit_type_table[_UNITS[battle_units[itr].unit_idx].type].Transport > 0)
                &&
                ((battle_units[itr].Move_Flags & MV_SAILING) != 0)
                &&
                ((battle_units[itr].Move_Flags & MV_FLYING) == 0)
            )
            {

                battle_units[itr].status = bus_Uninvolved;

            }

        }

    }


    Deploy_Battle_Units(_combat_attacker_player);  // sets cgx,cgy and target_cgx,cgy

    Deploy_Battle_Units(_combat_defender_player);  // sets cgx,cgy and target_cgx,cgy


    if(_players[_combat_attacker_player].casting_spell_idx == spl_Spell_Of_Return)
    {

        // ; THIS WILL CORRUPT MEMORY IF IT IS POSSIBLE AT ALL!
        _players[_combat_attacker_player].Cmbt_Skill_Left = 0;

    }
    else
    {

        _players[_combat_attacker_player].Cmbt_Skill_Left = _players[_combat_attacker_player].Nominal_Skill;

    }


    // ; THIS WILL CORRUPT MEMORY IF IT IS POSSIBLE AT ALL!
    if(_combat_defender_player != MOO_MONSTER_PLAYER_IDX)
    {

        if(_players[_combat_defender_player].casting_spell_idx == spl_Spell_Of_Return)
        {

            _players[_combat_defender_player].Cmbt_Skill_Left = 0;

        }
        else
        {

            _players[_combat_defender_player].Cmbt_Skill_Left = _players[_combat_defender_player].Nominal_Skill;

        }

    }
    else
    {
        STU_DEBUG_BREAK();
        // Severity	Code	Description	Project	File	Line	Suppression State	Details
        // Warning	C6386	Buffer overrun while writing to '_players'.sdl2_ReMoM	C : \STU\devel\ReMoM\src\Combat.C	6228
        // _players[_combat_defender_player].Cmbt_Skill_Left = 0;

    }
    
    return count;

}


// WZD o98p17
// drake178: BU_CheckFlight()
/*
; returns 1 if the unit has the flying movement type,
; unless it is under a Web or Black Sleep effect; or 0
*/
/*

*/
int16_t BU_CheckFlight__WIP(int16_t battle_unit_idx)
{
    int16_t result = 0;  // _CX_

    result = ST_FALSE;

    if((battle_units[battle_unit_idx].Move_Flags & MV_FLYING) != 0)
    {
        result = ST_TRUE;
    }

    if((battle_units[battle_unit_idx].Combat_Effects & bue_Web) != 0)
    {
        result = ST_FALSE;
    }

    if((battle_units[battle_unit_idx].Combat_Effects & bue_Black_Sleep) != 0)
    {
        result = ST_FALSE;
    }

    return result;

}


// WZD o98p18
// drake178: BU_IsFlying()
/*
; returns 1 if the target unit can fly in battle, or 0
; if not
;
; BUG: considers the Wind Walking spell a form of
; flight, but ignores Demon Wings unless it sets the
; flight movement flag in the BU record
*/
/*

has UE_WINDWALKING, UE_FLIGHT, MV_FLYING
not bue_Web, bue_Black_Sleep

*/
int16_t Battle_Unit_Has_Flight(int16_t battle_unit_idx)
{
    uint32_t enchantments = 0;
    int16_t has_flight = 0;  // DNE in Dasm

    has_flight = ST_FALSE;

    enchantments = (battle_units[battle_unit_idx].enchantments | battle_units[battle_unit_idx].item_enchantments);

    if((enchantments & UE_WINDWALKING) != 0)
    {

        has_flight = ST_TRUE;
        
    }

    if((enchantments & UE_FLIGHT) != 0)
    {

        has_flight = ST_TRUE;
        
    }

    if((_unit_type_table[_UNITS[battle_units[battle_unit_idx].unit_idx].type].Move_Flags & MV_FLYING) != 0)
    {

        has_flight = ST_TRUE;
        
    }

    if((battle_units[battle_unit_idx].Move_Flags & MV_FLYING) != 0)
    {

        has_flight = ST_TRUE;

    }

    if((battle_units[battle_unit_idx].Combat_Effects & bue_Web) != 0)
    {

        has_flight = ST_FALSE;

    }

    if((battle_units[battle_unit_idx].Combat_Effects & bue_Black_Sleep) != 0)
    {

        has_flight = ST_FALSE;

    }

    return has_flight;

}


// WZD o98p19
// drake178: BU_MeleeFlightCheck()
/*
; returns 1 if the specified unit can initiate melee
; against the target, or 0 if it can't
;
; BUG: considers Wind Walking a form of flight
; BUG: ignores Demon Wings unless it sets the flight
;  movement flag (which it does, but still)
*/
/*

¿ compliment to Check_Attack_Ranged() ?

*/
int16_t Check_Attack_Melee(int16_t attacker_battle_unit_idx, int16_t defender_battle_unit_idx)
{
    int16_t result = 0;
    int16_t defender_has_flight = 0;
    int16_t attacker_has_flight = 0;

    result = ST_FALSE;

    attacker_has_flight = Battle_Unit_Has_Flight(attacker_battle_unit_idx);

    defender_has_flight = Battle_Unit_Has_Flight(defender_battle_unit_idx);

    if(
        (attacker_has_flight == ST_TRUE)
        ||
        (defender_has_flight != ST_TRUE)
        ||
        (battle_units[attacker_battle_unit_idx].ranged_type > rat_NONE)
    )
    {

        result = ST_TRUE;

    }
    else
    {

        result = ST_FALSE;

    }

    return result;

}


// WZD o98p20
// drake178: BU_PrepMoveSound()
/*
; plays the silence sound if SFX are enabled, then
; marks the World_Data@ allocation, loads the unit's
; movement sound into it, finally undoing the
; allocation
; returns the segment address for the sound effect
;
; BUG: should not mark, load, or undo if SFX are
; disabled, but should instead return the expected - 1
; in this case
*/
/*

*/
// SAMB_ptr BU_PrepMoveSound__WIP(int16_t battle_unit_idx)
void BU_PrepMoveSound__WIP(int16_t battle_unit_idx, SAMB_ptr * Sound_Data_Seg, uint32_t * Sound_Data_Seg_size)
{
    // SAMB_ptr sound_seg;
    SAMB_ptr l_sound_seg;
    uint32_t l_sound_seg_size;

    if(magic_set.sound_effects == ST_TRUE)
    {
        // DOMSDOS  Play_Sound__STUB(SND_CMB_Silence);
        sdl2_Play_Sound__WIP(SND_CMB_Silence, SND_CMB_Silence_size);
    }

    Mark_Block(World_Data);

    // Sound_Data_Seg = BU_LoadMoveSound__WIP(battle_unit_idx);
    BU_LoadMoveSound__WIP(battle_unit_idx, &l_sound_seg, &l_sound_seg_size);

    Release_Block(World_Data);

    // return Sound_Data_Seg;
    *Sound_Data_Seg = l_sound_seg;
    * Sound_Data_Seg_size = l_sound_seg_size;

}


// WZD o98p21
// drake178: G_CMB_Auto_OFF()
/*
; turns auto combat off, selects the next unit, and
; fills the target maps?
;
; contains a select next BU call, RE-EXPLORE!
*/
/*
    Cancel / Turn Of Auto Combat

OON XREF:
    Tactical_Combat__WIP()
        ***cancel auto combat***

*/
void Turn_Off_Auto_Combat(void)
{

    _auto_combat_flag = ST_FALSE;

    Next_Battle_Unit(_human_player_idx);

    Assign_Combat_Grids();

}


// WZD o98p22
/*
; composes the "While fleeing, you lost ..." string
; into the GUI_NearMsgString global based on the passed list of units
; INCONSISTENT in its use of plurals with the rest of the game
*/
void STK_ComposeFleeLost__STUB(int16_t troop_count, int16_t troop_list[])
{
    int16_t Unit_Types = 0;
    int16_t Type_Counts = 0;
    int16_t rest_of_the_array = 0;
    int16_t Last_Char_Pointer = 0;
    int16_t Conv_String = 0;
    int16_t Record_Count = 0;
    int16_t Added_Count = 0;
    int16_t Record_Loopvar = 0;
    int16_t itr = 0;  // _DI_

    if(troop_count <= 0)
    {
        strcpy(GUI_NearMsgString, "");
        return;
    }



    strcpy(GUI_NearMsgString, "");
    return;

}



/*
    WIZARDS.EXE  ovr099
*/

// WZD o99p01
// WZD o99p02
// WZD o99p03
// WZD o99p04
// WZD o99p05
// WZD o99p06
// WZD o99p07

// WZD o99p01
// drake178: CMB_DrawFullScreen()
// MoO2  Module: CMBTDRW1  Draw_Main_Combat_Screen_()
/*
; draws the combat UI into the current draw frame using
; only global variables, on a background loaded from
; the third VGA frame, recreating the combat entity
; (sprite) array on every run
*/
/*

CMB_DrawMap__WIP() calls Copy_Back_To_Off()
    and CMB_DrawEntities__WIP()

*/
void Tactical_Combat_Draw(void)
{
    int16_t First_CE_Help_Entry = 0;
    int16_t Opponent_Type = 0;
    int16_t Can_Split_Name = 0;
    int16_t CE_Window_Left = 0;
    uint8_t colors[6] = { 0, 0, 0, 0, 0, 0 };
    int16_t itr = 0;  // _SI_
    int16_t string_index = 0;  // _DI_

    Set_Page_Off();

    CMB_CreateEntities__WIP();

    CMB_DrawMap__WIP();

    Reset_Window();


    _combat_total_battle_effect_count = Combat_Info_Effects_Count();


    Tactical_Combat_Draw_Buttons();


    if(_combat_attacker_player == _human_player_idx)
    {
        CE_Window_Left = 247;
        First_CE_Help_Entry = 9;
    }
    else
    {
        CE_Window_Left = 8;
        First_CE_Help_Entry = 13;
    }

    _help_entries[ 9].help_idx = ST_UNDEFINED;
    _help_entries[10].help_idx = ST_UNDEFINED;
    _help_entries[11].help_idx = ST_UNDEFINED;
    _help_entries[12].help_idx = ST_UNDEFINED;
    _help_entries[13].help_idx = ST_UNDEFINED;
    _help_entries[14].help_idx = ST_UNDEFINED;
    _help_entries[15].help_idx = ST_UNDEFINED;
    _help_entries[16].help_idx = ST_UNDEFINED;

    for(itr = 0; (((CMB_ATKR_First_CE + itr) < attacker_enchantment_count) && (itr < 4)); itr++)
    {

        FLIC_Draw((CE_Window_Left + (itr * 17)), 179, CMB_ATKR_CE_Wnd[(CMB_ATKR_First_CE + itr)].icon_seg);

        _help_entries[(First_CE_Help_Entry + itr)].help_idx = CMB_ATKR_CE_Wnd[(CMB_ATKR_First_CE + itr)].help_idx;

    }

    if(_combat_defender_player == _human_player_idx)
    {
        CE_Window_Left = 247;
        First_CE_Help_Entry = 9;
    }
    else
    {
        CE_Window_Left = 8;
        First_CE_Help_Entry = 13;
    }

    for(itr = 0; (((CMB_DEFR_First_CE + itr) < defender_enchantment_count) && (itr < 4)); itr++)
    {
        
        FLIC_Draw((CE_Window_Left + (itr * 17)), 179, CMB_DEFR_CE_Wnd[(CMB_DEFR_First_CE + itr)].icon_seg);

        _help_entries[(First_CE_Help_Entry + itr)].help_idx = CMB_DEFR_CE_Wnd[(CMB_DEFR_First_CE + itr)].help_idx;

    }


    colors[0] = 241;


    Opponent_Type = 0;  /* Player */

    if(CMB_AI_Player == NEUTRAL_PLAYER_IDX)
    {

        for(itr = 0; itr < _combat_total_unit_count; itr++)
        {

            if(battle_units[itr].controller_idx == CMB_AI_Player)
            {

                if((_unit_type_table[_UNITS[battle_units[itr].unit_idx].type].Abilities & UA_FANTASTIC) != 0)
                {
                    Opponent_Type = 1;  /* Monsters */
                }
                else
                {
                    Opponent_Type = 2;  /* Raiders */
                }

            }

        }

    }


    if(OVL_Action_Type == 0)  /* Enemy Stack */
    {

        for(itr = 1; itr < 5; itr++)
        {

            colors[itr] = COL_CMBUI_Banner[_players[CMB_AI_Player].banner_id][(itr - 1)];

        }

        Set_Font_Colors_15(4, &colors[0]);

        Set_Font_Style_Shadow_Down(4, 15, 0, 0);

        Set_Outline_Color(241);

        if(CMB_AI_Player != NEUTRAL_PLAYER_IDX)
        {

            Print_Centered(40, 168, _players[CMB_AI_Player].name);

        }
        else
        {

            if(Opponent_Type == 1)  /* Monsters */
            {

                Print_Centered(40, 168, str_Monsters__ovr099);

            }
            else  /* Raiders */
            {

                Print_Centered(40, 168, str_Raiders__ovr099);

            }

        }

    }
    else if(OVL_Action_Type == 1)  /* Enemy City */
    {

        for(itr = 1; itr < 5; itr++)
        {

            colors[itr] = COL_CMBUI_Banner[_players[CMB_AI_Player].banner_id][(itr - 1)];

        }

        Set_Font_Colors_15(4, &colors[0]);

        Set_Font_Style_Shadow_Down(4, 15, 0, 0);

        Set_Outline_Color(241);

        if(CMB_AI_Player != NEUTRAL_PLAYER_IDX)
        {

            Print_Centered(40, 168, _players[CMB_AI_Player].name);

        }
        else
        {

            if(
                (Opponent_Type == 0)  /* Player */
                ||
                (_combat_attacker_player != NEUTRAL_PLAYER_IDX)
            )
            {

                Print_Centered(40, 168, CMB_CityName);

            }
            else
            {

                if(Opponent_Type == 1)  /* Monsters */
                {

                    Print_Centered(40, 168, str_Monsters__ovr099);

                }
                else  /* Raiders */
                {

                    Print_Centered(40, 168, str_Raiders__ovr099);

                }

            }

        }

    }
    else if(OVL_Action_Type == 5)  /* Lair */
    {

        strcpy(GUI_String_1, TBL_EZ_Names[_LAIRS[OVL_Action_Structure].type]);

        string_index = 0;

        Can_Split_Name = ST_FALSE;

        for(itr = 0; strlen(GUI_String_1) > itr; itr++)
        {

            if(GUI_String_1[string_index] == ' ')
            {

                Can_Split_Name = ST_TRUE;

                break;

            }

            string_index++;

        }

        string_index++;

        if(Can_Split_Name == ST_FALSE)
        {
            string_index = 0;
        }

        GUI_String_1[string_index] -= 32;

        for(itr = 1; itr < 5; itr++)
        {

            colors[itr] = COL_CMBUI_Banner[_players[CMB_AI_Player].banner_id][(itr - 1)];

        }

        Set_Font_Colors_15(4, &colors[0]);

        Set_Font_Style_Shadow_Down(4, 15, 0, 0);

        Set_Outline_Color(241);

        Print_Centered(40, 167, &GUI_String_1[string_index]);

    }


    for(itr = 1; itr < 5; itr++)
    {

        colors[itr] = COL_CMBUI_Banner[_players[_human_player_idx].banner_id][(itr - 1)];

    }

    Set_Font_Colors_15(4, &colors[0]);

    Set_Font_Style_Shadow_Down(4, 15, 0, 0);

    Set_Outline_Color(241);

    Print_Centered(278, 167, _players[_human_player_idx].name);

    Draw_Spell_Information_Window();

    /*
        BEGIN:  Combat Unit Display
    */
    if(magic_set.auto_unit_information == ST_TRUE)
    {

        Draw_Combat_Unit_Display();

    }
    /*
        END:  Combat Unit Display
    */


    Cycle_Palette_Color__STUB(198, 0, 0, 0, 55, 0, 0, 11);

    if(
        (CMB_ImmobileCanAct == ST_TRUE)
        &&
        (_auto_combat_flag == ST_FALSE)
        &&
        (_active_battle_unit != ST_UNDEFINED)
    )
    {

        if(battle_units[_active_battle_unit].controller_idx != combat_human_player)
        {
            
            Draw_Active_Unit_Window();

        }
        else
        {
            if(CMB_HumanTurn == ST_TRUE)
            {
                colors[0] = 227;
                colors[1] = 243;
                Set_Font_Colors_15(0, &colors[0]);
                Set_Outline_Color(227);
                Set_Font_Style_Shadow_Down(0, 15, 0, 0);
                Set_Font_LF(1);
                Print_Paragraph(84, 167, 58, cnst_Cmbt_Immobile, 0);  // "All units are immobilized. Select an action."
            }

        }

    }
    else
    {

        Draw_Active_Unit_Window();

    }


    // Eh? CMB_VortexAnimStage++ > 5 ? CMB_VortexAnimStage : 0;

    CMB_VortexAnimStage++;

    if(CMB_VortexAnimStage > 5)
    {
        CMB_VortexAnimStage = 0;
    }

    CMB_ChasmAnimStage++;

    if(CMB_ChasmAnimStage > 7)
    {
        CMB_ChasmAnimStage = 0;
    }

    frame_anim_cycle++;

    if(frame_anim_cycle > 2)
    {
        frame_anim_cycle = 0;
    }

    CMB_MudAnimStage++;

    if(CMB_MudAnimStage > 2)
    {
        CMB_MudAnimStage = 0;
    }


    if(DBG_debug_flag)
    {
        Combat_Screen_Draw_Debug_Information();
    }

}


// WZD o99p02
// drake178: CMB_DrawActiveUnitW()
/*
; draws the active/selected battle unit and its stats
; and icons normally shown in the active unit window
; into the current draw frame; or alternatively the
; magic vortex icon and movement prompt text
*/
/*

*/
void Draw_Active_Unit_Window(void)
{
    uint8_t colors[4] = { 0, 0, 0, 0 };
    int16_t unit_owner_idx = 0;
    int16_t unit_type = 0;
    int16_t unit_idx = 0;
    int16_t bitm_h = 0;
    int16_t bitm_w = 0;
    int16_t bitm_y = 0;
    int16_t bitm_x = 0;
    int16_t Hero_Slot = 0;
    int16_t start_y = 0;  // _DI_
    int16_t start_x = 0;  // _SI_

    if(_active_battle_unit == ST_UNDEFINED)
    {
        FLIC_Set_CurrentFrame(IMG_GUI_Vortex, 1);
        Draw_Picture_To_Bitmap(IMG_GUI_Vortex, GfxBuf_2400B);
        FLIC_Set_CurrentFrame(IMG_GUI_Vortex, CMB_VortexAnimStage);
        start_x = 82;
        start_y = 170;
        Get_Bitmap_Actual_Size(GfxBuf_2400B, &bitm_x, &bitm_y, &bitm_w, &bitm_h);
        start_x = (((32 - bitm_w) / 2) - bitm_x);
        start_y = (((25 - bitm_h) / 2) - bitm_y);
        Draw_Picture_Windowed(start_x, start_y, GfxBuf_2400B);
        colors[0] = 227;
        colors[1] = 243;
        Set_Font_Colors_15(0, &colors[0]);
        Set_Outline_Color(227);
        Set_Font_Style_Shadow_Down(0, 15, 0, 0);
        Set_Font_Spacing_Width(1);
        Print(115, 172, cnst_VortexMove_1);  // "Move"
        Print(115, 178, cnst_VortexMove_2);  // "Magic"
        Print(115, 184, cnst_VortexMove_3);  // "Vortex"
        Print(115, 190, cnst_VortexMove_4);  // "1 Space"
    }
    else
    {

        // composes figure picture into GfxBuf_2400B
        USELESS_Combat_Figure_Load_Compose(battle_units[_active_battle_unit].bufpi, 2, battle_units[_active_battle_unit].controller_idx, battle_units[_active_battle_unit].outline_magic_realm, 0);

        Combat_Figure_Effect__WIP(battle_units[_active_battle_unit].Image_Effect);

        if((_unit_type_table[_UNITS[battle_units[_active_battle_unit].unit_idx].type].Abilities & UA_INVISIBILITY) != 0)
        {

            Outline_Bitmap_Pixels(GfxBuf_2400B, 1);

        }

        unit_idx = battle_units[_active_battle_unit].unit_idx;

        unit_type = _UNITS[unit_idx].type;

        start_x = 84;
        start_y = 173;

        Get_Bitmap_Actual_Size(GfxBuf_2400B, &bitm_x, &bitm_y, &bitm_w, &bitm_h);

        start_x += (((32 - bitm_w) / 2) - bitm_x);

        start_y += (((25 - bitm_h) / 2) - bitm_y);

        FLIC_Set_LoopFrame_1(GfxBuf_2400B);

        // draws the composed figure picture, USELESS_Combat_Figure_Load_Compose() put in GfxBuf_2400B
        Draw_Picture(start_x, start_y, GfxBuf_2400B);


        Draw_Active_Unit_Stats_And_Icons();


        colors[0] = 227;
        colors[1] = 243;

        Set_Font_Colors_15(0, &colors[0]);

        Set_Outline_Color(227);

        Set_Font_Style_Shadow_Down(0, 15, 0, 0);

        Set_Font_Spacing_Width(1);

        unit_owner_idx = _UNITS[unit_idx].owner_idx;

        Hero_Slot = _UNITS[unit_idx].Hero_Slot;


        if(Hero_Slot != ST_UNDEFINED)
        {

            strcpy(GUI_String_1, _players[unit_owner_idx].Heroes[Hero_Slot].name);

        }
        else
        {

            strcpy(GUI_String_1, *_unit_type_table[unit_type].name);

        }

        Print_Centered(112, 167, GUI_String_1);

        FLIC_Draw(117, 194, unit_hit_bar_seg);

        Draw_Active_Unit_Damage_Bar(_active_battle_unit, 118, 195);

    }

}


// WZD o99p03
// drake178: CMB_Draw_GUI_Btns()
// MoO2  Module: COMBAT1  Add_Tactical_Combat_Fields_()
/*
; draws the active (unclicked) or locked action buttons
; into the current draw frame, depending on the
; circumstances
*/
/*

    ¿ no lock auto button ?



*/
void Tactical_Combat_Draw_Buttons(void)
{

    if(_auto_combat_flag == ST_TRUE)
    {

        FLIC_Set_CurrentFrame(_cmbt_auto_button_seg, 0);

        FLIC_Draw(170, 178, _cmbt_auto_button_seg);

        FLIC_Draw(144, 168, _cmbt_lock_spell_button_seg);

        FLIC_Draw(170, 168, _cmbt_lock_wait_button_seg);

        FLIC_Draw(170, 188, _cmbt_lock_done_button_seg);

        FLIC_Draw(144, 178, _cmbt_lock_info_button_seg);

        FLIC_Draw(144, 188, _cmbt_lock_flee_button_seg);

    }
    else
    {

        /* DEDU  used three other places? macro? */
        if(
            (battle_units[_active_battle_unit].movement_points > 0)
            &&
            (
                (battle_units[_active_battle_unit].mana >= 3)
                ||
                (BU_HasSpellAbility__WIP(_active_battle_unit) == ST_TRUE)
                ||
                (battle_units[_active_battle_unit].Item_Charges > 0)
                ||
                (CMB_WizCastAvailable == ST_TRUE)
            )
        )
        {

            FLIC_Set_CurrentFrame(_cmbt_spell_button_seg, 0);

            FLIC_Draw(144, 168, _cmbt_spell_button_seg);

        }
        else
        {

            FLIC_Draw(144, 168, _cmbt_lock_spell_button_seg);

        }

        FLIC_Set_CurrentFrame(_cmbt_wait_button_seg, 0);

        FLIC_Draw(170, 168, _cmbt_wait_button_seg);

        FLIC_Set_CurrentFrame(_cmbt_done_button_seg, 0);

        FLIC_Draw(170, 188, _cmbt_done_button_seg);

        if(_combat_total_battle_effect_count > 0)
        {

            FLIC_Set_CurrentFrame(_cmbt_info_button_seg, 0);

            FLIC_Draw(144, 178, _cmbt_info_button_seg);

        }
        else
        {

            FLIC_Draw(144, 178, _cmbt_lock_info_button_seg);

        }

        FLIC_Set_CurrentFrame(_cmbt_auto_button_seg, 0);

        FLIC_Draw(170, 178, _cmbt_auto_button_seg);

        FLIC_Set_CurrentFrame(_cmbt_flee_button_seg, 0);

        FLIC_Draw(144, 188, _cmbt_flee_button_seg);

    }

}


// WZD o99p04
// drake178: CMB_DrawSpellInfoW()
/*
; draws the text and numbers of the spell information
; window for the human player into the current draw
; frame
*/
/*

*/
void Draw_Spell_Information_Window(void)
{
    uint8_t colors[4] = { 0, 0, 0, 0 };
    char Range_Display_String[6] = { 0, 0, 0, 0, 0, 0 };
    char Range_Number_String[6] = { 0, 0, 0, 0, 0, 0 };
    int16_t value = 0;  // _SI_

    colors[0] = 227;
    colors[1] = 243;

    Set_Font_Colors_15(0, &colors[0]);

    Set_Outline_Color(227);

    Set_Font_Style_Shadow_Down(0, 15, 0, 0);

    Print_Integer_Right(236, 172, _players[_human_player_idx].Cmbt_Skill_Left);

    Print(200, 172, cnst_CMB_Skill);

    Print_Integer_Right(236, 181, _players[_human_player_idx].mana_reserve);

    Print(200, 181, cnst_CMB_Mana);

    value = Combat_Casting_Cost_Multiplier(_human_player_idx);

    itoa(value, Range_Number_String, 10);

    Print(200, 190, cnst_CMB_Range);

    if(value < 10)
    {

        Range_Display_String[0] = '0';

        Range_Display_String[1] = '.';

        Range_Display_String[2] = Range_Number_String[0];

        Range_Display_String[3] = 'x';

        Range_Display_String[4] = 0;

    }
    else
    {

        Range_Display_String[0] = Range_Number_String[0];

        Range_Display_String[1] = 'x';

        Range_Display_String[2] = 0;

        if(Range_Number_String[1] != '0')
        {

            Range_Display_String[1] = '.';

            Range_Display_String[2] = Range_Number_String[1];

            Range_Display_String[3] = 'x';

            Range_Display_String[4] = 0;

        }

    }

    Print_Right(236, 190, Range_Display_String);

}


// WZD o99p05
// drake178: CMB_DrawAUWStats()
/*
; draws the attack and movement stats and icons of the
; active/selected battle unit into the active unit
; window area of the current draw frame
*/
/*

~ Draw_Combat_Unit_Display()

*/
void Draw_Active_Unit_Stats_And_Icons(void)
{
    uint8_t colors[4] = { 0, 0, 0, 0};
    int16_t move_type = 0;
    int16_t icon_idx = 0;  // _SI_
    int16_t attack_strength = 0;  // _DI_

    colors[0] = 227;
    colors[1] = 243;

    Set_Font_Colors_15(0, &colors[0]);

    Set_Outline_Color(227);

    Set_Font_Style_Shadow_Down(0, 15, 0, 0);


    icon_idx = Battle_Unit_Melee_Attack_Icon(_active_battle_unit);

    if(icon_idx != ST_UNDEFINED)
    {

        attack_strength = battle_units[_active_battle_unit].melee;

        Print_Integer_Right(126, 174, attack_strength);

        FLIC_Draw(128, 172, combat_weapon_icon_segs[icon_idx]);

    }


    icon_idx = Battle_Unit_Ranged_Attack_Icon(_active_battle_unit);

    if(icon_idx != ST_UNDEFINED)
    {

        attack_strength = battle_units[_active_battle_unit].ranged;

        Print_Integer_Right(126, 181, attack_strength);

        FLIC_Draw(128, 179, combat_weapon_icon_segs[icon_idx]);

    }


    if(battle_units[_active_battle_unit].movement_points != 0)
    {

        Print_Moves_String(126, 188, battle_units[_active_battle_unit].movement_points, 1);

    }
    else
    {

        Print_Integer_Right(126, 188, battle_units[_active_battle_unit].movement_points);

    }


    move_type = Battle_Unit_Movement_Icon(_active_battle_unit);

    icon_idx = 0;

    if(move_type == 2)  /* Move_Flags & MV_FLYING */
    {

        icon_idx = 1;

    }
    else if(
        (move_type == 4)  /* Move_Flags & MV_SAILING */
        ||
        (move_type == 5)  /* Move_Flags & MV_SWIMMING */
    )
    {

        icon_idx = 2;

    }

    FLIC_Draw(128, 186, combat_movemode_icon_segs[icon_idx]);

}


// WZD o99p06
// drake178: CMB_CreateEntities()
/*
; resets and recreates the combat entity array for
; drawing the combat sprites
*/
/*
¿ ~== MainScr_Maps.C  Set_Entities_On_Map_Window() ?

~ Create Entities
    clears entities and defaults order
    *spawns* trees, rocks, figures, projectiles, vortices, structures

*/
void CMB_CreateEntities__WIP(void)
{
    int16_t Curse_Anim = 0;
    int16_t unit_figure_maximum = 0;
    int16_t unit_figure_count = 0;
    int16_t itr = 0;  // _SI_
    int16_t itr_figures = 0;  // _DI_
    // struct s_BATTLE_UNIT * bu = 0;  // DNE in Dasm

    // sets combat_grid_entity_count to 0
    Clear_Combat_Grid_Entities();

    // TODO  CMB_SpawnTrees();
    // ; creates combat entities for each tree appearing on
    // ; the battlefield

    // TODO  CMB_SpawnRocks();
    // ; creates combat entities for each rock appearing on
    // ; the battlefield
    // ;
    // ; BUG: uses the tree counts and indices instead of the
    // ; proper ones, resulting in either not enough/wrong
    // ; rocks, or extra empty combat entities being created

    CMB_UpdateTrueSight();

    for(itr = 0; itr < _combat_total_unit_count; itr++)
    {
        // bu = &battle_units[itr];


        if(battle_units[itr].status != bus_Active)
        {
            continue;
        }

        unit_figure_count = battle_units[itr].Cur_Figures;

        unit_figure_maximum = battle_units[itr].Max_Figures;

        if(_UNITS[battle_units[itr].unit_idx].type == spell_data_table[SPL_HYDRA].unit_type)
        {
            unit_figure_count = 1;
            unit_figure_maximum = 1;
        }

        battle_units[itr].Image_Effect = 0;

        Combat_Unit_Enchantment_Outline_Set(itr);  // sets battle_units[].outline_magic_realm

        BU_SetBaseAnims__WIP(itr);  // sets Always_Animate and Move_Bob

        // TODO  Curse_Anim = BU_GetCombatEffect(itr);
        // ; returns the highest order (negative) combat effect
        // ; on the unit, or -1 if there aren't any; Black Sleep
        // ; and Warp Creature set a BU image effect instead
        /* HACK */  Curse_Anim = ST_UNDEFINED;

        BU_SetVisibility__WIP(itr);  // sets Image_Effect to 4 or 5

        for(itr_figures = 0; itr_figures < unit_figure_count; itr_figures++)
        {
            CMB_SpawnFigure__WIP(battle_units[itr].bufpi, battle_units[itr].cgx, battle_units[itr].cgy, battle_units[itr].target_cgx, battle_units[itr].target_cgy, battle_units[itr].MoveStage, itr_figures, unit_figure_maximum, battle_units[itr].controller_idx, battle_units[itr].outline_magic_realm, battle_units[itr].Blood_Amount, battle_units[itr].Moving, battle_units[itr].Atk_FigLoss, 0);
            // CMB_SpawnFigure__WIP(bu->bufpi, bu->cgx, bu->cgy, bu->target_cgx, bu->target_cgy, bu->MoveStage, itr_figures, unit_figure_maximum, bu->controller_idx, bu->outline_magic_realm, bu->Blood_Amount, bu->Moving, bu->Atk_FigLoss, 0);
        }

        if(Curse_Anim != ST_UNDEFINED)
        {

            // TODO  CMB_SpawnUnitCurse(battle_units[itr].cgx, battle_units[itr].cgy, battle_units[itr].target_cgx, battle_units[itr].target_cgy, battle_units[itr].MoveStage, Curse_Anim);
            // ; creates a combat entity corresponding to the passed
            // ; curse effect and positioning

        }

    }


    // TODO  CMB_SpawnProjectiles();

    // TODO  CMB_SpawnVortices();

    // TODO  CMB_SpawnStructures();


    Cycle_Unit_Enchantment_Animation();

}


// WZD o99p07
// drake178: CMB_DrawBUHPBar()
/*
; draws a battle unit HP bar into the current draw
; segment, with one pixel representing 5% of the unit's
; total health, colored green at 65% or more, yellow
; between 30% and 65%, and red below 30%
*/
/*

~== Unit_Window_Draw_Unit_Attributes()

*/
void Draw_Active_Unit_Damage_Bar(int16_t battle_unit_idx, int16_t x, int16_t y)
{
    int16_t bar_color = 0;
    int16_t damage_hits = 0;
    int16_t current_hits = 0;
    int16_t max_hits = 0;
    int16_t bar_length = 0;  // _DI_

    max_hits = (battle_units[battle_unit_idx].hits * battle_units[battle_unit_idx].Max_Figures);

    damage_hits = ((battle_units[battle_unit_idx].Max_Figures - battle_units[battle_unit_idx].Cur_Figures) * battle_units[battle_unit_idx].hits);

    if(battle_units[battle_unit_idx].TopFig_Dmg > 0)
    {

        damage_hits += battle_units[battle_unit_idx].TopFig_Dmg;

    }

    current_hits = (max_hits - damage_hits);

    /*
        BEGIN: Draw Unit Damage Bar
    */
    if(current_hits > 0)
    {

        bar_length = (((current_hits * 20) / max_hits) - 1);

        if(bar_length >= 13)
        {

            bar_color = DAMAGE_BAR_GREEN;

        }
        else if(bar_length >= 6)
        {

            bar_color = DAMAGE_BAR_YELLOW;

        }
        else
        {

            bar_color = DAMAGE_BAR_RED;

        }

        Line(x, y, (x + bar_length), y, bar_color);

    }
    /*
        END: Draw Unit Damage Bar
    */

}



/*
    WIZARDS.EXE  ovr103
*/

// WZD s103p01
// drake178: CMB_ShowInfoWindow()
/*
; sets up and displays the combat information window
;
; INCONSISTENT: excludes Evil Omens and Spell Wards
*/
/*

*/
void Combat_Information_Window(void)
{
    int16_t full_screen_ESC_field = 0;
    int16_t input_field_idx = 0;
    int16_t leave_screen = 0;
    int16_t itr = 0;  // _SI_
    int16_t IDK_screen_offset = 0;  // _DI_

    CMB_CentralStructure = battlefield->Central_Structure;

    CMB_CloudofShadow = battlefield->city_enchantments[CLOUD_OF_SHADOW];
    CMB_HeavenlyLight = battlefield->city_enchantments[HEAVENLY_LIGHT];

    Clear_Fields();

    Deactivate_Auto_Function();

    Assign_Auto_Function(Combat_Information_Window_Draw, 1);

    Set_Mouse_List(1, mouse_list_default);

    Copy_On_To_Off_Page();

    Copy_Off_To_Back();

    GUI_String_1 = (char *)Near_Allocate_First(80);

    for(itr = 0; itr < 14; itr++)
    {

        // _combat_info_effects[itr] = Near_Allocate_Next(30);
        _combat_info_effects[itr] = (struct s_CMB_InfoItem *)Near_Allocate_Next(sizeof(struct s_CMB_InfoItem));

    }

    Mark_Block(World_Data);

    Mark_Block(_screen_seg);

    Allocate_Next_Block(_screen_seg, 990);

    // COMPIX.LBX, 056  "INFOBAC1"   "info bot"
    // COMPIX.LBX, 057  "INFOBAC2"   "info mid"
    // COMPIX.LBX, 058  "INFOBAC3"   "info top"
    // COMPIX.LBX, 059  "INFOBOX"    "info box"

    _combat_info_wnd_bot_seg = LBX_Reload_Next(compix_lbx_file__ovr103, 56, World_Data);

    _combat_info_wnd_mid_seg = LBX_Reload_Next(compix_lbx_file__ovr103, 57, World_Data);

    _combat_info_wnd_box_seg = LBX_Reload_Next(compix_lbx_file__ovr103, 59, World_Data);

    // COMPIX.LBX, 042  "INFOICON"   "crusade"
    // COMPIX.LBX, 043  "INFOICON"   "holy arms"
    // COMPIX.LBX, 044  "INFOICON"   "holy light"
    // COMPIX.LBX, 045  "INFOICON"   "charm of life"
    // COMPIX.LBX, 046  "INFOICON"   "chaos surge"
    // COMPIX.LBX, 047  "INFOICON"   "red node"
    // COMPIX.LBX, 048  "INFOICON"   "red node w/aura"
    // COMPIX.LBX, 049  "INFOICON"   "eternal night"
    // COMPIX.LBX, 050  "INFOICON"   "cloud of darkness"
    // COMPIX.LBX, 051  "INFOICON"   "zombie mastery"
    // COMPIX.LBX, 052  "INFOICON"   "green node"
    // COMPIX.LBX, 053  "INFOICON"   "green node w/aura"
    // COMPIX.LBX, 054  "INFOICON"   "blue node"
    // COMPIX.LBX, 055  "INFOICON"   "blue node w/aura"

    for(itr = 0; itr < 14; itr++)
    {

        _combat_info_effect_icon_segs[itr] = LBX_Reload_Next(compix_lbx_file__ovr103, (42 + itr), World_Data);

    }

    _combat_info_wnd_top_seg = LBX_Reload_Next(compix_lbx_file__ovr103, 58, _screen_seg);


    Combat_Info_Effects();


    _combat_info_wnd_start_x = 50;
    _combat_info_wnd_start_y = 10;


    if(_combat_info_item_count > 0)
    {

        IDK_screen_offset = (((_combat_info_item_count / 2) * 19) + 9);

    }
    else
    {

        IDK_screen_offset = 4;

    }


    if(CMB_PerSideInfo > 0)
    {

        IDK_screen_offset = (_combat_info_wnd_start_y + (CMB_PerSideInfo * 20) + IDK_screen_offset + 21);

    }
    else
    {

        IDK_screen_offset = (_combat_info_wnd_start_y + (CMB_PerSideInfo * 20) + IDK_screen_offset);

    }

    _combat_info_wnd_start_y = (100 - (IDK_screen_offset / 2));

    IDK_screen_offset = (_combat_info_wnd_start_y + IDK_screen_offset);

    Deactivate_Help_List();

    for(itr = 0; itr < 20; itr++)
    {

        _help_entries[itr].help_idx = ST_UNDEFINED;
        _help_entries[itr].x1 = 0;
        _help_entries[itr].y1 = 0;
        _help_entries[itr].x2 = 0;
        _help_entries[itr].y2 = 0;

    }

    Set_Help_List(_help_entries, 20);

    leave_screen = ST_FALSE;

    while(leave_screen == ST_FALSE)
    {

        Mark_Time();

        Clear_Fields();

        full_screen_ESC_field = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, cnst_HOTKEY_Esc13[0], ST_UNDEFINED);

        input_field_idx = Get_Input();

        if(input_field_idx == full_screen_ESC_field)
        {

            leave_screen = ST_UNDEFINED;

        }

        if(leave_screen == ST_FALSE)
        {

            Copy_Back_To_Off();

            Combat_Information_Window_Draw();

            PageFlip_FX();

            Release_Time(1);

        }

    }

    Release_Block(_screen_seg);

    Release_Block(World_Data);

    Clear_Fields();

    Deactivate_Auto_Function();

    PageFlipEffect = 3;

    Deactivate_Help_List();

}


// WZD s103p02
// drake178: CMB_DrawInfoWindow()
void Combat_Information_Window_Draw(void)
{
    int16_t Drawing_Sides = 0;
    uint8_t colors[4] = { 0, 0, 0, 0 };
    int16_t PerSide_Draw_Top = 0;
    int16_t Draw_Top = 0;
    int16_t Draw_Left = 0;
    // int16_t Middle_Y = 0;
    int16_t IDK_itr = 0;  // _SI_
    // int16_t IDK_count = 0;  // _DI_
    int16_t top_y2 = 0;  // uses IDK_count
    int16_t mid_start_y = 0;  // same as top_y2
    int16_t mid_y2 = 0;  // Middle_Y
    int16_t bot_y1 = 0;  // same as mid_y2
    int16_t bot_start_y = 0;  // same as bot_y1 - 2

    Set_Page_Off();

    if(_combat_info_item_count > 0)
    {

        // IDK_count = (9 + ((_combat_info_item_count / 2) * 19));
        // IDK_count += _combat_info_wnd_start_y;
        // Set_Window(0, 0, 319, IDK_count);
        top_y2 = (9 + ((_combat_info_item_count / 2) * 19));
        top_y2 += _combat_info_wnd_start_y;
        Set_Window(0, 0, 319, top_y2);

    }
    else
    {

        // IDK_count = 4;
        // Set_Window(0, 0, 319, IDK_count);
        top_y2 = 4;
        Set_Window(0, 0, 319, top_y2);

    }

    Clipped_Draw(_combat_info_wnd_start_x, _combat_info_wnd_start_y, _combat_info_wnd_top_seg);

    Reset_Window();

    if(CMB_PerSideInfo > 0)
    {

        // Middle_Y = (CMB_PerSideInfo * 20) + IDK_count + 21;
        mid_y2 = (CMB_PerSideInfo * 20) + top_y2 + 21;

    }
    else
    {

        // Middle_Y = ((CMB_PerSideInfo * 20) + IDK_count);
        mid_y2 = ((CMB_PerSideInfo * 20) + top_y2);

    }

    // Set_Window(0, 0, 319, Middle_Y);
    Set_Window(0, 0, 319, mid_y2);

    // Clipped_Draw(_combat_info_wnd_start_x, IDK_count, _combat_info_wnd_mid_seg);
    mid_start_y = top_y2;
    Clipped_Draw(_combat_info_wnd_start_x, mid_start_y, _combat_info_wnd_mid_seg);

    if(CMB_PerSideInfo > 0)
    {

        Reset_Window();

    }
    else
    {

        // Set_Window(0, Middle_Y, 319, 199);
        // Middle_Y -= 2;
        bot_y1 = mid_y2;
        Set_Window(0, bot_y1, 319, 199);
        bot_y1 -= 2;

    }

    // Clipped_Draw(_combat_info_wnd_start_x, Middle_Y, _combat_info_wnd_bot_seg);
    bot_start_y = bot_y1;
    Clipped_Draw(_combat_info_wnd_start_x, bot_start_y, _combat_info_wnd_bot_seg);

    Reset_Window();

    colors[0] = 250;
    colors[1] = 177;

    Set_Outline_Color(254);

    Set_Font_Colors_15(2, &colors[0]);

    Set_Font_Style_Shadow_Down(2, 15, 0, 0);

    Set_Font_Spacing_Width(2);

    if(CMB_PerSideInfo > 0)
    {

        if(_combat_info_item_count > 0)
        {

            PerSide_Draw_Top = (15 + ((_combat_info_item_count / 2) * 19));

        }
        else
        {

            PerSide_Draw_Top = 11;

        }

        if(CMB_AI_Player < NEUTRAL_PLAYER_IDX)
        {

            strcpy(GUI_String_1, _players[CMB_AI_Player].name);

            strcat(GUI_String_1, cnst_SpaceSpells);

            Print_Centered((_combat_info_wnd_start_x + 54), (_combat_info_wnd_start_y + PerSide_Draw_Top), GUI_String_1);

        }

        strcpy(GUI_String_1, _players[combat_human_player].name);

        strcat(GUI_String_1, cnst_SpaceSpells);

        Print_Centered((_combat_info_wnd_start_x + 170), (_combat_info_wnd_start_y + PerSide_Draw_Top), GUI_String_1);

    }

    if(_combat_info_item_count > 0)
    {

        PerSide_Draw_Top = 10;

    }
    else
    {

        PerSide_Draw_Top = 26;

    }

    Drawing_Sides = 0;

    for(IDK_itr = 0; ((CMB_PerSideInfo / 2) + _combat_info_item_count) > IDK_itr; IDK_itr++)
    {

        if(
            (IDK_itr >= _combat_info_item_count)
            &&
            (_combat_info_item_count > 0)
            &&
            (Drawing_Sides == 0)
        )
        {

            Drawing_Sides = 1;
            PerSide_Draw_Top = 31;

        }

        Draw_Left = (_combat_info_wnd_start_x + ((IDK_itr % 2) * 112) + 11);

        Draw_Top = _combat_info_wnd_start_y + PerSide_Draw_Top + ((IDK_itr / 2) * 19);

        FLIC_Draw((Draw_Left - 1), (Draw_Top - 1), _combat_info_wnd_box_seg);

        if(_combat_info_effects[IDK_itr]->icon_seg != (SAMB_ptr)ST_UNDEFINED)
        {

            FLIC_Draw(Draw_Left, Draw_Top, _combat_info_effects[IDK_itr]->icon_seg);

            Print((Draw_Left + 20), (Draw_Top + 5), _combat_info_effects[IDK_itr]->Name);

            _help_entries[IDK_itr].help_idx = _combat_info_effects[IDK_itr]->help_idx;
            _help_entries[IDK_itr].x1 = Draw_Left;
            _help_entries[IDK_itr].y1 = Draw_Top;
            _help_entries[IDK_itr].x2 = (Draw_Left + 100);
            _help_entries[IDK_itr].y2 = (Draw_Top  +  15);

        }

    }

}


// WZD s103p03
// drake178: CMB_FillInfoArrays()
/*
; fills out the CMB_External_FX@ array and related
; global variables for the display of the combat info
; window
;
; INCONSISTENT: excludes Evil Omens and Spell Wards
*/
/*

handles
Crusade
Holy Arms
Charm Of Life
Zombie Mastery

¿ info_common_count isn't actually used for anything ?
...used to index _combat_info_effects[]
+= 2, because it's a pointer to 2-byte values?
...doesn't make sense

*/
void Combat_Info_Effects(void)
{
    int16_t computer_player_battle_effect_count = 0;
    int16_t itr_combatants = 0;
    int16_t info_common_count = 0;  // _SI_
    int16_t player_idx = 0;  // _DI_


    Combat_Info_Effects_Base();


    _combat_total_battle_effect_count = 0;

    info_common_count = _combat_info_item_count;  // just set in Combat_Info_Effects_Base()

    computer_player_battle_effect_count = 0;

    player_idx = CMB_AI_Player;

    for(itr_combatants = 0; itr_combatants < 2; itr_combatants++)
    {

        if(player_idx < NEUTRAL_PLAYER_IDX)
        {

            if(_players[player_idx].Globals[CRUSADE] > 0)
            {

                _combat_info_effects[info_common_count]->icon_seg = _combat_info_effect_icon_segs[0];

                _combat_info_effects[info_common_count]->help_idx = HLP_CRUSADE;

                strcpy(_combat_info_effects[info_common_count]->Name, cnst_Crusade);

                _combat_total_battle_effect_count++;

                info_common_count += 2;

            }

            if(_players[player_idx].Globals[HOLY_ARMS] > 0)
            {

                _combat_info_effects[info_common_count]->icon_seg = _combat_info_effect_icon_segs[1];

                _combat_info_effects[info_common_count]->help_idx = HLP_HOLY_ARMS;

                strcpy(_combat_info_effects[info_common_count]->Name, cnst_HolyArms);

                _combat_total_battle_effect_count++;

                info_common_count += 2;

            }

            if(_players[player_idx].Globals[CHARM_OF_LIFE] > 0)
            {

                _combat_info_effects[info_common_count]->icon_seg = _combat_info_effect_icon_segs[0];

                _combat_info_effects[info_common_count]->help_idx = HLP_CRUSADE;

                strcpy(_combat_info_effects[info_common_count]->Name, cnst_Crusade);

                _combat_total_battle_effect_count++;

                info_common_count += 2;

            }

            if(_players[player_idx].Globals[ZOMBIE_MASTERY] > 0)
            {

                _combat_info_effects[info_common_count]->icon_seg = _combat_info_effect_icon_segs[9];

                _combat_info_effects[info_common_count]->help_idx = HLP_ZOMBIE_MASTERY;

                strcpy(_combat_info_effects[info_common_count]->Name, cnst_ZombieMastery);

                _combat_total_battle_effect_count++;

                info_common_count += 2;

            }

        }

        player_idx = _human_player_idx;

        info_common_count = (_combat_info_item_count + 1);

        if(itr_combatants == 0)
        {

            computer_player_battle_effect_count = _combat_total_battle_effect_count;

        }

    }


    if((_combat_total_battle_effect_count - computer_player_battle_effect_count) > computer_player_battle_effect_count)
    {

        CMB_PerSideInfo = (_combat_total_battle_effect_count - computer_player_battle_effect_count);

    }
    else
    {

        CMB_PerSideInfo = computer_player_battle_effect_count;
        
    }

}


// WZD s103p04
// drake178: CMB_SetCommonXFX()
/*
; counts and adds to the CMB_Extrenal_FX@ array any
; universal / neutral external effects, whose affected
; targets do not depend on the effect origin
;
; INCONSISTENT: ignores Evil Omens
*/
/*

handles
node dispel - Sorcery, Chaos, Nature
node auta - Sorcery, Nature, Chaos
Cloud of Shadows, Heavenly Light
Chaos Surge, Eternal Night

*/
void Combat_Info_Effects_Base(void)
{
    int16_t itr = 0;  // _DI_
    int16_t idx = 0;  // _SI_

    for(itr = 0; itr < 14; itr++)
    {

        _combat_info_effects[itr]->icon_seg = (SAMB_ptr)ST_UNDEFINED;

    }

    idx = 0;

    if(CMB_CentralStructure == CS_SorceryNode)
    {

        _combat_info_effects[idx]->icon_seg = _combat_info_effect_icon_segs[12];

        _combat_info_effects[idx]->help_idx = HLP_DISPELS_NON_SORCERY;

        strcpy(_combat_info_effects[idx]->Name, cnst_SorcNodeDispel);

        idx++;

    }
    else if(CMB_CentralStructure == CS_ChaosNode)
    {

        _combat_info_effects[idx]->icon_seg = _combat_info_effect_icon_segs[5];

        _combat_info_effects[idx]->help_idx = HLP_DISPELS_NON_CHAOS;

        strcpy(_combat_info_effects[idx]->Name, cnst_ChaosNodeDispel);

        idx++;

    }
    else if(CMB_CentralStructure == CS_NatureNode)
    {

        _combat_info_effects[idx]->icon_seg = _combat_info_effect_icon_segs[10];

        _combat_info_effects[idx]->help_idx = HLP_DISPELS_NON_NATURE;

        strcpy(_combat_info_effects[idx]->Name, cnst_NatNodeDispel);

        idx++;

    }


    if(_combat_node_type == cnt_Sorcery)
    {

        _combat_info_effects[idx]->icon_seg = _combat_info_effect_icon_segs[13];

        _combat_info_effects[idx]->help_idx = HLP_SORCERY_NODE_AURA;

        strcpy(_combat_info_effects[idx]->Name, cnst_SorceryAura);

        idx++;

    }
    else if(_combat_node_type == cnt_Nature)
    {

        _combat_info_effects[idx]->icon_seg = _combat_info_effect_icon_segs[11];

        _combat_info_effects[idx]->help_idx = HLP_NATURE_NODE_AURA;

        strcpy(_combat_info_effects[idx]->Name, cnst_NatureAura);

        idx++;

    }
    else if(_combat_node_type == cnt_Chaos)
    {

        _combat_info_effects[idx]->icon_seg = _combat_info_effect_icon_segs[6];

        _combat_info_effects[idx]->help_idx = HLP_CHAOS_NODE_AURA;

        strcpy(_combat_info_effects[idx]->Name, cnst_ChaosAura);

        idx++;

    }


    if(OVL_Action_Type == 1)
    {

        if(CMB_CloudofShadow > 0)
        {

            _combat_info_effects[idx]->icon_seg = _combat_info_effect_icon_segs[8];

            _combat_info_effects[idx]->help_idx = HLP_CLOUD_OF_DARKNESS;

            strcpy(_combat_info_effects[idx]->Name, cnst_CloudOfDarkness);

            idx++;

        }

        if(CMB_HeavenlyLight > 0)
        {

            _combat_info_effects[idx]->icon_seg = _combat_info_effect_icon_segs[2];

            _combat_info_effects[idx]->help_idx = HLP_HOLY_LIGHT;

            strcpy(_combat_info_effects[idx]->Name, cnst_HolyLight);

            idx++;

        }

    }

    for(itr = 0; itr < NUM_PLAYERS; itr++)
    {

        if(_players[itr].Globals[CHAOS_SURGE] > 0)
        {

            _combat_info_effects[idx]->icon_seg = _combat_info_effect_icon_segs[4];

            _combat_info_effects[idx]->help_idx = HLP_CHAOS_SURGE;

            strcpy(_combat_info_effects[idx]->Name, cnst_ChaosSurge);

            idx++;

        }

    }

    for(itr = 0; itr < NUM_PLAYERS; itr++)
    {

        if(_players[itr].Globals[ETERNAL_NIGHT] > 0)
        {

            _combat_info_effects[idx]->icon_seg = _combat_info_effect_icon_segs[7];

            _combat_info_effects[idx]->help_idx = HLP_ETERNAL_NIGHT;

            strcpy(_combat_info_effects[idx]->Name, cnst_EternalNight);

            idx++;

        }

    }


    if((idx % 2) != 0)
    {

        _combat_info_effects[idx]->icon_seg = (SAMB_ptr)ST_UNDEFINED;

        idx++;

    }


    _combat_info_item_count = idx;

}


// WZD s103p05
// drake178: CMB_CountExternalFX()
/*
; returns the total amount of external effects that
; affect combat on the current tile
;
; BUG: ignores Evil Omens despite its combat effect
*/
/*

¿ exact same logic as Combat_Info_Effects() and Combat_Info_Effects_Base() ?
~ Battle Effects
~ External

*/
int16_t Combat_Info_Effects_Count(void)
{
    int16_t battle_effects_count = 0;  // _CX_
    int16_t player_idx = 0;  // _DI_
    int16_t itr_combatants = 0;  // _SI_
    int16_t itr_players = 0;  // _SI_

    battle_effects_count = 0;

    player_idx = CMB_AI_Player;


    for(itr_combatants = 0; itr_combatants < 2; itr_combatants++)
    {

        if(player_idx < NEUTRAL_PLAYER_IDX)
        {

            if(_players[player_idx].Globals[CRUSADE] > 0)
            {

                battle_effects_count++;

            }

            if(_players[player_idx].Globals[HOLY_ARMS] > 0)
            {

                battle_effects_count++;

            }

            if(_players[player_idx].Globals[CHARM_OF_LIFE] > 0)
            {

                battle_effects_count++;

            }

            if(_players[player_idx].Globals[ZOMBIE_MASTERY] > 0)
            {

                battle_effects_count++;

            }

            if(_players[player_idx].Globals[CRUSADE] > 0)
            {

                battle_effects_count++;

            }

        }

        player_idx = _human_player_idx;

    }


    if(
        (battlefield->Central_Structure == CS_SorceryNode)
        ||
        (battlefield->Central_Structure == CS_ChaosNode)
        ||
        (battlefield->Central_Structure == CS_NatureNode)
    )
    {

        battle_effects_count++;

    }


    if(
        (_combat_node_type == cnt_Sorcery)
        ||
        (_combat_node_type == cnt_Nature)
        ||
        (_combat_node_type == cnt_Chaos)
    )
    {

        battle_effects_count++;

    }



    if(OVL_Action_Type == 1)
    {

        if(battlefield->city_enchantments[CLOUD_OF_SHADOW] > 0)
        {

            battle_effects_count++;

        }


        if(battlefield->city_enchantments[HEAVENLY_LIGHT] > 0)
        {

            battle_effects_count++;

        }

    }


    for(itr_players = 0; itr_players < NUM_PLAYERS; itr_players++)
    {

        if(_players[itr_players].Globals[CHAOS_SURGE] > 0)
        {

            battle_effects_count++;

        }

    }


    for(itr_players = 0; itr_players < NUM_PLAYERS; itr_players++)
    {

        if(_players[itr_players].Globals[ETERNAL_NIGHT] > 0)
        {

            battle_effects_count++;

        }

    }


    return battle_effects_count;

}


// WZD s103p06
// ¿ MoO2  Point_Is_In_Some_Nebula_() |-> Point_Is_In_Nebula_N_() ?
/*

sets _combat_node_type

*/
void Combat_Node_Type(void)
{
    int16_t itr_auras;  // _SI_
    int16_t node_type;  // _DI_
    int16_t itr_nodes;  // _CX_

    node_type = ST_UNDEFINED;

    for(itr_nodes = 0; ((itr_nodes < NUM_NODES) && (node_type == ST_UNDEFINED)); itr_nodes++)
    {
        if(_NODES[itr_nodes].wp != _combat_wp)
        {
            continue;
        }

        for(itr_auras = 0; ((itr_auras < _NODES[itr_nodes].power) && (node_type == ST_UNDEFINED)); itr_auras++)
        {
            if(
                (_NODES[itr_nodes].Aura_Xs[itr_auras] == _combat_wx)
                &&
                (_NODES[itr_nodes].Aura_Ys[itr_auras] == _combat_wy)
            )
            {
                node_type = _NODES[itr_nodes].type;
            }
        }

    }

    _combat_node_type = node_type;

}


// segrax
// WZD s103p07
// drake178: BU_CreateImage()
/*
; allocates a 2k buffer in World_Data@, into which it
; composes an image of the battle unit, saving the
; pointer to IMG_CMB_FX_Figure@ - this is drawn
; separately from the combat entity sprites
*/
/*
    does a complete composes of a new battle unit picture into GfxBuf_2400B
    does not change any game-data

~ USELESS_Combat_Figure_Load_Compose()

*/
void BU_CreateImage__SEGRAX(int battle_unit_idx)
{
    int16_t cur_fig = 0;
    int16_t offset = 0;
    // SAMB_ptr temp_figure_pict_set_seg = NULL;
    SAMB_ptr * figure_pict_set_seg = NULL;
    int16_t target_cgy = 0;
    int16_t target_cgx = 0;
    int16_t cgy = 0;
    int16_t cgx = 0;
    int16_t frame_num = 0;
    int16_t bufpi = 0;
    int16_t IMG_Effect = 0;
    int16_t enchantment_magic_realm = 0;
    int16_t player_idx = 0;
    int16_t fig_y = 0;
    int16_t fig_x = 0;
    int16_t fig_max = 0;
    int16_t fig_cnt = 0;
// _SI_battle_unit_idx = si
    int16_t figure_set_idx = 0;  // _DI_

    // struct s_BATTLE_UNIT * battle_unit = &battle_units[battle_unit_idx];

    IMG_CMB_FX_Figure = Allocate_Next_Block(World_Data, 124);  // 124 PR, 1984 B

    if(battle_units[battle_unit_idx].status != bus_Active)
    {
        return;
    }


    Create_Picture(47, 42, IMG_CMB_FX_Figure);

    
    // ~ CMB_CreateEntities__WIP()
    fig_cnt = battle_units[battle_unit_idx].Cur_Figures;
    fig_max = battle_units[battle_unit_idx].Max_Figures;
    if(_UNITS[battle_units[battle_unit_idx].unit_idx].type == spell_data_table[ut_Hydra].unit_type)
    {
        fig_cnt = 1;
        fig_max = 1;
    }
    Combat_Unit_Enchantment_Outline_Set(battle_unit_idx);  // battle_units[].sets outline_magic_realm
    BU_SetBaseAnims__WIP(battle_unit_idx);  // sets battle_units[].Always_Animate and .Move_Bob
    BU_SetVisibility__WIP(battle_unit_idx);  // sets battle_units[].Image_Effect


    // ~ Combat_Figure_Compose_USEFULL()
    bufpi = battle_units[battle_unit_idx].bufpi;

    player_idx = battle_units[battle_unit_idx].controller_idx;

    enchantment_magic_realm = battle_units[battle_unit_idx].outline_magic_realm;

    IMG_Effect = battle_units[battle_unit_idx].Image_Effect;

    if(battle_units[battle_unit_idx].Always_Animate == ST_TRUE)
    {
        frame_num = CMB_BaseAnimFrame;
    }
    else if (battle_units[battle_unit_idx].Moving == ST_TRUE)
    {
        if (battle_units[battle_unit_idx].Move_Bob == ST_TRUE)
        {
            frame_num = CMB_BaseAnimFrame;
        } 
        else
        {
            frame_num = CMB_MoveAnimFrame;
        }
    }
    else
    {
        frame_num = 1;
    }

    cgx = battle_units[battle_unit_idx].cgx;
    cgy = battle_units[battle_unit_idx].cgy;
    target_cgx = battle_units[battle_unit_idx].target_cgx;
    target_cgy = battle_units[battle_unit_idx].target_cgy;
    BATTLE_UNIT_FACING_DRECTION

    FIGUREX_MAP
    FIGUREX_OFFSET
    FIGUREX_POINTER

    figure_set_idx = 7;  // look left/west?

    FLIC_Set_CurrentFrame(figure_pict_set_seg[figure_set_idx], frame_num);
    Draw_Picture_To_Bitmap(figure_pict_set_seg[figure_set_idx], GfxBuf_2400B);

    Combat_Figure_Banner_Color(player_idx);
    Combat_Unit_Enchantment_Outline_Draw(enchantment_magic_realm);
    Combat_Figure_Effect__WIP(IMG_Effect);


    // ~ CMB_DrawEntities__WIP()
    for(cur_fig = 0; cur_fig < fig_cnt; cur_fig++)
    {
        Battle_Unit_Figure_Position(fig_max, cur_fig, &fig_x, &fig_y);
        Clipped_Copy_Bitmap(fig_x, fig_y - 4, IMG_CMB_FX_Figure, GfxBuf_2400B);
    }

}


// WZD s103p08
// drake178: WIZ_BU_SelectNext()
/*
; an awfully awkward wrapper for WIZ_BU_SelectClosest
;
; what is going on here?
*/
/*

sets _active_battle_unit, via Next_Battle_Unit_Nearest_Available(player_idx)

¿ ~== WIZ_NextIdleStack() ?

*/
void Next_Battle_Unit(int16_t player_idx)
{
    int16_t all_done_none_available = 0;
    int16_t itr_battle_units = 0;  // _SI_
    int16_t done = 0;  // _DI_

    all_done_none_available = ST_FALSE;

    // ; BUG? why is this in a loop?
    done = ST_FALSE;
    while(done == ST_FALSE)
    {

        all_done_none_available = Next_Battle_Unit_Nearest_Available(player_idx);

        if(all_done_none_available == ST_TRUE)
        {
            
            CMB_HumanUnitsDone = ST_TRUE;
        }
        else
        {
            CMB_ImmobileCanAct = ST_FALSE;
        }

        done = ST_TRUE;

    }


    // ; isn't this an overland variable?
    if(all_units_moved == ST_FALSE)
    {

        // ; completely redundant, the variable is already zero
        // ; if these conditions are met

        if(
            (battle_units[_active_battle_unit].movement_points > 0)
            &&
            (battle_units[_active_battle_unit].controller_idx == combat_human_player)
        )
        {

            CMB_ImmobileCanAct = ST_FALSE;

        }

    }


    if(battle_units[_active_battle_unit].controller_idx != combat_human_player)
    {

        for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
        {

            if(battle_units[itr_battle_units].controller_idx == _combat_attacker_player)
            {

                _active_battle_unit = itr_battle_units;

            }

        }

    }

}


// WZD s103p09
// drake178: WIZ_BU_SelectClosest()
/*
; selects from the specified player's active battle
; units the closest to the one currently selected - if
; all units are waiting, it will mark them all as ready
; instead in the process
; returns 0 if successful, or 1 if no units have
; movement left (in which case no selection will be
; made either)
*/
/*
    sets _active_battle_unit, via Switch_Active_Battle_Unit()
    returns ST_TRUE if all done / none available

¿ ~== UNITSTK.C  int16_t Next_Unit_Nearest_Available(int16_t player_idx, int16_t * map_plane) ?

*/
int16_t Next_Battle_Unit_Nearest_Available(int16_t player_idx)
{
    int16_t Selected_Unit_Y = 0;
    int16_t Selected_Unit_X = 0;
    int16_t Processed_Units = 0;
    int16_t uu_flag = 0;
    int16_t Unit_Count = 0;
    int16_t Closest_Active_Unit = 0;
    int16_t Closest_Active_Dist = 0;
    int16_t Closest_Waiting_Unit = 0;
    int16_t Closest_Waiting_Dist = 0;
    int16_t battle_unit_ctr = 0;
    int16_t delta = 0;
    int16_t all_done_none_available = 0;
    int16_t done = 0;
    int16_t itr = 0;  // _SI_
    int16_t battle_unit_idx = 0;  // _DI_

    Selected_Unit_X = battle_units[_active_battle_unit].cgx;

    Selected_Unit_Y = battle_units[_active_battle_unit].cgy;

    Unit_Count = 0;

    for(itr = 0; itr < _combat_total_unit_count; itr++)
    {

        if(
            (battle_units[itr].status == bus_Active)
            &&
            (battle_units[itr].controller_idx == player_idx)
        )
        {

            Unit_Count++;

        }

    }

    Closest_Waiting_Dist = 1000;
    Closest_Waiting_Unit = ST_UNDEFINED;
    Closest_Active_Dist = 1000;
    Closest_Active_Unit = ST_UNDEFINED;

    uu_flag = ST_UNDEFINED;

    all_done_none_available = ST_FALSE;

    done = ST_FALSE;

    battle_unit_ctr = 0;

    Processed_Units = 0;

    while(done == ST_FALSE)
    {

        battle_unit_idx = battle_unit_ctr;

        if(
            (battle_units[battle_unit_idx].status == bus_Active)
            &&
            (battle_units[battle_unit_idx].controller_idx == player_idx)
        )
        {

            if(battle_units[battle_unit_idx].movement_points > 0)
            {

                delta = Delta_XY_With_Wrap(Selected_Unit_X, Selected_Unit_Y, battle_units[battle_unit_idx].cgx, battle_units[battle_unit_idx].cgy, WORLD_WIDTH);

                if(battle_units[battle_unit_idx].action == bua_Wait)
                {
                    if(Closest_Waiting_Dist > delta)
                    {
                        Closest_Waiting_Dist = delta;
                        Closest_Waiting_Unit = battle_unit_idx;
                    }
                }
                else
                {

                    if(Closest_Active_Dist > delta)
                    {
                        Closest_Active_Dist = delta;
                        Closest_Active_Unit = battle_unit_idx;
                    }

                }

            }

            Processed_Units++;            

        }

        battle_unit_ctr++;

        // processed unit count == total unit count
        if(Processed_Units == Unit_Count)
        {

            if(Closest_Active_Unit != ST_UNDEFINED)
            {

                done = ST_TRUE;

                Switch_Active_Battle_Unit(Closest_Active_Unit);

            }
            else if(Closest_Waiting_Unit != ST_UNDEFINED)
            {

                Switch_Active_Battle_Unit(Closest_Waiting_Unit);

                done = ST_TRUE;

                for(itr = 0; itr < _combat_total_unit_count; itr++)
                {

                    if(battle_units[itr].action == bua_Wait)
                    {

                        battle_units[itr].action = bua_Ready;

                    }

                }

            }
            else
            {

                done = ST_TRUE;

                all_done_none_available = ST_TRUE;

            }

        }

    }

    return all_done_none_available;

}


// WZD s103p10
// drake178: CMB_SpellcastError()
/*
; displays a warning dialog for not being able to cast
; combat spells due to either having cast one already
; this turn (1), or not having any available that can
; be cast in the current situation (2)
*/
/*

*/
void Combat_Cast_Spell_Error(int16_t type)
{
    Clear_Fields();
    switch(type)
    {
        case 1:
        {
            Warn1(cnst_CmbCastError_1);  // "You may only cast once per turn."
        } break;
        case 2:
        {
            Warn1(cnst_CmbCastError_2);  // "You are unable to throw spells at this time."
        } break;
    }
}


// WZD s103p11
// drake178: BU_MoveConfused()
/*
; moves the unit to a random destination within its
; reach, if one can be found in 600 random attempts,
; but zeroes movement allowance and marks the unit as
; finished in any case regardless
*/
/*

*/
void BU_MoveConfused__WIP(int16_t battle_unit_idx)
{



}


// WZD s103p12
// drake178: CMB_LoadResources()
/*
; creates the rest of the combat sandbox allocations,
; loads COMPIX image resources, sets the battle figure
; image pointers, and calls CMB_SetNearAllocs
; uses roughly 990h paragraphs of the sandbox, of which
; 3f2h are the figure images, which are undone in the
; end, and have to be reallocated whenever their space
; is used for something else
;
; WARNING: the aura arrays are larger than necessary
*/
/*

¿ ~== Strategic_Combat_Allocate() ?

Strategic_Combat_Allocate() does Allocate_First_Block() for battle_units.

*/
void CMB_LoadResources__WIP(void)
{
    int16_t itr;  // _SI_

    // 249 * 16 = 3984 / 110 = 36.2182 ... 36? 2 * max stack = 18 so, ...?
    // battle_units = SA_MK_FP0(Allocate_Next_Block(_screen_seg, 249));
    battle_units = (struct s_BATTLE_UNIT *)Allocate_Next_Block(_screen_seg, 249);

    CMB_IDK_4PR = (int16_t *)Allocate_Next_Block(_screen_seg, 4);  // header + sub-header + 2 PR ... or header + 3 sub-headers?

    CMB_HolyBonusArray  = (int16_t *)Allocate_Next_Block(_screen_seg, 6);
    CMB_ResistAllArray  = (int16_t *)Allocate_Next_Block(_screen_seg, 6);
    CMB_LeadershipArray = (int16_t *)Allocate_Next_Block(_screen_seg, 6);

    _cmbt_spell_button_seg = LBX_Reload_Next(compix_lbx_file__ovr103, 1, _screen_seg);

    _cmbt_wait_button_seg = LBX_Reload_Next(compix_lbx_file__ovr103, 2, _screen_seg);

    _cmbt_done_button_seg = LBX_Reload_Next(compix_lbx_file__ovr103, 3, _screen_seg);

    _cmbt_auto_button_seg = LBX_Reload_Next(compix_lbx_file__ovr103, 4, _screen_seg);

    // COMPIX.LBX, 020  "CMBTBUTT"   "use button"
    // COMPIX.LBX, 021  "CMBTBUTT"   "flee butotn"
    _cmbt_info_button_seg = LBX_Reload_Next(compix_lbx_file__ovr103, 20, _screen_seg);
    _cmbt_flee_button_seg = LBX_Reload_Next(compix_lbx_file__ovr103, 21, _screen_seg);


    // 3 * 16 = 48 ... 1PR header, 15 2-byte values
    // combat_enchantments = (struct s_COMBAT_ENCHANTMENTS *)Allocate_Next_Block(_screen_seg, 3);
    combat_enchantments = (int8_t *)Allocate_Next_Block(_screen_seg, 3);

    for(itr = 0; itr < 30; itr++)
    {
        // *((char *)&combat_enchantments[itr]) = ST_FALSE;
        combat_enchantments[itr] = ST_FALSE;
    }



    // IMG_CMB_TrueLight = LBX_Reload_Next(compix_lbx_file__ovr103, 5, _screen_seg);
    // IMG_CMB_Darkness = LBX_Reload_Next(compix_lbx_file__ovr103, 6, _screen_seg);
    // IMG_CMB_WarpReality = LBX_Reload_Next(compix_lbx_file__ovr103, 7, _screen_seg);
    // IMG_CMB_BlackPrayer = LBX_Reload_Next(compix_lbx_file__ovr103, 8, _screen_seg);
    // IMG_CMB_Wrack = LBX_Reload_Next(compix_lbx_file__ovr103, 9, _screen_seg);
    // IMG_CMB_MetalFires = LBX_Reload_Next(compix_lbx_file__ovr103, 10, _screen_seg);
    // IMG_CMB_Prayer = LBX_Reload_Next(compix_lbx_file__ovr103, 11, _screen_seg);
    // IMG_CMB_HighPrayer = LBX_Reload_Next(compix_lbx_file__ovr103, 12, _screen_seg);
    // IMG_CMB_Terror = LBX_Reload_Next(compix_lbx_file__ovr103, 13, _screen_seg);
    // IMG_CMB_CallLightng = LBX_Reload_Next(compix_lbx_file__ovr103, 14, _screen_seg);
    // IMG_CMB_CounterMgc = LBX_Reload_Next(compix_lbx_file__ovr103, 15, _screen_seg);
    // IMG_CMB_MassInvis = LBX_Reload_Next(compix_lbx_file__ovr103, 41, _screen_seg);
    // IMG_CMB_Entangle = LBX_Reload_Next(compix_lbx_file__ovr103, 60, _screen_seg);
    // IMG_CMB_ManaLeak = LBX_Reload_Next(compix_lbx_file__ovr103, 79, _screen_seg);
    // IMG_CMB_Blur = LBX_Reload_Next(compix_lbx_file__ovr103, 80, _screen_seg);

    // COMPIX.LBX, 005  "COMENCHI"   "light"
    // COMPIX.LBX, 006  "COMENCHI"   "darkness"
    // COMPIX.LBX, 007  "COMENCHI"   "warp reality"
    // COMPIX.LBX, 008  "COMENCHI"   "black curse"
    // COMPIX.LBX, 009  "COMENCHI"   "wrack"
    // COMPIX.LBX, 010  "COMENCHI"   "metal fires"
    // COMPIX.LBX, 011  "COMENCHI"   "prayer"
    // COMPIX.LBX, 012  "COMENCHI"   "high prayer"
    // COMPIX.LBX, 013  "COMENCHI"   "terror"
    // COMPIX.LBX, 014  "COMENCHI"   "lightning"
    // COMPIX.LBX, 015  "COMENCHI"   "counter magic"
    // COMPIX.LBX, 041  "MASINVIS"   "mass invis"
    // COMPIX.LBX, 060  "COMENCHI"   "entangle icon"
    // COMPIX.LBX, 079  "COMENCHI"   "mana leek"
    // COMPIX.LBX, 080  "COMENCHI"   "blur"

    combat_enchantment_icon_segs[ 0] = LBX_Reload_Next(compix_lbx_file__ovr103,  5, _screen_seg);
    combat_enchantment_icon_segs[ 1] = LBX_Reload_Next(compix_lbx_file__ovr103,  6, _screen_seg);
    combat_enchantment_icon_segs[ 2] = LBX_Reload_Next(compix_lbx_file__ovr103,  7, _screen_seg);
    combat_enchantment_icon_segs[ 3] = LBX_Reload_Next(compix_lbx_file__ovr103,  8, _screen_seg);
    combat_enchantment_icon_segs[ 4] = LBX_Reload_Next(compix_lbx_file__ovr103,  9, _screen_seg);
    combat_enchantment_icon_segs[ 5] = LBX_Reload_Next(compix_lbx_file__ovr103, 10, _screen_seg);
    combat_enchantment_icon_segs[ 6] = LBX_Reload_Next(compix_lbx_file__ovr103, 11, _screen_seg);
    combat_enchantment_icon_segs[ 7] = LBX_Reload_Next(compix_lbx_file__ovr103, 12, _screen_seg);
    combat_enchantment_icon_segs[ 8] = LBX_Reload_Next(compix_lbx_file__ovr103, 13, _screen_seg);
    combat_enchantment_icon_segs[ 9] = LBX_Reload_Next(compix_lbx_file__ovr103, 14, _screen_seg);
    combat_enchantment_icon_segs[10] = LBX_Reload_Next(compix_lbx_file__ovr103, 15, _screen_seg);
    combat_enchantment_icon_segs[11] = LBX_Reload_Next(compix_lbx_file__ovr103, 41, _screen_seg);
    combat_enchantment_icon_segs[12] = LBX_Reload_Next(compix_lbx_file__ovr103, 60, _screen_seg);
    combat_enchantment_icon_segs[13] = LBX_Reload_Next(compix_lbx_file__ovr103, 79, _screen_seg);
    combat_enchantment_icon_segs[14] = LBX_Reload_Next(compix_lbx_file__ovr103, 80, _screen_seg);


    // COMPIX.LBX, 022  "CMBTBUTT"   "cancel button"
    _cmbt_cancel_button_seg = LBX_Reload_Next(compix_lbx_file__ovr103, 22, _screen_seg);

    // COMPIX.LBX, 023  "LOCKBUTT"   "spell lock"
    // COMPIX.LBX, 024  "LOCKBUTT"   "wait lock"
    // COMPIX.LBX, 028  "LOCKBUTT"   "done lock"
    // COMPIX.LBX, 026  "LOCKBUTT"   "auto lock"
    // COMPIX.LBX, 025  "LOCKBUTT"   "use lock"
    // COMPIX.LBX, 027  "LOCKBUTT"   "flee lock"
    _cmbt_lock_spell_button_seg = LBX_Reload_Next(compix_lbx_file__ovr103, 23, _screen_seg);
    _cmbt_lock_wait_button_seg  = LBX_Reload_Next(compix_lbx_file__ovr103, 24, _screen_seg);
    _cmbt_lock_done_button_seg  = LBX_Reload_Next(compix_lbx_file__ovr103, 28, _screen_seg);
    _cmbt_lock_auto_button_seg  = LBX_Reload_Next(compix_lbx_file__ovr103, 26, _screen_seg);
    _cmbt_lock_info_button_seg  = LBX_Reload_Next(compix_lbx_file__ovr103, 25, _screen_seg);
    _cmbt_lock_flee_button_seg  = LBX_Reload_Next(compix_lbx_file__ovr103, 27, _screen_seg);

    // COMPIX.LBX, 029  "SMALICON"   "sword"
    // COMPIX.LBX, 030  "SMALICON"   "fireball"
    // COMPIX.LBX, 031  "SMALICON"   "bless"
    // COMPIX.LBX, 032  "SMALICON"   "magic"
    // COMPIX.LBX, 033  "SMALICON"   "adam"
    // COMPIX.LBX, 034  "SMALICON"   "bow"
    // COMPIX.LBX, 035  "SMALICON"   "rock"
    // COMPIX.LBX, 036  "SMALICON"   "breath"
    // COMPIX.LBX, 037  "SMALICON"   "axe"

    for(itr = 0; itr < 9; itr++)
    {

        combat_weapon_icon_segs[itr] = LBX_Reload_Next(compix_lbx_file__ovr103, (29 + itr), _screen_seg);

    }

    // COMPIX.LBX, 038  "SMALICON"   "walk"
    // COMPIX.LBX, 039  "SMALICON"   "fly"
    // COMPIX.LBX, 040  "SMALICON"   "swim"

    for(itr = 0; itr < 3; itr++)
    {

        combat_movemode_icon_segs[itr] = LBX_Reload_Next(compix_lbx_file__ovr103, (38 + itr), _screen_seg);

    }


    // COMPIX.LBX, 061  "SCANICON"   "sword"
    // COMPIX.LBX, 062  "SCANICON"   "fireball"
    // COMPIX.LBX, 063  "SCANICON"   "bless"
    // COMPIX.LBX, 064  "SCANICON"   "magic"
    // COMPIX.LBX, 065  "SCANICON"   "adam"
    // COMPIX.LBX, 066  "SCANICON"   "bow"
    // COMPIX.LBX, 067  "SCANICON"   "rock"
    // COMPIX.LBX, 068  "SCANICON"   "breath"
    // COMPIX.LBX, 069  "SCANICON"   "axe"
    // COMPIX.LBX, 070  "SCANICON"   "defense"
    // COMPIX.LBX, 071  "SCANICON"   "health"
    // COMPIX.LBX, 072  "SCANICON"   "walk"
    // COMPIX.LBX, 073  "SCANICON"   "fly"
    // COMPIX.LBX, 074  "SCANICON"   "swim"
    // COMPIX.LBX, 075  "SCANICON"   "resistance"
    // COMPIX.LBX, 076  "SCANICON"   "silver"
    // COMPIX.LBX, 077  "SCANICON"   "gold"
    // COMPIX.LBX, 078  "SCANICON"   "red"

    for(itr = 0; itr < 18; itr++)
    {

        // IMG_CMB_ScanIcons[itr].Stat.Melee = LBX_Reload_Next(compix_lbx_file__ovr103, (61 + itr), _screen_seg);
        IMG_CMB_ScanIcons[itr] = LBX_Reload_Next(compix_lbx_file__ovr103, (61 + itr), _screen_seg);

    }


    // COMPIX.LBX, 018  "HITBAR"     "unit hit bar"
    unit_hit_bar_seg = LBX_Reload_Next(compix_lbx_file__ovr103, 18, _screen_seg);


    Mark_Block(_screen_seg);

    for(itr = 0; itr < MAX_BATTLE_UNIT_COUNT; itr++)
    {

        battle_unit_picts_seg[itr] = Allocate_Next_Block(_screen_seg, 55);  // 55 PR  880 B

    }

    Release_Block(_screen_seg);


    CMB_SetNearAllocs__WIP();

}


// WZD s103p13
// drake178: CMB_SetNearAllocs()
/*
; resets the dynamic near memory allocation (in the
; data segment), and recreates the resident combat
; buffers into it (2630 bytes allocated in total)
*/
/*

*/
void CMB_SetNearAllocs__WIP(void)
{
    int16_t itr = 0;  // _SI_

    CMB_ActiveMoveMap = (int8_t *)Near_Allocate_First(504);

    CMB_Path_Costs = (uint8_t *)Near_Allocate_Next(504);

    CMB_NearBuffer_3 = (int16_t *)Near_Allocate_Next(1008);  // 504 2-byte values

    CMB_Path_Xs = Near_Allocate_Next(60);

    CMB_Path_Ys = Near_Allocate_Next(60);

    _combat_mouse_grid = (struct s_mouse_list *)Near_Allocate_Next(12);

    for(itr = 0; itr < COMBAT_GRID_HEIGHT; itr++)
    {

        CMB_TargetRows[itr] = (int8_t *)Near_Allocate_Next(COMBAT_GRID_WIDTH);

    } 

    GUI_String_1 = (char *)Near_Allocate_Next(20);

}


// WZD s103p14
// drake178: CMB_SelectCaster()
/*
; shows the combat spell caster selection dialog and,
; if a valid caster is chosen, proceeds directly to
; calling and resolving the CMB_CastSpell function
*/
/*

*/
void Combat_Cast_Spell_With_Caster(int16_t caster_id)
{
    char * selection_list_text[4] = { 0, 0, 0, 0 };
    int16_t selection_list_idx = 0;
    int16_t cast_status = 0;  // _SI_

    if(CMB_WizCastAvailable != ST_TRUE)
    {
        cast_status = Combat_Cast_Spell__WIP(caster_id, _combat_wx, _combat_wy, _combat_wp);
    }
    else
    {
        selection_list_text[0] = _players[HUMAN_PLAYER_IDX].name;
        selection_list_text[1] = _unit_type_table[_UNITS[battle_units[caster_id].unit_idx].type].name;
        selection_list_text[2] = str_cancel__ovr103;
        selection_list_text[3] = str_empty_string__ovr103;
        Clear_Fields();
        selection_list_idx = Selection_Box(3, &selection_list_text[0], 0, cnst_CasterSelectMsg);  // "Who Will Cast"
        if(selection_list_idx == 0)  /* Player */
        {
            cast_status = Combat_Cast_Spell__WIP((20 + _human_player_idx), _combat_wx, _combat_wy, _combat_wp);
            if(cast_status == 2)
            {
                CMB_WizCastAvailable = ST_FALSE;
            }
        }
        else if(selection_list_idx == 1)  /* Battle Unit */
        {
            cast_status = Combat_Cast_Spell__WIP(caster_id, _combat_wx, _combat_wy, _combat_wp);
        }
        else  /* Cancel */
        {
            cast_status = 1;
        }
    }
    if(cast_status == ST_FALSE)
    {
        Combat_Cast_Spell_Error(2);  // "You are unable to throw spells at this time."
    }
    CMB_ComposeBackgrnd__WIP();  // ... |-> Copy_Off_To_Back();
}



/*
    WIZARDS.EXE  ovr105
*/

// WZD o105p01
// drake178: CMB_WinLoseFlee()
// MoO2  Module: COMBAT1  Check_For_Winner_()
/*
; checks whether either side has no units left on the
; battlefield, and if the AI player is not the neutral
; one, checks for fight or flight, and will flee if it
; finds the situation hopeless and has units to save
; returns the index of the victorious player, if any,
; or -1 if the battle can continue
*/
/*

*/
int16_t Check_For_Winner__WIP(void)
{
    int16_t defender_count = 0;
    int16_t itr = 0;  // _SI_
    int16_t attacker_count = 0;  // _DI_

    attacker_count = 0;

    defender_count = 0;

    for(itr = 0; itr < _combat_total_unit_count; itr++)
    {

        if(battle_units[itr].status == bus_Active)
        {

            if(battle_units[itr].controller_idx == _combat_attacker_player)
            {

                if(battle_units[itr].Confusion_State == 2)  // ; BUG: never checks for the actual effect
                {

                    defender_count++;

                }
                else
                {

                    attacker_count++;

                }

            }
            else
            {

                if(battle_units[itr].controller_idx == _combat_defender_player)
                {

                    if(battle_units[itr].Confusion_State == 2)  // ; BUG: never checks for the actual effect
                    {

                        attacker_count++;

                    }
                    else
                    {

                        defender_count++;

                    }

                }

            }

        }

    }


    if(attacker_count == 0)
    {

        return _combat_defender_player;

    }

    if(defender_count == 0)
    {

        return _combat_attacker_player;

    }

    if(_combat_turn > 50)
    {

        return _combat_defender_player;

    }

    if(combat_computer_player == NEUTRAL_PLAYER_IDX)
    {

        return ST_UNDEFINED;

    }


    // TODO  flip this logic around the the fleeing code is wrapped in the tests and the default/fall-through is return ST_UNDEFINED
    if(
        !(CMB_WizardCitySiege == ST_FALSE)
        ||
        !(_combat_turn > 1)
        ||
        !(AI_FightorFlight__STUB(combat_computer_player) == ST_TRUE)
    )
    {

        return ST_UNDEFINED;

    }


    for(itr = 0; itr < _combat_total_unit_count; itr++)
    {

        if(battle_units[itr].controller_idx == combat_computer_player)
        {


            battle_units[itr].action = BUA_Flee;

            if(battle_units[itr].status == bus_Active)
            {

                battle_units[itr].status = bus_Fleeing;

            }

        }

    }

    CMB_AI_Fled = ST_TRUE;

    return combat_human_player;

}


// WZD o105p02
// drake178: LBX_IMG_BannerPaint()
/*
; performs banner painting on whatever image is loaded
; into the GUI_SmallWork_IMG@ work area, typically a
; combat figure, using the banner color of the
; specified player
*/
/*

same as seen in UnitView.C Draw_Unit_Figure()

*/
void Combat_Figure_Banner_Color(int16_t player_idx)
{
    int16_t unit_owner_banner_idx = 0;
    int16_t itr_banner_colors = 0;  // _SI_

    unit_owner_banner_idx = _players[_human_player_idx].banner_id;

    for(itr_banner_colors = 0; itr_banner_colors < 5; itr_banner_colors++)
    {
        // Draw_Map_Towers()
        // Replace_Color(Map_Square_WorkArea, 224 + itr_color_remap, *(COL_Banners + (_players[tower_owner_idx].banner_id * 5)));
        Replace_Color(GfxBuf_2400B, (214 + itr_banner_colors), COL_Banners[((unit_owner_banner_idx * 5) + itr_banner_colors)]);
    }

}


// WZD o105p03
// drake178: LBX_IMG_Highlight()
/*
; if the passed BU index matches that of the currently
; highlighted combat unit, replaces the black palette
; colors ($01) in the figure loaded into the GUI small
; image work area with the blink color ($C6)
*/
/*

Page 92  (PDF Page 97)
"(a red outline surrounds the active unit’s square on the combat grid)"
*/
void Combat_Figure_Active_Red_Outline(int16_t battle_unit_idx)
{

    if(_scanned_battle_unit == battle_unit_idx)
    {

        Replace_Color(GfxBuf_2400B, 1, 198);

    }

}


// WZD o105p04
// drake178: LBX_IMG_FX()
/*
; applies a special effect to whatever image is loaded
; into the GUI_SmallWork_IMG@ allocation, typically a
; battle unit figure:
;   1 - greyscale (Black Sleep)
;   2 - bluescale (unused?)
;   3 - redscale (Warp Creature)
;   4 - clear (Invisibility, spotted)
;   5 - strip (Invisibility, undetected)
*/
/*

*/
void Combat_Figure_Effect__WIP(int16_t effect)
{

    FLIC_Set_LoopFrame_1(GfxBuf_2400B);

    switch(effect)
    {

        case 1:
        {

            Transparent_Color_Range(GfxBuf_2400B, 232, 232);

            Gray_Scale_Bitmap(GfxBuf_2400B, 1);

        } break;

        case 2:
        {

            Transparent_Color_Range(GfxBuf_2400B, 232, 232);

            /* TODO */  LBX_IMG_RevGrayscale__STUB(GfxBuf_2400B, 104);

        } break;

        case 3:
        {

            Transparent_Color_Range(GfxBuf_2400B, 232, 232);

            /* TODO */  LBX_IMG_RevGrayscale__STUB(GfxBuf_2400B, 40);

        } break;

        case 4:
        {

            Transparent_Color_Range(GfxBuf_2400B, 232, 232);

            Replace_Color_All(GfxBuf_2400B, 233);

        } break;

        case 5:
        {

            Transparent_Color_Range(GfxBuf_2400B,   1, 228);

            Transparent_Color_Range(GfxBuf_2400B, 232, 232);

        } break;

    }

}


// WZD o105p05
// drake178: BU_SetUEColor()
/*

sets battle_units[].outline_magic_realm

Combat_Unit_Enchantment_Outline_Set() + Combat_Unit_Enchantment_Outline_Draw() is the same as Draw_Unit_Enchantment_Outline() from 'Main Screen'

*/
void Combat_Unit_Enchantment_Outline_Set(int16_t battle_unit_idx)
{
    int16_t Combat_Effects = 0;
    uint32_t unit_enchantments = 0;
    int8_t enchantment_magic_realm = 0;  // _SI_

    uint8_t * color_list = 0;

    enchantment_magic_realm = ST_UNDEFINED;

    unit_enchantments = _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments;

    Combat_Effects = battle_units[battle_unit_idx].Combat_Effects;

    if(
        ((unit_enchantments & UE_INVISIBILITY) != 0)
        ||
        ((battle_units[battle_unit_idx].Abilities & UA_INVISIBILITY) != 0)
    )
    {

        unit_enchantments = 0;

        Combat_Effects = 0;

    }

    if(
        (unit_enchantments != 0)
        ||
        (Combat_Effects != 0)
    )
    {

        if((unit_enchantments & UE_REGENERATION) != 0)
        {
            enchantment_magic_realm = mr_Nature;
        }
        else if((unit_enchantments & UE_RESISTMAGIC) != 0)
        {
            enchantment_magic_realm = mr_Sorcery;
        }
        else if((unit_enchantments & UE_RIGHTEOUSNESS) != 0)
        {
            enchantment_magic_realm = mr_Life;
        }
        else if((unit_enchantments & UE_PLANARTRAVEL) != 0)
        {
            enchantment_magic_realm = mr_Life;
        }
        else if((Combat_Effects & bue_Haste) != 0)
        {
            enchantment_magic_realm = mr_Sorcery;
        }
        else if((unit_enchantments & UE_BLACKCHANNELS) != 0)
        {
            enchantment_magic_realm = mr_Death;
        }
        else if((unit_enchantments & UE_LIONHEART) != 0)
        {
            enchantment_magic_realm = mr_Life;
        }
        else if((unit_enchantments & UE_IRONSKIN) != 0)
        {
            enchantment_magic_realm = mr_Nature;
        }
        else if((unit_enchantments & UE_MAGICIMMUNITY) != 0)
        {
            enchantment_magic_realm = mr_Sorcery;
        }
        else if((unit_enchantments & UE_WINDWALKING) != 0)
        {
            enchantment_magic_realm = mr_Sorcery;
        }
        else if((unit_enchantments & UE_ELEMENTALARMOR) != 0)
        {
            enchantment_magic_realm = mr_Nature;
        }
        else if((unit_enchantments & UE_TRUESIGHT) != 0)
        {
            enchantment_magic_realm = mr_Life;
        }
        else if((unit_enchantments & UE_INVULNERABILITY) != 0)
        {
            enchantment_magic_realm = mr_Life;
        }
        else if((unit_enchantments & UE_PATHFINDING) != 0)
        {
            enchantment_magic_realm = mr_Nature;
        }
        else if((unit_enchantments & UE_FLIGHT) != 0)
        {
            enchantment_magic_realm = mr_Sorcery;
        }
        else if((unit_enchantments & UE_ELDRITCHWEAPON) != 0)
        {
            enchantment_magic_realm = mr_Chaos;
        }
        else if((unit_enchantments & UE_WRAITHFORM) != 0)
        {
            enchantment_magic_realm = mr_Death;
        }
        else if((unit_enchantments & UE_GIANTSTRENGTH) != 0)
        {
            enchantment_magic_realm = mr_Nature;
        }
        else if((unit_enchantments & UE_IMMOLATION) != 0)
        {
            enchantment_magic_realm = mr_Chaos;
        }
        else if((unit_enchantments & UE_GUARDIANWIND) != 0)
        {
            enchantment_magic_realm = mr_Sorcery;
        }
        else if((unit_enchantments & UE_SPELLLOCK) != 0)
        {
            enchantment_magic_realm = mr_Sorcery;
        }
        else if((unit_enchantments & UE_HEROISM) != 0)
        {
            enchantment_magic_realm = mr_Life;
        }
        else if((unit_enchantments & UE_BERSERK) != 0)
        {
            enchantment_magic_realm = mr_Death;
        }
        else if((unit_enchantments & UE_HOLYARMOR) != 0)
        {
            enchantment_magic_realm = mr_Life;
        }
        else if((unit_enchantments & UE_WATERWALKING) != 0)
        {
            enchantment_magic_realm = mr_Nature;
        }
        else if((unit_enchantments & UE_ENDURANCE) != 0)
        {
            enchantment_magic_realm = mr_Life;
        }
        else if((unit_enchantments & UE_CLOAKOFFEAR) != 0)
        {
            enchantment_magic_realm = mr_Death;
        }
        else if((unit_enchantments & UE_STONESKIN) != 0)
        {
            enchantment_magic_realm = mr_Nature;
        }
        else if((unit_enchantments & UE_FLAMEBLADE) != 0)
        {
            enchantment_magic_realm = mr_Chaos;
        }
        else if((unit_enchantments & UE_BLESS) != 0)
        {
            enchantment_magic_realm = mr_Life;
        }
        else if((unit_enchantments & UE_HOLYWEAPON) != 0)
        {
            enchantment_magic_realm = mr_Life;
        }
        else if((unit_enchantments & UE_RESISTELEMENTS) != 0)
        {
            enchantment_magic_realm = mr_Nature;
        }
    }

    battle_units[battle_unit_idx].outline_magic_realm = enchantment_magic_realm;

}


// WZD o105p06
// drake178: LBX_IMG_UE_Outline()
/*
*/
/*

Combat_Unit_Enchantment_Outline_Set() + Combat_Unit_Enchantment_Outline_Draw() is the same as Draw_Unit_Enchantment_Outline() from 'Main Screen'

here    GfxBuf_2400B
there   UnitDraw_WorkArea

*/
void Combat_Unit_Enchantment_Outline_Draw(int16_t enchantment_magic_realm)
{

    if(enchantment_magic_realm != ST_UNDEFINED)
    {

        Outline_Bitmap_Pixels_No_Glass(GfxBuf_2400B, 255);

        Bitmap_Aura_Pixels(GfxBuf_2400B, 255, &enchantment_outline_colors[enchantment_magic_realm][unit_enchantment_animation_count][0]);

    }

}


// WZD o105p07
// drake178: CMB_UpdateTrueSight()
/*
; updates both the attacker's and defender's
; invisibility detection variables
*/
/*
¿ attkr/dfndr has *true sight* if any unit has immunity to illusions ?
¿ e.i., can you always see all invisible units if any or your units have 'Immunity to Illusions' ?

True Sight:
Endows a target normal unit with the gift of immunity to illusions
  and allows the unit to see through wall of darkness.

Wall of Darkness:
Surrounds a city with a wall of utter night, protecting the garrison from ranged attacks
  and preventing all enemy units (without true sight or immunity to illusions) from peering in.

Immunity to Illusion
    Negates Illusory bonus of Illusory Attacks;
    Unit is unaffected by Illusory spells: mind fires, vertigo, word of command;
    Unit can see Invisible enemies.

*/
void CMB_UpdateTrueSight(void)
{
    int16_t itr_battle_units;  // _CX_

    CMB_ATKR_TrueSight = ST_FALSE;
    CMB_DEFR_TrueSight = ST_FALSE;

    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
    {
        if(battle_units[itr_battle_units].status == bus_Active)
        {
            if(battle_units[itr_battle_units].controller_idx == _combat_attacker_player)
            {
                if((battle_units[itr_battle_units].Attribs_1 & USA_IMMUNITY_ILLUSION) != 0)
                {
                    CMB_ATKR_TrueSight = ST_TRUE;
                }
            }
            else
            {
                if(battle_units[itr_battle_units].controller_idx == _combat_defender_player)
                {
                    if((battle_units[itr_battle_units].Attribs_1 & USA_IMMUNITY_ILLUSION) != 0)
                    {
                        CMB_DEFR_TrueSight = ST_TRUE;
                    }
                }
            }
        }
    }

}


// WZD o105p08
// drake178: BU_SetVisibility()
/*
; checks the unit's visibility state and modifies the
; BU image effect field accordingly if necessary
*/
/*

sets battle_units[].Image_Effect

*/
void BU_SetVisibility__WIP(int16_t battle_unit_idx)
{
    int16_t Visible = 0;
    int16_t Tile_Distance = 0;
    int16_t owner_idx = 0;
    uint32_t enchantments = 0;
    int16_t itr = 0;  // _DI_

    owner_idx = battle_units[battle_unit_idx].controller_idx;

    enchantments = (battle_units[battle_unit_idx].enchantments | _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments);

    Visible = ST_TRUE;

    if(
        ((enchantments & UE_INVISIBILITY) != 0)
        ||
        ((battle_units[battle_unit_idx].Abilities & UA_INVISIBILITY) != 0)
    )
    {

        Visible = ST_FALSE;

        if(
            (owner_idx == _combat_defender_player)
            &&
            (CMB_ATKR_TrueSight == ST_TRUE)
        )
        {

            Visible == ST_TRUE;

            battle_units[battle_unit_idx].Image_Effect = 4;

        }

        if(owner_idx == combat_human_player)
        {

            Visible == ST_TRUE;

            battle_units[battle_unit_idx].Image_Effect = 4;

        }

        if(Visible == ST_FALSE)
        {

            battle_units[battle_unit_idx].Image_Effect = 5;

            for(itr = 0; itr < _combat_total_unit_count; itr++)
            {

                if(
                    (battle_units[itr].status == bus_Active)
                    &&
                    (battle_units[itr].controller_idx != owner_idx)
                )
                {

                    Tile_Distance = Range_To_Battle_Unit(battle_unit_idx, itr);

                    if(Tile_Distance < 2)
                    {

                        Visible = ST_TRUE;

                        battle_units[battle_unit_idx].Image_Effect = 4;

                    }

                }

            }

        }

    }

}


// WZD o105p09
// drake178: BU_GetCombatEffect()

// WZD o105p10
// drake178: BU_SetBaseAnims()
/*
; sets the BU fields controlling whether the unit is
; animated when moving and when not
*/
/*
    sets battle_units[].Always_Animate and .Move_Bob

*/
void BU_SetBaseAnims__WIP(int16_t battle_unit_idx)
{
    int16_t Flight_Bob = 0;
    int16_t animate = 0;  // _DI_

    Flight_Bob = ST_FALSE;

    animate = ST_FALSE;

    if((battle_units[battle_unit_idx].Attribs_1 & USA_FLYING) != 0)
    {

        Flight_Bob = ST_TRUE;

        animate = BU_CheckFlight__WIP(battle_unit_idx, 0);  // ST_TRUE if flying and not web or black sleep

    }

    if(
        (_UNITS[battle_units[battle_unit_idx].unit_idx].type == spell_data_table[spl_Great_Wyrm].Param0)
        ||
        (_UNITS[battle_units[battle_unit_idx].unit_idx].type == spell_data_table[spl_Fire_Elemental].Param0)
    )
    {

        animate = ST_TRUE;

    }

    battle_units[battle_unit_idx].Always_Animate = animate;

    battle_units[battle_unit_idx].Move_Bob = Flight_Bob;

}


// WZD o105p11
// drake178: BU_GetWpnIconIndex()
/*
; returns the icon index indicating the unit's weapon
; quality (normal-mithril-magic-adamantium), or -1
; if the unit does not have a melee attack (its melee
; strength is 0 or lower)
*/
/*

icon_idx { -1, 0, 2, 3, 4 }
¿ no weapon type 1 ?

*/
int16_t Battle_Unit_Melee_Attack_Icon(int16_t battle_unit_idx)
{
    int16_t icon_idx = 0;  // _CX_
    int16_t weapon_type = 0;  // _DI_

    icon_idx = ST_UNDEFINED;

    if(battle_units[battle_unit_idx].melee > 0)
    {

        weapon_type = battle_units[battle_unit_idx].Weapon_Plus1;

        if(weapon_type > 0)
        {

            if(weapon_type == 2)
            {

                icon_idx = 3;

            }
            else if(weapon_type == 3)
            {

                icon_idx = 2;

            }
            else if(weapon_type == 4)
            {

                icon_idx = 4;

            }
            else
            {

                icon_idx = 0;

            }

        }
        else
        {
            icon_idx = 0;
        }

    }

    return icon_idx;
}


// WZD o105p12
// drake178: BU_GetRngIconIndex()
/*
; returns the icon index indicating the unit's ranged
; attack type (magic-bow-rock-breath-thrown), or -1
; if the unit does not have a ranged attack (its ranged
; strength is 0 or lower)
;
; BUG? thrown and breath are actually disabled
*/
/*

*/
int16_t Battle_Unit_Ranged_Attack_Icon(int16_t battle_unit_idx)
{
    int16_t weapon_type = 0;
    int16_t icon_idx = 0;  // _SI_
    int16_t ranged_type = 0;  // _DI_

    icon_idx = ST_UNDEFINED;

    ranged_type = battle_units[battle_unit_idx].ranged_type;

    // ¿ only covers ranged attack of types 10-19, 20-29, 30-39 ?
    if(
        (ranged_type != ST_UNDEFINED)
        &&
        (
            ((ranged_type / 10) == rag_Boulder)
            ||
            ((ranged_type / 10) == rag_Missile)
            ||
            ((ranged_type / 10) == rag_Magic)
        )
    )
    {

        weapon_type = ranged_type;

        switch(weapon_type)
        {

            case rat_Rock:
            case rat_Cannon:
            case rat_Sling:
            case rat_Unknown:
            {

                icon_idx = 6;

            } break;

            case rat_Lightning:
            case rat_Fireball:
            case rat_Sorcery:
            case rat_Deathbolt:
            case rat_Icebolt:
            case rat_Pr_Shaman:
            case rat_Drow:
            case rat_Sprite:
            case rat_Nat_Bolt:
            {

                icon_idx = 1;

            } break;

            case srat_Thrown:
            {

                icon_idx = 8;

            } break;

            case srat_FireBreath:
            case srat_Lightning:
            case srat_StoneGaze:
            {

                icon_idx = 7;

            } break;

            default:
            {
                icon_idx = 5;
            }

        }

    }

    return icon_idx;

}

// WZD o105p13
// drake178: CMB_DrawUnitDisplay()
/*
; draws the combat unit display into the current draw
; frame based on the unit that the mouse is hovering
; over (top right corner)
*/
/*

~ Draw_Active_Unit_Stats_And_Icons()

*/
void Draw_Combat_Unit_Display(void)
{
    int16_t Resist_Score = 0;
    int16_t Defense_Score = 0;
    uint8_t colors[2] = { 0, 0 };
    int16_t Attribute_Value = 0;
    int16_t y2 = 0;
    int16_t x2 = 0;
    int16_t level_icon_count = 0;
    int16_t Move_Type = 0;
    SAMB_ptr level_icon_seg = 0;
    int16_t unit_idx = 0;
    int16_t unit_type = 0;
    int16_t unit_owner_idx = 0;
    // int16_t unit_hero_slot_idx__Attr_Display_Var = 0;
    int16_t unit_hero_slot_idx = 0;
    int16_t Attr_Display_Var = 0;
    int16_t x1 = 0;  // _SI_
    int16_t y1 = 0;  // _DI_


    if(
        (_scanned_battle_unit != ST_UNDEFINED)
        &&
        (_scanned_battle_unit < 36)
    )
    {

        x1 = 250;
        y1 = 4;
        x2 = 318;
        y2 = 43;

        Gradient_Fill(x1, y1, x2, y2, 15, 8, ST_NULL, ST_NULL, ST_NULL);

        Line(x1, y1, x1, y2, 230);

        Line(x1, y1, (x2 - 1), y1, 230);

        Line(x2, y1, x2, y2, 237);

        Line((x1 + 1), y2, x2, y2, 237);

        colors[0] = 182;
        colors[1] = 177;

        Set_Font_Colors_15(0, &colors[0]);

        Set_Font_Style_Shadow_Down(0, 15, 0, 0);

        Set_Alias_Color(182);

        Set_Outline_Color(2);

        unit_idx = battle_units[_scanned_battle_unit].unit_idx;

        unit_owner_idx = _UNITS[unit_idx].owner_idx;

        unit_hero_slot_idx = _UNITS[unit_idx].Hero_Slot;

        unit_type = _UNITS[unit_idx].type;

        if(unit_hero_slot_idx != ST_UNDEFINED)
        {

            strcpy(GUI_String_1, _players[unit_owner_idx].Heroes[unit_hero_slot_idx].name);

        }
        else
        {

            strcpy(GUI_String_1, *_unit_type_table[unit_type].name);

        }

        Print_Centered((x1 + ((x2 - x1) / 2)), (y1 + 2), GUI_String_1);


        Attr_Display_Var = Battle_Unit_Melee_Attack_Icon(_scanned_battle_unit);

        if(Attr_Display_Var != ST_UNDEFINED)
        {

            Attribute_Value = battle_units[_scanned_battle_unit].melee;

            Print_Integer_Right((x1 + 9), (y1 + 10), Attribute_Value);

            FLIC_Draw((x1 + 11), (y1 + 8), IMG_CMB_ScanIcons[Attr_Display_Var]);

        }


        Attr_Display_Var = Battle_Unit_Ranged_Attack_Icon(_scanned_battle_unit);

        if(Attr_Display_Var != ST_UNDEFINED)
        {

            Attribute_Value = battle_units[_scanned_battle_unit].ranged;

            Print_Integer_Right((x1 + 9), (y1 + 17), Attribute_Value);

            FLIC_Draw((x1 + 11), (y1 + 15), IMG_CMB_ScanIcons[Attr_Display_Var]);

        }


        if(battle_units[_scanned_battle_unit].movement_points != 0)
        {

            Print_Moves_String((x1 + 9), (y1 + 24), battle_units[_scanned_battle_unit].movement_points, 1);

        }
        else
        {

            Print_Integer_Right((x1 + 9), (y1 + 24), battle_units[_scanned_battle_unit].movement_points);

        }


        Move_Type = Battle_Unit_Movement_Icon(_scanned_battle_unit);

        Attr_Display_Var = 11;

        if(Move_Type == 2)
        {

            Attr_Display_Var = 12;

        }

        if(
            (Move_Type == 4)
            ||
            (Move_Type == 5)
        )
        {

            Attr_Display_Var = 13;

        }

        FLIC_Draw((x1 + 11), (y1 + 22), IMG_CMB_ScanIcons[Attr_Display_Var]);

        Print((x1 + 3), (y1 + 32), cnst_Hits_2);

        Gradient_Fill((x1 + 19), (y1 + 34), (x1 + 38), (y1 + 34), 3, ST_NULL, ST_NULL, ST_NULL, ST_NULL);

        Line((x1 + 19), (y1 + 35), (x1 + 38), (y1 + 35), 1);

        Draw_Active_Unit_Damage_Bar(_scanned_battle_unit, (x1 + 19), (y1 + 34));

        Defense_Score = battle_units[_scanned_battle_unit].defense;

        Print_Integer_Right((x1 + 48), (y1 + 10), Defense_Score);

        FLIC_Draw((x1 + 50), (y1 + 8), IMG_CMB_ScanIcons[9]);

        Resist_Score = battle_units[_scanned_battle_unit].resist;

        Print_Integer_Right((x1 + 48), (y1 + 17), Resist_Score);

        FLIC_Draw((x1 + 50), (y1 + 15), IMG_CMB_ScanIcons[14]);  // ~RESIST

        if(battle_units[_scanned_battle_unit].mana > 0)
        {

            Print_Integer_Right((x1 + 48), (y1 + 24), battle_units[_scanned_battle_unit].mana);

            Print((x1 + 51), (y1 + 24), cnst_mp);

        }
        else
        {

            if(battle_units[_scanned_battle_unit].ammo > 0)
            {

                Print_Integer_Right((x1 + 48), (y1 + 24), battle_units[_scanned_battle_unit].ammo);

                Print((x1 + 51), (y1 + 24), cnst_Ammo);

            }

        }

        if(_UNITS[unit_idx].Level > 0)
        {

            if(_UNITS[unit_idx].Level > 6)
            {

                level_icon_seg = IMG_CMB_ScanIcons[17];

                Attr_Display_Var = 6;

            }
            else if(_UNITS[unit_idx].Level > 3)
            {

                level_icon_seg = IMG_CMB_ScanIcons[16];

                Attr_Display_Var = 3;

            }
            else
            {

                level_icon_seg = IMG_CMB_ScanIcons[15];

                Attr_Display_Var = 0;

            }

            level_icon_count = 0;

            Attribute_Value = Attr_Display_Var;

            while(_UNITS[unit_idx].Level > Attribute_Value)
            {

                FLIC_Draw((x1 + 48 + (level_icon_count * 5)), (y1 + 33), level_icon_seg);

                Attribute_Value++;

            }

        }

    }

}



/*
    WIZARDS.EXE  ovr110
*/

// WZD o110p01
/*

*/
void Strategic_Combat_Allocate(void)
{
    int16_t itr;  // _SI_

    // MoO2  Strategic_Combat()  _qcombat_data = Allocate_First_Block()
    // 249 * 16 = 3984 / 110 = 36.2182 ... 36? 2 * max stack = 18 so, ... 36 units, but 16 pictures?
    battle_units = (struct s_BATTLE_UNIT *)Allocate_First_Block(_screen_seg, 249);

    // 3 * 16 = 48 ... 1PR header, 15 2-byte values
    // combat_enchantments = (struct s_COMBAT_ENCHANTMENTS *)Allocate_Next_Block(_screen_seg, 3);
    combat_enchantments = (int8_t *)Allocate_Next_Block(_screen_seg, 3);

    for(itr = 0; itr < 30; itr++)
    {
        // *((char *)&combat_enchantments[itr]) = ST_FALSE;
        combat_enchantments[itr] = ST_FALSE;
    }

    CMB_IDK_4PR = (int16_t *)Allocate_Next_Block(_screen_seg, 4);  // header + sub-header + 2 PR ... or header + 3 sub-headers?
    CMB_HolyBonusArray  = (int16_t *)Allocate_Next_Block(_screen_seg, 3);  // sub-header + 2 PR  32 B
    CMB_ResistAllArray  = (int16_t *)Allocate_Next_Block(_screen_seg, 3);  // sub-header + 2 PR  32 B
    CMB_LeadershipArray = (int16_t *)Allocate_Next_Block(_screen_seg, 3);  // sub-header + 2 PR  32 B

}


// WZD o110p02
/*
    "WIP", cause ¿ ?

*/
int16_t Strategic_Combat__WIP(int16_t troops[], int16_t troop_count, int16_t wx, int16_t wy, int16_t wp, int16_t * item_count, int16_t item_list[])
{
    int16_t Weights[36] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t var_66 = 0;
    int16_t var_64 = 0;
    int16_t var_62 = 0;
    int16_t var_60 = 0;
    int16_t var_5E = 0;
    int16_t var_5C = 0;
    int16_t defender_third_nominal_skill = 0;
    int16_t attacker_third_nominal_skill = 0;
    int16_t var_56 = 0;
    int16_t var_54 = 0;
    int16_t var_52 = 0;
    int16_t var_50 = 0;
    int16_t var_4E = 0;
    int16_t var_4C = 0;
    int16_t defender_mana_multiplied = 0;
    int16_t attacker_mana_multiplied = 0;
    int16_t HP = 0;
    int16_t special_ranged_attack_strength = 0;  // DNE in Dasm  (re-uses HP)
    int16_t var_44 = 0;
    int16_t var_42 = 0;
    int16_t var_40 = 0;
    int16_t IDK_damage_defender = 0;
    int16_t IDK_damage_attacker = 0;
    int32_t var_3A = 0;
    int32_t var_36 = 0;
    int32_t IDK_health_defender__2 = 0;
    int32_t IDK_ranged_threat_defender = 0;
    int32_t IDK_melee_threat_defender = 0;
    int32_t IDK_health_attacker__2 = 0;
    int32_t IDK_ranged_threat_attacker = 0;
    int32_t IDK_melee_threat_attacker = 0;
    int32_t IDK_health_defender = 0;
    int32_t IDK_health_attacker = 0;
    int16_t combat_structure = 0;
    int16_t Dmg_Array[3] = { 0, 0, 0 };
    int16_t battle_unit_idx = 0;  // ¿ MsgType__BU_Index ?
    int16_t MsgType = 0;  // ¿ MsgType__BU_Index ?
    int16_t unit_idx = 0;
    int16_t _combat_defender_count = 0;
    int16_t winner_player_idx = 0;
    int16_t itr = 0;  // _SI_
    int16_t _combat_attacker_count = 0;  // DNE in Dasm
    int16_t itr_units = 0;  // _SI_
    int16_t itr_battle_units = 0;  // _SI_
    int16_t itr_combat_turns = 0;  // _SI_
    int16_t spell_ranks = 0;  // _DI_
    int16_t did_win = 0;  // DNE in Dasm


    var_5C = 0;
    var_64 = 0;
    var_60 = 0;
    var_5E = 0;
    var_66 = 0;
    var_62 = 0;

    Set_Page_Off();
    Allocate_Reduced_Map();
    Main_Screen_Draw();
    Copy_Off_To_Back();

    _combat_wx = wx;
    _combat_wy = wy;
    _combat_wp = wp;

    _combat_defender_count = 0;
    _combat_attacker_count = troop_count;  // DNE in Dasm

    _combat_total_unit_count = 0;

    Strategic_Combat_Allocate();


    combat_structure = Combat_Structure(wx, wy, wp, ST_TRUE);


    for(itr_units = (_units - 1); itr_units > 0; itr_units--)
    {
        if(
            (_UNITS[itr_units].wx == wx)
            &&
            (_UNITS[itr_units].wy == wy)
            &&
            (_UNITS[itr_units].wp == wp)
            &&
            (_UNITS[itr_units].owner_idx != _UNITS[troops[0]].owner_idx)
        )
        {
            _combat_defender_count++;

            _combat_defender_player = _UNITS[itr_units].owner_idx;

            Load_Battle_Unit(itr_units, &battle_units[_combat_total_unit_count]);

            if(battle_units[_combat_total_unit_count].controller_idx == ST_UNDEFINED)
            {
                battle_units[_combat_total_unit_count].controller_idx = MOO_MONSTER_PLAYER_IDX;
            }

            _combat_total_unit_count++;
        }
    }


    for(itr = 0; itr < troop_count; itr++)
    {
        unit_idx = troops[itr];
        _combat_attacker_player = _UNITS[troops[0]].owner_idx;
        Load_Battle_Unit(unit_idx, &battle_units[_combat_total_unit_count]);
        _combat_total_unit_count++;
    }


    Init_Battlefield_Effects(combat_structure);


    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
    {
        BU_Apply_Battlefield_Effects__WIP(&battle_units[itr_battle_units]);
    }


    /*
        "Ocean Combat"
        "Only flying, sailing and swimming units may actually engage in combat over such squares."
    */
    _combat_attacker_count -= Undeployable_Battle_Units_On_Water(_combat_attacker_player);
    _combat_defender_count -= Undeployable_Battle_Units_On_Water(_combat_defender_player);


    IDK_health_attacker__2 = 0;
    IDK_health_defender__2 = 0;
    IDK_health_attacker = 0;
    IDK_health_defender = 0;
    IDK_melee_threat_attacker = 0;
    IDK_melee_threat_defender = 0;
    IDK_ranged_threat_attacker = 0;
    IDK_ranged_threat_defender = 0;
    IDK_damage_attacker = 0;
    IDK_damage_defender = 0;


    /*
        BEGIN:  ¿ WTF ?
    */
    if((_combat_attacker_player < HUMAN_PLAYER_IDX) || (_combat_attacker_player > NEUTRAL_PLAYER_IDX))
    {
        for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
        {
            if(battle_units[itr_battle_units].controller_idx == _combat_attacker_player)
            {
                battle_units[itr_battle_units].status = 6;  /* Unit_Gone */
            }
        }
    }

    if((_combat_defender_player < HUMAN_PLAYER_IDX) || (_combat_defender_player > NEUTRAL_PLAYER_IDX))
    {
        for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
        {
            if(battle_units[itr_battle_units].controller_idx == _combat_defender_player)
            {
                battle_units[itr_battle_units].status = 6;  /* Unit_Gone */
            }
        }
    }
    /*
        END:  ¿ WTF ?
    */


   for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
   {
        if(battle_units[itr_battle_units].status == bus_Active)
        {
            Weights[itr_battle_units] = (30 - battle_units[itr_battle_units].defense);

            if(battle_units[itr_battle_units].controller_idx == _combat_attacker_player)
            {
                // Unit's Current Total Hit Points
                HP = ((battle_units[itr_battle_units].Cur_Figures * battle_units[itr_battle_units].hits) - battle_units[itr_battle_units].TopFig_Dmg);

                IDK_health_attacker += Get_Effective_Hits(HP, battle_units[itr_battle_units].defense);

                if(Battle_Unit_Has_Ranged_Attack(itr_battle_units) == ST_TRUE)
                {
                    IDK_ranged_threat_attacker += Get_Effective_Ranged_Strength(battle_units[itr_battle_units].ranged, battle_units[itr_battle_units].Cur_Figures, (battle_units[itr_battle_units].Attack_Flags | battle_units[itr_battle_units].Ranged_ATK_Flags));
                }

                // {100, 101, 102, 103, 104, 105} srat_Thrown, srat_FireBreath, srat_Lightning, srat_StoneGaze, srat_MultiGaze, srat_DeathGaze
                // special ranged attack strength
                // Thrown, Breath, Gaze
                if(battle_units[itr_battle_units].ranged_type < srat_Thrown)
                {
                    special_ranged_attack_strength = 0;
                }
                else
                {
                    special_ranged_attack_strength = battle_units[itr_battle_units].ranged;
                }

                IDK_melee_threat_attacker += Get_Effective_Melee_Strength(battle_units[itr_battle_units].melee, special_ranged_attack_strength, battle_units[itr_battle_units].Cur_Figures, (battle_units[itr_battle_units].Attack_Flags | battle_units[itr_battle_units].Melee_ATK_Flags), battle_units[itr_battle_units].ranged_type);

                IDK_damage_attacker += ((battle_units[itr_battle_units].hits * battle_units[itr_battle_units].Cur_Figures) - battle_units[itr_battle_units].TopFig_Dmg);
            }
            else  /* battle_units[itr_battle_units].controller_idx == _combat_defender_player */
            {
                // Unit's Current Total Hit Points
                HP = ((battle_units[itr_battle_units].Cur_Figures * battle_units[itr_battle_units].hits) - battle_units[itr_battle_units].TopFig_Dmg);

                IDK_health_defender += Get_Effective_Hits(HP, battle_units[itr_battle_units].defense);

                if(Battle_Unit_Has_Ranged_Attack(itr_battle_units) == ST_TRUE)
                {
                    IDK_ranged_threat_defender += Get_Effective_Ranged_Strength(battle_units[itr_battle_units].ranged, battle_units[itr_battle_units].Cur_Figures, (battle_units[itr_battle_units].Attack_Flags | battle_units[itr_battle_units].Ranged_ATK_Flags));
                }

                if(battle_units[itr_battle_units].ranged_type < srat_Thrown)
                {
                    special_ranged_attack_strength = 0;
                }
                else
                {
                    special_ranged_attack_strength = battle_units[itr_battle_units].ranged;
                }

                IDK_melee_threat_defender += Get_Effective_Melee_Strength(battle_units[itr_battle_units].melee, special_ranged_attack_strength, battle_units[itr_battle_units].Cur_Figures, (battle_units[itr_battle_units].Attack_Flags | battle_units[itr_battle_units].Melee_ATK_Flags), battle_units[itr_battle_units].ranged_type);

                IDK_damage_defender += ((battle_units[itr_battle_units].hits * battle_units[itr_battle_units].Cur_Figures) - battle_units[itr_battle_units].TopFig_Dmg);
            }  /* if(battle_units[itr_battle_units].controller_idx == _combat_attacker_player) */
        }
    }



    /*
        BEGIN:  Mods - Mana / Spell-Ranks
    */

    if(_combat_attacker_player >= _num_players)  /* Neural Player or Monster */
    {
        attacker_mana_multiplied = 0;
    }
    else
    {
        attacker_mana_multiplied = ((10 * _players[_combat_attacker_player].mana_reserve) / Combat_Casting_Cost_Multiplier(_combat_attacker_player));

        if(_players[_combat_attacker_player].Nominal_Skill < attacker_mana_multiplied)
        {
            attacker_mana_multiplied = _players[_combat_attacker_player].Nominal_Skill;
        }

        attacker_third_nominal_skill = (_players[_combat_attacker_player].Nominal_Skill / 3);

        if(attacker_third_nominal_skill > attacker_mana_multiplied)
        {
            attacker_third_nominal_skill = attacker_mana_multiplied;
        }

        var_4C = 0;
        var_50 = 0;
        var_54 = 0;

        if(attacker_mana_multiplied >= 10)
        {
            spell_ranks = _players[_combat_attacker_player].spellranks[0];

            if(
                (combat_structure == 2)
                ||
                (combat_structure == 4)
            )
            {
                spell_ranks = 0;
            }

            if(spell_ranks > 0)
            {
                var_4C = (spell_ranks * 50);
                var_50 = (spell_ranks * 10);
                var_54 = (spell_ranks * 25);
            }

            if(
                (combat_structure == 3)
                ||
                (combat_structure == 4)
            )
            {
                spell_ranks = 0;
            }

            if(spell_ranks > 0)
            {
                var_4C = (spell_ranks * 75);
                var_54 = (spell_ranks * 25);
            }
            
            // Chaos
            spell_ranks = _players[_combat_attacker_player].spellranks[2];

            if(
                (combat_structure == 2)
                ||
                (combat_structure == 3)
            )
            {
                spell_ranks = 0;
            }

            if(spell_ranks > 0)
            {
                var_4C = (spell_ranks * 100);
                var_50 = (spell_ranks * 30);
            }

            // Life
            spell_ranks = _players[_combat_attacker_player].spellranks[3];

            if(
                (combat_structure == 2)
                ||
                (combat_structure == 4)
                ||
                (combat_structure == 3)
            )
            {
                spell_ranks = 0;
            }

            if(spell_ranks > 0)
            {
                var_50 = (spell_ranks * 15);
                var_54 = (spell_ranks * 40);
            }

            // Death
            spell_ranks = _players[_combat_attacker_player].spellranks[4];

            if(
                (combat_structure == 2)
                ||
                (combat_structure == 4)
                ||
                (combat_structure == 3)
            )
            {
                spell_ranks = 0;
            }

            if(spell_ranks > 0)
            {
                var_4C = (spell_ranks * 75);
                var_54 = (spell_ranks * 15);
            }

            var_4C = (var_4C / 10);
            var_50 = (var_50 / 10);
            var_54 = (var_54 / 20);

        }  /* if(attacker_mana_multiplied >= 10) */

    }  /* if(_combat_attacker_player >= _num_players) */

    if(_combat_defender_player >= _num_players)
    {
        defender_mana_multiplied = 0;
    }
    else
    {
        defender_mana_multiplied = ((10 * _players[_combat_defender_player].mana_reserve) / Combat_Casting_Cost_Multiplier(_combat_defender_player));

        if(_players[_combat_defender_player].Nominal_Skill < defender_mana_multiplied)
        {
            defender_mana_multiplied = _players[_combat_defender_player].Nominal_Skill;
        }

        defender_third_nominal_skill = (_players[_combat_defender_player].Nominal_Skill / 3);

        if(defender_third_nominal_skill > defender_mana_multiplied)
        {
            defender_third_nominal_skill = defender_mana_multiplied;
        }

        var_4E = 0;
        var_52 = 0;
        var_56 = 0;

        if(defender_mana_multiplied >= 10)
        {
            spell_ranks = _players[_combat_defender_player].spellranks[0];

            if(
                (combat_structure == 2)
                ||
                (combat_structure == 4)
            )
            {
                spell_ranks = 0;
            }

            if(spell_ranks > 0)
            {
                var_4E = (spell_ranks * 50);
                var_52 = (spell_ranks * 10);
                var_56 = (spell_ranks * 25);
            }

            if(
                (combat_structure == 3)
                ||
                (combat_structure == 4)
            )
            {
                spell_ranks = 0;
            }

            if(spell_ranks > 0)
            {
                var_4E = (spell_ranks * 75);
                var_56 = (spell_ranks * 25);
            }
            
            // Chaos
            spell_ranks = _players[_combat_defender_player].spellranks[2];

            if(
                (combat_structure == 2)
                ||
                (combat_structure == 3)
            )
            {
                spell_ranks = 0;
            }

            if(spell_ranks > 0)
            {
                var_4E = (spell_ranks * 100);
                var_52 = (spell_ranks * 30);
            }

            // Life
            spell_ranks = _players[_combat_defender_player].spellranks[3];

            if(
                (combat_structure == 2)
                ||
                (combat_structure == 4)
                ||
                (combat_structure == 3)
            )
            {
                spell_ranks = 0;
            }

            if(spell_ranks > 0)
            {
                var_52 = (spell_ranks * 15);
                var_56 = (spell_ranks * 40);
            }

            // Death
            spell_ranks = _players[_combat_defender_player].spellranks[4];

            if(
                (combat_structure == cs_SorceryNode)
                ||
                (combat_structure == cs_ChaosNode)
                ||
                (combat_structure == cs_NatureNode)
            )
            {
                spell_ranks = 0;
            }

            if(spell_ranks > 0)
            {
                var_4E = (spell_ranks * 75);
                var_56 = (spell_ranks * 15);
            }

            var_4E = (var_4C / 10);
            var_52 = (var_50 / 10);
            var_56 = (var_54 / 20);

        }
    }  /* if(_combat_defender_player >= _num_players) */

    /*
        END:  Mods - Mana / Spell-Ranks
    */


    IDK_health_attacker__2 = IDK_health_attacker;
    IDK_health_defender__2 = IDK_health_defender;
    /* DEMOHACK */ if(OVL_Action_Type == 5) { IDK_health_attacker__2 = IDK_health_attacker = 0x7FFFFFFF /*0b01111111111111111111111111111111*/; }

    /*
        BEGIN:  ¿ halve values for Neutral Player ?
    */

    // BUGBUG: should be ||? either or both
    if(
        (_combat_attacker_player != HUMAN_PLAYER_IDX)
        &&
        (_combat_defender_player != HUMAN_PLAYER_IDX)
    )
    {
        if(_combat_attacker_player != NEUTRAL_PLAYER_IDX)
        {
            var_5C = (var_5C / 2);
            var_64 = (var_64 / 2);
            var_60 = (var_60 / 2);
            IDK_health_attacker__2 = (IDK_health_attacker__2 / 2);
            IDK_health_attacker = (IDK_health_attacker / 2);
            IDK_melee_threat_attacker = (IDK_melee_threat_attacker / 2);
            IDK_ranged_threat_attacker = (IDK_ranged_threat_attacker / 2);
            var_4C = (var_4C / 2);
            var_50 = (var_50 / 2);
            var_54 = (var_54 / 2);
        }
        if(_combat_attacker_player != NEUTRAL_PLAYER_IDX)
        {
            var_5E = (var_5E / 2);
            var_66 = (var_66 / 2);
            var_62 = (var_62 / 2);
            IDK_health_defender__2 = (IDK_health_defender__2 / 2);
            IDK_health_defender = (IDK_health_defender / 2);
            IDK_melee_threat_defender = (IDK_melee_threat_defender / 2);
            IDK_ranged_threat_defender = (IDK_ranged_threat_defender / 2);
            var_4E = (var_4E / 2);
            var_52 = (var_52 / 2);
            var_56 = (var_56 / 2);
        }
    }

    /*
        END:  ¿ halve values for Neutral Player ?
    */


    /*
        BEGIN:  Combat Turn
    */

    for(itr_combat_turns = 0; itr_combat_turns < 3; itr_combat_turns++)
    {
        if((IDK_ranged_threat_attacker > 10) || (IDK_ranged_threat_defender > 10))
        {
            if((IDK_health_attacker > 0) && (IDK_health_defender > 0))
            {
                if(attacker_mana_multiplied > 10)
                {
                    var_64 = ((var_4C * attacker_third_nominal_skill) / 5);
                    var_5C = ((var_50 * attacker_third_nominal_skill) / 5);
                    var_60 = ((var_54 * attacker_third_nominal_skill) / 5);

                    var_64 += ((IDK_ranged_threat_attacker * var_5C) / 100);

                    var_54 = (var_54 / 2);

                    _players[_combat_attacker_player].mana_reserve -= ((Combat_Casting_Cost_Multiplier(_combat_attacker_player) * attacker_third_nominal_skill) / 10);

                    attacker_mana_multiplied -= attacker_third_nominal_skill;

                    if(attacker_third_nominal_skill < attacker_mana_multiplied)
                    {
                        attacker_third_nominal_skill = attacker_mana_multiplied;
                    }
                }
                else
                {
                    var_5C = 0;
                    var_64 = 0;
                    var_60 = 0;
                }

                if(defender_mana_multiplied > 10)
                {
                    var_66 = ((var_4E * defender_third_nominal_skill) / 5);
                    var_5E = ((var_52 * defender_third_nominal_skill) / 5);
                    var_62 = ((var_56 * defender_third_nominal_skill) / 5);

                    var_66 += ((IDK_ranged_threat_defender * var_5E) / 100);

                    var_56 = (var_56 / 2);

                    _players[_combat_defender_player].mana_reserve -= ((Combat_Casting_Cost_Multiplier(_combat_defender_player) * defender_third_nominal_skill) / 10);

                    defender_mana_multiplied -= defender_third_nominal_skill;

                    if(defender_third_nominal_skill < defender_mana_multiplied)
                    {
                        defender_third_nominal_skill = defender_mana_multiplied;
                    }
                }
                else
                {
                    var_5E = 0;
                    var_66 = 0;
                    var_62 = 0;
                }


                var_36 = (((IDK_ranged_threat_attacker + var_64) * Random(10)) / 100);
                var_3A = (((IDK_ranged_threat_defender + var_66) * Random(10)) / 100);

                IDK_health_attacker += var_60;
                IDK_health_defender += var_62;

                IDK_ranged_threat_attacker -= ((var_3A * IDK_ranged_threat_attacker) / IDK_health_attacker);
                IDK_ranged_threat_defender -= ((var_36 * IDK_ranged_threat_defender) / IDK_health_defender);

                IDK_melee_threat_attacker -= ((var_3A * IDK_melee_threat_attacker) / IDK_health_attacker);
                IDK_melee_threat_defender -= ((var_36 * IDK_melee_threat_defender) / IDK_health_defender);

                IDK_health_attacker -= var_3A;
                IDK_health_defender -= var_36;

                if(IDK_ranged_threat_attacker < 0)
                {
                    IDK_ranged_threat_attacker = 0;
                }

                if(IDK_ranged_threat_defender < 0)
                {
                    IDK_ranged_threat_defender = 0;
                }

            }
        }

    }  /* for(itr_combat_turns = 0; itr_combat_turns < 3; itr_combat_turns++) */


    while((IDK_melee_threat_attacker > 10) && (IDK_melee_threat_defender > 10))
    {
        if(attacker_mana_multiplied <= 10)
        {
            var_5C = 0;
            var_64 = 0;
            var_60 = 0;  // ¿ ~== healing ?
        }
        else
        {
            var_64 = ((var_4C * attacker_third_nominal_skill) / 5);
            var_5C = ((var_50 * attacker_third_nominal_skill) / 5);
            var_60 = ((var_50 * attacker_third_nominal_skill) / 5);  // ¿ ~== healing ?
            var_64 += ((IDK_ranged_threat_attacker * var_5C) / 100);
            _players[_combat_attacker_player].mana_reserve -= ((Combat_Casting_Cost_Multiplier(_combat_attacker_player) * attacker_third_nominal_skill) / 10);
            attacker_mana_multiplied -= (attacker_third_nominal_skill / 2);
            if((attacker_mana_multiplied / 2) < attacker_third_nominal_skill)
            {
                attacker_third_nominal_skill = (attacker_mana_multiplied / 2);
            }
        }

        if(defender_mana_multiplied <= 10)
        {
            var_5E = 0;
            var_66 = 0;
            var_62 = 0;  // ¿ ~== healing ?
        }
        else
        {
            var_66 = ((var_4E * defender_third_nominal_skill) / 5);
            var_5E = ((var_52 * defender_third_nominal_skill) / 5);
            var_62 = ((var_52 * defender_third_nominal_skill) / 5);  // ¿ ~== healing ?
            var_66 += ((IDK_ranged_threat_defender * var_5E) / 100);
            _players[_combat_defender_player].mana_reserve -= ((Combat_Casting_Cost_Multiplier(_combat_defender_player) * defender_third_nominal_skill) / 10);
            defender_mana_multiplied -= (defender_third_nominal_skill / 2);
            if((defender_mana_multiplied / 2) < defender_third_nominal_skill)
            {
                defender_third_nominal_skill = (defender_mana_multiplied / 2);
            }
        }

        var_36 = (((IDK_melee_threat_attacker + var_64) * Random(10)) / 100);
        var_3A = (((IDK_melee_threat_defender + var_66) * Random(10)) / 100);

        IDK_health_attacker += var_60;  // ¿ ~== healing ?
        IDK_health_defender += var_62;  // ¿ ~== healing ?

        IDK_melee_threat_attacker -= ((IDK_melee_threat_attacker * var_3A) / IDK_health_attacker);
        IDK_melee_threat_defender -= ((IDK_melee_threat_defender * var_36) / IDK_health_defender);

        IDK_health_attacker -= var_3A;
        IDK_health_defender -= var_36;

    }  /* while((IDK_melee_threat_attacker > 10) & (IDK_melee_threat_defender > 10)) */

    /*
        END:  Combat Turn
    */


    if(IDK_melee_threat_attacker > 10)
    {
        winner_player_idx = _combat_attacker_player;

        var_44 = ((100 * IDK_health_attacker) / IDK_health_attacker__2);

        var_42 = ((var_44 * IDK_damage_attacker) / 100);

        var_40 = IDK_damage_attacker;
    }

    // BUGBUG: duplicated code?
    if(IDK_melee_threat_attacker > 10)
    {
        winner_player_idx = _combat_attacker_player;
        var_44 = ((100 * IDK_health_attacker) / IDK_health_attacker__2);
        var_42 = ((var_44 * IDK_damage_attacker) / 100);
        var_40 = IDK_damage_attacker;
    }
    else
    {
        if(IDK_melee_threat_defender > 10)
        {
            winner_player_idx = _combat_defender_player;
            var_44 = ((100 * IDK_health_defender) / IDK_health_defender__2);
            var_42 = ((var_44 * IDK_damage_defender) / 100);
            var_40 = IDK_damage_defender;
        }
    }


    for(itr = 0; itr < _combat_total_unit_count; itr++)
    {
        if(battle_units[itr].controller_idx != winner_player_idx)
        {
            battle_units[itr].status = bus_Dead;  /* Unit_Dead */
            Weights[itr] = 0;
        }
    }


    Dmg_Array[0] = 3;
    Dmg_Array[1] = 0;
    Dmg_Array[2] = 0;

    if(winner_player_idx != ST_UNDEFINED)
    {
        MsgType = ST_UNDEFINED;

        while(var_40 > var_42)
        {
            if(MsgType <= ST_UNDEFINED)
            {
                // ; chooses a random item from a list of 16bit weighted
                // ; chances; condensing the weights such that the total
                // ; fits into a single call of the 9-bit RNG (max 512),
                // ; using repeated divisions by 2 if necessary
                battle_unit_idx = Get_Weighted_Choice(&Weights[0], _combat_total_unit_count);
            }
            else
            {
                battle_unit_idx = MsgType;
            }

            Weights[battle_unit_idx] += 50;

            // ; applies the damage points passed through the array
            // ; to the target battle unit, reducing its figure count
            // ; or marking it dead as necessary (in which case the
            // ; combat victor and true sights are also updated)
            BU_ApplyDamage__WIP(battle_unit_idx, &Dmg_Array[0]);

            if(battle_units[battle_unit_idx].status <= 0)
            {
                MsgType = battle_unit_idx;
            }
            else
            {
                Weights[battle_unit_idx] = 0;
                MsgType = ST_UNDEFINED;
            }

            var_40 -= 3;
        }
    }


    if(winner_player_idx == HUMAN_PLAYER_IDX)
    {
        MsgType = 6;  /* ¿ cmbmsg_DefVictory ? */
    }
    else
    {
        MsgType = 7;  /* ¿ cmbmsg_DefDefeat ?*/
    }



    End_Of_Combat__WIP(winner_player_idx, item_count, item_list, MsgType);



    if(winner_player_idx == _combat_attacker_player)
    {
        did_win = ST_TRUE;
    }
    else
    {
        did_win = ST_FALSE;
    }


    return did_win;
}


// WZD o110p03
// drake178: BU_Heal()
/*
; heals a battle unit by the specified amount, and may
; grant temporary extra Hits above the maximum if the
; unit is overhealed and TempHits is set to 1
;
; contains multiple BUGs relating to Life Stealing and
; gaining extra Hits per Figure
*/
/*

*/
void Battle_Unit_Heal(int16_t battle_unit_idx, int16_t Healing, int16_t TempHits)
{
    int16_t damages[3] = { 0, 0, 0 };
    int16_t Moves_Left = 0;
    int16_t Healable_Damage = 0;
    int16_t Top_Figure_Damage = 0;
    int16_t itr_damages = 0;  // _DI_

    Healable_Damage = battle_units[battle_unit_idx].damage[1] + battle_units[battle_unit_idx].damage[0];

    Top_Figure_Damage = 0;

    // ; BUG: allows Life Stealing attacks to ignore Irreversible Damage
    if(
        (TempHits == 0)
        &&
        (Healing > Healable_Damage)
    )
    {

        Healing = Healable_Damage;

    }

    damages[0] = battle_units[battle_unit_idx].damage[0];
    damages[1] = battle_units[battle_unit_idx].damage[1];
    damages[2] = battle_units[battle_unit_idx].damage[2];

    damages[0] -= Healing;

    if(damages[0] < 0)
    {

        damages[1] += damages[0];

        damages[0] = 0;

        SETMIN(damages[1], 0);

    }

    for(itr_damages = 0; itr_damages < 3; itr_damages++)
    {

        battle_units[battle_unit_idx].damage[itr_damages] += damages[itr_damages];

    }

    battle_units[battle_unit_idx].TopFig_Dmg -= Healing;

    if(battle_units[battle_unit_idx].TopFig_Dmg < 0)
    {

        Top_Figure_Damage = battle_units[battle_unit_idx].TopFig_Dmg;

        battle_units[battle_unit_idx].TopFig_Dmg = 0;

        while(Top_Figure_Damage < 0)
        {

            if(battle_units[battle_unit_idx].Max_Figures > battle_units[battle_unit_idx].Cur_Figures)
            {

                battle_units[battle_unit_idx].Cur_Figures += 1;

                Top_Figure_Damage += battle_units[battle_unit_idx].hits;

            }
            
        }

        if(Top_Figure_Damage > 0)
        {

            battle_units[battle_unit_idx].TopFig_Dmg = Top_Figure_Damage;

        }

    }

    // ; BUG: allows gaining extra hits even if set to 0 by jumping to the wrong location (should be 74 70)
    // ; BUG: allows gaining extra hits without overhealing based on the top figure damage alone (should be jle)
    if(
        (TempHits == 0)
        &&
        (Top_Figure_Damage < 0)
    )
    {
        Top_Figure_Damage = 0;
    }

    Top_Figure_Damage = abs(Top_Figure_Damage);

    if(battle_units[battle_unit_idx].Max_Figures <= Top_Figure_Damage)
    {

        battle_units[battle_unit_idx].Extra_Hits += (Top_Figure_Damage / battle_units[battle_unit_idx].Max_Figures);

    }

    Moves_Left = battle_units[battle_unit_idx].movement_points;

    BU_Init_Battle_Unit(&battle_units[battle_unit_idx]);

    BU_Apply_Battlefield_Effects__WIP(&battle_units[battle_unit_idx]);

    battle_units[battle_unit_idx].movement_points = Moves_Left;

}


// WZD o110p04
/*
; assigns and returns an arbitrary effective health
; value based on the passed defense and hit points:
;   defense 1:           hp*18/10        (*1.8)
;   defense 2:           hp*21/10        (*2.1)
;   defense 3:           hp*24/10        (*2.4)
;   defense 4+:          hp*(def-2)*(def-2)*3/2
;     (hp*6) - (hp*13.5) - (hp*24) - (hp*37.5)...

Strategic_Combat()
    |-> HP = ((battle_units[itr_battle_units].hits / battle_units[itr_battle_units].Cur_Figures) - battle_units[itr_battle_units].TopFig_Dmg);
    |-> IDK_health_attacker += Get_Effective_Hits(HP, battle_units[itr_battle_units].defense);

HP = ((battle_units[].hits / battle_units[].Cur_Figures) - battle_units[].TopFig_Dmg);

battle_units[].defense = ¿ unit_type_table[_UNITS[].type].defense ?

*/
int16_t Get_Effective_Hits(int16_t hits, int16_t defense)
{
    int16_t effective_hits;  // _SI_

    effective_hits = (hits * 3);

    if(defense > 3)
    {
        effective_hits = (effective_hits * (((defense + -2) * (defense + -2)) / 2));
        // 4:  ((4 - 2) * (4 - 2)) = (2 * 2) =  4 / 2 = 2  200%
        // 5:  ((5 - 2) * (5 - 2)) = (3 * 3) =  9 / 2 = 4  400%
        // 6:  ((6 - 2) * (6 - 2)) = (4 * 4) = 16 / 2 = 8  800%
    }
    else
    {
        switch(defense)
        {
            case 0:
            {
                effective_hits = (effective_hits / 2);  // 50%
            } break;
            case 1:
            {
                effective_hits = ((effective_hits * 3) / 5);  // 60%
            } break;
            case 2:
            {
                effective_hits = ((effective_hits * 7) / 15);  // 70%
            } break;
            case 3:
            {
                effective_hits = ((effective_hits * 4) / 5);  // 80%
            } break;
        }
    }

    return effective_hits;
}

// WZD o110p05
/*

; assigns and returns an arbitrary threat value for the passed ranged combat attributes:
;   ranged 1:      figs*40/7    (*5.7)
;   ranged 2:      figs*40/3   (*13.3)
;   ranged 3:      figs*80/3   (*26.6)
;   ranged 4+:     figs*20*(m-2) (*40)-(*60)...
;   doom:          total*2
;   ap:            total*5/4 (unless doom)
;   touch:         +figs*60 each
;   illusion:      total*5
;   eldritch w:    total*5/4

Strategic_Combat()
    |-> IDK_ranged_threat_defender += Get_Effective_Ranged_Strength(battle_units[itr_battle_units].ranged, battle_units[itr_battle_units].Cur_Figures, (battle_units[itr_battle_units].Attack_Flags | battle_units[itr_battle_units].Ranged_ATK_Flags));

battle_units[].ranged
battle_units[].Cur_Figures
(battle_units[].Attack_Flags | battle_units[].Ranged_ATK_Flags)

*/
int16_t Get_Effective_Ranged_Strength(int16_t ranged, int16_t figures, int16_t attack_flags)
{
    int16_t ranged_threat;  // _CX_

    if(ranged == 0)
    {
        return 0;
    }

    if(ranged > 3)
    {
        ranged_threat = ((figures * 20) * (ranged + -2));
    }
    else
    {
        ranged_threat = (figures * 40);

        if(ranged == 1)
        {
            ranged_threat = (ranged_threat / 7);
        }
        else if(ranged == 2)
        {
            ranged_threat = (ranged_threat / 7);
        }
        else if(ranged == 3)
        {
            ranged_threat = (ranged_threat / 7);
        }
    }

    if(attack_flags != 0)
    {
        if((attack_flags & Att_DoomDmg) != 0)
        {
            ranged_threat = (ranged_threat * 2);  // 200%
        }
        else if((attack_flags & Att_ArmorPrc) != 0)
        {
            ranged_threat = ((ranged_threat * 5) / 4);  // 125%
        }

        if((attack_flags & Att_Poison) != 0)
        {
            ranged_threat = (ranged_threat + (figures * 60));
        }

        if((attack_flags & Att_LifeSteal) != 0)
        {
            ranged_threat = (ranged_threat + (figures * 60));
        }

        if((attack_flags & Att_Destruct) != 0)
        {
            ranged_threat = (ranged_threat + (figures * 60));
        }

        if((attack_flags & Att_StnTouch) != 0)
        {
            ranged_threat = (ranged_threat + (figures * 60));
        }

        if((attack_flags & Att_DthTouch) != 0)
        {
            ranged_threat = (ranged_threat + (figures * 60));
        }

        if((attack_flags & Att_Illusion) != 0)
        {
            ranged_threat = (ranged_threat * 5);  // 500%
        }

        // DNE  if((attack_flags & Att_1stStrike) != 0)
        // DNE  {
        // DNE      ranged_threat = ((ranged_threat * 5) / 4);  // 125%
        // DNE  }

        if((attack_flags & Att_EldrWeap) != 0)
        {
            ranged_threat = ((ranged_threat * 5) / 4);  // 125%
        }
    }

    // DNE if(ranged_type >= srat_StoneGaze)
    // DNE {
    // DNE     melee_threat = (melee_threat + 300);
    // DNE     if(ranged_type == srat_MultiGaze)
    // DNE     {
    // DNE         melee_threat = (melee_threat + 900);
    // DNE     }
    // DNE }

    return ranged_threat;
}

// WZD o110p06
// ¿ ~ MoO2  Module: CMBTAI  Expected_Weapon_Damage_(); Get_Effective_Missile_Strength_(); Get_Effective_Beam_Strength_() ?
/*
; assigns and returns an arbitrary threat value for the passed melee combat attributes:
;   melee 1:       figs*40/7    (*5.7)
;   melee 2:       figs*40/3   (*13.3)
;   melee 3:       figs*80/3   (*26.6)
;   melee 4+:      figs*20*(m-2) (*40)-(*60)...
;   short range:   +same*1.5
;   doom:          total*2
;   ap:            total*5/4 (unless doom)
;   touch:         +figs*60 each
;   illusion:      total*5
;   1st strike:    total*5/4
;   eldritch w:    total*5/4
;   gaze:          +300
;   multi-gaze:    +900

Strategic_Combat()
    |-> IDK_melee_threat_attacker += Get_Effective_Melee_Strength(battle_units[itr_battle_units].melee, special_ranged_attack_strength, battle_units[itr_battle_units].Cur_Figures, (battle_units[itr_battle_units].Attack_Flags | battle_units[itr_battle_units].Melee_ATK_Flags), battle_units[itr_battle_units].ranged_type);

battle_units[itr_battle_units].melee
special_ranged_attack_strength
battle_units[itr_battle_units].Cur_Figures
(battle_units[itr_battle_units].Attack_Flags | battle_units[itr_battle_units].Melee_ATK_Flags)
battle_units[itr_battle_units].ranged_type

¿ Calc, Mods, Effects, etc. ?
    battle_units[].melee = ¿ unit_type_table[_UNITS[].type].melee ?
    thrown = ¿ unit_type_table[_UNITS[].type].ranged ?
    battle_units[].Cur_Figures
    (battle_units[].Attack_Flags | battle_units[].Melee_ATK_Flags)
    battle_units[].ranged_type

"thrown" ~== 'Special Ranged Attack Strength' ~== {Thrown, Breath, Gaze}

*/
int16_t Get_Effective_Melee_Strength(int16_t melee, int16_t thrown, int16_t figures, int16_t attack_flags, int16_t ranged_type)
{
    int16_t melee_threat;  // _CX_
    int16_t thrown_threat;  // _DI_

    if(melee == 0)
    {
        return 0;
    }

    if(melee > 3)
    {
        melee_threat = ((figures * 20) * (melee + -2));
    }
    else
    {
        melee_threat = (figures * 40);
        switch(melee)
        {
            case 0:
            {
                melee_threat = 0;
            } break;
            case 1:
            {
                melee_threat = (melee_threat / 7);
            } break;
            case 2:
            {
                melee_threat = (melee_threat / 3);
            } break;
            case 3:
            {
                melee_threat = ((melee_threat * 2) / 3);
            } break;
        }
    }

    thrown_threat = 0;
    if(thrown > 0)
    {
        if(thrown > 3)
        {
            thrown_threat = ((figures * 30) * (thrown + -2));
        }
        else
        {
            thrown_threat = (figures * 60);
            switch(thrown)
            {
                case 0:
                {
                    thrown_threat = 0;
                } break;
                case 1:
                {
                    thrown_threat = (thrown_threat / 7);
                } break;
                case 2:
                {
                    thrown_threat = (thrown_threat / 3);
                } break;
                case 3:
                {
                    melee_threat = ((melee_threat * 2) / 3);
                } break;
            }
        }
    }
    melee_threat += thrown_threat;  // BUG:  += here should just be inside the if > 0

    if(attack_flags != 0)
    {
        if((attack_flags & Att_DoomDmg) != 0)
        {
            melee_threat = (melee_threat * 2);
        }
        else if((attack_flags & Att_ArmorPrc) != 0)
        {
            melee_threat = ((melee_threat * 5) / 4);
        }

        if((attack_flags & Att_Poison) != 0)
        {
            melee_threat = (melee_threat + (figures * 60));
        }

        if((attack_flags & Att_LifeSteal) != 0)
        {
            melee_threat = (melee_threat + (figures * 60));
        }

        if((attack_flags & Att_Destruct) != 0)
        {
            melee_threat = (melee_threat + (figures * 60));
        }

        if((attack_flags & Att_StnTouch) != 0)
        {
            melee_threat = (melee_threat + (figures * 60));
        }

        if((attack_flags & Att_DthTouch) != 0)
        {
            melee_threat = (melee_threat + (figures * 60));
        }

        if((attack_flags & Att_Illusion) != 0)
        {
            melee_threat = (melee_threat * 5);
        }

        if((attack_flags & Att_1stStrike) != 0)
        {
            melee_threat = ((melee_threat * 5) / 4);
        }

        if((attack_flags & Att_EldrWeap) != 0)
        {
            melee_threat = ((melee_threat * 5) / 4);
        }
    }

    if(ranged_type >= srat_StoneGaze)
    {
        melee_threat = (melee_threat + 300);
        if(ranged_type == srat_MultiGaze)
        {
            melee_threat = (melee_threat + 900);
        }
    }

    return melee_threat;
}




/*
    WIZARDS.EXE  ovr111
*/

// WZD o111p01
// drake178: AITP_CombatSpell()
/*
; combined AI target picker for combat spells - returns
; -1 if the spell can't be cast, 99 if it doesn't
; require a target, a BU index if it does, and sets the
; X/Y return pointers for spells that target a tile
;
; contains numerous BUGs, which allow spells to be cast
; when they shouldn't, and prevent others when they
; could be
*/
/*

*/
int16_t AITP_CombatSpell__STUB(int16_t spell_idx, int16_t player_idx, int16_t * Target_X, int16_t * Target_Y)
{

    return ST_UNDEFINED;

}

// WZD 111p02
// drake178: AITP_DarknessLight() 
// AITP_DarknessLight()

// WZD 111p03
// drake178: AITP_Healing() 
// AITP_Healing()

// WZD 111p04
// drake178: AITP_WarpWood() 
// AITP_WarpWood()

// WZD 111p05
// drake178: AITP_WarpCreature() 
// AITP_WarpCreature()

// WZD 111p06
// drake178: UU15_AITP_Disintegrate() 
// UU15_AITP_Disintegrate()

// WZD 111p07
// drake178: AITP_DispelMagic() 
// AITP_DispelMagic()

// WZD 111p08
// drake178: G_CMB_SpellEffect() 
// WZD o111p
/*
*/
/*

Combat_cast_Spell__WIP()
    Target = Combat_Spell_Target_Screen__WIP(spell_idx, &Target_X, &Target_Y);
    G_CMB_SpellEffect__WIP(spell_idx, Target, caster_idx, Target_X, Target_Y, IDK_mana, ST_TRUE, ST_NULL, ST_NULL);

*/
void G_CMB_SpellEffect__WIP(int16_t spell_idx, int16_t target_idx, int16_t caster_idx, int16_t target_cgx, int16_t target_cgy, int16_t Mana, int16_t Anims, int16_t UU1, int16_t UU2)
{
    int16_t Not_Moved_Yet = 0;
    int16_t Moves_Left = 0;
    int16_t Damage_Array[3] = { 0, 0, 0 };
    uint32_t enchantments = 0;
    int16_t resistance_modifier = 0;
    int16_t Figure_Count = 0;
    int16_t player_idx = 0;
    int16_t itr = 0;
    int16_t Resist_Result = 0;

    if(caster_idx >= CASTER_IDX_BASE)
    {
        player_idx = (caster_idx - CASTER_IDX_BASE);
    }
    else
    {
        player_idx = battle_units[caster_idx].controller_idx;
    }

    // ; BUG: range checking necessary, this can not only be
    // ; 99, but also -1 or -2
    // ...
    // ; magicdvw bla-bla-bla marker
    // ...
    // ; used for checking Spell Lock vs Banish/Dispel Evil,
    // ; and Righteousness vs non-resistables (none exist)
    enchantments = (_UNITS[battle_units[target_idx].unit_idx].enchantments | battle_units[itr].enchantments | battle_units[itr].item_enchantments);
    
    resistance_modifier = Spell_Resistance_Modifier(spell_idx);

    // ; apply -Spell Save modifiers, if any
    // ; BUG: looks for the items based on the battle
    // ; unit's owner, not the global one
    if(
        (caster_idx <= MAX_BATTLE_UNIT_COUNT)
        &&
        (_UNITS[battle_units[caster_idx].unit_idx].Hero_Slot > -1)
    )
    {
        for(itr = 0; itr < NUM_HERO_ITEM_SLOTS; itr++)
        {
            // ; BUG: this may not be the hero's original owner
            if(_players[player_idx].Heroes[_UNITS[battle_units[caster_idx].unit_idx].Hero_Slot].Items[itr] > -1)
            {
                // NON SPELL-SPECIFIC ARTIFACT ENCHANTMENTS
                // Spell Save **
                // ** Each -1 to Spell Save decreases the opponent’s ability to resist this unit’s spell attacks against it by 10%
                //      (i.e., it negates one of the enemy unit’s crosses).
                resistance_modifier -= _players[player_idx].Heroes[_UNITS[battle_units[caster_idx].unit_idx].Hero_Slot].Items[itr];
            }
        }
    }

    for(itr = 0; itr < 3; itr++)
    {
        Damage_Array[itr] = 0;
    }

    switch(spell_data_table[spell_idx].type)
    {
        case scc_Summoning:
        {
// int16_t Create_Unit__WIP(int16_t unit_type, int16_t owner_idx, int16_t wx, int16_t wy, int16_t wp, int16_t R_Param)
            Figure_Count = Create_Unit__WIP(spell_data_table[spell_idx].unit_type, player_idx, 0, 0, 9, 2000);
            if(Figure_Count == ST_TRUE)
            {
                UNIT_SummonToBattle__SEGRAX(player_idx, (_units - 1), target_cgx, target_cgy);
                BU_CombatSummon__SEGRAX((_combat_total_unit_count - 1), target_cgx, target_cgy, spell_idx, caster_idx);
            }
            else
            {
                STU_DEBUG_BREAK();
            }
        } break;
        case scc_Unit_Enchantment:
        {

        } break;
        case scc_City_Enchantment:
        {

        } break;
        case scc_City_Curse:
        {

        } break;
        case scc_Fixed_Dmg_Spell:
        {

        } break;
        case scc_Special_Spell:  /* 38 of these... :(.. */
        {
            if(
                (spell_idx != spl_Wall_Of_Stone)
                &&
                (spell_idx != spl_Raise_Dead)
                &&
                (spell_idx != spl_Animate_Dead)
            )
            {
                // SPELLY  CMB_PlaySpellAnim(target_cgx, target_cgy, spell_idx, player_idx, Anims, caster_idx);
            }
            if(spell_idx == spl_Wall_Of_Stone)
            {
                // SPELLY  CMB_WallRise_Anim(spl_Wall_Of_Stone, caster_idx);
            }
            if(spell_idx == spl_Warp_Wood)
            {
                battle_units[target_idx].ammo = 0;
                battle_units[target_idx].ranged_type = rat_None;
                Moves_Left = Battle_Unit_Moves2(target_idx);
                Not_Moved_Yet = ST_FALSE;
                if(battle_units[target_idx].movement_points == Moves_Left)
                {
                    Not_Moved_Yet = ST_TRUE;
                }
                else
                {
                    Moves_Left = battle_units[target_idx].movement_points;
                }
                BU_Init_Battle_Unit(&battle_units[target_idx]);
                BU_Apply_Battlefield_Effects__WIP(&battle_units[target_idx]);
                if(Not_Moved_Yet == ST_TRUE)
                {
                    battle_units[target_idx].movement_points = Battle_Unit_Moves2(target_idx);
                }
                else
                {
                    battle_units[target_idx].movement_points = Moves_Left;
                }
            }
            if(spell_idx == spl_Healing)
            {
                Battle_Unit_Heal(target_idx, 5, 0);
            }
            if(spell_idx == spl_Creature_Binding)
            {
                Resist_Result = BU_ResistRoll__STUB(battle_units[target_idx], resistance_modifier, spell_data_table[spell_idx].magic_realm);
                if(Resist_Result > 0)
                {
                    battle_units[target_idx].Combat_Effects |= bue_Creature_Binding;

                    if(battle_units[target_idx].controller_idx == _combat_attacker_player)
                    {
                        battle_units[target_idx].controller_idx = _combat_defender_player;
                    }
                    else
                    {
                        battle_units[target_idx].controller_idx = _combat_attacker_player;
                    }
                }
            }
            if(spell_idx == spl_Warp_Creature)
            {
                Resist_Result = BU_ResistRoll__STUB(battle_units[target_idx], resistance_modifier, spell_data_table[spell_idx].magic_realm);
                if(Resist_Result > 0)
                {
                    // SPELLY  BU_WarpCreature(target_idx);
                }
                Moves_Left = Battle_Unit_Moves2(target_idx);
                Not_Moved_Yet = ST_FALSE;
                if(battle_units[target_idx].movement_points == Moves_Left)
                {
                    Not_Moved_Yet = ST_TRUE;
                }
                else
                {
                    Moves_Left = battle_units[target_idx].movement_points;
                }
                BU_Init_Battle_Unit(&battle_units[target_idx]);
                BU_Apply_Battlefield_Effects__WIP(&battle_units[target_idx]);
                if(Not_Moved_Yet == ST_TRUE)
                {
                    battle_units[target_idx].movement_points = Battle_Unit_Moves2(target_idx);
                }
                else
                {
                    battle_units[target_idx].movement_points = Moves_Left;
                }

            }
            if(
                (spell_idx == spl_Recall_Hero)
                ||
                (spell_idx == spl_Word_Of_Recall)
            )
            {
                battle_units[target_idx].status = bus_Recalled;
            }
            if(spell_idx == spl_Magic_Vortex)
            {
                // SPELLY  WIZ_CreateVortex(player_idx, target_cgx, target_cgy);
            }
            if(spell_idx == spl_Earth_To_Mud)
            {
                // SPELLY  CMB_EarthToMud(target_cgx, target_cgy);
            }
            if(spell_idx == spl_Cracks_Call)
            {
                // SPELLY  CMB_ApplyCracksCall(target_cgx, target_cgy);
            }
            if(spell_idx == spl_Disrupt)
            {
                battlefield->Wall_Sections[(((target_cgy - 10) * 3) + (target_cgx - 5))] = 2;
            }
            if(spell_idx == spl_Raise_Dead)
            {
                Cast_Raise_Dead(player_idx, caster_idx, target_cgx, target_cgy);
            }
            if(spell_idx == spl_Animate_Dead)
            {
                Cast_Animate_Dead(player_idx, caster_idx);
            }
        } break;
        case scc_Target_Wiz_Spell:
        {

        } break;
        case scc_Global_Enchantment:
        {

        } break;
        case scc_Battlefield_Spell:
        {

        } break;
        case scc_Crafting_Spell:
        {

        } break;
        case scc_Destruction_Spell:
        {

        } break;
        case scc_Resistable_Spell:
        {

        } break;
        case scc_Unresistable_Spell:
        {

        } break;
        case scc_Mundane_Enchantment:
        {

        } break;
        case scc_Mundane_Curse:
        {

        } break;
        case scc_Infusable_Spell:
        {

        } break;
        case scc_Dispel_Spell:
        {

        } break;
        case scc_Disenchant_Spell:
        {

        } break;
        case scc_Disjunction_Spell:
        {

        } break;
        case scc_Counter_Spell:
        {

        } break;
        case scc_Var_Dmg_Spell:
        {

        } break;
        case scc_Banish_Spell:
        {

        } break;
    }

    Tactical_Combat_Draw();

    PageFlip_FX();

}


// WZD 111p09
// drake178: AITP_HolyWord() 
// AITP_HolyWord()



/*
    WIZARDS.EXE  ovr112
*/

// WZD o112p01
// drake178: CMB_ComposeBookBG()
/*
; draws the current combat screen with the spellbook
; open over it, and saves this into the third VGA frame
; ($A800) to serve as a background image
*/
/*

*/
void CMB_ComposeBookBG__WIP(void)
{
    CMB_ComposeBackgrnd__WIP();  // ... |-> Copy_Off_To_Back();
    Set_Page_Off();
    Tactical_Combat_Draw();
    FLIC_Draw(16, 10, _spellbook_small_seg);
    Copy_Off_To_Back();
}


// WZD o112p02
// drake178: CMB_RedrawSpellbook()
/*
; loads the third VGA frame into the current draw
; frame, then uses CMB_DrawSpellbook to redraw and
; fill out the actual book
*/
/*

~== Spellbook_Screen_Draw()
    |-> SmlBook_Draw__WIP(16, 10);
*/
void Combat_Spellbook_Screen_Draw(void)
{
    Copy_Back_To_Off();
    CmbBook_Draw__WIP(16, 10, _combat_caster_idx);
}


// WZD o112p03
// drake178: G_CMB_CastSpell()
/*
; resolves combat spellcasting by the specified entity,
; from opening the spellbook to casting the spell; or
; evaluating the castable list, selecting one, and
; casting it in case of the AI
; returns 0 if no spell can be cast, 1 if cancelled
; during targeting, or 2 if resolved (or countered)
;
; contains many BUGs related to casting costs, and
; inherints many more from the targeting-, spell list
; building-, and spell effect functions
*/
/*

not sure about the multiple returns
handful of variable reuses  (don't make distinct without maintaining the bug(s)!!)
atleast one BUGBUG with Target
cast_status seems mixed up

*/
int16_t Combat_Cast_Spell__WIP(int16_t caster_idx, int16_t wx, int16_t wy, int16_t wp)
{
    int16_t battle_unit_mana = 0;
    int16_t Spell_Like_Ability = 0;  // ; set to 1 if the human player uses a Spell ability
    int16_t cast_status = 0;
    int16_t Opponent_Index = 0;
    int16_t Base_Cost = 0;
    int16_t Extra_Mana = 0;
    int16_t player_idx = 0;
    int16_t Target_Y = 0;
    int16_t Target_X = 0;
    int16_t Target = 0;
    int16_t Selected_Spell = 0;
    int16_t Effective_Cost = 0;
    int16_t Overland_Cast_Save = 0;
    int16_t Can_Cast = 0;
    int16_t spell_idx = 0;  // _SI_
    int16_t IDK_mana = 0;  // _DI_

    cast_status = ST_TRUE;

    Spell_Like_Ability = ST_FALSE;

    _combat_caster_idx = caster_idx;

    if(caster_idx >= CASTER_IDX_BASE)
    {
        player_idx = (caster_idx - CASTER_IDX_BASE);
    }
    else
    {
        player_idx = battle_units[caster_idx].controller_idx;
        battle_unit_mana = battle_units[caster_idx].mana;
    }

    if(caster_idx == (CASTER_IDX_BASE + NEUTRAL_PLAYER_IDX))
    {
        return cast_status;
    }

    if(
        (player_idx == HUMAN_PLAYER_IDX)
        &&
        (_auto_combat_flag == ST_FALSE)
    )
    {

        spell_idx = spl_NONE;

        Spell_Like_Ability = ST_FALSE;

        /*
            BEGIN:  Caster is Battle Unit
        */
        if(caster_idx < CASTER_IDX_BASE)
        {

            if((battle_units[caster_idx].Attribs_2 & USA_DOOMBOLT) != 0)
            {
                spell_idx = spl_Doom_Bolt;
                Spell_Like_Ability = ST_TRUE;
            }

            if((battle_units[caster_idx].Attribs_2 & USA_FIREBALL) != 0)
            {
                spell_idx = spl_Fireball;
                Spell_Like_Ability = ST_TRUE;
            }

            if((battle_units[caster_idx].Attribs_2 & USA_WEB) != 0)
            {
                spell_idx = spl_Web;
                Spell_Like_Ability = ST_TRUE;
            }

            if((battle_units[caster_idx].Attribs_2 & USA_HEALING) != 0)
            {
                spell_idx = spl_Healing;
                Spell_Like_Ability = ST_TRUE;
            }
            if((battle_units[caster_idx].Attribs_2 & (USA_SUMMON_DEMON_1 | USA_SUMMON_DEMON_2)) != 0)
            {
                if(_units < MAX_UNIT_COUNT)
                {
                    BU_SummonDemon__SEGRAX(caster_idx);
                    battle_units[caster_idx].movement_points = 0;
                    return ST_TRUE;
                }
                else
                {
                    return ST_FALSE;
                }
            }
        /*
            END:  Caster is Battle Unit
        */

        }

    }

    // HERE:  caster is player or handled unit abilities
    // spell_idx could be Doom Bolt, Fireball, Web, or Healing

    if(spell_idx == spl_NONE)
    {

        Can_Cast = Combat_Spellbook_Build__WIP(caster_idx);

        if(Can_Cast == ST_FALSE)
        {
            return ST_FALSE;
        }

        do {

            CMB_ComposeBookBG__WIP();

            spell_idx = Combat_Spellbook_Screen(caster_idx, &Selected_Spell);

            if(
                (spell_idx <= 0)
                ||
                (spell_data_table[spell_idx].type < scc_Infusable_Spell)
            )
            {
                
                PageFlipEffect = 3;

                CMB_ComposeBookBG__WIP();

            }

        } while (Do_Legal_Spell_Check__WIP(spell_idx) != ST_FALSE);
        // ...not illegal...

        // ; conflicting condition - will always jump
        if(spell_data_table[spell_idx].type > scc_Infusable_Spell)
        {
            Spell_Like_Ability = ST_TRUE;
        }

        CMB_ComposeBackgrnd__WIP();  // ... |-> Copy_Off_To_Back();
        Set_Page_Off();
        Tactical_Combat_Draw();
        PageFlip_FX();

    }


    /*
        BEGIN:  
    */
    if(spell_idx != spl_NONE)
    {

        /*
            BEGIN:  ¿ Effective_Cost ?
        */
        if(
            (spell_data_table[spell_idx].type < scc_Infusable_Spell)
            ||
            (Spell_Like_Ability != ST_FALSE)
        )
        {
            
            if(caster_idx > CASTER_IDX_BASE)
            {

                // ; BUG: causes counters to use the effective cost
                IDK_mana = Casting_Cost(player_idx, spell_idx, 1);

            }
            else
            {

                // ; BUG: ignores Evil Omens
                IDK_mana = spell_data_table[spell_idx].casting_cost;

            }

            Effective_Cost = IDK_mana;

        }
        else
        {
            
            if(
                (player_idx == HUMAN_PLAYER_IDX)
                &&
                (_auto_combat_flag == ST_FALSE)
            )
            {

                Can_Cast = _players[HUMAN_PLAYER_IDX].casting_cost_remaining;

                Overland_Cast_Save = _players[HUMAN_PLAYER_IDX].casting_spell_idx;

                _players[HUMAN_PLAYER_IDX].casting_spell_idx = spell_idx;

                if(caster_idx >= CASTER_IDX_BASE)
                {

                    _players[HUMAN_PLAYER_IDX].casting_cost_remaining = Casting_Cost(HUMAN_PLAYER_IDX, spell_idx, 1);

                }
                else
                {

                    // ; BUG: ignores Evil Omens
                    _players[HUMAN_PLAYER_IDX].casting_cost_remaining = spell_data_table[spell_idx].casting_cost;

                }

                IDK_mana = CMB_SpellSlider__STUB(spell_idx, Selected_Spell, caster_idx);

                if(caster_idx >= CASTER_IDX_BASE)
                {

                    Effective_Cost = (IDK_mana - ((IDK_mana * Casting_Cost_Reduction(player_idx, spell_idx)) / 100));

                }
                else
                {

                    Effective_Cost = IDK_mana;

                }

                _players[HUMAN_PLAYER_IDX].casting_cost_remaining = Can_Cast;

                _players[HUMAN_PLAYER_IDX].casting_spell_idx = Overland_Cast_Save;

                CMB_ComposeBackgrnd__WIP();  // ... |-> Copy_Off_To_Back();

                Set_Page_Off();

                Tactical_Combat_Draw();

                PageFlip_FX();

            }
            else  /* (player_idx != HUMAN_PLAYER_IDX) || (_auto_combat_flag != ST_FALSE) */
            {

                if(caster_idx >= CASTER_IDX_BASE)
                {

                    //  ; BUG: ignores the base casting cost
                    IDK_mana = _players[player_idx].Cmbt_Skill_Left;

                    // ; BUG: ignores casting cost modifiers
                    if(spell_data_table[spell_idx].casting_cost < IDK_mana)
                    {

                        IDK_mana = spell_data_table[spell_idx].casting_cost;

                    }

                    Extra_Mana = Combat_Casting_Cost_Multiplier(player_idx);

                    // ; BUG: ignores casting cost modifiers
                    if(((_players[player_idx].mana_reserve * 10) / Extra_Mana) < spell_data_table[spell_idx].casting_cost)
                    {

                        // ; BUG: ignores casting cost modifiers
                        IDK_mana = (((_players[player_idx].mana_reserve * 10) / Extra_Mana) - spell_data_table[spell_idx].casting_cost);

                    }

                }
                else
                {

                    // ; BUG: ignores Evil Omens
                    // ; BUG: infusable item charges will be automatically
                    // ; countered if the user has no mana
                    IDK_mana = (battle_unit_mana - spell_data_table[spell_idx].casting_cost);

                    if(spell_data_table[spell_idx].casting_cost < IDK_mana)
                    {

                        IDK_mana = spell_data_table[spell_idx].casting_cost;

                    }

                }

                Base_Cost = spell_data_table[spell_idx].casting_cost;

                Extra_Mana = 0;

                // ; BUG: Banish has an effective gain of 1/15 mana, not 5
                if(
                    (spell_idx == spl_Life_Drain)
                    ||
                    (spell_idx == spl_Banish)
                )
                {

                    Extra_Mana = ((Random(((IDK_mana / 5) + 1)) - 1) * 5);

                }
                else if(spell_idx == spl_Counter_Magic)
                {

                    Extra_Mana = ((Random(((IDK_mana / 5) + 1)) - 1) * 5);

                }
                else
                {

                    Extra_Mana = IDK_mana;

                }

                IDK_mana = (Base_Cost + Extra_Mana);

                if(caster_idx > CASTER_IDX_BASE)
                {

                    // ; BUG: ignores Evil Omens (Ice Bolt...)
                    Effective_Cost = (IDK_mana - ((IDK_mana * Casting_Cost_Reduction(player_idx, spell_idx)) / 100));

                }
                else
                {

                    Effective_Cost = IDK_mana;

                }

            }  /* (player_idx != HUMAN_PLAYER_IDX) || (_auto_combat_flag != ST_FALSE) */

        }
        /*
            END:  ¿ Effective_Cost ?
        */


        /*
            BEGIN:  ¿ Counter Magic ?
        */
        if(
            (spell_idx != spl_Web)
            ||
            (Spell_Like_Ability != ST_TRUE)
        )
        {

            if(player_idx == _combat_attacker_player)
            {
                // Target_X = combat_enchantments[COUNTER_MAGIC_ATTKR];
                Target_X = COUNTER_MAGIC_ATTKR;
                Opponent_Index = _combat_defender_player;
            }
            else
            {
                // Target_X = combat_enchantments[COUNTER_MAGIC_DFNDR];
                Target_X = COUNTER_MAGIC_DFNDR;
                Opponent_Index = _combat_attacker_player;
            }

            if(combat_enchantments[Target_X] > 0)
            {

                if(WIZ_DispelAttempt__STUB(combat_enchantments[Target_X], IDK_mana, player_idx, spell_data_table[spell_idx].magic_realm) != ST_FALSE)
                {

                    Set_Page_Off();

                    Tactical_Combat_Draw();

                    PageFlip_FX();

                    Copy_On_To_Off_Page();

                    /* SPELLY */  CMB_CounterMessage__STUB(caster_idx, Opponent_Index, spell_idx, cnst_Counter_Magic);  // "Counter Magic"

                    if(caster_idx >= CASTER_IDX_BASE)
                    {

                        // ; BUG: this variable is still in use and needed below!
                        // This code is the same as the 'pay the cost' for when it actually gets cast, below?
                        // This bug could be because it was a macro?
                        Target_X = Combat_Casting_Cost_Multiplier((caster_idx - CASTER_IDX_BASE));

                        _players[(caster_idx - CASTER_IDX_BASE)].Cmbt_Skill_Left -= Effective_Cost;

                        _players[(caster_idx - CASTER_IDX_BASE)].mana_reserve -= (Target_X / 10);

                    }
                    else
                    {

                        if(Spell_Like_Ability != ST_TRUE)
                        {

                            // ; BUG: this may not be the hero's original owner
                            if(
                                (battle_units[caster_idx].Item_Charges > 0)
                                &&
                                (_ITEMS[_players[player_idx].Heroes[_UNITS[battle_units[caster_idx].unit_idx].Hero_Slot].Items[0]].embed_spell_idx == spell_idx)
                            )
                            {

                                battle_units[caster_idx].Item_Charges -= 1;

                            }
                            else
                            {

                                Effective_Cost -= battle_unit_mana;

                            }

                            if(battle_unit_mana < 0)
                            {

                                battle_unit_mana = 0;

                            }

                        }
                        else
                        {
                            
                            if(spell_idx == spl_Doom_Bolt)
                            {

                                battle_units[caster_idx].Attribs_2 ^= USA_DOOMBOLT;

                            }

                            if(spell_idx == spl_Web)
                            {

                                battle_units[caster_idx].Attribs_2 ^= USA_WEB;

                            }

                            if(spell_idx == spl_Fireball)
                            {

                                battle_units[caster_idx].Attribs_2 ^= USA_FIREBALL;

                            }

                            if(spell_idx == spl_Healing)
                            {

                                battle_units[caster_idx].Attribs_2 ^= USA_HEALING;

                            }

                        }

                        battle_units[caster_idx].movement_points = 0;

                    }

                    cast_status == 2;

                    spell_idx = ST_UNDEFINED;

                }

                combat_enchantments[Target_X] -= 5;

                if(combat_enchantments[Target_X] < 0)
                {

                    combat_enchantments[Target_X] = 0;

                    CMB_CE_Refresh__WIP();

                }

            }  /* if(combat_enchantments[Target_X] > 0) */


            if(battlefield->Central_Structure == CS_SorceryNode)
            {
                Target_X = sbr_Sorcery;
            }
            else if(battlefield->Central_Structure == CS_ChaosNode)
            {
                Target_X = sbr_Chaos;
            }
            else if(battlefield->Central_Structure == CS_NatureNode)
            {
                Target_X = sbr_Nature;
            }
            else
            {
                Target_X = ST_UNDEFINED;
            }

            if(
                (player_idx < _num_players)
                &&
                (_players[player_idx].node_mastery > 0)
            )
            {

                Target_X = ST_UNDEFINED;

            }

            if(
                (Target_X > ST_UNDEFINED)
                &&
                (spell_idx > ST_UNDEFINED)
                &&
                (spell_data_table[spell_idx].magic_realm != Target_X)
                &&
                (WIZ_DispelAttempt__STUB(50, IDK_mana, player_idx, spell_data_table[spell_idx].magic_realm) != ST_FALSE)
            )            
            {

                // NOTE: copied from above

                Set_Page_Off();

                Tactical_Combat_Draw();

                PageFlip_FX();

                Copy_On_To_Off_Page();

                // CMB_CounterMessage__STUB(caster_idx, Opponent_Index, spell_idx, cnst_Counter_Magic);  // "Counter Magic"
                /* SPELLY */  CMB_CounterMessage__STUB(caster_idx, 5, spell_idx, str_empty_string__ovr112);

                if(caster_idx >= CASTER_IDX_BASE)
                {

                    Target_X = Combat_Casting_Cost_Multiplier((caster_idx - CASTER_IDX_BASE));

                    _players[(caster_idx - CASTER_IDX_BASE)].Cmbt_Skill_Left -= Effective_Cost;

                    _players[(caster_idx - CASTER_IDX_BASE)].mana_reserve -= (Target_X / 10);

                }
                else
                {

                    if(Spell_Like_Ability != ST_TRUE)
                    {

                        // ; BUG: this may not be the hero's original owner
                        if(
                            (battle_units[caster_idx].Item_Charges > 0)
                            &&
                            (_ITEMS[_players[player_idx].Heroes[_UNITS[battle_units[caster_idx].unit_idx].Hero_Slot].Items[0]].embed_spell_idx == spell_idx)
                        )
                        {

                            battle_units[caster_idx].Item_Charges -= 1;

                        }
                        else
                        {

                            Effective_Cost -= battle_unit_mana;

                        }

                        if(battle_unit_mana < 0)
                        {

                            battle_unit_mana = 0;

                        }

                    }
                    else
                    {
                        
                        if(spell_idx == spl_Doom_Bolt)
                        {

                            battle_units[caster_idx].Attribs_2 ^= USA_DOOMBOLT;

                        }

                        if(spell_idx == spl_Web)
                        {

                            battle_units[caster_idx].Attribs_2 ^= USA_WEB;

                        }

                        if(spell_idx == spl_Fireball)
                        {

                            battle_units[caster_idx].Attribs_2 ^= USA_FIREBALL;

                        }

                        if(spell_idx == spl_Healing)
                        {

                            battle_units[caster_idx].Attribs_2 ^= USA_HEALING;

                        }

                    }

                    battle_units[caster_idx].movement_points = 0;

                }

                cast_status == 2;

                // spell_idx = ST_UNDEFINED;
                spell_idx = 0;

            }

        }
        /*
            END:  ¿ Counter Magic ?
        */

    }
    /*
        END:  
    */


    // ovr112:0A6D
    // @@Target_And_Effect:
    if(spell_idx > spl_NONE)
    {

        if(
            (spell_data_table[spell_idx].type == scc_Battlefield_Spell)
            ||
            (spell_data_table[spell_idx].type == scc_Counter_Spell)
            ||
            (spell_data_table[spell_idx].type == scc_Disenchant_Spell)
            ||
            (spell_data_table[spell_idx].type == scc_City_Enchantment)
            ||
            (spell_idx == spl_Animate_Dead)
        )
        {
            Target = 99;
            Target_X = 0;
            Target_Y = 0;
        }
        else
        {

            if(
                (player_idx == HUMAN_PLAYER_IDX)
                &&
                (_auto_combat_flag == ST_FALSE)
            )
            {

                Target = Combat_Spell_Target_Screen__WIP(spell_idx, &Target_X, &Target_Y);

            }
            else
            {

                Target = AITP_CombatSpell__STUB(spell_idx, player_idx, &Target_X, &Target_Y);

                if(Target != 99)
                {

                    Target_X = battle_units[Target].cgx;

                    Target_Y = battle_units[Target].cgy;

                }

            }

        }

        if(Target != 999)
        {

            G_CMB_SpellEffect__WIP(spell_idx, Target, caster_idx, Target_X, Target_Y, IDK_mana, ST_TRUE, ST_NULL, ST_NULL);

            cast_status = 2;

            if(caster_idx >= CASTER_IDX_BASE)
            {

                Target_X = Combat_Casting_Cost_Multiplier((caster_idx - CASTER_IDX_BASE));

                _players[caster_idx].Cmbt_Skill_Left -= Effective_Cost;

                _players[caster_idx].mana_reserve -= ((Effective_Cost * Target_X) / 10);

            }
            else  /* caster_idx < CASTER_IDX_BASE */
            {

                if(Spell_Like_Ability != ST_TRUE)
                {

                    // ; BUG: this may not be the hero's original owner
                    if(
                        (battle_units[caster_idx].Item_Charges > 0)
                        &&
                        (_ITEMS[_players[player_idx].Heroes[_UNITS[battle_units[caster_idx].unit_idx].Hero_Slot].Items[0]].embed_spell_idx == spell_idx)
                    )
                    {

                        battle_units[caster_idx].Item_Charges -= 1;

                    }
                    else
                    {

                        Effective_Cost -= battle_unit_mana;

                    }

                    if(battle_unit_mana < 0)
                    {

                        battle_unit_mana = 0;

                    }

                }
                else
                {
                    
                    if(spell_idx == spl_Doom_Bolt)
                    {

                        battle_units[caster_idx].Attribs_2 ^= USA_DOOMBOLT;

                    }

                    if(spell_idx == spl_Web)
                    {

                        battle_units[caster_idx].Attribs_2 ^= USA_WEB;

                    }

                    if(spell_idx == spl_Fireball)
                    {

                        battle_units[caster_idx].Attribs_2 ^= USA_FIREBALL;

                    }

                    if(spell_idx == spl_Healing)
                    {

                        battle_units[caster_idx].Attribs_2 ^= USA_HEALING;

                    }

                }

                battle_units[caster_idx].movement_points = 0;

            }  /* caster_idx < CASTER_IDX_BASE */

        }

    }

// AFTER:
//     spell_idx > 0
//     Target
//     Effect

    if(caster_idx < CASTER_IDX_BASE)
    {

        battle_units[caster_idx].mana = battle_unit_mana;

        if(
            (battle_unit_mana < 3)
            ||
            (battle_units[caster_idx].ammo == 0)
        )
        {

            // ; BUG: also removes short range attacks
            battle_units[caster_idx].ranged = 0;

            battle_units[caster_idx].ranged_type = ST_UNDEFINED;

        }

    }

    return cast_status;

}


// WZD o112p04
// drake178: CMB_ShowSpellbook()
/*
; displays the combat spellbook, allowing a spell to
; be selected for casting by the specified entity
; (player or unit)
; returns the index of the selected spell, and sets the
; passed pointer's value to the clicked control's index
; which indicates its current screen slot
*/
/*

¿ ~== Spellbook_Screen() ?
¿ ~== SmlBook_Draw__WIP() ?
*/
int16_t Combat_Spellbook_Screen(int16_t caster_idx, int16_t * selected_spell)
{
    int16_t spellbook_pages[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t spell_cost = 0;
    int16_t hotkey_B = 0;
    int16_t hotkey_F = 0;
    int16_t spellbook_page_spell_index = 0;
    int16_t spell_idx = 0;
    int16_t hotkey_ESC = 0;
    int16_t y_start = 0;
    int16_t input_field_idx = 0;
    int16_t leave_screen = 0;
    int16_t itr = 0;  // _SI_
    int16_t x_start = 0;  // _DI_

    Load_Palette_From_Animation(_spellbook_small_seg);

    Reset_Cycle_Palette_Color();

    Mark_Block(_screen_seg);

    IMG_SBK_Anims = Allocate_Next_Block(_screen_seg, 1090);

    // {Nature, Sorcery, Chaos, Life, Death, Arcane}
    for(itr = 0; itr < NUM_MAGIC_REALMS; itr++)
    {
        spellbook_symbols_bitm[itr] = Allocate_Next_Block(_screen_seg, 4);  // 4 PR  64 B
        Draw_Picture_To_Bitmap(_spellbook_small_symbols[itr], spellbook_symbols_bitm[itr]);
    }

    SBK_Dogears = 1;

    SBK_OpenPage = CMB_SpellBookPage;

    Assign_Auto_Function(Combat_Spellbook_Screen_Draw, 2);

    LBX_Load_Data_Static(hlpentry_lbx_file__ovr112, 0, _help_entries, 0, 15, 10);

    Set_Help_List(_help_entries, 15);

    Set_Outline_Color(ST_TRANSPARENT);

    Set_Font_Style_Shadow_Down(0, 3, 0, 0);

    Clear_Fields();

    x_start = 16;
    y_start = 12;

    for(itr = 0; itr < 6; itr++)
    {
        spellbook_pages[(itr + 0)] = Add_Hidden_Field((x_start + 16), (y_start + (itr * 22) + 17), (x_start + 137), (y_start + (itr * 22) + 34), str_empty_string__ovr112, ST_UNDEFINED);
    }

    for(itr = 0; itr < 6; itr++)
    {
        spellbook_pages[(itr + 6)] = Add_Hidden_Field((x_start + 148), (y_start + (itr * 22) + 17), (x_start + 268), (y_start + (itr * 22) + 34), str_empty_string__ovr112, ST_UNDEFINED);
    }

    hotkey_ESC = Add_Hidden_Field(x_start + 159, y_start + 154, x_start + 177, y_start + 183, ST_UNDEFINED, ST_UNDEFINED);
    hotkey_F   = Add_Hidden_Field(x_start + 259, y_start +   2, x_start + 272, y_start +  15, &str_hotkey_F__ovr112[0], ST_UNDEFINED);
    hotkey_B   = Add_Hidden_Field(x_start +  13, y_start +   2, x_start +  26, y_start +  14, &str_hotkey_B__ovr112[0], ST_UNDEFINED);

    leave_screen = ST_FALSE;

    while(leave_screen == ST_FALSE)
    {
        input_field_idx = Get_Input();

        /*
            Hot-Key ESCAPE
        */
        if(input_field_idx == hotkey_ESC)
        {
            Play_Left_Click__DUPE();
            leave_screen = ST_TRUE;
            spell_idx = 0;
        }

        /*
            Help Fields
        */
        for(itr = 0; itr < 6; itr++)
        {
            if(m_spellbook_pages[SBK_OpenPage].count > itr)
            {
                _help_entries[(3 + itr)].help_idx = abs(m_spellbook_pages[SBK_OpenPage].spell[itr]);
            }
            else
            {
                _help_entries[(3 + itr)].help_idx = ST_UNDEFINED;
            }
        }
        for(itr = 0; itr < 6; itr++)
        {
            if(m_spellbook_pages[(SBK_OpenPage + 1)].count > itr)
            {
                _help_entries[(9 + itr)].help_idx = abs(m_spellbook_pages[(SBK_OpenPage + 1)].spell[itr]);
            }
            else
            {
                _help_entries[(9 + itr)].help_idx = ST_UNDEFINED;
            }
        }

        /*
            Hot-Key Page Forward
        */
        if(input_field_idx == hotkey_F)
        {
            if((m_spellbook_page_count - 2) > SBK_OpenPage)
            {
                Release_Block(_screen_seg);
                Play_Left_Click__DUPE();
                // Spellbook_Screen()  SmlBook_PageTurn__WIP(1, 0, 0);
                SmlBook_PageTurn__WIP(1, 1, caster_idx);
                SBK_OpenPage += 2;
                Mark_Block(_screen_seg);
                IMG_SBK_Anims = Allocate_Next_Block(_screen_seg, 1090);
                for(itr = 0; itr < NUM_MAGIC_REALMS; itr++)
                {
                    spellbook_symbols_bitm[itr] = Allocate_Next_Block(_screen_seg, 4);  // 4 PR  64 B
                    Draw_Picture_To_Bitmap(_spellbook_small_symbols[itr], spellbook_symbols_bitm[itr]);
                }
            }
        }

        /*
            Hot-Key Page Backward
        */
        if(input_field_idx == hotkey_B)
        {
            if(SBK_OpenPage > 1)
            {
                Play_Left_Click__DUPE();
                // Spellbook_Screen()  SmlBook_PageTurn__WIP(0, 0, 0);
                SmlBook_PageTurn__WIP(0, 1, caster_idx);

                SBK_OpenPage -= 2;
                Mark_Block(_screen_seg);
                IMG_SBK_Anims = Allocate_Next_Block(_screen_seg, 1090);
                for(itr = 0; itr < NUM_MAGIC_REALMS; itr++)
                {
                    spellbook_symbols_bitm[itr] = Allocate_Next_Block(_screen_seg, 4);  // 4 PR  64 B
                    Draw_Picture_To_Bitmap(_spellbook_small_symbols[itr], spellbook_symbols_bitm[itr]);
                }
            }
        }

        
        /*
            BEGIN:  Left-Click Spellbook Page Spell Fields
        */
        for(itr = 0; itr < 12; itr++)
        {

            if(spellbook_pages[itr] == input_field_idx)
            {

                if(itr < 6)
                {

                    if(m_spellbook_pages[SBK_OpenPage].count > itr)
                    {
                        Play_Left_Click();
                        spellbook_page_spell_index = itr;
                        spell_idx = m_spellbook_pages[SBK_OpenPage].spell[itr];
                    }
                    else
                    {
                        spell_idx = ST_UNDEFINED;
                    }
                }
                else  /* (itr >= 6) */
                {
                    if(m_spellbook_pages[(SBK_OpenPage + 1)].count > (itr - 6))
                    {
                        Play_Left_Click();
                        spell_idx = m_spellbook_pages[(SBK_OpenPage + 1)].spell[(itr - 6)];
                        spellbook_page_spell_index = (itr - 6);
                    }
                    else
                    {
                        spell_idx = ST_UNDEFINED;
                    }
                }

                if(caster_idx < (CASTER_IDX_BASE - 1))
                {
                    spell_cost = spell_data_table[abs(spell_idx)].casting_cost;
                }
                else
                {
                    spell_cost = Casting_Cost(HUMAN_PLAYER_IDX, abs(spell_idx), 1);
                }

                if(
                    (spell_data_table[abs(spell_idx)].type == scc_Summoning)
                    &&
                    (CMB_combat_structure == cs_OceanTerrainType)
                    &&
                    ((_unit_type_table[spell_data_table[abs(spell_idx)].unit_type].Move_Flags & MV_FLYING) == 0)
                    &&
                    ((_unit_type_table[spell_data_table[abs(spell_idx)].unit_type].Abilities & UA_NONCORPOREAL) == 0)
                )
                {
                    spell_idx = ST_UNDEFINED;
                }

                if(
                    (
                        (spell_data_table[abs(spell_idx)].type == scc_City_Enchantment)
                        ||
                        (abs(spell_idx) == spl_Wall_Of_Stone)
                    )
                    &&
                    (
                        (_combat_attacker_player == HUMAN_PLAYER_IDX)
                        ||
                        (OVL_Action_Type != 1)  /* move onto Enemy City */
                    )
                )
                {
                    spell_idx = ST_UNDEFINED;
                }

                // ; BUG: this may not be the unit's original owner
                if(
                    (spell_cost > SBK_BookManaLimit)
                    &&
                    (caster_idx <= CASTER_IDX_BASE)
                    &&
                    (_ITEMS[_players[battle_units[caster_idx].controller_idx].Heroes[_UNITS[battle_units[caster_idx].unit_idx].Hero_Slot].Items[0]].embed_spell_idx != spell_idx)
                )
                {
                    spell_idx = ST_UNDEFINED;
                }

                if(spell_idx > ST_UNDEFINED)
                {
                    leave_screen = ST_TRUE;
                }

            }

        }

        /*
            END:  Left-Click Spellbook Page Spell Fields
        */

        if(leave_screen == ST_FALSE)
        {
            /* DEDU  ¿ Macro // Same as in preamble ? */
            Clear_Fields();
            x_start = 16;
            y_start = 12;
            for(itr = 0; itr < 6; itr++)
            {
                spellbook_pages[(itr + 0)] = Add_Hidden_Field((x_start + 16), (y_start + (itr * 22) + 17), (x_start + 137), (y_start + (itr * 22) + 34), str_empty_string__ovr112, ST_UNDEFINED);
            }
            for(itr = 0; itr < 6; itr++)
            {
                spellbook_pages[(itr + 6)] = Add_Hidden_Field((x_start + 148), (y_start + (itr * 22) + 17), (x_start + 268), (y_start + (itr * 22) + 34), str_empty_string__ovr112, ST_UNDEFINED);
            }
            hotkey_ESC = Add_Hidden_Field(x_start + 159, y_start + 154, x_start + 177, y_start + 183, ST_UNDEFINED, ST_UNDEFINED);
            hotkey_F   = Add_Hidden_Field(x_start + 259, y_start +   2, x_start + 272, y_start +  15, &str_hotkey_F__ovr112[0], ST_UNDEFINED);
            hotkey_B   = Add_Hidden_Field(x_start +  13, y_start +   2, x_start +  26, y_start +  14, &str_hotkey_B__ovr112[0], ST_UNDEFINED);

            Set_Page_Off();
            Combat_Spellbook_Screen_Draw();
            PageFlip_FX();
        }

    }

    CMB_SpellBookPage = SBK_OpenPage;

    Deactivate_Auto_Function();

    Deactivate_Help_List();

    Near_Allocate_Undo();

    Release_Block(_screen_seg);

    Clear_Fields();

    *selected_spell = spellbook_page_spell_index;

    CMB_ComposeBackgrnd__WIP();  // ... |-> Copy_Off_To_Back();

    return spell_idx;

}


// WZD o112p05
// drake178: CMB_CheckSpellErrors()
/*
; checks if there are any conditions in play that would
; prevent the specified spell from being cast without
; even selecting a target
; returns 1 and displays a warning if the spell can't
; be cast, or returns 0 if it can
;
; contains multiple BUGs and misses many conditions
; that could have been evaluated here
*/
/*

¿ BUGBUG no legal check for 'Dispel Evil' ?
*/
int16_t Do_Legal_Spell_Check__WIP(int16_t spell_idx)
{
    int16_t Controlled_Units = 0;
    int16_t illegal = 0;
    int16_t itr = 0;  // _SI_
    int16_t IDK = 0;  // _DI_

    illegal = ST_FALSE;

    if(spell_idx == 666)
    {
        return ST_TRUE;
    }

    if(spell_idx <= spl_NONE)
    {
        return ST_FALSE;
    }

    if(spell_idx == spl_Magic_Vortex)
    {
        if(CMB_Vortex_Count == 10)
        {
            LBX_Load_Data_Static(message_lbx_file__ovr112, 0, GUI_NearMsgString, 76, 1, 150);  // "Only ten vortexes can be in any area at once"
            Warn1(GUI_NearMsgString);
            illegal = ST_TRUE;
        }
    }

    if(spell_idx == spl_Word_Of_Recall)
    {
        if(_players[HUMAN_PLAYER_IDX].casting_spell_idx == spl_Spell_Of_Return)
        {
            LBX_Load_Data_Static(message_lbx_file__ovr112, 0, GUI_NearMsgString, 85, 1, 150);  // "Word of Recall and Recall Hero may not be cast while you are banished"
            Warn1(GUI_NearMsgString);
            illegal = ST_TRUE;
        }
    }

    if(spell_idx == spl_Recall_Hero)
    {
        IDK = ST_FALSE;
        if(_players[HUMAN_PLAYER_IDX].casting_spell_idx == spl_Spell_Of_Return)
        {
            LBX_Load_Data_Static(message_lbx_file__ovr112, 0, GUI_NearMsgString, 85, 1, 150);  // "Word of Recall and Recall Hero may not be cast while you are banished"
            Warn1(GUI_NearMsgString);
            illegal = ST_TRUE;
        }
        else if(
                (_combat_wx == _players[HUMAN_PLAYER_IDX].summon_wx)
                &&
                (_combat_wy == _players[HUMAN_PLAYER_IDX].summon_wy)
                &&
                (_combat_wp == _players[HUMAN_PLAYER_IDX].summon_wp)
        )
        {
            LBX_Load_Data_Static(message_lbx_file__ovr112, 0, GUI_NearMsgString, 77, 1, 150);  // "Recall Hero returns your heroes to your summoning circle and this battle is at your summoning circle"
            Warn1(GUI_NearMsgString);
            illegal = ST_TRUE;
        }
        else
        {
            for(itr = 0; ((itr < _combat_total_unit_count) && (IDK == ST_FALSE)); itr++)
            {
                if(
                    (battle_units[itr].controller_idx == HUMAN_PLAYER_IDX)
                    &&
                    (_UNITS[battle_units[itr].unit_idx].Hero_Slot > -1)
                )
                {
                    IDK = ST_TRUE;
                }
                if(IDK == ST_FALSE)
                {
                    LBX_Load_Data_Static(message_lbx_file__ovr112, 0, GUI_NearMsgString, 78, 1, 150);  // "There are no heroes left to recall"
                    Warn1(GUI_NearMsgString);
                    illegal = ST_TRUE;
                }
            }
        }
    }

    if(spell_idx == spl_Star_Fires)
    {
        IDK = ST_FALSE;
        for(itr = 0; ((itr < _combat_total_unit_count) && (IDK == ST_FALSE)); itr++)
        {
                if(
                    (battle_units[itr].controller_idx == HUMAN_PLAYER_IDX)
                    &&
                    (
                        (battle_units[itr].race == rt_Death)
                        ||
                        (battle_units[itr].race == rt_Chaos)
                    )
                )
                {
                    IDK = ST_TRUE;
                }
        }
        if(IDK == ST_FALSE)
        {
            LBX_Load_Data_Static(message_lbx_file__ovr112, 0, GUI_NearMsgString, 79, 1, 150);  // "There are no Chaos or Death units to throw this spell on"
            Warn1(GUI_NearMsgString);
            illegal = ST_TRUE;
        }
    }

    // ; BUG: the former does not recognize confused units as
    // ; not belonging to the player, while the latter counts
    // ; uninvolved, recalled, and fleeing units as valid
    // ; targets
    if(spell_idx == spl_Animate_Dead)
    {
        IDK = ST_FALSE;
        Controlled_Units = 0;
        for(itr = 0; ((itr < _combat_total_unit_count) && (IDK == ST_FALSE)); itr++)
        {
                if(
                    (battle_units[itr].status == bus_Active)
                    &&
                    (battle_units[itr].controller_idx == HUMAN_PLAYER_IDX)
                )
                {
                    Controlled_Units++;
                }
                if(
                    (battle_units[itr].status > bus_Active)
                    &&
                    (battle_units[itr].status != bus_Gone)
                    &&
                    (battle_units[itr].race != rt_Death)
                    &&
                    ((battle_units[itr].Attribs_1 & USA_IMMUNITY_MAGIC) == 0)
                    &&
                    (battle_units[itr].controller_idx == HUMAN_PLAYER_IDX)
                    &&
                    (_UNITS[battle_units[itr].unit_idx].Hero_Slot == -1)
                    &&
                    (_UNITS[battle_units[itr].unit_idx].wp != 9)
                )
                {
                    IDK = ST_TRUE;
                }
        }
        if(IDK == ST_FALSE)
        {
            LBX_Load_Data_Static(message_lbx_file__ovr112, 0, GUI_NearMsgString, 80, 1, 150);  // "There are no dead units that can be animated"
            Warn1(GUI_NearMsgString);
            illegal = ST_TRUE;
        }
        else if(Controlled_Units == MAX_STACK)
        {
            LBX_Load_Data_Static(message_lbx_file__ovr112, 0, GUI_NearMsgString, 84, 1, 150);  // "You may only control 9 units in combat at one time"
            Warn1(GUI_NearMsgString);
            illegal = ST_TRUE;
        }
    }

    // ; BUG: the former does not recognize confused units as
    // ; not belonging to the player, while the latter counts
    // ; uninvolved, recalled, and fleeing units as valid
    // ; targets
    /*
        Check if there are any (valid) *dead* units.
            invalid: status <= bus_Active (0)  ¿ ?
            invalid: controller is not Current/Human Player
            invalid: race >= rt_Arcane  ¿ ?
            
    */
    if(spell_idx == spl_Raise_Dead)
    {
        IDK = ST_FALSE;
        Controlled_Units = 0;
        for(itr = 0; ((itr < _combat_total_unit_count) && (IDK == ST_FALSE)); itr++)
        {
                if(
                    (battle_units[itr].status == bus_Active)
                    &&
                    (battle_units[itr].controller_idx == HUMAN_PLAYER_IDX)
                )
                {
                    Controlled_Units++;
                }
                if(
                    (battle_units[itr].status > bus_Active)
                    &&
                    (battle_units[itr].controller_idx == HUMAN_PLAYER_IDX)
                    &&
                    (battle_units[itr].race < rt_Arcane)
                    &&
                    (battle_units[itr].status != bus_Gone)
                    &&
                    (_UNITS[battle_units[itr].unit_idx].wp != 9)
                )
                {
                    IDK = ST_TRUE;
                }
        }
        if(IDK == ST_FALSE)
        {
            LBX_Load_Data_Static(message_lbx_file__ovr112, 0, GUI_NearMsgString, 81, 1, 150);  // "There are no dead units that can be raised from the dead"
            Warn1(GUI_NearMsgString);
            illegal = ST_TRUE;
        }
        else if(Controlled_Units == MAX_STACK)
        {
            LBX_Load_Data_Static(message_lbx_file__ovr112, 0, GUI_NearMsgString, 84, 1, 150);  // "You may only control 9 units in combat at one time"
            Warn1(GUI_NearMsgString);
            illegal = ST_TRUE;
        }
    }

    // ; BUG: the exclusions fail to include Mass Healing,
    // ; which then can't be cast with True Light active
    if(
        (spell_data_table[spell_idx].type == scc_Battlefield_Spell)
        ||
        (spell_data_table[spell_idx].type == scc_Counter_Spell)
        ||
        (spell_idx == spl_Holy_Word)
        ||
        (spell_idx == spl_Death_Spell)
        ||
        (spell_idx == spl_Flame_Strike)
        ||
        (spell_idx == spl_Call_Chaos)
    )
    {
        if(_combat_attacker_player == HUMAN_PLAYER_IDX)
        {
            IDK = 0;
        }
        else
        {
            IDK = 1;
        }
        if(combat_enchantments[(spell_data_table[spell_idx].Param0 + IDK)] > 0)
        {
            LBX_Load_Data_Static(message_lbx_file__ovr112, 0, GUI_NearMsgString, 82, 1, 150);  // "That combat enchantment is already in effect"
            Warn1(GUI_NearMsgString);
            illegal = ST_TRUE;
        }
    }

    // ; BUG: fails to check if the units are active or not
    if(spell_data_table[spell_idx].type == scc_Mundane_Enchantment)
    {
        IDK = ST_FALSE;
        for(itr = 0; ((itr < _combat_total_unit_count) && (IDK == ST_FALSE)); itr++)
        {
                if(
                    (battle_units[itr].controller_idx == HUMAN_PLAYER_IDX)
                    &&
                    (battle_units[itr].race < rt_Arcane)
                )
                {
                    IDK = ST_TRUE;
                }
        }
        if(IDK == ST_FALSE)
        {
            LBX_Load_Data_Static(message_lbx_file__ovr112, 0, GUI_NearMsgString, 83, 1, 150);  // "There are no normal units to throw this spell on"
            Warn1(GUI_NearMsgString);
            illegal = ST_TRUE;
        }
    }
    if(spell_data_table[spell_idx].type == scc_Mundane_Curse)
    {
        IDK = ST_FALSE;
        for(itr = 0; ((itr < _combat_total_unit_count) && (IDK == ST_FALSE)); itr++)
        {
                if(
                    (battle_units[itr].controller_idx == HUMAN_PLAYER_IDX)
                    &&
                    (battle_units[itr].race < rt_Arcane)
                )
                {
                    IDK = ST_TRUE;
                }
        }
        if(IDK == ST_FALSE)
        {
            LBX_Load_Data_Static(message_lbx_file__ovr112, 0, GUI_NearMsgString, 83, 1, 150);  // "There are no normal units to throw this spell on"
            Warn1(GUI_NearMsgString);
            illegal = ST_TRUE;
        }
    }

    // ; should also include Possession and Creature Binding
    if(spell_data_table[spell_idx].type == scc_Summoning)
    {
        if(_units == MAX_UNIT_COUNT)
        {
            LBX_Load_Data_Static(message_lbx_file__ovr112, 0, GUI_NearMsgString, 50, 1, 150);  // "Maximum number of units exceeded"
            Warn1(GUI_NearMsgString);
            illegal = ST_TRUE;
        }
        else
        {
            Controlled_Units = 0;
            for(itr = 0; itr < _combat_total_unit_count; itr++)
            {
                if(
                    (battle_units[itr].controller_idx == HUMAN_PLAYER_IDX)
                    &&
                    (battle_units[itr].status == bus_Active)
                )
                {
                    Controlled_Units++;
                }
            }
            if(Controlled_Units == MAX_STACK)
            {
                LBX_Load_Data_Static(message_lbx_file__ovr112, 0, GUI_NearMsgString, 84, 1, 150);  // "You may only control 9 units in combat at one time"
                Warn1(GUI_NearMsgString);
                illegal = ST_TRUE;
            }
        }
    }

    return illegal;

}


// WZD o112p06
// drake178: SPL_GetResistMod()
/*
; returns the resistance modifier for the specified
; spell, assigned through code rather than tables
*/
/*

Are these all Combat-Only spells?
Why is this not just in SPELLDAT?
Barring that, why is this not in that function that updates the static game data?

XREF:
    j_Spell_Resistance_Modifier()
        AITP_CombatSpell__STUB()
        AITP_CombatSpell__STUB()
        G_CMB_SpellEffect__WIP()

RESISTANCE TO SPELLS
...resistance to magic and poison (as symbolized by the number of crosses they have on their unit statistics window)
...base resistance...modifiers to resistance (spells and special unit abilities)...
...makes a resistance roll of 1 to 10. The roll is compared to the creature’s modified (if applicable) resistance.

Note that "saves" decrease the target's ability to resist the spell.
Each reduction has the effect of negating one enemy resistance (i.e., cross) or, in other words, reducing the enemy’s chance to save by 10% per point.

*/
int16_t Spell_Resistance_Modifier(int16_t spell_idx)
{
    int16_t resist_mod = 0;  // _DX_

    resist_mod = 0;

    // ¿ WTF ?  ; this is drake creating the database from the
    // ¿ WTF ?  ; wrong file... it is actually -1 ($FFFF) in v1.31
    // ¿ WTF ?  if(spell_idx == spl_Warp_Creature) { resist_mod = -15; }

    if(spell_idx == spl_Shatter)           { resist_mod =  0; }
    if(spell_idx == spl_Warp_Creature)     { resist_mod = -1; }
    if(spell_idx == spl_Weakness)          { resist_mod = -2; }
    if(spell_idx == spl_Black_Sleep)       { resist_mod = -2; }
    if(spell_idx == spl_Possession)        { resist_mod = -1; }
    if(spell_idx == spl_Black_Wind)        { resist_mod = -1; }
    if(spell_idx == spl_Terror)            { resist_mod =  1; }
    if(spell_idx == spl_Wrack)             { resist_mod =  1; }
    if(spell_idx == spl_Word_Of_Death)     { resist_mod = -5; }
    if(spell_idx == spl_Death_Spell)       { resist_mod = -2; }
    if(spell_idx == spl_Death_Wish)        { resist_mod =  0; }
    if(spell_idx == spl_Dispel_Evil)       { resist_mod = -4; }
    if(spell_idx == spl_Holy_Word)         { resist_mod = -2; }
    if(spell_idx == spl_Petrify)           { resist_mod =  0; }
    if(spell_idx == spl_Vertigo)           { resist_mod =  0; }
    if(spell_idx == spl_Banish)            { resist_mod = -3; }
    if(spell_idx == spl_Confusion)         { resist_mod = -4; }
    if(spell_idx == spl_Stasis)            { resist_mod = -5; }
    if(spell_idx == spl_Creature_Binding)  { resist_mod = -2; }
    if(spell_idx == spl_Great_Unsummoning) { resist_mod = -3; }

}


// WZD o112p07
/*    combat = asting cost multiplier; )
    Cost for Magic Reserve, not Casting Skill

¿ maximum distance is 30, because world is 60 and it wraps ?
¿ 30 is 300% AKA 3x (*3.0) ?
¿ Neutral Player is hard-coded to 10% ?
*/
int16_t Combat_Casting_Cost_Multiplier(int16_t player_idx)
{
    int16_t Y_Distance;
    int16_t modifier;  // _SI_
    int16_t X_Distance;  // _SI_  DNE in Dasm
    int16_t distance;  // _DI_

    if(player_idx == NEUTRAL_PLAYER_IDX)
    {
        modifier = 1;
    }
    else
    {

        if(_FORTRESSES[player_idx].wp != _combat_wp)
        {
            distance = (WORLD_WIDTH / 2);
        }
        else
        {
            Y_Distance = abs(_FORTRESSES[player_idx].wy - _combat_wy);

            X_Distance = abs(_FORTRESSES[player_idx].wx - _combat_wx);

            if(X_Distance > (WORLD_WIDTH / 2))
            {
                X_Distance = (WORLD_WIDTH - X_Distance);
            }

            if(X_Distance > (WORLD_WIDTH / 2))
            {
                X_Distance = (WORLD_WIDTH - X_Distance);
            }

            if(X_Distance < Y_Distance)
            {
                distance = X_Distance;
            }
            else
            {
                distance = Y_Distance;
            }
        }

        if(distance == 0)
        {
            modifier = 5;
        }
        else
        {

            if(_players[player_idx].channeler > 0)
            {
                distance = 2;
            }

            modifier = 10;

            if(distance > 5)
            {
                modifier += 5;
            }

            if(distance > 10)
            {
                modifier += 5;
            }

            if(distance > 15)
            {
                modifier += 5;
            }

            if(distance > 20)
            {
                modifier += 5;
            }

        }

    }

    return modifier;
}


// WZD o112p08
// drake178: AITP_EarthToMud()

// WZD o112p09
// drake178: AITP_Disrupt()

// WZD o112p10
// drake178: AITP_CracksCall()

// WZD o112p11
// drake178: UU_AITP_WordofRecall()

// WZD o112p12
// drake178: AITP_RecallHero()



/*
    WIZARDS.EXE  ovr113
*/

// WZD o113p01
// drake178: CMB_SetTargetCursor()
/*
; sets the GUI_CombatWindow@ structure as the active
; window and changes the cursor according to the spell
; being targeted and the object pointed at
;
; BUG: Wall Crusher units set the target index for wall
;  tiles to 99, which reads BU index #99 to determine
;  its cursor type and target validity
; BUG: tile targeting allows invalid ones
; BUG: Torin is treated as a fantastic unit
; BUG: the targeting frame remains off for tile spells
; BUG: some targeting types show a red X over the
;  bottom combat GUI
*/
/*

*/
void Combat_Set_Mouse_List_Image_Num(void)
{
    int16_t Pointer_Offset = 0;
    int16_t cgy = 0;
    int16_t screen_y = 0;
    int16_t screen_x = 0;
    int16_t Cursor_Unit = 0;
    int16_t cgx = 0;  // _SI_
    int16_t itr = 0;  // _DI_

    _combat_mouse_grid[0].image_num = crsr_Finger;

    _scanned_battle_unit = ST_UNDEFINED;

    Pointer_Offset = 4;

    screen_x = (Pointer_X() + Pointer_Offset);
    
    screen_y = (Pointer_Y() + Pointer_Offset);

    frame_scanned_flag = 0;

    if((168 + Pointer_Offset) > screen_y)
    {

        cgx = Get_Combat_Grid_Cell_X(screen_x, screen_y);

        cgy = Get_Combat_Grid_Cell_Y(screen_x, screen_y);

        if(CMB_TargetRows[cgy][cgx] == -2)
        {
            if(CMB_TargetingType < CTT_EnemyUnit)
            {
                _combat_mouse_grid[0].image_num =  crsr_CastBase;
            }
            else
            {
                _combat_mouse_grid[0].image_num =  crsr_RedCross;
            }
        }
        else if(CMB_TargetRows[cgy][cgx] == -1)
        {
            if(CMB_TargetingType < CTT_EnemyUnit)
            {
                _combat_mouse_grid[0].image_num =  crsr_CastBase;
            }
            else
            {
                _combat_mouse_grid[0].image_num =  crsr_RedCross;
            }
        }
        else
        {
            frame_scanned_flag = 1;
            frame_scanned_cgx = cgx;
            frame_scanned_cgy = cgy;
            frame_anim_cycle = ((frame_anim_cycle + 1) % 3);
            // ; BUG: this can be 99 for tiles with walls when a Wall
            // ; Crusher unit is selected while filling out the maps
            Cursor_Unit = CMB_TargetRows[cgy][cgx];
            _scanned_battle_unit = Cursor_Unit;
            if(battle_units[Cursor_Unit].controller_idx == HUMAN_PLAYER_IDX)
            {
                if(
                    (CMB_TargetingType == CTT_FriendlyUnit)
                    ||
                    (CMB_TargetingType == CTT_Tile)
                    ||
                    (CMB_TargetingType == CTT_DispelMagic)
                    ||
                    (
                        (CMB_TargetingType == CTT_FriendlyNU)
                        &&
                        (battle_units[Cursor_Unit].race == rt_Arcane)
                    )
                    ||
                    (
                        (CMB_TargetingType ==CTT_FriendlyHero)
                        &&
                        (_UNITS[battle_units[Cursor_Unit].unit_idx].Hero_Slot > -1)
                    )
                )
                {
                    _combat_mouse_grid[0].image_num = crsr_CastBase;
                }
                else
                {
                    _combat_mouse_grid[0].image_num = crsr_RedCross;
                }
            }
            else
            {
                if(
                    (CMB_TargetingType == CTT_EnemyUnit)
                    ||
                    (CMB_TargetingType == CTT_Tile)
                    ||
                    (CMB_TargetingType == CTT_DispelMagic)
                    ||
                    (
                        (CMB_TargetingType == CTT_EnemyNU)
                        &&
                        (battle_units[Cursor_Unit].race == rt_Arcane)
                    )
                )
                {
                    _combat_mouse_grid[0].image_num = crsr_CastBase;
                }
                else
                {
                    _combat_mouse_grid[0].image_num = crsr_RedCross;
                }
            }

        }

    }


    if(
        (CMB_TargetingType == CTT_Tile_NoUnitA)
        &&
        (cgx < 11)
    )
    {
        _combat_mouse_grid[0].image_num = crsr_RedCross;
    }

    if(
        (CMB_TargetingType == CTT_Tile_NoUnitD)
        &&
        (cgx > 10)
    )
    {
        _combat_mouse_grid[0].image_num = crsr_RedCross;
    }

    if(CMB_TargetingType == CTT_Wall)
    {
        _combat_mouse_grid[0].image_num = crsr_RedCross;
        if(Combat_Grid_Cell_Has_City_Wall(cgx, cgy) != ST_FALSE)
        {
            _combat_mouse_grid[0].image_num = crsr_CastBase;
        }
    }

    if(CMB_TargetingType == CTT_DispelMagic)
    {
        for(itr = 0; itr < CMB_Vortex_Count; itr++)
        {
            if(
                (CMB_Vortex_Array[itr].cgx == cgx)
                &&
                (CMB_Vortex_Array[itr].cgy == cgy)
            )
            {
                _combat_mouse_grid[0].image_num = crsr_CastBase;
            }
        }
    }

    SBK_NewSpellAnim_Stg = ((SBK_NewSpellAnim_Stg + 1) % 5);

    _combat_mouse_grid[0].center_offset = 2;
    _combat_mouse_grid[0].x1 = SCREEN_XMIN;
    _combat_mouse_grid[0].y1 = SCREEN_YMIN;
    _combat_mouse_grid[0].x2 = SCREEN_XMAX;
    _combat_mouse_grid[0].y2 = SCREEN_YMAX;

    Set_Mouse_List(1, &_combat_mouse_grid[0]);

}


// WZD o113p02
// drake178: CMB_DrawTargetScreen()
/*
; redraws the combat screen with the spell targeting
; message panel, and sets the active window and cursor
; according to the object being pointed at
;
; inherits various BUGs from GUI_SetCmbTargetCrsr
*/
/*

~== SCastScr  Spell_Casting_Screen__WIP() |-> Set_Mouse_List_Image_Nums()

*/
void Combat_Spell_Target_Screen_Draw(void)
{
    SAMB_ptr Msg_Panel_IMG;

    Tactical_Combat_Draw();

    Mark_Block(_screen_seg);

    // COMBATFX.LBX, 028  "MSGPANEL"
    Msg_Panel_IMG = LBX_Reload_Next(cmbtfx_lbx_file__ovr113, 28, _screen_seg);

    Release_Block(_screen_seg);

    FLIC_Draw(238, 164, Msg_Panel_IMG);

    Set_Outline_Color(240);

    Set_Alias_Color(227);

    Set_Font_Style_Shadow_Down(0, 0, 0, 0);

    Set_Font_Spacing_Width(1);

    Print_Paragraph(241, 168, 75, GUI_NearMsgString, 0);

    Combat_Set_Mouse_List_Image_Num(); 

}


// WZD o113p03
// drake178: CMB_TargetSpell()
/*

Combat_Cast_Spell__WIP()
    Target = CMB_TargetSpell__WIP(spell_idx, &Target_X, &Target_Y);

*/
int16_t Combat_Spell_Target_Screen__WIP(int16_t spell_idx, int16_t * target_cgx, int16_t * target_cgy)
{
    char spell_name[LEN_SPELL_NAME] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t BU_Loop_Index = 0;
    uint32_t enchantments = 0;
    int16_t Vortex_Index = 0;
    int16_t target_idx = 0;
    int16_t input_field_idx = 0;
    int64_t Grid_Y = 0;
    int64_t Grid_X = 0;
    int16_t cancel_button_field = 0;
    int16_t combat_grid_field = 0;
    int16_t leave_screen = 0;  // _DI_

    Assign_Auto_Function(Combat_Spell_Target_Screen_Draw, 2);

    SBK_NewSpellAnim_Stg = 0;

    if(spell_idx == spl_NONE)
    {
        CMB_TargetingType = CTT_Tile_NoUnit;
    }
    else
    {
        switch(spell_data_table[spell_idx].type)
        {
            case scc_Summoning:
            {
                if(_combat_attacker_player == HUMAN_PLAYER_IDX)
                {
                    CMB_TargetingType = CTT_Tile_NoUnitA;
                }
                else
                {
                    CMB_TargetingType = CTT_Tile_NoUnitD;
                }
            } // FALL-THROUGH
            case scc_Special_Spell:
            {
                if(spell_idx == spl_Healing)
                {
                    CMB_TargetingType = CTT_FriendlyUnit;
                }
                if(
                    (spell_idx == spl_Raise_Dead)
                    ||
                    (spell_idx == spl_Animate_Dead)
                )
                {
                    if(_combat_attacker_player == HUMAN_PLAYER_IDX)
                    {
                        CMB_TargetingType = CTT_Tile_NoUnitA;
                    }
                    else
                    {
                        CMB_TargetingType = CTT_Tile_NoUnitD;
                    }
                }
                if(
                    (spell_idx == spl_Warp_Creature)
                    ||
                    (spell_idx == spl_Creature_Binding)
                )
                {
                    CMB_TargetingType == CTT_EnemyUnit;
                }
                if(
                    (spell_idx == spl_Earth_To_Mud)
                    ||
                    (spell_idx == spl_Cracks_Call)
                )
                {
                    CMB_TargetingType == CTT_Tile;
                }
                if(spell_idx == spl_Magic_Vortex)
                {
                    CMB_TargetingType == CTT_Tile_NoUnit;
                }
                if(spell_idx == spl_Disrupt)
                {
                    CMB_TargetingType = CTT_Wall;
                }
                if(spell_idx == spl_Recall_Hero)
                {
                    CMB_TargetingType = CTT_FriendlyHero;
                }
                if(spell_idx == spl_Word_Of_Recall)
                {
                    CMB_TargetingType = CTT_FriendlyUnit;
                }
            } break;
            case scc_Unit_Enchantment:
            {
                CMB_TargetingType = CTT_FriendlyUnit;
            } break;
            case scc_Fixed_Dmg_Spell:
            {
                CMB_TargetingType = CTT_EnemyUnit;
            } break;
            case scc_Mundane_Enchantment:
            {
                CMB_TargetingType = CTT_FriendlyNU;
            } break;
            case scc_Mundane_Curse:
            {
                CMB_TargetingType = CTT_EnemyNU;
            } break;
            case scc_Dispel_Spell:
            {
                CMB_TargetingType = CTT_DispelMagic;
            } break;
        }
// scc_City_Enchantment    =  2,
// scc_City_Curse              =  3,
// scc_Target_Wiz_Spell        =  6,
// scc_Global_Enchantment  =  9,
// scc_Battlefield_Spell   = 10,
// scc_Crafting_Spell      = 11,
// scc_Destruction_Spell       = 12,
// scc_Resistable_Spell        = 13,
// scc_Unresistable_Spell         = 14,
// scc_Infusable_Spell     = 17,
// Disenchant_Spell        = 19,
// scc_Disjunction_Spell       = 20,
// scc_Counter_Spell       = 21,
// scc_Var_Dmg_Spell           = 22,
// scc_Banish_Spell            = 23
    }

    Clear_Fields();

    combat_grid_field = Add_Grid_Field(0, 0, 1, 1, 319, 168, &Grid_X, &Grid_Y, ST_UNDEFINED);

    cancel_button_field = Add_Button_Field(263, 186, str_empty_string__ovr113, _cmbt_cancel_button_seg, str_hotkey_ESC__ovr113, ST_UNDEFINED);

    leave_screen = ST_FALSE;

    while(leave_screen == ST_FALSE)
    {

        strcpy(GUI_NearMsgString, cnst_CmbSpellMsg1);  // "Select a target for a "
        String_Copy_Far(spell_name, spell_data_table[spell_idx].name);
        strcat(GUI_NearMsgString, spell_name);
        strcat(GUI_NearMsgString, cnst_SpaceSpellDot_3);  // " spell."

        Mark_Time();

        input_field_idx = Get_Input();

        if(input_field_idx == cancel_button_field)
        {
            leave_screen = ST_TRUE;
            target_idx = 999;
        }

        if(input_field_idx == combat_grid_field)
        {

            *target_cgx = Get_Combat_Grid_Cell_X((Grid_X + 4), (Grid_Y + 4));

            *target_cgy = Get_Combat_Grid_Cell_Y((Grid_X + 4), (Grid_Y + 4));

            if(
                (CMB_TargetingType == CTT_Wall)
                &&
                (Combat_Grid_Cell_Has_City_Wall(*target_cgx, *target_cgy) != ST_FALSE)
            )
            {
                // ; BUG: the return value is undefined!
                leave_screen = ST_TRUE;
                continue;
            }

            if(CMB_TargetingType == CTT_DispelMagic)
            {
                STU_DEBUG_BREAK();
            }

            target_idx = CMB_TargetRows[*target_cgy][*target_cgx];
            
            if(target_idx < 0)
            {

                if(
                    (CMB_TargetingType == CTT_Tile_NoUnit)
                    ||
                    (CMB_TargetingType == CTT_Tile)
                )
                {
                    leave_screen = ST_TRUE;
                    target_idx = 99;  // ; BUG: allows targeting tiles outside the playfield
                }

                // ; BUG: allows summoning on invalid tiles
                if(
                    (CMB_TargetingType == CTT_Tile_NoUnitD)
                    &&
                    (*target_cgx < 11)
                )
                {
                    leave_screen = ST_TRUE;
                    target_idx = 99;
                    for(BU_Loop_Index = 0; ((BU_Loop_Index < _combat_total_unit_count) && (leave_screen == ST_TRUE)); BU_Loop_Index++)
                    {
                        if(
                            (battle_units[BU_Loop_Index].status == bus_Active)
                            &&
                            (battle_units[BU_Loop_Index].cgx == *target_cgx)
                            &&
                            (battle_units[BU_Loop_Index].cgy == *target_cgy)
                        )
                        {
                            leave_screen = ST_FALSE;
                            LBX_Load_Data_Static(message_lbx_file__ovr113__1of2, 0, GUI_NearMsgString, 88, 1, 150);
                            Warn1(GUI_NearMsgString);
                        }
                    }
                }

                // ; BUG: allows summoning on invalid tiles
                if(
                    (CMB_TargetingType == CTT_Tile_NoUnitA)
                    &&
                    (*target_cgx >= 11)
                )
                {
                    leave_screen = ST_TRUE;
                    target_idx = 99;
                    for(BU_Loop_Index = 0; ((BU_Loop_Index < _combat_total_unit_count) && (leave_screen == ST_TRUE)); BU_Loop_Index++)
                    {
                        if(
                            (battle_units[BU_Loop_Index].status == bus_Active)
                            &&
                            (battle_units[BU_Loop_Index].cgx == *target_cgx)
                            &&
                            (battle_units[BU_Loop_Index].cgy == *target_cgy)
                        )
                        {
                            leave_screen = ST_FALSE;
                            LBX_Load_Data_Static(message_lbx_file__ovr113__1of2, 0, GUI_NearMsgString, 88, 1, 150);  // "There is an invisible unit in that square."
                            Warn1(GUI_NearMsgString);
                        }
                    }
                }

            }
            else
            {

                enchantments = (_UNITS[battle_units[target_idx].unit_idx].enchantments | battle_units[target_idx].item_enchantments | battle_units[target_idx].enchantments);

                if(battle_units[target_idx].controller_idx == HUMAN_PLAYER_IDX)
                {
                    
                    if(
                        (CMB_TargetingType == CTT_FriendlyUnit)
                        ||
                        (CMB_TargetingType == CTT_Tile)
                        ||
                        (CMB_TargetingType == CTT_DispelMagic)
                    )
                    {
                        leave_screen == ST_TRUE;
                    }



                }
                else
                {

                }

            }



        }

        if(leave_screen == ST_FALSE)
        {
            strcpy(GUI_NearMsgString, cnst_CmbSpellMsg1);  // "Select a target for a "
            String_Copy_Far(spell_name, spell_data_table[spell_idx].name);
            strcat(GUI_NearMsgString, spell_name);
            strcat(GUI_NearMsgString, cnst_SpaceSpellDot_3);  // " spell."

            Combat_Spell_Target_Screen_Draw();

            PageFlip_FX();

            // ; BUG: reduces responsiveness, should be 1
            // ; ticks
            Release_Time(2);

        }

    }

    Clear_Fields();

    Assign_Auto_Function(Tactical_Combat_Draw, 2);

    return target_idx;

}


// WZD o113p04
// drake178: CMB_RangedAnim()
/*
; performs a ranged attack animation by the chosen
; unit against the target specified, using projectiles
; based on the unit's ranged attack type
*/
/*

*/
void CMB_RangedAnim__STUB(int16_t attacker_battle_unit_idx, int16_t defender_battle_unit_idx, int16_t defender_damage_total, int16_t cgx, int16_t cgy)
{

    return;

}


// WZD o113p05
// drake178: CMB_MeleeAnim()
/*
performs the melee attack animation between the
specified two units, or shows the chosen unit
attacking the tile at the selected coordinates (used
when targeting walls with Wall Crushers)*/
/*

*/
void CMB_MeleeAnim__STUB(int16_t attacker_battle_unit_idx, int16_t defender_battle_unit_idx, int16_t attacker_damage, int16_t defender_damage, int16_t cgx, int16_t cgy)
{

    return;

}


// Segrax
// WZD o113p06
// drake178: CMB_ConvSpellAttack()
/*
*/
/*



*/
void CMB_ConvSpellAttack(uint16_t spell_idx, uint16_t battle_unit_idx, int16_t* Dmg_Array, int16_t ATK_Override) {
    int32_t Enchants = 0;
    int16_t Total_Damage = 0;
    int16_t Figures_Slain = 0;
    int16_t To_Block = 0;
    int16_t Attack_Count = 0;
    int16_t Attack_Strength = 0;
    int16_t Effective_Defense = 0;
    int16_t Immunity_Flags = 0;
    int16_t Attack_Flags = 0;
    int16_t Top_Figure_Damage = 0;
    uint16_t Loop_Var = 0;

    struct s_BATTLE_UNIT* battleunit = &battle_units[battle_unit_idx];
    struct s_UNIT* unit = &_UNITS[battleunit->unit_idx];

    Enchants = unit->enchantments | battleunit->enchantments | battleunit->item_enchantments;

    for (Loop_Var = 0; Loop_Var < 3; Loop_Var++) {
        Dmg_Array[Loop_Var] = 0;
    }

    if (Enchants & UE_RIGHTEOUSNESS) {
        struct s_SPELL_DATA* spell = &spell_data_table[spell_idx];
        if (spell->magic_realm == sbr_Chaos || spell->magic_realm == sbr_Death) {
            return;
        }
    }

    if (battleunit->Attribs_1 & USA_IMMUNITY_MAGIC) {
        return;
    }

    Top_Figure_Damage = battleunit->TopFig_Dmg;

    struct s_SPELL_DATA* spell = &spell_data_table[spell_idx];
    Attack_Flags = spell->Params2_3;

    To_Block = battleunit->To_Block;
    if (Attack_Flags & Att_EldrWeap) {
        To_Block--;
    }

    Immunity_Flags = spell->Param1;

    if (ATK_Override > 0) {
        Attack_Strength = ATK_Override;
    } else {
        Attack_Strength = spell->Param0;
    }

    Effective_Defense = BU_GetEffectiveDEF__SEGRAX(battle_unit_idx, 0x26, Immunity_Flags, Attack_Flags, spell->magic_realm);

    if (Attack_Flags & Att_AREAFLAG) {
        Attack_Count = battleunit->Cur_Figures;
        Attack_Flags |= Att_DMGLIMIT;
    }
    else if (Attack_Flags & Att_WarpLghtn) {
        Attack_Count = Attack_Strength;
    }
    else {
        Attack_Count = 1;
    }

    if (battleunit->Combat_Effects & bue_Black_Sleep) {
        Attack_Flags |= Att_DoomDmg;
    }

    for (Loop_Var = 0; Loop_Var < Attack_Count; Loop_Var++) {
        int16_t damage = 0;

        if (Attack_Flags & Att_DoomDmg) {
            damage = Attack_Strength;
        }
        else {
            damage = CMB_AttackRoll__SEGRAX(Attack_Strength, 0) - CMB_DefenseRoll__SEGRAX(Effective_Defense, To_Block);
            if (Enchants & UE_INVULNERABILITY) {
                damage -= 2;
            }
        }

        if (damage < 0) damage = 0;

        if (Loop_Var == 0 && Top_Figure_Damage > 0) {
            damage += Top_Figure_Damage;
            Top_Figure_Damage = 0;
        }

        if (Top_Figure_Damage < 0) {
            Top_Figure_Damage += damage;
            if (Top_Figure_Damage > 0) {
                damage = Top_Figure_Damage;
                Top_Figure_Damage = 0;
            }
            else {
                damage = 0;
            }
        }

        if (Attack_Flags & Att_DMGLIMIT) {
            if (battleunit->hits <= damage) {
                Figures_Slain++;
                damage = 0;
            }
            else {
                battleunit->hits -= damage;
            }
        }
        else {
            while (battleunit->hits < damage) {
                Figures_Slain++;

                damage -= battleunit->hits;

                if (!(Attack_Flags & Att_DoomDmg)) {

                    damage -= CMB_DefenseRoll__SEGRAX(Effective_Defense, To_Block);

                    if (Enchants & UE_INVULNERABILITY) {
                        damage -= 2;
                    }
                }
            } 

            if (damage < 0) {
                damage = 0;
            }
        }

        //ovr113:1A6B
        Total_Damage += battleunit->hits * Figures_Slain;
        Figures_Slain = 0;

        if (Attack_Flags & Att_WarpLghtn) {
            Attack_Strength--;
        }
    }

    Total_Damage -= battleunit->TopFig_Dmg;

    if (Total_Damage < 0) {
        Total_Damage = 0;
    }

    Dmg_Array[0] = Total_Damage;
}


// Segrax
// WZD o113p07
// drake178: BU_ApplyDamage()
/*
; applies the damage points passed through the array
; to the target battle unit, reducing its figure count
; or marking it dead as necessary (in which case the
; combat victor and true sights are also updated)
*/
/*

*/
void BU_ApplyDamage__WIP(int16_t battle_unit_idx, int16_t Dmg_Array[])
{
    int16_t Figures_Lost = 0;
    int16_t Total_Damage = 0;
    int16_t itr = 0;  // _DI_

    Total_Damage = 0;

    for(itr = 0; itr < 3; itr++)
    {
        Total_Damage += Dmg_Array[itr];
    }

    if(
        (Total_Damage > 0)
        &&
        (battle_units[battle_unit_idx].status == 0)  /* Unit_Active */
    )
    {
        for(itr = 0; itr < 3; itr++)
        {
            if((battle_units[battle_unit_idx].damage[itr] + Dmg_Array[itr]) <= 200)
            {
                battle_units[battle_unit_idx].damage[itr] += Dmg_Array[itr];
            }
            else
            {
                battle_units[battle_unit_idx].damage[itr] = 200;
            }
        }
    }

    Total_Damage += battle_units[battle_unit_idx].TopFig_Dmg;

    if(Total_Damage > 0)
    {
        Figures_Lost = (Total_Damage / battle_units[battle_unit_idx].hits);

        if(battle_units[battle_unit_idx].Cur_Figures < Figures_Lost)
        {
            Figures_Lost = battle_units[battle_unit_idx].Cur_Figures;
        }

        battle_units[battle_unit_idx].Cur_Figures -= Figures_Lost;

        battle_units[battle_unit_idx].TopFig_Dmg = (Total_Damage % battle_units[battle_unit_idx].hits);

    }

    if(battle_units[battle_unit_idx].Cur_Figures <= 0)
    {
        battle_units[battle_unit_idx].Cur_Figures = 0;

        CMB_Winner = Check_For_Winner();

        if(BU_IsCombatSummon__WIP(battle_unit_idx) == ST_TRUE)
        {
            _UNITS[battle_units[battle_unit_idx].unit_idx].wp = 9;
        }

        if(
            (battle_units[battle_unit_idx].damage[2] >= battle_units[battle_unit_idx].damage[1])
            &&
            (battle_units[battle_unit_idx].damage[2] >= battle_units[battle_unit_idx].damage[0])
        )
        {
            battle_units[battle_unit_idx].status = 6;  /* Unit_Gone */
        }
        else
        {
            if(
                (battle_units[battle_unit_idx].damage[1] > battle_units[battle_unit_idx].damage[2])
                &&
                (battle_units[battle_unit_idx].damage[1] >= battle_units[battle_unit_idx].damage[0])
            )
            {
                if(_UNITS[battle_units[battle_unit_idx].unit_idx].wp != 9)
                {
                    battle_units[battle_unit_idx].status = 5;  /* Unit_Drained */
                }
                else
                {
                    battle_units[battle_unit_idx].status = 6;  /* Unit_Gone */
                }
            }
            else
            {
                if(
                    (battle_units[battle_unit_idx].damage[0] > battle_units[battle_unit_idx].damage[2])
                    &&
                    (battle_units[battle_unit_idx].damage[0] > battle_units[battle_unit_idx].damage[1])
                )
                {
                    battle_units[battle_unit_idx].status = 4;  /* Unit_Dead */
                }
            }
        }

        CMB_UpdateTrueSight();
    }

}


// WZD o113p08
// drake178: BU_RangedValidate()
/*
; returns 0 if the target can be attacked at range, 1
; if this is prevented by Invisibility, or 2 if by Wall
; of Darkness
;
; BUG: the Wall of Darkness check ignores natural
; Illusions Immunity, disallowing the attack
*/
/*

returns {0,1,2}
0: allow ranged attack
1: disallow ranged attack, due to 'Invisibility'
2: disallow ranged attack, due to 'Wall of Darkness'

compliment to Check_Attack_Melee()

*/
int16_t Check_Attack_Ranged(int16_t attacker_battle_unit_idx, int16_t defender_battle_unit_idx)
{
    int16_t result = 0;
    uint32_t defender_enchantments;
    uint32_t attacker_enchantments;

    defender_enchantments = (battle_units[defender_battle_unit_idx].enchantments | battle_units[defender_battle_unit_idx].item_enchantments);

    attacker_enchantments = (battle_units[attacker_battle_unit_idx].enchantments | battle_units[attacker_battle_unit_idx].item_enchantments);

    result = 0;

    if(
        ((attacker_enchantments & UE_TRUESIGHT) != 0)
        ||
        ((battle_units[attacker_battle_unit_idx].Attribs_1 & USA_IMMUNITY_ILLUSION) != 0)
    )
    {

        result = 0;

    }
    else
    {

         if(
            ((defender_enchantments & UE_INVISIBILITY) != 0)
            ||
            ((battle_units[defender_battle_unit_idx].Abilities & UA_INVISIBILITY) != 0)
        )
        {

            result = 1;

        }

    }

    if(battlefield->Wall_of_Darkness == 1)
    {

        // ; BUG: ignores natural Illusions Immunity
        if((attacker_enchantments & UE_TRUESIGHT) == 0)
        {

            if(BU_IsInCityProper__STUB(defender_battle_unit_idx) == ST_TRUE)
            {
                
                if(BU_IsInCityProper__STUB(attacker_battle_unit_idx) == ST_FALSE)
                {

                    result = 2;

                }

            }

        }

    }

    return result;

}


// WZD o113p09
// drake178: CMB_PlaceArmy()
/*
; marks landlubbers if necessary, then positions's the
; target player's forces on the combat map using a set
; of predetermined coordinates, melee units up front
; followed by ranged ones
*/
/*
    sets cgx,y and target_cgx,cgy

*/
void Deploy_Battle_Units(int16_t player_idx)
{
    int16_t starting_cgy[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t starting_cgx[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Ranged_Units[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Melee_Units[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t MoveFront_Count = 0;
    int16_t Highest_Melee_Unit = 0;
    int16_t Highest_Melee = 0;
    int16_t Loop_Var = 0;
    int16_t Ranged_Count = 0;
    int16_t Melee_Count = 0;
    int16_t itr = 0;  // _SI_
    int16_t ctr = 0;  // _DI_

    Undeployable_Battle_Units_On_Water(player_idx);

    if(player_idx == _combat_defender_player)
    {
        starting_cgx[ 0] = 8;
        starting_cgx[ 1] = 8;
        starting_cgx[ 2] = 8;
        starting_cgx[ 3] = 8;
        starting_cgx[ 4] = 7;
        starting_cgx[ 5] = 7;
        starting_cgx[ 6] = 7;
        starting_cgx[ 7] = 7;
        starting_cgx[ 8] = 6;
        starting_cgx[ 9] = 6;
        starting_cgx[10] = 6;
        starting_cgx[11] = 6;
    }
    else
    {
        starting_cgx[ 0] = 14;
        starting_cgx[ 1] = 14;
        starting_cgx[ 2] = 14;
        starting_cgx[ 3] = 14;
        starting_cgx[ 4] = 15;
        starting_cgx[ 5] = 15;
        starting_cgx[ 6] = 15;
        starting_cgx[ 7] = 15;
        starting_cgx[ 8] = 16;
        starting_cgx[ 9] = 16;
        starting_cgx[10] = 16;
        starting_cgx[11] = 16;
    }

    starting_cgy[ 0] = 12;
    starting_cgy[ 1] = 11;
    starting_cgy[ 2] = 13;
    starting_cgy[ 3] = 10;
    starting_cgy[ 4] = 12;
    starting_cgy[ 5] = 11;
    starting_cgy[ 6] = 13;
    starting_cgy[ 7] = 10;
    starting_cgy[ 8] = 12;
    starting_cgy[ 9] = 11;
    starting_cgy[10] = 13;
    starting_cgy[11] = 10;


    Melee_Count = 0;
    Ranged_Count = 0;

    for(itr = 0; itr < _combat_total_unit_count; itr++)
    {

        if(battle_units[itr].controller_idx == player_idx)
        {

            if(Battle_Unit_Has_Ranged_Attack(itr) != ST_FALSE)
            {

                Ranged_Units[Ranged_Count] = itr;

                Ranged_Count++;

            }
            else
            {

                Melee_Units[Melee_Count] = itr;

                Melee_Count++;

            }

        }

    }


    // if there are more than 5 ranged units, move the ones
    // with the highest melee strength over to the melee
    // group until there are either at least 4 units there,
    // or only 4 left in the ranged group
    if(Ranged_Count > 5)
    {

        MoveFront_Count = (Ranged_Count - 4);

        // ; find the ranged unit with the highest melee strength
        for(itr = 0; ((itr < MoveFront_Count) & (Melee_Count < 4)); itr++)
        {

            Highest_Melee = -1;

            Highest_Melee_Unit = ST_UNDEFINED;

            for(Loop_Var = 0; Loop_Var < Ranged_Count; Loop_Var++)
            {

                if(battle_units[Loop_Var].melee > Highest_Melee)
                {

                    Highest_Melee = battle_units[Loop_Var].melee;

                    Highest_Melee_Unit = Loop_Var;

                }

            }

            if(Melee_Count < 4)
            {

                Melee_Units[Melee_Count] = Ranged_Units[Highest_Melee_Unit];

                Melee_Count++;

                Clear_Structure(Highest_Melee_Unit, (uint8_t *)&Ranged_Units, 2, Ranged_Count);

                Ranged_Count--;

            }

        }

    }


    // place the melee units, starting from the front of the
    // local coordinate array, and skipping wall corner and
    // central structure tiles for the defender
    ctr = 0;
    for(itr = 0; itr < Melee_Count; itr++)
    {

        if(_combat_defender_player == player_idx)
        {

            while(
                (battlefield->Walled_City == ST_TRUE)
                &&
                (
                    (ctr == 2)
                    ||
                    (ctr == 3)
                )
            )
            {
                ctr++;
            }

            if(
                (battlefield->Central_Structure != CS_City)
                &&
                (ctr == 9)
            )
            {
                ctr++;
            }

        }

        battle_units[Melee_Units[itr]].cgx = starting_cgx[ctr];

        battle_units[Melee_Units[itr]].cgy = starting_cgy[ctr];

        ctr++;

    }


    // place the ranged units, skipping wall corner and
    // central structure tiles for the defender
    for(itr = 0; itr < Ranged_Count; itr++)
    {

        if(_combat_defender_player == player_idx)
        {

            while(
                (battlefield->Walled_City == ST_TRUE)
                &&
                (
                    (ctr == 2)
                    ||
                    (ctr == 3)
                )
            )
            {
                ctr++;
            }

            if(
                (battlefield->Central_Structure != CS_City)
                &&
                (ctr == 9)
            )
            {
                ctr++;
            }

        }

        battle_units[Ranged_Units[itr]].cgx = starting_cgx[ctr];

        battle_units[Ranged_Units[itr]].cgy = starting_cgy[ctr];

        ctr++;

    }


    for(itr = 0; itr < _combat_total_unit_count; itr++)
    {

        if(battle_units[itr].controller_idx == player_idx)
        {

            if(_combat_attacker_player == player_idx)  // attacker */
            {

                battle_units[itr].target_cgx = (battle_units[itr].cgx - 1);  // face left/west

            }
            else
            {

                battle_units[itr].target_cgx = (battle_units[itr].cgx + 1);  // face right/east

            }

            battle_units[itr].target_cgy = battle_units[itr].cgy;

        }

    }

}


// segrax
// WZD o113p10
// drake178: BU_SummonDemon()
/*
; processes summoning a demon by the specified unit,
; from decrementing its available summon counter to
; creating the unit and playing the summon animation
;
; BUG: will enter an infinite loop if all 9 tiles that
; the unit's side can summon to are occupied
*/
/*
    ~=== Demon Lord's Summon Demon
    2 USA bits are used as the count of summonable demons
*/
void BU_SummonDemon__SEGRAX(int16_t caster_idx)
{
    int16_t cgy = 0;
    int16_t caster_attribs_1 = 0;
    int16_t cgx = 0;  // _DI_

    // TODO SEGRAX  struct s_BATTLE_UNIT* battleunit1 = &battle_units[battle_unit_idx];

    caster_attribs_1 = battle_units[caster_idx].Attribs_1 & (USA_SUMMON_DEMON_1 | USA_SUMMON_DEMON_2);

    if(caster_attribs_1 == USA_SUMMON_DEMON_1)
    {
        battle_units[caster_idx].Attribs_1 ^= USA_SUMMON_DEMON_1;
    }
    else if(caster_attribs_1 == USA_SUMMON_DEMON_2)
    {
        battle_units[caster_idx].Attribs_1 ^= USA_SUMMON_DEMON_2;
        battle_units[caster_idx].Attribs_1 |= USA_SUMMON_DEMON_1;
    }
    else
    {
        battle_units[caster_idx].Attribs_1 ^= USA_SUMMON_DEMON_1;
    }

    Create_Unit__WIP(ut_Demon, battle_units[caster_idx].controller_idx, 0, 0, 9, 2000);

    do {
        if(battle_units[caster_idx].controller_idx == _combat_attacker_player)
        {
            cgx = (14 - Random(3));
        }
        else
        {
            cgx = (7 + Random(3));
        }
        cgy = (8 + Random(3));
    } while(CMB_TargetRows[cgy][cgx] >= 0);

    UNIT_SummonToBattle__SEGRAX(battle_units[caster_idx].controller_idx, (_units - 1), cgx, cgy);

    BU_CombatSummon__SEGRAX((_combat_total_unit_count - 1), cgx, cgy, 0, caster_idx);

}


// WZD o113p11
// drake178: BU_MeleeWallCheck()
/*
; returns 1 if the specified unit can attack the target
; based on the condition of City Walls, if any, or 0 if
; it can't
;
; BUG? teleporting and merging units can attack through
;  walls
; BUG: Flying Fortress should be considered here, but
;  it isn't
*/
/*

*/
int16_t BU_MeleeWallCheck__WIP(int16_t src_battle_unit_idx, int16_t dst_battle_unit_idx)
{

    /* HACK */  return ST_TRUE;

}


// WZD o113p12
// drake178: CMB_ManaLeak()

// WZD o113p13
// drake178: GUI_DrawNearMessage()

// WZD o113p14
// drake178: CMB_SpellcastMessage()
/*
; draws the successful combat spellcast message into
; the current draw frame on a black shaded rectangle
; background
*/
/*

*/
void CMB_SpellcastMessage__WIP(int16_t caster_idx, int16_t spell_idx)
{
    char spell_name[LEN_SPELL_NAME] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t string_width = 0;
    uint8_t colors[2] = { 0, 0 };
    int16_t y2 = 0;
    int16_t x2 = 0;
    int16_t y1 = 0;
    int16_t x1 = 0;  // _DI_

    if(caster_idx >= CASTER_IDX_BASE)
    {
        strcpy(GUI_NearMsgString, _players[(caster_idx - 20)].name);
        strcat(GUI_NearMsgString, cnst_CombatCast_1);  // " has cast "
    }
    else
    {
        if(_UNITS[battle_units[caster_idx].unit_idx].Hero_Slot > ST_UNDEFINED)
        {
            // ; BUG: this may not be the hero's original owner
            strcpy(GUI_NearMsgString, _players[battle_units[caster_idx].controller_idx].Heroes[_UNITS[battle_units[caster_idx].unit_idx].Hero_Slot].name);
            strcat(GUI_NearMsgString, cnst_CombatCast_1);  // " has cast "
        }
        else
        {
            strcpy(GUI_NearMsgString, cnst_CombatCast_2);  // "The "
            strcat(GUI_NearMsgString, _unit_type_table[_UNITS[battle_units[caster_idx].unit_idx].type].name);
            if(battle_units[caster_idx].Max_Figures > 1)
            {
                strcat(GUI_NearMsgString, cnst_CombatCast_3);  // " have cast "
            }
            else
            {
                strcat(GUI_NearMsgString, cnst_CombatCast_1);  // " has cast "
            }
        }
    }

    if(spell_idx != spl_NONE)
    {
        String_Copy_Far(spell_name, spell_data_table[spell_idx].name);
        strcat(GUI_NearMsgString, spell_name);
    }
    else
    {
        strcat(GUI_NearMsgString, cnst_CombatCast_4);  // "summon demon"
    }

    colors[0] = 182;
    colors[1] = 177;

    Set_Font_Colors_15(0, &colors[0]);

    Set_Font_Style_Shadow_Down(1, 15, 0, 0);

    Set_Alias_Color(182);

    Set_Outline_Color(2);

    string_width = Get_String_Width(GUI_NearMsgString);

    x1 = (160 - (string_width / 2) - 5);

    x2 = (165 + (string_width / 2));

    y1 = 5;

    y2 = 15;

    Gradient_Fill(x1, y1, x2, y2, 15, 8, ST_NULL, ST_NULL, ST_NULL);

    Line(x1, y1, x1, y2, 230);

    Line(x1, y1, (x2 - 1), y1, 230);

    Line(x2, y1, x2, y2, 237);

    Line((x1 + 1), y2, x2, y2, 237);

    Print_Centered(160, 8, GUI_NearMsgString);

}


// segrax
// AKA  CMB_FindEmptyFigSlot__SEGRAX()
// WZD o113p15
// drake178: CMB_FindEmptyFigSlot()
// AKA  Battle_Unit_Slot_Open()
/*
; loops through the battle unit table to find the first
; figure index that does not belong to a unit that is
; currently active on the field
; returns the figure slot index or garbage
*/
/*
    returns bufpi {0,...17}
    an unused entry in the battle unit pictures array

Cast_Raise_Dead()
    battle_units[battle_unit_idx].bufpi = Combat_Figure_Load(_UNITS[battle_units[battle_unit_idx].unit_idx].type, Battle_Unit_Pict_Open());
*/
int16_t Battle_Unit_Pict_Open(void)
{
    int16_t battle_unit_slots[MAX_BATTLE_UNIT_COUNT] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t battle_unit_slot_idx = 0;  // _CX_

    for(battle_unit_slot_idx = 0; battle_unit_slot_idx < MAX_BATTLE_UNIT_COUNT; battle_unit_slot_idx++)
    {
        battle_unit_slots[battle_unit_slot_idx] = ST_FALSE;
    }

    for (battle_unit_slot_idx = 0; battle_unit_slot_idx < _combat_total_unit_count; battle_unit_slot_idx++)
    {
        struct s_BATTLE_UNIT * battle_unit = &battle_units[battle_unit_slot_idx];

        if(
            (battle_unit->bufpi > ST_UNDEFINED)
            && 
            (battle_unit->status == bus_Active)
        )
        {
            battle_unit_slots[battle_unit->bufpi] = ST_TRUE;
        }
    }

    for (battle_unit_slot_idx = 0; battle_unit_slot_idx < MAX_BATTLE_UNIT_COUNT; battle_unit_slot_idx++)
    {
        if (battle_unit_slots[battle_unit_slot_idx] == ST_FALSE)
        {
            return battle_unit_slot_idx;
        }
    }

    // BUGBUG  function is missing return value if no empty slot is found  SEE: Hero_Slot_Open()
    return -1;  // DNE in Dasm
}


// WZD o113p16
// MoO2  Module: COMBAT1  Check_For_Winner_()
/*
    returns player_idx or ST_UNDEFINED
*/
int16_t Check_For_Winner(void)
{
    int16_t attacker_count;  // _SI_
    int16_t defender_count;  // _DI_
    int16_t itr_battle_units;  // _CX_
    int16_t winner_player_idx;  // DNE in Dasm

    attacker_count = 0;
    defender_count = 0;

    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
    {
        if(battle_units[itr_battle_units].status == 0)  /* Unit_Active */
        {
            if(battle_units[itr_battle_units].controller_idx == _combat_attacker_player)
            {
                if(battle_units[itr_battle_units].Confusion_State != 2)  /* ¿ ? */
                {
                    attacker_count++;
                }
                else
                {
                    defender_count++;
                }
            }
            else
            {
                if(battle_units[itr_battle_units].controller_idx == _combat_defender_player)
                {
                    if(battle_units[itr_battle_units].Confusion_State != 2)  /* ¿ ? */
                    {
                        defender_count++;
                    }
                    else
                    {
                        attacker_count++;
                    }
                }
            }
        }
    }

    if(attacker_count == 0)
    {
        winner_player_idx = _combat_defender_player;
    }
    else if(defender_count == 0)
    {
        winner_player_idx = _combat_attacker_player;
    }
    else
    {
        winner_player_idx = ST_UNDEFINED;
    }

    return winner_player_idx;
}




/*
    WIZARDS.EXE  ovr116
*/

// WZD o114p01
// drake178: AI_SetBasicAttacks()
/*
sets the tactical action for all of the player's
units to either melee, ranged, or 66h/67h depensing
on comparisons of the two armies facing each other
*/
/*

*/
void AI_SetBasicAttacks__WIP(int16_t player_idx)
{
    int16_t Rnd_3_Minus_4 = 0;
    int16_t Enemy_Threat_Total = 0;
    int16_t Own_Threat_Total = 0;
    int16_t Own_Melee_Threat = 0;
    int16_t Ranged_Difference = 0;
    int16_t Safety_Level = 0;
    int16_t Enemy_Mana_Div_4 = 0;
    int16_t IDK_ranged_strength = 0;  // _DI_
    int16_t battle_unit_idx = 0;  // _SI_


    IDK_ranged_strength = 0;


    Enemy_Mana_Div_4 = 0;
    Own_Threat_Total = 0;
    Enemy_Threat_Total = 0;


    for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
    {

        if(battle_units[battle_unit_idx].status == bus_Active)
        {

            if(battle_units[battle_unit_idx].controller_idx == player_idx)
            {

                if(Battle_Unit_Has_Ranged_Attack(battle_unit_idx) != ST_FALSE)
                {

                    battle_units[battle_unit_idx].action = BUA_RangedAttack;

                }
                else
                {

                    battle_units[battle_unit_idx].action = BUA_MeleeAttack;

                }

                Own_Threat_Total += Effective_Battle_Unit_Strength(battle_unit_idx);

                IDK_ranged_strength += (battle_units[battle_unit_idx].mana / 4);

            }
            else
            {

                Enemy_Threat_Total += Effective_Battle_Unit_Strength(battle_unit_idx);

                Enemy_Mana_Div_4 += (battle_units[battle_unit_idx].mana / 4);

            }
        
        }

    }


    IDK_ranged_strength += WIZ_GetLastRangedStr__WIP(player_idx);

    if(player_idx == _combat_attacker_player)
    {

        Enemy_Mana_Div_4 += WIZ_GetLastRangedStr__WIP(_combat_defender_player);

    }
    else
    {

        Enemy_Mana_Div_4 += WIZ_GetLastRangedStr__WIP(_combat_attacker_player);

    }


    if(
        (Own_Threat_Total == 0)
        ||
        (Enemy_Threat_Total == 0)
    )
    {
        return;
    }


    Safety_Level = AI_CompareArmies__STUB(player_idx);

    Ranged_Difference = (IDK_ranged_strength - Enemy_Mana_Div_4);

    /*
        sum up the threat of own units set to melee action
    */
    Own_Melee_Threat = 0;

    for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
    {

        if(
            (battle_units[battle_unit_idx].status == bus_Active)
            &&
            (battle_units[battle_unit_idx].controller_idx == player_idx)
            &&
            (battle_units[battle_unit_idx].action == BUA_MeleeAttack)
        )
        {

            Own_Melee_Threat += Effective_Battle_Unit_Strength(battle_unit_idx);
        
        }

    }


    Rnd_3_Minus_4 = (Random(3) - 4);

    for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
    {

        if(
            (battle_units[battle_unit_idx].status == bus_Active)
            &&
            (battle_units[battle_unit_idx].controller_idx == player_idx)
        )
        {

            switch(Safety_Level)
            {

                case 0:
                {

                } break;

                case 1:
                {

                } break;

                case 2:
                {

                } break;

                case 3:
                {

                } break;

                default:
                {

                    if(battle_units[battle_unit_idx].action == BUA_RangedAttack)
                    {

                        if(Random(2) == 1)
                        {

                            battle_units[battle_unit_idx].action = BUA_MoveNFire;

                        }

                    }

                } break;

            }

        }

    }

}


// WZD o114p02
// drake178: AI_BU_ProcessAction()
/*
; selects the chosen battle unit, and processes its
; assigned action
;
; BUG: can reassign into actions it then can't perform
; and loads of other BUGs...
;
; RE-EXPLORE!
*/
/*

Rally_X,Rally_Y

*/
void AI_BU_ProcessAction__WIP(int16_t battle_unit_idx, int16_t Rally_X, int16_t Rally_Y)
{
    int16_t No_Override = 0;
    int16_t Spell_Result = 0;
    int16_t some_variable = 0;

    if(
        (Rally_X == 0)
        &&
        (Rally_Y == 0)
    )
    {

        Rally_X = (battle_units[battle_units[battle_unit_idx].Target_BU].cgx);

        Rally_Y = (battle_units[battle_units[battle_unit_idx].Target_BU].cgy);

    }


    Switch_Active_Battle_Unit(battle_unit_idx);

    _active_battle_unit = battle_unit_idx;


    // ; BUG: Healing Spell action branch missing
/*
BUA_MeleeAttack     = 100,
BUA_RangedAttack    = 101,
BUA_MoveNFire       = 102,
BUA_MoveNAttack     = 103,
BUA_DoomBolt        = 104,
BUA_Fireball        = 105,
bua_Healing         = 106,
BUA_UseItem         = 107,
BUA_CastSpell       = 108,
BUA_SummonDemon     = 109,
bua_WebSpell        = 110,
BUA_Flee            = 150,

jt_bua_00
jt_bua_01
jt_bua_02
jt_bua_03
jt_bua_04
jt_bua_05
jt_bua_06
jt_bua_07_08
jt_bua_07_08
jt_bua_09
jt_bua_10
*/
    switch(battle_units[battle_unit_idx].action)
    {

        case BUA_MeleeAttack:
        {
            
            if(battle_units[battle_unit_idx].melee != 0)
            {

                if(
                    ((_battlefield_city_walls & BATTLEFIELD_CITY_WALL_STONE) != 0)
                    ||
                    (battle_units[battle_unit_idx].Target_BU != ST_UNDEFINED)
                )
                {

                    No_Override = ST_TRUE;

                }
                else
                {

                    /*
                        would try to find the defending unit at the gate, if
                        any - but will never be executed because of the
                        conflicting condition of the target having to be -1
                        and not being -1 at the same time
                    */
                    No_Override = ST_FALSE;

                    if(
                        (battle_units[battle_unit_idx].controller_idx != _combat_attacker_player)
                        ||
                        (battle_units[battle_unit_idx].Target_BU == ST_UNDEFINED)
                    )
                    {

                        G_AI_BU_MoveOrRampage__WIP(battle_unit_idx, 8, 12, -666, 8, 12);

                    }
                    else
                    {

                        for(some_variable = 0; some_variable < _combat_total_unit_count; some_variable++)
                        {

                            if(
                                (battle_units[some_variable].cgx == 8)
                                &&
                                (battle_units[some_variable].cgy == 12)
                                &&
                                (battle_units[battle_unit_idx].controller_idx != battle_units[some_variable].controller_idx)
                                &&
                                (battle_units[some_variable].status == bus_Active)
                            )
                            {

                                battle_units[battle_unit_idx].Target_BU = some_variable;

                                No_Override = ST_TRUE;

                            }

                        }

                    }

                }

                if(No_Override == ST_FALSE)
                {

                    G_AI_BU_MoveOrRampage__WIP(battle_unit_idx, 8, 12, -666, 8, 12);

                }


                if(No_Override == ST_TRUE)
                {

                    some_variable = 220;

                    if(
                        (battle_units[battle_unit_idx].Target_BU == ST_UNDEFINED)
                        &&
                        (battle_units[battle_unit_idx].controller_idx == NEUTRAL_PLAYER_IDX)
                        &&
                        (battle_units[battle_unit_idx].controller_idx == _combat_attacker_player)
                        &&
                        (OVL_Action_Type == 1)  /* Enemy City */
                    )
                    {

                        G_AI_BU_MoveOrRampage__WIP(battle_unit_idx, 0, 0, ST_UNDEFINED, 0, 0);

                    }
                    else
                    {

                        if(
                            (battle_units[battle_unit_idx].movement_points < some_variable)
                            &&
                            (battle_units[battle_unit_idx].Target_BU > ST_UNDEFINED)
                        )
                        {

                            if(
                                (battle_units[battle_unit_idx].movement_points > 0)
                                &&
                                (battle_units[battle_unit_idx].status == bus_Active)
                            )
                            {

                                some_variable = battle_units[battle_unit_idx].movement_points;

                                /*
                                ; BUG: will read outside of the array bounds with this
                                ; jump
                                */
                                if(battle_units[battle_unit_idx].Target_BU >= 0)
                                {

                                    G_AI_BU_MoveOrRampage__WIP(battle_unit_idx, battle_units[battle_units[battle_unit_idx].Target_BU].cgx, battle_units[battle_units[battle_unit_idx].Target_BU].cgy, battle_units[battle_unit_idx].Target_BU, Rally_X, Rally_Y);

                                }

                                if(battle_units[battle_units[battle_unit_idx].Target_BU].status != bus_Active)
                                {

                                    AI_BU_AssignAction__WIP(battle_unit_idx, ST_FALSE);

                                }

                            }

                        }

                        battle_units[battle_unit_idx].action = bua_Finished;

                    }

                }

            }

        } break;

        case BUA_RangedAttack:
        {
            STU_DEBUG_BREAK();

        } break;

        /*
            Action 102 - MoveNFire

            take a step towards the target, then attack until out
            of moves, switching targets if necessary
        */
        case BUA_MoveNFire:
        {
            STU_DEBUG_BREAK();

            if(battle_units[battle_unit_idx].Target_BU < 0)
            {
                break;
            }

            if(Range_To_Battle_Unit(battle_unit_idx, battle_units[battle_unit_idx].Target_BU) != 1)
            {

                some_variable = battle_units[battle_unit_idx].movement_points;

                battle_units[battle_unit_idx].movement_points = 1;

                G_AI_BU_MoveOrRampage__WIP(battle_unit_idx, battle_units[battle_units[battle_unit_idx].Target_BU].cgx, battle_units[battle_units[battle_unit_idx].Target_BU].cgy, battle_units[battle_unit_idx].Target_BU, Rally_X, Rally_Y);

            }

            while(
                (battle_units[battle_unit_idx].movement_points > 0)
                &&
                (battle_units[battle_unit_idx].Target_BU > -1)
            )
            {

                Battle_Unit_Attack__WIP(battle_unit_idx, battle_units[battle_unit_idx].Target_BU, 0, 0);

                if(battle_units[battle_units[battle_unit_idx].Target_BU].status == bus_Active)
                {
                    continue;
                }

                AI_BU_AssignAction__WIP(battle_unit_idx, 0);

            }

            battle_units[battle_unit_idx].action = bua_Finished;

        } break;

        case BUA_MoveNAttack:
        {
            STU_DEBUG_BREAK();

        } break;

        case BUA_DoomBolt:
        {
            STU_DEBUG_BREAK();

        } break;

        case BUA_Fireball:
        {
            STU_DEBUG_BREAK();

        } break;

        case bua_Healing:
        {
            STU_DEBUG_BREAK();

        } break;

        case BUA_UseItem:
        {
            STU_DEBUG_BREAK();

        } break;

        case BUA_CastSpell:
        {
            STU_DEBUG_BREAK();

        } break;

        case BUA_SummonDemon:
        {
            BU_SummonDemon__SEGRAX(battle_unit_idx);
        } break;

        case bua_WebSpell:
        {
            STU_DEBUG_BREAK();

        } break;

        case BUA_Flee:
        {
            STU_DEBUG_BREAK();

        } break;

    }


    SETMIN(battle_units[battle_unit_idx].movement_points, 0);

}


// WZD o114p03
// drake178: BU_SortSlowestFirst()
/*
; sorts the battle units in the passed list based on
; their remaining movement allowance, from least to
; most - slowest to fastest if none have moved yet
*/
/*

Melee_Unit_List[] is an array of battle_unit_idx

*/
void BU_SortSlowestFirst__WIP(int16_t Melee_Unit_List[], int16_t Melee_Unit_Count)
{
    int16_t current_battle_unit_moves2 = 0;
    int16_t previous_battle_unit_moves2 = 0;
    int16_t itr = 0;
    int16_t current_battle_unit_idx = 0;
    int16_t previous_battle_unit_idx = 0;  // _CL_


    for(itr = 1; itr < Melee_Unit_Count; itr++)
    {

        current_battle_unit_idx = Melee_Unit_List[itr];

        previous_battle_unit_idx = (itr - 1);

        previous_battle_unit_moves2 = battle_units[previous_battle_unit_idx].movement_points;

        current_battle_unit_moves2 = battle_units[current_battle_unit_idx].movement_points;

        while(
            (previous_battle_unit_idx > -1)
            &&
            (previous_battle_unit_moves2 > current_battle_unit_moves2)
        )
        {

            Melee_Unit_List[(previous_battle_unit_idx + 1)] = Melee_Unit_List[previous_battle_unit_idx];

            previous_battle_unit_idx--;

            if(previous_battle_unit_idx > -1)
            {

                previous_battle_unit_moves2 = battle_units[Melee_Unit_List[previous_battle_unit_idx]].movement_points;

            }

        }

        Melee_Unit_List[previous_battle_unit_idx] = current_battle_unit_idx;

        // NEWBUG  itr++;

    }

}


// WZD o114p04
// drake178: AI_GetCombatRallyPt()
/*
; sets into the return pointers the tile coordinates
; that the target unit will be after finishing its
; movement, unless it has ranged or 67h selected as its
; tactic, in which case it sets its current location
*/
/*

*/
void AI_GetCombatRallyPt__WIP(int16_t battle_unit_idx, int16_t * Rally_X, int16_t * Rally_Y)
{
    int16_t itr = 0;  // _SI_

    if(
        (battle_units[battle_unit_idx].action == BUA_MoveNAttack)
        ||
        (battle_units[battle_unit_idx].action == BUA_RangedAttack)
    )
    {

        *Rally_X = battle_units[battle_unit_idx].cgx;

        *Rally_Y = battle_units[battle_unit_idx].cgy;

        return;

    }


    Set_Movement_Cost_Map(battle_unit_idx);

    /*
        mark the target BU's tile as requiring 2 half moves
        to enter, and all other units' tiles as impassable
    */
    for(itr = 0; itr < _combat_total_unit_count; itr++)
    {

        if(battle_units[battle_unit_idx].Target_BU == itr)
        {

            CMB_ActiveMoveMap[((battle_units[itr].cgy * COMBAT_GRID_WIDTH) + battle_units[itr].cgx)] = 2;

        }
        else
        {

            if(battle_units[itr].status == bus_Active)
            {

                CMB_ActiveMoveMap[((battle_units[itr].cgy * COMBAT_GRID_WIDTH) + battle_units[itr].cgx)] = INF;

            }

        }

    }


    /*
        mark all magic vortices as impassable
    */
    for(itr = 0; itr < CMB_Vortex_Count; itr++)
    {

        CMB_ActiveMoveMap[((CMB_Vortex_Array[itr].cgy * COMBAT_GRID_WIDTH) + CMB_Vortex_Array[itr].cgx)] = INF;

    }


    /* BEGIN:  HACK */
    if(battle_units[battle_unit_idx].Target_BU == ST_UNDEFINED)
    {
        *Rally_X = 0;
        *Rally_Y = 0;
        return;
    }
    /* END:  HACK */


    // battle_units[battle_units[battle_unit_idx].Target_BU].cgx
    // battle_units[battle_units[battle_unit_idx].Target_BU].cgy
    Combat_Move_Path_Find(battle_units[battle_unit_idx].cgx, battle_units[battle_unit_idx].cgy, battle_units[battle_units[battle_unit_idx].Target_BU].cgx, battle_units[battle_units[battle_unit_idx].Target_BU].cgy);

    for(itr = 0; itr < movement_path_grid_cell_count; itr++)
    {

        if(itr != 0)
        {

            if((battle_units[battle_unit_idx].movement_points - CMB_Path_Costs[((CMB_Path_Ys[(itr - 1)] * COMBAT_GRID_WIDTH) + CMB_Path_Xs[(itr - 1)])]) <= 0)
            {

                break;

            }

        }

    }

    *Rally_X = CMB_Path_Xs[(itr - 1)];

    *Rally_Y = CMB_Path_Ys[(itr - 1)];

}


// WZD o114p05
// drake178: AI_MoveBattleUnits()
/*
; assigns and processes the turn's actions for all
; battle units controlled by the selected player
;
; needs serious RE-EXPLORING!
*/
/*

used for computer-player and 'Auto Combat' for human-player

*/
void AI_MoveBattleUnits__WIP(int16_t player_idx)
{
    int16_t Melee_Unit_List[36] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Attacker_Vortices = 0;
    int16_t Their_Last_Ranged_Str = 0;
    int16_t Our_Last_Ranged_Str = 0;
    int16_t Rally_Y = 0;
    int16_t Rally_X = 0;
    int16_t Melee_Unit_Count = 0;
    int16_t battle_unit_idx = 0;  // _SI_
    int16_t itr = 0;  // _SI_


    if(
        (player_idx == NEUTRAL_PLAYER_IDX)
        &&
        (player_idx == _combat_attacker_player)
        &&
        (AI_ImmobileCounter > 3)
    )
    {

        for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
        {

            if(
                (battle_units[battle_unit_idx].status == bus_Active)
                &&
                (battle_units[battle_unit_idx].controller_idx == NEUTRAL_PLAYER_IDX)
            )
            {

                STU_DEBUG_BREAK();  /* WTF, Mate? ... How did I get here? ... Do I ever get here? */
                battle_units[battle_unit_idx].status = bus_Dead;

            }

        }

    }


    Our_Last_Ranged_Str = WIZ_GetLastRangedStr__WIP(player_idx);


    if(player_idx == _combat_attacker_player)
    {

        Their_Last_Ranged_Str = WIZ_GetLastRangedStr__WIP(_combat_defender_player);

    }
    else
    {

        Their_Last_Ranged_Str = WIZ_GetLastRangedStr__WIP(_combat_attacker_player);

    }


    _battlefield_city_walls = 0;

    if(battlefield->Walled_City == ST_TRUE)
    {

        _battlefield_city_walls |= BATTLEFIELD_CITY_WALL_STONE;

    }

    if(battlefield->Wall_of_Darkness == ST_TRUE)
    {

        _battlefield_city_walls |= BATTLEFIELD_CITY_WALL_DARKNESS;

    }

    if(battlefield->Wall_of_Fire == ST_TRUE)
    {

        _battlefield_city_walls |= BATTLEFIELD_CITY_WALL_FIRE;

    }


    if(player_idx == _combat_defender_player)
    {
        
        if(G_AI_StayInTownProper != ST_TRUE)
        {
            
            _battlefield_city_walls = 0;

        }
        else
        {

            Attacker_Vortices = 0;

            for(itr = 0; itr < CMB_Vortex_Count; itr++)
            {

                if (CMB_Vortex_Array[itr].owner_idx == _combat_attacker_player)
                {

                    Attacker_Vortices++;

                }

            }

            if(
                (Their_Last_Ranged_Str > 30)
                &&
                (Our_Last_Ranged_Str == 0)
            )
            {
                _battlefield_city_walls = 0;

                G_AI_StayInTownProper = ST_FALSE;

            }

            if(
                (Attacker_Vortices > 0)
                ||
                (combat_enchantments[WRACK_ATTKR] > 0)
                ||
                (combat_enchantments[CALL_LIGHTNING_ATTKR] > 0)
                ||
                (
                    (combat_enchantments[MANA_LEAK_ATTKR] > 0)
                    &&
                    (player_idx < _num_players)
                )
            )
            {

                _battlefield_city_walls = 0;

                G_AI_StayInTownProper = ST_FALSE;

            }

        }

    }


    CMB_Winner = ST_UNDEFINED;


    AI_SetBasicAttacks__WIP(player_idx);


    if(Our_Last_Ranged_Str < Their_Last_Ranged_Str)
    {

        Rally_X = 0;

        Rally_Y = 0;

    }
    else
    {

        /*
            create a list of the units with melee attack or 67h
            set as their selected action
        */
        Melee_Unit_Count = 0;

        for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
        {

            if(battle_units[battle_unit_idx].controller_idx == player_idx)
            {

                if(battle_units[battle_unit_idx].status == bus_Active)
                {

                    if(
                        (battle_units[battle_unit_idx].action == BUA_MeleeAttack)
                        ||
                        (battle_units[battle_unit_idx].action == BUA_MoveNAttack)
                    )
                    {

                        Melee_Unit_List[Melee_Unit_Count] = battle_unit_idx;

                        Melee_Unit_Count++;

                    }

                }

            }

        }


        BU_SortSlowestFirst__WIP(&Melee_Unit_List[0], Melee_Unit_Count);

        AI_GetCombatRallyPt__WIP(Melee_Unit_List[(Melee_Unit_Count - 1)], &Rally_X, &Rally_Y);

    }


    AI_NoMeleeHeroSafety = ST_FALSE;

    for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
    {


        /* HACK */  if(DBG_Ignore_Computer_Player == ST_TRUE)
        /* HACK */  {
        /* HACK */  
        /* HACK */      if(battle_units[battle_unit_idx].controller_idx != HUMAN_PLAYER_IDX)
        /* HACK */      {
        /* HACK */  
        /* HACK */          continue;
        /* HACK */  
        /* HACK */      }
        /* HACK */  
        /* HACK */  }


        if(
            (battle_units[battle_unit_idx].controller_idx == player_idx)
            &&
            (battle_units[battle_unit_idx].status == bus_Active)
        )
        {

            if(battle_units[battle_unit_idx].movement_points > 0)
            {

                if((battle_units[battle_unit_idx].Combat_Effects & bue_Black_Sleep) == 0)
                {

                    Switch_Active_Battle_Unit(battle_unit_idx);

                    Assign_Combat_Grids();

                    AI_BU_AssignAction__WIP(battle_unit_idx, bua_Ready);

                    if(
                        (battle_units[battle_unit_idx].controller_idx == _combat_defender_player)
                        &&
                        (_battlefield_city_walls > 0)
                        &&
                        (BU_IsInCityProper__STUB(battle_unit_idx) == ST_FALSE)
                    )
                    {

                        AI_BU_ProcessAction__WIP(battle_unit_idx, 0, 0);

                    }
                    else
                    {

                        AI_BU_ProcessAction__WIP(battle_unit_idx, Rally_X, Rally_Y);

                    }

                }

            }

        }


        /*
        ; BUG: this is not set anywhere - BU_ApplyDamage__WIP does
        ; not set it at the right time either
        */
        if(CMB_Winner != ST_UNDEFINED)
        {
            break;
        }

    }


    if(AI_ImmobileCounter != ST_UNDEFINED)
    {

        AI_NoMeleeHeroSafety = ST_TRUE;

    }


    for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
    {

        if(
            (battle_units[battle_unit_idx].controller_idx == player_idx)
            &&
            (battle_units[battle_unit_idx].status == bus_Active)
        )
        {

            if(battle_units[battle_unit_idx].movement_points > 0)
            {

                if(_UNITS[battle_units[battle_unit_idx].unit_idx].Hero_Slot > -1)
                {

                    if((battle_units[battle_unit_idx].Combat_Effects & bue_Black_Sleep) == 0)
                    {

                        Switch_Active_Battle_Unit(battle_unit_idx);

                        Assign_Combat_Grids();

                        AI_BU_AssignAction__WIP(battle_unit_idx, bua_Ready);

                        AI_BU_ProcessAction__WIP(battle_unit_idx, Rally_X, Rally_Y);

                    }

                }

            }

        }


        if(CMB_Winner != ST_UNDEFINED)
        {
            break;
        }

    }

}


// WZD o114p06
// drake178: AI_BU_AssignAction()
int16_t AI_BU_AssignAction__WIP(int16_t battle_unit_idx, int16_t no_spells_flag)
{
    int16_t player_idx = 0;
    int16_t has_ranged_attack = 0;
    int16_t selected_action = 0;
    int16_t target_battle_unit_idx = 0;
    int16_t itr_battle_units = 0;  // _DI_

    if(battle_units[battle_unit_idx].status != bus_Active)
    {
        return ST_UNDEFINED;
    }

    if(no_spells_flag == ST_TRUE)
    {
        selected_action = BUA_No_Spells;
    }
    else
    {
        selected_action = bua_Ready;
    }

    player_idx = battle_units[battle_unit_idx].controller_idx;

    target_battle_unit_idx = ST_UNDEFINED;

    has_ranged_attack = Battle_Unit_Has_Ranged_Attack(battle_unit_idx);

    target_battle_unit_idx = AI_BU_SelectAction__WIP(battle_unit_idx, &selected_action, has_ranged_attack);

    if(
        (target_battle_unit_idx > ST_UNDEFINED)
        &&
        (has_ranged_attack == ST_TRUE)
        &&
        ((battle_units[battle_unit_idx].ranged_type / 10) == rag_Missile)
        &&
        ((battle_units[battle_unit_idx].melee * 3) < (battle_units[battle_unit_idx].ranged * 2))
    )
    {

        target_battle_unit_idx = ST_UNDEFINED;

    }

    if(
        (has_ranged_attack == ST_TRUE)
        &&
        (target_battle_unit_idx == ST_UNDEFINED)
        &&
        (
            (selected_action == bua_Ready)
            ||
            (selected_action == BUA_RangedAttack)
            ||
            (selected_action == BUA_MoveNFire)
        )
    )
    {

        has_ranged_attack = ST_FALSE;

        target_battle_unit_idx = AI_BU_SelectAction__WIP(battle_unit_idx, &selected_action, has_ranged_attack);

        if(
            (selected_action == BUA_RangedAttack)  /* ; conflicting condition - will never jump */
            ||
            (selected_action == BUA_MoveNFire)  /* ; conflicting condition - will always jump */
        )
        {
            selected_action = BUA_MeleeAttack;
        }

    }


    if(
        (selected_action == bua_Ready)
        ||
        (selected_action == BUA_No_Spells)
    )
    {

        if(has_ranged_attack == ST_TRUE)
        {

            battle_units[battle_unit_idx].action = BUA_RangedAttack;

        }
        else
        {

            battle_units[battle_unit_idx].action = BUA_MeleeAttack;

        }

    }
    else
    {

        battle_units[battle_unit_idx].action = selected_action;

    }



    if(
        (battle_units[battle_unit_idx].action == BUA_RangedAttack)
        ||
        (battle_units[battle_unit_idx].action == BUA_MeleeAttack)
    )
    {

        if(
            (battle_units[battle_unit_idx].controller_idx == _combat_defender_player)
            ||
            (_battlefield_city_walls == 0)
        )
        {

            if(
                (target_battle_unit_idx > ST_UNDEFINED)
                &&
                (target_battle_unit_idx  < 36)
            )
            {

                for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
                {

                    if(battle_units[itr_battle_units].status != bus_Active)
                    {
                        continue;
                    }

                    if(battle_units[itr_battle_units].controller_idx == battle_units[battle_unit_idx].controller_idx)
                    {
                        continue;
                    }

                    if(_UNITS[battle_units[target_battle_unit_idx].unit_idx].type != _UNITS[battle_units[itr_battle_units].unit_idx].type)
                    {
                        continue;
                    }

                    if(battle_units[target_battle_unit_idx].defense != battle_units[itr_battle_units].defense)
                    {
                        continue;
                    }

                    if(Range_To_Battle_Unit(battle_unit_idx, itr_battle_units) >= Range_To_Battle_Unit(battle_unit_idx, target_battle_unit_idx))
                    {
                        continue;
                    }

                    target_battle_unit_idx = itr_battle_units;

                }


            }


        }


    }


    /*
    if there is a lower index own unit with a target of
    the same type and same base Defense, target that
    instead

    BUG: ignores all other attributes of this new target
    besides base Defense (e.g. flight, immunities)
    */
    {

        for(itr_battle_units = 0; itr_battle_units < battle_unit_idx; itr_battle_units++)
        {

            if(battle_units[itr_battle_units].status != bus_Active)
            {
                continue;
            }

            if(battle_units[itr_battle_units].controller_idx != battle_units[battle_unit_idx].controller_idx)
            {
                continue;
            }

            if(_UNITS[battle_units[battle_units[itr_battle_units].Target_BU].unit_idx].type != _UNITS[battle_units[target_battle_unit_idx].unit_idx].type)
            {
                continue;
            }

            if(battle_units[battle_units[itr_battle_units].Target_BU].status != bus_Active)
            {
                continue;
            }

            if(battle_units[battle_units[itr_battle_units].Target_BU].defense != battle_units[target_battle_unit_idx].defense)
            {
                continue;
            }

            target_battle_unit_idx = battle_units[itr_battle_units].Target_BU;

        }

    }

    battle_units[battle_unit_idx].Target_BU = target_battle_unit_idx;

    return target_battle_unit_idx;

}


// WZD o114p07
// drake178: AI_BU_SelectAction()
/*
; selects the next action for the specified battle unit
; based on the circumstances
; either returns a target index, setting the value of
; the passed pointer to the chosen action (0 for a
; regular attack of the type indicated by the Ranged
; param); or -1 if no enemies can be attacked
;
; still need to check Spl_Healing and action 333
*/
/*

¿?

*/
int16_t AI_BU_SelectAction__WIP(int16_t battle_unit_idx, int16_t * selected_action, int16_t has_ranged_attack)
{
    int16_t Target_Y = 0;
    int16_t Target_X = 0;
    int16_t spell_idx = 0;
    int16_t Ability_Chosen = 0;
    // int16_t player_unit_count__attack_value__item_spell_idx = 0;
    int16_t player_unit_count = 0;
    int16_t attack_value = 0;
    int16_t item_spell_idx = 0;
    int16_t target_battle_unit_idx = 0;
    int16_t Highest_Value = 0;
    int16_t Current_Target_Health = 0;
    // int16_t hero_slot_idx__Total_Health = 0;
    int16_t hero_slot_idx = 0;
    int16_t Total_Health = 0;
    int16_t itr_battle_units = 0;  // _SI_


    Highest_Value = -100;

    target_battle_unit_idx = ST_UNDEFINED;

    Ability_Chosen = ST_FALSE;


    if(CMB_Winner != ST_UNDEFINED)
    {
        return ST_UNDEFINED;
    }


    if(
        ((battle_units[battle_unit_idx].Attribs_1 & (0x0800 /* Sum_Demon_1 */ | 0x1000 /* Sum_Demon_2 */ )) != 0)
        &&
        (_combat_total_unit_count < 36)
        &&
        (_units < 1000)
    )
    {

        player_unit_count = 0;

        for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
        {

            if(
                (battle_units[itr_battle_units].controller_idx == battle_units[battle_unit_idx].controller_idx)
                &&
                (battle_units[itr_battle_units].status == bus_Active)
            )
            {
                player_unit_count++;
            }

        }

        if(player_unit_count < 9)
        {
            *selected_action = BUA_SummonDemon;
        }

        Ability_Chosen = ST_TRUE;

        target_battle_unit_idx = 99;

    }


    if(
        ((battle_units[battle_unit_idx].Attribs_2 & 0x4 /* Spl_DoomBolt */ ) != 0)
        &&
        (Ability_Chosen == ST_FALSE)
    )
    {

        spell_idx = spl_Doom_Bolt;

        target_battle_unit_idx = AITP_CombatSpell__STUB(spell_idx, battle_units[battle_unit_idx].controller_idx, &Target_X, &Target_Y);

        if(target_battle_unit_idx > ST_UNDEFINED)
        {

            Highest_Value = 30;

            *selected_action = BUA_DoomBolt;

        }

    }


    if(
        ((battle_units[battle_unit_idx].Attribs_2 & 0x2 /* Spl_Fireball */ ) != 0)
        &&
        (Ability_Chosen == ST_FALSE)
    )
    {

        spell_idx = spl_Fireball;

        target_battle_unit_idx = AITP_CombatSpell__STUB(spell_idx, battle_units[battle_unit_idx].controller_idx, &Target_X, &Target_Y);

        if(target_battle_unit_idx > ST_UNDEFINED)
        {

            Highest_Value = 30;

            *selected_action = BUA_DoomBolt;

        }


        if(target_battle_unit_idx > ST_UNDEFINED)
        {

            Highest_Value = (16 - BU_GetEffectiveDEF__SEGRAX(target_battle_unit_idx, rat_Fireball, (0x1 /* Imm_Fire */ | 0x20 /* Imm_Magic */), 0, sbr_Chaos));

            *selected_action = BUA_Fireball;

        }


    }


    if(
        ((battle_units[battle_unit_idx].Attribs_2 & spl_Healing) != 0)
        &&
        (Ability_Chosen == ST_FALSE)
    )
    {

        spell_idx = spl_Healing;

        target_battle_unit_idx = AITP_CombatSpell__STUB(spell_idx, battle_units[battle_unit_idx].controller_idx, &Target_X, &Target_Y);

        if(target_battle_unit_idx > ST_UNDEFINED)
        {

            Ability_Chosen = ST_TRUE;

            // ; why is this done here?
            battle_units[battle_unit_idx].Attribs_2 ^= 1;

            *selected_action = bua_Healing;

        }

    }


    if(
        ((battle_units[battle_unit_idx].Attribs_2 & 0x10 /* Spl_Web */) != 0)
        &&
        (Ability_Chosen == ST_FALSE)
    )
    {

        spell_idx = spl_Web;

        target_battle_unit_idx = AITP_CombatSpell__STUB(spell_idx, battle_units[battle_unit_idx].controller_idx, &Target_X, &Target_Y);

        if(target_battle_unit_idx > ST_UNDEFINED)
        {

            Ability_Chosen = ST_TRUE;

            *selected_action = bua_WebSpell;

        }

    }


    if(
        (_UNITS[battle_units[battle_unit_idx].unit_idx].Hero_Slot > -1)
        &&
        (Ability_Chosen == ST_FALSE)
    )
    {

        hero_slot_idx = _UNITS[battle_units[battle_unit_idx].unit_idx].Hero_Slot;

        // ; BUG: this may not be the hero's original owner
        item_spell_idx = _ITEMS[_players[battle_units[battle_unit_idx].controller_idx].Heroes[hero_slot_idx].Items[0]].embed_spell_idx;

        if(
            (item_spell_idx > 0)
            &&
            (battle_units[battle_unit_idx].Item_Charges > 0)
        )
        {

            spell_idx = item_spell_idx;

            target_battle_unit_idx = AITP_CombatSpell__STUB(spell_idx, battle_units[battle_unit_idx].controller_idx, &Target_X, &Target_Y);

            if(target_battle_unit_idx > ST_UNDEFINED)
            {

                Ability_Chosen = ST_TRUE;

                *selected_action = BUA_UseItem;

            }

        }

    }


    if(
        (battle_units[battle_unit_idx].mana > 2)
        &&
        (Ability_Chosen == ST_FALSE)
        &&
        (*selected_action != BUA_No_Spells)
        &&
        ((battle_units[battle_unit_idx].ranged - 5) <= Random(15))
    )
    {

        *selected_action = BUA_CastSpell;

        Ability_Chosen = ST_TRUE;

    }


    if(Ability_Chosen == ST_FALSE)
    {

        for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
        {

            if(
                (battle_units[battle_unit_idx].controller_idx != battle_units[itr_battle_units].controller_idx)
                &&
                (battle_units[itr_battle_units].status == bus_Active)
            )
            {

                if(
                    (has_ranged_attack != ST_TRUE)
                    ||
                    (
                        ((battle_units[itr_battle_units].enchantments & UE_INVISIBILITY) == 0)
                        &&
                        ((battle_units[itr_battle_units].item_enchantments & UE_INVISIBILITY) == 0)
                        &&
                        ((_UNITS[battle_units[itr_battle_units].unit_idx].enchantments & UE_INVISIBILITY) == 0)
                        &&
                        ((battle_units[itr_battle_units].Abilities & UA_INVISIBILITY) == 0)
                    )
                )
                {

                    if(
                        (has_ranged_attack != ST_TRUE)
                        ||
                        (battlefield->city_enchantments[WALL_OF_DARKNESS] <= 0) /* ; BUG: ignores combat-cast Wall of Darkness */
                        ||
                        ((battle_units[battle_unit_idx].Attribs_1 & 0x8 /* Imm_Illusion */) != 0)
                        ||
                        (/* TODO */ BU_IsInCityProper__STUB(battle_unit_idx) != ST_FALSE)
                        ||
                        (BU_IsInCityProper__STUB(itr_battle_units) != ST_TRUE)
                    )
                    {

                        if(
                            (BU_IsVisible__STUB(itr_battle_units) == ST_TRUE)
                            ||
                            (
                                (battle_units[itr_battle_units].cgx == 8)
                                &&
                                (battle_units[itr_battle_units].cgy == 12)
                            )
                        )
                        {

//                             if(
//                                 (_UNITS[battle_units[battle_unit_idx].unit_idx].Hero_Slot <= -1)
//                                 &&
//                                 (has_ranged_attack != ST_FALSE)
//                                 &&
//                                 (AI_NoMeleeHeroSafety == ST_TRUE)
//                                 &&
//                                 (battle_units[battle_unit_idx].melee >= battle_units[itr_battle_units].defense)
//                                 &&
//                                 (battle_units[battle_unit_idx].defense >= ((battle_units[itr_battle_units].melee * 2) / 3))
//                                 &&
//                                 (battle_units[itr_battle_units].ranged_type < srat_Thrown)
//                                 &&
//                                 (battle_units[battle_unit_idx].defense >= ((battle_units[itr_battle_units].ranged * 2) / 3))
//                                 &&
//                                 (
//                                     (battle_units[itr_battle_units].ranged_type < srat_Thrown)
//                                     ||
//                                     (battle_units[battle_unit_idx].defense >= ((battle_units[itr_battle_units].ranged * 2) / 3))
//                                 )
//                             )
                                if(
                                    (_UNITS[battle_units[battle_unit_idx].unit_idx].Hero_Slot > -1)
                                )
                                {
                                    STU_DEBUG_BREAK();
                                    continue;
                                }



                                if(
                                    (_battlefield_city_walls >= 0)
                                    ||
                                    (has_ranged_attack != ST_FALSE)
                                    ||
                                    (Range_To_Battle_Unit(battle_unit_idx, itr_battle_units) == 1)
                                    ||
                                    (battle_units[battle_unit_idx].controller_idx != _combat_defender_player)
                                    ||
                                    (BU_IsInCityProper__STUB(battle_unit_idx) != ST_TRUE)
                                    ||
                                    (
                                        (
                                            (battle_units[battle_unit_idx].cgx != 8)
                                            ||
                                            (battle_units[battle_unit_idx].cgy != 12)
                                            ||
                                            ((_battlefield_city_walls & 0x1) == 0)
                                        )
                                        &&
                                        (BU_IsInCityProper__STUB(itr_battle_units) != ST_FALSE)
                                    )
                                )
                                {

                                    attack_value = AI_BU_GetAttackValue__STUB(battle_unit_idx, itr_battle_units, has_ranged_attack);

                                    if(has_ranged_attack != ST_TRUE)
                                    {

                                        if(attack_value >= Highest_Value)
                                        {

                                            if(attack_value != Highest_Value)
                                            {

                                                Highest_Value = attack_value;

                                                target_battle_unit_idx = itr_battle_units;

                                                *selected_action = bua_Ready;

                                            }
                                            else
                                            {

                                                Total_Health = ((battle_units[itr_battle_units].Cur_Figures * battle_units[itr_battle_units].hits) - battle_units[itr_battle_units].TopFig_Dmg);

                                                Current_Target_Health = ((battle_units[target_battle_unit_idx].Cur_Figures * battle_units[target_battle_unit_idx].hits) - battle_units[target_battle_unit_idx].TopFig_Dmg);

                                                if(Total_Health > Current_Target_Health)
                                                {

                                                    Highest_Value = attack_value;

                                                    target_battle_unit_idx = itr_battle_units;

                                                    *selected_action = bua_Ready;

                                                }

                                            }

                                        }

                                    }
                                    else
                                    {

                                        if(
                                            ((Highest_Value - 3) > attack_value)
                                            ||
                                            ((attack_value - 3) > Highest_Value)
                                            ||
                                            (target_battle_unit_idx <= -1)
                                        )
                                        {

                                            if(Highest_Value < attack_value)
                                            {

                                                Highest_Value = attack_value;
                                                
                                                target_battle_unit_idx = itr_battle_units;

                                                *selected_action = bua_Ready;

                                            }

                                        }
                                        else
                                        {

                                            Total_Health = ((battle_units[itr_battle_units].Cur_Figures * battle_units[itr_battle_units].hits) - battle_units[itr_battle_units].TopFig_Dmg);

                                            Current_Target_Health = ((battle_units[target_battle_unit_idx].Cur_Figures * battle_units[target_battle_unit_idx].hits) - battle_units[target_battle_unit_idx].TopFig_Dmg);

                                            if(Total_Health < Current_Target_Health)
                                            {

                                                if(Highest_Value < attack_value)
                                                {
                                                
                                                    Highest_Value = attack_value;

                                                }

                                                target_battle_unit_idx = itr_battle_units;

                                                *selected_action = bua_Ready;

                                            }

                                        }

                                    }

                                }


                        }

                    }

                }

            }

        }

    }


    return target_battle_unit_idx;

}


// WZD o114p08
// drake178: G_AI_BU_MoveOrRampage()
/*
; a wrapper for the AI battle unit movement function
; that will try to rampage inside the city proper if
; applicable when no enemies can be attacked
;
; RE-EXAMINE the horrible wrapped function!
*/
/*

    IDK, but gets to calling Auto_Move_Ship()


*/
void G_AI_BU_MoveOrRampage__WIP(int16_t battle_unit_idx, int16_t Dest_X, int16_t Dest_Y, int16_t target_battle_unit_idx, int16_t Rally_X, int Rally_Y)
{
    int16_t Rampage_Y = 0;
    int16_t Rampage_X = 0;
    int16_t City_Steps = 0;
    int16_t Town_Y = 0;
    int16_t Town_X = 0;
    int16_t itr_battle_units = 0;  // _SI_
    

/*
    BEGIN:  ~== Auto_Move_Ship()
*/

    Set_Movement_Cost_Map(battle_unit_idx);

    /*
        mark all tiles with units other than the target as
        impassable
    */
    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
    {

        if(
            (itr_battle_units != target_battle_unit_idx)
            &&
            (battle_units[itr_battle_units].status == bus_Active)
        )
        {

            CMB_ActiveMoveMap[((battle_units[itr_battle_units].cgy * COMBAT_GRID_WIDTH) + battle_units[itr_battle_units].cgx)] = INF;

        }

    }


    BU_SetCityMovement__WIP(battle_unit_idx);


    if(
        (battle_units[battle_unit_idx].controller_idx == _combat_defender_player)
        &&
        (G_AI_StayInTownProper == ST_TRUE)
        &&
        (
            (battlefield->Wall_of_Fire > 0)
            ||
            (battlefield->Wall_of_Darkness > 0)
        )
        &&
        (BU_IsInCityProper__STUB(battle_unit_idx) == ST_TRUE)
    )
    {

        AI_RestrictToCity__WIP();

    }


    for(itr_battle_units = 0; itr_battle_units < CMB_Vortex_Count; itr_battle_units++)
    {

        CMB_ActiveMoveMap[((CMB_Vortex_Array[itr_battle_units].cgy * COMBAT_GRID_WIDTH) + CMB_Vortex_Array[itr_battle_units].cgx)] = INF;

    }

    // sets movement_path_grid_cell_count
    Combat_Move_Path_Find(battle_units[battle_unit_idx].cgx, battle_units[battle_unit_idx].cgy, Dest_X, Dest_Y);

/*
    END:  ~== Auto_Move_Ship()
*/


    if(
        (
            (
                (target_battle_unit_idx > ST_UNDEFINED)
                &&
                CMB_ActiveMoveMap[((Dest_Y * COMBAT_GRID_WIDTH) + Dest_X)] == INF
            )
            ||
            (target_battle_unit_idx == ST_UNDEFINED)
        )
        &&
        (battle_units[battle_unit_idx].controller_idx == _combat_attacker_player)
    )
    {

        STU_DEBUG_BREAK();

        City_Steps = 999;

        for(Town_X = 5; Town_X < 9; Town_X++)
        {

            for(Town_Y = 10; Town_Y < 14; Town_Y++)
            {

                // sets movement_path_grid_cell_count
                Combat_Move_Path_Find(battle_units[battle_unit_idx].cgx, battle_units[battle_unit_idx].cgy, Town_X, Town_Y);

                if(
                    (movement_path_grid_cell_count > 0)
                    &&
                    (movement_path_grid_cell_count < City_Steps)
                )
                {

                    Rampage_X = Town_X;

                    Rampage_Y = Town_Y;

                    City_Steps = movement_path_grid_cell_count;

                }

            }

        }

        if(City_Steps < 999)
        {

            Auto_Move_Ship(battle_unit_idx, Rampage_X, Rampage_Y, ST_UNDEFINED, Rampage_X, Rampage_Y);

        }

    }

    Auto_Move_Ship(battle_unit_idx, Dest_X, Dest_Y, target_battle_unit_idx, Rally_X, Rally_Y);

}


// WZD o114p09
// drake178: G_AI_BU_Move()
/*
; creates a path and moves, or starts moving, the
; chosen unit to the specified destination or target
;
; if not the same as the destination, Max_X/Y define
; the rally point - a center tile around which the unit
; can only move within the quadrant it starts in
;
; moves are only expended for the last step, and
; remaining moves are checked only after the first one
;
; recreates the same move map and path as the parent
; function
;
; this is one horrible way of doing things, RE-EXAMINE!
*/
/*

Max_X, Max_Y

*/
int16_t Auto_Move_Ship(int16_t battle_unit_idx, int16_t Dest_X, int16_t Dest_Y, int16_t target_battle_unit_idx, int16_t Max_X, int16_t Max_Y)
{
    int16_t Move_Anim_Base_Speed = 0;
    int16_t First_Step_Index = 0;
    SAMB_INT Sound_Data_Seg = 0;
    int16_t Move_Visible = 0;
    int16_t Y_Distance = 0;
    int16_t X_Distance = 0;
    int16_t Min_Y = 0;
    int16_t Min_X = 0;
    int16_t RP_Origin_Y_2 = 0;
    int16_t RP_Origin_X_2 = 0;
    int16_t First_Step = 0;
    int16_t Attack_Step = 0;
    int16_t Last_Target_Y = 0;
    int16_t Last_Target_X = 0;
    int16_t Origin_Y = 0;
    int16_t Origin_X = 0;
    int16_t Facing_Y_Offset = 0;
    int16_t Facing_X_Offset = 0;
    int16_t itr_battle_units = 0;
    int16_t itr_grid = 0;  // _DI_
    int16_t DBG_path_cost = 0;
    uint32_t Sound_Data_Seg_size = 0;  // DNE in Dasm

    if(_auto_combat_flag == ST_TRUE)
    {

        Move_Anim_Base_Speed = 2;

    }
    else
    {

        Move_Anim_Base_Speed = 1;

    }


/*
    BEGIN:  ~== G_AI_BU_MoveOrRampage__WIP()
*/

    Set_Movement_Cost_Map(battle_unit_idx);


    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
    {

        /*
            mark all tiles with units other than the target as
            impassable

            BUG: this has just been done in the parent function
        */
        if(
            (itr_battle_units != target_battle_unit_idx)
            &&
            (battle_units[itr_battle_units].status == bus_Active)
        )
        {

            CMB_ActiveMoveMap[((battle_units[itr_battle_units].cgy * COMBAT_GRID_WIDTH) + battle_units[itr_battle_units].cgx)] = INF;

        }

    }


    BU_SetCityMovement__WIP(battle_unit_idx);


    if(
        (battle_units[battle_unit_idx].controller_idx == _combat_defender_player)
        &&
        (G_AI_StayInTownProper == ST_TRUE)
        &&
        (
            (battlefield->Wall_of_Fire > 0)
            ||
            (battlefield->Wall_of_Darkness > 0)
        )
        &&
        (BU_IsInCityProper__STUB(battle_unit_idx) == ST_TRUE)
    )
    {

        AI_RestrictToCity__WIP();

    }

/*
mark all tiles with Magic Vortices on them as
impassable

BUG: this has just been done in the parent function
*/
    for(itr_battle_units = 0; itr_battle_units < CMB_Vortex_Count; itr_battle_units++)
    {

        CMB_ActiveMoveMap[((CMB_Vortex_Array[itr_battle_units].cgy * COMBAT_GRID_WIDTH) + CMB_Vortex_Array[itr_battle_units].cgx)] = INF;

    }

    if(battle_unit_idx == 1)
    {
        // DELETEME  STU_DEBUG_BREAK();
    }

    // sets movement_path_grid_cell_count
    Combat_Move_Path_Find(battle_units[battle_unit_idx].cgx, battle_units[battle_unit_idx].cgy, Dest_X, Dest_Y);

/*
    END:  ~== G_AI_BU_MoveOrRampage__WIP()
*/


    if(movement_path_grid_cell_count == 0)
    {
        return ST_FALSE;
    }


    if(battle_units[battle_unit_idx].controller_idx == HUMAN_PLAYER_IDX)
    {

        AI_ImmobileCounter = -1;

    }

    Origin_X = battle_units[battle_unit_idx].cgx;

    Origin_Y = battle_units[battle_unit_idx].cgy;

    battle_units[battle_unit_idx].target_cgx = Dest_X;

    battle_units[battle_unit_idx].target_cgy = Dest_Y;

    RP_CMB_Movement_Var = 0;  // ; cleared before movement, never used for anything

    battle_units[battle_unit_idx].Moving = ST_TRUE;

    RP_Origin_X_2 = battle_units[battle_unit_idx].cgx;

    RP_Origin_Y_2 = battle_units[battle_unit_idx].cgy;

    if(target_battle_unit_idx > -1)
    {

        Dest_X = battle_units[target_battle_unit_idx].cgx;

        Dest_Y = battle_units[target_battle_unit_idx].cgy;

    }


    if(
        (Max_X == Dest_X)
        &&
        (Max_Y == Dest_Y)
    )
    {

        Min_X = COMBAT_GRID_XMIN;
        Max_X = COMBAT_GRID_XMAX;
        Min_Y = COMBAT_GRID_YMIN;
        Max_Y = COMBAT_GRID_YMAX;

    }
    else
    {

        X_Distance = abs(RP_Origin_X_2 - Dest_X);

        Y_Distance = abs(RP_Origin_Y_2 - Dest_Y);

        if(X_Distance < Y_Distance)
        {

            Min_X = COMBAT_GRID_XMIN;

            Max_X = COMBAT_GRID_WIDTH;

            if(RP_Origin_Y_2 < Dest_Y)
            {

                Min_Y = COMBAT_GRID_YMIN;

            }
            else
            {

                Min_Y = Max_Y;

                Max_Y = COMBAT_GRID_HEIGHT;

            }

        }

        if(X_Distance > Y_Distance)
        {

            Min_Y = 0;

            Max_Y = COMBAT_GRID_HEIGHT;

            if(RP_Origin_X_2 < Dest_X)
            {

                Min_X = COMBAT_GRID_XMIN;

            }
            else
            {

                Min_X = Max_X;

                Max_X = COMBAT_GRID_XMAX;

            }

        }

        if(X_Distance == Y_Distance)
        {

            if(RP_Origin_X_2 < Dest_X)
            {

                Min_X = COMBAT_GRID_XMIN;

            }
            else
            {

                Min_X = Max_X;

                Max_X = COMBAT_GRID_XMAX;

            }

            if(RP_Origin_Y_2 > Dest_Y)
            {

                Min_Y = COMBAT_GRID_YMIN;

            }
            else
            {

                Min_Y = Max_Y;

                Max_Y = COMBAT_GRID_YMAX;

            }

        }

    }


    First_Step = ST_TRUE;


    if(
        (
            ((battle_units[battle_unit_idx].enchantments & UE_INVISIBILITY) != 0)
            ||
            ((battle_units[battle_unit_idx].item_enchantments & UE_INVISIBILITY) != 0)
            ||
            ((battle_units[battle_unit_idx].Abilities & UA_INVISIBILITY) != 0)
            ||
            ((_UNITS[battle_units[battle_unit_idx].unit_idx].enchantments & UE_INVISIBILITY) != 0)
        )
        &&
        (battle_units[battle_unit_idx].controller_idx != HUMAN_PLAYER_IDX)
    )
    {

        if(
            (
                (battle_units[battle_unit_idx].controller_idx != _combat_attacker_player)
                &&
                (CMB_ATKR_TrueSight != ST_TRUE)
            )
            ||
            (
                (battle_units[battle_unit_idx].controller_idx != _combat_defender_player)
                &&
                (CMB_DEFR_TrueSight != ST_TRUE)
            )
        )
        {

            Move_Visible = ST_FALSE;

        }
        else
        {

            Move_Visible = ST_TRUE;

        }

    }
    else
    {
        Move_Visible = ST_TRUE;
    }


    First_Step_Index = 0;


    if(
        ((battle_units[battle_unit_idx].Move_Flags & MV_TELEPORT) != 0)
        ||
        ((battle_units[battle_unit_idx].Move_Flags & MV_MERGING) != 0)
    )
    {

        if(
            (target_battle_unit_idx > -1)
            &&
            (CMB_Path_Xs[(movement_path_grid_cell_count - 1)] == battle_units[target_battle_unit_idx].cgx)
            &&
            (CMB_Path_Ys[(movement_path_grid_cell_count - 1)] == battle_units[target_battle_unit_idx].cgy)
        )
        {

            if(movement_path_grid_cell_count > 1)
            {

                First_Step_Index = (movement_path_grid_cell_count - 2);

            }

        }
        else
        {

            First_Step_Index = (movement_path_grid_cell_count - 1);

        }

    }


    for(itr_grid = First_Step_Index; itr_grid < movement_path_grid_cell_count; itr_grid++)
    {

        DBG_path_cost = CMB_Path_Costs[((CMB_Path_Ys[(itr_grid - 1)] * COMBAT_GRID_WIDTH) + CMB_Path_Xs[(itr_grid - 1)])];

        if(
            (First_Step != ST_TRUE)
            &
            (battle_units[battle_unit_idx].movement_points <= CMB_Path_Costs[((CMB_Path_Ys[(itr_grid - 1)] * COMBAT_GRID_WIDTH) + CMB_Path_Xs[(itr_grid - 1)])])
        )
        {

            break;

        }

        if(
            (
                (CMB_Path_Xs[itr_grid] <= Max_X)
                &&
                (CMB_Path_Xs[itr_grid] >= Min_X)
                &&
                (CMB_Path_Ys[itr_grid] <= Max_Y)
                &&
                (CMB_Path_Ys[itr_grid] >= Min_Y)
            )
            ||
            (
                (CMB_Path_Xs[itr_grid] == Dest_X)
                &&
                (CMB_Path_Ys[itr_grid] == Dest_Y)
            )
        )
        {

            First_Step = ST_FALSE;

            battle_units[battle_unit_idx].target_cgx = CMB_Path_Xs[itr_grid];

            battle_units[battle_unit_idx].target_cgy = CMB_Path_Ys[itr_grid];

            Attack_Step = ST_FALSE;

            if(
                (battle_units[battle_unit_idx].target_cgx == battle_units[target_battle_unit_idx].cgx)
                &&
                (battle_units[battle_unit_idx].target_cgy == battle_units[target_battle_unit_idx].cgy)
                &&
                (battle_units[target_battle_unit_idx].status == bus_Active)
            )
            {

                Attack_Step = ST_TRUE;

            }

            if(Attack_Step == ST_TRUE)
            {

                if(battle_units[battle_unit_idx].Cur_Figures <= 0)
                {

                    battle_units[battle_unit_idx].movement_points = -2;

                }
                else
                {

                    Battle_Unit_Attack__WIP(battle_unit_idx, target_battle_unit_idx, 0, 0);

                }

                itr_grid--;

            }
            else
            {

                if(
                    (battle_units[battle_unit_idx].controller_idx != _combat_defender_player)
                    ||
                    (_battlefield_city_walls != BATTLEFIELD_CITY_WALL_STONE)  /* ; BUG: will only be 1 if there's only a stone wall but  ; no other types */
                    ||
                    (
                        (CMB_Path_Xs[itr_grid] >= 5)
                        &&
                        (CMB_Path_Xs[itr_grid] <= 8)
                        &&
                        (CMB_Path_Ys[itr_grid] >= 10)
                        &&
                        (CMB_Path_Ys[itr_grid] <= 13)
                    )
                    ||
                    (BU_IsInCityProper__STUB(battle_unit_idx) != ST_TRUE)
                )
                {

                    if(Move_Visible == ST_TRUE)
                    {

                        if(
                            ((battle_units[battle_unit_idx].Move_Flags & MV_TELEPORT) != 0)
                            ||
                            ((battle_units[battle_unit_idx].Move_Flags & MV_MERGING) != 0)
                        )
                        {

                            if((battle_units[battle_unit_idx].Move_Flags & MV_TELEPORT) != 0)
                            {

                                // TODO  BU_Teleport(battle_unit_idx, CMB_Path_Xs[itr_grid], CMB_Path_Ys[itr_grid]);
                                // ; plays the teleport animation for, and moves the
                                // ; specified unit to the selected tile on the field

                            }
                            else
                            {

                                // TODO  BU_TunnelTo(battle_unit_idx, CMB_Path_Xs[itr_grid], CMB_Path_Ys[itr_grid]);
                                // ; plays the tunneling (Merging) animation for, and
                                // ; moves the specified unit to the selected tile on the
                                // ; battlefield
                                // ;
                                // ; BUG: ignores the unit movement animations setting

                            }

                        }
                        else
                        {

                            if(magic_set.sound_effects == ST_TRUE)
                            {

                                // DOMSDOS  Play_Sound__WIP(SND_CMB_Silence);
                                sdl2_Play_Sound__WIP(SND_CMB_Silence, SND_CMB_Silence_size);

                                Mark_Block(World_Data);

                                // DOMSDOS  Sound_Data_Seg = BU_LoadMoveSound__WIP(battle_unit_idx);
                                BU_LoadMoveSound__WIP(battle_unit_idx, &Sound_Data_Seg, &Sound_Data_Seg_size);

                                Release_Block(World_Data);

                            }
                            else
                            {
                                Sound_Data_Seg = ST_UNDEFINED;
                            }

                            battle_units[battle_unit_idx].MoveStage = 1;

                            if(Sound_Data_Seg != ST_UNDEFINED)
                            {

                                // DOMSDOS  Play_Sound__WIP(Sound_Data_Seg);
                                sdl2_Play_Sound__WIP(Sound_Data_Seg, Sound_Data_Seg_size);

                            }

                            if(magic_set.Movement_Anims == ST_TRUE)
                            {

                                for(itr_battle_units = 0; itr_battle_units < 8; itr_battle_units += Move_Anim_Base_Speed)
                                {

                                    battle_units[battle_unit_idx].MoveStage += Move_Anim_Base_Speed;

                                    Tactical_Combat_Draw();

                                    PageFlip_FX();

                                }

                            }
                            else
                            {

                                battle_units[battle_unit_idx].MoveStage = 7;

                                Tactical_Combat_Draw();

                                PageFlip_FX();

                            }


                            if(magic_set.sound_effects == ST_TRUE)
                            {

                                // DOMSDOS  Play_Sound__WIP(SND_CMB_Silence);
                                sdl2_Play_Sound__WIP(SND_CMB_Silence, SND_CMB_Silence_size);

                            }

                        }

                        battle_units[battle_unit_idx].MoveStage = 0;

                        battle_units[battle_unit_idx].cgx = CMB_Path_Xs[itr_grid];

                        battle_units[battle_unit_idx].cgy = CMB_Path_Ys[itr_grid];

                        assert(battle_units[battle_unit_idx].cgx >= COMBAT_GRID_XMIN);
                        assert(battle_units[battle_unit_idx].cgx <= COMBAT_GRID_XMAX);
                        assert(battle_units[battle_unit_idx].cgy >= COMBAT_GRID_YMIN);
                        assert(battle_units[battle_unit_idx].cgy <= COMBAT_GRID_YMAX);

                    }

                }

            }

        }

    }


    battle_units[battle_unit_idx].MoveStage = 0;

    battle_units[battle_unit_idx].Moving = ST_FALSE;

    if(itr_grid > 1)
    {

        Origin_X = CMB_Path_Xs[(itr_grid - 2)];

        Origin_Y = CMB_Path_Ys[(itr_grid - 2)];

        assert(Origin_X >= COMBAT_GRID_XMIN);
        assert(Origin_X <= COMBAT_GRID_XMAX);
        assert(Origin_Y >= COMBAT_GRID_YMIN);
        assert(Origin_Y <= COMBAT_GRID_YMAX);

        Last_Target_X = CMB_Path_Xs[(itr_grid - 1)];

        Last_Target_Y = CMB_Path_Ys[(itr_grid - 1)];

        assert(Last_Target_X >= COMBAT_GRID_XMIN);
        assert(Last_Target_X <= COMBAT_GRID_XMAX);
        assert(Last_Target_Y >= COMBAT_GRID_YMIN);
        assert(Last_Target_Y <= COMBAT_GRID_YMAX);

    }
    else
    {

        Last_Target_X = Dest_X;

        Last_Target_Y = Dest_Y;

        assert(Last_Target_X >= COMBAT_GRID_XMIN);
        assert(Last_Target_X <= COMBAT_GRID_XMAX);
        assert(Last_Target_Y >= COMBAT_GRID_YMIN);
        assert(Last_Target_Y <= COMBAT_GRID_YMAX);

    }

    Facing_X_Offset = (Last_Target_X - Origin_X);

    Facing_Y_Offset = (Last_Target_Y - Origin_Y);

    if(itr_grid != 0)
    {

        battle_units[battle_unit_idx].cgx = CMB_Path_Xs[(itr_grid - 1)];

        battle_units[battle_unit_idx].cgy = CMB_Path_Ys[(itr_grid - 1)];

        assert(battle_units[battle_unit_idx].cgx >= COMBAT_GRID_XMIN);
        assert(battle_units[battle_unit_idx].cgx <= COMBAT_GRID_XMAX);
        assert(battle_units[battle_unit_idx].cgy >= COMBAT_GRID_YMIN);
        assert(battle_units[battle_unit_idx].cgy <= COMBAT_GRID_YMAX);

        battle_units[battle_unit_idx].target_cgx = (Last_Target_X + Facing_X_Offset);

        battle_units[battle_unit_idx].target_cgy = (Last_Target_Y + Facing_Y_Offset);

        assert(battle_units[battle_unit_idx].target_cgx >= COMBAT_GRID_XMIN);
        assert(battle_units[battle_unit_idx].target_cgx <= COMBAT_GRID_XMAX);
        assert(battle_units[battle_unit_idx].target_cgy >= COMBAT_GRID_YMIN);
        assert(battle_units[battle_unit_idx].target_cgy <= COMBAT_GRID_YMAX);

    }

    if(
        ((battle_units[battle_unit_idx].Move_Flags & MV_TELEPORT) != 0)
        ||
        ((battle_units[battle_unit_idx].Move_Flags & MV_MERGING) != 0)
    )
    {

        // ; BUG: teleporting units can't use roads
        battle_units[battle_unit_idx].movement_points -= 2;

    }
    else
    {

        battle_units[battle_unit_idx].movement_points -= CMB_Path_Costs[((battle_units[battle_unit_idx].cgy * COMBAT_GRID_WIDTH) + battle_units[battle_unit_idx].cgx)];

    }

    return ST_TRUE;

}




/*
    WIZARDS.EXE  ovr116
*/

// WZD o116p01
/*
returns battle_unit->item_enchantments  <-| BU_Apply_Item_Enchantments()

short-circuits if unit is not a hero unit, returns 0 as in no enchantments

¿ ~ Apply Item Powers ?
    |-> BU_Apply_Item_Enchantments()
so, ...
    handles everything except item enchantments?
but, not ip_Flaming or ip_Giant_Strength ?!?
also, ...
    |-> BU_Apply_Item_Attack_Specials()

*/
uint32_t BU_Apply_Item_Powers(int16_t unit_idx, struct s_BATTLE_UNIT * battle_unit)
{
    int16_t hero_slot_idx;
    int16_t unit_owner_idx;
    uint32_t battle_unit_item_enchantments;
    int16_t item_idx;  // _SI_
    int16_t itr_items;  // _DI_


    battle_unit->item_enchantments = 0;
    battle_unit->Melee_ATK_Flags = 0;
    battle_unit->Ranged_ATK_Flags = 0;


    unit_owner_idx = _UNITS[unit_idx].owner_idx;
    hero_slot_idx = _UNITS[unit_idx].Hero_Slot;


    if(hero_slot_idx == ST_UNDEFINED)
    {
        return 0;
    }


    battle_unit_item_enchantments = 0;

    for(itr_items = 0; itr_items < NUM_HERO_ITEMS; itr_items++)
    {

        if(_players[unit_owner_idx].Heroes[hero_slot_idx].Items[itr_items] != ST_UNDEFINED)
        {
            item_idx = _players[unit_owner_idx].Heroes[hero_slot_idx].Items[itr_items];


            BU_Apply_Item_Enchantments(item_idx, battle_unit);


            if(_ITEMS[item_idx].type == it_Shield)
            {
                battle_unit->Abilities = (battle_unit->Abilities | UA_LARGESHIELD);
            }


            if(battle_unit->mana_max > 0)
            {
                battle_unit->mana_max += _ITEMS[item_idx].spell_skill;
            }


            battle_unit->defense          += _ITEMS[item_idx].defense;
            battle_unit->Gold_Defense     += _ITEMS[item_idx].defense;
            battle_unit->movement_points  += _ITEMS[item_idx].moves2;
            battle_unit->resist           += _ITEMS[item_idx].resistance;
            battle_unit->Gold_Resist      += _ITEMS[item_idx].resistance;


            // if it_Sword, it_Mace, it_Axe, it_Misc ...
            if(
                (_ITEMS[item_idx].type <= it_Axe)
                ||
                (_ITEMS[item_idx].type == it_Misc)
            )
            {
                battle_unit->melee         += _ITEMS[item_idx].attack;
                battle_unit->Gold_Melee    += _ITEMS[item_idx].attack;
                battle_unit->Melee_To_Hit  += _ITEMS[item_idx].tohit;
                
                if(ITEM_POWER(item_idx,ip_Flaming))
                {
                    battle_unit->melee += 3;
                    battle_unit->Gold_Melee += 3;
                }

                if(ITEM_POWER(item_idx,ip_Giant_Strength))
                {
                    battle_unit->melee += 1;
                    battle_unit->Gold_Melee += 1;
                }

                // ip_Vampiric,   ip_Lightning, ip_Destruction, ip_Chaos,    ip_Death,     ip_Power_Drain, ip_Holy_Avenger, ip_Phantasmal, ip_Stoning
                // Att_LifeSteal, Att_ArmorPrc, Att_Destruct,   Att_DoomDmg, Att_DthTouch, Att_PwrDrain,   Att_DsplEvil,    Att_Illusion,  Att_StnTouch
                BU_Apply_Item_Attack_Specials(&battle_unit->Melee_ATK_Flags, item_idx);

            }


            if(
                ((_ITEMS[item_idx].type == it_Bow)   && ((battle_unit->ranged_type / 10) == rag_Missile))
                ||
                ((_ITEMS[item_idx].type == it_Staff) && ((battle_unit->ranged_type / 10) == rag_Magic))
                ||
                ((_ITEMS[item_idx].type == it_Wand)  && ((battle_unit->ranged_type / 10) == rag_Magic))
                ||
                ((_ITEMS[item_idx].type == it_Axe)   && (battle_unit->ranged_type == srat_Thrown))
                ||
                (_ITEMS[item_idx].type == it_Misc)
            )
            {
                battle_unit->ranged         += _ITEMS[item_idx].attack;
                battle_unit->Gold_Ranged    += _ITEMS[item_idx].attack;
                battle_unit->Ranged_To_Hit  += _ITEMS[item_idx].tohit;
                
                if(ITEM_POWER(item_idx,ip_Flaming))
                {
                    battle_unit->ranged += 3;
                    battle_unit->Gold_Ranged += 3;
                }

                if(ITEM_POWER(item_idx,ip_Giant_Strength) && (_ITEMS[item_idx].type == it_Axe) && (battle_unit->ranged_type == srat_Thrown))
                {
                    battle_unit->ranged += 1;
                    battle_unit->Gold_Ranged += 1;
                }

                BU_Apply_Item_Attack_Specials(&battle_unit->Ranged_ATK_Flags, item_idx);

            }

        }

    }


    battle_unit_item_enchantments |= battle_unit->item_enchantments;

    return battle_unit_item_enchantments;
}


// WZD o116p02
/*
    sets item powers in battle_unit.item_enchantments

    ¿ "Spell-Specific Artifact Enchantments" ?
*/
void BU_Apply_Item_Enchantments(int16_t item_idx, struct s_BATTLE_UNIT * battle_unit)
{
    uint32_t item_enchantments;

    item_enchantments = 0;

    if(ITEM_POWER(item_idx,ip_Cloak_Of_Fear))
    {
        item_enchantments |= UE_CLOAKOFFEAR;
    }

    if(ITEM_POWER(item_idx,ip_Wraith_Form))
    {
        item_enchantments |= UE_WRAITHFORM;
    }

    if(ITEM_POWER(item_idx,ip_Regeneration))
    {
        item_enchantments |= UE_REGENERATION;
    }

    if(ITEM_POWER(item_idx,ip_Pathfinding))
    {
        item_enchantments |= UE_PATHFINDING;
    }

    if(ITEM_POWER(item_idx,ip_Water_Walking))
    {
        item_enchantments |= UE_WATERWALKING;
    }

    if(ITEM_POWER(item_idx,ip_Resist_Elements))
    {
        item_enchantments |= UE_RESISTELEMENTS;
    }

    if(ITEM_POWER(item_idx,ip_Elemental_Armour))
    {
        item_enchantments |= UE_ELEMENTALARMOR;
    }

    if(ITEM_POWER(item_idx,ip_Endurance))
    {
        item_enchantments |= UE_ENDURANCE;
    }

    if(ITEM_POWER(item_idx,ip_Invisibility))
    {
        item_enchantments |= UE_INVISIBILITY;
    }

    if(ITEM_POWER(item_idx,ip_Flight))
    {
        item_enchantments |= UE_FLIGHT;
    }

    if(ITEM_POWER(item_idx,ip_Resist_Magic))
    {
        item_enchantments |= UE_RESISTMAGIC;
    }

    if(ITEM_POWER(item_idx,ip_Guardian_Wind))
    {
        item_enchantments |= UE_GUARDIANWIND;
    }

    if(ITEM_POWER(item_idx,ip_Magic_Immunity))
    {
        item_enchantments |= UE_MAGICIMMUNITY;
    }

    if(ITEM_POWER(item_idx,ip_True_Sight))
    {
        item_enchantments |= UE_TRUESIGHT;
    }

    if(ITEM_POWER(item_idx,ip_Bless))
    {
        item_enchantments |= UE_BLESS;
    }

    if(ITEM_POWER(item_idx,ip_Lion_Heart))
    {
        item_enchantments |= UE_LIONHEART;
    }

    if(ITEM_POWER(item_idx,ip_Planar_Travel))
    {
        item_enchantments |= UE_PLANARTRAVEL;
    }

    if(ITEM_POWER(item_idx,ip_Righteousness))
    {
        item_enchantments |= UE_RIGHTEOUSNESS;
    }

    if(ITEM_POWER(item_idx,ip_Invulnerability))
    {
        item_enchantments |= UE_INVULNERABILITY;
    }

    if(ITEM_POWER(item_idx,ip_Holy_Avenger))
    {
        item_enchantments |= UE_BLESS;
    }

    battle_unit->item_enchantments |= item_enchantments;

}


// WZD o116p03
/*
    takes a pointer to a battle unit's attack flags field
    translates item powers to attack specials
*/
void BU_Apply_Item_Attack_Specials(uint16_t * attack_flags, int16_t item_idx)
{

    if(ITEM_POWER(item_idx,ip_Vampiric))
    {
        *attack_flags |= Att_LifeSteal;
    }

    if(ITEM_POWER(item_idx,ip_Lightning))
    {
        *attack_flags |= Att_ArmorPrc;
    }

    if(ITEM_POWER(item_idx,ip_Destruction))
    {
        *attack_flags |= Att_Destruct;
    }

    if(ITEM_POWER(item_idx,ip_Chaos))
    {
        *attack_flags |= Att_DoomDmg;
    }

    if(ITEM_POWER(item_idx,ip_Death))
    {
        *attack_flags |= Att_DthTouch;
    }

    if(ITEM_POWER(item_idx,ip_Power_Drain))
    {
        *attack_flags |= Att_PwrDrain;
    }

    if(ITEM_POWER(item_idx,ip_Holy_Avenger))
    {
        *attack_flags |= Att_DsplEvil;
    }

    if(ITEM_POWER(item_idx,ip_Phantasmal))
    {
        *attack_flags |= Att_Illusion;
    }

    if(ITEM_POWER(item_idx,ip_Stoning))
    {
        *attack_flags |= Att_StnTouch;
    }

}


// WZD o116p04
int16_t Unit_Hit_Points(int16_t unit_idx)
{
    int8_t unit_level;
    int16_t Charm_of_Life_Bonus;
    int16_t hit_points;  // _DI_

    unit_level = _UNITS[unit_idx].Level;

    if(
        ((_UNITS[unit_idx].enchantments & UE_HEROISM) != 0)
        &&
        (unit_level < 3)
    )
    {
        unit_level = 3;
    }

    hit_points = _unit_type_table[_UNITS[unit_idx].type].Hits;

    if((_UNITS[unit_idx].enchantments & UE_BLACKCHANNELS) != 0)
    {
        hit_points += 1;
    }

    if((_UNITS[unit_idx].enchantments & UE_LIONHEART) != 0)
    {
        hit_points += 3;
    }

    /*
        BEGIN: Unit is Hero-Unit
    */
    if(_UNITS[unit_idx].Hero_Slot > ST_UNDEFINED)
    {
        if(unit_level > 0)
        {
            hit_points += 1;
        }

        if(HERO_CONSTITUTION(_UNITS[unit_idx].owner_idx, _UNITS[unit_idx].type))
        {
            hit_points += unit_level + 1;
        }

        if(HERO_CONSTITUTION2(_UNITS[unit_idx].owner_idx, _UNITS[unit_idx].type))
        {
            hit_points += (((unit_level + 1) * 3) / 2);
        }

        if(unit_level > 1)
        {
            hit_points += 1;
        }

        if(unit_level > 3)
        {
            hit_points += 1;
        }

    }
    /*
        END: Unit is Hero-Unit
    */

    if(unit_level > 2)
    {
        hit_points += 1;
    }

    if(unit_level > 4)
    {
        hit_points += 1;
    }

    if(unit_level > 5)
    {
        hit_points += 1;
    }

    if(unit_level > 6)
    {
        hit_points += 1;
    }

    if(unit_level > 7)
    {
        hit_points += 1;
    }

    if(_players[_UNITS[unit_idx].owner_idx].Globals[CHARM_OF_LIFE] > 0)
    {
        Charm_of_Life_Bonus = hit_points / 4;
        if(Charm_of_Life_Bonus < 1)
        {
            Charm_of_Life_Bonus = 1;
        }
        hit_points += Charm_of_Life_Bonus;
    }

    return hit_points;
}

// WZD o116p05
int16_t Battle_Unit_Hit_Points(struct s_BATTLE_UNIT * battle_unit)
{
    int16_t Charm_of_Life_Hits;
    int16_t unit_level;
    uint32_t battle_unit_enchantments;
    int16_t unit_idx;
    int16_t hit_points;  // _DI_

    unit_idx = battle_unit->unit_idx;

    unit_level = _UNITS[unit_idx].Level;

    battle_unit_enchantments = _UNITS[unit_idx].enchantments | battle_unit->enchantments | battle_unit->item_enchantments;

    if(
        ((battle_unit_enchantments & UE_HEROISM) != 0)
        &&
        (unit_level < 3)
    )
    {
        unit_level = 3;
    }

    if(
        (_players[battle_unit->controller_idx].Globals[CRUSADE] > 0)
        &&
        ((_unit_type_table[unit_idx].Abilities & UA_FANTASTIC) == 0)
    )
    {
        unit_level++;
    }

    hit_points = _unit_type_table[_UNITS[unit_idx].type].Hits;

    if((battle_unit_enchantments & UE_BLACKCHANNELS) != 0)
    {
        hit_points += 1;
        battle_unit->Gold_Hits += 1;
    }

    if((battle_unit_enchantments & UE_LIONHEART) != 0)
    {
        hit_points += 3;
        battle_unit->Gold_Hits += 3;
    }

    if(_UNITS[unit_idx].Hero_Slot != -1)
    {
        if(unit_level > 0)
        {
            hit_points += 1;
        }

        if(HERO_CONSTITUTION(_UNITS[unit_idx].owner_idx, _UNITS[unit_idx].type))
        {
            hit_points += (unit_level + 1);
        }

        if(HERO_CONSTITUTION2(_UNITS[unit_idx].owner_idx, _UNITS[unit_idx].type))
        {
            hit_points += (((unit_level + 1) * 3) / 2);
        }

        if(unit_level > 1)
        {
            hit_points += 1;
        }

        if(unit_level > 3)
        {
            hit_points += 1;
        }
    }  /* if(_UNITS[unit_idx].Hero_Slot != -1) */

    if(unit_level > 2)
    {
        hit_points += 1;
    }

    if(unit_level > 4)
    {
        hit_points += 1;
    }

    if(unit_level > 5)
    {
        hit_points += 1;
    }

    if(unit_level > 6)
    {
        hit_points += 1;
    }

    if(unit_level > 7)
    {
        hit_points += 1;
    }

    if(_players[battle_unit->controller_idx].Globals[CHARM_OF_LIFE] > 0)
    {
        Charm_of_Life_Hits = (hit_points / 4);
        
        SETMIN(Charm_of_Life_Hits, 1);

        hit_points += Charm_of_Life_Hits;
    }

    hit_points += battle_unit->Extra_Hits;

    battle_unit->Gold_Hits += battle_unit->Extra_Hits;

    return hit_points;
}


// WZD o116p06
// MoO2  Module: COMBINIT  Load_Combat_Ship_()
/*
; creates a battle unit record for the specified unit,
; calculating and setting its overland statistics, and
; saving the struct to the passed destination pointer
*/
void Load_Battle_Unit(int16_t unit_idx, struct s_BATTLE_UNIT * battle_unit)
{
    int16_t Item_Charges;
    int16_t itr;  //  _DI_

    memcpy(battle_unit, &_unit_type_table[_UNITS[unit_idx].type].Melee, sizeof(struct s_UNIT_TYPE));

    battle_unit->Combat_Effects = 0;

    battle_unit->Melee_To_Hit = 0;
    battle_unit->Ranged_To_Hit = 0;

    battle_unit->tohit = 0;  // set in BU_Init_Battle_Unit()

    battle_unit->To_Block = 0;

    battle_unit->Weapon_Plus1 = 0;
    battle_unit->Melee_ATK_Flags = 0;
    battle_unit->Ranged_ATK_Flags = 0;
    battle_unit->item_enchantments = 0;
    battle_unit->Extra_Hits = 0;
    battle_unit->unit_idx = unit_idx;
    battle_unit->Web_HP = 0;

    battle_unit->Gold_Melee = 0;
    battle_unit->Gold_Ranged = 0;
    battle_unit->Gold_Defense = 0;
    battle_unit->Gold_Resist = 0;
    battle_unit->Gold_Hits = 0;
    battle_unit->Grey_Melee = 0;
    battle_unit->Grey_Ranged = 0;
    battle_unit->Grey_Defense = 0;
    battle_unit->Grey_Resist = 0;

    battle_unit->bufpi = ST_UNDEFINED;

    battle_unit->status = bus_Active;

    battle_unit->controller_idx = _UNITS[unit_idx].owner_idx;

    for(itr = 0; itr < 3; itr++)
    {
        battle_unit->damage[itr] = 0;
    }

    battle_unit->enchantments = 0;
    battle_unit->Suppression = 0;
    battle_unit->mana_max = 0;
    battle_unit->Item_Charges = 0;

    battle_unit->Target_BU = ST_UNDEFINED;

    battle_unit->Poison_Strength = 0;

    battle_unit->upkeep = Unit_Gold_Upkeep(unit_idx);

    if((battle_unit->Attack_Flags & 0x04 /* Att_Poison */) != 0)
    {
        battle_unit->Poison_Strength = battle_unit->Spec_Att_Attrib;
    }

    if((battle_unit->ranged_type & 0x68 /* SR_MultiGaze */) != 0)
    {
        battle_unit->Spec_Att_Attrib = 0;
    }

    BU_Init_Battle_Unit(battle_unit);

    battle_unit->mana = battle_unit->mana_max;

    battle_unit->damage[0] = _UNITS[unit_idx].Damage;

    battle_unit->TopFig_Dmg = (_UNITS[unit_idx].Damage % battle_unit->hits);

    battle_unit->Cur_Figures = (battle_unit->Max_Figures - (_UNITS[unit_idx].Damage / battle_unit->hits));

    battle_unit->Atk_FigLoss = 0;
    battle_unit->Confusion_State = 0;
    battle_unit->Blood_Amount = 0;

    battle_unit->Unknown_5A = 0;  // NOTE: Per the Dasm, duplicate `battle_unit->Unknown_5A = 0;`
    battle_unit->Unknown_5A = 0;

    battle_unit->Melee_Anim = 0;
    battle_unit->outline_magic_realm = 0;
    battle_unit->MoveStage = 0;
    battle_unit->Moving = ST_FALSE;
    battle_unit->action = bua_Ready;
    battle_unit->Always_Animate = ST_FALSE;
    battle_unit->Image_Effect = 0;
    battle_unit->Move_Bob = 0;

    // ¿ BUG:  only gets item charges for 1/first item ?
    // Macro:  Unit Is Hero Unit
    // if(HEROSLOT() > ST_UNDEFINED)
    // if(YAYHERO())
    if(_UNITS[unit_idx].Hero_Slot > ST_UNDEFINED)
    {
        // Macro:  Hero Slot (Index)  _players[].Heroes[]
        // if(_players[_UNITS[unit_idx].owner_idx].Heroes[HEROSLOT()].Items[0] > ST_UNDEFINED)
        if(_players[_UNITS[unit_idx].owner_idx].Heroes[_UNITS[unit_idx].Hero_Slot].Items[0] > -1)
        {
            // Macro:  Hero Item (Index)  _ITEMS[]
            // Item_Charges = _ITEMS[_players[UNITOWNER()].Heroes[HEROSLOT()].Items[0]].embed_spell_cnt;
            Item_Charges = _ITEMS[_players[_UNITS[unit_idx].owner_idx].Heroes[_UNITS[unit_idx].Hero_Slot].Items[0]].embed_spell_cnt;
        }
        else
        {
            Item_Charges = 0;
        }

        if(Item_Charges > 0)
        {
            battle_unit->Item_Charges = Item_Charges;
        }
    }

}

// WZD o116p07
// ¿ MoO2  Module: COMBINIT  Load_Combat_Ship_() ?
/*
    base from copy of Unit Type
    calls out for Level Stats, Hero Ability Stats, Item Stats, Enchantment Stats
    
    ...
    Chaos Surge
    ...

    ...
    movement_points
    Hits
*/
void BU_Init_Battle_Unit(struct s_BATTLE_UNIT * battle_unit)
{
    int16_t weapon_quality = 0;
    int8_t var_8 = 0;
    int8_t unit_mutations = 0;
    uint32_t battle_unit_enchantments = 0;
    int16_t chaos_surge = 0;
    int16_t unit_idx = 0;  //  _SI_
    int16_t itr_players = 0;  // _DI_

    unit_idx = battle_unit->unit_idx;

    unit_mutations = _UNITS[unit_idx].mutations;

    battle_unit->tohit = _unit_type_table[_UNITS[battle_unit->unit_idx].type].To_Hit;

    battle_unit->Melee_To_Hit = 0;
    battle_unit->Ranged_To_Hit = 0;
    battle_unit->To_Block = 0;

    battle_unit->Gold_Melee = 0;
    battle_unit->Gold_Ranged = 0;
    battle_unit->Gold_Defense = 0;
    battle_unit->Gold_Resist = 0;
    battle_unit->Gold_Hits = 0;
    battle_unit->Grey_Melee = 0;
    battle_unit->Grey_Ranged = 0;
    battle_unit->Grey_Defense = 0;
    battle_unit->Grey_Resist = 0;

    battle_unit->resist = _unit_type_table[_UNITS[battle_unit->unit_idx].type].Resist;

    battle_unit->defense = _unit_type_table[_UNITS[battle_unit->unit_idx].type].Defense;

    battle_unit->melee = _unit_type_table[_UNITS[battle_unit->unit_idx].type].Melee;

    battle_unit->ranged = _unit_type_table[_UNITS[battle_unit->unit_idx].type].Ranged;

    if(BU_CASTER_40())
    {
        battle_unit->mana_max = 40;
    }

    if(BU_CASTER_20())
    {
        battle_unit->mana_max = 20;
    }

    BU_Apply_Level(unit_idx, battle_unit);

    if(BU_LUCKY())
    {
        battle_unit->tohit += 1;
        battle_unit->To_Block += 1;
        battle_unit->resist += 1;
        battle_unit->Gold_Resist += 1;
    }

    /*
        Unit is a Hero Unit
    */
    if(_UNITS[unit_idx].Hero_Slot > -1)
    {
        if(battle_unit->Weapon_Plus1 == 0)
        {
            battle_unit->Weapon_Plus1 = 1;
        }

        BU_Init_Hero_Unit(unit_idx, battle_unit);

        BU_Apply_Item_Powers(unit_idx, battle_unit);
            // |-> 
            // |-> 
    }


    battle_unit_enchantments = battle_unit->item_enchantments | _UNITS[unit_idx].enchantments;


    weapon_quality = (_UNITS[unit_idx].mutations & 0x03);  // ; mask first 3 bits

    if(weapon_quality > 0)
    {
        if(battle_unit->melee > 0)
        {
            battle_unit->melee += (weapon_quality - 1);
            battle_unit->Gold_Melee += (weapon_quality - 1);
            battle_unit->Melee_To_Hit += 1;
        }

        if(
            ((battle_unit->ranged_type / 10) == rag_Missile)
            ||
            ((battle_unit->ranged_type / 10) == rag_Boulder)
            ||
            (battle_unit->ranged_type == srat_Thrown)
        )
        {
            battle_unit->ranged += (weapon_quality - 1);
            battle_unit->Gold_Ranged += (weapon_quality - 1);
            battle_unit->Ranged_To_Hit += 1;
        }

            battle_unit->defense += (weapon_quality - 1);
            battle_unit->Gold_Defense += (weapon_quality - 1);
            battle_unit->Weapon_Plus1 = (weapon_quality + 1);
    }


    // drake178:  BUG: only checks the human player's global enchantments in each iteration of the player loop
    chaos_surge = ST_FALSE;
    for(itr_players = 0; itr_players < NUM_PLAYERS; itr_players++)
    {
        if(_players[HUMAN_PLAYER_IDX].Globals[CHAOS_SURGE] > 0)
        {
            chaos_surge = ST_TRUE;
        }
    }

    if(chaos_surge == ST_TRUE)
    {
        if(battle_unit->race == rt_Chaos)
        {
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged += 2;
                battle_unit->Gold_Ranged += 2;
            }
            if(battle_unit->melee > 0)
            {
                battle_unit->melee += 2;
                battle_unit->Gold_Melee += 2;
            }
        }
    }

    if(_players[_UNITS[unit_idx].owner_idx].Globals[HOLY_ARMS] > 0)
    {
        if(
            ((battle_unit->Abilities & UA_FANTASTIC) != 0)
        )
        {
            if((unit_mutations & UM_UNDEAD) != 0)
            {
                if((battle_unit_enchantments & UE_HOLYWEAPON) != 0)
                {
                    battle_unit->enchantments |= UE_HOLYWEAPON;
                    battle_unit->Melee_To_Hit += 1;

                    if(
                        (battle_unit->ranged_type == srat_Thrown)
                        ||
                        ((battle_unit->ranged_type / 10) == rag_Boulder)
                        ||
                        ((battle_unit->ranged_type / 10) == rag_Missile)
                    )
                    {
                        battle_unit->Ranged_To_Hit += 1;
                    }

                    if(battle_unit->Weapon_Plus1 == 0)
                    {
                        battle_unit->Weapon_Plus1 = 1;
                    }
                }
            }
        }
    }

    if(
        ((battle_unit->Abilities & UA_FANTASTIC) != 0)
        ||
        (battle_unit->race > rt_Troll)
    )
    {
        if(battle_unit->Weapon_Plus1 == 0)
        {
            battle_unit->Weapon_Plus1 = 1;
        }
    }

    if((battle_unit->Abilities & UA_NONCORPOREAL) != 0)
    {
        battle_unit->Move_Flags |= MV_SWIMMING;
    }


    BU_Apply_Specials(battle_unit, battle_unit_enchantments, unit_mutations);
    

    battle_unit->movement_points = Unit_Moves2(unit_idx);

    battle_unit_enchantments = battle_unit->item_enchantments | _UNITS[unit_idx].enchantments;


    // ¿ maybe, but, also, why is this even here ?
    // drake178:  ; BUG: value not in halves, but if it was, it would cause a display bug below
    //              this entire check is completely redundant, as the above function already incorporates it,
    //              while any overland movement calculations will IGNORE this value,
    //              causing a discrepancy between the displayed and the actual movement allowance of the unit
    //              it could also only ever trigger if an opposing Wind Mastery has reduced the moves of a ship with Flight
    if(battle_unit->movement_points < 3)
    {
        if((battle_unit_enchantments & UE_FLIGHT) != 0)
        {
            // drake178:  ; BUG: could override Wind Mastery malus overland, but
            battle_unit->movement_points = 6;
        }
    }


    battle_unit->hits = Unit_Hit_Points(unit_idx);

}


// WZD o116p08
void BU_Apply_Specials(struct s_BATTLE_UNIT * battle_unit, uint32_t battle_unit_enchantments, uint8_t unit_mutations)
{

    if((battle_unit_enchantments & UE_WATERWALKING) != 0)
    {
        battle_unit->Move_Flags |= MV_SWIMMING;
    }

    if((battle_unit_enchantments & UE_TRUESIGHT) != 0)
    {
        battle_unit->Attribs_1 |= USA_IMMUNITY_ILLUSION;
    }

    if((battle_unit_enchantments & UE_INVULNERABILITY) != 0)
    {
        battle_unit->Attribs_1 |= USA_IMMUNITY_WEAPON;
    }

    if((battle_unit_enchantments & UE_FLIGHT) != 0)
    {
        battle_unit->Move_Flags |= MV_FLYING;
    }

    if((battle_unit_enchantments & UE_WRAITHFORM) != 0)
    {
        battle_unit->Abilities |= UA_NONCORPOREAL;
        battle_unit->Attribs_1 |= USA_IMMUNITY_WEAPON;
        battle_unit->Move_Flags |= MV_SWIMMING;
    }

    if((unit_mutations & UM_UNDEAD) != 0)
    {
        battle_unit->race = rt_Death;
        battle_unit->Abilities |= UA_FANTASTIC;
    }

    if((battle_unit_enchantments & UE_BLACKCHANNELS) != 0)
    {
        if(battle_unit->melee > 0)        
        {
            battle_unit->melee += 2;
            battle_unit->Gold_Melee += 2;
        }

        if(battle_unit->ranged != -1)
        {
            battle_unit->ranged += 1;
            battle_unit->Gold_Ranged += 1;
        }

        battle_unit->defense += 1;
        battle_unit->Gold_Defense += 1;
        battle_unit->resist += 1;
        battle_unit->Gold_Resist += 1;

        battle_unit->race = rt_Death;
        battle_unit->Abilities |= UA_FANTASTIC;

        battle_unit->Attribs_1 |= (USA_IMMUNITY_ILLUSION | USA_IMMUNITY_COLD | USA_IMMUNITY_POISON);

    }

    if((battle_unit_enchantments & UE_IRONSKIN) != 0)
    {
        battle_unit->defense += 5;
        battle_unit->Gold_Defense += 5;
    }
    else if((battle_unit_enchantments & UE_IRONSKIN) != 0)
    {
        battle_unit->defense += 1;
        battle_unit->Gold_Defense += 1;
    }

    if((battle_unit_enchantments & UE_GUARDIANWIND) != 0)
    {
        battle_unit->Attribs_1 |= USA_IMMUNITY_MISSILES;
    }

    if((battle_unit_enchantments & UE_MAGICIMMUNITY) != 0)
    {
        battle_unit->Attribs_1 |= USA_IMMUNITY_MAGIC;
    }

    if((battle_unit_enchantments & UE_FLAMEBLADE) != 0)
    {
        if(battle_unit->melee > 0)
        {
            battle_unit->melee += 2;
            battle_unit->Gold_Melee += 2;
        }

        if(
            (battle_unit->ranged_type == srat_Thrown)
            ||
            ((battle_unit->ranged_type / 10) == rag_Missile)
        )
        {
            battle_unit->ranged += 2;
            battle_unit->Gold_Ranged += 2;
        }
    }

    if((battle_unit_enchantments & UE_GIANTSTRENGTH) != 0)
    {
        if(battle_unit->melee > 0)
        {
            battle_unit->melee += 1;
            battle_unit->Gold_Melee += 1;
        }

        if(battle_unit->ranged_type == srat_Thrown)
        {
            battle_unit->ranged += 1;
            battle_unit->Gold_Ranged += 1;
        }
    }

    if((battle_unit_enchantments & UE_IMMOLATION) != 0)
    {
        battle_unit->Attribs_2 |= USA_IMMOLATION;
    }

    if((battle_unit_enchantments & UE_ELDRITCHWEAPON) != 0)
    {
        battle_unit->Melee_ATK_Flags |= Att_EldrWeap;

        if(
            ((battle_unit->ranged_type / 10) == rag_Missile)
            ||
            (battle_unit->ranged_type == srat_Thrown)
        )
        {
            battle_unit->Ranged_ATK_Flags |= Att_EldrWeap;
        }

        if(battle_unit->Weapon_Plus1 == 0)
        {
            battle_unit->Weapon_Plus1 = 1;
        }
    }

    if((unit_mutations & CC_ARMOR) != 0)
    {
        battle_unit->defense += 3;
        battle_unit->Gold_Defense += 3;
        battle_unit->race = rt_Chaos;
    }

    if((unit_mutations & CC_FLIGHT) != 0)
    {
        battle_unit->Move_Flags |= MV_FLYING;
        battle_unit->race = rt_Chaos;
    }

    if((unit_mutations & CC_BREATH) != 0)
    {
        battle_unit->ranged = 2;
        battle_unit->ranged_type = srat_FireBreath;
        battle_unit->race = rt_Chaos;
    }

    if((battle_unit_enchantments & UE_LIONHEART) != 0)
    {
        if(battle_unit->melee > 0)
        {
            battle_unit->melee += 3;
            battle_unit->Gold_Melee += 3;
        }

        if(
            ((battle_unit->ranged_type / 10) == rag_Missile)
            ||
            ((battle_unit->ranged_type / 10) == rag_Boulder)
            ||
            (battle_unit->ranged_type == srat_Thrown)
        )
        {
            battle_unit->ranged += 3;
            battle_unit->Gold_Ranged += 3;
        }

        battle_unit->resist += 3;
        battle_unit->Gold_Resist += 3;
    }

    if((battle_unit_enchantments & UE_HOLYARMOR) != 0)
    {
        battle_unit->defense += 2;
        battle_unit->Gold_Defense += 2;
    }

    // drake178:  ; BUG: undead are also supposed to gain Cold, Poison, and Illusions Immunities (native Death units have all of these anyway)
    if(battle_unit->race == rt_Death)
    {
        battle_unit->Attribs_1 |= USA_IMMUNITY_DEATH;
    }

    if((battle_unit_enchantments & UE_BERSERK) != 0)
    {
        battle_unit->Gold_Melee = battle_unit->melee;
        battle_unit->melee *= 2;
        battle_unit->Grey_Defense = battle_unit->defense;
        battle_unit->defense = -20;
    }

}


// WZD o116p09
void BU_Apply_Level(int16_t unit_idx, struct s_BATTLE_UNIT * battle_unit)
{
    int16_t unit_level;  // _SI_

    unit_level = _UNITS[unit_idx].Level;

    if(
        ((_UNITS[unit_idx].enchantments & UE_HEROISM) != 0)
        ||
        ((battle_unit->enchantments & UE_HEROISM) != 0)
    )
    {
        if(unit_level < 3)
        {
            unit_level = 3;
        }
    }


    if(_UNITS[unit_idx].Hero_Slot > -1)
    {
        if(unit_level > 0)
        {
            battle_unit->resist++;
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged++;
            }
            if(battle_unit->melee > 0)
            {
                battle_unit->melee++;
            }
            battle_unit->defense++;
        }

        if(unit_level > 1)
        {
            battle_unit->tohit++;
            battle_unit->resist++;
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged++;
            }
            if(battle_unit->melee > 0)
            {
                battle_unit->melee++;
            }
        }

        if(unit_level > 2)
        {
            battle_unit->resist++;
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged++;
            }
            if(battle_unit->melee > 0)
            {
                battle_unit->melee++;
            }
            battle_unit->defense++;
        }

        if(unit_level > 3)
        {
            battle_unit->resist++;
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged++;
            }
            if(battle_unit->melee > 0)
            {
                battle_unit->melee++;
            }
        }

        if(unit_level > 4)
        {
            battle_unit->tohit++;
            battle_unit->resist++;
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged++;
            }
            if(battle_unit->melee > 0)
            {
                battle_unit->melee++;
            }
            battle_unit->defense++;
        }

        if(unit_level > 5)
        {
            battle_unit->resist++;
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged++;
            }
            if(battle_unit->melee > 0)
            {
                battle_unit->melee++;
            }
        }

        if(unit_level > 6)
        {
            battle_unit->resist++;
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged++;
            }
            if(battle_unit->melee > 0)
            {
                battle_unit->melee++;
            }
            battle_unit->defense++;
        }

        if(unit_level > 7)
        {
            battle_unit->tohit++;
            battle_unit->resist++;
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged++;
            }
            if(battle_unit->melee > 0)
            {
                battle_unit->melee++;
            }
        }
    }
    else
    {
        if(unit_level > 0)
        {
            battle_unit->resist++;
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged++;
            }
            if(battle_unit->melee > 0)
            {
                battle_unit->melee++;
            }
        }

        if(unit_level > 1)
        {
            battle_unit->resist++;
            battle_unit->defense++;
        }

        if(unit_level > 2)
        {
            battle_unit->tohit++;
            battle_unit->resist++;
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged++;
            }
            if(battle_unit->melee > 0)
            {
                battle_unit->melee++;
            }
        }

        if(unit_level > 3)
        {
            battle_unit->tohit++;
            battle_unit->resist++;
            battle_unit->defense++;
        }

        if(unit_level > 4)
        {
            battle_unit->tohit++;
            battle_unit->resist++;
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged++;
            }
            if(battle_unit->melee > 0)
            {
                battle_unit->melee++;
            }
        }
    }

}


// WZD o116p10
/*
    ¿ overwrites much of what was just done ?

    upkeep

*/
void BU_Init_Hero_Unit(int16_t unit_idx, struct s_BATTLE_UNIT * battle_unit)
{
    int16_t hero_owner_idx;  // _SI_
    int16_t hero_type;  // _DI_

    hero_owner_idx = UNITOWNER();
    hero_type = UNITTYPE();
    
    if(
        (HERO_NOBLE(hero_owner_idx, hero_type))
        ||
        (_players[hero_owner_idx].famous > ST_FALSE)
    )
    {
        battle_unit->upkeep = 0;
    }


    if(HERO_AGILITY(hero_owner_idx, hero_type))
    {
        battle_unit->defense = (battle_unit->defense + (_UNITS[unit_idx].Level + 1));
    }
    
    if(HERO_AGILITY2(hero_owner_idx,hero_type))
    {
        battle_unit->defense = (battle_unit->defense + (((_UNITS[unit_idx].Level + 1) * 3) / 2));
    }

    if(HERO_BLADEMASTER(hero_owner_idx,hero_type))
    {
        battle_unit->tohit = (battle_unit->tohit + ((_UNITS[unit_idx].Level + 1) / 2));
    }
    
    if(HERO_BLADEMASTER2(hero_owner_idx,hero_type))
    {
        battle_unit->tohit = (battle_unit->tohit + (((_UNITS[unit_idx].Level + 1) * 3) / 4));
    }

    if(HERO_MIGHT(hero_owner_idx,hero_type))
    {
        battle_unit->melee = (battle_unit->melee + (_UNITS[unit_idx].Level + 1));
    }
    
    if(HERO_MIGHT2(hero_owner_idx,hero_type))
    {
        battle_unit->melee = (battle_unit->melee + (((_UNITS[unit_idx].Level + 1) * 3) / 2));
    }


    if(
        (HERO_ARCANE_POWER(hero_owner_idx,hero_type))
        &&
        ((battle_unit->ranged_type / 10) == rag_Magic)
    )
    {
        battle_unit->ranged = (battle_unit->ranged + (_UNITS[unit_idx].Level + 1));
    }
    
    if(
        (HERO_ARCANE_POWER2(hero_owner_idx,hero_type))
        &&
        ((battle_unit->ranged_type / 10) == rag_Magic)
    )
    {
        battle_unit->ranged = (battle_unit->ranged + (((_UNITS[unit_idx].Level + 1) * 3) / 2));
    }

    if(_HEROES2[hero_owner_idx]->heroes[hero_type].Casting_Skill > 0)
    {
        battle_unit->mana_max = ((((_HEROES2[hero_owner_idx]->heroes[hero_type].Casting_Skill + 1) * (_UNITS[unit_idx].Level + 1)) * 5) / 2);
    }
    else
    {
        battle_unit->mana_max = 0;
    }

    if(HERO_LUCKY(hero_owner_idx,hero_type))
    {
        battle_unit->tohit += 1;
        battle_unit->To_Block += 1;
        battle_unit->resist += 1;
        battle_unit->Gold_Resist += 1;
    }

}


// WZD o116p11
/*
    Magic Node Combat Effect



Page 98  (PDF Page 103)
STRUCTURES INFLUENCING COMBAT
Magic Nodes
Magic nodes can affect combat in one of two ways.
They may increase (by one) the attack strengths (swords and ranged weapons), defenses (shields) and resistance (crosses) of all creatures from the same magic realm as the node
  (i.e., a nature node would exert this positive effect on all creatures of nature such as sprites and basilisks)
  when these creatures are fighting in any area controlled by the node.

*/
void BU_Apply_Battlefield_Effects__WIP(struct s_BATTLE_UNIT * battle_unit)
{
    int16_t Node_Aura_Applies;
    int16_t Item_Index;
    int16_t Mutation_Flags;
    uint32_t enchantments;
    int16_t unit_idx;

    unit_idx = battle_unit->unit_idx;

    Node_Aura_Applies = ST_FALSE;

    if(
        (
            (_combat_node_type == cnt_Sorcery)
            &&
            (battle_unit->race == rt_Sorcery)
        )
        ||
        (
            (_combat_node_type == cnt_Chaos)
            &&
            (battle_unit->race == rt_Chaos)
        )
        ||
        (
            (_combat_node_type == cnt_Nature)
            &&
            (battle_unit->race == rt_Nature)
        )
    )
    {
        Node_Aura_Applies = ST_TRUE;
    }

    if(Node_Aura_Applies == ST_TRUE)
    {
        battle_unit->melee += 2;
        battle_unit->Gold_Melee += 2;

        if(battle_unit->ranged > 0)
        {
            battle_unit->ranged += 2;
            battle_unit->Gold_Ranged += 2;
        }

        battle_unit->resist += 2;
        battle_unit->Gold_Resist += 2;

        battle_unit->defense += 2;
        battle_unit->Gold_Defense += 2;
    }



    // ...
    // ...
    // ...



    battle_unit[0].hits = Battle_Unit_Hit_Points(&battle_unit[0]);

}


// WZD o116p12
// sub_9A43E()



/*
    WIZARDS.EXE  ovr122
*/

// Segrax
// WZD o122p01
// drake178: CMB_AttackRoll()
/*
*/
/*

one D10 roll per attack (strength) units
returns count of applicable attack units

*/
int16_t CMB_AttackRoll__SEGRAX(uint16_t attack_strength, uint16_t to_hit) {
    int16_t die_roll;
    uint16_t success_count = 0;
    uint16_t total_rolls = 0;

    while (total_rolls < attack_strength) {
        die_roll = Random(10);

        if ((8 - to_hit) <= die_roll || die_roll == 10) {
            success_count++;
        }
        total_rolls++;
    }

    return success_count;
}


// WZD o122p02
// Segrax
// drake178: CMB_DefenseRoll()
/*
; performs a defense roll, returning the amount of
; damage points blocked
*/
/*

one D10 roll per defense unit
returns count of applicable defense units

*/
uint16_t CMB_DefenseRoll__SEGRAX(uint16_t defense, uint16_t to_block) {
    uint16_t success_count = 0;
    uint16_t total_rolls = 0;

    while (total_rolls < defense) {
        int16_t die_roll = Random(10);

        if (die_roll >= (8 - to_block)) {
            success_count++;
        }

        total_rolls++;
    }

    return success_count;
}

// WZD o122p03
// drake178: BU_ResistRoll()
/*
; calculates the unit's effective Resistance score,
; then makes a resistance check with the passed
; modifier, returning 0 if the unit succeeded, or the
; difference from the target number if it didn't
*/
int16_t BU_ResistRoll__STUB(struct s_BATTLE_UNIT battle_unit, int16_t Save_Mod, int16_t magic_realm)
{

    return 0;

}


// WZD o122p04
// drake178: BU_GetEffectiveRES()

// WZD o122p05
// drake178: BU_GetATKImmFlags()

// WZD o122p06
// drake178: BU_AttackTarget()
/*
; processes an attack made by the chosen unit against
; the specified target, returning the damage done to
; both units in the passed array pointers
;
; BUG: applies Cause Fear to the wrong unit on attacks
; BUG: ranged magical attacks are often not doubled by
;  Haste as intended
; BUG: First Strike figures slain by Doom Gaze will
;  sometimes still perform their melee attacks
*/
/*

OON XREF: Battle_Unit_Attack__WIP()

SpFx {F,T}
as compiled, hard-coded to ST_TRUE

*/
void BU_AttackTarget__WIP(int16_t attacker_battle_unit_idx, int16_t defender_battle_unit_idx, int16_t defender_damage_array[], int16_t attacker_damage_array[], int16_t ranged_attack_flag, int16_t SpFx)
{
    int16_t Can_Attack_Again = 0;
    int16_t Source_Unit_Damage = 0;
    int16_t Feared_Figures = 0;
    int16_t ranged_attack_check = 0;
    int16_t Figs = 0;
    int16_t Target_Damage_Sum = 0;
    int16_t Damage_Array[3] = { 0, 0, 0 };
    int16_t itr_damage_types = 0;  // _SI_

    Feared_Figures = 0;

    // ; zero out both damage return arrays
    for(itr_damage_types = 0; itr_damage_types < 3; itr_damage_types++)
    {

        defender_damage_array[itr_damage_types] = 0;

        attacker_damage_array[itr_damage_types] = 0;

    }

    Source_Unit_Damage = 0;

    // {0: allow, 1: disallow, 2: disallow}
    ranged_attack_check = Check_Attack_Ranged(attacker_battle_unit_idx, defender_battle_unit_idx);

    if(ranged_attack_flag != ST_TRUE)
    {

        // ; if the unit has a short range attack, process it and,
        // ; if it is hasted and the attack is not a gaze, repeat
        // ; it once more

        if(battle_units[attacker_battle_unit_idx].ranged_type >= srat_Thrown)
        {

            BU_ProcessAttack__WIP(attacker_battle_unit_idx, battle_units[attacker_battle_unit_idx].Cur_Figures, defender_battle_unit_idx, am_ThrownOrBreath, &Damage_Array[0], 0, SpFx);

            for(itr_damage_types = 0; itr_damage_types < 3; itr_damage_types++)
            {

                defender_damage_array[itr_damage_types] += Damage_Array[itr_damage_types];

            }

            if(
                ((battle_units[attacker_battle_unit_idx].Combat_Effects & bue_Haste) != 0)
                &&
                (battle_units[attacker_battle_unit_idx].ranged_type < srat_StoneGaze)
            )
            {

                BU_ProcessAttack__WIP(attacker_battle_unit_idx, battle_units[attacker_battle_unit_idx].Cur_Figures, defender_battle_unit_idx, am_ThrownOrBreath, &Damage_Array[0], 0, SpFx);

                for(itr_damage_types = 0; itr_damage_types < 3; itr_damage_types++)
                {

                    defender_damage_array[itr_damage_types] += Damage_Array[itr_damage_types];

                }

            }

        }

        // AFTER
        // if(battle_units[attacker_battle_unit_idx].ranged_type >= srat_Thrown)

        if(
            ((battle_units[defender_battle_unit_idx].Combat_Effects & bue_Black_Sleep) != 0)
            &&
            (battle_units[defender_battle_unit_idx].ranged_type >= srat_StoneGaze)
        )
        {

            Target_Damage_Sum = 0;

            for(itr_damage_types = 0; itr_damage_types < 3; itr_damage_types++)
            {

                Target_Damage_Sum += defender_damage_array[itr_damage_types];

            }

            Target_Damage_Sum += battle_units[defender_battle_unit_idx].TopFig_Dmg;

            if(Target_Damage_Sum > 0)
            {

                Figs = (battle_units[defender_battle_unit_idx].Cur_Figures - (Target_Damage_Sum / battle_units[defender_battle_unit_idx].hits));

            }
            else
            {

                Figs = battle_units[defender_battle_unit_idx].Cur_Figures;

            }

            BU_ProcessAttack__WIP(defender_battle_unit_idx, Figs, attacker_battle_unit_idx, am_ThrownOrBreath, &Damage_Array[0], 1, SpFx);

            for(itr_damage_types = 0; itr_damage_types < 3; itr_damage_types++)
            {

                defender_damage_array[itr_damage_types] += Damage_Array[itr_damage_types];

            }

            // ; transfer the damage to the counter attack damage
            // ; return array, and sum it into a local variable too

            attacker_damage_array[itr_damage_types] += Damage_Array[itr_damage_types];

            Source_Unit_Damage += Damage_Array[itr_damage_types];

        }


        // Wall of Fire, First Strike, Cause Fear
            
        if(SpFx != 0)
        {

            BU_WallofFire__NOOP(attacker_battle_unit_idx);

        }

        if(battle_units[attacker_battle_unit_idx].status == bus_Active)
        {

            // ; if the attacking unit has First Strike, and the
            // ; defender can't negate it, process an attack before
            // ; the defender could retaliate
            // ; BUG: Cause Fear is applied to the wrong unit
            // ; BUG: the attacker's damage taken is not calculated
            // ;  properly, and can yield extra attacks on their part

            /*
                BEGIN: Melee
            */

            /*
                BEGIN: Attacker First-Strike
            */
            if(
                ((battle_units[attacker_battle_unit_idx].Attack_Flags & Att_1stStrike) != 0)
                &&
                ((battle_units[defender_battle_unit_idx].Abilities & UA_NEGATEFIRSTSTRIKE) == 0)
            )
            {

                if(SpFx == 1)
                {

                    // ; BUG: wrong parameter order, this fear will backfire
                    Feared_Figures = BU_CauseFear__NOOP(attacker_battle_unit_idx, defender_battle_unit_idx);

                }

                for(itr_damage_types = 0; itr_damage_types < 3; itr_damage_types++)
                {

                    // ; BUG: not all gaze attacks and associated effects
                    // ; deal figure-based damage
                    Feared_Figures += (attacker_damage_array[itr_damage_types] / battle_units[attacker_battle_unit_idx].hits);

                }

                BU_ProcessAttack__WIP(attacker_battle_unit_idx, (battle_units[attacker_battle_unit_idx].Cur_Figures - Feared_Figures), defender_battle_unit_idx, am_Melee, &Damage_Array[0], 0, SpFx);

                for(itr_damage_types = 0; itr_damage_types < 3; itr_damage_types++)
                {

                    defender_damage_array[itr_damage_types] += Damage_Array[itr_damage_types];

                }

            }
            /*
                END: Attacker First-Strike
            */

            /*
                BEGIN: Defender Counter-Attack
            */
            if((battle_units[defender_battle_unit_idx].Combat_Effects & bue_Black_Sleep) == 0)
            {

                // ; sum up the damage taken by the target unit so far
                
                Target_Damage_Sum = 0;

                for(itr_damage_types = 0; itr_damage_types < 3; itr_damage_types++)
                {

                    Target_Damage_Sum += defender_damage_array[itr_damage_types];

                }

                Target_Damage_Sum += battle_units[defender_battle_unit_idx].TopFig_Dmg;

                if(Target_Damage_Sum > 0)
                {

                    Figs = (battle_units[defender_battle_unit_idx].Cur_Figures - (Target_Damage_Sum / battle_units[defender_battle_unit_idx].hits));

                }
                else
                {

                    Figs = battle_units[defender_battle_unit_idx].Cur_Figures;

                }

                if(SpFx == 1)
                {

                    Figs -= BU_CauseFear__NOOP(attacker_battle_unit_idx, defender_battle_unit_idx);

                }

                if(Figs > 0)
                {

                    BU_ProcessAttack__WIP(defender_battle_unit_idx, Figs, attacker_battle_unit_idx, am_Melee, &Damage_Array[0], 1, SpFx);

                    for(itr_damage_types = 0; itr_damage_types < 3; itr_damage_types++)
                    {

                        attacker_damage_array[itr_damage_types] += Damage_Array[itr_damage_types];

                    }

                    if((battle_units[defender_battle_unit_idx].Combat_Effects & bue_Haste) != 0)
                    {

                        BU_ProcessAttack__WIP(defender_battle_unit_idx, Figs, attacker_battle_unit_idx, am_Melee, &Damage_Array[0], 1, SpFx);

                        for(itr_damage_types = 0; itr_damage_types < 3; itr_damage_types++)
                        {

                            attacker_damage_array[itr_damage_types] += Damage_Array[itr_damage_types];

                        }

                    }

                }

            }
            /*
                END: Defender Counter-Attack
            */

            /*
                BEGIN: Attacker Non-First-Strike
            */
            if(
                ((battle_units[attacker_battle_unit_idx].Attack_Flags & Att_1stStrike) == 0)
                ||
                ((battle_units[defender_battle_unit_idx].Abilities & UA_NEGATEFIRSTSTRIKE) != 0)
            )
            {

                if(SpFx == 1)
                {

                    // ; BUG: wrong parameter order, this fear will backfire
                    // BUGBUG
                    Feared_Figures = BU_CauseFear__NOOP(attacker_battle_unit_idx, defender_battle_unit_idx);

                }

                Source_Unit_Damage += battle_units[attacker_battle_unit_idx].TopFig_Dmg;

                Feared_Figures += (Source_Unit_Damage / battle_units[attacker_battle_unit_idx].hits);

                BU_ProcessAttack__WIP(attacker_battle_unit_idx, (battle_units[attacker_battle_unit_idx].Cur_Figures - Feared_Figures), defender_battle_unit_idx, am_Melee, &Damage_Array[0], 0, SpFx);

                for(itr_damage_types = 0; itr_damage_types < 3; itr_damage_types++)
                {

                    defender_damage_array[itr_damage_types] += Damage_Array[itr_damage_types];

                }

                if((battle_units[attacker_battle_unit_idx].Combat_Effects & bue_Haste) != 0)
                {

                    BU_ProcessAttack__WIP(attacker_battle_unit_idx, (battle_units[attacker_battle_unit_idx].Cur_Figures - Feared_Figures), defender_battle_unit_idx, am_Melee, &Damage_Array[0], 0, SpFx);

                    for(itr_damage_types = 0; itr_damage_types < 3; itr_damage_types++)
                    {

                        defender_damage_array[itr_damage_types] += Damage_Array[itr_damage_types];

                    }

                }

                /*
                    END: Melee
                */

            }
            /*
                END: Attacker Non-First-Strike
            */

        }
        // else goto @@Done

    }
    else  /* (ranged_attack_flag == ST_TRUE) */
    {


        if(
            (battle_units[attacker_battle_unit_idx].ranged <= 0)
            ||
            (ranged_attack_check != 0)
        )
        {

            // ; subtract 5 from each type of damage in the target
            // ; damage array
            // ; BUG...?

            for(itr_damage_types = 0; itr_damage_types < 3; itr_damage_types++)
            {

                defender_damage_array[itr_damage_types] += -5;

            }

        }
        else
        {

            BU_ProcessAttack__WIP(attacker_battle_unit_idx, battle_units[attacker_battle_unit_idx].Cur_Figures, defender_battle_unit_idx, am_Ranged, &Damage_Array[0], 0, SpFx);

            for(itr_damage_types = 0; itr_damage_types < 3; itr_damage_types++)
            {

                defender_damage_array[itr_damage_types] += Damage_Array[itr_damage_types];

            }

        }

    }

}


// WZD o122p07
// drake178: BU_ProcessAttack()
/*
; processes a single melee-, ranged-, or counter attack
; made by the specified unit against a target, setting
; any damage done into the passed array pointer
;
; BUG: fails to apply ranged To Hit modifiers to short
;  range attacks
; BUG: Blur checks immunity on the wrong unit, and
;  is limited to half damage reduction
; BUG? the Power Drain item power is ignored here
*/
/*

Counter: {F,T}
SpFx:  {F,T}  as compiled, hard-coded to ST_TRUE

*/
void BU_ProcessAttack__WIP(int16_t attacker_battle_unit_idx, int16_t figure_count, int16_t defender_battle_unit_idx, int16_t attack_mode, int16_t damage_array[], int16_t Counter, int16_t SpFx)
{

    int16_t Save_Mod = 0;
    int16_t defender_enchantments = 0;
    // int16_t Reg_Dmg = 0;
    // int16_t Drain_Dmg = 0;
    // int16_t Irrev_Dmg = 0;
    int16_t new_damage_array[3] = { 0, 0, 0 };
    int16_t defender_front_figure_damage = 0;
    int16_t Target_Damage = 0;
    int16_t Target_To_Block = 0;
    int16_t Unused_Local = 0;
    int16_t To_Hit = 0;
    int16_t Imm_Flags = 0;
    int16_t Type_Specific_ATK_Flags = 0;
    int16_t Combined_ATK_Flags = 0;
    int16_t Attack_Strength = 0;
    int16_t Attack_Realm = 0;
    int16_t Attack_Type = 0;
    int16_t Target_Defense = 0;
    int16_t Range_Penalty = 0;
    int16_t Attack_Damage = 0;
    int16_t Blur_Loop_Var = 0;
    int16_t Loop_Var_2 = 0;
    int16_t itr = 0;

    defender_front_figure_damage = battle_units[defender_battle_unit_idx].TopFig_Dmg;

    defender_enchantments = (_UNITS[battle_units[defender_battle_unit_idx].unit_idx].enchantments | battle_units[defender_battle_unit_idx].enchantments);

    Unused_Local = 0;

    Attack_Strength = 0;

    Attack_Damage = 0;

    To_Hit = battle_units[attacker_battle_unit_idx].tohit;

    for(itr = 0; itr < 3; itr++)
    {

        damage_array[itr] = 0;

    }

    if(figure_count <= 0)
    {

        return;

    }

    Combined_ATK_Flags = battle_units[attacker_battle_unit_idx].Attack_Flags;

    Target_To_Block = battle_units[defender_battle_unit_idx].To_Block;

    // Imm_Flags = BU_GetATKImmFlags(attacker_battle_unit_idx, attack_mode);
    // ; returns the immunity flag bitfield that applies
    // ; against the selected unit's melee or ranged attacks
    // ;
    // ; BUG: fails to properly set Weapon Immunity for Thrown
    // ; attacks made with normal weapons
    /* HACK */  Imm_Flags = 0;

    if(attack_mode <= am_Melee)
    {

        Combined_ATK_Flags |= battle_units[attacker_battle_unit_idx].Melee_ATK_Flags;

        Type_Specific_ATK_Flags = battle_units[attacker_battle_unit_idx].Melee_ATK_Flags;

        // ; BUG: this already reduces the damage elsewhere
        To_Hit = (battle_units[attacker_battle_unit_idx].Melee_To_Hit - battle_units[defender_battle_unit_idx].To_Block);

        Attack_Strength = battle_units[attacker_battle_unit_idx].melee;

        // Attack_Realm = BU_GetATKRealm(0, attacker_battle_unit_idx);
        // ; returns the realm associated with the specified
        // ; attack type, or -1 if none

        Attack_Type = 0;  // Melee

    }
    else
    {

        Combined_ATK_Flags |= battle_units[attacker_battle_unit_idx].Ranged_ATK_Flags;

        Type_Specific_ATK_Flags = battle_units[attacker_battle_unit_idx].Ranged_ATK_Flags;

        // ; BUG: this already reduces the damage elsewhere
        To_Hit = (battle_units[attacker_battle_unit_idx].Melee_To_Hit - battle_units[defender_battle_unit_idx].To_Block);

        Attack_Strength = battle_units[attacker_battle_unit_idx].ranged;

        // Attack_Realm = BU_GetATKRealm(battle_units[attacker_battle_unit_idx].ranged_type, attacker_battle_unit_idx);
        // ; returns the realm associated with the specified
        // ; attack type, or -1 if none
        /* HACK */  Attack_Realm = ST_UNDEFINED;

        Attack_Type = battle_units[attacker_battle_unit_idx].ranged_type;

        if(Attack_Type == srat_Lightning)
        {

            Combined_ATK_Flags |= Att_ArmorPrc;

        }

        if(Attack_Type == srat_MultiGaze)
        {

            Combined_ATK_Flags |= Att_DoomDmg;

        }

        // ; BUG: fails to apply the proper To Hit modifiers to
        // ; Thrown and Breath attacks
        if(
            (attack_mode == 2)
            &&
            (Battle_Unit_Has_Ranged_Attack(attacker_battle_unit_idx) != ST_FALSE)
        )
        {

            To_Hit = battle_units[attacker_battle_unit_idx].Ranged_To_Hit;

            if((battle_units[attacker_battle_unit_idx].ranged_type / 10) != rag_Magic)
            {

                Range_Penalty = (Range_To_Battle_Unit(attacker_battle_unit_idx, defender_battle_unit_idx) / 3);

                if(
                    ((battle_units[attacker_battle_unit_idx].Abilities & UA_LONGRANGE) != 0)
                    &&
                    (Range_Penalty > 0)
                )
                {
                    Range_Penalty = 1;
                }

                To_Hit -= Range_Penalty;

            }

        }
        else
        {

            Attack_Strength = 0;

        }

    }

    for(itr = 0; itr < 3; itr++)
    {

        new_damage_array[itr] = 0;

    }

    if(
        (
            ((defender_enchantments & UE_INVISIBILITY) != 0)
            ||
            ((battle_units[defender_battle_unit_idx].Abilities & UA_INVISIBILITY) != 0)
        )
        &&
        ((battle_units[attacker_battle_unit_idx].Attribs_1 & USA_IMMUNITY_ILLUSION) == 0)
    )
    {

        To_Hit -= 1;

    }

    if(Counter == ST_TRUE)
    {

        To_Hit -= (battle_units[attacker_battle_unit_idx].Suppression / 2);

    }

    if(
        ((battle_units[defender_battle_unit_idx].Move_Flags & MV_FLYING) != 0)
        &&
        (attack_mode == am_Melee)
        &&
        ((battle_units[attacker_battle_unit_idx].Move_Flags & MV_FLYING) == 0)
        &&
        (Counter != ST_TRUE)
        &&
        (battle_units[attacker_battle_unit_idx].ranged_type < srat_Thrown)
    )
    {

        return;

    }


    if((battle_units[defender_battle_unit_idx].Attribs_1 & USA_IMMUNITY_MAGIC) == 0)
    {

        if((battle_units[attacker_battle_unit_idx].Attribs_2 & USA_IMMOLATION) != 0)
        {

            CMB_ConvSpellAttack(spl_Fireball, defender_battle_unit_idx, &new_damage_array[0], 4);

        }


        if(
            (
                (Attack_Type == srat_StoneGaze)
                ||
                (Attack_Type == srat_MultiGaze)
            )
            &&
            ((battle_units[defender_battle_unit_idx].Attribs_1 & USA_IMMUNITY_STONING) == 0)
        )
        {

            for(itr = 0; battle_units[defender_battle_unit_idx].Cur_Figures > itr; itr++)
            {

                // // IDGI  BU_ResistRoll(SPUSH@(battle_units[defender_battle_unit_idx]), -(abs(battle_units[attacker_battle_unit_idx].Spec_Att_Attrib)), 0);
                if(BU_ResistRoll__STUB(battle_units[defender_battle_unit_idx], -(abs(battle_units[attacker_battle_unit_idx].Spec_Att_Attrib)), sbr_Nature) > 0)
                {

                    new_damage_array[2] += battle_units[defender_battle_unit_idx].hits;

                }

            }

        }


        if(
            (
                (Attack_Type == srat_MultiGaze)
                ||
                (Attack_Type == srat_DeathGaze)
            )
            &&
            ((battle_units[defender_battle_unit_idx].Attribs_1 & USA_IMMUNITY_DEATH) == 0)
        )
        {

            for(itr = 0; battle_units[defender_battle_unit_idx].Cur_Figures > itr; itr++)
            {

                // // IDGI  BU_ResistRoll(SPUSH@(battle_units[defender_battle_unit_idx]), -(abs(battle_units[attacker_battle_unit_idx].Spec_Att_Attrib)), sbr_Death);
                if(BU_ResistRoll__STUB(battle_units[defender_battle_unit_idx], -(abs(battle_units[attacker_battle_unit_idx].Spec_Att_Attrib)), sbr_Death) > 0)
                {

                    new_damage_array[0] += battle_units[defender_battle_unit_idx].hits;

                }

            }

        }

    }


    if((battle_units[defender_battle_unit_idx].Combat_Effects & bue_Black_Sleep) != 0)
    {

        Combined_ATK_Flags |= Att_DoomDmg;

    }


    if(Attack_Strength <= 0)
    {

        return;

    }


    Target_Defense = BU_GetEffectiveDEF__SEGRAX(defender_battle_unit_idx, Attack_Type, Imm_Flags, Combined_ATK_Flags, Attack_Realm);


    if((Combined_ATK_Flags & Att_EldrWeap) != 0)
    {

        Target_To_Block -= 1;

    }


    if(
        (BU_IsInCityProper__STUB(defender_battle_unit_idx) == ST_TRUE)
        &&
        (BU_IsInCityProper__STUB(attacker_battle_unit_idx) == ST_FALSE)
        &&
        (battlefield->Walled_City != 0)
    )
    {

        if(Combat_Grid_Cell_Has_City_Wall(battle_units[defender_battle_unit_idx].cgx, battle_units[defender_battle_unit_idx].cgy) != ST_FALSE)
        {

            Target_Defense += 3;

        }
        else
        {

            Target_Defense += 1;

        }


    }


    for(itr = 0; itr < figure_count; itr++)
    {


        if((battle_units[defender_battle_unit_idx].Attribs_1 & USA_IMMUNITY_MAGIC) == 0)
        {

            // process the Dispel Evil touch attack if applicable
            // INCONSISTENT: Spell Lock does not protect from this,
            // even though it does from the spell versions
            if(
                ((Combined_ATK_Flags & Att_DsplEvil) != 0)
                &&
                (
                    (battle_units[defender_battle_unit_idx].race == rt_Chaos)
                    ||
                    (battle_units[defender_battle_unit_idx].race == rt_Death)
                )
            )
            {

                Save_Mod = -4;

                if((_UNITS[battle_units[defender_battle_unit_idx].unit_idx].mutations & UM_UNDEAD) != 0)
                {
                    Save_Mod -= 5;
                }

                if(BU_ResistRoll__STUB(battle_units[defender_battle_unit_idx], Save_Mod, sbr_Life) > 0)
                {

                    new_damage_array[2] += battle_units[defender_battle_unit_idx].hits;

                }

            }


            if(
                ((Combined_ATK_Flags & Att_StnTouch) != 0)
                &&
                ((battle_units[defender_battle_unit_idx].Attribs_1 & USA_IMMUNITY_STONING) == 0)
            )
            {

                Save_Mod = -(abs(battle_units[attacker_battle_unit_idx].Spec_Att_Attrib));

                if((Type_Specific_ATK_Flags & Att_StnTouch) != 0)
                {

                    Save_Mod -= 1;

                }

                if(BU_ResistRoll__STUB(battle_units[defender_battle_unit_idx], Save_Mod, sbr_Nature) > 0)
                {

                    new_damage_array[2] += battle_units[defender_battle_unit_idx].hits;

                }

            }


            if(
                ((Combined_ATK_Flags & Att_DthTouch) != 0)
                &&
                ((battle_units[defender_battle_unit_idx].Attribs_1 & USA_IMMUNITY_DEATH) == 0)
            )
            {

                Save_Mod = -(abs(battle_units[attacker_battle_unit_idx].Spec_Att_Attrib));

                if((Type_Specific_ATK_Flags & Att_DthTouch) != 0)
                {

                    Save_Mod -= 3;

                }

                if(BU_ResistRoll__STUB(battle_units[defender_battle_unit_idx], Save_Mod, sbr_Death) > 0)
                {

                    new_damage_array[0] += battle_units[defender_battle_unit_idx].hits;

                }

            }


            if(
                ((Combined_ATK_Flags & Att_LifeSteal) != 0)
                &&
                ((battle_units[defender_battle_unit_idx].Attribs_1 & USA_IMMUNITY_DEATH) == 0)
            )
            {

                Save_Mod = -(abs(battle_units[attacker_battle_unit_idx].Spec_Att_Attrib));

                if((Type_Specific_ATK_Flags & Att_LifeSteal) != 0)
                {

                    Save_Mod -= 3;

                }

                Loop_Var_2 = BU_ResistRoll__STUB(battle_units[defender_battle_unit_idx], Save_Mod, sbr_Death);

                new_damage_array[1] += Loop_Var_2;

                if(SpFx != 0)
                {

                    Battle_Unit_Heal(attacker_battle_unit_idx, Loop_Var_2, 1);

                }

            }


            if((Combined_ATK_Flags & Att_Destruct) != 0)
            {

                if(BU_ResistRoll__STUB(battle_units[defender_battle_unit_idx], 0, sbr_Death) > 0)
                {

                    new_damage_array[2] += battle_units[defender_battle_unit_idx].hits;

                }

            }

        }


        if((Combined_ATK_Flags & Att_DoomDmg) != 0)
        {

            if((Type_Specific_ATK_Flags & Att_DoomDmg) != 0)
            {

                Attack_Damage = Attack_Strength;

            }
            else
            {

                Attack_Damage = (Attack_Strength / 2);

            }

        }
        else
        {

            Attack_Damage = CMB_AttackRoll__SEGRAX(Attack_Strength, To_Hit);

            if(
                (
                    (battle_units[defender_battle_unit_idx].controller_idx == _combat_attacker_player)
                    &&
                    (combat_enchantments[BLUR_ATTKR] > 0)
                    &&
                    ((battle_units[defender_battle_unit_idx].Attribs_1 & USA_IMMUNITY_ILLUSION) == 0)  // ; BUG: this is not the attacking unit's index
                )
                ||
                (
                    (battle_units[defender_battle_unit_idx].controller_idx == _combat_defender_player)
                    &&
                    (combat_enchantments[BLUR_DFNDR]> 0)
                    &&
                    ((battle_units[defender_battle_unit_idx].Attribs_1 & USA_IMMUNITY_ILLUSION) == 0)  // ; BUG: this is not the attacking unit's index
                )
            )
            {

                for(Blur_Loop_Var = 0; Blur_Loop_Var < Attack_Damage; Blur_Loop_Var++)
                {

                    if(Random(10) == 10)
                    {

                        // ; BUG: this also alters the loop condition, which
                        // ; limits the total reduction to at most 50%
                        Attack_Damage -= 1;

                    }

                }

            }


            // attack units minus defense units
            Attack_Damage -= CMB_DefenseRoll__SEGRAX(Target_Defense, Target_To_Block);


            if((defender_enchantments & UE_INVULNERABILITY) != 0)
            {

                Attack_Damage -= 2;

            }

            SETMIN(Attack_Damage, 0);

        }


        // ; process Poison Touch, if applicable
        if(
            ((battle_units[attacker_battle_unit_idx].Attack_Flags & Att_Poison) != 0)
            &&
            ((battle_units[defender_battle_unit_idx].Attribs_1 & USA_IMMUNITY_POISON) == 0)
        )
        {

            for(Loop_Var_2 = 0; battle_units[attacker_battle_unit_idx].Poison_Strength > Loop_Var_2; Loop_Var_2++)
            {

                if(BU_ResistRoll__STUB(battle_units[defender_battle_unit_idx], 0, -1) != 0)
                {

                    if(
                        ((battle_units[attacker_battle_unit_idx].Abilities & UA_CREATEUNDEAD) != 0)
                        &&
                        ((battle_units[defender_battle_unit_idx].Attribs_1 & USA_IMMUNITY_MAGIC) == 0)
                    )
                    {

                        new_damage_array[1] += 1;

                    }
                    else
                    {

                        new_damage_array[0] += 1;

                    }

                }

            }

        }


        // ; sum up the damage done so far, and add to it the
        // ; target's original top figure damage

        Target_Damage = 0;

        for(Loop_Var_2 = 0; Loop_Var_2 < 3; Loop_Var_2++)
        {

            Target_Damage += new_damage_array[Loop_Var_2];

        }

        Target_Damage += defender_front_figure_damage;

        // ; process the actual attack damage directly into the
        // ; return array, making extra defense rolls when
        // ; necessary

        while (battle_units[defender_battle_unit_idx].hits > (Attack_Damage + Target_Damage))
        {

            Attack_Damage -= battle_units[defender_battle_unit_idx].hits;

            if(Attack_Damage < 0)
            {

// went negative
// damage < hit - pooints
// alive

                // ; no additional defense rolls required - transfer the
                // ; attack damage to the total, and add it to the return
                // ; array

                if (
                    ((battle_units[attacker_battle_unit_idx].Abilities & UA_CREATEUNDEAD) != 0)
                    &&
                    ((battle_units[defender_battle_unit_idx].Attribs_1 & USA_IMMUNITY_MAGIC) == 0)
                    )
                {

                    damage_array[1] += (battle_units[defender_battle_unit_idx].hits + Attack_Damage);

                }
                else
                {

                    damage_array[0] += (battle_units[defender_battle_unit_idx].hits + Attack_Damage);

                }

                Target_Damage += Attack_Damage;

                Attack_Damage = 0;

            }
            else
            {

// didn't go negative
// damage >= hit - pooints
// dead

                // ; figure overkill - transfer Hits worth of damage from
                // ; the attack damage to the return array, then make a
                // ; new defense roll to reduce the remainder

                if (
                    ((battle_units[attacker_battle_unit_idx].Abilities & UA_CREATEUNDEAD) != 0)
                    &&
                    ((battle_units[defender_battle_unit_idx].Attribs_1 & USA_IMMUNITY_MAGIC) == 0)
                    )
                {

                    damage_array[1] += battle_units[defender_battle_unit_idx].hits;

                }
                else
                {

                    damage_array[0] += battle_units[defender_battle_unit_idx].hits;

                }

                if ((Combined_ATK_Flags & Att_DoomDmg) == 0)
                {

                    Attack_Damage = CMB_DefenseRoll__SEGRAX(Target_Defense, Target_To_Block);

                    if ((defender_enchantments & UE_INVULNERABILITY) != 0)
                    {

                        Attack_Damage -= 2;

                    }

                    SETMIN(Attack_Damage, 0);

                }

            }


        }  /* while(battle_units[defender_battle_unit_idx].hits > (Attack_Damage + Target_Damage)) */

        if(
            ((battle_units[attacker_battle_unit_idx].Abilities & UA_CREATEUNDEAD) != 0)
            &&
            ((battle_units[defender_battle_unit_idx].Attribs_1 & USA_IMMUNITY_MAGIC) == 0)
        )
        {

            damage_array[1] += Attack_Damage;

        }
        else
        {

            damage_array[0] += Attack_Damage;

        }

        defender_front_figure_damage = (Target_Damage + Attack_Damage);

        Attack_Damage = 0;

        Unused_Local = 0;

        for(Loop_Var_2 = 0; Loop_Var_2 < 3; Loop_Var_2++)
        {

            damage_array[Loop_Var_2] += new_damage_array[Loop_Var_2];

            new_damage_array[Loop_Var_2] = 0;

        }

    }  /* END: for(itr = 0; itr < figure_count; itr++) */

}


// WZD o122p08
// Segrax
// drake178: BU_GetEffectiveDEF()
/*
; calculates and returns the unit's effective Defense
; score based on immunities and other conditional
; modifiers (except City Walls and Doom Damage), attack_mode
; only matters if it is melee (value of 0)
*/
/*



*/
int16_t BU_GetEffectiveDEF__SEGRAX(int16_t battle_unit_idx, int16_t attack_type, int16_t Imm_Flags, int16_t ATK_Flags, int16_t Realm)
{
    int16_t Immunity_Type = 0;
    int16_t IDK_effective_defense = 0;  // _SI_

    struct s_BATTLE_UNIT* battleunit = &battle_units[battle_unit_idx];
    struct s_UNIT* unit = &_UNITS[battleunit->unit_idx];

    int32_t Enchants = unit->enchantments | battleunit->enchantments | battleunit->item_enchantments;

    IDK_effective_defense = battleunit->defense;

    if (Imm_Flags & battleunit->Attribs_1 & USA_IMMUNITY_ILLUSION) {
        Imm_Flags ^= USA_IMMUNITY_ILLUSION;
    }

    if (Imm_Flags & USA_IMMUNITY_ILLUSION) {
        return 0;
    }

    if ((battleunit->Abilities & UA_LARGESHIELD) && attack_type != am_Melee) {
        IDK_effective_defense += 2;
    }

    if (Imm_Flags & battleunit->Attribs_1) {
        if (attack_type != am_Melee) {
            Immunity_Type = 2;
        }
    }

    if (Imm_Flags & battleunit->Attribs_1 & USA_IMMUNITY_WEAPON) {
        Immunity_Type = 1;
    }

    if ((battleunit->Attribs_1 & USA_IMMUNITY_MAGIC) && Realm != ST_UNDEFINED && attack_type != 0) {
        Immunity_Type = 2;
    }

    if ((Realm == sbr_Chaos || Realm == sbr_Death) && (Enchants & UE_RESISTELEMENTS)) {
        IDK_effective_defense += 3;
    }

    if ((Realm == sbr_Chaos || Realm == sbr_Nature) && (Enchants & UE_ELEMENTALARMOR)) {
        IDK_effective_defense += 10;
    }
    else if ((Enchants & UE_RESISTELEMENTS)) {
        IDK_effective_defense += 3;
    }

    if (ATK_Flags & Att_ArmorPrc) {
        IDK_effective_defense -= IDK_effective_defense >> 1;
    }

    if (Immunity_Type == 1 && IDK_effective_defense < 10) {
        IDK_effective_defense = 10;
    }

    if (Immunity_Type == 2) {
        return 50;
    }

    return IDK_effective_defense;

}

// WZD o122p09
// BU_GetATKRealm()

// WZD o122p10
/*
    ¿ only applies to city combat ?
    ~== MoO Combat at Colony, where Colony is owned by Defender



*/
/*
; calculates and stores the highest unit aura values
; for each player (Holy Bonus, Resistance to All, and
; Leadership), and applies Heavenly Light, Cloud of
; Shadow, and Eternal Night
;
; BUG: grants +3 Defense to defending units in battle
; condition 1 (else wouldn't even need this argument)
; Attributes: bp-based frame
*/
// ¿ MoO2  Module: COMBINIT  Load_Colony_Defense_() |-> Load_Tactical_Colony_() ... Load_Combat_Satelite_()
/*
    combat_enchantments, CMB_HolyBonusArray, CMB_ResistAllArray, CMB_LeadershipArray, battle_units[].defense
    combat_enchantments - Darkness, True_Light
    +3 Defender defense, if combat_structure == 1
    Hero - Prayer Master
    Hero - Leadership
    CMB_ResistAllArray += CMB_HolyBonusArray
*/
void Init_Battlefield_Effects(int16_t combat_structure)
{
    int16_t Leadership_Value;
    int16_t Prayermaster_Bonus;
    int16_t itr_players;  // _SI_
    int16_t itr_arrays;  // _SI_
    int16_t itr_battle_units;  // _SI_
    int16_t battle_unit_owner_idx;  // _DI_


    // WTFmate:  Per the Dasm, battlefield is not even allocated for Strategic_Combat()  (and, I can't find where it's populated, in either case)

    if(battlefield->city_enchantments[CLOUD_OF_SHADOW] > 0)
    {

        combat_enchantments[DARKNESS_DFNDR] = 2;

    }

    if(battlefield->city_enchantments[HEAVENLY_LIGHT] > 0)
    {

        combat_enchantments[TRUE_LIGHT_DFNDR] = 2;

    }

    /*
        if any player has 'Eternal Night'
        and the defender does not have 'True Light' or its city does not have 'Heavenly Light'
        then
        if that player is the attacker, the attacker has 'Darkness'
        if that player is the defender, the defender has 'Darkness'
        if that player is neither, then both have 'Darkness'
    */
    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {

        if(_players[itr_players].Globals[ETERNAL_NIGHT] > 0)
        {
            // if(combat_enchantments->True_Light.Dfndr != 2)
            if(combat_enchantments[TRUE_LIGHT_DFNDR] != 2)
            {

                if(itr_players == _combat_attacker_player)
                {

                    // combat_enchantments->Darkness.Attkr = 3;
                    combat_enchantments[DARKNESS_ATTKR] = 3;

                }
                else if(itr_players == _combat_defender_player)
                {

                    // combat_enchantments->Darkness.Dfndr = 3;
                    combat_enchantments[DARKNESS_DFNDR] = 3;

                }
                else
                {
                    
                    // combat_enchantments->Darkness.Dfndr = 3;
                    // combat_enchantments->Darkness.Attkr = 3;
                    combat_enchantments[DARKNESS_DFNDR] = 3;
                    combat_enchantments[DARKNESS_ATTKR] = 3;

                }

            }

        }

    }

    for(itr_arrays = 0; itr_arrays < 7; itr_arrays++)
    {
        CMB_HolyBonusArray[itr_arrays] = 0;
        CMB_ResistAllArray[itr_arrays] = 0;
        CMB_LeadershipArray[itr_arrays] = 0;
    }


    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
    {
        Leadership_Value = 0;

        if(battle_units[itr_battle_units].status == 0) /* Unit_Active */
        {

            // BUGBUG: should be checking for City Wall
            if(
                (combat_structure = cs_City)  /* probably just {F,T} */
                &&
                (battle_units[itr_battle_units].controller_idx == _combat_defender_player)
            )
            {
                battle_units[itr_battle_units].defense += 3;
            }

            battle_unit_owner_idx = battle_units[itr_battle_units].controller_idx;

            if((battle_units[itr_battle_units].Attribs_2 & USA_HOLYBONUS) !=0)
            {
                // DEDU  what is it testing/setting here? what's in Spec_Att_Attrib, how'd it get there - memcpy?
                if(battle_units[itr_battle_units].Spec_Att_Attrib > CMB_HolyBonusArray[battle_unit_owner_idx])
                {
                    CMB_HolyBonusArray[battle_unit_owner_idx] = battle_units[itr_battle_units].Spec_Att_Attrib;
                }
            }

            if((battle_units[itr_battle_units].Attribs_2 & USA_RESISTALL) != 0)
            {
                // DEDU  what is it testing/setting here? what's in Spec_Att_Attrib, how'd it get there - memcpy?
                if(battle_units[itr_battle_units].Spec_Att_Attrib > CMB_ResistAllArray[battle_unit_owner_idx])
                {
                    CMB_ResistAllArray[battle_unit_owner_idx] = battle_units[itr_battle_units].Spec_Att_Attrib;
                }
            }

            if(_UNITS[battle_units[itr_battle_units].unit_idx].Hero_Slot >= 0)
            {

                // Prayermaster
                if(HERO_PRAYERMASTER(battle_unit_owner_idx, _UNITS[battle_units[itr_battle_units].unit_idx].type))
                {
                    Prayermaster_Bonus = (_UNITS[battle_units[itr_battle_units].unit_idx].Level + 1);
                    if(Prayermaster_Bonus > CMB_ResistAllArray[battle_unit_owner_idx])
                    {
                        CMB_ResistAllArray[battle_unit_owner_idx] = Prayermaster_Bonus;
                    }
                }
                if(HERO_PRAYERMASTER2(battle_unit_owner_idx, _UNITS[battle_units[itr_battle_units].unit_idx].type))
                {
                    Prayermaster_Bonus = (((_UNITS[battle_units[itr_battle_units].unit_idx].Level + 1) * 3) / 2);
                    if(Prayermaster_Bonus > CMB_ResistAllArray[battle_unit_owner_idx])
                    {
                        CMB_ResistAllArray[battle_unit_owner_idx] = Prayermaster_Bonus;
                    }
                }

                // Leadership
                if(HERO_LEADERSHIP(battle_unit_owner_idx, _UNITS[battle_units[itr_battle_units].unit_idx].type))
                {
                    Leadership_Value = ((_UNITS[battle_units[itr_battle_units].unit_idx].Level + 1) / 3);
                    if(Leadership_Value > CMB_LeadershipArray[battle_unit_owner_idx])
                    {
                        CMB_LeadershipArray[battle_unit_owner_idx] = Leadership_Value;
                    }
                }
                if(HERO_LEADERSHIP2(battle_unit_owner_idx, _UNITS[battle_units[itr_battle_units].unit_idx].type))
                {
                    Leadership_Value = ((_UNITS[battle_units[itr_battle_units].unit_idx].Level + 1) / 2);
                    if(Leadership_Value > CMB_LeadershipArray[battle_unit_owner_idx])
                    {
                        CMB_LeadershipArray[battle_unit_owner_idx] = Leadership_Value;
                    }
                }

            }

        }
        
    }


    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        CMB_ResistAllArray[itr_players] += CMB_HolyBonusArray[itr_players];
    }

}


// WZD o122p11
// drake178: BU_Attack()
/*
; processes a full attack action by the chosen unit
; against the specified target unit or wall tile
;
; BUG: fails to properly expend the moves of units
;  performing ranged attacks
; INCONSISTENT in its decision-making about whether to
;  melee or shoot at an adjacent target
*/
/*


Battle_Unit_Action__WIP()
    |-> BU_Attack__WIP(battle_unit_idx, combat_grid_target, Target_X, Target_Y);

*/
void Battle_Unit_Attack__WIP(int16_t attacker_battle_unit_idx, int16_t defender_battle_unit_idx, int16_t cgx, int16_t cgy)
{
    int16_t Wall_Destroyed = 0;
    int16_t ranged_attack_check = 0;
    int16_t range_to_target = 0;
    int16_t ranged_attack_flag = 0;  // DNE in Dasm, reuses range_to_target
    int16_t attacker_damage_array[3] = { 0, 0, 0 };
    int16_t defender_damage_array[3] = { 0, 0, 0 };
    int16_t defender_damage_total = 0;
    int16_t attacker_damage_total = 0;
    int16_t itr_damage_type = 0;

    if(battle_units[attacker_battle_unit_idx].controller_idx != HUMAN_PLAYER_IDX)
    {

        AI_ImmobileCounter = ST_UNDEFINED;

    }

    if(defender_battle_unit_idx != 99)
    {

        cgx = battle_units[defender_battle_unit_idx].cgx;

        cgy = battle_units[defender_battle_unit_idx].cgy;

        // {0: allow; 1: disallow (Invisibility); 2: disallow (Wall of Darkness)}
        ranged_attack_check = Check_Attack_Ranged(attacker_battle_unit_idx, defender_battle_unit_idx);

        range_to_target = Range_To_Battle_Unit(attacker_battle_unit_idx, defender_battle_unit_idx);

        if(range_to_target <= 1)
        {

            // ; these checks are in the wrong logical order
            if(
                (battle_units[attacker_battle_unit_idx].ranged > (battle_units[attacker_battle_unit_idx].melee / 2))
                &&
                (Battle_Unit_Has_Ranged_Attack(attacker_battle_unit_idx) != ST_FALSE)
            )
            {

                if(
                    (
                        ((battle_units[attacker_battle_unit_idx].ranged_type / 10) == rag_Missile)
                        &&
                        ((battle_units[defender_battle_unit_idx].Attribs_1 & USA_IMMUNITY_MISSILES) != 0)
                    )
                    ||
                    (
                        ((battle_units[attacker_battle_unit_idx].ranged_type / 10) == rag_Magic)
                        &&
                        ((battle_units[defender_battle_unit_idx].Attribs_1 & USA_IMMUNITY_MAGIC) != 0)  // ; BUG: ignores Righteousness
                    )
                )
                {

                    ranged_attack_flag = ST_FALSE;

                }
                else
                {

                    ranged_attack_flag = ST_TRUE;

                }

            }
            else
            {

                ranged_attack_flag = ST_FALSE;

            }

        }
        else  /* (range_to_target > 1) */
        {

            if(ranged_attack_check == 0)
            {

                ranged_attack_flag = ST_TRUE;

            }
            else if(ranged_attack_check == 1)
            {

                if(
                    (battle_units[attacker_battle_unit_idx].controller_idx == HUMAN_PLAYER_IDX)
                    &&
                    (_auto_combat_flag == ST_FALSE)
                )
                {

                    LBX_Load_Data_Static(message_lbx_file__ovr122, 0, (SAMB_ptr)GUI_NearMsgString, 3, 1, 150);
                    
                    Warn1(GUI_NearMsgString);

                    return;

                }

            }
            else if(ranged_attack_check == 2)
            {
                
                if(
                    (battle_units[attacker_battle_unit_idx].controller_idx == HUMAN_PLAYER_IDX)
                    &&
                    (_auto_combat_flag == ST_FALSE)
                )
                {

                    LBX_Load_Data_Static(message_lbx_file__ovr122, 0, (SAMB_ptr)GUI_NearMsgString, 4, 1, 150);
                    
                    Warn1(GUI_NearMsgString);

                    return;

                }
                
            }

        }


        if(ranged_attack_check > 0)
        {

            ranged_attack_flag = ST_FALSE;

        }


        battle_units[attacker_battle_unit_idx].target_cgx = battle_units[defender_battle_unit_idx].cgx;

        battle_units[attacker_battle_unit_idx].target_cgy = battle_units[defender_battle_unit_idx].cgy;


        BU_AttackTarget__WIP(attacker_battle_unit_idx, defender_battle_unit_idx, &defender_damage_array[0], &attacker_damage_array[0], ranged_attack_flag, 1);


        battle_units[defender_battle_unit_idx].Suppression += 1;

        attacker_damage_total = 0;

        defender_damage_total = 0;

        for(itr_damage_type = 0; itr_damage_type < 3; itr_damage_type++)
        {

            attacker_damage_total += attacker_damage_array[itr_damage_type];

            defender_damage_total += defender_damage_array[itr_damage_type];

        }

        if(ranged_attack_flag != ST_TRUE)
        {

            CMB_MeleeAnim__STUB(attacker_battle_unit_idx, defender_battle_unit_idx, attacker_damage_total, defender_damage_total, battle_units[defender_battle_unit_idx].cgx, battle_units[defender_battle_unit_idx].cgy);

            battle_units[attacker_battle_unit_idx].movement_points -= ((Battle_Unit_Moves2(attacker_battle_unit_idx) + 1) / 2);

            SETMIN(battle_units[attacker_battle_unit_idx].movement_points, 0);

        }
        else  /* (ranged_attack_flag == ST_TRUE) */
        {

            CMB_RangedAnim__STUB(attacker_battle_unit_idx, defender_battle_unit_idx, defender_damage_total, cgx, cgy);

            // ; BUG: performing a ranged attack is supposed to
            // ; always end the unit's turn
            battle_units[attacker_battle_unit_idx].movement_points -= 20;

            SETMIN(battle_units[attacker_battle_unit_idx].movement_points, 0);

            if(
                (
                    (_UNITS[battle_units[attacker_battle_unit_idx].unit_idx].Hero_Slot > -1)
                    ||
                    ((battle_units[attacker_battle_unit_idx].Attribs_1 & (USA_CASTER_20 | USA_CASTER_40)) != 0)
                )
                &&
                ((battle_units[attacker_battle_unit_idx].ranged_type / 10) == rag_Magic)
            )
            {

                if(battle_units[attacker_battle_unit_idx].mana > 3)
                {

                    battle_units[attacker_battle_unit_idx].mana -= 3;

                }
                else
                {

                    // NEWBUG  battle_units[attacker_battle_unit_idx].mana;

                }

                if(battle_units[attacker_battle_unit_idx].mana < 3)
                {

                    battle_units[attacker_battle_unit_idx].ranged_type = rat_None;

                    battle_units[attacker_battle_unit_idx].ranged = 0;

                }

            }
            else
            {

                battle_units[attacker_battle_unit_idx].ammo -= 1;

                if(battle_units[attacker_battle_unit_idx].ammo <= 0)
                {

                    battle_units[attacker_battle_unit_idx].ranged_type = rat_None;

                    battle_units[attacker_battle_unit_idx].ranged = 0;

                }

            }

        }



        BU_ApplyDamage__WIP(defender_battle_unit_idx, defender_damage_array);

        BU_ApplyDamage__WIP(attacker_battle_unit_idx, attacker_damage_array);

    }


    // @@Wall_Crusher:
    Wall_Destroyed = ST_FALSE;


    if(
        ((battle_units[attacker_battle_unit_idx].Abilities & UA_WALLCRUSHER) != 0)
        &&
        (battle_units[attacker_battle_unit_idx].controller_idx != _combat_defender_player)
        &&
        (Combat_Grid_Cell_Has_City_Wall(cgx, cgy) != 0)
    )
    {

        if(defender_battle_unit_idx != 99)
        {

            if(
                (abs(battle_units[attacker_battle_unit_idx].cgx - cgx) > 1)
                ||
                (abs(battle_units[attacker_battle_unit_idx].cgy - cgy) > 1)
            )
            {

                // ; 25% probability if ranged attack
                if(Random(100) <= 25)
                {
                    Wall_Destroyed = ST_TRUE;
                }

            }
            else
            {

                // ; 50% probability if melee attack
                if(Random(100) <= 50)
                {
                    Wall_Destroyed = ST_TRUE;
                }

            }

        }
        else  /* (defender_battle_unit_idx == 99) */
        {

            if(
                (abs(battle_units[attacker_battle_unit_idx].cgx - cgx) > 1)
                ||
                (abs(battle_units[attacker_battle_unit_idx].cgy - cgy) > 1)
            )
            {

                // ; 25% probability if ranged attack
                if(Random(100) <= 25)
                {
                    Wall_Destroyed = ST_TRUE;
                }

                // perform a ranged attack animation, expend movement,
                // and expend the attacker's ammo or mana as
                // appropriate, removing their ranged attack if they
                // don't have enough left to perform another one
                // BUG: only expends 10 movement points instead of all
                // of them, as stated in the v1.2 patch notes

                CMB_RangedAnim__STUB(attacker_battle_unit_idx, defender_battle_unit_idx, defender_damage_total, cgx, cgy);

                // ; BUG: performing a ranged attack is supposed to
                // ; always end the unit's turn
                battle_units[attacker_battle_unit_idx].movement_points -= 20;

                if(
                    (_UNITS[battle_units[attacker_battle_unit_idx].unit_idx].Hero_Slot > -1)
                    &&
                    ((battle_units[attacker_battle_unit_idx].ranged_type / 10) == rag_Magic)
                )
                {

                    if(battle_units[attacker_battle_unit_idx].mana > 3)
                    {

                        battle_units[attacker_battle_unit_idx].mana -= 3;

                    }
                    else
                    {

                        // NEWBUG  battle_units[attacker_battle_unit_idx].mana;

                    }

                    if(battle_units[attacker_battle_unit_idx].mana < 5)
                    {

                        battle_units[attacker_battle_unit_idx].ranged_type = rat_None;

                        battle_units[attacker_battle_unit_idx].ranged = 0;

                    }

                }
                else
                {

                    battle_units[attacker_battle_unit_idx].ammo -= 1;

                    if(battle_units[attacker_battle_unit_idx].ammo <= 0)
                    {

                        battle_units[attacker_battle_unit_idx].ranged_type = rat_None;

                        battle_units[attacker_battle_unit_idx].ranged = 0;

                    }

                }

            }
            else
            {

                // ; 50% probability if melee attack
                if(Random(100) <= 50)
                {
                    Wall_Destroyed = ST_TRUE;
                }

                CMB_MeleeAnim__STUB(attacker_battle_unit_idx, defender_battle_unit_idx, attacker_damage_total, defender_damage_total, cgx, cgy);

                battle_units[attacker_battle_unit_idx].movement_points -= ((Battle_Unit_Moves2(attacker_battle_unit_idx) + 1) / 2);

            }

        }

    }


    // @@JmpDone_DrawFlip:
    Set_Page_Off();
    Tactical_Combat_Draw();
    PageFlip_FX();

}


// WZD o122p12
// drake178: BU_GetDistanceFrom()
/*
; returns the simple distance (largest of X and Y)
; between the two specified battle units
*/
/*

*/
int16_t Range_To_Battle_Unit(int16_t BU_1, int16_t BU_2)
{
    int16_t Y_Distance = 0;
    int16_t X_Distance = 0;
    int16_t distance = 0;  // DNE in Dasm


    X_Distance = abs(battle_units[BU_1].cgx - battle_units[BU_2].cgx);

    Y_Distance = abs(battle_units[BU_1].cgy - battle_units[BU_2].cgy);


    if(X_Distance > Y_Distance)
    {

        distance = Y_Distance;

    }
    else
    {

        distance = X_Distance;

    }


    return distance;

}


// WZD o122p13
// drake178: AI_BU_GetAttackValue()
/*
; returns the target value for attacking the specified
; unit, or -200 if it can't be attacked due to flight
;
; base: own ATK - enemy DEF, (-10 if confused), (+3 if
;  attacking spirit in a Node), (-10 if own DEF/2<enemy
;  ATK), (+10 if current target)
; melee only: +5 if gatekeeper, -20 if within walls,
;  -20 if requires damage from Wall of Fire, +25 if
;  dist <= moves (with +20 more if sleeping)
; ranged only: -(enemy ranged ATK/3+2), +20 if sleeping
;
; WARNING: ignores short-range attacks on melee
; WARNING: can return the negative base value even if
;  an attack would be valid (e.g. 3 ATK vs 4 DEF)
*/
/*

*/
int16_t AI_BU_GetAttackValue__STUB(int16_t battle_unit_idx, int16_t target_battle_unit_idx, int16_t has_ranged_attack)
{

    return Random(200);

}


// WZD o122p14
// drake178: ¿ ?
/*
; returns 1 if the unit has a proper (not short-)
; ranged attack, or 0 otherwise
*/
/*

*/
int16_t Battle_Unit_Has_Ranged_Attack(int16_t battle_unit_idx)
{
    int16_t is_ranged = 0;  // _SI_

    is_ranged = ST_FALSE;

    if(
        (battle_units[battle_unit_idx].ranged_type > rat_None)
        &&
        (battle_units[battle_unit_idx].ranged_type < srat_Thrown)
    )
    {
        is_ranged = ST_TRUE;
    }

    return is_ranged;
}


// WZD o122p15
// drake178: BU_CauseFear()
/*
; processes the Cause Fear effect if applicable,
; returning the amount of attacking figures that have
; failed their resistance checks
*/
/*



*/
int16_t BU_CauseFear__NOOP(int16_t attacker_battle_unit_idx, int16_t defender_battle_unit_idx)
{

    return 0;

}



/*

    WIZARDS.EXE ovr123

*/

// WZD o123p01
// MoO2  Module: COMBINIT  End_Of_Combat_()
/*
    "WIP", cause CMB_CreateUndeadAnim(ut_Zombies) and CMB_CreateUndeadAnim(CMB_GetUndeadCreator(player_idx))

    // ; finishes the active combat, processing all related
    // ; events and effects - including the display of the
    // ; summary scroll and undead creation animations if applicable
    End_Of_Combat(winner_player_idx, item_count, item_list, MsgType);

Diplomatic_Value:
    starts at 0
    adds 20 for Hero Unit
    or Random(20) for Non-Hero Unit and Random(10) for Engineer or Settler
    passes negated value to G_DIPL_Action()
*/
void End_Of_Combat__WIP(int16_t player_idx, int16_t * item_count, int16_t item_list[], int16_t MsgType)
{
    int16_t Buildings_Lost[36] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Rare_Foe_Defeated = 0;
    int16_t Diplomatic_Value = 0;
    int16_t Summoned_Unit = 0;
    int16_t Population_Loss_Percent = 0;
    int16_t Destruction_Chance = 0;
    int16_t No_Secondaries = 0;
    int16_t BU_CombatHits = 0;
    char temp_buffer[LEN_TEMP_BUFFER] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Undead_Created = 0;
    int16_t Zombies_Raised = 0;
    int16_t Experience_Gained = 0;
    int16_t Surviving_Unit_Count = 0;
    int16_t battle_unit_owner_idx = 0;
    uint32_t enchantments = 0;
    int16_t itr_battle_units = 0;  // _SI_
    int16_t RazeCity = 0;  // _SI_
    int16_t itr_buildings = 0;  // _SI_
    int16_t itr_population = 0;  // _SI_
    int16_t itr_bldg_msg = 0;  // _SI_
    int16_t IDK_population_lost = 0;  // _DI_
    int16_t bldg_msg_idx = 0;  // _DI_
    int16_t itr_hero_items = 0;  // _DI_

    Rare_Foe_Defeated = 0;

    CMB_ScrollMsg_Type = MsgType;

    CMB_Population_Lost = 0;
    CMB_Buildings_Lost = 0;
    CMB_Gold_Reward = 0;

    // ~ Monsters
    if((player_idx < HUMAN_PLAYER_IDX) || (player_idx > NEUTRAL_PLAYER_IDX))
    {
        for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
        {
            if(battle_units[itr_battle_units].controller_idx == player_idx)
            {
                battle_units[itr_battle_units].status = bus_Gone;
            }
        }
    }

    *item_count = 0;

    Experience_Gained = 0;
    Zombies_Raised = 0;
    Undead_Created = 0;
    No_Secondaries = ST_FALSE;


    // Lair Guard nibbles
    if(OVL_Action_Type == 5)  /* Lair */
    {
        _LAIRS[OVL_Action_Structure].guard1_count = (_LAIRS[OVL_Action_Structure].guard1_count & 0xF0);  // clear low-nibble
        if((_LAIRS[OVL_Action_Structure].guard2_count & 0x0F) == 0)
        {
            No_Secondaries = ST_TRUE;
        }
        _LAIRS[OVL_Action_Structure].guard2_count = (_LAIRS[OVL_Action_Structure].guard2_count & 0xF0);  // clear low-nibble
    }


    /*
        BEGIN:  
            drake178: process control change effects, regeneration, and recalls, while also counting the surviving units
    */

    Surviving_Unit_Count = 0;

    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
    {
        if((battle_units[itr_battle_units].Combat_Effects & bue_Confusion) != 0)
        {
            if(_UNITS[battle_units[itr_battle_units].unit_idx].owner_idx != player_idx)
            {
                battle_units[itr_battle_units].status = bus_Dead;
            }
            else
            {
                battle_units[itr_battle_units].controller_idx = _UNITS[battle_units[itr_battle_units].unit_idx].owner_idx;
            }
        }

        if(battle_units[itr_battle_units].controller_idx != player_idx)
        {
            if(battle_units[itr_battle_units].status == bus_Uninvolved)
            {
                battle_units[itr_battle_units].status = bus_Dead;
            }
        }
        else
        {
            if(battle_units[itr_battle_units].status == bus_Uninvolved)
            {
                battle_units[itr_battle_units].status = bus_Active;
            }
        }

        enchantments = battle_units[itr_battle_units].enchantments;
        enchantments |= _UNITS[battle_units[itr_battle_units].unit_idx].enchantments;
        enchantments |= battle_units[itr_battle_units].item_enchantments;

        if(
            ((battle_units[itr_battle_units].Combat_Effects & bue_Possession) != 0)
            ||
            ((battle_units[itr_battle_units].Combat_Effects & bue_Creature_Binding) != 0)
        )
        {
            if(battle_units[itr_battle_units].controller_idx != player_idx)
            {
                if(battle_units[itr_battle_units].controller_idx != _combat_attacker_player)
                {
                    battle_units[itr_battle_units].controller_idx = _combat_attacker_player;
                }
                else
                {
                    battle_units[itr_battle_units].controller_idx = _combat_defender_player;
                }
            }
            else
            {
                battle_units[itr_battle_units].status = bus_Gone;
            }
        }

        if(
            (battle_units[itr_battle_units].status <= bus_Dead)
            ||
            (battle_units[itr_battle_units].status == bus_Fleeing)
            ||
            (battle_units[itr_battle_units].status == bus_Recalled)
        )
        {
            if(((battle_units[itr_battle_units].Abilities & UA_REGENERATION) | (enchantments & UE_REGENERATION)) != 0)
            {
                if(battle_units[itr_battle_units].controller_idx == player_idx)
                {
                    battle_units[itr_battle_units].status = bus_Active;
                }

                battle_units[itr_battle_units].Cur_Figures = battle_units[itr_battle_units].Max_Figures;

                battle_units[itr_battle_units].TopFig_Dmg = 0;
            }
        }

        if(battle_units[itr_battle_units].status == bus_Recalled)
        {
            battle_unit_owner_idx = battle_units[itr_battle_units].controller_idx;

            _UNITS[battle_units[itr_battle_units].unit_idx].wx = _players[battle_unit_owner_idx].summon_wx;
            _UNITS[battle_units[itr_battle_units].unit_idx].wy = _players[battle_unit_owner_idx].summon_wy;
            _UNITS[battle_units[itr_battle_units].unit_idx].wp = _players[battle_unit_owner_idx].summon_wp;
            _UNITS[battle_units[itr_battle_units].unit_idx].Finished = ST_TRUE;
            UNIT_RemoveExcess(battle_units[itr_battle_units].unit_idx);
            battle_units[itr_battle_units].status = bus_Active;
            if(
                (battle_unit_owner_idx == player_idx)
                &&
                (battle_unit_owner_idx == _combat_attacker_player)
            )
            {
                _UNITS[battle_units[itr_battle_units].unit_idx].owner_idx = 100;
            }
        }

        if(
            (battle_units[itr_battle_units].controller_idx == player_idx)
            &&
            (battle_units[itr_battle_units].status == bus_Active)
        )
        {
            Surviving_Unit_Count++;
        }

    }  /* for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++) */

    /*
        END:  
    */


   Retreat_From_Combat(player_idx);

   GUI_Multipurpose_Int = 0;


    /*
        BEGIN:  
            drake178: process undead and zombie creation from dead units
                      INCONSISTENT: does not remove enchantments and mutations that are not available for the new unit
    */

    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
    {
        // Undead
        if(
            (battle_units[itr_battle_units].controller_idx == player_idx)
            &&
            (battle_units[itr_battle_units].status == bus_Drained)
            &&
            (battle_units[itr_battle_units].race == rt_Death)
            &&
            (_UNITS[battle_units[itr_battle_units].unit_idx].Hero_Slot == ST_UNDEFINED)
            &&
            (Surviving_Unit_Count < MAX_STACK)
        )
        {
            battle_units[itr_battle_units].Cur_Figures = battle_units[itr_battle_units].Max_Figures;
            battle_units[itr_battle_units].TopFig_Dmg = 0;
            _UNITS[battle_units[itr_battle_units].unit_idx].wx = _combat_wx;
            _UNITS[battle_units[itr_battle_units].unit_idx].wy = _combat_wy;
            _UNITS[battle_units[itr_battle_units].unit_idx].wp = _combat_wp;
            _UNITS[battle_units[itr_battle_units].unit_idx].owner_idx = player_idx;
            _UNITS[battle_units[itr_battle_units].unit_idx].mutations |= UM_UNDEAD;
            battle_units[itr_battle_units].controller_idx = player_idx;
            battle_units[itr_battle_units].status = bus_Active;
            Undead_Created++;
            Surviving_Unit_Count++;
            Experience_Gained += 2;
        }
        // Zombies
        if(
            (_players[player_idx].Globals[ZOMBIE_MASTERY] > 0)
            &&
            (battle_units[itr_battle_units].status == bus_Dead)
            &&
            (battle_units[itr_battle_units].race < rt_Arcane)
            &&
            (_UNITS[battle_units[itr_battle_units].unit_idx].Hero_Slot == ST_UNDEFINED)
            &&
            (Surviving_Unit_Count < 9)
        )
        {
            if(battle_units[itr_battle_units].controller_idx == player_idx)
            {
                Experience_Gained += 2;
            }

            Zombies_Raised++;

            _UNITS[battle_units[itr_battle_units].unit_idx].type = ut_Zombies;
            _UNITS[battle_units[itr_battle_units].unit_idx].owner_idx = player_idx;
            _UNITS[battle_units[itr_battle_units].unit_idx].XP = 0;
            _UNITS[battle_units[itr_battle_units].unit_idx].Level = 0;
            _UNITS[battle_units[itr_battle_units].unit_idx].wx = _combat_wx;
            _UNITS[battle_units[itr_battle_units].unit_idx].wy = _combat_wy;
            _UNITS[battle_units[itr_battle_units].unit_idx].wp = _combat_wp;
            battle_units[itr_battle_units].Cur_Figures = _unit_type_table[ut_Zombies].Figures;
            battle_units[itr_battle_units].Max_Figures = _unit_type_table[ut_Zombies].Figures;
            battle_units[itr_battle_units].TopFig_Dmg = 0;
            battle_units[itr_battle_units].status = bus_Active;

            Surviving_Unit_Count++;
        }

    }  /* for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++) */

    /*
        END:  
    */


    /*
        BEGIN:  Enemey City
    */

    if(OVL_Action_Type == 1)
    {
        CMB_Population_Lost = 0;
        CMB_Buildings_Lost = 0;
        if(player_idx != _combat_attacker_player)
        {
            CMB_Gold_Reward = 0;
        }
        else
        {
            if(_combat_defender_player != NEUTRAL_PLAYER_IDX)
            {
                _players[_combat_defender_player].fame -= _CITIES[OVL_Action_Structure].size;
                SETMIN(_players[_combat_defender_player].fame, 0);
            }

            if(_combat_defender_player != HUMAN_PLAYER_IDX)
            {
                GUI_Multipurpose_Int = _CITIES[OVL_Action_Structure].size;
            }

            if(_CITIES[OVL_Action_Structure].population == 0)
            {
                GAME_RazeCity = ST_TRUE;
            }
            else
            {
                if(player_idx != HUMAN_PLAYER_IDX)
                {
                    if(player_idx < _num_players)  /* ~== not NEUTRAL_PLAYER_IDX */
                    {
                        // ; decides whether the AI will raze the selected city or not
                        // ; returns 1 if yes, 0 if no
                        // ; INCONSISTENT: Lawful wizards disproportionately
                        // ;  adjust based on whether hidden relation is 0 or not
                        // ; BUG? surviving units are checked without owners
                        // TODO  GAME_RazeCity = AI_Raze_Decision(player_idx, OVL_Action_Structure);
                        GAME_RazeCity = ST_FALSE;
                    }
                }
                else
                {
                    if(magic_set.Raze_City == ST_TRUE)
                    {
                        RazeCity = Raze_City_Prompt(cnst_RazeCity_Msg);
                        if(RazeCity == 0)
                        {
                            GAME_RazeCity = ST_TRUE;
                        }
                    }
                }

            }

            if(GAME_RazeCity == ST_FALSE)
            {
                CMB_Gold_Reward = (_CITIES[OVL_Action_Structure].size - 2);

                SETMIN(CMB_Gold_Reward, 0);

                _players[player_idx].fame += CMB_Gold_Reward;
                
                if(player_idx == HUMAN_PLAYER_IDX)
                {
                    GUI_Multipurpose_Int -= CMB_Gold_Reward;
                }

                CMB_Gold_Reward = 0;

                CMB_Gold_Reward = CTY_GetConquerGold(OVL_Action_Structure);  // the conquering wizard gets a portion of the previous owner's gold reserve as loot.

                if(player_idx < _num_players)
                {
                    Player_Add_Gold(player_idx, CMB_Gold_Reward);
                }

                if(_CITIES[OVL_Action_Structure].owner_idx < _num_players)
                {
                    _players[_combat_defender_player].gold_reserve -= CMB_Gold_Reward;
                }
            }
            else
            {
                CMB_Gold_Reward = _CITIES[OVL_Action_Structure].size;

                if(player_idx == HUMAN_PLAYER_IDX)
                {
                    GUI_Multipurpose_Int -= CMB_Gold_Reward;
                }

                _players[player_idx].fame -= CMB_Gold_Reward;

                SETMIN(_players[player_idx].fame, 0);

                CMB_Gold_Reward = CTY_GetConquerGold(OVL_Action_Structure);

                if(_CITIES[OVL_Action_Structure].owner_idx < _num_players)
                {
                    _players[_combat_defender_player].gold_reserve -= CMB_Gold_Reward;
                }

                for(itr_buildings = bt_Barracks; itr_buildings < NUM_BUILDINGS; itr_buildings++)
                {
                    if(_CITIES[OVL_Action_Structure].bldg_status[itr_buildings] > bs_NotBuilt)
                    {
                        CMB_Gold_Reward += (bldg_data_table[itr_buildings].construction_cost / 10);
                    }
                }

                if(player_idx < _num_players)
                {
                    Player_Add_Gold(player_idx, CMB_Gold_Reward);
                }

            }

        }  /* if(player_idx != _combat_attacker_player) */



        if(_CITIES[OVL_Action_Structure].population != 0)
        {
            Population_Loss_Percent = (CMB_CityDamage / 2);

            if(player_idx == _combat_attacker_player)
            {
                if(player_idx == NEUTRAL_PLAYER_IDX)
                {
                    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
                    {
                        if(battle_units[itr_battle_units].controller_idx == NEUTRAL_PLAYER_IDX)
                        {
                            if((battle_units[itr_battle_units].Abilities & UA_FANTASTIC) == 0)
                            {
                                Summoned_Unit = ST_FALSE;
                            }
                            else
                            {
                                Summoned_Unit = ST_TRUE;
                            }
                        }
                    }

                    if(Summoned_Unit != ST_TRUE)
                    {
                        _active_battle_unit = 667;  /* 667 - raiders won (city neutral) */
                    }
                    else
                    {
                        Population_Loss_Percent += 50;

                        _active_battle_unit = CTY_RampageVictory();  /* 666 - rampage, no ruins  668 - rampage, created ruins */
                    }

                }
                else
                {
                    Population_Loss_Percent += 10;
                }
            }

            SETMAX(Population_Loss_Percent, 50);

            IDK_population_lost = 0;

            for(itr_population = 0; (_CITIES[OVL_Action_Structure].population - 1) > itr_population; itr_population++)
            {
                if(Random(100) <= Population_Loss_Percent)
                {
                    IDK_population_lost++;
                }
            }

            for(itr_buildings = 0; itr_buildings < NUM_BUILDINGS; itr_buildings++)
            {
                Buildings_Lost[itr_buildings] = 0;
            }

            Destruction_Chance = CMB_CityDamage;

            if(player_idx == _combat_attacker_player)
            {
                if(player_idx != NEUTRAL_PLAYER_IDX)
                {
                    Destruction_Chance += 10;
                }
                else
                {
                    Destruction_Chance += 50;
                }
            }

            SETMAX(Destruction_Chance, 75);

            if(_active_battle_unit != 668)  /* 668 - rampage, created ruins */
            {
                CTY_ApplyDamage(OVL_Action_Structure, IDK_population_lost, Destruction_Chance, &Buildings_Lost[0]);
            }

            if(player_idx != _combat_defender_player)
            {
                for(itr_bldg_msg = 0; g_bldg_msg_ctr> itr_bldg_msg; itr_bldg_msg++)
                {
                    if(MSG_Building_Complete[itr_bldg_msg].city_idx == OVL_Action_Structure)
                    {
                        for(bldg_msg_idx = itr_bldg_msg; g_bldg_msg_ctr > itr_bldg_msg; itr_bldg_msg++)
                        {
                            MSG_Building_Complete[bldg_msg_idx].city_idx = MSG_Building_Complete[(bldg_msg_idx + 1)].city_idx;
                        }
                    }
                }
            }

            CMB_Buildings_Lost = 0;

            for(itr_buildings = 0; itr_buildings < NUM_BUILDINGS; itr_buildings++)
            {
                if(Buildings_Lost[itr_buildings] > 0)
                {
                    CMB_Buildings_Lost++;
                }
            }

            CMB_Population_Lost = IDK_population_lost;

            CMB_LostBuildings = &Buildings_Lost[0];

        }  /* if(OVL_Action_Type == 1) */


    }  /* if(_CITIES[OVL_Action_Structure].population != 0) */

    /*
        END:  Enemey City
    */


    /*
        BEGIN:  
    */

    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
    {
        
        /*
            Lair Combat, Neutral Player won, Neutral Player's Unit, Unit is ~Alive
            move the unit back into the lair unit count and kill it's battle unit record
            not sure about the "No_Secondaries" part
        */
        {
            if(
                (OVL_Action_Type == 5)
                &&
                (player_idx == NEUTRAL_PLAYER_IDX)
                &&
                (battle_units[itr_battle_units].controller_idx == NEUTRAL_PLAYER_IDX)
                &&
                (battle_units[itr_battle_units].status == bus_Active)
            )
            {
                if(_UNITS[battle_units[itr_battle_units].unit_idx].type == _LAIRS[OVL_Action_Structure].guard1_unit_type)
                {
                    _LAIRS[OVL_Action_Structure].guard1_count++;
                    battle_units[itr_battle_units].status = bus_Dead;
                }
                else if(_UNITS[battle_units[itr_battle_units].unit_idx].type == _LAIRS[OVL_Action_Structure].guard2_unit_type)
                {
                    _LAIRS[OVL_Action_Structure].guard2_count++;
                    battle_units[itr_battle_units].status = bus_Dead;
                }
                else
                {
                    if(No_Secondaries != ST_TRUE)
                    {
                        battle_units[itr_battle_units].status = bus_Dead;
                    }
                    else
                    {
                        _LAIRS[OVL_Action_Structure].guard2_unit_type = _UNITS[battle_units[itr_battle_units].unit_idx].type;
                        _LAIRS[OVL_Action_Structure].guard2_count += 0x11;  // add 1 to both the high and low nibbles
                        battle_units[itr_battle_units].status = bus_Dead;
                        No_Secondaries = ST_FALSE;
                    }
                }
            }
        }

        /*
            @@Diplomatic_Value
        */
        Diplomatic_Value = 0;

        /*
            Current Battle Unit is Alive or Dead
        */
        if(
            (battle_units[itr_battle_units].status > bus_Active)
            ||
            (_UNITS[battle_units[itr_battle_units].unit_idx].wp == 9)  /* BU Combat Summon */  /* ¿ ~unsummoned ? */
            ||
            (
                (battle_units[itr_battle_units].controller_idx == NEUTRAL_PLAYER_IDX)
                &&
                (OVL_Action_Type != 0)  /* Enemy Army */
                &&
                (
                    (OVL_Action_Type != 1)  /* Enemy City */
                    ||
                    ((battle_units[itr_battle_units].Abilities & UA_FANTASTIC) != 0)
                )
            )
        )
        {

            // it's dead, and it's not ours, so we killed it, and we deserve the credit
            if(battle_units[itr_battle_units].controller_idx != player_idx)
            {
                Experience_Gained += 2;

                if(_unit_type_table[_UNITS[battle_units[itr_battle_units].unit_idx].type].cost >= 600)  /* ¿ >= the cost of Torin ? */
                {
                    Rare_Foe_Defeated = ST_TRUE;  // later, +1 Fame for "winning a battle where the enemy lost a very rare creature" - MoM-OSG
                }
            }


            if(
                (battle_units[itr_battle_units].controller_idx == _combat_defender_player)
                &&
                (_UNITS[battle_units[itr_battle_units].unit_idx].wp != 9)  /* BU Combat Summon */
            )
            {
                if(_UNITS[battle_units[itr_battle_units].unit_idx].Hero_Slot > -1)
                {
                    Diplomatic_Value += 20;
                }
                else
                {
                    Diplomatic_Value += Random(20);

                    if(
                        (battle_units[itr_battle_units].Construction > 0)
                        ||
                        ((battle_units[itr_battle_units].Abilities & UA_CREATEOUTPOST) != 0)
                    )
                    {
                        Diplomatic_Value += Random(10);
                    }
                }
            }


            /*
                BEGIN: Hero Unit

                    IDA Color: brick red
            */
            // if(_UNITS[battle_units[itr_battle_units].unit_idx].Hero_Slot > -1)
            if(_UNITS[battle_units[itr_battle_units].unit_idx].Hero_Slot > -1)
            {
                for(itr_hero_items = 0; itr_hero_items < NUM_HERO_ITEMS; itr_hero_items++)
                {

                    // if(_players[BUNITSOWNER()].Heroes[BUNITSHEROSLOT()].Items[itr_hero_items] > -1)
                    if(_players[_UNITS[battle_units[itr_battle_units].unit_idx].owner_idx].Heroes[_UNITS[battle_units[itr_battle_units].unit_idx].Hero_Slot].Items[itr_hero_items] > -1)
                    {
                        // BU Status 6 ~== unsummoned, banished, disintegrated, stoned, cracks called
                        if(battle_units[itr_battle_units].status == bus_Gone)
                        {
                            Remove_Item(_players[_UNITS[battle_units[itr_battle_units].unit_idx].owner_idx].Heroes[_UNITS[battle_units[itr_battle_units].unit_idx].Hero_Slot].Items[itr_hero_items]);
                        }
                        else
                        {
                            // only mostly dead
                            if(*item_count < 18)
                            {
                                item_list[*item_count] = _players[_UNITS[battle_units[itr_battle_units].unit_idx].owner_idx].Heroes[_UNITS[battle_units[itr_battle_units].unit_idx].Hero_Slot].Items[itr_hero_items];
                                *item_count += 1;
                            }
                            else
                            {
                                Remove_Item(_players[_UNITS[battle_units[itr_battle_units].unit_idx].owner_idx].Heroes[_UNITS[battle_units[itr_battle_units].unit_idx].Hero_Slot].Items[itr_hero_items]);
                            }
                        }
                    }

                    _players[_UNITS[battle_units[itr_battle_units].unit_idx].owner_idx].Heroes[_UNITS[battle_units[itr_battle_units].unit_idx].Hero_Slot].Items[itr_hero_items] = -1;
                }

                if(battle_units[itr_battle_units].controller_idx != player_idx)
                {
                    if(battle_units[itr_battle_units].controller_idx == HUMAN_PLAYER_IDX)
                    {
                        GUI_Multipurpose_Int -= ((_UNITS[battle_units[itr_battle_units].unit_idx].Level + 1) / 2);
                    }

                    _players[battle_units[itr_battle_units].controller_idx].fame -= ((_UNITS[battle_units[itr_battle_units].unit_idx].Level + 1) / 2);
                }

            }
            /*
                END: Hero Unit
            */


            // BU Status 6 ~== unsummoned, banished, disintegrated, stoned, cracks called
            if(
                (battle_units[itr_battle_units].status == bus_Gone)
                ||
                ((_UNITS[battle_units[itr_battle_units].unit_idx].mutations & UM_UNDEAD) != 0)
            )
            {
                Kill_Unit(battle_units[itr_battle_units].unit_idx, 2);  // wiped from existence
            }
            else
            {
                Kill_Unit(battle_units[itr_battle_units].unit_idx, 0);  // just mostly dead
            }

        }
        else
        {
            /*
                Current Battle Unit is Alive
            */
            SETMIN(battle_units[itr_battle_units].TopFig_Dmg, 0);
            battle_units[itr_battle_units].Extra_Hits = 0;
            battle_units[itr_battle_units].enchantments = 0;
            BU_CombatHits = Battle_Unit_Hit_Points(&battle_units[itr_battle_units]);
            BU_CombatHits -= battle_units[itr_battle_units].hits;
            battle_units[itr_battle_units].TopFig_Dmg -= BU_CombatHits;
            SETMIN(battle_units[itr_battle_units].TopFig_Dmg, 0);
            _UNITS[battle_units[itr_battle_units].unit_idx].Damage = (((battle_units[itr_battle_units].Max_Figures - battle_units[itr_battle_units].Cur_Figures) * (battle_units[itr_battle_units].hits - BU_CombatHits)) + battle_units[itr_battle_units].TopFig_Dmg);
            _UNITS[battle_units[itr_battle_units].unit_idx].moves2 = 0;
        }

    // @@Next_Battle_Unit__2
    }

    /*
        END:  
    */


    if(
        (_combat_defender_player != HUMAN_PLAYER_IDX)
        &&
        (_combat_defender_player != NEUTRAL_PLAYER_IDX)
        &&
        (_combat_attacker_player != NEUTRAL_PLAYER_IDX)
    )
    {
        Change_Relations(-Diplomatic_Value, _combat_attacker_player, _combat_defender_player, 8, ST_NULL, ST_NULL);
    }


    /*
        BEGIN:  
    */

    if(
        (OVL_Action_Type == 5)  // Lair
        &&   
        (player_idx == NEUTRAL_PLAYER_IDX)
    )
    {

        if(
            ((_LAIRS[OVL_Action_Structure].guard1_count & 0x0F) == 0)
            &&
            ((_LAIRS[OVL_Action_Structure].guard2_count & 0x0F) > 0)
        )
        {
            _LAIRS[OVL_Action_Structure].guard1_count = _LAIRS[OVL_Action_Structure].guard2_count;
            _LAIRS[OVL_Action_Structure].guard1_unit_type = _LAIRS[OVL_Action_Structure].guard2_unit_type;
            _LAIRS[OVL_Action_Structure].guard2_count = 0;
            _LAIRS[OVL_Action_Structure].guard2_unit_type = ut_Gargoyles;
        }

        if(
            (Undead_Created > 0)
            &&
            (_LAIRS[OVL_Action_Structure].guard1_unit_type >= ut_Magic_Spirit)
            &&
            (_LAIRS[OVL_Action_Structure].guard2_unit_type >= ut_Magic_Spirit)
        )
        {
            Undead_Created = 0;
        }
    }

    /*
        END:

    */


    /*
        BEGIN:  Fame for defeating 4+ enemy units
    */

    if(
        (player_idx != NEUTRAL_PLAYER_IDX)
        &&
        (Experience_Gained > 8)
    )
    {
        if(player_idx == HUMAN_PLAYER_IDX)
        {
            GUI_Multipurpose_Int++;
        }

        _players[player_idx].fame++;

        if(player_idx != _combat_attacker_player)
        {
            if(
                (_players[_combat_attacker_player].fame > 20)
                &&
                (_combat_attacker_player == HUMAN_PLAYER_IDX)
            )
            {
                GUI_Multipurpose_Int--;
            }

            _players[_combat_attacker_player].fame--;
        }
        else
        {
            if(
                (_players[_combat_defender_player].fame > 20)
                &&
                (_combat_defender_player == HUMAN_PLAYER_IDX)
            )
            {
                GUI_Multipurpose_Int--;
            }

            _players[_combat_defender_player].fame--;
        }
    }

    /*
        END:  Fame for defeating 4+ enemy units
    */


    if(player_idx == HUMAN_PLAYER_IDX)
    {
        GUI_Multipurpose_Int += Rare_Foe_Defeated;
    }

    _players[player_idx].fame += Rare_Foe_Defeated;



    if(
        (_combat_attacker_player == HUMAN_PLAYER_IDX)
        ||
        (_combat_defender_player == HUMAN_PLAYER_IDX)
    )
    {
        Combat_Results_Scroll();
    }



    /*
        BEGIN:  Experience
    */

    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
    {
        if(
            (battle_units[itr_battle_units].status == bus_Active)
            &&
            (battle_units[itr_battle_units].controller_idx == player_idx)
            &&
            (
                (battle_units[itr_battle_units].race < rt_Arcane)
                ||
                (_UNITS[battle_units[itr_battle_units].unit_idx].type == ut_Chosen)
            )
        )
        {
            _UNITS[battle_units[itr_battle_units].unit_idx].XP += Experience_Gained;
        }
    }

    /*
        END:  Experience
    */


    /*
        BEGIN:  Zombies & Undead
    */

    if(
    (_combat_attacker_player == HUMAN_PLAYER_IDX)
    ||
    (_combat_defender_player == HUMAN_PLAYER_IDX)
    )
    {
        if(Zombies_Raised > 0)
        {
            itoa(Zombies_Raised, temp_buffer, 10);

            strcpy(GUI_NearMsgString, temp_buffer);

            if(Zombies_Raised > 1)
            {
                strcat(GUI_NearMsgString, cnst_Zombie_Msg_1);
            }
            else
            {
                strcat(GUI_NearMsgString, cnst_Zombie_Msg_2);
            }

            strcat(GUI_NearMsgString, cnst_Zombie_Msg_3);

            // TODO  CMB_CreateUndeadAnim(ut_Zombies);
        }

        if(Undead_Created > 0)
        {
            itoa(Undead_Created, temp_buffer, 10);

            strcpy(GUI_NearMsgString, temp_buffer);

            if(Undead_Created > 1)
            {
                strcat(GUI_NearMsgString, cnst_Undead_Msg_1);
            }
            else
            {
                strcat(GUI_NearMsgString, cnst_Undead_Msg_2);
            }

            if(player_idx == HUMAN_PLAYER_IDX)
            {
                strcat(GUI_NearMsgString, cnst_Undead_Msg_3);
            }
            else
            {
                if(player_idx == NEUTRAL_PLAYER_IDX)
                {
                    if(Undead_Created > 1)
                    {
                        strcat(GUI_NearMsgString, cnst_Undead_Msg_4);
                    }
                    else
                    {
                        strcat(GUI_NearMsgString, cnst_Undead_Msg_5);
                    }
                }
                else
                {
                    strcat(GUI_NearMsgString, _players[player_idx].name);
                    strcat(GUI_NearMsgString, cnst_Dot9);
                }
            }

            // TODO  CMB_CreateUndeadAnim(CMB_GetUndeadCreator(player_idx));
        }

    }

    /*
        END:  Zombies & Undead
    */

    if(_combat_attacker_player == HUMAN_PLAYER_IDX)
    {
        // TODO  ¿ odd behavior, what to do ?  Set_Mouse_List(1, mouse_list_hourglass);
    }

}


// WZD o123p02
// CMB_GetUndeadCreator()

// WZD o123p03
void Combat_Results_Scroll_Draw(void)
{
    Copy_Back_To_Off();
    Set_Window(0, 0, SCREEN_XMAX, (_scroll_text_top + _scroll_text_height));
    Clipped_Draw(54, _scroll_text_top, _scroll_paper_seg);
    Reset_Window();
    FLIC_Draw(54, (_scroll_text_top + _scroll_text_height), _combat_results_scroll_bottom_seg);
    Combat_Results_Scroll_Text();
}


// WZD o123p04
// ¿ MoO2  Module: COMBFIND  Strategic_Results_Popup_() ?
// Settings - Help - 'Raze City' "combat results scroll"
void Combat_Results_Scroll(void)
{
    int16_t City_Capture;
    int16_t Hotkey_R2_Index;
    int16_t Hotkey_R1_Index;
    int16_t hotkey_ESC;
    int16_t leave_screen;
    int16_t Keep_City;
    int16_t IDK_popup_timer;  // _SI_
    int16_t input_field_idx;  // _DI_

    City_Capture = ST_FALSE;

    if(CMB_ScrollMsg_Type == 4)
    {
        // SCROLL.LBX  011  CMBLOSE
        _scroll_paper_seg = LBX_Reload_Next(scroll_lbx_file__ovr123, 11, _screen_seg);
    }
    else
    {
        if(OVL_Action_Type == 1)  /* Enemy City */
        {
            if(
                (CMB_ScrollMsg_Type == 1)
                ||
                (CMB_ScrollMsg_Type == 5)
                ||
                (CMB_ScrollMsg_Type == 6)
            )
            {
                // City, Won
                if(_combat_attacker_player == HUMAN_PLAYER_IDX)
                {
                    // SCROLL.LBX  009  CMBSCRL1
                    _scroll_paper_seg = LBX_Reload_Next(scroll_lbx_file__ovr123, 9, _screen_seg);
                    City_Capture = ST_TRUE;
                }
                else
                {
                    // SCROLL.LBX  010  CMBWIN
                    _scroll_paper_seg = LBX_Reload_Next(scroll_lbx_file__ovr123, 10, _screen_seg);
                }
            }
            else
            {
                // City, Lost
                if(_combat_attacker_player == HUMAN_PLAYER_IDX)
                {
                    // SCROLL.LBX  010  CMBWIN
                    _scroll_paper_seg = LBX_Reload_Next(scroll_lbx_file__ovr123, 10, _screen_seg);
                }
                else
                {
                    // SCROLL.LBX  009  CMBSCRL1
                    _scroll_paper_seg = LBX_Reload_Next(scroll_lbx_file__ovr123, 9, _screen_seg);
                }
            }
        }
        else
        {
            if(
                (CMB_ScrollMsg_Type == 1)  // Combat Victory, Tactical Combat
                ||
                (CMB_ScrollMsg_Type == 5)  // Enemy Fled
                ||
                (CMB_ScrollMsg_Type == 6)  // Combat Victory, Strategic Combat
            )
            {
                // SCROLL.LBX  010  CMBWIN
                _scroll_paper_seg = LBX_Reload_Next(scroll_lbx_file__ovr123, 10, _screen_seg);
            }
            else
            {
                // SCROLL.LBX  011  CMBLOSE
                _scroll_paper_seg = LBX_Reload_Next(scroll_lbx_file__ovr123, 11, _screen_seg);
            }
        }
    }

    // HELP.LBX  001  <no name, no description>
    _combat_results_scroll_bottom_seg = LBX_Reload_Next(help_lbx_file__ovr123, 1, _screen_seg);

    Set_Page_Off();

    _scroll_text_height = Combat_Results_Scroll_Text();

    _scroll_text_top = ((SCREEN_YMAX - (22 + _scroll_text_height)) / 2);

    Clear_Fields();

    Set_Page_Off();

    Combat_Results_Scroll_Draw();

    PageFlipEffect = 3;
    PageFlip_FX();
    PageFlipEffect = 0;
    
    Copy_On_To_Off_Page();

    Assign_Auto_Function(Combat_Results_Scroll_Draw, 1);
    
    leave_screen = ST_FALSE;

    Hotkey_R1_Index = Add_Hot_Key(cnst_HOTKEY_R);
    Hotkey_R2_Index = Add_Hot_Key(cnst_HOTKEY_R_2);
    hotkey_ESC = Add_Hidden_Field(0, 0, SCREEN_XMAX, SCREEN_YMAX, cnst_HOTKEY_Esc1A, ST_UNDEFINED);

    IDK_popup_timer = 0;
    while((IDK_popup_timer < 400) && (leave_screen == ST_FALSE))
    {
        input_field_idx = Get_Input();

        if(input_field_idx == hotkey_ESC)
        {
            leave_screen = ST_TRUE;
        }

        if((input_field_idx == Hotkey_R1_Index) || (input_field_idx == Hotkey_R2_Index))
        {
            if(GAME_RazeCity == ST_FALSE)
            {
                if(City_Capture == ST_TRUE)
                {
                    Keep_City = Raze_City_Prompt(cnst_RazeCity_Msg);

                    if(Keep_City == ST_FALSE)
                    {
                        GAME_RazeCity = ST_TRUE;
                        // ; BUG: this is different than the original penalty, as conquest fame is already applied!
                        // ; BUG: no zero check!
                        _players[HUMAN_PLAYER_IDX].fame -= (_CITIES[OVL_Action_Structure].size + 1);
                        GUI_Multipurpose_Int -= (_CITIES[OVL_Action_Structure].size + 1);

                        // ; (this resets the timeout counter)
                        for(IDK_popup_timer = 3; IDK_popup_timer < NUM_BUILDINGS; IDK_popup_timer++)
                        {
                            if(_CITIES[OVL_Action_Structure].bldg_status[IDK_popup_timer] > bs_NotBuilt)
                            {
                                // ; BUG: will re-award the original looted gold another time!
                                CMB_Gold_Reward += bldg_data_table[IDK_popup_timer].construction_cost;
                            }
                        }

                        Player_Add_Gold(HUMAN_PLAYER_IDX, CMB_Gold_Reward);

                        _scroll_text_height = Combat_Results_Scroll_Text();

                        _scroll_text_top = ((SCREEN_YMAX - (22 + _scroll_text_height)) / 2);

                    }

                    Clear_Fields();

                    hotkey_ESC = Add_Hidden_Field(0, 0, SCREEN_XMAX, SCREEN_YMAX, cnst_HOTKEY_Esc1A, ST_UNDEFINED);
                }
            }
        }

        if(leave_screen == ST_FALSE)
        {
            Set_Page_Off();
            Combat_Results_Scroll_Draw();
            PageFlip_FX();
        }

    }

    Deactivate_Auto_Function();
    Release_Block(_screen_seg);

}


// WZD o123p05
int16_t Combat_Results_Scroll_Text(void)
{
    char message[150] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char temp_string[20] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    uint8_t colors2[5] = { 0, 0, 0, 0, 0 };
    uint8_t colors1[5] = { 0, 0, 0, 0, 0 };
    int16_t next_x = 0;
    int16_t text_height = 0;  // _SI_
    int16_t itr_colors = 0;  // _DI_
    int16_t itr_buildings = 0;  // _DI_


    CMB_Scroll_MinHeight = 0;


    for(itr_colors = 0; itr_colors < 5; itr_colors++)
    {
        colors1[itr_colors] = 47;
    }

    for(itr_colors = 0; itr_colors < 5; itr_colors++)
    {
        colors2[itr_colors] = 55;
    }

    colors2[0] = 247;

    colors1[0] = 245;

    text_height = 92;

    Set_Font_Colors_15(4, &colors1[0]);


    switch(CMB_ScrollMsg_Type)
    {
        case 0:
        {

        } break;
        case 1:
        case 6:
        {
            LBX_Load_Data_Static(message_lbx_file__ovr123, 0, (SAMB_ptr)message, 5, 1, 150);  /* "You are triumphant" */
            Print_Centered(160, (25 + _scroll_text_top), message);
        } break;
        case 2:
        case 7:
        {
            LBX_Load_Data_Static(message_lbx_file__ovr123, 0, (SAMB_ptr)message, 6, 1, 150);  /* "You have been defeated" */
            Print_Centered(160, (25 + _scroll_text_top), message);
        } break;
        case 3:
        {
            LBX_Load_Data_Static(message_lbx_file__ovr123, 0, (SAMB_ptr)message, 7, 1, 150);  /* "Your forces have retreated" */
            Print_Centered(160, (_scroll_text_top + 25), message);
            if(GUI_NearMsgString != 0)
            {
                Set_Font_Colors_15(1, &colors2[0]);
                Set_Font_Spacing_Width(2);
                Print_Paragraph(75, (_scroll_text_top + text_height), 75, GUI_NearMsgString, 0);
                // TODO  text_height += (Get_Paragraph_Max_Height(175, GUI_NearMsgString, 0) + 2);
                text_height += (Get_Paragraph_Max_Height(175, GUI_NearMsgString) + 2);
            }
        } break;
        case 4:
        {
            LBX_Load_Data_Static(message_lbx_file__ovr123, 0, (SAMB_ptr)message, 8, 1, 150);  /*  "All units retreat exhausted" */
            Print_Centered(160, (25 + _scroll_text_top), message);
        } break;
        case 5:
        {
            LBX_Load_Data_Static(message_lbx_file__ovr123, 0, (SAMB_ptr)message, 9, 1, 150);  /*  "Your opponent has fled" */
            Print_Centered(160, (25 + _scroll_text_top), message);
        } break;
        case 8:
        {

        } break;
        case 9:
        {

        } break;
        case 10:
        {

        } break;
        case 11:
        {

        } break;
        case 12:
        {
            // String_Copy_Far(GUI_NearMsgString, _CITIES[OVL_Action_Structure].name);
            strcpy(GUI_NearMsgString, _CITIES[OVL_Action_Structure].name);
            strcat(GUI_NearMsgString, cnst_CityLost_Msg);  /* " has been conquered" */
            Print_Centered(160, (_scroll_text_top + 25), GUI_NearMsgString);
        } break;
    }


    if(_active_battle_unit == 666)  /* ; 666 - rampage, no ruins */
    {
        Set_Font_Colors_15(1, &colors2[0]);
        LBX_Load_Data_Static(message_lbx_file__ovr123, 0, (SAMB_ptr)message, 10, 1, 150);  /* "The monsters rampage through the city and then disappear into the wilderness" */
        Print_Paragraph(75, (_scroll_text_top + text_height), 175, message, 2);
        // TODO  text_height += (Get_Paragraph_Max_Height(175, message, 2) + 2));
        text_height += (Get_Paragraph_Max_Height(175, message) + 2);
    }

    if(_active_battle_unit == 668)  /* ; 668 - rampage, created ruins */
    {
        Set_Font_Colors_15(1, &colors2[0]);
        // String_Copy_Far(GUI_NearMsgString, _CITIES[OVL_Action_Structure].name);
        strcpy(message, _CITIES[OVL_Action_Structure].name);
        strcat(message, cnst_NewRuins_Msg);  /* " has been reduced to ruins" */
        Print_Paragraph(75, (_scroll_text_top + text_height), 175, message, 2);
        // TODO  text_height += (Get_Paragraph_Max_Height(175, message, 2) + 2));
        text_height += (Get_Paragraph_Max_Height(175, message) + 2);
    }

    if(_active_battle_unit == 667)  /* ; 667 - raiders won (city neutral) */
    {
        // String_Copy_Far(GUI_NearMsgString, _CITIES[OVL_Action_Structure].name);
        strcpy(message, _CITIES[OVL_Action_Structure].name);
        strcat(message, cnst_CityRaided_Msg);  /* " has fallen to raiders" */
        Print_Paragraph(75, (_scroll_text_top + text_height), 175, message, 2);
        // TODO  text_height += (Get_Paragraph_Max_Height(175, message, 2) + 2));
        text_height += (Get_Paragraph_Max_Height(175, message) + 2);
    }


    if(GUI_Multipurpose_Int != 0)
    {
        Set_Font_Colors_15(1, &colors2[0]);
        strcpy(message, cnst_ScrlFame_Msg_1);  /* "You have " */
        if(GUI_Multipurpose_Int < 0)
        {
            strcat(message, cnst_ScrlFame_Msg_2);  /* "lost " */
        }
        else
        {
            strcat(message, cnst_ScrlFame_Msg_3);  /* "gained " */
        }
        itoa(abs(GUI_Multipurpose_Int), temp_string, 10);
        strcat(message, temp_string);
        strcat(message, cnst_ScrlFame_Msg_4);  /* " fame." */
        Print_Centered(160, (_scroll_text_top + text_height), message);
        text_height += 9;
    }


    if(CMB_Gold_Reward > 0)
    {
        Set_Font_Colors_15(1, &colors2[0]);
        Set_Font_Spacing_Width(2);
        itoa(CMB_Gold_Reward, temp_string, 10);
        strcpy(message, temp_string);
        strcat(message, cnst_ScrlGold_Msg);  /* " gold pieces were looted." */
        Print_Centered(160, (_scroll_text_top + text_height), message);
        text_height += 9;
    }


    if(GAME_RazeCity == ST_TRUE)
    {
        Set_Font_Colors_15(1, &colors2[0]);
        Set_Font_Spacing_Width(2);
        Print_Centered(160, (_scroll_text_top + text_height), cnst_ScrlRaze_Msg);  /* "The city has been completly destroyed." */
        text_height += 7;
        text_height += 2;
    }
    else
    {
        if(CMB_Population_Lost > 0)
        {
            Set_Font_Colors_15(1, &colors2[0]);
            Set_Font_Spacing_Width(2);
            itoa(CMB_Population_Lost, temp_string, 10);
            strcpy(message, temp_string);
            strcat(message, cnst_ScrlPop_Msg);  /* " thousand inhabitants killed." */
            Print_Centered(160, (_scroll_text_top + text_height), message);
            text_height += 7;
            text_height += 2;
        }

        if(CMB_Buildings_Lost > 0)
        {
            Set_Font_Colors_15(4, &colors1[0]);
            Print_Centered(160, (_scroll_text_top + text_height), cnst_ScrlBldng_Msg);  /* "Destroyed Buildings" */
            text_height += 11;
            Set_Font_Colors_15(1, &colors2[0]);
            Set_Font_Spacing_Width(2);
            _scroll_start_x = text_height;
            for(itr_buildings = 0; itr_buildings < CMB_Buildings_Lost; itr_buildings++)
            {
                if(CMB_LostBuildings[itr_buildings] > 0)
                {
                    // next_x = Print_Far((75 + ((itr_buildings % 2) * 90)), (_scroll_text_top + text_height), bldg_data_table[CMB_LostBuildings[itr_buildings]]);
                    next_x = Print((75 + ((itr_buildings % 2) * 90)), (_scroll_text_top + text_height), bldg_data_table[CMB_LostBuildings[itr_buildings]].name);
                }
                if((itr_buildings % 2) == 1)
                {
                    text_height += 7;
                }
            }
            text_height += 2;
        }

        // ¿ BUG: this should be outside this if / the second-to-last-thing to happen ?
        SETMIN(text_height, CMB_Scroll_MinHeight);
    }

    return text_height;
}


// WZD o123p06
int16_t CTY_GetConquerGold(int16_t city_idx)
{
    int16_t Empire_Population;
    int16_t Gold_Reward;
    int16_t itr;  // _SI_

    Gold_Reward = 0;

    if(_CITIES[city_idx].owner_idx != NEUTRAL_PLAYER_IDX)
    {
        Empire_Population = 0;

        for(itr = 0; itr < _cities; itr++)
        {
            if(_CITIES[itr].owner_idx == _CITIES[city_idx].owner_idx)
            {
                Empire_Population += _CITIES[itr].population;
            }
        }

        Gold_Reward = ((_CITIES[city_idx].population * _players[_CITIES[city_idx].owner_idx].gold_reserve) / Empire_Population);
    }
    else
    {
        for(itr = 0; _CITIES[city_idx].population > itr; itr++)
        {
            Gold_Reward += Random(10);
        }
    }

    return Gold_Reward;
}


// WZD o123p07
int16_t CTY_RampageVictory(void)
{
    int16_t Unit_Type;
    int16_t Unit_Types;
    int16_t Secondary_Unit;
    int16_t Primary_Unit;
    int16_t Secondary_Count;
    int16_t Primary_Count;
    int16_t itr;  // _DI_
    int16_t idx;  // _SI_
    int16_t return_value;  // DNE in Dasm

    return_value = 0;  // DNE in Dasm

    // get next available lair_idx
    idx = ST_UNDEFINED;
    for(itr = 0; ((itr < NUM_LAIRS) && (idx == ST_UNDEFINED)); itr++)
    {
        if(_LAIRS[itr].Intact == ST_FALSE)
        {
            idx = itr;
        }
    }

    Primary_Unit = 0;
    Secondary_Unit = 0;
    Primary_Count = 0;
    Secondary_Count = 0;

    // 50:50 - destroyed city becomes new lair
    // OR no more lair_idx available
    // OR the city was the player's fortress city
    if(
        (Random(2) == 1)
        ||
        (idx == ST_UNDEFINED)
        ||
        (Player_Fortress_City(_CITIES[OVL_Action_Structure].owner_idx) == OVL_Action_Structure)
    )
    {
        return_value = 666;
    }
    else
    {
        Change_City_Ownership(OVL_Action_Structure, NEUTRAL_PLAYER_IDX);
        Destroy_City(OVL_Action_Structure);

        Unit_Types = 0;

        for(itr = 0; itr < _combat_total_unit_count; itr++)
        {

            if(
                (battle_units[itr].status == 0) // Unit_Active
                &&
                (battle_units[itr].controller_idx == NEUTRAL_PLAYER_IDX) // Unit_Active
            )
            {
                battle_units[itr].status = 4; // Unit_Dead

                Unit_Type = _UNITS[battle_units[itr].unit_idx].type;

                if(Primary_Unit == Unit_Type)
                {
                    Primary_Count++;
                }
                else
                {
                    if(Secondary_Unit == Unit_Type)
                    {
                        Secondary_Count++;
                    }
                    else
                    {
                        if(Unit_Types == 0)
                        {
                            Primary_Unit = Unit_Type;
                            Primary_Count = 1;
                            Unit_Types = 1;
                        }
                        else if(Unit_Types == 1)
                        {
                            Unit_Types = 2;
                            if(_unit_type_table[Unit_Type].cost < _unit_type_table[Primary_Unit].cost)
                            {
                                Secondary_Unit = Unit_Type;
                                Secondary_Count = 1;
                            }
                            else
                            {
                                Secondary_Unit = Primary_Unit;
                                Secondary_Count = Primary_Count;
                                Primary_Unit = Unit_Type;
                                Primary_Count = 1;
                            }
                        }
                        else
                        {
                            if(_unit_type_table[Unit_Type].cost > _unit_type_table[Primary_Unit].cost)
                            {
                                Secondary_Unit = Primary_Unit;
                                Secondary_Count = Primary_Count;
                                Primary_Unit = Unit_Type;
                                Primary_Count = 1;
                            }
                            else
                            {
                                if(_unit_type_table[Unit_Type].cost > _unit_type_table[Secondary_Unit].cost)
                                {
                                    Secondary_Unit = Unit_Type;
                                    Secondary_Count = 1;
                                }
                            }
                        }
                    }
                }
            }
        }

        _LAIRS[idx].guard1_unit_type = Primary_Unit;
        _LAIRS[idx].guard2_unit_type = Secondary_Unit;
        _LAIRS[idx].guard1_count = Primary_Count;
        _LAIRS[idx].guard2_count = Secondary_Count;
        _LAIRS[idx].wx = _combat_wx;
        _LAIRS[idx].wy = _combat_wy;
        _LAIRS[idx].wp = _combat_wp;
        _LAIRS[idx].Intact = ST_TRUE;
        _LAIRS[idx].type = lt_Ruins;
        _LAIRS[idx].Loot_Gold = CMB_Gold_Reward;
        _LAIRS[idx].Loot_Mana = 0;
        _LAIRS[idx].Spell_n_Special = 0;
        _LAIRS[idx].Misc_Flags = 0;
        _LAIRS[idx].Item_Count = 0;
        _LAIRS[idx].Item_Values[0] = 0;
        _LAIRS[idx].Item_Values[1] = 0;
        _LAIRS[idx].Item_Values[2] = 0;
    }

    return return_value;
}




/*
    WIZARDS.EXE  ovr124
*/

// WZD o124p01
// drake178: WIZ_GetLastRangedStr()
/*
; last (highest BU index) active ranged unit
;
; WARNING: was likely intended to return either the
; highest or the total ranged strength, rather than the
; last one, which is not very indicative for basing
; strategic decisions on
*/
/*

*/
int16_t WIZ_GetLastRangedStr__WIP(int16_t player_idx)
{
    int16_t battle_unit_idx = 0;  // _SI_
    int16_t IDK_ranged_strength = 0;  // _DI_

    IDK_ranged_strength = 0;

    for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
    {

        if(battle_units[battle_unit_idx].status == bus_Active)
        {

            if(battle_units[battle_unit_idx].controller_idx == player_idx)
            {

                if(Battle_Unit_Has_Ranged_Attack(battle_unit_idx) != ST_FALSE)
                {

                    IDK_ranged_strength = battle_units[battle_unit_idx].ranged;

                }

            }

        }

    }

    return IDK_ranged_strength;

}


// WZD o124p02
// drake178: BU_SetCityMovement()
/*
; modifies the GUI_ActiveMoveMap based on whether the
; unit is allowed to enter the city proper or not,
; depending on any intact walls and Flying Fortress
;
; BUGs: allows Merging and Non-Corporeal units to enter
; a Flying Fortress even if they don't fly, and can set
; the central structure tile impassable even when there
; isn't one
*/
/*

*/
void BU_SetCityMovement__WIP(int16_t battle_unit_idx)
{
    int16_t itr2 = 0;  // _SI_
    int16_t itr1 = 0;  // _DI_

    if(battlefield->Walled_City == ST_TRUE)
    {

        if(battlefield->Wall_Sections[0] == ST_TRUE)
        {

            CMB_ActiveMoveMap[((10 * COMBAT_GRID_WIDTH) + 5)] = INF;  // 0xD7

        }

        if(battlefield->Wall_Sections[3] == ST_TRUE)
        {
            
            CMB_ActiveMoveMap[((10 * COMBAT_GRID_WIDTH) + 8)] = INF;  // 0xDA

        }

        if(battlefield->Wall_Sections[9] == ST_TRUE)
        {
            
            CMB_ActiveMoveMap[((13 * COMBAT_GRID_WIDTH) + 5)] = INF;  // 0x116

        }

        if(battlefield->Wall_Sections[15] == ST_TRUE)
        {
            
            CMB_ActiveMoveMap[((13 * COMBAT_GRID_WIDTH) + 8)] = INF;  // 0x119

        }

    }

    // ; BUG: open fields don't have anything there either
    if(battlefield->Central_Structure != CS_City)
    {

        CMB_ActiveMoveMap[((11 * COMBAT_GRID_WIDTH) + 6)] = INF;  // 0xED
        
    }


    if(
        (
            ((battle_units[battle_unit_idx].Move_Flags & MV_FLYING) != 0)
            ||
            ((battle_units[battle_unit_idx].Move_Flags & MV_TELEPORT) != 0)
            ||
            ((battle_units[battle_unit_idx].Move_Flags & MV_MERGING) != 0)
            ||
            ((battle_units[battle_unit_idx].Abilities & UA_NONCORPOREAL) != 0)
        )
        &&
        (
            (battlefield->Walled_City == ST_TRUE)
            ||
            (battlefield->city_enchantments[FLYING_FORTRESS] != 0)
        )
    )
    {

        if(BU_IsInCityProper__STUB(battle_unit_idx) == ST_FALSE)
        {
            /*
            mark all tiles immediately behind an intact city wall
            section as impassable with the exception of the tile
            behind the gates, and mark every tile inside a Flying
            Fortress also impassable
            */
        }
        else
        {
            /*
            mark the tiles immediately outside of the city proper
            as impassable if there's an active Flying Fortress
            enchantment on the city, or if there's an intact
            city wall section between it and the immediately
            adjacent city proper tile - except at the gate

            WARNING: only allows passage through destroyed walls
            in the cardinal directions (i.e. no diagonals unless
            the next wall section is also destroyed)
            */
        }

    }


    if(
        ((battle_units[battle_unit_idx].Abilities & 0) == 0)
        &&
        (battlefield->city_enchantments[FLYING_FORTRESS] != 0)
        &&
        ((battle_units[battle_unit_idx].Move_Flags & MV_FLYING) == 0)
        &&
        ((battle_units[battle_unit_idx].Move_Flags & MV_TELEPORT) == 0)
        &&
        (BU_IsInCityProper__STUB(battle_unit_idx) != ST_FALSE)
    )
    {

        for(itr1 = 0; itr1 < COMBAT_GRID_CITY_AREA_WIDTH; itr1++)
        {

            for(itr2 = 0; itr2 < COMBAT_GRID_CITY_AREA_HEIGHT; itr2++)
            {

                CMB_ActiveMoveMap[(((10 + itr2) * COMBAT_GRID_WIDTH) + itr1)] = INF;

            }

        }

    }

}


// WZD o124p03
// drake178: AI_RestrictToCity()
/*
; marks the tiles surrounding the city proper area in
; battle as impassable in GUI_ActiveMoveMap,
; preventing units from pathing to the outside
*/
/*

    set city area perimeter to *impassible*

XREF:
    G_AI_BU_MoveOrRampage__WIP()
    Auto_Move_Ship()

*/
void AI_RestrictToCity__WIP(void)
{
    int16_t itr1 = 0;  // _SI_
    int16_t itr2 = 0;  // _CX_

    for(itr1 = 0; itr1 < 4; itr1++)
    {

        for(itr2 = 0; itr2 < 4; itr2++)
        {

            if(itr1 == 0)
            {

                CMB_ActiveMoveMap[(((10 + itr2) * COMBAT_GRID_WIDTH) + 4)] = INF;

                if(itr2 == 0)
                {

                    CMB_ActiveMoveMap[((9 * COMBAT_GRID_WIDTH) + 4)] = INF;  // 0xC1  193  (9 * 21) = 189 + 4

                }

                if(itr2 == 3)
                {

                    CMB_ActiveMoveMap[((14 * COMBAT_GRID_WIDTH) + 4)] = INF;  // 0x12A  298  (14 * 21) = 294 + 4

                }

            }

            if(itr2 == 0)
            {

                CMB_ActiveMoveMap[(((9 * COMBAT_GRID_WIDTH) + 5) + itr1)] = INF;  // 0xC2  194  (9 * 21) = 189 + 5

            }

            if(itr1 == 3)
            {

                CMB_ActiveMoveMap[(((10 + itr2) * COMBAT_GRID_WIDTH) + 9)] = INF;

                if(itr2 == 0)
                {

                    CMB_ActiveMoveMap[((9 * COMBAT_GRID_WIDTH) + 9)] = INF;  // 0xC6  198  (9 * 21) = 189 + 9

                }

                if(itr2 == 3)
                {

                    CMB_ActiveMoveMap[((14 * COMBAT_GRID_WIDTH) + 9)] = INF;  // 0x12F  303  (14 * 21) = 294 + 9

                }

            }

            if(itr2 == 3)
            {

                CMB_ActiveMoveMap[(((14 * COMBAT_GRID_WIDTH) + 5) + itr1)] = INF;  // 0x12B  299  (14 * 21) = 294 + 5

            }

        }

    }

}


// WZD o124p04
/*
    "STRUCTURES INFLUENCING COMBAT"

    (only?) used Init_Combat_Conditions() as {F,T} ... if defender == owner then +3 defense  (~ City Walls)

Eh ? set_city_flag?
only called from Strategic Combat and Tactical Combat
Tactical calls it with SetCtyCnd 0
Strategic calls it with SetCtyCnd 1
so, for Tactical, could be city combat but no condition 1?

Maybe not pure happenstance that 2,3,4,6 inherently means it can't be city combat?

maybe not 5 because 5 is Lair?
...which means it is pure happenstance that 5 is neutral player and lair combat?

*/
int16_t Combat_Structure(int16_t wx, int16_t wy, int16_t wp, int16_t set_city_flag)
{
    int16_t combat_structure = 0;  //  _SI_
    int16_t terrain_type = 0;  // _DI_

    combat_structure = cs_NONE;

    if(Square_Is_Sailable(wx, wy, wp) != ST_FALSE)
    {

        combat_structure = cs_OceanTerrainType;

    }

    if(OVL_Action_Type == 1)  /* Combat - Enemy City */
    {

        if(_CITIES[OVL_Action_Structure].enchantments[HEAVENLY_LIGHT] > 0)
        {

            combat_enchantments[TRUE_LIGHT_DFNDR] = 2;

        }

        if(_CITIES[OVL_Action_Structure].enchantments[CLOUD_OF_SHADOW] > 0)
        {

            combat_enchantments[DARKNESS_DFNDR] = 2;

        }

        if(set_city_flag == ST_TRUE)
        {

            combat_structure = cs_City;

        }

    }


    terrain_type = (_world_maps[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] % NUM_TERRAIN_TYPES);

    if(terrain_type == tt_SorceryNode)
    {

        combat_structure = cs_SorceryNode;

    }
    
    if(terrain_type == tt_NatureNode)
    {

        combat_structure = cs_NatureNode;

    }
    
    if(terrain_type == tt_ChaosNode)
    {

        combat_structure = cs_ChaosNode;

    }

    return combat_structure;

}


// WZD o124p05
// drake178: CMB_MarkLandLubbers()
/*
*/
/*
    checks for wind-walker or floating island
    if none, sets non-boat, non-flying, non-swimming units to 'uninvolved'
    returns a count of units that can not participate

¿ kinda like // WZD o95p05  int16_t Army_Boatriders(int16_t troop_count, int16_t troops[], int16_t boatriders[]) ?

Page 98  (PDF Page 103)
Ocean Combat
...
"Only flying, sailing and swimming units may actually engage in combat over such squares."
*/
int16_t Undeployable_Battle_Units_On_Water(int16_t player_idx)
{
    int16_t unit_count = 0;  // _DI_
    int16_t itr_battle_units = 0;  // _SI_

    unit_count = 0;

    if(Square_Is_Sailable(_combat_wx, _combat_wy, _combat_wp) == ST_TRUE)
    {

        for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
        {
            if(battle_units[itr_battle_units].controller_idx == player_idx)
            {
                if(
                    ((battle_units[itr_battle_units].Abilities & UA_WINDWALKING) == 0)
                    ||
                    ((battle_units[itr_battle_units].enchantments & UE_WINDWALKING))
                    ||
                    (_UNITS[battle_units[itr_battle_units].unit_idx].type == spell_data_table[ut_Floating_Island].unit_type)
                )
                {
                    return 0;
                }

            }
        }

        for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
        {
            if(
                (battle_units[itr_battle_units].controller_idx == player_idx)
                &&
                (battle_units[itr_battle_units].carry_capacity == 0)
                &&
                ((battle_units[itr_battle_units].Move_Flags & MV_FLYING) == 0)
                &&
                ((battle_units[itr_battle_units].Move_Flags & MV_SWIMMING) == 0)
            )
            {
                battle_units[itr_battle_units].status = bus_Uninvolved;
                unit_count++;
            }

        }

    }

    return unit_count;
}


// WZD o124p06
// drake178: AI_CompareArmies()
/*
; calculates the threat levels of both own and enemy
; units, and returns a value ranging from 0
; (overwhelming enemy advantage) to 5 (insignificant
; opposition)
;
; WARNING: after the first turn, AI units with no
;  target set don't count into the army strength!
*/
/*

*/
int16_t AI_CompareArmies__STUB(int16_t player_idx)
{

    return 4;

}


// WZD o124p07
// drake178: BU_LoadMoveSound()
/*
; appends the movement sound effect of the specified
; unit into the World_Data@ allocation
; returns the segment pointer to the effect data
;
; WARNING: contains no error checking, and will try to
; load regardless fo the passed index or SFX settings
*/
/*

*/
// SAMB_ptr BU_LoadMoveSound__WIP(int16_t battle_unit_idx)
void BU_LoadMoveSound__WIP(int16_t battle_unit_idx, SAMB_ptr * sound_seg, uint32_t * sound_seg_size)
{
    // SAMB_ptr sound = 0;  // _SI_
    int16_t sound = 0;  // _SI_

    sound = (_unit_type_table[_UNITS[battle_units[battle_unit_idx].unit_idx].type].Sound >> 4);  // high nibble

    if(sound == 0)
    {
        // sound = LBX_Reload_Next(soundfx_lbx_file__ovr124__1of2, SFX_Silence, World_Data);
        *sound_seg = LBX_Reload_Next(soundfx_lbx_file__ovr124__1of2, SFX_Silence, World_Data);
        *sound_seg_size = lbxload_entry_length;
    }
    else if(sound == 13)
    {
        // sound = LBX_Reload_Next(newsound_lbx_file__ovr124__1of2, 0, World_Data);
        *sound_seg = LBX_Reload_Next(newsound_lbx_file__ovr124__1of2, 0, World_Data);
        *sound_seg_size = lbxload_entry_length;
    }
    else
    {
        // sound = LBX_Reload_Next(soundfx_lbx_file__ovr124__1of2, sound, World_Data);
        *sound_seg = LBX_Reload_Next(soundfx_lbx_file__ovr124__1of2, sound, World_Data);
        *sound_seg_size = lbxload_entry_length;
    }

    // return sound;

}


// WZD o124p08
// drake178: BU_LoadMeleeSound()
/*
; appends the melee sound effect of the specified unit
; into the World_Data@ allocation
; returns the segment pointer to the effect data
*/
/*

*/
// SAMB_ptr BU_LoadMeleeSound__WIP(int16_t battle_unit_idx)
void BU_LoadMeleeSound__WIP(int16_t battle_unit_idx, SAMB_ptr * sound_seg, uint32_t * sound_seg_size)
{
    // SAMB_ptr sound = 0;  // _SI_
    int16_t sound = 0;  // _SI_

    sound = (_unit_type_table[_UNITS[battle_units[battle_unit_idx].unit_idx].type].Sound & 0x0F);  // low nibble

    if(sound == 8)
    {
        // sound = LBX_Reload_Next(soundfx_lbx_file__ovr124__1of2, SFX_Silence, World_Data);
        *sound_seg = LBX_Reload_Next(soundfx_lbx_file__ovr124__1of2, SFX_Silence, World_Data);
        *sound_seg_size = lbxload_entry_length;
    }
    else
    {
        // sound = LBX_Reload_Next(cmbtsnd_lbx_file__ovr124, sound, World_Data);
        *sound_seg = LBX_Reload_Next(cmbtsnd_lbx_file__ovr124, sound, World_Data);
        *sound_seg_size = lbxload_entry_length;
    }

    // return sound;

}

// WZD o124p09
// drake178: GAME_LoadSpellSound()
/*
; if sound effects are enabled, plays the silence
; sound, marks the World_Data@ allocation, appends the
; sound of the specified spell into it, assigns it to
; a pointer, then undoes the allocation (the data
; remains in memory); if sound effects are disabled,
; the pointer is set to -1 instead
*/
/*

XREF:
    j_GAME_LoadSpellSound__WIP()
        CMB_BattlefieldSpell()
        CMB_PlaySpellAnim()
        WIZ_CallLightning()
        CMB_CallChaos()

*/
void GAME_LoadSpellSound__WIP(int16_t spell_idx)
{

    if(magic_set.sound_effects == ST_TRUE)
    {
        // DOMSDOS  Play_Sound__STUB(SND_CMB_Silence);
        sdl2_Play_Sound__WIP(SND_CMB_Silence, SND_CMB_Silence_size);

        Mark_Block(World_Data);

        if(spell_data_table[spell_idx].Sound > -1)
        {
            SND_SpellCast = LBX_Reload_Next(soundfx_lbx_file__ovr124__2of2, spell_data_table[spell_idx].Sound, World_Data);
            SND_SpellCast_size = lbxload_entry_length;
        }
        else
        {
            SND_SpellCast = LBX_Reload_Next(newsound_lbx_file__ovr124__2of2, abs(spell_data_table[spell_idx].Sound), World_Data);
            SND_SpellCast_size = lbxload_entry_length;
        }

        Release_Block(World_Data);

    }
    else
    {
        SND_SpellCast = ST_UNDEFINED;
    }

}


// WZD o124p10
// drake178: BU_WallofFire()
/*
; check whether the unit's target coordinates are
; through a Wall of Fire from the outside, and if so,
; process and apply a base strength Fireball effect
; targeted at it
*/
/*



*/
void BU_WallofFire__NOOP(int16_t battle_unit_idx)
{



}


// WZD o124p11
// drake178: BU_SetBloodAnim()

// WZD o124p12
// drake178: BU_ClearBlood()

// WZD o124p13
// drake178: BU_IsInCityProper()
int16_t BU_IsInCityProper__STUB(int16_t battle_unit_idx)
{

    return ST_FALSE;

}


// WZD o124p14
// drake178: TILE_HasIntactWall()
/*
; returns 1 if the specified combat tile has an intact
; city wall section on it, or 0 otherwise
*/
/*

*/
int16_t Combat_Grid_Cell_Has_City_Wall(int16_t cgc2, int16_t cgc1)
{
    int16_t has_wall = 0;  // _DI_

    has_wall = ST_FALSE;

    if(
        (battlefield->Walled_City == ST_TRUE)
        &&
        (
            (cgc2 >= 5)
            &&
            (cgc2 <= 8)
        )
        &&
        (
            (cgc1 >= 10)
            &&
            (cgc1 <= 13)
        )
        &&
        (
            !(
                ((cgc2 == 6) || (cgc2 == 7))
                &&
                ((cgc1 == 11) || (cgc1 == 12))
            )
        )
    )
    {

        // DEDU  sizeof()?  if(battlefield->Wall_Sections[((cgc1 - 10) * 8) + ((cgc2 - 5) * 2)] == ST_TRUE)
        if(battlefield->Wall_Sections[(((cgc1 - 10) * 4) + (cgc2 - 5))] == ST_TRUE)
        {

            has_wall = ST_TRUE;

        }

    }

    return has_wall;

}


// Segrax
// WZD o124p15
// drake178: BU_IsCombatSummon
/*
returns 1 if the target unit is a combat summon, or
0 otherwise
*/
/*

"While most creature summonings are cast overland, the summoning of Elementals, Phantom Warriors and Phantom Beasts can only be performed during combat."

*/
int16_t BU_IsCombatSummon__WIP(int16_t battle_unit_idx)
{
    int16_t unit_type = 0;  // _SI_

    unit_type = _UNITS[battle_units[battle_unit_idx].unit_idx].type;

    // ovr124:0BFE 26 39 B7 C8 0C                                  cmp     es:[bx+(Fire_Elemental*24h)+s_SPELL_DATA.Param0], _SI_unit_type
    // field_CA8 / 24h = 5Ah  90 Fire_Elemental
    // field_438 / 24h = 1Eh  30 Earth_Elemental  
    // field_870 / 24h = 3Ch  60 Phantom_Beast
    // field_654 / 24h = 2Dh  45 Phantom_Warriors
    // field_948 / 24h = 42h  66 Air_Elemental
    if(
        (spell_data_table[90].Param0 == unit_type)
        ||
        (spell_data_table[30].Param0 == unit_type)
        ||
        (spell_data_table[60].Param0 == unit_type)
        ||
        (spell_data_table[45].Param0 == unit_type)
        ||
        (spell_data_table[66].Param0 == unit_type)
        ||
        (unit_type == ut_Demon)
    )
    {
        return ST_TRUE;
    }
    else
    {
        return ST_FALSE;
    }

}


// WZD o124p16
// drake178: BU_GetHalfMoves()
/*
; calculates and returns the unit's movement allowance
; in half movement point units, accounting for all
; factors
;
; BUG: fails to initialize the Endurance variable,
; causing units without it to sometimes gain the effect
;
; INCONSISTENT: it applies modifiers in different order
; than UNIT_GetHalfMoves, resulting in discrepancies
; between overland and combat movement allowance under
; certain situations
*/
/*

~== NEXTTURN.C  Unit_Moves2()

*/
int16_t Battle_Unit_Moves2(int16_t battle_unit_idx)
{
    uint32_t enchantments;
    int16_t * hero_items = 0;
    int16_t wind_mastery = 0;
    int16_t item_moves2 = 0;
    int16_t itr = 0;
    int16_t endurance = 0;
    int16_t moves2 = 0;  // _SI_


    if(battle_units[battle_unit_idx].Web_HP > 0)
    {
        return 0;
    }


    enchantments = (battle_units[battle_unit_idx].enchantments | _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments);

    item_moves2 = 0;

    /*
        BEGIN: Hero Items
    */
    if(_UNITS[battle_units[battle_unit_idx].unit_idx].Hero_Slot > -1)
    {

        // ; BUG: this may not be the hero's original owner
        // ¿ should index UNITS[] for owner_idx ?
        hero_items = &(_players[battle_units[battle_unit_idx].controller_idx].Heroes[_UNITS[battle_units[battle_unit_idx].unit_idx].Hero_Slot].Items[0]);

        for(itr = 0; itr < NUM_HERO_ITEMS; itr++)
        {

            if(hero_items[itr] > -1)
            {
                
                if(ITEM_POWER(hero_items[itr], ip_Endurance))
                {
                    endurance = ST_TRUE;
                }

                /* NOTE: Battle_Unit_Moves2() does not add the Item Enchantments like Unit_Moves2()
                 * TODO  enchantments |= _ITEMS[itr].Powers;
                 * enchantments |= GET_4B_OFS((uint8_t*)&_ITEMS[hero_items[itr]], 0x2E);
                 */

                item_moves2 += _ITEMS[hero_items[itr]].moves2;

            }

        }

    }
    /*
        END: Hero Items
    */


    moves2 = _unit_type_table[_UNITS[battle_units[battle_unit_idx].unit_idx].type].Move_Halves;

    moves2 += item_moves2;


    /*
        BEGIN: Wind Mastery
    */
    if(_unit_type_table[_UNITS[battle_units[battle_unit_idx].unit_idx].type].Transport > 0)
    {
        wind_mastery = 0;
        for(itr = 0; itr < NUM_PLAYERS; itr++)
        {
            if(_players[itr].Globals[WIND_MASTERY] > 0)
            {
                if(battle_units[battle_unit_idx].controller_idx == itr)
                {
                    wind_mastery++;
                }
                else
                {
                    wind_mastery--;
                }
            }
        }
        if(wind_mastery > 0)
        {
            moves2 = ((moves2 * 3) / 2);  /* +50% */
        }
        if(wind_mastery < 0)
        {
            moves2 = (moves2 / 2);  /* -50% */
        }
    }
    /*
        END: Wind Mastery
    */


    if(moves2 < 6)
    {
        if((enchantments & UE_FLIGHT) != 0)
        {
            moves2 = 6;
        }
    }

    if(moves2 < 4)
    {
        if((_UNITS[battle_units[battle_unit_idx].unit_idx].mutations & CC_FLIGHT) != 0)
        {
            moves2 = 4;
        }
    }

    if((enchantments & UE_ENDURANCE) != 0)
    {
        endurance = ST_TRUE;
    }

    // ; BUG: variable never initialized
    // ¿ NOBUG  as compiled, set to 0 ?
    if(endurance == ST_TRUE)
    {
        moves2 += 2;
    }

    if((battle_units[battle_unit_idx].Combat_Effects & bue_Haste) != 0)
    {
        moves2 *= 2;
    }


    return moves2;

}


// WZD o124p17
/*
    "WIP", cause AI_Raze_Decision(Stack_Owner, OVL_Action_Structure);
*/
void STK_CaptureCity__WIP(int16_t troop_count, int16_t troops[])
{
    int16_t City_Owner = 0;
    int16_t Stack_Owner = 0;  // _DI_
    int16_t itr = 0;  // _SI_

    Unit_View_Allocate();  // ¿ if/where used ?

    Set_Page_Off();
    Main_Screen_Draw();
    Copy_Off_To_Back();

    Stack_Owner = _UNITS[troops[0]].owner_idx;

    City_Owner = _CITIES[OVL_Action_Structure].owner_idx;

    GUI_Multipurpose_Int = 0;

    if(City_Owner < _num_players)
    {
        _players[City_Owner].fame -= _CITIES[OVL_Action_Structure].size;

        if(City_Owner == HUMAN_PLAYER_IDX)
        {
            GUI_Multipurpose_Int -= _CITIES[OVL_Action_Structure].size;
        }

        if(_players[City_Owner].fame < 0)
        {
            _players[City_Owner].fame = 0;
        }
    }

    if(_CITIES[OVL_Action_Structure].population == 0)
    {
        GAME_RazeCity = ST_TRUE;
    }
    else
    {
        GAME_RazeCity = ST_FALSE;

        if(Stack_Owner == HUMAN_PLAYER_IDX)
        {
            if(magic_set.Raze_City == ST_TRUE)
            {
                if(Raze_City_Prompt(cnst_RazeCity_Msg2) == 0)
                {
                    GAME_RazeCity = ST_TRUE;
                }
            }
        }
        else
        {
            if(Stack_Owner < _num_players)
            {
                // TODO GAME_RazeCity = AI_Raze_Decision(Stack_Owner, OVL_Action_Structure);
            }
        }

    }

    if(GAME_RazeCity == ST_FALSE)
    {
        CMB_Gold_Reward = (_CITIES[OVL_Action_Structure].size - 2);

        if(CMB_Gold_Reward < 0)
        {
            CMB_Gold_Reward = 0;
        }

        _players[Stack_Owner].fame += CMB_Gold_Reward;

        if(Stack_Owner == HUMAN_PLAYER_IDX)
        {
            GUI_Multipurpose_Int += CMB_Gold_Reward;
        }

        CMB_Gold_Reward = 0;

        CMB_Gold_Reward = CTY_GetConquerGold(OVL_Action_Structure);

        if(Stack_Owner < _num_players)
        {
            Player_Add_Gold(Stack_Owner, CMB_Gold_Reward);
        }

        if(City_Owner < _num_players)
        {
            _players[City_Owner].gold_reserve -= CMB_Gold_Reward;

            if(_players[City_Owner].gold_reserve < 0)
            {
                _players[City_Owner].gold_reserve = 0;
            }
        }

    }
    else
    {
        CMB_Gold_Reward = _CITIES[OVL_Action_Structure].size;

        if(Stack_Owner == HUMAN_PLAYER_IDX)
        {
            GUI_Multipurpose_Int += CMB_Gold_Reward;
        }

        if(Stack_Owner < _num_players)
        {
            _players[Stack_Owner].fame -= CMB_Gold_Reward;

            if(_players[Stack_Owner].fame < 0)
            {
                _players[Stack_Owner].fame = 0;
            }
        }

        CMB_Gold_Reward = CTY_GetConquerGold(OVL_Action_Structure);

        if(City_Owner < _num_players)
        {
            _players[City_Owner].gold_reserve -= CMB_Gold_Reward;

            if(_players[City_Owner].gold_reserve < 0)
            {
                _players[City_Owner].gold_reserve = 0;
            }
        }

        for(itr = 3; itr < 36; itr++)
        {
            if(_CITIES[OVL_Action_Structure].bldg_status[itr] > bs_NotBuilt)
            {
                CMB_Gold_Reward += (bldg_data_table[itr].construction_cost / 10);
            }
        }

        if(Stack_Owner < _num_players)
        {
            Player_Add_Gold(Stack_Owner, CMB_Gold_Reward);
        }

    }


    battle_units = (struct s_BATTLE_UNIT *)Allocate_First_Block(_screen_seg, 63);

    if(
        (_UNITS[troops[0]].owner_idx == NEUTRAL_PLAYER_IDX)
        &&
        ((_unit_type_table[_UNITS[troops[0]].type].Abilities & UA_FANTASTIC) != 0)
    )
    {
        for(itr = 0; itr < troop_count; itr++)
        {
            battle_units[itr].controller_idx = NEUTRAL_PLAYER_IDX;
            battle_units[itr].status = 0;  // Unit_Active
            battle_units[itr].unit_idx = troops[itr];
        }

        _combat_total_unit_count = troop_count;

        _active_battle_unit = CTY_RampageVictory();

        for(itr = 0; itr < troop_count; itr++)
        {
            Kill_Unit(troops[itr], 0);
        }

    }


    CMB_Population_Lost = 0;

    if(Stack_Owner == HUMAN_PLAYER_IDX)
    {
        CMB_ScrollMsg_Type = 1;  // victory
    }

    if(City_Owner == HUMAN_PLAYER_IDX)
    {
        CMB_ScrollMsg_Type = 12; // city lost
    }

    CMB_Population_Lost = 0;
    CMB_Buildings_Lost = 0;

    if(
        (Stack_Owner == HUMAN_PLAYER_IDX)
        ||
        (City_Owner == HUMAN_PLAYER_IDX)
    )
    {
        Combat_Results_Scroll();
    }

}

// WZD o124p18
// drake178: AI_FightorFlight()
/*
; decides whether the AI will try to flee from the
; battle, which it will only if its situation seems
; hopeless, and it either has at least one hero on the
; field or otherwise all of the units are builders
; returns 1 if the AI picks flight, or 0 if fight
*/
/*



*/
int16_t AI_FightorFlight__STUB(int16_t player_idx)
{

    /* HACK */ return ST_FALSE;

}


// WZD o124p19
// drake178: AI_Raze_Decision()

// WZD o124p20
int16_t Raze_City_Prompt(char * message)
{
    int16_t Keep_City;
    int16_t Label_Ctrl_Index;
    int16_t Esc_Hotkey_Index;
    int16_t Raze_Button_Index;
    int16_t NoRaze_Button_Index;
    int16_t input_field_idx;
    int16_t Dialog_Box_Height;
    int16_t para_height;  // _SI_
    int16_t leave_screen;  // _DI_

    Save_Alias_Colors();

    Set_Font_Colors_15(0, &COL_Dialog_Text[0]);

    // DONT EMM_Sandbox2VGAFILEH();


    // RESOURCE.LBX, 000  CONFMBAK
    // RESOURCE.LBX, 001  CONFMBAK
    confirmation_background_top_seg = LBX_Reload(resource_lbx_file__ovr124, 0, _screen_seg);
    confirmation_background_bottom_seg = LBX_Reload_Next(resource_lbx_file__ovr124, 1, _screen_seg);

    // COMPIX.LBX, 081  BASE       no raze button
    // COMPIX.LBX, 082  BASE       raze button
    confirmation_button_yes_seg = LBX_Reload_Next(compix_lbx_file__ovr124, 81, _screen_seg);
    confirmation_button_no_seg = LBX_Reload_Next(compix_lbx_file__ovr124, 82, _screen_seg);


    Copy_On_To_Off_Page();

    message_box_text = message;

    Set_Font_Style(4, 4, 15, ST_NULL);

    para_height = Get_Paragraph_Max_Height(166, message);

    Dialog_Box_Height = (34 + para_height);

    message_box_x = (68 + confirmation_box_x_offset);

    message_box_y = (confirmation_box_y_offset + ((SCREEN_HEIGHT - Dialog_Box_Height) / 2));

    Clear_Fields();

    NoRaze_Button_Index = Add_Button_Field((message_box_x + 101), (message_box_y + para_height + 15), "", confirmation_button_yes_seg, 'N', ST_UNDEFINED);

    Raze_Button_Index = Add_Button_Field((message_box_x + 18), (message_box_y + para_height + 15), "", confirmation_button_no_seg, 'R', ST_UNDEFINED);

    Label_Ctrl_Index = Add_Hidden_Field(message_box_x, message_box_y, (message_box_x + 185), (message_box_y + 63), ST_NULL, ST_UNDEFINED);

    Esc_Hotkey_Index = Add_Hidden_Field(0, 0, SCREEN_XMAX, SCREEN_YMAX, '\x1B', ST_UNDEFINED);

    Assign_Auto_Function(Raze_City_Prompt_Draw, 1);

    leave_screen = ST_FALSE;

    while(leave_screen == ST_FALSE)
    {
        input_field_idx = abs(Get_Input());

        if(
            (input_field_idx == ST_UNDEFINED)
            ||
            (input_field_idx == Raze_Button_Index)
        )
        {
            leave_screen = ST_TRUE;
            Keep_City = ST_FALSE;
        }

        if(input_field_idx == NoRaze_Button_Index)
        {
            leave_screen = ST_TRUE;
            Keep_City = ST_TRUE;
        }

        Raze_City_Prompt_Draw();
        PageFlip_FX();
    }

    Deactivate_Auto_Function();
    Clear_Fields();
    Restore_Alias_Colors();
    Reset_Window();
    // DONT  EMM_VGAFILEH2Sandbox()

    return Keep_City;
}


// WZD o124p21
// drake178: ; byte-identical to GUI_DrawConfrmDialog()  AKA Confirmation_Box_Draw()  GENDRAW.C
/*
    === Confirmation_Box_Draw()
        presumably copied to this overlay to avoid swapping
*/
void Raze_City_Prompt_Draw(void)
{
    int16_t paragraph_height;

    Set_Font_Style(4, 4, 15, ST_NULL);

    paragraph_height = Get_Paragraph_Max_Height(166, message_box_text);

    Set_Page_Off();

    Set_Window(0, 0, SCREEN_XMAX, (message_box_y + paragraph_height + 12));

    Clipped_Draw(message_box_x, message_box_y, confirmation_background_top_seg);

    Reset_Window();

    FLIC_Draw(message_box_x, (message_box_y + paragraph_height + 10), confirmation_background_bottom_seg);

    Set_Font_Colors_15(4, &COL_ConfirmShadows[0]);
    Set_Font_Style(4, 15, 15, ST_NULL);
    Print_Paragraph((message_box_x + 10), (message_box_y + 10), 166, message_box_text, 0);  // print_type 0: Print Left Aligned
    Print_Paragraph((message_box_x +  9), (message_box_y + 10), 166, message_box_text, 0);  // print_type 0: Print Left Aligned

    Set_Alias_Color(18);
    Set_Font_Colors_15(4, &COL_Dialog_Text[0]);
    Set_Font_Style(4, 4, 15, ST_NULL);
    Print_Paragraph((message_box_x +  9), (message_box_y +  9), 166, message_box_text, 0);  // print_type 0: Print Left Aligned

}



/*
    WIZARDS.EXE  ovr139
*/

// WZD ovr139p01
// drake178: AI_SelectCmbtSpell()
//AI_SelectCmbtSpell()

// WZD ovr139p02
// drake178: AI_EvaluateCmbtSpell()
//AI_EvaluateCmbtSpell()

// WZD ovr139p03
// drake178: AI_GetThreat_BU()
/*
; calculates and returns an arbitrary battle unit
; combat value based on attack attributes, defense,
; and remaining hit points and figures
*/
/*

Get_Effective_Melee_Strength()
+
Get_Effective_Hits()
+
Get_Effective_Ranged_Strength()

*/
int16_t Effective_Battle_Unit_Strength(int16_t battle_unit_idx)
{
    int16_t short_range_strength = 0;
    int16_t efective_strength = 0;  // _DI_


    if(battle_units[battle_unit_idx].ranged_type >= srat_Thrown)
    {

        short_range_strength = battle_units[battle_unit_idx].ranged;

        efective_strength = Get_Effective_Melee_Strength(battle_units[battle_unit_idx].melee, short_range_strength, battle_units[battle_unit_idx].Cur_Figures, (battle_units[battle_unit_idx].Attack_Flags | battle_units[battle_unit_idx].Melee_ATK_Flags), battle_units[battle_unit_idx].ranged_type);

    }
    else
    {

        short_range_strength = 0;

        efective_strength = Get_Effective_Melee_Strength(battle_units[battle_unit_idx].melee, short_range_strength, battle_units[battle_unit_idx].Cur_Figures, (battle_units[battle_unit_idx].Attack_Flags | battle_units[battle_unit_idx].Melee_ATK_Flags), ST_UNDEFINED);

    }

    efective_strength += Get_Effective_Hits(((battle_units[battle_unit_idx].hits * battle_units[battle_unit_idx].Cur_Figures) - battle_units[battle_unit_idx].TopFig_Dmg), battle_units[battle_unit_idx].defense);
          
    if(Battle_Unit_Has_Ranged_Attack(battle_unit_idx) != ST_FALSE)
    {

        efective_strength += Get_Effective_Ranged_Strength(battle_units[battle_unit_idx].ranged, battle_units[battle_unit_idx].Cur_Figures, (battle_units[battle_unit_idx].Attack_Flags | battle_units[battle_unit_idx].Ranged_ATK_Flags));

    }

    return efective_strength;

}


// WZD ovr139p04
// drake178: AI_SetCombatRealms()
//AI_SetCombatRealms()

// WZD ovr139p05
// drake178: AI_UnitThreatRealms()
//AI_UnitThreatRealms()

// WZD ovr139p06
// drake178: AI_CombatSpellList()
//AI_CombatSpellList()



/*
    WIZARDS.EXE  ovr153
*/

// WZD ovr153p01
// drake178: UU_sub_DC990()
void NX_IDK_CombatInit_Tactical(int16_t wx, int16_t wy, int16_t wp)
{
    CMB_BaseAllocs__WIP();
    CMB_Terrain_Init__WIP(wx, wy, wp);
}


// WZD ovr153p02
// drake178: CMB_DrawMap()
/*
; draws the combat map into the current draw frame,
; starting with the background saved in VGA frame 3,
; and drawing all tile animations, roads, and combat
; entities on top of that; includes the bottom UI
; background, but not the actual controls
*/
/*

*/
void CMB_DrawMap__WIP(void)
{
    int16_t Road_Flags = 0;
    int16_t Set_Base_2 = 0;
    int16_t Set_Base_1 = 0;
    int16_t cgy = 0;
    int16_t cgx = 0;
    int16_t screen_y = 0;
    int16_t screen_x = 0;
    int16_t IDK_base_cgc1 = 0;
    int16_t IDK_base_cgc2 = 0;
    int16_t itr_x = 0;
    int16_t itr_y = 0;  // _SI_
    int16_t battlefield_terrain = 0;  // _DI_

    Copy_Back_To_Off();  // 'combat background' from Combat_Screen_Compose_Background()

    Combat_Figure_Compose_USEFULL();

    Set_Page_Off();

    Set_Window(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, COMBAT_MAP_YMAX);

    for(itr_y = 0; itr_y < 22; itr_y++)
    {
        CALC_BASE_CGC2;
        CALC_BASE_CGC1;
        for(itr_x = 0; itr_x < 11; itr_x++)
        {
            CALC_CGX;
            CALC_CGY;
            CALC_SCREEN_X;
            CALC_SCREEN_Y;
            if(
                (cgx >= COMBAT_GRID_XMIN)
                &&
                (cgx < COMBAT_GRID_XMAX)
                &&
                (cgy >= COMBAT_GRID_YMIN)
                &&
                (cgy < COMBAT_GRID_YMAX)
            )
            {
                // ¿ Earth to Mud ?
                if(battlefield->Tile_Mud[((cgy * COMBAT_GRID_WIDTH) + cgx)] != 0)
                {
                    STU_DEBUG_BREAK();
                    FLIC_Set_CurrentFrame(IMG_CMB_Mud, ((cgx + cgy + CMB_MudAnimStage) % 8));
                    Clipped_Draw(screen_x, screen_y, IMG_CMB_Mud);
                }
                else
                {
                    battlefield_terrain = battlefield->terrain_type[((cgy * COMBAT_GRID_WIDTH) + cgx)];
                    if(battlefield_terrain >= 56)
                    {
                        if(battlefield->wp == 0)
                        {
// push    [CMB_WaterAnimStage]            ; frame_num
// mov     bx, _DI_battlefield_terrain
// shl     bx, 1
// push    (IMG_CMB_RiverTile@-70h)[bx]    ; pict_seg
// call    FLIC_Set_CurrentFrame
// pop     cx
// pop     cx
// mov     bx, _DI_battlefield_terrain
// shl     bx, 1
// push    (IMG_CMB_RiverTile@-70h)[bx]    ; array of 12 appended reserved EMM headers in
//                                         ; GFX_Swap_Seg; the first 6 are empty, the other 6
//                                         ; have 5 frame animations for rivers
                        }
                        else
                        {
// push    [CMB_WaterAnimStage]
// mov     bx, _DI_battlefield_terrain
// shl     bx, 1
// push    (IMG_CMB_RivrNULLs@-70h)[bx]    ; pict_seg
// call    FLIC_Set_CurrentFrame
// pop     cx
// pop     cx
// mov     bx, _DI_battlefield_terrain
// shl     bx, 1
// push    (IMG_CMB_RivrNULLs@-70h)[bx]    ; picture
                        }

//                         Clipped_Draw(screen_x, screen_y);

                    }
                    else if(
                        (battlefield_terrain >= 48)
                        &&
                        (battlefield_terrain < 52)
                    )
                    {
                        if(battlefield->wp == 0)
                        {
// push    [CMB_WaterAnimStage]            ; frame_num
// mov     bx, _DI_battlefield_terrain
// shl     bx, 1
// push    (IMG_CMB_OceanTile@-60h)[bx]    ; pict_seg
// call    FLIC_Set_CurrentFrame
// pop     cx
// pop     cx
// mov     bx, _DI_battlefield_terrain
// shl     bx, 1
// push    (IMG_CMB_OceanTile@-60h)[bx]    ; array of 4 appended reserved EMM headers in
//                                         ; GFX_Swap_Seg, each with a 5 frame animation
                        }
                        else
                        {
// push    [CMB_WaterAnimStage]
// mov     bx, _DI_battlefield_terrain
// shl     bx, 1
// push    (IMG_CMB_ChaosOcn@-60h)[bx]     ; pict_seg
// call    FLIC_Set_CurrentFrame
// pop     cx
// pop     cx
// mov     bx, _DI_battlefield_terrain
// shl     bx, 1
// push    (IMG_CMB_ChaosOcn@-60h)[bx]     ; picture
                        }
// push    [CMB_WaterAnimStage]            ; frame_num
// mov     bx, _DI_battlefield_terrain
// shl     bx, 1
// push    (IMG_CMB_Cloud@-68h)[bx]        ; pict_seg
// call    FLIC_Set_CurrentFrame
// pop     cx
// pop     cx
// mov     bx, _DI_battlefield_terrain
// shl     bx, 1
// push    (IMG_CMB_Cloud@-68h)[bx]        ; picture
// push    [bp+screen_y]                        ; y
// push    [bp+screen_x]                       ; x
// call    Clipped_Draw
                    }
                    else if(
                        (battlefield_terrain >= 52)
                        &&
                        (battlefield_terrain < 56)
                    )
                    {
// push    [CMB_WaterAnimStage]            ; frame_num
// mov     bx, _DI_battlefield_terrain
// shl     bx, 1
// push    (IMG_CMB_Cloud@-68h)[bx]        ; pict_seg
// call    FLIC_Set_CurrentFrame
// pop     cx
// pop     cx
// mov     bx, _DI_battlefield_terrain
// shl     bx, 1
// push    (IMG_CMB_Cloud@-68h)[bx]        ; picture
// push    [bp+screen_y]                        ; y
// push    [bp+screen_x]                       ; x
// call    Clipped_Draw
                    }
                    if(
                        (battlefield->Central_Structure == CS_SorceryNode)
                        &&
                        (cgx == 6)
                        &&
                        (cgy == 11)
                    )
                    {
                        STU_DEBUG_BREAK();
                        Combat_Grid_Screen_Coordinates(6, 11, 0, 0, &screen_x, &screen_y);
                        screen_x -= 46;
                        screen_y -= 15;
                        FLIC_Set_CurrentFrame(IMG_CMB_SorcNode, CMB_CNodeAnimStage);
                        Clipped_Draw(screen_x, screen_y, IMG_CMB_SorcNode);
                        screen_x += 46;
                        screen_y += 15;
                    }
                    if(
                        (battlefield->Central_Structure == CS_ChaosNode)
                        &&
                        (cgx == 6)
                        &&
                        (cgy == 11)
                    )
                    {
                        STU_DEBUG_BREAK();
                        Combat_Grid_Screen_Coordinates(6, 11, 0, 0, &screen_x, &screen_y);
                        screen_x -= 10;
                        screen_y -= 2;
                        FLIC_Draw(screen_x, screen_y, IMG_CMB_Volcano[(CMB_CNodeAnimStage % 8)]);
                        screen_x += 10;
                        screen_y += 2;
                    }
                    Road_Flags = battlefield->Tile_Road[((cgy * COMBAT_GRID_WIDTH) + cgx)];
                    if(Road_Flags != 0)
                    {
                        STU_DEBUG_BREAK();  // don't recall why I put this here... did I just not do roads?
                        if(
                            (Road_Flags & 0x80) != 0
                            &&
                            (Road_Flags & 0x01) != 0
                        )
                        {
                            FLIC_Set_CurrentFrame(IMG_CMB_FlotIsle, CMB_RoadAnimStage);
                            Clipped_Draw((screen_x - 32), (screen_y - 48), IMG_CMB_FlotIsle);
                        }
                        else
                        {
                            if(CMB_Enchanted_Roads == ST_TRUE)
                            {
                                Set_Base_1 = 14;
                            }
                            else
                            {
                                Set_Base_1 = 0;
                            }
                            if((Road_Flags & 0x01) != 0)
                            {
                                Set_Base_2 = 0;
                            }
                            else
                            {
                                Set_Base_2 = 7;
                            }
                            if((Road_Flags & 0x02) != 0)
                            {
                                FLIC_Set_CurrentFrame(IMG_CMB_RoadTiles[(0 + (Set_Base_1 + Set_Base_2))], CMB_RoadAnimStage);
                                Clipped_Draw(screen_x, screen_y, IMG_CMB_RoadTiles[(0 + (Set_Base_1 + Set_Base_2))]);
                            }
                            if((Road_Flags & 0x04) != 0)
                            {
                                FLIC_Set_CurrentFrame(IMG_CMB_RoadTiles[(1 + (Set_Base_1 + Set_Base_2))], CMB_RoadAnimStage);
                                Clipped_Draw(screen_x, screen_y, IMG_CMB_RoadTiles[(1 + (Set_Base_1 + Set_Base_2))]);
                            }
                            if((Road_Flags & 0x08) != 0)
                            {
                                FLIC_Set_CurrentFrame(IMG_CMB_RoadTiles[(2 + (Set_Base_1 + Set_Base_2))], CMB_RoadAnimStage);
                                Clipped_Draw(screen_x, screen_y, IMG_CMB_RoadTiles[(2 + (Set_Base_1 + Set_Base_2))]);
                            }
                            if((Road_Flags & 0x10) != 0)
                            {
                                FLIC_Set_CurrentFrame(IMG_CMB_RoadTiles[(3 + (Set_Base_1 + Set_Base_2))], CMB_RoadAnimStage);
                                Clipped_Draw(screen_x, screen_y, IMG_CMB_RoadTiles[(3 + (Set_Base_1 + Set_Base_2))]);
                            }
                            if((Road_Flags & 0x20) != 0)
                            {
                                FLIC_Set_CurrentFrame(IMG_CMB_RoadTiles[(4 + (Set_Base_1 + Set_Base_2))], CMB_RoadAnimStage);
                                Clipped_Draw(screen_x, screen_y, IMG_CMB_RoadTiles[(4 + (Set_Base_1 + Set_Base_2))]);
                            }
                            if((Road_Flags & 0x40) != 0)
                            {
                                FLIC_Set_CurrentFrame(IMG_CMB_RoadTiles[(5 + (Set_Base_1 + Set_Base_2))], CMB_RoadAnimStage);
                                Clipped_Draw(screen_x, screen_y, IMG_CMB_RoadTiles[(5 + (Set_Base_1 + Set_Base_2))]);
                            }
                            if(
                                (battlefield->Central_Structure == CS_Outpost)
                                &&
                                (cgx == 6)
                                &&
                                (cgy == 11)
                            )
                            {
                                Clipped_Draw(screen_x, screen_y, IMG_CMB_DirtTile);
                            }
                            // ; BUG: will fail to show Flying Fortress graphics on outposts
                            if(
                                (
                                    (battlefield->Central_Structure == CS_City)
                                    ||
                                    (battlefield->Central_Structure == CS_Fortress)
                                )
                                &&
                                (cgx == 8)
                                &&
                                (cgy == 13)
                            )
                            {
                                if(battlefield->city_enchantments[FLYING_FORTRESS] != 0)
                                {
                                    Clipped_Draw((screen_x - 48), (screen_y - 48), IMG_CMB_Cloud);
                                }
                                else
                                {
                                    Clipped_Draw((screen_x - 48), (screen_y - 48), IMG_CMB_RoadGrid);
                                }
                            }
                            if(
                                (CMB_Chasm_Anim == 1)
                                &&
                                (CMB_Chasm_Anim_X == cgx)
                                &&
                                (CMB_Chasm_Anim_Y == cgy)
                            )
                            {
                                FLIC_Set_CurrentFrame(IMG_GUI_Chasm, CMB_ChasmAnimStage);
                                Clipped_Draw(screen_x, (screen_y - 18), IMG_GUI_Chasm);
                            }
                        }
                    }
                }
            }
        }
    }


    /*
        BEGIN:  Draw Outlines  (red,blue (active unit,scanned square))
    */
    {
        for(itr_y = 0; itr_y < 22; itr_y++)
        {
            CALC_BASE_CGC2;
            CALC_BASE_CGC1;
            for(itr_x = 0; itr_x < 11; itr_x++)
            {
                CALC_CGX;
                CALC_CGY;
                CALC_SCREEN_X;
                CALC_SCREEN_Y;
                if(
                    (frame_scanned_flag == ST_TRUE)
                    &&
                    (cgx == frame_scanned_cgx)
                    &&
                    (cgy == frame_scanned_cgy)
                )
                {
                    FLIC_Set_CurrentFrame(frame_scanned_seg, frame_anim_cycle);
                    Clipped_Draw(screen_x, screen_y, frame_scanned_seg);
                }
                if(
                    (frame_active_flag == ST_TRUE)
                    &&
                    (cgx == frame_active_cgx)
                    &&
                    (cgy == frame_active_cgy)
                )
                {
                    FLIC_Set_CurrentFrame(frame_active_seg, frame_anim_cycle);
                    Clipped_Draw(screen_x, screen_y, frame_active_seg);
                }
            }
        }
    }
    /*
        END:  Draw Outlines  (red,blue (active unit,scanned square))
    */


    CMB_RoadAnimStage = ((CMB_RoadAnimStage + 1) % 5);

    CMB_WaterAnimStage = ((CMB_WaterAnimStage + 1) % 5);

    CMB_CNodeAnimStage = ((CMB_CNodeAnimStage + 1) % 8);

    // ; 0 or 1, controls whether something else is stepped
    if(G_CMB_MWallAnimSkip == 0)
    {

        G_CMB_MWallAnimStage = ((G_CMB_MWallAnimStage + 1) % 4);

    }

    G_CMB_MWallAnimSkip = (1 - G_CMB_MWallAnimSkip);

    CMB_CurseAnimStage++;

    if(CMB_CurseAnimStage > 30000)
    {
        CMB_CurseAnimStage = 0;
    }

    if(CMB_MoveAnimDir == 0)
    {

        CMB_MoveAnimFrame++;

        if(CMB_MoveAnimFrame == 3)
        {

            CMB_MoveAnimDir = 1;

            CMB_MoveAnimFrame = 1;

        }

    }
    else
    {

        CMB_MoveAnimFrame--;

        if(CMB_MoveAnimFrame == -1)
        {

            CMB_MoveAnimDir = 0;

            CMB_MoveAnimFrame = 1;

        }

    }

    CMB_BaseAnimFrame = ((CMB_BaseAnimFrame + 1) % 3);

    if(CMB_UnitATKAnimFrame == 1)
    {
        CMB_UnitATKAnimFrame = 3;
    }
    else
    {
        CMB_UnitATKAnimFrame = 1;
    }

    if(CMB_UnitDEFAnimFrame == 1)
    {
        CMB_UnitDEFAnimFrame = 3;
    }
    else
    {
        CMB_UnitDEFAnimFrame = 1;
    }


    CMB_DrawEntities__WIP();  // OON XREF


    Reset_Window();

}


// WZD ovr153p03
// drake178: CMB_DrawEntities()
/*
; draws all combat entities into the current draw frame
; from farthest to nearest in two passes, with curses
; (2) and projectiles (3) being drawn on top of the
; other entity types (0, 1, and 4)
*/
/*



*/
void CMB_DrawEntities__WIP(void)
{
    int16_t combat_grid_cell_y_offset = 0;
    int16_t combat_grid_cell_x_offset = 0;
    int16_t combat_grid_cell_y = 0;
    int16_t combat_grid_cell_x = 0;
    int16_t Blood_Offset_X = 0;
    int16_t Blood_Offset_Y = 0;
    int16_t itr = 0;  // _DI_
    int16_t combat_grid_entity_idx = 0;  // _SI_
    struct s_COMBAT_ENTITY * ptr_combat_grid_entity = 0;  // DNE in Dasm
    SAMB_ptr pict_seg = 0;  // DNE in Dasm

    for(itr = 0; itr < combat_grid_entity_count; itr++)
    {

        // get combat_grid_entity_idx, in order of draw_order_value
        combat_grid_entity_idx = combat_grid_entities_draw_order[itr];

        if(combat_grid_entities[combat_grid_entity_idx].entity_type == 1)  /* Battle Unit Figure */
        {

            if(combat_grid_entities[combat_grid_entity_idx].Blood_Amt == 0)
            {
                // Draw_Picture_Windowed(
                //     (combat_grid_entities[combat_grid_entity_idx].draw_x - combat_grid_entities[combat_grid_entity_idx].draw_x_shift),
                //     (combat_grid_entities[combat_grid_entity_idx].draw_y - combat_grid_entities[combat_grid_entity_idx].draw_y_shift),
                //     battle_unit_picts_seg[combat_grid_entities[combat_grid_entity_idx].index]
                // );
                ptr_combat_grid_entity = &combat_grid_entities[combat_grid_entity_idx];
                // index into battle unit figure pictures array  [2 * MAX_STACK]
                // ¿ all 8 entity figures should have the same index ?
                // 
                // Draw_Picture_Windowed(
                //     (combat_grid_entities[combat_grid_entity_idx].draw_x - combat_grid_entities[combat_grid_entity_idx].draw_x_shift),
                //     (combat_grid_entities[combat_grid_entity_idx].draw_y - combat_grid_entities[combat_grid_entity_idx].draw_y_shift),
                //     battle_unit_picts_seg[ptr_combat_grid_entity->index]
                // );
                pict_seg = battle_unit_picts_seg[combat_grid_entities[combat_grid_entity_idx].index];
                Draw_Picture_Windowed(
                    (combat_grid_entities[combat_grid_entity_idx].draw_x - combat_grid_entities[combat_grid_entity_idx].draw_x_shift),
                    (combat_grid_entities[combat_grid_entity_idx].draw_y - combat_grid_entities[combat_grid_entity_idx].draw_y_shift),
                    pict_seg
                );

            }
            else
            {
                if(combat_grid_entities[combat_grid_entity_idx].Blood_Frame != 3)
                {

                    Draw_Picture_Windowed(
                        (combat_grid_entities[combat_grid_entity_idx].draw_x - combat_grid_entities[combat_grid_entity_idx].draw_x_shift),
                        (combat_grid_entities[combat_grid_entity_idx].draw_y - combat_grid_entities[combat_grid_entity_idx].draw_y_shift),
                        battle_unit_picts_seg[combat_grid_entities[combat_grid_entity_idx].index]
                    );

                    if(combat_grid_entities[combat_grid_entity_idx].Blood_Frame >= 0)
                    {

                        FLIC_Set_CurrentFrame(
                            IMG_CMB_Blood[(combat_grid_entities[combat_grid_entity_idx].Blood_Amt - 1)],
                            combat_grid_entities[combat_grid_entity_idx].Blood_Frame
                        );

                        Blood_Offset_Y = 11;
                        Blood_Offset_X = 3;

                        Clipped_Draw(
                            (combat_grid_entities[combat_grid_entity_idx].draw_x - Blood_Offset_X), 
                            (combat_grid_entities[combat_grid_entity_idx].draw_y - Blood_Offset_Y), 
                            IMG_CMB_Blood[(combat_grid_entities[combat_grid_entity_idx].Blood_Amt - 1)]
                        );

                    }

                }

            }

        }
        else if(combat_grid_entities[combat_grid_entity_idx].entity_type == 0)
        {

            FLIC_Set_CurrentFrame(
                combat_grid_entities[combat_grid_entity_idx].pict_seg,
                combat_grid_entities[combat_grid_entity_idx].frame_num
            );

            Clipped_Draw(
                (combat_grid_entities[combat_grid_entity_idx].draw_x - combat_grid_entities[combat_grid_entity_idx].draw_x_shift),
                (combat_grid_entities[combat_grid_entity_idx].draw_y - combat_grid_entities[combat_grid_entity_idx].draw_y_shift),
                combat_grid_entities[combat_grid_entity_idx].pict_seg
            );

        }
        else if(combat_grid_entities[combat_grid_entity_idx].entity_type == 4)
        {

            EMM_TILEX_Init__HACK();

            Clipped_Draw(
                (combat_grid_entities[combat_grid_entity_idx].draw_x - combat_grid_entities[combat_grid_entity_idx].draw_x_shift),
                (combat_grid_entities[combat_grid_entity_idx].draw_y - combat_grid_entities[combat_grid_entity_idx].draw_y_shift),
                combat_grid_entities[combat_grid_entity_idx].pict_seg
            );

        }

    }


    /*
        ~== draw next layer up
        ¿ projectiles ?
    */
// REDO     for(itr_combat_grid_entities = 0; itr_combat_grid_entities < combat_grid_entity_count; itr_combat_grid_entities++)
// REDO     {
// REDO 
// REDO         if(combat_grid_entities[combat_grid_entity_idx].entity_type == 3)
// REDO         {
// REDO 
// REDO 
// REDO             FLIC_Set_CurrentFrame(
// REDO                 combat_grid_entities[combat_grid_entity_idx].pict_seg,
// REDO                 CMB_ProjectileFrame
// REDO             );
// REDO 
// REDO             Clipped_Draw(
// REDO                 (combat_grid_entities[combat_grid_entity_idx].draw_x - combat_grid_entities[combat_grid_entity_idx].draw_x_shift),
// REDO                 (combat_grid_entities[combat_grid_entity_idx].draw_y - combat_grid_entities[combat_grid_entity_idx].draw_y_shift),
// REDO                 combat_grid_entities[combat_grid_entity_idx].pict_seg
// REDO             );
// REDO 
// REDO         }
// REDO         else if(combat_grid_entities[combat_grid_entity_idx].entity_type == 2)
// REDO         {
// REDO 
// REDO             FLIC_Set_CurrentFrame(
// REDO                 combat_grid_entities[combat_grid_entity_idx].pict_seg,
// REDO                 CMB_CurseAnimStage
// REDO             );
// REDO 
// REDO             Clipped_Draw(
// REDO                 (combat_grid_entities[combat_grid_entity_idx].draw_x - combat_grid_entities[combat_grid_entity_idx].draw_x_shift),
// REDO                 (combat_grid_entities[combat_grid_entity_idx].draw_y - combat_grid_entities[combat_grid_entity_idx].draw_y_shift),
// REDO                 combat_grid_entities[combat_grid_entity_idx].pict_seg
// REDO             );
// REDO 
// REDO         }
// REDO 
// REDO     }


    Screen_To_Combat_Grid_Cell_X_And_Offset(Pointer_X(), Pointer_Y(), &combat_grid_cell_x, &combat_grid_cell_x_offset);

    Screen_To_Combat_Grid_Cell_Y_And_Offset(Pointer_X(), Pointer_Y(), &combat_grid_cell_y, &combat_grid_cell_y_offset);

}


// WZD ovr153p04
// drake178: sub_DD536()

// WZD ovr153p05
// drake178: CMB_SpawnTrees()

// WZD ovr153p06
// drake178: CMB_SpawnRocks()

// WZD ovr153p07
// drake178: CMB_SpawnStructures()

// WZD ovr153p08
// drake178: CMB_SpawnStoneWall()

// WZD ovr153p09
// drake178: CMB_SpawnDarkWall()

// WZD ovr153p10
// drake178: CMB_SpawnFireWall()

// WZD ovr153p11
// drake178: CMB_SpawnVortices()

// WZD ovr153p12
// drake178: CMB_SpawnFigure()
/*
; creates a combat entity corresponding to a figure of
; a battle unit
*/
/*
    battle unit specific wrapper for Create_Entity()
    
~ Battle_Unit_Figure_Position()
~ UnitView.C  Unit_Figure_Position()

CMB_SpawnFigure__WIP(battle_units[itr].bufpi, battle_units[itr].cgx, battle_units[itr].cgy, battle_units[itr].target_cgx, battle_units[itr].target_cgy, battle_units[itr].MoveStage, itr_figures, unit_figure_maximum, battle_units[itr].controller_idx, battle_units[itr].outline_magic_realm, battle_units[itr].Blood_Amount, battle_units[itr].Moving, battle_units[itr].Atk_FigLoss, 0);

bufpi
    passed through
uu_frame_num
    passed through
SrcBld
    hard-coded to 0
*/
void CMB_SpawnFigure__WIP(int64_t bufpi, int16_t cgx, int16_t cgy, int16_t target_cgx, int16_t target_cgy, int16_t MoveStage, int16_t current_figure, int16_t figure_count, int16_t controller_idx, int16_t outline_magic_realm, int16_t BldAmt, int16_t uu_frame_num, int16_t LostFigs, int16_t SrcBld)
{
    int16_t Blood_Frame = 0;
    int16_t figure_set_idx = 0;
    int16_t Prev_Blood_Frame = 0;
    int16_t fig_y = 0;
    int16_t draw_y = 0;
    int16_t target_screen_y = 0;
    int16_t position_screen_y = 0;
    int16_t draw_x = 0;
    int16_t target_screen_x = 0;
    int16_t position_screen_x = 0;
    int16_t fig_x = 0;  // _DI_

    Combat_Grid_Screen_Coordinates(cgx, cgy, 0, 0, &position_screen_x, &position_screen_y);

    Combat_Grid_Screen_Coordinates(target_cgx, target_cgy, 0, 0, &target_screen_x, &target_screen_y);

    /*
        BEGIN:  ~== Unit_Figure_Position()
    */
//     switch(figure_count)
//     {
//         case 1:
//         {
//             fig_x = 1;
//             fig_y = 8;
//         } break;
//         case 2:
//         {
//             switch(current_figure)
//             {
//                 case 0:  { fig_x = -7; fig_y =  9; } break;
//                 default: { fig_x =  7; fig_y =  9; } break;
//             }
//         } break;
//         case 3:
//         {
//             switch(current_figure)
//             {
//                 case 0:  { fig_x =  0; fig_y =  4; } break;
//                 case 1:  { fig_x = -6; fig_y = 10; } break;
//                 default: { fig_x =  7; fig_y = 10; } break;
//             }
//         } break;
//         case 4:
//         {
//             switch(current_figure)
//             {
//                 case 0:  { fig_x =  1; fig_y =  4; } break;
//                 case 1:  { fig_x = -7; fig_y =  8; } break;
//                 case 2:  { fig_x =  8; fig_y =  8; } break;
//                 default: { fig_x =  1; fig_y = 11; } break;
//             }
//         } break;
//         case 5:
//         {
//             switch(current_figure)
//             {
//                 case 0:  { fig_x =  1; fig_y =  4; } break;
//                 case 1:  { fig_x = -7; fig_y =  8; } break;
//                 case 2:  { fig_x =  1; fig_y =  8; } break;
//                 case 3:  { fig_x =  8; fig_y =  8; } break;
//                 default: { fig_x =  1; fig_y = 11; } break;
//             }
//         } break;
//         case 6:
//         {
//             switch(current_figure)
//             {
//                 case 0:  { fig_x =   1; fig_y =  4; } break;
//                 case 1:  { fig_x =   4; fig_y =  7; } break;
//                 case 2:  { fig_x =  -8; fig_y =  8; } break;
//                 case 3:  { fig_x =   9; fig_y =  8; } break;
//                 case 4:  { fig_x =  -3; fig_y =  9; } break;
//                 default: { fig_x =   1; fig_y = 11; } break;
//             }
//         } break;
//         case 7:
//         {
//             switch(current_figure)
//             {
//                 case 0:  { fig_x =  1; fig_y =  4; } break;
//                 case 1:  { fig_x =  6; fig_y =  6; } break;
//                 case 2:  { fig_x = -8; fig_y =  8; } break;
//                 case 3:  { fig_x =  1; fig_y =  8; } break;
//                 case 4:  { fig_x = 10; fig_y =  8; } break;
//                 case 5:  { fig_x = -3; fig_y = 11; } break;
//                 default: { fig_x =  1; fig_y = 11; } break;
//             }
//         } break;
//         case 8:
//         {
//             switch(current_figure)
//             {
//                 case 0:  { fig_x =   1; fig_y =  4; } break;
//                 case 1:  { fig_x =   6; fig_y =  6; } break;
//                 case 2:  { fig_x =  -2; fig_y =  7; } break;
//                 case 3:  { fig_x =  -8; fig_y =  8; } break;
//                 case 4:  { fig_x =  10; fig_y =  8; } break;
//                 case 5:  { fig_x =   3; fig_y =  9; } break;
//                 case 6:  { fig_x =  -3; fig_y = 11; } break;
//                 default: { fig_x =   1; fig_y = 11; } break;
//             }
//         } break;
//     }
    BATTLE_UNIT_FIGURE_POSITION

    /*
        END:  ~== Unit_Figure_Position()
    */

    BATTLE_UNIT_FACING_DRECTION

    draw_x = (((((target_screen_x - position_screen_x) * MoveStage) / 8) + position_screen_x) + fig_x);
    draw_y = (((((target_screen_y - position_screen_y) * MoveStage) / 8) + position_screen_y) + fig_y);

    /*
        BEGIN:  ¿ Blood_Frame & BldAmt ?
    */
    {
        if(
            (BldAmt > 0)
            &&
            (current_figure < LostFigs)
        )
        {
            if(SrcBld == 0)
            {
                Prev_Blood_Frame = CMB_BloodFrames[current_figure];
                CMB_BloodFrames[current_figure] += 1;
                if(CMB_BloodFrames[current_figure] > 5)
                {
                    CMB_BloodFrames[current_figure] = 5;
                }
            }
            else
            {
                Prev_Blood_Frame = CMB_BloodFrames[(10 + current_figure)];
                CMB_BloodFrames[(10 + current_figure)] += 1;
                if(CMB_BloodFrames[(10 + current_figure)] > 5)
                {
                    CMB_BloodFrames[(10 + current_figure)] = 5;
                }
            }
            Prev_Blood_Frame -= 2;
            Blood_Frame = Prev_Blood_Frame;
            if(
                ((LostFigs - 1) == current_figure)
                &&
                (Blood_Frame == 3)
            )
            {
                Blood_Frame = 2;
            }
        }
        else
        {
            Blood_Frame = 0;
            BldAmt = 0;
        }
    }
    /*
        END:  ¿ Blood_Frame & BldAmt ?
    */

    CMB_CreateEntity__WIP(draw_x, draw_y, bufpi, 13, 23, uu_frame_num, 1, controller_idx, figure_set_idx, outline_magic_realm, BldAmt, uu_frame_num, Blood_Frame);

}


// WZD ovr153p13
// drake178: CMB_SpawnUnitCurse()

// WZD ovr153p14
// drake178: CMB_EntitiesReset()
/*
; resets the combat entity table and rewrites the Z
; sorting array
*/
/*

256 values of {0,...,255}
*/
/*
    clears the entity count and defaults the draw order
*/
void Clear_Combat_Grid_Entities(void)
{
    int16_t itr = 0;  // _DX_
    combat_grid_entity_count = 0;
    for(itr = 0; itr < MAX_ENTITIES; itr++)
    {
        combat_grid_entities_draw_order[itr] = itr;
    }
}

// WZD ovr153p15
// drake178: CMB_ComposeFigureIMG()
/*
; draws a battle unit figure into the GUI_SmallWork_IMG
; allocation, complete with banner colors and unit
; enchantment outline (the image needs to be loaded
; into the FIGUREX EMM handle already)
*/
/*

Does this actually accomplish anything?

¿ ~ UnitView.C Draw_Unit_Figure() ?

*/
void USELESS_Combat_Figure_Load_Compose(int16_t bufpi, int16_t figure_set_idx, int16_t player_idx, int16_t enchantment_magic_realm, int16_t frame_num)
{
    SAMB_ptr * figure_pict_set_seg = 0;
    // SAMB_ptr temp_figure_pict_set_seg = 0;
    int16_t offset = 0;  // _SI_
    FIGUREX_OFFSET
    FIGUREX_MAP
    FIGUREX_POINTER
    FLIC_Set_CurrentFrame(figure_pict_set_seg[figure_set_idx], frame_num);
    Draw_Picture_To_Bitmap(figure_pict_set_seg[figure_set_idx], GfxBuf_2400B);
    Combat_Figure_Banner_Color(player_idx);
    Combat_Unit_Enchantment_Outline_Draw(enchantment_magic_realm);
}


// WZD ovr153p16
// drake178: CMB_SpawnProjectiles()



// segrax
// WZD ovr153p17
// drake178: CMB_GetFigDrawPos()
/*
XREF:
    BU_CreateImage__SEGRAX()
    CMB_SetProjectiles()

!!!!! ALL DIFFERENT !!!!!
¿ in-line in CMB_CreateEntities__WIP() ?
~ UnitView.C  Unit_Figure_Position()

*/
void Battle_Unit_Figure_Position(int16_t figure_count, int16_t current_figure, int16_t * figure_x, int16_t * figure_y)
{

    assert(figure_count   >  0);
    assert(figure_count   <= 8);
    assert(current_figure >= 0);
    assert(current_figure <  8);

    switch(figure_count - 1)
    {
        case 0:
        {
            *figure_x = 1;
            *figure_y = 8;
        } break;
        case 1:
        {
            switch(current_figure)
            {
                case 0:
                {
                    *figure_x = -7;
                    *figure_y = 9;
                } break;
                default:
                {
                    *figure_x = 7;
                    *figure_y = 9;
                } break;
            }
        } break;
        case 0x2:
            if (current_figure == 0) {
                *figure_x = 0;
                *figure_y = 4;
            }
            else if (current_figure == 1) {
                *figure_x = -6;
                *figure_y = 10;
            }
            else {
                *figure_x = 7;
                *figure_y = 10;
            }
            break;
        case 0x3:
            if (current_figure == 0) {
                *figure_x = 1;
                *figure_y = 4;
            }
            else if (current_figure == 1) {
                *figure_x = 8;
                *figure_y = 8;
            }
            else if (current_figure == 2) {
                *figure_x = 1;
                *figure_y = 12;
            }
            else {
                *figure_x = -7;
                *figure_y = 8;
            }
            break;
        case 0x4:
            if (current_figure == 0) {
                *figure_x = 1;
                *figure_y = 4;
            }
            else if (current_figure == 1) {
                *figure_x = 8;
                *figure_y = 8;
            }
            else if (current_figure == 2) {
                *figure_x = 1;
                *figure_y = 12;
            }
            else if (current_figure == 3) {
                *figure_x = -7;
                *figure_y = 8;
            }
            else {
                *figure_x = 1;
                *figure_y = 8;
            }
            break;
        case 0x5:
            if (current_figure == 0) {
                *figure_x = 1;
                *figure_y = 4;
            }
            else if (current_figure == 1) {
                *figure_x = 9;
                *figure_y = 8;
            }
            else if (current_figure == 2) {
                *figure_x = -8;
                *figure_y = 8;
            }
            else if (current_figure == 3) {
                *figure_x = 1;
                *figure_y = 12;
            }
            else if (current_figure == 4) {
                *figure_x = -3;
                *figure_y = 9;
            }
            else {
                *figure_x = 4;
                *figure_y = 7;
            }
            break;
        case 0x6:
            if (current_figure == 0) {
                *figure_x = 1;
                *figure_y = 4;
            }
            else if (current_figure == 1) {
                *figure_x = 10;
                *figure_y = 8;
            }
            else if (current_figure == 2) {
                *figure_x = -8;
                *figure_y = 8;
            }
            else if (current_figure == 3) {
                *figure_x = 1;
                *figure_y = 12;
            }
            else if (current_figure == 4) {
                *figure_x = 6;
                *figure_y = 6;
            }
            else if (current_figure == 5) {
                *figure_x = -3;
                *figure_y = 11;
            }
            else {
                *figure_x = 1;
                *figure_y = 8;
            }
            break;
        case 0x7:
            if (current_figure == 0) {
                *figure_x = 1;
                *figure_y = 4;
            }
            else if (current_figure == 1) {
                *figure_x = 10;
                *figure_y = 8;
            }
            else if (current_figure == 2) {
                *figure_x = -8;
                *figure_y = 8;
            }
            else if (current_figure == 3) {
                *figure_x = 2;
                *figure_y = 12;
            }
            else if (current_figure == 4) {
                *figure_x = 6;
                *figure_y = 6;
            }
            else if (current_figure == 5) {
                *figure_x = -3;
                *figure_y = 12;
            }
            else if (current_figure == 6) {
                *figure_x = -2;
                *figure_y = 7;
            }
            else {
                *figure_x = 3;
                *figure_y = 9;
            }
            break;
        default:
            break;
    }
}


// WZD ovr153p18
// drake178: CMB_CreateEntity()
/*
; creates a combat entity (aka battle figure) based on
; the supplied parameters, calculating its Z depth and
; inserting it into the sorting array; XOff and YOff
; are subtracted from the origin to get the Top/Left
; image draw coordinate, while draw type can be:
;   0 - normal frame selection
;   1 - precomposed BU figure (IMG_Loc is an index)
;   2 - second-pass variable-frame (out of 8)
;   3 - second-pass fixed frame (GUI_ProjectileFrame)
;   4 - simple single frame from EMM TILEX
*/
/*

seg_or_idx
    CMB_SpawnFigure__WIP() passes battle_units[itr].bufpi

figure_set_idx
    not in use

CMB_SpawnFigure__WIP()
    CMB_CreateEntity__WIP(draw_x, draw_y, seg_or_idx, 13, 23, UU, 1, controller_idx, figure_set_idx, outline_magic_realm, BldAmt, UU, Blood_Frame);

*/
void CMB_CreateEntity__WIP(int16_t draw_x, int16_t draw_y, int64_t seg_or_idx, int16_t draw_x_shift, int16_t draw_y_shift, int16_t Frame, int16_t entity_type, int16_t controller_idx, int16_t niu_figure_set_idx, int16_t outline_magic_realm, int16_t BldAmt, int16_t UU_14h, int16_t BldFrm)
{
    int16_t combat_grid_cell_y_offset = 0;
    int16_t combat_grid_cell_x_offset = 0;
    int16_t combat_grid_cell_y = 0;
    int16_t combat_grid_cell_x = 0;

    if(combat_grid_entity_count > MAX_ENTITIES)
    {
        STU_DEBUG_BREAK();
        // TODO  Exit_With_Message(cnst_CMBEntity_Error);  // "E1"
    }

    combat_grid_entities[combat_grid_entity_count].draw_x = draw_x;

    combat_grid_entities[combat_grid_entity_count].draw_y = draw_y;

    combat_grid_entities[combat_grid_entity_count].seg_or_idx = seg_or_idx;

    combat_grid_entities[combat_grid_entity_count].frame_num = Frame;

    combat_grid_entities[combat_grid_entity_count].draw_x_shift = draw_x_shift;

    combat_grid_entities[combat_grid_entity_count].draw_y_shift = draw_y_shift;

    combat_grid_entities[combat_grid_entity_count].entity_type = entity_type;

    combat_grid_entities[combat_grid_entity_count].owner_idx = controller_idx;

    combat_grid_entities[combat_grid_entity_count].niu_figure_set_idx = niu_figure_set_idx;

    combat_grid_entities[combat_grid_entity_count].outline_magic_realm = outline_magic_realm;

    combat_grid_entities[combat_grid_entity_count].Unused_14h = UU_14h;

    combat_grid_entities[combat_grid_entity_count].Blood_Frame = BldFrm;

    combat_grid_entities[combat_grid_entity_count].Blood_Amt = BldAmt;


    Screen_To_Combat_Grid_Cell_X_And_Offset(draw_x, draw_y, &combat_grid_cell_x, &combat_grid_cell_x_offset);

    Screen_To_Combat_Grid_Cell_Y_And_Offset(draw_x, draw_y, &combat_grid_cell_y, &combat_grid_cell_y_offset);


    combat_grid_entities[combat_grid_entity_count].draw_order_value = ((combat_grid_cell_y * 8000) + (combat_grid_cell_x * 320) + (combat_grid_cell_y_offset * 16) + combat_grid_cell_x_offset);

    Set_Entity_Draw_Order();

    combat_grid_entity_count++;

}


// WZD ovr153p19
// drake178: CMB_ZSort_Insert()
/*
; called before incrementing the entity counter, this
; function inserts the entity being created into the
; Z sorting array based on its ZDepth field value
*/
/*

find the first entity that has a higher draw order
if that is the last entity, nothing to do, early-exit
if not, move all the other entities up one
insert the current entity
*/
void Set_Entity_Draw_Order(void)
{
    uint32_t draw_order_value = 0;
    int16_t itr_up = 0;  // _SI_
    int16_t itr_dn = 0;  // _DI_
    int16_t entity_idx;  // _CX_

    if(combat_grid_entity_count == 0)
    {
        return;
    }

    draw_order_value = combat_grid_entities[combat_grid_entity_count].draw_order_value;

    for(itr_up = 0; itr_up < combat_grid_entity_count; itr_up++)
    {

        if(combat_grid_entities[combat_grid_entities_draw_order[itr_up]].draw_order_value > draw_order_value)
        {
            break;
        }

    }

    if(itr_up == combat_grid_entity_count)
    {
        return;
    }

    entity_idx = itr_up;

    // ; should be jg (or ja) instead
    // ¿ maybe was `entity_idx < itr_dn` ?
    for(itr_dn = combat_grid_entity_count; itr_dn >= entity_idx; itr_dn--)
    {

        combat_grid_entities_draw_order[itr_dn] = combat_grid_entities_draw_order[(itr_dn - 1)];

    }

    combat_grid_entities_draw_order[entity_idx] = combat_grid_entity_count;  // combat_grid_entity_count is incremented in CMB_CreateEntity__WIP()

}


// WZD ovr153p20
// drake178: EMM_FIGX_CreateHdr()
// AKA EMM_FIGUREX_Init__HACK()
/*
; maps in and creates an LBX_Alloc_Space header in the
; EMM FIGUREX handle, at a total offset of 638h times
; the figure index, with a size of 637h paragraphs
; plus 1 for the header
*/
/*
    sets EMS_PFBA


25456 / 8 = 

*/
void EMM_FIGUREX_Init__HACK(int16_t bufpi)
{
    int16_t logical_page = 0;
    int16_t offset = 0;  // _SI_

    logical_page = ((bufpi * 3) / 2);

    FIGUREX_OFFSET

    // TODO  EMM_Map4Pages(logical_page, EmmHndl_FIGUREX);
    EMS_PFBA = (EmmHndl_FIGUREX + (logical_page * SZ_EMM_LOGICAL_PAGE));

    SET_2B_OFS((EMS_PFBA + offset), SAMB_MEMSIG1, _SA_MEMSIG1);
    SET_2B_OFS((EMS_PFBA + offset), SAMB_MEMSIG2, _SA_MEMSIG2);
    SET_2B_OFS((EMS_PFBA + offset), SAMB_SIZE, 1591);  // 1591 PR, 25456 B
    SET_2B_OFS((EMS_PFBA + offset), SAMB_USED, 1);

}


// WZD ovr153p21
// drake178: EMM_TILEX_Init()
void EMM_TILEX_Init__HACK(void)
{

    // EMM_Map4Pages(0, EmmHndl_TILEXXX);
    EMS_PFBA = (EmmHndl_TILEXXX + (0 * SZ_EMM_LOGICAL_PAGE));

    SET_2B_OFS(EMS_PFBA, SAMB_MEMSIG1, _SA_MEMSIG1);
    SET_2B_OFS(EMS_PFBA, SAMB_MEMSIG2, _SA_MEMSIG2);
    SET_2B_OFS(EMS_PFBA, SAMB_SIZE, 3071);  // 3071 PR, 49136 B
    SET_2B_OFS(EMS_PFBA, SAMB_USED, 1);

}


// WZD ovr153p22
// drake178: UU_NullSub_153_1()

// WZD ovr153p23
// drake178: CMB_Predraw_Figures()
/*
; predraws a figure of each battle unit into the
; sandbox (battle_unit_picts_seg@), complete with unit
; enchantment outlines, frame animation, image effects,
; selection highlight, and banner colors
*/
/*



*/
void Combat_Figure_Compose_USEFULL(void)
{
    SAMB_ptr temp_GfxBuf_2400B = 0;
    int16_t IMG_Effect = 0;
    int16_t frame_num = 0;
    int16_t outline_magic_realm = 0;
    int16_t player_idx = 0;
    int16_t bufpi = 0;
    SAMB_ptr * figure_pict_set_seg = 0;
    int16_t offset = 0;
    // SAMB_ptr temp_figure_pict_set_seg = 0;
    int16_t target_cgy = 0;
    int16_t cgy = 0;
    int16_t target_cgx = 0;
    int16_t cgx = 0;
    int16_t battle_unit_idx = 0;  // _SI_
    int16_t figure_set_idx = 0;  // _DI_

    temp_GfxBuf_2400B = GfxBuf_2400B;

    for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
    {

        if(battle_units[battle_unit_idx].status != bus_Active)
        {
            continue;
        }

        bufpi = battle_units[battle_unit_idx].bufpi;
        
        player_idx = battle_units[battle_unit_idx].controller_idx;
        
        outline_magic_realm = battle_units[battle_unit_idx].outline_magic_realm;
        
        IMG_Effect = battle_units[battle_unit_idx].Image_Effect;
        
        /*
            BEGIN:  frame_num
        */
        {
            if(battle_units[battle_unit_idx].Always_Animate == ST_TRUE)
            {
                frame_num = CMB_BaseAnimFrame;
            }
            else
            {
                if(battle_units[battle_unit_idx].Moving == ST_TRUE)
                {
                    if(battle_units[battle_unit_idx].Move_Bob == ST_TRUE)
                    {
                        frame_num = CMB_BaseAnimFrame;
                    }
                    else
                    {
                        frame_num = CMB_MoveAnimFrame;
                    }
                }
                else
                {
                    frame_num = 1;
                }
            }
            if(battle_units[battle_unit_idx].Melee_Anim != 0)
            {
                if(battle_units[battle_unit_idx].Melee_Anim == 1)
                {
                    frame_num = CMB_UnitATKAnimFrame;
                }
                else
                {
                    frame_num = CMB_UnitDEFAnimFrame;
                }
            }
        }
        /*
            END:  frame_num
        */

        cgx = battle_units[battle_unit_idx].cgx;
        cgy = battle_units[battle_unit_idx].cgy;
        target_cgx = battle_units[battle_unit_idx].target_cgx;
        target_cgy = battle_units[battle_unit_idx].target_cgy;
        BATTLE_UNIT_FACING_DRECTION

        FIGUREX_MAP
        FIGUREX_OFFSET
        FIGUREX_POINTER

        FLIC_Set_CurrentFrame(figure_pict_set_seg[figure_set_idx], frame_num);
        GfxBuf_2400B = battle_unit_picts_seg[battle_units[battle_unit_idx].bufpi];
        Draw_Picture_To_Bitmap(figure_pict_set_seg[figure_set_idx], GfxBuf_2400B);

        Combat_Figure_Banner_Color(player_idx);
        Combat_Unit_Enchantment_Outline_Draw(outline_magic_realm);
        Combat_Figure_Effect__WIP(IMG_Effect);
        Combat_Figure_Active_Red_Outline(battle_unit_idx);
    }

    GfxBuf_2400B = temp_GfxBuf_2400B;

}


// WZD ovr153p24
// drake178: RP_NullSub_153_2()



/*
    WIZARDS.EXE  ovr154
*/

// WZD ovr154p01
// drake178: CMB_Terrain_Init()
/*
; sets the terrain and map-related combat variables
; into the battlefield structure, loads the combat tile
; images, generates the combat map, and then draws the
; background, saving it into VGA frame 3
;
; has multiple BUGs related to outposts, roads across
; the Y-axis, rivers, and walls
*/
/*

*/
void CMB_Terrain_Init__WIP(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t River_Matrix[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Road_Matrix[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Battlefield_CityEnchant_Ptr = 0;
    int8_t * CityEnchant_Ptr = 0;
    int16_t Magic_Walls = 0;
    int16_t City_Walls = 0;
    int16_t G_Unused_Local = 0;
    int16_t Flying_Fortress = 0;
    /* combat map terrain set/core/base */
    int16_t cts = 0;
    int16_t Enchanted_Roads = 0;
    int16_t City_House_Type = 0;
    int16_t City_Population = 0;
    int16_t Location_Type = 0;
    uint32_t random_seed = 0;
    int16_t TileTest_Y = 0;
    int16_t TileTest_X = 0;
    int16_t Loop_Var = 0;
    int16_t itr = 0;  // _SI_


    CMB_WallRise_Going = 0;  // ; set to 0 at the beginning of combat map creation
                             // ; 1 branches wall entity creation (unknown IMG array)

    Magic_Walls = 0;

    CityEnchant_Ptr = (int8_t *)&battlefield->city_enchantments;

    for(itr = 0; itr < 30; itr++)
    {
        CityEnchant_Ptr[itr] = 0;
    }

    Location_Type = 0;

    for(itr = 0; itr < _cities; itr++)
    {
        if(
            (_CITIES[itr].wx == wx)
            &&
            (_CITIES[itr].wy == wy)
            &&
            (_CITIES[itr].wp == wp)
        )
        {

            City_Population = _CITIES[itr].population;

            if(City_Population == 0)
            {

                Location_Type = clt_Outpost;  /* Outpost */

            }
            else
            {

                Location_Type = clt_City;  /* City */

                for(Loop_Var = 0; Loop_Var < _num_players; Loop_Var++)
                {

                    if(
                        (_FORTRESSES[Loop_Var].active == ST_TRUE)
                        &&
                        (_FORTRESSES[Loop_Var].wx == wx)
                        &&
                        (_FORTRESSES[Loop_Var].wy == wy)
                        &&
                        (_FORTRESSES[Loop_Var].wp == wp)
                    )
                    {

                        Location_Type = clt_Fortress;  /* Fortress City */

                        G_Unused_Local = 1;

                    }

                }

                City_House_Type = _race_type_table[_CITIES[itr].race].house_type;  /* {0: standard, 1: tree house, 2: mound} */

                if(City_House_Type == 1)
                {
                    City_House_Type = 2;
                }
                else if(City_House_Type == 1)
                {
                    City_House_Type = 1;
                }

                if(_CITIES[itr].enchantments[FLYING_FORTRESS] != 0)
                {
                    Flying_Fortress = ST_TRUE;
                }
                else
                {
                    Flying_Fortress = ST_FALSE;
                }

                Magic_Walls = 0;

                if(_CITIES[itr].enchantments[WALL_OF_FIRE] != 0)
                {
                    Magic_Walls = 1;
                }

                if(_CITIES[itr].enchantments[WALL_OF_DARKNESS] != 0)
                {
                    Magic_Walls += 2;
                }

                if(_CITIES[itr].bldg_status[CITY_WALLS] >= bs_Replaced)
                {

                    City_Walls = ST_TRUE;

                }
                else
                {
                    
                    City_Walls = ST_FALSE;

                }



            }

//             CityEnchant_Ptr = _CITIES[itr].enchantments;
// 
//             Battlefield_CityEnchant_Ptr = battlefield.city_enchantments;
// 
// if(_CITIES[itr].enchantments[NIGHTSHADE] > CityEnchant_Ptr)
// {
// les     bx, [bp+CityEnchant_Ptr]
// mov     al, [es:bx]
// les     bx, [bp+Battlefield_CityEnchant_Ptr]
// mov     [es:bx], al
// 
// inc     [word ptr bp+CityEnchant_Ptr]
// inc     [word ptr bp+Battlefield_CityEnchant_Ptr]
// 
// }

        }

    }


    battlefield->wp = wp;


    for(itr = 0; itr < NUM_TOWERS; itr++)
    {

        if(
            (_TOWERS[itr].wx == wx)
            &&
            (_TOWERS[itr].wy == wy)
        )
        {
            Location_Type = 4;
        }

    }


    for(itr = 0; itr < NUM_NODES; itr++)
    {

        if(
            (_NODES[itr].wx == wx)
            &&
            (_NODES[itr].wy == wy)
            &&
            (_NODES[itr].wp == wp)
        )
        {

            Location_Type = (_NODES[itr].type % 7);

        }

    }


    for(itr = 0; itr < NUM_LAIRS; itr++)
    {

        if(
            (_LAIRS[itr].wx == wx)
            &&
            (_LAIRS[itr].wy == wy)
            &&
            (_LAIRS[itr].wp == wp)
            &&
            (_LAIRS[itr].Intact == ST_TRUE)
        )
        {

            Location_Type = (_LAIRS[itr].type + 4);

        }

    }


    for(itr = 0; itr < 9; itr++)
    {

        Road_Matrix[itr] = 0;

    }

    for(itr = 0; itr < 9; itr++)
    {

        River_Matrix[itr] = 0;

    }


    for(TileTest_Y = (wy - 1); (wy + 2) > TileTest_Y; TileTest_Y++)
    {

        for(TileTest_X = (wx - 1); (wx + 2) > TileTest_X; TileTest_X++)
        {

            if(
                (TileTest_Y >= 0)
                &&
                (TileTest_Y < WORLD_HEIGHT)
                &&
                (TileTest_X >= 0)
                &&
                (TileTest_X < WORLD_WIDTH)
            )
            {

                if((MAP_SQUARE_FLAG(TileTest_X, TileTest_Y, wp) & MSF_ROAD) != 0)
                {

                    Road_Matrix[((TileTest_Y - wy + 1) * 3) + (TileTest_X - wx + 1)] = ST_TRUE;

                }

            }

        }

    }


    if((MAP_SQUARE_FLAG(wx, wy, wp) & TF_Enc_Road) != 0)
    {

        Enchanted_Roads = ST_TRUE;

    }
    else
    {

        Enchanted_Roads = ST_FALSE;

    }

    cts = cts_Water;

    if(Square_Is_Forest(wx, wy, wp) == ST_TRUE)
    {

        cts = cts_Forest;

    }
    else if(Square_Is_Hills(wx, wy, wp) == ST_TRUE)
    {

        cts = cts_Hills;

    }
    else if(Square_Is_Mountain(wx, wy, wp) == ST_TRUE)
    {

        cts = cts_Mountains;

        CMB_StoneWallType = 0;

    }
    else if(Square_Is_Desert(wx, wy, wp) == ST_TRUE)
    {

        cts = cts_Desert;

        CMB_StoneWallType = 0;

    }
    else if(Square_Is_Swamp(wx, wy, wp) == ST_TRUE)
    {

        cts = cts_Plains;

    }
    else if(Square_Is_Grasslands(wx, wy, wp) == ST_TRUE)
    {

        cts = cts_Plains;

    }
    else if(Square_Is_River(wx, wy, wp) == ST_TRUE)
    {

        cts = cts_Plains;

    }
    else if(Square_Is_Tundra(wx, wy, wp) == ST_TRUE)
    {

        cts = cts_Tundra;

    }
    else
    {

        cts = cts_Water;

    }

    // ¿ BUG ? already caught in Square_Is_Forest()
    if(Location_Type == clt_NatureNode)
    {

        cts = cts_Forest;

    }

    if(wp == 1)
    {

        CMB_StoneWallType = 1;

    }


    random_seed = Get_Random_Seed();

    // IDGI. the term for wp has to be further *left* than (320 * 200 = 64000)  1111101000000000  ~ 10000000000000000 0x10000 65536d  16 bits?  11111101000000000 129536
    Set_Random_Seed( (10039 + ((wp + 5) * (wx * wy))) );

    Load_Combat_Terrain_Pictures(cts, wp);

    if(RP_CMB_NoCombatMap == ST_FALSE)
    {

        Generate_Combat_Map__WIP(
            Location_Type, 
            City_House_Type, 
            &Road_Matrix[0], 
            Enchanted_Roads, 
            cts, 
            &River_Matrix[0], 
            Flying_Fortress, 
            ATKR_FloatingIsland, 
            DEFR_FloatingIsland, 
            City_Walls, 
            City_Population, 
            Magic_Walls
        );

    }

    CMB_Vortex_Count = 0;

    Set_Random_Seed(random_seed);

    CMB_ComposeBackgrnd__WIP();  // ... |-> Copy_Off_To_Back();

}


// WZD ovr154p02
// drake178: CMB_GenerateMap()
/*
; generates the combat map based on the passed values,
; including the associated movement maps, and wall,
; house, tree, and rock arrays (the FlyFort argument is
; not actually used here)
;
; has two wall-related BUGs: Outposts can not have City
; Walls (i.e. Wall of Stone will have no effect), but
; any non-city combat location can have destroyed
; walls, as the field value is not zeroed for them
*/
/*



*/
void Generate_Combat_Map__WIP(
    int16_t location_type, 
    int16_t house_type, 
    int16_t roads_array[], 
    int16_t enchanted_roads_flag, 
    int16_t cts, 
    int16_t rivers_array[], 
    int16_t flying_fortress_flag, 
    int16_t attacker_floating_island_flag, 
    int16_t defender_floating_island_flag, 
    int16_t city_walls, 
    int16_t city_population, 
    int16_t magic_walls
)
{

    uint32_t random_seed = 0;
    int16_t Random_Y = 0;
    int16_t Random_X = 0;
    int16_t Not_Valid = 0;  /* what's not valid? */
    int16_t House_Index = 0;
    int16_t Dirt_PatchCount = 0;
    int16_t Rough_PatchCount = 0;
    int16_t itr1 = 0;  // _SI_
    int16_t itr2 = 0;  // _DI_
    int16_t itr_cgx = 0;  // _SI_
    int16_t itr_cgy = 0;  // _DI_


    battlefield->House_Count = 0;

    for(itr2 = 0; itr2 <= 3; itr2++)
    {

        for(itr1 = 0; itr1 <= 3; itr1++)
        {

            battlefield->Wall_Sections[((itr2 * 3) + itr1)] = 0;

        }

    }


    random_seed = Get_Random_Seed();


    switch(location_type)
    {

        case clt_OpenField:
        {
            battlefield->Central_Structure = CS_None;
        } break;

        case clt_Outpost:
        {
            // ; BUG: disregards Wall of Stone
            battlefield->Central_Structure = CS_Outpost;
        } break;

        case clt_City:
        case clt_Fortress:
        {

            battlefield->Central_Structure = CS_City;

            if(city_population < 12)
            {

                for(battlefield->House_Count = 0; battlefield->House_Count < city_population; battlefield->House_Count++)
                {

                    Random_X = (Random(4) - 1);

                    Random_Y = (Random(4) - 1);

                    itr1 = (Random_X + 5);

                    itr2 = (Random_Y + 10);

                    Not_Valid = ST_FALSE;

                    if(city_walls == ST_TRUE)
                    {

                        if(
                            (Random_X == 0)
                            &&
                            (Random_Y == 0)
                        )
                        {
                            Not_Valid = ST_TRUE;
                        }

                        if
                        (
                            (Random_X == 0)
                            &&
                            (Random_Y == 3)
                        )
                        {
                            Not_Valid = ST_TRUE;
                        }

                        if
                        (
                            (Random_X == 3)
                            &&
                            (Random_Y == 0)
                        )
                        {
                            Not_Valid = ST_TRUE;
                        }

                        if
                        (
                            (Random_X == 3)
                            &&
                            (Random_Y == 3)
                        )
                        {
                            Not_Valid = ST_TRUE;
                        }

                    }

                    for(House_Index = 0; battlefield->House_Count > House_Index; House_Index++)
                    {

                        if(
                            (battlefield->House_TileXs[House_Index] == itr1)
                            &&
                            (battlefield->House_TileYs[House_Index] == itr2)
                        )
                        {
                            Not_Valid = ST_TRUE;
                        }

                    }

                    if(Not_Valid == ST_FALSE)
                    {

                        battlefield->House_TileXs[battlefield->House_Count] = itr1;

                        battlefield->House_TileYs[battlefield->House_Count] = itr2;

                        battlefield->House_IMG_Segs[battlefield->House_Count] = IMG_CMB_Houses[(((house_type * 5) + Random(5)) - 1)];

                        battlefield->House_Count += 1;

                    }

                }


            }
            else
            {

                House_Index = 0;

                for(itr2 = 0; itr2 <= 3; itr2++)
                {

                    for(itr1 = 0; itr1 <= 3; itr1++)
                    {
                        if(
                            (
                                !(itr1 == 0 && itr2 == 0)
                                &&
                                !(itr1 == 3 && itr2 == 0)
                                &&
                                !(itr1 == 0 && itr2 == 3)
                                &&
                                !(itr1 == 3 && itr2 == 3)
                            )
                            ||
                            (city_walls == ST_FALSE)
                        )
                        {

                            battlefield->House_TileXs[House_Index] = (5 + itr1);

                            battlefield->House_TileXs[House_Index] = (10 + itr1);

                            battlefield->House_IMG_Segs[House_Index] = IMG_CMB_Houses[(((house_type * 5) + Random(5)) - 1)];

                            House_Index += 1;

                        }

                    }

                }

                battlefield->House_Count = 12;

            }


            battlefield->Walled_City = city_walls;

            if(city_walls == ST_TRUE)
            {
                for(itr2 = 0; itr2 <= 3; itr2++)
                {
                    
                    for(itr1 = 0; itr1 <= 3; itr1++)
                    {

                        battlefield->Wall_Sections[((itr2 * 4) + itr1)] = 1;

                    }

                }

                battlefield->Wall_Sections[5] = 0;
                battlefield->Wall_Sections[9] = 0;
                battlefield->Wall_Sections[6] = 0;
                battlefield->Wall_Sections[10] = 0;

            }

        } break;

        // TODO  case clt_Fortress:
        // {
        // 
        // } break;

        case clt_Tower:
        {
            battlefield->Central_Structure = CS_Tower;
        } break;

        case clt_ChaosNode:
        {
            battlefield->Central_Structure = CS_ChaosNode;
        } break;

        case clt_NatureNode:
        {
            battlefield->Central_Structure = CS_NatureNode;
        } break;

        case clt_SorceryNode:
        {
            battlefield->Central_Structure = CS_SorceryNode;
        } break;

        case clt_Cave:
        {
            battlefield->Central_Structure = CS_Cave;
        } break;

        case clt_Dungeon:
        {
            battlefield->Central_Structure = CS_Dungeon;
        } break;

        case clt_AncientTemple:
        {
            battlefield->Central_Structure = CS_Temple;
        } break;

        case clt_Keep:
        {
            battlefield->Central_Structure = CS_Fort;
        } break;

        case clt_MonsterLair:
        {
            battlefield->Central_Structure = CS_Cave;
        } break;

        case clt_Ruins:
        {
            battlefield->Central_Structure = CS_Dungeon;
        } break;

        case clt_FallenTemple:
        {
            battlefield->Central_Structure = CS_Ruins;
        } break;

    }


    Set_Random_Seed(random_seed);


    for(itr_cgy = 0; itr_cgy < COMBAT_GRID_HEIGHT; itr_cgy++)
    {

        for(itr_cgx = 0; itr_cgx < COMBAT_GRID_WIDTH; itr_cgx++)
        {

            battlefield->Tile_Mud[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = 0;

        }

    }

    if((magic_walls & 1) != 0)
    {

        battlefield->Wall_of_Fire = ST_TRUE;

    }
    else
    {

        battlefield->Wall_of_Fire = ST_FALSE;

    }

    if((magic_walls & 2) != 0)
    {

        battlefield->Wall_of_Darkness = ST_TRUE;

    }
    else
    {

        battlefield->Wall_of_Darkness = ST_FALSE;

    }

    CMB_Enchanted_Roads = enchanted_roads_flag;

    for(itr_cgy = 0; itr_cgy < COMBAT_GRID_HEIGHT; itr_cgy++)
    {

        for(itr_cgx = 0; itr_cgx < COMBAT_GRID_WIDTH; itr_cgx++)
        {

            battlefield->Tile_Road[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = 0;

        }

    }

    if(cts != cts_Water)
    {

        for(itr_cgy = 0; itr_cgy < COMBAT_GRID_HEIGHT; itr_cgy++)
        {

            for(itr_cgx = 0; itr_cgx < COMBAT_GRID_WIDTH; itr_cgx++)
            {

                battlefield->terrain_group[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = CTG_Grass;

            }

        }

        Rough_PatchCount = 0;

        Dirt_PatchCount = 8;

        if(cts == cts_Plains)
        {
            Rough_PatchCount = 5;
        }

        if(cts == cts_Hills)
        {
            Rough_PatchCount = 20;
        }

        if(cts == cts_Desert)
        {
            Rough_PatchCount = 10;
        }

        if(cts == cts_Mountains)
        {
            Rough_PatchCount = 30;
        }

        if(cts == cts_Desert)
        {
            Rough_PatchCount = 10;
        }

        if(cts == cts_Forest)
        {
            Rough_PatchCount = 5;
        }


        // Patch_Terrain_Group(CTG_Rough, Rough_PatchCount, 8, 3);

        // Patch_Terrain_Group(CTG_Dirt, Dirt_PatchCount, 4, 0);


        if(cts == cts_Plains)
        {

            // TODO  CMB_RiverGen(rivers_array);
            // ; creates a river into the combat map if the tile that
            // ; the battle is taking place on has one, except the
            // ; the array passed is never filled out, and the
            // ; function will thus do nothing
            // ;
            // ; BUG: the origin tile for south-east faring rivers has
            // ; the wrong Y coordinate, causing 1-2 tile rivers to be
            // ; generated at the bottom (non-visible) part of the map

        }

        // TODO  CMB_RoadGen(location_type, roads_array);
        // ; adds roads to the combat map based on the passed
        // ; road matrix

        // TODO  CMB_RemoveRough(location_type);
        // ; replaces rough terrain with grass for tiles that have
        // ; a road, an adjacent dirt tile, or are in the
        // ; defender's starting area for non-field battles

        // TODO  CMB_MergeDirt();
        // ; changes any grass tile into a dirt tile if it is
        // ; horizontally or vertically between two dirt tiles

    }
    else  /* cts == cts_Water */
    {

        for(itr_cgy = 0; itr_cgy < COMBAT_GRID_HEIGHT; itr_cgy++)
        {

            for(itr_cgx = 0; itr_cgx < COMBAT_GRID_WIDTH; itr_cgx++)
            {

                battlefield->terrain_group[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = CTG_DeepWater;

            }

        }

        if(defender_floating_island_flag == ST_TRUE)
        {
            // les     bx, [battlefield]
            // mov     [byte ptr es:bx+66Bh], 80h      ; tile [5,12]
            // les     bx, [battlefield]
            // mov     [byte ptr es:bx+66Ch], 80h      ; tile [6,12]
            // les     bx, [battlefield]
            // mov     [byte ptr es:bx+66Dh], 80h      ; tile [7,12]
            // les     bx, [battlefield]
            // mov     [byte ptr es:bx+656h], 80h      ; tile [5,11]
            // les     bx, [battlefield]
            // mov     [byte ptr es:bx+657h], 80h      ; tile [6,11]
            // les     bx, [battlefield]
            // mov     [byte ptr es:bx+658h], 80h      ; tile [7,11]
            // les     bx, [battlefield]
            // mov     [byte ptr es:bx+680h], 80h      ; tile [5,13]
            // les     bx, [battlefield]
            // mov     [byte ptr es:bx+681h], 80h      ; tile [6,13]
            // les     bx, [battlefield]
            // mov     [byte ptr es:bx+682h], 80h      ; tile [7,13]
            // les     bx, [battlefield]
            // mov     [byte ptr es:bx+683h], 81h      ; tile [8,13]
        }

        if(attacker_floating_island_flag == ST_TRUE)
        {

            if(defender_floating_island_flag == ST_TRUE)
            {
                // les     bx, [battlefield]
                // mov     [byte ptr es:bx+66Eh], 80h      ; tile [8,12]
                // les     bx, [battlefield]
                // mov     [byte ptr es:bx+66Fh], 80h      ; tile [9,12]
                // les     bx, [battlefield]
                // mov     [byte ptr es:bx+670h], 80h      ; tile [10,12]
                // les     bx, [battlefield]
                // mov     [byte ptr es:bx+671h], 80h      ; tile [11,12]
                // les     bx, [battlefield]
                // mov     [byte ptr es:bx+65Ah], 80h      ; tile [9,11]
                // les     bx, [battlefield]
                // mov     [byte ptr es:bx+65Bh], 80h      ; tile [10,11]
                // les     bx, [battlefield]
                // mov     [byte ptr es:bx+65Ch], 80h      ; tile [11,11]
                // les     bx, [battlefield]
                // mov     [byte ptr es:bx+684h], 80h      ; tile [9,13]
                // les     bx, [battlefield]
                // mov     [byte ptr es:bx+685h], 80h      ; tile [10,13]
                // les     bx, [battlefield]
                // mov     [byte ptr es:bx+686h], 80h      ; tile [11,13]
                // les     bx, [battlefield]
                // mov     [byte ptr es:bx+687h], 81h      ; tile [12,13]
            }
            else
            {
                // les     bx, [battlefield]
                // mov     [byte ptr es:bx+661h], 80h      ; tile [16,11]
                // les     bx, [battlefield]
                // mov     [byte ptr es:bx+662h], 80h      ; tile [17,11]
                // les     bx, [battlefield]
                // mov     [byte ptr es:bx+663h], 80h      ; tile [18,11]
                // les     bx, [battlefield]
                // mov     [byte ptr es:bx+64Ch], 80h      ; tile [16,10]
                // les     bx, [battlefield]
                // mov     [byte ptr es:bx+64Dh], 80h      ; tile [17,10]
                // les     bx, [battlefield]
                // mov     [byte ptr es:bx+64Eh], 80h      ; tile [18,10]
                // les     bx, [battlefield]
                // mov     [byte ptr es:bx+676h], 80h      ; tile [16,12]
                // les     bx, [battlefield]
                // mov     [byte ptr es:bx+677h], 80h      ; tile [17,12]
                // les     bx, [battlefield]
                // mov     [byte ptr es:bx+678h], 80h      ; tile [18,12]
                // les     bx, [battlefield]
                // mov     [byte ptr es:bx+679h], 81h      ; tile [19,12]
            }

        }

    }


    CMB_TileGen__WIP(cts);


    Set_Movement_Cost_Maps(location_type, city_walls);


    if(cts == cts_Water)
    {

        battlefield->Tree_Count = 0;

        battlefield->Rock_Count = 0;

    }
    else
    {

        battlefield->Tree_Count = 0;

        battlefield->Rock_Count = 0;

        // TODO  CMB_TreeGen(cts, location_type);
        // ; generates trees for the combat map based on location
        // ; and terrain type, and adds them to the battlefield
        // ; structure
        // ;
        // ; BUG: will generate trees outside of the visible
        // ; screen boundary due to an implementation flaw

        // TODO  CMB_RockGen(cts, location_type);
        // ; generates rocks for the combat map based on location
        // ; and terrain type, and adds them to the battlefield
        // ; structure
        // ;
        // ; BUG: may generate rocks outside of the visible
        // ; screen boundary (under the bottom combat GUI)

    }

}


// WZD ovr154p03
// drake178: CMB_TreeGen()

// WZD ovr154p04
// drake178: CMB_RockGen()

// WZD ovr154p05
// drake178: CMB_RoadGen()

// WZD ovr154p06
// drake178: CMB_RiverGen()

// WZD ovr154p07
// drake178: CMB_MergeDirt()

// WZD ovr154p08
// drake178: CMB_RemoveRough()

// WZD ovr154p09
// drake178: CMB_TileGen()
/*
; fills out the actual tile type array of the
; battlefield table based on the tile type groups
*/
/*

*/
void CMB_TileGen__WIP(int16_t ctt)
{
    int16_t UU_RoughTiles_Index = 0;
    int16_t uu_1C = 0;
    int16_t UU_Dirt_Trans_Index = 0;
    int16_t UU_DirtTiles_Index = 0;
    int16_t UU_BaseTiles_Index = 0;
    int16_t Tileset_Index = 0;
    int16_t ctg_3 = 0;
    int16_t ctg_2 = 0;
    int16_t ctg_1 = 0;
    int16_t ctg_6 = 0;
    int16_t ctg_5 = 0;
    int16_t ctg_4 = 0;
    int16_t ctg_9 = 0;
    int16_t ctg_8 = 0;
    int16_t ctg_7 = 0;
    int16_t itr_y = 0;  // _DI_  combat grid row
    int16_t itr_x = 0;  // _SI_  combat grid column
    int16_t itr_cgx = 0;  // _SI_
    int16_t itr_cgy = 0;  // _DI_
    int16_t DBG_battlefield_terrain_group_idx = 0;
    int8_t * DBG_ptr_battlefield_terrain_group = 0;
    

    UU_BaseTiles_Index = 0;
    UU_DirtTiles_Index = 0;
    UU_Dirt_Trans_Index = 0;
    uu_1C = 0;
    UU_RoughTiles_Index = 0;

    for(itr_cgy = 0; itr_cgy < COMBAT_GRID_HEIGHT; itr_cgy++)
    {

        for(itr_cgx = 0; itr_cgx < COMBAT_GRID_WIDTH; itr_cgx++)
        {

            // ctg_7 = battlefield->terrain_group[((((itr_cgy - 1) * COMBAT_GRID_WIDTH) + itr_cgx) - 1)];
            // ctg_8 = battlefield->terrain_group[((((itr_cgy - 1) * COMBAT_GRID_WIDTH) + itr_cgx)    )];
            // ctg_9 = battlefield->terrain_group[((((itr_cgy - 1) * COMBAT_GRID_WIDTH) + itr_cgx) + 1)];
            // ctg_4 = battlefield->terrain_group[((((itr_cgy    ) * COMBAT_GRID_WIDTH) + itr_cgx) - 1)];
            // ctg_5 = battlefield->terrain_group[((((itr_cgy    ) * COMBAT_GRID_WIDTH) + itr_cgx)    )];
            // ctg_6 = battlefield->terrain_group[((((itr_cgy    ) * COMBAT_GRID_WIDTH) + itr_cgx) + 1)];
            // ctg_1 = battlefield->terrain_group[((((itr_cgy + 1) * COMBAT_GRID_WIDTH) + itr_cgx) - 1)];
            // ctg_2 = battlefield->terrain_group[((((itr_cgy + 1) * COMBAT_GRID_WIDTH) + itr_cgx)    )];
            // ctg_3 = battlefield->terrain_group[((((itr_cgy + 1) * COMBAT_GRID_WIDTH) + itr_cgx) + 1)];

            DBG_battlefield_terrain_group_idx = ((((itr_cgy - 1) * COMBAT_GRID_WIDTH) + itr_cgx) - 1);
            DBG_ptr_battlefield_terrain_group = &battlefield->terrain_group[DBG_battlefield_terrain_group_idx];
            ctg_7 = *DBG_ptr_battlefield_terrain_group;

            DBG_battlefield_terrain_group_idx = ((((itr_cgy - 1) * COMBAT_GRID_WIDTH) + itr_cgx)    );
            DBG_ptr_battlefield_terrain_group = &battlefield->terrain_group[DBG_battlefield_terrain_group_idx];
            ctg_8 = battlefield->terrain_group[DBG_battlefield_terrain_group_idx];

            DBG_battlefield_terrain_group_idx = ((((itr_cgy - 1) * COMBAT_GRID_WIDTH) + itr_cgx) + 1);
            DBG_ptr_battlefield_terrain_group = &battlefield->terrain_group[DBG_battlefield_terrain_group_idx];
            ctg_9 = battlefield->terrain_group[DBG_battlefield_terrain_group_idx];

            DBG_battlefield_terrain_group_idx = ((((itr_cgy    ) * COMBAT_GRID_WIDTH) + itr_cgx) - 1);
            DBG_ptr_battlefield_terrain_group = &battlefield->terrain_group[DBG_battlefield_terrain_group_idx];
            ctg_4 = battlefield->terrain_group[DBG_battlefield_terrain_group_idx];

            DBG_battlefield_terrain_group_idx = ((((itr_cgy    ) * COMBAT_GRID_WIDTH) + itr_cgx)    );
            DBG_ptr_battlefield_terrain_group = &battlefield->terrain_group[DBG_battlefield_terrain_group_idx];
            ctg_5 = battlefield->terrain_group[DBG_battlefield_terrain_group_idx];

            DBG_battlefield_terrain_group_idx = ((((itr_cgy    ) * COMBAT_GRID_WIDTH) + itr_cgx) + 1);
            DBG_ptr_battlefield_terrain_group = &battlefield->terrain_group[DBG_battlefield_terrain_group_idx];
            ctg_6 = battlefield->terrain_group[DBG_battlefield_terrain_group_idx];

            DBG_battlefield_terrain_group_idx = ((((itr_cgy + 1) * COMBAT_GRID_WIDTH) + itr_cgx) - 1);
            DBG_ptr_battlefield_terrain_group = &battlefield->terrain_group[DBG_battlefield_terrain_group_idx];
            ctg_1 = battlefield->terrain_group[DBG_battlefield_terrain_group_idx];

            DBG_battlefield_terrain_group_idx = ((((itr_cgy + 1) * COMBAT_GRID_WIDTH) + itr_cgx)    );
            DBG_ptr_battlefield_terrain_group = &battlefield->terrain_group[DBG_battlefield_terrain_group_idx];
            ctg_2 = battlefield->terrain_group[DBG_battlefield_terrain_group_idx];

            DBG_battlefield_terrain_group_idx = ((((itr_cgy + 1) * COMBAT_GRID_WIDTH) + itr_cgx) + 1);
            DBG_ptr_battlefield_terrain_group = &battlefield->terrain_group[DBG_battlefield_terrain_group_idx];
            ctg_3 = battlefield->terrain_group[DBG_battlefield_terrain_group_idx];



            if(itr_cgy == COMBAT_GRID_YMIN)
            {
                ctg_7 = CTG_Grass;
                ctg_8 = CTG_Grass;
                ctg_9 = CTG_Grass;
            }

            if(itr_cgy == (COMBAT_GRID_YMAX - 1))
            {
                ctg_1 = CTG_Grass;
                ctg_2 = CTG_Grass;
                ctg_3 = CTG_Grass;
            }

            if(itr_cgx == COMBAT_GRID_XMIN)
            {
                ctg_7 = CTG_Grass;
                ctg_4 = CTG_Grass;
                ctg_1 = CTG_Grass;
            }

            if(itr_cgx == (COMBAT_GRID_XMAX - 1))
            {
                ctg_9 = CTG_Grass;
                ctg_6 = CTG_Grass;
                ctg_3 = CTG_Grass;
            }


            /*
                BEGIN:  TerrGroup_Middle == CTG_Grass
            */
            if(ctg_5 == CTG_Grass)
            {

                if(ctg_6 == CTG_Dirt)
                {

                    STU_DEBUG_BREAK();

                    if(ctg_8 == CTG_Dirt)
                    {

                        battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = (Random(2) + UU_Dirt_Trans_Index + 27);

                    }
                    else
                    {

                        if(ctg_2 == CTG_Dirt)
                        {

                            battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = (Random(2) + UU_Dirt_Trans_Index + 11);

                        }
                        else
                        {

                            battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = (Random(2) + UU_Dirt_Trans_Index + 29);

                        }

                    }

                }
                else if(ctg_8 == CTG_Dirt)
                {

                    STU_DEBUG_BREAK();

                    if(ctg_4 == CTG_Dirt)
                    {

                        battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = (Random(2) + UU_Dirt_Trans_Index + 21);

                    }
                    else
                    {

                        battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = (Random(2) + UU_Dirt_Trans_Index + 23);

                    }

                }
                else if(ctg_4 == CTG_Dirt)
                {

                    STU_DEBUG_BREAK();

                    if(ctg_8 == CTG_Dirt)
                    {

                        battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = (Random(2) + UU_Dirt_Trans_Index + 21);

                    }
                    else if(ctg_2 == CTG_Dirt)
                    {

                        battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = (Random(2) + UU_Dirt_Trans_Index + 15);

                    }
                    else
                    {

                        battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = (Random(2) + UU_Dirt_Trans_Index + 17);

                    }

                }
                else if(ctg_2 == CTG_Dirt)
                {

                    STU_DEBUG_BREAK();

                    if(ctg_6 == CTG_Dirt)
                    {

                        battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = (Random(2) + UU_Dirt_Trans_Index + 11);

                    }
                    else if(ctg_4 == CTG_Dirt)
                    {

                        battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = (Random(2) + UU_Dirt_Trans_Index + 15);

                    }
                    else
                    {

                        battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = (Random(2) + UU_Dirt_Trans_Index + 9);

                    }
                }
                else if(ctg_3 == CTG_Dirt)
                {

                    STU_DEBUG_BREAK();

                    battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = (Random(2) + UU_Dirt_Trans_Index + 7);

                }
                else if(ctg_9 == CTG_Dirt)
                {

                    STU_DEBUG_BREAK();

                    battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = (Random(2) + UU_Dirt_Trans_Index + 25);

                }
                else if(ctg_7 == CTG_Dirt)
                {

                    STU_DEBUG_BREAK();

                    battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = (Random(2) + UU_Dirt_Trans_Index + 19);

                }
                else if(ctg_1 == CTG_Dirt)
                {

                    STU_DEBUG_BREAK();

                    battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = (Random(2) + UU_Dirt_Trans_Index + 13);

                }
                else
                {

                    battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = (Random(4) + UU_BaseTiles_Index - 1);

                }

            }
            else
            {

                STU_DEBUG_BREAK();

            }
            /*
                END:  TerrGroup_Middle == CTG_Grass
            */


            /*
                BEGIN:  CTG_River
            */
            if(battlefield->terrain_group[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] == CTG_River)
            {

                STU_DEBUG_BREAK();

                if(itr_cgy == 0)
                {
                    ctg_7 = CTG_River;
                    ctg_8 = CTG_River;
                    ctg_9 = CTG_River;
                }

                if(itr_cgy == COMBAT_GRID_WIDTH)
                {
                    ctg_1 = CTG_River;
                    ctg_2 = CTG_River;
                    ctg_3 = CTG_River;
                }

                if(itr_cgx == 0)
                {
                    ctg_7 = CTG_River;
                    ctg_4 = CTG_River;
                    ctg_1 = CTG_River;
                }

                if(itr_cgx == 20)
                {
                    ctg_9 = CTG_River;
                    ctg_6 = CTG_River;
                    ctg_3 = CTG_River;
                }

                if(
                    (ctg_4 == CTG_River)
                    &&
                    (ctg_6 == CTG_River)
                )
                {

                    battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = (56 + ((Random(2) - 1) * 6));
                }

                if(
                    (ctg_8 == CTG_River)
                    &&
                    (ctg_2 == CTG_River)
                )
                {

                    battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = (57 + ((Random(2) - 1) * 6));
                }

                if(
                    (ctg_8 == CTG_River)
                    &&
                    (ctg_4 == CTG_River)
                )
                {

                    battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = (58 + ((Random(2) - 1) * 6));
                }

                if(
                    (ctg_6 == CTG_River)
                    &&
                    (ctg_8 == CTG_River)
                )
                {

                    battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = (59 + ((Random(2) - 1) * 6));
                }

                if(
                    (ctg_2 == CTG_River)
                    &&
                    (ctg_6 == CTG_River)
                )
                {

                    battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = (60 + ((Random(2) - 1) * 6));
                }

                if(
                    (ctg_4 == CTG_River)
                    &&
                    (ctg_2 == CTG_River)
                )
                {

                    battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = (61 + ((Random(2) - 1) * 6));
                }


                if(itr_cgy == 0)
                {
                    ctg_7 = CTG_Grass;
                    ctg_8 = CTG_Grass;
                    ctg_9 = CTG_Grass;
                }

                if(itr_cgy == COMBAT_GRID_WIDTH)
                {
                    ctg_1 = CTG_Grass;
                    ctg_2 = CTG_Grass;
                    ctg_3 = CTG_Grass;
                }

                if(itr_cgx == 0)
                {
                    ctg_7 = CTG_Grass;
                    ctg_4 = CTG_Grass;
                    ctg_1 = CTG_Grass;
                }

                if(itr_cgx == 20)
                {
                    ctg_9 = CTG_Grass;
                    ctg_6 = CTG_Grass;
                    ctg_3 = CTG_Grass;
                }

            }
            /*
                END:  CTG_River
            */


            /*
                BEGIN:  CTG_Dirt
            */
            if(ctg_5 == CTG_Dirt)
            {

                STU_DEBUG_BREAK();

                battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = (Random(4) + UU_DirtTiles_Index + 3);

            }
            /*
                END:  CTG_Dirt
            */


            /*
                BEGIN:  CTG_DeepWater
            */
            if(ctg_5 == CTG_DeepWater)
            {

                STU_DEBUG_BREAK();

                battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = (Random(4) + 47);

            }
            /*
                END:  CTG_DeepWater
            */


            /*
                BEGIN:  CTG_IDK
            */
            if(ctg_5 == CTG_IDK)
            {

                STU_DEBUG_BREAK();

                battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = (Random(4) + 51);

            }
            /*
                END:  CTG_IDK
            */


            /*
                BEGIN:  CTG_Rough
            */
            if(ctg_5 == CTG_IDK)
            {

                STU_DEBUG_BREAK();

            }
            /*
                END:  CTG_Rough
            */

        }

    }

}


// WZD ovr154p10
// drake178: CMB_SetMoveMaps()
/*
; fills out the movement type map arrays based on the
; terrain type data, roads, and central structure
*/
/*

    location_type and city_walls pushed like parameters, but do not get used

*/
void Set_Movement_Cost_Maps(int16_t location_type, int16_t city_walls)
{
    int16_t itr_cgy = 0;  // _CX_
    int16_t itr_cgx = 0;  // _SI_
    int16_t terain_group = 0;  // _DI_

    for(itr_cgy = 0; itr_cgy < COMBAT_GRID_HEIGHT; itr_cgy++)
    {

        for(itr_cgx = 0; itr_cgx < COMBAT_GRID_WIDTH; itr_cgx++)
        {

            terain_group = battlefield->terrain_group[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)];

            switch(terain_group)
            {

                case 0:
                case 2:
                {
                    battlefield->MoveCost_Ground[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]   = 2;
                    battlefield->MoveCost_Teleport[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = 2;
                    battlefield->MoveCost_Ground2[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]  = 2;
                    battlefield->MoveCost_Sailing[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]  = INF;
                } break;

                case 1:
                {
                    battlefield->MoveCost_Ground[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]   = 4;
                    battlefield->MoveCost_Teleport[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = 2;
                    battlefield->MoveCost_Ground2[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]  = 4;
                    battlefield->MoveCost_Sailing[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]  = INF;
                } break;

                case 3:
                {
                    battlefield->MoveCost_Ground[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]   = 4;
                    battlefield->MoveCost_Teleport[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = 2;
                    battlefield->MoveCost_Ground2[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]  = 4;
                    battlefield->MoveCost_Sailing[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]  = 2;
                } break;

                case 4:
                {
                    battlefield->MoveCost_Ground[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]   = INF;
                    battlefield->MoveCost_Teleport[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = 2;
                    battlefield->MoveCost_Ground2[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]  = INF;
                    battlefield->MoveCost_Sailing[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]  = 2;
                } break;

            }

        }

    }


    for(itr_cgy = 0; itr_cgy < COMBAT_GRID_HEIGHT; itr_cgy++)
    {

        for(itr_cgx = 0; itr_cgx < COMBAT_GRID_WIDTH; itr_cgx++)
        {

            terain_group = battlefield->Tile_Road[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)];

            if(
                (terain_group != 0)
                &&
                (terain_group != 0x81)
            )
            {
                battlefield->MoveCost_Ground[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]   = 1;
                battlefield->MoveCost_Teleport[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = 2;
                battlefield->MoveCost_Ground2[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]  = 1;
                battlefield->MoveCost_Sailing[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]  = INF;
            }



        }

    }


    // top-left corner
    for(itr_cgy = 0; itr_cgy < 11; itr_cgy++)
    {

        for(itr_cgx = 0; ((11 - itr_cgy) > itr_cgx); itr_cgx++)
        {

            battlefield->MoveCost_Ground[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]   = INF;
            battlefield->MoveCost_Teleport[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = INF;
            battlefield->MoveCost_Ground2[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]  = INF;
            battlefield->MoveCost_Sailing[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]  = INF;

        }
        
    }


    // bottom-left corner
    for(itr_cgy = 10; itr_cgy < 22; itr_cgy++)
    {

        for(itr_cgx = 0; ((itr_cgy - 9) > itr_cgx); itr_cgx++)
        {

            battlefield->MoveCost_Ground[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]   = INF;
            battlefield->MoveCost_Teleport[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = INF;
            battlefield->MoveCost_Ground2[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]  = INF;
            battlefield->MoveCost_Sailing[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]  = INF;

        }
        
    }


    // top-right corner
    for(itr_cgy = 0; itr_cgy < 12; itr_cgy++)
    {

        for(itr_cgx = (itr_cgy + 10); itr_cgx < COMBAT_GRID_WIDTH; itr_cgx++)
        {

            battlefield->MoveCost_Ground[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]   = INF;
            battlefield->MoveCost_Teleport[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = INF;
            battlefield->MoveCost_Ground2[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]  = INF;
            battlefield->MoveCost_Sailing[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]  = INF;

        }
        
    }


    // bottom-right corner
    for(itr_cgy = 10; itr_cgy < COMBAT_GRID_HEIGHT; itr_cgy++)
    {

        for(itr_cgx = (32 - itr_cgy); itr_cgx < COMBAT_GRID_WIDTH; itr_cgx++)
        {

            battlefield->MoveCost_Ground[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]   = INF;
            battlefield->MoveCost_Teleport[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = INF;
            battlefield->MoveCost_Ground2[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]  = INF;
            battlefield->MoveCost_Sailing[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]  = INF;

        }
        
    }


    // not None, but not City ... So, Lair, Node, Tower
    if(
        (battlefield->Central_Structure != CS_None)
        &&
        (battlefield->Central_Structure != CS_City)
    )
    {

            battlefield->MoveCost_Ground[COMBAT_STRUCTURE_IDX] = INF;
            battlefield->MoveCost_Teleport[COMBAT_STRUCTURE_IDX] = INF;
            battlefield->MoveCost_Ground2[COMBAT_STRUCTURE_IDX] = INF;
            battlefield->MoveCost_Sailing[COMBAT_STRUCTURE_IDX] = INF;

    }

}


// WZD ovr154p11
// drake178: CMB_TerrPatchGen()
/*
; adds an amount of random patches to the combat map
; from the specified terrain group
*/
/*

updates battlefield->terrain_group

*/
void Patch_Terrain_Group(int16_t ctg, int16_t count, int16_t max, int16_t min)
{
    int16_t itr2 = 0;
    int16_t size = 0;
    int16_t uu_0E = 0;
    int16_t direction = 0;
    int16_t next_cgy = 0;
    int16_t next_cgx = 0;
    int16_t random_cgy = 0;
    int16_t random_cgx = 0;
    int16_t itr1 = 0;
    int16_t cgx = 0;  // _SI_
    int16_t cgy = 0;  // _DI_


    for(itr1 = 0; itr1 < count; itr1++)
    {

        random_cgx = (Random(COMBAT_GRID_WIDTH) - 1);
        random_cgy = (Random(COMBAT_GRID_HEIGHT) - 1);

        next_cgx = (random_cgx + MoveDir_NextX_0[Random(4)]);
        next_cgy = (random_cgy + MoveDir_NextY_0[Random(4)]);

        uu_0E = ST_UNDEFINED;

        size = (min + Random(max));

        for(itr2 = 0; itr2 < size; itr2++)
        {

            direction = (Random(4) - 1);

            cgx = (next_cgx + MoveDir_NextX_0[(1 + direction)]);
            cgy = (next_cgy + MoveDir_NextY_0[(1 + direction)]);

            SETMAX(cgx, 20);
            SETMAX(cgy, 21);

            SETMIN(cgx, 0);
            SETMIN(cgy, 0);

            next_cgx = cgx;
            next_cgy = cgy;

            battlefield->terrain_group[((cgy * COMBAT_GRID_WIDTH) + cgx)] = ctg;

        }

    }
    
}


// WZD ovr154p12
// drake178: CMB_GetScreenCoords()
/*
; transforms a set of combat map tile coordinates,
; along with positioning within the tile, into a
; corresponding set of screen pixel coordinates which
; are set into the passed pointer variables
*/
/*

combat grid x
combat grid y
¿ half rows/columns ?


CMB_DrawMap__WIP()
    Combat_Grid_Screen_Coordinates(6, 11, 0, 0, &Left, &Top);
...{6,11} are combat grid coordinates of lair/node structure

CMB_SpawnFigure__WIP()
    Combat_Grid_Screen_Coordinates(cgx, cgy, 0, 0, &position_screen_x, &position_screen_y);

    Combat_Grid_Screen_Coordinates(target_cgx, target_cgy, 0, 0, &target_screen_x, &target_screen_y);

cgx and cgy get passed in
pbr's/in-out's screen_x,screen_y

*/
void Combat_Grid_Screen_Coordinates(int16_t cgx, int16_t cgy, int16_t something_x, int16_t something_y, int16_t * screen_x, int16_t * screen_y)
{

    *screen_x = (((cgx - cgy) * 16) + 158);  /* ¿ + mid x ? */
    *screen_y = (((cgx + cgy) *  8) -  80);  /* ¿ + mid y ? */

    *screen_x += ((something_x - something_y) * 2);
    *screen_y += (something_x + something_y);

}


// WZD ovr154p13
// drake178: CMB_EarthToMud()

// WZD ovr154p14
// drake178: CMB_GetTileX()
/*
; calculates and returns the X coordinate of the combat
; map tile at the selected screen pixel location
*/
/*
    returns cgx

same calc as in Screen_To_Combat_Grid_Cell_X_And_Offset()
Tactical_Combat__WIP() calls with (Grid_X + 4), (Grid_Y + 4)
Assign_Mouse_Image() calls with (Pointer_X() + 4), (Pointer_Y() + 4)

*/
int16_t Get_Combat_Grid_Cell_X(int16_t screen_x, int16_t screen_y)
{
    int16_t combat_grid_cell_x = 0;

    combat_grid_cell_x = ((((screen_x - 158) / 2) + (screen_y + 80)) / 16);

    return combat_grid_cell_x;

}


// WZD ovr154p15
// drake178: CMB_GetTileY()
/*
; calculates and returns the Y coordinate of the combat
; map tile at the selected screen pixel location
*/
/*
    returns cgy

same calc as in Screen_To_Combat_Grid_Cell_Y_And_Offset()
Tactical_Combat__WIP() calls with (Grid_X + 4), (Grid_Y + 4)
Assign_Mouse_Image() calls with (Pointer_X() + 4), (Pointer_Y() + 4)

*/
int16_t Get_Combat_Grid_Cell_Y(int16_t screen_x, int16_t screen_y)
{
    int16_t combat_grid_cell_y = 0;

    combat_grid_cell_y = (((screen_y + 80) - ((screen_x - 158) / 2)) / 16);

    return combat_grid_cell_y;

}


// WZD ovr154p16
// drake178: CMB_GetInTileX()
/*
; calculates and returns in the passed pointers both
; the X coordinate of, and the X offset within, the
; combat tile at the specified screen pixel location
*/
/*

XREF:
    CMB_DrawEntities__WIP()
    CMB_CreateEntity__WIP()

CMB_DrawEntities__WIP()
    Screen_To_Combat_Grid_Cell_X_And_Offset(Pointer_X(), Pointer_Y(), &combat_grid_cell_x, &combat_grid_cell_x_offset);

CMB_CreateEntity__WIP()
    Screen_To_Combat_Grid_Cell_X_And_Offset(draw_x, draw_y, &combat_grid_cell_x, &combat_grid_cell_x_offset);

*/
void Screen_To_Combat_Grid_Cell_X_And_Offset(int16_t screen_x, int16_t screen_y, int16_t * combat_grid_cell_x, int16_t * combat_grid_cell_x_offset)
{
    int16_t IDK = 0;  // _CX_

    IDK = (((screen_x - 158) / 2) + (screen_y + 80));

    *combat_grid_cell_x = (IDK / 16);

    *combat_grid_cell_x_offset = (IDK % 16);

}


// WZD ovr154p17
// drake178: CMB_GetInTileY()
/*
; calculates and returns in the passed pointers both
; the Y coordinate of, and the Y offset within, the
; combat tile at the specified screen pixel location
*/
/*
screen x,y  {0,...,319},{0,...199}

XREF:
    CMB_DrawEntities__WIP()
    CMB_CreateEntity__WIP()

CMB_DrawEntities__WIP()
    Screen_To_Combat_Grid_Cell_Y_And_Offset(Pointer_X(), Pointer_Y(), &combat_grid_cell_y, &combat_grid_cell_y_offset);

CMB_CreateEntity__WIP()
    Screen_To_Combat_Grid_Cell_Y_And_Offset(draw_x, draw_y, &combat_grid_cell_y, &combat_grid_cell_y_offset);

*/
void Screen_To_Combat_Grid_Cell_Y_And_Offset(int16_t screen_x, int16_t screen_y, int16_t * combat_grid_cell_y, int16_t * combat_grid_cell_y_offset)
{

    int16_t IDK = 0;  // _CX_

    IDK = (screen_y + 80) - ((screen_x - 158) / 2);

    *combat_grid_cell_y = (IDK / 16);

    *combat_grid_cell_y_offset = (IDK % 16);

}


// WZD ovr154p18
// drake178: GAME_ReloadWorldData()
/*
; reloads the contents of the World_Data@ allocation
; from the COMBAT.TMP disk file, overwriting it
; completely
*/
/*

*/
void Combat_Cache_Read(void)
{
    int16_t more_world_data_size_PR = 0;
    int16_t world_data_size_PR = 0;  // _SI_
    // int16_t file_handle = 0;  // _DI_
    FILE * file_pointer = 0;
    size_t world_data_size_B = 0;  // DNE in Dasm
    size_t more_world_data_size_B = 0;  // DNE in Dasm

    world_data_size_PR = tmp_World_Data_Paras;

    if(world_data_size_PR > 2048)  /* 2048 PR, 32768 B */
    {
        world_data_size_PR = 2047;
        more_world_data_size_PR = (tmp_World_Data_Paras - 2047);
    }
    else
    {
        more_world_data_size_PR = 0;
    }

    // filehandle = gfopen(str_COMBAT_TMP__ovr154, str_RB__ovr154);
    file_pointer = fopen(str_COMBAT_TMP__ovr154, str_RB__ovr154);

    // gfread(0, World_Data, (world_data_size_PR * 16), 1, file_handle);
    // fread(World_Data, (world_data_size_PR * 16), 1, file_pointer);
    // world_data_size_B = world_data_size_PR * SZ_PARAGRAPH_B;
    world_data_size_B = world_data_size_PR;
    world_data_size_B *= SZ_PARAGRAPH_B;
    fread(World_Data, world_data_size_B, 1, file_pointer);

    // gfread((world_data_size * 16), World_Data, (more_world_data_size_PR * 16), 1, filehandle);
    // fread(World_Data, (more_world_data_size_PR * 16), 1, file_pointer);
    // more_world_data_size_B = (more_world_data_size_PR * 16SZ_PARAGRAPH_B
    more_world_data_size_B = more_world_data_size_PR;
    more_world_data_size_B *= SZ_PARAGRAPH_B;
    fread(World_Data, more_world_data_size_B, 1, file_pointer);

    // gfclose(filehandle);
    fclose(file_pointer);

}


// WZD ovr154p19
// drake178: GAME_DumpWorldData()
/*
; saves the contents of the World_Data@ allocation into
; a disk file named COMBAT.TMP, overwriting any
; existing previous version
*/
/*

*/
void Combat_Cache_Write(void)
{
    int16_t more_world_data_size_PR = 0;
    int16_t world_data_size_PR = 0;  // _SI_
    // int16_t file_handle = 0;  // _DI_
    FILE * file_pointer = 0;
    size_t world_data_size_B = 0;  // DNE in Dasm
    size_t more_world_data_size_B = 0;  // DNE in Dasm

    world_data_size_PR = tmp_World_Data_Paras;

    if(world_data_size_PR > 2048)  /* 2048 PR, 32768 B */
    {
        world_data_size_PR = 2047;
        more_world_data_size_PR = (tmp_World_Data_Paras - 2047);
    }
    else
    {
        more_world_data_size_PR = 0;
    }

    // filehandle = gfopen(str_COMBAT_TMP__ovr154, str_WB__ovr154);
    file_pointer = fopen(str_COMBAT_TMP__ovr154, str_WB__ovr154);

    // gfwrite(0, World_Data, (world_data_size_PR * 16), 1, file_handle);
    // fwrite(World_Data, (world_data_size_PR * 16), 1, file_pointer);
    // world_data_size_B = (world_data_size_PR * SZ_PARAGRAPH_B);
    world_data_size_B = world_data_size_PR;
    world_data_size_B *= SZ_PARAGRAPH_B;
    fwrite(World_Data, world_data_size_B, 1, file_pointer);

    // gfwrite((world_data_size_PR * 16), World_Data, (more_world_data_size_PR * 16), 1, file_handle);
    // fwrite(World_Data, (more_world_data_size_PR * 16), 1, file_pointer);
    // more_world_data_size_B = (more_world_data_size_PR * SZ_PARAGRAPH_B);
    more_world_data_size_B = more_world_data_size_PR;
    more_world_data_size_B *= SZ_PARAGRAPH_B;
    fwrite(World_Data, more_world_data_size_B, 1, file_pointer);


    // gfclose(filehandle);
    fclose(file_pointer);

}


/*
    WIZARDS.EXE  ovr155
*/

// WZD ovr155p01
// drake178: CMB_GetPath()
/*
; calculates the shortest path from the source to the
; destination tile in combat based on the
; CMB_ActiveMoveMap
; returns the path length, or 0 if the tile can't
; be entered by the unit, setting the path array in
; CMB_Path_Xs@ and CMB_Path_Ys@, the length also in
; movement_path_grid_cell_count, and leaving CMB_Path_Costs@ set up
*/
/*

What does it mean for movement_path_grid_cell_count to end as 0?

*/
void Combat_Move_Path_Find(int16_t source_cgx, int16_t source_cgy, int16_t destination_cgx, int16_t destination_cgy)
{
    int16_t move_cost = 0;
    int16_t potential_path_cost = 0;
    int16_t max_i = 0;
    int16_t adjacent_path_cost = 0;  // 1-byte, unsigned
    int16_t max_j = 0;
    int16_t itr_j = 0;
    int16_t itr_i = 0;
    int16_t old_next_cell_index = 0;
    int16_t unstable = 0;
    int16_t itr = 0;
    int16_t ctr = 0;  // _CX_
    int16_t itr_adjacent = 0;  // _DI_
    int16_t adjacent_idx = 0;  // _SI_
    int16_t existing_path_cost = 0;  // DNE in Dasm
    int16_t new_next_cell_index = 0;  // DNE in Dasm
    int16_t next_index = 0;  // DNE in Dasm
    int16_t path_cgx = 0;  // DNE in Dasm
    int16_t path_cgy = 0;  // DNE in Dasm
    int16_t DBG_next_index_cgx = 0;  // DNE in Dasm
    int16_t DBG_next_index_cgy = 0;  // DNE in Dasm


    if (
        (source_cgx == 8)
        &&
        (source_cgy == 12)
        &&
        (destination_cgx == 14)
        &&
        (destination_cgy == 12)
        )
    {

        // DELETEME  STU_DEBUG_BREAK();

    }


    movement_path_grid_cell_count = 0;

    if(IS_INF(CMB_ActiveMoveMap[TERMINAL_VERTEX]))
    {
        return;
    }

    SET_REACH_FROM

    SET_LENGTH

    unstable = ST_TRUE;

    while(unstable == ST_TRUE)
    {
        unstable = ST_FALSE;
        max_j = 19;  // 21 - 2
        max_i = 20;  // 22 - 2
        ctr = 0;

        for(itr_i = 0; itr_i < max_i; itr_i++)
        {

            /*
                BEGIN: First
            */
            move_cost = CMB_ActiveMoveMap[ctr];
            if(!IS_INF(move_cost))
            {
                old_next_cell_index = CMB_NearBuffer_3[ctr];
                for(itr_adjacent = 0; itr_adjacent < 8; itr_adjacent++)
                {
                    adjacent_idx = (ctr + CMB_AdjctOfs_NoWest[itr_adjacent]);
                    // adjacent_idx = (ctr + adjacent_offsets[(((0) * 8) + itr_adjacent)]);
                    if(GTELT(adjacent_idx, 0, COMBAT_GRID_CELL_COUNT))
                    {
                        adjacent_path_cost = CMB_Path_Costs[adjacent_idx];
                        if(!IS_INF(adjacent_path_cost))
                        {
                            potential_path_cost = adjacent_path_cost + move_cost;
                            existing_path_cost = CMB_Path_Costs[ctr];
                            if(existing_path_cost > potential_path_cost)
                            {
                                CMB_NearBuffer_3[ctr] = adjacent_idx;
                                CMB_Path_Costs[ctr] = potential_path_cost;
                                new_next_cell_index = CMB_NearBuffer_3[ctr];
                                if(new_next_cell_index != old_next_cell_index)
                                {
                                    unstable = ST_TRUE;
                                }
                            }
                        }
                    }
                }
            }
            ctr++;
            /*
                END: First
            */

            /*
                BEGIN: Middle
            */
            for(itr_j = 0; itr_j < max_j; itr_j++)
            {
                move_cost = CMB_ActiveMoveMap[ctr];
                if(!IS_INF(move_cost))
                {
                    old_next_cell_index = CMB_NearBuffer_3[ctr];
                    for(itr_adjacent = 0; itr_adjacent < 8; itr_adjacent++)
                    {
                        adjacent_idx = (ctr + CMB_AdjacentOffsets[itr_adjacent]);
                        // adjacent_idx = (ctr + adjacent_offsets[(((1) * 8) + itr_adjacent)]);
                        if(GTELT(adjacent_idx, 0, COMBAT_GRID_CELL_COUNT))
                        {
                            adjacent_path_cost = CMB_Path_Costs[adjacent_idx];
                            if(!IS_INF(adjacent_path_cost))
                            {
                                potential_path_cost = adjacent_path_cost + move_cost;
                                existing_path_cost = CMB_Path_Costs[ctr];
                                if(existing_path_cost > potential_path_cost)
                                {
                                    CMB_NearBuffer_3[ctr] = adjacent_idx;
                                    CMB_Path_Costs[ctr] = potential_path_cost;
                                    new_next_cell_index = CMB_NearBuffer_3[ctr];
                                    if(new_next_cell_index != old_next_cell_index)
                                    {
                                        unstable = ST_TRUE;
                                    }
                                }
                            }
                        }
                    }
                }                                                           
                ctr++;
            }
            /*
                END: Middle
            */

            /*
                BEGIN: Last
            */
            move_cost = CMB_ActiveMoveMap[ctr];
            if(!IS_INF(move_cost))
            {
                old_next_cell_index = CMB_NearBuffer_3[ctr];
                for(itr_adjacent = 0; itr_adjacent < 8; itr_adjacent++)
                {
                    adjacent_idx = (ctr + CMB_AdjctOfs_NoWest[itr_adjacent]);
                    // adjacent_idx = (ctr + adjacent_offsets[(((0) * 8) + itr_adjacent)]);
                    if(GTELT(adjacent_idx, 0, COMBAT_GRID_CELL_COUNT))
                    {
                        adjacent_path_cost = CMB_Path_Costs[adjacent_idx];
                        if(!IS_INF(adjacent_path_cost))
                        {
                            potential_path_cost = adjacent_path_cost + move_cost;
                            existing_path_cost = CMB_Path_Costs[ctr];
                            if(existing_path_cost > potential_path_cost)
                            {
                                CMB_NearBuffer_3[ctr] = adjacent_idx;
                                CMB_Path_Costs[ctr] = potential_path_cost;
                                new_next_cell_index = CMB_NearBuffer_3[ctr];
                                if(new_next_cell_index != old_next_cell_index)
                                {
                                    unstable = ST_TRUE;
                                }
                            }
                        }
                    }
                }
            }
            ctr++;
            /*
                END: Last
            */

        }

    }


    RP_CMB_MoveMap = (int16_t *)CMB_ActiveMoveMap;  /* Why?  ¿ pointer of different data-type ? */

    movement_path_grid_cell_count = 0;

    ctr = ((destination_cgy * COMBAT_GRID_WIDTH) + destination_cgx);  /* starting index */
    // ctr = TERMINAL_VERTEX;

    assert(ctr >= ((COMBAT_GRID_YMIN * COMBAT_GRID_WIDTH) + COMBAT_GRID_XMIN));
    assert(ctr <= ((COMBAT_GRID_YMAX * COMBAT_GRID_WIDTH) + COMBAT_GRID_XMAX));


    // while(CMB_NearBuffer_3[ctr] != ctr)  /* ~ while next index is not current index */
    next_index = CMB_NearBuffer_3[ctr];
    DBG_next_index_cgx = (next_index % COMBAT_GRID_WIDTH);
    DBG_next_index_cgy = (next_index / COMBAT_GRID_WIDTH);
    // while(next_index != ctr)
    while (CMB_NearBuffer_3[ctr] != ctr)
    {

        RP_CMB_MoveMap[movement_path_grid_cell_count] = ctr;

        // ctr = CMB_NearBuffer_3[ctr];
        // ctr = next_index;
        // next_index = CMB_NearBuffer_3[ctr];
        // DBG_next_index_cgx = (next_index % COMBAT_GRID_WIDTH);
        // DBG_next_index_cgy = (next_index / COMBAT_GRID_WIDTH);
        ctr = CMB_NearBuffer_3[ctr];

        movement_path_grid_cell_count++;

    }


    for(itr = 0; itr < movement_path_grid_cell_count; itr++)
    {

        // CMB_Path_Xs[itr] = (RP_CMB_MoveMap[((movement_path_grid_cell_count - 1) - itr)] % COMBAT_GRID_WIDTH);
        // assert(CMB_Path_Xs[itr] >= COMBAT_GRID_XMIN);
        // assert(CMB_Path_Xs[itr] <= COMBAT_GRID_XMAX);

        path_cgx = (RP_CMB_MoveMap[((movement_path_grid_cell_count - 1) - itr)] % COMBAT_GRID_WIDTH);
        assert(path_cgx >= COMBAT_GRID_XMIN);
        assert(path_cgx <= COMBAT_GRID_XMAX);
        CMB_Path_Xs[itr] = path_cgx;

        // CMB_Path_Ys[itr] = (RP_CMB_MoveMap[((movement_path_grid_cell_count - 1) - itr)] / COMBAT_GRID_WIDTH);
        // assert(CMB_Path_Ys[itr] >= COMBAT_GRID_YMIN);
        // assert(CMB_Path_Ys[itr] <= COMBAT_GRID_YMAX);

        path_cgy = (RP_CMB_MoveMap[((movement_path_grid_cell_count - 1) - itr)] / COMBAT_GRID_WIDTH);
        assert(path_cgy >= COMBAT_GRID_XMIN);
        assert(path_cgy <= COMBAT_GRID_XMAX);
        CMB_Path_Ys[itr] = path_cgy;

    }

}

void Combat_Move_Path_Find__v02(int16_t source_cgx, int16_t source_cgy, int16_t destination_cgx, int16_t destination_cgy)
{
    int16_t move_cost = 0;
    int16_t potential_path_cost = 0;
    int16_t max_i = 0;
    int16_t adjacent_path_cost = 0;  // 1-byte, unsigned
    int16_t max_j = 0;
    int16_t itr_j = 0;
    int16_t itr_i = 0;
    int16_t old_next_cell_index = 0;
    int16_t unstable = 0;
    int16_t itr = 0;
    int16_t ctr = 0;  // _CX_
    int16_t itr_adjacent = 0;  // _DI_
    int16_t adjacent_idx = 0;  // _SI_
    int16_t existing_path_cost = 0;  // DNE in Dasm
    int16_t new_next_cell_index = 0;  // DNE in Dasm

    movement_path_grid_cell_count = 0;

    if(IS_INF(CMB_ActiveMoveMap[TERMINAL_VERTEX]))
    {
        return;
    }

    SET_REACH_FROM

    SET_LENGTH

    unstable = ST_TRUE;

    while(unstable == ST_TRUE)
    {
        unstable = ST_FALSE;
        max_j = 19;  // 21 - 2
        max_i = 20;  // 22 - 2
        ctr = 0;

        for(itr_i = 0; itr_i < max_i; itr_i++)
        {

            // NEW_PATH_COST_ALL()
            move_cost = CMB_ActiveMoveMap[ctr];
            if(!IS_INF(move_cost))
            {
                old_next_cell_index = CMB_NearBuffer_3[ctr];
                for(itr_adjacent = 0; itr_adjacent < 4; itr_adjacent++)
                {
                    NEW_PATH_COST_ANY(itr_adjacent)
                }
                for(itr_adjacent = 4; itr_adjacent < 8; itr_adjacent++)
                {
                    NEW_PATH_COST_ANY(itr_adjacent)
                }
            }

            for(itr_j = 0; itr_j < max_j; itr_j++)
            {
                // NEW_PATH_COST_ALL()
                move_cost = CMB_ActiveMoveMap[ctr];
                if(!IS_INF(move_cost))
                {
                    old_next_cell_index = CMB_NearBuffer_3[ctr];
                    for(itr_adjacent = 0; itr_adjacent < 4; itr_adjacent++)
                    {
                        NEW_PATH_COST_ANY(itr_adjacent)
                    }
                    for(itr_adjacent = 4; itr_adjacent < 8; itr_adjacent++)
                    {
                        NEW_PATH_COST_ANY(itr_adjacent)
                    }
                }
            }

            // NEW_PATH_COST_ALL()
            move_cost = CMB_ActiveMoveMap[ctr];
            if(!IS_INF(move_cost))
            {
                old_next_cell_index = CMB_NearBuffer_3[ctr];
                for(itr_adjacent = 0; itr_adjacent < 4; itr_adjacent++)
                {
                    NEW_PATH_COST_ANY(itr_adjacent)
                }
                for(itr_adjacent = 4; itr_adjacent < 8; itr_adjacent++)
                {
                    NEW_PATH_COST_ANY(itr_adjacent)
                }
            }

        }

    }


    RP_CMB_MoveMap = (int16_t *)CMB_ActiveMoveMap;  /* Why?  ¿ pointer of different data-type ? */

    movement_path_grid_cell_count = 0;

    // ctr = ((destination_cgy * COMBAT_GRID_WIDTH) + destination_cgx);  /* starting index */
    ctr = TERMINAL_VERTEX;

    while(CMB_NearBuffer_3[ctr] != ctr)  /* ~ while next index is not current index */
    {

        RP_CMB_MoveMap[movement_path_grid_cell_count] = ctr;

        ctr = CMB_NearBuffer_3[ctr];

        movement_path_grid_cell_count++;

    }


    for(itr = 0; itr < movement_path_grid_cell_count; itr++)
    {

        CMB_Path_Xs[itr] = (RP_CMB_MoveMap[((movement_path_grid_cell_count - 1) - itr)] % COMBAT_GRID_WIDTH);

        CMB_Path_Ys[itr] = (RP_CMB_MoveMap[((movement_path_grid_cell_count - 1) - itr)] / COMBAT_GRID_WIDTH);

    }

}

void Combat_Move_Path_Find__v01(int16_t source_cgx, int16_t source_cgy, int16_t destination_cgx, int16_t destination_cgy)
{
    int16_t move_cost = 0;
    int16_t potential_path_cost = 0;
    int16_t max_i = 0;
    int16_t adjacent_path_cost = 0;  // 1-byte, unsigned
    int16_t max_j = 0;
    int16_t itr_x = 0;
    int16_t itr_i = 0;
    int16_t old_next_cell_index = 0;
    int16_t unstable = 0;
    int16_t itr = 0;
    int16_t ctr = 0;  // _CX_
    int16_t itr_adjacent = 0;  // _DI_
    int16_t adjacent_idx = 0;  // _SI_


    movement_path_grid_cell_count = 0;


//     if(CMB_ActiveMoveMap[((destination_cgy * COMBAT_GRID_WIDTH) + destination_cgx)] == -1)  /* impassible */
//     {
// 
//         return;
// 
//     }
    // if(IS_INF(CMB_ActiveMoveMap[((destination_cgy * COMBAT_GRID_WIDTH) + destination_cgx)]))
    if(IS_INF(CMB_ActiveMoveMap[TERMINAL_VERTEX]))
    {

        return;

    }


    // // MovePath.C  Move_Path_Find()
    // // for(itr = 0; itr < WORLD_SIZE; itr++) { movepath_cost_map->Reach_From[itr] = itr; }
    // for(itr = 0; itr < COMBAT_GRID_CELL_COUNT; itr++)
    // {
    // 
    //     CMB_NearBuffer_3[itr] = itr;  /* ¿ reach from ? */
    // 
    // }
    SET_REACH_FROM

    // // MovePath.C  Move_Path_Find()
    // // for(itr = 0; itr < WORLD_SIZE; itr++) {movepath_cost_map->Reach_Costs[itr] = 255; }
    // for(itr = 0; itr < COMBAT_GRID_CELL_COUNT; itr++)
    // {
    // 
    //     CMB_Path_Costs[itr] = 255;  /* ¿ infinity ? */
    // 
    // }
    // CMB_Path_Costs[((source_cgy * COMBAT_GRID_WIDTH) + source_cgx)] = 0;
    SET_LENGTH


    unstable = ST_TRUE;

    while(unstable == ST_TRUE)
    {

        unstable = ST_FALSE;

        max_j = 19;  // 21 - 2

        max_i = 20;  // 22 - 2

        ctr = 0;

        for(itr_i = 0; itr_i < max_i; itr_i++)
        {

            if(ctr == ((DBG_cgy * COMBAT_GRID_WIDTH) + DBG_cgx))
            {
                STU_DEBUG_BREAK();
            }
            if(ctr == (((DBG_cgy - 1) * COMBAT_GRID_WIDTH) + (DBG_cgx -1)))
            {
                STU_DEBUG_BREAK();
            }

            // movepath_cost = &movepath_cost_map->moves2[CS_Row_Start];
            // move_cost = *movepath_cost++;
            move_cost = CMB_ActiveMoveMap[ctr];  // moves2 cost of cell, given terrain and movement mode

            // if(move_cost != -1)
            if(!IS_INF(move_cost))
            {

                old_next_cell_index = CMB_NearBuffer_3[ctr];

                for(itr_adjacent = 0; itr_adjacent < 4; itr_adjacent++)
                {

                    // // North-West
                    // adj_pos = -1;
                    adjacent_idx = CMB_AdjctOfs_NoWest[itr_adjacent];

                    if(adjacent_idx == ((DBG_cgy * COMBAT_GRID_WIDTH) + DBG_cgx))
                    {
                        STU_DEBUG_BREAK();
                    }


                    // if((adjacent_idx >= 0) && (adjacent_idx < 462))
                    if(GTELT(adjacent_idx, 0, COMBAT_GRID_CELL_COUNT))
                    {

                        // adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
                        adjacent_path_cost = CMB_Path_Costs[adjacent_idx];
                        
                        // if(adjacent_reach_cost != 255)
                        // if(adjacent_path_cost != 255)
                        if(!IS_INF(adjacent_path_cost))
                        {

                            // new_reach_cost = adjacent_reach_cost + tmp_move_cost;
                            // current_reach_cost = *movepath_reach_cost;
                            potential_path_cost = adjacent_path_cost + move_cost;

                            // if(new_reach_cost < current_reach_cost)
                            if(CMB_Path_Costs[ctr] > potential_path_cost)
                            {

                                // *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                                CMB_NearBuffer_3[ctr] = adjacent_idx;

                                // *movepath_reach_cost = new_reach_cost;
                                CMB_Path_Costs[ctr] = potential_path_cost;

                                if(CMB_NearBuffer_3[ctr] != old_next_cell_index)
                                {

                                    // reach_costs_changed = ST_TRUE;
                                    unstable = ST_TRUE;

                                }

                            }

                        }

                    }

                }

                for(itr_adjacent = 4; itr_adjacent < 8; itr_adjacent++)
                {

                    adjacent_idx = CMB_AdjctOfs_NoWest[itr_adjacent];

                    if(
                        (adjacent_idx >= 0)
                        &&
                        (adjacent_idx < COMBAT_GRID_CELL_COUNT)
                    )
                    {

                        adjacent_path_cost = CMB_Path_Costs[adjacent_idx];
                            
                        if(adjacent_path_cost != 255)
                        {

                            potential_path_cost = adjacent_path_cost + move_cost;

                            if(CMB_Path_Costs[ctr] > potential_path_cost)
                            {

                                CMB_NearBuffer_3[ctr] = adjacent_idx;

                                CMB_Path_Costs[ctr] = potential_path_cost;

                                if(CMB_NearBuffer_3[ctr] != old_next_cell_index)
                                {
                                    unstable = ST_TRUE;
                                }

                            }

                        }

                    }

                }

            }

            ctr++;

            for(itr_x = 0; itr_x < max_j; itr_x++)
            {

                move_cost = CMB_ActiveMoveMap[ctr];

                // if(move_cost != -1)
                if(!IS_INF(move_cost))
                {

                    old_next_cell_index = CMB_NearBuffer_3[ctr];

                    for(itr_adjacent = 0; itr_adjacent < 4; itr_adjacent++)
                    {

                        adjacent_idx = CMB_AdjacentOffsets[itr_adjacent];

                        if(
                            (adjacent_idx >= 0)
                            &&
                            (adjacent_idx < COMBAT_GRID_CELL_COUNT)
                        )
                        {

                            adjacent_path_cost = CMB_Path_Costs[adjacent_idx];
                                
                            // if(adjacent_path_cost != 255)
                            if(!IS_INF(adjacent_path_cost))
                            {

                                potential_path_cost = adjacent_path_cost + move_cost;

                                if(CMB_Path_Costs[ctr] > potential_path_cost)
                                {

                                    CMB_NearBuffer_3[ctr] = adjacent_idx;

                                    CMB_Path_Costs[ctr] = potential_path_cost;

                                    if(CMB_NearBuffer_3[ctr] != old_next_cell_index)
                                    {
                                        unstable = ST_TRUE;
                                    }

                                }

                            }

                        }

                    }

                    for(itr_adjacent = 4; itr_adjacent < 8; itr_adjacent++)
                    {

                        adjacent_idx = CMB_AdjacentOffsets[itr_adjacent];

                        if(
                            (adjacent_idx >= 0)
                            &&
                            (adjacent_idx < COMBAT_GRID_CELL_COUNT)
                        )
                        {

                            adjacent_path_cost = CMB_Path_Costs[adjacent_idx];
                                
                            // if(adjacent_path_cost != 255)
                            if(!IS_INF(adjacent_path_cost))
                            {

                                potential_path_cost = adjacent_path_cost + move_cost;

                                if(CMB_Path_Costs[ctr] > potential_path_cost)
                                {

                                    CMB_NearBuffer_3[ctr] = adjacent_idx;

                                    CMB_Path_Costs[ctr] = potential_path_cost;

                                    if(CMB_NearBuffer_3[ctr] != old_next_cell_index)
                                    {
                                        unstable = ST_TRUE;
                                    }

                                }

                            }

                        }

                    }

                }

                ctr++;

            }


            move_cost = CMB_ActiveMoveMap[ctr];

            // if(move_cost != -1)
            if(!IS_INF(move_cost))
            {

                old_next_cell_index = CMB_NearBuffer_3[ctr];

                for(itr_adjacent = 0; itr_adjacent < 4; itr_adjacent++)
                {

                    adjacent_idx = CMB_AdjctOfs_NoEast[itr_adjacent];

                    if(
                        (adjacent_idx >= 0)
                        &&
                        (adjacent_idx < COMBAT_GRID_CELL_COUNT)
                    )
                    {

                        adjacent_path_cost = CMB_Path_Costs[adjacent_idx];
                        
                        // if(adjacent_path_cost != 255)
                        if(!IS_INF(adjacent_path_cost))
                        {

                            potential_path_cost = adjacent_path_cost + move_cost;

                            if(CMB_Path_Costs[ctr] > potential_path_cost)
                            {

                                CMB_NearBuffer_3[ctr] = adjacent_idx;

                                CMB_Path_Costs[ctr] = potential_path_cost;

                                if(CMB_NearBuffer_3[ctr] != old_next_cell_index)
                                {
                                    unstable = ST_TRUE;
                                }

                            }

                        }

                    }

                }

                for(itr_adjacent = 4; itr_adjacent < 8; itr_adjacent++)
                {

                    adjacent_idx = CMB_AdjctOfs_NoEast[itr_adjacent];

                    if(
                        (adjacent_idx >= 0)
                        &&
                        (adjacent_idx < COMBAT_GRID_CELL_COUNT)
                    )
                    {

                        adjacent_path_cost = CMB_Path_Costs[adjacent_idx];
                            
                        // if(adjacent_path_cost != 255)
                        if(!IS_INF(adjacent_path_cost))
                        {

                            potential_path_cost = adjacent_path_cost + move_cost;

                            if(CMB_Path_Costs[ctr] > potential_path_cost)
                            {

                                CMB_NearBuffer_3[ctr] = adjacent_idx;

                                CMB_Path_Costs[ctr] = potential_path_cost;

                                if(CMB_NearBuffer_3[ctr] != old_next_cell_index)
                                {
                                    unstable = ST_TRUE;
                                }

                            }

                        }

                    }

                }

            }

            ctr++;

        }

    }


    RP_CMB_MoveMap = (int16_t *)CMB_ActiveMoveMap;  /* Why?  ¿ pointer of different data-type ? */

    movement_path_grid_cell_count = 0;

    // ctr = ((destination_cgy * COMBAT_GRID_WIDTH) + destination_cgx);  /* starting index */
    ctr = TERMINAL_VERTEX;

    while(CMB_NearBuffer_3[ctr] != ctr)  /* ~ while next index is not current index */
    {

        RP_CMB_MoveMap[movement_path_grid_cell_count] = ctr;

        ctr = CMB_NearBuffer_3[ctr];

        movement_path_grid_cell_count++;

    }


    for(itr = 0; itr < movement_path_grid_cell_count; itr++)
    {

        CMB_Path_Xs[itr] = (RP_CMB_MoveMap[((movement_path_grid_cell_count - 1) - itr)] % COMBAT_GRID_WIDTH);

        CMB_Path_Ys[itr] = (RP_CMB_MoveMap[((movement_path_grid_cell_count - 1) - itr)] / COMBAT_GRID_WIDTH);

    }


}


// WZD ovr155p02
// drake178: CMB_FillReachMap()
/*
; creates a path map from the origin tile, then
; replaces the result array with 0s or 1s depending on
; whether the tile can be reached and entered; storing
; the result in CMB_NearBuffer_3
*/
/*

Assign_Combat_Grids()
    CMB_FillReachMap(battle_units[_active_battle_unit].cgx, battle_units[_active_battle_unit].cgy, useable_moves2);

What's in CMB_ActiveMoveMap[] when this is called?
-1 for *impassible* combat grid cells
movement cost of the combat grid cell  (set in Set_Movement_Cost_Map())

'ctr' is the current array index
...for CMB_ActiveMoveMap[], CMB_NearBuffer_3[], and CMB_Path_Costs[]

*/
void Combat_Move_Path_Valid(int16_t source_cgx, int16_t source_cgy, int16_t moves2)
{
    int16_t move_cost = 0;
    int16_t potential_path_cost = 0;
    int16_t Tile_GetTo_Cost = 0;
    int16_t max_i = 0;
    int16_t adjacent_path_cost = 0;
    int16_t max_j = 0;
    int16_t itr_j = 0;
    int16_t itr_i = 0;
    int16_t old_next_cell_index = 0;
    int16_t unstable = 0;
    int16_t itr = 0;
    int16_t itr_y = 0;
    int16_t itr_x = 0;
    int16_t ctr = 0;  // _CX_
    int16_t itr_adjacent = 0;  // _DI_
    int16_t adjacent_idx = 0;  // _SI_
    int16_t existing_path_cost = 0;  // DNE in Dasm
    int16_t new_next_cell_index = 0;  // DNE in Dasm

    movement_path_grid_cell_count = 0;

    SET_REACH_FROM

    SET_LENGTH

    unstable = ST_TRUE;

    while(unstable == ST_TRUE)
    {
        unstable = ST_FALSE;
        max_j = (COMBAT_GRID_WIDTH  - 2);
        max_i = (COMBAT_GRID_HEIGHT - 2);
        ctr = 0;

        for(itr_i = 0; itr_i < max_i; itr_i++)
        {

            /*
                BEGIN: First
            */
            move_cost = CMB_ActiveMoveMap[ctr];
            if(!IS_INF(move_cost))
            {
                old_next_cell_index = CMB_NearBuffer_3[ctr];
                for(itr_adjacent = 0; itr_adjacent < 8; itr_adjacent++)
                {
                    adjacent_idx = (ctr + CMB_AdjctOfs_NoWest[itr_adjacent]);
                    // adjacent_idx = (ctr + adjacent_offsets[(((0) * 8) + itr_adjacent)]);
                    if(GTELT(adjacent_idx, 0, COMBAT_GRID_CELL_COUNT))
                    {
                        adjacent_path_cost = CMB_Path_Costs[adjacent_idx];
                        if(!IS_INF(adjacent_path_cost))
                        {
                            potential_path_cost = adjacent_path_cost + move_cost;
                            existing_path_cost = CMB_Path_Costs[ctr];
                            if(existing_path_cost > potential_path_cost)
                            {
                                CMB_NearBuffer_3[ctr] = adjacent_idx;
                                CMB_Path_Costs[ctr] = potential_path_cost;
                                new_next_cell_index = CMB_NearBuffer_3[ctr];
                                if(new_next_cell_index != old_next_cell_index)
                                {
                                    unstable = ST_TRUE;
                                }
                            }
                        }
                    }
                }
            }
            ctr++;
            /*
                END: First
            */

            /*
                BEGIN: Middle
            */
            for(itr_j = 0; itr_j < max_j; itr_j++)
            {
                move_cost = CMB_ActiveMoveMap[ctr];
                if(!IS_INF(move_cost))
                {
                    old_next_cell_index = CMB_NearBuffer_3[ctr];
                    for(itr_adjacent = 0; itr_adjacent < 8; itr_adjacent++)
                    {
                        adjacent_idx = (ctr + CMB_AdjacentOffsets[itr_adjacent]);
                        // adjacent_idx = (ctr + adjacent_offsets[(((1) * 8) + itr_adjacent)]);
                        if(GTELT(adjacent_idx, 0, COMBAT_GRID_CELL_COUNT))
                        {
                            adjacent_path_cost = CMB_Path_Costs[adjacent_idx];
                            if(!IS_INF(adjacent_path_cost))
                            {
                                potential_path_cost = adjacent_path_cost + move_cost;
                                existing_path_cost = CMB_Path_Costs[ctr];
                                if(existing_path_cost > potential_path_cost)
                                {
                                    CMB_NearBuffer_3[ctr] = adjacent_idx;
                                    CMB_Path_Costs[ctr] = potential_path_cost;
                                    new_next_cell_index = CMB_NearBuffer_3[ctr];
                                    if(new_next_cell_index != old_next_cell_index)
                                    {
                                        unstable = ST_TRUE;
                                    }
                                }
                            }
                        }
                    }
                }                                                           
                ctr++;
            }
            /*
                END: Middle
            */

            /*
                BEGIN: Last
            */
            move_cost = CMB_ActiveMoveMap[ctr];

            if(!IS_INF(move_cost))
            {
                old_next_cell_index = CMB_NearBuffer_3[ctr];
                for(itr_adjacent = 0; itr_adjacent < 8; itr_adjacent++)
                {
                    adjacent_idx = (ctr + CMB_AdjctOfs_NoEast[itr_adjacent]);
                    // adjacent_idx = (ctr + adjacent_offsets[(((2) * 8) + itr_adjacent)]);
                    if(GTELT(adjacent_idx, 0, COMBAT_GRID_CELL_COUNT))
                    {
                        adjacent_path_cost = CMB_Path_Costs[adjacent_idx];
                        if(!IS_INF(adjacent_path_cost))
                        {
                            potential_path_cost = adjacent_path_cost + move_cost;
                            existing_path_cost = CMB_Path_Costs[ctr];
                            if(existing_path_cost > potential_path_cost)
                            {
                                CMB_NearBuffer_3[ctr] = adjacent_idx;
                                CMB_Path_Costs[ctr] = potential_path_cost;
                                new_next_cell_index = CMB_NearBuffer_3[ctr];
                                if(new_next_cell_index != old_next_cell_index)
                                {
                                    unstable = ST_TRUE;
                                }
                            }
                        }
                    }
                }
            }                                                           
            ctr++;
            /*
                END: Last
            */
            
        }

    }


    for(itr_y = 0; itr_y < COMBAT_GRID_HEIGHT; itr_y++)
    {

        for(itr_x = 0; itr_x < COMBAT_GRID_WIDTH; itr_x++)
        {

            // if(
            //     (itr_x == DBG_cgx)
            //     &&
            //     (itr_y == DBG_cgy)
            //     )
            // {
            //     // STU_DEBUG_BREAK();
            //     DBG_move_cost_1 = CMB_ActiveMoveMap[DBG_array_index_1];
            //     DBG_move_cost_2 = CMB_ActiveMoveMap[DBG_array_index_2];
            //     DBG_path_cost_1 = CMB_Path_Costs[DBG_array_index_1];
            //     DBG_path_cost_2 = CMB_Path_Costs[DBG_array_index_2];
            // }

            ctr = ((itr_y * COMBAT_GRID_WIDTH) + itr_x);

            CMB_NearBuffer_3[ctr] = ST_FALSE;

            if(!IS_INF(CMB_Path_Costs[ctr]))
            {

                move_cost = CMB_ActiveMoveMap[ctr];

                Tile_GetTo_Cost = (CMB_Path_Costs[ctr] - move_cost);

                if(moves2 > Tile_GetTo_Cost)
                {

                    CMB_NearBuffer_3[ctr] = ST_TRUE;

                }

            }

        }

    }

}

void Combat_Move_Path_Valid__v02(int16_t source_cgx, int16_t source_cgy, int16_t moves2)
{
    int16_t move_cost = 0;
    int16_t potential_path_cost = 0;
    int16_t Tile_GetTo_Cost = 0;
    int16_t max_i = 0;
    int16_t adjacent_path_cost = 0;
    int16_t max_j = 0;
    int16_t itr_j = 0;
    int16_t itr_i = 0;
    int16_t old_next_cell_index = 0;
    int16_t unstable = 0;
    int16_t itr = 0;
    int16_t itr_y = 0;
    int16_t itr_x = 0;
    int16_t ctr = 0;  // _CX_
    int16_t itr_adjacent = 0;  // _DI_
    int16_t adjacent_idx = 0;  // _SI_
    int16_t existing_path_cost = 0;  // DNE in Dasm
    int16_t new_next_cell_index = 0;  // DNE in Dasm
    int16_t DBG_array_index_1 = 0;
    int16_t DBG_array_index_2 = 0;
    uint8_t DBG_path_cost_1 = 0;
    uint8_t DBG_path_cost_2 = 0;
    uint8_t DBG_move_cost_1 = 0;
    uint8_t DBG_move_cost_2 = 0;
    int16_t DBG_ctr = 0;

    if(
        (source_cgx == DBG_cgx)
        &&
        (source_cgy == DBG_cgy)
        )
    {
        // STU_DEBUG_BREAK();
        DBG_array_index_1 = ((DBG_cgy * COMBAT_GRID_WIDTH) + DBG_cgx);  // current combat grid cell
        DBG_array_index_2 = (((DBG_cgy - 1) * COMBAT_GRID_WIDTH) + (DBG_cgx - 1));  // NW
        DBG_move_cost_1 = CMB_ActiveMoveMap[DBG_array_index_1];
        DBG_move_cost_2 = CMB_ActiveMoveMap[DBG_array_index_2];
    }


    movement_path_grid_cell_count = 0;

    SET_REACH_FROM
    SET_LENGTH

    unstable = ST_TRUE;

    while(unstable == ST_TRUE)
    {
        unstable = ST_FALSE;
        max_j = (COMBAT_GRID_WIDTH  - 2);
        max_i = (COMBAT_GRID_HEIGHT - 2);
        ctr = 0;

        for(itr_i = 0; itr_i < max_i; itr_i++)
        {

            if(ctr == ((11 * COMBAT_GRID_WIDTH) + 13))
            {
                STU_DEBUG_BREAK();
            }
            if(ctr == ((11 * COMBAT_GRID_WIDTH) + 14))
            {
                STU_DEBUG_BREAK();
            }
            if(ctr == ((11 * COMBAT_GRID_WIDTH) + 15))
            {
                STU_DEBUG_BREAK();
            }
            // NEW_PATH_COST_ALL()
            move_cost = CMB_ActiveMoveMap[ctr];
            if(!IS_INF(move_cost))
            {
                old_next_cell_index = CMB_NearBuffer_3[ctr];
                for(itr_adjacent = 0; itr_adjacent < 8; itr_adjacent++)
                {
                    // NEW_PATH_COST_ANY(itr_adjacent)
                    // adjacent_idx = adjacent_offsets[(((_value_) * 8) + itr_adjacent)];
                    // adjacent_idx = adjacent_offsets[(((itr_adjacent) * 8) + itr_adjacent)];
                    // adjacent_idx = (ctr + adjacent_offsets[(((itr_adjacent) * 8) + itr_adjacent)]);
                    adjacent_idx = (ctr + CMB_AdjctOfs_NoWest[itr_adjacent]);
                    
                    if(adjacent_idx == ((DBG_cgy * COMBAT_GRID_WIDTH) + DBG_cgx))
                    {
                        STU_DEBUG_BREAK();
                    }     
                    if(GTELT(adjacent_idx, 0, COMBAT_GRID_CELL_COUNT))
                    {
                        adjacent_path_cost = CMB_Path_Costs[adjacent_idx];
                        if(!IS_INF(adjacent_path_cost))
                        {
                            potential_path_cost = adjacent_path_cost + move_cost;
                            // if(CMB_Path_Costs[ctr] > potential_path_cost)
                            existing_path_cost = CMB_Path_Costs[ctr];
                            if(existing_path_cost > potential_path_cost)
                            {
                                CMB_NearBuffer_3[ctr] = adjacent_idx;
                                CMB_Path_Costs[ctr] = potential_path_cost;
                                // if(CMB_NearBuffer_3[ctr] != old_next_cell_index)
                                new_next_cell_index = CMB_NearBuffer_3[ctr];
                                if(new_next_cell_index != old_next_cell_index)
                                {
                                    unstable = ST_TRUE;
                                    if((ctr == ((11 * 21) + 13)) && (adjacent_idx == ((12 * 21) + 14))) { DBG_PFA_1311_set = ST_TRUE; }
                                    if((ctr == ((11 * 21) + 14)) && (adjacent_idx == ((12 * 21) + 14))) { DBG_PFA_1411_set = ST_TRUE; }
                                    if((ctr == ((11 * 21) + 15)) && (adjacent_idx == ((12 * 21) + 14))) { DBG_PFA_1511_set = ST_TRUE; }

                                    if((ctr == ((11 * 21) + 13)) && (adjacent_idx == ((12 * 21) + 14)) && (!(DBG_ctr < COMBAT_GRID_CELL_COUNT))) { STU_DEBUG_BREAK(); }

                                    if (DBG_ctr > (22 * 462)) { STU_DEBUG_BREAK(); }

                                }
                            }
                        }
                    }
                }
            }
            ctr++;
            DBG_ctr++;
            // STU_DEBUG_BREAK();

            for(itr_j = 0; itr_j < max_j; itr_j++)
            {

                if (ctr == ((11 * COMBAT_GRID_WIDTH) + 13))
                {
                    // STU_DEBUG_BREAK();
                }
                if (ctr == ((11 * COMBAT_GRID_WIDTH) + 14))
                {
                    // STU_DEBUG_BREAK();
                }
                if (ctr == ((11 * COMBAT_GRID_WIDTH) + 15))
                {
                    // STU_DEBUG_BREAK();
                }
                //  if((itr_i == (DBG_cgy - 1)) && (itr_j == (DBG_cgx -1))) { STU_DEBUG_BREAK(); }
                // NEW_PATH_COST_ALL()
                move_cost = CMB_ActiveMoveMap[ctr];
                if(!IS_INF(move_cost))
                {
                    old_next_cell_index = CMB_NearBuffer_3[ctr];
                    for(itr_adjacent = 0; itr_adjacent < 8; itr_adjacent++)
                    {
                        // NEW_PATH_COST_ANY(itr_adjacent)
                        // adjacent_idx = adjacent_offsets[(((_value_) * 8) + itr_adjacent)];
                        // adjacent_idx = adjacent_offsets[(((itr_adjacent) * 8) + itr_adjacent)];
                        // adjacent_idx = (ctr + adjacent_offsets[(((itr_adjacent) * 8) + itr_adjacent)]);
                        adjacent_idx = (ctr + CMB_AdjacentOffsets[itr_adjacent]);
                        if(adjacent_idx == ((DBG_cgy * COMBAT_GRID_WIDTH) + DBG_cgx))
                        {
                            // STU_DEBUG_BREAK();
                        }     
                        if(
                            (ctr == ((11 * COMBAT_GRID_WIDTH) + 13))
                            &&
                            (adjacent_idx == ((DBG_cgy * COMBAT_GRID_WIDTH) + DBG_cgx))
                        )
                        {
                            // STU_DEBUG_BREAK();
                        }
                        if(GTELT(adjacent_idx, 0, COMBAT_GRID_CELL_COUNT))
                        {
                            adjacent_path_cost = CMB_Path_Costs[adjacent_idx];
                            if(!IS_INF(adjacent_path_cost))
                            {
                                potential_path_cost = adjacent_path_cost + move_cost;
                                // if(CMB_Path_Costs[ctr] > potential_path_cost)
                                existing_path_cost = CMB_Path_Costs[ctr];
                                if(existing_path_cost > potential_path_cost)
                                {
                                    CMB_NearBuffer_3[ctr] = adjacent_idx;
                                    CMB_Path_Costs[ctr] = potential_path_cost;
                                    // if(CMB_NearBuffer_3[ctr] != old_next_cell_index)
                                    new_next_cell_index = CMB_NearBuffer_3[ctr];
                                    if(new_next_cell_index != old_next_cell_index)
                                    {
                                        unstable = ST_TRUE;
                                        if((ctr == ((11 * 21) + 13)) && (adjacent_idx == ((12 * 21) + 14))) { DBG_PFA_1311_set = ST_TRUE; }
                                        if((ctr == ((11 * 21) + 14)) && (adjacent_idx == ((12 * 21) + 14))) { DBG_PFA_1411_set = ST_TRUE; }
                                        if((ctr == ((11 * 21) + 15)) && (adjacent_idx == ((12 * 21) + 14))) { DBG_PFA_1511_set = ST_TRUE; }

                                        if((ctr == ((11 * 21) + 13)) && (adjacent_idx == ((12 * 21) + 14)) && (!(DBG_ctr < COMBAT_GRID_CELL_COUNT))) { STU_DEBUG_BREAK(); }

                                        if (DBG_ctr > (22 * 462)) { STU_DEBUG_BREAK(); }

                                    }
                                }
                            }
                        }
                    }
                }                                                           
                ctr++;
                DBG_ctr++;
                // STU_DEBUG_BREAK();

            }

            if (ctr == ((11 * COMBAT_GRID_WIDTH) + 13))
            {
                STU_DEBUG_BREAK();
            }
            if (ctr == ((11 * COMBAT_GRID_WIDTH) + 14))
            {
                STU_DEBUG_BREAK();
            }
            if (ctr == ((11 * COMBAT_GRID_WIDTH) + 15))
            {
                STU_DEBUG_BREAK();
            }
            // NEW_PATH_COST_ALL()
            move_cost = CMB_ActiveMoveMap[ctr];
            if(!IS_INF(move_cost))
            {
                old_next_cell_index = CMB_NearBuffer_3[ctr];
                for(itr_adjacent = 0; itr_adjacent < 8; itr_adjacent++)
                {
                    // NEW_PATH_COST_ANY(itr_adjacent)
                    // adjacent_idx = adjacent_offsets[(((_value_) * 8) + itr_adjacent)];
                    // adjacent_idx = adjacent_offsets[(((itr_adjacent) * 8) + itr_adjacent)];
                    // adjacent_idx = (ctr + adjacent_offsets[(((itr_adjacent) * 8) + itr_adjacent)]);
                    adjacent_idx = (ctr + CMB_AdjctOfs_NoEast[itr_adjacent]);
                    if(adjacent_idx == ((DBG_cgy * COMBAT_GRID_WIDTH) + DBG_cgx))
                    {
                        STU_DEBUG_BREAK();
                    }     
                    if(GTELT(adjacent_idx, 0, COMBAT_GRID_CELL_COUNT))
                    {
                        adjacent_path_cost = CMB_Path_Costs[adjacent_idx];
                        if(!IS_INF(adjacent_path_cost))
                        {
                            potential_path_cost = adjacent_path_cost + move_cost;
                            // if(CMB_Path_Costs[ctr] > potential_path_cost)
                            existing_path_cost = CMB_Path_Costs[ctr];
                            if(existing_path_cost > potential_path_cost)
                            {
                                CMB_NearBuffer_3[ctr] = adjacent_idx;
                                CMB_Path_Costs[ctr] = potential_path_cost;
                                // if(CMB_NearBuffer_3[ctr] != old_next_cell_index)
                                new_next_cell_index = CMB_NearBuffer_3[ctr];
                                if(new_next_cell_index != old_next_cell_index)
                                {
                                    unstable = ST_TRUE;
                                    if((ctr == ((11 * 21) + 13)) && (adjacent_idx == ((12 * 21) + 14))) { DBG_PFA_1311_set = ST_TRUE; }
                                    if((ctr == ((11 * 21) + 14)) && (adjacent_idx == ((12 * 21) + 14))) { DBG_PFA_1411_set = ST_TRUE; }
                                    if((ctr == ((11 * 21) + 15)) && (adjacent_idx == ((12 * 21) + 14))) { DBG_PFA_1511_set = ST_TRUE; }

                                    if((ctr == ((11 * 21) + 13)) && (adjacent_idx == ((12 * 21) + 14)) && (!(DBG_ctr < COMBAT_GRID_CELL_COUNT))) { STU_DEBUG_BREAK(); }

                                    if (DBG_ctr > (22 * 462)) { STU_DEBUG_BREAK(); }

                                }
                            }
                        }
                    }
                }
                
            }                                                           
            ctr++;
            DBG_ctr++;
            // STU_DEBUG_BREAK();
            
        }

    }


    for(itr_y = 0; itr_y < COMBAT_GRID_HEIGHT; itr_y++)
    {

        for(itr_x = 0; itr_x < COMBAT_GRID_WIDTH; itr_x++)
        {

            if(
                (itr_x == DBG_cgx)
                &&
                (itr_y == DBG_cgy)
                )
            {
                // STU_DEBUG_BREAK();
                DBG_move_cost_1 = CMB_ActiveMoveMap[DBG_array_index_1];
                DBG_move_cost_2 = CMB_ActiveMoveMap[DBG_array_index_2];
                DBG_path_cost_1 = CMB_Path_Costs[DBG_array_index_1];
                DBG_path_cost_2 = CMB_Path_Costs[DBG_array_index_2];
            }

            ctr = ((itr_y * COMBAT_GRID_WIDTH) + itr_x);

            CMB_NearBuffer_3[ctr] = ST_FALSE;

            if(!IS_INF(CMB_Path_Costs[ctr]))
            {

                move_cost = CMB_ActiveMoveMap[ctr];

                Tile_GetTo_Cost = (CMB_Path_Costs[ctr] - move_cost);

                if(moves2 > Tile_GetTo_Cost)
                {

                    CMB_NearBuffer_3[ctr] = ST_TRUE;

                }

            }

        }

    }

}

void Combat_Move_Path_Valid__v01(int16_t source_cgx, int16_t source_cgy, int16_t moves2)
{
    int16_t move_cost = 0;
    int16_t potential_path_cost = 0;
    int16_t Tile_GetTo_Cost = 0;
    int16_t max_i = 0;
    int16_t adjacent_path_cost = 0;
    int16_t max_j = 0;
    int16_t itr_j = 0;
    int16_t itr_i = 0;
    int16_t old_next_cell_index = 0;
    int16_t unstable = 0;
    int16_t itr = 0;
    int16_t itr_y = 0;
    int16_t itr_x = 0;
    int16_t ctr = 0;  // _CX_
    int16_t itr_adjacent = 0;  // _DI_
    int16_t adjacent_idx = 0;  // _SI_
    int16_t DBG_array_index_1 = 0;
    int16_t DBG_array_index_2 = 0;
    uint8_t DBG_path_cost_1 = 0;
    uint8_t DBG_path_cost_2 = 0;
    uint8_t DBG_move_cost_1 = 0;
    uint8_t DBG_move_cost_2 = 0;

    if(
        (source_cgx == DBG_cgx)
        &&
        (source_cgy == DBG_cgy)
        )
    {
        // STU_DEBUG_BREAK();
        DBG_array_index_1 = ((DBG_cgy * COMBAT_GRID_WIDTH) + DBG_cgx);  // current combat grid cell
        DBG_array_index_2 = (((DBG_cgy - 1) * COMBAT_GRID_WIDTH) + (DBG_cgx - 1));  // NW
        DBG_move_cost_1 = CMB_ActiveMoveMap[DBG_array_index_1];
        DBG_move_cost_2 = CMB_ActiveMoveMap[DBG_array_index_2];
    }


    movement_path_grid_cell_count = 0;


    // // MovePath.C  Move_Path_Find()
    // // for(itr = 0; itr < WORLD_SIZE; itr++) { movepath_cost_map->Reach_From[itr] = itr; }
    // for(itr = 0; itr < COMBAT_GRID_CELL_COUNT; itr++)
    // {
    //     
    //     CMB_NearBuffer_3[itr] = itr;
    // 
    // }
    SET_REACH_FROM

    // // MovePath.C  Move_Path_Find()
    // // for(itr = 0; itr < WORLD_SIZE; itr++) {movepath_cost_map->Reach_Costs[itr] = 255; }
    // for(itr = 0; itr < COMBAT_GRID_CELL_COUNT; itr++)
    // {
    // 
    //     CMB_Path_Costs[itr] = 255;  // ¿ ~ inifinity ?
    // 
    // }
    // CMB_Path_Costs[((destination_cgy * COMBAT_GRID_WIDTH) + destination_cgx)] = 0;  /* manually set the current cell's cost to zero */
    SET_LENGTH


    unstable = ST_TRUE;

    while(unstable == ST_TRUE)
    {

        unstable = ST_FALSE;

        max_j = 19;  // ¿ not 21 ?

        max_i = 20;  // ¿ not 22 ?

        ctr = 0;

        for(itr_i = 0; itr_i < max_i; itr_i++)
        {

            /*
                First Column
            */

            // ~ move_cost = *movepath_cost++;
            move_cost = CMB_ActiveMoveMap[ctr];

            // if(move_cost != -1)
            if(!IS_INF(move_cost))
            {

                // movepath_reach_from = &movepath_cost_map->Reach_From[CS_Row_Start];
                old_next_cell_index = CMB_NearBuffer_3[ctr];

                for(itr_adjacent = 0; itr_adjacent < 8; itr_adjacent++)
                {

                    adjacent_idx = CMB_AdjctOfs_NoWest[itr_adjacent];

                    if(
                        (adjacent_idx >= 0)
                        &&
                        (adjacent_idx < COMBAT_GRID_CELL_COUNT)
                    )
                    {

                        adjacent_path_cost = CMB_Path_Costs[adjacent_idx];
                            
                        // if(adjacent_path_cost != 255)
                        if(!IS_INF(adjacent_path_cost))
                        {

                            potential_path_cost = adjacent_path_cost + move_cost;

                            if(CMB_Path_Costs[ctr] > potential_path_cost)
                            {

                                CMB_NearBuffer_3[ctr] = adjacent_idx;

                                CMB_Path_Costs[ctr] = potential_path_cost;

                                if(CMB_NearBuffer_3[ctr] != old_next_cell_index)
                                {
                                    unstable = ST_TRUE;
                                }

                            }

                        }

                    }

                }

            }

            ctr++;

            /*
                Middle Columns
            */
            for(itr_j = 0; itr_j < max_j; itr_j++)
            {

                move_cost = CMB_ActiveMoveMap[ctr];

                // if(move_cost != -1)
                if(!IS_INF(move_cost))
                {

                    old_next_cell_index = CMB_NearBuffer_3[ctr];

                    for(itr_adjacent = 0; itr_adjacent < 8; itr_adjacent++)
                    {

                        adjacent_idx = CMB_AdjacentOffsets[itr_adjacent];

                        if(
                            (adjacent_idx >= 0)
                            &&
                            (adjacent_idx < COMBAT_GRID_CELL_COUNT)
                        )
                        {

                            adjacent_path_cost = CMB_Path_Costs[adjacent_idx];
                                
                            // if(adjacent_path_cost != 255)
                            if(!IS_INF(adjacent_path_cost))
                            {

                                potential_path_cost = adjacent_path_cost + move_cost;

                                if(CMB_Path_Costs[ctr] > potential_path_cost)
                                {

                                    CMB_NearBuffer_3[ctr] = adjacent_idx;

                                    CMB_Path_Costs[ctr] = potential_path_cost;

                                    if(CMB_NearBuffer_3[ctr] != old_next_cell_index)
                                    {
                                        unstable = ST_TRUE;
                                    }

                                }

                            }

                        }

                    }

                }

                ctr++;

            }

            /*
                Last Column
            */

            move_cost = CMB_ActiveMoveMap[ctr];

            // if(move_cost != -1)
            if(!IS_INF(move_cost))
            {

                old_next_cell_index = CMB_NearBuffer_3[ctr];

                for(itr_adjacent = 0; itr_adjacent < 8; itr_adjacent++)
                {

                    adjacent_idx = CMB_AdjctOfs_NoEast[itr_adjacent];

                    if(
                        (adjacent_idx >= 0)
                        &&
                        (adjacent_idx < COMBAT_GRID_CELL_COUNT)
                    )
                    {

                        adjacent_path_cost = CMB_Path_Costs[adjacent_idx];
                            
                        // if(adjacent_path_cost != 255)
                        if(!IS_INF(adjacent_path_cost))
                        {

                            potential_path_cost = adjacent_path_cost + move_cost;

                            if(CMB_Path_Costs[ctr] > potential_path_cost)
                            {

                                CMB_NearBuffer_3[ctr] = adjacent_idx;

                                CMB_Path_Costs[ctr] = potential_path_cost;

                                if(CMB_NearBuffer_3[ctr] != old_next_cell_index)
                                {
                                    unstable = ST_TRUE;
                                }

                            }

                        }

                    }

                }

            }

            ctr++;

        }

    }


    for(itr_y = 0; itr_y < COMBAT_GRID_HEIGHT; itr_y++)
    {

        for(itr_x = 0; itr_x < COMBAT_GRID_WIDTH; itr_x++)
        {

            if(
                (itr_x == DBG_cgx)
                &&
                (itr_y == DBG_cgy)
                )
            {
                // STU_DEBUG_BREAK();
                DBG_move_cost_1 = CMB_ActiveMoveMap[DBG_array_index_1];
                DBG_move_cost_2 = CMB_ActiveMoveMap[DBG_array_index_2];
                DBG_path_cost_1 = CMB_Path_Costs[DBG_array_index_1];
                DBG_path_cost_2 = CMB_Path_Costs[DBG_array_index_2];
            }

            ctr = ((itr_y * COMBAT_GRID_WIDTH) + itr_x);

            CMB_NearBuffer_3[ctr] = ST_FALSE;

            if(CMB_Path_Costs[ctr] != 255)
            {

                move_cost = CMB_ActiveMoveMap[ctr];

                Tile_GetTo_Cost = (CMB_Path_Costs[ctr] - move_cost);

                if(moves2 > Tile_GetTo_Cost)
                {

                    CMB_NearBuffer_3[ctr] = ST_TRUE;

                }

            }

        }

    }

}



/*
    WIZARDS.EXE  ovr163
*/

// WZD ovr163p01
// drake178: CMB_LoadTerrainGFX()
/*
; maps in the TILEX EMM handle, and if the combat
; terrain is not water, loads the appropriate tile,
; tree, and rock images into it
*/
/*

loads set of 48 terrain pictures, 5 trees, 5 rocks

*/
void Load_Combat_Terrain_Pictures(int16_t cts, int16_t wp)
{

    char combat_terrain_set_lbx_filename[LEN_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    SAMB_ptr temp_seg = 0;
    int16_t itr = 0;  // _SI_

    EMM_TILEX_Init__HACK();

    if(cts != cts_Water)
    {

        if(
            (cts == cts_Plains)
            ||
            (cts == cts_Hills)
            ||
            (cts == cts_Forest)
        )
        {

            if(wp == 0)  /* Arcanus */
            {

                strcpy(combat_terrain_set_lbx_filename, cmbgrass_lbx_file__ovr163);

            }
            else
            {

                strcpy(combat_terrain_set_lbx_filename, cmbgrasc_lbx_file__ovr163);

            }

        }
        else if(cts == cts_Desert)
        {
            
            if(wp == 0)  /* Arcanus */
            {

                strcpy(combat_terrain_set_lbx_filename, cmbdesrt_lbx_file__ovr163);

            }
            else
            {

                strcpy(combat_terrain_set_lbx_filename, cmbdesrc_lbx_file__ovr163);

            }

        }
        else if(cts == cts_Mountains)
        {
            
            if(wp == 0)  /* Arcanus */
            {

                strcpy(combat_terrain_set_lbx_filename, cmbmount_lbx_file__ovr163);

            }
            else
            {

                strcpy(combat_terrain_set_lbx_filename, cmbmounc_lbx_file__ovr163);

            }

        }
        else if(cts == cts_Tundra)
        {
            
            if(wp == 0)  /* Arcanus */
            {

                strcpy(combat_terrain_set_lbx_filename, cmbtundr_lbx_file__ovr163);

            }
            else
            {

                strcpy(combat_terrain_set_lbx_filename, cmbtundc_lbx_file__ovr163);

            }

        }

    }

    temp_seg = Allocate_First_Block(EMS_PFBA, 1);

    for(itr = 0; itr < 48; itr++)
    {

        // _combat_terrain_pict_segs[itr] = LBX_Reload_Next(combat_terrain_set_lbx_filename, itr, EMS_PFBA);
        _combat_terrain_pict_segs[itr] = LBX_Reload_Next(combat_terrain_set_lbx_filename, 0, EMS_PFBA);

    }

    for(itr = 0; itr < 5; itr++)
    {

        IMG_CMB_Trees[itr] = LBX_Reload_Next(combat_terrain_set_lbx_filename, (48 + itr), EMS_PFBA);

    }

    for(itr = 0; itr < 5; itr++)
    {

        IMG_CMB_Rocks[itr] = LBX_Reload_Next(combat_terrain_set_lbx_filename, (53 + itr), EMS_PFBA);

    }

}


// WZD ovr163p02
// drake178: CMB_ComposeBackgrnd()
/*
; composes the combat background (tile images and
; bottom UI base image) into the current draw frame,
; advances all combat background animations, and saves
; the screen into VGA frame 3 ($A800)
*/
/*

CMB_Terrain_Init__WIP(wx, wy, wp);
Load_Combat_Terrain_Pictures(cts, wp);
Generate_Combat_Map__WIP(...)
CMB_ComposeBackgrnd__WIP();
*/
void CMB_ComposeBackgrnd__WIP(void)
{
    int16_t combat_terrain_type = 0;
    int16_t cgy = 0;
    int16_t cgx = 0;
    int16_t screen_y = 0;
    int16_t screen_x = 0;
    int16_t IDK_base_cgc1 = 0;
    int16_t IDK_base_cgc2 = 0;
    int16_t itr_x = 0;  // _DI_
    int16_t itr_y = 0;  // _SI_

    dbg_prn("DEBUG: [%s, %d]: BEGIN: CMB_ComposeBackgrnd__WIP()\n", __FILE__, __LINE__);

    Set_Page_Off();

    Reset_Window();

    /* ~== draw terrain picts */ Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, COMBAT_MAP_YMAX, ST_GRAY);

    EMM_TILEX_Init__HACK();

    for(itr_y = 0; itr_y < 22; itr_y++)
    {
        CALC_BASE_CGC2;
        CALC_BASE_CGC1;
        for(itr_x = 0; itr_x < 11; itr_x++)
        {
            CALC_CGX;
            CALC_CGY;
            CALC_SCREEN_X;
            CALC_SCREEN_Y;
            combat_terrain_type = battlefield->terrain_type[((cgy * COMBAT_GRID_WIDTH) + cgx)];
            assert(combat_terrain_type >= 0);
            assert(combat_terrain_type < 48);
            // if(screen_x > 0 && screen_y > 0 && screen_x < 320 - 32 && screen_y < 200 - 16)
            //     FLIC_Draw(screen_x, screen_y, _combat_terrain_pict_segs[combat_terrain_type]);
            if(combat_terrain_type >= 56)
            {
                STU_DEBUG_BREAK();
                combat_terrain_type = ((cgy + cgx) & 0x3);  // ¿ " base tile" ?
            }
            if(combat_terrain_type < 48)
            {
                Clipped_Draw(screen_x, screen_y, _combat_terrain_pict_segs[combat_terrain_type]);
                // nope, can't handle negative sx,sy  FLIC_Draw(screen_x, screen_y, _combat_terrain_pict_segs[combat_terrain_type]);
            }
        }
    }

    CMB_RoadAnimStage = ((CMB_RoadAnimStage + 1) % 5);
    CMB_WaterAnimStage = ((CMB_WaterAnimStage + 1) % 5);
    CMB_CNodeAnimStage = ((CMB_CNodeAnimStage + 1) % 8);
    if(G_CMB_MWallAnimSkip == ST_FALSE)
    {
        G_CMB_MWallAnimStage = ((G_CMB_MWallAnimStage + 1) % 4);
    }
    G_CMB_MWallAnimSkip = (1 - G_CMB_MWallAnimSkip);
    CMB_CurseAnimStage += 1;
    if(CMB_CurseAnimStage > 30000)
    {
        CMB_CurseAnimStage = 0;
    }

    if(battlefield->Central_Structure == CS_ChaosNode)
    {
        Combat_Grid_Screen_Coordinates(6, 11, 0, 0, &screen_x, &screen_y);
        screen_x -= 46;
        screen_y -= 13;
        FLIC_Draw(screen_x, screen_y, IMG_CMB_Volcano[8]);
    }

    if(CMB_MoveAnimDir == 0)
    {
        CMB_MoveAnimFrame += 1;
        if(CMB_MoveAnimFrame == 3)
        {
            CMB_MoveAnimDir = 1;
            CMB_MoveAnimFrame = 1;
        }
    }
    else
    {
        CMB_MoveAnimFrame -= 1;
        if(CMB_MoveAnimFrame == -1)
        {
            CMB_MoveAnimDir = 0;
            CMB_MoveAnimFrame = 1;
        }
    }

    CMB_BaseAnimFrame = ((CMB_BaseAnimFrame + 1) % 3);

    if(CMB_UnitATKAnimFrame == 1)
    {
        CMB_UnitATKAnimFrame = 3;
    }
    else
    {
        CMB_UnitATKAnimFrame = 1;
    }

    if(CMB_UnitDEFAnimFrame == 1)
    {
        CMB_UnitDEFAnimFrame = 3;
    }
    else
    {
        CMB_UnitDEFAnimFrame = 1;
    }

    FLIC_Draw(0, 164, combat_background_bottom);

    Copy_Off_To_Back();

    dbg_prn("DEBUG: [%s, %d]: END: CMB_ComposeBackgrnd__WIP()\n", __FILE__, __LINE__);

}


// WZD ovr163p03
// drake178: CMB_LoadWallRiseGFX()

// WZD ovr163p04
// drake178: CMB_BaseAllocs()
/*
clears the sandbox and the LBX near buffer, and
allocates into the former the battlefield structure
and the sprite, projectile, and vortex arrays (351h
paragraphs in total); and the latter the pathfinding
arrays which are entirely useless here as they will
be redone by another function in the current code
*/
/*

*/
void CMB_BaseAllocs__WIP(void)
{

    // MoO2  _combat_data = Allocate_First_Block()
    battlefield = (struct s_BATTLEFIELD *)Allocate_First_Block(_screen_seg, 348);  // 348 PR, 5568 B

    combat_grid_entities = (struct s_COMBAT_ENTITY *)Allocate_Next_Block(_screen_seg, 482);  // 482 PR, 7712 B

    CMB_Projectiles = Allocate_Next_Block(_screen_seg, 10);  // 10 PR, 160 B

    CMB_Vortex_Array = (struct s_MAGIC_VORTEX *)Allocate_Next_Block(_screen_seg, 9);  // 9 PR, 144 B

    // ¿ drake178:  ; WARNING: these are entirely redundant and will be  reallocated immediately after this! ?
    CMB_ActiveMoveMap = (int8_t *)Near_Allocate_First(504);
    CMB_Path_Costs = Near_Allocate_Next(504);
    CMB_NearBuffer_3 = (int16_t *)Near_Allocate_Next(1008);
    CMB_Path_Xs = Near_Allocate_Next(504);
    CMB_Path_Ys = Near_Allocate_Next(504);

}


// WZD ovr163p05
// drake178: CMB_SetProjectiles()

// WZD ovr163p06
// drake178: EMM_LoadFigureGFX()
/*
; loads the graphics for the figures of a given unit
; type into the specified allocation index of the EMM
; FIGUREX handle and test draws it into the small image
; GUI work area, returning the EMM figure index
*/
/*

~ UnitView.C  Load_Unit_Figure()
~ USELESS_Combat_Figure_Load_Compose()

bufpi gets passed in as the current battle_unit_idx
nothing modifies it, so it gets returned as-is
then, it gets assigned to battle_units[_combat_total_unit_count].bufpi
so, the battle_unit_idx is equivalent to the unit figure index

the is just itr_troop_count {0,...,8}
used to set the offset into EMM
per battle_units[], it's just value used to track the cached figure picture

How does it track them separately for each stack?
loads attackers, then loads for any units that are the defenders

Cast_Raise_Dead()
    battle_units[battle_unit_idx].bufpi = Combat_Figure_Load(_UNITS[battle_units[battle_unit_idx].unit_idx].type, Battle_Unit_Pict_Open());
*/
int16_t Combat_Figure_Load(int16_t unit_type, int16_t bufpi)
{
    char file_name[LEN_FILE_NAME] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char buffer[6] = { 0, 0, 0, 0, 0, 0 };;
    SAMB_ptr * figure_pict_set_seg = 0;
    int16_t offset = 0;
    int16_t entry_num = 0;
    // SAMB_ptr temp_figure_pict_set_seg = 0;
    int16_t itr = 0;  // _DI_
    int64_t DBG_memory_address_1 = 0;
    int64_t DBG_memory_address_2 = 0;

    FIGUREX_MAP
    FIGUREX_OFFSET
    FIGUREX_POINTER

    itoa(((unit_type / 15) + 1), buffer, 10);

    strcpy(file_name, figure_lbx_file__ovr163);

    if(((unit_type / 15) + 1) < 10)
    {

        strcat(file_name, str_figure_plural_s__ovr163);

    }

    strcat(file_name, buffer);

    strcat(file_name, str_empty_string__ovr163);

    entry_num = ((unit_type % 15) * 8);

    for(itr = 0; itr < 8; itr++)  /* 8 directions/faces per unit figure picture set */
    {

        figure_pict_set_seg[itr] = LBX_Reload_Next(file_name, (entry_num + itr), (EMS_PFBA + offset));

    }

    // WTF?  USELESS_Combat_Figure_Load_Compose(bufpi, 0, 0, 0, 0);

    return bufpi;

}



void Combat_Screen_Draw_Debug_Information(void)
{
    int16_t mouse_x = 0;
    int16_t mouse_y = 0;
    int16_t screen_x = 0;
    int16_t screen_y = 0;
    int16_t combat_grid_x = 0;
    int16_t combat_grid_y = 0;
    int16_t x_pos = 0;
    int16_t y_pos = 0;
    int16_t x_off = 0;
    int16_t y_off = 0;
    int16_t line_height = 0;
    int16_t x_col1 = 0;
    int16_t x_col2 = 0;
    int16_t string_width = 0;
    char temp_string[LEN_TEMP_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t combat_grid_x1 = 0;
    int16_t combat_grid_y1 = 0;
    int16_t combat_grid_x2 = 0;
    int16_t combat_grid_y2 = 0;
    int16_t combat_grid_col = 0;
    int16_t combat_grid_row = 0;
    int16_t l_screen_x = 0;
    int16_t l_screen_y = 0;
    int16_t cgc1 = 0;
    int16_t cgc2 = 0;
    int16_t battlefield_terrain = 0;

    Set_Outline_Color(0);
    Set_Font_Style_Shadow_Down(1, 0, 0, 0);  /* ¿ smallest/narrow font ? */
    Set_Alias_Color(8);
    line_height = Get_Font_Height();

    x_off = 0;
    y_off = 8;
    x_pos = 0;
    y_pos = 0;
    x_col1 = 2;
    x_col2 = 0;

    mouse_x = Pointer_X();
    mouse_y = Pointer_Y();

    screen_x = mouse_x;
    screen_y = mouse_y;

    combat_grid_x = Get_Combat_Grid_Cell_X((mouse_x + 4), (mouse_y + 4));
    combat_grid_y = Get_Combat_Grid_Cell_Y((mouse_x + 4), (mouse_y + 4));

    combat_grid_x1 = 
    (
        (
            ( ( screen_x - 158 ) / 2 )
            +
            ( screen_y + 80 )
        )
        / 16
    );
    combat_grid_y1 = Get_Combat_Grid_Cell_Y((mouse_x + 4), (mouse_y + 4));

    combat_grid_x2 = 
    (
        (
            ( ( screen_x - 158 ) / 32 )
            +
            ( ( screen_y +  80 ) / 16)
        )
    );
    combat_grid_y2 = Get_Combat_Grid_Cell_Y((mouse_x + 4), (mouse_y + 4));


    combat_grid_col = (screen_x / COMBAT_GRID_CELL_WIDTH);
    combat_grid_row = (screen_y / COMBAT_GRID_CELL_HEIGHT);

    cgc2 = Get_Combat_Grid_Cell_X(mouse_x, mouse_y);
    cgc1 = Get_Combat_Grid_Cell_Y(mouse_x, mouse_y);
    l_screen_x = (((cgc2 - cgc1) * 16) + 158);  /* ¿ + mid x ? */
    l_screen_y = (((cgc2 + cgc1) *  8) -  80);  /* ¿ + mid y ? */


    strcpy(temp_string, "MD X,Y");
    string_width = Get_String_Width(temp_string);
    Print(         2, (y_off+(y_pos*line_height)), temp_string);
    Print_Integer(40, (y_off+(y_pos*line_height)), mouse_x);
    Print_Integer(56, (y_off+(y_pos*line_height)), mouse_y);
    y_pos++;

    y_pos++;

    strcpy(temp_string, "CG C,R");
    string_width = Get_String_Width(temp_string);
    Print(         2, (y_off+(y_pos*line_height)), temp_string);
    Print_Integer(40, (y_off+(y_pos*line_height)), combat_grid_col);
    Print_Integer(56, (y_off+(y_pos*line_height)), combat_grid_row);
    y_pos++;

    y_pos++;

    strcpy(temp_string, "CG X,Y");
    string_width = Get_String_Width(temp_string);
    Print(         2, (y_off+(y_pos*line_height)), temp_string);
    Print_Integer(40, (y_off+(y_pos*line_height)), combat_grid_x);
    Print_Integer(56, (y_off+(y_pos*line_height)), combat_grid_y);
    y_pos++;

    strcpy(temp_string, "CG1 X,Y");
    string_width = Get_String_Width(temp_string);
    Print(         2, (y_off+(y_pos*line_height)), temp_string);
    Print_Integer(40, (y_off+(y_pos*line_height)), combat_grid_x1);
    Print_Integer(56, (y_off+(y_pos*line_height)), combat_grid_y1);
    y_pos++;

    strcpy(temp_string, "CG2 X,Y");
    string_width = Get_String_Width(temp_string);
    Print(         2, (y_off+(y_pos*line_height)), temp_string);
    Print_Integer(40, (y_off+(y_pos*line_height)), combat_grid_x2);
    Print_Integer(56, (y_off+(y_pos*line_height)), combat_grid_y2);
    y_pos++;

    y_pos++;

    strcpy(temp_string, "CGC 2,1");
    string_width = Get_String_Width(temp_string);
    Print(         2, (y_off+(y_pos*line_height)), temp_string);
    Print_Integer((2 + string_width +  5), (y_off+(y_pos*line_height)), cgc2);
    Print_Integer((2 + string_width + 16), (y_off+(y_pos*line_height)), cgc1);
    y_pos++;

    strcpy(temp_string, "CGC SX,SY");
    string_width = Get_String_Width(temp_string);
    Print(         2, (y_off+(y_pos*line_height)), temp_string);
    Print_Integer((2 + string_width +  5), (y_off+(y_pos*line_height)), l_screen_x);
    Print_Integer((2 + string_width + 16), (y_off+(y_pos*line_height)), l_screen_y);
    y_pos++;


    battlefield_terrain = battlefield->terrain_type[((cgc1 * COMBAT_GRID_WIDTH) + cgc2)];
    strcpy(temp_string, "CMBT TERR");
    string_width = Get_String_Width(temp_string);
    Print(         2, (y_off + (y_pos * line_height)), temp_string);
    Print_Integer((2 + string_width +  5), (y_off + (y_pos * line_height)), battlefield_terrain);
    y_pos++;



    // Set_Font_Style_Shadow_Down(0, 0, 0, 0);
    // Set_Outline_Color(0);
    // Set_Alias_Color(0);

}
