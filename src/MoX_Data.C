
#include "MoX_TYPE.H"
#include "MoX_DEF.H"
#include "MoM_DEF.H"

#include "MoX_Data.H"






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





// WZD dseg:291E                                                     BEGIN: ovr052
// WZD dseg:291E
// WZD dseg:291E                                                 Load/Init Main Game
// WZD dseg:291E
// WZD dseg:291E 4D 41 49 4E 2E 4C 42 58 00                      rsc01_MAIN_LBX db 'MAIN.LBX',0          ; DATA XREF: Load_Init_MainGame:loc_45FC8o
// WZD dseg:291E                                                                                         ; should use dseg:29ef (reused in cud thrown/breath,
// WZD dseg:291E                                                                                         ; with the first two bytes still free)
// WZD dseg:2927 42 41 43 4B 47 52 4E 44 2E 4C 42 58 00          rsc02_BACKGRND_LBX db 'BACKGRND.LBX',0  ; DATA XREF: Load_Init_MainGame:loc_45FD2o
// WZD dseg:2927                                                                                         ; should use dseg:29f4 (reused in cud thrown/breath)
// WZD dseg:2934 55 4E 49 54 53 31 2E 4C 42 58 00                rsc03_UNITS1_LBX db 'UNITS1.LBX',0      ; DATA XREF: Load_Init_MainGame:loc_45FDCo
// WZD dseg:2934                                                                                         ; should use dseg:2a1f (reused in cud thrown/breath)
// WZD dseg:293F 55 4E 49 54 53 32 2E 4C 42 58 00                rsc04_UNITS2_LBX db 'UNITS2.LBX',0      ; DATA XREF: Load_Init_MainGame+26o
// WZD dseg:293F                                                                                         ; should use dseg:2a26 (reused in cud thrown/breath)
// WZD dseg:294A 55 4E 49 54 56 49 45 57 2E 4C 42 58 00          rsc05_UNITVIEW_LBX db 'UNITVIEW.LBX',0  ; DATA XREF: Load_Init_MainGame+30o
// WZD dseg:294A                                                                                         ; should use dseg:2a59 (reused in cud thrown/breath)
// WZD dseg:2957 53 50 45 43 49 41 4C 2E 4C 42 58 00             rsc06_SPECIAL_LBX db 'SPECIAL.LBX',0    ; DATA XREF: Load_Init_MainGame+3Ao
// WZD dseg:2957                                                                                         ; should use dseg:2a62 (reused in cud thrown/breath)
// WZD dseg:2963 53 50 45 43 49 41 4C 32 2E 4C 42 58 00          rsc07_SPECIAL2_LBX db 'SPECIAL2.LBX',0  ; DATA XREF: Load_Init_MainGame+44o
// WZD dseg:2963                                                                                         ; should use dseg:2a6a (reused in cud thrown/breath)
// WZD dseg:2970 49 54 45 4D 53 2E 4C 42 58 00                   rsc08_ITEMS_LBX db 'ITEMS.LBX',0        ; DATA XREF: Load_Init_MainGame+4Eo
// WZD dseg:2970                                                                                         ; should use dseg:2a73 (reused in cud thrown/breath)
// WZD dseg:297A 43 48 52 49 56 45 52 2E 4C 42 58 00             rsc09_CHRIVER_LBX db 'CHRIVER.LBX',0    ; DATA XREF: Load_Init_MainGame+58o
// WZD dseg:297A                                                                                         ; should use dseg:2a3f (reused in cud thrown/breath)
// WZD dseg:2986 49 54 45 4D 49 53 43 2E 4C 42 58 00             rsc0A_ITEMISC_LBX db 'ITEMISC.LBX',0    ; DATA XREF: Load_Init_MainGame+62o
// WZD dseg:2986                                                                                         ; should use dseg:2a79 (first 9 bytes reused in cud
// WZD dseg:2986                                                                                         ; thrown/breath, last 3 in word of recall at circle)
// WZD dseg:2992 43 49 54 59 53 43 41 50 2E 4C 42 58 00          rsc0B_CITYSCAP_LBX db 'CITYSCAP.LBX',0  ; DATA XREF: Load_Init_MainGame+6Co
// WZD dseg:2992                                                                                         ; should use dseg:2a50 (reused in recall at circle)

// WZD dseg:299F                                                 Init_Terrain()
// WZD dseg:299F 4D 41 50 42 41 43 4B 2E 4C 42 58 00             rsc0C_MAPBACK_LBX db 'MAPBACK.LBX',0
char rsc0C_MAPBACK_LBX[] = "MAPBACK.LBX";   // ; should use dseg:29e7 (reused in recall at circle)

// WZD dseg:29AB 43 4D 42 4D 41 47 49 43 2E 4C 42 58 00          rsc0D_CMBMAGIC_LBX db 'CMBMAGIC.LBX',0  ; DATA XREF: Load_Init_MainGame+80o
// WZD dseg:29AB                                                                                         ; should use dseg:2a2d
// WZD dseg:29B8 43 4D 42 54 43 49 54 59 2E 4C 42 58 00          rsc0E_CMBTCITY_LBX db 'CMBTCITY.LBX',0  ; DATA XREF: Load_Init_MainGame+8Ao
// WZD dseg:29B8                                                                                         ; should use dseg:2a36
// WZD dseg:29C5 43 49 54 59 57 41 4C 4C 2E 4C 42 58 00          rsc0F_CITYWALL_LBX db 'CITYWALL.LBX',0  ; DATA XREF: Load_Init_MainGame+94o
// WZD dseg:29C5                                                                                         ; should use dseg:2a47
// WZD dseg:29D2                                                 Init_Terrain()
// WZD dseg:29D2 54 45 52 52 41 49 4E 2E 4C 42 58 00             terrain_lbx_file db 'TERRAIN.LBX',0     ; DATA XREF: Terrain_Init+Bo ...
// WZD dseg:29DE 54 45 52 52 53 54 41 54 00                      terrstat_lbx_file db 'TERRSTAT',0       ; DATA XREF: Terrain_Init+6Fo
// WZD dseg:29E7 4D 41 50 42 41 43 4B 00                         mapback_lbx_file db 'MAPBACK',0         ; DATA XREF: Terrain_Init+E4o ...





// WZD dseg:599C
// TODO  char hlpentry_lbx_file[] = "hlpentry";



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




// WZD dseg:9136
SAMB_ptr TBL_Items;
// WZD dseg:913A
SAMB_ptr TBL_Premade_Items;
// WZD dseg:913E
// IMG_USW_Items ITEM_ICONS
// WZD dseg:9226
SAMB_ptr Active_Unit;
// WZD dseg:922A
// TBL_BattleUnits
// WZD dseg:922E
// TBL_CombatEnchants


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
uint8_t * square_scouted_p0;                // Bit_Field  alloc in MoM_Tables_Init()
// WZD dseg:9396
// drake178: Visibility_Arcanus
uint8_t * square_scouted_p1;                // Bit_Field  alloc in MoM_Tables_Init()


/* -2: NEVER, -1: ALWAYS, {0,1,2,3}: frame - draw off, {4,5,6,7}: frame - draw on */
// ? Set_Draw_Active_Stack_...() means this variable is private to ovr067 ?
// WZD dseg:9442
int16_t draw_active_stack_flag;  // AKA Active_Stack_Draw

// WZD dseg:9444
int16_t all_units_moved;


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
uint8_t * _events_table;  // alloc in MoM_Tables_Init()  7 PR 112 B  type? 1-byte,2-byte signed,unsigned 112 events or 56 events? Hrrm... all the indexing is in evens, so 2-byte?

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
SAMB_ptr TBL_SharedTiles_EMS;
// WZD dseg:9CA4
SAMB_ptr TBL_TempMoveMap_EMS;
// WZD dseg:9CA8
// COL_MinimapTiles
// WZD dseg:9CAC
SAMB_ptr TBL_MoveMaps_EMS;


// WZD dseg:9CB0
/*
    302 Paragraphs, 4832 Bytes
    4800 bytes used - 1-byte value, 2400 world squares, 2 planes
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
    alloc in MoM_Tables_Init()
    load in Load_SAVE_GAM() - read 102 of 24

*/
struct s_LAIR * TBL_Lairs;

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
// SAMB_ptr TBL_Maps;
SAMB_ptr _world_maps;


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
// struct s_HLP_ENTRY _help_entries[50];
uint8_t _help_entries[500];


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
int16_t OVL_Map_CenterY;  // AKA _active_world_y
// WZD dseg:BD84 
int16_t OVL_Map_CenterX;  // AKA _active_world_x
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
int16_t _units;
int16_t _cities;
int16_t _difficulty;
int16_t _magic;
int16_t _landsize;
int16_t _num_players;  // New Game: magic_set.opponents + 1
struct s_GAME_DATA game_data;
// WZD dseg:BD9E
// magic_set s_MAGIC_SET

// ovr150 MainScr_Maps  // WZD dseg:CB5C
// ovr150 MainScr_Maps  // AKA OVL_NewMapDrawing
// ovr150 MainScr_Maps  int16_t draw_map_full;  //; determines whether non-animated terrain tiles will be redrawn or not
