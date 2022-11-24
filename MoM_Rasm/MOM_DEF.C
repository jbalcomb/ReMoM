
#include "MOM_DEF.H"


//dseg:87C4 00 00 00 00 00 00 00 00 00 00+Game GAME_DATA <0>
//dseg:87D4 00 00 00 00 00 00 00 00 00 00+Settings SETTINGS <0>

SettingsStruct Settings;



/*
    seg001, same in MGC & WZD
*/
// MGC dseg:27FF
// WZD dseg:2881
extern char cnst_ConfigFile[] = "CONFIG.MOM";
// MGC dseg:280A
// WZD dseg:288C
extern char cnst_ConfigErr[] = "Run INSTALL to configure MASTER OF MAGIC.\n\n";
// MGC dseg:2836
// WZD dseg:28B8
extern char cnst_RB[] = "rb";
// MGC dseg:2839
// WZD dseg:28BB
extern char cnst_MIDI_Init[] = "Initializing Roland Drivers...$";
// MGC dseg:2859
// WZD dseg:28DB
extern char cnst_SAVE[] = "SAVE";
// MGC dseg:285E
// WZD dseg:28E0
extern char cnst_SAVE_ext[] = ".GAM";
// MGC dseg:2863
// WZD dseg:28E5
extern char cnst_Set_File[] = "MAGIC.SET";
// MGC dseg:286D
// WZD dseg:28EF
extern char cnst_WB[] = "wb";
// MGC dseg:2870
// WZD dseg:28F2
extern char cnst_FONT_File[] = "FONTS.LBX";

// MGC dseg:87A4 Config_Data CONFIG_DATA <0>
// WZD dseg:BD6E Config_Data CONFIG_DATA <0>
//struct CONFIG_DATA Config_Data;
int Config_Data[18];
// MGC dseg:87B6 UU_IMG_OVL_WorkMark@ dw 0
// WZD dseg:BD80 UU_IMG_OVL_WorkMark@ dw 0               ; single-loaded 6 frame animation, unused afaik ; (blue tile frame called "city work area")
// MGC dseg:87B8 UU_OVL_Map_CenterY dw 0
// WZD dseg:BD82 OVL_Map_CenterY dw 0
// MGC dseg:87BA UU_OVL_Map_CenterX dw 0
// WZD dseg:BD84 OVL_Map_CenterX dw 0
// MGC dseg:87BC UU_OVL_Map_Plane dw 0
// WZD dseg:BD86 OVL_Map_Plane dw 0
// MGC dseg:87BE UU_OVL_Map_TopY dw 0
// WZD dseg:BD88 OVL_Map_TopY dw 0
// MGC dseg:87C0 UU_OVL_Map_LeftX dw 0
// WZD dseg:BD8A OVL_Map_LeftX dw 0
// MGC dseg:87C2 UU_Human_Player dw 0
// WZD dseg:BD8C Human_Player dw 0                       ; index of the human player (special rules apply)
// MGC dseg:87C4 Game GAME_DATA <0>
// WZD dseg:BD8E Game GAME_DATA <0>
// MGC dseg:87D4 Settings SETTINGS <0>
// WZD dseg:BD9E Settings SETTINGS <0>



long g_LbxFileSize;


int g_TIMER_Count_Lo = 0;                   // dseg:3CB8
int g_TIMER_Count_Hi = 0;                   // dseg:3CBA

// MoX_EXIT unsigned int g_MemFreeWorst_KB = 64000;         // dseg:3CC2  0xFA00  ? Bytes vs. Paragrphs ? 62.5 KB vs 4000 Pr ? sizeof VGA 320x200 ?
// MoX_EXIT char *cnst_Quit_Report1 = "Data";               // dseg:3CC4
// MoX_EXIT char *cnst_Quit_Report2 = " Free: ";            // dseg:3CC9
// MoX_EXIT char *cnst_Quit_Report3 = " bytes   Memory";    // dseg:3CD1
// MoX_EXIT char *cnst_Quit_Report4 = "k   Worst";          // dseg:3CE1
// MoX_EXIT char *cnst_Quit_Report5 = "k   EMM: ";          // dseg:3CEB
// MoX_EXIT char *cnst_Quit_Report6 = " blocks";            // dseg:3CF5


void * gfp_TBL_Moves_Per_Tile;      // dseg:5BAC            // loaded in ST_NEWG
unsigned int gsa_CRP_SND_Music_Segment; // dseg:5BB0
void * gfp_CRP_Visibility_Myrror;   // dseg:5BB2
void * gfp_CRP_Visibility_Arcanus;  // dseg:5BB6

unsigned int gsa_CRP_UnitDraw_WorkArea; // dseg:5E68

unsigned int gsa_GUI_SmallWork_IMG;     // dseg:61AC
unsigned int gsa_Sandbox;               // dseg:61AE

void * gfp_TBL_Events;              // dseg:61B8

void * gfp_TBL_Items;               // dseg:64B0
void * gfp_TBL_Premade_Items;       // dseg:64B4
// void * gfp_UU_IMG_USW_Items;     // dseg:64B8  ITEM_ICONS <0>
void * gfp_CRP_Active_Unit;         // dseg:65A0  contains a single battle unit record (110 bytes)
void * gfp_UU_TBL_BattleUnits;      // dseg:65A4
void * gfp_UU_TBL_CombatEnchants;   // dseg:65A8

void * gfp_TBL_HeroStats_P0; // dseg:65AC
void * gfp_TBL_HeroStats_P1; // dseg:65B0
void * gfp_TBL_HeroStats_P2; // dseg:65B4
void * gfp_TBL_HeroStats_P3; // dseg:65B8
void * gfp_TBL_HeroStats_P4; // dseg:65BC
void * gfp_TBL_HeroStats_P5; // dseg:65C0

void * gfp_TBL_Spell_Data;   // dseg:662C       // loaded in ST_NEWG

unsigned int g_World_Data_Paras;                // dseg:6750
unsigned int gsa_World_Data_Extra;              // dseg:6752
unsigned int gsa_Gfx_Swap;                      // dseg:6754
unsigned int gsa_World_Data;                    // dseg:6756

void * gfp_TBL_Units; // dseg:68F8
void * gfp_TBL_Hero_Names; // dseg:68FC

void * gfp_CRP_TBL_OvlMovePathsEMM; // dseg:89A6
// ? UU gfp EMM ? dseg:89AA
// ? UU gfp EMM ? dseg:89AE
void * gfp_CRP_TBL_Catchments_EMM; // dseg:89B2
void * gfp_CRP_TBL_SharedTiles_EMM; // dseg:89B6
void * gfp_CRP_TBL_TempMoveMap_EMM; // dseg:89BA
// extern void * UU_COL_MinimapTiles; // dseg:89BE
void * gfp_TBL_MoveMaps_EMM; // dseg:89C2
void * gfp_TBL_Scouting; // dseg:89C6
void * gfp_TBL_TerrainFlags; // dseg:89CA
void * gfp_TBL_Terr_Specials; // dseg:89CE
void * gfp_TBL_Cities; // dseg:89D2
 
void * gfp_TBL_Encounters;  // dseg:89D6
void * gfp_TBL_Towers;      // dseg:89DA
void * gfp_TBL_Fortresses;  // dseg:89DE
void * gfp_TBL_Nodes;       // dseg:89E2

void * gfp_TBL_Landmasses;  // dseg:89E6
void * gfp_UU_TBL_1;        // dseg:89EA
void * gfp_UU_TBL_2;        // dseg:89EE
void * gfp_TBL_Maps;        // dseg:89F2
