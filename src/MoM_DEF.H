#ifndef MOM_DEF_H
#define MOM_DEF_H




/*
    Experience Level
*/
/*
    Normal Unit
        Experience Level    Ring Symbol
        Recruit             1 Silver
        Regular             2 Silver
        Veteran             3 Silver
        Elite               1 Gold
        Ulta-Elite          2 Gold
        Champion
    Hero Unit
        Hero
        Myrmidon            1 Silver
        Captain             2 Silver
        Commander           3 Silver
        Champion            1 Gold
        Lord                2 Gold
        Grand Lord          3 Gold
        Super Hero          1 Red
        Demi-God            2 Red
*/



/*

    Manifest Constants for Master of Magic (~v1.31)

        Specific to MoM, not MoX (SimTex *Game-Engine*)

*/

#define NUM_MAGIC_TYPES        5

// MoO2: _NUM_SHIPS, _NUM_COLONIES, _NUM_PLAYERS, _NUM_STARS, _NUM_NEBULAS
#define PLANE_COUNT_MAX        2
#define PLAYER_COUNT_MAX       6
#define CITY_COUNT_MAX       100
#define UNIT_COUNT_MAX      1009
#define TOWER_COUNT_MAX        6
#define FORTRESS_COUNT_MAX     6
// #define NODE_COUNT_MAX        30
// #define LAIR_COUNT_MAX       102
#define NUM_PLANES             2
#define NUM_PLAYERS            6
#define NUM_CITIES           100
#define NUM_UNITS           1009
#define NUM_HEROES             6  // "Hero Slots"
#define NUM_TOWERS             6
#define NUM_FORTRESSES         6
#define NUM_NODES             30
#define NUM_LAIRS            102

#define SCREEN_WIDTH    320
#define SCREEN_HEIGHT   200

#define SCREEN_XMIN       0
#define SCREEN_YMIN       0
#define SCREEN_XMAX     319
#define SCREEN_YMAX     199

#define CURSOR_WIDTH    16
#define CURSOR_HEIGHT   16



// Banner Color Remapping
// 5 Banners (banner_idx)
// Units - 5 color indices
// 214 + {0,1,2,3,4} = {214,215,216,218,219}
// Different than array remap colors for other things - e.g., Towers(Owned,Unowned)
//     Draw_Unit_StatFig()
//         FLIC_Remap_Color(UnitDraw_WorkArea, itr_color_remap + 214, COL_Banners[ ((banner_idx * 5) + itr_color_remap) ]);

//     Draw_Unit_StatFig()
//         if(UNIT_HasInvisibility(unit_idx) == ST_TRUE)
//             Replace_Color_All(UnitDraw_WorkArea, 233);
//             Outline_Picture_Pixels(UnitDraw_WorkArea, 1);

// MainScr_Maps.C
// reduced_map_box_color = 13;
// MoX_DEF.H
// #define MINIMAP_MARK_COLOR 15

// LILWIZ.LBX
// WIZARDS.LBX
// ¿ order of wizards ?
// ¿ order of colors ?
/*
LILWIZ.LBX
    blue
    green
    purple
    red
    yellow
*/
/*
LILWIZ.LBX
    MERLE2
    SHAMAN2
    VOODOO2
    EASTERN2
    ARAB2
    WHITEH2
    REDEYES2
    LIZARD2
    BARON2
    BLOND2
    GALEN2
    HEDBAND2
    AZTEC2
    WITCH2
*/
/*
WIZARDS.LBX
    MERLIN
    SHAMAN
    PRIESTES
    WU
    ARAB
    OBERIC
    WRAITH
    DRACONIA
    EVILWIZ
    FREYA
    GALE
    ARIEL
    AZTEC
    KARLA
*/


/*
    World Map Dimensions
*/
// #define WMAP_WIDTH      60  // in Squares
// #define WMAP_HEIGHT     40  // in Squares
/*
    Main Screen Movement Map Dimensions
*/
#define MMAP_WIDTH      12  // in Squares
#define MMAP_HEIGHT     10  // in Squares
/*
    Main Screen Reduced Map Dimensions
*/
#define RMAP_WIDTH      58  // in Squares
#define RMAP_HEIGHT     30  // in Squares
/*
    World / Movement-Map Square Dimensions
*/
#define SQUARE_WIDTH    20  // in Pixels
#define SQUARE_HEIGHT   18  // in Pixels
#define TILE_WIDTH      20
#define TILE_HEIGHT     18

#define WORLD_WIDTH         60  // in Squares
#define WORLD_HEIGHT        40  // in Squares
#define _MAP_MAX_X      60
#define _MAP_MAX_Y      40
#define WORLD_SIZE          2400
#define WORLD_SIZE_DB       2400  // 1-byte values
#define WORLD_SIZE_DW       4800  // 2-byte values


#define MAP_WIDTH           12  // in Squares
#define MAP_HEIGHT          10  // in Squares
#define MAP_SIZE           120  // in Squares

#define MAP_WIDTH_HALF       6  // in Squares
#define MAP_HEIGHT_HALF      5  // in Squares

#define MAP_SCREEN_X                  0  // in Pixels
#define MAP_SCREEN_Y                 20  // in Pixels

#define REDUCED_MAP_SCREEN_X        251  // in Pixels
#define REDUCED_MAP_SCREEN_Y         21  // in Pixels
#define REDUCED_MAP_SQUARE_WIDTH      1  // in Pixels
#define REDUCED_MAP_SQUARE_HEIGHT     1  // in Pixels

#define MOVEMENT_MAP_SCREEN_X         0
#define MOVEMENT_MAP_SCREEN_Y        20
#define REDUCED_MAP_SCREEN_X        251
#define REDUCED_MAP_SCREEN_Y         21

#define REDUCED_MAP_X   251
#define REDUCED_MAP_Y    21
#define REDUCED_MAP_W    58
#define REDUCED_MAP_H    30

#define ARMYLIST_REDUCED_MAP_X    86
#define ARMYLIST_REDUCED_MAP_Y   164
#define ARMYLIST_REDUCED_MAP_W    49
#define ARMYLIST_REDUCED_MAP_H    33

#define CITYLIST_REDUCED_MAP_X    42
#define CITYLIST_REDUCED_MAP_Y   162
#define CITYLIST_REDUCED_MAP_W    49
#define CITYLIST_REDUCED_MAP_H    33


#define TERRAIN_COUNT   762
#define TERRAIN_TYPE_COUNT   762

#define STATFIG_WIDTH       18  // in Pixels
#define STATFIG_HEIGHT      16  // in Pixels

/*
hanging out in MoX_DEF.H


#define HUMAN_PLAYER_IDX 0
#define NEUTRAL_PLAYER_IDX 5

// World (Grid) Dimensions, in Map Sqaures
#define WORLD_WIDTH 60
#define WORLD_HEIGHT 40
#define MOVEMENT_MAP_WIDTH 12
#define MOVEMENT_MAP_HEIGHT 10
#define REDUCED_MAP_WIDTH 58
#define REDUCED_MAP_HEIGTH 30

#define PLAYER_COUNT_MAX 6
#define CITY_COUNT_MAX 100
#define UNIT_COUNT_MAX 1009
#define TOWER_COUNT_MAX 6
#define FORTRESS_COUNT_MAX 6

#define MINIMAP_MARK_COLOR 15

*/



/*
1-byte
0b00000000  0x0     0
0b00000001  0x1     1
0b00000010  0x2     2
0b00000100  0x4     4
0b00001000  0x8     8
0b00010000  0x10   16
0b00100000  0x20   32
0b01000000  0x40   64
0b10000000  0x80  128
2-byte
0b0000000000000000  0x0000      0
0b0000000000000001  0x0001      1
0b0000000000000010  0x0002      2
0b0000000000000100  0x0004      4
0b0000000000001000  0x0008      8
0b0000000000010000  0x0010     16
0b0000000000100000  0x0020     32
0b0000000001000000  0x0040     64
0b0000000010000000  0x0080    128
0b0000000100000000  0x0100    256
0b0000001000000000  0x0200    512
0b0000010000000000  0x0400   1024
0b0000100000000000  0x0800   2048
0b0001000000000000  0x1000   4096
0b0010000000000000  0x2000   8192
0b0100000000000000  0x4000  16384
0b1000000000000000  0x8000  32768
*/

/*
    Movement Modes
*/
#define CAVALRY         0x0001
#define SAILING         0x0002
#define SWIMMING        0x0004
#define FLYING          0x0008
#define FORESTER        0x0020
#define MOUNTAINEER     0x0040
// "Planar Travel" -000A Move_PlaneShift dw ?
// "Wind Walking"  -0008 Move_WindWalk dw ?
// "walking"       -0006 Move_Ground dw ?
/* ¿ array / enum offsets ?*/
// #define OFS_CAVALRY          0x00
// #define OFS_FORESTER         0x02
// #define OFS_MOUNTAINEER      0x04
// #define OFS_SWIMMING         0x06
// #define OFS_SAILING          0x08
// #define OFS_FLYING           0x0A
// #define OFS_PLANARTRAVEL     0x0C
// #define OFS_WALKING          0x0E
#define OFS_CAVALRY          1
#define OFS_FORESTER         2
#define OFS_MOUNTAINEER      3
#define OFS_SWIMMING         4
#define OFS_SAILING          5
#define OFS_FLYING           6
#define OFS_PLANARTRAVEL     7
#define OFS_WALKING          8

// MM: Movement Mode
#define MM_CAVALRY          0
#define MM_FORESTER         1
#define MM_MOUNTAINEER      2
#define MM_SWIMMING         3
#define MM_SAILING          4
#define MM_FLYING           5
#define MM_PLANARTRAVEL     6
#define MM_WALKING          7


/*
    Unit Abilities
*/
// 0b0000000000000000  0x0000      0
// 0b0000000000000001  0x0001      1
// 0b0000000000000010  0x0002      2
#define UA_PLANARTRAVEL     0x04        // 0b0000000000000100  0x0004      4
// 0b0000000000001000  0x0008      8
// 0b0000000000010000  0x0010     16
// 0b0000000000100000  0x0020     32
#define UA_INVISIBILITY     0x40        // 0b0000000001000000  0x0040     64
// 0b0000000010000000  0x0080    128
// 0b0000000100000000  0x0100    256
// 0b0000001000000000  0x0200    512
// 0b0000010000000000  0x0400   1024
// 0b0000100000000000  0x0800   2048
#define UA_WINDWALKING      0x1000      // 0b0001000000000000  0x1000   4096
// 0b0010000000000000  0x2000   8192
// 0b0100000000000000  0x4000  16384
// 0b1000000000000000  0x8000  32768


/*
    Unit Enchantments
*/
// 0b0000000000000000  0x0000      0
#define UE_WINDWALKING     0x0001      // 0b0000000000000001  0x0001      1
// 0b0000000000000010  0x0002      2
// 0b0000000000000100  0x0004      4
// 0b0000000000001000  0x0008      8
// 0b0000000000010000  0x0010     16
// 0b0000000000100000  0x0020     32
// 0b0000000001000000  0x0040     64
// 0b0000000010000000  0x0080    128
// 0b0000000100000000  0x0100    256
// 0b0000001000000000  0x0200    512
// 0b0000010000000000  0x0400   1024
// 0b0000100000000000  0x0800   2048
#define UE_PLANARTRAVEL    0x1000       // 0b0001000000000000  0x1000   4096
// 0b0010000000000000  0x2000   8192
// 0b0100000000000000  0x4000  16384
#define UE_INVISIBILITY     0x8000      // 0b1000000000000000  0x8000  32768


/*
    Unit Mutations
*/

/*
    Item Enchantments / Attributes / Effects
*/



/*
    Screen Layout Variables
*/

/*
    Army List Screen
*/
#define NUM_ARMY_LIST   6
#define NUM_HERO_PORTRAIT   6
/*
    City List Screen
*/
#define NUM_CITY_LIST   9

/*
    Main Screen
        Game Buttons
        Movement Map
        Reduced Map
        Status Window
        Summary Window


Status Window
    Print_Integer_Right(265, 68, _players[_human_player_idx].gold_reserve);
    Print_Integer_Right(303, 68, _players[_human_player_idx].mana_reserve);
    Print(268, 68, cstr_GP);
    Print(306, 68, cstr_MP);

Summary Window
    FLIC_Draw(240, 76, deselect_background);
    Print_Integer_Right(273, 101, gold);
    Print(278, 101, cstr_Gold);
    Print_Integer_Right(273, 133, food);
    Print(278, 133, cstr_Food);
    Print_Integer_Right(273, 165, mana);
    Print(278, 165, cstr_Mana);
    Print_Centered(280, 157, cstr_Conjunction);
    Print_Centered(280, 158, cstr_Conjunction);
    Print_Centered(280, 158, cstr_Conjunction);
    Print_Centered(280, 158, cstr_Bad_Moon);
    Print_Centered(280, 158, cstr_Good_Moon);
    Print_Centered(280, 158, cstr_Mana_Short);

Next Turn Button
    FLIC_Draw(240, 173, next_turn_background_seg);
    FLIC_Draw(246, 178, next_turn_button_seg);  // ? offset by 6,5 to support the 3-D illusion of being depressed

Main_Screen_Draw_Unit_Action_Buttons()
    push    [main_done_button]      
    mov     ax, 176
    mov     ax, 246
    push    [main_patrol_button]    
    mov     ax, 176
    mov     ax, 280
    push    [main_wait_button]      
    mov     ax, 186
    mov     ax, 246

    switch(special_action_flag)
    push    [main_purify_button]    
    mov     ax, 186
    mov     ax, 280
    push    [main_meld_button]      
    mov     ax, 186
    mov     ax, 280
    push    [main_build_button]     
    mov     ax, 186
    mov     ax, 280
    push    [main_lock_build_button]
    mov     ax, 186
    mov     ax, 280

    push    [main_lock_done_button]
    mov     ax, 176
    mov     ax, 246
    push    [main_lock_patrol_button]
    mov     ax, 176
    mov     ax, 280
    push    [main_lock_build_button]
    mov     ax, 186
    mov     ax, 280
    push    [main_wait_button]      
    mov     ax, 186
    mov     ax, 246
*/



#endif /* MOM_DEF_H */
