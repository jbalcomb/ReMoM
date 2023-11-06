
#include "MoX.H"
#include "SCastScr.H"



/*
    Spell Casting Screen

    WIZARDS.EXE
    ovr070
*/

// WZD o70p01

// WZD o70p02

/*
    map_xw: World X Coordinate of the first square in the Movement Map
    map_yw: World Y Coordinate of the first square in the Movement Map
    unit_xw: World X Coordinate of the Unit
    unit_yw: World Y Coordinate of the Unit
    unit_xm: Movement Map X Coordinate of the Unit
    unit_ym: Movement Map Y Coordinate of the Unit

    X,Y of Movement Map, in Squares (top-left corner)
    X,Y of Unit on World Map, in Squares

    Unit Is In-View
        Translate from World Coordinates to Map Coordinates

e.g.,

BEGIN: World_To_Screen(map_xw = 7, map_yw = 0, *unit_xw = 18, *unit_yw = 10)

(*unit_xw = 18) >= (map_xw = 7)
(*unit_xw = 18) <=  ((map_xw = 7) + (MAP_WIDTH = 12) = 19)

(*unit_yw = 10) >= (map_yw = 0)
(*unit_yw = 10) <= ((map_yw = 0) + (MAP_HEIGHT = 10) = 10)

Unit Map X,Y:
    unit_xm = *unit_xw - map_xw := (18 - 7) = 11
    unit_ym = *unit_yw - map_yw := (10 - 0) = 10

Unit Screen X,Y:
    (*unit_xw = 220) := ((*unit_xm = 11) * (SQUARE_WIDTH = 20)) = 220
    (*unit_yw = 200) := ((unit_ym = 10) * (SQUARE_HEIGHT = 18)) + (SQUARE_WIDTH = 20) = 200


END: World_To_Screen(map_xw = 7, map_yw = 0, *unit_xw = 220, *unit_yw = 200) {in_view = 1}

WTF?
unit_yw = 10
unit_ym =  9
unit_yw = 182
(unit_ym * SQUARE_HEIGHT) + SQUARE_WIDTH
9 * 18 + 20 = 162 + 20 = 182

12 * 20 - 20 = 240 - 20 = 220
10 * 18 - 18 = 180 - 18 = 162  ... + 20 = 182


*/
/*

╔═════╦═════╗
║     ║     ║
║     ║     ║
╠═════╬═════╣
║     ║     ║
║     ║     ║
╚═════╩═════╝

╔═════╤═════╗
║     │     ║
║     │     ║
╟─────┼─────╢
║     │     ║
║     │     ║
╚═════╧═════╝

World
60 x 40
{0, ..., 59}
{0, ..., 39}
Map
12 x 10
{0, ..., 11}
{0, ...,  9}

  wx   +1   +2   +3   +4   +5   +6   +7   +7   +7  +10  +11
   0    1    2    3    4    5    6    7    8    9   10   11
╔════╤════╤════╤════╤════╤════╤════╤════╤════╤════╤════╤════╗
║    │    │    │    │    │    │    │    │    │    │    │    ║  1
╟────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────╢
║    │    │    │    │    │    │    │    │    │    │    │    ║  2
╟────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────╢
║    │    │    │    │    │    │    │    │    │    │    │    ║  3
╟────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────╢
║    │    │    │    │    │    │    │    │    │    │    │    ║  4
╟────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────╢
║    │    │    │    │    │    │    │    │    │    │    │    ║  5
╟────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────╢
║    │    │    │    │    │    │    │    │    │    │    │    ║  6
╟────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────╢
║    │    │    │    │    │    │    │    │    │    │    │    ║  7
╟────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────╢
║    │    │    │    │    │    │    │    │    │    │    │    ║  8
╟────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────╢
║    │    │    │    │    │    │    │    │    │    │    │    ║  9
╟────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────╢
║    │    │    │    │    │    │    │    │    │    │    │    ║ 10
╚════╧════╧════╧════╧════╧════╧════╧════╧════╧════╧════╧════╝
   1    2    3    4    5    6    7    8    9   10   11   12

*/
// WZD o70p03
// AKA OVL_Tile2DrawCoords
/*
[-] Draw_Active_Unit_Stack
OVL_DrawPath
OVL_MoveUnitStack
OVL_MoveUnitStack
OVL_MoveUnitStack
OVL_MoveUnitStack
sub_AFCA8
sub_B0C07
sub_B517B
sub_B529D
sub_B529D
sub_BA1AF
sub_BF048
*/
int16_t World_To_Screen(int16_t map_xw, int16_t map_yw, int16_t * unit_xw, int16_t * unit_yw)
{
    int16_t in_view;
    int16_t unit_ym;
    int16_t unit_xm;
#ifdef STU_DEBUG
    int16_t ORIG_unit_xw;
    int16_t ORIG_unit_yw;
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: World_To_Screen(map_xw = %d, map_yw = %d, *unit_xw = %d, *unit_yw = %d)\n", __FILE__, __LINE__, map_xw, map_yw, *unit_xw, *unit_yw);
#endif
    // TODO  fix your brain
    //       ...or just use Min,Max X,Y
    // assert( map_xw  >= 0 &&  map_xw  <= WORLD_WIDTH);
    // assert( map_yw  >= 0 &&  map_yw  <= WORLD_HEIGHT);
    // assert(*unit_xw >= 0 && *unit_xw <= WORLD_HEIGHT);
    // assert(*unit_yw >= 0 && *unit_yw <= WORLD_HEIGHT);
    assert( map_xw  >= WORLD_X_MIN &&  map_xw  <= WORLD_X_MAX);
    assert( map_yw  >= WORLD_Y_MIN &&  map_yw  <= WORLD_Y_MAX);
    assert(*unit_xw >= WORLD_X_MIN && *unit_xw <= WORLD_X_MAX);
    assert(*unit_yw >= WORLD_Y_MIN && *unit_yw <= WORLD_Y_MAX);

#ifdef STU_DEBUG
    ORIG_unit_xw = *unit_xw;
    ORIG_unit_yw = *unit_yw;
#endif

    in_view = ST_FALSE;

    // if( (*unit_yw >= map_yw) && (*unit_yw <= (map_yw + MAP_HEIGHT)) )
    if( (*unit_yw >= map_yw) && (*unit_yw <= (map_yw + MAP_Y_MAX)) )
    {
        unit_ym = *unit_yw - map_yw;  // e.g, unit is at 37,21 and move-map is at 29,12; this Y is 21-12=9, which is Y relative to the move-map viewport 

        // if( (*unit_xw >= map_xw) && (*unit_xw <= (map_xw + MAP_WIDTH)) )
        if( (*unit_xw >= map_xw) && (*unit_xw <= (map_xw + MAP_X_MAX)) )
        {
            unit_xm = *unit_xw - map_xw;
            in_view = ST_TRUE;
        }
        else
        {
            *unit_xw += WORLD_WIDTH;
            // if((*unit_xw >= map_xw) && (*unit_xw <= (map_xw + MAP_WIDTH)))
            if((*unit_xw >= map_xw) && (*unit_xw <= (map_xw + MAP_X_MAX)))
            {
                unit_xm = *unit_xw - map_xw;
                in_view = ST_TRUE;
            }
        }
    }

    if(in_view == ST_TRUE)
    {
        // *unit_xw = (unit_xm * SQUARE_WIDTH);  // movement-map square width, pixels
        // *unit_yw = (unit_ym * SQUARE_HEIGHT) + SQUARE_WIDTH;  // movement-map square height, in pixels
        *unit_xw = MAP_SCREEN_X + (unit_xm * SQUARE_WIDTH);  // movement-map square width, pixels
        *unit_yw = MAP_SCREEN_Y + (unit_ym * SQUARE_HEIGHT);  // movement-map square height, in pixels
    }
    else
    {
        *unit_xw = 0;  // ? ST_NULL ?
        *unit_yw = 0;  // ? ST_NULL ?
    }

    // assert(*unit_xw >= 0 && *unit_xw <= SCREEN_WIDTH);
    // assert(*unit_yw >= 0 && *unit_yw <= SCREEN_HEIGHT);
    // maybe don't allow for screen coordinates that would allow the drawing to end up off the screen?
    // {0, ..., 319}  320 pixels wide  max: draw 20 pixels, end at 319  {300, ..., 319}
    // {0, ..., 199}  200 pixels high  max: draw 18 pixels, end at 199  {180, ..., 199}
    // assert(*unit_xw >= 0 && *unit_xw <= (SCREEN_WIDTH  - SQUARE_WIDTH ));
    // assert(*unit_yw >= 0 && *unit_yw <= (SCREEN_HEIGHT - SQUARE_HEIGHT));
    assert(*unit_xw >= MAP_SQUARE_SCREEN_X_MIN && *unit_xw <= MAP_SQUARE_SCREEN_X_MAX);
    assert(*unit_yw >= MAP_SQUARE_SCREEN_Y_MIN && *unit_yw <= MAP_SQUARE_SCREEN_Y_MAX);


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: World_To_Screen(map_xw = %d, map_yw = %d, *unit_xw = %d, *unit_yw = %d) {in_view = %d}\n", __FILE__, __LINE__, map_xw, map_yw, *unit_xw, *unit_yw, in_view);
#endif

    return in_view;
}

// WZD o70p04

// WZD o70p05

// WZD o70p06

// WZD o70p07

// WZD o70p08

// WZD o70p09

// WZD o70p10

// WZD o70p11

// WZD o70p12

// WZD o70p13

// WZD o70p14
