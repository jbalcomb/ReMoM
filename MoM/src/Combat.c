/*
    Combat

    WIZARDS.EXE
        ovr090
        ovr091  ¿ MoO2  COMBINIT ?
        ovr096
        ovr098  ¿ MoO2  COMBINIT || COMBAT1 ?
        ovr099  ¿ MoO2  Module: CMBTDRW1 ?  (would have been CMBTDRW or CMBTDRAW?)
        ovr103
        ovr105
        ovr110  ¿ Strategic Combat ?
        ovr111  ¿ AITP.* ?
        ovr112  ¿ LBX CMBMAGIC ?
        ovr113  
        ovr114  ¿ MoO2  Module: CMBTAI ?
        ovr116  MoO2  COMBINIT
        ovr122
        ovr123
        ovr124
        ovr139
        ovr153  ¿ Combat Map ?                                          Or, Map1?
        ovr154  ¿ Combat Terrain ?                                      Or, Map2?
        ovr155  ¿ MoO2  Module: CMBTMOV1 ?  (would have been CMBTMOVE?) Or, Map3?
        ovr163  ¿ MoO2  Module: COMBINIT ?
*/

#include "../../STU/src/STU_DBG.h"
#include "../../STU/src/STU_LOG.h"

#include "../../MoX/src/angle.h"
#include "../../MoX/src/EXIT.h"
#include "../../MoX/src/paragrph.h"
#include "../../MoX/src/Timer.h"
#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/FLIC_Draw.h"
#include "../../MoX/src/Fields.h"
#include "../../MoX/src/Fonts.h"
#include "../../MoX/src/GENDRAW.h"
#include "../../MoX/src/Graphics.h"
#include "../../MoX/src/Help.h"
#include "../../MoX/src/LBX_Load.h"
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_BASE.h"
#include "../../MoX/src/MOX_DAT.h"  /* _players, _screen_seg */
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_SET.h"  /* magic_set */
#include "../../MoX/src/MOX_T4.h"
#include "../../MoX/src/random.h"
#include "../../MoX/src/SOUND.h"
#include "../../MoX/src/special.h"
#include "../../MoX/src/Util.h"

#include "DIPLODEF.h"
#include "City_ovr55.h"
#include "CITYCALC.h"
#include "DIPLOMAC.h"
#include "Items.h"
#include "Lair.h"
#include "LOADER.h"
#include "MainScr.h"
#include "MainScr_Maps.h"
#include "MOM_DBG.h"
#include "NEXTTURN.h"
#include "RACETYPE.h"
#include "SBookScr.h"
#include "Spellbook.h"  /* spl_Doom_Bolt;  Combat_Spellbook_Build() */
#include "SPELLDEF.h"
#include "Spells131.h"
#include "Spells133.h"
#include "SPLMASTR.h"
#include "Terrain.h"
#include "TerrType.h"
#include "UnitMove.h"   // WTFMATE
#include "UnitStat.h"
#include "UNITTYPE.h"   /* rag_Missile */
#include "UnitView.h"
#include "WZD_o059.h"

#include "../../ext/stu_compat.h"

#include <assert.h>
#include <stddef.h> /* offsetof() */
#include <stdlib.h>
#include <string.h> /* memcpy() memset() */

#include "CMBMAGIC.h"
#include "CMBTAI.h"
#include "CMBTDEF.h"
#include "CMBTMVPT.h"
#include "Combat.h"
#include "COMBINIT.h"



#ifdef STU_DEBUG
int16_t DBG_player_idx = HUMAN_PLAYER_IDX;
int16_t DBG_cgx = 0;  // 14
int16_t DBG_cgy = 0;  // 12
int16_t DBG_target_cgx = 0;  // 13 W
int16_t DBG_target_cgy = 0;  // 11 N
int16_t DBG_PFA_1311_set = ST_FALSE;
int16_t DBG_PFA_1411_set = ST_FALSE;
int16_t DBG_PFA_1511_set = ST_FALSE;
extern uint8_t DBG_debug_flag;
void Combat_Screen_Draw_Debug_Information(void);
void DBG_Compare_Battle_Units(const char * label);
#endif



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
char soundfx_lbx_file__ovr090[] = "SOUNDFX";
// WZD dseg:50EF
char str_empty_string__ovr090[] = "";
// WZD dseg:50F0
char music_lbx_file__ovr090[] = "MUSIC";
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
char str_WhileFleeingYouLost[] = "While fleeing you lost:";
// WZD dseg:56E4
char str_SPACE__ovr098[] = " ";
// WZD dseg:56E6
char str_PERIOD__ovr098[] = ".";

// WZD dseg:56E6                                                 END:  ovr098 - Initialized Data



// WZD dseg:56E8                                                 BEGIN:  ovr099 - Initialized Data

// WZD dseg:56E8
uint8_t combat_name_font_colors_by_banner[6][4] = {
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
// WZD dseg:5B48
char cnst_SpellError_1[] = "That unit already has ";
// WZD dseg:5B5F
char cnst_SpellError_2[] = " cast on it";
// WZD dseg:5B6B
char cnst_SpellError_3[] = "That unit is immune to Death spells";
// WZD dseg:5B8F
char message_lbx_file__ovr113__2of2[] = "message.lbx";
// WZD dseg:5B9B
char soundfx_lbx_file__ovr113[] = "soundfx";
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
It selects which stone-wall sprite bank the battlefield draws — a visual variant, nothing mechanical.
It's used as the first index of IMG_CMB_StoneWalls[3][12] (Combat.c:1166) at ten sites in the wall-entity spawner (20530-20626), and as a bank offset into WALLRISE.LBX at 24134 — (CMB_StoneWallType * 12) + itr, twelve animation frames per style.
So three banks of twelve exist in the array, though only two are ever selected.
The three writes are all in Build_Battlefield:
    Where                   Value
    Mountains terrain       0
    Desert terrain          0
    wp == 1 — Myrror        1
The plane test runs last and is unconditional on terrain, so Myrror always wins.
It's a stale global on Arcanus, and that's OG. Nothing writes it for an Arcanus battle on grassland, forest, hills, swamp, river, tundra or water — only mountains and desert set it. It's dw 0 in dseg (_misc.asm:2905) and initialised once, so after any Myrror battle it stays 1 and every subsequent Arcanus grassland siege draws Myrran walls until a mountain or desert battle resets it.
*/
int16_t _combat_wall_sprite_bank = 0;
/*
global                          period              drives
_combat_curse_anim_frame        wraps at MAX_SINT   curse effect
*/
// WZD dseg:7070
int16_t _combat_magic_wall_anim_skip = 0;
// WZD dseg:7070                                                                                         ; 0 or 1, controls whether something else is stepped
// WZD dseg:7072 01                                              db    1
// WZD dseg:7073 00                                              db    0
// WZD dseg:7074
int16_t _combat_magic_wall_anim_frame = 0;
// WZD dseg:7074                                                                                         ; steps 0 to 4 if the above is 0
// WZD dseg:7076
int16_t _combat_road_anim_frame = 0;
// WZD dseg:7076                                                                                         ; steps 0 to 4
// WZD dseg:7078
int16_t _combat_water_anim_frame = 0;
/*
advanced in Combat_Screen_Draw(), rather than in Combat_Compose_Background()
*/
// WZD dseg:707A
int16_t _combat_mud_anim_phase = 0;
// WZD dseg:707C 00                                                  db    0
// WZD dseg:707D 00                                                  db    0
// WZD dseg:707E
int16_t _combat_chaos_node_anim_frame = 0;
// WZD dseg:7080
int16_t frame_anim_cycle = 0;
// WZD dseg:7082
int16_t uu_combat_movement_variable;
// WZD dseg:7084
int16_t niu_projectile_anim_frame_2;
// WZD dseg:7086
int16_t projectile_anim_frame = 0;
// WZD dseg:7088
int16_t _combat_vortex_anim_frame = 0;
// WZD dseg:708A
int16_t cmbt_cell_effect_frame = 0;
// WZD 36AA:708C 00                                                  db    0
// WZD 36AA:708D 00                                                  db    0
// WZD dseg:708E
int16_t cmbt_cell_effect_active = 0;
// WZD dseg:7090
int16_t _combat_curse_anim_frame = 0;
// WZD dseg:7092
int16_t combat_walk_anim_dir = 0;
// WZD dseg:7094
int16_t combat_walk_anim_frame = 0;
// WZD dseg:7096
int16_t combat_idle_anim_frame = 0;
// WZD dseg:7098
int16_t _combat_unit_attack_anim_frame = 1;
// WZD dseg:709A
int16_t _combat_unit_defend_anim_frame = 3;

// WZD dseg:709C
/*
SimTex Debug Code
If it is not false, Build_Battlefield() does not call Generate_Combat_Map().
*/
int16_t _combat_skip_map_generation = ST_FALSE;

// WZD dseg:709E
char str_E1[] = "E1";

// WZD dseg:709E                                                 END:  ovr153 - Initialized Data



// WZD dseg:70A0                                                 BEGIN:  ovr154 - Initialized Data

// WZD dseg:70A0
// WZD dseg:70AA
/*
0 is NIU, because Random() is 1-based
{ 1:south, 2:west, 3:north, 4:east }
*/
int16_t step_delta_cgx[5] = { 0,  0, -1,  0,  1};
int16_t step_delta_cgy[5] = { 0,  1,  0, -1,  0};

// WZD dseg:70B4 00                                              db    0
// WZD dseg:70B5 00                                              db    0

// WZD dseg:70B6
char str_COMBAT_TMP__ovr154[] = "COMBAT.TMP";

// WZD dseg:70C1
char str_RB__ovr154[] = "rb";
// WZD dseg:70C4
char str_WB__ovr154[] = "wb";

// WZD dseg:70C7 00                                              align 2

// WZD dseg:70C7                                                 END:  ovr154 - Initialized Data





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
// WZD dseg:71D0
char wallrise_lbx_file__ovr163[] = "WALLRISE";
// WZD dseg:71D9
char figure_lbx_file__ovr163[] = "FIGURE";

// WZD dseg:71D9                                                 END:  ovr163 - Initialized Data



// WZD dseg:C406                                                 ¿ BEGIN:  ?

// WZD dseg:C406
/*
DEDU is this just an 'end of turn wait'?

¿ ~ NOT {F,T} no units left of status active, player controller, movement points ?

*/
int16_t _human_handle_immobile;

// WZD dseg:C408
int16_t _computer_player_city_seige;

// WZD dseg:C40A
char _combat_city_name[LEN_CITY_NAME];

// WZD dseg:C418
/*
¿ just for destroying buildings ?

'Magic Vortex'
    ...does += 5
"Finally, for combat turns during which a vortex passes over city squares, every building has a 5% chance of being destroyed."

Page 99  (PDF Page 104)
AFTER IT’S OVER
City Damage
When cities are invaded, there is a small chance that townsfolk and buildings will be destroyed, even if the attack is repelled.
The total amount of death and destruction depends on how long the city is occupied (i.e., how long the battle takes).
Far greater losses to citizens and buildings are suffered, however, when a city is conquered by an enemy.
City buildings and citizens are especially likely to be lost
  if the attacker actually conducts combat within the city
  (as represented by a cluster of buildings on map squares during combat)
  itself—rather than engaging the enemy outside city walls.
*/
int16_t _combat_city_damage;

// WZD dseg:C41A
SAMB_ptr sound_silent_seg;
uint32_t sound_silent_seg_size;  // DNE in Dasm

// WZD dseg:C41C
SAMB_ptr SND_CMB_Music;
uint32_t SND_CMB_Music_size;  // DNE in Dasm

// WZD dseg:C41E
/*
_defender_sees_illusions and _attacker_sees_illusions are side-level flags, not unit-level flags
*/
int16_t _defender_sees_illusions;
int16_t _attacker_sees_illusions;

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
/*
m_cp_took_turn is a turn-order flag that controls whether the AI's defending turn is skipped at the top of Combat_Next_Turn().

Here's how it works:

Default: ST_FALSE — initialized to false in Combat_Screen() and reset in Begin_Combat_Turn().

At the start of Combat_Next_Turn() (Combat.c:4803): if m_cp_took_turn == ST_FALSE, the computer player gets its AI turn via Auto_Cast_Spell_And_Do_Combat_Turn(_combat_remote_player). If it's ST_TRUE, this call is skipped — because the AI already went.

Later in the same function (Combat.c:4813-4816): if the defender is the computer, the defending AI plays its turn, and then m_cp_took_turn is set to ST_TRUE.

Effect on the next call: When Combat_Next_Turn() is called again next turn, the ST_TRUE value causes the AI's turn at the top to be skipped — preventing the defending AI from getting a double turn (once as defender at the end of the previous cycle, and again as the computer player at the start of the next).

In short: it's a flag that prevents the AI defender from acting twice in a row across the turn boundary. As your existing comment notes, it's only checked at the top of this function, so setting it to ST_TRUE at line 4816 is specifically meant to carry over to the next invocation. It gets cleared back to ST_FALSE when auto-combat is cancelled or when a new combat turn begins via Begin_Combat_Turn().
*/
int16_t m_cp_took_turn;

// WZD dseg:C432
int16_t _auto_combat_flag;

/* CLAUDE: test-support -- when ST_TRUE, Combat_Screen() re-enables _auto_combat_flag right after its entry reset, so the tactical battle runs AI-vs-AI with no human input (HeMoM --combat-tactical). Normal gameplay leaves this ST_FALSE. */
int16_t g_cmbt_force_auto_combat = ST_FALSE;

/*
    CP, NPC, or MONSTER
    (either attacker or defender)

vs. _combat_ai_player
It is not a synonym for _combat_remote_player.
They agree whenever one side is human, which is the normal case.
They diverge in AI-vs-AI:
    the defender-priority ordering makes this one the defender,
      while _combat_remote_player comes out as the attacker
      (asm:301-313 assigns it as "whichever isn't _combat_local_player", and that pair names the defender as the human).

Attacks can only be made by the current player, so the _combat_attacker_player is always _combat_local_player.
*/
// WZD dseg:C434
int16_t _combat_remote_player;

/*
    human/current player
    (either attacker or defender)
*/
// WZD dseg:9294
// drake178: RP_AI_UnsetRealmVar1
/* DNE  no writer anywhere in the Dasm - always 0;  read by AITP_DispelMagic() (o111p07) and AI_Score_Combat_Spell() (ovr139p02, attacker branch) */
int16_t g_ai_combat_unset_realm_flags;

// WZD dseg:9296
// drake178: RP_AI_UnsetRealmVar2
/* DNE  no writer anywhere in the Dasm - always 0;  read by AI_Score_Combat_Spell() (ovr139p02, defender branch) */
int16_t g_ai_combat_unset_realm_flags_2;

// WZD dseg:9298
// drake178: AI_DEFR_UnitRealms
/* per-realm unit counts/values for the combat defender;  written by the (not yet reconstructed) o139 combat-realm setup */
int16_t g_ai_combat_defender_unit_realms[5];

// WZD dseg:92A2
// drake178: AI_ATKR_UnitRealms
/* per-realm unit counts/values for the combat attacker;  written by the (not yet reconstructed) o139 combat-realm setup */
int16_t g_ai_combat_attacker_unit_realms[5];

// WZD dseg:92AC
// drake178: AI_DEFR_RealmFlags
/* realm bit flags for the combat defender's spellbooks;  written by the (not yet reconstructed) o139 combat-realm setup */
int16_t g_ai_combat_defender_realm_flags;

// WZD dseg:92AE
// drake178: AI_ATKR_RealmFlags
/* realm bit flags for the combat attacker's spellbooks;  written by the (not yet reconstructed) o139 combat-realm setup */
int16_t g_ai_combat_attacker_realm_flags;

// WZD dseg:C436
int16_t _combat_local_player;

// WZD dseg:C438
int16_t m_wizard_cast_available;

// WZD dseg:C43A
// WZD dseg:C43C
int16_t defender_enchantment_first_shown;
int16_t attacker_enchantment_first_shown;

// WZD dseg:C43E
int16_t _combat_ai_player;

// WZD dseg:C440
// WZD dseg:C45E
int16_t defender_enchantment_fields[NUM_COMBAT_ENCHANTMENTS];
int16_t attacker_enchantment_fields[NUM_COMBAT_ENCHANTMENTS];


// WZD dseg:C47C
// WZD dseg:C47E
// WZD dseg:C480
/*
    _combat_wx is reused as the anim ctr for Cast_Spell_Of_Mastery()
*/
int16_t _combat_wp;
int16_t _combat_wy;
int16_t _combat_wx;

// WZD dseg:C482
// WZD dseg:C484
int16_t attacker_enchantment_count;
int16_t defender_enchantment_count;
// WZD dseg:C486
// WZD dseg:C4C2
struct  s_COMBAT_ENCHANTMENT_ICON _combat_enchantments_defender[NUM_COMBAT_ENCHANTMENTS];
struct  s_COMBAT_ENCHANTMENT_ICON _combat_enchantments_attacker[NUM_COMBAT_ENCHANTMENTS];
// WZD dseg:C4FE
SAMB_ptr combat_enchantment_icon_segs[NUM_COMBAT_ENCHANTMENTS];

// WZD dseg:C51C
int16_t _combat_turn;

// WZD dseg:C51E
/*
{F,T} no units left of status active, player controller, movement points
*/
int16_t _human_out_of_moves;

// WZD dseg:C520
/*
; a seemingly pointless variable whose state could have
; been extrapolated from others
;   1 - city battle (defender only for wall spells?)
;   2 - sorcery node
;   3 - nature node
;   4 - chaos node
;   6 - sailable square
*/
/*
{ 1: city, 2: node, sorcery, 3: node, nature, 4: node, chaos, 6: ocean }
*/
int16_t _combat_structure;

// WZD dseg:C522
// MoO2  Module: MOX  _combat_mouse_grid
struct s_mouse_list * _combat_mouse_grid;
// struct s_mouse_list _combat_mouse_grid[1] = {
//     {crsr_Finger, 0, SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX}
// };

// WZD dseg:C524
// MoO2  Module: MOX  _combat_grid  ..._combat_mouse_grid
/*
; array of 22 pointers to LBX_NearAlloc_Next allocations of 21 bytes each
indexed by sizeof(2)
uses Near_Allocate_Next(), so points to 1-byte values
mov     bx, [bp+YPos]
shl     bx, 1
mov     bx, [g_combat_grid_action_map@+bx]
add     bx, _SI_XPos
mov     al, [bx]
cbw
uses AL and CBW, so 1-byte, signed values
allocated in Allocate_Combat_Near_Buffers()
*/
/*
e.g., g_combat_grid_action_map[cgy][cgx]
1-byte, signed
'Right-Click Combat Grid' uses it to get battle_unit_idx
populated in Assign_Combat_Grids()
{-2, -1, battle_unit_idx, 99}
*/
int8_t * g_combat_grid_action_map[COMBAT_GRID_HEIGHT];

// WZD dseg:C550
/*
array of pointers to composed bitmaps of combat battle unit figure pictures

Combat_Screen_Map_Draw_Entities() |-> Draw_Picture_Windowed()

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
// MoO2  Module: MOX  _cur_ship  (not _cur_ptr?)
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
// WZD dseg:C586
/*
¿ part of the reason for these to exist is to have values outside the range of {127,-128} ?
*/
int16_t _combat_defender_player;
int16_t _combat_attacker_player;

// WZD dseg:C588
int16_t _combat_total_unit_count;

// WZD dseg:C58A
/*
    scanned battle unit, as shown in the combat unit display
    set in Assign_Mouse_Image(), from g_combat_grid_action_map[grid_y][grid_x]

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
/*
m_turn_is_local is a flag indicating whether it's currently the human player's turn in the combat flow.

Here's how it's used:

Set to ST_FALSE at the start of Combat_Next_Turn() (Combat.c:4810) — while the AI is taking its turns, it's not the human's turn.

Set to ST_TRUE after all AI turns complete and before the human gets control (Combat.c:4826). Also initialized to ST_TRUE in Combat_Screen() (Combat.c:1684) when the combat screen first opens.

Checked in the UI drawing code (Combat.c:7070): when the active battle unit belongs to the human player and all units are immobilized, m_turn_is_local gates whether the message "All units are immobilized. Select an action." is displayed. This prevents the message from appearing during the AI's processing phase — it only shows when the human is actually in control and can respond to it.

In short: it's a UI guard that tracks whether the human player currently has control of the combat turn, used to conditionally show player-facing prompts and messages.
*/
int16_t m_turn_is_local;

// WZD dseg:C79C
int16_t niu_combat_screen_dirty;

// WZD dseg:C79C                                                 ¿ ovr098 ?



// WZD dseg:C7AA                                                 ¿ BEGIN:  ovr103 ?

// WZD dseg:C7AA
SAMB_ptr combat_movemode_icon_segs[3];

// WZD dseg:C7B0
SAMB_ptr unit_hit_bar_seg;

// WZD dseg:C7B2
SAMB_ptr battle_unit_scratch_seg;

// WZD dseg:C7B4
int16_t _combat_info_heavenly_light;
// WZD dseg:C7B5
int16_t _combat_info_cloud_of_shadow;
// WZD dseg:C7B6
int16_t g_center_square_structure;

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
int16_t _combat_per_side_effect_rows;

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



// WZD dseg:C972                                                 ¿ BEGIN: ovr123 - Uninitialized Data ?

// WZD dseg:C972
int16_t _combat_winner;
/* CLAUDE */ int16_t DBG_atk_active, DBG_atk_dead, DBG_atk_gone, DBG_def_active, DBG_def_dead, DBG_def_gone;
// WZD dseg:C974
int16_t destroy_combat_city;
// WZD dseg:C976
int16_t CMB_Gold_Reward;
// WZD dseg:C978
int16_t CMB_Buildings_Lost;
// WZD dseg:C97A
int16_t * CMB_LostBuildings;
// WZD dseg:C97C
int16_t CMB_Population_Lost;
// WZD dseg:C97E
/*
enum e_COMBAT_SCROLL_MESSAGE_TYPE
*/
int16_t combat_results_scroll_message;

// WZD dseg:C97E                                                 ¿ END: ovr123 - Uninitialized Data ?    



// WZD dseg:CF9A                                                 ovr153

// WZD dseg:CF9A
SAMB_ptr EmmHndl_TILEXXX;

// WZD dseg:CF9C
SAMB_ptr EmmHndl_FIGUREX;

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

// WZD dseg:CFA2
SAMB_ptr _wallrise_seg[14];

// WZD dseg:CFBE                                                 ovr052, ovr153

// WZD dseg:CFBE
SAMB_ptr cmbtcity_blood_segs[5];
// WZD dseg:CFBE                                                                                         ; array of 5 appended reserved EMM headers in
// WZD dseg:CFBE                                                                                         ; GFX_Swap_Seg, with a 3 frame animation each
// WZD dseg:CFC8 00                                              db    0
// WZD dseg:CFC9 00                                              db    0
// WZD dseg:CFCA
SAMB_ptr combat_curse_entity_seg[8];
// WZD dseg:CFCA                                                                                         ; array of 8 appended reserved EMM headers in
// WZD dseg:CFCA                                                                                         ; GFX_Swap_Seg, with the first 7 containing 8 frame
// WZD dseg:CFCA                                                                                         ; animations, while the last one is single image (web)

// WZD dseg:CFDA
SAMB_ptr IMG_CMB_Rocks[5];

// WZD dseg:CFE4
SAMB_ptr IMG_CMB_Trees[5];

// WZD dseg:CFEE
SAMB_ptr cmbtcity_river_segs[12];
// WZD dseg:CFEE 00 00 00 00 00 00 00 00                                                                 ; array of 12 appended reserved EMM headers in
// WZD dseg:CFEE                                                                                         ; GFX_Swap_Seg; the first 6 are empty, the other 6
// WZD dseg:CFEE                                                                                         ; have 5 frame animations for rivers
// WZD dseg:D006
SAMB_ptr chriver_river_segs[12];
// WZD dseg:D006 00 00 00 00 00 00 00 00                                                                 ; array of 12 appended reserved EMM headers in
// WZD dseg:D006                                                                                         ; GFX_Swap_Seg with no actual data (entry size 0)
// WZD dseg:D01E
SAMB_ptr cmbtcity_roadgrid_segs[28];
// WZD dseg:D01E 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; array of 28 appended reserved EMM headers in
// WZD dseg:D01E 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; GFX_Swap_Seg with 5 frame animations each, although
// WZD dseg:D01E 00 00 00 00 00 00 00 00                                                                 ; the normal ones (first in array) are all static
// WZD dseg:D056
SAMB_ptr cmbtcity_ocean_segs[4];
// WZD dseg:D056                                                                                         ; array of 4 appended reserved EMM headers in
// WZD dseg:D056                                                                                         ; GFX_Swap_Seg, each with a 5 frame animation
// WZD dseg:D05E
SAMB_ptr chriver_chaoswat_segs[4];
// WZD dseg:D05E                                                                                         ; array of 4 appended reserved EMM headers in
// WZD dseg:D05E                                                                                         ; GFX_Swap_Seg, each with a 5 frame animation
// WZD dseg:D066
SAMB_ptr cmbtcity_cmbcloud_segs[4];
// WZD dseg:D066                                                                                         ; appended reserved EMM header in GFX_Swap_Seg

// WZD dseg:D06E
SAMB_ptr cmbtcity_flotisle_seg;
// WZD dseg:D06E                                                                                         ; this image is actually empty
// WZD dseg:D070
SAMB_ptr cmbtcity_mud_seg;
// WZD dseg:D070                                                                                         ; 5 frame (earth to) mud animation

// WZD dseg:D072
SAMB_ptr IMG_CMB_SorcNode;
// WZD dseg:D074
SAMB_ptr IMG_CMB_NatNode;

// WZD dseg:D076
SAMB_ptr chriver_volca_segs[9];


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
SAMB_ptr _combat_house_picts_segs[15];

// WZD dseg:D13A
SAMB_ptr cmbt_cell_effect_seg;
// WZD dseg:D13A                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:D13A                                                                                         ; 8 frame animation
// WZD dseg:D13C                                                 ovr099, ovr153
// WZD dseg:D13C
SAMB_ptr magic_vortex_seg;
// WZD dseg:D13C                                                                                         ; appended reserved EMM header in GFX_Swap_Seg
// WZD dseg:D13C                                                                                         ; 6 frame animation

// WZD dseg:D13E
int16_t * _cmbt_mvpth_y;
// WZD dseg:D140
int16_t * _cmbt_mvpth_x;
// WZD dseg:D142
int16_t movement_path_grid_cell_count;

// WZD dseg:D144

// WZD dseg:D144
/*
2-byte, signed
allocated in Allocate_Combat_Near_Buffers()
_cmbt_path_data = Near_Allocate_Next(1008);
504 2-byte values

21 * 22 = 462
...is a 462-element integer array (exactly the size of the 21x22 combat grid) that serves two completely different purposes depending on which of those two pathfinding functions is currently executing.
In Combat_Move_Path_Find, it stores the "came from" index — for each cell, it records which adjacent cell provided the cheapest path to reach it. That's how the path is reconstructed after the cost map converges: you start at the destination and follow _cmbt_path_data backwards to the source.
In Combat_Move_Path_Valid, it's being repurposed as a boolean reachability map — marking each square as reachable (ST_TRUE) or not (ST_FALSE) based on whether the unit has enough movement points to enter it. This is used to draw the blue movement highlight overlay on the combat screen.

predecessor / parent array
When the engine executes the trace loop:
    It starts at the destination (itr = 84).
    It looks at the parent map (_cmbt_path_data[84]) which says, "You got here from Square 83."
    It repeats this until it hits the source square.
*/
int16_t * _cmbt_path_data;
/* GEMINI */
// /* The Dual-Purpose Routing Buffer */
// union combat_path_state {
//     unsigned short came_from_idx[COMBAT_GRID_CELLS];    /* Used by Path_Find */
//     unsigned short is_reachable[COMBAT_GRID_CELLS];     /* Used by Path_Valid */
// };
// union combat_path_state combat_path_state_share;
// /* ... later in the code ... */
// shared_buffer.reachable_grid[cx] = ST_TRUE;
// union Combat_Routing_Data {
//     unsigned short came_from_grid[462]; /* The pred(v) tree for Path_Find */
//     unsigned short reachable_mask[462]; /* The boolean overlay for Path_Valid */
// };
// union Combat_Routing_Data m_combat_routing;
// /* 1. Allocate the actual block of memory once (perhaps during game boot) */
// unsigned short m_combat_work_ram[COMBAT_GRID_CELLS];
// /* 2. Declare your semantic pointers and aim them at the work RAM */
// unsigned short *m_parent_tile_grid = m_combat_work_ram;
// unsigned short *m_reachable_mask   = m_combat_work_ram;
// union Combat_Routing_Data {
//     int16_t * came_from_idx;  /* Used by Path_Find  */  /* The pred(v) tree for Path_Find */
//     int16_t * is_reachable;   /* Used by Path_Valid */  /* The boolean overlay for Path_Valid */
// };
union Combat_Routing_Data _cmbt_path_route;

// WZD dseg:D146
/*
1-byte, unsigned
move cost/edge weight
*/
uint8_t * _cmbt_mvpth_c;

// WZD dseg:D148
/*
1-byte, unsigned
used as movement cost map
-1: ¿ occupied or impassible ?
¿ otherwise, the movement cost of the combat grid cell ?
...which gets memcpy()'d over, in in Set_Movement_Cost_Map()
movement cost is set in 
...a Dynamic Buffer Hijacking...
Combat_Move_Path_Find() takes the address _cmbt_movepath_cost_mapfor its own pointer m_movement_path_grid_cell_index
which then overwites the int8_t cost values with int16_t index values
*/
/*
mov     bx, [_cmbt_mvpth_c]
add     bx, _CX_ctr
mov     al, [bx]
mov     ah, 0
mov     dl, [bp+move_cost]
mov     dh, 0
sub     ax, dx
mov     [bp+Tile_GetTo_Cost], ax
...elsewhere...
mov     bx, [_cmbt_movepath_cost_map]
add     bx, _CX_ctr
mov     al, [bx]
mov     [bp+move_cost], al
...no `mov     ah, 0` or `cbw`, so move_cost and _cmbt_movepath_cost_map have the same type
*/
uint8_t * _cmbt_movepath_cost_map;

// WZD dseg:D14A
// WZD dseg:D14C
/*
cgx,cgy

set in Battle_Unit_Summon_Animation()
    ...where used?
*/
int16_t cmbt_cell_effect_cgy;
int16_t cmbt_cell_effect_cgx;

/*
Magic Vortex

Do_Legal_Spell_Check() has max vortex count as 10
*/
// WZD dseg:D14E
struct s_MAGIC_VORTEX * _vortexes;
// WZD dseg:D152
int16_t _vortex_count;

// WZD dseg:D154
/*
; (up to 11 records of 14 bytes each)
*/
struct s_MISSILE * _missiles;
// WZD dseg:D158
int16_t m_missile_count;

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
int16_t m_gibs_frames[20];

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

// WZD dseg:D27E
int16_t _wall_rise_type;
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



// WZD dseg:D380                                                 BEGIN:  ovr154 - Uninitialized Data

// WZD dseg:D380
int16_t defender_on_floating_island;

// WZD dseg:D382
int16_t attacker_on_floating_island;

// WZD dseg:D382                                                 END:  ovr154 - Uninitialized Data



/*
    WIZARDS.EXE  ovr090
*/

// WZD s90p01
// MoO2  Module: COMBINIT  Combat_Screen_()
// MoO2  Module: COMBINIT  Tactical_Combat_()
// 1oom
/*
    returns ST_TRUE, if Combat_Winner is _combat_attacker_player
    otherwise, returns ST_FALSE
*/
int16_t Combat_Screen(int16_t combat_attacker_player_idx, int16_t combat_defender_player_idx, int16_t troops[], int16_t troop_count, int16_t wx, int16_t wy, int16_t wp, int16_t * item_count, int16_t item_list[])
{
    int16_t did_win = 0;
    int16_t end_of_combat_message_type = 0;  /* enum e_COMBAT_SCROLL_MESSAGE */
    int16_t defender_unit_count = 0;
    int16_t human_player_did_flee = 0;
    int16_t winner_player_idx = 0;
    int16_t auto_combat_cancel_ESC_field = 0;
    int16_t cast_status = 0;
    int16_t temp_movement_points = 0;
    uint32_t temp_unit_enchantments = 0;
    int16_t active_unit_window_field = 0;
    int16_t space_hotkey_field = 0;
    int16_t info_button_field = 0;
    int16_t flee_button_field = 0;
    int16_t auto_button_field = 0;
    int16_t done_button_field = 0;
    int16_t wait_button_field = 0;
    int16_t spell_button_field = 0;
    int16_t right_click_cgy = 0;
    int16_t right_click_cgx = 0;
    int16_t combat_grid_field = 0;
    int64_t grid_sy = 0;
    int64_t grid_sx = 0;
    int16_t escape_field = 0;
    int16_t screen_changed = 0;
    int16_t leave_screen = 0;
    int16_t input_field_idx = 0;
    int16_t itr = 0;
    int16_t battle_unit_idx = 0;
#ifdef STU_DEBUG
    int16_t hotkey_idx_Z = 0;  // debug_hotkey
    int16_t hotkey_idx_T = 0;  // test_hotkey
#endif
    _ai_immobile_counter = 0;
    _computer_player_city_seige = ST_FALSE;
    if(
        (_combat_environ == 1)  /* City-Siege */
        &&
        (combat_defender_player_idx != NEUTRAL_PLAYER_IDX)
    )
    {
        _computer_player_city_seige = ST_TRUE;
    }
    _computer_player_did_flee = ST_FALSE;
    human_player_did_flee = ST_FALSE;
    Deactivate_Auto_Function();
    Clear_Fields();
    Fade_Out();
    Set_Page_Off();
    Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, ST_TRANSPARENT);
    Toggle_Pages();
    _page_flip_effect = pfe_None;
    if(_combat_environ == 1)  /* City-Siege */
    {
        _fstrcpy(_combat_city_name, _CITIES[_combat_environ_idx].name);
    }
    Allocate_Combat_Base_Blocks();
    Combat_Screen_Load_Resources();  /* calls Allocate_Combat_Near_Buffers() */
    _combat_wx = wx;
    _combat_wy = wy;
    _combat_wp = wp;
    _combat_attacker_player = combat_attacker_player_idx;
    _combat_defender_player = combat_defender_player_idx;
    Cache_Graphics_Combat();
    Build_Battlefield(wx, wy, wp);
    defender_unit_count = Prepare_All_Battle_Units(troop_count, troops);
    Clear_Fields();
    Deactivate_Auto_Function();
    Assign_Auto_Function(Combat_Screen_Draw, 1);
    attacker_enchantment_first_shown = 0;
    defender_enchantment_first_shown = 0;
    _combat_structure = Combat_Structure(wx, wy, wp, 0);
    Update_Combat_Enchantments_Icon_And_Help();
    Combat_Node_Type();
    _human_out_of_moves = ST_FALSE;
    _combat_turn = 0;
    m_wizard_cast_available = ST_TRUE;
    _combat_total_battle_effect_count = Combat_Info_Effects_Count();
    Calc_Battlefield_Bonuses(_combat_structure);
    Combat_Cache_Write();
    // SOUNDFX.LBX, 106  "SILENT V"  "sILENCE"
    sound_silent_seg = LBX_Reload(soundfx_lbx_file__ovr090, SFX_Silence, World_Data);
    sound_silent_seg_size = lbxload_entry_length;
    // ; when populating the BU table, set to the index of the AI player (the other participant being always human)
    if(_combat_ai_player != NEUTRAL_PLAYER_IDX)
    {
        SND_CMB_Music = LBX_Reload_Next(music_lbx_file__ovr090, ((MUSIC_Merlin_Cmbt1 - 1) + ((_players[_combat_ai_player].wizard_id * 2) + Random(2))), World_Data);
        SND_CMB_Music_size = lbxload_entry_length;
    }
    else
    {
        itr = (MUSIC_Combat_1 - 1) + Random(2);
        SND_CMB_Music = LBX_Reload_Next(music_lbx_file__ovr090, itr, World_Data);
        SND_CMB_Music_size = lbxload_entry_length;
    }
    if(magic_set.background_music == ST_TRUE)
    {
        Play_Sound(SND_CMB_Music, SND_CMB_Music_size);
    }
    _human_handle_immobile = ST_FALSE;
    _ai_stay_in_city = ST_TRUE;
    _scanned_battle_unit = ST_UNDEFINED;
    Reset_Cycle_Palette_Color();
    Load_Palette_From_Animation(combat_background_bottom);
    Apply_Palette();
    Cycle_Palette_Color(198, 0, 0, 0, 55, 0, 0, 8);
    Set_Palette_Changes(0, 243);
    Calculate_Remap_Colors();
    /* OGBUG: should set _active_battle_unit manually, it's unset/leftover here, could OOB AVWL */
    if(_combat_attacker_player == _human_player_idx)
    {
        Switch_Active_Battle_Unit(0);  /* first attacker battle_unit_idx */
    }
    else
    {
        Switch_Active_Battle_Unit((_combat_total_unit_count - defender_unit_count));  /* first defender battle_unit_idx */
    }
    frame_active_flag = 0;
    frame_scanned_flag = 0;
    niu_combat_screen_dirty = ST_TRUE;
    Combat_Screen_Draw();
    PageFlip_FX();
    _combat_city_damage = ST_FALSE;
    leave_screen = ST_FALSE;
    m_turn_is_local = ST_TRUE;
    _auto_combat_flag = ST_FALSE;
    /* CLAUDE */ if(g_cmbt_force_auto_combat == ST_TRUE) { _auto_combat_flag = ST_TRUE; }  /* test-support: see g_cmbt_force_auto_combat definition */
    Begin_Combat_Turn();
    _human_handle_immobile = ST_FALSE;
    /* OGBUG: redundant code, none of the above functions change the focus unit */
    if(_combat_attacker_player == _human_player_idx)
    {
        Switch_Active_Battle_Unit(0);
    }
    else
    {
        Switch_Active_Battle_Unit((_combat_total_unit_count - defender_unit_count));
    }
    frame_active_flag = 0;
    frame_scanned_flag = 0;
    niu_combat_screen_dirty = ST_TRUE;
    Combat_Screen_Draw();
    PageFlip_FX();
    m_cp_took_turn = ST_FALSE;
    if(_combat_defender_player == _combat_remote_player)
    {
        Auto_Cast_Spell_And_Do_Combat_Turn(_combat_defender_player);
        // ; BUG: the defending AI gets an extra turn?
        Begin_Combat_Turn();
        m_cp_took_turn = ST_TRUE;
    }
    winner_player_idx = Check_For_Winner();
    /* OGBUG: redundant code, second time clearing this without using it */
    if(winner_player_idx != ST_UNDEFINED)
    {
        leave_screen = ST_UNDEFINED;
    }
    _human_out_of_moves = ST_FALSE;
    if(_combat_attacker_player == _human_player_idx)
    {
        Next_Battle_Unit(_human_player_idx);
    }
    else
    {
        Switch_Active_Battle_Unit((_combat_total_unit_count - defender_unit_count));  /* first defender battle_unit_idx */
    }
    Combat_Screen_Draw();
    PageFlip_FX();
    Assign_Combat_Grids();
    Set_Input_Delay(3);
    Update_Combat_Enchantments_Icon_And_Help();
    Deactivate_Help_List();
    Set_Combat_Help_List();
    niu_combat_screen_dirty = ST_TRUE;
    while(leave_screen == ST_FALSE)
    {
        LOG_DEBUG(LOG_CAT_COMBAT, "BEGIN:  Combat Screen Loop");
        Assign_Auto_Function(Combat_Screen_Draw, 1);
        Mark_Time();
        Clear_Fields();
        /*
            BEGIN: Auto Combat
        */
        {
            LOG_DEBUG(LOG_CAT_COMBAT, "BEGIN:  Auto Combat Loop");
            if(_auto_combat_flag == ST_TRUE)
            {
                auto_combat_cancel_ESC_field = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, str_hotkey_ESC__ovr090[0], ST_UNDEFINED);
                input_field_idx = Get_Input();
                if(input_field_idx == auto_combat_cancel_ESC_field)  /* turn off 'Auto Combat' */
                {
                    Play_Left_Click();
                    _auto_combat_flag = ST_FALSE;
                    m_cp_took_turn = ST_FALSE;
                    niu_combat_screen_dirty = ST_TRUE;
                    Combat_Next_Turn();
                    Turn_Off_Auto_Combat();
                    niu_combat_screen_dirty = ST_TRUE;
                    _human_handle_immobile = ST_FALSE;
                }
                else  /* do 'Auto Combat' */
                {
                    Combat_Next_Turn();
                }
                /* CLAUDE */ { int16_t dbg_i; DBG_atk_active=0; DBG_atk_dead=0; DBG_atk_gone=0; DBG_def_active=0; DBG_def_dead=0; DBG_def_gone=0; for(dbg_i=0; dbg_i<_combat_total_unit_count; dbg_i++) { if(battle_units[dbg_i].controller_idx==_combat_attacker_player) { if(battle_units[dbg_i].status==bus_Active) DBG_atk_active++; else if(battle_units[dbg_i].status==bus_Dead) DBG_atk_dead++; else if(battle_units[dbg_i].status==bus_Gone) DBG_atk_gone++; } else if(battle_units[dbg_i].controller_idx==_combat_defender_player) { if(battle_units[dbg_i].status==bus_Active) DBG_def_active++; else if(battle_units[dbg_i].status==bus_Dead) DBG_def_dead++; else if(battle_units[dbg_i].status==bus_Gone) DBG_def_gone++; } } }
                winner_player_idx = Check_For_Winner();
                /* CLAUDE */ LOG_DEBUG(LOG_CAT_COMBAT, "[CombatLoop] atk: active=%d dead=%d gone=%d | def: active=%d dead=%d gone=%d | winner_player_idx=%d _combat_winner=%d", DBG_atk_active, DBG_atk_dead, DBG_atk_gone, DBG_def_active, DBG_def_dead, DBG_def_gone, winner_player_idx, _combat_winner);
                if(winner_player_idx != ST_UNDEFINED)
                {
                    LOG_DEBUG(LOG_CAT_COMBAT, "AUTOCOMBATWINNER");
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
            (
                (battle_units[_active_battle_unit].movement_points > 0)
                &&
                (
                    (battle_units[_active_battle_unit].mana >= 3)  /* OGBUG: units need at least 5 to cast anything */
                    ||
                    (Battle_Unit_Has_Spell_Ability(_active_battle_unit) == ST_TRUE)
                    ||
                    (battle_units[_active_battle_unit].item_charges > 0)
                )
            )
            ||
            (m_wizard_cast_available == ST_TRUE)
        )
        {
            spell_button_field = Add_Button_Field(144, SCREEN_YBOT_CMBT, str_empty_string__ovr090, _cmbt_spell_button_seg, cnst_HOTKEY_S_3[0], ST_UNDEFINED);
        }
        else
        {
            spell_button_field = INVALID_FIELD;
        }
        wait_button_field = Add_Button_Field(170, SCREEN_YBOT_CMBT, str_empty_string__ovr090, _cmbt_wait_button_seg, cnst_HOTKEY_W_2[0], ST_UNDEFINED);
        done_button_field = Add_Button_Field(170, (SCREEN_YBOT_CMBT + 20), str_empty_string__ovr090, _cmbt_done_button_seg, cnst_HOTKEY_D_5[0], ST_UNDEFINED);
        if(_combat_total_battle_effect_count > 0)
        {
            info_button_field = Add_Button_Field(144, 178, str_empty_string__ovr090, _cmbt_info_button_seg, cnst_HOTKEY_U_5[0], ST_UNDEFINED);
        }
        else
        {
            info_button_field = INVALID_FIELD;
        }
        auto_button_field = Add_Button_Field(170, (SCREEN_YBOT_CMBT + 10), str_empty_string__ovr090, _cmbt_auto_button_seg, cnst_HOTKEY_A_4[0], ST_UNDEFINED);
        flee_button_field = Add_Button_Field(144, (SCREEN_YBOT_CMBT + 20), str_empty_string__ovr090, _cmbt_flee_button_seg, cnst_HOTKEY_F[0], ST_UNDEFINED);
        // ¿ Right-Click brings up 'Unit View' ?
        active_unit_window_field = Add_Hidden_Field(83, 173, 116, 198, str_empty_string__ovr090[0], ST_UNDEFINED);
        Add_Combat_Enchantment_Fields();
        escape_field = Add_Hidden_Field(0, 164, SCREEN_XMAX, SCREEN_YMAX, str_hotkey_ESC__ovr090[0], ST_UNDEFINED);
        combat_grid_field = Add_Grid_Field(0, 0, 1, 1, SCREEN_XMAX, 164, &grid_sx, &grid_sy, ST_UNDEFINED);
        space_hotkey_field = Add_Hot_Key(cnst_HOTKEY_SPACE_4[0]);
#ifdef STU_DEBUG
        hotkey_idx_Z = Add_Hot_Key('Z');  // debug_hotkey  ...  Derp. 'D' is already used for the "Done" button.
        hotkey_idx_T = Add_Hot_Key('T');  // test_hotkey
#endif
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
            TOGGLE(_auto_combat_flag);
            _human_out_of_moves = ST_FALSE;
            Clear_Fields();
            input_field_idx = 0;
            Auto_Cast_Spell_And_Do_Combat_Turn(_combat_local_player);
            winner_player_idx = Check_For_Winner();
            if(winner_player_idx != ST_UNDEFINED)
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
            (battle_units[_active_battle_unit].status > bus_Active)  // disengaged - recalled, fled, inelligable, dead, dead, dead
        )
        {
            battle_units[_active_battle_unit].mid_move = ST_FALSE;
            battle_units[_active_battle_unit].movement_points = 0;
            battle_units[_active_battle_unit].action = bua_Finished;
            if(
                ((battle_units[_active_battle_unit].combat_effects & bue_Confusion) != 0)
                &&
                (battle_units[_active_battle_unit].confusion_state == 2)
            )
            {
                if(battle_units[_active_battle_unit].controller_idx == _combat_attacker_player)
                {
                    battle_units[_active_battle_unit].controller_idx = (int8_t)_combat_defender_player;
                }
                else
                {
                    battle_units[_active_battle_unit].controller_idx = (int8_t)_combat_attacker_player;
                }
            }
            Next_Battle_Unit(_human_player_idx);
            Assign_Combat_Grids();
            input_field_idx = ST_UNDEFINED;
            screen_changed = ST_TRUE;
            niu_combat_screen_dirty = ST_TRUE;
            winner_player_idx = Check_For_Winner();
            if(winner_player_idx != ST_UNDEFINED)
            {
                leave_screen = ST_UNDEFINED;
                input_field_idx = 0;
            }
        }
        /*
            END:  ¿ what is going on here ?
        */
        if(
            (battle_units[_active_battle_unit].confusion_state == 1)
            &&
            (battle_units[_active_battle_unit].movement_points > 0)
        )
        {
            Move_Confused(_active_battle_unit);
            Next_Battle_Unit(_human_player_idx);
            Assign_Combat_Grids();
            input_field_idx = ST_UNDEFINED;
            screen_changed = ST_TRUE;
            niu_combat_screen_dirty = ST_TRUE;
            winner_player_idx = Check_For_Winner();
            if(winner_player_idx != ST_UNDEFINED)
            {
                leave_screen = ST_UNDEFINED;
                input_field_idx = 0;
            }
        }
        for(itr = 0; ((itr < attacker_enchantment_count) && (itr < 4) && (attacker_enchantment_count > 4)); itr++)
        {
            if(attacker_enchantment_fields[itr] == input_field_idx)
            {
                Play_Left_Click();
                attacker_enchantment_first_shown += 4;
                if(attacker_enchantment_first_shown > 8)
                {
                    attacker_enchantment_first_shown = 0;
                }
                niu_combat_screen_dirty = ST_TRUE;
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
                defender_enchantment_first_shown += 4;
                if(defender_enchantment_first_shown > 8)
                {
                    defender_enchantment_first_shown = 0;
                }
                niu_combat_screen_dirty = ST_TRUE;
                break;
            }
        }
        /*
            BEGIN:  Left-Click Flee Button
        */
        if(input_field_idx == flee_button_field)
        {
            _human_handle_immobile = ST_FALSE;
            Play_Left_Click();
            Set_Mouse_List(1, mouse_list_default);
            if(Confirmation_Box(cnst_FleeConfirm_Msg) == ST_TRUE)
            {
                for(itr = 0; itr < _combat_total_unit_count; itr++)
                {
                    if(battle_units[itr].controller_idx == _combat_local_player)
                    {
                        battle_units[itr].action = bua_Flee;
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
                winner_player_idx = _combat_remote_player;
                human_player_did_flee = ST_TRUE;
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
                _human_handle_immobile = ST_FALSE;
                frame_scanned_cgx = Get_Combat_Grid_Cell_X(((int16_t)grid_sx + 4), ((int16_t)grid_sy + 4));
                frame_scanned_cgy = Get_Combat_Grid_Cell_Y(((int16_t)grid_sx + 4), ((int16_t)grid_sy + 4));
                Battle_Unit_Action(_active_battle_unit, frame_scanned_cgx, frame_scanned_cgy);
                for(itr = 0; itr < _combat_total_unit_count; itr++)
                {
                    Battle_Unit_Set_Invisibility_Effect(itr);
                }
                Assign_Combat_Grids();
                niu_combat_screen_dirty = ST_TRUE;
                winner_player_idx = Check_For_Winner();
                if(winner_player_idx != ST_UNDEFINED)  /* invalid / no winner_player_idx / none / neither */
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
                right_click_cgx = Get_Combat_Grid_Cell_X(((int16_t)grid_sx + 4), ((int16_t)grid_sy + 4));
                right_click_cgy = Get_Combat_Grid_Cell_Y(((int16_t)grid_sx + 4), ((int16_t)grid_sy + 4));
                battle_unit_idx = g_combat_grid_action_map[right_click_cgy][right_click_cgx];
                /* OGBUG: battle_unit_idx needs to be range checked, it can be 99 */
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
                        temp_movement_points = battle_units[battle_unit_idx].movement_points;
                        battle_units[battle_unit_idx].movement_points = (int8_t)Battle_Unit_Moves2(battle_unit_idx);
                        temp_unit_enchantments = _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments;
                        _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments = (_UNITS[battle_units[battle_unit_idx].unit_idx].enchantments | battle_units[battle_unit_idx].enchantments);
                        memcpy(global_battle_unit, &battle_units[battle_unit_idx], sizeof(struct s_BATTLE_UNIT));
                        Combat_Unit_Statistics_Window(61, 6, 89, 174, 117, 194, 2, battle_units[battle_unit_idx].unit_idx);
                        battle_units[battle_unit_idx].movement_points = temp_movement_points;
                        _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments = temp_unit_enchantments;
                        Allocate_Combat_Near_Buffers();
                        Assign_Combat_Grids();
                        Assign_Auto_Function(Combat_Screen_Draw, 1);
                        Update_Combat_Enchantments_Icon_And_Help();
                        Combat_Compose_Background();  // ... |-> Copy_Off_To_Back();
                        Deactivate_Help_List();
                        Set_Combat_Help_List();
                        niu_combat_screen_dirty = ST_TRUE;
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
                            temp_movement_points = battle_units[battle_unit_idx].movement_points;
                            battle_units[battle_unit_idx].movement_points = (int8_t)Battle_Unit_Moves2(battle_unit_idx);
                            temp_unit_enchantments = _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments;
                            _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments = (_UNITS[battle_units[battle_unit_idx].unit_idx].enchantments | battle_units[battle_unit_idx].enchantments);
                            memcpy(global_battle_unit, &battle_units[battle_unit_idx], sizeof(struct s_BATTLE_UNIT));
                            Combat_Unit_Statistics_Window(61, 6, 89, 174, 117, 194, 2, battle_units[battle_unit_idx].unit_idx);
                            battle_units[battle_unit_idx].movement_points = (int8_t)temp_movement_points;
                            _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments = temp_unit_enchantments;
                            Allocate_Combat_Near_Buffers();
                            Assign_Combat_Grids();
                            Assign_Auto_Function(Combat_Screen_Draw, 1);
                            Update_Combat_Enchantments_Icon_And_Help();
                            Combat_Compose_Background();  // ... |-> Copy_Off_To_Back();
                            Deactivate_Help_List();
                            Set_Combat_Help_List();
                        }
                        niu_combat_screen_dirty = ST_TRUE;
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
                        (battle_units[_active_battle_unit].mana >= 3)  /* OGBUG: units need at least 5 to cast anything */
                        ||
                        (Battle_Unit_Has_Spell_Ability(_active_battle_unit) == ST_TRUE)
                        ||
                        (battle_units[_active_battle_unit].item_charges > 0)
                    )
                )
                {
                    Combat_Cast_Spell_With_Caster(_active_battle_unit);
                }
                else
                {
                    if(m_wizard_cast_available == ST_TRUE)
                    {
                        cast_status = Combat_Cast_Spell((CASTER_IDX_BASE + _human_player_idx), _combat_wx, _combat_wy, _combat_wp);
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
                                m_wizard_cast_available = ST_FALSE;
                                _human_handle_immobile = ST_FALSE;
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
                Assign_Auto_Function(Combat_Screen_Draw, 1);
                Assign_Combat_Grids();
                Update_Combat_Enchantments_Icon_And_Help();  // ¿ because you may just cast a 'Combat Enchantment'
                Combat_Compose_Background();  // ... |-> Copy_Off_To_Back();
                Deactivate_Help_List();
                Set_Combat_Help_List();
                niu_combat_screen_dirty = ST_TRUE;
                winner_player_idx = Check_For_Winner();  // ¿ because your spell may have just killed the last enemy unit ?
                if(winner_player_idx != ST_UNDEFINED)
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
                _human_handle_immobile = ST_FALSE;
                Play_Left_Click();
                battle_units[_active_battle_unit].action = bua_Wait;
                Next_Battle_Unit(_human_player_idx);
                Assign_Combat_Grids();
                niu_combat_screen_dirty = ST_TRUE;
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
                _human_handle_immobile = ST_FALSE;
                Play_Left_Click();
                Deactivate_Help_List();
                Combat_Information_Window();
                Allocate_Combat_Near_Buffers();
                Assign_Auto_Function(Combat_Screen_Draw, 1);
                Assign_Combat_Grids();
                Update_Combat_Enchantments_Icon_And_Help();
                Combat_Compose_Background();  // ... |-> Copy_Off_To_Back();
                Deactivate_Help_List();
                Set_Combat_Help_List();
                niu_combat_screen_dirty = ST_TRUE;
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
                (input_field_idx == space_hotkey_field)
            )
            {
                _human_handle_immobile = ST_FALSE;
                Play_Left_Click();
                battle_units[_active_battle_unit].action = bua_Finished;
                battle_units[_active_battle_unit].movement_points = 0;
                Next_Battle_Unit(_human_player_idx);
                Assign_Combat_Grids();
                if(_human_out_of_moves == ST_TRUE)  // human player's turn is over
                {
                    screen_changed = ST_TRUE;
                }
                niu_combat_screen_dirty = ST_TRUE;
                winner_player_idx = Check_For_Winner();
                if(winner_player_idx != ST_UNDEFINED)
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
            temp_movement_points = battle_units[_active_battle_unit].movement_points;
            battle_units[_active_battle_unit].movement_points = (int8_t)Battle_Unit_Moves2(_active_battle_unit);
            temp_unit_enchantments = _UNITS[battle_units[_active_battle_unit].unit_idx].enchantments;
            _UNITS[battle_units[_active_battle_unit].unit_idx].enchantments = (_UNITS[battle_units[_active_battle_unit].unit_idx].enchantments | battle_units[_active_battle_unit].enchantments);
            memcpy(global_battle_unit, &battle_units[_active_battle_unit], sizeof(struct s_BATTLE_UNIT));
            Combat_Unit_Statistics_Window(61, 6, 89, 174, 117, 194, uvt_Cmbt, battle_units[_active_battle_unit].unit_idx);
            battle_units[_active_battle_unit].movement_points = (int8_t)temp_movement_points;
            _UNITS[battle_units[_active_battle_unit].unit_idx].enchantments = temp_unit_enchantments;
            Allocate_Combat_Near_Buffers();
            Assign_Combat_Grids();
            Assign_Auto_Function(Combat_Screen_Draw, 1);
            Combat_Compose_Background();  // ... |-> Copy_Off_To_Back();
            Update_Combat_Enchantments_Icon_And_Help();
            Deactivate_Help_List();
            Set_Combat_Help_List();
            niu_combat_screen_dirty = ST_TRUE;
        }
        /*
            END:  Right-Click Unit
        */
        /*
            BEGIN:  ST_DEBUG Hot-Keys
        */
#ifdef STU_DEBUG
        if(input_field_idx == hotkey_idx_Z)  /* Debug Hot-Key */
        {
            DLOG("STU_DEBUG: Combat: debug_hotkey");
            DBG_debug_flag = !DBG_debug_flag;  // ~== `^= 1`
            if(DBG_debug_flag)
            {

            }
        }
        if(input_field_idx == hotkey_idx_T)  /* Test Hot-Key */
        {
            DLOG("STU_DEBUG: Combat: test_hotkey");

        }
#endif
        /*
            END:  ST_DEBUG Hot-Keys
        */
        // NOTE(JimBalcomb,20250729): this debug-break still has never been hit
        // NOTE(JimBalcomb,20260331): this debug-break still has never been hit
        // NOTE(JimBalcomb,20260717): this debug-break has now been hit  (don't know why)
        // What is this?  sanity check? hack bug-fix?  should actually never happen?
        // When does _active_battle_unit ever get set to a battle_unit_idx that is not created/owner/controlled by the human player?
        if(battle_units[_active_battle_unit].controller_idx != _combat_local_player)
        {
            // STU_DEBUG_BREAK();
            _human_out_of_moves = ST_TRUE;  // human turn is over
            _human_handle_immobile = ST_FALSE;  // don't draw target frames or all immobilized message
        }
        // What is this block?
        // ¿ only call to Combat_Next_Turn() outside of 'Auto Combat' ?
        if(
            (leave_screen == ST_FALSE)
            &&
            (_human_out_of_moves == ST_TRUE)  // human player's turn is over
            &&
            (_human_handle_immobile == ST_FALSE)  // 
        )
        {
            _human_out_of_moves = ST_FALSE;  // Where does this get used after this?
            Combat_Next_Turn();
            // maybe, sets _human_out_of_moves = ST_TRUE and/or _human_handle_immobile = ST_FALSE
            Next_Battle_Unit(_human_player_idx);
            Assign_Combat_Grids();
            input_field_idx = ST_UNDEFINED;
            screen_changed = ST_TRUE;
            niu_combat_screen_dirty = ST_TRUE;
            winner_player_idx = Check_For_Winner();
            if(winner_player_idx != ST_UNDEFINED)
            {
                leave_screen = ST_UNDEFINED;
                input_field_idx = 0;
            }
        }
        if(
            (leave_screen == ST_FALSE)
            &&
            (screen_changed == ST_FALSE)
        )
        {
            Combat_Screen_Draw();
            Assign_Mouse_Images();
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
    if(_computer_player_did_flee == ST_TRUE)
    {
        end_of_combat_message_type = csmt_EnemyFled;
    }
    else if(winner_player_idx == _human_player_idx)
    {
        end_of_combat_message_type = csmt_Victory;
    }
    else if(human_player_did_flee == ST_TRUE)
    {
        end_of_combat_message_type = csmt_PlayerFled;
    }
    else if(_combat_turn > 50)
    {
        end_of_combat_message_type = csmt_TurnLimit;
    }
    else
    {
        end_of_combat_message_type = csmt_Defeat;
    }
    Copy_Page4_To_Off();
    Copy_Off_To_Back();
    Stop_All_Sounds__STUB();
#ifdef STU_DEF
    /* CLAUDE */ LOG_DEBUG(LOG_CAT_COMBAT, "[Combat_Screen] snapshot: _units=%d  _combat_total_unit_count=%d", _units, _combat_total_unit_count);
    memcpy(DBG_battle_units, battle_units, (sizeof(struct s_BATTLE_UNIT) * _combat_total_unit_count));
#endif
    Combat_Cache_Read();        // reloads World_Data
    Cache_Graphics_Overland();  // reloads g_graphics_cache_seg
#ifdef STU_DEF
    /* CLAUDE */ DBG_Compare_Battle_Units("after Combat_Cache_Read + Cache_Graphics_Overland");
#endif
    Mark_Time();
    Release_Time(1);
    Play_Background_Music();
    End_Of_Combat(winner_player_idx, item_count, item_list, end_of_combat_message_type);
    // if(winner_player_idx == _combat_attacker_player)
    // {
    //     return ST_TRUE;
    // }
    // else
    // {
    //     return ST_FALSE;
    // }
    // ~ Strategic_Combat()
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




/*
    WIZARDS.EXE  ovr091
*/

// WZD s91p01
void NIU_Battle_Unit_Load_Figure_Pictures(int16_t battle_unit_idx)
{
    int16_t unit_type = 0;
    int16_t bufpi = 0;
    unit_type = _UNITS[battle_units[battle_unit_idx].unit_idx].type;
    bufpi = Combat_Figure_Load(unit_type, battle_unit_idx);
    battle_units[battle_unit_idx].bufpi = bufpi;
}


// WZD s91p02
/*

    increments _combat_turn  [MoO2: incremented at end of Tactical_Combat_()]

    calls Battle_Unit_Regular_Stats() on all battle units
    then, manually sets movement_points via Battle_Unit_Moves2()
    PS. Battle_Unit_Regular_Stats() does battle_unit->movement_points = Unit_Moves2(unit_idx);

    MoO2
    End_Of_Turn_Bookeeping_()
        Recharge_Shields_();  Repair_All_Combat_Ships_();  check retreat;  reset weapons;

*/
void Begin_Combat_Turn(void)
{
    int16_t saved_active_battle_unit = 0;
    int16_t resist_fails = 0;
    int16_t itr = 0;
    _human_handle_immobile = ST_TRUE;
    _ai_immobile_counter++;
    _combat_turn++;
    m_wizard_cast_available = ST_TRUE;
    _scanned_battle_unit = ST_UNDEFINED;
    m_cp_took_turn = ST_FALSE;
    /*
        BEGIN:  apply spell effects per-turn/on-going
    */
    Apply_Mana_Leak();
    if(combat_enchantments[CALL_LIGHTNING_ATTKR] != 0)
    {
        Apply_Call_Lightning(_combat_attacker_player);
    }
    if(combat_enchantments[CALL_LIGHTNING_DFNDR] != 0)
    {
        Apply_Call_Lightning(_combat_defender_player);
    }
    if(combat_enchantments[WRACK_ATTKR] != 0)
    {
        Apply_Wrack(_combat_attacker_player);
    }
    if(combat_enchantments[WRACK_DFNDR] != 0)
    {
        Apply_Wrack(_combat_defender_player);
    }
    /*
        BEGIN: Magic Vortex
    */
    saved_active_battle_unit = _active_battle_unit;
    Vortex_Combat_Round();
    _active_battle_unit = saved_active_battle_unit;
    /*
        END: Magic Vortex
    */
    Calc_Battlefield_Bonuses(_combat_structure);
    Add_City_Damage_From_Battle_Units_Within();
    for(itr = 0; itr < _combat_total_unit_count; itr++)
    {
        battle_units[itr].Suppression = 0;
        if(battle_units[itr].status != bus_Active)
        {
            continue;
        }
        // ¿ Why not Load_Battle_Unit() |-> Battle_Unit_Regular_Stats() ?
        Battle_Unit_Regular_Stats(&battle_units[itr]);
        Battle_Unit_Special_Stats(&battle_units[itr]);
        battle_units[itr].movement_points = (int8_t)Battle_Unit_Moves2(itr);  // ¿ manually fix-up the value set in Battle_Unit_Regular_Stats() ?
        battle_units[itr].action = bus_Active;
        if(
            (
                (combat_enchantments[TERROR_ATTKR] != 0)
                &&
                (battle_units[itr].controller_idx == _combat_defender_player)
            )
            ||
            (
                (combat_enchantments[TERROR_DFNDR] != 0)
                &&
                (battle_units[itr].controller_idx == _combat_attacker_player)
            )
        )
        {
            // ¿ ; BUG: this is already applied to the unit itself ?
            resist_fails = Combat_Resistance_Check(battle_units[itr], (_battlefield_resistall[battle_units[itr].controller_idx] + 1), spell_data_table[spl_Terror].magic_realm);
            if(resist_fails > 0)
            {
                battle_units[itr].action = bua_Finished;
                battle_units[itr].movement_points = 0;
            }
        }
        if(
            (
                (combat_enchantments[ENTANGLE_ATTKR] != 0)
                &&
                (battle_units[itr].controller_idx == _combat_defender_player)
            )
            ||
            (
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
                    resist_fails = battle_units[itr].melee;
                }
                else
                {
                    resist_fails = battle_units[itr].ranged;
                }
            }
            else if(battle_units[itr].ranged_type > srat_Thrown)
            {
                resist_fails = (battle_units[itr].melee + battle_units[itr].ranged);
            }
            else
            {
                resist_fails = battle_units[itr].melee;
            }
            battle_units[itr].Web_HP -= resist_fails;
            if(battle_units[itr].Web_HP > 0)
            {
                battle_units[itr].action = bua_Finished;
                battle_units[itr].movement_points = 0;
            }
            else
            {
                battle_units[itr].Web_HP = 0;
            }
        }
        if((battle_units[itr].combat_effects & bue_Black_Sleep) != 0)
        {
            battle_units[itr].action = bua_Finished;
            battle_units[itr].movement_points = 0;
        }
        /*
            BEGIN:  Confusion

                "Every turn the confused unit randomly does one of the following: stand around and do nothing while looking foolish and confused, move randomly, attack allies, or attack enemies."
        */
        // ; BUG: resets the state without ever returning control of the unit to its previous owner!
        // ¿ NOBUG  pretty sure this is covered by having called Battle_Unit_Regular_Stats() to (re-)initialize the battle unit ?
        battle_units[itr].confusion_state = 0;
        if((battle_units[itr].combat_effects & bue_Confusion) != 0)
        {
            resist_fails = Random(4);
            if(resist_fails == 1)
            {
                battle_units[itr].action = bua_Finished;
                battle_units[itr].movement_points = 0;
            }
            if(resist_fails == 2)
            {
                battle_units[itr].confusion_state = 1;
            }
            if(resist_fails == 3)
            {
                battle_units[itr].confusion_state = 2;
                if(battle_units[itr].controller_idx == _combat_attacker_player)
                {
                    battle_units[itr].controller_idx = (int8_t)_combat_defender_player;
                }
                else
                {
                    battle_units[itr].controller_idx = (int8_t)_combat_attacker_player;
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
void Set_Movement_Cost_Map(int16_t battle_unit_idx)
{
    int16_t battle_unit_movement_mode = 0;
    int16_t instant_movement_mode = 0;
    int16_t itr_y = 0;
    int16_t itr_x = 0;
    battle_unit_movement_mode = Battle_Unit_Movement_Mode(battle_unit_idx);
    instant_movement_mode = Battle_Unit_Instant_Movement_Mode(battle_unit_idx);
    if(instant_movement_mode > 0)
    {
        battle_unit_movement_mode = instant_movement_mode;
    }
    switch(battle_unit_movement_mode)
    {
        case bumm_Ground0:
        case bumm_Ground1:
        {
            memcpy(_cmbt_movepath_cost_map, &battlefield->MoveCost_Ground[0], COMBAT_GRID_CELL_COUNT);
        } break;
        case bumm_Flight:
        {
            memcpy(_cmbt_movepath_cost_map, &battlefield->MoveCost_Teleport[0], COMBAT_GRID_CELL_COUNT);
        } break;
        case bumm_Ground3:
        {
            memcpy(_cmbt_movepath_cost_map, &battlefield->MoveCost_Ground2[0], COMBAT_GRID_CELL_COUNT);  /* OGBUG  unreachable */
        } break;
        case bumm_Sailing:
        {
            memcpy(_cmbt_movepath_cost_map, &battlefield->MoveCost_Sailing[0], COMBAT_GRID_CELL_COUNT);
        } break;
        case bumm_Swimming:
        {
            for(itr_y = 0; itr_y < COMBAT_GRID_HEIGHT; itr_y++)
            {
                for(itr_x = 0; itr_x < COMBAT_GRID_WIDTH; itr_x++)
                {
                    if(battlefield->MoveCost_Sailing[((itr_y * COMBAT_GRID_WIDTH) + itr_x)] > battlefield->MoveCost_Ground[((itr_y * COMBAT_GRID_WIDTH) + itr_x)])
                    {
                        _cmbt_movepath_cost_map[((itr_y * COMBAT_GRID_WIDTH) + itr_x)] = battlefield->MoveCost_Ground[((itr_y * COMBAT_GRID_WIDTH) + itr_x)];
                    }
                    else
                    {
                        _cmbt_movepath_cost_map[((itr_y * COMBAT_GRID_WIDTH) + itr_x)] = battlefield->MoveCost_Sailing[((itr_y * COMBAT_GRID_WIDTH) + itr_x)];
                    }
                }
            }
        } break;
        case bumm_Teleport:
        case bumm_Tunnel:
        {
            memcpy(_cmbt_movepath_cost_map, &battlefield->MoveCost_Teleport[0], COMBAT_GRID_CELL_COUNT);
        } break;
    }
}


// WZD s91p04
/*
; BUG: prevents moving into the central structure square in regular city battles
; BUG: teleporting units can't use roads
*/
/*
~ Combat.c   Auto_Move_Unit()
~ MainScr.c  Move_Units_Draw()
*/
void Move_Battle_Unit(int16_t battle_unit_idx, int16_t target_cgx, int16_t target_cgy)
{
    SAMB_ptr move_sound_seg = 0;
    int16_t has_instant_movement_mode = 0;
    int16_t instant_movement_mode = 0;
    int16_t Last_Facing_Y = 0;
    int16_t Last_Facing_X = 0;
    int16_t Origin_Y = 0;
    int16_t Origin_X = 0;
    int16_t battle_unit_owner_idx = 0;
    int16_t Facing_Diff_Y = 0;
    int16_t Facing_Diff_X = 0;
    int16_t Move_Step_Index = 0;
    int16_t itr = 0;
    uint32_t move_sound_seg_size = 0;  // DNE in Dasm
    battle_unit_owner_idx = battle_units[battle_unit_idx].controller_idx;
    Set_Movement_Cost_Map(battle_unit_idx);
    for(itr = 0; itr < _combat_total_unit_count; itr++)
    {
        if(battle_units[itr].status == bus_Active)
        {
            _cmbt_movepath_cost_map[((battle_units[itr].cgy * COMBAT_GRID_WIDTH) + battle_units[itr].cgx)] = INF;  /* ¿ occupied ? */
        }
    }
    for(itr = 0; itr < _vortex_count; itr++)
    {
        _cmbt_movepath_cost_map[((_vortexes[itr].cgy * COMBAT_GRID_WIDTH) + _vortexes[itr].cgx)] = INF;  /* ¿ occupied ? */
    }
    /* OGBUG  outposts and cities don't have anything on that square either; same as in Assign_Combat_Grids() */
    if(battlefield->center_square_structure != CS_None)
    {
        _cmbt_movepath_cost_map[COMBAT_STRUCTURE_IDX] = INF;
    }
    if(
        (battlefield->walled == ST_TRUE)
        ||
        (battlefield->city_enchantments[FLYING_FORTRESS] > 0)
    )
    {
        Update_Move_Map_City_Area_Restrictions(battle_unit_idx);
    }
    has_instant_movement_mode = ST_FALSE;
    instant_movement_mode = Battle_Unit_Instant_Movement_Mode(battle_unit_idx);
    if(
        (instant_movement_mode == bumm_Teleport)
        ||
        (instant_movement_mode == bumm_Tunnel)
    )
    {
        movement_path_grid_cell_count = 0;
        has_instant_movement_mode = ST_TRUE;
    }
    else
    {
        Combat_Move_Path_Find(battle_units[battle_unit_idx].cgx, battle_units[battle_unit_idx].cgy, target_cgx, target_cgy);
        if(movement_path_grid_cell_count == 0)
        {
            return;
        }
    }
    Origin_X = battle_units[battle_unit_idx].cgx;
    Origin_Y = battle_units[battle_unit_idx].cgy;
    uu_combat_movement_variable = 0;  // ; cleared before movement, never used for anything
    battle_units[battle_unit_idx].mid_move = ST_TRUE;
    move_sound_seg = Get_Battle_Unit_Move_Sound_Buffer(battle_unit_idx, &move_sound_seg_size);
    if(has_instant_movement_mode != ST_FALSE)
    {
        if(instant_movement_mode == bumm_Teleport)
        {
            Battle_Unit_Teleport(battle_unit_idx, target_cgx, target_cgy);
        }
        else  /* instant_movement_mode == bumm_Tunnel */
        {
            Battle_Unit_Tunnel(battle_unit_idx, target_cgx, target_cgy);
        }
    }
    else
    {
        battle_units[battle_unit_idx].target_cgx = target_cgx;
        battle_units[battle_unit_idx].target_cgy = target_cgy;
        for(Move_Step_Index = 0; Move_Step_Index < movement_path_grid_cell_count; Move_Step_Index++)
        {
            battle_units[battle_unit_idx].target_cgx = _cmbt_mvpth_x[Move_Step_Index];
            battle_units[battle_unit_idx].target_cgy = _cmbt_mvpth_y[Move_Step_Index];
            if(magic_set.sound_effects == ST_TRUE)
            {
                Play_Sound(move_sound_seg, move_sound_seg_size);
            }
            if(magic_set.movement_animations == ST_TRUE)
            {
                for(itr = 0; itr < MOVE_ANIM_CNT; itr++)
                {
                    /* HACK  pacing */  Mark_Time();
                    battle_units[battle_unit_idx].move_anim_ctr += 1;
                    Combat_Screen_Draw();
                    PageFlip_FX();
                    /* HACK  pacing */  Release_Time(1);
                }
            }
            else
            {
                battle_units[battle_unit_idx].move_anim_ctr = MOVE_ANIM_MAX;
                Combat_Screen_Draw();
                PageFlip_FX();
            }
            if(magic_set.sound_effects == ST_TRUE)
            {
                Play_Sound(sound_silent_seg, sound_silent_seg_size);
            }
            Check_Wall_Of_Fire_Attack(battle_unit_idx);
            if(battle_units[battle_unit_idx].status != bus_Active)
            {
                break;
            }
            battle_units[battle_unit_idx].move_anim_ctr = 0;
            battle_units[battle_unit_idx].cgx = _cmbt_mvpth_x[Move_Step_Index];
            battle_units[battle_unit_idx].cgy = _cmbt_mvpth_y[Move_Step_Index];
        }
    }
    battle_units[battle_unit_idx].move_anim_ctr = 0;
    battle_units[battle_unit_idx].mid_move = ST_FALSE;
    if(movement_path_grid_cell_count > 1)
    {
        Origin_X = _cmbt_mvpth_x[(movement_path_grid_cell_count - 2)];
        Origin_Y = _cmbt_mvpth_y[(movement_path_grid_cell_count - 2)];
        Last_Facing_X = _cmbt_mvpth_x[(movement_path_grid_cell_count - 1)];
        Last_Facing_Y = _cmbt_mvpth_y[(movement_path_grid_cell_count - 1)];
    }
    else
    {
        Last_Facing_X = target_cgx;
        Last_Facing_Y = target_cgy;
    }
    Facing_Diff_X = (Last_Facing_X - Origin_X);
    Facing_Diff_Y = (Last_Facing_Y - Origin_Y);
    if(has_instant_movement_mode != ST_FALSE)
    {
        if(battle_units[battle_unit_idx].movement_points > 2)
        {
            battle_units[battle_unit_idx].movement_points -= 2;
        }
        else
        {
            battle_units[battle_unit_idx].movement_points = 0;
        }
    }
    else
    {
        battle_units[battle_unit_idx].cgx = battle_units[battle_unit_idx].target_cgx;
        battle_units[battle_unit_idx].cgy = battle_units[battle_unit_idx].target_cgy;
        battle_units[battle_unit_idx].target_cgx = (Last_Facing_X + Facing_Diff_X);
        battle_units[battle_unit_idx].target_cgy = (Last_Facing_Y + Facing_Diff_Y);
        battle_units[battle_unit_idx].movement_points -= _cmbt_mvpth_c[((battle_units[battle_unit_idx].cgy * COMBAT_GRID_WIDTH) + battle_units[battle_unit_idx].cgx)];
    }
    SETMIN(battle_units[battle_unit_idx].movement_points, 0);
}


// WZD s91p05
// ¿ MoO2  Module: COMBAT1  Set_Cur_Ship_To_() ?
void Switch_Active_Battle_Unit(int16_t battle_unit_idx)
{
    battle_units[_active_battle_unit].mid_move = ST_FALSE;
    _active_battle_unit = battle_unit_idx;
}


// WZD s91p06
void Battle_Unit_Action(int16_t _battle_unit_idx, int16_t cgx, int16_t cgy)
{
    int16_t battle_unit_idx = 0;
    int16_t combat_grid_target = 0;
    int16_t target_cgx = 0;
    int16_t target_cgy = 0;
    int16_t delta_x = 0;
    int16_t delta_y = 0;
    int16_t ranged_group = 0;
    int16_t niu_local_variable = 0;
    battle_unit_idx = _battle_unit_idx;
    /* Get target unit/object index from combat grid */
    combat_grid_target = g_combat_grid_action_map[cgy][cgx];
    if(combat_grid_target != 99)
    {
        if(combat_grid_target < 0)
        {
            goto loc_MoveCheck;
        }
        /* If target is a unit, check if it's an active enemy */
        if(battle_units[combat_grid_target].controller_idx == battle_units[battle_unit_idx].controller_idx)
        {
            goto loc_MoveCheck;
        }
        if(battle_units[combat_grid_target].status != bus_Active)
        {
            goto loc_MoveCheck;
        }
    }
    /* Target is either a wall (99) or an active enemy unit */
    if(combat_grid_target == 99)
    {
        target_cgx = cgx;
        target_cgy = cgy;
    }
    else
    {
        target_cgx = battle_units[combat_grid_target].cgx;
        target_cgy = battle_units[combat_grid_target].cgy;
    }
    /* Calculate Manhattan distance components */
    delta_x = abs(target_cgx - battle_units[battle_unit_idx].cgx);
    delta_y = abs(target_cgy - battle_units[battle_unit_idx].cgy);
    niu_local_variable = -2;
    if(battle_units[battle_unit_idx].movement_points <= 0)
    {
        return;
    }
    if(combat_grid_target == 99)
    {
        /* Wall/Object attack logic */
        if(delta_x <= 1 && delta_y <= 1)
        {
            Battle_Unit_Attack(battle_unit_idx, combat_grid_target, target_cgx, target_cgy);
        }
        else
        {
            ranged_group = battle_units[battle_unit_idx].ranged_type / 10;
            if(ranged_group == rag_Boulder || ranged_group == rag_Missile || ranged_group == rag_Magic)
            {
                Battle_Unit_Attack(battle_unit_idx, combat_grid_target, target_cgx, target_cgy);
            }
        }
    }
    else
    {
        /* Enemy unit interaction logic */
        if(Check_Attack_Melee(battle_unit_idx, combat_grid_target) == ST_TRUE)
        {
            if(delta_x <= 1 && delta_y <= 1)
            {
                /* Adjacent melee check (includes flight/wall physics) */
                if(Check_Attack_Melee_City_Wall(battle_unit_idx, combat_grid_target) == ST_TRUE)
                {
                    Battle_Unit_Attack(battle_unit_idx, combat_grid_target, target_cgx, target_cgy);
                }
                else
                {
                    /* Failed wall check? Try ranged if available */
                    ranged_group = battle_units[battle_unit_idx].ranged_type / 10;
                    if(ranged_group == rag_Boulder || ranged_group == rag_Missile || ranged_group == rag_Magic)
                    {
                        Battle_Unit_Attack(battle_unit_idx, combat_grid_target, target_cgx, target_cgy);
                    }
                }
            }
            else
            {
                /* Not adjacent: Ranged attack only */
                ranged_group = battle_units[battle_unit_idx].ranged_type / 10;
                if(ranged_group == rag_Boulder || ranged_group == rag_Missile || ranged_group == rag_Magic)
                {
                    Battle_Unit_Attack(battle_unit_idx, combat_grid_target, target_cgx, target_cgy);
                }
            }
        }
    }
    return;
loc_MoveCheck:
    /* Move logic for empty squares */
    if(combat_grid_target == -1)
    {
        if(battle_units[battle_unit_idx].movement_points > 0)
        {
            Move_Battle_Unit(battle_unit_idx, cgx, cgy);
        }
    }
    return;
}


// WZD s91p07
void Assign_Combat_Grids(void)
{
    int16_t has_instant_movement_mode = 0;
    int16_t instant_movement_mode = 0;
    int16_t moves2 = 0;
    int16_t uu_max_moves2 = 0;
    int16_t uu_count_of_reachable_cells = 0;
    int16_t uu_cgy = 0;
    int16_t uu_cgx = 0;
    int16_t cgy_offset = 0;
    int16_t itr = 0;
    int16_t itr_x = 0;
    int16_t itr_y = 0;
    int16_t useable_moves2 = 0;
    Set_Movement_Cost_Map(_active_battle_unit);
    for(itr = 0; itr < _combat_total_unit_count; itr++)
    {
        if(battle_units[itr].status == bus_Active)
        {
            _cmbt_movepath_cost_map[((battle_units[itr].cgy * COMBAT_GRID_WIDTH) + battle_units[itr].cgx)] = INF;
        }
    }
    for(itr= 0; itr < _vortex_count; itr++)
    {
        _cmbt_movepath_cost_map[((_vortexes[itr].cgy * COMBAT_GRID_WIDTH) + _vortexes[itr].cgx)] = INF;
    }
    /* OGBUG  cities don't have anything on that square either */
    if(battlefield->center_square_structure != CS_None)
    {
        _cmbt_movepath_cost_map[COMBAT_STRUCTURE_IDX] = INF;
    }
    /* same as in Move_Battle_Unit() */
    if((battlefield->walled == ST_TRUE) || (battlefield->city_enchantments[FLYING_FORTRESS] > 0))
    {
        Update_Move_Map_City_Area_Restrictions(_active_battle_unit);
    }
/*
    NOTE: nothing else touches _cmbt_movepath_cost_map[]
*/
    has_instant_movement_mode = ST_FALSE;
    instant_movement_mode = Battle_Unit_Instant_Movement_Mode(_active_battle_unit);
    if(
        (instant_movement_mode == 6)
        ||
        (instant_movement_mode == 7)
    )
    {
        has_instant_movement_mode = ST_TRUE;
    }
    moves2 = battle_units[_active_battle_unit].movement_points;
    uu_max_moves2 = Unit_Moves2(battle_units[_active_battle_unit].unit_idx);
    uu_cgx = battle_units[_active_battle_unit].cgx;
    uu_cgy = battle_units[_active_battle_unit].cgy;
    for(itr_y = 0; itr_y < COMBAT_GRID_HEIGHT; itr_y++)
    {
        for(itr_x = 0; itr_x < COMBAT_GRID_WIDTH; itr_x++ )
        {
            g_combat_grid_action_map[itr_y][itr_x] = COMBAT_CELL_NO_ACTION;
        }
    }
    /* tested for in Battle_Unit_Action() */
    if(
        (battlefield->walled == ST_TRUE)
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
                    g_combat_grid_action_map[itr_y][itr_x] = COMBAT_CELL_CITY_WALL;
                }
            }
        }
    }
    /*
        BEGIN:  assign Battle Unit Indices
    */
    for(itr = 0; itr < _combat_total_unit_count; itr++)
    {
        if(
            (battle_units[itr].status == bus_Active)
            &&
            (battle_units[itr].figure_effect != 5)  // ~ Invisible
        )
        {
            g_combat_grid_action_map[battle_units[itr].cgy][battle_units[itr].cgx] = (int8_t)itr;
        }
    }
    /*
        END:  assign Battle Unit Indices
    */
    useable_moves2 = 0;
    if(has_instant_movement_mode == ST_FALSE)
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
    uu_count_of_reachable_cells = 0;
    Combat_Move_Path_Valid(battle_units[_active_battle_unit].cgx, battle_units[_active_battle_unit].cgy, useable_moves2);
    for(itr_y = 0; itr_y < COMBAT_GRID_HEIGHT; itr_y++)
    {
        cgy_offset = (itr_y * COMBAT_GRID_WIDTH);
        for(itr_x = 0; itr_x < COMBAT_GRID_WIDTH; itr_x++)
        {
            if(_cmbt_path_data[cgy_offset + itr_x] == ST_TRUE)  /* combat grid cell is *reachable* */
            {
                g_combat_grid_action_map[itr_y][itr_x] = COMBAT_CELL_REACHABLE;
                uu_count_of_reachable_cells++;
            }
        }
    }
    g_combat_grid_action_map[battle_units[_active_battle_unit].cgy][battle_units[_active_battle_unit].cgx] = (int8_t)_active_battle_unit;
}


// WZD s91p08
/* GEMINI */
int16_t Target_Is_Visible(int16_t battle_unit_idx)
{
    int16_t distance = 0;
    int16_t player_idx = 0;
    uint32_t enchantments = 0;
    int16_t is_visible = 0;
    int16_t other_battle_unit_idx = 0;
    struct s_BATTLE_UNIT * bu_ptr = {0};
    struct s_UNIT * u_ptr = {0};

    /* Get the controller of the target unit */
    bu_ptr = &battle_units[battle_unit_idx];
    player_idx = battle_units[battle_unit_idx].controller_idx;

    /* 
     * Combine enchantments from the active battle unit and the base unit template.
     */
    // OGBUG  ignores invisibility granted by items
    u_ptr = &_UNITS[battle_units[battle_unit_idx].unit_idx];
    enchantments = battle_units[battle_unit_idx].enchantments | u_ptr->enchantments;

    is_visible = ST_TRUE;

    /* Check for Invisibility enchantment or innate Invisibility ability */
    if((enchantments & UE_INVISIBILITY) != 0)
    {
        goto loc_783E8;
    }

    bu_ptr = &battle_units[battle_unit_idx];
    if((bu_ptr->Abilities & UA_INVISIBILITY) == 0)
    {
        /* Not invisible, return 1 */
        goto Done;
    }

loc_783E8:
    /* Unit is invisible; check if it can be detected */
    is_visible = 0;

    /* If owner is the defender, check if attacker has True Sight (sees illusions) */
    if(player_idx == _combat_defender_player)
    {
        if(_attacker_sees_illusions == ST_TRUE)
        {
            is_visible = 1;
        }
    }

    /* If owner is the attacker, check if defender has True Sight (sees illusions) */
    if(player_idx == _combat_attacker_player)
    {
        if(_defender_sees_illusions == ST_TRUE)
        {
            is_visible = 1;
        }
    }

    /* If not seen by True Sight, check for proximity detection (adjacent units) */
    if(is_visible == 0)
    {
        for(other_battle_unit_idx = 0; other_battle_unit_idx < _combat_total_unit_count; other_battle_unit_idx++)
        {
            bu_ptr = &battle_units[other_battle_unit_idx];
            
            /* Only active enemy units can detect the invisible unit */
            if(bu_ptr->status == bus_Active)
            {
                if(bu_ptr->controller_idx != player_idx)
                {
                    distance = Range_To_Battle_Unit(battle_unit_idx, other_battle_unit_idx);
                    
                    /* If an enemy is adjacent (distance < 2), the unit is detected */
                    if(distance < 2)
                    {
                        is_visible = 1;
                        break;
                    }
                }
            }
        }
    }

Done:
    return is_visible;
}


// WZD s91p09
void Add_City_Damage_From_Battle_Units_Within(void)
{
    int16_t battle_unit_idx = 0;
    if(_combat_environ != cnv_Enemy_City)
    {
        return;
    }
    for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
    {
        if(
            (battle_units[battle_unit_idx].status == bus_Active)
            &&
            (battle_units[battle_unit_idx].controller_idx == _combat_attacker_player)
            &&
            (Battle_Unit_Is_Within_City(battle_unit_idx) == ST_TRUE)
        )
        {
            _combat_city_damage += 1;
        }
    }
}


// WZD s91p10
int16_t Battle_Unit_Instant_Movement_Mode(int16_t battle_unit_idx)
{
    if((battle_units[battle_unit_idx].Move_Flags & MV_TELEPORT) != 0)
    {
        return bumm_Teleport;
    }
    else if((battle_units[battle_unit_idx].Move_Flags & MV_MERGING) != 0)
    {
        return bumm_Tunnel;
    }
    else
    {
        return bumm_Ground0;
    }
}



/*
    WIZARDS.EXE  ovr096
*/

// WZD s96p01
/*
Move_Units() |-> Combat()
Lair_Combat() |-> ... |-> Combat()

defender_player_idx
    unit_idx or player_idx
*/
int16_t Combat(int16_t attacker_player_idx, int16_t defender_player_idx, int16_t troop_count, int16_t troops[])
{
    int16_t item_list[18] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t city_destroyed = 0;
    int16_t item_count = 0;
    int16_t garrison_size = 0;
    int16_t combat_attacker_player_idx = 0;
    int16_t attacker_won = 0;
    int16_t defender_idx = 0;
    int16_t itr = 0;
    int16_t combat_defender_player_idx = 0;
    int16_t return_value = 0;  // DNE in Dasm
    city_destroyed = ST_FALSE;
    destroy_combat_city = ST_FALSE;
    _unit_stack_count = 0;
    combat_attacker_player_idx = attacker_player_idx;
    defender_idx = defender_player_idx;
    for(itr = 0; itr < troop_count; itr++)
    {
        _UNITS[troops[itr]].wx = (int8_t)OVL_Action_OriginX;
        _UNITS[troops[itr]].wy = (int8_t)OVL_Action_OriginY;
    }
    if(combat_attacker_player_idx == NEUTRAL_PLAYER_IDX)
    {
        switch(_combat_environ)
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
                if(_CITIES[_combat_environ_idx].owner_idx == NEUTRAL_PLAYER_IDX)
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
    switch(_combat_environ)
    {
        case 0:  // Stack vs. Stack
        {
            _combat_wx = _UNITS[defender_idx].wx;
            _combat_wy = _UNITS[defender_idx].wy;
            _combat_wp = _UNITS[defender_idx].wp;
            combat_defender_player_idx = _UNITS[defender_idx].owner_idx;
        } break;
        case 1:  // Stack vs. City
        {
            _players[NEUTRAL_PLAYER_IDX].banner_id = BNR_Brown;
            _combat_wx = _CITIES[_combat_environ_idx].wx;
            _combat_wy = _CITIES[_combat_environ_idx].wy;
            _combat_wp = _CITIES[_combat_environ_idx].wp;
            garrison_size = 0;
            for(itr = 0; itr < _units; itr++)
            {
                if(
                    (_UNITS[itr].wx == _combat_wx) &&
                    (_UNITS[itr].wy == _combat_wy) &&
                    (_UNITS[itr].wp == _combat_wp) &&
                    (_UNITS[itr].owner_idx == combat_defender_player_idx)
                )
                {
                    garrison_size++;
                }
            }
        } break;
        case 5:  // Stack vs. Lair
        {
            _players[NEUTRAL_PLAYER_IDX].banner_id = BNR_Brown;
            Lair_Make_Guardians(_combat_environ_idx);
            _combat_wx = _LAIRS[_combat_environ_idx].wx;
            _combat_wy = _LAIRS[_combat_environ_idx].wy;
            _combat_wp = _LAIRS[_combat_environ_idx].wp;
        } break;
    }
    if(
        (combat_attacker_player_idx < NEUTRAL_PLAYER_IDX)
        &&
        (combat_defender_player_idx < NEUTRAL_PLAYER_IDX)
        &&
        (
            (_combat_environ == cnv_Enemy_City)   /* City-Siege */
            ||
            (_combat_environ == cnv_Enemy_Stack)  /* Open-Field */
        )
    )
    {
        if(
            (_players[combat_defender_player_idx].Dipl.Dipl_Status[combat_attacker_player_idx] == DIPL_Alliance)
            ||
            (_players[combat_defender_player_idx].Dipl.Dipl_Status[combat_attacker_player_idx] == DIPL_WizardPact)
        )
        {
            if(combat_attacker_player_idx == _human_player_idx)
            {
                stu_strcpy(GUI_NearMsgString, cnst_TreatyAtk_Msg1);  // "You have a treaty with "
                stu_strcat(GUI_NearMsgString, _players[combat_defender_player_idx].name);
                stu_strcat(GUI_NearMsgString, cnst_TreatyAtk_Msg2);  // ".  Do you still wish to attack?"
                if(Confirmation_Box(GUI_NearMsgString) != ST_FALSE)
                {
                    if(
                        (combat_attacker_player_idx < NUM_PLAYERS)
                        &&
                        (combat_defender_player_idx < NUM_PLAYERS)
                    )
                    {
                        /* OGBUG: doesn't set Dipl_Status or call Declare_War() on this path */
                        Change_Relations(-40, combat_attacker_player_idx, combat_defender_player_idx, 5, 0, 0);
                        Break_Treaties(combat_defender_player_idx, combat_attacker_player_idx);
                    }
                }
                else
                {
                    goto No_Combat;
                }
            }
            else
            {
                for(itr = 0; itr < troop_count; itr++)
                {
                    _UNITS[troops[itr]].Status = us_Ready;
                }
                goto No_Combat;
            }

        }
    }
    if(
        (_combat_environ == 1)  /* City-Siege */
        &&
        (garrison_size < 1)
    )
    {
        attacker_won = ST_TRUE;
        item_count = 0;
        Combat_City_Capture(troop_count, &troops[0]);
    }
    else
    {
        if(
            (
                (combat_attacker_player_idx == _human_player_idx)
                ||
                (combat_defender_player_idx == _human_player_idx)
            )
            &&
            (magic_set.strategic_combat_only == ST_FALSE)
        )
        {
            Stop_All_Sounds__STUB();
            attacker_won = Combat_Screen(combat_attacker_player_idx, combat_defender_player_idx, troops, troop_count, _combat_wx, _combat_wy, _combat_wp, &item_count, &item_list[0]);
            // attacker_won = Combat_Screen_TST_001();
            // attacker_won = Combat_Screen_TST_002();
            // attacker_won = Combat_Screen_TST_003();
            // attacker_won = Combat_Screen_TST_004(combat_attacker_player_idx, combat_defender_player_idx, troops, troop_count, _combat_wx, _combat_wy, _combat_wp, &item_count, &item_list[0]);
            Play_Background_Music();
        }
        else
        {
            attacker_won = Strategic_Combat(troops, troop_count, _combat_wx, _combat_wy, _combat_wp, &item_count, &item_list[0]);
            item_count = 0;
        }
    }
// @@StartPostCombat
    if(attacker_won == ST_TRUE)
    {
        for(itr = 0; itr < troop_count; itr++)
        {
            if(_UNITS[troops[itr]].owner_idx == 100)  /* 100 means 'recalled during combat, so don't do post-combat move */
            {
                _UNITS[troops[itr]].owner_idx = (int8_t)combat_attacker_player_idx;
                _UNITS[troops[itr]].Finished = ST_TRUE;
            }
            else
            {
                _UNITS[troops[itr]].wx = (int8_t)_combat_wx;
                _UNITS[troops[itr]].wy = (int8_t)_combat_wy;
                _UNITS[troops[itr]].wp = (int8_t)_combat_wp;
            }
        }
        item_pool_in_process = ST_TRUE;
        m_item_wx = _combat_wx;
        m_item_wy = _combat_wy;
        m_item_wp = _combat_wp;
        Player_Process_Item_Pool(combat_attacker_player_idx, item_count, &item_list[0]);
        item_pool_in_process = ST_FALSE;
        if(
            (_combat_environ == 1)  /* City-Siege */
            &&
            (_CITIES[_combat_environ_idx].owner_idx != combat_attacker_player_idx)
        )
        {
            if(_CITIES[_combat_environ_idx].size != 0)
            {
                if(
                    (combat_attacker_player_idx < NUM_PLAYERS)
                    &&
                    (combat_defender_player_idx < NUM_PLAYERS)
                )
                {
                    Change_Relations(-40, combat_attacker_player_idx, combat_defender_player_idx, 9, _combat_environ_idx, 0);
                }
                if(
                    (combat_attacker_player_idx != NEUTRAL_PLAYER_IDX)
                    ||
                    ((_unit_type_table[_UNITS[troops[0]].type].Abilities & UA_FANTASTIC) == 0)
                )
                {
                    if(
                        (combat_attacker_player_idx < NUM_PLAYERS)
                        &&
                        (combat_defender_player_idx < NUM_PLAYERS)
                    )
                    {
                        Declare_War(combat_attacker_player_idx, combat_defender_player_idx);
                    }
                    if(destroy_combat_city == ST_FALSE)
                    {
                        Change_City_Ownership(_combat_environ_idx, combat_attacker_player_idx);

                        if(combat_attacker_player_idx != _human_player_idx)
                        {
                            _CITIES[_combat_environ_idx].construction = bt_AUTOBUILD;  // -4 gran vizier
                        }
                    }
                }
            }
            else
            {
                if(
                    (combat_attacker_player_idx != NEUTRAL_PLAYER_IDX)
                    ||
                    ((_unit_type_table[_UNITS[troops[0]].type].Abilities & UA_FANTASTIC) == 0)
                )
                {
                    city_destroyed = ST_TRUE;
                }
            }
        }
    }
    else
    {
        /* OGBUG: missing `item_pool_in_process = ST_TRUE;` */
        Player_Process_Item_Pool(combat_defender_player_idx, item_count, &item_list[0]);
        item_pool_in_process = ST_FALSE;
        if(_combat_environ == 1)  /* City-Siege */
        {
            if(_CITIES[_combat_environ_idx].owner_idx == combat_defender_player_idx)
            {
                if(
                    (combat_attacker_player_idx < NUM_PLAYERS)
                    &&
                    (combat_defender_player_idx < NUM_PLAYERS)
                )
                {
                    if(_CITIES[_combat_environ_idx].size != 0)
                    {
                        if(Player_Fortress_City(combat_defender_player_idx) == _combat_environ_idx)
                        {
                            Change_Relations(-60, combat_attacker_player_idx, combat_defender_player_idx, 9, _combat_environ_idx, 0);
                        }
                        else
                        {
                            Change_Relations(-20, combat_attacker_player_idx, combat_defender_player_idx, 9, _combat_environ_idx, 0);
                        }
                    }
                }
            }
            else
            {
                if(_CITIES[_combat_environ_idx].size == 0)
                {
                    city_destroyed = ST_TRUE;
                }
                else
                {
                    if(destroy_combat_city == ST_FALSE)
                    {
                        Change_City_Ownership(_combat_environ_idx, combat_defender_player_idx);
                    }
                }
            }
        }
    }
    Update_Defender_Hostility(combat_attacker_player_idx, combat_defender_player_idx);
    if(city_destroyed == ST_TRUE)
    {
        Change_City_Ownership(_combat_environ_idx, combat_attacker_player_idx);
        Destroy_City(_combat_environ_idx);
        destroy_combat_city = ST_FALSE;
    }
    if(destroy_combat_city == ST_TRUE)
    {
        Change_City_Ownership(_combat_environ_idx, combat_attacker_player_idx);
        Destroy_City(_combat_environ_idx);
        destroy_combat_city = ST_FALSE;
    }
    _combat_environ_idx = ST_UNDEFINED;
    _combat_environ = ST_UNDEFINED;
    o153p24_empty_function();
    Allocate_Reduced_Map();
    if(
        (combat_attacker_player_idx == _human_player_idx)
        ||
        (combat_defender_player_idx == _human_player_idx)
    )
    {
        Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);
        Full_Draw_Main_Screen();
        Copy_On_To_Off_Page();
    }
    return_value = attacker_won;
    goto Done;
No_Combat:
    _combat_environ_idx = ST_UNDEFINED;
    _combat_environ = ST_UNDEFINED;
    return_value = ST_FALSE;
    goto Done;
Done:
    return return_value;
}


// WZD s96p02
// MoO2: DNE
void Lair_Make_Guardians(int16_t lair_idx)
{
    int16_t guard_count = 0;
    int16_t unit_count = 0;
    int16_t itr = 0;
    guard_count = (_LAIRS[lair_idx].guard1_count & 0x0F);
    unit_count = 0;
    for(itr = 0; ((itr < guard_count) && (unit_count < MAX_STACK)); itr++)
    {
        if(Create_Unit(_LAIRS[lair_idx].guard1_unit_type, NEUTRAL_PLAYER_IDX, _LAIRS[lair_idx].wx, _LAIRS[lair_idx].wy, _LAIRS[lair_idx].wp, 2000) == ST_TRUE)
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
        if(Create_Unit(_LAIRS[lair_idx].guard2_unit_type, NEUTRAL_PLAYER_IDX, _LAIRS[lair_idx].wx, _LAIRS[lair_idx].wy, _LAIRS[lair_idx].wp, 2000) == ST_TRUE)
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
    winner = Combat(player_idx, NEUTRAL_PLAYER_IDX, troop_count, &troops[0]);
    return winner;
}


// WZD s96p04
// drake178: sub_7DE08()


// WZD s96p05
void Update_Defender_Hostility(int16_t attacker_player_idx, int16_t defender_player_idx)
{
    _players[defender_player_idx].Hostility[attacker_player_idx] = 2;
    if(_players[defender_player_idx].Dipl.Dipl_Status[attacker_player_idx] >= DIPL_War)
    {
        _players[defender_player_idx].Hostility[attacker_player_idx] = 3;
    }
    if(_players[defender_player_idx].Dipl.Dipl_Status[attacker_player_idx] == DIPL_Alliance)
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
/**
 * @brief Rebuilds the combat enchantment icon and help-entry lists for both sides.
 *
 * This routine scans the global combat_enchantments state for every attacker and defender combat
 * enchantment slot, determines whether each enchantment should be treated as active, and then
 * repopulates the UI-facing attacker and defender enchantment arrays with the matching icon segment
 * and help index metadata. The per-side counts are reset before the scan and incremented as active
 * enchantments are appended.
 *
 * Most combat enchantments are considered active only when their slot value is exactly 1. Counter
 * Magic is handled specially because it stores a strength value rather than a simple boolean flag,
 * so any value greater than 0 is treated as active.
 *
 * @note The function updates the global attacker_enchantment_count and
 *       defender_enchantment_count values as a side effect.
 * @note Even-numbered entries in combat_enchantments are interpreted as attacker-side slots, while
 *       odd-numbered entries are interpreted as defender-side slots.
 */
void Update_Combat_Enchantments_Icon_And_Help(void)
{
    int16_t is_active = 0;
    int16_t itr = 0;
    int16_t idx = 0;
    attacker_enchantment_count = 0;
    defender_enchantment_count = 0;
    for(itr = 0; itr < (NUM_COMBAT_ENCHANTMENTS * 2); itr++)
    {
        is_active = ST_FALSE;
        if(combat_enchantments[itr] == 1)
        {
            is_active = ST_TRUE;
        }
        if(
            (itr == COUNTER_MAGIC_ATTKR)  /* Counter Magic - Attacker */
            ||
            (itr == COUNTER_MAGIC_DFNDR)  /* Counter Magic - Defender */
        )
        {
            if(combat_enchantments[itr] > 0)
            {
                is_active = ST_TRUE;
            }
            else
            {
                is_active = ST_FALSE;
            }
        }
        if(is_active == ST_TRUE)
        {
            // enchantment index
            idx = (itr / 2);  /* { 0, 1, 2, ..., 27, 28, 29}  { 0, 0, 1, ..., 13, 14, 14 } */
            // even/odd - attacker/defender
            if((itr % 2) == 0)
            {
                /* Attacker Enchantment Update */
                _combat_enchantments_attacker[attacker_enchantment_count].icon_seg = combat_enchantment_icon_segs[combat_enchantment_icon_data[idx].icon_idx];
                _combat_enchantments_attacker[attacker_enchantment_count].help_idx = combat_enchantment_icon_data[idx].help_idx;
                attacker_enchantment_count++;
            }
            else
            {
                /* Defender Enchantment Update */
                _combat_enchantments_defender[defender_enchantment_count].icon_seg = combat_enchantment_icon_segs[combat_enchantment_icon_data[idx].icon_idx];
                _combat_enchantments_defender[defender_enchantment_count].help_idx = combat_enchantment_icon_data[idx].help_idx;
                defender_enchantment_count++;
            }
        }
    }
}


// WZD o98p02
int16_t Battle_Unit_Has_Spell_Ability(int16_t battle_unit_idx)
{
    if(battle_units[battle_unit_idx].Attribs_2 & USA_HEALING)
    {
        return ST_TRUE;
    }
    if(battle_units[battle_unit_idx].Attribs_2 & USA_FIREBALL)
    {
        return ST_TRUE;
    }
    if(battle_units[battle_unit_idx].Attribs_2 & USA_DOOMBOLT)
    {
        return ST_TRUE;
    }
    if(battle_units[battle_unit_idx].Attribs_2 & USA_WEB)
    {
        return ST_TRUE;
    }
    if(battle_units[battle_unit_idx].Attribs_1 & (USA_SUMMON_DEMON_1 | USA_SUMMON_DEMON_2))
    {
        return ST_TRUE;
    }
    if(battle_units[battle_unit_idx].Attribs_1 & USA_CASTER_40)
    {
        return ST_TRUE;
    }
    return ST_FALSE;
}


// WZD o98p03
int16_t NIU_Who_Has_More_Leadership(void)
{
    if(_battlefield_leadership[_combat_attacker_player] > _battlefield_leadership[_combat_defender_player])
    {
        return _combat_attacker_player;
    }
    else
    {
        return _combat_defender_player;
    }
}


// WZD o98p04
/*
~100% certain this is at the same level as MoO2's Do_Combat_Turn_()
But, this feels like an added wrapper for that
*/
void Auto_Cast_Spell_And_Do_Combat_Turn(int16_t player_idx)
{
    int16_t winner;
    /* AI players attempt to cast a spell at the start of their turn */
    if(player_idx != _combat_local_player)
    {
        /* Caster_ID for wizards in combat is 20 + player_idx */
        Combat_Cast_Spell((20 + player_idx), _combat_wx, _combat_wy, _combat_wp);
    }
    winner = Check_For_Winner();  /* ¿ because spell cast may resulted in a win/loss ? */
    /* If no winner has been determined yet, proceed with unit movement */
    if(winner == ST_UNDEFINED)
    {
        Update_Combat_Enchantments_Icon_And_Help(); /* ¿ because spell cast may been an enchantment ? */
        Auto_Do_Combat_Turn(player_idx);
    }
}


// WZD o98p05
void Combat_Next_Turn(void)
{
    _scanned_battle_unit = ST_UNDEFINED;
    frame_active_flag = ST_FALSE;
    frame_scanned_flag = ST_FALSE;
    Set_Mouse_List(1, mouse_list_default);
    m_turn_is_local = ST_FALSE;
    if(m_cp_took_turn == ST_FALSE)
    {
        frame_active_flag = ST_FALSE;
        frame_scanned_flag = ST_FALSE;
        Auto_Cast_Spell_And_Do_Combat_Turn(_combat_remote_player);
    }
    if(Check_For_Winner() == ST_UNDEFINED)
    {
        Begin_Combat_Turn();
        if(_combat_defender_player == _combat_remote_player)
        {
            Auto_Cast_Spell_And_Do_Combat_Turn(_combat_defender_player);
            m_cp_took_turn = ST_TRUE;
        }
        m_turn_is_local = ST_TRUE;
        if(_auto_combat_flag == ST_TRUE)
        {
            Auto_Cast_Spell_And_Do_Combat_Turn(_combat_local_player);
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
    int16_t lost_unit_types[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };  /* passed to Build_Flee_Loss_Message() */
    int16_t has_wind_walker = 0;
    int16_t transport_capacity = 0;
    int16_t lost_unit_count = 0;  /* passed to Build_Flee_Loss_Message() */
    int16_t unit_idx = 0;
    int16_t checked_wx = 0;
    int16_t boat_riders = 0;  // DNE in Dasm, reuses checked_wx
    int16_t scan_span = 0;
    int16_t min_wy = 0;
    int16_t min_wx = 0;
    int16_t checked_wy = 0;
    int16_t defender_fleeing_count = 0;
    int16_t attacker_fleeing_count = 0;
    int16_t itr_battle_units = 0;
    int16_t fleeing_player_idx = 0;
    lost_unit_count = 0;
    if(player_idx == _combat_attacker_player)
    {
        fleeing_player_idx = _combat_defender_player;
    }
    else
    {
        fleeing_player_idx = _combat_attacker_player;
    }
    /*
        BEGIN:  Fleeing Battle Unit Counts
    */
    attacker_fleeing_count = 0;
    defender_fleeing_count = 0;
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
                attacker_fleeing_count++;
            }
            else if(battle_units[itr_battle_units].controller_idx == _combat_defender_player)
            {
                defender_fleeing_count++;
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
        (attacker_fleeing_count != 0)
        ||
        (defender_fleeing_count != 0)
    )
    {
        // mark all uninvolved units as fleeing
        /* OGBUG: End_Of_Combat() already marked uninvolved units as dead */
        for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
        {
            if(battle_units[itr_battle_units].status == bus_Uninvolved)
            {
                battle_units[itr_battle_units].status = bus_Fleeing;
            }
        }
        /*
            BEGIN:  
        */
        for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
        {
            if(battle_units[itr_battle_units].status == bus_Fleeing)
            {
                unit_idx = battle_units[itr_battle_units].unit_idx;
                _UNITS[unit_idx].wx = (int8_t)OVL_Action_OriginX;
                _UNITS[unit_idx].wy = (int8_t)OVL_Action_OriginY;
                battle_units[itr_battle_units].status = bus_Active;

            }
        }
        /*
            END:  
        */
        /*
            BEGIN:  Flee Chance
        */
        if(fleeing_player_idx == _combat_attacker_player)
        {
            for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
            {
                if(battle_units[itr_battle_units].status == bus_Fleeing)
                {
                    if(_UNITS[battle_units[itr_battle_units].unit_idx].Rd_Constr_Left > ST_UNDEFINED)
                    {
                        _UNITS[battle_units[itr_battle_units].unit_idx].Status = us_Ready;
                        _UNITS[battle_units[itr_battle_units].unit_idx].Rd_Constr_Left = ST_UNDEFINED;
                    }
                    if(
                        (_difficulty <= god_Easy)
                        &&
                        (fleeing_player_idx == _human_player_idx)
                    )
                    {
                        battle_units[itr_battle_units].status = bus_Active;
                    }
                    else
                    {
                        if(_UNITS[battle_units[itr_battle_units].unit_idx].Hero_Slot == ST_UNDEFINED)
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
                        ((battle_units[itr_battle_units].combat_effects & bue_Black_Sleep) != 0)
                        ||
                        ((battle_units[itr_battle_units].combat_effects & bue_Confusion) != 0)
                        ||
                        (
                            ((battle_units[itr_battle_units].combat_effects & bue_Web) != 0)
                            &&
                            (battle_units[itr_battle_units].Web_HP > 0)
                        )
                    )
                    {
                        battle_units[itr_battle_units].status = bus_Fleeing;
                    }
                }
            }
            /*
                END:  Flee Chance
            */
            /*
                BEGIN:  Ocean Combat
            */
            if(Square_Is_Sailable(_combat_wx, _combat_wy, _combat_wp) == ST_TRUE)
            {
                // calculate the transport capacity of the player's surviving units
                transport_capacity = 0;
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
                        transport_capacity += battle_units[itr_battle_units].carry_capacity;
                    }
                }
                if(transport_capacity == 0)
                {
                    // check if there's a wind walker among the surviving units
                    has_wind_walker = ST_FALSE;
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
                                has_wind_walker = ST_TRUE;
                            }
                        }
                    }
                    if(has_wind_walker != ST_TRUE)
                    {
                        // mark all active units of the losing player as fleeing if they don't have seafaring capability
                        /* OGBUG: ignores Non-Corporeal units */
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
                                    (Unit_Has_WaterTravel(unit_idx) == ST_TRUE)  /* OGBUG:  ¿ should be Unit_Has_Swimming() ? SEE BELOW */
                                    ||
                                    (Unit_Has_AirTravel_Item(unit_idx) == ST_TRUE)
                                    ||
                                    (Unit_Has_WaterTravel_Item(unit_idx) == ST_TRUE)
                                )
                                {
                                    battle_units[itr_battle_units].status = bus_Active;  /* OGBUG:  ¿ this check for filtered status is due to macro usage ? */
                                }
                                else
                                {
                                    battle_units[itr_battle_units].status = bus_Fleeing;
                                }
                            }
                        }
                    }
                }
                else  /* if(transport_capacity == 0) */
                {
                    // check if there's a wind walker among the surviving units
                    has_wind_walker = ST_FALSE;
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
                                has_wind_walker = ST_TRUE;
                            }
                        }
                    }
                    // process transport capacity, marking any active unit that doesn't fit as fleeing again
                    if(has_wind_walker != ST_TRUE)
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
                                        (Unit_Has_Swimming(battle_units[itr_battle_units].unit_idx) != ST_TRUE)  /* OGBUG:  ¿ should be Unit_Has_WaterTravel() ? SEE ABOVE */
                                        &&
                                        (Unit_Has_WindWalking(battle_units[itr_battle_units].unit_idx) != ST_TRUE)  /* OGBUG:  only got here because there were no WindWalkers */
                                        &&
                                        (Unit_Has_AirTravel(battle_units[itr_battle_units].unit_idx) != ST_TRUE)
                                    )
                                    {
                                        boat_riders++;
                                        if(boat_riders > transport_capacity)
                                        {
                                            battle_units[itr_battle_units].status = bus_Fleeing;
                                        }
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
                    (battle_units[itr_battle_units].status == bus_Fleeing)
                    &&
                    (battle_units[itr_battle_units].controller_idx == fleeing_player_idx)
                )
                {
                    lost_unit_types[lost_unit_count] = _UNITS[battle_units[itr_battle_units].unit_idx].type;

                    lost_unit_count++;
                }
            }
            Build_Flee_Loss_Message(lost_unit_count, lost_unit_types);
        }
        else  /* if(fleeing_player_idx == _combat_attacker_player) */
        {
            for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
            {
                if(battle_units[itr_battle_units].status == bus_Fleeing)
                {
                    /* OGBUG: mixed conditions on status and road build cancel */
                    if(fleeing_player_idx != _human_player_idx)  /* ~== NOT Current Player; Attacker || Defender; But, HERE, _combat_defender_player; */
                    {
                        _UNITS[battle_units[itr_battle_units].unit_idx].Status = us_Ready;
                        _UNITS[battle_units[itr_battle_units].unit_idx].Rd_Constr_Left = ST_UNDEFINED;
                    }
                    /* OGBUG:  dead code */
                    if(_UNITS[battle_units[itr_battle_units].unit_idx].Rd_Constr_Left > ST_UNDEFINED)
                    {
                        _UNITS[battle_units[itr_battle_units].unit_idx].Status = us_Ready;
                        _UNITS[battle_units[itr_battle_units].unit_idx].Rd_Constr_Left = ST_UNDEFINED;
                    }
                    if(
                        (_difficulty > god_Easy)
                        ||
                        (fleeing_player_idx != _human_player_idx)
                    )
                    {
                        if(_UNITS[battle_units[itr_battle_units].unit_idx].Hero_Slot == ST_UNDEFINED)
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
                        ((battle_units[itr_battle_units].combat_effects & bue_Black_Sleep) != 0)
                        ||
                        ((battle_units[itr_battle_units].combat_effects & bue_Confusion) != 0)
                        ||
                        (
                            ((battle_units[itr_battle_units].combat_effects & bue_Web) != 0)
                            &&
                            (battle_units[itr_battle_units].Web_HP > 0)
                        )
                    )
                    {
                        battle_units[itr_battle_units].status = bus_Dead;
                    }
                    if(battle_units[itr_battle_units].status == bus_Dead)
                    {
                        lost_unit_types[lost_unit_count] = _UNITS[battle_units[itr_battle_units].unit_idx].type;
                        lost_unit_count++;
                    }
                }
            }
            /*
                BEGIN:  
            */
            /* OGBUG: this clamp should not exist and is bad - changes subsequent range from {-1,0,1} to {0,1,2} */
            min_wy = (_combat_wy - 1);
            if(min_wy < 0)
            {
                min_wy = 0;
            }
            min_wx = (_combat_wx - 1);
            if(min_wx < 0)
            {
                min_wx += WORLD_WIDTH;
            }
            // attempt to flee as many units as possible to the adjacent squares
            /* OGBUG: range checking allows 2-move flight if the origin X or Y is 0 */
            scan_span = 3;
            checked_wy = min_wy;
            while(((min_wy + scan_span) > checked_wy) && (checked_wy < WORLD_HEIGHT))
            {
                itr_battle_units = min_wx;
                while((min_wx + scan_span) > itr_battle_units)
                {
                    if(itr_battle_units < WORLD_WIDTH)
                    {
                        checked_wx = itr_battle_units;
                    }
                    else
                    {
                        checked_wx = (itr_battle_units - WORLD_WIDTH);
                    }

                    if(
                        (checked_wx != _combat_wx)
                        ||
                        (checked_wy != _combat_wy)
                    )
                    {
                        if(
                            (Square_Has_Lair(checked_wx, checked_wy, _combat_wp) == ST_UNDEFINED)
                            &&
                            (Player_City_At_Square(checked_wx, checked_wy, _combat_wp, fleeing_player_idx) == ST_UNDEFINED)
                        )
                        {
                            Process_Retreating_Units(checked_wx, checked_wy, _combat_wp, fleeing_player_idx);
                        }
                    }
                    itr_battle_units++;
                }
                checked_wy++;
            }
            /*
                END:  
            */
            // mark all fleeing units of the losing player as dead, and add them to the list of lost units
            for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
            {
                if(
                    (battle_units[itr_battle_units].status == bus_Fleeing)
                    &&
                    (battle_units[itr_battle_units].controller_idx == fleeing_player_idx)
                )
                {
                    battle_units[itr_battle_units].status = bus_Dead;
                    lost_unit_types[lost_unit_count] = _UNITS[battle_units[itr_battle_units].unit_idx].type;
                    lost_unit_count++;
                }
            }
            Build_Flee_Loss_Message(lost_unit_count, lost_unit_types);
        }  /* else ... if(fleeing_player_idx == _combat_attacker_player) */
        /*
            END:  
        */
    }
    /*
        END:  
    */
}


// WZD o98p07
int16_t Player_City_At_Square(int16_t wx, int16_t wy, int16_t wp, int16_t player_idx)
{
    int16_t city_idx = 0;
    int16_t itr_cities = 0;
    city_idx = ST_UNDEFINED;
    for(itr_cities = 0; ((itr_cities < _cities) && (city_idx == ST_UNDEFINED)); itr_cities++)
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
// ¿ MoO2  Module: COMBFIND  Process_Retreating_Ships_() ?
int16_t Process_Retreating_Units(int16_t wx, int16_t wy, int16_t wp, int16_t player_idx)
{
    int16_t troop_list[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t boat_rider_count = 0;
    int16_t transport_capacity = 0;
    int16_t has_wind_walker = 0;
    int16_t retreat_possible = 0;
    int16_t troop_count = 0;
    int16_t itr_towers = 0;
    int16_t itr_battle_units = 0;
    int16_t itr_troops = 0;
    int16_t unit_idx = 0;
    int16_t return_value = 0;  // DNE in Dasm
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
        has_wind_walker = ST_FALSE;
        for(itr_towers = 0; ((itr_towers < NUM_TOWERS) && (has_wind_walker == ST_FALSE)); itr_towers++)
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
        retreat_possible = ST_FALSE;
        // Ocean Combat
        if(Square_Is_Sailable(wx, wy, wp) == ST_TRUE)
        {
            has_wind_walker = ST_FALSE;
            // check for Wind Walker on source square
            for(itr_battle_units = 0; ((itr_battle_units < _combat_total_unit_count) && (has_wind_walker == ST_FALSE)); itr_battle_units++)
            {
                if(
                    (battle_units[itr_battle_units].status == bus_Fleeing)
                    &&
                    (battle_units[itr_battle_units].controller_idx == player_idx)
                )
                {
                    unit_idx = battle_units[itr_battle_units].unit_idx;
                    if(unit_idx != ST_UNDEFINED)
                    {
                        if(Unit_Has_WindWalking(unit_idx) == ST_TRUE)
                        {
                            retreat_possible = ST_TRUE;
                            has_wind_walker = ST_TRUE;
                        }
                    }
                }
            }
            // check for Wind Walker on destination square
            if(has_wind_walker == ST_FALSE)
            {
                has_wind_walker = ST_FALSE;
                for(itr_troops = 0; ((itr_troops < troop_count) && (has_wind_walker == ST_FALSE)); itr_troops++)
                {
                    unit_idx = troop_list[itr_troops];

                    if(unit_idx != ST_UNDEFINED)
                    {
                        if(Unit_Has_WindWalking(unit_idx) == ST_TRUE)
                        {
                            retreat_possible = ST_TRUE;
                            has_wind_walker = ST_TRUE;
                        }
                    }
                }
            }
            if(retreat_possible == ST_FALSE)
            {
                transport_capacity = 0;
                for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
                {
                    if(
                        (battle_units[itr_battle_units].status == bus_Fleeing)
                        &&
                        (battle_units[itr_battle_units].controller_idx == player_idx)
                    )
                    {
                        unit_idx = battle_units[itr_battle_units].unit_idx;

                        if(_unit_type_table[_UNITS[unit_idx].type].Transport > 0)
                        {
                            transport_capacity += _unit_type_table[_UNITS[unit_idx].type].Transport;
                        }
                    }
                }
                has_wind_walker = ST_FALSE;
                boat_rider_count = 0;
                for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
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
                            boat_rider_count++;
                            if(boat_rider_count <= transport_capacity)
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
            }  /* if(retreat_possible == ST_FALSE) */
        }
        else  /* if(Square_Is_Sailable(wx, wy, wp) == ST_TRUE) */
        {
            // abort if any of the fleeing units has sailing movement
            /* OGBUG: flying and Non-Corporeal ships can move on land */
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
            retreat_possible = ST_TRUE;
        }
        if(retreat_possible == ST_TRUE)
        {
            // move as many fleeing units over to the square as possible, marking them as active if successful
            /* OGBUG: wind walker might not have moved */
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
        _UNITS[unit_idx].wx = (int8_t)wx;
        _UNITS[unit_idx].wy = (int8_t)wy;
        _UNITS[unit_idx].wp = (int8_t)wp;

        return_value = ST_TRUE;
    }

    return return_value;
}


// WZD o98p10
// MoO2  Module:  COMBAT1  Assign_Mouse_Images_()
/*
finger on own unit
winged boot on reachable move
red x on unreachable
crossed swords on reachable attack
wand?
*/
void Assign_Mouse_Images(void)
{
    int16_t ranged_attack_type_group = 0;
    int16_t cgy = 0;
    int16_t screen_y = 0;
    int16_t screen_x = 0;
    int16_t range_y = 0;
    int16_t range_x = 0;
    int16_t scanned_battle_unit_idx = 0;
    int16_t cgx = 0;
    _scanned_battle_unit = ST_UNDEFINED;
    frame_active_flag = ST_FALSE;
    if(_active_battle_unit > ST_UNDEFINED)
    {
        frame_active_flag = ST_TRUE;
        frame_active_cgx = battle_units[_active_battle_unit].cgx;
        frame_active_cgy = battle_units[_active_battle_unit].cgy;
        if(
            (_human_handle_immobile == ST_TRUE)
            &&
            (_auto_combat_flag == ST_FALSE)
            &&
            (battle_units[_active_battle_unit].controller_idx == _combat_local_player)
        )
        {
            frame_active_flag = ST_FALSE;
        }
    }
    _combat_mouse_grid->image_num = crsr_Finger;
    screen_x = (Pointer_X() + 4);
    screen_y = (Pointer_Y() + 4);
    frame_scanned_flag = ST_FALSE;
    if(screen_y < SCREEN_YBOT_CMBT)
    {
        cgx = Get_Combat_Grid_Cell_X(screen_x, screen_y);
        cgy = Get_Combat_Grid_Cell_Y(screen_x, screen_y);
        if(g_combat_grid_action_map[cgy][cgx] == -2)
        {
            _combat_mouse_grid->image_num = crsr_RedCross;  /* Page 92  (PDF Page 97) "Squares that are not valid as targets for an active unit’s actions show a red "X" when the mouse cursor moves over them." */
        }
        else if(g_combat_grid_action_map[cgy][cgx] == -1)
        {
            frame_scanned_flag = ST_TRUE;
            frame_scanned_cgx = cgx;
            frame_scanned_cgy = cgy;
            _combat_mouse_grid->image_num = crsr_WingedBoot;
        }
        else if(g_combat_grid_action_map[cgy][cgx] == 99)
        {
            frame_scanned_flag = ST_TRUE;
            frame_scanned_cgx = cgx;
            frame_scanned_cgy = cgy;
            frame_anim_cycle = ((frame_anim_cycle + 1) % 3);
            scanned_battle_unit_idx = g_combat_grid_action_map[cgy][cgx];
            range_x = abs((cgx - battle_units[_active_battle_unit].cgx));
            range_y = abs((cgy - battle_units[_active_battle_unit].cgy));
            _combat_mouse_grid->image_num = crsr_RedCross;
            if(battle_units[_active_battle_unit].movement_points > 0)
            {
                // ; BUG: ranged units will still make this as a ranged attack?
                if(
                    (range_x <= 1)
                    &&
                    (range_y <= 1)
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
            scanned_battle_unit_idx = g_combat_grid_action_map[cgy][cgx];
            _scanned_battle_unit = scanned_battle_unit_idx;  // ; the combat unit display is based on this
            // Eh? Opposite conditions as above for turning off the active unit highlight frame?
            // Just human mousing around while it's not their turn?
            // ...MoO2 uses net_flag and clock mouse image?
            if(
                (
                    (_human_handle_immobile != ST_TRUE)
                    ||
                    (_auto_combat_flag != ST_FALSE)
                )
                &&
                (battle_units[scanned_battle_unit_idx].controller_idx != _human_player_idx)
            )
            {
                range_x = abs(battle_units[scanned_battle_unit_idx].cgx - battle_units[_active_battle_unit].cgx);
                range_y = abs(battle_units[scanned_battle_unit_idx].cgy - battle_units[_active_battle_unit].cgy);
                _combat_mouse_grid->image_num = crsr_RedCross;
                if(battle_units[_active_battle_unit].movement_points > 0)
                {
                    if(Check_Attack_Melee(_active_battle_unit, scanned_battle_unit_idx) == ST_TRUE)
                    {
                        if(
                            (range_x <= 1)
                            &&
                            (range_y <= 1)
                        )
                        {
                            if(Check_Attack_Melee_City_Wall(_active_battle_unit, scanned_battle_unit_idx) == ST_TRUE)
                            {
                                _combat_mouse_grid->image_num = crsr_Melee;
                            }
                            else
                            {
                                _combat_mouse_grid->image_num = Ranged_Mouse_Image(_active_battle_unit, scanned_battle_unit_idx);
                            }
                        }
                        else
                        {
                            _combat_mouse_grid->image_num = Ranged_Mouse_Image(_active_battle_unit, scanned_battle_unit_idx);
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
/**
 * @brief Selects the tactical combat mouse cursor image for a potential ranged attack.
 *
 * The decision is based on the attacker's ranged attack group and visibility
 * interactions between attacker and defender.
 *
 * Logic summary:
 * - If the attacker does not use a valid ranged group (Boulder, Missile, Magic),
 *   returns @c crsr_RedCross (not a valid ranged shot).
 * - Otherwise starts as @c crsr_Ranged.
 * - If the attacker has @c UE_TRUE_SIGHT, the ranged cursor remains enabled.
 * - If the defender is invisible (enchantment or innate ability), ranged fire is
 *   only allowed when @c _attacker_sees_illusions is true; otherwise it returns
 *   @c crsr_RedCross.
 *
 * @param attacker_idx Index into @c battle_units for the currently active attacker.
 * @param defender_idx Index into @c battle_units for the scanned/target defender.
 *
 * @return Cursor image id to assign to the combat mouse grid:
 *         @c crsr_Ranged when ranged targeting is valid, otherwise
 *         @c crsr_RedCross.
 *
 * @note Reads combat state from global/unit data, including @c battle_units and
 *       @c _attacker_sees_illusions. This function does not mutate game state.
 */
int16_t Ranged_Mouse_Image(int16_t attacker_idx, int16_t defender_idx)
{
    uint32_t defender_enchantments = 0;
    uint32_t attacker_enchantments = 0;
    int16_t ranged_attack_group = 0;
    int16_t image_num = 0;
    image_num = crsr_RedCross;
    ranged_attack_group = (battle_units[attacker_idx].ranged_type / 10);
    if(
        (ranged_attack_group != rag_Boulder)
        &&
        (ranged_attack_group != rag_Missile)
        &&
        (ranged_attack_group != rag_Magic)
    )
    {
        return crsr_RedCross;
    }        
    image_num = crsr_Ranged;
    defender_enchantments = (battle_units[defender_idx].enchantments | battle_units[defender_idx].item_enchantments | _UNITS[battle_units[defender_idx].unit_idx].enchantments);
    attacker_enchantments = (battle_units[attacker_idx].enchantments | battle_units[attacker_idx].item_enchantments | _UNITS[battle_units[attacker_idx].unit_idx].enchantments);
    if((attacker_enchantments & UE_TRUE_SIGHT) != 0)
    {
        image_num = crsr_Ranged;
    }
    else if(
        ((defender_enchantments & UE_INVISIBILITY) != 0)
        ||
        ((battle_units[defender_idx].Abilities & UA_INVISIBILITY) != 0)
    )
    {
        if(_attacker_sees_illusions == ST_TRUE)
        {
            image_num = crsr_Ranged;
        }
        else
        {
            image_num = crsr_RedCross;
        }
    }
    return image_num;
}


// WZD o98p12
void Add_Combat_Enchantment_Fields(void)
{
    int16_t start_x = 0;
    int16_t itr = 0;
    if(_combat_attacker_player == _human_player_idx)
    {
        start_x = 247;
    }
    else
    {
        start_x = 8;
    }
    for(itr = 0; (((attacker_enchantment_first_shown + itr)  < attacker_enchantment_count) && (itr < 4)); itr++)
    {
        attacker_enchantment_fields[itr] = Add_Picture_Field((start_x + (itr * 17)), 179, _combat_enchantments_attacker[(attacker_enchantment_first_shown + itr)].icon_seg, str_empty_string__ovr098, _combat_enchantments_attacker[(attacker_enchantment_first_shown + itr)].help_idx);
    }
    if(_combat_defender_player == _human_player_idx)
    {
        start_x = 247;
    }
    else
    {
        start_x = 8;
    }
    for(itr = 0; (((defender_enchantment_first_shown + itr)  < defender_enchantment_count) && (itr < 4)); itr++)
    {
        defender_enchantment_fields[itr] = Add_Picture_Field((start_x + (itr * 17)), 179, _combat_enchantments_defender[(defender_enchantment_first_shown + itr)].icon_seg, str_empty_string__ovr098, _combat_enchantments_defender[(defender_enchantment_first_shown + itr)].help_idx);
    }
}


// WZD o98p13
/* OGBUG  INCONSISTENT: forester and mountaineer will both shortcut the sailing check and default to ground */
int16_t Battle_Unit_Movement_Mode(int16_t battle_unit_idx)
{
    int16_t mode_idx = 0;
    if((battle_units[battle_unit_idx].Move_Flags & MV_FLYING) != 0)
    {
        mode_idx = bumm_Flight;
    }
    else if((battle_units[battle_unit_idx].Move_Flags & MV_SWIMMING) != 0)
    {
        mode_idx = bumm_Swimming;
    }
    else if((battle_units[battle_unit_idx].Move_Flags & MV_FORESTER) != 0)
    {
        mode_idx = bumm_Ground1;
    }
    else if((battle_units[battle_unit_idx].Move_Flags & MV_MOUNTAINEER) != 0)
    {
        mode_idx = bumm_Ground1;
    }
    else if((battle_units[battle_unit_idx].Move_Flags & MV_SAILING) != 0)
    {
        mode_idx = bumm_Sailing;
    }
    else
    {
        mode_idx = bumm_Ground1;
    }
    return mode_idx;
}


// WZD o98p14
static void Prepare_Battle_Unit_Summons(int16_t player_idx, int16_t unit_idx, int16_t cgx, int16_t cgy)
{
    int16_t itr = 0;
    if(_combat_total_unit_count < MAX_BATTLE_UNIT_SLOT_COUNT)
    {
        Prepare_Battle_Unit(_combat_total_unit_count, player_idx, unit_idx, cgx, cgy);
        _combat_total_unit_count++;
    } 
    else
    {
        for(itr = 0; itr < _combat_total_unit_count; itr++)
        {
            if(
                (battle_units[itr].status > bus_Active)
                &&
                (_UNITS[battle_units[itr].unit_idx].wp == 9)
            )
            {
                Prepare_Battle_Unit(itr, player_idx, unit_idx, cgx, cgy);
                return;
            }
        }
    }
}


// WZD o98p15
void Prepare_Battle_Unit(int16_t battle_unit_idx, int16_t player_idx, int16_t unit_idx, int16_t cgx, int16_t cgy)
{
    int16_t bufpi = 0;
    Load_Battle_Unit(unit_idx, &battle_units[battle_unit_idx]);
    bufpi = Battle_Unit_Pict_Open();
    /* OGBUG  should handle invalid bufpi */
    assert(bufpi >= 0);
    assert(bufpi < MAX_BATTLE_UNIT_COUNT);
    Combat_Figure_Load(_UNITS[unit_idx].type, bufpi);
    battle_units[battle_unit_idx].bufpi = bufpi;
    battle_units[battle_unit_idx].controller_idx = (int8_t)player_idx;
    // ~ Deploy_Battle_Units()
    battle_units[battle_unit_idx].cgx = cgx;
    battle_units[battle_unit_idx].cgy = cgy;
    battle_units[battle_unit_idx].target_cgx = cgx;
    battle_units[battle_unit_idx].target_cgy = cgy;
    /* all exactly as just done in Load_Battle_Unit() */
    battle_units[battle_unit_idx].move_anim_ctr = 0;
    battle_units[battle_unit_idx].outline_magic_realm = 0;
    battle_units[battle_unit_idx].Atk_FigLoss = 0;
    battle_units[battle_unit_idx].mid_move = ST_FALSE;
    battle_units[battle_unit_idx].action = bua_Ready;
    /* ¿ OGBUG  should call Battle_Unit_Special_Stats() ? */
}

// WZD o98p16
// MoO2  Module: COMBINIT  Deploy_Ships_()
/*
    Load_Battle_Unit(); Combat_Figure_Load(); Deploy_Battle_Units();
    inits battle units, load figure picts, sets position & orientation

checks spl_Spell_Of_Return
sets _players[].Cmbt_Skill_Left

*/
int16_t Prepare_All_Battle_Units(int16_t troop_count, int16_t troops[])
{
    int16_t itr = 0;
    int16_t count = 0;
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
            attacker_on_floating_island = ST_TRUE;
            battle_units[_combat_total_unit_count].status = bus_Uninvolved;
        }
        battle_units[_combat_total_unit_count].bufpi = Combat_Figure_Load(_UNITS[troops[itr]].type, itr);
        battle_units[_combat_total_unit_count].controller_idx = (int8_t)_combat_attacker_player;
        if(battle_units[_combat_total_unit_count].controller_idx != _human_player_idx)
        {
            _UNITS[battle_units[_combat_total_unit_count].unit_idx].Status = us_Ready;  // ; 'MoM Demo': NO ORDERS
            _UNITS[battle_units[_combat_total_unit_count].unit_idx].Rd_Constr_Left = ST_UNDEFINED;
        }
        _combat_total_unit_count++;
    }
    if(_combat_attacker_player != _human_player_idx)
    {
        _combat_ai_player = _combat_attacker_player;
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
            if(_UNITS[itr].owner_idx != _human_player_idx)
            {
                _UNITS[itr].Status = us_Ready;
            }
            Load_Battle_Unit(itr, &battle_units[_combat_total_unit_count]);
            if(_UNITS[battle_units[_combat_total_unit_count].unit_idx].type == spell_data_table[spl_Floating_Island].unit_type)
            {
                defender_on_floating_island = ST_TRUE;
                battle_units[_combat_total_unit_count].status = bus_Uninvolved;
            }
            battle_units[_combat_total_unit_count].bufpi = Combat_Figure_Load(_UNITS[itr].type, _combat_total_unit_count);
            battle_units[_combat_total_unit_count].controller_idx = (int8_t)_combat_defender_player; // Why? Load_Battle_Unit() sets controller_idx = _UNITS[].owner_idx
            count++;
            _combat_total_unit_count++;
        }
    }
    if(_combat_defender_player == ST_UNDEFINED)
    {
        _combat_defender_player = MOO_MONSTER_PLAYER_IDX;
    }
    if(_combat_attacker_player == ST_UNDEFINED)
    {
        _combat_attacker_player = MOO_MONSTER_PLAYER_IDX;
    }
    if(_combat_defender_player != _human_player_idx)
    {
        _combat_ai_player = _combat_defender_player;
    }
    if(_combat_ai_player == ST_UNDEFINED)
    {
        _combat_ai_player = MOO_MONSTER_PLAYER_IDX;
    }
    /*
        axis translation - project the role axis (attacker/defender) onto the control axis (local/remote)
            attacker / defender
            current / opponent
    */
    if(_combat_attacker_player == _human_player_idx)
    {
        _combat_local_player = _combat_attacker_player;
        _combat_remote_player = _combat_defender_player;
    }
    else
    {
        _combat_local_player = _combat_defender_player;
        _combat_remote_player = _combat_attacker_player;
    }
    // ~ Prepare_Battle_Unit()
    for(itr = 0; itr < _combat_total_unit_count; itr++)
    {
        battle_units[itr].cgx = 0;
        battle_units[itr].cgy = 0;
        battle_units[itr].target_cgx = 0;
        battle_units[itr].target_cgy = 0;
        battle_units[itr].move_anim_ctr = 0;
        battle_units[itr].outline_magic_realm = 0;
        battle_units[itr].Atk_FigLoss = 0;
        battle_units[itr].mid_move = ST_FALSE;
        battle_units[itr].action = bua_Ready;
        battle_units[itr].gibs = 0;
        battle_units[itr].Unknown_5A = 0;
        battle_units[itr].animate_idle = 0;
        battle_units[itr].Melee_Anim = 0;
        battle_units[itr].figure_effect = 0;
        battle_units[itr].animate_move_as_idle = 0;
        /* City Combat - the only place you can have a land battle with a stack that has boats */
        if(_combat_environ == 1)  /* City-Siege */
        {
            if(
                (_unit_type_table[_UNITS[battle_units[itr].unit_idx].type].Transport > 0)
                &&
                ((battle_units[itr].Move_Flags & MV_SAILING) != 0)
                &&
                ((battle_units[itr].Move_Flags & MV_FLYING) == 0)
                /* OGBUG:  excludes boats with Wraith Form; should `&& ((battle_units[itr].Abilities & UA_NONCORPOREAL) == 0)`; because Wraith Form / Non-Corporial inherently includes MV_SWIMMING */
            )
            {
                battle_units[itr].status = bus_Uninvolved;
            }
        }
    }
    Deploy_Battle_Units(_combat_attacker_player);
    Deploy_Battle_Units(_combat_defender_player);
    if(_players[_combat_attacker_player].casting_spell_idx == spl_Spell_Of_Return)
    {
        _players[_combat_attacker_player].Cmbt_Skill_Left = 0;
    }
    else
    {
        _players[_combat_attacker_player].Cmbt_Skill_Left = _players[_combat_attacker_player].Nominal_Skill;
    }
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
    else  /* (_combat_defender_player == MOO_MONSTER_PLAYER_IDX) */
    {
        _players[_combat_defender_player].Cmbt_Skill_Left = 0;  /* OGBUG  OOB */
    }
    return count;
}


// WZD o98p17
int16_t Battle_Unit_Is_Airborne(int16_t battle_unit_idx)
{
    int16_t result = 0;
    result = ST_FALSE;
    if((battle_units[battle_unit_idx].Move_Flags & MV_FLYING) != 0)
    {
        result = ST_TRUE;
    }
    if((battle_units[battle_unit_idx].combat_effects & bue_Web) != 0)
    {
        result = ST_FALSE;
    }
    if((battle_units[battle_unit_idx].combat_effects & bue_Black_Sleep) != 0)
    {
        result = ST_FALSE;
    }
    return result;
}


// WZD o98p18
int16_t Battle_Unit_Has_Flight(int16_t battle_unit_idx)
{
    uint32_t enchantments = 0;
    int16_t has_flight = 0;  // DNE in Dasm
    has_flight = ST_FALSE;
    enchantments = (battle_units[battle_unit_idx].enchantments | battle_units[battle_unit_idx].item_enchantments | _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments);
    if((enchantments & UE_WIND_WALKING) != 0)
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
    if((battle_units[battle_unit_idx].combat_effects & bue_Web) != 0)
    {
        has_flight = ST_FALSE;
    }
    if((battle_units[battle_unit_idx].combat_effects & bue_Black_Sleep) != 0)
    {
        has_flight = ST_FALSE;
    }
    return has_flight;
}


// WZD o98p19
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
    if(attacker_has_flight == ST_TRUE)
    {
        result = ST_TRUE;
    }
    else if(defender_has_flight == ST_TRUE)
    {
        if(battle_units[attacker_battle_unit_idx].ranged_type > rat_NONE)
        {
            result = ST_TRUE;
        }
        else
        {
            result = ST_FALSE;
        }
    }
    else
    {
        result = ST_TRUE;
    }
    return result;
}


// WZD o98p20
/* OGBUG: should not mark, load, or undo if SFX are disabled, but should instead return the expected - 1 in this case
*/
SAMB_ptr Get_Battle_Unit_Move_Sound_Buffer(int16_t battle_unit_idx, /* HACK */ uint32_t * sound_buffer_size)
{
    SAMB_ptr sound_seg;
    if(magic_set.sound_effects == ST_TRUE)
    {
        Play_Sound(sound_silent_seg, sound_silent_seg_size);
    }
    Mark_Block(World_Data);
    sound_seg = Reload_Battle_Unit_Move_Sound(battle_unit_idx, sound_buffer_size);
    Release_Block(World_Data);
    return sound_seg;
}


// WZD o98p21
void Turn_Off_Auto_Combat(void)
{
    _auto_combat_flag = ST_FALSE;
    Next_Battle_Unit(_human_player_idx);
    Assign_Combat_Grids();
}


// WZD o98p22
void Build_Flee_Loss_Message(int16_t troop_count, int16_t troop_list[])
{
    int16_t unit_type_tally[MAX_STACK][2] = { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } };
    int16_t last_char_idx = 0;
    char buffer[6] = { 0, 0, 0, 0, 0, 0 };
    int16_t tally_count = 0;
    int16_t type_matched = 0;
    int16_t phrases_emitted = 0;  // DNE in Dasm, reuses type_matched
    int16_t itr_tally = 0;
    int16_t itr = 0;
    if(troop_count > 0)
    {
        for(itr = 0; itr < MAX_STACK; itr++)
        {
            unit_type_tally[itr][1] = 0;
        }
        tally_count = 0;
        for(itr = 0; itr < troop_count; itr++)
        {
            type_matched = 0;
            for(itr_tally = 0; itr_tally < tally_count; itr_tally++)
            {
                if(unit_type_tally[itr_tally][0] == troop_list[itr])
                {
                    unit_type_tally[itr_tally][1]++;
                    type_matched = 1;
                }
            }
            if(type_matched == 0)
            {
                unit_type_tally[tally_count][0] = troop_list[itr];
                unit_type_tally[tally_count][1]++;
                tally_count++;
            }
        }
        stu_strcpy(GUI_NearMsgString, str_WhileFleeingYouLost);  // "While fleeing you lost:"
        phrases_emitted = 0;
        for(itr = 0; itr < tally_count; itr++)
        {
            Add_Comma_Or_And(&phrases_emitted, tally_count, GUI_NearMsgString);
            stu_itoa(unit_type_tally[itr][1], buffer, 10);
            stu_strcat(GUI_NearMsgString, buffer);
            stu_strcat(GUI_NearMsgString, str_SPACE__ovr098);
            stu_strcat(GUI_NearMsgString, *_unit_type_table[unit_type_tally[itr][0]].name);
            /* try to make plural names be singular */
            /* OGBUG: inconsistent in its use of plurals with the rest of the game, and broken - "Spearmen", "Swordsmen", "Bowmen", "Hammerhands", "Cavalry", etc. */
            if(unit_type_tally[itr][1] == 1)
            {
                /* don't drops 's' from s-ending singular names - Huntress, Priestess, etc. */
                if(unit_type_tally[itr][0] != ut_Huntress && unit_type_tally[itr][0] != ut_Priestess)
                {
                    last_char_idx = ((int16_t)stu_strlen(GUI_NearMsgString) - 1);
                    if(GUI_NearMsgString[last_char_idx] == 's')
                    {
                        GUI_NearMsgString[last_char_idx] = ST_NULL;
                    }
                }
            }
        }
        stu_strcat(GUI_NearMsgString, str_PERIOD__ovr098);
    }
    else
    {
        stu_strcpy(GUI_NearMsgString, str_empty_string__ovr098);
    }
}



/*
    WIZARDS.EXE  ovr099
*/

// WZD o99p01
// MoO2  Module: CMBTDRW1  Draw_Main_Combat_Screen_()
/*
Combat_Screen_Map_Draw() calls Copy_Back_To_Off() and Combat_Screen_Map_Draw_Entities()
*/
void Combat_Screen_Draw(void)
{
    int16_t enchantment_help_entry_base = 0;
    int16_t Opponent_Type = 0;
    int16_t Can_Split_Name = 0;
    int16_t enchantment_strip_left_x = 0;
    uint8_t colors[6] = { 0, 0, 0, 0, 0, 0 };
    int16_t itr = 0;
    int16_t string_index = 0;
    Set_Page_Off();
    Combat_Grid_Entities();
    Combat_Screen_Map_Draw();  // |-> Copy_Back_To_Off();  // 'combat background' from Combat_Screen_Compose_Background()
    Reset_Window();
    _combat_total_battle_effect_count = Combat_Info_Effects_Count();
    Tactical_Combat_Draw_Buttons();
    if(_combat_attacker_player == _human_player_idx)
    {
        enchantment_strip_left_x = 247;
        enchantment_help_entry_base = 9;
    }
    else
    {
        enchantment_strip_left_x = 8;
        enchantment_help_entry_base = 13;
    }
    _help_entries[ 9].help_idx = ST_UNDEFINED;
    _help_entries[10].help_idx = ST_UNDEFINED;
    _help_entries[11].help_idx = ST_UNDEFINED;
    _help_entries[12].help_idx = ST_UNDEFINED;
    _help_entries[13].help_idx = ST_UNDEFINED;
    _help_entries[14].help_idx = ST_UNDEFINED;
    _help_entries[15].help_idx = ST_UNDEFINED;
    _help_entries[16].help_idx = ST_UNDEFINED;
    for(itr = 0; (((attacker_enchantment_first_shown + itr) < attacker_enchantment_count) && (itr < 4)); itr++)
    {
        FLIC_Draw((enchantment_strip_left_x + (itr * 17)), 179, _combat_enchantments_attacker[(attacker_enchantment_first_shown + itr)].icon_seg);
        _help_entries[(enchantment_help_entry_base + itr)].help_idx = _combat_enchantments_attacker[(attacker_enchantment_first_shown + itr)].help_idx;
    }
    if(_combat_defender_player == _human_player_idx)
    {
        enchantment_strip_left_x = 247;
        enchantment_help_entry_base = 9;
    }
    else
    {
        enchantment_strip_left_x = 8;
        enchantment_help_entry_base = 13;
    }
    for(itr = 0; (((defender_enchantment_first_shown + itr) < defender_enchantment_count) && (itr < 4)); itr++)
    {
        FLIC_Draw((enchantment_strip_left_x + (itr * 17)), 179, _combat_enchantments_defender[(defender_enchantment_first_shown + itr)].icon_seg);
        _help_entries[(enchantment_help_entry_base + itr)].help_idx = _combat_enchantments_defender[(defender_enchantment_first_shown + itr)].help_idx;
    }
    colors[0] = 241;
    Opponent_Type = 0;  /* Player */
    if(_combat_ai_player == NEUTRAL_PLAYER_IDX)
    {
        for(itr = 0; itr < _combat_total_unit_count; itr++)
        {
            if(battle_units[itr].controller_idx == _combat_ai_player)
            {
                if((_unit_type_table[_UNITS[battle_units[itr].unit_idx].type].Abilities & UA_FANTASTIC) != 0)
                {
                    Opponent_Type = 1;  /* Monsters */
                    break;
                }
                else
                {
                    Opponent_Type = 2;  /* Raiders */
                    break;
                }
            }
        }
    }
    if(_combat_environ == 0)  /* Open-Field */
    {
        for(itr = 1; itr < 5; itr++)
        {
            colors[itr] = combat_name_font_colors_by_banner[_players[_combat_ai_player].banner_id][(itr - 1)];
        }
        Set_Font_Colors_15(4, &colors[0]);
        Set_Font_Style_Shadow_Down(4, 15, 0, 0);
        Set_Outline_Color(241);
        if(_combat_ai_player != NEUTRAL_PLAYER_IDX)
        {
            Print_Centered(40, SCREEN_YBOT_CMBT, _players[_combat_ai_player].name);
        }
        else
        {
            if(Opponent_Type == 1)  /* Monsters */
            {
                Print_Centered(40, SCREEN_YBOT_CMBT, str_Monsters__ovr099);
            }
            else  /* Raiders */
            {
                Print_Centered(40, SCREEN_YBOT_CMBT, str_Raiders__ovr099);
            }
        }
    }
    else if(_combat_environ == 1)  /* City-Siege */
    {
        for(itr = 1; itr < 5; itr++)
        {
            colors[itr] = combat_name_font_colors_by_banner[_players[_combat_ai_player].banner_id][(itr - 1)];
        }
        Set_Font_Colors_15(4, &colors[0]);
        Set_Font_Style_Shadow_Down(4, 15, 0, 0);
        Set_Outline_Color(241);
        if(_combat_ai_player != NEUTRAL_PLAYER_IDX)
        {
            Print_Centered(40, SCREEN_YBOT_CMBT, _players[_combat_ai_player].name);
        }
        else
        {
            if(
                (Opponent_Type == 0)  /* Player */
                ||
                (_combat_attacker_player != NEUTRAL_PLAYER_IDX)
            )
            {
                Print_Centered(40, SCREEN_YBOT_CMBT, _combat_city_name);
            }
            else
            {
                if(Opponent_Type == 1)  /* Monsters */
                {
                    Print_Centered(40, SCREEN_YBOT_CMBT, str_Monsters__ovr099);
                }
                else  /* Raiders */
                {
                    Print_Centered(40, SCREEN_YBOT_CMBT, str_Raiders__ovr099);
                }
            }
        }
    }
    else if(_combat_environ == 5)  /* Lair */
    {
        stu_strcpy(GUI_String_1, TBL_EZ_Names[_LAIRS[_combat_environ_idx].type]);
        string_index = 0;
        Can_Split_Name = ST_FALSE;
        for(itr = 0; stu_strlen(GUI_String_1) > itr; itr++)
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
            colors[itr] = combat_name_font_colors_by_banner[_combat_ai_player][(itr - 1)];  /* OGBUG:  should use _players[_combat_ai_player].banner_id, not just _combat_ai_player */
        }
        Set_Font_Colors_15(4, &colors[0]);
        Set_Font_Style_Shadow_Down(4, 15, 0, 0);
        Set_Outline_Color(241);
        Print_Centered(40, 167, &GUI_String_1[string_index]);
    }
    for(itr = 1; itr < 5; itr++)
    {
        colors[itr] = combat_name_font_colors_by_banner[_players[_human_player_idx].banner_id][(itr - 1)];
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
    Cycle_Palette_Color(198, 0, 0, 0, 55, 0, 0, 11);
    if(
        (_human_handle_immobile == ST_TRUE)
        &&
        (_auto_combat_flag == ST_FALSE)
        &&
        (_active_battle_unit != ST_UNDEFINED)
    )
    {
        if(battle_units[_active_battle_unit].controller_idx != _combat_local_player)
        {
            Draw_Active_Unit_Window();
        }
        else
        {
            // ¿ ~== if current turn is human/current player's turn ?
            if(m_turn_is_local == ST_TRUE)
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
    // Eh? _combat_vortex_anim_frame++ > 5 ? _combat_vortex_anim_frame : 0;
    _combat_vortex_anim_frame++;
    if(_combat_vortex_anim_frame > VORTEX_ANIM_FRAME_COUNT)
    {
        _combat_vortex_anim_frame = 0;
    }
    // ¿ Cracks Calls, Summon, ... ?
    cmbt_cell_effect_frame++;
    if(cmbt_cell_effect_frame > (CELL_EFFECT_ANIM_FRAME_COUNT * CELL_EFFECT_ANIM_HOLD))  /* HACK  to slow down the cycle */
    {
        cmbt_cell_effect_frame = 0;
    }
    frame_anim_cycle++;
    if(frame_anim_cycle > 2)
    {
        frame_anim_cycle = 0;
    }
    _combat_mud_anim_phase++;
    if(_combat_mud_anim_phase > 4)
    {
        _combat_mud_anim_phase = 0;
    }
#ifdef STU_DEBUG
    if(DBG_debug_flag)
    {
        Combat_Screen_Draw_Debug_Information();
    }
#endif
}


// WZD o99p02
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
    int16_t hero_slot = 0;
    int16_t start_x = 0;
    int16_t start_y = 0;
    if(_active_battle_unit == ST_UNDEFINED)
    {
        Set_Animation_Frame(magic_vortex_seg, 1);
        Draw_Picture_To_Bitmap(magic_vortex_seg, scratch_bitmap_seg);
        Set_Animation_Frame(magic_vortex_seg, _combat_vortex_anim_frame);
        start_x = 82;
        start_y = 170;
        Get_Bitmap_Actual_Size(scratch_bitmap_seg, &bitm_x, &bitm_y, &bitm_w, &bitm_h);
        start_x += (((32 - bitm_w) / 2) - bitm_x);
        start_y += (((25 - bitm_h) / 2) - bitm_y);
        Draw_Picture_Windowed(start_x, start_y, scratch_bitmap_seg);
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
        // composes figure picture into scratch_bitmap_seg
        Combat_Figure_Compose(battle_units[_active_battle_unit].bufpi, 2, battle_units[_active_battle_unit].controller_idx, battle_units[_active_battle_unit].outline_magic_realm, 0);
        Combat_Figure_Effect(battle_units[_active_battle_unit].figure_effect);
        if((_unit_type_table[_UNITS[battle_units[_active_battle_unit].unit_idx].type].Abilities & UA_INVISIBILITY) != 0)
        {
            Outline_Bitmap_Pixels(scratch_bitmap_seg, 1);
        }
        unit_idx = battle_units[_active_battle_unit].unit_idx;
        unit_type = _UNITS[unit_idx].type;
        start_x = 84;
        start_y = 173;
        Get_Bitmap_Actual_Size(scratch_bitmap_seg, &bitm_x, &bitm_y, &bitm_w, &bitm_h);
        start_x += (((32 - bitm_w) / 2) - bitm_x);
        start_y += (((25 - bitm_h) / 2) - bitm_y);
        FLIC_Set_LoopFrame_1(scratch_bitmap_seg);
        // draws the composed figure picture, Combat_Figure_Compose() put in scratch_bitmap_seg
        Draw_Picture(start_x, start_y, scratch_bitmap_seg);
        Draw_Active_Unit_Stats_And_Icons();
        colors[0] = 227;
        colors[1] = 243;
        Set_Font_Colors_15(0, &colors[0]);
        Set_Outline_Color(227);
        Set_Font_Style_Shadow_Down(0, 15, 0, 0);
        Set_Font_Spacing_Width(1);
        unit_owner_idx = _UNITS[unit_idx].owner_idx;
        hero_slot = _UNITS[unit_idx].Hero_Slot;
        if(hero_slot != ST_UNDEFINED)
        {
            stu_strcpy(GUI_String_1, _players[unit_owner_idx].Heroes[hero_slot].name);
        }
        else
        {
            stu_strcpy(GUI_String_1, *_unit_type_table[unit_type].name);
        }
        Print_Centered(112, 167, GUI_String_1);
        FLIC_Draw(117, 194, unit_hit_bar_seg);
        Draw_Active_Unit_Damage_Bar(_active_battle_unit, 118, 195);
    }
}


// WZD o99p03
// MoO2  Module: COMBAT1  Add_Tactical_Combat_Fields_()
/*
    ¿ no lock auto button ?
*/
void Tactical_Combat_Draw_Buttons(void)
{
    if(_auto_combat_flag == ST_TRUE)
    {
        Set_Animation_Frame(_cmbt_auto_button_seg, 0);
        FLIC_Draw(170, (SCREEN_YBOT_CMBT + 10), _cmbt_auto_button_seg);
        FLIC_Draw(144, SCREEN_YBOT_CMBT, _cmbt_lock_spell_button_seg);
        FLIC_Draw(170, SCREEN_YBOT_CMBT, _cmbt_lock_wait_button_seg);
        FLIC_Draw(170, (SCREEN_YBOT_CMBT + 20), _cmbt_lock_done_button_seg);
        FLIC_Draw(144, (SCREEN_YBOT_CMBT + 10), _cmbt_lock_info_button_seg);
        FLIC_Draw(144, (SCREEN_YBOT_CMBT + 20), _cmbt_lock_flee_button_seg);
    }
    else
    {
        /* DEDU  used three other places? macro? */
        if(
            (battle_units[_active_battle_unit].movement_points > 0)
            &&
            (
                (battle_units[_active_battle_unit].mana >= 3)  /* OGBUG: units need at least 5 to cast anything */
                ||
                (Battle_Unit_Has_Spell_Ability(_active_battle_unit) == ST_TRUE)
                ||
                (battle_units[_active_battle_unit].item_charges > 0)
                ||
                (m_wizard_cast_available == ST_TRUE)
            )
        )
        {
            Set_Animation_Frame(_cmbt_spell_button_seg, 0);
            FLIC_Draw(144, SCREEN_YBOT_CMBT, _cmbt_spell_button_seg);
        }
        else
        {
            FLIC_Draw(144, SCREEN_YBOT_CMBT, _cmbt_lock_spell_button_seg);
        }
        Set_Animation_Frame(_cmbt_wait_button_seg, 0);
        FLIC_Draw(170, SCREEN_YBOT_CMBT, _cmbt_wait_button_seg);
        Set_Animation_Frame(_cmbt_done_button_seg, 0);
        FLIC_Draw(170, (SCREEN_YBOT_CMBT + 20), _cmbt_done_button_seg);
        if(_combat_total_battle_effect_count > 0)
        {
            Set_Animation_Frame(_cmbt_info_button_seg, 0);
            FLIC_Draw(144, (SCREEN_YBOT_CMBT + 10), _cmbt_info_button_seg);
        }
        else
        {
            FLIC_Draw(144, (SCREEN_YBOT_CMBT + 10), _cmbt_lock_info_button_seg);
        }
        Set_Animation_Frame(_cmbt_auto_button_seg, 0);
        FLIC_Draw(170, (SCREEN_YBOT_CMBT + 10), _cmbt_auto_button_seg);
        Set_Animation_Frame(_cmbt_flee_button_seg, 0);
        FLIC_Draw(144, (SCREEN_YBOT_CMBT + 20), _cmbt_flee_button_seg);
    }
}


// WZD o99p04
void Draw_Spell_Information_Window(void)
{
    uint8_t colors[4] = { 0, 0, 0, 0 };
    char Range_Display_String[6] = { 0, 0, 0, 0, 0, 0 };
    char Range_Number_String[6] = { 0, 0, 0, 0, 0, 0 };
    int16_t value = 0;
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
    stu_itoa(value, Range_Number_String, 10);
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
    int16_t battle_unit_movement_mode = 0;
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


    battle_unit_movement_mode = Battle_Unit_Movement_Mode(_active_battle_unit);

    icon_idx = 0;

    if(battle_unit_movement_mode == bumm_Flight)
    {

        icon_idx = 1;

    }
    else if(
        (battle_unit_movement_mode == bumm_Sailing)
        ||
        (battle_unit_movement_mode == bumm_Swimming)
    )
    {

        icon_idx = 2;

    }

    FLIC_Draw(128, 186, combat_movemode_icon_segs[icon_idx]);

}


// WZD o99p06
/*
¿ ~== MainScr_Maps.C  Set_Entities_On_Map_Window() ?

~ Create Entities
    clears entities and defaults order
    *spawns* trees, rocks, figures, projectiles, vortices, structures

¿ "map" vs. "grid" ?

*/
void Combat_Grid_Entities(void)
{
    int16_t combat_curse_entity_idx = 0;
    int16_t unit_figure_maximum = 0;
    int16_t unit_figure_count = 0;
    int16_t itr = 0;
    int16_t itr_figures = 0;
    Clear_Combat_Grid_Entities();
    Spawn_Tree_Entities();
    Spawn_Rock_Entities();
    Update_Sees_Illusions();
    for(itr = 0; itr < _combat_total_unit_count; itr++)
    {
        if(battle_units[itr].status != bus_Active)
        {
            continue;
        }
        unit_figure_count = battle_units[itr].figure_cnt;
        unit_figure_maximum = battle_units[itr].figure_max;
        if(_UNITS[battle_units[itr].unit_idx].type == spell_data_table[SPL_HYDRA].unit_type)
        {
            unit_figure_count = 1;
            unit_figure_maximum = 1;
        }
        battle_units[itr].figure_effect = 0;
        Combat_Unit_Enchantment_Outline_Set(itr);
        Battle_Unit_Set_Animation_Flags(itr);
        combat_curse_entity_idx = Battle_Unit_Curse_Effects(itr);
        Battle_Unit_Set_Invisibility_Effect(itr);
        if(STU_Log_Category_Enabled(LOG_CAT_COMBAT_TEST))
        {
            if(battle_units[itr].mid_move == ST_TRUE)
            {
                LOG_INFO(LOG_CAT_COMBAT_TEST, "DRAW turn=%d unit=%d src=(%d,%d) dst=(%d,%d) ctr=%d cnt=%d mid=%d", (int)_combat_turn, (int)itr, (int)battle_units[itr].cgx, (int)battle_units[itr].cgy, (int)battle_units[itr].target_cgx, (int)battle_units[itr].target_cgy, (int)battle_units[itr].move_anim_ctr, (int)MOVE_ANIM_CNT, (int)battle_units[itr].mid_move);
            }
        }
        for(itr_figures = 0; itr_figures < unit_figure_count; itr_figures++)
        {
            Spawn_Figure_Entity(battle_units[itr].bufpi, battle_units[itr].cgx, battle_units[itr].cgy, battle_units[itr].target_cgx, battle_units[itr].target_cgy, battle_units[itr].move_anim_ctr, itr_figures, unit_figure_maximum, battle_units[itr].controller_idx, battle_units[itr].outline_magic_realm, battle_units[itr].gibs, battle_units[itr].mid_move, battle_units[itr].Atk_FigLoss, 0);
        }
        if(combat_curse_entity_idx != ST_UNDEFINED)
        {
            Spawn_Curse_Entity(battle_units[itr].cgx, battle_units[itr].cgy, battle_units[itr].target_cgx, battle_units[itr].target_cgy, battle_units[itr].move_anim_ctr, combat_curse_entity_idx);
        }
    }
    Spawn_Missile_Entities();  // MoO! Missiles
    Spawn_Vortex_Entities();
    Spawn_Structure_Entities();
    Cycle_Unit_Enchantment_Animation();
}


// WZD o99p07
/*
~== Unit_Window_Draw_Unit_Attributes()
*/
void Draw_Active_Unit_Damage_Bar(int16_t battle_unit_idx, int16_t x, int16_t y)
{
    int16_t bar_color = 0;
    int16_t damage_hits = 0;
    int16_t current_hits = 0;
    int16_t max_hits = 0;
    int16_t bar_length = 0;
    max_hits = (battle_units[battle_unit_idx].hits * battle_units[battle_unit_idx].figure_max);
    damage_hits = ((battle_units[battle_unit_idx].figure_max - battle_units[battle_unit_idx].figure_cnt) * battle_units[battle_unit_idx].hits);
    if(battle_units[battle_unit_idx].front_figure_damage > 0)
    {
        damage_hits += battle_units[battle_unit_idx].front_figure_damage;
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
        Line(x, y, (x + bar_length), y, (uint8_t)bar_color);
    }
    /*
        END: Draw Unit Damage Bar
    */
}



/*
    WIZARDS.EXE  ovr103
*/

// WZD s103p01
/* OGBUG:  INCONSISTENT: excludes Evil Omens and Spell Wards */
void Combat_Information_Window(void)
{
    int16_t full_screen_esc_field = 0;
    int16_t input_field_idx = 0;
    int16_t leave_screen = 0;
    int16_t itr = 0;
    int16_t window_height = 0;
    g_center_square_structure = battlefield->center_square_structure;
    _combat_info_cloud_of_shadow = battlefield->city_enchantments[CLOUD_OF_SHADOW];
    _combat_info_heavenly_light = battlefield->city_enchantments[HEAVENLY_LIGHT];
    Clear_Fields();
    Deactivate_Auto_Function();
    Assign_Auto_Function(Combat_Information_Window_Draw, 1);
    Set_Mouse_List(1, mouse_list_default);
    Copy_On_To_Off_Page();
    Copy_Off_To_Back();
    GUI_String_1 = (char *)Near_Allocate_First(80);
    for(itr = 0; itr < 14; itr++)
    {
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
        window_height = (((_combat_info_item_count / 2) * 19) + 9);
    }
    else
    {
        window_height = 4;
    }
    if(_combat_per_side_effect_rows > 0)
    {
        window_height = (_combat_info_wnd_start_y + (_combat_per_side_effect_rows * 20) + window_height + 21);
    }
    else
    {
        window_height = (_combat_info_wnd_start_y + (_combat_per_side_effect_rows * 20) + window_height);
    }
    _combat_info_wnd_start_y = (SCREEN_YMID - (window_height / 2));
    window_height = (_combat_info_wnd_start_y + window_height);  /* OGBUG:  dead store - never read after here; re-purposes the local as the window's bottom edge */
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
        full_screen_esc_field = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, cnst_HOTKEY_Esc13[0], ST_UNDEFINED);
        input_field_idx = Get_Input();
        if(input_field_idx == full_screen_esc_field)
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
    _page_flip_effect = pfe_Dissolve;
    Deactivate_Help_List();
}


// WZD s103p02
void Combat_Information_Window_Draw(void)
{
    int16_t in_per_side_section = 0;
    uint8_t colors[4] = { 0, 0, 0, 0 };
    int16_t section_top_offset = 0;
    int16_t cell_y = 0;
    int16_t cell_x = 0;
    int16_t cell_idx = 0;
    int16_t top_y2 = 0;
    int16_t mid_start_y = 0;
    int16_t mid_y2 = 0;
    int16_t bot_y1 = 0;
    int16_t bot_start_y = 0;
    Set_Page_Off();
    if(_combat_info_item_count > 0)
    {
        top_y2 = (9 + ((_combat_info_item_count / 2) * 19));
        top_y2 += _combat_info_wnd_start_y;
        Set_Window(0, 0, SCREEN_XMAX, top_y2);
    }
    else
    {
        top_y2 = 4;
        Set_Window(0, 0, SCREEN_XMAX, top_y2);
    }
    Clipped_Draw(_combat_info_wnd_start_x, _combat_info_wnd_start_y, _combat_info_wnd_top_seg);
    Reset_Window();
    if(_combat_per_side_effect_rows > 0)
    {
        mid_y2 = (_combat_per_side_effect_rows * 20) + top_y2 + 21;
    }
    else
    {
        mid_y2 = ((_combat_per_side_effect_rows * 20) + top_y2);
    }
    Set_Window(0, 0, SCREEN_XMAX, mid_y2);
    mid_start_y = top_y2;
    Clipped_Draw(_combat_info_wnd_start_x, mid_start_y, _combat_info_wnd_mid_seg);
    if(_combat_per_side_effect_rows > 0)
    {
        Reset_Window();
    }
    else
    {
        bot_y1 = mid_y2;
        Set_Window(0, bot_y1, SCREEN_XMAX, SCREEN_YMAX);
        bot_y1 -= 2;
    }
    bot_start_y = bot_y1;
    Clipped_Draw(_combat_info_wnd_start_x, bot_start_y, _combat_info_wnd_bot_seg);
    Reset_Window();
    colors[0] = 250;
    colors[1] = 177;
    Set_Outline_Color(254);
    Set_Font_Colors_15(2, &colors[0]);
    Set_Font_Style_Shadow_Down(2, 15, 0, 0);
    Set_Font_Spacing_Width(2);
    if(_combat_per_side_effect_rows > 0)
    {
        if(_combat_info_item_count > 0)
        {
            section_top_offset = (15 + ((_combat_info_item_count / 2) * 19));
        }
        else
        {
            section_top_offset = 11;
        }
        if(_combat_ai_player < NEUTRAL_PLAYER_IDX)
        {
            stu_strcpy(GUI_String_1, _players[_combat_ai_player].name);
            stu_strcat(GUI_String_1, cnst_SpaceSpells);
            Print_Centered((_combat_info_wnd_start_x + 54), (_combat_info_wnd_start_y + section_top_offset), GUI_String_1);
        }
        stu_strcpy(GUI_String_1, _players[_combat_local_player].name);
        stu_strcat(GUI_String_1, cnst_SpaceSpells);
        Print_Centered((_combat_info_wnd_start_x + 170), (_combat_info_wnd_start_y + section_top_offset), GUI_String_1);
    }
    if(_combat_info_item_count > 0)
    {
        section_top_offset = 10;
    }
    else
    {
        section_top_offset = 26;
    }
    in_per_side_section = 0;
    for(cell_idx = 0; ((_combat_per_side_effect_rows / 2) + _combat_info_item_count) > cell_idx; cell_idx++)
    {
        if(
            (cell_idx >= _combat_info_item_count)
            &&
            (_combat_info_item_count > 0)
            &&
            (in_per_side_section == 0)
        )
        {
            in_per_side_section = 1;
            section_top_offset = 31;
        }
        cell_x = (_combat_info_wnd_start_x + ((cell_idx % 2) * 112) + 11);
        cell_y = _combat_info_wnd_start_y + section_top_offset + ((cell_idx / 2) * 19);
        FLIC_Draw((cell_x - 1), (cell_y - 1), _combat_info_wnd_box_seg);
        if(_combat_info_effects[cell_idx]->icon_seg != (SAMB_ptr)ST_UNDEFINED)
        {
            FLIC_Draw(cell_x, cell_y, _combat_info_effects[cell_idx]->icon_seg);
            Print((cell_x + 20), (cell_y + 5), _combat_info_effects[cell_idx]->Name);
            _help_entries[cell_idx].help_idx = _combat_info_effects[cell_idx]->help_idx;
            _help_entries[cell_idx].x1 = cell_x;
            _help_entries[cell_idx].y1 = cell_y;
            _help_entries[cell_idx].x2 = (cell_x + 100);
            _help_entries[cell_idx].y2 = (cell_y  +  15);
        }
    }
}


// WZD s103p03
/* OGBUG:  INCONSISTENT: excludes Evil Omens and Spell Wards */
void Combat_Info_Effects(void)
{
    int16_t computer_player_battle_effect_count = 0;
    int16_t itr_combatants = 0;
    int16_t info_common_count = 0;
    int16_t player_idx = 0;
    Combat_Info_Effects_Base();
    _combat_total_battle_effect_count = 0;
    info_common_count = _combat_info_item_count;  // just set in Combat_Info_Effects_Base()
    computer_player_battle_effect_count = 0;
    player_idx = _combat_ai_player;
    for(itr_combatants = 0; itr_combatants < 2; itr_combatants++)
    {
        if(player_idx < NEUTRAL_PLAYER_IDX)
        {
            if(_players[player_idx].Globals[CRUSADE] > 0)
            {
                _combat_info_effects[info_common_count]->icon_seg = _combat_info_effect_icon_segs[0];
                _combat_info_effects[info_common_count]->help_idx = HLP_CRUSADE;
                stu_strcpy(_combat_info_effects[info_common_count]->Name, cnst_Crusade);
                _combat_total_battle_effect_count++;
                info_common_count += 2;
            }
            if(_players[player_idx].Globals[HOLY_ARMS] > 0)
            {
                _combat_info_effects[info_common_count]->icon_seg = _combat_info_effect_icon_segs[1];
                _combat_info_effects[info_common_count]->help_idx = HLP_HOLY_ARMS;
                stu_strcpy(_combat_info_effects[info_common_count]->Name, cnst_HolyArms);
                _combat_total_battle_effect_count++;
                info_common_count += 2;
            }
            if(_players[player_idx].Globals[CHARM_OF_LIFE] > 0)
            {
                _combat_info_effects[info_common_count]->icon_seg = _combat_info_effect_icon_segs[3];
                _combat_info_effects[info_common_count]->help_idx = HLP_CHARM_OF_LIFE;
                stu_strcpy(_combat_info_effects[info_common_count]->Name, cnst_CharmofLife);
                _combat_total_battle_effect_count++;
                info_common_count += 2;
            }
            if(_players[player_idx].Globals[ZOMBIE_MASTERY] > 0)
            {
                _combat_info_effects[info_common_count]->icon_seg = _combat_info_effect_icon_segs[9];
                _combat_info_effects[info_common_count]->help_idx = HLP_ZOMBIE_MASTERY;
                stu_strcpy(_combat_info_effects[info_common_count]->Name, cnst_ZombieMastery);
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
        _combat_per_side_effect_rows = (_combat_total_battle_effect_count - computer_player_battle_effect_count);
    }
    else
    {
        _combat_per_side_effect_rows = computer_player_battle_effect_count;
    }
}


// WZD s103p04
/*
handles
node dispel - Sorcery, Chaos, Nature
node auta - Sorcery, Nature, Chaos
Cloud of Shadows, Heavenly Light
Chaos Surge, Eternal Night
*/
/* OGBUG:  INCONSISTENT: ignores Evil Omens */
void Combat_Info_Effects_Base(void)
{
    int16_t itr = 0;
    int16_t idx = 0;
    for(itr = 0; itr < 14; itr++)
    {
        _combat_info_effects[itr]->icon_seg = (SAMB_ptr)ST_UNDEFINED;
    }
    idx = 0;
    if(g_center_square_structure == CS_SorceryNode)
    {
        _combat_info_effects[idx]->icon_seg = _combat_info_effect_icon_segs[12];
        _combat_info_effects[idx]->help_idx = HLP_DISPELS_NON_SORCERY;
        stu_strcpy(_combat_info_effects[idx]->Name, cnst_SorcNodeDispel);
        idx++;
    }
    else if(g_center_square_structure == CS_ChaosNode)
    {
        _combat_info_effects[idx]->icon_seg = _combat_info_effect_icon_segs[5];
        _combat_info_effects[idx]->help_idx = HLP_DISPELS_NON_CHAOS;
        stu_strcpy(_combat_info_effects[idx]->Name, cnst_ChaosNodeDispel);
        idx++;
    }
    else if(g_center_square_structure == CS_NatureNode)
    {
        _combat_info_effects[idx]->icon_seg = _combat_info_effect_icon_segs[10];
        _combat_info_effects[idx]->help_idx = HLP_DISPELS_NON_NATURE;
        stu_strcpy(_combat_info_effects[idx]->Name, cnst_NatNodeDispel);
        idx++;
    }
    if(_combat_node_type == cnt_Sorcery)
    {
        _combat_info_effects[idx]->icon_seg = _combat_info_effect_icon_segs[13];
        _combat_info_effects[idx]->help_idx = HLP_SORCERY_NODE_AURA;
        stu_strcpy(_combat_info_effects[idx]->Name, cnst_SorceryAura);
        idx++;
    }
    else if(_combat_node_type == cnt_Nature)
    {
        _combat_info_effects[idx]->icon_seg = _combat_info_effect_icon_segs[11];
        _combat_info_effects[idx]->help_idx = HLP_NATURE_NODE_AURA;
        stu_strcpy(_combat_info_effects[idx]->Name, cnst_NatureAura);
        idx++;
    }
    else if(_combat_node_type == cnt_Chaos)
    {
        _combat_info_effects[idx]->icon_seg = _combat_info_effect_icon_segs[6];
        _combat_info_effects[idx]->help_idx = HLP_CHAOS_NODE_AURA;
        stu_strcpy(_combat_info_effects[idx]->Name, cnst_ChaosAura);
        idx++;
    }
    if(_combat_environ == 1)  /* City-Siege */
    {
        if(_combat_info_cloud_of_shadow > 0)
        {
            _combat_info_effects[idx]->icon_seg = _combat_info_effect_icon_segs[8];
            _combat_info_effects[idx]->help_idx = HLP_CLOUD_OF_DARKNESS;
            stu_strcpy(_combat_info_effects[idx]->Name, cnst_CloudOfDarkness);
            idx++;
        }
        if(_combat_info_heavenly_light > 0)
        {
            _combat_info_effects[idx]->icon_seg = _combat_info_effect_icon_segs[2];
            _combat_info_effects[idx]->help_idx = HLP_HOLY_LIGHT;
            stu_strcpy(_combat_info_effects[idx]->Name, cnst_HolyLight);
            idx++;
        }
    }
    for(itr = 0; itr < NUM_PLAYERS; itr++)
    {
        if(_players[itr].Globals[CHAOS_SURGE] > 0)
        {
            _combat_info_effects[idx]->icon_seg = _combat_info_effect_icon_segs[4];
            _combat_info_effects[idx]->help_idx = HLP_CHAOS_SURGE;
            stu_strcpy(_combat_info_effects[idx]->Name, cnst_ChaosSurge);
            idx++;
            break;
        }
    }
    for(itr = 0; itr < NUM_PLAYERS; itr++)
    {
        if(_players[itr].Globals[ETERNAL_NIGHT] > 0)
        {
            _combat_info_effects[idx]->icon_seg = _combat_info_effect_icon_segs[7];
            _combat_info_effects[idx]->help_idx = HLP_ETERNAL_NIGHT;
            stu_strcpy(_combat_info_effects[idx]->Name, cnst_EternalNight);
            idx++;
            break;
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
/* OGBUG: missing Evil Omens */
int16_t Combat_Info_Effects_Count(void)
{
    int16_t battle_effects_count = 0;
    int16_t player_idx = 0;
    int16_t itr_combatants = 0;
    int16_t itr_players = 0;
    battle_effects_count = 0;
    player_idx = _combat_ai_player;
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
        }
        player_idx = _human_player_idx;
    }
    if(
        (battlefield->center_square_structure == CS_SorceryNode)
        ||
        (battlefield->center_square_structure == CS_ChaosNode)
        ||
        (battlefield->center_square_structure == CS_NatureNode)
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
    if(_combat_environ == 1)  /* City-Siege */
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
            break;
        }
    }
    for(itr_players = 0; itr_players < NUM_PLAYERS; itr_players++)
    {
        if(_players[itr_players].Globals[ETERNAL_NIGHT] > 0)
        {
            battle_effects_count++;
            break;
        }
    }
    return battle_effects_count;
}


// WZD s103p06
// ¿ MoO2  Point_Is_In_Some_Nebula_() |-> Point_Is_In_Nebula_N_() ?
void Combat_Node_Type(void)
{
    int16_t itr_auras = 0;
    int16_t node_type = 0;
    int16_t itr_nodes = 0;
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


// WZD s103p07
/*
~ Combat_Figure_Compose()
¿ vs. Combat_Screen_Map_Compose_Figures() ?
¿ vs. Combat_Grid_Entities() ?
*/
void Battle_Unit_Compose_Bitmap(int16_t battle_unit_idx)
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
    int16_t figure_effect = 0;
    int16_t enchantment_magic_realm = 0;
    int16_t player_idx = 0;
    int16_t fig_y = 0;
    int16_t fig_x = 0;
    int16_t fig_max = 0;
    int16_t fig_cnt = 0;
    int16_t figure_set_idx = 0;
    battle_unit_scratch_seg = Allocate_Next_Block(World_Data, 124);  /* an inactive unit still leaves an allocated block for the caller to release */
    if(battle_units[battle_unit_idx].status != bus_Active)
    {
        return;
    }
    Create_Picture(47, 42, battle_unit_scratch_seg);
    // ~ Combat_Grid_Entities()
    fig_cnt = battle_units[battle_unit_idx].figure_cnt;
    fig_max = battle_units[battle_unit_idx].figure_max;
    if(_UNITS[battle_units[battle_unit_idx].unit_idx].type == spell_data_table[spl_Hydra].unit_type)
    {
        fig_cnt = 1;
        fig_max = 1;
    }
    Combat_Unit_Enchantment_Outline_Set(battle_unit_idx);
    Battle_Unit_Set_Animation_Flags(battle_unit_idx);
    Battle_Unit_Set_Invisibility_Effect(battle_unit_idx);
    // ~ Combat_Screen_Map_Compose_Figures()
    bufpi = battle_units[battle_unit_idx].bufpi;
    player_idx = battle_units[battle_unit_idx].controller_idx;
    enchantment_magic_realm = battle_units[battle_unit_idx].outline_magic_realm;
    figure_effect = battle_units[battle_unit_idx].figure_effect;
    if(battle_units[battle_unit_idx].animate_idle == ST_TRUE)
    {
        frame_num = combat_idle_anim_frame;
    }
    else if(battle_units[battle_unit_idx].mid_move == ST_TRUE)
    {
        if(battle_units[battle_unit_idx].animate_move_as_idle == ST_TRUE)
        {
            frame_num = combat_idle_anim_frame;
        } 
        else
        {
            frame_num = combat_walk_anim_frame;
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
    /* OGBUG: figure_set_idx set by BATTLE_UNIT_FACING_DRECTION is deprecated */
    figure_set_idx = 7;  // look left/west?
    Set_Animation_Frame(figure_pict_set_seg[figure_set_idx], frame_num);
    Draw_Picture_To_Bitmap(figure_pict_set_seg[figure_set_idx], scratch_bitmap_seg);
    Combat_Figure_Banner_Color(player_idx);
    Combat_Unit_Enchantment_Outline_Draw(enchantment_magic_realm);
    Combat_Figure_Effect(figure_effect);
    // ~ Combat_Screen_Map_Draw_Entities()
    for(cur_fig = 0; cur_fig < fig_cnt; cur_fig++)
    {
        Battle_Unit_Figure_Position(fig_max, cur_fig, &fig_x, &fig_y);
        Clipped_Copy_Bitmap(fig_x, fig_y - 4, battle_unit_scratch_seg, scratch_bitmap_seg);
    }
}


// WZD s103p08
/*
sets _active_battle_unit, via Next_Battle_Unit_Nearest_Available(player_idx)
¿ ~== WIZ_NextIdleStack() ?
*/
void Next_Battle_Unit(int16_t player_idx)
{
    int16_t all_done_none_available = 0;
    int16_t itr_battle_units = 0;
    int16_t done = 0;
    all_done_none_available = ST_FALSE;
    // ; BUG? why is this in a loop?
    done = ST_FALSE;
    while(done == ST_FALSE)
    {
        // ...status active, player controller, movement points
        all_done_none_available = Next_Battle_Unit_Nearest_Available(player_idx);
        if(all_done_none_available == ST_TRUE)
        {
            _human_out_of_moves = ST_TRUE;   // all out of movement points
        }
        else
        {
            _human_handle_immobile = ST_FALSE;  // have moves, so ... don't trigger ???
        }
        done = ST_TRUE;
    }
    // ; isn't this an overland variable?
    if(all_units_moved == ST_FALSE)
    {
        // ; completely redundant, the variable is already zero if these conditions are met
        if(
            (battle_units[_active_battle_unit].movement_points > 0)
            &&
            (battle_units[_active_battle_unit].controller_idx == _combat_local_player)
        )
        {
            _human_handle_immobile = ST_FALSE;
        }
    }
    if(battle_units[_active_battle_unit].controller_idx != _combat_local_player)
    {
        for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
        {
            if(battle_units[itr_battle_units].controller_idx == _combat_attacker_player)
            {
                _active_battle_unit = itr_battle_units;
                break;
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
    returns {F,T} any active, controlled, moveable unit
    sets _active_battle_unit, via Switch_Active_Battle_Unit()

¿ ~== UNITSTK.C  int16_t Next_Unit_Nearest_Available(int16_t player_idx, int16_t * map_plane) ?

*/
/* [Pure-Function] */
int16_t Next_Battle_Unit_Nearest_Available(int16_t player_idx)
{
    int16_t Selected_Unit_Y = 0;
    int16_t Selected_Unit_X = 0;
    int16_t Processed_Units = 0;
    int16_t uu_flag = 0;
    int16_t Unit_Count = 0; // active status, current player
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
/**
 * @brief Executes the movement phase for a confused battle unit.
 *
 * This routine attempts to send the specified unit to a random unoccupied combat square, modeling
 * the erratic behavior caused by confusion. It performs up to 600 random square probes within the
 * combat grid and selects the first location whose entry in g_combat_grid_action_map indicates that no unit
 * currently occupies the square. If such a square is found, the unit is moved there through
 * Move_Battle_Unit().
 *
 * Regardless of whether a valid destination is found, the unit's turn is then ended by forcing its
 * action to bua_Finished and clearing its remaining movement points.
 *
 * @param battle_unit_idx The battle-unit index of the confused unit to move.
 *
 * @note The function does not guarantee movement; if no unoccupied square is found during the
 *       random search, the unit simply loses the rest of its turn in place.
 * @note Occupancy is determined from g_combat_grid_action_map, where a value of -1 marks an empty square.
 */
void Move_Confused(int16_t battle_unit_idx)
{
    int16_t target_found = 0;
    int16_t random_x = 0;
    int16_t random_y = 0;
    int16_t target_y = 0;
    int16_t target_x = 0;
    int16_t i = 0;
    target_found = ST_FALSE;
    for(i = 0; ((i < 600) && (target_found == ST_FALSE)); i++)
    {
        random_x = (Random(COMBAT_GRID_WIDTH) - 1);
        random_y = (Random(COMBAT_GRID_HEIGHT) - 1);
        if(g_combat_grid_action_map[random_y][random_x] == -1)  /* unoccupied */
        {
            target_x = random_x;
            target_y = random_y;
            target_found = ST_TRUE;
        }
    }
    if(target_found == ST_TRUE)
    {
        Move_Battle_Unit(battle_unit_idx, target_x, target_y);
    }
    battle_units[battle_unit_idx].action = bua_Finished;
    battle_units[battle_unit_idx].movement_points = 0;
}


// WZD s103p12
/*
~== Strategic_Combat_Allocate()
*/
void Combat_Screen_Load_Resources(void)
{
    int16_t itr = 0;
    battle_units = (struct s_BATTLE_UNIT *)Allocate_Next_Block(_screen_seg, ((((4 * MAX_STACK) * sizeof(struct s_BATTLE_UNIT)) / 16) + 1));
    _battlefield_holybonus  = (int16_t *)Allocate_Next_Block(_screen_seg, 6);
    _battlefield_resistall  = (int16_t *)Allocate_Next_Block(_screen_seg, 6);
    _battlefield_leadership = (int16_t *)Allocate_Next_Block(_screen_seg, 6);
    // COMPIX.LBX, 001  "CMBTBUTT"   "spell button"
    // COMPIX.LBX, 002  "CMBTBUTT"   "wait button"
    // COMPIX.LBX, 003  "CMBTBUTT"   "done button"
    // COMPIX.LBX, 004  "CMBTBUTT"   "auto button"
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
    // ¿ DEDU uses 30, like for battlefield->city_enchantments ?
    for(itr = 0; itr < 30; itr++)
    {
        combat_enchantments[itr] = ST_FALSE;
    }
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
        combat_scan_icon_segs[itr] = LBX_Reload_Next(compix_lbx_file__ovr103, (61 + itr), _screen_seg);

    }
    // COMPIX.LBX, 018  "HITBAR"     "unit hit bar"
    unit_hit_bar_seg = LBX_Reload_Next(compix_lbx_file__ovr103, 18, _screen_seg);
    Mark_Block(_screen_seg);
    for(itr = 0; itr < MAX_BATTLE_UNIT_COUNT; itr++)
    {
        battle_unit_picts_seg[itr] = Allocate_Next_Block(_screen_seg, 55);  // 55 PR  880 B
    }
    Release_Block(_screen_seg);
    Allocate_Combat_Near_Buffers();
}


// WZD s103p13
void Allocate_Combat_Near_Buffers(void)
{
    int16_t itr = 0;
    _cmbt_movepath_cost_map = (uint8_t *)Near_Allocate_First(504);
    _cmbt_mvpth_c = (uint8_t *)Near_Allocate_Next(504);
    _cmbt_path_data = (int16_t *)Near_Allocate_Next(1008);  // 504 2-byte values
    _cmbt_mvpth_x = (int16_t *)Near_Allocate_Next(60);
    _cmbt_mvpth_y = (int16_t *)Near_Allocate_Next(60);
    _combat_mouse_grid = (struct s_mouse_list *)Near_Allocate_Next(12);
    for(itr = 0; itr < COMBAT_GRID_HEIGHT; itr++)
    {
        g_combat_grid_action_map[itr] = (int8_t *)Near_Allocate_Next(COMBAT_GRID_WIDTH);
    } 
    GUI_String_1 = (char *)Near_Allocate_Next(20);
}


// WZD s103p14
void Combat_Cast_Spell_With_Caster(int16_t caster_id)
{
    char * selection_list_text[4] = { 0, 0, 0, 0 };
    int16_t selection_list_idx = 0;
    int16_t cast_status = 0;
    if(m_wizard_cast_available == ST_TRUE)
    {
        selection_list_text[0] = _players[HUMAN_PLAYER_IDX].name;
        selection_list_text[1] = *_unit_type_table[_UNITS[battle_units[caster_id].unit_idx].type].name;
        selection_list_text[2] = str_cancel__ovr103;
        selection_list_text[3] = str_empty_string__ovr103;
        Clear_Fields();
        selection_list_idx = Selection_Box(3, &selection_list_text[0], 0, cnst_CasterSelectMsg);  // "Who Will Cast"
        switch(selection_list_idx)
        {
            case 0:  /* Player */
            {
                cast_status = Combat_Cast_Spell((CASTER_IDX_BASE + _human_player_idx), _combat_wx, _combat_wy, _combat_wp);
                switch(cast_status)
                {
                    case 2:
                    {
                        m_wizard_cast_available = ST_FALSE;
                    } break;
                }
            } break;
            case 1:  /* Battle Unit */
            {
                cast_status = Combat_Cast_Spell(caster_id, _combat_wx, _combat_wy, _combat_wp);
            } break;
            default:  /* Cancel */
            {
                cast_status = 1;
            } break;
        }
    }
    else
    {
        cast_status = Combat_Cast_Spell(caster_id, _combat_wx, _combat_wy, _combat_wp);
    }
    if(cast_status == ST_FALSE)
    {
        Combat_Cast_Spell_Error(2);  // "You are unable to throw spells at this time."
    }
    Combat_Compose_Background();  // ... |-> Copy_Off_To_Back();
}



/*
    WIZARDS.EXE  ovr105
*/

// WZD o105p01
// MoO2  Module: COMBAT1  Check_For_Winner_()
/**
 * @brief Determines whether combat has ended and, if so, which side wins.
 *
 * This routine evaluates the current battle state in several stages. It first counts active
 * attacker and defender units, treating units with confusion_state equal to 2 as belonging to the
 * opposite side for winner determination. It then checks for outright elimination, applies the
 * combat turn-limit rule that awards the battle to the defender after turn 50, and finally allows
 * the AI-controlled side to concede through AI_Retreat_Check() when that behavior is permitted.
 *
 * If an AI side chooses to flee, all of that side's units are switched to bua_Flee and any active
 * units are marked bus_Fleeing before the opposing human-controlled side is returned as the winner.
 * If none of the ending conditions are met, the battle is still in progress.
 *
 * @return The attacker or defender player index when one side has won the battle.
 * @return _combat_local_player if the computer-controlled side decides to flee.
 * @return ST_UNDEFINED if the battle should continue.
 *
 * @note Confused units with confusion_state equal to 2 are counted for the opposing army.
 * @note A battle that exceeds 50 combat turns is resolved immediately in the defender's favor.
 * @note This function mutates unit action and status state when the AI flee path is taken.
 */
int16_t Check_For_Winner(void)
{
    int16_t defender_count = 0;
    int16_t itr = 0;
    int16_t attacker_count = 0;
    attacker_count = 0;
    defender_count = 0;
    /* Count active units on each side, accounting for confusion */
    for(itr = 0; itr < _combat_total_unit_count; itr++)
    {
        if(battle_units[itr].status == bus_Active)
        {
            if(battle_units[itr].controller_idx == _combat_attacker_player)
            {
                /* OGBUG: The assembly compares confusion_state to 2 */
                if(battle_units[itr].confusion_state == 2)
                {
                    defender_count++;
                }
                else
                {
                    attacker_count++;
                }
            }
            else if(battle_units[itr].controller_idx == _combat_defender_player)
            {
                /* OGBUG: The assembly compares confusion_state to 2 */
                if(battle_units[itr].confusion_state == 2)
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
    LOG_DEBUG(LOG_CAT_COMBAT, "attacker_count: %d", attacker_count);
    LOG_DEBUG(LOG_CAT_COMBAT, "defender_count: %d", defender_count);
    LOG_DEBUG(LOG_CAT_COMBAT, "_combat_turn: %d", _combat_turn);
    /* Check for elimination */
    if(attacker_count == 0)    { return _combat_defender_player; }
    if(defender_count == 0)    { return _combat_attacker_player; }
    /* Check for turn limit timeout (50 turns) */
    if(_combat_turn > 50)      { return _combat_defender_player; }
    /* AI "Fight or Flight" Logic */
    if(_combat_remote_player != NEUTRAL_PLAYER_IDX)
    {
        /* AI will not flee during wizard city sieges or on the first turn */
        if(_computer_player_city_seige == ST_FALSE && _combat_turn > 1)
        {
            if(AI_Retreat_Check(_combat_remote_player) == ST_TRUE)
            {
                /* AI has decided to flee the battle */
                for(itr = 0; itr < _combat_total_unit_count; itr++)
                {
                    if(battle_units[itr].controller_idx == _combat_remote_player)
                    {
                        battle_units[itr].action = bua_Flee;
                        
                        if(battle_units[itr].status == bus_Active)
                        {
                            battle_units[itr].status = bus_Fleeing;
                        }
                    }
                }
                _computer_player_did_flee = ST_TRUE;
                return _combat_local_player;
            }
        }
    }
    /* Battle continues */
    return ST_UNDEFINED;
}


// WZD o105p02
/*
~ UnitView.C Draw_Unit_Figure()
*/
void Combat_Figure_Banner_Color(int16_t player_idx)
{
    int16_t unit_owner_banner_idx = 0;
    int16_t itr_banner_colors = 0;
    unit_owner_banner_idx = _players[player_idx].banner_id;
    for(itr_banner_colors = 0; itr_banner_colors < 5; itr_banner_colors++)
    {
        Replace_Color(scratch_bitmap_seg, (214 + itr_banner_colors), COL_Banners[((unit_owner_banner_idx * 5) + itr_banner_colors)]);
    }
}


// WZD o105p03
/*
Page 92  (PDF Page 97)
"(a red outline surrounds the active unit’s square on the combat grid)"
*/
void Combat_Figure_Active_Red_Outline(int16_t battle_unit_idx)
{
    if(_scanned_battle_unit == battle_unit_idx)
    {
        Replace_Color(scratch_bitmap_seg, 1, 198);
    }
}


// WZD o105p04
void Combat_Figure_Effect(int16_t figure_effect)
{
    FLIC_Set_LoopFrame_1(scratch_bitmap_seg);
    switch(figure_effect)
    {
        case bufe_Black_Sleep:
        {
            Transparent_Color_Range(scratch_bitmap_seg, ST_REMAP_COLOR, ST_REMAP_COLOR);
            Gray_Scale_Bitmap(scratch_bitmap_seg, ST_BLACK);
        } break;
        case bufe_2:  /* Blue, for Guise spell? */
        {
            Transparent_Color_Range(scratch_bitmap_seg, ST_REMAP_COLOR, ST_REMAP_COLOR);
            Inverse_Gray_Scale_Bitmap(scratch_bitmap_seg, 104);
        } break;
        case bufe_Warp_Creature:
        {
            Transparent_Color_Range(scratch_bitmap_seg, ST_REMAP_COLOR, ST_REMAP_COLOR);
            Inverse_Gray_Scale_Bitmap(scratch_bitmap_seg, 40);
        } break;
        case bufe_Invisible_Revealed:
        {
            Transparent_Color_Range(scratch_bitmap_seg, ST_REMAP_COLOR, ST_REMAP_COLOR);
            Replace_Color_All(scratch_bitmap_seg, 233);
        } break;
        case bufe_Invisible_Hidden:
        {
            Transparent_Color_Range(scratch_bitmap_seg,   1, 228);
            Transparent_Color_Range(scratch_bitmap_seg, ST_REMAP_COLOR, ST_REMAP_COLOR);
        } break;
    }
}


// WZD o105p05
/*
Combat_Unit_Enchantment_Outline_Set() + Combat_Unit_Enchantment_Outline_Draw() is the same as Draw_Unit_Enchantment_Outline() from 'Main Screen'
*/
void Combat_Unit_Enchantment_Outline_Set(int16_t battle_unit_idx)
{
    int16_t combat_effects = 0;
    uint32_t unit_enchantments = 0;
    int16_t enchantment_magic_realm = 0;
    enchantment_magic_realm = ST_UNDEFINED;
    unit_enchantments = (battle_units[battle_unit_idx].enchantments | _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments);
    combat_effects = battle_units[battle_unit_idx].combat_effects;
    if(
        ((unit_enchantments & UE_INVISIBILITY) != 0)
        ||
        ((battle_units[battle_unit_idx].Abilities & UA_INVISIBILITY) != 0)
    )
    {
        unit_enchantments = 0;
        combat_effects = 0;
    }
    if(
        (unit_enchantments != 0)
        ||
        (combat_effects != 0)
    )
    {
        if((unit_enchantments & UE_REGENERATION) != 0)
        {
            enchantment_magic_realm = mr_Nature;
        }
        else if((unit_enchantments & UE_RESIST_MAGIC) != 0)
        {
            enchantment_magic_realm = mr_Sorcery;
        }
        else if((unit_enchantments & UE_RIGHTEOUSNESS) != 0)
        {
            enchantment_magic_realm = mr_Life;
        }
        else if((unit_enchantments & UE_PLANAR_TRAVEL) != 0)
        {
            enchantment_magic_realm = mr_Life;
        }
        else if((combat_effects & bue_Haste) != 0)
        {
            enchantment_magic_realm = mr_Sorcery;
        }
        else if((unit_enchantments & UE_BLACK_CHANNELS) != 0)
        {
            enchantment_magic_realm = mr_Death;
        }
        else if((unit_enchantments & UE_LION_HEART) != 0)
        {
            enchantment_magic_realm = mr_Life;
        }
        else if((unit_enchantments & UE_IRON_SKIN) != 0)
        {
            enchantment_magic_realm = mr_Nature;
        }
        else if((unit_enchantments & UE_MAGIC_IMMUNITY) != 0)
        {
            enchantment_magic_realm = mr_Sorcery;
        }
        else if((unit_enchantments & UE_WIND_WALKING) != 0)
        {
            enchantment_magic_realm = mr_Sorcery;
        }
        else if((unit_enchantments & UE_ELEMENTAL_ARMOR) != 0)
        {
            enchantment_magic_realm = mr_Nature;
        }
        else if((unit_enchantments & UE_TRUE_SIGHT) != 0)
        {
            enchantment_magic_realm = mr_Life;
        }
        else if((unit_enchantments & UE_INVULNERABILITY) != 0)
        {
            enchantment_magic_realm = mr_Life;
        }
        else if((unit_enchantments & UE_PATH_FINDING) != 0)
        {
            enchantment_magic_realm = mr_Nature;
        }
        else if((unit_enchantments & UE_FLIGHT) != 0)
        {
            enchantment_magic_realm = mr_Sorcery;
        }
        else if((unit_enchantments & UE_ELDRITCH_WEAPON) != 0)
        {
            enchantment_magic_realm = mr_Chaos;
        }
        else if((unit_enchantments & UE_WRAITH_FORM) != 0)
        {
            enchantment_magic_realm = mr_Death;
        }
        else if((unit_enchantments & UE_GIANT_STRENGTH) != 0)
        {
            enchantment_magic_realm = mr_Nature;
        }
        else if((unit_enchantments & UE_IMMOLATION) != 0)
        {
            enchantment_magic_realm = mr_Chaos;
        }
        else if((unit_enchantments & UE_GUARDIAN_WIND) != 0)
        {
            enchantment_magic_realm = mr_Sorcery;
        }
        else if((unit_enchantments & UE_SPELL_LOCK) != 0)
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
        else if((unit_enchantments & UE_HOLY_ARMOR) != 0)
        {
            enchantment_magic_realm = mr_Life;
        }
        else if((unit_enchantments & UE_WATER_WALKING) != 0)
        {
            enchantment_magic_realm = mr_Nature;
        }
        else if((unit_enchantments & UE_ENDURANCE) != 0)
        {
            enchantment_magic_realm = mr_Life;
        }
        else if((unit_enchantments & UE_CLOAK_OF_FEAR) != 0)
        {
            enchantment_magic_realm = mr_Death;
        }
        else if((unit_enchantments & UE_STONE_SKIN) != 0)
        {
            enchantment_magic_realm = mr_Nature;
        }
        else if((unit_enchantments & UE_FLAME_BLADE) != 0)
        {
            enchantment_magic_realm = mr_Chaos;
        }
        else if((unit_enchantments & UE_BLESS) != 0)
        {
            enchantment_magic_realm = mr_Life;
        }
        else if((unit_enchantments & UE_HOLY_WEAPON) != 0)
        {
            enchantment_magic_realm = mr_Life;
        }
        else if((unit_enchantments & UE_RESIST_ELEMENTS) != 0)
        {
            enchantment_magic_realm = mr_Nature;
        }
    }
    battle_units[battle_unit_idx].outline_magic_realm = enchantment_magic_realm;
}


// WZD o105p06
/*
Combat_Unit_Enchantment_Outline_Set() + Combat_Unit_Enchantment_Outline_Draw() is the same as Draw_Unit_Enchantment_Outline() from 'Main Screen'
*/
void Combat_Unit_Enchantment_Outline_Draw(int16_t enchantment_magic_realm)
{
    if(enchantment_magic_realm != ST_UNDEFINED)
    {
        Outline_Bitmap_Pixels_No_Glass(scratch_bitmap_seg, 255);
        Bitmap_Aura_Pixels(scratch_bitmap_seg, 255, &enchantment_outline_colors[enchantment_magic_realm][unit_enchantment_animation_count][0]);
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

XREF:
    j_Update_Sees_Illusions()
        Combat_Grid_Entities()
        Battle_Unit_Commit_Damage()
*/
void Update_Sees_Illusions(void)
{
    int16_t itr_battle_units;  // _CX_

    _attacker_sees_illusions = ST_FALSE;
    _defender_sees_illusions = ST_FALSE;

    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
    {
        if(battle_units[itr_battle_units].status == bus_Active)
        {
            if(battle_units[itr_battle_units].controller_idx == _combat_attacker_player)
            {
                if((battle_units[itr_battle_units].Attribs_1 & USA_IMMUNITY_ILLUSION) != 0)
                {
                    _attacker_sees_illusions = ST_TRUE;
                }
            }
            else
            {
                if(battle_units[itr_battle_units].controller_idx == _combat_defender_player)
                {
                    if((battle_units[itr_battle_units].Attribs_1 & USA_IMMUNITY_ILLUSION) != 0)
                    {
                        _defender_sees_illusions = ST_TRUE;
                    }
                }
            }
        }
    }

}


// WZD o105p08
void Battle_Unit_Set_Invisibility_Effect(int16_t battle_unit_idx)
{
    uint32_t enchantments = 0;
    int16_t owner_idx = 0;
    int16_t range = 0;
    int16_t is_visible = 0;
    int16_t i = 0;
    /* owner_idx = battle_units[battle_unit_idx].controller_idx; */
    owner_idx = battle_units[battle_unit_idx].controller_idx;
    /* Combine enchantments from battle unit instance and base unit definition */
    enchantments = battle_units[battle_unit_idx].enchantments;
    enchantments |= _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments;
    is_visible = ST_TRUE;
    /* Check for Invisibility (Enchantment or Innate Ability) */
    if(
        (enchantments & UE_INVISIBILITY)
        ||
        (battle_units[battle_unit_idx].Abilities & UA_INVISIBILITY)
    )
    {
        is_visible = ST_FALSE;
        /* True Seeing check: Defender units vs Attacker True Seeing */
        if(owner_idx == _combat_defender_player)
        {
            if(_attacker_sees_illusions == ST_TRUE)
            {
                is_visible = ST_TRUE;
                battle_units[battle_unit_idx].figure_effect = 4;
            }
        }
        /* True Seeing check: Attacker units vs Defender True Seeing */
        if(owner_idx == _combat_attacker_player)
        {
            if(_defender_sees_illusions == ST_TRUE)
            {
                is_visible = ST_TRUE;
                battle_units[battle_unit_idx].figure_effect = 4;
            }
        }
        /* Human player always sees their own invisible units */
        if(owner_idx == _combat_local_player)
        {
            is_visible = ST_TRUE;
            battle_units[battle_unit_idx].figure_effect = 4;
        }
        /* If still not visible, set 'Hidden' effect and check for proximity detection */
        if(is_visible == ST_FALSE)
        {
            battle_units[battle_unit_idx].figure_effect = 5;
            for(i = 0; i < _combat_total_unit_count; i++)
            {
                if(battle_units[i].status == bus_Active)
                {
                    /* Only check proximity for enemy units */
                    if(battle_units[i].controller_idx != owner_idx)
                    {
                        range = Range_To_Battle_Unit(battle_unit_idx, i);
                        /* If an enemy is adjacent (range 0 or 1), the unit is revealed */
                        if(range < 2)
                        {
                            is_visible = ST_TRUE;
                            battle_units[battle_unit_idx].figure_effect = 4;
                            /* Note: Assembly does not break early; continues checking all units */
                        }
                    }
                }
            }
        }
    }
}


// WZD o105p09
int16_t Battle_Unit_Curse_Effects(int16_t battle_unit_idx)
{
    int16_t effect = 0;
    if((battle_units[battle_unit_idx].combat_effects & bue_Black_Sleep) != 0)
    {
        battle_units[battle_unit_idx].figure_effect = bufe_Black_Sleep;
    }
    if(
        ((battle_units[battle_unit_idx].combat_effects & bue_Warped_Attack) != 0)
        ||
        ((battle_units[battle_unit_idx].combat_effects & bue_Warped_Defense) != 0)
        ||
        ((battle_units[battle_unit_idx].combat_effects & bue_Warped_Resist) != 0)
    )
    {
        battle_units[battle_unit_idx].figure_effect = bufe_Warp_Creature;
    }
    if(      (battle_units[battle_unit_idx].combat_effects & bue_Vertigo   ) != 0) { effect = 0; }
    else if( (battle_units[battle_unit_idx].combat_effects & bue_Confusion ) != 0) { effect = 1; }
    else if( (battle_units[battle_unit_idx].combat_effects & bue_Whirlwind ) != 0) { effect = 2; }
    else if( (battle_units[battle_unit_idx].combat_effects & bue_Mind_Storm) != 0) { effect = 3; }
    else if( (battle_units[battle_unit_idx].combat_effects & bue_Shatter   ) != 0) { effect = 4; }
    else if( (battle_units[battle_unit_idx].combat_effects & bue_Weakness  ) != 0) { effect = 5; }
    else if( (battle_units[battle_unit_idx].combat_effects & bue_Mind_Twist) != 0) { effect = 6; }
    else if(((battle_units[battle_unit_idx].combat_effects & bue_Web       ) != 0)
          && (battle_units[battle_unit_idx].Web_HP > 0)                          ) { effect = 7; }
    else { effect = ST_UNDEFINED; }
    return effect;
}


// WZD o105p10
void Battle_Unit_Set_Animation_Flags(int16_t battle_unit_idx)
{
    int16_t battle_unit_is_flying_unit_type = 0;
    int16_t animate = 0;
    battle_unit_is_flying_unit_type = ST_FALSE;
    animate = ST_FALSE;
    if((battle_units[battle_unit_idx].Attribs_1 & USA_FLYING) != 0)
    {
        battle_unit_is_flying_unit_type = ST_TRUE;
        animate = Battle_Unit_Is_Airborne(battle_unit_idx);  /* OGBUG  calls Battle_Unit_Is_Airborne(battle_unit_idx, 0) */
    }
    if(
        (_UNITS[battle_units[battle_unit_idx].unit_idx].type == spell_data_table[spl_Great_Wyrm].unit_type)
        ||
        (_UNITS[battle_units[battle_unit_idx].unit_idx].type == spell_data_table[spl_Fire_Elemental].unit_type)
    )
    {
        animate = ST_TRUE;
    }
    battle_units[battle_unit_idx].animate_idle = animate;
    battle_units[battle_unit_idx].animate_move_as_idle = battle_unit_is_flying_unit_type;
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

            /* 'magic ranged attack' */
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
/*
~ Draw_Active_Unit_Stats_And_Icons()
*/
void Draw_Combat_Unit_Display(void)
{
    int16_t resist_value = 0;
    int16_t defense_value = 0;
    uint8_t colors[2] = { 0, 0 };
    int16_t attack_strength = 0;
    int16_t level_cursor = 0;
    int16_t y2 = 0;
    int16_t x2 = 0;
    int16_t level_icon_count = 0;
    int16_t battle_unit_movement_mode = 0;
    SAMB_ptr level_icon_seg = NULL;
    int16_t unit_idx = 0;
    int16_t unit_type = 0;
    int16_t unit_owner_idx = 0;
    int16_t unit_hero_slot_idx = 0;
    int16_t scan_icon_idx = 0;
    int16_t level_tier_base = 0;
    int16_t x1 = 0;
    int16_t y1 = 0;
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
            stu_strcpy(GUI_String_1, _players[unit_owner_idx].Heroes[unit_hero_slot_idx].name);
        }
        else
        {
            stu_strcpy(GUI_String_1, *_unit_type_table[unit_type].name);
        }
        Print_Centered((x1 + ((x2 - x1) / 2)), (y1 + 2), GUI_String_1);
        scan_icon_idx = Battle_Unit_Melee_Attack_Icon(_scanned_battle_unit);
        if(scan_icon_idx != ST_UNDEFINED)
        {
            attack_strength = battle_units[_scanned_battle_unit].melee;
            Print_Integer_Right((x1 + 9), (y1 + 10), attack_strength);
            FLIC_Draw((x1 + 11), (y1 + 8), combat_scan_icon_segs[scan_icon_idx]);
        }
        scan_icon_idx = Battle_Unit_Ranged_Attack_Icon(_scanned_battle_unit);
        if(scan_icon_idx != ST_UNDEFINED)
        {
            attack_strength = battle_units[_scanned_battle_unit].ranged;
            Print_Integer_Right((x1 + 9), (y1 + 17), attack_strength);
            FLIC_Draw((x1 + 11), (y1 + 15), combat_scan_icon_segs[scan_icon_idx]);
        }
        if(battle_units[_scanned_battle_unit].movement_points != 0)
        {
            Print_Moves_String((x1 + 9), (y1 + 24), battle_units[_scanned_battle_unit].movement_points, 1);
        }
        else
        {
            Print_Integer_Right((x1 + 9), (y1 + 24), battle_units[_scanned_battle_unit].movement_points);
        }
        battle_unit_movement_mode = Battle_Unit_Movement_Mode(_scanned_battle_unit);
        scan_icon_idx = 11;
        if(battle_unit_movement_mode == bumm_Flight)
        {
            scan_icon_idx = 12;
        }
        if(
            (battle_unit_movement_mode == bumm_Sailing)
            ||
            (battle_unit_movement_mode == bumm_Swimming)
        )
        {
            scan_icon_idx = 13;
        }
        FLIC_Draw((x1 + 11), (y1 + 22), combat_scan_icon_segs[scan_icon_idx]);
        Print((x1 + 3), (y1 + 32), cnst_Hits_2);
        Gradient_Fill((x1 + 19), (y1 + 34), (x1 + 38), (y1 + 34), 3, ST_NULL, ST_NULL, ST_NULL, ST_NULL);
        Line((x1 + 19), (y1 + 35), (x1 + 38), (y1 + 35), 1);
        Draw_Active_Unit_Damage_Bar(_scanned_battle_unit, (x1 + 19), (y1 + 34));
        defense_value = battle_units[_scanned_battle_unit].defense;
        Print_Integer_Right((x1 + 48), (y1 + 10), defense_value);
        FLIC_Draw((x1 + 50), (y1 + 8), combat_scan_icon_segs[9]);
        resist_value = battle_units[_scanned_battle_unit].resist;
        Print_Integer_Right((x1 + 48), (y1 + 17), resist_value);
        FLIC_Draw((x1 + 50), (y1 + 15), combat_scan_icon_segs[14]);
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
                level_icon_seg = combat_scan_icon_segs[17];
                level_tier_base = 6;
            }
            else if(_UNITS[unit_idx].Level > 3)
            {
                level_icon_seg = combat_scan_icon_segs[16];
                level_tier_base = 3;
            }
            else
            {
                level_icon_seg = combat_scan_icon_segs[15];
                level_tier_base = 0;
            }
            level_icon_count = 0;
            level_cursor = level_tier_base;
            while(_UNITS[unit_idx].Level > level_cursor)
            {
                FLIC_Draw((x1 + 48 + (level_icon_count * 5)), (y1 + 33), level_icon_seg);
                level_icon_count++;
                level_cursor++;
            }
        }
    }
}



/*
    WIZARDS.EXE  ovr110
*/

// WZD o110p01
// MoO2  Module: COMBAT  Strategic_Combat_()
/*
MoO2  Strategic_Combat()  _qcombat_data = Allocate_First_Block()
TODO  struct s_COMBAT_ENCHANTMENTS
*/
void Strategic_Combat_Allocate(void)
{
    int16_t itr = 0;
    /* HACK */  battlefield = (struct s_BATTLEFIELD *)Allocate_First_Block(_screen_seg, ((sizeof(struct s_BATTLEFIELD) / 16) + 1));
    /* HACK */  battle_units = (struct s_BATTLE_UNIT *)Allocate_Next_Block(_screen_seg, ((((4 * MAX_STACK) * sizeof(struct s_BATTLE_UNIT)) / 16) + 1));
    combat_enchantments = (int8_t *)Allocate_Next_Block(_screen_seg, 3);
    for(itr = 0; itr < 30; itr++)
    {
        combat_enchantments[itr] = ST_FALSE;
    }
    _niu_battlefield_effect = (int16_t *)Allocate_Next_Block(_screen_seg, 4);
    _battlefield_holybonus  = (int16_t *)Allocate_Next_Block(_screen_seg, 3);
    _battlefield_resistall  = (int16_t *)Allocate_Next_Block(_screen_seg, 3);
    _battlefield_leadership = (int16_t *)Allocate_Next_Block(_screen_seg, 3);
}


// WZD o110p02
int16_t Strategic_Combat(int16_t troops[], int16_t troop_count, int16_t wx, int16_t wy, int16_t wp, int16_t * item_count, int16_t item_list[])
{
    int16_t target_weights[(4 * MAX_STACK)] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t defender_spell_attack = 0;
    int16_t attacker_spell_attack = 0;
    int16_t defender_spell_healing = 0;
    int16_t attacker_spell_healing = 0;
    int16_t defender_spell_ranged_pct = 0;
    int16_t attacker_spell_ranged_pct = 0;
    int16_t defender_third_nominal_skill = 0;
    int16_t attacker_third_nominal_skill = 0;
    int16_t defender_healing_magic = 0;
    int16_t attacker_healing_magic = 0;
    int16_t defender_ranged_magic = 0;
    int16_t attacker_ranged_magic = 0;
    int16_t defender_attack_magic = 0;
    int16_t attacker_attack_magic = 0;
    int16_t defender_mana_multiplied = 0;
    int16_t attacker_mana_multiplied = 0;
    int16_t unit_current_hits = 0;
    int16_t special_ranged_attack_strength = 0;
    int16_t winner_health_pct = 0;
    int16_t winner_hits_floor = 0;
    int16_t winner_hits_countdown = 0;
    int16_t defender_raw_hits = 0;
    int16_t attacker_raw_hits = 0;
    int32_t damage_to_attacker = 0;
    int32_t damage_to_defender = 0;
    int32_t defender_hits_start = 0;
    int32_t defender_ranged_strength = 0;
    int32_t defender_melee_strength = 0;
    int32_t attacker_hits_start = 0;
    int32_t attacker_ranged_strength = 0;
    int32_t attacker_melee_strength = 0;
    int32_t defender_hits_left = 0;
    int32_t attacker_hits_left = 0;
    int16_t combat_structure = 0;
    int16_t damage_types[NUM_DAMAGE_TYPES] = { 0, 0, 0 };
    int16_t battle_unit_idx = 0;
    int16_t repeat_target_idx = 0;
    int16_t end_of_combat_message_type = 0;
    int16_t unit_idx = 0;
    int16_t _combat_defender_count = 0;
    int16_t winner_player_idx = 0;
    int16_t itr = 0;
    int16_t _combat_attacker_count = 0;
    int16_t itr_units = 0;
    int16_t itr_battle_units = 0;
    int16_t itr_combat_turns = 0;
    int16_t spell_ranks = 0;
    int16_t did_win = 0;
    attacker_spell_ranged_pct = 0;
    attacker_spell_attack = 0;
    attacker_spell_healing = 0;
    defender_spell_ranged_pct = 0;
    defender_spell_attack = 0;
    defender_spell_healing = 0;
    Set_Page_Off();
    Allocate_Reduced_Map();
    Main_Screen_Draw();
    Copy_Off_To_Back();
    _combat_wx = wx;
    _combat_wy = wy;
    _combat_wp = wp;
    _combat_defender_count = 0;
    /* HACK */  _combat_attacker_count = troop_count;  /* OGBUG:  _combat_attacker_count is uninitialized */
    _combat_total_unit_count = 0;
    Strategic_Combat_Allocate();
    combat_structure = Combat_Structure(wx, wy, wp, ST_TRUE);
    for(itr_units = (_units - 1); itr_units >= 0; itr_units--)
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
        _combat_attacker_player = _UNITS[unit_idx].owner_idx;
        Load_Battle_Unit(unit_idx, &battle_units[_combat_total_unit_count]);
        _combat_total_unit_count++;
    }
    Calc_Battlefield_Bonuses(combat_structure);
    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
    {
        Battle_Unit_Special_Stats(&battle_units[itr_battle_units]);
    }
    /*
        "Ocean Combat"
        "Only flying, sailing and swimming units may actually engage in combat over such squares."
    */
    if(combat_structure == cs_OceanTerrainType)
    {
        _combat_attacker_count -= Undeployable_Battle_Units_On_Water(_combat_attacker_player);  /* OGBUG:  _combat_attacker_count is uninitialized */
        _combat_defender_count -= Undeployable_Battle_Units_On_Water(_combat_defender_player);
    }
    attacker_hits_start = 0;
    defender_hits_start = 0;
    attacker_hits_left = 0;
    defender_hits_left = 0;
    attacker_melee_strength = 0;
    defender_melee_strength = 0;
    attacker_ranged_strength = 0;
    defender_ranged_strength = 0;
    attacker_raw_hits = 0;
    defender_raw_hits = 0;
    /*
        BEGIN:  Not a real player  (ST_UNDEFINED, MOO_MONSTER_PLAYER_IDX, etc.)
    */
    if((_combat_attacker_player < HUMAN_PLAYER_IDX) || (_combat_attacker_player > NEUTRAL_PLAYER_IDX))
    {
        for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
        {
            if(battle_units[itr_battle_units].controller_idx == _combat_attacker_player)
            {
                battle_units[itr_battle_units].status = bus_Gone;
            }
        }
    }
    if((_combat_defender_player < HUMAN_PLAYER_IDX) || (_combat_defender_player > NEUTRAL_PLAYER_IDX))
    {
        for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
        {
            if(battle_units[itr_battle_units].controller_idx == _combat_defender_player)
            {
                battle_units[itr_battle_units].status = bus_Gone;
            }
        }
    }
    /*
        END:  Not a real player  (ST_UNDEFINED, MOO_MONSTER_PLAYER_IDX, etc.)
    */
   for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
   {
        if(battle_units[itr_battle_units].status == bus_Active)
        {
            target_weights[itr_battle_units] = (30 - battle_units[itr_battle_units].defense);

            if(battle_units[itr_battle_units].controller_idx == _combat_attacker_player)
            {
                // Unit's Current Total Hit Points
                unit_current_hits = ((battle_units[itr_battle_units].hits * battle_units[itr_battle_units].figure_cnt) - battle_units[itr_battle_units].front_figure_damage);
                attacker_hits_left += Get_Effective_Hits(unit_current_hits, battle_units[itr_battle_units].defense);
                if(Battle_Unit_Has_Ranged_Attack(itr_battle_units) != ST_FALSE)
                {
                    attacker_ranged_strength += Get_Effective_Ranged_Strength(battle_units[itr_battle_units].ranged, battle_units[itr_battle_units].figure_cnt, (battle_units[itr_battle_units].attack_attributes | battle_units[itr_battle_units].ranged_attack_attributes));
                }
                // {100, 101, 102, 103, 104, 105} srat_Thrown, srat_FireBreath, srat_Lightning, srat_StoneGaze, srat_MultiGaze, srat_DeathGaze
                // special ranged attack strength
                // Thrown, Breath, Gaze
                if(battle_units[itr_battle_units].ranged_type >= srat_Thrown)
                {
                    special_ranged_attack_strength = battle_units[itr_battle_units].ranged;
                }
                else
                {
                    special_ranged_attack_strength = 0;
                }
                attacker_melee_strength += Get_Effective_Melee_Strength(battle_units[itr_battle_units].melee, special_ranged_attack_strength, battle_units[itr_battle_units].figure_cnt, (battle_units[itr_battle_units].attack_attributes | battle_units[itr_battle_units].melee_attack_attributes), battle_units[itr_battle_units].ranged_type);
                attacker_raw_hits += ((battle_units[itr_battle_units].hits * battle_units[itr_battle_units].figure_cnt) - battle_units[itr_battle_units].front_figure_damage);
            }
            else  /* battle_units[itr_battle_units].controller_idx == _combat_defender_player */
            {
                // Unit's Current Total Hit Points
                unit_current_hits = ((battle_units[itr_battle_units].hits * battle_units[itr_battle_units].figure_cnt) - battle_units[itr_battle_units].front_figure_damage);
                defender_hits_left += Get_Effective_Hits(unit_current_hits, battle_units[itr_battle_units].defense);
                if(Battle_Unit_Has_Ranged_Attack(itr_battle_units) != ST_FALSE)
                {
                    defender_ranged_strength += Get_Effective_Ranged_Strength(battle_units[itr_battle_units].ranged, battle_units[itr_battle_units].figure_cnt, (battle_units[itr_battle_units].attack_attributes | battle_units[itr_battle_units].ranged_attack_attributes));
                }
                if(battle_units[itr_battle_units].ranged_type >= srat_Thrown)
                {
                    special_ranged_attack_strength = battle_units[itr_battle_units].ranged;
                }
                else
                {
                    special_ranged_attack_strength = 0;
                }
                defender_melee_strength += Get_Effective_Melee_Strength(battle_units[itr_battle_units].melee, special_ranged_attack_strength, battle_units[itr_battle_units].figure_cnt, (battle_units[itr_battle_units].attack_attributes | battle_units[itr_battle_units].melee_attack_attributes), battle_units[itr_battle_units].ranged_type);
                defender_raw_hits += ((battle_units[itr_battle_units].hits * battle_units[itr_battle_units].figure_cnt) - battle_units[itr_battle_units].front_figure_damage);
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
        attacker_attack_magic = 0;
        attacker_ranged_magic = 0;
        attacker_healing_magic = 0;
        if(attacker_mana_multiplied >= 10)
        {
            spell_ranks = _players[_combat_attacker_player].spellranks[sbr_Nature];
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
                attacker_attack_magic = (spell_ranks * 50);
                attacker_ranged_magic = (spell_ranks * 10);
                attacker_healing_magic = (spell_ranks * 25);
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
                attacker_attack_magic = (spell_ranks * 75);
                attacker_healing_magic = (spell_ranks * 25);
            }
            spell_ranks = _players[_combat_attacker_player].spellranks[sbr_Sorcery];
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
                attacker_attack_magic = (spell_ranks * 75);
                attacker_healing_magic = (spell_ranks * 25);
            }
            spell_ranks = _players[_combat_attacker_player].spellranks[sbr_Chaos];
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
                attacker_attack_magic = (spell_ranks * 100);
                attacker_ranged_magic = (spell_ranks * 30);
            }
            spell_ranks = _players[_combat_attacker_player].spellranks[sbr_Life];
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
                attacker_ranged_magic = (spell_ranks * 15);
                attacker_healing_magic = (spell_ranks * 40);
            }
            spell_ranks = _players[_combat_attacker_player].spellranks[sbr_Death];

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
                attacker_attack_magic = (spell_ranks * 75);
                attacker_healing_magic = (spell_ranks * 15);
            }
            attacker_attack_magic = (attacker_attack_magic / 10);
            attacker_ranged_magic = (attacker_ranged_magic / 10);
            attacker_healing_magic = (attacker_healing_magic / 20);
        }  /* if(attacker_mana_multiplied >= 10) */

    }  /* if(_combat_attacker_player >= _num_players) */
    if(_combat_defender_player >= _num_players)  /* Neural Player or Monster */
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
        defender_attack_magic = 0;
        defender_ranged_magic = 0;
        defender_healing_magic = 0;
        if(defender_mana_multiplied >= 10)
        {
            spell_ranks = _players[_combat_defender_player].spellranks[sbr_Nature];
            if(spell_ranks > 0)
            {
                defender_attack_magic = (spell_ranks * 50);
                defender_ranged_magic = (spell_ranks * 10);
                defender_healing_magic = (spell_ranks * 25);
            }
            spell_ranks = _players[_combat_defender_player].spellranks[sbr_Sorcery];
            if(spell_ranks > 0)
            {
                defender_attack_magic = (spell_ranks * 75);
                defender_healing_magic = (spell_ranks * 25);
            }
            spell_ranks = _players[_combat_defender_player].spellranks[sbr_Chaos];
            if(spell_ranks > 0)
            {
                defender_attack_magic = (spell_ranks * 100);
                defender_ranged_magic = (spell_ranks * 30);
            }
            spell_ranks = _players[_combat_defender_player].spellranks[sbr_Life];
            if(spell_ranks > 0)
            {
                defender_ranged_magic = (spell_ranks * 15);
                defender_healing_magic = (spell_ranks * 40);
            }
            spell_ranks = _players[_combat_defender_player].spellranks[sbr_Death];
            if(spell_ranks > 0)
            {
                defender_attack_magic = (spell_ranks * 75);
                defender_healing_magic = (spell_ranks * 15);
            }
            defender_attack_magic = (defender_attack_magic / 10);
            defender_ranged_magic = (defender_ranged_magic / 10);
            defender_healing_magic = (defender_healing_magic / 20);
        }
    }  /* if(_combat_defender_player >= _num_players) */

    /*
        END:  Mods - Mana / Spell-Ranks
    */
    attacker_hits_start = attacker_hits_left;
    defender_hits_start = defender_hits_left;
    /*
        BEGIN:  CP Cheat vs. NCP/Monster
    */
    if(
        (_combat_attacker_player != HUMAN_PLAYER_IDX)
        &&
        (_combat_defender_player != HUMAN_PLAYER_IDX)
    )
    {
        if(_combat_attacker_player != NEUTRAL_PLAYER_IDX)
        {
            attacker_spell_ranged_pct += (attacker_spell_ranged_pct / 2);
            attacker_spell_attack     += (attacker_spell_attack     / 2);
            attacker_spell_healing    += (attacker_spell_healing    / 2);
            attacker_hits_start       += (attacker_hits_start       / 2);
            attacker_hits_left        += (attacker_hits_left        / 2);
            attacker_melee_strength   += (attacker_melee_strength   / 2);
            attacker_ranged_strength  += (attacker_ranged_strength  / 2);
            attacker_attack_magic     += (attacker_attack_magic     / 2);
            attacker_ranged_magic     += (attacker_ranged_magic     / 2);
            attacker_healing_magic    += (attacker_healing_magic    / 2);
        }
        if(_combat_defender_player != NEUTRAL_PLAYER_IDX)
        {
            defender_spell_ranged_pct += (defender_spell_ranged_pct / 2);
            defender_spell_attack += (defender_spell_attack         / 2);
            defender_spell_healing += (defender_spell_healing       / 2);
            defender_hits_start     += (defender_hits_start         / 2);
            defender_hits_left        += (defender_hits_left        / 2);
            defender_melee_strength  += (defender_melee_strength    / 2);
            defender_ranged_strength += (defender_ranged_strength   / 2);
            defender_attack_magic += (defender_attack_magic         / 2);
            defender_ranged_magic += (defender_ranged_magic         / 2);
            defender_healing_magic += (defender_healing_magic       / 2);
        }
    }
    /*
        END:  CP Cheat vs. NCP/Monster
    */
    /*
        BEGIN:  Combat Turn
    */
    for(itr_combat_turns = 0; itr_combat_turns < 3; itr_combat_turns++)
    {
        if((attacker_ranged_strength > 10) || (defender_ranged_strength > 10))
        {
            if((attacker_hits_left > 0) && (defender_hits_left > 0))
            {
                if(attacker_mana_multiplied > 10)
                {
                    attacker_spell_attack = ((attacker_attack_magic * attacker_third_nominal_skill) / 5);
                    attacker_spell_ranged_pct = ((attacker_ranged_magic * attacker_third_nominal_skill) / 5);
                    attacker_spell_healing = ((attacker_healing_magic * attacker_third_nominal_skill) / 5);
                    attacker_spell_attack += ((attacker_ranged_strength * attacker_spell_ranged_pct) / 100);
                    attacker_healing_magic = (attacker_healing_magic / 2);
                    _players[_combat_attacker_player].mana_reserve -= ((Combat_Casting_Cost_Multiplier(_combat_attacker_player) * attacker_third_nominal_skill) / 10);
                    attacker_mana_multiplied -= attacker_third_nominal_skill;
                    if(attacker_third_nominal_skill < attacker_mana_multiplied)
                    {
                        attacker_third_nominal_skill = attacker_mana_multiplied;
                    }
                }
                else
                {
                    attacker_spell_ranged_pct = 0;
                    attacker_spell_attack = 0;
                    attacker_spell_healing = 0;
                }
                if(defender_mana_multiplied > 10)
                {
                    defender_spell_attack = ((defender_attack_magic * defender_third_nominal_skill) / 5);
                    defender_spell_ranged_pct = ((defender_ranged_magic * defender_third_nominal_skill) / 5);
                    defender_spell_healing = ((defender_healing_magic * defender_third_nominal_skill) / 5);
                    defender_spell_attack += ((defender_ranged_strength * defender_spell_ranged_pct) / 100);
                    defender_healing_magic = (defender_healing_magic / 2);
                    _players[_combat_defender_player].mana_reserve -= ((Combat_Casting_Cost_Multiplier(_combat_defender_player) * defender_third_nominal_skill) / 10);
                    defender_mana_multiplied -= defender_third_nominal_skill;
                    if(defender_third_nominal_skill < defender_mana_multiplied)
                    {
                        defender_third_nominal_skill = defender_mana_multiplied;
                    }
                }
                else
                {
                    defender_spell_ranged_pct = 0;
                    defender_spell_attack = 0;
                    defender_spell_healing = 0;
                }
                damage_to_defender = (((attacker_ranged_strength + attacker_spell_attack) * Random(10)) / 100);
                damage_to_attacker = (((defender_ranged_strength + defender_spell_attack) * Random(10)) / 100);
                attacker_hits_left += attacker_spell_healing;
                defender_hits_left += defender_spell_healing;
                attacker_ranged_strength -= ((damage_to_attacker * attacker_ranged_strength) / attacker_hits_left);
                defender_ranged_strength -= ((damage_to_defender * defender_ranged_strength) / defender_hits_left);
                attacker_melee_strength -= ((damage_to_attacker * attacker_melee_strength) / attacker_hits_left);
                defender_melee_strength -= ((damage_to_defender * defender_melee_strength) / defender_hits_left);
                attacker_hits_left -= damage_to_attacker;
                defender_hits_left -= damage_to_defender;
                if(attacker_ranged_strength < 0)
                {
                    attacker_ranged_strength = 0;
                }
                if(defender_ranged_strength < 0)
                {
                    defender_ranged_strength = 0;
                }
            }
        }
    }  /* for(itr_combat_turns = 0; itr_combat_turns < 3; itr_combat_turns++) */
    while((attacker_melee_strength > 10) && (defender_melee_strength > 10))
    {
        if(attacker_mana_multiplied <= 10)
        {
            attacker_spell_ranged_pct = 0;
            attacker_spell_attack = 0;
            attacker_spell_healing = 0;  // ¿ ~== healing ?
        }
        else
        {
            attacker_spell_attack = ((attacker_attack_magic * attacker_third_nominal_skill) / 5);
            attacker_spell_ranged_pct = ((attacker_ranged_magic * attacker_third_nominal_skill) / 5);
            attacker_spell_healing = ((attacker_ranged_magic * attacker_third_nominal_skill) / 5);  // ¿ OGBUG  ...  ~== healing ?
            attacker_spell_attack += ((attacker_ranged_strength * attacker_spell_ranged_pct) / 100);
            _players[_combat_attacker_player].mana_reserve -= ((Combat_Casting_Cost_Multiplier(_combat_attacker_player) * attacker_third_nominal_skill) / 10);
            attacker_mana_multiplied -= (attacker_third_nominal_skill / 2);
            if((attacker_mana_multiplied / 2) < attacker_third_nominal_skill)
            {
                attacker_third_nominal_skill = (attacker_mana_multiplied / 2);
            }
        }
        if(defender_mana_multiplied <= 10)
        {
            defender_spell_ranged_pct = 0;
            defender_spell_attack = 0;
            defender_spell_healing = 0;  // ¿ ~== healing ?
        }
        else
        {
            defender_spell_attack = ((defender_attack_magic * defender_third_nominal_skill) / 5);
            defender_spell_ranged_pct = ((defender_ranged_magic * defender_third_nominal_skill) / 5);
            defender_spell_healing = ((defender_ranged_magic * defender_third_nominal_skill) / 5);  // ¿ ~== healing ?
            defender_spell_attack += ((defender_ranged_strength * defender_spell_ranged_pct) / 100);
            _players[_combat_defender_player].mana_reserve -= ((Combat_Casting_Cost_Multiplier(_combat_defender_player) * defender_third_nominal_skill) / 10);
            defender_mana_multiplied -= (defender_third_nominal_skill / 2);
            if((defender_mana_multiplied / 2) < defender_third_nominal_skill)
            {
                defender_third_nominal_skill = (defender_mana_multiplied / 2);
            }
        }
        damage_to_defender = (((attacker_melee_strength + attacker_spell_attack) * Random(10)) / 100);
        damage_to_attacker = (((defender_melee_strength + defender_spell_attack) * Random(10)) / 100);
        attacker_hits_left += attacker_spell_healing;  // ¿ ~== healing ?
        defender_hits_left += defender_spell_healing;  // ¿ ~== healing ?
        attacker_melee_strength -= ((attacker_melee_strength * damage_to_attacker) / attacker_hits_left);
        defender_melee_strength -= ((defender_melee_strength * damage_to_defender) / defender_hits_left);
        attacker_hits_left -= damage_to_attacker;
        defender_hits_left -= damage_to_defender;

    }  /* while((attacker_melee_strength > 10) & (defender_melee_strength > 10)) */
    /*
        END:  Combat Turn
    */
    if(attacker_melee_strength > 10)
    {
        winner_player_idx = _combat_attacker_player;
        winner_health_pct = ((100 * attacker_hits_left) / attacker_hits_start);
        winner_hits_floor = ((winner_health_pct * attacker_raw_hits) / 100);
        winner_hits_countdown = attacker_raw_hits;
    }
    // BUGBUG: duplicated code?
    if(attacker_melee_strength > 10)
    {
        winner_player_idx = _combat_attacker_player;
        winner_health_pct = ((100 * attacker_hits_left) / attacker_hits_start);
        winner_hits_floor = ((winner_health_pct * attacker_raw_hits) / 100);
        winner_hits_countdown = attacker_raw_hits;
    }
    else
    {
        if(defender_melee_strength > 10)
        {
            winner_player_idx = _combat_defender_player;
            winner_health_pct = ((100 * defender_hits_left) / defender_hits_start);
            winner_hits_floor = ((winner_health_pct * defender_raw_hits) / 100);
            winner_hits_countdown = defender_raw_hits;
        }
    }
    for(itr = 0; itr < _combat_total_unit_count; itr++)
    {
        if(battle_units[itr].controller_idx != winner_player_idx)
        {
            battle_units[itr].status = bus_Dead;  /* Unit_Dead */
            target_weights[itr] = 0;
        }
    }
    damage_types[dt_Normal] = 3;
    damage_types[dt_Drain] = 0;
    damage_types[dt_Doom] = 0;
    if(winner_player_idx != ST_UNDEFINED)
    {
        repeat_target_idx = ST_UNDEFINED;
        while(winner_hits_countdown > winner_hits_floor)
        {
            if(repeat_target_idx <= ST_UNDEFINED)
            {
                battle_unit_idx = Get_Weighted_Choice(&target_weights[0], _combat_total_unit_count);
            }
            else
            {
                battle_unit_idx = repeat_target_idx;
            }
            target_weights[battle_unit_idx] += 50;
            Battle_Unit_Commit_Damage(battle_unit_idx, &damage_types[0]);
            if(battle_units[battle_unit_idx].status > 0)
            {
                target_weights[battle_unit_idx] = 0;
                repeat_target_idx = ST_UNDEFINED;
            }
            else
            {
                repeat_target_idx = battle_unit_idx;
            }
            winner_hits_countdown -= 3;
        }
    }
    if(winner_player_idx == HUMAN_PLAYER_IDX)
    {
        end_of_combat_message_type = csmt_VictoryStrategic;
    }
    else
    {
        end_of_combat_message_type = csmt_DefeatStrategic;
    }
    End_Of_Combat(winner_player_idx, item_count, item_list, end_of_combat_message_type);
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
void Battle_Unit_Heal(int16_t battle_unit_idx, int16_t healing_amount, int16_t overheal_flag)
{
    int16_t damages[NUM_DAMAGE_TYPES] = { 0, 0, 0 };
    int16_t movement_points_saved = 0;
    int16_t damage_amount_healable = 0;
    int16_t healing_balance = 0;
    int16_t itr_damages = 0;
    damage_amount_healable = battle_units[battle_unit_idx].damage[1] + battle_units[battle_unit_idx].damage[0];
    healing_balance = 0;
    /* OGBUG  allows Life Stealing attacks to ignore Irreversible Damage - need to remove `(overheal_flag == 0)` here and then `if(overheal_flag == 0) { healing_surplus = 0; } healing_balance = healing_surplus;` below */
    if(
        (overheal_flag == 0)
        &&
        (healing_amount > damage_amount_healable)
    )
    {
        healing_amount = damage_amount_healable;
    }
    damages[0] = battle_units[battle_unit_idx].damage[dt_Normal];
    damages[1] = battle_units[battle_unit_idx].damage[1];
    damages[2] = battle_units[battle_unit_idx].damage[2];
    damages[0] -= healing_amount;
    if(damages[0] < 0)
    {
        damages[1] += damages[0];
        damages[0] = 0;
        SETMIN(damages[1], 0);
    }
    for(itr_damages = 0; itr_damages < NUM_DAMAGE_TYPES; itr_damages++)
    {
        battle_units[battle_unit_idx].damage[itr_damages] = damages[itr_damages];
    }
    battle_units[battle_unit_idx].front_figure_damage -= healing_amount;
    if(battle_units[battle_unit_idx].front_figure_damage < 0)
    {
        healing_balance = battle_units[battle_unit_idx].front_figure_damage;
        battle_units[battle_unit_idx].front_figure_damage = 0;
        while((healing_balance < 0) && (battle_units[battle_unit_idx].figure_max > battle_units[battle_unit_idx].figure_cnt))
        {
            battle_units[battle_unit_idx].figure_cnt += 1;
            healing_balance += battle_units[battle_unit_idx].hits;
        }
        if(healing_balance > 0)
        {
            battle_units[battle_unit_idx].front_figure_damage = (int8_t)healing_balance;
        }
    }
    /* OGBUG  allows gaining extra hits even if set to 0 by jumping to the wrong location */
    /* OGBUG  allows gaining extra hits without overhealing based on the top figure damage alone (should be `(healing_balance <= 0)`) */
    if(
        (overheal_flag == 0)
        &&
        (healing_balance < 0)
    )
    {
        healing_balance = 0;
    }
    healing_balance = abs(healing_balance);
    if(battle_units[battle_unit_idx].figure_max <= healing_balance)
    {
        battle_units[battle_unit_idx].Extra_Hits += (healing_balance / battle_units[battle_unit_idx].figure_max);
    }
    movement_points_saved = battle_units[battle_unit_idx].movement_points;
    Battle_Unit_Regular_Stats(&battle_units[battle_unit_idx]);
    Battle_Unit_Special_Stats(&battle_units[battle_unit_idx]);
    battle_units[battle_unit_idx].movement_points = (int8_t)movement_points_saved;
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
    |-> unit_current_hits  = ((battle_units[itr_battle_units].hits / battle_units[itr_battle_units].figure_cnt) - battle_units[itr_battle_units].TopFig_Dmg);
    |-> attacker_hits_left += Get_Effective_Hits(unit_current_hits , battle_units[itr_battle_units].defense);

unit_current_hits  = ((battle_units[].hits / battle_units[].figure_cnt) - battle_units[].TopFig_Dmg);

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
                effective_hits = ((effective_hits * 7) / 10);  // 70%
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
    |-> defender_ranged_strength += Get_Effective_Ranged_Strength(battle_units[itr_battle_units].ranged, battle_units[itr_battle_units].figure_cnt, (battle_units[itr_battle_units].Attack_Flags | battle_units[itr_battle_units].Ranged_ATK_Flags));

battle_units[].ranged
battle_units[].figure_cnt
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
            ranged_threat = (ranged_threat / 3);
        }
        else if(ranged == 3)
        {
            ranged_threat = ((ranged_threat * 2) / 3);
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
    |-> attacker_melee_strength += Get_Effective_Melee_Strength(battle_units[itr_battle_units].melee, special_ranged_attack_strength, battle_units[itr_battle_units].figure_cnt, (battle_units[itr_battle_units].Attack_Flags | battle_units[itr_battle_units].Melee_ATK_Flags), battle_units[itr_battle_units].ranged_type);

battle_units[itr_battle_units].melee
special_ranged_attack_strength
battle_units[itr_battle_units].figure_cnt
(battle_units[itr_battle_units].Attack_Flags | battle_units[itr_battle_units].Melee_ATK_Flags)
battle_units[itr_battle_units].ranged_type

¿ Calc, Mods, Effects, etc. ?
    battle_units[].melee = ¿ unit_type_table[_UNITS[].type].melee ?
    thrown = ¿ unit_type_table[_UNITS[].type].ranged ?
    battle_units[].figure_cnt
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
                    thrown_threat = ((thrown_threat * 2) / 3);
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
/**
 * @brief Chooses an AI combat-spell target (unit or square) for a specific spell.
 *
 * @details
 * Dispatches on @c spell_data_table[spell_idx].type and evaluates candidate
 * targets using spell-specific heuristics and eligibility filters. Depending on
 * spell class, this routine may:
 * - Score enemy units for direct damage / save-or-die effects.
 * - Select friendly units for buffs.
 * - Select enemy units for debuffs/control effects.
 * - Delegate to specialized pickers (for example healing, warp effects,
 *   dispel, wall/terrain spells).
 * - Produce map coordinates for square-targeted spells and summoning placement
 *   through @p target_wx/@p target_wy.
 *
 * Common filters include active status, ownership, visibility, immunity flags,
 * resistance-derived expected value, and selected enchantment interactions.
 *
 * @param spell_idx Spell identifier to evaluate.
 * @param player_idx Casting player index.
 * @param target_wx Output X coordinate for square-targeted selections.
 * @param target_wy Output Y coordinate for square-targeted selections.
 *
 * @return Battle-unit index for unit-targeted spells when a target is selected.
 * @return @c 99 for spell types that resolve as "castable/valid" without a
 *         specific unit index (for example some global/special targets).
 * @return @c -1 when no valid target is found for the evaluated branch.
 * @return @c ST_UNDEFINED in early failure paths (for example summon when unit
 *         cap is already reached).
 *
 * @note The routine preserves multiple OGBUG/legacy behaviors noted inline,
 *       including switch-domain exclusions, some immunity/eligibility
 *       inconsistencies, and summon placement edge handling.
 * @note Coordinate outputs are only meaningful for square-targeted spell paths.
 *
 * @see AITP_Healing(), AITP_WarpWood(), AITP_WarpCreature(),
 *      AITP_DispelMagic(), AITP_EarthToMud(), AITP_Disrupt(),
 *      AITP_CracksCall(), Compute_Battle_Unit_Damage_From_Spell()
 */
int16_t AITP_Combat_Spell(int16_t spell_idx, int16_t player_idx, int16_t * target_wx, int16_t * target_wy)
{
    int16_t damage_types[NUM_DAMAGE_TYPES] = { 0, 0, 0 };
    int16_t spell_resistance_modifier = 0;
    int32_t enchantments = 0;
    int16_t unit_resistance = 0;  /* Unit_Resist  reused: attacker/defender side index in the Battlefield arm, own-unit count then retry counter in the Summoning arm */
    int16_t selected_target_idx = 0;
    int16_t target_value = 0;
    int16_t highest_value = 0;
    int16_t battle_unit_idx = 0;
    struct s_BATTLE_UNIT * bu_ptr = NULL;
    uint32_t battle_unit_enchantments = 0;
    highest_value = ST_UNDEFINED;
    selected_target_idx = ST_UNDEFINED;
    /* OGBUG  switch jump table excludes Wall spells */
    if(spell_data_table[spell_idx].type > scc_Combat_Banish)
    {
        return selected_target_idx;
    }
    switch(spell_data_table[spell_idx].type)
    {
        case scc_Direct_Damage_Fixed:
        case scc_Direct_Damage_Variable:  /* Direct Damage */
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                damage_types[dt_Normal] = 0;
                damage_types[dt_Drain] = 0;
                damage_types[dt_Doom] = 0;
                bu_ptr = &battle_units[battle_unit_idx];
                if(bu_ptr->Attribs_1 & USA_IMMUNITY_MAGIC) continue;
                if(spell_idx == spl_Star_Fires)
                {
                    if(bu_ptr->race != rt_Death && bu_ptr->race != rt_Chaos) continue;
                }
                if(spell_idx == spl_Psionic_Blast)
                {
                    if(bu_ptr->Attribs_1 & USA_IMMUNITY_ILLUSION) continue;
                }
                if(spell_idx == spl_Life_Drain)
                {
                    if(bu_ptr->race == rt_Death || (bu_ptr->Attribs_1 & USA_IMMUNITY_DEATH)) continue;
                }
                enchantments = _UNITS[bu_ptr->unit_idx].enchantments;
                enchantments |= bu_ptr->enchantments;
                enchantments |= bu_ptr->item_enchantments;
                if(enchantments & UE_RIGHTEOUSNESS)
                {
                    if(spell_data_table[spell_idx].magic_realm == sbr_Chaos || spell_data_table[spell_idx].magic_realm == sbr_Death) continue;
                }
                if(bu_ptr->controller_idx == player_idx) continue;
                if(bu_ptr->status != bus_Active) continue;
                if(!Target_Is_Visible(battle_unit_idx)) continue;
                if(spell_idx == spl_Life_Drain)
                {
                    /* Manual effective resistance check for Life Drain */
                    unit_resistance = Combat_Effective_Resistance(battle_units[battle_unit_idx], sbr_Death);
                    damage_types[dt_Drain] = (13 - unit_resistance);
                    if(damage_types[dt_Drain] < 0) damage_types[dt_Drain] = 0;
                }
                else
                {
                    Compute_Battle_Unit_Damage_From_Spell(spell_idx, battle_unit_idx, &damage_types[0], 25);
                }
                target_value = damage_types[dt_Normal] + damage_types[dt_Drain] + damage_types[dt_Doom];
                if(target_value > 0)
                {
                    /* MoM AI heuristic: Prioritize damaged/weakened units */
                    /* Value += (100 - current_total_hp) + current_figure_damage */
                    target_value += (100 - (bu_ptr->figure_cnt * bu_ptr->hits)) + bu_ptr->front_figure_damage;
                }
                if(target_value > highest_value)
                {
                    highest_value = target_value;
                    selected_target_idx = battle_unit_idx;
                }
            }
            break;
        case scc_Combat_Destroy_Unit:
        case scc_Combat_Banish: /* Resistance-based Kill Spells */
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                bu_ptr = &battle_units[battle_unit_idx];
                if(bu_ptr->controller_idx == player_idx) continue;
                if(bu_ptr->status != bus_Active) continue;
                if(bu_ptr->Attribs_1 & USA_IMMUNITY_MAGIC) continue;
                if((bu_ptr->Attribs_1 & USA_IMMUNITY_STONING) && spell_idx == spl_Petrify) continue;
                if(spell_idx == spl_Dispel_Evil)
                {
                    if(bu_ptr->race != rt_Death && bu_ptr->race != rt_Chaos) continue;
                }
                if(spell_idx == spl_Banish)
                {
                    if(bu_ptr->race < rt_Arcane) continue; /* Only Fantastic creatures */
                }
                enchantments = _UNITS[bu_ptr->unit_idx].enchantments;
                enchantments |= bu_ptr->enchantments;
                enchantments |= bu_ptr->item_enchantments;
                if(enchantments & UE_RIGHTEOUSNESS)
                {
                    if(spell_data_table[spell_idx].magic_realm == sbr_Chaos || spell_data_table[spell_idx].magic_realm == sbr_Death) continue;
                }
                if(!Target_Is_Visible(battle_unit_idx)) continue;
                spell_resistance_modifier = Spell_Resistance_Modifier(spell_idx);
                unit_resistance = Combat_Effective_Resistance(battle_units[battle_unit_idx], spell_data_table[spell_idx].magic_realm);
                unit_resistance += spell_resistance_modifier;
                if(unit_resistance >= 10) continue;
                if(spell_idx == spl_Disintegrate)
                {
                    target_value = Effective_Battle_Unit_Strength(battle_unit_idx);
                }
                else
                {
                    /* Scaled strength value based on failure chance */
                    target_value = (Effective_Battle_Unit_Strength(battle_unit_idx) * (10 - unit_resistance) + 9) / 10;
                }
                if(target_value > highest_value)
                {
                    highest_value = target_value;
                    selected_target_idx = battle_unit_idx;
                }
            }
            break;
        case scc_Unit_Enchantment: /* Unit Buffs */
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                bu_ptr = &battle_units[battle_unit_idx];
                if(bu_ptr->controller_idx != player_idx) continue;
                if(bu_ptr->status != bus_Active) continue;
                enchantments = _UNITS[bu_ptr->unit_idx].enchantments;
                enchantments |= bu_ptr->enchantments;
                enchantments |= bu_ptr->item_enchantments;
                /* Check if buff already exists or incompatible */
                if(enchantments & spell_data_table[spell_idx].enchantments) continue;
                if(spell_idx == spl_Stone_Skin && (enchantments & UE_IRON_SKIN)) continue;
                if(spell_idx == spl_Resist_Elements && (enchantments & UE_ELEMENTAL_ARMOR)) continue;
                if(spell_idx == spl_Resist_Magic && (enchantments & UE_WRAITH_FORM)) continue;
                if(spell_idx == spl_Flight && (bu_ptr->Move_Flags & MV_FLYING)) continue;
                if(spell_idx == spl_Berserk)
                {
                    if(bu_ptr->target_battle_unit_idx == ST_UNDEFINED) continue;
                    if(bu_ptr->target_battle_unit_idx == battle_unit_idx) continue;
                    /* Don't berserk if target is out of range of movement */
                    if(Range_To_Battle_Unit(battle_unit_idx, bu_ptr->target_battle_unit_idx) > bu_ptr->movement_points) continue;
                    /* Don't berserk Heroes */
                    if(_UNITS[bu_ptr->unit_idx].Hero_Slot > ST_UNDEFINED) continue;
                }
                target_value = (bu_ptr->figure_cnt * 10) + bu_ptr->melee;
                if(target_value > highest_value)
                {
                    highest_value = target_value;
                    selected_target_idx = battle_unit_idx;
                }
            }
            break;
        case scc_Unit_Enchantment_Normal_Only:
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                bu_ptr = &battle_units[battle_unit_idx];
                if(bu_ptr->controller_idx != player_idx) continue;
                if(bu_ptr->status != bus_Active) continue;
                if(bu_ptr->race >= rt_Arcane) continue;
                enchantments = _UNITS[bu_ptr->unit_idx].enchantments;
                enchantments |= bu_ptr->enchantments;
                enchantments |= bu_ptr->item_enchantments;
                if(enchantments & spell_data_table[spell_idx].enchantments) continue;
                if(spell_idx == spl_Heroism && _UNITS[bu_ptr->unit_idx].Level > 2) continue;
                target_value = (bu_ptr->figure_cnt * 10) + bu_ptr->melee;
                if(target_value > highest_value)
                {
                    highest_value = target_value;
                    selected_target_idx = battle_unit_idx;
                }
            }
            break;
        case scc_Resistable_Spell: /* Combat Enchantments / Debuffs */
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                bu_ptr = &battle_units[battle_unit_idx];
                if(bu_ptr->controller_idx == player_idx) continue;
                if(bu_ptr->status != bus_Active) continue;
                if(bu_ptr->combat_effects & spell_data_table[spell_idx].enchantments) continue;
                if(bu_ptr->Attribs_1 & USA_IMMUNITY_MAGIC) continue;
                if(spell_data_table[spell_idx].magic_realm == sbr_Sorcery && (bu_ptr->Attribs_1 & USA_IMMUNITY_ILLUSION)) continue;
                if(spell_data_table[spell_idx].magic_realm == sbr_Death && (bu_ptr->Attribs_1 & USA_IMMUNITY_DEATH)) continue;
                enchantments = _UNITS[bu_ptr->unit_idx].enchantments;
                enchantments |= bu_ptr->enchantments;
                enchantments |= bu_ptr->item_enchantments;
                if(enchantments & UE_RIGHTEOUSNESS)
                {
                    if(spell_data_table[spell_idx].magic_realm == sbr_Chaos || spell_data_table[spell_idx].magic_realm == sbr_Death) continue;
                }
                if(spell_idx == spl_Creature_Binding && bu_ptr->race < rt_Arcane) continue;
                if(spell_idx == spl_Shatter)
                {
                    if(bu_ptr->melee <= 1 && bu_ptr->ranged <= 1) continue;
                }
                if(!Target_Is_Visible(battle_unit_idx)) continue;
                spell_resistance_modifier = Spell_Resistance_Modifier(spell_idx);
                unit_resistance = Combat_Effective_Resistance(battle_units[battle_unit_idx], spell_data_table[spell_idx].magic_realm);
                unit_resistance += spell_resistance_modifier;
                if(unit_resistance >= 10) continue;
                target_value = (Effective_Battle_Unit_Strength(battle_unit_idx) * (10 - unit_resistance) + 9) / 10;
                if(target_value > highest_value)
                {
                    highest_value = target_value;
                    selected_target_idx = battle_unit_idx;
                }
            }
            break;
        case scc_Mundane_Curse: /* Resistance-based Debuffs */
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                bu_ptr = &battle_units[battle_unit_idx];
                if(bu_ptr->Attribs_1 & USA_IMMUNITY_MAGIC) continue;
                battle_unit_enchantments = bu_ptr->combat_effects;
                if(battle_unit_enchantments & spell_data_table[spell_idx].enchantments) continue;
                if(spell_data_table[spell_idx].magic_realm == sbr_Sorcery && (bu_ptr->Attribs_1 & USA_IMMUNITY_ILLUSION)) continue;
                enchantments = _UNITS[bu_ptr->unit_idx].enchantments;
                enchantments |= bu_ptr->enchantments;
                enchantments |= bu_ptr->item_enchantments;
                if(enchantments & UE_RIGHTEOUSNESS)
                {
                    if(spell_data_table[spell_idx].magic_realm == sbr_Chaos || spell_data_table[spell_idx].magic_realm == sbr_Death) continue;
                }
                if(bu_ptr->controller_idx == player_idx) continue;
                if(bu_ptr->status != bus_Active) continue;
                if(bu_ptr->race >= rt_Arcane) continue;
                if(!Target_Is_Visible(battle_unit_idx)) continue;
                unit_resistance = Combat_Effective_Resistance(battle_units[battle_unit_idx], spell_data_table[spell_idx].magic_realm);
                if(unit_resistance >= 10) continue;
                target_value = (Effective_Battle_Unit_Strength(battle_unit_idx) * (10 - unit_resistance) + 9) / 10;
                if(target_value > highest_value)
                {
                    highest_value = target_value;
                    selected_target_idx = battle_unit_idx;
                }
            }
            break;
        case scc_Unresistable_Spell:
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                bu_ptr = &battle_units[battle_unit_idx];
                if(bu_ptr->controller_idx == player_idx) continue;
                if(bu_ptr->status != bus_Active) continue;
                if(spell_idx == spl_Web)
                {
                    if(bu_ptr->Abilities & UA_NONCORPOREAL) continue;
                    if(bu_ptr->Web_HP > 0) continue;
                }
                else
                {
                    battle_unit_enchantments = bu_ptr->combat_effects;
                    if(battle_unit_enchantments & spell_data_table[spell_idx].enchantments)
                    {
                        continue;

                    }

                    if(bu_ptr->Attribs_1 & USA_IMMUNITY_MAGIC) continue;
                    if(spell_data_table[spell_idx].magic_realm == sbr_Sorcery && (bu_ptr->Attribs_1 & USA_IMMUNITY_ILLUSION)) continue;
                }
                enchantments = _UNITS[bu_ptr->unit_idx].enchantments;
                enchantments |= bu_ptr->enchantments;
                enchantments |= bu_ptr->item_enchantments;
                if(enchantments & UE_RIGHTEOUSNESS)
                {
                    if(spell_data_table[spell_idx].magic_realm == sbr_Chaos || spell_data_table[spell_idx].magic_realm == sbr_Death) continue;
                }
                if(!Target_Is_Visible(battle_unit_idx)) continue;
                target_value = Effective_Battle_Unit_Strength(battle_unit_idx);
                if(spell_idx == spl_Web && (bu_ptr->Move_Flags & MV_FLYING))
                {
                    target_value += 2000;
                }
                if(target_value > highest_value)
                {
                    highest_value = target_value;
                    selected_target_idx = battle_unit_idx;
                }
            }
            break;
        case scc_Battlefield_Spell:
        case scc_Combat_Counter_Magic: /* Global Battlefield Enchantments */
            unit_resistance = (player_idx == _combat_attacker_player) ? 0 : 1;
            if(combat_enchantments[*(int16_t /* */ *)&spell_data_table[spell_idx].Param0 + unit_resistance] > 0)
            {
                return ST_UNDEFINED;
            }
            if(spell_idx == spl_True_Light || spell_idx == spl_Darkness)
            {
                selected_target_idx = AITP_DarknessLight(spell_idx);
            }
            else if(spell_idx == spl_Holy_Word)
            {
                selected_target_idx = AITP_HolyWord(player_idx);
            }
            else
            {
                selected_target_idx = 99;
            }
            break;
        case scc_Special_Spell: /* Special Target Pickers */
        {
            if(spell_idx == spl_Healing)
            {
                selected_target_idx = AITP_Healing(player_idx);
            }
            if(spell_idx == spl_Mass_Healing)
            {
                selected_target_idx = AITP_Healing(player_idx);
                if(selected_target_idx > ST_UNDEFINED)
                {
                    selected_target_idx = 99;
                }
            }
            if(spell_idx == spl_Warp_Wood)
            {
                selected_target_idx = AITP_WarpWood(player_idx);
            }
            if(spell_idx == spl_Warp_Creature)
            {
                selected_target_idx = AITP_WarpCreature(player_idx);
            }
            if(spell_idx == spl_Earth_To_Mud)
            {
                selected_target_idx = AITP_EarthToMud(player_idx, target_wx, target_wy);
            }
            if(spell_idx == spl_Disrupt)
            {
                selected_target_idx = AITP_Disrupt(player_idx, target_wx, target_wy);
            }
            if(spell_idx == spl_Recall_Hero)
            {
                selected_target_idx = AITP_RecallHero(player_idx);
            }
            if(spell_idx == spl_Cracks_Call)
            {
                selected_target_idx = AITP_CracksCall(player_idx, target_wx, target_wy);
            }
            if(spell_idx == spl_Raise_Dead)
            {
                selected_target_idx = 99;
                *target_wx = 0;
                *target_wy = 0;
            }
        } break;
        case scc_Dispels:
        {
            selected_target_idx = AITP_DispelMagic(player_idx);
        } break;
        case scc_Summoning: /* Unit Summoning (Find valid square) */
            if(_units == 1000) return ST_UNDEFINED;
            unit_resistance = 0;
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                if(battle_units[battle_unit_idx].controller_idx == player_idx && battle_units[battle_unit_idx].status == bus_Active)
                {
                    unit_resistance++;
                }
            }
            if(unit_resistance >= 9 || _combat_total_unit_count >= 36) return ST_UNDEFINED;
            unit_resistance = 0;
            do {
                if(player_idx == _combat_attacker_player)
                {
                    *target_wx = 14 - Random(3);
                }
                else
                {
                    *target_wx = 7 + Random(3);
                }
                *target_wy = 8 + Random(3);
                unit_resistance++;
                /* Check square validity via g_combat_grid_action_map lookup */
                /* OGBUG: should signal failure when all 200 rolls land on an occupied square; instead the loop falls out with the last rejected coordinates still in *target_wx / *target_wy and the caller is told 99 (target found) regardless */
                if(g_combat_grid_action_map[*target_wy][*target_wx] < 0)
                {
                    break;
                }
            } while(unit_resistance < 200);
            selected_target_idx = 99;
            break;
        case scc_Disenchants:
        {
            selected_target_idx = 99;
        } break;
        default:
        {
            /* loc_8B1E4 - Do nothing */
        } break;
    }
    return selected_target_idx;
}


// WZD o111p02
/* Returns 99 if valid targets exist, else -1 */
/* BUG: Assumes realm exclusivity, doesn't check allegiance */
int16_t AITP_DarknessLight(int16_t spell_idx)
{
    int16_t niu_local_variable = 0;
    int16_t selected_target_idx = 0;
    int16_t itr = 0;
    selected_target_idx = ST_UNDEFINED;
    if(spell_idx == spl_True_Light)
    {
        niu_local_variable = 1;
    }
    else
    {
        niu_local_variable = 0;
    }
    for(itr = 0; itr < _combat_total_unit_count; itr++)
    {
        if(battle_units[itr].race == rt_Life || battle_units[itr].race == rt_Death)
        {
            if(battle_units[itr].status == bus_Active)
            {
                selected_target_idx = 99;
            }
        }
    }
    return selected_target_idx;
}


// WZD o111p03
int16_t AITP_Healing(int16_t player_idx)
{
    int16_t selected_target_idx = 0;
    int16_t target_value = 0;
    int16_t highest_value = 0;
    int16_t battle_unit_idx = 0;
    struct s_BATTLE_UNIT * bu_ptr = NULL;
    highest_value = 0;
    selected_target_idx = ST_UNDEFINED;
    for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
    {
        bu_ptr = &battle_units[battle_unit_idx];
        if(bu_ptr->controller_idx != player_idx) continue;
        if(bu_ptr->status != bus_Active) continue;
        if(bu_ptr->race == rt_Death) continue;  /* Death creatures cannot be healed */
        target_value = (((bu_ptr->figure_max - bu_ptr->figure_cnt) * bu_ptr->hits) + bu_ptr->front_figure_damage);
        if(target_value > 2)
        {
            target_value = (target_value * (bu_ptr->melee + bu_ptr->ranged));
        }
        else
        {
            target_value = 0;
        }
        if(target_value > highest_value)
        {
            highest_value = target_value;
            selected_target_idx = battle_unit_idx;
        }
    }
    return selected_target_idx;
}


// WZD o111p04
int16_t AITP_WarpWood(int16_t player_idx)
{
    int32_t enchantments = 0;
    int16_t selected_target_idx = 0;
    int16_t highest_value = 0;
    int16_t target_value = 0;
    int16_t battle_unit_idx = 0;
    struct s_BATTLE_UNIT * bu_ptr = NULL;
    selected_target_idx = ST_UNDEFINED;
    highest_value = ST_UNDEFINED;
    for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
    {
        bu_ptr = &battle_units[battle_unit_idx];
        if(bu_ptr->Attribs_1 & USA_IMMUNITY_MAGIC) continue;
        enchantments = _UNITS[bu_ptr->unit_idx].enchantments;
        enchantments |= bu_ptr->enchantments;
        enchantments |= bu_ptr->item_enchantments;
        if(enchantments & UE_RIGHTEOUSNESS) continue;
        if(bu_ptr->controller_idx == player_idx) continue;
        if(bu_ptr->status != bus_Active) continue;
        if(!Target_Is_Visible(battle_unit_idx)) continue;
        if((bu_ptr->ranged_type / 10) == rag_Missile)
        {
            target_value = ((bu_ptr->ranged * bu_ptr->ammo) * bu_ptr->figure_cnt);
        }
        else
        {
            target_value = -10;
        }
        if(target_value > highest_value)
        {
            highest_value = target_value;
            selected_target_idx = battle_unit_idx;
        }
    }
    return selected_target_idx;
}


// WZD o111p05
int16_t AITP_WarpCreature(int16_t player_idx)
{
    int32_t enchantments = 0;
    int16_t selected_target_idx = 0;
    int16_t highest_value = 0;
    int16_t target_value = 0;
    int16_t unit_resist = 0;
    int16_t battle_unit_idx = 0;
    struct s_BATTLE_UNIT * bu_ptr = NULL;
    selected_target_idx = ST_UNDEFINED;
    highest_value = ST_UNDEFINED;
    for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
    {
        bu_ptr = &battle_units[battle_unit_idx];
        if(bu_ptr->Attribs_1 & USA_IMMUNITY_MAGIC) continue;
        enchantments = _UNITS[bu_ptr->unit_idx].enchantments;
        enchantments |= bu_ptr->enchantments;
        enchantments |= bu_ptr->item_enchantments;
        if(enchantments & UE_RIGHTEOUSNESS) continue;
        if(bu_ptr->combat_effects & bue_Warped_Attack) continue;
        if(bu_ptr->combat_effects & bue_Warped_Defense) continue;
        if(bu_ptr->combat_effects & bue_Warped_Resist) continue;
        if(bu_ptr->controller_idx == player_idx) continue;
        if(bu_ptr->status != bus_Active) continue;
        if(bu_ptr->race >= rt_Arcane) continue;  /* Fantastic creatures are exempt */
        if(!Target_Is_Visible(battle_unit_idx)) continue;
        unit_resist = Combat_Effective_Resistance(battle_units[battle_unit_idx], spell_data_table[spl_Warp_Creature].magic_realm);
        if(unit_resist >= 10) continue;
        /* attack value scaled by the failure chance of the resistance roll */
        target_value = (((((bu_ptr->melee + bu_ptr->ranged) * bu_ptr->figure_cnt) * (10 - unit_resist)) + 9) / 10);
        if(target_value > highest_value)
        {
            highest_value = target_value;
            selected_target_idx = battle_unit_idx;
        }
    }
    return selected_target_idx;
}


// WZD o111p06
int16_t AITP_Disintegrate(int16_t player_idx)
{
    int16_t selected_target_idx = 0;
    int16_t highest_value = 0;
    int16_t effective_resist = 0;
    uint32_t enchantments = 0;
    int16_t unit_idx = 0;
    int16_t target_value = 0;
    selected_target_idx = ST_UNDEFINED;
    highest_value = -1;
    for(unit_idx = 0; unit_idx < _combat_total_unit_count; unit_idx++)
    {
        if((battle_units[unit_idx].Attribs_1 & USA_IMMUNITY_MAGIC) != 0)
        {
            continue;
        }
        enchantments = _UNITS[battle_units[unit_idx].unit_idx].enchantments | battle_units[unit_idx].enchantments | battle_units[unit_idx].item_enchantments;
        if((enchantments & UE_RIGHTEOUSNESS) != 0)  /* provides complete immunity from all death and chaos magic spells */
        {
            continue;
        }
        if(battle_units[unit_idx].controller_idx == player_idx)
        {
            continue;
        }
        if(battle_units[unit_idx].status != bus_Active)
        {
            continue;
        }
        if(!Target_Is_Visible(unit_idx))
        {
            continue;
        }
        effective_resist = Combat_Effective_Resistance(battle_units[unit_idx], spell_data_table[spl_Disintegrate].magic_realm);
        if(effective_resist > 10)
        {
            continue;
        }
        target_value = ((battle_units[unit_idx].melee + battle_units[unit_idx].ranged) * battle_units[unit_idx].figure_cnt);
        if(target_value > highest_value)
        {
            highest_value = target_value;
            selected_target_idx = unit_idx;
        }
    }
    return selected_target_idx;
}


// WZD o111p07
/*
OGBUG:  the attacker/defender branch fails to swap: both branches assign own = attacker and enemy = defender realm data, so a defending caster evaluates with the attacker's realms
OGBUG:  the enemy-side eligibility test is a SIGNED compare of the combined enchantments, so a unit whose only enchantment is Invulnerability (bit 31) reads as "no enchantments" and is only eligible via the Haste/Binding/Possession/Confusion effects (the "ignores Invulnerability" bug noted at the dispatch call site)
OGBUG:  the combined enchantments is missing item_enchantments
*/
/* BUGs: Dispel confusion on own units, ignores Invulnerability except for some conditions */
int16_t AITP_DispelMagic(int16_t player_idx)
{
    int16_t niu_realm_value = 0;         /* NIU after assignment */
    int16_t enemy_spell_realms = 0;      /* NIU after assignment */
    int16_t own_spell_realms = 0;
    int16_t * own_unit_realms = NULL;    /* NIU after assignment */
    int16_t * enemy_unit_realms = NULL;  /* NIU after assignment */
    int32_t enchantments = 0;
    int16_t highest_value = 0;
    int16_t selected_target_idx = 0;
    int16_t target_value = 0;
    int16_t battle_unit_idx = 0;
    struct s_BATTLE_UNIT * bu_ptr = NULL;
    highest_value = 0;
    selected_target_idx = ST_UNDEFINED;
    /* OGBUG  both branches are identical - the defender branch was never swapped */
    if(player_idx == _combat_attacker_player)
    {
        enemy_spell_realms = g_ai_combat_defender_realm_flags;
        enemy_unit_realms = &g_ai_combat_defender_unit_realms[0];
        own_spell_realms = g_ai_combat_attacker_realm_flags;
        own_unit_realms = &g_ai_combat_attacker_unit_realms[0];
    }
    else
    {
        own_spell_realms = g_ai_combat_attacker_realm_flags;
        own_unit_realms = &g_ai_combat_attacker_unit_realms[0];
        enemy_spell_realms = g_ai_combat_defender_realm_flags;
        enemy_unit_realms = &g_ai_combat_defender_unit_realms[0];
    }
    niu_realm_value = g_ai_combat_unset_realm_flags;
    for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
    {
        target_value = ST_UNDEFINED;
        bu_ptr = &battle_units[battle_unit_idx];
        enchantments = bu_ptr->enchantments;
        enchantments |= _UNITS[bu_ptr->unit_idx].enchantments;
        /* Phase 1: own unit carrying combat debuffs - value dispelling them off of it */
        if(
            (bu_ptr->status == bus_Active)
            &&
            (bu_ptr->controller_idx == player_idx)
            &&
            ((bu_ptr->combat_effects & (bue_Vertigo | bue_Confusion | bue_Whirlwind | bue_Mind_Storm | bue_Shatter | bue_Weakness | bue_Black_Sleep | bue_Warped_Attack | bue_Warped_Defense | bue_Warped_Resist | bue_Mind_Twist | bue_NoEffect)) != 0)
        )
        {
            target_value = Effective_Battle_Unit_Strength(battle_unit_idx);
            if(bu_ptr->combat_effects & bue_Vertigo) target_value += 20;
            if((bu_ptr->combat_effects & bue_Confusion) && (bu_ptr->confusion_state != 2)) target_value += 30;
            if(bu_ptr->combat_effects & bue_Mind_Storm) target_value += 40;
            if(bu_ptr->combat_effects & bue_Shatter) target_value += 10;
            if(bu_ptr->combat_effects & bue_Weakness) target_value += 10;
            if(bu_ptr->combat_effects & bue_Black_Sleep) target_value += 40;
            if(bu_ptr->combat_effects & bue_Warped_Attack) target_value += 15;
            if(bu_ptr->combat_effects & bue_Warped_Defense) target_value += 10;
            if(bu_ptr->combat_effects & bue_Warped_Resist) target_value += 10;
            if(bu_ptr->combat_effects & bue_Mind_Twist) target_value += 10;
        }
        /* Phase 2: enemy unit carrying enchantments / beneficial effects - value stripping them */
        if(
            (bu_ptr->status == bus_Active)
            &&
            (bu_ptr->controller_idx != player_idx)
        )
        {
            if(
                (enchantments > 0)  /* OGBUG  signed - Invulnerability-only reads as "no enchantments" */
                ||
                ((bu_ptr->combat_effects & (bue_Haste | bue_Creature_Binding | bue_Possession)) != 0)
                ||
                (((bu_ptr->combat_effects & bue_Confusion) != 0) && (bu_ptr->confusion_state == 2))
            )
            {
                if(!Target_Is_Visible(battle_unit_idx)) continue;  /* skips the Phase 3 compare, as in the Dasm */
                target_value = Effective_Battle_Unit_Strength(battle_unit_idx);
                if(enchantments & UE_IMMOLATION) target_value += 20;
                if((bu_ptr->combat_effects & bue_Confusion) && (bu_ptr->confusion_state != 2)) target_value += 30;
                if(enchantments & UE_GUARDIAN_WIND) target_value += 10;
                if(enchantments & UE_CLOAK_OF_FEAR) target_value += 10;
                if(enchantments & UE_WRAITH_FORM) target_value += 10;
                if((enchantments & UE_ELEMENTAL_ARMOR) && (((own_spell_realms & 2) != 0) || ((own_spell_realms & 0) != 0))) target_value += 20;  /* OGBUG  `& 0` never passes */
                if(enchantments & UE_STONE_SKIN) target_value += 10;
                if(enchantments & UE_IRON_SKIN) target_value += 20;
                if(enchantments & UE_SPELL_LOCK) target_value -= 10;
                if(enchantments & UE_INVISIBILITY) target_value += 10;
                if(enchantments & UE_MAGIC_IMMUNITY) target_value += 25;
                if((enchantments & UE_TRUE_SIGHT) && ((own_spell_realms & 1) != 0)) target_value += 30;
                if(enchantments & UE_LION_HEART) target_value += 30;
                if((enchantments & UE_RIGHTEOUSNESS) && (((own_spell_realms & 2) != 0) || ((own_spell_realms & 8) != 0))) target_value += 30;
                if(enchantments & UE_INVULNERABILITY) target_value += 40;
                if(bu_ptr->combat_effects & bue_Creature_Binding) target_value += 70;
                if(bu_ptr->combat_effects & bue_Possession) target_value += 70;
            }
        }
        /* Phase 3: keep the best-scoring unit */
        if(target_value > highest_value)
        {
            highest_value = target_value;
            selected_target_idx = battle_unit_idx;
        }
    }
    return selected_target_idx;
}


// WZD o111p08
/*
IDA Group Colors
    scc_Summoning                   ( 0)    #24 reddish-brown
    scc_Unit_Enchantment            ( 1)    #43 pea green
    scc_City_Enchantment_Positive   ( 2)    #14 blueish lighter
    scc_City_Enchantment_Negative   ( 3)    #14 blueish lighter
    scc_Direct_Damage_Fixed         ( 4)    #32 purple
    scc_Special_Spell               ( 5)  
    scc_Global_Enchantment          ( 9)    #13 ~ blue, greyish/greenish
    scc_Crafting_Spell              (11)    #17 mauve
    scc_Battlefield_Spell           (10,21) #11 green-yellow
    scc_Combat_Counter_Magic        (10,21) #11 green-yellow
    scc_Direct_Damage_Variable      (22)    #31 redish purple
*/
void Combat_Cast_Apply_Spell_Effect(int16_t spell_idx, int16_t target_idx, int16_t caster_idx, int16_t target_cgx, int16_t target_cgy, int16_t tscc, int16_t anims_on, int16_t unused1, int16_t unused2)
{
    int16_t Not_Moved_Yet = 0;
    int16_t Moves_Left = 0;
    int16_t damage_types[3] = { 0, 0, 0 };
    uint32_t enchantments = 0;
    int16_t resistance_modifier = 0;
    int16_t figure_count = 0;
    int16_t did_create_unit = 0;
    int16_t player_idx = 0;
    int16_t itr = 0;
    int16_t resist_fails = 0;
    int16_t combat_enchantment_index = 0;
    if(caster_idx >= CASTER_IDX_BASE)
    {
        player_idx = (caster_idx - CASTER_IDX_BASE);
    }
    else
    {
        player_idx = battle_units[caster_idx].controller_idx;
    }
    /* OGBUG:  need to range check target_idx, can be -1, -2, 99; `if((target_idx >= 0) && (target_idx <= MAX_BATTLE_UNIT_COUNT))` */
    /* OGBUG: should use target_idx, not itr */
    enchantments = (_UNITS[battle_units[target_idx].unit_idx].enchantments | battle_units[itr].enchantments | battle_units[itr].item_enchantments);
    resistance_modifier = Spell_Resistance_Modifier(spell_idx);
    if(
        (caster_idx <= MAX_BATTLE_UNIT_COUNT)
        &&
        (_UNITS[battle_units[caster_idx].unit_idx].Hero_Slot > ST_UNDEFINED)
    )
    {
        for(itr = 0; itr < NUM_HERO_ITEM_SLOTS; itr++)
        {
            /* OGBUG: this may not be the hero's original owner, should use _UNITS[].owner_idx */
            if(_players[player_idx].Heroes[_UNITS[battle_units[caster_idx].unit_idx].Hero_Slot].Items[itr] > ST_UNDEFINED)
            {
                // NON SPELL-SPECIFIC ARTIFACT ENCHANTMENTS
                // Spell Save **
                // ** Each -1 to Spell Save decreases the opponent’s ability to resist this unit’s spell attacks against it by 10%
                //      (i.e., it negates one of the enemy unit’s crosses).
                resistance_modifier -= _ITEMS[_players[player_idx].Heroes[_UNITS[battle_units[caster_idx].unit_idx].Hero_Slot].Items[itr]].spell_save;
            }
        }
    }
    for(itr = 0; itr < 3; itr++)
    {
        damage_types[itr] = 0;
    }
    switch(spell_data_table[spell_idx].type)
    {
        case scc_Summoning:
        {
            did_create_unit = Create_Unit(spell_data_table[spell_idx].unit_type, player_idx, 0, 0, 9, 2000);
            if(did_create_unit == ST_TRUE)
            {
                Prepare_Battle_Unit_Summons(player_idx, (_units - 1), target_cgx, target_cgy);
                Battle_Unit_Summon_Animation((_combat_total_unit_count - 1), target_cgx, target_cgy, spell_idx, caster_idx);
            }
        } break;
        case scc_Unit_Enchantment:
        case scc_Unit_Enchantment_Normal_Only:
        {
            Combat_Spell_Animation(target_cgx, target_cgy, spell_idx, player_idx, anims_on, caster_idx);
            if(spell_idx == spl_Haste)
            {
                battle_units[target_idx].combat_effects |= bue_Haste;
            }
            else
            {
                battle_units[target_idx].enchantments |= spell_data_table[spell_idx].enchantments;
            }
            Set_Page_Off();
            Combat_Screen_Draw();
            PageFlip_FX();
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
            Battle_Unit_Regular_Stats(&battle_units[target_idx]);
            Battle_Unit_Special_Stats(&battle_units[target_idx]);
            Not_Moved_Yet = ST_FALSE;  /* OGBUG: clears the flag right before testing it, so the ST_TRUE branch is unreachable */
            if(Not_Moved_Yet == ST_TRUE)
            {
                battle_units[target_idx].movement_points = Battle_Unit_Moves2(target_idx);
            }
            else
            {
                battle_units[target_idx].movement_points = Moves_Left;
            }
        } break;
        case scc_City_Enchantment_Positive:
        {
            Wall_Rise(spell_idx, caster_idx);
        } break;
        case scc_Direct_Damage_Fixed:
        {
            Combat_Spell_Animation(target_cgx, target_cgy, spell_idx, player_idx, anims_on, caster_idx);
            Compute_Battle_Unit_Damage_From_Spell(spell_idx, target_idx, &damage_types[0], 0);
            Battle_Unit_Commit_Damage(target_idx, &damage_types[0]);
            Set_Page_Off();
            Combat_Screen_Draw();
            PageFlip_FX();
        } break;
        case scc_Special_Spell:
        {
            if(
                (spell_idx != spl_Wall_Of_Stone)
                &&
                (spell_idx != spl_Raise_Dead)
                &&
                (spell_idx != spl_Animate_Dead)
            )
            {
                Combat_Spell_Animation(target_cgx, target_cgy, spell_idx, player_idx, anims_on, caster_idx);
            }
            /* OGBUG: this was moved to scc_City_Enchantment_Positive */
            if(spell_idx == spl_Wall_Of_Stone)
            {
                Wall_Rise(spl_Wall_Of_Stone, caster_idx);
            }
            if(spell_idx == spl_Warp_Wood)
            {
                battle_units[target_idx].ammo = 0;
                battle_units[target_idx].ranged_type = rat_UNDEF;
                Moves_Left = Battle_Unit_Moves2(target_idx);
                Not_Moved_Yet = ST_FALSE;
                if(battle_units[target_idx].movement_points == (int8_t)Moves_Left)
                {
                    Not_Moved_Yet = ST_TRUE;
                }
                else
                {
                    Moves_Left = battle_units[target_idx].movement_points;
                }
                Battle_Unit_Regular_Stats(&battle_units[target_idx]);
                Battle_Unit_Special_Stats(&battle_units[target_idx]);
                if(Not_Moved_Yet == ST_TRUE)
                {
                    battle_units[target_idx].movement_points = (int8_t)Battle_Unit_Moves2(target_idx);
                }
                else
                {
                    battle_units[target_idx].movement_points = (int8_t)Moves_Left;
                }
            }
            if(spell_idx == spl_Healing)
            {
                Battle_Unit_Heal(target_idx, 5, ST_FALSE);
            }
            if(spell_idx == spl_Creature_Binding)
            {
                resist_fails = Combat_Resistance_Check(battle_units[target_idx], resistance_modifier, spell_data_table[spell_idx].magic_realm);
                if(resist_fails > 0)
                {
                    battle_units[target_idx].combat_effects |= bue_Creature_Binding;
                    if(battle_units[target_idx].controller_idx == _combat_attacker_player)
                    {
                        battle_units[target_idx].controller_idx = (int8_t)_combat_defender_player;
                    }
                    else
                    {
                        battle_units[target_idx].controller_idx = (int8_t)_combat_attacker_player;
                    }
                }
            }
            if(spell_idx == spl_Warp_Creature)
            {
                resist_fails = Combat_Resistance_Check(battle_units[target_idx], resistance_modifier, spell_data_table[spell_idx].magic_realm);
                if(resist_fails > 0)
                {
                    Apply_Warp_Creature(target_idx);
                }
                Moves_Left = Battle_Unit_Moves2(target_idx);
                Not_Moved_Yet = ST_FALSE;
                if(battle_units[target_idx].movement_points == (int8_t)Moves_Left)
                {
                    Not_Moved_Yet = ST_TRUE;
                }
                else
                {
                    Moves_Left = battle_units[target_idx].movement_points;
                }
                Battle_Unit_Regular_Stats(&battle_units[target_idx]);
                Battle_Unit_Special_Stats(&battle_units[target_idx]);
                if(Not_Moved_Yet == ST_TRUE)
                {
                    battle_units[target_idx].movement_points = (int8_t)Battle_Unit_Moves2(target_idx);
                }
                else
                {
                    battle_units[target_idx].movement_points = (int8_t)Moves_Left;
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
                Magic_Vortex_Create(player_idx, target_cgx, target_cgy);
            }
            if(spell_idx == spl_Earth_To_Mud)
            {
                Apply_Earth_To_Mud(target_cgx, target_cgy);
            }
            if(spell_idx == spl_Cracks_Call)
            {
                Apply_Cracks_Call(target_cgx, target_cgy);
            }
            if(spell_idx == spl_Disrupt)
            {
                battlefield->walls[(target_cgy - 10)][(target_cgx - 5)] = 2;
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
        case scc_Battlefield_Spell:
        case scc_Combat_Counter_Magic:
        {
            Combat_Spell_Animation(target_cgx, target_cgy, spell_idx, player_idx, anims_on, caster_idx);
            // Combat Battlefield Enchantment  (NOT Combat Battlefield Instant)
            if(
                (spell_idx != spl_Flame_Strike)
                &&
                (spell_idx != spl_Holy_Word)
                &&
                (spell_idx != spl_Death_Spell)
                &&
                (spell_idx != spl_Call_Chaos)
                &&
                (spell_idx != spl_Mass_Healing)
            )
            {
                combat_enchantment_index = spell_data_table[spell_idx].ce_idx;
                if(player_idx != _combat_attacker_player)
                {
                    combat_enchantment_index++;
                }
                if(spell_idx == spl_Counter_Magic)
                {
                    combat_enchantments[combat_enchantment_index] = (int8_t)tscc;
                }
                else
                {
                    combat_enchantments[combat_enchantment_index] = ST_TRUE;
                }
                for(itr = 0; itr < _combat_total_unit_count; itr++)
                {
                    if(battle_units[itr].status == bus_Active)
                    {
                        Not_Moved_Yet = ST_FALSE;
                        Moves_Left = Battle_Unit_Moves2(itr);
                        if(battle_units[itr].movement_points == (int8_t)Moves_Left)
                        {
                            Not_Moved_Yet = ST_TRUE;
                        }
                        else
                        {
                            Moves_Left = battle_units[itr].movement_points;
                        }
                        Battle_Unit_Regular_Stats(&battle_units[itr]);
                        Battle_Unit_Special_Stats(&battle_units[itr]);
                        if(Not_Moved_Yet == ST_TRUE)
                        {
                            battle_units[itr].movement_points = (int8_t)Battle_Unit_Moves2(itr);
                        }
                        else
                        {
                            battle_units[itr].movement_points = (int8_t)Moves_Left;
                        }
                    }
                }
            }
        } break;
        case scc_Combat_Destroy_Unit:
        case scc_Combat_Banish:
        {
            // "If the target unit is a summoned creature, it also protects the bonds that keep it tied to the controlling wizard."
            if(
                !(
                    (
                        (spell_idx == spl_Banish)
                        ||
                        (spell_idx == spl_Dispel_Evil)
                    )
                    &&
                    ((enchantments & UE_SPELL_LOCK) != 0)
                )
            )
            {
                Combat_Spell_Animation(target_cgx, target_cgy, spell_idx, player_idx, anims_on, caster_idx);
                if(spell_idx == spl_Disintegrate)
                {
                    if((Combat_Effective_Resistance(battle_units[target_idx], sbr_Chaos) + resistance_modifier) < 10)
                    {
                        damage_types[2] = 200;
                        Battle_Unit_Commit_Damage(target_idx, &damage_types[0]);
                    }
                }
                else
                {
                    if(
                        (spell_idx == spl_Dispel_Evil)
                        &&
                        ((_UNITS[battle_units[target_idx].unit_idx].mutations & UM_UNDEAD) != 0)
                    )
                    {
                        resistance_modifier -= 5;
                    }
                    if(spell_idx == spl_Banish)
                    {
                        resistance_modifier -= ((tscc - spell_data_table[spl_Banish].casting_cost) / 15);
                    }
                    if(
                        !(
                            (spell_idx == spl_Petrify)
                            &&
                            ((battle_units[target_idx].Attribs_1 & USA_IMMUNITY_STONING) != 0)
                        )
                    )
                    {
                        figure_count = battle_units[target_idx].figure_cnt;
                        for(itr = 0; itr < figure_count; itr++)
                        {
                            resist_fails = Combat_Resistance_Check(battle_units[target_idx], resistance_modifier, spell_data_table[spell_idx].magic_realm);
                            if(resist_fails > 0)
                            {
                                damage_types[2] = battle_units[target_idx].hits;
                                Battle_Unit_Commit_Damage(target_idx, &damage_types[0]);
                            }
                        }
                    }
                }
            Set_Page_Off();
            Combat_Screen_Draw();
            PageFlip_FX();
            }
        } break;
        case scc_Resistable_Spell:  // 13  Black Sleep, Confusion, Creature Binding, Vertigo, Weakness
        case scc_Mundane_Curse:     // 16  Possession, Shatter
        {
            Combat_Spell_Animation(target_cgx, target_cgy, spell_idx, player_idx, anims_on, caster_idx);
            resist_fails = Combat_Resistance_Check(battle_units[target_idx], resistance_modifier, spell_data_table[spell_idx].magic_realm);
            if(resist_fails <= 0)
            {
                Set_Page_Off();
                Combat_Screen_Draw();
                PageFlip_FX();
            }
            else
            {
                battle_units[target_idx].combat_effects |= spell_data_table[spell_idx].ce_idx;  // e.g., bue_Black_Sleep
                if(
                    (spell_idx == spl_Possession)
                    ||
                    (spell_idx == spl_Creature_Binding)
                )
                {
                    if(battle_units[target_idx].controller_idx == _combat_attacker_player)
                    {
                        battle_units[target_idx].controller_idx = (int8_t)_combat_defender_player;
                    }
                    else
                    {
                        battle_units[target_idx].controller_idx = (int8_t)_combat_attacker_player;
                    }
                    if(battle_units[target_idx].controller_idx == HUMAN_PLAYER_IDX)
                    {
                        battle_units[target_idx].action = bua_Ready;
                    }
                }
                if(spell_idx == spl_Black_Sleep)
                {
                    battle_units[target_idx].action = bua_Finished;
                    battle_units[target_idx].movement_points = 0;
                }
                Set_Page_Off();
                Combat_Screen_Draw();
                PageFlip_FX();
                Moves_Left = Battle_Unit_Moves2(target_idx);
                Not_Moved_Yet = ST_FALSE;
                if(battle_units[target_idx].movement_points == (int8_t)Moves_Left)
                {
                    Not_Moved_Yet = ST_TRUE;
                }
                else
                {
                    Moves_Left = battle_units[target_idx].movement_points;
                }
                Battle_Unit_Regular_Stats(&battle_units[target_idx]);
                Battle_Unit_Special_Stats(&battle_units[target_idx]);
                if(Not_Moved_Yet == ST_TRUE)
                {
                    battle_units[target_idx].movement_points = (int8_t)Battle_Unit_Moves2(target_idx);
                }
                else
                {
                    battle_units[target_idx].movement_points = (int8_t)Moves_Left;
                }
            }
        } break;
        case scc_Unresistable_Spell:  // 14  Mind Storm, Web
        {
            if(
                (
                    ((battle_units[target_idx].Attribs_1 & USA_IMMUNITY_MAGIC) != 0)
                    &&
                    (spell_idx != spl_Web)
                )
                ||
                (
                    (
                        (
                            (spell_data_table[spell_idx].magic_realm == sbr_Chaos)
                            ||
                            (spell_data_table[spell_idx].magic_realm == sbr_Death)
                        )
                        &&
                        ((enchantments & UE_RIGHTEOUSNESS) != 0)
                    )
                )
                ||
                (
                    (spell_data_table[spell_idx].magic_realm == sbr_Sorcery)
                    &&
                    ((battle_units[target_idx].Attribs_1 & USA_IMMUNITY_ILLUSION) != 0)
                )
            )
            {
                Set_Page_Off();
                Combat_Screen_Draw();
                PageFlip_FX();
            }
            else
            {
                Combat_Spell_Animation(target_cgx, target_cgy, spell_idx, player_idx, anims_on, caster_idx);
                battle_units[target_idx].combat_effects |= spell_data_table[spell_idx].Param0;  // e.g., bue_Black_Sleep
                if(spell_idx == spl_Web)
                {
                    battle_units[target_idx].Web_HP = 12;
                    battle_units[target_idx].action = bua_Finished;
                    battle_units[target_idx].movement_points = 0;
                }
                Set_Page_Off();
                Combat_Screen_Draw();
                PageFlip_FX();
                Moves_Left = Battle_Unit_Moves2(target_idx);
                Not_Moved_Yet = ST_FALSE;
                if(battle_units[target_idx].movement_points == (int8_t)Moves_Left)
                {
                    Not_Moved_Yet = ST_TRUE;
                }
                else
                {
                    Moves_Left = battle_units[target_idx].movement_points;
                }
                Battle_Unit_Regular_Stats(&battle_units[target_idx]);
                Battle_Unit_Special_Stats(&battle_units[target_idx]);
                if(Not_Moved_Yet == ST_TRUE)
                {
                    battle_units[target_idx].movement_points = (int8_t)Battle_Unit_Moves2(target_idx);
                }
                else
                {
                    battle_units[target_idx].movement_points = (int8_t)Moves_Left;
                }
            }
        } break;
        case scc_Dispels:
        {
            Combat_Spell_Animation(target_cgx, target_cgy, spell_idx, player_idx, anims_on, caster_idx);
            _page_flip_effect = pfe_Dissolve;
            Set_Page_Off();
            Combat_Screen_Draw();
            PageFlip_FX();
            _page_flip_effect = pfe_None;  // ; this is done automatically already
            // ¿ BUGBUG  no runemaster for 'Dispel Magic' ?
            if(
                (caster_idx >= CASTER_IDX_BASE)
                &&
                (_players[caster_idx].runemaster > 0)
            )
            {
                tscc *= 2;
            }
            if(spell_idx == spl_Dispel_Magic_True)
            {
                tscc *= 3;
            }
            resistance_modifier = 0;
            Combat_Cast_Dispel(target_cgx, target_cgy, caster_idx, tscc, &resistance_modifier);
            // ; BUG: Dispel Magic DOES NOT use unit-based targeting, this value can contain any valid index or even 99
            Moves_Left = Battle_Unit_Moves2(target_idx);
            Not_Moved_Yet = ST_FALSE;
            if(battle_units[target_idx].movement_points == (int8_t)Moves_Left)
            {
                Not_Moved_Yet = ST_TRUE;
            }
            else
            {
                Moves_Left = battle_units[target_idx].movement_points;
            }
            Battle_Unit_Regular_Stats(&battle_units[target_idx]);
            Battle_Unit_Special_Stats(&battle_units[target_idx]);
            if(Not_Moved_Yet == ST_TRUE)
            {
                battle_units[target_idx].movement_points = (int8_t)Battle_Unit_Moves2(target_idx);
            }
            else
            {
                battle_units[target_idx].movement_points = (int8_t)Moves_Left;
            }
        } break;
        case scc_Disenchants:  // 19
        {
            Combat_Spell_Animation(target_cgx, target_cgy, spell_idx, player_idx, anims_on, caster_idx);
            _page_flip_effect = pfe_Dissolve;
            Set_Page_Off();
            Combat_Screen_Draw();
            PageFlip_FX();
            _page_flip_effect = pfe_None;  // ; this is done automatically already
            if(
                (caster_idx >= CASTER_IDX_BASE)
                &&
                (_players[caster_idx].runemaster > 0)
            )
            {
                tscc *= 2;
            }
            // BUGBUG  should be if *3 else *2, in previous block
            if(spell_idx == spl_Disenchant_True)
            {
                tscc *= 3;
            }
            Combat_Cast_Disenchant(caster_idx, tscc);
            // BUGBUG  doesn't check active?
            for(itr = 0; itr < _combat_total_unit_count; itr++)
            {
                Not_Moved_Yet = ST_FALSE;
                if(battle_units[itr].status == bus_Active)
                {
                    Moves_Left = Battle_Unit_Moves2(itr);
                    if(battle_units[itr].movement_points == (int8_t)Moves_Left)
                    {
                        Not_Moved_Yet = ST_TRUE;
                    }
                    else
                    {
                        Moves_Left = battle_units[itr].movement_points;
                    }
                    Battle_Unit_Regular_Stats(&battle_units[itr]);
                    Battle_Unit_Special_Stats(&battle_units[itr]);
                    if(Not_Moved_Yet == ST_TRUE)
                    {
                        battle_units[itr].movement_points = (int8_t)Battle_Unit_Moves2(itr);
                    }
                    else
                    {
                        battle_units[itr].movement_points = (int8_t)Moves_Left;
                    }
                }
            }
        } break;
        case scc_Direct_Damage_Variable:
        {
            Combat_Spell_Animation(target_cgx, target_cgy, spell_idx, player_idx, anims_on, caster_idx);
            if(spell_idx == spl_Life_Drain)
            {
                Apply_Life_Drain(target_idx, &damage_types[0], caster_idx, tscc);
            }
            // Fire Bolt, Ice Bolt, Lightning Bolt: +1 damage per extra mana spent
            if(
                (spell_idx == spl_Fire_Bolt)
                ||
                (spell_idx == spl_Ice_Bolt)
                ||
                (spell_idx == spl_Lightning_Bolt)
            )
            {
                Moves_Left = spell_data_table[spell_idx].strength;
                if(tscc > 0)
                {
                    Moves_Left += (tscc - spell_data_table[spell_idx].casting_cost);
                }
                Compute_Battle_Unit_Damage_From_Spell(spell_idx, target_idx, &damage_types[0], Moves_Left);
            }
            // Psionic_Blast: +1 damage per every 2 extra mana spent
            if(spell_idx == spl_Psionic_Blast)
            {
                Moves_Left = spell_data_table[spell_idx].strength;
                if(tscc > 0)
                {
                    Moves_Left += ((tscc - spell_data_table[spell_idx].casting_cost) / 2);
                }
                Compute_Battle_Unit_Damage_From_Spell(spell_idx, target_idx, &damage_types[0], Moves_Left);
            }
            // Fireball: +1 damage per every 3 extra mana spent
            if(spell_idx == spl_Fireball)
            {
                Moves_Left = spell_data_table[spell_idx].strength;
                if(tscc > 0)
                {
                    Moves_Left += ((tscc - spell_data_table[spell_idx].casting_cost) / 3);
                }
                Compute_Battle_Unit_Damage_From_Spell(spell_idx, target_idx, &damage_types[0], Moves_Left);
            }
            Battle_Unit_Commit_Damage(target_idx, &damage_types[0]);
            Set_Page_Off();
            Combat_Screen_Draw();
            PageFlip_FX();
        } break;
    }
    Combat_Screen_Draw();
    PageFlip_FX();
}


// WZD o111p09
// drake178: AITP_HolyWord()
/*
Holy Word target check: looks for any enemy Fantastic (race >= rt_Arcane) battle unit not protected by Spell Lock.
OGBUG  the status test is `<= bus_Active` (skip), so only NON-active units (recalled, fled, dead, ...) are ever examined - active enemies never qualify
OGBUG  does not check Magic Immunity
OGBUG  the Dasm never moves the result (DI) into AX before returning: when a target is found AX happens to be 0 (left over from the `and ax, 0` in the enchantment test), otherwise
       AX holds loop scratch that is in practice always >= 0.  Returning the intended found-flag (0 / 99) keeps the observable behavior: the targeter never rejects Holy Word.
*/
int16_t AITP_HolyWord(int16_t player_idx)
{
    int16_t found_flag = 0;       /* _DI_ */
    int16_t battle_unit_idx = 0;  /* _SI_ */
    struct s_BATTLE_UNIT * bu_ptr = NULL;

    found_flag = 0;

    for(battle_unit_idx = 0; ((battle_unit_idx < _combat_total_unit_count) && (found_flag == 0)); battle_unit_idx++)
    {
        bu_ptr = &battle_units[battle_unit_idx];

        if(bu_ptr->status <= bus_Active) continue;  /* OGBUG  skips active units */
        if(bu_ptr->controller_idx == player_idx) continue;
        if(bu_ptr->race < rt_Arcane) continue;
        if(bu_ptr->enchantments & UE_SPELL_LOCK) continue;
        if(_UNITS[bu_ptr->unit_idx].enchantments & UE_SPELL_LOCK) continue;

        found_flag = 99;
    }

    return found_flag;
}



/*
    WIZARDS.EXE  ovr113
*/

// WZD o113p01
/* OGBUG: drake178 observed that Torin The Chosen fails the race < rt_Arcane test and so is excluded from normal-unit-only spells; unverified here — race_type comes from UNITS.LBX at runtime, not from the source */
void Combat_Screen_Assign_Mouse_Images(void)
{
    int16_t pointer_offset = 0;
    int16_t cgy = 0;
    int16_t screen_y = 0;
    int16_t screen_x = 0;
    int16_t battle_unit_idx = 0;
    int16_t cgx = 0;
    int16_t itr = 0;
    _combat_mouse_grid[0].image_num = crsr_Finger;
    _scanned_battle_unit = ST_UNDEFINED;
    pointer_offset = 4;
    screen_x = (Pointer_X() + pointer_offset);
    screen_y = (Pointer_Y() + pointer_offset);
    frame_scanned_flag = ST_FALSE;
    if(screen_y <= (SCREEN_XMID_168 + pointer_offset))
    {
        cgx = Get_Combat_Grid_Cell_X(screen_x, screen_y);
        cgy = Get_Combat_Grid_Cell_Y(screen_x, screen_y);
        switch(g_combat_grid_action_map[cgy][cgx])
        {
             case -2:
            {
                /* OGBUG: frame_scanned_flag is only set in the default arm, so square-target spells (empty cell, -1 or -2) never light the targeting frame; the two negative cases should set it too */
                /* OGBUG: both empty-cell cases show crsr_CastBase for any target type below cstt_EnemyUnit, so COMBAT_CELL_NO_ACTION (-2) squares look castable; the click handler in Combat_Spell_Target_Screen() accepts them too */
                if(_combat_spell_target_type < cstt_EnemyUnit)
                {
                    _combat_mouse_grid[0].image_num =  crsr_CastBase;
                }
                else
                {
                    _combat_mouse_grid[0].image_num =  crsr_RedCross;
                }
            } break;
            case -1:
            {
                /* OGBUG: frame_scanned_flag is only set in the default arm, so square-target spells (empty cell, -1 or -2) never light the targeting frame; the two negative cases should set it too */
                /* OGBUG: both empty-cell cases show crsr_CastBase for any target type below cstt_EnemyUnit, so COMBAT_CELL_NO_ACTION (-2) squares look castable; the click handler in Combat_Spell_Target_Screen() accepts them too */
                if(_combat_spell_target_type < cstt_EnemyUnit)
                {
                    _combat_mouse_grid[0].image_num =  crsr_CastBase;
                }
                else
                {
                    _combat_mouse_grid[0].image_num =  crsr_RedCross;
                }
            } break;
            default:
            {
                frame_scanned_flag = ST_TRUE;
                frame_scanned_cgx = cgx;
                frame_scanned_cgy = cgy;
                frame_anim_cycle = ((frame_anim_cycle + 1) % 3);
                /* OGBUG: a wall square holds COMBAT_CELL_CITY_WALL (99) when a Wall Crusher is the active unit, and 99 is neither -1 nor -2, so it falls to default and battle_units[99] is read past the 36-slot array; should test for 99 alongside the two negative cells, not treat it as a unit index */
                battle_unit_idx = g_combat_grid_action_map[cgy][cgx];
                _scanned_battle_unit = battle_unit_idx;
                if(battle_units[battle_unit_idx].controller_idx != HUMAN_PLAYER_IDX)
                {
                    if(
                        (_combat_spell_target_type == cstt_EnemyUnit)
                        ||
                        (_combat_spell_target_type == cstt_Tile)
                        ||
                        (_combat_spell_target_type == cstt_DispelMagic)
                        ||
                        (
                            (_combat_spell_target_type == cstt_EnemyNU)
                            &&
                            (battle_units[battle_unit_idx].race < rt_Arcane)
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
                else  /* (battle_units[battle_unit_idx].controller_idx == HUMAN_PLAYER_IDX) */
                {
                    if(
                        (_combat_spell_target_type == cstt_FriendlyUnit)
                        ||
                        (_combat_spell_target_type == cstt_Tile)
                        ||
                        (_combat_spell_target_type == cstt_DispelMagic)
                        ||
                        (
                            (_combat_spell_target_type == cstt_FriendlyNU)
                            &&
                            (battle_units[battle_unit_idx].race < rt_Arcane)
                        )
                        ||
                        (
                            (_combat_spell_target_type ==cstt_FriendlyHero)
                            &&
                            (_UNITS[battle_units[battle_unit_idx].unit_idx].Hero_Slot > ST_UNDEFINED)
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
            } break;
        }
    }
    /* OGBUG: below y=172 the guard skips the grid scan, but the target-type overrides that follow still read cgx/cgy and force crsr_RedCross over the bottom GUI; they should be inside the guard */
    if(
        (_combat_spell_target_type == cstt_Tile_NoUnitA)
        &&
        (cgx < 11)
    )
    {
        _combat_mouse_grid[0].image_num = crsr_RedCross;
    }
    if(
        (_combat_spell_target_type == cstt_Tile_NoUnitD)
        &&
        (cgx > 10)
    )
    {
        _combat_mouse_grid[0].image_num = crsr_RedCross;
    }
    if(_combat_spell_target_type == cstt_Wall)
    {
        _combat_mouse_grid[0].image_num = crsr_RedCross;
        if(Combat_Grid_Cell_Has_City_Wall(cgx, cgy) != ST_FALSE)
        {
            _combat_mouse_grid[0].image_num = crsr_CastBase;
        }
    }
    if(_combat_spell_target_type == cstt_DispelMagic)
    {
        for(itr = 0; itr < _vortex_count; itr++)
        {
            if(
                (_vortexes[itr].cgx == cgx)
                &&
                (_vortexes[itr].cgy == cgy)
            )
            {
                _combat_mouse_grid[0].image_num = crsr_CastBase;
            }
        }
    }
    if(_combat_mouse_grid[0].image_num == crsr_CastBase)
    {
        _combat_mouse_grid[0].image_num = (crsr_CastAnim1 + g_spellbook_anim_stage);
    }
    g_spellbook_anim_stage = ((g_spellbook_anim_stage + 1) % 5);
    _combat_mouse_grid[0].center_offset = 2;
    _combat_mouse_grid[0].x1 = SCREEN_XMIN;
    _combat_mouse_grid[0].y1 = SCREEN_YMIN;
    _combat_mouse_grid[0].x2 = SCREEN_XMAX;
    _combat_mouse_grid[0].y2 = SCREEN_YMAX;
    Set_Mouse_List(1, &_combat_mouse_grid[0]);
}


// WZD o113p02
/*
~== SCastScr  Spell_Casting_Screen__WIP() |-> Spell_Casting_Screen_Assign_Mouse_Images()
*/
void Combat_Spell_Target_Screen_Draw(void)
{
    SAMB_ptr msg_panel_seg;
    Combat_Screen_Draw();
    Mark_Block(_screen_seg);
    // COMBTFX.LBX, 028  "MSGPANEL"
    msg_panel_seg = LBX_Reload_Next(cmbtfx_lbx_file__ovr113, 28, _screen_seg);
    Release_Block(_screen_seg);
    FLIC_Draw(238, 164, msg_panel_seg);
    Set_Outline_Color(240);
    Set_Alias_Color(227);
    Set_Font_Style_Shadow_Down(0, 0, 0, 0);
    Set_Font_Spacing_Width(1);
    Print_Paragraph(241, SCREEN_YBOT_CMBT, 75, GUI_NearMsgString, 0);
    Combat_Screen_Assign_Mouse_Images(); 
}


// WZD o113p03
int16_t Combat_Spell_Target_Screen(int16_t spell_idx, int16_t * target_cgx, int16_t * target_cgy)
{
    char spell_name[LEN_SPELL_NAME] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t battle_unit_idx = 0;
    uint32_t enchantments = 0;
    int16_t vortex_idx = 0;
    int16_t target_idx = 0;
    int16_t input_field_idx = 0;
    int64_t grid_sy = 0;    // DOMSDOS  int16_t grid_sy = 0;
    int64_t grid_sx = 0;    // DOMSDOS  int16_t grid_sx = 0;
    int16_t cancel_button_field = 0;
    int16_t combat_grid_field = 0;
    int16_t leave_screen = 0;
    Assign_Auto_Function(Combat_Spell_Target_Screen_Draw, 2);
    g_spellbook_anim_stage = 0;
    if(spell_idx == spl_NONE)
    {
        _combat_spell_target_type = cstt_Tile_NoUnit;
    }
    else
    {
        switch(spell_data_table[spell_idx].type)
        {
            case scc_Summoning:
            {
                if(_combat_attacker_player == HUMAN_PLAYER_IDX)
                {
                    _combat_spell_target_type = cstt_Tile_NoUnitA;
                }
                else
                {
                    _combat_spell_target_type = cstt_Tile_NoUnitD;
                }
            } // FALL-THROUGH
            case scc_Special_Spell:
            {
                if(spell_idx == spl_Healing)
                {
                    _combat_spell_target_type = cstt_FriendlyUnit;
                }
                if(
                    (spell_idx == spl_Raise_Dead)
                    ||
                    (spell_idx == spl_Animate_Dead)
                )
                {
                    if(_combat_attacker_player == HUMAN_PLAYER_IDX)
                    {
                        _combat_spell_target_type = cstt_Tile_NoUnitA;
                    }
                    else
                    {
                        _combat_spell_target_type = cstt_Tile_NoUnitD;
                    }
                }
                if(
                    (spell_idx == spl_Warp_Creature)
                    ||
                    (spell_idx == spl_Creature_Binding)
                )
                {
                    _combat_spell_target_type = cstt_EnemyUnit;
                }
                if(
                    (spell_idx == spl_Earth_To_Mud)
                    ||
                    (spell_idx == spl_Cracks_Call)
                )
                {
                    _combat_spell_target_type = cstt_Tile;
                }
                if(spell_idx == spl_Magic_Vortex)
                {
                    _combat_spell_target_type = cstt_Tile_NoUnit;
                }
                if(spell_idx == spl_Disrupt)
                {
                    _combat_spell_target_type = cstt_Wall;
                }
                if(spell_idx == spl_Recall_Hero)
                {
                    _combat_spell_target_type = cstt_FriendlyHero;
                }
                if(spell_idx == spl_Word_Of_Recall)
                {
                    _combat_spell_target_type = cstt_FriendlyUnit;
                }
            } break;
            case scc_Unit_Enchantment:
            {
                _combat_spell_target_type = cstt_FriendlyUnit;
            } break;
            case scc_Direct_Damage_Fixed:
            case scc_Combat_Destroy_Unit:
            case scc_Resistable_Spell:
            case scc_Unresistable_Spell:
            case scc_Direct_Damage_Variable:
            case scc_Combat_Banish:
            {
                _combat_spell_target_type = cstt_EnemyUnit;
            } break;
            case scc_Unit_Enchantment_Normal_Only:
            {
                _combat_spell_target_type = cstt_FriendlyNU;
            } break;
            case scc_Mundane_Curse:
            {
                _combat_spell_target_type = cstt_EnemyNU;
            } break;
            case scc_Dispels:
            {
                _combat_spell_target_type = cstt_DispelMagic;
            } break;
        }
    }
    Clear_Fields();
    combat_grid_field = Add_Grid_Field(0, 0, 1, 1, SCREEN_XMAX, SCREEN_YBOT_CMBT, &grid_sx, &grid_sy, ST_UNDEFINED);
    /* OGBUG: passes the address of the ESC string where a hotkey character is expected, so the key code compared against input is a pointer value and ESC never matches this button; should be str_hotkey_ESC__ovr113[0] (0x1B) */
    cancel_button_field = Add_Button_Field(263, 186, str_empty_string__ovr113, _cmbt_cancel_button_seg, (int16_t)(intptr_t)&str_hotkey_ESC__ovr113[0], ST_UNDEFINED);
    leave_screen = ST_FALSE;
    while(leave_screen == ST_FALSE)
    {
        stu_strcpy(GUI_NearMsgString, cnst_CmbSpellMsg1);  // "Select a target for a "
        _fstrcpy(spell_name, spell_data_table[spell_idx].name);
        stu_strcat(GUI_NearMsgString, spell_name);
        stu_strcat(GUI_NearMsgString, cnst_SpaceSpellDot_3);  // " spell."
        Mark_Time();
        input_field_idx = Get_Input();
        if(input_field_idx == cancel_button_field)
        {
            leave_screen = ST_TRUE;
            target_idx = 999;
        }
        if(input_field_idx == combat_grid_field)
        {
            *target_cgx = Get_Combat_Grid_Cell_X(((int16_t)grid_sx + 4), ((int16_t)grid_sy + 4));
            *target_cgy = Get_Combat_Grid_Cell_Y(((int16_t)grid_sx + 4), ((int16_t)grid_sy + 4));
            /* Disrupt */
            if(
                (_combat_spell_target_type == cstt_Wall)
                &&
                (Combat_Grid_Cell_Has_City_Wall(*target_cgx, *target_cgy) != ST_FALSE)
            )
            {
                /* OGBUG: leaves target_idx unassigned, so the caller gets whatever the slot held; should be 99 (COMBAT_CELL_CITY_WALL) like the tile paths below, not 999 (the cancel marker). Combat_Cast_Apply_Spell_Effect() then indexes battle_units[target_idx] unguarded */
                leave_screen = ST_TRUE;
                /* HACK */  target_idx = 999;   /* not the OG value - 999 makes the caller skip the cast rather than deref an arbitrary index */
                continue;
            }
            if(_combat_spell_target_type == cstt_DispelMagic)
            {
                for(vortex_idx = 0; vortex_idx < _vortex_count; vortex_idx++)
                {
                    if(
                        (_vortexes[vortex_idx].cgx == *target_cgx)
                        &&
                        (_vortexes[vortex_idx].cgy == *target_cgy)
                    )
                    {
                        leave_screen = ST_TRUE;
                    }
                }
            }
            target_idx = g_combat_grid_action_map[*target_cgy][*target_cgx];
            if(target_idx < 0)
            {
                if(
                    (_combat_spell_target_type == cstt_Tile_NoUnit)
                    ||
                    (_combat_spell_target_type == cstt_Tile)
                )
                {
                    leave_screen = ST_TRUE;
                    target_idx = 99;  /* OGBUG: accepts any target_idx < 0, so COMBAT_CELL_NO_ACTION (-2) squares pass as well as COMBAT_CELL_REACHABLE (-1); and neither *target_cgx nor *target_cgy is range-checked before the map read above, so a click outside the 21x22 grid reads out of bounds */
                }
                /* OGBUG: only checks which half of the field the square is on and rescans for an invisible occupant; never tests that the square is passable or inside the grid, so a summon can land on water, a wall segment, or an off-grid cell */
                if(
                    (_combat_spell_target_type == cstt_Tile_NoUnitD)
                    &&
                    (*target_cgx < 11)
                )
                {
                    leave_screen = ST_TRUE;
                    target_idx = 99;
                    for(battle_unit_idx = 0; ((battle_unit_idx < _combat_total_unit_count) && (leave_screen == ST_TRUE)); battle_unit_idx++)
                    {
                        if(
                            (battle_units[battle_unit_idx].status == bus_Active)
                            &&
                            (battle_units[battle_unit_idx].cgx == *target_cgx)
                            &&
                            (battle_units[battle_unit_idx].cgy == *target_cgy)
                        )
                        {
                            leave_screen = ST_FALSE;
                            LBX_Load_Data_Static(message_lbx_file__ovr113__1of2, 0, (SAMB_ptr)&GUI_NearMsgString[0], 88, 1, 150);
                            Warn1(GUI_NearMsgString);
                        }
                    }
                }
                /* OGBUG: only checks which half of the field the square is on and rescans for an invisible occupant; never tests that the square is passable or inside the grid, so a summon can land on water, a wall segment, or an off-grid cell */
                if(
                    (_combat_spell_target_type == cstt_Tile_NoUnitA)
                    &&
                    (*target_cgx >= 11)
                )
                {
                    leave_screen = ST_TRUE;
                    target_idx = 99;
                    for(battle_unit_idx = 0; ((battle_unit_idx < _combat_total_unit_count) && (leave_screen == ST_TRUE)); battle_unit_idx++)
                    {
                        if(
                            (battle_units[battle_unit_idx].status == bus_Active)
                            &&
                            (battle_units[battle_unit_idx].cgx == *target_cgx)
                            &&
                            (battle_units[battle_unit_idx].cgy == *target_cgy)
                        )
                        {
                            leave_screen = ST_FALSE;
                            LBX_Load_Data_Static(message_lbx_file__ovr113__1of2, 0, (SAMB_ptr)&GUI_NearMsgString[0], 88, 1, 150);  // "There is an invisible unit in that square."
                            Warn1(GUI_NearMsgString);
                        }
                    }
                }
            }
            else  /* (target_idx >= 0) */
            {
                enchantments = (_UNITS[battle_units[target_idx].unit_idx].enchantments | battle_units[target_idx].item_enchantments | battle_units[target_idx].enchantments);
                if(battle_units[target_idx].controller_idx != HUMAN_PLAYER_IDX)/* Target is Enemy */
                {
                    if(
                        (_combat_spell_target_type == cstt_EnemyUnit)
                        ||
                        (_combat_spell_target_type == cstt_Tile)
                        ||
                        (_combat_spell_target_type == cstt_DispelMagic)
                    )
                    {
                        leave_screen = ST_TRUE;
                    }
                    if(
                        (_combat_spell_target_type == cstt_EnemyNU)
                        &&
                        (battle_units[target_idx].race < rt_Arcane)
                    )
                    {
                        leave_screen = ST_TRUE;
                    }
                    if(spell_idx == spl_Creature_Binding)
                    {
                        if(battle_units[target_idx].race < rt_Arcane)
                        {
                            leave_screen = 0;
                            LBX_Load_Data_Static(message_lbx_file__ovr113__1of2, 0, (SAMB_ptr)&GUI_NearMsgString[0], 69, 1, 150);
                            _fstrcpy((char *)spell_name, (char *)spell_data_table[spell_idx].name);
                            stu_strcat(GUI_NearMsgString, spell_name);
                            Warn1(GUI_NearMsgString);
                        }
                    }
                    if(spell_idx == spl_Banish)
                    {
                        if(battle_units[target_idx].race < rt_Arcane)
                        {
                            leave_screen = 0;
                            LBX_Load_Data_Static(message_lbx_file__ovr113__1of2, 0, (SAMB_ptr)&GUI_NearMsgString[0], 69, 1, 150);
                            _fstrcpy((char *)spell_name, (char *)spell_data_table[spell_idx].name);
                            stu_strcat(GUI_NearMsgString, spell_name);
                            Warn1(GUI_NearMsgString);
                        }
                    }
                    if(spell_idx == spl_Star_Fires)
                    {
                        if(
                            battle_units[target_idx].race != rt_Chaos
                            &&
                            battle_units[target_idx].race != rt_Death
                        )
                        {
                            leave_screen = 0;
                            LBX_Load_Data_Static(message_lbx_file__ovr113__1of2, 0, (SAMB_ptr)&GUI_NearMsgString[0], 70, 1, 150);
                            Warn1(GUI_NearMsgString);
                        }
                    }
                    if(spell_idx == spl_Web)
                    {
                        if(battle_units[target_idx].Abilities & UA_NONCORPOREAL)
                        {
                            leave_screen = 0;
                            LBX_Load_Data_Static(message_lbx_file__ovr113__1of2, 0, (SAMB_ptr)&GUI_NearMsgString[0], 71, 1, 150);
                            Warn1(GUI_NearMsgString);
                        }
                    }
                    if(spell_idx == spl_Warp_Wood)
                    {
                        if((battle_units[target_idx].ranged_type / 10) != rag_Missile)
                        {
                            leave_screen = 0;
                            LBX_Load_Data_Static(message_lbx_file__ovr113__1of2, 0, (SAMB_ptr)&GUI_NearMsgString[0], 72, 1, 150);
                            Warn1(GUI_NearMsgString);
                        }
                    }
                    if(
                        spell_data_table[spell_idx].type == scc_Resistable_Spell
                        ||
                        spell_data_table[spell_idx].type == scc_Mundane_Curse
                        ||
                        spell_data_table[spell_idx].type == scc_Unresistable_Spell
                    )
                    {
                        if(spell_idx == spl_Web)
                        {
                            if(battle_units[target_idx].Web_HP > 0)
                            {
                                leave_screen = ST_FALSE;
                                _fstrcpy((char *)spell_name, (char *)spell_data_table[spell_idx].name);
                                stu_strcpy(GUI_NearMsgString, cnst_SpellError_1);
                                stu_strcat(GUI_NearMsgString, spell_name);
                                stu_strcat(GUI_NearMsgString, cnst_SpellError_2);
                                Warn1(GUI_NearMsgString);
                            }
                        }
                        else
                        {
                            if(((unsigned long)battle_units[target_idx].combat_effects & ((unsigned long)spell_data_table[spell_idx].Param0 | ((unsigned long)spell_data_table[spell_idx].Params2_3 << 16))) != 0)
                            {
                                leave_screen = ST_FALSE;
                                _fstrcpy((char *)spell_name, (char *)spell_data_table[spell_idx].name);
                                stu_strcpy(GUI_NearMsgString, cnst_SpellError_1);
                                stu_strcat(GUI_NearMsgString, spell_name);
                                stu_strcat(GUI_NearMsgString, cnst_SpellError_2);
                                Warn1(GUI_NearMsgString);
                            }
                            else
                            {
                                if(spell_data_table[spell_idx].magic_realm == sbr_Sorcery)
                                {
                                    if(battle_units[target_idx].Attribs_1 & USA_IMMUNITY_ILLUSION)
                                    {
                                        leave_screen = ST_FALSE;
                                        LBX_Load_Data_Static(message_lbx_file__ovr113__1of2, 0, (SAMB_ptr)&GUI_NearMsgString[0], 73, 1, 150);
                                        Warn1(GUI_NearMsgString);
                                    }
                                }
                                if(spell_data_table[spell_idx].magic_realm == sbr_Death)
                                {
                                    if(battle_units[target_idx].Attribs_1 & USA_IMMUNITY_DEATH)
                                    {
                                        leave_screen = ST_FALSE;
                                        Warn1(cnst_SpellError_3);
                                    }
                                }
                            }
                        }
                    }
                }
                else  /* Target is Friend */
                {
                    if(
                        (_combat_spell_target_type == cstt_FriendlyUnit)
                        ||
                        (_combat_spell_target_type == cstt_Tile)
                        ||
                        (_combat_spell_target_type == cstt_DispelMagic)
                    )
                    {
                        leave_screen = ST_TRUE;
                    }
                    if(_combat_spell_target_type == cstt_FriendlyNU)
                    {
                        if(battle_units[target_idx].race < rt_Arcane)
                        {
                            leave_screen = 1;
                        }
                    }
                    if(_combat_spell_target_type == cstt_FriendlyHero)
                    {
                        if(_UNITS[battle_units[target_idx].unit_idx].Hero_Slot > -1)
                        {
                            leave_screen = 1;
                        }
                    }
                    if(spell_idx == spl_Healing)
                    {
                        if(
                            battle_units[target_idx].race == rt_Death
                            ||
                            (_UNITS[battle_units[target_idx].unit_idx].mutations & UM_UNDEAD)
                        )
                        {
                            leave_screen = ST_FALSE;
                            LBX_Load_Data_Static(message_lbx_file__ovr113__1of2, 0, (SAMB_ptr)&GUI_NearMsgString[0], 74, 1, 150);
                            Warn1(GUI_NearMsgString);
                        }
                    }
                    if(
                        spell_data_table[spell_idx].type == scc_Unit_Enchantment
                        ||
                        spell_data_table[spell_idx].type == scc_Unit_Enchantment_Normal_Only
                    )
                    {
                        if(spell_idx == spl_Haste)
                        {
                            if(battle_units[target_idx].combat_effects & bue_Haste)
                            {
                                leave_screen = ST_FALSE;
                                _fstrcpy((char *)spell_name, (char *)spell_data_table[spell_idx].name);
                                stu_strcpy(GUI_NearMsgString, cnst_SpellError_1);
                                stu_strcat(GUI_NearMsgString, spell_name);
                                stu_strcat(GUI_NearMsgString, cnst_SpellError_2);
                                Warn1(GUI_NearMsgString);
                            }
                        }
                        else if((enchantments & spell_data_table[spell_idx].enchantments) != 0)
                        {
                            leave_screen = ST_FALSE;
                            _fstrcpy((char *)spell_name, (char *)spell_data_table[spell_idx].name);
                            stu_strcpy(GUI_NearMsgString, cnst_SpellError_1);
                            stu_strcat(GUI_NearMsgString, spell_name);
                            stu_strcat(GUI_NearMsgString, cnst_SpellError_2);
                            Warn1(GUI_NearMsgString);
                        }
                        else
                        {
                            if(spell_idx == spl_Immolation)
                            {
                                if((battle_units[target_idx].Attribs_2 & USA_IMMOLATION) != 0)
                                {
                                    leave_screen = ST_FALSE;
                                    LBX_Load_Data_Static(message_lbx_file__ovr113__2of2, 0, (SAMB_ptr)&GUI_NearMsgString[0], 0x5A, 1, 150);
                                    Warn1(GUI_NearMsgString);
                                }
                            }
                            if(spell_idx == spl_Cloak_Of_Fear)
                            {
                                if((battle_units[target_idx].Attribs_2 & USA_CAUSEFEAR) != 0)
                                {
                                    leave_screen = ST_FALSE;
                                    LBX_Load_Data_Static(message_lbx_file__ovr113__2of2, 0, (SAMB_ptr)&GUI_NearMsgString[0], 0x5A, 1, 150);
                                    Warn1(GUI_NearMsgString);
                                }
                            }
                            if(spell_idx == spl_True_Sight)
                            {
                                if(battle_units[target_idx].Attribs_1 & USA_IMMUNITY_ILLUSION)
                                {
                                    leave_screen = ST_FALSE;
                                    LBX_Load_Data_Static(message_lbx_file__ovr113__2of2, 0, (SAMB_ptr)&GUI_NearMsgString[0], 0x5A, 1, 150);
                                    Warn1(GUI_NearMsgString);
                                }
                            }
                            if(spell_idx == spl_Magic_Immunity)
                            {
                                if(battle_units[target_idx].Attribs_1 & USA_IMMUNITY_MAGIC)
                                {
                                    leave_screen = ST_FALSE;
                                    LBX_Load_Data_Static(message_lbx_file__ovr113__2of2, 0, (SAMB_ptr)&GUI_NearMsgString[0], 0x5A, 1, 150);
                                    Warn1(GUI_NearMsgString);
                                }
                            }
                            if(spell_idx == spl_Planar_Travel)
                            {
                                if((battle_units[target_idx].Abilities & UA_PLANARTRAVEL) != 0)
                                {
                                    leave_screen = ST_FALSE;
                                    LBX_Load_Data_Static(message_lbx_file__ovr113__2of2, 0, (SAMB_ptr)&GUI_NearMsgString[0], 0x5A, 1, 150);
                                    Warn1(GUI_NearMsgString);
                                }
                            }
                            if(spell_idx == spl_Invisibility)
                            {
                                if(battle_units[target_idx].Abilities & UA_INVISIBILITY)
                                {
                                    leave_screen = ST_FALSE;
                                    LBX_Load_Data_Static(message_lbx_file__ovr113__2of2, 0, (SAMB_ptr)&GUI_NearMsgString[0], 0x5A, 1, 150);
                                    Warn1(GUI_NearMsgString);
                                }
                            }
                            if(spell_idx == spl_Wraith_Form)
                            {
                                if(battle_units[target_idx].Abilities & UA_NONCORPOREAL)
                                {
                                    leave_screen = ST_FALSE;
                                    LBX_Load_Data_Static(message_lbx_file__ovr113__2of2, 0, (SAMB_ptr)&GUI_NearMsgString[0], 0x5A, 1, 150);
                                    Warn1(GUI_NearMsgString);
                                }
                            }
                            if(spell_idx == spl_Wind_Walking)
                            {
                                if(battle_units[target_idx].Abilities & UA_WINDWALKING)
                                {
                                    leave_screen = ST_FALSE;
                                    LBX_Load_Data_Static(message_lbx_file__ovr113__2of2, 0, (SAMB_ptr)&GUI_NearMsgString[0], 0x5A, 1, 150);
                                    Warn1(GUI_NearMsgString);
                                }
                            }
                            if(spell_idx == spl_Regeneration)
                            {
                                if(battle_units[target_idx].Abilities & UA_REGENERATION)
                                {
                                    leave_screen = ST_FALSE;
                                    LBX_Load_Data_Static(message_lbx_file__ovr113__2of2, 0, (SAMB_ptr)&GUI_NearMsgString[0], 0x5A, 1, 150);
                                    Warn1(GUI_NearMsgString);
                                }
                            }
                        }
                    }
                }
            }
        }
        if(leave_screen == ST_FALSE)
        {
            stu_strcpy(GUI_NearMsgString, cnst_CmbSpellMsg1);  // "Select a target for a "
            _fstrcpy(spell_name, spell_data_table[spell_idx].name);
            stu_strcat(GUI_NearMsgString, spell_name);
            stu_strcat(GUI_NearMsgString, cnst_SpaceSpellDot_3);  // " spell."
            Combat_Spell_Target_Screen_Draw();
            PageFlip_FX();
            /* OGBUG: the targeting loop yields two ticks per frame rather than one, so the cursor updates at roughly half the rate of the other input loops; faithful to the listing */
            Release_Time(2);
        }
    }
    Clear_Fields();
    Assign_Auto_Function(Combat_Screen_Draw, 2);
    return target_idx;
}


// WZD o113p04
void Ranged_Animation(int16_t attacker_battle_unit_idx, int16_t defender_battle_unit_idx, int16_t Target_Damage_Sum, int16_t cgx, int16_t cgy)
{
    int16_t travel_percent_step = 0;
    int16_t range = 0;
    int16_t missile_type = 0;
    int16_t itr_msl = 0;
    int16_t travel_percent_elapsed = 0;
    int16_t dist_x = 0;  // Dasm reuses travel_percent_step
    int16_t dist_y = 0;  // Dasm reuses range
    SAMB_ptr sound_buffer = NULL;
    uint32_t sound_buffer_length = 0;  /* HACK */
    sound_buffer = (SAMB_ptr)ST_UNDEFINED;
    if(defender_battle_unit_idx != 99)  /* City Walls */
    {
        range = Range_To_Battle_Unit(attacker_battle_unit_idx, defender_battle_unit_idx);
        battle_units[attacker_battle_unit_idx].target_cgx = battle_units[defender_battle_unit_idx].cgx;
        battle_units[attacker_battle_unit_idx].target_cgy = battle_units[defender_battle_unit_idx].cgy;
    }
    else
    {
        battle_units[attacker_battle_unit_idx].target_cgx = cgx;
        battle_units[attacker_battle_unit_idx].target_cgy = cgy;
        dist_x = abs(battle_units[attacker_battle_unit_idx].cgx - cgx);
        dist_y = abs(battle_units[attacker_battle_unit_idx].cgy - cgy);
        range = dist_y;  // DNE in Dasm  BUGBUG?
        if(dist_x > dist_y)
        {
            range = dist_x;
        }
    }
    if(
        (range >= 3)
        &&
        (magic_set.movement_animations != ST_FALSE)
    )
    {
        travel_percent_step = 33;  // ¿ DEDU  99 / 3 ?
    }
    else
    {
        travel_percent_step = 49;  // ¿ DEDU  99 / 2 ?
    }
    if(magic_set.sound_effects == ST_TRUE)
    {
        Play_Sound(sound_silent_seg, sound_silent_seg_size);
        Mark_Block(World_Data);
    }
    switch(battle_units[attacker_battle_unit_idx].ranged_type)
    {
        case rat_Rock:
        {
            missile_type = msl_Rocks;
            if(magic_set.sound_effects == ST_TRUE)
            {
                // SOUNDFX.LBX, 015  "CATAPLT3"  "Attack, Catapult"
                sound_buffer = LBX_Reload_Next(soundfx_lbx_file__ovr113, SFX_ATK_Catapult, World_Data);
                sound_buffer_length = lbxload_entry_length;
            }
        } break;
        case rat_Cannon:
        {
            missile_type = msl_Cloud;
            if(magic_set.sound_effects == ST_TRUE)
            {
                // SOUNDFX.LBX, 016  "EXPLODE "  "Attack, Cannon"
                sound_buffer = LBX_Reload_Next(soundfx_lbx_file__ovr113, SFX_ATK_Cannon, World_Data);
                sound_buffer_length = lbxload_entry_length;
            }
        } break;
        case rat_Bow:
        {
            missile_type = msl_Arrow;
            if(magic_set.sound_effects == ST_TRUE)
            {
                // SOUNDFX.LBX, 017  "BOW5 VOC"  "Attack, Bows"
                sound_buffer = LBX_Reload_Next(soundfx_lbx_file__ovr113, SFX_ATK_Bows, World_Data);
                sound_buffer_length = lbxload_entry_length;
            }
        } break;
        case rat_Sling:
        {
            missile_type = msl_Sling;
            if(magic_set.sound_effects == ST_TRUE)
            {
                // SOUNDFX.LBX, 018  "SLING VO"  "Attack, Slings"

                sound_buffer = LBX_Reload_Next(soundfx_lbx_file__ovr113, SFX_ATK_Slings, World_Data);
                sound_buffer_length = lbxload_entry_length;
            }
        } break;
        case rat_Lightning:
        {
            missile_type = msl_Lightning;
            if(magic_set.sound_effects == ST_TRUE)
            {
                // SOUNDFX.LBX, 019  "LITBOLT3"  "Attack, Lightning"
                sound_buffer = LBX_Reload_Next(soundfx_lbx_file__ovr113, SFX_ATK_Lightning, World_Data);
                sound_buffer_length = lbxload_entry_length;
            }
        } break;
        case rat_Fireball:
        {
            missile_type = msl_Fireball;
            if(magic_set.sound_effects == ST_TRUE)
            {
                // SOUNDFX.LBX, 020  "FIREBAL4"  "Attack, Fire"
                sound_buffer = LBX_Reload_Next(soundfx_lbx_file__ovr113, SFX_ATK_Fire, World_Data);
                sound_buffer_length = lbxload_entry_length;
            }
        } break;
        case rat_Sorcery:
        {
            missile_type = msl_Illusion;
            if(magic_set.sound_effects == ST_TRUE)
            {
                // SOUNDFX.LBX, 021  "ILUZUN4 "  "Attack, Illusion"
                sound_buffer = LBX_Reload_Next(soundfx_lbx_file__ovr113, SFX_ATK_Illusion, World_Data);
                sound_buffer_length = lbxload_entry_length;
            }
        } break;
        case rat_Deathbolt:
        {
            missile_type = msl_Deathbolt;
            if(magic_set.sound_effects == ST_TRUE)
            {
                // SOUNDFX.LBX, 022  "ZAP4 VOC"  "Attack, Death"
                sound_buffer = LBX_Reload_Next(soundfx_lbx_file__ovr113, SFX_ATK_Death, World_Data);
                sound_buffer_length = lbxload_entry_length;
            }
        } break;
        case rat_Icebolt:
        {
            missile_type = msl_Icebolt;
            if(magic_set.sound_effects == ST_TRUE)
            {
                // SOUNDFX.LBX, 023  "ICESTRM3"  "Attack, Ice"
                sound_buffer = LBX_Reload_Next(soundfx_lbx_file__ovr113, SFX_ATK_Ice, World_Data);
                sound_buffer_length = lbxload_entry_length;
            }
        } break;
        case rat_Pr_Shaman:
        {
            missile_type = msl_Priest;
            if(magic_set.sound_effects == ST_TRUE)
            {
                // SOUNDFX.LBX, 024  "HOLYATK2"  "Attack, Holy"
                sound_buffer = LBX_Reload_Next(soundfx_lbx_file__ovr113, SFX_ATK_Holy, World_Data);
                sound_buffer_length = lbxload_entry_length;
            }
        } break;
        case rat_Drow:
        {
            missile_type = msl_Drow;
            if(magic_set.sound_effects == ST_TRUE)
            {
                // SOUNDFX.LBX, 025  "ZAP1 VOC"  "Attack, Dark Elf"
                sound_buffer = LBX_Reload_Next(soundfx_lbx_file__ovr113, SFX_ATK_Drow, World_Data);
                sound_buffer_length = lbxload_entry_length;
            }
        } break;
        case rat_Sprite:
        case rat_Nat_Bolt:
        {
            missile_type = msl_Shimmer;
            if(magic_set.sound_effects == ST_TRUE)
            {
                // SOUNDFX.LBX, 026  "SPRITE V"  "Attack, Sprite"

                sound_buffer = LBX_Reload_Next(soundfx_lbx_file__ovr113, SFX_ATK_Sprite, World_Data);
                sound_buffer_length = lbxload_entry_length;
            }
        } break;
    }
    if(magic_set.sound_effects == ST_FALSE)
    {
        sound_buffer = (SAMB_ptr)ST_UNDEFINED;
    }
    if(defender_battle_unit_idx != 99)
    {
        Make_Missiles(battle_units[attacker_battle_unit_idx].figure_cnt, battle_units[defender_battle_unit_idx].figure_cnt, battle_units[attacker_battle_unit_idx].cgx, battle_units[attacker_battle_unit_idx].cgy, battle_units[defender_battle_unit_idx].cgx, battle_units[defender_battle_unit_idx].cgy, missile_type);
    }
    else
    {
        Make_Missiles(battle_units[attacker_battle_unit_idx].figure_cnt, 1, battle_units[attacker_battle_unit_idx].cgx, battle_units[attacker_battle_unit_idx].cgy, cgx, cgy, missile_type);
    }
    projectile_anim_frame = 0;
    niu_projectile_anim_frame_2 = 0;
    if(sound_buffer != (SAMB_ptr)ST_UNDEFINED)
    {
        /* HACK */  if(NULL != sound_buffer && sound_buffer_length != 0)
        /* HACK */  {
        /* HACK */      Play_Sound(sound_buffer, sound_buffer_length);
        /* HACK */  }
    }
    for(travel_percent_elapsed = 0; travel_percent_elapsed < 100; travel_percent_elapsed += travel_percent_step)
    {
        if((travel_percent_elapsed + travel_percent_step) > 99)
        {
            projectile_anim_frame = 4;
            niu_projectile_anim_frame_2 = 4;
            if(defender_battle_unit_idx != 99)
            {
                Set_Gibs(defender_battle_unit_idx, Target_Damage_Sum);
            }
        }
        else
        {
            projectile_anim_frame = ((projectile_anim_frame + 1) % 3);  // ; used with entity drawing type 3, steps 0 to 2
            niu_projectile_anim_frame_2 = ((niu_projectile_anim_frame_2 + 1) % 3);
        }
        Combat_Screen_Draw();
        PageFlip_FX();
        for(itr_msl = 0; itr_msl < m_missile_count; itr_msl++)
        {
            _missiles[itr_msl].travel_percent += travel_percent_step;
            if(_missiles[itr_msl].travel_percent > 100)
            {
                _missiles[itr_msl].travel_percent = 100;
            }
        }
    }
    m_missile_count = 0;
    if(defender_battle_unit_idx != 99)
    {
        Clear_Gibs(defender_battle_unit_idx);
    }
    if(magic_set.sound_effects == ST_TRUE)
    {
        Release_Block(World_Data);
    }
}


// WZD o113p05
/**
 * @brief Plays the melee hit animation sequence between an attacker and a target.
 *
 * Sets temporary gore/figure-loss display data via `Set_Gibs()`, configures
 * melee target coordinates for visual movement, optionally plays melee SFX,
 * advances a short animation loop, and then clears temporary state with
 * `Clear_Gibs()`.
 *
 * @param attacker_battle_unit_idx Index of the attacking unit in `battle_units`.
 * @param defender_battle_unit_idx Index of the defending unit in `battle_units`,
 *        or `99` when striking a map cell instead of a unit.
 * @param attacker_damage Damage received by the attacker during this exchange
 *        (used to compute attacker blood/figure-loss visuals).
 * @param defender_damage Damage received by the defender during this exchange
 *        (used only when `defender_battle_unit_idx != 99`).
 * @param cgx Combat-grid X coordinate used as the strike target when
 *        `defender_battle_unit_idx == 99`.
 * @param cgy Combat-grid Y coordinate used as the strike target when
 *        `defender_battle_unit_idx == 99`.
 *
 * @note This function mutates per-unit animation fields including
 *       `target_cgx`, `target_cgy`, and `Melee_Anim`.
 * @note Sound playback and resource block marking/release occur only when
 *       `magic_set.sound_effects == ST_TRUE`.
 */
void Melee_Animation(int16_t attacker_battle_unit_idx, int16_t defender_battle_unit_idx, int16_t attacker_damage, int16_t defender_damage, int16_t cgx, int16_t cgy)
{
    SAMB_ptr sound_data = 0;
    int16_t anim_frame_count = 0;
    int16_t itr_anim_frame = 0;
    /* HACK */  uint32_t sound_buffer_size = 0;  // DNE in Dasm
    if(magic_set.movement_animations == ST_TRUE)
    {
        anim_frame_count = 5;
    }
    else
    {
        anim_frame_count = 1;
    }
    Set_Gibs(attacker_battle_unit_idx, attacker_damage);
    if(defender_battle_unit_idx == 99)
    {
        battle_units[attacker_battle_unit_idx].target_cgx = cgx;
        battle_units[attacker_battle_unit_idx].target_cgy = cgy;
    }
    else
    {
        Set_Gibs(defender_battle_unit_idx, defender_damage);
        battle_units[defender_battle_unit_idx].Melee_Anim = 2;
        battle_units[defender_battle_unit_idx].target_cgx = battle_units[attacker_battle_unit_idx].cgx;
        battle_units[defender_battle_unit_idx].target_cgy = battle_units[attacker_battle_unit_idx].cgy;
        battle_units[attacker_battle_unit_idx].target_cgx = battle_units[defender_battle_unit_idx].cgx;
        battle_units[attacker_battle_unit_idx].target_cgy = battle_units[defender_battle_unit_idx].cgy;
    }
    battle_units[attacker_battle_unit_idx].Melee_Anim = 1;
    if(magic_set.sound_effects == ST_TRUE)
    {
        Play_Sound(sound_silent_seg, sound_silent_seg_size);
        Mark_Block(World_Data);
        sound_data = Reload_Melee_Sound(attacker_battle_unit_idx, &sound_buffer_size);
        Play_Sound(sound_data, sound_buffer_size);
    }
    for(itr_anim_frame = 0; itr_anim_frame < anim_frame_count; itr_anim_frame++)
    {
        Mark_Time();
        Set_Page_Off();
        Combat_Screen_Draw();
        PageFlip_FX();
        Release_Time(2);
    }
    Clear_Gibs(attacker_battle_unit_idx);
    if(defender_battle_unit_idx != 99)
    {
        Clear_Gibs(defender_battle_unit_idx);
        battle_units[defender_battle_unit_idx].Melee_Anim = 0;
    }
    battle_units[attacker_battle_unit_idx].Melee_Anim = 0;
    if(magic_set.sound_effects == ST_TRUE)
    {
        Release_Block(World_Data);
    }
}


// WZD o113p06
void Compute_Battle_Unit_Damage_From_Spell(uint16_t spell_idx, uint16_t battle_unit_idx, int16_t damage_types[], int16_t attack_strength_override)
{
    uint32_t enchantments = 0;
    int16_t damage_total = 0;
    int16_t figures_lost = 0;
    int16_t defender_toblock = 0;
    int16_t attack_count = 0;
    int16_t attack_strength = 0;
    int16_t defense_special = 0;
    int16_t attack_immunities = 0;
    int16_t attack_attributes = 0;
    int16_t front_figure_damage = 0;
    int16_t itr = 0;
    int16_t damage = 0;
    int16_t attack_roll = 0;  // DNE in Dasm
    int16_t defense_roll = 0;  // DNE in Dasm
    figures_lost = 0;
    damage = 0;
    enchantments = (battle_units[battle_unit_idx].enchantments | battle_units[battle_unit_idx].item_enchantments | _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments);
    for(itr = 0; itr < NUM_DAMAGE_TYPES; itr++)
    {

        damage_types[itr] = 0;

    }
    if(
        ((enchantments & UE_RIGHTEOUSNESS) != 0)
        &&
        (
            (spell_data_table[spell_idx].magic_realm == sbr_Chaos)
            ||
            (spell_data_table[spell_idx].magic_realm == sbr_Death)
        )
    )
    {
        return;
    }
    if((battle_units[battle_unit_idx].Attribs_1 & USA_IMMUNITY_MAGIC) != 0)
    {
        return;
    }
    front_figure_damage = battle_units[battle_unit_idx].front_figure_damage;
    attack_attributes = spell_data_table[spell_idx].attributes;
    defender_toblock = battle_units[battle_unit_idx].toblock;
    if(attack_attributes & Att_EldrWeap)
    {
        defender_toblock--;
    }
    attack_immunities = spell_data_table[spell_idx].immunities;
    if(attack_strength_override > 0)
    {
        attack_strength = attack_strength_override;
    }
    else
    {
        attack_strength = spell_data_table[spell_idx].strength;
    }
    defense_special = Battle_Unit_Defense_Special(battle_unit_idx, rat_Nat_Bolt, attack_immunities, attack_attributes, spell_data_table[spell_idx].magic_realm);
    if(attack_attributes & Att_AREAFLAG)
    {
        attack_count = battle_units[battle_unit_idx].figure_cnt;
        attack_attributes |= Att_DMGLIMIT;
    }
    else if(attack_attributes & Att_WarpLghtn)
    {
        attack_count = attack_strength;
    }
    else
    {
        attack_count = 1;
    }
    if(battle_units[battle_unit_idx].combat_effects & bue_Black_Sleep)
    {
        attack_attributes |= Att_DoomDmg;
    }
    /*
        BEGIN:  Attacks
    */
    for(itr = 0; itr < attack_count; itr++)
    {
        if(attack_attributes & Att_DoomDmg)
        {
            damage = attack_strength;
        }
        else
        {
            attack_roll = Combat_Roll_Damage_Dealt(attack_strength, 0);
            defense_roll = Combat_Roll_Damage_Blocked(defense_special, defender_toblock);
            damage += attack_roll;
            damage -= defense_roll;
            if((enchantments & UE_INVULNERABILITY) != 0)
            {
                damage -= 2;
            }
            SETMIN(damage, 0);
        }
        if(
            (itr == 0)
            &&
            (front_figure_damage >= 0)
        )
        {
            damage += front_figure_damage;
            front_figure_damage = 0;
        }
        if(front_figure_damage < 0)
        {
            front_figure_damage += damage;
            if(front_figure_damage > 0)
            {
                damage = front_figure_damage;
                front_figure_damage = 0;
            }
            else
            {
                damage = 0;
            }
        }
        if((attack_attributes & Att_DMGLIMIT) != 0)
        {
            if(battle_units[battle_unit_idx].hits < damage)
            {
                figures_lost++;
                damage = 0;
            }
        }
        else
        {
            while(battle_units[battle_unit_idx].hits < damage)
            {
                figures_lost++;
                damage -= battle_units[battle_unit_idx].hits;
                if((attack_attributes & Att_DoomDmg) == 0)
                {
                    defense_roll = Combat_Roll_Damage_Blocked(defense_special, defender_toblock);
                    damage -= defense_roll;
                    if((enchantments & UE_INVULNERABILITY) != 0)
                    {
                        damage -= 2;
                    }
                }
            } 
            SETMIN(damage, 0);
        }
        damage_total += (damage + (figures_lost * battle_units[battle_unit_idx].hits));
        damage = 0;
        figures_lost = 0;
        if(attack_attributes & Att_WarpLghtn)
        {
            attack_strength--;
        }
    }
    /*
        END:  Attacks
    */
    damage_total -= battle_units[battle_unit_idx].front_figure_damage;
    if(damage_total < 0)
    {
        damage_total = 0;
    }
    damage_types[0] = damage_total;
}


// WZD o113p07
void Battle_Unit_Commit_Damage(int16_t battle_unit_idx, int16_t damage_types[])
{
    int16_t figures_killed = 0;
    int16_t damage_total = 0;
    int16_t itr = 0;
    damage_total = 0;
    for(itr = 0; itr < 3; itr++)
    {
        damage_total += damage_types[itr];
    }
    if(damage_total <= 0)
    {
        return;
    }
    if(battle_units[battle_unit_idx].status != bus_Active)
    {
        return;
    }
    for(itr = 0; itr < 3; itr++)
    {
        if((battle_units[battle_unit_idx].damage[itr] + damage_types[itr]) <= 200)
        {
            battle_units[battle_unit_idx].damage[itr] += damage_types[itr];
        }
        else
        {
            battle_units[battle_unit_idx].damage[itr] = 200;
        }
    }
    damage_total += battle_units[battle_unit_idx].front_figure_damage;
    if(damage_total > 0)
    {
        figures_killed = (damage_total / battle_units[battle_unit_idx].hits);
        if(battle_units[battle_unit_idx].figure_cnt < figures_killed)
        {
            figures_killed = battle_units[battle_unit_idx].figure_cnt;
        }
        battle_units[battle_unit_idx].figure_cnt -= figures_killed;
        battle_units[battle_unit_idx].front_figure_damage = (damage_total % battle_units[battle_unit_idx].hits);
    }
    /*
        He's dead, Jim.
    */
   // @@HesDeadJim  Pure-Red in IDA
    if(battle_units[battle_unit_idx].figure_cnt <= 0)
    {
        battle_units[battle_unit_idx].figure_cnt = 0;
        _combat_winner = Eliminated_Opponent();
        if(Battle_Unit_Is_Summoned_Creature(battle_unit_idx) == ST_TRUE)
        {
            _UNITS[battle_units[battle_unit_idx].unit_idx].wp = 9;  // dead combat summon  (or, just anything that can not be revived?)
        }
        if(
            (battle_units[battle_unit_idx].damage[2] >= battle_units[battle_unit_idx].damage[1])
            &&
            (battle_units[battle_unit_idx].damage[2] >= battle_units[battle_unit_idx].damage[0])
        )
        {
            battle_units[battle_unit_idx].status = bus_Gone;
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
                    battle_units[battle_unit_idx].status = bus_Drained;
                }
                else
                {
                    battle_units[battle_unit_idx].status = bus_Dead;
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
                    battle_units[battle_unit_idx].status = bus_Dead;
                }
            }
        }
        Update_Sees_Illusions();
    }
}


// WZD o113p08
int16_t Check_Attack_Ranged(int16_t attacker_battle_unit_idx, int16_t defender_battle_unit_idx)
{
    int16_t result = 0;
    uint32_t defender_enchantments = 0;
    uint32_t attacker_enchantments = 0;
    defender_enchantments = (battle_units[defender_battle_unit_idx].enchantments | battle_units[defender_battle_unit_idx].item_enchantments | _UNITS[battle_units[defender_battle_unit_idx].unit_idx].enchantments);
    attacker_enchantments = (battle_units[attacker_battle_unit_idx].enchantments | battle_units[attacker_battle_unit_idx].item_enchantments | _UNITS[battle_units[attacker_battle_unit_idx].unit_idx].enchantments);
    result = 0;
    if(
        ((attacker_enchantments & UE_TRUE_SIGHT) != 0)
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
    if(battlefield->wall_of_darkness == 1)
    {
        /* OGBUG  ignores innate Illusions Immunity */
        if((attacker_enchantments & UE_TRUE_SIGHT) == 0)
        {
            if(Battle_Unit_Is_Within_City(defender_battle_unit_idx) == ST_TRUE)
            {
                if(Battle_Unit_Is_Within_City(attacker_battle_unit_idx) == ST_FALSE)
                {
                    result = 2;
                }
            }
        }
    }
    return result;
}


// WZD o113p09
void Deploy_Battle_Units(int16_t player_idx)
{
    int16_t starting_cgy[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t starting_cgx[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t ranged_battle_unit_idxs[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t melee_battle_unit_idxs[MAX_STACK]  = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t move_to_front_count = 0;
    int16_t highest_melee_ranged_slot = 0;
    int16_t highest_melee_strength = 0;
    int16_t itr_ranged_slots = 0;
    int16_t ranged_battle_unit_count = 0;
    int16_t melee_battle_unit_count = 0;
    int16_t itr = 0;
    int16_t ctr = 0;
    Undeployable_Battle_Units_On_Water(player_idx);
    if(player_idx == _combat_defender_player)
    {
        starting_cgx[ 0] = DEPLOY_DFNDR_ROW1;
        starting_cgx[ 1] = DEPLOY_DFNDR_ROW1;
        starting_cgx[ 2] = DEPLOY_DFNDR_ROW1;
        starting_cgx[ 3] = DEPLOY_DFNDR_ROW1;
        starting_cgx[ 4] = DEPLOY_DFNDR_ROW2;
        starting_cgx[ 5] = DEPLOY_DFNDR_ROW2;
        starting_cgx[ 6] = DEPLOY_DFNDR_ROW2;
        starting_cgx[ 7] = DEPLOY_DFNDR_ROW2;
        starting_cgx[ 8] = DEPLOY_DFNDR_ROW3;
        starting_cgx[ 9] = DEPLOY_DFNDR_ROW3;
        starting_cgx[10] = DEPLOY_DFNDR_ROW3;
        starting_cgx[11] = DEPLOY_DFNDR_ROW3;
    }
    else
    {
        starting_cgx[ 0] = DEPLOY_ATTKR_ROW1;
        starting_cgx[ 1] = DEPLOY_ATTKR_ROW1;
        starting_cgx[ 2] = DEPLOY_ATTKR_ROW1;
        starting_cgx[ 3] = DEPLOY_ATTKR_ROW1;
        starting_cgx[ 4] = DEPLOY_ATTKR_ROW2;
        starting_cgx[ 5] = DEPLOY_ATTKR_ROW2;
        starting_cgx[ 6] = DEPLOY_ATTKR_ROW2;
        starting_cgx[ 7] = DEPLOY_ATTKR_ROW2;
        starting_cgx[ 8] = DEPLOY_ATTKR_ROW3;
        starting_cgx[ 9] = DEPLOY_ATTKR_ROW3;
        starting_cgx[10] = DEPLOY_ATTKR_ROW3;
        starting_cgx[11] = DEPLOY_ATTKR_ROW3;
    }
    starting_cgy[ 0] = DEPLOY_COL1;
    starting_cgy[ 1] = DEPLOY_COL2;
    starting_cgy[ 2] = DEPLOY_COL3;
    starting_cgy[ 3] = DEPLOY_COL4;
    starting_cgy[ 4] = DEPLOY_COL1;
    starting_cgy[ 5] = DEPLOY_COL2;
    starting_cgy[ 6] = DEPLOY_COL3;
    starting_cgy[ 7] = DEPLOY_COL4;
    starting_cgy[ 8] = DEPLOY_COL1;
    starting_cgy[ 9] = DEPLOY_COL2;
    starting_cgy[10] = DEPLOY_COL3;
    starting_cgy[11] = DEPLOY_COL4;
    melee_battle_unit_count = 0;
    ranged_battle_unit_count = 0;
    for(itr = 0; itr < _combat_total_unit_count; itr++)
    {
        if(battle_units[itr].controller_idx == player_idx)
        {
            if(Battle_Unit_Has_Ranged_Attack(itr) != ST_FALSE)
            {
                ranged_battle_unit_idxs[ranged_battle_unit_count] = itr;
                ranged_battle_unit_count++;
            }
            else
            {
                melee_battle_unit_idxs[melee_battle_unit_count] = itr;
                melee_battle_unit_count++;
            }
        }
    }
    // if there are more than 5 ranged units, move the ones with the highest melee strength over to the melee group
    // until there are either at least 4 units there, or only 4 left in the ranged group
    if(ranged_battle_unit_count > 5)
    {
        move_to_front_count = (ranged_battle_unit_count - 4);
        // find the ranged unit with the highest melee strength
        for(itr = 0; ((itr < move_to_front_count) && (melee_battle_unit_count < 4)); itr++)
        {
            highest_melee_strength = -1;
            highest_melee_ranged_slot = ST_UNDEFINED;
            for(itr_ranged_slots = 0; itr_ranged_slots < ranged_battle_unit_count; itr_ranged_slots++)
            {
                /* OGBUG: should use ranged_battle_unit_idxs[], not battle_units[] */
                if(battle_units[itr_ranged_slots].melee > highest_melee_strength)
                {
                    highest_melee_strength = battle_units[itr_ranged_slots].melee;
                    highest_melee_ranged_slot = itr_ranged_slots;
                }
            }
            if(melee_battle_unit_count < 4)
            {
                melee_battle_unit_idxs[melee_battle_unit_count] = ranged_battle_unit_idxs[highest_melee_ranged_slot];
                melee_battle_unit_count++;
                Clear_Structure(highest_melee_ranged_slot, (uint8_t *)&ranged_battle_unit_idxs, 2, ranged_battle_unit_count);
                ranged_battle_unit_count--;
            }
        }
    }
    // place the melee units, starting from the front of the local coordinate array, and skipping wall corner and central structure squares for the defender
    ctr = 0;
    for(itr = 0; itr < melee_battle_unit_count; itr++)
    {
        if(_combat_defender_player == player_idx)
        {
            while(
                (battlefield->walled == ST_TRUE)
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
                (battlefield->center_square_structure != CS_City)
                &&
                (ctr == 9)
            )
            {
                ctr++;
            }
        }
        battle_units[melee_battle_unit_idxs[itr]].cgx = starting_cgx[ctr];
        battle_units[melee_battle_unit_idxs[itr]].cgy = starting_cgy[ctr];
        ctr++;
    }
    // place the ranged units, skipping wall corner and central structure squares for the defender
    for(itr = 0; itr < ranged_battle_unit_count; itr++)
    {
        if(_combat_defender_player == player_idx)
        {
            while(
                (battlefield->walled == ST_TRUE)
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
                (battlefield->center_square_structure != CS_City)
                &&
                (ctr == 9)
            )
            {
                ctr++;
            }
        }
        battle_units[ranged_battle_unit_idxs[itr]].cgx = starting_cgx[ctr];
        battle_units[ranged_battle_unit_idxs[itr]].cgy = starting_cgy[ctr];
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


// WZD o113p10
/*
; BUG: will enter an infinite loop if all 9 squares that the unit's side can summon to are occupied
*/
/*
    ~=== Demon Lord's Summon Demon
    2 USA bits are used as the count of summonable demons
*/
void Summon_Demon(int16_t caster_idx)
{
    int16_t cgy = 0;
    int16_t caster_attribs_1 = 0;
    int16_t cgx = 0;
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
    /*
        Create_Unit(unit_type, owner_idx, wx, wy, wp, R_Param)
            wx, wy  =    0  no world position; the demon exists only on the battlefield
            wp      =    9  the "in combat" marker; Prepare_Battle_Unit_Summons tests _UNITS[].wp == 9 to find a recyclable slot once all 36 battle unit slots are full
            R_Param = 2000  bypasses both unit-count caps in Create_Unit() -- >950 for non-human owners, and >980 for anyone
    */
    Create_Unit(ut_Demon, battle_units[caster_idx].controller_idx, 0, 0, 9, 2000);
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
    } while(g_combat_grid_action_map[cgy][cgx] >= 0);
    Prepare_Battle_Unit_Summons(battle_units[caster_idx].controller_idx, (_units - 1), cgx, cgy);
    Battle_Unit_Summon_Animation((_combat_total_unit_count - 1), cgx, cgy, spl_NONE, caster_idx);
}


// WZD o113p11
int16_t Check_Attack_Melee_City_Wall(int16_t src_battle_unit_idx, int16_t dst_battle_unit_idx)
{
    if(battlefield->walled != 1)
    {
        return ST_TRUE;
    }
    /* OGBUG: Teleporting and merging units can attack through walls according to the code below */
    if((battle_units[src_battle_unit_idx].Move_Flags & MV_FLYING) != 0)
    {
        return ST_TRUE;
    }
    if((battle_units[src_battle_unit_idx].Move_Flags & MV_TELEPORT) != 0)
    {
        return ST_TRUE;
    }
    if((battle_units[src_battle_unit_idx].Move_Flags & MV_MERGING) != 0)
    {
        return ST_TRUE;
    }
    if((battle_units[src_battle_unit_idx].Abilities & UA_NONCORPOREAL) != 0)
    {
        return ST_TRUE;
    }
    /* If the target is not inside the city, walls do not apply */
    if(Battle_Unit_Is_Within_City(dst_battle_unit_idx) != 1)
    {
        return ST_TRUE;
    }
    /* If the attacker is already inside the city, walls do not apply */
    if(Battle_Unit_Is_Within_City(src_battle_unit_idx) != 0)
    {
        return ST_TRUE;
    }
    /* Check for the city gate at (8, 12) */
    if(battle_units[dst_battle_unit_idx].cgx == CGX_GATE && battle_units[dst_battle_unit_idx].cgy == CGY_GATE)
    {
        return ST_TRUE;
    }
    /* Check if the specific cell occupied by the target contains a city wall section */
    if(Combat_Grid_Cell_Has_City_Wall(battle_units[dst_battle_unit_idx].cgx, battle_units[dst_battle_unit_idx].cgy) != 1)
    {
        return ST_TRUE;
    }
    /* Cannot attack through the wall */
    return ST_FALSE;
}


// WZD o113p12
// drake178: 
void Apply_Mana_Leak(void)
{
    int16_t player_idx = 0;  // _SI_
    int16_t battle_unit_idx = 0;  // _CX_

    player_idx = ST_UNDEFINED;

    if(combat_enchantments[MANA_LEAK_ATTKR] > 0)
    {

            player_idx = _combat_defender_player;

            if(combat_enchantments[MANA_LEAK_DFNDR] > 0)
            {

                player_idx = 666;

            }

    }
    else
    {

        if(combat_enchantments[MANA_LEAK_DFNDR] > 0)
        {

            player_idx = _combat_attacker_player;
            
        }

    }

    if(player_idx >= 0)
    {

        for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
        {

            if(
                (battle_units[battle_unit_idx].status == bus_Active)
                &&
                (
                    (battle_units[battle_unit_idx].controller_idx == player_idx)
                    ||
                    (player_idx == 666)
                )
            )
            {

                if(battle_units[battle_unit_idx].mana < 5)
                {

                    battle_units[battle_unit_idx].mana = 0;

                }
                else
                {

                    battle_units[battle_unit_idx].mana -= 5;

                }

                if(
                    ((battle_units[battle_unit_idx].ranged_type / 10) == rag_Magic)
                    &&
                    (battle_units[battle_unit_idx].ammo > 0)
                )
                {

                    battle_units[battle_unit_idx].ammo -= 1;

                }

                if(
                    (battle_units[battle_unit_idx].ammo == 0)
                    &&
                    (battle_units[battle_unit_idx].mana < 3)
                )
                {

                    battle_units[battle_unit_idx].ranged_type = rat_UNDEF;

                    battle_units[battle_unit_idx].ranged = 0;

                }

            }

        }

    }

    if(player_idx < _num_players)
    {

        _players[player_idx].mana_reserve -= 5;

        if(_players[player_idx].mana_reserve < 0)
        {

            _players[player_idx].mana_reserve = 0;

        }

    }
    else  /* ¿ player_idx == 666 ? */
    {

        if(player_idx != NEUTRAL_PLAYER_IDX)
        {

            if(_combat_attacker_player < _num_players)
            {

                _players[_combat_attacker_player].mana_reserve -= 5;

                if(_players[_combat_attacker_player].mana_reserve < 0)
                {

                    _players[_combat_attacker_player].mana_reserve = 0;
                    
                }
                
            }

            if(_combat_defender_player < _num_players)
            {

                _players[_combat_defender_player].mana_reserve -= 5;

                if(_players[_combat_defender_player].mana_reserve < 0)
                {

                    _players[_combat_defender_player].mana_reserve = 0;
                    
                }
                
            }

        }
    
    }

}


// WZD o113p13
void Combat_Spell_Counter_Message_Box_Draw(void)
{
    int16_t height = 0;
    int16_t width = 0;
    uint8_t colors[2] = { 0, 0 };
    int16_t y2 = 0;
    int16_t x2 = 0;
    int16_t x1 = 0;
    int16_t y1 = 0;
    colors[0] = 182;
    colors[1] = 177;
    Set_Font_Colors_15(0, &colors[0]);
    Set_Font_Style_Shadow_Down(1, 15, 0, 0);
    Set_Alias_Color(182);
    Set_Outline_Color(2);
    width = Get_Paragraph_Max_Width(150, GUI_NearMsgString, 2);
    height = Get_Paragraph_Max_Height(150, GUI_NearMsgString);  /* OGBUG  passes 3rd argument */
    x1 = 160 - (width / 2) - 5;
    x2 = (SCREEN_XMID_168 + (width / 2));
    y1 = 5;
    y2 = height + 8;
    Gradient_Fill(x1, y1, x2, y2, 15, 8, ST_NULL, ST_NULL, ST_NULL);
    Line(x1, y1, x1, y2, 230);
    Line(x1, y1, x2 - 1, y1, 230);
    Line(x2, y1, x2, y2, 237);
    Line(x1 + 1, y2, x2, y2, 237);
    Print_Paragraph(x1 + 6, 8, 150, GUI_NearMsgString, 2);
}


// WZD o113p14
void Combat_Cast_Spell_Message(int16_t caster_idx, int16_t spell_idx)
{
    char spell_name[LEN_SPELL_NAME] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t string_width = 0;
    uint8_t colors[2] = { 0, 0 };
    int16_t y2 = 0;
    int16_t x2 = 0;
    int16_t y1 = 0;
    int16_t x1 = 0;
    if(caster_idx >= CASTER_IDX_BASE)
    {
        stu_strcpy(GUI_NearMsgString, _players[(caster_idx - CASTER_IDX_BASE)].name);
        stu_strcat(GUI_NearMsgString, cnst_CombatCast_1);  // " has cast "
    }
    else  /* (caster_idx < CASTER_IDX_BASE) */
    {
        if(_UNITS[battle_units[caster_idx].unit_idx].Hero_Slot > ST_UNDEFINED)
        {
            /* OGBUG: this may not be the hero's original owner, should use _UNITS[].owner_idx */
            stu_strcpy(GUI_NearMsgString, _players[battle_units[caster_idx].controller_idx].Heroes[_UNITS[battle_units[caster_idx].unit_idx].Hero_Slot].name);
            stu_strcat(GUI_NearMsgString, cnst_CombatCast_1);  // " has cast "
        }
        else
        {
            stu_strcpy(GUI_NearMsgString, cnst_CombatCast_2);  // "The "
            stu_strcat(GUI_NearMsgString, *_unit_type_table[_UNITS[battle_units[caster_idx].unit_idx].type].name);
            if(battle_units[caster_idx].figure_max > 1)
            {
                stu_strcat(GUI_NearMsgString, cnst_CombatCast_3);  // " have cast "
            }
            else
            {
                stu_strcat(GUI_NearMsgString, cnst_CombatCast_1);  // " has cast "
            }
        }
    }
    if(spell_idx != spl_NONE)
    {
        _fstrcpy(spell_name, spell_data_table[spell_idx].name);
        stu_strcat(GUI_NearMsgString, spell_name);
    }
    else
    {
        stu_strcat(GUI_NearMsgString, cnst_CombatCast_4);  // "summon demon"
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


// WZD o113p15
int16_t Battle_Unit_Pict_Open(void)
{
    int16_t battle_unit_slots[MAX_BATTLE_UNIT_COUNT] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t battle_unit_slot_idx = 0;
    for(battle_unit_slot_idx = 0; battle_unit_slot_idx < MAX_BATTLE_UNIT_COUNT; battle_unit_slot_idx++)
    {
        battle_unit_slots[battle_unit_slot_idx] = ST_FALSE;
    }
    for(battle_unit_slot_idx = 0; battle_unit_slot_idx < _combat_total_unit_count; battle_unit_slot_idx++)
    {
        if(
            (battle_units[battle_unit_slot_idx].bufpi > ST_UNDEFINED)
            &&
            (battle_units[battle_unit_slot_idx].status == bus_Active)
        )
        {
            battle_unit_slots[battle_units[battle_unit_slot_idx].bufpi] = ST_TRUE;
        }
    }
    for(battle_unit_slot_idx = 0; battle_unit_slot_idx < MAX_BATTLE_UNIT_COUNT; battle_unit_slot_idx++)
    {
        if(battle_unit_slots[battle_unit_slot_idx] == ST_FALSE)
        {
            return battle_unit_slot_idx;
        }
    }
    /* OGBUG  function is missing return value if no empty slot is found  SEE: Hero_Slot_Open() */
    /* HACK */  return ST_UNDEFINED;  // DNE in Dasm
}


// WZD o113p16
/**
 * @brief Determines whether either combat side has been completely eliminated.
 *
 * This routine scans all active battle units and counts how many effective combatants remain for
 * the attacker and defender. Units under confusion_state value 2 are counted for the opposing side
 * instead of their controller, matching the combat logic that treats fully confused units as acting
 * against their original army for winner-determination purposes.
 *
 * After all active units are evaluated, the function reports a winner only if one side has no
 * remaining effective units. If both sides still have at least one counted combatant, combat is
 * considered unresolved.
 *
 * @return The winning player index when one side has no remaining counted units.
 * @return ST_UNDEFINED when both the attacker and defender still have effective units in combat.
 *
 * @note Only units with status bus_Active are considered.
 * @note Confused units with confusion_state equal to 2 are counted for the opposite side.
 */
int16_t Eliminated_Opponent(void)
{
    int16_t attacker_count = 0;
    int16_t defender_count = 0;
    int16_t itr = 0;
    attacker_count = 0;
    defender_count = 0;
    for(itr = 0; itr < _combat_total_unit_count; itr++)
    {
        if(battle_units[itr].status == bus_Active)
        {
            if(battle_units[itr].controller_idx == _combat_attacker_player)
            {
                if(battle_units[itr].confusion_state != 2)
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
                if(battle_units[itr].controller_idx == _combat_defender_player)
                {
                    if(battle_units[itr].confusion_state != 2)
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
    if(attacker_count == 0) { return _combat_defender_player; }
    if(defender_count == 0) { return _combat_attacker_player; }
    return ST_UNDEFINED;
}



/*
    WIZARDS.EXE  ovr122
*/

// WZD o122p01
int16_t Combat_Roll_Damage_Dealt(int16_t attack_strength, int16_t to_hit)
{
    int16_t success_count = 0;
    int16_t total_rolls = 0;
    int16_t die_roll = 0;
    success_count = 0;
    total_rolls = 0;
    while(total_rolls < attack_strength)
    {
        die_roll = Random(10);
        if((8 - to_hit) <= die_roll || die_roll == 10)
        {
            success_count++;
        }
        total_rolls++;
    }
    return success_count;
}


// WZD o122p02
int16_t Combat_Roll_Damage_Blocked(int16_t defense, int16_t to_block)
{
    int16_t success_count = 0;
    int16_t total_rolls = 0;
    int16_t die_roll = 0;
    success_count = 0;
    total_rolls = 0;
    while(total_rolls < defense)
    {
        die_roll = Random(10);
        if(die_roll >= (8 - to_block))
        {
            success_count++;
        }
        total_rolls++;
    }
    return success_count;
}

// WZD o122p03
int16_t Combat_Resistance_Check(struct s_BATTLE_UNIT battle_unit, int16_t resistance_modifier, int16_t magic_realm)
{
    int16_t resistance = 0;
    int16_t chance = 0;
    int16_t fail;  // DNE in Dasm
    resistance = (Combat_Effective_Resistance(battle_unit, magic_realm) + resistance_modifier);
    chance = Random(10);  // 10 as in a 10% chance out of 100%  (min:1,max:10)
    if(chance > resistance)
    {
        fail = (chance - resistance);
    }
    else
    {
        fail = 0;
    }
    return fail;
}


// WZD o122p04
/**
 * @brief Calculates a battle unit's effective resistance against a magic realm.
 *
 * Starts with the unit's base resistance and adds bonuses from hero status,
 * magic immunity, realm-specific enchantments, and resistance enchantments.
 *
 * @param battle_unit Battle unit whose resistance and enchantments are evaluated.
 * @param magic_realm Magic realm of the spell or attack being resisted.
 * @return The effective resistance value used by combat resistance checks.
 *
 * @details Charmed heroes receive a +30 bonus. Magic immunity provides +30
 * against Nature, Chaos, and Death realms. Righteousness provides +30 against
 * Chaos and Death. Elemental Armor (+10) takes precedence over Resist Elements
 * (+3) against Chaos and Nature. Bless (+3) applies against Chaos and Death,
 * while Resist Magic (+5) applies against Nature, Chaos, and Death.
 *
 * @note This function does not modify the supplied battle unit or global game
 * state. The returned value is combined with any spell-specific modifier by
 * Combat_Resistance_Check().
 */
int16_t Combat_Effective_Resistance(struct s_BATTLE_UNIT battle_unit, int16_t magic_realm)
{
    uint32_t enchantments = 0;
    int16_t unit_idx = 0;
    int16_t resistance = 0;
    enchantments = (_UNITS[battle_unit.unit_idx].enchantments | battle_unit.enchantments | battle_unit.item_enchantments);
    resistance = battle_unit.resist;
    unit_idx = battle_unit.unit_idx;
    if(
        (_UNITS[unit_idx].Hero_Slot > ST_UNDEFINED)
        &&
        ((_HEROES2[_UNITS[unit_idx].owner_idx]->heroes[_UNITS[unit_idx].type].abilities & HSA_CHARMED) != 0)
    )
    {
        resistance += 30;
    }
    if(
        ((battle_unit.Attribs_1 & USA_IMMUNITY_MAGIC) != 0)
        &&
        (magic_realm >= sbr_Nature)
    )
    {
        resistance += 30;
    }
    if(
        ((enchantments & UE_RIGHTEOUSNESS) != 0)
        &&
        (
            (magic_realm == sbr_Chaos)
            ||
            (magic_realm == sbr_Death)
        )
    )
    {
        resistance += 30;
    }
    if(
        (magic_realm == sbr_Chaos)
        ||
        (magic_realm == sbr_Nature)
    )
    {
        if((enchantments & UE_ELEMENTAL_ARMOR) != 0)
        {
            resistance += 10;
        }
        else if((enchantments & UE_RESIST_ELEMENTS) != 0)
        {
            resistance += 3;
        }
    }
    if(
        ((enchantments & UE_BLESS) != 0)
        &&
        (
            (magic_realm == sbr_Chaos)
            ||
            (magic_realm == sbr_Death)
        )
    )
    {
        resistance += 3;
    }
    if(
        ((enchantments & UE_RESIST_MAGIC) != 0)
        &&
        (magic_realm >= sbr_Nature)
    )
    {
        resistance += 5;
    }
    return resistance;
}


// WZD o122p05
/*
; BUG: fails to properly set Weapon Immunity for Thrown attacks made with normal weapons
*/
static int16_t Battle_Unit_Attack_Immunities(int16_t battle_unit_idx, int16_t attack_mode)
{
    int16_t attack_immunities = 0;
    attack_immunities = 0;
    if((battle_units[battle_unit_idx].attack_attributes & Att_Illusion) > 0)
    {
        attack_immunities |= USA_IMMUNITY_ILLUSION;
    }
    if(attack_mode <= am_Melee)
    {
        if(battle_units[battle_unit_idx].Weapon_Plus1 == 0)
        {
            attack_immunities |= USA_IMMUNITY_WEAPON;
        }
        if((battle_units[battle_unit_idx].melee_attack_attributes & Att_Illusion) != 0)
        {
            attack_immunities |= USA_IMMUNITY_ILLUSION;
        }
    }
    else
    {
        if((battle_units[battle_unit_idx].ranged_attack_attributes & Att_Illusion) > 0)
        {
            attack_immunities |= USA_IMMUNITY_ILLUSION;
        }
        if((battle_units[battle_unit_idx].ranged_type / 10) == rag_Missile)
        {
            attack_immunities |= USA_IMMUNITY_MISSILES;
        }
        if(battle_units[battle_unit_idx].ranged_type == srat_FireBreath)
        {
            attack_immunities |= USA_IMMUNITY_FIRE;
        }
        if((battle_units[battle_unit_idx].ranged_type / 10) == rag_Magic)
        {
            attack_immunities |= USA_IMMUNITY_MAGIC;
        }
        // rag_Boulder, rag_Missile ... short-range or thrown?
        // BUGBUG  should apply to rag_Short or srat_Thrown?  if srat_Thrown, needs to be type, not group
        if(
            (
                ((battle_units[battle_unit_idx].ranged_type / 10) < rag_Magic)
                ||
                ((battle_units[battle_unit_idx].ranged_type / 10) == srat_Thrown)
            )
            &&
            (battle_units[battle_unit_idx].Weapon_Plus1 == 0)
        )
        {
            attack_immunities |= USA_IMMUNITY_WEAPON;
        }
        
    }
    return attack_immunities;
}


// WZD o122p06
void Battle_Unit_Attack_Target(int16_t attacker_battle_unit_idx, int16_t defender_battle_unit_idx, int16_t defender_damage_types[], int16_t attacker_damage_types[], int16_t ranged_attack_flag, int16_t resolve_for_real)
{
    int16_t has_haste_and_shot = 0;
    int16_t attacker_damage_taken = 0;
    int16_t feared_figure_count = 0;
    int16_t ranged_attack_check = 0;
    int16_t counter_attack_figures = 0;
    int16_t defender_damage_total = 0;
    int16_t damage_types[NUM_DAMAGE_TYPES] = { 0, 0, 0 };
    int16_t itr_damage_types = 0;
    feared_figure_count = 0;
    for(itr_damage_types = 0; itr_damage_types < NUM_DAMAGE_TYPES; itr_damage_types++)
    {
        defender_damage_types[itr_damage_types] = 0;
        attacker_damage_types[itr_damage_types] = 0;
    }
    attacker_damage_taken = 0;
    // {0: allow, 1: disallow, 2: disallow}
    ranged_attack_check = Check_Attack_Ranged(attacker_battle_unit_idx, defender_battle_unit_idx);
    if(ranged_attack_flag != ST_TRUE)
    {
        // ; if the unit has a short range attack, process it and, if it is hasted and the attack is not a gaze, repeat it once more
        if(battle_units[attacker_battle_unit_idx].ranged_type >= srat_Thrown)
        {
            Battle_Unit_Process_Attack(attacker_battle_unit_idx, battle_units[attacker_battle_unit_idx].figure_cnt, defender_battle_unit_idx, am_ThrownOrBreath, &damage_types[0], 0, resolve_for_real);
            for(itr_damage_types = 0; itr_damage_types < NUM_DAMAGE_TYPES; itr_damage_types++)
            {
                defender_damage_types[itr_damage_types] += damage_types[itr_damage_types];
            }
            if(
                ((battle_units[attacker_battle_unit_idx].combat_effects & bue_Haste) != 0)
                &&
                (battle_units[attacker_battle_unit_idx].ranged_type < srat_StoneGaze)
            )
            {
                Battle_Unit_Process_Attack(attacker_battle_unit_idx, battle_units[attacker_battle_unit_idx].figure_cnt, defender_battle_unit_idx, am_ThrownOrBreath, &damage_types[0], 0, resolve_for_real);
                for(itr_damage_types = 0; itr_damage_types < NUM_DAMAGE_TYPES; itr_damage_types++)
                {
                    defender_damage_types[itr_damage_types] += damage_types[itr_damage_types];
                }
            }
        }
        // AFTER
        // if(battle_units[attacker_battle_unit_idx].ranged_type >= srat_Thrown)
        if(
            ((battle_units[defender_battle_unit_idx].combat_effects & bue_Black_Sleep) == 0)
            &&
            (battle_units[defender_battle_unit_idx].ranged_type >= srat_StoneGaze)
        )
        {
            defender_damage_total = 0;
            for(itr_damage_types = 0; itr_damage_types < NUM_DAMAGE_TYPES; itr_damage_types++)
            {
                defender_damage_total += defender_damage_types[itr_damage_types];
            }
            defender_damage_total += battle_units[defender_battle_unit_idx].front_figure_damage;
            if(defender_damage_total > 0)
            {
                counter_attack_figures = (battle_units[defender_battle_unit_idx].figure_cnt - (defender_damage_total / battle_units[defender_battle_unit_idx].hits));
            }
            else
            {
                counter_attack_figures = battle_units[defender_battle_unit_idx].figure_cnt;
            }
            Battle_Unit_Process_Attack(defender_battle_unit_idx, counter_attack_figures, attacker_battle_unit_idx, am_ThrownOrBreath, &damage_types[0], 1, resolve_for_real);
            // ; transfer the damage to the counter attack damage return array, and sum it into a local variable too
            for(itr_damage_types = 0; itr_damage_types < NUM_DAMAGE_TYPES; itr_damage_types++)
            {
                attacker_damage_types[itr_damage_types] += damage_types[itr_damage_types];
                attacker_damage_taken += damage_types[itr_damage_types];
            }
        }
        // Wall of Fire, First Strike, Cause Fear
        if(resolve_for_real != ST_FALSE)
        {
            Check_Wall_Of_Fire_Attack(attacker_battle_unit_idx);
        }
        if(battle_units[attacker_battle_unit_idx].status == bus_Active)
        {
            // ; if the attacking unit has First Strike, and the defender can't negate it, process an attack before the defender could retaliate
            /*
                BEGIN: Melee
            */
            /*
                BEGIN: Attacker First-Strike
            */
            if(
                ((battle_units[attacker_battle_unit_idx].attack_attributes & Att_1stStrike) != 0)
                &&
                ((battle_units[defender_battle_unit_idx].Abilities & UA_NEGATEFIRSTSTRIKE) == 0)
            )
            {
                if(resolve_for_real == ST_TRUE)
                {
                    /* First-Strike: apply Attacker's Fear Effect to Defender's Figures */
                    /* OGBUG  this feared_figure_count should be applied to the defender */
                    feared_figure_count = Apply_Fear_Attack(attacker_battle_unit_idx, defender_battle_unit_idx);
                }
                for(itr_damage_types = 0; itr_damage_types < NUM_DAMAGE_TYPES; itr_damage_types++)
                {
                    /* OGBUG  not all gaze attacks and associated effects deal figure-based damage */
                    feared_figure_count += (attacker_damage_types[itr_damage_types] / battle_units[attacker_battle_unit_idx].hits);
                }
                Battle_Unit_Process_Attack(attacker_battle_unit_idx, (battle_units[attacker_battle_unit_idx].figure_cnt - feared_figure_count), defender_battle_unit_idx, am_Melee, &damage_types[0], 0, resolve_for_real);
                for(itr_damage_types = 0; itr_damage_types < NUM_DAMAGE_TYPES; itr_damage_types++)
                {
                    defender_damage_types[itr_damage_types] += damage_types[itr_damage_types];
                }
            }
            /*
                END: Attacker First-Strike
            */
            /*
                BEGIN: Defender Counter-Attack
            */
            if((battle_units[defender_battle_unit_idx].combat_effects & bue_Black_Sleep) == 0)
            {
                // ; sum up the damage taken by the target unit so far
                defender_damage_total = 0;
                for(itr_damage_types = 0; itr_damage_types < NUM_DAMAGE_TYPES; itr_damage_types++)
                {
                    defender_damage_total += defender_damage_types[itr_damage_types];
                }
                defender_damage_total += battle_units[defender_battle_unit_idx].front_figure_damage;
                if(defender_damage_total > 0)
                {
                    counter_attack_figures = (battle_units[defender_battle_unit_idx].figure_cnt - (defender_damage_total / battle_units[defender_battle_unit_idx].hits));
                }
                else
                {
                    counter_attack_figures = battle_units[defender_battle_unit_idx].figure_cnt;
                }
                if(resolve_for_real == ST_TRUE)
                {
                    counter_attack_figures -= Apply_Fear_Attack(attacker_battle_unit_idx, defender_battle_unit_idx);
                }
                if(counter_attack_figures > 0)
                {
                    Battle_Unit_Process_Attack(defender_battle_unit_idx, counter_attack_figures, attacker_battle_unit_idx, am_Melee, &damage_types[0], 1, resolve_for_real);
                    for(itr_damage_types = 0; itr_damage_types < NUM_DAMAGE_TYPES; itr_damage_types++)
                    {
                        attacker_damage_types[itr_damage_types] += damage_types[itr_damage_types];
                    }
                    if((battle_units[defender_battle_unit_idx].combat_effects & bue_Haste) != 0)
                    {
                        Battle_Unit_Process_Attack(defender_battle_unit_idx, counter_attack_figures, attacker_battle_unit_idx, am_Melee, &damage_types[0], 1, resolve_for_real);
                        for(itr_damage_types = 0; itr_damage_types < NUM_DAMAGE_TYPES; itr_damage_types++)
                        {
                            attacker_damage_types[itr_damage_types] += damage_types[itr_damage_types];
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
                ((battle_units[attacker_battle_unit_idx].attack_attributes & Att_1stStrike) == 0)
                ||
                ((battle_units[defender_battle_unit_idx].Abilities & UA_NEGATEFIRSTSTRIKE) != 0)
            )
            {
                if(resolve_for_real == ST_TRUE)
                {
                    /* OGBUG  this feared_figure_count should be applied to the defender */
                    feared_figure_count = Apply_Fear_Attack(attacker_battle_unit_idx, defender_battle_unit_idx);
                }
                attacker_damage_taken += battle_units[attacker_battle_unit_idx].front_figure_damage;
                feared_figure_count += (attacker_damage_taken / battle_units[attacker_battle_unit_idx].hits);
                Battle_Unit_Process_Attack(attacker_battle_unit_idx, (battle_units[attacker_battle_unit_idx].figure_cnt - feared_figure_count), defender_battle_unit_idx, am_Melee, &damage_types[0], 0, resolve_for_real);
                for(itr_damage_types = 0; itr_damage_types < NUM_DAMAGE_TYPES; itr_damage_types++)
                {
                    defender_damage_types[itr_damage_types] += damage_types[itr_damage_types];
                }
                if((battle_units[attacker_battle_unit_idx].combat_effects & bue_Haste) != 0)
                {
                    Battle_Unit_Process_Attack(attacker_battle_unit_idx, (battle_units[attacker_battle_unit_idx].figure_cnt - feared_figure_count), defender_battle_unit_idx, am_Melee, &damage_types[0], 0, resolve_for_real);
                    for(itr_damage_types = 0; itr_damage_types < NUM_DAMAGE_TYPES; itr_damage_types++)
                    {
                        defender_damage_types[itr_damage_types] += damage_types[itr_damage_types];
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
            // ; subtract 5 from each type of damage in the target damage array
            // ; BUG...?
            for(itr_damage_types = 0; itr_damage_types < NUM_DAMAGE_TYPES; itr_damage_types++)
            {
                defender_damage_types[itr_damage_types] += -5;
            }
        }
        else
        {
            Battle_Unit_Process_Attack(attacker_battle_unit_idx, battle_units[attacker_battle_unit_idx].figure_cnt, defender_battle_unit_idx, am_Ranged, &damage_types[0], 0, resolve_for_real);
            for(itr_damage_types = 0; itr_damage_types < NUM_DAMAGE_TYPES; itr_damage_types++)
            {
                defender_damage_types[itr_damage_types] += damage_types[itr_damage_types];
            }
            if(battle_units[attacker_battle_unit_idx].combat_effects & bue_Haste)
            {
                has_haste_and_shot = ST_FALSE;
                if(
                    (battle_units[itr_damage_types].ranged_type / 10 == rag_Magic)  /* OGBUG  should use attacker_battle_unit_idx, not itr_damage_types */
                    ||
                    ((battle_units[attacker_battle_unit_idx].Attribs_1 & (USA_CASTER_20 | USA_CASTER_40)) != 0)
                )
                {
                    if(battle_units[attacker_battle_unit_idx].mana > 6)
                    {
                        battle_units[attacker_battle_unit_idx].mana -= 3;
                        has_haste_and_shot = ST_TRUE;
                    }
                }
                else
                {
                    if(battle_units[attacker_battle_unit_idx].ammo > 1)
                    {
                        has_haste_and_shot = ST_TRUE;
                        battle_units[attacker_battle_unit_idx].ammo--;
                    }
                }
                if(has_haste_and_shot == ST_TRUE)
                {
                    Battle_Unit_Process_Attack(attacker_battle_unit_idx, battle_units[attacker_battle_unit_idx].figure_cnt, defender_battle_unit_idx, am_Ranged, &damage_types[0], 0, resolve_for_real);
                    for(itr_damage_types = 0; itr_damage_types < NUM_DAMAGE_TYPES; itr_damage_types++)
                    {
                        defender_damage_types[itr_damage_types] += damage_types[itr_damage_types];
                    }
                }
            }
        }
    }
}


// WZD o122p07
/* OGBUG  the Power Drain item power is ignored here */
void Battle_Unit_Process_Attack(int16_t attacker_battle_unit_idx, int16_t figure_count, int16_t defender_battle_unit_idx, int16_t attack_mode, int16_t damage_types[], int16_t is_counter_attack, int16_t resolve_for_real)
{
    int16_t resistance_modifier = 0;
    uint32_t defender_enchantments = 0;
    int16_t new_damage_array[NUM_DAMAGE_TYPES] = { 0, 0, 0 };  // regular, drain, irreversible
    int16_t defender_front_figure_damage = 0;
    int16_t accumulated_figure_damage = 0;
    int16_t defender_toblock = 0;
    int16_t niu_var = 0;  // NIU  set to ST_FALSE in two places, but never checked
    int16_t attack_tohit = 0;
    int16_t attack_immunities = 0;
    int16_t noninnate_attack_attributes = 0;
    int16_t attack_attributes = 0;
    int16_t attack_strength = 0;  /* battle_units[].melee OR battle_units[].ranged OR ... */
    int16_t attack_magic_realm = 0;
    int16_t attack_type = 0;
    int16_t defense_special = 0;
    int16_t range_tohit_penalty = 0;
    int16_t remaining_attack_damage = 0;
    int16_t blur_rolls = 0;
    int16_t itr2 = 0;
    int16_t healing = 0;  // Dasm reuses itr2
    int16_t itr = 0;
    defender_front_figure_damage = battle_units[defender_battle_unit_idx].front_figure_damage;
    defender_enchantments = (_UNITS[battle_units[defender_battle_unit_idx].unit_idx].enchantments | battle_units[defender_battle_unit_idx].enchantments | battle_units[defender_battle_unit_idx].item_enchantments);
    niu_var = 0;
    attack_strength = 0;
    remaining_attack_damage = 0;
    attack_tohit = battle_units[attacker_battle_unit_idx].tohit;
    for(itr = 0; itr < 3; itr++)
    {
        damage_types[itr] = 0;
    }
    if(figure_count <= 0)
    {
        return;
    }
    attack_attributes = battle_units[attacker_battle_unit_idx].attack_attributes;
    defender_toblock = battle_units[defender_battle_unit_idx].toblock;
    attack_immunities = Battle_Unit_Attack_Immunities(attacker_battle_unit_idx, attack_mode);
    if(attack_mode <= am_Melee)
    {
        attack_attributes |= battle_units[attacker_battle_unit_idx].melee_attack_attributes;
        noninnate_attack_attributes = battle_units[attacker_battle_unit_idx].melee_attack_attributes;
        /* OGBUG  double counted in defender_toblock */
        attack_tohit += (battle_units[attacker_battle_unit_idx].melee_tohit - battle_units[defender_battle_unit_idx].toblock);
        attack_strength = battle_units[attacker_battle_unit_idx].melee;
        attack_magic_realm = Battle_Unit_Attack_Magic_Realm(0, attacker_battle_unit_idx);
        attack_type = am_Melee;
    }
    else
    {
        attack_attributes |= battle_units[attacker_battle_unit_idx].ranged_attack_attributes;
        noninnate_attack_attributes = battle_units[attacker_battle_unit_idx].ranged_attack_attributes;
        attack_strength = battle_units[attacker_battle_unit_idx].ranged;
        attack_magic_realm = Battle_Unit_Attack_Magic_Realm(battle_units[attacker_battle_unit_idx].ranged_type, attacker_battle_unit_idx);
        attack_type = battle_units[attacker_battle_unit_idx].ranged_type;
        if(attack_type == srat_Lightning)
        {
            attack_attributes |= Att_ArmorPrc;
        }
        if(attack_type == srat_MultiGaze)
        {
            attack_attributes |= Att_DoomDmg;
        }
        /* OGBUG  fails to apply ranged To Hit modifiers to short range Thrown and Breath attacks */
        if(
            (attack_mode == am_Ranged)
            &&
            (Battle_Unit_Has_Ranged_Attack(attacker_battle_unit_idx) != ST_FALSE)
        )
        {
            attack_tohit += battle_units[attacker_battle_unit_idx].ranged_tohit;
            if((battle_units[attacker_battle_unit_idx].ranged_type / 10) != rag_Magic)
            {
                range_tohit_penalty = (Range_To_Battle_Unit(attacker_battle_unit_idx, defender_battle_unit_idx) / 3);
                if(
                    ((battle_units[attacker_battle_unit_idx].Abilities & UA_LONGRANGE) != 0)
                    &&
                    (range_tohit_penalty > 0)
                )
                {
                    range_tohit_penalty = 1;
                }
                attack_tohit -= range_tohit_penalty;
            }
        }
        else
        {
            attack_strength = 0;
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
        attack_tohit -= 1;
    }
    if(is_counter_attack == ST_TRUE)
    {
        attack_tohit -= (battle_units[attacker_battle_unit_idx].Suppression / 2);
    }
    if(
        ((battle_units[defender_battle_unit_idx].Move_Flags & MV_FLYING) != 0)
        &&
        (attack_mode == am_Melee)
        &&
        ((battle_units[attacker_battle_unit_idx].Move_Flags & MV_FLYING) == 0)
        &&
        (is_counter_attack != ST_TRUE)
        &&
        (battle_units[attacker_battle_unit_idx].ranged_type < srat_Thrown)
    )
    {
        return;
    }
    if((battle_units[defender_battle_unit_idx].Attribs_1 & USA_IMMUNITY_MAGIC) == 0)
    {
        /*
            Immolation . . . . . . . . Every opponent engaged in melee combat is subjected to a strength four fire attack; this occurs at the same time and in addition to melee combat.
            ...strength four fire attack...
            ...occurs at the same time and in addition to melee combat
        */
        if((battle_units[attacker_battle_unit_idx].Attribs_2 & USA_IMMOLATION) != 0)
        {
            // sets damage_types[0]
            Compute_Battle_Unit_Damage_From_Spell(spl_Fireball, defender_battle_unit_idx, &new_damage_array[0], 4);
        }
        // Stonig Gaze - before melee combat
        if(
            (
                (attack_type == srat_StoneGaze)
                ||
                (attack_type == srat_MultiGaze)
            )
            &&
            ((battle_units[defender_battle_unit_idx].Attribs_1 & USA_IMMUNITY_STONING) == 0)
        )
        {
            for(itr = 0; battle_units[defender_battle_unit_idx].figure_cnt > itr; itr++)
            {
                if(Combat_Resistance_Check(battle_units[defender_battle_unit_idx], -(abs(battle_units[attacker_battle_unit_idx].Spec_Att_Attrib)), sbr_Nature) > 0)
                {
                    new_damage_array[2] += battle_units[defender_battle_unit_idx].hits;
                }
            }
        }
        // Death Gaze - before melee combat
        if(
            (
                (attack_type == srat_MultiGaze)
                ||
                (attack_type == srat_DeathGaze)
            )
            &&
            ((battle_units[defender_battle_unit_idx].Attribs_1 & USA_IMMUNITY_DEATH) == 0)
        )
        {
            for(itr = 0; battle_units[defender_battle_unit_idx].figure_cnt > itr; itr++)
            {
                if(Combat_Resistance_Check(battle_units[defender_battle_unit_idx], -(abs(battle_units[attacker_battle_unit_idx].Spec_Att_Attrib)), sbr_Death) > 0)
                {
                    new_damage_array[0] += battle_units[defender_battle_unit_idx].hits;
                }
            }
        }
    }
    if((battle_units[defender_battle_unit_idx].combat_effects & bue_Black_Sleep) != 0)
    {
        attack_attributes |= Att_DoomDmg;
    }
    if(attack_strength <= 0)
    {
        return;
    }
    /*
        BEGIN:  Defense Special
    */
    defense_special = Battle_Unit_Defense_Special(defender_battle_unit_idx, attack_type, attack_immunities, attack_attributes, attack_magic_realm);
    if((attack_attributes & Att_EldrWeap) != 0)
    {
        defender_toblock -= 1;
    }
    // BUGBUG  broken logic, bad flag value; gets +1 in non-City combat
    if(
        (Battle_Unit_Is_Within_City(defender_battle_unit_idx) == ST_TRUE)
        &&
        (Battle_Unit_Is_Within_City(attacker_battle_unit_idx) == ST_FALSE)
        &&
        (battlefield->walled != ST_FALSE)
    )
    {
        if(Combat_Grid_Cell_Has_City_Wall(battle_units[defender_battle_unit_idx].cgx, battle_units[defender_battle_unit_idx].cgy) != ST_FALSE)
        {
            defense_special += 3;
        }
        else
        {
            defense_special += 1;
        }
    }
    /*
        END:  Defense Special
    */
    for(itr = 0; itr < figure_count; itr++)
    {
        if((battle_units[defender_battle_unit_idx].Attribs_1 & USA_IMMUNITY_MAGIC) == 0)
        {
            // process the Dispel Evil touch attack if applicable
            /* OGBUG  Spell Lock does not protect from this, even though it does from the spell versions */
            if(
                ((attack_attributes & Att_DsplEvil) != 0)
                &&
                (
                    (battle_units[defender_battle_unit_idx].race == rt_Chaos)
                    ||
                    (battle_units[defender_battle_unit_idx].race == rt_Death)
                )
            )
            {
                resistance_modifier = -4;
                if((_UNITS[battle_units[defender_battle_unit_idx].unit_idx].mutations & UM_UNDEAD) != 0)
                {
                    resistance_modifier -= 5;
                }
                if(Combat_Resistance_Check(battle_units[defender_battle_unit_idx], resistance_modifier, sbr_Life) > 0)
                {
                    new_damage_array[2] += battle_units[defender_battle_unit_idx].hits;
                }
            }
            if(
                ((attack_attributes & Att_StnTouch) != 0)
                &&
                ((battle_units[defender_battle_unit_idx].Attribs_1 & USA_IMMUNITY_STONING) == 0)
            )
            {
                resistance_modifier = -(abs(battle_units[attacker_battle_unit_idx].Spec_Att_Attrib));
                if((noninnate_attack_attributes & Att_StnTouch) != 0)
                {
                    resistance_modifier -= 1;
                }
                if(Combat_Resistance_Check(battle_units[defender_battle_unit_idx], resistance_modifier, sbr_Nature) > 0)
                {
                    new_damage_array[2] += battle_units[defender_battle_unit_idx].hits;
                }
            }
            if(
                ((attack_attributes & Att_DthTouch) != 0)
                &&
                ((battle_units[defender_battle_unit_idx].Attribs_1 & USA_IMMUNITY_DEATH) == 0)
            )
            {
                resistance_modifier = -(abs(battle_units[attacker_battle_unit_idx].Spec_Att_Attrib));
                if((noninnate_attack_attributes & Att_DthTouch) != 0)
                {
                    resistance_modifier -= 3;
                }
                if(Combat_Resistance_Check(battle_units[defender_battle_unit_idx], resistance_modifier, sbr_Death) > 0)
                {
                    new_damage_array[0] += battle_units[defender_battle_unit_idx].hits;
                }
            }
            if(
                ((attack_attributes & Att_LifeSteal) != 0)
                &&
                ((battle_units[defender_battle_unit_idx].Attribs_1 & USA_IMMUNITY_DEATH) == 0)
            )
            {
                resistance_modifier = -(abs(battle_units[attacker_battle_unit_idx].Spec_Att_Attrib));
                if((noninnate_attack_attributes & Att_LifeSteal) != 0)
                {
                    resistance_modifier -= 3;
                }
                healing = Combat_Resistance_Check(battle_units[defender_battle_unit_idx], resistance_modifier, sbr_Death);
                new_damage_array[1] += healing;
                if(resolve_for_real != ST_FALSE)
                {
                    Battle_Unit_Heal(attacker_battle_unit_idx, healing, 1);
                }
            }
            if((attack_attributes & Att_Destruct) != 0)
            {
                if(Combat_Resistance_Check(battle_units[defender_battle_unit_idx], 0, sbr_Chaos) > 0)
                {
                    new_damage_array[2] += battle_units[defender_battle_unit_idx].hits;
                }
            }
        }
        if((attack_attributes & Att_DoomDmg) != 0)
        {
            if((noninnate_attack_attributes & Att_DoomDmg) != 0)
            {
                remaining_attack_damage = (attack_strength / 2);
            }
            else
            {
                remaining_attack_damage = attack_strength;
            }
        }
        else
        {
            remaining_attack_damage += Combat_Roll_Damage_Dealt(attack_strength, attack_tohit);
            /* OGBUG  Blur checks immunity on the wrong unit - USA_IMMUNITY_ILLUSION check should be on attacker_battle_unit_idx */
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
                for(blur_rolls = 0; blur_rolls < remaining_attack_damage; blur_rolls++)
                {
                    if(Random(10) == 10)
                    {
                        /* OGBUG  this also alters the loop condition, which limits the total reduction to at most 50% */
                        remaining_attack_damage -= 1;
                    }
                }
            }
            // attack units minus defense units
            remaining_attack_damage -= Combat_Roll_Damage_Blocked(defense_special, defender_toblock);
            if((defender_enchantments & UE_INVULNERABILITY) != 0)
            {
                remaining_attack_damage -= 2;
            }
            SETMIN(remaining_attack_damage, 0);
        }
        // ; process Poison Touch, if applicable
        if(
            ((battle_units[attacker_battle_unit_idx].attack_attributes & Att_Poison) != 0)
            &&
            ((battle_units[defender_battle_unit_idx].Attribs_1 & USA_IMMUNITY_POISON) == 0)
        )
        {
            for(itr2 = 0; battle_units[attacker_battle_unit_idx].Poison_Strength > itr2; itr2++)
            {
                if(Combat_Resistance_Check(battle_units[defender_battle_unit_idx], 0, -1) > 0)
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
        // ; sum up the damage done so far, and add to it the target's original top figure damage
        accumulated_figure_damage = 0;
        for(itr2 = 0; itr2 < 3; itr2++)
        {
            accumulated_figure_damage += new_damage_array[itr2];
        }
        accumulated_figure_damage += defender_front_figure_damage;
        // ; process the actual attack damage directly into the return array, making extra defense rolls when necessary
        while(battle_units[defender_battle_unit_idx].hits <= (remaining_attack_damage + accumulated_figure_damage))
        {
            remaining_attack_damage -= battle_units[defender_battle_unit_idx].hits;
            if(remaining_attack_damage < 0)
            {
// went negative
// damage < hit - pooints
// alive
                // ; no additional defense rolls required - transfer the attack damage to the total, and add it to the return array
                if(
                    ((battle_units[attacker_battle_unit_idx].Abilities & UA_CREATEUNDEAD) != 0)
                    &&
                    ((battle_units[defender_battle_unit_idx].Attribs_1 & USA_IMMUNITY_MAGIC) == 0)
                    )
                {
                    damage_types[1] += (battle_units[defender_battle_unit_idx].hits + remaining_attack_damage);
                }
                else
                {
                    damage_types[0] += (battle_units[defender_battle_unit_idx].hits + remaining_attack_damage);
                }
                accumulated_figure_damage += remaining_attack_damage;
                remaining_attack_damage = 0;
            }
            else
            {
// didn't go negative
// damage >= hit - pooints
// dead
                // ; figure overkill - transfer Hits worth of damage from the attack damage to the return array, then make a new defense roll to reduce the remainder
                if(
                    ((battle_units[attacker_battle_unit_idx].Abilities & UA_CREATEUNDEAD) != 0)
                    &&
                    ((battle_units[defender_battle_unit_idx].Attribs_1 & USA_IMMUNITY_MAGIC) == 0)
                    )
                {
                    damage_types[1] += battle_units[defender_battle_unit_idx].hits;
                }
                else
                {
                    damage_types[0] += battle_units[defender_battle_unit_idx].hits;
                }
                if((attack_attributes & Att_DoomDmg) == 0)
                {
                    remaining_attack_damage = Combat_Roll_Damage_Blocked(defense_special, defender_toblock);
                    if((defender_enchantments & UE_INVULNERABILITY) != 0)
                    {
                        remaining_attack_damage -= 2;
                    }
                    SETMIN(remaining_attack_damage, 0);
                }
            }
        }  /* while(battle_units[defender_battle_unit_idx].hits > (remaining_attack_damage + accumulated_figure_damage)) */
        if(
            ((battle_units[attacker_battle_unit_idx].Abilities & UA_CREATEUNDEAD) != 0)
            &&
            ((battle_units[defender_battle_unit_idx].Attribs_1 & USA_IMMUNITY_MAGIC) == 0)
        )
        {
            damage_types[1] += remaining_attack_damage;
        }
        else
        {
            damage_types[0] += remaining_attack_damage;
        }
        defender_front_figure_damage = (accumulated_figure_damage + remaining_attack_damage);
        remaining_attack_damage = 0;
        niu_var = 0;
        for(itr2 = 0; itr2 < 3; itr2++)
        {
            damage_types[itr2] += new_damage_array[itr2];
            new_damage_array[itr2] = 0;
        }
    }  /* END: for(itr = 0; itr < figure_count; itr++) */
}


// WZD o122p08
int16_t Battle_Unit_Defense_Special(int16_t battle_unit_idx, int16_t attack_type, int16_t attack_immunities, int16_t attack_attributes, int16_t magic_realm)
{
    int16_t Immunity_Type = 0;
    uint32_t enchantments = 0;
    int16_t effective_defense = 0;
    enchantments = (_UNITS[battle_units[battle_unit_idx].unit_idx].enchantments | battle_units[battle_unit_idx].enchantments | battle_units[battle_unit_idx].item_enchantments);
    effective_defense = battle_units[battle_unit_idx].defense;
    /*
        BEGIN:  'Immunity to Illusion' && 'Illusory Attack'
    */
    if(((attack_immunities & battle_units[battle_unit_idx].Attribs_1) & USA_IMMUNITY_ILLUSION) != 0)
    {
        attack_immunities ^= USA_IMMUNITY_ILLUSION;
    }
    if(attack_immunities & USA_IMMUNITY_ILLUSION)
    {
        return 0;
    }
    /*
        END:  'Immunity to Illusion' && 'Illusory Attack'
    */
    /*
        BEGIN:  'Large Shield'
                +2 against all ranged attacks (including missile, magic, rocks) and thrown and breath 
    */
    if(
        ((battle_units[battle_unit_idx].Abilities & UA_LARGESHIELD) > 0)
        &&
        (attack_type != am_Melee)
    )
    {
        effective_defense += 2;
    }
    /*
        END:  'Large Shield'
    */
    /*
        BEGIN:  Ranged - 'Immunity Fire', 'Immunity Stoning', 'Immunity Missiles', 'Immunity Cold', 'Immunity Magic', 'Immunity Death', 'Immunity Poison', 'Immunity Weapon'
    */
    // ¿ BUGBUG  should include magic_realm condition to exclude 'Immunity Magic' ?
    if((attack_immunities & battle_units[battle_unit_idx].Attribs_1) != 0)
    {
        if(attack_type != am_Melee)
        {
            Immunity_Type = 2;
        }
    }
    /*
        END:  Ranged - 'Immunity Fire', 'Immunity Stoning', 'Immunity Missiles', 'Immunity Cold', 'Immunity Magic', 'Immunity Death', 'Immunity Poison', 'Immunity Weapon'
    */
    /*
        BEGIN:  Melee - 'Immunity Weapon'
    */
    if(attack_immunities & battle_units[battle_unit_idx].Attribs_1 & USA_IMMUNITY_WEAPON)
    {
        Immunity_Type = 1;  // ""...unit’s defense (shields) increases to 10..."
    }
    /*
        END:  Melee - 'Immunity Weapon'
    */
    /*
        BEGIN:  Ranged - 'Immunity Magic'
    */
    if(
        (battle_units[battle_unit_idx].Attribs_1 & USA_IMMUNITY_MAGIC)
        &&
        (magic_realm > ST_UNDEFINED)
        &&
        (attack_type != am_Melee)
    )
    {
        Immunity_Type = 2;
    }
    /*
        BEGIN:  Ranged - 'Immunity Magic'
    */
    if(
        (
            (magic_realm == sbr_Chaos)
            ||
            (magic_realm == sbr_Death)
        )
    )
    {
        if((enchantments & UE_BLESS) != 0)
        {
            effective_defense += 3;
        }
        // ...complete immunity from all death and chaos magic spells
        if(
            (
                ((_UNITS[battle_units[battle_unit_idx].unit_idx].enchantments & UE_RIGHTEOUSNESS) != 0)
                ||
                ((battle_units[battle_unit_idx].enchantments & UE_RIGHTEOUSNESS) != 0)
            )
            &&
            (attack_type != am_Melee)
        )
        {
            Immunity_Type = 2;
        }
    }
    /*
        BEGIN:  'Elemental Armor', 'Resist Elements'
    */
    if(
        (
            (magic_realm == sbr_Chaos)
            ||
            (magic_realm == sbr_Nature)
        )
        &&
        (attack_type != am_Melee)
    )
    {
        if((enchantments & UE_ELEMENTAL_ARMOR) != 0)
        {
            effective_defense += 10;
        }
        else if((enchantments & UE_RESIST_ELEMENTS))
        {
            effective_defense += 3;
        }
    }
    /*
        END:  'Elemental Armor', 'Resist Elements'
    */
    /*
        BEGIN:  'Armor-Piercing Attack'
                halves defense (shields) of defender (rounded down)
    */
    if((attack_attributes & Att_ArmorPrc) != 0)
    {
        effective_defense /= 2;
    }
    /*
        END:  'Armor-Piercing Attack'
    */
    if(
        (Immunity_Type == 1)
        &&
        (effective_defense < 10)
    )
    {
        effective_defense = 10;
    }
    if(Immunity_Type == 2)
    {
        return 50;
    }
    return effective_defense;
}

// WZD o122p09
/**
 * @brief Determines the magic realm associated with a battle unit's attack type.
 *
 * Maps attack types (rock, lightning, fireball, etc.) to their corresponding magic realms.
 * For non-magical attacks (rat_NONE), returns the magic realm based on the unit's racial affiliation.
 * Physical attacks with no magical component return sbr_NONE.
 *
 * @param attack_type The attack type (e.g., rat_Lightning, rat_Fireball, rat_Sorcery).
 * @param battle_unit_idx Index of the battle unit performing the attack.
 *
 * @return The spell school/magic realm (sbr_*) associated with the attack, or ST_UNDEFINED if unknown.
 */
int16_t Battle_Unit_Attack_Magic_Realm(int16_t attack_type, int16_t battle_unit_idx)
{
    int16_t sw_attack_type = 0;
    int16_t magic_realm = 0;
    magic_realm = ST_UNDEFINED;
    sw_attack_type = attack_type;
    switch(sw_attack_type)
    {
        case rat_NONE:
        {
            if(battle_units[battle_unit_idx].race < rt_Arcane)
            {
                magic_realm = sbr_NONE;
            }
            else if(battle_units[battle_unit_idx].race == rt_Arcane)
            {
                magic_realm = sbr_Arcane;
            }
            else
            {
                magic_realm = (battle_units[battle_unit_idx].race - rt_Nature);
            }
        } break;
        case rat_Rock:
        case rat_Cannon:
        case rat_Bow:
        case rat_Sling:
        case rat_Unknown:
        case srat_Thrown:
        {
            magic_realm = sbr_NONE;
        } break;
        case rat_Lightning:
        case rat_Fireball:
        case rat_Deathbolt:
        case rat_Drow:
        case srat_FireBreath:
        case srat_Lightning:
        case srat_MultiGaze:
        {
            magic_realm = sbr_Chaos;
        } break;
        case rat_Sorcery:
        {
            magic_realm = sbr_Sorcery;
        } break;
        case rat_Icebolt:
        case rat_Pr_Shaman:
        case rat_Sprite:
        case rat_Nat_Bolt:
        case srat_StoneGaze:
        {
            magic_realm = sbr_Nature;
        } break;
        case srat_DeathGaze:
        {
            magic_realm = sbr_Death;
        } break;
    }
    return magic_realm;
}


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
    combat_enchantments, CMB_HolyBonusArray, CMB_ResistAllArray, _battlefield_leadership, battle_units[].defense
    combat_enchantments - Darkness, True_Light
    +3 Defender defense, if combat_structure == 1
    Hero - Prayer Master
    Hero - Leadership
    CMB_ResistAllArray += CMB_HolyBonusArray

Holy Bonus . . . . . . . . Increases attack strength (swords), defense (shields) and resistance (crosses) of all friendly units in combat by bonus level of holy bonus; only the best Holy Bonus applies at any one time.

*Leadership. . . . . . . . Increases melee attack strength by one per three experience levels of the hero to all normal units on the battlefield; note that Undead, Black Channeled or Chaos Channeled units which were once normal do not get this bonus. Only the highest Leadership bonus applies.

*Prayer Master . . . . . . Increases all resistance rolls for all units on the battlefield by one (10%) per experience level. Only the best Prayer Master or Resistance to All applies.


Cloud of Shadow:
Death. City Enchantment. Casting Cost: 150 mana; Upkeep: 3 mana/turn. Rare.
Envelops the target friendly city in a dense cloud of darkness (equivalent in effect to the darkness spell). All combat in the city takes place under this effect.

Darkness:
Death. Combat Enchantment. Casting Cost: 25 mana. Common.
Drapes a shroud of darkness over the entire battlefield, inspiring creatures of death who gain one point each in attack strength (swords), defense (shields) and resistance (crosses). Creatures of life, on the other hand, are dispirited and lose one point each in attack strength, defense and resistance.

*/
/*
    handles Cloud of Darkness, City Walls, Holy Bonus, Leadershipd, Prayer Master/Resistance to All

*/
/**
 * @brief Calculates and applies various battlefield bonuses and enchantment effects.
 *
 * Processes overland enchantments (Eternal Night, Heavenly Light) and hero abilities (Prayer Master,
 * Resistance to All, Leadership, Holy Bonus) to update battlefield bonuses for all players and units.
 * Updates combat enchantment arrays and hero ability bonuses based on battle unit states and city conditions.
 *
 * @param combat_structure Combat structure type (e.g., cs_City for city combat).
 *
 * @note Updates global arrays: _battlefield_holybonus[], _battlefield_resistall[], _battlefield_leadership[].
 * @note Updates global array: combat_enchantments[] for overland enchantment effects.
 * @note Also applies city wall defense bonus to defender units in city combat.
 */
void Calc_Battlefield_Bonuses(int16_t combat_structure)
{
    int16_t leadership_level = 0;
    int16_t prayer_level = 0;
    int16_t itr_players = 0;
    int16_t itr_arrays = 0;
    int16_t itr_battle_units = 0;
    int16_t battle_unit_owner_idx = 0;
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
            if(combat_enchantments[TRUE_LIGHT_DFNDR] != 2)
            {
                if(itr_players == _combat_attacker_player)
                {
                    combat_enchantments[DARKNESS_ATTKR] = 3;
                }
                else if(itr_players == _combat_defender_player)
                {
                    combat_enchantments[DARKNESS_DFNDR] = 3;
                }
                else
                {
                    combat_enchantments[DARKNESS_DFNDR] = 3;
                    combat_enchantments[DARKNESS_ATTKR] = 3;
                }
            }
        }
    }
    for(itr_arrays = 0; itr_arrays < (NUM_PLAYERS + 1); itr_arrays++)
    {
        _battlefield_holybonus[itr_arrays] = 0;
        _battlefield_resistall[itr_arrays] = 0;
        _battlefield_leadership[itr_arrays] = 0;
    }
    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
    {
        leadership_level = 0;
        if(battle_units[itr_battle_units].status == bus_Active)
        {
            /* OGBUG: should be checking for City Wall c&p error from 'Strategic Combat'? */
            if(
                (combat_structure == cs_City)  /* probably just {F,T} */
                &&
                (battle_units[itr_battle_units].controller_idx == _combat_defender_player)
            )
            {
                battle_units[itr_battle_units].defense += 3;
            }
            battle_unit_owner_idx = battle_units[itr_battle_units].controller_idx;
            if((battle_units[itr_battle_units].Attribs_2 & USA_HOLYBONUS) != 0)
            {
                // DEDU  what is it testing/setting here? what's in Spec_Att_Attrib, how'd it get there - memcpy?
                if(battle_units[itr_battle_units].Spec_Att_Attrib > _battlefield_holybonus[battle_unit_owner_idx])
                {
                    _battlefield_holybonus[battle_unit_owner_idx] = battle_units[itr_battle_units].Spec_Att_Attrib;
                }
            }
            if((battle_units[itr_battle_units].Attribs_2 & USA_RESISTALL) != 0)
            {
                // DEDU  what is it testing/setting here? what's in Spec_Att_Attrib, how'd it get there - memcpy?
                if(battle_units[itr_battle_units].Spec_Att_Attrib > _battlefield_resistall[battle_unit_owner_idx])
                {
                    _battlefield_resistall[battle_unit_owner_idx] = battle_units[itr_battle_units].Spec_Att_Attrib;
                }
            }
            if(_UNITS[battle_units[itr_battle_units].unit_idx].Hero_Slot >= 0)
            {
                // Prayermaster
                if(HERO_PRAYERMASTER(battle_unit_owner_idx, _UNITS[battle_units[itr_battle_units].unit_idx].type))
                {
                    prayer_level = (_UNITS[battle_units[itr_battle_units].unit_idx].Level + 1);
                    if(prayer_level > _battlefield_resistall[battle_unit_owner_idx])
                    {
                        _battlefield_resistall[battle_unit_owner_idx] = prayer_level;
                    }
                }
                if(HERO_PRAYERMASTER2(battle_unit_owner_idx, _UNITS[battle_units[itr_battle_units].unit_idx].type))
                {
                    prayer_level = (((_UNITS[battle_units[itr_battle_units].unit_idx].Level + 1) * 3) / 2);
                    if(prayer_level > _battlefield_resistall[battle_unit_owner_idx])
                    {
                        _battlefield_resistall[battle_unit_owner_idx] = prayer_level;
                    }
                }
                // Leadership
                if(HERO_LEADERSHIP(battle_unit_owner_idx, _UNITS[battle_units[itr_battle_units].unit_idx].type))
                {
                    leadership_level = ((_UNITS[battle_units[itr_battle_units].unit_idx].Level + 1) / 3);
                    if(leadership_level > _battlefield_leadership[battle_unit_owner_idx])
                    {
                        _battlefield_leadership[battle_unit_owner_idx] = leadership_level;
                    }
                }
                if(HERO_LEADERSHIP2(battle_unit_owner_idx, _UNITS[battle_units[itr_battle_units].unit_idx].type))
                {
                    leadership_level = ((_UNITS[battle_units[itr_battle_units].unit_idx].Level + 1) / 2);
                    if(leadership_level > _battlefield_leadership[battle_unit_owner_idx])
                    {
                        _battlefield_leadership[battle_unit_owner_idx] = leadership_level;
                    }
                }
            }
        }
    }
    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        _battlefield_resistall[itr_players] += _battlefield_holybonus[itr_players];
    }
}


// WZD o122p11
/*
; BUG: fails to properly expend the moves of units performing ranged attacks
; INCONSISTENT in its decision-making about whether to melee or shoot at an adjacent target
*/
void Battle_Unit_Attack(int16_t attacker_battle_unit_idx, int16_t defender_battle_unit_idx, int16_t cgx, int16_t cgy)
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
        _ai_immobile_counter = ST_UNDEFINED;
    }
    if(defender_battle_unit_idx != 99)  /* City Wall */
    {
        // WTF? same as was passed in?
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
            if(ranged_attack_check == 0)  /* 0: allow */
            {
                ranged_attack_flag = ST_TRUE;
            }
            else if(ranged_attack_check == 1)  /* 1: disallow (Invisibility) */
            {
                if(
                    (battle_units[attacker_battle_unit_idx].controller_idx == HUMAN_PLAYER_IDX)
                    &&
                    (_auto_combat_flag == ST_FALSE)
                )
                {
                    LBX_Load_Data_Static(message_lbx_file__ovr122, 0, (SAMB_ptr)GUI_NearMsgString, 3, 1, 150);
                    Warn1(GUI_NearMsgString);
                }
                return;
            }
            else if(ranged_attack_check == 2) /* 2: disallow (Wall of Darkness) */
            {
                if(
                    (battle_units[attacker_battle_unit_idx].controller_idx == HUMAN_PLAYER_IDX)
                    &&
                    (_auto_combat_flag == ST_FALSE)
                )
                {
                    LBX_Load_Data_Static(message_lbx_file__ovr122, 0, (SAMB_ptr)GUI_NearMsgString, 4, 1, 150);
                    Warn1(GUI_NearMsgString);
                }
                return;
            }
        }
        if(ranged_attack_check > 0)
        {
            ranged_attack_flag = ST_FALSE;
        }
        battle_units[attacker_battle_unit_idx].target_cgx = battle_units[defender_battle_unit_idx].cgx;
        battle_units[attacker_battle_unit_idx].target_cgy = battle_units[defender_battle_unit_idx].cgy;
        /* CLAUDE: test-only combat-legality logging (LOG_CAT_COMBAT_TEST, off by default).
           A melee attack (mode=melee) with range>1 is an illegal attack -- a unit striking
           a non-adjacent target. */
        if(STU_Log_Category_Enabled(LOG_CAT_COMBAT_TEST))
        {
            LOG_INFO(LOG_CAT_COMBAT_TEST, "ATTACK turn=%d attacker=%d defender=%d attacker_cell=(%d,%d) defender_cell=(%d,%d) range=%d has_ranged=%d action=%d mode=%s", (int)_combat_turn, (int)attacker_battle_unit_idx, (int)defender_battle_unit_idx, (int)battle_units[attacker_battle_unit_idx].cgx, (int)battle_units[attacker_battle_unit_idx].cgy, (int)battle_units[defender_battle_unit_idx].cgx, (int)battle_units[defender_battle_unit_idx].cgy, (int)range_to_target, (int)(Battle_Unit_Has_Ranged_Attack(attacker_battle_unit_idx) != ST_FALSE), (int)battle_units[attacker_battle_unit_idx].action, (ranged_attack_flag == ST_TRUE) ? "ranged" : "melee");
        }
        Battle_Unit_Attack_Target(attacker_battle_unit_idx, defender_battle_unit_idx, &defender_damage_array[0], &attacker_damage_array[0], ranged_attack_flag, 1);
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
            Melee_Animation(attacker_battle_unit_idx, defender_battle_unit_idx, attacker_damage_total, defender_damage_total, battle_units[defender_battle_unit_idx].cgx, battle_units[defender_battle_unit_idx].cgy);
            battle_units[attacker_battle_unit_idx].movement_points -= ((Battle_Unit_Moves2(attacker_battle_unit_idx) + 1) / 2);
            SETMIN(battle_units[attacker_battle_unit_idx].movement_points, 0);
        }
        else  /* (ranged_attack_flag == ST_TRUE) */
        {
            Ranged_Animation(attacker_battle_unit_idx, defender_battle_unit_idx, defender_damage_total, cgx, cgy);
            /* OGBUG  performing a ranged attack is supposed to always end the unit's turn */
            battle_units[attacker_battle_unit_idx].movement_points -= 20;
            SETMIN(battle_units[attacker_battle_unit_idx].movement_points, 0);
            if(
                (
                    (_UNITS[battle_units[attacker_battle_unit_idx].unit_idx].Hero_Slot > ST_UNDEFINED)
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
                    battle_units[attacker_battle_unit_idx].mana = 0;
                }
                if(battle_units[attacker_battle_unit_idx].mana < 3)
                {
                    battle_units[attacker_battle_unit_idx].ranged_type = rat_UNDEF;
                    battle_units[attacker_battle_unit_idx].ranged = 0;
                }
            }
            else
            {
                battle_units[attacker_battle_unit_idx].ammo -= 1;
                if(battle_units[attacker_battle_unit_idx].ammo <= 0)
                {
                    battle_units[attacker_battle_unit_idx].ranged_type = rat_UNDEF;
                    battle_units[attacker_battle_unit_idx].ranged = 0;
                }
            }
        }
        Battle_Unit_Commit_Damage(defender_battle_unit_idx, defender_damage_array);
        Battle_Unit_Commit_Damage(attacker_battle_unit_idx, attacker_damage_array);
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
                if(Random(100) <= 25)  /* 25% probability if ranged attack */
                {
                    Wall_Destroyed = ST_TRUE;
                }
            }
            else
            {
                if(Random(100) <= 50)  /* 50% probability if melee attack */
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
                if(Random(100) <= 25)  /* 25% probability if ranged attack */
                {
                    Wall_Destroyed = ST_TRUE;
                }
                /* OGBUG  only expends 10 movement points instead of all of them, as stated in the v1.2 patch notes */
                Ranged_Animation(attacker_battle_unit_idx, defender_battle_unit_idx, defender_damage_total, cgx, cgy);
                /* OGBUG  performing a ranged attack is supposed to always end the unit's turn */
                battle_units[attacker_battle_unit_idx].movement_points -= 20;
                if(
                    (_UNITS[battle_units[attacker_battle_unit_idx].unit_idx].Hero_Slot > ST_UNDEFINED)
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
                        battle_units[attacker_battle_unit_idx].mana = 0;
                    }
                    if(battle_units[attacker_battle_unit_idx].mana < 5)
                    {
                        battle_units[attacker_battle_unit_idx].ranged_type = rat_UNDEF;
                        battle_units[attacker_battle_unit_idx].ranged = 0;
                    }
                }
                else
                {
                    battle_units[attacker_battle_unit_idx].ammo -= 1;
                    if(battle_units[attacker_battle_unit_idx].ammo <= 0)
                    {
                        battle_units[attacker_battle_unit_idx].ranged_type = rat_UNDEF;
                        battle_units[attacker_battle_unit_idx].ranged = 0;
                    }
                }
            }
            else
            {
                if(Random(100) <= 50)  /* 50% probability if melee attack */
                {
                    Wall_Destroyed = ST_TRUE;
                }
                Melee_Animation(attacker_battle_unit_idx, defender_battle_unit_idx, attacker_damage_total, defender_damage_total, cgx, cgy);
                battle_units[attacker_battle_unit_idx].movement_points -= ((Battle_Unit_Moves2(attacker_battle_unit_idx) + 1) / 2);
            }
        }
        if(Wall_Destroyed != ST_FALSE)
        {
            battlefield->walls[(cgy - 10)][(cgx - 5)] = 2;
        }
    }
    // @@JmpDone_DrawFlip:
    Set_Page_Off();
    Combat_Screen_Draw();
    PageFlip_FX();
}


// WZD o122p12
// ~ MoO2  Module: CMBTAI  Range_To_Ship_()
/*
Chebyshev distance (max of deltas)
¿ same as MoO2  Module: special  Range() ? I think not.
*/
int16_t Range_To_Battle_Unit(int16_t BU_1, int16_t BU_2)
{
    int16_t delta_y = 0;
    int16_t delta_x = 0;
    delta_x = abs(battle_units[BU_1].cgx - battle_units[BU_2].cgx);
    delta_y = abs(battle_units[BU_1].cgy - battle_units[BU_2].cgy);
    return (delta_x > delta_y) ? delta_x : delta_y; // Return the maximum
}


// WZD o122p13
/*
MoO2  Module: CMBTAI  Target_Ship_Value_()
...  Expected_Weapon_Damage_(); Missile_Overkill_Check_(); Being_Captured_();
e.g., short-circuit in Target_Ship_Value_() is if(Ship_Is_In_Stasis_()) { return -2000 }
*/
int16_t Target_Unit_Value(int16_t attacker_idx, int16_t target_idx, int16_t has_ranged_attack)
{
    int16_t damage_array[NUM_DAMAGE_TYPES] = { 0, 0, 0 };
    int16_t to_hit = 0;
    uint16_t attack_immunities = 0;
    uint16_t attack_attributes = 0;
    uint16_t unused_local = 0;
    int16_t attack_realm = 0;
    int16_t attack_type = 0;
    int16_t dist_penalty = 0;
    int16_t target_atk_str = 0;
    int16_t ship_value = 0;
    int16_t defense_strength = 0;
    int16_t attack_strength = 0;
    int16_t attacker_defense = 0;
    int16_t distance = 0;
    struct s_BATTLE_UNIT * attacker = NULL;
    struct s_BATTLE_UNIT * target = NULL;
    target = &battle_units[target_idx];
    attacker = &battle_units[attacker_idx];
    ship_value = -100;
    /* Check if target is unreachable due to flight */
    if((
        (target->Move_Flags & MV_FLYING) != 0)
        &&
        ((attacker->Move_Flags & MV_FLYING) == 0)
        &&
        (attacker->ranged_type == rat_NONE)
    )
    {
        return -200;
    }
    /* Calculate target's highest attack strength for later comparison */
    if(target->ranged > target->melee)
    {
        target_atk_str = target->ranged;
    }
    else
    {
        target_atk_str = target->melee;
    }
    attack_attributes = attacker->attack_attributes;
    to_hit = attacker->tohit;
    if(has_ranged_attack != ST_FALSE)
    {
        /* Ranged Attack Logic */
        attack_immunities = Battle_Unit_Attack_Immunities(attacker_idx, has_ranged_attack);
        attack_attributes |= attacker->ranged_attack_attributes;
        attack_strength = attacker->ranged;
        attack_realm = Battle_Unit_Attack_Magic_Realm(attacker->ranged_type, attacker_idx);
        attack_type = attacker->ranged_type;
        to_hit += attacker->ranged_tohit;
        /* Calculate Distance Penalty for non-magic ranged attacks */
        if((attack_type / 10) != rag_Magic)
        {
            dist_penalty = Range_To_Battle_Unit(attacker_idx, target_idx) / 3;
            if(attacker->Abilities & UA_LONGRANGE)
            {
                if(dist_penalty > 0)
                {
                    dist_penalty = ST_TRUE;
                }
            }
            to_hit -= dist_penalty;
        }
    }
    else
    {
        /* Melee Attack Logic */
        attack_immunities = Battle_Unit_Attack_Immunities(attacker_idx, has_ranged_attack);
        attack_attributes |= attacker->melee_attack_attributes;
        /* OGBUG */  unused_local = attacker->melee_attack_attributes;
        to_hit += (attacker->melee_tohit - target->toblock);
        attack_strength = attacker->melee;
        attack_realm = Battle_Unit_Attack_Magic_Realm(rat_NONE, attacker_idx);
        attack_type = rat_NONE;
    }
    /* Resolve defense and base value */
    defense_strength = Battle_Unit_Defense_Special(target_idx, attack_type, attack_immunities, attack_attributes, attack_realm);
    ship_value = attack_strength - defense_strength;
    /* Threat assessment: targets with ranged attacks are higher priority */
    if(Battle_Unit_Has_Ranged_Attack(target_idx))
    {
        ship_value += (target->ranged / 3) + 2;
    }
    /* Status effects */
    if(target->combat_effects & bue_Confusion)
    {
        ship_value -= 10;
    }
    /* Node effects: attacking spirits in a Sorcery Node */
    if(battlefield->center_square_structure >= CS_SorceryNode && ship_value > 0)
    {
        /* Check if attacker is under index 40 and belongs to the defender player */
        if(attacker_idx < 40 && attacker->controller_idx == _combat_defender_player)
        {
            if(target->Abilities & UA_MELD)
            {
                ship_value += 3;
                goto CurrentTargetCheck;
            }
        }
    }
    /* Tactical safety: if our defense is high enough relative to target attack, de-prioritize */
    if(ship_value > 0 && attacker_idx < 40)
    {
        attacker_defense = attacker->defense;
        if((attacker_defense / 2) > target_atk_str)
        {
            ship_value -= 10;
        }
    }
CurrentTargetCheck:
    /* Stick to current target bonus */
    if(attacker->target_battle_unit_idx == target_idx)
    {
        ship_value += 10;
    }
    /* City Wall Logic (Melee Only) */
    if((_ai_battlefield_city_walls & 1) && (has_ranged_attack == ST_FALSE))
    {
        /* Only applicable to attackers who can't bypass walls */
        if(attacker->controller_idx == _combat_attacker_player && 
            !(attacker->Move_Flags & MV_FLYING) && 
            !(attacker->Abilities & UA_NONCORPOREAL) && 
            !(attacker->Move_Flags & MV_TELEPORT) && 
            !(attacker->Move_Flags & MV_MERGING))
        {
            if(Combat_Grid_Cell_Has_City_Wall(target->cgx, target->cgy) > 0)
            {
                /* Check for gate location (8, 12) */
                if(target->cgx == CGX_GATE && target->cgy == CGY_GATE)
                {
                    ship_value += 5;
                }
                else
                {
                    ship_value = -20;
                }
            }
            /* Hardcoded wall segment checks for specific coordinates */
            if(
                (target->cgx == MIN_CGX_CITY_INNER || target->cgx == MAX_CGX_CITY_INNER)
                &&
                (target->cgy == MIN_CGY_CITY_INNER || target->cgy == MAX_CGY_CITY_INNER)
            )
            {
                ship_value = -20;
            }
        }
    }
    /* Wall of Fire Logic (Melee Only) */
    if(
        ((_ai_battlefield_city_walls & 2) != 0)
        &&
        (has_ranged_attack == 0)
    )
    {
        if(
            attacker->controller_idx == _combat_attacker_player
            && 
            !(attacker->Move_Flags & MV_FLYING)
            &&
            !(attacker->Move_Flags & MV_TELEPORT)
            &&
            !(attacker->Move_Flags & MV_MERGING)
        )
        {
            /* If target is inside city and attacker is outside */
            if(Battle_Unit_Is_Within_City(target_idx) == 1 && Battle_Unit_Is_Within_City(attacker_idx) == 0)
            {
                /* Predict damage from Fireball (used as proxy for Wall of Fire damage) */
                Compute_Battle_Unit_Damage_From_Spell(spl_Fireball, attacker_idx, damage_array, 0);
                if(damage_array[0] > 1)
                {
                    ship_value = -20;
                }
            }
        }
    }
    /* Final range/movement adjustments */
    if(has_ranged_attack == ST_TRUE)
    {
        if(target->combat_effects & bue_Black_Sleep)
        {
            ship_value += 20;
        }
    }
    if(has_ranged_attack == ST_FALSE)
    {
        /* Melee reach check */
        distance = Range_To_Battle_Unit(attacker_idx, target_idx);
        if((distance * 2) <= attacker->movement_points)
        {
            if(target->combat_effects & bue_Black_Sleep)
            {
                ship_value += 20;
            }
            ship_value += 25;
        }
    }
    return ship_value;
}


// WZD o122p14
int16_t Battle_Unit_Has_Ranged_Attack(int16_t battle_unit_idx)
{
    int16_t is_ranged = 0;
    is_ranged = ST_FALSE;
    if(
        (battle_units[battle_unit_idx].ranged_type > rat_NONE)
        &&
        (battle_units[battle_unit_idx].ranged_type < srat_Thrown)
    )
    {
        is_ranged = ST_TRUE;
    }
    return is_ranged;
}


// WZD o122p15
int16_t Apply_Fear_Attack(int16_t attacker_battle_unit_idx, int16_t defender_battle_unit_idx)
{
    int16_t feared_figure_count = 0;
    int16_t itr_figures = 0;
    feared_figure_count = 0;
    /* Attacker has Fear */
    if(
        (battle_units[attacker_battle_unit_idx].Attribs_2 & USA_CAUSEFEAR)
        ||
        (battle_units[attacker_battle_unit_idx].enchantments & UE_CLOAK_OF_FEAR)
        ||
        (battle_units[attacker_battle_unit_idx].item_enchantments & UE_CLOAK_OF_FEAR)
        ||
        (_UNITS[battle_units[attacker_battle_unit_idx].unit_idx].enchantments & UE_CLOAK_OF_FEAR)
    )
    {
        /* Defender is not immune to Death/Fear */
        if((battle_units[defender_battle_unit_idx].Attribs_1 & USA_IMMUNITY_DEATH) == 0)
        {
            for(itr_figures = 0; itr_figures < battle_units[defender_battle_unit_idx].figure_cnt; itr_figures++)
            {
                if(Combat_Resistance_Check(battle_units[defender_battle_unit_idx], 0, sbr_Death) > 0)
                {
                    feared_figure_count++;
                }
            }
        }
    }
    return feared_figure_count;
}



/*

    WIZARDS.EXE ovr123

*/

// WZD o123p01
// MoO2  Module: COMBINIT  End_Of_Combat_()
/*

Diplomatic_Value:
    starts at 0
    adds 20 for Hero Unit
    or Random(20) for Non-Hero Unit and Random(10) for Engineer or Settler
    passes negated value to G_DIPL_Action()
*/
void End_Of_Combat(int16_t winner_player_idx, int16_t * item_count, int16_t item_list[], int16_t end_of_combat_message_type)
{
    // GCC  warning: storing the address of local variable 'Buildings_Lost' in 'CMB_LostBuildings' [-Wdangling-pointer=]
    // CMB_LostBuildings is read within the same function (the combat resolution / post-battle summary screen). So the pointer is still valid when it's actually used — it's not truly dangling at runtime, but GCC can't prove that.
    static int16_t Buildings_Lost[NUM_BUILDINGS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  /* HACK  static moves this to the data segment */
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
    int16_t itr_battle_units = 0;
    int16_t RazeCity = 0;
    int16_t itr_buildings = 0;
    int16_t itr_population = 0;
    int16_t itr_bldg_msg = 0;
    int16_t IDK_population_lost = 0;
    int16_t bldg_msg_idx = 0;
    int16_t itr_hero_items = 0;
    Rare_Foe_Defeated = 0;
    combat_results_scroll_message = end_of_combat_message_type;
    CMB_Population_Lost = 0;
    CMB_Buildings_Lost = 0;
    CMB_Gold_Reward = 0;
    // ~ Monsters
    if((winner_player_idx < HUMAN_PLAYER_IDX) || (winner_player_idx > NEUTRAL_PLAYER_IDX))
    {
        for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
        {
            if(battle_units[itr_battle_units].controller_idx == winner_player_idx)
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
    if(_combat_environ == 5)  /* Lair */
    {
        _LAIRS[_combat_environ_idx].guard1_count = (_LAIRS[_combat_environ_idx].guard1_count & 0xF0);  // clear low-nibble
        if((_LAIRS[_combat_environ_idx].guard2_count & 0x0F) == 0)
        {
            No_Secondaries = ST_TRUE;
        }
        _LAIRS[_combat_environ_idx].guard2_count = (_LAIRS[_combat_environ_idx].guard2_count & 0xF0);  // clear low-nibble
    }
    /*
        BEGIN:  
            drake178: process control change effects, regeneration, and recalls, while also counting the surviving units
    */
    Surviving_Unit_Count = 0;
    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
    {
        if((battle_units[itr_battle_units].combat_effects & bue_Confusion) != 0)
        {
            if(_UNITS[battle_units[itr_battle_units].unit_idx].owner_idx != winner_player_idx)
            {
                battle_units[itr_battle_units].status = bus_Dead;
            }
            else
            {
                battle_units[itr_battle_units].controller_idx = _UNITS[battle_units[itr_battle_units].unit_idx].owner_idx;
            }
        }
        if(battle_units[itr_battle_units].controller_idx != winner_player_idx)
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
        enchantments = (battle_units[itr_battle_units].enchantments | _UNITS[battle_units[itr_battle_units].unit_idx].enchantments | battle_units[itr_battle_units].item_enchantments);
        if(
            ((battle_units[itr_battle_units].combat_effects & bue_Possession) != 0)
            ||
            ((battle_units[itr_battle_units].combat_effects & bue_Creature_Binding) != 0)
        )
        {
            if(battle_units[itr_battle_units].controller_idx != winner_player_idx)
            {
                if(battle_units[itr_battle_units].controller_idx != _combat_attacker_player)
                {
                    battle_units[itr_battle_units].controller_idx = (int8_t)_combat_attacker_player;
                }
                else
                {
                    battle_units[itr_battle_units].controller_idx = (int8_t)_combat_defender_player;
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
                if(battle_units[itr_battle_units].controller_idx == winner_player_idx)
                {
                    battle_units[itr_battle_units].status = bus_Active;
                }
                battle_units[itr_battle_units].figure_cnt = battle_units[itr_battle_units].figure_max;
                battle_units[itr_battle_units].front_figure_damage = 0;
            }
        }
        if(battle_units[itr_battle_units].status == bus_Recalled)
        {
            battle_unit_owner_idx = battle_units[itr_battle_units].controller_idx;
            _UNITS[battle_units[itr_battle_units].unit_idx].wx = (int8_t)_players[battle_unit_owner_idx].summon_wx;
            _UNITS[battle_units[itr_battle_units].unit_idx].wy = (int8_t)_players[battle_unit_owner_idx].summon_wy;
            _UNITS[battle_units[itr_battle_units].unit_idx].wp = (int8_t)_players[battle_unit_owner_idx].summon_wp;
            _UNITS[battle_units[itr_battle_units].unit_idx].Finished = ST_TRUE;
            Evict_Weakest_Unit(battle_units[itr_battle_units].unit_idx);
            battle_units[itr_battle_units].status = bus_Active;
            if(
                (battle_unit_owner_idx == winner_player_idx)
                &&
                (battle_unit_owner_idx == _combat_attacker_player)
            )
            {
                _UNITS[battle_units[itr_battle_units].unit_idx].owner_idx = 100;  /* 100 means 'recalled during combat, so don't do post-combat move */
            }
        }
        if(
            (battle_units[itr_battle_units].controller_idx == winner_player_idx)
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
    Retreat_From_Combat(winner_player_idx);
    GUI_Multipurpose_Int = 0;
    /*
        BEGIN:  Undead / Zombie
                INCONSISTENT: does not remove enchantments and mutations that are not available for the new unit
    */
    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
    {
        // Undead
        if(
            (battle_units[itr_battle_units].controller_idx != winner_player_idx)
            &&
            (battle_units[itr_battle_units].status == bus_Drained)
            &&
            (battle_units[itr_battle_units].race != rt_Death)
            &&
            (_UNITS[battle_units[itr_battle_units].unit_idx].Hero_Slot == ST_UNDEFINED)
            &&
            (Surviving_Unit_Count < MAX_STACK)
        )
        {
            battle_units[itr_battle_units].figure_cnt = battle_units[itr_battle_units].figure_max;
            battle_units[itr_battle_units].front_figure_damage = 0;
            _UNITS[battle_units[itr_battle_units].unit_idx].wx = (int8_t)_combat_wx;
            _UNITS[battle_units[itr_battle_units].unit_idx].wy = (int8_t)_combat_wy;
            _UNITS[battle_units[itr_battle_units].unit_idx].wp = (int8_t)_combat_wp;
            _UNITS[battle_units[itr_battle_units].unit_idx].owner_idx = (int8_t)winner_player_idx;
            _UNITS[battle_units[itr_battle_units].unit_idx].mutations |= UM_UNDEAD;
            battle_units[itr_battle_units].controller_idx = (int8_t)winner_player_idx;
            battle_units[itr_battle_units].status = bus_Active;
            Undead_Created++;
            Surviving_Unit_Count++;
            Experience_Gained += 2;
        }
        // Zombies
        if(
            (_players[winner_player_idx].Globals[ZOMBIE_MASTERY] > 0)
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
            if(battle_units[itr_battle_units].controller_idx != winner_player_idx)
            {
                Experience_Gained += 2;
            }
            Zombies_Raised++;
            _UNITS[battle_units[itr_battle_units].unit_idx].type = ut_Zombies;
            _UNITS[battle_units[itr_battle_units].unit_idx].owner_idx = (int8_t)winner_player_idx;
            _UNITS[battle_units[itr_battle_units].unit_idx].XP = 0;
            _UNITS[battle_units[itr_battle_units].unit_idx].Level = 0;
            _UNITS[battle_units[itr_battle_units].unit_idx].wx = (int8_t)_combat_wx;
            _UNITS[battle_units[itr_battle_units].unit_idx].wy = (int8_t)_combat_wy;
            _UNITS[battle_units[itr_battle_units].unit_idx].wp = (int8_t)_combat_wp;
            battle_units[itr_battle_units].figure_cnt = _unit_type_table[ut_Zombies].Figures;
            battle_units[itr_battle_units].figure_max = _unit_type_table[ut_Zombies].Figures;
            battle_units[itr_battle_units].front_figure_damage = 0;
            battle_units[itr_battle_units].status = bus_Active;
            Surviving_Unit_Count++;
        }
    }  /* for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++) */
    /*
        END:  Undead / Zombie
    */
    /*
        BEGIN:  Enemey City
    */
    if(_combat_environ == 1)  /* City-Siege */
    {
        CMB_Population_Lost = 0;
        CMB_Buildings_Lost = 0;
        if(winner_player_idx != _combat_attacker_player)
        {
            CMB_Gold_Reward = 0;
        }
        else
        {
            if(_combat_defender_player != NEUTRAL_PLAYER_IDX)
            {
                _players[_combat_defender_player].fame -= _CITIES[_combat_environ_idx].size;
                SETMIN(_players[_combat_defender_player].fame, 0);
            }
            if(_combat_defender_player == HUMAN_PLAYER_IDX)
            {
                GUI_Multipurpose_Int -= _CITIES[_combat_environ_idx].size;
            }
            if(_CITIES[_combat_environ_idx].population == 0)
            {
                destroy_combat_city = ST_TRUE;
            }
            else
            {
                if(winner_player_idx != HUMAN_PLAYER_IDX)
                {
                    if(winner_player_idx < _num_players)  /* ~== not NEUTRAL_PLAYER_IDX */
                    {
                        destroy_combat_city = Raze_Check(winner_player_idx, _combat_environ_idx);
                    }
                }
                else
                {
                    if(magic_set.raze_city == ST_TRUE)
                    {
                        RazeCity = Raze_City_Prompt(cnst_RazeCity_Msg);
                        if(RazeCity == 0)
                        {
                            destroy_combat_city = ST_TRUE;
                        }
                    }
                }

            }
            if(destroy_combat_city == ST_FALSE)
            {
                CMB_Gold_Reward = (_CITIES[_combat_environ_idx].size - 2);
                SETMIN(CMB_Gold_Reward, 0);
                _players[winner_player_idx].fame += CMB_Gold_Reward;
                if(winner_player_idx == HUMAN_PLAYER_IDX)
                {
                    GUI_Multipurpose_Int += CMB_Gold_Reward;
                }
                CMB_Gold_Reward = 0;
                CMB_Gold_Reward = City_Gold(_combat_environ_idx);  // the conquering wizard gets a portion of the previous owner's gold reserve as loot.
                if(winner_player_idx < _num_players)
                {
                    Player_Add_Gold(winner_player_idx, CMB_Gold_Reward);
                }
                if(_CITIES[_combat_environ_idx].owner_idx < _num_players)
                {
                    _players[_combat_defender_player].gold_reserve -= CMB_Gold_Reward;
                }
            }
            else
            {
                CMB_Gold_Reward = _CITIES[_combat_environ_idx].size;
                if(winner_player_idx == HUMAN_PLAYER_IDX)
                {
                    GUI_Multipurpose_Int -= CMB_Gold_Reward;
                }
                _players[winner_player_idx].fame -= CMB_Gold_Reward;
                SETMIN(_players[winner_player_idx].fame, 0);
                CMB_Gold_Reward = City_Gold(_combat_environ_idx);
                if(_CITIES[_combat_environ_idx].owner_idx < _num_players)
                {
                    _players[_combat_defender_player].gold_reserve -= CMB_Gold_Reward;
                }
                for(itr_buildings = bt_Barracks; itr_buildings < NUM_BUILDINGS; itr_buildings++)
                {
                    if(_CITIES[_combat_environ_idx].bldg_status[itr_buildings] > bs_NotBuilt)
                    {
                        CMB_Gold_Reward += (bldg_data_table[itr_buildings].construction_cost / 10);
                    }
                }
                if(winner_player_idx < _num_players)
                {
                    Player_Add_Gold(winner_player_idx, CMB_Gold_Reward);
                }
            }
        }  /* if(winner_player_idx != _combat_attacker_player) */
        if(_CITIES[_combat_environ_idx].population != 0)
        {
            Population_Loss_Percent = (_combat_city_damage * 2);
            if(winner_player_idx == _combat_attacker_player)
            {
                if(winner_player_idx == NEUTRAL_PLAYER_IDX)
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
                            break;
                        }
                    }
                    if(Summoned_Unit != ST_TRUE)
                    {
                        _active_battle_unit = 667;  /* 667 - raiders won (city neutral) */
                    }
                    else
                    {
                        Population_Loss_Percent += 50;

                        _active_battle_unit = Rampage_Combat_City();  /* 666 - rampage, no ruins  668 - rampage, created ruins */
                    }
                }
                else
                {
                    Population_Loss_Percent += 10;
                }
            }
            SETMAX(Population_Loss_Percent, 50);
            IDK_population_lost = 0;
            for(itr_population = 0; (_CITIES[_combat_environ_idx].population - 1) > itr_population; itr_population++)
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
            Destruction_Chance = _combat_city_damage;
            if(winner_player_idx == _combat_attacker_player)
            {
                if(winner_player_idx != NEUTRAL_PLAYER_IDX)
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
                Apply_Damage_To_City(_combat_environ_idx, IDK_population_lost, Destruction_Chance, &Buildings_Lost[0]);
            }
            if(winner_player_idx != _combat_defender_player)
            {
                for(itr_bldg_msg = 0; g_bldg_msg_ctr> itr_bldg_msg; itr_bldg_msg++)
                {
                    if(MSG_Building_Complete[itr_bldg_msg].city_idx == _combat_environ_idx)
                    {
                        for(bldg_msg_idx = itr_bldg_msg; g_bldg_msg_ctr > bldg_msg_idx; bldg_msg_idx++)
                        {
                            MSG_Building_Complete[bldg_msg_idx].city_idx = MSG_Building_Complete[(bldg_msg_idx + 1)].city_idx;
                        }
                        g_bldg_msg_ctr--;
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
    }  /* if(_CITIES[_combat_environ_idx].population != 0) */
    /*
        END:  Enemey City
    */
    /*
        BEGIN:  
    */
    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
    {
        assert(battle_units[itr_battle_units].unit_idx >= 0);
        assert(battle_units[itr_battle_units].unit_idx <= _units);
        /*
            Lair Combat, Neutral Player won, Neutral Player's Unit, Unit is ~Alive
            move the unit back into the lair unit count and kill it's battle unit record
            not sure about the "No_Secondaries" part
        */
        {
            if(
                (_combat_environ == 5)  /* Lair */
                &&
                (winner_player_idx == NEUTRAL_PLAYER_IDX)
                &&
                (battle_units[itr_battle_units].controller_idx == NEUTRAL_PLAYER_IDX)
                &&
                (battle_units[itr_battle_units].status == bus_Active)
            )
            {
                if(_UNITS[battle_units[itr_battle_units].unit_idx].type == _LAIRS[_combat_environ_idx].guard1_unit_type)
                {
                    _LAIRS[_combat_environ_idx].guard1_count++;
                    battle_units[itr_battle_units].status = bus_Dead;
                }
                else if(_UNITS[battle_units[itr_battle_units].unit_idx].type == _LAIRS[_combat_environ_idx].guard2_unit_type)
                {
                    _LAIRS[_combat_environ_idx].guard2_count++;
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
                        _LAIRS[_combat_environ_idx].guard2_unit_type = _UNITS[battle_units[itr_battle_units].unit_idx].type;
                        _LAIRS[_combat_environ_idx].guard2_count += 0x11;  // add 1 to both the high and low nibbles
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
            (_UNITS[battle_units[itr_battle_units].unit_idx].wp == 9)  /* Combat Summon - ¿ ~unsummoned ? */
            ||
            (
                (_UNITS[battle_units[itr_battle_units].unit_idx].owner_idx == NEUTRAL_PLAYER_IDX)
                &&
                (_combat_environ != cnv_Enemy_Stack)
                &&
                (
                    (_combat_environ != cnv_Enemy_City)
                    ||
                    ((battle_units[itr_battle_units].Abilities & UA_FANTASTIC) != 0)
                )
            )
        )
        {
            // it's dead, and it's not ours, so we killed it, and we deserve the credit
            if(battle_units[itr_battle_units].controller_idx != winner_player_idx)
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
                if(_UNITS[battle_units[itr_battle_units].unit_idx].Hero_Slot > ST_UNDEFINED)
                {
                    Diplomatic_Value += 20;
                }
                else
                {
                    Diplomatic_Value += Random(20);
                }
                if(
                    (battle_units[itr_battle_units].Construction > 0)
                    ||
                    ((battle_units[itr_battle_units].Abilities & UA_CREATEOUTPOST) != 0)
                )
                {
                    Diplomatic_Value += Random(10);
                }
            }
            /*
                BEGIN: Hero Unit

                    IDA Color: brick red
            */
            if(_UNITS[battle_units[itr_battle_units].unit_idx].Hero_Slot > ST_UNDEFINED)
            {
                for(itr_hero_items = 0; itr_hero_items < NUM_HERO_ITEMS; itr_hero_items++)
                {
                    if(_players[_UNITS[battle_units[itr_battle_units].unit_idx].owner_idx].Heroes[_UNITS[battle_units[itr_battle_units].unit_idx].Hero_Slot].Items[itr_hero_items] > ST_UNDEFINED)
                    {
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
                    _players[_UNITS[battle_units[itr_battle_units].unit_idx].owner_idx].Heroes[_UNITS[battle_units[itr_battle_units].unit_idx].Hero_Slot].Items[itr_hero_items] = ST_UNDEFINED;
                }
                if(battle_units[itr_battle_units].controller_idx != winner_player_idx)
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
                Kill_Unit(battle_units[itr_battle_units].unit_idx, kt_Disappeared);
            }
            else
            {
                Kill_Unit(battle_units[itr_battle_units].unit_idx, kt_Normal);
            }
        }
        else
        {
            /*
                Current Battle Unit is Alive
            */
            SETMIN(battle_units[itr_battle_units].front_figure_damage, 0);
            battle_units[itr_battle_units].Extra_Hits = 0;
            battle_units[itr_battle_units].enchantments = 0;
            BU_CombatHits = Battle_Unit_Hit_Points(&battle_units[itr_battle_units]);
            BU_CombatHits = (battle_units[itr_battle_units].hits - BU_CombatHits);
            battle_units[itr_battle_units].front_figure_damage -= BU_CombatHits;
            SETMIN(battle_units[itr_battle_units].front_figure_damage, 0);
            _UNITS[battle_units[itr_battle_units].unit_idx].Damage = (((battle_units[itr_battle_units].figure_max - battle_units[itr_battle_units].figure_cnt) * (battle_units[itr_battle_units].hits - BU_CombatHits)) + battle_units[itr_battle_units].front_figure_damage);
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
        (_combat_environ == 5)  // Lair
        &&   
        (winner_player_idx == NEUTRAL_PLAYER_IDX)
    )
    {
        if(
            ((_LAIRS[_combat_environ_idx].guard1_count & 0x0F) == 0)
            &&
            ((_LAIRS[_combat_environ_idx].guard2_count & 0x0F) > 0)
        )
        {
            _LAIRS[_combat_environ_idx].guard1_count = _LAIRS[_combat_environ_idx].guard2_count;
            _LAIRS[_combat_environ_idx].guard1_unit_type = _LAIRS[_combat_environ_idx].guard2_unit_type;
            _LAIRS[_combat_environ_idx].guard2_count = 0;
            _LAIRS[_combat_environ_idx].guard2_unit_type = ut_Gargoyles;
        }
        if(
            (Undead_Created > 0)
            &&
            (_LAIRS[_combat_environ_idx].guard1_unit_type >= ut_Magic_Spirit)
            &&
            (_LAIRS[_combat_environ_idx].guard2_unit_type >= ut_Magic_Spirit)
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
        (winner_player_idx != NEUTRAL_PLAYER_IDX)
        &&
        (Experience_Gained >= 8)
    )
    {
        if(winner_player_idx == HUMAN_PLAYER_IDX)
        {
            GUI_Multipurpose_Int++;
        }
        _players[winner_player_idx].fame++;
        if(winner_player_idx != _combat_attacker_player)
        {
            if(
                (_players[_combat_attacker_player].fame > 20)
                &&
                (_combat_attacker_player == HUMAN_PLAYER_IDX)
            )
            {
                GUI_Multipurpose_Int--;
                _players[_combat_attacker_player].fame--;
            }
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
                _players[_combat_defender_player].fame--;
            }
        }
    }
    /*
        END:  Fame for defeating 4+ enemy units
    */
    if(winner_player_idx == HUMAN_PLAYER_IDX)
    {
        GUI_Multipurpose_Int += Rare_Foe_Defeated;
    }
    _players[winner_player_idx].fame += Rare_Foe_Defeated;
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
            (battle_units[itr_battle_units].controller_idx == winner_player_idx)
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
            stu_itoa(Zombies_Raised, temp_buffer, 10);
            stu_strcpy(GUI_NearMsgString, temp_buffer);
            if(Zombies_Raised > 1)
            {
                stu_strcat(GUI_NearMsgString, cnst_Zombie_Msg_1);
            }
            else
            {
                stu_strcat(GUI_NearMsgString, cnst_Zombie_Msg_2);
            }
            stu_strcat(GUI_NearMsgString, cnst_Zombie_Msg_3);
            Undead_Animation(ut_Zombies);
        }
        if(Undead_Created > 0)
        {
            stu_itoa(Undead_Created, temp_buffer, 10);
            stu_strcpy(GUI_NearMsgString, temp_buffer);
            if(Undead_Created > 1)
            {
                stu_strcat(GUI_NearMsgString, cnst_Undead_Msg_1);
            }
            else
            {
                stu_strcat(GUI_NearMsgString, cnst_Undead_Msg_2);
            }
            if(winner_player_idx == HUMAN_PLAYER_IDX)
            {
                stu_strcat(GUI_NearMsgString, cnst_Undead_Msg_3);
            }
            else
            {
                if(winner_player_idx == NEUTRAL_PLAYER_IDX)
                {
                    if(Undead_Created > 1)
                    {
                        stu_strcat(GUI_NearMsgString, cnst_Undead_Msg_4);
                    }
                    else
                    {
                        stu_strcat(GUI_NearMsgString, cnst_Undead_Msg_5);
                    }
                }
                else
                {
                    stu_strcat(GUI_NearMsgString, _players[winner_player_idx].name);
                    stu_strcat(GUI_NearMsgString, cnst_Dot9);
                }
            }
            Undead_Animation(Find_Undead_Creator_Type(winner_player_idx));
        }
    }
    /*
        END:  Zombies & Undead
    */
    if(_combat_attacker_player != HUMAN_PLAYER_IDX)
    {
        Set_Mouse_List(1, mouse_list_hourglass);
    }
}


// WZD o123p02
int16_t Find_Undead_Creator_Type(int16_t player_idx)
{
    int16_t item_idx = 0;
    int16_t unit_value = 0;
    int16_t highest_value = 0;
    int16_t unit_type = 0;
    int16_t itr1 = 0;  /* unit_idx in battle */
    int16_t itr2 = 0;  /* item_slot_idx */
    highest_value = -1;
    unit_type = 10;
    for(itr1 = 0; itr1 < _combat_total_unit_count; itr1++)
    {
        /* Check if current unit is controlled by the player in question */
        /* Check for natural abilities: Life Steal or Create Undead */
        if(
            (battle_units[itr1].controller_idx != player_idx)
            ||
            (
                ((battle_units[itr1].attack_attributes & Att_LifeSteal) == 0)
                &&
                ((battle_units[itr1].Abilities & UA_CREATEUNDEAD) == 0)
            )
        )
        {
            if(highest_value < 10 && player_idx < MOO_MONSTER_PLAYER_IDX)
            {
                /* Check for Heroes with Vampiric items */
                /* Only proceed if we haven't found a "high value" (10+) creator yet and player is not Invalid/Monster */
                /* Check if this unit is a hero (Hero_Slot != -1) */
                if(_UNITS[battle_units[itr1].unit_idx].Hero_Slot > -1)
                {
                    /* Iterate through the 3 item slots of the hero */
                    for(itr2 = 0; itr2 < 3; itr2++)
                    {
                        /* Access players[player_idx].Heroes[hero_slot].Items[itr2] */
                        item_idx = _players[player_idx].Heroes[_UNITS[battle_units[itr1].unit_idx].Hero_Slot].Items[itr2];
                        if(item_idx > ST_UNDEFINED)
                        {
                            /* Check if the item has the Vampiric enchantment */
                            if((_ITEMS[item_idx].Powers & IP_VAMPIRIC) != 0)
                            {
                                highest_value = 10;
                                unit_type = 10; /* Return generic 'hero' or 'undead' result code */
                            }
                        }
                    }
                }
            }
        }
        else  /* (battle_units[itr1].controller_idx == player_idx) && (((battle_units[itr1].attack_attributes & Att_LifeSteal) != 0) || ((battle_units[itr1].Abilities & UA_CREATEUNDEAD) != 0)) */
        {
            /* Calculate a value based on upkeep to prioritize more powerful creators */
            unit_value = (100 + battle_units[itr1].upkeep);
            if(unit_value > highest_value)
            {
                /* Only consider special unit types (likely monsters/summons) above 'ut_Chosen' */
                if(_UNITS[battle_units[itr1].unit_idx].type > ut_Chosen)
                {
                    highest_value = unit_value;
                    unit_type = _UNITS[battle_units[itr1].unit_idx].type;
                }
            }
        }
    }
    return unit_type;
}


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
    int16_t City_Capture = 0;
    int16_t Hotkey_R2_Index = 0;
    int16_t Hotkey_R1_Index = 0;
    int16_t hotkey_esc = 0;
    int16_t leave_screen = 0;
    int16_t spare = 0;
    int16_t IDK_popup_timer = 0;
    int16_t input_field_idx = 0;
    City_Capture = ST_FALSE;
    if(combat_results_scroll_message == csmt_TurnLimit)
    {
        // SCROLL.LBX  011  CMBLOSE
        _scroll_paper_seg = LBX_Reload_Next(scroll_lbx_file__ovr123, 11, _screen_seg);
    }
    else
    {
        if(_combat_environ == 1)  /* City-Siege */
        {
            if(
                (combat_results_scroll_message == csmt_Victory)
                ||
                (combat_results_scroll_message == csmt_EnemyFled)
                ||
                (combat_results_scroll_message == csmt_VictoryStrategic)
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
                (combat_results_scroll_message == csmt_Victory)
                ||
                (combat_results_scroll_message == csmt_EnemyFled)
                ||
                (combat_results_scroll_message == csmt_VictoryStrategic)
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
    _page_flip_effect = 3;
    PageFlip_FX();
    _page_flip_effect = pfe_None;
    Copy_On_To_Off_Page();
    Assign_Auto_Function(Combat_Results_Scroll_Draw, 1);
    leave_screen = ST_FALSE;
    Hotkey_R1_Index = Add_Hot_Key(cnst_HOTKEY_R);
    Hotkey_R2_Index = Add_Hot_Key(cnst_HOTKEY_R_2);
    hotkey_esc = Add_Hidden_Field(0, 0, SCREEN_XMAX, SCREEN_YMAX, cnst_HOTKEY_Esc1A, ST_UNDEFINED);
    IDK_popup_timer = 0;
    while((IDK_popup_timer < 400) && (leave_screen == ST_FALSE))
    {
        input_field_idx = Get_Input();
        if(input_field_idx == hotkey_esc)
        {
            leave_screen = ST_TRUE;
        }
        if((input_field_idx == Hotkey_R1_Index) || (input_field_idx == Hotkey_R2_Index))
        {
            if(destroy_combat_city == ST_FALSE)
            {
                if(City_Capture == ST_TRUE)
                {
                    spare = Raze_City_Prompt(cnst_RazeCity_Msg);

                    if(spare == ST_FALSE)
                    {
                        destroy_combat_city = ST_TRUE;
                        // BUG: this is different than the original penalty, as conquest fame is already applied!
                        // BUG: no zero check
                        _players[HUMAN_PLAYER_IDX].fame -= (_CITIES[_combat_environ_idx].size + 1);
                        GUI_Multipurpose_Int -= (_CITIES[_combat_environ_idx].size + 1);
                        /* OGBUG: this resets the timeout counter */
                        for(IDK_popup_timer = 3; IDK_popup_timer < NUM_BUILDINGS; IDK_popup_timer++)
                        {
                            if(_CITIES[_combat_environ_idx].bldg_status[IDK_popup_timer] > bs_NotBuilt)
                            {
                                /* OGBUG: will re-award the original looted gold another time */
                                CMB_Gold_Reward += (bldg_data_table[IDK_popup_timer].construction_cost / 10);
                            }
                        }
                        Player_Add_Gold(HUMAN_PLAYER_IDX, CMB_Gold_Reward);
                        _scroll_text_height = Combat_Results_Scroll_Text();
                        _scroll_text_top = ((SCREEN_YMAX - (22 + _scroll_text_height)) / 2);
                    }
                    Clear_Fields();
                    hotkey_esc = Add_Hidden_Field(0, 0, SCREEN_XMAX, SCREEN_YMAX, cnst_HOTKEY_Esc1A, ST_UNDEFINED);
                }
            }
        }
        if(leave_screen == ST_FALSE)
        {
            Set_Page_Off();
            Combat_Results_Scroll_Draw();
            PageFlip_FX();
        }
        IDK_popup_timer++;
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
    int16_t text_height = 0;
    int16_t itr_colors = 0;
    int16_t itr_buildings = 0;
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
    switch(combat_results_scroll_message)
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
            // if(GUI_NearMsgString != 0)
            if(GUI_NearMsgString[0] != '\0')
            {
                Set_Font_Colors_15(1, &colors2[0]);
                Set_Font_Spacing_Width(2);
                Print_Paragraph(75, (_scroll_text_top + text_height), 175, GUI_NearMsgString, 0);
                text_height += (Get_Paragraph_Max_Height(175, GUI_NearMsgString) + 2);  /* OGBUG  passes 3rd argument */
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
            // _fstrcpy(GUI_NearMsgString, _CITIES[_combat_environ_idx].name);
            stu_strcpy(GUI_NearMsgString, _CITIES[_combat_environ_idx].name);
            stu_strcat(GUI_NearMsgString, cnst_CityLost_Msg);  /* " has been conquered" */
            Print_Centered(160, (_scroll_text_top + 25), GUI_NearMsgString);
        } break;
    }
    if(_active_battle_unit == 666)  /* ; 666 - rampage, no ruins */
    {
        Set_Font_Colors_15(1, &colors2[0]);
        LBX_Load_Data_Static(message_lbx_file__ovr123, 0, (SAMB_ptr)message, 10, 1, 150);  /* "The monsters rampage through the city and then disappear into the wilderness" */
        Print_Paragraph(75, (_scroll_text_top + text_height), 175, message, 2);
        text_height += (Get_Paragraph_Max_Height(175, message) + 2);  /* OGBUG  passes 3rd argument */
    }
    if(_active_battle_unit == 668)  /* ; 668 - rampage, created ruins */
    {
        Set_Font_Colors_15(1, &colors2[0]);
        stu_strcpy(message, _CITIES[_combat_environ_idx].name);
        stu_strcat(message, cnst_NewRuins_Msg);  /* " has been reduced to ruins" */
        Print_Paragraph(75, (_scroll_text_top + text_height), 175, message, 2);
        text_height += (Get_Paragraph_Max_Height(175, message) + 2);  /* OGBUG  passes 3rd argument */
    }
    if(_active_battle_unit == 667)  /* ; 667 - raiders won (city neutral) */
    {
        stu_strcpy(message, _CITIES[_combat_environ_idx].name);
        stu_strcat(message, cnst_CityRaided_Msg);  /* " has fallen to raiders" */
        Print_Paragraph(75, (_scroll_text_top + text_height), 175, message, 2);
        text_height += (Get_Paragraph_Max_Height(175, message) + 2);  /* OGBUG  passes 3rd argument */
    }
    if(GUI_Multipurpose_Int != 0)
    {
        Set_Font_Colors_15(1, &colors2[0]);
        stu_strcpy(message, cnst_ScrlFame_Msg_1);  /* "You have " */
        if(GUI_Multipurpose_Int < 0)
        {
            stu_strcat(message, cnst_ScrlFame_Msg_2);  /* "lost " */
        }
        else
        {
            stu_strcat(message, cnst_ScrlFame_Msg_3);  /* "gained " */
        }
        stu_itoa(abs(GUI_Multipurpose_Int), temp_string, 10);
        stu_strcat(message, temp_string);
        stu_strcat(message, cnst_ScrlFame_Msg_4);  /* " fame." */
        Print_Centered(160, (_scroll_text_top + text_height), message);
        text_height += 9;
    }
    if(CMB_Gold_Reward > 0)
    {
        Set_Font_Colors_15(1, &colors2[0]);
        Set_Font_Spacing_Width(2);
        stu_itoa(CMB_Gold_Reward, temp_string, 10);
        stu_strcpy(message, temp_string);
        stu_strcat(message, cnst_ScrlGold_Msg);  /* " gold pieces were looted." */
        Print_Centered(160, (_scroll_text_top + text_height), message);
        text_height += 9;
    }
    if(destroy_combat_city == ST_TRUE)
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
            stu_itoa(CMB_Population_Lost, temp_string, 10);
            stu_strcpy(message, temp_string);
            stu_strcat(message, cnst_ScrlPop_Msg);  /* " thousand inhabitants killed." */
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
int16_t City_Gold(int16_t city_idx)
{
    int16_t empire_population = 0;
    int16_t amount = 0;
    int16_t itr = 0;
    int16_t city_owner = 0;
    amount = 0;
    /* Check if the city belongs to the neutral player */
    if(_CITIES[city_idx].owner_idx == NEUTRAL_PLAYER_IDX)
    {
        itr = 0;
        /* amount is a sum of random(10) for each population point */
        while(itr < _CITIES[city_idx].population)
        {
            amount += Random(10);
            itr++;
        }
    }
    else
    {
        /* If owned by a wizard, amount is proportional to their gold reserve */
        empire_population = 0;
        city_owner = _CITIES[city_idx].owner_idx;
        /* Calculate total population of the owner's empire */
        for(itr = 0; itr < _cities; itr++)
        {
            if(_CITIES[itr].owner_idx == city_owner)
            {
                empire_population += _CITIES[itr].population;
            }
        }
        amount = (((int32_t)_players[city_owner].gold_reserve * _CITIES[city_idx].population) / empire_population);
    }
    return amount;
}


// WZD o123p07
int16_t Rampage_Combat_City(void)
{
    int16_t current_u_type = 0;
    int16_t unit_types_found = 0;
    int16_t secondary_unit = 0;
    int16_t primary_unit = 0;
    int16_t secondary_count = 0;
    int16_t primary_count = 0;
    int16_t empty_lair_idx = 0;
    int16_t itr = 0;
    empty_lair_idx = ST_UNDEFINED;
    for(itr = 0; (itr < NUM_LAIRS) && (empty_lair_idx == ST_UNDEFINED); itr++)
    {
        if(_LAIRS[itr].intact == ST_FALSE)
        {
            empty_lair_idx = itr;
        }
    }
    primary_unit = 0;
    secondary_unit = 0;
    primary_count = 0;
    secondary_count = 0;
    /* 50% chance for rampage to create ruins/lair */
    /* No room for a new lair record */
    /* Check if the city is a player's capital; capitals cannot be turned into ruins by rampage */
    if(
        (Random(2) == 1)
        ||
        (empty_lair_idx == ST_UNDEFINED)
        ||
        (Player_Fortress_City(_CITIES[_combat_environ_idx].owner_idx) == _combat_environ_idx)
    )
    {
        return 666;
    }
    /* Transfer ownership to Neutral and destroy the city */
    Change_City_Ownership(_combat_environ_idx, NEUTRAL_PLAYER_IDX);
    Destroy_City(_combat_environ_idx);
    unit_types_found = 0;
    /* Identify the two most expensive unit types in the rampaging monster stack to guard the ruins */
    for(itr = 0; itr < _combat_total_unit_count; itr++)
    {
        if(battle_units[itr].status != bus_Active)
        {
            continue;
        }
        if(battle_units[itr].controller_idx != NEUTRAL_PLAYER_IDX)
        {
            continue;
        }
        /* Mark rampaging units as dead (clearing the combat state) */
        battle_units[itr].status = bus_Dead;
        current_u_type = _UNITS[battle_units[itr].unit_idx].type;
        if(current_u_type == primary_unit)
        {
            primary_count++;
        }
        else if(current_u_type == secondary_unit)
        {
            secondary_count++;
        }
        else
        {
            if(unit_types_found == 0)
            {
                primary_unit = current_u_type;
                primary_count = 1;
                unit_types_found = 1;
            }
            else if(unit_types_found == 1)
            {
                unit_types_found = 2;
                /* If new unit is more expensive than primary, shift primary to secondary */
                if(_unit_type_table[current_u_type].cost < _unit_type_table[primary_unit].cost)
                {
                    secondary_unit = current_u_type;
                    secondary_count = 1;
                }
                else
                {
                    secondary_unit = primary_unit;
                    secondary_count = primary_count;
                    primary_unit = current_u_type;
                    primary_count = 1;
                }
            }
            else
            {
                /* Compare with existing primary and secondary to keep the top two most expensive */
                if(_unit_type_table[current_u_type].cost > _unit_type_table[primary_unit].cost)
                {
                    secondary_unit = primary_unit;
                    secondary_count = primary_count;
                    primary_unit = current_u_type;
                    primary_count = 1;
                }
                else if(_unit_type_table[current_u_type].cost > _unit_type_table[secondary_unit].cost)
                {
                    secondary_unit = current_u_type;
                    secondary_count = 1;
                }
            }
        }
    }
    /* Initialize the new Ruins lair */
    _LAIRS[empty_lair_idx].guard1_unit_type = (uint8_t)primary_unit;
    _LAIRS[empty_lair_idx].guard2_unit_type = (uint8_t)secondary_unit;
    _LAIRS[empty_lair_idx].guard1_count = (uint8_t)(primary_count * 17);  /* shift to upper nibble */
    _LAIRS[empty_lair_idx].guard2_count = (uint8_t)(secondary_count * 17);  /* shift to upper nibble */
    _LAIRS[empty_lair_idx].wx = (int8_t)_combat_wx;
    _LAIRS[empty_lair_idx].wy = (int8_t)_combat_wy;
    _LAIRS[empty_lair_idx].wp = (int8_t)_combat_wp;
    _LAIRS[empty_lair_idx].intact = ST_TRUE;
    _LAIRS[empty_lair_idx].type = lt_Ruins;
    _LAIRS[empty_lair_idx].Loot_Gold = CMB_Gold_Reward;
    _LAIRS[empty_lair_idx].Loot_Mana = 0;
    _LAIRS[empty_lair_idx].Spell_n_Special = 0;
    _LAIRS[empty_lair_idx].Misc_Flags = 0;
    _LAIRS[empty_lair_idx].Item_Count = 0;
    _LAIRS[empty_lair_idx].Item_Values[0] = 0;
    _LAIRS[empty_lair_idx].Item_Values[1] = 0;
    _LAIRS[empty_lair_idx].Item_Values[2] = 0;
    return 668;
}



/*
    WIZARDS.EXE  ovr124
*/

// WZD o124p01
int16_t Total_Ranged_Attack_Strength(int16_t player_idx)
{
    int16_t battle_unit_idx = 0;
    int16_t amount = 0;
    amount = 0;
    for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
    {
        if(battle_units[battle_unit_idx].status == bus_Active)
        {
            if(battle_units[battle_unit_idx].controller_idx == player_idx)
            {
                if(Battle_Unit_Has_Ranged_Attack(battle_unit_idx) != ST_FALSE)
                {
                    amount = battle_units[battle_unit_idx].ranged;  // OGBUG  should have been cummulative `+=`, not `=`  (deduced from usage in Auto_Do_Combat_Turn())
                }
            }
        }
    }
    return amount;
}


// WZD o124p02
/**
 * @brief Applies city-wall and Flying Fortress movement restrictions to the shared combat move-cost map for one battle unit.
 *
 * This routine updates _cmbt_movepath_cost_map so later pathfinding treats city-edge and city-area
 * cells as impassable when stone walls or Flying Fortress should block movement. It first marks the
 * four city-corner squares and the central structure square when applicable, then evaluates whether the
 * acting unit ignores wall restrictions through movement flags or special abilities.
 *
 * For units affected by city defenses, the function applies different restrictions depending on
 * whether the unit is already inside the city proper. Units inside the city are prevented from
 * pathing outward through wall segments, while units outside the city are prevented from entering
 * blocked city-area cells. In both cases the city gate square is left open unless Flying Fortress
 * is active. The final labeled block preserves the original Flying Fortress bug behavior documented
 * in the existing comments.
 *
 * @param battle_unit_idx Index of the acting battle unit in battle_units.
 *
 * @note This function mutates the global/shared combat movement buffer _cmbt_movepath_cost_map in
 *       place.
 * @note Units with flying, teleport, merging, or noncorporeal movement bypass the normal city-wall
 *       restriction logic and fall through to the original Flying Fortress handling block.
 * @note The gate exemption is based on CGX_GATE/CGY_GATE and is disabled when Flying Fortress is
 *       present.
 * @note The Check_FlyingFortress_Bug label intentionally preserves pre-existing original-game
 *       behavior, including the documented OGBUG path.
 */
void Update_Move_Map_City_Area_Restrictions(int16_t battle_unit_idx)
{
    struct s_BATTLE_UNIT * bu_ptr = NULL;
    int16_t i = 0;
    int16_t j = 0;
    bu_ptr = &battle_units[battle_unit_idx];
    /* Check specific wall corner logic */
    if(battlefield->walled == ST_TRUE)
    {
        /* [5,10] */
        if(battlefield->walls[0][0] == ST_TRUE)
        {
            _cmbt_movepath_cost_map[((MIN_CGY_CITY * COMBAT_GRID_WIDTH) + MIN_CGX_CITY)] = INF;
        }
        /* [8,10] */
        if(battlefield->walls[0][3] == ST_TRUE)
        {
            _cmbt_movepath_cost_map[((MIN_CGY_CITY * COMBAT_GRID_WIDTH) + MAX_CGX_CITY)] = INF;
        }
        /* [5,13] */
        if(battlefield->walls[3][0] == ST_TRUE)
        {
            _cmbt_movepath_cost_map[((MAX_CGY_CITY * COMBAT_GRID_WIDTH) + MIN_CGX_CITY)] = INF;
        }
        /* [8,13] */
        if(battlefield->walls[3][3] == ST_TRUE)
        {
            _cmbt_movepath_cost_map[((MAX_CGY_CITY * COMBAT_GRID_WIDTH) + MAX_CGX_CITY)] = INF;
        }
    }
    /* Central Structure block [6,11] */
    /* OGBUG  open fields don't have anything there either */
    if(battlefield->center_square_structure != CS_City)
    {
        _cmbt_movepath_cost_map[((CGY_LAIR * COMBAT_GRID_WIDTH) + CGX_LAIR)] = INF;
    }
    /* Check for units that ignore wall movement restrictions */
    if(bu_ptr->Move_Flags & MV_FLYING)
    {
        goto Check_FlyingFortress_Bug;
    }
    if(bu_ptr->Move_Flags & MV_TELEPORT)
    {
        goto Check_FlyingFortress_Bug;
    }
    if(bu_ptr->Move_Flags & MV_MERGING)
    {
        goto Check_FlyingFortress_Bug;
    }
    if(bu_ptr->Abilities & UA_NONCORPOREAL)
    {
        goto Check_FlyingFortress_Bug;
    }
    /* Basic wall/fortress check */
    if(battlefield->walled != ST_TRUE && battlefield->city_enchantments[FLYING_FORTRESS] == 0)
    {
        goto Check_FlyingFortress_Bug;
    }
    /* Check if unit is currently inside city boundaries */
    // defender or attacker?
    if(Battle_Unit_Is_Within_City(battle_unit_idx))
    {
        for(i = 0; i < COMBAT_GRID_CITY_AREA_WIDTH; i++)
        {
            for(j = 0; j < COMBAT_GRID_CITY_AREA_HEIGHT; j++)
            {
                // ¿ don't set city walls gate as impassible, unless there is a Flying Fortress ?
                if(
                    ((MIN_CGX_CITY + i) == CGX_GATE)
                    &&
                    ((MIN_CGY_CITY + j) == CGY_GATE)
                    &&
                    (battlefield->city_enchantments[FLYING_FORTRESS] == 0)
                )
                {
                    continue;
                }
                if(
                    battlefield->walls[j][i] == ST_TRUE
                    ||
                    battlefield->city_enchantments[FLYING_FORTRESS] > 0
                )
                {
                    if(i == 0)  /* cax / left / west */
                    {
                        _cmbt_movepath_cost_map[(((MIN_CGY_CITY + j) * COMBAT_GRID_WIDTH) + (MIN_CGX_CITY - 1))] = INF;
                        if(j == 0)  /* cay / top-left / north-west */
                        {
                            _cmbt_movepath_cost_map[(((MIN_CGY_CITY - 1) * COMBAT_GRID_WIDTH) + (MIN_CGX_CITY - 1))] = INF;
                        }
                        if(j == 3)  /* cay / bottom-left / south-west */
                        {
                            _cmbt_movepath_cost_map[(((MAX_CGY_CITY + 1) * COMBAT_GRID_WIDTH) + (MIN_CGX_CITY - 1))] = INF;
                        }
                    }
                    if(j == 0)  /* north */
                    {
                        _cmbt_movepath_cost_map[((MIN_CGY_CITY - 1) * COMBAT_GRID_WIDTH) + (MIN_CGX_CITY + i)] = INF;
                    }
                    if(i == 3)  /* east */
                    {
                        _cmbt_movepath_cost_map[(((MIN_CGY_CITY + j) * COMBAT_GRID_WIDTH) + (MAX_CGX_CITY + 1))] = INF;
                        if(j == 0)  /* cay / top-right / north-east */
                        {
                            _cmbt_movepath_cost_map[(((MIN_CGY_CITY - 1) * COMBAT_GRID_WIDTH) + (MAX_CGX_CITY + 1))] = INF;
                        }
                        if(j == 3)  /* cay / bottom-right / south-east */
                        {
                            _cmbt_movepath_cost_map[(((MAX_CGY_CITY + 1) * COMBAT_GRID_WIDTH) + (MAX_CGX_CITY + 1))] = INF;
                        }
                    }
                    if(j == 3)  /* south */
                    {
                        _cmbt_movepath_cost_map[(((MAX_CGY_CITY + 1) * COMBAT_GRID_WIDTH) + (MIN_CGX_CITY + i))] = INF;
                    }
                }
            }
        }
    }
    else
    {
        // sets city perimeter as impassible, except maybe the city walls gate, if there's not a flying fortress
        for(i = 0; i < COMBAT_GRID_CITY_AREA_WIDTH; i++)
        {
            for(j = 0; j < COMBAT_GRID_CITY_AREA_HEIGHT; j++)
            {
                // ¿ don't set city walls gate as impassible, unless there is a Flying Fortress ?
                if(
                    ((MIN_CGX_CITY + i) == CGX_GATE)
                    &&
                    ((MIN_CGY_CITY + j) == CGY_GATE)
                    &&
                    (battlefield->city_enchantments[FLYING_FORTRESS] == 0)
                )
                {
                    continue;
                }
                if(battlefield->walls[j][i] == ST_TRUE || battlefield->city_enchantments[FLYING_FORTRESS] > 0)
                {
                    _cmbt_movepath_cost_map[(MIN_CGY_CITY + j) * COMBAT_GRID_WIDTH + (MIN_CGX_CITY + i)] = INF;
                }
            }
        }
    }
Check_FlyingFortress_Bug:
    /* OGBUG  will always jump, ignoring Flying Fortress */
    if((bu_ptr->Abilities & 0) == 0)
    {
        return;
    }
    if(battlefield->city_enchantments[FLYING_FORTRESS] == 0)
    {
        return;
    }
    if((bu_ptr->Move_Flags & MV_FLYING) != 0)
    {
        return;
    }
    if((bu_ptr->Move_Flags & MV_TELEPORT) != 0)
    {
        return;
    }
    if(Battle_Unit_Is_Within_City(battle_unit_idx) != ST_FALSE)
    {
        return;
    }
    for(i = 0; i < COMBAT_GRID_CITY_AREA_WIDTH; i++)
    {
        for(j = 0; j < COMBAT_GRID_CITY_AREA_HEIGHT; j++)
        {
            _cmbt_movepath_cost_map[(MIN_CGY_CITY + j) * COMBAT_GRID_WIDTH + (MIN_CGX_CITY + i)] = INF;
        }
    }
}


// WZD o124p03
// drake178: AI_RestrictToCity()
/*
; marks the squares surrounding the city proper area in
; battle as impassable in GUI_ActiveMoveMap,
; preventing units from pathing to the outside
*/
/*

    sets city area perimeter as impassible

XREF:
    Do_Auto_Unit_Turn()
    Auto_Move_Unit()

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

                _cmbt_movepath_cost_map[(((10 + itr2) * COMBAT_GRID_WIDTH) + 4)] = INF;

                if(itr2 == 0)
                {

                    _cmbt_movepath_cost_map[((9 * COMBAT_GRID_WIDTH) + 4)] = INF;  // 0xC1  193  (9 * 21) = 189 + 4

                }

                if(itr2 == 3)
                {

                    _cmbt_movepath_cost_map[((14 * COMBAT_GRID_WIDTH) + 4)] = INF;  // 0x12A  298  (14 * 21) = 294 + 4

                }

            }

            if(itr2 == 0)
            {

                _cmbt_movepath_cost_map[(((9 * COMBAT_GRID_WIDTH) + 5) + itr1)] = INF;  // 0xC2  194  (9 * 21) = 189 + 5

            }

            if(itr1 == 3)
            {

                _cmbt_movepath_cost_map[(((10 + itr2) * COMBAT_GRID_WIDTH) + 9)] = INF;

                if(itr2 == 0)
                {

                    _cmbt_movepath_cost_map[((9 * COMBAT_GRID_WIDTH) + 9)] = INF;  // 0xC6  198  (9 * 21) = 189 + 9

                }

                if(itr2 == 3)
                {

                    _cmbt_movepath_cost_map[((14 * COMBAT_GRID_WIDTH) + 9)] = INF;  // 0x12F  303  (14 * 21) = 294 + 9

                }

            }

            if(itr2 == 3)
            {

                _cmbt_movepath_cost_map[(((14 * COMBAT_GRID_WIDTH) + 5) + itr1)] = INF;  // 0x12B  299  (14 * 21) = 294 + 5

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
    int16_t combat_structure = 0;
    int16_t terrain_type = 0;
    combat_structure = cs_NONE;
    if(Square_Is_Sailable(wx, wy, wp) != ST_FALSE)
    {
        combat_structure = cs_OceanTerrainType;
    }
    if(_combat_environ == 1)  /* City-Siege */
    {
        if(_CITIES[_combat_environ_idx].enchantments[HEAVENLY_LIGHT] > 0)
        {
            combat_enchantments[TRUE_LIGHT_DFNDR] = 2;
        }
        if(_CITIES[_combat_environ_idx].enchantments[CLOUD_OF_SHADOW] > 0)
        {
            combat_enchantments[DARKNESS_DFNDR] = 2;
        }
        if(set_city_flag == ST_TRUE)
        {
            combat_structure = cs_City;
        }
    }
    terrain_type = (p_world_map[wp][wy][wx] % NUM_TERRAIN_TYPES);
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
/*

¿ kinda like // WZD o95p05  int16_t Army_Boatriders(int16_t troop_count, int16_t troops[], int16_t boatriders[]) ?

Page 98  (PDF Page 103)
Ocean Combat
...
"Only flying, sailing and swimming units may actually engage in combat over such squares."
*/
int16_t Undeployable_Battle_Units_On_Water(int16_t player_idx)
{
    int16_t unit_count = 0;
    int16_t itr_battle_units = 0;
    unit_count = 0;
    if(Square_Is_Sailable(_combat_wx, _combat_wy, _combat_wp) == ST_TRUE)
    {
        for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
        {
            if(battle_units[itr_battle_units].controller_idx == player_idx)
            {
                if(
                    ((battle_units[itr_battle_units].Abilities & UA_WINDWALKING) != 0)
                    ||
                    ((_UNITS[battle_units[itr_battle_units].unit_idx].enchantments & UE_WIND_WALKING) != 0)
                    ||
                    (_UNITS[battle_units[itr_battle_units].unit_idx].type == spell_data_table[spl_Floating_Island].unit_type)
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
// ¿ MoO2  Fleet_Strength_Comparison_() ... Get_Player_Mode_()?!? ?
/*
accumulates effective_strength from Effective_Battle_Unit_Strength()
into own_effective_strength and enemy_effective_strength
accumulates (battle_units[].melee + battle_units[].ranged)
into own_attack_strength and enemy_attack_strength
NOTE: Level 3 crosses over from "enemy is stronger" to "own is stronger"
    returns {Level: 0,1,2,3,4,5}
...some sort of *mode* for making decisions? more/less offensive/defensive?
*/
int16_t AI_Player_Mode(int16_t player_idx)
{
    int16_t enemy_attack_strength = 0;      /* raw, per-figure, offence only */
    int16_t own_attack_strength = 0;        /* raw, per-figure, offence only */
    int16_t effective_strength = 0;
    int16_t player_mode_level = 0;
    int16_t enemy_effective_strength = 0;   /* real, whole-unit, combat-power estimate */
    int16_t itr = 0;
    int16_t own_effective_strength = 0;     /* real, whole-unit, combat-power estimate */
    own_effective_strength = 0;
    enemy_effective_strength = 0;
    own_attack_strength = 0;
    enemy_attack_strength = 0;
    for(itr = 0; itr < _combat_total_unit_count; itr++)
    {
        if(
            (battle_units[itr].status == bus_Active)
            &&
            (
                (_combat_turn < 2)
                ||
                (battle_units[itr].target_battle_unit_idx != ST_UNDEFINED)
                ||
                (battle_units[itr].controller_idx == HUMAN_PLAYER_IDX)
            )
        )
        {
            effective_strength = Effective_Battle_Unit_Strength(itr);
            if(battle_units[itr].controller_idx == player_idx)
            {
                own_effective_strength += effective_strength;
                own_attack_strength += (battle_units[itr].melee + battle_units[itr].ranged);
            }
            else
            {
                enemy_effective_strength += effective_strength;
                enemy_attack_strength += (battle_units[itr].melee + battle_units[itr].ranged);
            }
        }
    }
    if(
        (own_effective_strength == 0)
        ||
        (own_attack_strength == 0)
    )
    {
        return 0;
    }
    if(
        (enemy_effective_strength == 0)
        ||
        (enemy_attack_strength == 0)
    )
    {
        return 5;
    }
    if((enemy_effective_strength / own_effective_strength) > 3)
    {
        player_mode_level = 0;  /* floor(r) >= 4 */
    }
    else if((enemy_effective_strength / own_effective_strength) > 2)
    {
        player_mode_level = 1;  /* floor(r) == 3 */
    }
    else if((enemy_effective_strength * 2) >= (own_effective_strength * 3))
    {
        player_mode_level = 2;  /* r >= 1.5 */
    }
    else if((own_effective_strength / enemy_effective_strength) > 3)
    {
        player_mode_level = 5;  /* floor(1/r) >= 4 */
    }
    else if((own_effective_strength / enemy_effective_strength) > 2)
    {
        player_mode_level = 4;  /* floor(1/r) == 3 */
    }
    else
    {
        player_mode_level = 3;  /* 1/3 < r < 1.5 */
    }
    return player_mode_level;
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
SAMB_ptr Reload_Battle_Unit_Move_Sound(int16_t battle_unit_idx, /* HACK */ uint32_t * sound_seg_size)
{
    SAMB_ptr sound_seg = 0;  // _SI_
    int16_t sound_num = 0;  // _SI_

    sound_num = (_unit_type_table[_UNITS[battle_units[battle_unit_idx].unit_idx].type].Sound >> 4);  // high nibble

    if(sound_num == 0)
    {
        sound_seg = LBX_Reload_Next(soundfx_lbx_file__ovr124__1of2, SFX_Silence, World_Data);
        *sound_seg_size = lbxload_entry_length;
    }
    else if(sound_num == 13)
    {
        sound_seg = LBX_Reload_Next(newsound_lbx_file__ovr124__1of2, 0, World_Data);
        *sound_seg_size = lbxload_entry_length;
    }
    else
    {
        sound_seg = LBX_Reload_Next(soundfx_lbx_file__ovr124__1of2, sound_num, World_Data);
        *sound_seg_size = lbxload_entry_length;
    }

    return sound_seg;

}


// WZD o124p08
/**
 * @brief Reloads the melee attack sound data for a battle unit.
 *
 * Uses the low nibble of the unit type Sound field as the melee sound index.
 * Sound index 8 is treated as a silence fallback from SOUNDFX.LBX; all other
 * melee sound indices are loaded from CMBTSND.LBX.
 *
 * @param battle_unit_idx Index of the battle unit whose melee sound is requested.
 * @param sound_seg_size Output pointer that receives the loaded sound segment size in bytes.
 * @return SAMB_ptr Pointer to the loaded sound data segment.
 *
 * @note The @p sound_seg_size pointer is required and is written on every path.
 * @note Parameter @p sound_seg_size is marked as HACK in the original signature.
 */
SAMB_ptr Reload_Melee_Sound(int16_t battle_unit_idx, /* HACK */ uint32_t * sound_seg_size)
{
    SAMB_ptr sound_seg = NULL;
    int16_t sound_num = 0;  // DNE in Dasm
    sound_num = (_unit_type_table[_UNITS[battle_units[battle_unit_idx].unit_idx].type].Sound & 0x0F);  // low nibble
    if(sound_num == 8)
    {
        sound_seg = LBX_Reload_Next(soundfx_lbx_file__ovr124__1of2, SFX_Silence, World_Data);
        *sound_seg_size = lbxload_entry_length;
    }
    else
    {
        sound_seg = LBX_Reload_Next(cmbtsnd_lbx_file__ovr124, sound_num, World_Data);
        *sound_seg_size = lbxload_entry_length;
    }
    return sound_seg;
}

// WZD o124p09
void Combat_Load_Spell_Sound_Effect(int16_t spell_idx)
{
    if(magic_set.sound_effects == ST_TRUE)
    {
        Play_Sound(sound_silent_seg, sound_silent_seg_size);
        Mark_Block(World_Data);
        if(spell_data_table[spell_idx].Sound > ST_UNDEFINED)
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
        SND_SpellCast = (SAMB_ptr)ST_UNDEFINED;
    }
}


// WZD o124p10
void Check_Wall_Of_Fire_Attack(int16_t battle_unit_idx)
{
    int16_t damage_array[3] = { 0, 0, 0 };
    if(
        (battlefield->wall_of_fire > ST_FALSE)
        &&
        ((battle_units[battle_unit_idx].Move_Flags & MV_FLYING) == 0)
        &&
        ((battle_units[battle_unit_idx].Move_Flags & MV_TELEPORT) == 0)
        &&
        ((battle_units[battle_unit_idx].Move_Flags & MV_MERGING) == 0)
        &&
        (Battle_Unit_Is_Within_City(battle_unit_idx) == ST_FALSE)
        &&
        (battle_units[battle_unit_idx].target_cgx >= MIN_CGX_CITY)
        &&
        (battle_units[battle_unit_idx].target_cgx <= MAX_CGX_CITY)
        &&
        (battle_units[battle_unit_idx].target_cgy >= MIN_CGY_CITY)
        &&
        (battle_units[battle_unit_idx].target_cgy <= MAX_CGY_CITY)
    )
    {
        Compute_Battle_Unit_Damage_From_Spell(spl_Fireball, battle_unit_idx, &damage_array[0], 0);
        Battle_Unit_Commit_Damage(battle_unit_idx, &damage_array[0]);
    }
}


// WZD o124p11
/**
 * @brief Calculates and stores combat gore state for a damaged battle unit.
 *
 * Computes two post-hit presentation values on the target unit:
 * - `Atk_FigLoss`: estimated number of figures lost from the incoming damage.
 * - `gibs`: amount of blood/gib effect intensity to display.
 *
 * The function also refreshes `m_gibs_frames` with randomized frame offsets
 * used by combat blood visual effects.
 *
 * @param battle_unit_idx Index of the target unit in the global `battle_units` array.
 * @param Damage Total damage applied to the target unit for this hit event.
 *
 * @note `gibs` is clamped to a maximum of 5 and forced to at least 1 for
 *       positive nonzero damage when initial computation yields 0.
 * @note `figure_loss` is capped to the unit's current figure count.
 */
void Set_Gibs(int16_t battle_unit_idx, int16_t Damage)
{
    int16_t itr = 0;
    int16_t figure_loss = 0;
    int16_t gibs = 0;
    figure_loss = 0;
    if(battle_units[battle_unit_idx].figure_cnt == 0)
    {
        gibs = 3;
    }
    else
    {
        gibs = ((Damage + (battle_units[battle_unit_idx].figure_cnt / 2)) / (battle_units[battle_unit_idx].figure_cnt * 2));
    }
    SETMAX(gibs, 5);
    if(
        (Damage > 0)
        &&
        (gibs == 0)
    )
    {
        gibs = 1;
    }
    if(Damage > 0)
    {
        // This formula basically gives "damage per 2 HP per figure" — so a 1-figure unit taking 10 damage bleeds more than an 8-figure unit taking 10 damage.
        figure_loss = (((Damage + battle_units[battle_unit_idx].hits) - 1) / battle_units[battle_unit_idx].hits);
        if(battle_units[battle_unit_idx].figure_cnt < figure_loss)
        {
            figure_loss = battle_units[battle_unit_idx].figure_cnt;
        }
    }
    // ~ NIU_Gibs_Frames()
    for(itr = 0; itr < 20; itr++)
    {
        m_gibs_frames[itr] = (Random(4) - 1);
    }
    battle_units[battle_unit_idx].Atk_FigLoss = figure_loss;
    battle_units[battle_unit_idx].gibs = gibs; 
}


// WZD o124p12
/**
 * @brief Clears gore-related combat state for a battle unit.
 *
 * Resets transient post-hit presentation fields used by combat animation and
 * rendering so the unit no longer displays pending figure-loss or gib effects.
 *
 * @param battle_unit_idx Index of the target unit in the global `battle_units` array.
 */
void Clear_Gibs(int16_t battle_unit_idx)
{
    battle_units[battle_unit_idx].Atk_FigLoss =  0;
    battle_units[battle_unit_idx].gibs =  0;
}


// WZD o124p13
int16_t Battle_Unit_Is_Within_City(int16_t battle_unit_idx)
{
    if(
        (battle_units[battle_unit_idx].cgx >= MIN_CGX_CITY)
        &&
        (battle_units[battle_unit_idx].cgy >= MIN_CGY_CITY)
        &&
        (battle_units[battle_unit_idx].cgx <= MAX_CGX_CITY)
        &&
        (battle_units[battle_unit_idx].cgy <= MAX_CGY_CITY)
    )
    {
        return ST_TRUE;
    }
    else
    {
        return ST_FALSE;
    }
}


// WZD o124p14
int16_t Combat_Grid_Cell_Has_City_Wall(int16_t cgx, int16_t cgy)
{
    int16_t has_wall = 0;
    has_wall = ST_FALSE;
    if(
        (battlefield->walled == ST_TRUE)  /* is/was, any wall at all */
        &&
        (
            (cgx >= MIN_CGX_CITY)
            &&
            (cgx <= MAX_CGX_CITY)
        )
        &&
        (
            (cgy >= MIN_CGY_CITY)
            &&
            (cgy <= MAX_CGY_CITY)
        )
        &&
        (
            !(
                ((cgx == MIN_CGX_CITY_INNER) || (cgx == MAX_CGX_CITY_INNER))
                &&
                ((cgy == MIN_CGY_CITY_INNER) || (cgy == MAX_CGY_CITY_INNER))
            )
        )
    )
    {
        // map cgx,cgy to walls[][] array
        if(battlefield->walls[(cgy - MIN_CGY_CITY)][(cgx - MIN_CGX_CITY)] == ST_TRUE)
        {
            has_wall = ST_TRUE;
        }
    }
    return has_wall;
}


// WZD o124p15
/*
"While most creature summonings are cast overland, the summoning of Elementals, Phantom Warriors and Phantom Beasts can only be performed during combat."
*/
int16_t Battle_Unit_Is_Summoned_Creature(int16_t battle_unit_idx)
{
    int16_t unit_type = 0;
    unit_type = _UNITS[battle_units[battle_unit_idx].unit_idx].type;
    if(
        (spell_data_table[spl_Fire_Elemental].unit_type == unit_type)
        ||
        (spell_data_table[spl_Earth_Elemental].unit_type == unit_type)
        ||
        (spell_data_table[spl_Phantom_Beast].unit_type == unit_type)
        ||
        (spell_data_table[spl_Phantom_Warriors].unit_type == unit_type)
        ||
        (spell_data_table[spl_Air_Elemental].unit_type == unit_type)
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
/*
~== NEXTTURN.C  Unit_Moves2()
*/
int16_t Battle_Unit_Moves2(int16_t battle_unit_idx)
{
    uint32_t enchantments;
    int16_t * hero_items = NULL;
    int16_t wind_mastery = 0;
    int16_t item_moves2 = 0;
    int16_t itr = 0;
    int16_t endurance = 0;
    int16_t moves2 = 0;
    if(battle_units[battle_unit_idx].Web_HP > 0)
    {
        return 0;
    }
    enchantments = (battle_units[battle_unit_idx].enchantments | _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments | battle_units[battle_unit_idx].item_enchantments);
    item_moves2 = 0;
    /*
        BEGIN: Hero Items
    */
    if(_UNITS[battle_units[battle_unit_idx].unit_idx].Hero_Slot > ST_UNDEFINED)
    {
        // ; BUG: this may not be the hero's original owner
        // ¿ should index UNITS[] for owner_idx ?
        hero_items = &(_players[battle_units[battle_unit_idx].controller_idx].Heroes[_UNITS[battle_units[battle_unit_idx].unit_idx].Hero_Slot].Items[0]);
        for(itr = 0; itr < NUM_HERO_ITEMS; itr++)
        {
            if(hero_items[itr] > ST_UNDEFINED)
            {
                if(ITEM_POWER(hero_items[itr], ip_Endurance))
                {
                    endurance = ST_TRUE;
                }
                item_moves2 += _ITEMS[hero_items[itr]].moves2;
            }
        }
    }
    /*
        END: Hero Items
    */
    moves2 = _unit_type_table[_UNITS[battle_units[battle_unit_idx].unit_idx].type].moves2_base;
    moves2 += item_moves2;
    /*
        BEGIN: Wind Mastery
    */
    if(_unit_type_table[_UNITS[battle_units[battle_unit_idx].unit_idx].type].Transport > 0)
    {
        wind_mastery = 0;
        for(itr = 0; itr < _num_players; itr++)
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
    if((enchantments & UE_ENDURANCE) != 0 || endurance == ST_TRUE)
    {
        moves2 += 2;
    }
    if((battle_units[battle_unit_idx].combat_effects & bue_Haste) != 0)
    {
        moves2 *= 2;
    }
    return moves2;
}


// WZD o124p17
/**
 * @brief Resolves the strategic aftermath of capturing or destroying a city after combat.
 *
 * This routine handles the post-battle city outcome for the victorious stack identified by the
 * passed troop list. It determines the attacking stack owner and defending city owner, updates fame
 * and treasury values for the winner and loser, decides whether the city is kept or razed, and
 * prepares the combat-result summary shown to the human player when relevant.
 *
 * When the city is kept, the attacker gains fame based on city size, receives the city's gold
 * treasury as loot, and the defender loses that same treasury amount. When the city is razed, the
 * attacker instead suffers a fame penalty, loots the city's treasury plus a fraction of built
 * structure costs, and the defender's treasury is reduced accordingly. Human-controlled attackers
 * may be prompted for the raze decision, while AI attackers delegate that choice to Raze_Check().
 *
 * The function also contains a special path for neutral fantastic stacks that rampage through the
 * city: temporary battle-unit state is constructed, Rampage_Combat_City() is invoked, and the
 * rampaging units are destroyed afterward.
 *
 * @param troop_count The number of units in the victorious stack.
 * @param troops Pointer to the victorious stack's unit indices. The first entry is used to infer
 *        the stack owner.
 *
 * @note GUI_Multipurpose_Int is used as a summary accumulator for human-visible fame changes.
 * @note The function updates multiple global combat-result fields, including CMB_Gold_Reward,
 *       combat_results_scroll_message, CMB_Population_Lost, and CMB_Buildings_Lost.
 * @note A human-facing results scroll is only shown when either the attacker or the city owner is
 *       the human player.
 */
void Combat_City_Capture(int16_t troop_count, int16_t * troops)
{
    int16_t city_owner_idx = 0;
    int16_t troop_owner_idx = 0;
    int16_t itr = 0;
    int16_t city_size = 0;
    int16_t unit_type = 0;
    int16_t fame_penalty = 0;  // DNE in Dasm
    struct s_CITY * city_ptr = NULL;
    struct s_UNIT * unit_ptr = NULL;
    struct s_WIZARD * wizard_ptr = NULL;

    Unit_View_Allocate();  // DEDU  ¿ if/where used ?
    
    Set_Page_Off();
    Main_Screen_Draw();
    Copy_Off_To_Back();

    /* Identify attacker's owner from the first unit in the stack */
    unit_ptr = &_UNITS[troops[0]];
    troop_owner_idx = unit_ptr->owner_idx;

    /* Identify city owner */
    city_ptr = &_CITIES[_combat_environ_idx];
    city_owner_idx = city_ptr->owner_idx;

    /* GUI_Multipurpose_Int is used here as a change accumulator for the summary scroll */
    GUI_Multipurpose_Int = 0;

    /* Reduce fame of the loser */
    if(city_owner_idx < _num_players)
    {
        city_size = _CITIES[_combat_environ_idx].size;
        _players[city_owner_idx].fame -= city_size;

        if(city_owner_idx == HUMAN_PLAYER_IDX)
        {
            /* Track fame loss for human player summary */
            GUI_Multipurpose_Int -= city_size;
        }

        if(_players[city_owner_idx].fame < 0)
        {
            _players[city_owner_idx].fame = 0;
        }
    }

    /* Raze Decision Logic */
    if(_CITIES[_combat_environ_idx].population == 0)
    {
        destroy_combat_city = ST_TRUE;
    }
    else
    {
        destroy_combat_city = ST_FALSE;

        if(troop_owner_idx == HUMAN_PLAYER_IDX)
        {
            if(magic_set.raze_city == ST_TRUE)
            {
                /* displays the passed message as a raze city confirmation dialog */
                /* returns 0 if the city is razed, or 1 if not */
                itr = Raze_City_Prompt(cnst_RazeCity_Msg2);
                if(itr == 0)
                {
                    destroy_combat_city = ST_TRUE;
                }
            }
        }
        else if(troop_owner_idx < _num_players)
        {
            /* AI decides whether to raze or not. Returns 1 for yes, 0 for no. */
            destroy_combat_city = Raze_Check(_combat_environ_idx, troop_owner_idx);
        }
    }

    if(destroy_combat_city == ST_FALSE)
    {
        /* --- KEEP CITY --- */
        CMB_Gold_Reward = _CITIES[_combat_environ_idx].size - 2;
        if(CMB_Gold_Reward < 0)
        {
            CMB_Gold_Reward = 0;
        }

        /* Attacker gains fame for capturing */
        _players[troop_owner_idx].fame += CMB_Gold_Reward;

        if(troop_owner_idx == HUMAN_PLAYER_IDX)
        {
            GUI_Multipurpose_Int += CMB_Gold_Reward;
        }

        /* Calculate gold loot from conquest */
        CMB_Gold_Reward = City_Gold(_combat_environ_idx);

        if(troop_owner_idx < _num_players)
        {
            Player_Add_Gold(troop_owner_idx, CMB_Gold_Reward);
        }

        /* Loser loses the same amount of gold */
        if(city_owner_idx < _num_players)
        {
            _players[city_owner_idx].gold_reserve -= CMB_Gold_Reward;
            if(_players[city_owner_idx].gold_reserve < 0)
            {
                _players[city_owner_idx].gold_reserve = 0;
            }
        }
    }
    else
    {
        /* --- RAZE CITY --- */
        fame_penalty = _CITIES[_combat_environ_idx].size;

        if(troop_owner_idx == HUMAN_PLAYER_IDX)
        {
            /* Fame penalty for human razing */
            GUI_Multipurpose_Int -= fame_penalty;
        }

        /* Attacker loses fame for razing */
        if(troop_owner_idx < _num_players)
        {
            _players[troop_owner_idx].fame -= fame_penalty;
            if(_players[troop_owner_idx].fame < 0)
            {
                _players[troop_owner_idx].fame = 0;
            }
        }

        /* Loot treasury */
        CMB_Gold_Reward = City_Gold(_combat_environ_idx);

        if(city_owner_idx < _num_players)
        {
            _players[city_owner_idx].gold_reserve -= CMB_Gold_Reward;
            if(_players[city_owner_idx].gold_reserve < 0)
            {
                _players[city_owner_idx].gold_reserve = 0;
            }
        }

        /* Loot buildings (10% of cost) */
        for(itr = 3; itr < 36; itr++)
        {
            /* bs_NotBuilt is -1, Replaced 0, Built 1, Removed 2 */
            if(_CITIES[_combat_environ_idx].bldg_status[itr] > bs_NotBuilt)
            {
                CMB_Gold_Reward += bldg_data_table[itr].construction_cost / 10;
            }
        }

        if(troop_owner_idx < _num_players)
        {
            Player_Add_Gold(troop_owner_idx, CMB_Gold_Reward);
        }
    }

    battle_units = (struct s_BATTLE_UNIT *)Allocate_First_Block(_screen_seg, 63);  // 63 PR, 1008 B

    /* Check for rampaging neutral fantastic units (e.g., Great Drake, Hydra) */
    if(_UNITS[troops[0]].owner_idx == NEUTRAL_PLAYER_IDX)
    {
        unit_type = _UNITS[troops[0]].type;
        if((_unit_type_table[unit_type].Abilities & UA_FANTASTIC) != 0)
        {
            /* Populate battle unit data for the rampage */
            for(itr = 0; itr < troop_count; itr++)
            {
                battle_units[itr].controller_idx = NEUTRAL_PLAYER_IDX;
                battle_units[itr].status = bus_Active;
                battle_units[itr].unit_idx = troops[itr];
            }
            _combat_total_unit_count = troop_count;

            /* Rampage_Combat_City creates Ruins if applicable */
            _active_battle_unit = Rampage_Combat_City();

            /* Rampaging stack dies after the assault */
            for(itr = 0; itr < troop_count; itr++)
            {
                Kill_Unit(troops[itr], 0);
            }
        }
    }

    /* Prep UI Summary Scroll */
    CMB_Population_Lost = 0;

    if(troop_owner_idx == HUMAN_PLAYER_IDX)
    {
        combat_results_scroll_message = csmt_Victory;
    }

    if(city_owner_idx == HUMAN_PLAYER_IDX)
    {
        combat_results_scroll_message = csmt_CityLost;
    }

    CMB_Population_Lost = 0;
    CMB_Buildings_Lost = 0;

    if(troop_owner_idx == HUMAN_PLAYER_IDX || city_owner_idx == HUMAN_PLAYER_IDX)
    {
        /* Displays summary and finalizes raze/capture status */
        Combat_Results_Scroll();
    }
}


// WZD o124p18
/**
 * @brief Determines whether the AI should abandon combat and retreat.
 *
 * This routine evaluates the current combat state for one player and returns true only when the
 * AI considers the situation hopeless enough to flee. It first queries AI_Player_Mode() and only
 * continues when that safety assessment returns the lowest-risk tolerance state. It then counts the
 * player's units that are still relevant to the battle, including both active and uninvolved units,
 * and classifies them into heroes and builder-type units.
 *
 * The retreat decision is conservative in two specific cases: the AI will retreat to preserve any
 * surviving hero units, and it will also retreat when every remaining counted unit is a builder or
 * outpost-capable non-combatant. If neither of those conditions applies, the battle continues.
 *
 * @param player_idx The player whose surviving combat force should be evaluated for retreat.
 *
 * @return ST_TRUE if the AI should flee the battle.
 * @return ST_FALSE if the AI should continue fighting.
 *
 * @note Only units with status bus_Active or bus_Uninvolved are considered when evaluating whether
 *       anything valuable remains to save.
 * @note A unit is treated as a builder if it can create an outpost or has a positive Construction
 *       value.
 */
int16_t AI_Retreat_Check(int16_t player_idx)
{
    int16_t risked_unit_count = 0;
    int16_t builder_count = 0;
    int16_t player_mode_level = 0;
    int16_t hero_count = 0;
    int16_t itr = 0;
    struct s_BATTLE_UNIT * b_unit_ptr = NULL;
    struct s_UNIT * unit_ptr = NULL;
    hero_count = 0;
    builder_count = 0;
    risked_unit_count = 0;
    /* AI_Player_Mode returns 0 if the AI's situation is considered hopeless */
    player_mode_level = AI_Player_Mode(player_idx);
    if(player_mode_level != 0)
    {
        return ST_FALSE;
    }
    for(itr = 0; itr < _combat_total_unit_count; itr++)
    {
        b_unit_ptr = &battle_units[itr];
        /* Only consider units that are actively in the fight or uninvolved (not dead/fled) */
        if(
            (b_unit_ptr->status == bus_Active)
            ||
            (b_unit_ptr->status == bus_Uninvolved)
        )
        {
            if(b_unit_ptr->controller_idx == player_idx)
            {
                risked_unit_count++;
                unit_ptr = &_UNITS[b_unit_ptr->unit_idx];
                /* Check if the unit is a Hero */
                if(unit_ptr->Hero_Slot > ST_UNDEFINED)
                {
                    hero_count++;
                }
                /* Check if the unit is a "builder" (Settlers or Engineer) */
                if(
                    ((b_unit_ptr->Abilities & UA_CREATEOUTPOST) != 0)
                    ||
                    (b_unit_ptr->Construction > 0)
                )
                {
                    builder_count++;
                }
            }
        }
    }
    /* AI will flee if it has at least one hero to save */
    if(hero_count > 0)
    {
        return ST_TRUE;
    }
    /* AI will flee if all remaining units are just builders (non-combatants) */
    if(risked_unit_count == builder_count)
    {
        return ST_TRUE;
    }
    return ST_FALSE;
}


// WZD o124p19
int16_t Raze_Check(int16_t player_idx, int16_t city_idx)
{
    int16_t city_owner_units_on_landmass = 0;
    int16_t own_units_on_landmass = 0;
    int16_t city_landmass = 0;
    int16_t total_surviving = 0;
    int16_t itr = 0;
    int16_t score = 0;
    int16_t city_owner = 0;
    int16_t personality = 0;
    score = 0;
    if(_CITIES[city_idx].owner_idx == NEUTRAL_PLAYER_IDX)
    {
        return ST_FALSE;
    }
    city_owner = _CITIES[city_idx].owner_idx;  // DNE in Dasm
    /* Adjust score based on AI personality */
    personality = _players[player_idx].Personality;
    switch(personality)
    {
        case PRS_Maniacal:
        {
            score += 25;
        } break;
        case PRS_Ruthless:
        {
            score += 10;
        } break;
        case PRS_Aggressive:
        case PRS_Chaotic:
        {
            // DNE
        } break;
        case PRS_Lawful:
        {
            /* OGBUG: this should either check the diplomatic status or hostility instead, or use a different jump */
            /* BUG: Lawful check uses Hidden_Rel == 0 to penalize raze score */
            if(_players[player_idx].Dipl.Hidden_Rel[city_owner] != 0)
            {
                score += 25;
            }
            else
            {
                score -= 200;
            }
        } break;
        case PRS_Peaceful:
        {
            score -= 200;
        } break;
        default:
            break;
    }
    /* Count all active units surviving the battle */
    /* BUG? This counts units regardless of owner */
    {
        total_surviving = 0;
        for(itr = 0; itr < _combat_total_unit_count; itr++)
        {
            if(battle_units[itr].status == bus_Active)
            {
                total_surviving++;
            }
        }
        if(total_surviving < 3)
        {
            score += 10;
        }
    }
    /* Identify the landmass (continent) of the city */
    city_landmass = _landmasses[(_CITIES[city_idx].wp * WORLD_SIZE) + (_CITIES[city_idx].wy * WORLD_WIDTH) + _CITIES[city_idx].wx];
    /* Count total units on the same landmass for current player and former owner */
    own_units_on_landmass = 0;
    city_owner_units_on_landmass = 0;
    for(itr = 0; itr < _units; itr++)
    {
        if(_landmasses[(_UNITS[itr].wp * WORLD_SIZE) + (_UNITS[itr].wy * WORLD_WIDTH) + _UNITS[itr].wx] == city_landmass)
        {
            if(_UNITS[itr].owner_idx == player_idx)
            {
                own_units_on_landmass++;
            }
            else if(_UNITS[itr].owner_idx == city_owner)
            {
                city_owner_units_on_landmass++;
            }
        }
    }
    /* Strategic value adjustment */
    if(city_owner_units_on_landmass == 0)
    {
        score -= 200;
    }
    else
    {
        /* Ratio of own units vs former owner units on this continent */
        score += ((own_units_on_landmass * 50) / city_owner_units_on_landmass) - 50;
    }
    /* Chaotic wizards have a baseline chance to raze */
    if(_players[player_idx].Personality == PRS_Chaotic)
    {
        if(score < 10)
        {
            score = 10;
        }
    }
    /* Final probability check */
    if(Random(100) <= score)
    {
        return ST_TRUE; /* Raze city */
    }
    return ST_FALSE; /* Spare city */
}


// WZD o124p20
/**
 * @brief Display the combat raze-city confirmation dialog and return the player's choice.
 *
 * This function saves the current combat screen state, loads the confirmation dialog art,
 * installs @ref Raze_City_Prompt_Draw as the dialog redraw callback, and then processes
 * input until the player chooses whether to spare or destroy the captured city.
 *
 * The supplied message pointer is assigned to the global dialog text buffer used by the
 * redraw routine, so the string must remain valid for the duration of the prompt.
 *
 * @param message Dialog text shown inside the confirmation box.
 *
 * @return ST_TRUE when the player chooses not to raze the city.
 * @return ST_FALSE when the player chooses to raze the city.
 *
 * @note The return convention is inverted relative to the dialog theme: a false result means
 *       "raze", while a true result means "spare".
 * @note The prompt restores the saved screen, field list, alias colors, and window state
 *       before returning control to the caller.
 */
int16_t Raze_City_Prompt(char * message)
{
    int16_t spare = 0;
    int16_t window_fld = 0;
    int16_t hotkey_esc = 0;
    int16_t raze_button_fld = 0;
    int16_t no_button_fld = 0;
    int16_t input_field_idx = 0;
    int16_t message_box_height = 0;
    int16_t message_height = 0;
    int16_t leave_screen = 0;
    Save_Alias_Colors();
    Set_Font_Colors_15(0, &COL_Dialog_Text[0]);
    Save_ScreenSeg();  /* Save/Restore - otherwise it trashes the 'Combat Screen' data, include battle_units */
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
    message_height = Get_Paragraph_Max_Height(166, message);
    message_box_height = (34 + message_height);
    message_box_x = (68 + confirmation_box_x_offset);
    message_box_y = (confirmation_box_y_offset + ((SCREEN_HEIGHT - message_box_height) / 2));
    Clear_Fields();
    no_button_fld = Add_Button_Field((message_box_x + 101), (message_box_y + message_height + 15), "", confirmation_button_yes_seg, 'N', ST_UNDEFINED);
    raze_button_fld = Add_Button_Field((message_box_x + 18), (message_box_y + message_height + 15), "", confirmation_button_no_seg, 'R', ST_UNDEFINED);
    window_fld = Add_Hidden_Field(message_box_x, message_box_y, (message_box_x + 185), (message_box_y + 63), ST_NULL, ST_UNDEFINED);
    hotkey_esc = Add_Hidden_Field(0, 0, SCREEN_XMAX, SCREEN_YMAX, '\x1B', ST_UNDEFINED);
    Assign_Auto_Function(Raze_City_Prompt_Draw, 1);
    leave_screen = ST_FALSE;
    while(leave_screen == ST_FALSE)
    {
        input_field_idx = abs(Get_Input());
        if(
            (input_field_idx == ST_UNDEFINED)
            ||
            (input_field_idx == raze_button_fld)
        )
        {
            leave_screen = ST_TRUE;
            spare = ST_FALSE;
        }
        if(input_field_idx == no_button_fld)
        {
            leave_screen = ST_TRUE;
            spare = ST_TRUE;
        }
        Raze_City_Prompt_Draw();
        PageFlip_FX();
    }
    Deactivate_Auto_Function();
    Clear_Fields();
    Restore_Alias_Colors();
    Reset_Window();
    Restore_ScreenSeg();
    return spare;
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
int16_t AI_Select_Combat_Spell(int16_t caster_idx)
{
    int16_t spell_list[92] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t player_mode_level = 0;
    int16_t player_idx = 0;
    int16_t list_idx = 0;
    int16_t chosen_spell = 0;
    int16_t highest_value = 0;
    int16_t spell_score = 0;
    if(caster_idx > (CASTER_IDX_BASE - 1))
    {
        player_idx = (caster_idx - CASTER_IDX_BASE);
    }
    else
    {
        player_idx = battle_units[caster_idx].controller_idx;
    }
    highest_value = 0;
    chosen_spell = 0;
    if(caster_idx > (CASTER_IDX_BASE - 1))
    {
        spell_score = Combat_Casting_Cost_Multiplier(caster_idx - CASTER_IDX_BASE);
    }
    else
    {
        spell_score = 1;
    }
    AI_Build_Castable_Combat_Spell_List(caster_idx, &spell_list[0], spell_score);
    player_mode_level = AI_Player_Mode(player_idx);
    for(list_idx = 0; list_idx < GUI_Multipurpose_Int; list_idx++)
    {
        spell_score = AI_Score_Combat_Spell(player_idx, spell_list[list_idx], player_mode_level);
        if(spell_score > highest_value)
        {
            highest_value = spell_score;
            chosen_spell = spell_list[list_idx];
        }
    }
    return chosen_spell;
}


// WZD ovr139p02
/*
Returns -100 when there are no enemies, -1 when the spell has no valid target, else the score.
OGBUG:  the AITP_Combat_Spell() probe passes &spell_value for BOTH coordinate return pointers
OGBUG:  the attacker reads RP_AI_UnsetRealmVar1, the defender RP_AI_UnsetRealmVar2 - neither is ever written, so every "realm threat" bonus term is 0
OGBUG:  rp_disenchant_prio is computed from the Wrack / Mana Leak / Call Lightning enchantments but never read afterwards - Dasm dead store, kept for fidelity
*/
int16_t AI_Score_Combat_Spell(int16_t player_idx, int16_t spell_idx, int16_t player_mode_level)
{
    int16_t group_1[6] = { 0, 0, 0, 0, 0, 0 };
    int16_t group_2[6] = { 0, 0, 0, 0, 0, 0 };
    int16_t group_3[6] = { 0, 0, 0, 0, 0, 0 };
    int16_t group_4[6] = { 0, 0, 0, 0, 0, 0 };
    int16_t group_5[6] = { 0, 0, 0, 0, 0, 0 };
    int16_t rp_disenchant_prio = 0;
    int16_t target_value = 0;
    int16_t enemy_units = 0;
    int16_t rp_realm_threat_value = 0;  /* OGBUG:  rp_realm_threat_value is permanently 0, uses bogus realm variables */
    int16_t opp_spell_realms = 0;
    int16_t * opp_unit_realms = NULL;
    int16_t local_var = 0;
    int16_t spell_value = 0;
    int16_t counter = 0;
    int16_t battle_unit_idx = 0;
    int16_t realm_idx = 0;
    Assign_Combat_Grids();
    enemy_units = 0;
    for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
    {
        if((battle_units[battle_unit_idx].controller_idx != player_idx) && (battle_units[battle_unit_idx].status == bus_Active))
        {
            enemy_units++;
        }
    }
    if(enemy_units == 0)
    {
        return -100;
    }
    group_5[0] =  500;
    group_4[0] = -900;
    group_3[0] = -900;
    group_2[0] = -900;
    group_1[0] =    0;
    group_5[1] =    0;
    group_4[1] =   30;
    group_3[1] =   10;
    group_2[1] =   30;
    group_1[1] =   10;
    group_5[2] =   10;
    group_4[2] =   20;
    group_3[2] =   20;
    group_2[2] =   35;
    group_1[2] =   20;
    group_5[3] =   20;
    group_4[3] =   15;
    group_3[3] =   20;
    group_2[3] =   15;
    group_1[3] =   25;
    group_5[4] =   30;
    group_4[4] =   10;
    group_3[4] =   20;
    group_2[4] =    0;
    group_1[4] =   10;
    group_5[5] =   40;
    group_4[5] =    0;
    group_3[5] =   10;
    group_2[5] =    0;
    group_1[5] =  -10;
    if(player_idx == _combat_attacker_player)
    {
        opp_spell_realms = g_ai_combat_defender_realm_flags;
        opp_unit_realms = &g_ai_combat_defender_unit_realms[0];
        rp_realm_threat_value = g_ai_combat_unset_realm_flags;
        if((combat_enchantments[WRACK_DFNDR] > 0) || (combat_enchantments[MANA_LEAK_DFNDR] > 0) || (combat_enchantments[CALL_LIGHTNING_DFNDR] > 0))
        {
            rp_disenchant_prio = 500;
        }
        else
        {
            rp_disenchant_prio = 0;
        }
    }
    else
    {
        opp_spell_realms = g_ai_combat_attacker_realm_flags;
        opp_unit_realms = &g_ai_combat_attacker_unit_realms[0];
        rp_realm_threat_value = g_ai_combat_unset_realm_flags_2;
        if((combat_enchantments[WRACK_ATTKR] > 0) || (combat_enchantments[MANA_LEAK_ATTKR] > 0) || (combat_enchantments[CALL_LIGHTNING_ATTKR] > 0))
        {
            rp_disenchant_prio = 500;
        }
        else
        {
            rp_disenchant_prio = 0;
        }
    }
    /* OGBUG  passes &spell_value for both the X and Y return pointers */
    target_value = AITP_Combat_Spell(spell_idx, player_idx, &spell_value, &spell_value);
    if(target_value < 0)
    {
        return ST_UNDEFINED;
    }
    spell_value = 0;
    switch(spell_idx)
    {
        case spl_Earth_To_Mud:
            counter = 0;
            local_var = 0;
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                if(battle_units[battle_unit_idx].status != bus_Active) continue;
                if(!Battle_Unit_Has_Ranged_Attack(battle_unit_idx)) continue;
                if(battle_units[battle_unit_idx].controller_idx == player_idx)
                {
                    counter += (battle_units[battle_unit_idx].ranged * battle_units[battle_unit_idx].figure_cnt);
                }
                else
                {
                    local_var += (battle_units[battle_unit_idx].ranged * battle_units[battle_unit_idx].figure_cnt);
                }
            }
            if((counter * 2) > (local_var * 3))
            {
                spell_value += 15;
            }
            else
            {
                spell_value -= 100;
            }
            break;
        case spl_Resist_Elements:
            spell_value += ((opp_unit_realms[sbr_Nature] + opp_unit_realms[sbr_Chaos]) / 4);
            if(((opp_spell_realms & 0) != 0) || ((opp_spell_realms & 2) != 0))  /* OGBUG  `& 0` never passes */
            {
                spell_value += rp_realm_threat_value;
            }
            if(spell_value != 0)
            {
                spell_value += group_4[player_mode_level];
            }
            else
            {
                spell_value -= 100;
            }
            break;
        case spl_Wall_Of_Stone:
            counter = 0;
            if(player_idx == _combat_defender_player)
            {
                for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
                {
                    if(battle_units[battle_unit_idx].status != bus_Active) continue;
                    if(battle_units[battle_unit_idx].controller_idx == player_idx) continue;
                    if(battle_units[battle_unit_idx].Move_Flags & MV_FLYING) continue;
                    if(battle_units[battle_unit_idx].Move_Flags & MV_TELEPORT) continue;
                    if(battle_units[battle_unit_idx].Move_Flags & MV_MERGING) continue;
                    counter++;
                }
                spell_value += (group_4[player_mode_level] * counter);
            }
            else
            {
                spell_value = -100;
            }
            break;
        case spl_Web:
            spell_value += group_2[player_mode_level];
            counter = 0;
            for(battle_unit_idx = 0; ((battle_unit_idx < _combat_total_unit_count) && (counter == 0)); battle_unit_idx++)
            {
                if(battle_units[battle_unit_idx].status != bus_Active) continue;
                if(battle_units[battle_unit_idx].controller_idx == player_idx) continue;
                if((battle_units[battle_unit_idx].Move_Flags & MV_FLYING) == 0) continue;
                counter = 1;
            }
            spell_value += (counter * 10);
            break;
        case spl_Stone_Skin:
            spell_value += group_4[player_mode_level];
            spell_value += 15;
            break;
        case spl_Cracks_Call:
            if(_combat_structure != cs_OceanTerrainType)
            {
                spell_value += group_5[player_mode_level];
                if((player_idx == _combat_attacker_player) && (battlefield->walled > 0))
                {
                    spell_value += 20;
                }
            }
            else
            {
                spell_value = -100;
            }
            break;
        case spl_Ice_Bolt:
        case spl_Petrify:
        case spl_Psionic_Blast:
        case spl_Banish:
        case spl_Fire_Bolt:
        case spl_Lightning_Bolt:
        case spl_Warp_Lightning:
        case spl_Doom_Bolt:
        case spl_Magic_Vortex:
        case spl_Star_Fires:
        case spl_Dispel_Evil:
        case spl_Word_Of_Death:
        {
            spell_value += group_5[player_mode_level];
        } break;
        case spl_Elemental_Armor:
            spell_value += ((opp_unit_realms[sbr_Nature] + opp_unit_realms[sbr_Chaos]) / 2);
            if(((opp_spell_realms & 0) != 0) || ((opp_spell_realms & 2) != 0))  /* OGBUG  `& 0` never passes */
            {
                spell_value += (rp_realm_threat_value * 2);
            }
            if(spell_value > 0)
            {
                spell_value += group_4[player_mode_level];
            }
            else
            {
                spell_value -= 100;
            }
            break;
        case spl_Iron_Skin:
            spell_value += group_4[player_mode_level];
            spell_value += 35;
            break;
        case spl_Earth_Elemental:
        {
            if(_combat_structure == cs_OceanTerrainType)
            {
                spell_value -= 100;
            }
            else
            {
                spell_value += group_1[player_mode_level];
            }
        } break;
        case spl_Fire_Elemental:
        {
            if(_combat_structure == cs_OceanTerrainType)
            {
                spell_value -= 100;
            }
            else
            {
                spell_value += group_1[player_mode_level];
            }
        } break;
        case spl_Regeneration:
        case spl_Shatter:
        case spl_Holy_Armor:
        case spl_Cloak_Of_Fear:
        case spl_Wraith_Form:
        {
            spell_value += group_4[player_mode_level];
        } break;
        case spl_Call_Lightning:
            if(player_mode_level > 1)
            {
                spell_value += (enemy_units * 4);
            }
            spell_value += group_5[player_mode_level];
            break;
        case spl_Resist_Magic:
            counter = 0;
            for(realm_idx = 0; realm_idx < 5; realm_idx++)
            {
                counter += opp_unit_realms[realm_idx];
            }
            spell_value += (counter / 4);
            if(opp_spell_realms > 0)
            {
                spell_value += rp_realm_threat_value;
            }
            if(spell_value > 0)
            {
                spell_value += group_4[player_mode_level];
            }
            else
            {
                spell_value -= 100;
            }
            break;
        case spl_Dispel_Magic:
        {
            counter = 0;
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                if(battle_units[battle_unit_idx].status != bus_Active) continue;
                if(battle_units[battle_unit_idx].controller_idx == player_idx)
                {
                    /* own unit carrying a dispellable debuff (signed > 0: a set bue_NoEffect bit makes this negative and uncounted) */
                    if((int16_t)(battle_units[battle_unit_idx].combat_effects & (int16_t)~(bue_Haste | bue_Possession | bue_Creature_Binding)) > 0)
                    {
                        counter++;
                    }
                }
                else
                {
                    /* enemy unit carrying enchantments (signed: Invulnerability-only reads as none) or beneficial effects */
                    if(
                        ((int32_t)battle_units[battle_unit_idx].enchantments > 0)
                        ||
                        ((int16_t)(battle_units[battle_unit_idx].combat_effects & (bue_Haste | bue_Creature_Binding | bue_Possession)) > 0)
                        ||
                        ((int32_t)_UNITS[battle_units[battle_unit_idx].unit_idx].enchantments > 0)
                    )
                    {
                        counter++;
                    }
                }
            }
            if(counter > 0)
            {
                spell_value += 25;
            }
            else
            {
                spell_value -= 100;
            }
        } break;
        case spl_Dispel_Magic_True:
        {
            counter = 0;
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                if(battle_units[battle_unit_idx].status != bus_Active) continue;
                if(battle_units[battle_unit_idx].controller_idx == player_idx)
                {
                    /* own unit carrying a dispellable debuff (signed > 0: a set bue_NoEffect bit makes this negative and uncounted) */
                    if((int16_t)(battle_units[battle_unit_idx].combat_effects & (int16_t)~(bue_Haste | bue_Possession | bue_Creature_Binding)) > 0)
                    {
                        counter++;
                    }
                }
                else
                {
                    /* enemy unit carrying enchantments (signed: Invulnerability-only reads as none) or beneficial effects */
                    if(
                        ((int32_t)battle_units[battle_unit_idx].enchantments > 0)
                        ||
                        ((int16_t)(battle_units[battle_unit_idx].combat_effects & (bue_Haste | bue_Creature_Binding | bue_Possession)) > 0)
                        ||
                        ((int32_t)_UNITS[battle_units[battle_unit_idx].unit_idx].enchantments > 0)
                    )
                    {
                        counter++;
                    }
                }
            }
            if(counter > 0)
            {
                spell_value += 35;
            }
            else
            {
                spell_value -= 100;
            }
        } break;
        case spl_Guardian_Wind:
            counter = 0;
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                if((battle_units[battle_unit_idx].ranged_type / 10) != rag_Missile) continue;
                if(battle_units[battle_unit_idx].status != bus_Active) continue;
                if(battle_units[battle_unit_idx].controller_idx == player_idx) continue;
                counter += battle_units[battle_unit_idx].ranged;
            }
            if(counter > 0)
            {
                spell_value += group_4[player_mode_level];
                spell_value += counter;
            }
            break;
        case spl_Phantom_Warriors:
            if(_combat_structure == cs_OceanTerrainType)
            {
                spell_value -= 100;
            }
            else
            {
                spell_value += group_1[player_mode_level];
            }
            if((battlefield->city_enchantments[WALL_OF_FIRE] > 0) && (player_idx == _combat_attacker_player))
            {
                spell_value = -100;
            }
            break;
        case spl_Confusion:
        case spl_Vertigo:
        case spl_Mind_Storm:
        case spl_Warp_Wood:
        case spl_Warp_Creature:
        case spl_Weakness:
        {
            spell_value += group_2[player_mode_level];
        } break;
        case spl_Word_Of_Recall:
            if(_players[player_idx].casting_spell_idx == spl_Spell_Of_Return)
            {
                spell_value = -100;
            }
            else if(player_idx == HUMAN_PLAYER_IDX)
            {
                spell_value = -100;
            }
            else
            {
                counter = 0;
                for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
                {
                    if(battle_units[battle_unit_idx].status != bus_Active) continue;
                    if(battle_units[battle_unit_idx].controller_idx != player_idx) continue;
                    if(battle_units[battle_unit_idx].cost < 200) continue;
                    if((player_mode_level < 2) || ((battle_units[battle_unit_idx].front_figure_damage + 3) >= battle_units[battle_unit_idx].hits))
                    {
                        counter++;
                    }
                }
                if(counter != 0)
                {
                    spell_value += 100;
                }
                else
                {
                    spell_value -= 100;
                }
            }
            break;
        case spl_Counter_Magic:
            if(opp_spell_realms > 0)
            {
                spell_value += ((rp_realm_threat_value * 4) + 30);
                spell_value += group_4[player_mode_level];
            }
            else
            {
                spell_value = -100;
            }
            break;
        case spl_Disenchant_True:
        case spl_Disenchant_Area:
            counter = 0;
            spell_value = 0;
            local_var = 0;
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                /* count non-invisible units not owned by the human player (feeds the +150 "nothing hidden" bonus) */
                if(
                    ((battle_units[battle_unit_idx].enchantments & UE_INVISIBILITY) == 0)
                    &&
                    ((battle_units[battle_unit_idx].item_enchantments & UE_INVISIBILITY) == 0)
                    &&
                    ((battle_units[battle_unit_idx].Abilities & UA_INVISIBILITY) == 0)
                    &&
                    ((_UNITS[battle_units[battle_unit_idx].unit_idx].enchantments & UE_INVISIBILITY) == 0)
                    &&
                    (battle_units[battle_unit_idx].controller_idx != 0)
                )
                {
                    local_var++;
                }
                /* own unit carrying a dispellable debuff */
                if(
                    (battle_units[battle_unit_idx].status == bus_Active)
                    &&
                    (battle_units[battle_unit_idx].controller_idx == player_idx)
                    &&
                    ((int16_t)(battle_units[battle_unit_idx].combat_effects & (int16_t)~(bue_Haste | bue_Possession | bue_Creature_Binding)) > 0)
                )
                {
                    counter++;
                }
                /* enemy unit carrying enchantments or beneficial effects */
                if(
                    (battle_units[battle_unit_idx].status == bus_Active)
                    &&
                    (battle_units[battle_unit_idx].controller_idx != player_idx)
                    &&
                    (
                        ((int32_t)battle_units[battle_unit_idx].enchantments > 0)
                        ||
                        ((int16_t)(battle_units[battle_unit_idx].combat_effects & (bue_Haste | bue_Creature_Binding | bue_Possession)) > 0)
                        ||
                        ((int32_t)_UNITS[battle_units[battle_unit_idx].unit_idx].enchantments > 0)
                    )
                )
                {
                    /* Holy Arms exemption: a lone free Holy Weapon from the global enchantment is not worth a disenchant */
                    if(
                        (_players[battle_units[battle_unit_idx].controller_idx].Globals[HOLY_ARMS] != 0)
                        &&
                        (battle_units[battle_unit_idx].enchantments == UE_HOLY_WEAPON)
                        &&
                        (!((int16_t)(battle_units[battle_unit_idx].combat_effects & (bue_Haste | bue_Creature_Binding | bue_Possession)) > 0))
                        &&
                        (!((int32_t)_UNITS[battle_units[battle_unit_idx].unit_idx].enchantments > 0))
                    )
                    {
                        ;  /* not counted */
                    }
                    else
                    {
                        counter++;
                    }
                }
            }
            spell_value += (counter * 6);
            if(local_var == 0)
            {
                spell_value += 150;
            }
            /* count active battlefield enchantments on the opposing side (byte entries: even = attacker, odd = defender) */
            local_var = ((player_idx == _combat_attacker_player) ? 1 : 0);
            counter = 0;
            for(battle_unit_idx = local_var; battle_unit_idx < 30; battle_unit_idx += 2)
            {
                if(combat_enchantments[battle_unit_idx] == 1)
                {
                    counter++;
                }
            }
            spell_value += (counter * 30);
            if(spell_value == 0)
            {
                spell_value = -100;
            }
            break;
        case spl_Flight:
            counter = 0;
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                if((battle_units[battle_unit_idx].Move_Flags & MV_FLYING) == 0) continue;
                if(battle_units[battle_unit_idx].status != bus_Active) continue;
                if(battle_units[battle_unit_idx].controller_idx == player_idx) continue;
                counter++;
            }
            spell_value += group_4[player_mode_level];
            if(counter == 0)
            {
                spell_value += 20;
            }
            spell_value += (counter * 2);
            break;
        case spl_Phantom_Beast:
        case spl_Air_Elemental:
        {
            spell_value += group_1[player_mode_level];
        } break;
        case spl_Invisibility:
        {
            counter = 0;
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                if(battle_units[battle_unit_idx].status != bus_Active) continue;
                if(battle_units[battle_unit_idx].controller_idx == player_idx) continue;
                if(battle_units[battle_unit_idx].Attribs_1 & USA_IMMUNITY_ILLUSION) continue;
                counter++;
                if(battle_units[battle_unit_idx].ranged_type < srat_Thrown)
                {
                    counter += battle_units[battle_unit_idx].ranged;
                }
            }
            if(counter > 0)
            {
                spell_value += group_4[player_mode_level];
                spell_value += 5;
                spell_value += counter;
            }
            else
            {
                spell_value = -100;
            }
        } break;
        case spl_Mass_Invisibility:
        {
            counter = 0;
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                if(battle_units[battle_unit_idx].status != bus_Active) continue;
                if(battle_units[battle_unit_idx].controller_idx == player_idx) continue;
                if(battle_units[battle_unit_idx].Attribs_1 & USA_IMMUNITY_ILLUSION) continue;
                counter++;
                if(battle_units[battle_unit_idx].ranged_type < srat_Thrown)
                {
                    counter += battle_units[battle_unit_idx].ranged;
                }
            }
            if(counter > 0)
            {
                spell_value += group_4[player_mode_level];
                spell_value += counter;
            }
            else
            {
                spell_value -= 100;
            }
        } break;
        case spl_Magic_Immunity:
            counter = 0;
            for(realm_idx = 0; realm_idx < 5; realm_idx++)
            {
                counter += opp_unit_realms[realm_idx];
            }
            spell_value += (counter / 2);
            if(opp_spell_realms > 0)
            {
                spell_value += (rp_realm_threat_value * 3);
            }
            if(spell_value > 0)
            {
                spell_value += group_4[player_mode_level];
            }
            else
            {
                spell_value -= 100;
            }
            break;
        case spl_Haste:
        case spl_Heroism:
        {
            spell_value += group_3[player_mode_level];
        } break;
        case spl_Creature_Binding:
            counter = 0;
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                if(battle_units[battle_unit_idx].status != bus_Active) continue;
                if(battle_units[battle_unit_idx].controller_idx == player_idx) continue;
                if(battle_units[battle_unit_idx].race < rt_Arcane) continue;
                counter++;
            }
            if(counter == 1)
            {
                spell_value += group_5[player_mode_level];
            }
            else
            {
                spell_value += group_1[player_mode_level];
                spell_value += group_5[player_mode_level];
                spell_value += 20;
            }
            break;
        case spl_Disrupt:
            if((player_idx == _combat_attacker_player) && (battlefield->walled > 0))
            {
                spell_value += 20;
            }
            else
            {
                spell_value -= 100;
            }
            break;
        case spl_Eldritch_Weapon:
        {
            counter = 0;
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                if(battle_units[battle_unit_idx].status != bus_Active) continue;
                if(battle_units[battle_unit_idx].controller_idx == player_idx) continue;
                if(((battle_units[battle_unit_idx].enchantments | _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments | battle_units[battle_unit_idx].item_enchantments) & UE_INVULNERABILITY) != 0)
                {
                    counter++;
                }
            }
            if(counter > 0)
            {
                spell_value += 25;
            }
            spell_value += group_3[player_mode_level];
        } break;
        case spl_Flame_Blade:
        {
            counter = 0;
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                if(battle_units[battle_unit_idx].status != bus_Active) continue;
                if(battle_units[battle_unit_idx].controller_idx == player_idx) continue;
                if(((battle_units[battle_unit_idx].enchantments | _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments | battle_units[battle_unit_idx].item_enchantments) & UE_INVULNERABILITY) != 0)
                {
                    counter++;
                }
            }
            if(counter > 0)
            {
                spell_value += 25;
            }
            spell_value += group_3[player_mode_level];
        } break;
        case spl_Holy_Weapon:
        {
            counter = 0;
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                if(battle_units[battle_unit_idx].status != bus_Active) continue;
                if(battle_units[battle_unit_idx].controller_idx == player_idx) continue;
                if(((battle_units[battle_unit_idx].enchantments | _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments | battle_units[battle_unit_idx].item_enchantments) & UE_INVULNERABILITY) != 0)
                {
                    counter++;
                }
            }
            if(counter > 0)
            {
                spell_value += 25;
            }
            spell_value += group_3[player_mode_level];
        } break;
        case spl_Wall_Of_Fire:
            counter = 0;
            if(player_idx == _combat_defender_player)
            {
                for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
                {
                    if(battle_units[battle_unit_idx].status != bus_Active) continue;
                    if(battle_units[battle_unit_idx].controller_idx == player_idx) continue;
                    if(battle_units[battle_unit_idx].Move_Flags & MV_FLYING) continue;
                    if(battle_units[battle_unit_idx].Move_Flags & MV_TELEPORT) continue;
                    if(battle_units[battle_unit_idx].Move_Flags & MV_MERGING) continue;
                    counter++;
                }
                spell_value += ((group_3[player_mode_level] * (counter + 1)) / 2);
            }
            else
            {
                spell_value = -100;
            }
            break;
        case spl_Fireball:
            counter = 0;
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                if(battle_units[battle_unit_idx].status != bus_Active) continue;
                if(battle_units[battle_unit_idx].controller_idx == player_idx) continue;
                if(battle_units[battle_unit_idx].figure_cnt > counter)
                {
                    counter = battle_units[battle_unit_idx].figure_cnt;
                }
            }
            spell_value += group_5[player_mode_level];
            if(counter > 3)
            {
                spell_value += ((counter - 3) * 5);
            }
            break;
        case spl_Immolation:
        case spl_Berserk:
        {
            spell_value += group_3[player_mode_level];
            spell_value += 10;
        } break;
        case spl_Metal_Fires:
            counter = 0;
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                if(battle_units[battle_unit_idx].status != bus_Active) continue;
                if(battle_units[battle_unit_idx].controller_idx != player_idx) continue;
                if(battle_units[battle_unit_idx].race >= rt_Arcane) continue;
                counter++;
            }
            if(counter > 0)
            {
                spell_value += group_3[player_mode_level];
                spell_value += (counter * 5);
            }
            else
            {
                spell_value -= 100;
            }
            break;
        case spl_Warp_Reality:
            counter = 0;
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                if(battle_units[battle_unit_idx].status != bus_Active) continue;
                if(battle_units[battle_unit_idx].race != rt_Chaos) continue;
                if(battle_units[battle_unit_idx].controller_idx == player_idx)
                {
                    counter++;
                }
                else
                {
                    counter--;
                }
            }
            if(counter > 0)
            {
                spell_value += group_5[player_mode_level];
                spell_value += (counter * 5);
            }
            else
            {
                spell_value -= 100;
            }
            break;
        case spl_Flame_Strike:
        {
            spell_value += group_5[player_mode_level];
            spell_value += (enemy_units * 2);
        } break;
        case spl_Disintegrate:
        {
            spell_value += group_5[player_mode_level];
            spell_value += 25;
        } break;
        case spl_Bless:
            spell_value += ((opp_unit_realms[sbr_Death] + opp_unit_realms[sbr_Chaos]) / 4);
            if(((opp_spell_realms & 8) != 0) || ((opp_spell_realms & 2) != 0))
            {
                spell_value += rp_realm_threat_value;
            }
            if(spell_value != 0)
            {
                spell_value += group_4[player_mode_level];
            }
            else
            {
                spell_value -= 100;
            }
            break;
        case spl_Healing:
        {
            spell_value += (Effective_Battle_Unit_Strength(target_value) / 4);
        } break;
        case spl_True_Light:
            counter = 0;
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                if(battle_units[battle_unit_idx].status != bus_Active) continue;
                if((battle_units[battle_unit_idx].controller_idx == player_idx) && (battle_units[battle_unit_idx].race == rt_Life))
                {
                    counter++;
                }
                else if((battle_units[battle_unit_idx].controller_idx != player_idx) && (battle_units[battle_unit_idx].race == rt_Death))
                {
                    counter++;
                }
            }
            if(counter == 0)
            {
                spell_value = -100;
            }
            else
            {
                spell_value += (counter * 3);
            }
            break;
        case spl_True_Sight:
        {
            spell_value += (opp_unit_realms[sbr_Sorcery] / 2);
            if((opp_spell_realms & 1) != 0)
            {
                spell_value += (rp_realm_threat_value * 2);
            }
            if(spell_value != 0)
            {
                spell_value += group_4[player_mode_level];
            }
            else
            {
                spell_value -= 100;
            }
        } break;
        case spl_Raise_Dead:
            counter = 0;
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                if(battle_units[battle_unit_idx].status <= bus_Active) continue;
                if(battle_units[battle_unit_idx].controller_idx != player_idx) continue;
                if(battle_units[battle_unit_idx].race >= rt_Arcane) continue;
                if(battle_units[battle_unit_idx].status == bus_Gone) continue;
                if(_UNITS[battle_units[battle_unit_idx].unit_idx].wp == 9) continue;
                counter++;
            }
            if(counter > 0)
            {
                spell_value += group_1[player_mode_level];
            }
            else
            {
                spell_value -= 100;
            }
            break;
        case spl_Prayer:
        {
            counter = 0;
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                if(battle_units[battle_unit_idx].status != bus_Active) continue;
                if(battle_units[battle_unit_idx].controller_idx != player_idx) continue;
                counter++;
            }
            spell_value += group_3[player_mode_level];
            spell_value += (counter * 2);
        } break;
        case spl_High_Prayer:
            counter = 0;
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                if(battle_units[battle_unit_idx].status != bus_Active) continue;
                if(battle_units[battle_unit_idx].controller_idx != player_idx) continue;
                counter++;
            }
            spell_value += group_3[player_mode_level];
            spell_value += (counter * 4);
            break;
        case spl_Invulnerability:
        {
            spell_value += group_4[player_mode_level];
            spell_value += 30;
        } break;
        case spl_Righteousness:
            spell_value += (opp_unit_realms[sbr_Death] + opp_unit_realms[sbr_Chaos]);
            if(((opp_spell_realms & 8) != 0) || ((opp_spell_realms & 2) != 0))
            {
                spell_value += (rp_realm_threat_value * 2);
            }
            if(spell_value > 0)
            {
                spell_value += group_4[player_mode_level];
            }
            break;
        case spl_Mass_Healing:
            counter = 0;
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                if(battle_units[battle_unit_idx].status != bus_Active) continue;
                if(battle_units[battle_unit_idx].controller_idx != player_idx) continue;
                if((battle_units[battle_unit_idx].figure_cnt > battle_units[battle_unit_idx].figure_max) || (battle_units[battle_unit_idx].front_figure_damage > 0))
                {
                    counter++;
                }
            }
            if(counter > 0)
            {
                spell_value += (((counter - 1) * 10) + 5);
            }
            else
            {
                spell_value = -1000;
            }
            break;
        case spl_Holy_Word:
            counter = 0;
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                if(battle_units[battle_unit_idx].status != bus_Active) continue;
                if(battle_units[battle_unit_idx].controller_idx == player_idx) continue;
                if((Combat_Effective_Resistance(battle_units[battle_unit_idx], sbr_Life) - 2) >= 10) continue;
                if(battle_units[battle_unit_idx].race < rt_Arcane) continue;
                counter += battle_units[battle_unit_idx].figure_cnt;
            }
            if(counter > 0)
            {
                spell_value += group_5[player_mode_level];
                spell_value += (counter * 3);
            }
            else
            {
                spell_value -= 100;
            }
            break;
        case spl_Black_Sleep:
        {
            spell_value += group_2[player_mode_level];
            spell_value += group_5[player_mode_level];
        } break;
        case spl_Life_Drain:
        {
            spell_value += group_5[player_mode_level];
            spell_value += 10;
        } break;
        case spl_Terror:
            counter = 0;
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                if(battle_units[battle_unit_idx].status != bus_Active) continue;
                if(battle_units[battle_unit_idx].controller_idx == player_idx) continue;
                if((Combat_Effective_Resistance(battle_units[battle_unit_idx], sbr_Death) + 1) >= 10) continue;
                if(battle_units[battle_unit_idx].Attribs_1 & USA_IMMUNITY_DEATH) continue;
                counter++;
            }
            if(counter > 0)
            {
                spell_value += group_2[player_mode_level];
                spell_value += (counter * 3);
            }
            else
            {
                spell_value -= 100;
            }
            break;
        case spl_Darkness:
            counter = 0;
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                if(battle_units[battle_unit_idx].status != bus_Active) continue;
                if((battle_units[battle_unit_idx].controller_idx == player_idx) && (battle_units[battle_unit_idx].race == rt_Death))
                {
                    counter++;
                }
                else if((battle_units[battle_unit_idx].controller_idx != player_idx) && (battle_units[battle_unit_idx].race == rt_Life))
                {
                    counter++;
                }
            }
            if(counter == 0)
            {
                spell_value = -100;
            }
            else
            {
                spell_value += (counter * 3);
            }
            break;
        case spl_Mana_Leak:
            counter = 0;
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                if(battle_units[battle_unit_idx].status != bus_Active) continue;
                if(battle_units[battle_unit_idx].controller_idx == player_idx) continue;
                if((battle_units[battle_unit_idx].mana > 0) || ((battle_units[battle_unit_idx].ranged_type / 10) == rag_Magic))
                {
                    counter += battle_units[battle_unit_idx].figure_cnt;
                }
            }
            if(counter > 0)
            {
                spell_value += (10 - (_combat_turn * 2));
            }
            else
            {
                spell_value = -100;
            }
            break;
        case spl_Possession:
        {
            spell_value += group_5[player_mode_level];
            spell_value += group_1[player_mode_level];
        } break;
        case spl_Black_Prayer:
            counter = 0;
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                if(battle_units[battle_unit_idx].status != bus_Active) continue;
                if(battle_units[battle_unit_idx].controller_idx == player_idx) continue;
                counter++;
            }
            spell_value += group_4[player_mode_level];
            spell_value += (counter * 4);
            break;
        case spl_Wall_Of_Darkness:
            counter = 0;
            if(player_idx == _combat_defender_player)
            {
                for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
                {
                    if(battle_units[battle_unit_idx].status != bus_Active) continue;
                    if(battle_units[battle_unit_idx].controller_idx == player_idx) continue;
                    if(!Battle_Unit_Has_Ranged_Attack(battle_unit_idx)) continue;
                    counter += battle_units[battle_unit_idx].ranged;
                }
                spell_value += (group_4[player_mode_level] + (counter * 2));
            }
            else
            {
                spell_value = -100;
            }
            break;
        case spl_Wrack:
            counter = 0;
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                if(battle_units[battle_unit_idx].status != bus_Active) continue;
                if(battle_units[battle_unit_idx].controller_idx == player_idx) continue;
                if((Combat_Effective_Resistance(battle_units[battle_unit_idx], sbr_Death) + 1) >= 10) continue;
                if(battle_units[battle_unit_idx].Attribs_1 & USA_IMMUNITY_DEATH) continue;
                counter += battle_units[battle_unit_idx].figure_cnt;
            }
            if(counter > 0)
            {
                spell_value += group_5[player_mode_level];
                spell_value += (counter * 5);
            }
            else
            {
                spell_value -= 100;
            }
            break;
        case spl_Death_Spell:
            counter = 0;
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                if(battle_units[battle_unit_idx].status != bus_Active) continue;
                if(battle_units[battle_unit_idx].controller_idx == player_idx) continue;
                if((Combat_Effective_Resistance(battle_units[battle_unit_idx], sbr_Death) - 2) >= 10) continue;
                if(battle_units[battle_unit_idx].Attribs_1 & USA_IMMUNITY_DEATH) continue;
                counter += battle_units[battle_unit_idx].figure_cnt;
            }
            if(counter > 0)
            {
                spell_value += group_5[player_mode_level];
                spell_value += (counter * 3);
            }
            else
            {
                spell_value -= 100;
            }
            break;
        case spl_Animate_Dead:
            /* OGBUG:  the counted total is never applied to the score - Dasm dead code, kept for fidelity */
            counter = 0;
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                if(battle_units[battle_unit_idx].status <= bus_Active) continue;
                if(battle_units[battle_unit_idx].status == bus_Gone) continue;
                if(battle_units[battle_unit_idx].race == rt_Death) continue;
                if(_UNITS[battle_units[battle_unit_idx].unit_idx].Hero_Slot != ST_UNDEFINED) continue;
                if(_UNITS[battle_units[battle_unit_idx].unit_idx].wp == 9) continue;
                counter++;
            }
            break;
        case spl_Recall_Hero:
            if(_players[player_idx].casting_spell_idx == spl_Spell_Of_Return)
            {
                spell_value = -100;  /* OGBUG: falls through to the checks below, unlike spl_Word_Of_Recall */
            }
            if(player_idx == HUMAN_PLAYER_IDX)
            {
                spell_value = -100;
            }
            else
            {
                counter = 0;
                for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
                {
                    if(battle_units[battle_unit_idx].status != bus_Active) continue;
                    if(battle_units[battle_unit_idx].controller_idx != player_idx) continue;
                    if(_UNITS[battle_units[battle_unit_idx].unit_idx].Hero_Slot <= ST_UNDEFINED) continue;
                    if((player_mode_level < 2) || ((battle_units[battle_unit_idx].front_figure_damage + 3) >= battle_units[battle_unit_idx].hits))
                    {
                        counter++;
                    }
                }
                if(counter != 0)
                {
                    spell_value += 100;
                }
                else
                {
                    spell_value = -100;
                }
            }
            break;
    }
    spell_value += (Casting_Cost_Reduction(player_idx, spell_idx) / 2);
    spell_value += Random(20);
    return spell_value;
}

// WZD ovr139p03
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
    int16_t effective_strength = 0;
    if(battle_units[battle_unit_idx].ranged_type >= srat_Thrown)
    {
        short_range_strength = battle_units[battle_unit_idx].ranged;
        effective_strength = Get_Effective_Melee_Strength(battle_units[battle_unit_idx].melee, short_range_strength, battle_units[battle_unit_idx].figure_cnt, (battle_units[battle_unit_idx].attack_attributes | battle_units[battle_unit_idx].melee_attack_attributes), battle_units[battle_unit_idx].ranged_type);
    }
    else
    {
        short_range_strength = 0;
        effective_strength = Get_Effective_Melee_Strength(battle_units[battle_unit_idx].melee, short_range_strength, battle_units[battle_unit_idx].figure_cnt, (battle_units[battle_unit_idx].attack_attributes | battle_units[battle_unit_idx].melee_attack_attributes), ST_UNDEFINED);
    }
    effective_strength += Get_Effective_Hits(((battle_units[battle_unit_idx].hits * battle_units[battle_unit_idx].figure_cnt) - battle_units[battle_unit_idx].front_figure_damage), battle_units[battle_unit_idx].defense);
    if(Battle_Unit_Has_Ranged_Attack(battle_unit_idx) != ST_FALSE)
    {
        effective_strength += Get_Effective_Ranged_Strength(battle_units[battle_unit_idx].ranged, battle_units[battle_unit_idx].figure_cnt, (battle_units[battle_unit_idx].attack_attributes | battle_units[battle_unit_idx].ranged_attack_attributes));
    }
    return effective_strength;
}


// WZD ovr139p04
/* OGBUG: g_ai_combat_attacker_realm_flags and g_ai_combat_defender_unit_realms are not cleared, so flags accumulate across combat sessions */
void AI_Prepare_Combat_Realm_Threats(void)
{
    int16_t spell_list[92] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t range_modifier = 0;
    int16_t list_idx = 0;
    int16_t spell_type = 0;
    range_modifier = Combat_Casting_Cost_Multiplier(_combat_attacker_player);
    AI_Build_Castable_Combat_Spell_List((_combat_attacker_player + CASTER_IDX_BASE), &spell_list[0], range_modifier);
    for(list_idx = 0; list_idx < GUI_Multipurpose_Int; list_idx++)
    {
        spell_type = spell_data_table[spell_list[list_idx]].type;
        if(
            (spell_type == scc_Direct_Damage_Fixed)
            ||
            (spell_type == scc_Direct_Damage_Variable)
            ||
            (spell_type == scc_Combat_Destroy_Unit)
            ||
            (spell_type == scc_Resistable_Spell)
            ||
            (spell_type == scc_Mundane_Curse)
            ||
            (spell_list[list_idx] == spl_Creature_Binding)
        )
        {
            g_ai_combat_attacker_realm_flags |= (1 << spell_data_table[spell_list[list_idx]].magic_realm);
        }
    }
    range_modifier = Combat_Casting_Cost_Multiplier(_combat_defender_player);
    AI_Build_Castable_Combat_Spell_List((_combat_defender_player + CASTER_IDX_BASE), &spell_list[0], range_modifier);
    for(list_idx = 0; list_idx < GUI_Multipurpose_Int; list_idx++)
    {
        spell_type = spell_data_table[spell_list[list_idx]].type;
        if(
            (spell_type == scc_Direct_Damage_Fixed)
            ||
            (spell_type == scc_Combat_Destroy_Unit)
            ||
            (spell_type == scc_Direct_Damage_Variable)
            ||
            (spell_type == scc_Resistable_Spell)
            ||
            (spell_type == scc_Mundane_Curse)
            ||
            (spell_list[list_idx] == spl_Creature_Binding)
            ||
            (spell_list[list_idx] == spl_Chaos_Channels)  /* OGBUG: defender-pass extra - asymmetry; see AITP_DispelMagic() */
        )
        {
            g_ai_combat_defender_realm_flags |= (1 << spell_data_table[spell_list[list_idx]].magic_realm);
        }
    }
    AI_Build_Unit_Realm_Threat_Percentages(_combat_attacker_player, &g_ai_combat_attacker_unit_realms[0]);
    AI_Build_Unit_Realm_Threat_Percentages(_combat_defender_player, &g_ai_combat_defender_unit_realms[0]);
}


// WZD ovr139p05
void AI_Build_Unit_Realm_Threat_Percentages(int16_t player_idx, int16_t * realm_array)
{
    int32_t enchantments = 0;
    int16_t total_strength = 0;
    int16_t itr = 0;
    int16_t battle_unit_idx = 0;
    total_strength = 0;
    for(itr = 0; itr < 5; itr++)
    {
        realm_array[itr] = 0;
    }
    for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
    {
        if(battle_units[battle_unit_idx].controller_idx != player_idx)
        {
            continue;
        }
        if(battle_units[battle_unit_idx].status != bus_Active)
        {
            continue;
        }
        enchantments = battle_units[battle_unit_idx].enchantments;
        enchantments |= battle_units[battle_unit_idx].item_enchantments;
        enchantments |= _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments;
        total_strength += (battle_units[battle_unit_idx].melee * battle_units[battle_unit_idx].figure_cnt);
        total_strength += (battle_units[battle_unit_idx].ranged * battle_units[battle_unit_idx].figure_cnt);
        if((battle_units[battle_unit_idx].ranged_type == srat_FireBreath) || (battle_units[battle_unit_idx].ranged_type == srat_Lightning))
        {
            realm_array[sbr_Chaos] += (battle_units[battle_unit_idx].ranged * battle_units[battle_unit_idx].figure_cnt);
        }
        if(battle_units[battle_unit_idx].ranged_type == srat_StoneGaze)
        {
            realm_array[sbr_Nature] += (battle_units[battle_unit_idx].ranged * battle_units[battle_unit_idx].figure_cnt);
        }
        if((battle_units[battle_unit_idx].Attribs_2 & USA_FIREBALL) || (battle_units[battle_unit_idx].Attribs_2 & USA_DOOMBOLT))
        {
            total_strength += 10;
            realm_array[sbr_Chaos] += 10;
        }
        if((battle_units[battle_unit_idx].Attribs_2 & USA_IMMOLATION) || (enchantments & UE_IMMOLATION))
        {
            total_strength += 20;
            realm_array[sbr_Chaos] += 20;
        }
        if((battle_units[battle_unit_idx].attack_attributes & Att_LifeSteal) || (battle_units[battle_unit_idx].melee_attack_attributes & Att_LifeSteal) || (battle_units[battle_unit_idx].ranged_attack_attributes & Att_LifeSteal))
        {
            total_strength += ((battle_units[battle_unit_idx].Spec_Att_Attrib + 3) * battle_units[battle_unit_idx].figure_cnt);
            realm_array[sbr_Death] += ((battle_units[battle_unit_idx].Spec_Att_Attrib + 3) * battle_units[battle_unit_idx].figure_cnt);
        }
        if((battle_units[battle_unit_idx].attack_attributes & Att_Destruct) || (battle_units[battle_unit_idx].melee_attack_attributes & Att_Destruct) || (battle_units[battle_unit_idx].ranged_attack_attributes & Att_Destruct))
        {
            total_strength += (battle_units[battle_unit_idx].figure_cnt * 5);
            realm_array[sbr_Chaos] += (battle_units[battle_unit_idx].figure_cnt * 5);
        }
        if((battle_units[battle_unit_idx].attack_attributes & Att_StnTouch) || (battle_units[battle_unit_idx].melee_attack_attributes & Att_StnTouch) || (battle_units[battle_unit_idx].ranged_attack_attributes & Att_StnTouch))
        {
            total_strength += (battle_units[battle_unit_idx].figure_cnt * 5);
            realm_array[sbr_Nature] += (battle_units[battle_unit_idx].figure_cnt * 5);
        }
        if((battle_units[battle_unit_idx].Attribs_2 & USA_CAUSEFEAR) || (enchantments & UE_CLOAK_OF_FEAR))
        {
            total_strength += battle_units[battle_unit_idx].figure_cnt;
            realm_array[sbr_Death] += battle_units[battle_unit_idx].figure_cnt;
        }
        if((battle_units[battle_unit_idx].attack_attributes & Att_DthTouch) || (battle_units[battle_unit_idx].melee_attack_attributes & Att_DthTouch) || (battle_units[battle_unit_idx].ranged_attack_attributes & Att_DthTouch))
        {
            total_strength += (battle_units[battle_unit_idx].figure_cnt * 5);
            realm_array[sbr_Death] += (battle_units[battle_unit_idx].figure_cnt * 5);
        }
        if((battle_units[battle_unit_idx].attack_attributes & Att_DsplEvil) || (battle_units[battle_unit_idx].melee_attack_attributes & Att_DsplEvil) || (battle_units[battle_unit_idx].ranged_attack_attributes & Att_DsplEvil))
        {
            total_strength += (battle_units[battle_unit_idx].figure_cnt * 5);
            realm_array[sbr_Life] += (battle_units[battle_unit_idx].figure_cnt * 5);
        }
    }
    if(total_strength != 0)
    {
        for(itr = 0; itr < 5; itr++)
        {
            realm_array[itr] = (int16_t)(((int32_t)realm_array[itr] * 100) / total_strength);
        }
    }
}


// WZD ovr139p06
void AI_Build_Castable_Combat_Spell_List(int16_t caster_idx, int16_t * spell_list, int16_t range_mod)
{
    int16_t mana_limit = 0;
    int16_t hero_spell_idx = 0;
    int16_t spell_cost = 0;
    int16_t in_realm_idx = 0;
    int16_t realm_idx = 0;
    int16_t player_idx = 0;
    int16_t spell_idx = 0;
    GUI_Multipurpose_Int = 0;
    if(caster_idx > (CASTER_IDX_BASE - 1))
    {
        player_idx = (caster_idx - CASTER_IDX_BASE);
        mana_limit = _players[player_idx].Cmbt_Skill_Left;
        if((int16_t)(((int32_t)_players[player_idx].mana_reserve * 10) / range_mod) < mana_limit)
        {
            mana_limit = (int16_t)(((int32_t)_players[player_idx].mana_reserve * 10) / range_mod);
        }
    }
    else
    {
        player_idx = battle_units[caster_idx].controller_idx;
        mana_limit = (uint8_t)battle_units[caster_idx].mana;
    }
    if(caster_idx < CASTER_IDX_BASE)
    {
        if((battle_units[caster_idx].Attribs_1 & (USA_CASTER_20 | USA_CASTER_40)) != 0)
        {
            /* caster_idx creature: only its own realm's spells (fantastic race - 16 == sbr_ realm) */
            for(in_realm_idx = 0; in_realm_idx < NUM_SPELLS_PER_MAGIC_REALM; in_realm_idx++)
            {
                spell_idx = (((battle_units[caster_idx].race - 16) * NUM_SPELLS_PER_MAGIC_REALM) + in_realm_idx + 1);
                if(spell_data_table[spell_idx].Eligibility == 1) { continue; }
                spell_cost = spell_data_table[spell_idx].casting_cost;
                if(spell_cost > mana_limit) { continue; }
                spell_list[GUI_Multipurpose_Int] = spell_idx;
                GUI_Multipurpose_Int++;
            }
            return;
        }
    }
    for(realm_idx = 0; realm_idx < NUM_MAGIC_REALMS; realm_idx++)
    {
        for(in_realm_idx = 0; in_realm_idx < NUM_SPELLS_PER_MAGIC_REALM; in_realm_idx++)
        {
            spell_idx = ((realm_idx * NUM_SPELLS_PER_MAGIC_REALM) + in_realm_idx + 1);
            if(_players[player_idx].spells_list[((realm_idx * NUM_SPELLS_PER_MAGIC_REALM) + in_realm_idx)] != sls_Known) continue;
            if(spell_data_table[spell_idx].Eligibility == 1) continue;
            if(caster_idx > (CASTER_IDX_BASE - 1))
            {
                spell_cost = Casting_Cost(player_idx, spell_idx, 1);
            }
            else
            {
                spell_cost = spell_data_table[spell_idx].casting_cost;
            }
            if(spell_cost > mana_limit) continue;
            spell_list[GUI_Multipurpose_Int] = spell_idx;
            GUI_Multipurpose_Int++;
        }
    }
    /* OGBUG  the hero-spell scan is gated on caster_idx < 19, not < 20, so battle unit #19 never offers hero spells */
    if(caster_idx < (CASTER_IDX_BASE - 1))  /* OGBUG: battle unit #19 is excluded; should be `< CASTER_IDX_BASE` */
    {
        for(hero_spell_idx = 0; hero_spell_idx < NUM_HERO_SPELL_SLOTS; hero_spell_idx++)
        {
            /* OGBUG  hero spell bytes are sign-extended: an innate spell with id > 127 reads as negative and is skipped */
            spell_idx = (int8_t)_HEROES2[player_idx]->heroes[_UNITS[battle_units[caster_idx].unit_idx].type].Spells[hero_spell_idx];  /* OGBUG  sign-extended byte */
            if(spell_idx <= 0) { continue; }
            if(spell_data_table[spell_idx].Eligibility == 1) { continue; }
            if(spell_data_table[spell_idx].casting_cost > mana_limit) { continue; }
            spell_list[GUI_Multipurpose_Int] = spell_idx;
            GUI_Multipurpose_Int++;
        }
    }
}



/*
    WIZARDS.EXE  ovr153
*/

// WZD ovr153p01
/*
    ¿ debug for testing the combat map gen code ?
Combat_Screen()
    Allocate_Combat_Base_Blocks();
    Combat_Screen_Load_Resources();  / * calls Allocate_Combat_Near_Buffers() * /
    _combat_wx = wx;
    _combat_wy = wy;
    _combat_wp = wp;
    _combat_attacker_player = combat_attacker_player_idx;
    _combat_defender_player = combat_defender_player_idx;
    Cache_Graphics_Combat();
    Build_Battlefield(wx, wy, wp);
*/
void NX_IDK_CombatInit_Tactical(int16_t wx, int16_t wy, int16_t wp)
{
    Allocate_Combat_Base_Blocks();
    Build_Battlefield(wx, wy, wp);
}


// WZD ovr153p02
void Combat_Screen_Map_Draw(void)
{
    int16_t Road_Flags = 0;
    int16_t Set_Base_2 = 0;
    int16_t Set_Base_1 = 0;
    int16_t cgy = 0;
    int16_t cgx = 0;
    int16_t screen_y = 0;
    int16_t screen_x = 0;
    int16_t row_start_cgy = 0;
    int16_t row_start_cgx = 0;
    int16_t itr_x = 0;
    int16_t itr_y = 0;
    int16_t battlefield_terrain_type = 0;
    Copy_Back_To_Off();  // 'combat background' from Combat_Screen_Compose_Background()
    Combat_Screen_Map_Compose_Figures();
    Set_Page_Off();
    Set_Window(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, COMBAT_MAP_YMAX);
    for(itr_y = 0; itr_y < 22; itr_y++)
    {
        CALC_ROW_START_CGX;
        CALC_ROW_START_CGY;
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
                if(battlefield->muds[((cgy * COMBAT_GRID_WIDTH) + cgx)] != ST_FALSE)
                {
                    Set_Animation_Frame(cmbtcity_mud_seg, ((cgx + cgy + _combat_mud_anim_phase) % 8));
                    Clipped_Draw(screen_x, screen_y, cmbtcity_mud_seg);
                    continue;
                }
                /*
                    BEGIN:  Update/Redraw Animated Terrains
                */
                battlefield_terrain_type = battlefield->terrain_type[((cgy * COMBAT_GRID_WIDTH) + cgx)];
                // CTILE_LeftRightRiver1  = 56, CTILE_UpDownRiver1  = 57, CTILE_UpLeftRiver1  = 58, CTILE_UpRightRiver1  = 59, CTILE_DownRightRiver1  = 60, CTILE_DownLeftRiver1  = 61, CTILE_LeftRightRiver2  = 62, CTILE_UpDownRiver2  = 63, CTILE_UpLeftRiver2  = 64, CTILE_UpRightRiver2  = 65, CTILE_DownRightRiver2  = 66, CTILE_DownLeftRiver2  = 67
                if(battlefield_terrain_type >= btt_River_First)
                {
                    if(battlefield->wp == ARCANUS_PLANE)
                    {
                        Set_Animation_Frame(cmbtcity_river_segs[(battlefield_terrain_type - btt_River_First)], _combat_water_anim_frame);
                        Clipped_Draw(screen_x, screen_y, cmbtcity_river_segs[(battlefield_terrain_type - btt_River_First)]);
                    }
                    else  /* MYRROR_PLANE */
                    {
                        Set_Animation_Frame(chriver_river_segs[(battlefield_terrain_type - btt_River_First)], _combat_water_anim_frame);
                        Clipped_Draw(screen_x, screen_y, chriver_river_segs[(battlefield_terrain_type - btt_River_First)]);
                    }
                }
                else if((battlefield_terrain_type >= btt_Ocean_First) && (battlefield_terrain_type <= btt_Ocean_Last))  // CTILE_Ocean1  = 48, CTILE_Ocean2  = 49, CTILE_Ocean3  = 50, CTILE_Ocean4  = 51,
                {
                    if(battlefield->wp == ARCANUS_PLANE)
                    {
                        Set_Animation_Frame(cmbtcity_ocean_segs[(battlefield_terrain_type - btt_Ocean_First)], _combat_water_anim_frame);
                        Clipped_Draw(screen_x, screen_y, cmbtcity_ocean_segs[(battlefield_terrain_type - btt_Ocean_First)]);
                    }
                    else  /* MYRROR_PLANE */
                    {
                        Set_Animation_Frame(chriver_chaoswat_segs[(battlefield_terrain_type - btt_Ocean_First)], _combat_water_anim_frame);
                        Clipped_Draw(screen_x, screen_y, chriver_chaoswat_segs[(battlefield_terrain_type - btt_Ocean_First)]);
                    }
                }
                else if((battlefield_terrain_type >= btt_Cloud_First) && (battlefield_terrain_type <= btt_Cloud_Last))  // CTILE_Type5_1  = 52, CTILE_Type5_2  = 53, CTILE_Type5_3  = 54, CTILE_Type5_4  = 55,
                {
                    Set_Animation_Frame(cmbtcity_cmbcloud_segs[(battlefield_terrain_type - btt_Cloud_First)], _combat_water_anim_frame);
                    Clipped_Draw(screen_x, screen_y, cmbtcity_cmbcloud_segs[(battlefield_terrain_type - btt_Cloud_First)]);
                }
                if(
                    (battlefield->center_square_structure == CS_SorceryNode) && (cgx == CGX_LAIR) && (cgy == CGY_LAIR))
                {
                    Combat_Grid_Screen_Coordinates(CGX_LAIR, CGY_LAIR, 0, 0, &screen_x, &screen_y);
                    screen_x -= 46;
                    screen_y -= 15;
                    Set_Animation_Frame(IMG_CMB_SorcNode, _combat_chaos_node_anim_frame);
                    Clipped_Draw(screen_x, screen_y, IMG_CMB_SorcNode);
                    screen_x += 46;
                    screen_y += 15;
                }
                if(
                    (battlefield->center_square_structure == CS_ChaosNode) && (cgx == CGX_LAIR) && (cgy == CGY_LAIR))
                {
                    Combat_Grid_Screen_Coordinates(6, 11, 0, 0, &screen_x, &screen_y);
                    screen_x -= 10;
                    screen_y -= 2;
                    FLIC_Draw(screen_x, screen_y, chriver_volca_segs[(_combat_chaos_node_anim_frame % 8)]);
                    screen_x += 10;
                    screen_y += 2;
                }
                /*
                    END:  Update/Redraw Animated Terrains
                */
                /*
                    BEGIN:  Roads
                */
                Road_Flags = battlefield->roads[((cgy * COMBAT_GRID_WIDTH) + cgx)];
                if(Road_Flags != 0)
                {
                    if((Road_Flags & 0x80) != 0 && (Road_Flags & 0x01) != 0)
                    {
                        Set_Animation_Frame(cmbtcity_flotisle_seg, _combat_road_anim_frame);
                        Clipped_Draw((screen_x - 32), (screen_y - 48), cmbtcity_flotisle_seg);
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
                            Set_Animation_Frame(cmbtcity_roadgrid_segs[(0 + (Set_Base_1 + Set_Base_2))], _combat_road_anim_frame);
                            Clipped_Draw(screen_x, screen_y, cmbtcity_roadgrid_segs[(0 + (Set_Base_1 + Set_Base_2))]);
                        }
                        if((Road_Flags & 0x04) != 0)
                        {
                            Set_Animation_Frame(cmbtcity_roadgrid_segs[(1 + (Set_Base_1 + Set_Base_2))], _combat_road_anim_frame);
                            Clipped_Draw(screen_x, screen_y, cmbtcity_roadgrid_segs[(1 + (Set_Base_1 + Set_Base_2))]);
                        }
                        if((Road_Flags & 0x08) != 0)
                        {
                            Set_Animation_Frame(cmbtcity_roadgrid_segs[(2 + (Set_Base_1 + Set_Base_2))], _combat_road_anim_frame);
                            Clipped_Draw(screen_x, screen_y, cmbtcity_roadgrid_segs[(2 + (Set_Base_1 + Set_Base_2))]);
                        }
                        if((Road_Flags & 0x10) != 0)
                        {
                            Set_Animation_Frame(cmbtcity_roadgrid_segs[(3 + (Set_Base_1 + Set_Base_2))], _combat_road_anim_frame);
                            Clipped_Draw(screen_x, screen_y, cmbtcity_roadgrid_segs[(3 + (Set_Base_1 + Set_Base_2))]);
                        }
                        if((Road_Flags & 0x20) != 0)
                        {
                            Set_Animation_Frame(cmbtcity_roadgrid_segs[(4 + (Set_Base_1 + Set_Base_2))], _combat_road_anim_frame);
                            Clipped_Draw(screen_x, screen_y, cmbtcity_roadgrid_segs[(4 + (Set_Base_1 + Set_Base_2))]);
                        }
                        if((Road_Flags & 0x40) != 0)
                        {
                            Set_Animation_Frame(cmbtcity_roadgrid_segs[(5 + (Set_Base_1 + Set_Base_2))], _combat_road_anim_frame);
                            Clipped_Draw(screen_x, screen_y, cmbtcity_roadgrid_segs[(5 + (Set_Base_1 + Set_Base_2))]);
                        }
                    }
                }
                /*
                    END:  Roads
                */
                if(
                    (battlefield->center_square_structure == CS_Outpost)&& (cgx == CGX_LAIR) && (cgy == CGY_LAIR))
                {
                    Clipped_Draw(screen_x, screen_y, IMG_CMB_DirtTile);
                }
                /* OGBUG: skips Flying Fortress for Outposts */
                if(
                    (
                        (battlefield->center_square_structure == CS_City)
                        ||
                        (battlefield->center_square_structure == CS_Fortress)
                    )
                    &&
                    (cgx == MAX_CGX_CITY)
                    &&
                    (cgy == MAX_CGY_CITY)
                )
                {
                    if(battlefield->city_enchantments[FLYING_FORTRESS] != 0)
                    {
                        Clipped_Draw((screen_x - 48), (screen_y - 48), cmbtcity_cmbcloud_segs[0]);
                    }
                    else
                    {
                        Clipped_Draw((screen_x - 48), (screen_y - 48), IMG_CMB_RoadGrid);
                    }
                }
                /* ¿ only one cell at a time can have an effect ? */
                if(
                    (cmbt_cell_effect_active == ST_TRUE)
                    &&
                    (cmbt_cell_effect_cgx == cgx)
                    &&
                    (cmbt_cell_effect_cgy == cgy)
                )
                {
                    Set_Animation_Frame(cmbt_cell_effect_seg, (cmbt_cell_effect_frame / CELL_EFFECT_ANIM_HOLD));  /* HACK  to slow down the cycle */
                    Clipped_Draw(screen_x, (screen_y - 18), cmbt_cell_effect_seg);
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
            CALC_ROW_START_CGX;
            CALC_ROW_START_CGY;
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
                    Set_Animation_Frame(frame_scanned_seg, frame_anim_cycle);
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
                    Set_Animation_Frame(frame_active_seg, frame_anim_cycle);
                    Clipped_Draw(screen_x, screen_y, frame_active_seg);
                }
            }
        }
    }
    /*
        END:  Draw Outlines  (red,blue (active unit,scanned square))
    */
    _combat_road_anim_frame = ((_combat_road_anim_frame + 1) % 5);
    _combat_water_anim_frame = ((_combat_water_anim_frame + 1) % 5);
    _combat_chaos_node_anim_frame = ((_combat_chaos_node_anim_frame + 1) % 8);
    if(_combat_magic_wall_anim_skip == 0)
    {
        _combat_magic_wall_anim_frame = ((_combat_magic_wall_anim_frame + 1) % 4);
    }
    _combat_magic_wall_anim_skip = (1 - _combat_magic_wall_anim_skip);
    _combat_curse_anim_frame++;
    if(_combat_curse_anim_frame > MAX_SINT)
    {
        _combat_curse_anim_frame = 0;
    }
    if(combat_walk_anim_dir == 0)
    {
        combat_walk_anim_frame++;
        if(combat_walk_anim_frame == 3)
        {
            combat_walk_anim_dir = 1;
            combat_walk_anim_frame = 1;
        }
    }
    else
    {
        combat_walk_anim_frame--;
        if(combat_walk_anim_frame == -1)
        {
            combat_walk_anim_dir = 0;
            combat_walk_anim_frame = 1;
        }
    }
    combat_idle_anim_frame = ((combat_idle_anim_frame + 1) % 3);
    if(_combat_unit_attack_anim_frame == 1)
    {
        _combat_unit_attack_anim_frame = 3;
    }
    else
    {
        _combat_unit_attack_anim_frame = 1;
    }
    if(_combat_unit_defend_anim_frame == 1)
    {
        _combat_unit_defend_anim_frame = 3;
    }
    else
    {
        _combat_unit_defend_anim_frame = 1;
    }
    Combat_Screen_Map_Draw_Entities();  // OON XREF
    Reset_Window();
}


// WZD ovr153p03
void Combat_Screen_Map_Draw_Entities(void)
{
    int16_t combat_grid_cell_y_offset = 0;
    int16_t combat_grid_cell_x_offset = 0;
    int16_t combat_grid_cell_y = 0;
    int16_t combat_grid_cell_x = 0;
    int16_t gibs_draw_x_shift = 0;
    int16_t gibs_draw_y_shift = 0;
    int16_t itr = 0;
    int16_t combat_grid_entity_idx = 0;
    for(itr = 0; itr < combat_grid_entity_count; itr++)
    {
        combat_grid_entity_idx = combat_grid_entities_draw_order[itr];
        switch(combat_grid_entities[combat_grid_entity_idx].entity_type)
        {
            case cet_Battle_Unit_Figure:
            {
                if(combat_grid_entities[combat_grid_entity_idx].gibs == 0)
                {
                    Draw_Picture_Windowed((combat_grid_entities[combat_grid_entity_idx].draw_x - combat_grid_entities[combat_grid_entity_idx].draw_x_shift), (combat_grid_entities[combat_grid_entity_idx].draw_y - combat_grid_entities[combat_grid_entity_idx].draw_y_shift), battle_unit_picts_seg[combat_grid_entities[combat_grid_entity_idx].index]);
                }
                else
                {
                    if(combat_grid_entities[combat_grid_entity_idx].Blood_Frame != 3)
                    {
                        Draw_Picture_Windowed((combat_grid_entities[combat_grid_entity_idx].draw_x - combat_grid_entities[combat_grid_entity_idx].draw_x_shift), (combat_grid_entities[combat_grid_entity_idx].draw_y - combat_grid_entities[combat_grid_entity_idx].draw_y_shift), battle_unit_picts_seg[combat_grid_entities[combat_grid_entity_idx].index]);
                        if(combat_grid_entities[combat_grid_entity_idx].Blood_Frame >= 0)
                        {
                            Set_Animation_Frame(cmbtcity_blood_segs[(combat_grid_entities[combat_grid_entity_idx].gibs - 1)], combat_grid_entities[combat_grid_entity_idx].Blood_Frame);
                            gibs_draw_y_shift = 11;
                            gibs_draw_x_shift = 3;
                            Clipped_Draw((combat_grid_entities[combat_grid_entity_idx].draw_x - gibs_draw_x_shift), (combat_grid_entities[combat_grid_entity_idx].draw_y - gibs_draw_y_shift), cmbtcity_blood_segs[(combat_grid_entities[combat_grid_entity_idx].gibs - 1)]);
                        }
                    }
                }
            } break;
            case cet_NONE:
            {
                Set_Animation_Frame(combat_grid_entities[combat_grid_entity_idx].pict_seg, combat_grid_entities[combat_grid_entity_idx].frame_num);
                Clipped_Draw((combat_grid_entities[combat_grid_entity_idx].draw_x - combat_grid_entities[combat_grid_entity_idx].draw_x_shift), (combat_grid_entities[combat_grid_entity_idx].draw_y - combat_grid_entities[combat_grid_entity_idx].draw_y_shift), combat_grid_entities[combat_grid_entity_idx].pict_seg);
            } break;
            case cet_Tree_Or_Rock:
            {
                Map_Tile_EMS_Page_As_Sandbox();
                Clipped_Draw((combat_grid_entities[combat_grid_entity_idx].draw_x - combat_grid_entities[combat_grid_entity_idx].draw_x_shift), (combat_grid_entities[combat_grid_entity_idx].draw_y - combat_grid_entities[combat_grid_entity_idx].draw_y_shift), combat_grid_entities[combat_grid_entity_idx].pict_seg);
            } break;
        }
    }
    for(itr = 0; itr < combat_grid_entity_count; itr++)
    {
        combat_grid_entity_idx = combat_grid_entities_draw_order[itr];
        switch(combat_grid_entities[combat_grid_entity_idx].entity_type)
        {
            case cet_Missiles:
            {
                Set_Animation_Frame(combat_grid_entities[combat_grid_entity_idx].pict_seg, projectile_anim_frame);
                Clipped_Draw((combat_grid_entities[combat_grid_entity_idx].draw_x - combat_grid_entities[combat_grid_entity_idx].draw_x_shift), (combat_grid_entities[combat_grid_entity_idx].draw_y - combat_grid_entities[combat_grid_entity_idx].draw_y_shift), combat_grid_entities[combat_grid_entity_idx].pict_seg);
            } break;
            case cet_Curse:
            {
                Set_Animation_Frame(combat_grid_entities[combat_grid_entity_idx].pict_seg, _combat_curse_anim_frame);
                Clipped_Draw((combat_grid_entities[combat_grid_entity_idx].draw_x - combat_grid_entities[combat_grid_entity_idx].draw_x_shift), (combat_grid_entities[combat_grid_entity_idx].draw_y - combat_grid_entities[combat_grid_entity_idx].draw_y_shift), combat_grid_entities[combat_grid_entity_idx].pict_seg);
            } break;
        }
    }
    Screen_To_Combat_Grid_Cell_X_And_Offset(Pointer_X(), Pointer_Y(), &combat_grid_cell_x, &combat_grid_cell_x_offset);
    Screen_To_Combat_Grid_Cell_Y_And_Offset(Pointer_X(), Pointer_Y(), &combat_grid_cell_y, &combat_grid_cell_y_offset);
}


// WZD ovr153p04
void NIU_Gibs_Frames(void)
{
    int16_t itr = 0;
    for(itr = 0; itr < 20; itr++)
    {
        m_gibs_frames[itr] = (Random(4) - 1);
    }
}

// WZD ovr153p05
void Spawn_Tree_Entities(void)
{
    int16_t itr = 0;
    for(itr = 0; battlefield->tree_count > itr; itr++)
    {
        Combat_Grid_Entity_Create(
            battlefield->Tree_DrawXs[itr],
            battlefield->Tree_DrawYs[itr],
            (int64_t)IMG_CMB_Trees[battlefield->Tree_Indices[itr]],
            8,
            13,
            0,
            cet_Tree_Or_Rock,
            0,
            0,
            0,
            0,
            0,
            0
        );
    }
}

// WZD ovr153p06
void Spawn_Rock_Entities(void)
{
    int16_t itr = 0;
    for(itr = 0; battlefield->tree_count > itr; itr++)  /* OGBUG: rocks uses tree_count */
    {
        Combat_Grid_Entity_Create(
            battlefield->rock_sx_array[itr],
            battlefield->rock_sy_array[itr],
            (int64_t)IMG_CMB_Rocks[battlefield->Tree_Indices[itr]],  /* OGBUG: rocks uses Tree_Indices */
            6,
            12,
            0,
            cet_Tree_Or_Rock,
            0,
            0,
            0,
            0,
            0,
            0
        );
    }
}


// WZD ovr153p07
void Spawn_Structure_Entities(void)
{
    int16_t screen_y = 0;
    int16_t screen_x = 0;
    SAMB_ptr structure_pict_seg = 0;
    int16_t itr = 0;
    if(battlefield->center_square_structure != CS_None)
    {
        Combat_Grid_Screen_Coordinates(CGX_LAIR, CGY_LAIR, 0, 0, &screen_x, &screen_y);
        switch(battlefield->center_square_structure)
        {
            case CS_Outpost:        { structure_pict_seg = IMG_CMB_Outpost;     } break;
            case CS_City:           { structure_pict_seg = ST_NULL;             } break;
            case CS_Fortress:       { structure_pict_seg = IMG_CMB_Fortress;    } break;
            case CS_Dungeon:        { structure_pict_seg = IMG_CMB_Dungeon;     } break;
            case CS_Tower:          { structure_pict_seg = IMG_CMB_Tower;       } break;
            case CS_Cave:           { structure_pict_seg = IMG_CMB_Cave;        } break;
            case CS_Temple:         { structure_pict_seg = IMG_CMB_Temple;      } break;
            case CS_Fort:           { structure_pict_seg = IMG_CMB_Fort;        } break;
            case CS_SorceryNode:    { structure_pict_seg = ST_NULL;             } break;
            case CS_ChaosNode:      { structure_pict_seg = ST_NULL;             } break;
            case CS_NatureNode:     { structure_pict_seg = IMG_CMB_NatNode;     } break;
            case CS_Ruins:          { structure_pict_seg = IMG_CMB_Ruins;       } break;
        }
        if(structure_pict_seg != ST_NULL)
        {
            if(battlefield->center_square_structure == CS_NatureNode)
            {
                Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)structure_pict_seg, 16, 15, 0, cet_NONE, 0, 0, 0, 0, 0, 0);
            }
            else
            {
                Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)structure_pict_seg, 15, (FLIC_Get_Height(structure_pict_seg) - 21), 0, cet_NONE, 0, 0, 0, 0, 0, 0);
            }
        }
    }
    if(
        (battlefield->center_square_structure == CS_City)
        ||
        (battlefield->center_square_structure == CS_Fortress)
    )
    {
        for(itr = 0; battlefield->house_cnt > itr; itr++)
        {
            Combat_Grid_Screen_Coordinates(battlefield->house_cgxs[itr], battlefield->house_cgys[itr], 0, 0, &screen_x, &screen_y);
            screen_y += 2;
            Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)battlefield->house_pict_segs[itr], (FLIC_Get_Width(battlefield->house_pict_segs[itr]) / 2), (FLIC_Get_Height(battlefield->house_pict_segs[itr]) - 14), 0, cet_NONE, 0, 0, 0, 0, 0, 0);
        }
    }
    if(battlefield->walled == ST_TRUE)              { Spawn_Stone_Wall_Entities(0); }
    if(battlefield->wall_of_darkness == ST_TRUE)    { Spawn_Dark_Wall_Entities(0);  }
    if(battlefield->wall_of_fire == ST_TRUE)        { Spawn_Fire_Wall_Entities(0);  }
    if(battlefield->wall_of_fire == ST_TRUE)        { Spawn_Fire_Wall_Entities(1);  }
    if(battlefield->wall_of_darkness == ST_TRUE)    { Spawn_Dark_Wall_Entities(1);  }
    if(battlefield->walled == ST_TRUE)              { Spawn_Stone_Wall_Entities(1); }
}

// WZD ovr153p08
void Spawn_Stone_Wall_Entities(int16_t flag)
{
    int16_t screen_y = 0;
    int16_t screen_x = 0;
    int16_t itr = 0;
    // north-west corner
    for(itr = 0; ((itr < 1) && (flag == 0)); itr++)
    {
        Combat_Grid_Screen_Coordinates(MIN_CGX_CITY, (MIN_CGY_CITY + itr), 0, 0, &screen_x, &screen_y);
        screen_y += 1;
        screen_x += 1;
        if((_wall_rise_type == 0) && (_wall_rise_on == ST_TRUE))
        {
            Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)_wallrise_seg[itr], 15, 18, _wall_rise_frame, cet_NONE, 0, 0, 0, 0, 0, 0);
        }
        else
        {
            if(battlefield->walls[itr][0] == 1)
            {
                Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)IMG_CMB_StoneWalls[_combat_wall_sprite_bank][itr], (FLIC_Get_Width(IMG_CMB_StoneWalls[_combat_wall_sprite_bank][itr]) / 2), 18, 0, cet_NONE, 0, 0, 0, 0, 0, 0);
            }
            if(battlefield->walls[itr][0] == 2)
            {
                Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)IMG_CMB_StoneWalls[_combat_wall_sprite_bank][itr], (FLIC_Get_Width(IMG_CMB_StoneWalls[_combat_wall_sprite_bank][itr]) / 2), 18, 1, cet_NONE, 0, 0, 0, 0, 0, 0);
            }
        }
    }
    // west
    for(itr = 1; ((itr < 4) && (flag == 0)); itr++)
    {
        Combat_Grid_Screen_Coordinates(MIN_CGX_CITY, (MIN_CGY_CITY + itr), 0, 0, &screen_x, &screen_y);
        screen_x += 1;
        if(itr > 0)
        {
            screen_y += 1;
        }
        if( (_wall_rise_type == 0) && (_wall_rise_on == ST_TRUE) )
        {
            Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)_wallrise_seg[itr], 17, 17, _wall_rise_frame, cet_NONE, 0, 0, 0, 0, 0, 0);
        }
        else
        {
            if(battlefield->walls[itr][0] == 1)  /* flat: itr*4 */
            {
                Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)IMG_CMB_StoneWalls[_combat_wall_sprite_bank][itr], 17, 17, 0, 0, 0, 0, 0, 0, 0, 0);
            }
            if(battlefield->walls[itr][0] == 2)  /* flat: itr*4 */
            {
                Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)IMG_CMB_StoneWalls[_combat_wall_sprite_bank][itr], 17, 17, 1, 0, 0, 0, 0, 0, 0, 0);
            }
        }
    }
    // south
    for(itr = 4; ((itr < 7) && (flag == 0)); itr++)
    {
        Combat_Grid_Screen_Coordinates((2 + itr), MIN_CGY_CITY, 0, 0, &screen_x, &screen_y);
        screen_x += 1;
        if((_wall_rise_type == 0) && (_wall_rise_on == ST_TRUE))
        {
            Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)_wallrise_seg[itr], 17, 17, _wall_rise_frame, cet_NONE, 0, 0, 0, 0, 0, 0);
        }
        else
        {
            if(battlefield->walls[0][(itr - 3)] == 1)
            {
                Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)IMG_CMB_StoneWalls[_combat_wall_sprite_bank][itr], 17, 17, 0, 0, 0, 0, 0, 0, 0, 0);
            }
            if(battlefield->walls[0][(itr - 3)] == 2)
            {
                Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)IMG_CMB_StoneWalls[_combat_wall_sprite_bank][itr], 17, 17, 1, 0, 0, 0, 0, 0, 0, 0);
            }
        }
    }
    // north
    for(itr = 7; ((itr < 10) && (flag == 1)); itr++)
    {
        Combat_Grid_Screen_Coordinates((itr - 1), MAX_CGY_CITY, 0, 0, &screen_x, &screen_y);
        screen_y += 14;
        if((_wall_rise_type == 0) && (_wall_rise_on == ST_TRUE))
        {
            Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)_wallrise_seg[itr], 16, 30, _wall_rise_frame, cet_NONE, 0, 0, 0, 0, 0, 0);
        }
        else
        {
            if(battlefield->walls[0][(itr - 6)] == 1)  /* flat: itr-6 */
            {
                Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)IMG_CMB_StoneWalls[_combat_wall_sprite_bank][itr], 16, 30, 0, 0, 0, 0, 0, 0, 0, 0);
            }
            if(battlefield->walls[0][(itr - 6)] == 2)  /* flat: itr-6 */
            {
                Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)IMG_CMB_StoneWalls[_combat_wall_sprite_bank][itr], 16, 30, 1, 0, 0, 0, 0, 0, 0, 0);
            }
        }
    }
    // east
    for(itr = MIN_CGY_CITY; ((itr < (MAX_CGY_CITY - 1)) && (flag == 1)); itr++)
    {
        Combat_Grid_Screen_Coordinates(MAX_CGX_CITY, (itr + 1), 0, 0, &screen_x, &screen_y);
        screen_y += 14;
        if((_wall_rise_type == 0) && (_wall_rise_on == ST_TRUE))
        {
            Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)_wallrise_seg[itr], 16, 30, _wall_rise_frame, cet_NONE, 0, 0, 0, 0, 0, 0);
        }
        else
        {
            if(battlefield->walls[0][(itr - 9)] == 1)  /* flat: itr-9 */
            {
                Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)IMG_CMB_StoneWalls[_combat_wall_sprite_bank][itr], 16, 30, 0, 0, 0, 0, 0, 0, 0, 0);
            }
            if(battlefield->walls[0][(itr - 9)] == 2)  /* flat: itr-9 */
            {
                Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)IMG_CMB_StoneWalls[_combat_wall_sprite_bank][itr], 16, 30, 1, 0, 0, 0, 0, 0, 0, 0);
            }
        }
    }
}


// WZD ovr153p09
void Spawn_Dark_Wall_Entities(int16_t flag)
{
    int16_t screen_y = 0;
    int16_t screen_x = 0;
    int16_t itr = 0;
    // north-west corner
    for(itr = 0; ((itr < 1) && (flag == 0)); itr++)
    {
        Combat_Grid_Screen_Coordinates(MIN_CGX_CITY, (MIN_CGY_CITY + itr), 0, 0, &screen_x, &screen_y);
        screen_y += 1;
        screen_x += 1;
        if(
            (_wall_rise_type == 2)
            &&
            (_wall_rise_on == ST_TRUE)
        )
        {
            Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)_wallrise_seg[itr], 15, 19, _wall_rise_frame, cet_NONE, 0, 0, 0, 0, 0, 0);
        }
        else
        {
            Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)IMG_CMB_DarkWall[itr], (FLIC_Get_Width(IMG_CMB_DarkWall[itr]) / 2), 19, _combat_magic_wall_anim_frame, cet_NONE, 0, 0, 0, 0, 0, 0);
        }
    }
    // west
    for(itr = 1; ((itr < 4) && (flag == 0)); itr++)
    {
        Combat_Grid_Screen_Coordinates(MIN_CGX_CITY, (MIN_CGY_CITY + itr), 0, 0, &screen_x, &screen_y);
        screen_x += 1;
        if(itr > 0)
        {
            screen_y += 1;
        }
        if(
            (_wall_rise_type == 2)
            &&
            (_wall_rise_on == ST_TRUE)
        )
        {
            Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)_wallrise_seg[itr], 17, 18, _wall_rise_frame, cet_NONE, 0, 0, 0, 0, 0, 0);
        }
        else
        {
            Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)IMG_CMB_DarkWall[itr], 17, 18, _combat_magic_wall_anim_frame, cet_NONE, 0, 0, 0, 0, 0, 0);
            
        }
    }
    // south
    for(itr = 4; ((itr < 7) && (flag == 0)); itr++)
    {
        Combat_Grid_Screen_Coordinates((2 + itr), MIN_CGY_CITY, 0, 0, &screen_x, &screen_y);
        screen_x += 1;
        if(
            (_wall_rise_type == 2)
            &&
            (_wall_rise_on == ST_TRUE)
        )
        {
            Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)_wallrise_seg[itr], 17, 17, _wall_rise_frame, cet_NONE, 0, 0, 0, 0, 0, 0);
        }
        else
        {
            Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)IMG_CMB_DarkWall[itr], 17, 17, _combat_magic_wall_anim_frame, cet_NONE, 0, 0, 0, 0, 0, 0);
        }
    }
    // north
    for(itr = 7; ((itr < 10) && (flag == 1)); itr++)
    {
        Combat_Grid_Screen_Coordinates((itr - 1), MAX_CGY_CITY, 0, 0, &screen_x, &screen_y);
        screen_y += 14;
        if(
            (_wall_rise_type == 2)
            &&
            (_wall_rise_on == ST_TRUE)
        )
        {
            Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)_wallrise_seg[itr], 16, 29, _wall_rise_frame, cet_NONE, 0, 0, 0, 0, 0, 0);
        }
        else
        {
            Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)IMG_CMB_DarkWall[itr], 16, 29, _combat_magic_wall_anim_frame, cet_NONE, 0, 0, 0, 0, 0, 0);
        }
    }
    // east
    for(itr = 10; ((itr < 12) && (flag == 1)); itr++)
    {
        Combat_Grid_Screen_Coordinates(MAX_CGX_CITY, (itr + 1), 0, 0, &screen_x, &screen_y);
        screen_y += 14;
        if(
            (_wall_rise_type == 2)
            &&
            (_wall_rise_on == ST_TRUE)
        )
        {
            Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)_wallrise_seg[itr], 16, 29, _wall_rise_frame, cet_NONE, 0, 0, 0, 0, 0, 0);
        }
        else
        {
            Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)IMG_CMB_DarkWall[itr], 16, 29, _combat_magic_wall_anim_frame, cet_NONE, 0, 0, 0, 0, 0, 0);
        }
    }
    // 
    for(itr = 12; ((itr < 13) && (flag == 1)); itr++)
    {
        Combat_Grid_Screen_Coordinates(MIN_CGX_CITY, MAX_CGY_CITY, 0, 0, &screen_x, &screen_y);
        screen_y += 14;
        if(
            (_wall_rise_type == 2)
            &&
            (_wall_rise_on == ST_TRUE)
        )
        {
            Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)_wallrise_seg[itr], 16, 30, _wall_rise_frame, cet_NONE, 0, 0, 0, 0, 0, 0);
        }
        else
        {
            Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)IMG_CMB_DarkWall[itr], 16, 30, _combat_magic_wall_anim_frame, cet_NONE, 0, 0, 0, 0, 0, 0);
        }
    }
    // south-west corner  {8,10}
    for(itr = 13; ((itr < 14) && (flag == 1)); itr++)
    {
        Combat_Grid_Screen_Coordinates(MAX_CGX_CITY, MIN_CGY_CITY, 0, 0, &screen_x, &screen_y);
        screen_y += 14;
        if(
            (_wall_rise_type == 2)
            &&
            (_wall_rise_on == ST_TRUE)
        )
        {
            Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)_wallrise_seg[itr], 16, 30, _wall_rise_frame, cet_NONE, 0, 0, 0, 0, 0, 0);
        }
        else
        {
            Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)IMG_CMB_DarkWall[itr], 16, 30, _combat_magic_wall_anim_frame, cet_NONE, 0, 0, 0, 0, 0, 0);
        }
    }
}


// WZD ovr153p10
void Spawn_Fire_Wall_Entities(int16_t flag)
{
    int16_t screen_y = 0;
    int16_t screen_x = 0;
    int16_t itr = 0;
    // north-west corner
    for(itr = 0; ((itr < 1) && (flag == 0)); itr++)
    {
        Combat_Grid_Screen_Coordinates(MIN_CGX_CITY, (MIN_CGY_CITY + itr), 0, 0, &screen_x, &screen_y);
        screen_y += 1;
        screen_x += 1;
        if(
            (_wall_rise_type == 1)
            &&
            (_wall_rise_on == ST_TRUE)
        )
        {
            Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)_wallrise_seg[itr], 15, 19, _wall_rise_frame, cet_NONE, 0, 0, 0, 0, 0, 0);
        }
        else
        {
            Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)IMG_CMB_FireWall[itr], (FLIC_Get_Width(IMG_CMB_FireWall[itr]) / 2), 19, _combat_magic_wall_anim_frame, cet_NONE, 0, 0, 0, 0, 0, 0);
        }
    }
    // west
    for(itr = 1; ((itr < 4) && (flag == 0)); itr++)
    {
        Combat_Grid_Screen_Coordinates(MIN_CGX_CITY, (MIN_CGY_CITY + itr), 0, 0, &screen_x, &screen_y);
        screen_x += 1;
        if(itr > 0)
        {
            screen_y += 1;
        }
        if(
            (_wall_rise_type == 1)
            &&
            (_wall_rise_on == ST_TRUE)
        )
        {
            Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)_wallrise_seg[itr], 17, 18, _wall_rise_frame, cet_NONE, 0, 0, 0, 0, 0, 0);
        }
        else
        {
            Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)IMG_CMB_FireWall[itr], 17, 18, _combat_magic_wall_anim_frame, cet_NONE, 0, 0, 0, 0, 0, 0);
        }
    }
    // south
    for(itr = 4; ((itr < 7) && (flag == 0)); itr++)
    {
        Combat_Grid_Screen_Coordinates((2 + itr), MIN_CGY_CITY, 0, 0, &screen_x, &screen_y);
        screen_x += 1;
        if(
            (_wall_rise_type == 1)
            &&
            (_wall_rise_on == ST_TRUE)
        )
        {
            Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)_wallrise_seg[itr], 17, 17, _wall_rise_frame, cet_NONE, 0, 0, 0, 0, 0, 0);
        }
        else
        {
            Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)IMG_CMB_FireWall[itr], 17, 17, _combat_magic_wall_anim_frame, cet_NONE, 0, 0, 0, 0, 0, 0);
        }
    }
    // north
    for(itr = 7; ((itr < 10) && (flag == 1)); itr++)
    {
        Combat_Grid_Screen_Coordinates((itr - 1), MAX_CGY_CITY, 0, 0, &screen_x, &screen_y);
        screen_y += 14;
        if(
            (_wall_rise_type == 1)
            &&
            (_wall_rise_on == ST_TRUE)
        )
        {
            Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)_wallrise_seg[itr], 16, 29, _wall_rise_frame, cet_NONE, 0, 0, 0, 0, 0, 0);
        }
        else
        {
            Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)IMG_CMB_FireWall[itr], 16, 29, _combat_magic_wall_anim_frame, cet_NONE, 0, 0, 0, 0, 0, 0);
        }
    }
    // east {{8,11},{8,12}}
    for(itr = 10; ((itr < 12) && (flag == 1)); itr++)
    {
        Combat_Grid_Screen_Coordinates(MAX_CGX_CITY, (1 + itr), 0, 0, &screen_x, &screen_y);
        screen_y += 14;
        if(
            (_wall_rise_type == 1)
            &&
            (_wall_rise_on == ST_TRUE)
        )
        {
            Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)_wallrise_seg[itr], 16, 29, _wall_rise_frame, cet_NONE, 0, 0, 0, 0, 0, 0);
        }
        else
        {
            Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)IMG_CMB_FireWall[itr], 16, 29, _combat_magic_wall_anim_frame, cet_NONE, 0, 0, 0, 0, 0, 0);
        }
    }
    // south-west corner  {5,13}
    for(itr = 12; ((itr < 13) && (flag == 1)); itr++)
    {
        Combat_Grid_Screen_Coordinates(MIN_CGX_CITY, MAX_CGY_CITY, 0, 0, &screen_x, &screen_y);
        screen_y += 14;
        if(
            (_wall_rise_type == 1)
            &&
            (_wall_rise_on == ST_TRUE)
        )
        {
            Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)_wallrise_seg[itr], 16, 30, _wall_rise_frame, cet_NONE, 0, 0, 0, 0, 0, 0);
        }
        else
        {
            Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)IMG_CMB_FireWall[itr], 16, 30, _combat_magic_wall_anim_frame, cet_NONE, 0, 0, 0, 0, 0, 0);
        }
    }
    // south-west corner  {8,10}
    for(itr = 13; ((itr < 14) && (flag == 1)); itr++)
    {
        Combat_Grid_Screen_Coordinates(8, 10, 0, 0, &screen_x, &screen_y);
        screen_y += 14;
        if(
            (_wall_rise_type == 1)
            &&
            (_wall_rise_on == ST_TRUE)
        )
        {
            Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)_wallrise_seg[itr], 16, 30, _wall_rise_frame, cet_NONE, 0, 0, 0, 0, 0, 0);
        }
        else
        {
            Combat_Grid_Entity_Create(screen_x, screen_y, (int64_t)IMG_CMB_FireWall[itr], 16, 30, _combat_magic_wall_anim_frame, cet_NONE, 0, 0, 0, 0, 0, 0);
        }
    }
}


// WZD ovr153p11
void Spawn_Vortex_Entities(void)
{
    int16_t move_screen_y = 0;
    int16_t screen_y = 0;
    int16_t move_screen_x = 0;
    int16_t screen_x = 0;
    int16_t draw_y = 0;
    int16_t draw_x = 0;
    int16_t vortex_idx = 0;
    for(vortex_idx = 0; vortex_idx < _vortex_count; vortex_idx++)
    {
        Combat_Grid_Screen_Coordinates(_vortexes[vortex_idx].cgx, _vortexes[vortex_idx].cgy, 0, 0, &screen_x, &screen_y);
        Combat_Grid_Screen_Coordinates(_vortexes[vortex_idx].move_cgx, _vortexes[vortex_idx].move_cgy, 0, 0, &move_screen_x, &move_screen_y);
        screen_y += 8;
        move_screen_y += 8;
        draw_x = (screen_x + (((move_screen_x - screen_x) * _vortexes[vortex_idx].stage) / 8));
        draw_y = (screen_y + (((move_screen_y - screen_y) * _vortexes[vortex_idx].stage) / 8));
        Combat_Grid_Entity_Create(
            draw_x,
            draw_y,
            (int64_t)magic_vortex_seg,
            13,
            25,
            ((_combat_vortex_anim_frame + vortex_idx) % FLIC_Get_FrameCount(magic_vortex_seg)),
            cet_NONE,
            0,
            0,
            0,
            0,
            0,
            0
        );
    }
}


// WZD ovr153p12
/*
~ Battle_Unit_Figure_Position()
~ UnitView.C  Unit_Figure_Position()
*/
void Spawn_Figure_Entity(int64_t bufpi, int16_t cgx, int16_t cgy, int16_t target_cgx, int16_t target_cgy, int16_t move_anim_ctr, int16_t current_figure, int16_t figure_count, int16_t controller_idx, int16_t outline_magic_realm, int16_t gibs, int16_t frame_num, int16_t figures_lost, int16_t gibs_bank)
{
    int16_t gibs_frame = 0;
    int16_t figure_set_idx = 0;
    int16_t prev_gibs_frame = 0;
    int16_t fig_y = 0;
    int16_t draw_y = 0;
    int16_t dst_screen_y = 0;
    int16_t src_screen_y = 0;
    int16_t draw_x = 0;
    int16_t dst_screen_x = 0;
    int16_t src_screen_x = 0;
    int16_t fig_x = 0;
    Combat_Grid_Screen_Coordinates(cgx, cgy, 0, 0, &src_screen_x, &src_screen_y);
    Combat_Grid_Screen_Coordinates(target_cgx, target_cgy, 0, 0, &dst_screen_x, &dst_screen_y);
    /*
        BEGIN:  ~== Unit_Figure_Position()
    */
    BATTLE_UNIT_FIGURE_POSITION
    /*
        END:  ~== Unit_Figure_Position()
    */
    BATTLE_UNIT_FACING_DRECTION
    draw_x = (((((dst_screen_x - src_screen_x) * move_anim_ctr) / MOVE_ANIM_CNT) + src_screen_x) + fig_x);
    draw_y = (((((dst_screen_y - src_screen_y) * move_anim_ctr) / MOVE_ANIM_CNT) + src_screen_y) + fig_y);
    /*
        BEGIN:  gibs
    */
    if(
        (gibs > 0)
        &&
        (current_figure < figures_lost)
    )
    {
        if(gibs_bank == 0)
        {
            prev_gibs_frame = m_gibs_frames[current_figure];
            m_gibs_frames[current_figure] += 1;
            if(m_gibs_frames[current_figure] > 5)
            {
                m_gibs_frames[current_figure] = 5;
            }
        }
        else
        {
            prev_gibs_frame = m_gibs_frames[(10 + current_figure)];
            m_gibs_frames[(10 + current_figure)] += 1;
            if(m_gibs_frames[(10 + current_figure)] > 5)
            {
                m_gibs_frames[(10 + current_figure)] = 5;
            }
        }
        prev_gibs_frame -= 2;
        gibs_frame = prev_gibs_frame;
        if(
            ((figures_lost - 1) == current_figure)
            &&
            (gibs_frame == 3)
        )
        {
            gibs_frame = 2;
        }
    }
    else
    {
        gibs_frame = 0;
        gibs = 0;
    }
    /*
        END:  gibs
    */
    Combat_Grid_Entity_Create(
        draw_x,
        draw_y,
        bufpi,
        13,
        23,
        frame_num,
        cet_Battle_Unit_Figure,
        controller_idx,
        figure_set_idx,
        outline_magic_realm,
        gibs,
        frame_num,
        gibs_frame
    );
}


// WZD ovr153p13
void Spawn_Curse_Entity(int16_t cgx, int16_t cgy, int16_t target_cgx, int16_t target_cgy, int16_t move_anim_ctr, int16_t combat_curse_entity_idx)
{
    int16_t draw_y = 0;
    int16_t dst_screen_y = 0;
    int16_t screen_y = 0;
    int16_t draw_x = 0;
    int16_t dst_screen_x = 0;
    int16_t screen_x = 0;
    Combat_Grid_Screen_Coordinates(cgx, cgy, 0, 0, &screen_x, &screen_y);
    Combat_Grid_Screen_Coordinates(target_cgx, target_cgy, 0, 0, &dst_screen_x, &dst_screen_y);
    draw_x = screen_x + ((dst_screen_x - screen_x) * move_anim_ctr) / 8;
    draw_y = screen_y + ((dst_screen_y - screen_y) * move_anim_ctr) / 8;
    Combat_Grid_Entity_Create(
        draw_x,
        draw_y,
        (int64_t)combat_curse_entity_seg[combat_curse_entity_idx],
        13,
        15,
        _combat_curse_anim_frame,
        cet_Curse,
        0,
        0,
        0,
        0,
        0,
        0
    );
}


// WZD ovr153p14
void Clear_Combat_Grid_Entities(void)
{
    int16_t itr = 0;
    combat_grid_entity_count = 0;
    for(itr = 0; itr < MAX_ENTITIES; itr++)
    {
        combat_grid_entities_draw_order[itr] = (uint8_t)itr;
    }
}


// WZD ovr153p15
/*
¿ ~ UnitView.C Draw_Unit_Figure() ?
*/
void Combat_Figure_Compose(int16_t bufpi, int16_t figure_set_idx, int16_t player_idx, int16_t enchantment_magic_realm, int16_t frame_num)
{
    SAMB_ptr * figure_pict_set_seg = 0;
    int16_t offset = 0;
    FIGUREX_OFFSET
    FIGUREX_MAP
    FIGUREX_POINTER
    Set_Animation_Frame(figure_pict_set_seg[figure_set_idx], frame_num);
    Draw_Picture_To_Bitmap(figure_pict_set_seg[figure_set_idx], scratch_bitmap_seg);
    Combat_Figure_Banner_Color(player_idx);
    Combat_Unit_Enchantment_Outline_Draw(enchantment_magic_realm);
}


// WZD ovr153p16
void Spawn_Missile_Entities(void)
{
    int16_t arc_offset_y = 0;
    int16_t missile_screen_y = 0;
    int16_t missile_screen_x = 0;
    int16_t itr_msl = 0;
    for(itr_msl = 0; itr_msl < m_missile_count; itr_msl++)
    {
        missile_screen_x = (_missiles[itr_msl].src_screen_x + (((int32_t)(_missiles[itr_msl].dst_screen_x - _missiles[itr_msl].src_screen_x) * _missiles[itr_msl].travel_percent) / 100));
        missile_screen_y = (_missiles[itr_msl].src_screen_y + (((int32_t)(_missiles[itr_msl].dst_screen_y - _missiles[itr_msl].src_screen_y) * _missiles[itr_msl].travel_percent) / 100));
        if(
            (_missiles[itr_msl].type == msl_Arrow)
            ||
            (_missiles[itr_msl].type == msl_Rocks)
            ||
            (_missiles[itr_msl].type == msl_Sling)
        )
        {
            arc_offset_y = (((_missiles[itr_msl].travel_percent - 50) / 2) / 2);
        }
        else
        {
            arc_offset_y = 0;
        }
        if(arc_offset_y < 0)
        {
            arc_offset_y = -(arc_offset_y);
        }
        Combat_Grid_Entity_Create(
            missile_screen_x,
            (missile_screen_y + arc_offset_y),
            (int64_t)CMB_RangedAtx_GFX[_missiles[itr_msl].type][_missiles[itr_msl].direction],
            14,
            22,
            0,
            cet_Missiles,
            0,
            0,
            0,
            0,
            0,
            0);
    }
}


// WZD ovr153p17
/*
XREF:
    Battle_Unit_Compose_Bitmap()
    Spawn_Missile_Entities()

!!!!! ALL DIFFERENT !!!!!
¿ in-line in Combat_Grid_Entities() ?
~ UnitView.C  Unit_Figure_Position()

*/
void Battle_Unit_Figure_Position(int16_t figure_count, int16_t current_figure, int16_t * figure_x, int16_t * figure_y)
{
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
        case 2:
            if(current_figure == 0) {
                *figure_x = 0;
                *figure_y = 4;
            }
            else if(current_figure == 1) {
                *figure_x = -6;
                *figure_y = 10;
            }
            else {
                *figure_x = 7;
                *figure_y = 10;
            }
            break;
        case 3:
            if(current_figure == 0) {
                *figure_x = 1;
                *figure_y = 4;
            }
            else if(current_figure == 1) {
                *figure_x = 8;
                *figure_y = 8;
            }
            else if(current_figure == 2) {
                *figure_x = 1;
                *figure_y = 12;
            }
            else {
                *figure_x = -7;
                *figure_y = 8;
            }
            break;
        case 4:
            if(current_figure == 0) {
                *figure_x = 1;
                *figure_y = 4;
            }
            else if(current_figure == 1) {
                *figure_x = 8;
                *figure_y = 8;
            }
            else if(current_figure == 2) {
                *figure_x = 1;
                *figure_y = 12;
            }
            else if(current_figure == 3) {
                *figure_x = -7;
                *figure_y = 8;
            }
            else {
                *figure_x = 1;
                *figure_y = 8;
            }
            break;
        case 5:
            if(current_figure == 0) {
                *figure_x = 1;
                *figure_y = 4;
            }
            else if(current_figure == 1) {
                *figure_x = 9;
                *figure_y = 8;
            }
            else if(current_figure == 2) {
                *figure_x = -8;
                *figure_y = 8;
            }
            else if(current_figure == 3) {
                *figure_x = 1;
                *figure_y = 12;
            }
            else if(current_figure == 4) {
                *figure_x = -3;
                *figure_y = 9;
            }
            else {
                *figure_x = 4;
                *figure_y = 7;
            }
            break;
        case 6:
            if(current_figure == 0) {
                *figure_x = 1;
                *figure_y = 4;
            }
            else if(current_figure == 1) {
                *figure_x = 10;
                *figure_y = 8;
            }
            else if(current_figure == 2) {
                *figure_x = -8;
                *figure_y = 8;
            }
            else if(current_figure == 3) {
                *figure_x = 1;
                *figure_y = 12;
            }
            else if(current_figure == 4) {
                *figure_x = 6;
                *figure_y = 6;
            }
            else if(current_figure == 5) {
                *figure_x = -3;
                *figure_y = 11;
            }
            else {
                *figure_x = 1;
                *figure_y = 8;
            }
            break;
        case 7:
            if(current_figure == 0) {
                *figure_x = 1;
                *figure_y = 4;
            }
            else if(current_figure == 1) {
                *figure_x = 10;
                *figure_y = 8;
            }
            else if(current_figure == 2) {
                *figure_x = -8;
                *figure_y = 8;
            }
            else if(current_figure == 3) {
                *figure_x = 2;
                *figure_y = 12;
            }
            else if(current_figure == 4) {
                *figure_x = 6;
                *figure_y = 6;
            }
            else if(current_figure == 5) {
                *figure_x = -3;
                *figure_y = 12;
            }
            else if(current_figure == 6) {
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
/*
draw type can be:
;   0 - normal frame selection
;   1 - precomposed BU figure (IMG_Loc is an index)
;   2 - second-pass variable-frame (out of 8)
;   3 - second-pass fixed frame (GUI_ProjectileFrame)
;   4 - simple single frame from EMM TILEX
*/
/*

seg_or_idx
    Spawn_Figure_Entity() passes battle_units[itr].bufpi

figure_set_idx
    not in use

Spawn_Figure_Entity()
    Combat_Grid_Entity_Create(draw_x, draw_y, seg_or_idx, 13, 23, UU, 1, controller_idx, figure_set_idx, outline_magic_realm, gibs, UU, Blood_Frame);

*/
void Combat_Grid_Entity_Create(int16_t draw_x, int16_t draw_y, int64_t seg_or_idx, int16_t draw_x_shift, int16_t draw_y_shift, int16_t Frame, int16_t entity_type, int16_t controller_idx, int16_t niu_figure_set_idx, int16_t outline_magic_realm, int16_t gibs, int16_t UU_14h, int16_t BldFrm)
{
    int16_t combat_grid_cell_y_offset = 0;
    int16_t combat_grid_cell_x_offset = 0;
    int16_t combat_grid_cell_y = 0;
    int16_t combat_grid_cell_x = 0;
    if(combat_grid_entity_count > MAX_ENTITIES)
    {
        Exit_With_Message(str_E1);  /* "E1" */
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
    combat_grid_entities[combat_grid_entity_count].gibs = gibs;
    Screen_To_Combat_Grid_Cell_X_And_Offset(draw_x, draw_y, &combat_grid_cell_x, &combat_grid_cell_x_offset);
    Screen_To_Combat_Grid_Cell_Y_And_Offset(draw_x, draw_y, &combat_grid_cell_y, &combat_grid_cell_y_offset);
    combat_grid_entities[combat_grid_entity_count].draw_order_value = (((uint32_t)combat_grid_cell_y * 8000) + ((uint32_t)combat_grid_cell_x * 320) + ((uint32_t)combat_grid_cell_y_offset * 16) + combat_grid_cell_x_offset);
    Set_Entity_Draw_Order();
    combat_grid_entity_count++;
}


// WZD ovr153p19
void Set_Entity_Draw_Order(void)
{
    uint32_t draw_order_value = 0;
    int16_t itr_up = 0;
    int16_t itr_dn = 0;
    int16_t entity_idx = 0;
    if(combat_grid_entity_count == 0)
    {
        return;
    }
    draw_order_value = combat_grid_entities[combat_grid_entity_count].draw_order_value;
    for(itr_up = 0; itr_up < combat_grid_entity_count; itr_up++)
    {
        if(combat_grid_entities[combat_grid_entities_draw_order[itr_up]].draw_order_value >= draw_order_value)
        {
            break;
        }
    }
    if(itr_up == combat_grid_entity_count)
    {
        return;
    }
    entity_idx = itr_up;
    for(itr_dn = combat_grid_entity_count; itr_dn >= entity_idx; itr_dn--)  /* OGBUG: OOB one iteration too many */
    {
        combat_grid_entities_draw_order[itr_dn] = combat_grid_entities_draw_order[(itr_dn - 1)];
    }
    combat_grid_entities_draw_order[entity_idx] = (uint8_t)combat_grid_entity_count;  // combat_grid_entity_count is incremented in Combat_Grid_Entity_Create()
}


// WZD ovr153p20
void Claim_EMS_Page_For_Figure_Set(int16_t bufpi)
{
    int16_t logical_page = 0;
    int16_t offset = 0;
    logical_page = ((bufpi * 3) / 2);
    FIGUREX_OFFSET
    EMS_PFBA = (EmmHndl_FIGUREX + (logical_page * SZ_EMM_LOGICAL_PAGE));  // TODO  EMM_MapMulti4(logical_page, EmmHndl_FIGUREX);
    SET_2B_OFS((EMS_PFBA + offset), SAMB_MEMSIG1, _SA_MEMSIG1);
    SET_2B_OFS((EMS_PFBA + offset), SAMB_MEMSIG2, _SA_MEMSIG2);
    SET_2B_OFS((EMS_PFBA + offset), SAMB_SIZE, 1591);  // 1591 PR, 25456 B
    SET_2B_OFS((EMS_PFBA + offset), SAMB_USED, 1);
}


// WZD ovr153p21
void Map_Tile_EMS_Page_As_Sandbox(void)
{
    EMS_PFBA = (EmmHndl_TILEXXX + (0 * SZ_EMM_LOGICAL_PAGE));  // TODO  EMM_MapMulti4(0, EmmHndl_TILEXXX);
    SET_2B_OFS(EMS_PFBA, SAMB_MEMSIG1, _SA_MEMSIG1);
    SET_2B_OFS(EMS_PFBA, SAMB_MEMSIG2, _SA_MEMSIG2);
    SET_2B_OFS(EMS_PFBA, SAMB_SIZE, 3071);  // 3071 PR, 49136 B
    SET_2B_OFS(EMS_PFBA, SAMB_USED, 1);
}


// WZD ovr153p22
void o153p22_empty_function(void)
{
// push    bp
// mov     bp, sp
// pop     bp
// retf
}


// WZD ovr153p23
void Combat_Screen_Map_Compose_Figures(void)
{
    SAMB_ptr temp_GfxBuf_2400B = 0;
    int16_t figure_effect = 0;
    int16_t frame_num = 0;
    int16_t outline_magic_realm = 0;
    int16_t player_idx = 0;
    int16_t bufpi = 0;
    SAMB_ptr * figure_pict_set_seg = 0;
    int16_t offset = 0;
    int16_t target_cgy = 0;
    int16_t cgy = 0;
    int16_t target_cgx = 0;
    int16_t cgx = 0;
    int16_t battle_unit_idx = 0;
    int16_t figure_set_idx = 0;
    temp_GfxBuf_2400B = scratch_bitmap_seg;
    for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
    {
        if(battle_units[battle_unit_idx].status != bus_Active)
        {
            continue;
        }
        bufpi = battle_units[battle_unit_idx].bufpi;
        player_idx = battle_units[battle_unit_idx].controller_idx;
        outline_magic_realm = battle_units[battle_unit_idx].outline_magic_realm;
        figure_effect = battle_units[battle_unit_idx].figure_effect;
        /*
            BEGIN:  frame_num
        */
        {
            if(battle_units[battle_unit_idx].animate_idle == ST_TRUE)
            {
                frame_num = combat_idle_anim_frame;
            }
            else
            {
                if(battle_units[battle_unit_idx].mid_move == ST_TRUE)
                {
                    if(battle_units[battle_unit_idx].animate_move_as_idle == ST_TRUE)
                    {
                        frame_num = combat_idle_anim_frame;
                    }
                    else
                    {
                        frame_num = combat_walk_anim_frame;
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
                    frame_num = _combat_unit_attack_anim_frame;
                }
                else
                {
                    frame_num = _combat_unit_defend_anim_frame;
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
        Set_Animation_Frame(figure_pict_set_seg[figure_set_idx], frame_num);
        scratch_bitmap_seg = battle_unit_picts_seg[battle_units[battle_unit_idx].bufpi];
        Draw_Picture_To_Bitmap(figure_pict_set_seg[figure_set_idx], scratch_bitmap_seg);
        Combat_Figure_Banner_Color(player_idx);
        Combat_Unit_Enchantment_Outline_Draw(outline_magic_realm);
        Combat_Figure_Effect(figure_effect);
        Combat_Figure_Active_Red_Outline(battle_unit_idx);
    }
    scratch_bitmap_seg = temp_GfxBuf_2400B;
}


// WZD ovr153p24
void o153p24_empty_function(void)
{
// push    bp
// mov     bp, sp
// pop     bp
// retf
}



/*
    WIZARDS.EXE  ovr154
*/

// WZD ovr154p01
/**
 * @brief Initialize battlefield state and build the tactical combat map for a world square.
 *
 * Resolves the combat location at the provided world coordinates (open field, outpost, city,
 * fortress, tower, node, or lair), gathers city-specific combat metadata when applicable, derives
 * neighborhood road/river context, selects the combat terrain set, and then generates and composes
 * the combat battlefield visuals.
 *
 * @param wx World map X coordinate of the combat square.
 * @param wy World map Y coordinate of the combat square.
 * @param wp World plane index of the combat square.
 *
 * @return This function does not return a value.
 *
 * @note Updates global combat/battlefield state, including battlefield fields, wall sprite bank,
 *       vortex count reset, and composed background buffers.
 * @note Temporarily replaces the global RNG seed with a deterministic combat-map seed derived from
 *       square coordinates, then restores the prior seed before returning.
 * @note Passes several derived flags into Generate_Combat_Map(), including magical wall bits,
 *       city wall presence, and floating-island battle context.
 */
void Build_Battlefield(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t neighborhood_river_flags[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t neighborhood_road_flags[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int8_t * enchantments_ptr2 = 0;
    int8_t * enchantments_ptr1 = 0;
    int16_t magic_wall_flags = 0;
    int16_t city_has_stone_walls = 0;
    int16_t niu_fortress_found = 0;
    int16_t city_has_flying_fortress = 0;
    /* combat map terrain set/core/base */
    int16_t combat_terrain_set = 0;
    int16_t roads_enchanted = 0;
    int16_t combat_house_style = 0;
    int16_t city_population = 0;
    int16_t combat_location_type = 0;
    uint32_t random_seed = 0;
    int16_t itr_wy = 0;
    int16_t itr_wx = 0;
    int16_t fortress_player_idx = 0;
    int16_t itr = 0;
    _wall_rise_on = 0;  // ; set to 0 at the beginning of combat map creation; 1 branches wall entity creation (unknown IMG array)
    magic_wall_flags = 0;
    enchantments_ptr1 = &battlefield->city_enchantments[0];
    /* OGBUG: clears 30 instead of 26 - NUM_CITY_ENCHANTMENTS */
    for(itr = 0; itr < 30; itr++)
    {
        enchantments_ptr1[itr] = 0;
    }
    combat_location_type = clt_OpenField;
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
            city_population = _CITIES[itr].population;
            if(city_population == 0)
            {
                combat_location_type = clt_Outpost;
            }
            else
            {
                combat_location_type = clt_City;
                for(fortress_player_idx = 0; fortress_player_idx < _num_players; fortress_player_idx++)
                {
                    if(
                        (_FORTRESSES[fortress_player_idx].active == ST_TRUE)
                        &&
                        (_FORTRESSES[fortress_player_idx].wx == wx)
                        &&
                        (_FORTRESSES[fortress_player_idx].wy == wy)
                        &&
                        (_FORTRESSES[fortress_player_idx].wp == wp)
                    )
                    {
                        combat_location_type = clt_Fortress;
                        niu_fortress_found = ST_TRUE;
                    }
                }
                combat_house_style = _race_type_table[_CITIES[itr].race].house_type;
                if(combat_house_style == rht_Tree)
                {
                    combat_house_style = rht_Mound;
                }
                else if(combat_house_style == rht_Mound)
                {
                    combat_house_style = rht_Tree;
                }
                if(_CITIES[itr].enchantments[FLYING_FORTRESS] != 0)
                {
                    city_has_flying_fortress = ST_TRUE;
                }
                else
                {
                    city_has_flying_fortress = ST_FALSE;
                }
                magic_wall_flags = 0;
                if(_CITIES[itr].enchantments[WALL_OF_FIRE] != 0)
                {
                    magic_wall_flags = 1;  // 00000001b
                }
                if(_CITIES[itr].enchantments[WALL_OF_DARKNESS] != 0)
                {
                    magic_wall_flags += 2;  // 00000010b
                }
                if(_CITIES[itr].bldg_status[CITY_WALLS] >= bs_Replaced)
                {
                    city_has_stone_walls = ST_TRUE;
                }
                else
                {
                    city_has_stone_walls = ST_FALSE;
                }
            }
            enchantments_ptr1 = &_CITIES[itr].enchantments[0];
            enchantments_ptr2 = &battlefield->city_enchantments[0];
            while(enchantments_ptr1 <= (int8_t *)&_CITIES[itr].enchantments[NIGHTSHADE])
            {
                *enchantments_ptr2 = *enchantments_ptr1;
                enchantments_ptr1++;
                enchantments_ptr2++;
            }
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
            combat_location_type = clt_Tower;
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
            combat_location_type = (clt_SorceryNode - _NODES[itr].type);  // {clt_SorceryNode, clt_NatureNode, clt_ChaosNode}
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
            (_LAIRS[itr].intact == ST_TRUE)
        )
        {
            combat_location_type = (clt_Tower + _LAIRS[itr].type);
        }
    }
    for(itr = 0; itr < 9; itr++)
    {
        neighborhood_road_flags[itr] = 0;
    }
    for(itr = 0; itr < 9; itr++)
    {
        neighborhood_river_flags[itr] = 0;
    }
    for(itr_wy = (wy - 1); (wy + 2) > itr_wy; itr_wy++)
    {
        for(itr_wx = (wx - 1); (wx + 2) > itr_wx; itr_wx++)
        {
            if(
                (itr_wy >= 0)
                &&
                (itr_wy < WORLD_HEIGHT)
                &&
                (itr_wx >= 0)
                &&
                (itr_wx < WORLD_WIDTH)
            )
            {
                if((MAP_SQUARE_FLAG(itr_wx, itr_wy, wp) & MSF_ROAD) != 0)
                {
                    neighborhood_road_flags[((itr_wy - wy + 1) * 3) + (itr_wx - wx + 1)] = ST_TRUE;
                }
            }
        }
    }
    if((MAP_SQUARE_FLAG(wx, wy, wp) & MSF_EROAD) != 0)
    {
        roads_enchanted = ST_TRUE;
    }
    else
    {
        roads_enchanted = ST_FALSE;
    }
    combat_terrain_set = cts_Water;
    if(Square_Is_Forest(wx, wy, wp) == ST_TRUE)
    {
        combat_terrain_set = cts_Forest;
    }
    else if(Square_Is_Hills(wx, wy, wp) == ST_TRUE)
    {
        combat_terrain_set = cts_Hills;
    }
    else if(Square_Is_Mountain(wx, wy, wp) == ST_TRUE)
    {
        combat_terrain_set = cts_Mountains;
        _combat_wall_sprite_bank = 0;
    }
    else if(Square_Is_Desert(wx, wy, wp) == ST_TRUE)
    {
        combat_terrain_set = cts_Desert;
        _combat_wall_sprite_bank = 0;
    }
    else if(
        (Square_Is_Swamp(wx, wy, wp) == ST_TRUE)
        ||
        (Square_Is_Grasslands(wx, wy, wp) == ST_TRUE)
    )
    {
        combat_terrain_set = cts_Plains;
    }
    else if(Square_Is_River(wx, wy, wp) == ST_TRUE)
    {
        combat_terrain_set = cts_Plains;
    }
    else if(Square_Is_Tundra(wx, wy, wp) == ST_TRUE)
    {
        combat_terrain_set = cts_Tundra;
    }
    else
    {
        combat_terrain_set = cts_Water;
    }
    /* WTF - vestigial: faithful, executed, and without effect - should already be caught in Square_Is_Forest(), except for its weird SQUARE_EXPLORED check. So, this weirdly catches unexplored nature nodes. But, we the map square can not be unexplored here. */
    if(combat_location_type == clt_NatureNode)
    {
        combat_terrain_set = cts_Forest;
    }
    if(wp == MYRROR_PLANE)
    {
        _combat_wall_sprite_bank = 1;
    }
    random_seed = Get_Random_Seed();
    Set_Random_Seed( (((int32_t)wx * wy * (wp + 5)) + 10039) );  /* OGBUG: the (wp + 5) is pointless */
    Load_Combat_Terrain_Pictures(combat_terrain_set, wp);
    if(_combat_skip_map_generation == ST_FALSE)
    {
        Generate_Combat_Map(
            combat_location_type, 
            combat_house_style, 
            &neighborhood_road_flags[0], 
            roads_enchanted, 
            combat_terrain_set, 
            &neighborhood_river_flags[0], 
            city_has_flying_fortress, 
            attacker_on_floating_island, 
            defender_on_floating_island, 
            city_has_stone_walls, 
            city_population, 
            magic_wall_flags
        );
    }
    _vortex_count = 0;
    Set_Random_Seed(random_seed);
    Combat_Compose_Background();  // ... |-> Copy_Off_To_Back();
}


// WZD ovr154p02
/*
magic_walls
    two-bit mask, and the consumer proves it — Combat.c:22490 and 22503 bit-test it rather than comparing values:
    bit source city enchantment     becomes
    1    WALL_OF_FIRE                battlefield->wall_of_fire
    2    WALL_OF_DARKNESS            battlefield->wall_of_darkness
flying_fortress_flag
    is passed and ignored
city_walls
    stone walls; standing, not rubble;
city_population
    location class and house count
*/
void Generate_Combat_Map(
    int16_t location_type, 
    int16_t house_type, 
    int16_t roads_array[], 
    int16_t enchanted_roads_flag, 
    int16_t combat_terrain_set, 
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
    int16_t random_cay = 0;
    int16_t random_cax = 0;
    int16_t reject_house_location = 0;
    int16_t house_ctr = 0;
    int16_t dirt_patch_count = 0;
    int16_t rough_patch_count = 0;
    int16_t itr1 = 0;
    int16_t itr2 = 0;
    int16_t itr_cgx = 0;
    int16_t itr_cgy = 0;
    int16_t i = 0;  // city area x  NOTE <=  // DNE in Dasm
    int16_t j = 0;  // city area y  NOTE <=  // DNE in Dasm
    battlefield->house_cnt = 0;
    for(j = 0; j <= 3; j++)
    {
        for(i = 0; i <= 3; i++)
        {
            battlefield->walls[j][i] = 0;
        }
    }
    random_seed = Get_Random_Seed();
    switch(location_type)
    {
        case clt_OpenField:
        {
            battlefield->center_square_structure = CS_None;
        } break;
        case clt_Outpost:
        {
            /* OGBUG: disregards 'Wall Of Stone' */
            battlefield->center_square_structure = CS_Outpost;
        } break;
        case clt_City:
        {
            battlefield->center_square_structure = CS_City;
            if(city_population >= 12)
            {
                house_ctr = 0;
                for(j = 0; j <= 3; j++)
                {
                    for(i = 0; i <= 3; i++)
                    {
                        /* Logic to exclude corners if city walls exist */
                        if(
                            ((i == 0 && j == 0) || (i == 3 && j == 0) || (i == 0 && j == 3) || (i == 3 && j == 3))
                            &&
                            (city_walls != ST_FALSE)
                        )
                        {
                            continue;
                        }
                        battlefield->house_cgxs[house_ctr] = (MIN_CGX_CITY + i);
                        battlefield->house_cgys[house_ctr] = (MIN_CGY_CITY + j);
                        battlefield->house_pict_segs[house_ctr] = _combat_house_picts_segs[((house_type * 5) + (Random(5) - 1))];
                        house_ctr++;
                    }
                }
                battlefield->house_cnt = 12;
            }
            else
            {
                battlefield->house_cnt = 0;
                while(battlefield->house_cnt < city_population)
                {
                    random_cax = Random(4) - 1;
                    random_cay = Random(4) - 1;
                    i = (MIN_CGX_CITY + random_cax);
                    j = (MIN_CGY_CITY + random_cay);
                    reject_house_location = ST_FALSE;
                    if(city_walls == ST_TRUE)
                    {
                        if((random_cax == 0 && random_cay == 0) || (random_cax == 0 && random_cay == 3) || (random_cax == 3 && random_cay == 0) || (random_cax == 3 && random_cay == 3))
                        {
                            reject_house_location = ST_TRUE;
                        }
                    }
                    for(house_ctr = 0; house_ctr < battlefield->house_cnt; house_ctr++)
                    {
                        if(battlefield->house_cgxs[house_ctr] == i && battlefield->house_cgys[house_ctr] == j)
                        {
                            reject_house_location = ST_TRUE;
                        }
                    }
                    if(reject_house_location == ST_FALSE)
                    {
                        battlefield->house_cgxs[battlefield->house_cnt] = i;
                        battlefield->house_cgys[battlefield->house_cnt] = j;
                        battlefield->house_pict_segs[battlefield->house_cnt] = _combat_house_picts_segs[((house_type * 5) + (Random(5) - 1))];
                        battlefield->house_cnt++;
                    }
                }
            }
            battlefield->walled = city_walls;
            if(city_walls == ST_TRUE)
            {
                for(j = 0; j <= 3; j++)
                {
                    for(i = 0; i <= 3; i++)
                    {
                        battlefield->walls[j][i] = 1;
                    }
                }
                battlefield->walls[1][1] = 0;  /* 6,11 */
                battlefield->walls[2][1] = 0;  /* 6,12 */
                battlefield->walls[1][2] = 0;  /* 7,11 */
                battlefield->walls[2][2] = 0;  /* 7,12 */
            }

        } break;
        case clt_Fortress:
        {
            battlefield->center_square_structure = CS_Fortress;
            if(city_population >= 11)
            {
                house_ctr = 0;
                for(j = 0; j <= 3; j++)
                {
                    for(i = 0; i <= 3; i++)
                    {
                        /* Logic to exclude corners if city walls exist */
                        if(
                            ((i == 0 && j == 0) || (i == 3 && j == 0) || (i == 0 && j == 3) || (i == 3 && j == 3))
                            &&
                            (city_walls != ST_FALSE)
                        )
                        {
                            continue;
                        }
                        if(i == 1 && j == 1)  /* also exclude Fortress */
                        {
                            continue;
                        }
                        battlefield->house_cgxs[house_ctr] = (MIN_CGX_CITY + i);
                        battlefield->house_cgys[house_ctr] = (MIN_CGY_CITY + j);
                        battlefield->house_pict_segs[house_ctr] = _combat_house_picts_segs[((house_type * 5) + (Random(5) - 1))];
                        house_ctr++;
                    }
                }
                battlefield->house_cnt = 11;  /* 12 - 1, for fortress cell */
            }
            else
            {
                battlefield->house_cnt = 0;
                while(battlefield->house_cnt < city_population)
                {
                    random_cax = Random(4) - 1;
                    random_cay = Random(4) - 1;
                    i = (MIN_CGX_CITY + random_cax);
                    j = (MIN_CGY_CITY + random_cay);
                    reject_house_location = ST_FALSE;
                    if(random_cax == 1 && random_cay == 1) reject_house_location = ST_TRUE;
                    if(city_walls == ST_TRUE)
                    {
                        if((random_cax == 0 && random_cay == 0) || (random_cax == 0 && random_cay == 3) || (random_cax == 3 && random_cay == 0) || (random_cax == 3 && random_cay == 3))
                        {
                            reject_house_location = ST_TRUE;
                        }
                    }
                    for(house_ctr = 0; house_ctr < battlefield->house_cnt; house_ctr++)
                    {
                        if(battlefield->house_cgxs[house_ctr] == i && battlefield->house_cgys[house_ctr] == j)
                        {
                            reject_house_location = ST_TRUE;
                        }
                    }
                    if(reject_house_location == ST_FALSE)
                    {
                        battlefield->house_cgxs[battlefield->house_cnt] = i;
                        battlefield->house_cgys[battlefield->house_cnt] = j;
                        battlefield->house_pict_segs[battlefield->house_cnt] = _combat_house_picts_segs[((house_type * 5) + (Random(5) - 1))];
                        battlefield->house_cnt++;
                    }
                }
            }
            battlefield->walled = city_walls;
            if(city_walls == ST_TRUE)
            {
                for(j = 0; j <= 3; j++)
                {
                    for(i = 0; i <= 3; i++)
                    {
                        battlefield->walls[j][i] = 1;
                    }
                }
                battlefield->walls[1][1] = 0;  /* 6,11 */
                battlefield->walls[2][1] = 0;  /* 6,12 */
                battlefield->walls[1][2] = 0;  /* 7,11 */
                battlefield->walls[2][2] = 0;  /* 7,12 */
            }
        } break;
        case clt_Tower:
        {
            battlefield->center_square_structure = CS_Tower;
        } break;
        case clt_ChaosNode:
        {
            battlefield->center_square_structure = CS_ChaosNode;
        } break;
        case clt_NatureNode:
        {
            battlefield->center_square_structure = CS_NatureNode;
        } break;
        case clt_SorceryNode:
        {
            battlefield->center_square_structure = CS_SorceryNode;
        } break;
        case clt_Cave:
        {
            battlefield->center_square_structure = CS_Cave;
        } break;
        case clt_Dungeon:
        {
            battlefield->center_square_structure = CS_Dungeon;
        } break;
        case clt_AncientTemple:
        {
            battlefield->center_square_structure = CS_Temple;
        } break;
        case clt_Keep:
        {
            battlefield->center_square_structure = CS_Fort;
        } break;
        case clt_MonsterLair:
        {
            battlefield->center_square_structure = CS_Cave;
        } break;
        case clt_Ruins:
        {
            battlefield->center_square_structure = CS_Dungeon;
        } break;
        case clt_FallenTemple:
        {
            battlefield->center_square_structure = CS_Ruins;
        } break;
    }
    Set_Random_Seed(random_seed);
    for(itr_cgy = 0; itr_cgy < COMBAT_GRID_HEIGHT; itr_cgy++)
    {
        for(itr_cgx = 0; itr_cgx < COMBAT_GRID_WIDTH; itr_cgx++)
        {
            battlefield->muds[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = ST_FALSE;
        }
    }
    if((magic_walls & 1) != 0)
    {
        battlefield->wall_of_fire = ST_TRUE;
    }
    else
    {
        battlefield->wall_of_fire = ST_FALSE;
    }
    if((magic_walls & 2) != 0)
    {
        battlefield->wall_of_darkness = ST_TRUE;
    }
    else
    {
        battlefield->wall_of_darkness = ST_FALSE;
    }
    CMB_Enchanted_Roads = enchanted_roads_flag;
    for(itr_cgy = 0; itr_cgy < COMBAT_GRID_HEIGHT; itr_cgy++)
    {
        for(itr_cgx = 0; itr_cgx < COMBAT_GRID_WIDTH; itr_cgx++)
        {
            battlefield->roads[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = ST_FALSE;
        }
    }
    if(combat_terrain_set != cts_Water)
    {
        for(itr_cgy = 0; itr_cgy < COMBAT_GRID_HEIGHT; itr_cgy++)
        {
            for(itr_cgx = 0; itr_cgx < COMBAT_GRID_WIDTH; itr_cgx++)
            {
                battlefield->terrain_group[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = ctg_Grass;
            }
        }
        rough_patch_count = 0;
        dirt_patch_count = 8;
        if(combat_terrain_set == cts_Plains)
        {
            rough_patch_count = 5;
        }
        if(combat_terrain_set == cts_Hills)
        {
            rough_patch_count = 20;
        }
        if(combat_terrain_set == cts_Desert)
        {
            rough_patch_count = 10;
        }
        if(combat_terrain_set == cts_Mountains)
        {
            rough_patch_count = 30;
        }
        if(combat_terrain_set == cts_Tundra)
        {
            rough_patch_count = 10;
        }
        if(combat_terrain_set == cts_Forest)
        {
            rough_patch_count = 5;
        }
        Scatter_Terrain_Patches(CTG_Rough, rough_patch_count, 8, 3);
        Scatter_Terrain_Patches(ctg_Dirt, dirt_patch_count, 4, 0);
        if(combat_terrain_set == cts_Plains)
        {
            Carve_River_Terrain(rivers_array);
        }
        Build_Road_Network(location_type, roads_array);
        Remove_Rough_Terrain(location_type);
        Merge_Dirt_Patches();
    }
    else  /* combat_terrain_set == cts_Water */
    {
        for(itr_cgy = 0; itr_cgy < COMBAT_GRID_HEIGHT; itr_cgy++)
        {
            for(itr_cgx = 0; itr_cgx < COMBAT_GRID_WIDTH; itr_cgx++)
            {
                battlefield->terrain_group[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = ctg_Ocean;
            }
        }
        /*
Build_Road_Network() only runs on non-water maps, so we set battlefield->roads manually.
0x80 is the island's walkable deck
0x81 is marked but deliberately excluded — the cmp di, 10000001b carve-out means that cell gets no land movement and keeps the ocean's costs. 
Geometrically it's always the single cell immediately right of the block's bottom row: (8,13) for defender-only, (19,12) for attacker-only, (12,13) for both. 
A 3×3 walkable deck plus one non-walkable cell hanging off the bottom-right corner reads as the island sprite's edge — the bit of artwork that overhangs the deck and must not be stood on.
Set_Movement_Cost_Maps() skips cells set to 0x81
        */
        if(defender_floating_island_flag == ST_TRUE)
        {
            battlefield->roads[((12 * COMBAT_GRID_WIDTH) + (MIN_CGX_CITY + 0))] = 0x80;
            battlefield->roads[((12 * COMBAT_GRID_WIDTH) + (MIN_CGX_CITY + 1))] = 0x80;
            battlefield->roads[((12 * COMBAT_GRID_WIDTH) + (MIN_CGX_CITY + 2))] = 0x80;
            battlefield->roads[((11 * COMBAT_GRID_WIDTH) + (MIN_CGX_CITY + 0))] = 0x80;
            battlefield->roads[((11 * COMBAT_GRID_WIDTH) + (MIN_CGX_CITY + 1))] = 0x80;
            battlefield->roads[((11 * COMBAT_GRID_WIDTH) + (MIN_CGX_CITY + 2))] = 0x80;
            battlefield->roads[((13 * COMBAT_GRID_WIDTH) + (MIN_CGX_CITY + 0))] = 0x80;
            battlefield->roads[((13 * COMBAT_GRID_WIDTH) + (MIN_CGX_CITY + 1))] = 0x80;
            battlefield->roads[((13 * COMBAT_GRID_WIDTH) + (MIN_CGX_CITY + 2))] = 0x80;
            battlefield->roads[((13 * COMBAT_GRID_WIDTH) + (MIN_CGX_CITY + 3))] = 0x81;  /* special exclusion for Set_Movement_Cost_Maps() */
        }
        if(attacker_floating_island_flag == ST_TRUE)
        {
            if(defender_floating_island_flag == ST_TRUE)
            {
            battlefield->roads[((12 * COMBAT_GRID_WIDTH) +  8)] = 0x80;
            battlefield->roads[((12 * COMBAT_GRID_WIDTH) +  9)] = 0x80;
            battlefield->roads[((12 * COMBAT_GRID_WIDTH) + 10)] = 0x80;
            battlefield->roads[((12 * COMBAT_GRID_WIDTH) + 11)] = 0x80;
            battlefield->roads[((11 * COMBAT_GRID_WIDTH) +  9)] = 0x80;
            battlefield->roads[((11 * COMBAT_GRID_WIDTH) + 10)] = 0x80;
            battlefield->roads[((11 * COMBAT_GRID_WIDTH) + 11)] = 0x80;
            battlefield->roads[((13 * COMBAT_GRID_WIDTH) +  9)] = 0x80;
            battlefield->roads[((13 * COMBAT_GRID_WIDTH) + 10)] = 0x80;
            battlefield->roads[((13 * COMBAT_GRID_WIDTH) + 11)] = 0x80;
            battlefield->roads[((13 * COMBAT_GRID_WIDTH) + 12)] = 0x81;  /* special exclusion for Set_Movement_Cost_Maps() */
            }
            else
            {
            battlefield->roads[((11 * COMBAT_GRID_WIDTH) + 16)] = 0x80;
            battlefield->roads[((11 * COMBAT_GRID_WIDTH) + 17)] = 0x80;
            battlefield->roads[((11 * COMBAT_GRID_WIDTH) + 18)] = 0x80;
            battlefield->roads[((10 * COMBAT_GRID_WIDTH) + 16)] = 0x80;
            battlefield->roads[((10 * COMBAT_GRID_WIDTH) + 17)] = 0x80;
            battlefield->roads[((10 * COMBAT_GRID_WIDTH) + 18)] = 0x80;
            battlefield->roads[((12 * COMBAT_GRID_WIDTH) + 16)] = 0x80;
            battlefield->roads[((12 * COMBAT_GRID_WIDTH) + 17)] = 0x80;
            battlefield->roads[((12 * COMBAT_GRID_WIDTH) + 18)] = 0x80;
            battlefield->roads[((12 * COMBAT_GRID_WIDTH) + 19)] = 0x81;  /* special exclusion for Set_Movement_Cost_Maps() */
            }
        }
    }
    Set_Terrain_Tile_Types();  /* OGBUG: calls as `Set_Terrain_Tile_Types(combat_terrain_set)` */
    Set_Movement_Cost_Maps(location_type, city_walls);
    if(combat_terrain_set == cts_Water)
    {
        battlefield->tree_count = 0;
        battlefield->rock_count = 0;
    }
    else
    {
        battlefield->tree_count = 0;
        battlefield->rock_count = 0;
        Scatter_Tree_Scenery(combat_terrain_set, location_type);
        Scatter_Rock_Scenery(combat_terrain_set, location_type);
    }
}


// WZD ovr154p03
void Scatter_Tree_Scenery(int16_t combat_terrain_set, int16_t combat_location_type)
{
    int16_t patch_size = 0;
    int16_t tries = 0;
    int16_t itr_patches = 0;
    int16_t patch_cgy = 0;
    int16_t patch_cgx = 0;
    int16_t patch_count = 0;
    int16_t tree_sy = 0;
    int16_t tree_sx = 0;
    int16_t cgy_subcell_offset = 0;
    int16_t cgx_subcell_offset = 0;
    int16_t trees_placed = 0;
    int16_t illegal_square = 0;
    int16_t tree_count = 0;
    int16_t trck_cgx = 0;
    int16_t trck_cgy = 0;
    if(combat_terrain_set == cts_Plains || combat_terrain_set == cts_Hills)
    {
        tree_count = Random(20);
    }
    else if(combat_terrain_set == cts_Desert)
    {
        tree_count = Random(10);
    }
    else if(combat_terrain_set == cts_Mountains)
    {
        tree_count = Random(40);
    }
    else if(combat_terrain_set == cts_Tundra)
    {
        tree_count = Random(20);
    }
    else
    {
        tree_count = (30 + Random(30));
    }
    battlefield->tree_count = 0;
    if(tree_count == 0)
    {
        return;
    }
    patch_count = (tree_count / 10) + Random(4);
    if(tree_count < patch_count)
    {
        return;
    }
    patch_size = tree_count / patch_count;
    if(patch_size < 1)
    {
        patch_size = 1;
    }
    tries = 0;
    itr_patches = 0;
    while(itr_patches < patch_count)
    {
        for(;;)
        {
            if(tries > 500)
            {
                return;
            }
            patch_cgx = (Random(COMBAT_GRID_WIDTH) - 1);
            patch_cgy = (Random(COMBAT_GRID_HEIGHT) - 1);
            if(battlefield->roads[((patch_cgy * COMBAT_GRID_WIDTH) + patch_cgx)] != 0)
            {
                continue;
            }
            if(battlefield->terrain_group[((patch_cgy * COMBAT_GRID_WIDTH) + patch_cgx)] != 0)
            {
                continue;
            }
            if(battlefield->terrain_type[((patch_cgy * COMBAT_GRID_WIDTH) + patch_cgx)] == INF)
            {
                continue;
            }
            break;
        }
        trees_placed = 0;
        while(trees_placed < patch_size)
        {
            trck_cgx = patch_cgx + Random(7) - 4;
            trck_cgy = patch_cgy + Random(7) - 4;
            cgx_subcell_offset = Random(6);
            cgy_subcell_offset = Random(6);
            illegal_square = ST_FALSE;
            if(patch_cgx < 0)
            {
                illegal_square = ST_TRUE;
            }
            if(patch_cgy < 0)
            {
                illegal_square = ST_TRUE;
            }
            if(patch_cgx >= COMBAT_GRID_WIDTH)
            {
                illegal_square = ST_TRUE;
            }
            if(patch_cgy >= COMBAT_GRID_HEIGHT)
            {
                illegal_square = ST_TRUE;
            }
            if(combat_location_type != clt_OpenField && combat_location_type != clt_Tower)
            {
                if(trck_cgy >= (MIN_CGY_CITY + 1) && trck_cgy <= MAX_CGY_CITY && trck_cgx >= MIN_CGX_CITY && trck_cgx <= (MAX_CGX_CITY - 1))
                {
                    illegal_square = ST_TRUE;
                }
            }
            if(battlefield->roads[((trck_cgy * COMBAT_GRID_WIDTH) + trck_cgx)] != 0)
            {
                illegal_square = ST_TRUE;
            }
            if(battlefield->terrain_group[((trck_cgy * COMBAT_GRID_WIDTH) + trck_cgx)] != ctg_Grass)
            {
                illegal_square = ST_TRUE;
            }
            if(battlefield->terrain_type[((trck_cgy * COMBAT_GRID_WIDTH) + trck_cgx)] == INF)
            {
                illegal_square = ST_TRUE;
            }
            if(
                battlefield->center_square_structure != CS_None
                &&
                battlefield->center_square_structure != CS_NatureNode
            )
            {
                if(
                    trck_cgx >= MIN_CGX_CITY
                    &&
                    trck_cgx <= (MAX_CGX_CITY - 1)
                    &&
                    trck_cgy >= MIN_CGY_CITY
                    &&
                    trck_cgy <= (MAX_CGY_CITY - 1))
                {
                    illegal_square = ST_TRUE;
                }
            }
            if(
                battlefield->center_square_structure == CS_City
                ||
                battlefield->center_square_structure == CS_Fortress
            )
            {
                if(
                    (trck_cgx == MAX_CGX_CITY && trck_cgy == MIN_CGY_CITY) ||
                    (trck_cgx == MAX_CGX_CITY && trck_cgy == (MIN_CGY_CITY + 1)) ||
                    (trck_cgx == MAX_CGX_CITY && trck_cgy == (MIN_CGY_CITY + 2)) ||
                    (trck_cgx == MAX_CGX_CITY && trck_cgy == (MIN_CGY_CITY + 3)) ||
                    (trck_cgx == (MIN_CGX_CITY    ) && trck_cgy == MAX_CGY_CITY) ||
                    (trck_cgx == (MIN_CGX_CITY + 1) && trck_cgy == MAX_CGY_CITY) ||
                    (trck_cgx == (MIN_CGX_CITY + 2) && trck_cgy == MAX_CGY_CITY) ||
                    (trck_cgx == (MIN_CGX_CITY + 3) && trck_cgy == MAX_CGY_CITY)
                )
                {
                    illegal_square = ST_TRUE;
                }
            }
            Combat_Grid_Screen_Coordinates(trck_cgx, trck_cgy, cgx_subcell_offset, cgy_subcell_offset, &tree_sx, &tree_sy);
            if(
                illegal_square == ST_FALSE
                &&
                tree_sx > SCREEN_XMIN
                &&
                tree_sx < SCREEN_WIDTH
                &&
                tree_sy > SCREEN_YMIN
                &&
                tree_sy < (SCREEN_HEIGHT - 20))
            {
                battlefield->Tree_DrawXs[battlefield->tree_count] = tree_sx - 5;
                battlefield->Tree_DrawYs[battlefield->tree_count] = tree_sy;
                if(battlefield->MoveCost_Ground[((trck_cgy * COMBAT_GRID_WIDTH) + trck_cgx)] < 4)
                {
                    battlefield->MoveCost_Ground[((trck_cgy * COMBAT_GRID_WIDTH) + trck_cgx)]++;
                }
                if(battlefield->MoveCost_Ground2[((trck_cgy * COMBAT_GRID_WIDTH) + trck_cgx)] < 4)
                {
                    battlefield->MoveCost_Ground2[((trck_cgy * COMBAT_GRID_WIDTH) + trck_cgx)]++;
                }
                battlefield->Tree_Indices[battlefield->tree_count] = Random(5) - 1;
                battlefield->tree_count++;
            }
            else
            {
                tries++;
                if(tries < 500)
                {
                    trees_placed--;
                }
            }
            trees_placed++;
        }
        itr_patches++;
    }
}


// WZD ovr154p04
void Scatter_Rock_Scenery(int16_t combat_terrain_set, int16_t combat_location_type)
{
    int16_t rock_sy = 0;
    int16_t rock_sx = 0;
    int16_t cgy_subcell_offset = 0;
    int16_t cgx_subcell_offset = 0;
    int16_t rocks_placed = 0;
    int16_t illegal_square = 0;
    int16_t rock_count = 0;
    int16_t trck_cgx = 0;
    int16_t trck_cgy = 0;
    if(combat_terrain_set == cts_Plains || combat_terrain_set == cts_Hills)
    {
        rock_count = Random(5) - 1;
    }
    else if(combat_terrain_set == cts_Desert)
    {
        rock_count = Random(8) - 1;
    }
    else if(combat_terrain_set == cts_Mountains)
    {
        rock_count = Random(12);
    }
    else if(combat_terrain_set == cts_Tundra)
    {
        rock_count = Random(12) + 4;
    }
    else
    {
        rock_count = Random(8);
    }
    battlefield->rock_count = 0;
    rocks_placed = 0;
    while(rocks_placed < rock_count)
    {
        trck_cgx = Random(COMBAT_GRID_WIDTH) - 1;
        trck_cgy = Random(COMBAT_GRID_HEIGHT) - 1;
        cgx_subcell_offset = Random(2) + 3;
        cgy_subcell_offset = Random(2) + 3;
        illegal_square = 0;
        if(combat_location_type != clt_OpenField && combat_location_type != clt_Tower)
        {
            if(trck_cgy >= (MIN_CGY_CITY + 1) && trck_cgy <= MAX_CGY_CITY && trck_cgx >= MIN_CGX_CITY && trck_cgx <= (MAX_CGX_CITY - 1))
            {
                illegal_square = ST_TRUE;
            }
        }
        if(battlefield->roads[trck_cgy * COMBAT_GRID_WIDTH + trck_cgx] != 0)
        {
            illegal_square = ST_TRUE;
        }
        if(battlefield->terrain_group[trck_cgy * COMBAT_GRID_WIDTH + trck_cgx] != ctg_Grass)
        {
            illegal_square = ST_TRUE;
        }
        if(battlefield->terrain_type[(trck_cgy * COMBAT_GRID_WIDTH) + trck_cgx] == INF)
        {
            illegal_square = ST_TRUE;
        }
        if(battlefield->center_square_structure != CS_None && battlefield->center_square_structure != CS_NatureNode)
        {
            if(trck_cgx >= MIN_CGX_CITY && trck_cgx <= (MAX_CGX_CITY - 1) && trck_cgy >= MIN_CGY_CITY && trck_cgy <= (MAX_CGY_CITY - 1))
            {
                illegal_square = ST_TRUE;
            }
        }
        if(battlefield->center_square_structure == CS_City || battlefield->center_square_structure == CS_Fortress)
        {
            if(trck_cgx == MAX_CGX_CITY && trck_cgy == 10)
            {
                illegal_square = ST_TRUE;
            }
        }
        if(
            (trck_cgx == MAX_CGX_CITY && trck_cgy == (MIN_CGY_CITY + 1)) ||
            (trck_cgx == MAX_CGX_CITY && trck_cgy == (MIN_CGY_CITY + 2)) ||
            (trck_cgx == MAX_CGX_CITY && trck_cgy == (MIN_CGY_CITY + 3)) ||
            (trck_cgx == (MIN_CGX_CITY    ) && trck_cgy == MAX_CGY_CITY) ||
            (trck_cgx == (MIN_CGX_CITY + 1) && trck_cgy == MAX_CGY_CITY) ||
            (trck_cgx == (MIN_CGX_CITY + 2) && trck_cgy == MAX_CGY_CITY) ||
            (trck_cgx == (MIN_CGX_CITY + 3) && trck_cgy == MAX_CGY_CITY)
        )
        {
            illegal_square = ST_TRUE;
        }
        Combat_Grid_Screen_Coordinates(trck_cgx, trck_cgy, cgx_subcell_offset, cgy_subcell_offset, &rock_sx, &rock_sy);
        if(
            illegal_square == ST_FALSE
            &&
            rock_sx > SCREEN_XMIN
            &&
            rock_sx < SCREEN_WIDTH
            &&
            rock_sy > SCREEN_YMIN
            &&
            rock_sy < (SCREEN_HEIGHT - 20)
        )
        {
            battlefield->rock_sx_array[battlefield->rock_count] = rock_sx - 5;
            battlefield->rock_sy_array[battlefield->rock_count] = rock_sy;
            if(battlefield->MoveCost_Ground[trck_cgy * COMBAT_GRID_WIDTH + trck_cgx] < 4)
            {
                battlefield->MoveCost_Ground[trck_cgy * COMBAT_GRID_WIDTH + trck_cgx]++;
            }
            if(battlefield->MoveCost_Ground2[trck_cgy * COMBAT_GRID_WIDTH + trck_cgx] < 4)
            {
                battlefield->MoveCost_Ground2[trck_cgy * COMBAT_GRID_WIDTH + trck_cgx]++;
            }
            battlefield->rock_pict_segs[battlefield->rock_count] = IMG_CMB_Rocks[Random(5) - 1];
            battlefield->rock_count++;
        }
        else
        {
            rocks_placed--;
        }
        rocks_placed++;
    }
}


// WZD ovr154p05
void Build_Road_Network(int16_t location_type, int16_t * roads_array)
{
    int16_t ctg_3 = 0;  /* SE / DR */
    int16_t ctg_2 = 0;  /* S  / D  */
    int16_t ctg_1 = 0;  /* SW / DL */
    int16_t ctg_6 = 0;  /* E  / R  */
    int16_t ctg_5 = 0;  /* C  / M  */
    int16_t ctg_4 = 0;  /* W  / L  */
    int16_t previous_step_direction = 0;
    int16_t ctg_9 = 0;  /* NE / UR */
    int16_t ctg_8 = 0;  /* N  / U  */
    int16_t ctg_7 = 0;  /* NW / UL */
    int16_t steps_on_heading = 0;
    int16_t next_cgy = 0;
    int16_t next_cgx = 0;
    int16_t step_direction = 0;
    int16_t road_origin_cgy = 0;
    int16_t road_origin_cgx = 0;
    int16_t itr_cgx = 0;
    int16_t itr_cgy = 0;
    /* Clear battlefield road tiles */
    for(itr_cgy = 0; itr_cgy < COMBAT_GRID_HEIGHT; itr_cgy++)
    {
        for(itr_cgx = 0; itr_cgx < COMBAT_GRID_WIDTH; itr_cgx++)
        {
            battlefield->roads[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = 0;
        }
    }
    if(roads_array[4] == 1)  /* no road hub — skip all four walks */
    {
        road_origin_cgx = 10;
        road_origin_cgy = 10;
        /* Check East-bound road connection */
        if(roads_array[5] == 1 || roads_array[2] == 1)
        {
            itr_cgx = road_origin_cgx;
            itr_cgy = road_origin_cgy;
            if(location_type == clt_City || location_type == clt_Fortress)
            {
                itr_cgx = (MAX_CGX_CITY - 1);
                itr_cgy = (MAX_CGY_CITY - 1);
            }
            battlefield->roads[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = 1;
            previous_step_direction = 3;
            step_direction = 3;
            steps_on_heading = 0;
            while(itr_cgx < 20)
            {
                if(steps_on_heading < 2)
                {
                    step_direction = previous_step_direction;
                    steps_on_heading++;
                }
                else
                {
                    do
                    {
                        if(Random(4) == 1)
                        {
                            step_direction = Random(4) - 1;
                        }
                        else
                        {
                            step_direction = 3;
                        }
                    } while(step_direction == 1);
                    steps_on_heading = 1;
                }
                previous_step_direction = step_direction;
                next_cgx = itr_cgx + step_delta_cgx[step_direction + 1];
                next_cgy = itr_cgy + step_delta_cgy[step_direction + 1];
                if(next_cgx >= 0 && next_cgx < COMBAT_GRID_WIDTH && next_cgy >= 0 && step_direction != 1 && next_cgy < COMBAT_GRID_HEIGHT)
                {
                    itr_cgx = next_cgx;
                    itr_cgy = next_cgy;
                    battlefield->roads[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = 1;
                }
            }
        }
        /* Check North-bound road connection */
        if(roads_array[1] == 1 || roads_array[0] == 1)
        {
            itr_cgx = road_origin_cgx;
            itr_cgy = road_origin_cgy;
            if(location_type == clt_City || location_type == clt_Fortress)
            {
                itr_cgx = 7;
                itr_cgy = 11;
            }
            battlefield->roads[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = 1;
            previous_step_direction = 2;
            steps_on_heading = 0;
            step_direction = 2;
            while(itr_cgy > 0)
            {
                if(steps_on_heading < 2)
                {
                    step_direction = previous_step_direction;
                    steps_on_heading++;
                }
                else
                {
                    do
                    {
                        if(Random(3) == 1)
                        {
                            step_direction = Random(4) - 1;
                        }
                        else
                        {
                            step_direction = 2;
                        }
                    } while(step_direction == 0);
                    if(step_direction != previous_step_direction)
                    {
                        steps_on_heading = 1;
                    }
                }
                previous_step_direction = step_direction;
                next_cgx = itr_cgx + step_delta_cgx[step_direction + 1];
                next_cgy = itr_cgy + step_delta_cgy[step_direction + 1];
                if(next_cgx >= 0 && next_cgx < COMBAT_GRID_WIDTH && next_cgy >= 0 && next_cgy < COMBAT_GRID_HEIGHT)
                {
                    itr_cgx = next_cgx;
                    itr_cgy = next_cgy;
                    battlefield->roads[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = 1;
                }
            }
        }
        /* Check West-bound road connection */
        if(roads_array[3] == 1 || roads_array[6] == 1)
        {
            itr_cgx = road_origin_cgx;
            itr_cgy = road_origin_cgy;
            if(location_type == 2 || location_type == 3)
            {
                itr_cgx = 6;
                itr_cgy = 12;
            }
            battlefield->roads[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = 1;
            previous_step_direction = 1;
            step_direction = 0;
            steps_on_heading = 0;
            while(itr_cgx > 0)
            {
                if(steps_on_heading < 2)
                {
                    step_direction = previous_step_direction;
                    steps_on_heading++;
                }
                else
                {
                    do
                    {
                        if(Random(4) == 1)
                        {
                            step_direction = Random(4) - 1;
                        }
                        else
                        {
                            step_direction = 1;
                        }
                    } while(step_direction == 3);
                    steps_on_heading = 1;
                }
                previous_step_direction = step_direction;
                next_cgx = itr_cgx + step_delta_cgx[step_direction + 1];
                next_cgy = itr_cgy + step_delta_cgy[step_direction + 1];
                if(next_cgx >= 0 && next_cgx < COMBAT_GRID_WIDTH && next_cgy >= 0 && next_cgy < COMBAT_GRID_HEIGHT)
                {
                    itr_cgx = next_cgx;
                    itr_cgy = next_cgy;
                    battlefield->roads[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = 1;
                }
            }
        }
        /* Check South-bound road connection */
        if(roads_array[7] == 1 || roads_array[8] == 1)
        {
            itr_cgx = road_origin_cgx;
            itr_cgy = road_origin_cgy;
            if(location_type == 2 || location_type == 3)
            {
                itr_cgx = 7;
                itr_cgy = 12;
            }
            battlefield->roads[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = 1;
            previous_step_direction = 0;
            step_direction = 0;
            steps_on_heading = 0;
            while(itr_cgy < COMBAT_GRID_WIDTH)
            {
                if(steps_on_heading < 2)
                {
                    step_direction = previous_step_direction;
                    steps_on_heading++;
                }
                else
                {
                    do
                    {
                        if(Random(4) == 1)
                        {
                            step_direction = Random(4) - 1;
                        }
                        else
                        {
                            step_direction = 0;
                        }
                    } while(step_direction == 2);
                    steps_on_heading = 1;
                }
                previous_step_direction = step_direction;
                next_cgx = itr_cgx + step_delta_cgx[step_direction + 1];
                next_cgy = itr_cgy + step_delta_cgy[step_direction + 1];
                if(next_cgx >= 0 && next_cgx < COMBAT_GRID_WIDTH && next_cgy >= 0 && next_cgy < COMBAT_GRID_HEIGHT)
                {
                    itr_cgx = next_cgx;
                    itr_cgy = next_cgy;
                    battlefield->roads[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = 1;
                }
            }
        }
    }
    /* Second pass: calculate road masks based on neighboring road tiles */
    for(itr_cgy = 0; itr_cgy < COMBAT_GRID_HEIGHT; itr_cgy++)
    {
        for(itr_cgx = 0; itr_cgx < COMBAT_GRID_WIDTH; itr_cgx++)
        {
            if(battlefield->roads[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] == 0)
            {
                continue;
            }
            ctg_7 = (uint8_t)battlefield->roads[(((itr_cgy - 1) * COMBAT_GRID_WIDTH) + (itr_cgx - 1))];
            ctg_8 = (uint8_t)battlefield->roads[(((itr_cgy - 1) * COMBAT_GRID_WIDTH) + (itr_cgx    ))];
            ctg_9 = (uint8_t)battlefield->roads[(((itr_cgy - 1) * COMBAT_GRID_WIDTH) + (itr_cgx + 1))];
            ctg_4 = (uint8_t)battlefield->roads[(((itr_cgy    ) * COMBAT_GRID_WIDTH) + (itr_cgx - 1))];
            ctg_5 = (uint8_t)battlefield->roads[(((itr_cgy    ) * COMBAT_GRID_WIDTH) + (itr_cgx    ))];
            ctg_6 = (uint8_t)battlefield->roads[(((itr_cgy    ) * COMBAT_GRID_WIDTH) + (itr_cgx + 1))];
            ctg_1 = (uint8_t)battlefield->roads[(((itr_cgy + 1) * COMBAT_GRID_WIDTH) + (itr_cgx - 1))];
            ctg_2 = (uint8_t)battlefield->roads[(((itr_cgy + 1) * COMBAT_GRID_WIDTH) + (itr_cgx    ))];
            ctg_3 = (uint8_t)battlefield->roads[(((itr_cgy + 1) * COMBAT_GRID_WIDTH) + (itr_cgx + 1))];
            if(itr_cgy == 0)
            {
                ctg_7 = 1;
                ctg_8  = 1;
                ctg_9 = 1;
            }
            if(itr_cgy == COMBAT_GRID_WIDTH)
            {
                ctg_1 = 1;
                ctg_2  = 1;
                ctg_3 = 1;
            }
            if(itr_cgx == 0)
            {
                ctg_7 = 1;
                ctg_4  = 1;
                ctg_1 = 1;
            }
            if(itr_cgx == 20)
            {
                ctg_9 = 1;
                ctg_6  = 1;
                ctg_3 = 1;
            }
            if(ctg_4 != 0 && ctg_6 != 0)
            {
                battlefield->roads[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] += 2;
            }
            if(ctg_8 != 0 && ctg_2 != 0)
            {
                battlefield->roads[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] += 4;
            }
            if(ctg_8 != 0 && ctg_4 != 0)
            {
                battlefield->roads[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] += 8;
            }
            if(ctg_6 != 0 && ctg_8 != 0)
            {
                battlefield->roads[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] += 16;
            }
            if(ctg_2 != 0 && ctg_6 != 0)
            {
                battlefield->roads[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] += 32;
            }
            if(ctg_4 != 0 && ctg_2 != 0)
            {
                battlefield->roads[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] += 64;
            }
        }
    }
    /* Third pass: randomize road tile bitmask bit 0 */
    for(itr_cgy = 0; itr_cgy < COMBAT_GRID_HEIGHT; itr_cgy++)
    {
        for(itr_cgx = 0; itr_cgx < COMBAT_GRID_WIDTH; itr_cgx++)
        {
            if(battlefield->roads[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] != 0)
            {
                if(Random(2) == 1)
                {
                    battlefield->roads[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] &= 0xFE;
                }
            }
        }
    }
}


// WZD ovr154p06
void Carve_River_Terrain(int16_t * rivers_array)
{
    int16_t direction_is_forward = 0;
    int16_t heading_alternator = 0;
    int16_t next_cgy = 0;
    int16_t next_cgx = 0;
    int16_t river_cgy = 0;
    int16_t river_cgx = 0;
    int16_t step_direction = 0;
    heading_alternator = 0;
    if(rivers_array[4] != 1)
    {
        return;
    }
    if(rivers_array[5] == 1 || rivers_array[2] == 1 || rivers_array[3] == 1 || rivers_array[6] == 1)
    {
        river_cgx = 7;
        river_cgy = 20;
        while(river_cgx < (COMBAT_GRID_WIDTH - 1) && river_cgy > 0)
        {
            if(Random(2) == 1)
            {
                step_direction = Random(4) - 1;
            }
            else
            {
                if(heading_alternator == 0)
                {
                    step_direction = 3;
                }
                else
                {
                    step_direction = 2;
                }
            }
            heading_alternator = 1 - heading_alternator;
            next_cgx = river_cgx + step_delta_cgx[step_direction + 1];
            next_cgy = river_cgy + step_delta_cgy[step_direction + 1];
            direction_is_forward = ST_TRUE;
            if(step_direction == 1)
            {
                direction_is_forward = ST_FALSE;
            }
            if(step_direction == 0)
            {
                direction_is_forward = ST_FALSE;
            }
            if(next_cgx >= 0 && next_cgx < COMBAT_GRID_WIDTH && next_cgy >= 0 && direction_is_forward == ST_TRUE && next_cgy < COMBAT_GRID_HEIGHT)
            {
                river_cgx = next_cgx;
                river_cgy = next_cgy;
                battlefield->terrain_group[((river_cgy * COMBAT_GRID_WIDTH) + river_cgx)] = ctg_River;
            }
        }
    }
    else if(rivers_array[0] == 1 || rivers_array[1] == 1 || rivers_array[7] == 1 || rivers_array[8] == 1)
    {
        river_cgx = (4 + Random(8));
        river_cgy = 20;
        while(river_cgx < (COMBAT_GRID_WIDTH - 1) && river_cgy < (COMBAT_GRID_HEIGHT - 1))
        {
            if(Random(2) == 1)
            {
                step_direction = Random(4) - 1;
            }
            else
            {
                if(heading_alternator == 0)
                {
                    step_direction = 3;
                }
                else
                {
                    step_direction = 0;
                }
            }
            heading_alternator = 1 - heading_alternator;
            next_cgx = river_cgx + step_delta_cgx[step_direction + 1];
            next_cgy = river_cgy + step_delta_cgy[step_direction + 1];
            direction_is_forward = ST_TRUE;
            if(step_direction == 1)
            {
                direction_is_forward = ST_FALSE;
            }
            if(step_direction == 2)
            {
                direction_is_forward = ST_FALSE;
            }
            if(next_cgx >= 0 && next_cgx < COMBAT_GRID_WIDTH && next_cgy >= 0 && direction_is_forward == ST_TRUE && next_cgy < COMBAT_GRID_HEIGHT)
            {
                river_cgx = next_cgx;
                river_cgy = next_cgy;
                battlefield->terrain_group[((river_cgy * COMBAT_GRID_WIDTH) + river_cgx)] = ctg_River;
            }
        }
    }
}


// WZD ovr154p07
void Merge_Dirt_Patches(void)
{
    int16_t pass = 0;
    int16_t dirt_cgy = 0;
    int16_t dirt_cgx = 0;
    for(pass = 0; pass < 10; pass++)
    {
        for(dirt_cgy = 0; dirt_cgy < COMBAT_GRID_HEIGHT; dirt_cgy++)
        {
            for(dirt_cgx = 0; dirt_cgx < COMBAT_GRID_WIDTH; dirt_cgx++)
            {
                if(battlefield->terrain_group[((dirt_cgy * COMBAT_GRID_WIDTH) + dirt_cgx)] == ctg_Grass)
                {
                    if(
                        battlefield->terrain_group[((dirt_cgy * COMBAT_GRID_WIDTH) + (dirt_cgx + 1))] == ctg_Dirt
                        &&
                        battlefield->terrain_group[((dirt_cgy * COMBAT_GRID_WIDTH) + (dirt_cgx - 1))] == ctg_Dirt
                    )
                    {
                        battlefield->terrain_group[((dirt_cgy * COMBAT_GRID_WIDTH) + dirt_cgx)] = ctg_Dirt;
                    }
                    if(
                        battlefield->terrain_group[(((dirt_cgy - 1) * COMBAT_GRID_WIDTH) + dirt_cgx)] == ctg_Dirt
                        &&
                        battlefield->terrain_group[(((dirt_cgy + 1) * COMBAT_GRID_WIDTH) + dirt_cgx)] == ctg_Dirt
                    )
                    {
                        battlefield->terrain_group[((dirt_cgy * COMBAT_GRID_WIDTH) + dirt_cgx)] = ctg_Dirt;
                    }
                }
            }
        }
    }
}


// WZD ovr154p08
void Remove_Rough_Terrain(int16_t combat_location_type)
{
    int16_t pass = 0;
    int16_t rough_cgy = 0;
    int16_t rough_cgx = 0;
    for(pass = 0; pass < 8; pass++)
    {
        for(rough_cgy = 0; rough_cgy < COMBAT_GRID_HEIGHT; rough_cgy++)
        {
            for(rough_cgx = 0; rough_cgx < COMBAT_GRID_WIDTH; rough_cgx++)
            {
                if(battlefield->terrain_group[((rough_cgy * COMBAT_GRID_WIDTH) + rough_cgx)] == CTG_Rough)
                {
                    if(
                        battlefield->terrain_group[(((rough_cgy - 1) * COMBAT_GRID_WIDTH) + (rough_cgx - 1))] == ctg_Dirt ||
                        battlefield->terrain_group[(((rough_cgy - 1) * COMBAT_GRID_WIDTH) + (rough_cgx    ))] == ctg_Dirt ||
                        battlefield->terrain_group[(((rough_cgy - 1) * COMBAT_GRID_WIDTH) + (rough_cgx + 1))] == ctg_Dirt ||
                        battlefield->terrain_group[(((rough_cgy    ) * COMBAT_GRID_WIDTH) + (rough_cgx - 1))] == ctg_Dirt ||
                        battlefield->terrain_group[(((rough_cgy    ) * COMBAT_GRID_WIDTH) + (rough_cgx + 1))] == ctg_Dirt ||
                        battlefield->terrain_group[(((rough_cgy + 1) * COMBAT_GRID_WIDTH) + (rough_cgx - 1))] == ctg_Dirt ||
                        battlefield->terrain_group[(((rough_cgy + 1) * COMBAT_GRID_WIDTH) + (rough_cgx    ))] == ctg_Dirt ||
                        battlefield->terrain_group[(((rough_cgy + 1) * COMBAT_GRID_WIDTH) + (rough_cgx + 1))] == ctg_Dirt
                    )
                    {
                        battlefield->terrain_group[((rough_cgy * COMBAT_GRID_WIDTH) + rough_cgx)] = ctg_Grass;
                    }
                    if(battlefield->roads[((rough_cgy * COMBAT_GRID_WIDTH) + rough_cgx)] != ST_FALSE)
                    {
                        battlefield->terrain_group[((rough_cgy * COMBAT_GRID_WIDTH) + rough_cgx)] = ctg_Grass;
                    }
                }
            }
        }
    }
    if(combat_location_type != clt_OpenField)
    {
        for(rough_cgy = MIN_CGY_CITY; rough_cgy <= MAX_CGY_CITY; rough_cgy++)
        {
            for(rough_cgx = MIN_CGX_CITY; rough_cgx <= MAX_CGX_CITY; rough_cgx++)
            {
                if(battlefield->terrain_group[((rough_cgy * COMBAT_GRID_WIDTH) + rough_cgx)] == CTG_Rough)
                {
                    battlefield->terrain_group[((rough_cgy * COMBAT_GRID_WIDTH) + rough_cgx)] = ctg_Grass;
                }
            }
        }
    }
}


// WZD ovr154p09
/*
    translates terrain_group into terrain_type
    "ctg" === "combat terrain group"
*/
/*
It converts the abstract terrain_group map into concrete terrain_type tile-picture indices — 10 terrain_group reads in, 26 terrain_type writes out. For every cell it samples the 3×3 neighbourhood into ctg_1..ctg_9, switches on the centre's group, and picks a tile:
centre group    tile selection
Grass           Random(4) - 1 — four interchangeable variants
Dirt            Random(2) + 27, Random(4) + 3 — banded variants
DeepWater       its own band
Rough           16-way autotile — autotile_variant + CTILE_DownRough, chosen from which of N/E/S/W also match
CTG_IDK (5)     Random(4) + 51 — unreachable, as established
So two mechanisms in one pass: random variant selection for interiors, edge autotiling where Rough meets something else.
*/
void Set_Terrain_Tile_Types(void)
{
    int16_t niu_rough_tile_base = 0;
    int16_t niu_variable = 0;
    int16_t dirt_tile_base = 0;
    int16_t niu_dirt_tile_base = 0;
    int16_t grass_tile_base = 0;
    int16_t autotile_variant = 0;
    int16_t ctg_3 = 0;  /* SE / DR */
    int16_t ctg_2 = 0;  /* S  / D  */
    int16_t ctg_1 = 0;  /* SW / DL */
    int16_t ctg_6 = 0;  /* E  / R  */
    int16_t ctg_5 = 0;  /* C  / M  */
    int16_t ctg_4 = 0;  /* W  / L  */
    int16_t ctg_9 = 0;  /* NE / UR */
    int16_t ctg_8 = 0;  /* N  / U  */
    int16_t ctg_7 = 0;  /* NW / UL */
    int16_t itr_y = 0;
    int16_t itr_x = 0;
    int16_t itr_cgx = 0;
    int16_t itr_cgy = 0;
    grass_tile_base = 0;
    niu_dirt_tile_base = 0;
    dirt_tile_base = 0;
    niu_variable = 0;
    niu_rough_tile_base = 0;
    for(itr_cgy = 0; itr_cgy < COMBAT_GRID_HEIGHT; itr_cgy++)
    {
        for(itr_cgx = 0; itr_cgx < COMBAT_GRID_WIDTH; itr_cgx++)
        {
            ctg_7 = battlefield->terrain_group[(((itr_cgy - 1) * COMBAT_GRID_WIDTH) + (itr_cgx - 1))];
            ctg_8 = battlefield->terrain_group[(((itr_cgy - 1) * COMBAT_GRID_WIDTH) + (itr_cgx    ))];
            ctg_9 = battlefield->terrain_group[(((itr_cgy - 1) * COMBAT_GRID_WIDTH) + (itr_cgx + 1))];
            ctg_4 = battlefield->terrain_group[(((itr_cgy    ) * COMBAT_GRID_WIDTH) + (itr_cgx - 1))];
            ctg_5 = battlefield->terrain_group[(((itr_cgy    ) * COMBAT_GRID_WIDTH) + (itr_cgx    ))];
            ctg_6 = battlefield->terrain_group[(((itr_cgy    ) * COMBAT_GRID_WIDTH) + (itr_cgx + 1))];
            ctg_1 = battlefield->terrain_group[(((itr_cgy + 1) * COMBAT_GRID_WIDTH) + (itr_cgx - 1))];
            ctg_2 = battlefield->terrain_group[(((itr_cgy + 1) * COMBAT_GRID_WIDTH) + (itr_cgx    ))];
            ctg_3 = battlefield->terrain_group[(((itr_cgy + 1) * COMBAT_GRID_WIDTH) + (itr_cgx + 1))];
            /*
                BEGIN: Perimeter
            */
            if(itr_cgy == COMBAT_GRID_YMIN)
            {
                ctg_7 = ctg_Grass;
                ctg_8 = ctg_Grass;
                ctg_9 = ctg_Grass;
            }
            if(itr_cgy == (COMBAT_GRID_YMAX - 1))
            {
                ctg_1 = ctg_Grass;
                ctg_2 = ctg_Grass;
                ctg_3 = ctg_Grass;
            }
            if(itr_cgx == COMBAT_GRID_XMIN)
            {
                ctg_7 = ctg_Grass;
                ctg_4 = ctg_Grass;
                ctg_1 = ctg_Grass;
            }
            if(itr_cgx == (COMBAT_GRID_XMAX - 1))
            {
                ctg_9 = ctg_Grass;
                ctg_6 = ctg_Grass;
                ctg_3 = ctg_Grass;
            }
            /*
                END: Perimeter
            */
            /*
                BEGIN:  TerrGroup_Middle == ctg_Grass
                        sets grass or one of 12 pairs of grass/plain edges
            */
            if(ctg_5 == ctg_Grass)
            {
                if(ctg_6 == ctg_Dirt)  /* right is dirt */
                {
                    if(ctg_8 == ctg_Dirt)
                    {
                        // grass with dirt right and up
                        battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = ((dirt_tile_base + 27) + Random(2));
                    }
                    else if(ctg_2 == ctg_Dirt)
                    {
                        // grass with dirt right and down
                        battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = ((dirt_tile_base + 11) + Random(2));
                    }
                    else
                    {
                        // grass with dirt right
                        battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = ((dirt_tile_base + 29) + Random(2));
                    }
                }
                else if(ctg_8 == ctg_Dirt)  /* up is dirt */
                {
                    if(ctg_4 == ctg_Dirt)
                    {
                        // grass with dirt up and left
                        battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = ((dirt_tile_base + 21) + Random(2));
                    }
                    else
                    {
                        // grass with dirt up
                        battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = ((dirt_tile_base + 23) + Random(2));
                    }
                }
                else if(ctg_4 == ctg_Dirt)  /* left is dirt */
                {
                    if(ctg_8 == ctg_Dirt)
                    {
                        // grass with dirt left and up
                        battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = ((dirt_tile_base + 21) + Random(2));
                    }
                    else if(ctg_2 == ctg_Dirt)
                    {
                        // grass with dirt left and down
                        battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = ((dirt_tile_base + 15) + Random(2));
                    }
                    else
                    {
                        // grass with dirt left
                        battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = ((dirt_tile_base + 17) + Random(2));
                    }
                }
                else if(ctg_2 == ctg_Dirt)  /* down is dirt */
                {
                    if(ctg_6 == ctg_Dirt)
                    {
                        // grass with dirt down and right
                        battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = ((dirt_tile_base + 11) + Random(2));
                    }
                    else if(ctg_4 == ctg_Dirt)
                    {
                        // grass with dirt down and left
                        battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = ((dirt_tile_base + 15) + Random(2));
                    }
                    else
                    {
                        // grass with dirt down
                        battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = ((dirt_tile_base + 9) + Random(2));
                    }
                }
                else if(ctg_3 == ctg_Dirt)  /* down-right is dirt */
                {
                    // CTILE_DownRightD1  = 8, CTILE_DownRightD2  = 9,
                    battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = ((dirt_tile_base + 7) + Random(2));
                }
                else if(ctg_9 == ctg_Dirt)  /* up-right is dirt */
                {
                    battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = ((dirt_tile_base + 25) + Random(2));
                }
                else if(ctg_7 == ctg_Dirt)  /* up-left is dirt */
                {
                    battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = ((dirt_tile_base + 19) + Random(2));
                }
                else if(ctg_1 == ctg_Dirt)  /* down-left is dirt */
                {
                    battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = ((dirt_tile_base + 13) + Random(2));
                }
                else  /* there is no adjacent dirt */
                {
                    // terrain type is grass {0,1,2,3}
                    battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = (grass_tile_base + (Random(4) - 1));
                }
            }
            /*
                END:  TerrGroup_Middle == ctg_Grass
            */
            /*
                BEGIN:  ctg_River
            */
            if(battlefield->terrain_group[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] == ctg_River)
            {
                if(itr_cgy == COMBAT_GRID_YMIN)
                {
                    ctg_7 = ctg_River;
                    ctg_8 = ctg_River;
                    ctg_9 = ctg_River;
                }
                if(itr_cgy == (COMBAT_GRID_YMAX - 1))
                {
                    ctg_1 = ctg_River;
                    ctg_2 = ctg_River;
                    ctg_3 = ctg_River;
                }
                if(itr_cgx == COMBAT_GRID_XMIN)
                {
                    ctg_7 = ctg_River;
                    ctg_4 = ctg_River;
                    ctg_1 = ctg_River;
                }
                if(itr_cgx == (COMBAT_GRID_XMAX - 1))
                {
                    ctg_9 = ctg_River;
                    ctg_6 = ctg_River;
                    ctg_3 = ctg_River;
                }
                if(
                    (ctg_4 == ctg_River)
                    &&
                    (ctg_6 == ctg_River)
                )
                {
                    battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = (56 + ((Random(2) - 1) * 6));
                }
                if(
                    (ctg_8 == ctg_River)
                    &&
                    (ctg_2 == ctg_River)
                )
                {
                    battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = (57 + ((Random(2) - 1) * 6));
                }
                if(
                    (ctg_8 == ctg_River)
                    &&
                    (ctg_4 == ctg_River)
                )
                {
                    battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = (58 + ((Random(2) - 1) * 6));
                }
                if(
                    (ctg_6 == ctg_River)
                    &&
                    (ctg_8 == ctg_River)
                )
                {
                    battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = (59 + ((Random(2) - 1) * 6));
                }
                if(
                    (ctg_2 == ctg_River)
                    &&
                    (ctg_6 == ctg_River)
                )
                {
                    battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = (60 + ((Random(2) - 1) * 6));
                }
                if(
                    (ctg_4 == ctg_River)
                    &&
                    (ctg_2 == ctg_River)
                )
                {
                    battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = (61 + ((Random(2) - 1) * 6));
                }
                if(itr_cgy == COMBAT_GRID_YMIN)
                {
                    ctg_7 = ctg_Grass;
                    ctg_8 = ctg_Grass;
                    ctg_9 = ctg_Grass;
                }
                if(itr_cgy == (COMBAT_GRID_YMAX - 1))
                {
                    ctg_1 = ctg_Grass;
                    ctg_2 = ctg_Grass;
                    ctg_3 = ctg_Grass;
                }
                if(itr_cgx == COMBAT_GRID_XMIN)
                {
                    ctg_7 = ctg_Grass;
                    ctg_4 = ctg_Grass;
                    ctg_1 = ctg_Grass;
                }
                if(itr_cgx == (COMBAT_GRID_XMAX - 1))
                {
                    ctg_9 = ctg_Grass;
                    ctg_6 = ctg_Grass;
                    ctg_3 = ctg_Grass;
                }
            }
            /*
                END:  ctg_River
            */
            /*
                BEGIN:  ctg_Dirt
            */
            if(ctg_5 == ctg_Dirt)
            {
                // CTILE_Dirt1  = 4, CTILE_Dirt2  = 5, CTILE_Dirt3  = 6, CTILE_Dirt4  = 7,
                battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = ((niu_dirt_tile_base + 3) + Random(4));
            }
            /*
                END:  ctg_Dirt
            */
            /*
                BEGIN:  ctg_Ocean
            */
            if(ctg_5 == ctg_Ocean)
            {
                // CTILE_Ocean1  = 48, CTILE_Ocean2  = 49, CTILE_Ocean3  = 50, CTILE_Ocean4  = 51,
                battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = (47 + Random(4));
            }
            /*
                END:  ctg_Ocean
            */
            /*
                BEGIN:  ctg_NIU_5
            */
            if(ctg_5 == ctg_NIU_5)
            {
                // CTILE_Type5_1  = 52, CTILE_Type5_2  = 53, CTILE_Type5_3  = 54, CTILE_Type5_4  = 55,
                battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = (51 + Random(4));
            }
            /*
                END:  ctg_NIU_5
            */
            /*
                BEGIN:  CTG_Rough
            */
            if(ctg_5 == CTG_Rough)
            {
                if(ctg_8 == CTG_Rough)
                {
                    if(ctg_6 == CTG_Rough)
                    {
                        if(ctg_2 == CTG_Rough)
                        {
                            if(ctg_4 == CTG_Rough)
                            {
                                autotile_variant = 4;
                            }
                            else
                            {
                                autotile_variant = 10;
                            }
                        }
                        else
                        {
                            if(ctg_4 == CTG_Rough)
                            {
                                autotile_variant = 13;
                            }
                            else
                            {
                                autotile_variant = 12;
                            }
                        }
                    }
                    else
                    {
                        if(ctg_2 == CTG_Rough)
                        {
                            if(ctg_4 == CTG_Rough)
                            {
                                autotile_variant = 11;
                            }
                            else
                            {
                                autotile_variant = 1;
                            }
                        }
                        else
                        {
                            if(ctg_4 == CTG_Rough)
                            {
                                autotile_variant = 14;
                            }
                            else
                            {
                                autotile_variant = 5;
                            }
                        }
                    }
                }
                else
                {
                    if(ctg_6 == CTG_Rough)
                    {
                        if(ctg_2 == CTG_Rough)
                        {
                            if(ctg_4 == CTG_Rough)
                            {
                                autotile_variant = 8;
                            }
                            else
                            {
                                autotile_variant = 7;
                            }
                        }
                        else
                        {
                            if(ctg_4 == CTG_Rough)
                            {
                                autotile_variant = 3;
                            }
                            else
                            {
                                autotile_variant = 2;
                            }
                        }
                    }
                    else
                    {
                        if(ctg_2 == CTG_Rough)
                        {
                            if(ctg_4 == CTG_Rough)
                            {
                                autotile_variant = 9;
                            }
                            else
                            {
                                autotile_variant = 0;
                            }
                        }
                        else
                        {
                            if(ctg_4 == CTG_Rough)
                            {
                                autotile_variant = 6;
                            }
                            else
                            {
                                autotile_variant = 15;
                            }
                        }
                    }
                }
                battlefield->terrain_type[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = (CTILE_DownRough + niu_rough_tile_base + autotile_variant);
            }
            /*
                END:  CTG_Rough
            */
        }
    }
}


// WZD ovr154p10
/*

    OGBUG: location_type and city_walls pushed like parameters, but do not get used

Terrain Movement Points Per Square  
Cities          1/2  
Hills           2  
Rivers          2  
Roads           1/2  
Rough (Dirt)    2  
Tree            2  

*/
void Set_Movement_Cost_Maps(int16_t location_type, int16_t city_walls)
{
    int16_t itr_cgy = 0;
    int16_t itr_cgx = 0;
    int16_t terain_group = 0;
    int16_t road = 0;  // Dasm reuses terain_group
    for(itr_cgy = 0; itr_cgy < COMBAT_GRID_HEIGHT; itr_cgy++)
    {
        for(itr_cgx = 0; itr_cgx < COMBAT_GRID_WIDTH; itr_cgx++)
        {
            terain_group = battlefield->terrain_group[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)];
            switch(terain_group)
            {
                case ctg_Grass:
                case ctg_Dirt:
                {
                    battlefield->MoveCost_Ground[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]   = 2;
                    battlefield->MoveCost_Teleport[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = 2;
                    battlefield->MoveCost_Ground2[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]  = 2;
                    battlefield->MoveCost_Sailing[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]  = -1;  // INF
                } break;
                case CTG_Rough:  /* "Rough (Dirt)" ... ¿ also, "Tree" ? */
                {
                    battlefield->MoveCost_Ground[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]   = 4;
                    battlefield->MoveCost_Teleport[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = 2;
                    battlefield->MoveCost_Ground2[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]  = 4;
                    battlefield->MoveCost_Sailing[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]  = -1;  // INF
                } break;
                case ctg_River:
                {
                    battlefield->MoveCost_Ground[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]   = 4;
                    battlefield->MoveCost_Teleport[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = 2;
                    battlefield->MoveCost_Ground2[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]  = 4;
                    battlefield->MoveCost_Sailing[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]  = 2;
                } break;
                case ctg_Ocean:
                {
                    battlefield->MoveCost_Ground[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]   = -1;  // INF
                    battlefield->MoveCost_Teleport[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = 2;
                    battlefield->MoveCost_Ground2[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]  = -1;  // INF
                    battlefield->MoveCost_Sailing[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)]  = 2;
                } break;
            }
        }
    }
    for(itr_cgy = 0; itr_cgy < COMBAT_GRID_HEIGHT; itr_cgy++)
    {
        for(itr_cgx = 0; itr_cgx < COMBAT_GRID_WIDTH; itr_cgx++)
        {
            road = battlefield->roads[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)];
            if(
                (road != 0)
                &&
                (road != 0x81)  /* OON extra special exclusion from Generate_Combat_Map() for 'Floating Island' */
            )
            {
                battlefield->MoveCost_Ground[  ((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = 1;
                battlefield->MoveCost_Teleport[((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = 2;
                battlefield->MoveCost_Ground2[ ((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = 1;
                battlefield->MoveCost_Sailing[ ((itr_cgy * COMBAT_GRID_WIDTH) + itr_cgx)] = INF;
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
    for(itr_cgy = 10; itr_cgy < COMBAT_GRID_HEIGHT; itr_cgy++)
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
        (battlefield->center_square_structure != CS_None)
        &&
        (battlefield->center_square_structure != CS_City)
    )
    {
        battlefield->MoveCost_Ground[COMBAT_STRUCTURE_IDX] = INF;
        battlefield->MoveCost_Teleport[COMBAT_STRUCTURE_IDX] = INF;
        battlefield->MoveCost_Ground2[COMBAT_STRUCTURE_IDX] = INF;
        battlefield->MoveCost_Sailing[COMBAT_STRUCTURE_IDX] = INF;
    }
}


// WZD ovr154p11
/*
updates battlefield->terrain_group
OON XREF:  Generate_Combat_Map()
*/
void Scatter_Terrain_Patches(int16_t ctg, int16_t patch_count, int16_t length_span, int16_t length_base)
{
    int16_t itr2 = 0;
    int16_t size = 0;
    int16_t niu_variable = 0;
    int16_t direction = 0;
    int16_t next_cgy = 0;
    int16_t next_cgx = 0;
    int16_t random_cgy = 0;
    int16_t random_cgx = 0;
    int16_t itr1 = 0;
    int16_t cgx = 0;
    int16_t cgy = 0;
    for(itr1 = 0; itr1 < patch_count; itr1++)
    {
        random_cgx = (Random(COMBAT_GRID_WIDTH) - 1);
        random_cgy = (Random(COMBAT_GRID_HEIGHT) - 1);
        next_cgx = (random_cgx + step_delta_cgx[Random(4)]);
        next_cgy = (random_cgy + step_delta_cgy[Random(4)]);
        niu_variable = ST_UNDEFINED;
        size = (length_base + Random(length_span));
        for(itr2 = 0; itr2 < size; itr2++)
        {
            direction = (Random(4) - 1);
            cgx = (next_cgx + step_delta_cgx[(1 + direction)]);
            cgy = (next_cgy + step_delta_cgy[(1 + direction)]);
            SETMAX(cgx, 20);
            SETMAX(cgy, 21);
            SETMIN(cgx, 0);
            SETMIN(cgy, 0);
            next_cgx = cgx;
            next_cgy = cgy;
            battlefield->terrain_group[((cgy * COMBAT_GRID_WIDTH) + cgx)] = (int8_t)ctg;
        }
    }
}


// WZD ovr154p12
void Combat_Grid_Screen_Coordinates(int16_t cgx, int16_t cgy, int16_t cgx_subcell_offset, int16_t cgy_subcell_offset, int16_t * screen_x, int16_t * screen_y)
{
    int16_t sy = 0;
    int16_t sx = 0;
    sx = (((cgx - cgy) * 16) + 158);
    sy = (((cgx + cgy) *  8) -  80);
    sx += ((cgx_subcell_offset - cgy_subcell_offset) * 2);
    sy += (cgx_subcell_offset + cgy_subcell_offset);
    *screen_x = sx;
    *screen_y = sy;
}


// WZD ovr154p13
void Apply_Earth_To_Mud(int16_t cgx, int16_t cgy)
{
    int16_t itr_cgy = 0;
    int16_t itr_cgx = 0;
    int16_t combat_terrain_type_group = 0;
    for(itr_cgy = -2; itr_cgy < 3; itr_cgy++)
    {
        for(itr_cgx = -2; itr_cgx < 3; itr_cgx++)
        {
            combat_terrain_type_group = battlefield->terrain_group[(((cgy + itr_cgy) * COMBAT_GRID_WIDTH) + (cgx + itr_cgx))];
            if(
                (combat_terrain_type_group == ctg_Grass)
                ||
                (combat_terrain_type_group == ctg_Dirt)
            )
            {
                battlefield->muds[(((cgy + itr_cgy) * COMBAT_GRID_WIDTH) + (cgx + itr_cgx))] = ST_TRUE;
                battlefield->MoveCost_Ground[ (((cgy + itr_cgy) * COMBAT_GRID_WIDTH) + (cgx + itr_cgx))] = 12;
                battlefield->MoveCost_Ground2[(((cgy + itr_cgy) * COMBAT_GRID_WIDTH) + (cgx + itr_cgx))] = 12;
                battlefield->MoveCost_Sailing[(((cgy + itr_cgy) * COMBAT_GRID_WIDTH) + (cgx + itr_cgx))] = 12;
            }
        }
    }
}


// WZD ovr154p14
int16_t Get_Combat_Grid_Cell_X(int16_t screen_x, int16_t screen_y)
{
    int16_t combat_grid_cell_x = 0;
    combat_grid_cell_x = ((((screen_x - 158) / 2) + screen_y + 80) / 16);
    return combat_grid_cell_x;
}


// WZD ovr154p15
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
; combat square at the specified screen pixel location
*/
/*

XREF:
    Combat_Screen_Map_Draw_Entities()
    Combat_Grid_Entity_Create()

Combat_Screen_Map_Draw_Entities()
    Screen_To_Combat_Grid_Cell_X_And_Offset(Pointer_X(), Pointer_Y(), &combat_grid_cell_x, &combat_grid_cell_x_offset);

Combat_Grid_Entity_Create()
    Screen_To_Combat_Grid_Cell_X_And_Offset(draw_x, draw_y, &combat_grid_cell_x, &combat_grid_cell_x_offset);

*/
void Screen_To_Combat_Grid_Cell_X_And_Offset(int16_t screen_x, int16_t screen_y, int16_t * combat_grid_cell_x, int16_t * combat_grid_cell_x_offset)
{
    int16_t IDK = 0;  // _CX_

    IDK = (((screen_x - 158) / 2) + screen_y + 80);

    *combat_grid_cell_x = (IDK / 16);

    *combat_grid_cell_x_offset = (IDK % 16);

}


// WZD ovr154p17
// drake178: CMB_GetInTileY()
/*
; calculates and returns in the passed pointers both
; the Y coordinate of, and the Y offset within, the
; combat square at the specified screen pixel location
*/
/*
screen x,y  {0,...,319},{0,...199}

XREF:
    Combat_Screen_Map_Draw_Entities()
    Combat_Grid_Entity_Create()

Combat_Screen_Map_Draw_Entities()
    Screen_To_Combat_Grid_Cell_Y_And_Offset(Pointer_X(), Pointer_Y(), &combat_grid_cell_y, &combat_grid_cell_y_offset);

Combat_Grid_Entity_Create()
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
void Combat_Cache_Read(void)
{
    int16_t more_world_data_size_PR = 0;
    int16_t world_data_size_PR = 0;
    FILE * file_pointer = NULL;
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
    file_pointer = stu_fopen_ci(str_COMBAT_TMP__ovr154, str_RB__ovr154);

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
    fread((World_Data + world_data_size_B), more_world_data_size_B, 1, file_pointer);

    // gfclose(filehandle);
    fclose(file_pointer);

}


// WZD ovr154p19
void Combat_Cache_Write(void)
{
    int16_t more_world_data_size_PR = 0;
    int16_t world_data_size_PR = 0;
    FILE * file_pointer = NULL;
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
    file_pointer = stu_fopen_ci(str_COMBAT_TMP__ovr154, str_WB__ovr154);

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
    fwrite((World_Data + world_data_size_B), more_world_data_size_B, 1, file_pointer);

    // gfclose(filehandle);
    fclose(file_pointer);

}



/*
    WIZARDS.EXE  ovr163
*/

// WZD ovr163p01
// drake178: CMB_LoadTerrainGFX()
/*
; maps in the TILEX EMM handle, and if the combat
; terrain is not water, loads the appropriate square,
; tree, and rock images into it
*/
/*

loads set of 48 terrain pictures, 5 trees, 5 rocks

*/
void Load_Combat_Terrain_Pictures(int16_t combat_terrain_set, int16_t wp)
{

    char combat_terrain_set_lbx_filename[LEN_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    SAMB_ptr temp_seg = 0;
    int16_t itr = 0;  // _SI_

    Map_Tile_EMS_Page_As_Sandbox();

    if(combat_terrain_set == cts_Water)
    {
        return;
    }
    
    if(
        (combat_terrain_set == cts_Plains)
        ||
        (combat_terrain_set == cts_Hills)
        ||
        (combat_terrain_set == cts_Forest)
    )
    {

        if(wp == ARCANUS_PLANE)
        {

            stu_strcpy(combat_terrain_set_lbx_filename, cmbgrass_lbx_file__ovr163);

        }
        else  /* MYRROR_PLANE */
        {

            stu_strcpy(combat_terrain_set_lbx_filename, cmbgrasc_lbx_file__ovr163);

        }

    }
    else if(combat_terrain_set == cts_Desert)
    {
        
        if(wp == ARCANUS_PLANE)
        {

            stu_strcpy(combat_terrain_set_lbx_filename, cmbdesrt_lbx_file__ovr163);

        }
        else  /* MYRROR_PLANE */
        {

            stu_strcpy(combat_terrain_set_lbx_filename, cmbdesrc_lbx_file__ovr163);

        }

    }
    else if(combat_terrain_set == cts_Mountains)
    {
        
        if(wp == ARCANUS_PLANE)
        {

            stu_strcpy(combat_terrain_set_lbx_filename, cmbmount_lbx_file__ovr163);

        }
        else  /* MYRROR_PLANE */
        {

            stu_strcpy(combat_terrain_set_lbx_filename, cmbmounc_lbx_file__ovr163);

        }

    }
    else if(combat_terrain_set == cts_Tundra)
    {
        
        if(wp == ARCANUS_PLANE)
        {

            stu_strcpy(combat_terrain_set_lbx_filename, cmbtundr_lbx_file__ovr163);

        }
        else  /* MYRROR_PLANE */
        {

            stu_strcpy(combat_terrain_set_lbx_filename, cmbtundc_lbx_file__ovr163);

        }

    }



    temp_seg = Allocate_First_Block(EMS_PFBA, 1);

    for(itr = 0; itr < 48; itr++)
    {

        // _combat_terrain_pict_segs[itr] = LBX_Reload_Next(combat_terrain_set_lbx_filename, itr, EMS_PFBA);
        _combat_terrain_pict_segs[itr] = LBX_Reload_Next(combat_terrain_set_lbx_filename, itr, EMS_PFBA);

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
void Combat_Compose_Background(void)
{
    int16_t battlefield_terrain_type = 0;
    int16_t cgy = 0;
    int16_t cgx = 0;
    int16_t screen_y = 0;
    int16_t screen_x = 0;
    int16_t row_start_cgy = 0;
    int16_t row_start_cgx = 0;
    int16_t itr_x = 0;
    int16_t itr_y = 0;
    Set_Page_Off();
    Reset_Window();
    Map_Tile_EMS_Page_As_Sandbox();
    for(itr_y = 0; itr_y < COMBAT_GRID_HEIGHT; itr_y++)
    {
        CALC_ROW_START_CGX;
        CALC_ROW_START_CGY;
        for(itr_x = 0; itr_x < 11; itr_x++)
        {
            CALC_CGX;
            CALC_CGY;
            CALC_SCREEN_X;
            CALC_SCREEN_Y;
            battlefield_terrain_type = battlefield->terrain_type[((cgy * COMBAT_GRID_WIDTH) + cgx)];
            if(battlefield_terrain_type >= CTILE_LeftRightRiver1)  // River
            {
                battlefield_terrain_type = ((cgy + cgx) & 0x3);  // mask on first two bits  {0,1,2,3}  ¿ " base square" ?  ...(56 & 0x3) == 0, ..., (67 & 0x3) == 3
            }
            if(battlefield_terrain_type < 48)
            {
                Clipped_Draw(screen_x, screen_y, _combat_terrain_pict_segs[battlefield_terrain_type]);
            }
        }
    }
    _combat_road_anim_frame = ((_combat_road_anim_frame + 1) % 5);
    _combat_water_anim_frame = ((_combat_water_anim_frame + 1) % 5);
    _combat_chaos_node_anim_frame = ((_combat_chaos_node_anim_frame + 1) % 8);
    if(_combat_magic_wall_anim_skip == 0)
    {
        _combat_magic_wall_anim_frame = ((_combat_magic_wall_anim_frame + 1) % 4);
    }
    _combat_magic_wall_anim_skip = (1 - _combat_magic_wall_anim_skip);
    _combat_curse_anim_frame += 1;
    if(_combat_curse_anim_frame > MAX_SINT)
    {
        _combat_curse_anim_frame = 0;
    }
    if(battlefield->center_square_structure == CS_ChaosNode)
    {
        Combat_Grid_Screen_Coordinates(6, 11, 0, 0, &screen_x, &screen_y);
        screen_x -= 46;
        screen_y -= 13;
        FLIC_Draw(screen_x, screen_y, chriver_volca_segs[8]);
    }
    if(combat_walk_anim_dir == 0)
    {
        combat_walk_anim_frame += 1;
        if(combat_walk_anim_frame == 3)
        {
            combat_walk_anim_dir = 1;
            combat_walk_anim_frame = 1;
        }
    }
    else
    {
        combat_walk_anim_frame -= 1;
        if(combat_walk_anim_frame == -1)
        {
            combat_walk_anim_dir = 0;
            combat_walk_anim_frame = 1;
        }
    }
    combat_idle_anim_frame = ((combat_idle_anim_frame + 1) % 3);
    if(_combat_unit_attack_anim_frame == 1)
    {
        _combat_unit_attack_anim_frame = 3;
    }
    else
    {
        _combat_unit_attack_anim_frame = 1;
    }
    if(_combat_unit_defend_anim_frame == 1)
    {
        _combat_unit_defend_anim_frame = 3;
    }
    else
    {
        _combat_unit_defend_anim_frame = 1;
    }
    FLIC_Draw(0, 164, combat_background_bottom);
    Copy_Off_To_Back();
}


// WZD ovr163p03
void Wall_Rise_Load(int16_t wall_type)
{
    int16_t itr = 0;
    _wall_rise_type = wall_type;
    Mark_Block(_screen_seg);
    // ; reallocate the battle figure predraw space and reassign all of their pointers
    // ; WARNING: redoing the pointers may cause trouble
    for(itr = 0; itr < 18; itr++)
    {
        battle_unit_picts_seg[itr] = Allocate_Next_Block(_screen_seg, 55);
    }
    switch(wall_type)
    {
        case 0:
        {
            // load a set of 12 animations based on what type of walls are being used in the battle
            // WARNING: these animations are empty in the file (but are present in CMBTWALL.LBX, an otherwise unused data file, although the animation sequence is in reverse and the total is too large for the sandbox)
            for(itr = 0; itr < 12; itr++)
            {
                _wallrise_seg[itr] = LBX_Reload_Next(wallrise_lbx_file__ovr163, ((_combat_wall_sprite_bank * 12) + itr), _screen_seg);
            }
            break;
        }
        case 1:
        {
            for(itr = 0; itr < 14; itr++)
            {
                _wallrise_seg[itr] = LBX_Reload_Next(wallrise_lbx_file__ovr163, ((3 * 12) + itr), _screen_seg);
            }
            break;
        }
        case 2:
        {
            for(itr = 0; itr < 14; itr++)
            {
                _wallrise_seg[itr] = LBX_Reload_Next(wallrise_lbx_file__ovr163, (((3 * 12) + 14) + itr), _screen_seg);
            }
            break;
        }
    }
    Release_Block(_screen_seg);
}


// WZD ovr163p04
void Allocate_Combat_Base_Blocks(void)
{
    // MoO2  _combat_data = Allocate_First_Block()
    battlefield = (struct s_BATTLEFIELD *)Allocate_First_Block(_screen_seg, ((sizeof(struct s_BATTLEFIELD) / 16) + 1));
    combat_grid_entities = (struct s_COMBAT_ENTITY *)Allocate_Next_Block( _screen_seg, 482);    // 482 PR, 7712 B
    _missiles            = (struct s_MISSILE       *)Allocate_Next_Block( _screen_seg,  10);    //  10 PR,  160 B
    _vortexes            = (struct s_MAGIC_VORTEX  *)Allocate_Next_Block( _screen_seg,   9);    //   9 PR,  144 B
    _cmbt_movepath_cost_map = (uint8_t *)Near_Allocate_First( 504);
    _cmbt_mvpth_c           = (uint8_t *)Near_Allocate_Next(  504);
    _cmbt_path_data         = (int16_t *)Near_Allocate_Next( 1008);
    _cmbt_mvpth_x           = (int16_t *)Near_Allocate_Next(   60);
    _cmbt_mvpth_y           = (int16_t *)Near_Allocate_Next(   60);
}


// WZD ovr163p05
/**
 * @brief Initializes projectile records for a combat ranged attack volley.
 *
 * Builds up to @p missile_count entries in the global `_missiles` array by
 * computing source/target figure offsets, converting combat-grid cells to
 * screen coordinates, deriving a firing angle, and mapping that angle to one
 * of the eight projectile direction enums.
 *
 * Each initialized missile gets:
 * - source and target screen coordinates,
 * - projectile type,
 * - initial travel progress (`travel_percent = 10`),
 * - directional sprite orientation (`direction`).
 *
 * Finally, the global volley counter `m_missile_count` is set to
 * @p missile_count.
 *
 * @param missile_count Number of missiles to generate in the current volley.
 * @param Targets Defender figure count. Missiles are dealt round-robin across
 *        the defender's figure slots via `itr_msl % Targets`.
 * @param src_wx Source combat-grid X coordinate.
 * @param src_wy Source combat-grid Y coordinate.
 * @param dst_wx Destination combat-grid X coordinate.
 * @param dst_wy Destination combat-grid Y coordinate.
 * @param type Projectile type value stored in each missile record.
 *
 * @note This function writes to global combat state (`_missiles` and
 *       `m_missile_count`).
 * @note `Targets` is used as a modulus and must be nonzero.
 */
void Make_Missiles(int16_t missile_count, int16_t Targets, int16_t src_wx, int16_t src_wy, int16_t dst_wx, int16_t dst_wy, int16_t type)
{
    int16_t aim_origin_y = 0;
    int16_t aim_origin_x = 0;
    int16_t dst_screen_y = 0;
    int16_t dst_screen_x = 0;
    int16_t src_screen_y = 0;
    int16_t src_screen_x = 0;
    int16_t dst_figure_y = 0;
    int16_t dst_figure_x = 0;
    int16_t src_figure_y = 0;
    int16_t src_figure_x = 0;
    int16_t angle = 0;
    int16_t itr_msl = 0;
    int16_t direction = 0;
    for(itr_msl = 0; itr_msl < missile_count; itr_msl++)
    {
        Battle_Unit_Figure_Position(missile_count, itr_msl, &src_figure_x, &src_figure_y);
        Battle_Unit_Figure_Position(missile_count, (itr_msl % Targets), &dst_figure_x, &dst_figure_y);
        Combat_Grid_Screen_Coordinates(src_wx, src_wy, 0, 0, &src_screen_x, &src_screen_y);
        Combat_Grid_Screen_Coordinates(dst_wx, dst_wy, 0, 0, &dst_screen_x, &dst_screen_y);
        src_figure_y -= 8;
        dst_figure_y -= 8;
        aim_origin_x = src_screen_x;
        aim_origin_y = (src_screen_y + 8);
        src_screen_x += src_figure_x;
        src_screen_y += src_figure_y;
        dst_screen_x += dst_figure_x;
        dst_screen_y += dst_figure_y;
        angle = Get_Angle((dst_screen_x - aim_origin_x), (dst_screen_y - aim_origin_y));
        if(
            (angle < 22)
            ||
            (angle > 337)
        )
        {
            direction = cemd_Right;
        }
        else if(angle < 69)
        {
            direction = cemd_DownRight;
        }
        else if(angle < 112)
        {
            direction = cemd_Down;
        }
        else if(angle < 158)
        {
            direction = cemd_DownLeft;
        }
        else if(angle < 202)
        {
            direction = cemd_Left;
        }
        else if(angle < 248)
        {
            direction = cemd_UpLeft;
        }
        else if(angle < 292)
        {
            direction = cemd_Up;
        }
        else
        {
            direction = cemd_UpRight;
        }
        _missiles[itr_msl].src_screen_x = src_screen_x;
        _missiles[itr_msl].src_screen_y = src_screen_y;
        _missiles[itr_msl].dst_screen_x = dst_screen_x;
        _missiles[itr_msl].dst_screen_y = dst_screen_y;
        _missiles[itr_msl].type = type;
        _missiles[itr_msl].travel_percent = 10;
        _missiles[itr_msl].direction = direction;
    }
    m_missile_count = missile_count;
}


// WZD ovr163p06
/*
~ UnitView.C  Load_Unit_Figure()
~ Combat_Figure_Compose()
*/
int16_t Combat_Figure_Load(int16_t unit_type, int16_t bufpi)
{
    char file_name[LEN_FILE_NAME] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char buffer[6] = { 0, 0, 0, 0, 0, 0 };
    SAMB_ptr * figure_pict_set_seg = 0;
    int16_t offset = 0;
    int16_t entry_num = 0;
    int16_t itr = 0;
    FIGUREX_MAP
    FIGUREX_OFFSET
    FIGUREX_POINTER
    stu_itoa(((unit_type / 15) + 1), buffer, 10);
    stu_strcpy(file_name, figure_lbx_file__ovr163);
    if(((unit_type / 15) + 1) < 10)
    {
        stu_strcat(file_name, str_figure_plural_s__ovr163);
    }
    stu_strcat(file_name, buffer);
    stu_strcat(file_name, str_empty_string__ovr163);
    entry_num = ((unit_type % 15) * 8);
    for(itr = 0; itr < 8; itr++)  /* 8 directions/faces per unit figure picture set */
    {
        figure_pict_set_seg[itr] = LBX_Reload_Next(file_name, (entry_num + itr), (EMS_PFBA + offset));
    }
    Combat_Figure_Compose(bufpi, 0, 0, 0, 0);  /* OGBUG:  nothing uses the results of this call */
    return bufpi;
}



#ifdef STU_DEBUG
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
    int16_t cgy = 0;
    int16_t cgx = 0;
    int16_t battlefield_terrain_type = 0;

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

    cgx = Get_Combat_Grid_Cell_X(mouse_x, mouse_y);
    cgy = Get_Combat_Grid_Cell_Y(mouse_x, mouse_y);
    l_screen_x = (((cgx - cgy) * 16) + 158);  /* ¿ + mid x ? */
    l_screen_y = (((cgx + cgy) *  8) -  80);  /* ¿ + mid y ? */


    stu_strcpy(temp_string, "MD X,Y");
    string_width = Get_String_Width(temp_string);
    Print(         2, (y_off+(y_pos*line_height)), temp_string);
    Print_Integer(40, (y_off+(y_pos*line_height)), mouse_x);
    Print_Integer(56, (y_off+(y_pos*line_height)), mouse_y);
    y_pos++;

    y_pos++;

    stu_strcpy(temp_string, "CG C,R");
    string_width = Get_String_Width(temp_string);
    Print(         2, (y_off+(y_pos*line_height)), temp_string);
    Print_Integer(40, (y_off+(y_pos*line_height)), combat_grid_col);
    Print_Integer(56, (y_off+(y_pos*line_height)), combat_grid_row);
    y_pos++;

    y_pos++;

    stu_strcpy(temp_string, "CG X,Y");
    string_width = Get_String_Width(temp_string);
    Print(         2, (y_off+(y_pos*line_height)), temp_string);
    Print_Integer(40, (y_off+(y_pos*line_height)), combat_grid_x);
    Print_Integer(56, (y_off+(y_pos*line_height)), combat_grid_y);
    y_pos++;

    stu_strcpy(temp_string, "CG1 X,Y");
    string_width = Get_String_Width(temp_string);
    Print(         2, (y_off+(y_pos*line_height)), temp_string);
    Print_Integer(40, (y_off+(y_pos*line_height)), combat_grid_x1);
    Print_Integer(56, (y_off+(y_pos*line_height)), combat_grid_y1);
    y_pos++;

    stu_strcpy(temp_string, "CG2 X,Y");
    string_width = Get_String_Width(temp_string);
    Print(         2, (y_off+(y_pos*line_height)), temp_string);
    Print_Integer(40, (y_off+(y_pos*line_height)), combat_grid_x2);
    Print_Integer(56, (y_off+(y_pos*line_height)), combat_grid_y2);
    y_pos++;

    y_pos++;

    stu_strcpy(temp_string, "CGC 2,1");
    string_width = Get_String_Width(temp_string);
    Print(         2, (y_off+(y_pos*line_height)), temp_string);
    Print_Integer((2 + string_width +  5), (y_off+(y_pos*line_height)), cgx);
    Print_Integer((2 + string_width + 16), (y_off+(y_pos*line_height)), cgy);
    y_pos++;

    stu_strcpy(temp_string, "CGC SX,SY");
    string_width = Get_String_Width(temp_string);
    Print(         2, (y_off+(y_pos*line_height)), temp_string);
    Print_Integer((2 + string_width +  5), (y_off+(y_pos*line_height)), l_screen_x);
    Print_Integer((2 + string_width + 16), (y_off+(y_pos*line_height)), l_screen_y);
    y_pos++;


    battlefield_terrain_type = battlefield->terrain_type[((cgy * COMBAT_GRID_WIDTH) + cgx)];
    stu_strcpy(temp_string, "CMBT TERR");
    string_width = Get_String_Width(temp_string);
    Print(         2, (y_off + (y_pos * line_height)), temp_string);
    Print_Integer((2 + string_width +  5), (y_off + (y_pos * line_height)), battlefield_terrain_type);
    y_pos++;



    y_pos++;
    stu_strcpy(temp_string, "ATK A/D/G");
    Print(         2, (y_off+(y_pos*line_height)), temp_string);
    Print_Integer(56, (y_off+(y_pos*line_height)), DBG_atk_active);
    Print_Integer(68, (y_off+(y_pos*line_height)), DBG_atk_dead);
    Print_Integer(80, (y_off+(y_pos*line_height)), DBG_atk_gone);
    y_pos++;

    stu_strcpy(temp_string, "DEF A/D/G");
    Print(         2, (y_off+(y_pos*line_height)), temp_string);
    Print_Integer(56, (y_off+(y_pos*line_height)), DBG_def_active);
    Print_Integer(68, (y_off+(y_pos*line_height)), DBG_def_dead);
    Print_Integer(80, (y_off+(y_pos*line_height)), DBG_def_gone);
    y_pos++;

    stu_strcpy(temp_string, "WINNER");
    Print(         2, (y_off+(y_pos*line_height)), temp_string);
    Print_Integer(40, (y_off+(y_pos*line_height)), _combat_winner);
    y_pos++;



    // Set_Font_Style_Shadow_Down(0, 0, 0, 0);
    // Set_Outline_Color(0);
    // Set_Alias_Color(0);

}
#endif


#ifdef STU_DEBUG
/* CLAUDE */
void DBG_Compare_Battle_Units(const char * label)
{
    int16_t itr;
    int16_t mismatch_count = 0;
    struct s_BATTLE_UNIT * live;
    struct s_BATTLE_UNIT * snap;

    LOG_DEBUG(LOG_CAT_COMBAT, "[DBG_Compare_Battle_Units] %s: _combat_total_unit_count=%d, battle_units=%p, DBG_battle_units=%p", label, _combat_total_unit_count, (void *)battle_units, (void *)DBG_battle_units);

    if(_combat_total_unit_count < 0 || _combat_total_unit_count > MAX_BATTLE_UNIT_SLOT_COUNT)
    {
        LOG_DEBUG(LOG_CAT_COMBAT, "[DBG_Compare_Battle_Units] %s: BOGUS _combat_total_unit_count=%d, aborting compare", label, _combat_total_unit_count);
        return;
    }

    for(itr = 0; itr < _combat_total_unit_count && itr < MAX_BATTLE_UNIT_SLOT_COUNT; itr++)
    {
        live = &battle_units[itr];
        snap = &DBG_battle_units[itr];

        if(snap->unit_idx < 0 || snap->unit_idx > _units || snap->status < bus_Active || snap->status > bus_Gone || snap->controller_idx < 0 || snap->controller_idx > NEUTRAL_PLAYER_IDX)
        {
            LOG_DEBUG(LOG_CAT_COMBAT, "[DBG_Compare_Battle_Units] SNAPSHOT ALREADY GARBAGE [%d]: unit_idx=%d  status=%d  controller_idx=%d  (_units=%d)", itr, snap->unit_idx, snap->status, snap->controller_idx, _units);
        }

        if(live->unit_idx < 0 || live->unit_idx > _units || live->status < bus_Active || live->status > bus_Gone || live->controller_idx < 0 || live->controller_idx > NEUTRAL_PLAYER_IDX)
        {
            LOG_DEBUG(LOG_CAT_COMBAT, "[DBG_Compare_Battle_Units] LIVE DATA GARBAGE [%d]: unit_idx=%d  status=%d  controller_idx=%d  (_units=%d)", itr, live->unit_idx, live->status, live->controller_idx, _units);
        }

        if(memcmp(live, snap, sizeof(struct s_BATTLE_UNIT)) != 0)
        {
            mismatch_count++;
            LOG_DEBUG(LOG_CAT_COMBAT, "[DBG_Compare_Battle_Units] MISMATCH battle_units[%d]:", itr);
            if(live->unit_idx != snap->unit_idx) { LOG_DEBUG(LOG_CAT_COMBAT, "  unit_idx:        live=%d  snap=%d", live->unit_idx, snap->unit_idx); }
            if(live->status != snap->status) { LOG_DEBUG(LOG_CAT_COMBAT, "  status:          live=%d  snap=%d", live->status, snap->status); }
            if(live->controller_idx != snap->controller_idx) { LOG_DEBUG(LOG_CAT_COMBAT, "  controller_idx:  live=%d  snap=%d", live->controller_idx, snap->controller_idx); }
            if(live->melee != snap->melee) { LOG_DEBUG(LOG_CAT_COMBAT, "  melee:           live=%d  snap=%d", live->melee, snap->melee); }
            if(live->ranged != snap->ranged) { LOG_DEBUG(LOG_CAT_COMBAT, "  ranged:          live=%d  snap=%d", live->ranged, snap->ranged); }
            if(live->defense != snap->defense) { LOG_DEBUG(LOG_CAT_COMBAT, "  defense:         live=%d  snap=%d", live->defense, snap->defense); }
            if(live->resist != snap->resist) { LOG_DEBUG(LOG_CAT_COMBAT, "  resist:          live=%d  snap=%d", live->resist, snap->resist); }
            if(live->hits != snap->hits) { LOG_DEBUG(LOG_CAT_COMBAT, "  hits:            live=%d  snap=%d", live->hits, snap->hits); }
            if(live->figure_cnt != snap->figure_cnt) { LOG_DEBUG(LOG_CAT_COMBAT, "  figure_cnt:     live=%d  snap=%d", live->figure_cnt, snap->figure_cnt); }
            if(live->figure_max != snap->figure_max) { LOG_DEBUG(LOG_CAT_COMBAT, "  figure_max:     live=%d  snap=%d", live->figure_max, snap->figure_max); }
            if(live->front_figure_damage != snap->front_figure_damage) { LOG_DEBUG(LOG_CAT_COMBAT, "  front_fig_dmg:   live=%d  snap=%d", live->front_figure_damage, snap->front_figure_damage); }
            if(live->cgx != snap->cgx) { LOG_DEBUG(LOG_CAT_COMBAT, "  cgx:             live=%d  snap=%d", live->cgx, snap->cgx); }
            if(live->cgy != snap->cgy) { LOG_DEBUG(LOG_CAT_COMBAT, "  cgy:             live=%d  snap=%d", live->cgy, snap->cgy); }
            if(live->enchantments != snap->enchantments) { LOG_DEBUG(LOG_CAT_COMBAT, "  enchantments:    live=0x%08X  snap=0x%08X", live->enchantments, snap->enchantments); }
            if(live->combat_effects != snap->combat_effects) { LOG_DEBUG(LOG_CAT_COMBAT, "  combat_effects:  live=0x%04X  snap=0x%04X", live->combat_effects, snap->combat_effects); }
            if(live->race != snap->race) { LOG_DEBUG(LOG_CAT_COMBAT, "  race:            live=%d  snap=%d", live->race, snap->race); }
            if(live->movement_points != snap->movement_points) { LOG_DEBUG(LOG_CAT_COMBAT, "  movement_points: live=%d  snap=%d", live->movement_points, snap->movement_points); }
            if(live->Abilities != snap->Abilities) { LOG_DEBUG(LOG_CAT_COMBAT, "  Abilities:       live=0x%04X  snap=0x%04X", live->Abilities, snap->Abilities); }
            if(live->item_enchantments != snap->item_enchantments) { LOG_DEBUG(LOG_CAT_COMBAT, "  item_enchants:   live=0x%08X  snap=0x%08X", live->item_enchantments, snap->item_enchantments); }
        }
    }

    if(mismatch_count == 0)
    {
        LOG_DEBUG(LOG_CAT_COMBAT, "[DBG_Compare_Battle_Units] %s: all %d battle units match snapshot", label, itr);
    }
    else
    {
        LOG_DEBUG(LOG_CAT_COMBAT, "[DBG_Compare_Battle_Units] %s: %d of %d battle units DIFFER from snapshot", label, mismatch_count, itr);
    }
}
#endif
