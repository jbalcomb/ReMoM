#ifndef MOX_DEF_H
#define MOX_DEF_H


#ifndef STU_DEBUG
#define DLOG
#endif



#define VIDEO_MODE_MODE_Y
#define VIDEO_MODE       3  // vm_Mode_Y


#define ST_NULL          0
#define ST_TRUE          1
#define ST_FALSE         0
#define ST_UNDEFINED    -1
#define ST_SUCCESS      -1
#define ST_FAILURE       0


#define MOUSE_BUTTON_LEFT   1
#define MOUSE_BUTTON_RIGHT  2


// Define macro to swap two numbers
// #define SWAP(x, y, temp) temp = x; x = y; y = temp;  ... macro expansion(temp = x; x = y; y = temp;)
// x = x + y - ( y = x );
// #define SWAP(x, y, T) do { T SWAP = x; x = y; y = SWAP; } while (0)  ...  invoke: SWAP(a, b, int);
#define SWAP(x, y) (x ^= y ^= x ^= y)



#define HUMAN_PLAYER_IDX 0
#define NEUTRAL_PLAYER_IDX 5
#define NEUTRAL_PLAYER_BANNER_COLOR_IDX 5

// World (Grid) Dimensions, in Map Sqaures
#define WORLD_WIDTH         60
#define WORLD_HEIGHT        40
#define MOVEMENT_MAP_WIDTH  12
#define MOVEMENT_MAP_HEIGHT 10
#define REDUCED_MAP_WIDTH   58
#define REDUCED_MAP_HEIGTH  30

#define PLAYER_COUNT_MAX       6
#define CITY_COUNT_MAX       100
#define UNIT_COUNT_MAX      1009
#define TOWER_COUNT_MAX        6
#define FORTRESS_COUNT_MAX     6

#define MINIMAP_MARK_COLOR  15

#ifdef VIDEO_MODE_MODE_Y
#define SCREEN_WIDTH    320
#define SCREEN_HEIGHT   200
#define SCREEN_XMIN       0
#define SCREEN_YMIN       0
#define SCREEN_XMAX     319
#define SCREEN_YMAX     199
#define CURSOR_WIDTH     16
#define CURSOR_HEIGHT    16
#endif
#ifdef VIDEO_MODE_HI_RES
#define SCREEN_WIDTH    640
#define SCREEN_HEIGHT   400
#define SCREEN_XMIN       0
#define SCREEN_YMIN       0
#define SCREEN_XMAX     639
#define SCREEN_YMAX     399
#define CURSOR_WIDTH     32
#define CURSOR_HEIGHT    32
#endif
#ifdef VIDEO_MODE_HI_RES_HI_COL
#define SCREEN_WIDTH    640
#define SCREEN_HEIGHT   400
#define SCREEN_XMIN       0
#define SCREEN_YMIN       0
#define SCREEN_XMAX     639
#define SCREEN_YMAX     399
#define CURSOR_WIDTH     32
#define CURSOR_HEIGHT    32
#endif


#define ST_TRANSPARENT   0
#define ST_BLACK         1
#define ST_WHITE        15



#endif /* MOX_DEF_H */
