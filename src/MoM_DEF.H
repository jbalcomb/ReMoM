#ifndef MOM_DEF_H
#define MOM_DEF_H

/*

    Manifest Constants for Master of Magic (~v1.31)

        Specific to MoM, not MoX (SimTex *Game-Engine*)

*/

#define NUM_TOWERS        6
#define NUM_FORTRESSES    6




#define SCREEN_WIDTH    320
#define SCREEN_HEIGHT   200

#define SCREEN_XMIN       0
#define SCREEN_YMIN       0
#define SCREEN_XMAX     319
#define SCREEN_YMAX     199

#define CURSOR_WIDTH    16
#define CURSOR_HEIGHT   16

// ? /* Palette Index 0: TRANSPARENT */ ?
#define TRANSPARENT 0x00


/*
    World Map Dimensions
*/
#define WMAP_WIDTH      60  // in Squares
#define WMAP_HEIGHT     40  // in Squares
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

#define WORLD_SIZE          2400
#define WORLD_SIZE_DB       2400  // 1-byte values
#define WORLD_SIZE_DW       4800  // 2-byte values

#define WORLD_WIDTH         60  // in Squares
#define WORLD_HEIGHT        40  // in Squares

#define MAP_WIDTH           12  // in Squares
#define MAP_HEIGHT          10  // in Squares
#define MAP_WIDTH_HALF       6  // in Squares
#define MAP_HEIGHT_HALF      5  // in Squares

#define MAP_START_X          0  // in Pixels
#define MAP_START_Y         20  // in Pixels

#define TILE_WIDTH      20
#define TILE_HEIGHT     18

#define _MAP_MAX_X      60
#define _MAP_MAX_Y      40

#define TERRAIN_COUNT   762


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

#define ST_BLACK 1
#define ST_WHITE 15
*/


#endif /* MOM_DEF_H */
