#ifndef MOM_DEF_H
#define MOM_DEF_H


#define EMM_PAGES_REQUIRED 158
#define EMM_MIN_KB 2700  // 168 pages ? 168 = 158 + (1 + 5 + 4) ?
#define RAM_MIN_KB 583

#define MOM_FONT_FILE "FONTS.LBX"
#define GAME_FONT_FILE MOM_FONT_FILE

#define INPUT_TYPE_KEYBOARD_AND_MOUSE 1


struct CONFIG_DATA //; (sizeof=0x12, standard type)
{
    int M_IO;
    int M_HW;
    int M_IRQ;
    int D_IO;
    int D_HW;
    int D_IRQ;
    int D_DMA;
    int Unused_0E;
    int Unused_10;
};

struct GAME_DATA //; (sizeof=0x10, standard type)
{
    int Current_Unit;
    int Turn;
    int Total_Units;
    int Total_Cities;
    int Difficulty;
    int Magic;
    int Land_Size;
    int Total_Wizards;
};

typedef struct
    {
        char name[20];
    } SaveNameStruct;

typedef struct
    {
        int SoundFX;
        int BG_Music;
        int Event_Music;
        int City_Spell_Events;
        int Ovl_Spell_Events;
        int Summoning_Events;
        int EoT_Summary;
        int Raze_City;
        int Random_Events;
        int EoT_Wait;
        int Strategic_Combat;
        int Auto_Unit_Info;
        int Enemy_Moves;
        int Enemy_Spells;
        int Spell_Book_Order;
        int Spell_Anims;
        int Show_Node_Owners;
        int Expanding_Help;
        int SoundChannels;
        int InputType;
        int Difficulty;
        int Opponents;
        int LandSize;
        int MagicPower;
        int Using_Saves[8];
        SaveNameStruct Save_Names[8];
        int Movement_Anims;
        SaveNameStruct HallofFame_Names[10];
        int HallofFame_Scores[10];
        int HallofFame_Races[10];
    } SettingsStruct;

extern SettingsStruct Settings;



/*
    seg001, same in MGC & WZD
*/
// MGC dseg:27FF
// WZD dseg:2881
extern char cnst_ConfigFile[];
// MGC dseg:280A
// WZD dseg:288C
extern char cnst_ConfigErr[];
// MGC dseg:2836
// WZD dseg:28B8
extern char cnst_RB[];
// MGC dseg:2839
// WZD dseg:28BB
extern char cnst_MIDI_Init[];
// MGC dseg:2859
// WZD dseg:28DB
extern char cnst_SAVE[];
// MGC dseg:285E
// WZD dseg:28E0
extern char cnst_SAVE_ext[];
// MGC dseg:2863
// WZD dseg:28E5
extern char cnst_Set_File[];
// MGC dseg:286D
// WZD dseg:28EF
extern char cnst_WB[];
// MGC dseg:2870
// WZD dseg:28F2
extern char cnst_FONT_File[];


// dseg:28E4 cnst_SAVE2 db 'SAVE',0
// dseg:2904 53 41 56 45 00                cnst_SAVE2 db 'SAVE',0
// dseg:28E9 cnst_SAVE_ext2 db '.GAM',0
// dseg:2909 2E 47 41 4D 00                cnst_SAVE_ext2 db '.GAM',0
// dseg:28EE cnst_WB2 db 'wb',0
// dseg:290E 77 62 00                      cnst_WB2 db 'wb',0
// dseg:28F1 cnst_Set_File2 db 'MAGIC.SET',0
// dseg:2911 4D 41 47 49 43 2E 53 45 54 00 cnst_Set_File2 db 'MAGIC.SET',0
// dseg:28FB cnst_RB2 db 'rb',0
// dseg:291B 72 62 00                      cnst_RB2 db 'rb',0


// MGC dseg:87A4 Config_Data CONFIG_DATA <0>
// WZD dseg:BD6E Config_Data CONFIG_DATA <0>
//extern struct CONFIG_DATA Config_Data;
extern int Config_Data[];
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



extern long g_LbxFileSize;


extern int g_TIMER_Count_Lo;                // dseg:3CB8
extern int g_TIMER_Count_Hi;                // dseg:3CBA

// MoX_EXIT  extern unsigned int g_MemFreeWorst_KB;      // dseg:3CC2
// MoX_EXIT  extern char *cnst_Quit_Report1;             // dseg:3CC4
// MoX_EXIT  extern char *cnst_Quit_Report2;             // dseg:3CC9
// MoX_EXIT  extern char *cnst_Quit_Report3;             // dseg:3CD1
// MoX_EXIT  extern char *cnst_Quit_Report4;             // dseg:3CE1
// MoX_EXIT  extern char *cnst_Quit_Report5;             // dseg:3CEB
// MoX_EXIT  extern char *cnst_Quit_Report6;             // dseg:3CF5


extern void * gfp_TBL_Moves_Per_Tile;       // dseg:5BAC
extern unsigned int gsa_CRP_SND_Music_Segment;  // dseg:5BB0
extern void * gfp_CRP_Visibility_Myrror;    // dseg:5BB2
extern void * gfp_CRP_Visibility_Arcanus;   // dseg:5BB6

extern unsigned int gsa_CRP_UnitDraw_WorkArea; // dseg:5E68

extern unsigned int gsa_GUI_SmallWork_IMG;  // dseg:61AC
extern unsigned int gsa_Sandbox;            // dseg:61AE

extern void * gfp_TBL_Events;           // dseg:61B8

extern void * gfp_TBL_Items;                // dseg:64B0
extern void * gfp_TBL_Premade_Items;        // dseg:64B4
// extern void * gfp_UU_IMG_USW_Items;      // dseg:64B8  ITEM_ICONS <0>
extern void * gfp_CRP_Active_Unit;          // dseg:65A0  contains a single battle unit record (110 bytes)
extern void * gfp_UU_TBL_BattleUnits;       // dseg:65A4
extern void * gfp_UU_TBL_CombatEnchants;    // dseg:65A8

extern void * gfp_TBL_HeroStats_P0; // dseg:65AC
extern void * gfp_TBL_HeroStats_P1; // dseg:65B0
extern void * gfp_TBL_HeroStats_P2; // dseg:65B4
extern void * gfp_TBL_HeroStats_P3; // dseg:65B8
extern void * gfp_TBL_HeroStats_P4; // dseg:65BC
extern void * gfp_TBL_HeroStats_P5; // dseg:65C0

extern void * gfp_TBL_Spell_Data;   // dseg:662C

extern unsigned int g_World_Data_Paras;     // dseg:6750
extern unsigned int gsa_World_Data_Extra;   // dseg:6752
extern unsigned int gsa_Gfx_Swap;           // dseg:6754
extern unsigned int gsa_World_Data;         // dseg:6756

extern void * gfp_TBL_Units;            // dseg:68F8
extern void * gfp_TBL_Hero_Names;       // dseg:68FC

extern void * gfp_CRP_TBL_OvlMovePathsEMM; // dseg:89A6
// ? UU gfp EMM ? dseg:89AA
// ? UU gfp EMM ? dseg:89AE
extern void * gfp_CRP_TBL_Catchments_EMM; // dseg:89B2
extern void * gfp_CRP_TBL_SharedTiles_EMM; // dseg:89B6
extern void * gfp_CRP_TBL_TempMoveMap_EMM; // dseg:89BA
// extern void * UU_COL_MinimapTiles; // dseg:89BE
extern void * gfp_TBL_MoveMaps_EMM; // dseg:89C2
extern void * gfp_TBL_Scouting; // dseg:89C6
extern void * gfp_TBL_TerrainFlags; // dseg:89CA
extern void * gfp_TBL_Terr_Specials; // dseg:89CE
extern void * gfp_TBL_Cities; // dseg:89D2

extern void * gfp_TBL_Encounters;   // dseg:89D6
extern void * gfp_TBL_Towers;       // dseg:89DA
extern void * gfp_TBL_Fortresses;   // dseg:89DE
extern void * gfp_TBL_Nodes;        // dseg:89E2
extern void * gfp_TBL_Landmasses;   // dseg:89E6
extern void * gfp_UU_TBL_1;         // dseg:89EA
extern void * gfp_UU_TBL_2;         // dseg:89EE
extern void * gfp_TBL_Maps;         // dseg:89F2



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

/*
    seg002
*/

// _s02p01.asm
void CLK_SaveCounter(void);
// _s02p03.asm
void CLK_Wait(int nticks);
// _s02p05.asm
void VGA_WaitSync(void);


/*
    seg003
*/

// _s03p05
long DOS_GetFileSize(char *fname);
//unsigned int C_GetFileSize(const char *filename);


/*
    seg005
*/

// _s05p01c.c
void Quit_MemBug(void);
// _s05p02c.c
void Quit(char *quitmsg);
// _s05p05c.c
void Update_MemFreeWorst_KB(void);

/*
    seg006
*/

// _s06p01c.c
void VGA_SetTextMode(void);
// _s06p02c.c
void QuitToDOS(char *quitmsg);
// _s06p04
unsigned int dos_memfree_kb(void);


/*
    ovr057
*/

// _o57p01
void MoM_Tables_Init(int gfx_buff_nparas);


#ifdef __cplusplus
}
#endif


#endif /* MOM_DEF_H */
