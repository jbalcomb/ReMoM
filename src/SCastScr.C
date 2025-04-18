#include "SCastScr.H"

#include "MOX/MOX_DEF.H"
#include "MOX/MOX_TYPE.H"

#include "MOM_DEF.H"

#include <assert.h>



/*
    Spell Casting Screen

    WIZARDS.EXE
    ovr070
*/

// WZD o70p01

// WZD o70p02

// WZD o70p03
// AKA OVL_Tile2DrawCoords
/*
; converts the tile coordinates at the passed pointer
; locations into draw pixel coordinates for the large
; overland 12x10 map display; returning 1 if the tile
; is visible in the map window, or 0 if it is not
*/
/*
World_To_Screen()
XREF:
    j_World_To_Screen()
        Draw_Active_Stack_Movement_Path()
        Draw_Active_Unit_Stack()
        Move_Units_Draw()
        Move_Units_Draw()
        Move_Units_Draw()
        Move_Units_Draw()
        Cast_ChangeTerain()
        Cast_Corruption()
        Cast_PlaneShift_DrawSwitchPlane()
        IDK_SplCst_sB529D()
        IDK_SplCst_sB529D()
        IDK_SpellAnim_Screen_Driver()
        IDK_Spell_Cityscape_1()
*/
/*
    returns in_view {F,T}
    in-outs Unit's World X,Y to Screen X,Y
*/
int16_t World_To_Screen(int16_t map_wx, int16_t map_wy, int16_t * unit_wx, int16_t * unit_wy)
{
    int16_t in_view;
    int16_t unit_my;
    int16_t unit_mx;
    int16_t screen_y;  // DNE in Dasm
    int16_t screen_x;  // DNE in Dasm

    assert( map_wx  >= WORLD_XMIN &&  map_wx  <= WORLD_XMAX);  /*  0 & 59 */
    assert( map_wy  >= WORLD_YMIN &&  map_wy  <= WORLD_YMAX);  /*  0 & 39 */
    assert(*unit_wx >= WORLD_XMIN && *unit_wx <= WORLD_XMAX);  /*  0 & 59 */
    assert(*unit_wy >= WORLD_YMIN && *unit_wy <= WORLD_YMAX);  /*  0 & 39 */

    in_view = ST_FALSE;

    if(
        (*unit_wy >= map_wy)
        &&
        (*unit_wy <= (map_wy + MAP_Y_MAX))
    )
    {
        unit_my = *unit_wy - map_wy;  // e.g, unit is at 37,21 and move-map is at 29,12; this Y is 21-12=9, which is Y relative to the move-map viewport 

        if( (*unit_wx >= map_wx) && (*unit_wx <= (map_wx + MAP_X_MAX)) )
        {
            unit_mx = *unit_wx - map_wx;
            in_view = ST_TRUE;
        }
        else
        {
            *unit_wx += WORLD_WIDTH;
            if((*unit_wx >= map_wx) && (*unit_wx <= (map_wx + MAP_X_MAX)))
            {
                unit_mx = *unit_wx - map_wx;
                in_view = ST_TRUE;
            }
        }
    }

    if(in_view == ST_TRUE)
    {
        screen_x = MAP_SCREEN_X + (unit_mx * SQUARE_WIDTH);
        screen_y = MAP_SCREEN_Y + (unit_my * SQUARE_HEIGHT);
    }
    else
    {
        screen_x = 0;
        screen_y = 0;
    }

    assert(screen_x >= MAP_SQUARE_SCREEN_X_MIN && screen_x <= MAP_SQUARE_SCREEN_X_MAX);
    assert(screen_y >= MAP_SQUARE_SCREEN_Y_MIN && screen_y <= MAP_SQUARE_SCREEN_Y_MAX);

    *unit_wx = screen_x;
    *unit_wy = screen_y;

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
