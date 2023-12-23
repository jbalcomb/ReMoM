#ifndef MOX_DEF_H
#define MOX_DEF_H


#ifndef STU_DEBUG
#define DLOG
#endif



#define VIDEO_MODE_MODE_Y
// #define VIDEO_MODE_MODE_Y_2x
// #define VIDEO_MODE_MODE_Y_2x_XBGR
#define VIDEO_MODE       3  // vm_Mode_Y
// #define VIDEO_MODE       5  // vm_Mode_Y_2x
// #define VIDEO_MODE       6  // vm_Mode_Y_2x_XBGR



#define ST_FAILURE       0
#define ST_SUCCESS      -1

#define ST_FALSE         0
#define ST_TRUE          1

#define ST_NULL          0

#define ST_NONE          0
#define ST_UNDEFINED    -1  /* ¿ "Empty" ? (// Army List Screen - "Empty Hero Slot") */
/* Elsewhere, "NOT IN USE" ¿ for Fields ? ¿ Debug for outline of fields on screen */



#define MOUSE_BUTTON_LEFT   1
#define MOUSE_BUTTON_RIGHT  2

#define INVALID_FIELD       -1000


#define AbsVal(_val_) ( (_val_) < 0 ? (-(_val_)) : (_val_) )

// Define macro to swap two numbers
// #define SWAP(x, y, temp) temp = x; x = y; y = temp;  ... macro expansion(temp = x; x = y; y = temp;)
// x = x + y - ( y = x );
// #define SWAP(x, y, T) do { T SWAP = x; x = y; y = SWAP; } while (0)  ...  invoke: SWAP(a, b, int);
#define SWAP(x, y) (x ^= y ^= x ^= y)


/*
    Macros to translate coordinates from/to 'Movement Map', 'World Map', 'Reduced Map'
        ...
        ¿ including main_map_grid ?
        ¿ including reduced_map_grid ?
    
    MMAPX2WMAPX vs. MMX2WMX vs. MX2WX
    MMAPY2WMAPY vs. MMY2WMY vs. MY2WY
    ¿ congnitive effort to visually differentiate between MMAP, WMAP, RMAP, given the one character difference ?
    ¿ congnitive effort to eschew 'mx' as 'Mouse X', given the generally accepted practice in the industry ?
    ¿ lazy cunts and thier retarded concern for typing less characters ?

*/
#define MMAPX2WMAPX(_mmap_x_) { ( (_map_x + _main_map_grid_x) / WORLD_WIDTH ) }
#define MMAPY2WMAPY(_mmap_y_) { ( (_map_y + _main_map_grid_y) / WORLD_HEIGHT ) }
#define MMX2WMX(_mmx_) { ( (_map_x + _main_map_grid_x) / WORLD_WIDTH  ) }
#define MMY2WMY(_mmy_) { ( (_map_y + _main_map_grid_y) / WORLD_HEIGHT  ) }
#define MX2WX(_mx_) { ( (_map_x + _main_map_grid_x) / WORLD_WIDTH  ) }
#define MY2WY(_my_) { ( (_map_y + _main_map_grid_y) / WORLD_HEIGHT  ) }


#define CITYX() ( _CITIES[city_idx].wx     )
#define CITYY() ( _CITIES[city_idx].wy     )
#define CITYP() ( _CITIES[city_idx].wp )



// something or other for color difference/distance
#define REMAP_THRESHOLD  21



#define HUMAN_PLAYER_IDX 0
#define NEUTRAL_PLAYER_IDX 5
#define NEUTRAL_PLAYER_BANNER_COLOR_IDX 5

// World (Grid) Dimensions, in Map Sqaures
#define WORLD_WIDTH         60
#define WORLD_HEIGHT        40
#define MAP_WIDTH           12
#define MAP_HEIGHT          10
#define MOVEMENT_MAP_WIDTH  12
#define MOVEMENT_MAP_HEIGHT 10
#define REDUCED_MAP_WIDTH   58
#define REDUCED_MAP_HEIGHT  30
/*
    World / Movement-Map Square Dimensions
*/
#define SQUARE_WIDTH        20  // in Pixels
#define SQUARE_HEIGHT       18  // in Pixels


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
#ifdef VIDEO_MODE_MODE_Y_2x
#define SCREEN_WIDTH    640
#define SCREEN_HEIGHT   400
#define SCREEN_XMIN       0
#define SCREEN_YMIN       0
#define SCREEN_XMAX     639
#define SCREEN_YMAX     399
#define CURSOR_WIDTH     32
#define CURSOR_HEIGHT    32
#endif
#ifdef VIDEO_MODE_MODE_Y_2x_XBGR
#define SCREEN_WIDTH    640
#define SCREEN_HEIGHT   400
#define SCREEN_XMIN       0
#define SCREEN_YMIN       0
#define SCREEN_XMAX     639
#define SCREEN_YMAX     399
#define CURSOR_WIDTH     32
#define CURSOR_HEIGHT    32
#endif


/*
    SimTex Colors
*/

#define MINIMAP_MARK_COLOR  15
#define REDUCED_MAP_BOX_COLOR   13

// Unit_Window_Draw_Unit_Attributes()
#define DAMAGE_BAR_GREEN        0xD9  // 217
#define DAMAGE_BAR_YELLOW       0xD3  // 211
#define DAMAGE_BAR_RED          0xCA  // 202

/* ¿ Normal vs. Hero ? */
#define EXPERIENCE_LEVEL_SILVER     
#define EXPERIENCE_LEVEL_GOLD       
#define EXPERIENCE_LEVEL_RED        

#define ST_TRANSPARENT       0  /*  */
#define ST_BLACK             1  /*  */

#define ST_GRAY              8

#define ST_LIGHT_MAGENTA    13

#define ST_WHITE            15
// "silver" used for experience level rings/circles?
// same as reduced map box?
// white/bright white for reduced map mark, and GP/MP font color, and outline/aura color for unit enchantment outline?

// ¿ which palette ?
// Production Screen / Build Table
#define PAL_GREY_DARK  4  // ¿ font color ?
#define PAL_GREY_LIGHT 5  // ¿ font color ?

#define PAL_TURQUOISE_LIGHT  244
#define PAL_TURQUOISE_DARK   248
#define PAL_TURQUOISE_DIM    251
#define PAL_TURQUOISE_BRIGHT 255


/*
    EGA / VGA Colors
        $00 Black
        $01 Blue
        $02 Green
        $03 Cyan
        $04 Red
        $05 Magenta
        $06 Brown
        $07 White
        $08 Gray
        $09 Light Blue
        $0A Light Green
        $0B Light Cyan
        $0C Light Red
        $0D Light Magenta
        $0E Yellow
        $0F Bright White
*/

#endif /* MOX_DEF_H */
