
#include "MoX_TYPE.H"
#include "MoX_DEF.H"

#include "MoX_Data.H"



// WZD dseg:6FFE
int16_t Map_LastDraw_X = ST_UNDEFINED;
// WZD dseg:7000
int16_t Map_LastDraw_Y = ST_UNDEFINED;
// WZD dseg:7002
int16_t OVL_NewMapSustain = 2;


// WZD dseg:9136
SAMB_ptr TBL_Items;
// WZD dseg:913A
SAMB_ptr TBL_Premade_Items;
// WZD dseg:913E
// IMG_USW_Items ITEM_ICONS
// WZD dseg:9226
// Active_Unit
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


// WZD dseg:9392
SAMB_ptr Visibility_Myrror;
// WZD dseg:9396
SAMB_ptr Visibility_Arcanus;


// WZD dseg:9444
int16_t all_units_moved;



// WZD dseg:9648
SAMB_ptr UnitDraw_WorkArea;  // alloc in MoM_Init_Tables(), 60 PR, 960 DB



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
SAMB_ptr _events_table;  // AKA TBL_Events;

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
SAMB_ptr TBL_Scouting;
// WZD dseg:9CB4  
SAMB_ptr TBL_Terrain_Flags;
// WZD dseg:9CB8  
SAMB_ptr TBL_Terr_Specials;
// WZD dseg:9CBC  
// SAMB_ptr _CITIES;
// struct s_CITY _CITIES[CITY_COUNT_MAX];  // 100 * sizeof(114)
struct s_CITY * _CITIES;

// WZD dseg:9CC0  
SAMB_ptr TBL_Encounters;

// WZD dseg:9CC4  
// SAMB_ptr TBL_Towers;
// struct s_TOWER _TOWERS[TOWER_COUNT_MAX];  // 6 * sizeof(4)
// struct s_TOWER _TOWERS[];
struct s_TOWER * _TOWERS;

// WZD dseg:9CC8  
// SAMB_ptr TBL_Fortresses;
struct s_FORTRESS * _FORTRESSES;

// WZD dseg:9CCC  
SAMB_ptr TBL_Nodes;
// WZD dseg:9CD0
SAMB_ptr TBL_Landmasses;  // 12Eh 302d PR 302*16=4832, in World_Data
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


// dseg:BD82 _active_world_y dw 0                    ; DATA XREF: GAME_Overland_Init+127w ...
// dseg:BD84 _active_world_x dw 0                    ; DATA XREF: GAME_Overland_Init+11Bw ...
// dseg:BD86 _world_plane dw 0                       ; DATA XREF: GAME_Overland_Init+13Fw ...
// dseg:BD88 _curr_world_y dw 0                      ; DATA XREF: GAME_Overland_Init+FDw ...
// dseg:BD8A _curr_world_x dw 0                      ; DATA XREF: GAME_Overland_Init+F7w ...
// dseg:BD8C _human_player_idx dw 0                  ; DATA XREF: WZD_Startup_MainGame+Aw ...

// WZD dseg:BD82 
int16_t OVL_Map_CenterY;
// WZD dseg:BD84 
int16_t OVL_Map_CenterX;
// WZD dseg:BD86 
int16_t _map_plane;
// WZD dseg:BD88 
int16_t _map_y;
// WZD dseg:BD8A 
int16_t _map_x;

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

// WZD dseg:CB5C
int16_t OVL_NewMapDrawing;  //; determines whether non-animated terrain tiles will be redrawn or not
