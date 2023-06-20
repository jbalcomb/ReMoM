
#include "MoM_DEF.H"
#include "MoX_DEF.H"
#include "MoX_TYPE.H"

/*
    Spell Casting Screen

    WIZARDS.EXE
    ovr070
*/

// WZD o70p01

// WZD o70p02

/*
    X,Y of Movement Map, in Squares (top-left corner)
    X,Y of Unit on World Map, in Squares
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: World_To_Screen(map_xw = %d, map_yw = %d, *unit_xw = %d, *unit_yw = %d)\n", __FILE__, __LINE__, map_xw, map_yw, *unit_xw, *unit_yw);
#endif

    in_view = ST_FALSE;

    if( (*unit_yw >= map_yw) && (*unit_yw <= (map_yw + MMAP_WIDTH)) )
    {
        unit_ym = *unit_yw - map_yw;  // e.g, unit is at 37,21 and move-map is at 29,12; this Y is 21-12=9, which is Y relative to the move-map viewport 

        if((*unit_xw >= map_xw) && (*unit_xw <= (map_xw + MMAP_HEIGHT)))
        {
            in_view = ST_TRUE;
        }
        else
        {
            *unit_xw += WMAP_WIDTH;
            if((*unit_xw >= map_xw) && (*unit_xw <= (map_xw + MMAP_HEIGHT)))
            {
                in_view = ST_TRUE;
            }
        }
    }

    if(in_view == ST_TRUE)
    {
        *unit_xw = *unit_xw * SQUARE_WIDTH;  // movement-map square width, pixels
        *unit_yw = (unit_ym * SQUARE_HEIGHT) + (*unit_xw + SQUARE_WIDTH);  // movement-map square height, in pixels
    }
    else
    {
        *unit_xw = 0;  // ? ST_NULL ?
        *unit_yw = 0;  // ? ST_NULL ?
    }

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
